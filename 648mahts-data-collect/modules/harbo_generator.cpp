#include <iostream>
#include <unistd.h>


#include "sgo_sub_pub.h"
#include "sgo_func.h"
#include "sgo_ams/harbo_generator.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

using namespace std;

const char* harbo_generator_str[]={
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
    "harbo_d_g_dcu_unit_battery_volt_dcu_control_power",
    "harbo_d_g_lube_oil_pressure",
    "harbo_d_g_coolant_pressure",
    "harbo_d_g_coolant_temperature",
    "harbo_d_g_lube_oil_temperature",
    "harbo_d_g_exhaust_temperature",
    "harbo_d_g_engine_speed",
    "harbo_d_g_running_hours",
    "harbo_d_g_rpu_unit_battery_a_volt_rpu_main_control_power",
    "harbo_d_g_rpu_unit_battery_b_volt_rpu_backup_control_power",
    "harbo_d_g_coolant_level_low",
    "harbo_d_g_main_power_failure",
    "harbo_d_g_backup_power_failure",
    "harbo_d_g_charger_failure",
    "harbo_d_g_lube_oil_pressure_too_low_shutdown",
    "harbo_d_g_coolant_temperature_too_high_shutdown",
    "harbo_d_g_local_emergency_stop",
    "harbo_d_g_remote_emergency_stop",
    "harbo_d_g_rpu_emergency_stop",
    "harbo_d_g_lube_oil_pressure_low_alarm",
    "harbo_d_g_coolant_pressure_low_alarm",
    "harbo_d_g_coolant_temperature_high_alarm",
    "harbo_d_g_lube_oil_temperature_high_alarm",
    "harbo_d_g_exhaust_temperature_high_alarm",
    "harbo_d_g_overspeed_alarm",
    "harbo_d_g_overspeed_shutdown",
    "harbo_d_g_speed_pickup_sensor_fail",
    "harbo_d_g_dcu_unit_battery_volt_dct_control_power_low",
    "harbo_d_g_start_fail",
    "harbo_d_g_rpu_unit_battery_a_volt_rpu_main_control_power_low",
    "harbo_d_g_rpu_unit_battery_b_volt_rpu_main_control_power_low",
    "harbo_d_g_common_alarm",
    "harbo_d_g_common_warning",
    "harbo_d_g_common_shutdown",
    "harbo_d_g_common_cooldowm",
    "harbo_d_g_common_faultsensor_fail",
    "harbo_d_g_remote_control_status_auxaccess_lock",
    "harbo_d_g_ready_to_start",
    "harbo_d_g_ready_to_load",
    "harbo_d_g_running",
    "harbo_d_g_inlet_oil_flowmeter",
    "harbo_d_g_outlet_oil_flowmeter",
    "device_offline",
#endif
};
	
static const char* productCode = "harbo_generator";

HARBO_GENERATOR::HARBO_GENERATOR()
{}

HARBO_GENERATOR::~HARBO_GENERATOR()
{}



