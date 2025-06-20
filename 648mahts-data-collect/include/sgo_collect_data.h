#ifndef SGO_COLLECT_DATA_H
#define SGO_COLLECT_DATA_H

#include <vector>
#include <string>
#include <mutex>
#include <boost/any.hpp>

//#define TEST
#define DEBUG
//#define DEMO
//#define JANA505
//#define JANA508509
#define MAHTS648

#define CUMMINS_DCU_NUMS 336
#define PLC1_AI_NUMS 56
#define PLC1_DI_NUMS 6
#define PLC1_DI_WRITE_NUMS 3
#define PLC2_AI_NUMS 16
#define PLC2_DI_NUMS 10
#define PLC2_DI_WRITE_NUMS 1

#define PLATFORM_JACKING_SYSTEM_NUMS 207

#define MAIN_PROPULSION_IP_MOXA "192.168.20.30"
#define POWER_SYSTEM_IP "192.168.20.13"
#define MAIN_PROPULSION_IP1 "192.168.20.14"
#define MAIN_PROPULSION_IP2 "192.168.20.15"
#define SAU_IP0 "192.168.20.221"
#define SAU_IP1 "192.168.20.222"
#define SAU_IP2 "192.168.20.223"
#define MOXA_IP1 "192.168.20.224"
#define MOXA_IP2 "192.168.20.225"
enum {
	MOXA_NPORT_1 = 10001,
	MOXA_NPORT_2,
	MOXA_NPORT_3,
	MOXA_NPORT_4,
	MOXA_NPORT_5,
	MOXA_NPORT_6,
	MOXA_NPORT_7,
	MOXA_NPORT_8,	
};
#define EXP_IP1 "192.168.20.211"
#define EXP_IP2 "192.168.20.212"
#define EXP_IP3 "192.168.20.213"
#define EXP_IP4 "192.168.20.214"
#define EXP_IP5 "192.168.20.215"
#define EXP_IP6 "192.168.20.216"
#define EXP_IP_PREFIX "192.168.20.21"
#define DG_IP_PREFIX "192.168.20.10"
#define PC_IP_PREFIX "192.168.20.20"
#define MQTT_IP_PREFIX "192.168.20.20"

const int mqtt_proxy_nums = 2;//4;
#if defined(JANA505)
const int main_gens_nums = 6;
#endif
#if defined(MAHTS648)
const int main_gens_nums = 4;
#endif
#ifdef JANA508509
const int main_gens_nums = 5;
#endif
const int baudouin_main_gens_nums=2;
const int main_propulsion_nums = 2;
const int main_transformer_nums = 2;
const int bow_thruster_nums = 2;
const int buzzer_nums = 3;
const int extend_alarm_panel_nums = 7;
const int pc_nums = 2;// 4;
const int sau_nums =  2;

const int threshold_low = -10000;
const int threshold_high = 10000;

typedef std::vector<std::vector<std::string>> StringList;

typedef struct{
	uint16_t baseAddr = 0;

	//value
    uint16_t vStartWord = 0xffff;
	uint16_t vStartBit = 0xffff;
	//H-A
    std::pair<uint16_t, uint16_t> ha_bit{0xffff, 0xffff};
    //L-A
    std::pair<uint16_t, uint16_t> la_bit{0xffff, 0xffff};

	//HH-A

	//LL-A

    //range
	std::pair<float, float> range{-10000.0, -10000.0};

	//INHIBIT
    bool inhibit = false;

    boost::any value;
}value_type;  //write extern alarm panle


typedef struct
{
	std::string deviceCode;
	std::string alarmType;
	std::string itemKey;
} alarm_type;

typedef struct{
	int indx; //com口号
	std::vector<std::string> data;
	std::string errmsg;
}data_test;

typedef struct{
	int id;
	int errCode;
	std::string data;
	std::string errmsg;
}alarm_test;

typedef struct{
	int id;
	std::vector<unsigned char> m_vdata;
}qz_data;

typedef struct{
	std::vector<uint16_t> m_vdata;
}km_data;

typedef struct {
	std::string m_name;
	union {
		int m_i;
		double m_d;
	}m_value;
	char m_value_type;//0:int 1:double
	int m_index;
}property_index_array;


/////////////////////////////////////////AMS start/////////////////////////////////////////////////////////
//
//主推进
typedef struct{
	int no;
	int main_propulsion_control_position;
	float main_propulsion_rudder_indication;
	float main_propulsion_vfd_rpm_indication;
	float main_propulsion_vfd_load_indication;
	float main_propulsion_motor_u_winding_temperature;
	float main_propulsion_motor_v_winding_temperature;
	float main_propulsion_motor_w_winding_temperature;
	float main_propulsion_motor_de_bearing_temperature;
	float main_propulsion_motor_nde_bearing_temperature;
	float main_propulsion_motor_air_inlet_temperature;
	float main_propulsion_motor_air_outlet_temperature;
	char main_propulsion_rudder_feedback_signal_missing;
	char main_propulsion_control_system_main_power_failure;
	char main_propulsion_control_system_backup_power_failure;
	char main_propulsion_steering_command_missing_joystick;
	char main_propulsion_speed_command_missing_joystick;
	char main_propulsion_fu_steering_control_failure;
	char main_propulsion_fu_speed_control_failure;
	char main_propulsion_vfd_speed_feedback_signal_missing;
	char main_propulsion_vfd_load_feedback_signal_missing;
	char main_propulsion_steering_command_missing_fwd_console;
	char main_propulsion_speed_command_missing_fwd_console;
	char main_propulsion_rudder_command_missing_aft_console;
	char main_propulsion_speed_command_missing_aft_console;
	char azimuth_thruster_common_alarm;
	char azimuth_thruster_fault;
	char main_propulsion_remote_control_system_communication_failure;
#if defined(MAHTS648)
	char propulsion_vfd_isu_communication_lost;  //506
	char propulsion_vfd_inu_communication_lost;  //506
	char propulsion_vfd_lcu_communication_lost;  //506
	char main_propulsion_vfd_stop_temp;   //506
	char main_propulsion_vfd_control_power_failure_ups;   //506
	char main_propulsion_vfd_isu_fault;  //506
    char main_propulsion_vfd_inu_fault;  //506
    char main_propulsion_vfd_lcu_fault;  //506
	char azimuth_thruster_steering_vfd_fault_2;  //506
	char main_propulsion_vfd_inu_waring;  //506
    char main_propulsion_vfd_lcu_waring;  //506
#endif
	char main_propulsion_vfd_fault;
	char main_propulsion_vfd_common_alarm;
	char main_propulsion_vfd_emergency_stop;
	char main_propulsion_vfd_emergency_stop_wire_broken;
	char main_propulsion_vfd_power_limit;
    char azimuth_thruster_steering_vfd_fault;
	char main_propulsion_vfd_slowdown_pre_alarm;
	char main_propulsion_motor_water_leakage;
	char main_propulsion_motor_de_leakage;
	char main_propulsion_motor_nde_leakage;
	char main_propulsion_motor_u_winding_temrpeature_high;
	char main_propulsion_motor_v_winding_temrpeature_high;
	char main_propulsion_motor_w_winding_temrpeature_high;
	char main_propulsion_motor_high_temrpeature_slowdown;
	char main_propulsion_motor_high_temrpeature_shutdown;
	char main_propulsion_motor_de_bearing_high_temperature_alarm;
	char main_propulsion_motor_de_bearing_high_temperature_slowdown;
	char main_propulsion_motor_de_bearing_high_temperature_shutdown;
	char main_propulsion_motor_nde_bearing_high_temperature_alarm;
	char main_propulsion_motor_nde_bearing_high_temperature_slowdown;
	char main_propulsion_motor_nde_bearing_high_temperature_shutdown;
#if defined(MAHTS648)
	char main_propulsion_vfd_cooling_water_inlet_pressure_low_alarm;  //506
	char main_propulsion_vfd_cooling_water_inlet_pressure_low_low_alarm;  //506
	char main_propulsion_vfd_cooling_water_inlet_pressure_low_low_fault;  //506
	char main_propulsion_vfd_lcu_outlet_pressure_high_alarm;  //506
	char main_propulsion_vfd_lcu_diff_pressure_low_alarm;  //506
	char main_propulsion_vfd_lcu_diff_pressure_low_low_fault;  //506
	char main_propulsion_shaft_lock_status;  //506
	char main_propulsion_vfd_heater_on;  //506
	char main_propulsion_vfd_power_on;  //506
 	char main_propulsion_motor_heater_on;  //506
    char main_propulsion_vfd_fu_nfu_mode;  //506
	char azimuth_thruster_fu_nfu_mode;  //506
	char azimuth_thruster_vfd_running;  //506
    char azimuth_thruster_steering_power_supply_fault;  //506
#endif
	char main_propulsion_vfd_control_mode;
	char main_propulsion_steering_control_nfu_mode;
    char main_propulsion_vfd_ready;
    char main_propulsion_vfd_remote_control_position;
	char main_propulsion_vfd_running;
	char main_propulsion_vfd_override;
    char main_propulsion_vfd_start_interlock;
	char main_propulsion_motor_air_inlet_high_temperature_alarm;
	char main_propulsion_motor_air_outlet_high_temperature_alarm;
	char main_propulsion_motor_high_temrpeature_slowdown_pre_alarm;
	char main_propulsion_motor_high_temrpeature_shutdown_pre_alarm;
	char main_propulsion_motor_de_bearing_high_temp_slowdown_pre_alarm;
	char main_propulsion_motor_de_bearing_high_tem_shutdown_pre_alarm;
	char main_propulsion_motor_nde_bearing_high_temp_slowdown_pre_alarm;
	char main_propulsion_motor_nde_bearing_high_tem_shutdown_pre_alarm;
	char main_propulsion_emergency_control_mode;
	char main_propulsion_shaft_lock_indication;
	char main_propulsion_vfd_torque_limitation;
	char main_propulsion_speed_control_nfu_mode;
	char azimuth_thruster_vfd1_running;
	char azimuth_thruster_vfd2_running;
	char azimuth_thruster_ready;
	char main_propulsion_joystick_control_permission;
	char main_propulsion_ap_control_permission;
	char azimuth_thruster_main_power_failure;
	char azimuth_thruster_backup_power_failure;
	char azimuth_thruster_shaft_lock_inlet_air_pressure_low_alarm;
	char azimuth_thruster_lub_oil_level_low_alarm;
	char azimuth_thruster_lub_oil_temp_high_alarm;
	char azimuth_thruster_lub_oil_filter_blocked_alarm;
	char azimuth_thruster_lub_oil_press_low_alarm;
	char azimuth_thruster_sealing_oil_tank_level_low_alarm;
	char azimuth_thruster_vfd1_fault;
	char azimuth_thruster_vfd2_fault;
	char azimuth_thruster_fu_steering_fault;
	char azimuth_thruster_lub_motor1_overload_alarm;
	char azimuth_thruster_lub_motor2_overload_alarm;
	char azimuth_thruster_angle_order_lost_alarm;
	char azimuth_thruster_angle_feedback_lost_alarm;
	char azimuth_thruster_steering_power1_supply_fault;
	char azimuth_thruster_steering_power2_supply_fault;
	char azimuth_thruster_sealing_oil_inlet_air_press_low_alarm;
	char azimuth_thruster_sealing_oil_inlet_air_press_high_alarm;
	char azimuth_thruster_steering_motor1_temp_high_alarm;
	char azimuth_thruster_steering_motor2_temp_high_alarm;
	float main_propulsion_vfd_speed_reference;
	float main_propulsion_vfd_actual_speed;
	float main_propulsion_vfd_actual_torque;
	float main_propulsion_motor_current;
	float main_propulsion_vfd_dc_voltage;
	float main_propulsion_motor_power;
#if defined(MAHTS648)
	float main_propulsion_vfd_voltage;  //506
	float main_propulsion_vfd_current;  //506
	float main_propulsion_lcu_inlet_pressure;  //506
	float main_propulsion_lcu_outlet_pressure;  //506
	float main_propulsion_vfd_cooling_water_differential_pressure;  //506
	float azimuth_thruster_angle_feedback;  //506
#endif
	char device_offline;
}main_propulsion;

typedef struct{
	int no;
	value_type main_propulsion_control_position;
	value_type main_propulsion_rudder_indication;
	value_type main_propulsion_vfd_rpm_indication;
	value_type main_propulsion_vfd_load_indication;
	value_type main_propulsion_motor_u_winding_temperature;
	value_type main_propulsion_motor_v_winding_temperature;
	value_type main_propulsion_motor_w_winding_temperature;
	value_type main_propulsion_motor_de_bearing_temperature;
	value_type main_propulsion_motor_nde_bearing_temperature;
	value_type main_propulsion_motor_air_inlet_temperature;
	value_type main_propulsion_motor_air_outlet_temperature;
	value_type main_propulsion_rudder_feedback_signal_missing;
	value_type main_propulsion_control_system_main_power_failure;
	value_type main_propulsion_control_system_backup_power_failure;
	value_type main_propulsion_steering_command_missing_joystick;
	value_type main_propulsion_speed_command_missing_joystick;
	value_type main_propulsion_fu_steering_control_failure;
	value_type main_propulsion_fu_speed_control_failure;
	value_type main_propulsion_vfd_speed_feedback_signal_missing;
	value_type main_propulsion_vfd_load_feedback_signal_missing;
	value_type main_propulsion_steering_command_missing_fwd_console;
	value_type main_propulsion_speed_command_missing_fwd_console;
	value_type main_propulsion_rudder_command_missing_aft_console;
	value_type main_propulsion_speed_command_missing_aft_console;
	value_type azimuth_thruster_common_alarm;
	value_type azimuth_thruster_fault;
	value_type main_propulsion_remote_control_system_communication_failure;
#if defined(MAHTS648)
	value_type propulsion_vfd_isu_communication_lost;  //506
	value_type propulsion_vfd_inu_communication_lost;  //506
	value_type propulsion_vfd_lcu_communication_lost;  //506
	value_type main_propulsion_vfd_stop_temp;   //506
	value_type main_propulsion_vfd_control_power_failure_ups;   //506
	value_type main_propulsion_vfd_isu_fault;  //506
    value_type main_propulsion_vfd_inu_fault;  //506
    value_type main_propulsion_vfd_lcu_fault;  //506
	value_type azimuth_thruster_steering_vfd_fault_2;  //506
	value_type main_propulsion_vfd_inu_waring;  //506
    value_type main_propulsion_vfd_lcu_waring;  //506
#endif
	value_type main_propulsion_vfd_fault;
	value_type main_propulsion_vfd_common_alarm;
	value_type main_propulsion_vfd_emergency_stop;
	value_type main_propulsion_vfd_emergency_stop_wire_broken;
	value_type main_propulsion_vfd_power_limit;
    value_type azimuth_thruster_steering_vfd_fault;
	value_type main_propulsion_vfd_slowdown_pre_alarm;
	value_type main_propulsion_motor_water_leakage;
	value_type main_propulsion_motor_de_leakage;
	value_type main_propulsion_motor_nde_leakage;
	value_type main_propulsion_motor_u_winding_temrpeature_high;
	value_type main_propulsion_motor_v_winding_temrpeature_high;
	value_type main_propulsion_motor_w_winding_temrpeature_high;
	value_type main_propulsion_motor_high_temrpeature_slowdown;
	value_type main_propulsion_motor_high_temrpeature_shutdown;
	value_type main_propulsion_motor_de_bearing_high_temperature_alarm;
	value_type main_propulsion_motor_de_bearing_high_temperature_slowdown;
	value_type main_propulsion_motor_de_bearing_high_temperature_shutdown;
	value_type main_propulsion_motor_nde_bearing_high_temperature_alarm;
	value_type main_propulsion_motor_nde_bearing_high_temperature_slowdown;
	value_type main_propulsion_motor_nde_bearing_high_temperature_shutdown;
#if defined(MAHTS648)
	value_type main_propulsion_vfd_cooling_water_inlet_pressure_low_alarm;  //506
	value_type main_propulsion_vfd_cooling_water_inlet_pressure_low_low_alarm;  //506
	value_type main_propulsion_vfd_cooling_water_inlet_pressure_low_low_fault;  //506
	value_type main_propulsion_vfd_lcu_outlet_pressure_high_alarm;  //506
	value_type main_propulsion_vfd_lcu_diff_pressure_low_alarm;  //506
	value_type main_propulsion_vfd_lcu_diff_pressure_low_low_fault;  //506
	value_type main_propulsion_shaft_lock_status;  //506
	value_type main_propulsion_vfd_heater_on;  //506
	value_type main_propulsion_vfd_power_on;  //506
 	value_type main_propulsion_motor_heater_on;  //506
    value_type main_propulsion_vfd_fu_nfu_mode;  //506
	value_type azimuth_thruster_fu_nfu_mode;  //506
	value_type azimuth_thruster_vfd_running;  //506
    value_type azimuth_thruster_steering_power_supply_fault;  //506
#endif
	value_type main_propulsion_vfd_control_mode;
	value_type main_propulsion_steering_control_nfu_mode;
    value_type main_propulsion_vfd_ready;
    value_type main_propulsion_vfd_remote_control_position;
	value_type main_propulsion_vfd_running;
	value_type main_propulsion_vfd_override;
    value_type main_propulsion_vfd_start_interlock;
	value_type main_propulsion_motor_air_inlet_high_temperature_alarm;
	value_type main_propulsion_motor_air_outlet_high_temperature_alarm;
	value_type main_propulsion_motor_high_temrpeature_slowdown_pre_alarm;
	value_type main_propulsion_motor_high_temrpeature_shutdown_pre_alarm;
	value_type main_propulsion_motor_de_bearing_high_temp_slowdown_pre_alarm;
	value_type main_propulsion_motor_de_bearing_high_tem_shutdown_pre_alarm;
	value_type main_propulsion_motor_nde_bearing_high_temp_slowdown_pre_alarm;
	value_type main_propulsion_motor_nde_bearing_high_tem_shutdown_pre_alarm;
	value_type main_propulsion_emergency_control_mode;
	value_type main_propulsion_shaft_lock_indication;
	value_type main_propulsion_vfd_torque_limitation;
	value_type main_propulsion_speed_control_nfu_mode;
	value_type azimuth_thruster_vfd1_running;
	value_type azimuth_thruster_vfd2_running;
	value_type azimuth_thruster_ready;
	value_type main_propulsion_joystick_control_permission;
	value_type main_propulsion_ap_control_permission;
	value_type azimuth_thruster_main_power_failure;
	value_type azimuth_thruster_backup_power_failure;
	value_type azimuth_thruster_shaft_lock_inlet_air_pressure_low_alarm;
	value_type azimuth_thruster_lub_oil_level_low_alarm;
	value_type azimuth_thruster_lub_oil_temp_high_alarm;
	value_type azimuth_thruster_lub_oil_filter_blocked_alarm;
	value_type azimuth_thruster_lub_oil_press_low_alarm;
	value_type azimuth_thruster_sealing_oil_tank_level_low_alarm;
	value_type azimuth_thruster_vfd1_fault;
	value_type azimuth_thruster_vfd2_fault;
	value_type azimuth_thruster_fu_steering_fault;
	value_type azimuth_thruster_lub_motor1_overload_alarm;
	value_type azimuth_thruster_lub_motor2_overload_alarm;
	value_type azimuth_thruster_angle_order_lost_alarm;
	value_type azimuth_thruster_angle_feedback_lost_alarm;
	value_type azimuth_thruster_steering_power1_supply_fault;
	value_type azimuth_thruster_steering_power2_supply_fault;
	value_type azimuth_thruster_sealing_oil_inlet_air_press_low_alarm;
	value_type azimuth_thruster_sealing_oil_inlet_air_press_high_alarm;
	value_type azimuth_thruster_steering_motor1_temp_high_alarm;
	value_type azimuth_thruster_steering_motor2_temp_high_alarm;
	value_type main_propulsion_vfd_speed_reference;
	value_type main_propulsion_vfd_actual_speed;
	value_type main_propulsion_vfd_actual_torque;
	value_type main_propulsion_motor_current;
	value_type main_propulsion_vfd_dc_voltage;
	value_type main_propulsion_motor_power;
#if defined(MAHTS648)
	value_type main_propulsion_vfd_voltage;  //506
	value_type main_propulsion_vfd_current;  //506
	value_type main_propulsion_lcu_inlet_pressure;  //506
	value_type main_propulsion_lcu_outlet_pressure;  //506
	value_type main_propulsion_vfd_cooling_water_differential_pressure;  //506
	value_type azimuth_thruster_angle_feedback;  //506
#endif
	value_type device_offline;
}main_propulsion_store;

