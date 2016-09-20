/*
 * @brief UART interrupt/DMA example using the ROM API
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
#include "romapi_dma.h"

/** @defgroup UART_INT_DMA_5410X UART interrupt/DMA example using the ROM API
 * @ingroup EXAMPLES_ROM_5410X
 * @include "rom\uart_int_dma\readme.txt"
 */

/**
 * @}
 */

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
#ifdef __ICCARM__
#define ALIGNSTR(x) # x
#define ALIGN(x) _Pragma(ALIGNSTR(data_alignment = ## x))
#else
#define ALIGN(x) __attribute__ ((aligned(x)))
#endif

/* UART Driver context memory */
#define RAMBLOCK_H          60
static uint32_t  drv_mem[RAMBLOCK_H];

/* UART ROM Driver Handle */
static UART_HANDLE_T hUART;
/* DMA ROM Driver Handle */
static ROM_DMA_HANDLE_T  hDMA;

#define UART_BAUD_RATE     115200	/* Required UART Baud rate */
#define UART_BUAD_ERR      1/* Percentage of Error allowed in baud */

#define tmsg1(x) "Type " # x " chars to echo it back\r\n"
#define tmsg(x) tmsg1(x)
#define RX_SZ  16
#define msg   tmsg(RX_SZ)
#define rmsg  "UART received : "

static volatile uint32_t tx_done, rx_done;

/* DMA Queues */
ROM_DMA_QUEUE_T txQueue, rxQueue;

#define DMA_UART0_RXCH      0		/* DMA RX Channel */
#define DMA_UART0_TXCH      1		/* DMA TX Channel */

/* DMA Descriptor table memory */
ALIGN(512) ROM_DMA_DESC_T EXChip_DMA_Table[MAX_DMA_CHANNEL];
ALIGN(16) ROM_DMA_DESC_T dmaDescTX;
ALIGN(16) ROM_DMA_DESC_T dmaDescRX;

LPC_DMA_T *x = LPC_DMA;

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

static void uart_dma_txdone(ROM_DMA_HANDLE_T dmaHandle, struct ROM_DMA_QUEUE *pQueue, ROM_DMA_DESC_T *pTranDesc)
{
	tx_done = 1;
}

static void uart_dma_txdesc(ROM_DMA_HANDLE_T dmaHandle, struct ROM_DMA_QUEUE *pQueue, ROM_DMA_DESC_T *pTranDesc)
{
	tx_done = 1;
}

static void uart_dma_txerr(ROM_DMA_HANDLE_T dmaHandle, struct ROM_DMA_QUEUE *pQueue, ROM_DMA_DESC_T *pTranDesc)
{}

static void uart_dma_rxdone(ROM_DMA_HANDLE_T dmaHandle, struct ROM_DMA_QUEUE *pQueue, ROM_DMA_DESC_T *pTranDesc)
{
	rx_done = RX_SZ;
}

static void uart_dma_rxdesc(ROM_DMA_HANDLE_T dmaHandle, struct ROM_DMA_QUEUE *pQueue, ROM_DMA_DESC_T *pTranDesc)
{}

static void uart_dma_rxerr(ROM_DMA_HANDLE_T dmaHandle, struct ROM_DMA_QUEUE *pQueue, ROM_DMA_DESC_T *pTranDesc)
{}

