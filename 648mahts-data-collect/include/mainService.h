#ifndef MAIN_SERVICE_H
#define MAIN_SERVICE_H

#include <string>
#include <thread>
#include <mutex>
#include <unistd.h>

#include "sgo_conf.h"
#include "blockqueue.h"
#include "sgo_func.h"
#include "bytesUtil.h"
#include "crc16.h"
#include "sgo_sub_pub.h"
#include "cMqttClient.h"
#include "tcpclient.h"

#include "modbus.h"
#include "sgo_collect_data.h"
#include "sgo_ams_mods.h"

#include "buzzer.h"
#include "extend_alarm_panel.h"
#include "ams_pc.h"
#include "ams_sau.h"
#include "CallbackQueue.h"

class AlarmsGroupHandler
{
public:
	AlarmsGroupHandler(CallbackQueue& callback, uint16_t addr)
	: m_callbackQueue(callback), m_addr(addr)
	{}
	~AlarmsGroupHandler()
	{}

     void SetModbus(modbus *mb, std::mutex *mtx)
	 {
         m_mb = mb;
		 m_mtx = mtx;
	 }


	void SetAlarmPendingOn()
	{
		m_callbackQueue.call([this]() {
            m_bAlarmPending = true;
		});
	}

	void SetCurrAlarms(std::vector<char> currAlarms)
	{
        
		m_callbackQueue.call([this, currAlarms]() {
            m_vecCurrAlarms = currAlarms;
		});
		
	}

    void SetCallStatus(bool status){
		m_callbackQueue.call([this, status]() {
            m_bCallStatus = status;
		});
	}

    void CheckCall()
	{
        m_callbackQueue.call([this]() {
			if (m_bAlarmPending)
			{
                //无条件消音
				if(m_bCallStatus)
				{
					if (m_mb && m_mtx && m_mb->is_connected())
					{
						std::lock_guard<std::mutex> lock(*m_mtx);
						m_mb->modbus_write_register(m_addr, 0);
					}
				}
				
				m_bAlarmPending = false;
                //m_vecPreAlarms = m_vecCurrAlarms;
			}
			else//(!m_bAlarmPending)
			{
                bool changed = AlarmsOnChanged();
                if (changed)
				{
					if (!m_bCallStatus)
					{
						if (m_mb && m_mtx && m_mb->is_connected())
						{
							std::lock_guard<std::mutex> lock(*m_mtx);
							auto ret = m_mb->modbus_write_register(m_addr, 1);
						}
					}

                    //m_vecPreAlarms = m_vecCurrAlarms;
				}
				else  // no change
				{
					//
                    auto isAnyAlarmOn = IsAnyAlarmOn();
					if(!isAnyAlarmOn)
					{
						// 消音
						if (m_bCallStatus)
						{
							if (m_mb && m_mtx && m_mb->is_connected())
							{
								std::lock_guard<std::mutex> lock(*m_mtx);
								m_mb->modbus_write_register(m_addr, 0);
							}
						}
					}
					else
					{
                          sgo_log_stderr(0, "Alarms are still on, no action taken.");
					}
				}
			}

             m_vecPreAlarms = m_vecCurrAlarms;
		});
	}

private:
    
    bool IsAnyAlarmOn()
	{
		bool ret = false;
		for(size_t i = 0; i < m_vecCurrAlarms.size(); ++i)
		{
			if(1 == m_vecCurrAlarms[i])
			{
				ret = true;
				break;
			}
		}

		return ret;
	}
    
	bool  AlarmsOnChanged()
	{
		bool ret = false;
			
		if(m_vecCurrAlarms.size() == m_vecPreAlarms.size())
	    {
            for(size_t i = 0; i < m_vecCurrAlarms.size(); ++i)
			{
				if((1 == m_vecCurrAlarms[i]) && (m_vecCurrAlarms[i] != m_vecPreAlarms[i]))
				{
					ret = true;
					break;
				}
			}
		}
		else
		{
            if(0 == m_vecPreAlarms.size())
			{
                if(IsAnyAlarmOn())
				{
					ret = true;
				}        
			}
		}
		
        return ret;
	}
    
    bool m_bAlarmPending = false;
    bool m_bCallStatus = false; //灯柱状态
  

