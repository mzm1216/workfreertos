/***********************************************************************************
 * 文 件 名   : driver_timer.c
 * 负 责 人   : MZM
 * 创建日期   : 2016年3月2日
 * 文件描述   : 定时器0和定时器1的驱动相关代码，包括定时器初始化，定时器中断
                服务程序等接口
 * 版权说明   : Copyright (c) 2008-2016   北京市万格数码通讯科技有限公司
 * 其    他   : 
 * 修改日志   : 
***********************************************************************************/

#include "board.h"
#include <stdio.h>
#include "config.h"
#include "driver_adc.h"
//#include "driver_spi.h"

//#define TIMER_TEST //测试时使用，timer0 位100ms，timer1位100ms，，正式版本取消注释，
extern xQueueHandle Sem_Adc_Deal_A;//Adc 数据处理信号量---10ms执行一次

/*****************************************************************************
 * Private types/enumerations/variables
 ***********************************************0*****************************/
#ifndef TIMER_TEST
#define TICKRATE_HZ1 (100000)/* 96 ticks per second */
#define TICKRATE_HZ2 (1000000)/* 100 ticks per second */
#else
#define TICKRATE_HZ1 (1000)/* 10 ticks per second */
#define TICKRATE_HZ2 (2)/* 20 ticks per second */
#endif
uint8_t Timer1_Flg;//timer 1 中断标志，中断服务程序中将置为TRUE

uint8_t first[3]={TRUE,TRUE,TRUE};
typedef struct
{
	uint32_t FAN_first_value[3];
	uint32_t FAN_fg[3];
	uint32_t FAN_fg1;
	uint32_t FAN_fg2;
}FAN_FG_T;

