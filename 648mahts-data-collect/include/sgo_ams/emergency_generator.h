#ifndef EMERGENCY_GENERATOR_H
#define EMERGENCY_GENERATOR_H


#include "cMqttClient.h"
#include "sgo_collect_data.h"
#include "blockqueue.h"
#include <string>


class EMERGENCY_GENERATOR : public CMQTTCLIENT{
public:
	EMERGENCY_GENERATOR();
	virtual ~EMERGENCY_GENERATOR();

	void init(int devId,BlockQueue<emergency_generator> &bq,modbus *mb,mutex *mux);


	void subClient();
	void pubClient(BlockQueue<emergency_generator> &bq);
	void reconn();
	void resnd();
	void chksensor();
	
	void start(BlockQueue<emergency_generator> &bq);

	bool UpdateHa(std::string key, uint16_t *data, int size);

	
	static int messageArrivedCallback(void* context, char* topicName, int topicLen, MQTTClient_message* message);
	static void connectionLostCallback(void* context, char* cause);
	static void deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt);

	std::string property_reply(char flag,const emergency_generator& one,const std::string& msg_id="");

	void init_mb_addr();
	void thSetRange();
	void thSetThreshold();
	void thSetInhibit();
	void thSetDelay();


	emergency_generator m_cur_data;
    emergency_generator_store m_data_store;
};

#endif



