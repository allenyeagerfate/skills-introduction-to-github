#include <iostream>
#include <unistd.h>

#include "sgo_sub_pub.h"
#include "sgo_func.h"
#include "sgo_ams/miscellaneous.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

using namespace std;

const char* miscellaneous_str[]={
#ifndef MAHTS648
	,
	"fire_alarm",
	"sewage_water_treatment_plant_common_alarm",
	"fuel_oil_purifier_common_alarm",
	"oil_water_separator_common_alarm",
	"oil_water_separator15ppm_alarm",
	"oil_water_separator_running_timeout_alarm_nc",
	"no1_fresh_water_generator_common_alarm",
	"no2_fresh_water_generator_common_alarm",
	"fresh_water_generator_no1_boost_pump_overload",
	"fresh_water_generator_no2_boost_pump_overload",
	"water_sprinkler_system_common_alarm",
	"no1_submersible_pump_failure",
	"no1_submersible_pump_running",
	"no2_submersible_pump_failure",
	"no2_submersible_pump_running",
	"no3_submersible_pump_failure",
	"no3_submersible_pump_running",
	"no4_submersible_pump_failure",
	"no4_submersible_pump_running",
	"ac440v_galley_equipment_db_low_insulation",
	"ac230v_galley_equipment_db_low_insulation",
	"co2_release_alarm_system_power_failure_alarm",
	"co2_leakage_alarm",
	"co2_release_alarm",
	"e_r_co2_release_pre_alarm",
	"e_r_co2_release_valve_opened",
	"paint_room_co2_release_pre_alarm",
	"paint_room_co2_release_valve_opened",
	"no1_anti_fouling_device_common_alarm",
	"no2_anti_fouling_device_common_alarm",
	"uv_sterilizer_common_alarm",
	"navigation_aids_system_common_alarm",
	"public_address_general_alarm_system_fault",
	"public_address_general_alarm_system_power_fault",
	"auto_telephone_system_failure",
	"audio_visual_alarm_column_main_power_failure",
	"audio_visual_alarm_column_backup_power_failure",
	"no1_leg_control_unit_common_alarm",
	"no2_leg_control_unit_common_alarm",
	"no3_leg_control_unit_common_alarm",
	"no4_leg_control_unit_common_alarm",
	"fog_horn_controller_power_failure",
	"bnwas_main_unit_power_failure",
	"fresh_water_hydrophore_failure",
	"sea_water_hydrophore_failure",
	"calorifier_failure",
	"spare",
	"no1_ups_for_ams_power_failure",
	"no2_ups_for_ams_power_failure",
	"no3_ups_for_ams_power_failure",
	"no1_crane_fire_alarm",
	"no2_crane_fire_alarm",
	"engineer_alarm_system_power_failure",
	"watertight_door_indication_system_common_alarm",
	"freshwater_filling_flowmeter",
	"device_offline",
#else
     "no1_watertight_door_common_alarm",
     "no2_watertight_door_common_alarm",
     "no3_watertight_door_common_alarm",
     "no4_watertight_door_common_alarm",
     "fuel_oil_purifier_running",
     "fuel_oil_purifier_abnormal_alarm",
     "oily_water_separator_oil_content_high",
     "oily_water_separator_common_alarm",
     "ele_heating_calorifier_heater_running",
     "ele_heating_calorifier_hot_water_circulating_pump_running",
     "ele_heating_calorifier_high_temp_alarm",
     "fresh_water_maker_common_alarm",
     "fifi_system_common_alarm",
     "sw_fw_fress_set_common_alarm",
     "miscellaneousbffdc0c37da000f4",
     "electrical_valve_remote_control_system_common_alarm",
     "a_c_common_alarm",
     "provision_refrigeration_system_fault",
     "freezer_room_high_temp_alarm",
     "chiller_room_high_temp_alarm",
     "deck_machine_hpu_packdeck_machine_hpu_pack_low_oil_level_alarm",
     "miscellaneous49510c947aae3742",
     "deck_machine_hpu_packdeck_machine_hpu_pack_high_oil_temp_alarm",
     "miscellaneousb8c79c2fcc0eddd2",
     "mud_agitator_system_common_alarm",
     "bnwas_system_failure",
     "no1_gyro_ac220v_power_failure_alarm",
     "no2_gyro_ac220v_power_failure_alarm",
     "no3_gyro_ac220v_power_failure_alarm",
     "autopilot_system_failure_alarm",
     "gyro_compass_distribution_unit_system_failure",
     "gyro_compass_distribution_unit_gyro1_failure",
     "gyro_compass_distribution_unit_gyro2_failure",
     "gyro_compass_distribution_unit_gyro3_failure",
     "gyro_compass_distribution_unit_tmc_failure",
     "gyro_compass_distribution_unit_can_bus_failure",
     "gyro_compass_distribution_unit_sensor_diff_alarm",
     "gyro_compass_distribution_unit_heading_monitor_alarm",
     "watertight_door_or_hatch_cover_power_failure",
     "fire_take_place_signal",
     "fire_alarm_system_failure",//将fire_system_failure_alarm改为fire_alarm_system_failure
     "from_pa_system_power_fault",
     "from_ga_system_power_fault",
     "general_service_air_compressor_common_alarm",
     "alarm_column_machinery_alarm",
     "alarm_column_tank_level_alarm",
     "alarm_column_relay_box_power_fault",
     "engine_call_system_power_fault",
     "mian_propulsion_order_telegraph_p_s_power_fault",
     "refri_call_main_unit_power_fault",
     "hospital_call_main_unit_power_fault",
     "watermist_main_control_boxecr_system_failure",
     "ams_ups1_common_alarm",
     "ams_ups2_common_alarm",
     "control_box_loss_of_power",
     "purifier_motor_overload",
     "purifier_outlet_pressure_high",
     "no1_gyro_dc24v_power_failure_alarm",
     "no2_gyro_dc24v_power_failure_alarm",
     "no3_gyro_dc24v_power_failure_alarm",
     "autopilot_off_heading_alarm",
     "watertight_door_or_hatch_cover_open_alarm",
     "device_offline",
#endif
};



	
static const char* productCode = "miscellaneous";

