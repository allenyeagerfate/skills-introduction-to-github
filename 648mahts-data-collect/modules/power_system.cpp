#include <iostream>
#include <unistd.h>

#include "sgo_sub_pub.h"
#include "sgo_func.h"
#include "sgo_ams/power_system.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

using namespace std;

const char* power_system_str[]={
#ifndef MAHTS648
	"eg_running",
	"eg_cb_close",
	"esb_dc24v_power_failure",
	"eg_breaker_trip",
	"eg_over_current",
	"esb_common_alarm",
	"esb_ac440v_bus_low_insulation",
	"esb_ac230v_bus_low_insulation",
	"esb_auto_mode",
	"no1_main_transformer_breaker_trip",
	"no2_main_transformer_breaker_trip",
	"no1_main_crane_cb_close",
	"no2_main_crane_cb_close",
	"no1_jacking_system_phase_transformer_cb_close",
	"no2_jacking_system_phase_transformer_cb_close",
	"no3_jacking_system_phase_transformer_cb_close",
	"no4_jacking_system_phase_transformer_cb_close",
	"no1_bow_thruster_cb_close",
	"no2_bow_thruster_cb_close",
	"no1_main_propulsion_vfd_cb_close",
	"no2_main_propulsion_vfd_cb_close",
	"dg1_ppm_fault",
	"dg2_ppm_fault",
	"dg3_ppm_fault",
	"dg4_ppm_fault",
	"dg5_ppm_fault",
	"dg6_ppm_fault",
	"msb_ac690v_bus_a_low_insulation",
	"msb_ac690v_bus_b_low_insulation",
	"no1_main_transformer_cb_close",
	"no2_main_transformer_cb_close",
	"ac690v_msb_dc24v_power_failure",
	"msb_ac690v_bus_b_harmonic_over_limit",
	"msb_ac690v_bus_a_harmonic_over_limit",
	"msb_ac690v_shore_power_cb_closed",
	"no1_main_transformer_secondary_breaker_input_power_on",
	"no1_main_transformer_breaker_auto_mode",
	"no1_main_transformer_secondary_breaker_close",
	"no1_main_transformer_secondary_breaker_trip",
	"msb_ac440v_bus_b_blackout",
	"no2_main_transformer_secondary_breaker_input_power_on",
	"no2_main_transformer_breaker_auto_mode",
	"no2_main_transformer_secondary_breaker_close",
	"no2_main_transformer_secondary_breaker_trip",
	"ac440v_msbbustie_acb_close",
	"msb_ac440v_bus_a_blackout",
	"ac440v_msb_dc24v_power_failure",
	"pt_power_failure",
	"ac440v_msb_bus_c_low_insulation",
	"ac440v_msb_bus_d_low_insulation",
	"ac230v_msb_bus_low_insulation",
	"preferential_trip_alarm",
	"e_stop_shutdown_power_failure",
	"ac690v_msb_bus_tie_cb_closed",
	"ac440v_msb_to_esb_cb_colse_statues",
	"ac440v_shore_power_cb_closed",
	"emergency_shutdown_wire_break_es1",
	"emergency_shutdown_wire_break_es21",
	"emergency_shutdown_wire_break_es22",
	"emergency_shutdown_wire_break_es23",
	"emergency_shutdown_wire_break_es24",
	"emergency_shutdown_wire_break_es25",
	"emergency_shutdown_wire_break_es26",
	"emergency_shutdown_es1_actived",
	"emergency_shutdown_es21_actived",
	"emergency_shutdown_es22_actived",
	"emergency_shutdown_es23_actived",
	"emergency_shutdown_es24_actived",
	"emergency_shutdown_es25_actived",
	"emergency_shutdown_es26_actived",
	"dg1_acb_close_fail",
	"dg1_acb_open_fail",
	"dg1_ready_to_start",
	"dg1_cb_open",
	"dg1_cb_close",
	"dg1_auto_mode",
	"dg1_semi_auto_mode",
	"dg1_on_first_priority",
	"dg2_acb_close_fail",
	"dg2_acb_open_fail",
	"dg2_ready_to_start",
	"dg2_cb_open",
	"dg2_cb_close",
	"dg2_auto_mode",
	"dg2_semi_auto_mode",
	"dg2_on_first_priority",
	"dg3_acb_close_fail",
	"dg3_acb_open_fail",
	"dg3_ready_to_start",
	"dg3_cb_open",
	"dg3_cb_close",
	"dg3_auto_mode",
	"dg3_semi_auto_mode",
	"dg3_on_first_priority",
	"dg4_acb_close_fail",
	"dg4_acb_open_fail",
	"dg4_ready_to_start",
	"dg4_cb_open",
	"dg4_cb_close",
	"dg4_auto_mode",
	"dg4_semi_auto_mode",
	"dg4_on_first_priority",
	"dg5_acb_close_fail",
	"dg5_acb_open_fail",
	"dg5_ready_to_start",
	"dg5_cb_open",
	"dg5_cb_close",
	"dg5_auto_mode",
	"dg5_semi_auto_mode",
	"dg5_on_first_priority",
	"dg6_acb_close_fail",
	"dg6_acb_open_fail",
	"dg6_ready_to_start",
	"dg6_cb_open",
	"dg6_cb_close",
	"dg6_auto_mode",
	"dg6_semi_auto_mode",
	"dg6_on_first_priority",
	"dg1_supply_voltage_high",
	"dg1_supply_voltage_low",
	"dg1_supply_frequency_high",
	"dg1_supply_frequency_low",
	"dg1_acb_abnormal_trip",
	"dg1_reverse_power",
	"dg1_ppm_alarms_on",
	"ac690v_msb_bus_bar_voltage_high",
	"ac690v_msb_bus_bar_voltage_low",
	"ac690v_msb_bus_bar_frequency_high",
	"ac690v_msb_bus_bar_frequency_low",
	"dg1_auto_synchro_failure",
	"dg1_voltage_l1_l2",
	"dg1_voltage_l2_l3",
	"dg1_voltage_l3_l1",
	"dg1_frequency_l1",
	"dg1_frequency_l2",
	"dg1_frequency_l3",
	"dg1_current_l1",
	"dg1_current_l2",
	"dg1_current_l3",
	"dg1_active_power",
	"dg1_reactive_power",
	"dg1_apparent_power_s",
	"dg1_power_factor",
	"dg1_available_power",
	"ac690v_msb_busbar_voltage_l1_l2",
	"ac690v_msb_busbar_voltage_l2_l3",
	"ac690v_msb_busbar_voltage_l3_l1",
	"ac690v_msb_busbar_frequency_l1",
	"ac690v_msb_busbar_frequency_l2",
	"ac690v_msb_busbar_frequency_l3",
	"dg2_supply_voltage_high",
	"dg2_supply_voltage_low",
	"dg2_supply_frequency_high",
	"dg2_supply_frequency_low",
	"dg2_acb_abnormal_trip",
	"dg2_reverse_power",
	"dg2_ppm_alarms_on",
	"dg2_auto_synchro_failure",
	"dg2_voltage_l1_l2",
	"dg2_voltage_l2_l3",
	"dg2_voltage_l3_l1",
	"dg2_frequency_l1",
	"dg2_frequency_l2",
	"dg2_frequency_l3",
	"dg2_current_l1",
	"dg2_current_l2",
	"dg2_current_l3",
	"dg2_active_power",
	"dg2_reactive_power",
	"dg2_apparent_power_s",
	"dg2_power_factor",
	"dg2_available_power",
	"dg3_supply_voltage_high",
	"dg3_supply_voltage_low",
	"dg3_supply_frequency_high",
	"dg3_supply_frequency_low",
	"dg3_acb_abnormal_trip",
	"dg3_reverse_power",
	"dg3_ppm_alarms_on",
	"dg3_auto_synchro_failure",
	"dg3_voltage_l1_l2",
	"dg3_voltage_l2_l3",
	"dg3_voltage_l3_l1",
	"dg3_frequency_l1",
	"dg3_frequency_l2",
	"dg3_frequency_l3",
	"dg3_current_l1",
	"dg3_current_l2",
	"dg3_current_l3",
	"dg3_active_power",
	"dg3_reactive_power",
	"dg3_apparent_power_s",
	"dg3_power_factor",
	"dg3_available_power",
	"dg4_supply_voltage_high",
	"dg4_supply_voltage_low",
	"dg4_supply_frequency_high",
	"dg4_supply_frequency_low",
	"dg4_acb_abnormal_trip",
	"dg4_reverse_power",
	"dg4_ppm_alarms_on",
	"dg4_auto_synchro_failure",
	"dg4_voltage_l1_l2",
	"dg4_voltage_l2_l3",
	"dg4_voltage_l3_l1",
	"dg4_frequency_l1",
	"dg4_frequency_l2",
	"dg4_frequency_l3",
	"dg4_current_l1",
	"dg4_current_l2",
	"dg4_current_l3",
	"dg4_active_power",
	"dg4_reactive_power",
	"dg4_apparent_power_s",
	"dg4_power_factor",
	"dg4_available_power",
	"dg5_supply_voltage_high",
	"dg5_supply_voltage_low",
	"dg5_supply_frequency_high",
	"dg5_supply_frequency_low",
	"dg5_acb_abnormal_trip",
	"dg5_reverse_power",
	"dg5_ppm_alarms_on",
	"dg5_auto_synchro_failure",
	"dg5_voltage_l1_l2",
	"dg5_voltage_l2_l3",
	"dg5_voltage_l3_l1",
	"dg5_frequency_l1",
	"dg5_frequency_l2",
	"dg5_frequency_l3",
	"dg5_current_l1",
	"dg5_current_l2",
	"dg5_current_l3",
	"dg5_active_power",
	"dg5_reactive_power",
	"dg5_apparent_power_s",
	"dg5_power_factor",
	"dg5_available_power",
	"dg6_supply_voltage_high",
	"dg6_supply_voltage_low",
	"dg6_supply_frequency_high",
	"dg6_supply_frequency_low",
	"dg6_acb_abnormal_trip",
	"dg6_reverse_power",
	"dg6_ppm_alarms_on",
	"dg6_auto_synchro_failure",
	"dg6_voltage_l1_l2",
	"dg6_voltage_l2_l3",
	"dg6_voltage_l3_l1",
	"dg6_frequency_l1",
	"dg6_frequency_l2",
	"dg6_frequency_l3",
	"dg6_current_l1",
	"dg6_current_l2",
	"dg6_current_l3",
	"dg6_active_power",
	"dg6_reactive_power",
	"dg6_apparent_power_s",
	"dg6_power_factor",
	"dg6_available_power",
	"no1_jetting_pump_request_failure",
	"no2_jetting_pump_request_failure",
	"no1_bow_thruster_request_failure",
	"no2_bow_thruster_request_failure",
	"no1_main_crane_request_failure",
	"no2_main_crane_request_failure",
	"no1_leg_control_unit_request_failure",
	"no2_leg_control_unit_request_failure",
	"no3_leg_control_unit_request_failure",
	"no4_leg_control_unit_request_failure",
	"no1_submersible_pump_request_failure",
	"no2_submersible_pump_request_failure",
	"device_offline",
#else
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

#endif
};
	
