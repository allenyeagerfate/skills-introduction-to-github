#ifndef AIR_CONDITIONER_REFIGERATION_SYSTEM_H
#define AIR_CONDITIONER_REFIGERATION_SYSTEM_H


#include "cMqttClient.h"
#include "sgo_collect_data.h"
#include "blockqueue.h"
#include <string>

class AIR_CONDITIONER_REFIGERATION_SYSTEM : public CMQTTCLIENT{
public:
	AIR_CONDITIONER_REFIGERATION_SYSTEM();
	virtual ~AIR_CONDITIONER_REFIGERATION_SYSTEM();

	void init(int devId,BlockQueue<air_conditioner_refrigeration_system> &bq,modbus *mb,std::mutex *mux);
	



	void subClient();
	void pubClient(BlockQueue<air_conditioner_refrigeration_system> &bq);
	void reconn();
	void resnd();
	void chksensor();
	
	void start(BlockQueue<air_conditioner_refrigeration_system> &bq);

	
	static int messageArrivedCallback(void* context, char* topicName, int topicLen, MQTTClient_message* message);
	static void connectionLostCallback(void* context, char* cause);
	static void deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt);

	std::string property_reply(char flag,const air_conditioner_refrigeration_system& one,const std::string& msg_id="");

	air_conditioner_refrigeration_system m_cur_data;
	air_conditioner_refrigeration_system_store m_data_store;

	void init_mb_addr();

	void thSetInhibit();
	void thSetDelay();	
};

#endif





