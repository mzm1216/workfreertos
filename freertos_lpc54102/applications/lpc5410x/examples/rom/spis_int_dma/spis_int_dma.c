/*
 * @brief SPIM bus slave example using DMA
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
#include <stdlib.h>

/** @defgroup SPIS_INT_DMA_5410X SPI slave (interrupt mode) with DMA ROM API example
 * @ingroup EXAMPLES_ROM_5410X
 * @include "rom\spis_int_dma\readme.txt"
 */

/**
 * @}
 */

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

#define LPC_SPIS_PORT         LPC_SPI0
#define LPC_IRQ_HANDLER             SPI0_IRQHandler
#define LPC_SPIIRQNUM         SPI0_IRQn
#define LPC_SPIS_CLOCK              SYSCON_CLOCK_SPI0
#define LPC_SPIS_RESET              RESET_SPI0
#define LPC_SPI_TX_DMACH      ROM_DMAREQ_SPI0_TX
#define LPC_SPI_RX_DMACH      ROM_DMAREQ_SPI0_RX

#ifdef __ICCARM__
#define ALIGNSTR(x) # x
#define ALIGN(x) _Pragma(ALIGNSTR(data_alignment = ## x))
#else
#define ALIGN(x) __attribute__ ((aligned(x)))
#endif

/* DMA descriptors must be aligned to 16 bytes */
ALIGN(16) static ROM_DMA_DESC_T dmaSPITXDesc;
ALIGN(16) static ROM_DMA_DESC_T dmaSPIRXDesc;

/* Keil alignement to 512 bytes */
ALIGN(512) ROM_DMA_DESC_T EXChip_DMA_Table[MAX_DMA_CHANNEL];

#define BUFFSENDSIZE 130

/* ROM driver handle for SPI slave */
static ROM_SPIS_HANDLE_T spisHandle;
static ROM_DMA_HANDLE_T dmaHandle;

/* SPI and DMA driver context areas */
static uint32_t spiDrvData[16];
static uint32_t dmaDrvData[16];

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/* Made global to help with debug */
uint16_t rx16[BUFFSENDSIZE], tx16[BUFFSENDSIZE];
ROM_DMA_QUEUE_T spiDmaTXQueue, spiDmaRXQueue;
ROM_SPIS_XFER_T sXfer;
ROM_DMA_XFERDESC_CFG_T dmaXferCfg;
int spiCnt = 0, dmaCnt = 0;
volatile bool xferDone;

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Initializes pin muxing for SPI interface - note that SystemInit() may
   already setup your pin muxing at system startup */
