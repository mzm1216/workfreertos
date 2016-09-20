/*
 * @brief UART interrupt example using the system FIFO
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
#include "ring_buffer.h"
#include "romapi_uart.h"

/** @defgroup UART_INTFIFO_5410X UART interrupt example using the system FIFO
 * @ingroup EXAMPLES_ROM_5410X
 * @include "rom\uart_int_sfifo\readme.txt"
 */

/**
 * @}
 */

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

typedef struct UART_ISR_STATISTICStag {
	uint32_t rx_rdy;			/* receive character ready */
	uint32_t tx_rdy;			/* transmitter ready (shift register empty) */
	uint32_t tx_idle;			/* transmitter idle (shift and transmit buffer empty) */
	uint32_t tx_dis;			/* transmitter disabled */
	uint32_t er_frame;			/* framing error */
	uint32_t er_parity;			/* parity error */
	uint32_t er_auto_baud;		/* auto baud error */
	uint32_t er_rx_noise;		/* receive line noise error */
	uint32_t er_overrun;		/* over-run error */
	uint32_t stat_cts;			/* CTS status change error */
	uint32_t stat_break;		/* BREAK received */
	uint32_t stat_start;		/* start bit detected */
} UART_ISR_STATISTICS;

/* UART statistics */
static UART_ISR_STATISTICS u_stat;

/* UART Driver context memory */
static uint32_t drv_mem[16];

/* UART ROM Driver Handle */
static UART_HANDLE_T hUART;

#define UART_ID             0			/* UART used for example */
#define UART_BAUD_RATE      115200		/* Required UART Baud rate (tested at 921600, 460800 and 115200) */
#define UART_BUAD_ERR       1			/* Percentage of Error allowed in baud */

/* ring buffer data */
#define RB_COUNT            128			/* Ring buffer count */
static  uint8_t rx_buff[RB_COUNT];		/* Receive ring buffer */
static  uint8_t tx_buff[RB_COUNT];		/* Transmit ring buffer */
static  uint8_t io_buff[RB_COUNT];		/* I/O buffer for interrupts */
static  uint8_t ap_buff[RB_COUNT];		/* I/O buffer for application */
static  RINGBUFF_T  rx_rb, tx_rb;		/* Ring buffer structures */

/*	Event flags */
#define EVENT_RX_TH         0x01		/* set when rx fifo threshold interrupt is processed */
#define EVENT_RX_EMPTY      0x02		/* set when rx fifo empty is detected (not an interrupt)*/
#define EVENT_TX_TH         0x04		/* set when tx fifo threshold interrupt is processed */
#define EVENT_TX_EMPTY      0x08		/* set when tx fifo empty is detected (not an interrupt)*/
#define EVENT_BUS_ERROR     0x10		/* set when bus error is detected */
static volatile uint32_t event_flags;	/* event_flags: set in interrupt, cleared in main loop */

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
#else
#warning "No UART PIN/CLK setup for this example"
#endif
}

/* Display error string and spin */
static void errorOut(char *errStr)
{
	Chip_SYSCON_DisableSysFIFO(SYSCON_FIFO_U0TXFIFOEN | SYSCON_FIFO_U0RXFIFOEN);
	DEBUGOUT(errStr);
	//while (1) {}
}

/* Initialize the UART ROM Driver */
static int uartrom_init(void)
{
	int sz;

	/* Setup UART pin muxing */
	UART_PinMuxSetup();

	/* Get and check memory size for driver context */
	sz =  ROM_UART_GetMemSize();
	if (sz > sizeof(drv_mem)) {
		errorOut("Driver context area is too small, increase size of drv_mem\r\n");
	}

	/* Initiailize driver and get UART driver handle */
	hUART = ROM_UART_Init(drv_mem, LPC_USART0_BASE, NULL);
	if (hUART == NULL) {
		errorOut("Error initializing UART driver\r\n");
	}

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
		errorOut("Error calculating baud rate\r\n");
	}

	/* Set fractional control register */
	Chip_SYSCON_SetUSARTFRGCtrl(baud.mul, 255);

	/* See if the calculated baud is < +/- UART_BUAD_ERR% of the required baud */
	if (ABS(baud.baud - UART_BAUD_RATE) > (UART_BAUD_RATE * UART_BUAD_ERR) / 100) {
		/* WARNING: Baud rate is has more than UART_BUAD_ERR percentage */
		/* Try to auto-detect the Oversampling rate by setting baud.ovr to 0 */
		errorOut("Error : calculated baud rate is not within tolerance\r\n");
	}

	/* Configure the UART */
	cfg.cfg = UART_CFG_8BIT | UART_CFG_BRKRX;
	cfg.div = baud.div;	/* Use the calculated div value */
	cfg.ovr = baud.ovr;	/* Use oversampling rate from baud */
	cfg.res = UART_BIT_DLY(UART_BAUD_RATE);
	ROM_UART_Configure(hUART, &cfg);

	/* Enable UART interrupts, system FIFO used standard UART interrupts */
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);

	return 0;
}

