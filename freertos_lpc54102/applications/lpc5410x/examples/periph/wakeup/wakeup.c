/*
 * @brief Wakeup example
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

/** @defgroup PERIPH_WAKEUP_5410X Wakeup example
 * @ingroup EXAMPLES_PERIPH_5410X
 * @include "periph\wakeup\readme.txt"
 */

/**
 * @}
 */

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* GPIO pin for PININT input */
#if defined(BOARD_NXP_LPCXPRESSO_54102)
#define GPIO_PININT_PIN     24	/* GPIO pin number mapped to PININT */
#define GPIO_PININT_PORT    0	/* GPIO port number mapped to PININT */
#define GPIO_PININT_INDEX   PININTSELECT0	/* PININT index used for GPIO mapping */
#define PININT_IRQ_HANDLER  PIN_INT0_IRQHandler	/* GPIO interrupt IRQ function name */
#define PININT_NVIC_NAME    PIN_INT0_IRQn	/* GPIO interrupt NVIC interrupt name */

#else
#error "Board not supported - setup GPIO for pin interrupt"
#endif

/* Select a mode of type 'POWER_MODE_T' for this example: mode available are
   POWER_SLEEP, POWER_DEEP_SLEEP,
   POWER_POWER_DOWN, POWER_DEEP_POWER_DOWN */
#define PDOWNMODE   (POWER_POWER_DOWN)

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Turns on the LED and stalls for a bit */
static void ledStallOn(void)
{
	volatile int i;

	/* Turn on LED and delay for a very short time so it can be seen
	   as on. */
	Board_LED_Set(0, true);
	for (i = 0; i < 0x40000; i++) {}
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Handle interrupt from RItimer */
void RIT_IRQHandler(void)
{
	Chip_RIT_ClearInt(LPC_RITIMER);
}

/**
 * @brief	Handle interrupt from GPIO pin or GPIO pin mapped to PININT
 * @return	Nothing
 */
void PININT_IRQ_HANDLER(void)
{
	Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH(GPIO_PININT_INDEX));
}

/**
 * @brief	Main routine for wakeup example
 * @return	Function should not exit
 */
int main(void)
{
	CHIP_SYSCON_MAINCLKSRC_T saved_clksrc;

	/* Generic Initialization */
	SystemCoreClockUpdate();
	Board_Init();
	ledStallOn();

	/* Show ROM version */
	DEBUGOUT("ROM version = %04x\r\n", Chip_POWER_GetROMVersion());

	/* Wakeup from deep powerdown? Then turn on LED1 */
	if ((Chip_PMU_GetDPDWUSource() & PMU_DPDWU_RESET) != 0) {
		Board_LED_Set(1, true);
		Chip_PMU_ClearDPDWUSource(Chip_PMU_GetDPDWUSource());
	}

	/* Switch main system clock to IRC and power down PLL */
	saved_clksrc = Chip_Clock_GetMainClockSource();
	if (saved_clksrc == SYSCON_MAINCLKSRC_PLLOUT) {
		Chip_Clock_SetMainClockSource(SYSCON_MAINCLKSRC_IRC);
		Chip_SYSCON_PowerDown(SYSCON_PDRUNCFG_PD_SYS_PLL);
	}

	/**** Setup PININT ****/
	/* Wakeup pin is setup as a GPIO input */
	Chip_IOCON_PinMuxSet(LPC_IOCON, GPIO_PININT_PORT, GPIO_PININT_PIN,
						 (IOCON_FUNC0 | IOCON_DIGITAL_EN | IOCON_GPIO_MODE));
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_PININT_PORT, GPIO_PININT_PIN);

	/* Wait for pin to not be pressed */
	while (Chip_GPIO_ReadPortBit(LPC_GPIO, GPIO_PININT_PORT, GPIO_PININT_PIN) == 0) {}

	/* Configure pin interrupt selection for the GPIO pin in Input Mux Block */
	Chip_PININT_Init(LPC_PININT);
	Chip_INMUX_PinIntSel(GPIO_PININT_INDEX, GPIO_PININT_PORT, GPIO_PININT_PIN);

	/* Configure channel interrupt as edge sensitive and falling edge interrupt */
	Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH(GPIO_PININT_INDEX));
	Chip_PININT_SetPinModeEdge(LPC_PININT, PININTCH(GPIO_PININT_INDEX));
	Chip_PININT_EnableIntLow(LPC_PININT, PININTCH(GPIO_PININT_INDEX));

	/**** Setup RITimer ****/
	/* Initialize RI Timer */
	Chip_RIT_Init(LPC_RITIMER);

	/* Setup wakeup period to 5s */
	Chip_RIT_SetTimerInterval(LPC_RITIMER, (5 * 1000));

	/* Enable RI Timer and clear on compare match or ....
	   use Chip_RIT_EnableCTRL(LPC_RITIMER, RIT_CTRL_ENCLR | RIT_CTRL_TEN);
	   to save space. */
	Chip_RIT_Enable(LPC_RITIMER);
	Chip_RIT_CompClearEnable(LPC_RITIMER);

	/* Enable RI Timer and PININT interrupts */
	NVIC_EnableIRQ(RIT_IRQn);
	NVIC_EnableIRQ(PININT_NVIC_NAME);

	/* Enable wakeup for RIT and PININT0 */
	Chip_SYSCON_EnableWakeup(SYSCON_STARTER_RIT);
	Chip_SYSCON_EnableWakeup(SYSCON_STARTER_PINT0);

	/* Wait for wakeup event */
	while (1) {
		/* Turn on LED and stall */
		ledStallOn();

		/* Disable PLL, if previously enabled, prior to sleep */
		if (saved_clksrc == SYSCON_MAINCLKSRC_PLLOUT) {
			Chip_Clock_SetMainClockSource(SYSCON_MAINCLKSRC_IRC);
			Chip_SYSCON_PowerDown(SYSCON_PDRUNCFG_PD_SYS_PLL);
		}

		/* Turn off LED before going to sleep */
		Board_LED_Set(0, false);

		/* Lower system voltages to current lock (likely IRC) */
		Chip_POWER_SetVoltage(POWER_LOW_POWER_MODE, Chip_Clock_GetMainClockRate());

		/* Go to sleep leaving SRAM powered during sleep. Use lower
		     voltage during sleep. */
		Chip_POWER_EnterPowerMode(PDOWNMODE,
								  (SYSCON_PDRUNCFG_PD_SRAM0A | SYSCON_PDRUNCFG_PD_SRAM0B));

		/* Turn on LED and stall */
		ledStallOn();

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
