#include <iostream>
#include <unistd.h>

#include "sgo_sub_pub.h"
#include "sgo_func.h"
#include "sgo_ams/cooling_water_system.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
//#include "cooling_water_system.h"

using namespace std;

const char* cooling_water_system_str[]={
#ifndef MAHTS648
	"no1_propulsion_sw_cooling_pump_outlet_pressure",
	"no2_propulsion_sw_cooling_pump_outlet_pressure",
	"no1_propulsion_fw_cooling_pump_outlet_pressure",
	"no2_propulsion_fw_cooling_pump_outlet_pressure",
	"device_offline",
#else
"main_sea_water_line_pressure_p",
"main_sea_water_line_pressure_s",
"no1_bilge_water_pump_inlet_pressure",
"no1_bilge_water_pump_outlet_pressure",
"no2_bilge_water_pump_inlet_pressure",
"no2_bilge_water_pump_outlet_pressure",
"no1_ballast_water_pump_inlet_pressure",
"no2_ballast_water_pump_inlet_pressure",
"no1_fire_water_pump_outlet_pressure",
"no2_fire_water_pump_outlet_pressure",
"jocky_pump_outlet_pressure",
"jocky_pump_inlet_pressure",
"fire_main_line_pressurep",
"fire_main_line_pressures",
"no1_fresh_water_transfer_pump_outlet_pressure",
"no2_fresh_water_transfer_pump_outlet_pressure",
"no1_propulsion_sw_cooling_pump_outlet_pressure",
"no2_propulsion_sw_cooling_pump_outlet_pressure",
"no1_propulsion_fw_cooling_pump_outlet_pressure",
"no2_propulsion_fw_cooling_pump_outlet_pressure",
"no1_propulsion_fw_cooling_pump_outlet_temperature",
"no2_propulsion_fw_cooling_pump_outlet_temperature",
"no1_propulsion_fw_cooling_plant_inlet_temperature",
"no2_propulsion_fw_cooling_plant_inlet_temperature",
"no1_propulsion_motor_fw_cooling_outlet_tempreature",
"no1_propulsion_vfd_fw_cooling_outlet_tempreature",
"no2_propulsion_motor_fw_cooling_outlet_tempreature",
"no2_propulsion_vfd_fw_cooling_outlet_tempreature",
"no3_propulsion_motor_fw_cooling_outlet_tempreature",
"no3_propulsion_vfd_fw_cooling_outlet_tempreature",
"no4_propulsion_motor_fw_cooling_outlet_tempreature",
"no4_propulsion_vfd_fw_cooling_outlet_tempreature",
"no5_propulsion_motor_fw_cooling_outlet_tempreature",
"no5_propulsion_vfd_fw_cooling_outlet_tempreature",
"no6_propulsion_motor_fw_cooling_outlet_tempreature",
"no6_propulsion_vfd_fw_cooling_outlet_tempreature",
"no1_fire_water_pump_inlet_pressure",
"no2_fire_water_pump_inlet_pressure",
"device_offline",



#endif
};

	
static const char* productCode = "cooling_water_system";


COOLING_WATER_SYSTEM::COOLING_WATER_SYSTEM()
{}

COOLING_WATER_SYSTEM::~COOLING_WATER_SYSTEM()
{}

