/*
 * @brief I2C bus slave example using DMA mode
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

/** @defgroup I2CS_INT_DMA_5410X I2C slave with DMA ROM API example
 * @ingroup EXAMPLES_ROM_5410X
 * @include "rom\i2cs_int_dma\readme.txt"
 */

/**
 * @}
 */

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/** I2C setup */
#define I2C_ADDR_7BIT                   (0x18)
#define LPC_I2C_PORT                    LPC_I2C2
#define LPC_I2CS_CLOCK              SYSCON_CLOCK_I2C2
#define LPC_I2CS_RESET              RESET_I2C2
#define LPC_I2CS_INT                    I2C2_IRQn
#define LPC_I2CS_HANDLER            I2C2_IRQHandler
#define LPC_I2C_DMACH                   ROM_DMAREQ_I2C2_SLAVE

#ifdef __ICCARM__
#define ALIGNSTR(x) # x
#define ALIGN(x) _Pragma(ALIGNSTR(data_alignment = ## x))
#else
#define ALIGN(x) __attribute__ ((aligned(x)))
#endif

/* DMA descriptors must be aligned to 16 bytes */
ALIGN(16) static ROM_DMA_DESC_T dmaI2CSDesc;

/* Alignement to 512 bytes */
ALIGN(512) ROM_DMA_DESC_T EXChip_DMA_Table[MAX_DMA_CHANNEL];

/* ROM driver handle for I2C slave */
static ROM_I2CS_HANDLE_T i2csHandle;
static ROM_DMA_HANDLE_T dmaHandle;

/* I2C and DMA driver context area */
static uint32_t i2csDrvData[16];
static uint32_t dmaDrvData[16];

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/* Made global so data can be seen in the debuuger */
ROM_I2CS_XFER_T sXfer;
uint8_t slaveRx[12], slaveTx[12];
volatile bool xferDone;
ROM_DMA_XFERDESC_CFG_T dmaXferCfg;
ROM_DMA_QUEUE_T i2cDmaQueue;
int lastStateRecv, i2cInts, dmaInts;

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Initializes pin muxing for I2C interface - note that SystemInit() may
   already setup your pin muxing at system startup */
static void Init_I2C_PinMux(void)
{
#if defined(BOARD_NXP_LPCXPRESSO_54102)
	/* Connect the I2C_SDA and I2C_SCL signals to port pins */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 27, (IOCON_FUNC1 | IOCON_DIGITAL_EN));	/* I2C2 */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 28, (IOCON_FUNC1 | IOCON_DIGITAL_EN));	/* I2C2 */

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

/* Setup I2C transmit side */
static void i2cSlaveTranSetup(ROM_I2CS_HANDLE_T i2csHandle, ROM_I2CS_XFER_T *pXfer)
{
	dmaXferCfg.src  = (void *) pXfer->txBuff;
	dmaXferCfg.dest = (void *) &LPC_I2C_PORT->SLVDAT;

	/* Called with a buffer size of 0, set the real size */
	pXfer->txSz = sizeof(slaveTx);

	/* Setup source to desination copy for trigger for memory */
	dmaXferCfg.xferCount = pXfer->txSz;				/* Transfer data values of size width */
	dmaXferCfg.swTrig = 1;									/* Software triggering */
	dmaXferCfg.clrTrig = 0;									/* Do not clear trigger after this descriptor completes */
	dmaXferCfg.fireDescCB = 0;							/* Do not fire descriptor callback on complettion of this descriptor */
	dmaXferCfg.enabCirc = 0;							/* Do not use circularo buffer */
	dmaXferCfg.dmaCh = LPC_I2C_DMACH;							/* Do not use circularo buffer */
	dmaXferCfg.stallDesc = 0;					/* No descriptor stalling */
	dmaXferCfg.srcInc = ROM_DMA_ADDRINC_1X;	/* Increment source address by width for each data transfer */
	dmaXferCfg.dstInc = ROM_DMA_ADDRINC_0X;	/* Increment destination address by width for each data transfer */
	if (ROM_DMA_BuildDescriptorChain(dmaHandle, &dmaXferCfg, &dmaI2CSDesc, NULL) != LPC_OK) {
		errorOut("Error building descriptor chain (I2C single link)\r\n");
	}

	/* Queue descriptor for SPI TX transfer */
	ROM_DMA_QueueDescriptor(dmaHandle, &i2cDmaQueue, &dmaI2CSDesc);

	/* Start queue */
	ROM_DMA_StartQueue(dmaHandle, &i2cDmaQueue);
}

