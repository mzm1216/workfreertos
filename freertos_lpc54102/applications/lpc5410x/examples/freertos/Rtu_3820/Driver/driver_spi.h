#ifndef _DRIVER_SPI_h
#define _DRIVER_SPI_h

#ifdef  SPI_GLOBALS
#define spi_ext
#else
#define spi_ext  extern
#endif

#include "board.h"
#include "config.h"


/* 3MHz SPI bit-rate */
//#define SPI_BITRATE         (20000000)
//#define SPI_BITRATE         (7680000/4)
#define SPI0_TXMOD_BITRATE         (2000000)
#define SPI1_DAC_BITRATE         (8000000)
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

#define CH_SPI0					0
#define CH_SPI1					1
#define CH_TX_PLL_MOD_PORT		CH_SPI0
#define CH_SPI_DAC_PORT			CH_SPI1	
#define FIRST_SEND_CH				CH_TX_PLL_MOD_PORT
#define SECOND_SEND_CH			CH_SPI_DAC_PORT
/* SPI and DMA driver context areas */
static uint32_t spiDrvData[16];
static uint32_t spiDrvData1[16];
static uint32_t dmaDrvData[16];

#define XFERSZ 16	/* Either 8 or 16, number of data bits */
#define BUFFSIZE    64

#define DMA_SPI_DATA_BUF_A  		0
#define DMA_SPI_DATA_BUF_B  		1

#define DMA_SPI_DATA_MAX_NUM		RTU_PING_PONG_BUF_MAX_NUM		// spi搬移数据 个数
#define SPI_DAC_A	0		//DAC--A通道
#define SPI_DAC_B	1//DAC--B通道
#define SPI_DAC_C	2//DAC--C通道
#define SPI_DAC_D	3//DAC--D通道


/* DMA descriptors must be aligned to 16 bytes */
ALIGN(16) static ROM_DMA_DESC_T dmaSPITXDesc;
ALIGN(16) static ROM_DMA_DESC_T dmaSPIRXDesc;

ALIGN(16) static ROM_DMA_DESC_T dmaSPITXDesc1,dmaSPITXDesc11;
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
uint16_t rx1611[BUFFSIZE], tx1611[BUFFSIZE];
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
 * Private types/enumerations/variables
 ****************************************************************************/

/* 3MHz SPI bit-rate */
//#define SPI_BITRATE         (3000000)

#define XFERSZ 16	/* Either 8 or 16, number of data bits */
#define BUFFSIZE    64




/* DMA channel configuration */
static const  ROM_DMA_CHAN_CFG_T chanCfg = {
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
spi_ext ALIGN(512)  ROM_DMA_DESC_T EXChip_DMA_Table[MAX_DMA_CHANNEL];

typedef struct {
	/* ROM driver handle for SPI master and DMA */
	ROM_SPIM_HANDLE_T spimHandle;

	/* SPI and DMA driver context areas */
	SPIM_DATACONTEXT_T spiDrvData;
	
	/* DMA descriptors must be aligned to 16 bytes */
	ALIGN(16)  ROM_DMA_DESC_T dmaSPITXDesc;
	ALIGN(16)  ROM_DMA_DESC_T dmaSPITXDesc1;
	ALIGN(16)  ROM_DMA_DESC_T dmaSPIRXDesc;


	/* Made global to help with debug */
	ROM_SPIM_XFER_T mXfer;
	
	ROM_DMA_QUEUE_T spiDmaTXQueue;
	ROM_DMA_QUEUE_T spiDmaRXQueue;
	
	uint16_t rx16[BUFFSIZE];
	uint16_t tx16[BUFFSIZE];
	uint16_t tx161[BUFFSIZE];
    void *ping_pong_buf_spi;

	uint32_t spiCnt;
} RTU_SPI_DMA_DEV_T;

spi_ext RTU_PING_PONG_SPI1_DATA_T rtu_spi1_pp;	//spi1结构-dma操作搬移4个通道的dac 
spi_ext RTU_PING_PONG_DATA_T rtu_spi0_pp;		//spi0 结构 dma搬移1个通道数据到txmod

spi_ext RTU_SPI_DMA_DEV_T rtu_spi_dev[2];//spi 0 spi1
uint8_t SPI_Send_One_Data(uint8_t spi_Port,uint8_t tlv5614port,uint32_t data);
#endif