/* Setup UART system FIFO */
static void setup_uartFifo(int uartIndex, uint32_t szr, uint32_t szt)
{
	LPC_FIFO_CFG_T uartCfg;

	/* Configurre UART 0 */
	uartCfg.noTimeoutContWrite = 0;		/* Timeout Continue On Write: timeout is reset each time data is transferred into the receive FIFO.*/
	uartCfg.noTimeoutContEmpty = 0;		/* Timeout Continue On Empty. When 0, the timeout is reset when the receive FIFO becomes empty. */
	uartCfg.timeoutBase = 0;			/* Use bits 3..7 for timeout value, used with timeoutValue. Range: 0..15*/
	uartCfg.timeoutValue = 12;			/* Timeout value is 0b1111 << timeoutBase = 0x78 clocks. Range: 2..15 */
	uartCfg.rxThreshold = szr / 2;		/* FIFO TX threshold interrupt on half FIFO level */
	uartCfg.txThreshold = szt / 2;		/* FIFO RX threshold interrupt on half FIFO level */
	Chip_FIFOUSART_Configure(LPC_FIFO, uartIndex, &uartCfg);

	/* Flush UART FIFOs */
	Chip_FIFOUSART_FlushFIFO(LPC_FIFO, uartIndex, (LPC_PERIPFIFO_INT_RXFLUSH | LPC_PERIPFIFO_INT_TXFLUSH));
}

/* Setup global system FIFO */
static void setup_systemFifo(void)
{
	LPC_FIFO_CFGSIZE_T cfg;
	uint32_t szr, szt;

	/* Initialize syetem FIFO */
	Chip_FIFO_Init(LPC_FIFO);

	/* Get global FIFO size for UARTs */
	szr = Chip_FIFO_GetFifoSpace(LPC_FIFO, FIFO_USART, FIFO_RX);
	szt = Chip_FIFO_GetFifoSpace(LPC_FIFO, FIFO_USART, FIFO_TX);

	/* Enable FIFO */
	Chip_SYSCON_EnableSysFIFO(SYSCON_FIFO_U0TXFIFOEN | SYSCON_FIFO_U0RXFIFOEN);

	/* Global UART FIFO configuration */
	cfg.fifoRXSize[0] = szr / 2;	/* UART 0 RX */
	cfg.fifoTXSize[0] = szt / 2;	/* UART 0 TX */
	cfg.fifoRXSize[1] = szr / 8;	/* UART 1 RX */
	cfg.fifoTXSize[1] = szt / 8;	/* UART 1 TX */
	cfg.fifoRXSize[2] = szr / 8;	/* UART 2 RX */
	cfg.fifoTXSize[2] = szt / 8;	/* UART 2 TX */
	cfg.fifoRXSize[3] = szr / 8;	/* UART 3 RX */
	cfg.fifoTXSize[3] = szt / 8;	/* UART 3 TX */
	Chip_FIFO_ConfigFifoSize(LPC_FIFO, FIFO_USART, &cfg);

	/* Setup UART 0 FIFO */
	setup_uartFifo(UART_ID, cfg.fifoRXSize[UART_ID], cfg.fifoTXSize[UART_ID]);

	/* Unpause FIFOS after configuration */
	Chip_FIFO_UnpauseFifo(LPC_FIFO, FIFO_USART, FIFO_RX);
	Chip_FIFO_UnpauseFifo(LPC_FIFO, FIFO_USART, FIFO_TX);
}

