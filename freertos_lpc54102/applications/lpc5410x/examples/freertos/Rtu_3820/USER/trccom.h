#ifndef __TRCCOM_H_
#define __TRCCOM_H_

#include "ring_buffer.h"

#include "lpc_types.h"
#include "config.h"
#include "rtudev.h"

#include "driver_uart.h"
#define DISABLE_DEBUG	0//关闭调试信息

#define MASK_PRINTF 0// 控制调试信息输出，1位输出。0不输出
//extern RINGBUFF_T Rxring[], Txring[];
//extern RINGBUFF_T Rxring[], Txring[];

#define TRC_COM_MAX_TX_NUM		RB_COUNT
#define TRC_COM_MAX_RX_NUM		RB_COUNT
#define APP_READ_NUM			TRC_COM_MAX_RX_NUM/2
/**
 * @brief Ring buffer structure
 */
#define MAX_COM_PORT 1// 一共2个串口端口
#define STX		0x1
#define ETX 	0x4
#define ELP 	0x10
typedef struct{
	uint32_t flag;		//bit0-31 代表有效与否
	uint32_t delay0;	//DAC两路延时 单位us
	uint32_t ad_3ch;	//读取ADC的通道指示
	uint32_t txfreq;	//发射频率


}RTU_FOR_TEST_DATA;
typedef  struct 
{
	RINGBUFF_T	rx_frame_buf;//串口接收队列---协议数据
	RINGBUFF_T*	rx_driver_buf_ptr;//串口接收队列--底层数据
	uint8 		rxdata[TRC_COM_MAX_RX_NUM];//接收缓冲区
	uint8 		start_flg;					//协议开始
	uint8 		end_flg;					//解析协议结束
	uint8 		escape_flg;					//转换字符标志
	//uint8 		txdata[TRC_COM_MAX_TX_NUM];
	//uint32 		tx_len;
	
	xQueueHandle mutex_sem_read_ok;//操作ringbuf的 读出完毕
	xQueueHandle mutex_sem_write_ok;//操作ringbuf的

	
	RINGBUFF_T	tx_ringbuf;//串口接收队列
	
	RINGBUFF_T*	tx_driver;//串口接收队列

	
	xQueueHandle driver_sem_receive_data;//操作ringbuf的 读出完毕
	RTU_FOR_TEST_DATA test_data;

	
} TRC_COM_T,*TRC_COM_P;


typedef struct
{
	uint8 						ch;									//通道类型 0 pc 1 chc
	VTR_DEV_TX_FUN 				txd_fun;							//发送相关功能
	uint8 						rxd_buf[TRC_COM_MAX_RX_NUM];		//协议帧缓冲区


}VTR_PROTOCOL_DATA;

extern void rt_kprintf(uint32 flg,const char *fmt, ...);


#endif /* __TRCCOM_H_ */

