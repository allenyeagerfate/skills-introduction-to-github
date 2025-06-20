#include <iostream>
#include <unistd.h>


#include "sgo_sub_pub.h"
#include "sgo_func.h"
#include "sgo_ams/main_diesel_generator.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
//#include "main_diesel_generator.h"

using namespace std;

const char* main_diesel_generator_str[]={
#ifndef MAHTS648
	"d_g_dcu_unit_battery_volt_dcu_control_power",
	"d_g_lube_oil_pressure",
	"d_g_coolant_pressure",
	"d_g_start_air_pressure",
	"d_g_fuel_oil_pressure",
	"d_g_coolant_temperature",
	"d_g_lube_oil_temperature",
	"d_g_left_exhaust_temperature",
	"d_g_right_exhaust_temperature",
	"d_g_engine_speed",
	"d_g_running_hours",
	"d_g_rpu_unit_battery_a_volt_rpu_main_control_power",
	"d_g_rpu_unit_battery_b_volt_rpu_backup_control_power",
	"d_g_coolant_level_low",
	"d_g_main_power_failure",
	"d_g_backup_power_failure",
	"d_g_sea_water_pump_overload_trip",
	"d_g_ecr_remote_emergency_stop",
	"d_g_aft_bridge_remote_emergency_stop",
	"d_g_fwd_bridge_remote_emergency_stop",
	"d_g_lube_oil_pressure_too_low_shutdown",
	"d_g_coolant_temperature_too_high_shutdown",
	"d_g_local_emergency_stop",
	"d_g_remote_emergency_stop",
	"d_g_rpu_emergency_stop",
	"d_g_lube_oil_pressure_low_alarm",
	"d_g_coolant_pressure_low_alarm",
	"d_g_start_air_pressure_low_alarm",
	"d_g_fuel_oil_pressure_low_alarm",
	"d_g_coolant_temperature_high_alarm",
	"d_g_lube_oil_temperature_high_alarm",
	"d_g_left_exhaust_temperature_high_alarm",
	"d_g_right_exhaust_temperature_high_alarm",
	"d_g_overspeed_alarm",
	"d_g_overspeed_shutdown",
	"d_g_speed_sensor_fail",
	"d_g_dcu_unit_battery_volt_dct_control_power_low",
	"d_g_start_fail",
	"d_g_rpu_unit_battery_a_volt_rpu_main_control_power_low",
	"d_g_rpu_unit_battery_b_volt_rpu_main_control_power_low",
	"d_g_common_alarm",
	"d_g_common_warning",
	"d_g_common_shutdown",
	//"d_g_common_cooldown",
	"d_g_common_faultsensor_fail",
	"d_g_remote_control_status_auxaccess_lock",
	"d_g_ready_to_start",
	"d_g_ready_to_load",
	"d_g_main_generator_running",
	"d_g_main_generator_u_winding_temperature",
	"d_g_main_generator_v_winding_temperature",
	"d_g_main_generator_w_winding_temperature",
	"d_g_main_generator_de_bearing_temperature",
	"d_g_main_generator_nde_bearing_temperature",
	"d_g_main_generator_excitation_current",
	"d_g_main_generator_excitation_voltage",
	"device_offline",
#else
    "d_g_dcu_unit_battery_volt_dcu_control_power",
    "d_g_lube_oil_pressure",
    "d_g_coolant_pressure",
    "d_g_coolant_temperature",
    "d_g_lube_oil_temperature",
    "d_g_left_exhaust_temperature",
    "d_g_right_exhaust_temperature",
    "d_g_engine_speed",
    "d_g_running_hours",
    "d_g_rpu_unit_battery_a_volt_rpu_main_control_power",
    "d_g_rpu_unit_battery_b_volt_rpu_backup_control_power",
    "d_g_coolant_level_low",
    "d_g_main_power_failure",
    "d_g_backup_power_failure",
    "d_g_charger_failure",
    "d_g_alternator_cooling_water_leakage",
    "d_g_ecr_remote_emergency_stop",
    "d_g_lube_oil_pressure_too_low_shutdown",
    "d_g_coolant_temperature_too_high_shutdown",
    "d_g_local_emergency_stop",
    "d_g_remote_emergency_stop",
    "d_g_rpu_emergency_stop",
    "d_g_lube_oil_pressure_low_alarm",
    "d_g_coolant_pressure_low_alarm",
    "d_g_coolant_temperature_high_alarm",
    "d_g_lube_oil_temperature_high_alarm",
    "d_g_left_exhaust_temperature_high_alarm",
    "d_g_right_exhaust_temperature_high_alarm",
    "d_g_overspeed_alarm",
    "d_g_overspeed_shutdown",
    "d_g_speed_pickup_sensor_fail",
    "d_g_dcu_unit_battery_volt_dct_control_power_low",
    "d_g_start_fail",
    "d_g_rpu_unit_battery_a_volt_rpu_main_control_power_low",
    "d_g_rpu_unit_battery_b_volt_rpu_main_control_power_low",
    "d_g_common_alarm",
    "d_g_common_warning",
    "d_g_common_shutdown",
    "d_g_common_cooldowm",
    "d_g_common_faultsensor_fail",
    "d_g_remote_control_status_auxaccess_lock",
    "d_g_ready_to_start",
    "d_g_ready_to_load",
    "d_g_running",
    "d_g_u_winding_temperature",
    "d_g_v_winding_temperature",
    "d_g_w_winding_temperature",
    "d_g_alternator_cooling_water_inlet_temp",
    "d_g_alternator_cooling_water_outlet_temp",
    "d_g_de_bearing_temperature",
    "d_g_nde_bearing_temperature",
    "d_g_colling_air_cold_temperature",
    "d_g_colling_air_warm_temperature",
    "d_g_fresh_water_cooling_pump_outlet_pressure",
    "d_g_sea_water_cooling_pump_outlet_pressure",
    "d_g_inlet_oil_flowmeter",
    "d_g_outlet_oil_flowmeter",
    "device_offline",    
#endif
};
	
