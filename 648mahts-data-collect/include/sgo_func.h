//函数声明放在这个头文件里-------------------------------------------
#ifndef __SGO_FUNC_H__
#define __SGO_FUNC_H__

#include <cstring>
#include <string>
#include <vector>
#include <sys/time.h>
#include <algorithm>
#include <cmath>

#include "sgo_collect_data.h"


typedef enum{
	SW1PS,
	SW2PS,
	SW3PS,
	FWPS,
	FOPS,
	FODTPS
}LIQUIDTYPE;

//字符串相关函数
void   Rtrim(char *string);
void   Ltrim(char *string);

void Stringsplit(const std::string& str, char split, std::vector<std::string>& res);


long getSystemTime();

//和日志，打印输出有关
void   sgo_log_init();
void   sgo_log_stderr(int err, const char *fmt, ...);
void   sgo_log_error_core(int level,  int err, const char *fmt, ...);
u_char *sgo_log_errno(u_char *buf, u_char *last, int err);
u_char *sgo_snprintf(u_char *buf, size_t max, const char *fmt, ...);
u_char *sgo_slprintf(u_char *buf, u_char *last, const char *fmt, ...);
u_char *sgo_vslprintf(u_char *buf, u_char *last,const char *fmt,va_list args);



//io信号转换
//4-20mA ----> -50~100℃
float cur2float(float cur,int first,int last,int tempStart,int tempEnd);
float cur2float_f(float cur,int first,int last,int tempStart,int tempEnd);


//crontab second "0/3 * * * * ?"
int getCronSecond(const std::string& cronStr);


void parseGensProp(const char *sdata,std::vector<std::string>& vRes);


double convertDegrees(double degreesMinutes);


//liquid heigh->volume
float calcVolumByHeight(float height,LIQUIDTYPE type);


std::string getCurrentTime(char flag);
bool isFloat(const std::string & str);
void parseRangeString(const std::string& rangeStr,std::vector<float>& result);

bool isNetworkInterfaceUp(const std::string& interfaceName);

std::string getHostIp(const char* ifname);


//fileLog
void FILELOG_main_diesel_generator(const main_diesel_generator &one);
void FILELOG_baudouin_main_generator(const baudouin_main_generator &one);
void FILELOG_emergency_generator(const emergency_generator &one);
void FILELOG_main_propulsion(const main_propulsion& one);
void FILELOG_bow_thruster(const bow_thruster &one);
void FILELOG_power_system(const power_system &one);
void FILELOG_general_charger_and_discharger_panel(const general_charger_and_discharger_panel &one);
void FILELOG_e_r_charger_and_discharger_panel(const e_r_charger_and_discharger_panel &one); 
void FILELOG_main_transformer(const main_transformer &one);
void FILELOG_level_gauging(const level_gauging &one);
void FILELOG_level_switch(const level_switch &one);
void FILELOG_air_conditioner_refrigeration_system(const air_conditioner_refrigeration_system &one);
void FILELOG_compressed_air_system(const compressed_air_system &one);
void FILELOG_cooling_water_system(const cooling_water_system &one);
void FILELOG_gas_detection_system(const gas_detection_system &one);
void FILELOG_miscellaneous(const miscellaneous &one);
void FILELOG_buzzer(const buzzer &one);
void FILELOG_extend_alarm_panel(const extend_alarm_panel &one);



#endif  
