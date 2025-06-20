#ifndef BOW_THRUSTER_H
#define BOW_THRUSTER_H


#include "cMqttClient.h"
#include "sgo_collect_data.h"
#include "blockqueue.h"
#include <string>

class BOW_THRUSTER : public CMQTTCLIENT{
public:
	BOW_THRUSTER();
	virtual ~BOW_THRUSTER();

	void init(int devId,BlockQueue<bow_thruster> &bq,modbus *mb,std::mutex *mux);



	void subClient();
	void pubClient(BlockQueue<bow_thruster> &bq);
	void reconn();
	void resnd();
	void chksensor();
	
	void start(BlockQueue<bow_thruster> &bq);

	
	static int messageArrivedCallback(void* context, char* topicName, int topicLen, MQTTClient_message* message);
	static void connectionLostCallback(void* context, char* cause);
	static void deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt);

	std::string property_reply(char flag,const bow_thruster& one,const std::string& msg_id="");

	bow_thruster m_cur_data;
	bow_thruster_store m_cur_data_store;

	void init_mb_addr();
	void thSetRange();
	void thSetThreshold();
	void thSetInhibit();
	void thSetDelay();		
};

#endif







