#include <iostream>
#include <unistd.h>

#include "sgo_sub_pub.h"
#include "sgo_func.h"
#include "sgo_ams/port_azimuth_center_control_cabinet.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

using namespace std;

const char* port_azimuth_center_control_cabinet_str[]={
    "port_no1_azimuth_position",
    "port_no2_azimuth_position",
    "port_propeller_speed",
    "port_main_vfd_speed",
    "port_lub_oil_temperature",
    "port_main_motor_u_winding_temp",
    "port_main_motor_v_winding_temp",
    "port_main_motor_w_winding_temp",
    "port_main_motor1_bearing_temp",
    "port_main_motor2_bearing_temp",
    "port_main_motor_wind_in_temp",
    "port_main_motor_wind_out_temp",
    "port_pms_power_capacity",
    "port_main_vfd_load",
    "port_control_position",
    "port_main_power_fault",
    "port_back_power_fault",
    "port_lo_level_low",
    "port_lo_temp_high",
    "port_lo_filter_blocked",
    "port_lo_pressure_low",
    "port_lo_standby_pump_switched",
    "port_azimut1_vfd_fault",
    "port_azimut2_vfd_fault",
    "port_azimuth_control_fault",
    "port1lo_pump_overload",
    "port2lo_pump_overload",
    "port_azimuth_position1control_feedback_lost",
    "port_fwd_azimuth_command_lost",
    "port_fwd_speed_command_lost",
    "port_aft_azimuth_command_lost",
    "port_aft_speed_command_lost",
    "port_main_vfd_common_alarm",
    "port_main_vfd_estop",
    "port_mian_motor_water_leak",
    "port_shaft_lock_air_pressure_low",
    "port_gravity_oil_level_low",
    "port_gravity_air_pressure_low",
    "port_gravity_air_pressure_high",
    "port_main_motor_u_temp_high",
    "port_main_motor_v_temp_high",
    "port_main_motor_w_temp_high",
    "port_front_shaft_bearing_temp_high",
    "port_after_shaft_bearing_temp_high",
    "port_main_vfd_load_feedback_lost",
    "port_pms_power_capacity_feedback_lost",
    "port_main_vfd_speed_feedback_lost",
    "port_main_vfd_speed_control_fault",
    "port_azimuth1vfd_communication_lost",
    "port_azimuth2vfd_communication_lost",
    "port_main_motor1_bearing_temp_high",
    "port_main_motor2_bearing_temp_high",
    "port_main_motor_wind_in_temp_high",
    "port_main_motor_wind_out_temp_high",
    "port_main_motor_wind_temp_high_sld_per_alarm",
    "port_main_motor_bearing_temp_high_sld_per_alarm",
    "port_sld_active",
    "port_bridge_station_communication_lost",
    "port_heavy_load_inquiry_fault",
    "port_main_vfd_start_interlock",
    "port_main_vfd_fault",
    "port_main_vfd_esiop_wire_broken",
    "port_gravity_oil_level_high",
    "port_azimuth_position2indication_feedback_lost",
    "port_azimuth_position_feedback_unsynchronized",
    "port_vibration_sensor1_lost",
    "port_vibration_sensor2_lost",
    "port_dp_azimuth_command_lost",
    "port_dp_speed_command_lost",
    "port_joy_azimuth_command_lost",
    "port_joy_speed_command_lost",
    "port_ecr_speed_command_lost",
    "port_ecr_station_communication_lost",
    "pst_fresh_water_cooling_pump_outlet_pressure",
    "pst_sea_water_cooling_pump_outlet_pressure",
    "pst_cb_status",
    "device_offline",
};

	
static const char* productCode = "port_azimuth_center_control_cabinet";

PORT_AZIMUTH_CENTER_CONTROL_CABINET::PORT_AZIMUTH_CENTER_CONTROL_CABINET()
{}

PORT_AZIMUTH_CENTER_CONTROL_CABINET::~PORT_AZIMUTH_CENTER_CONTROL_CABINET()
{}

