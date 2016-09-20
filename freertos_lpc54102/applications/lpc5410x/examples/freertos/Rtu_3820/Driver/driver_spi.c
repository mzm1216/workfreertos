/***********************************************************************************
 * 文 件 名   : driver_spi.c
 * 负 责 人   : MZM
 * 创建日期   : 2016年6月5日
 * 文件描述   : spidma驱动代码，首先启动dma-spi1，然后启动timer2延时一段时间
                ，再启动dma-spi0
 * 版权说明   : Copyright (c) 2008-2016   北京市万格数码通讯科技有限公司
 * 其    他   : 
 * 修改日志   : 
***********************************************************************************/

#define SPI_GLOBALS
#undef ADC_GLOBALS

#include "board.h"
#include <stdlib.h>
#include "romapi_spim.h"
#include "hw_spimd.h"
#include "driver_spi.h"
#include "driver_adc.h"

#include "ioconfig.h"

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Initializes pin muxing for SPI interface - note that SystemInit() may
   already setup your pin muxing at system startup */
 void Init_SPI_PinMux_Func(void)
{
#ifndef RTU_IO_SPI0
	/* 0.11 = SPI0_SCK, 0.12 = SPI0_MOSI, 0.13 = SPI0_MISO, 0.14 = SPI0_SSELN0 */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 11, (IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 12, (IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));
//	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 13, (IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 14, (IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));
#else
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 11, (IOCON_FUNC0 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 12, (IOCON_FUNC0 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));
//	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 13, (IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 14, (IOCON_FUNC0 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));

#endif
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 12, (IOCON_FUNC4 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 13, (IOCON_FUNC4 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));
//	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 13, (IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 15, (IOCON_FUNC4 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));

}
 void Init_SPI_PinMux_IO(void)
{
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 11, (IOCON_FUNC0 | IOCON_DIGITAL_EN |IOCON_MODE_INACT));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 12, (IOCON_FUNC0 | IOCON_DIGITAL_EN |IOCON_MODE_INACT));
//	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 13, (IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 14, (IOCON_FUNC0 | IOCON_DIGITAL_EN |IOCON_MODE_INACT));
	spi0_io_init();
}

/* Display error string and spin */
static void errorOut(char *errStr)
{
	DEBUGOUT(errStr);
	//while (1) {}
}

/*****************************************************************************
 * 函 数 名  : VT_Get_Spi_Dev
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月1日
 * 函数功能  : 通过传入的端口号，获取spi设备指针，错误端口号将返回NULL
 * 输入参数  : uint8_t port  SPI的端口号 ：0==spi0,1==spi1
 * 输出参数  : 无
 * 返 回 值  : RTU_SPI_DMA_DEV_T
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
RTU_SPI_DMA_DEV_T * VT_Get_Spi_Dev(uint8_t port)
{
	if(port>1)//端口非法
	{
		return NULL;
	}
	
	return &rtu_spi_dev[port];//返回spi0设备指针
}

void * Get_spi0_ping_pong_buf()
{
#ifdef MALLOC_PING_PONG
			return(RTU_PING_PONG_DATA_P *)&rtu_spi_dev[0].ping_pong_buf_spi;
#else
			return (void*)&rtu_spi0_pp;
#endif

}

void * Get_spi1_ping_pong_buf()
{
#ifdef MALLOC_PING_PONG
				return(RTU_PING_PONG_DATA_P *)&rtu_spi_dev[0].ping_pong_buf_spi;
#else
				return (void*)&rtu_spi1_pp;
#endif
}

void spi0_spi1_dma_start_queue(uint8_t spi_port)
{
	RTU_SPI_DMA_DEV_T *device_spi_1;
	
	device_spi_1 = VT_Get_Spi_Dev(spi_port);//获取设备指针
	
	/* Start queue */
	ROM_DMA_StartQueue(dmaHandle, &device_spi_1->spiDmaTXQueue);

}


/*****************************************************************************
 * 函 数 名  : spiTranSetup_SPI1
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月1日
 * 函数功能  : SPI1设备的DMA设备发送的初始化，初始化Queue，并启动dma
 * 输入参数  : ROM_SPIM_HANDLE_T spimHandle  SPI设备系统环境指针
               ROM_SPIM_XFER_T *pXfer        发送接收缓冲区 结构体指针
 * 输出参数  : 无
 * 返 回 值  : static
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
static void spiTranSetup_SPI1(ROM_SPIM_HANDLE_T spimHandle, ROM_SPIM_XFER_T *pXfer)
{
	uint8_t flen;
	ROM_DMA_XFERDESC_CFG_T dmaXferCfg;
	RTU_SPI_DMA_DEV_T *device_spi_1;
			
	device_spi_1 = VT_Get_Spi_Dev(CH_SPI1);//获取设备指针

	/* The DMA needs to be setup for 8- or 16-bit transfers based on the configured
	   SPI transfer size. For the example, we'll get the size from the SPI peripherals
	     config. */
	flen = 1 + (uint8_t) ((LPC_SPIM_PORT1->TXCTRL >> 24) & 0xF);

	dmaXferCfg.src  = (void *) rtu_spi1_pp.ping_pong_buf[0].ping_pong_buf_4K;
	dmaXferCfg.dest = (void *) &LPC_SPIM_PORT1->TXDAT;

//	rt_kprintf(1,"spi1\r\n");
	/* Setup source to desination copy for trigger for memory */
//	dmaXferCfg.xferCount = device_spi_1->spiCnt;					/* Transfer data values of size width */
	dmaXferCfg.xferCount = RTU_PING_PONG_BUF_MAX_NUM*4;					/* Transfer data values of size width */
	dmaXferCfg.swTrig = 1;									/* No software triggering */
	dmaXferCfg.clrTrig = 0;									/* Do not clear trigger after this descriptor completes */
	dmaXferCfg.fireDescCB = 1;							/* Do not fire descriptor callback on complettion of this descriptor */
	dmaXferCfg.enabCirc = 1;
	dmaXferCfg.dmaCh = device_spi_1->spiDmaTXQueue.dmaCh;
	dmaXferCfg.stallDesc = 0;
	device_spi_1->spiCnt = 0;//qing 0
	
	if (flen > 8)
	{
		dmaXferCfg.width = ROM_DMA_WIDTH_2;			/* Width is 2 bytes */
	}
	else 
	{
		dmaXferCfg.width = ROM_DMA_WIDTH_1;			/* Width is 1 byte */
	}
	dmaXferCfg.srcInc = ROM_DMA_ADDRINC_1X;	/* Increment source address by width for each data transfer */
	dmaXferCfg.dstInc = ROM_DMA_ADDRINC_0X;	/* Increment destination address by width for each data transfer */
