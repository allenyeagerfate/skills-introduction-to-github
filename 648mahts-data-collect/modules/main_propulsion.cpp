#include <iostream>
#include <unistd.h>

#include "sgo_sub_pub.h"
#include "sgo_func.h"
#include "sgo_ams/main_propulsion.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
//#include "main_propulsion.h"

using namespace std;

const char* main_propulsion_str[]={
#ifndef MAHTS648
	"main_propulsion_control_position",
	"main_propulsion_rudder_indication",
	"main_propulsion_vfd_rpm_indication",
	"main_propulsion_vfd_load_indication",
	"main_propulsion_motor_u_winding_temperature",
	"main_propulsion_motor_v_winding_temperature",
	"main_propulsion_motor_w_winding_temperature",
	"main_propulsion_motor_de_bearing_temperature",
	"main_propulsion_motor_nde_bearing_temperature",
	"main_propulsion_motor_air_inlet_temperature",
	"main_propulsion_motor_air_outlet_temperature",
	"main_propulsion_rudder_feedback_signal_missing",
	"main_propulsion_control_system_main_power_failure",
	"main_propulsion_control_system_backup_power_failure",
	"main_propulsion_steering_command_missing_joystick",
	"main_propulsion_speed_command_missing_joystick",
	"main_propulsion_fu_steering_control_failure",
	"main_propulsion_fu_speed_control_failure",
	"main_propulsion_vfd_speed_feedback_signal_missing",
	"main_propulsion_vfd_load_feedback_signal_missing",
	"main_propulsion_steering_command_missing_fwd_console",
	"main_propulsion_speed_command_missing_fwd_console",
	"main_propulsion_rudder_command_missing_aft_console",
	"main_propulsion_speed_command_missing_aft_console",
	"azimuth_thruster_common_alarm",
	"azimuth_thruster_fault",
	"main_propulsion_remote_control_system_communication_failure",
	"main_propulsion_vfd_fault",
	"main_propulsion_vfd_common_alarm",
	"main_propulsion_vfd_emergency_stop",
	"main_propulsion_vfd_slowdown_pre_alarm",
	"main_propulsion_vfd_power_limit",
	"azimuth_thruster_steering_vfd_fault",
	"main_propulsion_motor_water_leakage",
	"main_propulsion_motor_u_winding_temrpeature_high",
	"main_propulsion_motor_v_winding_temrpeature_high",
	"main_propulsion_motor_w_winding_temrpeature_high",
	"main_propulsion_motor_high_temrpeature_slowdown",
	"main_propulsion_motor_high_temrpeature_shutdown",
	"main_propulsion_motor_de_bearing_high_temperature_alarm",
	"main_propulsion_motor_de_bearing_high_temperature_slowdown",
	"main_propulsion_motor_de_bearing_high_temperature_shutdown",
	"main_propulsion_motor_nde_bearing_high_temperature_alarm",
	"main_propulsion_motor_nde_bearing_high_temperature_slowdown",
	"main_propulsion_motor_nde_bearing_high_temperature_shutdown",
	"main_propulsion_motor_air_inlet_high_temperature_alarm",
	"main_propulsion_motor_air_outlet_high_temperature_alarm",
	"main_propulsion_motor_high_temrpeature_slowdown_pre_alarm",
	"main_propulsion_motor_high_temrpeature_shutdown_pre_alarm",
	"main_propulsion_motor_de_bearing_high_temp_slowdown_pre_alarm",
	"main_propulsion_motor_de_bearing_high_tem_shutdown_pre_alarm",
	"main_propulsion_motor_nde_bearing_high_temp_slowdown_pre_alarm",
	"main_propulsion_motor_nde_bearing_high_tem_shutdown_pre_alarm",
	"main_propulsion_emergency_control_mode",
	"main_propulsion_shaft_lock_indication",
	"main_propulsion_vfd_ready",
	"main_propulsion_vfd_remote_control_position",
	"main_propulsion_vfd_running",
	"main_propulsion_vfd_override",
	"main_propulsion_vfd_torque_limitation",
	"main_propulsion_speed_control_nfu_mode",
	"main_propulsion_vfd_start_interlock",
	"main_propulsion_vfd_control_mode",
	"main_propulsion_steering_control_nfu_mode",
	"azimuth_thruster_vfd1_running",
	"azimuth_thruster_vfd2_running",
	"azimuth_thruster_ready",
	"main_propulsion_joystick_control_permission",
	"main_propulsion_ap_control_permission",
	"azimuth_thruster_main_power_failure",
	"azimuth_thruster_backup_power_failure",
	"azimuth_thruster_shaft_lock_inlet_air_pressure_low_alarm",
	"azimuth_thruster_lub_oil_level_low_alarm",
	"azimuth_thruster_lub_oil_temp_high_alarm",
	"azimuth_thruster_lub_oil_filter_blocked_alarm",
	"azimuth_thruster_lub_oil_press_low_alarm",
	"azimuth_thruster_sealing_oil_tank_level_low_alarm",
	"azimuth_thruster_vfd1_fault",
	"azimuth_thruster_vfd2_fault",
	"azimuth_thruster_fu_steering_fault",
	"azimuth_thruster_lub_motor1_overload_alarm",
	"azimuth_thruster_lub_motor2_overload_alarm",
	"azimuth_thruster_angle_order_lost_alarm",
	"azimuth_thruster_angle_feedback_lost_alarm",
	"azimuth_thruster_steering_power1_supply_fault",
	"azimuth_thruster_steering_power2_supply_fault",
	"azimuth_thruster_sealing_oil_inlet_air_press_low_alarm",
	"azimuth_thruster_sealing_oil_inlet_air_press_high_alarm",
	"azimuth_thruster_steering_motor1_temp_high_alarm",
	"azimuth_thruster_steering_motor2_temp_high_alarm",
	"main_propulsion_vfd_speed_reference",
	"main_propulsion_vfd_actual_speed",
	"main_propulsion_vfd_actual_torque",
	"main_propulsion_motor_current",
	"main_propulsion_vfd_dc_voltage",
	"main_propulsion_motor_power",
	"device_offline",
#else
	"main_propulsion_control_position",
	"main_propulsion_rudder_indication",
	"main_propulsion_vfd_rpm_indication",
	"main_propulsion_vfd_load_indication",
	"main_propulsion_motor_u_winding_temperature",
	"main_propulsion_motor_v_winding_temperature",
	"main_propulsion_motor_w_winding_temperature",
	"main_propulsion_motor_de_bearing_temperature",
	"main_propulsion_motor_nde_bearing_temperature",
	"main_propulsion_vfd_voltage",
	"main_propulsion_vfd_current",
	"propulsion_vfd_lcu_inlet_pressure",
	"propulsion_vfd_lcu_outlet_pressure",
	"propulsion_vfd_cooling_water_differential_pressure",
	"main_propulsion_rudder_feedback_signal_missing",
	"main_propulsion_control_system_main_power_failure",
	"main_propulsion_control_system_backup_power_failure",
	"main_propulsion_steering_command_missing_joystick",
	"main_propulsion_speed_command_missing_joystick",
	"main_propulsion_fu_steering_control_failure",
	"main_propulsion_fu_speed_control_failure",
	"spare1",
	"spare2",
	"main_propulsion_steering_command_missing_fwd_console",
	"main_propulsion_speed_command_missing_fwd_console",
	"main_propulsion_rudder_command_missing_aft_console",
	"main_propulsion_speed_command_missing_aft_console",
	"azimuth_thruster_common_alarm",
	"azimuth_thruster_fault",
	"main_propulsion_remote_control_system_communication_failure",
	"propulsion_vfd_isu_communication_lost",
	"propulsion_vfd_inu_communication_lost",
	"main_propulsion_vfd_lcu_communication_lost",
	"main_propulsion_vfd_fault",
	"main_propulsion_vfd_common_alarm",
	"main_propulsion_vfd_emergency_stop",
	"main_propulsion_vfd_emergency_stop_wire_broken",
	"spare3",
	"main_propulsion_vfd_control_power_failure_ups",
	"main_propulsion_vfd_power_limit",
	"main_propulsion_vfd_isu_fault",
	"main_propulsion_vfd_inu_fault",
	"main_propulsion_vfd_lcu_fault",
	"azimuth_thruster_steering_vfd_fault",
	"main_propulsion_motor_de_leaker",
	"main_propulsion_motor_nde_leaker",
	"main_propulsion_vfd_inu_waring",
	"main_propulsion_vfd_lcu_waring",
	"main_propulsion_motor_u_winding_temrpeature_high",
	"main_propulsion_motor_v_winding_temrpeature_high",
	"main_propulsion_motor_w_winding_temrpeature_high",
	"main_propulsion_motor_high_temrpeature_slowdown",
	"main_propulsion_motor_high_temrpeature_shutdown",
	"main_propulsion_motor_de_bearing_high_temperature_alarm",
	"main_propulsion_motor_de_bearing_high_temperature_slowdown",
	"main_propulsion_motor_de_bearing_high_temperature_shutdown",
	"main_propulsion_motor_nde_bearing_high_temperature_alarm",
	"main_propulsion_motor_nde_bearing_high_temperature_slowdown",
	"main_propulsion_motor_nde_bearing_high_temperature_shutdown",
	"main_propulsion_vfd_cooling_water_inlet_pressure_low_alarm",
	"main_propulsion_vfd_cooling_water_inlet_pressure_low_low_alarm",
	"main_propulsion_vfd_cooling_water_inlet_pressure_low_low_fault",
	"main_propulsion_vfd_lcu_outlet_pressure_high_alarm",
	"main_propulsion_vfd_lcu_diff_pressure_low_alarm",
	"main_propulsion_vfd_lcu_diff_pressure_low_low_fault",
	"main_propulsion_vfd_control_mode",
	"main_propulsion_steering_control_nfu_mode",
	"main_propulsion_shaft_lock_status",
	"main_propulsion_vfd_ready",
	"main_propulsion_vfd_remote_control_position",
	"main_propulsion_vfd_running",
	"main_propulsion_vfd_override",
	"main_propulsion_vfd_heater_on",
	"main_propulsion_vfd_power_on",
	"main_propulsion_motor_heater_on",
	"main_propulsion_vfd_fu_nfu_mode",
	"main_propulsion_start_interlock",
	"azimuth_thruster_fu_nfu_mode",
	"spare4",
	"azimuth_thruster_main_power_failure",
	"azimuth_thruster_backup_power_failure",
	"azimuth_thruster_shaft_lock_inlet_air_pressure_low_alarm",
	"azimuth_thruster_lub_oil_level_low_alarm",
	"azimuth_thruster_lub_oil_temp_high_alarm",
	"azimuth_thruster_sealing_oil_tank_level_low",
	"azimuth_thruster_sealing_oil_inlet_air_press_low",
	"azimuth_thruster_sealing_oil_inlet_air_press_high",
	"azimuth_thruster_angle_order_lost_alarm",
	"azimuth_thruster_angle_feedback_lost_alarm",
	"azimuth_thruster_steering_power_supply_fault",
	"azimuth_thruster_steering_vfd_fault2",
	"azimuth_thruster_angle_feedback",
	"device_offline"

#endif
};

	
static const char* productCode = "main_propulsion";


