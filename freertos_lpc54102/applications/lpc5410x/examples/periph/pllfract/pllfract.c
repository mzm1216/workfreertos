/*
 * @brief PLL setup example
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

/** @defgroup PERIPH_PLLFRACT_5410X PLL setup example
 * @ingroup EXAMPLES_PERIPH_5410X
 * @include "periph\pllfract\readme.txt"
 */

/**
 * @}
 */

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* GPIO pin for button input */
#if defined(BOARD_NXP_LPCXPRESSO_54102)
#define GPIO_BUTTON_PORT    0	/* GPIO port number mapped to button */
#define GPIO_BUTTON_PIN     24	/* GPIO pin number mapped to button */

#else
#error "Board not supported - setup GPIO for button"
#endif

/* CLKOUT divider */
#define CLKOUT_DIV      10

/* PLL clock source, must be one of the following:
   SYSCON_PLLCLKSRC_IRC, SYSCON_PLLCLKSRC_CLKIN,
   SYSCON_PLLCLKSRC_WDTOSC, SYSCON_PLLCLKSRC_RTC. */
#define PLLCLOCKSOURCE  SYSCON_PLLCLKSRC_IRC

/* Enable this definition to use more precise PLL outputs */
#define USEPLLPRECISE

#define TICKSPERKHZ     (1000)
#define TICKSPERMHZ     (TICKSPERKHZ * 1000)
#define REFFREQPLL      (13 * TICKSPERMHZ)

/* PLL frequency output test list */
static const uint32_t freqs[] = {
	1500000, 3000000, 5000000, 9000000, 11000000,
	REFFREQPLL, 15000000, 21000000, 25000000, 32000000,
	37000000, 41000000, 45000000, 50000000, 54000000,
	59000000, 63000000, 66000000, 70000000, 75000000,
	78000000, 82000000, 90000000, 96000000, 100000000,
#if 0
	/* Frequencies under this line are beyond spec and
	   are disabled by default in this example. It would
	   be a good idea to pretend you didn't see them at
	   all and not ignore the datasheet. But your
	   adventurous and you are going to try these, yes?
	   If you enable them, you will drive the device
	   beyond the spec limit and things might not work
	   correctly. You might damage your board! Use with
	   care! */
	110000000, 117000000, 124000000, 129000000,
	134000000, 140000000, 147000000, 150000000
#endif
};

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Get a baseline count reading in the frequency measurement hardware. Can
   be used to determine rate of reference clock once a known source and
   reference pair value is known. */
static uint32_t reverseFreqMeasure(void)
{
	uint32_t ticksRefs;

	/* Setup to measure the selected target wirh main clock as reference */
	Chip_INMUX_SetFreqMeasRefClock(FREQ_MEAS_MAIN_CLK);
	Chip_INMUX_SetFreqMeasTargClock(FREQMSR_IRC);

	/* Start a measurement cycle and wait for it to complete. If the target
	   clock is not running, the measurement cycle will remain active
	   forever, so a timeout may be necessary if the target clock can stop */
	Chip_SYSCON_StartFreqMeas();
	while (!Chip_SYSCON_IsFreqMeasComplete()) {}

	/* Get computed frequency */
	ticksRefs = Chip_SYSCON_GetRawFreqMeasCapval();

	return ticksRefs;
}

/* Sets the best FLASH clock arte for the passed frequency */
static void setupFlashClocks(uint32_t freq)
{
	/* v17.0 ROM support only - coarse FLASH clocking timing.
	   FLASH access is setup based on voltage for v17.1 and later ROMs
	   as part of the power library. */
	if (Chip_POWER_GetROMVersion() == LPC5410X_ROMVER_0) {
		if (freq < 20000000) {
			Chip_SYSCON_SetFLASHAccess(SYSCON_FLASH_1CYCLE);
		}
		else if (freq < 48000000) {
			Chip_SYSCON_SetFLASHAccess(SYSCON_FLASH_2CYCLE);
		}
		else if (freq < 72000000) {
			Chip_SYSCON_SetFLASHAccess(SYSCON_FLASH_3CYCLE);
		}
		else {
			Chip_SYSCON_SetFLASHAccess(SYSCON_FLASH_4CYCLE);
		}
	}
}

