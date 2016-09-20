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

/******** д��ָ�********/
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

/******** д�����ݡ�********/
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

/******** �������ݡ�********/
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
*	�� �� ��: OLED_InitHard
*	����˵��: ��ʼ��LCD
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_InitHard(void)
{ 
	OLCD_RESET(SETPIN_LOW);  
	Delay(100000);
	OLCD_RESET(SETPIN_HIGH);  

// ׼���������
      Olcd_Write_Command(0xFD); 
	  Olcd_Write_Data(0x12);	       	/* 12H���ܣ�16H�ܾ� */

//���߿��أ�
	  Olcd_Write_Command(0xAE); 		   	/* AEH�����ߣ�AF������ */ 

// �����д��ڣ�
	  Olcd_Write_Command(0x15); 	
	  Olcd_Write_Data(0x1c); 	       
	  Olcd_Write_Data(0x5b);            /*256�� */

//�����д��ڣ�
	  Olcd_Write_Command(0x75); 	
	  Olcd_Write_Data(0x00); 	       
	  Olcd_Write_Data(0x3f);            /*64�� */  
 
// ���ÿ����ͼ��
	  Olcd_Write_Command(0xA0); 	
	  Olcd_Write_Data(0x04); 	        /* ѡ���ֽ���ǰ��COMɨ�跴�� ��ˮƽ��ַģʽ */
	  Olcd_Write_Data(0x11);            /*ѡ˫��ģʽ */
 
// ������ʾģʽ��
	  Olcd_Write_Command(0xA6); 	        /* A4Hȫ�أ�A5Hȫ����A6H������A7H���� */
  			 
// ���ò�����ʾ��
	  Olcd_Write_Command(0xA9);     	    /* A8H���벿����ʾ��A9H�˳� */
	 	                         
// Vdd��Դ��
	  Olcd_Write_Command(0xAB); 	
	  Olcd_Write_Data(0x01); 	        /* 00HΪ�ⲿ��01HΪ�ڲ� */

// ����P1 P2 ��
	  Olcd_Write_Command(0xB1); 	        /*P1=29��P2=7 */
	  Olcd_Write_Data(0xE7);

// ����D��Fosc;
	  Olcd_Write_Command(0xB3); 	  
	  Olcd_Write_Data(0xf1);            /*F=9��D=1 */  

//����GPIO��
      Olcd_Write_Command(0xB5);
	  Olcd_Write_Data(0x00);

//����Tp3
      Olcd_Write_Command(0xB6);
	  Olcd_Write_Data(0x08); 

//����Vp2;
     Olcd_Write_Command(0xBB);
	 Olcd_Write_Data(0x1F);

// ����Vcomh;
     Olcd_Write_Command(0xBE);
	 Olcd_Write_Data(0x07); 	  /* Vcomh=0.86Vcc  */  

//���öԱȶȣ�
     Olcd_Write_Command(0xC1);
   //Olcd_Write_Data(0x7F); 	  /* ��256��ѡ��128��  */  
	 Olcd_Write_Data(0x3F);   	  /* ��256��ѡ��64��  */  

//����SEG������Χ��
     Olcd_Write_Command(0xC7);
	 Olcd_Write_Data(0x0F);		  /* ����Χ  */  

//������ʾ�У�
     Olcd_Write_Command(0xCA);
	 Olcd_Write_Data(0x3F);		  /* 64 */   

     Olcd_Write_Command(0xAF); 		  /* ����ʾ  */
 

} 
  
/**********************************************************************************************************
*  ������:OLCD_PutPixel
*  ����˵��:��1�����ص�
*  ����:
* 		  _usX,_usY : ��������
* 		  _usColor	��ʾ��ɫ
*  ����ֵ:��

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
*  ������:OLED_GetPixel
*  ����˵��:��ȡ1�����ص�
*  ����:
* 		  _usX,_usY : ��������
* 		  _usColor	��ʾ��ɫ
*  ����ֵ:��

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
*  ������:Olcd_clear
*  ����˵��:�����Ļ
*  ����:
* 		  _usX,_usY : ��������
* 		  _usColor	��ʾ��ɫ
*  ����ֵ:��

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



