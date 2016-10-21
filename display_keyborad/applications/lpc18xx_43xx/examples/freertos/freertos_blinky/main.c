/***********************************************************************************
 * 文 件 名   : freertos_blinky.c
 * 负 责 人   : MZM
 * 创建日期   : 2016年2月17日
 * 文件描述   : 移植FreeRtos系统到LPC4370ulink2开发板应用程序入口函数（）
 * 版权说明   : Copyright (c) 2008-2016   
 * 其    他   : 
 * 修改日志   : 
***********************************************************************************/

#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "GUI.h"
#include "GUIConf.h"
#include "semphr.h"
//#define NOT_USE_GUI   //不适用gui
uint8_t LCDbuf_Refresh_Flg=0;  //???lcdbuf ??? ??0:δ?????????￡?1????????????
extern xQueueHandle refresh_LCD_sem;
extern 	uint16_t LCDBUFF_MZM1[8][320];//256*64*4bit=64Row *64Column*2byte

extern void OLED_InitHard(void);
extern void OLED_PutPixel(uint16_t X, uint16_t Y, uint16_t Color);

//extern GUI_CONST_STORAGE GUI_FONT GUI_FontYH16;

extern GUI_CONST_STORAGE GUI_FONT GUI_FontHT24;

#if 0

extern unsigned GUI_CONST_STORAGE char _acxxY[];
extern GUI_CONST_STORAGE GUI_BITMAP bmxxY;
extern unsigned GUI_CONST_STORAGE char _acen[];

#endif
#ifdef MORE_PIC

extern GUI_CONST_STORAGE GUI_BITMAP bmen;
extern GUI_CONST_STORAGE GUI_BITMAP bmxxzj;
extern GUI_CONST_STORAGE GUI_BITMAP bmwoxx;
extern GUI_CONST_STORAGE GUI_BITMAP bmxm1;
extern GUI_CONST_STORAGE GUI_BITMAP bmxm2;
extern GUI_CONST_STORAGE GUI_BITMAP bmxm3;
extern GUI_CONST_STORAGE GUI_BITMAP bmxm5;
extern GUI_CONST_STORAGE GUI_BITMAP bmxxgd;

extern GUI_CONST_STORAGE GUI_BITMAP bms1;
extern GUI_CONST_STORAGE GUI_BITMAP bms2;
extern GUI_CONST_STORAGE GUI_BITMAP bms3;
extern GUI_CONST_STORAGE GUI_BITMAP bm48;
extern GUI_CONST_STORAGE GUI_BITMAP bms85;

#endif
//#ifdef USED_SSD2119
//__align(8) uint16_t LCDBUFF_MZM[120* 320] __attribute__ ((section ("VRAM"), zero_init));//256*64*4bit=64Row *64Column*2byte
//#else
//__align(8) uint8_t LCDBUFF_MZM[64* 128] ;//__attribute__ ((section ("VRAM"), zero_init));//256*64*4bit=64Row *64Column*2byte

//#endif
/*****************************************************************************
 * Private functions
 ****************************************************************************/





/*****************************************************************************
 * 函 数 名  : prvSetupHardware
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月17日
 * 函数功能  : 应用设备硬件初始化函数
 * 输入参数  : 8  无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其	 它  : 

*****************************************************************************/
static void prvSetupHardware(void)
{
	SystemCoreClockUpdate();
	Board_Init();

//	/* Initial LED0 state is off */
	Board_LED_Set(LED_WARN, false);
	Board_LED_Set(LED_RX, false);
	Board_LED_Set(LED_TX, true);

	
	uart_dev_init();	//串口设备初始化
#ifndef NOT_USE_GUI
	OLED_INIT();		//液晶初始化
#endif
	kb_dev_init();		//安检设备初始化
}

