#include <iostream>
#include <unistd.h>

#include "sgo_sub_pub.h"
#include "sgo_func.h"
#include "sgo_ams/gas_detection_system.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

using namespace std;

const char* gas_detection_system_str[]={
#ifndef MAHTS648
	"gas_detection_alarm_system_failure",
	"gas_detector_fault_top_of_wheelhouse",
	"gas_pre_alarm_top_of_wheelhouse",
	"gas_level1_alarm_top_of_wheelhouse",
	"gas_level2_alarm_top_of_wheelhouse",
	"gas_detector_disabled_top_of_wheelhouse",
	"gas_detector_fault_air_intake_of_b_deck_ahu",
	"gas_pre_alarm_air_intake_of_b_deck_ahu",
	"gas_level1_alarm_air_intake_of_b_deck_ahu",
	"gas_level2_alarm_air_intake_of_b_deck_ahu",
	"gas_detector_disabled_air_intake_of_b_deck_ahu",
	"gas_detector_fault_air_intake_of_a_deck_ahu",
	"gas_pre_alarm_air_intake_of_a_deck_ahu",
	"gas_level1_alarm_air_intake_of_a_deck_ahu",
	"gas_level2_alarm_air_intake_of_a_deck_ahu",
	"gas_detector_disabled_air_intake_of_a_deck_ahu",
	"gas_detector_fault_air_intake_of_below_main_deck_ahu",
	"gas_pre_alarm_air_intake_of_below_main_deck_ahu",
	"gas_level1_alarm_air_intake_of_below_main_deck_ahu",
	"gas_level2_alarm_air_intake_of_below_main_deck_ahu",
	"gas_detector_disabled_air_intake_of_below_main_deck_ahu",
	"gas_detector_fault_air_intake_of_bow_thruster_room",
	"gas_pre_alarm_air_intake_of_bow_thruster_room",
	"gas_level1_alarm_air_intake_of_bow_thruster_room",
	"gas_level2_alarm_air_intake_of_bow_thruster_room",
	"gas_detector_disabled_air_intake_of_bow_thruster_room",
	"gas_detector_fault_air_intake_of_aft_thruster_roomp",
	"gas_pre_alarm_air_intake_of_aft_thruster_roomp",
	"gas_level1_alarm_air_intake_of_aft_thruster_roomp",
	"gas_level2_alarm_air_intake_of_aft_thruster_roomp",
	"gas_detector_disabled_air_intake_of_aft_thruster_roomp",
	"gas_detector_fault_air_intake_of_aft_thruster_rooms",
	"gas_pre_alarm_air_intake_of_aft_thruster_rooms",
	"gas_level1_alarm_air_intake_of_aft_thruster_rooms",
	"gas_level2_alarm_air_intake_of_aft_thruster_rooms",
	"gas_detector_disabled_air_intake_of_aft_thruster_rooms",
	"gas_detector_fault_air_intake_of_engine_roomp",
	"gas_pre_alarm_air_intake_of_engine_roomp",
	"gas_level1_alarm_air_intake_of_engine_roomp",
	"gas_level2_alarm_air_intake_of_engine_roomp",
	"gas_detector_disabled_air_intake_of_engine_roomp",
	"gas_detector_fault_air_intake_of_engine_rooms",
	"gas_pre_alarm_air_intake_of_engine_rooms",
	"gas_level1_alarm_air_intake_of_engine_rooms",
	"gas_level2_alarm_air_intake_of_engine_rooms",
	"gas_detector_disabled_air_intake_of_engine_rooms",
	"gas_detector_fault_air_intake_of_aux_machinery_room1",
	"gas_pre_alarm_air_intake_of_aux_machinery_room1",
	"gas_level1_alarm_air_intake_of_aux_machinery_room1",
	"gas_level2_alarm_air_intake_of_aux_machinery_room1",
	"gas_detector_disabled_air_intake_of_aux_machinery_room1",
	"gas_detector_fault_air_intake_of_aux_machinery_room2",
	"gas_pre_alarm_air_intake_of_aux_machinery_room2",
	"gas_level1_alarm_air_intake_of_aux_machinery_room2",
	"gas_level2_alarm_air_intake_of_aux_machinery_room2",
	"gas_detector_disabled_air_intake_of_aux_machinery_room2",
	"gas_detector_fault_b_deck_entrancefr109p",
	"gas_pre_alarm_b_deck_entrancefr109p",
	"gas_level1_alarm_b_deck_entrancefr109p",
	"gas_level2_alarm_b_deck_entrancefr109p",
	"gas_detector_disabled_b_deck_entrancefr109p",
	"gas_detector_fault_b_deck_entrancefr109s",
	"gas_pre_alarm_b_deck_entrancefr109s",
	"gas_level1_alarm_b_deck_entrancefr109s",
	"gas_level2_alarm_b_deck_entrancefr109s",
	"gas_detector_disabled_b_deck_entrancefr109s",
	"gas_detector_fault_wheelhouse_entrancep",
	"gas_pre_alarm_wheelhouse_entrancep",
	"gas_level1_alarm_wheelhouse_entrancep",
	"gas_level2_alarm_wheelhouse_entrancep",
	"gas_detector_disabled_wheelhouse_entrancep",
	"gas_detector_fault_wheelhouse_entrances",
	"gas_pre_alarm_wheelhouse_entrances",
	"gas_level1_alarm_wheelhouse_entrances",
	"gas_level2_alarm_wheelhouse_entrances",
	"gas_detector_disabled_wheelhouse_entrances",
	"gas_detector_fault_b_deck_entrancefr90p",
	"gas_pre_alarm_b_deck_entrancefr90p",
	"gas_level1_alarm_b_deck_entrancefr90p",
	"gas_level2_alarm_b_deck_entrancefr90p",
	"gas_detector_disabled_b_deck_entrancefr90p",
	"gas_detector_fault_b_deck_entrancefr90s",
	"gas_pre_alarm_b_deck_entrancefr90s",
	"gas_level1_alarm_b_deck_entrancefr90s",
	"gas_level2_alarm_b_deck_entrancefr90s",
	"gas_detector_disabled_b_deck_entrancefr90s",
	"gas_detector_fault_a_deck_entrancefr109p",
	"gas_pre_alarm_a_deck_entrancefr109p",
	"gas_level1_alarm_a_deck_entrancefr109p",
	"gas_level2_alarm_a_deck_entrancefr109p",
	"gas_detector_disabled_a_deck_entrancefr109p",
	"gas_detector_fault_a_deck_entrancefr109s",
	"gas_pre_alarm_a_deck_entrancefr109s",
	"gas_level1_alarm_a_deck_entrancefr109s",
	"gas_level2_alarm_a_deck_entrancefr109s",
	"gas_detector_disabled_a_deck_entrancefr109s",
	"gas_detector_fault_a_deck_entrancefr90p",
	"gas_pre_alarm_a_deck_entrancefr90p",
	"gas_level1_alarm_a_deck_entrancefr90p",
	"gas_level2_alarm_a_deck_entrancefr90p",
	"gas_detector_disabled_a_deck_entrancefr90p",
	"gas_detector_fault_a_deck_entrancefr90s",
	"gas_pre_alarm_a_deck_entrancefr90s",
	"gas_level1_alarm_a_deck_entrancefr90s",
	"gas_level2_alarm_a_deck_entrancefr90sp",
	"gas_detector_disabled_a_deck_entrancefr90s",
	"gas_detector_fault_main_deck_entrancefr81m",
	"gas_pre_alarm_main_deck_entrancefr81m",
	"gas_level1_alarm_main_deck_entrancefr81m",
	"gas_level2_alarm_main_deck_entrancefr81m",
	"gas_detector_disabled_main_deck_entrancefr81m",
	"gas_detector_fault_main_deck_entrancefr81s",
	"gas_pre_alarm_main_deck_entrancefr81s",
	"gas_level1_alarm_main_deck_entrancefr81s",
	"gas_level2_alarm_main_deck_entrancefr81s",
	"gas_detector_disabled_main_deck_entrancefr81s",
	"gas_detector_fault_air_intake_of_construction_equipment_room",
	"gas_pre_alarm_air_intake_of_construction_equipment_room",
	"gas_level1_alarm_air_intake_of_construction_equipment_room",
	"gas_level2_alarm_air_intake_of_construction_equipment_room",
	"gas_detector_disabled_air_intake_of_construction_equipment_room",
	"toxic_gas_detector_fault_top_of_wheelhouse",
	"toxic_gas_pre_alarm_top_of_wheelhouse",
	"toxic_gas_level1_alarm_top_of_wheelhouse",
	"toxic_gas_level2_alarm_top_of_wheelhouse",
	"toxic_gas_detector_disabled_top_of_wheelhouse",
	"toxic_gas_detector_fault_air_intake_of_b_deck_ahu",
	"toxic_gas_pre_alarm_air_intake_of_b_deck_ahu",
	"toxic_gas_level1_alarm_air_intake_of_b_deck_ahu",
	"toxic_gas_level2_alarm_air_intake_of_b_deck_ahu",
	"toxic_gas_detector_disabled_air_intake_of_b_deck_ahu",
	"toxic_gas_detector_fault_air_intake_of_a_deck_ahu",
	"toxic_gas_pre_alarm_air_intake_of_a_deck_ahu",
	"toxic_gas_level1_alarm_air_intake_of_a_deck_ahu",
	"toxic_gas_level2_alarm_air_intake_of_a_deck_ahu",
	"toxic_gas_detector_disabled_air_intake_of_a_deck_ahu",
	"toxic_gas_detector_fault_air_intake_of_below_main_deck_ahu",
	"toxic_gas_pre_alarm_air_intake_of_below_main_deck_ahu",
	"toxic_gas_level1_alarm_air_intake_of_below_main_deck_ahu",
	"toxic_gas_level2_alarm_air_intake_of_below_main_deck_ahu",
	"toxic_gas_detector_disabled_air_intake_of_below_main_deck_ahu",
	"toxic_gas_detector_fault_air_intake_of_bow_thruster_room",
	"toxic_gas_pre_alarm_air_intake_of_bow_thruster_room",
	"toxic_gas_level1_alarm_air_intake_of_bow_thruster_room",
	"toxic_gas_level2_alarm_air_intake_of_bow_thruster_room",
	"toxic_gas_detector_disabled_air_intake_of_bow_thruster_room",
	"toxic_gas_detector_fault_air_intake_of_aft_thruster_roomp",
	"toxic_gas_pre_alarm_air_intake_of_aft_thruster_roomp",
	"toxic_gas_level1_alarm_air_intake_of_aft_thruster_roomp",
	"toxic_gas_level2_alarm_air_intake_of_aft_thruster_roomp",
	"toxic_gas_detector_disabled_air_intake_of_aft_thruster_roomp",
	"toxic_gas_detector_fault_air_intake_of_aft_thruster_rooms",
	"toxic_gas_pre_alarm_air_intake_of_aft_thruster_rooms",
	"toxic_gas_level1_alarm_air_intake_of_aft_thruster_rooms",
	"toxic_gas_level2_alarm_air_intake_of_aft_thruster_rooms",
	"toxic_gas_detector_disabled_air_intake_of_aft_thruster_rooms",
	"toxic_gas_detector_fault_air_intake_of_engine_roomp",
	"toxic_gas_pre_alarm_air_intake_of_engine_roomp",
	"toxic_gas_level1_alarm_air_intake_of_engine_roomp",
	"toxic_gas_level2_alarm_air_intake_of_engine_roomp",
	"toxic_gas_detector_disabled_air_intake_of_engine_roomp",
	"toxic_gas_detector_fault_air_intake_of_engine_rooms",
	"toxic_gas_pre_alarm_air_intake_of_engine_rooms",
	"toxic_gas_level1_alarm_air_intake_of_engine_rooms",
	"toxic_gas_level2_alarm_air_intake_of_engine_rooms",
	"toxic_gas_detector_disabled_air_intake_of_engine_rooms",
	"toxic_gas_detector_fault_air_intake_of_aux_machinery_room1",
	"toxic_gas_pre_alarm_air_intake_of_aux_machinery_room1",
	"toxic_gas_level1_alarm_air_intake_of_aux_machinery_room1",
	"toxic_gas_level2_alarm_air_intake_of_aux_machinery_room1",
	"toxic_gas_detector_disabled_air_intake_of_aux_machinery_room1",
	"toxic_gas_detector_fault_air_intake_of_aux_machinery_room2",
	"toxic_gas_pre_alarm_air_intake_of_aux_machinery_room2",
	"toxic_gas_level1_alarm_air_intake_of_aux_machinery_room2",
	"toxic_gas_level2_alarm_air_intake_of_aux_machinery_room2",
	"toxic_gas_detector_disabled_air_intake_of_aux_machinery_room2",
	"toxic_gas_detector_fault_b_deck_entrancefr109p",
	"toxic_gas_pre_alarm_b_deck_entrancefr109p",
	"toxic_gas_level1_alarm_b_deck_entrancefr109p",
	"toxic_gas_level2_alarm_b_deck_entrancefr109p",
	"toxic_gas_detector_disabled_b_deck_entrancefr109p",
	"toxic_gas_detector_fault_b_deck_entrancefr109s",
	"toxic_gas_pre_alarm_b_deck_entrancefr109s",
	"toxic_gas_level1_alarm_b_deck_entrancefr109s",
	"toxic_gas_level2_alarm_b_deck_entrancefr109s",
	"toxic_gas_detector_disabled_b_deck_entrancefr109s",
	"device_offline",
#else
"co2_power_failure_alarm",
"co2_pre_release_alarm",
"co2_release_alarm",
"co2_leakage_alarm",
"device_offline",


#endif
};


	
static const char* productCode = "gas_detection_system";

