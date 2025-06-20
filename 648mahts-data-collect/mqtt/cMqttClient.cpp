#include <iostream>
#include <unistd.h>
#include "cMqttClient.h"
#include "sgo_sub_pub.h"
#include "sgo_func.h"
#include "sgo_conf.h"

#include <random>

using namespace std;

//#define MQTT_PROXY "tcp://127.0.0.1:1883" 
//#define MQTT_PROXY "tcp://192.168.110.243:1883" 
#define MQTT_PROXY "tcp://192.168.20.203:1883"
#define DEV_MQTT_505_PROXY_1 "tcp://192.168.110.243:1883"
#define DEV_MQTT_505_PROXY_2 "tcp://192.168.100.84:1883"
#define DEV_MQTT_505_PROXY_3 "tcp://192.168.100.86:1883"
#define DEV_MQTT_505_PROXY_4 "tcp://192.168.100.244:1883"

#define TEST_MQTT_505_PROXY_1 "tcp://192.168.100.87:1883"
#define TEST_MQTT_505_PROXY_2 "tcp://192.168.100.89:1883"
#define TEST_MQTT_505_PROXY_3 "tcp://192.168.100.90:1883"



#define TEST_MQTT_508_PROXY "tcp://192.168.100.229:1883" 

CMQTTCLIENT::CMQTTCLIENT():m_address(MQTT_PROXY)
{
	m_report_pub_index = 0;
	m_pubStop = 0;
	m_crons = 3;
	m_devId = 1;
	m_lost = 0;

	m_v_address.clear();





#ifdef TEST
	//test
	#ifdef JANA505
	m_v_address.push_back(DEV_MQTT_505_PROXY_1);
	m_v_address.push_back(DEV_MQTT_505_PROXY_2);
	m_v_address.push_back(DEV_MQTT_505_PROXY_3);
	m_v_address.push_back(DEV_MQTT_505_PROXY_4);

//	m_v_address.push_back(TEST_MQTT_505_PROXY_1);
//	m_v_address.push_back(TEST_MQTT_505_PROXY_2);
//	m_v_address.push_back(TEST_MQTT_505_PROXY_3);
	//m_v_address.push_back(TEST_MQTT_505_PROXY_4);
	#endif
	#ifdef JANA508509
	m_v_address.push_back(TEST_MQTT_508_PROXY);
	#endif

//	m_v_a

#else

    std::string  m_cur_pc_ip = getHostIp("bond0");
    std::string push_first_addr;
	for(int i = 0;i < pc_nums;i++)
	{
		std::string ip(MQTT_IP_PREFIX);
        ip.append(to_string(3 + i));

		string addr("tcp://");
		addr.append(MQTT_IP_PREFIX);
		addr.append(to_string(3 + i));
		addr.append(":1883");
        if(!(m_cur_pc_ip.empty()))
		{
            if(ip == m_cur_pc_ip)
			{
                push_first_addr = addr;
			}
			else
			{
				m_v_address.push_back(addr);
			}
		}
		else
		{
			m_v_address.push_back(addr);
		}
	}

    if(!push_first_addr.empty())
	{
        m_v_address.insert(m_v_address.begin(), push_first_addr);
	}


#endif

	m_v_property_index_array.clear();

	m_v_lost.resize(m_v_address.size());
	m_conn_opts.resize(m_v_address.size());
	sgo_log_stderr(0,"*********************m_v_address begin*******************************");
	for(int i=0;i < m_v_address.size();i++)
	{
		sgo_log_stderr(0,"proxy address:%s",m_v_address[i].c_str());
		m_v_lost[i] = 0;
	}
	sgo_log_stderr(0, "***********************m_v_address end*****************************");
}

CMQTTCLIENT::~CMQTTCLIENT()
{
    //MQTTClient_destroy(&m_client);
}

void CMQTTCLIENT::setProxy(const string & proxy)
{
	m_address.assign(proxy);
}

