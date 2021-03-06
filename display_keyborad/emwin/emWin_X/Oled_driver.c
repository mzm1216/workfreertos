/***********************************************************************************
 * 文 件 名   : Oled_driver.c
 * 负 责 人   : MZM
 * 创建日期   : 2016年2月17日
 * 文件描述   : OLED液晶控制器SSD1322驱动文件
 * 版权说明   : Copyright (c) 2008-2016   
 * 其    他   : 
 * 修改日志   : 
***********************************************************************************/
#define OLED_GLOBALS

#include "Oled_driver.h"
#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "config.h"
#include "GUI.h"
#include "GUIConf.h"
#include "semphr.h"



#define SSD1322
#define WRGRAM_BYTES 2
#define P8080
#define CMD_BYTE_IS_2
STCOM_INI  TFTSSD2119_INI[] = 
{ 
	#if 0
	{0x28,	0x0006,	0},   //
	{0x00,	0x0001,	0},	//
	{0x10,	0x0000,	0},   //
	{0x01,	0x72EF,	0},	//  
	{0x02,	0x0600,	0},	//
	{0x03,	0x6870,	0},	// 4A38?
	
	{0x11,	0x6870, 	0},   //  am=0 id0-1 =3从左到右(横屏显示)
	//{0x11,	0x685C, 	0},   //  从上到下(竖屏显示)  
	
	{0x0F,	0x0000, 	0}, //
	
	{0x0b,	0x5308, 	0}, //	
	{0x0c,	0x0003, 	0},   //
	{0x0d,	0x000a, 	0}, //	009?
	{0x0e,	0x2e00, 	0}, //   2300?
	{0x1e,	0x00be, 	0}, //  0010?
	{0x25,	0x8000, 	0},   // A000
	{0x26,	0x3800, 	0}, //
	{0x27,	0x0078, 	0},   //
	{0x4e,	0x0000, 	0}, //	
	{0x4f,	0x0000, 	0}, //
	{0x12,	0x08d9, 	0}, //

	// ---Adjust the Gamma Curve----//
	{0x30,	0x0000, 	0},   	//0007
	{0x31,	0x0104, 	0}, 	//0203   0101?
	{0x32,	0x0100, 	0},   	//0001
	{0x33,	0x0305, 	0}, 	//0007	
	{0x34,	0x0505, 	0}, 	//0007  0707
	{0x35,	0x0305, 	0}, 	//0407
	{0x36,	0x0707, 	0},   	//0407
	{0x37,	0x0300, 	0}, 	//0607	0201
	{0x3a,	0x1200, 	0}, 	//0106
	{0x3b,	0x0800, 	0}, 	//       0900
	{0x07,	0x0033, 	0}, 	//
	#else
//	{0x28,	0x0006,	0},   //
	{0x00,	0x0001,	0},	//  OSCEN -1 晶振使能
	{0x10,	0x0000, 	0},   	//	SLP  =0  退出sleep mode
	{0x07,	0x0033, 	0},  	 //  D1:0	=3 GON DTE=3 
	
	{0x11,	0x6870, 	0},   //  Entry Mode DFM[1:0]: 65Kcolor,ignore sync,am=0 id0-1 =3从左到右(横屏显示)
	
	{0x02,	0x0600,	0},	//  LCD-Driving-Waveform Control
	{0x03,	0x4A38,	0},	// 4Power control 1 Fline × 6
	{0x01,	0x72EF,	0},	//  Driver Output Control REV:1 GD:1,BGR:0SM:0 RL:0 TB 1
	
	//{0x11,	0x685C, 	0},   //  从上到下(竖屏显示)  
	
	{0x0F,	0x0000, 	0}, //Gate Scan Positio 
	
	{0x28,	0x0006, 	0}, //
	{0x25,	0xA000, 	0},   // Frame Frequency Control A:413K framfreq:70HZ
	{0x12,	0x0999, 	0},   //Sleep mode  
	
	{0x26,	0x3800, 	0},   //Analogue Setting Suggested Code for R26H is x3800
	
	{0x0b,	0x5308, 	0}, //	Frame Cycle Contro
	{0x0c,	0x0003, 	0},   //Power Control 2
	{0x0d,	0x0009, 	0}, //	Power Control 3
	{0x0e,	0x2300, 	0}, //   Power Control 4
	{0x1e,	0x0010, 	0}, //  Power Control 5
	
//	{0x44,	0xef78, 	0}, //	Vertical RAM address position EF==240 --78==120
	{0x44,	0xEF00, 	0}, //	Vertical RAM address position EF==240 --78==120
	{0x45,	0x0000, 	0}, //   HSA  00h ≤ HSA[8:0]
	{0x46,	0x013F, 	0}, //  HEA[8:0] ≤ 13Fh

	// ---Adjust the Gamma Curve----//
	{0x30,	0x0000, 	0},   	//0007
	{0x31,	0x0101, 	0}, 	//0203   0101?
	{0x32,	0x0100, 	0},   	//0001
	{0x33,	0x0305, 	0}, 	//0007	
	{0x34,	0x0707, 	0}, 	//0007  0707
	{0x35,	0x0305, 	0}, 	//0407
	{0x36,	0x0707, 	0},   	//0407
	{0x37,	0x0201, 	0}, 	//0607	0201
	{0x3a,	0x1200, 	0}, 	//0106
	{0x3b,	0x0900, 	0}, 	//       0900
	
	{0x07,	0x0033, 0},  	 //  D1:0	=3 GON DTE=3 
	#endif
    	 {0xFF,    	0x0000,  	0}
		
};
#ifdef USED_SSD2119
__align(8) uint16_t LCDBUFF_MZM[120][320] __attribute__ ((section ("VRAM"), zero_init));//256*64*4bit=64Row *64Column*2byte
#else
__align(8) uint8_t LCDBUFF_MZM[64* 128] ;//__attribute__ ((section ("VRAM"), zero_init));//256*64*4bit=64Row *64Column*2byte

