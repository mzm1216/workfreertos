#ifndef _KB_DEV_H
#define _KB_DEV_H

#ifdef  KBDEV_GLOBALS
#define kb_dev_ext
#else
#define kb_dev_ext  extern
#endif

#include "board.h"
#include "config.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h" 

#define STATE_DISABLE	0
#define STATE_ENABLE	1

#define ICON_DARK		0	//暗图标
#define ICON_BRIGHT		1	//亮图标

#define CTR_CH_STATE_MAIN		0	//主控
#define CTR_CH_STATE_IDLE		1	//空闲
#define CTR_CH_STATE_BUSY		2	//busy
typedef struct 
{
	uint8_t heart_state:1;		//心跳状态，0不显示，1，显示
	uint8_t net_state:1;		//网络状态，0 暗图标，1亮图标
	uint8_t msc_state:1;		//交换中心状态，0暗图标，1，亮图标
	uint8_t dbs_state:1;		//数据库状态，0暗图标，1亮图标
	uint8_t main_ctr_state:1;	//主控状态，0，暗图标，1亮图标
	uint8_t main_state:9;		//备用

	uint8_t ch_state_1;			//通道状态1--Cch/idle/busy
	uint8_t ch_state_2;			//通道状态1--Cch/idle/busy
	
	uint16_t sys_id;			//si系统标识---B1F1
	
	uint16_t call_timer;		//通话计时器---
	
}DIS_STATE_T;


typedef struct
{
	uint16_t temperature_value; //温度值
	uint16_t rssi_value;		//场强值
	
		
}SYSTEM_PARAM_T;

typedef struct
{
	uint8_t work_mode;		//工作模式:0--DMR,1--PDT,2--MPT,3--常规   用于常住界面显示区域2的 文字显示
	uint8_t work_mode_1;	//工作模式:DMR:0-中继，1-常规? ;PDT:0-集群，1-,MPT---,常规  用于常驻界面显示区域2的  文字显示
	uint16_t ch_no;			//信道号
	uint32_t tx_freq;		//发射频率
	uint32_t rx_freq;		//接收频率
		
}COMMUNICATE_PARAM_T;


//显示板，按键板 结构体；
typedef struct
{
	DIS_STATE_T 		state_param;		//显示的状态 信息参数
	SYSTEM_PARAM_T 		system_param;		//系统参数
	COMMUNICATE_PARAM_T communicate_param;	//通讯参数
}DISPLAY_KB_T,*DISPLAY_KB_P;



#endif
