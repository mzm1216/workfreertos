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

#define ICON_DARK		0	//��ͼ��
#define ICON_BRIGHT		1	//��ͼ��

#define CTR_CH_STATE_MAIN		0	//����
#define CTR_CH_STATE_IDLE		1	//����
#define CTR_CH_STATE_BUSY		2	//busy
typedef struct 
{
	uint8_t heart_state:1;		//����״̬��0����ʾ��1����ʾ
	uint8_t net_state:1;		//����״̬��0 ��ͼ�꣬1��ͼ��
	uint8_t msc_state:1;		//��������״̬��0��ͼ�꣬1����ͼ��
	uint8_t dbs_state:1;		//���ݿ�״̬��0��ͼ�꣬1��ͼ��
	uint8_t main_ctr_state:1;	//����״̬��0����ͼ�꣬1��ͼ��
	uint8_t main_state:9;		//����

	uint8_t ch_state_1;			//ͨ��״̬1--Cch/idle/busy
	uint8_t ch_state_2;			//ͨ��״̬1--Cch/idle/busy
	
	uint16_t sys_id;			//siϵͳ��ʶ---B1F1
	
	uint16_t call_timer;		//ͨ����ʱ��---
	
}DIS_STATE_T;


typedef struct
{
	uint16_t temperature_value; //�¶�ֵ
	uint16_t rssi_value;		//��ǿֵ
	
		
}SYSTEM_PARAM_T;

typedef struct
{
	uint8_t work_mode;		//����ģʽ:0--DMR,1--PDT,2--MPT,3--����   ���ڳ�ס������ʾ����2�� ������ʾ
	uint8_t work_mode_1;	//����ģʽ:DMR:0-�м̣�1-����? ;PDT:0-��Ⱥ��1-,MPT---,����  ���ڳ�פ������ʾ����2��  ������ʾ
	uint16_t ch_no;			//�ŵ���
	uint32_t tx_freq;		//����Ƶ��
	uint32_t rx_freq;		//����Ƶ��
		
}COMMUNICATE_PARAM_T;


//��ʾ�壬������ �ṹ�壻
typedef struct
{
	DIS_STATE_T 		state_param;		//��ʾ��״̬ ��Ϣ����
	SYSTEM_PARAM_T 		system_param;		//ϵͳ����
	COMMUNICATE_PARAM_T communicate_param;	//ͨѶ����
}DISPLAY_KB_T,*DISPLAY_KB_P;



#endif
