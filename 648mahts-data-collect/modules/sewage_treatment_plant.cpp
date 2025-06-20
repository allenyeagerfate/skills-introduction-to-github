#include <iostream>
#include <unistd.h>

#include "sgo_sub_pub.h"
#include "sgo_func.h"
#include "sgo_ams/sewage_treatment_plant.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

using namespace std;

const char* sewage_treatment_plant_str[]={
"sewage_treatment_plant_running",
"sewage_treatment_plant_common_fault",
"sewage_treatment_plant_buffer_tank_high_level",
"sewage_treatment_plant_cutting_pump_running",
"device_offline",

};

	
static const char* productCode = "sewage_treatment_plant";

SEWAGE_TREATMENT_PLANT::SEWAGE_TREATMENT_PLANT()
{}

SEWAGE_TREATMENT_PLANT::~SEWAGE_TREATMENT_PLANT()
{}

void SEWAGE_TREATMENT_PLANT::init(int devId,BlockQueue<sewage_treatment_plant> &bq,modbus* mb,mutex* mux)
{
	m_mb_plc1 = mb;
	m_mux_plc1 = mux;

	m_clientId.clear();
	m_clientId.append(productCode);
	m_clientId.append("_device");
	m_clientId.append(to_string(devId));

	m_devId = devId;
	//init_mb_addr();
	
	m_productCode = productCode;
	
	setSubTopics();
	setPubTopics();
	
	start(bq);

}



