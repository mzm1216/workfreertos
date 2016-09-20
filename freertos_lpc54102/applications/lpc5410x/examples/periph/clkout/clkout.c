/*
 * @brief Clock output example
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

/** @defgroup PERIPH_CLKOUT_5410X Clock output example
 * @ingroup EXAMPLES_PERIPH_5410X
 * @include "periph\clkout\readme.txt"
 */

/**
 * @}
 */

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* Select clock and divider */
#define CLKOUT_SEL      SYSCON_CLKOUTSRC_MAINCLK
#define CLKOUT_DIV      10

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	Application main function
 * @return	Does not return
 */
int main(void)
{
	/* Board Initialization */
	SystemCoreClockUpdate();
	Board_Init();

	/* Clock enables and checks */
	if (CLKOUT_SEL == SYSCON_CLKOUTSRC_RTC) {
		/* Turn on the RTC 32K Oscillator */
		Chip_SYSCON_PowerUp(SYSCON_PDRUNCFG_PD_32K_OSC);
		Chip_Clock_EnableRTCOsc();
	}
	else if (CLKOUT_SEL == SYSCON_CLKOUTSRC_WDTOSC) {
		/* Enable the power to the WDT Oscillator */
		Chip_SYSCON_PowerUp(SYSCON_PDRUNCFG_PD_WDT_OSC);
	}
	else if (CLKOUT_SEL == SYSCON_CLKOUTSRC_CLKIN) {
		/* Setup CLKIN via IOCON (pin muxing) */
		Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 22,
							 (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_DIGITAL_EN));
		if (Chip_Clock_GetExtClockInRate() == 0) {
			/* Can't continue, stop! */
			DEBUGSTR("CLKIN selected for CLKOUT, but CLKIN rate is 0\r\n");
			while (1) {}
		}
	}

	/* Map P0.21 as CLKOUT pin */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 21,
						 (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_DIGITAL_EN));

	/* There isn't too much to this example */
	Chip_Clock_SetCLKOUTSource(CLKOUT_SEL, CLKOUT_DIV);

	return 0;
}