	std::vector<char> m_vecCurrAlarms; //目前的信号
	std::vector<char> m_vecPreAlarms;  //保存操作时候的信号值

    CallbackQueue& m_callbackQueue;

	modbus* m_mb = nullptr; 
	std::mutex* m_mtx = nullptr;
	uint16_t m_addr = 0;
};


class mainService{
public:
	mainService(): m_callbackQueue(1024), 
	               m_mud_handler(m_callbackQueue, 3012),
				   m_level_handler(m_callbackQueue, 3011),
				   m_machinery_handler(m_callbackQueue, 3010)
	{}
	~mainService(){}

	int init();


private:
	void initMqtt(modbus *mb,std::mutex *mux);
	void initMqtt2();
	void GetHttpAlarms();

	void getDatasFromPLC1();
	void setAmsLost(int flag);
	void setMqttProxyLost(int pcIndex,int status);

	void gen_data_diesel_generator(int i);
	void gen_data_diesel_generator2(int i); //MAHTS648
	void gen_data_harbor_generator();	
	void gen_data_diesel_generator();
	void gen_data_diesel_generator2();	
	void gen_data_baudouin_main_generator();
	void gen_data_baudouin_main_generator(int i);  //TBD

	void gen_data_emergency_generator();
	void gen_data_emergency_generator2(); //MAHTS648
	
	void gen_data_main_propulsion();
	void gen_data_main_propulsion2();
	void gen_data_main_propulsion(int i);
	void gen_data_main_propulsion2(int i); //MAHTS648

	void gen_data_power_system();
	void gen_data_power_system2();  //MAHTS648
	void gen_data_msb(); //MAHTS648
	void gen_data_power_system2_2(); //MAHTS648
	
	void gen_data_air_conditioner_refrigeration_system();
	void gen_data_air_conditioner_refrigeration_system2(); //MAHTS648
	void gen_data_compressed_air_system();
	void gen_data_compressed_air_system2();  //MAHTS648
	void gen_data_general_charger_and_discharger_panel();
	void gen_data_general_charger_and_discharger_panel2();  //MAHTS648
	void gen_data_main_transformer();
	void gen_data_main_transformer2(int i); //MAHTS648
	void gen_data_bow_thruster();

	void gen_data_bow_thruster2(int i); //MAHTS648

    void gen_data_sewage_treatment_plant();//MAHTS648

    void gen_data_port_azimuth_center_control_cabinet(); ////MAHTS648
	void gen_data_stbd_azimuth_center_control_cabinet();//MAHTS648

	void gen_data_e_r_charger_and_discharger_panel();
	void gen_data_e_r_charger_and_discharger_panel2(); //MAHTS648

	void gen_data_level_gauging();
    void gen_data_level_gauging2(); //MAHTS648

	void gen_data_level_switch();
	void gen_data_level_switch2();	
	void gen_data_gas_detection_system();
	void gen_data_gas_detection_system2();  //MAHTS648
	void gen_data_miscellaneous();
	void gen_data_miscellaneous2();  //MAHTS648

    void gen_data_co2_system(); 


	void gen_data_cooling_water_system();
	void gen_data_cooling_water_system2(); //MAHTS648

	void gen_data_buzzer_and_extend_panel();
	void gen_data_buzzer_and_extend_panel2(); //MAHTS648


	void gen_data_buzzer_and_extend_alarm_panel_test();

	////////////////
	void check_sau_connection();

	//modbus write
	void write_main_gens(int i);
	void write_emergency(int i);
	void write_propulsion(int i);
	void write_power_system(int i);
	void write_miscellaneous(int i);
	void write_level(int i);
	void write_gas(int i);

private:
	CConfig *m_pConfig;
		
	std::thread *m_thr_mqtt;
	std::thread *m_thr_ams;

	char m_core_mode;//0:digital 1:smart 2:digital and smart 
	std::string m_digital_mqtt_ip;
	std::string m_smart_mqtt_ip;
	bool fire_alarm_trige = false;
    //main_diesel_generator_store g_main_diesel[6];

