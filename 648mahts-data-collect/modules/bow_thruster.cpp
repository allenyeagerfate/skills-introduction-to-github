#include <iostream>
#include <unistd.h>

#include "sgo_sub_pub.h"
#include "sgo_func.h"
#include "sgo_ams/bow_thruster.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

using namespace std;

const char* bow_thruster_str[]={
    "b_t_hydraulic_oil_tank_low_oil_level",
    "b_t_spare_power_failure",
    "b_t_main_motor_high_temp",
    "b_t_main_power_fault",
    "b_t_ac220v_power_fault",
    "b_t_dc24v_power_fault",
    "b_t_main_vfd_integrate",
    "b_t_integrate_alarm",
    "b_t_fresh_water_cooling_pump_outlet_pressure",
    "b_t_sea_water_cooling_pump_outlet_pressure",
    "b_t_cb_status",
    "b_t_ams_common_alarm",
	"emcy_wire_break",
    "device_offline",
};

	
static const char* productCode = "bow_thruster";

BOW_THRUSTER::BOW_THRUSTER()
{}

BOW_THRUSTER::~BOW_THRUSTER()
{}

void BOW_THRUSTER::init(int devId,BlockQueue<bow_thruster> &bq,modbus* mb,mutex* mux)
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
void BOW_THRUSTER::connectionLostCallback(void* context, char* cause) 
{
	// 处理连接丢失事件
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	BOW_THRUSTER* client = static_cast<BOW_THRUSTER*>(ctx->manager);
	
	printf("--%s %d--address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());
	sgo_log_stderr(0,"--%s %d----address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());

	//std::lock_guard<std::mutex> lock(m_losts_mutex);
	client->m_v_lost[ctx->index] = 1;
	client->m_v_property_index_array.clear();
}


// 消息到达回调函数
int BOW_THRUSTER::messageArrivedCallback(void* context, char* topicName, int topicLen,MQTTClient_message* message) 
{
	// 处理接收到的消息
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	BOW_THRUSTER* client = static_cast<BOW_THRUSTER*>(ctx->manager);
	
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
			client->m_property[bow_thruster_str[i]]=i;
		
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
void BOW_THRUSTER::deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt) 
{
	// 处理交付完成事件
	//std::cout << "Message delivery complete." << std::endl;
}

void BOW_THRUSTER::subClient()
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


void BOW_THRUSTER::pubClient(BlockQueue<bow_thruster>& bq)
{
	while(1)
	{
		MQTTClient_message pubmsg = MQTTClient_message_initializer;
		//声明消息token
		MQTTClient_deliveryToken token;

		while(!m_pubStop){
			bow_thruster one = bq.take();
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



void BOW_THRUSTER::start(BlockQueue<bow_thruster> &bq)
{
	m_sub = new thread(&BOW_THRUSTER::subClient,this);
	m_sub->detach();

	m_pub = new thread(&BOW_THRUSTER::pubClient,this,ref(bq));
	m_pub->detach();

	m_thr_reconn = new thread(&BOW_THRUSTER::reconn,this);
	m_thr_reconn->detach();

	m_thr_resend = new thread(&BOW_THRUSTER::resnd,this);
	m_thr_resend->detach();

	m_thr_chksensor = new thread(&BOW_THRUSTER::chksensor,this);
	m_thr_chksensor->detach();

//	thread *thr_range = new thread(&BOW_THRUSTER::thSetRange,this);
//	thr_range->detach();

	thread *thr_threshold = new thread(&BOW_THRUSTER::thSetThreshold,this);
	thr_threshold->detach();

	thread *thr_inhibit = new thread(&BOW_THRUSTER::thSetInhibit,this);
	thr_inhibit->detach();

	thread *thr_delay = new thread(&BOW_THRUSTER::thSetDelay,this);
	thr_delay->detach();		
}

void BOW_THRUSTER::chksensor()
{
	checkSensorStatus();
}

void BOW_THRUSTER::resnd()
{
	resend();
}

void BOW_THRUSTER::reconn()
{
	reconnect();
}


string BOW_THRUSTER::property_reply(char flag,const bow_thruster& one,const string& msg_id)
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
           case 0:writer.Int(one.b_t_hydraulic_oil_tank_low_oil_level);break;
           case 1:writer.Int(one.b_t_spare_power_failure);break;
           case 2:writer.Int(one.b_t_main_motor_high_temp);break;
           case 3:writer.Int(one.b_t_main_power_fault);break;
           case 4:writer.Int(one.b_t_ac220v_power_fault);break;
           case 5:writer.Int(one.b_t_dc24v_power_fault);break;
           case 6:writer.Int(one.b_t_main_vfd_integrate);break;
           case 7:writer.Int(one.b_t_integrate_alarm);break;
           case 8:writer.Double(one.b_t_fresh_water_cooling_pump_outlet_pressure);break;
           case 9:writer.Double(one.b_t_sea_water_cooling_pump_outlet_pressure);break;
           case 10:writer.Int(one.b_t_cb_status);break;
           case 11:writer.Int(one.b_t_ams_common_alarm);break;
		   case 12:writer.Int(one.b_t_emcy_wire_break);break;
           case 13:writer.Int(one.device_offline);break;

			default:
				break;break;
		}
	}

	writer.EndArray();		
#else
	writer.Key("properties");
	writer.StartObject();

	int i=0;
	writer.Key(bow_thruster_str[i++]);writer.Int(one.b_t_remote_control_system_ac_power_failure);
	writer.Key(bow_thruster_str[i++]);writer.Int(one.b_t_remote_control_system_dc_power_failure);
	writer.Key(bow_thruster_str[i++]);writer.Int(one.b_t_remote_control_system_failure);
	writer.Key(bow_thruster_str[i++]);writer.Int(one.b_t_main_motor_overload);
	writer.Key(bow_thruster_str[i++]);writer.Int(one.b_t_main_motor_high_temperature);
	writer.Key(bow_thruster_str[i++]);writer.Int(one.b_t_oil_low_level);
	writer.Key(bow_thruster_str[i++]);writer.Int(one.b_t_oil_low_pressure);
	writer.Key(bow_thruster_str[i++]);writer.Int(one.b_t_oil_high_pressure);
	writer.Key(bow_thruster_str[i++]);writer.Int(one.b_t_main_motor_short_circuit);
	writer.Key(bow_thruster_str[i++]);writer.Int(one.b_t_emergency_stop_circuit_failure);
	writer.Key(bow_thruster_str[i++]);writer.Int(one.b_t_too_many_times_start);
	writer.Key(bow_thruster_str[i++]);writer.Int(one.b_t_integrated_alarm);

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



void BOW_THRUSTER::thSetRange()
{
	setRange();
}

void BOW_THRUSTER::thSetThreshold()
{
	setThreshold();
}

void BOW_THRUSTER::thSetDelay()
{
	setDelay();
}


void BOW_THRUSTER::thSetInhibit()
{
	setInhibit(5806,2);
}


void BOW_THRUSTER::init_mb_addr()
{
	int addr_ai = 8000;
	for(int i = 0; i < 3; i++)
		m_mp_delay_addr[bow_thruster_str[i]] = addr_ai + 102 + 3 * (m_devId - 1) + i;

	for(int i = 0; i < 4; i++)
	{
		m_mp_delay_addr[bow_thruster_str[3 + i]] = addr_ai + 108 + 9 * (m_devId - 1) + i;
		m_mp_delay_addr[bow_thruster_str[7 + i]] = addr_ai + 113 + 9 * (m_devId - 1) + i;
	}
	m_mp_delay_addr[bow_thruster_str[11]] = addr_ai + 112 + 9 * (m_devId - 1);

	//////////////////////////////////////////////////////
	if(m_devId == 0)
	{
		int i = 0;
		m_mp_inhibit_addr[bow_thruster_str[i++]] = 2;
		m_mp_inhibit_addr[bow_thruster_str[i++]] = 3;
		m_mp_inhibit_addr[bow_thruster_str[i++]] = 4;
		m_mp_inhibit_addr[bow_thruster_str[i++]] = 8;
		m_mp_inhibit_addr[bow_thruster_str[i++]] = 9;
		m_mp_inhibit_addr[bow_thruster_str[i++]] = 10;
		m_mp_inhibit_addr[bow_thruster_str[i++]] = 11;
		m_mp_inhibit_addr[bow_thruster_str[i++]] = 13;
		m_mp_inhibit_addr[bow_thruster_str[i++]] = 14;
		m_mp_inhibit_addr[bow_thruster_str[i++]] = 15;
		m_mp_inhibit_addr[bow_thruster_str[i++]] = 100;
		m_mp_inhibit_addr[bow_thruster_str[i++]] = 12;		
	}
	else
	{
		int i = 0;
		m_mp_inhibit_addr[bow_thruster_str[i++]] = 5;
		m_mp_inhibit_addr[bow_thruster_str[i++]] = 6;
		m_mp_inhibit_addr[bow_thruster_str[i++]] = 7;
		m_mp_inhibit_addr[bow_thruster_str[i++]] = 101;
		m_mp_inhibit_addr[bow_thruster_str[i++]] = 102;
		m_mp_inhibit_addr[bow_thruster_str[i++]] = 103;
		m_mp_inhibit_addr[bow_thruster_str[i++]] = 104;
		m_mp_inhibit_addr[bow_thruster_str[i++]] = 106;
		m_mp_inhibit_addr[bow_thruster_str[i++]] = 107;
		m_mp_inhibit_addr[bow_thruster_str[i++]] = 108;
		m_mp_inhibit_addr[bow_thruster_str[i++]] = 109;
		m_mp_inhibit_addr[bow_thruster_str[i++]] = 105;		
	}		
}





