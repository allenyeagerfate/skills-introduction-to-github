#include <iostream>
#include <unistd.h>

#include "sgo_sub_pub.h"
#include "sgo_func.h"
#include "sgo_ams/e_r_charger_and_discharger_panel.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

using namespace std;

const char *e_r_charger_and_discharger_panel_str[] = {
#ifndef MAHTS648
	"ups_low_insulation",
	"ups_low_battery_voltage",
	"ups_input_fault",
	"ups_bypass_activation",
	"ups_common_alarm",
	"ups_alarm",
	"ups_running",
	"ups_battery_mode",
	"ups_battery_protection_trip",
	"ups_output_alarm",
	"device_offline",
#else
    "no1_battery_discharge",
    "no1_battery_voltage_low",
    "no1_charger_failure",
    "no1_charger_power_failure",
    "no1_charger_common_alarm",
    "no2_battery_discharge",
    "no2_battery_voltage_low",
    "no2_charger_failure",
    "no2_charger_power_failure",
    "no2_charger_common_alarm",
    "no3_battery_discharge",
    "no4_battery_discharge",
    "chp_bus_a_low_insulation",
    "chp_bus_b_low_insulation",
    "de1_low_insulation",
    "de1_common_failure",
    "de2_low_insulation",
    "de2_common_failure",
    "device_offline",
#endif
};

static const char* productCode = "e_r_ups";

E_R_CHARGER_AND_DISCHARGER_PANEL::E_R_CHARGER_AND_DISCHARGER_PANEL()
{}

E_R_CHARGER_AND_DISCHARGER_PANEL::~E_R_CHARGER_AND_DISCHARGER_PANEL()
{}

