#include "Oled_driver.h"
#include "board.h"
#define OLCD_GLOBALS
#define SSD1322


static void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

void OLCD_DATA(uint8_t a)
{
	a&=0xFF;
	Chip_GPIO_SetPortOutHigh(LPC_GPIO_PORT,OLCD_D0_PORT,a);
	a^=a;
	a&=0xFF;
	Chip_GPIO_SetPortOutLow(LPC_GPIO_PORT,OLCD_D0_PORT,a);
}

/******** 写“指令”********/
void Olcd_Write_Command(uint8_t com)  
{ 
#ifdef P8080
	OLCD_CS(SETPIN_LOW);
	OLCD_DC(SETPIN_LOW);
	OLCD_WR(SETPIN_LOW);
	
 	OLCD_DATA(com&0xFF);
	
	OLCD_WR(SETPIN_HIGH);
	OLCD_CS(SETPIN_HIGH);
#else

	OLCD_DC(SETPIN_LOW);
	ssp_send_one_byte(com);


#endif
}

/******** 写“数据”********/
void Olcd_Write_Data(uint8_t dat)  
{ 
#ifdef P8080

	OLCD_CS(SETPIN_LOW);
	OLCD_DC(SETPIN_HIGH);
	OLCD_WR(SETPIN_LOW);
	
 	OLCD_DATA(dat&0xFF);
	
	OLCD_WR(SETPIN_HIGH);
	OLCD_CS(SETPIN_HIGH);
	
#else
	OLCD_DC(SETPIN_HIGH);	
	ssp_send_one_byte(dat);
	
#endif
}

/******** 读“数据”********/
void Olcd_Read_Data(uint8_t *dat)  
{ 
	OLCD_CS(SETPIN_LOW);
	OLCD_DC(SETPIN_HIGH);
	OLCD_RD(SETPIN_LOW);
	
 	OLCD_DATA(*dat&0xFF);
	
	OLCD_RD(SETPIN_HIGH);
	OLCD_CS(SETPIN_HIGH);
}

void OLCD_READ_DATA(uint8_t* a)
{
	//*a=Chip_GPIO_ReadValue(LPC_GPIO_PORT,OLCD_D0_PORT);
}


/*
*********************************************************************************************************
*	函 数 名: OLED_InitHard
*	功能说明: 初始化LCD
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void OLED_InitHard(void)
{ 
	OLCD_RESET(SETPIN_LOW);  
	Delay(100000);
	OLCD_RESET(SETPIN_HIGH);  

// 准备接受命令；
      Olcd_Write_Command(0xFD); 
	  Olcd_Write_Data(0x12);	       	/* 12H接受，16H拒绝 */

//休眠开关；
	  Olcd_Write_Command(0xAE); 		   	/* AEH开休眠，AF关休眠 */ 

// 设置列窗口；
	  Olcd_Write_Command(0x15); 	
	  Olcd_Write_Data(0x1c); 	       
	  Olcd_Write_Data(0x5b);            /*256列 */

//设置行窗口；
	  Olcd_Write_Command(0x75); 	
	  Olcd_Write_Data(0x00); 	       
	  Olcd_Write_Data(0x3f);            /*64行 */  
 
// 设置可逆地图；
	  Olcd_Write_Command(0xA0); 	
	  Olcd_Write_Data(0x04); 	        /* 选高字节在前，COM扫描反向 ，水平地址模式 */
	  Olcd_Write_Data(0x11);            /*选双屏模式 */
 
// 设置显示模式；
	  Olcd_Write_Command(0xA6); 	        /* A4H全关，A5H全开，A6H正常，A7H反显 */
  			 
// 设置部分显示；
	  Olcd_Write_Command(0xA9);     	    /* A8H进入部分显示，A9H退出 */
	 	                         
// Vdd来源；
	  Olcd_Write_Command(0xAB); 	
	  Olcd_Write_Data(0x01); 	        /* 00H为外部，01H为内部 */