static void dmarom_init(void)
{
	static uint32_t memDMA[32];
	ROM_DMA_INIT_T dmaInit = {
		0,
		LPC_DMA_BASE,
		0
	};
	ROM_DMA_CHAN_CFG_T chanCfgRX = {
		1,											/* Use peripheral DMA request */
		0,											/* Hardware trigger polarity high */
		0,											/* Hardware trigger edge triggered */
		1,											/* Burst transfer on each trigger */
		ROM_DMA_BURSTPOWER_1,						/* Burst size of 1 datum */
		0,											/* Disable source burst wrap */
		0,											/* Disable destination burst wrap */
		2,											/* Channel priority = 0 (highest) */
		0,											/* reserved */
	};
	ROM_DMA_CHAN_CFG_T chanCfgTX = {
		1,											/* Use peripheral DMA request */
		0,											/* Hardware trigger polarity high */
		0,											/* Hardware trigger edge triggered */
		1,											/* Burst transfer on each trigger */
		ROM_DMA_BURSTPOWER_1,						/* Burst size of 1 datum */
		0,											/* Disable source burst wrap */
		0,											/* Disable destination burst wrap */
		1,											/* Channel priority = 0 (highest) */
		0,											/* reserved */
	};
	dmaInit.sramBase = (uint32_t) EXChip_DMA_Table;

	/* Enable DMA clocking prior to calling DMA init functions */
	Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_DMA);

	if (sizeof(memDMA) < ROM_DMA_GetMemSize()) {
		DEBUGSTR("Memory for DMA too small!\r\n");
		while (1) {}
	}

	hDMA = ROM_DMA_Init(memDMA, &dmaInit);
	if (hDMA == NULL) {
		DEBUGSTR("Unable to initialize DMA ROM Driver!\r\n");
		while (1) {}
	}
	ROM_DMA_SetupChannel(hDMA, &chanCfgTX, DMA_UART0_TXCH);
	ROM_DMA_InitQueue(hDMA, DMA_UART0_TXCH, &txQueue);
	ROM_DMA_SetupChannel(hDMA, &chanCfgRX, DMA_UART0_RXCH);
	ROM_DMA_InitQueue(hDMA, DMA_UART0_RXCH, &rxQueue);

	/* Register error, descriptor completion, and descriptor chain completion callbacks for UART TX */
	ROM_DMA_RegisterQueueCallback(hDMA, &txQueue, ROM_DMA_XFERCOMPLETE_CB, (void *) uart_dma_txdone);
	ROM_DMA_RegisterQueueCallback(hDMA, &txQueue, ROM_DMA_XFERDESCCOMPLETE_CB, (void *) uart_dma_txdesc);
	ROM_DMA_RegisterQueueCallback(hDMA, &txQueue, ROM_DMA_XFERERROR_CB, (void *) uart_dma_txerr);

	/* Register error, descriptor completion, and descriptor chain completion callbacks for UART RX */
	ROM_DMA_RegisterQueueCallback(hDMA, &rxQueue, ROM_DMA_XFERCOMPLETE_CB, (void *) uart_dma_rxdone);
	ROM_DMA_RegisterQueueCallback(hDMA, &rxQueue, ROM_DMA_XFERDESCCOMPLETE_CB, (void *) uart_dma_rxdesc);
	ROM_DMA_RegisterQueueCallback(hDMA, &rxQueue, ROM_DMA_XFERERROR_CB, (void *) uart_dma_rxerr);

	/* Enable the interrupt for the DMA controller */
	NVIC_EnableIRQ(DMA_IRQn);
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

static void uartdma_setuptx(UART_DATA_T *dat)
{
	ROM_DMA_XFERDESC_CFG_T XferCfg;
	ROM_DMA_DESC_T *pPrevDesc;

	/* clear stack structures before use */
	memset(&XferCfg, 0, sizeof(ROM_DMA_XFERDESC_CFG_T));

	/* Setup source to destination copy for trigger for memory */
	XferCfg.xferCount = dat->count;				/* Transfer 1 data values of size width */
	XferCfg.swTrig = 1;							/* No software triggering */
	XferCfg.clrTrig = 0;							/* Clear trigger after this descriptor completes */
	XferCfg.fireDescCB = 1;						/* Fire descriptor callback on complettion of this descriptor */
	XferCfg.enabCirc = 0;							/* Not a circular buffer */
	XferCfg.stallDesc = 0;							/* No descriptor stalling */
	XferCfg.dmaCh = txQueue.dmaCh;					/* DMA channel */
	XferCfg.width = dat->dwidth ? ROM_DMA_WIDTH_2 : ROM_DMA_WIDTH_1;			/* Width is 4 bytes */
	XferCfg.srcInc = dat->dwidth ? ROM_DMA_ADDRINC_2X : ROM_DMA_ADDRINC_1X;	/* Increment source address by width for each data transfer */
	XferCfg.dstInc = ROM_DMA_ADDRINC_0X;	/* Increment destination address by width for each data transfer */
	XferCfg.src  = dat->buf;	/* Source address */
	XferCfg.dest = (void *) UART_DMA_TXADDR(hUART);
	pPrevDesc = NULL;
	if (ROM_DMA_BuildDescriptorChain(hDMA, &XferCfg, &dmaDescTX, pPrevDesc) != LPC_OK) {
		DEBUGSTR("Error building descriptor chain (single link)\r\n");
	}

	/* Queue descriptor for transfer */
	ROM_DMA_QueueDescriptor(hDMA, &txQueue, &dmaDescTX);

	/* Start queue, won't transfer until trigger occurs */
	if (ROM_DMA_StartQueue(hDMA, &txQueue) != LPC_OK) {
		DEBUGSTR("Error starting queue\r\n");
	}
}