FAN_FG_T rtu_fan_fg;
/*****************************************************************************
 * 函 数 名  : Set_CT31B1_TimerISR_Flg
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月2日
 * 函数功能  : 修改timer中断标志，开启定时器1是，将标志置为false，定时器1中
               断将把其修改为True
 * 输入参数  : uint8_t flg  设置标志，TRUE或者False
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void Set_CT31B1_TimerISR_Flg(uint8_t flg)
{
	Timer1_Flg = flg;
}

/*****************************************************************************
 * 函 数 名  : Get_CT31B1_TimerISR_Flg
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月2日
 * 函数功能  : 读取 定时器中断标志
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : True,产生中断，0false为产生中断
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
uint8_t Get_CT31B1_TimerISR_Flg()
{
	return Timer1_Flg;
}


/*****************************************************************************
 * 函 数 名  : CT32B0_IRQHandler
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月2日
 * 函数功能  : 定时器0 ISR，96K的定时器，启动adc通道转换。
 * 输入参数  : void  wu
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void CT32B1_IRQHandler(void)
{
	signed portBASE_TYPE xadcTaskWoken = pdFALSE;

	if (Chip_TIMER_MatchPending(LPC_TIMER1, 1))
	{
		
		Chip_TIMER_ClearMatch(LPC_TIMER1, 1);
//		Board_LED_Toggle(1);
		Rtu_Start_ADC_A_Conversion();
		
//		Board_LED_Toggle(1);
//	
//		signed portBASE_TYPE xadcTaskWoken = pdFALSE;
//		xSemaphoreGiveFromISR(Sem_Adc_Deal_A,&xadcTaskWoken);
//		portYIELD_FROM_ISR(xadcTaskWoken);
	}
}

/*****************************************************************************
 * 函 数 名  : CT32B1_IRQHandler                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月2日
 * 函数功能  : 定时器1ISR，设置中断标志
 * 输入参数  : void  wu
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void CT32B2_IRQHandler(void)
{
	if (Chip_TIMER_MatchPending(LPC_TIMER2, 1))	
	{
		spi0_spi1_dma_start_queue(Get_Second_Send_CH_Port());//启动 第二个dma通道

		Chip_TIMER_ClearMatch(LPC_TIMER2, 1);
		
		NVIC_DisableIRQ(CT32B2_IRQn);
		
	}
}

/*****************************************************************************
 * 函 数 名  : CT32B0_IRQHandler
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月2日
 * 函数功能  : 定时器1ISR，设置中断标志
 * 输入参数  : void  wu
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void CT32B0_IRQHandler(void)
{

#if 1
	uint8_t i;
	uint32_t lastvalue,nowvalue;
	//3 路 捕获
	for(i=0;i<3;i++)
	{
		if (Chip_TIMER_CapturePending(LPC_TIMER0, i))	//判断捕获中断
		{
			if(first[i]==TRUE)//第一次上电 捕获 value
			{
				first[i]=FALSE;
				rtu_fan_fg.FAN_first_value[i] = Chip_TIMER_ReadCapture(LPC_TIMER0,i);
			}
			else
			{
				nowvalue = Chip_TIMER_ReadCapture(LPC_TIMER0,i);//读取本次捕获值
				nowvalue += 0xFFFFFFFF;							//溢出处理
				lastvalue =  rtu_fan_fg.FAN_first_value[i];		//上次捕获value
				rtu_fan_fg.FAN_fg[i] = (nowvalue - lastvalue)% 0xFFFFFFFF;//保存转速value
				rtu_fan_fg.FAN_first_value[i] = nowvalue;		//保存本次捕获值
			}
			Chip_TIMER_ClearCapture(LPC_TIMER0, i);
		}
	}
#else

	if (Chip_TIMER_CapturePending(LPC_TIMER0, 0))	
	{
		if(first[0]==TRUE)
		{
			first[0]==FALSE;
			rtu_fan_fg.FAN_first_value = Chip_TIMER_ReadCapture(LPC_TIMER0,0);
		}
		else
		{
			rtu_fan_fg.FAN_fg[0] = Chip_TIMER_ReadCapture(LPC_TIMER0,0);
			rtu_fan_fg.FAN_fg[0] += 0xFFFFFFFF;
			rtu_fan_fg.FAN_fg[0] -= rtu_fan_fg.FAN_first_value;
			rtu_fan_fg.FAN_fg[0] = rtu_fan_fg.FAN_fg[0]% 0xFFFFFFFF;
		}
		Chip_TIMER_ClearMatch(LPC_TIMER0, 0);
	}
	else 	if (Chip_TIMER_CapturePending(LPC_TIMER0, 1))	
	{
		
		if(first[1]==TRUE)
		{
			first[1]==FALSE;
			rtu_fan_fg.FAN_first_value = Chip_TIMER_ReadCapture(LPC_TIMER0,1);
		}
		else
		{
			rtu_fan_fg.FAN_fg[1] = Chip_TIMER_ReadCapture(LPC_TIMER0,0);
			rtu_fan_fg.FAN_fg[1] += 0xFFFFFFFF;
			rtu_fan_fg.FAN_fg[1] -= rtu_fan_fg.FAN_first_value;
			rtu_fan_fg.FAN_fg[1] = rtu_fan_fg.FAN_fg[1]% 0xFFFFFFFF;
		}
	}
	else 	if (Chip_TIMER_CapturePending(LPC_TIMER0, 2))	
	{
		
		if(first[2]==TRUE)
		{
			first[2]==FALSE;
			rtu_fan_fg.FAN_first_value = Chip_TIMER_ReadCapture(LPC_TIMER0,2);
		}
		else
		{
			rtu_fan_fg.FAN_fg[2] = Chip_TIMER_ReadCapture(LPC_TIMER0,2);
			rtu_fan_fg.FAN_fg[2] += 0xFFFFFFFF;
			rtu_fan_fg.FAN_fg[2] -= rtu_fan_fg.FAN_first_value;
			rtu_fan_fg.FAN_fg[2] = rtu_fan_fg.FAN_fg[2]% 0xFFFFFFFF;
		}
	}
	else 	if (Chip_TIMER_CapturePending(LPC_TIMER0, 3))	
	{		
		Chip_TIMER_ClearMatch(LPC_TIMER0, 3);
	}
#endif
}


/*****************************************************************************
 * 函 数 名  : RTU_Timer1Dev_Stop
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月25日
 * 函数功能  : 禁用timer1中断
 * 输入参数  : void  wu 
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void RTU_Timer1Dev_Stop()
{
	NVIC_DisableIRQ(CT32B1_IRQn);
}
/*****************************************************************************
 * 函 数 名  : RTU_Timer1Dev_Stop
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月25日
 * 函数功能  : 禁用timer1中断
 * 输入参数  : void  wu 
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void RTU_Timer2Dev_Stop()
{
	NVIC_DisableIRQ(CT32B2_IRQn);
}


/*****************************************************************************
 * 函 数 名  : RTU_Timer1Dev_Restart
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月25日
 * 函数功能  : Timer初始化，定时器1默认为1us中断，参数valueperusec，是1us的倍数
 							并且不应该超过10
 * 输入参数  : void  wu 
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void RTU_Timer2Dev_Restart(uint32_t valueperus)
{
	uint32_t timerBaseClock;

	NVIC_DisableIRQ(CT32B2_IRQn);

	/* Initialize Timer 0 and Timer 1 */
	Chip_TIMER_Init(LPC_TIMER2);

	/* Setup prescale value on Timer 1 for lower resolution */
	Chip_TIMER_PrescaleSet(LPC_TIMER2, 0);

	/* Reset timers */
	Chip_TIMER_Reset(LPC_TIMER2);

	/* Enable both timers to generate interrupts when time matches */
	Chip_TIMER_MatchEnableInt(LPC_TIMER2, 1);

	/* Get rate of timer base clock */
	timerBaseClock = Chip_Clock_GetAsyncSyscon_ClockRate();


