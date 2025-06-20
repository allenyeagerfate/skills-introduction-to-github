#ifndef LEVEL_SWITCH_H
#define LEVEL_SWITCH_H


#include "cMqttClient.h"
#include "sgo_collect_data.h"
#include "blockqueue.h"
#include <string>

class LEVEL_SWITCH : public CMQTTCLIENT{
public:
	LEVEL_SWITCH();
	virtual ~LEVEL_SWITCH();

	void init(int devId,BlockQueue<level_switch> &bq,modbus* mb,std::mutex* mux);



	void subClient();
	void pubClient(BlockQueue<level_switch> &bq);
	void reconn();
	void resnd();
	void chksensor();
	
	void start(BlockQueue<level_switch> &bq);

	
	static int messageArrivedCallback(void* context, char* topicName, int topicLen, MQTTClient_message* message);
	static void connectionLostCallback(void* context, char* cause);
	static void deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt);

	std::string property_reply(char flag,const level_switch& one,const std::string& msg_id = "");

	level_switch m_cur_data;
	void init_mb_addr();
	void thSetInhibit();
	void thSetDelay();	

	
};

#endif









