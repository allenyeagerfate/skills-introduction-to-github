#include <iostream>
#include <unistd.h>

#include "sgo_sub_pub.h"
#include "sgo_func.h"
#include "sgo_ams/emergency_generator.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
//#include "emergency_generator.h"

using namespace std;

const char* emergency_generator_str[]={
#ifndef MAHTS648
	"eg_dcu_unit_battery_volt_dcu_control_power",
	"eg_lube_oil_pressure",
	"eg_coolant_pressure",
	"eg_coolant_temperature",
	"eg_lube_oil_temperature",
	"eg_engine_speed",
	"eg_running_hours",
	"eg_rpu_unit_battery_a_volt_rpu_main_control_power",
	"eg_rpu_unit_battery_b_volt_rpu_backup_control_power",
	"eg_coolant_level_low",
	"eg_main_power_failure",
	"eg_backup_power_failure",
	"eg_no1_charger_failure",
	"eg_no2_charger_failure",
	"eg_local_emergency_stop",
	"eg_remote_emergency_stop",
	"eg_rpu_emergency_stop",
	"eg_lube_oil_pressure_low_alarm",
	"eg_coolant_pressure_low_alarm",
	"eg_coolant_temperature_high_alarm",
	"eg_lube_oil_temperature_high_alarm",
	"eg_overspeed_alarm",
	"eg_overspeed_shutdown",
	"eg_speed_sensor_fail",
	"eg_dcu_unit_battery_volt_dct_control_power_low",
	"eg_start_fail",
	"eg_rpu_unit_battery_a_volt_rpu_main_control_power_low",
	"eg_rpu_unit_battery_b_volt_rpu_main_control_power_low",
	"eg_common_alarm",
	"eg_common_warning",
	"eg_common_shutdown",
	"eg_common_cooldown",
	"eg_common_faultsensor_fail",
	"eg_remote_control_status_auxaccess_lock",
	"eg_ready_to_start",
	"eg_ready_to_load",
	"eg_generator_running",
	"eg_generator_u_winding_temperature",
	"eg_generator_v_winding_temperature",
	"eg_generator_w_winding_temperature",
	"device_offline",
#else
    "eg_dcu_unit_battery_volt_dcu_control_power",
    "eg_lube_oil_pressure",
    "eg_coolant_pressure",
    "eg_coolant_temperature",
    "eg_lube_oil_temperature",
    "eg_exhaust_temperature",
    "eg_engine_speed",
    "eg_running_hours",
    "eg_rpu_unit_battery_a_volt_rpu_main_control_power",
    "eg_rpu_unit_battery_b_volt_rpu_backup_control_power",
    "eg_coolant_level_low",
    "eg_main_power_failure",
    "eg_backup_power_failure",
    "eg_no1_charger_failure",
    "eg_no2_charger_failure",
    "eg_fuel_leakage_alarm",
    "eg_local_emergency_stop",
    "eg_remote_emergency_stop",
    "eg_rpu_emergency_stop",
    "eg_lube_oil_pressure_low_alarm",
    "eg_coolant_pressure_low_alarm",
    "eg_coolant_temperature_high_alarm",
    "eg_lube_oil_temperature_high_alarm",
    "eg_overspeed_alarm",
    "eg_overspeed_shutdown",
    "eg_speed_pickup_sensor_fail",
    "eg_dcu_unit_battery_volt_dct_control_power_low",
    "eg_start_fail",
    "eg_rpu_unit_battery_a_volt_rpu_main_control_power_low",
    "eg_rpu_unit_battery_b_volt_rpu_main_control_power_low",
    "eg_common_alarm",
    "eg_common_warning",
    "eg_common_shutdown",
    "eg_common_cooldowm",
    "eg_common_faultsensor_fail",
    "eg_remote_control_status_auxaccess_lock",
    "eg_ready_to_start",
    "eg_ready_to_load",
    "eg_running",
    "device_offline",
#endif
};

	
static const char* productCode = "emergency_generator";


EMERGENCY_GENERATOR::EMERGENCY_GENERATOR()
{}

EMERGENCY_GENERATOR::~EMERGENCY_GENERATOR()
{}

