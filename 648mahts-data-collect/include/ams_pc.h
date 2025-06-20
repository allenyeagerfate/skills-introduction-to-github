#ifndef AMS_PC_H
#define AMS_PC_H


#include "cMqttClient.h"
#include "sgo_collect_data.h"
#include "blockqueue.h"
#include <string>

class AMS_PC : public CMQTTCLIENT{
public:
	AMS_PC();
	virtual ~AMS_PC();

	void init(int devId,BlockQueue<ams_pc> &bq,modbus *mb1,mutex *mux1);



	void subClient();
	void pubClient(BlockQueue<ams_pc> &bq);
	void reconn();
	void resnd();
	void chksensor();
	
	void start(BlockQueue<ams_pc> &bq);

	
	static int messageArrivedCallback(void* context, char* topicName, int topicLen, MQTTClient_message* message);
	static void connectionLostCallback(void* context, char* cause);
	static void deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt);

	std::string property_reply(char flag,const ams_pc& one,const std::string& msg_id="");



	ams_pc m_cur_data;

	
};

#endif