void CMQTTCLIENT::setSubTopics()
{
	char prop[256]={0};
	snprintf(prop,sizeof(prop),"/%s/%s_device%d%s",m_productCode.c_str(),m_productCode.c_str(),m_devId,PROPERTY_READ);
	m_subs.push_back(prop);

	memset(prop,0,sizeof(prop));
	snprintf(prop,sizeof(prop),"/%s/%s_device%d%s",m_productCode.c_str(),m_productCode.c_str(),m_devId,FUNC_INVOKE);
	m_subs.push_back(prop);

	for(int i = 0; i < m_v_address.size(); i++)
	{
		m_conn_opts[i] = MQTTClient_connectOptions_initializer;
		m_conn_opts[i].keepAliveInterval = 3;
	    m_conn_opts[i].cleansession = 1;
	    m_conn_opts[i].username = "device_user";
		m_conn_opts[i].password = "34BC7B4E97F5";
	}
}
void CMQTTCLIENT::setPubTopics()
{
	char pub[256]={0};
	snprintf(pub,sizeof(pub),"%s%s",m_clientId.c_str(),PROPERTY_REPLY);
	m_pubs.push_back(pub);

	memset(pub,0,sizeof(pub));
	snprintf(pub,sizeof(pub),"%s%s",m_clientId.c_str(),FUNC_REPLY);	
	m_pubs.push_back(pub);


	memset(pub,0,sizeof(pub));
	snprintf(pub,sizeof(pub),"%s%s",m_clientId.c_str(),EVENT_REPLY);	
	m_pubs.push_back(pub);

}


void CMQTTCLIENT::parseProp(const char *sdata)
{
    //创建解析对象
    rapidjson::Document doc;

	string s(sdata);
	int index = s.find('\x88');
	if(index != string::npos)
		s = s.substr(0,index);

    //首先进行解析，没有解析错误才能进行具体字段的解析
    if(!doc.Parse(s.data()).HasParseError())
    {
        if(doc.HasMember("cron") && doc["cron"].IsString())
        {
			m_crons = getCronSecond(doc["cron"].GetString());
			//y cout<<"cron:"<<m_crons<<endl;			
        }

        if(doc.HasMember("msgType") && doc["msgType"].IsString())
        {
            //y cout << "msgType = " << doc["msgType"].GetString() << endl;
			m_readType = doc["msgType"].GetString();
        }

        if(doc.HasMember("msgId") && doc["msgId"].IsString())
        {
            //y cout << "msgId = " << doc["msgId"].GetString() << endl;
			if(0 == m_readType.compare("PROPERTY_REPORT"))
			{
				m_msgPubId = doc["msgId"].GetString();
				//y cout<<"m_msgPubId:"<<m_msgPubId<<endl;
			}
			else
			{
				m_msgReplyId = doc["msgId"].GetString();
				//y cout<<"m_msgReplyId:"<<m_msgReplyId<<endl;
			}
        }

        if(doc.HasMember("transferProtocol") && doc["transferProtocol"].IsString())
        {
            //y cout << "transferProtocol = " << doc["transferProtocol"].GetString() << endl;
        }

        if(doc.HasMember("protocolType") && doc["protocolType"].IsString())
        {
            //y cout << "protocolType = " << doc["protocolType"].GetString() << endl;
        }
        if(doc.HasMember("timestamp") && doc["timestamp"].IsInt64())
        {
            //y cout << "timestamp = " << doc["timestamp"].GetInt64() << endl;
        }


        //4. 解析结构体
        if(doc.HasMember("headers") && doc["headers"].IsObject())
        {
            const rapidjson::Value& object = doc["headers"];
            if(object.HasMember("deviceCode") && object["deviceCode"].IsString())
            {
				m_deviceCode = object["deviceCode"].GetString();
				//y cout <<"m_deviceCode:"<<m_deviceCode<<endl;
            }
            if(object.HasMember("productCode") && object["productCode"].IsString())
            {
				m_productCode = object["productCode"].GetString();
				//y cout <<"m_productCode:"<<m_productCode<<endl;				
            }

			if(object.HasMember("isInvokeByRule") && object["isInvokeByRule"].IsBool())
            {
				m_isInvokeByRule = object["isInvokeByRule"].GetBool();
				//y cout << "m_isInvokeByRule:"<<m_isInvokeByRule<<endl;
            }
        }
        //5. 解析数组类型
        //5.1 整型数组类型
        if(doc.HasMember("properties") && doc["properties"].IsArray())
        {
            //5.1.1 将字段转换成为rapidjson::Value类型
            const rapidjson::Value& array = doc["properties"];
            //5.1.2 获取数组长度
            size_t len = array.Size();
            //5.1.3 根据下标遍历，注意将元素转换为相应类型，即需要调用GetInt()
            for(size_t i = 0; i < len; i++)
            {
				//y cout << "properties[" << i << "] = " << array[i].GetString() << endl;
				m_property[array[i].GetString()] = i; //index
            }
        }

		if(doc.HasMember("propertyReadFieldInfoList") && doc["propertyReadFieldInfoList"].IsArray())
		{
			//5.1.1 将字段转换成为rapidjson::Value类型
			const rapidjson::Value& array = doc["propertyReadFieldInfoList"];
			//5.1.2 获取数组长度
			m_property_size = array.Size();
			//5.1.3 根据下标遍历，注意将元素转换为相应类型
			m_v_property_index_array.clear();
			for(size_t i = 0; i < m_property_size; i++)
			{
				if(array[i].IsObject())
				{
		            if(array[i].HasMember("fieldKey") && array[i]["fieldKey"].IsString())
		            {
						string fieldKey = array[i]["fieldKey"].GetString();
						//y cout <<"fieldKey:"<<fieldKey<<endl;	
		            }					
					if(array[i].HasMember("index") && array[i]["index"].IsInt())
					{
						int index = array[i]["index"].GetInt();
						//y cout <<"index:"<<index<<endl; 
					}
					if(array[i].HasMember("modelType") && array[i]["modelType"].IsString())
					{
						string modelType = array[i]["modelType"].GetString();
						//y cout <<"modelType:"<<modelType<<endl; 
					}

					property_index_array one;
					one.m_name = array[i]["fieldKey"].GetString();
					one.m_index = array[i]["index"].GetInt();
					if(array[i]["modelType"].GetString() == "INT" ||
						array[i]["modelType"].GetString() == "Int")
						one.m_value_type =  0;
					else if(array[i]["modelType"].GetString() == "DOUBLE" ||
						array[i]["modelType"].GetString() == "Double")
						one.m_value_type = 1;
					else
						one.m_value.m_i = 2;

					m_v_property_index_array.push_back(one); 
				}
			}
			std::sort(m_v_property_index_array.begin(), m_v_property_index_array.end(), compare_property);
		}

    }
	else
	{
		//y cout<<"CMQTTCLIENT json error"<<endl;
		sgo_log_stderr(0,"JSON:%s",sdata);
	}
}

