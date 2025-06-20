#include <iostream>
#include <unistd.h>

#include "sgo_sub_pub.h"
#include "sgo_func.h"
#include "sgo_ams/main_transformer.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
//#include "main_transformer.h"

using namespace std;

const char* main_transformer_str[]={
#ifndef MAHTS648
	"main_transformer_u_winding_temp",
	"main_transformer_v_winding_temp",
	"main_transformer_w_winding_temp",
	"device_offline",
#else
    "transformer_door_openalarm",
    "transformer_thermostat_failure",
    "transformer_high_temp_alarm",
    "winding_r_temp",
    "winding_s_temp",
    "winding_t_temp",
    "mt_cooling_fan_stop",
    "mt_high_temp_high_trip_alarm",
    "mt_cooling_fan_overload",
    "mt_winding_d_temp_sensor_failure",
    "mt_winding_r_temp_sensor_failure",
    "mt_winding_s_temp_sensor_failure",
    "mt_winding_t_temp_sensor_failure",
    "device_offline",
#endif
};

	
static const char* productCode = "main_transformer";

MAIN_TRANSFORMER::MAIN_TRANSFORMER()
{}

MAIN_TRANSFORMER::~MAIN_TRANSFORMER()
{}

void MAIN_TRANSFORMER::init(int devId,BlockQueue<main_transformer> &bq,modbus *mb,mutex *mux)
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
void MAIN_TRANSFORMER::connectionLostCallback(void* context, char* cause) 
{
	// 处理连接丢失事件
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	MAIN_TRANSFORMER* client = static_cast<MAIN_TRANSFORMER*>(ctx->manager);
	
	printf("--%s %d--address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());
	sgo_log_stderr(0,"--%s %d----address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());

	//std::lock_guard<std::mutex> lock(m_losts_mutex);
	client->m_v_lost[ctx->index] = 1;
	client->m_v_property_index_array.clear();

}


// 消息到达回调函数
int MAIN_TRANSFORMER::messageArrivedCallback(void* context, char* topicName, int topicLen,MQTTClient_message* message) 
{
	// 处理接收到的消息
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	MAIN_TRANSFORMER* client = static_cast<MAIN_TRANSFORMER*>(ctx->manager);
	
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
			client->m_property[main_transformer_str[i]]=i;
		
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
void MAIN_TRANSFORMER::deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt) 
{
	// 处理交付完成事件
	//std::cout << "Message delivery complete." << std::endl;
}

void MAIN_TRANSFORMER::subClient()
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


void MAIN_TRANSFORMER::pubClient(BlockQueue<main_transformer>& bq)
{
	while(1)
	{
		MQTTClient_message pubmsg = MQTTClient_message_initializer;
		//声明消息token
		MQTTClient_deliveryToken token;

		while(!m_pubStop){
			main_transformer one = bq.take();
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




void MAIN_TRANSFORMER::start(BlockQueue<main_transformer> &bq)
{
	m_sub = new thread(&MAIN_TRANSFORMER::subClient,this);
	m_sub->detach();

	m_pub = new thread(&MAIN_TRANSFORMER::pubClient,this,ref(bq));
	m_pub->detach();

	m_thr_reconn = new thread(&MAIN_TRANSFORMER::reconn,this);
	m_thr_reconn->detach();

	m_thr_resend = new thread(&MAIN_TRANSFORMER::resnd,this);
	m_thr_resend->detach();

	m_thr_chksensor = new thread(&MAIN_TRANSFORMER::chksensor,this);
	m_thr_chksensor->detach();


//	thread *thr_range = new thread(&MAIN_TRANSFORMER::thSetRange,this);
//	thr_range->detach();

	thread *thr_threshold = new thread(&MAIN_TRANSFORMER::thSetThreshold,this);
	thr_threshold->detach();

	thread *thr_inhibit = new thread(&MAIN_TRANSFORMER::thSetInhibit,this);
	thr_inhibit->detach();

	thread *thr_delay = new thread(&MAIN_TRANSFORMER::thSetDelay,this);
	thr_delay->detach();		
}

bool MAIN_TRANSFORMER::UpdateHa(std::string key, uint16_t *data, int size)
{
	bool ret = false;
	for(int i = 0; i < sizeof(main_transformer_str); ++i)
	{
        std::string s(main_transformer_str[i]);
		if(s == key)
		{
			std::pair<uint8_t, uint8_t> byte_bit{0xff, 0xff};
            switch(i)
			{
                case 0:{byte_bit = m_data_store.no1_main_transformer_u_winding_temp.ha_bit;break;}
                case 1:{byte_bit = m_data_store.no1_main_transformer_v_winding_temp.ha_bit;break;}
                case 2:{byte_bit = m_data_store.no1_main_transformer_w_winding_temp.ha_bit;break;}
                case 3:{byte_bit = m_data_store.no2_main_transformer_u_winding_temp.ha_bit;break;}
                case 4:{byte_bit = m_data_store.no2_main_transformer_v_winding_temp.ha_bit;break;}
                case 5:{byte_bit = m_data_store.no2_main_transformer_w_winding_temp.ha_bit;break;}
				case 6:{byte_bit = m_data_store.device_offline.ha_bit;break;}
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
void MAIN_TRANSFORMER::chksensor()
{
	checkSensorStatus();
}

void MAIN_TRANSFORMER::resnd()
{
	resend();
}
	

void MAIN_TRANSFORMER::reconn()
{
	reconnect();
}


string MAIN_TRANSFORMER::property_reply(char flag,const main_transformer& one,const string& msg_id)
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

	writer.Key("properties");
	writer.StartObject();

	
#ifndef MAHTS648
	writer.Key(main_transformer_str[i++]);writer.Double(one.main_transformer_u_winding_temp);
	writer.Key(main_transformer_str[i++]);writer.Double(one.main_transformer_v_winding_temp);
	writer.Key(main_transformer_str[i++]);writer.Double(one.main_transformer_w_winding_temp);
	writer.Key(main_transformer_str[i++]);writer.Int(one.device_offline);
#else

    int i=0;
    writer.Key(main_transformer_str[i++]);writer.Int(one.transformer_door_openalarm);
    writer.Key(main_transformer_str[i++]);writer.Int(one.transformer_thermostat_failure);
    writer.Key(main_transformer_str[i++]);writer.Int(one.transformer_high_temp_alarm);
    writer.Key(main_transformer_str[i++]);writer.Double(one.winding_r_temp);
    writer.Key(main_transformer_str[i++]);writer.Double(one.winding_s_temp);
    writer.Key(main_transformer_str[i++]);writer.Double(one.winding_t_temp);
	writer.Key(main_transformer_str[i++]);writer.Int(one.mt_cooling_fan_stop);
	writer.Key(main_transformer_str[i++]);writer.Int(one.mt_high_temp_high_trip_alarm);
	writer.Key(main_transformer_str[i++]);writer.Int(one.mt_cooling_fan_overload);
	writer.Key(main_transformer_str[i++]);writer.Int(one.mt_winding_d_temp_sensor_failure);
	writer.Key(main_transformer_str[i++]);writer.Int(one.mt_winding_r_temp_sensor_failure);
	writer.Key(main_transformer_str[i++]);writer.Int(one.mt_winding_s_temp_sensor_failure);
	writer.Key(main_transformer_str[i++]);writer.Int(one.mt_winding_t_temp_sensor_failure);
	writer.Key(main_transformer_str[i++]);writer.Int(one.device_offline);
#endif
	writer.EndObject();

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

void MAIN_TRANSFORMER::thSetRange()
{
	setRange();
}

void MAIN_TRANSFORMER::thSetThreshold()
{
	setThreshold();
}

void MAIN_TRANSFORMER::thSetDelay()
{
	setDelay();
}


void MAIN_TRANSFORMER::thSetInhibit()
{
	setInhibit(5818,1);
}


void MAIN_TRANSFORMER::init_mb_addr()
{
	for(int i = 0;i < 3;i++)
	{
		m_mp_threshold_addr[main_transformer_str[i]] = 2856 + i + 3 *(m_devId - 1);
		m_mp_delay_addr[main_transformer_str[i]] = 8292 + 2 * i + 6 *(m_devId - 1);
		m_mp_inhibit_addr[main_transformer_str[i]] = 2 * i + 6 *(m_devId - 1);
	}
}