static const char* productCode = "power_system1";


POWER_SYSTEM::POWER_SYSTEM()
{}
 
POWER_SYSTEM::~POWER_SYSTEM()
{}

void POWER_SYSTEM::init(int devId,BlockQueue<power_system> &bq,modbus *mb,mutex *mux)
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
void POWER_SYSTEM::connectionLostCallback(void* context, char* cause) 
{
	// 处理连接丢失事件
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	POWER_SYSTEM* client = static_cast<POWER_SYSTEM*>(ctx->manager);
	
	printf("--%s %d--address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());
	sgo_log_stderr(0,"--%s %d----address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());

	//std::lock_guard<std::mutex> lock(m_losts_mutex);
	client->m_v_lost[ctx->index] = 1;
	client->m_v_property_index_array.clear();

}


// 消息到达回调函数
int POWER_SYSTEM::messageArrivedCallback(void* context, char* topicName, int topicLen,MQTTClient_message* message) 
{
	// 处理接收到的消息
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	POWER_SYSTEM* client = static_cast<POWER_SYSTEM*>(ctx->manager);
	
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
			client->m_property[power_system_str[i]]=i;
		
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
void POWER_SYSTEM::deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt) 
{
	// 处理交付完成事件
	//std::cout << "Message delivery complete." << std::endl;
}

void POWER_SYSTEM::subClient()
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


void POWER_SYSTEM::pubClient(BlockQueue<power_system>& bq)
{
	#if 1 
	while(1)
	{
		MQTTClient_message pubmsg = MQTTClient_message_initializer;
		//声明消息token
		MQTTClient_deliveryToken token;

		while(!m_pubStop){
			power_system one = bq.take();
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
	#endif
}




void POWER_SYSTEM::start(BlockQueue<power_system> &bq)
{
	m_sub = new thread(&POWER_SYSTEM::subClient,this);
	m_sub->detach();

	m_pub = new thread(&POWER_SYSTEM::pubClient,this,ref(bq));
	m_pub->detach();

	m_thr_reconn = new thread(&POWER_SYSTEM::reconn,this);
	m_thr_reconn->detach();

	m_thr_resend = new thread(&POWER_SYSTEM::resnd,this);
	m_thr_resend->detach();

	m_thr_chksensor = new thread(&POWER_SYSTEM::chksensor,this);
	m_thr_chksensor->detach();

//	thread *thr_range = new thread(&POWER_SYSTEM::thSetRange,this);
//	thr_range->detach();

	thread *thr_threshold = new thread(&POWER_SYSTEM::thSetThreshold,this);
	thr_threshold->detach();

	thread *thr_inhibit = new thread(&POWER_SYSTEM::thSetInhibit,this);
	thr_inhibit->detach();

	thread *thr_delay = new thread(&POWER_SYSTEM::thSetDelay,this);
	thr_delay->detach();

	
}

void POWER_SYSTEM::chksensor()
{
	checkSensorStatus();
}

void POWER_SYSTEM::resnd()
{
	resend();
}

void POWER_SYSTEM::reconn()
{
	reconnect();
}



string POWER_SYSTEM::property_reply(char flag,const power_system& one,const string& msg_id)
{
	if(0 == m_property_size)
		return string();

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

	writer.Key("compressData");
	writer.StartArray();

	for(int i = 0; i < m_v_index.size();++i)
	{

		switch(m_v_index[i]){
#ifndef MAHTS648
		case 0:writer.Int(one.eg_running);break;
		case 1:writer.Int(one.eg_cb_close);break;
		case 2:writer.Int(one.esb_dc24v_power_failure);break;
		case 3:writer.Int(one.eg_breaker_trip);break;
		case 4:writer.Int(one.eg_over_current);break;
		case 5:writer.Int(one.esb_common_alarm);break;
		case 6:writer.Int(one.esb_ac440v_bus_low_insulation);break;
		case 7:writer.Int(one.esb_ac230v_bus_low_insulation);break;
		case 8:writer.Int(one.esb_auto_mode);break;
		case 9:writer.Int(one.no1_main_transformer_breaker_trip);break;
		case 10:writer.Int(one.no2_main_transformer_breaker_trip);break;
		case 11:writer.Int(one.no1_main_crane_cb_close);break;
		case 12:writer.Int(one.no2_main_crane_cb_close);break;
		case 13:writer.Int(one.no1_jacking_system_phase_transformer_cb_close);break;
		case 14:writer.Int(one.no2_jacking_system_phase_transformer_cb_close);break;
		case 15:writer.Int(one.no3_jacking_system_phase_transformer_cb_close);break;
		case 16:writer.Int(one.no4_jacking_system_phase_transformer_cb_close);break;
		case 17:writer.Int(one.no1_bow_thruster_cb_close);break;
		case 18:writer.Int(one.no2_bow_thruster_cb_close);break;
		case 19:writer.Int(one.no1_main_propulsion_vfd_cb_close);break;
		case 20:writer.Int(one.no2_main_propulsion_vfd_cb_close);break;
		case 21:writer.Int(one.dg1_ppm_fault);break;
		case 22:writer.Int(one.dg2_ppm_fault);break;
		case 23:writer.Int(one.dg3_ppm_fault);break;
		case 24:writer.Int(one.dg4_ppm_fault);break;
		case 25:writer.Int(one.dg5_ppm_fault);break;
		case 26:writer.Int(one.dg6_ppm_fault);break;
		case 27:writer.Int(one.msb_ac690v_bus_a_low_insulation);break;
		case 28:writer.Int(one.msb_ac690v_bus_b_low_insulation);break;
		case 29:writer.Int(one.no1_main_transformer_cb_close);break;
		case 30:writer.Int(one.no2_main_transformer_cb_close);break;
		case 31:writer.Int(one.ac690v_msb_dc24v_power_failure);break;
		case 32:writer.Int(one.msb_ac690v_bus_b_harmonic_over_limit);break;
		case 33:writer.Int(one.msb_ac690v_bus_a_harmonic_over_limit);break;
		case 34:writer.Int(one.msb_ac690v_shore_power_cb_closed);break;
		case 35:writer.Int(one.no1_main_transformer_secondary_breaker_input_power_on);break;
		case 36:writer.Int(one.no1_main_transformer_breaker_auto_mode);break;
		case 37:writer.Int(one.no1_main_transformer_secondary_breaker_close);break;
		case 38:writer.Int(one.no1_main_transformer_secondary_breaker_trip);break;
		case 39:writer.Int(one.msb_ac440v_bus_b_blackout);break;
		case 40:writer.Int(one.no2_main_transformer_secondary_breaker_input_power_on);break;
		case 41:writer.Int(one.no2_main_transformer_breaker_auto_mode);break;
		case 42:writer.Int(one.no2_main_transformer_secondary_breaker_close);break;
		case 43:writer.Int(one.no2_main_transformer_secondary_breaker_trip);break;
		case 44:writer.Int(one.ac440v_msbbustie_acb_close);break;
		case 45:writer.Int(one.msb_ac440v_bus_a_blackout);break;
		case 46:writer.Int(one.ac440v_msb_dc24v_power_failure);break;
		case 47:writer.Int(one.pt_power_failure);break;
		case 48:writer.Int(one.ac440v_msb_bus_c_low_insulation);break;
		case 49:writer.Int(one.ac440v_msb_bus_d_low_insulation);break;
		case 50:writer.Int(one.ac230v_msb_bus_low_insulation);break;
		case 51:writer.Int(one.preferential_trip_alarm);break;
		case 52:writer.Int(one.e_stop_shutdown_power_failure);break;
		case 53:writer.Int(one.ac690v_msb_bus_tie_cb_closed);break;
		case 54:writer.Int(one.ac440v_msb_to_esb_cb_colse_statues);break;
		case 55:writer.Int(one.ac440v_shore_power_cb_closed);break;
		case 56:writer.Int(one.emergency_shutdown_wire_break_es1);break;
		case 57:writer.Int(one.emergency_shutdown_wire_break_es21);break;
		case 58:writer.Int(one.emergency_shutdown_wire_break_es22);break;
		case 59:writer.Int(one.emergency_shutdown_wire_break_es23);break;
		case 60:writer.Int(one.emergency_shutdown_wire_break_es24);break;
		case 61:writer.Int(one.emergency_shutdown_wire_break_es25);break;
		case 62:writer.Int(one.emergency_shutdown_wire_break_es26);break;
		case 63:writer.Int(one.emergency_shutdown_es1_actived);break;
		case 64:writer.Int(one.emergency_shutdown_es21_actived);break;
		case 65:writer.Int(one.emergency_shutdown_es22_actived);break;
		case 66:writer.Int(one.emergency_shutdown_es23_actived);break;
		case 67:writer.Int(one.emergency_shutdown_es24_actived);break;
		case 68:writer.Int(one.emergency_shutdown_es25_actived);break;
		case 69:writer.Int(one.emergency_shutdown_es26_actived);break;
		case 70:writer.Int(one.dg1_acb_close_fail);break;
		case 71:writer.Int(one.dg1_acb_open_fail);break;
		case 72:writer.Int(one.dg1_ready_to_start);break;
		case 73:writer.Int(one.dg1_cb_open);break;
		case 74:writer.Int(one.dg1_cb_close);break;
		case 75:writer.Int(one.dg1_auto_mode);break;
		case 76:writer.Int(one.dg1_semi_auto_mode);break;
		case 77:writer.Int(one.dg1_on_first_priority);break;
		case 78:writer.Int(one.dg2_acb_close_fail);break;
		case 79:writer.Int(one.dg2_acb_open_fail);break;
		case 80:writer.Int(one.dg2_ready_to_start);break;
		case 81:writer.Int(one.dg2_cb_open);break;
		case 82:writer.Int(one.dg2_cb_close);break;
		case 83:writer.Int(one.dg2_auto_mode);break;
		case 84:writer.Int(one.dg2_semi_auto_mode);break;
		case 85:writer.Int(one.dg2_on_first_priority);break;
		case 86:writer.Int(one.dg3_acb_close_fail);break;
		case 87:writer.Int(one.dg3_acb_open_fail);break;
		case 88:writer.Int(one.dg3_ready_to_start);break;
		case 89:writer.Int(one.dg3_cb_open);break;
		case 90:writer.Int(one.dg3_cb_close);break;
		case 91:writer.Int(one.dg3_auto_mode);break;
		case 92:writer.Int(one.dg3_semi_auto_mode);break;
		case 93:writer.Int(one.dg3_on_first_priority);break;
		case 94:writer.Int(one.dg4_acb_close_fail);break;
		case 95:writer.Int(one.dg4_acb_open_fail);break;
		case 96:writer.Int(one.dg4_ready_to_start);break;
		case 97:writer.Int(one.dg4_cb_open);break;
		case 98:writer.Int(one.dg4_cb_close);break;
		case 99:writer.Int(one.dg4_auto_mode);break;
		case 100:writer.Int(one.dg4_semi_auto_mode);break;
		case 101:writer.Int(one.dg4_on_first_priority);break;
		case 102:writer.Int(one.dg5_acb_close_fail);break;
		case 103:writer.Int(one.dg5_acb_open_fail);break;
		case 104:writer.Int(one.dg5_ready_to_start);break;
		case 105:writer.Int(one.dg5_cb_open);break;
		case 106:writer.Int(one.dg5_cb_close);break;
		case 107:writer.Int(one.dg5_auto_mode);break;
		case 108:writer.Int(one.dg5_semi_auto_mode);break;
		case 109:writer.Int(one.dg5_on_first_priority);break;
		case 110:writer.Int(one.dg6_acb_close_fail);break;
		case 111:writer.Int(one.dg6_acb_open_fail);break;
		case 112:writer.Int(one.dg6_ready_to_start);break;
		case 113:writer.Int(one.dg6_cb_open);break;
		case 114:writer.Int(one.dg6_cb_close);break;
		case 115:writer.Int(one.dg6_auto_mode);break;
		case 116:writer.Int(one.dg6_semi_auto_mode);break;
		case 117:writer.Int(one.dg6_on_first_priority);break;
		case 118:writer.Int(one.dg1_supply_voltage_high);break;
		case 119:writer.Int(one.dg1_supply_voltage_low);break;
		case 120:writer.Int(one.dg1_supply_frequency_high);break;
		case 121:writer.Int(one.dg1_supply_frequency_low);break;
		case 122:writer.Int(one.dg1_acb_abnormal_trip);break;
		case 123:writer.Int(one.dg1_reverse_power);break;
		case 124:writer.Int(one.dg1_ppm_alarms_on);break;
		case 125:writer.Int(one.ac690v_msb_bus_bar_voltage_high);break;
		case 126:writer.Int(one.ac690v_msb_bus_bar_voltage_low);break;
		case 127:writer.Int(one.ac690v_msb_bus_bar_frequency_high);break;
		case 128:writer.Int(one.ac690v_msb_bus_bar_frequency_low);break;
		case 129:writer.Int(one.dg1_auto_synchro_failure);break;
		case 130:writer.Double(one.dg1_voltage_l1_l2);break;
		case 131:writer.Double(one.dg1_voltage_l2_l3);break;
		case 132:writer.Double(one.dg1_voltage_l3_l1);break;
		case 133:writer.Double(one.dg1_frequency_l1);break;
		case 134:writer.Double(one.dg1_frequency_l2);break;
		case 135:writer.Double(one.dg1_frequency_l3);break;
		case 136:writer.Double(one.dg1_current_l1);break;
		case 137:writer.Double(one.dg1_current_l2);break;
		case 138:writer.Double(one.dg1_current_l3);break;
		case 139:writer.Double(one.dg1_active_power);break;
		case 140:writer.Double(one.dg1_reactive_power);break;
		case 141:writer.Double(one.dg1_apparent_power_s);break;
		case 142:writer.Double(one.dg1_power_factor);break;
		case 143:writer.Double(one.dg1_available_power);break;
		case 144:writer.Double(one.ac690v_msb_busbar_voltage_l1_l2);break;
		case 145:writer.Double(one.ac690v_msb_busbar_voltage_l2_l3);break;
		case 146:writer.Double(one.ac690v_msb_busbar_voltage_l3_l1);break;
		case 147:writer.Double(one.ac690v_msb_busbar_frequency_l1);break;
		case 148:writer.Double(one.ac690v_msb_busbar_frequency_l2);break;
		case 149:writer.Double(one.ac690v_msb_busbar_frequency_l3);break;
		case 150:writer.Int(one.dg2_supply_voltage_high);break;
		case 151:writer.Int(one.dg2_supply_voltage_low);break;
		case 152:writer.Int(one.dg2_supply_frequency_high);break;
		case 153:writer.Int(one.dg2_supply_frequency_low);break;
		case 154:writer.Int(one.dg2_acb_abnormal_trip);break;
		case 155:writer.Int(one.dg2_reverse_power);break;
		case 156:writer.Int(one.dg2_ppm_alarms_on);break;
		case 157:writer.Int(one.dg2_auto_synchro_failure);break;
		case 158:writer.Double(one.dg2_voltage_l1_l2);break;
		case 159:writer.Double(one.dg2_voltage_l2_l3);break;
		case 160:writer.Double(one.dg2_voltage_l3_l1);break;
		case 161:writer.Double(one.dg2_frequency_l1);break;
		case 162:writer.Double(one.dg2_frequency_l2);break;
		case 163:writer.Double(one.dg2_frequency_l3);break;
		case 164:writer.Double(one.dg2_current_l1);break;
		case 165:writer.Double(one.dg2_current_l2);break;
		case 166:writer.Double(one.dg2_current_l3);break;
		case 167:writer.Double(one.dg2_active_power);break;
		case 168:writer.Double(one.dg2_reactive_power);break;
		case 169:writer.Double(one.dg2_apparent_power_s);break;
		case 170:writer.Double(one.dg2_power_factor);break;
		case 171:writer.Double(one.dg2_available_power);break;
		case 172:writer.Int(one.dg3_supply_voltage_high);break;
		case 173:writer.Int(one.dg3_supply_voltage_low);break;
		case 174:writer.Int(one.dg3_supply_frequency_high);break;
		case 175:writer.Int(one.dg3_supply_frequency_low);break;
		case 176:writer.Int(one.dg3_acb_abnormal_trip);break;
		case 177:writer.Int(one.dg3_reverse_power);break;
		case 178:writer.Int(one.dg3_ppm_alarms_on);break;
		case 179:writer.Int(one.dg3_auto_synchro_failure);break;
		case 180:writer.Double(one.dg3_voltage_l1_l2);break;
		case 181:writer.Double(one.dg3_voltage_l2_l3);break;
		case 182:writer.Double(one.dg3_voltage_l3_l1);break;
		case 183:writer.Double(one.dg3_frequency_l1);break;
		case 184:writer.Double(one.dg3_frequency_l2);break;
		case 185:writer.Double(one.dg3_frequency_l3);break;
		case 186:writer.Double(one.dg3_current_l1);break;
		case 187:writer.Double(one.dg3_current_l2);break;
		case 188:writer.Double(one.dg3_current_l3);break;
		case 189:writer.Double(one.dg3_active_power);break;
		case 190:writer.Double(one.dg3_reactive_power);break;
		case 191:writer.Double(one.dg3_apparent_power_s);break;
		case 192:writer.Double(one.dg3_power_factor);break;
		case 193:writer.Double(one.dg3_available_power);break;
		case 194:writer.Int(one.dg4_supply_voltage_high);break;
		case 195:writer.Int(one.dg4_supply_voltage_low);break;
		case 196:writer.Int(one.dg4_supply_frequency_high);break;
		case 197:writer.Int(one.dg4_supply_frequency_low);break;
		case 198:writer.Int(one.dg4_acb_abnormal_trip);break;
		case 199:writer.Int(one.dg4_reverse_power);break;
		case 200:writer.Int(one.dg4_ppm_alarms_on);break;
		case 201:writer.Int(one.dg4_auto_synchro_failure);break;
		case 202:writer.Double(one.dg4_voltage_l1_l2);break;
		case 203:writer.Double(one.dg4_voltage_l2_l3);break;
		case 204:writer.Double(one.dg4_voltage_l3_l1);break;
		case 205:writer.Double(one.dg4_frequency_l1);break;
		case 206:writer.Double(one.dg4_frequency_l2);break;
		case 207:writer.Double(one.dg4_frequency_l3);break;
		case 208:writer.Double(one.dg4_current_l1);break;
		case 209:writer.Double(one.dg4_current_l2);break;
		case 210:writer.Double(one.dg4_current_l3);break;
		case 211:writer.Double(one.dg4_active_power);break;
		case 212:writer.Double(one.dg4_reactive_power);break;
		case 213:writer.Double(one.dg4_apparent_power_s);break;
		case 214:writer.Double(one.dg4_power_factor);break;
		case 215:writer.Double(one.dg4_available_power);break;
		case 216:writer.Int(one.dg5_supply_voltage_high);break;
		case 217:writer.Int(one.dg5_supply_voltage_low);break;
		case 218:writer.Int(one.dg5_supply_frequency_high);break;
		case 219:writer.Int(one.dg5_supply_frequency_low);break;
		case 220:writer.Int(one.dg5_acb_abnormal_trip);break;
		case 221:writer.Int(one.dg5_reverse_power);break;
		case 222:writer.Int(one.dg5_ppm_alarms_on);break;
		case 223:writer.Int(one.dg5_auto_synchro_failure);break;
		case 224:writer.Double(one.dg5_voltage_l1_l2);break;
		case 225:writer.Double(one.dg5_voltage_l2_l3);break;
		case 226:writer.Double(one.dg5_voltage_l3_l1);break;
		case 227:writer.Double(one.dg5_frequency_l1);break;
		case 228:writer.Double(one.dg5_frequency_l2);break;
		case 229:writer.Double(one.dg5_frequency_l3);break;
		case 230:writer.Double(one.dg5_current_l1);break;
		case 231:writer.Double(one.dg5_current_l2);break;
		case 232:writer.Double(one.dg5_current_l3);break;
		case 233:writer.Double(one.dg5_active_power);break;
		case 234:writer.Double(one.dg5_reactive_power);break;
		case 235:writer.Double(one.dg5_apparent_power_s);break;
		case 236:writer.Double(one.dg5_power_factor);break;
		case 237:writer.Double(one.dg5_available_power);break;
		case 238:writer.Int(one.dg6_supply_voltage_high);break;
		case 239:writer.Int(one.dg6_supply_voltage_low);break;
		case 240:writer.Int(one.dg6_supply_frequency_high);break;
		case 241:writer.Int(one.dg6_supply_frequency_low);break;
		case 242:writer.Int(one.dg6_acb_abnormal_trip);break;
		case 243:writer.Int(one.dg6_reverse_power);break;
		case 244:writer.Int(one.dg6_ppm_alarms_on);break;
		case 245:writer.Int(one.dg6_auto_synchro_failure);break;
		case 246:writer.Double(one.dg6_voltage_l1_l2);break;
		case 247:writer.Double(one.dg6_voltage_l2_l3);break;
		case 248:writer.Double(one.dg6_voltage_l3_l1);break;
		case 249:writer.Double(one.dg6_frequency_l1);break;
		case 250:writer.Double(one.dg6_frequency_l2);break;
		case 251:writer.Double(one.dg6_frequency_l3);break;
		case 252:writer.Double(one.dg6_current_l1);break;
		case 253:writer.Double(one.dg6_current_l2);break;
		case 254:writer.Double(one.dg6_current_l3);break;
		case 255:writer.Double(one.dg6_active_power);break;
		case 256:writer.Double(one.dg6_reactive_power);break;
		case 257:writer.Double(one.dg6_apparent_power_s);break;
		case 258:writer.Double(one.dg6_power_factor);break;
		case 259:writer.Double(one.dg6_available_power);break;
		case 260:writer.Int(one.no1_jetting_pump_request_failure);break;
		case 261:writer.Int(one.no2_jetting_pump_request_failure);break;
		case 262:writer.Int(one.no1_bow_thruster_request_failure);break;
		case 263:writer.Int(one.no2_bow_thruster_request_failure);break;
		case 264:writer.Int(one.no1_main_crane_request_failure);break;
		case 265:writer.Int(one.no2_main_crane_request_failure);break;
		case 266:writer.Int(one.no1_leg_control_unit_request_failure);break;
		case 267:writer.Int(one.no2_leg_control_unit_request_failure);break;
		case 268:writer.Int(one.no3_leg_control_unit_request_failure);break;
		case 269:writer.Int(one.no4_leg_control_unit_request_failure);break;
		case 270:writer.Int(one.no1_submersible_pump_request_failure);break;
		case 271:writer.Int(one.no2_submersible_pump_request_failure);break;	
		case 272:writer.Int(one.device_offline);break;
#else
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
#endif

		default:break;
		}
	}

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

void POWER_SYSTEM::init_mb_addr()
{
	int addr_ai = 7800;
	for(int i = 0; i < 70; i++)
		m_mp_delay_addr[power_system_str[i]] = addr_ai + i;
	
	
	for(int dg = 0; dg < 6; dg++)
	{
		addr_ai = 7870 + 20 * dg;
		for(int i = 1; i < 4; i++)
			m_mp_delay_addr[power_system_str[70 + 8 * dg + i]] = addr_ai + i;

		m_mp_delay_addr[power_system_str[70 + 8 * dg + 7]] = addr_ai + 5;

		for(int i = 6; i < 10; i++)
			m_mp_delay_addr[power_system_str[70 + i]] = addr_ai + i;
	}


	///////////////////////////////////////////////////
	for(int ix = 0; ix < 4;ix++)
	{
		for(int i = 0; i < 16; i++)
			m_mp_inhibit_addr[power_system_str[16*ix + i]] = i + 100 * ix;
	}
	for(int i = 0; i < 6; i++)
		m_mp_inhibit_addr[power_system_str[64 + i]] = i + 400;


	for(int dg = 0; dg < 6; dg++)
	{
		for(int i = 1; i < 4; i++)
			m_mp_inhibit_addr[power_system_str[70 + 8 * dg + i]] =  i + 100 * dg + 500;
		
		m_mp_inhibit_addr[power_system_str[70 + 8 * dg + 7]] = 100 * dg + 505;
		
		for(int i = 6; i < 10; i++)
			m_mp_inhibit_addr[power_system_str[70 + i]] = 100 * dg + i + 500;;
	}
	//dg6需要plc改下地址 45512.XX 改成45511.XX?

	
}

void POWER_SYSTEM::thSetRange()
{
	setRange();
}

void POWER_SYSTEM::thSetThreshold()
{
	setThreshold();
}

void POWER_SYSTEM::thSetDelay()
{
	setDelay();
}

void POWER_SYSTEM::thSetInhibit()
{
	while(1)
	{
		inhibit one = bq_inhibit.take();
		string key = one.m_keyname;
		bool inhib = one.is_inhibit;
		uint16_t addr_di = 5500;
		int ix = m_mp_inhibit_addr[key]/100;
		int iloop = m_mp_inhibit_addr[key]%100;

		if(!m_mb_plc1)
			continue;

		std::lock_guard<std::mutex> lock(*m_mux_plc1);
		uint16_t status[12]={0};
		if(m_mb_plc1->modbus_read_holding_registers(addr_di,12,status))
			continue;

		if(inhib) 
			status[ix] |= (1<<iloop);
		else
			status[ix] &= ~(1<<iloop);

		if(ix != 10)
			m_mb_plc1->modbus_write_register(addr_di + ix,status[ix]);
		else
			m_mb_plc1->modbus_write_register(addr_di + ix + 1,status[ix+1]);
	}

}





