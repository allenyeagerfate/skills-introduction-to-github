#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>

#include "sgo_global.h"
#include "sgo_macro.h"
#include "sgo_func.h"
#include "sgo_conf.h"

//全局量---------------------
//错误等级，和sgo_macro.h里定义的日志等级宏是一一对应关系
static u_char err_levels[][20]  = 
{
    {"stderr"},    //0：控制台错误
    {"emerg"},     //1：紧急
    {"alert"},     //2：警戒
    {"crit"},      //3：严重
    {"error"},     //4：错误
    {"warn"},      //5：警告
    {"notice"},    //6：注意
    {"info"},      //7：信息
    {"debug"}      //8：调试
};
sgo_log_t   sgo_log;


//----------------------------------------------------------------------------------------------------------------------
//调用格式比如：sgo_log_stderr(0, "invalid option: \"%s\",%d", "testinfo",123);
 /* 
    sgo_log_stderr(0, "invalid option: \"%s\"", argv[0]);  //nginx: invalid option: "./nginx"
    sgo_log_stderr(0, "invalid option: %10d", 21);         //nginx: invalid option:         21  ---21前面有8个空格
    sgo_log_stderr(0, "invalid option: %.6f", 21.378);     //nginx: invalid option: 21.378000   ---%.这种只跟f配合有效，往末尾填充0
    sgo_log_stderr(0, "invalid option: %.6f", 12.999);     //nginx: invalid option: 12.999000
    sgo_log_stderr(0, "invalid option: %.2f", 12.999);     //nginx: invalid option: 13.00
    sgo_log_stderr(0, "invalid option: %xd", 1678);        //nginx: invalid option: 68E
    sgo_log_stderr(0, "invalid option: %Xd", 1678);        //nginx: invalid option: 68E
    sgo_log_stderr(15, "invalid option: %s , %d", "testInfo",326);        //nginx: invalid option: testInfo , 326
    sgo_log_stderr(0, "invalid option: %d", 1678); 
    */
void sgo_log_stderr(int err, const char *fmt, ...)
{    
    va_list args;                        //创建一个va_list类型变量
    u_char  errstr[SGO_MAX_ERROR_STR+1]; 
    u_char  *p,*last;

    memset(errstr,0,sizeof(errstr)); 
    last = errstr + SGO_MAX_ERROR_STR;                                          
    p = sgo_cpymem(errstr, "sgo-collect: ", 13);   
    
    va_start(args, fmt); //使args指向起始的参数
    p = sgo_vslprintf(p,last,fmt,args); //组合出这个字符串保存在errstr里
    va_end(args);        //释放args

    if (err)
    {
        p = sgo_log_errno(p, last, err);
    }
    
    if (p >= (last - 1))
    {
        p = (last - 1) - 1; 
    }
    *p++ = '\n';  
    
    write(STDERR_FILENO,errstr,p - errstr); 

    if(sgo_log.fd > STDERR_FILENO)
    {
        err = 0; 
        p--;*p = 0; 
        sgo_log_error_core(SGO_LOG_STDERR,err,(const char *)errstr); 
    }    
    return;
}

//----------------------------------------------------------------------------------------------------------------------
u_char *sgo_log_errno(u_char *buf, u_char *last, int err)
{
    char *perrorinfo = strerror(err);
    size_t len = strlen(perrorinfo);

    char leftstr[10] = {0}; 
    sprintf(leftstr," (%d: ",err);
    size_t leftlen = strlen(leftstr);

    char rightstr[] = ") "; 
    size_t rightlen = strlen(rightstr);
    
    size_t extralen = leftlen + rightlen;
    if ((buf + len + extralen) < last)
    {
        buf = sgo_cpymem(buf, leftstr, leftlen);
        buf = sgo_cpymem(buf, perrorinfo, len);
        buf = sgo_cpymem(buf, rightstr, rightlen);
    }
    return buf;
}

void sgo_log_error_core(int level,  int err, const char *fmt, ...)
{
    u_char  *last;
    u_char  errstr[SGO_MAX_ERROR_STR+1];
    memset(errstr,0,sizeof(errstr));  
    last = errstr + SGO_MAX_ERROR_STR;   
    
    struct timeval   tv;
    struct tm        tm;
    time_t           sec;
    u_char           *p;
    va_list          args;

    memset(&tv,0,sizeof(struct timeval));    
    memset(&tm,0,sizeof(struct tm));

    gettimeofday(&tv, NULL);       

    sec = tv.tv_sec;
    localtime_r(&sec, &tm);
    tm.tm_mon++;
    tm.tm_year += 1900;
    
    u_char strcurrtime[40]={0};
    sgo_slprintf(strcurrtime,  
                    (u_char *)-1,                     
                    "%4d/%02d/%02d %02d:%02d:%02d", 
                    tm.tm_year, tm.tm_mon,
                    tm.tm_mday, tm.tm_hour,
                    tm.tm_min, tm.tm_sec);
    p = sgo_cpymem(errstr,strcurrtime,strlen((const char *)strcurrtime));
    p = sgo_slprintf(p, last, " [%s] ", err_levels[level]);    

    va_start(args, fmt);
    p = sgo_vslprintf(p, last, fmt, args);
    va_end(args);

    if (err)
    {
        p = sgo_log_errno(p, last, err);
    }

    if (p >= (last - 1))
    {
        p = (last - 1) - 1;
	}
    *p++ = '\n';      

    ssize_t   n;
    while(1) 
    {        
        if (level > sgo_log.log_level) 
        {
            break;
        }
      
        n = write(sgo_log.fd,errstr,p - errstr);
        if (n == -1) 
        {
            if(errno == ENOSPC) 
            {
            }
            else
            {
                if(sgo_log.fd != STDERR_FILENO)
                {
                    n = write(STDERR_FILENO,errstr,p - errstr);
                }
            }
        }
        break;
    }  
    return;
}

//----------------------------------------------------------------------------------------------------------------------
void sgo_log_init()
{
    u_char *plogname = NULL;
    size_t nlen;

    //从配置文件中读取和日志相关的配置信息
    CConfig *p_config = CConfig::GetInstance();
    plogname = (u_char *)p_config->GetString("Log");
    if(plogname == NULL)
    {
        plogname = (u_char *) SGO_ERROR_LOG_PATH;
    }
    sgo_log.log_level = p_config->GetIntDefault("LogLevel",SGO_LOG_NOTICE);
    sgo_log.fd = open((const char *)plogname,O_WRONLY|O_APPEND|O_CREAT,0644);  
    if (sgo_log.fd == -1) 
    {
    	sgo_log_stderr(errno,"[alert] could not open error log file: open() \"%s\" failed", plogname);
        sgo_log.fd = STDERR_FILENO;   
    } 
    return;
}


///////////////////////////////////FILELOG AMS
void FILELOG_main_diesel_generator(const main_diesel_generator &one)
{
#define DEBUG
#ifdef DEBUG
	static int cnt0=0,cnt1=0,cnt2=0,cnt3=0,cnt4=0;
#ifdef JANA505
	static int cnt5=0;
	if( (one.gen_id == 0) && (cnt0++ % PRINT_INTERVAL == 0)
		|| (one.gen_id == 1) && (cnt1++ % PRINT_INTERVAL == 0)
		|| (one.gen_id == 2) && (cnt2++ % PRINT_INTERVAL == 0)
		|| (one.gen_id == 3) && (cnt3++ % PRINT_INTERVAL == 0)
		|| (one.gen_id == 4) && (cnt4++ % PRINT_INTERVAL == 0)
		|| (one.gen_id == 5) && (cnt5++ % PRINT_INTERVAL == 0)
	)
#endif
#ifdef JANA508509
	if( (one.gen_id == 0) && (cnt0++ % PRINT_INTERVAL == 0)
		|| (one.gen_id == 1) && (cnt1++ % PRINT_INTERVAL == 0)
		|| (one.gen_id == 2) && (cnt2++ % PRINT_INTERVAL == 0)
		|| (one.gen_id == 3) && (cnt3++ % PRINT_INTERVAL == 0)
		|| (one.gen_id == 4) && (cnt4++ % PRINT_INTERVAL == 0)
	)
#endif
	{
		sgo_log_stderr(0,"main_diesel_generator[%d]:"
						"d_g_dcu_unit_battery_volt_dcu_control_power:%.2f,\n"
						"d_g_lube_oil_pressure:%.2f,\n"
						"d_g_coolant_pressure:%.2f,\n"
						"d_g_start_air_pressure:%.2f,\n"
						"d_g_fuel_oil_pressure:%.2f,\n"
						"d_g_coolant_temperature:%.2f,\n"
						"d_g_lube_oil_temperature:%.2f,\n"
						"d_g_left_exhaust_temperature:%.2f,\n"
						"d_g_right_exhaust_temperature:%.2f,\n"
						"d_g_engine_speed:%.2f,\n"
						"d_g_running_hours:%.2f,\n"
						"d_g_rpu_unit_battery_a_volt_rpu_main_control_power:%.2f,\n"
						"d_g_rpu_unit_battery_b_volt_rpu_backup_control_power:%.2f",
			one.gen_id,
			one.d_g_dcu_unit_battery_volt_dcu_control_power,
			one.d_g_lube_oil_pressure,
			one.d_g_coolant_pressure,
			one.d_g_start_air_pressure,
			one.d_g_fuel_oil_pressure,
			one.d_g_coolant_temperature,
			one.d_g_lube_oil_temperature,
			one.d_g_left_exhaust_temperature,
			one.d_g_right_exhaust_temperature,
			one.d_g_engine_speed,
			one.d_g_running_hours,
			one.d_g_rpu_unit_battery_a_volt_rpu_main_control_power,
			one.d_g_rpu_unit_battery_b_volt_rpu_backup_control_power);
		sgo_log_stderr(0,"main_diesel_generator[%d]:"
						"d_g_coolant_level_low:%d,\n"
						"d_g_main_power_failure:%d,\n"
						"d_g_backup_power_failure:%d,\n"
						"d_g_sea_water_pump_overload_trip:%d,\n"
						"d_g_ecr_remote_emergency_stop:%d,\n"
						"d_g_aft_bridge_remote_emergency_stop:%d,\n"
						"d_g_fwd_bridge_remote_emergency_stop:%d,\n"
						"d_g_lube_oil_pressure_too_low_shutdown:%d,\n"
						"d_g_coolant_temperature_too_high_shutdown:%d,\n"
						"d_g_local_emergency_stop:%d,\n"
						"d_g_remote_emergency_stop:%d,\n"
						"d_g_rpu_emergency_stop:%d,\n"
						"d_g_lube_oil_pressure_low_alarm:%d,\n"
						"d_g_coolant_pressure_low_alarm:%d,\n"
						"d_g_start_air_pressure_low_alarm:%d,\n"
						"d_g_fuel_oil_pressure_low_alarm:%d,\n"
						"d_g_coolant_temperature_high_alarm:%d,\n"
						"d_g_lube_oil_temperature_high_alarm:%d,\n"
						"d_g_left_exhaust_temperature_high_alarm:%d,\n"
						"d_g_right_exhaust_temperature_high_alarm:%d,\n"
						"d_g_overspeed_alarm:%d,\n"
						"d_g_overspeed_shutdown:%d,\n"
						"d_g_speed_sensor_fail:%d,\n"
						"d_g_dcu_unit_battery_volt_dct_control_power_low:%d,\n"
						"d_g_start_fail:%d,\n"
						"d_g_rpu_unit_battery_a_volt_rpu_main_control_power_low:%d,\n"
						"d_g_rpu_unit_battery_b_volt_rpu_main_control_power_low:%d,\n"
						"d_g_common_alarm:%d,\n"
						"d_g_common_warning:%d,\n"
						"d_g_common_shutdown:%d,\n"
						"d_g_common_cooldown:%d,\n"
						"d_g_common_faultsensor_fail:%d,\n"
						"d_g_remote_control_status_auxaccess_lock:%d,\n"
						"d_g_ready_to_start:%d,\n"
						"d_g_ready_to_load:%d,\n"
						"d_g_main_generator_running:%d",
			one.gen_id,			
			one.d_g_coolant_level_low,
			one.d_g_main_power_failure,
			one.d_g_backup_power_failure,
			one.d_g_sea_water_pump_overload_trip,
			one.d_g_ecr_remote_emergency_stop,
			one.d_g_aft_bridge_remote_emergency_stop,
			one.d_g_fwd_bridge_remote_emergency_stop,
			one.d_g_lube_oil_pressure_too_low_shutdown,
			one.d_g_coolant_temperature_too_high_shutdown,
			one.d_g_local_emergency_stop,
			one.d_g_remote_emergency_stop,
			one.d_g_rpu_emergency_stop,
			one.d_g_lube_oil_pressure_low_alarm,
			one.d_g_coolant_pressure_low_alarm,
			one.d_g_start_air_pressure_low_alarm,
			one.d_g_fuel_oil_pressure_low_alarm,
			one.d_g_coolant_temperature_high_alarm,
			one.d_g_lube_oil_temperature_high_alarm,
			one.d_g_left_exhaust_temperature_high_alarm,
			one.d_g_right_exhaust_temperature_high_alarm,
			one.d_g_overspeed_alarm,
			one.d_g_overspeed_shutdown,
			one.d_g_speed_sensor_fail,
			one.d_g_dcu_unit_battery_volt_dct_control_power_low,
			one.d_g_start_fail,
			one.d_g_rpu_unit_battery_a_volt_rpu_main_control_power_low,
			one.d_g_rpu_unit_battery_b_volt_rpu_main_control_power_low,
			one.d_g_common_alarm,
			one.d_g_common_warning,
			one.d_g_common_shutdown,
			one.d_g_common_cooldown,
			one.d_g_common_faultsensor_fail,
			one.d_g_remote_control_status_auxaccess_lock,
			one.d_g_ready_to_start,
			one.d_g_ready_to_load,
			one.d_g_main_generator_running);
		sgo_log_stderr(0,"main_diesel_generator[%d]:"
						"d_g_main_generator_u_winding_temperature:%.2f,\n"
						"d_g_main_generator_v_winding_temperature:%.2f,\n"
						"d_g_main_generator_w_winding_temperature:%.2f,\n"
						"d_g_main_generator_de_bearing_temperature:%.2f,\n"
						"d_g_main_generator_nde_bearing_temperature:%.2f,\n"
						"d_g_main_generator_excitation_current:%.2f,\n"
						"d_g_main_generator_excitation_voltage:%.2f",
			one.gen_id,			
			one.d_g_main_generator_u_winding_temperature,
			one.d_g_main_generator_v_winding_temperature,
			one.d_g_main_generator_w_winding_temperature,
			one.d_g_main_generator_de_bearing_temperature,
			one.d_g_main_generator_nde_bearing_temperature,
			one.d_g_main_generator_excitation_current,
			one.d_g_main_generator_excitation_voltage);
	}

#endif
}

