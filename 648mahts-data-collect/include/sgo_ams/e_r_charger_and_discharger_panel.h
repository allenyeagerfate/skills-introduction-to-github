#ifndef E_R_CHARGER_AND_DISCHARGER_PANEL_H
#define E_R_CHARGER_AND_DISCHARGER_PANEL_H


#include "cMqttClient.h"
#include "sgo_collect_data.h"
#include "blockqueue.h"
#include <string>

class E_R_CHARGER_AND_DISCHARGER_PANEL : public CMQTTCLIENT{
public:
	E_R_CHARGER_AND_DISCHARGER_PANEL();
	virtual ~E_R_CHARGER_AND_DISCHARGER_PANEL();

	void init(int devId,BlockQueue<e_r_charger_and_discharger_panel> &bq,modbus* mb,std::mutex* mux);



	void subClient();
	void pubClient(BlockQueue<e_r_charger_and_discharger_panel> &bq);
	void reconn();
	void resnd();
	void chksensor();
	
	void start(BlockQueue<e_r_charger_and_discharger_panel> &bq);

	
	static int messageArrivedCallback(void* context, char* topicName, int topicLen, MQTTClient_message* message);
	static void connectionLostCallback(void* context, char* cause);
	static void deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt);

	std::string property_reply(char flag,const e_r_charger_and_discharger_panel& one,const std::string& msg_id="");

	e_r_charger_and_discharger_panel m_cur_data;
	e_r_charger_and_discharger_panel_store m_data_store;
	void init_mb_addr();
	void thSetInhibit();
	void thSetDelay();	

	
};

#endif







