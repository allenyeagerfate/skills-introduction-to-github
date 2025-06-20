#ifndef COOLING_WATER_SYSTEM_H
#define COOLING_WATER_SYSTEM_H


#include "cMqttClient.h"
#include "sgo_collect_data.h"
#include "blockqueue.h"
#include <string>

class COOLING_WATER_SYSTEM : public CMQTTCLIENT{
public:
	COOLING_WATER_SYSTEM();
	virtual ~COOLING_WATER_SYSTEM();

	void init(int devId,BlockQueue<cooling_water_system> &bq,modbus *mb,mutex *mux);
	



	void subClient();
	void pubClient(BlockQueue<cooling_water_system> &bq);
	void reconn();
	void resnd();
	void chksensor();
	
	void start(BlockQueue<cooling_water_system> &bq);

	bool UpdateLa(std::string key, uint16_t *data, int size);

	static int messageArrivedCallback(void* context, char* topicName, int topicLen, MQTTClient_message* message);
	static void connectionLostCallback(void* context, char* cause);
	static void deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt);

	std::string property_reply(char flag,const cooling_water_system& one,const std::string& msg_id="");


	cooling_water_system m_cur_data;
	cooling_water_system_store m_data_store;

	void init_mb_addr();
	void thSetRange();
	void thSetThreshold();
	void thSetInhibit();
	void thSetDelay();		
};

#endif







