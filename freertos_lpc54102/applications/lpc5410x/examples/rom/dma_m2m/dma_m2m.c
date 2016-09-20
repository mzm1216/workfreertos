/*
 * @brief DMA ROM driver memory to memory example
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
#include "stopwatch.h"

/** @defgroup DMAALT_M2M_5410X DMA memory to memory example using ROM API
 * @ingroup EXAMPLES_ROM_5410X
 * @include "rom\dma_m2m\readme.txt"
 */

/**
 * @}
 */

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

#define NUMDMADESC 16

/* DMA driver context area */
static uint32_t drvData[16];

/* DMA descriptors must be aligned to 16 bytes */
#if defined(__CC_ARM)
__align(16) static ROM_DMA_DESC_T dmaDesc[NUMDMADESC];

/* Keil alignement to 512 bytes */
__align(512) ROM_DMA_DESC_T Chip_DMA_Table[ROM_DMALASTCHANNEL];
#endif /* defined (__CC_ARM) */

/* IAR support */
#if defined(__ICCARM__)
#pragma data_alignment=16
static ROM_DMA_DESC_T dmaDesc[NUMDMADESC];

/* IAR EWARM alignement to 512 bytes */
#pragma data_alignment=512
ROM_DMA_DESC_T Chip_DMA_Table[ROM_DMALASTCHANNEL];
#endif /* defined (__ICCARM__) */

#if defined( __GNUC__ )
static ROM_DMA_DESC_T dmaDesc[NUMDMADESC] __attribute__ ((aligned(16)));

/* GNU alignement to 512 bytes */
ROM_DMA_DESC_T Chip_DMA_Table[ROM_DMALASTCHANNEL] __attribute__ ((aligned(512)));
#endif /* defined (__GNUC__) */

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/* ROM driver handle for DMA controller */
ROM_DMA_HANDLE_T dmaHandle;

/* Source and destination buffers */
#define XFERSIZE    4096
uint32_t src[XFERSIZE];
uint32_t dst[XFERSIZE];

int timesTT;