/*****************************************************************************
 * 函 数 名  : vLEDTask1
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月17日
 * 函数功能  : 闪灯应用线程1
 * 输入参数  : void *pvParameters  传入参数
 * 输出参数  : 无
 * 返 回 值  : static
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
static void vLEDTask1(void *pvParameters) {
	bool LedState = false;
	portBASE_TYPE ret;

	while (1) {
	//	Board_LED_Set(0, LedState);
		LedState = (bool) !LedState;
		ret=xSemaphoreTake(refresh_LCD_sem,1);//
		if(ret==pdPASS)
		{
			OLED_refresh_fun();

		}
//		
		/* About a 3Hz on/off toggle rate */
		vTaskDelay(configTICK_RATE_HZ/10 );
	}
}


static const char * mxh[] = {
	"\xe6\x98\x8e\xe9\x91\xab\xe5\x8d\x8e\xe9\x97\xaf\xe5\xa4\xa9\xe6\xb6\xaf\xef\xbc\x9a"
};
static const char * mcx[] = {
	"\xe9\xa9\xac\xe6\x9c\x9d\xe9\x91\xab\xef\xbc\x8c\xe4\xbd\xa0\xe4\xb8\xaa\xe8\x87\xad\xe5\xb0\x8f\xe5\xad\x90\xef\xbc\x81"
};
static const char * lp[] = {
	"\xe8\x80\x81\xe5\xa9\x86\xef\xbc\x8cI  YOU\xef\xbc\x81\xef\xbc\x81"
};

void oled_top_windows()
{

//	GUI_DrawBitmap((void*)&bms1,0,0);
//					GUI_DrawBitmap((void*)&bms2,64,0);
//					GUI_DrawBitmap((void*)&bms3,128,0);
//					GUI_DrawBitmap((void*)&bm48,192,0);
//					OLED_enable_refresh_ram();

}
void test_key_oled(uint8_t k)
{

#if 0
	GUI_SetFont(&GUI_FontHT24);


	//all_screen_1(k);
			//all_screen_Line(k);
			GUI_Clear();
		//	memset(LCDBUFF_MZM,0,128*64);
			if(k%6==0)
			{
			 GUI_DrawBitmap((void*)&bmen,0,0);
			 GUI_DrawBitmap((void*)&bmxxzj,60,0);
			 GUI_DrawBitmap((void*)&bmwoxx,157,0);
			}
			else if(k%6==1)
			{
			 GUI_DrawBitmap((void*)&bmxm2,0,0);
			 GUI_DrawBitmap((void*)&bmxm3,128,0);
				
			}
			else if(k%6==2){
				GUI_DrawBitmap((void*)&bmxm5,0,0);
				GUI_DrawBitmap((void*)&bmxm1,128,0);	
			}
			else if(k%6==3){
				
				GUI_DrawBitmap((void*)&bmxxgd,0,0);
				GUI_DrawBitmap((void*)&bmxxgd,128,0);	
				
	//			GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	//			GUI_DispStringAt(*mcx, 0, 00);
	//			GUI_SetTextMode(GUI_TEXTMODE_XOR);
	//			GUI_DispStringAt(*mcx, 0, 16);
	//			GUI_SetTextMode(GUI_TM_TRANS);
	//			GUI_DispStringAt(*mcx, 0, 32);
	//				
	//			GUI_SetTextMode(GUI_TEXTMODE_REV);
	//			GUI_DispStringAt(*mcx, 0, 48);
			}
			else if(k%6==4){
	
				
//				GUI_DispStringAt(*mxh, 0, 0);
//				GUI_DispStringAt(*mcx, 0, 20);
//				GUI_DispStringAt(*lp, 0, 40);
				}
			else if(k%6==5){
	
				
				GUI_DrawBitmap((void*)&bms1,0,0);
				GUI_DrawBitmap((void*)&bms2,64,0);
				GUI_DrawBitmap((void*)&bms3,128,0);
				GUI_DrawBitmap((void*)&bm48,192,0);
	
				}
	#endif
		OLED_enable_refresh_ram();

}

