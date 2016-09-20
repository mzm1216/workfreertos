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

#include "board.h"
#include <stdlib.h>
#include "romapi_spim.h"
#include "hw_spimd.h"



extern ROM_DMA_HANDLE_T *hDMA;

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* 3MHz SPI bit-rate */
#define SPI_BITRATE         (3000000)

/* Use SPI0 port */
//#define LPC_SPIM_PORT         LPC_SPI0
//#define LPC_SPIM_CLOCK              SYSCON_CLOCK_SPI0
//#define LPC_SPIM_RESET              RESET_SPI0
//#define LPC_SPIM_INTHANDLER   SPI0_IRQHandler
//#define LPC_SPIIRQNUM         SPI0_IRQn
//#define LPC_SPI_TX_DMACH      ROM_DMAREQ_SPI0_TX
//#define LPC_SPI_RX_DMACH      ROM_DMAREQ_SPI0_RX



#define XFERSZ 16	/* Either 8 or 16, number of data bits */
#define BUFFSIZE    64

#ifdef __ICCARM__
#define ALIGNSTR(x) # x
#define ALIGN(x) _Pragma(ALIGNSTR(data_alignment = ## x))
#else
#define ALIGN(x) __attribute__ ((aligned(x)))
#endif

/* DMA channel configuration */
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
typedef uint16_t (*spiMasterFifoPushFN)(LPC_SPI_T *pSPI, void *data, uint32_t sendBytes, uint32_t curIndex);
typedef uint16_t (*spiMasterFifoPopFN)(LPC_SPI_T *pSPI, void *data, uint32_t bytes, uint32_t curIndex);

typedef struct {
	void                        *pUserData;			/*!< Pointer to user data used by driver instance, use NULL if not used */
	LPC_SPI_T                   *base;				/*!< Base address of SPI peripheral to use */
	uint32_t                    baseClockRate;		/*!< SPI base clock rate in Hz, call Init() again if this rate changes */
	spiMasterXferCSAssertCB     pAssertCb;			/*!< SSEL assertion callback */
	spiMasterTransmitCB         pTranCb;			/*!< Transmit data start callback */
	spiMasterReceiveCB          pRecvCb;			/*!< Receive data start callback */
	spiMMasterXferCSDeAssertCB  pDeassertCb;		/*!< SSEL deassertion callback */
	spiMasterFifoPushFN         pPushFN;			/*!< Pointer to current FIFO push function */
	spiMasterFifoPopFN          pPopFN;				/*!< Pointer to current FIFO pop function */
	ROM_SPIM_XFER_T             *pXfer;				/*!< Pointer to current transfer descriptor */
	uint32_t                    sendIdx;			/*!< Current transmit buffer index */
	uint32_t                    recvIdx;			/*!< Current receive buffer index */
	ErrorCode_t                 pendingStatus;		/*!< Pending transfer status */
	uint8_t                     xmitOn;				/*!< Transfer in progress flag */
	uint8_t                     terminate;			/*!< Terminate transfer flag */
	uint8_t                     reserved[2];
} SPIM_DATACONTEXT_T;

/* Alignement to 512 bytes */
ALIGN(512) ROM_DMA_DESC_T EXChip_DMA_Table[MAX_DMA_CHANNEL];

typedef struct {
	/* ROM driver handle for SPI master and DMA */
	ROM_SPIM_HANDLE_T spimHandle;
	ROM_DMA_HANDLE_T dmaHandle;

	/* SPI and DMA driver context areas */
	uint32_t spiDrvData[16];
	uint32_t dmaDrvData[16];
	
	/* DMA descriptors must be aligned to 16 bytes */
	ALIGN(16)  ROM_DMA_DESC_T dmaSPITXDesc;
	ALIGN(16)  ROM_DMA_DESC_T dmaSPIRXDesc;


	/* Made global to help with debug */
	ROM_SPIM_XFER_T mXfer;
	
	ROM_DMA_QUEUE_T spiDmaTXQueue;
	ROM_DMA_QUEUE_T spiDmaRXQueue;
	
	uint16_t rx16[BUFFSIZE];
	uint16_t tx16[BUFFSIZE];
	
	ROM_DMA_XFERDESC_CFG_T dmaXferCfg;
	int dmaCnt;
	int spiCnt;
} SPI_DMA_DEV_T;

