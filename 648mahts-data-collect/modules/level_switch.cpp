#include <iostream>
#include <unistd.h>

#include "sgo_sub_pub.h"
#include "sgo_func.h"
#include "sgo_ams/level_switch.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

using namespace std;

const char* level_switch_str[]={
#ifndef MAHTS648
	"fuel_oil_store_tank_p_high_level_alarm",
	"fuel_oil_store_tank_p_low_level_alarm",
	"fuel_oil_day_tank_p_high_level_alarm",
	"fuel_oil_day_tank_p_low_level_alarm",
	"fuel_oil_store_tank_s_high_level_alarm",
	"fuel_oil_store_tank_s_low_level_alarm",
	"fuel_oil_day_tank_s_high_level_alarm",
	"fuel_oil_day_tank_s_low_level_alarm",
	"lube_oil_tank_high_level_alarm",
	"dirty_oil_tank_high_level_alarm",
	"black_water_tank_high_level_alarm",
	"grey_water_tank_high_level_alarm",
	"e_g_m_d_o_service_tank_high_level_alarm",
	"e_g_m_d_o_service_tk_low_level_alarm",
	"spill_tank_high_level_alarm",
	"b_t_room_bilge_high_level_alarm",
	"em_cy_fire_fighting_pump_room_bilge_high_level_alarm",
	"no1_void_p_tank_bilge_high_level_alarm",
	"no2_void_p_tank_bilge_high_level_alarm",
	"no2_void_s_tank_bilge_high_level_alarm",
	"no3_void_p_tank_bilge_high_level_alarm",
	"no3_void_s_tank_bilge_high_level_alarm",
	"no4_void_p_tank_bilge_high_level_alarm",
	"no4_void_s_tank_bilge_high_level_alarm",
	"no5_void_p_tank_bilge_high_level_alarm",
	"no5_void_s_tank_bilge_high_level_alarm",
	"no6_void_p_tank_bilge_high_level_alarm",
	"no6_void_s_tank_bilge_high_level_alarm",
	"aux_machinery_room1_bilge_high_level_alarm",
	"aux_machinery_room2_bilge_high_level_alarm",
	"e_r_ps_bilge_high_level_alarm",
	"e_r_sb_bilge_high_level_alarm",
	"aft_thruster_rm_ps_bilge_high_level_alarm",
	"aft_thruster_rm_sb_bilge_high_level_alarm",
	"bilge_water_tank_high_level_alarm",
	"no1_expansion_tank_low_level_alarm",
	"no2_expansion_tank_low_level_alarm",
	"no1_jackhouse_bilge_high_level_alarm",
	"no2_jackhouse_bilge_high_level_alarm",
	"no3_jackhouse_bilge_high_level_alarm",
	"no4_jackhouse_bilge_high_level_alarm",
	"galley_transformer_room_bilge_low_level_alarm",
	"f_o_transfer_pump_room_bilge_low_level_alarm",
	"device_offline",
#else
"fwd_thruster_vfd_room_bilge_high_level_alarm",
"aft_thruster_vfd_room_bilge_high_level_alarm",
"fuel_oil_day_tank_p_high_level_alarm",
"fuel_oil_day_tank_p_low_level_alarm",
"no1_expansion_tank_low_level_alarm",
"no2_expansion_tank_low_level_alarm",
"fuel_oil_day_tank_s_high_level_alarm",
"fuel_oil_day_tank_s_low_level_alarm",
"lube_oil_tank_high_level_alarm",
"dirty_oil_tank_high_level_alarm",
"black_water_tank_high_level_alarm",
"grey_water_tank_high_level_alarm",
"e_g_m_d_o_service_tank_high_level_alarm",
"e_g_m_d_o_service_tk_low_level_alarm",
"aft_thruster_room_p_bilge_high_level_alarm",
"aft_thruster_room_c_bilge_high_level_alarm",
"aft_thruster_room_s_bilge_high_level_alarm",
"pump_room_p_bilge_high_level_alarm",
"pump_room_s_bilge_high_level_alarm",
"store_room_bilge_high_level_alarm",
"compressor_room_bilge_high_level_alarm",
"water_maker_room_bilge_high_level_alarm",
"transformer_room_bilge_high_level_alarm",
"chiller_room_bilge_high_level_alarm",
"switchboard_room_bilge_high_level_alarm",
"spill_tank_high_level_alarm",
"void6_p_bilge_high_level_alarm",
"no6_void_s_tank_bilge_high_level_alarm",
"aux_machinery_room_p_bilge_high_level_alarm",
"aux_machinery_room_s_bilge_high_level_alarm",
"engine_room_p_bilge_high_level_alarm",
"engine_room_s_bilge_high_level_alarm",
"engine_room_c_bilge_high_level_alarm",
"pump_room_c_bilge_high_level_alarm",
"fwd_thruster_rm_p_bilge_high_level_alarm",
"fwd_thruster_rm_c_bilge_high_level_alarm",
"fwd_thruster_rm_s_bilge_high_level_alarm",
"void3_p_bilge_high_level_alarm",
"void3_s_bilge_high_level_alarm",
"void5_p_bilge_high_level_alarm",
"void5_s_bilge_high_level_alarm",
"void19_p_bilge_high_level_alarm",
"void19_s_bilge_high_level_alarm",
"void7_p_bilge_high_level_alarm",
"void7_s_bilge_high_level_alarm",
"void10_p_bilge_high_level_alarm",
"void10_s_bilge_high_level_alarm",
"cofferdam_p_bilge_high_level_alarm",
"cofferdam_s_bilge_high_level_alarm",
"void11_p_bilge_high_level_alarm",
"void11_s_bilge_high_level_alarm",
"void12_p_bilge_high_level_alarm",
"void12_s_bilge_high_level_alarm",
"void20_p_bilge_high_level_alarm",
"void20_s_bilge_high_level_alarm",
"void14_c_p_bilge_high_level_alarm",
"void14_c_s_bilge_high_level_alarm",
"bilge_tank_high_level_alarm",
"lube_oil_tank_low_level_alarm",
"fuel_oil_store_tank9_p_high_level_alarm",
"fuel_oil_store_tank16_p_high_level_alarm",
"fuel_oil_store_tank9_s_high_level_alarm",
"fuel_oil_store_tank16_s_high_level_alarm",
"void13_p_bilge_high_level_alarm",
"void13_s_bilge_high_level_alarm",
"pump_room_c_bilge_high_level_alarm2",
"device_offline",

#endif
};

