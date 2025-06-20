#include <iostream>
#include <unistd.h>

//#include "sgo_sub_pub.h"
#include "sgo_func.h"
#include "sgo_ams/compressed_air_system.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
//#include "compressed_air_system.h"

using namespace std;

const char* compressed_air_system_str[]={
#ifndef MAHTS648
	,
	"no2_air_compressor_common_alarm",
	"service_air_compressor_common_alarm",
	"quick_closing_valve_air_reservoir_low_pressure",
	"air_dryer_common_alarm",
	"no1_main_air_reservoir_pressure",
	"no2_main_air_reservoir_pressure",
	"daily_air_reservoir_pressure",
	"no1_main_deck_air_reservoir_pressure",
	"no2_main_deck_air_reservoir_pressure",
	"no3_main_deck_air_reservoir_pressure",
	"no4_main_deck_air_reservoir_pressure",	
	"device_offline",
#else
"no1_air_compressor_common_alarm",
"no2_air_compressor_common_alarm",
"service_air_compressor_common_alarm",
"no1_main_air_reservoir_pressure",
"no2_main_air_reservoir_pressure",
"daily_air_reservoir_pressure",
"quick_closing_valve_air_reservoir_low_pressure",
"air_dryer_common_alarm",
"device_offline",


#endif
};
	
static const char* productCode = "compressed_air_system";

COMPRESSED_AIR_SYSTEM::COMPRESSED_AIR_SYSTEM()
{}

COMPRESSED_AIR_SYSTEM::~COMPRESSED_AIR_SYSTEM()
{}

void COMPRESSED_AIR_SYSTEM::init(int devId,BlockQueue<compressed_air_system> &bq,modbus* mb,mutex *mux)
{
	m_mb_plc1 = mb;
	m_mux_plc1 = mux;

	m_clientId.clear();
	m_clientId.append(productCode);
	m_clientId.append("_device");
	m_clientId.append(to_string(devId));

	m_devId = devId;
	m_productCode = productCode;
	
	//init_mb_addr();	
	
	setSubTopics();
	setPubTopics();
	
	start(bq);

}



