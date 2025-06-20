#include <iostream>
#include <unistd.h>

#include "sgo_func.h"
#include "extend_alarm_panel.h"
#include "modbus.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

using namespace std;

const char* extend_alarm_panel_str[]={
    "mute_or_ack",
    "status",
   "device_offline",
};

static const char* productCode = "extended_alarm_panel";

EXTEND_ALARM_PANEL::EXTEND_ALARM_PANEL()
{}

EXTEND_ALARM_PANEL::~EXTEND_ALARM_PANEL()
{}

void EXTEND_ALARM_PANEL::init(int devId,BlockQueue<extend_alarm_panel> &bq,modbus* mb1,mutex* mux1)
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
void EXTEND_ALARM_PANEL::connectionLostCallback(void* context, char* cause) 
{
	// 处理连接丢失事件
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	EXTEND_ALARM_PANEL* client = static_cast<EXTEND_ALARM_PANEL*>(ctx->manager);

	printf("--%s %d--address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());
	sgo_log_stderr(0,"--%s %d----address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());

	//std::lock_guard<std::mutex> lock(m_losts_mutex);
	client->m_v_lost[ctx->index] = 1;
	client->m_v_property_index_array.clear();
}


// 消息到达回调函数
int EXTEND_ALARM_PANEL::messageArrivedCallback(void* context, char* topicName, int topicLen,MQTTClient_message* message) 
{
	// 处理接收到的消息
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	EXTEND_ALARM_PANEL* client = static_cast<EXTEND_ALARM_PANEL*>(ctx->manager);

	//printf("--%s %d--address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());
	//sgo_log_stderr(0,"--%s %d----address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());


	std::lock_guard<std::mutex> lock(client->m_mux_arrive);
    char* payloadptr = (char*)message->payload;

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
	printf("ex [%d] invoke\n",client->m_devId);
		client->parseInvoke(payloadptr);

		//A10Q  40080(0.9-0.13) 延伸报警板的蜂鸣器
		/*	"chief",
		"chief office",
		"rest room",
		"meeting room",
		"restautant",*/

		client->m_func_res = false;



       
		if(0 == client->m_funcKey.compare("callExtendAlarmPanel"))
		{
			char turnOn = static_cast<char>(client->m_u32Call);       
            (client->bq_call_status).put(std::make_pair(1, turnOn));

		}


	}
	


exit:	

	// 记得释放消息
	MQTTClient_freeMessage(&message);
	MQTTClient_free(topicName);

	return 1;
}

// 交付完成回调函数
void EXTEND_ALARM_PANEL::deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt) 
{
	// 处理交付完成事件
	//std::cout << "Message delivery complete." << std::endl;
}

void EXTEND_ALARM_PANEL::subClient()
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


void EXTEND_ALARM_PANEL::pubClient(BlockQueue<extend_alarm_panel>& bq)
{
	while(1)
	{
		MQTTClient_message pubmsg = MQTTClient_message_initializer;
		//声明消息token
		MQTTClient_deliveryToken token;

		while(!m_pubStop){
			extend_alarm_panel one = bq.take();
			//memcpy(&m_cur_data,&one,sizeof(one));

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



void EXTEND_ALARM_PANEL::start(BlockQueue<extend_alarm_panel> &bq)
{
	m_sub = new thread(&EXTEND_ALARM_PANEL::subClient,this);
	m_sub->detach();

	m_pub = new thread(&EXTEND_ALARM_PANEL::pubClient,this,ref(bq));
	m_pub->detach();

	m_thr_reconn = new thread(&EXTEND_ALARM_PANEL::reconn,this);
	m_thr_reconn->detach();

	m_thr_resend = new thread(&EXTEND_ALARM_PANEL::resnd,this);
	m_thr_resend->detach();

	m_thr_chksensor = new thread(&EXTEND_ALARM_PANEL::chksensor,this);
	m_thr_chksensor->detach();

	thread* thr_setduty = new thread(&EXTEND_ALARM_PANEL::setduty,this);
	thr_setduty->detach();


}

void EXTEND_ALARM_PANEL::chksensor()
{
	checkSensorStatus();
}

void EXTEND_ALARM_PANEL::resnd()
{
	resend();
}

void EXTEND_ALARM_PANEL::reconn()
{
	reconnect();
}


void EXTEND_ALARM_PANEL::setduty()
{
	setDeviceDuty();
}

string EXTEND_ALARM_PANEL::property_reply(char flag,const extend_alarm_panel& one,const string& msg_id)
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

//	if(m_vOutParams.empty())
//		return strBuf.GetString();

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
		writer.Key(extend_alarm_panel_str[i++]);writer.Int(one.mute_or_ack);
		writer.Key(extend_alarm_panel_str[i++]);writer.Int(one.status);
		writer.Key(extend_alarm_panel_str[i++]);writer.Int(one.device_offline);
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
   // cout << data << endl;
	return strBuf.GetString();
}


void EXTEND_ALARM_PANEL::set_call_bq(std::pair<char, char> status)
{
     bq_call_status.put(status);
}




/*	{uint16_t buffer = 0;
	bool bRes = false;
	int addr_di;
	if(m_mb_plc1)
	{
	cout<<"in turnOnExtendedAlarmPanel"<<endl;
		std::lock_guard<std::mutex> lock(*m_mux_plc1);
		bRes = m_mb_plc1->modbus_write_register(3118 + m_devId,1);usleep(10);
		printf("bRes:%d\n",bRes);

		m_mb_plc1->modbus_write_register(3118+m_devId,0);

		cout<<"after 22222 turnOnExtendedAlarmPanel"<<endl;
	}
	return bRes;
}*/


