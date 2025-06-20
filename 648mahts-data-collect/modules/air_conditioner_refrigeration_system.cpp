#include <iostream>
#include <unistd.h>

#include "sgo_sub_pub.h"
#include "sgo_func.h"
#include "sgo_ams/air_conditioner_refrigeration_system.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

using namespace std;

const char* air_conditioner_refrigeration_system_str[]={
#ifndef MAHTS648
	"no1_wheel_house_air_condition_common_alarm",
	"no2_wheel_house_air_condition_common_alarm",
	"no1_galley_package_air_condition_common_alarm",
	"no2_galley_package_air_condition_common_alarm",
	"m_s_b_room_package_air_condition_common_alarm",
	"no1_jacking_vfd_room_air_condition_common_alarm",
	"no2_jacking_vfd_room_air_condition_common_alarm",
	"deck_storeroom_air_condition_common_alarm",
	"paint_locker_room_air_condition_common_alarm",
	"workshop_air_condition_common_alarm",
	"a_z_v_f_d_room_air_condition_common_alarm",
	"no1_a_c_chilled_unit_common_alarm",
	"no2_a_c_chilled_unit_common_alarm",
	"no3_a_c_chilled_unit_common_alarm",
	"chiller_water_pump_system_common_alarm",
	"no1_ahu_common_alarm",
	"no2_ahu_common_alarm",
	"no3_ahu_common_alarm",
	"no4_ahu_common_alarm",
	"refrigeration_plant_common_alarm",
	"refrigerant_leaking_alarm",
	"device_offline",
#else
	"no1_wheel_house_air_condition_common_alarm",
"no2_wheel_house_air_condition_common_alarm",
"no1_galley_package_air_condition_common_alarm",
"no2_galley_package_air_condition_common_alarm",
"no1_m_s_b_room_package_air_condition_common_alarm",
"no2_m_s_b_room_package_air_condition_common_alarm",
"no3_m_s_b_room_package_air_condition_common_alarm",
"no4_m_s_b_room_package_air_condition_common_alarm",
"electrical_workshop_package_air_condition_common_alarm",
"mechanical_workshop_package_air_condition",
"fwd_thruster_vfd_room_package_air_condition",
"aft_thruster_vfd_room_package_air_condition_common_alarm",
"ecr_air_condition_common_alarm",
"no1_a_c_chilled_unit_common_alarm",
"no2_a_c_chilled_unit_common_alarm",
"chiller_water_pump_system_common_alarm",
"no1_ahu_common_alarm",
"no2_ahu_common_alarm",
"no3_ahu_common_alarm",
"refrigeration_plant_common_alarm",
"refrigerant_leaking_alarm",
"no1_a_c_chiller_compressor1_running",
"no1_a_c_chiller_compressor2_running",
"no1_a_c_chiller_antifreeze_alarm",
"no1_a_c_chiller_chilled_water_flow_alarm",
"no1_a_c_chiller_compressor1_high_pressure_alarm",
"no1_a_c_chiller_compressor1_over_temp",
"no1_a_c_chiller_compressor1_overload",
"no1_a_c_chiller_cooling_water1_flow_alarm",
"no1_a_c_chiller_compressor1_low_pressure_alarm",
"no1_a_c_chiller_compressor1_oil_level_alarm",
"no1_a_c_chiller_compressor2_high_pressure_alarm",
"no1_a_c_chiller_compressor2_over_temp",
"no1_a_c_chiller_compressor2_overload",
"no1_a_c_chiller_cooling_water2_flow_alarm",
"no1_a_c_chiller_compressor2_low_pressure_alarm",
"no1_a_c_chiller_compressor2_oil_level_alarm",
"no1_a_c_chiller_power_failure",
"no1_a_c_chiller_chilled_water_inlet_temp",
"no1_a_c_chiller_chilled_water_outlet_temp",
"no2a_c_chiller_compressor1_running",
"no2a_c_chiller_compressor2_running",
"no2a_c_chiller_antifreeze_alarm",
"no2a_c_chiller_chilled_water_flow_alarm",
"no2a_c_chiller_compressor1_high_pressure_alarm",
"no2a_c_chiller_compressor1_over_temp",
"no2a_c_chiller_compressor1_overload",
"no2a_c_chiller_cooling_water1_flow_alarm",
"no2a_c_chiller_compressor1_low_pressure_alarm",
"no2a_c_chiller_compressor1_oil_level_alarm",
"no2a_c_chiller_compressor2_high_pressure_alarm",
"no2a_c_chiller_compressor2_over_temp",
"no2a_c_chiller_compressor2_overload",
"no2a_c_chiller_cooling_water2_flow_alarm",
"no2a_c_chiller_compressor2_low_pressure_alarm",
"no2a_c_chiller_compressor2_oil_level_alarm",
"no2a_c_chiller_power_failure",
"no2a_c_chiller_chilled_water_inlet_temp",
"no2a_c_chiller_chilled_water_outlet_temp",
"refrigeration_plant_compressor1_high_pressure_alarm",
"refrigeration_plant_compressor1_overload_alarm",
"refrigeration_plant_compressor1_overheat_alarm",
"refrigeration_plant_compressor2_high_pressure_alarm",
"refrigeration_plant_compressor2_overload_alarm",
"refrigeration_plant_compressor2_overheat_alarm",
"refrigeration_plant_compressor1_water_pressure_low_alarm",
"refrigeration_plant_compressor2_water_pressure_low_alarm",
"refrigeration_plant_compressor1_running",
"refrigeration_plant_compressor2_running",
"freezer_room_overheat",
"chiller_room_overheat",
"dry_provision_store_room_overheat",
"freezer_room_temp",
"chiller_room_temp",
"dry_provision_store_room_temp",
"refrigeration_plant_compressor1_running_time",
"refrigeration_plant_compressor2_running_time",
"device_offline",

#endif
};

	
static const char* productCode = "air_conditioner_refrigeration_system";