GAS_DETECTION_SYSTEM::GAS_DETECTION_SYSTEM()
{}

GAS_DETECTION_SYSTEM::~GAS_DETECTION_SYSTEM()
{}

void GAS_DETECTION_SYSTEM::init(int devId,BlockQueue<gas_detection_system> &bq,modbus* mb,mutex* mux)
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
void GAS_DETECTION_SYSTEM::connectionLostCallback(void* context, char* cause) 
{
	// 处理连接丢失事件
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	GAS_DETECTION_SYSTEM* client = static_cast<GAS_DETECTION_SYSTEM*>(ctx->manager);
	
	printf("--%s %d--address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());
	sgo_log_stderr(0,"--%s %d----address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());

	//std::lock_guard<std::mutex> lock(m_losts_mutex);
	client->m_v_lost[ctx->index] = 1;
	client->m_v_property_index_array.clear();
}


// 消息到达回调函数
int GAS_DETECTION_SYSTEM::messageArrivedCallback(void* context, char* topicName, int topicLen,MQTTClient_message* message) 
{
	// 处理接收到的消息
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	GAS_DETECTION_SYSTEM* client = static_cast<GAS_DETECTION_SYSTEM*>(ctx->manager);
	
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
			client->m_property[gas_detection_system_str[i]]=i;
		
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
void GAS_DETECTION_SYSTEM::deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt) 
{
	// 处理交付完成事件
	//std::cout << "Message delivery complete." << std::endl;
}

void GAS_DETECTION_SYSTEM::subClient()
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


void GAS_DETECTION_SYSTEM::pubClient(BlockQueue<gas_detection_system>& bq)
{
	while(1)
	{
		MQTTClient_message pubmsg = MQTTClient_message_initializer;
		//声明消息token
		MQTTClient_deliveryToken token;

		while(!m_pubStop){
			gas_detection_system one = bq.take();
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




void GAS_DETECTION_SYSTEM::start(BlockQueue<gas_detection_system> &bq)
{
	m_sub = new thread(&GAS_DETECTION_SYSTEM::subClient,this);
	m_sub->detach();

	m_pub = new thread(&GAS_DETECTION_SYSTEM::pubClient,this,ref(bq));
	m_pub->detach();

	m_thr_reconn = new thread(&GAS_DETECTION_SYSTEM::reconn,this);
	m_thr_reconn->detach();

	m_thr_resend = new thread(&GAS_DETECTION_SYSTEM::resnd,this);
	m_thr_resend->detach();

	m_thr_chksensor = new thread(&GAS_DETECTION_SYSTEM::chksensor,this);
	m_thr_chksensor->detach();

	thread *thr_inhibit = new thread(&GAS_DETECTION_SYSTEM::thSetInhibit,this);
	thr_inhibit->detach();

	thread *thr_delay = new thread(&GAS_DETECTION_SYSTEM::thSetDelay,this);
	thr_delay->detach();		
}

void GAS_DETECTION_SYSTEM::chksensor()
{
	checkSensorStatus();
}

void GAS_DETECTION_SYSTEM::resnd()
{
	resend();
}
	

void GAS_DETECTION_SYSTEM::reconn()
{
	reconnect();
}


string GAS_DETECTION_SYSTEM::property_reply(char flag,const gas_detection_system& one,const string& msg_id)
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
		case 0: writer.Int(one.gas_detection_alarm_system_failure); break;
		case 1: writer.Int(one.gas_detector_fault_top_of_wheelhouse); break;
		case 2: writer.Int(one.gas_pre_alarm_top_of_wheelhouse); break;
		case 3: writer.Int(one.gas_level1_alarm_top_of_wheelhouse); break;
		case 4: writer.Int(one.gas_level2_alarm_top_of_wheelhouse); break;
		case 5: writer.Int(one.gas_detector_disabled_top_of_wheelhouse); break;
		case 6: writer.Int(one.gas_detector_fault_air_intake_of_b_deck_ahu); break;
		case 7: writer.Int(one.gas_pre_alarm_air_intake_of_b_deck_ahu); break;
		case 8: writer.Int(one.gas_level1_alarm_air_intake_of_b_deck_ahu); break;
		case 9: writer.Int(one.gas_level2_alarm_air_intake_of_b_deck_ahu); break;
		case 10: writer.Int(one.gas_detector_disabled_air_intake_of_b_deck_ahu); break;
		case 11: writer.Int(one.gas_detector_fault_air_intake_of_a_deck_ahu); break;
		case 12: writer.Int(one.gas_pre_alarm_air_intake_of_a_deck_ahu); break;
		case 13: writer.Int(one.gas_level1_alarm_air_intake_of_a_deck_ahu); break;
		case 14: writer.Int(one.gas_level2_alarm_air_intake_of_a_deck_ahu); break;
		case 15: writer.Int(one.gas_detector_disabled_air_intake_of_a_deck_ahu); break;
		case 16: writer.Int(one.gas_detector_fault_air_intake_of_below_main_deck_ahu); break;
		case 17: writer.Int(one.gas_pre_alarm_air_intake_of_below_main_deck_ahu); break;
		case 18: writer.Int(one.gas_level1_alarm_air_intake_of_below_main_deck_ahu); break;
		case 19: writer.Int(one.gas_level2_alarm_air_intake_of_below_main_deck_ahu); break;
		case 20: writer.Int(one.gas_detector_disabled_air_intake_of_below_main_deck_ahu); break;
		case 21: writer.Int(one.gas_detector_fault_air_intake_of_bow_thruster_room); break;
		case 22: writer.Int(one.gas_pre_alarm_air_intake_of_bow_thruster_room); break;
		case 23: writer.Int(one.gas_level1_alarm_air_intake_of_bow_thruster_room); break;
		case 24: writer.Int(one.gas_level2_alarm_air_intake_of_bow_thruster_room); break;
		case 25: writer.Int(one.gas_detector_disabled_air_intake_of_bow_thruster_room); break;
		case 26: writer.Int(one.gas_detector_fault_air_intake_of_aft_thruster_roomp); break;
		case 27: writer.Int(one.gas_pre_alarm_air_intake_of_aft_thruster_roomp); break;
		case 28: writer.Int(one.gas_level1_alarm_air_intake_of_aft_thruster_roomp); break;
		case 29: writer.Int(one.gas_level2_alarm_air_intake_of_aft_thruster_roomp); break;
		case 30: writer.Int(one.gas_detector_disabled_air_intake_of_aft_thruster_roomp); break;
		case 31: writer.Int(one.gas_detector_fault_air_intake_of_aft_thruster_rooms); break;
		case 32: writer.Int(one.gas_pre_alarm_air_intake_of_aft_thruster_rooms); break;
		case 33: writer.Int(one.gas_level1_alarm_air_intake_of_aft_thruster_rooms); break;
		case 34: writer.Int(one.gas_level2_alarm_air_intake_of_aft_thruster_rooms); break;
		case 35: writer.Int(one.gas_detector_disabled_air_intake_of_aft_thruster_rooms); break;
		case 36: writer.Int(one.gas_detector_fault_air_intake_of_engine_roomp); break;
		case 37: writer.Int(one.gas_pre_alarm_air_intake_of_engine_roomp); break;
		case 38: writer.Int(one.gas_level1_alarm_air_intake_of_engine_roomp); break;
		case 39: writer.Int(one.gas_level2_alarm_air_intake_of_engine_roomp); break;
		case 40: writer.Int(one.gas_detector_disabled_air_intake_of_engine_roomp); break;
		case 41: writer.Int(one.gas_detector_fault_air_intake_of_engine_rooms); break;
		case 42: writer.Int(one.gas_pre_alarm_air_intake_of_engine_rooms); break;
		case 43: writer.Int(one.gas_level1_alarm_air_intake_of_engine_rooms); break;
		case 44: writer.Int(one.gas_level2_alarm_air_intake_of_engine_rooms); break;
		case 45: writer.Int(one.gas_detector_disabled_air_intake_of_engine_rooms); break;
		case 46: writer.Int(one.gas_detector_fault_air_intake_of_aux_machinery_room1); break;
		case 47: writer.Int(one.gas_pre_alarm_air_intake_of_aux_machinery_room1); break;
		case 48: writer.Int(one.gas_level1_alarm_air_intake_of_aux_machinery_room1); break;
		case 49: writer.Int(one.gas_level2_alarm_air_intake_of_aux_machinery_room1); break;
		case 50: writer.Int(one.gas_detector_disabled_air_intake_of_aux_machinery_room1); break;
		case 51: writer.Int(one.gas_detector_fault_air_intake_of_aux_machinery_room2); break;
		case 52: writer.Int(one.gas_pre_alarm_air_intake_of_aux_machinery_room2); break;
		case 53: writer.Int(one.gas_level1_alarm_air_intake_of_aux_machinery_room2); break;
		case 54: writer.Int(one.gas_level2_alarm_air_intake_of_aux_machinery_room2); break;
		case 55: writer.Int(one.gas_detector_disabled_air_intake_of_aux_machinery_room2); break;
		case 56: writer.Int(one.gas_detector_fault_b_deck_entrancefr109p); break;
		case 57: writer.Int(one.gas_pre_alarm_b_deck_entrancefr109p); break;
		case 58: writer.Int(one.gas_level1_alarm_b_deck_entrancefr109p); break;
		case 59: writer.Int(one.gas_level2_alarm_b_deck_entrancefr109p); break;
		case 60: writer.Int(one.gas_detector_disabled_b_deck_entrancefr109p); break;
		case 61: writer.Int(one.gas_detector_fault_b_deck_entrancefr109s); break;
		case 62: writer.Int(one.gas_pre_alarm_b_deck_entrancefr109s); break;
		case 63: writer.Int(one.gas_level1_alarm_b_deck_entrancefr109s); break;
		case 64: writer.Int(one.gas_level2_alarm_b_deck_entrancefr109s); break;
		case 65: writer.Int(one.gas_detector_disabled_b_deck_entrancefr109s); break;
		case 66: writer.Int(one.gas_detector_fault_wheelhouse_entrancep); break;
		case 67: writer.Int(one.gas_pre_alarm_wheelhouse_entrancep); break;
		case 68: writer.Int(one.gas_level1_alarm_wheelhouse_entrancep); break;
		case 69: writer.Int(one.gas_level2_alarm_wheelhouse_entrancep); break;
		case 70: writer.Int(one.gas_detector_disabled_wheelhouse_entrancep); break;
		case 71: writer.Int(one.gas_detector_fault_wheelhouse_entrances); break;
		case 72: writer.Int(one.gas_pre_alarm_wheelhouse_entrances); break;
		case 73: writer.Int(one.gas_level1_alarm_wheelhouse_entrances); break;
		case 74: writer.Int(one.gas_level2_alarm_wheelhouse_entrances); break;
		case 75: writer.Int(one.gas_detector_disabled_wheelhouse_entrances); break;

#ifdef JANA505
		case 76: writer.Int(one.gas_detector_fault_b_deck_entrancefr90p); break;
		case 77: writer.Int(one.gas_pre_alarm_b_deck_entrancefr90p); break;
		case 78: writer.Int(one.gas_level1_alarm_b_deck_entrancefr90p); break;
		case 79: writer.Int(one.gas_level2_alarm_b_deck_entrancefr90p); break;
		case 80: writer.Int(one.gas_detector_disabled_b_deck_entrancefr90p); break;
		case 81: writer.Int(one.gas_detector_fault_b_deck_entrancefr90s); break;
		case 82: writer.Int(one.gas_pre_alarm_b_deck_entrancefr90s); break;
		case 83: writer.Int(one.gas_level1_alarm_b_deck_entrancefr90s); break;
		case 84: writer.Int(one.gas_level2_alarm_b_deck_entrancefr90s); break;
		case 85: writer.Int(one.gas_detector_disabled_b_deck_entrancefr90s); break;
#endif
#ifdef JANA508509		
		case 76: writer.Int(one.gas_detector_fault_b_deck_entrancefr100p); break;
		case 77: writer.Int(one.gas_pre_alarm_b_deck_entrancefr100p); break;
		case 78: writer.Int(one.gas_level1_alarm_b_deck_entrancefr100p); break;
		case 79: writer.Int(one.gas_level2_alarm_b_deck_entrancefr100p); break;
		case 80: writer.Int(one.gas_detector_disabled_b_deck_entrancefr100p); break;
		case 81: writer.Int(one.gas_detector_fault_b_deck_entrancefr100s); break;
		case 82: writer.Int(one.gas_pre_alarm_b_deck_entrancefr100s); break;
		case 83: writer.Int(one.gas_level1_alarm_b_deck_entrancefr100s); break;
		case 84: writer.Int(one.gas_level2_alarm_b_deck_entrancefr100s); break;
		case 85: writer.Int(one.gas_detector_disabled_b_deck_entrancefr100s); break;
#endif


		
		case 86: writer.Int(one.gas_detector_fault_a_deck_entrancefr109p); break;
		case 87: writer.Int(one.gas_pre_alarm_a_deck_entrancefr109p); break;
		case 88: writer.Int(one.gas_level1_alarm_a_deck_entrancefr109p); break;
		case 89: writer.Int(one.gas_level2_alarm_a_deck_entrancefr109p); break;
		case 90: writer.Int(one.gas_detector_disabled_a_deck_entrancefr109p); break;
		case 91: writer.Int(one.gas_detector_fault_a_deck_entrancefr109s); break;
		case 92: writer.Int(one.gas_pre_alarm_a_deck_entrancefr109s); break;
		case 93: writer.Int(one.gas_level1_alarm_a_deck_entrancefr109s); break;
		case 94: writer.Int(one.gas_level2_alarm_a_deck_entrancefr109s); break;
		case 95: writer.Int(one.gas_detector_disabled_a_deck_entrancefr109s); break;
#ifdef JANA505
		case 96: writer.Int(one.gas_detector_fault_a_deck_entrancefr90p); break;
		case 97: writer.Int(one.gas_pre_alarm_a_deck_entrancefr90p); break;
		case 98: writer.Int(one.gas_level1_alarm_a_deck_entrancefr90p); break;
		case 99: writer.Int(one.gas_level2_alarm_a_deck_entrancefr90p); break;
		case 100: writer.Int(one.gas_detector_disabled_a_deck_entrancefr90p); break;
		case 101: writer.Int(one.gas_detector_fault_a_deck_entrancefr90s); break;
		case 102: writer.Int(one.gas_pre_alarm_a_deck_entrancefr90s); break;
		case 103: writer.Int(one.gas_level1_alarm_a_deck_entrancefr90s); break;
		case 104: writer.Int(one.gas_level2_alarm_a_deck_entrancefr90sp); break;
		case 105: writer.Int(one.gas_detector_disabled_a_deck_entrancefr90s); break;
		case 106: writer.Int(one.gas_detector_fault_main_deck_entrancefr81m); break;
		case 107: writer.Int(one.gas_pre_alarm_main_deck_entrancefr81m); break;
		case 108: writer.Int(one.gas_level1_alarm_main_deck_entrancefr81m); break;
		case 109: writer.Int(one.gas_level2_alarm_main_deck_entrancefr81m); break;
		case 110: writer.Int(one.gas_detector_disabled_main_deck_entrancefr81m); break;
		case 111: writer.Int(one.gas_detector_fault_main_deck_entrancefr81s); break;
		case 112: writer.Int(one.gas_pre_alarm_main_deck_entrancefr81s); break;
		case 113: writer.Int(one.gas_level1_alarm_main_deck_entrancefr81s); break;
		case 114: writer.Int(one.gas_level2_alarm_main_deck_entrancefr81s); break;
		case 115: writer.Int(one.gas_detector_disabled_main_deck_entrancefr81s); break;
		case 116: writer.Int(one.gas_detector_fault_air_intake_of_construction_equipment_room); break;
		case 117: writer.Int(one.gas_pre_alarm_air_intake_of_construction_equipment_room); break;
		case 118: writer.Int(one.gas_level1_alarm_air_intake_of_construction_equipment_room); break;
		case 119: writer.Int(one.gas_level2_alarm_air_intake_of_construction_equipment_room); break;
		case 120: writer.Int(one.gas_detector_disabled_air_intake_of_construction_equipment_room); break;

		case 121: writer.Int(one.toxic_gas_detector_fault_top_of_wheelhouse); break;
		case 122: writer.Int(one.toxic_gas_pre_alarm_top_of_wheelhouse); break;
		case 123: writer.Int(one.toxic_gas_level1_alarm_top_of_wheelhouse); break;
		case 124: writer.Int(one.toxic_gas_level2_alarm_top_of_wheelhouse); break;
		case 125: writer.Int(one.toxic_gas_detector_disabled_top_of_wheelhouse); break;
		case 126: writer.Int(one.toxic_gas_detector_fault_air_intake_of_b_deck_ahu); break;
		case 127: writer.Int(one.toxic_gas_pre_alarm_air_intake_of_b_deck_ahu); break;
		case 128: writer.Int(one.toxic_gas_level1_alarm_air_intake_of_b_deck_ahu); break;
		case 129: writer.Int(one.toxic_gas_level2_alarm_air_intake_of_b_deck_ahu); break;
		case 130: writer.Int(one.toxic_gas_detector_disabled_air_intake_of_b_deck_ahu); break;
		case 131: writer.Int(one.toxic_gas_detector_fault_air_intake_of_a_deck_ahu); break;
		case 132: writer.Int(one.toxic_gas_pre_alarm_air_intake_of_a_deck_ahu); break;
		case 133: writer.Int(one.toxic_gas_level1_alarm_air_intake_of_a_deck_ahu); break;
		case 134: writer.Int(one.toxic_gas_level2_alarm_air_intake_of_a_deck_ahu); break;
		case 135: writer.Int(one.toxic_gas_detector_disabled_air_intake_of_a_deck_ahu); break;
		case 136: writer.Int(one.toxic_gas_detector_fault_air_intake_of_below_main_deck_ahu); break;
		case 137: writer.Int(one.toxic_gas_pre_alarm_air_intake_of_below_main_deck_ahu); break;
		case 138: writer.Int(one.toxic_gas_level1_alarm_air_intake_of_below_main_deck_ahu); break;
		case 139: writer.Int(one.toxic_gas_level2_alarm_air_intake_of_below_main_deck_ahu); break;
		case 140: writer.Int(one.toxic_gas_detector_disabled_air_intake_of_below_main_deck_ahu); break;
		case 141: writer.Int(one.toxic_gas_detector_fault_air_intake_of_bow_thruster_room); break;
		case 142: writer.Int(one.toxic_gas_pre_alarm_air_intake_of_bow_thruster_room); break;
		case 143: writer.Int(one.toxic_gas_level1_alarm_air_intake_of_bow_thruster_room); break;
		case 144: writer.Int(one.toxic_gas_level2_alarm_air_intake_of_bow_thruster_room); break;
		case 145: writer.Int(one.toxic_gas_detector_disabled_air_intake_of_bow_thruster_room); break;
		case 146: writer.Int(one.toxic_gas_detector_fault_air_intake_of_aft_thruster_roomp); break;
		case 147: writer.Int(one.toxic_gas_pre_alarm_air_intake_of_aft_thruster_roomp); break;
		case 148: writer.Int(one.toxic_gas_level1_alarm_air_intake_of_aft_thruster_roomp); break;
		case 149: writer.Int(one.toxic_gas_level2_alarm_air_intake_of_aft_thruster_roomp); break;
		case 150: writer.Int(one.toxic_gas_detector_disabled_air_intake_of_aft_thruster_roomp); break;
		case 151: writer.Int(one.toxic_gas_detector_fault_air_intake_of_aft_thruster_rooms); break;
		case 152: writer.Int(one.toxic_gas_pre_alarm_air_intake_of_aft_thruster_rooms); break;
		case 153: writer.Int(one.toxic_gas_level1_alarm_air_intake_of_aft_thruster_rooms); break;
		case 154: writer.Int(one.toxic_gas_level2_alarm_air_intake_of_aft_thruster_rooms); break;
		case 155: writer.Int(one.toxic_gas_detector_disabled_air_intake_of_aft_thruster_rooms); break;
		case 156: writer.Int(one.toxic_gas_detector_fault_air_intake_of_engine_roomp); break;
		case 157: writer.Int(one.toxic_gas_pre_alarm_air_intake_of_engine_roomp); break;
		case 158: writer.Int(one.toxic_gas_level1_alarm_air_intake_of_engine_roomp); break;
		case 159: writer.Int(one.toxic_gas_level2_alarm_air_intake_of_engine_roomp); break;
		case 160: writer.Int(one.toxic_gas_detector_disabled_air_intake_of_engine_roomp); break;
		case 161: writer.Int(one.toxic_gas_detector_fault_air_intake_of_engine_rooms); break;
		case 162: writer.Int(one.toxic_gas_pre_alarm_air_intake_of_engine_rooms); break;
		case 163: writer.Int(one.toxic_gas_level1_alarm_air_intake_of_engine_rooms); break;
		case 164: writer.Int(one.toxic_gas_level2_alarm_air_intake_of_engine_rooms); break;
		case 165: writer.Int(one.toxic_gas_detector_disabled_air_intake_of_engine_rooms); break;
		case 166: writer.Int(one.toxic_gas_detector_fault_air_intake_of_aux_machinery_room1); break;
		case 167: writer.Int(one.toxic_gas_pre_alarm_air_intake_of_aux_machinery_room1); break;
		case 168: writer.Int(one.toxic_gas_level1_alarm_air_intake_of_aux_machinery_room1); break;
		case 169: writer.Int(one.toxic_gas_level2_alarm_air_intake_of_aux_machinery_room1); break;
		case 170: writer.Int(one.toxic_gas_detector_disabled_air_intake_of_aux_machinery_room1); break;
		case 171: writer.Int(one.toxic_gas_detector_fault_air_intake_of_aux_machinery_room2); break;
		case 172: writer.Int(one.toxic_gas_pre_alarm_air_intake_of_aux_machinery_room2); break;
		case 173: writer.Int(one.toxic_gas_level1_alarm_air_intake_of_aux_machinery_room2); break;
		case 174: writer.Int(one.toxic_gas_level2_alarm_air_intake_of_aux_machinery_room2); break;
		case 175: writer.Int(one.toxic_gas_detector_disabled_air_intake_of_aux_machinery_room2); break;
		case 176: writer.Int(one.toxic_gas_detector_fault_b_deck_entrancefr109p); break;
		case 177: writer.Int(one.toxic_gas_pre_alarm_b_deck_entrancefr109p); break;
		case 178: writer.Int(one.toxic_gas_level1_alarm_b_deck_entrancefr109p); break;
		case 179: writer.Int(one.toxic_gas_level2_alarm_b_deck_entrancefr109p); break;
		case 180: writer.Int(one.toxic_gas_detector_disabled_b_deck_entrancefr109p); break;
		case 181: writer.Int(one.toxic_gas_detector_fault_b_deck_entrancefr109s); break;
		case 182: writer.Int(one.toxic_gas_pre_alarm_b_deck_entrancefr109s); break;
		case 183: writer.Int(one.toxic_gas_level1_alarm_b_deck_entrancefr109s); break;
		case 184: writer.Int(one.toxic_gas_level2_alarm_b_deck_entrancefr109s); break;
		case 185: writer.Int(one.toxic_gas_detector_disabled_b_deck_entrancefr109s); break;
		case 186: writer.Int(one.device_offline); break;
#endif		
#ifdef JANA508509		
		case 96: writer.Int(one.gas_detector_fault_a_deck_entrancefr91p); break;
		case 97: writer.Int(one.gas_pre_alarm_a_deck_entrancefr91p); break;
		case 98: writer.Int(one.gas_level1_alarm_a_deck_entrancefr91p); break;
		case 99: writer.Int(one.gas_level2_alarm_a_deck_entrancefr91p); break;
		case 100: writer.Int(one.gas_detector_disabled_a_deck_entrancefr91p); break;
		case 101: writer.Int(one.gas_detector_fault_a_deck_entrancefr91s); break;
		case 102: writer.Int(one.gas_pre_alarm_a_deck_entrancefr91s); break;
		case 103: writer.Int(one.gas_level1_alarm_a_deck_entrancefr91s); break;
		case 104: writer.Int(one.gas_level2_alarm_a_deck_entrancefr91sp); break;
		case 105: writer.Int(one.gas_detector_disabled_a_deck_entrancefr91s); break;
		case 106: writer.Int(one.gas_detector_fault_main_deck_entrancefr91m); break;
		case 107: writer.Int(one.gas_pre_alarm_main_deck_entrancefr91m); break;
		case 108: writer.Int(one.gas_level1_alarm_main_deck_entrancefr91m); break;
		case 109: writer.Int(one.gas_level2_alarm_main_deck_entrancefr91m); break;
		case 110: writer.Int(one.gas_detector_disabled_main_deck_entrancefr91m); break;
		case 111: writer.Int(one.gas_detector_fault_main_deck_entrancefr91s); break;
		case 112: writer.Int(one.gas_pre_alarm_main_deck_entrancefr91s); break;
		case 113: writer.Int(one.gas_level1_alarm_main_deck_entrancefr91s); break;
		case 114: writer.Int(one.gas_level2_alarm_main_deck_entrancefr91s); break;
		case 115: writer.Int(one.gas_detector_disabled_main_deck_entrancefr91s); break;

		case 116: writer.Int(one.toxic_gas_detector_fault_top_of_wheelhouse); break;
		case 117: writer.Int(one.toxic_gas_pre_alarm_top_of_wheelhouse); break;
		case 118: writer.Int(one.toxic_gas_level1_alarm_top_of_wheelhouse); break;
		case 119: writer.Int(one.toxic_gas_level2_alarm_top_of_wheelhouse); break;
		case 120: writer.Int(one.toxic_gas_detector_disabled_top_of_wheelhouse); break;
		case 121: writer.Int(one.toxic_gas_detector_fault_air_intake_of_b_deck_ahu); break;
		case 122: writer.Int(one.toxic_gas_pre_alarm_air_intake_of_b_deck_ahu); break;
		case 123: writer.Int(one.toxic_gas_level1_alarm_air_intake_of_b_deck_ahu); break;
		case 124: writer.Int(one.toxic_gas_level2_alarm_air_intake_of_b_deck_ahu); break;
		case 125: writer.Int(one.toxic_gas_detector_disabled_air_intake_of_b_deck_ahu); break;
		case 126: writer.Int(one.toxic_gas_detector_fault_air_intake_of_a_deck_ahu); break;
		case 127: writer.Int(one.toxic_gas_pre_alarm_air_intake_of_a_deck_ahu); break;
		case 128: writer.Int(one.toxic_gas_level1_alarm_air_intake_of_a_deck_ahu); break;
		case 129: writer.Int(one.toxic_gas_level2_alarm_air_intake_of_a_deck_ahu); break;
		case 130: writer.Int(one.toxic_gas_detector_disabled_air_intake_of_a_deck_ahu); break;
		case 131: writer.Int(one.toxic_gas_detector_fault_air_intake_of_below_main_deck_ahu); break;
		case 132: writer.Int(one.toxic_gas_pre_alarm_air_intake_of_below_main_deck_ahu); break;
		case 133: writer.Int(one.toxic_gas_level1_alarm_air_intake_of_below_main_deck_ahu); break;
		case 134: writer.Int(one.toxic_gas_level2_alarm_air_intake_of_below_main_deck_ahu); break;
		case 135: writer.Int(one.toxic_gas_detector_disabled_air_intake_of_below_main_deck_ahu); break;
		case 136: writer.Int(one.toxic_gas_detector_fault_air_intake_of_bow_thruster_room); break;
		case 137: writer.Int(one.toxic_gas_pre_alarm_air_intake_of_bow_thruster_room); break;
		case 138: writer.Int(one.toxic_gas_level1_alarm_air_intake_of_bow_thruster_room); break;
		case 139: writer.Int(one.toxic_gas_level2_alarm_air_intake_of_bow_thruster_room); break;
		case 140: writer.Int(one.toxic_gas_detector_disabled_air_intake_of_bow_thruster_room); break;
		case 141: writer.Int(one.toxic_gas_detector_fault_air_intake_of_aft_thruster_roomp); break;
		case 142: writer.Int(one.toxic_gas_pre_alarm_air_intake_of_aft_thruster_roomp); break;
		case 143: writer.Int(one.toxic_gas_level1_alarm_air_intake_of_aft_thruster_roomp); break;
		case 144: writer.Int(one.toxic_gas_level2_alarm_air_intake_of_aft_thruster_roomp); break;
		case 145: writer.Int(one.toxic_gas_detector_disabled_air_intake_of_aft_thruster_roomp); break;
		case 146: writer.Int(one.toxic_gas_detector_fault_air_intake_of_aft_thruster_rooms); break;
		case 147: writer.Int(one.toxic_gas_pre_alarm_air_intake_of_aft_thruster_rooms); break;
		case 148: writer.Int(one.toxic_gas_level1_alarm_air_intake_of_aft_thruster_rooms); break;
		case 149: writer.Int(one.toxic_gas_level2_alarm_air_intake_of_aft_thruster_rooms); break;
		case 150: writer.Int(one.toxic_gas_detector_disabled_air_intake_of_aft_thruster_rooms); break;
		case 151: writer.Int(one.toxic_gas_detector_fault_air_intake_of_engine_roomp); break;
		case 152: writer.Int(one.toxic_gas_pre_alarm_air_intake_of_engine_roomp); break;
		case 153: writer.Int(one.toxic_gas_level1_alarm_air_intake_of_engine_roomp); break;
		case 154: writer.Int(one.toxic_gas_level2_alarm_air_intake_of_engine_roomp); break;
		case 155: writer.Int(one.toxic_gas_detector_disabled_air_intake_of_engine_roomp); break;
		case 156: writer.Int(one.toxic_gas_detector_fault_air_intake_of_engine_rooms); break;
		case 157: writer.Int(one.toxic_gas_pre_alarm_air_intake_of_engine_rooms); break;
		case 158: writer.Int(one.toxic_gas_level1_alarm_air_intake_of_engine_rooms); break;
		case 159: writer.Int(one.toxic_gas_level2_alarm_air_intake_of_engine_rooms); break;
		case 160: writer.Int(one.toxic_gas_detector_disabled_air_intake_of_engine_rooms); break;
		case 161: writer.Int(one.toxic_gas_detector_fault_air_intake_of_aux_machinery_room1); break;
		case 162: writer.Int(one.toxic_gas_pre_alarm_air_intake_of_aux_machinery_room1); break;
		case 163: writer.Int(one.toxic_gas_level1_alarm_air_intake_of_aux_machinery_room1); break;
		case 164: writer.Int(one.toxic_gas_level2_alarm_air_intake_of_aux_machinery_room1); break;
		case 165: writer.Int(one.toxic_gas_detector_disabled_air_intake_of_aux_machinery_room1); break;
		case 166: writer.Int(one.toxic_gas_detector_fault_air_intake_of_aux_machinery_room2); break;
		case 167: writer.Int(one.toxic_gas_pre_alarm_air_intake_of_aux_machinery_room2); break;
		case 168: writer.Int(one.toxic_gas_level1_alarm_air_intake_of_aux_machinery_room2); break;
		case 169: writer.Int(one.toxic_gas_level2_alarm_air_intake_of_aux_machinery_room2); break;
		case 170: writer.Int(one.toxic_gas_detector_disabled_air_intake_of_aux_machinery_room2); break;
		case 171: writer.Int(one.toxic_gas_detector_fault_b_deck_entrancefr109p); break;
		case 172: writer.Int(one.toxic_gas_pre_alarm_b_deck_entrancefr109p); break;
		case 173: writer.Int(one.toxic_gas_level1_alarm_b_deck_entrancefr109p); break;
		case 174: writer.Int(one.toxic_gas_level2_alarm_b_deck_entrancefr109p); break;
		case 175: writer.Int(one.toxic_gas_detector_disabled_b_deck_entrancefr109p); break;
		case 176: writer.Int(one.toxic_gas_detector_fault_b_deck_entrancefr109s); break;
		case 177: writer.Int(one.toxic_gas_pre_alarm_b_deck_entrancefr109s); break;
		case 178: writer.Int(one.toxic_gas_level1_alarm_b_deck_entrancefr109s); break;
		case 179: writer.Int(one.toxic_gas_level2_alarm_b_deck_entrancefr109s); break;
		case 180: writer.Int(one.toxic_gas_detector_disabled_b_deck_entrancefr109s); break;
		case 181: writer.Int(one.device_offline); break;
#endif
#else
         case 0:writer.Int(one.co2_power_failure_alarm);break;
         case 1:writer.Int(one.co2_pre_release_alarm);break;
         case 2:writer.Int(one.co2_release_alarm);break;
         case 3:writer.Int(one.co2_leakage_alarm);break;
         case 4:writer.Int(one.device_offline);break;
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
//cout<<"gas:" << data << endl;
	return strBuf.GetString();
}



void GAS_DETECTION_SYSTEM::thSetDelay()
{
	setDelay();
}


void GAS_DETECTION_SYSTEM::thSetInhibit()
{
	setInhibit(5800,1);

	setInhibit(5900,12);
}

void GAS_DETECTION_SYSTEM::init_mb_addr()
{
	int addr_ai = 8700;
#ifdef JANA505	
	m_mp_delay_addr[gas_detection_system_str[0]] = 8010;
	for(int i = 1; i < 186; i++)
	{
		m_mp_delay_addr[gas_detection_system_str[i]] = addr_ai + i;
	}

	////////////////inhibit
	m_mp_inhibit_addr[gas_detection_system_str[0]] = 10;//5801.10

	for(int i = 0; i < 12; i++)
	{
		for(int j = 0;j < 16; j++)
		{
			m_mp_inhibit_addr[gas_detection_system_str[16 * i + j + 1]] = 100 * i + j;
		}
	}
#endif
#ifdef JANA508509
	m_mp_delay_addr[gas_detection_system_str[0]] = 8010;
	for(int i = 1; i < 115; i++)
	{
		m_mp_delay_addr[gas_detection_system_str[i]] = addr_ai + i;
	}
	for(int i = 115; i < 181; i++)
	{
		m_mp_delay_addr[gas_detection_system_str[i]] = addr_ai + i + 5;
	}

	////////////////inhibit
	m_mp_inhibit_addr[gas_detection_system_str[0]] = 10;//5801.10
	int ix = 0;
	for(int i = 0; i < 7; i++)
	{
		for(int j = 0;j < 16; j++)
		{
			m_mp_inhibit_addr[gas_detection_system_str[ix++]] = 100 * i + j;
		}
	}

	int value = 700;
	for(int i = 0; i < 3;i++)
		m_mp_inhibit_addr[gas_detection_system_str[ix++]] = value+i;

	for(int i = 8; i < 16;i++)
		m_mp_inhibit_addr[gas_detection_system_str[ix++]] = value+i;
	

	for(int i = 8; i < 11; i++)
	{
		for(int j = 0;j < 16; j++)
		{
			m_mp_inhibit_addr[gas_detection_system_str[ix++]] = 100 * i + j;
		}
	}

	for(int i = 0; i < 9;i++)
		m_mp_inhibit_addr[gas_detection_system_str[ix++]] = 1100 + i;	
#endif
}
