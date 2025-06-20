#ifndef MAIN_TRANSFORMER_H
#define MAIN_TRANSFORMER_H


#include "cMqttClient.h"
#include "sgo_collect_data.h"
#include "blockqueue.h"
#include <string>

class MAIN_TRANSFORMER : public CMQTTCLIENT{
public:
	MAIN_TRANSFORMER();
	virtual ~MAIN_TRANSFORMER();

	void init(int devId,BlockQueue<main_transformer> &bq,modbus* mb,mutex* mux);



	void subClient();
	void pubClient(BlockQueue<main_transformer> &bq);
	void reconn();
	void resnd();
	void chksensor();
	
	void start(BlockQueue<main_transformer> &bq);

	bool UpdateHa(std::string key, uint16_t *data, int size);

	
	static int messageArrivedCallback(void* context, char* topicName, int topicLen, MQTTClient_message* message);
	static void connectionLostCallback(void* context, char* cause);
	static void deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt);

	std::string property_reply(char flag,const main_transformer& one,const std::string& msg_id="");

	main_transformer m_cur_data;
	main_transformer_store m_data_store;
	
	void init_mb_addr();
	void thSetRange();
	void thSetThreshold();
	void thSetInhibit();
	void thSetDelay();	

	
};

#endif






