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

#include "board.h"
#include "romapi_uart.h"

/** @defgroup UART_INT_5410X UART interrupt example using the ROM API
 * @ingroup EXAMPLES_ROM_5410X
 * @include "rom\uart_int\readme.txt"
 */

/**
 * @}
 */

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
/* UART Driver context memory */
#define RAMBLOCK_H          60
static uint32_t  drv_mem[RAMBLOCK_H];

/* UART ROM Driver Handle */
static UART_HANDLE_T *hUART;

#define UART_BAUD_RATE     115200	/* Required UART Baud rate */
#define UART_BUAD_ERR      1/* Percentage of Error allowed in baud */

#define tmsg1(x) "Type chars to echo them back\r\n"
#define tmsg(x) tmsg1(x)
#define RX_SZ  16
#define msg   tmsg(RX_SZ)
#define rmsg  "UART received : "

volatile uint32_t tx_done, rx_done;

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

#define ABS(x) ((int) (x) < 0 ? -(x) : (x))
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
	cfg.cfg = UART_CFG_8BIT | UART_CFG_BRKRX;
	cfg.div = baud.div;	/* Use the calculated div value */
	cfg.ovr = baud.ovr;	/* Use oversampling rate from baud */
	cfg.res = UART_BIT_DLY(UART_BAUD_RATE);

	/* Configure the UART */
	ROM_UART_Configure(hUART, &cfg);
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);

	return 0;
}

/* UART ROM error handler */
static void uartrom_error(UART_HANDLE_T hUART, uint32_t err)
{
	switch (err) {
	case UART_ERROR_FRAME:
		/* No stop bit in uart frame; mismatched baud(?) or incorrect/short BREAK condition(?) */
		Board_LED_Set(0, 1);
		break;

	case UART_ERROR_PARITY:
		/* Parity error; mismatched baud(?) */
		while (1) {}

	case UART_ERROR_AUTOBAUD:
		/* Autobaud timeout error */
		while (1) {}

	case UART_ERROR_OVERRUN:
		/* Uart received character before ROM_UART_Receive() is called */
		Board_LED_Set(1, 1);
		break;

	case UART_ERROR_RXNOISE:
		/* Typically problem is with the baud rate and or Over sampling count */
		while (1) {}

	default:
		/* Control will never reach this */
		break;
	}
}

/* UART ROM event handler */
static void uartrom_event(UART_HANDLE_T hUART, uint32_t evt)
{
	switch (evt) {
	case UART_EVENT_BREAK:
		Board_LED_Set(2, 1);		/* TURN ON LED_2 when BREAK is received on RX line */
		break;

	case UART_EVENT_NOBREAK:
		Board_LED_Set(2, 0);		/* TURN OFF LED_2 when RX comes out of break */
		break;

	case UART_EVENT_TXIDLE:
		/* Can be used for flow control */
		/* This will be called when the TX shift register is done with
		   sending the last bit to uart line; event will be called only
		   after calling ROM_UART_SetCtrl(hUART, UART_TXIDLE_ON), event
		   can be turned off using ROM_UART_SetCtrl(hUART, UART_TXIDLE_OFF)
		 */
		break;

	case UART_EVENT_TXPAUSED:
		/* Event will happen after ROM_UART_SetCtrl(hUART, UART_TX_PAUSE) is
		   called. This event does not mean the the TX is idle, meaning, the
		   TX holding register might contain data (which is not loaded into
		   TX shift register anymore) and the the TX shift register is done
		   sending the data that it was sending when UART_TX_PAUSE was called.
		 */
		/* Can be used to implement flow control or safely send BREAK signal
		   without createing frame errors in the currently transmitted data
		 */
		break;

	case UART_EVENT_CTSHI:
		/* CTS line went from Low to High */
		/* Could be used for flow control or RS-485 implementations */
		break;

	case UART_EVENT_CTSLO:
		/* CTS line went from High to Low */
		/* Could be used for flow control or RS-485 implementations */
		break;

	default:
		while (1) {}	/* Control will never reach here */
	}
}