SPI_DMA_DEV_T dev_spi[2];//spi 0 spi1
/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/



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
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 13, (IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 14, (IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));

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

static void spiTranSetup(ROM_SPIM_HANDLE_T spimHandle, ROM_SPIM_XFER_T *pXfer)
{
	uint8_t flen;
	SPIM_DATACONTEXT_T *pDrv = (SPIM_DATACONTEXT_T *) spimHandle;
	SPI_DMA_DEV_T *spi_dev = (SPI_DMA_DEV_T *)pDrv->pUserData;


	/* The DMA needs to be setup for 8- or 16-bit transfers based on the configured
	   SPI transfer size. For the example, we'll get the size from the SPI peripherals
	     config. */
	flen = 1 + (uint8_t) ((pDrv->base->TXCTRL >> 24) & 0xF);

	spi_dev->dmaXferCfg.src  = (void *) spi_dev->tx16;
	spi_dev->dmaXferCfg.dest = (void *) &pDrv->base->TXDAT;

	/* Setup source to desination copy for trigger for memory */
	spi_dev->dmaXferCfg.xferCount = BUFFSIZE;					/* Transfer data values of size width */
	spi_dev->dmaXferCfg.swTrig = 1;									/* No software triggering */
	spi_dev->dmaXferCfg.clrTrig = 0;									/* Do not clear trigger after this descriptor completes */
	spi_dev->dmaXferCfg.fireDescCB = 1;							/* Do not fire descriptor callback on complettion of this descriptor */
	spi_dev->dmaXferCfg.enabCirc = 0;
	spi_dev->dmaXferCfg.dmaCh = spi_dev->spiDmaTXQueue.dmaCh;
	spi_dev->dmaXferCfg.stallDesc = 0;
	if (flen > 8) {
		spi_dev->dmaXferCfg.width = ROM_DMA_WIDTH_2;			/* Width is 2 bytes */
	}
	else {
		spi_dev->dmaXferCfg.width = ROM_DMA_WIDTH_1;			/* Width is 1 byte */
	}
	spi_dev->dmaXferCfg.srcInc = ROM_DMA_ADDRINC_1X;	/* Increment source address by width for each data transfer */
	spi_dev->dmaXferCfg.dstInc = ROM_DMA_ADDRINC_0X;	/* Increment destination address by width for each data transfer */
	if (ROM_DMA_BuildDescriptorChain(spi_dev->dmaHandle, &spi_dev->dmaXferCfg, &spi_dev->dmaSPITXDesc, NULL) != LPC_OK) {
		errorOut("Error building descriptor chain (SPI TX single link)\r\n");
	}

	/* Queue descriptor for SPI TX transfer */
	ROM_DMA_QueueDescriptor(spi_dev->dmaHandle, &spi_dev->spiDmaTXQueue, &spi_dev->dmaSPITXDesc);

	/* Start queue */
	ROM_DMA_StartQueue(spi_dev->dmaHandle, &spi_dev->spiDmaTXQueue);
}

//void spiRecvSetup(ROM_SPIM_HANDLE_T spimHandle, ROM_SPIM_XFER_T *pXfer)
//{
//	uint8_t flen;

//	/* The DMA needs to be setup for 8- or 16-bit transfers based on the configured
//	   SPI transfer size. For the example, we'll get the size from the SPI peripherals
//	     config. */
//	flen = 1 + (uint8_t) ((LPC_SPIM_PORT->TXCTRL >> 24) & 0xF);

