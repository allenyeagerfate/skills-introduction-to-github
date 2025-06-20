#ifndef EXTEND_ALARM_PANEL_H
#define EXTEND_ALARM_PANEL_H


#include "cMqttClient.h"
#include "sgo_collect_data.h"
#include "blockqueue.h"
#include <string>

class EXTEND_ALARM_PANEL : public CMQTTCLIENT{
public:
	EXTEND_ALARM_PANEL();
	virtual ~EXTEND_ALARM_PANEL();

	void init(int devId,BlockQueue<extend_alarm_panel> &bq,modbus* mb1,mutex* mux1);


	void subClient();
	void pubClient(BlockQueue<extend_alarm_panel> &bq);
	void reconn();
	void resnd();
	void chksensor();
	void setduty();
	
	void start(BlockQueue<extend_alarm_panel> &bq);


	void set_call_bq(std::pair<char, char> status);
	
	static int messageArrivedCallback(void* context, char* topicName, int topicLen, MQTTClient_message* message);
	static void connectionLostCallback(void* context, char* cause);
	static void deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt);

	std::string property_reply(char flag,const extend_alarm_panel& one,const std::string& msg_id="");

	extend_alarm_panel m_cur_data;


    

	//BlockQueue<char> bq_call_status; //1:call 0:not call

    BlockQueue<std::pair<char, char>> bq_call_status; //1:call 0:not call

};

#endif