void PORT_AZIMUTH_CENTER_CONTROL_CABINET::init(int devId,BlockQueue<port_azimuth_center_control_cabinet> &bq,modbus* mb,mutex* mux)
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
void PORT_AZIMUTH_CENTER_CONTROL_CABINET::connectionLostCallback(void* context, char* cause) 
{
	// 处理连接丢失事件
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	PORT_AZIMUTH_CENTER_CONTROL_CABINET* client = static_cast<PORT_AZIMUTH_CENTER_CONTROL_CABINET*>(ctx->manager);
	
	printf("--%s %d--address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());
	sgo_log_stderr(0,"--%s %d----address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());

	//std::lock_guard<std::mutex> lock(m_losts_mutex);
	client->m_v_lost[ctx->index] = 1;
	client->m_v_property_index_array.clear();
}


// 消息到达回调函数
int PORT_AZIMUTH_CENTER_CONTROL_CABINET::messageArrivedCallback(void* context, char* topicName, int topicLen,MQTTClient_message* message) 
{
	// 处理接收到的消息
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	PORT_AZIMUTH_CENTER_CONTROL_CABINET* client = static_cast<PORT_AZIMUTH_CENTER_CONTROL_CABINET*>(ctx->manager);
	
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
			client->m_property[port_azimuth_center_control_cabinet_str[i]]=i;
		
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
void PORT_AZIMUTH_CENTER_CONTROL_CABINET::deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt) 
{
	// 处理交付完成事件
	//std::cout << "Message delivery complete." << std::endl;
}

void PORT_AZIMUTH_CENTER_CONTROL_CABINET::subClient()
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


void PORT_AZIMUTH_CENTER_CONTROL_CABINET::pubClient(BlockQueue<port_azimuth_center_control_cabinet>& bq)
{
	while(1)
	{
		MQTTClient_message pubmsg = MQTTClient_message_initializer;
		//声明消息token
		MQTTClient_deliveryToken token;

		while(!m_pubStop){
			port_azimuth_center_control_cabinet one = bq.take();
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



void PORT_AZIMUTH_CENTER_CONTROL_CABINET::start(BlockQueue<port_azimuth_center_control_cabinet> &bq)
{
	m_sub = new thread(&PORT_AZIMUTH_CENTER_CONTROL_CABINET::subClient,this);
	m_sub->detach();

	m_pub = new thread(&PORT_AZIMUTH_CENTER_CONTROL_CABINET::pubClient,this,ref(bq));
	m_pub->detach();

	m_thr_reconn = new thread(&PORT_AZIMUTH_CENTER_CONTROL_CABINET::reconn,this);
	m_thr_reconn->detach();

	m_thr_resend = new thread(&PORT_AZIMUTH_CENTER_CONTROL_CABINET::resnd,this);
	m_thr_resend->detach();

	m_thr_chksensor = new thread(&PORT_AZIMUTH_CENTER_CONTROL_CABINET::chksensor,this);
	m_thr_chksensor->detach();

//	thread *thr_range = new thread(&PORT_AZIMUTH_CENTER_CONTROL_CABINET::thSetRange,this);
//	thr_range->detach();

	thread *thr_threshold = new thread(&PORT_AZIMUTH_CENTER_CONTROL_CABINET::thSetThreshold,this);
	thr_threshold->detach();

	thread *thr_inhibit = new thread(&PORT_AZIMUTH_CENTER_CONTROL_CABINET::thSetInhibit,this);
	thr_inhibit->detach();

	thread *thr_delay = new thread(&PORT_AZIMUTH_CENTER_CONTROL_CABINET::thSetDelay,this);
	thr_delay->detach();		
}

void PORT_AZIMUTH_CENTER_CONTROL_CABINET::chksensor()
{
	checkSensorStatus();
}

void PORT_AZIMUTH_CENTER_CONTROL_CABINET::resnd()
{
	resend();
}

void PORT_AZIMUTH_CENTER_CONTROL_CABINET::reconn()
{
	reconnect();
}


string PORT_AZIMUTH_CENTER_CONTROL_CABINET::property_reply(char flag,const port_azimuth_center_control_cabinet& one,const string& msg_id)
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
            case 0:writer.Double(one.port_no1_azimuth_position);break;
            case 1:writer.Double(one.port_no2_azimuth_position);break;
            case 2:writer.Double(one.port_propeller_speed);break;
            case 3:writer.Double(one.port_main_vfd_speed);break;
            case 4:writer.Double(one.port_lub_oil_temperature);break;
            case 5:writer.Double(one.port_main_motor_u_winding_temp);break;
            case 6:writer.Double(one.port_main_motor_v_winding_temp);break;
            case 7:writer.Double(one.port_main_motor_w_winding_temp);break;
            case 8:writer.Double(one.port_main_motor1_bearing_temp);break;
            case 9:writer.Double(one.port_main_motor2_bearing_temp);break;
            case 10:writer.Double(one.port_main_motor_wind_in_temp);break;
            case 11:writer.Double(one.port_main_motor_wind_out_temp);break;
            case 12:writer.Double(one.port_pms_power_capacity);break;
            case 13:writer.Double(one.port_main_vfd_load);break;
            case 14:writer.Double(one.port_control_position);break;
            case 15:writer.Int(one.port_main_power_fault);break;
            case 16:writer.Int(one.port_back_power_fault);break;
            case 17:writer.Int(one.port_lo_level_low);break;
            case 18:writer.Int(one.port_lo_temp_high);break;
            case 19:writer.Int(one.port_lo_filter_blocked);break;
            case 20:writer.Int(one.port_lo_pressure_low);break;
            case 21:writer.Int(one.port_lo_standby_pump_switched);break;
            case 22:writer.Int(one.port_azimut1_vfd_fault);break;
            case 23:writer.Int(one.port_azimut2_vfd_fault);break;
            case 24:writer.Int(one.port_azimuth_control_fault);break;
            case 25:writer.Int(one.port1lo_pump_overload);break;
            case 26:writer.Int(one.port2lo_pump_overload);break;
            case 27:writer.Int(one.port_azimuth_position1control_feedback_lost);break;
            case 28:writer.Int(one.port_fwd_azimuth_command_lost);break;
            case 29:writer.Int(one.port_fwd_speed_command_lost);break;
            case 30:writer.Int(one.port_aft_azimuth_command_lost);break;
            case 31:writer.Int(one.port_aft_speed_command_lost);break;
            case 32:writer.Int(one.port_main_vfd_common_alarm);break;
            case 33:writer.Int(one.port_main_vfd_estop);break;
            case 34:writer.Int(one.port_mian_motor_water_leak);break;
            case 35:writer.Int(one.port_shaft_lock_air_pressure_low);break;
            case 36:writer.Int(one.port_gravity_oil_level_low);break;
            case 37:writer.Int(one.port_gravity_air_pressure_low);break;
            case 38:writer.Int(one.port_gravity_air_pressure_high);break;
            case 39:writer.Int(one.port_main_motor_u_temp_high);break;
            case 40:writer.Int(one.port_main_motor_v_temp_high);break;
            case 41:writer.Int(one.port_main_motor_w_temp_high);break;
            case 42:writer.Int(one.port_front_shaft_bearing_temp_high);break;
            case 43:writer.Int(one.port_after_shaft_bearing_temp_high);break;
            case 44:writer.Int(one.port_main_vfd_load_feedback_lost);break;
            case 45:writer.Int(one.port_pms_power_capacity_feedback_lost);break;
            case 46:writer.Int(one.port_main_vfd_speed_feedback_lost);break;
            case 47:writer.Int(one.port_main_vfd_speed_control_fault);break;
            case 48:writer.Int(one.port_azimuth1vfd_communication_lost);break;
            case 49:writer.Int(one.port_azimuth2vfd_communication_lost);break;
            case 50:writer.Int(one.port_main_motor1_bearing_temp_high);break;
            case 51:writer.Int(one.port_main_motor2_bearing_temp_high);break;
            case 52:writer.Int(one.port_main_motor_wind_in_temp_high);break;
            case 53:writer.Int(one.port_main_motor_wind_out_temp_high);break;
            case 54:writer.Int(one.port_main_motor_wind_temp_high_sld_per_alarm);break;
            case 55:writer.Int(one.port_main_motor_bearing_temp_high_sld_per_alarm);break;
            case 56:writer.Int(one.port_sld_active);break;
            case 57:writer.Int(one.port_bridge_station_communication_lost);break;
            case 58:writer.Int(one.port_heavy_load_inquiry_fault);break;
            case 59:writer.Int(one.port_main_vfd_start_interlock);break;
            case 60:writer.Int(one.port_main_vfd_fault);break;
            case 61:writer.Int(one.port_main_vfd_esiop_wire_broken);break;
            case 62:writer.Int(one.port_gravity_oil_level_high);break;
            case 63:writer.Int(one.port_azimuth_position2indication_feedback_lost);break;
            case 64:writer.Int(one.port_azimuth_position_feedback_unsynchronized);break;
            case 65:writer.Int(one.port_vibration_sensor1_lost);break;
            case 66:writer.Int(one.port_vibration_sensor2_lost);break;
            case 67:writer.Int(one.port_dp_azimuth_command_lost);break;
            case 68:writer.Int(one.port_dp_speed_command_lost);break;
            case 69:writer.Int(one.port_joy_azimuth_command_lost);break;
            case 70:writer.Int(one.port_joy_speed_command_lost);break;
            case 71:writer.Int(one.port_ecr_speed_command_lost);break;
            case 72:writer.Int(one.port_ecr_station_communication_lost);break;
            case 73:writer.Double(one.pst_fresh_water_cooling_pump_outlet_pressure);break;
            case 74:writer.Double(one.pst_sea_water_cooling_pump_outlet_pressure);break;
            case 75:writer.Int(one.pst_cb_status);break;
            case 76:writer.Int(one.device_offline);break;
			default:
				break;break;
		}
	}

	writer.EndArray();		
#else
	writer.Key("properties");
	writer.StartObject();

	int i=0;
	writer.Key(port_azimuth_center_control_cabinet_str[i++]);writer.Int(one.b_t_remote_control_system_ac_power_failure);
	writer.Key(port_azimuth_center_control_cabinet_str[i++]);writer.Int(one.b_t_remote_control_system_dc_power_failure);
	writer.Key(port_azimuth_center_control_cabinet_str[i++]);writer.Int(one.b_t_remote_control_system_failure);
	writer.Key(port_azimuth_center_control_cabinet_str[i++]);writer.Int(one.b_t_main_motor_overload);
	writer.Key(port_azimuth_center_control_cabinet_str[i++]);writer.Int(one.b_t_main_motor_high_temperature);
	writer.Key(port_azimuth_center_control_cabinet_str[i++]);writer.Int(one.b_t_oil_low_level);
	writer.Key(port_azimuth_center_control_cabinet_str[i++]);writer.Int(one.b_t_oil_low_pressure);
	writer.Key(port_azimuth_center_control_cabinet_str[i++]);writer.Int(one.b_t_oil_high_pressure);
	writer.Key(port_azimuth_center_control_cabinet_str[i++]);writer.Int(one.b_t_main_motor_short_circuit);
	writer.Key(port_azimuth_center_control_cabinet_str[i++]);writer.Int(one.b_t_emergency_stop_circuit_failure);
	writer.Key(port_azimuth_center_control_cabinet_str[i++]);writer.Int(one.b_t_too_many_times_start);
	writer.Key(port_azimuth_center_control_cabinet_str[i++]);writer.Int(one.b_t_integrated_alarm);

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



void PORT_AZIMUTH_CENTER_CONTROL_CABINET::thSetRange()
{
	setRange();
}

void PORT_AZIMUTH_CENTER_CONTROL_CABINET::thSetThreshold()
{
	setThreshold();
}

void PORT_AZIMUTH_CENTER_CONTROL_CABINET::thSetDelay()
{
	setDelay();
}


void PORT_AZIMUTH_CENTER_CONTROL_CABINET::thSetInhibit()
{
	setInhibit(5806,2);
}


void PORT_AZIMUTH_CENTER_CONTROL_CABINET::init_mb_addr()
{
	int addr_ai = 8000;
	for(int i = 0; i < 3; i++)
		m_mp_delay_addr[port_azimuth_center_control_cabinet_str[i]] = addr_ai + 102 + 3 * (m_devId - 1) + i;

	for(int i = 0; i < 4; i++)
	{
		m_mp_delay_addr[port_azimuth_center_control_cabinet_str[3 + i]] = addr_ai + 108 + 9 * (m_devId - 1) + i;
		m_mp_delay_addr[port_azimuth_center_control_cabinet_str[7 + i]] = addr_ai + 113 + 9 * (m_devId - 1) + i;
	}
	m_mp_delay_addr[port_azimuth_center_control_cabinet_str[11]] = addr_ai + 112 + 9 * (m_devId - 1);

	//////////////////////////////////////////////////////
	if(m_devId == 0)
	{
		int i = 0;
		m_mp_inhibit_addr[port_azimuth_center_control_cabinet_str[i++]] = 2;
		m_mp_inhibit_addr[port_azimuth_center_control_cabinet_str[i++]] = 3;
		m_mp_inhibit_addr[port_azimuth_center_control_cabinet_str[i++]] = 4;
		m_mp_inhibit_addr[port_azimuth_center_control_cabinet_str[i++]] = 8;
		m_mp_inhibit_addr[port_azimuth_center_control_cabinet_str[i++]] = 9;
		m_mp_inhibit_addr[port_azimuth_center_control_cabinet_str[i++]] = 10;
		m_mp_inhibit_addr[port_azimuth_center_control_cabinet_str[i++]] = 11;
		m_mp_inhibit_addr[port_azimuth_center_control_cabinet_str[i++]] = 13;
		m_mp_inhibit_addr[port_azimuth_center_control_cabinet_str[i++]] = 14;
		m_mp_inhibit_addr[port_azimuth_center_control_cabinet_str[i++]] = 15;
		m_mp_inhibit_addr[port_azimuth_center_control_cabinet_str[i++]] = 100;
		m_mp_inhibit_addr[port_azimuth_center_control_cabinet_str[i++]] = 12;		
	}
	else
	{
		int i = 0;
		m_mp_inhibit_addr[port_azimuth_center_control_cabinet_str[i++]] = 5;
		m_mp_inhibit_addr[port_azimuth_center_control_cabinet_str[i++]] = 6;
		m_mp_inhibit_addr[port_azimuth_center_control_cabinet_str[i++]] = 7;
		m_mp_inhibit_addr[port_azimuth_center_control_cabinet_str[i++]] = 101;
		m_mp_inhibit_addr[port_azimuth_center_control_cabinet_str[i++]] = 102;
		m_mp_inhibit_addr[port_azimuth_center_control_cabinet_str[i++]] = 103;
		m_mp_inhibit_addr[port_azimuth_center_control_cabinet_str[i++]] = 104;
		m_mp_inhibit_addr[port_azimuth_center_control_cabinet_str[i++]] = 106;
		m_mp_inhibit_addr[port_azimuth_center_control_cabinet_str[i++]] = 107;
		m_mp_inhibit_addr[port_azimuth_center_control_cabinet_str[i++]] = 108;
		m_mp_inhibit_addr[port_azimuth_center_control_cabinet_str[i++]] = 109;
		m_mp_inhibit_addr[port_azimuth_center_control_cabinet_str[i++]] = 105;		
	}		
}