void CMQTTCLIENT::parseRange(const rapidjson::Value& array,int len)
{
	range ra;
	for(size_t i = 0; i < len; i++)
	{	
		string key,value;
		if(array[i].HasMember("key") && array[i]["key"].IsString())
		{
			key =  array[i]["key"].GetString();
		}				
		if(array[i].HasMember("value") && array[i]["value"].IsString())
		{
			value =  array[i]["value"].GetString();
		}

		if(key.compare("propertyKey") == 0)
			ra.m_keyname = value;
		if(key.compare("range") == 0)
		{
	       cout<<"range:"<<value<<endl;	
			vector<float> v;
			parseRangeString(value, v);
			if(v.size() == 2)
			{
				ra.start = v[0];
				ra.end = v[1];
			}
		}
	}
	if(!ra.m_keyname.empty())
		bq_range.put(ra);


}

void CMQTTCLIENT::parseThreshold(const rapidjson::Value& array,int len)
{
	threshold ts;
	for(size_t i = 0; i < len; i++)
	{	
		string key,value;
		if(array[i].HasMember("key") && array[i]["key"].IsString())
		{
			key =  array[i]["key"].GetString();
		}				
		if(array[i].HasMember("value") && array[i]["value"].IsString())
		{
			value =  array[i]["value"].GetString();
		}

		if(key.compare("propertyKey") == 0)
			ts.m_keyname = value;
		if(key.compare("lowAlarm") == 0)
			ts.m_low_alarm = value;
		if(key.compare("highAlarm") == 0)
			ts.m_high_alarm = value;

	}
	if(!ts.m_keyname.empty())
	{
		bq_threshold.put(ts);
	}
}


