#ifndef STBD_AZIMUTH_CENTER_CONTROL_CABINET_H
#define STBD_AZIMUTH_CENTER_CONTROL_CABINET_H


#include "cMqttClient.h"
#include "sgo_collect_data.h"
#include "blockqueue.h"
#include <string>

class STBD_AZIMUTH_CENTER_CONTROL_CABINET : public CMQTTCLIENT{
public:
	STBD_AZIMUTH_CENTER_CONTROL_CABINET();
	virtual ~STBD_AZIMUTH_CENTER_CONTROL_CABINET();

	void init(int devId,BlockQueue<stbd_azimuth_center_control_cabinet> &bq,modbus *mb,std::mutex *mux);



	void subClient();
	void pubClient(BlockQueue<stbd_azimuth_center_control_cabinet> &bq);
	void reconn();
	void resnd();
	void chksensor();
	
	void start(BlockQueue<stbd_azimuth_center_control_cabinet> &bq);

	
	static int messageArrivedCallback(void* context, char* topicName, int topicLen, MQTTClient_message* message);
	static void connectionLostCallback(void* context, char* cause);
	static void deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt);

	std::string property_reply(char flag,const stbd_azimuth_center_control_cabinet& one,const std::string& msg_id="");

	stbd_azimuth_center_control_cabinet m_cur_data;
	stbd_azimuth_center_control_cabinet_store m_cur_data_store;
	void init_mb_addr();
	void thSetRange();
	void thSetThreshold();
	void thSetInhibit();
	void thSetDelay();		
};

#endif