//	dmaXferCfg.src = (void *) &LPC_SPIM_PORT->RXDAT;
//	dmaXferCfg.dest  = (void *) rx16;

//	/* Setup source to desination copy for trigger for memory */
//	dmaXferCfg.xferCount = BUFFSIZE;					/* Transfer data values of size width */
//	dmaXferCfg.swTrig = 1;									/* No software triggering */
//	dmaXferCfg.clrTrig = 0;									/* Do not clear trigger after this descriptor completes */
//	dmaXferCfg.fireDescCB = 1;							/* Do not fire descriptor callback on complettion of this descriptor */
//	dmaXferCfg.enabCirc = 0;								/* Not a circular buffer */
//	dmaXferCfg.dmaCh = spiDmaRXQueue.dmaCh;
//	dmaXferCfg.stallDesc = 0;
//	if (flen > 8) {
//		dmaXferCfg.width = ROM_DMA_WIDTH_2;			/* Width is 2 bytes */
//	}
//	else {
//		dmaXferCfg.width = ROM_DMA_WIDTH_1;			/* Width is 1 byte */
//	}
//	dmaXferCfg.srcInc = ROM_DMA_ADDRINC_0X;	/* Increment source address by width for each data transfer */
//	dmaXferCfg.dstInc = ROM_DMA_ADDRINC_1X;	/* Increment destination address by width for each data transfer */
//	if (ROM_DMA_BuildDescriptorChain(dmaHandle, &dmaXferCfg, &dmaSPIRXDesc, NULL) != LPC_OK) {
//		errorOut("Error building descriptor chain (SPI TX single link)\r\n");
//	}

//	/* Queue descriptor for SPI TX transfer */
//	ROM_DMA_QueueDescriptor(dmaHandle, &spiDmaRXQueue, &dmaSPIRXDesc);