void CMQTTCLIENT::parseInhibit(const rapidjson::Value& array,int len,bool isInhib)
{
	inhibit ib;
	for(int i = 0;i < len;i++)		
	{
		if(array[i].HasMember("value") && array[i]["value"].IsString())
		{
			ib.m_keyname = array[i]["value"].GetString();
			ib.is_inhibit =  isInhib;
			bq_inhibit.put(ib);
		}
	}
}

void CMQTTCLIENT::parseDelay(const rapidjson::Value& array,int len)
{
	delay dy;
	for(size_t i = 0; i < len; i++)
	{	
		if(array[i].HasMember("key") && array[i]["key"].IsString())
		{
			dy.m_keyname =  array[i]["key"].GetString();
		}				
		if(array[i].HasMember("value") && array[i]["value"].IsString())
		{
			dy.delaytime =  stoi(array[i]["value"].GetString());
		}

		bq_delay.put(dy);
	}
}

void CMQTTCLIENT::parseDuty(const rapidjson::Value& array,int len)
{
	onduty od;
	string key,value;
	for(size_t i = 0; i < len; i++)
	{	

		if(array[i].HasMember("key") && array[i]["key"].IsString())
		{
			key =  array[i]["key"].GetString();
		}				
		if(array[i].HasMember("value") && array[i]["value"].IsString())
		{
			value =  array[i]["value"].GetString();
		}

		if(0 == key.compare("deviceCode"))
			od.no = stoi(value.substr(value.find_last_of("0123456789"))) - 1;
		if(0 == key.compare("event"))
			od.is_on_duty = stoi(value);
	}
	if(!key.empty())
	{
		bq_onduty.put(od);
	}

}

