/*
 * @brief SPIM bus master example using interrupt mode
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
#define SPI_GLOBALS
#undef ADC_GLOBALS

#include "board.h"
#include <stdlib.h>
#include "romapi_spim.h"
#include "hw_spimd.h"
#include "driver_spi.h"
#include "driver_adc.h"

/*
 * @brief SPIM bus master example using DMA
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
#include "romapi_spim.h"
#include "romapi_5410x.h"

/** @defgroup SPIM_INT_DMA_5410X SPI master with DMA ROM API example
 * @ingroup EXAMPLES_ROM_5410X
 * @include "rom\spim_int_dma\readme.txt"
 */

/**
 * @}
 */

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* 3MHz SPI bit-rate */
#define SPI_BITRATE         (3000000)
#if 1
/* Use SPI0 port */
#define LPC_SPIM_PORT         LPC_SPI0
#define LPC_SPIM_CLOCK              SYSCON_CLOCK_SPI0
#define LPC_SPIM_RESET              RESET_SPI0
#define LPC_SPIM_INTHANDLER   SPI0_IRQHandler
#define LPC_SPIIRQNUM         SPI0_IRQn
#define LPC_SPI_TX_DMACH      ROM_DMAREQ_SPI0_TX
#define LPC_SPI_RX_DMACH      ROM_DMAREQ_SPI0_RX

#else
/* Use SPI0 port */
#define LPC_SPIM_PORT         LPC_SPI1
#define LPC_SPIM_CLOCK              SYSCON_CLOCK_SPI1
#define LPC_SPIM_RESET              RESET_SPI1
#define LPC_SPIM_INTHANDLER   SPI1_IRQHandler
#define LPC_SPIIRQNUM         SPI1_IRQn
#define LPC_SPI_TX_DMACH      ROM_DMAREQ_SPI1_TX
#define LPC_SPI_RX_DMACH      ROM_DMAREQ_SPI1_RX

#endif
/* Use SPI0 port */
#define LPC_SPIM_PORT1         	LPC_SPI1
#define LPC_SPIM_CLOCK1              SYSCON_CLOCK_SPI1
#define LPC_SPIM_RESET1              RESET_SPI1
#define LPC_SPIM_INTHANDLER1   SPI1_IRQHandler
#define LPC_SPIIRQNUM1         SPI1_IRQn
#define LPC_SPI_TX_DMACH1      ROM_DMAREQ_SPI1_TX
#define LPC_SPI_RX_DMACH1      ROM_DMAREQ_SPI1_RX

/* ROM driver handle for SPI master and DMA */
static ROM_SPIM_HANDLE_T spimHandle;
static ROM_SPIM_HANDLE_T spimHandle1;
static ROM_DMA_HANDLE_T dmaHandle;

/* SPI and DMA driver context areas */
static uint32_t spiDrvData[16];
static uint32_t spiDrvData1[16];
static uint32_t dmaDrvData[16];

#define XFERSZ 16	/* Either 8 or 16, number of data bits */
#define BUFFSIZE    64


/* DMA descriptors must be aligned to 16 bytes */
ALIGN(16) static ROM_DMA_DESC_T dmaSPITXDesc;
ALIGN(16) static ROM_DMA_DESC_T dmaSPIRXDesc;

ALIGN(16) static ROM_DMA_DESC_T dmaSPITXDesc1;
ALIGN(16) static ROM_DMA_DESC_T dmaSPIRXDesc1;

/* Alignement to 512 bytes */
ALIGN(512) ROM_DMA_DESC_T EXChip_DMA_Table[MAX_DMA_CHANNEL];

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/* Made global to help with debug */
ROM_SPIM_XFER_T mXfer,mXfer1;
ROM_DMA_QUEUE_T spiDmaTXQueue, spiDmaRXQueue;
ROM_DMA_QUEUE_T spiDmaTXQueue1, spiDmaRXQueue1;
uint16_t rx161[BUFFSIZE], tx161[BUFFSIZE];
uint16_t rx16[BUFFSIZE], tx16[BUFFSIZE];
//ROM_DMA_XFERDESC_CFG_T dmaXferCfg;
int dmaCnt, spiCnt1,spiCnt;

