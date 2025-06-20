#ifndef C_MQTTCLIENT_H
#define C_MQTTCLIENT_H

#include <string>
#include <vector>
#include <MQTTClient.h>
#include <thread>
#include <mutex>
#include <atomic>
#include <map>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>


#include "blockqueue.h"
#include "sgo_collect_data.h"
#include "modbus.h"

class CMQTTCLIENT
{
public:
    CMQTTCLIENT();
    virtual ~CMQTTCLIENT();

	void setProxy(const std::string& proxy);
	void setSubTopics();
	void setPubTopics();

	void parseProp(const char *sdata);
	void parseInvoke(const char *sdata);
	void parseRange(const rapidjson::Value& array,int len);
	void parseThreshold(const rapidjson::Value& array,int len);
	void parseInhibit(const rapidjson::Value& array,int len,bool isInhib);
	void parseDelay(const rapidjson::Value& array,int len);
	void parseDuty(const rapidjson::Value& array,int len);
    void parseExtendAlarmPanel(const rapidjson::Value& array, int len);

	void reconnect();
	void resend();
	void reSendLost(int i);	
	void checkSensorStatus();
	std::string generateFakeMsgId();

	void setThreshold();
	void setDelay();
	void setRange();
	void setInhibit(int addr,int need);
	void setDeviceDuty();
	bool subOne(const MQTTClient &client,int ix);

	virtual void SetCall(uint32_t call) {}; //

	int cron(){return m_crons;}

	void sendMessage(const std::string& payload,int index);

	static bool compare_property(const property_index_array& a,const property_index_array& b){ return a.m_index < b.m_index;}

	void sub();

	char m_lost;//0:not lost 1:lost
	std::vector<char> m_v_lost;//0:not lost 1:lost,pc[i]
	char m_lost_1;
	char m_lost_2;
	char m_lost_3;
	char m_lost_4;
	//std::mutex m_losts_mutex;

     BlockQueue<onduty> bq_onduty;
	 uint16_t* m_duty = nullptr;
	 std::string* m_pCurIpAddr = nullptr;
protected:
    std::string m_address;   
	MQTTClient m_client;

	std::vector<std::string> m_v_address;
	std::vector<MQTTClient> m_v_clients;
	std::vector<MQTTClient_connectOptions> m_conn_opts;

    std::string m_clientId;
    int m_qos;
    std::vector<std::string> m_subs;
    std::vector<std::string> m_pubs;
	int m_report_pub_index;
	int m_cur_proxy_id;

	char m_pubStop; //1:stop 0:start



	int m_crons;
	std::string m_msgPubId; //上报id
	std::string m_msgReplyId;//回复id
	std::string m_productCode;
	std::string m_deviceCode;
	std::string m_readType;
	bool m_isInvokeByRule;
	int m_devId; //从1开始
    //int m_moduleName;

	uint32_t m_u32Call =  0;
	std::string m_funcKey;
	std::string m_funcName;
	std::vector<std::string> m_vInParams;
	std::vector<std::string> m_vOutParams;

	std::thread *m_pub;
	std::thread *m_sub;
	std::thread *m_thr_reconn;
	std::thread *m_thr_resend;
	std::thread *m_thr_chksensor;

	std::string m_cur_threshold_key;
	std::mutex m_mux_threshold;
	BlockQueue<range> bq_range;
	BlockQueue<threshold> bq_threshold;
	BlockQueue<delay> bq_delay;
	BlockQueue<inhibit> bq_inhibit;
	BlockQueue<extend_alarm_panel> bq_extend_alarm_panel;


	std::map<std::string,int> m_mp_range_addr;
	std::map<std::string,int> m_mp_threshold_addr;
	std::map<std::string,int> m_mp_delay_addr;	//addr
	std::map<std::string,int> m_mp_inhibit_addr; //mask addr

	std::vector<property_index_array> m_v_property_index_array;
	std::map<std::string,int> m_property; //字段对应位置
	std::vector<int> m_v_index;//pub发送下标
	int m_property_size;


	BlockQueue<std::string> m_bq_resend[pc_nums];
public:
	char m_sensor_exception;

	std::mutex mux_cur_data;
	bool m_func_res;
	modbus *m_mb_plc1;
	modbus *m_mb_plc2;
	std::mutex *m_mux_plc1;
	std::mutex *m_mux_plc2;

	std::mutex m_mux_arrive;

	std::map<MQTTClient,char> m_mp_client;


};

struct CallbackContext {
    void* manager; // 指向 CMQTTCLIENT 实例的指针
    int index;  // 客户端的索引
};


#endif
