/***********************************************************************************
 * �� �� ��   : Oled_driver.c
 * �� �� ��   : MZM
 * ��������   : 2016��2��17��
 * �ļ�����   : OLEDҺ��������SSD1322�����ļ�
 * ��Ȩ˵��   : Copyright (c) 2008-2016   
 * ��    ��   : 
 * �޸���־   : 
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
	
	{0x11,	0x6870, 	0},   //  am=0 id0-1 =3������(������ʾ)
	//{0x11,	0x685C, 	0},   //  ���ϵ���(������ʾ)  
	
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
	{0x00,	0x0001,	0},	//  OSCEN -1 ����ʹ��
	{0x10,	0x0000, 	0},   	//	SLP  =0  �˳�sleep mode
	{0x07,	0x0033, 	0},  	 //  D1:0	=3 GON DTE=3 
	
	{0x11,	0x6870, 	0},   //  Entry Mode DFM[1:0]: 65Kcolor,ignore sync,am=0 id0-1 =3������(������ʾ)
	
	{0x02,	0x0600,	0},	//  LCD-Driving-Waveform Control
	{0x03,	0x4A38,	0},	// 4Power control 1 Fline �� 6
	{0x01,	0x72EF,	0},	//  Driver Output Control REV:1 GD:1,BGR:0SM:0 RL:0 TB 1
	
	//{0x11,	0x685C, 	0},   //  ���ϵ���(������ʾ)  
	
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
	{0x45,	0x0000, 	0}, //   HSA  00h �� HSA[8:0]
	{0x46,	0x013F, 	0}, //  HEA[8:0] �� 13Fh

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
extern uint8_t LCDbuf_Refresh_Flg;  //���lcdbuf ��־ ��0:δ��䣬����ˢ�£�1������䣬��ˢ��


extern void ssp_send_one_byte(uint8_t* dat,uint32_t len);
void OLED_fill_all_screen(uint8_t m);


/*********************************************************************
* ������:OLED_Refresh_Flg_Set
*  ����:����ˢ��Һ����־--�Ƕ�ʱˢ��
*return:wu
**********************************************************************
*/
void OLED_Refresh_Flg_Set(uint8_t flg)
{
	LCDbuf_Refresh_Flg=flg;
}
/*********************************************************************
* ������:LCD_Flush_Flg_Get
*  ����:��ȡˢ��Һ����־,�Ƕ�ʱˢ��
*return:0����ˢ�£�1ˢ��
**********************************************************************
*/
uint8_t OLED_Refresh_Flg_Get()
{
	return LCDbuf_Refresh_Flg;
}