/* Insert a buffer of data to the TX ring buffer */
static uint32_t outViaRingBuffer(uint8_t *tx_buff, uint32_t len)
{
	uint32_t rb_ct, write_ct;

	if (RingBuffer_IsFull(&tx_rb)) {
		return 0;
	}

	rb_ct = RingBuffer_GetFree(&tx_rb);
	write_ct = (rb_ct > len) ? len : rb_ct;

	RingBuffer_InsertMult(&tx_rb, tx_buff, write_ct);

	Chip_FIFOUSART_EnableInts(LPC_FIFO, UART_ID, LPC_PERIPFIFO_INT_TXTH);

	return write_ct;
}

/* Pop a buffer of data from the RX ring buffer */
static uint32_t inViaRingBuffer(uint8_t *rx_buff, uint32_t len)
{
	uint32_t rb_ct, read_ct;

	if (RingBuffer_IsEmpty(&rx_rb)) {
		return 0;
	}

	rb_ct = RingBuffer_GetCount(&rx_rb);
	read_ct = (rb_ct > len) ? len : rb_ct;

	RingBuffer_PopMult(&rx_rb, rx_buff, read_ct);

	return read_ct;
}

/* Insert a buffer from the FIFO into the RX ring buffer */
static void FIFO_rx_read(uint32_t uart)
{
	uint32_t fifo_ct, ringbuff_ct, ct;

	fifo_ct = Chip_FIFOUSART_GetRxCount(LPC_FIFO, uart);
	ringbuff_ct = RingBuffer_GetFree(&rx_rb);
	ct = (fifo_ct > ringbuff_ct) ? ringbuff_ct : fifo_ct;
	Chip_FIFOUSART_ReadRX(LPC_FIFO, uart, true, io_buff, ct);
	RingBuffer_InsertMult(&rx_rb, io_buff, ct);
}

/* Pop a buffer from the TX ring buffer into the TX FIFO */
static void FIFO_tx_write(uint32_t uart)
{
	uint32_t fifo_ct, ringbuff_ct, ct;

	fifo_ct = Chip_FIFOUSART_GetTxCount(LPC_FIFO, uart);
	ringbuff_ct = RingBuffer_GetCount(&tx_rb);
	ct = (fifo_ct > ringbuff_ct) ? ringbuff_ct : fifo_ct;
	RingBuffer_PopMult(&tx_rb, io_buff, ct);
	Chip_FIFOUSART_WriteTX(LPC_FIFO, uart, true, io_buff, ct);
}

/* UART ISR function */
/* It's possible to get non-data interrupts directly from the UART. */
/* There should be no data oriented interrupts (e.g. TX ready, RX ready, TX idle). */
static void UART_ISR_proc(void)
{
	UART_REGS_T *pUART = LPC_USART0;
	uint32_t        flags = pUART->INTENSET & pUART->INTSTAT;

	/* clear interrupt status */
	pUART->INTENCLR = flags;

	/* transmitter ready (shift register empty) */
	if (flags & UART_INT_TXRDY) {
		u_stat.tx_rdy++;
	}

	/* framing error */
	if (flags & UART_INT_FRMERR) {
		u_stat.er_frame++;
	}

	/* parity error */
	if (flags & UART_INT_PARERR) {
		u_stat.er_parity++;
	}

	/* auto baud error */
	if (flags & UART_INT_ABAUDERR) {
		u_stat.er_auto_baud++;
	}

	/* receive line noise error */
	if (flags & UART_INT_RXNOISE) {
		u_stat.er_rx_noise++;
	}

	/* over-run error */
	if (flags & UART_INT_OVR) {
		u_stat.er_overrun++;
	}

	/* receive character ready */
	if (flags & UART_INT_RXRDY) {
		u_stat.rx_rdy++;
	}

	/* transmitter idle (shift and transmit buffer empty) */
	if (flags & UART_INT_TXIDLE) {
		u_stat.tx_idle++;
	}

	/* transmitter disabled */
	if (flags & UART_INT_TXDIS) {
		u_stat.tx_dis++;
	}

	/* CTS status change error */
	if (flags & UART_INT_CTS) {
		u_stat.stat_cts++;
	}

	/* BREAK received */
	if (flags & UART_INT_BREAK) {
		u_stat.stat_break++;
	}

	/* start bit detected */
	if (flags & UART_INT_START) {
		u_stat.stat_start++;
	}
}