/* Helper function for safe PLL rate setup */
static void setPllRate(uint32_t rate)
{
	PLL_CONFIG_T pllConfig;
	PLL_SETUP_T pllSetup;
	PLL_ERROR_T pllError;
	volatile int j;

	/* Switch to IRC while adjusting the PLL. Note peripherals
	   using the ASYNC clock use the IRC as the reference clock
	   and are not affected by this switch. */
	Chip_Clock_SetMainClockSource(SYSCON_MAINCLKSRC_IRC);

	/* Power down PLL to change the PLL divider ratio */
	Chip_SYSCON_PowerDown(SYSCON_PDRUNCFG_PD_SYS_PLL);

	/* Select the PLL input */
	Chip_Clock_SetSystemPLLSource(PLLCLOCKSOURCE);
	if ((PLLCLOCKSOURCE == SYSCON_PLLCLKSRC_RTC) || (PLLCLOCKSOURCE == SYSCON_PLLCLKSRC_WDTOSC)) {
		/* When switching clock sources for the PLL, both the current and new source
		   must be enabled and requires a small sync time. */
		for (j = 0; j < 0x10000; j++) {}
	}

	/* Setup PLL configuration */
	pllConfig.desiredRate = rate;
	pllConfig.InputRate = 0;/* Not used unless PLL_CONFIGFLAG_USEINRATE flag is used */
#if defined(USEPLLPRECISE)
	pllConfig.flags = 0;
	pllConfig.ss_mf = SS_MF_64;
	pllConfig.ss_mr = SS_MR_K3;
	pllConfig.ss_mc = SS_MC_RECC;
	pllConfig.mfDither = false;
#else
	/* Force non-fractional mode */
	pllConfig.flags = PLL_CONFIGFLAG_FORCENOFRACT;
#endif
	pllError = Chip_Clock_SetupPLLData(&pllConfig, &pllSetup);
	if (pllError != PLL_ERROR_SUCCESS) {
		DEBUGOUT("PLL config error %d\r\n", pllError);
		while (1) {}
	}

	if ((PLLCLOCKSOURCE == SYSCON_PLLCLKSRC_RTC) || (PLLCLOCKSOURCE == SYSCON_PLLCLKSRC_WDTOSC)) {
		/* Disable PLL wait lock flag when using RTC or WDTOSC, since
		   it will never lock */
		pllSetup.flags = PLL_SETUPFLAG_POWERUP | PLL_SETUPFLAG_ADGVOLT;
	}
	else {
		/* If using WAITLOCK, powerup is implied, but we set the flag here for
		   consistency */
#if defined(USEPLLPRECISE)
		/* Don't wait for PLL lock when using SS, it might not lock */
		pllSetup.flags = PLL_SETUPFLAG_POWERUP | PLL_SETUPFLAG_ADGVOLT;
#else
		pllSetup.flags = PLL_SETUPFLAG_POWERUP | PLL_SETUPFLAG_WAITLOCK | PLL_SETUPFLAG_ADGVOLT;
#endif
	}
	pllError = Chip_Clock_SetupSystemPLLPrec(&pllSetup);
	if (pllError != PLL_ERROR_SUCCESS) {
		DEBUGOUT("PLL setup error %d\r\n", pllError);
		while (1) {}
	}

	/* Setup FLASH access speed */
	setupFlashClocks(rate);

#if defined(USEPLLPRECISE)
	/* Since PLL lock may not happen when using SS mode, force a small
	   delay here to let it stabilize. */
	for (j = 0; j < 0x8000; j++) {}
#else
	if ((PLLCLOCKSOURCE == SYSCON_PLLCLKSRC_RTC) || (PLLCLOCKSOURCE == SYSCON_PLLCLKSRC_WDTOSC)) {
		/* Small delay for PLL lock when using RTC or WDTOSC */
		for (j = 0; j < 0x8000; j++) {}
	}
#endif
	/* Set main clock source to the system PLL  */
	Chip_Clock_SetMainClockSource(SYSCON_MAINCLKSRC_PLLOUT);
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	Application main function
 * @return	Does not return
 */
int main(void)
{
	uint64_t refTicks, upTicks, upRate;
	int i;
	volatile int j;

	/* Board Initialization */
	SystemCoreClockUpdate();
	Board_Init();

	if (PLLCLOCKSOURCE == SYSCON_PLLCLKSRC_RTC) {
		/* Turn on the RTC 32K Oscillator if it needs to be used as a source
		   for the PLL. */
		Chip_SYSCON_PowerUp(SYSCON_PDRUNCFG_PD_32K_OSC);
		Chip_Clock_EnableRTCOsc();
	}
	else if (PLLCLOCKSOURCE == SYSCON_PLLCLKSRC_WDTOSC) {
		/* Enable the power to the WDT Oscillator */
		Chip_SYSCON_PowerUp(SYSCON_PDRUNCFG_PD_WDT_OSC);
	}
	else if (PLLCLOCKSOURCE == SYSCON_PLLCLKSRC_CLKIN) {
		/* Setup CLKIN via IOCON (pin muxing) */
		Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 22,
							 (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_DIGITAL_EN));
		if (Chip_Clock_GetExtClockInRate() == 0) {
			/* Can't continue, stop! */
			DEBUGSTR("CLKIN selected for PLL, but CLKIN rate is 0\r\n");
			while (1) {}
		}
	}

	/* Map P0.21 as CLKOUT pin */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 21,
						 (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_DIGITAL_EN));

	/* Setup CLKOUT for main system clock divided by 10 */
	Chip_Clock_SetCLKOUTSource(SYSCON_CLKOUTSRC_MAINCLK, CLKOUT_DIV);

	/* Button GPIO is an input */
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_BUTTON_PORT, GPIO_BUTTON_PIN);