//		timerBaseClock = 100000000;
	/* Setup Timer 1 for a match twice in a second */
	Chip_TIMER_SetMatch(LPC_TIMER2, 1, (timerBaseClock*valueperus/TICKRATE_HZ2 )-1);

	/* Setup  timers1 to stop when match occurs */
	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER2, 1);//

	/* Start both timers */
	Chip_TIMER_Enable(LPC_TIMER2);

	/* Clear both timers of any pending interrupts */
	NVIC_ClearPendingIRQ(CT32B2_IRQn);

	NVIC_SetPriority(CT32B2_IRQn,ISR_PRIORITY_TIMER2);
	/* Enable both timer interrupts */
	NVIC_EnableIRQ(CT32B2_IRQn);
	//NVIC_DisableIRQ(CT32B0_IRQn);

}


uint32_t Rtu_Get_capreg()
{
	return Chip_TIMER_ReadCapture(LPC_TIMER0,0);

}

/*****************************************************************************
 * 函 数 名  : RTU_TimerDev_Init
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月25日
 * 函数功能  : Timer初始化
 * 输入参数  : void  wu 
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void RTU_Timer0capDev_Init(uint8_t chnum)
{
	uint32_t timerBaseClock;

	/* Initialize Timer 0 and Timer 1 */
	Chip_TIMER_Init(LPC_TIMER0);

		/* Setup prescale value on Timer 0 to PCLK */
		Chip_TIMER_PrescaleSet(LPC_TIMER0, 0);
	
		/* Reset timers */
		Chip_TIMER_Reset(LPC_TIMER0);
	
		Chip_TIMER_CaptureRisingEdgeEnable(LPC_TIMER0,chnum);
		Chip_TIMER_CaptureEnableInt(LPC_TIMER0,chnum);

		/* Start both timers */
		Chip_TIMER_Enable(LPC_TIMER0);
	
		/* Clear both timers of any pending interrupts */
		NVIC_ClearPendingIRQ(CT32B0_IRQn);
	
		/* Enable both timer interrupts */
		
		NVIC_SetPriority(CT32B0_IRQn,ISR_PRIORITY_TIMER0);
		NVIC_EnableIRQ(CT32B0_IRQn);
		//RTU_Timer2Dev_Stop();

}
void RTU_PWM_CAP_Dev_Init()
{
	uint32_t timerBaseClock;

	/* Initialize Timer 0 and Timer 1 */
	Chip_TIMER_Init(LPC_TIMER0);

	/* Setup prescale value on Timer 0 to PCLK */
	Chip_TIMER_PrescaleSet(LPC_TIMER0, 0);

	/* Reset timers */
	Chip_TIMER_Reset(LPC_TIMER0);
	
	Chip_TIMER_CaptureFallingEdgeEnable(LPC_TIMER0,0);
	Chip_TIMER_CaptureEnableInt(LPC_TIMER0,0);
	Chip_TIMER_CaptureFallingEdgeEnable(LPC_TIMER0,1);
	Chip_TIMER_CaptureEnableInt(LPC_TIMER0,1);
	Chip_TIMER_CaptureFallingEdgeEnable(LPC_TIMER0,2);
	Chip_TIMER_CaptureEnableInt(LPC_TIMER0,2);

	/* Start both timers */
	Chip_TIMER_Enable(LPC_TIMER0);

	/* Clear both timers of any pending interrupts */
	NVIC_ClearPendingIRQ(CT32B0_IRQn);

	/* Enable both timer interrupts */
	
	NVIC_SetPriority(CT32B0_IRQn,ISR_PRIORITY_TIMER0);
	NVIC_EnableIRQ(CT32B0_IRQn);
	//RTU_Timer2Dev_Stop();

}