    //uint16_t m_u16_mud_flag = 0; //OUTPUT MUD TK LEVEL ALARM BELL
	//uint16_t m_u16_machinery_flag = 0; //ALARM COLUMN MACHINERY ALARM
	//uint16_t m_u16_level_flag = 0; //ALARM COLUMN TANK LEVEL ALARM


    AlarmsGroupHandler m_mud_handler;
	AlarmsGroupHandler m_level_handler;
    AlarmsGroupHandler m_machinery_handler;

	CallbackQueue m_callbackQueue; 
	//uint16_t m_u16_loop_flag = 0; //
	std::vector<char> m_vecTmpMachinery;

public:
	///////////ams modules start//////////////////////////////////
#if defined(JANA505)
	MAIN_DIESEL_GENERATOR m_client_main_gens[6];
#endif
#ifdef JANA508509
	MAIN_DIESEL_GENERATOR m_client_main_gens[5];
	BAUDOUIN_MAIN_GENERATOR m_client_baudouin_main_gens[2];
#endif

#if defined(MAHTS648)
	MAIN_DIESEL_GENERATOR m_client_main_gens[4];
	BAUDOUIN_MAIN_GENERATOR m_client_baudouin_main_gens[2];
	HARBO_GENERATOR m_client_harbor_generator;
#endif

	EMERGENCY_GENERATOR m_client_emergency_generator;
	MAIN_PROPULSION m_client_main_propulsion[6];
	BOW_THRUSTER m_client_bow_thruster[2];
	POWER_SYSTEM m_client_power_system;
	GENERAL_CHARGER_AND_DISCHARGER_PANEL m_client_general_charger_and_discharger_panel;
	E_R_CHARGER_AND_DISCHARGER_PANEL m_client_e_r_charger_and_discharger_panel;
	MAIN_TRANSFORMER m_client_main_transformer[2];
	LEVEL_GAUGING m_client_level_gauging;
	LEVEL_SWITCH m_client_level_switch;
	GAS_DETECTION_SYSTEM m_client_gas_detection_system;
	COOLING_WATER_SYSTEM m_client_cooling_water_system;	
	COMPRESSED_AIR_SYSTEM m_client_compressed_air_system;
	AIR_CONDITIONER_REFIGERATION_SYSTEM m_client_air_conditioner_refrigeration_system;
	MISCELLANEOUS m_client_miscellaneous;
	CO2_SYSTEM m_client_co2_system;
    PORT_AZIMUTH_CENTER_CONTROL_CABINET m_client_port_azimuth_center_control_cabinet;
	STBD_AZIMUTH_CENTER_CONTROL_CABINET m_client_stbd_azimuth_center_control_cabinet;
	POWER_SYSTEM2 m_client_power_system2;
	SEWAGE_TREATMENT_PLANT m_client_sewage_treatment_plant;
    MSB m_client_msb;

	BlockQueue<main_diesel_generator> bq_main_diesel_generator[6];
	BlockQueue<harbo_generator> bq_harbor_generator;
	BlockQueue<baudouin_main_generator> bq_baudouin_main_generator[2];	
	BlockQueue<emergency_generator> bq_emergency_generator;
	BlockQueue<main_propulsion> bq_main_propulsion[6];
	BlockQueue<bow_thruster> bq_bow_thruster[2];
	BlockQueue<power_system> bq_power_system;
	BlockQueue<main_transformer> bq_main_transformer[2];
	BlockQueue<general_charger_and_discharger_panel> bq_general_charger_and_discharger_panel;
	BlockQueue<e_r_charger_and_discharger_panel> bq_e_r_charger_and_discharger_panel;
	BlockQueue<level_gauging> bq_level_gauging;
	BlockQueue<level_switch> bq_level_switch;
	BlockQueue<gas_detection_system> bq_gas_detection_system;
	BlockQueue<cooling_water_system> bq_cooling_water_system;
	BlockQueue<compressed_air_system> bq_compressed_air_system;
	BlockQueue<air_conditioner_refrigeration_system> bq_air_conditioner_refrigeration_system;
	BlockQueue<miscellaneous> bq_miscellaneous;
	BlockQueue<co2_system> bq_co2_system;
    BlockQueue<port_azimuth_center_control_cabinet> bq_port_azimuth_center_control_cabinet;
	BlockQueue<stbd_azimuth_center_control_cabinet> bq_stbd_azimuth_center_control_cabinet;
	BlockQueue<power_system2> bq_power_system2;
	BlockQueue<sewage_treatment_plant> bq_sewage_treatment_plant;
    BlockQueue<msb> bq_msb;