//	/* Start queue */
//	ROM_DMA_StartQueue(dmaHandle, &spiDmaRXQueue);
//}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* DMA controller interrupt handler */
void DMA_IRQHandler(void)
{
	//dmaCnt++;
	
	ROM_DMA_DMAHandler(dev_spi[0].dmaHandle);
	ROM_DMA_DMAHandler(dev_spi[1].dmaHandle);
	ROM_DMA_DMAHandler(hDMA);
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
//void dmaSPIRXTransferComplete(ROM_DMA_HANDLE_T dmaHandle, struct ROM_DMA_QUEUE *pQueue, ROM_DMA_DESC_T *pTranDesc)
//{
//	/* Receive finishes after transmit, so close the transfer once the DMA has received all the data */
//	ROM_SPIM_ClosePendingTransfer(spimHandle);
//}

/* DMA controller transfer descriptor complete callback */
//void dmaSPIRXTransferDescComplete(ROM_DMA_HANDLE_T dmaHandle, struct ROM_DMA_QUEUE *pQueue, ROM_DMA_DESC_T *pTranDesc)
//{}

/*DMA controller transfer descriptor error callback */
//void dmaSPIRXTransferError(ROM_DMA_HANDLE_T dmaHandle, struct ROM_DMA_QUEUE *pQueue, ROM_DMA_DESC_T *pTranDesc)
//{}

/* SPI interrupt handler */
void SPI0_IRQHandler(void)
{
	dev_spi[0].spiCnt++;
	ROM_SPIM_TransferHandler(dev_spi[0].spimHandle);
}
/* SPI interrupt handler */
void SPI1_IRQHandler(void)
{
	dev_spi[1].spiCnt++;
	ROM_SPIM_TransferHandler(dev_spi[1].spimHandle);
}

/* To keep things transparent, the DMA transfer is setup as part of the
   SPI receive transfer callback */
void CBspiMasterReceiveCB(ROM_SPIM_HANDLE_T spimHandle, ROM_SPIM_XFER_T *pXfer)
{
//	spiRecvSetup(spimHandle, pXfer);
	pXfer->rxSz = 0;
}

/* To keep things transparent, the DMA transfer is setup as part of the
   SPI transmit transfer callback */
void CBspiMasterTransmitCB(ROM_SPIM_HANDLE_T spimHandle, ROM_SPIM_XFER_T *pXfer)
{
	spiTranSetup(spimHandle, pXfer);
	pXfer->txSz = 0;
}

void CBspiMasterXferCSAssertCB(ROM_SPIM_HANDLE_T spimHandle, ROM_SPIM_XFER_T *pXfer)
{
	/* SPI master assertion event */
}

void CBspiMMasterXferCSDeAssertCB(ROM_SPIM_HANDLE_T spimHandle, ROM_SPIM_XFER_T *pXfer)
{
	/* SPI master de-assertion event */
}

/*****************************************************************************
 * 函 数 名  : HW_spi_Dma_init_by_spi_port
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月23日
 * 函数功能  : 初始化SPI的ＤＭＡ外设功能，1、填充DMA-DATACONTEXT-T 结构体，2
               、将cfg配置信息配置到相应通道的配置寄存器。3、初始化对应通道
               的DMA队列，4、由于rtu不存在接收，所以配置时将只配置发送。
 * 输入参数  : uint32_t spi_Port  spi端口号
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void HW_spi_Dma_init_by_spi_port(uint32_t spi_Port)
{
	uint32_t memSize, *devMem;
	
	ROM_DMA_INIT_T dmaInit;
	SPI_DMA_DEV_T *devptr;
	uint8_t dma_tx_ch,dma_rx_ch;

	if(spi_Port==0)
	{
		devptr = &dev_spi[0];
		dma_tx_ch =  ROM_DMAREQ_SPI0_TX;
//		dma_rx_ch =  ROM_DMAREQ_SPI0_RX
	}
	else
	{
		devptr = &dev_spi[1];
		dma_tx_ch =  ROM_DMAREQ_SPI1_TX;
//		dma_rx_ch =  ROM_DMAREQ_SPI1_RX
	}

	/* Enable DMA clocking prior to calling DMA init functions */
	Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_DMA);

	/* Get needed size for DMA driver context memory */
	memSize = ROM_DMA_GetMemSize();
	if (memSize > sizeof(devptr->dmaDrvData)) {
		errorOut("Can't allocate memory for DMA driver context\r\n");
	}
	devMem = devptr->dmaDrvData;/* Or just use malloc(memSize) */

	/* Initialize DMA driver */
	dmaInit.pUserData = (void *) devptr;
	dmaInit.base = (uint32_t) LPC_DMA;
	dmaInit.sramBase = (uint32_t) EXChip_DMA_Table;
	devptr->dmaHandle = ROM_DMA_Init(devMem, &dmaInit);
	if (devptr->dmaHandle == NULL) {
		errorOut("Error initializing DMA\r\n");
	}

	/* Setup SPI1 TX channel for peripheral request and init queue */
	if (ROM_DMA_SetupChannel(devptr->dmaHandle, (ROM_DMA_CHAN_CFG_T *) &chanCfg, dma_tx_ch) != LPC_OK) {
		errorOut("Error setting up SPI TX DMA channel\r\n");
	}
	if (ROM_DMA_InitQueue(devptr->dmaHandle, dma_tx_ch, &devptr->spiDmaTXQueue) != LPC_OK) {
		errorOut("Error initializing SPI TX DMA queue\r\n");
	}

