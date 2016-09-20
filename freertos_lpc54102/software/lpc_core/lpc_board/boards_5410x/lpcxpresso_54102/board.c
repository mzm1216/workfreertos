/*
 * @brief LPCXpresso LPC54102 board file
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
#include "retarget.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* Be careful that these pins are set to output now once Board_LED_Init() is
   called. */
static const uint8_t ledBits[] = {16,17};

/* UART ROM Driver Handle and context data area */
static UART_HANDLE_T hUART;
static uint32_t drv_mem[16];

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/* Clock rate on the CLKIN pin */
const uint32_t ExtClockIn = BOARD_EXTCLKINRATE;

/*****************************************************************************
 * Private functions
 ****************************************************************************/

#define ABS(x) ((int) (x) < 0 ? -(x) : (x))

/* Initialize the LEDs on the NXP LPC54000 LPCXpresso Board */
static void Board_LED_Init(void)
{
	int i;

	/* Pin muxing setup as part of board_sysinit */
	for (i = 0; i < sizeof(ledBits); i++) {
		Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, ledBits[i]);
		Chip_GPIO_SetPinState(LPC_GPIO, 1, ledBits[i], true);
	}
}

/* UART ROM RX call back */
static void  Board_UART_Done(UART_HANDLE_T hUART, UART_EVENT_T ev, void *arg)
{
	if (ev == UART_RX_DONE) {
		*(int *) ROM_UART_HANDLE_TOUDATA(hUART) = 1;
	}
}

/* Board Debug UART Initialisation function */
static void Board_UART_Init(void)
{
	int sz;
	UART_CFG_T cfg;
	UART_BAUD_T baud;
	static int cval;

	if (DEBUG_UART == LPC_USART0) {
		/* Enable peripheral clock to UART0 */
		Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_USART0);
	}

	/* Enable clock to Fractional divider */
	Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_FRG);

	/* UART0 pins are setup in board_sysinit.c */

	/* Test memory size */
	sz =  ROM_UART_GetMemSize();
	if (sz > sizeof(drv_mem)) {
		/* Not enough memory, return leaving UART handle as NULL */
	}

	/* Get handle to UART driver */
	hUART = ROM_UART_Init(drv_mem, (uint32_t) DEBUG_UART, &cval);

	/* Set up baudrate parameters */
	baud.clk = Chip_Clock_GetAsyncSyscon_ClockRate();	/* Clock frequency */
	baud.baud = DEBUGBAUDRATE;	/* Required baud rate */
	baud.ovr = 0;	/* Set the oversampling to the recommended rate */
	baud.mul = baud.div = 0;

	if (ROM_UART_CalBaud(&baud) != LPC_OK) {
		/* Unable to calculate the baud rate parameters */
		hUART = NULL;
		return;
	}

	/* Set fractional control register */
	Chip_SYSCON_SetUSARTFRGCtrl(baud.mul, 0xFF);

	/* Configure the UART */
	cfg.cfg = UART_CFG_8BIT;
	cfg.div = baud.div;	/* Use the calculated div value */
	cfg.ovr = baud.ovr;	/* Use oversampling rate from baud */
	cfg.res = UART_BIT_DLY(DEBUGBAUDRATE);

	/* Configure the UART */
	ROM_UART_Configure(hUART, &cfg);
	ROM_UART_RegisterCB(hUART, UART_CB_DONE, Board_UART_Done);
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Set the LED to the state of "On" */
void Board_LED_Set(uint8_t LEDNumber, bool On)
{
	if (LEDNumber < sizeof(ledBits)) {
		Chip_GPIO_SetPinState(LPC_GPIO, 1, ledBits[LEDNumber], (bool) !On);
	}
}

/* Return the state of LEDNumber */
bool Board_LED_Test(uint8_t LEDNumber)
{
	if (LEDNumber < sizeof(ledBits)) {
		return (bool) !Chip_GPIO_GetPinState(LPC_GPIO, 0, ledBits[LEDNumber]);
	}

	return false;
}

/* Toggles the current state of a board LED */
void Board_LED_Toggle(uint8_t LEDNumber)
{
	if (LEDNumber < sizeof(ledBits)) {
		Chip_GPIO_SetPinToggle(LPC_GPIO, 1, ledBits[LEDNumber]);
	}
}

/* Sends a character on the UART */
void Board_UARTPutChar(char ch)
{
#if defined(DEBUG_UART)
	if (hUART != NULL) {
		ROM_UART_Send(hUART, &ch, 1);
		ROM_UART_WaitTx(hUART);
	}
#endif
}

/* Gets a character from the UART, returns EOF if no character is ready */
int Board_UARTGetChar(void)
{
#if defined(DEBUG_UART)
	static uint8_t uart_rx, q;
	int *cv = ROM_UART_HANDLE_TOUDATA(hUART);

	if (!q) {	/* Queue a buffer for receive */
		q = ROM_UART_Receive(hUART, &uart_rx, 1) == LPC_OK;
	}
	ROM_UART_Handler(hUART);
	if (*cv) {
		q = *cv = 0;
		return (int) uart_rx;
	}
#endif
	return EOF;
}

/* Outputs a string on the debug UART */
void Board_UARTPutSTR(char *str)
{
#if defined(DEBUG_UART)
	ROM_UART_Send(hUART, str, strlen(str));
	ROM_UART_WaitTx(hUART);
#endif
}

/* Initialize debug output via UART for board */
void Board_Debug_Init(void)
{
#if defined(DEBUG_UART)
	Board_UART_Init();
#endif
}

/* Set up and initialize all required blocks and functions related to the
   board hardware */
void Board_Init(void)
{
	/* INMUX and IOCON are used by many apps, enable both INMUX and IOCON clock bits here. */
	Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_INPUTMUX);
	Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_IOCON);

	/* Enable peripheral clock to UART0 */
	Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_USART0);

	
	/* Enable clock to Fractional divider */
	Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_FRG);

	/* Sets up DEBUG UART */
//	DEBUGINIT();

	/* Initialize GPIO */
	Chip_GPIO_Init(LPC_GPIO);

	/* Initialize the LEDs. Be careful with below routine, once it's called some of the I/O will be set to output. */
	Board_LED_Init();
}