//配电系统
typedef struct{
	char dg_start_fail;
	char eg_running;
	char eg_cb_close;
	char esb_dc24v_power_failure;
	char eg_breaker_trip;
	char eg_over_current;
	char esb_common_alarm;
	char esb_ac440v_bus_low_insulation;
	char esb_ac230v_bus_low_insulation;
	char esb_auto_mode;
	char no1_main_transformer_breaker_trip;
	char no2_main_transformer_breaker_trip;
	char no1_main_crane_cb_close;
	char no2_main_crane_cb_close;
	char no1_jacking_system_phase_transformer_cb_close;
	char no2_jacking_system_phase_transformer_cb_close;
	char no3_jacking_system_phase_transformer_cb_close;
	char no4_jacking_system_phase_transformer_cb_close;
	char no1_bow_thruster_cb_close;
	char no2_bow_thruster_cb_close;
	char no1_main_propulsion_vfd_cb_close;
	char no2_main_propulsion_vfd_cb_close;
	char dg1_ppm_fault;
	char dg2_ppm_fault;
	char dg3_ppm_fault;
	char dg4_ppm_fault;
	char dg5_ppm_fault;
	char dg6_ppm_fault;
	char msb_ac690v_bus_a_low_insulation;
	char msb_ac690v_bus_b_low_insulation;
	char no1_main_transformer_cb_close;
	char no2_main_transformer_cb_close;
	char ac690v_msb_dc24v_power_failure;
	char msb_ac690v_bus_b_harmonic_over_limit;
	char msb_ac690v_bus_a_harmonic_over_limit;
	char msb_ac690v_shore_power_cb_closed;
	char no1_main_transformer_secondary_breaker_input_power_on;
	char no1_main_transformer_breaker_auto_mode;
	char no1_main_transformer_secondary_breaker_close;
	char no1_main_transformer_secondary_breaker_trip;
	char msb_ac440v_bus_b_blackout;
	char no2_main_transformer_secondary_breaker_input_power_on;
	char no2_main_transformer_breaker_auto_mode;
	char no2_main_transformer_secondary_breaker_close;
	char no2_main_transformer_secondary_breaker_trip;
	char ac440v_msbbustie_acb_close;
	char msb_ac440v_bus_a_blackout;
	char ac440v_msb_dc24v_power_failure;
	char pt_power_failure;
	char ac440v_msb_bus_c_low_insulation;
	char ac440v_msb_bus_d_low_insulation;
	char ac230v_msb_bus_low_insulation;
	char preferential_trip_alarm;
	char e_stop_shutdown_power_failure;
	char ac690v_msb_bus_tie_cb_closed;
	char ac440v_msb_to_esb_cb_colse_statues;
	char ac440v_shore_power_cb_closed;
	char emergency_shutdown_wire_break_es1;
	char emergency_shutdown_wire_break_es21;
	char emergency_shutdown_wire_break_es22;
	char emergency_shutdown_wire_break_es23;
	char emergency_shutdown_wire_break_es24;
	char emergency_shutdown_wire_break_es25;
	char emergency_shutdown_wire_break_es26;
	char emergency_shutdown_es1_actived;
	char emergency_shutdown_es21_actived;
	char emergency_shutdown_es22_actived;
	char emergency_shutdown_es23_actived;
	char emergency_shutdown_es24_actived;
	char emergency_shutdown_es25_actived;
	char emergency_shutdown_es26_actived;
    #ifdef MAHTS648
    char dg1_start_fail;
    char dg2_start_fail;
    char dg3_start_fail;
    char dg4_start_fail;
    //char dg1_acb_abnormal_trip;
    //char dg2_acb_abnormal_trip;
    //char dg3_acb_abnormal_trip;
    //char dg4_acb_abnormal_trip;
    //char dg1_acb_close_fail;
    //char dg2_acb_close_fail;
    //char dg3_acb_close_fail;
    //char dg4_acb_close_fail;
    char ac690v_bus_a_insul_low;
    char ac690v_bus_b_insul_low;
    char ac690v_bus_a_voltage_high;
    char ac690v_bus_b_voltage_high;
    char ac690v_bus_a_voltage_low;
    char ac690v_bus_b_voltage_low;
    char ac690v_bus_a_frequency_high;
    char ac690v_bus_b_frequency_high;
    char ac690v_bus_a_frequency_low;
    char ac690v_bus_b_frequency_low;
    //char preferential_trip_alarm;
    char ac690v_msb_common_alarm;
    char ac440v_bus_a_insul_low;
    char ac440v_bus_b_insul_low;
    char ac220v_bus_a_insul_low;
    char ac220v_bus_b_insul_low;
    char ac440v_msb_em_cy_stop_and_pre_trip_source_fail;
    char ac440v_msb_dc24v_source_fault;
    char ac690v_msb_dc24v_source_a_fault;
    char ac690v_msb_dc24v_source_b_fault;
    char dg1_acb_closed;
    char dg2_acb_closed;
    char dg3_acb_closed;
    char dg4_acb_closed;
    char ddbt_acb_closed;
    char pst_acb_closed;
    char sst_acb_closed;
    char bt1_acb_closed;
    char bt2_acb_closed;
    char dt1primary_acb_closed;
    char dt2primary_acb_closed;
    char dt1secondary_acb_closed;
    char dt2secondary_acb_closed;
    char ac440v_bus_tie_breaker_close;
    char shore_cb_closed;
    char dg1_running;
    char dg2_running;
    char dg3_running;
    char dg4_running;
    char dg1_common_alarm;
    char dg2_common_alarm;
    char dg3_common_alarm;
    char dg4_common_alarm;
    char stand_by_gen_changeover_alarm;
    char dg1_voltage_high;
    char dg1_voltage_low;
    char dg1_frequency_high;
    char dg1_frequency_low;
    char dg2_voltage_high;
    char dg2_voltage_low;
    char dg2_frequency_high;
    char dg2_frequency_low;
    char dg3_voltage_high;
    char dg3_voltage_low;
    char dg3_frequency_high;
    char dg3_frequency_low;
    char dg4_voltage_high;
    char dg4_voltage_low;
    char dg4_frequency_high;
    char dg4_frequency_low;
    char load_sharing_fail;
    char hg_start_failure;
    char hg_acb_abnormal_trip;
    char hg_rve_power;
    char hg_over_current;
    char hg_acb_close_fail;
    char synchro_failure;
    char ac440v_bus_a_insulation_low440v;
    char ac440v_bus_b_insulation_low440v;
    char ac220v_bus_a_insulation_low440v;
    char ac220v_bus_b_insulation_low440v;
    char pre_trippt;
    char dc24v_source_fault;
    char pt_ec_circuit_source_failure;
    char ac440v_bus_tie_cb_non_close;
    char uneven_load_sharing;
    char msb_es_line_break;
    char msb_common_alarm;
    char busbar_a_source_alive;
    char busbar_b_source_alive;
    char hg_acb_close;
    char dt1secondary_acb_closed440v;
    char dt2secondary_acb_closed440v;
	char ac440v_bus_tie_breaker_close_2;
    char ac220v_bus_tie_breaker_close;
    char shore_cb_closed440v;
    char lt1primary_acb_close;
    char lt1secondary_acb_close;
    char lt2primary_acb_close;
    char lt2secondary_acb_close;
    char hg_running;
    //char eg_running;
    char esb_ac415v_bus_low_insulation;
    char esb_ac220v_bus_low_insulation;
    //char esb_dc24v_power_failure;
    char em_cy_generator_overload;
    char em_cy_generator_running;
    //char esb_common_alarm;
    char e_g_cb_abnormal_trip;
    char ac690v_pms1_fault;
    char ac690v_pms2_fault;
    char ac440v_pms_fault;
    char ac440v_dc24_power_fault;
    #endif

	char dg1_acb_close_fail;
	char dg1_acb_open_fail;
	char dg1_ready_to_start;
	char dg1_cb_open;
	char dg1_cb_close;
	char dg1_auto_mode;
	char dg1_semi_auto_mode;
	char dg1_on_first_priority;
	char dg2_acb_close_fail;
	char dg2_acb_open_fail;
	char dg2_ready_to_start;
	char dg2_cb_open;
	char dg2_cb_close;
	char dg2_auto_mode;
	char dg2_semi_auto_mode;
	char dg2_on_first_priority;
	char dg3_acb_close_fail;
	char dg3_acb_open_fail;
	char dg3_ready_to_start;
	char dg3_cb_open;
	char dg3_cb_close;
	char dg3_auto_mode;
	char dg3_semi_auto_mode;
	char dg3_on_first_priority;
	char dg4_acb_close_fail;
	char dg4_acb_open_fail;
	char dg4_ready_to_start;
	char dg4_cb_open;
	char dg4_cb_close;
	char dg4_auto_mode;
	char dg4_semi_auto_mode;
	char dg4_on_first_priority;
	char dg5_acb_close_fail;
	char dg5_acb_open_fail;
	char dg5_ready_to_start;
	char dg5_cb_open;
	char dg5_cb_close;
	char dg5_auto_mode;
	char dg5_semi_auto_mode;
	char dg5_on_first_priority;
	char dg6_acb_close_fail;
	char dg6_acb_open_fail;
	char dg6_ready_to_start;
	char dg6_cb_open;
	char dg6_cb_close;
	char dg6_auto_mode;
	char dg6_semi_auto_mode;
	char dg6_on_first_priority;
#ifdef JANA508509	
	char dg7_acb_close_fail;
	char dg7_acb_open_fail;
	char dg7_ready_to_start;
	char dg7_cb_open;
	char dg7_cb_close;
	char dg7_auto_mode;
	char dg7_semi_auto_mode;
	char dg7_on_first_priority;
#endif		
	char dg1_supply_voltage_high;
	char dg1_supply_voltage_low;
	char dg1_supply_frequency_high;
	char dg1_supply_frequency_low;
	char dg1_acb_abnormal_trip;
	char dg1_reverse_power;
	char dg1_ppm_alarms_on;
	char dg1_auto_synchro_failure;
	char ac690v_msb_bus_bar_voltage_high;
	char ac690v_msb_bus_bar_voltage_low;
	char ac690v_msb_bus_bar_frequency_high;
	char ac690v_msb_bus_bar_frequency_low;
	float dg1_voltage_l1_l2;
	float dg1_voltage_l2_l3;
	float dg1_voltage_l3_l1;
	float dg1_frequency_l1;
	float dg1_frequency_l2;
	float dg1_frequency_l3;
	float dg1_current_l1;
	float dg1_current_l2;
	float dg1_current_l3;
	float dg1_active_power;
	float dg1_reactive_power;
	float dg1_apparent_power_s;
	float dg1_power_factor;
	float dg1_available_power;
	float ac690v_msb_busbar_voltage_l1_l2;
	float ac690v_msb_busbar_voltage_l2_l3;
	float ac690v_msb_busbar_voltage_l3_l1;
	float ac690v_msb_busbar_frequency_l1;
	float ac690v_msb_busbar_frequency_l2;
	float ac690v_msb_busbar_frequency_l3;
	char dg2_supply_voltage_high;
	char dg2_supply_voltage_low;
	char dg2_supply_frequency_high;
	char dg2_supply_frequency_low;
	char dg2_acb_abnormal_trip;
	char dg2_reverse_power;
	char dg2_ppm_alarms_on;
	char dg2_auto_synchro_failure;
	float dg2_voltage_l1_l2;
	float dg2_voltage_l2_l3;
	float dg2_voltage_l3_l1;
	float dg2_frequency_l1;
	float dg2_frequency_l2;
	float dg2_frequency_l3;
	float dg2_current_l1;
	float dg2_current_l2;
	float dg2_current_l3;
	float dg2_active_power;
	float dg2_reactive_power;
	float dg2_apparent_power_s;
	float dg2_power_factor;
	float dg2_available_power;
	char dg3_supply_voltage_high;
	char dg3_supply_voltage_low;
	char dg3_supply_frequency_high;
	char dg3_supply_frequency_low;
	char dg3_acb_abnormal_trip;
	char dg3_reverse_power;
	char dg3_ppm_alarms_on;
	char dg3_auto_synchro_failure;
	float dg3_voltage_l1_l2;
	float dg3_voltage_l2_l3;
	float dg3_voltage_l3_l1;
	float dg3_frequency_l1;
	float dg3_frequency_l2;
	float dg3_frequency_l3;
	float dg3_current_l1;
	float dg3_current_l2;
	float dg3_current_l3;
	float dg3_active_power;
	float dg3_reactive_power;
	float dg3_apparent_power_s;
	float dg3_power_factor;
	float dg3_available_power;
	char dg4_supply_voltage_high;
	char dg4_supply_voltage_low;
	char dg4_supply_frequency_high;
	char dg4_supply_frequency_low;
	char dg4_acb_abnormal_trip;
	char dg4_reverse_power;
	char dg4_ppm_alarms_on;
	char dg4_auto_synchro_failure;
	float dg4_voltage_l1_l2;
	float dg4_voltage_l2_l3;
	float dg4_voltage_l3_l1;
	float dg4_frequency_l1;
	float dg4_frequency_l2;
	float dg4_frequency_l3;
	float dg4_current_l1;
	float dg4_current_l2;
	float dg4_current_l3;
	float dg4_active_power;
	float dg4_reactive_power;
	float dg4_apparent_power_s;
	float dg4_power_factor;
	float dg4_available_power;
	char dg5_supply_voltage_high;
	char dg5_supply_voltage_low;
	char dg5_supply_frequency_high;
	char dg5_supply_frequency_low;
	char dg5_acb_abnormal_trip;
	char dg5_reverse_power;
	char dg5_ppm_alarms_on;
	char dg5_auto_synchro_failure;
	float dg5_voltage_l1_l2;
	float dg5_voltage_l2_l3;
	float dg5_voltage_l3_l1;
	float dg5_frequency_l1;
	float dg5_frequency_l2;
	float dg5_frequency_l3;
	float dg5_current_l1;
	float dg5_current_l2;
	float dg5_current_l3;
	float dg5_active_power;
	float dg5_reactive_power;
	float dg5_apparent_power_s;
	float dg5_power_factor;
	float dg5_available_power;
	char dg6_supply_voltage_high;
	char dg6_supply_voltage_low;
	char dg6_supply_frequency_high;
	char dg6_supply_frequency_low;
	char dg6_acb_abnormal_trip;
	char dg6_reverse_power;
	char dg6_ppm_alarms_on;
	char dg6_auto_synchro_failure;
	float dg6_voltage_l1_l2;
	float dg6_voltage_l2_l3;
	float dg6_voltage_l3_l1;
	float dg6_frequency_l1;
	float dg6_frequency_l2;
	float dg6_frequency_l3;
	float dg6_current_l1;
	float dg6_current_l2;
	float dg6_current_l3;
	float dg6_active_power;
	float dg6_reactive_power;
	float dg6_apparent_power_s;
	float dg6_power_factor;
	float dg6_available_power;
#ifdef JANA508509
	char dg7_supply_voltage_high;
	char dg7_supply_voltage_low;
	char dg7_supply_frequency_high;
	char dg7_supply_frequency_low;
	char dg7_acb_abnormal_trip;
	char dg7_reverse_power;
	char dg7_ppm_alarms_on;
	char dg7_auto_synchro_failure;
	float dg7_voltage_l1_l2;
	float dg7_voltage_l2_l3;
	float dg7_voltage_l3_l1;
	float dg7_frequency_l1;
	float dg7_frequency_l2;
	float dg7_frequency_l3;
	float dg7_current_l1;
	float dg7_current_l2;
	float dg7_current_l3;
	float dg7_active_power;
	float dg7_reactive_power;
	float dg7_apparent_power_s;
	float dg7_power_factor;
	float dg7_available_power;
#endif
#ifdef MAHTS648
    float dg1_power_kw;
    float dg2_power_kw;
    float dg3_power_kw;
    float dg4_power_kw;
    float dg1_voltagev;
    float dg2_voltagev;
    float dg3_voltagev;
    float dg4_voltagev;
    float ac690v_bus_a_frequencyhz;
    float ac690v_bus_b_frequencyhz;
    float ac690v_bus_a_total_powerkw;
    float ac690v_bus_b_total_powerkw;
    float ac690v_bus_total_powerkw;
    float ac690v_bus_a_surplus_powerkw;
    float ac690v_bus_b_surplus_powerkw;
    float ac690v_bus_surplus_powerkw;
    //float dg1_power_factor;
    //float dg2_power_factor;
    //float dg3_power_factor;
    //float dg4_power_factor;
    float hg_current_l1a;
    float hg_current_l2a;
    float hg_current_l3a;
    float hg_voltage_l1_l3v;
    float hg_voltage_l1_l2v;
    float hg_voltage_l2_l3v;
    float hg_frequencyhz;
    float hg_reactive_powerkvar;
    float hg_power_factorcos_phi;
    float hg_powerkw;

#endif
	char no1_jetting_pump_request_failure;
	char no2_jetting_pump_request_failure;
	char no1_bow_thruster_request_failure;
	char no2_bow_thruster_request_failure;
	char no1_main_crane_request_failure;
	char no2_main_crane_request_failure;
	char no1_leg_control_unit_request_failure;
	char no2_leg_control_unit_request_failure;
	char no3_leg_control_unit_request_failure;
	char no4_leg_control_unit_request_failure;
	char no1_submersible_pump_request_failure;
	char no2_submersible_pump_request_failure;
	char device_offline;
}power_system;

typedef struct{
    char hg_start_failure;
    char hg_acb_abnormal_trip;
    char hg_rve_power;
    char hg_over_current;
    char hg_acb_close_fail;
    char synchro_failure;
    char ac440v_bus_a_insulation_low440v;
    char ac440v_bus_b_insulation_low440v;
    char ac220v_bus_a_insulation_low440v;
    char ac220v_bus_b_insulation_low440v;
    char pre_trippt;
    char dc24v_source_fault;
    char pt_ec_circuit_source_failure;
    char ac440v_bus_tie_cb_non_close;
    char uneven_load_sharing;
    char msb_es_line_break;
    char msb_common_alarm;
    char busbar_a_source_alive;
    char busbar_b_source_alive;
    char hg_acb_close;
    char dt1secondary_acb_closed440v;
    char dt2secondary_acb_closed440v;
    char ac440v_bus_tie_breaker_close;
    char ac220v_bus_tie_breaker_close;
    char shore_cb_closed440v;
    char lt1primary_acb_close;
    char lt1secondary_acb_close;
    char lt2primary_acb_close;
    char lt2secondary_acb_close;
    char hg_running;
    char eg_running;
    double hg_current_l1a;
    double hg_current_l2a;
    double hg_current_l3a;
    double hg_voltage_l1_l3v;
    double hg_voltage_l1_l2v;
    double hg_voltage_l2_l3v;
    double hg_frequencyhz;
    double hg_reactive_powerkvar;
    double hg_power_factorcos_phi;
    double hg_powerkw;
    char esb_ac415v_bus_low_insulation;
    char esb_ac220v_bus_low_insulation;
    char esb_dc24v_power_failure;
    char em_cy_generator_overload;
    char esb_common_alarm;
    char e_g_cb_abnormal_trip;
    char ac440v_pms_fault;
    char ac440v_dc24_power_fault;
    char t3_cb_status;
    char t4_cb_status;
    char sc_cb_status;
    char spare;
    char device_offline;

}msb;