void CMQTTCLIENT::parseExtendAlarmPanel(const rapidjson::Value& array, int len)
{
	extend_alarm_panel eap;

    if(3 == len)
    {
		string value;
		if(array[2].HasMember("value") && array[2]["value"].IsString())
		{
			value = array[2]["value"].GetString();

			m_u32Call = stoi(value.c_str());
		}

	}

}
void CMQTTCLIENT::parseInvoke(const char * sdata)
{
	//创建解析对象
	rapidjson::Document doc;

	string s(sdata);
	int index = s.find('\x88');
	if(index != string::npos)
		s = s.substr(0,index);

	//首先进行解析，没有解析错误才能进行具体字段的解析
	if(!doc.Parse(s.data()).HasParseError())
	{

		if(doc.HasMember("function") && doc["function"].IsObject())
		{
			const rapidjson::Value& funcObject = doc["function"];

			if(funcObject.HasMember("functionKey") && funcObject["functionKey"].IsString())
			{
				m_funcKey = funcObject["functionKey"].GetString();
				////y cout<<"funcName:"<<m_funcKey<<endl;
			}

			if(funcObject.HasMember("functionName") && funcObject["functionName"].IsString())
			{
				m_funcName = funcObject["functionName"].GetString();
				////y cout<<"funcName:"<<m_funcKey<<endl;
			}

			if(funcObject.HasMember("inParams") && funcObject["inParams"].IsArray())
			{
				//5.1.1 将字段转换成为rapidjson::Value类型
				const rapidjson::Value& array = funcObject["inParams"];
				//5.1.2 获取数组长度
				size_t len = array.Size();
				m_vInParams.clear();				
				//5.1.3 根据下标遍历，注意将元素转换为相应类型，即需要调用GetInt()
				if(m_funcKey == "changeRange")
					parseRange(array,len);
				if(m_funcKey == "changeAlarmThreshold")
					parseThreshold(array,len);
				if(m_funcKey == "inhibitConfig")
					parseInhibit(array,len,true);
				if(m_funcKey == "uninhibitedConfig")
					parseInhibit(array,len,false);
				if(m_funcKey == "delayConfig")
					parseDelay(array,len);
				if(m_funcKey == "setDutyDevice")
					parseDuty(array,len);
				if (m_funcKey == "callExtendAlarmPanel")
                    parseExtendAlarmPanel(array, len);

			}

			if(funcObject.HasMember("outParams") && funcObject["outParams"].IsObject())
			{
				const rapidjson::Value& outObject = funcObject["outParams"];

				if(outObject.HasMember("properties") && outObject["properties"].IsArray())
				{
					const rapidjson::Value& array = outObject["properties"];
					m_vOutParams.clear();

					size_t len = array.Size();
					for(size_t i = 0; i < len; i++)
					{
					  	if(array[i].HasMember("key") && array[i]["key"].IsString())
					  	{
							string value =  array[i]["key"].GetString();
							m_vOutParams.push_back(value);
						}
					}
				}
			}
		}

		//4. 解析结构体
		if(doc.HasMember("headers") && doc["headers"].IsObject())
		{
			const rapidjson::Value& object = doc["headers"];

			if(object.HasMember("deviceCode") && object["deviceCode"].IsString())
			{
				m_deviceCode = object["deviceCode"].GetString();
				////y cout <<"m_deviceCode:"<<m_deviceCode<<endl;
			}
			if(object.HasMember("productCode") && object["productCode"].IsString())
			{
				m_productCode = object["productCode"].GetString();
				////y cout <<"m_productCode:"<<m_productCode<<endl;				
			}

			if(object.HasMember("isInvokeByRule") && object["isInvokeByRule"].IsBool())
			{
				m_isInvokeByRule = object["isInvokeByRule"].GetBool();
				////y cout << "m_isInvokeByRule:"<<m_isInvokeByRule<<endl;
			}
		}


		if(doc.HasMember("transferProtocol") && doc["transferProtocol"].IsString())
		{
			////y cout << "transferProtocol = " << doc["transferProtocol"].GetString() << endl;
		}

		if(doc.HasMember("protocolType") && doc["protocolType"].IsString())
		{
			////y cout << "protocolType = " << doc["protocolType"].GetString() << endl;
		}
		if(doc.HasMember("msgType") && doc["msgType"].IsString())
		{
			////y cout << "msgType = " << doc["msgType"].GetString() << endl;
		}

	}
	else
	{
		////y cout<<"CMQTTCLIENT json error"<<endl;
		sgo_log_stderr(0,"JSON:%s",sdata);
	}
}


void CMQTTCLIENT::reconnect()
{
//return;
	sleep(10);
	while(1)
	{
		int rc;
	    printf("clientId:%s,m_v_address.size():%ld\n",m_clientId.c_str(), m_v_address.size());

		for(int i=0;i < m_v_address.size();i++)
		{
	        printf("reconnect proxy [%d]:%s m_v_Lost:%d\n",i,m_v_address[i].c_str(), m_v_lost[i]);
			if(m_v_lost[i] == 0)
			{
				continue;
			}

			rc = subOne(m_v_clients[i],i);
			{
				//std::lock_guard<std::mutex> lock(m_losts_mutex);
				m_v_lost[i] = rc ? 0 : 1;
			}
		}
		sleep(3);
	}

}


void CMQTTCLIENT::resend()
{
	for(int i = 0; i < m_v_address.size();i++)
	{
		thread* resnd = new thread(&CMQTTCLIENT::reSendLost,this,i);
		resnd->detach();
	}

}

void CMQTTCLIENT::reSendLost(int i)
{
return;

	while(1)
	{
		string payload;
		if(1 == m_v_lost[i] && m_bq_resend[i].size() < 3600) //超过1小时数据，取出一条丢弃
		{
			sleep(1);
			continue;		
		}

		if(!m_bq_resend[i].poll(payload))
		{
			sleep(1);
			continue;
		}


		MQTTClient_message pubmsg = MQTTClient_message_initializer;
		//声明消息token
		MQTTClient_deliveryToken token;

		pubmsg.payload = (void*)payload.c_str();
		pubmsg.payloadlen = payload.length();
		pubmsg.qos = 1;
		pubmsg.retained = 0;

		//MQTTClient_publishMessage(m_client, m_pubs[m_report_pub_index].c_str(), &pubmsg, &token);
		MQTTClient_publishMessage(m_v_clients[i], m_pubs[m_report_pub_index].c_str(), &pubmsg, &token);
	}
}