static const char* productCode = "main_generator";

MAIN_DIESEL_GENERATOR::MAIN_DIESEL_GENERATOR()
{}

MAIN_DIESEL_GENERATOR::~MAIN_DIESEL_GENERATOR()
{}



void MAIN_DIESEL_GENERATOR::init(int devId,BlockQueue<main_diesel_generator> &bq,modbus *mb,mutex *mux)
{

	m_mb_plc1 = mb;
	m_mux_plc1 = mux;

	m_devId = devId;

	//init_mb_addr();
	
	m_clientId.clear();
	m_clientId.append(productCode);
	m_clientId.append("_device");
	m_clientId.append(to_string(devId));

	m_productCode = productCode;
	
	sgo_log_stderr(0, "#######################################%s %d start\n",m_productCode.c_str(), m_devId);
	setSubTopics();
	setPubTopics();
	
	start(bq);

}




// 连接丢失回调函数
void MAIN_DIESEL_GENERATOR::connectionLostCallback(void* context, char* cause) 
{
	// 处理连接丢失事件
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	MAIN_DIESEL_GENERATOR* client = static_cast<MAIN_DIESEL_GENERATOR*>(ctx->manager);
	
	printf("--%s %d--address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());
	sgo_log_stderr(0,"--%s %d----address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());

	//std::lock_guard<std::mutex> lock(m_losts_mutex);
	client->m_v_lost[ctx->index] = 1;
	client->m_v_property_index_array.clear();

}


// 消息到达回调函数
int MAIN_DIESEL_GENERATOR::messageArrivedCallback(void* context, char* topicName, int topicLen,MQTTClient_message* message) 
{
	// 处理接收到的消息
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	MAIN_DIESEL_GENERATOR* client = static_cast<MAIN_DIESEL_GENERATOR*>(ctx->manager);
	
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
			client->m_property[main_diesel_generator_str[i]]=i;
		
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
void MAIN_DIESEL_GENERATOR::deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt) 
{
	// 处理交付完成事件
	//std::cout << "Message delivery complete." << std::endl;
}

void MAIN_DIESEL_GENERATOR::subClient()
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


void MAIN_DIESEL_GENERATOR::pubClient(BlockQueue<main_diesel_generator>& bq)
{
	while(1)
	{
		MQTTClient_message pubmsg = MQTTClient_message_initializer;
		//声明消息token
		MQTTClient_deliveryToken token;

		while(!m_pubStop){
			main_diesel_generator one = bq.take();
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




void MAIN_DIESEL_GENERATOR::start(BlockQueue<main_diesel_generator> &bq)
{
	m_sub = new thread(&MAIN_DIESEL_GENERATOR::subClient,this);
	m_sub->detach();

	m_pub = new thread(&MAIN_DIESEL_GENERATOR::pubClient,this,ref(bq));
	m_pub->detach();

	m_thr_reconn = new thread(&MAIN_DIESEL_GENERATOR::reconn,this);
	m_thr_reconn->detach();

	m_thr_resend = new thread(&MAIN_DIESEL_GENERATOR::resnd,this);
	m_thr_resend->detach();

	m_thr_chksensor = new thread(&MAIN_DIESEL_GENERATOR::chksensor,this);
	m_thr_chksensor->detach();

	thread *thr_range = new thread(&MAIN_DIESEL_GENERATOR::thSetRange,this);
	thr_range->detach();

	thread *thr_threshold = new thread(&MAIN_DIESEL_GENERATOR::thSetThreshold,this);
	thr_threshold->detach();

	thread *thr_inhibit = new thread(&MAIN_DIESEL_GENERATOR::thSetInhibit,this);
	thr_inhibit->detach();

	thread *thr_delay = new thread(&MAIN_DIESEL_GENERATOR::thSetDelay,this);
	thr_delay->detach();	
}

void MAIN_DIESEL_GENERATOR::chksensor()
{
	checkSensorStatus();
}

void MAIN_DIESEL_GENERATOR::resnd()
{
	resend();
}

void MAIN_DIESEL_GENERATOR::reconn()
{
	reconnect();
}


string MAIN_DIESEL_GENERATOR::property_reply(char flag,const main_diesel_generator& one,const string& msg_id)
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

    writer.StartObject();

	writer.Key("msgId");
	writer.String(msgId.c_str());

	writer.Key("success");
	writer.Bool(true);


	if(m_deviceCode.empty())
		m_deviceCode = m_clientId;


//auto start = std::chrono::high_resolution_clock::now();
#if 1

	if(0 == m_property_size)
		return string();
	if(m_v_index.empty())
		return string();

	writer.Key("compressData");
	writer.StartArray();

	for(int i = 0; i < m_v_index.size(); i++)
	{
		switch(m_v_index[i]){
#ifndef MAHTS648
			case 0:writer.Double(one.d_g_dcu_unit_battery_volt_dcu_control_power);break;
			case 1:writer.Double(one.d_g_lube_oil_pressure);break;
			case 2:writer.Double(one.d_g_coolant_pressure);break;
			case 3:writer.Double(one.d_g_start_air_pressure);break;
			case 4:writer.Double(one.d_g_fuel_oil_pressure);break;
			case 5:writer.Double(one.d_g_coolant_temperature);break;
			case 6:writer.Double(one.d_g_lube_oil_temperature);break;
			case 7:writer.Double(one.d_g_left_exhaust_temperature);break;
			case 8:writer.Double(one.d_g_right_exhaust_temperature);break;
			case 9:writer.Double(one.d_g_engine_speed);break;
			case 10:writer.Double(one.d_g_running_hours);break;
			case 11:writer.Double(one.d_g_rpu_unit_battery_a_volt_rpu_main_control_power);break;
			case 12:writer.Double(one.d_g_rpu_unit_battery_b_volt_rpu_backup_control_power);break;
			case 13:writer.Int(one.d_g_coolant_level_low);break;
			case 14:writer.Int(one.d_g_main_power_failure);break;
			case 15:writer.Int(one.d_g_backup_power_failure);break;
			case 16:writer.Int(one.d_g_sea_water_pump_overload_trip);break;
			case 17:writer.Int(one.d_g_ecr_remote_emergency_stop);break;
			case 18:writer.Int(one.d_g_aft_bridge_remote_emergency_stop);break;
			case 19:writer.Int(one.d_g_fwd_bridge_remote_emergency_stop);break;
			case 20:writer.Int(one.d_g_lube_oil_pressure_too_low_shutdown);break;
			case 21:writer.Int(one.d_g_coolant_temperature_too_high_shutdown);break;
			case 22:writer.Int(one.d_g_local_emergency_stop);break;
			case 23:writer.Int(one.d_g_remote_emergency_stop);break;
			case 24:writer.Int(one.d_g_rpu_emergency_stop);break;
			case 25:writer.Int(one.d_g_lube_oil_pressure_low_alarm);break;
			case 26:writer.Int(one.d_g_coolant_pressure_low_alarm);break;
			case 27:writer.Int(one.d_g_start_air_pressure_low_alarm);break;
			case 28:writer.Int(one.d_g_fuel_oil_pressure_low_alarm);break;
			case 29:writer.Int(one.d_g_coolant_temperature_high_alarm);break;
			case 30:writer.Int(one.d_g_lube_oil_temperature_high_alarm);break;
			case 31:writer.Int(one.d_g_left_exhaust_temperature_high_alarm);break;
			case 32:writer.Int(one.d_g_right_exhaust_temperature_high_alarm);break;
			case 33:writer.Int(one.d_g_overspeed_alarm);break;
			case 34:writer.Int(one.d_g_overspeed_shutdown);break;
			case 35:writer.Int(one.d_g_speed_sensor_fail);break;
			case 36:writer.Int(one.d_g_dcu_unit_battery_volt_dct_control_power_low);break;
			case 37:writer.Int(one.d_g_start_fail);break;
			case 38:writer.Int(one.d_g_rpu_unit_battery_a_volt_rpu_main_control_power_low);break;
			case 39:writer.Int(one.d_g_rpu_unit_battery_b_volt_rpu_main_control_power_low);break;
			case 40:writer.Int(one.d_g_common_alarm);break;
			case 41:writer.Int(one.d_g_common_warning);break;
			case 42:writer.Int(one.d_g_common_shutdown);break;
			//case 43:writer.Int(one.d_g_common_cooldown);break;
			case 43:writer.Int(one.d_g_common_faultsensor_fail);break;
			case 44:writer.Int(one.d_g_remote_control_status_auxaccess_lock);break;
			case 45:writer.Int(one.d_g_ready_to_start);break;
			case 46:writer.Int(one.d_g_ready_to_load);break;
			case 47:writer.Int(one.d_g_main_generator_running);break;
			case 48:writer.Double(one.d_g_main_generator_u_winding_temperature);break;
			case 49:writer.Double(one.d_g_main_generator_v_winding_temperature);break;
			case 50:writer.Double(one.d_g_main_generator_w_winding_temperature);break;
			case 51:writer.Double(one.d_g_main_generator_de_bearing_temperature);break;
			case 52:writer.Double(one.d_g_main_generator_nde_bearing_temperature);break;
			case 53:writer.Double(one.d_g_main_generator_excitation_current);break;
			case 54:writer.Double(one.d_g_main_generator_excitation_voltage);break;
			case 55:writer.Int(one.device_offline);break;
#else
case 0:writer.Double(one.d_g_dcu_unit_battery_volt_dcu_control_power);break;
case 1:writer.Double(one.d_g_lube_oil_pressure);break;
case 2:writer.Double(one.d_g_coolant_pressure);break;
case 3:writer.Double(one.d_g_coolant_temperature);break;
case 4:writer.Double(one.d_g_lube_oil_temperature);break;
case 5:writer.Double(one.d_g_left_exhaust_temperature);break;
case 6:writer.Double(one.d_g_right_exhaust_temperature);break;
case 7:writer.Double(one.d_g_engine_speed);break;
case 8:writer.Double(one.d_g_running_hours);break;
case 9:writer.Double(one.d_g_rpu_unit_battery_a_volt_rpu_main_control_power);break;
case 10:writer.Double(one.d_g_rpu_unit_battery_b_volt_rpu_backup_control_power);break;
case 11:writer.Int(one.d_g_coolant_level_low);break;
case 12:writer.Int(one.d_g_main_power_failure);break;
case 13:writer.Int(one.d_g_backup_power_failure);break;
case 14:writer.Int(one.d_g_charger_failure);break;
case 15:writer.Int(one.d_g_alternator_cooling_water_leakage);break;
case 16:writer.Int(one.d_g_ecr_remote_emergency_stop);break;
case 17:writer.Int(one.d_g_lube_oil_pressure_too_low_shutdown);break;
case 18:writer.Int(one.d_g_coolant_temperature_too_high_shutdown);break;
case 19:writer.Int(one.d_g_local_emergency_stop);break;
case 20:writer.Int(one.d_g_remote_emergency_stop);break;
case 21:writer.Int(one.d_g_rpu_emergency_stop);break;
case 22:writer.Int(one.d_g_lube_oil_pressure_low_alarm);break;
case 23:writer.Int(one.d_g_coolant_pressure_low_alarm);break;
case 24:writer.Int(one.d_g_coolant_temperature_high_alarm);break;
case 25:writer.Int(one.d_g_lube_oil_temperature_high_alarm);break;
case 26:writer.Int(one.d_g_left_exhaust_temperature_high_alarm);break;
case 27:writer.Int(one.d_g_right_exhaust_temperature_high_alarm);break;
case 28:writer.Int(one.d_g_overspeed_alarm);break;
case 29:writer.Int(one.d_g_overspeed_shutdown);break;
case 30:writer.Int(one.d_g_speed_sensor_fail);break;
case 31:writer.Int(one.d_g_dcu_unit_battery_volt_dct_control_power_low);break;
case 32:writer.Int(one.d_g_start_fail);break;
case 33:writer.Int(one.d_g_rpu_unit_battery_a_volt_rpu_main_control_power_low);break;
case 34:writer.Int(one.d_g_rpu_unit_battery_b_volt_rpu_main_control_power_low);break;
case 35:writer.Int(one.d_g_common_alarm);break;
case 36:writer.Int(one.d_g_common_warning);break;
case 37:writer.Int(one.d_g_common_shutdown);break;
case 38:writer.Int(one.d_g_common_cooldowm);break;
case 39:writer.Int(one.d_g_common_faultsensor_fail);break;
case 40:writer.Int(one.d_g_remote_control_status_auxaccess_lock);break;
case 41:writer.Int(one.d_g_ready_to_start);break;
case 42:writer.Int(one.d_g_ready_to_load);break;
case 43:writer.Int(one.d_g_running);break;
case 44:writer.Double(one.d_g_u_winding_temperature);break;
case 45:writer.Double(one.d_g_v_winding_temperature);break;
case 46:writer.Double(one.d_g_w_winding_temperature);break;
case 47:writer.Double(one.d_g_alternator_cooling_water_inlet_temp);break;
case 48:writer.Double(one.d_g_alternator_cooling_water_outlet_temp);break;
case 49:writer.Double(one.d_g_de_bearing_temperature);break;
case 50:writer.Double(one.d_g_nde_bearing_temperature);break;
case 51:writer.Double(one.d_g_colling_air_cold_temperature);break;
case 52:writer.Double(one.d_g_colling_air_warm_temperature);break;
case 53:writer.Double(one.d_g_fresh_water_cooling_pump_outlet_pressure);break;
case 54:writer.Double(one.d_g_sea_water_cooling_pump_outlet_pressure);break;
case 55:writer.Double(one.d_g_inlet_oil_flowmeter);break;
case 56:writer.Double(one.d_g_outlet_oil_flowmeter);break;
case 57:writer.Int(one.device_offline);break;

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
	writer.Key(main_diesel_generator_str[i++]);writer.Double(one.d_g_dcu_unit_battery_volt_dcu_control_power);
	writer.Key(main_diesel_generator_str[i++]);writer.Double(one.d_g_lube_oil_pressure);
	writer.Key(main_diesel_generator_str[i++]);writer.Double(one.d_g_coolant_pressure);
	writer.Key(main_diesel_generator_str[i++]);writer.Double(one.d_g_start_air_pressure);
	writer.Key(main_diesel_generator_str[i++]);writer.Double(one.d_g_fuel_oil_pressure);
	writer.Key(main_diesel_generator_str[i++]);writer.Double(one.d_g_coolant_temperature);
	writer.Key(main_diesel_generator_str[i++]);writer.Double(one.d_g_lube_oil_temperature);
	writer.Key(main_diesel_generator_str[i++]);writer.Double(one.d_g_left_exhaust_temperature);
	writer.Key(main_diesel_generator_str[i++]);writer.Double(one.d_g_right_exhaust_temperature);
	writer.Key(main_diesel_generator_str[i++]);writer.Double(one.d_g_engine_speed);
	writer.Key(main_diesel_generator_str[i++]);writer.Double(one.d_g_running_hours);
	writer.Key(main_diesel_generator_str[i++]);writer.Double(one.d_g_rpu_unit_battery_a_volt_rpu_main_control_power);
	writer.Key(main_diesel_generator_str[i++]);writer.Double(one.d_g_rpu_unit_battery_b_volt_rpu_backup_control_power);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_coolant_level_low);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_main_power_failure);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_backup_power_failure);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_sea_water_pump_overload_trip);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_ecr_remote_emergency_stop);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_aft_bridge_remote_emergency_stop);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_fwd_bridge_remote_emergency_stop);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_lube_oil_pressure_too_low_shutdown);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_coolant_temperature_too_high_shutdown);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_local_emergency_stop);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_remote_emergency_stop);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_rpu_emergency_stop);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_lube_oil_pressure_low_alarm);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_coolant_pressure_low_alarm);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_start_air_pressure_low_alarm);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_fuel_oil_pressure_low_alarm);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_coolant_temperature_high_alarm);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_lube_oil_temperature_high_alarm);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_left_exhaust_temperature_high_alarm);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_right_exhaust_temperature_high_alarm);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_overspeed_alarm);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_overspeed_shutdown);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_speed_sensor_fail);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_dcu_unit_battery_volt_dct_control_power_low);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_start_fail);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_rpu_unit_battery_a_volt_rpu_main_control_power_low);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_rpu_unit_battery_b_volt_rpu_main_control_power_low);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_common_alarm);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_common_warning);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_common_shutdown);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_common_cooldown);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_common_faultsensor_fail);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_remote_control_status_auxaccess_lock);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_ready_to_start);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_ready_to_load);
	writer.Key(main_diesel_generator_str[i++]);writer.Int(one.d_g_main_generator_running);
	writer.Key(main_diesel_generator_str[i++]);writer.Double(one.d_g_main_generator_u_winding_temperature);
	writer.Key(main_diesel_generator_str[i++]);writer.Double(one.d_g_main_generator_v_winding_temperature);
	writer.Key(main_diesel_generator_str[i++]);writer.Double(one.d_g_main_generator_w_winding_temperature);
	writer.Key(main_diesel_generator_str[i++]);writer.Double(one.d_g_main_generator_de_bearing_temperature);
	writer.Key(main_diesel_generator_str[i++]);writer.Double(one.d_g_main_generator_nde_bearing_temperature);
	writer.Key(main_diesel_generator_str[i++]);writer.Double(one.d_g_main_generator_excitation_current);
	writer.Key(main_diesel_generator_str[i++]);writer.Double(one.d_g_main_generator_excitation_voltage);
	
	writer.EndObject();
