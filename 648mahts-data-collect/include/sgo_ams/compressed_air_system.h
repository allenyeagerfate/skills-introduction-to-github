#ifndef COMPRESSED_AIR_SYSTEM_H
#define COMPRESSED_AIR_SYSTEM_H


#include "cMqttClient.h"
#include "sgo_collect_data.h"
#include "blockqueue.h"
#include <string>

class COMPRESSED_AIR_SYSTEM : public CMQTTCLIENT{
public:
	COMPRESSED_AIR_SYSTEM();
	virtual ~COMPRESSED_AIR_SYSTEM();

	void init(int devId,BlockQueue<compressed_air_system> &bq,modbus* mb,mutex *mux);



	void subClient();
	void pubClient(BlockQueue<compressed_air_system> &bq);
	void reconn();
	void resnd();
	void chksensor();
	
	void start(BlockQueue<compressed_air_system> &bq);

	bool UpdateLa(std::string key, uint16_t *data, int size);
	
	static int messageArrivedCallback(void* context, char* topicName, int topicLen, MQTTClient_message* message);
	static void connectionLostCallback(void* context, char* cause);
	static void deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt);

	std::string property_reply(char flag,const compressed_air_system& one,const std::string& msg_id="");

	compressed_air_system m_cur_data;
    compressed_air_system_store m_data_store;

	void init_mb_addr();
	void thSetRange();
	void thSetThreshold();
	void thSetInhibit();
	void thSetDelay();	

};

#endif