	//////////////ams modules end//////////////////////////////////////////

	/*ecr station 
	 fwd whc station
	 aft whc station
	 */
	BlockQueue<buzzer> bq_buzzer[3];
	BUZZER m_client_buzzer[3];	

	/*
	Chief
	chief office
	rest room
	meeting
	restautant*/
	BlockQueue<extend_alarm_panel> bq_extend_alarm_panel[extend_alarm_panel_nums];
	EXTEND_ALARM_PANEL m_client_extend_alarm_panel[extend_alarm_panel_nums];


	AMS_PC m_client_ams_pc[pc_nums];
	BlockQueue<ams_pc> bq_ams_pc[pc_nums];

	AMS_SAU m_client_ams_sau[sau_nums];
	BlockQueue<ams_sau> bq_ams_sau[sau_nums];
	
	//ams ai
	uint16_t m_v_plc1_ais[PLC1_AI_NUMS];
	uint16_t m_v_plc1_dis[PLC1_DI_NUMS];

	uint16_t m_v_plc2_ais[PLC2_AI_NUMS];
	uint16_t m_v_plc2_dis[PLC2_DI_NUMS];

	//jacking system
	//uint16_t m_v_platform_jacking_system[PLATFORM_JACKING_SYSTEM_NUMS];

	//cummins dcu
	//uint16_t m_v_cummins_dcu[CUMMINS_DCU_NUMS];

	uint16_t main_gen_ai[7][15];
	uint16_t main_gen_di[7][8];

	uint16_t m_main_prop_ais[13];
	uint16_t m_main_prop_dis[7];

	uint16_t m_v_power_ais[84];
	uint16_t m_v_power_dis[12];

	uint16_t m_v_level_switch[3];

	std::string m_cur_pc_ip;

#ifndef MAHTS648
	uint16_t m_v_ios[100];
	uint16_t m_v_ios_alarm[80];
#else
    uint16_t m_v_ios[120];
	uint16_t m_v_ios_alarm[100];
	uint16_t m_v_ios_w[120];

	uint16_t m_alarm_h_w[100];  //base 199, H-A or L-A
#endif

#ifdef JANA505	
	uint16_t m_v_plc[400];
#endif
#ifdef JANA508509
	uint16_t m_v_plc[515];
#endif

#ifdef MAHTS648
	uint16_t m_v_plc[800];
	uint16_t m_v_plc_w[800];
#endif

	shm_main_generator m_shm_main_generator[6];
	shm_eme_generator m_shm_eme_generator;

	shm_power_system_dg m_shm_power_system_dg[6];

	modbus *m_mb_plc1;
	modbus *m_mb_plc1_503;
	modbus *m_mb_plc1_504;
	modbus *m_mb_plc2;
	//std::mutex m_plc1_read_mux;
	std::mutex *m_plc1_rw_mux;
	std::mutex *m_plc2_rw_mux;

#ifdef MAHTS648
    // modbus *m_mb_gen_bd = nullptr;  //baudouin
	// std::mutex *m_gen_bd_mux = nullptr;
    // modbus * m_mb_gen_comm= nullptr;  //cummins
	// std::mutex *m_gen_comm_mux = nullptr;
    // modbus *m_mb_propulsion = nullptr;        //buzzer & external
	// std::mutex *m_propulsion_mux = nullptr;
	// modbus *m_mb_power = nullptr;
	// std::mutex *m_power_mux = nullptr;
	// modbus *m_mb_level = nullptr;
	// std::mutex *m_level_mux = nullptr;
	// modbus *m_mb_gas = nullptr;
	// std::mutex *m_gas_mux = nullptr;
	// modbus *m_mb_misc = nullptr;
	// std::mutex *m_misc_mux = nullptr;
	// modbus *m_mb_ecc = nullptr;
	// std::mutex *m_ecc_mux = nullptr;
	// modbus *m_mb_buzzer = nullptr;
	// std::mutex *m_buzzer_mux = nullptr;