void FILELOG_baudouin_main_generator(const baudouin_main_generator &one)
{
#ifdef DEBUG
	static int cnt0=0,cnt1 = 0;
	if( (one.gen_id == 0) && (cnt0++ % PRINT_INTERVAL == 0)
		|| (one.gen_id == 1) && (cnt1++ % PRINT_INTERVAL == 0)
		)
	{
		sgo_log_stderr(0,"gen_id:%d,\n"
			"d_g_engine_speed:%.2f,\n"
			"d_g_battery_voltage:%.2f,\n"
			"d_g_boost_t_ps:%.2f,\n"
			"d_g_boost_t_sb:%.2f,\n"
			"d_g_speed_r:%.2f,\n"
			"d_g_speed_ma:%.2f,\n"
			"d_g_oil_piston_jet:%.2f,\n"
			"d_g_coolant_press:%.2f,\n"
			"d_g_exhaust_t_sb:%.2f,\n"
			"d_g_exhaust_t_ps:%.2f,\n"
			"d_g_raw_wat_press:%.2f,\n"
			"d_g_load:%.2f,\n"
			"d_g_cool_temp_sb:%.2f,\n"
			"d_g_fuel_temp:%.2f,\n"
			"d_g_oil_temp:%.2f,\n"
			"d_g_intake_p_sb:%.2f,\n"
			"d_g_intake_t_sb:%.2f,\n"
			"d_g_oil_press:%.2f,\n"
			"d_g_fuel_press:%.2f,\n"
			"d_g_fuel_rate:%.2f,\n"
			"d_g_ecu_battvol_m:%.2f,\n"
			"d_g_ecu_temp_m:%.2f,\n"
			"d_g_lever_pos:%.2f,\n"
			"d_g_rail_press_sb:%.2f,\n"
			"d_g_tot_fuel_used:%.2f,\n"
			"d_g_tot_fuel_used_l:%.2f,\n"
			"d_g_rpu_rpm:%.2f,\n"
			"d_g_rpu_ubat_a:%.2f,\n"
			"d_g_rpu_ubat_b:%.2f,\n"
			"d_g_pri_battery:%.2f,\n"
			"d_g_sec_battery:%.2f,\n"
			"d_g_cool_temp_ps:%.2f,\n"
			"d_g_intake_p_ps:%.2f,\n"
			"d_g_intake_t_ps:%.2f,\n"
			"d_g_ecu_battvol_s:%.2f,\n"
			"d_g_ecu_temp_s:%.2f,\n"
			"d_g_rail_press_ps:%.2f,\n"
			"d_g_engine_hours:%.2f,\n"
			"d_g_run_hours:%.2f,\n"
			"d_g_service_time:%.2f,\n"
			"d_g_engine_state:%.2f,\n"
			"d_g_mode_id:%.2f",
		one.gen_id,
		one.d_g_engine_rpm,
		one.d_g_battery_voltage,
		one.d_g_boost_t_ps,
		one.d_g_boost_t_sb,
		one.d_g_speed_r,
		one.d_g_speed_ma,
		one.d_g_oil_piston_jet,
		one.d_g_coolant_press,
		one.d_g_exhaust_t_sb,
		one.d_g_exhaust_t_ps,
		one.d_g_raw_wat_press,
		one.d_g_load,
		one.d_g_cool_temp_sb,
		one.d_g_fuel_temp,
		one.d_g_oil_temp,
		one.d_g_intake_p_sb,
		one.d_g_intake_t_sb,
		one.d_g_oil_press,
		one.d_g_fuel_press,
		one.d_g_fuel_rate,
		one.d_g_ecu_battvol_m,
		one.d_g_ecu_temp_m,
		one.d_g_lever_pos,
		one.d_g_rail_press_sb,
		one.d_g_tot_fuel_used,
		one.d_g_tot_fuel_used_l,
		one.d_g_rpu_rpm,
		one.d_g_rpu_ubat_a,
		one.d_g_rpu_ubat_b,
		one.d_g_pri_battery,
		one.d_g_sec_battery,
		one.d_g_cool_temp_ps,
		one.d_g_intake_p_ps,
		one.d_g_intake_t_ps,
		one.d_g_ecu_battvol_s,
		one.d_g_ecu_temp_s,
		one.d_g_rail_press_ps,
		one.d_g_engine_hours,
		one.d_g_run_hours,
		one.d_g_service_time,
		one.d_g_engine_state,
		one.d_g_mode_id);


	sgo_log_stderr(0,"d_g_water_in_fuel:%d,\n"
					"d_g_remoter_start:%d,\n"
					"d_g_remote_stop:%d,\n"
					"d_g_priming_pump:%d,\n"
					"d_g_increase_speed:%d,\n"
					"d_g_ecu_slave_on:%d,\n"
					"d_g_lo_lvl_raw_wat:%d,\n"
					"d_g_mode2:%d,\n"
					"d_g_decrease_speed:%d,\n"
					"d_g_mode1:%d,\n"
					"d_g_fuel_leak_sb:%d,\n"
					"d_g_ecu_master_on:%d,\n"
					"d_g_fuel_leak_ps:%d,\n"
					"d_g_lo_lvl_coolant:%d,\n"
					"d_g_very_lo_oil_p:%d,\n"
					"d_g_very_hi_cool_t:%d,\n"
					"d_g_rpu_sd3:%d,\n"
					"d_g_rpu_sd4:%d,\n"
					"d_g_rpu_sd5:%d,\n"
					"d_g_emstop_rpu:%d,\n"
					"d_g_turning_gear:%d,\n"
					"d_g_light_test:%d,\n"
					"d_g_start_air_p_lo:%d,\n"
					"d_g_diff_eng_oil_p:%d,\n"
					"d_g_pre_heat_cde:%d,\n"
					"d_g_override:%d,\n"
					"d_g_ecucomm:%d,\n"
					"d_g_ecu_warning:%d,\n"
					"d_g_ecu_bin3:%d,\n"
					"d_g_ecu_bin4:%d,\n"
					"d_g_ecu_bin5:%d,\n"
					"d_g_ecu_bin6:%d,\n"
					"d_g_glow_lamp:%d,\n"
					"d_g_ecu_bin8:%d,\n"
					"d_g_ain11:%d,\n"
					"d_g_ain13:%d,\n"
					"d_g_shutoffreqm:%d,\n"
					"d_g_shutoffreqs:%d,\n"
					"d_g_ra153_bout:%d,\n"
					"d_g_detcon:%d,\n"
					"d_g_rpu_fail:%d,\n"
					"d_g_bw_rpu_fuelsol:%d,\n"
					"d_g_bw_rpu_stopsol:%d,\n"
					"d_g_ecu_alarmlist:%d,\n"
					"d_g_ecucompconsist:%d,\n"
					"d_g_actcall_fail:%d,\n"
					"d_g_derate_alarm:%d,\n"
					"d_g_ecudiagblocked:%d,\n"
					"d_g_pwr_limited:%d,\n"
					"d_g_scm_fail:%d,\n"
					"d_g_rtcbatteryflat:%d,\n"
					"d_g_emergency_stop:%d,\n"
					"d_g_overload:%d,\n"
					"d_g_othrcommer:%d,\n"
					"d_g_chrgalternfail:%d,\n"
					"d_g_stop_fail:%d,\n"
					"d_g_overspeed:%d,\n"
					"d_g_underspeed:%d,\n"
					"d_g_pickup_fail:%d,\n"
					"d_g_battery_flat:%d,\n"
					"d_g_start_fail:%d,\n"
					"d_g_op_rpmdep:%d,\n"
					"d_g_cp_rpmdep:%d,\n"
					"d_g_wrn_rpm_diff:%d,\n"
					"d_g_switchtobatt_b:%d,\n"
					"d_g_start_blocking:%d,\n"
					"d_g_load_blocking:%d,\n"
					"d_g_primbatactive:%d,\n"
					"d_g_secbatactive:%d,\n"
					"d_g_emssupply10a:%d,\n"
					"d_g_extrasupply7a:%d,\n"
					"d_g30supply7a:%d,\n"
					"d_g_ecu_yellowlamp:%d,\n"
					"d_g_ecu_red_lamp:%d,\n"
					"d_g_ecu_waittostrt:%d,\n"
					"device_offline:%d",
			one.d_g_water_in_fuel,
			one.d_g_remoter_start,
			one.d_g_remote_stop,
			one.d_g_priming_pump,
			one.d_g_increase_speed,
			one.d_g_ecu_slave_on,
			one.d_g_lo_lvl_raw_wat,
			one.d_g_mode2,
			one.d_g_decrease_speed,
			one.d_g_mode1,
			one.d_g_fuel_leak_sb,
			one.d_g_ecu_master_on,
			one.d_g_fuel_leak_ps,
			one.d_g_lo_lvl_coolant,
			one.d_g_very_lo_oil_p,
			one.d_g_very_hi_cool_t,
			one.d_g_rpu_sd3,
			one.d_g_rpu_sd4,
			one.d_g_rpu_sd5,
			one.d_g_emstop_rpu,
			one.d_g_turning_gear,
			one.d_g_light_test,
			one.d_g_start_air_p_lo,
			one.d_g_diff_eng_oil_p,
			one.d_g_pre_heat_cde,
			one.d_g_override,
			one.d_g_ecucomm,
			one.d_g_ecu_warning,
			one.d_g_ecu_bin3,
			one.d_g_ecu_bin4,
			one.d_g_ecu_bin5,
			one.d_g_ecu_bin6,
			one.d_g_glow_lamp,
			one.d_g_ecu_bin8,
			one.d_g_ain11,
			one.d_g_ain13,
			one.d_g_shutoffreqm,
			one.d_g_shutoffreqs,
			one.d_g_ra153_bout,
			one.d_g_detcon,
			one.d_g_rpu_fail,
			one.d_g_bw_rpu_fuelsol,
			one.d_g_bw_rpu_stopsol,
			one.d_g_ecu_alarmlist,
			one.d_g_ecucompconsist,
			one.d_g_actcall_fail,
			one.d_g_derate_alarm,
			one.d_g_ecudiagblocked,
			one.d_g_pwr_limited,
			one.d_g_scm_fail,
			one.d_g_rtcbatteryflat,
			one.d_g_emergency_stop,
			one.d_g_overload,
			one.d_g_othrcommer,
			one.d_g_chrgalternfail,
			one.d_g_stop_fail,
			one.d_g_overspeed,
			one.d_g_underspeed,
			one.d_g_pickup_fail,
			one.d_g_battery_flat,
			one.d_g_start_fail,
			one.d_g_op_rpmdep,
			one.d_g_cp_rpmdep,
			one.d_g_wrn_rpm_diff,
			one.d_g_switchtobatt_b,
			one.d_g_start_blocking,
			one.d_g_load_blocking,
			one.d_g_primbatactive,
			one.d_g_secbatactive,
			one.d_g_emssupply10a,
			one.d_g_extrasupply7a,
			one.d_g30supply7a,
			one.d_g_ecu_yellowlamp,
			one.d_g_ecu_red_lamp,
			one.d_g_ecu_waittostrt,
			one.device_offline);
	}
#endif		
}


