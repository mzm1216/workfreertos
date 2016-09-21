#ifndef __TRCCOM_H_
#define __TRCCOM_H_

#include "ring_buffer.h"

#include "lpc_types.h"
#include "config.h"
#include "rtudev.h"

#include "driver_uart.h"
#define DISABLE_DEBUG	0//�رյ�����Ϣ

#define MASK_PRINTF 0// ���Ƶ�����Ϣ�����1λ�����0�����
//extern RINGBUFF_T Rxring[], Txring[];
//extern RINGBUFF_T Rxring[], Txring[];

#define TRC_COM_MAX_TX_NUM		RB_COUNT
#define TRC_COM_MAX_RX_NUM		RB_COUNT
#define APP_READ_NUM			TRC_COM_MAX_RX_NUM/2
/**
 * @brief Ring buffer structure
 */
#define MAX_COM_PORT 1// һ��2�����ڶ˿�
#define STX		0x1
#define ETX 	0x4
#define ELP 	0x10
typedef struct{
	uint32_t flag;		//bit0-31 ������Ч���
	uint32_t delay0;	//DAC��·��ʱ ��λus
	uint32_t ad_3ch;	//��ȡADC��ͨ��ָʾ
	uint32_t txfreq;	//����Ƶ��


}RTU_FOR_TEST_DATA;
typedef  struct 
{
	RINGBUFF_T	rx_frame_buf;//���ڽ��ն���---Э������
	RINGBUFF_T*	rx_driver_buf_ptr;//���ڽ��ն���--�ײ�����
	uint8 		rxdata[TRC_COM_MAX_RX_NUM];//���ջ�����
	uint8 		start_flg;					//Э�鿪ʼ
	uint8 		end_flg;					//����Э�����
	uint8 		escape_flg;					//ת���ַ���־
	//uint8 		txdata[TRC_COM_MAX_TX_NUM];
	//uint32 		tx_len;
	
	xQueueHandle mutex_sem_read_ok;//����ringbuf�� �������
	xQueueHandle mutex_sem_write_ok;//����ringbuf��

	
	RINGBUFF_T	tx_ringbuf;//���ڽ��ն���
	
	RINGBUFF_T*	tx_driver;//���ڽ��ն���

	
	xQueueHandle driver_sem_receive_data;//����ringbuf�� �������
	RTU_FOR_TEST_DATA test_data;

	
} TRC_COM_T,*TRC_COM_P;


typedef struct
{
	uint8 						ch;									//ͨ������ 0 pc 1 chc
	VTR_DEV_TX_FUN 				txd_fun;							//������ع���
	uint8 						rxd_buf[TRC_COM_MAX_RX_NUM];		//Э��֡������


}VTR_PROTOCOL_DATA;

extern void rt_kprintf(uint32 flg,const char *fmt, ...);


#endif /* __TRCCOM_H_ */

