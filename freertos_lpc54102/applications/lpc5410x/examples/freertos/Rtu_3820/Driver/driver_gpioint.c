/*
 * @brief Pin Interrupt example
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
#define   GPIOINT_GLOBALS

#include "board.h"
#include "driver_gpioint.h"

extern xQueueHandle Sem_FGU_INI_Deal;//fgu_int 

/*****************************************************************************
 * Public functions
 ****************************************************************************/
void Chip_PININT_ClearIntStatus11(LPC_PIN_INT_T *pPININT, uint32_t pins)
{
	pPININT->IST = pins;
}

/**
 * @brief	Handle interrupt fro                                                      m GPIO pin or GPIO pin mapped to PININT
 * @return	Nothing
 */
void PININT_IRQ_HANDLER(void)
{
	signed portBASE_TYPE xadcTaskWoken = pdFALSE;
	
//	Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH(GPIO_PININT_INDEX));
	Chip_PININT_ClearIntStatus11(LPC_PININT, PININTCH(GPIO_PININT_INDEX));
//	Board_LED_Toggle(0);
	
	xSemaphoreGiveFromISR(Sem_FGU_INI_Deal,&xadcTaskWoken);
	portYIELD_FROM_ISR(xadcTaskWoken);
}

/*****************************************************************************
 * 函 数 名  : RTU_GpioINT_Init
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月3日
 * 函数功能  : fgu_intiGPIO中断初始化
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void RTU_GpioINT_Init()
{

	Chip_PININT_Init(LPC_PININT);

	/* Configure GPIO pin as input */
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_PININT_PORT, GPIO_PININT_PIN);

	/* Configure pin as GPIO */
	Chip_IOCON_PinMuxSet(LPC_IOCON, GPIO_PININT_PORT, GPIO_PININT_PIN,
						 (IOCON_FUNC0 | IOCON_DIGITAL_EN  | IOCON_GPIO_MODE));

	/* Configure pin interrupt selection for the GPIO pin in Input Mux Block */
	Chip_INMUX_PinIntSel(GPIO_PININT_INDEX, GPIO_PININT_PORT, GPIO_PININT_PIN);

	/* Configure channel interrupt as edge sensitive and falling edge interrupt */
	Chip_PININT_ClearIntStatus11(LPC_PININT, PININTCH(GPIO_PININT_INDEX));
	
//	Chip_PININT_SetPinModeLevel(LPC_PININT, PININTCH(GPIO_PININT_INDEX));
//	Chip_PININT_EnableIntLow(LPC_PININT, PININTCH(GPIO_PININT_INDEX));
	
	Chip_PININT_SetPinModeEdge(LPC_PININT, PININTCH(GPIO_PININT_INDEX));
	Chip_PININT_EnableIntLow(LPC_PININT, PININTCH(GPIO_PININT_INDEX));
	
	/* Enable interrupt in the NVIC */
	
	NVIC_SetPriority(PININT_NVIC_NAME,ISR_PRIORITY_PININT);
	NVIC_EnableIRQ(PININT_NVIC_NAME);

	/* Enable wakeup for PININT0 */
//	Chip_SYSCON_EnableWakeup(SYSCON_STARTER_PINT0);

}

uint8_t Rtu_check_FGU_int()
{
	return Chip_GPIO_GetPinState(LPC_GPIO, 0, GPIO_PININT_PIN);
}