MISCELLANEOUS::MISCELLANEOUS()
{}

MISCELLANEOUS::~MISCELLANEOUS()
{}

void MISCELLANEOUS::init(int devId,BlockQueue<miscellaneous> &bq,modbus *mb,mutex *mux)
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
void MISCELLANEOUS::connectionLostCallback(void* context, char* cause) 
{
	// 处理连接丢失事件
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	MISCELLANEOUS* client = static_cast<MISCELLANEOUS*>(ctx->manager);
	
	printf("--%s %d----address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());
	sgo_log_stderr(0,"--%s %d----address[%d]:%s",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());

	//std::lock_guard<std::mutex> lock(m_losts_mutex);
	client->m_v_lost[ctx->index] = 1;
	client->m_v_property_index_array.clear();

}


// 消息到达回调函数
int MISCELLANEOUS::messageArrivedCallback(void* context, char* topicName, int topicLen,MQTTClient_message* message) 
{
	// 处理接收到的消息
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	MISCELLANEOUS* client = static_cast<MISCELLANEOUS*>(ctx->manager);
	
	printf("--%s %d----address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());
	sgo_log_stderr(0,"--%s %d----address[%d]:%s",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());

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
			client->m_property[miscellaneous_str[i]]=i;
		
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
void MISCELLANEOUS::deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt) 
{
	// 处理交付完成事件
	//std::cout << "Message delivery complete." << std::endl;
}

void MISCELLANEOUS::subClient()
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


void MISCELLANEOUS::pubClient(BlockQueue<miscellaneous>& bq)
{
	while(1)
	{
		MQTTClient_message pubmsg = MQTTClient_message_initializer;
		//声明消息token
		MQTTClient_deliveryToken token;

		while(!m_pubStop){
			miscellaneous one = bq.take();
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




void MISCELLANEOUS::start(BlockQueue<miscellaneous> &bq)
{
	m_sub = new thread(&MISCELLANEOUS::subClient,this);
	m_sub->detach();

	m_pub = new thread(&MISCELLANEOUS::pubClient,this,ref(bq));
	m_pub->detach();

	m_thr_reconn = new thread(&MISCELLANEOUS::reconn,this);
	m_thr_reconn->detach();

	m_thr_resend = new thread(&MISCELLANEOUS::resnd,this);
	m_thr_resend->detach();

	m_thr_chksensor = new thread(&MISCELLANEOUS::chksensor,this);
	m_thr_chksensor->detach();

	thread *thr_range = new thread(&MISCELLANEOUS::thSetRange,this);
	thr_range->detach();

	thread *thr_inhibit = new thread(&MISCELLANEOUS::thSetInhibit,this);
	thr_inhibit->detach();

	thread *thr_delay = new thread(&MISCELLANEOUS::thSetDelay,this);
	thr_delay->detach();
}

void MISCELLANEOUS::chksensor()
{
	checkSensorStatus();
}

void MISCELLANEOUS::resnd()
{
	resend();
}

void MISCELLANEOUS::reconn()
{
	reconnect();
}


string MISCELLANEOUS::property_reply(char flag,const miscellaneous& one,const string& msg_id)
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
	
		for(int i = 0; i < m_v_index.size(); i++) //下发少个字段
		{
			switch(m_v_index[i]){
#ifndef MAHTS648
				case 0:writer.Int(one.fire_alarm_control_unit_system_failure);break;
				case 1:writer.Int(one.fire_alarm);break;
				case 2:writer.Int(one.sewage_water_treatment_plant_common_alarm);break;
				case 3:writer.Int(one.fuel_oil_purifier_common_alarm);break;
				case 4:writer.Int(one.oil_water_separator_common_alarm);break;
				case 5:writer.Int(one.oil_water_separator15ppm_alarm);break;
				case 6:writer.Int(one.oil_water_separator_running_timeout_alarm_nc);break;
				case 7:writer.Int(one.no1_fresh_water_generator_common_alarm);break;
				case 8:writer.Int(one.no2_fresh_water_generator_common_alarm);break;
				case 9:writer.Int(one.fresh_water_generator_no1_boost_pump_overload);break;
				case 10:writer.Int(one.fresh_water_generator_no2_boost_pump_overload);break;
				case 11:writer.Int(one.water_sprinkler_system_common_alarm);break;
				case 12:writer.Int(one.no1_submersible_pump_failure);break;
				case 13:writer.Int(one.no1_submersible_pump_running);break;
				case 14:writer.Int(one.no2_submersible_pump_failure);break;
				case 15:writer.Int(one.no2_submersible_pump_running);break;
				case 16:writer.Int(one.no3_submersible_pump_failure);break;
				case 17:writer.Int(one.no3_submersible_pump_running);break;
				case 18:writer.Int(one.no4_submersible_pump_failure);break;
				case 19:writer.Int(one.no4_submersible_pump_running);break;
				case 20:writer.Int(one.ac440v_galley_equipment_db_low_insulation);break;
				case 21:writer.Int(one.ac230v_galley_equipment_db_low_insulation);break;
				case 22:writer.Int(one.co2_release_alarm_system_power_failure_alarm);break;
				case 23:writer.Int(one.co2_leakage_alarm);break;
				case 24:writer.Int(one.co2_release_alarm);break;
				case 25:writer.Int(one.e_r_co2_release_pre_alarm);break;
				case 26:writer.Int(one.e_r_co2_release_valve_opened);break;
				case 27:writer.Int(one.paint_room_co2_release_pre_alarm);break;
				case 28:writer.Int(one.paint_room_co2_release_valve_opened);break;
				case 29:writer.Int(one.no1_anti_fouling_device_common_alarm);break;
				case 30:writer.Int(one.no2_anti_fouling_device_common_alarm);break;
				case 31:writer.Int(one.uv_sterilizer_common_alarm);break;
				case 32:writer.Int(one.navigation_aids_system_common_alarm);break;
				case 33:writer.Int(one.public_address_general_alarm_system_fault);break;
				case 34:writer.Int(one.public_address_general_alarm_system_power_fault);break;
				case 35:writer.Int(one.auto_telephone_system_failure);break;
				case 36:writer.Int(one.audio_visual_alarm_column_main_power_failure);break;
				case 37:writer.Int(one.audio_visual_alarm_column_backup_power_failure);break;
				case 38:writer.Int(one.no1_leg_control_unit_common_alarm);break;
				case 39:writer.Int(one.no2_leg_control_unit_common_alarm);break;
				case 40:writer.Int(one.no3_leg_control_unit_common_alarm);break;
				case 41:writer.Int(one.no4_leg_control_unit_common_alarm);break;
				case 42:writer.Int(one.fog_horn_controller_power_failure);break;
				case 43:writer.Int(one.bnwas_main_unit_power_failure);break;
				case 44:writer.Int(one.fresh_water_hydrophore_failure);break;
				case 45:writer.Int(one.sea_water_hydrophore_failure);break;
				case 46:writer.Int(one.calorifier_failure);break;			
				case 47:writer.Int(one.spare);break;
				case 48:writer.Int(one.no1_ups_for_ams_power_failure);break;
				case 49:writer.Int(one.no2_ups_for_ams_power_failure);break;
				case 50:writer.Int(one.no3_ups_for_ams_power_failure);break;
				case 51:writer.Int(one.no1_crane_fire_alarm);break;
				case 52:writer.Int(one.no2_crane_fire_alarm);break;
				case 53:writer.Int(one.engineer_alarm_system_power_failure);break;
				case 54:writer.Int(one.watertight_door_indication_system_common_alarm);break;
				case 55:writer.Double(one.freshwater_filling_flowmeter);break;
				case 56:writer.Int(one.device_offline);break;
#else
                case 0:writer.Int(one.no1_watertight_door_common_alarm);break;
                case 1:writer.Int(one.no2_watertight_door_common_alarm);break;
                case 2:writer.Int(one.no3_watertight_door_common_alarm);break;
                case 3:writer.Int(one.no4_watertight_door_common_alarm);break;
                case 4:writer.Int(one.fuel_oil_purifier_running);break;
                case 5:writer.Int(one.fuel_oil_purifier_abnormal_alarm);break;
                case 6:writer.Int(one.oily_water_separator_oil_content_high);break;
                case 7:writer.Int(one.oily_water_separator_common_alarm);break;
                case 8:writer.Int(one.ele_heating_calorifier_heater_running);break;
                case 9:writer.Int(one.ele_heating_calorifier_hot_water_circulating_pump_running);break;
                case 10:writer.Int(one.ele_heating_calorifier_high_temp_alarm);break;
                case 11:writer.Int(one.fresh_water_maker_common_alarm);break;
                case 12:writer.Int(one.fifi_system_common_alarm);break;
                case 13:writer.Int(one.sw_fw_fress_set_common_alarm);break;
                case 14:writer.Int(one.miscellaneousbffdc0c37da000f4);break;
                case 15:writer.Int(one.electrical_valve_remote_control_system_common_alarm);break;
                case 16:writer.Int(one.a_c_common_alarm);break;
                case 17:writer.Int(one.provision_refrigeration_system_fault);break;
                case 18:writer.Int(one.freezer_room_high_temp_alarm);break;
                case 19:writer.Int(one.chiller_room_high_temp_alarm);break;
                case 20:writer.Int(one.deck_machine_hpu_packdeck_machine_hpu_pack_low_oil_level_alarm);break;
                case 21:writer.Int(one.miscellaneous49510c947aae3742);break;
                case 22:writer.Int(one.deck_machine_hpu_packdeck_machine_hpu_pack_high_oil_temp_alarm);break;
                case 23:writer.Int(one.miscellaneousb8c79c2fcc0eddd2);break;
                case 24:writer.Int(one.mud_agitator_system_common_alarm);break;
                case 25:writer.Int(one.bnwas_system_failure);break;
                case 26:writer.Int(one.no1_gyro_ac220v_power_failure_alarm);break;
                case 27:writer.Int(one.no2_gyro_ac220v_power_failure_alarm);break;
                case 28:writer.Int(one.no3_gyro_ac220v_power_failure_alarm);break;
                case 29:writer.Int(one.autopilot_system_failure_alarm);break;
                case 30:writer.Int(one.gyro_compass_distribution_unit_system_failure);break;
                case 31:writer.Int(one.gyro_compass_distribution_unit_gyro1_failure);break;
                case 32:writer.Int(one.gyro_compass_distribution_unit_gyro2_failure);break;
                case 33:writer.Int(one.gyro_compass_distribution_unit_gyro3_failure);break;
                case 34:writer.Int(one.gyro_compass_distribution_unit_tmc_failure);break;
                case 35:writer.Int(one.gyro_compass_distribution_unit_can_bus_failure);break;
                case 36:writer.Int(one.gyro_compass_distribution_unit_sensor_diff_alarm);break;
                case 37:writer.Int(one.gyro_compass_distribution_unit_heading_monitor_alarm);break;
                case 38:writer.Int(one.watertight_door_or_hatch_cover_power_failure);break;
                case 39:writer.Int(one.fire_take_place_signal);break;
                case 40:writer.Int(one.fire_alarm_system_failure);break;//将fire_system_failure_alarm改为fire_alarm_system_failure
                case 41:writer.Int(one.from_pa_system_power_fault);break;
                case 42:writer.Int(one.from_ga_system_power_fault);break;
                case 43:writer.Int(one.general_service_air_compressor_common_alarm);break;
                case 44:writer.Int(one.alarm_column_machinery_alarm);break;
                case 45:writer.Int(one.alarm_column_tank_level_alarm);break;
                case 46:writer.Int(one.alarm_column_relay_box_power_fault);break;
                case 47:writer.Int(one.engine_call_system_power_fault);break;
                case 48:writer.Int(one.mian_propulsion_order_telegraph_p_s_power_fault);break;
                case 49:writer.Int(one.refri_call_main_unit_power_fault);break;
                case 50:writer.Int(one.hospital_call_main_unit_power_fault);break;
                case 51:writer.Int(one.watermist_main_control_boxecr_system_failure);break;
                case 52:writer.Int(one.ams_ups1_common_alarm);break;
                case 53:writer.Int(one.ams_ups2_common_alarm);break;
                case 54:writer.Int(one.control_box_loss_of_power);break;
                case 55:writer.Int(one.purifier_motor_overload);break;
                case 56:writer.Int(one.purifier_outlet_pressure_high);break;
                case 57:writer.Int(one.no1_gyro_dc24v_power_failure_alarm);break;
                case 58:writer.Int(one.no2_gyro_dc24v_power_failure_alarm);break;
                case 59:writer.Int(one.no3_gyro_dc24v_power_failure_alarm);break;
                case 60:writer.Int(one.autopilot_off_heading_alarm);break;
                case 61:writer.Int(one.watertight_door_or_hatch_cover_open_alarm);break;
                case 62:writer.Int(one.device_offline);break;

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
	writer.Key(miscellaneous_str[i++]);writer.Int(one.fire_alarm_control_unit_system_failure);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.fire_alarm);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.sewage_water_treatment_plant_common_alarm);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.fuel_oil_purifier_common_alarm);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.oil_water_separator_common_alarm);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.oil_water_separator15ppm_alarm);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.oil_water_separator_running_timeout_alarm_nc);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.no1_fresh_water_generator_common_alarm);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.no2_fresh_water_generator_common_alarm);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.fresh_water_generator_no1_boost_pump_overload);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.fresh_water_generator_no2_boost_pump_overload);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.water_sprinkler_system_common_alarm);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.no1_submersible_pump_failure);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.no1_submersible_pump_running);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.no2_submersible_pump_failure);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.no2_submersible_pump_running);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.no3_submersible_pump_failure);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.no3_submersible_pump_running);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.no4_submersible_pump_failure);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.no4_submersible_pump_running);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.ac440v_galley_equipment_db_low_insulation);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.ac230v_galley_equipment_db_low_insulation);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.co2_release_alarm_system_power_failure_alarm);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.co2_leakage_alarm);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.co2_release_alarm);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.e_r_co2_release_pre_alarm);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.e_r_co2_release_valve_opened);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.paint_room_co2_release_pre_alarm);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.paint_room_co2_release_valve_opened);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.no1_anti_fouling_device_common_alarm);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.no2_anti_fouling_device_common_alarm);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.uv_sterilizer_common_alarm);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.navigation_aids_system_common_alarm);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.public_address_general_alarm_system_fault);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.public_address_general_alarm_system_power_fault);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.auto_telephone_system_failure);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.audio_visual_alarm_column_main_power_failure);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.audio_visual_alarm_column_backup_power_failure);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.no1_leg_control_unit_common_alarm);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.no2_leg_control_unit_common_alarm);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.no3_leg_control_unit_common_alarm);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.no4_leg_control_unit_common_alarm);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.fog_horn_controller_power_failure);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.bnwas_main_unit_power_failure);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.fresh_water_hydrophore_failure);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.sea_water_hydrophore_failure);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.calorifier_failure);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.no1_ups_for_ams_power_failure);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.no2_ups_for_ams_power_failure);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.no3_ups_for_ams_power_failure);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.no1_crane_fire_alarm);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.no2_crane_fire_alarm);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.engineer_alarm_system_power_failure);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.watertight_door_indication_system_common_alarm);
	writer.Key(miscellaneous_str[i++]);writer.Double(one.freshwater_filling_flowmeter);
	writer.Key(miscellaneous_str[i++]);writer.Int(one.device_offline);
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


