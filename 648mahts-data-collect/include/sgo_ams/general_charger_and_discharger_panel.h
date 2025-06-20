#ifndef GENERAL_CHARGER_AND_DISCHARGER_PANEL_H
#define GENERAL_CHARGER_AND_DISCHARGER_PANEL_H


#include "cMqttClient.h"
#include "sgo_collect_data.h"
#include "blockqueue.h"
#include <string>

class GENERAL_CHARGER_AND_DISCHARGER_PANEL : public CMQTTCLIENT{
public:
	GENERAL_CHARGER_AND_DISCHARGER_PANEL();
	virtual ~GENERAL_CHARGER_AND_DISCHARGER_PANEL();

	void init(int devId,BlockQueue<general_charger_and_discharger_panel> &bq,modbus* mb,mutex *mux);



	void subClient();
	void pubClient(BlockQueue<general_charger_and_discharger_panel> &bq);
	void reconn();
	void resnd();
	void chksensor();
	
	void start(BlockQueue<general_charger_and_discharger_panel> &bq);

	
	static int messageArrivedCallback(void* context, char* topicName, int topicLen, MQTTClient_message* message);
	static void connectionLostCallback(void* context, char* cause);
	static void deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt);

	std::string property_reply(char flag,const general_charger_and_discharger_panel& one,const std::string& msg_id="");

	general_charger_and_discharger_panel m_cur_data;

	void init_mb_addr();

	void thSetInhibit();
	void thSetDelay();	
};

#endif






