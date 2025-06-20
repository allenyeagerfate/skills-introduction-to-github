#ifndef POWER_SYSTEM_H
#define POWER_SYSTEM_H


#include "cMqttClient.h"
#include "sgo_collect_data.h"
#include "blockqueue.h"
#include <string>

class POWER_SYSTEM : public CMQTTCLIENT{
public:
	POWER_SYSTEM();
	virtual ~POWER_SYSTEM();

	void init(int devId,BlockQueue<power_system> &bq,modbus *mb,mutex *mux);



	void subClient();
	void pubClient(BlockQueue<power_system> &bq);
	void reconn();
	void resnd();
	void chksensor();
	
	void start(BlockQueue<power_system> &bq);

	
	static int messageArrivedCallback(void* context, char* topicName, int topicLen, MQTTClient_message* message);
	static void connectionLostCallback(void* context, char* cause);
	static void deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt);

	std::string property_reply(char flag,const power_system& one,const std::string& msg_id="");

	power_system m_cur_data;

	void init_mb_addr();
	void thSetRange();
	void thSetThreshold();
	void thSetInhibit();
	void thSetDelay();	
};

#endif

