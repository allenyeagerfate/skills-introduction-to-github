#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <vector>
#include <iomanip>
#include <chrono>
#include <signal.h>
#include <cmath>
#include <algorithm>
#include <functional>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <sys/wait.h>
#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include "mainService.h"
#include "timeUtil.h"

using namespace std;

int aaa = 0;  //byte
int bbb = 0;  //bit
float ccc = 0.0;  //result
float ddd = 0.0; //range first
float eee = 0.0; //range second



#define SGO_CONFIG_PATH "/etc/sgo.conf"
#if 1
#define PLC1_VALUE_ZERO(name,ix) (name[ix]==32767 || name[ix] > 64000|| name[ix]== 32768 ? io_error:name[ix])
#define PLC1_VALUE_TEN(name,ix) (name[ix]==32767 || name[ix] > 64000|| name[ix]== 32768 ? io_error:name[ix]/10.0)
#define PLC1_VALUE_HUNDRED(name,ix) (name[ix] >=32767 ? io_error:name[ix]/100.0)
#define PLC1_VALUE_HUNDRED_1(name,ix) (name[ix] ==32767 || name[ix] ==32768 ? io_error:(int16_t(name[ix]))/100.0)
#define PLC1_VALUE_THOUSAND(name,ix) (name[ix]==32767 || name[ix] > 64000|| name[ix]== 32768 ?io_error:name[ix]/1000.0)

#define PLC1_VALUE_TEN3(name,ix) ((name[ix]==32767) ?io_error:((int16_t)name[ix])/10.0)

#define PLC1_VALUE_RS485_ZERO(name,ix) (name[ix] >= 32767 ? 0:((int)name[ix])/1.0);
#define PLC1_VALUE_RS485_TEN(name,ix) (name[ix] >= 32767 ? 0:((int)name[ix])/10.0);
#define PLC1_VALUE_RS485_HUNDRED(name,ix) (name[ix] >= 32767 ? 0:((int)name[ix])/100.0);
#define PLC1_VALUE_RS485_THOUSAND(name,ix) (name[ix] >= 32767 ? 0:((int)name[ix])/1000.0);

#define PLC1_VALUE_EXCI_ZERO(name,ix) ((name[ix]==32767 || name[ix]== 65535) ?io_error:((int)name[ix])/1.0)
#define PLC1_VALUE_EXCI_TEN(name,ix) ((name[ix]==32767 || name[ix]== 65535) ?io_error:((int)name[ix])/10.0)


#define PLC1_VALUE_PRESS_REAL_VALUE(name,ix) ( (name[ix]) > 32767 ? 0:((int)name[ix]) )
#define PLC1_VALUE_PRESS_1(name,ix) ((name[ix]==32767 || name[ix]==32768 || name[ix]== 65535) ?io_error:((PLC1_VALUE_PRESS_REAL_VALUE(name,ix))*1.0) / (27648))  //0~1
#define PLC1_VALUE_PRESS_2(name,ix) ((name[ix]==32767 || name[ix]==32768 || name[ix]== 65535) ?io_error:((PLC1_VALUE_PRESS_REAL_VALUE(name,ix))*0.8) / (27648) - 0.2)  //-0.2 ~ 0.6
#define PLC1_VALUE_PRESS_3(name,ix) ((name[ix]==32767 || name[ix]==32768 || name[ix]== 65535) ?io_error:((PLC1_VALUE_PRESS_REAL_VALUE(name,ix))*0.6) / (27648))  //0 ~ 0.6
#define PLC1_VALUE_PRESS_4(name,ix) ((name[ix]==32767 || name[ix]==32768 || name[ix]== 65535) ?io_error:((PLC1_VALUE_PRESS_REAL_VALUE(name,ix))*4.0) / (27648))  //0 ~ 4
#define PLC1_VALUE_PRESS_5(name,ix) ((name[ix]==32767 || name[ix]==32768 || name[ix]== 65535) ?io_error:((PLC1_VALUE_PRESS_REAL_VALUE(name,ix))*1.6) / (27648))  //0 ~ 1.6
#define PLC1_VALUE_PRESS_6(name,ix) ((name[ix]==32767 || name[ix]==32768 || name[ix]== 65535) ?io_error:((PLC1_VALUE_PRESS_REAL_VALUE(name,ix))*10.0) / (27648))  //0 ~ 10
#define PLC1_VALUE_PRESS_7(name,ix) ((name[ix]==32767 || name[ix]==32768 || name[ix]== 65535) ?io_error:((PLC1_VALUE_PRESS_REAL_VALUE(name,ix))*6.0) / (27648))  //0 ~ 6

#define PLC1_VALUE_PRESS_1_2(data, name,ix) (aaa = (ix), bbb = 0, ddd = (((data).range.first > -5000.0)?(data).range.first:0.0), eee = (((data).range.second > -5000.0)?(data).range.second:1.0), ccc = ((name[aaa]==32767 || name[aaa]==32768 || name[aaa]== 65535) ?io_error:((PLC1_VALUE_PRESS_REAL_VALUE(name,aaa))*(eee - ddd)) / (27648) + ddd), (name == m_v_ios)?(m_v_ios_w[aaa] = (ccc*100)):0, ccc)  //0~1
#define PLC1_VALUE_PRESS_2_2(data, name,ix) (aaa = (ix), bbb = 0, ddd = (((data).range.first > -5000.0)?(data).range.first:-0.2), eee = (((data).range.second > -5000.0)?(data).range.second:0.6), ccc = ((name[aaa]==32767 || name[aaa]==32768 || name[aaa]== 65535) ?io_error:((PLC1_VALUE_PRESS_REAL_VALUE(name,aaa))*(eee - ddd)) / (27648) + ddd), (name == m_v_ios)?(m_v_ios_w[aaa] = (ccc*100)):0, ccc)  //-0.2 ~ 0.6
#define PLC1_VALUE_PRESS_3_2(data, name,ix) (aaa = (ix), bbb = 0, ddd = (((data).range.first > -5000.0)?(data).range.first:0.0), eee = (((data).range.second > -5000.0)?(data).range.second:0.6), ccc = ((name[aaa]==32767 || name[aaa]==32768 || name[aaa]== 65535) ?io_error:((PLC1_VALUE_PRESS_REAL_VALUE(name,aaa))*(eee - ddd)) / (27648) + ddd), (name == m_v_ios)?(m_v_ios_w[aaa] = (ccc*100)):0, ccc)  //0 ~ 0.6
#define PLC1_VALUE_PRESS_4_2(data, name,ix) (aaa = (ix), bbb = 0, ddd = (((data).range.first > -5000.0)?(data).range.first:0.0), eee = (((data).range.second > -5000.0)?(data).range.second:4.0), ccc = ((name[aaa]==32767 || name[aaa]==32768 || name[aaa]== 65535) ?io_error:((PLC1_VALUE_PRESS_REAL_VALUE(name,aaa))*(eee - ddd)) / (27648) + ddd), (name == m_v_ios)?(m_v_ios_w[aaa] = (ccc*100)):0, ccc)  //0 ~ 4
#define PLC1_VALUE_PRESS_5_2(data, name,ix) (aaa = (ix), bbb = 0, ddd = (((data).range.first > -5000.0)?(data).range.first:0.0), eee = (((data).range.second > -5000.0)?(data).range.second:1.6), ccc = ((name[aaa]==32767 || name[aaa]==32768 || name[aaa]== 65535) ?io_error:((PLC1_VALUE_PRESS_REAL_VALUE(name,aaa))*(eee - ddd)) / (27648) + ddd), (name == m_v_ios)?(m_v_ios_w[aaa] = (ccc*100)):0, ccc)  //0 ~ 1.6
#define PLC1_VALUE_PRESS_6_2(data, name,ix) (aaa = (ix), bbb = 0, ddd = (((data).range.first > -5000.0)?(data).range.first:0.0), eee = (((data).range.second > -5000.0)?(data).range.second:10.0), ccc = ((name[aaa]==32767 || name[aaa]==32768 || name[aaa]== 65535) ?io_error:((PLC1_VALUE_PRESS_REAL_VALUE(name,aaa))*(eee - ddd)) / (27648) + ddd), (name == m_v_ios)?(m_v_ios_w[aaa] = (ccc*100)):0, ccc)  //0 ~ 10
#define PLC1_VALUE_PRESS_7_2(data, name,ix) (aaa = (ix), bbb = 0, ddd = (((data).range.first > -5000.0)?(data).range.first:0.0), eee = (((data).range.second > -5000.0)?(data).range.second:6.0), ccc = ((name[aaa]==32767 || name[aaa]==32768 || name[aaa]== 65535) ?io_error:((PLC1_VALUE_PRESS_REAL_VALUE(name,aaa))*(eee - ddd)) / (27648) + ddd), (name == m_v_ios)?(m_v_ios_w[aaa] = (ccc*100)):0, ccc)  //0 ~ 6

#define PLC1_VALUE_PRESS_7_3(data, name,ix) (aaa = (ix), bbb = 0, ddd = (((data).range.first > -5000.0)?(data).range.first:0.0), eee = (((data).range.second > -5000.0)?(data).range.second:6.0), ccc = ((name[aaa]==32767 || name[aaa]==32768 || name[aaa]== 65535) ?io_error:((PLC1_VALUE_PRESS_REAL_VALUE(name,aaa))*(eee - ddd)) / (27648) + ddd), (name == m_v_plc)?(m_v_plc_w[aaa] = (ccc*100)):0, ccc)  //0 ~ 6

#define PLC1_VALUE_EXCITATION_CURRENT(data, name, ix) (aaa = (ix), bbb = 0, ddd = (((data).range.first > -5000.0)?(data).range.first:0.0), eee = (((data).range.second > -5000.0)?(data).range.second:10.0), ccc = ((name[aaa]==32767 || name[aaa]==32768 || name[aaa]== 65535) ?io_error:((PLC1_VALUE_PRESS_REAL_VALUE(name,aaa))*(eee - ddd)) / (27648) + ddd), (name == m_v_ios)?(m_v_ios_w[aaa] = (ccc*10)):0, ccc)  //0 ~ 10A
#define PLC1_VALUE_EXCITATION_VOLTAGE(data, name, ix) (aaa = (ix), bbb = 0, ddd = (((data).range.first > -5000.0)?(data).range.first:0.0), eee = (((data).range.second > -5000.0)?(data).range.second:100.0), ccc = ((name[aaa]==32767 || name[aaa]==32768 || name[aaa]== 65535) ?io_error:((PLC1_VALUE_PRESS_REAL_VALUE(name,aaa))*(eee - ddd)) / (27648) + ddd), (name == m_v_ios)?(m_v_ios_w[aaa] = (ccc*10)):0, ccc)  //0 ~ 100v

#else
#define PLC1_VALUE_ZERO(name,ix) (name[ix]==32767 ? io_error:name[ix])
#define PLC1_VALUE_TEN(name,ix) (name[ix]==32767 ? io_error:name[ix]/10.0)
#define PLC1_VALUE_HUNDRED(name,ix) (name[ix]==32767 ?io_error:name[ix]/100.0)
#define PLC1_VALUE_THOUSAND(name,ix) (name[ix]==32767 ?io_error:name[ix]/1000.0)
#endif
#define PLC1_VALUE_DI(name,ix,loop) ((name[ix]>>(loop)) & 1)


#define PLC1_VALUE_DI2(data, name, ix, loop) (aaa = (ix), bbb = (loop), (data).vStartWord = aaa, (data).vStartBit = bbb, (data).value = (name[aaa]>>(bbb)) & 1, (name[aaa]>>(bbb)) & 1)
#define PLC1_VALUE_TEN2(data, name,ix) (aaa = (ix), bbb = 0, ccc = ((name[aaa] >= 32767) ? io_error:name[aaa]/10.0), (data).vStartWord = aaa, (data).vStartBit = bbb, (data).value = ccc, ccc)
#define PLC1_VALUE_EXCI_TEN2(data, name,ix) (aaa = (ix), bbb = 0, ccc = (name[aaa]==32767 || name[aaa]== 65535) ?io_error:((int)name[aaa])/10.0, (data).vStartWord = aaa, (data).vStartBit = bbb, (data).value = ccc, ccc)
#define PLC1_VALUE_EXCI_ZERO2(data, name,ix) (aaa = (ix), bbb = 0, ccc = (name[aaa]==32767 || name[aaa]== 65535) ?io_error:((int)name[aaa])/1.0, (data).vStartWord = aaa, (data).vStartBit = bbb, (data).value = ccc, ccc)
#define PLC1_VALUE_ZERO2(data, name,ix) (aaa = (ix), bbb = 0, ccc = ((name[aaa]==32767 || name[aaa] > 64000|| name[aaa]== 32768) ? io_error:name[aaa]), (data).vStartWord = aaa, (data).vStartBit = bbb, (data).value = ccc, ccc)

#define PLC2_VALUE_ZERO(name,ix) (name[ix])
#define PLC2_VALUE_TEN(ix) (m_v_plc2_ais[ix]/10.0)
#define PLC2_VALUE_HUNDRED(ix) (m_v_plc2_ais[ix]/100.0)
#define PLC2_VALUE_THOUSAND(ix) (m_v_plc2_ais[ix]/1000.0)
#define PLC2_VALUE_DI(name,ix,loop) ((name[ix]>>(loop)) & 1)


#define SIEMENS_PORT_502 502
#define SIEMENS_PORT_503 503
#define SIEMENS_PORT_504 504
#define SIEMENS_PORT_505 505
#define SIEMENS_PORT_506 506
// #define SIEMENS_PORT_507 507
// #define SIEMENS_PORT_508 508
// #define SIEMENS_PORT_509 509
// #define SIEMENS_PORT_510 510
// #define SIEMENS_PORT_511 511
// #define SIEMENS_PORT_512 512
// #define SIEMENS_PORT_513 513
// #define SIEMENS_PORT_514 514
// #define SIEMENS_PORT_515 515
// #define SIEMENS_PORT_516 516
// #define SIEMENS_PORT_517 517

const float io_NaN = -1001.0;
const float io_error = -1000.0;

enum {
	MOXA_NPORT_GPS			=	10001,
	MOXA_NPORT_AIS,
	MOXA_NPORT_ANCHOR,
	MOXA_NPORT_MAINFLOWMETER,
	MOXA_NPORT_GYRO			=	20001,
	MOXA_NPORT_ECDIS,
	MOXA_NPORT_ECHOSOUNDER,
	MOXA_NPORT_WINDMETER,	
};


#define PLATFORM_JACKING_SYSTEM_IP "192.168.1.100"
#define AMS_IP "192.168.1.1"
#define CUMMINS_DCU_IP_1 "192.168.0.111"
#define CUMMINS_DCU_IP_2 "192.168.0.116"


string msgId;

extern char g_ams_lost;
extern long curDatetime;

static int timerunning = 0;

//#ifdef TEST
char rr[]={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
char rminutes[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
float running_hour[]={250.0,1000.0,1500,10000};
#define RANDMOD(d) (rand()%(d))
#define RANDMOD4  (running_hour[rand()%4])

//#define RANDMOD2 (rr[rand()%sizeof(rr)])
#define RANDMOD2 0
#define RANDMOD2_1 (rr[rand()%sizeof(rr)])

#define RANDMOD_MIN(d) (rminutes[d%sizeof(rminutes)])

const int alarm_gap = 180;

//#endif

const char *extend_alarm_panel_location[]={
	"chief",
	"chief office",
	"rest room",
	"meeting room",
	"restautant",
};

const string pc_name[]={
	"MAIN-PC1",
	"AUX-PC2",
	"FWD-PC3",
	"AFT-PC4",
};
	
const string pc_location[]={
	"ECC",
	"ECC",
	"FWD-WHC",
	"AFT-WHC",
};

/////////////////////////////////////////////////////////////////////////////////////////////////
int mainService::init()
{	
	m_pConfig = CConfig::GetInstance();
	if(!m_pConfig->Load(SGO_CONFIG_PATH))
	{
		sgo_log_stderr(0,"配置文件[%s]加载失败，退出。",SGO_CONFIG_PATH);
		exit(-1);
	}
	sgo_log_init();



#ifndef TEST
	//利用plc的一个端口只能一个客户端访问的性质，实现多个pc互斥连接
	connect_plc();
#endif

	sigset_t signal_mask;
	sigemptyset(&signal_mask);
	sigaddset(&signal_mask,SIGPIPE);
	pthread_sigmask(SIG_BLOCK,&signal_mask,NULL);

	//interface: "enp1s0"
	m_cur_pc_ip = getHostIp("eno1");
	if(!m_cur_pc_ip.empty())
		sgo_log_stderr(0,"m_cur_pc_ip:%s",m_cur_pc_ip.c_str());

    sleep(1);

	#ifndef MAHTS648
		m_plc1_rw_mux = new mutex;
	    m_plc2_rw_mux = new mutex;
		m_thr_mqtt = new thread(&mainService::initMqtt,this,m_mb_plc1,m_plc1_rw_mux);
	#else

	m_502_mux = new mutex;
	m_503_mux = new mutex;
	//m_504_mux = new mutex;
	m_506_sau1_mux = new mutex;
	//m_506_sau2_mux = new mutex;


    sleep(15);
	m_thr_mqtt = new thread(&mainService::initMqtt2, this);
	#endif

	m_thr_mqtt->detach();


	m_callbackQueue.Init();
	m_mud_handler.SetModbus(m_mb_506_sau1, m_506_sau1_mux);
    m_level_handler.SetModbus(m_mb_506_sau1, m_506_sau1_mux);
	m_machinery_handler.SetModbus(m_mb_506_sau1, m_506_sau1_mux);
	sleep(5);


#ifdef TEST
	thread *m_thr_test = new thread(&mainService::genRandTest,this);
	m_thr_test->detach();

	thread* buzzer_test = new thread(&mainService::gen_data_buzzer_and_extend_alarm_panel_test,this);
	buzzer_test->detach();

	thread* sau_test = new thread(&mainService::gen_data_sau_test,this);
	sau_test->detach();

	for(int i = 0; i < pc_nums; i++)
	{
		thread *thr_pc = new thread(&mainService::check_pc_status_test,this,i);
		thr_pc->detach();

		thread *thr_pc_dis = new thread(&mainService::check_pc_disconnect_test,this,i);
		thr_pc_dis->detach();		
	}

#else

	//plc1:503
	thread *m_thr_plc1 = new thread(&mainService::getDatasFromPLC1,this);
	m_thr_plc1->detach();

#ifndef MAHTS648
	//plc:504
	thread* buzzer = new thread(&mainService::gen_data_buzzer_and_extend_panel,this);
	buzzer->detach();
#else
    thread* buzzer = new thread(&mainService::gen_data_buzzer_and_extend_panel2,this);
	buzzer->detach();
#endif


	//plc:502
	for(int i = 0;i < buzzer_nums;i++)
	{
		thread* write_buzzer = new thread(&mainService::write_buzzer2,this,i);
		write_buzzer->detach();
	}

	//thread* write_bell = new thread(&mainService::write_bell,this);

	thread* write_duty = new thread(&mainService::write_duty,this);
	write_duty->detach();

    for(int i = 0;i < extend_alarm_panel_nums;i++)
	{
		thread* write_call_extend = new thread(&mainService::call_extend_alarm_panel2,this,i);
		write_call_extend->detach();
	}

	//plc:502
	for(int i = 0; i < pc_nums; i++)
	{
		thread *thr_pc = new thread(&mainService::check_pc_status,this,i);
		thr_pc->detach();

	
	}

	thread *thr_pc_dis = new thread(&mainService::check_pc_disconnect,this);
	thr_pc_dis->detach();	

	for(int i = 0; i < sau_nums; i++)
	{
		thread *thr_sau = new thread(&mainService::check_sau_status,this,i);
		thr_sau->detach();
	
	}
	
	thread *thr_sau_dis = new thread(&mainService::check_sau_disconnect,this);
	thr_sau_dis->detach();	

	//write plc1,plc2 datetime
	thread *m_thr_write_plc = new thread(&mainService::write_plc_datetime,this);
	m_thr_write_plc->detach();

    thread *thr_http = new thread(&mainService::GetHttpAlarms,this);
	thr_http->detach();
    
#endif

	//检测自身网络是否断开
	// *thr_check_self_network = new thread(&mainService::check_self_network,this);
	//thr_check_self_network->detach();



	/*bool isNetworkCableConnected() {
		// 使用 "ip link" 命令检查网络接口状态
		const char* command = "ip link show eth0 | grep 'state UP'";
		int result = system(command);
		
		// system 函数返回值为 0 表示命令成功执行且找到匹配项
		return WIFEXITED(result) && WEXITSTATUS(result) == 0;
	}*/


	return 0;
}

//Type-B/D main geneartor
void mainService::gen_data_diesel_generator()
{
	for(int i = 0; i < main_gens_nums;i++)
		gen_data_diesel_generator(i);
}

//Type-B/D main geneartor
void mainService::gen_data_diesel_generator2()
{
	for(int i = 0; i < main_gens_nums;i++)
		gen_data_diesel_generator2(i);
}

void mainService::gen_data_diesel_generator(int i)
{
	int ix;
	int iy;
	int iloop;
	float temp;
	main_diesel_generator one;
	memset(&one,0,sizeof(one));
	one.gen_id = i;

	ix = 63 + 20 * i;
	one.d_g_dcu_unit_battery_volt_dcu_control_power = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
	//one.d_g_lube_oil_pressure = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
	uint16_t lube_oil_press = m_v_plc[ix];
	//if(lube_oil_press == 32767 || lube_oil_press == 32768 || lube_oil_press > 65527 || lube_oil_press == 65534)
	if(lube_oil_press >= 32767)
		one.d_g_lube_oil_pressure = 0;
	else
		one.d_g_lube_oil_pressure = ((int)lube_oil_press)/100.0;
	ix++;

	one.d_g_coolant_pressure = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
	one.d_g_start_air_pressure = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
	//one.d_g_fuel_oil_pressure = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
	uint16_t fuel_oil_press =  m_v_plc[ix];
	//if(fuel_oil_press == 32767 || fuel_oil_press == 32768 || fuel_oil_press > 65527 || fuel_oil_press == 65534)
	if(fuel_oil_press >= 32767)
		one.d_g_fuel_oil_pressure = 0;
	else
		one.d_g_fuel_oil_pressure = ((int)fuel_oil_press)/100.0;
	ix++;

	one.d_g_coolant_temperature = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
	one.d_g_lube_oil_temperature = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
	one.d_g_left_exhaust_temperature = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.d_g_right_exhaust_temperature = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.d_g_engine_speed = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.d_g_rpu_unit_battery_a_volt_rpu_main_control_power = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
	one.d_g_rpu_unit_battery_b_volt_rpu_backup_control_power = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
	temp = PLC1_VALUE_ZERO(m_v_plc,ix);
	if( fabs(temp - io_error) < 1e-8)
		one.d_g_running_hours = 0;
	else
		one.d_g_running_hours = m_v_plc[ix]<<16;
	ix++;	
	temp = PLC1_VALUE_ZERO(m_v_plc,ix);
	if( fabs(temp - io_error) < 1e-8)
		temp = 0;
	one.d_g_running_hours += temp;
	
	ix = 60 + i * 20;
	iloop = 0;
	one.d_g_coolant_level_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_main_power_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_backup_power_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	iloop++;
	iloop++;
	one.d_g_sea_water_pump_overload_trip = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_ecr_remote_emergency_stop = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_aft_bridge_remote_emergency_stop = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_fwd_bridge_remote_emergency_stop = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	iloop++;
	one.d_g_lube_oil_pressure_too_low_shutdown = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_coolant_temperature_too_high_shutdown = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_local_emergency_stop = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_remote_emergency_stop = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	iloop++;
	one.d_g_rpu_emergency_stop = !PLC1_VALUE_DI(m_v_plc,ix,iloop++); //取反

	ix++;
	iloop = 0;
	one.d_g_lube_oil_pressure_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_coolant_pressure_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_start_air_pressure_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_fuel_oil_pressure_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_coolant_temperature_high_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_lube_oil_temperature_high_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_left_exhaust_temperature_high_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_right_exhaust_temperature_high_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_overspeed_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_overspeed_shutdown = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_speed_sensor_fail = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_dcu_unit_battery_volt_dct_control_power_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_start_fail = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_rpu_unit_battery_a_volt_rpu_main_control_power_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_rpu_unit_battery_b_volt_rpu_main_control_power_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_common_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
	one.d_g_common_warning = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_common_shutdown = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_common_cooldown = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_common_faultsensor_fail = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_remote_control_status_auxaccess_lock = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_ready_to_start = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_ready_to_load = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_main_generator_running = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix = 20 + i * 5;
	one.d_g_main_generator_u_winding_temperature = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_main_generator_v_winding_temperature = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_main_generator_w_winding_temperature = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_main_generator_de_bearing_temperature = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_main_generator_nde_bearing_temperature = PLC1_VALUE_TEN(m_v_ios, ix);ix++;

	iy = 60 + i * 2;
	//one.d_g_main_generator_excitation_current= PLC1_VALUE_TEN(m_v_ios, iy);iy++;
	//one.d_g_main_generator_excitation_voltage = PLC1_VALUE_ZERO(m_v_ios, iy);iy++;
	//4-20mA
	one.d_g_main_generator_excitation_current= PLC1_VALUE_EXCI_TEN(m_v_ios, iy);iy++;
	one.d_g_main_generator_excitation_voltage = PLC1_VALUE_EXCI_ZERO(m_v_ios, iy);iy++;
	//m 1127


	one.device_offline = 0;
	bq_main_diesel_generator[i].put(one);
	FILELOG_main_diesel_generator(one);
	m_client_main_gens[i].m_sensor_exception = 0;

}

void mainService::gen_data_baudouin_main_generator()
{
	for(int i = 0; i < baudouin_main_gens_nums;i++)
		gen_data_baudouin_main_generator(i);
}

void mainService::gen_data_baudouin_main_generator(int i)
{
	int ix;
	int iy;
	int iloop;
	float temp;
	baudouin_main_generator one;
	memset(&one,0,sizeof(one));
	one.gen_id = i;

	ix = 415 + 50 * i; //TBD
	one.gen_id = i;
	one.d_g_engine_rpm = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_battery_voltage = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_boost_t_ps = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_boost_t_sb = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_speed_r = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_speed_ma = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_oil_piston_jet = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_coolant_press = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_exhaust_t_sb = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_exhaust_t_ps = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_raw_wat_press = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_load = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_cool_temp_sb = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_fuel_temp = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_oil_temp = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_intake_p_sb = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_intake_t_sb = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_oil_press = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_fuel_press = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_fuel_rate = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_ecu_battvol_m = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_ecu_temp_m = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_lever_pos = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_rail_press_sb = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_tot_fuel_used = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_tot_fuel_used_l = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_rpu_rpm = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_rpu_ubat_a = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_rpu_ubat_b = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_pri_battery = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_sec_battery = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_cool_temp_ps = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_intake_p_ps = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_intake_t_ps = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_ecu_battvol_s = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_ecu_temp_s = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_rail_press_ps = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_engine_hours = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_run_hours = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_service_time = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_engine_state = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.d_g_mode_id = PLC1_VALUE_TEN(m_v_ios, ix);ix++;

	iloop = 0;
	one.d_g_water_in_fuel = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_remoter_start = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_remote_stop = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_priming_pump = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_increase_speed = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_ecu_slave_on = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_lo_lvl_raw_wat = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_mode2 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_decrease_speed = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_mode1 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_fuel_leak_sb = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_ecu_master_on = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_fuel_leak_ps = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_lo_lvl_coolant = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_very_lo_oil_p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_very_hi_cool_t = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
	one.d_g_rpu_sd3 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_rpu_sd4 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_rpu_sd5 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_emstop_rpu = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_turning_gear = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_light_test = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_start_air_p_lo = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_diff_eng_oil_p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_pre_heat_cde = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_override = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_ecucomm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_ecu_warning = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_ecu_bin3 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_ecu_bin4 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_ecu_bin5 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_ecu_bin6 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;	
	one.d_g_glow_lamp = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_ecu_bin8 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_ain11 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_ain13 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_shutoffreqm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_shutoffreqs = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_ra153_bout = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_detcon = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_rpu_fail = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_bw_rpu_fuelsol = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_bw_rpu_stopsol = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_ecu_alarmlist = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_ecucompconsist = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_actcall_fail = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_derate_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_ecudiagblocked = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;	
	one.d_g_pwr_limited = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_scm_fail = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_rtcbatteryflat = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_emergency_stop = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_overload = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_othrcommer = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_chrgalternfail = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_stop_fail = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_overspeed = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_underspeed = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_pickup_fail = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_battery_flat = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_start_fail = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_op_rpmdep = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_cp_rpmdep = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_wrn_rpm_diff = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;	
	one.d_g_switchtobatt_b = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_start_blocking = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_load_blocking = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_primbatactive = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_secbatactive = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_emssupply10a = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_extrasupply7a = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g30supply7a = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_ecu_yellowlamp = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_ecu_red_lamp = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.d_g_ecu_waittostrt = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	
	one.device_offline = 0;

	bq_baudouin_main_generator[i].put(one);
	FILELOG_baudouin_main_generator(one);
}



void mainService::gen_data_emergency_generator()
{
	int ix;
	int iloop;
	float temp;
	emergency_generator one;
	memset(&one,0,sizeof(one));

	ix = 39;
    iloop = 0;     
    one.eg_generator_u_winding_temperature = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.eg_generator_v_winding_temperature = PLC1_VALUE_TEN(m_v_ios, ix);ix++;	
	one.eg_generator_w_winding_temperature = PLC1_VALUE_TEN(m_v_ios, ix);ix++;

    //>>>>>>>>>>>>>>>>>>

	one.eg_dcu_unit_battery_volt_dcu_control_power = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
	one.eg_lube_oil_pressure = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
	one.eg_coolant_pressure = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
	ix++;//one.eg_start_air_pressure = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
	ix++;//one.eg_fuel_oil_pressure = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
	one.eg_coolant_temperature = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
	one.eg_lube_oil_temperature = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
	ix++;//one.eg_left_exhaust_temperature = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	ix++;//one.eg_right_exhaust_temperature = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.eg_engine_speed = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.eg_rpu_unit_battery_a_volt_rpu_main_control_power = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
	one.eg_rpu_unit_battery_b_volt_rpu_backup_control_power = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
	temp = PLC1_VALUE_ZERO(m_v_plc,ix);
	if( fabs(temp - io_error) < 1e-8)
		one.eg_running_hours = 0;
	else
		one.eg_running_hours = m_v_plc[ix]<<16;
	ix++;	
	temp = PLC1_VALUE_ZERO(m_v_plc,ix);
	if( fabs(temp - io_error) < 1e-8)
		temp = 0;
	one.eg_running_hours += temp;


	ix = 180;
	iloop = 0;
	one.eg_coolant_level_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_main_power_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_backup_power_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_no1_charger_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
	one.eg_no2_charger_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	
  	iloop = 12;
	one.eg_local_emergency_stop = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_remote_emergency_stop = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	iloop++;

	if(fabs(one.eg_coolant_temperature) > 0)
		one.eg_rpu_emergency_stop = !PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
	one.eg_lube_oil_pressure_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_coolant_pressure_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	iloop++;//one.eg_start_air_pressure_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	iloop++;//one.eg_fuel_oil_pressure_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_coolant_temperature_high_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_lube_oil_temperature_high_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	iloop++;//one.eg_left_exhaust_temperature_high_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	iloop++;//one.eg_right_exhaust_temperature_high_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_overspeed_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_overspeed_shutdown = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_speed_sensor_fail = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_dcu_unit_battery_volt_dct_control_power_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_start_fail = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_rpu_unit_battery_a_volt_rpu_main_control_power_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_rpu_unit_battery_b_volt_rpu_main_control_power_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_common_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
	one.eg_common_warning = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_common_shutdown = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_common_cooldown = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_common_faultsensor_fail = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_remote_control_status_auxaccess_lock = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_ready_to_start = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_ready_to_load = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_generator_running = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix = 50;
	one.eg_generator_u_winding_temperature = PLC1_VALUE_TEN(m_v_ios, ix);ix++;
	one.eg_generator_v_winding_temperature = PLC1_VALUE_TEN(m_v_ios, ix);ix++;	
	one.eg_generator_w_winding_temperature = PLC1_VALUE_TEN(m_v_ios, ix);ix++;

	bq_emergency_generator.put(one);
	
	FILELOG_emergency_generator(one);
	m_client_emergency_generator.m_sensor_exception = 0;

}

void mainService::gen_data_main_propulsion()
{
	for(int i = 0; i < 6; i++)
	{
		m_client_main_propulsion[i].m_sensor_exception = 0;
		gen_data_main_propulsion(i);
	}
}


void mainService::gen_data_main_propulsion(int i)
{
	int ix = 0;
	int iloop = 0;

	main_propulsion one;

	one.no = i;
	ix = 3 * i;
	iloop = 0;
	one.main_propulsion_rudder_feedback_signal_missing = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_control_system_main_power_failure = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_control_system_backup_power_failure = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_steering_command_missing_joystick = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_speed_command_missing_joystick = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_fu_steering_control_failure = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_fu_speed_control_failure = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_vfd_speed_feedback_signal_missing = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_vfd_load_feedback_signal_missing = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_steering_command_missing_fwd_console = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_speed_command_missing_fwd_console = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_rudder_command_missing_aft_console = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_speed_command_missing_aft_console = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_common_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_fault = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_remote_control_system_communication_failure = PLC1_VALUE_DI(m_v_plc, ix, iloop++);

	ix++;
	iloop = 0;
	one.main_propulsion_vfd_fault = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_vfd_common_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_vfd_emergency_stop = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_vfd_slowdown_pre_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_vfd_power_limit = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_steering_vfd_fault = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_motor_water_leakage = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_motor_u_winding_temrpeature_high = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_motor_v_winding_temrpeature_high = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_motor_w_winding_temrpeature_high = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_motor_high_temrpeature_slowdown = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_motor_high_temrpeature_shutdown = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_motor_de_bearing_high_temperature_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_motor_de_bearing_high_temperature_slowdown = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_motor_de_bearing_high_temperature_shutdown = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_motor_nde_bearing_high_temperature_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);

	ix++;
	iloop++;
	one.main_propulsion_motor_nde_bearing_high_temperature_slowdown = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_motor_nde_bearing_high_temperature_shutdown = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_motor_air_inlet_high_temperature_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_motor_air_outlet_high_temperature_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_motor_high_temrpeature_slowdown_pre_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_motor_high_temrpeature_shutdown_pre_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_motor_de_bearing_high_temp_slowdown_pre_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_motor_de_bearing_high_tem_shutdown_pre_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_motor_nde_bearing_high_temp_slowdown_pre_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_motor_nde_bearing_high_tem_shutdown_pre_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);

	ix = 6 + 4 * i;
	iloop = 1;
	//one.main_propulsion_control_position = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_emergency_control_mode = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	iloop = 7;
	one.main_propulsion_shaft_lock_indication = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_vfd_ready = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_vfd_remote_control_position = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_vfd_running = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_vfd_override = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_vfd_torque_limitation = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	iloop++;
	iloop++;			
	one.main_propulsion_speed_control_nfu_mode = PLC1_VALUE_DI(m_v_plc, ix, iloop++);

	ix++;
	iloop = 0;
	one.main_propulsion_vfd_start_interlock = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_vfd_control_mode = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_steering_control_nfu_mode = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	iloop = 8;
	one.azimuth_thruster_vfd1_running = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_vfd2_running = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_ready = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_joystick_control_permission = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_ap_control_permission = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	
	ix++;
	ix++;
	one.main_propulsion_control_position = m_v_plc[ix++];



	ix = 14 + 20 * i;
	one.main_propulsion_vfd_rpm_indication = m_v_plc[ix++];
	one.main_propulsion_vfd_load_indication = m_v_plc[ix++];
	ix++;
	ix++;
	one.main_propulsion_motor_u_winding_temperature = m_v_plc[ix++];
	one.main_propulsion_motor_v_winding_temperature = m_v_plc[ix++];
	one.main_propulsion_motor_w_winding_temperature = m_v_plc[ix++];
	one.main_propulsion_motor_de_bearing_temperature = m_v_plc[ix++];
	one.main_propulsion_motor_nde_bearing_temperature = m_v_plc[ix++];
	one.main_propulsion_motor_air_inlet_temperature = m_v_plc[ix++];
	one.main_propulsion_motor_air_outlet_temperature = m_v_plc[ix++];


	///////////////////////////////////////////////
	ix = 240 + 6 * i;
	iloop = 0;
	one.azimuth_thruster_main_power_failure = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_backup_power_failure = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_shaft_lock_inlet_air_pressure_low_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_lub_oil_level_low_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_lub_oil_temp_high_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_lub_oil_filter_blocked_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_lub_oil_press_low_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_sealing_oil_tank_level_low_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_vfd1_fault = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_vfd2_fault = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_fu_steering_fault = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_lub_motor1_overload_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_lub_motor2_overload_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_angle_order_lost_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_angle_feedback_lost_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_steering_power1_supply_fault = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	
	ix++;
	iloop = 0;
	one.azimuth_thruster_steering_power2_supply_fault = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_sealing_oil_inlet_air_press_low_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_sealing_oil_inlet_air_press_high_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_steering_motor1_temp_high_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_steering_motor2_temp_high_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);

	ix = 245 + 6 * i;
	one.main_propulsion_rudder_indication =(int16_t) m_v_plc[ix++];

	///////////////////////
	ix = 253 + 10 * i;
	one.main_propulsion_vfd_actual_speed = m_v_plc[ix++];
	one.main_propulsion_vfd_actual_torque = m_v_plc[ix++];
	one.main_propulsion_motor_current = m_v_plc[ix++];
	one.main_propulsion_vfd_dc_voltage = m_v_plc[ix++];
	one.main_propulsion_motor_power = m_v_plc[ix++];

	bq_main_propulsion[i].put(one);
	FILELOG_main_propulsion(one);
}

#if 0
void mainService::gen_data_main_propulsion(int i)
{
	main_propulsion one;
	modbus *mb = new modbus(MOXA_IP1,10007+i);
	mb->modbus_set_slave_id(1);

	int addr_ai;
	int need_ai;
	int addr_di;
	int need_di;	

	int ix;
	int iloop;
	while(1)
	{
		if(!mb->modbus_connect())
		{
			sleep(3);
			continue;
		}

		sgo_log_stderr(0,"modbus connect main propulsion[%d] %s:%d success.",i,MOXA_IP1,10007+i);
		addr_di = 0;
		need_di = 7;
		addr_ai = 9;
		need_ai = 13;
		while(1)
		{
			if(mb->modbus_read_holding_registers(addr_ai,need_ai,m_main_prop_ais))
			{
#ifdef DEMO
goto loopDemo;
#endif
				sgo_log_stderr(0,"modbus_read_holding_registers DI[44000-44048] failed.");
				goto loop;
			}

		
			if(mb->modbus_read_holding_registers(addr_di,need_di,m_main_prop_dis))
			{
#ifdef DEMO
goto loopDemo;
#endif			
				sgo_log_stderr(0,"modbus_read_holding_registers DI[44000-44048] failed.");
				goto loop;
			}
loopDemo:
			ix = 0;
			one.main_propulsion_control_position = m_main_prop_ais[ix++];
			one.main_propulsion_rudder_indication = m_main_prop_ais[ix++];
			one.main_propulsion_vfd_rpm_indication = m_main_prop_ais[ix++];
			one.main_propulsion_vfd_load_indication = m_main_prop_ais[ix++];
			ix++;
			ix++;
			one.main_propulsion_motor_u_winding_temperature = m_main_prop_ais[ix++];
			one.main_propulsion_motor_v_winding_temperature = m_main_prop_ais[ix++];
			one.main_propulsion_motor_w_winding_temperature = m_main_prop_ais[ix++];
			one.main_propulsion_motor_de_bearing_temperature = m_main_prop_ais[ix++];
			one.main_propulsion_motor_nde_bearing_temperature = m_main_prop_ais[ix++];
			one.main_propulsion_motor_air_inlet_temperature = m_main_prop_ais[ix++];
			one.main_propulsion_motor_air_outlet_temperature = m_main_prop_ais[ix++];


			ix = 0;
			iloop = 0;
			one.main_propulsion_rudder_feedback_signal_missing = MAIN_PROP_VALUE_DI(ix,iloop++);
			iloop = 6;
			one.main_propulsion_control_system_main_power_failure = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_control_system_backup_power_failure = MAIN_PROP_VALUE_DI(ix,iloop++);
			iloop = 9;
			one.main_propulsion_steering_command_missing_joystick = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_speed_command_missing_joystick = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_fu_steering_control_failure = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_fu_speed_control_failure = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_vfd_speed_feedback_signal_missing = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_vfd_load_feedback_signal_missing = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_steering_command_missing_fwd_console = MAIN_PROP_VALUE_DI(ix,iloop++);

			ix = 1;
			iloop = 0;
			one.main_propulsion_speed_command_missing_fwd_console = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_rudder_command_missing_aft_console = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_speed_command_missing_aft_console = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.azimuth_thruster_common_alarm = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.azimuth_thruster_fault = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_remote_control_system_communication_failure = MAIN_PROP_VALUE_DI(ix,iloop++);

			iloop = 10;
			one.main_propulsion_vfd_fault = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_vfd_common_alarm = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_vfd_emergency_stop = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_vfd_slowdown_pre_alarm = MAIN_PROP_VALUE_DI(ix,iloop++);
			iloop++;
			one.main_propulsion_vfd_power_limit = MAIN_PROP_VALUE_DI(ix,iloop++);

			ix = 2;
			iloop = 0;
			one.azimuth_thruster_steering_vfd_fault = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_motor_water_leakage = MAIN_PROP_VALUE_DI(ix,iloop++);

			ix = 3;
			iloop = 0;
			one.main_propulsion_motor_u_winding_temrpeature_high = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_motor_v_winding_temrpeature_high = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_motor_w_winding_temrpeature_high = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_motor_high_temrpeature_slowdown = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_motor_high_temrpeature_shutdown = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_motor_de_bearing_high_temperature_alarm = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_motor_de_bearing_high_temperature_slowdown = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_motor_de_bearing_high_temperature_shutdown = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_motor_nde_bearing_high_temperature_alarm = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_motor_nde_bearing_high_temperature_slowdown = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_motor_nde_bearing_high_temperature_shutdown = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_motor_air_inlet_high_temperature_alarm = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_motor_air_outlet_high_temperature_alarm = MAIN_PROP_VALUE_DI(ix,iloop++);

			ix = 4;
			iloop = 7;
			one.main_propulsion_motor_high_temrpeature_slowdown_pre_alarm = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_motor_high_temrpeature_shutdown_pre_alarm = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_motor_de_bearing_high_temp_slowdown_pre_alarm = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_motor_de_bearing_high_tem_shutdown_pre_alarm = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_motor_nde_bearing_high_temp_slowdown_pre_alarm = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_motor_nde_bearing_high_tem_shutdown_pre_alarm = MAIN_PROP_VALUE_DI(ix,iloop++);

			ix = 5;
			iloop = 0;
			iloop++;
			one.main_propulsion_emergency_control_mode = MAIN_PROP_VALUE_DI(ix,iloop++);
			iloop = 7;
			one.main_propulsion_shaft_lock_indication = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_vfd_ready = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_vfd_remote_control_position = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_vfd_running = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_vfd_override = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_vfd_torque_limitation = MAIN_PROP_VALUE_DI(ix,iloop++);
			iloop++;
			iloop++;			
			one.main_propulsion_speed_control_nfu_mode = MAIN_PROP_VALUE_DI(ix,iloop++);

			ix = 6;
			iloop = 0;
			one.main_propulsion_vfd_start_interlock = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_vfd_control_mode = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_steering_control_nfu_mode = MAIN_PROP_VALUE_DI(ix,iloop++);
			iloop = 8;
			one.azimuth_thruster_vfd1_running = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.azimuth_thruster_vfd2_running = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.azimuth_thruster_ready = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_joystick_control_permission = MAIN_PROP_VALUE_DI(ix,iloop++);
			one.main_propulsion_ap_control_permission = MAIN_PROP_VALUE_DI(ix,iloop++);



			bq_main_propulsion[i].put(one);
			FILELOG_main_propulsion(one);

			m_client_main_propulsion[0].m_sensor_exception = 0;
			std::this_thread::sleep_for(std::chrono::seconds(3));
		}
loop:
		
		m_client_main_propulsion[0].m_sensor_exception = 1;
		mb->modbus_close();
		std::this_thread::sleep_for(std::chrono::seconds(3));
	}



}
#endif

void mainService::gen_data_bow_thruster()
{
	bow_thruster one1,one2;
	int ix;
	int iloop;



    


	ix = 6;
	iloop = 2;
	one1.no = 0;
	one2.no = 1;
	one1.b_t_remote_control_system_ac_power_failure = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one1.b_t_remote_control_system_dc_power_failure = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one1.b_t_remote_control_system_failure = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one2.b_t_remote_control_system_ac_power_failure = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one2.b_t_remote_control_system_dc_power_failure = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one2.b_t_remote_control_system_failure = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one1.b_t_main_motor_overload = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one1.b_t_main_motor_high_temperature = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one1.b_t_oil_low_level = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one1.b_t_oil_low_pressure = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one1.b_t_integrated_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one1.b_t_oil_high_pressure = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one1.b_t_main_motor_short_circuit = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one1.b_t_emergency_stop_circuit_failure = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one1.device_offline = 0;
	ix = 7;
	iloop = 0;
	one1.b_t_too_many_times_start = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one2.b_t_main_motor_overload = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one2.b_t_main_motor_high_temperature = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one2.b_t_oil_low_level = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one2.b_t_oil_low_pressure = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one2.b_t_integrated_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one2.b_t_oil_high_pressure = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one2.b_t_main_motor_short_circuit = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one2.b_t_emergency_stop_circuit_failure = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one2.b_t_too_many_times_start = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one2.device_offline = 0;

	FILELOG_bow_thruster(one1);
	bq_bow_thruster[0].put(one1);
	
	FILELOG_bow_thruster(one2);
	bq_bow_thruster[1].put(one2);

}

void mainService::gen_data_power_system()
{
	power_system one;
	memset(&one,0,sizeof(one));
	
	int ix = 280;
	int iloop = 0;
	one.eg_running = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_cb_close = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.esb_dc24v_power_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_breaker_trip = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_over_current = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.esb_common_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.esb_ac440v_bus_low_insulation = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.esb_ac230v_bus_low_insulation = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.esb_auto_mode = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no1_main_transformer_breaker_trip = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no2_main_transformer_breaker_trip = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no1_main_crane_cb_close = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no2_main_crane_cb_close = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no1_jacking_system_phase_transformer_cb_close = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no2_jacking_system_phase_transformer_cb_close = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no3_jacking_system_phase_transformer_cb_close = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
	one.no4_jacking_system_phase_transformer_cb_close = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no1_bow_thruster_cb_close = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no2_bow_thruster_cb_close = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no1_main_propulsion_vfd_cb_close = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no2_main_propulsion_vfd_cb_close = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg1_ppm_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg2_ppm_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg3_ppm_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg4_ppm_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg5_ppm_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg6_ppm_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.msb_ac690v_bus_a_low_insulation = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.msb_ac690v_bus_b_low_insulation = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no1_main_transformer_cb_close = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no2_main_transformer_cb_close = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.ac690v_msb_dc24v_power_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
	one.msb_ac690v_bus_b_harmonic_over_limit = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.msb_ac690v_bus_a_harmonic_over_limit = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.msb_ac690v_shore_power_cb_closed = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no1_main_transformer_secondary_breaker_input_power_on = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no1_main_transformer_breaker_auto_mode = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no2_main_transformer_secondary_breaker_close = PLC1_VALUE_DI(m_v_plc,ix,iloop++);//v2.1.4
	one.no2_main_transformer_secondary_breaker_trip = PLC1_VALUE_DI(m_v_plc,ix,iloop++);//v2.1.4
	one.msb_ac440v_bus_b_blackout = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no2_main_transformer_secondary_breaker_input_power_on = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no2_main_transformer_breaker_auto_mode = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.ac440v_msbbustie_acb_close = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no1_main_transformer_secondary_breaker_close = PLC1_VALUE_DI(m_v_plc,ix,iloop++);//v2.1.4
	one.no1_main_transformer_secondary_breaker_trip = PLC1_VALUE_DI(m_v_plc,ix,iloop++);//v2.1.4
	one.msb_ac440v_bus_a_blackout = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.ac440v_msb_dc24v_power_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.pt_power_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
	one.ac440v_msb_bus_c_low_insulation = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.ac440v_msb_bus_d_low_insulation = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.ac230v_msb_bus_low_insulation = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.preferential_trip_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.e_stop_shutdown_power_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.ac690v_msb_bus_tie_cb_closed = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.ac440v_msb_to_esb_cb_colse_statues = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.ac440v_shore_power_cb_closed = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.emergency_shutdown_wire_break_es1 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.emergency_shutdown_wire_break_es21 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.emergency_shutdown_wire_break_es22 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.emergency_shutdown_wire_break_es23 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.emergency_shutdown_wire_break_es24 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.emergency_shutdown_wire_break_es25 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.emergency_shutdown_wire_break_es26 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.emergency_shutdown_es1_actived = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
	one.emergency_shutdown_es21_actived = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.emergency_shutdown_es22_actived = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.emergency_shutdown_es23_actived = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.emergency_shutdown_es24_actived = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.emergency_shutdown_es25_actived = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.emergency_shutdown_es26_actived = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 1;
	one.dg1_acb_close_fail =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg1_acb_open_fail =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg1_ready_to_start =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	iloop++;
	one.dg1_on_first_priority =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg1_cb_open =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg1_cb_close =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg1_auto_mode =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg1_semi_auto_mode =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	iloop = 13;
	one.dg2_acb_close_fail =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg2_acb_open_fail =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg2_ready_to_start =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	
	ix++;
	iloop = 1;
	one.dg2_on_first_priority =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg2_cb_open =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg2_cb_close =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg2_auto_mode =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg2_semi_auto_mode =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	iloop = 9;
	one.dg3_acb_close_fail =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg3_acb_open_fail =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg3_ready_to_start =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	iloop = 13;
	one.dg3_on_first_priority =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg3_cb_open =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg3_cb_close =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
	one.dg3_auto_mode =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg3_semi_auto_mode =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	iloop = 5;
	one.dg4_acb_close_fail =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg4_acb_open_fail =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg4_ready_to_start =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	iloop++;
	one.dg4_on_first_priority =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg4_cb_open =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg4_cb_close =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg4_auto_mode =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg4_semi_auto_mode =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	
	ix++;
	iloop = 1;
	one.dg5_acb_close_fail =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg5_acb_open_fail =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg5_ready_to_start =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	iloop++;
	one.dg5_on_first_priority =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg5_cb_open =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg5_cb_close =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg5_auto_mode =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg5_semi_auto_mode =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	iloop = 13;
	one.dg6_acb_close_fail =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg6_acb_open_fail =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg6_ready_to_start =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	
	ix++;
	iloop = 1;
	one.dg6_on_first_priority =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg6_cb_open =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg6_cb_close =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg6_auto_mode =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg6_semi_auto_mode =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);

#ifdef JANA508509
	//dg7
	ix = 400;
	iloop = 0;
	one.dg7_acb_close_fail =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg7_acb_open_fail =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg7_ready_to_start =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg7_cb_open =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg7_cb_close =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg7_auto_mode =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg7_semi_auto_mode =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg7_on_first_priority =  PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	///
#endif
	
	ix = 290;
	iloop = 0;
	one.dg1_supply_voltage_high =	PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg1_supply_voltage_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg1_supply_frequency_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg1_supply_frequency_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg1_acb_abnormal_trip = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg1_reverse_power = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg1_ppm_alarms_on = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.ac690v_msb_bus_bar_voltage_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.ac690v_msb_bus_bar_voltage_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.ac690v_msb_bus_bar_frequency_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.ac690v_msb_bus_bar_frequency_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg1_auto_synchro_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	ix++;
	one.dg1_voltage_l1_l2 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg1_voltage_l2_l3 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg1_voltage_l3_l1 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg1_frequency_l1 = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
	one.dg1_frequency_l2 = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
	one.dg1_frequency_l3 = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
	one.dg1_current_l1 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg1_current_l2 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg1_current_l3 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg1_active_power = PLC1_VALUE_ZERO(m_v_plc,ix);ix++; //ix= 300;
	one.dg1_reactive_power = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg1_apparent_power_s = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg1_power_factor = PLC1_VALUE_TEN(m_v_plc,ix);ix++;

cout<<"-------one.factor:"<<one.dg1_power_factor<<endl;
	
	one.dg1_available_power = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.ac690v_msb_busbar_voltage_l1_l2 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.ac690v_msb_busbar_voltage_l2_l3 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.ac690v_msb_busbar_voltage_l3_l1 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.ac690v_msb_busbar_frequency_l1 = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
	one.ac690v_msb_busbar_frequency_l2 = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
	one.ac690v_msb_busbar_frequency_l3 = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;

	ix = 311;
	iloop = 0;
	one.dg2_supply_voltage_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg2_supply_voltage_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg2_supply_frequency_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg2_supply_frequency_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg2_acb_abnormal_trip = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg2_reverse_power = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg2_ppm_alarms_on = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	iloop++;
	one.no1_jetting_pump_request_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	iloop++;
	one.no1_bow_thruster_request_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	iloop++;
	one.no1_main_crane_request_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	iloop++;
	one.no2_jetting_pump_request_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg2_auto_synchro_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	one.dg2_voltage_l1_l2 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg2_voltage_l2_l3 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg2_voltage_l3_l1 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg2_frequency_l1 = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
	one.dg2_frequency_l2 = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
	one.dg2_frequency_l3 = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
	one.dg2_current_l1 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg2_current_l2 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg2_current_l3 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg2_active_power =PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg2_reactive_power = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg2_apparent_power_s = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg2_power_factor = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
	one.dg2_available_power = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;

	ix = 326;
	iloop = 0;
	one.dg3_supply_voltage_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg3_supply_voltage_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg3_supply_frequency_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg3_supply_frequency_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg3_acb_abnormal_trip = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg3_reverse_power = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg3_ppm_alarms_on = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg3_auto_synchro_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	iloop++;
	one.no1_leg_control_unit_request_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	iloop++;
	one.no3_leg_control_unit_request_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	iloop++;
	one.no1_submersible_pump_request_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	iloop++;
	one.no2_main_crane_request_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	ix++;
	one.dg3_voltage_l1_l2 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg3_voltage_l2_l3 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg3_voltage_l3_l1 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg3_frequency_l1 = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
	one.dg3_frequency_l2 = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
	one.dg3_frequency_l3 = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
	one.dg3_current_l1 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg3_current_l2 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg3_current_l3 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg3_active_power = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg3_reactive_power = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg3_apparent_power_s = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg3_power_factor = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
	one.dg3_available_power = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;

	ix = 341;
	iloop = 0;
	one.dg4_supply_voltage_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg4_supply_voltage_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg4_supply_frequency_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg4_supply_frequency_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg4_acb_abnormal_trip = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg4_reverse_power = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg4_ppm_alarms_on = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg4_auto_synchro_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	iloop++;
	one.no2_leg_control_unit_request_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	iloop++;
	one.no4_leg_control_unit_request_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	iloop++;
	one.no2_submersible_pump_request_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	iloop++;
	one.no2_bow_thruster_request_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	ix++;
	one.dg4_voltage_l1_l2 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg4_voltage_l2_l3 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg4_voltage_l3_l1 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg4_frequency_l1 = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
	one.dg4_frequency_l2 = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
	one.dg4_frequency_l3 = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
	one.dg4_current_l1 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg4_current_l2 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg4_current_l3 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg4_active_power = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg4_reactive_power = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg4_apparent_power_s = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg4_power_factor = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
	one.dg4_available_power = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;		

	ix = 356;
	iloop = 0;
	one.dg5_supply_voltage_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg5_supply_voltage_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg5_supply_frequency_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg5_supply_frequency_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg5_acb_abnormal_trip = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg5_reverse_power = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg5_ppm_alarms_on = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg5_auto_synchro_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	ix++;
	one.dg5_voltage_l1_l2 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg5_voltage_l2_l3 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg5_voltage_l3_l1 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg5_frequency_l1 = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
	one.dg5_frequency_l2 = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
	one.dg5_frequency_l3 = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
	one.dg5_current_l1 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg5_current_l2 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg5_current_l3 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg5_active_power = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg5_reactive_power = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg5_apparent_power_s = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg5_power_factor = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
	one.dg5_available_power = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;

	ix = 371;
	iloop = 0;
	one.dg6_supply_voltage_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg6_supply_voltage_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg6_supply_frequency_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg6_supply_frequency_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg6_acb_abnormal_trip = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg6_reverse_power = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg6_ppm_alarms_on = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg6_auto_synchro_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	ix++;
	one.dg6_voltage_l1_l2 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg6_voltage_l2_l3 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg6_voltage_l3_l1 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg6_frequency_l1 = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
	one.dg6_frequency_l2 = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
	one.dg6_frequency_l3 = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
	one.dg6_current_l1 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg6_current_l2 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg6_current_l3 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg6_active_power = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg6_reactive_power = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg6_apparent_power_s = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg6_power_factor = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
	one.dg6_available_power = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;		

#ifdef JANA508509
	//dg7
	ix = 400;
	iloop = 8;
	one.dg7_supply_voltage_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg7_supply_voltage_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg7_supply_frequency_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg7_supply_frequency_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg7_acb_abnormal_trip = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg7_reverse_power = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg7_ppm_alarms_on = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg7_auto_synchro_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	one.dg7_voltage_l1_l2 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg7_voltage_l2_l3 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg7_voltage_l3_l1 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg7_frequency_l1 = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
	one.dg7_frequency_l2 = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
	one.dg7_current_l1 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg7_frequency_l3 = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
	one.dg7_current_l2 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg7_current_l3 = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg7_active_power = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg7_reactive_power = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg7_apparent_power_s = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dg7_power_factor = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
	one.dg7_available_power = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;	
#endif

	one.device_offline = 0;

	bq_power_system.put(one);
	FILELOG_power_system(one);

	m_client_power_system.m_sensor_exception = 0;
}

void mainService::gen_data_general_charger_and_discharger_panel()
{
	general_charger_and_discharger_panel one;

	int ix = 4;
 	int iloop = 0;
	one.low_insulation = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.rectifier_fault = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.charger_fault = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.inverter_failure = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.battery_protector_active = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.battery_discharge = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.battery_low_voltage = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.inverter_low_voltage = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.device_offline = 0;

	bq_general_charger_and_discharger_panel.put(one);
	FILELOG_general_charger_and_discharger_panel(one);
}

void mainService::gen_data_bow_thruster2(int i)
{

	bow_thruster_store& m_cur_data_store = m_client_bow_thruster[i].m_cur_data_store; 
	bow_thruster one;
	int ix;
	int iloop;

    
    one.no = i;
	
	ix = 200 + i*10;
	iloop = 0;
	one.b_t_hydraulic_oil_tank_low_oil_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.b_t_spare_power_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.b_t_main_motor_high_temp = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.b_t_main_power_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.b_t_ac220v_power_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.b_t_dc24v_power_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.b_t_main_vfd_integrate = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.b_t_integrate_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	iloop = 10;
    one.b_t_emcy_wire_break = PLC1_VALUE_DI(m_v_plc,ix,iloop++);


	switch(i)
	{
	case 0:
	    ix = 520;
	    iloop = 0;
		one.b_t_cb_status = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
		one.b_t_ams_common_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
		break;
	case 1:
	    ix = 520;
	    iloop = 2;
		one.b_t_cb_status = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
		one.b_t_ams_common_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
		break;
	default:
		break;
	}

	ix = 598 + i * 2;
	iloop = 0;
	one.b_t_fresh_water_cooling_pump_outlet_pressure = PLC1_VALUE_PRESS_7_2(m_cur_data_store.b_t_fresh_water_cooling_pump_outlet_pressure, m_v_plc, ix);ix++;
	one.b_t_sea_water_cooling_pump_outlet_pressure = PLC1_VALUE_PRESS_7_2(m_cur_data_store.b_t_sea_water_cooling_pump_outlet_pressure, m_v_plc, ix);ix++;

    //>>>>>>>>>>>>>>>>>>>>>>
	m_vecTmpMachinery.push_back (one.b_t_main_power_fault);
    m_vecTmpMachinery.push_back (one.b_t_ac220v_power_fault);
	m_vecTmpMachinery.push_back (one.b_t_dc24v_power_fault);
	//>>>>>>>>>>>>>>>>>>>>>>


	one.device_offline = 0;

	FILELOG_bow_thruster(one);
	bq_bow_thruster[i].put(one);

}

void mainService::gen_data_sewage_treatment_plant()
{
    sewage_treatment_plant one;
	int ix = 510;
	int iloop = 0;

    one.sewage_treatment_plant_running = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.sewage_treatment_plant_common_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.sewage_treatment_plant_buffer_tank_high_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);    
    one.sewage_treatment_plant_cutting_pump_running = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	m_vecTmpMachinery.push_back (one.sewage_treatment_plant_common_fault);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	 
	one.device_offline = 0;
    bq_sewage_treatment_plant.put(one);
}

void mainService::gen_data_port_azimuth_center_control_cabinet()
{
	port_azimuth_center_control_cabinet_store& m_cur_data_store = m_client_port_azimuth_center_control_cabinet.m_cur_data_store;
	port_azimuth_center_control_cabinet one;
	int ix = 220;
	int iloop = 0;

    one.port_no1_azimuth_position = PLC1_VALUE_TEN3(m_v_plc,ix);ix++;
	one.port_no2_azimuth_position = PLC1_VALUE_TEN3(m_v_plc,ix);ix++;
	one.port_propeller_speed = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.port_main_vfd_speed = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.port_lub_oil_temperature = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
	one.port_main_motor_u_winding_temp = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
	one.port_main_motor_v_winding_temp = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
	one.port_main_motor_w_winding_temp = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
	one.port_main_motor1_bearing_temp = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
	one.port_main_motor2_bearing_temp = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
	one.port_main_motor_wind_in_temp = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
	one.port_main_motor_wind_out_temp = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
	one.port_pms_power_capacity = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.port_main_vfd_load = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
    one.port_control_position = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;

    ix = 250;
	iloop = 0;

    one.port_main_power_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_back_power_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_lo_level_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_lo_temp_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_lo_filter_blocked = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_lo_pressure_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_lo_standby_pump_switched = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_azimut1_vfd_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_azimut2_vfd_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_azimuth_control_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port1lo_pump_overload = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port2lo_pump_overload = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_azimuth_position1control_feedback_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_fwd_azimuth_command_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_fwd_speed_command_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_aft_azimuth_command_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
    one.port_aft_speed_command_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_main_vfd_common_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_main_vfd_estop = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_mian_motor_water_leak = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_shaft_lock_air_pressure_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_gravity_oil_level_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_gravity_air_pressure_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_gravity_air_pressure_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_main_motor_u_temp_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_main_motor_v_temp_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_main_motor_w_temp_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_front_shaft_bearing_temp_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_after_shaft_bearing_temp_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_main_vfd_load_feedback_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_pms_power_capacity_feedback_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_main_vfd_speed_feedback_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
    one.port_main_vfd_speed_control_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_azimuth1vfd_communication_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_azimuth2vfd_communication_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_main_motor1_bearing_temp_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_main_motor2_bearing_temp_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_main_motor_wind_in_temp_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_main_motor_wind_out_temp_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_main_motor_wind_temp_high_sld_per_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_main_motor_bearing_temp_high_sld_per_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_sld_active = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_bridge_station_communication_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_heavy_load_inquiry_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_main_vfd_start_interlock = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_main_vfd_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_main_vfd_esiop_wire_broken = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_gravity_oil_level_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
    one.port_azimuth_position2indication_feedback_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_azimuth_position_feedback_unsynchronized = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_vibration_sensor1_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_vibration_sensor2_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_dp_azimuth_command_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_dp_speed_command_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_joy_azimuth_command_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_joy_speed_command_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_ecr_speed_command_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_ecr_station_communication_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

    ix = 520;
	iloop = 4;
    one.pst_cb_status = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

    ix = 602;
    one.pst_fresh_water_cooling_pump_outlet_pressure = PLC1_VALUE_PRESS_7_2(m_cur_data_store.pst_fresh_water_cooling_pump_outlet_pressure,m_v_plc,ix);ix++;
	one.pst_sea_water_cooling_pump_outlet_pressure = PLC1_VALUE_PRESS_7_2(m_cur_data_store.pst_sea_water_cooling_pump_outlet_pressure,m_v_plc,ix);ix++;

    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	m_vecTmpMachinery.push_back (one.port_main_power_fault);
    m_vecTmpMachinery.push_back (one.port_back_power_fault);
	m_vecTmpMachinery.push_back (one.port_azimut1_vfd_fault);
    m_vecTmpMachinery.push_back (one.port_azimut2_vfd_fault);
    m_vecTmpMachinery.push_back (one.port_azimuth_control_fault);
	m_vecTmpMachinery.push_back (one.port_main_vfd_speed_control_fault);
	m_vecTmpMachinery.push_back (one.port_heavy_load_inquiry_fault);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


	one.device_offline = 0;
	bq_port_azimuth_center_control_cabinet.put(one);
}

void mainService::gen_data_stbd_azimuth_center_control_cabinet()
{
	stbd_azimuth_center_control_cabinet_store& m_cur_data_store = m_client_stbd_azimuth_center_control_cabinet.m_cur_data_store;
	stbd_azimuth_center_control_cabinet one;
	int ix = 260;
	int iloop = 0;

    one.stbd_no1_azimuth_position = PLC1_VALUE_TEN3(m_v_plc,ix);ix++;
    one.stbd_no2_azimuth_position = PLC1_VALUE_TEN3(m_v_plc,ix);ix++;
    one.stbd_propeller_speed = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
    one.stbd_main_vfd_speed = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
    one.stbd_lub_oil_temperature = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.stbd_main_motor_u_winding_temp = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.stbd_main_motor_v_winding_temp = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.stbd_main_motor_w_winding_temp = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.stbd_main_motor1_bearing_temp = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.stbd_main_motor2_bearing_temp = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.stbd_main_motor_wind_in_temp = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.stbd_main_motor_wind_out_temp = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.stbd_pms_power_capacity = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
    one.stbd_main_vfd_load = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
    one.stbd_control_position = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;

    ix = 290;
	iloop = 0;
	one.stbd_main_power_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_back_power_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_lo_level_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_lo_temp_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_lo_filter_blocked = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_lo_pressure_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_lo_standby_pump_switched = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_azimut1_vfd_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_azimut2_vfd_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_azimuth_control_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd1lo_pump_overload = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd2lo_pump_overload = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_azimuth_position1control_feedback_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_fwd_azimuth_command_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_fwd_speed_command_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_aft_azimuth_command_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
    one.stbd_aft_speed_command_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_main_vfd_common_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_main_vfd_estop = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_mian_motor_water_leak = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_shaft_lock_air_pressure_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_gravity_oil_level_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_gravity_air_pressure_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_gravity_air_pressure_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_main_motor_u_temp_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_main_motor_v_temp_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_main_motor_w_temp_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_front_shaft_bearing_temp_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_after_shaft_bearing_temp_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_main_vfd_load_feedback_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_pms_power_capacity_feedback_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_main_vfd_speed_feedback_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
    one.stbd_main_vfd_speed_control_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_azimuth1vfd_communication_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_azimuth2vfd_communication_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_main_motor1_bearing_temp_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_main_motor2_bearing_temp_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_main_motor_wind_in_temp_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_main_motor_wind_out_temp_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_main_motor_wind_temp_high_sld_per_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_main_motor_bearing_temp_high_sld_per_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_sld_active = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_bridge_station_communication_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_heavy_load_inquiry_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_main_vfd_start_interlock = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_main_vfd_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_main_vfd_esiop_wire_broken = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_gravity_oil_level_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
    one.stbd_azimuth_position2indication_feedback_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_azimuth_position_feedback_unsynchronized = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_vibration_sensor1_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_vibration_sensor2_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_dp_azimuth_command_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_dp_speed_command_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_joy_azimuth_command_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_joy_speed_command_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_ecr_speed_command_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_ecr_station_communication_lost = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
     
    ix = 520;
	iloop = 5;
    one.sst_cb_status = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix = 604;
	iloop = 0;
	one.sst_fresh_water_cooling_pump_outlet_pressure = PLC1_VALUE_PRESS_7_2(m_cur_data_store.sst_fresh_water_cooling_pump_outlet_pressure,m_v_plc,ix);ix++;
	one.sst_sea_water_cooling_pump_outlet_pressure = PLC1_VALUE_PRESS_7_2(m_cur_data_store.sst_sea_water_cooling_pump_outlet_pressure,m_v_plc,ix);ix++;

    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	m_vecTmpMachinery.push_back (one.stbd_main_power_fault);
    m_vecTmpMachinery.push_back (one.stbd_back_power_fault);
	m_vecTmpMachinery.push_back (one.stbd_azimut1_vfd_fault);
    m_vecTmpMachinery.push_back (one.stbd_azimut2_vfd_fault);
    m_vecTmpMachinery.push_back (one.stbd_azimuth_control_fault);
	m_vecTmpMachinery.push_back (one.stbd_main_vfd_speed_control_fault);
	m_vecTmpMachinery.push_back (one.stbd_heavy_load_inquiry_fault);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	one.device_offline = 0;
	bq_stbd_azimuth_center_control_cabinet.put(one);
}

void mainService::gen_data_e_r_charger_and_discharger_panel()
{
	e_r_charger_and_discharger_panel one;
	int ix = 4;
	int iloop = 8;

	one.ups_low_insulation = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.ups_battery_low_voltage = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.ups_input_fault = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.ups_battery_bypass_activation = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.ups_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);

	ix = 10;
	iloop = 5;
	one.ups_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.ups_running = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.ups_battery_mode = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	iloop++;
	iloop++;
	one.ups_battery_protection_trip = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.ups_output_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.device_offline = 0;

	bq_e_r_charger_and_discharger_panel.put(one);
	FILELOG_e_r_charger_and_discharger_panel(one);
}

void mainService::gen_data_main_transformer()
{
	main_transformer one;
	one.device_offline = 0;
	int ix = 76;
	one.no = 0;
	one.main_transformer_u_winding_temp = PLC1_VALUE_TEN(m_v_ios,ix);ix++;
	one.main_transformer_v_winding_temp = PLC1_VALUE_TEN(m_v_ios,ix);ix++;
	one.main_transformer_w_winding_temp = PLC1_VALUE_TEN(m_v_ios,ix);ix++;
	bq_main_transformer[0].put(one);
	FILELOG_main_transformer(one);

	one.no = 1;
	one.main_transformer_u_winding_temp = PLC1_VALUE_TEN(m_v_ios,ix);ix++;
	one.main_transformer_v_winding_temp = PLC1_VALUE_TEN(m_v_ios,ix);ix++;
	one.main_transformer_w_winding_temp = PLC1_VALUE_TEN(m_v_ios,ix);ix++;
	bq_main_transformer[1].put(one);
	FILELOG_main_transformer(one);
}


void mainService::gen_data_level_gauging()
{
	uint16_t tmp;
	level_gauging one;
	int i = 200;
	tmp = m_v_plc[i++];
	one.forep_draft = (tmp == 0xFFFF) ? io_error: tmp/1000.0;
	tmp = m_v_plc[i++];
	one.fores_draft = (tmp == 0xFFFF) ? io_error: tmp/1000.0;
	tmp = m_v_plc[i++];
	one.aftp_draft = (tmp == 0xFFFF) ? io_error: tmp/1000.0;
	tmp = m_v_plc[i++];
	one.afts_draft = (tmp == 0xFFFF) ? io_error: tmp/1000.0;
	tmp = m_v_plc[i++];
	one.sw_ballast_tank1p_level = (tmp == 0xFFFF) ? io_error: tmp/1000.0;
	tmp = m_v_plc[i++];
	one.sw_ballast_tank1s_level = (tmp == 0xFFFF) ? io_error: tmp/1000.0;
	tmp = m_v_plc[i++];
	one.sw_ballast_tank2p_level = (tmp == 0xFFFF) ? io_error: tmp/1000.0;
	tmp = m_v_plc[i++];
	one.sw_ballast_tank2s_level = (tmp == 0xFFFF) ? io_error: tmp/1000.0;
	tmp = m_v_plc[i++];
	one.sw_ballast_tank3p_level = (tmp == 0xFFFF) ? io_error: tmp/1000.0;
	tmp = m_v_plc[i++];
	one.sw_ballast_tank3s_level = (tmp == 0xFFFF) ? io_error: tmp/1000.0;
	tmp = m_v_plc[i++];
	one.sw_ballast_tank3m_level = (tmp == 0xFFFF) ? io_error: tmp/1000.0;
	tmp = m_v_plc[i++];
	one.fresh_water_tank1p_level = (tmp == 0xFFFF) ? io_error: tmp/1000.0;
	tmp = m_v_plc[i++];
	one.fresh_water_tank1s_level = (tmp == 0xFFFF) ? io_error: tmp/1000.0;
	tmp = m_v_plc[i++];
	one.fresh_water_tank2p_level = (tmp == 0xFFFF) ? io_error: tmp/1000.0;
	tmp = m_v_plc[i++];
	one.fresh_water_tank2s_level = (tmp == 0xFFFF) ? io_error: tmp/1000.0;
	tmp = m_v_plc[i++];
	one.fuel_oil_store_tank_p_level = (tmp == 0xFFFF) ? io_error: tmp/1000.0;
	tmp = m_v_plc[i++];
	one.fuel_oil_store_tank_s_level = (tmp == 0xFFFF) ? io_error: tmp/1000.0;
	tmp = m_v_plc[i++];
	one.fuel_oil_day_tank_p_level = (tmp == 0xFFFF) ? io_error: tmp/1000.0;
	tmp = m_v_plc[i++];
	one.fuel_oil_day_tank_s_level = (tmp == 0xFFFF) ? io_error: tmp/1000.0;

	tmp = m_v_plc[i++];
	one.sw_ballast_tank1p_capacity = (tmp == 0xFFFF) ? io_error: tmp/10.0;
	tmp = m_v_plc[i++];
	one.sw_ballast_tank1s_capacity = (tmp == 0xFFFF) ? io_error: tmp/10.0;
	tmp = m_v_plc[i++];
	one.sw_ballast_tank2p_capacity = (tmp == 0xFFFF) ? io_error: tmp/10.0;
	tmp = m_v_plc[i++];
	one.sw_ballast_tank2s_capacity = (tmp == 0xFFFF) ? io_error: tmp/10.0;
	tmp = m_v_plc[i++];
	one.sw_ballast_tank3p_capacity = (tmp == 0xFFFF) ? io_error: tmp/10.0;
	tmp = m_v_plc[i++];
	one.sw_ballast_tank3s_capacity = (tmp == 0xFFFF) ? io_error: tmp/10.0;
	tmp = m_v_plc[i++];
	one.sw_ballast_tank3m_capacity = (tmp == 0xFFFF) ? io_error: tmp/10.0;
	tmp = m_v_plc[i++];
	one.fresh_water_tank1p_capacity = (tmp == 0xFFFF) ? io_error: tmp/10.0;
	tmp = m_v_plc[i++];
	one.fresh_water_tank1s_capacity = (tmp == 0xFFFF) ? io_error: tmp/10.0;
	tmp = m_v_plc[i++];
	one.fresh_water_tank2p_capacity = (tmp == 0xFFFF) ? io_error: tmp/10.0;
	tmp = m_v_plc[i++];
	one.fresh_water_tank2s_capacity = (tmp == 0xFFFF) ? io_error: tmp/10.0;
	tmp = m_v_plc[i++];
	one.fuel_oil_store_tank_p_capacity = (tmp == 0xFFFF) ? io_error: tmp/10.0;
	tmp = m_v_plc[i++];
	one.fuel_oil_store_tank_s_capacity = (tmp == 0xFFFF) ? io_error: tmp/10.0;
	tmp = m_v_plc[i++];
	one.fuel_oil_day_tank_p_capacity = (tmp == 0xFFFF) ? io_error: tmp/10.0;
	tmp = m_v_plc[i++];
	one.fuel_oil_day_tank_s_capacity = (tmp == 0xFFFF) ? io_error: tmp/10.0;

	one.level_mearsurement_system_common_alarm = PLC1_VALUE_DI(m_v_ios,0,9);

	m_client_level_gauging.m_sensor_exception = 0;
	one.device_offline = 0;

	bq_level_gauging.put(one);
	FILELOG_level_gauging(one);
}

void mainService::gen_data_level_switch()
{
	level_switch one;
	int ix = 7;
	int iloop = 10;
	one.fuel_oil_store_tank_p_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.fuel_oil_store_tank_p_low_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.fuel_oil_day_tank_p_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.fuel_oil_day_tank_p_low_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.fuel_oil_store_tank_s_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.fuel_oil_store_tank_s_low_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);

	ix = 8;
	iloop = 0;
	one.fuel_oil_day_tank_s_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.fuel_oil_day_tank_s_low_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.lube_oil_tank_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.dirty_oil_tank_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.black_water_tank_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.grey_water_tank_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.e_g_m_d_o_service_tank_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.e_g_m_d_o_service_tk_low_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.spill_tank_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.b_t_room_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.em_cy_fire_fighting_pump_room_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no1_void_p_tank_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no2_void_p_tank_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no2_void_s_tank_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no3_void_p_tank_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no3_void_s_tank_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	
	ix = 9;
	iloop = 0;
	one.no4_void_p_tank_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no4_void_s_tank_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no5_void_p_tank_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no5_void_s_tank_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no6_void_p_tank_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no6_void_s_tank_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.aux_machinery_room1_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.aux_machinery_room2_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.e_r_ps_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.e_r_sb_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.aft_thruster_rm_ps_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.aft_thruster_rm_sb_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.bilge_water_tank_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no1_expansion_tank_low_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no2_expansion_tank_low_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no1_jackhouse_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	
	ix = 10;
	iloop = 0;
	one.no2_jackhouse_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no3_jackhouse_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no4_jackhouse_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.galley_transformer_room_bilge_low_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.f_o_transfer_pump_room_bilge_low_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.device_offline = 0;
	bq_level_switch.put(one);
	FILELOG_level_switch(one);
}


void mainService::gen_data_gas_detection_system()
{
	int ix,iloop;
	gas_detection_system one;
	memset(&one,0,sizeof(one));

	ix = 0;
	iloop = 10;
	one.gas_detection_alarm_system_failure = PLC1_VALUE_DI(m_v_ios,ix,iloop);

	ix = 388;
	iloop = 0;
	one.gas_detector_fault_top_of_wheelhouse = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_pre_alarm_top_of_wheelhouse = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level1_alarm_top_of_wheelhouse = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level2_alarm_top_of_wheelhouse = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_disabled_top_of_wheelhouse = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_fault_air_intake_of_b_deck_ahu = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_pre_alarm_air_intake_of_b_deck_ahu = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level1_alarm_air_intake_of_b_deck_ahu = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level2_alarm_air_intake_of_b_deck_ahu = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_disabled_air_intake_of_b_deck_ahu = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_fault_air_intake_of_a_deck_ahu = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_pre_alarm_air_intake_of_a_deck_ahu = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level1_alarm_air_intake_of_a_deck_ahu = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level2_alarm_air_intake_of_a_deck_ahu = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_disabled_air_intake_of_a_deck_ahu = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_fault_air_intake_of_below_main_deck_ahu = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	
	ix++;
	iloop = 0;
	one.gas_pre_alarm_air_intake_of_below_main_deck_ahu = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level1_alarm_air_intake_of_below_main_deck_ahu = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level2_alarm_air_intake_of_below_main_deck_ahu = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_disabled_air_intake_of_below_main_deck_ahu = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_fault_air_intake_of_bow_thruster_room = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_pre_alarm_air_intake_of_bow_thruster_room = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level1_alarm_air_intake_of_bow_thruster_room = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level2_alarm_air_intake_of_bow_thruster_room = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_disabled_air_intake_of_bow_thruster_room = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_fault_air_intake_of_aft_thruster_roomp = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_pre_alarm_air_intake_of_aft_thruster_roomp = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level1_alarm_air_intake_of_aft_thruster_roomp = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level2_alarm_air_intake_of_aft_thruster_roomp = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_disabled_air_intake_of_aft_thruster_roomp = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_fault_air_intake_of_aft_thruster_rooms = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_pre_alarm_air_intake_of_aft_thruster_rooms = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
	one.gas_level1_alarm_air_intake_of_aft_thruster_rooms = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level2_alarm_air_intake_of_aft_thruster_rooms = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_disabled_air_intake_of_aft_thruster_rooms = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_fault_air_intake_of_engine_roomp = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_pre_alarm_air_intake_of_engine_roomp = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level1_alarm_air_intake_of_engine_roomp = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level2_alarm_air_intake_of_engine_roomp = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_disabled_air_intake_of_engine_roomp = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_fault_air_intake_of_engine_rooms = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_pre_alarm_air_intake_of_engine_rooms = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level1_alarm_air_intake_of_engine_rooms = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level2_alarm_air_intake_of_engine_rooms = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_disabled_air_intake_of_engine_rooms = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_fault_air_intake_of_aux_machinery_room1 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_pre_alarm_air_intake_of_aux_machinery_room1 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level1_alarm_air_intake_of_aux_machinery_room1 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
	one.gas_level2_alarm_air_intake_of_aux_machinery_room1 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_disabled_air_intake_of_aux_machinery_room1 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_fault_air_intake_of_aux_machinery_room2 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_pre_alarm_air_intake_of_aux_machinery_room2 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level1_alarm_air_intake_of_aux_machinery_room2 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level2_alarm_air_intake_of_aux_machinery_room2 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_disabled_air_intake_of_aux_machinery_room2 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_fault_b_deck_entrancefr109p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_pre_alarm_b_deck_entrancefr109p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level1_alarm_b_deck_entrancefr109p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level2_alarm_b_deck_entrancefr109p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_disabled_b_deck_entrancefr109p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_fault_b_deck_entrancefr109s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_pre_alarm_b_deck_entrancefr109s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level1_alarm_b_deck_entrancefr109s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level2_alarm_b_deck_entrancefr109s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
	one.gas_detector_disabled_b_deck_entrancefr109s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_fault_wheelhouse_entrancep = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_pre_alarm_wheelhouse_entrancep = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level1_alarm_wheelhouse_entrancep = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level2_alarm_wheelhouse_entrancep = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_disabled_wheelhouse_entrancep = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_fault_wheelhouse_entrances = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_pre_alarm_wheelhouse_entrances = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level1_alarm_wheelhouse_entrances = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level2_alarm_wheelhouse_entrances = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_disabled_wheelhouse_entrances = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
#ifdef JANA505	
	one.gas_detector_fault_b_deck_entrancefr90p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_pre_alarm_b_deck_entrancefr90p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level1_alarm_b_deck_entrancefr90p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level2_alarm_b_deck_entrancefr90p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_disabled_b_deck_entrancefr90p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
	one.gas_detector_fault_b_deck_entrancefr90s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_pre_alarm_b_deck_entrancefr90s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level1_alarm_b_deck_entrancefr90s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level2_alarm_b_deck_entrancefr90s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_disabled_b_deck_entrancefr90s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
#endif
#ifdef JANA508509
	one.gas_detector_fault_b_deck_entrancefr100p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_pre_alarm_b_deck_entrancefr100p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level1_alarm_b_deck_entrancefr100p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level2_alarm_b_deck_entrancefr100p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_disabled_b_deck_entrancefr100p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
	one.gas_detector_fault_b_deck_entrancefr100s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_pre_alarm_b_deck_entrancefr100s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level1_alarm_b_deck_entrancefr100s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level2_alarm_b_deck_entrancefr100s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_disabled_b_deck_entrancefr100s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
#endif
	one.gas_detector_fault_a_deck_entrancefr109p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_pre_alarm_a_deck_entrancefr109p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level1_alarm_a_deck_entrancefr109p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level2_alarm_a_deck_entrancefr109p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_disabled_a_deck_entrancefr109p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_fault_a_deck_entrancefr109s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_pre_alarm_a_deck_entrancefr109s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level1_alarm_a_deck_entrancefr109s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level2_alarm_a_deck_entrancefr109s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_disabled_a_deck_entrancefr109s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
#ifdef JANA505
	one.gas_detector_fault_a_deck_entrancefr90p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
	one.gas_pre_alarm_a_deck_entrancefr90p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level1_alarm_a_deck_entrancefr90p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level2_alarm_a_deck_entrancefr90p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_disabled_a_deck_entrancefr90p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_fault_a_deck_entrancefr90s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_pre_alarm_a_deck_entrancefr90s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level1_alarm_a_deck_entrancefr90s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level2_alarm_a_deck_entrancefr90sp = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_disabled_a_deck_entrancefr90s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_fault_main_deck_entrancefr81m = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_pre_alarm_main_deck_entrancefr81m = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level1_alarm_main_deck_entrancefr81m = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level2_alarm_main_deck_entrancefr81m = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_disabled_main_deck_entrancefr81m = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_fault_main_deck_entrancefr81s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_pre_alarm_main_deck_entrancefr81s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
	one.gas_level1_alarm_main_deck_entrancefr81s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level2_alarm_main_deck_entrancefr81s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_disabled_main_deck_entrancefr81s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_fault_air_intake_of_construction_equipment_room = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_pre_alarm_air_intake_of_construction_equipment_room = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level1_alarm_air_intake_of_construction_equipment_room = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level2_alarm_air_intake_of_construction_equipment_room = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_disabled_air_intake_of_construction_equipment_room = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
#endif
#ifdef JANA508509
	one.gas_detector_fault_a_deck_entrancefr91p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
	one.gas_pre_alarm_a_deck_entrancefr91p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level1_alarm_a_deck_entrancefr91p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level2_alarm_a_deck_entrancefr91p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_disabled_a_deck_entrancefr91p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_fault_a_deck_entrancefr91s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_pre_alarm_a_deck_entrancefr91s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level1_alarm_a_deck_entrancefr91s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level2_alarm_a_deck_entrancefr91sp = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_disabled_a_deck_entrancefr91s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_fault_main_deck_entrancefr91m = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_pre_alarm_main_deck_entrancefr91m = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level1_alarm_main_deck_entrancefr91m = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level2_alarm_main_deck_entrancefr91m = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_disabled_main_deck_entrancefr91m = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_fault_main_deck_entrancefr91s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_pre_alarm_main_deck_entrancefr91s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
	one.gas_level1_alarm_main_deck_entrancefr91s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level2_alarm_main_deck_entrancefr91s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_disabled_main_deck_entrancefr91s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
/*	one.gas_detector_fault_air_intake_of_construction_equipment_room = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_pre_alarm_air_intake_of_construction_equipment_room = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level1_alarm_air_intake_of_construction_equipment_room = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_level2_alarm_air_intake_of_construction_equipment_room = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.gas_detector_disabled_air_intake_of_construction_equipment_room = PLC1_VALUE_DI(m_v_plc,ix,iloop++);*/
	iloop += 5;
#endif

	one.toxic_gas_detector_fault_top_of_wheelhouse = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_pre_alarm_top_of_wheelhouse = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_level1_alarm_top_of_wheelhouse = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_level2_alarm_top_of_wheelhouse = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_detector_disabled_top_of_wheelhouse = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_detector_fault_air_intake_of_b_deck_ahu = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_pre_alarm_air_intake_of_b_deck_ahu = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_level1_alarm_air_intake_of_b_deck_ahu = PLC1_VALUE_DI(m_v_plc,ix,iloop++);


	ix++;
	iloop = 0;
	one.toxic_gas_level2_alarm_air_intake_of_b_deck_ahu = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_detector_disabled_air_intake_of_b_deck_ahu = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_detector_fault_air_intake_of_a_deck_ahu = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_pre_alarm_air_intake_of_a_deck_ahu = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_level1_alarm_air_intake_of_a_deck_ahu = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_level2_alarm_air_intake_of_a_deck_ahu = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_detector_disabled_air_intake_of_a_deck_ahu = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_detector_fault_air_intake_of_below_main_deck_ahu = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_pre_alarm_air_intake_of_below_main_deck_ahu = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_level1_alarm_air_intake_of_below_main_deck_ahu = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_level2_alarm_air_intake_of_below_main_deck_ahu = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_detector_disabled_air_intake_of_below_main_deck_ahu = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_detector_fault_air_intake_of_bow_thruster_room = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_pre_alarm_air_intake_of_bow_thruster_room = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_level1_alarm_air_intake_of_bow_thruster_room = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_level2_alarm_air_intake_of_bow_thruster_room = PLC1_VALUE_DI(m_v_plc,ix,iloop++);


	ix++;
	iloop = 0;
	one.toxic_gas_detector_disabled_air_intake_of_bow_thruster_room = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_detector_fault_air_intake_of_aft_thruster_roomp = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_pre_alarm_air_intake_of_aft_thruster_roomp = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_level1_alarm_air_intake_of_aft_thruster_roomp = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_level2_alarm_air_intake_of_aft_thruster_roomp = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_detector_disabled_air_intake_of_aft_thruster_roomp = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_detector_fault_air_intake_of_aft_thruster_rooms = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_pre_alarm_air_intake_of_aft_thruster_rooms = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_level1_alarm_air_intake_of_aft_thruster_rooms = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_level2_alarm_air_intake_of_aft_thruster_rooms = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_detector_disabled_air_intake_of_aft_thruster_rooms = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_detector_fault_air_intake_of_engine_roomp = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_pre_alarm_air_intake_of_engine_roomp = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_level1_alarm_air_intake_of_engine_roomp = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_level2_alarm_air_intake_of_engine_roomp = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_detector_disabled_air_intake_of_engine_roomp = PLC1_VALUE_DI(m_v_plc,ix,iloop++);


	ix++;
	iloop = 0;
	one.toxic_gas_detector_fault_air_intake_of_engine_rooms = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_pre_alarm_air_intake_of_engine_rooms = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_level1_alarm_air_intake_of_engine_rooms = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_level2_alarm_air_intake_of_engine_rooms = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_detector_disabled_air_intake_of_engine_rooms = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_detector_fault_air_intake_of_aux_machinery_room1 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_pre_alarm_air_intake_of_aux_machinery_room1 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_level1_alarm_air_intake_of_aux_machinery_room1 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_level2_alarm_air_intake_of_aux_machinery_room1 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_detector_disabled_air_intake_of_aux_machinery_room1 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_detector_fault_air_intake_of_aux_machinery_room2 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_pre_alarm_air_intake_of_aux_machinery_room2 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_level1_alarm_air_intake_of_aux_machinery_room2 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_level2_alarm_air_intake_of_aux_machinery_room2 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_detector_disabled_air_intake_of_aux_machinery_room2 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_detector_fault_b_deck_entrancefr109p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);


	ix++;
	iloop = 0;
	one.toxic_gas_pre_alarm_b_deck_entrancefr109p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_level1_alarm_b_deck_entrancefr109p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_level2_alarm_b_deck_entrancefr109p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_detector_disabled_b_deck_entrancefr109p = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_detector_fault_b_deck_entrancefr109s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_pre_alarm_b_deck_entrancefr109s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_level1_alarm_b_deck_entrancefr109s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_level2_alarm_b_deck_entrancefr109s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.toxic_gas_detector_disabled_b_deck_entrancefr109s = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	
	one.device_offline = 0;

	bq_gas_detection_system.put(one);
	FILELOG_gas_detection_system(one);
}

void mainService::gen_data_cooling_water_system()
{
	cooling_water_system one;
	int ix = 87;
	//4-20mA
	one.no1_propulsion_sw_cooling_pump_outlet_pressure = PLC1_VALUE_EXCI_ZERO(m_v_ios,ix);ix++;
	one.no2_propulsion_sw_cooling_pump_outlet_pressure = PLC1_VALUE_EXCI_ZERO(m_v_ios,ix);ix++;
	one.no1_propulsion_fw_cooling_pump_outlet_pressure = PLC1_VALUE_EXCI_ZERO(m_v_ios,ix);ix++;
	one.no2_propulsion_fw_cooling_pump_outlet_pressure = PLC1_VALUE_EXCI_ZERO(m_v_ios,ix);ix++;

	one.device_offline = 0;
	bq_cooling_water_system.put(one);
	FILELOG_cooling_water_system(one);
}


void mainService::gen_data_compressed_air_system()
{
	compressed_air_system one;
	int ix = 3;
	int iloop = 0;
	one.no1_air_compressor_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no2_air_compressor_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.service_air_compressor_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	ix = 84;//4-20mA
	one.no1_main_air_reservoir_pressure = PLC1_VALUE_EXCI_ZERO(m_v_ios,ix);ix++;
	one.no2_main_air_reservoir_pressure = PLC1_VALUE_EXCI_ZERO(m_v_ios,ix);ix++;
	one.daily_air_reservoir_pressure = PLC1_VALUE_EXCI_ZERO(m_v_ios,ix);ix++;
	ix = 5;
	iloop = 12;
	one.quick_closing_valve_air_reservoir_low_pressure = PLC1_VALUE_ZERO(m_v_ios,ix);ix++;
	one.air_dryer_common_alarm = PLC1_VALUE_ZERO(m_v_ios,ix);ix++;

	//4-20mA
	ix = 72;
	one.no1_main_deck_air_reservoir_pressure = PLC1_VALUE_ZERO(m_v_ios,ix);ix++;
	one.no2_main_deck_air_reservoir_pressure = PLC1_VALUE_ZERO(m_v_ios,ix);ix++;
	one.no3_main_deck_air_reservoir_pressure = PLC1_VALUE_ZERO(m_v_ios,ix);ix++;	
	one.no4_main_deck_air_reservoir_pressure = PLC1_VALUE_ZERO(m_v_ios,ix);ix++;

	one.device_offline = 0;

	bq_compressed_air_system.put(one);
	FILELOG_compressed_air_system(one);
}

void mainService::gen_data_air_conditioner_refrigeration_system()
{
	air_conditioner_refrigeration_system one;
	int ix = 1;
	int iloop = 10;
	one.m_s_b_room_package_air_condition_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no1_jacking_vfd_room_air_condition_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no2_jacking_vfd_room_air_condition_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++); 
	one.deck_storeroom_air_condition_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no1_wheel_house_air_condition_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no2_wheel_house_air_condition_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	ix = 2;
	iloop = 0;
	one.paint_locker_room_air_condition_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.workshop_air_condition_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no1_galley_package_air_condition_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no2_galley_package_air_condition_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.a_z_v_f_d_room_air_condition_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.chiller_water_pump_system_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no1_a_c_chilled_unit_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no2_a_c_chilled_unit_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no3_a_c_chilled_unit_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);

	ix = 3;
	iloop = 12;
	one.no1_ahu_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no2_ahu_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no3_ahu_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no4_ahu_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);

	ix = 5;
	iloop = 8;
	one.refrigeration_plant_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	//
	//REFRIGERANT LEAKING ALARM  制冷剂泄露报警
	one.refrigerant_leaking_alarm = PLC2_VALUE_DI(m_v_ios,1,5);

	one.device_offline = 0;

	bq_air_conditioner_refrigeration_system.put(one);
	FILELOG_air_conditioner_refrigeration_system(one);
}


void mainService::gen_data_miscellaneous()
{
	miscellaneous one;
	int ix;
	int iloop;

	ix = 0;
	iloop = 0;
	one.no1_ups_for_ams_power_failure = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no2_ups_for_ams_power_failure = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no3_ups_for_ams_power_failure = PLC1_VALUE_DI(m_v_ios,ix,14);
	

	ix = 1;
	iloop = 0;
	one.fog_horn_controller_power_failure = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.bnwas_main_unit_power_failure = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.fresh_water_hydrophore_failure = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.sea_water_hydrophore_failure = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.calorifier_failure = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	iloop++;
	one.navigation_aids_system_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.fire_alarm_control_unit_system_failure= PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.fire_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);

    fire_alarm_trige = one.fire_alarm;  //fire alarm

	one.water_sprinkler_system_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);

	ix = 2;
	iloop = 9;
	one.no1_leg_control_unit_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no2_leg_control_unit_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no3_leg_control_unit_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no4_leg_control_unit_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.public_address_general_alarm_system_fault = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.public_address_general_alarm_system_power_fault = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.auto_telephone_system_failure = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.audio_visual_alarm_column_main_power_failure = PLC1_VALUE_DI(m_v_ios,0,12); 
	one.audio_visual_alarm_column_backup_power_failure = PLC1_VALUE_DI(m_v_ios,0,13); 


	
	ix = 3;
	iloop = 3;
	one.sewage_water_treatment_plant_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.fuel_oil_purifier_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.oil_water_separator_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.oil_water_separator15ppm_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.oil_water_separator_running_timeout_alarm_nc = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no1_fresh_water_generator_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no2_fresh_water_generator_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.fresh_water_generator_no1_boost_pump_overload = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.fresh_water_generator_no2_boost_pump_overload = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	
	ix = 4;
	iloop = 13;
	one.co2_release_alarm_system_power_failure_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.co2_leakage_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.co2_release_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);

	ix = 5;
	iloop = 0;
	one.e_r_co2_release_pre_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.e_r_co2_release_valve_opened = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.paint_room_co2_release_pre_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.paint_room_co2_release_valve_opened = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no1_anti_fouling_device_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no2_anti_fouling_device_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.ac440v_galley_equipment_db_low_insulation = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.ac230v_galley_equipment_db_low_insulation = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	iloop++;
	one.uv_sterilizer_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no1_crane_fire_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no2_crane_fire_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	iloop++;
	iloop++;
	one.no1_submersible_pump_failure = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no2_submersible_pump_failure = PLC1_VALUE_DI(m_v_ios,ix,iloop++);

	ix = 6;
	iloop = 0;
	one.no3_submersible_pump_failure = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no4_submersible_pump_failure = PLC1_VALUE_DI(m_v_ios,ix,iloop++);

	ix = 10;
	iloop = 8;
	one.no3_submersible_pump_running = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no4_submersible_pump_running = PLC1_VALUE_DI(m_v_ios,ix,iloop++);

	one.engineer_alarm_system_power_failure = PLC1_VALUE_DI(m_v_ios,0,11);
	one.watertight_door_indication_system_common_alarm = PLC1_VALUE_DI(m_v_ios,0,12);
	one.freshwater_filling_flowmeter = PLC1_VALUE_ZERO(m_v_ios,91);

	one.device_offline = 0;
	
	bq_miscellaneous.put(one);
	FILELOG_miscellaneous(one);
}


void mainService::write_buzzer(int i)
{
	while(1)
	{
		char status = m_client_buzzer[i].bq_oper_status.take();
		std::lock_guard<std::mutex> lock(*m_502_mux);

		sgo_log_stderr(0,"buzzer[%d] called status:%d.[0:silence,1:call,m_buzzer_status:%d]",i,status,m_buzzer_status);	

		if(status == 0)	//silence
		{
			if(((m_buzzer_status>>i) & 1) == 0)//already silence
				continue;		
			int res = m_mb_502->modbus_write_register(3103+2*i,1);//置1
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			m_mb_502->modbus_write_register(3103+2*i,0);//置0
		}
		else //call
		{
            printf("m_buzzer_status:%d\n",m_buzzer_status);		
			//已经在响，无需再下发
			if((m_buzzer_status>>i) & 1)
				continue;
			
			m_mb_502->modbus_write_register(3115+i,1);
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
			m_mb_502->modbus_write_register(3115+i,0);				
		}		
	}
}



void mainService::call_extend_alarm_panel(int i)
{
	while(1)
	{
		auto status = m_client_extend_alarm_panel[i].bq_call_status.take();
		if(m_mb_plc1)
		{
			std::lock_guard<std::mutex> lock(*m_plc1_rw_mux);
			if((m_buzzer_status>>(4+i)) & 1)
				continue;
			
			m_mb_plc1->modbus_write_register(3119 + i,0);
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
			m_mb_plc1->modbus_write_register(3119 + i,1);
		}
	}
}

void mainService::check_sau_connection()
{
	//每5s往plc1的49991写0，plc里面处理
	while(1)
	{
		if(m_mb_plc1)
		{			
			std::lock_guard<std::mutex> lock(*m_plc1_rw_mux);
			m_mb_plc1->modbus_write_register(9990,0);
		}
		std::this_thread::sleep_for(std::chrono::seconds(5));
	}
}




void mainService::gen_data_buzzer_and_extend_alarm_panel_test()
{
	while(1)
	{
		uint16_t status = 0;
		uint16_t btn_status[12]={0};
		uint16_t ex_status = 0;
		extend_alarm_panel ex;
		buzzer one;
		auto start = std::chrono::high_resolution_clock::now();
		{
			//plc 504 test
			//std::lock_guard<std::mutex> lock(*m_plc1_rw_mux);
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}

		for(int i = 0; i < 3; i++)
		{
			one.no = i;
			one.status =  (status>>i)&1 ? 2 : 1;
			one.mute = btn_status[2*i+1];
			one.ack = btn_status[2*i+2];

			one.device_offline = 0;
			bq_buzzer[i].put(one);
			//FILELOG_buzzer(one);
//printf("buzeer [%d] mute:%d,ack:%d,status:%d\n",one.no,one.mute,one.ack,one.status);
				
		}
		

		for(int i = 0; i < 5;i++)
		{
			ex.no = i;
			if((ex_status>>(9+i)) & 1)//掉线
			{
				ex.status = 0;
				ex.device_offline = 1;
			}
			else
			{
				ex.status = (status>>(4+i)) & 1 ? 2 : 1;						
				ex.mute_or_ack =  btn_status[8+i];
				ex.device_offline = 0;
			}
			bq_extend_alarm_panel[i].put(ex);
			//FILELOG_extend_alarm_panel(ex);
//printf("extend [%d] status:%d offline:%d\n",ex.no,ex.status,ex.device_offline);
			
		}
		
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> duration = end - start;
	//	std::cout << "Function took " << duration.count() << " seconds" << std::endl;

		std::this_thread::sleep_for(std::chrono::milliseconds(300));
	}
}


void mainService::gen_data_buzzer_and_extend_panel()
{
	while(1)
	{
		if(!m_mb_plc1_504)
		{
			m_mb_plc1_504 = new modbus(SAU_IP1,SIEMENS_PORT_504);
		}
		if(!m_mb_plc1_504->is_connected())
		{
			if(!m_mb_plc1_504->modbus_connect())
			{
				sleep(3);
				continue;
			}
		}

		sgo_log_stderr(0,"modbus connect AMS %s:%d success.",SAU_IP1,SIEMENS_PORT_504);

		while(1)
		{
			uint16_t status[40] = {0};
			extend_alarm_panel ex;
			buzzer bz;
			{
				//std::lock_guard<std::mutex> lock(m_plc1_read_mux);
#ifdef DEMO
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		goto loopdemo;
#endif	
				if(m_mb_plc1_504->modbus_read_holding_registers(3102,40,status))
				{
					break;
				}
				m_buzzer_status = status[0];
			}
	loopdemo:
			for(int i = 0; i < 3; i++)
			{
				bz.no = i;
				bz.status =  (status[0]>>i)&1 ? 2 : 1;
				bz.mute = status[2*i+1];
				bz.ack = status[2*i+2];

				bz.device_offline = 0;
				bq_buzzer[i].put(bz);
				FILELOG_buzzer(bz);
	//printf("buzeer [%d] mute:%d,ack:%d,status:%d\n",bz.no,bz.mute,bz.ack,bz.status);
			}
			
			for(int i = 0; i < 5;i++)
			{
				ex.no = i;
				if(status[22+i] == 1)//掉线
				{
					ex.status = 0;
					ex.device_offline = 1;
				}
				else
				{
					ex.status = (status[0] >> (4+i)) & 1 ? 2 : 1;						
					ex.mute_or_ack =  status[8+i];
					ex.device_offline = 0;
				}
				bq_extend_alarm_panel[i].put(ex);
				FILELOG_extend_alarm_panel(ex);
	//printf("extend [%d] status:%d offline:%d\n",ex.no,ex.status,ex.device_offline);
			}	
				
			//sleep(1);
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
		}
		m_mb_plc1_504->modbus_close();
		sleep(3);
	}
}



/* calc time 
auto start = std::chrono::high_resolution_clock::now();	
auto end = std::chrono::high_resolution_clock::now();
std::chrono::duration<double> duration = end - start;
std::cout << "Function took " << duration.count() << " seconds" << std::endl;
*/

//////////////////////////////////////////////////////////////////////////////////////////////////
//modified:
//use port 503
//
//
//
void mainService::getDatasFromPLC1()
{
#ifndef MAHTS648
	if (!m_mb_plc1_503)
	{
		m_mb_plc1_503 = new modbus(SAU_IP1, SIEMENS_PORT_503);
		m_mb_plc1_503->modbus_set_slave_id(1);
	}
#endif

	while (1)
	{

        sgo_log_stderr(0, "1");
		std::this_thread::sleep_for(std::chrono::milliseconds(10));    

        if (!m_mb_502->is_connected())
		{
			if (!m_mb_502->modbus_connect())
			{
				sleep(3);
				continue;
			}
		}
        sgo_log_stderr(0, "2");
		std::this_thread::sleep_for(std::chrono::milliseconds(10));  

		// if (!m_mb_503->is_connected())
		// {
		// 	if (!m_mb_503->modbus_connect())
		// 	{
		// 		sleep(3);
		// 		continue;
		// 	}
		// }

		// if (!m_mb_504->is_connected())
		// {
		// 	if (!m_mb_504->modbus_connect())
		// 	{
		// 		sleep(3);
		// 		continue;
		// 	}
		// }

		if (!m_mb_506_sau1->is_connected())
		{
			if (!m_mb_506_sau1->modbus_connect())
			{
				sleep(3);
				continue;
			}
		}
        sgo_log_stderr(0, "3");
		std::this_thread::sleep_for(std::chrono::milliseconds(10));  

		
      

		memset(m_v_ios, 0, sizeof(m_v_ios));
		memset(m_v_plc, 0, sizeof(m_v_plc));
		memset(m_v_ios_alarm, 0, sizeof(m_v_ios_alarm));
		while (1)
		{
            uint64_t t1 = getTickCount64();
			sgo_log_stderr(0, "4");
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			if (m_mb_502->modbus_read_holding_registers(0, 100, &m_v_plc[0])) // DG
			{
				break;
			}

			sgo_log_stderr(0, "5");
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			if (m_mb_502->modbus_read_holding_registers(100, 100, &m_v_plc[100])) // DG EG
			{
				break;
			}

			sgo_log_stderr(0, "6");
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			if (m_mb_502->modbus_read_holding_registers(200, 100, &m_v_plc[200])) // azimuth_center_control
			{
				break;
			}

			sgo_log_stderr(0, "7");
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			if (m_mb_502->modbus_read_holding_registers(300, 100, &m_v_plc[300])) // POWER, MT
			{
				break;
			}

			sgo_log_stderr(0, "8");
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			if (m_mb_502->modbus_read_holding_registers(400, 100, &m_v_plc[400])) // POWER, MT
			{
				break;
			}

			sgo_log_stderr(0, "9");
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			if (m_mb_502->modbus_read_holding_registers(500, 100, &m_v_plc[500])) // MISCELLANEOUS
			{
				break;
			}

			sgo_log_stderr(0, "10");
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			if (m_mb_502->modbus_read_holding_registers(600, 20, &m_v_plc[600])) // MISCELLANEOUS
			{
				break;
			}

			sgo_log_stderr(0, "11");
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			if (m_mb_502->modbus_read_holding_registers(700, 100, &m_v_plc[700])) // f.o level
			{
				break;
			}

			// if (m_mb_503->modbus_read_holding_registers(3129, 10, m_v_ios))
			// {
			// 	break;
			// }

			m_disconnect = 0;


            memcpy(m_v_plc_w, m_v_plc, sizeof(m_v_plc));
            memcpy(m_v_ios_w, m_v_ios, sizeof(m_v_ios));

            //m_u16_loop_flag = 0;
            m_vecTmpMachinery.clear();

           std::cout<< "!!!!!!!!!!!!!!!!!!!!!!!aaaaaaaaaa!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
		   sgo_log_stderr(0, "12");
		   std::this_thread::sleep_for(std::chrono::milliseconds(10)); 
            gen_data_diesel_generator2();
            gen_data_harbor_generator();
			gen_data_emergency_generator2();
			for(int i = 0; i < 2; i++)
			{
                gen_data_bow_thruster2(i);
			}
			gen_data_level_gauging2();
			gen_data_power_system2();
			gen_data_gas_detection_system2();
			gen_data_general_charger_and_discharger_panel2();
			for(int i = 0; i < 2; i++)
			{
				gen_data_main_transformer2(i);
			}
			
			gen_data_miscellaneous2();
			gen_data_msb();
			gen_data_port_azimuth_center_control_cabinet();
			gen_data_stbd_azimuth_center_control_cabinet();
            gen_data_sewage_treatment_plant();

		    std::cout<< "!!!!!!!!!!!!!!!!!!!!!!!bbbbbbbbb!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;

            //m_machinery_handler.SetCurrAlarms(m_vecTmpMachinery);

			sgo_log_stderr(0, "13");
			std::this_thread::sleep_for(std::chrono::milliseconds(10)); 

			
			//TBD
			m_506_sau1_mux->lock();
			if (m_mb_506_sau1->modbus_write_registers(0, 100, &m_v_plc_w[0]))
			{
				m_506_sau1_mux->unlock();
				break;
			}
			m_506_sau1_mux->unlock();

            std::this_thread::sleep_for(std::chrono::milliseconds(10)); 

			m_506_sau1_mux->lock();
			if (m_mb_506_sau1->modbus_write_registers(100, 100, &m_v_plc_w[100]))
			{
				m_506_sau1_mux->unlock();
				break;
			}
			m_506_sau1_mux->unlock();

			std::this_thread::sleep_for(std::chrono::milliseconds(10)); 

			m_506_sau1_mux->lock();
			if (m_mb_506_sau1->modbus_write_registers(200, 100, &m_v_plc_w[200]))
			{
				m_506_sau1_mux->unlock();
				break;
			}
			m_506_sau1_mux->unlock();

			std::this_thread::sleep_for(std::chrono::milliseconds(10)); 
			m_506_sau1_mux->lock();
			if (m_mb_506_sau1->modbus_write_registers(300, 100, &m_v_plc_w[300]))
			{
				m_506_sau1_mux->unlock();
				break;
			}
			m_506_sau1_mux->unlock();

			std::this_thread::sleep_for(std::chrono::milliseconds(10)); 

			m_506_sau1_mux->lock();
			if (m_mb_506_sau1->modbus_write_registers(400, 100, &m_v_plc_w[400]))
			{
				m_506_sau1_mux->unlock();
				break;
			}
			m_506_sau1_mux->unlock();

			std::this_thread::sleep_for(std::chrono::milliseconds(10)); 

			m_506_sau1_mux->lock();
			if (m_mb_506_sau1->modbus_write_registers(500, 100, &m_v_plc_w[500]))
			{
				m_506_sau1_mux->unlock();
				break;
			}
			m_506_sau1_mux->unlock();

			std::this_thread::sleep_for(std::chrono::milliseconds(10)); 

			m_506_sau1_mux->lock();
			if (m_mb_506_sau1->modbus_write_registers(600, 20, &m_v_plc_w[600]))
			{
				m_506_sau1_mux->unlock();
				break;
			}
			m_506_sau1_mux->unlock();



            sgo_log_stderr(0, "14");
			
            std::this_thread::sleep_for(std::chrono::milliseconds(10)); 
            auto t2 = getTickCount64();
			uint64_t elapsed = t2 - t1;
			if (elapsed < 1000)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(1000 - elapsed));
			}
		}

        sgo_log_stderr(0, "15");
		m_mb_502->modbus_close(); // cummins
		m_mb_506_sau1->modbus_close();
		m_disconnect = 1;
		sleep(3);
	}
}

void mainService::setAmsLost(int flag)
{
	//主发电机组
	for(int i = 0; i < 6; i++)
	{
		main_diesel_generator one;
		one.gen_id = i;
		one.device_offline = flag;
		bq_main_diesel_generator[i].put(one);
		m_client_main_gens[i].m_sensor_exception = flag;
	}

	

	for(int i = 0; i < 2; i++)
	{
		//主推进
		m_client_main_propulsion[i].m_sensor_exception = flag;
		main_propulsion prop;
		prop.no = i;
		prop.device_offline = flag;
		bq_main_propulsion[i].put(prop);

		//艏侧推起动器
		m_client_bow_thruster[i].m_sensor_exception = flag;
		bow_thruster bt;
		bt.no = i;
		bt.device_offline = flag;
		bq_bow_thruster[i].put(bt);
		

		//主变压器
		m_client_main_transformer[i].m_sensor_exception = flag;
		main_transformer mt;
		mt.no = i;
		mt.device_offline = flag;
		bq_main_transformer[i].put(mt);		
	}
	
	//应急发电机组
	m_client_emergency_generator.m_sensor_exception = flag;
	emergency_generator eg;
	eg.device_offline = flag;
	bq_emergency_generator.put(eg);

	//配电系统
	m_client_power_system.m_sensor_exception = flag;
	power_system ps;
	ps.device_offline = flag;
	bq_power_system.put(ps);
	
	//通用充放电板
	m_client_general_charger_and_discharger_panel.m_sensor_exception = flag;
	general_charger_and_discharger_panel gc;
	gc.device_offline = flag;
	bq_general_charger_and_discharger_panel.put(gc);

	//机舱充放电板
	m_client_e_r_charger_and_discharger_panel.m_sensor_exception = flag;
	e_r_charger_and_discharger_panel er;
	er.device_offline = flag;
	bq_e_r_charger_and_discharger_panel.put(er);

	//空调
	m_client_air_conditioner_refrigeration_system.m_sensor_exception = flag;
	air_conditioner_refrigeration_system ac;
	ac.device_offline = flag;
	bq_air_conditioner_refrigeration_system.put(ac);

	//空压机
	m_client_compressed_air_system.m_sensor_exception = flag;
	compressed_air_system ca;
	ca.device_offline = flag;
	bq_compressed_air_system.put(ca);

	//液位遥测系统
	m_client_level_gauging.m_sensor_exception = flag;
	level_gauging lg;
	lg.device_offline = flag;
	bq_level_gauging.put(lg);

	//液位开关
	m_client_level_switch.m_sensor_exception = flag;
	level_switch ls;
	ls.device_offline = flag;
	bq_level_switch.put(ls);

	//气体探测系统
	m_client_gas_detection_system.m_sensor_exception = flag;
	gas_detection_system gs;
	gs.device_offline = flag;
	bq_gas_detection_system.put(gs);

	//其他
	m_client_miscellaneous.m_sensor_exception = flag;
	miscellaneous ms;
	ms.device_offline = flag;
	bq_miscellaneous.put(ms);

	//冷却水系统
	m_client_cooling_water_system.m_sensor_exception = flag;
	cooling_water_system cs;
	cs.device_offline = flag;
	bq_cooling_water_system.put(cs);

    //co2系统
	m_client_co2_system.m_sensor_exception = flag;
    co2_system csm;
	ms.device_offline = flag;
	bq_co2_system.put(csm);

	//buzzer
	for(int i = 0;i < 3;i++)
	{
		buzzer bz;
		bz.no = i;
		bz.device_offline = flag;
		bq_buzzer[i].put(bz);
	}
	//extend alarm panel
	for(int i = 0; i < 5; i++)
	{
		extend_alarm_panel ep;
		ep.no = i;
		ep.device_offline = flag;
		bq_extend_alarm_panel[i].put(ep);
	}
}

void mainService::setMqttProxyLost(int pcIndex,int status)
{
	//主发电机组
	for(int i = 0; i < 6; i++)
	{
		m_client_main_gens[i].m_v_lost[pcIndex] = status;
	}

 	for(int i = 0; i < 6; i++)
	{
		//主推进
		m_client_main_propulsion[i].m_v_lost[pcIndex] = status;
	}

	for(int i = 0; i < 2; i++)
	{


		//艏侧推起动器
		//m_client_bow_thruster[i].m_v_lost[pcIndex] = status;

		//主变压器
		m_client_main_transformer[i].m_v_lost[pcIndex] = status;	
	}
	
	//应急发电机组
	m_client_emergency_generator.m_v_lost[pcIndex] = status;

	//配电系统
	m_client_power_system.m_v_lost[pcIndex] = status;
	
	//通用充放电板
	m_client_general_charger_and_discharger_panel.m_v_lost[pcIndex] = status;

	//机舱充放电板
	m_client_e_r_charger_and_discharger_panel.m_v_lost[pcIndex] = status;

	//空调
	m_client_air_conditioner_refrigeration_system.m_v_lost[pcIndex] = status;

	//空压机
	m_client_compressed_air_system.m_v_lost[pcIndex] = status;

	//液位遥测系统
	m_client_level_gauging.m_v_lost[pcIndex] = status;

	//液位开关
	m_client_level_switch.m_v_lost[pcIndex] = status;

	//气体探测系统
	m_client_gas_detection_system.m_v_lost[pcIndex] = status;

	//其他
	m_client_miscellaneous.m_v_lost[pcIndex] = status;

    //CO2系统    
    m_client_co2_system.m_v_lost[pcIndex] = status;
    

	//冷却水系统
	m_client_cooling_water_system.m_v_lost[pcIndex] = status;

	//buzzer
	for(int i = 0;i < 3;i++)
	{
		m_client_buzzer[i].m_v_lost[pcIndex] = status;
	}
	//extend alarm panel
	for(int i = 0; i < 5; i++)
	{
		m_client_extend_alarm_panel[i].m_v_lost[pcIndex] = status;
	}
}



#if 0

void mainService::initMqtt()
{

#if 1
	/******************************************AMS start*********************************************/
	//主发电机组
	for(int i = 0; i < 6; i++)
	{
	//	m_client_main_gens[i].init(i+1,bq_main_diesel_generator[i]);
	}
	m_client_main_gens[0].init(1,bq_main_diesel_generator[0],m_mb_plc1,m_plc1_rw_mux);
#endif
#if 1
	//应急发电机组
	m_client_emergency_generator.init(1,bq_emergency_generator);

	for(int i = 0; i < 2; i++)
	{
		//主推进
		m_client_main_propulsion[i].init(i+1,bq_main_propulsion[i]);
		
		//艏侧推
		m_client_bow_thruster[i].init(i+1,bq_bow_thruster[i]);	

		//主变压器
		m_client_main_transformer[i].init(i+1,bq_main_transformer[i]);
	}

	//配电系统
	m_client_power_system.init(1,bq_power_system);

	//通用充放电板
	m_client_general_charger_and_discharger_panel.init(1,bq_general_charger_and_discharger_panel);

	//机舱充放电板
	m_client_e_r_charger_and_discharger_panel.init(1,bq_e_r_charger_and_discharger_panel);

	//液位遥测
	m_client_level_gauging.init(1,bq_level_gauging);
#endif
	//液位开关
	m_client_level_switch.init(1,bq_level_switch);

	//气体探测系统
	m_client_gas_detection_system.init(1,bq_gas_detection_system);
#if 1
	//冷却水系统
	m_client_cooling_water_system.init(1,bq_cooling_water_system);

	//压缩空气系统
	m_client_compressed_air_system.init(1,bq_compressed_air_system);

	//空调冷藏系统
	m_client_air_conditioner_refrigeration_system.init(1,bq_air_conditioner_refrigeration_system);

	//其他
	m_client_miscellaneous.init(1,bq_miscellaneous);
#endif
	/****************************************AMS end**************************************************/
#if 0
	//GPS
	m_client_gps.init(1,bq_gps);	

	//AIS
	m_client_ais.init(1,bq_ais);

	//罗经gyro
	m_client_gyro.init(1,bq_gyro);

	//流量计
	for(int i = 0; i < 10; i++)
		m_client_flowmeters[i].init(i+1,bq_flowmeter[i]);

	//总流量计
	m_client_total_flowmeter.init(1,bq_total_flowmeter);

	//锚链
	m_client_anchorChain.init(1,bq_anchorChain);

	//升降系统
	m_client_platform_jacking_system.init(1,bq_platform_jacking_system);

	//ECDIS
	m_client_ecdis.init(1,bq_ecdis);

	//Echo Sounder
	m_client_echosounder.init(1,bq_echosounder);

	//Wind meter
	m_client_windmeter.init(1,bq_windmeter);


#endif
	//蜂鸣器
	for(int i = 0; i < 3;i++)
	{
		m_client_buzzer[i].init(i+1,bq_buzzer[i]);
	}

	//延伸报警板
	for(int i = 0; i < 5; i++)
	{
		m_client_extend_alarm_panel[i].init(i+1,bq_extend_alarm_panel[i]);
	}
	

}
#endif

void mainService::initMqtt(modbus *mb,mutex* mux)
{

#if 1
	/******************************************AMS start*********************************************/
	//主发电机组
	for(int i = 0; i < main_gens_nums; i++)
	{
		m_client_main_gens[i].init(i+1,bq_main_diesel_generator[i],mb,mux);
	}

	//应急发电机组
	m_client_emergency_generator.init(1,bq_emergency_generator,mb,mux);


	//HARBO_GENERATOR
	m_client_harbor_generator.init(1,bq_harbor_generator,mb,mux);


	for(int i = 0; i < main_propulsion_nums; i++)
	{
	    //艏侧推
		m_client_bow_thruster[i].init(i+1,bq_bow_thruster[i],mb,mux);
	}

	//配电系统
	m_client_power_system.init(1,bq_power_system,mb,mux);

	//通用充放电板
	m_client_general_charger_and_discharger_panel.init(1,bq_general_charger_and_discharger_panel,mb,mux);

	//机舱充放电板
	m_client_e_r_charger_and_discharger_panel.init(1,bq_e_r_charger_and_discharger_panel,mb,mux);

	//液位遥测
	m_client_level_gauging.init(1,bq_level_gauging,mb,mux);

	//液位开关
	m_client_level_switch.init(1,bq_level_switch,mb,mux);

	//气体探测系统
	m_client_gas_detection_system.init(1,bq_gas_detection_system,mb,mux);

	//冷却水系统
	m_client_cooling_water_system.init(1,bq_cooling_water_system,mb,mux);

	//压缩空气系统
	m_client_compressed_air_system.init(1,bq_compressed_air_system,mb,mux);

	//空调冷藏系统
	m_client_air_conditioner_refrigeration_system.init(1,bq_air_conditioner_refrigeration_system,mb,mux);

	//其他
	m_client_miscellaneous.init(1,bq_miscellaneous,mb,mux);

	/****************************************AMS end**************************************************/
#endif
#if 1
	//蜂鸣器
	for(int i = 0; i < buzzer_nums;i++)
	{
		m_client_buzzer[i].init(i+1,bq_buzzer[i],mb,mux);
	}

	//延伸报警板
	for(int i = 0; i < extend_alarm_panel_nums; i++)
	{
		m_client_extend_alarm_panel[i].init(i+1,bq_extend_alarm_panel[i],mb,mux);
	}

	//主机pc
	for(int i = 0; i < pc_nums; i++)
		m_client_ams_pc[i].init(i+1,bq_ams_pc[i],mb,mux);

	//SAU
	for(int i = 0; i < sau_nums; i++)
		m_client_ams_sau[i].init(i+1,bq_ams_sau[i],mb,mux);	
#endif
}

void mainService::initMqtt2()
{
#if 1
	/******************************************AMS start*********************************************/
	//主发电机组
	for(int i = 0; i < main_gens_nums; i++)
	{
		m_client_main_gens[i].init(i+1,bq_main_diesel_generator[i],m_mb_502,m_502_mux);
		(m_client_main_gens[i]).m_pCurIpAddr = &m_cur_pc_ip;
	}

#if  defined(JANA508509) || defined(MAHTS648)
    m_client_harbor_generator.init(1, bq_harbor_generator,m_mb_502,m_502_mux);
	(m_client_harbor_generator).m_pCurIpAddr = &m_cur_pc_ip;
#endif

	//应急发电机组
	m_client_emergency_generator.init(1,bq_emergency_generator,m_mb_502,m_502_mux);
	m_client_emergency_generator.m_pCurIpAddr = &m_cur_pc_ip;

	for(int i = 0; i < bow_thruster_nums; i++)
	{
		//主推进
		m_client_bow_thruster[i].init(i+1,bq_bow_thruster[i],m_mb_502,m_502_mux);
		(m_client_bow_thruster[i]).m_pCurIpAddr = &m_cur_pc_ip;
	}

	for(int i = 0; i < main_transformer_nums; i++)
	{
		//主变压器
		m_client_main_transformer[i].init(i+1,bq_main_transformer[i],m_mb_502,m_502_mux);
	}

	//配电系统
	m_client_power_system.init(1,bq_power_system,m_mb_502,m_502_mux);
	m_client_power_system.m_pCurIpAddr = &m_cur_pc_ip;

	//配电系统2
	m_client_power_system2.init(1,bq_power_system2,m_mb_502,m_502_mux);
	m_client_power_system2.m_pCurIpAddr = &m_cur_pc_ip;

	//通用充放电板
	m_client_general_charger_and_discharger_panel.init(1,bq_general_charger_and_discharger_panel,m_mb_502,m_502_mux);
    m_client_general_charger_and_discharger_panel.m_pCurIpAddr = &m_cur_pc_ip;

	//液位遥测
	m_client_level_gauging.init(1,bq_level_gauging,m_mb_502,m_502_mux);
    m_client_level_gauging.m_pCurIpAddr = &m_cur_pc_ip;

	// //液位开关
	// m_client_level_switch.init(1,bq_level_switch,m_mb_503,m_503_mux);
	// m_client_level_switch.m_pCurIpAddr = &m_cur_pc_ip;

	//气体探测系统
	m_client_gas_detection_system.init(1,bq_gas_detection_system,m_mb_502,m_502_mux);
	m_client_gas_detection_system.m_pCurIpAddr = &m_cur_pc_ip;

	// //冷却水系统
	// m_client_cooling_water_system.init(1,bq_cooling_water_system,m_mb_504,m_504_mux);
	// m_client_cooling_water_system.m_pCurIpAddr = &m_cur_pc_ip;

	// //压缩空气系统
	// m_client_compressed_air_system.init(1,bq_compressed_air_system,m_mb_504,m_504_mux);
    // m_client_compressed_air_system.m_pCurIpAddr = &m_cur_pc_ip;

	// //空调冷藏系统
	// m_client_air_conditioner_refrigeration_system.init(1,bq_air_conditioner_refrigeration_system,m_mb_504,m_504_mux);
    // m_client_air_conditioner_refrigeration_system.m_pCurIpAddr = &m_cur_pc_ip;

	//其他
	m_client_miscellaneous.init(1,bq_miscellaneous,m_mb_502,m_502_mux);
    m_client_miscellaneous.m_pCurIpAddr = &m_cur_pc_ip;

    // //CO2系统
    // m_client_co2_system.init(1,bq_co2_system,m_mb_504,m_504_mux);
    // m_client_co2_system.m_pCurIpAddr = &m_cur_pc_ip;

    
    //PORT_AZIMUTH
    m_client_port_azimuth_center_control_cabinet.init(1,bq_port_azimuth_center_control_cabinet,m_mb_502,m_502_mux);

	//STBD_AZIMUTH
    m_client_stbd_azimuth_center_control_cabinet.init(1,bq_stbd_azimuth_center_control_cabinet,m_mb_502,m_502_mux);

	//SEWAGE_TREATMENT_PLANT
    m_client_sewage_treatment_plant.init(1,bq_sewage_treatment_plant,m_mb_502,m_502_mux);

	//MSB
	m_client_msb.init(1,bq_msb,m_mb_502,m_502_mux);


	/****************************************AMS end**************************************************/
#endif
#if 1
	//蜂鸣器
	for(int i = 0; i < buzzer_nums;i++)
	{
		m_client_buzzer[i].init(i+1,bq_buzzer[i],m_mb_502,m_502_mux);
	}

	//延伸报警板
	for(int i = 0; i < extend_alarm_panel_nums; i++)
	{
		m_client_extend_alarm_panel[i].init(i+1,bq_extend_alarm_panel[i],m_mb_502,m_502_mux);
		m_client_extend_alarm_panel[i].m_duty = &m_duty;
	}

	//主机pc
	for(int i = 0; i < pc_nums; i++)
		m_client_ams_pc[i].init(i+1,bq_ams_pc[i],m_mb_502,m_502_mux);

	//SAU
	for(int i = 0; i < sau_nums; i++)
		m_client_ams_sau[i].init(i+1,bq_ams_sau[i],m_mb_502,m_502_mux);	
#endif
}

void mainService::GetHttpAlarms()
{
	namespace http = boost::beast::http;

    while(1)
	{
		try
		{
			// 设置循环间隔时间（以毫秒为单位）
			const std::chrono::milliseconds interval(1000);

            // 延迟指定的时间间隔
			std::this_thread::sleep_for(interval);

			// 设置 Boost.Asio 上下文
			boost::asio::io_context io_context;

			// 创建 HTTP 客户端
			boost::asio::ip::tcp::resolver resolver(io_context);
			boost::asio::ip::tcp::socket socket(io_context);

			// 解析服务器地址
			auto const results = resolver.resolve("192.168.20.248", "9990");

			// 建立连接
			boost::asio::connect(socket, results.begin(), results.end());



			while (true)
			{
				// 构建 HTTP POST 请求
				http::request<http::string_body> req{http::verb::post, "/api/alarm/alarming", 11};
				req.set(http::field::host, "192.168.20.248:9990");
				req.set(http::field::user_agent, "Boost Beast");
				req.set(http::field::content_type, "application/json");
				//req.body() = R"({"key":"value"})";

				// 发送请求并获取响应
				http::write(socket, req);

				boost::beast::flat_buffer buffer;
				http::response<http::string_body> res;
				http::read(socket, buffer, res);

				// 输出响应状态和内容
				std::cout << "Status: " << res.result_int() << std::endl;
				std::cout << "Body: " << res.body() << std::endl;
				auto s = std::make_shared<std::string>();
				*s = res.body();

                //decode json
				rapidjson::Document doc;
				// 首先进行解析，没有解析错误才能进行具体字段的解析
				if (!(doc.Parse(s->data()).HasParseError()))
				{
					memset(m_alarm_h_w, 0, sizeof(m_alarm_h_w));
					const rapidjson::Value &data = doc["data"];
					for (rapidjson::SizeType i = 0; i < data.Size(); i++)
					{
						const rapidjson::Value &item = data[i];

						std::string itemKey = item["itemKey"].GetString();
						std::string deviceCode = item["deviceCode"].GetString();
						int  deviceNum = deviceCode.back() - '0';
						std::string alarmType = item["alarmType"].GetString();

						if (deviceNum > 0)
						{
							if ("H-A" == alarmType)
							{
								if (std::string::npos != deviceCode.find("main_generator"))
								{
									m_client_main_gens[deviceNum - 1].UpdateHa(itemKey, m_alarm_h_w, sizeof(m_alarm_h_w));
								}
								else if (std::string::npos != deviceCode.find("emergency_generator"))
								{
									m_client_emergency_generator.UpdateHa(itemKey, m_alarm_h_w, sizeof(m_alarm_h_w));
								}
								else
								{
									// TBD
								}
							}
							else if ("L-A" == alarmType)
							{
								if (std::string::npos != deviceCode.find("cooling_water"))
								{
									m_client_cooling_water_system.UpdateLa(itemKey, m_alarm_h_w, sizeof(m_alarm_h_w));
								}
								else if (std::string::npos != deviceCode.find("compressed_air"))
								{
									m_client_compressed_air_system.UpdateLa(itemKey, m_alarm_h_w, sizeof(m_alarm_h_w));
								}
								else
								{
									// TBD
								}
							}
						}
					}

                    // if(m_506_sau1_mux && m_mb_506_sau1 && m_mb_506_sau1->is_connected())
					// {
					// 	m_506_sau1_mux->lock();
                    //     m_mb_506_sau1->modbus_write_registers(199, 10, m_alarm_h_w);
					// 	m_506_sau1_mux->unlock();
					// }
				}

				// 延迟指定的时间间隔
				std::this_thread::sleep_for(interval);
			}
		}
		catch (std::exception const &e)
		{
			std::cerr << "Error: " << e.what() << std::endl;

			std::string s = e.what();
		}
	}

	return;
}

////////////////////test start///////////////////////////////////////////////////////////////////////////
void mainService::genRandTest()
{
	srand(time(NULL));

	while(1)
	{
		getDatasFromAMS_TEST(); 

	}

}

void mainService::getDatasFromAMS_TEST()
{
	{
	//test plc 503
		//std::lock_guard<std::mutex> lock(*m_plc1_rw_mux);
		this_thread::sleep_for(chrono::seconds(1));
	}
#if 1
	gen_data_main_diesel_generator_test();
#ifdef JANA508509
	gen_data_baudouin_main_generator_test();
#endif
	gen_data_emergency_generator_test();
	gen_data_bow_thruster_test();
	gen_data_main_propulsion_test();

	for(int i = 0; i < 2; i++)
	{	
		gen_data_main_transformer_test(i);
	}

	gen_data_power_system_test();
	gen_data_general_charger_and_discharger_panel_test();
	gen_data_e_r_charger_and_discharger_panel_test();
	gen_data_level_gauging_test();
	gen_data_level_switch_test();
	gen_data_gas_detection_system_test();
	
	gen_data_cooling_water_system_test();
	gen_data_compressed_air_system_test();
#endif
	gen_data_air_conditioner_refrigeration_system_test();
	gen_data_miscellaneous_test();


}

void mainService::gen_data_baudouin_main_generator_test()
{
	static int loop[2] = {0};
	static baudouin_main_generator one[2];

	for(int i = 0; i < baudouin_main_gens_nums;++i)
	{	
		if(loop[i]++ % 10 == 0)
		{	
			one[i].gen_id = i;
			one[i].d_g_engine_rpm = RANDMOD(1000);
			one[i].d_g_battery_voltage = RANDMOD(1000);
			one[i].d_g_boost_t_ps = RANDMOD(1000);
			one[i].d_g_boost_t_sb = RANDMOD(1000);
			one[i].d_g_speed_r = RANDMOD(1000);
			one[i].d_g_speed_ma = RANDMOD(1000);
			one[i].d_g_oil_piston_jet = RANDMOD(1000);
			one[i].d_g_coolant_press = RANDMOD(1000);
			one[i].d_g_exhaust_t_sb = RANDMOD(1000);
			one[i].d_g_exhaust_t_ps = RANDMOD(1000);
			one[i].d_g_raw_wat_press = RANDMOD(1000);
			one[i].d_g_load = RANDMOD(1000);
			one[i].d_g_cool_temp_sb = RANDMOD(1000);
			one[i].d_g_fuel_temp = RANDMOD(1000);
			one[i].d_g_oil_temp = RANDMOD(1000);
			one[i].d_g_intake_p_sb = RANDMOD(1000);
			one[i].d_g_intake_t_sb = RANDMOD(1000);
			one[i].d_g_oil_press = RANDMOD(1000);
			one[i].d_g_fuel_press = RANDMOD(1000);
			one[i].d_g_fuel_rate = RANDMOD(1000);
			one[i].d_g_ecu_battvol_m = RANDMOD(1000);
			one[i].d_g_ecu_temp_m = RANDMOD(1000);
			one[i].d_g_lever_pos = RANDMOD(1000);
			one[i].d_g_rail_press_sb = RANDMOD(1000);
			one[i].d_g_tot_fuel_used = RANDMOD(1000);
			one[i].d_g_tot_fuel_used_l = RANDMOD(1000);
			one[i].d_g_rpu_rpm = RANDMOD(1000);
			one[i].d_g_rpu_ubat_a = RANDMOD(1000);
			one[i].d_g_rpu_ubat_b = RANDMOD(1000);
			one[i].d_g_pri_battery = RANDMOD(1000);
			one[i].d_g_sec_battery = RANDMOD(1000);
			one[i].d_g_cool_temp_ps = RANDMOD(1000);
			one[i].d_g_intake_p_ps = RANDMOD(1000);
			one[i].d_g_intake_t_ps = RANDMOD(1000);
			one[i].d_g_ecu_battvol_s = RANDMOD(1000);
			one[i].d_g_ecu_temp_s = RANDMOD(1000);
			one[i].d_g_rail_press_ps = RANDMOD(1000);
			one[i].d_g_engine_hours = RANDMOD(1000);
			one[i].d_g_run_hours = RANDMOD(1000);
			one[i].d_g_service_time = RANDMOD(1000);
			one[i].d_g_engine_state = RANDMOD(16);
			one[i].d_g_mode_id = RANDMOD(1000);
			one[i].d_g_water_in_fuel = RANDMOD2_1;
			one[i].d_g_remoter_start = RANDMOD2_1;
			one[i].d_g_remote_stop = RANDMOD2_1;
			one[i].d_g_priming_pump = RANDMOD2_1;
			one[i].d_g_increase_speed = RANDMOD2_1;
			one[i].d_g_ecu_slave_on = RANDMOD2_1;
			one[i].d_g_lo_lvl_raw_wat = RANDMOD2_1;
			one[i].d_g_mode2 = RANDMOD2_1;
			one[i].d_g_decrease_speed = RANDMOD2_1;
			one[i].d_g_mode1 = RANDMOD2_1;
			one[i].d_g_fuel_leak_sb = RANDMOD2_1;
			one[i].d_g_ecu_master_on = RANDMOD2_1;
			one[i].d_g_fuel_leak_ps = RANDMOD2_1;
			one[i].d_g_lo_lvl_coolant = RANDMOD2_1;
			one[i].d_g_very_lo_oil_p = RANDMOD2_1;
			one[i].d_g_very_hi_cool_t = RANDMOD2_1;
			one[i].d_g_rpu_sd3 = RANDMOD2_1;
			one[i].d_g_rpu_sd4 = RANDMOD2_1;
			one[i].d_g_rpu_sd5 = RANDMOD2_1;
			one[i].d_g_emstop_rpu = RANDMOD2_1;
			one[i].d_g_turning_gear = RANDMOD2_1;
			one[i].d_g_light_test = RANDMOD2_1;
			one[i].d_g_start_air_p_lo = RANDMOD2_1;
			one[i].d_g_diff_eng_oil_p = RANDMOD2_1;
			one[i].d_g_pre_heat_cde = RANDMOD2_1;
			one[i].d_g_override = RANDMOD2_1;
			one[i].d_g_ecucomm = RANDMOD2_1;
			one[i].d_g_ecu_warning = RANDMOD2_1;
			one[i].d_g_ecu_bin3 = RANDMOD2_1;
			one[i].d_g_ecu_bin4 = RANDMOD2_1;
			one[i].d_g_ecu_bin5 = RANDMOD2_1;
			one[i].d_g_ecu_bin6 = RANDMOD2_1;
			one[i].d_g_glow_lamp = RANDMOD2_1;
			one[i].d_g_ecu_bin8 = RANDMOD2_1;
			one[i].d_g_ain11 = RANDMOD2_1;
			one[i].d_g_ain13 = RANDMOD2_1;
			one[i].d_g_shutoffreqm = RANDMOD2_1;
			one[i].d_g_shutoffreqs = RANDMOD2_1;
			one[i].d_g_ra153_bout = RANDMOD2_1;
			one[i].d_g_detcon = RANDMOD2_1;
			one[i].d_g_rpu_fail = RANDMOD2_1;
			one[i].d_g_bw_rpu_fuelsol = RANDMOD2_1;
			one[i].d_g_bw_rpu_stopsol = RANDMOD2_1;
			one[i].d_g_ecu_alarmlist = RANDMOD2_1;
			one[i].d_g_ecucompconsist = RANDMOD2_1;
			one[i].d_g_actcall_fail = RANDMOD2_1;
			one[i].d_g_derate_alarm = RANDMOD2_1;
			one[i].d_g_ecudiagblocked = RANDMOD2_1;
			one[i].d_g_pwr_limited = RANDMOD2_1;
			one[i].d_g_scm_fail = RANDMOD2_1;
			one[i].d_g_rtcbatteryflat = RANDMOD2_1;
			one[i].d_g_emergency_stop = RANDMOD2_1;
			one[i].d_g_overload = RANDMOD2_1;
			one[i].d_g_othrcommer = RANDMOD2_1;
			one[i].d_g_chrgalternfail = RANDMOD2_1;
			one[i].d_g_stop_fail = RANDMOD2_1;
			one[i].d_g_overspeed = RANDMOD2_1;
			one[i].d_g_underspeed = RANDMOD2_1;
			one[i].d_g_pickup_fail = RANDMOD2_1;
			one[i].d_g_battery_flat = RANDMOD2_1;
			one[i].d_g_start_fail = RANDMOD2_1;
			one[i].d_g_op_rpmdep = RANDMOD2_1;
			one[i].d_g_cp_rpmdep = RANDMOD2_1;
			one[i].d_g_wrn_rpm_diff = RANDMOD2_1;
			one[i].d_g_switchtobatt_b = RANDMOD2_1;
			one[i].d_g_start_blocking = RANDMOD2_1;
			one[i].d_g_load_blocking = RANDMOD2_1;
			one[i].d_g_primbatactive = RANDMOD2_1;
			one[i].d_g_secbatactive = RANDMOD2_1;
			one[i].d_g_emssupply10a = RANDMOD2_1;
			one[i].d_g_extrasupply7a = RANDMOD2_1;
			one[i].d_g30supply7a = RANDMOD2_1;
			one[i].d_g_ecu_yellowlamp = RANDMOD2_1;
			one[i].d_g_ecu_red_lamp = RANDMOD2_1;
			one[i].d_g_ecu_waittostrt = RANDMOD2_1;
			one[i].device_offline = 0;
		}

		bq_baudouin_main_generator[i].put(one[i]);
	}
}


void mainService::gen_data_main_propulsion_test()
{
	static int loop[2] = {0};
	static main_propulsion one[2];
	for(int i = 0; i < 2; ++i)
	{
		one[i].no = i;
		one[i].main_propulsion_control_position = RANDMOD(5);
		one[i].main_propulsion_rudder_indication = -180 + RANDMOD(360);
		one[i].main_propulsion_vfd_rpm_indication = RANDMOD(1000);
		one[i].main_propulsion_vfd_load_indication = RANDMOD(1000);
		one[i].main_propulsion_motor_u_winding_temperature = RANDMOD(200);
		one[i].main_propulsion_motor_v_winding_temperature = RANDMOD(200);
		one[i].main_propulsion_motor_w_winding_temperature = RANDMOD(200);
		one[i].main_propulsion_motor_de_bearing_temperature = RANDMOD(100);
		one[i].main_propulsion_motor_nde_bearing_temperature = RANDMOD(100);
		one[i].main_propulsion_motor_air_inlet_temperature = RANDMOD(100);
		one[i].main_propulsion_motor_air_outlet_temperature = RANDMOD(100);

		one[i].main_propulsion_vfd_actual_speed = RANDMOD(2200);
		one[i].main_propulsion_vfd_actual_torque = RANDMOD(2200);
		one[i].main_propulsion_motor_current = RANDMOD(1000);
		one[i].main_propulsion_vfd_dc_voltage = RANDMOD(1000);
		one[i].main_propulsion_motor_power = RANDMOD(2200);
		

		if(loop[i]++ % alarm_gap == 0)
		{
			one[i].main_propulsion_rudder_feedback_signal_missing = RANDMOD2_1;
			one[i].main_propulsion_control_system_main_power_failure = RANDMOD2_1;
			one[i].main_propulsion_control_system_backup_power_failure = RANDMOD2_1;
			one[i].main_propulsion_steering_command_missing_joystick = RANDMOD2_1;
			one[i].main_propulsion_speed_command_missing_joystick = RANDMOD2_1;
			one[i].main_propulsion_fu_steering_control_failure = RANDMOD2_1;
			one[i].main_propulsion_fu_speed_control_failure = RANDMOD2_1;
			one[i].main_propulsion_vfd_speed_feedback_signal_missing = RANDMOD2_1;
			one[i].main_propulsion_vfd_load_feedback_signal_missing = RANDMOD2_1;
			one[i].main_propulsion_steering_command_missing_fwd_console = RANDMOD2_1;
			one[i].main_propulsion_speed_command_missing_fwd_console = RANDMOD2_1;
			one[i].main_propulsion_rudder_command_missing_aft_console = RANDMOD2_1;
			one[i].main_propulsion_speed_command_missing_aft_console = RANDMOD2_1;
			one[i].azimuth_thruster_common_alarm = RANDMOD2_1;
			one[i].azimuth_thruster_fault = RANDMOD2_1;
			one[i].main_propulsion_remote_control_system_communication_failure = RANDMOD2_1;
			one[i].main_propulsion_vfd_fault = RANDMOD2_1;
			one[i].main_propulsion_vfd_common_alarm = RANDMOD2_1;
			one[i].main_propulsion_vfd_emergency_stop = RANDMOD2_1;
			one[i].main_propulsion_vfd_slowdown_pre_alarm = RANDMOD2_1;
			one[i].main_propulsion_vfd_power_limit = RANDMOD2_1;
			one[i].azimuth_thruster_steering_vfd_fault = RANDMOD2_1;
			one[i].main_propulsion_motor_water_leakage = RANDMOD2_1;
			one[i].main_propulsion_motor_u_winding_temrpeature_high = RANDMOD2_1;
			one[i].main_propulsion_motor_v_winding_temrpeature_high = RANDMOD2_1;
			one[i].main_propulsion_motor_w_winding_temrpeature_high = RANDMOD2_1;
			one[i].main_propulsion_motor_high_temrpeature_slowdown = RANDMOD2_1;
			one[i].main_propulsion_motor_high_temrpeature_shutdown = RANDMOD2_1;
			one[i].main_propulsion_motor_de_bearing_high_temperature_alarm = RANDMOD2_1;
			one[i].main_propulsion_motor_de_bearing_high_temperature_slowdown = RANDMOD2_1;
			one[i].main_propulsion_motor_de_bearing_high_temperature_shutdown = RANDMOD2_1;
			one[i].main_propulsion_motor_nde_bearing_high_temperature_alarm = RANDMOD2_1;
			one[i].main_propulsion_motor_nde_bearing_high_temperature_slowdown = RANDMOD2_1;
			one[i].main_propulsion_motor_nde_bearing_high_temperature_shutdown = RANDMOD2_1;
			one[i].main_propulsion_motor_air_inlet_high_temperature_alarm = RANDMOD2_1;
			one[i].main_propulsion_motor_air_outlet_high_temperature_alarm = RANDMOD2_1;
			one[i].main_propulsion_motor_high_temrpeature_slowdown_pre_alarm = RANDMOD2_1;
			one[i].main_propulsion_motor_high_temrpeature_shutdown_pre_alarm = RANDMOD2_1;
			one[i].main_propulsion_motor_de_bearing_high_temp_slowdown_pre_alarm = RANDMOD2_1;
			one[i].main_propulsion_motor_de_bearing_high_tem_shutdown_pre_alarm = RANDMOD2_1;
			one[i].main_propulsion_motor_nde_bearing_high_temp_slowdown_pre_alarm = RANDMOD2_1;
			one[i].main_propulsion_motor_nde_bearing_high_tem_shutdown_pre_alarm = RANDMOD2_1;
			one[i].main_propulsion_emergency_control_mode = RANDMOD2_1;
			one[i].main_propulsion_shaft_lock_indication = RANDMOD2_1;
			one[i].main_propulsion_vfd_ready = RANDMOD2_1;
			one[i].main_propulsion_vfd_remote_control_position = RANDMOD2_1;
			one[i].main_propulsion_vfd_running = RANDMOD2_1;
			one[i].main_propulsion_vfd_override = RANDMOD2_1;
			one[i].main_propulsion_vfd_torque_limitation = RANDMOD2_1;
			one[i].main_propulsion_speed_control_nfu_mode = RANDMOD2_1;
			one[i].main_propulsion_vfd_start_interlock = RANDMOD2_1;
			one[i].main_propulsion_vfd_control_mode = RANDMOD2_1;
			one[i].main_propulsion_steering_control_nfu_mode = RANDMOD2_1;
			one[i].azimuth_thruster_vfd1_running = RANDMOD2_1;
			one[i].azimuth_thruster_vfd2_running = RANDMOD2_1;
			one[i].azimuth_thruster_ready = RANDMOD2_1;
			one[i].main_propulsion_joystick_control_permission = RANDMOD2_1;
			one[i].main_propulsion_ap_control_permission = RANDMOD2_1;
			one[i].azimuth_thruster_main_power_failure = RANDMOD2_1;
			one[i].azimuth_thruster_backup_power_failure = RANDMOD2_1;
			one[i].azimuth_thruster_shaft_lock_inlet_air_pressure_low_alarm = RANDMOD2_1;
			one[i].azimuth_thruster_lub_oil_level_low_alarm = RANDMOD2_1;
			one[i].azimuth_thruster_lub_oil_temp_high_alarm = RANDMOD2_1;
			one[i].azimuth_thruster_lub_oil_filter_blocked_alarm = RANDMOD2_1;
			one[i].azimuth_thruster_lub_oil_press_low_alarm = RANDMOD2_1;
			one[i].azimuth_thruster_sealing_oil_tank_level_low_alarm = RANDMOD2_1;
			one[i].azimuth_thruster_vfd1_fault = RANDMOD2_1;
			one[i].azimuth_thruster_vfd2_fault = RANDMOD2_1;
			one[i].azimuth_thruster_fu_steering_fault = RANDMOD2_1;
			one[i].azimuth_thruster_lub_motor1_overload_alarm = RANDMOD2_1;
			one[i].azimuth_thruster_lub_motor2_overload_alarm = RANDMOD2_1;
			one[i].azimuth_thruster_angle_order_lost_alarm = RANDMOD2_1;
			one[i].azimuth_thruster_angle_feedback_lost_alarm = RANDMOD2_1;
			one[i].azimuth_thruster_steering_power1_supply_fault = RANDMOD2_1;
			one[i].azimuth_thruster_steering_power2_supply_fault = RANDMOD2_1;
			one[i].azimuth_thruster_sealing_oil_inlet_air_press_low_alarm = RANDMOD2_1;
			one[i].azimuth_thruster_sealing_oil_inlet_air_press_high_alarm = RANDMOD2_1;
			one[i].azimuth_thruster_steering_motor1_temp_high_alarm = RANDMOD2_1;
			one[i].azimuth_thruster_steering_motor2_temp_high_alarm = RANDMOD2_1;
		}
		
		bq_main_propulsion[i].put(one[i]);
	}

}

void mainService::gen_data_power_system_test()
{
	static int loop = 0;
	static power_system one;
	if(loop++ % alarm_gap == 0)
	{

		one.eg_running = RANDMOD2_1;
		one.eg_cb_close = RANDMOD2_1;
		one.esb_dc24v_power_failure = RANDMOD2_1;
		one.eg_breaker_trip = RANDMOD2_1;
		one.eg_over_current = RANDMOD2_1;
		one.esb_common_alarm = RANDMOD2_1;
		one.esb_ac440v_bus_low_insulation = RANDMOD2_1;
		one.esb_ac230v_bus_low_insulation = RANDMOD2_1;
		one.esb_auto_mode = RANDMOD2_1;
		one.no1_main_transformer_breaker_trip = RANDMOD2_1;
		one.no2_main_transformer_breaker_trip = RANDMOD2_1;
		one.no1_main_crane_cb_close = RANDMOD2_1;
		one.no2_main_crane_cb_close = RANDMOD2_1;
		one.no1_jacking_system_phase_transformer_cb_close = RANDMOD2_1;
		one.no2_jacking_system_phase_transformer_cb_close = RANDMOD2_1;
		one.no3_jacking_system_phase_transformer_cb_close = RANDMOD2_1;
		one.no4_jacking_system_phase_transformer_cb_close = RANDMOD2_1;
		one.no1_bow_thruster_cb_close = RANDMOD2_1;
		one.no2_bow_thruster_cb_close = RANDMOD2_1;
		one.no1_main_propulsion_vfd_cb_close = RANDMOD2_1;
		one.no2_main_propulsion_vfd_cb_close = RANDMOD2_1;
		one.dg1_ppm_fault = RANDMOD2_1;
		one.dg2_ppm_fault = RANDMOD2_1;
		one.dg3_ppm_fault = RANDMOD2_1;
		one.dg4_ppm_fault = RANDMOD2_1;
		one.dg5_ppm_fault = RANDMOD2_1;
		one.dg6_ppm_fault = RANDMOD2_1;
		one.msb_ac690v_bus_a_low_insulation = RANDMOD2_1;
		one.msb_ac690v_bus_b_low_insulation = RANDMOD2_1;
		one.no1_main_transformer_cb_close = RANDMOD2_1;
		one.no2_main_transformer_cb_close = RANDMOD2_1;
		one.ac690v_msb_dc24v_power_failure = RANDMOD2_1;
		one.msb_ac690v_bus_b_harmonic_over_limit = RANDMOD2_1;
		one.msb_ac690v_bus_a_harmonic_over_limit = RANDMOD2_1;
		one.msb_ac690v_shore_power_cb_closed = RANDMOD2_1;
		one.no1_main_transformer_secondary_breaker_input_power_on = RANDMOD2_1;
		one.no1_main_transformer_breaker_auto_mode = RANDMOD2_1;
		one.no1_main_transformer_secondary_breaker_close = RANDMOD2_1;
		one.no1_main_transformer_secondary_breaker_trip = RANDMOD2_1;
		one.msb_ac440v_bus_b_blackout = RANDMOD2_1;
		one.no2_main_transformer_secondary_breaker_input_power_on = RANDMOD2_1;
		one.no2_main_transformer_breaker_auto_mode = RANDMOD2_1;
		one.no2_main_transformer_secondary_breaker_close = RANDMOD2_1;
		one.no2_main_transformer_secondary_breaker_trip = RANDMOD2_1;
		one.ac440v_msbbustie_acb_close = RANDMOD2_1;
		one.msb_ac440v_bus_a_blackout = RANDMOD2_1;
		one.ac440v_msb_dc24v_power_failure = RANDMOD2_1;
		one.pt_power_failure = RANDMOD2_1;
		one.ac440v_msb_bus_c_low_insulation = RANDMOD2_1;
		one.ac440v_msb_bus_d_low_insulation = RANDMOD2_1;
		one.ac230v_msb_bus_low_insulation = RANDMOD2_1;
		one.preferential_trip_alarm = RANDMOD2_1;
		one.e_stop_shutdown_power_failure = RANDMOD2_1;
		one.ac690v_msb_bus_tie_cb_closed = RANDMOD2_1;
		one.ac440v_msb_to_esb_cb_colse_statues = RANDMOD2_1;
		one.ac440v_shore_power_cb_closed = RANDMOD2_1;
		one.emergency_shutdown_wire_break_es1 = RANDMOD2_1;
		one.emergency_shutdown_wire_break_es21 = RANDMOD2_1;
		one.emergency_shutdown_wire_break_es22 = RANDMOD2_1;
		one.emergency_shutdown_wire_break_es23 = RANDMOD2_1;
		one.emergency_shutdown_wire_break_es24 = RANDMOD2_1;
		one.emergency_shutdown_wire_break_es25 = RANDMOD2_1;
		one.emergency_shutdown_wire_break_es26 = RANDMOD2_1;
		one.emergency_shutdown_es1_actived = RANDMOD2_1;
		one.emergency_shutdown_es21_actived = RANDMOD2_1;
		one.emergency_shutdown_es22_actived = RANDMOD2_1;
		one.emergency_shutdown_es23_actived = RANDMOD2_1;
		one.emergency_shutdown_es24_actived = RANDMOD2_1;
		one.emergency_shutdown_es25_actived = RANDMOD2_1;
		one.emergency_shutdown_es26_actived = RANDMOD2_1;
		one.dg1_acb_close_fail = RANDMOD2_1;
		one.dg1_acb_open_fail = RANDMOD2_1;
		one.dg1_ready_to_start = RANDMOD2_1;
		one.dg1_cb_open = RANDMOD2_1;
		one.dg1_cb_close = RANDMOD2_1;
		one.dg1_auto_mode = RANDMOD2_1;
		one.dg1_semi_auto_mode = RANDMOD2_1;
		one.dg1_on_first_priority = RANDMOD2_1;
		one.dg2_acb_close_fail = RANDMOD2_1;
		one.dg2_acb_open_fail = RANDMOD2_1;
		one.dg2_ready_to_start = RANDMOD2_1;
		one.dg2_cb_open = RANDMOD2_1;
		one.dg2_cb_close = RANDMOD2_1;
		one.dg2_auto_mode = RANDMOD2_1;
		one.dg2_semi_auto_mode = RANDMOD2_1;
		one.dg2_on_first_priority = RANDMOD2_1;
		one.dg3_acb_close_fail = RANDMOD2_1;
		one.dg3_acb_open_fail = RANDMOD2_1;
		one.dg3_ready_to_start = RANDMOD2_1;
		one.dg3_cb_open = RANDMOD2_1;
		one.dg3_cb_close = RANDMOD2_1;
		one.dg3_auto_mode = RANDMOD2_1;
		one.dg3_semi_auto_mode = RANDMOD2_1;
		one.dg3_on_first_priority = RANDMOD2_1;
		one.dg4_acb_close_fail = RANDMOD2_1;
		one.dg4_acb_open_fail = RANDMOD2_1;
		one.dg4_ready_to_start = RANDMOD2_1;
		one.dg4_cb_open = RANDMOD2_1;
		one.dg4_cb_close = RANDMOD2_1;
		one.dg4_auto_mode = RANDMOD2_1;
		one.dg4_semi_auto_mode = RANDMOD2_1;
		one.dg4_on_first_priority = RANDMOD2_1;
		one.dg5_acb_close_fail = RANDMOD2_1;
		one.dg5_acb_open_fail = RANDMOD2_1;
		one.dg5_ready_to_start = RANDMOD2_1;
		one.dg5_cb_open = RANDMOD2_1;
		one.dg5_cb_close = RANDMOD2_1;
		one.dg5_auto_mode = RANDMOD2_1;
		one.dg5_semi_auto_mode = RANDMOD2_1;
		one.dg5_on_first_priority = RANDMOD2_1;
		one.dg6_acb_close_fail = RANDMOD2_1;
		one.dg6_acb_open_fail = RANDMOD2_1;
		one.dg6_ready_to_start = RANDMOD2_1;
		one.dg6_cb_open = RANDMOD2_1;
		one.dg6_cb_close = RANDMOD2_1;
		one.dg6_auto_mode = RANDMOD2_1;
		one.dg6_semi_auto_mode = RANDMOD2_1;
		one.dg6_on_first_priority = RANDMOD2_1;
#ifdef JANA508509	
		one.dg7_acb_close_fail = RANDMOD2_1;
		one.dg7_acb_open_fail = RANDMOD2_1;
		one.dg7_ready_to_start = RANDMOD2_1;
		one.dg7_cb_open = RANDMOD2_1;
		one.dg7_cb_close = RANDMOD2_1;
		one.dg7_auto_mode = RANDMOD2_1;
		one.dg7_semi_auto_mode = RANDMOD2_1;	
		one.dg7_on_first_priority = RANDMOD2_1;
#endif	
		one.dg1_supply_voltage_high = RANDMOD2_1;
		one.dg1_supply_voltage_low = RANDMOD2_1;
		one.dg1_supply_frequency_high = RANDMOD2_1;
		one.dg1_supply_frequency_low = RANDMOD2_1;
		one.dg1_acb_abnormal_trip = RANDMOD2_1;
		one.dg1_reverse_power = RANDMOD2_1;
		one.dg1_ppm_alarms_on = RANDMOD2_1;
		one.ac690v_msb_bus_bar_voltage_high = RANDMOD2_1;
		one.ac690v_msb_bus_bar_voltage_low = RANDMOD2_1;
		one.ac690v_msb_bus_bar_frequency_high = RANDMOD2_1;
		one.ac690v_msb_bus_bar_frequency_low = RANDMOD2_1;
		one.dg1_auto_synchro_failure = RANDMOD2_1;
		one.dg1_voltage_l1_l2 = RANDMOD(1000);
		one.dg1_voltage_l2_l3 = RANDMOD(1000);
		one.dg1_voltage_l3_l1 = RANDMOD(1000);
		one.dg1_frequency_l1 = RANDMOD(1000);
		one.dg1_frequency_l2 = RANDMOD(1000);
		one.dg1_frequency_l3 = RANDMOD(1000);
		one.dg1_current_l1 = RANDMOD(1000);
		one.dg1_current_l2 = RANDMOD(1000);
		one.dg1_current_l3 = RANDMOD(1000);
		one.dg1_active_power = RANDMOD(1000);
		one.dg1_reactive_power = RANDMOD(1000);
		one.dg1_apparent_power_s = RANDMOD(1000);
		one.dg1_power_factor = RANDMOD(1000);
		one.dg1_available_power = RANDMOD(1000);
		one.ac690v_msb_busbar_voltage_l1_l2 = RANDMOD(1000);
		one.ac690v_msb_busbar_voltage_l2_l3 = RANDMOD(1000);
		one.ac690v_msb_busbar_voltage_l3_l1 = RANDMOD(1000);
		one.ac690v_msb_busbar_frequency_l1 = RANDMOD(1000);
		one.ac690v_msb_busbar_frequency_l2 = RANDMOD(1000);
		one.ac690v_msb_busbar_frequency_l3 = RANDMOD(1000);
		one.dg2_supply_voltage_high = RANDMOD2_1;
		one.dg2_supply_voltage_low = RANDMOD2_1;
		one.dg2_supply_frequency_high = RANDMOD2_1;
		one.dg2_supply_frequency_low = RANDMOD2_1;
		one.dg2_acb_abnormal_trip = RANDMOD2_1;
		one.dg2_reverse_power = RANDMOD2_1;
		one.dg2_ppm_alarms_on = RANDMOD2_1;
		one.dg2_auto_synchro_failure = RANDMOD2_1;
		one.dg2_voltage_l1_l2 = RANDMOD(1000);
		one.dg2_voltage_l2_l3 = RANDMOD(1000);
		one.dg2_voltage_l3_l1 = RANDMOD(1000);
		one.dg2_frequency_l1 = RANDMOD(1000);
		one.dg2_frequency_l2 = RANDMOD(1000);
		one.dg2_frequency_l3 = RANDMOD(1000);
		one.dg2_current_l1 = RANDMOD(1000);
		one.dg2_current_l2 = RANDMOD(1000);
		one.dg2_current_l3 = RANDMOD(1000);
		one.dg2_active_power = RANDMOD(1000);
		one.dg2_reactive_power = RANDMOD(1000);
		one.dg2_apparent_power_s = RANDMOD(1000);
		one.dg2_power_factor = RANDMOD(1000);
		one.dg2_available_power = RANDMOD(1000);
		one.dg3_supply_voltage_high = RANDMOD2_1;
		one.dg3_supply_voltage_low = RANDMOD2_1;
		one.dg3_supply_frequency_high = RANDMOD2_1;
		one.dg3_supply_frequency_low = RANDMOD2_1;
		one.dg3_acb_abnormal_trip = RANDMOD2_1;
		one.dg3_reverse_power = RANDMOD2_1;
		one.dg3_ppm_alarms_on = RANDMOD2_1;
		one.dg3_auto_synchro_failure = RANDMOD2_1;
		one.dg3_voltage_l1_l2 = RANDMOD(1000);
		one.dg3_voltage_l2_l3 = RANDMOD(1000);
		one.dg3_voltage_l3_l1 = RANDMOD(1000);
		one.dg3_frequency_l1 = RANDMOD(1000);
		one.dg3_frequency_l2 = RANDMOD(1000);
		one.dg3_frequency_l3 = RANDMOD(1000);
		one.dg3_current_l1 = RANDMOD(1000);
		one.dg3_current_l2 = RANDMOD(1000);
		one.dg3_current_l3 = RANDMOD(1000);
		one.dg3_active_power = RANDMOD(1000);
		one.dg3_reactive_power = RANDMOD(1000);
		one.dg3_apparent_power_s = RANDMOD(1000);
		one.dg3_power_factor = RANDMOD(1000);
		one.dg3_available_power = RANDMOD(1000);
		one.dg4_supply_voltage_high = RANDMOD2_1;
		one.dg4_supply_voltage_low = RANDMOD2_1;
		one.dg4_supply_frequency_high = RANDMOD2_1;
		one.dg4_supply_frequency_low = RANDMOD2_1;
		one.dg4_acb_abnormal_trip = RANDMOD2_1;
		one.dg4_reverse_power = RANDMOD2_1;
		one.dg4_ppm_alarms_on = RANDMOD2_1;
		one.dg4_auto_synchro_failure = RANDMOD2_1;
		one.dg4_voltage_l1_l2 = RANDMOD(1000);
		one.dg4_voltage_l2_l3 = RANDMOD(1000);
		one.dg4_voltage_l3_l1 = RANDMOD(1000);
		one.dg4_frequency_l1 = RANDMOD(1000);
		one.dg4_frequency_l2 = RANDMOD(1000);
		one.dg4_frequency_l3 = RANDMOD(1000);
		one.dg4_current_l1 = RANDMOD(1000);
		one.dg4_current_l2 = RANDMOD(1000);
		one.dg4_current_l3 = RANDMOD(1000);
		one.dg4_active_power = RANDMOD(1000);
		one.dg4_reactive_power = RANDMOD(1000);
		one.dg4_apparent_power_s = RANDMOD(1000);
		one.dg4_power_factor = RANDMOD(1000);
		one.dg4_available_power = RANDMOD(1000);
		one.dg5_supply_voltage_high = RANDMOD2_1;
		one.dg5_supply_voltage_low = RANDMOD2_1;
		one.dg5_supply_frequency_high = RANDMOD2_1;
		one.dg5_supply_frequency_low = RANDMOD2_1;
		one.dg5_acb_abnormal_trip = RANDMOD2_1;
		one.dg5_reverse_power = RANDMOD2_1;
		one.dg5_ppm_alarms_on = RANDMOD2_1;
		one.dg5_auto_synchro_failure = RANDMOD2_1;
		one.dg5_voltage_l1_l2 = RANDMOD(1000);
		one.dg5_voltage_l2_l3 = RANDMOD(1000);
		one.dg5_voltage_l3_l1 = RANDMOD(1000);
		one.dg5_frequency_l1 = RANDMOD(1000);
		one.dg5_frequency_l2 = RANDMOD(1000);
		one.dg5_frequency_l3 = RANDMOD(1000);
		one.dg5_current_l1 = RANDMOD(1000);
		one.dg5_current_l2 = RANDMOD(1000);
		one.dg5_current_l3 = RANDMOD(1000);
		one.dg5_active_power = RANDMOD(1000);
		one.dg5_reactive_power = RANDMOD(1000);
		one.dg5_apparent_power_s = RANDMOD(1000);
		one.dg5_power_factor = RANDMOD(1000);
		one.dg5_available_power = RANDMOD(1000);
		one.dg6_supply_voltage_high = RANDMOD(1000);
		one.dg6_supply_voltage_low = RANDMOD(1000);
		one.dg6_supply_frequency_high = RANDMOD2_1;
		one.dg6_supply_frequency_low = RANDMOD2_1;
		one.dg6_acb_abnormal_trip = RANDMOD2_1;
		one.dg6_reverse_power = RANDMOD2_1;
		one.dg6_ppm_alarms_on = RANDMOD2_1;
		one.dg6_auto_synchro_failure = RANDMOD2_1;
		one.dg6_voltage_l1_l2 = RANDMOD(1000);
		one.dg6_voltage_l2_l3 = RANDMOD(1000);
		one.dg6_voltage_l3_l1 = RANDMOD(1000);
		one.dg6_frequency_l1 = RANDMOD(1000);
		one.dg6_frequency_l2 = RANDMOD(1000);
		one.dg6_frequency_l3 = RANDMOD(1000);
		one.dg6_current_l1 = RANDMOD(1000);
		one.dg6_current_l2 = RANDMOD(1000);
		one.dg6_current_l3 = RANDMOD(1000);
		one.dg6_active_power = RANDMOD(1000);
		one.dg6_reactive_power = RANDMOD(1000);
		one.dg6_apparent_power_s = RANDMOD(1000);
		one.dg6_power_factor = RANDMOD(1000);
		one.dg6_available_power = RANDMOD(1000);
#ifdef JANA508509
		one.dg7_supply_voltage_high = RANDMOD(1000);
		one.dg7_supply_voltage_low = RANDMOD(1000);
		one.dg7_supply_frequency_high = RANDMOD2_1;
		one.dg7_supply_frequency_low = RANDMOD2_1;
		one.dg7_acb_abnormal_trip = RANDMOD2_1;
		one.dg7_reverse_power = RANDMOD2_1;
		one.dg7_ppm_alarms_on = RANDMOD2_1;
		one.dg7_auto_synchro_failure = RANDMOD2_1;
		one.dg7_voltage_l1_l2 = RANDMOD(1000);
		one.dg7_voltage_l2_l3 = RANDMOD(1000);
		one.dg7_voltage_l3_l1 = RANDMOD(1000);
		one.dg7_frequency_l1 = RANDMOD(1000);
		one.dg7_frequency_l2 = RANDMOD(1000);
		one.dg7_frequency_l3 = RANDMOD(1000);
		one.dg7_current_l1 = RANDMOD(1000);
		one.dg7_current_l2 = RANDMOD(1000);
		one.dg7_current_l3 = RANDMOD(1000);
		one.dg7_active_power = RANDMOD(1000);
		one.dg7_reactive_power = RANDMOD(1000);
		one.dg7_apparent_power_s = RANDMOD(1000);
		one.dg7_power_factor = RANDMOD(1000);
		one.dg7_available_power = RANDMOD(1000);
#endif
		one.no1_jetting_pump_request_failure = RANDMOD2_1;
		one.no2_jetting_pump_request_failure = RANDMOD2_1;
		one.no1_bow_thruster_request_failure = RANDMOD2_1;
		one.no2_bow_thruster_request_failure = RANDMOD2_1;
		one.no1_main_crane_request_failure = RANDMOD2_1;
		one.no2_main_crane_request_failure = RANDMOD2_1;
		one.no1_leg_control_unit_request_failure = RANDMOD2_1;
		one.no2_leg_control_unit_request_failure = RANDMOD2_1;
		one.no3_leg_control_unit_request_failure = RANDMOD2_1;
		one.no4_leg_control_unit_request_failure = RANDMOD2_1;
		one.no1_submersible_pump_request_failure = RANDMOD2_1;
		one.no2_submersible_pump_request_failure = RANDMOD2_1;	

	}
	bq_power_system.put(one);

}	



void mainService::gen_data_main_diesel_generator_test()
{
	static int loop[main_gens_nums] = {0};
	static main_diesel_generator one[main_gens_nums];

	for(int i = 0; i < main_gens_nums; ++i)
	{
		if(loop[i] % 5 == 0)
		{
			one[i].gen_id = i;
			one[i].d_g_dcu_unit_battery_volt_dcu_control_power = RANDMOD(40);
			//one[i].d_g_lube_oil_pressure = RANDMOD(10);
			//one[i].d_g_coolant_pressure = RANDMOD(6);
			one[i].d_g_start_air_pressure = RANDMOD(16);
			one[i].d_g_fuel_oil_pressure = RANDMOD(16);
			one[i].d_g_coolant_temperature = RANDMOD(200);
			one[i].d_g_lube_oil_temperature = RANDMOD(200);
			one[i].d_g_left_exhaust_temperature = RANDMOD(800);
			one[i].d_g_right_exhaust_temperature = RANDMOD(800);
			one[i].d_g_engine_speed = RANDMOD(2200);
			
			one[i].d_g_rpu_unit_battery_a_volt_rpu_main_control_power = RANDMOD(40);
			one[i].d_g_rpu_unit_battery_b_volt_rpu_backup_control_power = RANDMOD(40);

			one[i].d_g_main_generator_u_winding_temperature = RANDMOD(1000);
			one[i].d_g_main_generator_v_winding_temperature = RANDMOD(1000);
			one[i].d_g_main_generator_w_winding_temperature = RANDMOD(1000);
			one[i].d_g_main_generator_de_bearing_temperature = RANDMOD(1000);
			one[i].d_g_main_generator_nde_bearing_temperature = RANDMOD(1000);
			one[i].d_g_main_generator_excitation_current = RANDMOD(1000);
			one[i].d_g_main_generator_excitation_voltage = RANDMOD(1000);
		}
		if(loop[i] % alarm_gap == 0)
		{
			//one[i].d_g_lube_oil_pressure = 8 + RANDMOD(10);
			one[i].d_g_lube_oil_pressure = one[i].d_g_lube_oil_pressure > 10 ? RANDMOD(10): 10 + RANDMOD(10);
			one[i].d_g_coolant_pressure = one[i].d_g_coolant_pressure>6? RANDMOD(6):5 + RANDMOD(6);

			one[i].d_g_running_hours += 0.05;
			one[i].d_g_coolant_level_low = RANDMOD2_1;
			one[i].d_g_main_power_failure = RANDMOD2_1;
			one[i].d_g_backup_power_failure = RANDMOD2_1;
			one[i].d_g_sea_water_pump_overload_trip = RANDMOD2_1;	
			one[i].d_g_ecr_remote_emergency_stop = RANDMOD2_1;
			one[i].d_g_aft_bridge_remote_emergency_stop = RANDMOD2_1;
			one[i].d_g_fwd_bridge_remote_emergency_stop = RANDMOD2_1;
			one[i].d_g_lube_oil_pressure_too_low_shutdown = RANDMOD2_1;
			one[i].d_g_coolant_temperature_too_high_shutdown = RANDMOD2_1;
			one[i].d_g_local_emergency_stop = RANDMOD2_1;
			one[i].d_g_remote_emergency_stop = RANDMOD2_1;
			one[i].d_g_rpu_emergency_stop = RANDMOD2_1;
			one[i].d_g_lube_oil_pressure_low_alarm = RANDMOD2_1;
			one[i].d_g_coolant_pressure_low_alarm = RANDMOD2_1;
			one[i].d_g_start_air_pressure_low_alarm = RANDMOD2_1;
			one[i].d_g_fuel_oil_pressure_low_alarm = RANDMOD2_1;
			one[i].d_g_coolant_temperature_high_alarm = RANDMOD2_1;
			one[i].d_g_lube_oil_temperature_high_alarm= RANDMOD2_1;
			one[i].d_g_left_exhaust_temperature_high_alarm = RANDMOD2_1;
			one[i].d_g_right_exhaust_temperature_high_alarm = RANDMOD2_1;		
			one[i].d_g_overspeed_alarm = RANDMOD2_1;		
			one[i].d_g_overspeed_shutdown = RANDMOD2_1;
			one[i].d_g_speed_sensor_fail = RANDMOD2_1;
			one[i].d_g_dcu_unit_battery_volt_dct_control_power_low = RANDMOD2_1;
			one[i].d_g_start_fail = RANDMOD2_1;
			one[i].d_g_rpu_unit_battery_a_volt_rpu_main_control_power_low = RANDMOD2_1;
			one[i].d_g_rpu_unit_battery_b_volt_rpu_main_control_power_low = RANDMOD2_1;
			one[i].d_g_common_alarm = RANDMOD2_1;
			one[i].d_g_common_warning = RANDMOD2_1;
			one[i].d_g_common_shutdown = RANDMOD2_1;
			one[i].d_g_common_cooldown = RANDMOD2_1;
			one[i].d_g_common_faultsensor_fail = RANDMOD2_1;
			one[i].d_g_remote_control_status_auxaccess_lock = RANDMOD2_1;
			one[i].d_g_ready_to_start = RANDMOD2_1;
			one[i].d_g_ready_to_load = RANDMOD2_1;
			one[i].d_g_main_generator_running = RANDMOD2_1;
		}


		one[i].device_offline = 0;

		loop[i]++;
		
		bq_main_diesel_generator[i].put(one[i]);
	}

}


void mainService::gen_data_emergency_generator_test()
{
	static int loop = 0;
	static emergency_generator one;
	one.eg_running_hours = 0;

	one.eg_dcu_unit_battery_volt_dcu_control_power = RANDMOD(40);
	one.eg_lube_oil_pressure = RANDMOD(10);
	one.eg_coolant_pressure = RANDMOD(6);
	one.eg_coolant_temperature = RANDMOD(200);
	one.eg_lube_oil_temperature = RANDMOD(200);
	one.eg_engine_speed = RANDMOD(2200);
	
	one.eg_rpu_unit_battery_a_volt_rpu_main_control_power = RANDMOD(40);
	one.eg_rpu_unit_battery_b_volt_rpu_backup_control_power = RANDMOD(40);

	if(loop++ % alarm_gap == 0)
	{
		one.eg_running_hours += 0.05;
		one.eg_coolant_level_low = RANDMOD2_1;
		one.eg_main_power_failure = RANDMOD2_1;
		one.eg_backup_power_failure = RANDMOD2_1;
		one.eg_no1_charger_failure = RANDMOD2_1;
		one.eg_no2_charger_failure = RANDMOD2_1;
		one.eg_local_emergency_stop = RANDMOD2_1;
		one.eg_remote_emergency_stop = RANDMOD2_1;
		one.eg_rpu_emergency_stop = RANDMOD2_1;
		one.eg_lube_oil_pressure_low_alarm = RANDMOD2_1;
		one.eg_coolant_pressure_low_alarm = RANDMOD2_1;
		one.eg_coolant_temperature_high_alarm = RANDMOD2_1;
		one.eg_lube_oil_temperature_high_alarm = RANDMOD2_1;
		one.eg_overspeed_alarm = RANDMOD2_1;
		one.eg_overspeed_shutdown = RANDMOD2_1;
		one.eg_speed_sensor_fail = RANDMOD2_1;
		one.eg_dcu_unit_battery_volt_dct_control_power_low = RANDMOD2_1;
		one.eg_start_fail = RANDMOD2_1;
		one.eg_rpu_unit_battery_a_volt_rpu_main_control_power_low = RANDMOD2_1;
		one.eg_rpu_unit_battery_b_volt_rpu_main_control_power_low = RANDMOD2_1;
		one.eg_common_alarm = RANDMOD2_1;
		one.eg_common_warning = RANDMOD2_1;
		one.eg_common_shutdown = RANDMOD2_1;
		one.eg_common_cooldown = RANDMOD2_1;
		one.eg_common_faultsensor_fail = RANDMOD2_1;
		one.eg_remote_control_status_auxaccess_lock = RANDMOD2_1;
		one.eg_ready_to_start = RANDMOD2_1;
		one.eg_ready_to_load = RANDMOD2_1;
		one.eg_generator_running = RANDMOD2_1;
	}

	one.eg_generator_u_winding_temperature = RANDMOD(1000);
	one.eg_generator_v_winding_temperature = RANDMOD(1000);
	one.eg_generator_w_winding_temperature = RANDMOD(1000);
	one.device_offline = 0;

	bq_emergency_generator.put(one);

}


void mainService::gen_data_general_charger_and_discharger_panel_test()
{
	static int loop = 0;
	static general_charger_and_discharger_panel one;
	if(loop++ % alarm_gap == 0)
	{
		one.low_insulation = RANDMOD2_1;
		one.rectifier_fault = RANDMOD2_1;
		one.charger_fault = RANDMOD2_1;
		one.inverter_failure = RANDMOD2_1;
		one.battery_protector_active = RANDMOD2_1;
		one.battery_discharge = RANDMOD2_1;
		one.battery_low_voltage = RANDMOD2_1;
		one.inverter_low_voltage = RANDMOD2_1;
		one.device_offline = 0;
	}

	bq_general_charger_and_discharger_panel.put(one);
}


void mainService::gen_data_main_transformer_test(int i)
{
	main_transformer one;
	one.no = i;
	one.main_transformer_u_winding_temp = RANDMOD(1000);
	one.main_transformer_v_winding_temp = RANDMOD(1000);
	one.main_transformer_w_winding_temp = RANDMOD(1000);
	one.device_offline = 0;
	bq_main_transformer[i].put(one);
}


void mainService::gen_data_bow_thruster_test()
{
	static int loop[2] = {0};
	static bow_thruster one[2];
	for(int i = 0; i < 2; ++i)
	{
		one[i].no = i;
		if(loop[i]++ % alarm_gap == 0)
		{
			one[i].b_t_remote_control_system_ac_power_failure = RANDMOD2_1;
			one[i].b_t_remote_control_system_dc_power_failure = RANDMOD2_1;
			one[i].b_t_remote_control_system_failure = RANDMOD2_1;
			one[i].b_t_main_motor_overload = RANDMOD2_1;
			one[i].b_t_main_motor_high_temperature = RANDMOD2_1;
			one[i].b_t_oil_low_level = RANDMOD2_1;
			one[i].b_t_oil_low_pressure = RANDMOD2_1;
			one[i].b_t_oil_high_pressure = RANDMOD2_1;
			one[i].b_t_main_motor_short_circuit = RANDMOD2_1;
			one[i].b_t_emergency_stop_circuit_failure = RANDMOD2_1;
			one[i].b_t_too_many_times_start = RANDMOD2_1;
			one[i].b_t_integrated_alarm = RANDMOD2_1;
			one[i].device_offline = 0;
		}

		bq_bow_thruster[i].put(one[i]);
	}

}


void mainService::gen_data_e_r_charger_and_discharger_panel_test()
{
	static int loop = 0;
	static e_r_charger_and_discharger_panel one;
	if(loop++ % alarm_gap == 0)
	{
		one.ups_low_insulation = RANDMOD2_1;
		one.ups_battery_low_voltage = RANDMOD2_1;
		one.ups_input_fault = RANDMOD2_1;
		one.ups_battery_bypass_activation = RANDMOD2_1;
		one.ups_common_alarm = RANDMOD2_1;
		one.ups_alarm = RANDMOD2_1;
		one.ups_running = RANDMOD2_1;
		one.ups_battery_mode = RANDMOD2_1;
		one.ups_battery_protection_trip = 0;
		one.ups_output_alarm = 0;
		one.device_offline = 0;
	}

	bq_e_r_charger_and_discharger_panel.put(one);

}


void mainService::gen_data_level_gauging_test()
{
	level_gauging one;
	one.forep_draft = 1;// RANDMOD(100);
	one.fores_draft = 12;// RANDMOD(100);
	one.aftp_draft = 13;// RANDMOD(100);
	one.afts_draft = 14;// RANDMOD(100);
	one.sw_ballast_tank1p_level = 15;// RANDMOD(100);
	one.sw_ballast_tank1s_level = 16;// RANDMOD(100);
	one.sw_ballast_tank2p_level = 17;// RANDMOD(100);
	one.sw_ballast_tank2s_level = 18;// RANDMOD(100);
	one.sw_ballast_tank3p_level = 21;// RANDMOD(100);
	one.sw_ballast_tank3s_level = 222;// RANDMOD(100);
	one.sw_ballast_tank3m_level = 23;// RANDMOD(100);
	one.fresh_water_tank1p_level = 24;// RANDMOD(100);
	one.fresh_water_tank1s_level = 25;// RANDMOD(100);
	one.fresh_water_tank2p_level = 26;// RANDMOD(100);
	one.fresh_water_tank2s_level = 27;// RANDMOD(100);
	one.fuel_oil_store_tank_p_level = 28;// RANDMOD(100);
	one.fuel_oil_store_tank_s_level = 29;// RANDMOD(100);
	one.fuel_oil_day_tank_p_level = 30;// RANDMOD(100);
	one.fuel_oil_day_tank_s_level = 31;// RANDMOD(100);
	one.sw_ballast_tank1p_capacity = RANDMOD(100);
	one.sw_ballast_tank1s_capacity = RANDMOD(100);
	one.sw_ballast_tank2p_capacity = RANDMOD(100);
	one.sw_ballast_tank2s_capacity = RANDMOD(100);
	one.sw_ballast_tank3p_capacity = RANDMOD(100);
	one.sw_ballast_tank3s_capacity = RANDMOD(100);
	one.sw_ballast_tank3m_capacity = RANDMOD(100);
	one.fresh_water_tank1p_capacity = RANDMOD(100);
	one.fresh_water_tank1s_capacity = RANDMOD(100);
	one.fresh_water_tank2p_capacity = RANDMOD(100);
	one.fresh_water_tank2s_capacity = RANDMOD(100);
	one.fuel_oil_store_tank_p_capacity = RANDMOD(100);
	one.fuel_oil_store_tank_s_capacity = RANDMOD(100);
	one.fuel_oil_day_tank_p_capacity = RANDMOD(100);
	one.fuel_oil_day_tank_s_capacity = RANDMOD(100);
	one.level_mearsurement_system_common_alarm = 0;
	one.device_offline = 0;

//	FILELOG_level_gauging(one);
	bq_level_gauging.put(one);
}

void mainService::gen_data_level_switch_test()
{
	static int loop = 0;
	static level_switch one;
	if(loop++ % alarm_gap == 0)
	{
		one.fuel_oil_store_tank_p_high_level_alarm = RANDMOD2_1;
		one.fuel_oil_store_tank_p_low_level_alarm = RANDMOD2_1;
		one.fuel_oil_day_tank_p_high_level_alarm = RANDMOD2_1;
		one.fuel_oil_day_tank_p_low_level_alarm = RANDMOD2_1;
		one.fuel_oil_store_tank_s_high_level_alarm = RANDMOD2_1;
		one.fuel_oil_store_tank_s_low_level_alarm = RANDMOD2_1;
		one.fuel_oil_day_tank_s_high_level_alarm = RANDMOD2_1;
		one.fuel_oil_day_tank_s_low_level_alarm = RANDMOD2_1;
		one.lube_oil_tank_high_level_alarm = RANDMOD2_1;
		one.dirty_oil_tank_high_level_alarm = RANDMOD2_1;
		one.black_water_tank_high_level_alarm = RANDMOD2_1;
		one.grey_water_tank_high_level_alarm = RANDMOD2_1;
		one.e_g_m_d_o_service_tank_high_level_alarm = RANDMOD2_1;
		one.e_g_m_d_o_service_tk_low_level_alarm = RANDMOD2_1;
		one.spill_tank_high_level_alarm = RANDMOD2_1;
		one.b_t_room_bilge_high_level_alarm = RANDMOD2_1;
		one.em_cy_fire_fighting_pump_room_bilge_high_level_alarm = RANDMOD2_1;
		one.no1_void_p_tank_bilge_high_level_alarm = RANDMOD2_1;
		one.no2_void_p_tank_bilge_high_level_alarm = RANDMOD2_1;
		one.no2_void_s_tank_bilge_high_level_alarm = RANDMOD2_1;
		one.no3_void_p_tank_bilge_high_level_alarm = RANDMOD2_1;
		one.no3_void_s_tank_bilge_high_level_alarm = RANDMOD2_1;
		one.no4_void_p_tank_bilge_high_level_alarm = RANDMOD2_1;
		one.no4_void_s_tank_bilge_high_level_alarm = RANDMOD2_1;
		one.no5_void_p_tank_bilge_high_level_alarm = RANDMOD2_1;
		one.no5_void_s_tank_bilge_high_level_alarm = RANDMOD2_1;
		one.no6_void_p_tank_bilge_high_level_alarm = RANDMOD2_1;
		one.no6_void_s_tank_bilge_high_level_alarm = RANDMOD2_1;
		one.aux_machinery_room1_bilge_high_level_alarm = RANDMOD2_1;
		one.aux_machinery_room2_bilge_high_level_alarm = RANDMOD2_1;
		one.e_r_ps_bilge_high_level_alarm = RANDMOD2_1;
		one.e_r_sb_bilge_high_level_alarm = RANDMOD2_1;
		one.aft_thruster_rm_ps_bilge_high_level_alarm = RANDMOD2_1;
		one.aft_thruster_rm_sb_bilge_high_level_alarm = RANDMOD2_1;
		one.bilge_water_tank_high_level_alarm = RANDMOD2_1;
		one.no1_expansion_tank_low_level_alarm = RANDMOD2_1;
		one.no2_expansion_tank_low_level_alarm = RANDMOD2_1;
		one.no1_jackhouse_bilge_high_level_alarm = RANDMOD2_1;
		one.no2_jackhouse_bilge_high_level_alarm = RANDMOD2_1;
		one.no3_jackhouse_bilge_high_level_alarm = RANDMOD2_1;
		one.no4_jackhouse_bilge_high_level_alarm = RANDMOD2_1;
		one.galley_transformer_room_bilge_low_level_alarm = RANDMOD2_1;
		one.f_o_transfer_pump_room_bilge_low_level_alarm = RANDMOD2_1;
		one.device_offline = 0;
	}
	bq_level_switch.put(one);
}

void mainService::gen_data_gas_detection_system_test()
{
	static gas_detection_system one;
	static int loop = 0;
	if(loop++ % alarm_gap == 0)
	{
		one.gas_detection_alarm_system_failure = RANDMOD2_1;
		one.gas_detector_fault_top_of_wheelhouse = RANDMOD2_1;
		one.gas_pre_alarm_top_of_wheelhouse = RANDMOD2_1;
		one.gas_level1_alarm_top_of_wheelhouse = RANDMOD2_1;
		one.gas_level2_alarm_top_of_wheelhouse = RANDMOD2_1;
		one.gas_detector_disabled_top_of_wheelhouse = RANDMOD2_1;
		one.gas_detector_fault_air_intake_of_b_deck_ahu = RANDMOD2_1;
		one.gas_pre_alarm_air_intake_of_b_deck_ahu = RANDMOD2_1;
		one.gas_level1_alarm_air_intake_of_b_deck_ahu = RANDMOD2_1;
		one.gas_level2_alarm_air_intake_of_b_deck_ahu = RANDMOD2_1;
		one.gas_detector_disabled_air_intake_of_b_deck_ahu = RANDMOD2_1;
		one.gas_detector_fault_air_intake_of_a_deck_ahu = RANDMOD2_1;
		one.gas_pre_alarm_air_intake_of_a_deck_ahu = RANDMOD2_1;
		one.gas_level1_alarm_air_intake_of_a_deck_ahu = RANDMOD2_1;
		one.gas_level2_alarm_air_intake_of_a_deck_ahu = RANDMOD2_1;
		one.gas_detector_disabled_air_intake_of_a_deck_ahu = RANDMOD2_1;
		one.gas_detector_fault_air_intake_of_below_main_deck_ahu = RANDMOD2_1;
		one.gas_pre_alarm_air_intake_of_below_main_deck_ahu = RANDMOD2_1;
		one.gas_level1_alarm_air_intake_of_below_main_deck_ahu = RANDMOD2_1;
		one.gas_level2_alarm_air_intake_of_below_main_deck_ahu = RANDMOD2_1;
		one.gas_detector_disabled_air_intake_of_below_main_deck_ahu = RANDMOD2_1;
		one.gas_detector_fault_air_intake_of_bow_thruster_room = RANDMOD2_1;
		one.gas_pre_alarm_air_intake_of_bow_thruster_room = RANDMOD2_1;
		one.gas_level1_alarm_air_intake_of_bow_thruster_room = RANDMOD2_1;
		one.gas_level2_alarm_air_intake_of_bow_thruster_room = RANDMOD2_1;
		one.gas_detector_disabled_air_intake_of_bow_thruster_room = RANDMOD2_1;
		one.gas_detector_fault_air_intake_of_aft_thruster_roomp = RANDMOD2_1;
		one.gas_pre_alarm_air_intake_of_aft_thruster_roomp = RANDMOD2_1;
		one.gas_level1_alarm_air_intake_of_aft_thruster_roomp = RANDMOD2_1;
		one.gas_level2_alarm_air_intake_of_aft_thruster_roomp = RANDMOD2_1;
		one.gas_detector_disabled_air_intake_of_aft_thruster_roomp = RANDMOD2_1;
		one.gas_detector_fault_air_intake_of_aft_thruster_rooms = RANDMOD2_1;
		one.gas_pre_alarm_air_intake_of_aft_thruster_rooms = RANDMOD2_1;
		one.gas_level1_alarm_air_intake_of_aft_thruster_rooms = RANDMOD2_1;
		one.gas_level2_alarm_air_intake_of_aft_thruster_rooms = RANDMOD2_1;
		one.gas_detector_disabled_air_intake_of_aft_thruster_rooms = RANDMOD2_1;
		one.gas_detector_fault_air_intake_of_engine_roomp = RANDMOD2_1;
		one.gas_pre_alarm_air_intake_of_engine_roomp = RANDMOD2_1;
		one.gas_level1_alarm_air_intake_of_engine_roomp = RANDMOD2_1;
		one.gas_level2_alarm_air_intake_of_engine_roomp = RANDMOD2_1;
		one.gas_detector_disabled_air_intake_of_engine_roomp = RANDMOD2_1;
		one.gas_detector_fault_air_intake_of_engine_rooms = RANDMOD2_1;
		one.gas_pre_alarm_air_intake_of_engine_rooms = RANDMOD2_1;
		one.gas_level1_alarm_air_intake_of_engine_rooms = RANDMOD2_1;
		one.gas_level2_alarm_air_intake_of_engine_rooms = RANDMOD2_1;
		one.gas_detector_disabled_air_intake_of_engine_rooms = RANDMOD2_1;
		one.gas_detector_fault_air_intake_of_aux_machinery_room1 = RANDMOD2_1;
		one.gas_pre_alarm_air_intake_of_aux_machinery_room1 = RANDMOD2_1;
		one.gas_level1_alarm_air_intake_of_aux_machinery_room1 = RANDMOD2_1;
		one.gas_level2_alarm_air_intake_of_aux_machinery_room1 = RANDMOD2_1;
		one.gas_detector_disabled_air_intake_of_aux_machinery_room1 = RANDMOD2_1;
		one.gas_detector_fault_air_intake_of_aux_machinery_room2 = RANDMOD2_1;
		one.gas_pre_alarm_air_intake_of_aux_machinery_room2 = RANDMOD2_1;
		one.gas_level1_alarm_air_intake_of_aux_machinery_room2 = RANDMOD2_1;
		one.gas_level2_alarm_air_intake_of_aux_machinery_room2 = RANDMOD2_1;
		one.gas_detector_disabled_air_intake_of_aux_machinery_room2 = RANDMOD2_1;
		one.gas_detector_fault_b_deck_entrancefr109p = RANDMOD2_1;
		one.gas_pre_alarm_b_deck_entrancefr109p = RANDMOD2_1;
		one.gas_level1_alarm_b_deck_entrancefr109p = RANDMOD2_1;
		one.gas_level2_alarm_b_deck_entrancefr109p = RANDMOD2_1;
		one.gas_detector_disabled_b_deck_entrancefr109p = RANDMOD2_1;
		one.gas_detector_fault_b_deck_entrancefr109s = RANDMOD2_1;
		one.gas_pre_alarm_b_deck_entrancefr109s = RANDMOD2_1;
		one.gas_level1_alarm_b_deck_entrancefr109s = RANDMOD2_1;
		one.gas_level2_alarm_b_deck_entrancefr109s = RANDMOD2_1;
		one.gas_detector_disabled_b_deck_entrancefr109s = RANDMOD2_1;
		one.gas_detector_fault_wheelhouse_entrancep = RANDMOD2_1;
		one.gas_pre_alarm_wheelhouse_entrancep = RANDMOD2_1;
		one.gas_level1_alarm_wheelhouse_entrancep = RANDMOD2_1;
		one.gas_level2_alarm_wheelhouse_entrancep = RANDMOD2_1;
		one.gas_detector_disabled_wheelhouse_entrancep = RANDMOD2_1;
		one.gas_detector_fault_wheelhouse_entrances = RANDMOD2_1;
		one.gas_pre_alarm_wheelhouse_entrances = RANDMOD2_1;
		one.gas_level1_alarm_wheelhouse_entrances = RANDMOD2_1;
		one.gas_level2_alarm_wheelhouse_entrances = RANDMOD2_1;
		one.gas_detector_disabled_wheelhouse_entrances = RANDMOD2_1;
#ifdef JANA505
		one.gas_detector_fault_b_deck_entrancefr90p = RANDMOD2_1;
		one.gas_pre_alarm_b_deck_entrancefr90p = RANDMOD2_1;
		one.gas_level1_alarm_b_deck_entrancefr90p = RANDMOD2_1;
		one.gas_level2_alarm_b_deck_entrancefr90p = RANDMOD2_1;
		one.gas_detector_disabled_b_deck_entrancefr90p = RANDMOD2_1;
		one.gas_detector_fault_b_deck_entrancefr90s = RANDMOD2_1;
		one.gas_pre_alarm_b_deck_entrancefr90s = RANDMOD2_1;
		one.gas_level1_alarm_b_deck_entrancefr90s = RANDMOD2_1;
		one.gas_level2_alarm_b_deck_entrancefr90s = RANDMOD2_1;
		one.gas_detector_disabled_b_deck_entrancefr90s = RANDMOD2_1;
#endif
#ifdef JANA508509
		one.gas_detector_fault_b_deck_entrancefr100p = RANDMOD2_1;
		one.gas_pre_alarm_b_deck_entrancefr100p = RANDMOD2_1;
		one.gas_level1_alarm_b_deck_entrancefr100p = RANDMOD2_1;
		one.gas_level2_alarm_b_deck_entrancefr100p = RANDMOD2_1;
		one.gas_detector_disabled_b_deck_entrancefr100p = RANDMOD2_1;
		one.gas_detector_fault_b_deck_entrancefr100s = RANDMOD2_1;
		one.gas_pre_alarm_b_deck_entrancefr100s = RANDMOD2_1;
		one.gas_level1_alarm_b_deck_entrancefr100s = RANDMOD2_1;
		one.gas_level2_alarm_b_deck_entrancefr100s = RANDMOD2_1;
		one.gas_detector_disabled_b_deck_entrancefr100s = RANDMOD2_1;
#endif
		one.gas_detector_fault_a_deck_entrancefr109p = RANDMOD2_1;
		one.gas_pre_alarm_a_deck_entrancefr109p = RANDMOD2_1;
		one.gas_level1_alarm_a_deck_entrancefr109p = RANDMOD2_1;
		one.gas_level2_alarm_a_deck_entrancefr109p = RANDMOD2_1;
		one.gas_detector_disabled_a_deck_entrancefr109p = RANDMOD2_1;
		one.gas_detector_fault_a_deck_entrancefr109s = RANDMOD2_1;
		one.gas_pre_alarm_a_deck_entrancefr109s = RANDMOD2_1;
		one.gas_level1_alarm_a_deck_entrancefr109s = RANDMOD2_1;
		one.gas_level2_alarm_a_deck_entrancefr109s = RANDMOD2_1;
		one.gas_detector_disabled_a_deck_entrancefr109s = RANDMOD2_1;
#ifdef JANA505		
		one.gas_detector_fault_a_deck_entrancefr90p = RANDMOD2_1;
		one.gas_pre_alarm_a_deck_entrancefr90p = RANDMOD2_1;
		one.gas_level1_alarm_a_deck_entrancefr90p = RANDMOD2_1;
		one.gas_level2_alarm_a_deck_entrancefr90p = RANDMOD2_1;
		one.gas_detector_disabled_a_deck_entrancefr90p = RANDMOD2_1;
		one.gas_detector_fault_a_deck_entrancefr90s = RANDMOD2_1;
		one.gas_pre_alarm_a_deck_entrancefr90s = RANDMOD2_1;
		one.gas_level1_alarm_a_deck_entrancefr90s = RANDMOD2_1;
		one.gas_level2_alarm_a_deck_entrancefr90sp = RANDMOD2_1;
		one.gas_detector_disabled_a_deck_entrancefr90s = RANDMOD2_1;
		one.gas_detector_fault_main_deck_entrancefr81m = RANDMOD2_1;
		one.gas_pre_alarm_main_deck_entrancefr81m = RANDMOD2_1;
		one.gas_level1_alarm_main_deck_entrancefr81m = RANDMOD2_1;
		one.gas_level2_alarm_main_deck_entrancefr81m = RANDMOD2_1;
		one.gas_detector_disabled_main_deck_entrancefr81m = RANDMOD2_1;
		one.gas_detector_fault_main_deck_entrancefr81s = RANDMOD2_1;
		one.gas_pre_alarm_main_deck_entrancefr81s = RANDMOD2_1;
		one.gas_level1_alarm_main_deck_entrancefr81s = RANDMOD2_1;
		one.gas_level2_alarm_main_deck_entrancefr81s = RANDMOD2_1;
		one.gas_detector_disabled_main_deck_entrancefr81s = RANDMOD2_1;

		one.gas_detector_fault_air_intake_of_construction_equipment_room = RANDMOD2_1;
		one.gas_pre_alarm_air_intake_of_construction_equipment_room = RANDMOD2_1;
		one.gas_level1_alarm_air_intake_of_construction_equipment_room = RANDMOD2_1;
		one.gas_level2_alarm_air_intake_of_construction_equipment_room = RANDMOD2_1;
		one.gas_detector_disabled_air_intake_of_construction_equipment_room = RANDMOD2_1;
#endif
#ifdef JANA508509
		one.gas_detector_fault_a_deck_entrancefr91p = RANDMOD2_1;
		one.gas_pre_alarm_a_deck_entrancefr91p = RANDMOD2_1;
		one.gas_level1_alarm_a_deck_entrancefr91p = RANDMOD2_1;
		one.gas_level2_alarm_a_deck_entrancefr91p = RANDMOD2_1;
		one.gas_detector_disabled_a_deck_entrancefr91p = RANDMOD2_1;
		one.gas_detector_fault_a_deck_entrancefr91s = RANDMOD2_1;
		one.gas_pre_alarm_a_deck_entrancefr91s = RANDMOD2_1;
		one.gas_level1_alarm_a_deck_entrancefr91s = RANDMOD2_1;
		one.gas_level2_alarm_a_deck_entrancefr91sp = RANDMOD2_1;
		one.gas_detector_disabled_a_deck_entrancefr91s = RANDMOD2_1;
		one.gas_detector_fault_main_deck_entrancefr91m = RANDMOD2_1;
		one.gas_pre_alarm_main_deck_entrancefr91m = RANDMOD2_1;
		one.gas_level1_alarm_main_deck_entrancefr91m = RANDMOD2_1;
		one.gas_level2_alarm_main_deck_entrancefr91m = RANDMOD2_1;
		one.gas_detector_disabled_main_deck_entrancefr91m = RANDMOD2_1;
		one.gas_detector_fault_main_deck_entrancefr91s = RANDMOD2_1;
		one.gas_pre_alarm_main_deck_entrancefr91s = RANDMOD2_1;
		one.gas_level1_alarm_main_deck_entrancefr91s = RANDMOD2_1;
		one.gas_level2_alarm_main_deck_entrancefr91s = RANDMOD2_1;
		one.gas_detector_disabled_main_deck_entrancefr91s = RANDMOD2_1;
#endif
		one.toxic_gas_detector_fault_top_of_wheelhouse = RANDMOD2_1;
		one.toxic_gas_pre_alarm_top_of_wheelhouse = RANDMOD2_1;
		one.toxic_gas_level1_alarm_top_of_wheelhouse = RANDMOD2_1;
		one.toxic_gas_level2_alarm_top_of_wheelhouse = RANDMOD2_1;
		one.toxic_gas_detector_disabled_top_of_wheelhouse = RANDMOD2_1;
		one.toxic_gas_detector_fault_air_intake_of_b_deck_ahu = RANDMOD2_1;
		one.toxic_gas_pre_alarm_air_intake_of_b_deck_ahu = RANDMOD2_1;
		one.toxic_gas_level1_alarm_air_intake_of_b_deck_ahu = RANDMOD2_1;
		one.toxic_gas_level2_alarm_air_intake_of_b_deck_ahu = RANDMOD2_1;
		one.toxic_gas_detector_disabled_air_intake_of_b_deck_ahu = RANDMOD2_1;
		one.toxic_gas_detector_fault_air_intake_of_a_deck_ahu = RANDMOD2_1;
		one.toxic_gas_pre_alarm_air_intake_of_a_deck_ahu = RANDMOD2_1;
		one.toxic_gas_level1_alarm_air_intake_of_a_deck_ahu = RANDMOD2_1;
		one.toxic_gas_level2_alarm_air_intake_of_a_deck_ahu = RANDMOD2_1;
		one.toxic_gas_detector_disabled_air_intake_of_a_deck_ahu = RANDMOD2_1;
		one.toxic_gas_detector_fault_air_intake_of_below_main_deck_ahu = RANDMOD2_1;
		one.toxic_gas_pre_alarm_air_intake_of_below_main_deck_ahu = RANDMOD2_1;
		one.toxic_gas_level1_alarm_air_intake_of_below_main_deck_ahu = RANDMOD2_1;
		one.toxic_gas_level2_alarm_air_intake_of_below_main_deck_ahu = RANDMOD2_1;
		one.toxic_gas_detector_disabled_air_intake_of_below_main_deck_ahu = RANDMOD2_1;
		one.toxic_gas_detector_fault_air_intake_of_bow_thruster_room = RANDMOD2_1;
		one.toxic_gas_pre_alarm_air_intake_of_bow_thruster_room = RANDMOD2_1;
		one.toxic_gas_level1_alarm_air_intake_of_bow_thruster_room = RANDMOD2_1;
		one.toxic_gas_level2_alarm_air_intake_of_bow_thruster_room = RANDMOD2_1;
		one.toxic_gas_detector_disabled_air_intake_of_bow_thruster_room = RANDMOD2_1;
		one.toxic_gas_detector_fault_air_intake_of_aft_thruster_roomp = RANDMOD2_1;
		one.toxic_gas_pre_alarm_air_intake_of_aft_thruster_roomp = RANDMOD2_1;
		one.toxic_gas_level1_alarm_air_intake_of_aft_thruster_roomp = RANDMOD2_1;
		one.toxic_gas_level2_alarm_air_intake_of_aft_thruster_roomp = RANDMOD2_1;
		one.toxic_gas_detector_disabled_air_intake_of_aft_thruster_roomp = RANDMOD2_1;
		one.toxic_gas_detector_fault_air_intake_of_aft_thruster_rooms = RANDMOD2_1;
		one.toxic_gas_pre_alarm_air_intake_of_aft_thruster_rooms = RANDMOD2_1;
		one.toxic_gas_level1_alarm_air_intake_of_aft_thruster_rooms = RANDMOD2_1;
		one.toxic_gas_level2_alarm_air_intake_of_aft_thruster_rooms = RANDMOD2_1;
		one.toxic_gas_detector_disabled_air_intake_of_aft_thruster_rooms = RANDMOD2_1;
		one.toxic_gas_detector_fault_air_intake_of_engine_roomp = RANDMOD2_1;
		one.toxic_gas_pre_alarm_air_intake_of_engine_roomp = RANDMOD2_1;
		one.toxic_gas_level1_alarm_air_intake_of_engine_roomp = RANDMOD2_1;
		one.toxic_gas_level2_alarm_air_intake_of_engine_roomp = RANDMOD2_1;
		one.toxic_gas_detector_disabled_air_intake_of_engine_roomp = RANDMOD2_1;
		one.toxic_gas_detector_fault_air_intake_of_engine_rooms = RANDMOD2_1;
		one.toxic_gas_pre_alarm_air_intake_of_engine_rooms = RANDMOD2_1;
		one.toxic_gas_level1_alarm_air_intake_of_engine_rooms = RANDMOD2_1;
		one.toxic_gas_level2_alarm_air_intake_of_engine_rooms = RANDMOD2_1;
		one.toxic_gas_detector_disabled_air_intake_of_engine_rooms = RANDMOD2_1;
		one.toxic_gas_detector_fault_air_intake_of_aux_machinery_room1 = RANDMOD2_1;
		one.toxic_gas_pre_alarm_air_intake_of_aux_machinery_room1 = RANDMOD2_1;
		one.toxic_gas_level1_alarm_air_intake_of_aux_machinery_room1 = RANDMOD2_1;
		one.toxic_gas_level2_alarm_air_intake_of_aux_machinery_room1 = RANDMOD2_1;
		one.toxic_gas_detector_disabled_air_intake_of_aux_machinery_room1 = RANDMOD2_1;
		one.toxic_gas_detector_fault_air_intake_of_aux_machinery_room2 = RANDMOD2_1;
		one.toxic_gas_pre_alarm_air_intake_of_aux_machinery_room2 = RANDMOD2_1;
		one.toxic_gas_level1_alarm_air_intake_of_aux_machinery_room2 = RANDMOD2_1;
		one.toxic_gas_level2_alarm_air_intake_of_aux_machinery_room2 = RANDMOD2_1;
		one.toxic_gas_detector_disabled_air_intake_of_aux_machinery_room2 = RANDMOD2_1;
		one.toxic_gas_detector_fault_b_deck_entrancefr109p = RANDMOD2_1;
		one.toxic_gas_pre_alarm_b_deck_entrancefr109p = RANDMOD2_1;
		one.toxic_gas_level1_alarm_b_deck_entrancefr109p = RANDMOD2_1;
		one.toxic_gas_level2_alarm_b_deck_entrancefr109p = RANDMOD2_1;
		one.toxic_gas_detector_disabled_b_deck_entrancefr109p = RANDMOD2_1;
		one.toxic_gas_detector_fault_b_deck_entrancefr109s = RANDMOD2_1;
		one.toxic_gas_pre_alarm_b_deck_entrancefr109s = RANDMOD2_1;
		one.toxic_gas_level1_alarm_b_deck_entrancefr109s = RANDMOD2_1;
		one.toxic_gas_level2_alarm_b_deck_entrancefr109s = RANDMOD2_1;
		one.toxic_gas_detector_disabled_b_deck_entrancefr109s = RANDMOD2_1;
		one.device_offline = 0;
	}

	bq_gas_detection_system.put(one);
	FILELOG_gas_detection_system(one);
}

void mainService::gen_data_cooling_water_system_test()
{
	cooling_water_system one;
	one.no1_propulsion_fw_cooling_pump_outlet_pressure = RANDMOD(6);
	one.no2_propulsion_fw_cooling_pump_outlet_pressure = RANDMOD(6);
	one.no1_propulsion_sw_cooling_pump_outlet_pressure = RANDMOD(6);
	one.no2_propulsion_sw_cooling_pump_outlet_pressure = RANDMOD(6);
	one.device_offline = 0;
	bq_cooling_water_system.put(one);
}

void mainService::gen_data_compressed_air_system_test()
{
	static int loop = 0;
	static compressed_air_system one;
	if(loop++ % alarm_gap == 0)
	{	
		one.no1_air_compressor_common_alarm = RANDMOD2_1;
		one.no2_air_compressor_common_alarm = RANDMOD2_1;
		one.service_air_compressor_common_alarm = RANDMOD2_1;
		one.quick_closing_valve_air_reservoir_low_pressure = RANDMOD2_1;
		one.air_dryer_common_alarm = RANDMOD2_1;
	}

	one.no1_main_air_reservoir_pressure = RANDMOD(40);
	one.no2_main_air_reservoir_pressure = RANDMOD(40);
	one.daily_air_reservoir_pressure = RANDMOD(16);
	one.no1_main_deck_air_reservoir_pressure = RANDMOD(16);
	one.no2_main_deck_air_reservoir_pressure = RANDMOD(16);
	one.no3_main_deck_air_reservoir_pressure = RANDMOD(16);
	one.no4_main_deck_air_reservoir_pressure = RANDMOD(16);
	
	one.device_offline = 0;
	bq_compressed_air_system.put(one);

}


void mainService::gen_data_air_conditioner_refrigeration_system_test()
{
	static int loop = 0;
	static air_conditioner_refrigeration_system one;
	if(loop++ % alarm_gap == 0)
	{
		one.no1_wheel_house_air_condition_common_alarm = RANDMOD2_1;
		one.no2_wheel_house_air_condition_common_alarm = RANDMOD2_1;
		one.no1_galley_package_air_condition_common_alarm = RANDMOD2_1;
		one.no2_galley_package_air_condition_common_alarm = RANDMOD2_1;
		one.m_s_b_room_package_air_condition_common_alarm = RANDMOD2_1;
		one.no1_jacking_vfd_room_air_condition_common_alarm = RANDMOD2_1;
		one.no2_jacking_vfd_room_air_condition_common_alarm = RANDMOD2_1;
		one.deck_storeroom_air_condition_common_alarm = RANDMOD2_1;
		one.paint_locker_room_air_condition_common_alarm = RANDMOD2_1;
		one.workshop_air_condition_common_alarm = RANDMOD2_1;
		one.a_z_v_f_d_room_air_condition_common_alarm = RANDMOD2_1;
		one.no1_a_c_chilled_unit_common_alarm = RANDMOD2_1;
		one.no2_a_c_chilled_unit_common_alarm = RANDMOD2_1;
		one.no3_a_c_chilled_unit_common_alarm = RANDMOD2_1;
		one.chiller_water_pump_system_common_alarm = RANDMOD2_1;
		one.no1_ahu_common_alarm = RANDMOD2_1;
		one.no2_ahu_common_alarm = RANDMOD2_1;
		one.no3_ahu_common_alarm = RANDMOD2_1;
		one.no4_ahu_common_alarm = RANDMOD2_1;
		one.refrigeration_plant_common_alarm = RANDMOD2_1;
		one.refrigerant_leaking_alarm = RANDMOD2_1;	
		one.device_offline = 0;
	}

	bq_air_conditioner_refrigeration_system.put(one);
}

void mainService::gen_data_miscellaneous_test()
{
	static int loop = 0;
	static miscellaneous one;
	if(loop++ % alarm_gap == 0)
	{
		one.fire_alarm_control_unit_system_failure = RANDMOD2_1;
		one.fire_alarm = RANDMOD2_1;
		one.sewage_water_treatment_plant_common_alarm = RANDMOD2_1;
		one.fuel_oil_purifier_common_alarm = RANDMOD2_1;
		one.oil_water_separator_common_alarm = RANDMOD2_1;
		one.oil_water_separator15ppm_alarm = RANDMOD2_1;
		one.oil_water_separator_running_timeout_alarm_nc = RANDMOD2_1;
		one.no1_fresh_water_generator_common_alarm = RANDMOD2_1;
		one.no2_fresh_water_generator_common_alarm = RANDMOD2_1;
		one.fresh_water_generator_no1_boost_pump_overload = RANDMOD2_1;
		one.fresh_water_generator_no2_boost_pump_overload = RANDMOD2_1;
		one.water_sprinkler_system_common_alarm = RANDMOD2_1;
		one.no1_submersible_pump_failure = RANDMOD2_1;
		one.no1_submersible_pump_running = RANDMOD2_1;
		one.no2_submersible_pump_failure = RANDMOD2_1;
		one.no2_submersible_pump_running = RANDMOD2_1;
		one.no3_submersible_pump_failure = RANDMOD2_1;
		one.no3_submersible_pump_running = RANDMOD2_1;
		one.no4_submersible_pump_failure = RANDMOD2_1;
		one.no4_submersible_pump_running = RANDMOD2_1;
		one.ac440v_galley_equipment_db_low_insulation = RANDMOD2_1;
		one.ac230v_galley_equipment_db_low_insulation = RANDMOD2_1;
		one.co2_release_alarm_system_power_failure_alarm = RANDMOD2_1;
		one.co2_leakage_alarm = RANDMOD2_1;
		one.co2_release_alarm = RANDMOD2_1;
		one.e_r_co2_release_pre_alarm = RANDMOD2_1;
		one.e_r_co2_release_valve_opened = RANDMOD2_1;
		one.paint_room_co2_release_pre_alarm = RANDMOD2_1;
		one.paint_room_co2_release_valve_opened = RANDMOD2_1;
		one.no1_anti_fouling_device_common_alarm = RANDMOD2_1;
		one.no2_anti_fouling_device_common_alarm = RANDMOD2_1;
		one.uv_sterilizer_common_alarm = RANDMOD2_1;
		one.navigation_aids_system_common_alarm = RANDMOD2_1;
		one.public_address_general_alarm_system_fault = RANDMOD2_1;
		one.public_address_general_alarm_system_power_fault = RANDMOD2_1;
		one.auto_telephone_system_failure = RANDMOD2_1;
		one.audio_visual_alarm_column_main_power_failure = RANDMOD2_1;
		one.audio_visual_alarm_column_backup_power_failure = RANDMOD2_1;
		one.no1_leg_control_unit_common_alarm = RANDMOD2_1;
		one.no2_leg_control_unit_common_alarm = RANDMOD2_1;
		one.no3_leg_control_unit_common_alarm = RANDMOD2_1;
		one.no4_leg_control_unit_common_alarm = RANDMOD2_1;
		one.fog_horn_controller_power_failure = RANDMOD2_1;
		one.bnwas_main_unit_power_failure = RANDMOD2_1;
		one.fresh_water_hydrophore_failure = RANDMOD2_1;
		one.sea_water_hydrophore_failure = RANDMOD2_1;
		one.calorifier_failure = RANDMOD2_1;
		one.no1_ups_for_ams_power_failure = RANDMOD2_1;
		one.no2_ups_for_ams_power_failure = RANDMOD2_1;
		one.no3_ups_for_ams_power_failure = RANDMOD2_1;
		one.no1_crane_fire_alarm = RANDMOD2_1;
		one.no2_crane_fire_alarm = RANDMOD2_1;
		one.engineer_alarm_system_power_failure = RANDMOD2_1;
		one.freshwater_filling_flowmeter = RANDMOD(10);
		one.device_offline = 0;
	}

	bq_miscellaneous.put(one);
}


void mainService::write_plc_datetime()
{
	while(1)
	{
		std::time_t currentTime = std::time(nullptr);

		// 将时间转换为本地时间
		std::tm* localTime = std::localtime(&currentTime);

		uint16_t datetime[7];
		// 提取年月日时分秒信息
		datetime[0] = localTime->tm_year + 1900; // 年份需要加1900
		datetime[1] = localTime->tm_mon + 1; // 月份范围是0-11,所以需要加1
		datetime[2] = localTime->tm_mday;
		datetime[3]  = localTime->tm_hour;
		datetime[4] = localTime->tm_min;
		datetime[5] = localTime->tm_sec;
		datetime[6] = 111;

        {
            std::lock_guard<std::mutex> lock(*m_506_sau1_mux);
		    int res = m_mb_506_sau1->modbus_write_registers(9900, 7, datetime);
		}

        datetime[6] = 0;
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
        {
            std::lock_guard<std::mutex> lock(*m_506_sau1_mux);
		    int res = m_mb_506_sau1->modbus_write_registers(9900, 7, datetime);
		}

		std::this_thread::sleep_for(std::chrono::seconds(3600));
	}
}


void mainService::connect_plc()
{
#ifndef MAHTS648
	if(!m_mb_plc1)
	{
		m_mb_plc1 = new modbus(SAU_IP1,SIEMENS_PORT_502);
		m_mb_plc1->modbus_set_slave_id(1);
	}

	if(!m_mb_plc1_503)
	{
		m_mb_plc1_503 = new modbus(SAU_IP1,SIEMENS_PORT_503);
		m_mb_plc1_503->modbus_set_slave_id(1);
	}
	
	if(!m_mb_plc1_504)
	{
		m_mb_plc1_504 = new modbus(SAU_IP1,SIEMENS_PORT_504);
		m_mb_plc1_504->modbus_set_slave_id(1);
	}
	
	if(!m_mb_plc2)
	{
		m_mb_plc2 = new modbus(SAU_IP2,SIEMENS_PORT_502);
		m_mb_plc2->modbus_set_slave_id(1);
	}
#else

    if(!m_mb_502)
	{
		m_mb_502 = new modbus(SAU_IP1,SIEMENS_PORT_502);
		m_mb_502->modbus_set_slave_id(1);
	}
	if(!m_mb_503)
	{
		m_mb_503 = new modbus(SAU_IP1,SIEMENS_PORT_503);
		m_mb_503->modbus_set_slave_id(1);
	}

	// if(!m_mb_504)
	// {
	// 	m_mb_504 = new modbus(SAU_IP1,SIEMENS_PORT_504);
	// 	m_mb_504->modbus_set_slave_id(1);
	// }

    ////////////
	if(!m_mb_506_sau1)
	{
		m_mb_506_sau1 = new modbus(SAU_IP1,SIEMENS_PORT_506);
		m_mb_506_sau1->modbus_set_slave_id(1);
	}
    // if(!m_mb_506_sau2)
	// {
	// 	m_mb_506_sau2 = new modbus(SAU_IP2,SIEMENS_PORT_506);
	// 	m_mb_506_sau2->modbus_set_slave_id(1);
	// }



#endif

   //return;

	while(1)
	{
#ifndef MAHTS648
		if (!m_mb_plc1->is_connected())
		{
			if (!m_mb_plc1->modbus_connect())
			{
				sleep(3);
				continue;
			}
		}
		if (!m_mb_plc1_503->is_connected())
		{
			if (!m_mb_plc1_503->modbus_connect())
			{
				sleep(3);
				continue;
			}
		}
		if (!m_mb_plc1_504->is_connected())
		{
			if (!m_mb_plc1_504->modbus_connect())
			{
				sleep(3);
				continue;
			}
		}
		if (!m_mb_plc2->is_connected())
		{
			m_mb_plc2->modbus_connect(); // plc2先不管成功与否
		}
		break;

#else
        if (!m_mb_502->is_connected())
		{
			if (!m_mb_502->modbus_connect())
			{
				sleep(3);
				continue;
			}
		}

		if (!m_mb_503->is_connected())
		{
			if (!m_mb_503->modbus_connect())
			{
				sleep(3);
				continue;
			}
		}
		// if (!m_mb_504->is_connected())
		// {
		// 	if (!m_mb_504->modbus_connect())
		// 	{
		// 		sleep(3);
		// 		continue;
		// 	}
		// }
        //////////////
		if (!m_mb_506_sau1->is_connected())
		{
			if (!m_mb_506_sau1->modbus_connect())
			{
				sleep(3);
				continue;
			}
		}

		
        // if (!m_mb_506_sau2->is_connected())
		// {
		// 	if (!m_mb_506_sau2->modbus_connect())
		// 	{
		// 		sleep(3);
		// 		continue;
		// 	}
		// }
		

		break;
#endif
	}
}


void mainService::gen_data_sau_test()
{
	while(1)
	{
		for(int i = 0; i < sau_nums; i++)
		{
			ams_sau one;
			one.status = 1;//0：离线 1：在线	
			one.device_offline = 0; //1： 离线 0：在线	

			bq_ams_sau[i].put(one);
		}
		sleep(1);
	}
}

void mainService::check_pc_status_test(int i)
{
	string test_pcs[]={
		"192.168.110.243",
		"192.168.100.84",
		"192.168.100.86",
		"192.168.100.244",//测试不在线
		};

	string ip;
	int result;
	bool res;
	int iloop = 0;
	
	ams_pc one;
	one.ip = test_pcs[i];
	one.name = pc_name[i];
	one.current = 0;
	one.location = pc_location[i];
	one.status = 1;//0：离线 1：在线	
	one.device_offline = 0; //1： 离线 0：在线	
	m_pc_status_test = 0;
	std::string pingCommand = "ping -c 1 " + one.ip + " > /dev/null";
	
	while(1)
	{
	    result = std::system(pingCommand.c_str());
	    res = (WEXITSTATUS(result) == 0);
		if(!res)
		{
			{
				std::lock_guard<std::mutex> lock(m_mux_pc_status_test);
				m_pc_status_test |= (1<<i);
			}
			continue;
		}

		//cur pc ip可能会变，cur掉线会有其他pc连接上
		//if(one.ip == m_cur_pc_ip)
		//	one.current = 1;
		//else
		//	one.current = 0;
		
		bq_ams_pc[i].put(one);

std::this_thread::sleep_for(std::chrono::milliseconds(50));
		sleep(1);

		std::lock_guard<std::mutex> lock(m_mux_pc_status_test);
		m_pc_status_test &= ~(1<<i);	
	}

}


void mainService::check_pc_disconnect_test(int i)
{	
	string test_pcs[]={
		"192.168.110.243",
		"192.168.100.84",
		"192.168.100.86",
		"192.168.100.244",//测试不在线
		};

	ams_pc one;
	one.ip = test_pcs[i];
	one.name = pc_name[i];
	one.location = pc_location[i];
	one.status = 0;//0：离线 1：在线	
	one.device_offline = 1; //1： 离线 0：在线	
	one.current = 0;
	while(1)
	{
		int status = 0;
		{
			std::lock_guard<std::mutex> lock(m_mux_pc_status_test);
			status = (m_pc_status_test >> i) & 1;
		}
		if(status)
		{
			bq_ams_pc[i].put(one);
		}
		sleep(1);
	}

}

//只处理断线情况
void mainService::check_pc_disconnect()
{	
	while(1)
	{
	    // if (m_mb_506_sau1 && m_mb_506_sau1->is_connected() && m_506_sau1_mux)
		// {

            
			sleep(1);
		// }
	}

}



void mainService::check_pc_status(int i)
{
	string ip;
	int result;
	bool res;
	int iloop = 0;
	std::string s = PC_IP_PREFIX + to_string(i + 3);
	m_pc_status = 0;
	std::string pingCommand = "ping -c 1 " + s + " > /dev/null";
	
	while(1)
	{
	    result = std::system(pingCommand.c_str());
	    res = (WEXITSTATUS(result) == 0);
		if(!res)  // ping失败
		{
			{
				std::lock_guard<std::mutex> lock(m_mux_pc_status);
				m_pc_status |= (1<<i);
			}


            ams_pc one;
			one.current = 0;
			if(m_cur_pc_ip == s)
			{
                one.current = 1;
			}


			one.ip = PC_IP_PREFIX + to_string(i + 3);
			one.name = pc_name[i];
			one.location = pc_location[i];
			one.status = 0;			// 0：离线 1：在线
			one.device_offline = 1; // 1： 离线 0：在线
			one.current = 0;
			bq_ams_pc[i].put(one);

		}
		else
		{
			{
			    std::lock_guard<std::mutex> lock(m_mux_pc_status);
			    m_pc_status &= (~(1<<i));
			}

			ams_pc one;
			one.current = 0;
			if(m_cur_pc_ip == s)
			{
                one.current = 1;
			}
			one.ip = PC_IP_PREFIX + to_string(i + 3);
			one.name = pc_name[i];
			one.current = 0;
			one.location = pc_location[i];
			one.status = 1;			// 0：离线 1：在线
			one.device_offline = 0; // 1： 离线 0：在线
			m_pc_status = 0;
			bq_ams_pc[i].put(one);
		}
		
		sleep(1);
	}

}


//只处理断线情况
void mainService::check_sau_disconnect()
{	
		// if (m_mb_506_sau1 && m_mb_506_sau1->is_connected() && m_506_sau1_mux)
		// {

			sleep(1);
		// }
}


void mainService::check_sau_status(int i)  //ok
{
	int result;
	bool res;
	char last_status = 0;
	string ip = (i == 0) ? SAU_IP1 : SAU_IP2;
	std::string pingCommand = "ping -c 1 " + ip + " > /dev/null";

	while(1)
	{
	    result = std::system(pingCommand.c_str());
	    res = (WEXITSTATUS(result) == 0);
		if(!res)
		{
			{
				std::lock_guard<std::mutex> lock(m_mux_sau_status);
				m_sau_status |= (1<<i);
			}
			
			ams_sau one;
	        one.status = 0;//0：离线 1：在线	
	        one.device_offline = 1; //1： 离线 0：在线
			bq_ams_sau[i].put(one);
            
		}
		else
		{
			{
				std::lock_guard<std::mutex> lock(m_mux_sau_status);
				m_sau_status &=(~(1<<i));
			}
			
			ams_sau one;
	        one.status = 1;//0：离线 1：在线	
	        one.device_offline = 0; //1： 离线 0：在线
			bq_ams_sau[i].put(one);
		}
		
        sleep(1);
	}
}


void mainService::check_self_network()
{
	while(1)
	{
		string inf_name = "eno1";//根据主机接口 enp1s0
		if(!isNetworkInterfaceUp(inf_name))
		{
			exit(0); //外部监控去重新启动进程
		}
		sleep(3);
	}
}

void mainService::gen_data_diesel_generator2(int i)
{
	
    main_diesel_generator_store& main_diesel_store =  m_client_main_gens[i].m_data_store;
     
    int ix;
	//int iy;
	int iloop;
	float temp;
	main_diesel_generator one;
	memset(&one,0,sizeof(one));
	one.gen_id = i;

    ix = 30 * i;
    one.d_g_dcu_unit_battery_volt_dcu_control_power = PLC1_VALUE_TEN2((main_diesel_store.d_g_dcu_unit_battery_volt_dcu_control_power),m_v_plc,ix);ix++;
	one.d_g_lube_oil_pressure = PLC1_VALUE_HUNDRED_1(m_v_plc,ix);ix++;
	one.d_g_coolant_pressure = PLC1_VALUE_HUNDRED_1(m_v_plc,ix);ix++;
	one.d_g_coolant_temperature = PLC1_VALUE_TEN2((main_diesel_store.d_g_coolant_temperature),m_v_plc,ix);ix++;
    one.d_g_lube_oil_temperature = PLC1_VALUE_TEN2((main_diesel_store.d_g_lube_oil_temperature),m_v_plc,ix);ix++;
	one.d_g_left_exhaust_temperature = PLC1_VALUE_ZERO2((main_diesel_store.d_g_left_exhaust_temperature),m_v_plc,ix);ix++;
	one.d_g_right_exhaust_temperature = PLC1_VALUE_ZERO2((main_diesel_store.d_g_right_exhaust_temperature),m_v_plc,ix);ix++;
           one.d_g_engine_speed = PLC1_VALUE_ZERO2((main_diesel_store.d_g_engine_speed),m_v_plc,ix);ix++;

	temp = PLC1_VALUE_ZERO(m_v_plc,ix);
	if(fabs(temp - io_error) < 1e-8)
		one.d_g_running_hours = 0;
	else
		one.d_g_running_hours = m_v_plc[ix]<<16;
	ix++;	
	temp = PLC1_VALUE_ZERO(m_v_plc,ix);
	if(fabs(temp - io_error) < 1e-8)
		temp = 0;
	one.d_g_running_hours += temp;

	ix++;
    one.d_g_rpu_unit_battery_a_volt_rpu_main_control_power = PLC1_VALUE_TEN2((main_diesel_store.d_g_rpu_unit_battery_a_volt_rpu_main_control_power),m_v_plc,ix);ix++;
	one.d_g_rpu_unit_battery_b_volt_rpu_backup_control_power = PLC1_VALUE_TEN2((main_diesel_store.d_g_rpu_unit_battery_b_volt_rpu_backup_control_power),m_v_plc,ix);ix++;
    one.d_g_inlet_oil_flowmeter = PLC1_VALUE_HUNDRED_1(m_v_plc,ix);ix++;
    one.d_g_outlet_oil_flowmeter = PLC1_VALUE_HUNDRED_1(m_v_plc,ix);ix++;

    
    ix = 24 + 30 * i;
	iloop = 0;
    one.d_g_coolant_level_low = PLC1_VALUE_DI2((main_diesel_store.d_g_coolant_level_low),m_v_plc,ix,iloop++);
    one.d_g_main_power_failure = PLC1_VALUE_DI2((main_diesel_store.d_g_main_power_failure),m_v_plc,ix,iloop++);
	one.d_g_backup_power_failure = PLC1_VALUE_DI2((main_diesel_store.d_g_backup_power_failure),m_v_plc,ix,iloop++);
    one.d_g_charger_failure = PLC1_VALUE_DI2((main_diesel_store.d_g_charger_failure),m_v_plc,ix,iloop++);
	one.d_g_alternator_cooling_water_leakage = PLC1_VALUE_DI2((main_diesel_store.d_g_alternator_cooling_water_leakage),m_v_plc,ix,iloop++);
	one.d_g_ecr_remote_emergency_stop = PLC1_VALUE_DI2((main_diesel_store.d_g_ecr_remote_emergency_stop),m_v_plc,ix,iloop++);
	one.d_g_lube_oil_pressure_too_low_shutdown = PLC1_VALUE_DI2((main_diesel_store.d_g_lube_oil_pressure_too_low_shutdown),m_v_plc,ix,iloop++);
	one.d_g_coolant_temperature_too_high_shutdown = PLC1_VALUE_DI2((main_diesel_store.d_g_coolant_temperature_too_high_shutdown),m_v_plc,ix,iloop++);
    one.d_g_local_emergency_stop = PLC1_VALUE_DI2((main_diesel_store.d_g_local_emergency_stop),m_v_plc,ix,iloop++);
	one.d_g_remote_emergency_stop = PLC1_VALUE_DI2((main_diesel_store.d_g_remote_emergency_stop),m_v_plc,ix,iloop++);
	one.d_g_rpu_emergency_stop = PLC1_VALUE_DI2((main_diesel_store.d_g_rpu_emergency_stop),m_v_plc,ix,iloop++);
    one.d_g_lube_oil_pressure_low_alarm = PLC1_VALUE_DI2((main_diesel_store.d_g_lube_oil_pressure_low_alarm),m_v_plc,ix,iloop++);
	one.d_g_coolant_pressure_low_alarm = PLC1_VALUE_DI2((main_diesel_store.d_g_coolant_pressure_low_alarm),m_v_plc,ix,iloop++);
    one.d_g_coolant_temperature_high_alarm = PLC1_VALUE_DI2((main_diesel_store.d_g_coolant_temperature_high_alarm),m_v_plc,ix,iloop++);
	one.d_g_lube_oil_temperature_high_alarm = PLC1_VALUE_DI2((main_diesel_store.d_g_lube_oil_temperature_high_alarm),m_v_plc,ix,iloop++);
	one.d_g_left_exhaust_temperature_high_alarm = PLC1_VALUE_DI2((main_diesel_store.d_g_left_exhaust_temperature_high_alarm),m_v_plc,ix,iloop++);

    ix++;
	iloop = 0;
	one.d_g_right_exhaust_temperature_high_alarm = PLC1_VALUE_DI2((main_diesel_store.d_g_right_exhaust_temperature_high_alarm),m_v_plc,ix,iloop++);
    one.d_g_overspeed_alarm = PLC1_VALUE_DI2((main_diesel_store.d_g_overspeed_alarm),m_v_plc,ix,iloop++);
	one.d_g_overspeed_shutdown = PLC1_VALUE_DI2((main_diesel_store.d_g_overspeed_shutdown),m_v_plc,ix,iloop++);
	one.d_g_speed_sensor_fail = PLC1_VALUE_DI2((main_diesel_store.d_g_speed_sensor_fail),m_v_plc,ix,iloop++);
	one.d_g_dcu_unit_battery_volt_dct_control_power_low = PLC1_VALUE_DI2((main_diesel_store.d_g_dcu_unit_battery_volt_dct_control_power_low),m_v_plc,ix,iloop++);
	one.d_g_start_fail = PLC1_VALUE_DI2((main_diesel_store.d_g_start_fail),m_v_plc,ix,iloop++);
	one.d_g_rpu_unit_battery_a_volt_rpu_main_control_power_low = PLC1_VALUE_DI2((main_diesel_store.d_g_rpu_unit_battery_a_volt_rpu_main_control_power_low),m_v_plc,ix,iloop++);
    one.d_g_rpu_unit_battery_b_volt_rpu_main_control_power_low = PLC1_VALUE_DI2((main_diesel_store.d_g_rpu_unit_battery_b_volt_rpu_main_control_power_low),m_v_plc,ix,iloop++);
    one.d_g_common_alarm = PLC1_VALUE_DI2((main_diesel_store.d_g_common_alarm),m_v_plc,ix,iloop++);
	one.d_g_common_warning = PLC1_VALUE_DI2((main_diesel_store.d_g_common_warning),m_v_plc,ix,iloop++);
	one.d_g_common_shutdown = PLC1_VALUE_DI2((main_diesel_store.d_g_common_shutdown),m_v_plc,ix,iloop++);
	one.d_g_common_cooldowm = PLC1_VALUE_DI2((main_diesel_store.d_g_common_cooldowm),m_v_plc,ix,iloop++);
	one.d_g_common_faultsensor_fail = PLC1_VALUE_DI2((main_diesel_store.d_g_common_faultsensor_fail),m_v_plc,ix,iloop++);
	one.d_g_remote_control_status_auxaccess_lock = PLC1_VALUE_DI2((main_diesel_store.d_g_remote_control_status_auxaccess_lock),m_v_plc,ix,iloop++);
	one.d_g_ready_to_start = PLC1_VALUE_DI2((main_diesel_store.d_g_ready_to_start),m_v_plc,ix,iloop++);
	one.d_g_ready_to_load = PLC1_VALUE_DI2((main_diesel_store.d_g_ready_to_load),m_v_plc,ix,iloop++);
	
    ix++;
	iloop = 0;
    one.d_g_running = PLC1_VALUE_DI2((main_diesel_store.d_g_running),m_v_plc,ix,iloop++);

    ix = 540 + i * 9;
	one.d_g_u_winding_temperature = PLC1_VALUE_TEN2((main_diesel_store.d_g_main_generator_u_winding_temperature),m_v_plc, ix);ix++;
	one.d_g_v_winding_temperature = PLC1_VALUE_TEN2((main_diesel_store.d_g_main_generator_v_winding_temperature),m_v_plc, ix);ix++;
	one.d_g_w_winding_temperature = PLC1_VALUE_TEN2((main_diesel_store.d_g_main_generator_w_winding_temperature),m_v_plc, ix);ix++;
	one.d_g_alternator_cooling_water_inlet_temp = PLC1_VALUE_TEN2((main_diesel_store.d_g_alternator_cooling_water_inlet_temp),m_v_plc, ix);ix++;
	one.d_g_alternator_cooling_water_outlet_temp = PLC1_VALUE_TEN2((main_diesel_store.d_g_alternator_cooling_water_outlet_temp),m_v_plc, ix);ix++;
	one.d_g_de_bearing_temperature = PLC1_VALUE_TEN2((main_diesel_store.d_g_main_generator_de_bearing_temperature),m_v_plc, ix);ix++;
	one.d_g_nde_bearing_temperature = PLC1_VALUE_TEN2((main_diesel_store.d_g_main_generator_nde_bearing_temperature),m_v_plc, ix);ix++;
	one.d_g_colling_air_cold_temperature= PLC1_VALUE_TEN2((main_diesel_store.d_g_colling_air_cold_temperature),m_v_plc, ix);ix++;
	one.d_g_colling_air_warm_temperature = PLC1_VALUE_TEN2((main_diesel_store.d_g_colling_air_warm_temperature),m_v_plc, ix);ix++;

    if(std::fabs(m_preU[i] - (-1000)) < 0.01)
	{
        if(one.d_g_u_winding_temperature > 0 && one.d_g_u_winding_temperature < 100)
		{
            m_preU[i] = one.d_g_u_winding_temperature;
		}
	}
	else
	{
        if(std::fabs(m_preU[i] - one.d_g_u_winding_temperature) > 20)
		{
            one.d_g_u_winding_temperature = m_preU[i];
		}
		else
		{
            m_preU[i] = one.d_g_u_winding_temperature;
		}
	}

    if(std::fabs(m_preV[i] - (-1000)) < 0.01)
	{
        if(one.d_g_v_winding_temperature > 0 && one.d_g_v_winding_temperature < 100)
		{
            m_preV[i] = one.d_g_v_winding_temperature;
		}
	}
	else
	{
        if(std::fabs(m_preV[i] - one.d_g_v_winding_temperature) > 20)
		{
            one.d_g_v_winding_temperature = m_preV[i];
		}
		else
		{
            m_preV[i] = one.d_g_v_winding_temperature;
		}
	}

	if(std::fabs(m_preW[i] - (-1000)) < 0.01)
	{
        if(one.d_g_w_winding_temperature > 0 && one.d_g_w_winding_temperature < 100)
		{
            m_preW[i] = one.d_g_w_winding_temperature;
		}
	}
	else
	{
        if(std::fabs(m_preW[i] - one.d_g_w_winding_temperature) > 20)
		{
            one.d_g_w_winding_temperature = m_preW[i];
		}
		else
		{
            m_preW[i] = one.d_g_w_winding_temperature;
		}
	}

	ix = 590 + i * 2;
	one.d_g_fresh_water_cooling_pump_outlet_pressure = PLC1_VALUE_PRESS_7_3((main_diesel_store.d_g_fresh_water_cooling_pump_outlet_pressure),m_v_plc, ix);ix++;
	one.d_g_sea_water_cooling_pump_outlet_pressure = PLC1_VALUE_PRESS_7_3((main_diesel_store.d_g_sea_water_cooling_pump_outlet_pressure),m_v_plc, ix);ix++;

    //>>>>>>>>>>>>>>>>>>>>>>>>>>

	m_vecTmpMachinery.push_back (one.d_g_main_power_failure);
	m_vecTmpMachinery.push_back (one.d_g_backup_power_failure);
	m_vecTmpMachinery.push_back (one.d_g_charger_failure);
	m_vecTmpMachinery.push_back (one.d_g_speed_sensor_fail);
	m_vecTmpMachinery.push_back (one.d_g_common_faultsensor_fail);

    //>>>>>>>>>>>>>>>>>>>>>>>>>>

	one.device_offline = 0;
	bq_main_diesel_generator[i].put(one);
	FILELOG_main_diesel_generator(one);
	m_client_main_gens[i].m_sensor_exception = 0;

    if( 0 == i )
	{
	    main_diesel_store.d_g_main_generator_u_winding_temperature.ha_bit = std::make_pair(0, 0);
        main_diesel_store.d_g_main_generator_v_winding_temperature.ha_bit = std::make_pair(0, 1);
        main_diesel_store.d_g_main_generator_w_winding_temperature.ha_bit = std::make_pair(0, 2);
        main_diesel_store.d_g_main_generator_de_bearing_temperature.ha_bit = std::make_pair(0, 3);
        main_diesel_store.d_g_main_generator_nde_bearing_temperature.ha_bit = std::make_pair(0, 4);
	}
	else if( 1 == i )
	{
	    main_diesel_store.d_g_main_generator_u_winding_temperature.ha_bit = std::make_pair(0, 5);
        main_diesel_store.d_g_main_generator_v_winding_temperature.ha_bit = std::make_pair(0, 6);
        main_diesel_store.d_g_main_generator_w_winding_temperature.ha_bit = std::make_pair(0, 7);
        main_diesel_store.d_g_main_generator_de_bearing_temperature.ha_bit = std::make_pair(0, 8);
        main_diesel_store.d_g_main_generator_nde_bearing_temperature.ha_bit = std::make_pair(0, 9);
	}
	else if( 2 == i )
	{
	    main_diesel_store.d_g_main_generator_u_winding_temperature.ha_bit = std::make_pair(0, 10);
        main_diesel_store.d_g_main_generator_v_winding_temperature.ha_bit = std::make_pair(0, 11);
        main_diesel_store.d_g_main_generator_w_winding_temperature.ha_bit = std::make_pair(0, 12);
        main_diesel_store.d_g_main_generator_de_bearing_temperature.ha_bit = std::make_pair(0, 13);
        main_diesel_store.d_g_main_generator_nde_bearing_temperature.ha_bit = std::make_pair(0, 14);
	}
	else if( 3 == i )
	{
	    main_diesel_store.d_g_main_generator_u_winding_temperature.ha_bit = std::make_pair(0, 15);
        main_diesel_store.d_g_main_generator_v_winding_temperature.ha_bit = std::make_pair(1, 0);
        main_diesel_store.d_g_main_generator_w_winding_temperature.ha_bit = std::make_pair(1, 1);
        main_diesel_store.d_g_main_generator_de_bearing_temperature.ha_bit = std::make_pair(1, 2);
        main_diesel_store.d_g_main_generator_nde_bearing_temperature.ha_bit = std::make_pair(1, 3);
	}

}

void mainService::gen_data_harbor_generator()
{
    int ix;
	//int iy;
	int iloop;
	float temp;
	harbo_generator one;
	memset(&one,0,sizeof(one));

	ix = 120;
	one.harbo_d_g_dcu_unit_battery_volt_dcu_control_power = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.harbo_d_g_lube_oil_pressure =  PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
    one.harbo_d_g_coolant_pressure =  PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
	one.harbo_d_g_coolant_temperature = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
	one.harbo_d_g_lube_oil_temperature = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
	// one.harbo_d_g_exhaust_temperature= PLC1_VALUE_TEN(m_v_plc,ix);ix++;
	one.harbo_d_g_exhaust_temperature= PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
    one.harbo_d_g_engine_speed = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;

	temp = PLC1_VALUE_ZERO(m_v_plc,ix);
	if( fabs(temp - io_error) < 1e-8)
		one.harbo_d_g_running_hours = 0;
	else
		one.harbo_d_g_running_hours = m_v_plc[ix]<<16;
	ix++;	
	temp = PLC1_VALUE_ZERO(m_v_plc,ix);
	if( fabs(temp - io_error) < 1e-8)
		temp = 0;
	one.harbo_d_g_running_hours += temp;
	ix++;

	one.harbo_d_g_rpu_unit_battery_a_volt_rpu_main_control_power = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.harbo_d_g_rpu_unit_battery_b_volt_rpu_backup_control_power = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.harbo_d_g_inlet_oil_flowmeter = PLC1_VALUE_HUNDRED_1(m_v_plc,ix);ix++;
	one.harbo_d_g_outlet_oil_flowmeter = PLC1_VALUE_HUNDRED_1(m_v_plc,ix);ix++;

    ix = 150;
	iloop = 0;
	one.harbo_d_g_coolant_level_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.harbo_d_g_main_power_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.harbo_d_g_backup_power_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.harbo_d_g_charger_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.harbo_d_g_lube_oil_pressure_too_low_shutdown = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.harbo_d_g_coolant_temperature_too_high_shutdown = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.harbo_d_g_local_emergency_stop = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.harbo_d_g_remote_emergency_stop = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.harbo_d_g_rpu_emergency_stop = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.harbo_d_g_lube_oil_pressure_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.harbo_d_g_coolant_pressure_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.harbo_d_g_coolant_temperature_high_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.harbo_d_g_lube_oil_temperature_high_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.harbo_d_g_exhaust_temperature_high_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.harbo_d_g_overspeed_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.harbo_d_g_overspeed_shutdown = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
	one.harbo_d_g_speed_pickup_sensor_fail = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.harbo_d_g_dcu_unit_battery_volt_dct_control_power_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.harbo_d_g_start_fail = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.harbo_d_g_rpu_unit_battery_a_volt_rpu_main_control_power_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.harbo_d_g_rpu_unit_battery_b_volt_rpu_main_control_power_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.harbo_d_g_common_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.harbo_d_g_common_warning = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.harbo_d_g_common_shutdown = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.harbo_d_g_common_cooldowm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.harbo_d_g_common_faultsensor_fail = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.harbo_d_g_remote_control_status_auxaccess_lock = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.harbo_d_g_ready_to_start = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.harbo_d_g_ready_to_load	= PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.harbo_d_g_running = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	m_vecTmpMachinery.push_back (one.harbo_d_g_main_power_failure);
	m_vecTmpMachinery.push_back (one.harbo_d_g_backup_power_failure);
	m_vecTmpMachinery.push_back (one.harbo_d_g_charger_failure);
	m_vecTmpMachinery.push_back (one.harbo_d_g_speed_pickup_sensor_fail);
	m_vecTmpMachinery.push_back (one.harbo_d_g_common_faultsensor_fail);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	one.device_offline = 0;
	bq_harbor_generator.put(one);
	m_client_harbor_generator.m_sensor_exception = 0;
}

void mainService::gen_data_emergency_generator2()
{
     emergency_generator_store& m_store = m_client_emergency_generator.m_data_store;
    /////////////////////////////////////////
    int ix;
	int iloop;
	float temp;
	emergency_generator one;
	memset(&one,0,sizeof(one));

    ix = 160;
	iloop = 0;
	one.eg_dcu_unit_battery_volt_dcu_control_power = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
	one.eg_lube_oil_pressure = PLC1_VALUE_HUNDRED_1(m_v_plc,ix);ix++;
    one.eg_coolant_pressure = PLC1_VALUE_HUNDRED_1(m_v_plc,ix);ix++;
	one.eg_coolant_temperature = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
	one.eg_lube_oil_temperature = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
	// one.eg_exhaust_temperature = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
	one.eg_exhaust_temperature = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.eg_engine_speed = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;

	temp = PLC1_VALUE_ZERO(m_v_plc,ix);
	if( fabs(temp - io_error) < 1e-8)
		one.eg_running_hours = 0;
	else
		one.eg_running_hours = m_v_plc[ix]<<16;
	ix++;	
	temp = PLC1_VALUE_ZERO(m_v_plc,ix);
	if( fabs(temp - io_error) < 1e-8)
		temp = 0;
	one.eg_running_hours += temp;
	ix++;

    one.eg_rpu_unit_battery_a_volt_rpu_main_control_power = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
	one.eg_rpu_unit_battery_b_volt_rpu_backup_control_power = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    
	ix = 184;
	iloop = 0;
	one.eg_coolant_level_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_main_power_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_backup_power_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_no1_charger_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_no2_charger_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_fuel_leakage_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_local_emergency_stop = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_remote_emergency_stop = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.eg_rpu_emergency_stop = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_lube_oil_pressure_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_coolant_pressure_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_coolant_temperature_high_alarm	= PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_lube_oil_temperature_high_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.eg_overspeed_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_overspeed_shutdown = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_speed_sensor_fail = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
	one.eg_dcu_unit_battery_volt_dct_control_power_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_start_fail = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_rpu_unit_battery_a_volt_rpu_main_control_power_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_rpu_unit_battery_b_volt_rpu_main_control_power_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_common_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_common_warning = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_common_shutdown = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_common_cooldowm	= PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_common_faultsensor_fail = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_remote_control_status_auxaccess_lock = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.eg_ready_to_start = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_ready_to_load = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.eg_running = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	m_vecTmpMachinery.push_back (one.eg_main_power_failure);
	m_vecTmpMachinery.push_back (one.eg_backup_power_failure);
	m_vecTmpMachinery.push_back (one.eg_no1_charger_failure);
	m_vecTmpMachinery.push_back (one.eg_no2_charger_failure);
	m_vecTmpMachinery.push_back (one.eg_speed_sensor_fail);
	m_vecTmpMachinery.push_back (one.eg_common_faultsensor_fail);

	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	one.device_offline = 0;

	bq_emergency_generator.put(one);
	
	//FILELOG_emergency_generator(one);
	m_client_emergency_generator.m_sensor_exception = 0;

    
	m_store.eg_generator_u_winding_temperature.ha_bit = std::make_pair<uint16_t, uint16_t>(1, 4);
    m_store.eg_generator_v_winding_temperature.ha_bit = std::make_pair<uint16_t, uint16_t>(1, 5);
    m_store.eg_generator_w_winding_temperature.ha_bit = std::make_pair<uint16_t, uint16_t>(1, 6);
}

void mainService::gen_data_main_propulsion2()
{
		for(int i = 0; i < 6; i++)
	{
		m_client_main_propulsion[i].m_sensor_exception = 0;
		gen_data_main_propulsion2(i);
	}
}

void mainService::gen_data_main_propulsion2(int i)
{
    main_propulsion_store& m_store = m_client_main_propulsion[i].m_data_store;
    /////////////////////////////////////////////////////////////////////////////////////

    int ix = 0;
	int iloop = 0;

	main_propulsion one;

	one.no = i;
	ix = 200 + 40 * i;
	iloop = 0;
	one.main_propulsion_rudder_feedback_signal_missing = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
    iloop += 5;
	one.main_propulsion_control_system_main_power_failure = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_control_system_backup_power_failure = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	iloop++;
	one.main_propulsion_steering_command_missing_joystick = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_speed_command_missing_joystick = PLC1_VALUE_DI(m_v_plc, ix, iloop);iloop++;
	one.main_propulsion_fu_steering_control_failure = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_fu_speed_control_failure = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
    iloop += 2;
	one.main_propulsion_steering_command_missing_fwd_console = PLC1_VALUE_DI(m_v_plc, ix, iloop++);

    ix++;
	iloop = 0;
	one.main_propulsion_speed_command_missing_fwd_console = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_rudder_command_missing_aft_console = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_speed_command_missing_aft_console = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_common_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_fault = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_remote_control_system_communication_failure = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
    one.propulsion_vfd_isu_communication_lost = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
    one.propulsion_vfd_inu_communication_lost = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.propulsion_vfd_lcu_communication_lost = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
    iloop++;
	one.main_propulsion_vfd_fault = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_vfd_common_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_vfd_emergency_stop = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	iloop += 2;
	one.main_propulsion_vfd_emergency_stop_wire_broken = PLC1_VALUE_DI(m_v_plc, ix, iloop++);

	ix++;
	iloop = 0;
    one.main_propulsion_vfd_stop_temp = PLC1_VALUE_DI(m_v_plc, ix, iloop++);  //TBD
	one.main_propulsion_vfd_control_power_failure_ups = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_vfd_power_limit = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
    one.main_propulsion_vfd_isu_fault = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_vfd_inu_fault = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_vfd_lcu_fault = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
    one.azimuth_thruster_steering_vfd_fault = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
    one.main_propulsion_motor_de_leakage = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_motor_nde_leakage = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_vfd_inu_waring = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_vfd_lcu_waring = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	iloop += 5;

    ix++;
	iloop = 0;
	one.main_propulsion_motor_u_winding_temrpeature_high = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_motor_v_winding_temrpeature_high = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_motor_w_winding_temrpeature_high = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_motor_high_temrpeature_slowdown = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_motor_high_temrpeature_shutdown = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_motor_de_bearing_high_temperature_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_motor_de_bearing_high_temperature_slowdown = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_motor_de_bearing_high_temperature_shutdown = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_motor_nde_bearing_high_temperature_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_motor_nde_bearing_high_temperature_slowdown = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_motor_nde_bearing_high_temperature_shutdown = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
    one.main_propulsion_vfd_cooling_water_inlet_pressure_low_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
    one.main_propulsion_vfd_cooling_water_inlet_pressure_low_low_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_vfd_cooling_water_inlet_pressure_low_low_fault = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_vfd_lcu_outlet_pressure_high_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
    one.main_propulsion_vfd_lcu_diff_pressure_low_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);

	ix++;
	iloop = 0;
	one.main_propulsion_vfd_lcu_diff_pressure_low_low_fault = PLC1_VALUE_DI(m_v_plc, ix, iloop++);


    ix++;
	iloop = 0;
	one.main_propulsion_vfd_control_mode = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
    one.main_propulsion_steering_control_nfu_mode = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	iloop += 5;
	one.main_propulsion_shaft_lock_status = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_vfd_ready = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_vfd_remote_control_position = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_vfd_running = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_vfd_override = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_vfd_heater_on = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_vfd_power_on = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_motor_heater_on = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.main_propulsion_vfd_fu_nfu_mode = PLC1_VALUE_DI(m_v_plc, ix, iloop++);

	ix++;
	iloop = 0;
	one.main_propulsion_vfd_start_interlock = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	iloop++;
    one.azimuth_thruster_fu_nfu_mode = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	iloop += 5;
    one.azimuth_thruster_vfd_running = PLC1_VALUE_DI(m_v_plc, ix, iloop++);  //TBD


    ix = 209 + 40 * i;
	iloop = 0;
    one.main_propulsion_control_position = m_v_plc[ix++];
	one.main_propulsion_rudder_indication =(int16_t) m_v_plc[ix++];
	one.main_propulsion_vfd_rpm_indication = m_v_plc[ix++];
    one.main_propulsion_vfd_load_indication = m_v_plc[ix++];
    ix += 2;
    one.main_propulsion_motor_u_winding_temperature = PLC1_VALUE_TEN2(m_store.main_propulsion_motor_u_winding_temperature,m_v_plc,ix);ix++;
	one.main_propulsion_motor_v_winding_temperature = PLC1_VALUE_TEN2(m_store.main_propulsion_motor_v_winding_temperature,m_v_plc,ix);ix++;
	one.main_propulsion_motor_w_winding_temperature = PLC1_VALUE_TEN2(m_store.main_propulsion_motor_w_winding_temperature,m_v_plc,ix);ix++;
	one.main_propulsion_motor_de_bearing_temperature = PLC1_VALUE_TEN2(m_store.main_propulsion_motor_de_bearing_temperature,m_v_plc,ix);ix++;
	one.main_propulsion_motor_nde_bearing_temperature = PLC1_VALUE_TEN2(m_store.main_propulsion_motor_nde_bearing_temperature,m_v_plc,ix);ix++;
    ix += 2;
	one.main_propulsion_vfd_voltage = m_v_plc[ix++];
	one.main_propulsion_vfd_current = m_v_plc[ix++];
	one.main_propulsion_lcu_inlet_pressure = m_v_plc[ix++];
	one.main_propulsion_lcu_outlet_pressure = m_v_plc[ix++];
	one.main_propulsion_vfd_cooling_water_differential_pressure = m_v_plc[ix++];
	ix += 2;

	iloop = 0;
    one.azimuth_thruster_main_power_failure = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_backup_power_failure = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_shaft_lock_inlet_air_pressure_low_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_lub_oil_level_low_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_lub_oil_temp_high_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_sealing_oil_tank_level_low_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_sealing_oil_inlet_air_press_low_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_sealing_oil_inlet_air_press_high_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_angle_order_lost_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_angle_feedback_lost_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_steering_power_supply_fault = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
	one.azimuth_thruster_steering_vfd_fault_2 = PLC1_VALUE_DI(m_v_plc, ix, iloop++);

    ix += 2;
    one.azimuth_thruster_angle_feedback = m_v_plc[ix++];

    
	bq_main_propulsion[i].put(one);
	FILELOG_main_propulsion(one);

    
	
}

void mainService::gen_data_power_system2()
{
   int pms1_fail =  PLC1_VALUE_DI(m_v_plc, 520, 13);
   int pms2_fail =  PLC1_VALUE_DI(m_v_plc, 521, 2);

   power_system one;
   memset(&one, 0, sizeof(one));

   int ix = 300;
   int iloop = 0;

   one.dg1_start_fail = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg2_start_fail = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg3_start_fail = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg4_start_fail = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg1_acb_abnormal_trip = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg2_acb_abnormal_trip = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg3_acb_abnormal_trip = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg4_acb_abnormal_trip = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg1_acb_close_fail = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg2_acb_close_fail = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg3_acb_close_fail = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg4_acb_close_fail = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.ac690v_bus_a_insul_low = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.ac690v_bus_b_insul_low = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.ac690v_bus_a_voltage_high = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.ac690v_bus_b_voltage_high = PLC1_VALUE_DI(m_v_plc, ix, iloop++);

   ix++;
   iloop = 0;
   one.ac690v_bus_a_voltage_low = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.ac690v_bus_b_voltage_low = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.ac690v_bus_a_frequency_high = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.ac690v_bus_b_frequency_high = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.ac690v_bus_a_frequency_low = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.ac690v_bus_b_frequency_low = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.preferential_trip_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.ac690v_msb_common_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.ac440v_bus_a_insul_low = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.ac440v_bus_b_insul_low = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.ac220v_bus_a_insul_low = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.ac220v_bus_b_insul_low = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.ac440v_msb_em_cy_stop_and_pre_trip_source_fail = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.ac440v_msb_dc24v_source_fault = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.ac690v_msb_dc24v_source_a_fault = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.ac690v_msb_dc24v_source_b_fault = PLC1_VALUE_DI(m_v_plc, ix, iloop++);

   ix++;
   iloop = 0;
   one.dg1_acb_closed = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg2_acb_closed = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg3_acb_closed = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg4_acb_closed = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.ddbt_acb_closed = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.pst_acb_closed = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.sst_acb_closed = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.bt1_acb_closed = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.bt2_acb_closed = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dt1primary_acb_closed = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dt2primary_acb_closed = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dt1secondary_acb_closed = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dt2secondary_acb_closed = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.ac440v_bus_tie_breaker_close = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.shore_cb_closed = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg1_running = PLC1_VALUE_DI(m_v_plc, ix, iloop++);

   ix++;
   iloop = 0;
   one.dg2_running = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg3_running = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg4_running = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg1_common_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg2_common_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg3_common_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg4_common_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.stand_by_gen_changeover_alarm = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg1_voltage_high = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg1_voltage_low = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg1_frequency_high = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg1_frequency_low = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg2_voltage_high = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg2_voltage_low = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg2_frequency_high = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg2_frequency_low = PLC1_VALUE_DI(m_v_plc, ix, iloop++);

   ix++;
   iloop = 0;
   one.dg3_voltage_high = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg3_voltage_low = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg3_frequency_high = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg3_frequency_low = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg4_voltage_high = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg4_voltage_low = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg4_frequency_high = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.dg4_frequency_low = PLC1_VALUE_DI(m_v_plc, ix, iloop++);
   one.load_sharing_fail = PLC1_VALUE_DI(m_v_plc, ix, iloop++);

   one.ac690v_pms1_fault = pms1_fail;
   one.ac690v_pms2_fault = pms2_fail;

   ix = 320;
   iloop = 0;
   one.dg1_power_kw = PLC1_VALUE_ZERO(m_v_plc, ix);
   ix++;
   one.dg2_power_kw = PLC1_VALUE_ZERO(m_v_plc, ix);
   ix++;
   one.dg3_power_kw = PLC1_VALUE_ZERO(m_v_plc, ix);
   ix++;
   one.dg4_power_kw = PLC1_VALUE_ZERO(m_v_plc, ix);
   ix++;
   one.dg1_voltagev = PLC1_VALUE_ZERO(m_v_plc, ix);
   ix++;
   one.dg2_voltagev = PLC1_VALUE_ZERO(m_v_plc, ix);
   ix++;
   one.dg3_voltagev = PLC1_VALUE_ZERO(m_v_plc, ix);
   ix++;
   one.dg4_voltagev = PLC1_VALUE_ZERO(m_v_plc, ix);
   ix++;
   one.ac690v_bus_a_frequencyhz = PLC1_VALUE_TEN(m_v_plc, ix);
   ix++;
   one.ac690v_bus_b_frequencyhz = PLC1_VALUE_TEN(m_v_plc, ix);
   ix++;
   one.ac690v_bus_a_total_powerkw = PLC1_VALUE_ZERO(m_v_plc, ix);
   ix++;
   one.ac690v_bus_b_total_powerkw = PLC1_VALUE_ZERO(m_v_plc, ix);
   ix++;
   one.ac690v_bus_total_powerkw = PLC1_VALUE_ZERO(m_v_plc, ix);
   ix++;
   one.ac690v_bus_a_surplus_powerkw = PLC1_VALUE_ZERO(m_v_plc, ix);
   ix++;
   one.ac690v_bus_b_surplus_powerkw = PLC1_VALUE_ZERO(m_v_plc, ix);
   ix++;
   one.ac690v_bus_surplus_powerkw = PLC1_VALUE_ZERO(m_v_plc, ix);
   ix++;
   one.dg1_power_factor = PLC1_VALUE_HUNDRED(m_v_plc, ix);
   ix++;
   one.dg2_power_factor = PLC1_VALUE_HUNDRED(m_v_plc, ix);
   ix++;
   one.dg3_power_factor = PLC1_VALUE_HUNDRED(m_v_plc, ix);
   ix++;
   one.dg4_power_factor = PLC1_VALUE_HUNDRED(m_v_plc, ix);
   ix++;

   //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   m_vecTmpMachinery.push_back (one.ac440v_msb_em_cy_stop_and_pre_trip_source_fail);
   m_vecTmpMachinery.push_back (one.ac440v_msb_dc24v_source_fault);
   m_vecTmpMachinery.push_back (one.ac690v_msb_dc24v_source_a_fault);
   m_vecTmpMachinery.push_back (one.ac690v_msb_dc24v_source_b_fault);
   m_vecTmpMachinery.push_back (one.ac690v_pms1_fault);
   //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    
   one.device_offline = 0;

   bq_power_system.put(one);
   FILELOG_power_system(one);

   m_client_power_system.m_sensor_exception = 0;

   gen_data_power_system2_2();
	
}

void mainService::gen_data_msb()
{
    msb one;

    int ix = 304;
	int iloop = 9;

    one.hg_start_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.hg_acb_abnormal_trip = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.hg_rve_power = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.hg_over_current = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.hg_acb_close_fail = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.synchro_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.ac440v_bus_a_insulation_low440v = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
    one.ac440v_bus_b_insulation_low440v = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.ac220v_bus_a_insulation_low440v = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.ac220v_bus_b_insulation_low440v = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.pre_trippt = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dc24v_source_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.pt_ec_circuit_source_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.ac440v_bus_tie_cb_non_close = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.uneven_load_sharing = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.msb_es_line_break = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.msb_common_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.busbar_a_source_alive = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.busbar_b_source_alive = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.hg_acb_close = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dt1secondary_acb_closed440v = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dt2secondary_acb_closed440v = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.ac440v_bus_tie_breaker_close = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
    one.ac220v_bus_tie_breaker_close = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.shore_cb_closed440v = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.lt1primary_acb_close = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.lt1secondary_acb_close = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.lt2primary_acb_close = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.lt2secondary_acb_close = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.hg_running = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.eg_running = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix = 520;
	iloop = 6;
    one.esb_ac415v_bus_low_insulation = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.esb_ac220v_bus_low_insulation = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.esb_dc24v_power_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.em_cy_generator_overload = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	iloop++;
    one.esb_common_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.e_g_cb_abnormal_trip = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    iloop++;
	iloop++;
    one.ac440v_pms_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

    ix++;
	iloop = 0;
    one.ac440v_dc24_power_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	iloop++;
	iloop++;
    one.t3_cb_status = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.t4_cb_status = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.sc_cb_status = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

    ix = 340;
	one.hg_current_l1a = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
    one.hg_current_l2a = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
    one.hg_current_l3a = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
    one.hg_voltage_l1_l3v = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
    one.hg_voltage_l1_l2v = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
    one.hg_voltage_l2_l3v = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
    one.hg_frequencyhz = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.hg_reactive_powerkvar = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
    one.hg_power_factorcos_phi = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
    one.hg_powerkw = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
    
    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	m_vecTmpMachinery.push_back (one.dc24v_source_fault);
	m_vecTmpMachinery.push_back (one.pt_ec_circuit_source_failure);
	m_vecTmpMachinery.push_back (one.esb_dc24v_power_failure);
	m_vecTmpMachinery.push_back (one.ac440v_pms_fault);
	m_vecTmpMachinery.push_back (one.ac440v_dc24_power_fault);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	one.device_offline = 0;

	bq_msb.put(one);
	m_client_msb.m_sensor_exception = 0;
}

void mainService::gen_data_power_system2_2()
{
	power_system2 one;
	memset(&one,0,sizeof(one));
	
	int ix = 400;
	int iloop = 0;

	one.dg1_start_fail = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg2_start_fail = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg3_start_fail = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg4_start_fail = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg1_acb_abnormal_trip = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg2_acb_abnormal_trip = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg3_acb_abnormal_trip = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg4_acb_abnormal_trip = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg1_acb_close_fail = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg2_acb_close_fail = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg3_acb_close_fail = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg4_acb_close_fail = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.ac690v_bus_a_insul_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.ac690v_bus_b_insul_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.ac690v_bus_a_voltage_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.ac690v_bus_b_voltage_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
    one.ac690v_bus_a_voltage_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.ac690v_bus_b_voltage_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.ac690v_bus_a_frequency_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.ac690v_bus_b_frequency_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.ac690v_bus_a_frequency_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.ac690v_bus_b_frequency_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.preferential_trip_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.ac690v_msb_common_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.ac440v_bus_a_insul_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.ac440v_bus_b_insul_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.ac220v_bus_a_insul_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.ac220v_bus_b_insul_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.ac440v_msb_em_cy_stop_and_pre_trip_source_fail = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.ac440v_msb_dc24v_source_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.ac690v_msb_dc24v_source_a_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.ac690v_msb_dc24v_source_b_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
    one.dg1_acb_closed = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg2_acb_closed = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg3_acb_closed = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg4_acb_closed = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.ddbt_acb_closed = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.pst_acb_closed = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.sst_acb_closed = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.bt1_acb_closed = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.bt2_acb_closed = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dt1primary_acb_closed = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dt2primary_acb_closed = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dt1secondary_acb_closed = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dt2secondary_acb_closed = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.ac440v_bus_tie_breaker_close = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.shore_cb_closed = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.dg1_running = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
    one.dg2_running = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg3_running = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg4_running = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg1_common_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg2_common_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg3_common_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg4_common_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stand_by_gen_changeover_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg1_voltage_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg1_voltage_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg1_frequency_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg1_frequency_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg2_voltage_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg2_voltage_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg2_frequency_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg2_frequency_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

    ix++;
	iloop = 0;
    one.dg3_voltage_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg3_voltage_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg3_frequency_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg3_frequency_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg4_voltage_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg4_voltage_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg4_frequency_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dg4_frequency_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.load_sharing_fail = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix = 520;
	iloop = 13;
    one.ac690v_pms1_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix = 521;
	iloop = 2;
    one.ac690v_pms2_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);


    ix = 420;
	iloop = 0;
	one.dg1_power_kw = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
    one.dg2_power_kw = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
    one.dg3_power_kw = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
    one.dg4_power_kw = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
    one.dg1_voltagev = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
    one.dg2_voltagev = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
    one.dg3_voltagev = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
    one.dg4_voltagev = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
    one.ac690v_bus_a_frequencyhz = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
    one.ac690v_bus_b_frequencyhz = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
    one.ac690v_bus_a_total_powerkw = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
    one.ac690v_bus_b_total_powerkw = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
    one.ac690v_bus_total_powerkw = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
    one.ac690v_bus_a_surplus_powerkw = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
    one.ac690v_bus_b_surplus_powerkw = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
    one.ac690v_bus_surplus_powerkw = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
    one.dg1_power_factor = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
    one.dg2_power_factor = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
    one.dg3_power_factor = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;
    one.dg4_power_factor = PLC1_VALUE_HUNDRED(m_v_plc,ix);ix++;

    one.device_offline = 0;

	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	m_vecTmpMachinery.push_back (one.ac440v_msb_em_cy_stop_and_pre_trip_source_fail);
	m_vecTmpMachinery.push_back (one.ac440v_msb_dc24v_source_fault);
	m_vecTmpMachinery.push_back (one.ac690v_msb_dc24v_source_a_fault);
	m_vecTmpMachinery.push_back (one.ac690v_msb_dc24v_source_b_fault);
	m_vecTmpMachinery.push_back (one.ac690v_pms1_fault);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	bq_power_system2.put(one);

	m_client_power_system2.m_sensor_exception = 0;
}

void mainService::gen_data_air_conditioner_refrigeration_system2()
{
	air_conditioner_refrigeration_system one;
	int ix = 700;
	int iloop = 0;

    one.freezer_room_temp = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	ix++;
	one.chiller_room_temp = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.dry_provision_store_room_temp = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	ix++;
	one.refrigeration_plant_compress_1_running_time = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.refrigeration_plant_compress_2_running_time = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
   
    iloop = 0;
    one.refrigeration_plant_compressor_1_high_pressure_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.refrigeration_plant_compressor_1_overload_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.refrigeration_plant_compressor_1_overheat_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.refrigeration_plant_compressor_2_high_pressure_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.refrigeration_plant_compressor_2_overload_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.refrigeration_plant_compressor_2_overheat_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.refrigeration_plant_compressor_1_water_pressure_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.refrigeration_plant_compressor_2_water_pressure_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    iloop += 2;

	one.refrigeration_plant_compressor_1_running = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.refrigeration_plant_compressor_2_running = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.freezer_room_overheat = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	iloop++;
	one.chiller_room_overheat = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dry_provision_store_room_overheat = PLC1_VALUE_DI(m_v_plc,ix,iloop++);


	ix += 2;
	iloop = 0;
    one.no1_ac_chiller_compressor_1_running = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no1_ac_chiller_compressor_2_running = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no1_ac_chiller_antifreeze_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.no1_ac_chiller_chilled_water_flow_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.no1_ac_chiller_compressor_1_high_pressure_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no1_ac_chiller_compressor_1_over_temp = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no1_ac_chiller_compressor_1_overload = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no1_ac_chiller_cooling_water_1_flow_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no1_ac_chiller_compressor_1_low_pressure_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.no1_ac_chiller_compressor_1_oil_level_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no1_ac_chiller_compressor_2_high_pressure_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no1_ac_chiller_compressor_2_over_temp = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no1_ac_chiller_compressor_2_overload = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no1_ac_chiller_cooling_water_2_flow_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no1_ac_chiller_compressor_2_low_pressure_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.no1_ac_chiller_compressor_2_oil_level_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    ix++;
	iloop = 0;
	one.no1_ac_chiller_power_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
     
	ix++;
    one.no1_ac_chiller_chilled_water_inlet_temp = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.no1_ac_chiller_chilled_water_outlet_temp = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;

    ix = 700 + 39;
    iloop = 0;
	one.no2_ac_chiller_compressor_1_running = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no2_ac_chiller_compressor_2_running = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no2_ac_chiller_antifreeze_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.no2_ac_chiller_chilled_water_flow_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.no2_ac_chiller_compressor_1_high_pressure_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no2_ac_chiller_compressor_1_over_temp = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no2_ac_chiller_compressor_1_overload = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no2_ac_chiller_cooling_water_1_flow_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no2_ac_chiller_compressor_1_low_pressure_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.no2_ac_chiller_compressor_1_oil_level_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no2_ac_chiller_compressor_2_high_pressure_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no2_ac_chiller_compressor_2_over_temp = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no2_ac_chiller_compressor_2_overload = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no2_ac_chiller_cooling_water_2_flow_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	one.no2_ac_chiller_compressor_2_low_pressure_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.no2_ac_chiller_compressor_2_oil_level_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    ix++;
	iloop = 0;
	one.no2_ac_chiller_power_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

    ix++;
    one.no2_ac_chiller_chilled_water_inlet_temp = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
	one.no2_ac_chiller_chilled_water_outlet_temp = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;

    ix = 6;
	iloop = 5;
	one.no1_wheel_house_air_condition_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no2_wheel_house_air_condition_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no1_galley_package_air_condition_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no2_galley_package_air_condition_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
    one.no1_msb_room_package_air_condition_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
    one.no2_msb_room_package_air_condition_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
    one.no3_msb_room_package_air_condition_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
    one.no4_msb_room_package_air_condition_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.electric_workshop_package_air_condition_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.mechanical_workshop_package_air_condition_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.fwd_thruster_vfd_room_package_air_condition_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);

	ix++;
	iloop = 0;
	one.aft_thruster_vfd_room_package_air_condition_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
    one.ecr_air_condition_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
    one.no1_a_c_chilled_unit_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no2_a_c_chilled_unit_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
    one.chiller_water_pump_system_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no1_ahu_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no2_ahu_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
    one.no3_ahu_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.refrigeration_plant_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.refrigerant_leaking_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
    

	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	
	one.device_offline = 0;

	bq_air_conditioner_refrigeration_system.put(one);
	FILELOG_air_conditioner_refrigeration_system(one);
}

void mainService::gen_data_compressed_air_system2()
{
	compressed_air_system_store& m_store = m_client_compressed_air_system.m_data_store;
	///////////////////////////////////////////////////////////////////////////////////////////
	compressed_air_system one;
	int ix = 6;
	int iloop = 0;
	one.no1_air_compressor_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no2_air_compressor_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.service_air_compressor_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
    one.quick_closing_valve_air_reservoir_low_pressure = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.air_dryer_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
   
	ix = 95;
	one.no1_main_air_reservoir_pressure = PLC1_VALUE_PRESS_4_2(m_store.no1_main_air_reservoir_pressure,m_v_ios,ix);ix++;
	one.no2_main_air_reservoir_pressure = PLC1_VALUE_PRESS_4_2(m_store.no2_main_air_reservoir_pressure,m_v_ios,ix);ix++;
	one.daily_air_reservoir_pressure = PLC1_VALUE_PRESS_5_2(m_store.daily_air_reservoir_pressure,m_v_ios,ix);ix++;
	
    //>>>>>>>>>>>>>>>>>>>>>>>>>>

	one.device_offline = 0;

	bq_compressed_air_system.put(one);
	FILELOG_compressed_air_system(one);

    m_store.no1_main_air_reservoir_pressure.la_bit = std::make_pair<uint16_t, uint16_t>(4,4);
	m_store.no2_main_air_reservoir_pressure.la_bit = std::make_pair<uint16_t, uint16_t>(4,5);
	m_store.daily_air_reservoir_pressure.la_bit = std::make_pair<uint16_t, uint16_t>(4,6);
}

void mainService::gen_data_general_charger_and_discharger_panel2()
{
	general_charger_and_discharger_panel one;

	int ix = 521;
 	int iloop = 12;
	 one.no1_battery_discharge = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	 one.no1_battery_voltage_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	 one.no1_charger_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	 one.no1_charger_power_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
	 one.no1_charger_common_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	 one.no2_battery_discharge = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	 one.no2_battery_voltage_low = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	 one.no2_charger_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	 one.no2_charger_power_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	 one.no2_charger_common_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	 one.no3_battery_discharge = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	 one.no4_battery_discharge = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	 one.chp_bus_a_low_insulation = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	 one.chp_bus_b_low_insulation = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	 one.de1_low_insulation = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	 one.de1_common_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	 one.de2_low_insulation = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
	 one.de2_common_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);


	 //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	 m_vecTmpMachinery.push_back (one.no1_charger_failure);
	 m_vecTmpMachinery.push_back (one.no1_charger_power_failure);
	 m_vecTmpMachinery.push_back (one.no2_charger_failure);
	 m_vecTmpMachinery.push_back (one.no2_charger_power_failure);
	 //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	one.device_offline = 0;

	bq_general_charger_and_discharger_panel.put(one);
	FILELOG_general_charger_and_discharger_panel(one);
}

void mainService::gen_data_main_transformer2(int i)
{
	
	main_transformer_store& m_data = m_client_main_transformer[0].m_data_store;
	/////////////////////////////////////////////////////////////////////////
	main_transformer one;
	int ix = 0;
	int iloop = 0;
	one.no = i;

	switch(i)
	{
		case 0:
		    ix = 521;
		    iloop = 6;
		    one.transformer_door_openalarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
			one.transformer_thermostat_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
			one.transformer_high_temp_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

            ix = 490;
			one.winding_r_temp = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
			one.winding_s_temp = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
			one.winding_t_temp = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;

		    iloop = 0;
			one.mt_cooling_fan_stop = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
            one.mt_high_temp_high_trip_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
            one.mt_cooling_fan_overload = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
            one.mt_winding_d_temp_sensor_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
            one.mt_winding_r_temp_sensor_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
            one.mt_winding_s_temp_sensor_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
            one.mt_winding_t_temp_sensor_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

			break;
		case 1:
		    ix = 521;
		    iloop = 9;
			one.transformer_door_openalarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
			one.transformer_thermostat_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
			one.transformer_high_temp_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

			ix = 500;
			one.winding_r_temp = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
			one.winding_s_temp = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;
			one.winding_t_temp = PLC1_VALUE_ZERO(m_v_plc,ix);ix++;

		    iloop = 0;
			one.mt_cooling_fan_stop = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
            one.mt_high_temp_high_trip_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
            one.mt_cooling_fan_overload = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
            one.mt_winding_d_temp_sensor_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
            one.mt_winding_r_temp_sensor_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
            one.mt_winding_s_temp_sensor_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
            one.mt_winding_t_temp_sensor_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);


			
			break;
		default:
			break;
	}
    


    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	m_vecTmpMachinery.push_back (one.transformer_thermostat_failure);
	m_vecTmpMachinery.push_back (one.mt_winding_d_temp_sensor_failure);
	m_vecTmpMachinery.push_back (one.mt_winding_r_temp_sensor_failure);
	m_vecTmpMachinery.push_back (one.mt_winding_s_temp_sensor_failure);
	m_vecTmpMachinery.push_back (one.mt_winding_t_temp_sensor_failure);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


	one.device_offline = 0;


	// one.no1_main_transformer_u_winding_temp = PLC1_VALUE_TEN(m_v_ios,ix);ix++;
	// one.no1_main_transformer_v_winding_temp = PLC1_VALUE_TEN(m_v_ios,ix);ix++;
	// one.no1_main_transformer_w_winding_temp = PLC1_VALUE_TEN(m_v_ios,ix);ix++;
	// one.no2_main_transformer_u_winding_temp = PLC1_VALUE_TEN(m_v_ios,ix);ix++;
	// one.no2_main_transformer_v_winding_temp = PLC1_VALUE_TEN(m_v_ios,ix);ix++;
	// one.no2_main_transformer_w_winding_temp = PLC1_VALUE_TEN(m_v_ios,ix);ix++;
	bq_main_transformer[i].put(one);
	FILELOG_main_transformer(one);

	m_data.no1_main_transformer_u_winding_temp.ha_bit = std::make_pair<uint16_t, uint16_t>(1,7);
	m_data.no1_main_transformer_v_winding_temp.ha_bit = std::make_pair<uint16_t, uint16_t>(1,8);
	m_data.no1_main_transformer_w_winding_temp.ha_bit = std::make_pair<uint16_t, uint16_t>(1,9);
	m_data.no2_main_transformer_u_winding_temp.ha_bit = std::make_pair<uint16_t, uint16_t>(1,10);
	m_data.no2_main_transformer_v_winding_temp.ha_bit = std::make_pair<uint16_t, uint16_t>(1,11);
	m_data.no2_main_transformer_w_winding_temp.ha_bit = std::make_pair<uint16_t, uint16_t>(1,12);
}

void mainService::gen_data_e_r_charger_and_discharger_panel2()
{
	e_r_charger_and_discharger_panel one;
	int ix = 1;
	int iloop = 7;

    one.ups_common_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.ups_low_insulation = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.ups_battery_protection_trip = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
    //one.ups_battery_discharge = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.ups_battery_low_voltage = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.ups_battery_bypass_activation = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.ups_input_fault = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	
	one.device_offline = 0;

	bq_e_r_charger_and_discharger_panel.put(one);
	FILELOG_e_r_charger_and_discharger_panel(one);
}

void mainService::gen_data_level_gauging2()
{
	uint16_t tmp;
	level_gauging one;
	int ix = 522;
	int iloop = 14;
	one.s_g_roomp_bilge_well_high_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.s_g_rooms_bilge_well_high_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
    one.aft_cement_tkp_a_bilge_well_high_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.aft_cement_tks_a_bilge_well_high_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.aft_cement_tkm_bilge_well_high_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.fwd_cement_tkp_a_bilge_well_high_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.fwd_cement_tks_a_bilge_well_high_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.fwd_cement_tkm_bilge_well_high_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.e_rp_a_bilge_well_high_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.e_rs_a_bilge_well_high_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.e_rp_f_bilge_well_high_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.e_rs_f_bilge_well_high_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.b_t_f_bilge_well_high_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);//b_t_room_bilge_well_high_level->b_t_f_bilge_well_high_level 25.5.19
    one.base_oil_ro_tkp_high_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.base_oil_ro_tks_high_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.no1_mud_tkp_high_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.no1_mud_tks_high_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.no1_mud_tkp_low_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
    one.no1_mud_tks_low_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.no2_mud_tkp_high_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.no2_mud_tks_high_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.no2_mud_tkp_low_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.no2_mud_tks_low_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.no3_f_o_overflow_tkc_high_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.bilge_holding_tkp_high_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.dirty_oil_tk_high_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.sewage_tk_high_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.f_o_day_tkp_high_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.f_o_day_tkp_low_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.f_o_day_tks_high_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.f_o_day_tks_low_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.b_t_a_bilge_well_high_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.f_o_overflow_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.hpr_tks_high_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
    one.em_cy_generator_set_daily_tk_low_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.em_cy_generator_set_daily_tk_high_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.no1_bt_f_w_expansion_t_k_low_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.no2_bt_f_w_expansion_t_k_low_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.port_azimuth_f_w_expansion_t_k_low_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.stbd_azimuth_f_w_expansion_t_k_low_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.no1_f_w_expansion_t_k_low_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.no2_f_w_expansion_t_k_low_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.no3_f_w_expansion_t_k_low_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.no4_f_w_expansion_t_k_low_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.quick_closing_air_receiver_pressure_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix = 529;
	iloop = 13;
    one.output_mud_tk_level_alarm_bell = PLC1_VALUE_DI(m_v_plc,ix,iloop++);


    std::vector<char> currAlarms;
	currAlarms.push_back(one.no1_mud_tkp_high_level);
    currAlarms.push_back(one.no1_mud_tks_high_level);
    currAlarms.push_back(one.no1_mud_tkp_low_level);
    currAlarms.push_back(one.no1_mud_tks_low_level);
    currAlarms.push_back(one.no2_mud_tkp_high_level);
    currAlarms.push_back(one.no2_mud_tks_high_level);
    currAlarms.push_back(one.no2_mud_tkp_low_level);
    currAlarms.push_back(one.no2_mud_tks_low_level);
    m_mud_handler.SetCurrAlarms(currAlarms);


    ix = 700;
	iloop = 0;

    one.no1_db_f_o_tkp_level = PLC1_VALUE_THOUSAND(m_v_plc,ix);ix++;
    one.no1_db_f_o_tks_level = PLC1_VALUE_THOUSAND(m_v_plc,ix);ix++;
    one.no1_wing_f_o_tkp_level = PLC1_VALUE_THOUSAND(m_v_plc,ix);ix++;
    one.no1_wing_f_o_tks_level = PLC1_VALUE_THOUSAND(m_v_plc,ix);ix++;
    one.no2_db_f_o_tkp_level = PLC1_VALUE_THOUSAND(m_v_plc,ix);ix++;
    one.no2_db_f_o_tks_level = PLC1_VALUE_THOUSAND(m_v_plc,ix);ix++;
    one.no2_wing_f_o_tkp_level = PLC1_VALUE_THOUSAND(m_v_plc,ix);ix++;
    one.no2_wing_f_o_tks_level = PLC1_VALUE_THOUSAND(m_v_plc,ix);ix++;
    one.no3_db_f_o_tkcoverflow_level = PLC1_VALUE_THOUSAND(m_v_plc,ix);ix++;
    one.no3_db_f_o_tkp_level = PLC1_VALUE_THOUSAND(m_v_plc,ix);ix++;
    one.no3_db_f_o_tks_level = PLC1_VALUE_THOUSAND(m_v_plc,ix);ix++;
    one.no3_db_f_o_tkc_level = PLC1_VALUE_THOUSAND(m_v_plc,ix);ix++;
    one.no4_db_f_o_tkc_level = PLC1_VALUE_THOUSAND(m_v_plc,ix);ix++;
    one.no4_wing_f_o_tkp_level = PLC1_VALUE_THOUSAND(m_v_plc,ix);ix++;
    one.no4_wing_f_o_tks_level = PLC1_VALUE_THOUSAND(m_v_plc,ix);ix++;
    one.no5_f_o_tkp_level = PLC1_VALUE_THOUSAND(m_v_plc,ix);ix++;
    one.no5_f_o_tks_level = PLC1_VALUE_THOUSAND(m_v_plc,ix);ix++;
    one.no6_db_f_o_tkc_level = PLC1_VALUE_THOUSAND(m_v_plc,ix);ix++;
    one.f_o_day_tkp_level = PLC1_VALUE_THOUSAND(m_v_plc,ix);ix++;
    one.f_o_day_tks_level = PLC1_VALUE_THOUSAND(m_v_plc,ix);ix++;
    one.f_o_settling_tkp_level = PLC1_VALUE_THOUSAND(m_v_plc,ix);ix++;
    one.fresh_water_tank5p_level = PLC1_VALUE_THOUSAND(m_v_plc,ix);ix++;
    
	ix = 730;
	iloop = 0;
    one.no1_db_f_o_tkp_volume_m3 = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.no1_db_f_o_tks_volume_m3 = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.no1_wing_f_o_tkp_volume_m3 = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.no1_wing_f_o_tks_volume_m3 = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.no2_db_f_o_tkp_volume_m3 = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.no2_db_f_o_tks_volume_m3 = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.no2_wing_f_o_tkp_volume_m3 = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.no2_wing_f_o_tks_volume_m3 = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.no3_db_f_o_tkcoverflow_volume_m3 = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.no3_db_f_o_tkp_volume_m3 = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.no3_db_f_o_tks_volume_m3 = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.no3_db_f_o_tkc_volume_m3 = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.no4_db_f_o_tkc_volume_m3 = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.no4_wing_f_o_tkp_volume_m3 = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.no4_wing_f_o_tks_volume_m3 = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.no5_f_o_tkp_volume_m3 = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.no5_f_o_tks_volume_m3 = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.no6_db_f_o_tkc_volume_m3 = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.f_o_day_tkp_volume_m3 = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.f_o_day_tks_volume_m3 = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.f_o_settling_tkp_volume_m3 = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.fresh_water_tank5p_volume_m3 = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    
	ix = 760;
	iloop = 0;
    one.no1_db_f_o_tkp_volume_percent = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.no1_db_f_o_tks_volume_percent = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.no1_wing_f_o_tkp_volume_percent = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.no1_wing_f_o_tks_volume_percent = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.no2_db_f_o_tkp_volume_percent = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.no2_db_f_o_tks_volume_percent = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.no2_wing_f_o_tkp_volume_percent = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.no2_wing_f_o_tks_volume_percent = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.no3_db_f_o_tkcoverflow_volume_percent = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.no3_db_f_o_tkp_volume_percent = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.no3_db_f_o_tks_volume_percent = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.no3_db_f_o_tkc_volume_percent = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.no4_db_f_o_tkc_volume_percent = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.no4_wing_f_o_tkp_volume_percent = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.no4_wing_f_o_tks_volume_percent = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.no5_f_o_tkp_volume_percent = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.no5_f_o_tks_volume_percent = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.no6_db_f_o_tkc_volume_percent = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.f_o_day_tkp_volume_percent = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.f_o_day_tks_volume_percent = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.f_o_settling_tkp_volume_percent = PLC1_VALUE_TEN(m_v_plc,ix);ix++;
    one.fresh_water_tank5p_volume_percent = PLC1_VALUE_TEN(m_v_plc,ix);ix++;

    ix = 789;
	iloop = 0;
     one.no1_db_f_o_tkp_high_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.no1_db_f_o_tks_high_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.no1_wing_f_o_tkp_high_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.no1_wing_f_o_tks_high_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.no2_db_f_o_tkp_high_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.no2_db_f_o_tks_high_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.no2_wing_f_o_tkp_high_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.no2_wing_f_o_tks_high_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.no3_db_f_o_tkcoverflow_high_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.no3_db_f_o_tkp_high_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.no3_db_f_o_tks_high_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.no3_db_f_o_tkc_high_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.no4_db_f_o_tkc_high_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.no4_wing_f_o_tkp_high_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.no4_wing_f_o_tks_high_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.no5_f_o_tkp_high_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
     
     ix++;
     iloop = 0;
     one.no5_f_o_tks_high_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
     one.no6_db_f_o_tkc_high_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.f_o_day_tkp_high_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.f_o_day_tks_high_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.f_o_settling_tkp_high_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.fresh_water_tank5p_high_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     
     ix = 791;
     iloop = 0;
     one.no1_db_f_o_tkp_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.no1_db_f_o_tks_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.no1_wing_f_o_tkp_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.no1_wing_f_o_tks_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.no2_db_f_o_tkp_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.no2_db_f_o_tks_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.no2_wing_f_o_tkp_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.no2_wing_f_o_tks_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.no3_db_f_o_tkcoverflow_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.no3_db_f_o_tkp_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.no3_db_f_o_tks_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.no3_db_f_o_tkc_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.no4_db_f_o_tkc_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.no4_wing_f_o_tkp_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.no4_wing_f_o_tks_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.no5_f_o_tkp_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
     
     ix++;
     iloop = 0;
     one.no5_f_o_tks_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.no6_db_f_o_tkc_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.f_o_day_tkp_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.f_o_day_tks_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.f_o_settling_tkp_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.fresh_water_tank5p_low_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
	 one.f_o_setling_tkp_high_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.f_o_setling_tkp_low_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.f_o_setling_tks_high_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++); 
     one.f_o_setling_tks_low_level = PLC1_VALUE_DI(m_v_plc,ix,iloop++);


	//uint16_t flag = 0;
	currAlarms.clear();    
	currAlarms.push_back (one.s_g_roomp_bilge_well_high_level);
    currAlarms.push_back (one.s_g_rooms_bilge_well_high_level);
    currAlarms.push_back (one.aft_cement_tkp_a_bilge_well_high_level);
    currAlarms.push_back (one.aft_cement_tks_a_bilge_well_high_level);
    currAlarms.push_back (one.aft_cement_tkm_bilge_well_high_level);
    currAlarms.push_back (one.fwd_cement_tkp_a_bilge_well_high_level);
    currAlarms.push_back (one.fwd_cement_tks_a_bilge_well_high_level);
    currAlarms.push_back (one.fwd_cement_tkm_bilge_well_high_level);
    currAlarms.push_back (one.e_rp_a_bilge_well_high_level);
    currAlarms.push_back (one.e_rs_a_bilge_well_high_level);
    currAlarms.push_back (one.e_rp_f_bilge_well_high_level);
    currAlarms.push_back (one.e_rs_f_bilge_well_high_level);
	currAlarms.push_back (one.b_t_f_bilge_well_high_level);//b_t_room_bilge_well_high_level->b_t_f_bilge_well_high_level 25.5.19
    currAlarms.push_back (one.base_oil_ro_tkp_high_level);
    currAlarms.push_back (one.base_oil_ro_tks_high_level);
    currAlarms.push_back (one.no1_mud_tkp_high_level);
    currAlarms.push_back (one.no1_mud_tks_high_level);
    currAlarms.push_back (one.no1_mud_tkp_low_level);
    currAlarms.push_back (one.no1_mud_tks_low_level);
    currAlarms.push_back (one.no2_mud_tkp_high_level);
    currAlarms.push_back (one.no2_mud_tks_high_level);
    currAlarms.push_back (one.no2_mud_tkp_low_level);
    currAlarms.push_back (one.no2_mud_tks_low_level);
    currAlarms.push_back (one.no3_f_o_overflow_tkc_high_level);
    currAlarms.push_back (one.bilge_holding_tkp_high_level);
    currAlarms.push_back (one.dirty_oil_tk_high_level);
    currAlarms.push_back (one.sewage_tk_high_level);
    currAlarms.push_back (one.f_o_day_tkp_high_level);
    currAlarms.push_back (one.f_o_day_tkp_low_level);
    currAlarms.push_back (one.f_o_day_tks_high_level);
    currAlarms.push_back (one.f_o_day_tks_low_level);
	currAlarms.push_back (one.b_t_a_bilge_well_high_level);
    currAlarms.push_back (one.f_o_overflow_alarm);
    currAlarms.push_back (one.hpr_tks_high_level);
    currAlarms.push_back (one.em_cy_generator_set_daily_tk_low_level);
    currAlarms.push_back (one.em_cy_generator_set_daily_tk_high_level);
    currAlarms.push_back (one.no1_bt_f_w_expansion_t_k_low_level);
    currAlarms.push_back (one.no2_bt_f_w_expansion_t_k_low_level);
    currAlarms.push_back (one.port_azimuth_f_w_expansion_t_k_low_level);
    currAlarms.push_back (one.stbd_azimuth_f_w_expansion_t_k_low_level);
    currAlarms.push_back (one.no1_f_w_expansion_t_k_low_level);
    currAlarms.push_back (one.no2_f_w_expansion_t_k_low_level);
    currAlarms.push_back (one.no3_f_w_expansion_t_k_low_level);
    currAlarms.push_back (one.no4_f_w_expansion_t_k_low_level);
    // currAlarms.push_back (one.quick_closing_air_receiver_pressure_low_alarm);
	m_vecTmpMachinery.push_back (one.quick_closing_air_receiver_pressure_low_alarm);
	
	currAlarms.push_back (one.f_o_setling_tkp_high_level);
	currAlarms.push_back (one.f_o_setling_tkp_low_level);
	currAlarms.push_back (one.f_o_setling_tks_high_level);
	currAlarms.push_back (one.f_o_setling_tks_low_level);
    m_level_handler.SetCurrAlarms(currAlarms);

	m_client_level_gauging.m_sensor_exception = 0;
	one.device_offline = 0;

	bq_level_gauging.put(one);
	FILELOG_level_gauging(one);
}

void mainService::gen_data_level_switch2()
{
	level_switch one;
	int ix = 1;
	int iloop = 14;

	one.fwd_thruster_vfd_room_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.aft_thruster_vfd_room_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);

	ix++;
	iloop = 0;
	one.fuel_oil_day_tank_p_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.fuel_oil_day_tank_p_low_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
    one.no1_expansion_tank_low_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no2_expansion_tank_low_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
    one.fuel_oil_day_tank_s_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.fuel_oil_day_tank_s_low_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
    one.lube_oil_tank_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
    one.dirty_oil_tank_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
    one.black_water_tank_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
    one.grey_water_tank_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
    one.e_g_m_d_o_service_tank_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.e_g_m_d_o_service_tk_low_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.aft_thruster_room_p_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.aft_thruster_room_c_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.aft_thruster_room_s_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
    one.pump_room_p_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);

	ix++;
	iloop = 0;
	one.pump_room_s_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.store_room_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.compressor_room_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.water_maker_room_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.transformer_room_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.chiller_room_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.switchboard_room_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.spill_tank_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no6_void_p_tank_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no6_void_s_tank_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
    one.aux_machinery_room_p_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.aux_machinery_room_s_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.engine_room_p_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.engine_room_s_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.engine_room_c_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.pump_room_c_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);

	ix++;
	iloop = 0;
	one.fwd_thruster_rm_p_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.fwd_thruster_rm_c_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.fwd_thruster_rm_s_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no3_void_p_tank_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no3_void_s_tank_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no5_void_p_tank_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no5_void_s_tank_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no19_void_p_tank_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no19_void_s_tank_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no7_void_p_tank_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no7_void_s_tank_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no10_void_p_tank_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no10_void_s_tank_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.cofferdam_p_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.cofferdam_s_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no11_void_p_tank_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);


	ix++;
	iloop = 0;
	one.no11_void_s_tank_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no12_void_p_tank_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no12_void_s_tank_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no20_void_p_tank_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no20_void_s_tank_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no14c_void_p_tank_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no14c_void_s_tank_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
    one.bilge_tank_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.lube_oil_tank_low_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
    one.fuel_oil_store_tank_9_p_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
    one.fuel_oil_store_tank_16_p_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.fuel_oil_store_tank_9_s_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
    one.fuel_oil_store_tank_16_s_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no13_void_p_tank_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.no13_void_s_tank_bilge_high_level_alarm = PLC1_VALUE_DI(m_v_ios,ix,iloop++);
	one.pump_room_c_bilge_high_level_alarm_2 = PLC1_VALUE_DI(m_v_ios,ix,iloop++);


	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


	one.device_offline = 0;
	bq_level_switch.put(one);
	FILELOG_level_switch(one);
}

void mainService::gen_data_gas_detection_system2()
{
	int ix,iloop;
	gas_detection_system one;
	memset(&one,0,sizeof(one));
    ix = 525;
	iloop = 11;
	
	one.co2_power_failure_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.co2_pre_release_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.co2_release_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.co2_leakage_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	one.device_offline = 0;

	bq_gas_detection_system.put(one);
	FILELOG_gas_detection_system(one);
}

void mainService::gen_data_miscellaneous2()
{
	miscellaneous_store& m_store = m_client_miscellaneous.m_data_store;
	//////////////////////////////////////////////////////////////////////////
	miscellaneous one;
	int ix = 525; 
	int iloop = 15;
    
    one.no1_watertight_door_common_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
    one.no2_watertight_door_common_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.no3_watertight_door_common_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.no4_watertight_door_common_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.fuel_oil_purifier_running = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.fuel_oil_purifier_abnormal_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.oily_water_separator_oil_content_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.oily_water_separator_common_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.ele_heating_calorifier_heater_running = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.ele_heating_calorifier_hot_water_circulating_pump_running = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.ele_heating_calorifier_high_temp_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.fresh_water_maker_common_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.fifi_system_common_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.sw_fw_fress_set_common_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.miscellaneousbffdc0c37da000f4 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.electrical_valve_remote_control_system_common_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.a_c_common_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
    one.provision_refrigeration_system_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.freezer_room_high_temp_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.chiller_room_high_temp_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.deck_machine_hpu_packdeck_machine_hpu_pack_low_oil_level_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.miscellaneous49510c947aae3742 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.deck_machine_hpu_packdeck_machine_hpu_pack_high_oil_temp_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.miscellaneousb8c79c2fcc0eddd2 = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.mud_agitator_system_common_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.bnwas_system_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.no1_gyro_ac220v_power_failure_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.no2_gyro_ac220v_power_failure_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.no3_gyro_ac220v_power_failure_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.autopilot_system_failure_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.gyro_compass_distribution_unit_system_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.gyro_compass_distribution_unit_gyro1_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.gyro_compass_distribution_unit_gyro2_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
    one.gyro_compass_distribution_unit_gyro3_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.gyro_compass_distribution_unit_tmc_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.gyro_compass_distribution_unit_can_bus_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.gyro_compass_distribution_unit_sensor_diff_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.gyro_compass_distribution_unit_heading_monitor_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.watertight_door_or_hatch_cover_power_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.fire_take_place_signal = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.fire_alarm_system_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.from_pa_system_power_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.from_ga_system_power_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.general_service_air_compressor_common_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.alarm_column_machinery_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.alarm_column_tank_level_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.alarm_column_relay_box_power_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.engine_call_system_power_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.mian_propulsion_order_telegraph_p_s_power_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	ix++;
	iloop = 0;
    one.refri_call_main_unit_power_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.hospital_call_main_unit_power_fault = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.watermist_main_control_boxecr_system_failure = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.ams_ups1_common_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.ams_ups2_common_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.control_box_loss_of_power = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.purifier_motor_overload = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.purifier_outlet_pressure_high = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.no1_gyro_dc24v_power_failure_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.no2_gyro_dc24v_power_failure_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.no3_gyro_dc24v_power_failure_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.autopilot_off_heading_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);
    one.watertight_door_or_hatch_cover_open_alarm = PLC1_VALUE_DI(m_v_plc,ix,iloop++);

	 fire_alarm_trige = one.fire_take_place_signal;

    //>>>>>>>>>>>>>>>>>>>>>>>>>>
	m_vecTmpMachinery.push_back (one.bnwas_system_failure);
	m_vecTmpMachinery.push_back (one.gyro_compass_distribution_unit_system_failure);
	m_vecTmpMachinery.push_back (one.gyro_compass_distribution_unit_gyro1_failure);
	m_vecTmpMachinery.push_back (one.gyro_compass_distribution_unit_gyro2_failure);
	m_vecTmpMachinery.push_back (one.gyro_compass_distribution_unit_gyro3_failure);
	m_vecTmpMachinery.push_back (one.gyro_compass_distribution_unit_tmc_failure);
	m_vecTmpMachinery.push_back (one.gyro_compass_distribution_unit_can_bus_failure);
	m_vecTmpMachinery.push_back (one.watertight_door_or_hatch_cover_power_failure);
	m_vecTmpMachinery.push_back (one.from_pa_system_power_fault);
	m_vecTmpMachinery.push_back (one.from_ga_system_power_fault);
	m_vecTmpMachinery.push_back (one.alarm_column_relay_box_power_fault);
	m_vecTmpMachinery.push_back (one.engine_call_system_power_fault);
	m_vecTmpMachinery.push_back (one.watermist_main_control_boxecr_system_failure);
    //>>>>>>>>>>>>>>>>>>>>>>>>>>

	one.device_offline = 0;
	
	bq_miscellaneous.put(one);
	FILELOG_miscellaneous(one);
}

void mainService::gen_data_co2_system()
{
	//////////////////////////////////////////////////////////////////////////
	co2_system one;
	int ix = 7; 
	int iloop = 10;
    
    
    
	
    //>>>>>>>>>>>>>>>>>>>>>>>>>>

	one.device_offline = 0;
	
	bq_co2_system.put(one);
}

void mainService::gen_data_cooling_water_system2()
{
    cooling_water_system_store& m_store = m_client_cooling_water_system.m_data_store;
	////////////////////////////////////////////////////////////////////////////////

	cooling_water_system one;
	int ix = 48;
    one.no1_propulsion_fw_cooling_pump_outlet_temperature = PLC1_VALUE_EXCI_TEN(m_v_ios,ix);ix++;
	one.no2_propulsion_fw_cooling_pump_outlet_temperature = PLC1_VALUE_EXCI_TEN(m_v_ios,ix);ix++;
	one.no1_propulsion_fw_cooling_plant_inlet_temperature = PLC1_VALUE_EXCI_TEN(m_v_ios,ix);ix++;
	one.no2_propulsion_fw_cooling_plant_inlet_temperature = PLC1_VALUE_EXCI_TEN(m_v_ios,ix);ix++;
	one.no1_propulsion_motor_fw_cooling_outlet_temperature = PLC1_VALUE_EXCI_TEN(m_v_ios,ix);ix++;
	one.no1_propulsion_vfd_fw_cooling_outlet_temperature = PLC1_VALUE_EXCI_TEN(m_v_ios,ix);ix++;
	one.no2_propulsion_motor_fw_cooling_outlet_temperature = PLC1_VALUE_EXCI_TEN(m_v_ios,ix);ix++;
	one.no2_propulsion_vfd_fw_cooling_outlet_temperature = PLC1_VALUE_EXCI_TEN(m_v_ios,ix);ix++;
	one.no3_propulsion_motor_fw_cooling_outlet_temperature = PLC1_VALUE_EXCI_TEN(m_v_ios,ix);ix++;
	one.no3_propulsion_vfd_fw_cooling_outlet_temperature = PLC1_VALUE_EXCI_TEN(m_v_ios,ix);ix++;
	one.no4_propulsion_motor_fw_cooling_outlet_temperature = PLC1_VALUE_EXCI_TEN(m_v_ios,ix);ix++;
	one.no4_propulsion_vfd_fw_cooling_outlet_temperature = PLC1_VALUE_EXCI_TEN(m_v_ios,ix);ix++;
	one.no5_propulsion_motor_fw_cooling_outlet_temperature = PLC1_VALUE_EXCI_TEN(m_v_ios,ix);ix++;
	one.no5_propulsion_vfd_fw_cooling_outlet_temperature = PLC1_VALUE_EXCI_TEN(m_v_ios,ix);ix++;
	one.no6_propulsion_motor_fw_cooling_outlet_temperature = PLC1_VALUE_EXCI_TEN(m_v_ios,ix);ix++;
	one.no6_propulsion_vfd_fw_cooling_outlet_temperature = PLC1_VALUE_EXCI_TEN(m_v_ios,ix);ix++;

	ix = 75;
    one.main_sea_water_line_pressure_p = PLC1_VALUE_PRESS_1_2(m_store.main_sea_water_line_pressure_p,m_v_ios,ix);ix++;
    one.main_sea_water_line_pressure_s = PLC1_VALUE_PRESS_1_2(m_store.main_sea_water_line_pressure_s,m_v_ios,ix);ix++;
	one.no1_bilge_water_pump_inlet_pressure = PLC1_VALUE_PRESS_2_2(m_store.no1_bilge_water_pump_inlet_pressure,m_v_ios,ix);ix++;
	one.no1_bilge_water_pump_outlet_pressure = PLC1_VALUE_PRESS_3_2(m_store.no1_bilge_water_pump_outlet_pressure,m_v_ios,ix);ix++;
	one.no2_bilge_water_pump_inlet_pressure = PLC1_VALUE_PRESS_2_2(m_store.no2_bilge_water_pump_inlet_pressure,m_v_ios,ix);ix++;
	one.no2_bilge_water_pump_outlet_pressure = PLC1_VALUE_PRESS_3_2(m_store.no2_bilge_water_pump_outlet_pressure,m_v_ios,ix);ix++;
	one.no1_ballast_water_pump_inlet_pressure = PLC1_VALUE_PRESS_2_2(m_store.no1_ballast_water_pump_inlet_pressure,m_v_ios,ix);ix++;
	one.no2_ballast_water_pump_inlet_pressure = PLC1_VALUE_PRESS_2_2(m_store.no2_ballast_water_pump_inlet_pressure,m_v_ios,ix);ix++;
	one.no1_fire_water_pump_outlet_pressure = PLC1_VALUE_PRESS_1_2(m_store.no1_fire_water_pump_outlet_pressure,m_v_ios,ix);ix++;
	one.no2_fire_water_pump_outlet_pressure = PLC1_VALUE_PRESS_1_2(m_store.no2_fire_water_pump_outlet_pressure,m_v_ios,ix);ix++;
	one.jokcy_pump_outlet_pressure = PLC1_VALUE_PRESS_1_2(m_store.jokcy_pump_outlet_pressure,m_v_ios,ix);ix++;
	one.jokcy_pump_inlet_pressure = PLC1_VALUE_PRESS_2_2(m_store.jokcy_pump_inlet_pressure,m_v_ios,ix);ix++;
	one.fire_main_line_pressure_p = PLC1_VALUE_PRESS_1_2(m_store.fire_main_line_pressure_p,m_v_ios,ix);ix++;
	one.fire_main_line_pressure_s = PLC1_VALUE_PRESS_1_2(m_store.fire_main_line_pressure_s,m_v_ios,ix);ix++;
	one.no1_fresh_water_transfer_pump_outlet_pressure = PLC1_VALUE_PRESS_3_2(m_store.no1_fresh_water_transfer_pump_outlet_pressure,m_v_ios,ix);ix++;
	one.no2_fresh_water_transfer_pump_outlet_pressure = PLC1_VALUE_PRESS_3_2(m_store.no2_fresh_water_transfer_pump_outlet_pressure,m_v_ios,ix);ix++;
	one.no1_propulsion_sw_cooling_pump_outlet_pressure = PLC1_VALUE_PRESS_3_2(m_store.no1_propulsion_sw_cooling_pump_outlet_pressure,m_v_ios,ix);ix++;
	one.no2_propulsion_sw_cooling_pump_outlet_pressure = PLC1_VALUE_PRESS_3_2(m_store.no2_propulsion_sw_cooling_pump_outlet_pressure,m_v_ios,ix);ix++;
	one.no1_propulsion_fw_cooling_pump_outlet_pressure = PLC1_VALUE_PRESS_3_2(m_store.no1_propulsion_fw_cooling_pump_outlet_pressure,m_v_ios,ix);ix++;
	one.no2_propulsion_fw_cooling_pump_outlet_pressure = PLC1_VALUE_PRESS_3_2(m_store.no2_propulsion_fw_cooling_pump_outlet_pressure,m_v_ios,ix);ix++;

    ix = 100;
	one.no1_fire_water_pump_inlet_pressure = PLC1_VALUE_PRESS_2_2(m_store.no1_fire_water_pump_inlet_pressure,m_v_ios,ix);ix++;
	one.no2_fire_water_pump_inlet_pressure = PLC1_VALUE_PRESS_2_2(m_store.no2_fire_water_pump_inlet_pressure,m_v_ios,ix);ix++;
	//>>>>>>>>>>>>>>>>>>>>>>

	one.device_offline = 0;
	bq_cooling_water_system.put(one);
	FILELOG_cooling_water_system(one);


	m_store.no1_propulsion_sw_cooling_pump_outlet_pressure.la_bit = std::make_pair<uint16_t, uint16_t>(4,0);
	m_store.no2_propulsion_sw_cooling_pump_outlet_pressure.la_bit = std::make_pair<uint16_t, uint16_t>(4,1);
	m_store.no1_propulsion_fw_cooling_pump_outlet_pressure.la_bit = std::make_pair<uint16_t, uint16_t>(4,2);
	m_store.no2_propulsion_fw_cooling_pump_outlet_pressure.la_bit = std::make_pair<uint16_t, uint16_t>(4,3);
}

void mainService::gen_data_buzzer_and_extend_panel2()
{
	while (1)
	{
		if (!m_mb_503)
		{
			m_mb_503 = new modbus(SAU_IP1, SIEMENS_PORT_503);
			m_mb_503->modbus_set_slave_id(1);
		}

		if (!m_mb_503->is_connected())
		{
			if (!m_mb_503->modbus_connect())
			{
				sleep(3);
				continue;
			}
		}

		while (1)
		{
			if (m_mb_503 && m_mb_503->is_connected() && m_503_mux)
			{
				uint16_t status[15] = {0};
				extend_alarm_panel ex;
				buzzer bz;
				{
					m_503_mux->lock();
					if (m_mb_503->modbus_read_holding_registers(3129, 7, status))
					{
						m_mb_503->modbus_close();
						m_503_mux->unlock();
                        std::this_thread::sleep_for(std::chrono::milliseconds(300));
						break;
					}
					else
					{
						m_503_mux->unlock();	
					}
					

					m_buzzer_status = status[0];
					m_extend_alarm_panel_status = status[5];

                    bool machinery_calling = (m_buzzer_status >> 10) & 0b1;
			        bool level_calling = (m_buzzer_status >> 11) & 0b1;
			        bool mud_calling = (m_buzzer_status >> 12) & 0b1;
					m_mud_handler.SetCallStatus(mud_calling);
					m_level_handler.SetCallStatus(level_calling);
					m_machinery_handler.SetCallStatus(machinery_calling);
					m_mud_handler.CheckCall();
					m_level_handler.CheckCall();
					//m_machinery_handler.CheckCall();
				}

				for (int i = 0; i < 3; i++)
				{
					bz.no = i;
					bz.status = (status[0] >> i) & 1 ? 2 : 1;
					bz.mute = (status[2] >> (i * 2)) & 1;
					bz.ack = (status[2] >> (i * 2 + 1)) & 1;

					bz.device_offline = 0;
					bq_buzzer[i].put(bz);
					FILELOG_buzzer(bz);
					printf("buzeer [%d] mute:%d,ack:%d,status:%d\n", bz.no, bz.mute, bz.ack, bz.status);
				}

				for (int i = 0; i < 7; i++)
				{
					ex.no = i;
					if (1 == ((status[5] >> i) & 1)) // 掉线
					{
						ex.status = 0;
						ex.device_offline = 1;

					}
					else
					{
						ex.status = ((status[0] >> (i + 3)) & 1) ? 2 : 1;  //buzzer
						ex.mute_or_ack = (status[2] >> (i + 6)) & 1;
						ex.device_offline = 0;

					}

					bq_extend_alarm_panel[i].put(ex);

                    if(1 == ex.mute_or_ack)
					{
                        m_client_extend_alarm_panel[i].bq_call_status.put(std::make_pair(0, 0));
					}

					FILELOG_extend_alarm_panel(ex);
					printf("extend [%d] status:%d offline:%d\n", ex.no, ex.status, ex.device_offline);
				}

				std::this_thread::sleep_for(std::chrono::milliseconds(500));
			}
		}
	}
}

void mainService::write_buzzer2(int i)
{
	while (1)
	{
		if (m_mb_506_sau1 && m_mb_506_sau1->is_connected() && m_506_sau1_mux)
		{
				char status = m_client_buzzer[i].bq_oper_status.take();
				

				sgo_log_stderr(0, "buzzer[%d] called status:%d.[0:silence,1:call,m_buzzer_status:%d]", i, status, m_buzzer_status);


                if(0 == i)
				{
					if(0 == status)
					{
                        m_mud_handler.SetAlarmPendingOn();
						m_level_handler.SetAlarmPendingOn();
						m_machinery_handler.SetAlarmPendingOn();
					}
				}


				if (0 == status) // silence
				{
					if (0 == i) // ecc silence extend_alarm_panel
					{
						// fire alarm
						m_pc_call = 0;
						{
							uint16_t v[7] = {0, 0, 0, 0, 0, 0, 0};
							std::lock_guard<std::mutex> lock(*m_506_sau1_mux);
							m_mb_506_sau1->modbus_write_registers(3035, 7, v); // 置0
						}
						std::this_thread::sleep_for(std::chrono::milliseconds(100));
					}

					if (0 == ((m_buzzer_status >> i) & 1)) // already silence
					{
						sgo_log_stderr(0, "buzzer[%d] already silence.", i);
					}
					else
					{
						sgo_log_stderr(0, "buzzzer[%d] starting to silence.", i);
						// {
						// 	std::lock_guard<std::mutex> lock(*m_506_sau1_mux);
						// 	int res = m_mb_506_sau1->modbus_write_register(3000 + i, 1); // 置0
						// }

						//std::this_thread::sleep_for(std::chrono::milliseconds(100));

						{
							std::lock_guard<std::mutex> lock(*m_506_sau1_mux);
							m_mb_506_sau1->modbus_write_register(3000 + i, 0); // 置1
						}
					}
				}
				else // call
				{
                    //fire alarm
                    if(fire_alarm_trige && 0 == i)
					{

						{
                            uint16_t v[7]= {1,1,1,1,1,1,1};
						    std::lock_guard<std::mutex> lock(*m_506_sau1_mux);
						    m_mb_506_sau1->modbus_write_registers(3035, 7, v); // 置1
						}
						std::this_thread::sleep_for(std::chrono::milliseconds(100));
					}

					sgo_log_stderr(0, "m_buzzer_status:%d\n", m_buzzer_status);
					printf("m_buzzer_status:%d\n", m_buzzer_status);
					// 已经在响，无需再下发
					if (1 == ((m_buzzer_status >> i) & 1))
					{
					}
					else
					{
						// {
						// 	std::lock_guard<std::mutex> lock(*m_506_sau1_mux);
						// 	m_mb_506_sau1->modbus_write_register(3000 + i, 0);
						// }

						// std::this_thread::sleep_for(std::chrono::milliseconds(100));
						{
							std::lock_guard<std::mutex> lock(*m_506_sau1_mux);
							m_mb_506_sau1->modbus_write_register(3000 + i, 1);
						}
					}
				}
		}
		else
		{
			sgo_log_stderr(0, "modbus buzzer disconnect.");
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
		}
	}
}

// void mainService::write_bell()
// {
// 	while (1)
// 	{
// 		if (m_mb_506_sau1 && m_mb_506_sau1->is_connected() && m_506_sau1_mux)
// 		{
// 			bool machinery_calling = (m_buzzer_status >> 10) & 0b1;
// 			bool level_calling = (m_buzzer_status >> 11) & 0b1;
// 			bool mud_calling = (m_buzzer_status >> 12) & 0b1;


// 			if(m_u16_machinery_flag)
// 			{
//                 if(!machinery_calling)
// 				{
// 					{
// 						std::lock_guard<std::mutex> lock(*m_506_sau1_mux);
// 						m_mb_506_sau1->modbus_write_register(3010, 1); // 置1
// 						sgo_log_stderr(0,"Write to machinery register 3010: value = 1");
// 					}
// 				}
// 			}
// 			else
// 			{
// 				if(machinery_calling)
// 				{
// 					{
// 						std::lock_guard<std::mutex> lock(*m_506_sau1_mux);
// 						m_mb_506_sau1->modbus_write_register(3010, 0); // 置0
// 						sgo_log_stderr(0,"Write to machinery register 3010: value = 0");
// 					}
// 				}
// 			}

// 			if(m_u16_level_flag)
// 			{
//                 if(!level_calling)
// 				{
// 					{
// 						std::lock_guard<std::mutex> lock(*m_506_sau1_mux);
// 						m_mb_506_sau1->modbus_write_register(3011, 1); // 置1
// 						sgo_log_stderr(0,"Write to level register 3011: value = 1");
// 					}
// 				}
// 			}
// 			else
// 			{
// 				if(level_calling)
// 				{
// 					{
// 						std::lock_guard<std::mutex> lock(*m_506_sau1_mux);
// 						m_mb_506_sau1->modbus_write_register(3011, 0); // 置0
// 						sgo_log_stderr(0,"Write to level register 3011: value = 0");
// 					}
// 				}
// 			}

// 			if(m_u16_mud_flag)
// 			{
//                 if(!mud_calling)
// 				{
// 					{
// 						std::lock_guard<std::mutex> lock(*m_506_sau1_mux);
// 						m_mb_506_sau1->modbus_write_register(3012, 1); // 置1
// 						sgo_log_stderr(0,"Write to mud register 3012: value = 1");
// 					}
// 				}
// 			}
// 			else
// 			{
// 				if(mud_calling)
// 				{
// 					{
// 						std::lock_guard<std::mutex> lock(*m_506_sau1_mux);
// 						m_mb_506_sau1->modbus_write_register(3012, 0); // 置0
// 						sgo_log_stderr(0,"Write to mud register 3012: value = 0");
// 					}
// 				}
// 			}

// 		}



// 			std::this_thread::sleep_for(std::chrono::milliseconds(500));

// 	}
// }


void mainService::write_duty()
{
	while(1)
	{
		if (m_mb_506_sau1 && m_mb_506_sau1->is_connected() && m_506_sau1_mux)
		{

			uint16_t w_data[17] = {0};

            //sau2 1
			w_data[0]= (m_sau_status >> 1) & 0b1;

			//duty 7
			for(int i = 0; i < 7; i++)
			{
                w_data[1 + i] = (m_duty >> i) & 0b1;
			}

			//extend_alarm_panel 7
			for(int i = 0; i < 7; i++)
			{
                w_data[8 + i] = (m_extend_alarm_panel_status >> i) & 0b1;
			}

			//pc 2
			w_data[15] = m_pc_status & 0b1;
		    w_data[16] = (m_pc_status >> 1) & 0b1;

			{
				std::lock_guard<std::mutex> lock(*m_506_sau1_mux);
				m_mb_506_sau1->modbus_write_registers(3018, 17, w_data);
			}
			
			std::this_thread::sleep_for(std::chrono::milliseconds(300));

			//TBD
			{
				// std::lock_guard<std::mutex> lock(*m_506_sau1_mux);
				// m_mb_506_sau1->modbus_write_register(1021, m_pc_call);
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(300));
		}
	}

}

void mainService::call_extend_alarm_panel2(int i)
{
    while (1)
	{
		if (m_mb_506_sau1 && m_mb_506_sau1->is_connected() && m_506_sau1_mux)
		{
			auto status = m_client_extend_alarm_panel[i].bq_call_status.take();
			sgo_log_stderr(0, "extend_alarm_panel[%d] called status:%d.[0:silence,1:call,m_buzzer_status:%d]", i, status, m_buzzer_status);

			if (0 == (status.second)) // silence
			{
                //fire alarm
                {
					std::lock_guard<std::mutex> lock(*m_506_sau1_mux);
					m_mb_506_sau1->modbus_write_register(3035 + i, 0); // 置0
				}

                std::this_thread::sleep_for(std::chrono::milliseconds(300));

				if (0 == ((m_buzzer_status >> (i + 3)) & 1)) // already silence
				{
					sgo_log_stderr(0, "extend_alarm_panel[%d] already silence.", i);
					continue;
				}
				else
				{
				}

				{
					std::lock_guard<std::mutex> lock(*m_506_sau1_mux);
					m_mb_506_sau1->modbus_write_register(3003 + i, 0); // 置1
				}
			}
			else // call
			{
				//TBD
				if(status.first)
				{
                    m_pc_call |= (1 << i);
				}
				
				sgo_log_stderr(0, "extend_alarm_panel:%d\n", m_buzzer_status);
				printf("extend_alarm_panel:%d\n", m_buzzer_status);
				// 已经在响，无需再下发
				if (1 == ((m_buzzer_status >> (i + 3)) & 1))
				{
					continue;
				}


				{
					std::lock_guard<std::mutex> lock(*m_506_sau1_mux);
					m_mb_506_sau1->modbus_write_register(3003 + i, 1);
				}
			}


		}
		else
		{
			sgo_log_stderr(0, "modbus buzzer disconnect.");
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
		}
	}
}

