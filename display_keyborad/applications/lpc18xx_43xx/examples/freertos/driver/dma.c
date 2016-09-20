#include "board.h"
#include "Oled_driver.h"
#include "LCDConf.h"
#include "config.h"

//#define TEST_DMA_M2M_MZM

static uint8_t dmaCh;
 volatile uint8_t DMA_EndFlag = 0;

 volatile  uint16_t LCDBUFF_MZM1[8][320];//256*64*4bit=64Row *64Column*2byte

#define DRIVER_USE_BIG			1		// 0 小端，1大端模式


#define  USE_DMA_SRC_HALFWORD		1		//源 数据
#if USE_DMA_SRC_HALFWORD
	#define LCD_MAX_DMA_LLI_SIZE 		(4096) 	
	#define LCD_DMA_SEND_SIZE_FLG			2
	#define LCD_DMA_SRC_SIZE			GPDMA_WIDTH_HALFWORD
#else
	#define LCD_MAX_DMA_LLI_SIZE 		(2048) 
	#define LCD_DMA_SEND_SIZE_FLG		1	
	#define LCD_DMA_SRC_SIZE			GPDMA_WIDTH_BYTE
#endif
#define LCD_MAX_DMA_TIMES (((XSIZE_PHYS)*(YSIZE_PHYS)*2)/LCD_MAX_DMA_LLI_SIZE)

#define LCD_LAST_DMA_LLI_SIZE (((XSIZE_PHYS)*(YSIZE_PHYS)*2) &(LCD_MAX_DMA_LLI_SIZE-1))
 
static volatile DMA_TransferDescriptor_t LcdBuffer[LCD_MAX_DMA_TIMES];//传输1屏DMA次数


 
 
#define LCD_BUS_WIDTH  8
 
 
#if LCD_BUS_WIDTH == 32
#define LCD_BUS_DATA_TYPE 	uint32
#endif
 
#if LCD_BUS_WIDTH == 16
#define LCD_BUS_DATA_TYPE 	uint16_t
#endif
#if LCD_BUS_WIDTH == 8
#define LCD_BUS_DATA_TYPE 	uint8_t
#endif
 #ifdef USED_SSD2119
 extern __align(8)  uint16_t LCDBUFF_MZM[120* 320] __attribute__ ((section ("VRAM"), zero_init));
 //#else
 //extern uint8_t LCDBUFF_MZM[64][128];//256*64*4bit=64Row *64Column*2byte
 
#endif

 /**
  * @brief	 DMA interrupt handler
  * @return  Nothing
  */
 void DMA_IRQHandler(void)
 {
	  if (Chip_GPDMA_Interrupt(LPC_GPDMA, dmaCh) == SUCCESS)
	  {	  	
//		NVIC_DisableIRQ(DMA_IRQn);
//		  Chip_GPDMA_Stop(LPC_GPDMA,dmaCh);
		  DMA_EndFlag = 0;
	  }
 }
 

 void Oled_DMA_init(void)
{
	/* Initialize GPDMA controller */
	Chip_GPDMA_Init(LPC_GPDMA);

	/* Get DMA channel */
	dmaCh = Chip_GPDMA_GetFreeChannel(LPC_GPDMA, 0);

//	/* Setup DMA transfer */
//	Chip_GPDMA_Transfer(LPC_GPDMA, dmaCh,
//					  (uint32_t) LCDBUFF_MZM, (uint32_t)LCD_DATA_BASE_ADDR, //(uint32_t) &dest[0],
//					  GPDMA_TRANSFERTYPE_M2M_CONTROLLER_DMA,
//					  sizeof(LCDBUFF_MZM));

//			Chip_GPDMA_Stop(LPC_GPDMA, dmaChSSPTx);
/* Enable GPDMA interrupt */
//	NVIC_EnableIRQ(DMA_IRQn);

}


