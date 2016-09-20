/*
 * @brief I2CM bus master example using DMA mode
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

/** @defgroup I2CM_INT_DMA_5410X I2C master (DMA) ROM API example
 * @ingroup EXAMPLES_ROM_5410X
 * @include "rom\i2cm_int_dma\readme.txt"
 */

/**
 * @}
 */

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* 400KHz I2C bit-rate */
#define I2C_BITRATE         (400000)

/** I2C interface setup */
#define I2C_ADDR_7BIT       (0x4A)
#define LPC_I2C_PORT         LPC_I2C0
#define LPC_I2C_INTHAND      I2C0_IRQHandler
#define LPC_IRQNUM           I2C0_IRQn
#define LPC_I2CM_CLOCK              SYSCON_CLOCK_I2C0
#define LPC_I2CM_RESET              RESET_I2C0
#define LPC_I2C_DMACH               ROM_DMAREQ_I2C0_MASTER

#ifdef __ICCARM__
#define ALIGNSTR(x) # x
#define ALIGN(x) _Pragma(ALIGNSTR(data_alignment = ## x))
#else
#define ALIGN(x) __attribute__ ((aligned(x)))
#endif

/* DMA descriptors must be aligned to 16 bytes */
ALIGN(16) static ROM_DMA_DESC_T dmaI2CMDesc;

/* Keil alignement to 512 bytes */
ALIGN(512) ROM_DMA_DESC_T EXChip_DMA_Table[MAX_DMA_CHANNEL];

/* ROM driver handle for I2C master */
static ROM_I2CM_HANDLE_T i2cmHandle;
static ROM_DMA_HANDLE_T dmaHandle;

/* I2C and DMA driver context areas */
static uint32_t i2cDrvData[16];
static uint32_t dmaDrvData[16];

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/* Made global to help with debug */
uint8_t rx[10], tx[4];
ROM_I2CM_XFER_T mXfer;
uint32_t actualRate;
ROM_DMA_XFERDESC_CFG_T dmaXferCfg;
ROM_DMA_QUEUE_T i2cDmaQueue;
int lastStateRecv, i2cInts, dmaInts;
volatile bool done;

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Initializes pin muxing for I2C interface - note that SystemInit() may
   already setup your pin muxing at system startup */
static void Init_I2C_PinMux(void)
{
#if defined(BOARD_NXP_LPCXPRESSO_54102)
	/* Connect the I2C_SDA and I2C_SCL signals to port pins */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 23, (IOCON_FUNC1 | IOCON_DIGITAL_EN));	/* I2C0 */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 24, (IOCON_FUNC1 | IOCON_DIGITAL_EN));	/* I2C0 */

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

static void i2cMasterDoneCallback(ROM_I2CM_HANDLE_T handle, ROM_I2CM_XFER_T *pXfer)
{
	bool *done = (bool *) ROM_I2CM_HANDLE_TOUDATA(handle);

	*done = true;
}

static void i2cMasterTranSetup(ROM_I2CM_HANDLE_T i2cmHandle, ROM_I2CM_XFER_T *pXfer)
{
	if (pXfer->txSz > 0) {
		dmaXferCfg.src  = (void *) pXfer->txBuff;
		dmaXferCfg.dest = (void *) &LPC_I2C_PORT->MSTDAT;

		/* Setup source to desination copy for trigger from I2C */
		dmaXferCfg.xferCount = pXfer->txSz;				/* Transfer data values of size width */
		dmaXferCfg.swTrig = 1;									/* Software triggering */
		dmaXferCfg.clrTrig = 0;									/* Do not clear trigger after this descriptor completes */
		dmaXferCfg.fireDescCB = 0;							/* Do not fire descriptor callback on complettion of this descriptor */
		dmaXferCfg.enabCirc = 0;
		dmaXferCfg.width = ROM_DMA_WIDTH_1;			/* Width is 1 byte */
		dmaXferCfg.srcInc = ROM_DMA_ADDRINC_1X;	/* Increment source address by width for each data transfer */
		dmaXferCfg.dstInc = ROM_DMA_ADDRINC_0X;	/* Increment destination address by width for each data transfer */
		dmaXferCfg.dmaCh = LPC_I2C_DMACH;
		dmaXferCfg.stallDesc = 0;
		if (ROM_DMA_BuildDescriptorChain(dmaHandle, &dmaXferCfg, &dmaI2CMDesc, NULL) != LPC_OK) {
			errorOut("Error building descriptor chain (I2C single link)\r\n");
		}

		/* Queue descriptor for SPI TX transfer */
		ROM_DMA_QueueDescriptor(dmaHandle, &i2cDmaQueue, &dmaI2CMDesc);

		/* Start queue */
		ROM_DMA_StartQueue(dmaHandle, &i2cDmaQueue);
	}
}

