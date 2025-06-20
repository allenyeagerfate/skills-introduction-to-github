#ifndef MISCELLANEOUS_H
#define MISCELLANEOUS_H


#include "cMqttClient.h"
#include "sgo_collect_data.h"
#include "blockqueue.h"
#include <string>

class MISCELLANEOUS : public CMQTTCLIENT{
public:
	MISCELLANEOUS();
	virtual ~MISCELLANEOUS();

	void init(int devId,BlockQueue<miscellaneous> &bq,modbus *mb,mutex *mux);



	void subClient();
	void pubClient(BlockQueue<miscellaneous> &bq);
	void reconn();
	void resnd();
	void chksensor();

	void start(BlockQueue<miscellaneous> &bq);

	
	static int messageArrivedCallback(void* context, char* topicName, int topicLen, MQTTClient_message* message);
	static void connectionLostCallback(void* context, char* cause);
	static void deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt);

	std::string property_reply(char flag,const miscellaneous& one,const std::string& msg_id="");

	miscellaneous m_cur_data;
	miscellaneous_store m_data_store;
	
	void init_mb_addr();
	void thSetRange();
	void thSetThreshold();
	void thSetInhibit();
	void thSetDelay();		

    
	
};

#endif