void EMERGENCY_GENERATOR::init(int devId,BlockQueue<emergency_generator> &bq,modbus *mb,mutex *mux)
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
void EMERGENCY_GENERATOR::connectionLostCallback(void* context, char* cause) 
{
	// 处理连接丢失事件
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	EMERGENCY_GENERATOR* client = static_cast<EMERGENCY_GENERATOR*>(ctx->manager);
	
	printf("--%s %d--address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());
	sgo_log_stderr(0,"--%s %d----address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());

	//std::lock_guard<std::mutex> lock(m_losts_mutex);
	client->m_v_lost[ctx->index] = 1;
	client->m_v_property_index_array.clear();
}


// 消息到达回调函数
int EMERGENCY_GENERATOR::messageArrivedCallback(void* context, char* topicName, int topicLen,MQTTClient_message* message) 
{
	// 处理接收到的消息
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	EMERGENCY_GENERATOR* client = static_cast<EMERGENCY_GENERATOR*>(ctx->manager);
	
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
			client->m_property[emergency_generator_str[i]]=i;
		
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
void EMERGENCY_GENERATOR::deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt) 
{
	// 处理交付完成事件
	//std::cout << "Message delivery complete." << std::endl;
}

void EMERGENCY_GENERATOR::subClient()
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


void EMERGENCY_GENERATOR::pubClient(BlockQueue<emergency_generator>& bq)
{
	while(1)
	{
		MQTTClient_message pubmsg = MQTTClient_message_initializer;
		//声明消息token
		MQTTClient_deliveryToken token;

		while(!m_pubStop){
			emergency_generator one = bq.take();
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



void EMERGENCY_GENERATOR::start(BlockQueue<emergency_generator> &bq)
{
	m_sub = new thread(&EMERGENCY_GENERATOR::subClient,this);
	m_sub->detach();

	m_pub = new thread(&EMERGENCY_GENERATOR::pubClient,this,ref(bq));
	m_pub->detach();

	m_thr_reconn = new thread(&EMERGENCY_GENERATOR::reconn,this);
	m_thr_reconn->detach();

	m_thr_resend = new thread(&EMERGENCY_GENERATOR::resnd,this);
	m_thr_resend->detach();

	m_thr_chksensor = new thread(&EMERGENCY_GENERATOR::chksensor,this);
	m_thr_chksensor->detach();

//	thread *thr_range = new thread(&EMERGENCY_GENERATOR::thSetRange,this);
//	thr_range->detach();

	thread *thr_threshold = new thread(&EMERGENCY_GENERATOR::thSetThreshold,this);
	thr_threshold->detach();

	thread *thr_inhibit = new thread(&EMERGENCY_GENERATOR::thSetInhibit,this);
	thr_inhibit->detach();

	thread *thr_delay = new thread(&EMERGENCY_GENERATOR::thSetDelay,this);
	thr_delay->detach();

}

void EMERGENCY_GENERATOR::chksensor()
{
	checkSensorStatus();
}

void EMERGENCY_GENERATOR::resnd()
{
	resend();
}

void EMERGENCY_GENERATOR::reconn()
{
	reconnect();
}


string EMERGENCY_GENERATOR::property_reply(char flag,const emergency_generator& one,const string& msg_id)
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
			case 0:writer.Double(one.eg_dcu_unit_battery_volt_dcu_control_power);break;
			case 1:writer.Double(one.eg_lube_oil_pressure);break;
			case 2:writer.Double(one.eg_coolant_pressure);break;
			case 3:writer.Double(one.eg_coolant_temperature);break;
			case 4:writer.Double(one.eg_lube_oil_temperature);break;
			case 5:writer.Double(one.eg_engine_speed);break;
			case 6:writer.Double(one.eg_running_hours);break;
			case 7:writer.Double(one.eg_rpu_unit_battery_a_volt_rpu_main_control_power);break;
			case 8:writer.Double(one.eg_rpu_unit_battery_b_volt_rpu_backup_control_power);break;
			case 9:writer.Int(one.eg_coolant_level_low);break;
			case 10:writer.Int(one.eg_main_power_failure);break;
			case 11:writer.Int(one.eg_backup_power_failure);break;
			case 12:writer.Int(one.eg_no1_charger_failure);break;
			case 13:writer.Int(one.eg_no2_charger_failure);break;
			case 14:writer.Int(one.eg_local_emergency_stop);break;
			case 15:writer.Int(one.eg_remote_emergency_stop);break;
			case 16:writer.Int(one.eg_rpu_emergency_stop);break;
			case 17:writer.Int(one.eg_lube_oil_pressure_low_alarm);break;
			case 18:writer.Int(one.eg_coolant_pressure_low_alarm);break;
			case 19:writer.Int(one.eg_coolant_temperature_high_alarm);break;
			case 20:writer.Int(one.eg_lube_oil_temperature_high_alarm);break;
			case 21:writer.Int(one.eg_overspeed_alarm);break;
			case 22:writer.Int(one.eg_overspeed_shutdown);break;
			case 23:writer.Int(one.eg_speed_sensor_fail);break;
			case 24:writer.Int(one.eg_dcu_unit_battery_volt_dct_control_power_low);break;
			case 25:writer.Int(one.eg_start_fail);break;
			case 26:writer.Int(one.eg_rpu_unit_battery_a_volt_rpu_main_control_power_low);break;
			case 27:writer.Int(one.eg_rpu_unit_battery_b_volt_rpu_main_control_power_low);break;
			case 28:writer.Int(one.eg_common_alarm);break;
			case 29:writer.Int(one.eg_common_warning);break;
			case 30:writer.Int(one.eg_common_shutdown);break;
			case 31:writer.Int(one.eg_common_cooldown);break;
			case 32:writer.Int(one.eg_common_faultsensor_fail);break;
			case 33:writer.Int(one.eg_remote_control_status_auxaccess_lock);break;
			case 34:writer.Int(one.eg_ready_to_start);break;
			case 35:writer.Int(one.eg_ready_to_load);break;
			case 36:writer.Int(one.eg_generator_running);break;
			case 37:writer.Double(one.eg_generator_u_winding_temperature);break;
			case 38:writer.Double(one.eg_generator_v_winding_temperature);break;
			case 39:writer.Double(one.eg_generator_w_winding_temperature);break;
			case 40:writer.Int(one.device_offline);break;
#else
            case 0:writer.Double(one.eg_dcu_unit_battery_volt_dcu_control_power);break;
            case 1:writer.Double(one.eg_lube_oil_pressure);break;
            case 2:writer.Double(one.eg_coolant_pressure);break;
            case 3:writer.Double(one.eg_coolant_temperature);break;
            case 4:writer.Double(one.eg_lube_oil_temperature);break;
            case 5:writer.Double(one.eg_exhaust_temperature);break;
            case 6:writer.Double(one.eg_engine_speed);break;
            case 7:writer.Double(one.eg_running_hours);break;
            case 8:writer.Double(one.eg_rpu_unit_battery_a_volt_rpu_main_control_power);break;
            case 9:writer.Double(one.eg_rpu_unit_battery_b_volt_rpu_backup_control_power);break;
            case 10:writer.Int(one.eg_coolant_level_low);break;
            case 11:writer.Int(one.eg_main_power_failure);break;
            case 12:writer.Int(one.eg_backup_power_failure);break;
            case 13:writer.Int(one.eg_no1_charger_failure);break;
            case 14:writer.Int(one.eg_no2_charger_failure);break;
            case 15:writer.Int(one.eg_fuel_leakage_alarm);break;
            case 16:writer.Int(one.eg_local_emergency_stop);break;
            case 17:writer.Int(one.eg_remote_emergency_stop);break;
            case 18:writer.Int(one.eg_rpu_emergency_stop);break;
            case 19:writer.Int(one.eg_lube_oil_pressure_low_alarm);break;
            case 20:writer.Int(one.eg_coolant_pressure_low_alarm);break;
            case 21:writer.Int(one.eg_coolant_temperature_high_alarm);break;
            case 22:writer.Int(one.eg_lube_oil_temperature_high_alarm);break;
            case 23:writer.Int(one.eg_overspeed_alarm);break;
            case 24:writer.Int(one.eg_overspeed_shutdown);break;
            case 25:writer.Int(one.eg_speed_sensor_fail);break;
            case 26:writer.Int(one.eg_dcu_unit_battery_volt_dct_control_power_low);break;
            case 27:writer.Int(one.eg_start_fail);break;
            case 28:writer.Int(one.eg_rpu_unit_battery_a_volt_rpu_main_control_power_low);break;
            case 29:writer.Int(one.eg_rpu_unit_battery_b_volt_rpu_main_control_power_low);break;
            case 30:writer.Int(one.eg_common_alarm);break;
            case 31:writer.Int(one.eg_common_warning);break;
            case 32:writer.Int(one.eg_common_shutdown);break;
            case 33:writer.Int(one.eg_common_cooldowm);break;
            case 34:writer.Int(one.eg_common_faultsensor_fail);break;
            case 35:writer.Int(one.eg_remote_control_status_auxaccess_lock);break;
            case 36:writer.Int(one.eg_ready_to_start);break;
            case 37:writer.Int(one.eg_ready_to_load);break;
            case 38:writer.Int(one.eg_running);break;
            case 39:writer.Int(one.device_offline);break;

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
	writer.Key(emergency_generator_str[i++]);writer.Double(one.eg_dcu_unit_battery_volt_dcu_control_power);
	writer.Key(emergency_generator_str[i++]);writer.Double(one.eg_lube_oil_pressure);
	writer.Key(emergency_generator_str[i++]);writer.Double(one.eg_coolant_pressure);
	writer.Key(emergency_generator_str[i++]);writer.Double(one.eg_coolant_temperature);
	writer.Key(emergency_generator_str[i++]);writer.Double(one.eg_lube_oil_temperature);
	writer.Key(emergency_generator_str[i++]);writer.Double(one.eg_engine_speed);
	writer.Key(emergency_generator_str[i++]);writer.Double(one.eg_running_hours);
	writer.Key(emergency_generator_str[i++]);writer.Double(one.eg_rpu_unit_battery_a_volt_rpu_main_control_power);
	writer.Key(emergency_generator_str[i++]);writer.Double(one.eg_rpu_unit_battery_b_volt_rpu_backup_control_power);
	writer.Key(emergency_generator_str[i++]);writer.Int(one.eg_coolant_level_low);
	writer.Key(emergency_generator_str[i++]);writer.Int(one.eg_main_power_failure);
	writer.Key(emergency_generator_str[i++]);writer.Int(one.eg_backup_power_failure);
	writer.Key(emergency_generator_str[i++]);writer.Int(one.eg_no1_charger_failure);
	writer.Key(emergency_generator_str[i++]);writer.Int(one.eg_no2_charger_failure);
	writer.Key(emergency_generator_str[i++]);writer.Int(one.eg_local_emergency_stop);
	writer.Key(emergency_generator_str[i++]);writer.Int(one.eg_remote_emergency_stop);
	writer.Key(emergency_generator_str[i++]);writer.Int(one.eg_rpu_emergency_stop);
	writer.Key(emergency_generator_str[i++]);writer.Int(one.eg_lube_oil_pressure_low_alarm);
	writer.Key(emergency_generator_str[i++]);writer.Int(one.eg_coolant_pressure_low_alarm);
	writer.Key(emergency_generator_str[i++]);writer.Int(one.eg_coolant_temperature_high_alarm);
	writer.Key(emergency_generator_str[i++]);writer.Int(one.eg_lube_oil_temperature_high_alarm);
	writer.Key(emergency_generator_str[i++]);writer.Int(one.eg_overspeed_alarm);
	writer.Key(emergency_generator_str[i++]);writer.Int(one.eg_overspeed_shutdown);
	writer.Key(emergency_generator_str[i++]);writer.Int(one.eg_speed_sensor_fail);
	writer.Key(emergency_generator_str[i++]);writer.Int(one.eg_dcu_unit_battery_volt_dct_control_power_low);
	writer.Key(emergency_generator_str[i++]);writer.Int(one.eg_start_fail);
	writer.Key(emergency_generator_str[i++]);writer.Int(one.eg_rpu_unit_battery_a_volt_rpu_main_control_power_low);
	writer.Key(emergency_generator_str[i++]);writer.Int(one.eg_rpu_unit_battery_b_volt_rpu_main_control_power_low);
	writer.Key(emergency_generator_str[i++]);writer.Int(one.eg_common_alarm);
	writer.Key(emergency_generator_str[i++]);writer.Int(one.eg_common_warning);
	writer.Key(emergency_generator_str[i++]);writer.Int(one.eg_common_shutdown);
	writer.Key(emergency_generator_str[i++]);writer.Int(one.eg_common_cooldown);
	writer.Key(emergency_generator_str[i++]);writer.Int(one.eg_common_faultsensor_fail);
	writer.Key(emergency_generator_str[i++]);writer.Int(one.eg_remote_control_status_auxaccess_lock);
	writer.Key(emergency_generator_str[i++]);writer.Int(one.eg_ready_to_start);
	writer.Key(emergency_generator_str[i++]);writer.Int(one.eg_ready_to_load);
	writer.Key(emergency_generator_str[i++]);writer.Int(one.eg_generator_running);
	writer.Key(emergency_generator_str[i++]);writer.Double(one.eg_generator_u_winding_temperature);
	writer.Key(emergency_generator_str[i++]);writer.Double(one.eg_generator_v_winding_temperature);
	writer.Key(emergency_generator_str[i++]);writer.Double(one.eg_generator_w_winding_temperature);

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

void EMERGENCY_GENERATOR::init_mb_addr()
{
	int addr_ai = 8540;
	for(int i = 0; i < 3; i++)
	{
		m_mp_delay_addr[emergency_generator_str[i]] = addr_ai+2*i;
	}
	for(int i = 0; i < 2; i++)
	{
		m_mp_delay_addr[emergency_generator_str[3+i]] = addr_ai+10+2*i;
	}
	m_mp_delay_addr[emergency_generator_str[5]] = addr_ai+18;

	for(int i = 0; i < 2; i++)
	{
		m_mp_delay_addr[emergency_generator_str[7+i]] = addr_ai+20+2*i;
	}	
	
	addr_ai = 8500;
	for(int i = 0;i < 3;i++)
	{
		m_mp_delay_addr[emergency_generator_str[9+i]] = addr_ai+i;
	}
	
	for(int i = 12;i < 14;i++)
	{
		m_mp_delay_addr[emergency_generator_str[i+2]] =  addr_ai+i;
	}
	
	for(int i = 15;i < 18;i++)
	{
		m_mp_delay_addr[emergency_generator_str[i+1]] = addr_ai+i;
	}

	for(int i = 20;i < 22;i++)
	{
		m_mp_delay_addr[emergency_generator_str[i-1]] = addr_ai+i;
	}	
	
	for(int i = 24;i < 40;i++)
	{
		m_mp_delay_addr[emergency_generator_str[i-3]] = addr_ai+i;
	}

	addr_ai = 8240;
	for(int i = 0;i < 3;i++)
		m_mp_delay_addr[emergency_generator_str[37+i]] = addr_ai + 2*i;

	/////////////////////////////////////////////////////////////////
	for(int i = 0;i < 3;i++)
	{
		m_mp_inhibit_addr[emergency_generator_str[9+i]] = i;
	}
	
	//eg_no1_charger_failure
	//eg_no2_charger_failure
	
	for(int i = 12;i < 14;i++)
	{
		m_mp_inhibit_addr[emergency_generator_str[2+i]] = i;
	}
	
	m_mp_inhibit_addr[emergency_generator_str[16]] = 15;
	
	for(int i = 0;i < 2;i++)
	{
		m_mp_inhibit_addr[emergency_generator_str[17+i]] = i + 100;
	}
	
	for(int i = 4;i < 6;i++)
	{
		m_mp_inhibit_addr[emergency_generator_str[15+i]] = i + 100;
	}
	
	for(int i = 8;i < 16;i++)
	{
		m_mp_inhibit_addr[emergency_generator_str[13+i]] = i + 100;
	}
	
	for(int i = 0;i < 8;i++)
	{
		m_mp_inhibit_addr[emergency_generator_str[29+i]] = i + 200;
	}
	
	for(int i = 0; i < 3; i++)
	{
		m_mp_inhibit_addr[emergency_generator_str[i]] = 2 * i + 300;
	}

	for(int i = 5; i < 7; i++)
	{
		m_mp_inhibit_addr[emergency_generator_str[i-2]] = 2 * i + 300;
	}	

	m_mp_inhibit_addr[emergency_generator_str[5]] = 402;
	m_mp_inhibit_addr[emergency_generator_str[7]] = 404;
	m_mp_inhibit_addr[emergency_generator_str[8]] = 406;

	////////////////////////////////////////////////////////////////
	addr_ai = 2830;
	for(int i = 0; i < 3; i++)
	{
		m_mp_threshold_addr[emergency_generator_str[37 + i]] = addr_ai + i;
//cout<<"init_mb emerge:"<<emergency_generator_str[37 + i]<<" "<<	addr_ai + i<<endl;sleep(3);	
	}

}


void EMERGENCY_GENERATOR::thSetRange()
{
	setRange();
}

void EMERGENCY_GENERATOR::thSetThreshold()
{
	setThreshold();
}

void EMERGENCY_GENERATOR::thSetDelay()
{
	setDelay();
}


void EMERGENCY_GENERATOR::thSetInhibit()
{
	setInhibit(5600,5);
}

bool EMERGENCY_GENERATOR::UpdateHa(std::string key, uint16_t *data, int size)
{
	bool ret = false;
	for(int i = 0; i < sizeof(emergency_generator_str); ++i)
	{
        std::string s(emergency_generator_str[i]);
		if(s == key)
		{
			std::pair<uint8_t, uint8_t> byte_bit{0xff, 0xff};
            switch(i)
			{
                case 0:{byte_bit = m_data_store.eg_dcu_unit_battery_volt_dcu_control_power.ha_bit;break;}
                case 1:{byte_bit = m_data_store.eg_lube_oil_pressure.ha_bit;break;}
                case 2:{byte_bit = m_data_store.eg_coolant_pressure.ha_bit;break;}
                case 3:{byte_bit = m_data_store.eg_coolant_temperature.ha_bit;break;}
                case 4:{byte_bit = m_data_store.eg_lube_oil_temperature.ha_bit;break;}
                case 5:{byte_bit = m_data_store.eg_engine_speed.ha_bit;break;}
                case 6:{byte_bit = m_data_store.eg_rpu_unit_battery_a_volt_rpu_main_control_power.ha_bit;break;}
                case 7:{byte_bit = m_data_store.eg_rpu_unit_battery_b_volt_rpu_backup_control_power.ha_bit;break;}
                case 8:{byte_bit = m_data_store.eg_running_hours.ha_bit;break;}
                case 9:{byte_bit = m_data_store.eg_coolant_level_low.ha_bit;break;}
                case 10:{byte_bit = m_data_store.eg_main_power_failure.ha_bit;break;}
                case 11:{byte_bit = m_data_store.eg_backup_power_failure.ha_bit;break;}
                case 12:{byte_bit = m_data_store.eg_no1_charger_failure.ha_bit;break;}
                case 13:{byte_bit = m_data_store.eg_no2_charger_failure.ha_bit;break;}
                case 14:{byte_bit = m_data_store.eg_local_emergency_stop.ha_bit;break;}
                case 15:{byte_bit = m_data_store.eg_remote_emergency_stop.ha_bit;break;}
                case 16:{byte_bit = m_data_store.eg_rpu_emergency_stop.ha_bit;break;}
                case 17:{byte_bit = m_data_store.eg_lube_oil_pressure_low_alarm.ha_bit;break;}
                case 18:{byte_bit = m_data_store.eg_coolant_pressure_low_alarm.ha_bit;break;}
                case 19:{byte_bit = m_data_store.eg_coolant_temperature_high_alarm.ha_bit;break;}
                case 20:{byte_bit = m_data_store.eg_lube_oil_temperature_high_alarm.ha_bit;break;}
                case 21:{byte_bit = m_data_store.eg_overspeed_alarm.ha_bit;break;}
                case 22:{byte_bit = m_data_store.eg_overspeed_shutdown.ha_bit;break;}
                case 23:{byte_bit = m_data_store.eg_speed_sensor_fail.ha_bit;break;}
                case 24:{byte_bit = m_data_store.eg_dcu_unit_battery_volt_dct_control_power_low.ha_bit;break;}
                case 25:{byte_bit = m_data_store.eg_start_fail.ha_bit;break;}
                case 26:{byte_bit = m_data_store.eg_rpu_unit_battery_a_volt_rpu_main_control_power_low.ha_bit;break;}
                case 27:{byte_bit = m_data_store.eg_rpu_unit_battery_b_volt_rpu_main_control_power_low.ha_bit;break;}
                case 28:{byte_bit = m_data_store.eg_common_alarm.ha_bit;break;}
                case 29:{byte_bit = m_data_store.eg_common_warning.ha_bit;break;}
                case 30:{byte_bit = m_data_store.eg_common_shutdown.ha_bit;break;}
                case 32:{byte_bit = m_data_store.eg_common_faultsensor_fail.ha_bit;break;}
                case 33:{byte_bit = m_data_store.eg_remote_control_status_auxaccess_lock.ha_bit;break;}
                case 34:{byte_bit = m_data_store.eg_ready_to_start.ha_bit;break;}
                case 35:{byte_bit = m_data_store.eg_ready_to_load.ha_bit;break;}
                case 36:{byte_bit = m_data_store.eg_generator_running.ha_bit;break;}
                case 37:{byte_bit = m_data_store.eg_generator_u_winding_temperature.ha_bit;break;}
                case 38:{byte_bit = m_data_store.eg_generator_v_winding_temperature.ha_bit;break;}
                case 39:{byte_bit = m_data_store.eg_generator_w_winding_temperature.ha_bit;break;}
                case 40:{byte_bit = m_data_store.device_offline.ha_bit;break;}
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