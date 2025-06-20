#ifndef MAIN_PROPULSION_H
#define MAIN_PROPULSION_H

#include "cMqttClient.h"
#include "sgo_collect_data.h"
#include "blockqueue.h"
#include <string>

class MAIN_PROPULSION : public CMQTTCLIENT{
public:
	MAIN_PROPULSION();
	virtual ~MAIN_PROPULSION();

	void init(int devId,BlockQueue<main_propulsion> &bq,modbus *mb,mutex *mux);



	void subClient();
	void pubClient(BlockQueue<main_propulsion> &bq);
	void reconn();
	void resnd();
	void chksensor();
	
	void start(BlockQueue<main_propulsion> &bq);

	bool UpdateLa(std::string key, uint16_t *data, int size);

	static int messageArrivedCallback(void* context, char* topicName, int topicLen, MQTTClient_message* message);
	static void connectionLostCallback(void* context, char* cause);
	static void deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt);

	std::string property_reply(char flag,const main_propulsion& one,const std::string& msg_id="");

	main_propulsion m_cur_data;
	main_propulsion_store m_data_store;

	void init_mb_addr();
	void thSetRange();
	void thSetThreshold();
	void thSetInhibit();
	void thSetDelay();

	
};

#endif