typedef struct{
	std::mutex mux;
	char dg_acb_close_fail;
	char dg_acb_open_fail;
	char dg_ready_to_start;
	char dg_cb_open;
	char dg_cb_close;
	char dg_auto_mode;
	char dg_semi_auto_mode;
	char dg_on_first_priority;	
	char dg_supply_voltage_high;
	char dg_supply_voltage_low;
	char dg_supply_frequency_high;
	char dg_supply_frequency_low;
	char dg_acb_abnormal_trip;
	char dg_reverse_power;
	char dg_ppm_alarms_on;
	char ac690v_msb_bus_bar_voltage_high;
	char ac690v_msb_bus_bar_voltage_low;
	char ac690v_msb_bus_bar_frequency_high;
	char ac690v_msb_bus_bar_frequency_low;
	char dg_auto_synchro_failure;
	float dg_voltage_l1_l2;
	float dg_voltage_l2_l3;
	float dg_voltage_l3_l1;
	float dg_frequency_l1;
	float dg_frequency_l2;
	float dg_frequency_l3;
	float dg_current_l1;
	float dg_current_l2;
	float dg_current_l3;
	float dg_active_power;
	float dg_reactive_power;
	float dg_apparent_power_s;
	float dg_power_factor;
	float dg_available_power;
	float ac690v_msb_busbar_voltage_l1_l2;
	float ac690v_msb_busbar_voltage_l2_l3;
	float ac690v_msb_busbar_voltage_l3_l1;
	float ac690v_msb_busbar_frequency_l1;
	float ac690v_msb_busbar_frequency_l2;
	float ac690v_msb_busbar_frequency_l3;
	char no1_jetting_pump_request_failure;
	char no2_jetting_pump_request_failure;
	char no1_bow_thruster_request_failure;
	char no2_bow_thruster_request_failure;
	char no1_main_crane_request_failure;
	char no2_main_crane_request_failure;
	char no1_leg_control_unit_request_failure;
	char no2_leg_control_unit_request_failure;
	char no3_leg_control_unit_request_failure;
	char no4_leg_control_unit_request_failure;
	char no1_submersible_pump_request_failure;
	char no2_submersible_pump_request_failure;
}shm_power_system_dg;


//1~6主发电机组
typedef struct{
	char gen_id;
	float d_g_dcu_unit_battery_volt_dcu_control_power;
	float d_g_lube_oil_pressure;
	float d_g_coolant_pressure;
	float d_g_start_air_pressure;
	float d_g_fuel_oil_pressure;
	float d_g_coolant_temperature;
	float d_g_lube_oil_temperature;
	float d_g_left_exhaust_temperature;
	float d_g_right_exhaust_temperature;
	float d_g_engine_speed;
	float d_g_running_hours;
	float d_g_rpu_unit_battery_a_volt_rpu_main_control_power;
	float d_g_rpu_unit_battery_b_volt_rpu_backup_control_power;
	char d_g_coolant_level_low;
	char d_g_main_power_failure;
	char d_g_backup_power_failure;
	char d_g_sea_water_pump_overload_trip;
	char d_g_ecr_remote_emergency_stop;
	char d_g_aft_bridge_remote_emergency_stop;
	char d_g_fwd_bridge_remote_emergency_stop;
	char d_g_lube_oil_pressure_too_low_shutdown;
	char d_g_coolant_temperature_too_high_shutdown;
	char d_g_local_emergency_stop;
	char d_g_remote_emergency_stop;
	char d_g_rpu_emergency_stop;
	char d_g_lube_oil_pressure_low_alarm;
	char d_g_coolant_pressure_low_alarm;
	char d_g_start_air_pressure_low_alarm;
	char d_g_fuel_oil_pressure_low_alarm;
	char d_g_coolant_temperature_high_alarm;
	char d_g_lube_oil_temperature_high_alarm;
	char d_g_left_exhaust_temperature_high_alarm;
	char d_g_right_exhaust_temperature_high_alarm;
	char d_g_overspeed_alarm;
	char d_g_overspeed_shutdown;
	char d_g_speed_sensor_fail;
	char d_g_dcu_unit_battery_volt_dct_control_power_low;
	char d_g_start_fail;
	char d_g_rpu_unit_battery_a_volt_rpu_main_control_power_low;
	char d_g_rpu_unit_battery_b_volt_rpu_main_control_power_low;
	char d_g_common_alarm;
	char d_g_common_warning;
	char d_g_common_shutdown;
	char d_g_common_cooldown;
	char d_g_common_faultsensor_fail;
	char d_g_remote_control_status_auxaccess_lock;
	char d_g_ready_to_start;
	char d_g_ready_to_load;
	char d_g_main_generator_running;
	float d_g_main_generator_u_winding_temperature;
	float d_g_main_generator_v_winding_temperature;
	float d_g_main_generator_w_winding_temperature;
	float d_g_main_generator_de_bearing_temperature;
	float d_g_main_generator_nde_bearing_temperature;
	float d_g_main_generator_excitation_current;
	float d_g_main_generator_excitation_voltage;
#ifndef MAHTS648
#else
    char d_g_charger_failure;
    char d_g_alternator_cooling_water_leakage;
    char d_g_speed_pickup_sensor_fail;
    char d_g_common_cooldowm;
    char d_g_running;
    float d_g_u_winding_temperature;
    float d_g_v_winding_temperature;
    float d_g_w_winding_temperature;
    float d_g_alternator_cooling_water_inlet_temp;
    float d_g_alternator_cooling_water_outlet_temp;
    float d_g_de_bearing_temperature;
    float d_g_nde_bearing_temperature;
    float d_g_colling_air_cold_temperature;
    float d_g_colling_air_warm_temperature;
    float d_g_fresh_water_cooling_pump_outlet_pressure;
    float d_g_sea_water_cooling_pump_outlet_pressure;
    float d_g_inlet_oil_flowmeter;
    float d_g_outlet_oil_flowmeter;


#endif
	char device_offline;
}main_diesel_generator;


//1~6主发电机组
typedef struct{
	char gen_id;
	char prefix_num = 0;
	value_type d_g_dcu_unit_battery_volt_dcu_control_power;
	value_type d_g_lube_oil_pressure;
	value_type d_g_coolant_pressure;
	value_type d_g_start_air_pressure;
	value_type d_g_fuel_oil_pressure;
	value_type d_g_coolant_temperature;
	value_type d_g_lube_oil_temperature;
	value_type d_g_left_exhaust_temperature;
	value_type d_g_right_exhaust_temperature;
	value_type d_g_engine_speed;
	value_type d_g_running_hours;
	value_type d_g_rpu_unit_battery_a_volt_rpu_main_control_power;
	value_type d_g_rpu_unit_battery_b_volt_rpu_backup_control_power;
	value_type d_g_coolant_level_low;
	value_type d_g_main_power_failure;
	value_type d_g_backup_power_failure;
	value_type d_g_sea_water_pump_overload_trip;
	value_type d_g_ecr_remote_emergency_stop;
	value_type d_g_aft_bridge_remote_emergency_stop;
	value_type d_g_fwd_bridge_remote_emergency_stop;
	value_type d_g_lube_oil_pressure_too_low_shutdown;
	value_type d_g_coolant_temperature_too_high_shutdown;
	value_type d_g_local_emergency_stop;
	value_type d_g_remote_emergency_stop;
	value_type d_g_rpu_emergency_stop;
	value_type d_g_lube_oil_pressure_low_alarm;
	value_type d_g_coolant_pressure_low_alarm;
	value_type d_g_start_air_pressure_low_alarm;
	value_type d_g_fuel_oil_pressure_low_alarm;
	value_type d_g_coolant_temperature_high_alarm;
	value_type d_g_lube_oil_temperature_high_alarm;
	value_type d_g_left_exhaust_temperature_high_alarm;
	value_type d_g_right_exhaust_temperature_high_alarm;
	value_type d_g_overspeed_alarm;
	value_type d_g_overspeed_shutdown;
	value_type d_g_speed_sensor_fail;
	value_type d_g_dcu_unit_battery_volt_dct_control_power_low;
	value_type d_g_start_fail;
	value_type d_g_rpu_unit_battery_a_volt_rpu_main_control_power_low;
	value_type d_g_rpu_unit_battery_b_volt_rpu_main_control_power_low;
	value_type d_g_common_alarm;
	value_type d_g_common_warning;
	value_type d_g_common_shutdown;
	value_type d_g_common_cooldown;
	value_type d_g_common_faultsensor_fail;
	value_type d_g_remote_control_status_auxaccess_lock;
	value_type d_g_ready_to_start;
	value_type d_g_ready_to_load;
	value_type d_g_main_generator_running;
	value_type d_g_main_generator_u_winding_temperature;
	value_type d_g_main_generator_v_winding_temperature;
	value_type d_g_main_generator_w_winding_temperature;
	value_type d_g_main_generator_de_bearing_temperature;
	value_type d_g_main_generator_nde_bearing_temperature;
	value_type d_g_main_generator_excitation_current;
	value_type d_g_main_generator_excitation_voltage;
    value_type d_g_charger_failure;
	value_type d_g_alternator_cooling_water_leakage;
	value_type d_g_common_cooldowm;
	value_type d_g_running;
	value_type d_g_alternator_cooling_water_inlet_temp;
    value_type d_g_alternator_cooling_water_outlet_temp;
    value_type d_g_de_bearing_temperature;
    value_type d_g_nde_bearing_temperature;
    value_type d_g_colling_air_cold_temperature;
    value_type d_g_colling_air_warm_temperature;
	value_type d_g_fresh_water_cooling_pump_outlet_pressure;
    value_type d_g_sea_water_cooling_pump_outlet_pressure;
	value_type device_offline;
}main_diesel_generator_store;


typedef struct{
	int gen_id;
	float d_g_engine_rpm;
	float d_g_battery_voltage;
	float d_g_boost_t_ps;  //B
	float d_g_boost_t_sb;  //B
	float d_g_speed_r;     //B
	float d_g_speed_ma;    //B
	float d_g_oil_piston_jet;  //B
	float d_g_coolant_press;  
	float d_g_exhaust_t_sb;   //B
	float d_g_exhaust_t_ps;
	float d_g_raw_wat_press;
	float d_g_load;
	float d_g_cool_temp_sb;
	float d_g_fuel_temp;
	float d_g_oil_temp;
	float d_g_intake_p_sb;
	float d_g_intake_t_sb;
	float d_g_oil_press;
	float d_g_fuel_press;
	float d_g_fuel_rate;
	float d_g_ecu_battvol_m;
	float d_g_ecu_temp_m;
	float d_g_lever_pos;
	float d_g_rail_press_sb;
	float d_g_tot_fuel_used;
	float d_g_tot_fuel_used_l;
	float d_g_rpu_rpm;
	float d_g_rpu_ubat_a;
	float d_g_rpu_ubat_b;
	float d_g_pri_battery;
	float d_g_sec_battery;
	float d_g_cool_temp_ps;
	float d_g_intake_p_ps;
	float d_g_intake_t_ps;
	float d_g_ecu_battvol_s;
	float d_g_ecu_temp_s;
	float d_g_rail_press_ps;
	float d_g_engine_hours;
	float d_g_run_hours;
	float d_g_service_time;
	float d_g_engine_state;
	float d_g_mode_id;

#ifdef MAHTS648
	float d_g_de_bearing_temp;
	float d_g_w_winding_temp;
	float d_g_v_winding_temp;
	float d_g_u_winding_temp;
	float d_g_nde_bearing_temp;

#endif

	char d_g_water_in_fuel;
	char d_g_remoter_start;
	char d_g_remote_stop;
	char d_g_priming_pump;
	char d_g_increase_speed;
	char d_g_ecu_slave_on;
	char d_g_lo_lvl_raw_wat;
	char d_g_mode2;
	char d_g_decrease_speed;
	char d_g_mode1;
	char d_g_fuel_leak_sb;
	char d_g_ecu_master_on;
	char d_g_fuel_leak_ps;
	char d_g_lo_lvl_coolant;
	char d_g_very_lo_oil_p;
	char d_g_very_hi_cool_t;
	char d_g_rpu_sd3;
	char d_g_rpu_sd4;
	char d_g_rpu_sd5;
	char d_g_emstop_rpu;
	char d_g_turning_gear;
	char d_g_light_test;
	char d_g_start_air_p_lo;
	char d_g_diff_eng_oil_p;
	char d_g_pre_heat_cde;
	char d_g_override;
	char d_g_ecucomm;
	char d_g_ecu_warning;
	char d_g_ecu_bin3;
	char d_g_ecu_bin4;
	char d_g_ecu_bin5;
	char d_g_ecu_bin6;
	char d_g_glow_lamp;
	char d_g_ecu_bin8;
	char d_g_ain11;
	char d_g_ain13;
	char d_g_shutoffreqm;
	char d_g_shutoffreqs;
	char d_g_ra153_bout;
	char d_g_detcon;
	char d_g_rpu_fail;
	char d_g_bw_rpu_fuelsol;
	char d_g_bw_rpu_stopsol;
	char d_g_ecu_alarmlist;
	char d_g_ecucompconsist;
	char d_g_actcall_fail;
	char d_g_derate_alarm;
	char d_g_ecudiagblocked;
	char d_g_pwr_limited;
	char d_g_scm_fail;
	char d_g_rtcbatteryflat;
	char d_g_emergency_stop;
	char d_g_overload;
	char d_g_othrcommer;
	char d_g_chrgalternfail;
	char d_g_stop_fail;
	char d_g_overspeed;
	char d_g_underspeed;
	char d_g_pickup_fail;
	char d_g_battery_flat;
	char d_g_start_fail;
	char d_g_op_rpmdep;
	char d_g_cp_rpmdep;
	char d_g_wrn_rpm_diff;
	char d_g_switchtobatt_b;
	char d_g_start_blocking;
	char d_g_load_blocking;
	char d_g_primbatactive;
	char d_g_secbatactive;
	char d_g_emssupply10a;
	char d_g_extrasupply7a;
	char d_g30supply7a;
	char d_g_ecu_yellowlamp;
	char d_g_ecu_red_lamp;
	char d_g_ecu_waittostrt;

#ifdef MAHTS648
	char d_g_boost_temp_portside_high;
	char d_g_boost_temp_starboard_high;
	char d_g_coolant_press_low;
    char d_g_exhaust_temp_starboard_high;
	char d_g_exhaust_temp_portside_high;
	char d_g_raw_wat_press_low;
	char d_g_de_bearing_temp_high;
	char d_g_w_winding_temp_high;
	char d_g_v_winding_temp_high;
	char d_g_u_winding_temp_high;
	char d_g_nde_bearing_temp_high;
    char d_g_fuel_pressure_shutdown;
    char d_g_common_alarm;
	char d_g_common_shutdown;
	char d_g_common_cooldown;
    char d_g_ready_to_start;
	char d_g_ready_to_load;
	char d_g_running;
#endif

	char device_offline;
}baudouin_main_generator;




typedef struct{
     float harbo_d_g_dcu_unit_battery_volt_dcu_control_power;
     float harbo_d_g_lube_oil_pressure;
     float harbo_d_g_coolant_pressure;
     float harbo_d_g_coolant_temperature;
     float harbo_d_g_lube_oil_temperature;
     float harbo_d_g_exhaust_temperature;
     float harbo_d_g_engine_speed;
     float harbo_d_g_running_hours;
     float harbo_d_g_rpu_unit_battery_a_volt_rpu_main_control_power;
     float harbo_d_g_rpu_unit_battery_b_volt_rpu_backup_control_power;
     char harbo_d_g_coolant_level_low;
     char harbo_d_g_main_power_failure;
     char harbo_d_g_backup_power_failure;
     char harbo_d_g_charger_failure;
     char harbo_d_g_lube_oil_pressure_too_low_shutdown;
     char harbo_d_g_coolant_temperature_too_high_shutdown;
     char harbo_d_g_local_emergency_stop;
     char harbo_d_g_remote_emergency_stop;
     char harbo_d_g_rpu_emergency_stop;
     char harbo_d_g_lube_oil_pressure_low_alarm;
     char harbo_d_g_coolant_pressure_low_alarm;
     char harbo_d_g_coolant_temperature_high_alarm;
     char harbo_d_g_lube_oil_temperature_high_alarm;
     char harbo_d_g_exhaust_temperature_high_alarm;
     char harbo_d_g_overspeed_alarm;
     char harbo_d_g_overspeed_shutdown;
     char harbo_d_g_speed_pickup_sensor_fail;
     char harbo_d_g_dcu_unit_battery_volt_dct_control_power_low;
     char harbo_d_g_start_fail;
     char harbo_d_g_rpu_unit_battery_a_volt_rpu_main_control_power_low;
     char harbo_d_g_rpu_unit_battery_b_volt_rpu_main_control_power_low;
     char harbo_d_g_common_alarm;
     char harbo_d_g_common_warning;
     char harbo_d_g_common_shutdown;
     char harbo_d_g_common_cooldowm;
     char harbo_d_g_common_faultsensor_fail;
     char harbo_d_g_remote_control_status_auxaccess_lock;
     char harbo_d_g_ready_to_start;
     char harbo_d_g_ready_to_load;
     char harbo_d_g_running;
     float harbo_d_g_inlet_oil_flowmeter;
     float harbo_d_g_outlet_oil_flowmeter;
     char device_offline;
}harbo_generator;


//应急发电机组
typedef struct{
	float eg_dcu_unit_battery_volt_dcu_control_power;
	float eg_lube_oil_pressure;
	float eg_coolant_pressure;
	float eg_coolant_temperature;
	float eg_lube_oil_temperature;
	float eg_engine_speed;
	float eg_running_hours;
	float eg_rpu_unit_battery_a_volt_rpu_main_control_power;
	float eg_rpu_unit_battery_b_volt_rpu_backup_control_power;
	char eg_coolant_level_low;
	char eg_main_power_failure;
	char eg_backup_power_failure;
	char eg_no1_charger_failure;
	char eg_no2_charger_failure;
	char eg_local_emergency_stop;
	char eg_remote_emergency_stop;
	char eg_rpu_emergency_stop;
	char eg_lube_oil_pressure_low_alarm;
	char eg_coolant_pressure_low_alarm;
	char eg_coolant_temperature_high_alarm;
	char eg_lube_oil_temperature_high_alarm;
	char eg_overspeed_alarm;
	char eg_overspeed_shutdown;
	char eg_speed_sensor_fail;
	char eg_dcu_unit_battery_volt_dct_control_power_low;
	char eg_start_fail;
	char eg_rpu_unit_battery_a_volt_rpu_main_control_power_low;
	char eg_rpu_unit_battery_b_volt_rpu_main_control_power_low;
	char eg_common_alarm;
	char eg_common_warning;
	char eg_common_shutdown;
	char eg_common_cooldown;
	char eg_common_faultsensor_fail;
	char eg_remote_control_status_auxaccess_lock;
	char eg_ready_to_start;
	char eg_ready_to_load;
	char eg_generator_running;
	float eg_generator_u_winding_temperature;
	float eg_generator_v_winding_temperature;
	float eg_generator_w_winding_temperature;
#ifndef MAHTS648
#else

    float eg_exhaust_temperature;
    char eg_fuel_leakage_alarm;
    char eg_speed_pickup_sensor_fail;
    char eg_common_cooldowm;
    char eg_running;
#endif
	char device_offline;
}emergency_generator;

typedef struct{
	value_type eg_dcu_unit_battery_volt_dcu_control_power;
	value_type eg_lube_oil_pressure;
	value_type eg_coolant_pressure;
	value_type eg_coolant_temperature;
	value_type eg_lube_oil_temperature;
	value_type eg_engine_speed;
	value_type eg_running_hours;
	value_type eg_rpu_unit_battery_a_volt_rpu_main_control_power;
	value_type eg_rpu_unit_battery_b_volt_rpu_backup_control_power;
	value_type eg_coolant_level_low;
	value_type eg_main_power_failure;
	value_type eg_backup_power_failure;
	value_type eg_no1_charger_failure;
	value_type eg_no2_charger_failure;
	value_type eg_local_emergency_stop;
	value_type eg_remote_emergency_stop;
	value_type eg_rpu_emergency_stop;
	value_type eg_lube_oil_pressure_low_alarm;
	value_type eg_coolant_pressure_low_alarm;
	value_type eg_coolant_temperature_high_alarm;
	value_type eg_lube_oil_temperature_high_alarm;
	value_type eg_overspeed_alarm;
	value_type eg_overspeed_shutdown;
	value_type eg_speed_sensor_fail;
	value_type eg_dcu_unit_battery_volt_dct_control_power_low;
	value_type eg_start_fail;
	value_type eg_rpu_unit_battery_a_volt_rpu_main_control_power_low;
	value_type eg_rpu_unit_battery_b_volt_rpu_main_control_power_low;
	value_type eg_common_alarm;
	value_type eg_common_warning;
	value_type eg_common_shutdown;
	value_type eg_common_cooldown;
	value_type eg_common_faultsensor_fail;
	value_type eg_remote_control_status_auxaccess_lock;
	value_type eg_ready_to_start;
	value_type eg_ready_to_load;
	value_type eg_generator_running;
	value_type eg_generator_u_winding_temperature;
	value_type eg_generator_v_winding_temperature;
	value_type eg_generator_w_winding_temperature;
	value_type device_offline;
}emergency_generator_store;