// 连接丢失回调函数
void COMPRESSED_AIR_SYSTEM::connectionLostCallback(void* context, char* cause) 
{
	// 处理连接丢失事件
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	COMPRESSED_AIR_SYSTEM* client = static_cast<COMPRESSED_AIR_SYSTEM*>(ctx->manager);
	
	printf("--%s %d--address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());
	sgo_log_stderr(0,"--%s %d----address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());

	//std::lock_guard<std::mutex> lock(m_losts_mutex);
	client->m_v_lost[ctx->index] = 1;
	client->m_v_property_index_array.clear();
}


// 消息到达回调函数
int COMPRESSED_AIR_SYSTEM::messageArrivedCallback(void* context, char* topicName, int topicLen,MQTTClient_message* message) 
{
	// 处理接收到的消息
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	COMPRESSED_AIR_SYSTEM* client = static_cast<COMPRESSED_AIR_SYSTEM*>(ctx->manager);
	
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
			client->m_property[compressed_air_system_str[i]]=i;
		
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
void COMPRESSED_AIR_SYSTEM::deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt) 
{
	// 处理交付完成事件
	//std::cout << "Message delivery complete." << std::endl;
}

void COMPRESSED_AIR_SYSTEM::subClient()
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


void COMPRESSED_AIR_SYSTEM::pubClient(BlockQueue<compressed_air_system>& bq)
{
	while(1)
	{
		MQTTClient_message pubmsg = MQTTClient_message_initializer;
		//声明消息token
		MQTTClient_deliveryToken token;

		while(!m_pubStop){
			compressed_air_system one = bq.take();
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



void COMPRESSED_AIR_SYSTEM::start(BlockQueue<compressed_air_system> &bq)
{
	m_sub = new thread(&COMPRESSED_AIR_SYSTEM::subClient,this);
	m_sub->detach();

	m_pub = new thread(&COMPRESSED_AIR_SYSTEM::pubClient,this,ref(bq));
	m_pub->detach();
	
	m_thr_reconn = new thread(&COMPRESSED_AIR_SYSTEM::reconn,this);
	m_thr_reconn->detach();

	m_thr_resend = new thread(&COMPRESSED_AIR_SYSTEM::resnd,this);
	m_thr_resend->detach();

	m_thr_chksensor = new thread(&COMPRESSED_AIR_SYSTEM::chksensor,this);
	m_thr_chksensor->detach();

	thread *thr_range = new thread(&COMPRESSED_AIR_SYSTEM::thSetRange,this);
	thr_range->detach();

	thread *thr_threshold = new thread(&COMPRESSED_AIR_SYSTEM::thSetThreshold,this);
	thr_threshold->detach();

	thread *thr_inhibit = new thread(&COMPRESSED_AIR_SYSTEM::thSetInhibit,this);
	thr_inhibit->detach();

	thread *thr_delay = new thread(&COMPRESSED_AIR_SYSTEM::thSetDelay,this);
	thr_delay->detach();

}

bool COMPRESSED_AIR_SYSTEM::UpdateLa(std::string key, uint16_t *data, int size)
{
	bool ret = false;
	for(int i = 0; i < sizeof(compressed_air_system_str); ++i)
	{
        std::string s(compressed_air_system_str[i]);
		if(s == key)
		{
			std::pair<uint8_t, uint8_t> byte_bit{0xff, 0xff};
            switch(i)
			{
            case 0:{byte_bit = m_data_store.no1_air_compressor_common_alarm.la_bit;break;}
            case 1:{byte_bit = m_data_store.no2_air_compressor_common_alarm.la_bit;break;}
            case 2:{byte_bit = m_data_store.service_air_compressor_common_alarm.la_bit;break;}
            case 3:{byte_bit = m_data_store.no1_main_air_reservoir_pressure.la_bit;break;}
            case 4:{byte_bit = m_data_store.no2_main_air_reservoir_pressure.la_bit;break;}
            case 5:{byte_bit = m_data_store.daily_air_reservoir_pressure.la_bit;break;}
            case 6:{byte_bit = m_data_store.quick_closing_valve_air_reservoir_low_pressure.la_bit;break;}
            case 7:{byte_bit = m_data_store.air_dryer_common_alarm.la_bit;break;}
            case 8:{byte_bit = m_data_store.device_offline.la_bit;break;}
			}

			if(byte_bit.first < size && byte_bit.second < 16)
			{
                data[byte_bit.first] |= (1 << byte_bit.second);
				ret = true;
			}

			break;
		}
	}

	return ret;
}
void COMPRESSED_AIR_SYSTEM::chksensor()
{
	checkSensorStatus();
}

void COMPRESSED_AIR_SYSTEM::resnd()
{
	resend();
}

void COMPRESSED_AIR_SYSTEM::reconn()
{
	reconnect();
}



string COMPRESSED_AIR_SYSTEM::property_reply(char flag,const compressed_air_system& one,const string& msg_id)
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
#ifndef MAHTS648
			case 0:writer.Int(one.no1_air_compressor_common_alarm);break;
			case 1:writer.Int(one.no2_air_compressor_common_alarm);break;
			case 2:writer.Int(one.service_air_compressor_common_alarm);break;
			case 3:writer.Int(one.quick_closing_valve_air_reservoir_low_pressure);break;
			case 4:writer.Int(one.air_dryer_common_alarm);break;
			case 5:writer.Double(one.no1_main_air_reservoir_pressure);break;
			case 6:writer.Double(one.no2_main_air_reservoir_pressure);break;
			case 7:writer.Double(one.daily_air_reservoir_pressure);break;
			case 8:writer.Double(one.no1_main_deck_air_reservoir_pressure);break;
			case 9:writer.Double(one.no2_main_deck_air_reservoir_pressure);break;
			case 10:writer.Double(one.no3_main_deck_air_reservoir_pressure);break;
			case 11:writer.Double(one.no4_main_deck_air_reservoir_pressure);break;
			case 12:writer.Double(one.device_offline);break;
#else
            case 0:writer.Int(one.no1_air_compressor_common_alarm);break;
            case 1:writer.Int(one.no2_air_compressor_common_alarm);break;
            case 2:writer.Int(one.service_air_compressor_common_alarm);break;
            case 3:writer.Double(one.no1_main_air_reservoir_pressure);break;
            case 4:writer.Double(one.no2_main_air_reservoir_pressure);break;
            case 5:writer.Double(one.daily_air_reservoir_pressure);break;
            case 6:writer.Int(one.quick_closing_valve_air_reservoir_low_pressure);break;
            case 7:writer.Int(one.air_dryer_common_alarm);break;
            case 8:writer.Int(one.device_offline);break;

#endif
			default:
				break;break;
		}
	}

	writer.EndArray();		
#else

	writer.Key("properties");
	writer.StartObject();

	int i=0;
	writer.Key(compressed_air_system_str[i++]);writer.Int(one.no1_air_compressor_common_alarm);
	writer.Key(compressed_air_system_str[i++]);writer.Int(one.no2_air_compressor_common_alarm);
	writer.Key(compressed_air_system_str[i++]);writer.Int(one.service_air_compressor_common_alarm);
	writer.Key(compressed_air_system_str[i++]);writer.Int(one.quick_closing_valve_air_reservoir_low_pressure);
	writer.Key(compressed_air_system_str[i++]);writer.Int(one.air_dryer_common_alarm);
	writer.Key(compressed_air_system_str[i++]);writer.Double(one.no1_main_air_reservoir_pressure);
	writer.Key(compressed_air_system_str[i++]);writer.Double(one.no2_main_air_reservoir_pressure);
	writer.Key(compressed_air_system_str[i++]);writer.Double(one.daily_air_reservoir_pressure);

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


void COMPRESSED_AIR_SYSTEM::thSetRange()
{
	while(1)
	{
		range one = bq_range.take();
		string key = one.m_keyname;
		float start = one.start;
		float end = one.end;

		std::pair<float, float> rg = std::make_pair(start, end);

	    for(int i = 0; i < sizeof(compressed_air_system_str); ++i)
	    {
            std::string s(compressed_air_system_str[i]);
		    if(s == key)
		    {
                switch(i)
		    	{
                    case 0:{m_data_store.no1_air_compressor_common_alarm.range=rg;break;}
                    case 1:{m_data_store.no2_air_compressor_common_alarm.range=rg;break;}
                    case 2:{m_data_store.service_air_compressor_common_alarm.range=rg;break;}
                    case 3:{m_data_store.no1_main_air_reservoir_pressure.range=rg;break;}
                    case 4:{m_data_store.no2_main_air_reservoir_pressure.range=rg;break;}
                    case 5:{m_data_store.daily_air_reservoir_pressure.range=rg;break;}
                    case 6:{m_data_store.quick_closing_valve_air_reservoir_low_pressure.range=rg;break;}
                    case 7:{m_data_store.air_dryer_common_alarm.range=rg;break;}
                    case 8:{m_data_store.device_offline.range=rg;break;}       
		    	}
    
		    	break;
		    }
	    }       
	}
	//setRange();
}

void COMPRESSED_AIR_SYSTEM::thSetThreshold()
{
	setThreshold();
}

void COMPRESSED_AIR_SYSTEM::thSetDelay()
{
	setDelay();
}


void COMPRESSED_AIR_SYSTEM::thSetInhibit()
{
    while (1)
	{
		inhibit one = bq_inhibit.take();
		string key = one.m_keyname;
		bool inhib = one.is_inhibit;
        for(int i = 0; i < sizeof(compressed_air_system_str); ++i)
	    {
            std::string s(compressed_air_system_str[i]);
		    if(s == key)
		    {
                switch(i)
		    	{
                    case 0:{m_data_store.no1_air_compressor_common_alarm.inhibit = inhib;break;}
                    case 1:{m_data_store.no2_air_compressor_common_alarm.inhibit = inhib;break;}
                    case 2:{m_data_store.service_air_compressor_common_alarm.inhibit = inhib;break;}
                    case 3:{m_data_store.no1_main_air_reservoir_pressure.inhibit = inhib;break;}
                    case 4:{m_data_store.no2_main_air_reservoir_pressure.inhibit = inhib;break;}
                    case 5:{m_data_store.daily_air_reservoir_pressure.inhibit = inhib;break;}
                    case 6:{m_data_store.quick_closing_valve_air_reservoir_low_pressure.inhibit = inhib;break;}
                    case 7:{m_data_store.air_dryer_common_alarm.inhibit = inhib;break;}
                    case 8:{m_data_store.device_offline.inhibit = inhib;break;}
		    	}
    
		    	break;
		    }
	    }       
	}

	//setInhibit(5803,2);
}

void COMPRESSED_AIR_SYSTEM::init_mb_addr()
{
	for(int i = 0; i < 3;i++)
	{
		m_mp_delay_addr[compressed_air_system_str[i]] = 8052 + i;
		m_mp_inhibit_addr[compressed_air_system_str[i]] = i;
	}
	for(int i = 0; i < 2;i++)
	{
		m_mp_delay_addr[compressed_air_system_str[3 + i]] = 8096 + i;
		m_mp_inhibit_addr[compressed_air_system_str[3 + i]] = 212 + i;
	}

	//4-20mA
	int addr_ai = 9828;
	for(int i = 0; i < 3; i++)
		m_mp_range_addr[compressed_air_system_str[5 + i]] = addr_ai + 2 * i;

	
}


