#ifndef __TRCCOM_H_
#define __TRCCOM_H_

#ifdef  TRCCOM_GLOBALS
#define trccom_ext
#else
#define trccom_ext  extern
#endif

#include "ring_buffer.h"
#include "lpc_types.h"
#include "uart.h"

#define MASK_PRINTF 0// ���Ƶ�����Ϣ�����1λ�����0�����
//extern RINGBUFF_T Rxring[], Txring[];
//extern RINGBUFF_T Rxring[], Txring[];

#define TRC_COM_MAX_TX_NUM		UART_SRB_SIZE
#define TRC_COM_MAX_RX_NUM		UART_RRB_SIZE
#define APP_READ_NUM			TRC_COM_MAX_RX_NUM/2
/**
 * @brief Ring buffer structure
 */
#define MAX_COM_PORT 1// һ��2�����ڶ˿�
#define STX		0x1
#define ETX 	0x4
#define ELP 	0x10
typedef struct{
	uint32_t flag;//bit0-31 ������Ч���
	uint32_t delay0;//DAC��·��ʱ ��λus
	uint32_t ad_3ch;	//��ȡADC��ͨ��ָʾ


}RTU_FOR_TEST_DATA;
typedef  struct 
{
	RINGBUFF_T	rx_frame_buf;				//���ڽ��ն���---Э������
	RINGBUFF_T*	rx_driver_buf_ptr;			//���ڽ��ն���--�ײ�����
	uint8 		rxdata[TRC_COM_MAX_RX_NUM];//���ջ�����
	uint8 		start_flg;					//Э�鿪ʼ
	uint8 		end_flg;					//����Э�����
	uint8 		escape_flg;					//ת���ַ���־
	//uint8 		txdata[TRC_COM_MAX_TX_NUM];
	//uint32 		tx_len;
	
	xQueueHandle mutex_sem_read_ok;			//����ringbuf�� �������
	xQueueHandle mutex_sem_write_ok;		//����ringbuf��

	
	RINGBUFF_T	tx_ringbuf;//���ڽ��ն���
	
	RINGBUFF_T*	tx_driver;//���ڽ��ն���

	
	xQueueHandle driver_sem_receive_data;//����ringbuf�� �������
	RTU_FOR_TEST_DATA test_data;

	
} TRC_COM_T,*TRC_COM_P;

/*������ع��ܵĽṹ��*/
typedef struct 
{
	uint8 		lab;//��ǩ
	uint8 		seq;//֡���	
	uint8 		cmd;//����
	uint8 		txd_buf[512];
	uint16		tx_len;//�������ݳ���

}VTR_DEV_TX_FUN;

typedef struct
{
	uint8 						ch;									//ͨ������ 0 pc 1 chc
	VTR_DEV_TX_FUN 				txd_fun;							//������ع���
	uint8 						rxd_buf[TRC_COM_MAX_RX_NUM];		//Э��֡������


}VTR_PROTOCOL_DATA;


trccom_ext TRC_COM_T	trc_com0;
trccom_ext TRC_COM_T	trc_com1;

trccom_ext TRC_COM_P trc_com_ptr0;
trccom_ext TRC_COM_P trc_com_ptr1;
/* Initialize ring buffer */
//uint8 vtr_accessoriestype;//ѡ��������0x00:ģ�ⳣ�棻0x01:MPT1327��0x02:PDT��0x03:VSC��0x04:GSM��0x05:ZNCG��
//#define   DEBUGOUT	rt_kprintf


#define RT_CONSOLEBUF_SIZE   256
#define RT_SPOTOCOL_SIZE   256+128

trccom_ext char rt_log_buf[RT_CONSOLEBUF_SIZE];
trccom_ext char Send_Potocol_buf[RT_SPOTOCOL_SIZE];

trccom_ext void rt_kprintf(uint32_t flg,const char *fmt, ...);

#endif /* __TRCCOM_H_ */