//空调冷藏机
typedef struct{
	char no1_wheel_house_air_condition_common_alarm;
	char no2_wheel_house_air_condition_common_alarm;
	char no1_galley_package_air_condition_common_alarm;
	char no2_galley_package_air_condition_common_alarm;
	char m_s_b_room_package_air_condition_common_alarm;
	char no1_jacking_vfd_room_air_condition_common_alarm;
	char no2_jacking_vfd_room_air_condition_common_alarm;
	char deck_storeroom_air_condition_common_alarm;
	char paint_locker_room_air_condition_common_alarm;
	char workshop_air_condition_common_alarm;
	char a_z_v_f_d_room_air_condition_common_alarm;
	char no1_a_c_chilled_unit_common_alarm;
	char no2_a_c_chilled_unit_common_alarm;
	char no3_a_c_chilled_unit_common_alarm;
	char chiller_water_pump_system_common_alarm;
	char no1_ahu_common_alarm;
	char no2_ahu_common_alarm;
	char no3_ahu_common_alarm;
	char no4_ahu_common_alarm;
	char refrigeration_plant_common_alarm;
	char refrigerant_leaking_alarm;

#ifdef MAHTS648
	char refrigeration_plant_compressor_1_high_pressure_alarm;
    char refrigeration_plant_compressor_1_overload_alarm;
	char refrigeration_plant_compressor_1_overheat_alarm;
	char refrigeration_plant_compressor_2_high_pressure_alarm;
    char refrigeration_plant_compressor_2_overload_alarm;
	char refrigeration_plant_compressor_2_overheat_alarm;
	char refrigeration_plant_compressor_1_water_pressure_low_alarm;
    char refrigeration_plant_compressor_2_water_pressure_low_alarm;
	char refrigeration_plant_compressor_1_running;
    char refrigeration_plant_compressor_2_running;
    char freezer_room_overheat;
	char chiller_room_overheat;
    char dry_provision_store_room_overheat;
    char no1_ac_chiller_compressor_1_running;
	char no1_ac_chiller_compressor_2_running;
	char no1_ac_chiller_antifreeze_alarm;
    char no1_ac_chiller_chilled_water_flow_alarm;
    char no1_ac_chiller_compressor_1_high_pressure_alarm;
	char no1_ac_chiller_compressor_1_over_temp;
	char no1_ac_chiller_compressor_1_overload;
	char no1_ac_chiller_cooling_water_1_flow_alarm;
	char no1_ac_chiller_compressor_1_low_pressure_alarm;
    char no1_ac_chiller_compressor_1_oil_level_alarm;
	char no1_ac_chiller_compressor_2_high_pressure_alarm;
	char no1_ac_chiller_compressor_2_over_temp;
	char no1_ac_chiller_compressor_2_overload;
	char no1_ac_chiller_cooling_water_2_flow_alarm;
	char no1_ac_chiller_compressor_2_low_pressure_alarm;
    char no1_ac_chiller_compressor_2_oil_level_alarm;
	char no1_ac_chiller_power_failure;
////////
    char no2_ac_chiller_compressor_1_running;
	char no2_ac_chiller_compressor_2_running;
	char no2_ac_chiller_antifreeze_alarm;
    char no2_ac_chiller_chilled_water_flow_alarm;
    char no2_ac_chiller_compressor_1_high_pressure_alarm;
	char no2_ac_chiller_compressor_1_over_temp;
	char no2_ac_chiller_compressor_1_overload;
	char no2_ac_chiller_cooling_water_1_flow_alarm;
	char no2_ac_chiller_compressor_1_low_pressure_alarm;
    char no2_ac_chiller_compressor_1_oil_level_alarm;
	char no2_ac_chiller_compressor_2_high_pressure_alarm;
	char no2_ac_chiller_compressor_2_over_temp;
	char no2_ac_chiller_compressor_2_overload;
	char no2_ac_chiller_cooling_water_2_flow_alarm;
	char no2_ac_chiller_compressor_2_low_pressure_alarm;
    char no2_ac_chiller_compressor_2_oil_level_alarm;
	char no2_ac_chiller_power_failure;

    char no1_msb_room_package_air_condition_common_alarm;
	char no2_msb_room_package_air_condition_common_alarm;
	char no3_msb_room_package_air_condition_common_alarm;
	char no4_msb_room_package_air_condition_common_alarm;
    char electric_workshop_package_air_condition_common_alarm;
    char mechanical_workshop_package_air_condition_common_alarm;
    char fwd_thruster_vfd_room_package_air_condition_common_alarm;
    char aft_thruster_vfd_room_package_air_condition_common_alarm;
    char ecr_air_condition_common_alarm;


    float freezer_room_temp;
	float chiller_room_temp;
	float dry_provision_store_room_temp;
	float refrigeration_plant_compress_1_running_time;
	float refrigeration_plant_compress_2_running_time;
	float no1_ac_chiller_chilled_water_inlet_temp;
	float no1_ac_chiller_chilled_water_outlet_temp;
	float no2_ac_chiller_chilled_water_inlet_temp;
	float no2_ac_chiller_chilled_water_outlet_temp;
#endif

	char device_offline;	
}air_conditioner_refrigeration_system;

typedef struct{
	value_type no1_wheel_house_air_condition_common_alarm;
	value_type no2_wheel_house_air_condition_common_alarm;
	value_type no1_galley_package_air_condition_common_alarm;
	value_type no2_galley_package_air_condition_common_alarm;
	value_type m_s_b_room_package_air_condition_common_alarm;
	value_type no1_jacking_vfd_room_air_condition_common_alarm;
	value_type no2_jacking_vfd_room_air_condition_common_alarm;
	value_type deck_storeroom_air_condition_common_alarm;
	value_type paint_locker_room_air_condition_common_alarm;
	value_type workshop_air_condition_common_alarm;
	value_type a_z_v_f_d_room_air_condition_common_alarm;
	value_type no1_a_c_chilled_unit_common_alarm;
	value_type no2_a_c_chilled_unit_common_alarm;
	value_type no3_a_c_chilled_unit_common_alarm;
	value_type chiller_water_pump_system_common_alarm;
	value_type no1_ahu_common_alarm;
	value_type no2_ahu_common_alarm;
	value_type no3_ahu_common_alarm;
	value_type no4_ahu_common_alarm;
	value_type refrigeration_plant_common_alarm;
	value_type refrigerant_leaking_alarm;

#ifdef MAHTS648
	value_type refrigeration_plant_compressor_1_high_pressure_alarm;
    value_type refrigeration_plant_compressor_1_overload_alarm;
	value_type refrigeration_plant_compressor_1_overheat_alarm;
	value_type refrigeration_plant_compressor_2_high_pressure_alarm;
    value_type refrigeration_plant_compressor_2_overload_alarm;
	value_type refrigeration_plant_compressor_2_overheat_alarm;
	value_type refrigeration_plant_compressor_1_water_pressure_low_alarm;
    value_type refrigeration_plant_compressor_2_water_pressure_low_alarm;
	value_type refrigeration_plant_compressor_1_running;
    value_type refrigeration_plant_compressor_2_running;
    value_type freezer_room_overheat;
	value_type chiller_room_overheat;
    value_type dry_provision_store_room_overheat;
    value_type no1_ac_chiller_compressor_1_running;
	value_type no1_ac_chiller_compressor_2_running;
	value_type no1_ac_chiller_antifreeze_alarm;
    value_type no1_ac_chiller_chilled_water_flow_alarm;
    value_type no1_ac_chiller_compressor_1_high_pressure_alarm;
	value_type no1_ac_chiller_compressor_1_over_temp;
	value_type no1_ac_chiller_compressor_1_overload;
	value_type no1_ac_chiller_cooling_water_1_flow_alarm;
	value_type no1_ac_chiller_compressor_1_low_pressure_alarm;
    value_type no1_ac_chiller_compressor_1_oil_level_alarm;
	value_type no1_ac_chiller_compressor_2_high_pressure_alarm;
	value_type no1_ac_chiller_compressor_2_over_temp;
	value_type no1_ac_chiller_compressor_2_overload;
	value_type no1_ac_chiller_cooling_water_2_flow_alarm;
	value_type no1_ac_chiller_compressor_2_low_pressure_alarm;
    value_type no1_ac_chiller_compressor_2_oil_level_alarm;
	value_type no1_ac_chiller_power_failure;
////////
    value_type no2_ac_chiller_compressor_1_running;
	value_type no2_ac_chiller_compressor_2_running;
	value_type no2_ac_chiller_antifreeze_alarm;
    value_type no2_ac_chiller_chilled_water_flow_alarm;
    value_type no2_ac_chiller_compressor_1_high_pressure_alarm;
	value_type no2_ac_chiller_compressor_1_over_temp;
	value_type no2_ac_chiller_compressor_1_overload;
	value_type no2_ac_chiller_cooling_water_1_flow_alarm;
	value_type no2_ac_chiller_compressor_1_low_pressure_alarm;
    value_type no2_ac_chiller_compressor_1_oil_level_alarm;
	value_type no2_ac_chiller_compressor_2_high_pressure_alarm;
	value_type no2_ac_chiller_compressor_2_over_temp;
	value_type no2_ac_chiller_compressor_2_overload;
	value_type no2_ac_chiller_cooling_water_2_flow_alarm;
	value_type no2_ac_chiller_compressor_2_low_pressure_alarm;
    value_type no2_ac_chiller_compressor_2_oil_level_alarm;
	value_type no2_ac_chiller_power_failure;

    value_type no1_msb_room_package_air_condition_common_alarm;
	value_type no2_msb_room_package_air_condition_common_alarm;
	value_type no3_msb_room_package_air_condition_common_alarm;
	value_type no4_msb_room_package_air_condition_common_alarm;
    value_type electric_workshop_package_air_condition_common_alarm;
    value_type mechanical_workshop_package_air_condition_common_alarm;
    value_type fwd_thruster_vfd_room_package_air_condition_common_alarm;
    value_type aft_thruster_vfd_room_package_air_condition_common_alarm;
    value_type ecr_air_condition_common_alarm;


    value_type freezer_room_temp;
	value_type chiller_room_temp;
	value_type dry_provision_store_room_temp;
	value_type refrigeration_plant_compress_1_running_time;
	value_type refrigeration_plant_compress_2_running_time;
	value_type no1_ac_chiller_chilled_water_inlet_temp;
	value_type no1_ac_chiller_chilled_water_outlet_temp;
	value_type no2_ac_chiller_chilled_water_inlet_temp;
	value_type no2_ac_chiller_chilled_water_outlet_temp;
#endif

	value_type device_offline;	
}air_conditioner_refrigeration_system_store;

//压缩空气系统
typedef struct{
	char no1_air_compressor_common_alarm;
	char no2_air_compressor_common_alarm;
	char service_air_compressor_common_alarm;
	char quick_closing_valve_air_reservoir_low_pressure;
	char air_dryer_common_alarm;
	float no1_main_air_reservoir_pressure;
	float no2_main_air_reservoir_pressure;
	float daily_air_reservoir_pressure;
	float no1_main_deck_air_reservoir_pressure;
	float no2_main_deck_air_reservoir_pressure;
	float no3_main_deck_air_reservoir_pressure;
	float no4_main_deck_air_reservoir_pressure;	
	char device_offline;
}compressed_air_system;

typedef struct{
	value_type no1_air_compressor_common_alarm;
	value_type no2_air_compressor_common_alarm;
	value_type service_air_compressor_common_alarm;
	value_type quick_closing_valve_air_reservoir_low_pressure;
	value_type air_dryer_common_alarm;
	value_type no1_main_air_reservoir_pressure;
	value_type no2_main_air_reservoir_pressure;
	value_type daily_air_reservoir_pressure;
	value_type no1_main_deck_air_reservoir_pressure;
	value_type no2_main_deck_air_reservoir_pressure;
	value_type no3_main_deck_air_reservoir_pressure;
	value_type no4_main_deck_air_reservoir_pressure;	
	value_type device_offline;
}compressed_air_system_store;

//通用充放电板
typedef struct{
	char low_insulation;
	char rectifier_fault;
	char charger_fault;
	char inverter_failure;
	char battery_protector_active;
	char battery_discharge;
	char battery_low_voltage;
	char inverter_low_voltage;

#ifdef MAHTS648
    char no1_battery_discharge;
    char no1_battery_voltage_low;
    char no1_charger_failure;
    char no1_charger_power_failure;
    char no1_charger_common_alarm;
    char no2_battery_discharge;
    char no2_battery_voltage_low;
    char no2_charger_failure;
    char no2_charger_power_failure;
    char no2_charger_common_alarm;
    char no3_battery_discharge;
    char no4_battery_discharge;
    char chp_bus_a_low_insulation;
    char chp_bus_b_low_insulation;
    char de1_low_insulation;
    char de1_common_failure;
    char de2_low_insulation;
    char de2_common_failure;
#endif
char device_offline;
}general_charger_and_discharger_panel;

//主变压器
typedef struct{
	char no;
	float main_transformer_u_winding_temp;
	float main_transformer_v_winding_temp;
	float main_transformer_w_winding_temp;
#ifndef MAHTS648

#else
    char transformer_door_openalarm;
    char transformer_thermostat_failure;
    char transformer_high_temp_alarm;
    float winding_r_temp;
    float winding_s_temp;
    float winding_t_temp;
    char mt_cooling_fan_stop;
    char mt_high_temp_high_trip_alarm;
    char mt_cooling_fan_overload;
    char mt_winding_d_temp_sensor_failure;
    char mt_winding_r_temp_sensor_failure;
    char mt_winding_s_temp_sensor_failure;
    char mt_winding_t_temp_sensor_failure;
#endif
	char device_offline;
}main_transformer;

//主变压器
typedef struct{
	char no;
	value_type main_transformer_u_winding_temp;
	value_type main_transformer_v_winding_temp;
	value_type main_transformer_w_winding_temp;
#ifndef MAHTS648

#else
    value_type no1_main_transformer_u_winding_temp;
	value_type no1_main_transformer_v_winding_temp;
	value_type no1_main_transformer_w_winding_temp;
    value_type no2_main_transformer_u_winding_temp;
	value_type no2_main_transformer_v_winding_temp;
	value_type no2_main_transformer_w_winding_temp;
#endif
	value_type device_offline;
}main_transformer_store;

//艏侧推
typedef struct{
	char no;
	char b_t_remote_control_system_ac_power_failure;
	char b_t_remote_control_system_dc_power_failure;
	char b_t_remote_control_system_failure;
	char b_t_main_motor_overload;
	char b_t_main_motor_high_temperature;
	char b_t_oil_low_level;
	char b_t_oil_low_pressure;
	char b_t_oil_high_pressure;
	char b_t_main_motor_short_circuit;
	char b_t_emergency_stop_circuit_failure;
	char b_t_too_many_times_start;
	char b_t_integrated_alarm;
#ifndef MAHTS648
#else
    char b_t_hydraulic_oil_tank_low_oil_level;
    char b_t_spare_power_failure;
    char b_t_main_motor_high_temp;
    char b_t_main_power_fault;
    char b_t_ac220v_power_fault;
    char b_t_dc24v_power_fault;
    char b_t_main_vfd_integrate;
    char b_t_integrate_alarm;
	char b_t_emcy_wire_break;
    float b_t_fresh_water_cooling_pump_outlet_pressure;
    float b_t_sea_water_cooling_pump_outlet_pressure;
    char b_t_cb_status;
    char b_t_ams_common_alarm;
#endif
	char device_offline;
}bow_thruster;

//艏侧推
typedef struct{

    value_type b_t_fresh_water_cooling_pump_outlet_pressure;
    value_type b_t_sea_water_cooling_pump_outlet_pressure;

}bow_thruster_store;


//power system 2
typedef struct{
    char dg1_start_fail;
    char dg2_start_fail;
    char dg3_start_fail;
    char dg4_start_fail;
    char dg1_acb_abnormal_trip;
    char dg2_acb_abnormal_trip;
    char dg3_acb_abnormal_trip;
    char dg4_acb_abnormal_trip;
    char dg1_acb_close_fail;
    char dg2_acb_close_fail;
    char dg3_acb_close_fail;
    char dg4_acb_close_fail;
    char ac690v_bus_a_insul_low;
    char ac690v_bus_b_insul_low;
    char ac690v_bus_a_voltage_high;
    char ac690v_bus_b_voltage_high;
    char ac690v_bus_a_voltage_low;
    char ac690v_bus_b_voltage_low;
    char ac690v_bus_a_frequency_high;
    char ac690v_bus_b_frequency_high;
    char ac690v_bus_a_frequency_low;
    char ac690v_bus_b_frequency_low;
    char preferential_trip_alarm;
    char ac690v_msb_common_alarm;
    char ac440v_bus_a_insul_low;
    char ac440v_bus_b_insul_low;
    char ac220v_bus_a_insul_low;
    char ac220v_bus_b_insul_low;
    char ac440v_msb_em_cy_stop_and_pre_trip_source_fail;
    char ac440v_msb_dc24v_source_fault;
    char ac690v_msb_dc24v_source_a_fault;
    char ac690v_msb_dc24v_source_b_fault;
    char dg1_acb_closed;
    char dg2_acb_closed;
    char dg3_acb_closed;
    char dg4_acb_closed;
    char ddbt_acb_closed;
    char pst_acb_closed;
    char sst_acb_closed;
    char bt1_acb_closed;
    char bt2_acb_closed;
    char dt1primary_acb_closed;
    char dt2primary_acb_closed;
    char dt1secondary_acb_closed;
    char dt2secondary_acb_closed;
    char ac440v_bus_tie_breaker_close;
    char shore_cb_closed;
    float dg1_power_kw;
    float dg2_power_kw;
    float dg3_power_kw;
    float dg4_power_kw;
    float dg1_voltagev;
    float dg2_voltagev;
    float dg3_voltagev;
    float dg4_voltagev;
    float ac690v_bus_a_frequencyhz;
    float ac690v_bus_b_frequencyhz;
    float ac690v_bus_a_total_powerkw;
    float ac690v_bus_b_total_powerkw;
    float ac690v_bus_total_powerkw;
    float ac690v_bus_a_surplus_powerkw;
    float ac690v_bus_b_surplus_powerkw;
    float ac690v_bus_surplus_powerkw;
    float dg1_power_factor;
    float dg2_power_factor;
    float dg3_power_factor;
    float dg4_power_factor;
    char dg1_running;
    char dg2_running;
    char dg3_running;
    char dg4_running;
    char dg1_common_alarm;
    char dg2_common_alarm;
    char dg3_common_alarm;
    char dg4_common_alarm;
    char stand_by_gen_changeover_alarm;
    char dg1_voltage_high;
    char dg1_voltage_low;
    char dg1_frequency_high;
    char dg1_frequency_low;
    char dg2_voltage_high;
    char dg2_voltage_low;
    char dg2_frequency_high;
    char dg2_frequency_low;
    char dg3_voltage_high;
    char dg3_voltage_low;
    char dg3_frequency_high;
    char dg3_frequency_low;
    char dg4_voltage_high;
    char dg4_voltage_low;
    char dg4_frequency_high;
    char dg4_frequency_low;
    char load_sharing_fail;
    char ac690v_pms1_fault;
    char ac690v_pms2_fault;
    char t3_cb_status;
    char t4_cb_status;
    char sc_cb_status;
    char device_offline;
}power_system2;