#if 0
	if (ROM_DMA_BuildDescriptorChain(dmaHandle, &dmaXferCfg, &device_spi_1->dmaSPITXDesc, NULL) != LPC_OK) {
//		errorOut("Error building descriptor chain (SPI TX single link)\r\n");
	}

#else
//	
	if (ROM_DMA_BuildDescriptorChain(dmaHandle, &dmaXferCfg, &device_spi_1->dmaSPITXDesc, &device_spi_1->dmaSPITXDesc1) != LPC_OK) {
		errorOut("Error building descriptor chain (SPI TX single link)\r\n");
	}

	dmaXferCfg.src  = (void *) rtu_spi1_pp.ping_pong_buf[1].ping_pong_buf_4K;
	if (ROM_DMA_BuildDescriptorChain(dmaHandle, &dmaXferCfg, &device_spi_1->dmaSPITXDesc1, &device_spi_1->dmaSPITXDesc) != LPC_OK) {
		errorOut("Error building descriptor chain (SPI TX single link)\r\n");
	}
#endif
	/* Queue descriptor for SPI TX transfer */
	ROM_DMA_QueueDescriptor(dmaHandle, &device_spi_1->spiDmaTXQueue, &device_spi_1->dmaSPITXDesc);

//	/* Start queue */
//	ROM_DMA_StartQueue(dmaHandle, &device_spi_1->spiDmaTXQueue);


}



/*****************************************************************************
 * 函 数 名  : spiRecvSetup_SPI1
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月1日
 * 函数功能  : 初始化spi1的DMA接收：初始化DMA的接收Queue，并启动DMA接收
 * 输入参数  : ROM_SPIM_HANDLE_T spimHandle  SPI1设备系统环境指针
               ROM_SPIM_XFER_T *pXfer        发送接收缓冲区结构指针
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void spiRecvSetup_SPI1(ROM_SPIM_HANDLE_T spimHandle, ROM_SPIM_XFER_T *pXfer)
{
	uint8_t flen;
	ROM_DMA_XFERDESC_CFG_T dmaXferCfg;
	RTU_SPI_DMA_DEV_T *device_spi_1;
			
	device_spi_1 = VT_Get_Spi_Dev(1);//获取 spi1 设备
	/* The DMA needs to be setup for 8- or 16-bit transfers based on the configured
	   SPI transfer size. For the example, we'll get the size from the SPI peripherals
	     config. */
	flen = 1 + (uint8_t) ((LPC_SPIM_PORT1->TXCTRL >> 24) & 0xF);

	dmaXferCfg.src = (void *) &LPC_SPIM_PORT1->RXDAT;
	dmaXferCfg.dest  = (void *) device_spi_1->rx16;

	/* Setup source to desination copy for trigger for memory */
	dmaXferCfg.xferCount =  pXfer->rxSz;					/* Transfer data values of size width */
	dmaXferCfg.swTrig = 1;									/* No software triggering */
	dmaXferCfg.clrTrig = 0;									/* Do not clear trigger after this descriptor completes */
	dmaXferCfg.fireDescCB = 1;							/* Do not fire descriptor callback on complettion of this descriptor */
	dmaXferCfg.enabCirc = 0;								/* Not a circular buffer */
	dmaXferCfg.dmaCh = device_spi_1->spiDmaRXQueue.dmaCh;
	dmaXferCfg.stallDesc = 0;
	
	if (flen > 8) 
	{
		dmaXferCfg.width = ROM_DMA_WIDTH_2;			/* Width is 2 bytes */
	}
	else
	{
		dmaXferCfg.width = ROM_DMA_WIDTH_1;			/* Width is 1 byte */
	}
	
	dmaXferCfg.srcInc = ROM_DMA_ADDRINC_0X;	/* Increment source address by width for each data transfer */
	dmaXferCfg.dstInc = ROM_DMA_ADDRINC_1X;	/* Increment destination address by width for each data transfer */
	
	if (ROM_DMA_BuildDescriptorChain(dmaHandle, &dmaXferCfg, &device_spi_1->dmaSPIRXDesc, NULL) != LPC_OK) {
		errorOut("Error building descriptor chain (SPI TX single link)\r\n");
	}

	/* Queue descriptor for SPI TX transfer */
	ROM_DMA_QueueDescriptor(dmaHandle, &device_spi_1->spiDmaRXQueue, &device_spi_1->dmaSPIRXDesc);

	/* Start queue */
	ROM_DMA_StartQueue(dmaHandle, &device_spi_1->spiDmaRXQueue);
}

