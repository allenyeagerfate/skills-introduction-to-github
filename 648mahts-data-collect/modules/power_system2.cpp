#include <iostream>
#include <unistd.h>

#include "sgo_sub_pub.h"
#include "sgo_func.h"
#include "sgo_ams/power_system2.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

using namespace std;

const char* power_system2_str[]={
    "dg1_start_fail",
    "dg2_start_fail",
    "dg3_start_fail",
    "dg4_start_fail",
    "dg1_acb_abnormal_trip",
    "dg2_acb_abnormal_trip",
    "dg3_acb_abnormal_trip",
    "dg4_acb_abnormal_trip",
    "dg1_acb_close_fail",
    "dg2_acb_close_fail",
    "dg3_acb_close_fail",
    "dg4_acb_close_fail",
    "ac690v_bus_a_insul_low",
    "ac690v_bus_b_insul_low",
    "ac690v_bus_a_voltage_high",
    "ac690v_bus_b_voltage_high",
    "ac690v_bus_a_voltage_low",
    "ac690v_bus_b_voltage_low",
    "ac690v_bus_a_frequency_high",
    "ac690v_bus_b_frequency_high",
    "ac690v_bus_a_frequency_low",
    "ac690v_bus_b_frequency_low",
    "preferential_trip_alarm",
    "ac690v_msb_common_alarm",
    "ac440v_bus_a_insul_low",
    "ac440v_bus_b_insul_low",
    "ac220v_bus_a_insul_low",
    "ac220v_bus_b_insul_low",
    "ac440v_msb_em_cy_stop_and_pre_trip_source_fail",
    "ac440v_msb_dc24v_source_fault",
    "ac690v_msb_dc24v_source_a_fault",
    "ac690v_msb_dc24v_source_b_fault",
    "dg1_acb_closed",
    "dg2_acb_closed",
    "dg3_acb_closed",
    "dg4_acb_closed",
    "ddbt_acb_closed",
    "pst_acb_closed",
    "sst_acb_closed",
    "bt1_acb_closed",
    "bt2_acb_closed",
    "dt1primary_acb_closed",
    "dt2primary_acb_closed",
    "dt1secondary_acb_closed",
    "dt2secondary_acb_closed",
    "ac440v_bus_tie_breaker_close",
    "shore_cb_closed",
    "dg1_power_kw",
    "dg2_power_kw",
    "dg3_power_kw",
    "dg4_power_kw",
    "dg1_voltagev",
    "dg2_voltagev",
    "dg3_voltagev",
    "dg4_voltagev",
    "ac690v_bus_a_frequencyhz",
    "ac690v_bus_b_frequencyhz",
    "ac690v_bus_a_total_powerkw",
    "ac690v_bus_b_total_powerkw",
    "ac690v_bus_total_powerkw",
    "ac690v_bus_a_surplus_powerkw",
    "ac690v_bus_b_surplus_powerkw",
    "ac690v_bus_surplus_powerkw",
    "dg1_power_factor",
    "dg2_power_factor",
    "dg3_power_factor",
    "dg4_power_factor",
    "dg1_running",
    "dg2_running",
    "dg3_running",
    "dg4_running",
    "dg1_common_alarm",
    "dg2_common_alarm",
    "dg3_common_alarm",
    "dg4_common_alarm",
    "stand_by_gen_changeover_alarm",
    "dg1_voltage_high",
    "dg1_voltage_low",
    "dg1_frequency_high",
    "dg1_frequency_low",
    "dg2_voltage_high",
    "dg2_voltage_low",
    "dg2_frequency_high",
    "dg2_frequency_low",
    "dg3_voltage_high",
    "dg3_voltage_low",
    "dg3_frequency_high",
    "dg3_frequency_low",
    "dg4_voltage_high",
    "dg4_voltage_low",
    "dg4_frequency_high",
    "dg4_frequency_low",
    "load_sharing_fail",
    "ac690v_pms1_fault",
    "ac690v_pms2_fault",
    "device_offline",
};

	
static const char* productCode = "power_system2";

POWER_SYSTEM2::POWER_SYSTEM2()
{}

POWER_SYSTEM2::~POWER_SYSTEM2()
{}