//污水处理装置
typedef struct{
    char sewage_treatment_plant_running;
    char sewage_treatment_plant_common_fault;
    char sewage_treatment_plant_buffer_tank_high_level;
    char sewage_treatment_plant_cutting_pump_running;
    char device_offline;
}sewage_treatment_plant;

//右舷中央控制柜 STBD AZIMUTH CENTER CONTROL CABINET
typedef struct{
    float stbd_no1_azimuth_position;
    float stbd_no2_azimuth_position;
    float stbd_propeller_speed;
    float stbd_main_vfd_speed;
    float stbd_lub_oil_temperature;
    float stbd_main_motor_u_winding_temp;
    float stbd_main_motor_v_winding_temp;
    float stbd_main_motor_w_winding_temp;
    float stbd_main_motor1_bearing_temp;
    float stbd_main_motor2_bearing_temp;
    float stbd_main_motor_wind_in_temp;
    float stbd_main_motor_wind_out_temp;
    float stbd_pms_power_capacity;
    float stbd_main_vfd_load;
    float stbd_control_position;
    char stbd_main_power_fault;
    char stbd_back_power_fault;
    char stbd_lo_level_low;
    char stbd_lo_temp_high;
    char stbd_lo_filter_blocked;
    char stbd_lo_pressure_low;
    char stbd_lo_standby_pump_switched;
    char stbd_azimut1_vfd_fault;
    char stbd_azimut2_vfd_fault;
    char stbd_azimuth_control_fault;
    char stbd1lo_pump_overload;
    char stbd2lo_pump_overload;
    char stbd_azimuth_position1control_feedback_lost;
    char stbd_fwd_azimuth_command_lost;
    char stbd_fwd_speed_command_lost;
    char stbd_aft_azimuth_command_lost;
    char stbd_aft_speed_command_lost;
    char stbd_main_vfd_common_alarm;
    char stbd_main_vfd_estop;
    char stbd_mian_motor_water_leak;
    char stbd_shaft_lock_air_pressure_low;
    char stbd_gravity_oil_level_low;
    char stbd_gravity_air_pressure_low;
    char stbd_gravity_air_pressure_high;
    char stbd_main_motor_u_temp_high;
    char stbd_main_motor_v_temp_high;
    char stbd_main_motor_w_temp_high;
    char stbd_front_shaft_bearing_temp_high;
    char stbd_after_shaft_bearing_temp_high;
    char stbd_main_vfd_load_feedback_lost;
    char stbd_pms_power_capacity_feedback_lost;
    char stbd_main_vfd_speed_feedback_lost;
    char stbd_main_vfd_speed_control_fault;
    char stbd_azimuth1vfd_communication_lost;
    char stbd_azimuth2vfd_communication_lost;
    char stbd_main_motor1_bearing_temp_high;
    char stbd_main_motor2_bearing_temp_high;
    char stbd_main_motor_wind_in_temp_high;
    char stbd_main_motor_wind_out_temp_high;
    char stbd_main_motor_wind_temp_high_sld_per_alarm;
    char stbd_main_motor_bearing_temp_high_sld_per_alarm;
    char stbd_sld_active;
    char stbd_bridge_station_communication_lost;
    char stbd_heavy_load_inquiry_fault;
    char stbd_main_vfd_start_interlock;
    char stbd_main_vfd_fault;
    char stbd_main_vfd_esiop_wire_broken;
    char stbd_gravity_oil_level_high;
    char stbd_azimuth_position2indication_feedback_lost;
    char stbd_azimuth_position_feedback_unsynchronized;
    char stbd_vibration_sensor1_lost;
    char stbd_vibration_sensor2_lost;
    char stbd_dp_azimuth_command_lost;
    char stbd_dp_speed_command_lost;
    char stbd_joy_azimuth_command_lost;
    char stbd_joy_speed_command_lost;
    char stbd_ecr_speed_command_lost;
    char stbd_ecr_station_communication_lost;
    float sst_fresh_water_cooling_pump_outlet_pressure;
    float sst_sea_water_cooling_pump_outlet_pressure;
    char sst_cb_status;
    char device_offline;
}stbd_azimuth_center_control_cabinet;

typedef struct{
    value_type sst_fresh_water_cooling_pump_outlet_pressure;
    value_type sst_sea_water_cooling_pump_outlet_pressure;
}stbd_azimuth_center_control_cabinet_store;

//左舷中央控制柜 STBD AZIMUTH CENTER CONTROL CABINET
typedef struct{
    float port_no1_azimuth_position;
    float port_no2_azimuth_position;
    float port_propeller_speed;
    float port_main_vfd_speed;
    float port_lub_oil_temperature;
    float port_main_motor_u_winding_temp;
    float port_main_motor_v_winding_temp;
    float port_main_motor_w_winding_temp;
    float port_main_motor1_bearing_temp;
    float port_main_motor2_bearing_temp;
    float port_main_motor_wind_in_temp;
    float port_main_motor_wind_out_temp;
    float port_pms_power_capacity;
    float port_main_vfd_load;
    float port_control_position;
    char port_main_power_fault;
    char port_back_power_fault;
    char port_lo_level_low;
    char port_lo_temp_high;
    char port_lo_filter_blocked;
    char port_lo_pressure_low;
    char port_lo_standby_pump_switched;
    char port_azimut1_vfd_fault;
    char port_azimut2_vfd_fault;
    char port_azimuth_control_fault;
    char port1lo_pump_overload;
    char port2lo_pump_overload;
    char port_azimuth_position1control_feedback_lost;
    char port_fwd_azimuth_command_lost;
    char port_fwd_speed_command_lost;
    char port_aft_azimuth_command_lost;
    char port_aft_speed_command_lost;
    char port_main_vfd_common_alarm;
    char port_main_vfd_estop;
    char port_mian_motor_water_leak;
    char port_shaft_lock_air_pressure_low;
    char port_gravity_oil_level_low;
    char port_gravity_air_pressure_low;
    char port_gravity_air_pressure_high;
    char port_main_motor_u_temp_high;
    char port_main_motor_v_temp_high;
    char port_main_motor_w_temp_high;
    char port_front_shaft_bearing_temp_high;
    char port_after_shaft_bearing_temp_high;
    char port_main_vfd_load_feedback_lost;
    char port_pms_power_capacity_feedback_lost;
    char port_main_vfd_speed_feedback_lost;
    char port_main_vfd_speed_control_fault;
    char port_azimuth1vfd_communication_lost;
    char port_azimuth2vfd_communication_lost;
    char port_main_motor1_bearing_temp_high;
    char port_main_motor2_bearing_temp_high;
    char port_main_motor_wind_in_temp_high;
    char port_main_motor_wind_out_temp_high;
    char port_main_motor_wind_temp_high_sld_per_alarm;
    char port_main_motor_bearing_temp_high_sld_per_alarm;
    char port_sld_active;
    char port_bridge_station_communication_lost;
    char port_heavy_load_inquiry_fault;
    char port_main_vfd_start_interlock;
    char port_main_vfd_fault;
    char port_main_vfd_esiop_wire_broken;
    char port_gravity_oil_level_high;
    char port_azimuth_position2indication_feedback_lost;
    char port_azimuth_position_feedback_unsynchronized;
    char port_vibration_sensor1_lost;
    char port_vibration_sensor2_lost;
    char port_dp_azimuth_command_lost;
    char port_dp_speed_command_lost;
    char port_joy_azimuth_command_lost;
    char port_joy_speed_command_lost;
    char port_ecr_speed_command_lost;
    char port_ecr_station_communication_lost;
    float pst_fresh_water_cooling_pump_outlet_pressure;
    float pst_sea_water_cooling_pump_outlet_pressure;
    char pst_cb_status;
    char device_offline;

}port_azimuth_center_control_cabinet;


typedef struct{
    value_type pst_fresh_water_cooling_pump_outlet_pressure;
    value_type pst_sea_water_cooling_pump_outlet_pressure;
}port_azimuth_center_control_cabinet_store;

//机舱充放电板
typedef struct{
	char ups_low_insulation;
	char ups_battery_low_voltage;
	char ups_input_fault;
	char ups_battery_bypass_activation;
	char ups_common_alarm;
	char ups_alarm;
	char ups_running;
	char ups_battery_mode;
	char ups_battery_protection_trip;
	char ups_output_alarm;
#ifdef MAHTS648
    char no1_battery_discharge;
    char no1_battery_voltage_low;
    char no1_charger_failure;
    char no1_charger_power_failure;
    char no1_charger_common_alarm;
    char no2_battery_discharge;
    char no2_battery_voltage_low;
    char no2_charger_failure;
    char no2_charger_power_failure;
    char no2_charger_common_alarm;
    char no3_battery_discharge;
    char no4_battery_discharge;
    char chp_bus_a_low_insulation;
    char chp_bus_b_low_insulation;
    char de1_low_insulation;
    char de1_common_failure;
    char de2_low_insulation;
    char de2_common_failure;
#endif	
	char device_offline;
}e_r_charger_and_discharger_panel;

typedef struct{
	value_type ups_low_insulation;
	value_type ups_battery_low_voltage;
	value_type ups_input_fault;
	value_type ups_battery_bypass_activation;
	value_type ups_common_alarm;
	value_type ups_alarm;
	value_type ups_running;
	value_type ups_battery_mode;
	value_type ups_battery_protection_trip;
	value_type ups_output_alarm;
#ifdef MAHTS648
    value_type ups_battery_discharge;
#endif	
	value_type device_offline;
}e_r_charger_and_discharger_panel_store;

//液位遥测
typedef struct{
	char level_mearsurement_system_common_alarm;
	float forep_draft;
	float fores_draft;
	float aftp_draft;
	float afts_draft;
#ifdef MAHTS648
	char s_g_roomp_bilge_well_high_level;
	char s_g_rooms_bilge_well_high_level;
	char aft_cement_tkp_a_bilge_well_high_level;
	char aft_cement_tks_a_bilge_well_high_level;
	char aft_cement_tkm_bilge_well_high_level;
	char fwd_cement_tkp_a_bilge_well_high_level;
	char fwd_cement_tks_a_bilge_well_high_level;
	char fwd_cement_tkm_bilge_well_high_level;
	char e_rp_a_bilge_well_high_level;
	char e_rs_a_bilge_well_high_level;
	char e_rp_f_bilge_well_high_level;
	char e_rs_f_bilge_well_high_level;
	char b_t_f_bilge_well_high_level; // b_t_room_bilge_well_high_level->b_t_f_bilge_well_high_level 25.5.19
	char base_oil_ro_tkp_high_level;
	char base_oil_ro_tks_high_level;
	char no1_mud_tkp_high_level;
	char no1_mud_tks_high_level;
	char no1_mud_tkp_low_level;
	char no1_mud_tks_low_level;
	char no2_mud_tkp_high_level;
	char no2_mud_tks_high_level;
	char no2_mud_tkp_low_level;
	char no2_mud_tks_low_level;
	char no3_f_o_overflow_tkc_high_level;
	char bilge_holding_tkp_high_level;
	char dirty_oil_tk_high_level;
	char sewage_tk_high_level;
	char f_o_day_tkp_high_level;
	char f_o_day_tkp_low_level;
	char f_o_day_tks_high_level;
	char f_o_day_tks_low_level;
	char b_t_a_bilge_well_high_level; // b_t_a_bilge_well_low_level->b_t_a_bilge_well_high_level 25.5.27
	char f_o_overflow_alarm;
	char hpr_tks_high_level;
	char em_cy_generator_set_daily_tk_low_level;
	char em_cy_generator_set_daily_tk_high_level;
	char no1_bt_f_w_expansion_t_k_low_level;
	char no2_bt_f_w_expansion_t_k_low_level;
	char port_azimuth_f_w_expansion_t_k_low_level;
	char stbd_azimuth_f_w_expansion_t_k_low_level;
	char no1_f_w_expansion_t_k_low_level;
	char no2_f_w_expansion_t_k_low_level;
	char no3_f_w_expansion_t_k_low_level;
	char no4_f_w_expansion_t_k_low_level;
	char quick_closing_air_receiver_pressure_low_alarm;
	char output_mud_tk_level_alarm_bell;
	char spare;
	float no1_db_f_o_tkp_level;
	float no1_db_f_o_tks_level;
	float no1_wing_f_o_tkp_level;
	float no1_wing_f_o_tks_level;
	float no2_db_f_o_tkp_level;
	float no2_db_f_o_tks_level;
	float no2_wing_f_o_tkp_level;
	float no2_wing_f_o_tks_level;
	float no3_db_f_o_tkcoverflow_level;
	float no3_db_f_o_tkp_level;
	float no3_db_f_o_tks_level;
	float no3_db_f_o_tkc_level;
	float no4_db_f_o_tkc_level;
	float no4_wing_f_o_tkp_level;
	float no4_wing_f_o_tks_level;
	float no5_f_o_tkp_level;
	float no5_f_o_tks_level;
	float no6_db_f_o_tkc_level;
	float f_o_day_tkp_level;
	float f_o_day_tks_level;
	float f_o_settling_tkp_level;
	float fresh_water_tank5p_level;
	float no1_db_f_o_tkp_volume_m3;
	float no1_db_f_o_tks_volume_m3;
	float no1_wing_f_o_tkp_volume_m3;
	float no1_wing_f_o_tks_volume_m3;
	float no2_db_f_o_tkp_volume_m3;
	float no2_db_f_o_tks_volume_m3;
	float no2_wing_f_o_tkp_volume_m3;
	float no2_wing_f_o_tks_volume_m3;
	float no3_db_f_o_tkcoverflow_volume_m3;
	float no3_db_f_o_tkp_volume_m3;
	float no3_db_f_o_tks_volume_m3;
	float no3_db_f_o_tkc_volume_m3;
	float no4_db_f_o_tkc_volume_m3;
	float no4_wing_f_o_tkp_volume_m3;
	float no4_wing_f_o_tks_volume_m3;
	float no5_f_o_tkp_volume_m3;
	float no5_f_o_tks_volume_m3;
	float no6_db_f_o_tkc_volume_m3;
	float f_o_day_tkp_volume_m3;
	float f_o_day_tks_volume_m3;
	float f_o_settling_tkp_volume_m3;
	float fresh_water_tank5p_volume_m3;
	float no1_db_f_o_tkp_volume_percent;
	float no1_db_f_o_tks_volume_percent;
	float no1_wing_f_o_tkp_volume_percent;
	float no1_wing_f_o_tks_volume_percent;
	float no2_db_f_o_tkp_volume_percent;
	float no2_db_f_o_tks_volume_percent;
	float no2_wing_f_o_tkp_volume_percent;
	float no2_wing_f_o_tks_volume_percent;
	float no3_db_f_o_tkcoverflow_volume_percent;
	float no3_db_f_o_tkp_volume_percent;
	float no3_db_f_o_tks_volume_percent;
	float no3_db_f_o_tkc_volume_percent;
	float no4_db_f_o_tkc_volume_percent;
	float no4_wing_f_o_tkp_volume_percent;
	float no4_wing_f_o_tks_volume_percent;
	float no5_f_o_tkp_volume_percent;
	float no5_f_o_tks_volume_percent;
	float no6_db_f_o_tkc_volume_percent;
	float f_o_day_tkp_volume_percent;
	float f_o_day_tks_volume_percent;
	float f_o_settling_tkp_volume_percent;
	float fresh_water_tank5p_volume_percent;
	char no1_db_f_o_tkp_high_alarm;
	char no1_db_f_o_tks_high_alarm;
	char no1_wing_f_o_tkp_high_alarm;
	char no1_wing_f_o_tks_high_alarm;
	char no2_db_f_o_tkp_high_alarm;
	char no2_db_f_o_tks_high_alarm;
	char no2_wing_f_o_tkp_high_alarm;
	char no2_wing_f_o_tks_high_alarm;
	char no3_db_f_o_tkcoverflow_high_alarm;
	char no3_db_f_o_tkp_high_alarm;
	char no3_db_f_o_tks_high_alarm;
	char no3_db_f_o_tkc_high_alarm;
	char no4_db_f_o_tkc_high_alarm;
	char no4_wing_f_o_tkp_high_alarm;
	char no4_wing_f_o_tks_high_alarm;
	char no5_f_o_tkp_high_alarm;
	char no5_f_o_tks_high_alarm;
	char no6_db_f_o_tkc_high_alarm;
	char f_o_day_tkp_high_alarm;
	char f_o_day_tks_high_alarm;
	char f_o_settling_tkp_high_alarm;
	char fresh_water_tank5p_high_alarm;
	char no1_db_f_o_tkp_low_alarm;
	char no1_db_f_o_tks_low_alarm;
	char no1_wing_f_o_tkp_low_alarm;
	char no1_wing_f_o_tks_low_alarm;
	char no2_db_f_o_tkp_low_alarm;
	char no2_db_f_o_tks_low_alarm;
	char no2_wing_f_o_tkp_low_alarm;
	char no2_wing_f_o_tks_low_alarm;
	char no3_db_f_o_tkcoverflow_low_alarm;
	char no3_db_f_o_tkp_low_alarm;
	char no3_db_f_o_tks_low_alarm;
	char no3_db_f_o_tkc_low_alarm;
	char no4_db_f_o_tkc_low_alarm;
	char no4_wing_f_o_tkp_low_alarm;
	char no4_wing_f_o_tks_low_alarm;
	char no5_f_o_tkp_low_alarm;
	char no5_f_o_tks_low_alarm;
	char no6_db_f_o_tkc_low_alarm;
	char f_o_day_tkp_low_alarm;
	char f_o_day_tks_low_alarm;
	char f_o_settling_tkp_low_alarm;
	char fresh_water_tank5p_low_alarm;
	char f_o_setling_tkp_high_level;
    char f_o_setling_tkp_low_level;
    char f_o_setling_tks_high_level;
    char f_o_setling_tks_low_level;


#endif

	float sw_ballast_tank1p_level;
	float sw_ballast_tank1s_level;
	float sw_ballast_tank2p_level;
	float sw_ballast_tank2s_level;
	float sw_ballast_tank3p_level;
	float sw_ballast_tank3s_level;
	float sw_ballast_tank3m_level;
	float fresh_water_tank1p_level;
	float fresh_water_tank1s_level;
	float fresh_water_tank2p_level;
	float fresh_water_tank2s_level;
	float fuel_oil_store_tank_p_level;
	float fuel_oil_store_tank_s_level;
	float fuel_oil_day_tank_p_level;
	float fuel_oil_day_tank_s_level;
	float sw_ballast_tank1p_capacity;
	float sw_ballast_tank1s_capacity;
	float sw_ballast_tank2p_capacity;
	float sw_ballast_tank2s_capacity;
	float sw_ballast_tank3p_capacity;
	float sw_ballast_tank3s_capacity;
	float sw_ballast_tank3m_capacity;
	float fresh_water_tank1p_capacity;
	float fresh_water_tank1s_capacity;
	float fresh_water_tank2p_capacity;
	float fresh_water_tank2s_capacity;
	float fuel_oil_store_tank_p_capacity;
	float fuel_oil_store_tank_s_capacity;
	float fuel_oil_day_tank_p_capacity;
	float fuel_oil_day_tank_s_capacity;
	char device_offline;
}level_gauging;