void HARBO_GENERATOR::init(int devId,BlockQueue<harbo_generator> &bq,modbus *mb,mutex *mux)
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
void HARBO_GENERATOR::connectionLostCallback(void* context, char* cause) 
{
	// 处理连接丢失事件
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	HARBO_GENERATOR* client = static_cast<HARBO_GENERATOR*>(ctx->manager);
	
	printf("--%s %d----address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());
	sgo_log_stderr(0,"--%s %d----address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());

	//std::lock_guard<std::mutex> lock(m_losts_mutex);
	client->m_v_lost[ctx->index] = 1;
	client->m_v_property_index_array.clear();

}


// 消息到达回调函数
int HARBO_GENERATOR::messageArrivedCallback(void* context, char* topicName, int topicLen,MQTTClient_message* message) 
{
	// 处理接收到的消息
	std::cout << "Received message on topic: " << topicName << std::endl;

	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	HARBO_GENERATOR* client = static_cast<HARBO_GENERATOR*>(ctx->manager);

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
			client->m_property[harbo_generator_str[i]]=i;
		
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
void HARBO_GENERATOR::deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt) 
{
	// 处理交付完成事件
std::cout << "Message delivery complete." << std::endl;
}

void HARBO_GENERATOR::subClient()
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


void HARBO_GENERATOR::pubClient(BlockQueue<harbo_generator>& bq)
{
	while(1)
	{
		MQTTClient_message pubmsg = MQTTClient_message_initializer;
		//声明消息token
		MQTTClient_deliveryToken token;

		while(!m_pubStop){
			harbo_generator one = bq.take();
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




void HARBO_GENERATOR::start(BlockQueue<harbo_generator> &bq)
{
	m_sub = new thread(&HARBO_GENERATOR::subClient,this);
	m_sub->detach();

	m_pub = new thread(&HARBO_GENERATOR::pubClient,this,ref(bq));
	m_pub->detach();

	m_thr_reconn = new thread(&HARBO_GENERATOR::reconn,this);
	m_thr_reconn->detach();

	m_thr_resend = new thread(&HARBO_GENERATOR::resnd,this);
	m_thr_resend->detach();

	m_thr_chksensor = new thread(&HARBO_GENERATOR::chksensor,this);
	m_thr_chksensor->detach();

/*	thread *thr_range = new thread(&HARBO_GENERATOR::thSetRange,this);
	thr_range->detach();

	thread *thr_threshold = new thread(&HARBO_GENERATOR::thSetThreshold,this);
	thr_threshold->detach();*/

	// thread *thr_inhibit = new thread(&HARBO_GENERATOR::thSetInhibit,this);
	// thr_inhibit->detach();

	// thread *thr_delay = new thread(&HARBO_GENERATOR::thSetDelay,this);
	// thr_delay->detach();	
}

void HARBO_GENERATOR::chksensor()
{
	checkSensorStatus();
}

void HARBO_GENERATOR::resnd()
{
	resend();
}

void HARBO_GENERATOR::reconn()
{
	reconnect();
}



string HARBO_GENERATOR::property_reply (char flag,const harbo_generator& one,const string& msg_id)
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
                case 0:writer.Double(one.harbo_d_g_dcu_unit_battery_volt_dcu_control_power);break;
                case 1:writer.Double(one.harbo_d_g_lube_oil_pressure);break;
                case 2:writer.Double(one.harbo_d_g_coolant_pressure);break;
                case 3:writer.Double(one.harbo_d_g_coolant_temperature);break;
                case 4:writer.Double(one.harbo_d_g_lube_oil_temperature);break;
                case 5:writer.Double(one.harbo_d_g_exhaust_temperature);break;
                case 6:writer.Double(one.harbo_d_g_engine_speed);break;
                case 7:writer.Double(one.harbo_d_g_running_hours);break;
                case 8:writer.Double(one.harbo_d_g_rpu_unit_battery_a_volt_rpu_main_control_power);break;
                case 9:writer.Double(one.harbo_d_g_rpu_unit_battery_b_volt_rpu_backup_control_power);break;
                case 10:writer.Int(one.harbo_d_g_coolant_level_low);break;
                case 11:writer.Int(one.harbo_d_g_main_power_failure);break;
                case 12:writer.Int(one.harbo_d_g_backup_power_failure);break;
                case 13:writer.Int(one.harbo_d_g_charger_failure);break;
                case 14:writer.Int(one.harbo_d_g_lube_oil_pressure_too_low_shutdown);break;
                case 15:writer.Int(one.harbo_d_g_coolant_temperature_too_high_shutdown);break;
                case 16:writer.Int(one.harbo_d_g_local_emergency_stop);break;
                case 17:writer.Int(one.harbo_d_g_remote_emergency_stop);break;
                case 18:writer.Int(one.harbo_d_g_rpu_emergency_stop);break;
                case 19:writer.Int(one.harbo_d_g_lube_oil_pressure_low_alarm);break;
                case 20:writer.Int(one.harbo_d_g_coolant_pressure_low_alarm);break;
                case 21:writer.Int(one.harbo_d_g_coolant_temperature_high_alarm);break;
                case 22:writer.Int(one.harbo_d_g_lube_oil_temperature_high_alarm);break;
                case 23:writer.Int(one.harbo_d_g_exhaust_temperature_high_alarm);break;
                case 24:writer.Int(one.harbo_d_g_overspeed_alarm);break;
                case 25:writer.Int(one.harbo_d_g_overspeed_shutdown);break;
                case 26:writer.Int(one.harbo_d_g_speed_pickup_sensor_fail);break;
                case 27:writer.Int(one.harbo_d_g_dcu_unit_battery_volt_dct_control_power_low);break;
                case 28:writer.Int(one.harbo_d_g_start_fail);break;
                case 29:writer.Int(one.harbo_d_g_rpu_unit_battery_a_volt_rpu_main_control_power_low);break;
                case 30:writer.Int(one.harbo_d_g_rpu_unit_battery_b_volt_rpu_main_control_power_low);break;
                case 31:writer.Int(one.harbo_d_g_common_alarm);break;
                case 32:writer.Int(one.harbo_d_g_common_warning);break;
                case 33:writer.Int(one.harbo_d_g_common_shutdown);break;
                case 34:writer.Int(one.harbo_d_g_common_cooldowm);break;
                case 35:writer.Int(one.harbo_d_g_common_faultsensor_fail);break;
                case 36:writer.Int(one.harbo_d_g_remote_control_status_auxaccess_lock);break;
                case 37:writer.Int(one.harbo_d_g_ready_to_start);break;
                case 38:writer.Int(one.harbo_d_g_ready_to_load);break;
                case 39:writer.Int(one.harbo_d_g_running);break;
                case 40:writer.Double(one.harbo_d_g_inlet_oil_flowmeter);break;
                case 41:writer.Double(one.harbo_d_g_outlet_oil_flowmeter);break;
                case 42:writer.Int(one.device_offline);break;
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


void HARBO_GENERATOR::thSetRange()
{
	setRange();
}


void HARBO_GENERATOR::thSetThreshold()
{
	setThreshold();
}

void HARBO_GENERATOR::thSetDelay()
{
	setDelay();
}

void HARBO_GENERATOR::thSetInhibit()
{
	setInhibit(6489 + 200 * (m_devId - 1),8);
}


void HARBO_GENERATOR::init_mb_addr()
{
	int addr_ai;
	m_mp_delay_addr.clear();
	m_mp_inhibit_addr.clear();
	m_mp_threshold_addr.clear();
	m_mp_range_addr.clear();
	
	int ix_delay = 0;
	addr_ai = 6370 + 200 *(m_devId - 1); 
	for(int i = 0; i < 117; i++)
		m_mp_delay_addr[harbo_generator_str[ix_delay++]] = addr_ai + i;


	/////////////////////////////////////////////////////
	int ix_inh = 0;
	for(int i = 0;i < 7;i++)
	{
		for(int j = 0;j < 16;j++)
			m_mp_inhibit_addr[harbo_generator_str[ix_inh++]] = 100 * i + j;
	}
	
	for(int i = 0;i < 5;i++)
	{
		m_mp_inhibit_addr[harbo_generator_str[ix_inh++]] = 700 + i;
	}

}