void FILELOG_emergency_generator(const emergency_generator &one)
{
#ifdef DEBUG
	static int cnt = 0;
	if(cnt++ % PRINT_INTERVAL == 0)
		sgo_log_stderr(0,"emergency_generator: eg_dcu_unit_battery_volt_dcu_control_power:%.2f,\n"
							"eg_lube_oil_pressure:%.2f,\n"
							"eg_coolant_pressure:%.2f,\n"
							"eg_coolant_temperature:%.2f,\n"
							"eg_lube_oil_temperature:%.2f,\n"
							"eg_engine_speed:%.2f,\n"
							"eg_running_hours:%.2f,\n"
							"eg_rpu_unit_battery_a_volt_rpu_main_control_power:%.2f,\n"
							"eg_rpu_unit_battery_b_volt_rpu_backup_control_power:%.2f,\n"
							"eg_coolant_level_low:%d,\n"
							"eg_main_power_failure:%d,\n"
							"eg_backup_power_failure:%d,\n"
							"eg_no1_charger_failure:%d,\n"
							"eg_no2_charger_failure:%d,\n"
							"eg_local_emergency_stop:%d,\n"
							"eg_remote_emergency_stop:%d,\n"
							"eg_rpu_emergency_stop:%d,\n"
							"eg_lube_oil_pressure_low_alarm:%d,\n"
							"eg_coolant_pressure_low_alarm:%d,\n"
							"eg_coolant_temperature_high_alarm:%d,\n"
							"eg_lube_oil_temperature_high_alarm:%d,\n"
							"eg_overspeed_alarm:%d,\n"
							"eg_overspeed_shutdown:%d,\n"
							"eg_speed_sensor_fail:%d,\n"
							"eg_dcu_unit_battery_volt_dct_control_power_low:%d,\n"
							"eg_start_fail:%d,\n"
							"eg_rpu_unit_battery_a_volt_rpu_main_control_power_low:%d,\n"
							"eg_rpu_unit_battery_b_volt_rpu_main_control_power_low:%d,\n"
							"eg_common_alarm:%d,\n"
							"eg_common_warning:%d,\n"
							"eg_common_shutdown:%d,\n"
							"eg_common_cooldown:%d,\n"
							"eg_common_faultsensor_fail:%d,\n"
							"eg_remote_control_status_auxaccess_lock:%d,\n"
							"eg_ready_to_start:%d,\n"
							"eg_ready_to_load:%d,\n"
							"eg_generator_running:%d,\n"
							"eg_generator_u_winding_temperature:%.2f,\n"
							"eg_generator_v_winding_temperature:%.2f,\n"
							"eg_generator_w_winding_temperature:%.2f",
			one.eg_dcu_unit_battery_volt_dcu_control_power,
			one.eg_lube_oil_pressure,
			one.eg_coolant_pressure,
			one.eg_coolant_temperature,
			one.eg_lube_oil_temperature,
			one.eg_engine_speed,
			one.eg_running_hours,
			one.eg_rpu_unit_battery_a_volt_rpu_main_control_power,
			one.eg_rpu_unit_battery_b_volt_rpu_backup_control_power,
			one.eg_coolant_level_low,
			one.eg_main_power_failure,
			one.eg_backup_power_failure,
			one.eg_no1_charger_failure,
			one.eg_no2_charger_failure,
			one.eg_local_emergency_stop,
			one.eg_remote_emergency_stop,
			one.eg_rpu_emergency_stop,
			one.eg_lube_oil_pressure_low_alarm,
			one.eg_coolant_pressure_low_alarm,
			one.eg_coolant_temperature_high_alarm,
			one.eg_lube_oil_temperature_high_alarm,
			one.eg_overspeed_alarm,
			one.eg_overspeed_shutdown,
			one.eg_speed_sensor_fail,
			one.eg_dcu_unit_battery_volt_dct_control_power_low,
			one.eg_start_fail,
			one.eg_rpu_unit_battery_a_volt_rpu_main_control_power_low,
			one.eg_rpu_unit_battery_b_volt_rpu_main_control_power_low,
			one.eg_common_alarm,
			one.eg_common_warning,
			one.eg_common_shutdown,
			one.eg_common_cooldown,
			one.eg_common_faultsensor_fail,
			one.eg_remote_control_status_auxaccess_lock,
			one.eg_ready_to_start,
			one.eg_ready_to_load,
			one.eg_generator_running,
			one.eg_generator_u_winding_temperature,
			one.eg_generator_v_winding_temperature,
			one.eg_generator_w_winding_temperature);
	
#endif
}