void i2cMasterRecvSetup(ROM_I2CM_HANDLE_T i2cmHandle, ROM_I2CM_XFER_T *pXfer)
{
	if (pXfer->rxSz > 1) {
		dmaXferCfg.src = (void *) &LPC_I2C_PORT->MSTDAT;
		dmaXferCfg.dest  = (void *) pXfer->rxBuff;

		/* Setup source to desination copy for trigger from I2C */
		/* Note (-1) with RX count */
		dmaXferCfg.xferCount = pXfer->rxSz - 1;				/* Transfer data values of size width */
		dmaXferCfg.swTrig = 1;									/* Software triggering */
		dmaXferCfg.clrTrig = 0;									/* Do not clear trigger after this descriptor completes */
		dmaXferCfg.fireDescCB = 0;							/* Do not fire descriptor callback on complettion of this descriptor */
		dmaXferCfg.enabCirc = 0;
		dmaXferCfg.width = ROM_DMA_WIDTH_1;			/* Width is 1 byte */
		dmaXferCfg.srcInc = ROM_DMA_ADDRINC_0X;	/* Increment source address by width for each data transfer */
		dmaXferCfg.dstInc = ROM_DMA_ADDRINC_1X;	/* Increment destination address by width for each data transfer */
		dmaXferCfg.dmaCh = LPC_I2C_DMACH;
		dmaXferCfg.stallDesc = 0;
		if (ROM_DMA_BuildDescriptorChain(dmaHandle, &dmaXferCfg, &dmaI2CMDesc, NULL) != LPC_OK) {
			errorOut("Error building descriptor chain (I2C single link)\r\n");
		}

		/* Queue descriptor for SPI TX transfer */
		ROM_DMA_QueueDescriptor(dmaHandle, &i2cDmaQueue, &dmaI2CMDesc);

		/* Start queue */
		ROM_DMA_StartQueue(dmaHandle, &i2cDmaQueue);
	}
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* DMA controller interrupt handler */
void DMA_IRQHandler(void)
{
	dmaInts++;
	ROM_DMA_DMAHandler(dmaHandle);

	/* The I2C controller will not advance it's state machine and will stall
	   unless DMA is disabled. The data is already put in the FIFO's via DMA will
	   be transferred, so we need to disable DMA in I2C to allow I2C interrupts
	   to fire again. */
	LPC_I2C_PORT->MSTCTL = 0;
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

/**
 * @brief	Handle I2C1 interrupt by calling I2CM interrupt transfer handler
 * @return	Nothing
 */
void LPC_I2C_INTHAND(void)
{
	/* Call I2CM transfer handler function */
	ROM_I2CM_TransferHandler(i2cmHandle);
}

/**
 * @brief	Main routine for I2C example
 * @return	Function should not exit
 */
int main(void)
{
	uint32_t memSize, *devMem;
	ROM_I2CM_INIT_T i2cmInit;
	ROM_DMA_INIT_T dmaInit;

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
	Chip_Clock_EnablePeriphClock(LPC_I2CM_CLOCK);
	Chip_SYSCON_PeriphReset(LPC_I2CM_RESET);

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
		errorOut("Error setting up I2C DMA channel\r\n");
	}
	if (ROM_DMA_InitQueue(dmaHandle, LPC_I2C_DMACH, &i2cDmaQueue) != LPC_OK) {
		errorOut("Error initializing I2C DMA queue\r\n");
	}

	/* Register error, descriptor completion, and descriptor chain completion callbacks for I2CS channel */
	ROM_DMA_RegisterQueueCallback(dmaHandle, &i2cDmaQueue, ROM_DMA_XFERCOMPLETE_CB, (void *) dmaI2CTransferComplete);
	ROM_DMA_RegisterQueueCallback(dmaHandle,
								  &i2cDmaQueue,
								  ROM_DMA_XFERDESCCOMPLETE_CB,
								  (void *) dmaI2CransferDescComplete);
	ROM_DMA_RegisterQueueCallback(dmaHandle, &i2cDmaQueue, ROM_DMA_XFERERROR_CB, (void *) dmaI2CTransferError);

	/******************************************************************/
	/* START OF I2CM NASTER SETUP */
	/******************************************************************/

	/* Get needed size for driver context memory */
	memSize = ROM_I2CM_GetMemSize();
	if (memSize > sizeof(i2cDrvData)) {
		errorOut("Can't allocate memory for I2C driver context\r\n");
	}
	devMem = i2cDrvData;/* Or just use malloc(memSize) */

	/* Initialize driver */
	i2cmInit.pUserData = (void *) &done;
	i2cmInit.base = (uint32_t) LPC_I2C_PORT;
	i2cmHandle = ROM_I2CM_Init(devMem,  &i2cmInit);
	if (i2cmHandle == NULL) {
		/* Error initializing I2C */
		errorOut("Error initializing ROM\r\n");
	}

	/* Set I2C clock rate */
	actualRate = ROM_I2CM_SetClockRate(i2cmHandle,
									   Chip_Clock_GetAsyncSyscon_ClockRate(), I2C_BITRATE);
	DEBUGOUT("Actual I2C master rate = %dHz\r\n", actualRate);

	/* Enable the interrupt for the I2C */
	NVIC_EnableIRQ(LPC_IRQNUM);
	NVIC_EnableIRQ(DMA_IRQn);

	/* Register the transfer completion callback */
	ROM_I2CM_RegisterCallback(i2cmHandle, ROM_I2CM_DATACOMPLETE_CB, (void *) i2cMasterDoneCallback);

	/* Register data start callbacks */
	ROM_I2CM_RegisterCallback(i2cmHandle, ROM_I2CM_DATATRECEIVESTART_CB, (void *) i2cMasterRecvSetup);
	ROM_I2CM_RegisterCallback(i2cmHandle, ROM_I2CM_DATATRANSMITSTART_CB, (void *) i2cMasterTranSetup);

	while (1) {
		/* Populate some TX data and clear RX data */
		mXfer.slaveAddr = I2C_ADDR_7BIT;
		tx[0] = 0;
		tx[1] = 2;
		mXfer.txBuff = &tx;
		mXfer.rxBuff = &rx;
		mXfer.txSz = 2;
		mXfer.rxSz = 2;
		/* Do not use DMA with RX size of 1! */
		mXfer.flags = ROM_I2CM_FLAG_DMATX | ROM_I2CM_FLAG_DMARX;/* Will use DMA */

		/* Wait for a transfer from the master */
		done = false;
		ROM_I2CM_Transfer(i2cmHandle, &mXfer);

		/* Sleep while waiting for transfer to complete */
		while ((done == false) && (mXfer.status == ERR_I2C_BUSY)) {
			/* If it appears that this example is not working and is hanging here,
			   then the I2C interrupt may be firing before going into sleep mode,
			   so there is nothing to wakeup the device from sleep. Try commenting
			   out the line below. */
			__WFI();
		}

		/* Check status of the transfer */
		DEBUGOUT("I2C transfer completed: status = %x\r\n", mXfer.status);
		if (mXfer.status != LPC_OK) {
			errorOut("Error during I2CM transfer\r\n");
		}
	}

	/* Code never reaches here. Only used to satisfy standard main() */
	return 0;
}