/*****************************************************************************
 * 函 数 名  : spiTranSetup_SPI0
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月1日
 * 函数功能  : SPI0设备的DMA设备发送的初始化，初始化Queue，并启动dma
 * 输入参数  : ROM_SPIM_HANDLE_T spimHandle  SPI设备系统环境指针
               ROM_SPIM_XFER_T *pXfer        发送接收缓冲区 结构体指针
 * 输出参数  : 无
 * 返 回 值  : static
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
static void spiTranSetup_SPI0(ROM_SPIM_HANDLE_T spimHandle, ROM_SPIM_XFER_T *pXfer)
{
	uint8_t flen;
	ROM_DMA_XFERDESC_CFG_T dmaXferCfg;
	RTU_SPI_DMA_DEV_T *device_spi_0;
			
	device_spi_0 = VT_Get_Spi_Dev(CH_SPI0);

	/* The DMA needs to be setup for 8- or 16-bit transfers based on the configured
	   SPI transfer size. For the example, we'll get the size from the SPI peripherals
	     config. */
	flen = 1 + (uint8_t) ((LPC_SPIM_PORT->TXCTRL >> 24) & 0xF);

	dmaXferCfg.src  = (void *) rtu_spi0_pp.ping_pong_buf[0].ping_pong_buf_1K;
	dmaXferCfg.dest = (void *) &LPC_SPIM_PORT->TXDAT;

	/* Setup source to desination copy for trigger for memory */
	dmaXferCfg.xferCount = RTU_PING_PONG_BUF_MAX_NUM;					/* Transfer data values of size width */
	dmaXferCfg.swTrig = 1;									/* No software triggering */
	dmaXferCfg.clrTrig = 0;									/* Do not clear trigger after this descriptor completes */
	dmaXferCfg.fireDescCB = 1;							/* Do not fire descriptor callback on complettion of this descriptor */
	dmaXferCfg.enabCirc = 1;
	dmaXferCfg.dmaCh = device_spi_0->spiDmaTXQueue.dmaCh;
	dmaXferCfg.stallDesc = 0;
	device_spi_0->spiCnt = 0;

	
	if (flen > 8) {
		dmaXferCfg.width = ROM_DMA_WIDTH_2;			/* Width is 2 bytes */
	}
	else {
		dmaXferCfg.width = ROM_DMA_WIDTH_1;			/* Width is 1 byte */
	}
	dmaXferCfg.srcInc = ROM_DMA_ADDRINC_1X;	/* Increment source address by width for each data transfer */
	dmaXferCfg.dstInc = ROM_DMA_ADDRINC_0X;	/* Increment destination address by width for each data transfer */
	if (ROM_DMA_BuildDescriptorChain(dmaHandle, &dmaXferCfg, &device_spi_0->dmaSPITXDesc, &device_spi_0->dmaSPITXDesc1) != LPC_OK) {
		errorOut("Error building descriptor chain (SPI TX single link)\r\n");
	}

	dmaXferCfg.src  = (void *) rtu_spi0_pp.ping_pong_buf[1].ping_pong_buf_1K;
	if (ROM_DMA_BuildDescriptorChain(dmaHandle, &dmaXferCfg, &device_spi_0->dmaSPITXDesc1, &device_spi_0->dmaSPITXDesc) != LPC_OK) {
		errorOut("Error building descriptor chain (SPI TX single link)\r\n");
	}
	
	/* Queue descriptor for SPI TX transfer */
	ROM_DMA_QueueDescriptor(dmaHandle, &device_spi_0->spiDmaTXQueue, &device_spi_0->dmaSPITXDesc);

//	/* Start queue */
//	ROM_DMA_StartQueue(dmaHandle, &device_spi_0->spiDmaTXQueue);
}


/*****************************************************************************
 * 函 数 名  : spiRecvSetup_SPI0
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月1日
 * 函数功能  : 初始化spi0的DMA接收：初始化DMA的接收Queue，并启动DMA接收
 * 输入参数  : ROM_SPIM_HANDLE_T spimHandle  SPI0设备系统环境指针
               ROM_SPIM_XFER_T *pXfer        发送接收缓冲区结构指针
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void spiRecvSetup_SPI0(ROM_SPIM_HANDLE_T spimHandle, ROM_SPIM_XFER_T *pXfer)
{
	uint8_t flen;
	ROM_DMA_XFERDESC_CFG_T dmaXferCfg;
	RTU_SPI_DMA_DEV_T *device_spi_0;
			
	device_spi_0 = VT_Get_Spi_Dev(0);//获取设备指针

	/* The DMA needs to be setup for 8- or 16-bit transfers based on the configured
	   SPI transfer size. For the example, we'll get the size from the SPI peripherals
	     config. */
	flen = 1 + (uint8_t) ((LPC_SPIM_PORT->TXCTRL >> 24) & 0xF);

	dmaXferCfg.src = (void *) &LPC_SPIM_PORT->RXDAT;
	dmaXferCfg.dest  = (void *) device_spi_0->rx16;

	/* Setup source to desination copy for trigger for memory */
	dmaXferCfg.xferCount = pXfer->rxSz;					/* Transfer data values of size width */
	dmaXferCfg.swTrig = 1;									/* No software triggering */
	dmaXferCfg.clrTrig = 0;									/* Do not clear trigger after this descriptor completes */
	dmaXferCfg.fireDescCB = 1;							/* Do not fire descriptor callback on complettion of this descriptor */
	dmaXferCfg.enabCirc = 0;								/* Not a circular buffer */
	dmaXferCfg.dmaCh = device_spi_0->spiDmaRXQueue.dmaCh;
	dmaXferCfg.stallDesc = 0;
	if (flen > 8) {
		dmaXferCfg.width = ROM_DMA_WIDTH_2;			/* Width is 2 bytes */
	}
	else {
		dmaXferCfg.width = ROM_DMA_WIDTH_1;			/* Width is 1 byte */
	}
	dmaXferCfg.srcInc = ROM_DMA_ADDRINC_0X;	/* Increment source address by width for each data transfer */
	dmaXferCfg.dstInc = ROM_DMA_ADDRINC_1X;	/* Increment destination address by width for each data transfer */
	if (ROM_DMA_BuildDescriptorChain(dmaHandle, &dmaXferCfg, &device_spi_0->dmaSPIRXDesc, NULL) != LPC_OK) {
		errorOut("Error building descriptor chain (SPI TX single link)\r\n");
	}

	/* Queue descriptor for SPI TX transfer */
	ROM_DMA_QueueDescriptor(dmaHandle, &device_spi_0->spiDmaRXQueue, &device_spi_0->dmaSPIRXDesc);

	/* Start queue */
	ROM_DMA_StartQueue(dmaHandle, &device_spi_0->spiDmaRXQueue);
}

void dmacnt_printf()
{
	rt_kprintf(1,"dmacnt[%d]\r\n",dmaCnt);
	dmaCnt = 0;
}
/*****************************************************************************
 * 函 数 名  : DMA_IRQHandler
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月1日
 * 函数功能  : DMA中断服务函数
 * 输入参数  : void  无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void DMA_IRQHandler(void)
{
	dmaCnt++;
	
	ROM_DMA_DMAHandler(dmaHandle);
//	Board_LED_Toggle(0);
//	Board_LED_Toggle(1);
//	Board_LED_Toggle(0);
//	Board_LED_Toggle(1);
}

/*****************************************************************************
 * 函 数 名  : dmaSPIRXTransferComplete_SPI0
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月1日
 * 函数功能  : DMA controller transfer descriptor chain complete callback
 * 输入参数  : ROM_DMA_HANDLE_T dmaHandle    DMA设备系统环境变量结构体指针
               struct ROM_DMA_QUEUE *pQueue  SPI0发送Queue
               ROM_DMA_DESC_T *pTranDesc     DMA transfer descriptor
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void dmaSPIRXTransferComplete_SPI0(ROM_DMA_HANDLE_T dmaHandle, struct ROM_DMA_QUEUE *pQueue, ROM_DMA_DESC_T *pTranDesc)
{
	RTU_SPI_DMA_DEV_T *device_spi_0;
	
	device_spi_0 = VT_Get_Spi_Dev(0);
	/* Receive finishes after transmit, so close the transfer once the DMA has received all the data */
	ROM_SPIM_ClosePendingTransfer(device_spi_0->spimHandle);
}

