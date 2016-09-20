#ifndef _APP_TASK_h
#define _APP_TASK_h

#ifdef  APPTASK_GLOBALS
#define apptask_ext
#else
#define apptask_ext  extern
#endif

#include "board.h"
#include "config.h"
#include "driver_adc.h"
#include "i2c_dev.h"
#include "i2c_dev.h"
#include "trccom.h"

//#define JJJHZRX (470000000+73350000)
#define JJJHZRX (400000000)
//				336M +4.8*N+73.35M +X :X =0 无小数本分
//#define JJJHZRX (336000000 + (4800000)*4+73350000+0)
//70*4.8M =336M +73.35+X: X =0 无小数本分  90*4.8---432M
//#define JJJHZRX ((4800000)*75+73350000+00)
// 91*4.8 M = 436.8M ----- 97*4.8M=465.6M
//#define JJJHZTX ( (4800000*97)+600000)
#define JJJHZTX ( 466000000)
	

#define SPI_TIMER_DELAY_US	5			//两路DAC的发送时间差，单位us,数值必须小于10

#define NEED_DELAY_0	5
#define NEED_DELAY_1	1

#define MAX_COM_PORT 1// 一共2个串口端口
#define STX		0x1
#define ETX 	0x4
#define ELP 	0x10


typedef struct
{
	uint32_t app_buff_A[NUM_SAMPLES_A][NUM_CHANNELS_A]; /* Memory to hold samples for SEQ_A */
	uint32_t app_buff_B[NUM_SAMPLES_B][NUM_CHANNELS_B]; /* Memory to hold samples for SEQ_B */
	uint8_t tm1_flag; //timer1 到时标志 
	//	 uint8_t  app_buff_A_offset;
	uint32_t Cur_ticks;			//当前tick
	uint32_t Send_delay_timer0;	//发送间隔0
	uint32_t Send_delay_timer1;	//发送间隔1
}APP_ADC_DEAL_BUFF;

apptask_ext APP_ADC_DEAL_BUFF	app_adc_buf;

#define APP_AD_ARAAY_NUM		(2)		//当前数组，备份数组
#define APP_AD_VALUE_NUM		(2048)	//采样个数
typedef struct
{
	uint16_t value[APP_AD_ARAAY_NUM][APP_AD_VALUE_NUM]; /*数组 */
	uint16_t cur_araay_num;								//当前数组号 
	uint16_t cur_value_num;								//当前数值个数
}APP_AD_VALUE_T;

typedef struct
{
	APP_AD_VALUE_T  adc_samples_araay;		//采样	频率数组
	APP_AD_VALUE_T  DAC_B_CH_araay;		//DAC value
	APP_AD_VALUE_T  SPI_TX_MOD_araay;		//tx mod value
		
}APP_ADC_DAC_VALUE_ARAAY;
apptask_ext  uint32_t app_test_cpu;		//测试cpu使用率

//apptask_ext RTU_DEV_T 	rtu_dev;//rtu设备
#endif