void CMQTTCLIENT::checkSensorStatus()
{
	while(1)
	{
		sleep(3);	
		if(m_sensor_exception)
		{
			for(int i = 0 ;i < m_v_address.size();i++)
			{
				if(MQTTClient_isConnected(m_v_clients[i]))
					MQTTClient_disconnect(m_v_clients[i], 10000);
			}
		}
		else //正常了
		{
			for(int i = 0 ;i < m_v_address.size();i++)
			{
				if(!MQTTClient_isConnected(m_v_clients[i]))
					reconnect();
			}
		}
	}
}

string CMQTTCLIENT::generateFakeMsgId() 
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, 61);

    const string charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    string randomString("fakedevicemsgid");
    randomString.reserve(32);

    for (int i = 0; i < 17; ++i) {
        randomString.push_back(charset[dis(gen)]);
    }

    return randomString;
}

void CMQTTCLIENT::sendMessage(const string& payload,int index)
{
	if(!payload.empty())
	{
		MQTTClient_message pubmsg = MQTTClient_message_initializer;
		MQTTClient_deliveryToken token;

		pubmsg.payload = (void*)payload.c_str();
		pubmsg.payloadlen = payload.length();
		pubmsg.qos = 1;
		pubmsg.retained = 0;

		MQTTClient_publishMessage(m_client, m_pubs[index].c_str(), &pubmsg, &token);
	}
}


void CMQTTCLIENT::setThreshold()
{
	return;
	while(1)
	{
		threshold one = bq_threshold.take();
		string key = one.m_keyname;
		string lowAlarm = one.m_low_alarm;
		string highAlarm = one.m_high_alarm;
		cout << "---------key:" << key << "addr:" << m_mp_threshold_addr[key] << " low:" << lowAlarm << " high:" << highAlarm << endl;
		if (lowAlarm.empty())
			cout << "lowalarm empty" << endl;
		if (highAlarm.empty())
			cout << "highalarm empty" << endl;

		//	cout<<m_mp_threshold_addr[key]<<endl;

		if(!m_mb_plc1)
		{
		cout<<"m_mb_plc1 null"<<endl;
			continue;
		}

		if(0 == m_mp_threshold_addr[key])//device_offline
			continue;

		std::lock_guard<std::mutex> lock(*m_mux_plc1);

		//high low 调换
		//if(lowAlarm == "null")
		if(lowAlarm.empty())
			m_mb_plc1->modbus_write_register(m_mp_threshold_addr[key]+100,static_cast<uint16_t>(threshold_low));
		else
			m_mb_plc1->modbus_write_register(m_mp_threshold_addr[key]+100,static_cast<uint16_t>(round(stod(lowAlarm) * 10)));
		//if( highAlarm == "null")
		if(highAlarm.empty())
			m_mb_plc1->modbus_write_register(m_mp_threshold_addr[key],static_cast<uint16_t>(threshold_high));
		else
			m_mb_plc1->modbus_write_register(m_mp_threshold_addr[key],static_cast<uint16_t>(round(stod(highAlarm) * 10)));

	}
}

void CMQTTCLIENT::setDelay()
{
	return;
	while(1)
	{
		delay one = bq_delay.take();
		string key = one.m_keyname;
		int delaytime = one.delaytime;

cout<<"setDelay string:"<< key <<" addr:"<< 40001 + m_mp_delay_addr[key]<<"  delaytime:"<<delaytime<<endl;

		if(!m_mb_plc1)
			continue;
		if(0 == m_mp_delay_addr[key])//device_offline
			continue;
		std::lock_guard<std::mutex> lock(*m_mux_plc1);
		m_mb_plc1->modbus_write_register(m_mp_delay_addr[key],delaytime);
	}

}