/*****************************************************************************
 * 函 数 名  : dmaSPIRXTransferComplete_SPI1
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月1日
 * 函数功能  : DMA controller transfer descriptor chain complete callback
 * 输入参数  : ROM_DMA_HANDLE_T dmaHandle    DMA设备系统环境变量结构体指针
               struct ROM_DMA_QUEUE *pQueue  SPI0发送Queue
               ROM_DMA_DESC_T *pTranDesc     DMA transfer descriptor
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void dmaSPIRXTransferComplete_SPI1(ROM_DMA_HANDLE_T dmaHandle, struct ROM_DMA_QUEUE *pQueue, ROM_DMA_DESC_T *pTranDesc)
{
	RTU_SPI_DMA_DEV_T *device_spi_1;
	
	device_spi_1 = VT_Get_Spi_Dev(1);
	/* Receive finishes after transmit, so close the transfer once the DMA has received all the data */
	ROM_SPIM_ClosePendingTransfer(device_spi_1->spimHandle);
}


/*****************************************************************************
 * 函 数 名  : LPC_SPIM_INTHANDLER
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月1日
 * 函数功能  : SPI0 的中断服务函数
 * 输入参数  : void  无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void LPC_SPIM_INTHANDLER(void)
{	
	RTU_SPI_DMA_DEV_T *device_spi;
		
	device_spi = VT_Get_Spi_Dev(0);//获取设备指针
	ROM_SPIM_TransferHandler(device_spi->spimHandle);
}

/*****************************************************************************
 * 函 数 名  : LPC_SPIM_INTHANDLER
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月1日
 * 函数功能  : SPI1 的中断服务函数
 * 输入参数  : void  无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void LPC_SPIM_INTHANDLER1(void)
{
	//device_spi_1 = VT_Get_Spi_Dev(1);
	RTU_SPI_DMA_DEV_T *device_spi;
		
//		Board_LED_Toggle(0);
//		Board_LED_Toggle(1);
//		Board_LED_Toggle(0);
//		Board_LED_Toggle(0);
//		Board_LED_Toggle(1);
//		Board_LED_Toggle(0);
	device_spi = VT_Get_Spi_Dev(1);//获取设备指针
	ROM_SPIM_TransferHandler(device_spi->spimHandle);
//	ROM_SPIM_TransferHandler(VT_Get_Spi_Dev(1));
}

/*****************************************************************************
 * 函 数 名  : CBspiMasterReceiveCB_SPI1
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月1日
 * 函数功能  : 接收回调函数  To keep things transparent, the DMA transfer is 
 							setup as part of the SPI receive transfer callback
 * 输入参数  : ROM_SPIM_HANDLE_T spimHandle  spi
               ROM_SPIM_XFER_T *pXfer        发送缓冲
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void CBspiMasterReceiveCB_SPI1(ROM_SPIM_HANDLE_T spimHandle, ROM_SPIM_XFER_T *pXfer)
{
	spiRecvSetup_SPI1(spimHandle, pXfer);//初始化dma接收，并启动 dma
	
	pXfer->rxSz = 0;
}

/*****************************************************************************
 * 函 数 名  : CBspiMasterReceiveCB_SPI1
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月1日
 * 函数功能  : 接收回调函数  To keep things transparent, the DMA transfer is 
 							setup as part of the SPI receive transfer callback
 * 输入参数  : ROM_SPIM_HANDLE_T spimHandle  spi
               ROM_SPIM_XFER_T *pXfer        发送缓冲
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void CBspiMasterTransmitCB_SPI1(ROM_SPIM_HANDLE_T spimHandle, ROM_SPIM_XFER_T *pXfer)
{
	spiTranSetup_SPI1(spimHandle, pXfer);//初始化dma发送，不启动 dma
	if(FIRST_SEND_CH == CH_SPI1)
	{
		spi0_spi1_dma_start_queue(CH_SPI1);		//启动 dma
		RTU_Timer2Dev_Restart(Rtu_Get_tx_mod_delay());	//启动定时器2 延时
	}
	pXfer->txSz = 0;
}
void CBspiMasterXferCSAssertCB_SPI1(ROM_SPIM_HANDLE_T spimHandle, ROM_SPIM_XFER_T *pXfer)
{
	/* SPI master assertion event */
}

void CBspiMMasterXferCSDeAssertCB_SPI1(ROM_SPIM_HANDLE_T spimHandle, ROM_SPIM_XFER_T *pXfer)
{
	/* SPI master de-assertion event */
}

void CBspiMasterXferCSAssertCB_SPI0(ROM_SPIM_HANDLE_T spimHandle, ROM_SPIM_XFER_T *pXfer)
{
	/* SPI master assertion event */
}

void CBspiMMasterXferCSDeAssertCB_SPI0(ROM_SPIM_HANDLE_T spimHandle, ROM_SPIM_XFER_T *pXfer)
{
	/* SPI master de-assertion event */
}