#endif	
//auto end = std::chrono::high_resolution_clock::now();
//auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
//std::cout << "Time taken: " << duration << " microseconds" << std::endl;

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
//if(m_devId == 2)
//cout <<"main diesel:"<< data << endl;
	return strBuf.GetString();
}

void MAIN_DIESEL_GENERATOR::thSetRange()
{
	setRange();
}


void MAIN_DIESEL_GENERATOR::thSetThreshold()
{
	setThreshold();
}

void MAIN_DIESEL_GENERATOR::thSetDelay()
{
	setDelay();
}

void MAIN_DIESEL_GENERATOR::thSetInhibit()
{
	setInhibit(5000 + 10*m_devId,5);
}


void MAIN_DIESEL_GENERATOR::init_mb_addr()
{
	int addr_ai;
	m_mp_delay_addr.clear();
	m_mp_inhibit_addr.clear();
	m_mp_threshold_addr.clear();
	m_mp_range_addr.clear();
	

	addr_ai = 6300 + 200 *(m_devId - 1); 
	for(int i = 0; i < 10; i++)
		m_mp_delay_addr[main_diesel_generator_str[i]] = addr_ai + 40 + 2*i;
	for(int i = 0; i < 2; i++)
		m_mp_delay_addr[main_diesel_generator_str[11+i]] = addr_ai + 60 + 2*i;

	for(int i = 0;i < 3;i++)
	{
		m_mp_delay_addr[main_diesel_generator_str[13+i]] = addr_ai + i;
	}
	
	for(int i = 5;i < 9;i++)
	{
		m_mp_delay_addr[main_diesel_generator_str[11+i]] = addr_ai + i;
	}
	
	for(int i = 10;i < 14;i++)
	{
		m_mp_delay_addr[main_diesel_generator_str[10+i]] = addr_ai+i;
	}
	
	for(int i = 15;i < 40;i++)
	{
		m_mp_delay_addr[main_diesel_generator_str[9+i]] = addr_ai+i;
	}

	//PT100
	addr_ai = 8180 + 10 * (m_devId - 1);
	for(int i = 0; i < 5; i++)
		m_mp_delay_addr[main_diesel_generator_str[49+i]] = addr_ai + 2*i;

	//4-20mA
	addr_ai = 8260 + 4 * (m_devId - 1);
	for(int i = 0; i < 2; i++)
		m_mp_delay_addr[main_diesel_generator_str[54 + i]] = addr_ai + 2*i;

	/////////////////////////////////////////////////////
	for(int i = 0; i < 8; i++)
	{
		m_mp_inhibit_addr[main_diesel_generator_str[i]] = 2*i + 300;
	}
	m_mp_inhibit_addr[main_diesel_generator_str[8]] = 400;
	m_mp_inhibit_addr[main_diesel_generator_str[9]] = 402;
	m_mp_inhibit_addr[main_diesel_generator_str[11]] = 404;
	m_mp_inhibit_addr[main_diesel_generator_str[12]] = 406;
	
	/////////////////////
	for(int i = 0;i < 3;i++)
	{
		m_mp_inhibit_addr[main_diesel_generator_str[13+i]] = i;
	}
	
	for(int i = 5;i < 9;i++)
	{
		m_mp_inhibit_addr[main_diesel_generator_str[11+i]] = i;
	}
	
	for(int i = 10;i < 14;i++)
	{
		m_mp_inhibit_addr[main_diesel_generator_str[10+i]] = i;
	}
	
	m_mp_inhibit_addr[main_diesel_generator_str[24]] = 15;

	for(int i = 0;i < 16;i++)
	{
		m_mp_inhibit_addr[main_diesel_generator_str[25+i]] = i + 100;
	}
	
	for(int i = 0;i < 8;i++)
	{
		m_mp_inhibit_addr[main_diesel_generator_str[41+i]] = i + 200;
	}
	
	//PT100
	addr_ai = 2800 + 5*(m_devId-1);
	for(int i = 0; i < 5; i++)
	{
		m_mp_threshold_addr[main_diesel_generator_str[49+i]] = addr_ai + i;
		printf("main--------:%s:%d\n",main_diesel_generator_str[49+i],addr_ai+i);
	}
	//4-20mA range
	addr_ai = 9780 + 4*(m_devId - 1);
	for(int i = 0; i < 2; i++)
		m_mp_range_addr[main_diesel_generator_str[54+i]] = addr_ai + 2 * i;
	
}


