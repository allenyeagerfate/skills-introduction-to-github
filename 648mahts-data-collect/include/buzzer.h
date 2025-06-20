#ifndef BUZZER_H
#define BUZZER_H


#include "cMqttClient.h"
#include "sgo_collect_data.h"
#include "blockqueue.h"
#include <string>

class BUZZER : public CMQTTCLIENT{
public:
	BUZZER();
	virtual ~BUZZER();

	void init(int devId,BlockQueue<buzzer> &bq,modbus *mb1,mutex *mux1);



	void subClient();
	void pubClient(BlockQueue<buzzer> &bq);
	void reconn();
	void resnd();
	void chksensor();
	
	void start(BlockQueue<buzzer> &bq);

	
	static int messageArrivedCallback(void* context, char* topicName, int topicLen, MQTTClient_message* message);
	static void connectionLostCallback(void* context, char* cause);
	static void deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt);

	std::string property_reply(char flag,const buzzer& one,const std::string& msg_id="");


	void operBuzzer(char flag);
	void turnOnBuzzer();
	void turnOffBuzzer();

	buzzer m_cur_data;

	BlockQueue<char> bq_oper_status;

};

#endif






