/*
 * @brief Repetitive Interrupt Timer example
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
#include <stdio.h>

/** @defgroup PERIPH_RITIMER_5410X Repetitive Timer example
 * @ingroup EXAMPLES_PERIPH_5410X
 * @include "periph\ritimer\readme.txt"
 */

/**
 * @}
 */

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* 100mS (10Hz) timer rate */
#define TIMER_RATE_MS (100)

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/* Timer tick counter */
volatile uint32_t ostimerTick;

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	Handle interrupt from SysTick timer
 * @return	Nothing
 */
void RIT_IRQHandler(void)
{
	Chip_RIT_ClearInt(LPC_RITIMER);
	Board_LED_Toggle(0);
	ostimerTick++;
}

/**
 * @brief	main routine for OS timer example
 * @return	Function should not exit.
 */
int main(void)
{
	SystemCoreClockUpdate();
	Board_Init();

//	Board_LED_Set(0, false);

	/* Initialize RI Timer */
	Chip_RIT_Init(LPC_RITIMER);

	/* Setup timer period to 10Hz (100mS) */
	Chip_RIT_SetTimerInterval64(LPC_RITIMER, TIMER_RATE_MS);

	/* Enable RI Timer */
	NVIC_EnableIRQ(RIT_IRQn);

	/* Enable RI Timer and clear on compare match or ....
	   use Chip_RIT_EnableCTRL(LPC_RITIMER, RIT_CTRL_ENCLR | RIT_CTRL_TEN);
	   to save space. */
	Chip_RIT_Enable(LPC_RITIMER);
	Chip_RIT_CompClearEnable(LPC_RITIMER);

	/* Wait 10s and then stop timer */
	while (ostimerTick < (10 * (1000 / TIMER_RATE_MS))) {
		__WFI();
	}

	/* Disable RI Timer */
	Chip_RIT_Disable(LPC_RITIMER);

	return 0;
}