void POWER_SYSTEM2::init(int devId,BlockQueue<power_system2> &bq,modbus* mb,mutex* mux)
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
void POWER_SYSTEM2::connectionLostCallback(void* context, char* cause) 
{
	// 处理连接丢失事件
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	POWER_SYSTEM2* client = static_cast<POWER_SYSTEM2*>(ctx->manager);
	
	printf("--%s %d--address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());
	sgo_log_stderr(0,"--%s %d----address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());

	//std::lock_guard<std::mutex> lock(m_losts_mutex);
	client->m_v_lost[ctx->index] = 1;
	client->m_v_property_index_array.clear();
}


// 消息到达回调函数
int POWER_SYSTEM2::messageArrivedCallback(void* context, char* topicName, int topicLen,MQTTClient_message* message) 
{
	// 处理接收到的消息
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	POWER_SYSTEM2* client = static_cast<POWER_SYSTEM2*>(ctx->manager);
	
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
			client->m_property[power_system2_str[i]]=i;
		
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
void POWER_SYSTEM2::deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt) 
{
	// 处理交付完成事件
	//std::cout << "Message delivery complete." << std::endl;
}

void POWER_SYSTEM2::subClient()
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


void POWER_SYSTEM2::pubClient(BlockQueue<power_system2>& bq)
{
	while(1)
	{
		MQTTClient_message pubmsg = MQTTClient_message_initializer;
		//声明消息token
		MQTTClient_deliveryToken token;

		while(!m_pubStop){
			power_system2 one = bq.take();
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



void POWER_SYSTEM2::start(BlockQueue<power_system2> &bq)
{
	m_sub = new thread(&POWER_SYSTEM2::subClient,this);
	m_sub->detach();

	m_pub = new thread(&POWER_SYSTEM2::pubClient,this,ref(bq));
	m_pub->detach();

	m_thr_reconn = new thread(&POWER_SYSTEM2::reconn,this);
	m_thr_reconn->detach();

	m_thr_resend = new thread(&POWER_SYSTEM2::resnd,this);
	m_thr_resend->detach();

	m_thr_chksensor = new thread(&POWER_SYSTEM2::chksensor,this);
	m_thr_chksensor->detach();

//	thread *thr_range = new thread(&POWER_SYSTEM2::thSetRange,this);
//	thr_range->detach();

	thread *thr_threshold = new thread(&POWER_SYSTEM2::thSetThreshold,this);
	thr_threshold->detach();

	thread *thr_inhibit = new thread(&POWER_SYSTEM2::thSetInhibit,this);
	thr_inhibit->detach();

	thread *thr_delay = new thread(&POWER_SYSTEM2::thSetDelay,this);
	thr_delay->detach();		
}

void POWER_SYSTEM2::chksensor()
{
	checkSensorStatus();
}

void POWER_SYSTEM2::resnd()
{
	resend();
}

void POWER_SYSTEM2::reconn()
{
	reconnect();
}


string POWER_SYSTEM2::property_reply(char flag,const power_system2& one,const string& msg_id)
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
                case 0:writer.Int(one.dg1_start_fail);break;
                case 1:writer.Int(one.dg2_start_fail);break;
                case 2:writer.Int(one.dg3_start_fail);break;
                case 3:writer.Int(one.dg4_start_fail);break;
                case 4:writer.Int(one.dg1_acb_abnormal_trip);break;
                case 5:writer.Int(one.dg2_acb_abnormal_trip);break;
                case 6:writer.Int(one.dg3_acb_abnormal_trip);break;
                case 7:writer.Int(one.dg4_acb_abnormal_trip);break;
                case 8:writer.Int(one.dg1_acb_close_fail);break;
                case 9:writer.Int(one.dg2_acb_close_fail);break;
                case 10:writer.Int(one.dg3_acb_close_fail);break;
                case 11:writer.Int(one.dg4_acb_close_fail);break;
                case 12:writer.Int(one.ac690v_bus_a_insul_low);break;
                case 13:writer.Int(one.ac690v_bus_b_insul_low);break;
                case 14:writer.Int(one.ac690v_bus_a_voltage_high);break;
                case 15:writer.Int(one.ac690v_bus_b_voltage_high);break;
                case 16:writer.Int(one.ac690v_bus_a_voltage_low);break;
                case 17:writer.Int(one.ac690v_bus_b_voltage_low);break;
                case 18:writer.Int(one.ac690v_bus_a_frequency_high);break;
                case 19:writer.Int(one.ac690v_bus_b_frequency_high);break;
                case 20:writer.Int(one.ac690v_bus_a_frequency_low);break;
                case 21:writer.Int(one.ac690v_bus_b_frequency_low);break;
                case 22:writer.Int(one.preferential_trip_alarm);break;
                case 23:writer.Int(one.ac690v_msb_common_alarm);break;
                case 24:writer.Int(one.ac440v_bus_a_insul_low);break;
                case 25:writer.Int(one.ac440v_bus_b_insul_low);break;
                case 26:writer.Int(one.ac220v_bus_a_insul_low);break;
                case 27:writer.Int(one.ac220v_bus_b_insul_low);break;
                case 28:writer.Int(one.ac440v_msb_em_cy_stop_and_pre_trip_source_fail);break;
                case 29:writer.Int(one.ac440v_msb_dc24v_source_fault);break;
                case 30:writer.Int(one.ac690v_msb_dc24v_source_a_fault);break;
                case 31:writer.Int(one.ac690v_msb_dc24v_source_b_fault);break;
                case 32:writer.Int(one.dg1_acb_closed);break;
                case 33:writer.Int(one.dg2_acb_closed);break;
                case 34:writer.Int(one.dg3_acb_closed);break;
                case 35:writer.Int(one.dg4_acb_closed);break;
                case 36:writer.Int(one.ddbt_acb_closed);break;
                case 37:writer.Int(one.pst_acb_closed);break;
                case 38:writer.Int(one.sst_acb_closed);break;
                case 39:writer.Int(one.bt1_acb_closed);break;
                case 40:writer.Int(one.bt2_acb_closed);break;
                case 41:writer.Int(one.dt1primary_acb_closed);break;
                case 42:writer.Int(one.dt2primary_acb_closed);break;
                case 43:writer.Int(one.dt1secondary_acb_closed);break;
                case 44:writer.Int(one.dt2secondary_acb_closed);break;
                case 45:writer.Int(one.ac440v_bus_tie_breaker_close);break;
                case 46:writer.Int(one.shore_cb_closed);break;
                case 47:writer.Double(one.dg1_power_kw);break;
                case 48:writer.Double(one.dg2_power_kw);break;
                case 49:writer.Double(one.dg3_power_kw);break;
                case 50:writer.Double(one.dg4_power_kw);break;
                case 51:writer.Double(one.dg1_voltagev);break;
                case 52:writer.Double(one.dg2_voltagev);break;
                case 53:writer.Double(one.dg3_voltagev);break;
                case 54:writer.Double(one.dg4_voltagev);break;
                case 55:writer.Double(one.ac690v_bus_a_frequencyhz);break;
                case 56:writer.Double(one.ac690v_bus_b_frequencyhz);break;
                case 57:writer.Double(one.ac690v_bus_a_total_powerkw);break;
                case 58:writer.Double(one.ac690v_bus_b_total_powerkw);break;
                case 59:writer.Double(one.ac690v_bus_total_powerkw);break;
                case 60:writer.Double(one.ac690v_bus_a_surplus_powerkw);break;
                case 61:writer.Double(one.ac690v_bus_b_surplus_powerkw);break;
                case 62:writer.Double(one.ac690v_bus_surplus_powerkw);break;
                case 63:writer.Double(one.dg1_power_factor);break;
                case 64:writer.Double(one.dg2_power_factor);break;
                case 65:writer.Double(one.dg3_power_factor);break;
                case 66:writer.Double(one.dg4_power_factor);break;
                case 67:writer.Int(one.dg1_running);break;
                case 68:writer.Int(one.dg2_running);break;
                case 69:writer.Int(one.dg3_running);break;
                case 70:writer.Int(one.dg4_running);break;
                case 71:writer.Int(one.dg1_common_alarm);break;
                case 72:writer.Int(one.dg2_common_alarm);break;
                case 73:writer.Int(one.dg3_common_alarm);break;
                case 74:writer.Int(one.dg4_common_alarm);break;
                case 75:writer.Int(one.stand_by_gen_changeover_alarm);break;
                case 76:writer.Int(one.dg1_voltage_high);break;
                case 77:writer.Int(one.dg1_voltage_low);break;
                case 78:writer.Int(one.dg1_frequency_high);break;
                case 79:writer.Int(one.dg1_frequency_low);break;
                case 80:writer.Int(one.dg2_voltage_high);break;
                case 81:writer.Int(one.dg2_voltage_low);break;
                case 82:writer.Int(one.dg2_frequency_high);break;
                case 83:writer.Int(one.dg2_frequency_low);break;
                case 84:writer.Int(one.dg3_voltage_high);break;
                case 85:writer.Int(one.dg3_voltage_low);break;
                case 86:writer.Int(one.dg3_frequency_high);break;
                case 87:writer.Int(one.dg3_frequency_low);break;
                case 88:writer.Int(one.dg4_voltage_high);break;
                case 89:writer.Int(one.dg4_voltage_low);break;
                case 90:writer.Int(one.dg4_frequency_high);break;
                case 91:writer.Int(one.dg4_frequency_low);break;
                case 92:writer.Int(one.load_sharing_fail);break;
                case 93:writer.Int(one.ac690v_pms1_fault);break;
                case 94:writer.Int(one.ac690v_pms2_fault);break;
                case 95:writer.Int(one.device_offline);break;
			default:
				break;break;
		}
	}

	writer.EndArray();		
#else
	writer.Key("properties");
	writer.StartObject();

	int i=0;
	writer.Key(power_system2_str[i++]);writer.Int(one.b_t_remote_control_system_ac_power_failure);
	writer.Key(power_system2_str[i++]);writer.Int(one.b_t_remote_control_system_dc_power_failure);
	writer.Key(power_system2_str[i++]);writer.Int(one.b_t_remote_control_system_failure);
	writer.Key(power_system2_str[i++]);writer.Int(one.b_t_main_motor_overload);
	writer.Key(power_system2_str[i++]);writer.Int(one.b_t_main_motor_high_temperature);
	writer.Key(power_system2_str[i++]);writer.Int(one.b_t_oil_low_level);
	writer.Key(power_system2_str[i++]);writer.Int(one.b_t_oil_low_pressure);
	writer.Key(power_system2_str[i++]);writer.Int(one.b_t_oil_high_pressure);
	writer.Key(power_system2_str[i++]);writer.Int(one.b_t_main_motor_short_circuit);
	writer.Key(power_system2_str[i++]);writer.Int(one.b_t_emergency_stop_circuit_failure);
	writer.Key(power_system2_str[i++]);writer.Int(one.b_t_too_many_times_start);
	writer.Key(power_system2_str[i++]);writer.Int(one.b_t_integrated_alarm);

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



void POWER_SYSTEM2::thSetRange()
{
	setRange();
}

void POWER_SYSTEM2::thSetThreshold()
{
	setThreshold();
}

void POWER_SYSTEM2::thSetDelay()
{
	setDelay();
}


void POWER_SYSTEM2::thSetInhibit()
{
	setInhibit(5806,2);
}


void POWER_SYSTEM2::init_mb_addr()
{
	int addr_ai = 8000;
	for(int i = 0; i < 3; i++)
		m_mp_delay_addr[power_system2_str[i]] = addr_ai + 102 + 3 * (m_devId - 1) + i;

	for(int i = 0; i < 4; i++)
	{
		m_mp_delay_addr[power_system2_str[3 + i]] = addr_ai + 108 + 9 * (m_devId - 1) + i;
		m_mp_delay_addr[power_system2_str[7 + i]] = addr_ai + 113 + 9 * (m_devId - 1) + i;
	}
	m_mp_delay_addr[power_system2_str[11]] = addr_ai + 112 + 9 * (m_devId - 1);

	//////////////////////////////////////////////////////
	if(m_devId == 0)
	{
		int i = 0;
		m_mp_inhibit_addr[power_system2_str[i++]] = 2;
		m_mp_inhibit_addr[power_system2_str[i++]] = 3;
		m_mp_inhibit_addr[power_system2_str[i++]] = 4;
		m_mp_inhibit_addr[power_system2_str[i++]] = 8;
		m_mp_inhibit_addr[power_system2_str[i++]] = 9;
		m_mp_inhibit_addr[power_system2_str[i++]] = 10;
		m_mp_inhibit_addr[power_system2_str[i++]] = 11;
		m_mp_inhibit_addr[power_system2_str[i++]] = 13;
		m_mp_inhibit_addr[power_system2_str[i++]] = 14;
		m_mp_inhibit_addr[power_system2_str[i++]] = 15;
		m_mp_inhibit_addr[power_system2_str[i++]] = 100;
		m_mp_inhibit_addr[power_system2_str[i++]] = 12;		
	}
	else
	{
		int i = 0;
		m_mp_inhibit_addr[power_system2_str[i++]] = 5;
		m_mp_inhibit_addr[power_system2_str[i++]] = 6;
		m_mp_inhibit_addr[power_system2_str[i++]] = 7;
		m_mp_inhibit_addr[power_system2_str[i++]] = 101;
		m_mp_inhibit_addr[power_system2_str[i++]] = 102;
		m_mp_inhibit_addr[power_system2_str[i++]] = 103;
		m_mp_inhibit_addr[power_system2_str[i++]] = 104;
		m_mp_inhibit_addr[power_system2_str[i++]] = 106;
		m_mp_inhibit_addr[power_system2_str[i++]] = 107;
		m_mp_inhibit_addr[power_system2_str[i++]] = 108;
		m_mp_inhibit_addr[power_system2_str[i++]] = 109;
		m_mp_inhibit_addr[power_system2_str[i++]] = 105;		
	}		
}