/*****************************************************************************
 * 函 数 名  : vLEDTask2
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月17日
 * 函数功能  : 闪灯应用线程
 * 输入参数  : void *pvParameters  传入参数
 * 输出参数  : 无
 * 返 回 值  : static
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
static void vLEDTask2(void *pvParameters) {
	bool LedState = false;
	uint32_t i=0,j=0,k=0,m=14,ret=0;
	
//	ssp_main_init();
	
//	OLED_InitHard();
	tft_ssd2119_init();

	GUI_Init();	
	OLED_fill_all_screen(0);
	
	GUI_SetFont(&GUI_FontHT24);
    GUI_UC_SetEncodeUTF8();
	while (1)
	{
		
	//	Board_LED_Set(0, LedState);
		LedState = (bool) !LedState;
		
#ifdef MORE_PIC
		//m=(0x1<<31);
	#if 0
		//all_screen_1(k);
		//all_screen_Line(k);
		GUI_Clear();
	//	memset(LCDBUFF_MZM,0,128*64);
		if(k%6==0)
		{
		 GUI_DrawBitmap((void*)&bmen,0,0);
		 GUI_DrawBitmap((void*)&bmxxzj,60,0);
		 GUI_DrawBitmap((void*)&bmwoxx,157,0);
		}
		else if(k%6==1)
		{
		 GUI_DrawBitmap((void*)&bmxm2,0,0);
		 GUI_DrawBitmap((void*)&bmxm3,128,0);
			
		}
		else if(k%6==2){
			GUI_DrawBitmap((void*)&bmxm5,0,0);
			GUI_DrawBitmap((void*)&bmxm1,128,0);	
		}
		else if(k%6==3){
			
			GUI_DrawBitmap((void*)&bmxxgd,0,0);
			GUI_DrawBitmap((void*)&bmxxgd,128,0);	
			
//			GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
//			GUI_DispStringAt(*mcx, 0, 00);
//			GUI_SetTextMode(GUI_TEXTMODE_XOR);
//			GUI_DispStringAt(*mcx, 0, 16);
//			GUI_SetTextMode(GUI_TM_TRANS);
//			GUI_DispStringAt(*mcx, 0, 32);
//				
//			GUI_SetTextMode(GUI_TEXTMODE_REV);
//			GUI_DispStringAt(*mcx, 0, 48);
		}
		else if(k%6==4){

			
			GUI_DispStringAt(*mxh, 0, 0);
			GUI_DispStringAt(*mcx, 0, 20);
			GUI_DispStringAt(*lp, 0, 40);
			}
		else if(k%6==5){

			
			GUI_DrawBitmap((void*)&bms1,0,0);
			GUI_DrawBitmap((void*)&bms2,64,0);
			GUI_DrawBitmap((void*)&bms3,128,0);
			GUI_DrawBitmap((void*)&bm48,192,0);

			}

			OLED_Refresh_Flg_Set(ENABLE_REFRESH_OLED);	
			xSemaphoreGive(refresh_LCD_sem);
		#else
		//GUI_Clear();
		//GUI_GotoXY(0,0);
		{
			//GUI_SetBkColor(GUI_WHITE);
			GUI_SetColor(GUI_WHITE);

			//GUI_SetFont(GUI_FONT_8X16); // Buffer old font
			GUI_SetFont(&GUI_FontHT24); // Buffer old font
			//memcpy
			GUI_UC_SetEncodeUTF8();
			//GUI_DispStringAt("TTT text 8x16:",0,0);
		//	GUI_DispStringAt("T丐嫡丁一三二三五三一三明天涯T", 0, 0);
			GUI_DispStringAt("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 0, 16);
			GUI_DispStringAt("abcdefghijklmnopqrstuvwxyz ", 0, 32);
		//	GUI_DispStringAt("一二三四五六七八九十一二三四五六七八九十一二三四五六七八九十 ", 0, 48);
			//GUI_UC_DispString((uint16_t*)"TT");
			//memcpy(LCDBUFF_MZM,acGUI_FontYH16_4E10,50);
			
			//GUI_DispDec(k,1);
			GUI_SetFont(GUI_FONT_8X8);
			//GUI_DispStringAt("THE text68",0,20);
			GUI_SetColor(GUI_GRAY);
		
			ret=LCD_GetBitsPerPixel();
			//GUI_DispDec(k,4);
			GUI_SetFont(GUI_FONT_6X8);
			GUI_SetColor(GUI_DARKGRAY);
			//GUI_DispStringAt("The text 4x6:",0,40);
			
			//GUI_DispDec(k,4);
		}
		#endif
		#endif
		k++;
		
//		OLED_Refresh_Flg_Set(ENABLE_REFRESH_OLED);	
//		xSemaphoreGive(refresh_LCD_sem);
		vTaskDelay(1000);
		/* About a 7Hz on/off toggle rate */
	}
}