void Oled_dma_flush_LLI_init(uint32_t src_addr,uint32_t dest_addr,uint32_t dma_size)
{
		DMA_TransferDescriptor_t  *DMADescriptor;
		static uint32_t i,max_dma_times,last_times_size;
#if 1
		max_dma_times = dma_size /LCD_MAX_DMA_LLI_SIZE;
		last_times_size = dma_size &(LCD_MAX_DMA_LLI_SIZE-1);

//		max_dma_times += !(last_times_size==0);
		
		memset(LcdBuffer,0,sizeof(LcdBuffer));
		DMADescriptor = LcdBuffer;
		for(i=0;i<max_dma_times;i++)
		{
#ifdef TEST_DMA_M2M_MZM
			DMADescriptor ->dst = (uint32_t)((LCD_BUS_DATA_TYPE*)dest_addr +LCD_MAX_DMA_LLI_SIZE *i);
#else
			DMADescriptor ->dst = dest_addr;
#endif
			DMADescriptor ->src = (uint32_t)((LCD_BUS_DATA_TYPE*)src_addr +LCD_MAX_DMA_LLI_SIZE *i);
			DMADescriptor ->ctrl = (
									((LCD_MAX_DMA_LLI_SIZE/LCD_DMA_SEND_SIZE_FLG) & 0x0fff)				//设置传输大小为突发传输次数
									|(GPDMA_BSIZE_4<<12)|(GPDMA_BSIZE_4<<15)		//源突发和目的突发大小为16位
									|(LCD_DMA_SRC_SIZE<<18)|(GPDMA_WIDTH_BYTE<<21)		//源和目标的数据宽度是16位 1个半字
#ifdef TEST_DMA_M2M_MZM
//									|(0x01<<24) |(0x01<<25) 	//源地址自增和目标地址自增
									|(0x01<<26) |(0x01<<27) 	//源地址自增和目标地址自增
#else
									|(0x01<<26) & (~(0x01<<27)) 	//源地址自增和目标地址不自增
#endif
//									|0x80000000 					//终端计数中断使能;
									& (~0x80000000)
									);	
			DMADescriptor->lli = (uint32_t)(&LcdBuffer[ i + 1]);
			if(i>0)
			{
				#if DRIVER_USE_BIG
				swap1((uint32_t)DMADescriptor->dst)  ;
				swap1((uint32_t)DMADescriptor->src)  ;
				swap1((uint32_t)DMADescriptor->ctrl)  ;
				swap1((uint32_t)DMADescriptor->lli)  ;
				#endif
			}
			DMADescriptor ++;
		}

		if(last_times_size != 0)
		{
#ifdef TEST_DMA_M2M_MZM
			DMADescriptor ->dst = (uint32_t)((LCD_BUS_DATA_TYPE*)dest_addr +LCD_MAX_DMA_LLI_SIZE *i);
#else
			DMADescriptor ->dst = dest_addr;
#endif
			DMADescriptor ->src = (uint32_t)((LCD_BUS_DATA_TYPE*)src_addr +LCD_MAX_DMA_LLI_SIZE *i);
			DMADescriptor ->ctrl = (
									((last_times_size/LCD_DMA_SEND_SIZE_FLG) & 0x0fff)				//设置传输大小为突发传输次数
									|(GPDMA_BSIZE_4<<12)|(GPDMA_BSIZE_4<<15)		//源突发和目的突发大小为16位
									|(LCD_DMA_SRC_SIZE<<18)|(GPDMA_WIDTH_BYTE<<21)		//源和目标的数据宽度是16位 1个半字
#ifdef TEST_DMA_M2M_MZM
//									|(0x01<<24) |(0x01<<25) 	//源地址自增和目标地址自增
									|(0x01<<26) |(0x01<<27) 	//源地址自增和目标地址自增
									
#else
									|(0x01<<26) & (~(0x01<<27)) 	//源地址自增和目标地址不自增
#endif
									|0x80000000 					//终端计数中断使能;
//									& (~0x80000000)
									);	
			DMADescriptor->lli = 0;
			#ifdef DRIVER_USE_BIG
			swap1((uint32_t)DMADescriptor->dst)  ;
			swap1((uint32_t)DMADescriptor->src)  ;
			swap1((uint32_t)DMADescriptor->ctrl)  ;
			swap1((uint32_t)DMADescriptor->lli)  ;
			#endif
		}
	
//		DMADescriptor->lli = 0;
		Chip_GPDMA_SGTransfer(LPC_GPDMA, dmaCh,(const DMA_TransferDescriptor_t  *)LcdBuffer,GPDMA_TRANSFERTYPE_M2M_CONTROLLER_DMA);

			#endif
}

void Oled_start_dma_flush(uint8_t *pdata,uint32_t datalen)
{
	if(DMA_EndFlag == 0)
	{

	//				LCDBUFF_MZM[0][0] = 0x55aa;
	//				LCDBUFF_MZM[0][1] = 0x1234;
	//				LCDBUFF_MZM[0][2] = 0x4321;                   
	//test_ssd2119(0x001F);
	Oled_DMA_init();
	DMA_EndFlag = 1;

	NVIC_EnableIRQ(DMA_IRQn);

#if 0
	/* Setup DMA transfer */
	Chip_GPDMA_Transfer(LPC_GPDMA, dmaCh,
	(uint32_t) pdata,(uint32_t) LCDBUFF_MZM1, //(uint32_t) &dest[0],
	//	(uint32_t) pdata,(uint32_t) LCD_DATA_BASE_ADDR, //(uint32_t) &dest[0],
				  GPDMA_TRANSFERTYPE_M2M_CONTROLLER_DMA,
				  4090);
#else

#ifdef TEST_DMA_M2M_MZM
	Oled_dma_flush_LLI_init((uint32_t)pdata,(uint32_t)LCDBUFF_MZM1,sizeof(LCDBUFF_MZM1));
#else
	Oled_dma_flush_LLI_init((uint32_t)pdata,LCD_DATA_BASE_ADDR,sizeof(LCDBUFF_MZM));
#endif

#endif

	while(DMA_EndFlag);
	}

}

