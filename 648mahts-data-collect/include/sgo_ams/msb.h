#ifndef MSB_H
#define MSB_H


#include "cMqttClient.h"
#include "sgo_collect_data.h"
#include "blockqueue.h"
#include <string>

class MSB : public CMQTTCLIENT{
public:
	MSB();
	virtual ~MSB();

	void init(int devId,BlockQueue<msb> &bq,modbus *mb,mutex *mux);



	void subClient();
	void pubClient(BlockQueue<msb> &bq);
	void reconn();
	void resnd();
	void chksensor();
	
	void start(BlockQueue<msb> &bq);

	
	static int messageArrivedCallback(void* context, char* topicName, int topicLen, MQTTClient_message* message);
	static void connectionLostCallback(void* context, char* cause);
	static void deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt);

	std::string property_reply(char flag,const msb& one,const std::string& msg_id="");

	msb m_cur_data;

	void init_mb_addr();
	void thSetRange();
	void thSetThreshold();
	void thSetInhibit();
	void thSetDelay();	
};

#endif