void FILELOG_main_propulsion(const main_propulsion& one)
{
#ifdef DEBUG
	static int cnt0 = 0,cnt1 = 0;
	if(one.no == 0  && cnt0++ % PRINT_INTERVAL == 0 
		|| one.no == 1  && cnt1++ % PRINT_INTERVAL == 0)
	{
		sgo_log_stderr(0,"main_propulsion[%d]:main_propulsion_control_position:%d,\n"
							"main_propulsion_rudder_indication:%.2f,\n"
							"main_propulsion_vfd_rpm_indication:%.2f,\n"
							"main_propulsion_vfd_load_indication:%.2f,\n"
							"main_propulsion_motor_u_winding_temperature:%.2f,\n"
							"main_propulsion_motor_v_winding_temperature:%.2f,\n"
							"main_propulsion_motor_w_winding_temperature:%.2f,\n"
							"main_propulsion_motor_de_bearing_temperature:%.2f,\n"
							"main_propulsion_motor_nde_bearing_temperature:%.2f,\n"
							"main_propulsion_motor_air_inlet_temperature:%.2f,\n"
							"main_propulsion_motor_air_outlet_temperature:%.2f,\n"
							"main_propulsion_rudder_feedback_signal_missing:%d,\n"
							"main_propulsion_control_system_main_power_failure:%d,\n"
							"main_propulsion_control_system_backup_power_failure:%d,\n"
							"main_propulsion_steering_command_missing_joystick:%d,\n"
							"main_propulsion_speed_command_missing_joystick:%d,\n"
							"main_propulsion_fu_steering_control_failure:%d,\n"
							"main_propulsion_fu_speed_control_failure:%d,\n"
							"main_propulsion_vfd_speed_feedback_signal_missing:%d,\n"
							"main_propulsion_vfd_load_feedback_signal_missing:%d,\n"
							"main_propulsion_steering_command_missing_fwd_console:%d,\n"
							"main_propulsion_speed_command_missing_fwd_console:%d,\n"
							"main_propulsion_rudder_command_missing_aft_console:%d,\n"
							"main_propulsion_speed_command_missing_aft_console:%d,\n"
							"azimuth_thruster_common_alarm:%d,\n"
							"azimuth_thruster_fault:%d,\n"
							"main_propulsion_remote_control_system_communication_failure:%d,\n"
							"main_propulsion_vfd_fault:%d,\n"
							"main_propulsion_vfd_common_alarm:%d,\n"
							"main_propulsion_vfd_emergency_stop:%d",
					one.no,
					one.main_propulsion_control_position,
					one.main_propulsion_rudder_indication,
					one.main_propulsion_vfd_rpm_indication,
					one.main_propulsion_vfd_load_indication,
					one.main_propulsion_motor_u_winding_temperature,
					one.main_propulsion_motor_v_winding_temperature,
					one.main_propulsion_motor_w_winding_temperature,
					one.main_propulsion_motor_de_bearing_temperature,
					one.main_propulsion_motor_nde_bearing_temperature,
					one.main_propulsion_motor_air_inlet_temperature,
					one.main_propulsion_motor_air_outlet_temperature,
					one.main_propulsion_rudder_feedback_signal_missing,
					one.main_propulsion_control_system_main_power_failure,
					one.main_propulsion_control_system_backup_power_failure,
					one.main_propulsion_steering_command_missing_joystick,
					one.main_propulsion_speed_command_missing_joystick,
					one.main_propulsion_fu_steering_control_failure,
					one.main_propulsion_fu_speed_control_failure,
					one.main_propulsion_vfd_speed_feedback_signal_missing,
					one.main_propulsion_vfd_load_feedback_signal_missing,
					one.main_propulsion_steering_command_missing_fwd_console,
					one.main_propulsion_speed_command_missing_fwd_console,
					one.main_propulsion_rudder_command_missing_aft_console,
					one.main_propulsion_speed_command_missing_aft_console,
					one.azimuth_thruster_common_alarm,
					one.azimuth_thruster_fault,
					one.main_propulsion_remote_control_system_communication_failure,
					one.main_propulsion_vfd_fault,
					one.main_propulsion_vfd_common_alarm,
					one.main_propulsion_vfd_emergency_stop);


	sgo_log_stderr(0,"main_propulsion[%d]: main_propulsion_vfd_slowdown_pre_alarm:%d,\n"
							"main_propulsion_vfd_power_limit:%d,\n"
							"azimuth_thruster_steering_vfd_fault:%d,\n"
							"main_propulsion_motor_water_leakage:%d,\n"
							"main_propulsion_motor_u_winding_temrpeature_high:%d,\n"
							"main_propulsion_motor_v_winding_temrpeature_high:%d,\n"
							"main_propulsion_motor_w_winding_temrpeature_high:%d,\n"
							"main_propulsion_motor_high_temrpeature_slowdown:%d,\n"
							"main_propulsion_motor_high_temrpeature_shutdown:%d,\n"
							"main_propulsion_motor_de_bearing_high_temperature_alarm:%d,\n"
							"main_propulsion_motor_de_bearing_high_temperature_slowdown:%d,\n"
							"main_propulsion_motor_de_bearing_high_temperature_shutdown:%d,\n"
							"main_propulsion_motor_nde_bearing_high_temperature_alarm:%d,\n"
							"main_propulsion_motor_nde_bearing_high_temperature_slowdown:%d,\n"
							"main_propulsion_motor_nde_bearing_high_temperature_shutdown:%d,\n"
							"main_propulsion_motor_air_inlet_high_temperature_alarm:%d,\n"
							"main_propulsion_motor_air_outlet_high_temperature_alarm:%d,\n"
							"main_propulsion_motor_high_temrpeature_slowdown_pre_alarm:%d,\n"
							"main_propulsion_motor_high_temrpeature_shutdown_pre_alarm:%d,\n"
							"main_propulsion_motor_de_bearing_high_temp_slowdown_pre_alarm:%d,\n"
							"main_propulsion_motor_de_bearing_high_tem_shutdown_pre_alarm:%d,\n"
							"main_propulsion_motor_nde_bearing_high_temp_slowdown_pre_alarm:%d,\n"
							"main_propulsion_motor_nde_bearing_high_tem_shutdown_pre_alarm:%d,\n"
							"main_propulsion_emergency_control_mode:%d,\n"
							"main_propulsion_shaft_lock_indication:%d,\n"
							"main_propulsion_vfd_ready:%d,\n"
							"main_propulsion_vfd_remote_control_position:%d,\n"
							"main_propulsion_vfd_running:%d,\n"
							"main_propulsion_vfd_override:%d,\n"
							"main_propulsion_vfd_torque_limitation:%d",
					one.no,
					one.main_propulsion_vfd_slowdown_pre_alarm,
					one.main_propulsion_vfd_power_limit,
					one.azimuth_thruster_steering_vfd_fault,
					one.main_propulsion_motor_water_leakage,
					one.main_propulsion_motor_u_winding_temrpeature_high,
					one.main_propulsion_motor_v_winding_temrpeature_high,
					one.main_propulsion_motor_w_winding_temrpeature_high,
					one.main_propulsion_motor_high_temrpeature_slowdown,
					one.main_propulsion_motor_high_temrpeature_shutdown,
					one.main_propulsion_motor_de_bearing_high_temperature_alarm,
					one.main_propulsion_motor_de_bearing_high_temperature_slowdown,
					one.main_propulsion_motor_de_bearing_high_temperature_shutdown,
					one.main_propulsion_motor_nde_bearing_high_temperature_alarm,
					one.main_propulsion_motor_nde_bearing_high_temperature_slowdown,
					one.main_propulsion_motor_nde_bearing_high_temperature_shutdown,
					one.main_propulsion_motor_air_inlet_high_temperature_alarm,
					one.main_propulsion_motor_air_outlet_high_temperature_alarm,
					one.main_propulsion_motor_high_temrpeature_slowdown_pre_alarm,
					one.main_propulsion_motor_high_temrpeature_shutdown_pre_alarm,
					one.main_propulsion_motor_de_bearing_high_temp_slowdown_pre_alarm,
					one.main_propulsion_motor_de_bearing_high_tem_shutdown_pre_alarm,
					one.main_propulsion_motor_nde_bearing_high_temp_slowdown_pre_alarm,
					one.main_propulsion_motor_nde_bearing_high_tem_shutdown_pre_alarm,
					one.main_propulsion_emergency_control_mode,
					one.main_propulsion_shaft_lock_indication,
					one.main_propulsion_vfd_ready,
					one.main_propulsion_vfd_remote_control_position,
					one.main_propulsion_vfd_running,
					one.main_propulsion_vfd_override,
					one.main_propulsion_vfd_torque_limitation);


	sgo_log_stderr(0,"main_propulsion[%d]: main_propulsion_speed_control_nfu_mode:%d,\n"
							"main_propulsion_vfd_start_interlock:%d,\n"
							"main_propulsion_vfd_control_mode:%d,\n"
							"main_propulsion_steering_control_nfu_mode:%d,\n"
							"azimuth_thruster_vfd1_running:%d,\n"
							"azimuth_thruster_vfd2_running:%d,\n"
							"azimuth_thruster_ready:%d,\n"
							"main_propulsion_joystick_control_permission:%d,\n"
							"main_propulsion_ap_control_permission:%d,\n"
							"azimuth_thruster_main_power_failure:%d,\n"
							"azimuth_thruster_backup_power_failure:%d,\n"
							"azimuth_thruster_shaft_lock_inlet_air_pressure_low_alarm:%d,\n"
							"azimuth_thruster_lub_oil_level_low_alarm:%d,\n"
							"azimuth_thruster_lub_oil_temp_high_alarm:%d,\n"
							"azimuth_thruster_lub_oil_filter_blocked_alarm:%d,\n"
							"azimuth_thruster_lub_oil_press_low_alarm:%d,\n"
							"azimuth_thruster_sealing_oil_tank_level_low_alarm:%d,\n"
							"azimuth_thruster_vfd1_fault:%d,\n"
							"azimuth_thruster_vfd2_fault:%d,\n"
							"azimuth_thruster_fu_steering_fault:%d,\n"
							"azimuth_thruster_lub_motor1_overload_alarm:%d,\n"
							"azimuth_thruster_lub_motor2_overload_alarm:%d,\n"
							"azimuth_thruster_angle_order_lost_alarm:%d,\n"
							"azimuth_thruster_angle_feedback_lost_alarm:%d,\n"
							"azimuth_thruster_steering_power1_supply_fault:%d,\n"
							"azimuth_thruster_steering_power2_supply_fault:%d,\n"
							"azimuth_thruster_sealing_oil_inlet_air_press_low_alarm:%d,\n"
							"azimuth_thruster_sealing_oil_inlet_air_press_high_alarm:%d,\n"
							"azimuth_thruster_steering_motor1_temp_high_alarm:%d,\n"
							"azimuth_thruster_steering_motor2_temp_high_alarm:%d",	
					one.no,
					one.main_propulsion_speed_control_nfu_mode,
					one.main_propulsion_vfd_start_interlock,
					one.main_propulsion_vfd_control_mode,
					one.main_propulsion_steering_control_nfu_mode,
					one.azimuth_thruster_vfd1_running,
					one.azimuth_thruster_vfd2_running,
					one.azimuth_thruster_ready,
					one.main_propulsion_joystick_control_permission,
					one.main_propulsion_ap_control_permission,
					one.azimuth_thruster_main_power_failure,
					one.azimuth_thruster_backup_power_failure,
					one.azimuth_thruster_shaft_lock_inlet_air_pressure_low_alarm,
					one.azimuth_thruster_lub_oil_level_low_alarm,
					one.azimuth_thruster_lub_oil_temp_high_alarm,
					one.azimuth_thruster_lub_oil_filter_blocked_alarm,
					one.azimuth_thruster_lub_oil_press_low_alarm,
					one.azimuth_thruster_sealing_oil_tank_level_low_alarm,
					one.azimuth_thruster_vfd1_fault,
					one.azimuth_thruster_vfd2_fault,
					one.azimuth_thruster_fu_steering_fault,
					one.azimuth_thruster_lub_motor1_overload_alarm,
					one.azimuth_thruster_lub_motor2_overload_alarm,
					one.azimuth_thruster_angle_order_lost_alarm,
					one.azimuth_thruster_angle_feedback_lost_alarm,
					one.azimuth_thruster_steering_power1_supply_fault,
					one.azimuth_thruster_steering_power2_supply_fault,
					one.azimuth_thruster_sealing_oil_inlet_air_press_low_alarm,
					one.azimuth_thruster_sealing_oil_inlet_air_press_high_alarm,
					one.azimuth_thruster_steering_motor1_temp_high_alarm,
					one.azimuth_thruster_steering_motor2_temp_high_alarm);
	}
#endif
}

void FILELOG_bow_thruster(const bow_thruster &one)
{
#ifdef DEBUG
		static int cnt0 = 0,cnt1 = 0;
		if(one.no == 0	&& cnt0++ % PRINT_INTERVAL == 0 
			|| one.no == 1	&& cnt1++ % PRINT_INTERVAL == 0)
		{			
			sgo_log_stderr(0,"bow_thruster[%d]:	b_t_remote_control_system_ac_power_failure:%d,\n"
					"b_t_remote_control_system_dc_power_failure:%d,\n"
					"b_t_remote_control_system_failure:%d,\n"
					"b_t_main_motor_overload:%d,\n"
					"b_t_main_motor_high_temperature:%d,\n"
					"b_t_oil_low_level:%d,\n"
					"b_t_oil_low_pressure:%d,\n"
					"b_t_oil_high_pressure:%d,\n"
					"b_t_main_motor_short_circuit:%d,\n"
					"b_t_emergency_stop_circuit_failure:%d,\n"
					"b_t_too_many_times_start:%d,\n"
					"b_t_integrated_alarm:%d",
				one.no,
				one.b_t_remote_control_system_ac_power_failure,
				one.b_t_remote_control_system_dc_power_failure,
				one.b_t_remote_control_system_failure,
				one.b_t_main_motor_overload,
				one.b_t_main_motor_high_temperature,
				one.b_t_oil_low_level,
				one.b_t_oil_low_pressure,
				one.b_t_oil_high_pressure,
				one.b_t_main_motor_short_circuit,
				one.b_t_emergency_stop_circuit_failure,
				one.b_t_too_many_times_start,
				one.b_t_integrated_alarm);

		}
#endif
}










void FILELOG_power_system(const power_system &one)
{
#ifdef DEBUG
	static int cnt = 0;
	if(cnt++ % PRINT_INTERVAL == 0)
	;
#endif
}

void FILELOG_general_charger_and_discharger_panel(const general_charger_and_discharger_panel &one)
{
#ifdef DEBUG
	static int cnt = 0;
	if(cnt++ % PRINT_INTERVAL == 0)
		sgo_log_stderr(0,"general_charger_and_discharger_panel :"
								"low_insulation:%d,\n"
								"rectifier_fault:%d,\n"
								"charger_fault:%d,\n"
								"inverter_failure:%d,\n"
								"battery_protector_active:%d,\n"
								"battery_discharge:%d,\n"
								"battery_low_voltage:%d,\n"
								"inverter_low_voltage:%d",
			one.low_insulation,
			one.rectifier_fault,
			one.charger_fault,
			one.inverter_failure,
			one.battery_protector_active,
			one.battery_discharge,
			one.battery_low_voltage,
			one.inverter_low_voltage);
#endif
}



void FILELOG_e_r_charger_and_discharger_panel(const e_r_charger_and_discharger_panel &one)
{
#ifdef DEBUG
	static int cnt = 0;
	if(cnt++ % PRINT_INTERVAL == 0)
		sgo_log_stderr(0,"e_r_charger_and_discharger_panel :"
					"ups_low_insulation:%d,\n"
					"ups_battery_low_voltage:%d,\n"
					"ups_input_fault:%d,\n"
					"ups_battery_bypass_activation:%d,\n"
					"ups_common_alarm:%d,\n"
					"ups_alarm:%d,\n"
					"ups_running:%d,\n"
					"ups_battery_mode:%d,\n"
					"ups_battery_protection_trip:%d,\n"
					"ups_output_alarm:%d\n",
			one.ups_low_insulation,
			one.ups_battery_low_voltage,
			one.ups_input_fault,
			one.ups_battery_bypass_activation,
			one.ups_common_alarm,
			one.ups_alarm,
			one.ups_running,
			one.ups_battery_mode,
			one.ups_battery_protection_trip,
			one.ups_output_alarm);


#endif

}


void FILELOG_main_transformer(const main_transformer &one)
{
#ifdef DEBUG
	static int cnt0 = 0,cnt1 = 0;
	if(one.no == 0	&& cnt0++ % PRINT_INTERVAL == 0 
		|| one.no == 1	&& cnt1++ % PRINT_INTERVAL == 0)

	{
		sgo_log_stderr(0,"main_transformer[%d]:	main_transformer_u_winding_temp:%.2f,\n"
				"main_transformer_v_winding_temp:%.2f,\n"
				"main_transformer_w_winding_temp:%.2f",
			one.no,
			one.main_transformer_u_winding_temp,
			one.main_transformer_v_winding_temp,
			one.main_transformer_w_winding_temp);
	}
#endif
}