//文档	from_level_remote_control_system
static const char* productCode = "level_switch";

LEVEL_SWITCH::LEVEL_SWITCH()
{}

LEVEL_SWITCH::~LEVEL_SWITCH()
{}

void LEVEL_SWITCH::init(int devId,BlockQueue<level_switch> &bq,modbus* mb,std::mutex* mux)
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
void LEVEL_SWITCH::connectionLostCallback(void* context, char* cause) 
{
	// 处理连接丢失事件
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	LEVEL_SWITCH* client = static_cast<LEVEL_SWITCH*>(ctx->manager);
	
	printf("--%s %d--address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());
	sgo_log_stderr(0,"--%s %d----address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());

	//std::lock_guard<std::mutex> lock(m_losts_mutex);
	client->m_v_lost[ctx->index] = 1;
	client->m_v_property_index_array.clear();


}


// 消息到达回调函数
int LEVEL_SWITCH::messageArrivedCallback(void* context, char* topicName, int topicLen,MQTTClient_message* message) 
{
	// 处理接收到的消息
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	LEVEL_SWITCH* client = static_cast<LEVEL_SWITCH*>(ctx->manager);
	
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
			client->m_property[level_switch_str[i]]=i;
		
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
void LEVEL_SWITCH::deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt) 
{
	// 处理交付完成事件
	//std::cout << "Message delivery complete." << std::endl;
}

void LEVEL_SWITCH::subClient()
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

void LEVEL_SWITCH::pubClient(BlockQueue<level_switch>& bq)
{
	while(1)
	{
		MQTTClient_message pubmsg = MQTTClient_message_initializer;
		//声明消息token
		MQTTClient_deliveryToken token;

		while(!m_pubStop){
			level_switch one = bq.take();
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




void LEVEL_SWITCH::start(BlockQueue<level_switch> &bq)
{
	m_sub = new thread(&LEVEL_SWITCH::subClient,this);
	m_sub->detach();

	m_pub = new thread(&LEVEL_SWITCH::pubClient,this,ref(bq));
	m_pub->detach();

	m_thr_reconn = new thread(&LEVEL_SWITCH::reconn,this);
	m_thr_reconn->detach();

	m_thr_resend = new thread(&LEVEL_SWITCH::resnd,this);
	m_thr_resend->detach();

	m_thr_chksensor = new thread(&LEVEL_SWITCH::chksensor,this);
	m_thr_chksensor->detach();

	
	thread *thr_inhibit = new thread(&LEVEL_SWITCH::thSetInhibit,this);
	thr_inhibit->detach();

	thread *thr_delay = new thread(&LEVEL_SWITCH::thSetDelay,this);
	thr_delay->detach();
}

void LEVEL_SWITCH::chksensor()
{
	checkSensorStatus();
}

void LEVEL_SWITCH::resnd()
{
	resend();
}

void LEVEL_SWITCH::reconn()
{
	reconnect();
}


string LEVEL_SWITCH::property_reply(char flag,const level_switch& one,const string& msg_id)
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
			case 0: writer.Int(one.fuel_oil_store_tank_p_high_level_alarm); break;
			case 1: writer.Int(one.fuel_oil_store_tank_p_low_level_alarm); break;
			case 2: writer.Int(one.fuel_oil_day_tank_p_high_level_alarm); break;
			case 3: writer.Int(one.fuel_oil_day_tank_p_low_level_alarm); break;
			case 4: writer.Int(one.fuel_oil_store_tank_s_high_level_alarm); break;
			case 5: writer.Int(one.fuel_oil_store_tank_s_low_level_alarm); break;
			case 6: writer.Int(one.fuel_oil_day_tank_s_high_level_alarm); break;
			case 7: writer.Int(one.fuel_oil_day_tank_s_low_level_alarm); break;
			case 8: writer.Int(one.lube_oil_tank_high_level_alarm); break;
			case 9: writer.Int(one.dirty_oil_tank_high_level_alarm); break;
			case 10: writer.Int(one.black_water_tank_high_level_alarm); break;
			case 11: writer.Int(one.grey_water_tank_high_level_alarm); break;
			case 12: writer.Int(one.e_g_m_d_o_service_tank_high_level_alarm); break;
			case 13: writer.Int(one.e_g_m_d_o_service_tk_low_level_alarm); break;
			case 14: writer.Int(one.spill_tank_high_level_alarm); break;
			case 15: writer.Int(one.b_t_room_bilge_high_level_alarm); break;
			case 16: writer.Int(one.em_cy_fire_fighting_pump_room_bilge_high_level_alarm); break;
			case 17: writer.Int(one.no1_void_p_tank_bilge_high_level_alarm); break;
			case 18: writer.Int(one.no2_void_p_tank_bilge_high_level_alarm); break;
			case 19: writer.Int(one.no2_void_s_tank_bilge_high_level_alarm); break;
			case 20: writer.Int(one.no3_void_p_tank_bilge_high_level_alarm); break;
			case 21: writer.Int(one.no3_void_s_tank_bilge_high_level_alarm); break;
			case 22: writer.Int(one.no4_void_p_tank_bilge_high_level_alarm); break;
			case 23: writer.Int(one.no4_void_s_tank_bilge_high_level_alarm); break;
			case 24: writer.Int(one.no5_void_p_tank_bilge_high_level_alarm); break;
			case 25: writer.Int(one.no5_void_s_tank_bilge_high_level_alarm); break;
			case 26: writer.Int(one.no6_void_p_tank_bilge_high_level_alarm); break;
			case 27: writer.Int(one.no6_void_s_tank_bilge_high_level_alarm); break;
			case 28: writer.Int(one.aux_machinery_room1_bilge_high_level_alarm); break;
			case 29: writer.Int(one.aux_machinery_room2_bilge_high_level_alarm); break;
			case 30: writer.Int(one.e_r_ps_bilge_high_level_alarm); break;
			case 31: writer.Int(one.e_r_sb_bilge_high_level_alarm); break;
			case 32: writer.Int(one.aft_thruster_rm_ps_bilge_high_level_alarm); break;
			case 33: writer.Int(one.aft_thruster_rm_sb_bilge_high_level_alarm); break;
			case 34: writer.Int(one.bilge_water_tank_high_level_alarm); break;
			case 35: writer.Int(one.no1_expansion_tank_low_level_alarm); break;
			case 36: writer.Int(one.no2_expansion_tank_low_level_alarm); break;
			case 37: writer.Int(one.no1_jackhouse_bilge_high_level_alarm); break;
			case 38: writer.Int(one.no2_jackhouse_bilge_high_level_alarm); break;
			case 39: writer.Int(one.no3_jackhouse_bilge_high_level_alarm); break;
			case 40: writer.Int(one.no4_jackhouse_bilge_high_level_alarm); break;
			case 41: writer.Int(one.galley_transformer_room_bilge_low_level_alarm); break;
			case 42: writer.Int(one.f_o_transfer_pump_room_bilge_low_level_alarm); break;
			case 43: writer.Int(one.device_offline); break;
#else
case 0:writer.Int(one.fwd_thruster_vfd_room_bilge_high_level_alarm);break;
case 1:writer.Int(one.aft_thruster_vfd_room_bilge_high_level_alarm);break;
case 2:writer.Int(one.fuel_oil_day_tank_p_high_level_alarm);break;
case 3:writer.Int(one.fuel_oil_day_tank_p_low_level_alarm);break;
case 4:writer.Int(one.no1_expansion_tank_low_level_alarm);break;
case 5:writer.Int(one.no2_expansion_tank_low_level_alarm);break;
case 6:writer.Int(one.fuel_oil_day_tank_s_high_level_alarm);break;
case 7:writer.Int(one.fuel_oil_day_tank_s_low_level_alarm);break;
case 8:writer.Int(one.lube_oil_tank_high_level_alarm);break;
case 9:writer.Int(one.dirty_oil_tank_high_level_alarm);break;
case 10:writer.Int(one.black_water_tank_high_level_alarm);break;
case 11:writer.Int(one.grey_water_tank_high_level_alarm);break;
case 12:writer.Int(one.e_g_m_d_o_service_tank_high_level_alarm);break;
case 13:writer.Int(one.e_g_m_d_o_service_tk_low_level_alarm);break;
case 14:writer.Int(one.aft_thruster_room_p_bilge_high_level_alarm);break;
case 15:writer.Int(one.aft_thruster_room_c_bilge_high_level_alarm);break;
case 16:writer.Int(one.aft_thruster_room_s_bilge_high_level_alarm);break;
case 17:writer.Int(one.pump_room_p_bilge_high_level_alarm);break;
case 18:writer.Int(one.pump_room_s_bilge_high_level_alarm);break;
case 19:writer.Int(one.store_room_bilge_high_level_alarm);break;
case 20:writer.Int(one.compressor_room_bilge_high_level_alarm);break;
case 21:writer.Int(one.water_maker_room_bilge_high_level_alarm);break;
case 22:writer.Int(one.transformer_room_bilge_high_level_alarm);break;
case 23:writer.Int(one.chiller_room_bilge_high_level_alarm);break;
case 24:writer.Int(one.switchboard_room_bilge_high_level_alarm);break;
case 25:writer.Int(one.spill_tank_high_level_alarm);break;
case 26:writer.Int(one.no6_void_p_tank_bilge_high_level_alarm);break;
case 27:writer.Int(one.no6_void_s_tank_bilge_high_level_alarm);break;
case 28:writer.Int(one.aux_machinery_room_p_bilge_high_level_alarm);break;
case 29:writer.Int(one.aux_machinery_room_s_bilge_high_level_alarm);break;
case 30:writer.Int(one.engine_room_p_bilge_high_level_alarm);break;
case 31:writer.Int(one.engine_room_s_bilge_high_level_alarm);break;
case 32:writer.Int(one.engine_room_c_bilge_high_level_alarm);break;
case 33:writer.Int(one.pump_room_c_bilge_high_level_alarm);break;
case 34:writer.Int(one.fwd_thruster_rm_p_bilge_high_level_alarm);break;
case 35:writer.Int(one.fwd_thruster_rm_c_bilge_high_level_alarm);break;
case 36:writer.Int(one.fwd_thruster_rm_s_bilge_high_level_alarm);break;
case 37:writer.Int(one.no3_void_p_tank_bilge_high_level_alarm);break;
case 38:writer.Int(one.no3_void_s_tank_bilge_high_level_alarm);break;
case 39:writer.Int(one.no5_void_p_tank_bilge_high_level_alarm);break;
case 40:writer.Int(one.no5_void_s_tank_bilge_high_level_alarm);break;
case 41:writer.Int(one.no19_void_p_tank_bilge_high_level_alarm);break;
case 42:writer.Int(one.no19_void_s_tank_bilge_high_level_alarm);break;
case 43:writer.Int(one.no7_void_p_tank_bilge_high_level_alarm);break;
case 44:writer.Int(one.no7_void_s_tank_bilge_high_level_alarm);break;
case 45:writer.Int(one.no10_void_p_tank_bilge_high_level_alarm);break;
case 46:writer.Int(one.no10_void_s_tank_bilge_high_level_alarm);break;
case 47:writer.Int(one.cofferdam_p_bilge_high_level_alarm);break;
case 48:writer.Int(one.cofferdam_s_bilge_high_level_alarm);break;
case 49:writer.Int(one.no11_void_p_tank_bilge_high_level_alarm);break;
case 50:writer.Int(one.no11_void_s_tank_bilge_high_level_alarm);break;
case 51:writer.Int(one.no12_void_p_tank_bilge_high_level_alarm);break;
case 52:writer.Int(one.no12_void_s_tank_bilge_high_level_alarm);break;
case 53:writer.Int(one.no20_void_p_tank_bilge_high_level_alarm);break;
case 54:writer.Int(one.no20_void_s_tank_bilge_high_level_alarm);break;
case 55:writer.Int(one.no14c_void_p_tank_bilge_high_level_alarm);break;
case 56:writer.Int(one.no14c_void_s_tank_bilge_high_level_alarm);break;
case 57:writer.Int(one.bilge_tank_high_level_alarm);break;
case 58:writer.Int(one.lube_oil_tank_low_level_alarm);break;
case 59:writer.Int(one.fuel_oil_store_tank_9_p_high_level_alarm);break;
case 60:writer.Int(one.fuel_oil_store_tank_16_p_high_level_alarm);break;
case 61:writer.Int(one.fuel_oil_store_tank_9_s_high_level_alarm);break;
case 62:writer.Int(one.fuel_oil_store_tank_16_s_high_level_alarm);break;
case 63:writer.Int(one.no13_void_p_tank_bilge_high_level_alarm);break;
case 64:writer.Int(one.no13_void_s_tank_bilge_high_level_alarm);break;
case 65:writer.Int(one.pump_room_c_bilge_high_level_alarm_2);break;
case 66:writer.Int(one.device_offline);break;

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
 //cout << "level switch:"<<data << endl;
	return strBuf.GetString();
}

void LEVEL_SWITCH::thSetDelay()
{
	setDelay();
}


void LEVEL_SWITCH::thSetInhibit()
{
	setInhibit(5807,4);
}

void LEVEL_SWITCH::init_mb_addr()
{
	int addr_ai = 8126;
	for(int i = 0;i < 43;i++)
	{
		m_mp_delay_addr[level_switch_str[i]] = addr_ai + i;
	}

	/////////////////////////////////////////////
	for(int i = 0;i < 6;i++)
	{
		m_mp_inhibit_addr[level_switch_str[i]] = 10 + i;
	}	
	for(int i = 0;i < 16;i++)
	{
		m_mp_inhibit_addr[level_switch_str[6 + i]] = 100 + i;
	}
	for(int i = 0;i < 16;i++)
	{
		m_mp_inhibit_addr[level_switch_str[22 + i]] = 200 + i;
	}
	for(int i = 0;i < 5;i++)
	{
		m_mp_inhibit_addr[level_switch_str[38 + i]] = 300 + i;
	}
}