void COOLING_WATER_SYSTEM::init(int devId,BlockQueue<cooling_water_system> &bq,modbus *mb,mutex *mux)
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
void COOLING_WATER_SYSTEM::connectionLostCallback(void* context, char* cause) 
{
	// 处理连接丢失事件
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	COOLING_WATER_SYSTEM* client = static_cast<COOLING_WATER_SYSTEM*>(ctx->manager);
	
	printf("--%s %d--address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());
	sgo_log_stderr(0,"--%s %d----address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());

	//std::lock_guard<std::mutex> lock(m_losts_mutex);
	client->m_v_lost[ctx->index] = 1;
	client->m_v_property_index_array.clear();
}


// 消息到达回调函数
int COOLING_WATER_SYSTEM::messageArrivedCallback(void* context, char* topicName, int topicLen,MQTTClient_message* message) 
{
	// 处理接收到的消息
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	COOLING_WATER_SYSTEM* client = static_cast<COOLING_WATER_SYSTEM*>(ctx->manager);
	
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
			client->m_property[cooling_water_system_str[i]]=i;
		
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
void COOLING_WATER_SYSTEM::deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt) 
{
	// 处理交付完成事件
	//std::cout << "Message delivery complete." << std::endl;
}

void COOLING_WATER_SYSTEM::subClient()
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


void COOLING_WATER_SYSTEM::pubClient(BlockQueue<cooling_water_system>& bq)
{
	while(1)
	{
		MQTTClient_message pubmsg = MQTTClient_message_initializer;
		//声明消息token
		MQTTClient_deliveryToken token;

		while(!m_pubStop){
			cooling_water_system one = bq.take();
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




void COOLING_WATER_SYSTEM::start(BlockQueue<cooling_water_system> &bq)
{
	m_sub = new thread(&COOLING_WATER_SYSTEM::subClient,this);
	m_sub->detach();

	m_pub = new thread(&COOLING_WATER_SYSTEM::pubClient,this,ref(bq));
	m_pub->detach();

	m_thr_reconn = new thread(&COOLING_WATER_SYSTEM::reconn,this);
	m_thr_reconn->detach();

	m_thr_resend = new thread(&COOLING_WATER_SYSTEM::resnd,this);
	m_thr_resend->detach();

	m_thr_chksensor = new thread(&COOLING_WATER_SYSTEM::chksensor,this);
	m_thr_chksensor->detach();

	thread *thr_range = new thread(&COOLING_WATER_SYSTEM::thSetRange,this);
	thr_range->detach();

	thread *thr_threshold = new thread(&COOLING_WATER_SYSTEM::thSetThreshold,this);
	thr_threshold->detach();

	thread *thr_inhibit = new thread(&COOLING_WATER_SYSTEM::thSetInhibit,this);
	thr_inhibit->detach();

	thread *thr_delay = new thread(&COOLING_WATER_SYSTEM::thSetDelay,this);
	thr_delay->detach();	
}

bool COOLING_WATER_SYSTEM::UpdateLa(std::string key, uint16_t *data, int size)
{
	bool ret = false;
	for(int i = 0; i < sizeof(cooling_water_system_str); ++i)
	{
        std::string s(cooling_water_system_str[i]);
		if(s == key)
		{
			std::pair<uint8_t, uint8_t> byte_bit{0xff, 0xff};
            switch(i)
			{
                case 0:{byte_bit = m_data_store.main_sea_water_line_pressure_p.la_bit;break;}
                case 1:{byte_bit = m_data_store.main_sea_water_line_pressure_s.la_bit;break;}
                case 2:{byte_bit = m_data_store.no1_bilge_water_pump_inlet_pressure.la_bit;break;}
                case 3:{byte_bit = m_data_store.no1_bilge_water_pump_outlet_pressure.la_bit;break;}
                case 4:{byte_bit = m_data_store.no2_bilge_water_pump_inlet_pressure.la_bit;break;}
                case 5:{byte_bit = m_data_store.no2_bilge_water_pump_outlet_pressure.la_bit;break;}
                case 6:{byte_bit = m_data_store.no1_ballast_water_pump_inlet_pressure.la_bit;break;}
                case 7:{byte_bit = m_data_store.no2_ballast_water_pump_inlet_pressure.la_bit;break;}
                case 8:{byte_bit = m_data_store.no1_fire_water_pump_outlet_pressure.la_bit;break;}
                case 9:{byte_bit = m_data_store.no2_fire_water_pump_outlet_pressure.la_bit;break;}
                case 10:{byte_bit = m_data_store.jokcy_pump_outlet_pressure.la_bit;break;}
                case 11:{byte_bit = m_data_store.jokcy_pump_inlet_pressure.la_bit;break;}
                case 12:{byte_bit = m_data_store.fire_main_line_pressure_p.la_bit;break;}
                case 13:{byte_bit = m_data_store.fire_main_line_pressure_s.la_bit;break;}
                case 14:{byte_bit = m_data_store.no1_fresh_water_transfer_pump_outlet_pressure.la_bit;break;}
                case 15:{byte_bit = m_data_store.no2_fresh_water_transfer_pump_outlet_pressure.la_bit;break;}
                case 16:{byte_bit = m_data_store.no1_propulsion_sw_cooling_pump_outlet_pressure.la_bit;break;}
                case 17:{byte_bit = m_data_store.no2_propulsion_sw_cooling_pump_outlet_pressure.la_bit;break;}
                case 18:{byte_bit = m_data_store.no1_propulsion_fw_cooling_pump_outlet_pressure.la_bit;break;}
                case 19:{byte_bit = m_data_store.no2_propulsion_fw_cooling_pump_outlet_pressure.la_bit;break;}
                case 20:{byte_bit = m_data_store.no1_propulsion_fw_cooling_pump_outlet_temperature.la_bit;break;}
                case 21:{byte_bit = m_data_store.no2_propulsion_fw_cooling_pump_outlet_temperature.la_bit;break;}
                case 22:{byte_bit = m_data_store.no1_propulsion_fw_cooling_plant_inlet_temperature.la_bit;break;}
                case 23:{byte_bit = m_data_store.no2_propulsion_fw_cooling_plant_inlet_temperature.la_bit;break;}
                case 24:{byte_bit = m_data_store.no1_propulsion_motor_fw_cooling_outlet_temperature.la_bit;break;}
                case 25:{byte_bit = m_data_store.no1_propulsion_vfd_fw_cooling_outlet_temperature.la_bit;break;}
                case 26:{byte_bit = m_data_store.no2_propulsion_motor_fw_cooling_outlet_temperature.la_bit;break;}
                case 27:{byte_bit = m_data_store.no2_propulsion_vfd_fw_cooling_outlet_temperature.la_bit;break;}
                case 28:{byte_bit = m_data_store.no3_propulsion_motor_fw_cooling_outlet_temperature.la_bit;break;}
                case 29:{byte_bit = m_data_store.no3_propulsion_vfd_fw_cooling_outlet_temperature.la_bit;break;}
                case 30:{byte_bit = m_data_store.no4_propulsion_motor_fw_cooling_outlet_temperature.la_bit;break;}
                case 31:{byte_bit = m_data_store.no4_propulsion_vfd_fw_cooling_outlet_temperature.la_bit;break;}
                case 32:{byte_bit = m_data_store.no5_propulsion_motor_fw_cooling_outlet_temperature.la_bit;break;}
                case 33:{byte_bit = m_data_store.no5_propulsion_vfd_fw_cooling_outlet_temperature.la_bit;break;}
                case 34:{byte_bit = m_data_store.no6_propulsion_motor_fw_cooling_outlet_temperature.la_bit;break;}
                case 35:{byte_bit = m_data_store.no6_propulsion_vfd_fw_cooling_outlet_temperature.la_bit;break;}
                case 36:{byte_bit = m_data_store.no1_fire_water_pump_inlet_pressure.la_bit;break;}
                case 37:{byte_bit = m_data_store.no2_fire_water_pump_inlet_pressure.la_bit;break;}
                case 38:{byte_bit = m_data_store.device_offline.la_bit;break;}
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

void COOLING_WATER_SYSTEM::chksensor()
{
	checkSensorStatus();
}

void COOLING_WATER_SYSTEM::resnd()
{
	resend();
}

void COOLING_WATER_SYSTEM::reconn()
{
	reconnect();
}


string COOLING_WATER_SYSTEM::property_reply(char flag,const cooling_water_system& one,const string& msg_id)
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

#ifndef MAHTS648
	writer.Key("properties");
	writer.StartObject();

	int i=0;
	writer.Key(cooling_water_system_str[i++]);writer.Double(one.no1_propulsion_sw_cooling_pump_outlet_pressure);
	writer.Key(cooling_water_system_str[i++]);writer.Double(one.no2_propulsion_sw_cooling_pump_outlet_pressure);
	writer.Key(cooling_water_system_str[i++]);writer.Double(one.no1_propulsion_fw_cooling_pump_outlet_pressure);
	writer.Key(cooling_water_system_str[i++]);writer.Double(one.no2_propulsion_fw_cooling_pump_outlet_pressure);
	writer.Key(cooling_water_system_str[i++]);writer.Double(one.device_offline);
#else
    writer.Key("compressData");
	writer.StartArray();
	for (int i = 0; i < m_v_index.size(); i++)
	{
		switch (m_v_index[i])
		{
        case 0:writer.Double(one.main_sea_water_line_pressure_p);break;
        case 1:writer.Double(one.main_sea_water_line_pressure_s);break;
        case 2:writer.Double(one.no1_bilge_water_pump_inlet_pressure);break;
        case 3:writer.Double(one.no1_bilge_water_pump_outlet_pressure);break;
        case 4:writer.Double(one.no2_bilge_water_pump_inlet_pressure);break;
        case 5:writer.Double(one.no2_bilge_water_pump_outlet_pressure);break;
        case 6:writer.Double(one.no1_ballast_water_pump_inlet_pressure);break;
        case 7:writer.Double(one.no2_ballast_water_pump_inlet_pressure);break;
        case 8:writer.Double(one.no1_fire_water_pump_outlet_pressure);break;
        case 9:writer.Double(one.no2_fire_water_pump_outlet_pressure);break;
        case 10:writer.Double(one.jokcy_pump_outlet_pressure);break;
        case 11:writer.Double(one.jokcy_pump_inlet_pressure);break;
        case 12:writer.Double(one.fire_main_line_pressure_p);break;
        case 13:writer.Double(one.fire_main_line_pressure_s);break;
        case 14:writer.Double(one.no1_fresh_water_transfer_pump_outlet_pressure);break;
        case 15:writer.Double(one.no2_fresh_water_transfer_pump_outlet_pressure);break;
        case 16:writer.Double(one.no1_propulsion_sw_cooling_pump_outlet_pressure);break;
        case 17:writer.Double(one.no2_propulsion_sw_cooling_pump_outlet_pressure);break;
        case 18:writer.Double(one.no1_propulsion_fw_cooling_pump_outlet_pressure);break;
        case 19:writer.Double(one.no2_propulsion_fw_cooling_pump_outlet_pressure);break;
        case 20:writer.Double(one.no1_propulsion_fw_cooling_pump_outlet_temperature);break;
        case 21:writer.Double(one.no2_propulsion_fw_cooling_pump_outlet_temperature);break;
        case 22:writer.Double(one.no1_propulsion_fw_cooling_plant_inlet_temperature);break;
        case 23:writer.Double(one.no2_propulsion_fw_cooling_plant_inlet_temperature);break;
        case 24:writer.Double(one.no1_propulsion_motor_fw_cooling_outlet_temperature);break;
        case 25:writer.Double(one.no1_propulsion_vfd_fw_cooling_outlet_temperature);break;
        case 26:writer.Double(one.no2_propulsion_motor_fw_cooling_outlet_temperature);break;
        case 27:writer.Double(one.no2_propulsion_vfd_fw_cooling_outlet_temperature);break;
        case 28:writer.Double(one.no3_propulsion_motor_fw_cooling_outlet_temperature);break;
        case 29:writer.Double(one.no3_propulsion_vfd_fw_cooling_outlet_temperature);break;
        case 30:writer.Double(one.no4_propulsion_motor_fw_cooling_outlet_temperature);break;
        case 31:writer.Double(one.no4_propulsion_vfd_fw_cooling_outlet_temperature);break;
        case 32:writer.Double(one.no5_propulsion_motor_fw_cooling_outlet_temperature);break;
        case 33:writer.Double(one.no5_propulsion_vfd_fw_cooling_outlet_temperature);break;
        case 34:writer.Double(one.no6_propulsion_motor_fw_cooling_outlet_temperature);break;
        case 35:writer.Double(one.no6_propulsion_vfd_fw_cooling_outlet_temperature);break;
	    case 36:writer.Double(one.no1_fire_water_pump_inlet_pressure);break;
        case 37:writer.Double(one.no2_fire_water_pump_inlet_pressure);break;
        case 38:writer.Double(one.device_offline);break;

		default:
			break;
		}
	}
#endif

	writer.EndArray();

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


void COOLING_WATER_SYSTEM::thSetRange()
{
	
    while(1)
	{
		range one = bq_range.take();
		string key = one.m_keyname;
		float start = one.start;
		float end = one.end;

		std::pair<float, float> rg = std::make_pair(start, end);

	    for(int i = 0; i < sizeof(cooling_water_system_str); ++i)
	    {
            std::string s(cooling_water_system_str[i]);
		    if(s == key)
		    {
                switch(i)
		    	{
                    case 0:{m_data_store.main_sea_water_line_pressure_p.range=rg;break;}
                    case 1:{m_data_store.main_sea_water_line_pressure_s.range=rg;break;}
                    case 2:{m_data_store.no1_bilge_water_pump_inlet_pressure.range=rg;break;}
                    case 3:{m_data_store.no1_bilge_water_pump_outlet_pressure.range=rg;break;}
                    case 4:{m_data_store.no2_bilge_water_pump_inlet_pressure.range=rg;break;}
                    case 5:{m_data_store.no2_bilge_water_pump_outlet_pressure.range=rg;break;}
                    case 6:{m_data_store.no1_ballast_water_pump_inlet_pressure.range=rg;break;}
                    case 7:{m_data_store.no2_ballast_water_pump_inlet_pressure.range=rg;break;}
                    case 8:{m_data_store.no1_fire_water_pump_outlet_pressure.range=rg;break;}
                    case 9:{m_data_store.no2_fire_water_pump_outlet_pressure.range=rg;break;}
                    case 10:{m_data_store.jokcy_pump_outlet_pressure.range=rg;break;}
                    case 11:{m_data_store.jokcy_pump_inlet_pressure.range=rg;break;}
                    case 12:{m_data_store.fire_main_line_pressure_p.range=rg;break;}
                    case 13:{m_data_store.fire_main_line_pressure_s.range=rg;break;}
                    case 14:{m_data_store.no1_fresh_water_transfer_pump_outlet_pressure.range=rg;break;}
                    case 15:{m_data_store.no2_fresh_water_transfer_pump_outlet_pressure.range=rg;break;}
                    case 16:{m_data_store.no1_propulsion_sw_cooling_pump_outlet_pressure.range=rg;break;}
                    case 17:{m_data_store.no2_propulsion_sw_cooling_pump_outlet_pressure.range=rg;break;}
                    case 18:{m_data_store.no1_propulsion_fw_cooling_pump_outlet_pressure.range=rg;break;}
                    case 19:{m_data_store.no2_propulsion_fw_cooling_pump_outlet_pressure.range=rg;break;}
                    case 20:{m_data_store.no1_propulsion_fw_cooling_pump_outlet_temperature.range=rg;break;}
                    case 21:{m_data_store.no2_propulsion_fw_cooling_pump_outlet_temperature.range=rg;break;}
                    case 22:{m_data_store.no1_propulsion_fw_cooling_plant_inlet_temperature.range=rg;break;}
                    case 23:{m_data_store.no2_propulsion_fw_cooling_plant_inlet_temperature.range=rg;break;}
                    case 24:{m_data_store.no1_propulsion_motor_fw_cooling_outlet_temperature.range=rg;break;}
                    case 25:{m_data_store.no1_propulsion_vfd_fw_cooling_outlet_temperature.range=rg;break;}
                    case 26:{m_data_store.no2_propulsion_motor_fw_cooling_outlet_temperature.range=rg;break;}
                    case 27:{m_data_store.no2_propulsion_vfd_fw_cooling_outlet_temperature.range=rg;break;}
                    case 28:{m_data_store.no3_propulsion_motor_fw_cooling_outlet_temperature.range=rg;break;}
                    case 29:{m_data_store.no3_propulsion_vfd_fw_cooling_outlet_temperature.range=rg;break;}
                    case 30:{m_data_store.no4_propulsion_motor_fw_cooling_outlet_temperature.range=rg;break;}
                    case 31:{m_data_store.no4_propulsion_vfd_fw_cooling_outlet_temperature.range=rg;break;}
                    case 32:{m_data_store.no5_propulsion_motor_fw_cooling_outlet_temperature.range=rg;break;}
                    case 33:{m_data_store.no5_propulsion_vfd_fw_cooling_outlet_temperature.range=rg;break;}
                    case 34:{m_data_store.no6_propulsion_motor_fw_cooling_outlet_temperature.range=rg;break;}
                    case 35:{m_data_store.no6_propulsion_vfd_fw_cooling_outlet_temperature.range=rg;break;}
	                case 36:{m_data_store.no1_fire_water_pump_inlet_pressure.range=rg;break;}
                    case 37:{m_data_store.no2_fire_water_pump_inlet_pressure.range=rg;break;}
                    case 38:{m_data_store.device_offline.range=rg;break;}               
		    	}
    
		    	break;
		    }
	    }       
	}
	///////////////////
	//setRange();
	/////////////
}

void COOLING_WATER_SYSTEM::thSetThreshold()
{
	setThreshold();
}

void COOLING_WATER_SYSTEM::thSetDelay()
{
	setDelay();
}

void COOLING_WATER_SYSTEM::thSetInhibit()
{
	while (1)
	{
		inhibit one = bq_inhibit.take();
		string key = one.m_keyname;
		bool inhib = one.is_inhibit;
        for(int i = 0; i < sizeof(cooling_water_system_str); ++i)
	    {
            std::string s(cooling_water_system_str[i]);
		    if(s == key)
		    {
                switch(i)
		    	{
                    case 0:{m_data_store.main_sea_water_line_pressure_p.inhibit = inhib;break;}
                    case 1:{m_data_store.main_sea_water_line_pressure_s.inhibit = inhib;break;}
                    case 2:{m_data_store.no1_bilge_water_pump_inlet_pressure.inhibit = inhib;break;}
                    case 3:{m_data_store.no1_bilge_water_pump_outlet_pressure.inhibit = inhib;break;}
                    case 4:{m_data_store.no2_bilge_water_pump_inlet_pressure.inhibit = inhib;break;}
                    case 5:{m_data_store.no2_bilge_water_pump_outlet_pressure.inhibit = inhib;break;}
                    case 6:{m_data_store.no1_ballast_water_pump_inlet_pressure.inhibit = inhib;break;}
                    case 7:{m_data_store.no2_ballast_water_pump_inlet_pressure.inhibit = inhib;break;}
                    case 8:{m_data_store.no1_fire_water_pump_outlet_pressure.inhibit = inhib;break;}
                    case 9:{m_data_store.no2_fire_water_pump_outlet_pressure.inhibit = inhib;break;}
                    case 10:{m_data_store.jokcy_pump_outlet_pressure.inhibit = inhib;break;}
                    case 11:{m_data_store.jokcy_pump_inlet_pressure.inhibit = inhib;break;}
                    case 12:{m_data_store.fire_main_line_pressure_p.inhibit = inhib;break;}
                    case 13:{m_data_store.fire_main_line_pressure_s.inhibit = inhib;break;}
                    case 14:{m_data_store.no1_fresh_water_transfer_pump_outlet_pressure.inhibit = inhib;break;}
                    case 15:{m_data_store.no2_fresh_water_transfer_pump_outlet_pressure.inhibit = inhib;break;}
                    case 16:{m_data_store.no1_propulsion_sw_cooling_pump_outlet_pressure.inhibit = inhib;break;}
                    case 17:{m_data_store.no2_propulsion_sw_cooling_pump_outlet_pressure.inhibit = inhib;break;}
                    case 18:{m_data_store.no1_propulsion_fw_cooling_pump_outlet_pressure.inhibit = inhib;break;}
                    case 19:{m_data_store.no2_propulsion_fw_cooling_pump_outlet_pressure.inhibit = inhib;break;}
                    case 20:{m_data_store.no1_propulsion_fw_cooling_pump_outlet_temperature.inhibit = inhib;break;}
                    case 21:{m_data_store.no2_propulsion_fw_cooling_pump_outlet_temperature.inhibit = inhib;break;}
                    case 22:{m_data_store.no1_propulsion_fw_cooling_plant_inlet_temperature.inhibit = inhib;break;}
                    case 23:{m_data_store.no2_propulsion_fw_cooling_plant_inlet_temperature.inhibit = inhib;break;}
                    case 24:{m_data_store.no1_propulsion_motor_fw_cooling_outlet_temperature.inhibit = inhib;break;}
                    case 25:{m_data_store.no1_propulsion_vfd_fw_cooling_outlet_temperature.inhibit = inhib;break;}
                    case 26:{m_data_store.no2_propulsion_motor_fw_cooling_outlet_temperature.inhibit = inhib;break;}
                    case 27:{m_data_store.no2_propulsion_vfd_fw_cooling_outlet_temperature.inhibit = inhib;break;}
                    case 28:{m_data_store.no3_propulsion_motor_fw_cooling_outlet_temperature.inhibit = inhib;break;}
                    case 29:{m_data_store.no3_propulsion_vfd_fw_cooling_outlet_temperature.inhibit = inhib;break;}
                    case 30:{m_data_store.no4_propulsion_motor_fw_cooling_outlet_temperature.inhibit = inhib;break;}
                    case 31:{m_data_store.no4_propulsion_vfd_fw_cooling_outlet_temperature.inhibit = inhib;break;}
                    case 32:{m_data_store.no5_propulsion_motor_fw_cooling_outlet_temperature.inhibit = inhib;break;}
                    case 33:{m_data_store.no5_propulsion_vfd_fw_cooling_outlet_temperature.inhibit = inhib;break;}
                    case 34:{m_data_store.no6_propulsion_motor_fw_cooling_outlet_temperature.inhibit = inhib;break;}
                    case 35:{m_data_store.no6_propulsion_vfd_fw_cooling_outlet_temperature.inhibit = inhib;break;}
                    case 36:{m_data_store.no1_fire_water_pump_inlet_pressure.inhibit = inhib;break;}
                    case 37:{m_data_store.no2_fire_water_pump_inlet_pressure.inhibit = inhib;break;}
                    case 38:{m_data_store.device_offline.inhibit = inhib;break;}
		    	}
    
		    	break;
		    }
	    }       
	}
}

void COOLING_WATER_SYSTEM::init_mb_addr()
{
	for(int i = 0; i < 4; i++)
	{
		m_mp_threshold_addr[cooling_water_system_str[i]] = 2867 + i;
		//4-20mA
		m_mp_range_addr[cooling_water_system_str[i]] = 9834 + 2 * i; 
	}

}