void FILELOG_level_gauging(const level_gauging &one)
{
#ifdef DEBUG
	static int cnt = 0;
	if(cnt++ % PRINT_INTERVAL == 0)
		sgo_log_stderr(0,"level_gauging:forep_draft:%.3f,\n"
							"fores_draft:%.3f,\n"
							"aftp_draft:%.3f,\n"
							"afts_draft:%.3f,\n"
							"sw_ballast_tank1p_level:%.3f,\n"
							"sw_ballast_tank1s_level:%.3f,\n"
							"sw_ballast_tank2p_level:%.3f,\n"
							"sw_ballast_tank2s_level:%.3f,\n"
							"sw_ballast_tank3p_level:%.3f,\n"
							"sw_ballast_tank3s_level:%.3f,\n"
							"sw_ballast_tank3m_level:%.3f,\n"
							"fresh_water_tank1p_level:%.3f,\n"
							"fresh_water_tank1s_level:%.3f,\n"
							"fresh_water_tank2p_level:%.3f,\n"
							"fresh_water_tank2s_level:%.3f,\n"
							"fuel_oil_store_tank_p_level:%.3f,\n"
							"fuel_oil_store_tank_s_level:%.3f,\n"
							"fuel_oil_day_tank_p_level:%.3f,\n"
							"fuel_oil_day_tank_s_level:%.3f",
				one.forep_draft,
				one.fores_draft,
				one.aftp_draft,
				one.afts_draft,
				one.sw_ballast_tank1p_level,
				one.sw_ballast_tank1s_level,
				one.sw_ballast_tank2p_level,
				one.sw_ballast_tank2s_level,
				one.sw_ballast_tank3p_level,
				one.sw_ballast_tank3s_level,
				one.sw_ballast_tank3m_level,
				one.fresh_water_tank1p_level,
				one.fresh_water_tank1s_level,
				one.fresh_water_tank2p_level,
				one.fresh_water_tank2s_level,
				one.fuel_oil_store_tank_p_level,
				one.fuel_oil_store_tank_s_level,
				one.fuel_oil_day_tank_p_level,
				one.fuel_oil_day_tank_s_level);
#endif
}

void FILELOG_level_switch(const level_switch &one)
{
#ifdef DEBUG
	static int cnt = 0;
	if(cnt++ % PRINT_INTERVAL == 0)
		sgo_log_stderr(0,"level_switch:fuel_oil_store_tank_p_high_level_alarm:%d,\n"
							"fuel_oil_store_tank_p_low_level_alarm:%d,\n"
							"fuel_oil_day_tank_p_high_level_alarm:%d,\n"
							"fuel_oil_day_tank_p_low_level_alarm:%d,\n"
							"fuel_oil_store_tank_s_high_level_alarm:%d,\n"
							"fuel_oil_store_tank_s_low_level_alarm:%d,\n"
							"fuel_oil_day_tank_s_high_level_alarm:%d,\n"
							"fuel_oil_day_tank_s_low_level_alarm:%d,\n"
							"lube_oil_tank_high_level_alarm:%d,\n"
							"dirty_oil_tank_high_level_alarm:%d,\n"
							"black_water_tank_high_level_alarm:%d,\n"
							"grey_water_tank_high_level_alarm:%d,\n"
							"e_g_m_d_o_service_tank_high_level_alarm:%d,\n"
							"e_g_m_d_o_service_tk_low_level_alarm:%d,\n"
							"spill_tank_high_level_alarm:%d,\n"
							"b_t_room_bilge_high_level_alarm:%d,\n"
							"em_cy_fire_fighting_pump_room_bilge_high_level_alarm:%d,\n"
							"no1_void_p_tank_bilge_high_level_alarm:%d,\n"
							"no2_void_p_tank_bilge_high_level_alarm:%d,\n"
							"no2_void_s_tank_bilge_high_level_alarm:%d,\n"
							"no3_void_p_tank_bilge_high_level_alarm:%d,\n"
							"no3_void_s_tank_bilge_high_level_alarm:%d,\n"
							"no4_void_p_tank_bilge_high_level_alarm:%d,\n"
							"no4_void_s_tank_bilge_high_level_alarm:%d,\n"
							"no5_void_p_tank_bilge_high_level_alarm:%d,\n"
							"no5_void_s_tank_bilge_high_level_alarm:%d,\n"
							"no6_void_p_tank_bilge_high_level_alarm:%d,\n"
							"no6_void_s_tank_bilge_high_level_alarm:%d,\n"
							"aux_machinery_room1_bilge_high_level_alarm:%d,\n"
							"aux_machinery_room2_bilge_high_level_alarm:%d,\n"
							"e_r_ps_bilge_high_level_alarm:%d,\n"
							"e_r_sb_bilge_high_level_alarm:%d,\n"
							"aft_thruster_rm_ps_bilge_high_level_alarm:%d,\n"
							"aft_thruster_rm_sb_bilge_high_level_alarm:%d,\n"
							"bilge_water_tank_high_level_alarm:%d,\n"
							"no1_expansion_tank_low_level_alarm:%d,\n"
							"no2_expansion_tank_low_level_alarm:%d,\n"
							"no1_jackhouse_bilge_high_level_alarm:%d,\n"
							"no2_jackhouse_bilge_high_level_alarm:%d,\n"
							"no3_jackhouse_bilge_high_level_alarm:%d,\n"
							"no4_jackhouse_bilge_high_level_alarm:%d,\n"
							"galley_transformer_room_bilge_low_level_alarm:%d,\n"
							"f_o_transfer_pump_room_bilge_low_level_alarm:%d",
					one.fuel_oil_store_tank_p_high_level_alarm,
					one.fuel_oil_store_tank_p_low_level_alarm,
					one.fuel_oil_day_tank_p_high_level_alarm,
					one.fuel_oil_day_tank_p_low_level_alarm,
					one.fuel_oil_store_tank_s_high_level_alarm,
					one.fuel_oil_store_tank_s_low_level_alarm,
					one.fuel_oil_day_tank_s_high_level_alarm,
					one.fuel_oil_day_tank_s_low_level_alarm,
					one.lube_oil_tank_high_level_alarm,
					one.dirty_oil_tank_high_level_alarm,
					one.black_water_tank_high_level_alarm,
					one.grey_water_tank_high_level_alarm,
					one.e_g_m_d_o_service_tank_high_level_alarm,
					one.e_g_m_d_o_service_tk_low_level_alarm,
					one.spill_tank_high_level_alarm,
					one.b_t_room_bilge_high_level_alarm,
					one.em_cy_fire_fighting_pump_room_bilge_high_level_alarm,
					one.no1_void_p_tank_bilge_high_level_alarm,
					one.no2_void_p_tank_bilge_high_level_alarm,
					one.no2_void_s_tank_bilge_high_level_alarm,
					one.no3_void_p_tank_bilge_high_level_alarm,
					one.no3_void_s_tank_bilge_high_level_alarm,
					one.no4_void_p_tank_bilge_high_level_alarm,
					one.no4_void_s_tank_bilge_high_level_alarm,
					one.no5_void_p_tank_bilge_high_level_alarm,
					one.no5_void_s_tank_bilge_high_level_alarm,
					one.no6_void_p_tank_bilge_high_level_alarm,
					one.no6_void_s_tank_bilge_high_level_alarm,
					one.aux_machinery_room1_bilge_high_level_alarm,
					one.aux_machinery_room2_bilge_high_level_alarm,
					one.e_r_ps_bilge_high_level_alarm,
					one.e_r_sb_bilge_high_level_alarm,
					one.aft_thruster_rm_ps_bilge_high_level_alarm,
					one.aft_thruster_rm_sb_bilge_high_level_alarm,
					one.bilge_water_tank_high_level_alarm,
					one.no1_expansion_tank_low_level_alarm,
					one.no2_expansion_tank_low_level_alarm,
					one.no1_jackhouse_bilge_high_level_alarm,
					one.no2_jackhouse_bilge_high_level_alarm,
					one.no3_jackhouse_bilge_high_level_alarm,
					one.no4_jackhouse_bilge_high_level_alarm,
					one.galley_transformer_room_bilge_low_level_alarm,
					one.f_o_transfer_pump_room_bilge_low_level_alarm);

#endif
}