//液位开关
typedef struct{
	char fuel_oil_store_tank_p_high_level_alarm; //
	char fuel_oil_store_tank_p_low_level_alarm;  //
	char no1_expansion_tank_low_level_alarm;  //
	char no2_expansion_tank_low_level_alarm;  //
	char fuel_oil_day_tank_p_high_level_alarm;   //
	char fuel_oil_day_tank_p_low_level_alarm;    //
	char lube_oil_tank_high_level_alarm; //
	char dirty_oil_tank_high_level_alarm; //
	char fuel_oil_store_tank_s_high_level_alarm;
	char fuel_oil_store_tank_s_low_level_alarm;
	char fuel_oil_day_tank_s_high_level_alarm;
	char fuel_oil_day_tank_s_low_level_alarm;


	char black_water_tank_high_level_alarm;
	char grey_water_tank_high_level_alarm;
	char e_g_m_d_o_service_tank_high_level_alarm;
	char e_g_m_d_o_service_tk_low_level_alarm;
	char spill_tank_high_level_alarm;  //
	char b_t_room_bilge_high_level_alarm;
	char em_cy_fire_fighting_pump_room_bilge_high_level_alarm;
	char no1_void_p_tank_bilge_high_level_alarm;
	char no2_void_p_tank_bilge_high_level_alarm;
	char no2_void_s_tank_bilge_high_level_alarm;
	char no3_void_p_tank_bilge_high_level_alarm;  //
	char no3_void_s_tank_bilge_high_level_alarm;  //
	char no4_void_p_tank_bilge_high_level_alarm;
	char no4_void_s_tank_bilge_high_level_alarm;
	char no5_void_p_tank_bilge_high_level_alarm;  //
	char no5_void_s_tank_bilge_high_level_alarm;  //
	char no6_void_p_tank_bilge_high_level_alarm;  //
	char no6_void_s_tank_bilge_high_level_alarm;  //
	char aux_machinery_room1_bilge_high_level_alarm;
	char aux_machinery_room2_bilge_high_level_alarm;
	char e_r_ps_bilge_high_level_alarm;
	char e_r_sb_bilge_high_level_alarm;
	char aft_thruster_rm_ps_bilge_high_level_alarm;
	char aft_thruster_rm_sb_bilge_high_level_alarm;
	char bilge_water_tank_high_level_alarm;

	char no1_jackhouse_bilge_high_level_alarm;
	char no2_jackhouse_bilge_high_level_alarm;
	char no3_jackhouse_bilge_high_level_alarm;
	char no4_jackhouse_bilge_high_level_alarm;
	char galley_transformer_room_bilge_low_level_alarm;
	char f_o_transfer_pump_room_bilge_low_level_alarm;
#ifdef MAHTS648
    char fwd_thruster_vfd_room_bilge_high_level_alarm;
	char aft_thruster_vfd_room_bilge_high_level_alarm;
	char aft_thruster_room_p_bilge_high_level_alarm;
	char aft_thruster_room_c_bilge_high_level_alarm;
	char aft_thruster_room_s_bilge_high_level_alarm;
	char pump_room_p_bilge_high_level_alarm;
	char pump_room_s_bilge_high_level_alarm;
	char store_room_bilge_high_level_alarm;
	char compressor_room_bilge_high_level_alarm;
	char water_maker_room_bilge_high_level_alarm;
	char transformer_room_bilge_high_level_alarm;
	char chiller_room_bilge_high_level_alarm;
	char switchboard_room_bilge_high_level_alarm;
    char aux_machinery_room_p_bilge_high_level_alarm;
	char aux_machinery_room_s_bilge_high_level_alarm;
	char engine_room_p_bilge_high_level_alarm;
	char engine_room_s_bilge_high_level_alarm;
	char engine_room_c_bilge_high_level_alarm;
	char pump_room_c_bilge_high_level_alarm;
	char fwd_thruster_rm_p_bilge_high_level_alarm;
	char fwd_thruster_rm_c_bilge_high_level_alarm;
	char fwd_thruster_rm_s_bilge_high_level_alarm;
	char no19_void_p_tank_bilge_high_level_alarm;  //
	char no19_void_s_tank_bilge_high_level_alarm;  //
	char no7_void_p_tank_bilge_high_level_alarm;  //
	char no7_void_s_tank_bilge_high_level_alarm;  //
	char no10_void_p_tank_bilge_high_level_alarm;  //
	char no10_void_s_tank_bilge_high_level_alarm;  //
	char cofferdam_p_bilge_high_level_alarm;
	char cofferdam_s_bilge_high_level_alarm;
	char no11_void_p_tank_bilge_high_level_alarm;  //
	char no11_void_s_tank_bilge_high_level_alarm;  //
	char no12_void_p_tank_bilge_high_level_alarm;  //
	char no12_void_s_tank_bilge_high_level_alarm;  //
	char no20_void_p_tank_bilge_high_level_alarm;  //
	char no20_void_s_tank_bilge_high_level_alarm;  //
	char no14c_void_p_tank_bilge_high_level_alarm;  //
	char no14c_void_s_tank_bilge_high_level_alarm;  //
    char bilge_tank_high_level_alarm;
	char lube_oil_tank_low_level_alarm;
    char fuel_oil_store_tank_9_p_high_level_alarm;
    char fuel_oil_store_tank_16_p_high_level_alarm;
	char fuel_oil_store_tank_9_s_high_level_alarm;
    char fuel_oil_store_tank_16_s_high_level_alarm;
	char no13_void_p_tank_bilge_high_level_alarm;  //
	char no13_void_s_tank_bilge_high_level_alarm;  //
	char pump_room_c_bilge_high_level_alarm_2;

#endif
	char device_offline;
}level_switch;

//气体探测报警系统
typedef struct{
	char gas_detection_alarm_system_failure;
	char gas_detector_fault_top_of_wheelhouse;
	char gas_pre_alarm_top_of_wheelhouse;
	char gas_level1_alarm_top_of_wheelhouse;
	char gas_level2_alarm_top_of_wheelhouse;
	char gas_detector_disabled_top_of_wheelhouse;
	char gas_detector_fault_air_intake_of_b_deck_ahu;
	char gas_pre_alarm_air_intake_of_b_deck_ahu;
	char gas_level1_alarm_air_intake_of_b_deck_ahu;
	char gas_level2_alarm_air_intake_of_b_deck_ahu;
	char gas_detector_disabled_air_intake_of_b_deck_ahu;
	char gas_detector_fault_air_intake_of_a_deck_ahu;
	char gas_pre_alarm_air_intake_of_a_deck_ahu;
	char gas_level1_alarm_air_intake_of_a_deck_ahu;
	char gas_level2_alarm_air_intake_of_a_deck_ahu;
	char gas_detector_disabled_air_intake_of_a_deck_ahu;
	char gas_detector_fault_air_intake_of_below_main_deck_ahu;
	char gas_pre_alarm_air_intake_of_below_main_deck_ahu;
	char gas_level1_alarm_air_intake_of_below_main_deck_ahu;
	char gas_level2_alarm_air_intake_of_below_main_deck_ahu;
	char gas_detector_disabled_air_intake_of_below_main_deck_ahu;
	char gas_detector_fault_air_intake_of_bow_thruster_room;
	char gas_pre_alarm_air_intake_of_bow_thruster_room;
	char gas_level1_alarm_air_intake_of_bow_thruster_room;
	char gas_level2_alarm_air_intake_of_bow_thruster_room;
	char gas_detector_disabled_air_intake_of_bow_thruster_room;
	char gas_detector_fault_air_intake_of_aft_thruster_roomp;
	char gas_pre_alarm_air_intake_of_aft_thruster_roomp;
	char gas_level1_alarm_air_intake_of_aft_thruster_roomp;
	char gas_level2_alarm_air_intake_of_aft_thruster_roomp;
	char gas_detector_disabled_air_intake_of_aft_thruster_roomp;
	char gas_detector_fault_air_intake_of_aft_thruster_rooms;
	char gas_pre_alarm_air_intake_of_aft_thruster_rooms;
	char gas_level1_alarm_air_intake_of_aft_thruster_rooms;
	char gas_level2_alarm_air_intake_of_aft_thruster_rooms;
	char gas_detector_disabled_air_intake_of_aft_thruster_rooms;
	char gas_detector_fault_air_intake_of_engine_roomp;
	char gas_pre_alarm_air_intake_of_engine_roomp;
	char gas_level1_alarm_air_intake_of_engine_roomp;
	char gas_level2_alarm_air_intake_of_engine_roomp;
	char gas_detector_disabled_air_intake_of_engine_roomp;
	char gas_detector_fault_air_intake_of_engine_rooms;
	char gas_pre_alarm_air_intake_of_engine_rooms;
	char gas_level1_alarm_air_intake_of_engine_rooms;
	char gas_level2_alarm_air_intake_of_engine_rooms;
	char gas_detector_disabled_air_intake_of_engine_rooms;
	char gas_detector_fault_air_intake_of_aux_machinery_room1;
	char gas_pre_alarm_air_intake_of_aux_machinery_room1;
	char gas_level1_alarm_air_intake_of_aux_machinery_room1;
	char gas_level2_alarm_air_intake_of_aux_machinery_room1;
	char gas_detector_disabled_air_intake_of_aux_machinery_room1;
	char gas_detector_fault_air_intake_of_aux_machinery_room2;
	char gas_pre_alarm_air_intake_of_aux_machinery_room2;
	char gas_level1_alarm_air_intake_of_aux_machinery_room2;
	char gas_level2_alarm_air_intake_of_aux_machinery_room2;
	char gas_detector_disabled_air_intake_of_aux_machinery_room2;
	char gas_detector_fault_b_deck_entrancefr109p;
	char gas_pre_alarm_b_deck_entrancefr109p;
	char gas_level1_alarm_b_deck_entrancefr109p;
	char gas_level2_alarm_b_deck_entrancefr109p;
	char gas_detector_disabled_b_deck_entrancefr109p;
	char gas_detector_fault_b_deck_entrancefr109s;
	char gas_pre_alarm_b_deck_entrancefr109s;
	char gas_level1_alarm_b_deck_entrancefr109s;
	char gas_level2_alarm_b_deck_entrancefr109s;
	char gas_detector_disabled_b_deck_entrancefr109s;
	char gas_detector_fault_wheelhouse_entrancep;
	char gas_pre_alarm_wheelhouse_entrancep;
	char gas_level1_alarm_wheelhouse_entrancep;
	char gas_level2_alarm_wheelhouse_entrancep;
	char gas_detector_disabled_wheelhouse_entrancep;
	char gas_detector_fault_wheelhouse_entrances;
	char gas_pre_alarm_wheelhouse_entrances;
	char gas_level1_alarm_wheelhouse_entrances;
	char gas_level2_alarm_wheelhouse_entrances;
	char gas_detector_disabled_wheelhouse_entrances;
#ifdef JANA505	
	char gas_detector_fault_b_deck_entrancefr90p;
	char gas_pre_alarm_b_deck_entrancefr90p;
	char gas_level1_alarm_b_deck_entrancefr90p;
	char gas_level2_alarm_b_deck_entrancefr90p;
	char gas_detector_disabled_b_deck_entrancefr90p;
	char gas_detector_fault_b_deck_entrancefr90s;
	char gas_pre_alarm_b_deck_entrancefr90s;
	char gas_level1_alarm_b_deck_entrancefr90s;
	char gas_level2_alarm_b_deck_entrancefr90s;
	char gas_detector_disabled_b_deck_entrancefr90s;
#endif
#ifdef JANA508509
	char gas_detector_fault_b_deck_entrancefr100p;
	char gas_pre_alarm_b_deck_entrancefr100p;
	char gas_level1_alarm_b_deck_entrancefr100p;
	char gas_level2_alarm_b_deck_entrancefr100p;
	char gas_detector_disabled_b_deck_entrancefr100p;
	char gas_detector_fault_b_deck_entrancefr100s;
	char gas_pre_alarm_b_deck_entrancefr100s;
	char gas_level1_alarm_b_deck_entrancefr100s;
	char gas_level2_alarm_b_deck_entrancefr100s;
	char gas_detector_disabled_b_deck_entrancefr100s;
#endif
	char gas_detector_fault_a_deck_entrancefr109p;
	char gas_pre_alarm_a_deck_entrancefr109p;
	char gas_level1_alarm_a_deck_entrancefr109p;
	char gas_level2_alarm_a_deck_entrancefr109p;
	char gas_detector_disabled_a_deck_entrancefr109p;
	char gas_detector_fault_a_deck_entrancefr109s;
	char gas_pre_alarm_a_deck_entrancefr109s;
	char gas_level1_alarm_a_deck_entrancefr109s;
	char gas_level2_alarm_a_deck_entrancefr109s;
	char gas_detector_disabled_a_deck_entrancefr109s;
#ifdef JANA505	
	char gas_detector_fault_a_deck_entrancefr90p;
	char gas_pre_alarm_a_deck_entrancefr90p;
	char gas_level1_alarm_a_deck_entrancefr90p;
	char gas_level2_alarm_a_deck_entrancefr90p;
	char gas_detector_disabled_a_deck_entrancefr90p;
	char gas_detector_fault_a_deck_entrancefr90s;
	char gas_pre_alarm_a_deck_entrancefr90s;
	char gas_level1_alarm_a_deck_entrancefr90s;
	char gas_level2_alarm_a_deck_entrancefr90sp;
	char gas_detector_disabled_a_deck_entrancefr90s;
	char gas_detector_fault_main_deck_entrancefr81m;
	char gas_pre_alarm_main_deck_entrancefr81m;
	char gas_level1_alarm_main_deck_entrancefr81m;
	char gas_level2_alarm_main_deck_entrancefr81m;
	char gas_detector_disabled_main_deck_entrancefr81m;
	char gas_detector_fault_main_deck_entrancefr81s;
	char gas_pre_alarm_main_deck_entrancefr81s;
	char gas_level1_alarm_main_deck_entrancefr81s;
	char gas_level2_alarm_main_deck_entrancefr81s;
	char gas_detector_disabled_main_deck_entrancefr81s;
	char gas_detector_fault_air_intake_of_construction_equipment_room;
	char gas_pre_alarm_air_intake_of_construction_equipment_room;
	char gas_level1_alarm_air_intake_of_construction_equipment_room;
	char gas_level2_alarm_air_intake_of_construction_equipment_room;
	char gas_detector_disabled_air_intake_of_construction_equipment_room;
#endif
#ifdef JANA508509
	char gas_detector_fault_a_deck_entrancefr91p;
	char gas_pre_alarm_a_deck_entrancefr91p;
	char gas_level1_alarm_a_deck_entrancefr91p;
	char gas_level2_alarm_a_deck_entrancefr91p;
	char gas_detector_disabled_a_deck_entrancefr91p;
	char gas_detector_fault_a_deck_entrancefr91s;
	char gas_pre_alarm_a_deck_entrancefr91s;
	char gas_level1_alarm_a_deck_entrancefr91s;
	char gas_level2_alarm_a_deck_entrancefr91sp;
	char gas_detector_disabled_a_deck_entrancefr91s;
	char gas_detector_fault_main_deck_entrancefr91m;
	char gas_pre_alarm_main_deck_entrancefr91m;
	char gas_level1_alarm_main_deck_entrancefr91m;
	char gas_level2_alarm_main_deck_entrancefr91m;
	char gas_detector_disabled_main_deck_entrancefr91m;
	char gas_detector_fault_main_deck_entrancefr91s;
	char gas_pre_alarm_main_deck_entrancefr91s;
	char gas_level1_alarm_main_deck_entrancefr91s;
	char gas_level2_alarm_main_deck_entrancefr91s;
	char gas_detector_disabled_main_deck_entrancefr91s;
#endif
#ifdef MAHTS648
    char gas_detector_fault_level_01_deck_entrance_fr_6p;
	char gas_detector_pre_alarm_level_01_deck_entrance_fr_6p;
	char gas_detector_level_1_alarm_level_01_deck_entrance_fr_6p;
	char gas_detector_level_2_alarm_level_01_deck_entrance_fr_6p;
	char gas_detector_disabled_level_01_deck_entrance_fr_6p;
	char gas_detector_fault_level_01_deck_entrance_6s;
	char gas_detector_pre_alarm_level_01_deck_entrance_6s;
	char gas_detector_level_1_alarm_level_01_deck_entrance_6s;
	char gas_detector_level_2_alarm_level_01_deck_entrance_6s;
	char gas_detector_disabled_level_01_deck_entrance_fr_6s;
	char gas_detector_fault_air_intake_of_level_01_main_deck_ahu;
	char gas_detector_pre_alarm_air_intake_of_level_01_main_deck_ahu;
	char gas_detector_level_1_air_intake_of_level_01_main_deck_ahu;
	char gas_detector_level_2_air_intake_of_level_01_main_deck_ahu;
	char gas_detector_disable_air_intake_of_level_01_main_deck_ahu;
    char gas_detector_fault_fr_7p;
	char gas_detector_pre_alarm_fr_7p;
	char gas_detector_level_1_alarm_fr_7p;
	char gas_detector_level_2_alarm_fr_7p;
	char gas_detector_disabled_fr_7p;
    char gas_detector_fault_fr_7s;
	char gas_detector_pre_alarm_fr_7s;
	char gas_detector_level_1_alarm_fr_7s;
	char gas_detector_level_2_alarm_fr_7s;
	char gas_detector_disabled_fr_7s;
	char gas_detector_fault_fr_13p;
	char gas_detector_pre_alarm_fr_13p;
	char gas_detector_level_1_alarm_fr_13p;
	char gas_detector_level_2_alarm_fr_13p;
	char gas_detector_disabled_fr_13p;
    char gas_detector_fault_fr_13s;
	char gas_detector_pre_alarm_fr_13s;
	char gas_detector_level_1_alarm_fr_13s;
	char gas_detector_level_2_alarm_fr_13s;
	char gas_detector_disabled_fr_13s;
	char gas_detector_fault_level_01_deck_entrance_fr_14p;
	char gas_detector_pre_alarm_level_01_deck_entrance_fr_14p;
	char gas_detector_level_1_alarm_level_01_deck_entrance_fr_14p;
	char gas_detector_level_2_alarm_level_01_deck_entrance_fr_14p;
	char gas_detector_disabled_level_01_deck_entrance_fr_14p;
	char gas_detector_fault_fr_16s;
	char gas_detector_pre_alarm_fr_16s;
	char gas_detector_level_1_alarm_fr_16s;
	char gas_detector_level_2_alarm_fr_16s;
	char gas_detector_disabled_fr_16s;

    char gas_detector_fault_main_deck_entrance_fr_6p;
	char gas_detector_pre_alarm_main_deck_entrance_fr_6p;
	char gas_detector_level_1_alarm_main_deck_entrance_fr_6p;
	char gas_detector_level_2_alarm_main_deck_entrance_fr_6p;
	char gas_detector_disabled_main_deck_entrance_fr_6p;
	char gas_detector_fault_main_deck_entrance_6s;
	char gas_detector_pre_alarm_main_deck_entrance_6s;
	char gas_detector_level_1_alarm_main_deck_entrance_6s;
	char gas_detector_level_2_alarm_main_deck_entrance_6s;
	char gas_detector_disabled_main_deck_entrance_fr_6s;

	 char gas_detector_fault_fr_8p;
	char gas_detector_pre_alarm_fr_8p;
	char gas_detector_level_1_alarm_fr_8p;
	char gas_detector_level_2_alarm_fr_8p;
	char gas_detector_disabled_fr_8p;
    char gas_detector_fault_fr_8s;
	char gas_detector_pre_alarm_fr_8s;
	char gas_detector_level_1_alarm_fr_8s;
	char gas_detector_level_2_alarm_fr_8s;
	char gas_detector_disabled_fr_8s;

	char gas_detector_fault_main_deck_entrance_fr_14p;
	char gas_detector_pre_alarm_main_deck_entrance_fr_14p;
	char gas_detector_level_1_alarm_main_deck_entrance_fr_14p;
	char gas_detector_level_2_alarm_main_deck_entrance_fr_14p;
	char gas_detector_disabled_main_deck_entrance_fr_14p;

	char gas_detector_fault_main_deck_entrance_fr_14s;
	char gas_detector_pre_alarm_main_deck_entrance_fr_14s;
	char gas_detector_level_1_alarm_main_deck_entrance_fr_14s;
	char gas_detector_level_2_alarm_main_deck_entrance_fr_14s;
	char gas_detector_disabled_main_deck_entrance_fr_14s;

    char gas_detector_fault_main_deck_entrance_fr_16m;
	char gas_detector_pre_alarm_main_deck_entrance_fr_16m;
	char gas_detector_level_1_alarm_main_deck_entrance_fr_16m;
	char gas_detector_level_2_alarm_main_deck_entrance_fr_16m;
	char gas_detector_disabled_main_deck_entrance_fr_16m;

	char gas_detector_fault_sick_bay_entrance;
	char gas_detector_pre_alarm_sick_bay_entrance;
	char gas_detector_level_1_alarm_sick_bay_entrance;
	char gas_detector_level_2_alarm_main_sick_bay_entrance;
	char gas_detector_disabled_sick_bay_entrance;

	char gas_detector_fault_coffee_lounge_entrance;
	char gas_detector_pre_alarm_coffee_lounge_entrance;
	char gas_detector_level_1_alarm_coffee_lounge_entrance;
	char gas_detector_level_2_alarm_main_coffee_lounge_entrance;
	char gas_detector_disabled_coffee_lounge_entrance;

	char toxic_gas_detector_fault_level_01_deck_entrance_fr_6p;
	char toxic_gas_detector_pre_alarm_level_01_deck_entrance_fr_6p;
	char toxic_gas_detector_level_1_alarm_level_01_deck_entrance_fr_6p;
	char toxic_gas_detector_level_2_alarm_level_01_deck_entrance_fr_6p;
	char toxic_gas_detector_disabled_level_01_deck_entrance_fr_6p;
	char toxic_gas_detector_fault_level_01_deck_entrance_6s;
	char toxic_gas_detector_pre_alarm_level_01_deck_entrance_6s;
	char toxic_gas_detector_level_1_alarm_level_01_deck_entrance_6s;
	char toxic_gas_detector_level_2_alarm_level_01_deck_entrance_6s;
	char toxic_gas_detector_disabled_level_01_deck_entrance_fr_6s;

	char toxic_gas_detector_fault_air_intake_of_level_01_main_deck_ahu;
	char toxic_gas_detector_pre_alarm_air_intake_of_level_01_main_deck_ahu;
	char toxic_gas_detector_level_1_air_intake_of_level_01_main_deck_ahu;
	char toxic_gas_detector_level_2_air_intake_of_level_01_main_deck_ahu;
	char toxic_gas_detector_disable_air_intake_of_level_01_main_deck_ahu;

    char co2_power_failure_alarm;
    char co2_pre_release_alarm;
    char co2_release_alarm;
    char co2_leakage_alarm;

#endif
	char toxic_gas_detector_fault_top_of_wheelhouse;
	char toxic_gas_pre_alarm_top_of_wheelhouse;
	char toxic_gas_level1_alarm_top_of_wheelhouse;
	char toxic_gas_level2_alarm_top_of_wheelhouse;
	char toxic_gas_detector_disabled_top_of_wheelhouse;
	char toxic_gas_detector_fault_air_intake_of_b_deck_ahu;
	char toxic_gas_pre_alarm_air_intake_of_b_deck_ahu;
	char toxic_gas_level1_alarm_air_intake_of_b_deck_ahu;
	char toxic_gas_level2_alarm_air_intake_of_b_deck_ahu;
	char toxic_gas_detector_disabled_air_intake_of_b_deck_ahu;
	char toxic_gas_detector_fault_air_intake_of_a_deck_ahu;
	char toxic_gas_pre_alarm_air_intake_of_a_deck_ahu;
	char toxic_gas_level1_alarm_air_intake_of_a_deck_ahu;
	char toxic_gas_level2_alarm_air_intake_of_a_deck_ahu;
	char toxic_gas_detector_disabled_air_intake_of_a_deck_ahu;
	char toxic_gas_detector_fault_air_intake_of_below_main_deck_ahu;
	char toxic_gas_pre_alarm_air_intake_of_below_main_deck_ahu;
	char toxic_gas_level1_alarm_air_intake_of_below_main_deck_ahu;
	char toxic_gas_level2_alarm_air_intake_of_below_main_deck_ahu;
	char toxic_gas_detector_disabled_air_intake_of_below_main_deck_ahu;
	char toxic_gas_detector_fault_air_intake_of_bow_thruster_room;
	char toxic_gas_pre_alarm_air_intake_of_bow_thruster_room;
	char toxic_gas_level1_alarm_air_intake_of_bow_thruster_room;
	char toxic_gas_level2_alarm_air_intake_of_bow_thruster_room;
	char toxic_gas_detector_disabled_air_intake_of_bow_thruster_room;

	char toxic_gas_detector_fault_air_intake_of_aft_thruster_roomp;
	char toxic_gas_pre_alarm_air_intake_of_aft_thruster_roomp;
	char toxic_gas_level1_alarm_air_intake_of_aft_thruster_roomp;
	char toxic_gas_level2_alarm_air_intake_of_aft_thruster_roomp;
	char toxic_gas_detector_disabled_air_intake_of_aft_thruster_roomp;
	char toxic_gas_detector_fault_air_intake_of_aft_thruster_rooms;
	char toxic_gas_pre_alarm_air_intake_of_aft_thruster_rooms;
	char toxic_gas_level1_alarm_air_intake_of_aft_thruster_rooms;
	char toxic_gas_level2_alarm_air_intake_of_aft_thruster_rooms;
	char toxic_gas_detector_disabled_air_intake_of_aft_thruster_rooms;

	char toxic_gas_detector_fault_air_intake_of_engine_roomp;
	char toxic_gas_pre_alarm_air_intake_of_engine_roomp;
	char toxic_gas_level1_alarm_air_intake_of_engine_roomp;
	char toxic_gas_level2_alarm_air_intake_of_engine_roomp;
	char toxic_gas_detector_disabled_air_intake_of_engine_roomp;

	char toxic_gas_detector_fault_air_intake_of_engine_rooms;
	char toxic_gas_pre_alarm_air_intake_of_engine_rooms;
	char toxic_gas_level1_alarm_air_intake_of_engine_rooms;
	char toxic_gas_level2_alarm_air_intake_of_engine_rooms;
	char toxic_gas_detector_disabled_air_intake_of_engine_rooms;

	char toxic_gas_detector_fault_air_intake_of_aux_machinery_room1;
	char toxic_gas_pre_alarm_air_intake_of_aux_machinery_room1;
	char toxic_gas_level1_alarm_air_intake_of_aux_machinery_room1;
	char toxic_gas_level2_alarm_air_intake_of_aux_machinery_room1;
	char toxic_gas_detector_disabled_air_intake_of_aux_machinery_room1;
	char toxic_gas_detector_fault_air_intake_of_aux_machinery_room2;
	char toxic_gas_pre_alarm_air_intake_of_aux_machinery_room2;
	char toxic_gas_level1_alarm_air_intake_of_aux_machinery_room2;
	char toxic_gas_level2_alarm_air_intake_of_aux_machinery_room2;
	char toxic_gas_detector_disabled_air_intake_of_aux_machinery_room2;
	char toxic_gas_detector_fault_b_deck_entrancefr109p;
	char toxic_gas_pre_alarm_b_deck_entrancefr109p;
	char toxic_gas_level1_alarm_b_deck_entrancefr109p;
	char toxic_gas_level2_alarm_b_deck_entrancefr109p;
	char toxic_gas_detector_disabled_b_deck_entrancefr109p;
	char toxic_gas_detector_fault_b_deck_entrancefr109s;
	char toxic_gas_pre_alarm_b_deck_entrancefr109s;
	char toxic_gas_level1_alarm_b_deck_entrancefr109s;
	char toxic_gas_level2_alarm_b_deck_entrancefr109s;
	char toxic_gas_detector_disabled_b_deck_entrancefr109s;
	char device_offline;
}gas_detection_system;

