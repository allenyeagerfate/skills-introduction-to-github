#include <iostream>
#include <unistd.h>


#include "sgo_sub_pub.h"
#include "sgo_func.h"
#include "sgo_ams/baudouin_main_generator.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

using namespace std;

const char* baudouin_main_generator_str[]={
#ifndef MAHTS648
	"d_g_engine_speed",
	"d_g_battery_voltage",
	"d_g_boost_t_ps",
	"d_g_boost_t_sb",
	"d_g_speed_r",
	"d_g_speed_ma",
	"d_g_oil_piston_jet",
	"d_g_coolant_press",
	"d_g_exhaust_t_sb",
	"d_g_exhaust_t_ps",
	"d_g_raw_wat_press",
	"d_g_load",
	"d_g_cool_temp_sb",
	"d_g_fuel_temp",
	"d_g_oil_temp",
	"d_g_intake_p_sb",
	"d_g_intake_t_sb",
	"d_g_oil_press",
	"d_g_fuel_press",
	"d_g_fuel_rate",
	"d_g_ecu_battvol_m",
	"d_g_ecu_temp_m",
	"d_g_lever_pos",
	"d_g_rail_press_sb",
	"d_g_tot_fuel_used",
	"d_g_tot_fuel_used_l",
	"d_g_rpu_rpm",
	"d_g_rpu_ubat_a",
	"d_g_rpu_ubat_b",
	"d_g_pri_battery",
	"d_g_sec_battery",
	"d_g_cool_temp_ps",
	"d_g_intake_p_ps",
	"d_g_intake_t_ps",
	"d_g_ecu_battvol_s",
	"d_g_ecu_temp_s",
	"d_g_rail_press_ps",
	"d_g_engine_hours",
	"d_g_run_hours",
	"d_g_service_time",
	"d_g_engine_state",
	"d_g_mode_id",
	"d_g_water_in_fuel",
	"d_g_remoter_start",
	"d_g_remote_stop",
	"d_g_priming_pump",
	"d_g_increase_speed",
	"d_g_ecu_slave_on",
	"d_g_lo_lvl_raw_wat",
	"d_g_mode2",
	"d_g_decrease_speed",
	"d_g_mode1",
	"d_g_fuel_leak_sb",
	"d_g_ecu_master_on",
	"d_g_fuel_leak_ps",
	"d_g_lo_lvl_coolant",
	"d_g_very_lo_oil_p",
	"d_g_very_hi_cool_t",
	"d_g_rpu_sd3",
	"d_g_rpu_sd4",
	"d_g_rpu_sd5",
	"d_g_emstop_rpu",
	"d_g_turning_gear",
	"d_g_light_test",
	"d_g_start_air_p_lo",
	"d_g_diff_eng_oil_p",
	"d_g_pre_heat_cde",
	"d_g_override",
	"d_g_ecucomm",
	"d_g_ecu_warning",
	"d_g_ecu_bin3",
	"d_g_ecu_bin4",
	"d_g_ecu_bin5",
	"d_g_ecu_bin6",
	"d_g_glow_lamp",
	"d_g_ecu_bin8",
	"d_g_ain11",
	"d_g_ain13",
	"d_g_shutoffreqm",
	"d_g_shutoffreqs",
	"d_g_ra153_bout",
	"d_g_detcon",
	"d_g_rpu_fail",
	"d_g_bw_rpu_fuelsol",
	"d_g_bw_rpu_stopsol",
	"d_g_ecu_alarmlist",
	"d_g_ecucompconsist",
	"d_g_actcall_fail",
	"d_g_derate_alarm",
	"d_g_ecudiagblocked",
	"d_g_pwr_limited",
	"d_g_scm_fail",
	"d_g_rtcbatteryflat",
	"d_g_emergency_stop",
	"d_g_overload",
	"d_g_othrcommer",
	"d_g_chrgalternfail",
	"d_g_stop_fail",
	"d_g_overspeed",
	"d_g_underspeed",
	"d_g_pickup_fail",
	"d_g_battery_flat",
	"d_g_start_fail",
	"d_g_op_rpmdep",
	"d_g_cp_rpmdep",
	"d_g_wrn_rpm_diff",
	"d_g_switchtobatt_b",
	"d_g_start_blocking",
	"d_g_load_blocking",
	"d_g_primbatactive",
	"d_g_secbatactive",
	"d_g_emssupply10a",
	"d_g_extrasupply7a",
	"d_g30supply7a",
	"d_g_ecu_yellowlamp",
	"d_g_ecu_red_lamp",
	"d_g_ecu_waittostrt",
	"device_offline",
#else
"d_g_engine_rpm",
"d_g_battery_voltage",
"d_g_boost_temp_portside",
"d_g_boost_temp_starboard",
"d_g_oil_piston_jet_press",
"d_g_coolant_press",
"d_g_exhaust_temp_starboard",
"d_g_exhaust_temp_bank_portside",
"d_g_de_bearing_temp",
"d_g_w_winding_temp",
"d_g_v_winding_temp",
"d_g_u_winding_temp",
"d_g_nde_bearing_temp",
"d_g_load",
"d_g_cool_temp_starboard",
"d_g_fuel_temp",
"d_g_oil_temp",
"d_g_intake_pressure_starboard",
"d_g_intake_temperature_starboard",
"d_g_oil_pressure",
"d_g_fuel_pressure",
"d_g_fuel_rate",
"d_g_ecu_battery_voltage_master",
"d_g_ecu_temperature_master",
"d_g_rail_press_starboard",
"d_g_rpu_ubat_a",
"d_g_rpu_ubat_b",
"d_g_cool_temp_portside",
"d_g_intake_pressure_portside",
"d_g_intake_temperature_portside",
"d_g_rail_press_portside",
"d_g_run_hours",
"d_g_water_in_fuel",
"d_g_fuel_leak_starboard",
"d_g_fuel_leak_portside",
"d_g_low_lvl_coolant",
"d_g_very_low_oil_pressure",
"d_g_very_hi_cool_temperature",
"d_g_start_air_pressure_low",
"d_g_diff_eng_oil_pressure",
"d_g_boost_temp_portside_high",
"d_g_boost_temp_starboard_high",
"d_g_coolant_press_low",
"d_g_exhaust_temp_starboard_high",
"d_g_exhaust_temp_portside_high",
"d_g_raw_wat_press_low",
"d_g_de_bearing_temp_high",
"d_g_w_winding_temp_high",
"d_g_v_winding_temp_high",
"d_g_u_winding_temp_high",
"d_g_nde_bearing_temp_high",
"d_g_fuel_pressure_shutdown",
"d_g_emergency_stop",
"d_g_overload",
"d_g_stop_fail",
"d_g_overspeed",
"d_g_underspeed",
"d_g_pickup_fail",
"d_g_start_fail",
"d_g_start_blocking",
"d_g_common_alarm",
"d_g_common_shutdown",
"d_g_common_cooldown",
"d_g_ready_to_start",
"d_g_ready_to_load",
"d_g_running",
"device_offline",

#endif
};
	
