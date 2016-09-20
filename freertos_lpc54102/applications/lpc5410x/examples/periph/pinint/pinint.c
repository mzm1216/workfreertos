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

#include "board.h"

/** @defgroup PERIPH_PININT_5410X Pin Interrupt example
 * @ingroup EXAMPLES_PERIPH_5410X
 * @include "periph\pinint\readme.txt"
 */

/**
 * @}
 */

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

#if defined(BOARD_NXP_LPCXPRESSO_54102)
/* GPIO pin for PININT interrupt */
#define GPIO_PININT_PIN     24	/* GPIO pin number mapped to PININT */
#define GPIO_PININT_PORT    0	/* GPIO port number mapped to PININT */
#define GPIO_PININT_INDEX   PININTSELECT0	/* PININT index used for GPIO mapping */
#define PININT_IRQ_HANDLER  PIN_INT0_IRQHandler	/* GPIO interrupt IRQ function name */
#define PININT_NVIC_NAME    PIN_INT0_IRQn	/* GPIO interrupt NVIC interrupt name */

#else
#error "PININT not configured for this example"
#endif /* defined (BOARD_NXP_LPCXPRESSO_54102) */

/* Select a mode of type 'POWER_MODE_T' for this example: mode available are
   POWER_SLEEP, POWER_DEEP_SLEEP, POWER_POWER_DOWN*/
#define PDOWNMODE   (POWER_POWER_DOWN)

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
/* Saved clock source */
static  CHIP_SYSCON_MAINCLKSRC_T saved_clksrc;

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	Handle interrupt from GPIO pin or GPIO pin mapped to PININT
 * @return	Nothing
 */
void PININT_IRQ_HANDLER(void)
{
	Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH(GPIO_PININT_INDEX));
	Board_LED_Toggle(0);
}

/**
 * @brief	Main program body
 * @return	Does not return
 */
int main(void)
{
	/* Generic Initialization */
	SystemCoreClockUpdate();

	/* Board_Init calls Chip_GPIO_Init and enables GPIO clock if needed,
	   Chip_GPIO_Init is not called again */
	Board_Init();
//	Board_LED_Set(0, false);

	Chip_PININT_Init(LPC_PININT);

	/* Configure GPIO pin as input */
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_PININT_PORT, GPIO_PININT_PIN);

	/* Configure pin as GPIO */
	Chip_IOCON_PinMuxSet(LPC_IOCON, GPIO_PININT_PORT, GPIO_PININT_PIN,
						 (IOCON_FUNC0 | IOCON_DIGITAL_EN  | IOCON_GPIO_MODE));

	/* Configure pin interrupt selection for the GPIO pin in Input Mux Block */
	Chip_INMUX_PinIntSel(GPIO_PININT_INDEX, GPIO_PININT_PORT, GPIO_PININT_PIN);

	/* Configure channel interrupt as edge sensitive and falling edge interrupt */
	Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH(GPIO_PININT_INDEX));
	Chip_PININT_SetPinModeEdge(LPC_PININT, PININTCH(GPIO_PININT_INDEX));
	Chip_PININT_EnableIntLow(LPC_PININT, PININTCH(GPIO_PININT_INDEX));

	/* Enable interrupt in the NVIC */
	NVIC_EnableIRQ(PININT_NVIC_NAME);

	/* Enable wakeup for PININT0 */
	Chip_SYSCON_EnableWakeup(SYSCON_STARTER_PINT0);

	/* save the clock source, power down the PLL */
	saved_clksrc = Chip_Clock_GetMainClockSource();

	/* Go to sleep mode - LED will toggle on each wakeup event */
	while (1) {
		/* Go to sleep state - will wake up automatically on interrupt */
		/* Disable PLL, if previously enabled, prior to sleep */
		if (saved_clksrc == SYSCON_MAINCLKSRC_PLLOUT) {
			Chip_Clock_SetMainClockSource(SYSCON_MAINCLKSRC_IRC);
			Chip_SYSCON_PowerDown(SYSCON_PDRUNCFG_PD_SYS_PLL);
		}

		/* Lower system voltages to current lock (likely IRC) */
		Chip_POWER_SetVoltage(POWER_LOW_POWER_MODE, Chip_Clock_GetMainClockRate());

		/* Go to sleep leaving SRAM powered during sleep. Use lower
		    voltage during sleep. */
		Chip_POWER_EnterPowerMode(PDOWNMODE,
								  (SYSCON_PDRUNCFG_PD_SRAM0A | SYSCON_PDRUNCFG_PD_SRAM0B));

		/* On wakeup, restore PLL power if needed */
		if (saved_clksrc == SYSCON_MAINCLKSRC_PLLOUT) {
			Chip_SYSCON_PowerUp(SYSCON_PDRUNCFG_PD_SYS_PLL);

			/* Wait for PLL lock */
			while (!Chip_Clock_IsSystemPLLLocked()) {}

			Chip_POWER_SetVoltage(POWER_LOW_POWER_MODE, Chip_Clock_GetSystemPLLOutClockRate(false));

			/* Use PLL for system clock */
			Chip_Clock_SetMainClockSource(SYSCON_MAINCLKSRC_PLLOUT);
		}
	}

	return 0;
}