MAIN_PROPULSION::MAIN_PROPULSION()
{}

MAIN_PROPULSION::~MAIN_PROPULSION()
{}

void MAIN_PROPULSION::init(int devId,BlockQueue<main_propulsion> &bq,modbus *mb,mutex *mux)
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
	
	setSubTopics();
	setPubTopics();
	
	start(bq);

}



// 连接丢失回调函数
void MAIN_PROPULSION::connectionLostCallback(void* context, char* cause) 
{
	// 处理连接丢失事件
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	MAIN_PROPULSION* client = static_cast<MAIN_PROPULSION*>(ctx->manager);
	
	printf("--%s %d--address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());
	sgo_log_stderr(0,"--%s %d----address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());

	//std::lock_guard<std::mutex> lock(m_losts_mutex);
	client->m_v_lost[ctx->index] = 1;
	client->m_v_property_index_array.clear();

}


// 消息到达回调函数
int MAIN_PROPULSION::messageArrivedCallback(void* context, char* topicName, int topicLen,MQTTClient_message* message) 
{
	// 处理接收到的消息
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	MAIN_PROPULSION* client = static_cast<MAIN_PROPULSION*>(ctx->manager);
	
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
			client->m_property[main_propulsion_str[i]]=i;
		
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
void MAIN_PROPULSION::deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt) 
{
	// 处理交付完成事件
	//std::cout << "Message delivery complete." << std::endl;
}

void MAIN_PROPULSION::subClient()
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


void MAIN_PROPULSION::pubClient(BlockQueue<main_propulsion>& bq)
{
	while(1)
	{
		MQTTClient_message pubmsg = MQTTClient_message_initializer;
		//声明消息token
		MQTTClient_deliveryToken token;

		while(!m_pubStop){
			main_propulsion one = bq.take();
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



void MAIN_PROPULSION::start(BlockQueue<main_propulsion> &bq)
{
	m_sub = new thread(&MAIN_PROPULSION::subClient,this);
	m_sub->detach();

	m_pub = new thread(&MAIN_PROPULSION::pubClient,this,ref(bq));
	m_pub->detach();

	m_thr_reconn = new thread(&MAIN_PROPULSION::reconn,this);
	m_thr_reconn->detach();

	m_thr_resend = new thread(&MAIN_PROPULSION::resnd,this);
	m_thr_resend->detach();

	m_thr_chksensor = new thread(&MAIN_PROPULSION::chksensor,this);
	m_thr_chksensor->detach();
	
//	thread *thr_range = new thread(&MAIN_PROPULSION::thSetRange,this);
//	thr_range->detach();

	thread *thr_threshold = new thread(&MAIN_PROPULSION::thSetThreshold,this);
	thr_threshold->detach();

	thread *thr_inhibit = new thread(&MAIN_PROPULSION::thSetInhibit,this);
	thr_inhibit->detach();

	thread *thr_delay = new thread(&MAIN_PROPULSION::thSetDelay,this);
	thr_delay->detach();	
}

bool MAIN_PROPULSION::UpdateLa(std::string key, uint16_t *data, int size)
{
	bool ret = false;
	for(int i = 0; i < sizeof(main_propulsion_str); ++i)
	{
        std::string s(main_propulsion_str[i]);
		if(s == key)
		{
			std::pair<uint8_t, uint8_t> byte_bit{0xff, 0xff};
            switch(i)
			{
                case 0:{byte_bit = m_data_store.main_propulsion_control_position.la_bit;break;}
                case 1:{byte_bit = m_data_store.main_propulsion_rudder_indication.la_bit;break;}
                case 2:{byte_bit = m_data_store.main_propulsion_vfd_rpm_indication.la_bit;break;}
                case 3:{byte_bit = m_data_store.main_propulsion_vfd_load_indication.la_bit;break;}
                case 4:{byte_bit = m_data_store.main_propulsion_motor_u_winding_temperature.la_bit;break;}
                case 5:{byte_bit = m_data_store.main_propulsion_motor_v_winding_temperature.la_bit;break;}
                case 6:{byte_bit = m_data_store.main_propulsion_motor_w_winding_temperature.la_bit;break;}
                case 7:{byte_bit = m_data_store.main_propulsion_motor_de_bearing_temperature.la_bit;break;}
                case 8:{byte_bit = m_data_store.main_propulsion_motor_nde_bearing_temperature.la_bit;break;}
                case 9:{byte_bit = m_data_store.main_propulsion_vfd_voltage.la_bit;break;}
                case 10:{byte_bit = m_data_store.main_propulsion_vfd_current.la_bit;break;}
                case 11:{byte_bit = m_data_store.main_propulsion_lcu_inlet_pressure.la_bit;break;}
                case 12:{byte_bit = m_data_store.main_propulsion_lcu_outlet_pressure.la_bit;break;}
                case 13:{byte_bit = m_data_store.main_propulsion_vfd_cooling_water_differential_pressure.la_bit;break;}
                case 14:{byte_bit = m_data_store.main_propulsion_rudder_feedback_signal_missing.la_bit;break;}
                case 15:{byte_bit = m_data_store.main_propulsion_control_system_main_power_failure.la_bit;break;}
                case 16:{byte_bit = m_data_store.main_propulsion_control_system_backup_power_failure.la_bit;break;}
                case 17:{byte_bit = m_data_store.main_propulsion_steering_command_missing_joystick.la_bit;break;}
                case 18:{byte_bit = m_data_store.main_propulsion_speed_command_missing_joystick.la_bit;break;}
                case 19:{byte_bit = m_data_store.main_propulsion_fu_steering_control_failure.la_bit;break;}
                case 20:{byte_bit = m_data_store.main_propulsion_fu_speed_control_failure.la_bit;break;}
                case 23:{byte_bit = m_data_store.main_propulsion_steering_command_missing_fwd_console.la_bit;break;}
                case 24:{byte_bit = m_data_store.main_propulsion_speed_command_missing_fwd_console.la_bit;break;}
                case 25:{byte_bit = m_data_store.main_propulsion_rudder_command_missing_aft_console.la_bit;break;}
                case 26:{byte_bit = m_data_store.main_propulsion_speed_command_missing_aft_console.la_bit;break;}
                case 27:{byte_bit = m_data_store.azimuth_thruster_common_alarm.la_bit;break;}
                case 28:{byte_bit = m_data_store.azimuth_thruster_fault.la_bit;break;}
                case 29:{byte_bit = m_data_store.main_propulsion_remote_control_system_communication_failure.la_bit;break;}
                case 30:{byte_bit = m_data_store.propulsion_vfd_isu_communication_lost.la_bit;break;}
                case 31:{byte_bit = m_data_store.propulsion_vfd_inu_communication_lost.la_bit;break;}
                case 32:{byte_bit = m_data_store.propulsion_vfd_lcu_communication_lost.la_bit;break;}
                case 33:{byte_bit = m_data_store.main_propulsion_vfd_fault.la_bit;break;}
                case 34:{byte_bit = m_data_store.main_propulsion_vfd_common_alarm.la_bit;break;}
                case 35:{byte_bit = m_data_store.main_propulsion_vfd_emergency_stop.la_bit;break;}
                case 36:{byte_bit = m_data_store.main_propulsion_vfd_emergency_stop_wire_broken.la_bit;break;}
                case 38:{byte_bit = m_data_store.main_propulsion_vfd_control_power_failure_ups.la_bit;break;}
                case 39:{byte_bit = m_data_store.main_propulsion_vfd_power_limit.la_bit;break;}
                case 40:{byte_bit = m_data_store.main_propulsion_vfd_isu_fault.la_bit;break;}
                case 41:{byte_bit = m_data_store.main_propulsion_vfd_inu_fault.la_bit;break;}
                case 42:{byte_bit = m_data_store.main_propulsion_vfd_lcu_fault.la_bit;break;}
                case 43:{byte_bit = m_data_store.azimuth_thruster_steering_vfd_fault.la_bit;break;}
                case 44:{byte_bit = m_data_store.main_propulsion_motor_de_leakage.la_bit;break;}
                case 45:{byte_bit = m_data_store.main_propulsion_motor_nde_leakage.la_bit;break;}
                case 46:{byte_bit = m_data_store.main_propulsion_vfd_inu_waring.la_bit;break;}
                case 47:{byte_bit = m_data_store.main_propulsion_vfd_lcu_waring.la_bit;break;}
                case 48:{byte_bit = m_data_store.main_propulsion_motor_u_winding_temrpeature_high.la_bit;break;}
                case 49:{byte_bit = m_data_store.main_propulsion_motor_v_winding_temrpeature_high.la_bit;break;}
                case 50:{byte_bit = m_data_store.main_propulsion_motor_w_winding_temrpeature_high.la_bit;break;}
                case 51:{byte_bit = m_data_store.main_propulsion_motor_high_temrpeature_slowdown.la_bit;break;}
                case 52:{byte_bit = m_data_store.main_propulsion_motor_high_temrpeature_shutdown.la_bit;break;}
                case 53:{byte_bit = m_data_store.main_propulsion_motor_de_bearing_high_temperature_alarm.la_bit;break;}
                case 54:{byte_bit = m_data_store.main_propulsion_motor_de_bearing_high_temperature_slowdown.la_bit;break;}
                case 55:{byte_bit = m_data_store.main_propulsion_motor_de_bearing_high_temperature_shutdown.la_bit;break;}
                case 56:{byte_bit = m_data_store.main_propulsion_motor_nde_bearing_high_temperature_alarm.la_bit;break;}
                case 57:{byte_bit = m_data_store.main_propulsion_motor_nde_bearing_high_temperature_slowdown.la_bit;break;}
                case 58:{byte_bit = m_data_store.main_propulsion_motor_nde_bearing_high_temperature_shutdown.la_bit;break;}
                case 59:{byte_bit = m_data_store.main_propulsion_vfd_cooling_water_inlet_pressure_low_alarm.la_bit;break;}
                case 60:{byte_bit = m_data_store.main_propulsion_vfd_cooling_water_inlet_pressure_low_low_alarm.la_bit;break;}
                case 61:{byte_bit = m_data_store.main_propulsion_vfd_cooling_water_inlet_pressure_low_low_fault.la_bit;break;}
                case 62:{byte_bit = m_data_store.main_propulsion_vfd_lcu_outlet_pressure_high_alarm.la_bit;break;}
                case 63:{byte_bit = m_data_store.main_propulsion_vfd_lcu_diff_pressure_low_alarm.la_bit;break;}
                case 64:{byte_bit = m_data_store.main_propulsion_vfd_lcu_diff_pressure_low_low_fault.la_bit;break;}
                case 65:{byte_bit = m_data_store.main_propulsion_vfd_control_mode.la_bit;break;}
                case 66:{byte_bit = m_data_store.main_propulsion_steering_control_nfu_mode.la_bit;break;}
                case 67:{byte_bit = m_data_store.main_propulsion_shaft_lock_status.la_bit;break;}
                case 68:{byte_bit = m_data_store.main_propulsion_vfd_ready.la_bit;break;}
                case 69:{byte_bit = m_data_store.main_propulsion_vfd_remote_control_position.la_bit;break;}
                case 70:{byte_bit = m_data_store.main_propulsion_vfd_running.la_bit;break;}
                case 71:{byte_bit = m_data_store.main_propulsion_vfd_override.la_bit;break;}
                case 72:{byte_bit = m_data_store.main_propulsion_vfd_heater_on.la_bit;break;}
                case 73:{byte_bit = m_data_store.main_propulsion_vfd_power_on.la_bit;break;}
                case 74:{byte_bit = m_data_store.main_propulsion_motor_heater_on.la_bit;break;}
                case 75:{byte_bit = m_data_store.main_propulsion_vfd_fu_nfu_mode.la_bit;break;}
                case 76:{byte_bit = m_data_store.main_propulsion_vfd_start_interlock.la_bit;break;}
                case 77:{byte_bit = m_data_store.azimuth_thruster_fu_nfu_mode.la_bit;break;}
                case 79:{byte_bit = m_data_store.azimuth_thruster_main_power_failure.la_bit;break;}
                case 80:{byte_bit = m_data_store.azimuth_thruster_backup_power_failure.la_bit;break;}
                case 81:{byte_bit = m_data_store.azimuth_thruster_shaft_lock_inlet_air_pressure_low_alarm.la_bit;break;}
                case 82:{byte_bit = m_data_store.azimuth_thruster_lub_oil_level_low_alarm.la_bit;break;}
                case 83:{byte_bit = m_data_store.azimuth_thruster_lub_oil_temp_high_alarm.la_bit;break;}
                case 84:{byte_bit = m_data_store.azimuth_thruster_sealing_oil_tank_level_low_alarm.la_bit;break;}
                case 85:{byte_bit = m_data_store.azimuth_thruster_sealing_oil_inlet_air_press_low_alarm.la_bit;break;}
                case 86:{byte_bit = m_data_store.azimuth_thruster_sealing_oil_inlet_air_press_high_alarm.la_bit;break;}
                case 87:{byte_bit = m_data_store.azimuth_thruster_angle_order_lost_alarm.la_bit;break;}
                case 88:{byte_bit = m_data_store.azimuth_thruster_angle_feedback_lost_alarm.la_bit;break;}
                case 89:{byte_bit = m_data_store.azimuth_thruster_steering_power_supply_fault.la_bit;break;}
                case 90:{byte_bit = m_data_store.azimuth_thruster_steering_vfd_fault_2.la_bit;break;}
                case 91:{byte_bit = m_data_store.azimuth_thruster_angle_feedback.la_bit;break;}
                case 92:{byte_bit = m_data_store.device_offline.la_bit;break;}
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


void MAIN_PROPULSION::chksensor()
{
	checkSensorStatus();
}

void MAIN_PROPULSION::resnd()
{
	resend();
}

void MAIN_PROPULSION::reconn()
{
	reconnect();
}



string MAIN_PROPULSION::property_reply(char flag,const main_propulsion& one,const std::string& msg_id)
{
    rapidjson::StringBuffer strBuf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(strBuf);

    writer.StartObject();

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
			case 0:writer.Int(one.main_propulsion_control_position);break;
			case 1:writer.Double(one.main_propulsion_rudder_indication);break;
			case 2:writer.Double(one.main_propulsion_vfd_rpm_indication);break;
			case 3:writer.Double(one.main_propulsion_vfd_load_indication);break;
			case 4:writer.Double(one.main_propulsion_motor_u_winding_temperature);break;
			case 5:writer.Double(one.main_propulsion_motor_v_winding_temperature);break;
			case 6:writer.Double(one.main_propulsion_motor_w_winding_temperature);break;
			case 7:writer.Double(one.main_propulsion_motor_de_bearing_temperature);break;
			case 8:writer.Double(one.main_propulsion_motor_nde_bearing_temperature);break;
			case 9:writer.Double(one.main_propulsion_motor_air_inlet_temperature);break;
			case 10:writer.Double(one.main_propulsion_motor_air_outlet_temperature);break;
			case 11:writer.Int(one.main_propulsion_rudder_feedback_signal_missing);break;
			case 12:writer.Int(one.main_propulsion_control_system_main_power_failure);break;
			case 13:writer.Int(one.main_propulsion_control_system_backup_power_failure);break;
			case 14:writer.Int(one.main_propulsion_steering_command_missing_joystick);break;
			case 15:writer.Int(one.main_propulsion_speed_command_missing_joystick);break;
			case 16:writer.Int(one.main_propulsion_fu_steering_control_failure);break;
			case 17:writer.Int(one.main_propulsion_fu_speed_control_failure);break;
			case 18:writer.Int(one.main_propulsion_vfd_speed_feedback_signal_missing);break;
			case 19:writer.Int(one.main_propulsion_vfd_load_feedback_signal_missing);break;
			case 20:writer.Int(one.main_propulsion_steering_command_missing_fwd_console);break;
			case 21:writer.Int(one.main_propulsion_speed_command_missing_fwd_console);break;
			case 22:writer.Int(one.main_propulsion_rudder_command_missing_aft_console);break;
			case 23:writer.Int(one.main_propulsion_speed_command_missing_aft_console);break;
			case 24:writer.Int(one.azimuth_thruster_common_alarm);break;
			case 25:writer.Int(one.azimuth_thruster_fault);break;
			case 26:writer.Int(one.main_propulsion_remote_control_system_communication_failure);break;
			case 27:writer.Int(one.main_propulsion_vfd_fault);break;
			case 28:writer.Int(one.main_propulsion_vfd_common_alarm);break;
			case 29:writer.Int(one.main_propulsion_vfd_emergency_stop);break;
			case 30:writer.Int(one.main_propulsion_vfd_slowdown_pre_alarm);break;
			case 31:writer.Int(one.main_propulsion_vfd_power_limit);break;
			case 32:writer.Int(one.azimuth_thruster_steering_vfd_fault);break;
			case 33:writer.Int(one.main_propulsion_motor_water_leakage);break;
			case 34:writer.Int(one.main_propulsion_motor_u_winding_temrpeature_high);break;
			case 35:writer.Int(one.main_propulsion_motor_v_winding_temrpeature_high);break;
			case 36:writer.Int(one.main_propulsion_motor_w_winding_temrpeature_high);break;
			case 37:writer.Int(one.main_propulsion_motor_high_temrpeature_slowdown);break;
			case 38:writer.Int(one.main_propulsion_motor_high_temrpeature_shutdown);break;
			case 39:writer.Int(one.main_propulsion_motor_de_bearing_high_temperature_alarm);break;
			case 40:writer.Int(one.main_propulsion_motor_de_bearing_high_temperature_slowdown);break;
			case 41:writer.Int(one.main_propulsion_motor_de_bearing_high_temperature_shutdown);break;
			case 42:writer.Int(one.main_propulsion_motor_nde_bearing_high_temperature_alarm);break;
			case 43:writer.Int(one.main_propulsion_motor_nde_bearing_high_temperature_slowdown);break;
			case 44:writer.Int(one.main_propulsion_motor_nde_bearing_high_temperature_shutdown);break;
			case 45:writer.Int(one.main_propulsion_motor_air_inlet_high_temperature_alarm);break;
			case 46:writer.Int(one.main_propulsion_motor_air_outlet_high_temperature_alarm);break;
			case 47:writer.Int(one.main_propulsion_motor_high_temrpeature_slowdown_pre_alarm);break;
			case 48:writer.Int(one.main_propulsion_motor_high_temrpeature_shutdown_pre_alarm);break;
			case 49:writer.Int(one.main_propulsion_motor_de_bearing_high_temp_slowdown_pre_alarm);break;
			case 50:writer.Int(one.main_propulsion_motor_de_bearing_high_tem_shutdown_pre_alarm);break;
			case 51:writer.Int(one.main_propulsion_motor_nde_bearing_high_temp_slowdown_pre_alarm);break;
			case 52:writer.Int(one.main_propulsion_motor_nde_bearing_high_tem_shutdown_pre_alarm);break;
			case 53:writer.Int(one.main_propulsion_emergency_control_mode);break;
			case 54:writer.Int(one.main_propulsion_shaft_lock_indication);break;
			case 55:writer.Int(one.main_propulsion_vfd_ready);break;
			case 56:writer.Int(one.main_propulsion_vfd_remote_control_position);break;
			case 57:writer.Int(one.main_propulsion_vfd_running);break;
			case 58:writer.Int(one.main_propulsion_vfd_override);break;
			case 59:writer.Int(one.main_propulsion_vfd_torque_limitation);break;
			case 60:writer.Int(one.main_propulsion_speed_control_nfu_mode);break;
			case 61:writer.Int(one.main_propulsion_vfd_start_interlock);break;
			case 62:writer.Int(one.main_propulsion_vfd_control_mode);break;
			case 63:writer.Int(one.main_propulsion_steering_control_nfu_mode);break;
			case 64:writer.Int(one.azimuth_thruster_vfd1_running);break;
			case 65:writer.Int(one.azimuth_thruster_vfd2_running);break;
			case 66:writer.Int(one.azimuth_thruster_ready);break;
			case 67:writer.Int(one.main_propulsion_joystick_control_permission);break;
			case 68:writer.Int(one.main_propulsion_ap_control_permission);break;
			case 69:writer.Int(one.azimuth_thruster_main_power_failure);break;
			case 70:writer.Int(one.azimuth_thruster_backup_power_failure);break;
			case 71:writer.Int(one.azimuth_thruster_shaft_lock_inlet_air_pressure_low_alarm);break;
			case 72:writer.Int(one.azimuth_thruster_lub_oil_level_low_alarm);break;
			case 73:writer.Int(one.azimuth_thruster_lub_oil_temp_high_alarm);break;
			case 74:writer.Int(one.azimuth_thruster_lub_oil_filter_blocked_alarm);break;
			case 75:writer.Int(one.azimuth_thruster_lub_oil_press_low_alarm);break;
			case 76:writer.Int(one.azimuth_thruster_sealing_oil_tank_level_low_alarm);break;
			case 77:writer.Int(one.azimuth_thruster_vfd1_fault);break;
			case 78:writer.Int(one.azimuth_thruster_vfd2_fault);break;
			case 79:writer.Int(one.azimuth_thruster_fu_steering_fault);break;
			case 80:writer.Int(one.azimuth_thruster_lub_motor1_overload_alarm);break;
			case 81:writer.Int(one.azimuth_thruster_lub_motor2_overload_alarm);break;
			case 82:writer.Int(one.azimuth_thruster_angle_order_lost_alarm);break;
			case 83:writer.Int(one.azimuth_thruster_angle_feedback_lost_alarm);break;
			case 84:writer.Int(one.azimuth_thruster_steering_power1_supply_fault);break;
			case 85:writer.Int(one.azimuth_thruster_steering_power2_supply_fault);break;
			case 86:writer.Int(one.azimuth_thruster_sealing_oil_inlet_air_press_low_alarm);break;
			case 87:writer.Int(one.azimuth_thruster_sealing_oil_inlet_air_press_high_alarm);break;
			case 88:writer.Int(one.azimuth_thruster_steering_motor1_temp_high_alarm);break;
			case 89:writer.Int(one.azimuth_thruster_steering_motor2_temp_high_alarm);break;
			case 90:writer.Double(one.main_propulsion_vfd_speed_reference);break;
			case 91:writer.Double(one.main_propulsion_vfd_actual_speed);break;
			case 92:writer.Double(one.main_propulsion_vfd_actual_torque);break;
			case 93:writer.Double(one.main_propulsion_motor_current);break;
			case 94:writer.Double(one.main_propulsion_vfd_dc_voltage);break;
			case 95:writer.Double(one.main_propulsion_motor_power);break;
			case 96:writer.Double(one.device_offline);break;
#else
           case 0:writer.Double(one.main_propulsion_control_position);break;
           case 1:writer.Double(one.main_propulsion_rudder_indication);break;
           case 2:writer.Double(one.main_propulsion_vfd_rpm_indication);break;
           case 3:writer.Double(one.main_propulsion_vfd_load_indication);break;
           case 4:writer.Double(one.main_propulsion_motor_u_winding_temperature);break;
           case 5:writer.Double(one.main_propulsion_motor_v_winding_temperature);break;
           case 6:writer.Double(one.main_propulsion_motor_w_winding_temperature);break;
           case 7:writer.Double(one.main_propulsion_motor_de_bearing_temperature);break;
           case 8:writer.Double(one.main_propulsion_motor_nde_bearing_temperature);break;
           case 9:writer.Double(one.main_propulsion_vfd_voltage);break;
           case 10:writer.Double(one.main_propulsion_vfd_current);break;
           case 11:writer.Double(one.main_propulsion_lcu_inlet_pressure);break;
           case 12:writer.Double(one.main_propulsion_lcu_outlet_pressure);break;
           case 13:writer.Double(one.main_propulsion_vfd_cooling_water_differential_pressure);break;
           case 14:writer.Int(one.main_propulsion_rudder_feedback_signal_missing);break;
           case 15:writer.Int(one.main_propulsion_control_system_main_power_failure);break;
           case 16:writer.Int(one.main_propulsion_control_system_backup_power_failure);break;
           case 17:writer.Int(one.main_propulsion_steering_command_missing_joystick);break;
           case 18:writer.Int(one.main_propulsion_speed_command_missing_joystick);break;
           case 19:writer.Int(one.main_propulsion_fu_steering_control_failure);break;
           case 20:writer.Int(one.main_propulsion_fu_speed_control_failure);break;
           case 21:writer.Int(0);break;
           case 22:writer.Int(0);break;
           case 23:writer.Int(one.main_propulsion_steering_command_missing_fwd_console);break;
           case 24:writer.Int(one.main_propulsion_speed_command_missing_fwd_console);break;
           case 25:writer.Int(one.main_propulsion_rudder_command_missing_aft_console);break;
           case 26:writer.Int(one.main_propulsion_speed_command_missing_aft_console);break;
           case 27:writer.Int(one.azimuth_thruster_common_alarm);break;
           case 28:writer.Int(one.azimuth_thruster_fault);break;
           case 29:writer.Int(one.main_propulsion_remote_control_system_communication_failure);break;
           case 30:writer.Int(one.propulsion_vfd_isu_communication_lost);break;
           case 31:writer.Int(one.propulsion_vfd_inu_communication_lost);break;
           case 32:writer.Int(one.propulsion_vfd_lcu_communication_lost);break;
           case 33:writer.Int(one.main_propulsion_vfd_fault);break;
           case 34:writer.Int(one.main_propulsion_vfd_common_alarm);break;
           case 35:writer.Int(one.main_propulsion_vfd_emergency_stop);break;
           case 36:writer.Int(one.main_propulsion_vfd_emergency_stop_wire_broken);break;
           case 37:writer.Int(0);break;
           case 38:writer.Int(one.main_propulsion_vfd_control_power_failure_ups);break;
           case 39:writer.Int(one.main_propulsion_vfd_power_limit);break;
           case 40:writer.Int(one.main_propulsion_vfd_isu_fault);break;
           case 41:writer.Int(one.main_propulsion_vfd_inu_fault);break;
           case 42:writer.Int(one.main_propulsion_vfd_lcu_fault);break;
           case 43:writer.Int(one.azimuth_thruster_steering_vfd_fault);break;
           case 44:writer.Int(one.main_propulsion_motor_de_leakage);break;
           case 45:writer.Int(one.main_propulsion_motor_nde_leakage);break;
           case 46:writer.Int(one.main_propulsion_vfd_inu_waring);break;
           case 47:writer.Int(one.main_propulsion_vfd_lcu_waring);break;
           case 48:writer.Int(one.main_propulsion_motor_u_winding_temrpeature_high);break;
           case 49:writer.Int(one.main_propulsion_motor_v_winding_temrpeature_high);break;
           case 50:writer.Int(one.main_propulsion_motor_w_winding_temrpeature_high);break;
           case 51:writer.Int(one.main_propulsion_motor_high_temrpeature_slowdown);break;
           case 52:writer.Int(one.main_propulsion_motor_high_temrpeature_shutdown);break;
           case 53:writer.Int(one.main_propulsion_motor_de_bearing_high_temperature_alarm);break;
           case 54:writer.Int(one.main_propulsion_motor_de_bearing_high_temperature_slowdown);break;
           case 55:writer.Int(one.main_propulsion_motor_de_bearing_high_temperature_shutdown);break;
           case 56:writer.Int(one.main_propulsion_motor_nde_bearing_high_temperature_alarm);break;
           case 57:writer.Int(one.main_propulsion_motor_nde_bearing_high_temperature_slowdown);break;
           case 58:writer.Int(one.main_propulsion_motor_nde_bearing_high_temperature_shutdown);break;
           case 59:writer.Int(one.main_propulsion_vfd_cooling_water_inlet_pressure_low_alarm);break;
           case 60:writer.Int(one.main_propulsion_vfd_cooling_water_inlet_pressure_low_low_alarm);break;
           case 61:writer.Int(one.main_propulsion_vfd_cooling_water_inlet_pressure_low_low_fault);break;
           case 62:writer.Int(one.main_propulsion_vfd_lcu_outlet_pressure_high_alarm);break;
           case 63:writer.Int(one.main_propulsion_vfd_lcu_diff_pressure_low_alarm);break;
           case 64:writer.Int(one.main_propulsion_vfd_lcu_diff_pressure_low_low_fault);break;
           case 65:writer.Int(one.main_propulsion_vfd_control_mode);break;
           case 66:writer.Int(one.main_propulsion_steering_control_nfu_mode);break;
           case 67:writer.Int(one.main_propulsion_shaft_lock_status);break;
           case 68:writer.Int(one.main_propulsion_vfd_ready);break;
           case 69:writer.Int(one.main_propulsion_vfd_remote_control_position);break;
           case 70:writer.Int(one.main_propulsion_vfd_running);break;
           case 71:writer.Int(one.main_propulsion_vfd_override);break;
           case 72:writer.Int(one.main_propulsion_vfd_heater_on);break;
           case 73:writer.Int(one.main_propulsion_vfd_power_on);break;
           case 74:writer.Int(one.main_propulsion_motor_heater_on);break;
           case 75:writer.Int(one.main_propulsion_vfd_fu_nfu_mode);break;
           case 76:writer.Int(one.main_propulsion_vfd_start_interlock);break;
           case 77:writer.Int(one.azimuth_thruster_fu_nfu_mode);break;
           case 78:writer.Int(0);break;
           case 79:writer.Int(one.azimuth_thruster_main_power_failure);break;
           case 80:writer.Int(one.azimuth_thruster_backup_power_failure);break;
           case 81:writer.Int(one.azimuth_thruster_shaft_lock_inlet_air_pressure_low_alarm);break;
           case 82:writer.Int(one.azimuth_thruster_lub_oil_level_low_alarm);break;
           case 83:writer.Int(one.azimuth_thruster_lub_oil_temp_high_alarm);break;
           case 84:writer.Int(one.azimuth_thruster_sealing_oil_tank_level_low_alarm);break;
           case 85:writer.Int(one.azimuth_thruster_sealing_oil_inlet_air_press_low_alarm);break;
           case 86:writer.Int(one.azimuth_thruster_sealing_oil_inlet_air_press_high_alarm);break;
           case 87:writer.Int(one.azimuth_thruster_angle_order_lost_alarm);break;
           case 88:writer.Int(one.azimuth_thruster_angle_feedback_lost_alarm);break;
           case 89:writer.Int(one.azimuth_thruster_steering_power_supply_fault);break;
           case 90:writer.Int(one.azimuth_thruster_steering_vfd_fault_2);break;
           case 91:writer.Double(one.azimuth_thruster_angle_feedback);break;
           case 92:writer.Int(one.device_offline);break;

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
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_control_position);
	writer.Key(main_propulsion_str[i++]);writer.Double(one.main_propulsion_rudder_indication);
	writer.Key(main_propulsion_str[i++]);writer.Double(one.main_propulsion_vfd_rpm_indication);
	writer.Key(main_propulsion_str[i++]);writer.Double(one.main_propulsion_vfd_load_indication);
	writer.Key(main_propulsion_str[i++]);writer.Double(one.main_propulsion_motor_u_winding_temperature);
	writer.Key(main_propulsion_str[i++]);writer.Double(one.main_propulsion_motor_v_winding_temperature);
	writer.Key(main_propulsion_str[i++]);writer.Double(one.main_propulsion_motor_w_winding_temperature);
	writer.Key(main_propulsion_str[i++]);writer.Double(one.main_propulsion_motor_de_bearing_temperature);
	writer.Key(main_propulsion_str[i++]);writer.Double(one.main_propulsion_motor_nde_bearing_temperature);
	writer.Key(main_propulsion_str[i++]);writer.Double(one.main_propulsion_motor_air_inlet_temperature);
	writer.Key(main_propulsion_str[i++]);writer.Double(one.main_propulsion_motor_air_outlet_temperature);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_rudder_feedback_signal_missing);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_control_system_main_power_failure);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_control_system_backup_power_failure);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_steering_command_missing_joystick);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_speed_command_missing_joystick);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_fu_steering_control_failure);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_fu_speed_control_failure);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_vfd_speed_feedback_signal_missing);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_vfd_load_feedback_signal_missing);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_steering_command_missing_fwd_console);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_speed_command_missing_fwd_console);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_rudder_command_missing_aft_console);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_speed_command_missing_aft_console);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.azimuth_thruster_common_alarm);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.azimuth_thruster_fault);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_remote_control_system_communication_failure);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_vfd_fault);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_vfd_common_alarm);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_vfd_emergency_stop);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_vfd_slowdown_pre_alarm);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_vfd_power_limit);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.azimuth_thruster_steering_vfd_fault);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_motor_water_leakage);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_motor_u_winding_temrpeature_high);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_motor_v_winding_temrpeature_high);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_motor_w_winding_temrpeature_high);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_motor_high_temrpeature_slowdown);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_motor_high_temrpeature_shutdown);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_motor_de_bearing_high_temperature_alarm);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_motor_de_bearing_high_temperature_slowdown);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_motor_de_bearing_high_temperature_shutdown);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_motor_nde_bearing_high_temperature_alarm);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_motor_nde_bearing_high_temperature_slowdown);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_motor_nde_bearing_high_temperature_shutdown);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_motor_air_inlet_high_temperature_alarm);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_motor_air_outlet_high_temperature_alarm);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_motor_high_temrpeature_slowdown_pre_alarm);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_motor_high_temrpeature_shutdown_pre_alarm);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_motor_de_bearing_high_temp_slowdown_pre_alarm);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_motor_de_bearing_high_tem_shutdown_pre_alarm);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_motor_nde_bearing_high_temp_slowdown_pre_alarm);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_motor_nde_bearing_high_tem_shutdown_pre_alarm);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_emergency_control_mode);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_shaft_lock_indication);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_vfd_ready);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_vfd_remote_control_position);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_vfd_running);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_vfd_override);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_vfd_torque_limitation);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_speed_control_nfu_mode);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_vfd_start_interlock);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_vfd_control_mode);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_steering_control_nfu_mode);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.azimuth_thruster_vfd1_running);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.azimuth_thruster_vfd2_running);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.azimuth_thruster_ready);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_joystick_control_permission);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.main_propulsion_ap_control_permission);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.azimuth_thruster_main_power_failure);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.azimuth_thruster_backup_power_failure);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.azimuth_thruster_shaft_lock_inlet_air_pressure_low_alarm);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.azimuth_thruster_lub_oil_level_low_alarm);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.azimuth_thruster_lub_oil_temp_high_alarm);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.azimuth_thruster_lub_oil_filter_blocked_alarm);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.azimuth_thruster_lub_oil_press_low_alarm);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.azimuth_thruster_sealing_oil_tank_level_low_alarm);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.azimuth_thruster_vfd1_fault);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.azimuth_thruster_vfd2_fault);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.azimuth_thruster_fu_steering_fault);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.azimuth_thruster_lub_motor1_overload_alarm);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.azimuth_thruster_lub_motor2_overload_alarm);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.azimuth_thruster_angle_order_lost_alarm);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.azimuth_thruster_angle_feedback_lost_alarm);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.azimuth_thruster_steering_power1_supply_fault);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.azimuth_thruster_steering_power2_supply_fault);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.azimuth_thruster_sealing_oil_inlet_air_press_low_alarm);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.azimuth_thruster_sealing_oil_inlet_air_press_high_alarm);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.azimuth_thruster_steering_motor1_temp_high_alarm);
	writer.Key(main_propulsion_str[i++]);writer.Int(one.azimuth_thruster_steering_motor2_temp_high_alarm);
	writer.Key(main_propulsion_str[i++]);writer.Double(one.main_propulsion_vfd_speed_reference);
	writer.Key(main_propulsion_str[i++]);writer.Double(one.main_propulsion_vfd_actual_speed);
	writer.Key(main_propulsion_str[i++]);writer.Double(one.main_propulsion_vfd_actual_torque);
	writer.Key(main_propulsion_str[i++]);writer.Double(one.main_propulsion_motor_current);
	writer.Key(main_propulsion_str[i++]);writer.Double(one.main_propulsion_vfd_dc_voltage);
	writer.Key(main_propulsion_str[i++]);writer.Double(one.main_propulsion_motor_power);

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