#endif


//#ifdef USED_SSD2119
//extern uint16_t LCDBUFF_MZM[120][320];//256*64*4bit=64Row *64Column*2byte
//#else
//extern uint8_t LCDBUFF_MZM[64][128];//256*64*4bit=64Row *64Column*2byte
//#endif
extern uint8_t LCDbuf_Refresh_Flg;  //填充lcdbuf 标志 ，0:未填充，无需刷新，1，已填充，需刷新


extern void ssp_send_one_byte(uint8_t* dat,uint32_t len);
void OLED_fill_all_screen(uint8_t m);


/*********************************************************************
* 函数名:OLED_Refresh_Flg_Set
*  功能:设置刷新液晶标志--非定时刷新
*return:wu
**********************************************************************
*/
void OLED_Refresh_Flg_Set(uint8_t flg)
{
	LCDbuf_Refresh_Flg=flg;
}
/*********************************************************************
* 函数名:LCD_Flush_Flg_Get
*  功能:获取刷新液晶标志,非定时刷新
*return:0，不刷新，1刷新
**********************************************************************
*/
uint8_t OLED_Refresh_Flg_Get()
{
	return LCDbuf_Refresh_Flg;
}

/*****************************************************************************
 * 函 数 名  : Delay
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月17日
 * 函数功能  : 延时函数
 * 输入参数  : __IO uint32_t nCount  延时参数，uint32
 * 输出参数  : 无
 * 返 回 值  : static
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
static void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

/*****************************************************************************
 * 函 数 名  : Oled_gpio_init
 * 负 责 人  : MZM
 * 创建日期  : 2016年5月19日
 * 函数功能  : 液晶控制IO口：reset、on12、on28
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void Oled_gpio_init()
{
	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, OLCD_RESET_PORT, OLCD_RESET_PIN);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, OLED_ON12_PORT, OLED_ON12_PIN);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, OLED_ON28_PORT, OLED_ON28_PIN);
}

/*****************************************************************************
 * 函 数 名  : OLED_12
 * 负 责 人  : MZM
 * 创建日期  : 2016年5月19日
 * 函数功能  : 设置液晶12V开关状态
 * 输入参数  : uint8_t level  powerON 12V  电平状态：0--低电平，1高电平
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void OLED_12(uint8_t level)
{
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, OLED_ON12_PORT, OLED_ON12_PIN, level);
}

/*****************************************************************************
 * 函 数 名  : OLED_28
 * 负 责 人  : MZM
 * 创建日期  : 2016年5月19日
 * 函数功能  : 设置液晶128V开关状态
 * 输入参数  : uint8_t level  powerON 28V  电平状态：0--低电平，1高电平
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void OLED_28(uint8_t level)
{
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, OLED_ON28_PORT, OLED_ON28_PIN, level);
}

/*****************************************************************************
 * 函 数 名  : OLED_RESET
 * 负 责 人  : MZM
 * 创建日期  : 2016年5月19日
 * 函数功能  : 设置reset管脚电平
 * 输入参数  : uint8_t level  reset电平状态
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void OLED_RESET(uint8_t level)
{
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, OLCD_RESET_PORT, OLCD_RESET_PIN, level);
}


void OLED_DC(uint8_t level)
{
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, OLCD_DC_PORT, OLCD_DC_PIN, level);
}
/*****************************************************************************
 * 函 数 名  : Oled_Write_Command
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月17日
 * 函数功能  : 向液晶控制器SSD1322发送命令
 * 输入参数  : uint8_t com  需要发送的命令
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void Oled_Write_Command(uint8_t com)  
{ 
	uint8_t tmpbuf[10];

#ifdef P8080
		SSD1322_IR = com;

#else
	tmpbuf[0]=com;

	OLED_DC(SETPIN_LOW);
	ssp_send_one_byte(tmpbuf,1);
	
#endif
}

/*****************************************************************************
 * 函 数 名  : Oled_Write_Data
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月17日
 * 函数功能  : 向液晶控制器SSD1322发送一个字节的数据
 * 输入参数  : uint8_t dat  需要发送的数据，一个字节
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void Oled_Write_Data(uint8_t dat)  
{ 
	uint8_t tmpbuf[10];

#ifdef P8080
			SSD1322_DR= dat;	
#else
	tmpbuf[0]=dat;

	OLED_DC(SETPIN_HIGH);	
	ssp_send_one_byte(tmpbuf,1);
	OLED_DC(SETPIN_HIGH);	
#endif
}

/*指向一个函数的指针，该函数向控制器写
入一个字节，其中， C/D 线为低电平。*/
void oled_wirte_8_A0(uint8_t data)
{
	Oled_Write_Command(data);
}
/*指向一个函数的指针，该函数向控制器写
入一个字节，其中， C/D 线为高电平。*/
void oled_wirte_8_A1(uint8_t data)
{
	Oled_Write_Data(data);
}
/*指向一个函数的指针，读数据。*/
void oled_read_M8_A1(uint8_t *pdata,int numitems)
{
	int i;
	for( i=0;i<numitems;i++)
	{
		*pdata = Oled_Read_Data()&0xFF;
		pdata++;
	}
	
}
/*指向一个函数的指针，该函数向控制器写
入多个字节，其中， C/D 线为高电平。*/
void oled_write_M8_A1(uint8_t *pdata,int numitems)
{
	int i;
	Oled_start_dma_flush(pdata,numitems);
//	for( i=0;i<numitems;i++)
//	{
//		oled_wirte_8_A1(*(pdata++));
//	}
	
	
}