void MISCELLANEOUS::thSetRange()
{
	while(1)
	{
		range one = bq_range.take();
		string key = one.m_keyname;
		float start = one.start;
		float end = one.end;

		std::pair<float, float> rg = std::make_pair(start, end);

	    for(int i = 0; i < sizeof(miscellaneous_str); ++i)
	    {
            std::string s(miscellaneous_str[i]);
		    if(s == key)
		    {
                switch(i)
		    	{
                    case 0:{m_data_store.fire_alarm_control_unit_system_failure.range=rg;break;}
                    case 1:{m_data_store.fire_alarm.range=rg;break;}
                    case 2:{m_data_store.sewage_water_treatment_plant_common_alarm.range=rg;break;}
                    case 3:{m_data_store.fuel_oil_purifier_common_alarm.range=rg;break;}
                    case 4:{m_data_store.oil_water_separator_common_alarm.range=rg;break;}
                    case 5:{m_data_store.oil_water_separator15ppm_alarm.range=rg;break;}
                    case 6:{m_data_store.oil_water_separator_running_timeout_alarm_nc.range=rg;break;}
                    case 7:{m_data_store.no1_fresh_water_generator_common_alarm.range=rg;break;}
                    case 8:{m_data_store.no2_fresh_water_generator_common_alarm.range=rg;break;}
                    case 9:{m_data_store.fresh_water_generator_no1_boost_pump_overload.range=rg;break;}
                    case 10:{m_data_store.fresh_water_generator_no2_boost_pump_overload.range=rg;break;}
                    case 11:{m_data_store.water_sprinkler_system_common_alarm.range=rg;break;}
                    case 12:{m_data_store.no1_submersible_pump_failure.range=rg;break;}
                    case 13:{m_data_store.no1_submersible_pump_running.range=rg;break;}
                    case 14:{m_data_store.no2_submersible_pump_failure.range=rg;break;}
                    case 15:{m_data_store.no2_submersible_pump_running.range=rg;break;}
                    case 16:{m_data_store.no3_submersible_pump_failure.range=rg;break;}
                    case 17:{m_data_store.no3_submersible_pump_running.range=rg;break;}
                    case 18:{m_data_store.no4_submersible_pump_failure.range=rg;break;}
                    case 19:{m_data_store.no4_submersible_pump_running.range=rg;break;}
                    case 20:{m_data_store.ac440v_galley_equipment_db_low_insulation.range=rg;break;}
                    case 21:{m_data_store.ac230v_galley_equipment_db_low_insulation.range=rg;break;}
                    case 22:{m_data_store.no1_anti_fouling_device_common_alarm.range=rg;break;}
                    case 23:{m_data_store.no2_anti_fouling_device_common_alarm.range=rg;break;}
                    case 24:{m_data_store.uv_sterilizer_common_alarm.range=rg;break;}
                    case 25:{m_data_store.navigation_aids_system_common_alarm.range=rg;break;}
                    case 26:{m_data_store.public_address_general_alarm_system_fault.range=rg;break;}
                    case 27:{m_data_store.public_address_general_alarm_system_power_fault.range=rg;break;}
                    case 28:{m_data_store.auto_telephone_system_failure.range=rg;break;}
                    case 29:{m_data_store.audio_visual_alarm_column_main_power_failure .range=rg;break;}
                    case 30:{m_data_store.audio_visual_alarm_column_backup_power_failure.range=rg;break;}
                    case 31:{m_data_store.no1_leg_control_unit_common_alarm.range=rg;break;}
                    case 32:{m_data_store.no2_leg_control_unit_common_alarm.range=rg;break;}
                    case 33:{m_data_store.no3_leg_control_unit_common_alarm.range=rg;break;}
                    case 34:{m_data_store.no4_leg_control_unit_common_alarm.range=rg;break;}
                    case 35:{m_data_store.fog_horn_controller_power_failure.range=rg;break;}
                    case 36:{m_data_store.bnwas_main_unit_power_failure.range=rg;break;}
                    case 37:{m_data_store.fresh_water_hydrophore_failure.range=rg;break;}
                    case 38:{m_data_store.sea_water_hydrophore_failure.range=rg;break;}
                    case 39:{m_data_store.calorifier_failure.range=rg;break;}
                    case 40:{m_data_store.no1_ups_for_ams_power_failure.range=rg;break;}
                    case 41:{m_data_store.no2_ups_for_ams_power_failure.range=rg;break;}
                    case 42:{m_data_store.no3_ups_for_ams_power_failure.range=rg;break;}
                    case 43:{m_data_store.no4_ups_for_ams_power_failure.range=rg;break;}
                    case 44:{m_data_store.watertight_door_indication_system_common_alarm.range=rg;break;}
                    case 45:{m_data_store.freshwater_filling_flowmeter.range=rg;break;}
                    case 46:{m_data_store.fuel_oil_filling_flowmeter.range=rg;break;}
                    case 47:{m_data_store.device_offline.range=rg;break;}  
		    	}
    
		    	break;
		    }
	    }       
	}
	//setRange();
}