/*****************************************************************************
 * 函 数 名  : CBspiMasterReceiveCB_SPI0
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月1日
 * 函数功能  : 接收回调函数  To keep things transparent, the DMA transfer is 
 							setup as part of the SPI receive transfer callback
 * 输入参数  : ROM_SPIM_HANDLE_T spimHandle  spi
               ROM_SPIM_XFER_T *pXfer        发送缓冲
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void CBspiMasterReceiveCB_SPI0(ROM_SPIM_HANDLE_T spimHandle, ROM_SPIM_XFER_T *pXfer)
{
	spiRecvSetup_SPI0(spimHandle, pXfer);//初始化dma接收，并启动 dma

	pXfer->rxSz = 0;
}

/*****************************************************************************
 * 函 数 名  : CBspiMasterTransmitCB_SPI0
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月1日
 * 函数功能  : 接收回调函数  To keep things transparent, the DMA transfer is 
 							setup as part of the SPI receive transfer callback
 * 输入参数  : ROM_SPIM_HANDLE_T spimHandle  spi
               ROM_SPIM_XFER_T *pXfer        发送缓冲
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void CBspiMasterTransmitCB_SPI0(ROM_SPIM_HANDLE_T spimHandle, ROM_SPIM_XFER_T *pXfer)
{
	spiTranSetup_SPI0(spimHandle, pXfer);//初始化dma发送 不启动dma，待定时器2中断延时一段时间启动dma
	if(FIRST_SEND_CH == CH_SPI0)
	{
		spi0_spi1_dma_start_queue(CH_SPI0);		//启动 dma
		RTU_Timer2Dev_Restart(Rtu_Get_tx_mod_delay());	//启动定时器2 延时
	}
	pXfer->txSz = 0;
}



void testprintf(uint8_t port)
{
	
	RTU_SPI_DMA_DEV_T *device_spi;
	
	uint16_t tlv5614head=0;
	uint16_t data;
	int i;
		
	device_spi = VT_Get_Spi_Dev(port);//获取设备指针
	
	tlv5614head = (0x1 <<12);	//fast 模式，0 slow模式
	tlv5614head |= (1<<14);
	
	data = (uint16_t)((0xf &0x0FFF) | (tlv5614head)); // 组 5614数据格式 A1A0 PWR SPD+12bit data
	for (i=0; i<64; i++)
		device_spi->tx16[i] = data;

//	memset(device_spi->tx16,data,64);
	
	tlv5614head |= (1<<14);
	data = (uint16_t)((0x0 &0x0FFF) | (tlv5614head)); // 组 5614数据格式 A1A0 PWR SPD+12bit data
	for (i=0; i<64; i++)
		device_spi->tx161[i] = data;
//	memset(device_spi->tx161,data,64);

	rt_kprintf(1,"tx16[%x],tx161[%x]\r\n",device_spi->tx16,device_spi->tx161);
	rt_kprintf(1,"Desc[%x],Desc1[%x]\r\n",&device_spi->dmaSPITXDesc,&device_spi->dmaSPITXDesc1);
	rt_kprintf(1,"NextLink[%x] src[%x]\r\n",device_spi->dmaSPITXDesc.pNextLink,device_spi->dmaSPITXDesc.src);
	rt_kprintf(1,"NextLink1[%x] src1[%x]\r\n",device_spi->dmaSPITXDesc1.pNextLink,device_spi->dmaSPITXDesc1.src);
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
	
		RTU_SPI_DMA_DEV_T *device_spi_0,*device_spi_1;
	
		device_spi_0 = VT_Get_Spi_Dev(0);//获取 spi设备指针
		device_spi_1 = VT_Get_Spi_Dev(1);//获取 spi设备指针
		
		/* Generic Initialization */
//		SystemCoreClockUpdate();
//		Board_Init();
	
		/* Setup SPI pin muxing, enable SPI clock and reset SPI peripheral */
		Init_SPI_PinMux_Func();
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
#ifndef RTU_IO_SPI0
		/* Setup SPI1 TX channel for peripheral request and init queue */
		if (ROM_DMA_SetupChannel(dmaHandle, (ROM_DMA_CHAN_CFG_T *) &chanCfg, LPC_SPI_TX_DMACH) != LPC_OK) {
			errorOut("Error setting up SPI TX DMA channel\r\n");
		}
		
		if (ROM_DMA_InitQueue(dmaHandle, LPC_SPI_TX_DMACH, &device_spi_0->spiDmaTXQueue) != LPC_OK) {
			errorOut("Error initializing SPI TX DMA queue\r\n");
		}
	
		/* Setup SPI1 RX channel for peripheral request and init queue */
		if (ROM_DMA_SetupChannel(dmaHandle, (ROM_DMA_CHAN_CFG_T *) &chanCfg, LPC_SPI_RX_DMACH) != LPC_OK) {
			errorOut("Error setting up SPI RX DMA channel\r\n");
		}
		if (ROM_DMA_InitQueue(dmaHandle, LPC_SPI_RX_DMACH, &device_spi_0->spiDmaRXQueue) != LPC_OK) {
			errorOut("Error initializing SPI RX DMA queue\r\n");
		}
#endif
	/* Setup SPI1 TX channel for peripheral request and init queue */
		if (ROM_DMA_SetupChannel(dmaHandle, (ROM_DMA_CHAN_CFG_T *) &chanCfg1, LPC_SPI_TX_DMACH1) != LPC_OK) {
			errorOut("Error setting up SPI TX DMA channel\r\n");
		}
		if (ROM_DMA_InitQueue(dmaHandle, LPC_SPI_TX_DMACH1, &device_spi_1->spiDmaTXQueue) != LPC_OK) {
			errorOut("Error initializing SPI TX DMA queue\r\n");
		}
	
		/* Setup SPI1 RX channel for peripheral request and init queue */
		if (ROM_DMA_SetupChannel(dmaHandle, (ROM_DMA_CHAN_CFG_T *) &chanCfg1, LPC_SPI_RX_DMACH1) != LPC_OK) {
			errorOut("Error setting up SPI RX DMA channel\r\n");
		}
		if (ROM_DMA_InitQueue(dmaHandle, LPC_SPI_RX_DMACH1, &device_spi_1->spiDmaRXQueue) != LPC_OK) {
			errorOut("Error initializing SPI RX DMA queue\r\n");
		}
	
		/* Register error, descriptor completion, and descriptor chain completion callbacks for SPI RX channel */
		ROM_DMA_RegisterQueueCallback(dmaHandle, &device_spi_1->spiDmaRXQueue, ROM_DMA_XFERCOMPLETE_CB, (void *) dmaSPIRXTransferComplete_SPI1);
	
#ifndef RTU_IO_SPI0
		/* Register error, descriptor completion, and descriptor chain completion callbacks for SPI RX channel */
		ROM_DMA_RegisterQueueCallback(dmaHandle, &device_spi_0->spiDmaRXQueue, ROM_DMA_XFERCOMPLETE_CB, (void *) dmaSPIRXTransferComplete_SPI0);
	
#endif
		NVIC_SetPriority(DMA_IRQn,ISR_PRIORITY_DMA);
		NVIC_EnableIRQ(DMA_IRQn);

}

