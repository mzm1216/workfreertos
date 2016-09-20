/***********************************************************************************
 * 文 件 名   : uart.c
 * 负 责 人   : MZM
 * 创建日期   : 2016年3月25日
 * 文件描述   : 串口驱动，串口初始化、串口收发数据接口
 * 版权说明   : Copyright (c) 2008-2016   北京市万格数码通讯科技有限公司
 * 其    他   : 
 * 修改日志   : 
***********************************************************************************/

#define  UART3_GLOBALS

#include "chip.h"
#include "board.h"
#include "uart.h"
#include "string.h"
#include <stdarg.h>

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/




/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	UART interrupt handler using ring buffers
 * @return	Nothing
 */
void UARTx_IRQHandler(void)
{
	/* Want to handle any errors? Do it here. */

	/* Use default ring buffer handler. Override this with your own
	   code if you need more capability. */
	Chip_UART_IRQRBHandler(LPC_UARTX, &rxring, &txring);
}


/*****************************************************************************
 * 函 数 名  : uart_dev_init
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月25日
 * 函数功能  : 初始化串口，波特率115200,8N1
 * 输入参数  : void  无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void uart_dev_init(void)
{
	uint8_t key;
	int bytes;

	/* Setup UART for 115.2K8N1 */
	Chip_UART_Init(LPC_UARTX);
	Chip_UART_SetBaud(LPC_UARTX, 115200);
	Chip_UART_ConfigData(LPC_UARTX, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT |UART_LCR_PARITY_DIS));
	Chip_UART_SetupFIFOS(LPC_UARTX, (UART_FCR_FIFO_EN | UART_FCR_TRG_LEV2));
	Chip_UART_TXEnable(LPC_UARTX);

	/* Before using the ring buffers, initialize them using the ring
	   buffer init function */
	RingBuffer_Init(&rxring, rxbuff, 1, UART_RRB_SIZE);
	RingBuffer_Init(&txring, txbuff, 1, UART_SRB_SIZE);

	/* Reset and enable FIFOs, FIFO trigger level 3 (14 chars) */
	Chip_UART_SetupFIFOS(LPC_UARTX, (UART_FCR_FIFO_EN | UART_FCR_RX_RS |
							UART_FCR_TX_RS | UART_FCR_TRG_LEV2));

	/* Enable receive data and line status interrupt */
//	Chip_UART_IntEnable(LPC_UARTX, (UART_IER_RBRINT | UART_IER_RLSINT));
	Chip_UART_IntEnable(LPC_UARTX, (UART_IER_RBRINT ));

	/* preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(UARTx_IRQn, 1);
	NVIC_EnableIRQ(UARTx_IRQn);
}

/*****************************************************************************
 * 函 数 名  : uart_send_data
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月25日
 * 函数功能  : 向串口发送数据
 * 输入参数  : uint8_t *sendbuf  发送缓冲区
               uint32_t num      发送字数
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
uint8_t uart_send_data(uint8_t *sendbuf,uint32_t num)
{
	if (Chip_UART_SendRB(LPC_UARTX, &txring, (const uint8_t *) sendbuf, num) != 1) 
		{
//			Board_LED_Toggle(0);/* Toggle LED if the TX FIFO is full */
		}

}

/*****************************************************************************
 * 函 数 名  : uart_read_data
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月25日
 * 函数功能  : 从串口接收数据
 * 输入参数  : uint8_t *recvbuf  接收缓冲区
               uint32_t num      接收个数
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
uint32_t uart_read_data(uint8_t *recvbuf,uint32_t num)
{
	return Chip_UART_ReadRB(LPC_UARTX, &rxring, (void*) recvbuf, num);
}



