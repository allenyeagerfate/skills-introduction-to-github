#ifndef LEVEL_GAUGING_H
#define LEVEL_GAUGING_H


#include "cMqttClient.h"
#include "sgo_collect_data.h"
#include "blockqueue.h"
#include <string>

class LEVEL_GAUGING : public CMQTTCLIENT{
public:
	LEVEL_GAUGING();
	virtual ~LEVEL_GAUGING();

	void init(int devId,BlockQueue<level_gauging> &bq,modbus *mb,mutex *mux);



	void subClient();
	void pubClient(BlockQueue<level_gauging> &bq);
	void reconn();
	void resnd();
	void chksensor();
	
	void start(BlockQueue<level_gauging> &bq);

	
	static int messageArrivedCallback(void* context, char* topicName, int topicLen, MQTTClient_message* message);
	static void connectionLostCallback(void* context, char* cause);
	static void deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt);

	std::string property_reply(char flag,const level_gauging& one,const std::string& msg_id = "");

	level_gauging m_cur_data;

	void init_mb_addr();
	void thSetRange();
	void thSetThreshold();
	void thSetInhibit();
	void thSetDelay();

	
};

#endif