/*****************************************************************************
 * 函 数 名  : Rtu_init_spi_clk
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月1日
 * 函数功能  : 配置spi的系统时钟，并复位spi设备
 * 输入参数  : uint8_t spi_port  spi端口  0==spi0 ，1===spi1
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void Rtu_init_spi_clk(uint8_t spi_port)
{
	if(spi_port==0)
	{
		Chip_Clock_EnablePeriphClock(LPC_SPIM_CLOCK);	//使能spi0 clock
		Chip_SYSCON_PeriphReset(LPC_SPIM_RESET);		//复位spi0
	}
	else
	{
		Chip_Clock_EnablePeriphClock(LPC_SPIM_CLOCK1);	//使能spi1 clock
		Chip_SYSCON_PeriphReset(LPC_SPIM_RESET1);		//复位spi1
	}

}

/* SPI interrupt handler */
//void LPC_SPIM_INTHANDLER(void)
//{
//	ROM_SPIM_TransferHandler(spimHandle);
//}



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
void RTU_SpiOneDev_Init(uint8_t spi_port)
{
	uint32_t memSize, *devMem;
		ROM_SPIM_INIT_T spimInit;
		ROM_SPIM_XFER_CONFIG_T spimConfig;
		ROM_DMA_INIT_T dmaInit;
		int i;
		RTU_SPI_DMA_DEV_T *device_spi;
		
		
		Rtu_init_spi_clk(spi_port);//配置spi设备时钟
		
		device_spi = VT_Get_Spi_Dev(spi_port);//获取设备指针

#ifdef MALLOC_PING_PONG
		if(spi_port == 1)
		{
			device_spi->ping_pong_buf_spi = (void*) pvPortMalloc(sizeof(RTU_PING_PONG_SPI1_DATA_T));
		}
		else
		{
			device_spi->ping_pong_buf_spi = (void*) pvPortMalloc(sizeof(RTU_PING_PONG_DATA_T));
		}
		if(device_spi->ping_pong_buf_spi == NULL)
		{
			//while(1);
		}
		else
		{
			memset(device_spi->ping_pong_buf_spi,0,sizeof(RTU_PING_PONG_DATA_T));
		}	
#else
		if(spi_port == 1)
		{
			device_spi->ping_pong_buf_spi = (void*) &rtu_spi1_pp;
			for(i=0;i<RTU_PING_PONG_BUF_MAX_NUM;i++)
			{
				rtu_spi1_pp.ping_pong_buf[0].ping_pong_buf_4K[4*i+RTU_SPI1_DAC_DMA_CH_A] = (RTU_SPI1_DAC_DMA_CH_A << 14)+2048;
				rtu_spi1_pp.ping_pong_buf[0].ping_pong_buf_4K[4*i+RTU_SPI1_DAC_DMA_CH_B] = (RTU_SPI1_DAC_DMA_CH_B << 14);
				rtu_spi1_pp.ping_pong_buf[0].ping_pong_buf_4K[4*i+RTU_SPI1_DAC_DMA_CH_C] = (RTU_SPI1_DAC_DMA_CH_C << 14);
				rtu_spi1_pp.ping_pong_buf[0].ping_pong_buf_4K[4*i+RTU_SPI1_DAC_DMA_CH_D] = (RTU_SPI1_DAC_DMA_CH_D << 14);
				
				rtu_spi1_pp.ping_pong_buf[1].ping_pong_buf_4K[4*i+RTU_SPI1_DAC_DMA_CH_A] = (RTU_SPI1_DAC_DMA_CH_A << 14)+2048;
				rtu_spi1_pp.ping_pong_buf[1].ping_pong_buf_4K[4*i+RTU_SPI1_DAC_DMA_CH_B] = (RTU_SPI1_DAC_DMA_CH_B << 14);
				rtu_spi1_pp.ping_pong_buf[1].ping_pong_buf_4K[4*i+RTU_SPI1_DAC_DMA_CH_C] = (RTU_SPI1_DAC_DMA_CH_C << 14);
				rtu_spi1_pp.ping_pong_buf[1].ping_pong_buf_4K[4*i+RTU_SPI1_DAC_DMA_CH_D] = (RTU_SPI1_DAC_DMA_CH_D << 14);
			}
		}
		else
		{
			device_spi->ping_pong_buf_spi = (void*) &rtu_spi0_pp;
		}

#endif
		
		/* Get needed size for SPI driver context memory */
		memSize = ROM_SPIM_GetMemSize();
//		if (memSize > sizeof(device_spi->spiDrvData)) {
//			errorOut("Can't allocate memory for SPI driver context\r\n");
//		}
		devMem = (uint32_t*)&device_spi->spiDrvData;/* Or just use malloc(memSize) */
	
		/* Initialize driver */
		spimInit.pUserData = NULL;
		if(spi_port==0)
		{
			spimInit.base = (uint32_t) LPC_SPIM_PORT;
		}
		else
		{
			spimInit.base = (uint32_t) LPC_SPIM_PORT1;
		}
		spimInit.baseClockRate = Chip_Clock_GetAsyncSyscon_ClockRate();
		spimInit.spiPol[0] = 0; /* Active low select for SSEL0 */
		spimInit.spiPol[1] = 1;
		spimInit.spiPol[2] = 1;
		spimInit.spiPol[3] = 1;
		device_spi->spimHandle = ROM_SPIM_Init(devMem, &spimInit);//初始化spi设备环境变量
		if (device_spi->spimHandle == NULL) 
		{
			/* Error initializing SPI */
			errorOut("Error initializing ROM\r\n");
		}
	
		/* Set SPI transfer configuration */
		memset(&spimConfig, 0, sizeof(spimConfig));
		
		if(spi_port==0)
		{
			spimConfig.dXferBitRate = SPI0_TXMOD_BITRATE;
			spimConfig.mode = ROM_SPI_CLOCK_MODE0; //txmod
		}
		else
		{
			spimConfig.dXferBitRate = SPI1_DAC_BITRATE;
			spimConfig.mode = ROM_SPI_CLOCK_MODE2; //adc1
		}
		spimConfig.lsbFirst = 0;
		spimConfig.dataBits = XFERSZ;
		spimConfig.PreDelay = 0;
		spimConfig.PostDelay = 0;
		spimConfig.FrameDelay = 0;
		spimConfig.TransferDelay = 3;
		if (ROM_SPIM_SetupTransfer(device_spi->spimHandle, &spimConfig) != LPC_OK) 
		{
			errorOut("SPI configuration is invalid\r\n");
		}
	
		/* Show desired and actual SPI rates */
		DEBUGOUT("SPI rate = %d (actual %d)\r\n", spimConfig.dXferBitRate, spimConfig.rXferBitRate);

		/*配置回调函数*/
		if(spi_port==0)
		{
		
		/* Callback registration  */
//			ROM_SPIM_RegisterCallback(device_spi->spimHandle, ROM_SPIM_DATATRECEIVE_CB, (void *) CBspiMasterReceiveCB_SPI0);
			ROM_SPIM_RegisterCallback(device_spi->spimHandle, ROM_SPIM_DATATRANSMIT_CB, (void *) CBspiMasterTransmitCB_SPI0);
//			ROM_SPIM_RegisterCallback(device_spi->spimHandle, ROM_SPIM_ASSERTSSEL_CB, (void *) CBspiMasterXferCSAssertCB_SPI0);
//			ROM_SPIM_RegisterCallback(device_spi->spimHandle, ROM_SPIM_DEASSERTSSEL_CB, (void *) CBspiMMasterXferCSDeAssertCB_SPI0);

			
			/* Enable the interrupt for the SPI and DMA */
			NVIC_SetPriority(LPC_SPIIRQNUM,ISR_PRIORITY_SPI0);
			NVIC_EnableIRQ(LPC_SPIIRQNUM);
		}
		else
		{
//			ROM_SPIM_RegisterCallback(device_spi->spimHandle, ROM_SPIM_DATATRECEIVE_CB, (void *) CBspiMasterReceiveCB_SPI1);
			ROM_SPIM_RegisterCallback(device_spi->spimHandle, ROM_SPIM_DATATRANSMIT_CB, (void *) CBspiMasterTransmitCB_SPI1);
//			ROM_SPIM_RegisterCallback(device_spi->spimHandle, ROM_SPIM_ASSERTSSEL_CB, (void *) CBspiMasterXferCSAssertCB_SPI1);
//			ROM_SPIM_RegisterCallback(device_spi->spimHandle, ROM_SPIM_DEASSERTSSEL_CB, (void *) CBspiMMasterXferCSDeAssertCB_SPI1);
			/* Enable the interrupt for the SPI and DMA */
			NVIC_SetPriority(LPC_SPIIRQNUM1,ISR_PRIORITY_SPI1);

			NVIC_EnableIRQ(LPC_SPIIRQNUM1);
		}
	
	

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

	RTU_SpiOneDev_Init(0);
	RTU_SpiOneDev_Init(1);	
}


