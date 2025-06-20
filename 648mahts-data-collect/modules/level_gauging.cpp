#include <iostream>
#include <unistd.h>

#include "sgo_sub_pub.h"
#include "sgo_func.h"
#include "sgo_ams/level_gauging.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

using namespace std;


const char *level_gauging_str[300] = {
#ifndef MAHTS648

#else
	"s_g_roomp_bilge_well_high_level",
	"s_g_rooms_bilge_well_high_level",
	"aft_cement_tkp_a_bilge_well_high_level",
	"aft_cement_tks_a_bilge_well_high_level",
	"aft_cement_tkm_bilge_well_high_level",
	"fwd_cement_tkp_a_bilge_well_high_level",
	"fwd_cement_tks_a_bilge_well_high_level",
	"fwd_cement_tkm_bilge_well_high_level",
	"e_rp_a_bilge_well_high_level",
	"e_rs_a_bilge_well_high_level",
	"e_rp_f_bilge_well_high_level",
	"e_rs_f_bilge_well_high_level",
	"b_t_f_bilge_well_high_level",
	"base_oil_ro_tkp_high_level",
	"base_oil_ro_tks_high_level",
	"no1_mud_tkp_high_level",
	"no1_mud_tks_high_level",
	"no1_mud_tkp_low_level",
	"no1_mud_tks_low_level",
	"no2_mud_tkp_high_level",
	"no2_mud_tks_high_level",
	"no2_mud_tkp_low_level",
	"no2_mud_tks_low_level",
	"no3_f_o_overflow_tkc_high_level",
	"bilge_holding_tkp_high_level",
	"dirty_oil_tk_high_level",
	"sewage_tk_high_level",
	"f_o_day_tkp_high_level",
	"f_o_day_tkp_low_level",
	"f_o_day_tks_high_level",
	"f_o_day_tks_low_level",
	"b_t_a_bilge_well_high_level",
	"f_o_overflow_alarm",
	"hpr_tks_high_level",
	"em_cy_generator_set_daily_tk_low_level",
	"em_cy_generator_set_daily_tk_high_level",
	"no1_bt_f_w_expansion_t_k_low_level",
	"no2_bt_f_w_expansion_t_k_low_level",
	"port_azimuth_f_w_expansion_t_k_low_level",
	"stbd_azimuth_f_w_expansion_t_k_low_level",
	"no1_f_w_expansion_t_k_low_level",
	"no2_f_w_expansion_t_k_low_level",
	"no3_f_w_expansion_t_k_low_level",
	"no4_f_w_expansion_t_k_low_level",
	"quick_closing_air_receiver_pressure_low_alarm",
	"output_mud_tk_level_alarm_bell",
	"spare",
	"no1_db_f_o_tkp_level",
	"no1_db_f_o_tks_level",
	"no1_wing_f_o_tkp_level",
	"no1_wing_f_o_tks_level",
	"no2_db_f_o_tkp_level",
	"no2_db_f_o_tks_level",
	"no2_wing_f_o_tkp_level",
	"no2_wing_f_o_tks_level",
	"no3_db_f_o_tkcoverflow_level",
	"no3_db_f_o_tkp_level",
	"no3_db_f_o_tks_level",
	"no3_db_f_o_tkc_level",
	"no4_db_f_o_tkc_level",
	"no4_wing_f_o_tkp_level",
	"no4_wing_f_o_tks_level",
	"no5_f_o_tkp_level",
	"no5_f_o_tks_level",
	"no6_db_f_o_tkc_level",
	"f_o_day_tkp_level",
	"f_o_day_tks_level",
	"f_o_settling_tkp_level",
	"fresh_water_tank5p_level",
	"no1_db_f_o_tkp_volume_m3",
	"no1_db_f_o_tks_volume_m3",
	"no1_wing_f_o_tkp_volume_m3",
	"no1_wing_f_o_tks_volume_m3",
	"no2_db_f_o_tkp_volume_m3",
	"no2_db_f_o_tks_volume_m3",
	"no2_wing_f_o_tkp_volume_m3",
	"no2_wing_f_o_tks_volume_m3",
	"no3_db_f_o_tkcoverflow_volume_m3",
	"no3_db_f_o_tkp_volume_m3",
	"no3_db_f_o_tks_volume_m3",
	"no3_db_f_o_tkc_volume_m3",
	"no4_db_f_o_tkc_volume_m3",
	"no4_wing_f_o_tkp_volume_m3",
	"no4_wing_f_o_tks_volume_m3",
	"no5_f_o_tkp_volume_m3",
	"no5_f_o_tks_volume_m3",
	"no6_db_f_o_tkc_volume_m3",
	"f_o_day_tkp_volume_m3",
	"f_o_day_tks_volume_m3",
	"f_o_settling_tkp_volume_m3",
	"fresh_water_tank5p_volume_m3",
	"no1_db_f_o_tkp_volume_percent",
	"no1_db_f_o_tks_volume_percent",
	"no1_wing_f_o_tkp_volume_percent",
	"no1_wing_f_o_tks_volume_percent",
	"no2_db_f_o_tkp_volume_percent",
	"no2_db_f_o_tks_volume_percent",
	"no2_wing_f_o_tkp_volume_percent",
	"no2_wing_f_o_tks_volume_percent",
	"no3_db_f_o_tkcoverflow_volume_percent",
	"no3_db_f_o_tkp_volume_percent",
	"no3_db_f_o_tks_volume_percent",
	"no3_db_f_o_tkc_volume_percent",
	"no4_db_f_o_tkc_volume_percent",
	"no4_wing_f_o_tkp_volume_percent",
	"no4_wing_f_o_tks_volume_percent",
	"no5_f_o_tkp_volume_percent",
	"no5_f_o_tks_volume_percent",
	"no6_db_f_o_tkc_volume_percent",
	"f_o_day_tkp_volume_percent",
	"f_o_day_tks_volume_percent",
	"f_o_settling_tkp_volume_percent",
	"fresh_water_tank5p_volume_percent",
	"no1_db_f_o_tkp_high_alarm",
	"no1_db_f_o_tks_high_alarm",
	"no1_wing_f_o_tkp_high_alarm",
	"no1_wing_f_o_tks_high_alarm",
	"no2_db_f_o_tkp_high_alarm",
	"no2_db_f_o_tks_high_alarm",
	"no2_wing_f_o_tkp_high_alarm",
	"no2_wing_f_o_tks_high_alarm",
	"no3_db_f_o_tkcoverflow_high_alarm",
	"no3_db_f_o_tkp_high_alarm",
	"no3_db_f_o_tks_high_alarm",
	"no3_db_f_o_tkc_high_alarm",
	"no4_db_f_o_tkc_high_alarm",
	"no4_wing_f_o_tkp_high_alarm",
	"no4_wing_f_o_tks_high_alarm",
	"no5_f_o_tkp_high_alarm",
	"no5_f_o_tks_high_alarm",
	"no6_db_f_o_tkc_high_alarm",
	"f_o_day_tkp_high_alarm",
    "f_o_day_tks_high_alarm",
	"f_o_settling_tkp_high_alarm",
	"fresh_water_tank5p_high_alarm",
	"no1_db_f_o_tkp_low_alarm",
	"no1_db_f_o_tks_low_alarm",
	"no1_wing_f_o_tkp_low_alarm",
	"no1_wing_f_o_tks_low_alarm",
	"no2_db_f_o_tkp_low_alarm",
	"no2_db_f_o_tks_low_alarm",
	"no2_wing_f_o_tkp_low_alarm",
	"no2_wing_f_o_tks_low_alarm",
	"no3_db_f_o_tkcoverflow_low_alarm",
	"no3_db_f_o_tkp_low_alarm",
	"no3_db_f_o_tks_low_alarm",
	"no3_db_f_o_tkc_low_alarm",
	"no4_db_f_o_tkc_low_alarm",
	"no4_wing_f_o_tkp_low_alarm",
	"no4_wing_f_o_tks_low_alarm",
	"no5_f_o_tkp_low_alarm",
	"no5_f_o_tks_low_alarm",
	"no6_db_f_o_tkc_low_alarm",
	"f_o_day_tkp_low_alarm",
	"f_o_day_tks_low_alarm",
	"f_o_settling_tkp_low_alarm",
	"fresh_water_tank5p_low_alarm",
    "f_o_setling_tkp_high_level",
    "f_o_setling_tkp_low_level",
    "f_o_setling_tks_high_level",
    "f_o_settling_tks_low_level",
	"device_offline",

#endif
};