//其他
typedef struct{
	char fire_alarm_control_unit_system_failure;
	char fire_alarm;
	char sewage_water_treatment_plant_common_alarm;
	char fuel_oil_purifier_common_alarm;
	char oil_water_separator_common_alarm;
	char oil_water_separator15ppm_alarm;
	char oil_water_separator_running_timeout_alarm_nc;
	char no1_fresh_water_generator_common_alarm;
	char no2_fresh_water_generator_common_alarm;
	char fresh_water_generator_no1_boost_pump_overload;
	char fresh_water_generator_no2_boost_pump_overload;
	char water_sprinkler_system_common_alarm;
	char no1_submersible_pump_failure;
	char no1_submersible_pump_running;
	char no2_submersible_pump_failure;
	char no2_submersible_pump_running;
	char no3_submersible_pump_failure;
	char no3_submersible_pump_running;
	char no4_submersible_pump_failure;
	char no4_submersible_pump_running;
	char ac440v_galley_equipment_db_low_insulation;
	char ac230v_galley_equipment_db_low_insulation;
	char co2_release_alarm_system_power_failure_alarm;
	char co2_leakage_alarm;
	char co2_release_alarm;
	char e_r_co2_release_pre_alarm;
	char e_r_co2_release_valve_opened;
	char paint_room_co2_release_pre_alarm;
	char paint_room_co2_release_valve_opened;
	char no1_anti_fouling_device_common_alarm;
	char no2_anti_fouling_device_common_alarm;
	char uv_sterilizer_common_alarm;
	char navigation_aids_system_common_alarm;
	char public_address_general_alarm_system_fault;
	char public_address_general_alarm_system_power_fault;
	char auto_telephone_system_failure;
	char audio_visual_alarm_column_main_power_failure;
	char audio_visual_alarm_column_backup_power_failure;
	char no1_leg_control_unit_common_alarm;
	char no2_leg_control_unit_common_alarm;
	char no3_leg_control_unit_common_alarm;
	char no4_leg_control_unit_common_alarm;
	char fog_horn_controller_power_failure;
	char bnwas_main_unit_power_failure;
	char fresh_water_hydrophore_failure;
	char sea_water_hydrophore_failure;
	char calorifier_failure;
	char spare;
	char no1_ups_for_ams_power_failure;
	char no2_ups_for_ams_power_failure;
	char no3_ups_for_ams_power_failure;
	char no1_crane_fire_alarm;
	char no2_crane_fire_alarm;
	char engineer_alarm_system_power_failure;
	char watertight_door_indication_system_common_alarm;
#ifdef MAHTS648
    char no1_watertight_door_common_alarm;
    char no2_watertight_door_common_alarm;
    char no3_watertight_door_common_alarm;
    char no4_watertight_door_common_alarm;
    char fuel_oil_purifier_running;
    char fuel_oil_purifier_abnormal_alarm;
    char oily_water_separator_oil_content_high;
    char oily_water_separator_common_alarm;
    char ele_heating_calorifier_heater_running;
    char ele_heating_calorifier_hot_water_circulating_pump_running;
    char ele_heating_calorifier_high_temp_alarm;
    char fresh_water_maker_common_alarm;
    char fifi_system_common_alarm;
    char sw_fw_fress_set_common_alarm;
    char miscellaneousbffdc0c37da000f4;
    char electrical_valve_remote_control_system_common_alarm;
    char a_c_common_alarm;
    char provision_refrigeration_system_fault;
    char freezer_room_high_temp_alarm;
    char chiller_room_high_temp_alarm;
    char deck_machine_hpu_packdeck_machine_hpu_pack_low_oil_level_alarm;
    char miscellaneous49510c947aae3742;
    char deck_machine_hpu_packdeck_machine_hpu_pack_high_oil_temp_alarm;
    char miscellaneousb8c79c2fcc0eddd2;
    char mud_agitator_system_common_alarm;
    char bnwas_system_failure;
    char no1_gyro_ac220v_power_failure_alarm;
    char no2_gyro_ac220v_power_failure_alarm;
    char no3_gyro_ac220v_power_failure_alarm;
    char autopilot_system_failure_alarm;
    char gyro_compass_distribution_unit_system_failure;
    char gyro_compass_distribution_unit_gyro1_failure;
    char gyro_compass_distribution_unit_gyro2_failure;
    char gyro_compass_distribution_unit_gyro3_failure;
    char gyro_compass_distribution_unit_tmc_failure;
    char gyro_compass_distribution_unit_can_bus_failure;
    char gyro_compass_distribution_unit_sensor_diff_alarm;
    char gyro_compass_distribution_unit_heading_monitor_alarm;
    char watertight_door_or_hatch_cover_power_failure;
    char fire_take_place_signal;
    char fire_alarm_system_failure;//将fire_system_failure_alarm改为fire_alarm_system_failure
    char from_pa_system_power_fault;
    char from_ga_system_power_fault;
    char general_service_air_compressor_common_alarm;
    char alarm_column_machinery_alarm;
    char alarm_column_tank_level_alarm;
    char alarm_column_relay_box_power_fault;
    char engine_call_system_power_fault;
    char mian_propulsion_order_telegraph_p_s_power_fault;
    char refri_call_main_unit_power_fault;
    char hospital_call_main_unit_power_fault;
    char watermist_main_control_boxecr_system_failure;
    char ams_ups1_common_alarm;
    char ams_ups2_common_alarm;
    char control_box_loss_of_power;
    char purifier_motor_overload;
    char purifier_outlet_pressure_high;
    char no1_gyro_dc24v_power_failure_alarm;
    char no2_gyro_dc24v_power_failure_alarm;
    char no3_gyro_dc24v_power_failure_alarm;
    char autopilot_off_heading_alarm;
    char watertight_door_or_hatch_cover_open_alarm;
#endif

	float freshwater_filling_flowmeter;
	char device_offline;
}miscellaneous;

typedef struct{
	value_type fire_alarm_control_unit_system_failure;
	value_type fire_alarm;
	value_type sewage_water_treatment_plant_common_alarm;
	value_type fuel_oil_purifier_common_alarm;
	value_type oil_water_separator_common_alarm;
	value_type oil_water_separator15ppm_alarm;
	value_type oil_water_separator_running_timeout_alarm_nc;
	value_type no1_fresh_water_generator_common_alarm;
	value_type no2_fresh_water_generator_common_alarm;
	value_type fresh_water_generator_no1_boost_pump_overload;
	value_type fresh_water_generator_no2_boost_pump_overload;
	value_type water_sprinkler_system_common_alarm;
	value_type no1_submersible_pump_failure;
	value_type no1_submersible_pump_running;
	value_type no2_submersible_pump_failure;
	value_type no2_submersible_pump_running;
	value_type no3_submersible_pump_failure;
	value_type no3_submersible_pump_running;
	value_type no4_submersible_pump_failure;
	value_type no4_submersible_pump_running;
	value_type ac440v_galley_equipment_db_low_insulation;
	value_type ac230v_galley_equipment_db_low_insulation;
	value_type co2_release_alarm_system_power_failure_alarm;
	value_type co2_leakage_alarm;
	value_type co2_release_alarm;
	value_type e_r_co2_release_pre_alarm;
	value_type e_r_co2_release_valve_opened;
	value_type paint_room_co2_release_pre_alarm;
	value_type paint_room_co2_release_valve_opened;
	value_type no1_anti_fouling_device_common_alarm;
	value_type no2_anti_fouling_device_common_alarm;
	value_type uv_sterilizer_common_alarm;
	value_type navigation_aids_system_common_alarm;
	value_type public_address_general_alarm_system_fault;
	value_type public_address_general_alarm_system_power_fault;
	value_type auto_telephone_system_failure;
	value_type audio_visual_alarm_column_main_power_failure;
	value_type audio_visual_alarm_column_backup_power_failure;
	value_type no1_leg_control_unit_common_alarm;
	value_type no2_leg_control_unit_common_alarm;
	value_type no3_leg_control_unit_common_alarm;
	value_type no4_leg_control_unit_common_alarm;
	value_type fog_horn_controller_power_failure;
	value_type bnwas_main_unit_power_failure;
	value_type fresh_water_hydrophore_failure;
	value_type sea_water_hydrophore_failure;
	value_type calorifier_failure;
	value_type spare;
	value_type no1_ups_for_ams_power_failure;
	value_type no2_ups_for_ams_power_failure;
	value_type no3_ups_for_ams_power_failure;
	value_type no1_crane_fire_alarm;
	value_type no2_crane_fire_alarm;
	value_type engineer_alarm_system_power_failure;
	value_type watertight_door_indication_system_common_alarm;
#ifdef MAHTS648
    value_type no4_ups_for_ams_power_failure;
	value_type fuel_oil_filling_flowmeter;
#endif

	value_type freshwater_filling_flowmeter;
	value_type device_offline;
}miscellaneous_store;

typedef struct{
	char co2_release_alarm_system_power_failure_alarm;
	char co2_leakage_alarm;
	char co2_release_alarm;
	char e_r_co2_release_pre_alarm;
	char e_r_co2_release_valve_opened;
	char paint_room_co2_release_pre_alarm;
	char paint_room_co2_release_valve_opened;

	char device_offline;
}co2_system;

typedef struct{

	value_type co2_release_alarm_system_power_failure_alarm;
	value_type co2_leakage_alarm;
	value_type co2_release_alarm;
	value_type e_r_co2_release_pre_alarm;
	value_type e_r_co2_release_valve_opened;
	value_type paint_room_co2_release_pre_alarm;
	value_type paint_room_co2_release_valve_opened;
	value_type device_offline;
}co2_system_store;

//冷却水系统
typedef struct{
#ifdef MAHTS648
    float no1_propulsion_fw_cooling_pump_outlet_temperature;
	float no2_propulsion_fw_cooling_pump_outlet_temperature;
	float no1_propulsion_fw_cooling_plant_inlet_temperature;
	float no2_propulsion_fw_cooling_plant_inlet_temperature;
	float no1_propulsion_motor_fw_cooling_outlet_temperature;
	float no1_propulsion_vfd_fw_cooling_outlet_temperature;
	float no2_propulsion_motor_fw_cooling_outlet_temperature;
	float no2_propulsion_vfd_fw_cooling_outlet_temperature;
	float no3_propulsion_motor_fw_cooling_outlet_temperature;
	float no3_propulsion_vfd_fw_cooling_outlet_temperature;
	float no4_propulsion_motor_fw_cooling_outlet_temperature;
	float no4_propulsion_vfd_fw_cooling_outlet_temperature;
	float no5_propulsion_motor_fw_cooling_outlet_temperature;
	float no5_propulsion_vfd_fw_cooling_outlet_temperature;
	float no6_propulsion_motor_fw_cooling_outlet_temperature;
	float no6_propulsion_vfd_fw_cooling_outlet_temperature;
	float main_sea_water_line_pressure_p;
    float main_sea_water_line_pressure_s;
	float no1_bilge_water_pump_inlet_pressure;
	float no1_bilge_water_pump_outlet_pressure;
	float no2_bilge_water_pump_inlet_pressure;
	float no2_bilge_water_pump_outlet_pressure;
	float no1_ballast_water_pump_inlet_pressure;
	float no2_ballast_water_pump_inlet_pressure;	
	float no1_fire_water_pump_outlet_pressure;
	float no2_fire_water_pump_outlet_pressure;
	float jokcy_pump_outlet_pressure;
	float jokcy_pump_inlet_pressure;
	float fire_main_line_pressure_p;
	float fire_main_line_pressure_s;
	float no1_fresh_water_transfer_pump_outlet_pressure;
	float no2_fresh_water_transfer_pump_outlet_pressure;
	float no1_fire_water_pump_inlet_pressure;
	float no2_fire_water_pump_inlet_pressure;
#endif
	float no1_propulsion_sw_cooling_pump_outlet_pressure;
	float no2_propulsion_sw_cooling_pump_outlet_pressure;
	float no1_propulsion_fw_cooling_pump_outlet_pressure;
	float no2_propulsion_fw_cooling_pump_outlet_pressure;
	char device_offline;
}cooling_water_system;