void FILELOG_gas_detection_system(const gas_detection_system &one)
{
#ifdef DEBUG
#ifdef JANA505
		static int cnt = 0;
		if(cnt++ % PRINT_INTERVAL == 0)
		{
			sgo_log_stderr(0,"gas_detection_system : %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d",			
		one.gas_detection_alarm_system_failure,
		one.gas_detector_fault_top_of_wheelhouse,
		one.gas_pre_alarm_top_of_wheelhouse,
		one.gas_level1_alarm_top_of_wheelhouse,
		one.gas_level2_alarm_top_of_wheelhouse,
		one.gas_detector_disabled_top_of_wheelhouse,
		one.gas_detector_fault_air_intake_of_b_deck_ahu,
		one.gas_pre_alarm_air_intake_of_b_deck_ahu,
		one.gas_level1_alarm_air_intake_of_b_deck_ahu,
		one.gas_level2_alarm_air_intake_of_b_deck_ahu,
		one.gas_detector_disabled_air_intake_of_b_deck_ahu,
		one.gas_detector_fault_air_intake_of_a_deck_ahu,
		one.gas_pre_alarm_air_intake_of_a_deck_ahu,
		one.gas_level1_alarm_air_intake_of_a_deck_ahu,
		one.gas_level2_alarm_air_intake_of_a_deck_ahu,
		one.gas_detector_disabled_air_intake_of_a_deck_ahu,
		one.gas_detector_fault_air_intake_of_below_main_deck_ahu,
		one.gas_pre_alarm_air_intake_of_below_main_deck_ahu,
		one.gas_level1_alarm_air_intake_of_below_main_deck_ahu,
		one.gas_level2_alarm_air_intake_of_below_main_deck_ahu,
		one.gas_detector_disabled_air_intake_of_below_main_deck_ahu,
		one.gas_detector_fault_air_intake_of_bow_thruster_room,
		one.gas_pre_alarm_air_intake_of_bow_thruster_room,
		one.gas_level1_alarm_air_intake_of_bow_thruster_room,
		one.gas_level2_alarm_air_intake_of_bow_thruster_room,
		one.gas_detector_disabled_air_intake_of_bow_thruster_room,
		one.gas_detector_fault_air_intake_of_aft_thruster_roomp,
		one.gas_pre_alarm_air_intake_of_aft_thruster_roomp,
		one.gas_level1_alarm_air_intake_of_aft_thruster_roomp,
		one.gas_level2_alarm_air_intake_of_aft_thruster_roomp,
		one.gas_detector_disabled_air_intake_of_aft_thruster_roomp,
		one.gas_detector_fault_air_intake_of_aft_thruster_rooms,
		one.gas_pre_alarm_air_intake_of_aft_thruster_rooms,
		one.gas_level1_alarm_air_intake_of_aft_thruster_rooms,
		one.gas_level2_alarm_air_intake_of_aft_thruster_rooms,
		one.gas_detector_disabled_air_intake_of_aft_thruster_rooms,
		one.gas_detector_fault_air_intake_of_engine_roomp,
		one.gas_pre_alarm_air_intake_of_engine_roomp,
		one.gas_level1_alarm_air_intake_of_engine_roomp,
		one.gas_level2_alarm_air_intake_of_engine_roomp,
		one.gas_detector_disabled_air_intake_of_engine_roomp,
		one.gas_detector_fault_air_intake_of_engine_rooms,
		one.gas_pre_alarm_air_intake_of_engine_rooms,
		one.gas_level1_alarm_air_intake_of_engine_rooms,
		one.gas_level2_alarm_air_intake_of_engine_rooms,
		one.gas_detector_disabled_air_intake_of_engine_rooms,
		one.gas_detector_fault_air_intake_of_aux_machinery_room1,
		one.gas_pre_alarm_air_intake_of_aux_machinery_room1,
		one.gas_level1_alarm_air_intake_of_aux_machinery_room1,
		one.gas_level2_alarm_air_intake_of_aux_machinery_room1,
		one.gas_detector_disabled_air_intake_of_aux_machinery_room1,
		one.gas_detector_fault_air_intake_of_aux_machinery_room2,
		one.gas_pre_alarm_air_intake_of_aux_machinery_room2,
		one.gas_level1_alarm_air_intake_of_aux_machinery_room2,
		one.gas_level2_alarm_air_intake_of_aux_machinery_room2,
		one.gas_detector_disabled_air_intake_of_aux_machinery_room2,
		one.gas_detector_fault_b_deck_entrancefr109p,
		one.gas_pre_alarm_b_deck_entrancefr109p,
		one.gas_level1_alarm_b_deck_entrancefr109p,
		one.gas_level2_alarm_b_deck_entrancefr109p,
		one.gas_detector_disabled_b_deck_entrancefr109p,
		one.gas_detector_fault_b_deck_entrancefr109s,
		one.gas_pre_alarm_b_deck_entrancefr109s,
		one.gas_level1_alarm_b_deck_entrancefr109s,
		one.gas_level2_alarm_b_deck_entrancefr109s,
		one.gas_detector_disabled_b_deck_entrancefr109s,
		one.gas_detector_fault_wheelhouse_entrancep,
		one.gas_pre_alarm_wheelhouse_entrancep,
		one.gas_level1_alarm_wheelhouse_entrancep,
		one.gas_level2_alarm_wheelhouse_entrancep,
		one.gas_detector_disabled_wheelhouse_entrancep,
		one.gas_detector_fault_wheelhouse_entrances,
		one.gas_pre_alarm_wheelhouse_entrances,
		one.gas_level1_alarm_wheelhouse_entrances,
		one.gas_level2_alarm_wheelhouse_entrances,
		one.gas_detector_disabled_wheelhouse_entrances,
		one.gas_detector_fault_b_deck_entrancefr90p,
		one.gas_pre_alarm_b_deck_entrancefr90p,
		one.gas_level1_alarm_b_deck_entrancefr90p,
		one.gas_level2_alarm_b_deck_entrancefr90p,
		one.gas_detector_disabled_b_deck_entrancefr90p,
		one.gas_detector_fault_b_deck_entrancefr90s,
		one.gas_pre_alarm_b_deck_entrancefr90s,
		one.gas_level1_alarm_b_deck_entrancefr90s,
		one.gas_level2_alarm_b_deck_entrancefr90s,
		one.gas_detector_disabled_b_deck_entrancefr90s,
		one.gas_detector_fault_a_deck_entrancefr109p,
		one.gas_pre_alarm_a_deck_entrancefr109p,
		one.gas_level1_alarm_a_deck_entrancefr109p,
		one.gas_level2_alarm_a_deck_entrancefr109p,
		one.gas_detector_disabled_a_deck_entrancefr109p,
		one.gas_detector_fault_a_deck_entrancefr109s,
		one.gas_pre_alarm_a_deck_entrancefr109s,
		one.gas_level1_alarm_a_deck_entrancefr109s,
		one.gas_level2_alarm_a_deck_entrancefr109s,
		one.gas_detector_disabled_a_deck_entrancefr109s,
		one.gas_detector_fault_a_deck_entrancefr90p,
		one.gas_pre_alarm_a_deck_entrancefr90p,
		one.gas_level1_alarm_a_deck_entrancefr90p,
		one.gas_level2_alarm_a_deck_entrancefr90p,
		one.gas_detector_disabled_a_deck_entrancefr90p,
		one.gas_detector_fault_a_deck_entrancefr90s,
		one.gas_pre_alarm_a_deck_entrancefr90s,
		one.gas_level1_alarm_a_deck_entrancefr90s,
		one.gas_level2_alarm_a_deck_entrancefr90sp,
		one.gas_detector_disabled_a_deck_entrancefr90s,
		one.gas_detector_fault_main_deck_entrancefr81m,
		one.gas_pre_alarm_main_deck_entrancefr81m,
		one.gas_level1_alarm_main_deck_entrancefr81m,
		one.gas_level2_alarm_main_deck_entrancefr81m,
		one.gas_detector_disabled_main_deck_entrancefr81m,
		one.gas_detector_fault_main_deck_entrancefr81s,
		one.gas_pre_alarm_main_deck_entrancefr81s,
		one.gas_level1_alarm_main_deck_entrancefr81s,
		one.gas_level2_alarm_main_deck_entrancefr81s,
		one.gas_detector_disabled_main_deck_entrancefr81s,
		one.gas_detector_fault_air_intake_of_construction_equipment_room,
		one.gas_pre_alarm_air_intake_of_construction_equipment_room,
		one.gas_level1_alarm_air_intake_of_construction_equipment_room,
		one.gas_level2_alarm_air_intake_of_construction_equipment_room,
		one.gas_detector_disabled_air_intake_of_construction_equipment_room,
		one.toxic_gas_detector_fault_top_of_wheelhouse,
		one.toxic_gas_pre_alarm_top_of_wheelhouse,
		one.toxic_gas_level1_alarm_top_of_wheelhouse,
		one.toxic_gas_level2_alarm_top_of_wheelhouse,
		one.toxic_gas_detector_disabled_top_of_wheelhouse,
		one.toxic_gas_detector_fault_air_intake_of_b_deck_ahu,
		one.toxic_gas_pre_alarm_air_intake_of_b_deck_ahu,
		one.toxic_gas_level1_alarm_air_intake_of_b_deck_ahu,
		one.toxic_gas_level2_alarm_air_intake_of_b_deck_ahu,
		one.toxic_gas_detector_disabled_air_intake_of_b_deck_ahu,
		one.toxic_gas_detector_fault_air_intake_of_a_deck_ahu,
		one.toxic_gas_pre_alarm_air_intake_of_a_deck_ahu,
		one.toxic_gas_level1_alarm_air_intake_of_a_deck_ahu,
		one.toxic_gas_level2_alarm_air_intake_of_a_deck_ahu,
		one.toxic_gas_detector_disabled_air_intake_of_a_deck_ahu,
		one.toxic_gas_detector_fault_air_intake_of_below_main_deck_ahu,
		one.toxic_gas_pre_alarm_air_intake_of_below_main_deck_ahu,
		one.toxic_gas_level1_alarm_air_intake_of_below_main_deck_ahu,
		one.toxic_gas_level2_alarm_air_intake_of_below_main_deck_ahu,
		one.toxic_gas_detector_disabled_air_intake_of_below_main_deck_ahu,
		one.toxic_gas_detector_fault_air_intake_of_bow_thruster_room,
		one.toxic_gas_pre_alarm_air_intake_of_bow_thruster_room,
		one.toxic_gas_level1_alarm_air_intake_of_bow_thruster_room,
		one.toxic_gas_level2_alarm_air_intake_of_bow_thruster_room,
		one.toxic_gas_detector_disabled_air_intake_of_bow_thruster_room,
		one.toxic_gas_detector_fault_air_intake_of_aft_thruster_roomp,
		one.toxic_gas_pre_alarm_air_intake_of_aft_thruster_roomp,
		one.toxic_gas_level1_alarm_air_intake_of_aft_thruster_roomp,
		one.toxic_gas_level2_alarm_air_intake_of_aft_thruster_roomp,
		one.toxic_gas_detector_disabled_air_intake_of_aft_thruster_roomp,
		one.toxic_gas_detector_fault_air_intake_of_aft_thruster_rooms,
		one.toxic_gas_pre_alarm_air_intake_of_aft_thruster_rooms,
		one.toxic_gas_level1_alarm_air_intake_of_aft_thruster_rooms,
		one.toxic_gas_level2_alarm_air_intake_of_aft_thruster_rooms,
		one.toxic_gas_detector_disabled_air_intake_of_aft_thruster_rooms,
		one.toxic_gas_detector_fault_air_intake_of_engine_roomp,
		one.toxic_gas_pre_alarm_air_intake_of_engine_roomp,
		one.toxic_gas_level1_alarm_air_intake_of_engine_roomp,
		one.toxic_gas_level2_alarm_air_intake_of_engine_roomp,
		one.toxic_gas_detector_disabled_air_intake_of_engine_roomp,
		one.toxic_gas_detector_fault_air_intake_of_engine_rooms,
		one.toxic_gas_pre_alarm_air_intake_of_engine_rooms,
		one.toxic_gas_level1_alarm_air_intake_of_engine_rooms,
		one.toxic_gas_level2_alarm_air_intake_of_engine_rooms,
		one.toxic_gas_detector_disabled_air_intake_of_engine_rooms,
		one.toxic_gas_detector_fault_air_intake_of_aux_machinery_room1,
		one.toxic_gas_pre_alarm_air_intake_of_aux_machinery_room1,
		one.toxic_gas_level1_alarm_air_intake_of_aux_machinery_room1,
		one.toxic_gas_level2_alarm_air_intake_of_aux_machinery_room1,
		one.toxic_gas_detector_disabled_air_intake_of_aux_machinery_room1,
		one.toxic_gas_detector_fault_air_intake_of_aux_machinery_room2,
		one.toxic_gas_pre_alarm_air_intake_of_aux_machinery_room2,
		one.toxic_gas_level1_alarm_air_intake_of_aux_machinery_room2,
		one.toxic_gas_level2_alarm_air_intake_of_aux_machinery_room2,
		one.toxic_gas_detector_disabled_air_intake_of_aux_machinery_room2,
		one.toxic_gas_detector_fault_b_deck_entrancefr109p,
		one.toxic_gas_pre_alarm_b_deck_entrancefr109p,
		one.toxic_gas_level1_alarm_b_deck_entrancefr109p,
		one.toxic_gas_level2_alarm_b_deck_entrancefr109p,
		one.toxic_gas_detector_disabled_b_deck_entrancefr109p,
		one.toxic_gas_detector_fault_b_deck_entrancefr109s,
		one.toxic_gas_pre_alarm_b_deck_entrancefr109s,
		one.toxic_gas_level1_alarm_b_deck_entrancefr109s,
		one.toxic_gas_level2_alarm_b_deck_entrancefr109s,
		one.toxic_gas_detector_disabled_b_deck_entrancefr109s);
	}
#endif
#ifdef JANA508509
		static int cnt = 0;
		if(cnt++ % PRINT_INTERVAL == 0)
		{
			sgo_log_stderr(0,"gas_detection_system : %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n"
				"%d",			
		one.gas_detection_alarm_system_failure,
		one.gas_detector_fault_top_of_wheelhouse,
		one.gas_pre_alarm_top_of_wheelhouse,
		one.gas_level1_alarm_top_of_wheelhouse,
		one.gas_level2_alarm_top_of_wheelhouse,
		one.gas_detector_disabled_top_of_wheelhouse,
		one.gas_detector_fault_air_intake_of_b_deck_ahu,
		one.gas_pre_alarm_air_intake_of_b_deck_ahu,
		one.gas_level1_alarm_air_intake_of_b_deck_ahu,
		one.gas_level2_alarm_air_intake_of_b_deck_ahu,
		one.gas_detector_disabled_air_intake_of_b_deck_ahu,
		one.gas_detector_fault_air_intake_of_a_deck_ahu,
		one.gas_pre_alarm_air_intake_of_a_deck_ahu,
		one.gas_level1_alarm_air_intake_of_a_deck_ahu,
		one.gas_level2_alarm_air_intake_of_a_deck_ahu,
		one.gas_detector_disabled_air_intake_of_a_deck_ahu,
		one.gas_detector_fault_air_intake_of_below_main_deck_ahu,
		one.gas_pre_alarm_air_intake_of_below_main_deck_ahu,
		one.gas_level1_alarm_air_intake_of_below_main_deck_ahu,
		one.gas_level2_alarm_air_intake_of_below_main_deck_ahu,
		one.gas_detector_disabled_air_intake_of_below_main_deck_ahu,
		one.gas_detector_fault_air_intake_of_bow_thruster_room,
		one.gas_pre_alarm_air_intake_of_bow_thruster_room,
		one.gas_level1_alarm_air_intake_of_bow_thruster_room,
		one.gas_level2_alarm_air_intake_of_bow_thruster_room,
		one.gas_detector_disabled_air_intake_of_bow_thruster_room,
		one.gas_detector_fault_air_intake_of_aft_thruster_roomp,
		one.gas_pre_alarm_air_intake_of_aft_thruster_roomp,
		one.gas_level1_alarm_air_intake_of_aft_thruster_roomp,
		one.gas_level2_alarm_air_intake_of_aft_thruster_roomp,
		one.gas_detector_disabled_air_intake_of_aft_thruster_roomp,
		one.gas_detector_fault_air_intake_of_aft_thruster_rooms,
		one.gas_pre_alarm_air_intake_of_aft_thruster_rooms,
		one.gas_level1_alarm_air_intake_of_aft_thruster_rooms,
		one.gas_level2_alarm_air_intake_of_aft_thruster_rooms,
		one.gas_detector_disabled_air_intake_of_aft_thruster_rooms,
		one.gas_detector_fault_air_intake_of_engine_roomp,
		one.gas_pre_alarm_air_intake_of_engine_roomp,
		one.gas_level1_alarm_air_intake_of_engine_roomp,
		one.gas_level2_alarm_air_intake_of_engine_roomp,
		one.gas_detector_disabled_air_intake_of_engine_roomp,
		one.gas_detector_fault_air_intake_of_engine_rooms,
		one.gas_pre_alarm_air_intake_of_engine_rooms,
		one.gas_level1_alarm_air_intake_of_engine_rooms,
		one.gas_level2_alarm_air_intake_of_engine_rooms,
		one.gas_detector_disabled_air_intake_of_engine_rooms,
		one.gas_detector_fault_air_intake_of_aux_machinery_room1,
		one.gas_pre_alarm_air_intake_of_aux_machinery_room1,
		one.gas_level1_alarm_air_intake_of_aux_machinery_room1,
		one.gas_level2_alarm_air_intake_of_aux_machinery_room1,
		one.gas_detector_disabled_air_intake_of_aux_machinery_room1,
		one.gas_detector_fault_air_intake_of_aux_machinery_room2,
		one.gas_pre_alarm_air_intake_of_aux_machinery_room2,
		one.gas_level1_alarm_air_intake_of_aux_machinery_room2,
		one.gas_level2_alarm_air_intake_of_aux_machinery_room2,
		one.gas_detector_disabled_air_intake_of_aux_machinery_room2,
		one.gas_detector_fault_b_deck_entrancefr109p,
		one.gas_pre_alarm_b_deck_entrancefr109p,
		one.gas_level1_alarm_b_deck_entrancefr109p,
		one.gas_level2_alarm_b_deck_entrancefr109p,
		one.gas_detector_disabled_b_deck_entrancefr109p,
		one.gas_detector_fault_b_deck_entrancefr109s,
		one.gas_pre_alarm_b_deck_entrancefr109s,
		one.gas_level1_alarm_b_deck_entrancefr109s,
		one.gas_level2_alarm_b_deck_entrancefr109s,
		one.gas_detector_disabled_b_deck_entrancefr109s,
		one.gas_detector_fault_wheelhouse_entrancep,
		one.gas_pre_alarm_wheelhouse_entrancep,
		one.gas_level1_alarm_wheelhouse_entrancep,
		one.gas_level2_alarm_wheelhouse_entrancep,
		one.gas_detector_disabled_wheelhouse_entrancep,
		one.gas_detector_fault_wheelhouse_entrances,
		one.gas_pre_alarm_wheelhouse_entrances,
		one.gas_level1_alarm_wheelhouse_entrances,
		one.gas_level2_alarm_wheelhouse_entrances,
		one.gas_detector_disabled_wheelhouse_entrances,
		one.gas_detector_fault_b_deck_entrancefr100p,
		one.gas_pre_alarm_b_deck_entrancefr100p,
		one.gas_level1_alarm_b_deck_entrancefr100p,
		one.gas_level2_alarm_b_deck_entrancefr100p,
		one.gas_detector_disabled_b_deck_entrancefr100p,
		one.gas_detector_fault_b_deck_entrancefr100s,
		one.gas_pre_alarm_b_deck_entrancefr100s,
		one.gas_level1_alarm_b_deck_entrancefr100s,
		one.gas_level2_alarm_b_deck_entrancefr100s,
		one.gas_detector_disabled_b_deck_entrancefr100s,
		one.gas_detector_fault_a_deck_entrancefr109p,
		one.gas_pre_alarm_a_deck_entrancefr109p,
		one.gas_level1_alarm_a_deck_entrancefr109p,
		one.gas_level2_alarm_a_deck_entrancefr109p,
		one.gas_detector_disabled_a_deck_entrancefr109p,
		one.gas_detector_fault_a_deck_entrancefr109s,
		one.gas_pre_alarm_a_deck_entrancefr109s,
		one.gas_level1_alarm_a_deck_entrancefr109s,
		one.gas_level2_alarm_a_deck_entrancefr109s,
		one.gas_detector_disabled_a_deck_entrancefr109s,
		one.gas_detector_fault_a_deck_entrancefr91p,
		one.gas_pre_alarm_a_deck_entrancefr91p,
		one.gas_level1_alarm_a_deck_entrancefr91p,
		one.gas_level2_alarm_a_deck_entrancefr91p,
		one.gas_detector_disabled_a_deck_entrancefr91p,
		one.gas_detector_fault_a_deck_entrancefr91s,
		one.gas_pre_alarm_a_deck_entrancefr91s,
		one.gas_level1_alarm_a_deck_entrancefr91s,
		one.gas_level2_alarm_a_deck_entrancefr91sp,
		one.gas_detector_disabled_a_deck_entrancefr91s,
		one.gas_detector_fault_main_deck_entrancefr91m,
		one.gas_pre_alarm_main_deck_entrancefr91m,
		one.gas_level1_alarm_main_deck_entrancefr91m,
		one.gas_level2_alarm_main_deck_entrancefr91m,
		one.gas_detector_disabled_main_deck_entrancefr91m,
		one.gas_detector_fault_main_deck_entrancefr91s,
		one.gas_pre_alarm_main_deck_entrancefr91s,
		one.gas_level1_alarm_main_deck_entrancefr91s,
		one.gas_level2_alarm_main_deck_entrancefr91s,
		one.gas_detector_disabled_main_deck_entrancefr91s,
		one.toxic_gas_detector_fault_top_of_wheelhouse,
		one.toxic_gas_pre_alarm_top_of_wheelhouse,
		one.toxic_gas_level1_alarm_top_of_wheelhouse,
		one.toxic_gas_level2_alarm_top_of_wheelhouse,
		one.toxic_gas_detector_disabled_top_of_wheelhouse,
		one.toxic_gas_detector_fault_air_intake_of_b_deck_ahu,
		one.toxic_gas_pre_alarm_air_intake_of_b_deck_ahu,
		one.toxic_gas_level1_alarm_air_intake_of_b_deck_ahu,
		one.toxic_gas_level2_alarm_air_intake_of_b_deck_ahu,
		one.toxic_gas_detector_disabled_air_intake_of_b_deck_ahu,
		one.toxic_gas_detector_fault_air_intake_of_a_deck_ahu,
		one.toxic_gas_pre_alarm_air_intake_of_a_deck_ahu,
		one.toxic_gas_level1_alarm_air_intake_of_a_deck_ahu,
		one.toxic_gas_level2_alarm_air_intake_of_a_deck_ahu,
		one.toxic_gas_detector_disabled_air_intake_of_a_deck_ahu,
		one.toxic_gas_detector_fault_air_intake_of_below_main_deck_ahu,
		one.toxic_gas_pre_alarm_air_intake_of_below_main_deck_ahu,
		one.toxic_gas_level1_alarm_air_intake_of_below_main_deck_ahu,
		one.toxic_gas_level2_alarm_air_intake_of_below_main_deck_ahu,
		one.toxic_gas_detector_disabled_air_intake_of_below_main_deck_ahu,
		one.toxic_gas_detector_fault_air_intake_of_bow_thruster_room,
		one.toxic_gas_pre_alarm_air_intake_of_bow_thruster_room,
		one.toxic_gas_level1_alarm_air_intake_of_bow_thruster_room,
		one.toxic_gas_level2_alarm_air_intake_of_bow_thruster_room,
		one.toxic_gas_detector_disabled_air_intake_of_bow_thruster_room,
		one.toxic_gas_detector_fault_air_intake_of_aft_thruster_roomp,
		one.toxic_gas_pre_alarm_air_intake_of_aft_thruster_roomp,
		one.toxic_gas_level1_alarm_air_intake_of_aft_thruster_roomp,
		one.toxic_gas_level2_alarm_air_intake_of_aft_thruster_roomp,
		one.toxic_gas_detector_disabled_air_intake_of_aft_thruster_roomp,
		one.toxic_gas_detector_fault_air_intake_of_aft_thruster_rooms,
		one.toxic_gas_pre_alarm_air_intake_of_aft_thruster_rooms,
		one.toxic_gas_level1_alarm_air_intake_of_aft_thruster_rooms,
		one.toxic_gas_level2_alarm_air_intake_of_aft_thruster_rooms,
		one.toxic_gas_detector_disabled_air_intake_of_aft_thruster_rooms,
		one.toxic_gas_detector_fault_air_intake_of_engine_roomp,
		one.toxic_gas_pre_alarm_air_intake_of_engine_roomp,
		one.toxic_gas_level1_alarm_air_intake_of_engine_roomp,
		one.toxic_gas_level2_alarm_air_intake_of_engine_roomp,
		one.toxic_gas_detector_disabled_air_intake_of_engine_roomp,
		one.toxic_gas_detector_fault_air_intake_of_engine_rooms,
		one.toxic_gas_pre_alarm_air_intake_of_engine_rooms,
		one.toxic_gas_level1_alarm_air_intake_of_engine_rooms,
		one.toxic_gas_level2_alarm_air_intake_of_engine_rooms,
		one.toxic_gas_detector_disabled_air_intake_of_engine_rooms,
		one.toxic_gas_detector_fault_air_intake_of_aux_machinery_room1,
		one.toxic_gas_pre_alarm_air_intake_of_aux_machinery_room1,
		one.toxic_gas_level1_alarm_air_intake_of_aux_machinery_room1,
		one.toxic_gas_level2_alarm_air_intake_of_aux_machinery_room1,
		one.toxic_gas_detector_disabled_air_intake_of_aux_machinery_room1,
		one.toxic_gas_detector_fault_air_intake_of_aux_machinery_room2,
		one.toxic_gas_pre_alarm_air_intake_of_aux_machinery_room2,
		one.toxic_gas_level1_alarm_air_intake_of_aux_machinery_room2,
		one.toxic_gas_level2_alarm_air_intake_of_aux_machinery_room2,
		one.toxic_gas_detector_disabled_air_intake_of_aux_machinery_room2,
		one.toxic_gas_detector_fault_b_deck_entrancefr109p,
		one.toxic_gas_pre_alarm_b_deck_entrancefr109p,
		one.toxic_gas_level1_alarm_b_deck_entrancefr109p,
		one.toxic_gas_level2_alarm_b_deck_entrancefr109p,
		one.toxic_gas_detector_disabled_b_deck_entrancefr109p,
		one.toxic_gas_detector_fault_b_deck_entrancefr109s,
		one.toxic_gas_pre_alarm_b_deck_entrancefr109s,
		one.toxic_gas_level1_alarm_b_deck_entrancefr109s,
		one.toxic_gas_level2_alarm_b_deck_entrancefr109s,
		one.toxic_gas_detector_disabled_b_deck_entrancefr109s);
	}


#endif


#endif

}