//文档	from_level_remote_control_system
static const char* productCode = "level_gauging";

LEVEL_GAUGING::LEVEL_GAUGING()
{
    

}

LEVEL_GAUGING::~LEVEL_GAUGING()
{}

void LEVEL_GAUGING::init(int devId,BlockQueue<level_gauging> &bq,modbus *mb,mutex *mux)
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
void LEVEL_GAUGING::connectionLostCallback(void* context, char* cause) 
{
	// 处理连接丢失事件
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	LEVEL_GAUGING* client = static_cast<LEVEL_GAUGING*>(ctx->manager);
	
	printf("--%s %d--address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());
	sgo_log_stderr(0,"--%s %d----address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());

	//std::lock_guard<std::mutex> lock(m_losts_mutex);
	client->m_v_lost[ctx->index] = 1;
	client->m_v_property_index_array.clear();

}


// 消息到达回调函数
int LEVEL_GAUGING::messageArrivedCallback(void* context, char* topicName, int topicLen,MQTTClient_message* message) 
{
	// 处理接收到的消息
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	LEVEL_GAUGING* client = static_cast<LEVEL_GAUGING*>(ctx->manager);
	
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
			client->m_property[level_gauging_str[i]]=i;
		
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
void LEVEL_GAUGING::deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt) 
{
	// 处理交付完成事件
	//std::cout << "Message delivery complete." << std::endl;
}

void LEVEL_GAUGING::subClient()
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


void LEVEL_GAUGING::pubClient(BlockQueue<level_gauging>& bq)
{
	while(1)
	{
		MQTTClient_message pubmsg = MQTTClient_message_initializer;
		//声明消息token
		MQTTClient_deliveryToken token;

		while(!m_pubStop){
			level_gauging one = bq.take();
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




void LEVEL_GAUGING::start(BlockQueue<level_gauging> &bq)
{
	m_sub = new thread(&LEVEL_GAUGING::subClient,this);
	m_sub->detach();

	m_pub = new thread(&LEVEL_GAUGING::pubClient,this,ref(bq));
	m_pub->detach();

	m_thr_reconn = new thread(&LEVEL_GAUGING::reconn,this);
	m_thr_reconn->detach();

	m_thr_resend = new thread(&LEVEL_GAUGING::resnd,this);
	m_thr_resend->detach();

	m_thr_chksensor = new thread(&LEVEL_GAUGING::chksensor,this);
	m_thr_chksensor->detach();
	
//	thread *thr_range = new thread(&LEVEL_GAUGING::thSetRange,this);
//	thr_range->detach();

	thread *thr_threshold = new thread(&LEVEL_GAUGING::thSetThreshold,this);
	thr_threshold->detach();

	thread *thr_inhibit = new thread(&LEVEL_GAUGING::thSetInhibit,this);
	thr_inhibit->detach();

	thread *thr_delay = new thread(&LEVEL_GAUGING::thSetDelay,this);
	thr_delay->detach();	
}

void LEVEL_GAUGING::chksensor()
{
	checkSensorStatus();
}

void LEVEL_GAUGING::resnd()
{
	resend();
}

void LEVEL_GAUGING::reconn()
{
	reconnect();
}


string LEVEL_GAUGING::property_reply(char flag,const level_gauging& one,const string& msg_id)
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
				case 0:writer.Double(one.forep_draft);break;
				case 1:writer.Double(one.fores_draft);break;
				case 2:writer.Double(one.aftp_draft);break;
				case 3:writer.Double(one.afts_draft);break;
				case 4:writer.Double(one.sw_ballast_tank1p_level);break;
				case 5:writer.Double(one.sw_ballast_tank1s_level);break;
				case 6:writer.Double(one.sw_ballast_tank2p_level);break;
				case 7:writer.Double(one.sw_ballast_tank2s_level);break;
				case 8:writer.Double(one.sw_ballast_tank3p_level);break;
				case 9:writer.Double(one.sw_ballast_tank3s_level);break;
				case 10:writer.Double(one.sw_ballast_tank3m_level);break;
				case 11:writer.Double(one.fresh_water_tank1p_level);break;
				case 12:writer.Double(one.fresh_water_tank1s_level);break;
				case 13:writer.Double(one.fresh_water_tank2p_level);break;
				case 14:writer.Double(one.fresh_water_tank2s_level);break;
				case 15:writer.Double(one.fuel_oil_store_tank_p_level);break;
				case 16:writer.Double(one.fuel_oil_store_tank_s_level);break;
				case 17:writer.Double(one.fuel_oil_day_tank_p_level);break;
				case 18:writer.Double(one.fuel_oil_day_tank_s_level);break;
				case 19:writer.Double(one.sw_ballast_tank1p_capacity);break;
				case 20:writer.Double(one.sw_ballast_tank1s_capacity);break;
				case 21:writer.Double(one.sw_ballast_tank2p_capacity);break;
				case 22:writer.Double(one.sw_ballast_tank2s_capacity);break;
				case 23:writer.Double(one.sw_ballast_tank3p_capacity);break;
				case 24:writer.Double(one.sw_ballast_tank3s_capacity);break;
				case 25:writer.Double(one.sw_ballast_tank3m_capacity);break;
				case 26:writer.Double(one.fresh_water_tank1p_capacity);break;
				case 27:writer.Double(one.fresh_water_tank1s_capacity);break;
				case 28:writer.Double(one.fresh_water_tank2p_capacity);break;
				case 29:writer.Double(one.fresh_water_tank2s_capacity);break;
				case 30:writer.Double(one.fuel_oil_store_tank_p_capacity);break;
				case 31:writer.Double(one.fuel_oil_store_tank_s_capacity);break;
				case 32:writer.Double(one.fuel_oil_day_tank_p_capacity);break;
				case 33:writer.Double(one.fuel_oil_day_tank_s_capacity);break;
				case 34:writer.Int(one.level_mearsurement_system_common_alarm);break;
				case 35:writer.Int(one.device_offline);break;
#else
                case 0:writer.Int(one.s_g_roomp_bilge_well_high_level);break;
                case 1:writer.Int(one.s_g_rooms_bilge_well_high_level);break;
                case 2:writer.Int(one.aft_cement_tkp_a_bilge_well_high_level);break;
                case 3:writer.Int(one.aft_cement_tks_a_bilge_well_high_level);break;
                case 4:writer.Int(one.aft_cement_tkm_bilge_well_high_level);break;
                case 5:writer.Int(one.fwd_cement_tkp_a_bilge_well_high_level);break;
                case 6:writer.Int(one.fwd_cement_tks_a_bilge_well_high_level);break;
                case 7:writer.Int(one.fwd_cement_tkm_bilge_well_high_level);break;
                case 8:writer.Int(one.e_rp_a_bilge_well_high_level);break;
                case 9:writer.Int(one.e_rs_a_bilge_well_high_level);break;
                case 10:writer.Int(one.e_rp_f_bilge_well_high_level);break;
                case 11:writer.Int(one.e_rs_f_bilge_well_high_level);break;
                case 12:writer.Int(one.b_t_f_bilge_well_high_level);break;//b_t_room_bilge_well_high_level->b_t_f_bilge_well_high_level 25.5.19
                case 13:writer.Int(one.base_oil_ro_tkp_high_level);break;
                case 14:writer.Int(one.base_oil_ro_tks_high_level);break;
                case 15:writer.Int(one.no1_mud_tkp_high_level);break;
                case 16:writer.Int(one.no1_mud_tks_high_level);break;
                case 17:writer.Int(one.no1_mud_tkp_low_level);break;
                case 18:writer.Int(one.no1_mud_tks_low_level);break;
                case 19:writer.Int(one.no2_mud_tkp_high_level);break;
                case 20:writer.Int(one.no2_mud_tks_high_level);break;
                case 21:writer.Int(one.no2_mud_tkp_low_level);break;
                case 22:writer.Int(one.no2_mud_tks_low_level);break;
                case 23:writer.Int(one.no3_f_o_overflow_tkc_high_level);break;
                case 24:writer.Int(one.bilge_holding_tkp_high_level);break;
                case 25:writer.Int(one.dirty_oil_tk_high_level);break;
                case 26:writer.Int(one.sewage_tk_high_level);break;
                case 27:writer.Int(one.f_o_day_tkp_high_level);break;
                case 28:writer.Int(one.f_o_day_tkp_low_level);break;
                case 29:writer.Int(one.f_o_day_tks_high_level);break;
                case 30:writer.Int(one.f_o_day_tks_low_level);break;
                case 31:writer.Int(one.b_t_a_bilge_well_high_level);break;// b_t_a_bilge_well_low_level->b_t_a_bilge_well_high_level 25.5.27
                case 32:writer.Int(one.f_o_overflow_alarm);break;
                case 33:writer.Int(one.hpr_tks_high_level);break;
                case 34:writer.Int(one.em_cy_generator_set_daily_tk_low_level);break;
                case 35:writer.Int(one.em_cy_generator_set_daily_tk_high_level);break;
                case 36:writer.Int(one.no1_bt_f_w_expansion_t_k_low_level);break;
                case 37:writer.Int(one.no2_bt_f_w_expansion_t_k_low_level);break;
                case 38:writer.Int(one.port_azimuth_f_w_expansion_t_k_low_level);break;
                case 39:writer.Int(one.stbd_azimuth_f_w_expansion_t_k_low_level);break;
                case 40:writer.Int(one.no1_f_w_expansion_t_k_low_level);break;
                case 41:writer.Int(one.no2_f_w_expansion_t_k_low_level);break;
                case 42:writer.Int(one.no3_f_w_expansion_t_k_low_level);break;
                case 43:writer.Int(one.no4_f_w_expansion_t_k_low_level);break;
                case 44:writer.Int(one.quick_closing_air_receiver_pressure_low_alarm);break;
                case 45:writer.Int(one.output_mud_tk_level_alarm_bell);break;
                case 46:writer.Int(one.spare);break;
                case 47:writer.Double(one.no1_db_f_o_tkp_level);break;
                case 48:writer.Double(one.no1_db_f_o_tks_level);break;
                case 49:writer.Double(one.no1_wing_f_o_tkp_level);break;
                case 50:writer.Double(one.no1_wing_f_o_tks_level);break;
                case 51:writer.Double(one.no2_db_f_o_tkp_level);break;
                case 52:writer.Double(one.no2_db_f_o_tks_level);break;
                case 53:writer.Double(one.no2_wing_f_o_tkp_level);break;
                case 54:writer.Double(one.no2_wing_f_o_tks_level);break;
                case 55:writer.Double(one.no3_db_f_o_tkcoverflow_level);break;
                case 56:writer.Double(one.no3_db_f_o_tkp_level);break;
                case 57:writer.Double(one.no3_db_f_o_tks_level);break;
                case 58:writer.Double(one.no3_db_f_o_tkc_level);break;
                case 59:writer.Double(one.no4_db_f_o_tkc_level);break;
                case 60:writer.Double(one.no4_wing_f_o_tkp_level);break;
                case 61:writer.Double(one.no4_wing_f_o_tks_level);break;
                case 62:writer.Double(one.no5_f_o_tkp_level);break;
                case 63:writer.Double(one.no5_f_o_tks_level);break;
                case 64:writer.Double(one.no6_db_f_o_tkc_level);break;
                case 65:writer.Double(one.f_o_day_tkp_level);break;
                case 66:writer.Double(one.f_o_day_tks_level);break;
                case 67:writer.Double(one.f_o_settling_tkp_level);break;
                case 68:writer.Double(one.fresh_water_tank5p_level);break;
                case 69:writer.Double(one.no1_db_f_o_tkp_volume_m3);break;
                case 70:writer.Double(one.no1_db_f_o_tks_volume_m3);break;
                case 71:writer.Double(one.no1_wing_f_o_tkp_volume_m3);break;
                case 72:writer.Double(one.no1_wing_f_o_tks_volume_m3);break;
                case 73:writer.Double(one.no2_db_f_o_tkp_volume_m3);break;
                case 74:writer.Double(one.no2_db_f_o_tks_volume_m3);break;
                case 75:writer.Double(one.no2_wing_f_o_tkp_volume_m3);break;
                case 76:writer.Double(one.no2_wing_f_o_tks_volume_m3);break;
                case 77:writer.Double(one.no3_db_f_o_tkcoverflow_volume_m3);break;
                case 78:writer.Double(one.no3_db_f_o_tkp_volume_m3);break;
                case 79:writer.Double(one.no3_db_f_o_tks_volume_m3);break;
                case 80:writer.Double(one.no3_db_f_o_tkc_volume_m3);break;
                case 81:writer.Double(one.no4_db_f_o_tkc_volume_m3);break;
                case 82:writer.Double(one.no4_wing_f_o_tkp_volume_m3);break;
                case 83:writer.Double(one.no4_wing_f_o_tks_volume_m3);break;
                case 84:writer.Double(one.no5_f_o_tkp_volume_m3);break;
                case 85:writer.Double(one.no5_f_o_tks_volume_m3);break;
                case 86:writer.Double(one.no6_db_f_o_tkc_volume_m3);break;
                case 87:writer.Double(one.f_o_day_tkp_volume_m3);break;
                case 88:writer.Double(one.f_o_day_tks_volume_m3);break;
                case 89:writer.Double(one.f_o_settling_tkp_volume_m3);break;
                case 90:writer.Double(one.fresh_water_tank5p_volume_m3);break;
                case 91:writer.Double(one.no1_db_f_o_tkp_volume_percent);break;
                case 92:writer.Double(one.no1_db_f_o_tks_volume_percent);break;
                case 93:writer.Double(one.no1_wing_f_o_tkp_volume_percent);break;
                case 94:writer.Double(one.no1_wing_f_o_tks_volume_percent);break;
                case 95:writer.Double(one.no2_db_f_o_tkp_volume_percent);break;
                case 96:writer.Double(one.no2_db_f_o_tks_volume_percent);break;
                case 97:writer.Double(one.no2_wing_f_o_tkp_volume_percent);break;
                case 98:writer.Double(one.no2_wing_f_o_tks_volume_percent);break;
                case 99:writer.Double(one.no3_db_f_o_tkcoverflow_volume_percent);break;
                case 100:writer.Double(one.no3_db_f_o_tkp_volume_percent);break;
                case 101:writer.Double(one.no3_db_f_o_tks_volume_percent);break;
                case 102:writer.Double(one.no3_db_f_o_tkc_volume_percent);break;
                case 103:writer.Double(one.no4_db_f_o_tkc_volume_percent);break;
                case 104:writer.Double(one.no4_wing_f_o_tkp_volume_percent);break;
                case 105:writer.Double(one.no4_wing_f_o_tks_volume_percent);break;
                case 106:writer.Double(one.no5_f_o_tkp_volume_percent);break;
                case 107:writer.Double(one.no5_f_o_tks_volume_percent);break;
                case 108:writer.Double(one.no6_db_f_o_tkc_volume_percent);break;
                case 109:writer.Double(one.f_o_day_tkp_volume_percent);break;
                case 110:writer.Double(one.f_o_day_tks_volume_percent);break;
                case 111:writer.Double(one.f_o_settling_tkp_volume_percent);break;
                case 112:writer.Double(one.fresh_water_tank5p_volume_percent);break;
                case 113:writer.Int(one.no1_db_f_o_tkp_high_alarm);break;
                case 114:writer.Int(one.no1_db_f_o_tks_high_alarm);break;
                case 115:writer.Int(one.no1_wing_f_o_tkp_high_alarm);break;
                case 116:writer.Int(one.no1_wing_f_o_tks_high_alarm);break;
                case 117:writer.Int(one.no2_db_f_o_tkp_high_alarm);break;
                case 118:writer.Int(one.no2_db_f_o_tks_high_alarm);break;
                case 119:writer.Int(one.no2_wing_f_o_tkp_high_alarm);break;
                case 120:writer.Int(one.no2_wing_f_o_tks_high_alarm);break;
                case 121:writer.Int(one.no3_db_f_o_tkcoverflow_high_alarm);break;
                case 122:writer.Int(one.no3_db_f_o_tkp_high_alarm);break;
                case 123:writer.Int(one.no3_db_f_o_tks_high_alarm);break;
                case 124:writer.Int(one.no3_db_f_o_tkc_high_alarm);break;
                case 125:writer.Int(one.no4_db_f_o_tkc_high_alarm);break;
                case 126:writer.Int(one.no4_wing_f_o_tkp_high_alarm);break;
                case 127:writer.Int(one.no4_wing_f_o_tks_high_alarm);break;
                case 128:writer.Int(one.no5_f_o_tkp_high_alarm);break;
                case 129:writer.Int(one.no5_f_o_tks_high_alarm);break;
                case 130:writer.Int(one.no6_db_f_o_tkc_high_alarm);break;
                case 131:writer.Int(one.f_o_day_tkp_high_alarm);break;
                case 132:writer.Int(one.f_o_day_tks_high_alarm);break;
                case 133:writer.Int(one.f_o_settling_tkp_high_alarm);break;
                case 134:writer.Int(one.fresh_water_tank5p_high_alarm);break;
                case 135:writer.Int(one.no1_db_f_o_tkp_low_alarm);break;
                case 136:writer.Int(one.no1_db_f_o_tks_low_alarm);break;
                case 137:writer.Int(one.no1_wing_f_o_tkp_low_alarm);break;
                case 138:writer.Int(one.no1_wing_f_o_tks_low_alarm);break;
                case 139:writer.Int(one.no2_db_f_o_tkp_low_alarm);break;
                case 140:writer.Int(one.no2_db_f_o_tks_low_alarm);break;
                case 141:writer.Int(one.no2_wing_f_o_tkp_low_alarm);break;
                case 142:writer.Int(one.no2_wing_f_o_tks_low_alarm);break;
                case 143:writer.Int(one.no3_db_f_o_tkcoverflow_low_alarm);break;
                case 144:writer.Int(one.no3_db_f_o_tkp_low_alarm);break;
                case 145:writer.Int(one.no3_db_f_o_tks_low_alarm);break;
                case 146:writer.Int(one.no3_db_f_o_tkc_low_alarm);break;
                case 147:writer.Int(one.no4_db_f_o_tkc_low_alarm);break;
                case 148:writer.Int(one.no4_wing_f_o_tkp_low_alarm);break;
                case 149:writer.Int(one.no4_wing_f_o_tks_low_alarm);break;
                case 150:writer.Int(one.no5_f_o_tkp_low_alarm);break;
                case 151:writer.Int(one.no5_f_o_tks_low_alarm);break;
                case 152:writer.Int(one.no6_db_f_o_tkc_low_alarm);break;
                case 153:writer.Int(one.f_o_day_tkp_low_alarm);break;
                case 154:writer.Int(one.f_o_day_tks_low_alarm);break;
                case 155:writer.Int(one.f_o_settling_tkp_low_alarm);break;
                case 156:writer.Int(one.fresh_water_tank5p_low_alarm);break;
				case 157:writer.Int(one.f_o_setling_tkp_high_level);break;
				case 158:writer.Int(one.f_o_setling_tkp_low_level);break;
				case 159:writer.Int(one.f_o_setling_tks_high_level);break;
				case 160:writer.Int(one.f_o_setling_tks_low_level);break;
                case 161:writer.Int(one.device_offline);break;
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
	writer.Key(level_gauging_str[i++]);writer.Double(one.forep_draft);
	writer.Key(level_gauging_str[i++]);writer.Double(one.fores_draft);
	writer.Key(level_gauging_str[i++]);writer.Double(one.aftp_draft);
	writer.Key(level_gauging_str[i++]);writer.Double(one.afts_draft);
	writer.Key(level_gauging_str[i++]);writer.Double(one.sw_ballast_tank1p_level);
	writer.Key(level_gauging_str[i++]);writer.Double(one.sw_ballast_tank1s_level);
	writer.Key(level_gauging_str[i++]);writer.Double(one.sw_ballast_tank2p_level);
	writer.Key(level_gauging_str[i++]);writer.Double(one.sw_ballast_tank2s_level);
	writer.Key(level_gauging_str[i++]);writer.Double(one.sw_ballast_tank3p_level);
	writer.Key(level_gauging_str[i++]);writer.Double(one.sw_ballast_tank3s_level);
	writer.Key(level_gauging_str[i++]);writer.Double(one.sw_ballast_tank3m_level);
	writer.Key(level_gauging_str[i++]);writer.Double(one.fresh_water_tank1p_level);
	writer.Key(level_gauging_str[i++]);writer.Double(one.fresh_water_tank1s_level);
	writer.Key(level_gauging_str[i++]);writer.Double(one.fresh_water_tank2p_level);
	writer.Key(level_gauging_str[i++]);writer.Double(one.fresh_water_tank2s_level);
	writer.Key(level_gauging_str[i++]);writer.Double(one.fuel_oil_store_tank_p_level);
	writer.Key(level_gauging_str[i++]);writer.Double(one.fuel_oil_store_tank_s_level);
	writer.Key(level_gauging_str[i++]);writer.Double(one.fuel_oil_day_tank_p_level);
	writer.Key(level_gauging_str[i++]);writer.Double(one.fuel_oil_day_tank_s_level);

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


void LEVEL_GAUGING::init_mb_addr()
{
	int addr_ai = 7500;
	for(int i = 0; i < 19; i++)
	{
		m_mp_delay_addr[level_gauging_str[i]] = addr_ai + 2 * i;
	}
	m_mp_delay_addr[level_gauging_str[34]] = 8009;
	/////////////////////////////////////////////////////
	for(int i = 0; i < 8; i++)
	{
		m_mp_inhibit_addr[level_gauging_str[i]] = 2 * i;
	}

	for(int i = 0; i < 8; i++)
	{
		m_mp_inhibit_addr[level_gauging_str[8 + i]] = 2 * i + 100;
	}

	for(int i = 0; i < 3; i++)
	{
		m_mp_inhibit_addr[level_gauging_str[16 + i]] = 2 * i + 200;
	}
	m_mp_inhibit_addr[level_gauging_str[34]] = 9;

	
}

void LEVEL_GAUGING::thSetRange()
{
	setRange();
}

void LEVEL_GAUGING::thSetThreshold()
{
	setThreshold();
}

void LEVEL_GAUGING::thSetDelay()
{
	setDelay();
}

void LEVEL_GAUGING::thSetInhibit()
{
	setInhibit(5200,3);
	while(1)
	{
		inhibit one = bq_inhibit.take();
		string key = one.m_keyname;
		bool inhib = one.is_inhibit;
		int ix = m_mp_inhibit_addr[key]/100;
		int iloop = m_mp_inhibit_addr[key]%100;
		int need = 3;
		int addr = 5200;
		uint16_t status[need]={0};
		if(!m_mb_plc1)
			continue;

		std::lock_guard<std::mutex> lock(*m_mux_plc1);
		if(m_mb_plc1->modbus_read_holding_registers(addr,need,status))
			continue;
		
		if(inhib) 
			status[ix] |= (1<<iloop);
		else
			status[ix] &= ~(1<<iloop);
		m_mb_plc1->modbus_write_register(addr+ix,status[ix]);

		if(m_mb_plc1->modbus_read_holding_registers(5800,1,status))
			continue;
		
		if(inhib) 
			status[0] |= (1<<iloop);
		else
			status[0] &= ~(1<<iloop);
		m_mb_plc1->modbus_write_register(5800,status[0]);		

	}	
}	






