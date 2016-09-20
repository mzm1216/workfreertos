/*
 * @brief FreeRTOS task based blinky example
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2014
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "i2c_dev.h"


/** @defgroup PERIPH_FREERTOS_BLINKY_5410X FreeRTOS blinky example
 * @ingroup EXAMPLES_FREERTOS_5410X
 * @include "freertos\blinky\readme.txt"
 */

/**
 * @}
 */

#define TEST_PWM_SCT

extern void RTU_Uart0Dev_Init(void);
extern void RTU_AdcDev_Init(void);
extern void RTU_I2CDev_Init(void);
extern void RTU_SpiDev_Init(void);

xQueueHandle Sem_Adc_Deal_A;//Adc 数据处理信号量---10ms执行一次
xQueueHandle Sem_Adc_Deal_B;//Adc 数据处理信号量---10ms执行一次

xQueueHandle Sem_Adc_Deal_A_End;//Adc 数据处理信号量---10ms执行一次
xQueueHandle Sem_Adc_Deal_B_End;//Adc 数据处理信号量---10ms执行一次

xQueueHandle Sem_FGU_INI_Deal;//eint1 数据处理信号量---20ms执行一次


xQueueHandle Sem_Eint2_Deal;//eint2 数据处理信号量---
xQueueHandle Sem_gpint0F_Deal;//p0.13下降沿数据处理信号量---
xQueueHandle Sem_gpint0R_Deal;//p0.13 上升沿数据处理信号量---
xQueueHandle Sem_gpint2_Deal;//p2.13 数据处理信号量---

xQueueHandle Sem_DEBUG_start;//Adc 数据处理信号量---10ms执行一次


/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/* Sets up system hardware */
static void prvSetupHardware(void)
{
	SystemCoreClockUpdate();
	Board_Init();
	
	Set_All_Debug_Level();		//开启调试信息 ----调试
	RTU_Uart0Dev_Init();			//	uart0 		初始化

	I3c_GPIO_init();				// 	IO模拟 3线spi	初始化
#ifndef DISABLE_ADC_TIMER_FOR_JJJ
	RTU_AdcDev_Init();			//	ADC 		初始化
#endif
	RTU_I2CDev_Init();			// 	I2C			初始化
#ifndef DISABLE_ADC_TIMER_FOR_JJJ
	RTU_SpiDev_Init();			//	SPI			初始化	
#endif
	RTU_GpioINT_Init();			//	GPIO中断		初始化
	RTU_SE98_PCA9535_Init();		//	I2c设备		初始化
#ifndef DISABLE_ADC_TIMER_FOR_JJJ
	RTU_TimerDev_Init();			//	定时器		初始化
#endif
//	RTU_PWM_CAP_Dev_Init();		// 	PWM捕获		初始化
//	RTU_PWMDev_Init();			// 	PWM产生		初始化
	
//	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 21,
//					 (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_DIGITAL_EN));
}