void MAIN_PROPULSION::init_mb_addr()
{
#ifndef MAHTS648
	int addr_ai = 8700 + 40*(m_devId-1);
	for(int i = 0; i < 2;i++)
		m_mp_delay_addr[main_propulsion_str[2+i]] = addr_ai + 2 * i;

	for(int i = 4; i < 11;i++)
		m_mp_delay_addr[main_propulsion_str[i]] = addr_ai + 2 * i;	

	for(int i = 0; i < 42; i++)
		m_mp_delay_addr[main_propulsion_str[11+i]] = 6000 + 48*(m_devId-1) + i;

	addr_ai = 7600 + 21*(m_devId - 1);
	for(int i = 0; i < 21;i++)
		m_mp_delay_addr[main_propulsion_str[69+i]] = addr_ai + i;

	addr_ai = 7702 + 18 * (m_devId - 1);
	for(int i = 0; i < 5;i++)
		m_mp_delay_addr[main_propulsion_str[91+i]] = addr_ai + 2 * i;


	//////////////////////////////////////////////////////////
	int addr_di;
	for(int i = 0;i < 16;i++)
		m_mp_inhibit_addr[main_propulsion_str[11+i]] = i;
	for(int i = 0;i < 16;i++)
		m_mp_inhibit_addr[main_propulsion_str[27+i]] = i + 100;
	for(int i = 0;i < 10;i++)
		m_mp_inhibit_addr[main_propulsion_str[43+i]] = i + 200;

	for(int i = 0; i < 2;i++)
		m_mp_inhibit_addr[main_propulsion_str[2+i]] = 2 * i + 300;
	for(int i = 4; i < 8;i++)
		m_mp_inhibit_addr[main_propulsion_str[i]] = 2 * i + 400;	
	for(int i = 0; i < 3;i++)
		m_mp_inhibit_addr[main_propulsion_str[8+i]] = 2 * i + 500;

	for(int i = 0;i < 16;i++)
		m_mp_inhibit_addr[main_propulsion_str[69+i]] = i + 600;
	for(int i = 0;i < 5;i++)
		m_mp_inhibit_addr[main_propulsion_str[85+i]] = i + 700;

	for(int i = 0;i < 5;i++)
		m_mp_inhibit_addr[main_propulsion_str[91+i]] = 2 * i + 802;
#else

#endif
}

