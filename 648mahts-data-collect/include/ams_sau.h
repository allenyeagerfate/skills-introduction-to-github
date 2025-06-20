#ifndef AMS_SAU_H
#define AMS_SAU_H


#include "cMqttClient.h"
#include "sgo_collect_data.h"
#include "blockqueue.h"
#include <string>

class AMS_SAU : public CMQTTCLIENT{
public:
	AMS_SAU();
	virtual ~AMS_SAU();

	void init(int devId,BlockQueue<ams_sau> &bq,modbus *mb1,mutex *mux1);



	void subClient();
	void pubClient(BlockQueue<ams_sau> &bq);
	void reconn();
	void resnd();
	void chksensor();
	
	void start(BlockQueue<ams_sau> &bq);

	
	static int messageArrivedCallback(void* context, char* topicName, int topicLen, MQTTClient_message* message);
	static void connectionLostCallback(void* context, char* cause);
	static void deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt);

	std::string property_reply(char flag,const ams_sau& one,const std::string& msg_id="");



	ams_sau m_cur_data;

	
};

#endif