/* DMA channel configuration */
		const ROM_DMA_CHAN_CFG_T chanCfg1 = {
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
	


/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Initializes pin muxing for SPI interface - note that SystemInit() may
   already setup your pin muxing at system startup */
static void Init_SPI_PinMux(void)
{
#if defined(BOARD_NXP_LPCXPRESSO_54102)
	/* 0.11 = SPI0_SCK, 0.12 = SPI0_MOSI, 0.13 = SPI0_MISO, 0.14 = SPI0_SSELN0 */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 11, (IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 12, (IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));
//	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 13, (IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 14, (IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));

	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 12, (IOCON_FUNC4 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 13, (IOCON_FUNC4 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));
//	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 13, (IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 15, (IOCON_FUNC4 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));

#else
	/* Configure your own I2C pin muxing here if needed */
#warning "No SPI pin muxing defined"
#endif
}

/* Display error string and spin */
static void errorOut(char *errStr)
{
	DEBUGOUT(errStr);
	//while (1) {}
}
static void spiTranSetup1(ROM_SPIM_HANDLE_T spimHandle, ROM_SPIM_XFER_T *pXfer)
{
	uint8_t flen;
	ROM_DMA_XFERDESC_CFG_T dmaXferCfg;

	/* The DMA needs to be setup for 8- or 16-bit transfers based on the configured
	   SPI transfer size. For the example, we'll get the size from the SPI peripherals
	     config. */
	flen = 1 + (uint8_t) ((LPC_SPIM_PORT1->TXCTRL >> 24) & 0xF);

	dmaXferCfg.src  = (void *) tx161;
	dmaXferCfg.dest = (void *) &LPC_SPIM_PORT1->TXDAT;

	/* Setup source to desination copy for trigger for memory */
	dmaXferCfg.xferCount = BUFFSIZE;					/* Transfer data values of size width */
	dmaXferCfg.swTrig = 1;									/* No software triggering */
	dmaXferCfg.clrTrig = 0;									/* Do not clear trigger after this descriptor completes */
	dmaXferCfg.fireDescCB = 1;							/* Do not fire descriptor callback on complettion of this descriptor */
	dmaXferCfg.enabCirc = 0;
	dmaXferCfg.dmaCh = spiDmaTXQueue1.dmaCh;
	dmaXferCfg.stallDesc = 0;
	if (flen > 8) {
		dmaXferCfg.width = ROM_DMA_WIDTH_2;			/* Width is 2 bytes */
	}
	else {
		dmaXferCfg.width = ROM_DMA_WIDTH_1;			/* Width is 1 byte */
	}
	dmaXferCfg.srcInc = ROM_DMA_ADDRINC_1X;	/* Increment source address by width for each data transfer */
	dmaXferCfg.dstInc = ROM_DMA_ADDRINC_0X;	/* Increment destination address by width for each data transfer */
	if (ROM_DMA_BuildDescriptorChain(dmaHandle, &dmaXferCfg, &dmaSPITXDesc1, NULL) != LPC_OK) {
		errorOut("Error building descriptor chain (SPI TX single link)\r\n");
	}

	/* Queue descriptor for SPI TX transfer */
	ROM_DMA_QueueDescriptor(dmaHandle, &spiDmaTXQueue1, &dmaSPITXDesc1);

	/* Start queue */
	ROM_DMA_StartQueue(dmaHandle, &spiDmaTXQueue1);
}

void spiRecvSetup1(ROM_SPIM_HANDLE_T spimHandle, ROM_SPIM_XFER_T *pXfer)
{
	uint8_t flen;
	ROM_DMA_XFERDESC_CFG_T dmaXferCfg;

	/* The DMA needs to be setup for 8- or 16-bit transfers based on the configured
	   SPI transfer size. For the example, we'll get the size from the SPI peripherals
	     config. */
	flen = 1 + (uint8_t) ((LPC_SPIM_PORT1->TXCTRL >> 24) & 0xF);

	dmaXferCfg.src = (void *) &LPC_SPIM_PORT1->RXDAT;
	dmaXferCfg.dest  = (void *) rx16;

	/* Setup source to desination copy for trigger for memory */
	dmaXferCfg.xferCount = BUFFSIZE;					/* Transfer data values of size width */
	dmaXferCfg.swTrig = 1;									/* No software triggering */
	dmaXferCfg.clrTrig = 0;									/* Do not clear trigger after this descriptor completes */
	dmaXferCfg.fireDescCB = 1;							/* Do not fire descriptor callback on complettion of this descriptor */
	dmaXferCfg.enabCirc = 0;								/* Not a circular buffer */
	dmaXferCfg.dmaCh = spiDmaRXQueue1.dmaCh;
	dmaXferCfg.stallDesc = 0;
	if (flen > 8) {
		dmaXferCfg.width = ROM_DMA_WIDTH_2;			/* Width is 2 bytes */
	}
	else {
		dmaXferCfg.width = ROM_DMA_WIDTH_1;			/* Width is 1 byte */
	}
	dmaXferCfg.srcInc = ROM_DMA_ADDRINC_0X;	/* Increment source address by width for each data transfer */
	dmaXferCfg.dstInc = ROM_DMA_ADDRINC_1X;	/* Increment destination address by width for each data transfer */
	if (ROM_DMA_BuildDescriptorChain(dmaHandle, &dmaXferCfg, &dmaSPIRXDesc1, NULL) != LPC_OK) {
		errorOut("Error building descriptor chain (SPI TX single link)\r\n");
	}

	/* Queue descriptor for SPI TX transfer */
	ROM_DMA_QueueDescriptor(dmaHandle, &spiDmaRXQueue1, &dmaSPIRXDesc1);

	/* Start queue */
	ROM_DMA_StartQueue(dmaHandle, &spiDmaRXQueue1);
}

static void spiTranSetup(ROM_SPIM_HANDLE_T spimHandle, ROM_SPIM_XFER_T *pXfer)
{
	uint8_t flen;
	ROM_DMA_XFERDESC_CFG_T dmaXferCfg;

	/* The DMA needs to be setup for 8- or 16-bit transfers based on the configured
	   SPI transfer size. For the example, we'll get the size from the SPI peripherals
	     config. */
	flen = 1 + (uint8_t) ((LPC_SPIM_PORT->TXCTRL >> 24) & 0xF);

	dmaXferCfg.src  = (void *) tx16;
	dmaXferCfg.dest = (void *) &LPC_SPIM_PORT->TXDAT;

	/* Setup source to desination copy for trigger for memory */
	dmaXferCfg.xferCount = BUFFSIZE;					/* Transfer data values of size width */
	dmaXferCfg.swTrig = 1;									/* No software triggering */
	dmaXferCfg.clrTrig = 0;									/* Do not clear trigger after this descriptor completes */
	dmaXferCfg.fireDescCB = 1;							/* Do not fire descriptor callback on complettion of this descriptor */
	dmaXferCfg.enabCirc = 0;
	dmaXferCfg.dmaCh = spiDmaTXQueue.dmaCh;
	dmaXferCfg.stallDesc = 0;
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

void spiRecvSetup(ROM_SPIM_HANDLE_T spimHandle, ROM_SPIM_XFER_T *pXfer)
{
	uint8_t flen;
	ROM_DMA_XFERDESC_CFG_T dmaXferCfg;

	/* The DMA needs to be setup for 8- or 16-bit transfers based on the configured
	   SPI transfer size. For the example, we'll get the size from the SPI peripherals
	     config. */
	flen = 1 + (uint8_t) ((LPC_SPIM_PORT->TXCTRL >> 24) & 0xF);

	dmaXferCfg.src = (void *) &LPC_SPIM_PORT->RXDAT;
	dmaXferCfg.dest  = (void *) rx16;

	/* Setup source to desination copy for trigger for memory */
	dmaXferCfg.xferCount = BUFFSIZE;					/* Transfer data values of size width */
	dmaXferCfg.swTrig = 1;									/* No software triggering */
	dmaXferCfg.clrTrig = 0;									/* Do not clear trigger after this descriptor completes */
	dmaXferCfg.fireDescCB = 1;							/* Do not fire descriptor callback on complettion of this descriptor */
	dmaXferCfg.enabCirc = 0;								/* Not a circular buffer */
	dmaXferCfg.dmaCh = spiDmaRXQueue.dmaCh;
	dmaXferCfg.stallDesc = 0;
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
void dmaSPIRXTransferComplete(ROM_DMA_HANDLE_T dmaHandle, struct ROM_DMA_QUEUE *pQueue, ROM_DMA_DESC_T *pTranDesc)
{
	/* Receive finishes after transmit, so close the transfer once the DMA has received all the data */
	ROM_SPIM_ClosePendingTransfer(spimHandle);
}
/* DMA controller transfer descriptor chain complete callback */
void dmaSPIRXTransferComplete1(ROM_DMA_HANDLE_T dmaHandle, struct ROM_DMA_QUEUE *pQueue, ROM_DMA_DESC_T *pTranDesc)
{
	/* Receive finishes after transmit, so close the transfer once the DMA has received all the data */
	ROM_SPIM_ClosePendingTransfer(spimHandle1);
}




/* SPI interrupt handler */
void LPC_SPIM_INTHANDLER(void)
{
	spiCnt++;
	ROM_SPIM_TransferHandler(spimHandle);
}
/* SPI interrupt handler */
void LPC_SPIM_INTHANDLER1(void)
{
	spiCnt1++;
	ROM_SPIM_TransferHandler(spimHandle1);
}


/* To keep things transparent, the DMA transfer is setup as part of the
   SPI receive transfer callback */
void CBspiMasterReceiveCB1(ROM_SPIM_HANDLE_T spimHandle, ROM_SPIM_XFER_T *pXfer)
{
	spiRecvSetup1(spimHandle, pXfer);
	pXfer->rxSz = 0;
}

/* To keep things transparent, the DMA transfer is setup as part of the
   SPI transmit transfer callback */
void CBspiMasterTransmitCB1(ROM_SPIM_HANDLE_T spimHandle, ROM_SPIM_XFER_T *pXfer)
{
	spiTranSetup1(spimHandle, pXfer);
	pXfer->txSz = 0;
}

/* To keep things transparent, the DMA transfer is setup as part of the
   SPI receive transfer callback */
void CBspiMasterReceiveCB(ROM_SPIM_HANDLE_T spimHandle, ROM_SPIM_XFER_T *pXfer)
{
	spiRecvSetup(spimHandle, pXfer);
	pXfer->rxSz = 0;
}

/* To keep things transparent, the DMA transfer is setup as part of the
   SPI transmit transfer callback */
void CBspiMasterTransmitCB(ROM_SPIM_HANDLE_T spimHandle, ROM_SPIM_XFER_T *pXfer)
{
	spiTranSetup(spimHandle, pXfer);
	pXfer->txSz = 0;
}

/*****************************************************************************
 * 函 数 名  : RTU_SpiDev_Init
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月22日
 * 函数功能  : 初始化SPI0和SPI0
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void RTU_SpiDevdma_Init()
{
	uint32_t memSize, *devMem;
		ROM_SPIM_INIT_T spimInit;
		ROM_SPIM_XFER_CONFIG_T spimConfig;
		ROM_DMA_INIT_T dmaInit;
		int i;
	
		
		/* Generic Initialization */
		SystemCoreClockUpdate();
		Board_Init();
	
		/* Setup SPI pin muxing, enable SPI clock and reset SPI peripheral */
		Init_SPI_PinMux();
		Chip_Clock_EnablePeriphClock(LPC_SPIM_CLOCK);
		Chip_SYSCON_PeriphReset(LPC_SPIM_RESET);
	
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

	/* Setup SPI1 TX channel for peripheral request and init queue */
		if (ROM_DMA_SetupChannel(dmaHandle, (ROM_DMA_CHAN_CFG_T *) &chanCfg1, LPC_SPI_TX_DMACH1) != LPC_OK) {
			errorOut("Error setting up SPI TX DMA channel\r\n");
		}
		if (ROM_DMA_InitQueue(dmaHandle, LPC_SPI_TX_DMACH1, &spiDmaTXQueue1) != LPC_OK) {
			errorOut("Error initializing SPI TX DMA queue\r\n");
		}
	
		/* Setup SPI1 RX channel for peripheral request and init queue */
		if (ROM_DMA_SetupChannel(dmaHandle, (ROM_DMA_CHAN_CFG_T *) &chanCfg1, LPC_SPI_RX_DMACH1) != LPC_OK) {
			errorOut("Error setting up SPI RX DMA channel\r\n");
		}
		if (ROM_DMA_InitQueue(dmaHandle, LPC_SPI_RX_DMACH1, &spiDmaRXQueue1) != LPC_OK) {
			errorOut("Error initializing SPI RX DMA queue\r\n");
		}
	
		/* Register error, descriptor completion, and descriptor chain completion callbacks for SPI RX channel */
		ROM_DMA_RegisterQueueCallback(dmaHandle, &spiDmaRXQueue1, ROM_DMA_XFERCOMPLETE_CB, (void *) dmaSPIRXTransferComplete1);
	
		
		/* Register error, descriptor completion, and descriptor chain completion callbacks for SPI RX channel */
		ROM_DMA_RegisterQueueCallback(dmaHandle, &spiDmaRXQueue, ROM_DMA_XFERCOMPLETE_CB, (void *) dmaSPIRXTransferComplete);
	
		
		NVIC_EnableIRQ(DMA_IRQn);

}




/*****************************************************************************
 * 函 数 名  : RTU_SpiDev_Init
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月22日
 * 函数功能  : 初始化SPI0和SPI0
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void RTU_Spi1Dev_Init()
{
	uint32_t memSize, *devMem;
		ROM_SPIM_INIT_T spimInit;
		ROM_SPIM_XFER_CONFIG_T spimConfig;
		ROM_DMA_INIT_T dmaInit;
		int i;
	
		
//		/* Generic Initialization */
//		SystemCoreClockUpdate();
//		Board_Init();
//	
		/* Setup SPI pin muxing, enable SPI clock and reset SPI peripheral */
		//Init_SPI_PinMux();
		Chip_Clock_EnablePeriphClock(LPC_SPIM_CLOCK1);
		Chip_SYSCON_PeriphReset(LPC_SPIM_RESET1);
	
		
	
		/******************************************************************/
		/* START OF SPI MASTER SETUP */
		/******************************************************************/
	
		/* Get needed size for SPI driver context memory */
		memSize = ROM_SPIM_GetMemSize();
		if (memSize > sizeof(spiDrvData1)) {
			errorOut("Can't allocate memory for SPI driver context\r\n");
		}
		devMem = spiDrvData1;/* Or just use malloc(memSize) */
	
		/* Initialize driver */
		spimInit.pUserData = NULL;
		spimInit.base = (uint32_t) LPC_SPIM_PORT1;
		spimInit.baseClockRate = Chip_Clock_GetAsyncSyscon_ClockRate();
		spimInit.spiPol[0] = 0; /* Active low select for SSEL0 */
		spimInit.spiPol[1] = 1;
		spimInit.spiPol[2] = 1;
		spimInit.spiPol[3] = 1;
		spimHandle1 = ROM_SPIM_Init(devMem, &spimInit);
		if (spimHandle == NULL) {
			/* Error initializing SPI */
			errorOut("Error initializing ROM\r\n");
		}
	
		/* Set SPI transfer configuration */
		memset(&spimConfig, 0, sizeof(spimConfig));
		spimConfig.dXferBitRate = SPI_BITRATE;
		spimConfig.mode = ROM_SPI_CLOCK_MODE0;
		spimConfig.lsbFirst = 0;
		spimConfig.dataBits = XFERSZ;
		spimConfig.PreDelay = 7;
		spimConfig.PostDelay = 3;
		spimConfig.FrameDelay = 3;
		spimConfig.TransferDelay = 4;
		if (ROM_SPIM_SetupTransfer(spimHandle1, &spimConfig) != LPC_OK) {
			errorOut("SPI configuration is invalid\r\n");
		}
	
		/* Show desired and actual SPI rates */
		DEBUGOUT("SPI rate = %d (actual %d)\r\n", spimConfig.dXferBitRate, spimConfig.rXferBitRate);
	
		ROM_SPIM_RegisterCallback(spimHandle1, ROM_SPIM_DATATRECEIVE_CB, (void *) CBspiMasterReceiveCB1);
		ROM_SPIM_RegisterCallback(spimHandle1, ROM_SPIM_DATATRANSMIT_CB, (void *) CBspiMasterTransmitCB1);
	
		/* Enable the interrupt for the SPI and DMA */
		NVIC_EnableIRQ(LPC_SPIIRQNUM1);

}


/*****************************************************************************
 * 函 数 名  : RTU_SpiDev_Init
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月22日
 * 函数功能  : 初始化SPI0和SPI0
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void RTU_Spi0Dev_Init()
{
	uint32_t memSize, *devMem;
		ROM_SPIM_INIT_T spimInit;
		ROM_SPIM_XFER_CONFIG_T spimConfig;
		ROM_DMA_INIT_T dmaInit;
		int i;
	
		
//		/* Generic Initialization */
//		SystemCoreClockUpdate();
//		Board_Init();
//	
		/* Setup SPI pin muxing, enable SPI clock and reset SPI peripheral */
		//Init_SPI_PinMux();
		Chip_Clock_EnablePeriphClock(LPC_SPIM_CLOCK);
		Chip_SYSCON_PeriphReset(LPC_SPIM_RESET);
	
		
	
		/******************************************************************/
		/* START OF SPI MASTER SETUP */
		/******************************************************************/
	
		/* Get needed size for SPI driver context memory */
		memSize = ROM_SPIM_GetMemSize();
		if (memSize > sizeof(spiDrvData)) {
			errorOut("Can't allocate memory for SPI driver context\r\n");
		}
		devMem = spiDrvData;/* Or just use malloc(memSize) */
	
		/* Initialize driver */
		spimInit.pUserData = NULL;
		spimInit.base = (uint32_t) LPC_SPIM_PORT;
		spimInit.baseClockRate = Chip_Clock_GetAsyncSyscon_ClockRate();
		spimInit.spiPol[0] = 0; /* Active low select for SSEL0 */
		spimInit.spiPol[1] = 1;
		spimInit.spiPol[2] = 1;
		spimInit.spiPol[3] = 1;
		spimHandle = ROM_SPIM_Init(devMem, &spimInit);
		if (spimHandle == NULL) {
			/* Error initializing SPI */
			errorOut("Error initializing ROM\r\n");
		}
	
		/* Set SPI transfer configuration */
		memset(&spimConfig, 0, sizeof(spimConfig));
		spimConfig.dXferBitRate = SPI_BITRATE;
		spimConfig.mode = ROM_SPI_CLOCK_MODE0;
		spimConfig.lsbFirst = 0;
		spimConfig.dataBits = XFERSZ;
		spimConfig.PreDelay = 7;
		spimConfig.PostDelay = 3;
		spimConfig.FrameDelay = 3;
		spimConfig.TransferDelay = 4;
		if (ROM_SPIM_SetupTransfer(spimHandle, &spimConfig) != LPC_OK) {
			errorOut("SPI configuration is invalid\r\n");
		}
	
		/* Show desired and actual SPI rates */
		DEBUGOUT("SPI rate = %d (actual %d)\r\n", spimConfig.dXferBitRate, spimConfig.rXferBitRate);
	
		ROM_SPIM_RegisterCallback(spimHandle, ROM_SPIM_DATATRECEIVE_CB, (void *) CBspiMasterReceiveCB);
		ROM_SPIM_RegisterCallback(spimHandle, ROM_SPIM_DATATRANSMIT_CB, (void *) CBspiMasterTransmitCB);
	
		/* Enable the interrupt for the SPI and DMA */
		NVIC_EnableIRQ(LPC_SPIIRQNUM);

}