// 连接丢失回调函数
void SEWAGE_TREATMENT_PLANT::connectionLostCallback(void* context, char* cause) 
{
	// 处理连接丢失事件
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	SEWAGE_TREATMENT_PLANT* client = static_cast<SEWAGE_TREATMENT_PLANT*>(ctx->manager);
	
	printf("--%s %d--address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());
	sgo_log_stderr(0,"--%s %d----address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());

	//std::lock_guard<std::mutex> lock(m_losts_mutex);
	client->m_v_lost[ctx->index] = 1;
	client->m_v_property_index_array.clear();
}


// 消息到达回调函数
int SEWAGE_TREATMENT_PLANT::messageArrivedCallback(void* context, char* topicName, int topicLen,MQTTClient_message* message) 
{
	// 处理接收到的消息
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	SEWAGE_TREATMENT_PLANT* client = static_cast<SEWAGE_TREATMENT_PLANT*>(ctx->manager);
	
	printf("--%s %d--address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());
	sgo_log_stderr(0,"--%s %d----address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());


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
		for(int i = 0; i < client->m_property_size;i++)
			client->m_property[sewage_treatment_plant_str[i]]=i;
		
		client->m_v_index.resize(client->m_property_size);
		for(int i = 0; i < client->m_property_size;i++)
			client->m_v_index[i] = client->m_property[client->m_v_property_index_array[i].m_name];	
		
	}else if (0 == client->m_subs[1].compare(topicName)) //invoke
	{
		client->parseInvoke(payloadptr);
	}
	else
	{
	}


exit:
	
	// 记得释放消息
	MQTTClient_freeMessage(&message);
	MQTTClient_free(topicName);
	
	return 1;
}


// 交付完成回调函数
void SEWAGE_TREATMENT_PLANT::deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt) 
{
	// 处理交付完成事件
	//std::cout << "Message delivery complete." << std::endl;
}

void SEWAGE_TREATMENT_PLANT::subClient()
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


void SEWAGE_TREATMENT_PLANT::pubClient(BlockQueue<sewage_treatment_plant>& bq)
{
	while(1)
	{
		MQTTClient_message pubmsg = MQTTClient_message_initializer;
		//声明消息token
		MQTTClient_deliveryToken token;

		while(!m_pubStop){
			sewage_treatment_plant one = bq.take();
			{
				std::lock_guard<std::mutex> lock(mux_cur_data);
				memcpy(&m_cur_data,&one,sizeof(one));
			}

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



void SEWAGE_TREATMENT_PLANT::start(BlockQueue<sewage_treatment_plant> &bq)
{
	m_sub = new thread(&SEWAGE_TREATMENT_PLANT::subClient,this);
	m_sub->detach();

	m_pub = new thread(&SEWAGE_TREATMENT_PLANT::pubClient,this,ref(bq));
	m_pub->detach();

	m_thr_reconn = new thread(&SEWAGE_TREATMENT_PLANT::reconn,this);
	m_thr_reconn->detach();

	m_thr_resend = new thread(&SEWAGE_TREATMENT_PLANT::resnd,this);
	m_thr_resend->detach();

	m_thr_chksensor = new thread(&SEWAGE_TREATMENT_PLANT::chksensor,this);
	m_thr_chksensor->detach();

//	thread *thr_range = new thread(&SEWAGE_TREATMENT_PLANT::thSetRange,this);
//	thr_range->detach();

	thread *thr_threshold = new thread(&SEWAGE_TREATMENT_PLANT::thSetThreshold,this);
	thr_threshold->detach();

	thread *thr_inhibit = new thread(&SEWAGE_TREATMENT_PLANT::thSetInhibit,this);
	thr_inhibit->detach();

	thread *thr_delay = new thread(&SEWAGE_TREATMENT_PLANT::thSetDelay,this);
	thr_delay->detach();		
}

void SEWAGE_TREATMENT_PLANT::chksensor()
{
	checkSensorStatus();
}

void SEWAGE_TREATMENT_PLANT::resnd()
{
	resend();
}

void SEWAGE_TREATMENT_PLANT::reconn()
{
	reconnect();
}


string SEWAGE_TREATMENT_PLANT::property_reply(char flag,const sewage_treatment_plant& one,const string& msg_id)
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

	writer.Key("msgId");
	writer.String(msgId.c_str());

	writer.Key("success");
	writer.Bool(true);

#if 1
	if(0 == m_property_size)
		return string();

	writer.Key("compressData");
	writer.StartArray();

	for(int i = 0; i < m_v_index.size(); i++)
	{
		switch(m_v_index[i]){
            case 0:writer.Int(one.sewage_treatment_plant_running);break;
            case 1:writer.Int(one.sewage_treatment_plant_common_fault);break;
            case 2:writer.Int(one.sewage_treatment_plant_buffer_tank_high_level);break;
            case 3:writer.Int(one.sewage_treatment_plant_cutting_pump_running);break;
            case 4:writer.Int(one.device_offline);break;

			default:
				break;break;
		}
	}

	writer.EndArray();		
#else
	writer.Key("properties");
	writer.StartObject();

	int i=0;
	writer.Key(sewage_treatment_plant_str[i++]);writer.Int(one.b_t_remote_control_system_ac_power_failure);
	writer.Key(sewage_treatment_plant_str[i++]);writer.Int(one.b_t_remote_control_system_dc_power_failure);
	writer.Key(sewage_treatment_plant_str[i++]);writer.Int(one.b_t_remote_control_system_failure);
	writer.Key(sewage_treatment_plant_str[i++]);writer.Int(one.b_t_main_motor_overload);
	writer.Key(sewage_treatment_plant_str[i++]);writer.Int(one.b_t_main_motor_high_temperature);
	writer.Key(sewage_treatment_plant_str[i++]);writer.Int(one.b_t_oil_low_level);
	writer.Key(sewage_treatment_plant_str[i++]);writer.Int(one.b_t_oil_low_pressure);
	writer.Key(sewage_treatment_plant_str[i++]);writer.Int(one.b_t_oil_high_pressure);
	writer.Key(sewage_treatment_plant_str[i++]);writer.Int(one.b_t_main_motor_short_circuit);
	writer.Key(sewage_treatment_plant_str[i++]);writer.Int(one.b_t_emergency_stop_circuit_failure);
	writer.Key(sewage_treatment_plant_str[i++]);writer.Int(one.b_t_too_many_times_start);
	writer.Key(sewage_treatment_plant_str[i++]);writer.Int(one.b_t_integrated_alarm);

	writer.EndObject();
#endif
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



void SEWAGE_TREATMENT_PLANT::thSetRange()
{
	setRange();
}

void SEWAGE_TREATMENT_PLANT::thSetThreshold()
{
	setThreshold();
}

void SEWAGE_TREATMENT_PLANT::thSetDelay()
{
	setDelay();
}


void SEWAGE_TREATMENT_PLANT::thSetInhibit()
{
	setInhibit(5806,2);
}


void SEWAGE_TREATMENT_PLANT::init_mb_addr()
{
	int addr_ai = 8000;
	for(int i = 0; i < 3; i++)
		m_mp_delay_addr[sewage_treatment_plant_str[i]] = addr_ai + 102 + 3 * (m_devId - 1) + i;

	for(int i = 0; i < 4; i++)
	{
		m_mp_delay_addr[sewage_treatment_plant_str[3 + i]] = addr_ai + 108 + 9 * (m_devId - 1) + i;
		m_mp_delay_addr[sewage_treatment_plant_str[7 + i]] = addr_ai + 113 + 9 * (m_devId - 1) + i;
	}
	m_mp_delay_addr[sewage_treatment_plant_str[11]] = addr_ai + 112 + 9 * (m_devId - 1);

	//////////////////////////////////////////////////////
	if(m_devId == 0)
	{
		int i = 0;
		m_mp_inhibit_addr[sewage_treatment_plant_str[i++]] = 2;
		m_mp_inhibit_addr[sewage_treatment_plant_str[i++]] = 3;
		m_mp_inhibit_addr[sewage_treatment_plant_str[i++]] = 4;
		m_mp_inhibit_addr[sewage_treatment_plant_str[i++]] = 8;
		m_mp_inhibit_addr[sewage_treatment_plant_str[i++]] = 9;
		m_mp_inhibit_addr[sewage_treatment_plant_str[i++]] = 10;
		m_mp_inhibit_addr[sewage_treatment_plant_str[i++]] = 11;
		m_mp_inhibit_addr[sewage_treatment_plant_str[i++]] = 13;
		m_mp_inhibit_addr[sewage_treatment_plant_str[i++]] = 14;
		m_mp_inhibit_addr[sewage_treatment_plant_str[i++]] = 15;
		m_mp_inhibit_addr[sewage_treatment_plant_str[i++]] = 100;
		m_mp_inhibit_addr[sewage_treatment_plant_str[i++]] = 12;		
	}
	else
	{
		int i = 0;
		m_mp_inhibit_addr[sewage_treatment_plant_str[i++]] = 5;
		m_mp_inhibit_addr[sewage_treatment_plant_str[i++]] = 6;
		m_mp_inhibit_addr[sewage_treatment_plant_str[i++]] = 7;
		m_mp_inhibit_addr[sewage_treatment_plant_str[i++]] = 101;
		m_mp_inhibit_addr[sewage_treatment_plant_str[i++]] = 102;
		m_mp_inhibit_addr[sewage_treatment_plant_str[i++]] = 103;
		m_mp_inhibit_addr[sewage_treatment_plant_str[i++]] = 104;
		m_mp_inhibit_addr[sewage_treatment_plant_str[i++]] = 106;
		m_mp_inhibit_addr[sewage_treatment_plant_str[i++]] = 107;
		m_mp_inhibit_addr[sewage_treatment_plant_str[i++]] = 108;
		m_mp_inhibit_addr[sewage_treatment_plant_str[i++]] = 109;
		m_mp_inhibit_addr[sewage_treatment_plant_str[i++]] = 105;		
	}		
}