void i2cSlaveRecvSetup(ROM_I2CS_HANDLE_T i2csHandle, ROM_I2CS_XFER_T *pXfer)
{
	dmaXferCfg.src = (void *) &LPC_I2C_PORT->SLVDAT;
	dmaXferCfg.dest  = (void *) pXfer->rxBuff;

	/* Called with a buffer size of 0, set the real size */
	pXfer->rxSz = sizeof(slaveRx);

	/* Setup source to desination copy for trigger for memory */
	dmaXferCfg.xferCount = pXfer->rxSz;				/* Transfer data values of size width */
	dmaXferCfg.swTrig = 1;									/* Software triggering */
	dmaXferCfg.clrTrig = 0;									/* Do not clear trigger after this descriptor completes */
	dmaXferCfg.fireDescCB = 0;							/* Do not fire descriptor callback on complettion of this descriptor */
	dmaXferCfg.enabCirc = 0;							/* Do not use circularo buffer */
	dmaXferCfg.dmaCh = LPC_I2C_DMACH;							/* Do not use circularo buffer */
	dmaXferCfg.stallDesc = 0;					/* No descriptor stalling */
	dmaXferCfg.width = ROM_DMA_WIDTH_1;			/* Width is 1 byte */
	dmaXferCfg.srcInc = ROM_DMA_ADDRINC_0X;	/* Increment source address by width for each data transfer */
	dmaXferCfg.dstInc = ROM_DMA_ADDRINC_1X;	/* Increment destination address by width for each data transfer */
	if (ROM_DMA_BuildDescriptorChain(dmaHandle, &dmaXferCfg, &dmaI2CSDesc, NULL) != LPC_OK) {
		errorOut("Error building descriptor chain (I2C single link)\r\n");
	}

	/* Queue descriptor for SPI TX transfer */
	ROM_DMA_QueueDescriptor(dmaHandle, &i2cDmaQueue, &dmaI2CSDesc);

	/* Start queue */
	ROM_DMA_StartQueue(dmaHandle, &i2cDmaQueue);
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* DMA controller interrupt handler */
void DMA_IRQHandler(void)
{
	dmaInts++;
	ROM_DMA_DMAHandler(dmaHandle);
}

/* DMA controller transfer descriptor chain complete callback */
void dmaI2CTransferComplete(ROM_DMA_HANDLE_T dmaHandle, struct ROM_DMA_QUEUE *pQueue, ROM_DMA_DESC_T *pTranDesc)
{}

/* DMA controller transfer descriptor complete callback */
void dmaI2CransferDescComplete(ROM_DMA_HANDLE_T dmaHandle, struct ROM_DMA_QUEUE *pQueue, ROM_DMA_DESC_T *pTranDesc)
{}

/* DMA controller transfer descriptor error callback */
void dmaI2CTransferError(ROM_DMA_HANDLE_T dmaHandle, struct ROM_DMA_QUEUE *pQueue, ROM_DMA_DESC_T *pTranDesc)
{}

void i2cSlaveCompleteFunc(ROM_I2CS_HANDLE_T i2csHandle, ROM_I2CS_XFER_T *pXfer)
{
	uint32_t cnt;
	bool *pxferDone = (bool *) ROM_I2CS_HANDLE_TOUDATA(i2csHandle);

	/* Stop DMA queue and count datum transferred */
	ROM_DMA_StopQueue(dmaHandle, &i2cDmaQueue);
	cnt = ((LPC_DMA->DMACH[i2cDmaQueue.dmaCh].XFERCFG >> 16) & 0x3FF);
	if (cnt == 0x3ff) {
		/* Entire DMA buffer was used */
		if (lastStateRecv == 1) {
			cnt = pXfer->rxSz;
		}
		else {
			cnt = pXfer->txSz;
		}
	}
	else {
		/* Partial DMA buffer was used */
		if (lastStateRecv == 1) {
			cnt = pXfer->rxSz - cnt - 1;
		}
		else {
			cnt = pXfer->txSz - cnt - 1;
		}
	}

	if (lastStateRecv == 1) {
		pXfer->bytesRecv = cnt;
	}
	else {
		pXfer->bytesSent = cnt;
	}

	*pxferDone = true;
}

void i2cSlaveStartFunc(ROM_I2CS_HANDLE_T i2csHandle, uint16_t addr)
{
	/* Event callback called when a transfer for an enabled slave is started */
	lastStateRecv = 0;
}

ROM_I2CS_TRANCTRL_T i2cSlaveReceiveFunc(ROM_I2CS_HANDLE_T i2csHandle, ROM_I2CS_XFER_T *pXfer)
{
	/* Setup DMA transfer */
	i2cSlaveRecvSetup(i2csHandle, pXfer);
	lastStateRecv = 1;
	return ROM_I2CS_DMA;
}

ROM_I2CS_TRANCTRL_T i2cSlaveTransmitFunc(ROM_I2CS_HANDLE_T i2csHandle, ROM_I2CS_XFER_T *pXfer)
{
	uint32_t cnt;

	/* If switching from Receive to transmit mode, then de-queue the DMA descriptor,
	   determine transfer size for the received buffer, and thenqueue transmit
	   descriptor. I2C slave may stretch clock. */
	if (lastStateRecv == 1) {
		/* Stop DMA queue and count datum transferred */
		ROM_DMA_StopQueue(dmaHandle, &i2cDmaQueue);
		cnt = ((LPC_DMA->DMACH[i2cDmaQueue.dmaCh].XFERCFG >> 16) & 0x3FF);
		if (cnt == 0x3ff) {
			/* Entire DMA buffer was used */
			cnt = pXfer->rxSz;
		}
		else {
			/* Partial DMA buffer was used */
			cnt = pXfer->rxSz - cnt - 1;
		}
		pXfer->bytesRecv = cnt;
	}

	/* Setup DMA transfer */
	i2cSlaveTranSetup(i2csHandle, pXfer);
	lastStateRecv = 0;
	return ROM_I2CS_DMA;
}

/* I2C slave interrupt handler */
void LPC_I2CS_HANDLER(void)
{
	i2cInts++;
	ROM_I2CS_TransferHandler(i2csHandle);
}

/**
 * @brief	Main routine for I2C example
 * @return	Function should not exit
 */
int main(void)
{
	uint32_t memSize, *devMem, optimalDev;
	ROM_I2CS_INIT_T i2csInit;
	ROM_I2CS_SLAVE_T slaveSetup;
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

	/* Setup I2C pin muxing, enable I2C clock and reset I2C peripheral */
	Init_I2C_PinMux();
	Chip_Clock_EnablePeriphClock(LPC_I2CS_CLOCK);
	Chip_SYSCON_PeriphReset(LPC_I2CS_RESET);

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

	/* Setup I2CS channel for peripheral request and init queue */
	if (ROM_DMA_SetupChannel(dmaHandle, (ROM_DMA_CHAN_CFG_T *) &chanCfg, LPC_I2C_DMACH) != LPC_OK) {
		errorOut("Error setting up I2CS DMA channel\r\n");
	}
	if (ROM_DMA_InitQueue(dmaHandle, LPC_I2C_DMACH, &i2cDmaQueue) != LPC_OK) {
		errorOut("Error initializing I2CS DMA queue\r\n");
	}

	/* Register error, descriptor completion, and descriptor chain completion callbacks for I2CS channel */
	ROM_DMA_RegisterQueueCallback(dmaHandle, &i2cDmaQueue, ROM_DMA_XFERCOMPLETE_CB, (void *) dmaI2CTransferComplete);
	ROM_DMA_RegisterQueueCallback(dmaHandle,
								  &i2cDmaQueue,
								  ROM_DMA_XFERDESCCOMPLETE_CB,
								  (void *) dmaI2CransferDescComplete);
	ROM_DMA_RegisterQueueCallback(dmaHandle, &i2cDmaQueue, ROM_DMA_XFERERROR_CB, (void *) dmaI2CTransferError);

	/******************************************************************/
	/* START OF I2CS SLAVE SETUP */
	/******************************************************************/

	/* Get needed size for driver context memory */
	memSize = ROM_I2CS_GetMemSize();
	if (memSize > sizeof(i2csDrvData)) {
		errorOut("Can't allocate memory for I2C driver context\r\n");
	}
	devMem = i2csDrvData;	/* Or just use malloc(memSize) */

	/* Initialize driver */
	i2csInit.pUserData = (void *) &xferDone;
	i2csInit.base = (uint32_t) LPC_I2C_PORT;
	i2csHandle = ROM_I2CS_Init(devMem, &i2csInit);
	if (i2csHandle == NULL) {
		/* Error initializing I2C */
		errorOut("Error initializing ROM\r\n");
	}

	/* Register callbacks */
	ROM_I2CS_RegisterCallback(i2csHandle, ROM_I2CS_DONE_CB, (void *) i2cSlaveCompleteFunc);
	ROM_I2CS_RegisterCallback(i2csHandle, ROM_I2CS_START_CB, (void *) i2cSlaveStartFunc);
	ROM_I2CS_RegisterCallback(i2csHandle, ROM_I2CS_XFERSEND_CB, (void *) i2cSlaveTransmitFunc);
	ROM_I2CS_RegisterCallback(i2csHandle, ROM_I2CS_XFERRECV_CB, (void *) i2cSlaveReceiveFunc);

	/* Setup clock rate for I2C - this must be done for master or slave modes.
	   This function is not set by the ROM API code. */
	optimalDev = Chip_Clock_GetAsyncSyscon_ClockRate() / 4000000;	/* 250nS */
	LPC_I2C_PORT->CLKDIV = optimalDev;

	/* Enable the interrupt for the I2C and DMA */
	NVIC_EnableIRQ(LPC_I2CS_INT);
	NVIC_EnableIRQ(DMA_IRQn);

	/* As soon as the slave is enabled, the I2C slave becomes active, but a
	   transfer hasn't been queued up just yet, so we'll hold off until it's
	   ready. */
	slaveSetup.slaveAddr = I2C_ADDR_7BIT;
	slaveSetup.SlaveIndex = 0;

	while (1) {
		/* Populate some TX data and clear RX data */
		for (i = 0; i < sizeof(slaveTx); i++) {
			slaveTx[i] = 0x70 + i;
			slaveRx[i] = 0;
		}
		sXfer.txBuff = slaveTx;
		sXfer.rxBuff = slaveRx;
		sXfer.flags = 0;

		/* Initial transfer sizes of 0 are used. This will cause the I2CS driver
		   to call the transmit and receive callback functions. Those functions
		   will setup the DMA as necessary for each transfer direction. */
		sXfer.txSz = 0;
		sXfer.rxSz = 0;

		/* Wait for a transfer from the master */
		xferDone = false;
		ROM_I2CS_Transfer(i2csHandle, &sXfer);	/* Never blocks, always returns LPC_OK */

		/* Enable the slave - after the transfer is setup */
		slaveSetup.EnableSlave = 1;
		ROM_I2CS_SetupSlave(i2csHandle, &slaveSetup);

		DEBUGOUT("Waiting for master\r\n");

		/* Wait for transfer to complete. The slave data completion callback
		   will set xferDone when ready via it's completion callback. */
		while (xferDone == false) {
			__WFI();
		}

		/* Disable the slave until it can be setup again */
		slaveSetup.EnableSlave = 0;
		ROM_I2CS_SetupSlave(i2csHandle, &slaveSetup);

		/* Check status of the transfer */
		DEBUGOUT("Transfer complete (%x)\r\n", sXfer.status);
		DEBUGOUT("RX [%d]", sXfer.bytesRecv);
		for (i = 0; i < sXfer.bytesRecv; i++) {
			DEBUGOUT(" : %02x", slaveRx[i]);
		}
		DEBUGOUT("\r\n");
		DEBUGOUT("TX [%d]", sXfer.bytesSent);
		for (i = 0; i < sXfer.bytesSent; i++) {
			DEBUGOUT(" : %02x", slaveTx[i]);
		}
		DEBUGOUT("\r\n");
	}

	/* Code never reaches here. Only used to satisfy standard main() */
	return 0;
}