// 设置P1 P2 ；
	  Olcd_Write_Command(0xB1); 	        /*P1=29，P2=7 */
	  Olcd_Write_Data(0xE7);

// 设置D和Fosc;
	  Olcd_Write_Command(0xB3); 	  
	  Olcd_Write_Data(0xf1);            /*F=9，D=1 */  

//设置GPIO；
      Olcd_Write_Command(0xB5);
	  Olcd_Write_Data(0x00);

//设置Tp3
      Olcd_Write_Command(0xB6);
	  Olcd_Write_Data(0x08); 

//设置Vp2;
     Olcd_Write_Command(0xBB);
	 Olcd_Write_Data(0x1F);

// 设置Vcomh;
     Olcd_Write_Command(0xBE);
	 Olcd_Write_Data(0x07); 	  /* Vcomh=0.86Vcc  */  

//设置对比度；
     Olcd_Write_Command(0xC1);
   //Olcd_Write_Data(0x7F); 	  /* 共256级选第128级  */  
	 Olcd_Write_Data(0x3F);   	  /* 共256级选第64级  */  

//设置SEG电流范围；
     Olcd_Write_Command(0xC7);
	 Olcd_Write_Data(0x0F);		  /* 满范围  */  

//设置显示行；
     Olcd_Write_Command(0xCA);
	 Olcd_Write_Data(0x3F);		  /* 64 */   

     Olcd_Write_Command(0xAF); 		  /* 开显示  */
 

} 
  
/**********************************************************************************************************
*  函数名:OLCD_PutPixel
*  功能说明:画1个像素点
*  参数:
* 		  _usX,_usY : 像素坐标
* 		  _usColor	显示颜色
*  返回值:无

**********************************************************************************************************/
void OLCD_PutPixel(uint16_t X, uint16_t Y, uint8_t Color)
{
	Olcd_Write_Command(0x15);
	Olcd_Write_Data((Y&0xFF00)>>8);          //Column Start Address  
	Olcd_Write_Data(Y&0xFF);             	//Column End Address   y+n/4-1 n=40
	Olcd_Write_Command(0x75);
	Olcd_Write_Data((X&0xFF00)>>8);          //Column Start Address  
	Olcd_Write_Data(X&0xFF);             	//Column End Address   y+n/4-1 n=40

	Olcd_Write_Command(0x5d);	  				//write RAM command
	Olcd_Write_Data(Color);

}

/**********************************************************************************************************
*  函数名:OLED_GetPixel
*  功能说明:读取1个像素点
*  参数:
* 		  _usX,_usY : 像素坐标
* 		  _usColor	显示颜色
*  返回值:无

**********************************************************************************************************/
uint8_t OLED_GetPixel(uint16_t X, uint16_t Y)
{
	uint8_t color=0xFF;
	
	Olcd_Write_Command(0x15);
	Olcd_Write_Data((Y&0xFF00)>>8);          //Column Start Address  
	Olcd_Write_Data(Y&0xFF);             	//Column End Address   y+n/4-1 n=40
	Olcd_Write_Command(0x75);
	Olcd_Write_Data((X&0xFF00)>>8);          //Column Start Address  
	Olcd_Write_Data(X&0xFF);             	//Column End Address   y+n/4-1 n=40

	Olcd_Write_Command(0x5D);	  				//write RAM command

	OLCD_READ_DATA(&color);
	
	//Olcd_Read_Data(&color);
	return color;
}
/**********************************************************************************************************
*  函数名:Olcd_clear
*  功能说明:清空屏幕
*  参数:
* 		  _usX,_usY : 像素坐标
* 		  _usColor	显示颜色
*  返回值:无

**********************************************************************************************************/
void Olcd_clear()
{
	uint32_t i;
	Olcd_Write_Command(0xa4); 

	Olcd_Write_Command(0x5c); 

	for(i=0;i<4096;i++)  
	{  
		Olcd_Write_Data(0);  
		Olcd_Write_Data(0);   
	}   

	Olcd_Write_Command(0xa6); 
}



