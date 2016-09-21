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
//				336M +4.8*N+73.35M +X :X =0 ��С������
//#define JJJHZRX (336000000 + (4800000)*4+73350000+0)
//70*4.8M =336M +73.35+X: X =0 ��С������  90*4.8---432M
//#define JJJHZRX ((4800000)*75+73350000+00)
// 91*4.8 M = 436.8M ----- 97*4.8M=465.6M
//#define JJJHZTX ( (4800000*97)+600000)
#define JJJHZTX ( 466000000)
	

#define SPI_TIMER_DELAY_US	5			//��·DAC�ķ���ʱ����λus,��ֵ����С��10

#define NEED_DELAY_0	5
#define NEED_DELAY_1	1

#define MAX_COM_PORT 1// һ��2�����ڶ˿�
#define STX		0x1
#define ETX 	0x4
#define ELP 	0x10


typedef struct
{
	uint32_t app_buff_A[NUM_SAMPLES_A][NUM_CHANNELS_A]; /* Memory to hold samples for SEQ_A */
	uint32_t app_buff_B[NUM_SAMPLES_B][NUM_CHANNELS_B]; /* Memory to hold samples for SEQ_B */
	uint8_t tm1_flag; //timer1 ��ʱ��־ 
	//	 uint8_t  app_buff_A_offset;
	uint32_t Cur_ticks;			//��ǰtick
	uint32_t Send_delay_timer0;	//���ͼ��0
	uint32_t Send_delay_timer1;	//���ͼ��1
}APP_ADC_DEAL_BUFF;

apptask_ext APP_ADC_DEAL_BUFF	app_adc_buf;

#define APP_AD_ARAAY_NUM		(2)		//��ǰ���飬��������
#define APP_AD_VALUE_NUM		(2048)	//��������
typedef struct
{
	uint16_t value[APP_AD_ARAAY_NUM][APP_AD_VALUE_NUM]; /*���� */
	uint16_t cur_araay_num;								//��ǰ����� 
	uint16_t cur_value_num;								//��ǰ��ֵ����
}APP_AD_VALUE_T;

typedef struct
{
	APP_AD_VALUE_T  adc_samples_araay;		//����	Ƶ������
	APP_AD_VALUE_T  DAC_B_CH_araay;		//DAC value
	APP_AD_VALUE_T  SPI_TX_MOD_araay;		//tx mod value
		
}APP_ADC_DAC_VALUE_ARAAY;
apptask_ext  uint32_t app_test_cpu;		//����cpuʹ����

//apptask_ext RTU_DEV_T 	rtu_dev;//rtu�豸
#endif