void MAIN_PROPULSION::thSetRange()
{
	setRange();
}


void MAIN_PROPULSION::thSetThreshold()
{
	setThreshold();
}

void MAIN_PROPULSION::thSetDelay()
{
	setDelay();
}

void MAIN_PROPULSION::thSetInhibit()
{
	while(1)
	{
		inhibit one = bq_inhibit.take();
		string key = one.m_keyname;
		bool inhib = one.is_inhibit;

		if(!m_mb_plc1)
			continue;
		
		uint16_t status[9]={0};
		int ix = m_mp_inhibit_addr[key]/100;
		int iloop = m_mp_inhibit_addr[key]%100;
		
		std::lock_guard<std::mutex> lock(*m_mux_plc1);
		uint16_t addr_di = 5000 + 3*(m_devId-1);
		if(m_mb_plc1->modbus_read_holding_registers(addr_di,3,status))
			continue;
		
		addr_di = 5800 + 3*(m_devId-1);
		if(m_mb_plc1->modbus_read_holding_registers(addr_di,3,&status[3]))
			continue;

		addr_di = 5300 + 2*(m_devId-1);
		if(m_mb_plc1->modbus_read_holding_registers(addr_di,2,&status[6]))
			continue;

		addr_di = 5400 + 2*(m_devId-1);
		if(m_mb_plc1->modbus_read_holding_registers(addr_di,1,&status[8]))
			continue;


		if(inhib) 
			status[ix] |= (1<<iloop);
		else
			status[ix] &= ~(1<<iloop);
		m_mb_plc1->modbus_write_register(addr_di + ix,status[ix]);
	}

}