void CMQTTCLIENT::setInhibit(int addr,int need)
{
	return;
	while (1)
	{
		inhibit one = bq_inhibit.take();
		string key = one.m_keyname;
		bool inhib = one.is_inhibit;
		int ix = m_mp_inhibit_addr[key] / 100;
		int iloop = m_mp_inhibit_addr[key] % 100;
		uint16_t status[need] = {0};

		if (inhib)
			cout << "setInhibit string:" << key << " addr:" << 40001 + addr + ix << "." << iloop << endl;
		else
			cout << "setunInhibit string:" << key << " addr:" << 40001 + addr + ix << "." << iloop << endl;
		if (!m_mb_plc1)
			continue;
		if (0 == m_mp_inhibit_addr[key]) // device_offline
			continue;

		std::lock_guard<std::mutex> lock(*m_mux_plc1);
		if (m_mb_plc1->modbus_read_holding_registers(addr, need, status))
			continue;

		if (inhib)
			status[ix] |= (1 << iloop);
		else
			status[ix] &= ~(1 << iloop);

		m_mb_plc1->modbus_write_register(addr + ix, status[ix]);
	}
}


void CMQTTCLIENT::setRange()
{
	return;
	while(1)
	{
		range one = bq_range.take();
		string key = one.m_keyname;
		float start = one.start;
		float end = one.end;

		if(!m_mb_plc1)
			continue;
             //cout<<"setRange key:"<<key<<" addr:"<<m_mp_range_addr[key]<<" start:"<<static_cast<uint16_t>(start*10) << "end:"<<static_cast<uint16_t>(end*10)<<endl;
		if(0 == m_mp_range_addr[key])//device_offline
			continue;

		std::lock_guard<std::mutex> lock(*m_mux_plc1);
		//m_mb_plc1->modbus_write_register(m_mp_range_addr[key]+1,static_cast<uint16_t>(start*10));		
		//m_mb_plc1->modbus_write_register(m_mp_range_addr[key],static_cast<uint16_t>(end*10));

	}
}

void CMQTTCLIENT::setDeviceDuty()
{
	return;
	while(1)
	{
		onduty od = bq_onduty.take();
		// 1020
		if (m_mb_plc1 && m_mux_plc1 && m_mb_plc1->is_connected())
		{	
			if(1 == od.is_on_duty && m_duty)
			{
               (*m_duty) |= (1 << (od.no));
			}		
			else
			{
               (*m_duty) &= ~(1 << (od.no));
			}
		}
	}
}

bool CMQTTCLIENT::subOne(const MQTTClient &client,int ix)
{
	int rc;
	rc = MQTTClient_connect(client, &m_conn_opts[ix]);
    printf("MQTTClient_connect [%s] rc:%d\n",m_v_address[ix].c_str(), rc);	

    sgo_log_stderr(0, "#######################################%s %d MQTTClient_connect [%s] rc:%d\n", m_productCode.c_str(), m_devId, m_v_address[ix].c_str(), rc);

	if (rc != MQTTCLIENT_SUCCESS)
	{
		printf("subClient Failed to connect, return code %d\n", rc);
		return false;
	}

	int done = 0;

	vector<int> v_sub;
	for(int j = 0; j < m_subs.size(); j++)
	{
		if ((rc = MQTTClient_subscribe(client, m_subs[j].c_str(), 1)) == MQTTCLIENT_SUCCESS) {
			done++;
			v_sub.push_back(j);
		}
	}
    printf("sub done:%d,m_subs.size:%ld\n",done,m_subs.size());		
	if(done != m_subs.size())
	{
		for(int j=0;j<v_sub.size();j++)
			MQTTClient_unsubscribe(client, m_subs[v_sub[j]].c_str());
		MQTTClient_disconnect(client, 10000);
		return false;
	}

	for(int j=0;j<m_subs.size();j++)
	{
		sgo_log_stderr(0,"subtopic[%s]	successed.",m_subs[j].c_str());
		printf("subtopic[%s] successed.",m_subs[j].c_str());
	}

	return true;
}


void CMQTTCLIENT::sub()
{
	for(int i = 0;i < m_v_address.size();i++)
	{
		bool rc = subOne(m_v_clients[i],i);
		{
			//std::lock_guard<std::mutex> lock(m_losts_mutex);
			m_v_lost[i] = rc ? 0 : 1;
		}
		printf("[%d] --sub res:%d\n",i, rc);
	}
}