static void Init_SPI_PinMux(void)
{
#if defined(BOARD_NXP_LPCXPRESSO_54102)
	/* 1.3 = SPI0_SCK, 0.14 = SPI0_SSELN0, 0.12 = SPI0_MOSI, 1.4 = SPI0_MISO */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 3,  (IOCON_FUNC5 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 14, (IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 12, (IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 4,  (IOCON_FUNC5 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));

#else
	/* Configure your own I2C pin muxing here if needed */
#warning "No I2C pin muxing defined"
#endif
}

/* Display error string and spin */
static void errorOut(char *errStr)
{
	DEBUGOUT(errStr);
	while (1) {}
}

static void spiTranSetup(ROM_SPIS_HANDLE_T spisHandle, ROM_SPIS_XFER_T *pXfer)
{
	uint8_t flen;

	/* The DMA needs to be setup for 8- or 16-bit transfers based on the configured
	   SPI transfer size. For the example, we'll get the size from the SPI peripherals
	     config. */
	flen = 1 + (uint8_t) ((LPC_SPIS_PORT->TXCTRL >> 24) & 0xF);

	dmaXferCfg.src  = (void *) tx16;
	dmaXferCfg.dest = (void *) &LPC_SPIS_PORT->TXDAT;

	/* Setup source to desination copy for trigger for memory */
	dmaXferCfg.xferCount = pXfer->txSz;				/* Transfer data values of size width */
	dmaXferCfg.swTrig = 1;									/* No software triggering */
	dmaXferCfg.clrTrig = 0;									/* Do not clear trigger after this descriptor completes */
	dmaXferCfg.fireDescCB = 0;							/* Do not fire descriptor callback on complettion of this descriptor */
	dmaXferCfg.enabCirc = 0;							/* Do not use circular buffer */
	dmaXferCfg.dmaCh = spiDmaTXQueue.dmaCh;							/* DMA channel */
	dmaXferCfg.stallDesc = 0;					/* No descriptor stalling */
	if (flen > 8) {
		dmaXferCfg.width = ROM_DMA_WIDTH_2;			/* Width is 2 bytes */
	}
	else {
		dmaXferCfg.width = ROM_DMA_WIDTH_1;			/* Width is 1 byte */
	}
	dmaXferCfg.srcInc = ROM_DMA_ADDRINC_1X;	/* Increment source address by width for each data transfer */
	dmaXferCfg.dstInc = ROM_DMA_ADDRINC_0X;	/* Increment destination address by width for each data transfer */
	if (ROM_DMA_BuildDescriptorChain(dmaHandle, &dmaXferCfg, &dmaSPITXDesc, NULL) != LPC_OK) {
		errorOut("Error building descriptor chain (SPI TX single link)\r\n");
	}

	/* Queue descriptor for SPI TX transfer */
	ROM_DMA_QueueDescriptor(dmaHandle, &spiDmaTXQueue, &dmaSPITXDesc);

	/* Start queue */
	ROM_DMA_StartQueue(dmaHandle, &spiDmaTXQueue);
}

void spiRecvSetup(ROM_SPIS_HANDLE_T spisHandle, ROM_SPIS_XFER_T *pXfer)
{
	uint8_t flen;

	/* The DMA needs to be setup for 8- or 16-bit transfers based on the configured
	   SPI transfer size. For the example, we'll get the size from the SPI peripherals
	     config. */
	flen = 1 + (uint8_t) ((LPC_SPIS_PORT->TXCTRL >> 24) & 0xF);

	dmaXferCfg.src = (void *) &LPC_SPIS_PORT->RXDAT;
	dmaXferCfg.dest  = (void *) rx16;

	/* Setup source to desination copy for trigger for memory */
	dmaXferCfg.xferCount = pXfer->rxSz;			/* Transfer data values of size width */
	dmaXferCfg.swTrig = 1;									/* No software triggering */
	dmaXferCfg.clrTrig = 0;									/* Do not clear trigger after this descriptor completes */
	dmaXferCfg.fireDescCB = 0;							/* Do not fire descriptor callback on complettion of this descriptor */
	dmaXferCfg.enabCirc = 0;								/* Not a circular buffer */
	dmaXferCfg.dmaCh = spiDmaRXQueue.dmaCh;							/* Do not use circularo buffer */
	dmaXferCfg.stallDesc = 0;					/* No descriptor stalling */
	if (flen > 8) {
		dmaXferCfg.width = ROM_DMA_WIDTH_2;			/* Width is 2 bytes */
	}
	else {
		dmaXferCfg.width = ROM_DMA_WIDTH_1;			/* Width is 1 byte */
	}
	dmaXferCfg.srcInc = ROM_DMA_ADDRINC_0X;	/* Increment source address by width for each data transfer */
	dmaXferCfg.dstInc = ROM_DMA_ADDRINC_1X;	/* Increment destination address by width for each data transfer */
	if (ROM_DMA_BuildDescriptorChain(dmaHandle, &dmaXferCfg, &dmaSPIRXDesc, NULL) != LPC_OK) {
		errorOut("Error building descriptor chain (SPI TX single link)\r\n");
	}

	/* Queue descriptor for SPI TX transfer */
	ROM_DMA_QueueDescriptor(dmaHandle, &spiDmaRXQueue, &dmaSPIRXDesc);

	/* Start queue */
	ROM_DMA_StartQueue(dmaHandle, &spiDmaRXQueue);
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* DMA controller interrupt handler */
void DMA_IRQHandler(void)
{
	dmaCnt++;
	ROM_DMA_DMAHandler(dmaHandle);
}

/* DMA controller transfer descriptor chain complete callback */
void dmaSPITXTransferComplete(ROM_DMA_HANDLE_T dmaHandle, struct ROM_DMA_QUEUE *pQueue, ROM_DMA_DESC_T *pTranDesc)
{}

/* DMA controller transfer descriptor complete callback */
void dmaSPITXTransferDescComplete(ROM_DMA_HANDLE_T dmaHandle, struct ROM_DMA_QUEUE *pQueue, ROM_DMA_DESC_T *pTranDesc)
{}

/*DMA controller transfer descriptor error callback */
void dmaSPITXTransferError(ROM_DMA_HANDLE_T dmaHandle, struct ROM_DMA_QUEUE *pQueue, ROM_DMA_DESC_T *pTranDesc)
{}

/* DMA controller transfer descriptor chain complete callback */
void dmaSPIRXTransferComplete(ROM_DMA_HANDLE_T dmaHandle, struct ROM_DMA_QUEUE *pQueue, ROM_DMA_DESC_T *pTranDesc)
{}

/* DMA controller transfer descriptor complete callback */
void dmaSPIRXTransferDescComplete(ROM_DMA_HANDLE_T dmaHandle, struct ROM_DMA_QUEUE *pQueue, ROM_DMA_DESC_T *pTranDesc)
{}

/*DMA controller transfer descriptor error callback */
void dmaSPIRXTransferError(ROM_DMA_HANDLE_T dmaHandle, struct ROM_DMA_QUEUE *pQueue, ROM_DMA_DESC_T *pTranDesc)
{}

void LPC_IRQ_HANDLER(void)
{
	spiCnt++;
	ROM_SPIS_TransferHandler(spisHandle);
}

void CBspiSlaveXferCSTransmitCB(ROM_SPIS_HANDLE_T spisHandle, ROM_SPIS_XFER_T *pXfer)
{
	spiTranSetup(spisHandle, pXfer);
	spiRecvSetup(spisHandle, pXfer);
}

void CBspiSlaveXferCSReceiveCB(ROM_SPIS_HANDLE_T spisHandle, ROM_SPIS_XFER_T *pXfer)
{}

void CBspiSlaveXferCSAssertCB(ROM_SPIS_HANDLE_T spisHandle, uint8_t slaveNum)
{}

void CBspiSlaveXferCSDeAssertCB(ROM_SPIS_HANDLE_T spisHandle, ROM_SPIS_XFER_T *pXfer)
{
	uint32_t cnt;
	bool *pXferDone = (bool *) ROM_SPIS_HANDLE_TOUDATA(spisHandle);

	/* Stop DMA queue and count datum transferred */
	ROM_DMA_StopQueue(dmaHandle, &spiDmaTXQueue);
	cnt = ((LPC_DMA->DMACH[spiDmaTXQueue.dmaCh].XFERCFG >> 16) & 0x3FF);
	if (cnt == 0x3ff) {
		/* Entire DMA buffer was used */
		cnt = pXfer->txSz;
	}
	else {
		/* Partial DMA buffer was used */
		cnt = pXfer->txSz - cnt - 1;
	}
	pXfer->txSent += cnt;

	ROM_DMA_StopQueue(dmaHandle, &spiDmaRXQueue);
	cnt = ((LPC_DMA->DMACH[spiDmaRXQueue.dmaCh].XFERCFG >> 16) & 0x3FF);
	if (cnt == 0x3ff) {
		/* Entire DMA buffer was used */
		cnt = pXfer->rxSz;
	}
	else {
		/* Partial DMA buffer was used */
		cnt = pXfer->rxSz - cnt - 1;
	}
	pXfer->rxRecv += cnt;

	*pXferDone = true;
}

/**
 * @brief	Main routine for I2C example
 * @return	Function should not exit
 */
int main(void)
{
	uint32_t memSize, *devMem;
	ROM_SPIS_INIT_T spisInit;
	ROM_SPIS_SLAVE_T spisConfig;
	ROM_DMA_INIT_T dmaInit;
	int i;

	const ROM_DMA_CHAN_CFG_T chanCfg = {
		1,											/* Use peripheral DMA request */
		0,											/* Hardware trigger polarity high */
		0,											/* Hardware trigger edge triggered */
		0,											/* Single transfer on each trigger */
		ROM_DMA_BURSTPOWER_1,		/* Burst size of 1 datum */
		0,											/* Disable source burst wrap */
		0,											/* Disable destination burst wrap */
		0,											/* Channel priority = 0 (highest) */
		0,											/* reserved */
	};

	/* Generic Initialization */
	SystemCoreClockUpdate();
	Board_Init();

	/* Setup SPI pin muxing, enable SPI clock and reset SPI peripheral */
	Init_SPI_PinMux();
	Chip_Clock_EnablePeriphClock(LPC_SPIS_CLOCK);
	Chip_SYSCON_PeriphReset(LPC_SPIS_RESET);

	/******************************************************************/
	/* START OF DMA SETUP */
	/******************************************************************/

	/* Enable DMA clocking prior to calling DMA init functions */
	Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_DMA);

	/* Get needed size for DMA driver context memory */
	memSize = ROM_DMA_GetMemSize();
	if (memSize > sizeof(dmaDrvData)) {
		errorOut("Can't allocate memory for DMA driver context\r\n");
	}
	devMem = dmaDrvData;/* Or just use malloc(memSize) */

	/* Initialize DMA driver */
	dmaInit.pUserData = (void *) NULL;
	dmaInit.base = (uint32_t) LPC_DMA;
	dmaInit.sramBase = (uint32_t) EXChip_DMA_Table;
	dmaHandle = ROM_DMA_Init(devMem, &dmaInit);
	if (dmaHandle == NULL) {
		errorOut("Error initializing DMA\r\n");
	}

	/* Setup SPI1 TX channel for peripheral request and init queue */
	if (ROM_DMA_SetupChannel(dmaHandle, (ROM_DMA_CHAN_CFG_T *) &chanCfg, LPC_SPI_TX_DMACH) != LPC_OK) {
		errorOut("Error setting up SPI TX DMA channel\r\n");
	}
	if (ROM_DMA_InitQueue(dmaHandle, LPC_SPI_TX_DMACH, &spiDmaTXQueue) != LPC_OK) {
		errorOut("Error initializing SPI TX DMA queue\r\n");
	}

	/* Setup SPI1 RX channel for peripheral request and init queue */
	if (ROM_DMA_SetupChannel(dmaHandle, (ROM_DMA_CHAN_CFG_T *) &chanCfg, LPC_SPI_RX_DMACH) != LPC_OK) {
		errorOut("Error setting up SPI RX DMA channel\r\n");
	}
	if (ROM_DMA_InitQueue(dmaHandle, LPC_SPI_RX_DMACH, &spiDmaRXQueue) != LPC_OK) {
		errorOut("Error initializing SPI RX DMA queue\r\n");
	}

	/* Register error, descriptor completion, and descriptor chain completion callbacks for SPI TX channel */
	ROM_DMA_RegisterQueueCallback(dmaHandle, &spiDmaTXQueue, ROM_DMA_XFERCOMPLETE_CB, (void *) dmaSPITXTransferComplete);
	ROM_DMA_RegisterQueueCallback(dmaHandle,
								  &spiDmaTXQueue,
								  ROM_DMA_XFERDESCCOMPLETE_CB,
								  (void *) dmaSPITXTransferDescComplete);
	ROM_DMA_RegisterQueueCallback(dmaHandle, &spiDmaTXQueue, ROM_DMA_XFERERROR_CB, (void *) dmaSPITXTransferError);

	/* Register error, descriptor completion, and descriptor chain completion callbacks for SPI RX channel */
	ROM_DMA_RegisterQueueCallback(dmaHandle, &spiDmaRXQueue, ROM_DMA_XFERCOMPLETE_CB, (void *) dmaSPIRXTransferComplete);
	ROM_DMA_RegisterQueueCallback(dmaHandle,
								  &spiDmaRXQueue,
								  ROM_DMA_XFERDESCCOMPLETE_CB,
								  (void *) dmaSPIRXTransferDescComplete);
	ROM_DMA_RegisterQueueCallback(dmaHandle, &spiDmaRXQueue, ROM_DMA_XFERERROR_CB, (void *) dmaSPIRXTransferError);

	/******************************************************************/
	/* START OF SPI SLAVE SETUP */
	/******************************************************************/

	/* Get needed size for SPI driver context memory */
	memSize = ROM_SPIS_GetMemSize();
	if (memSize > sizeof(spiDrvData)) {
		errorOut("Can't allocate memory for SPI driver context\r\n");
	}
	devMem = spiDrvData;/* Or just use malloc(memSize) */

	/* Initialize driver */
	spisInit.pUserData = (void *) &xferDone;
	spisInit.base = (uint32_t) LPC_SPIS_PORT;
	spisInit.spiPol[0] = 0;	/* Active low select for SSEL0 */
	spisInit.spiPol[1] = 0;
	spisInit.spiPol[2] = 0;
	spisInit.spiPol[3] = 0;
	spisHandle = ROM_SPIS_Init(devMem, &spisInit);
	if (spisHandle == NULL) {
		/* Error initializing SPI */
		errorOut("Error initializing ROM\r\n");
	}

	/* Set SPI transfer configuration */
	spisConfig.mode = ROM_SPI_CLOCK_MODE0;
	spisConfig.lsbFirst = 0;
	spisConfig.dataBits = 8;
	if (ROM_SPIS_SetupSlave(spisHandle, &spisConfig) != LPC_OK) {
		errorOut("SPI configuration is invalid\r\n");
	}

	/* Callback registration  */
	ROM_SPIS_RegisterCallback(spisHandle, ROM_SPIS_ASSERTSSEL_CB, (void *) CBspiSlaveXferCSAssertCB);
	ROM_SPIS_RegisterCallback(spisHandle, ROM_SPIS_DATATRANSMIT_CB, (void *) CBspiSlaveXferCSTransmitCB);
	ROM_SPIS_RegisterCallback(spisHandle, ROM_SPIS_DATATRECEIVE_CB, (void *) CBspiSlaveXferCSReceiveCB);
	ROM_SPIS_RegisterCallback(spisHandle, ROM_SPIS_DEASSERTSSEL_CB, (void *) CBspiSlaveXferCSDeAssertCB);

	DEBUGSTR("SPI slave example\r\n");

	/* Enable the interrupt for DMA */
	NVIC_EnableIRQ(DMA_IRQn);

	/* Read data as fast as possible in loop */
	while (1) {
		/* Setup transfer buffers and maximum transfer sizes, DMA transfer flags */
		sXfer.txBuff = tx16;
		sXfer.txSz = BUFFSENDSIZE;
		sXfer.rxBuff = rx16;
		sXfer.rxSz = BUFFSENDSIZE;
		sXfer.flags = ROM_I2CM_FLAG_DMATX | ROM_I2CM_FLAG_DMARX;

		/* Start transfer. Will return immediately */
		xferDone = false;
		ROM_SPIS_Transfer(spisHandle, &sXfer);

		/* Enable the interrupt for SPI - it's done here after the transfer
		   is queued. */
		NVIC_EnableIRQ(LPC_SPIIRQNUM);

		/* Can monitor transfer status or completion flag to determine
		   end of transfer */
		while (xferDone == false) {
			/* Can sleep while waiting for SPI interrupts and end of trnasfer */
			__WFI();
		}

		/* Disable the SPI interrupt */
		NVIC_DisableIRQ(LPC_SPIIRQNUM);

		/* NOTE: Although valid SPI data has been received and the transfer is
		   cmoplete, the SPI slave interface is still active. Queue up another
		   transfer descriptor here (or better yet, in the de-assertion event handler)
		   or the next master operation to this slave will overrun or underrun
		   since the transfer isn't ready. For this example only, this may cause
		   the de-assertion event handler to be called with a NULL pointer to a
		   transfer descriptor (cuasing a crash). If this happens, either slow down
		   the master transfer rate or make sure a transfer descriptor is re-queued
		   prior to the master starting a transfer. */

		/* Check status of the transfer */
		if (sXfer.status != LPC_OK) {
			DEBUGOUT("-Error performing transfer = %x (%d)(%d, %d)\r\n", sXfer.status, sXfer.rxRecv, dmaCnt, spiCnt);
		}
		else {
			DEBUGOUT("-SPI transfer completed (%d): status = %x (%d, %d)\r\n",
					 sXfer.rxRecv,
					 sXfer.status,
					 dmaCnt,
					 spiCnt);
			for (i = 0; i < sXfer.rxRecv; i++) {
				DEBUGOUT("T:%02x R:%02x : ", tx16[i], rx16[i]);
			}
			DEBUGSTR("\r\n");
		}
		for (i = 0; i < sXfer.rxRecv; i++) {
			tx16[i] = rx16[i];
			rx16[i] = 0;
		}

		spiCnt = dmaCnt = 0;
	}

	/* Code never reaches here. Only used to satisfy standard main() */
	return 0;
}