typedef struct{
#ifdef MAHTS648
    value_type no1_propulsion_fw_cooling_pump_outlet_temperature;
	value_type no2_propulsion_fw_cooling_pump_outlet_temperature;
	value_type no1_propulsion_fw_cooling_plant_inlet_temperature;
	value_type no2_propulsion_fw_cooling_plant_inlet_temperature;
	value_type no1_propulsion_motor_fw_cooling_outlet_temperature;
	value_type no1_propulsion_vfd_fw_cooling_outlet_temperature;
	value_type no2_propulsion_motor_fw_cooling_outlet_temperature;
	value_type no2_propulsion_vfd_fw_cooling_outlet_temperature;
	value_type no3_propulsion_motor_fw_cooling_outlet_temperature;
	value_type no3_propulsion_vfd_fw_cooling_outlet_temperature;
	value_type no4_propulsion_motor_fw_cooling_outlet_temperature;
	value_type no4_propulsion_vfd_fw_cooling_outlet_temperature;
	value_type no5_propulsion_motor_fw_cooling_outlet_temperature;
	value_type no5_propulsion_vfd_fw_cooling_outlet_temperature;
	value_type no6_propulsion_motor_fw_cooling_outlet_temperature;
	value_type no6_propulsion_vfd_fw_cooling_outlet_temperature;
	value_type main_sea_water_line_pressure_p;
    value_type main_sea_water_line_pressure_s;
	value_type no1_bilge_water_pump_inlet_pressure;
	value_type no1_bilge_water_pump_outlet_pressure;
	value_type no2_bilge_water_pump_inlet_pressure;
	value_type no2_bilge_water_pump_outlet_pressure;
	value_type no1_ballast_water_pump_inlet_pressure;
	value_type no2_ballast_water_pump_inlet_pressure;	
	value_type no1_fire_water_pump_outlet_pressure;
	value_type no2_fire_water_pump_outlet_pressure;
	value_type jokcy_pump_outlet_pressure;
	value_type jokcy_pump_inlet_pressure;
	value_type fire_main_line_pressure_p;
	value_type fire_main_line_pressure_s;
	value_type no1_fresh_water_transfer_pump_outlet_pressure;
	value_type no2_fresh_water_transfer_pump_outlet_pressure;
	value_type no1_fire_water_pump_inlet_pressure;
	value_type no2_fire_water_pump_inlet_pressure;
#endif
	value_type no1_propulsion_sw_cooling_pump_outlet_pressure;
	value_type no2_propulsion_sw_cooling_pump_outlet_pressure;
	value_type no1_propulsion_fw_cooling_pump_outlet_pressure;
	value_type no2_propulsion_fw_cooling_pump_outlet_pressure;
	value_type device_offline;
}cooling_water_system_store;

typedef struct{
	std::mutex mux;
	float d_g_main_generator_u_winding_temperature;
	float d_g_main_generator_v_winding_temperature;
	float d_g_main_generator_w_winding_temperature;
	float d_g_main_generator_de_bearing_temperature;
	float d_g_main_generator_nde_bearing_temperature;
	float d_g_main_generator_excitation_current;
	float d_g_main_generator_excitation_voltage;
}shm_main_generator;

typedef struct{
	std::mutex mux;
	float eg_generator_u_winding_temperature;
	float eg_generator_v_winding_temperature;
	float eg_generator_w_winding_temperature;
}shm_eme_generator;



///////////////////////////////////AMS END/////////////////////////////////////////////////////////////////
/////流量计
typedef struct{
	float flow;
	float flow_rate;
	float temperature;
	float density;
	float bidirect_flow_signal;
	char device_offline;
}flowmeter;

//总流量计
typedef struct{
	float single_value;
	float total_value;
	float instantaneous_value;
	char status;//0:停止 1:加油中
	char device_offline;
}total_flowmeter;

#if 0
//////////gps
/*
UTC时间
经度
纬度
对地航向（真航向）
对地速度*/
typedef struct{
	std::string utc;
	std::string latitude;	
	std::string longitude;
	float SOG;
	float COG;
}gpsData;

//////////ais
/*临近目标数量
MMSI（目标）
IMO（目标）
船名（目标）
呼号（目标）
纬度（目标）
经度（目标）
SOG（目标）
COG（目标）*/

typedef struct{
	std::string MMSI;
	std::string IMO;
	std::string vessel_name;
	std::string call_sign;
	std::string latitude;
	std::string longitude;	
	float SOG;
	float COG;
	std::string datetime;
}aisData;

///gyro

/////////gyro艏向
//$HEHDT,x.x,a*hh<CR><LF>   $HEHDT,227.0,T*28
typedef struct{
	std::string heading;
}gyro;




//锚链
//modbus RTU 19200,8,N,1
//包头4bytes+Data1(4bytes)+0D,data1为缆绳张力值
typedef struct{
	float tension;
	char tension_status;
	char anchor_status;
	float anchor_len;
	std::string latitude;
	std::string longitude;
}anchorChain;

//ECDIS
typedef struct{
    std::string route_identifier;//路由标识符
    std::string waypoint_identifiers;//航路点标识符(s)
    std::string waypoint_identifier;//航路点标识符
    std::string latitude;//航路点纬度
    std::string longitude;//航路点经度
}ecdisData;

//Echo Sounder
typedef struct{
	float seabed_depth;
	float draft_depth;
	float measuring_range;
}echoSounder;

//Wind meter 风力计
typedef struct{
	float wind_direction;
	float wind_speed;
}windMeter;



//platform_jacking_system
typedef struct{
	char leg1_vfd1_running;
	char leg1_vfd2_running;
	char leg1_vfd3_running;
	char leg1_vfd4_running;
	char leg1_vfd5_running;
	char leg1_vfd6_running;
	char leg1_vfd7_running;
	char leg1_vfd8_running;
	char leg1_vfd9_running;
	char leg1_vfd10_running;
	char leg1_vfd11_running;
	char leg1_vfd12_running;
	char leg1_vfd13_running;
	char leg1_vfd14_running;
	char spare;
	char leg1_spare1;
	char leg1_spare2;
	char leg1_spare3;
	char leg1_spare4;
	char leg1_spare5;
	char leg1_spare6;
	char leg1_spare7;
	char leg1_spare8;
	char leg1_spare9;
	char leg1_spare10;
	char leg1_spare11;
	char leg1_spare12;
	char leg1_spare13;
	char leg1_spare14;
	char leg1_spare15;
	char leg1_spare16;
	char leg1_spare17;
	char leg1_spare18;
	char leg1_brake1_open;
	char leg1_brake2_open;
	char leg1_brake3_open;
	char leg1_brake4_open;
	char leg1_brake5_open;
	char leg1_brake6_open;
	char leg1_brake7_open;
	char leg1_brake8_open;
	char leg1_brake9_open;
	char leg1_brake10_open;
	char leg1_brake11_open;
	char leg1_brake12_open;
	char leg1_brake13_open;
	char leg1_brake14_open;
	char leg1_spare19;
	char leg1_spare20;
	char leg1_spare21;
	char leg1_spare22;
	char leg1_spare23;
	char leg1_spare24;
	char leg1_spare25;
	char leg1_spare26;
	char leg1_spare27;
	char leg1_spare28;
	char leg1_spare29;
	char leg1_spare30;
	char leg1_spare31;
	char leg1_spare32;
	char leg1_spare33;
	char leg1_spare34;
	char leg1_spare35;
	char leg1_spare36;
	char leg1_spare37;
	char leg1_spare38;
	char leg1_spare39;
	char leg1_spare40;
	char leg1_spare41;
	char leg1_spare42;
	char leg1_spare43;
	char leg1_spare44;
	char noral;
	char preload;
	char retorque;
	char rpd;
	char system_on;
	char ccs_control;
	char local_control;
	char low_speed;
	char rated_speed;
	char double_speed;
	char leg1_general_alarm;
	char leg1_raise;
	char leg1_lower;
	char leg1_touch_ground;
	char hull_stand;
	char hull_float;
	short leg1_pinion1_load;
	short leg1_pinion2_load;
	short leg1_pinion3_load;
	short leg1_pinion4_load;
	short leg1_pinion5_load;
	short leg1_pinion6_load;
	short leg1_pinion7_load;
	short leg1_pinion8_load;
	short leg1_pinion9_load;
	short leg1_pinion10_load;
	short leg1_pinion11_load;
	short leg1_pinion12_load;
	short leg1_pinion13_load;
	short leg1_pinion14_load;
	short leg1_vfd1_speed;
	short leg1_vfd2_speed;
	short leg1_vfd3_speed;
	short leg1_vfd4_speed;
	short leg1_vfd5_speed;
	short leg1_vfd6_speed;
	short leg1_vfd7_speed;
	short leg1_vfd8_speed;
	short leg1_vfd9_speed;
	short leg1_vfd10_speed;
	short leg1_vfd11_speed;
	short leg1_vfd12_speed;
	short leg1_vfd13_speed;
	short leg1_vfd14_speed;
	short leg1_power;
	short leg1_current_total;
	short leg1_load_total;
	float leg1_speed;
	float leg1_distance;
	float slope_angle;
	float x_slope;
	float y_slope;
	short hull_load;
	char leg2_vfd1_running;
	char leg2_vfd2_running;
	char leg2_vfd3_running;
	char leg2_vfd4_running;
	char leg2_vfd5_running;
	char leg2_vfd6_running;
	char leg2_vfd7_running;
	char leg2_vfd8_running;
	char leg2_vfd9_running;
	char leg2_vfd10_running;
	char leg2_vfd11_running;
	char leg2_vfd12_running;
	char leg2_vfd13_running;
	char leg2_vfd14_running;
	char leg2_spare1;
	char leg2_spare2;
	char leg2_spare3;
	char leg2_spare4;
	char leg2_spare5;
	char leg2_spare6;
	char leg2_spare7;
	char leg2_spare8;
	char leg2_spare9;
	char leg2_spare10;
	char leg2_spare11;
	char leg2_spare12;
	char leg2_spare13;
	char leg2_spare14;
	char leg2_spare15;
	char leg2_spare16;
	char leg2_spare17;
	char leg2_spare18;
	char leg2_brake1_open;
	char leg2_brake2_open;
	char leg2_brake3_open;
	char leg2_brake4_open;
	char leg2_brake5_open;
	char leg2_brake6_open;
	char leg2_brake7_open;
	char leg2_brake8_open;
	char leg2_brake9_open;
	char leg2_brake10_open;
	char leg2_brake11_open;
	char leg2_brake12_open;
	char leg2_brake13_open;
	char leg2_brake14_open;
	char leg2_spare19;
	char leg2_spare20;
	char leg2_spare21;
	char leg2_spare22;
	char leg2_spare23;
	char leg2_spare24;
	char leg2_spare25;
	char leg2_spare26;
	char leg2_spare27;
	char leg2_spare28;
	char leg2_spare29;
	char leg2_spare30;
	char leg2_spare31;
	char leg2_spare32;
	char leg2_spare33;
	char leg2_spare34;
	char leg2_spare35;
	char leg2_spare36;
	char leg2_spare37;
	char leg2_spare38;
	char leg2_spare39;
	char leg2_spare40;
	char leg2_spare41;
	char leg2_spare42;
	char leg2_spare43;
	char leg2_spare44;
	char leg2_spare45;
	char leg2_spare46;
	char leg2_spare47;
	char leg2_spare48;
	char leg2_spare49;
	char leg2_spare50;
	char leg2_spare51;
	char leg2_spare52;
	char leg2_spare53;
	char leg2_spare54;
	char leg2_general_alarm;
	char leg2_raise;
	char leg2_lower;
	char leg2_touch_ground;
	char leg2_spare55;
	char leg2_spare56;
	short leg2_pinion1_load;
	short leg2_pinion2_load;
	short leg2_pinion3_load;
	short leg2_pinion4_load;
	short leg2_pinion5_load;
	short leg2_pinion6_load;
	short leg2_pinion7_load;
	short leg2_pinion8_load;
	short leg2_pinion9_load;
	short leg2_pinion10_load;
	short leg2_pinion11_load;
	short leg2_pinion12_load;
	short leg2_pinion13_load;
	short leg2_pinion14_load;
	short leg2_vfd1_speed;
	short leg2_vfd2_speed;
	short leg2_vfd3_speed;
	short leg2_vfd4_speed;
	short leg2_vfd5_speed;
	short leg2_vfd6_speed;
	short leg2_vfd7_speed;
	short leg2_vfd8_speed;
	short leg2_vfd9_speed;
	short leg2_vfd10_speed;
	short leg2_vfd11_speed;
	short leg2_vfd12_speed;
	short leg2_vfd13_speed;
	short leg2_vfd14_speed;
	short leg2_power;
	short leg2_current_total;
	short leg2_load_total;
	float leg2_speed;
	float leg2_distance;
	char leg3_vfd1_running;
	char leg3_vfd2_running;
	char leg3_vfd3_running;
	char leg3_vfd4_running;
	char leg3_vfd5_running;
	char leg3_vfd6_running;
	char leg3_vfd7_running;
	char leg3_vfd8_running;
	char leg3_vfd9_running;
	char leg3_vfd10_running;
	char leg3_vfd11_running;
	char leg3_vfd12_running;
	char leg3_vfd13_running;
	char leg3_vfd14_running;
	char leg3_spare1;
	char leg3_spare2;
	char leg3_spare3;
	char leg3_spare4;
	char leg3_spare5;
	char leg3_spare6;
	char leg3_spare7;
	char leg3_spare8;
	char leg3_spare9;
	char leg3_spare10;
	char leg3_spare11;
	char leg3_spare12;
	char leg3_spare13;
	char leg3_spare14;
	char leg3_spare15;
	char leg3_spare16;
	char leg3_spare17;
	char leg3_spare18;
	char leg3_brake1_open;
	char leg3_brake2_open;
	char leg3_brake3_open;
	char leg3_brake4_open;
	char leg3_brake5_open;
	char leg3_brake6_open;
	char leg3_brake7_open;
	char leg3_brake8_open;
	char leg3_brake9_open;
	char leg3_brake10_open;
	char leg3_brake11_open;
	char leg3_brake12_open;
	char leg3_brake13_open;
	char leg3_brake14_open;
	char leg3_brake15_open;
	char leg3_brake16_open;
	char leg3_brake17_open;
	char leg3_brake18_open;
	char leg3_spare19;
	char leg3_spare20;
	char leg3_spare21;
	char leg3_spare22;
	char leg3_spare23;
	char leg3_spare24;
	char leg3_spare25;
	char leg3_spare26;
	char leg3_spare27;
	char leg3_spare28;
	char leg3_spare29;
	char leg3_spare30;
	char leg3_spare31;
	char leg3_spare32;
	char leg3_spare33;
	char leg3_spare34;
	char leg3_spare35;
	char leg3_spare36;
	char leg3_spare37;
	char leg3_spare38;
	char leg3_spare39;
	char leg3_spare40;
	char leg3_spare41;
	char leg3_spare42;
	char leg3_spare43;
	char leg3_spare44;
	char leg3_spare45;
	char leg3_spare46;
	char leg3_spare47;
	char leg3_spare48;
	char leg3_spare49;
	char leg3_spare50;
	char leg3_spare51;
	char leg3_spare52;
	char leg3_spare53;
	char leg3_spare54;
	char leg3_general_alarm;
	char leg3_raise;
	char leg3_lower;
	char leg3_touch_ground;
	char leg3_spare55;
	char leg3_spare56;
	short leg3_pinion1_load;
	short leg3_pinion2_load;
	short leg3_pinion3_load;
	short leg3_pinion4_load;
	short leg3_pinion5_load;
	short leg3_pinion6_load;
	short leg3_pinion7_load;
	short leg3_pinion8_load;
	short leg3_pinion9_load;
	short leg3_pinion10_load;
	short leg3_pinion11_load;
	short leg3_pinion12_load;
	short leg3_pinion13_load;
	short leg3_pinion14_load;
	short leg3_pinion15_load;
	short leg3_pinion16_load;
	short leg3_pinion17_load;
	short leg3_pinion18_load;
	short leg3_vfd1_speed;
	short leg3_vfd2_speed;
	short leg3_vfd3_speed;
	short leg3_vfd4_speed;
	short leg3_vfd5_speed;
	short leg3_vfd6_speed;
	short leg3_vfd7_speed;
	short leg3_vfd8_speed;
	short leg3_vfd9_speed;
	short leg3_vfd10_speed;
	short leg3_vfd11_speed;
	short leg3_vfd12_speed;
	short leg3_vfd13_speed;
	short leg3_vfd14_speed;
	short leg3_power;
	short leg3_current_total;
	short leg3_load_total;
	float leg3_speed;
	float leg3_distance;
	char leg4_vfd1_running;
	char leg4_vfd2_running;
	char leg4_vfd3_running;
	char leg4_vfd4_running;
	char leg4_vfd5_running;
	char leg4_vfd6_running;
	char leg4_vfd7_running;
	char leg4_vfd8_running;
	char leg4_vfd9_running;
	char leg4_vfd10_running;
	char leg4_vfd11_running;
	char leg4_vfd12_running;
	char leg4_vfd13_running;
	char leg4_vfd14_running;
	char leg4_spare1;
	char leg4_spare2;
	char leg4_spare3;
	char leg4_spare4;
	char leg4_spare5;
	char leg4_spare6;
	char leg4_spare7;
	char leg4_spare8;
	char leg4_spare9;
	char leg4_spare10;
	char leg4_spare11;
	char leg4_spare12;
	char leg4_spare13;
	char leg4_spare14;
	char leg4_spare15;
	char leg4_spare16;
	char leg4_spare17;
	char leg4_spare18;
	char leg4_brake1_open;
	char leg4_brake2_open;
	char leg4_brake3_open;
	char leg4_brake4_open;
	char leg4_brake5_open;
	char leg4_brake6_open;
	char leg4_brake7_open;
	char leg4_brake8_open;
	char leg4_brake9_open;
	char leg4_brake10_open;
	char leg4_brake11_open;
	char leg4_brake12_open;
	char leg4_brake13_open;
	char leg4_brake14_open;
	char leg4_spare19;
	char leg4_spare20;
	char leg4_spare21;
	char leg4_spare22;
	char leg4_spare23;
	char leg4_spare24;
	char leg4_spare25;
	char leg4_spare26;
	char leg4_spare27;
	char leg4_spare28;
	char leg4_spare29;
	char leg4_spare30;
	char leg4_spare31;
	char leg4_spare32;
	char leg4_spare33;
	char leg4_spare34;
	char leg4_spare35;
	char leg4_spare36;
	char leg4_spare37;
	char leg4_spare38;
	char leg4_spare39;
	char leg4_spare40;
	char leg4_spare41;
	char leg4_spare42;
	char leg4_spare43;
	char leg4_spare44;
	char leg4_spare45;
	char leg4_spare46;
	char leg4_spare47;
	char leg4_spare48;
	char leg4_spare49;
	char leg4_spare50;
	char leg4_spare51;
	char leg4_spare52;
	char leg4_spare53;
	char leg4_spare54;
	char leg4_general_alarm;
	char leg4_raise;
	char leg4_lower;
	char leg4_touch_ground;
	char leg4_spare55;
	char leg4_spare56;
	short leg4_pinion1_load;
	short leg4_pinion2_load;
	short leg4_pinion3_load;
	short leg4_pinion4_load;
	short leg4_pinion5_load;
	short leg4_pinion6_load;
	short leg4_pinion7_load;
	short leg4_pinion8_load;
	short leg4_pinion9_load;
	short leg4_pinion10_load;
	short leg4_pinion11_load;
	short leg4_pinion12_load;
	short leg4_pinion13_load;
	short leg4_pinion14_load;
	short leg4_vfd1_speed;
	short leg4_vfd2_speed;
	short leg4_vfd3_speed;
	short leg4_vfd4_speed;
	short leg4_vfd5_speed;
	short leg4_vfd6_speed;
	short leg4_vfd7_speed;
	short leg4_vfd8_speed;
	short leg4_vfd9_speed;
	short leg4_vfd10_speed;
	short leg4_vfd11_speed;
	short leg4_vfd12_speed;
	short leg4_vfd13_speed;
	short leg4_vfd14_speed;
	short leg4_power;
	short leg4_current_total;
	short leg4_load_total;
	float leg4_speed;
	float leg4_distance;
}platform_jacking_system;
#endif


typedef struct {
	char no;
	char mute;
	char ack;
    char status;
	char device_offline;
}buzzer;

typedef struct{
	char no;
	char status;
	char mute_or_ack;
	char device_offline;
	int panel_id;
	char alarm_state;
}extend_alarm_panel;

typedef struct{
	std::string m_keyname;
	float start;
	float end;
}range;

typedef struct{
	std::string m_keyname;
	std::string m_low_alarm;
	std::string m_high_alarm;
}threshold;

typedef struct{
	std::string m_keyname;
	int delaytime;
}delay;

typedef struct{
	std::string m_keyname;
	bool is_inhibit;
}inhibit;

typedef struct{
	char no;
	char is_on_duty;//0:not on   1:on
}onduty;

typedef struct{
	std::string ip;
	std::string name;
	std::string location;
	char current;
	char status;
	char device_offline;
}ams_pc;

typedef struct{
	char status;
	char device_offline;
}ams_sau;

#endif