/*****************************************************************************
 * �� �� ��  : Delay
 * �� �� ��  : MZM
 * ��������  : 2016��2��17��
 * ��������  : ��ʱ����
 * �������  : __IO uint32_t nCount  ��ʱ������uint32
 * �������  : ��
 * �� �� ֵ  : static
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
static void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

/*****************************************************************************
 * �� �� ��  : Oled_gpio_init
 * �� �� ��  : MZM
 * ��������  : 2016��5��19��
 * ��������  : Һ������IO�ڣ�reset��on12��on28
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void Oled_gpio_init()
{
	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, OLCD_RESET_PORT, OLCD_RESET_PIN);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, OLED_ON12_PORT, OLED_ON12_PIN);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, OLED_ON28_PORT, OLED_ON28_PIN);
}

/*****************************************************************************
 * �� �� ��  : OLED_12
 * �� �� ��  : MZM
 * ��������  : 2016��5��19��
 * ��������  : ����Һ��12V����״̬
 * �������  : uint8_t level  powerON 12V  ��ƽ״̬��0--�͵�ƽ��1�ߵ�ƽ
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void OLED_12(uint8_t level)
{
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, OLED_ON12_PORT, OLED_ON12_PIN, level);
}

/*****************************************************************************
 * �� �� ��  : OLED_28
 * �� �� ��  : MZM
 * ��������  : 2016��5��19��
 * ��������  : ����Һ��128V����״̬
 * �������  : uint8_t level  powerON 28V  ��ƽ״̬��0--�͵�ƽ��1�ߵ�ƽ
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void OLED_28(uint8_t level)
{
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, OLED_ON28_PORT, OLED_ON28_PIN, level);
}

/*****************************************************************************
 * �� �� ��  : OLED_RESET
 * �� �� ��  : MZM
 * ��������  : 2016��5��19��
 * ��������  : ����reset�ܽŵ�ƽ
 * �������  : uint8_t level  reset��ƽ״̬
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

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
 * �� �� ��  : Oled_Write_Command
 * �� �� ��  : MZM
 * ��������  : 2016��2��17��
 * ��������  : ��Һ��������SSD1322��������
 * �������  : uint8_t com  ��Ҫ���͵�����
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

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
 * �� �� ��  : Oled_Write_Data
 * �� �� ��  : MZM
 * ��������  : 2016��2��17��
 * ��������  : ��Һ��������SSD1322����һ���ֽڵ�����
 * �������  : uint8_t dat  ��Ҫ���͵����ݣ�һ���ֽ�
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

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

/*ָ��һ��������ָ�룬�ú����������д
��һ���ֽڣ����У� C/D ��Ϊ�͵�ƽ��*/
void oled_wirte_8_A0(uint8_t data)
{
	Oled_Write_Command(data);
}
/*ָ��һ��������ָ�룬�ú����������д
��һ���ֽڣ����У� C/D ��Ϊ�ߵ�ƽ��*/
void oled_wirte_8_A1(uint8_t data)
{
	Oled_Write_Data(data);
}
/*ָ��һ��������ָ�룬�����ݡ�*/
void oled_read_M8_A1(uint8_t *pdata,int numitems)
{
	int i;
	for( i=0;i<numitems;i++)
	{
		*pdata = Oled_Read_Data()&0xFF;
		pdata++;
	}
	
}
/*ָ��һ��������ָ�룬�ú����������д
�����ֽڣ����У� C/D ��Ϊ�ߵ�ƽ��*/
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
 * �� �� ��  : OLED_Read_Register
 * �� �� ��  : MZM
 * ��������  : 2016��8��26��
 * ��������  : ���ڶ�����������
 * �������  : uint16_t index  �Ĵ�����ַ
 * �������  : ��
 * �� �� ֵ  : uint16_t
 * ���ù�ϵ  : 
 * ��    ��  : 

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
 * �� �� ��  : OLED_Write_Register
 * �� �� ��  : MZM
 * ��������  : 2016��8��26��
 * ��������  : �������Ĵ���regaddr��д������ data
 * �������  : uint16_t regaddr  �Ĵ�����ַ
               uint16_t data     д������
 * �������  : ��
 * �� �� ֵ  : static
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
static void OLED_Write_Register(uint16_t regaddr,uint16_t data)
{
#ifdef CMD_BYTE_IS_2
	Oled_Write_Command(0x00);
#endif
	Oled_Write_Command(regaddr&0xFF);

	Oled_Write_Data((data>>8)&0xFF);		//д������	 
	Oled_Write_Data(data&0xFF);			

}

