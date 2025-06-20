#include <iostream>
#include <unistd.h>

//#include "sgo_sub_pub.h"
#include "sgo_func.h"
#include "buzzer.h"
#include "modbus.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

using namespace std;

const char* buzzer_str[]={
	"mute",
	"ack",
	"status",
	"device_offline",
};
	
static const char* productCode = "buzzer";

BUZZER::BUZZER()
{}

BUZZER::~BUZZER()
{}

void BUZZER::init(int devId,BlockQueue<buzzer> &bq,modbus *mb1,mutex *mux1)
{
	m_mb_plc1 = mb1;
	m_mux_plc1 = mux1;	

	m_clientId.clear();
	m_clientId.append(productCode);
	m_clientId.append("_device");
	m_clientId.append(to_string(devId));

	m_devId = devId;
	m_productCode = productCode;

	setSubTopics();
	setPubTopics();
	
	start(bq);

}



// 连接丢失回调函数
void BUZZER::connectionLostCallback(void* context, char* cause) 
{
	// 处理连接丢失事件
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	BUZZER* client = static_cast<BUZZER*>(ctx->manager);
	
	printf("--%s %d--address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());
	sgo_log_stderr(0,"--%s %d----address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());

	//std::lock_guard<std::mutex> lock(m_losts_mutex);
	client->m_v_lost[ctx->index] = 1;
	client->m_v_property_index_array.clear();
}


// 消息到达回调函数
int BUZZER::messageArrivedCallback(void* context, char* topicName, int topicLen,MQTTClient_message* message) 
{
	// 处理接收到的消息
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	BUZZER* client = static_cast<BUZZER*>(ctx->manager);

	//printf("--%s %d--address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());
	//sgo_log_stderr(0,"--%s %d----address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());


	std::lock_guard<std::mutex> lock(client->m_mux_arrive);
    char* payloadptr = (char*)message->payload;
	//cout<<"payload:"<<payloadptr<<endl;

	if(0 == client->m_subs[0].compare(topicName))
	{
		if(!client->m_v_property_index_array.empty())
			goto exit;
	
		client->parseProp(payloadptr);
		if(client->m_readType != "PROPERTY_REPORT") //回复
		{
			if(strstr(topicName,"properties"))
			{
				string payload = client->property_reply(0,client->m_cur_data);
				client->sendMessage(payload,0);
			}

		}

	}else if (0 == client->m_subs[1].compare(topicName)) //invoke
	{
		client->parseInvoke(payloadptr);
		//cout<<client->m_funcKey << " "<<client->m_deviceCode<<endl;
		client->m_func_res = false;
		if(client->m_funcKey.compare("turnOnBuzzer") == 0)
			client->turnOnBuzzer();
		if(client->m_funcKey.compare("turnOffBuzzer") == 0)
			client->turnOffBuzzer();		
		

	}


exit:
	// 记得释放消息
	MQTTClient_freeMessage(&message);
	MQTTClient_free(topicName);
	
	return 1;
}

// 交付完成回调函数
void BUZZER::deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt) 
{
	// 处理交付完成事件
	//std::cout << "Message delivery complete." << std::endl;
}

void BUZZER::subClient()
{
    int rc;
    int ch;

	m_v_clients.clear();
	for(int i = 0;i < m_v_address.size();i++)
	{
		MQTTClient client;
		MQTTClient_create(&client, m_v_address[i].c_str(), m_clientId.c_str(), MQTTCLIENT_PERSISTENCE_NONE, NULL);
		auto* context = new CallbackContext{this, i};		
		MQTTClient_setCallbacks(client, context, connectionLostCallback, messageArrivedCallback, deliveryCompleteCallback);

		m_v_clients.push_back(client);		
	}

	sub();
	
	while(1)
	{
		MQTTClient_yield();
	}

	for(int i = 0;i < m_v_address.size();i++)
	{
		for(int j = 0; j < m_subs.size(); j++)
		   	MQTTClient_unsubscribe(m_v_clients[i], m_subs[j].c_str());
	    MQTTClient_disconnect(m_v_clients[i], 10000);
	   	MQTTClient_destroy(&m_v_clients[i]);
	}
	


}


void BUZZER::pubClient(BlockQueue<buzzer>& bq)
{
	while(1)
	{
		MQTTClient_message pubmsg = MQTTClient_message_initializer;
		//声明消息token
		MQTTClient_deliveryToken token;

		while(!m_pubStop){
			buzzer one = bq.take();
			/*{
				std::lock_guard<std::mutex> lock(mux_cur_data);
				memcpy(&m_cur_data,&one,sizeof(one));
			}*/
	
			string payload = property_reply(1,one);
			if(payload.empty())
				continue;

			pubmsg.payload = (void*)payload.c_str();
			pubmsg.payloadlen = payload.length();
			pubmsg.qos = 1;
			pubmsg.retained = 0;

			for(int i = 0; i < m_v_address.size();i++)
			{
				if(0 == m_v_lost[i])
					MQTTClient_publishMessage(m_v_clients[i], m_pubs[m_report_pub_index].c_str(), &pubmsg, &token);
				else
					m_bq_resend[i].offer(payload);
			}


		}		
		sleep(3);
	}
}



void BUZZER::start(BlockQueue<buzzer> &bq)
{
	m_sub = new thread(&BUZZER::subClient,this);
	m_sub->detach();

	m_pub = new thread(&BUZZER::pubClient,this,ref(bq));
	m_pub->detach();
	
	m_thr_reconn = new thread(&BUZZER::reconn,this);
	m_thr_reconn->detach();

	m_thr_resend = new thread(&BUZZER::resnd,this);
	m_thr_resend->detach();

	m_thr_chksensor = new thread(&BUZZER::chksensor,this);
	m_thr_chksensor->detach();
}

void BUZZER::chksensor()
{
	checkSensorStatus();
}

void BUZZER::resnd()
{
	resend();
}

void BUZZER::reconn()
{
	reconnect();
}



string BUZZER::property_reply(char flag,const buzzer& one,const string& msg_id)
{
    rapidjson::StringBuffer strBuf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(strBuf);
	
	string msgId;
	if(0 == flag)
		msgId = m_msgReplyId;
	else if(1 == flag)
		msgId = m_msgPubId;
	else
		msgId = msg_id;

	if(msgId.empty())
		msgId = generateFakeMsgId();
	
	if(m_deviceCode.empty())
		m_deviceCode = m_clientId;

    writer.StartObject();

if(0 == flag)
{
	writer.Key("funcKey");
	writer.String(m_funcKey.c_str());

	writer.Key("output");
	writer.StartObject();

	int i=0;
	writer.Key(m_vOutParams[i++].c_str());writer.Bool(m_func_res);
	writer.EndObject();

}
else
{
	writer.Key("msgId");
	writer.String(msgId.c_str());

	writer.Key("properties");
	writer.StartObject();
	int i=0;
	writer.Key(buzzer_str[i++]);writer.Int(one.mute);
	writer.Key(buzzer_str[i++]);writer.Int(one.ack);
	writer.Key(buzzer_str[i++]);writer.Int(one.status);
	writer.Key(buzzer_str[i++]);writer.Int(one.device_offline);
	writer.EndObject();
}

	writer.Key("success");
	writer.Bool(true);

	writer.Key("deviceCode");
	writer.String(m_deviceCode.c_str());

	writer.Key("isInvokeByRule");
	writer.Bool(m_isInvokeByRule);	

	if(flag)
	{
		writer.Key("readType");
		writer.String("PROPERTY_REPORT");
	}
	writer.Key("reportTime");
	writer.Int64(getSystemTime());

    writer.EndObject();
 
    string data = strBuf.GetString();
//cout << data << endl;
	return strBuf.GetString();
}

#if 0
bool BUZZER::operBuzzer(char flag)
{
	modbus *mb = new modbus(SAU_IP1,503);
	mb->modbus_set_slave_id(1);

	if(!mb->modbus_connect())
	{
		return false;
	}
	uint16_t buffer = 0;
	if(mb->modbus_read_holding_registers(79,1,&buffer))
		return false;

	if(flag)
		buffer |= (1 << (m_devId-1));
	else	
		buffer &= ~(1 << (m_devId-1));
	
	bool bRes = mb->modbus_write_register(79,buffer) ? false : true;
printf("buzzer buffer %x bres:%d\n",buffer,bRes);

	mb->modbus_close();
	return bRes;	
}
#else
void BUZZER::operBuzzer(char flag)
{
/*
	bool bRes = false;
printf("operBuzzer[%d]:%d\n",m_devId-1,flag);
return false;

	if(m_mb_plc1)
	{
		std::lock_guard<std::mutex> lock(*m_mux_plc1);
		if(flag == 0)	//silence
		{
			bRes = m_mb_plc1->modbus_write_register(8620+2*(m_devId-1),1) ? false : true;
			bRes = m_mb_plc1->modbus_write_register(8620+2*(m_devId-1),0) ? false : true;
		}
		if(flag == 1) //
		{
			uint16_t status = 0;
			m_mb_plc1->modbus_read_holding_registers(3002,1,&status);		
			status |= (1<<(m_devId-1));	
printf("before write buzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz%d\n",status);				
			bRes = m_mb_plc1->modbus_write_register(3002,status) ? false : true;
cout<<"1111after write buzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz"<<endl;
sleep(1);
			status &= ~(1<<(m_devId-1));	
			m_mb_plc1->modbus_write_register(3002,status); 
cout<<"2222after write buzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz"<<endl;			
		}
		
	}
	return bRes;*/	
}
#endif

void BUZZER::turnOnBuzzer()
{
	bq_oper_status.put(1);
	//return operBuzzer(1);
}

void BUZZER::turnOffBuzzer()
{
	bq_oper_status.put(0);

	//return operBuzzer(0);
}


