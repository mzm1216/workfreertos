
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


typedef unsigned char  uint8;                                           /*  无符??位整型变??          */
typedef signed   char  int8;                                            /*  有符??位整型变??          */
typedef unsigned short uint16;                                          /*  无符??6位整型变??         */
typedef signed   short int16;                                           /*  有符??6位整型变??         */
typedef unsigned long   uint32;                                          /*  无符??2位整型变??         */
typedef signed   long   int32;                                           /*  有符??2位整型变??         */

typedef float          fp32;                                            /*  单精度浮点数??2位长度）    */
typedef double         fp64;                                            /*  双精度浮点数??4位长度）    */
typedef float          fp32_t;                                            /*  单精度浮点数??2位长度）    */
typedef double         fp64_t;                                            /*  双精度浮点数??4位长度）    */
	
	
#define	bit01	unsigned char
#define	uchar	unsigned char
#define	uint		unsigned short


#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h" 


//#define MAX(x, y)				(((x) > (y)) ? (x) : (y))

#define PARAM_OFFSET(Strct, Field) ((unsigned long)(unsigned long*)&(((Strct *)0)->Field))

#define DEBUG_SYS			1

#define DEBUG_TASK_ADC		2

#define DEBUG_TASK_UART 	3

#define DEBUG_TASK_1S		4

#define DEBUG_FOR_I2C 		5
#define DEBUG_TASP_APP		6

typedef __packed union{
                uint16 w;
                uint8 bb[2];
             }UintType;//bb[1]为高位  bb[0]为w低位      
typedef __packed union
{
	uint32 w;
	uint8 bb[4];
}UlongType;//bb[1]为高位  bb[0]为w低位  

typedef  union
{
	fp64 w;
	uint8 bb[8];
}Fp64Type;//bb[1]为高位  bb[0]为w低位   

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
}Fp128Type;//bb[1]为高位  bb[0]为w低位 


typedef __packed struct
{
		uint8 high;
		uint8 low;
}inthl;             
typedef __packed union
{
		uint16 w;
		inthl whl;
}UinthlType;//bb[1]为高位  bb[0]为w低位       
typedef  __packed struct
{		
		uint8 low;
		uint8 high;
}intlh;        
typedef __packed union
{
		uint16 w;
		intlh wlh;
}UintlhType;//bb[0]为高位  bb[1]为w低位      


#define   swap(a)     a   =   (uint16)a   >>   8   |   a   <<   8   
#define   swap_self(a)     (uint16)a   >>   8   |   a   <<   8   
#define   swap1(a)   a   =   (uint32)a   >>   24   |   (a   >>   8)   &   0x0000ff00   |   (a   <<   8)   &   0x00ff0000   |   a   <<   24          
#define	  swaprgb(color,r,g,b)		color = (uint16)(r>>3)<<11|((g>>2)<<5)|b>>3
#define suc_respond	0x00//??????	???
#define fail_respond	0x01//				???    


#endif /* MAINVTR_H */