uint16_t Oled_Read_Data()
{
	uint16_t ret;
	
	ret = SSD1322_DR;
	return ret;
	
}


/*****************************************************************************
 * 函 数 名  : OLED_Read_Register
 * 负 责 人  : MZM
 * 创建日期  : 2016年8月26日
 * 函数功能  : 用于读出数据内容
 * 输入参数  : uint16_t index  寄存器地址
 * 输出参数  : 无
 * 返 回 值  : uint16_t
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
 uint16_t OLED_Read_Register(uint16_t index)
{ 
	static uint16_t ret=0;
	
#ifdef CMD_BYTE_IS_2
	Oled_Write_Command(0x00); 			
#endif
	Oled_Write_Command(index&0xFF);
	//ret = Oled_Read_Data();
	ret = Oled_Read_Data();
	ret <<= 8;
	ret |= Oled_Read_Data();

	return  ret;

}

/*****************************************************************************
 * 函 数 名  : OLED_Write_Register
 * 负 责 人  : MZM
 * 创建日期  : 2016年8月26日
 * 函数功能  : 用于往寄存器regaddr中写入数据 data
 * 输入参数  : uint16_t regaddr  寄存器地址
               uint16_t data     写入内容
 * 输出参数  : 无
 * 返 回 值  : static
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
static void OLED_Write_Register(uint16_t regaddr,uint16_t data)
{
#ifdef CMD_BYTE_IS_2
	Oled_Write_Command(0x00);
#endif
	Oled_Write_Command(regaddr&0xFF);

	Oled_Write_Data((data>>8)&0xFF);		//写入数据	 
	Oled_Write_Data(data&0xFF);			

}

/*****************************************************************************
 * 函 数 名  : OLED_Set_Cur_sor
 * 负 责 人  : MZM
 * 创建日期  : 2016年8月30日
 * 函数功能  : 设置显示的起始地址，（x，y）
 * 输入参数  : uint16_t x  横坐标
               uint16_t y  纵坐标
 * 输出参数  : 无
 * 返 回 值  : static
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
static void OLED_Set_Cur_sor(uint16_t x,uint16_t y)
{
    OLED_Write_Register(0x004e,x); // 行
    OLED_Write_Register(0x004f,y); // 列	
}

/*****************************************************************************
 * 函 数 名  : OLED_Clear_screen
 * 负 责 人  : MZM
 * 创建日期  : 2016年8月30日
 * 函数功能  : 用color填充整个屏幕，可以用来清屏操纵
 * 输入参数  : uint16_t ClearColor  全屏填充颜色
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void OLED_Clear_screen(uint16_t ClearColor)
{
	uint32_t i;
	
	OLED_Set_Cur_sor(0x0000, 0x078);  //设置起始坐标为0�,0
	
#ifdef CMD_BYTE_IS_2
	Oled_Write_Command(0x00);
#endif
	Oled_Write_Command(0x22); 

//	for(i=0;i<2048;i++)
	for(i=0;i<76800;i++)
	{
		Oled_Write_Data((ClearColor>>8)&0xFF);			 
		Oled_Write_Data(ClearColor&0xFF); 
	}
}

/*****************************************************************************
 * 函 数 名  : tft_ssd2119_init
 * 负 责 人  : MZM
 * 创建日期  : 2016年8月30日
 * 函数功能  : 液晶控制器SSD2119的初始化
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void tft_ssd2119_init()
{
	int  i;
  	 Oled_gpio_init();	
	
	OLED_RESET(SETPIN_LOW);  
	
	Delay(2000000);
	
	OLED_RESET(SETPIN_HIGH);  
	Delay(2000000);
	for(i = 0; i < 100; i++)
	{	// 设定最多100条初始化命令
		if(TFTSSD2119_INI[i].com == 0xFF) 
		{
			break;
		}
		OLED_Write_Register(TFTSSD2119_INI[i].com&0xFF,TFTSSD2119_INI[i].dat);
		Delay(100);
	}	
	
	Delay(20000);
//	OLED_Clear_screen(0xF800);//清屏幕-黑屏
	OLED_Clear_screen(0x00);//清屏幕-黑屏
	
	Delay(2000000);
}
/*****************************************************************************
 * 函 数 名  : OLED_InitHard
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月17日
 * 函数功能  : OLCD液晶控制器SSD1322的初始化
 * 输入参数  : void  无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void OLED_InitHard(void)
{ 
	uint32_t i;

	Oled_gpio_init();	
	
	OLED_28(SETPIN_HIGH);
	Delay(200000);
	
	OLED_12(SETPIN_HIGH);
	
	Delay(200000);
	OLED_RESET(SETPIN_LOW);  
	Delay(200000);
	OLED_RESET(SETPIN_HIGH);  
	Delay(200000);

	
	// 准备接受命令；
	Oled_Write_Command(0xFD); 
	Oled_Write_Data(0x12);	       		/* 12H接受，16H拒绝 */

	//休眠开关；
	Oled_Write_Command(0xAE); 		   	/* AEH开休眠，AF关休眠 */ 

	// 设置D和Fosc;
	Oled_Write_Command(0xB3); 	  
	Oled_Write_Data(0x91);            	/*F=9，D=1 */
	
	//设置显示行；
	Oled_Write_Command(0xCA);
	Oled_Write_Data(0x3F);				/* Set MUX Ratio */   

	Oled_Write_Command(0xA2);
	Oled_Write_Data(0x00);				/* Set Display Offset */   
	
	Oled_Write_Command(0xA1);
	Oled_Write_Data(0x00);				/* Set Display Start Line */   

	// 设置列窗口；
	//Oled_Write_Command(0x15); 	
	//Oled_Write_Data(0x0); 	       
	//Oled_Write_Data(0x3f);           	 /*256列 */

	//设置行窗口；
	//Oled_Write_Command(0x75); 	
	//Oled_Write_Data(0x00); 	       
	//Oled_Write_Data(0x3f);            	/*64行 */  
	// 设置可逆地图；	
	Oled_Write_Command(0xA0); 	
	//Olcd_Write_Data_2(0x14,0x11);
	Oled_Write_Data(0x14); 	        	/* 选高字节在前，COM扫描反向 ，水平地址模式 */
	Oled_Write_Data(0x11);           	 /*选屏模式 */
	
	//设置GPIO；
	Oled_Write_Command(0xB5);
	Oled_Write_Data(0x00);
	
	// Vdd来源；
	Oled_Write_Command(0xAB); 	
	Oled_Write_Data(0x01); 	        	/* 00H为外部，01H为内部 */

	Oled_Write_Command(0xB4); 	
	Oled_Write_Data(0xA0); 	        //未找到命令
	Oled_Write_Data(0xFD);            	

	//设置对比度；
	Oled_Write_Command(0xC1);
	Oled_Write_Data(0x9F);   	  		/* 共256级选第160级  */  

	//设置SEG电流范围；
	Oled_Write_Command(0xC7);
	Oled_Write_Data(0x0F);		  		/* 不减 */  

	// 设置部分显示；
	Oled_Write_Command(0xB9);     	    /* 默认灰度 */
	
	// 设置P1 P2 ；
	Oled_Write_Command(0xB1);			/*Set Phase Length */
	Oled_Write_Data(0xE2);
	
	Oled_Write_Command(0xD1); 	
	Oled_Write_Data(0x82); 	      	//未找到命令
	Oled_Write_Data(0x20);      
	
	//设置Vp2;
	Oled_Write_Command(0xBB);
	Oled_Write_Data(0x1F);

	//设置Tp3
	Oled_Write_Command(0xB6);
	Oled_Write_Data(0x08); 
	
	// 设置Vcomh;
	Oled_Write_Command(0xBE);
	Oled_Write_Data(0x07);	  			/* Vcomh=0.86Vcc	*/	

	// 设置显示模式；
	Oled_Write_Command(0xA6); 	        /* A4H全关，A5H全开，A6H正常，A7H反显 */

	//清屏
	OLED_fill_all_screen(0);

	Oled_Write_Command(0xAF); 		  /* 开显示  */
} 