bool MAIN_DIESEL_GENERATOR::UpdateHa(std::string key, uint16_t *data, int size)
{
	bool ret = false;
	for(int i = 0; i < sizeof(main_diesel_generator_str); ++i)
	{
        std::string s(main_diesel_generator_str[i]);
		if(s == key)
		{
			std::pair<uint8_t, uint8_t> byte_bit{0xff, 0xff};
            switch(i)
			{
			case 0:{byte_bit = m_data_store.d_g_dcu_unit_battery_volt_dcu_control_power.ha_bit;break;}
            case 1:{byte_bit = m_data_store.d_g_lube_oil_pressure.ha_bit;break;}
            case 2:{byte_bit = m_data_store.d_g_coolant_pressure.ha_bit;break;}
            case 3:{byte_bit = m_data_store.d_g_start_air_pressure.ha_bit;break;}
            case 4:{byte_bit = m_data_store.d_g_fuel_oil_pressure.ha_bit;break;}
            case 5:{byte_bit = m_data_store.d_g_coolant_temperature.ha_bit;break;}
            case 6:{byte_bit = m_data_store.d_g_lube_oil_temperature.ha_bit;break;}
            case 7:{byte_bit = m_data_store.d_g_left_exhaust_temperature .ha_bit;break;}
            case 8:{byte_bit = m_data_store.d_g_right_exhaust_temperature.ha_bit;break;}
            case 9:{byte_bit = m_data_store.d_g_engine_speed.ha_bit;break;}
            case 10:{byte_bit = m_data_store.d_g_running_hours.ha_bit;break;}
            case 11:{byte_bit = m_data_store.d_g_rpu_unit_battery_a_volt_rpu_main_control_power.ha_bit;break;}
            case 12:{byte_bit = m_data_store.d_g_rpu_unit_battery_b_volt_rpu_backup_control_power.ha_bit;break;}
            case 13:{byte_bit = m_data_store.d_g_coolant_level_low.ha_bit;break;}
            case 14:{byte_bit = m_data_store.d_g_main_power_failure.ha_bit;break;}
            case 15:{byte_bit = m_data_store.d_g_backup_power_failure.ha_bit;break;}
            case 16:{byte_bit = m_data_store.d_g_sea_water_pump_overload_trip.ha_bit;break;}
            case 17:{byte_bit = m_data_store.d_g_ecr_remote_emergency_stop.ha_bit;break;}
            case 18:{byte_bit = m_data_store.d_g_aft_bridge_remote_emergency_stop.ha_bit;break;}
            case 19:{byte_bit = m_data_store.d_g_fwd_bridge_remote_emergency_stop.ha_bit;break;}
            case 20:{byte_bit = m_data_store.d_g_lube_oil_pressure_too_low_shutdown.ha_bit;break;}
            case 21:{byte_bit = m_data_store.d_g_coolant_temperature_too_high_shutdown.ha_bit;break;}
            case 22:{byte_bit = m_data_store.d_g_local_emergency_stop.ha_bit;break;}
            case 23:{byte_bit = m_data_store.d_g_remote_emergency_stop.ha_bit;break;}
            case 24:{byte_bit = m_data_store.d_g_rpu_emergency_stop.ha_bit;break;}
            case 25:{byte_bit = m_data_store.d_g_lube_oil_pressure_low_alarm.ha_bit;break;}
            case 26:{byte_bit = m_data_store.d_g_coolant_pressure_low_alarm.ha_bit;break;}
            case 27:{byte_bit = m_data_store.d_g_start_air_pressure_low_alarm.ha_bit;break;}
            case 28:{byte_bit = m_data_store.d_g_fuel_oil_pressure_low_alarm.ha_bit;break;}
            case 29:{byte_bit = m_data_store.d_g_coolant_temperature_high_alarm.ha_bit;break;}
            case 30:{byte_bit = m_data_store.d_g_lube_oil_temperature_high_alarm.ha_bit;break;}
            case 31:{byte_bit = m_data_store.d_g_left_exhaust_temperature_high_alarm.ha_bit;break;}
            case 32:{byte_bit = m_data_store.d_g_right_exhaust_temperature_high_alarm.ha_bit;break;}
            case 33:{byte_bit = m_data_store.d_g_overspeed_alarm.ha_bit;break;}
            case 34:{byte_bit = m_data_store.d_g_overspeed_shutdown.ha_bit;break;}
            case 35:{byte_bit = m_data_store.d_g_speed_sensor_fail.ha_bit;break;}
            case 36:{byte_bit = m_data_store.d_g_dcu_unit_battery_volt_dct_control_power_low.ha_bit;break;}
            case 37:{byte_bit = m_data_store.d_g_start_fail.ha_bit;break;}
            case 38:{byte_bit = m_data_store.d_g_rpu_unit_battery_a_volt_rpu_main_control_power_low.ha_bit;break;}
            case 39:{byte_bit = m_data_store.d_g_rpu_unit_battery_b_volt_rpu_main_control_power_low.ha_bit;break;}
            case 40:{byte_bit = m_data_store.d_g_common_alarm.ha_bit;break;}
            case 41:{byte_bit = m_data_store.d_g_common_warning.ha_bit;break;}
            case 42:{byte_bit = m_data_store.d_g_common_shutdown.ha_bit;break;}
            case 44:{byte_bit = m_data_store.d_g_common_faultsensor_fail.ha_bit;break;}
            case 45:{byte_bit = m_data_store.d_g_remote_control_status_auxaccess_lock.ha_bit;break;}
            case 46:{byte_bit = m_data_store.d_g_ready_to_start.ha_bit;break;}
            case 47:{byte_bit = m_data_store.d_g_ready_to_load.ha_bit;break;}
            case 48:{byte_bit = m_data_store.d_g_main_generator_running.ha_bit;break;}
            case 49:{byte_bit = m_data_store.d_g_main_generator_u_winding_temperature.ha_bit;break;}
            case 50:{byte_bit = m_data_store.d_g_main_generator_v_winding_temperature.ha_bit;break;}
            case 51:{byte_bit = m_data_store.d_g_main_generator_w_winding_temperature.ha_bit;break;}
            case 52:{byte_bit = m_data_store.d_g_main_generator_de_bearing_temperature.ha_bit;break;}
            case 53:{byte_bit = m_data_store.d_g_main_generator_nde_bearing_temperature.ha_bit;break;}
            case 54:{byte_bit = m_data_store.d_g_main_generator_excitation_current.ha_bit;break;}
            case 55:{byte_bit = m_data_store.d_g_main_generator_excitation_voltage.ha_bit;break;}
            case 56:{byte_bit = m_data_store.device_offline.ha_bit;break;}
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