#if defined(USEPLLPRECISE)
	DEBUGSTR("Using fractional PLL mode...\r\n");
#else
	DEBUGSTR("Using non-fractional PLL mode...\r\n");
#endif

	/* Get reference ticks for IRC as target and reference rate */
	setPllRate(REFFREQPLL);
	refTicks = (uint64_t) reverseFreqMeasure();

	/* Test a setup of different PLL frequencies */
	for (i = 0; i < (sizeof(freqs) / sizeof(freqs[0])); i++) {
		/* Setup PLL for selected rate */
		setPllRate(freqs[i]);

		DEBUGOUT("PLL rate requested    : %u KHz\r\n", (freqs[i] / TICKSPERKHZ));
		DEBUGOUT("PLL input rate        : %u KHz ", (Chip_Clock_GetSystemPLLInClockRate() / TICKSPERKHZ));
		if (PLLCLOCKSOURCE == SYSCON_PLLCLKSRC_RTC) {
			DEBUGSTR("(RTC)\r\n");
		}
		else if (PLLCLOCKSOURCE == SYSCON_PLLCLKSRC_CLKIN) {
			DEBUGSTR("(CLKIN)\r\n");
		}
		else if (PLLCLOCKSOURCE == SYSCON_PLLCLKSRC_WDTOSC) {
			DEBUGSTR("(WDTOSC)\r\n");
		}
		else {
			DEBUGSTR("(IRC)\r\n");
		}
		DEBUGOUT("PLL output (actual)   : %u KHz (reported by driver)\r\n",
				 (Chip_Clock_GetSystemPLLOutClockRate(false) / TICKSPERKHZ));
		DEBUGOUT("CLKOUT rate (est)     : %u KHz\r\n", ((freqs[i] / CLKOUT_DIV) / TICKSPERKHZ));

		/* Only accurate when frequency reference is above base reference */
		upRate = REFFREQPLL;
		if (freqs[i] > upRate) {
			/* Using the reference tick count with main clock at the IRC rate,
			   we can determine the approximate main clock rate */
			upTicks = (uint64_t) reverseFreqMeasure();
			upRate = (upRate * refTicks) / upTicks;

			/* Additional info */
			DEBUGOUT("CLKOUT rate (actual)  : %u KHz\r\n", ((uint32_t) (upRate / CLKOUT_DIV) / TICKSPERKHZ));
			DEBUGOUT("Estimated PLL rate    : %u KHz (computed by frequency measurement hardware)\r\n",
					 ((uint32_t) upRate /  TICKSPERKHZ));
			DEBUGOUT("Refticks              : %u\r\n", (uint32_t) refTicks);
			DEBUGOUT("upTicks               : %u\r\n", (uint32_t) upTicks);
		}
		DEBUGSTR("\r\n");

		/* Wait for button to be pressed to continue */
		while (Chip_GPIO_ReadPortBit(LPC_GPIO, GPIO_BUTTON_PORT, GPIO_BUTTON_PIN) != 0) {}
		for (j = 0; j < 0x30000; j++) {}	/* Simple debounce */
		while (Chip_GPIO_ReadPortBit(LPC_GPIO, GPIO_BUTTON_PORT, GPIO_BUTTON_PIN) == 0) {}
	}

	return 0;
}