/*****************************************************************************
 * 函 数 名  : test_ssd2119
 * 负 责 人  : MZM
 * 创建日期  : 2016年8月30日
 * 函数功能  : 测试液晶控制器SSD2119
 * 输入参数  : uint16_t color  填充颜色
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void test_ssd2119(uint16_t color)
{
	uint32_t i,j;

#ifdef USED_SSD2119
	for(i=0;i<120;i++)
		{
			for(j=0;j<320;j++)               
			{
				
#ifdef TEST_LED_AREA
				if(i==0)
				{
					LCDBUFF_MZM[i][j]= BLUE|RED;
				}
				else if(i==119)
				{
					LCDBUFF_MZM[i][j]= BLUE;
				}
				else{
					LCDBUFF_MZM[i][j]= RED;
					}
#else
				LCDBUFF_MZM[i][j]= color;

#endif
			}
		}
	#endif
}
/*****************************************************************************
 * 函 数 名  : OLED_refresh_fun
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月17日
 * 函数功能  : 刷屏函数，刷屏标志0，不需要刷屏，1，需要刷屏，将缓冲区的数据
               发送的液晶控制器SSD1322的GRAM中，进行刷屏
 * 输入参数  : void  空
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void OLED_refresh_fun(void)
{
	uint32_t x,y,maxx,maxy;
	#ifdef USED_SSD2119
	uint16_t *ptrbuf=NULL;
	uint16_t color;
	#else
	uint8_t *ptrbuf=NULL;
	#endif
	uint8_t dat;
	
	if(OLED_Refresh_Flg_Get() == DISABLE_REFRESH_OLED)//不需要刷新,用于非定时刷新。
	{
		return;
	}
#ifndef USED_SSD2119
	Oled_Write_Command(0x15);  /* Set Column Address */
	Oled_Write_Data(0x1C);     /* Start = 0 1c 5b*/
	Oled_Write_Data(0x5B);     /* End = 127 */
	Oled_Write_Command(0x75);  /* Set Row Address */
	Oled_Write_Data(0x00);     /* Start = 0 */
	Oled_Write_Data(0x3F);     /* End = 80 */

	Oled_Write_Command(0x5C);
	Delay(2000);
	maxx = 64;
	maxy = 128;
	ptrbuf=LCDBUFF_MZM;
	for (x=0;x<maxx;x++)   /* 64row*/
	{
		for (y=0;y<maxy;y++) /* 128*2=256 column a nibble of command is a dot*/
		{
			dat = (uint8_t) *ptrbuf;

			Oled_Write_Data((dat >> 4) | (dat << 4));
//			Oled_Write_Data(0x0);

		 	ptrbuf++;
		}
	}
	#else

	
	maxx = 120;
	maxy = 320;
	ptrbuf=(uint16_t *)&LCDBUFF_MZM[0][0];
	
	OLED_Set_Cur_sor(0x0000, 0x0078);  //设置起始坐标