//	/* Setup SPI1 RX channel for peripheral request and init queue */
//	if (ROM_DMA_SetupChannel(devptr->dmaHandle, (ROM_DMA_CHAN_CFG_T *) &chanCfg, dma_rx_ch) != LPC_OK) {
//		errorOut("Error setting up SPI RX DMA channel\r\n");
//	}
//	if (ROM_DMA_InitQueue(devptr->dmaHandle, dma_rx_ch, &devptr->spiDmaRXQueue) != LPC_OK) {
//		errorOut("Error initializing SPI RX DMA queue\r\n");
//	}

	/* Register error, descriptor completion, and descriptor chain completion callbacks for SPI TX channel */
	ROM_DMA_RegisterQueueCallback(	devptr->dmaHandle,
									&devptr->spiDmaTXQueue, 
									ROM_DMA_XFERCOMPLETE_CB,
									(void *) dmaSPITXTransferComplete);
	ROM_DMA_RegisterQueueCallback(	devptr->dmaHandle,
									  &devptr->spiDmaTXQueue,
									  ROM_DMA_XFERDESCCOMPLETE_CB,
									  (void *) dmaSPITXTransferDescComplete);
	ROM_DMA_RegisterQueueCallback(	devptr->dmaHandle, 
									&devptr->spiDmaTXQueue, 
									ROM_DMA_XFERERROR_CB, 
									(void *) dmaSPITXTransferError);

//	/* Register error, descriptor completion, and descriptor chain completion callbacks for SPI RX channel */
//	ROM_DMA_RegisterQueueCallback(	devptr->dmaHandle,
//									&devptr->spiDmaRXQueue, 
//									ROM_DMA_XFERCOMPLETE_CB, 
//									(void *) dmaSPIRXTransferComplete);
//	ROM_DMA_RegisterQueueCallback(	devptr->dmaHandle,
//								 	&devptr->spiDmaRXQueue,
//								  	ROM_DMA_XFERDESCCOMPLETE_CB,
//								  	(void *) dmaSPIRXTransferDescComplete);
//	ROM_DMA_RegisterQueueCallback(	devptr->dmaHandle,
//									&devptr->spiDmaRXQueue, 
//									ROM_DMA_XFERERROR_CB, 
//									(void *) dmaSPIRXTransferError);

}


/*****************************************************************************
 * 函 数 名  : HW_spi_init_by_port
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月22日
 * 函数功能  : 根据输入的端口号进行spi初始化，采用中断方式进行数据发送
 * 输入参数  : uint32_t spi_Port  spi端口：0---spi0,1----spi1
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : RTU_SpiDev_Init
 * 其    它  : 

*****************************************************************************/
void HW_spi_init_by_port(uint32_t spi_Port)
{
	uint32_t memSize, *devMem,portbase;
	ROM_SPIM_INIT_T spimInit;
	ROM_SPIM_XFER_CONFIG_T spimConfig;
	IRQn_Type irqno;

	SPI_DMA_DEV_T *devptr;

	if(spi_Port==0)
	{
		Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_SPI0);
		Chip_SYSCON_PeriphReset(RESET_SPI0);
		devptr = &dev_spi[0];
		portbase = (uint32_t)LPC_SPI0;
		irqno = SPI0_IRQn;
	}
	else
	{
		Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_SPI1);
		Chip_SYSCON_PeriphReset(RESET_SPI1);
		
		devptr = &dev_spi[1];
		portbase = (uint32_t)LPC_SPI1;
		irqno = SPI1_IRQn;
	}

	
	memSize = ROM_SPIM_GetMemSize();
	if (memSize > sizeof(devptr->spiDrvData))
	{
			errorOut("Can't allocate memory for driver context\r\n");
			return;
	}
	devMem = devptr->spiDrvData;	/* Or just use malloc(memSize) */
	
	/* Initialize driver */
	spimInit.pUserData = (void*)devptr;
	spimInit.base = (uint32_t) portbase;
	spimInit.baseClockRate = Chip_Clock_GetAsyncSyscon_ClockRate();
	spimInit.spiPol[0] = 0; /* Active low select for SSEL0 */
	spimInit.spiPol[1] = 1;
	spimInit.spiPol[2] = 1;
	spimInit.spiPol[3] = 1;
	devptr->spimHandle = ROM_SPIM_Init(devMem, &spimInit);
	
	if (devptr->spimHandle == NULL) {
		/* Error initializing SPI */
		errorOut("Error initializing ROM\r\n");
		return;
	}

	/* Set SPI transfer configuration */
	
	memset(&spimConfig, 0, sizeof(spimConfig));
	spimConfig.dXferBitRate = SPI_BITRATE;
	spimConfig.mode = ROM_SPI_CLOCK_CPHA0_CPOL0;
	spimConfig.lsbFirst = 0;
	spimConfig.dataBits = 16;
	spimConfig.PreDelay = 7;
	spimConfig.PostDelay = 3;
	spimConfig.FrameDelay = 3;
	spimConfig.TransferDelay = 4;
	if (ROM_SPIM_SetupTransfer(devptr->spimHandle, &spimConfig) != LPC_OK) {
		errorOut("SPI configuration is invalid\r\n");
		return;
	}

	/* Show desired and actual SPI rates */
	DEBUGOUT("SPI rate = %d (actual %d)\r\n", spimConfig.dXferBitRate, spimConfig.rXferBitRate);

	/* Callback registration  */
	ROM_SPIM_RegisterCallback(devptr->spimHandle, ROM_SPIM_ASSERTSSEL_CB, (void *) CBspiMasterXferCSAssertCB);
	ROM_SPIM_RegisterCallback(devptr->spimHandle, ROM_SPIM_DEASSERTSSEL_CB, (void *) CBspiMMasterXferCSDeAssertCB);
