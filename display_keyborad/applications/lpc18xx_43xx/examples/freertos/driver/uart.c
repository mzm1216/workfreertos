/***********************************************************************************
 * �� �� ��   : uart.c
 * �� �� ��   : MZM
 * ��������   : 2016��3��25��
 * �ļ�����   : �������������ڳ�ʼ���������շ����ݽӿ�
 * ��Ȩ˵��   : Copyright (c) 2008-2016   �������������ͨѶ�Ƽ����޹�˾
 * ��    ��   : 
 * �޸���־   : 
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
 * �� �� ��  : uart_dev_init
 * �� �� ��  : MZM
 * ��������  : 2016��3��25��
 * ��������  : ��ʼ�����ڣ�������115200,8N1
 * �������  : void  ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

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
 * �� �� ��  : uart_send_data
 * �� �� ��  : MZM
 * ��������  : 2016��3��25��
 * ��������  : �򴮿ڷ�������
 * �������  : uint8_t *sendbuf  ���ͻ�����
               uint32_t num      ��������
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
uint8_t uart_send_data(uint8_t *sendbuf,uint32_t num)
{
	if (Chip_UART_SendRB(LPC_UARTX, &txring, (const uint8_t *) sendbuf, num) != 1) 
		{
//			Board_LED_Toggle(0);/* Toggle LED if the TX FIFO is full */
		}

}

/*****************************************************************************
 * �� �� ��  : uart_read_data
 * �� �� ��  : MZM
 * ��������  : 2016��3��25��
 * ��������  : �Ӵ��ڽ�������
 * �������  : uint8_t *recvbuf  ���ջ�����
               uint32_t num      ���ո���
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
uint32_t uart_read_data(uint8_t *recvbuf,uint32_t num)
{
	return Chip_UART_ReadRB(LPC_UARTX, &rxring, (void*) recvbuf, num);
}