#ifdef CMD_BYTE_IS_2
	Oled_Write_Command(0x00);
#endif
	Oled_Write_Command(0x22);
//	Delay(2000);
	
	for (x=0;x<maxx;x++)   /*120row*/
	{
		for (y=0;y<maxy;y++) /* 320錒*/
		{
			color =*ptrbuf;
			Oled_Write_Data(((color)>>8)&0xFF);			 
			Oled_Write_Data((color)&0xFF); 	
		 	ptrbuf++;
		}
	}
#endif
	return;
}

/*****************************************************************************
 * 函 数 名  : OLED_fill_all_screen
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月17日
 * 函数功能  : 清空全屏，满屏写0
 * 输入参数  : uint8_t m  全屏颜色参数，保留
 * 输出参数  : 无 =
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void OLED_fill_all_screen(uint8_t m)
{
	uint32_t j,i;
	uint8_t f,s;
	
	Oled_Write_Command(0x15);  /* Set Column Address */
	Oled_Write_Data(0x1C);     /* Start = 0 1c 5b*/
	Oled_Write_Data(0x5B);     /* End = 127 */
	Oled_Write_Command(0x75);  /* Set Row Address */
	Oled_Write_Data(0x00);     /* Start = 0 */
	Oled_Write_Data(0x3F);     /* End = 80 */
	Oled_Write_Command(0x5C);
	Delay(20000);

	for (j=0;j<64;j++)   /* 64row*/
	 {
		  for (i=0;i<64;i++) /* 128*2=256 column a nibble of command is a dot*/
		  {
			  Oled_Write_Data(0);
			  Oled_Write_Data(0);

		  }
	 }
}