/*****************************************************************************
 * �� �� ��  : OLED_Set_Cur_sor
 * �� �� ��  : MZM
 * ��������  : 2016��8��30��
 * ��������  : ������ʾ����ʼ��ַ����x��y��
 * �������  : uint16_t x  ������
               uint16_t y  ������
 * �������  : ��
 * �� �� ֵ  : static
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
static void OLED_Set_Cur_sor(uint16_t x,uint16_t y)
{
    OLED_Write_Register(0x004e,x); // ��
    OLED_Write_Register(0x004f,y); // ��	
}

/*****************************************************************************
 * �� �� ��  : OLED_Clear_screen
 * �� �� ��  : MZM
 * ��������  : 2016��8��30��
 * ��������  : ��color���������Ļ������������������
 * �������  : uint16_t ClearColor  ȫ�������ɫ
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void OLED_Clear_screen(uint16_t ClearColor)
{
	uint32_t i;
	
	OLED_Set_Cur_sor(0x0000, 0x078);  //������ʼ����Ϊ0�,0
	
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
 * �� �� ��  : tft_ssd2119_init
 * �� �� ��  : MZM
 * ��������  : 2016��8��30��
 * ��������  : Һ��������SSD2119�ĳ�ʼ��
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

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
	{	// �趨���100����ʼ������
		if(TFTSSD2119_INI[i].com == 0xFF) 
		{
			break;
		}
		OLED_Write_Register(TFTSSD2119_INI[i].com&0xFF,TFTSSD2119_INI[i].dat);
		Delay(100);
	}	
	
	Delay(20000);
//	OLED_Clear_screen(0xF800);//����Ļ-����
	OLED_Clear_screen(0x00);//����Ļ-����
	
	Delay(2000000);
}
/*****************************************************************************
 * �� �� ��  : OLED_InitHard
 * �� �� ��  : MZM
 * ��������  : 2016��2��17��
 * ��������  : OLCDҺ��������SSD1322�ĳ�ʼ��
 * �������  : void  ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

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

	
	// ׼���������
	Oled_Write_Command(0xFD); 
	Oled_Write_Data(0x12);	       		/* 12H���ܣ�16H�ܾ� */

	//���߿��أ�
	Oled_Write_Command(0xAE); 		   	/* AEH�����ߣ�AF������ */ 

	// ����D��Fosc;
	Oled_Write_Command(0xB3); 	  
	Oled_Write_Data(0x91);            	/*F=9��D=1 */
	
	//������ʾ�У�
	Oled_Write_Command(0xCA);
	Oled_Write_Data(0x3F);				/* Set MUX Ratio */   

	Oled_Write_Command(0xA2);
	Oled_Write_Data(0x00);				/* Set Display Offset */   
	
	Oled_Write_Command(0xA1);
	Oled_Write_Data(0x00);				/* Set Display Start Line */   

	// �����д��ڣ�
	//Oled_Write_Command(0x15); 	
	//Oled_Write_Data(0x0); 	       
	//Oled_Write_Data(0x3f);           	 /*256�� */

	//�����д��ڣ�
	//Oled_Write_Command(0x75); 	
	//Oled_Write_Data(0x00); 	       
	//Oled_Write_Data(0x3f);            	/*64�� */  
	// ���ÿ����ͼ��	
	Oled_Write_Command(0xA0); 	
	//Olcd_Write_Data_2(0x14,0x11);
	Oled_Write_Data(0x14); 	        	/* ѡ���ֽ���ǰ��COMɨ�跴�� ��ˮƽ��ַģʽ */
	Oled_Write_Data(0x11);           	 /*ѡ��ģʽ */
	
	//����GPIO��
	Oled_Write_Command(0xB5);
	Oled_Write_Data(0x00);
	
	// Vdd��Դ��
	Oled_Write_Command(0xAB); 	
	Oled_Write_Data(0x01); 	        	/* 00HΪ�ⲿ��01HΪ�ڲ� */

	Oled_Write_Command(0xB4); 	
	Oled_Write_Data(0xA0); 	        //δ�ҵ�����
	Oled_Write_Data(0xFD);            	

	//���öԱȶȣ�
	Oled_Write_Command(0xC1);
	Oled_Write_Data(0x9F);   	  		/* ��256��ѡ��160��  */  

	//����SEG������Χ��
	Oled_Write_Command(0xC7);
	Oled_Write_Data(0x0F);		  		/* ���� */  

	// ���ò�����ʾ��
	Oled_Write_Command(0xB9);     	    /* Ĭ�ϻҶ� */
	
	// ����P1 P2 ��
	Oled_Write_Command(0xB1);			/*Set Phase Length */
	Oled_Write_Data(0xE2);
	
	Oled_Write_Command(0xD1); 	
	Oled_Write_Data(0x82); 	      	//δ�ҵ�����
	Oled_Write_Data(0x20);      
	
	//����Vp2;
	Oled_Write_Command(0xBB);
	Oled_Write_Data(0x1F);

	//����Tp3
	Oled_Write_Command(0xB6);
	Oled_Write_Data(0x08); 
	
	// ����Vcomh;
	Oled_Write_Command(0xBE);
	Oled_Write_Data(0x07);	  			/* Vcomh=0.86Vcc	*/	

	// ������ʾģʽ��
	Oled_Write_Command(0xA6); 	        /* A4Hȫ�أ�A5Hȫ����A6H������A7H���� */

	//����
	OLED_fill_all_screen(0);

	Oled_Write_Command(0xAF); 		  /* ����ʾ  */
} 

