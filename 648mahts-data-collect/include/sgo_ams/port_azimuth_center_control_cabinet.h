#ifndef PORT_AZIMUTH_CENTER_CONTROL_CABINET_H
#define PORT_AZIMUTH_CENTER_CONTROL_CABINET_H


#include "cMqttClient.h"
#include "sgo_collect_data.h"
#include "blockqueue.h"
#include <string>

class PORT_AZIMUTH_CENTER_CONTROL_CABINET : public CMQTTCLIENT{
public:
	PORT_AZIMUTH_CENTER_CONTROL_CABINET();
	virtual ~PORT_AZIMUTH_CENTER_CONTROL_CABINET();

	void init(int devId,BlockQueue<port_azimuth_center_control_cabinet> &bq,modbus *mb,std::mutex *mux);



	void subClient();
	void pubClient(BlockQueue<port_azimuth_center_control_cabinet> &bq);
	void reconn();
	void resnd();
	void chksensor();
	
	void start(BlockQueue<port_azimuth_center_control_cabinet> &bq);

	
	static int messageArrivedCallback(void* context, char* topicName, int topicLen, MQTTClient_message* message);
	static void connectionLostCallback(void* context, char* cause);
	static void deliveryCompleteCallback(void* context, MQTTClient_deliveryToken dt);

	std::string property_reply(char flag,const port_azimuth_center_control_cabinet& one,const std::string& msg_id="");

	port_azimuth_center_control_cabinet m_cur_data;
	port_azimuth_center_control_cabinet_store m_cur_data_store;

	void init_mb_addr();
	void thSetRange();
	void thSetThreshold();
	void thSetInhibit();
	void thSetDelay();		
};

#endif