/*****************************************************************************
 * 函 数 名  : OLED_INIT
 * 负 责 人  : MZM
 * 创建日期  : 2016年5月19日
 * 函数功能  : 液晶硬件初始化
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void OLED_INIT()
{
	#ifdef USED_SSD2119
	tft_ssd2119_init();//液晶初始化
	#else
	OLED_InitHard();	//液晶初始化
	#endif
	
	GUI_Init();			//gui系统初始化
	GUI_Clear();
	GUI_UC_SetEncodeUTF8();
}

/***** 清零oled *****/
void test_screen(uint8_t X1,uint8_t Y1,uint8_t X2,uint8_t Y2,uint8_t pixel)
{
	uint32_t j,i;
	Oled_Write_Command(0x15);
	
	//Olcd_Write_Data_2((Y&0xFF00)>>8,Y&0xFF);
	Oled_Write_Data(Y1);          //Column Start Address  
	Oled_Write_Data(0x3F);             	//Column End Address   y+n/4-1 n=40
	Oled_Write_Command(0x75);
	
	//Olcd_Write_Data_2((X&0xFF00)>>8,X&0xFF);
	Oled_Write_Data(X1);          //Column Start Address  
	Oled_Write_Data(0x3F);             	//Column End Address   y+n/4-1 n=40

	Oled_Write_Command(0x5C);	  				//write RAM command
	
	Delay(20000);
	Oled_Write_Data(pixel);

}