/*****************************************************************************
 * 函 数 名  : vUARTTask
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月17日
 * 函数功能  : 串口相关应用线程
 * 输入参数  : void *pvParameters  传入参数
 * 输出参数  : 无
 * 返 回 值  : static
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
static void vUARTTask(void *pvParameters) {
	int tickCnt = 0,count=0,ticklast=0,tickfirst= 0;
	bool LedState = false;
	uint8_t tmpbuf[20];
	static uint16_t devid=0;

	
	tft_ssd2119_init();
	Oled_DMA_init();
//	OLED_Clear_screen(0x000);


	while (1) {
		//
		
		devid = OLED_Read_Register(0);
//		rt_kprintf(1,"Tick: %d ,buflen[%d]last-first[%d] buf[%x] buf1[%x]\r\n", tickCnt,sizeof(LCDBUFF_MZM),ticklast-tickfirst,LCDBUFF_MZM1[0][1],LCDBUFF_MZM1[0][1]);
		rt_kprintf(1,"UU");
		count++;
		if(count %2)
		{
//			test_ssd2119(0x001F);
		}
		else
		{
//			test_ssd2119(0xF800);
			
		}
#if 0
//		OLED_Clear_screen(tickCnt);
		
		tickfirst = xTaskGetTickCount();
		OLED_Refresh_Flg_Set(ENABLE_REFRESH_OLED);
	
		OLED_refresh_fun();
		
		ticklast = xTaskGetTickCount();

#else

		OLED_Clear_screen_firstbuf();
#endif
//		OLED_Clear_screen
		tickCnt = xTaskGetTickCount();
//		
//		Board_LED_Set(0, LedState);
//		Board_LED_Set(1, LedState);
		Board_LED_Set(LED_TX, LedState);
		Board_LED_Set(LED_RX, LedState);
		Board_LED_Set(LED_WARN, LedState);
		LedState = (bool) !LedState;
		/* About a 1s delay here */
		vTaskDelay(configTICK_RATE_HZ);
	}
}

/*****************************************************************************
 * 函 数 名  : main
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月17日
 * 函数功能  : 应用入口函数
 * 输入参数  : void  无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
int main(void)
{
	prvSetupHardware();//硬件初始化
	
	/* LED1 toggle thread */
//	xTaskCreate(vLEDTask1, "vTaskLed1", configMINIMAL_STACK_SIZE,
//			NULL, (tskIDLE_PRIORITY + 1UL), (TaskHandle_t *) NULL);

//	/* LED2 toggle thread */
//	xTaskCreate(vLEDTask2, "vTaskLed2", 256,
//			NULL, (tskIDLE_PRIORITY + 1UL), (TaskHandle_t *) NULL);

//	/* UART output thread, simply counts seconds */
//	xTaskCreate(vUARTTask, "vTaskUart", configMINIMAL_STACK_SIZE,
//			NULL, (tskIDLE_PRIORITY + 1UL), (TaskHandle_t *) NULL);

	/* App task create*/
	AppTaskCreate();	

	/* Start the scheduler */
	vTaskStartScheduler();

	/* Should never arrive here */
	return 1;
}