extern void print_buf_A(uint32_t cnt);
extern void print_buf_B(uint32_t cnt);
/* LED1 toggle thread */
static void vLEDTask1(void *pvParameters) {
	bool LedState = false;
	uint32_t capreg,cnt1=0;
//	uint8_t tmpbuf[10]={0x01,0x20,0x13,0x49,0x00,0x00,0x49,0x04};
	uint8_t tmpbuf[20]="DEBUG:123";
	uint8_t tmpbuf1[20]={0x01,0x20,0x13,0x4b,0x00,0x00,0x4b,0x04};
	uint8_t tmpbuf2[20]={0x01,0x20,0x13,0x4d,0x00,0x70,0x00,0xBD,0x04};

	int led_out_0, led_out_1, led_dp = 0, led_step = 1, currLed = 0;
#ifdef TEST_PWM_SCT
	while (1) {
		cnt1++;
//		if (cnt1 >= 2) {
//			led_dp += 1;
//			if (led_dp < 0) {
//				led_dp = 0;
//				led_step = 1;
//			}
//			if (led_dp > 100) {
//				led_dp = 100;
//				led_step = -1;
//				currLed = 1 - currLed;
//			}

//			/* Increment or Decrement Dutycycle by 0.5% every 10ms */
//			if (currLed) {
//				led_out_0 = led_dp;
//				led_out_1 = 100;
//			}
//			else {
//				led_out_0 = 100;
//				led_out_1 = led_dp;
//			}
			
//			Board_LED_Set(1, 0);
//			sctpwm(2,90);
//			sctpwm(2,10*(cnt1%10));
//			sctpwm(2,led_out_1);
//			sctpwm(3,led_out_0);
//			sctpwm(4,led_out_1);
//			
//			rtu_start_cap(0);
//			printf_fg(0);
//			Board_LED_Set(1, 1);
//			rtu_start_cap(0);
//			rtu_start_cap(1);
//			rtu_start_cap(2);
//			cnt1 = 0;
			//break;
//		}
		
		vTaskDelay(configTICK_RATE_HZ*2 );
	}

	#else

	while (1) {
	LedState = (bool) !LedState;
	
//	Board_LED_Set(0, LedState);
		//SPI_Send_One_Data(LedState,0,LedState);
//	//print_buf_A(1);
//	//print_buf_B(1);
//	
//	//__WFI();

//		//test_uart();
//		//capreg=Rtu_Get_capreg();
//		
//		//RTU_PWMDev_Init();
//	//	test_Pwm(cnt1++);

//RTU_Uart0_Write("DEBUG:3	",10);
//		cnt1++;
//		if(cnt1%3==0)
//		{
//				RTU_Uart0_Write(tmpbuf,20);
//		}
//		if(cnt1%3==1)
//		{
//				RTU_Uart0_Write(tmpbuf1,20);
//		}
//		if(cnt1%3==2)
//		{
//				RTU_Uart0_Write(tmpbuf1,20);
//		}

		cnt1++;
		if(cnt1%20 == 0)
		{
			
//			Board_LED_Set(1, 1);
		#define JJJHZ (380000000+73350000)
//			Set_SKY72310_LD(0,JJJHZ,19200000,0,0,3,0,0x0f,0,0);
//			Set_PLL_Freq(1,3600000,192000,0,0,0,0,0,0,0);
//			Set_PLL_Freq(2,3600000,192000,0,0,0,0,0,0,0);
		}
		if(cnt1%20==10)
			{
			
//			Board_LED_Set(1, 0);
			}
//		/* About a 3Hz on/off toggle rate */
		vTaskDelay(configTICK_RATE_HZ / 100);

	}
#endif
}

/* LED2 toggle thread */
static void vLEDTask2(void *pvParameters) {
	bool LedState = false;

	while (1) {
//		Board_LED_Set(0, LedState);
		LedState = (bool) !LedState;

		/* About a 7Hz on/off toggle rate */
		vTaskDelay(configTICK_RATE_HZ / 1);
	}
}

/* UART (or output) thread */
static void vUARTTask(void *pvParameters) {
	int tickCnt = 0;

	while (1) {
		rt_kprintf("Tick: %d \r\n", tickCnt);
		tickCnt++;

		/* About a 1s delay here */
		vTaskDelay(configTICK_RATE_HZ);
	}
}

/*****************************************************************************
 * Public functions
****************************************************************************/
void init_Semaphore_fun()
{	
	vSemaphoreCreateBinary(Sem_Adc_Deal_A);//ADC数据处理互斥量
	vSemaphoreCreateBinary(Sem_Adc_Deal_B);//ADC数据处理互斥量
	xSemaphoreTake(Sem_Adc_Deal_A,10);
	xSemaphoreTake(Sem_Adc_Deal_B,10);
	rtu_uart_sem_init();
	
	vSemaphoreCreateBinary(Sem_FGU_INI_Deal);//FGU_INTz中断 发送
	xSemaphoreTake(Sem_FGU_INI_Deal,10);
	
	vSemaphoreCreateBinary(Sem_DEBUG_start);//debug 信号量
	xSemaphoreTake(Sem_DEBUG_start,10);


	
}

/**
 * @brief	main routine for FreeRTOS blinky example
 * @return	Nothing, function should not exit
 */
int main(void)
{
	
	
	init_Semaphore_fun();
	prvSetupHardware();

	AppTaskCreate();

	/* LED1 toggle thread */
//	xTaskCreate(vLEDTask1, "vTaskLed1",
//				configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
//				(xTaskHandle *) NULL);

//	/* LED2 toggle thread */
//	xTaskCreate(vLEDTask2, "vTaskLed2",
//				configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
//				(xTaskHandle *) NULL);

	/* UART output thread, simply counts seconds */
//	xTaskCreate(vUARTTask, "vTaskUart",
//				configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
//				(xTaskHandle *) NULL);

//	/* Start the scheduler */
	xSemaphoreGive(Sem_DEBUG_start);

	vTaskStartScheduler();

	/* Should never arrive here */
	return 1;
}