void FILELOG_cooling_water_system(const cooling_water_system &one)
{
#ifdef DEBUG
	static int cnt = 0;
	if(cnt++ % PRINT_INTERVAL == 0)
			sgo_log_stderr(0,"cooling_water_system:%.2f,%.2f,%.2f,%.2f",
		one.no1_propulsion_sw_cooling_pump_outlet_pressure,
		one.no2_propulsion_sw_cooling_pump_outlet_pressure,
		one.no1_propulsion_fw_cooling_pump_outlet_pressure,
		one.no2_propulsion_fw_cooling_pump_outlet_pressure);
#endif
}

void FILELOG_air_conditioner_refrigeration_system(const air_conditioner_refrigeration_system &one)
{
#ifdef DEBUG
		static int cnt = 0;
		if(cnt++ % PRINT_INTERVAL == 0)
				sgo_log_stderr(0,"cooling_water_system:no1_wheel_house_air_condition_common_alarm:%d,\n"
						"no2_wheel_house_air_condition_common_alarm:%d,\n"
						"no1_galley_package_air_condition_common_alarm:%d,\n"
						"no2_galley_package_air_condition_common_alarm:%d,\n"
						"m_s_b_room_package_air_condition_common_alarm:%d,\n"
						"no1_jacking_vfd_room_air_condition_common_alarm:%d,\n"
						"no2_jacking_vfd_room_air_condition_common_alarm:%d,\n"
						"deck_storeroom_air_condition_common_alarm:%d,\n"
						"paint_locker_room_air_condition_common_alarm:%d,\n"
						"workshop_air_condition_common_alarm:%d,\n"
						"a_z_v_f_d_room_air_condition_common_alarm:%d,\n"
						"no1_a_c_chilled_unit_common_alarm:%d,\n"
						"no2_a_c_chilled_unit_common_alarm:%d,\n"
						"no3_a_c_chilled_unit_common_alarm:%d,\n"
						"chiller_water_pump_system_common_alarm:%d,\n"
						"no1_ahu_common_alarm:%d,\n"
						"no2_ahu_common_alarm:%d,\n"
						"no3_ahu_common_alarm:%d,\n"
						"no4_ahu_common_alarm:%d,\n"
						"refrigeration_plant_common_alarm:%d,\n"
						"refrigerant_leaking_alarm:%d",
				one.no1_wheel_house_air_condition_common_alarm,
				one.no2_wheel_house_air_condition_common_alarm,
				one.no1_galley_package_air_condition_common_alarm,
				one.no2_galley_package_air_condition_common_alarm,
				one.m_s_b_room_package_air_condition_common_alarm,
				one.no1_jacking_vfd_room_air_condition_common_alarm,
				one.no2_jacking_vfd_room_air_condition_common_alarm,
				one.deck_storeroom_air_condition_common_alarm,
				one.paint_locker_room_air_condition_common_alarm,
				one.workshop_air_condition_common_alarm,
				one.a_z_v_f_d_room_air_condition_common_alarm,
				one.no1_a_c_chilled_unit_common_alarm,
				one.no2_a_c_chilled_unit_common_alarm,
				one.no3_a_c_chilled_unit_common_alarm,
				one.chiller_water_pump_system_common_alarm,
				one.no1_ahu_common_alarm,
				one.no2_ahu_common_alarm,
				one.no3_ahu_common_alarm,
				one.no4_ahu_common_alarm,
				one.refrigeration_plant_common_alarm,
				one.refrigerant_leaking_alarm);
#endif

}

