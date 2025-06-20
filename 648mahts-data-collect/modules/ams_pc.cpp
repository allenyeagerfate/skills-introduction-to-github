#include <iostream>
#include <unistd.h>

#include "sgo_sub_pub.h"
#include "sgo_func.h"
#include "ams_pc.h"
#include "modbus.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

using namespace std;

const char* ams_pc_str[]={
	"ip",
	"name",
	"location",
	"current",
	"status",
	"cpu_usage",
	"mem_usage",
	"disk_free",
	"device_offline",	
};

	
static const char* productCode = "ams_pc";

AMS_PC::AMS_PC()
{}

AMS_PC::~AMS_PC()
{}

void AMS_PC::init(int devId,BlockQueue<ams_pc> &bq,modbus *mb,mutex *mux)
{
	m_mb_plc1 = mb;
	m_mux_plc1 = mux;

	m_devId = devId;
	
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
void AMS_PC::connectionLostCallback(void* context, char* cause) 
{
	// 处理连接丢失事件
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	AMS_PC* client = static_cast<AMS_PC*>(ctx->manager);
	
	printf("--%s %d--address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());
	sgo_log_stderr(0,"--%s %d----address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());

	//std::lock_guard<std::mutex> lock(m_losts_mutex);
	client->m_v_lost[ctx->index] = 1;
	client->m_v_property_index_array.clear();

}


// 消息到达回调函数
int AMS_PC::messageArrivedCallback(void* context, char* topicName, int topicLen,MQTTClient_message* message) 
{
	// 处理接收到的消息
	CallbackContext* ctx = static_cast<CallbackContext*>(context);
	AMS_PC* client = static_cast<AMS_PC*>(ctx->manager);
	
	printf("--%s %d--address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());
	sgo_log_stderr(0,"--%s %d----address[%d]:%s\n",__func__,__LINE__, ctx->index,client->m_v_address[ctx->index].c_str());


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
cout<<"size;"<<client->m_property_size<<endl;		
		for(int i = 0; i < client->m_property_size;i++)
			client->m_property[ams_pc_str[i]]=i;
		
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
void AMS_PC::deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt) 
{
	// 处理交付完成事件
	//std::cout << "Message delivery complete." << std::endl;
}

void AMS_PC::subClient()
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


void AMS_PC::pubClient(BlockQueue<ams_pc>& bq)
{
	while(1)
	{
		MQTTClient_message pubmsg = MQTTClient_message_initializer;
		//声明消息token
		MQTTClient_deliveryToken token;

		while(!m_pubStop){
			ams_pc one = bq.take();
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
				{
					//printf("address[%d]:%s,ip:%s\n",i,m_v_address[i].c_str(),one.ip.c_str());
				
					if( m_v_address[i].find(one.ip) != string::npos)
					{
						one.current = 1;
					}
					else
						one.current = 0;
					
					payload = property_reply(1,one);
					//printf("-------[%d]:%s\n",i,payload.c_str());
					if(payload.empty())
					{
						continue;
					}	
					pubmsg.payload = (void*)payload.c_str();
					pubmsg.payloadlen = payload.length();
					
					MQTTClient_publishMessage(m_v_clients[i], m_pubs[m_report_pub_index].c_str(), &pubmsg, &token);
				}
				else
				{
					m_bq_resend[i].offer(payload);
		
				}
			}


		}		
		sleep(3);
	}
}




void AMS_PC::start(BlockQueue<ams_pc> &bq)
{
	m_sub = new thread(&AMS_PC::subClient,this);
	m_sub->detach();

	m_pub = new thread(&AMS_PC::pubClient,this,ref(bq));
	m_pub->detach();

	m_thr_reconn = new thread(&AMS_PC::reconn,this);
	m_thr_reconn->detach();

	m_thr_resend = new thread(&AMS_PC::resnd,this);
	m_thr_resend->detach();


}

void AMS_PC::resnd()
{
	resend();
}

void AMS_PC::reconn()
{
	reconnect();
}


string AMS_PC::property_reply(char flag,const ams_pc& one,const string& msg_id)
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

	if(0 == m_property_size)
		return string();
	if(m_v_index.empty())
		return string();

	writer.Key("compressData");
	writer.StartArray();

	for(int i = 0; i < m_v_index.size(); i++)
	{
		switch(m_v_index[i]){
			case 0:writer.String(one.ip.c_str());break;
			case 1:writer.String(one.name.c_str());break;
			case 2:writer.String(one.location.c_str());break;
			case 3:writer.Int(one.current);break;
			case 4:writer.Int(one.status);break;
			case 5:writer.Double(0.0);break;
			case 6:writer.Double(0.0);break;
			case 7:writer.Double(500.0);break;
			case 8:writer.Int(one.device_offline);break;



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

	return strBuf.GetString();
}