    // modbus *m_mb_gen_bd_w = nullptr;  //baudouin
	// std::mutex *m_gen_bd_mux_w = nullptr;
    // modbus *m_mb_gen_comm_w = nullptr;  //cummins
	// std::mutex *m_gen_comm_mux_w = nullptr;
    // modbus *m_mb_propulsion_w = nullptr;        //buzzer & external
	// std::mutex *m_propulsion_mux_w = nullptr;
	// modbus *m_mb_power_w = nullptr;
	// std::mutex *m_power_mux_w = nullptr;
	// modbus *m_mb_level_w = nullptr;
	// std::mutex *m_level_mux_w = nullptr;
	// modbus *m_mb_gas_w = nullptr;
	// std::mutex *m_gas_mux_w = nullptr;
	// modbus *m_mb_misc_w = nullptr;
	// std::mutex *m_misc_mux_w = nullptr;
	// modbus *m_mb_ecc_w = nullptr;
	// std::mutex *m_ecc_mux_w = nullptr;
	// modbus *m_mb_buzzer_w = nullptr;
	// std::mutex *m_buzzer_mux_w = nullptr;

	// modbus *m_mb_buzzer = nullptr;
	// std::mutex *m_buzzer_mux = nullptr;

	modbus *m_mb_502 = nullptr;
	std::mutex *m_502_mux = nullptr;

	modbus *m_mb_503 = nullptr;
	std::mutex *m_503_mux = nullptr;

	//modbus *m_mb_504 = nullptr;
	//std::mutex *m_504_mux = nullptr;

	modbus *m_mb_506_sau1 = nullptr;
	std::mutex *m_506_sau1_mux = nullptr;

    //modbus *m_mb_506_sau2 = nullptr;
	//std::mutex *m_506_sau2_mux = nullptr;

	

#endif


	uint16_t m_pc_status;
	std::mutex m_mux_pc_status;
	uint16_t m_sau_status;
	std::mutex m_mux_sau_status;

    uint16_t m_extend_alarm_panel_status = 0;
     
	uint16_t m_pc_status_test;
	std::mutex m_mux_pc_status_test;
	uint16_t m_sau_status_test;
	std::mutex m_mux_sau_status_test;


    std::mutex m_mux_h_a;
	std::vector<alarm_type> m_h_a_vec;

	std::atomic<char> m_disconnect;//1:m_disconnect 0:connect
		
	uint16_t m_buzzer_status;
    uint16_t m_duty = 0;
	uint16_t m_pc_call = 0;

	float m_preU[6] = {-1000,-1000,-1000,-1000,-1000,-1000};
	float m_preV[6] = {-1000,-1000,-1000,-1000,-1000,-1000};
	float m_preW[6] = {-1000,-1000,-1000,-1000,-1000,-1000};


	void connect_plc();
	void check_pc_status(int i);	
	void check_pc_disconnect();
	void check_sau_status(int i);
	void check_sau_disconnect();

	
	void write_buzzer(int i);
	void call_extend_alarm_panel(int i);

	void write_buzzer2(int i);
    void write_bell();
    void write_duty();
    void call_extend_alarm_panel2(int i);
	//void write_extend_alarm_panel2(int i);

//test start
public:
	void genRandTest();
	void getDatasFromAMS_TEST();
	void gen_data_main_propulsion_test();
	void gen_data_power_system_test();
	void gen_data_main_diesel_generator_test();
	void gen_data_baudouin_main_generator_test();
	void gen_data_emergency_generator_test();
	void gen_data_air_conditioner_refrigeration_system_test();
	void gen_data_compressed_air_system_test();
	void gen_data_general_charger_and_discharger_panel_test();
	void gen_data_main_transformer_test(int i);
	void gen_data_bow_thruster_test();
	void gen_data_e_r_charger_and_discharger_panel_test();
	void gen_data_level_gauging_test();
	void gen_data_level_switch_test();
	void gen_data_gas_detection_system_test();
	void gen_data_miscellaneous_test();
	void gen_data_cooling_water_system_test();


	void gen_data_sau_test();
	void check_pc_status_test(int i);	
	void check_pc_disconnect_test(int i);

	

//test end

	void write_plc_datetime();

	void check_self_network();

};


#endif
