#ifndef OLED_DRIVER_H
#define OLED_DRIVER_H
#include "board.h"

#ifdef  OLED_GLOBALS
#define olcd_ext
#else
#define olcd_ext  extern
#endif

void OLED_PutPixel(uint16_t X, uint16_t Y, uint16_t Color);
uint8_t OLED_GetPixel(uint16_t X, uint16_t Y);

#ifdef LPC4370_ULINK2
#define OLCD_RESET_PORT 		1
#define OLCD_RESET_PIN			11

#define OLCD_DC_PORT 		0
#define OLCD_DC_PIN			10

#else
#ifdef USED_SSD1322_BX
/*便携式 P0_4*/
#define OLCD_RESET_PORT 		0
#define OLCD_RESET_PIN			4

#else
#define OLCD_RESET_PORT 		5
#define OLCD_RESET_PIN			2
#endif
#define OLED_ON12_PORT 			5
#define OLED_ON12_PIN			5

#define OLED_ON28_PORT 			5
#define OLED_ON28_PIN			6

// TFT液晶模块硬件操作宏 (用nCS2)
#define  SSD1322_IR    	(*((volatile uint8_t*) 0x1C000000)) 
#define  SSD1322_DR    	(*((volatile uint8_t *) 0x1C000008))

#define  SSD1322_RST	    (1 << 2)	//p5
#define  SSD1322_12ON	    (1 << 5)	//p5
#define  SSD1322_28ON	    (1 << 6)	//p5

#define  LCD_ADDRESS_BASE    	 0x1C000008
#define  LCD_DATA_BASE_ADDR    ( 0x1C000008)

#endif
//#define OLCD_CS_PORT 		1
//#define OLCD_CS_PIN			1


#define OLCD_DC_PORT 		0
#define OLCD_DC_PIN			10

//#define OLCD_WR_PORT 		1
//#define OLCD_WR_PIN			1

//#define OLCD_RD_PORT 		1
//#define OLCD_RD_PIN			1

//#define OLCD_D0_PORT 		1
//#define OLCD_D0_PIN			1
//#define OLCD_D1_PORT 		0
//#define OLCD_D1_PIN			1
//#define OLCD_D2_PORT 		0
//#define OLCD_D2_PIN			1
//#define OLCD_D3_PORT 		0
//#define OLCD_D3_PIN			1
//#define OLCD_D4_PORT 		0
//#define OLCD_D4_PIN			1
//#define OLCD_D5_PORT 		0
//#define OLCD_D5_PIN			1
//#define OLCD_D6_PORT 		0
//#define OLCD_D6_PIN			1
//#define OLCD_D7_PORT 		0
//#define OLCD_D7_PIN			1

#define SETPIN_HIGH			1	//设置管脚为 高电平
#define SETPIN_LOW			0	//设置管脚为低电平
//Chip_GPIO_SetPinOutHigh
//#define OLCD_RESET(a) 		Chip_GPIO_SetPinOutHigh(LPC_GPIO_PORT, OLCD_RESET_PORT, OLCD_RESET_PORT, a)//片选
//#define OLCD_DC(a) 			Chip_GPIO_SetPinState(LPC_GPIO_PORT, OLCD_DC_PORT, OLCD_DC_PORT, a)//数据、命令选择

//#define OLCD_RESET(a) 		Chip_GPIO_SetPinState(LPC_GPIO_PORT, OLCD_RESET_PORT, OLCD_RESET_PORT, a)//片选
//#define OLCD_CS(a) 			Chip_GPIO_SetPinState(LPC_GPIO_PORT, OLCD_CS_PORT, OLCD_CS_PORT, a)//片选
//#define OLCD_DC(a) 			Chip_GPIO_SetPinState(LPC_GPIO_PORT, OLCD_DC_PORT, OLCD_DC_PORT, a)//数据、命令选择
//#define OLCD_WR(a) 			Chip_GPIO_SetPinState(LPC_GPIO_PORT, OLCD_WR_PORT, OLCD_WR_PORT, a)//读命令
//#define OLCD_RD(a) 			Chip_GPIO_SetPinState(LPC_GPIO_PORT, OLCD_RD_PORT, OLCD_RD_PORT, a)//读命令

//#define OLCD_DATA(a)	{a&=0xFF;Chip_GPIO_SetPortOutHigh(LPC_GPIO_PORT,OLCD_D0_PORT,a);a^=a;a&=0xFF;Chip_GPIO_SetPortOutLow(LPC_GPIO_PORT,OLCD_D0_PORT,a);};
//#define OLCD_READ_DATA(uint8* a)	{*a=Chip_GPIO_ReadValue(LPC_GPIO_PORT,OLCD_D0_PORT);};

    /* exact-width signed integer types */
typedef   signed          char int8_t;
typedef   signed short     int int16_t;
typedef   signed           int int32_t;

    /* exact-width unsigned integer types */
typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;

#define ENABLE_REFRESH_OLED			1
#define DISABLE_REFRESH_OLED		0
// 定义初始化数据的结构
typedef  struct
{  uint16_t  com;				// 命令字
   uint16_t  dat;				// 数据
   uint16_t  dly;				// 命令延时控制(如50)
}  STCOM_INI;
#define USE_SSD2119
//颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define RED           	 0xF800
#define GREEN         	 0x07E0
#define BLUE         	 0x001F  

#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define MAGENTA       	 0xF81F
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
void oled_wirte_8_A0(uint8_t data);
void oled_wirte_8_A1(uint8_t data);
void oled_read_M8_A1(uint8_t *pdata,int numitems);
void oled_write_M8_A1(uint8_t *pdata,int numitems);
uint16_t Oled_Read_Data(void);
#endif