void MISCELLANEOUS::thSetThreshold()
{
	setThreshold();
}

void MISCELLANEOUS::thSetDelay()
{
	setDelay();
}


void MISCELLANEOUS::thSetInhibit()
{
	setInhibit(5800,20);
}


void MISCELLANEOUS::init_mb_addr()
{
///ECC 报警屏蔽 改成45801 开始

	int addr_ai;
	/////////////开关量
	addr_ai = 8020;
	int i = 0;
	for(i;i < 5;i++)
	{
		m_mp_delay_addr[miscellaneous_str[42 + i]] = addr_ai + i;
		m_mp_inhibit_addr[miscellaneous_str[42 + i]] = 100 + i;
	}
	m_mp_delay_addr[miscellaneous_str[32]] = addr_ai + 6;
	m_mp_inhibit_addr[miscellaneous_str[32]] = 106;
	m_mp_delay_addr[miscellaneous_str[0]] = addr_ai + 7;
	m_mp_inhibit_addr[miscellaneous_str[0]] = 107;
	m_mp_delay_addr[miscellaneous_str[1]] = addr_ai + 8;
	m_mp_inhibit_addr[miscellaneous_str[1]] = 108;
	m_mp_delay_addr[miscellaneous_str[11]] = addr_ai + 9;
	m_mp_inhibit_addr[miscellaneous_str[11]] = 109;

	for(i = 0;i < 4;i++)
	{
		m_mp_delay_addr[miscellaneous_str[42 + i]] = addr_ai + 25 + i;
		m_mp_inhibit_addr[miscellaneous_str[42 + i]] = 209 + i;
	}
	
	for(i = 0;i < 3;i++)
	{
		m_mp_delay_addr[miscellaneous_str[33 + i]] = addr_ai + 29 + i;
		m_mp_inhibit_addr[miscellaneous_str[33 + i]] = 213 + i;
	}
	
	for(i = 0;i < 9;i++)
	{
		m_mp_delay_addr[miscellaneous_str[2 + i]] = addr_ai + 35 + i;
		m_mp_inhibit_addr[miscellaneous_str[2 + i]] = 303 + i;
	}
	
	for(i = 0;i < 3;i++)
	{
		m_mp_delay_addr[miscellaneous_str[22 + i]] = addr_ai + 61 + i;
		m_mp_inhibit_addr[miscellaneous_str[22 + i]] = 413 + i;
	}
	for(i = 0;i < 6;i++)
	{
		m_mp_delay_addr[miscellaneous_str[25 + i]] = addr_ai + 64 + i;
		m_mp_inhibit_addr[miscellaneous_str[25 + i]] = 500 + i;
	}

	for(i = 0;i < 2;i++)
	{
		m_mp_delay_addr[miscellaneous_str[20 + i]] = addr_ai + 70 + i;
		m_mp_inhibit_addr[miscellaneous_str[20 + i]] = 506 + i;
	}

	m_mp_delay_addr[miscellaneous_str[31]] = addr_ai + 73;
	m_mp_inhibit_addr[miscellaneous_str[31]] = 509;
	m_mp_delay_addr[miscellaneous_str[50]] = addr_ai + 74;
	m_mp_inhibit_addr[miscellaneous_str[50]] = 510;
	m_mp_delay_addr[miscellaneous_str[51]] = addr_ai + 75;
	m_mp_inhibit_addr[miscellaneous_str[51]] = 511;


	m_mp_delay_addr[miscellaneous_str[12]] = addr_ai + 78;
	m_mp_inhibit_addr[miscellaneous_str[12]] = 514;
	m_mp_delay_addr[miscellaneous_str[14]] = addr_ai + 79;
	m_mp_inhibit_addr[miscellaneous_str[14]] = 515;
	m_mp_delay_addr[miscellaneous_str[16]] = addr_ai + 80;
	m_mp_inhibit_addr[miscellaneous_str[16]] = 600;
	m_mp_delay_addr[miscellaneous_str[18]] = addr_ai + 81;
	m_mp_inhibit_addr[miscellaneous_str[18]] = 601;


	m_mp_delay_addr[miscellaneous_str[17]] = addr_ai + 152;
	m_mp_inhibit_addr[miscellaneous_str[17]] = 1008;
	m_mp_delay_addr[miscellaneous_str[19]] = addr_ai + 153;
	m_mp_inhibit_addr[miscellaneous_str[19]]= 1009;
	

	addr_ai = 8000;
	m_mp_delay_addr[miscellaneous_str[47]] = addr_ai;
	m_mp_inhibit_addr[miscellaneous_str[47]] = 0;	
	m_mp_delay_addr[miscellaneous_str[48]] = addr_ai + 1;
	m_mp_inhibit_addr[miscellaneous_str[48]] = 1;	
	m_mp_delay_addr[miscellaneous_str[49]] = addr_ai + 14;
	m_mp_inhibit_addr[miscellaneous_str[49]] = 14;	

	m_mp_delay_addr[miscellaneous_str[52]] = addr_ai + 11;
	m_mp_inhibit_addr[miscellaneous_str[52]] = 11;	
	m_mp_delay_addr[miscellaneous_str[36]] = addr_ai + 12;
	m_mp_inhibit_addr[miscellaneous_str[36]] = 12;	
	m_mp_delay_addr[miscellaneous_str[37]] = addr_ai + 13;
	m_mp_inhibit_addr[miscellaneous_str[37]] = 13;	





	/////////////模拟量
	//4-20mA
	m_mp_range_addr[miscellaneous_str[54]] = 9842;
	m_mp_threshold_addr[miscellaneous_str[54]] = 2871;
	m_mp_delay_addr[miscellaneous_str[54]] = 8322;
	m_mp_inhibit_addr[miscellaneous_str[54]] = 1914;
}