/*****************************************************************************
 * 函 数 名  : RTU_TimerDev_Init
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月25日
 * 函数功能  : Timer初始化
 * 输入参数  : void  wu 
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void RTU_TimerDev_Init(void)
{
	uint32_t timerBaseClock;
	

	/* Initialize Timer 0 and Timer 1 */
	Chip_TIMER_Init(LPC_TIMER1);
	Chip_TIMER_Init(LPC_TIMER2);
	/* Setup prescale value on Timer 0 to PCLK */
	Chip_TIMER_PrescaleSet(LPC_TIMER2, 0);
	/* Setup prescale value on Timer 1 for lower resolution */
	Chip_TIMER_PrescaleSet(LPC_TIMER1, 0);

	/* Reset timers */
	Chip_TIMER_Reset(LPC_TIMER2);
	Chip_TIMER_Reset(LPC_TIMER1);

	/* Enable both timers to generate interrupts when time matches */
	Chip_TIMER_MatchEnableInt(LPC_TIMER2, 1);
	Chip_TIMER_MatchEnableInt(LPC_TIMER1, 1);

	/* Get rate of timer base clock */
	timerBaseClock = Chip_Clock_GetAsyncSyscon_ClockRate();
	//timerBaseClock = 100000000;
	/* Setup Timer 0 for a match every 1s */
	Chip_TIMER_SetMatch(LPC_TIMER2, 1, (timerBaseClock / TICKRATE_HZ2)-1);

	/* Setup Timer 1 for a match twice in a second */
	Chip_TIMER_SetMatch(LPC_TIMER1, 1, (timerBaseClock / TICKRATE_HZ1)-1);

	/* Setup both timers to restart when match occurs */
	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER1, 1);
	
//	/* Setup  timers1 to stop when match occurs */
//	Chip_TIMER_ResetOnMatchDisable(LPC_TIMER2, 1);
	/* Setup  timers1 to stop when match occurs */
	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER2, 1);

	/* Start both timers */
	Chip_TIMER_Enable(LPC_TIMER2);
	Chip_TIMER_Enable(LPC_TIMER1);
	NVIC_SetPriority(CT32B2_IRQn,ISR_PRIORITY_TIMER2);
	NVIC_SetPriority(CT32B1_IRQn,ISR_PRIORITY_TIMER1);
	/* Clear both timers of any pending interrupts */
	NVIC_ClearPendingIRQ(CT32B2_IRQn);
	NVIC_ClearPendingIRQ(CT32B1_IRQn);

	/* Enable both timer interrupts */
	NVIC_EnableIRQ(CT32B1_IRQn);
	NVIC_EnableIRQ(CT32B2_IRQn);
	RTU_Timer2Dev_Stop();

}


uint32_t rtu_start_cap(uint8_t chnum)
{

//	first[chnum] = TRUE;
//	RTU_Timer0capDev_Init(chnum);

//	while(first[chnum] != FALSE)
//	{
//		;
//	}
	return  rtu_fan_fg.FAN_fg[chnum];//返回  转速的捕获计数器的值
}
void printf_fg(uint8_t ch)
{
	rt_kprintf(0,"fg_CH[%d]\r\n", ch);
	rt_kprintf(0,"fg_first[%x][%d]\r\n", rtu_fan_fg.FAN_first_value[ch]);
	rt_kprintf(0,"fg_fg[%x][%d]\r\n", rtu_fan_fg.FAN_fg[ch]);
	rt_kprintf(0,"fg1-fg0[%x][%d]\r\n", rtu_fan_fg.FAN_fg[ch]-rtu_fan_fg.FAN_first_value[ch]);
	
}