static void uartdma_setuprx(UART_DATA_T *dat)
{
	ROM_DMA_XFERDESC_CFG_T XferCfg;
	ROM_DMA_DESC_T *pPrevDesc;

	/* clear stack structures before use */
	memset(&XferCfg, 0, sizeof(ROM_DMA_XFERDESC_CFG_T));

	/* Setup source to desination copy for trigger for memory */
	XferCfg.xferCount = dat->count;							/* Transfer 1 data values of size width */
	XferCfg.swTrig = 1;							/* No software triggering */
	XferCfg.clrTrig = 1;							/* Clear trigger after this descriptor completes */
	XferCfg.fireDescCB = 1;						/* Fire descriptor callback on complettion of this descriptor */
	XferCfg.enabCirc = 0;							/* Not a circular buffer */
	XferCfg.stallDesc = 0;							/* No descriptor stalling */
	XferCfg.dmaCh = rxQueue.dmaCh;					/* DMA channel */
	XferCfg.width = dat->dwidth ? ROM_DMA_WIDTH_2 : ROM_DMA_WIDTH_1;			/* Width is 4 bytes */
	XferCfg.dstInc = dat->dwidth ? ROM_DMA_ADDRINC_2X : ROM_DMA_ADDRINC_1X;	/* Increment dest address by width for each data transfer */
	XferCfg.srcInc = ROM_DMA_ADDRINC_0X;	/* Increment source address by width for each data transfer */
	XferCfg.dest  = dat->buf;	/* Dest address */
	XferCfg.src = (void *) UART_DMA_RXADDR(hUART);
	pPrevDesc = NULL;
	if (ROM_DMA_BuildDescriptorChain(hDMA, &XferCfg, &dmaDescRX, pPrevDesc) != LPC_OK) {
		DEBUGSTR("Error building descriptor chain (single link)\r\n");
	}

	/* Queue descriptor for transfer */
	ROM_DMA_QueueDescriptor(hDMA, &rxQueue, &dmaDescRX);

	/* Start queue, won't transfer until trigger occurs */
	if (ROM_DMA_StartQueue(hDMA, &rxQueue) != LPC_OK) {
		DEBUGSTR("Error starting queue\r\n");
	}
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
		/* Typically problem will be a mismatched baudrate and/or too small Over sampling count */
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

/* UART transfer start */
static void uartrom_xfer_start(UART_HANDLE_T hUART, UART_EVENT_T evt, void *arg)
{
	UART_DATA_T *dat = (UART_DATA_T *) arg;

	(void) *dat;
	switch (evt) {
	case UART_TX_START:
		/* Transmit data started */
		break;

	case UART_RX_START:
		/* Receive of data started; can be used to implement timer logic */
		break;

	default:
		while (1) {}	/* Control will never reach here */
	}
}

/* DMA Xfer funcion */
static void uartrom_dma_xfer(UART_HANDLE_T hUART, UART_EVENT_T evt, void *arg)
{
	UART_DATA_T *dat = (UART_DATA_T *) arg;
	if (evt == UART_TX_DATA) {
		uartdma_setuptx(dat);
		dat->count = 0;
		return;
	}

	/* evt == UART_RX_DATA */
	uartdma_setuprx(dat);
	dat->count = 0;
}

/* Register call-backs */
static void uartrom_regcb(void)
{
	ROM_UART_RegisterCB(hUART, UART_CB_START, uartrom_xfer_start);	/* Start of transfer */
	ROM_UART_RegisterCB(hUART, UART_CB_ERREVT, uartrom_xfer_errevt);/* Error/Event callbacks */
	ROM_UART_RegisterCB(hUART, UART_CB_DATA, uartrom_dma_xfer);	/* Use DMA Data transfer */
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

/**
 * @brief	DMA controller interrupt handler
 * @return	Nothing
 */
void DMA_IRQHandler(void)
{
	ROM_DMA_DMAHandler(hDMA);
}

/**
 * @brief	Main routine for I2C example
 * @return	Function should not exit
 */
int main(void)
{
	uint8_t rxbuf[RX_SZ + 2];
	/* Generic Initialization */
	SystemCoreClockUpdate();
	Board_Init();

	/* Initialize the PinMux and setup the memory for ROM driver */
	uartrom_init();

	/* Configure the ADC */
	uartrom_config();

	uartrom_regcb();/* Register call-back functions */

	dmarom_init();	/* Initialize the DMA */

	ROM_UART_Send(hUART, msg, sizeof(msg) - 1);
	while (!tx_done) {
		__WFI();
	}
	while (1) {
		rx_done = 0;
		ROM_UART_Receive(hUART, rxbuf, RX_SZ);
		while (!rx_done) {
			__WFI();
		}
		tx_done = 0;
		ROM_UART_Send(hUART, rxbuf, rx_done);
		while (!tx_done) {
			__WFI();
		}
	}
	return 0;
}
