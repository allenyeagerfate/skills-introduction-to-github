#ifndef BAUDOUIN_MAIN_GENERATOR_H
#define BAUDOUIN_MAIN_GENERATOR_H


#include "cMqttClient.h"
#include "sgo_collect_data.h"
#include "blockqueue.h"
#include <string>
#include <mutex>

class BAUDOUIN_MAIN_GENERATOR : public CMQTTCLIENT{
public:
	BAUDOUIN_MAIN_GENERATOR();
	virtual ~BAUDOUIN_MAIN_GENERATOR();

	void init(int genId,BlockQueue<baudouin_main_generator> &bq,modbus *mb,std::mutex *mux);



	void subClient();
	void pubClient(BlockQueue<baudouin_main_generator> &bq);
	void reconn();
	void resnd();
	void chksensor();
	
	void start(BlockQueue<baudouin_main_generator> &bq);

	
	static int messageArrivedCallback(void* context, char* topicName, int topicLen, MQTTClient_message* message);
	static void connectionLostCallback(void* context, char* cause);
	static void deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt);

	std::string property_reply(char flag,const baudouin_main_generator& one,const std::string& msg_id="");


	void init_mb_addr();
	void thSetRange();
	void thSetThreshold();
	void thSetInhibit();
	void thSetDelay();
	
	baudouin_main_generator m_cur_data;

	
};

#endif

