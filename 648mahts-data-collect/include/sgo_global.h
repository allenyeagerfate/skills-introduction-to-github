#ifndef __SGO_GBLDEF_H__
#define __SGO_GBLDEF_H__

const int PRINT_INTERVAL = 300;


//一些比较通用的定义放在这里，比如typedef定义
//一些全局变量的外部声明也放在这里

//类型定义----------------

//结构定义
typedef struct _CConfItem
{
	char ItemName[50];
	char ItemContent[500];
}CConfItem,*LPCConfItem;

//和运行日志相关 
typedef struct
{
	int    log_level;   //日志级别 或者日志类型，ngx_macro.h里分0-8共9个级别
	int    fd;          //日志文件描述符
}sgo_log_t;



extern sgo_log_t     sgo_log;


#endif