void E_R_CHARGER_AND_DISCHARGER_PANEL::init(int devId,BlockQueue<e_r_charger_and_discharger_panel> &bq,modbus* mb,mutex* mux)
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
void E_R_CHARGER_AND_DISCHARGER_PANEL::connectionLostCallback(void* context, char* cause) 
{
	// 处理连接丢失事件
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	E_R_CHARGER_AND_DISCHARGER_PANEL* client = static_cast<E_R_CHARGER_AND_DISCHARGER_PANEL*>(ctx->manager);
	
	printf("--%s %d--address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());
	sgo_log_stderr(0,"--%s %d----address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());

	//std::lock_guard<std::mutex> lock(m_losts_mutex);
	client->m_v_lost[ctx->index] = 1;
	client->m_v_property_index_array.clear();
}


// 消息到达回调函数
int E_R_CHARGER_AND_DISCHARGER_PANEL::messageArrivedCallback(void* context, char* topicName, int topicLen,MQTTClient_message* message) 
{
	// 处理接收到的消息
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	E_R_CHARGER_AND_DISCHARGER_PANEL* client = static_cast<E_R_CHARGER_AND_DISCHARGER_PANEL*>(ctx->manager);
	
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
			client->m_property[e_r_charger_and_discharger_panel_str[i]]=i;
		
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
void E_R_CHARGER_AND_DISCHARGER_PANEL::deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt) 
{
	// 处理交付完成事件
	//std::cout << "Message delivery complete." << std::endl;
}

void E_R_CHARGER_AND_DISCHARGER_PANEL::subClient()
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


void E_R_CHARGER_AND_DISCHARGER_PANEL::pubClient(BlockQueue<e_r_charger_and_discharger_panel>& bq)
{
	while(1)
	{
		MQTTClient_message pubmsg = MQTTClient_message_initializer;
		//声明消息token
		MQTTClient_deliveryToken token;

		while(!m_pubStop){
			e_r_charger_and_discharger_panel one = bq.take();
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




void E_R_CHARGER_AND_DISCHARGER_PANEL::start(BlockQueue<e_r_charger_and_discharger_panel> &bq)
{
	m_sub = new thread(&E_R_CHARGER_AND_DISCHARGER_PANEL::subClient,this);
	m_sub->detach();

	m_pub = new thread(&E_R_CHARGER_AND_DISCHARGER_PANEL::pubClient,this,ref(bq));
	m_pub->detach();

	m_thr_reconn = new thread(&E_R_CHARGER_AND_DISCHARGER_PANEL::reconn,this);
	m_thr_reconn->detach();

	m_thr_resend = new thread(&E_R_CHARGER_AND_DISCHARGER_PANEL::resnd,this);
	m_thr_resend->detach();

	m_thr_chksensor = new thread(&E_R_CHARGER_AND_DISCHARGER_PANEL::chksensor,this);
	m_thr_chksensor->detach();

	thread *thr_inhibit = new thread(&E_R_CHARGER_AND_DISCHARGER_PANEL::thSetInhibit,this);
	thr_inhibit->detach();

	thread *thr_delay = new thread(&E_R_CHARGER_AND_DISCHARGER_PANEL::thSetDelay,this);
	thr_delay->detach();	
}

void E_R_CHARGER_AND_DISCHARGER_PANEL::chksensor()
{
	checkSensorStatus();
}

void E_R_CHARGER_AND_DISCHARGER_PANEL::resnd()
{
	resend();
}

void E_R_CHARGER_AND_DISCHARGER_PANEL::reconn()
{
	reconnect();
}


string E_R_CHARGER_AND_DISCHARGER_PANEL::property_reply(char flag,const e_r_charger_and_discharger_panel& one,const string& msg_id)
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
			case 0:writer.Int(one.ups_low_insulation);break;
			case 1:writer.Int(one.ups_battery_low_voltage);break;
			case 2:writer.Int(one.ups_input_fault);break;
			case 3:writer.Int(one.ups_battery_bypass_activation);break;
			case 4:writer.Int(one.ups_common_alarm);break;
			case 5:writer.Int(one.ups_alarm);break;
			case 6:writer.Int(one.ups_running);break;
			case 7:writer.Int(one.ups_battery_mode);break;
			case 8:writer.Int(one.ups_battery_protection_trip);break;
			case 9:writer.Int(one.ups_output_alarm);break;
			case 10:writer.Int(one.device_offline);break;
#else
            case 0:writer.Int(one.no1_battery_discharge);break;
            case 1:writer.Int(one.no1_battery_voltage_low);break;
            case 2:writer.Int(one.no1_charger_failure);break;
            case 3:writer.Int(one.no1_charger_power_failure);break;
            case 4:writer.Int(one.no1_charger_common_alarm);break;
            case 5:writer.Int(one.no2_battery_discharge);break;
            case 6:writer.Int(one.no2_battery_voltage_low);break;
            case 7:writer.Int(one.no2_charger_failure);break;
            case 8:writer.Int(one.no2_charger_power_failure);break;
            case 9:writer.Int(one.no2_charger_common_alarm);break;
            case 10:writer.Int(one.no3_battery_discharge);break;
            case 11:writer.Int(one.no4_battery_discharge);break;
            case 12:writer.Int(one.chp_bus_a_low_insulation);break;
            case 13:writer.Int(one.chp_bus_b_low_insulation);break;
            case 14:writer.Int(one.de1_low_insulation);break;
            case 15:writer.Int(one.de1_common_failure);break;
            case 16:writer.Int(one.de2_low_insulation);break;
            case 17:writer.Int(one.de2_common_failure);break;
            case 18:writer.Int(one.device_offline);break;

#endif
			default:
				break;
		}
	}

	writer.EndArray();		
#else

	writer.Key("properties");
	writer.StartObject();

	int i=0;
	writer.Key(e_r_charger_and_discharger_panel_str[i++]);writer.Int(one.ups_low_insulation);
	writer.Key(e_r_charger_and_discharger_panel_str[i++]);writer.Int(one.ups_battery_bypass_activation);
	writer.Key(e_r_charger_and_discharger_panel_str[i++]);writer.Int(one.ups_input_fault);
	writer.Key(e_r_charger_and_discharger_panel_str[i++]);writer.Int(one.ups_battery_bypass_activation);
	writer.Key(e_r_charger_and_discharger_panel_str[i++]);writer.Int(one.ups_common_alarm);
	writer.Key(e_r_charger_and_discharger_panel_str[i++]);writer.Int(one.ups_alarm);
	writer.Key(e_r_charger_and_discharger_panel_str[i++]);writer.Int(one.ups_running);
	writer.Key(e_r_charger_and_discharger_panel_str[i++]);writer.Int(one.ups_battery_mode);
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



void E_R_CHARGER_AND_DISCHARGER_PANEL::thSetDelay()
{
	setDelay();
}


void E_R_CHARGER_AND_DISCHARGER_PANEL::thSetInhibit()
{
	while (1)
	{
		inhibit one = bq_inhibit.take();
		string key = one.m_keyname;
		bool inhib = one.is_inhibit;
        for(int i = 0; i < sizeof(e_r_charger_and_discharger_panel_str); ++i)
	    {
            std::string s(e_r_charger_and_discharger_panel_str[i]);
		    if(s == key)
		    {
                switch(i)
		    	{
                    case 0:{m_data_store.ups_common_alarm.inhibit = inhib;break;}
                    case 1:{m_data_store.ups_low_insulation.inhibit = inhib;break;}
                    case 2:{m_data_store.ups_battery_protection_trip.inhibit = inhib;break;}
                    case 3:{m_data_store.ups_battery_discharge.inhibit = inhib;break;}
                    case 4:{m_data_store.ups_battery_low_voltage.inhibit = inhib;break;}
                    case 5:{m_data_store.ups_battery_bypass_activation.inhibit = inhib;break;}
                    case 6:{m_data_store.ups_input_fault.inhibit = inhib;break;}
                    case 7:{m_data_store.device_offline.inhibit = inhib;break;}
		    	}
    
		    	break;
		    }
	    }       
	}
	//setInhibit(5804,7);
}


void E_R_CHARGER_AND_DISCHARGER_PANEL::init_mb_addr()
{
	int addr_ai = 8076;
	for(int i = 0;i < 5;i++)
	{
		m_mp_delay_addr[e_r_charger_and_discharger_panel_str[i]] = addr_ai + i;
		m_mp_inhibit_addr[e_r_charger_and_discharger_panel_str[i]] = 8 + i;
	}
	addr_ai = 8169;
	for(int i = 0;i < 3;i++)
	{
		m_mp_delay_addr[e_r_charger_and_discharger_panel_str[5 + i]] = addr_ai + i;
		m_mp_inhibit_addr[e_r_charger_and_discharger_panel_str[5 + i]] = 605 + i;
	}	
}