AIR_CONDITIONER_REFIGERATION_SYSTEM::AIR_CONDITIONER_REFIGERATION_SYSTEM()
{}

AIR_CONDITIONER_REFIGERATION_SYSTEM::~AIR_CONDITIONER_REFIGERATION_SYSTEM()
{}

void AIR_CONDITIONER_REFIGERATION_SYSTEM::init(int devId,BlockQueue<air_conditioner_refrigeration_system> &bq,modbus *mb,mutex *mux)
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
void AIR_CONDITIONER_REFIGERATION_SYSTEM::connectionLostCallback(void* context, char* cause) 
{
	// 处理连接丢失事件
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	AIR_CONDITIONER_REFIGERATION_SYSTEM* client = static_cast<AIR_CONDITIONER_REFIGERATION_SYSTEM*>(ctx->manager);
	
	printf("--%s %d----address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());
	sgo_log_stderr(0,"--%s %d----address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());

	//std::lock_guard<std::mutex> lock(m_losts_mutex);
	client->m_v_lost[ctx->index] = 1;
	client->m_v_property_index_array.clear();

}

// 消息到达回调函数
int AIR_CONDITIONER_REFIGERATION_SYSTEM::messageArrivedCallback(void* context, char* topicName, int topicLen,MQTTClient_message* message) 
{
	// 处理接收到的消息
	CallbackContext* ctx = static_cast<CallbackContext*>(context); 
	AIR_CONDITIONER_REFIGERATION_SYSTEM* client = static_cast<AIR_CONDITIONER_REFIGERATION_SYSTEM*>(ctx->manager); // 获取类实例
	printf("--%s %d----address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());
	sgo_log_stderr(0,"--%s %d---address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());

	std::lock_guard<std::mutex> lock(client->m_mux_arrive);
    char* payloadptr = (char*)message->payload;
	//cout<<"payload:"<<payloadptr<<endl;

	if(0 == client->m_subs[0].compare(topicName))
	{
		if(!client->m_v_property_index_array.empty())
		{
			cout<<"another proxy has send."<<endl;
			goto exit;
		}
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
			client->m_property[air_conditioner_refrigeration_system_str[i]]=i;
		
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
void AIR_CONDITIONER_REFIGERATION_SYSTEM::deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt) 
{
	// 处理交付完成事件
	//std::cout << "Message delivery complete." << std::endl;
}

void AIR_CONDITIONER_REFIGERATION_SYSTEM::subClient()
{
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

void AIR_CONDITIONER_REFIGERATION_SYSTEM::pubClient(BlockQueue<air_conditioner_refrigeration_system>& bq)
{
	while(1)
	{
		MQTTClient_message pubmsg = MQTTClient_message_initializer;
		//声明消息token
		MQTTClient_deliveryToken token;

		while(!m_pubStop){
			air_conditioner_refrigeration_system one = bq.take();
			
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


void AIR_CONDITIONER_REFIGERATION_SYSTEM::start(BlockQueue<air_conditioner_refrigeration_system> &bq)
{
	m_sub = new thread(&AIR_CONDITIONER_REFIGERATION_SYSTEM::subClient,this);
	m_sub->detach();

	m_pub = new thread(&AIR_CONDITIONER_REFIGERATION_SYSTEM::pubClient,this,ref(bq));
	m_pub->detach();
	
	m_thr_reconn = new thread(&AIR_CONDITIONER_REFIGERATION_SYSTEM::reconn,this);
	m_thr_reconn->detach();

	m_thr_resend = new thread(&AIR_CONDITIONER_REFIGERATION_SYSTEM::resnd,this);
	m_thr_resend->detach();

	m_thr_chksensor = new thread(&AIR_CONDITIONER_REFIGERATION_SYSTEM::chksensor,this);
	m_thr_chksensor->detach();


	thread *thr_inhibit = new thread(&AIR_CONDITIONER_REFIGERATION_SYSTEM::thSetInhibit,this);
	thr_inhibit->detach();

	thread *thr_delay = new thread(&AIR_CONDITIONER_REFIGERATION_SYSTEM::thSetDelay,this);
	thr_delay->detach();	
}

void AIR_CONDITIONER_REFIGERATION_SYSTEM::chksensor()
{
	checkSensorStatus();
}

void AIR_CONDITIONER_REFIGERATION_SYSTEM::resnd()
{
	resend();
}

void AIR_CONDITIONER_REFIGERATION_SYSTEM::reconn()
{
	reconnect();
}


string AIR_CONDITIONER_REFIGERATION_SYSTEM::property_reply(char flag,const air_conditioner_refrigeration_system& one,const string& msg_id)
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
			case 0:writer.Int(one.no1_wheel_house_air_condition_common_alarm);break;
			case 1:writer.Int(one.no2_wheel_house_air_condition_common_alarm);break;
			case 2:writer.Int(one.no1_galley_package_air_condition_common_alarm);break;
			case 3:writer.Int(one.no2_galley_package_air_condition_common_alarm);break;
			case 4:writer.Int(one.m_s_b_room_package_air_condition_common_alarm);break;
			case 5:writer.Int(one.no1_jacking_vfd_room_air_condition_common_alarm);break;
			case 6:writer.Int(one.no2_jacking_vfd_room_air_condition_common_alarm);break;
			case 7:writer.Int(one.deck_storeroom_air_condition_common_alarm);break;
			case 8:writer.Int(one.paint_locker_room_air_condition_common_alarm);break;
			case 9:writer.Int(one.workshop_air_condition_common_alarm);break;
			case 10:writer.Int(one.a_z_v_f_d_room_air_condition_common_alarm);break;
			case 11:writer.Int(one.no1_a_c_chilled_unit_common_alarm);break;
			case 12:writer.Int(one.no2_a_c_chilled_unit_common_alarm);break;
			case 13:writer.Int(one.no3_a_c_chilled_unit_common_alarm);break;
			case 14:writer.Int(one.chiller_water_pump_system_common_alarm);break;
			case 15:writer.Int(one.no1_ahu_common_alarm);break;
			case 16:writer.Int(one.no2_ahu_common_alarm);break;
			case 17:writer.Int(one.no3_ahu_common_alarm);break;
			case 18:writer.Int(one.no4_ahu_common_alarm);break;
			case 19:writer.Int(one.refrigeration_plant_common_alarm);break;
			case 20:writer.Int(one.refrigerant_leaking_alarm);break;
			case 21:writer.Int(one.device_offline);break;
#else
           case 0:writer.Int(one.no1_wheel_house_air_condition_common_alarm);break;
           case 1:writer.Int(one.no2_wheel_house_air_condition_common_alarm);break;
           case 2:writer.Int(one.no1_galley_package_air_condition_common_alarm);break;
           case 3:writer.Int(one.no2_galley_package_air_condition_common_alarm);break;
           case 4:writer.Int(one.no1_msb_room_package_air_condition_common_alarm);break;
           case 5:writer.Int(one.no2_msb_room_package_air_condition_common_alarm);break;
           case 6:writer.Int(one.no3_msb_room_package_air_condition_common_alarm);break;
           case 7:writer.Int(one.no4_msb_room_package_air_condition_common_alarm);break;
           case 8:writer.Int(one.electric_workshop_package_air_condition_common_alarm);break;
           case 9:writer.Int(one.mechanical_workshop_package_air_condition_common_alarm);break;
           case 10:writer.Int(one.fwd_thruster_vfd_room_package_air_condition_common_alarm);break;
           case 11:writer.Int(one.aft_thruster_vfd_room_package_air_condition_common_alarm);break;
           case 12:writer.Int(one.ecr_air_condition_common_alarm);break;
           case 13:writer.Int(one.no1_a_c_chilled_unit_common_alarm);break;
           case 14:writer.Int(one.no2_a_c_chilled_unit_common_alarm);break;
           case 15:writer.Int(one.chiller_water_pump_system_common_alarm);break;
           case 16:writer.Int(one.no1_ahu_common_alarm);break;
           case 17:writer.Int(one.no2_ahu_common_alarm);break;
           case 18:writer.Int(one.no3_ahu_common_alarm);break;
           case 19:writer.Int(one.refrigeration_plant_common_alarm);break;
           case 20:writer.Int(one.refrigerant_leaking_alarm);break;
           case 21:writer.Int(one.no1_ac_chiller_compressor_1_running);break;
           case 22:writer.Int(one.no1_ac_chiller_compressor_2_running);break;
           case 23:writer.Int(one.no1_ac_chiller_antifreeze_alarm);break;
           case 24:writer.Int(one.no1_ac_chiller_chilled_water_flow_alarm);break;
           case 25:writer.Int(one.no1_ac_chiller_compressor_1_high_pressure_alarm);break;
           case 26:writer.Int(one.no1_ac_chiller_compressor_1_over_temp);break;
           case 27:writer.Int(one.no1_ac_chiller_compressor_1_overload);break;
           case 28:writer.Int(one.no1_ac_chiller_cooling_water_1_flow_alarm);break;
           case 29:writer.Int(one.no1_ac_chiller_compressor_1_low_pressure_alarm);break;
           case 30:writer.Int(one.no1_ac_chiller_compressor_1_oil_level_alarm);break;
           case 31:writer.Int(one.no1_ac_chiller_compressor_2_high_pressure_alarm);break;
           case 32:writer.Int(one.no1_ac_chiller_compressor_2_over_temp);break;
           case 33:writer.Int(one.no1_ac_chiller_compressor_2_overload);break;
           case 34:writer.Int(one.no1_ac_chiller_cooling_water_2_flow_alarm);break;
           case 35:writer.Int(one.no1_ac_chiller_compressor_2_low_pressure_alarm);break;
           case 36:writer.Int(one.no1_ac_chiller_compressor_2_oil_level_alarm);break;
           case 37:writer.Int(one.no1_ac_chiller_power_failure);break;
           case 38:writer.Double(one.no1_ac_chiller_chilled_water_inlet_temp);break;
           case 39:writer.Double(one.no1_ac_chiller_chilled_water_outlet_temp);break;
           case 40:writer.Int(one.no2_ac_chiller_compressor_1_running);break;
           case 41:writer.Int(one.no2_ac_chiller_compressor_2_running);break;
           case 42:writer.Int(one.no2_ac_chiller_antifreeze_alarm);break;
           case 43:writer.Int(one.no2_ac_chiller_chilled_water_flow_alarm);break;
           case 44:writer.Int(one.no2_ac_chiller_compressor_1_high_pressure_alarm);break;
           case 45:writer.Int(one.no2_ac_chiller_compressor_1_over_temp);break;
           case 46:writer.Int(one.no2_ac_chiller_compressor_1_overload);break;
           case 47:writer.Int(one.no2_ac_chiller_cooling_water_1_flow_alarm);break;
           case 48:writer.Int(one.no2_ac_chiller_compressor_1_low_pressure_alarm);break;
           case 49:writer.Int(one.no2_ac_chiller_compressor_1_oil_level_alarm);break;
           case 50:writer.Int(one.no2_ac_chiller_compressor_2_high_pressure_alarm);break;
           case 51:writer.Int(one.no2_ac_chiller_compressor_2_over_temp);break;
           case 52:writer.Int(one.no2_ac_chiller_compressor_2_overload);break;
           case 53:writer.Int(one.no2_ac_chiller_cooling_water_2_flow_alarm);break;
           case 54:writer.Int(one.no2_ac_chiller_compressor_2_low_pressure_alarm);break;
           case 55:writer.Int(one.no2_ac_chiller_compressor_2_oil_level_alarm);break;
           case 56:writer.Int(one.no2_ac_chiller_power_failure);break;
           case 57:writer.Double(one.no2_ac_chiller_chilled_water_inlet_temp);break;
           case 58:writer.Double(one.no2_ac_chiller_chilled_water_outlet_temp);break;
           case 59:writer.Int(one.refrigeration_plant_compressor_1_high_pressure_alarm);break;
           case 60:writer.Int(one.refrigeration_plant_compressor_1_overload_alarm);break;
           case 61:writer.Int(one.refrigeration_plant_compressor_1_overheat_alarm);break;
           case 62:writer.Int(one.refrigeration_plant_compressor_2_high_pressure_alarm);break;
           case 63:writer.Int(one.refrigeration_plant_compressor_2_overload_alarm);break;
           case 64:writer.Int(one.refrigeration_plant_compressor_2_overheat_alarm);break;
           case 65:writer.Int(one.refrigeration_plant_compressor_1_water_pressure_low_alarm);break;
           case 66:writer.Int(one.refrigeration_plant_compressor_2_water_pressure_low_alarm);break;
           case 67:writer.Int(one.refrigeration_plant_compressor_1_running);break;
           case 68:writer.Int(one.refrigeration_plant_compressor_2_running);break;
           case 69:writer.Int(one.freezer_room_overheat);break;
           case 70:writer.Int(one.chiller_room_overheat);break;
           case 71:writer.Int(one.dry_provision_store_room_overheat);break;
           case 72:writer.Double(one.freezer_room_temp);break;
           case 73:writer.Double(one.chiller_room_temp);break;
           case 74:writer.Double(one.dry_provision_store_room_temp);break;
           case 75:writer.Double(one.refrigeration_plant_compress_1_running_time);break;
           case 76:writer.Double(one.refrigeration_plant_compress_2_running_time);break;
           case 77:writer.Int(one.device_offline);break;
           
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
	writer.Key(air_conditioner_refrigeration_system_str[i++]);writer.Int(one.no1_wheel_house_air_condition_common_alarm);
	writer.Key(air_conditioner_refrigeration_system_str[i++]);writer.Int(one.no2_wheel_house_air_condition_common_alarm);
	writer.Key(air_conditioner_refrigeration_system_str[i++]);writer.Int(one.no1_galley_package_air_condition_common_alarm);
	writer.Key(air_conditioner_refrigeration_system_str[i++]);writer.Int(one.no2_galley_package_air_condition_common_alarm);
	writer.Key(air_conditioner_refrigeration_system_str[i++]);writer.Int(one.m_s_b_room_package_air_condition_common_alarm);
	writer.Key(air_conditioner_refrigeration_system_str[i++]);writer.Int(one.no1_jacking_vfd_room_air_condition_common_alarm);
	writer.Key(air_conditioner_refrigeration_system_str[i++]);writer.Int(one.no2_jacking_vfd_room_air_condition_common_alarm);
	writer.Key(air_conditioner_refrigeration_system_str[i++]);writer.Int(one.deck_storeroom_air_condition_common_alarm);
	writer.Key(air_conditioner_refrigeration_system_str[i++]);writer.Int(one.paint_locker_room_air_condition_common_alarm);
	writer.Key(air_conditioner_refrigeration_system_str[i++]);writer.Int(one.workshop_air_condition_common_alarm);
	writer.Key(air_conditioner_refrigeration_system_str[i++]);writer.Int(one.a_z_v_f_d_room_air_condition_common_alarm);
	writer.Key(air_conditioner_refrigeration_system_str[i++]);writer.Int(one.no1_a_c_chilled_unit_common_alarm);
	writer.Key(air_conditioner_refrigeration_system_str[i++]);writer.Int(one.no2_a_c_chilled_unit_common_alarm);
	writer.Key(air_conditioner_refrigeration_system_str[i++]);writer.Int(one.no3_a_c_chilled_unit_common_alarm);
	writer.Key(air_conditioner_refrigeration_system_str[i++]);writer.Int(one.chiller_water_pump_system_common_alarm);
	writer.Key(air_conditioner_refrigeration_system_str[i++]);writer.Int(one.no1_ahu_common_alarm);
	writer.Key(air_conditioner_refrigeration_system_str[i++]);writer.Int(one.no2_ahu_common_alarm);
	writer.Key(air_conditioner_refrigeration_system_str[i++]);writer.Int(one.no3_ahu_common_alarm);
	writer.Key(air_conditioner_refrigeration_system_str[i++]);writer.Int(one.no4_ahu_common_alarm);
	writer.Key(air_conditioner_refrigeration_system_str[i++]);writer.Int(one.refrigeration_plant_common_alarm);
	writer.Key(air_conditioner_refrigeration_system_str[i++]);writer.Int(one.refrigerant_leaking_alarm);

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



void AIR_CONDITIONER_REFIGERATION_SYSTEM::thSetDelay()
{
	setDelay();
}


void AIR_CONDITIONER_REFIGERATION_SYSTEM::thSetInhibit()
{
	while (1)
	{
		inhibit one = bq_inhibit.take();
		string key = one.m_keyname;
		bool inhib = one.is_inhibit;
        for(int i = 0; i < sizeof(air_conditioner_refrigeration_system_str); ++i)
	    {
            std::string s(air_conditioner_refrigeration_system_str[i]);
		    if(s == key)
		    {
                switch(i)
		    	{
                    case 0:{m_data_store.no1_wheel_house_air_condition_common_alarm.inhibit = inhib;break;}
                    case 1:{m_data_store.no2_wheel_house_air_condition_common_alarm.inhibit = inhib;break;}
                    case 2:{m_data_store.no1_galley_package_air_condition_common_alarm.inhibit = inhib;break;}
                    case 3:{m_data_store.no2_galley_package_air_condition_common_alarm.inhibit = inhib;break;}
                    case 4:{m_data_store.no1_msb_room_package_air_condition_common_alarm.inhibit = inhib;break;}
                    case 5:{m_data_store.no2_msb_room_package_air_condition_common_alarm.inhibit = inhib;break;}
                    case 6:{m_data_store.no3_msb_room_package_air_condition_common_alarm.inhibit = inhib;break;}
                    case 7:{m_data_store.no4_msb_room_package_air_condition_common_alarm.inhibit = inhib;break;}
                    case 8:{m_data_store.electric_workshop_package_air_condition_common_alarm.inhibit = inhib;break;}
                    case 9:{m_data_store.mechanical_workshop_package_air_condition_common_alarm.inhibit = inhib;break;}
                    case 10:{m_data_store.fwd_thruster_vfd_room_package_air_condition_common_alarm.inhibit = inhib;break;}
                    case 11:{m_data_store.aft_thruster_vfd_room_package_air_condition_common_alarm.inhibit = inhib;break;}
                    case 12:{m_data_store.ecr_air_condition_common_alarm.inhibit = inhib;break;}
                    case 13:{m_data_store.no1_a_c_chilled_unit_common_alarm.inhibit = inhib;break;}
                    case 14:{m_data_store.no2_a_c_chilled_unit_common_alarm.inhibit = inhib;break;}
                    case 15:{m_data_store.chiller_water_pump_system_common_alarm.inhibit = inhib;break;}
                    case 16:{m_data_store.no1_ahu_common_alarm.inhibit = inhib;break;}
                    case 17:{m_data_store.no2_ahu_common_alarm.inhibit = inhib;break;}
                    case 18:{m_data_store.no3_ahu_common_alarm.inhibit = inhib;break;}
                    case 19:{m_data_store.refrigeration_plant_common_alarm.inhibit = inhib;break;}
                    case 20:{m_data_store.refrigerant_leaking_alarm.inhibit = inhib;break;}
                    case 21:{m_data_store.no1_ac_chiller_compressor_1_running.inhibit = inhib;break;}
                    case 22:{m_data_store.no1_ac_chiller_compressor_2_running.inhibit = inhib;break;}
                    case 23:{m_data_store.no1_ac_chiller_antifreeze_alarm.inhibit = inhib;break;}
                    case 24:{m_data_store.no1_ac_chiller_chilled_water_flow_alarm.inhibit = inhib;break;}
                    case 25:{m_data_store.no1_ac_chiller_compressor_1_high_pressure_alarm.inhibit = inhib;break;}
                    case 26:{m_data_store.no1_ac_chiller_compressor_1_over_temp.inhibit = inhib;break;}
                    case 27:{m_data_store.no1_ac_chiller_compressor_1_overload.inhibit = inhib;break;}
                    case 28:{m_data_store.no1_ac_chiller_cooling_water_1_flow_alarm.inhibit = inhib;break;}
                    case 29:{m_data_store.no1_ac_chiller_compressor_1_low_pressure_alarm.inhibit = inhib;break;}
                    case 30:{m_data_store.no1_ac_chiller_compressor_1_oil_level_alarm.inhibit = inhib;break;}
                    case 31:{m_data_store.no1_ac_chiller_compressor_2_high_pressure_alarm.inhibit = inhib;break;}
                    case 32:{m_data_store.no1_ac_chiller_compressor_2_over_temp.inhibit = inhib;break;}
                    case 33:{m_data_store.no1_ac_chiller_compressor_2_overload.inhibit = inhib;break;}
                    case 34:{m_data_store.no1_ac_chiller_cooling_water_2_flow_alarm.inhibit = inhib;break;}
                    case 35:{m_data_store.no1_ac_chiller_compressor_2_low_pressure_alarm.inhibit = inhib;break;}
                    case 36:{m_data_store.no1_ac_chiller_compressor_2_oil_level_alarm.inhibit = inhib;break;}
                    case 37:{m_data_store.no1_ac_chiller_power_failure.inhibit = inhib;break;}
                    case 38:{m_data_store.no1_ac_chiller_chilled_water_inlet_temp.inhibit = inhib;break;}
                    case 39:{m_data_store.no1_ac_chiller_chilled_water_outlet_temp.inhibit = inhib;break;}
                    case 40:{m_data_store.no2_ac_chiller_compressor_1_running.inhibit = inhib;break;}
                    case 41:{m_data_store.no2_ac_chiller_compressor_2_running.inhibit = inhib;break;}
                    case 42:{m_data_store.no2_ac_chiller_antifreeze_alarm.inhibit = inhib;break;}
                    case 43:{m_data_store.no2_ac_chiller_chilled_water_flow_alarm.inhibit = inhib;break;}
                    case 44:{m_data_store.no2_ac_chiller_compressor_1_high_pressure_alarm.inhibit = inhib;break;}
                    case 45:{m_data_store.no2_ac_chiller_compressor_1_over_temp.inhibit = inhib;break;}
                    case 46:{m_data_store.no2_ac_chiller_compressor_1_overload.inhibit = inhib;break;}
                    case 47:{m_data_store.no2_ac_chiller_cooling_water_1_flow_alarm.inhibit = inhib;break;}
                    case 48:{m_data_store.no2_ac_chiller_compressor_1_low_pressure_alarm.inhibit = inhib;break;}
                    case 49:{m_data_store.no2_ac_chiller_compressor_1_oil_level_alarm.inhibit = inhib;break;}
                    case 50:{m_data_store.no2_ac_chiller_compressor_2_high_pressure_alarm.inhibit = inhib;break;}
                    case 51:{m_data_store.no2_ac_chiller_compressor_2_over_temp.inhibit = inhib;break;}
                    case 52:{m_data_store.no2_ac_chiller_compressor_2_overload.inhibit = inhib;break;}
                    case 53:{m_data_store.no2_ac_chiller_cooling_water_2_flow_alarm.inhibit = inhib;break;}
                    case 54:{m_data_store.no2_ac_chiller_compressor_2_low_pressure_alarm.inhibit = inhib;break;}
                    case 55:{m_data_store.no2_ac_chiller_compressor_2_oil_level_alarm.inhibit = inhib;break;}
                    case 56:{m_data_store.no2_ac_chiller_power_failure.inhibit = inhib;break;}
                    case 57:{m_data_store.no2_ac_chiller_chilled_water_inlet_temp.inhibit = inhib;break;}
                    case 58:{m_data_store.no2_ac_chiller_chilled_water_outlet_temp.inhibit = inhib;break;}
                    case 59:{m_data_store.refrigeration_plant_compressor_1_high_pressure_alarm.inhibit = inhib;break;}
                    case 60:{m_data_store.refrigeration_plant_compressor_1_overload_alarm.inhibit = inhib;break;}
                    case 61:{m_data_store.refrigeration_plant_compressor_1_overheat_alarm.inhibit = inhib;break;}
                    case 62:{m_data_store.refrigeration_plant_compressor_2_high_pressure_alarm.inhibit = inhib;break;}
                    case 63:{m_data_store.refrigeration_plant_compressor_2_overload_alarm.inhibit = inhib;break;}
                    case 64:{m_data_store.refrigeration_plant_compressor_2_overheat_alarm.inhibit = inhib;break;}
                    case 65:{m_data_store.refrigeration_plant_compressor_1_water_pressure_low_alarm.inhibit = inhib;break;}
                    case 66:{m_data_store.refrigeration_plant_compressor_2_water_pressure_low_alarm.inhibit = inhib;break;}
                    case 67:{m_data_store.refrigeration_plant_compressor_1_running.inhibit = inhib;break;}
                    case 68:{m_data_store.refrigeration_plant_compressor_2_running.inhibit = inhib;break;}
                    case 69:{m_data_store.freezer_room_overheat.inhibit = inhib;break;}
                    case 70:{m_data_store.chiller_room_overheat.inhibit = inhib;break;}
                    case 71:{m_data_store.dry_provision_store_room_overheat.inhibit = inhib;break;}
                    case 72:{m_data_store.freezer_room_temp.inhibit = inhib;break;}
                    case 73:{m_data_store.chiller_room_temp.inhibit = inhib;break;}
                    case 74:{m_data_store.dry_provision_store_room_temp.inhibit = inhib;break;}
                    case 75:{m_data_store.refrigeration_plant_compress_1_running_time.inhibit = inhib;break;}
                    case 76:{m_data_store.refrigeration_plant_compress_2_running_time.inhibit = inhib;break;}
                    case 77:{m_data_store.device_offline.inhibit = inhib;break;}          
		    	}
    
		    	break;
		    }
	    }       
	}

	//setInhibit(5801,5);
}



void AIR_CONDITIONER_REFIGERATION_SYSTEM::init_mb_addr()
{
	int addr_ai = 8000;
	m_mp_delay_addr[air_conditioner_refrigeration_system_str[0]] = addr_ai + 34;
	m_mp_inhibit_addr[air_conditioner_refrigeration_system_str[0]] = 14;
	m_mp_delay_addr[air_conditioner_refrigeration_system_str[1]] = addr_ai + 35;
	m_mp_inhibit_addr[air_conditioner_refrigeration_system_str[1]] = 15;
	m_mp_delay_addr[air_conditioner_refrigeration_system_str[2]] = addr_ai + 38;
	m_mp_inhibit_addr[air_conditioner_refrigeration_system_str[2]] = 102;
	m_mp_delay_addr[air_conditioner_refrigeration_system_str[3]] = addr_ai + 39;
	m_mp_inhibit_addr[air_conditioner_refrigeration_system_str[3]] = 103;
	m_mp_delay_addr[air_conditioner_refrigeration_system_str[4]] = addr_ai + 30;
	m_mp_inhibit_addr[air_conditioner_refrigeration_system_str[4]] = 10;
	m_mp_delay_addr[air_conditioner_refrigeration_system_str[5]] = addr_ai + 31;
	m_mp_inhibit_addr[air_conditioner_refrigeration_system_str[5]] = 11;
	m_mp_delay_addr[air_conditioner_refrigeration_system_str[6]] = addr_ai + 32;
	m_mp_inhibit_addr[air_conditioner_refrigeration_system_str[6]] = 12;
	m_mp_delay_addr[air_conditioner_refrigeration_system_str[7]] = addr_ai + 33;
	m_mp_inhibit_addr[air_conditioner_refrigeration_system_str[7]] = 13;
	m_mp_delay_addr[air_conditioner_refrigeration_system_str[8]] = addr_ai + 36;
	m_mp_inhibit_addr[air_conditioner_refrigeration_system_str[8]] = 100;
	m_mp_delay_addr[air_conditioner_refrigeration_system_str[9]] = addr_ai + 37;
	m_mp_inhibit_addr[air_conditioner_refrigeration_system_str[9]] = 101;
	m_mp_delay_addr[air_conditioner_refrigeration_system_str[10]] = addr_ai + 40;
	m_mp_inhibit_addr[air_conditioner_refrigeration_system_str[10]] = 104;
	m_mp_delay_addr[air_conditioner_refrigeration_system_str[11]] = addr_ai + 42;
	m_mp_inhibit_addr[air_conditioner_refrigeration_system_str[11]] = 106;
	m_mp_delay_addr[air_conditioner_refrigeration_system_str[12]] = addr_ai + 43;
	m_mp_inhibit_addr[air_conditioner_refrigeration_system_str[12]] = 107;
	m_mp_delay_addr[air_conditioner_refrigeration_system_str[13]] = addr_ai + 44;
	m_mp_inhibit_addr[air_conditioner_refrigeration_system_str[13]] = 108;
	m_mp_delay_addr[air_conditioner_refrigeration_system_str[14]] = addr_ai + 41;
	m_mp_inhibit_addr[air_conditioner_refrigeration_system_str[14]] = 105;


	for(int i = 0; i < 4;i++)
	{
		m_mp_delay_addr[air_conditioner_refrigeration_system_str[15 + i]] = addr_ai + 64 + i;
		m_mp_inhibit_addr[air_conditioner_refrigeration_system_str[15 + i]] = 212 + i;
	}
	m_mp_delay_addr[air_conditioner_refrigeration_system_str[19]] = addr_ai + 92;
	m_mp_inhibit_addr[air_conditioner_refrigeration_system_str[19]] = 408;

	
	m_mp_delay_addr[air_conditioner_refrigeration_system_str[20]] = addr_ai + 25;
	m_mp_inhibit_addr[air_conditioner_refrigeration_system_str[20]] = 5;
	
}