/* Times */
uint32_t timeStart, timeDma, timeSw;

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Display error string and spin */
static void errorOut(char *errStr)
{
	DEBUGOUT(errStr);
	while (1) {}
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	DMA controller interrupt handler
 * @return	Nothing
 */
void DMA_IRQHandler(void)
{
	ROM_DMA_DMAHandler(dmaHandle);
}

/* DMA controller transfer descriptor chain complete callback */
void dmaTransferComplete(ROM_DMA_HANDLE_T dmaHandle, struct ROM_DMA_QUEUE *pQueue, ROM_DMA_DESC_T *pTranDesc)
{
	bool *pDone = (bool *) ROM_DMA_HANDLE_TOUDATA(dmaHandle);
	*pDone = true;
}

/* DMA controller transfer descriptor complete callback */
void dmaTransferDescComplete(ROM_DMA_HANDLE_T dmaHandle, struct ROM_DMA_QUEUE *pQueue, ROM_DMA_DESC_T *pTranDesc)
{
	/* Count it */
	timesTT++;
}

/*DMA controller transfer descriptor error callback */
void dmaTransferError(ROM_DMA_HANDLE_T dmaHandle, struct ROM_DMA_QUEUE *pQueue, ROM_DMA_DESC_T *pTranDesc)
{
	bool *pDone = (bool *) ROM_DMA_HANDLE_TOUDATA(dmaHandle);
	*pDone = true;
}

/**
 * @brief	Main routine for DMA controller
 * @return	Function should not exit
 */
int main(void)
{
	uint32_t memSize, *devMem;
	int i;
	ROM_DMA_INIT_T dmaInit;
	volatile bool done;
	ROM_DMA_XFERDESC_CFG_T XferCfg;
	ROM_DMA_DESC_T *pPrevDesc;
	ROM_DMA_QUEUE_T queue0;
	const ROM_DMA_CHAN_CFG_T chanCfg = {
		0,											/* Use hardware trigger */
		0,											/* Hardware trigger polarity high */
		0,											/* Hardware trigger edge triggered */
		0,											/* Single transfer on each trigger */
		ROM_DMA_BURSTPOWER_4,		/* Burst size of 4 datum */
		0,											/* Disable source burst wrap */
		0,											/* Disable destination burst wrap */
		0,											/* Channel priority = 0 (highest) */
		0,											/* reserved */
	};

	/* clear stack structures before use */
	memset(&XferCfg, 0, sizeof(ROM_DMA_XFERDESC_CFG_T));
	memset(&dmaInit, 0, sizeof(ROM_DMA_INIT_T));
	memset(&queue0, 0, sizeof(ROM_DMA_QUEUE_T));

	/* Generic Initialization */
	SystemCoreClockUpdate();
	Board_Init();

	/* Stopwatch will be used to time DMA vs memcpy */
	StopWatch_Init();

	/* Enable DMA clocking prior to calling DMA init functions */
	Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_DMA);

	/* Get needed size for driver context memory */
	memSize = ROM_DMA_GetMemSize();
	if (memSize > sizeof(drvData)) {
		errorOut("Can't allocate memory for driver context\r\n");
	}
	devMem = drvData;	/* Or just use malloc(memSize) */

	/* Initialize driver */
	dmaInit.pUserData = (void *) &done;
	dmaInit.base = (uint32_t) LPC_DMA;
	dmaInit.sramBase = (uint32_t) Chip_DMA_Table;
	dmaHandle = ROM_DMA_Init(devMem, &dmaInit);
	if (dmaHandle == NULL) {
		errorOut("Error initializing DMA\r\n");
	}

	/* Enable the interrupt for the DMA controller */
	NVIC_EnableIRQ(DMA_IRQn);

	/* Setup channel 0 for trigger operation and M2M transfer */
	if (ROM_DMA_SetupChannel(dmaHandle, (ROM_DMA_CHAN_CFG_T *) &chanCfg, ROM_DMA_CH0) != LPC_OK) {
		errorOut("Error setting up channel\r\n");
	}

	/* Init queue for channel 0 */
	if (ROM_DMA_InitQueue(dmaHandle, ROM_DMA_CH0, &queue0) != LPC_OK) {
		errorOut("Error initializing queue\r\n");
	}

	/* Register error, descriptor completion, and descriptor chain completion callbacks for channel 0 */
	ROM_DMA_RegisterQueueCallback(dmaHandle, &queue0, ROM_DMA_XFERCOMPLETE_CB, (void *) dmaTransferComplete);
	ROM_DMA_RegisterQueueCallback(dmaHandle, &queue0, ROM_DMA_XFERDESCCOMPLETE_CB, (void *) dmaTransferDescComplete);
	ROM_DMA_RegisterQueueCallback(dmaHandle, &queue0, ROM_DMA_XFERERROR_CB, (void *) dmaTransferError);

	while (1) {
		/* Setup source to desination copy for trigger for memory */
		XferCfg.xferCount = XFERSIZE / NUMDMADESC;	/* Transfer 32 data values of size width */
		XferCfg.swTrig = 0;							/* No woftware triggering */
		XferCfg.clrTrig = 0;						/* Do not clear trigger after this descriptor completes */
		XferCfg.fireDescCB = 1;						/* Fire descriptor callback on complettion of this descriptor */
		XferCfg.enabCirc = 0;						/* Not a circular buffer */
		XferCfg.width = ROM_DMA_WIDTH_4;			/* Width is 4 bytes */
		XferCfg.srcInc = ROM_DMA_ADDRINC_1X;		/* Increment source address by width for each data transfer */
		XferCfg.dstInc = ROM_DMA_ADDRINC_1X;		/* Increment destination address by width for each data transfer */
		XferCfg.dmaCh = ROM_DMA_CH0;				/* Channel 0, any channel is ok */
		pPrevDesc = NULL;
		for (i = 0; i < NUMDMADESC; i++) {
			XferCfg.src  = (void *) &src[(XFERSIZE / NUMDMADESC) * i];	/* Source address */
			XferCfg.dest = (void *) &dst[(XFERSIZE / NUMDMADESC) * i];	/* Destination address */
			if (ROM_DMA_BuildDescriptorChain(dmaHandle, &XferCfg, &dmaDesc[i], pPrevDesc) != LPC_OK) {
				errorOut("Error building descriptor chain (single link)\r\n");
			}
			pPrevDesc = &dmaDesc[i];
		}

		/* Queue descriptor for transfer */
		ROM_DMA_QueueDescriptor(dmaHandle, &queue0, &dmaDesc[0]);

		/* Start queue, won't transfer until trigger occurs */
		if (ROM_DMA_StartQueue(dmaHandle, &queue0) != LPC_OK) {
			errorOut("Error starting queue\r\n");
		}

		/* Populate some data to copy */
		for (i = 0; i < XFERSIZE; i++) {
			src[i] = i;
		}

		/* Force trigger */
		done = false;
		timesTT = 0;
		DEBUGSTR("Starting DMA...");

		timeStart = StopWatch_Start();
		ROM_DMA_ForceTrigger(dmaHandle, &queue0);

		/* Wait for DMA to fire indicating completion */
		while (done == false) {}
		timeDma = StopWatch_Elapsed(timeStart);

		DEBUGOUT("...done\r\n");

		/* Pop descriptor (chain) */
		pPrevDesc = ROM_DMA_UnQueueDescriptor(dmaHandle, &queue0);
		if (pPrevDesc != &dmaDesc[0]) {
			DEBUGSTR("DMA descriptor pop failed\r\n");
		}
		else {
			DEBUGOUT("xfer cnt = %d\r\n", ROM_DMA_GetTransferCount(dmaHandle, pPrevDesc));
		}

		for (i = 0; i < ((XFERSIZE / NUMDMADESC) * 16); i++) {
			if (src[i] != dst[i]) {
				DEBUGSTR("DMA compare failed\r\n");
			}
		}

		/* Measure time using memcpy() */
		timeStart = StopWatch_Start();
		memcpy(dst, src, (XFERSIZE * sizeof(uint32_t)));
		timeSw = StopWatch_Elapsed(timeStart);
		DEBUGOUT("DMA transfer time (mSecs) = %d\r\n", timeSw);
	}

	/* Code never reaches here. Only used to satisfy standard main() */
	return 0;
}