/*****************************************************************************
 * 函 数 名  : RTU_SpiDev_Init
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月22日
 * 函数功能  : 初始化SPI0和SPI0
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void RTU_SpiDev_Init()
{
	RTU_SpiDevdma_Init();
	RTU_Spi0Dev_Init();
	RTU_Spi1Dev_Init();



}

uint8_t SPI_Send_One_Data(uint8_t spi_Port,uint8_t tlv5614port,uint32_t data)
{
	SPI_DMA_DEV_T *devptr;
	uint8_t tlv5614head;
	uint16_t *psendbuf,*prxvbuf;
	ROM_SPIM_HANDLE_T tmphandler;
	int *tmpcnt;

	if(spi_Port==0)
	{
		//devptr = &dev_spi[0];
		tmphandler = spimHandle;
		psendbuf = tx16;
		prxvbuf = rx16;
		tlv5614head = 0;
	}
	else
	{//
		//devptr = &dev_spi[1];
		
		tmphandler = spimHandle1;
		psendbuf = tx161;
		
		prxvbuf = rx161;
		tlv5614head = 0x1 <<12; 	//fast 模式，0 slow模式
		tlv5614head |= tlv5614port<<14;
		
	}
	psendbuf[spiCnt1%BUFFSIZE] = (uint16_t)data & tlv5614head; // 组 5614数据格式 A1A0 PWR SPD+12bit data
		/* Setup buffers and sizes, use select 0 for transfer */
		mXfer.txBuff = psendbuf;
		mXfer.txSz = 1;
		mXfer.rxBuff = prxvbuf;
		mXfer.rxSz = BUFFSIZE;
		mXfer.sselNum = 0;

		/* Setup for DMA and loopback mode */
//		devptr->mXfer.flags = ROM_SPIM_FLAG_LOOPBACK | ROM_SPIM_FLAG_DMATX | ROM_SPIM_FLAG_DMARX;
		mXfer.flags = ROM_SPIM_FLAG_DMATX ;

		/* Start transfer. Will return immediately */
		ROM_SPIM_Transfer(tmphandler, &mXfer);

		/* Status will change from BUSY once transfer is complete */
		while (mXfer.status == ERR_SPI_BUSY) {}

		/* Check status of the transfer */
		if (mXfer.status != LPC_OK) {
			DEBUGOUT("-Error performing transfer = %x\r\n", mXfer.status);
		}
		else {
			DEBUGOUT("-SPI transfer completed: status = %x (%d, %d)\r\n", mXfer.status, spiCnt, dmaCnt);
			}
}



