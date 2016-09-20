/*
 * @brief UART interrupt example using the ROM API
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2014
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */
#define	 UART_GLOBALS

#include "board.h"
#include "romapi_uart.h"
#include "driver_uart.h"

/** @defgroup UART_INT_5410X UART interrupt example using the ROM API
 * @ingroup EXAMPLES_ROM_5410X
 * @include "rom\uart_int\readme.txt"
 */

/**
 * @}
 */



/*****************************************************************************
 * 函 数 名  : Rtu_Get_Uart0_Dev_handle
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月26日
 * 函数功能  : 换取串口0设备操作handle
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : UART_HANDLE_T
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
UART_HANDLE_T * Rtu_Get_Uart0_Dev_handle()
{
	return hUART;
}


/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

static void UART_PinMuxSetup(void)
{
#if defined(BOARD_NXP_LPCXPRESSO_54102)
	/* Setup UART TX Pin */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 0, (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_DIGITAL_EN));
	/* Setup UART RX Pin */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 1, (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_DIGITAL_EN));
	Chip_SYSCON_Enable_ASYNC_Syscon(true);	/* Enable Async APB */ 
	Chip_Clock_SetAsyncSysconClockDiv(1);	/* Set Async clock divider to 1 */
#else
#warning "No UART PIN/CLK setup for this example"
#endif
}

/* Initialize the UART ROM Driver */
static int uartrom_init(void)
{
	int sz;

	UART_PinMuxSetup();

	sz =  ROM_UART_GetMemSize();

	if (RAMBLOCK_H < (sz / 4)) {
		while (1) {}
	}

	hUART = ROM_UART_Init(drv_mem, LPC_USART0_BASE, 0);

	return 0;
}

/* Configure UART ROM Driver and pripheral */
static int uartrom_config(void)
{
	UART_CFG_T cfg;
	UART_BAUD_T baud;

	/* Set up baudrate parameters */
	baud.clk = Chip_Clock_GetAsyncSyscon_ClockRate();	/* Clock frequency */
	baud.baud = UART_BAUD_RATE;	/* Required baud rate */
	baud.ovr = 0;	/* Set the oversampling to the recommended rate */
	baud.mul = baud.div = 0;

	if (ROM_UART_CalBaud(&baud) != LPC_OK) {
		/* Unable to calculate the baud rate parameters */
		while (1) {}
	}

	/* Set fractional control register */
	Chip_SYSCON_SetUSARTFRGCtrl(baud.mul, 255);

	/* See if the calculated baud is < +/- UART_BUAD_ERR% of the required baud */
	if (ABS(baud.baud - UART_BAUD_RATE) > (UART_BAUD_RATE * UART_BUAD_ERR) / 100) {
		/* WARNING: Baud rate is has more than UART_BUAD_ERR percentage */
		/* Try to auto-detect the Oversampling rate by setting baud.ovr to 0 */
		while (1) {}
	}

	/* Configure the UART */
	cfg.cfg = UART_CFG_8BIT ;
	cfg.div = baud.div;	/* Use the calculated div value */
	cfg.ovr = baud.ovr;	/* Use oversampling rate from baud */
	cfg.res = UART_BIT_DLY(UART_BAUD_RATE);

	/* Configure the UART */
	ROM_UART_Configure(hUART, &cfg);
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);

	return 0;
}



/* UART ROM event handler */
//static void uartrom_event(UART_HANDLE_T hUART, uint32_t evt)
//{
//	switch (evt) {
//	case UART_EVENT_BREAK:
//		Board_LED_Set(2, 1);		/* TURN ON LED_2 when BREAK is received on RX line */
//		break;

//	case UART_EVENT_NOBREAK:
//		Board_LED_Set(2, 0);		/* TURN OFF LED_2 when RX comes out of break */
//		break;

//	case UART_EVENT_TXIDLE:
//		/* Can be used for flow control */
//		/* This will be called when the TX shift register is done with
//		   sending the last bit to uart line; event will be called only
//		   after calling ROM_UART_SetCtrl(hUART, UART_TXIDLE_ON), event
//		   can be turned off using ROM_UART_SetCtrl(hUART, UART_TXIDLE_OFF)
//		 */
//		break;

//	case UART_EVENT_TXPAUSED:
//		/* Event will happen after ROM_UART_SetCtrl(hUART, UART_TX_PAUSE) is
//		   called. This event does not mean the the TX is idle, meaning, the
//		   TX holding register might contain data (which is not loaded into
//		   TX shift register anymore) and the the TX shift register is done
//		   sending the data that it was sending when UART_TX_PAUSE was called.
//		 */
//		/* Can be used to implement flow control or safely send BREAK signal
//		   without createing frame errors in the currently transmitted data
//		 */
//		break;

//	case UART_EVENT_CTSHI:
//		/* CTS line went from Low to High */
//		/* Could be used for flow control or RS-485 implementations */
//		break;

//	case UART_EVENT_CTSLO:
//		/* CTS line went from High to Low */
//		/* Could be used for flow control or RS-485 implementations */
//		break;

//	default:
//		while (1) {}	/* Control will never reach here */
//	}
//}



/* UART Transfer done */
static void uartrom_xfer_done(UART_HANDLE_T hUART, UART_EVENT_T evt, void *arg)
{
	UART_DATA_T *dat = (UART_DATA_T *) arg;

	switch (evt) {
	case UART_TX_DONE:
		tx_done = 1;
		break;

	case UART_RX_DONE:
		rx_done = dat->count;
		break;

	default:
		while (1) {}	/* Control will never reach here */
	}
}

/* Register call-backs */
static void uartrom_regcb(void)
{
	ROM_UART_RegisterCB(hUART, UART_CB_DONE, uartrom_xfer_done);/* End of transfer */
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/
/**
 * @brief	UART interrupt handler
 */
void UART0_IRQHandler(void)
{
	ROM_UART_Handler(hUART);
}

/*****************************************************************************
 * 函 数 名  : RTU_Uart0Dev_Init
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月25日
 * 函数功能  : 串口0  初始化：中断收发模式,前提条件是调用之前执行了board_init()
 * 输入参数  : void  无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void  RTU_Uart0Dev_Init(void)
{
	/* Initialize the PinMux and setup the memory for ROM driver */
	uartrom_init();

	/* Configure the ADC */
	uartrom_config();

	uartrom_regcb();/* Register call-back functions */

	return ;
}


RTU_UART_DEV_T * RTU_Get_Uart_Dev()
{
	return &rtu_uart_dev;//返回spi0设备指针
}




















void test_uart()
{
	uint8_t txbuf[RX_SZ + 2];
	uint8_t rxbuf[RX_SZ + 2];
	uint8_t i;

	for(i=0;i<RX_SZ;i++)
	{
		txbuf[i]=i+'1';
		rxbuf[i]= 0x0;

	tx_done=0;
	ROM_UART_Send(hUART, &txbuf[i%RX_SZ], 1);
	while (!tx_done) {
			__WFI();
		}

	rx_done = 0;
		ROM_UART_Receive(hUART, &rxbuf[i%RX_SZ], 1);
		while (!rx_done) {
			__WFI();
		}
			}
		

}