void FILELOG_compressed_air_system(const compressed_air_system &one)
{
#ifdef DEBUG
			static int cnt = 0;
			if(cnt++ % PRINT_INTERVAL == 0)
					sgo_log_stderr(0,"compressed_air_system:%d,%d,%d,%d,%d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f",
				one.no1_air_compressor_common_alarm,
				one.no2_air_compressor_common_alarm,
				one.service_air_compressor_common_alarm,
				one.quick_closing_valve_air_reservoir_low_pressure,
				one.air_dryer_common_alarm,
				one.no1_main_air_reservoir_pressure,
				one.no2_main_air_reservoir_pressure,
				one.daily_air_reservoir_pressure,
				one.no1_main_deck_air_reservoir_pressure,
				one.no2_main_deck_air_reservoir_pressure,
				one.no3_main_deck_air_reservoir_pressure,
				one.no4_main_deck_air_reservoir_pressure);
#endif

}


void FILELOG_miscellaneous(const miscellaneous &one)
{
#ifdef DEBUG
	static int cnt = 0;
	if(cnt++ % PRINT_INTERVAL == 0)
	{
		sgo_log_stderr(0,"miscellaneous:fire_alarm_control_unit_system_failure:%d,\n"
					"fire_alarm:%d,\n"
					"sewage_water_treatment_plant_common_alarm:%d,\n"
					"fuel_oil_purifier_common_alarm:%d,\n"
					"oil_water_separator_common_alarm:%d,\n"
					"oil_water_separator15ppm_alarm:%d,\n"
					"oil_water_separator_running_timeout_alarm_nc:%d,\n"
					"no1_fresh_water_generator_common_alarm:%d,\n"
					"no2_fresh_water_generator_common_alarm:%d,\n"
					"fresh_water_generator_no1_boost_pump_overload:%d,\n"
					"fresh_water_generator_no2_boost_pump_overload:%d,\n"
					"water_sprinkler_system_common_alarm:%d,\n"
					"no1_submersible_pump_failure:%d,\n"
					"no1_submersible_pump_running:%d,\n"
					"no2_submersible_pump_failure:%d,\n"
					"no2_submersible_pump_running:%d,\n"
					"no3_submersible_pump_failure:%d,\n"
					"no3_submersible_pump_running:%d,\n"
					"no4_submersible_pump_failure:%d,\n"
					"no4_submersible_pump_running:%d,\n"
					"ac440v_galley_equipment_db_low_insulation:%d,\n"
					"ac230v_galley_equipment_db_low_insulation:%d,\n"
					"co2_release_alarm_system_power_failure_alarm:%d,\n"
					"co2_leakage_alarm:%d,\n"
					"co2_release_alarm:%d",
	one.fire_alarm_control_unit_system_failure,
	one.fire_alarm,
	one.sewage_water_treatment_plant_common_alarm,
	one.fuel_oil_purifier_common_alarm,
	one.oil_water_separator_common_alarm,
	one.oil_water_separator15ppm_alarm,
	one.oil_water_separator_running_timeout_alarm_nc,
	one.no1_fresh_water_generator_common_alarm,
	one.no2_fresh_water_generator_common_alarm,
	one.fresh_water_generator_no1_boost_pump_overload,
	one.fresh_water_generator_no2_boost_pump_overload,
	one.water_sprinkler_system_common_alarm,
	one.no1_submersible_pump_failure,
	one.no1_submersible_pump_running,
	one.no2_submersible_pump_failure,
	one.no2_submersible_pump_running,
	one.no3_submersible_pump_failure,
	one.no3_submersible_pump_running,
	one.no4_submersible_pump_failure,
	one.no4_submersible_pump_running,
	one.ac440v_galley_equipment_db_low_insulation,
	one.ac230v_galley_equipment_db_low_insulation,
	one.co2_release_alarm_system_power_failure_alarm,
	one.co2_leakage_alarm,
	one.co2_release_alarm);

	sgo_log_stderr(0,"miscellaneous:e_r_co2_release_pre_alarm:%d,\n"
					"e_r_co2_release_valve_opened:%d,\n"
					"paint_room_co2_release_pre_alarm:%d,\n"
					"paint_room_co2_release_valve_opened:%d,\n"
					"no1_anti_fouling_device_common_alarm:%d,\n"
					"no2_anti_fouling_device_common_alarm:%d,\n"
					"uv_sterilizer_common_alarm:%d,\n"
					"navigation_aids_system_common_alarm:%d,\n"
					"public_address_general_alarm_system_fault:%d,\n"
					"public_address_general_alarm_system_power_fault:%d,\n"
					"auto_telephone_system_failure:%d,\n"
					"audio_visual_alarm_column_main_power_failure:%d,\n"
					"audio_visual_alarm_column_backup_power_failure:%d,\n"
					"no1_leg_control_unit_common_alarm:%d,\n"
					"no2_leg_control_unit_common_alarm:%d,\n"
					"no3_leg_control_unit_common_alarm:%d,\n"
					"no4_leg_control_unit_common_alarm:%d,\n"
					"fog_horn_controller_power_failure:%d,\n"
					"bnwas_main_unit_power_failure:%d,\n"
					"fresh_water_hydrophore_failure:%d,\n"
					"sea_water_hydrophore_failure:%d,\n"
					"calorifier_failure:%d,\n"
					"no1_ups_for_ams_power_failure:%d,\n"
					"no2_ups_for_ams_power_failure:%d,\n"
					"no3_ups_for_ams_power_failure:%d,\n"
					"no1_crane_fire_alarm:%d,\n"
					"no2_crane_fire_alarm:%d,\n"
					"engineer_alarm_system_power_failure:%d,\n"
					"watertight_door_indication_system_common_alarm:%d,\n"
					"freshwater_filling_flowmeter:%.3f,\n",
			one.e_r_co2_release_pre_alarm,
			one.e_r_co2_release_valve_opened,
			one.paint_room_co2_release_pre_alarm,
			one.paint_room_co2_release_valve_opened,
			one.no1_anti_fouling_device_common_alarm,
			one.no2_anti_fouling_device_common_alarm,
			one.uv_sterilizer_common_alarm,
			one.navigation_aids_system_common_alarm,
			one.public_address_general_alarm_system_fault,
			one.public_address_general_alarm_system_power_fault,
			one.auto_telephone_system_failure,
			one.audio_visual_alarm_column_main_power_failure,
			one.audio_visual_alarm_column_backup_power_failure,
			one.no1_leg_control_unit_common_alarm,
			one.no2_leg_control_unit_common_alarm,
			one.no3_leg_control_unit_common_alarm,
			one.no4_leg_control_unit_common_alarm,
			one.fog_horn_controller_power_failure,
			one.bnwas_main_unit_power_failure,
			one.fresh_water_hydrophore_failure,
			one.sea_water_hydrophore_failure,
			one.calorifier_failure,
			one.no1_ups_for_ams_power_failure,
			one.no2_ups_for_ams_power_failure,
			one.no3_ups_for_ams_power_failure,
			one.no1_crane_fire_alarm,
			one.no2_crane_fire_alarm,
			one.engineer_alarm_system_power_failure,
			one.watertight_door_indication_system_common_alarm,
			one.freshwater_filling_flowmeter);	
		}
#endif

}

void FILELOG_buzzer(const buzzer &one)
{
#ifdef DEBUG
	static int cnt0=0,cnt1=0,cnt2=0;
	if( (one.no == 0) && (cnt0++ % PRINT_INTERVAL == 0)
		|| (one.no == 1) && (cnt1++ % PRINT_INTERVAL == 0)
		|| (one.no == 2) && (cnt2++ % PRINT_INTERVAL == 0)
	)
	{
		sgo_log_stderr(0,"buzzer [%d] mute:%d,ack:%d,status:%d",one.no,one.mute,one.ack,one.status);
	}
#endif	
}


void FILELOG_extend_alarm_panel(const extend_alarm_panel &one)
{
#ifdef DEBUG
	static int cnt0=0,cnt1=0,cnt2=0,cnt3=0,cnt4=0;
	if( (one.no == 0) && (cnt0++ % PRINT_INTERVAL == 0)
		|| (one.no == 1) && (cnt1++ % PRINT_INTERVAL == 0)
		|| (one.no == 2) && (cnt2++ % PRINT_INTERVAL == 0)
		|| (one.no == 3) && (cnt3++ % PRINT_INTERVAL == 0)
		|| (one.no == 4) && (cnt4++ % PRINT_INTERVAL == 0)
	)
	{
		sgo_log_stderr(0,"extend alarm panel[%d] mute:%d ,status:%d",one.no,one.mute_or_ack,one.status);
	}
#endif	
}



///////FILELOG AMS end


