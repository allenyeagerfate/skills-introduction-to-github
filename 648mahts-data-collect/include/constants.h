#ifndef CONSTANTS_H
#define CONSTANTS_H

//每次读取100个寄存器数值  ，warning可以每次取1000个bits 
//J1939_1_status	 warning
//51400-51537		 1900-2235
//52100-52633

//J1939_2_status	warning
//61400-61537		11900-12235
//62100-62633
const static short SGO_J1939_1_STATUS_START 	= 51400 - 1;
const static short SGO_J1939_1_STATUS_START_1	= 52100 - 1;
const static short SGO_J1939_1_WARN_START		= 1900 - 1;

const static short SGO_J1939_2_STATUS_START 	= 61400 - 1;
const static short SGO_J1939_2_STATUS_START_1	= 62100 - 1;
const static short SGO_J1939_2_WARN_START 		= 11900 - 1;

const static short SGO_FETCH_NUM_REG = 100; //03 holding register
const static short SGO_FETCH_NUM_INPUT = 400; //02 input status

const static short SGO_J1939_NUMS = 336;


#endif