/*****************************************************************************
 * 函 数 名  : SPI_Send_One_Data
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月1日
 * 函数功能  : 通过spi发送2个字节的数据
 * 输入参数  : uint8_t spi_Port     spi端口：0==spi0,1===spi1
               uint8_t tlv5614port  tlv5614通道：0=chA，1-ChB，2-chC，3-chD
               uint32_t data        发送数据
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
uint8_t SPI_DMA_App_Send_Start(uint8_t spi_port)
{
	RTU_SPI_DMA_DEV_T *devptr;
	uint16_t tlv5614head=0;
	uint16_t *p_sendbuf,*p_rxvbuf;
	ROM_SPIM_HANDLE_T *tmphandler;
	uint32_t *tmpcnt;
	ROM_SPIM_XFER_T *mXfer;
	RTU_SPI_DMA_DEV_T *device_spi;
	
	device_spi = VT_Get_Spi_Dev(spi_port);//获取设备指针
	if(device_spi==NULL)
	{
		return;
	}
	
	tmphandler = &device_spi->spimHandle;//获取spi设备环境变量指针
	p_sendbuf = device_spi->tx16;			//获取发送缓冲区指针
	p_rxvbuf = device_spi->rx16;			//获取接收缓冲区指针
	mXfer = &device_spi->mXfer;				//获取发送接收结构体指针。

	/*
		spi0--直接操纵 tx pll，不涉及发送通道 
	  	spi1--操纵tlv5614芯片，输出四通道DAC.需要进行数据转换
	  	数据格式: D15(A1),D14(A0)，D13(PWR),D12(SPD),D11-D0(12bit DAC value)
	*/

	tlv5614head = (0x0 <<12); 	//fast 模式，0 slow模式
	tlv5614head |= (1<<14);
	
	p_sendbuf[0] = (uint16_t)((0x0 &0x0FFF) | (tlv5614head)); // 组 5614数据格式 A1A0 PWR SPD+12bit data

//	rt_kprintf(1,"SPI_Send_One_Data[%d]\r\n",device_spi->spiCnt);
	/* Setup buffers and sizes, use select 0 for transfer */
	mXfer->txBuff = p_sendbuf;
	mXfer->txSz = 1;
	mXfer->rxBuff = p_rxvbuf;
	mXfer->rxSz = 0;
	mXfer->sselNum = 0;

	/* Setup for DMA  mode */
	mXfer->flags = ROM_SPIM_FLAG_DMATX | ROM_SPIM_FLAG_RXIGNORE;
	
	/* Start transfer. Will return immediately */
	ROM_SPIM_Transfer(*tmphandler, mXfer);

 	return TRUE;
}

/*****************************************************************************
 * 函 数 名  : SPI_Send_One_Data
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月1日
 * 函数功能  : 通过spi发送2个字节的数据
 * 输入参数  : uint8_t spi_Port     spi端口：0==spi0,1===spi1
               uint8_t tlv5614port  tlv5614通道：0=chA，1-ChB，2-chC，3-chD
               uint32_t data        发送数据
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
uint8_t SPI_Send_One_Data(uint8_t spi_Port,uint8_t tlv5614port,uint32_t data)
{
	RTU_SPI_DMA_DEV_T *devptr;
	uint16_t tlv5614head=0;
	uint16_t *p_sendbuf,*p_rxvbuf;
	ROM_SPIM_HANDLE_T *tmphandler;
	uint32_t *tmpcnt;
	ROM_SPIM_XFER_T *mXfer;
	RTU_SPI_DMA_DEV_T *device_spi;
	
//		Board_LED_Toggle(1);
//		Board_LED_Toggle(0);
//		Board_LED_Toggle(1);
	device_spi = VT_Get_Spi_Dev(spi_Port);//获取设备指针
	if(device_spi==NULL)
	{
		return;
	}
	
	tmphandler = &device_spi->spimHandle;//获取spi设备环境变量指针
	p_sendbuf = device_spi->tx16;			//获取发送缓冲区指针
	p_rxvbuf = device_spi->rx16;			//获取接收缓冲区指针
	mXfer = &device_spi->mXfer;				//获取发送接收结构体指针。

	/*
		spi0--直接操纵 tx pll，不涉及发送通道 
	  	spi1--操纵tlv5614芯片，输出四通道DAC.需要进行数据转换
	  	数据格式: D15(A1),D14(A0)，D13(PWR),D12(SPD),D11-D0(12bit DAC value)
	*/
	if(spi_Port==0)
	{
		tlv5614head = 0;
		
		p_sendbuf[device_spi->spiCnt++%BUFFSIZE] = (uint16_t)((data &0xFFFF)); // 组 5614数据格式 A1A0 PWR SPD+12bit data
	}
	else
	{
		tlv5614head = (0x1 <<12); 	//fast 模式，0 slow模式
		tlv5614head |= (tlv5614port<<14);
		
		p_sendbuf[device_spi->spiCnt++%BUFFSIZE] = (uint16_t)((data &0x0FFF) | (tlv5614head)); // 组 5614数据格式 A1A0 PWR SPD+12bit data
	}

