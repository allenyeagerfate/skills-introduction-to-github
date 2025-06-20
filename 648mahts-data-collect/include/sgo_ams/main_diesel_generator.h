#ifndef MAIN_DIESEL_GENERATOR_H
#define MAIN_DIESEL_GENERATOR_H


#include "cMqttClient.h"
#include "sgo_collect_data.h"
#include "blockqueue.h"
#include <string>
#include <mutex>

class MAIN_DIESEL_GENERATOR : public CMQTTCLIENT{
public:
	MAIN_DIESEL_GENERATOR();
	virtual ~MAIN_DIESEL_GENERATOR();

	void init(int genId,BlockQueue<main_diesel_generator> &bq,modbus *mb,std::mutex *mux);

    bool UpdateHa(std::string key, uint16_t *data, int size);

	void subClient();
	void pubClient(BlockQueue<main_diesel_generator> &bq);
	void reconn();
	void resnd();
	void chksensor();
	
	void start(BlockQueue<main_diesel_generator> &bq);

	
	static int messageArrivedCallback(void* context, char* topicName, int topicLen, MQTTClient_message* message);
	static void connectionLostCallback(void* context, char* cause);
	static void deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt);

	std::string property_reply(char flag,const main_diesel_generator& one,const std::string& msg_id="");


	void init_mb_addr();
	void thSetRange();
	void thSetThreshold();
	void thSetInhibit();
	void thSetDelay();
	
	main_diesel_generator m_cur_data;
	main_diesel_generator_store m_data_store;
};

#endif