/* Call-back handler for error/event */
static void uartrom_xfer_errevt(UART_HANDLE_T hUART, UART_EVENT_T evt, void *arg)
{
	uint32_t val = (uint32_t) arg;
	if (evt == UART_EV_ERROR) {
		uartrom_error(hUART, val);
	}
	else if (evt == UART_EV_EVENT) {
		uartrom_event(hUART, val);
	}
	else {
		while (1) {}/* Control will never reach this */

	}
}

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

/* UART transfer start */
static void uartrom_xfer_start(UART_HANDLE_T hUART, UART_EVENT_T evt, void *arg)
{
	UART_DATA_T *dat = (UART_DATA_T *) arg;

	(void) *dat;
	switch (evt) {
	case UART_TX_START:
		/* Transmit of new buffer started */
		break;

	case UART_RX_START:
		/* Receive of data started; can be used to implement timer logic */
		break;

	default:
		while (1) {}	/* Control will never reach here */
	}
}

static void uartrom_rx_prog(UART_HANDLE_T hUART, UART_EVENT_T evt, void *arg)
{
	UART_DATA_T *dat = (UART_DATA_T *) arg;

	switch (evt) {
	case UART_RX_INPROG:
		break;

	case UART_RX_NOPROG:
		/* In case of frame error restart xfer */
		if (dat->state == UART_ST_ERRFRM) {
			dat->offset = 0;
			dat->state = UART_ST_BUSY;
		}
		else {
			/* If the received frame has errors don't stop just restart */
			ROM_UART_SetCtrl(hUART, UART_RX_STOP);
		}
		break;

	default:
		break;		/* Control should never reach here */
	}
}

/* Register call-backs */
static void uartrom_regcb(void)
{
	ROM_UART_RegisterCB(hUART, UART_CB_START, uartrom_xfer_start);	/* Start of transfer */
	ROM_UART_RegisterCB(hUART, UART_CB_DONE, uartrom_xfer_done);/* End of transfer */
	ROM_UART_RegisterCB(hUART, UART_CB_ERREVT, uartrom_xfer_errevt);/* Error/Event callbacks */
	ROM_UART_RegisterCB(hUART, UART_CB_RXPROG, uartrom_rx_prog);/* Receive progress callback */
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
 * 函数功能  : 串口0  初始化：中断收发模式
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

//	ROM_UART_Send(hUART, msg, sizeof(msg) - 1);
//	while (!tx_done) {
//		__WFI();
//	}
//	while (1) {
//		rx_done = 0;
//		ROM_UART_Receive(hUART, rxbuf, RX_SZ);
//		while (!rx_done) {
//			__WFI();
//		}
//		tx_done = 0;
//		ROM_UART_Send(hUART, rxbuf, rx_done);
//		while (!tx_done) {
//			__WFI();
//		}
//	}
	return ;
}

/**
 * @brief	Main routine for I2C example
 * @return	Function should not exit
 */
int main211(void)
{
//	uint8_t rxbuf[RX_SZ + 2];
//	/* Generic Initialization */
//	SystemCoreClockUpdate();
//	Board_Init();

//	/* Initialize the PinMux and setup the memory for ROM driver */
//	uartrom_init();

//	/* Configure the ADC */
//	uartrom_config();

//	uartrom_regcb();/* Register call-back functions */

//	ROM_UART_Send(hUART, msg, sizeof(msg) - 1);
//	while (!tx_done) {
//		__WFI();
//	}
//	while (1) {
//		rx_done = 0;
//		ROM_UART_Receive(hUART, rxbuf, RX_SZ);
//		while (!rx_done) {
//			__WFI();
//		}
//		tx_done = 0;
//		ROM_UART_Send(hUART, rxbuf, rx_done);
//		while (!tx_done) {
//			__WFI();
//		}
//	}
	return 0;
}

