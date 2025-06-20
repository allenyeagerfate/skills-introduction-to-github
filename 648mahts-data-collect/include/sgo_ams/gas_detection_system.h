#ifndef GAS_DETECTION_SYSTEM_H
#define GAS_DETECTION_SYSTEM_H


#include "cMqttClient.h"
#include "sgo_collect_data.h"
#include "blockqueue.h"
#include <string>

class GAS_DETECTION_SYSTEM : public CMQTTCLIENT{
public:
	GAS_DETECTION_SYSTEM();
	virtual ~GAS_DETECTION_SYSTEM();

	void init(int devId,BlockQueue<gas_detection_system> &bq,modbus* mb,mutex* mux);

	void subClient();
	void pubClient(BlockQueue<gas_detection_system> &bq);
	void reconn();
	void resnd();
	void chksensor();
	void thSetDelay();
	void thSetInhibit();
	void init_mb_addr();
	
	void start(BlockQueue<gas_detection_system> &bq);

	
	static int messageArrivedCallback(void* context, char* topicName, int topicLen, MQTTClient_message* message);
	static void connectionLostCallback(void* context, char* cause);
	static void deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt);

	std::string property_reply(char flag,const gas_detection_system& one,const std::string& msg_id="");

	gas_detection_system m_cur_data;

	
};

#endif