//	rt_kprintf(1,"SPI_Send_One_Data[%d]\r\n",device_spi->spiCnt);
	/* Setup buffers and sizes, use select 0 for transfer */
	mXfer->txBuff = p_sendbuf;
	mXfer->txSz = 1;
	mXfer->rxBuff = p_rxvbuf;
	mXfer->rxSz = 0;
	mXfer->sselNum = 0;

	/* Setup for DMA  mode */
	mXfer->flags = ROM_SPIM_FLAG_DMATX | ROM_SPIM_FLAG_RXIGNORE;
//	mXfer->flags = ROM_SPIM_FLAG_BLOCKING  | ROM_SPIM_FLAG_RXIGNORE; 
//	mXfer->flags = ROM_SPIM_FLAG_LOOPBACK | ROM_SPIM_FLAG_DMATX |ROM_SPIM_FLAG_DMARX; 

	/* Start transfer. Will return immediately */
	ROM_SPIM_Transfer(*tmphandler, mXfer);


 	return TRUE;
}


void SPI_Send_Data_DAC(uint8_t DACx,uint16_t data)
{
	SPI_Send_One_Data(1,DACx,data);
}

void SPI_Set_DAC_A(uint16_t data)
{
	SPI_Send_Data_DAC(SPI_DAC_A,data);
}

void SPI_Set_DAC_B(uint16_t data)
{
	SPI_Send_Data_DAC(SPI_DAC_B,data);
}

void SPI_Set_DAC_C(uint16_t data)
{
 	SPI_Send_Data_DAC(SPI_DAC_C,data);
}

void SPI_Set_DAC_D(uint16_t data)
{
	SPI_Send_Data_DAC(SPI_DAC_D,data);
}

/*****************************************************************************
 * 函 数 名  : stop_dma_spi_send
 * 负 责 人  : MZM
 * 创建日期  : 2016年6月7日
 * 函数功能  : 停用spi——代码发射，spi0和spi1同事停止
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void stop_dma_spi_send()
{
	RTU_SPI_DMA_DEV_T *device_spi;
	
	device_spi = VT_Get_Spi_Dev(1);//获取设备指针
//	ROM_DMA_FlushQueue(dmaHandle, &device_spi->spiDmaTXQueue);//停止dma搬移
	ROM_DMA_StopQueue(dmaHandle, &device_spi->spiDmaTXQueue);//停止dma搬移
	
	device_spi = VT_Get_Spi_Dev(0);//获取设备指针
//	ROM_DMA_FlushQueue(dmaHandle, &device_spi->spiDmaTXQueue);//停止dma搬移
	ROM_DMA_StopQueue(dmaHandle, &device_spi->spiDmaTXQueue);//停止dma搬移

	
}

/*****************************************************************************
 * 函 数 名  : Set_TX_PLL_Start_Init
 * 负 责 人  : MZM
 * 创建日期  : 2016年6月7日
 * 函数功能  : 设置TX——PLL操作，首先暂停adc转换，在听spi的dma发送，并切换-
               spi0为IO进行操作
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void Set_TX_PLL_Start_Init()
{
	
	Init_SPI_PinMux_IO();//spi0 io 操作
#ifndef DISABLE_ADC_TIMER_FOR_JJJ

//	RTU_Timer1Dev_Stop(); //停止 timer1 从而停止 启动adc转换
//	RTU_Timer2Dev_Stop(); //停止 timer2 从而停止 发送txmod dma启动
	
	stop_dma_spi_send();
	Rtu_Set_restart_dma_flg(FALSE);

#endif
	
}

/*****************************************************************************
 * 函 数 名  : Set_TX_PLL_End_Init
 * 负 责 人  : MZM
 * 创建日期  : 2016年6月7日
 * 函数功能  : 设置TXPLL寄存器完成后的操作，启动timer以启动ad转换，重新初始
               化spi，启用spi1的dma操作，不启用spi0的代码操作
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void Set_TX_PLL_End_Init()
{
#ifndef DISABLE_ADC_TIMER_FOR_JJJ

//	RTU_TimerDev_Init();	//初始化timer
//	RTU_SpiDev_Init();		//初始化spi--dma

	
//	Rtu_Set_restart_dma_flg(FALSE);
//	SPI_DMA_App_Send_Start(0);	//启动spi0--并不启动dma发送
//	SPI_DMA_App_Send_Start(1);	//启动spi1--启动dma发送
#endif
}

/*****************************************************************************
 * 函 数 名  : stop_dma_spi_for_adc_calc
 * 负 责 人  : MZM
 * 创建日期  : 2016年6月29日
 * 函数功能  : 停止spidma搬移数据
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void stop_dma_spi_for_adc_calc()
{
	Rtu_Set_restart_dma_flg(FALSE);//设置标志
	stop_dma_spi_send();//停止dma搬移数据
}

void restart_dma_spi_for_adc_calc(uint8_t data_flg)
{
	uint8_t dmaflg=Rtu_Get_restart_dma_flg() ;

	
//	if(( FALSE == dmaflg) && (data_flg == 0))// 当需要重新resetdma发送且第1个buf数据已经拷贝完毕，设置标志
	if(( FALSE == dmaflg))// 当需要重新resetdma发送且第1个buf数据已经拷贝完毕，设置标志
	{
		Rtu_Set_restart_dma_flg(TRUE);//设置重新启动dma标志，下次adc数据计算后 启动dma
//		SPI_DMA_App_Send_Start(0);	//启动spi0--并不启动dma发送
//		SPI_DMA_App_Send_Start(1);	//启动spi1--启动dma发送
	
		return;	
	}
	
	if( TRUE == dmaflg )
	{
		/*需要重新启动dma*/
		SPI_DMA_App_Send_Start(CH_SPI0);	//启动spi0--并不启动dma发送
		SPI_DMA_App_Send_Start(CH_SPI1);	//启动spi1--启动dma发送
		Rtu_Set_restart_dma_flg(0xFFFF);
	}
//	if(dmaflg != 0)
//	{
//		Rtu_Set_restart_dma_flg(--dmaflg);
//	}
}

uint8_t  Get_Second_Send_CH_Port()
{
	return SECOND_SEND_CH;
}