/*****************************************************************************
 * �� �� ��  : test_ssd2119
 * �� �� ��  : MZM
 * ��������  : 2016��8��30��
 * ��������  : ����Һ��������SSD2119
 * �������  : uint16_t color  �����ɫ
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

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
 * �� �� ��  : OLED_refresh_fun
 * �� �� ��  : MZM
 * ��������  : 2016��2��17��
 * ��������  : ˢ��������ˢ����־0������Ҫˢ����1����Ҫˢ������������������
               ���͵�Һ��������SSD1322��GRAM�У�����ˢ��
 * �������  : void  ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

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
	
	if(OLED_Refresh_Flg_Get() == DISABLE_REFRESH_OLED)//����Ҫˢ��,���ڷǶ�ʱˢ�¡�
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
	
	OLED_Set_Cur_sor(0x0000, 0x0078);  //������ʼ����

#ifdef CMD_BYTE_IS_2
	Oled_Write_Command(0x00);
#endif
	Oled_Write_Command(0x22);
//	Delay(2000);
	
	for (x=0;x<maxx;x++)   /*120row*/
	{
		for (y=0;y<maxy;y++) /* 320�H*/
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
 * �� �� ��  : OLED_fill_all_screen
 * �� �� ��  : MZM
 * ��������  : 2016��2��17��
 * ��������  : ���ȫ��������д0
 * �������  : uint8_t m  ȫ����ɫ����������
 * �������  : �� =
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

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
 * �� �� ��  : OLED_INIT
 * �� �� ��  : MZM
 * ��������  : 2016��5��19��
 * ��������  : Һ��Ӳ����ʼ��
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void OLED_INIT()
{
	#ifdef USED_SSD2119
	tft_ssd2119_init();//Һ����ʼ��
	#else
	OLED_InitHard();	//Һ����ʼ��
	#endif
	
	GUI_Init();			//guiϵͳ��ʼ��
	GUI_Clear();
	GUI_UC_SetEncodeUTF8();
}

/***** ����oled *****/
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
 * �� �� ��  : OLED_refresh_fun
 * �� �� ��  : MZM
 * ��������  : 2016��2��17��
 * ��������  : ˢ��������ˢ����־0������Ҫˢ����1����Ҫˢ������������������
               ���͵�Һ��������SSD1322��GRAM�У�����ˢ��
 * �������  : void  ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void OLED_refresh_step1_fun(void)
{
	
		OLED_Set_Cur_sor(0x0000, 0x0078);  //������ʼ����
	
#ifdef CMD_BYTE_IS_2
		Oled_Write_Command(0x00);
#endif
		Oled_Write_Command(0x22);

	
}

/*****************************************************************************
 * �� �� ��  : OLED_Clear_screen_firstbuf
 * �� �� ��  : MZM
 * ��������  : 2016��8��30��
 * ��������  : ˢ��Һ��������ram����
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void OLED_Clear_screen_firstbuf()
{
	
//	OLED_Clear_screen(0);//����Ļ-����
//	OLED_Clear_screen(0);
	OLED_refresh_fun();
//	LCDBUFF_MZM[0][0]=0x1234;
//	LCDBUFF_MZM[0][1]=0x5678;


//	OLED_refresh_step1_fun();
//	Oled_start_dma_flush((uint8_t *)LCDBUFF_MZM,sizeof(LCDBUFF_MZM));

}

