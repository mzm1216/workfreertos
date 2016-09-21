/***********************************************************************************
 * �� �� ��   : driver_timer.c
 * �� �� ��   : MZM
 * ��������   : 2016��3��2��
 * �ļ�����   : ��ʱ��0�Ͷ�ʱ��1��������ش��룬������ʱ����ʼ������ʱ���ж�
                �������Ƚӿ�
 * ��Ȩ˵��   : Copyright (c) 2008-2016   �������������ͨѶ�Ƽ����޹�˾
 * ��    ��   : 
 * �޸���־   : 
***********************************************************************************/

#include "board.h"
#include <stdio.h>
#include "config.h"
#include "driver_adc.h"
//#include "driver_spi.h"

//#define TIMER_TEST //����ʱʹ�ã�timer0 λ100ms��timer1λ100ms������ʽ�汾ȡ��ע�ͣ�
extern xQueueHandle Sem_Adc_Deal_A;//Adc ���ݴ����ź���---10msִ��һ��

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
uint8_t Timer1_Flg;//timer 1 �жϱ�־���жϷ�������н���ΪTRUE

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
 * �� �� ��  : Set_CT31B1_TimerISR_Flg
 * �� �� ��  : MZM
 * ��������  : 2016��3��2��
 * ��������  : �޸�timer�жϱ�־��������ʱ��1�ǣ�����־��Ϊfalse����ʱ��1��
               �Ͻ������޸�ΪTrue
 * �������  : uint8_t flg  ���ñ�־��TRUE����False
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void Set_CT31B1_TimerISR_Flg(uint8_t flg)
{
	Timer1_Flg = flg;
}

/*****************************************************************************
 * �� �� ��  : Get_CT31B1_TimerISR_Flg
 * �� �� ��  : MZM
 * ��������  : 2016��3��2��
 * ��������  : ��ȡ ��ʱ���жϱ�־
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : True,�����жϣ�0falseΪ�����ж�
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
uint8_t Get_CT31B1_TimerISR_Flg()
{
	return Timer1_Flg;
}


/*****************************************************************************
 * �� �� ��  : CT32B0_IRQHandler
 * �� �� ��  : MZM
 * ��������  : 2016��3��2��
 * ��������  : ��ʱ��0 ISR��96K�Ķ�ʱ��������adcͨ��ת����
 * �������  : void  wu
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

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
 * �� �� ��  : CT32B1_IRQHandler                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
 * �� �� ��  : MZM
 * ��������  : 2016��3��2��
 * ��������  : ��ʱ��1ISR�������жϱ�־
 * �������  : void  wu
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void CT32B2_IRQHandler(void)
{
	if (Chip_TIMER_MatchPending(LPC_TIMER2, 1))	
	{
		spi0_spi1_dma_start_queue(Get_Second_Send_CH_Port());//���� �ڶ���dmaͨ��

		Chip_TIMER_ClearMatch(LPC_TIMER2, 1);
		
		NVIC_DisableIRQ(CT32B2_IRQn);
		
	}
}

/*****************************************************************************
 * �� �� ��  : CT32B0_IRQHandler
 * �� �� ��  : MZM
 * ��������  : 2016��3��2��
 * ��������  : ��ʱ��1ISR�������жϱ�־
 * �������  : void  wu
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void CT32B0_IRQHandler(void)
{

#if 1
	uint8_t i;
	uint32_t lastvalue,nowvalue;
	//3 · ����
	for(i=0;i<3;i++)
	{
		if (Chip_TIMER_CapturePending(LPC_TIMER0, i))	//�жϲ����ж�
		{
			if(first[i]==TRUE)//��һ���ϵ� ���� value
			{
				first[i]=FALSE;
				rtu_fan_fg.FAN_first_value[i] = Chip_TIMER_ReadCapture(LPC_TIMER0,i);
			}
			else
			{
				nowvalue = Chip_TIMER_ReadCapture(LPC_TIMER0,i);//��ȡ���β���ֵ
				nowvalue += 0xFFFFFFFF;							//�������
				lastvalue =  rtu_fan_fg.FAN_first_value[i];		//�ϴβ���value
				rtu_fan_fg.FAN_fg[i] = (nowvalue - lastvalue)% 0xFFFFFFFF;//����ת��value
				rtu_fan_fg.FAN_first_value[i] = nowvalue;		//���汾�β���ֵ
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
 * �� �� ��  : RTU_Timer1Dev_Stop
 * �� �� ��  : MZM
 * ��������  : 2016��2��25��
 * ��������  : ����timer1�ж�
 * �������  : void  wu 
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void RTU_Timer1Dev_Stop()
{
	NVIC_DisableIRQ(CT32B1_IRQn);
}
/*****************************************************************************
 * �� �� ��  : RTU_Timer1Dev_Stop
 * �� �� ��  : MZM
 * ��������  : 2016��2��25��
 * ��������  : ����timer1�ж�
 * �������  : void  wu 
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void RTU_Timer2Dev_Stop()
{
	NVIC_DisableIRQ(CT32B2_IRQn);
}


/*****************************************************************************
 * �� �� ��  : RTU_Timer1Dev_Restart
 * �� �� ��  : MZM
 * ��������  : 2016��2��25��
 * ��������  : Timer��ʼ������ʱ��1Ĭ��Ϊ1us�жϣ�����valueperusec����1us�ı���
 							���Ҳ�Ӧ�ó���10
 * �������  : void  wu 
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

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
 * �� �� ��  : RTU_TimerDev_Init
 * �� �� ��  : MZM
 * ��������  : 2016��2��25��
 * ��������  : Timer��ʼ��
 * �������  : void  wu 
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

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
 * �� �� ��  : RTU_TimerDev_Init
 * �� �� ��  : MZM
 * ��������  : 2016��2��25��
 * ��������  : Timer��ʼ��
 * �������  : void  wu 
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

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
	return  rtu_fan_fg.FAN_fg[chnum];//����  ת�ٵĲ����������ֵ
}
void printf_fg(uint8_t ch)
{
	rt_kprintf(0,"fg_CH[%d]\r\n", ch);
	rt_kprintf(0,"fg_first[%x][%d]\r\n", rtu_fan_fg.FAN_first_value[ch]);
	rt_kprintf(0,"fg_fg[%x][%d]\r\n", rtu_fan_fg.FAN_fg[ch]);
	rt_kprintf(0,"fg1-fg0[%x][%d]\r\n", rtu_fan_fg.FAN_fg[ch]-rtu_fan_fg.FAN_first_value[ch]);
	
}