static const char* productCode = "baudouin_main_generator";

BAUDOUIN_MAIN_GENERATOR::BAUDOUIN_MAIN_GENERATOR()
{}

BAUDOUIN_MAIN_GENERATOR::~BAUDOUIN_MAIN_GENERATOR()
{}



void BAUDOUIN_MAIN_GENERATOR::init(int devId,BlockQueue<baudouin_main_generator> &bq,modbus *mb,mutex *mux)
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
void BAUDOUIN_MAIN_GENERATOR::connectionLostCallback(void* context, char* cause) 
{
	// 处理连接丢失事件
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	BAUDOUIN_MAIN_GENERATOR* client = static_cast<BAUDOUIN_MAIN_GENERATOR*>(ctx->manager);
	
	printf("--%s %d----address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());
	sgo_log_stderr(0,"--%s %d----address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());

	//std::lock_guard<std::mutex> lock(m_losts_mutex);
	client->m_v_lost[ctx->index] = 1;
	client->m_v_property_index_array.clear();

}


// 消息到达回调函数
int BAUDOUIN_MAIN_GENERATOR::messageArrivedCallback(void* context, char* topicName, int topicLen,MQTTClient_message* message) 
{
	// 处理接收到的消息
	std::cout << "Received message on topic: " << topicName << std::endl;

	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	BAUDOUIN_MAIN_GENERATOR* client = static_cast<BAUDOUIN_MAIN_GENERATOR*>(ctx->manager);

	std::lock_guard<std::mutex> lock(client->m_mux_arrive);
    char* payloadptr = (char*)message->payload;
	cout<<"payload:"<<payloadptr<<endl;

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
			client->m_property[baudouin_main_generator_str[i]]=i;
		
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
void BAUDOUIN_MAIN_GENERATOR::deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt) 
{
	// 处理交付完成事件
std::cout << "Message delivery complete." << std::endl;
}

void BAUDOUIN_MAIN_GENERATOR::subClient()
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


void BAUDOUIN_MAIN_GENERATOR::pubClient(BlockQueue<baudouin_main_generator>& bq)
{
	while(1)
	{
		MQTTClient_message pubmsg = MQTTClient_message_initializer;
		//声明消息token
		MQTTClient_deliveryToken token;

		while(!m_pubStop){
			baudouin_main_generator one = bq.take();
			/*{
				std::lock_guard<std::mutex> lock(mux_cur_data);
				memcpy(&m_cur_data,&one,sizeof(one));
			}*/
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




void BAUDOUIN_MAIN_GENERATOR::start(BlockQueue<baudouin_main_generator> &bq)
{
	m_sub = new thread(&BAUDOUIN_MAIN_GENERATOR::subClient,this);
	m_sub->detach();

	m_pub = new thread(&BAUDOUIN_MAIN_GENERATOR::pubClient,this,ref(bq));
	m_pub->detach();

	m_thr_reconn = new thread(&BAUDOUIN_MAIN_GENERATOR::reconn,this);
	m_thr_reconn->detach();

	m_thr_resend = new thread(&BAUDOUIN_MAIN_GENERATOR::resnd,this);
	m_thr_resend->detach();

	m_thr_chksensor = new thread(&BAUDOUIN_MAIN_GENERATOR::chksensor,this);
	m_thr_chksensor->detach();

/*	thread *thr_range = new thread(&BAUDOUIN_MAIN_GENERATOR::thSetRange,this);
	thr_range->detach();

	thread *thr_threshold = new thread(&BAUDOUIN_MAIN_GENERATOR::thSetThreshold,this);
	thr_threshold->detach();*/

	// thread *thr_inhibit = new thread(&BAUDOUIN_MAIN_GENERATOR::thSetInhibit,this);
	// thr_inhibit->detach();

	// thread *thr_delay = new thread(&BAUDOUIN_MAIN_GENERATOR::thSetDelay,this);
	// thr_delay->detach();	
}

void BAUDOUIN_MAIN_GENERATOR::chksensor()
{
	checkSensorStatus();
}

void BAUDOUIN_MAIN_GENERATOR::resnd()
{
	resend();
}

void BAUDOUIN_MAIN_GENERATOR::reconn()
{
	reconnect();
}



string BAUDOUIN_MAIN_GENERATOR::property_reply(char flag,const baudouin_main_generator& one,const string& msg_id)
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
	if(0 == m_property_size)
		return string();
	if(m_v_index.empty())
		return string();

	writer.Key("compressData");
	writer.StartArray();

	for(int i = 0; i < m_v_index.size(); i++)
	{
		switch(m_v_index[i]){
			case 0: writer.Double(one.d_g_engine_rpm); break;
			case 1: writer.Double(one.d_g_battery_voltage); break;
			case 2: writer.Double(one.d_g_boost_t_ps); break;
			case 3: writer.Double(one.d_g_boost_t_sb); break;
			case 4: writer.Double(one.d_g_speed_r); break;
			case 5: writer.Double(one.d_g_speed_ma); break;
			case 6: writer.Double(one.d_g_oil_piston_jet); break;
			case 7: writer.Double(one.d_g_coolant_press); break;
			case 8: writer.Double(one.d_g_exhaust_t_sb); break;
			case 9: writer.Double(one.d_g_exhaust_t_ps); break;
			case 10: writer.Double(one.d_g_raw_wat_press); break;
			case 11: writer.Double(one.d_g_load); break;
			case 12: writer.Double(one.d_g_cool_temp_sb); break;
			case 13: writer.Double(one.d_g_fuel_temp); break;
			case 14: writer.Double(one.d_g_oil_temp); break;
			case 15: writer.Double(one.d_g_intake_p_sb); break;
			case 16: writer.Double(one.d_g_intake_t_sb); break;
			case 17: writer.Double(one.d_g_oil_press); break;
			case 18: writer.Double(one.d_g_fuel_press); break;
			case 19: writer.Double(one.d_g_fuel_rate); break;
			case 20: writer.Double(one.d_g_ecu_battvol_m); break;
			case 21: writer.Double(one.d_g_ecu_temp_m); break;
			case 22: writer.Double(one.d_g_lever_pos); break;
			case 23: writer.Double(one.d_g_rail_press_sb); break;
			case 24: writer.Double(one.d_g_tot_fuel_used); break;
			case 25: writer.Double(one.d_g_tot_fuel_used_l); break;
			case 26: writer.Double(one.d_g_rpu_rpm); break;
			case 27: writer.Double(one.d_g_rpu_ubat_a); break;
			case 28: writer.Double(one.d_g_rpu_ubat_b); break;
			case 29: writer.Double(one.d_g_pri_battery); break;
			case 30: writer.Double(one.d_g_sec_battery); break;
			case 31: writer.Double(one.d_g_cool_temp_ps); break;
			case 32: writer.Double(one.d_g_intake_p_ps); break;
			case 33: writer.Double(one.d_g_intake_t_ps); break;
			case 34: writer.Double(one.d_g_ecu_battvol_s); break;
			case 35: writer.Double(one.d_g_ecu_temp_s); break;
			case 36: writer.Double(one.d_g_rail_press_ps); break;
			case 37: writer.Double(one.d_g_engine_hours); break;
			case 38: writer.Double(one.d_g_run_hours); break;
			case 39: writer.Double(one.d_g_service_time); break;
			case 40: writer.Double(one.d_g_engine_state); break;
			case 41: writer.Double(one.d_g_mode_id); break;
			case 42: writer.Int(one.d_g_water_in_fuel); break;
			case 43: writer.Int(one.d_g_remoter_start); break;
			case 44: writer.Int(one.d_g_remote_stop); break;
			case 45: writer.Int(one.d_g_priming_pump); break;
			case 46: writer.Int(one.d_g_increase_speed); break;
			case 47: writer.Int(one.d_g_ecu_slave_on); break;
			case 48: writer.Int(one.d_g_lo_lvl_raw_wat); break;
			case 49: writer.Int(one.d_g_mode2); break;
			case 50: writer.Int(one.d_g_decrease_speed); break;
			case 51: writer.Int(one.d_g_mode1); break;
			case 52: writer.Int(one.d_g_fuel_leak_sb); break;
			case 53: writer.Int(one.d_g_ecu_master_on); break;
			case 54: writer.Int(one.d_g_fuel_leak_ps); break;
			case 55: writer.Int(one.d_g_lo_lvl_coolant); break;
			case 56: writer.Int(one.d_g_very_lo_oil_p); break;
			case 57: writer.Int(one.d_g_very_hi_cool_t); break;
			case 58: writer.Int(one.d_g_rpu_sd3); break;
			case 59: writer.Int(one.d_g_rpu_sd4); break;
			case 60: writer.Int(one.d_g_rpu_sd5); break;
			case 61: writer.Int(one.d_g_emstop_rpu); break;
			case 62: writer.Int(one.d_g_turning_gear); break;
			case 63: writer.Int(one.d_g_light_test); break;
			case 64: writer.Int(one.d_g_start_air_p_lo); break;
			case 65: writer.Int(one.d_g_diff_eng_oil_p); break;
			case 66: writer.Int(one.d_g_pre_heat_cde); break;
			case 67: writer.Int(one.d_g_override); break;
			case 68: writer.Int(one.d_g_ecucomm); break;
			case 69: writer.Int(one.d_g_ecu_warning); break;
			case 70: writer.Int(one.d_g_ecu_bin3); break;
			case 71: writer.Int(one.d_g_ecu_bin4); break;
			case 72: writer.Int(one.d_g_ecu_bin5); break;
			case 73: writer.Int(one.d_g_ecu_bin6); break;
			case 74: writer.Int(one.d_g_glow_lamp); break;
			case 75: writer.Int(one.d_g_ecu_bin8); break;
			case 76: writer.Int(one.d_g_ain11); break;
			case 77: writer.Int(one.d_g_ain13); break;
			case 78: writer.Int(one.d_g_shutoffreqm); break;
			case 79: writer.Int(one.d_g_shutoffreqs); break;
			case 80: writer.Int(one.d_g_ra153_bout); break;
			case 81: writer.Int(one.d_g_detcon); break;
			case 82: writer.Int(one.d_g_rpu_fail); break;
			case 83: writer.Int(one.d_g_bw_rpu_fuelsol); break;
			case 84: writer.Int(one.d_g_bw_rpu_stopsol); break;
			case 85: writer.Int(one.d_g_ecu_alarmlist); break;
			case 86: writer.Int(one.d_g_ecucompconsist); break;
			case 87: writer.Int(one.d_g_actcall_fail); break;
			case 88: writer.Int(one.d_g_derate_alarm); break;
			case 89: writer.Int(one.d_g_ecudiagblocked); break;
			case 90: writer.Int(one.d_g_pwr_limited); break;
			case 91: writer.Int(one.d_g_scm_fail); break;
			case 92: writer.Int(one.d_g_rtcbatteryflat); break;
			case 93: writer.Int(one.d_g_emergency_stop); break;
			case 94: writer.Int(one.d_g_overload); break;
			case 95: writer.Int(one.d_g_othrcommer); break;
			case 96: writer.Int(one.d_g_chrgalternfail); break;
			case 97: writer.Int(one.d_g_stop_fail); break;
			case 98: writer.Int(one.d_g_overspeed); break;
			case 99: writer.Int(one.d_g_underspeed); break;
			case 100: writer.Int(one.d_g_pickup_fail); break;
			case 101: writer.Int(one.d_g_battery_flat); break;
			case 102: writer.Int(one.d_g_start_fail); break;
			case 103: writer.Int(one.d_g_op_rpmdep); break;
			case 104: writer.Int(one.d_g_cp_rpmdep); break;
			case 105: writer.Int(one.d_g_wrn_rpm_diff); break;
			case 106: writer.Int(one.d_g_switchtobatt_b); break;
			case 107: writer.Int(one.d_g_start_blocking); break;
			case 108: writer.Int(one.d_g_load_blocking); break;
			case 109: writer.Int(one.d_g_primbatactive); break;
			case 110: writer.Int(one.d_g_secbatactive); break;
			case 111: writer.Int(one.d_g_emssupply10a); break;
			case 112: writer.Int(one.d_g_extrasupply7a); break;
			case 113: writer.Int(one.d_g30supply7a); break;
			case 114: writer.Int(one.d_g_ecu_yellowlamp); break;
			case 115: writer.Int(one.d_g_ecu_red_lamp); break;
			case 116: writer.Int(one.d_g_ecu_waittostrt); break;
			case 117: writer.Int(one.device_offline); break;

			default:
				break;
		}

	}
	writer.EndArray();

	
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
//cout<<"bau:"<<data<<endl;
	return strBuf.GetString();
}

void BAUDOUIN_MAIN_GENERATOR::thSetRange()
{
	setRange();
}


void BAUDOUIN_MAIN_GENERATOR::thSetThreshold()
{
	setThreshold();
}

void BAUDOUIN_MAIN_GENERATOR::thSetDelay()
{
	setDelay();
}

void BAUDOUIN_MAIN_GENERATOR::thSetInhibit()
{
	setInhibit(6489 + 200 * (m_devId - 1),8);
}


void BAUDOUIN_MAIN_GENERATOR::init_mb_addr()
{
	int addr_ai;
	m_mp_delay_addr.clear();
	m_mp_inhibit_addr.clear();
	m_mp_threshold_addr.clear();
	m_mp_range_addr.clear();
	
	int ix_delay = 0;
	addr_ai = 6370 + 200 *(m_devId - 1); 
	for(int i = 0; i < 117; i++)
		m_mp_delay_addr[baudouin_main_generator_str[ix_delay++]] = addr_ai + i;


	/////////////////////////////////////////////////////
	int ix_inh = 0;
	for(int i = 0;i < 7;i++)
	{
		for(int j = 0;j < 16;j++)
			m_mp_inhibit_addr[baudouin_main_generator_str[ix_inh++]] = 100 * i + j;
	}
	
	for(int i = 0;i < 5;i++)
	{
		m_mp_inhibit_addr[baudouin_main_generator_str[ix_inh++]] = 700 + i;
	}
	




	
}