/*****************************************************************************
 * 函 数 名  : OLED_refresh_fun
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月17日
 * 函数功能  : 刷屏函数，刷屏标志0，不需要刷屏，1，需要刷屏，将缓冲区的数据
               发送的液晶控制器SSD1322的GRAM中，进行刷屏
 * 输入参数  : void  空
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void OLED_refresh_step1_fun(void)
{
	
		OLED_Set_Cur_sor(0x0000, 0x0078);  //设置起始坐标
	
#ifdef CMD_BYTE_IS_2
		Oled_Write_Command(0x00);
#endif
		Oled_Write_Command(0x22);

	
}

/*****************************************************************************
 * 函 数 名  : OLED_Clear_screen_firstbuf
 * 负 责 人  : MZM
 * 创建日期  : 2016年8月30日
 * 函数功能  : 刷新液晶控制器ram数据
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void OLED_Clear_screen_firstbuf()
{
	
//	OLED_Clear_screen(0);//清屏幕-黑屏
//	OLED_Clear_screen(0);
	OLED_refresh_fun();
//	LCDBUFF_MZM[0][0]=0x1234;
//	LCDBUFF_MZM[0][1]=0x5678;


//	OLED_refresh_step1_fun();
//	Oled_start_dma_flush((uint8_t *)LCDBUFF_MZM,sizeof(LCDBUFF_MZM));

}