//	ROM_SPIM_RegisterCallback(devptr->spimHandle, ROM_SPIM_DATATRECEIVE_CB, (void *) CBspiMasterReceiveCB);
	ROM_SPIM_RegisterCallback(devptr->spimHandle, ROM_SPIM_DATATRANSMIT_CB, (void *) CBspiMasterTransmitCB);

	/* Enable the interrupt for the SPI */
	

	NVIC_EnableIRQ(DMA_IRQn);
	NVIC_EnableIRQ(irqno);

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
	
	HW_spi_Dma_init_by_spi_port(0);//初始化dma
	HW_spi_init_by_port(0);  //初始化spi0
	
	HW_spi_Dma_init_by_spi_port(1);//初始化dma
	HW_spi_init_by_port(1);	//初始化spi1
}

/**
 * @brief	Main routine for I2C example
 * @return	Function should not exit
 */
int main12(void)
{
//	uint32_t memSize, *devMem;
//	ROM_SPIM_INIT_T spimInit;
//	ROM_SPIM_XFER_CONFIG_T spimConfig;
//	ROM_DMA_INIT_T dmaInit;
//	int i;


//	/* Generic Initialization */
//	SystemCoreClockUpdate();
//	Board_Init();

//	/* Setup SPI pin muxing, enable SPI clock and reset SPI peripheral */
//	Init_SPI_PinMux();
//	Chip_Clock_EnablePeriphClock(LPC_SPIM_CLOCK);
//	Chip_SYSCON_PeriphReset(LPC_SPIM_RESET);

//	/******************************************************************/
//	/* START OF DMA SETUP */
//	/******************************************************************/

//	/* Enable DMA clocking prior to calling DMA init functions */
//	Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_DMA);

//	/* Get needed size for DMA driver context memory */
//	memSize = ROM_DMA_GetMemSize();
//	if (memSize > sizeof(dmaDrvData)) {
//		errorOut("Can't allocate memory for DMA driver context\r\n");
//	}
//	devMem = dmaDrvData;/* Or just use malloc(memSize) */

//	/* Initialize DMA driver */
//	dmaInit.pUserData = (void *) NULL;
//	dmaInit.base = (uint32_t) LPC_DMA;
//	dmaInit.sramBase = (uint32_t) EXChip_DMA_Table;
//	dmaHandle = ROM_DMA_Init(devMem, &dmaInit);
//	if (dmaHandle == NULL) {
//		errorOut("Error initializing DMA\r\n");
//	}

//	/* Setup SPI1 TX channel for peripheral request and init queue */
//	if (ROM_DMA_SetupChannel(dmaHandle, (ROM_DMA_CHAN_CFG_T *) &chanCfg, LPC_SPI_TX_DMACH) != LPC_OK) {
//		errorOut("Error setting up SPI TX DMA channel\r\n");
//	}
//	if (ROM_DMA_InitQueue(dmaHandle, LPC_SPI_TX_DMACH, &spiDmaTXQueue) != LPC_OK) {
//		errorOut("Error initializing SPI TX DMA queue\r\n");
//	}

//	/* Setup SPI1 RX channel for peripheral request and init queue */
//	if (ROM_DMA_SetupChannel(dmaHandle, (ROM_DMA_CHAN_CFG_T *) &chanCfg, LPC_SPI_RX_DMACH) != LPC_OK) {
//		errorOut("Error setting up SPI RX DMA channel\r\n");
//	}
//	if (ROM_DMA_InitQueue(dmaHandle, LPC_SPI_RX_DMACH, &spiDmaRXQueue) != LPC_OK) {
//		errorOut("Error initializing SPI RX DMA queue\r\n");
//	}

//	/* Register error, descriptor completion, and descriptor chain completion callbacks for SPI TX channel */
//	ROM_DMA_RegisterQueueCallback(dmaHandle, &spiDmaTXQueue, ROM_DMA_XFERCOMPLETE_CB, (void *) dmaSPITXTransferComplete);
//	ROM_DMA_RegisterQueueCallback(dmaHandle,
//								  &spiDmaTXQueue,
//								  ROM_DMA_XFERDESCCOMPLETE_CB,
//								  (void *) dmaSPITXTransferDescComplete);
//	ROM_DMA_RegisterQueueCallback(dmaHandle, &spiDmaTXQueue, ROM_DMA_XFERERROR_CB, (void *) dmaSPITXTransferError);

//	/* Register error, descriptor completion, and descriptor chain completion callbacks for SPI RX channel */
//	ROM_DMA_RegisterQueueCallback(dmaHandle, &spiDmaRXQueue, ROM_DMA_XFERCOMPLETE_CB, (void *) dmaSPIRXTransferComplete);
//	ROM_DMA_RegisterQueueCallback(dmaHandle,
//								  &spiDmaRXQueue,
//								  ROM_DMA_XFERDESCCOMPLETE_CB,
//								  (void *) dmaSPIRXTransferDescComplete);
//	ROM_DMA_RegisterQueueCallback(dmaHandle, &spiDmaRXQueue, ROM_DMA_XFERERROR_CB, (void *) dmaSPIRXTransferError);

//	/******************************************************************/
//	/* START OF SPI MASTER SETUP */
//	/******************************************************************/

//	/* Get needed size for SPI driver context memory */
//	memSize = ROM_SPIM_GetMemSize();
//	if (memSize > sizeof(spiDrvData)) {
//		errorOut("Can't allocate memory for SPI driver context\r\n");
//	}
//	devMem = spiDrvData;/* Or just use malloc(memSize) */

//	/* Initialize driver */
//	spimInit.pUserData = NULL;
//	spimInit.base = (uint32_t) LPC_SPIM_PORT;
//	spimInit.baseClockRate = Chip_Clock_GetAsyncSyscon_ClockRate();
//	spimInit.spiPol[0] = 0;	/* Active low select for SSEL0 */
//	spimInit.spiPol[1] = 1;
//	spimInit.spiPol[2] = 1;
//	spimInit.spiPol[3] = 1;
//	spimHandle = ROM_SPIM_Init(devMem, &spimInit);
//	if (spimHandle == NULL) {
//		/* Error initializing SPI */
//		errorOut("Error initializing ROM\r\n");
//	}

//	/* Set SPI transfer configuration */
//	memset(&spimConfig, 0, sizeof(spimConfig));
//	spimConfig.dXferBitRate = SPI_BITRATE;
//	spimConfig.mode = ROM_SPI_CLOCK_MODE0;
//	spimConfig.lsbFirst = 0;
//	spimConfig.dataBits = XFERSZ;
//	spimConfig.PreDelay = 7;
//	spimConfig.PostDelay = 3;
//	spimConfig.FrameDelay = 3;
//	spimConfig.TransferDelay = 4;
//	if (ROM_SPIM_SetupTransfer(spimHandle, &spimConfig) != LPC_OK) {
//		errorOut("SPI configuration is invalid\r\n");
//	}

//	/* Show desired and actual SPI rates */
//	DEBUGOUT("SPI rate = %d (actual %d)\r\n", spimConfig.dXferBitRate, spimConfig.rXferBitRate);

//	/* Callback registration  */
//	ROM_SPIM_RegisterCallback(spimHandle, ROM_SPIM_ASSERTSSEL_CB, (void *) CBspiMasterXferCSAssertCB);
//	ROM_SPIM_RegisterCallback(spimHandle, ROM_SPIM_DEASSERTSSEL_CB, (void *) CBspiMMasterXferCSDeAssertCB);
//	ROM_SPIM_RegisterCallback(spimHandle, ROM_SPIM_DATATRECEIVE_CB, (void *) CBspiMasterReceiveCB);
//	ROM_SPIM_RegisterCallback(spimHandle, ROM_SPIM_DATATRANSMIT_CB, (void *) CBspiMasterTransmitCB);

//	/* Enable the interrupt for the SPI and DMA */
//	NVIC_EnableIRQ(DMA_IRQn);
//	NVIC_EnableIRQ(LPC_SPIIRQNUM);

//	/* Read data as fast as possible in loop */
//	while (1) {
//		/* Populate some TX data and clear RX data */
//		for (i = 0; i < BUFFSIZE; i++) {
//			tx16[i] = i + 1 + ((i + 1) << 8);
//			rx16[i] =  0;
//		}

//		/* Setup buffers and sizes, use select 0 for transfer */
//		mXfer.txBuff = tx16;
//		mXfer.txSz = BUFFSIZE;
//		mXfer.rxBuff = rx16;
//		mXfer.rxSz = BUFFSIZE;
//		mXfer.sselNum = 0;

//		/* Setup for DMA and loopback mode */
//		mXfer.flags = ROM_SPIM_FLAG_LOOPBACK | ROM_SPIM_FLAG_DMATX | ROM_SPIM_FLAG_DMARX;
//		mXfer.flags = ROM_SPIM_FLAG_LOOPBACK | ROM_SPIM_FLAG_DMATX ;

//		/* Start transfer. Will return immediately */
//		ROM_SPIM_Transfer(spimHandle, &mXfer);

//		/* Status will change from BUSY once transfer is complete */
//		while (mXfer.status == ERR_SPI_BUSY) {}

//		/* Check status of the transfer */
//		if (mXfer.status != LPC_OK) {
//			DEBUGOUT("-Error performing transfer = %x\r\n", mXfer.status);
//		}
//		else {
//			DEBUGOUT("-SPI transfer completed: status = %x (%d, %d)\r\n", mXfer.status, spiCnt, dmaCnt);
//			for (i = 0; i < BUFFSIZE; i++) {
//#if XFERSZ > 8
//				DEBUGOUT("%04x %04x : ", tx16[i], rx16[i]);
//#else
//				DEBUGOUT("%02x %02x : ", tx16[i], tx16[i]);
//#endif
//			}
//		}
//		spiCnt = dmaCnt = 0;
//	}

//	/* Code never reaches here. Only used to satisfy standard main() */
	return 0;
}


