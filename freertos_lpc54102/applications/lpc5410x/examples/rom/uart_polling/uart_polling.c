/*
 * @brief UART polling example using the ROM API
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

/** @defgroup UART_POLLING_5410X UART polling example using the ROM API
 * @ingroup EXAMPLES_ROM_5410X
 * @include "rom\uart_polling\readme.txt"
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

#define tmsg1(x) "Type " # x " chars to echo it back\r\n"
#define tmsg(x) tmsg1(x)
#define RX_SZ  16
#define msg   tmsg(RX_SZ)
#define rmsg  "UART received : "

#define DLY(x) if (1) {volatile uint32_t dl = (x); while (dl--) {}}

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/* Send data to UART: Blocking */
ErrorCode_t ROM_UART_SendBlock(UART_HANDLE_T hUART, const void *buffer, uint16_t size)
{
	ErrorCode_t ret = ROM_UART_Send(hUART, buffer, size);
	if (ret == LPC_OK) {
		ROM_UART_WaitTx(hUART);
	}
	return ret;
}

/* Receive data from UART: Blocking */
ErrorCode_t ROM_UART_ReceiveBlock(UART_HANDLE_T hUART, void *buffer, uint16_t size)
{
	ErrorCode_t ret = ROM_UART_Receive(hUART, buffer, size);
	if (ret == LPC_OK) {
		ROM_UART_WaitRx(hUART);
	}
	return ret;
}

char *uart_gets(char *buf, int sz)
{
	int i = 0;
	
	/* If incoming size is 1 or less, return: use get_char instead */
	if (sz < 2) {
		return NULL;
	}

	/* get a character from the ROM. */
	while (ROM_UART_ReceiveBlock(hUART, &buf[i], 1) == LPC_OK) {
		if ((buf[i] == '\n') || (buf[i] == '\r') || (i == sz)) {
			break;
		}
		i++;
	}

	/* If no characters were received, return 0 */
	if (i == 0) {
		return NULL;
	}

	/* Post end-of-string marker.
		Note: this operation over-writes the last character in the buffer */
	buf[i] = 0;
	return buf;
}

const char *uart_puts(const char *buf)
{
	if (ROM_UART_SendBlock(hUART, buf, strlen(buf)) == LPC_OK) {
		return buf;
	}
	return NULL;
}

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
	cfg.cfg = UART_CFG_8BIT;
	cfg.div = baud.div;	/* Use the calculated div value */
	cfg.ovr = baud.ovr;	/* Use oversampling rate from baud */
	cfg.res = UART_BIT_DLY(UART_BAUD_RATE);

	/* Configure the UART */
	ROM_UART_Configure(hUART, &cfg);
	return 0;
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	Main routine for I2C example
 * @return	Function should not exit
 */
int main(void)
{
	uint8_t rxbuf[RX_SZ + 4];
	/* Generic Initialization */
	SystemCoreClockUpdate();
	Board_Init();

	/* Initialize the PinMux and setup the memory for ROM driver */
	if (uartrom_init()) {
		return 1;
	}

	/* Configure the ADC */
	uartrom_config();

	// uartrom_regcb(); /* Register call-back functions */

	ROM_UART_SendBlock(hUART, msg, sizeof(msg) - 1);
	ROM_UART_ReceiveBlock(hUART, rxbuf, RX_SZ);
	rxbuf[RX_SZ] = '\r';
	rxbuf[RX_SZ + 1] = '\n';
	ROM_UART_SendBlock(hUART, rmsg, sizeof(rmsg) - 1);
	ROM_UART_SendBlock(hUART, rxbuf, RX_SZ + 2);

	/* Test BREAK (unsafe) */
	/* Below data might be ignored by receiver due to frame error */
	/* The two data bytes will be in shift register and the holding
	 * register respectively, by the same time BREAK will be inserted
	 * and hence the data will be lost since TX line is in BREAK state.
	 */
	ROM_UART_SendBlock(hUART, "OK", 2);
	ROM_UART_SetCtrl(hUART, UART_BREAK_ON);
	DLY(30000);
	ROM_UART_SetCtrl(hUART, UART_BREAK_OFF);
	/* Wait for line to idle */
	DLY(3000);

	/* Test BREAK function (Safe) */
	/* This will not have any problems
	 * Since the first byte of data already made it to the shift register
	 * it will be shifted out before BREAK is inserted on the TX line, the
	 * second byte of data will wait in Holding register till UART_TX_RESUME
	 * is invoked.
	 */
	ROM_UART_SendBlock(hUART, "YA", 2);
	ROM_UART_SetCtrl(hUART, UART_TX_PAUSE | UART_BREAK_ON);
	DLY(30000);
	ROM_UART_SetCtrl(hUART, UART_TX_RESUME | UART_BREAK_OFF);
	DLY(3000);

	ROM_UART_SendBlock(hUART, "\r\nBreak DONE\r\n", 14);

	uart_puts("Enter a string terminated by newline\r\n");
	if (uart_gets((char *) rxbuf, RX_SZ)) {
		uart_puts("DATA Received is: [");
		uart_puts((char *) rxbuf);
		uart_puts("]\r\n");
	}

	while (true) {
		__WFI();
	}
	return 0;
}
