#ifndef CO2_SYSTEM_H
#define CO2_SYSTEM_H


#include "cMqttClient.h"
#include "sgo_collect_data.h"
#include "blockqueue.h"
#include <string>

class CO2_SYSTEM : public CMQTTCLIENT{
public:
	CO2_SYSTEM();
	virtual ~CO2_SYSTEM();

	void init(int devId,BlockQueue<co2_system> &bq,modbus *mb,mutex *mux);

	void subClient();
	void pubClient(BlockQueue<co2_system> &bq);
	void reconn();
	void resnd();
	void chksensor();

	void start(BlockQueue<co2_system> &bq);

	
	static int messageArrivedCallback(void* context, char* topicName, int topicLen, MQTTClient_message* message);
	static void connectionLostCallback(void* context, char* cause);
	static void deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt);

	std::string property_reply(char flag,const co2_system& one,const std::string& msg_id="");

	co2_system m_cur_data;
	co2_system_store m_data_store;
	
	void init_mb_addr();
	void thSetRange();
	void thSetThreshold();
	void thSetInhibit();
	void thSetDelay();
};

#endif