static void FIFO_ISR_proc(void)
{
	uint32_t fifo_stat = Chip_FIFOUSART_GetIntStatus(LPC_FIFO, UART_ID);

	/* RX threshold interrupt */
	if (fifo_stat & LPC_PERIPFIFO_STAT_RXTH) {
		FIFO_rx_read(UART_ID);
		event_flags |= EVENT_RX_TH;
	}

	/* TX threshold interrupt */
	if (fifo_stat & LPC_PERIPFIFO_STAT_TXTH) {
		if (RingBuffer_IsEmpty(&tx_rb)) {
			Chip_FIFOUSART_DisableInts(LPC_FIFO, UART_ID, LPC_PERIPFIFO_INT_TXTH);
		}
		else {
			FIFO_tx_write(UART_ID);
		}
		event_flags |= EVENT_TX_TH;
	}

	/* RX timer time-out */
	if (fifo_stat & LPC_PERIPFIFO_STATCLR_RXTIMEOUT) {
		FIFO_rx_read(UART_ID);
		Chip_FIFOUSART_ClearStatus(LPC_FIFO, UART_ID, LPC_PERIPFIFO_STATCLR_RXTIMEOUT);
	}

	/* Bus error*/
	if (fifo_stat & LPC_PERIPFIFO_STATCLR_BUSERR) {
		Chip_FIFOUSART_ClearStatus(LPC_FIFO, UART_ID, LPC_PERIPFIFO_STATCLR_BUSERR);
		event_flags |= EVENT_BUS_ERROR;
	}

	/* RX FIFO empty, note: this is not an interrupt, it's a status */
	if (fifo_stat & LPC_PERIPFIFO_STAT_RXEMPTY) {
		event_flags |= EVENT_RX_EMPTY;
	}

	/* TX FIFO empty, note: this is not an interrupt, it's a status */
	if (fifo_stat & LPC_PERIPFIFO_STAT_TXEMPTY) {
		event_flags |= EVENT_TX_EMPTY;
	}
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	FIFO interrupt handler
 * @return	No return value
 */
void UART0_IRQHandler(void)
{
	UART_ISR_proc();
	FIFO_ISR_proc();
}

/**
 * @brief	Main routine for FIFO UART example
 * @return	Function should not exit
 */
int main(void)
{
	uint32_t in_ct;
	uint32_t loop_ct = 0;
	uint8_t txbuf[128];
	uint8_t rxbuf[128];
	

	/* Generic Initialization */
	SystemCoreClockUpdate();
	Board_Init();

	/* Initialize the PinMux and setup the memory for ROM driver */
	uartrom_init();

	/* Configure the UART */
	uartrom_config();

	/* Setup system FIFO for global operation */
	setup_systemFifo();

	/* Initialize the ring-buffer */
	RingBuffer_Init(&rx_rb, rx_buff, sizeof(uint8_t), RB_COUNT);
	RingBuffer_Init(&tx_rb, tx_buff, sizeof(uint8_t), RB_COUNT);

	Chip_FIFOUSART_EnableInts(LPC_FIFO, UART_ID, (LPC_PERIPFIFO_INT_RXTH | LPC_PERIPFIFO_INT_RXTIMEOUT));
	Board_LED_Set(0, true);

	outViaRingBuffer("UART FIFO demo ready.\n\r", 23);
	memset(txbuf,0x56,128);
	/* Main loop: serial port data echo */
	/* - after 1000 transfers, toggle the RED LED */
	/* - If we have received a buffer from the serial port, send it back out the serial port */
	while (1) {
		if ((loop_ct++ % 100) == 0) {
			if(ap_buff[0]==0x56)
				{
			Board_LED_Toggle(1);
				}
		}

		if (event_flags & EVENT_RX_TH) {
			in_ct = inViaRingBuffer(ap_buff, RB_COUNT);
		
			outViaRingBuffer(txbuf, RB_COUNT);
			event_flags &= ~EVENT_RX_TH;
		}

		__WFI();
	}
	return 0;
}
