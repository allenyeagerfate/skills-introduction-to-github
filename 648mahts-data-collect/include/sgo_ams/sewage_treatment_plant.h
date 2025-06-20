#ifndef SEWAGE_TREATMENT_PLANT_H
#define SEWAGE_TREATMENT_PLANT_H


#include "cMqttClient.h"
#include "sgo_collect_data.h"
#include "blockqueue.h"
#include <string>

class SEWAGE_TREATMENT_PLANT : public CMQTTCLIENT{
public:
	SEWAGE_TREATMENT_PLANT();
	virtual ~SEWAGE_TREATMENT_PLANT();

	void init(int devId,BlockQueue<sewage_treatment_plant> &bq,modbus *mb,std::mutex *mux);



	void subClient();
	void pubClient(BlockQueue<sewage_treatment_plant> &bq);
	void reconn();
	void resnd();
	void chksensor();
	
	void start(BlockQueue<sewage_treatment_plant> &bq);

	
	static int messageArrivedCallback(void* context, char* topicName, int topicLen, MQTTClient_message* message);
	static void connectionLostCallback(void* context, char* cause);
	static void deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt);

	std::string property_reply(char flag,const sewage_treatment_plant& one,const std::string& msg_id="");

	sewage_treatment_plant m_cur_data;

	void init_mb_addr();
	void thSetRange();
	void thSetThreshold();
	void thSetInhibit();
	void thSetDelay();		
};

#endif







