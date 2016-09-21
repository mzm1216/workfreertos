
#ifndef CONFIG_H
#define CONFIG_H

#include "lpc_types.h"
#include "stdint.h"

#ifdef __ICCARM__
#define ALIGNSTR(x) # x
#define ALIGN(x) _Pragma(ALIGNSTR(data_alignment = ## x))
#else
#define ALIGN(x) __attribute__ ((aligned(x)))
#endif


#ifdef  __cplusplus
extern "C" {
#endif

#define		VTR_VERX	2
#define		VTR_VERY	8
#define		VTR_VERZ	7


typedef unsigned char  uint8;                                           /*  �޷�??λ���ͱ�??          */
typedef signed   char  int8;                                            /*  �з�??λ���ͱ�??          */
typedef unsigned short uint16;                                          /*  �޷�??6λ���ͱ�??         */
typedef signed   short int16;                                           /*  �з�??6λ���ͱ�??         */
typedef unsigned long   uint32;                                          /*  �޷�??2λ���ͱ�??         */
typedef signed   long   int32;                                           /*  �з�??2λ���ͱ�??         */

typedef float          fp32;                                            /*  �����ȸ�����??2λ���ȣ�    */
typedef double         fp64;                                            /*  ˫���ȸ�����??4λ���ȣ�    */
typedef float          fp32_t;                                            /*  �����ȸ�����??2λ���ȣ�    */
typedef double         fp64_t;                                            /*  ˫���ȸ�����??4λ���ȣ�    */
	
	
#define	bit01	unsigned char
#define	uchar	unsigned char
#define	uint		unsigned short


#include "ioconfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h" 


#define MAX(x, y)				(((x) > (y)) ? (x) : (y))

#define PARAM_OFFSET(Strct, Field) ((unsigned long)(unsigned long*)&(((Strct *)0)->Field))

#define DEBUG_SYS			1

#define DEBUG_TASK_ADC		2

#define DEBUG_TASK_UART 	3

#define DEBUG_TASK_1S		4

#define DEBUG_FOR_I2C 		5
#define DEBUG_FOR_CHECK_PLL 		6


typedef __packed union{
                uint16 w;
                uint8 bb[2];
             }UintType;//bb[1]Ϊ��λ  bb[0]Ϊw��λ      
typedef __packed union
{
	uint32 w;
	uint8 bb[4];
}UlongType;//bb[1]Ϊ��λ  bb[0]Ϊw��λ  

typedef  union
{
	fp64 w;
	uint8 bb[8];
}Fp64Type;//bb[1]Ϊ��λ  bb[0]Ϊw��λ   

typedef  __packed struct
{
	UlongType wh1;
	UlongType wh2;
	UlongType wl1;
	UlongType wl2;
}fp128;

typedef  __packed union
{
	fp128 whl;
	uint8 bb[8+8];
}Fp128Type;//bb[1]Ϊ��λ  bb[0]Ϊw��λ 


typedef __packed struct
{
		uint8 high;
		uint8 low;
}inthl;             
typedef __packed union
{
		uint16 w;
		inthl whl;
}UinthlType;//bb[1]Ϊ��λ  bb[0]Ϊw��λ       
typedef  __packed struct
{		
		uint8 low;
		uint8 high;
}intlh;        
typedef __packed union
{
		uint16 w;
		intlh wlh;
}UintlhType;//bb[0]Ϊ��λ  bb[1]Ϊw��λ      


#define   swap(a)     a   =   (uint16)a   >>   8   |   a   <<   8   
#define   swap_self(a)     (uint16)a   >>   8   |   a   <<   8   
#define   swap1(a)   a   =   (uint32)a   >>   24   |   (a   >>   8)   &   0x0000ff00   |   (a   <<   8)   &   0x00ff0000   |   a   <<   24          
#define	  swaprgb(color,r,g,b)		color = (uint16)(r>>3)<<11|((g>>2)<<5)|b>>3
#define suc_respond	0x00//??????	???
#define fail_respond	0x01//				???    

//�Զ����жϵ����ȼ�
#define 	PRIORITY_BASE		(configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY)
#define 	PRIORITY_NUM0		(PRIORITY_BASE+0)
#define 	PRIORITY_NUM1		(PRIORITY_BASE+1)
#define 	PRIORITY_NUM2		(PRIORITY_BASE+2)
#define	PRIORITY_NUM3		(PRIORITY_BASE+3)
#define 	PRIORITY_NUM4		(PRIORITY_BASE+4)

#define 	ISR_PRIORITY_SPI1		PRIORITY_NUM1
#define 	ISR_PRIORITY_SPI0		PRIORITY_NUM1
#define 	ISR_PRIORITY_DMA		PRIORITY_NUM0
#define 	ISR_PRIORITY_TIMER1	PRIORITY_NUM0
#define 	ISR_PRIORITY_TIMER2	PRIORITY_NUM1
#define 	ISR_PRIORITY_ADC		PRIORITY_NUM1
#define 	ISR_PRIORITY_TIMER0	PRIORITY_NUM1
#define 	ISR_PRIORITY_UART0	PRIORITY_NUM2
#define 	ISR_PRIORITY_PININT	PRIORITY_NUM1


#define	swap(a)     a   =   (uint16)a   >>   8   |   a   <<   8   
#define   	swap_self(a)     (uint16)a   >>   8   |   a   <<   8   
#define   	swap1(a)   a   =   (uint32)a   >>   24   |   (a   >>   8)   &   0x0000ff00   |   (a   <<   8)   &   0x00ff0000   |   a   <<   24          
#define	swaprgb(color,r,g,b)		color = (uint16)(r>>3)<<11|((g>>2)<<5)|b>>3

#define RTU_PING_PONG_BUF_CH_NUM     	2			
#define RTU_PING_PONG_BUF_MAX_NUM    	200

#define RTU_SPI1_DAC_DMA_CH_A				0	//0-3  DAC A-D
#define RTU_SPI1_DAC_DMA_CH_B				1	//0-3  DAC A-D
#define RTU_SPI1_DAC_DMA_CH_C				2	//0-3  DAC A-D
#define RTU_SPI1_DAC_DMA_CH_D				3	//0-3  DAC A-D


typedef struct
{
	int16_t ping_pong_buf_1K[RTU_PING_PONG_BUF_MAX_NUM];
}PING_PONG_BUF_1K_T;

typedef struct
{
	int16_t ping_pong_buf_4K[RTU_PING_PONG_BUF_MAX_NUM*4];
}PING_PONG_BUF_4K_T;

typedef struct
{

	uint16_t ping_pong_flg;						// 0-1 ��ʾ����buf0��1�����
	uint16_t ping_pong_save_num;				//�������
	PING_PONG_BUF_1K_T ping_pong_buf[RTU_PING_PONG_BUF_CH_NUM];
}RTU_PING_PONG_DATA_T,*RTU_PING_PONG_DATA_P;


typedef struct
{
		uint16_t ping_pong_flg; 					// 0-1 ��ʾ����buf0��1�����
		uint16_t ping_pong_save_num;				//�������
		PING_PONG_BUF_4K_T ping_pong_buf[RTU_PING_PONG_BUF_CH_NUM];
}RTU_PING_PONG_SPI1_DATA_T,*RTU_PING_PONG_SPI1_DATA_P;


//#define DISABLE_ADC_TIMER_FOR_JJJ		//�ر�adc��timer��spi for��������
#define TESTFORJJJ 	//Ϊ�𾧾�������Ƶ������չ����---��ʽ�汾Ҫȥ��

#endif /* MAINVTR_H */
