/*
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
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
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "board.h"

#define KEY_BOARD_CLK_HZ	(96000000)
/* The System initialization code is called prior to the application and
   initializes the board for run-time operation. Board initialization
   includes clock setup and default pin muxing configuration. */

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
	/* LCD timing and chip Config */
	STATIC const IP_EMC_STATIC_CONFIG_T SSD1322_config = {
		0,
		EMC_STATIC_CONFIG_MEM_WIDTH_8 |
		EMC_STATIC_CONFIG_CS_POL_ACTIVE_LOW |
		EMC_STATIC_CONFIG_BLS_HIGH /* |
									  EMC_CONFIG_BUFFER_ENABLE*/,
	
		EMC_NANOSECOND(20),
			EMC_NANOSECOND(20),
			EMC_NANOSECOND(20),
			EMC_NANOSECOND(60),
			EMC_NANOSECOND(60),
			
			EMC_NANOSECOND(150)
//			EMC_CLOCK(200)
	
	};

#if defined(CORE_M4)
#if 0
/* EMC clock delay */
#define CLK0_DELAY 7

/* Keil SDRAM timing and chip Config */
STATIC const IP_EMC_DYN_CONFIG_T IS42S32800D_config = {
	EMC_NANOSECOND(64000000 / 4096),
	0x01,				/* Command Delayed */
	3,					/* tRP */
	7,					/* tRAS */
	EMC_NANOSECOND(70),	/* tSREX */
	EMC_CLOCK(0x01),	/* tAPR */
	EMC_CLOCK(0x05),	/* tDAL */
	EMC_NANOSECOND(12),	/* tWR */
	EMC_NANOSECOND(60),	/* tRC */
	EMC_NANOSECOND(60),	/* tRFC */
	EMC_NANOSECOND(70),	/* tXSR */
	EMC_NANOSECOND(12),	/* tRRD */
	EMC_CLOCK(0x02),	/* tMRD */
	{
		{
			EMC_ADDRESS_DYCS0,	/* EA Board uses DYCS0 for SDRAM */
			2,	/* RAS */

			EMC_DYN_MODE_WBMODE_PROGRAMMED |
			EMC_DYN_MODE_OPMODE_STANDARD |
			EMC_DYN_MODE_CAS_3 |
			EMC_DYN_MODE_BURST_TYPE_SEQUENTIAL |
			EMC_DYN_MODE_BURST_LEN_4,

			EMC_DYN_CONFIG_DATA_BUS_32 |
			EMC_DYN_CONFIG_LPSDRAM |
			EMC_DYN_CONFIG_16Mx16_4BANKS_13ROWS_9COLS |
			EMC_DYN_CONFIG_MD_SDRAM
		},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	}
};

/* NorFlash timing and chip Config */
STATIC const IP_EMC_STATIC_CONFIG_T SST39VF320_config = {
	0,
	EMC_STATIC_CONFIG_MEM_WIDTH_16 |
	EMC_STATIC_CONFIG_CS_POL_ACTIVE_LOW |
	EMC_STATIC_CONFIG_BLS_HIGH /* |
							      EMC_CONFIG_BUFFER_ENABLE*/,

	EMC_NANOSECOND(0),
		EMC_NANOSECOND(30),
		EMC_NANOSECOND(70),
		EMC_NANOSECOND(70),
		EMC_NANOSECOND(40),
		EMC_CLOCK(4)

};
/* LCD timing and chip Config */
STATIC const IP_EMC_STATIC_CONFIG_T TFI1P6089_config = {
	1,
	EMC_STATIC_CONFIG_MEM_WIDTH_16 |
	EMC_STATIC_CONFIG_CS_POL_ACTIVE_LOW |
	EMC_STATIC_CONFIG_BLS_HIGH /* |
							      EMC_CONFIG_BUFFER_ENABLE*/,

	EMC_NANOSECOND(0),
		EMC_NANOSECOND(0),
		EMC_NANOSECOND(0),
		EMC_NANOSECOND(0),
		EMC_NANOSECOND(0),
		EMC_CLOCK(1)

};

/* NandFlash timing and chip Config */
STATIC const IP_EMC_STATIC_CONFIG_T K9F1G_config = {
	2,
	EMC_STATIC_CONFIG_MEM_WIDTH_8 |
	EMC_STATIC_CONFIG_CS_POL_ACTIVE_LOW |
	EMC_STATIC_CONFIG_BLS_HIGH /* |
							      EMC_CONFIG_BUFFER_ENABLE*/,

	EMC_NANOSECOND(0),
	EMC_NANOSECOND(35),
	EMC_NANOSECOND(70),
	EMC_NANOSECOND(70),
	EMC_NANOSECOND(40),
	EMC_CLOCK(4)
};
#endif
/* Structure for initial base clock states */
struct CLK_BASE_STATES {
	CHIP_CGU_BASE_CLK_T clk;	/* Base clock */
	CHIP_CGU_CLKIN_T clkin;	/* Base clock source, see UM for allowable souorces per base clock */
	bool autoblock_enab;/* Set to true to enable autoblocking on frequency change */
	bool powerdn;		/* Set to true if the base clock is initially powered down */
};

/* Initial base clock states are mostly on */
STATIC const struct CLK_BASE_STATES InitClkStates[] = {
	{CLK_BASE_PHY_TX, CLKIN_ENET_TX, true, false},
#if defined(USE_RMII)
	{CLK_BASE_PHY_RX, CLKIN_ENET_TX, true, false},
#else
	{CLK_BASE_PHY_RX, CLKIN_ENET_RX, true, false},
#endif

	/* Clocks derived from dividers */
	{CLK_BASE_LCD, CLKIN_IDIVC, true, false},
	{CLK_BASE_USB1, CLKIN_IDIVD, true, true}
};

/* SPIFI high speed pin mode setup */
STATIC const PINMUX_GRP_T spifipinmuxing[] = {
	{0x3, 3,  (SCU_PINIO_FAST | SCU_MODE_FUNC3)},	/* SPIFI CLK */
	{0x3, 4,  (SCU_PINIO_FAST | SCU_MODE_FUNC3)},	/* SPIFI D3 */
	{0x3, 5,  (SCU_PINIO_FAST | SCU_MODE_FUNC3)},	/* SPIFI D2 */
	{0x3, 6,  (SCU_PINIO_FAST | SCU_MODE_FUNC3)},	/* SPIFI D1 */
	{0x3, 7,  (SCU_PINIO_FAST | SCU_MODE_FUNC3)},	/* SPIFI D0 */
	{0x3, 8,  (SCU_PINIO_FAST | SCU_MODE_FUNC3)}	/* SPIFI CS/SSEL */
};

/*便携式 显示板，OLED_REST管脚符号是--p1_0*/

STATIC const PINMUX_GRP_T pinmuxing[] = {
	
#if 1
	/*pinGroup	,Pinnum	,modefunc*/

	#ifndef USED_SSD1322

	//	{0x2, 7, (SCU_MODE_INACT | SCU_MODE_FUNC0)}, //p2_7
//	{0x1, 1, (SCU_MODE_INACT | SCU_MODE_FUNC0)}, //p1_1
//	{0x1, 2, (SCU_MODE_INACT | SCU_MODE_FUNC0)}, //p1_2
	
	{0x6, 1,  (SCU_MODE_INACT | SCU_MODE_FUNC0)}, //key_up
	{0x6, 2,  (SCU_MODE_INACT | SCU_MODE_FUNC0)}, //key_ack
	{0x1, 20, (SCU_MODE_INACT | SCU_MODE_FUNC0)}, //key_down
	{0x1, 18, (SCU_MODE_INACT | SCU_MODE_FUNC0)}, //key_left
	{0x1, 17, (SCU_MODE_INACT | SCU_MODE_FUNC0)}, //key_right
	{0x6, 5, (SCU_MODE_INACT | SCU_MODE_FUNC0)}, //key_enter
	{0x6, 4, (SCU_MODE_INACT | SCU_MODE_FUNC0)}, //key_cancel

	
	{0x1, 3,  (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //p1_3		nOE
	{0x1, 5,  (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //nCE
	{0x1, 6,  (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //nWE
	{0x1, 7,  (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //D0
	{0x1, 8,  (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //D1
	{0x1, 9,  (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //D2
	{0x1, 10, (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //D3	
	{0x1, 11, (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //D4
	{0x1, 12, (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //D5
	{0x1, 13, (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //D6
	{0x1, 14, (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //D7
	{0x2, 12, (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //A3
//	{0x2, 9, (SCU_MODE_INACT | SCU_MODE_FUNC0)}, //p2_9

	{0x2, 1, (SCU_MODE_INACT | SCU_MODE_FUNC4)}, //LED_RUN
#ifdef USED_SSD1322_BX
	/*便携式的 液晶reset管教为 P1_0(P0.4)*/
	{0x1, 0,  (SCU_MODE_INACT | SCU_MODE_FUNC0)}, //OLED_RES
#else
	/*TFT-SSD2119液晶reset管教为 P2_2(P5。2)*/
	{0x2, 2, (SCU_MODE_INACT | SCU_MODE_FUNC4)}, //	OLED_RES	
#endif
	{0x2, 3, (SCU_MODE_PULLDOWN | SCU_MODE_FUNC2)}, //U3_TX
	{0x2, 4, (SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC2)}, // U3_RX
	{0x2, 5, (SCU_MODE_INACT | SCU_MODE_FUNC4)}, //OLED_12ON
	{0x2, 6, (SCU_MODE_INACT | SCU_MODE_FUNC4)}, //OLED_28ON
	
//	{0x2, 8, (SCU_MODE_INACT | SCU_MODE_FUNC4)}, //P2_8
	
	{0x3, 1, (SCU_MODE_INACT | SCU_MODE_FUNC4)}, //LED_TX
	{0x3, 2, (SCU_MODE_INACT | SCU_MODE_FUNC4)}, //LED_RX
	#else
	
//	{0x2, 7, (SCU_MODE_INACT | SCU_MODE_FUNC0)}, //p2_7
//	{0x1, 1, (SCU_MODE_INACT | SCU_MODE_FUNC0)}, //p1_1
//	{0x1, 2, (SCU_MODE_INACT | SCU_MODE_FUNC0)}, //p1_2
	
	{0x0, 0,  (SCU_MODE_INACT | SCU_MODE_FUNC0)}, //key_up
	{0x0, 1,  (SCU_MODE_INACT | SCU_MODE_FUNC0)}, //key_ack
	{0x1, 15, (SCU_MODE_INACT | SCU_MODE_FUNC0)}, //key_down
	{0x1, 16, (SCU_MODE_INACT | SCU_MODE_FUNC0)}, //key_left
	{0x1, 17, (SCU_MODE_INACT | SCU_MODE_FUNC0)}, //key_right
	{0x1, 18, (SCU_MODE_INACT | SCU_MODE_FUNC0)}, //key_enter
	{0x1, 20, (SCU_MODE_INACT | SCU_MODE_FUNC0)}, //key_cancel

	
	{0x1, 3,  (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //p1_3		nOE
	{0x1, 5,  (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //nCE
	{0x1, 6,  (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //nWE
	{0x1, 7,  (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //D0
	{0x1, 8,  (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //D1
	{0x1, 9,  (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //D2
	{0x1, 10, (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //D3	
	{0x1, 11, (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //D4
	{0x1, 12, (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //D5
	{0x1, 13, (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //D6
	{0x1, 14, (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //D7
	{0x2, 12, (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //A3
//	{0x2, 9, (SCU_MODE_INACT | SCU_MODE_FUNC0)}, //p2_9

	{0x2, 1, (SCU_MODE_INACT | SCU_MODE_FUNC4)}, //LED_RUN
	{0x2, 2, (SCU_MODE_INACT | SCU_MODE_FUNC4)}, //	OLED_RES	
	{0x2, 3, (SCU_MODE_PULLDOWN | SCU_MODE_FUNC2)}, //U3_TX
	{0x2, 4, (SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC2)}, // U3_RX
	{0x2, 5, (SCU_MODE_INACT | SCU_MODE_FUNC4)}, //OLED_12ON
	{0x2, 6, (SCU_MODE_INACT | SCU_MODE_FUNC4)}, //OLED_28ON
	
//	{0x2, 8, (SCU_MODE_INACT | SCU_MODE_FUNC4)}, //P2_8
	
	{0x3, 1, (SCU_MODE_INACT | SCU_MODE_FUNC4)}, //LED_TX
	{0x3, 2, (SCU_MODE_INACT | SCU_MODE_FUNC4)}, //LED_RX
	
	#endif
#else
		
		{0x0, 0, (SCU_MODE_INACT | SCU_MODE_FUNC0)}, //key_up
		{0x0, 1, (SCU_MODE_INACT | SCU_MODE_FUNC0)}, //key_ack
		{0x0, 2, (SCU_MODE_INACT | SCU_MODE_FUNC0)}, //key_down
		{0x0, 3, (SCU_MODE_INACT | SCU_MODE_FUNC0)}, //key_left
		
		{0x0, 7, (SCU_MODE_INACT | SCU_MODE_FUNC0)}, //p2_7
		{0x0, 8, (SCU_MODE_INACT | SCU_MODE_FUNC0)}, //p1_1
		{0x0, 9, (SCU_MODE_INACT | SCU_MODE_FUNC0)}, //p1_2
		
		{0x0, 10, (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //p1_3		nOE
		
		{0x0, 12, (SCU_MODE_INACT | SCU_MODE_FUNC0)}, //key_right
		{0x0, 13, (SCU_MODE_INACT | SCU_MODE_FUNC0)}, //key_enter
		{0x0, 15, (SCU_MODE_INACT | SCU_MODE_FUNC0)}, //key_cancel
		
		
		{0x1, 0, (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //D0
		{0x1, 1, (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //D1
		{0x1, 2, (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //D2
		{0x1, 3, (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //D3	
		{0x1, 4, (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //D4
		{0x1, 5, (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //D5
		{0x1, 6, (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //D6
		{0x1, 7, (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //D7
		{0x1, 8, (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //nCE
		{0x1, 9, (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //nWE
		
		{0x1, 10, (SCU_MODE_INACT | SCU_MODE_FUNC0)}, //p2_9
		
		{0x1, 12, (SCU_MODE_INACT | SCU_MODE_FUNC3)}, //A3
		
		{0x5, 1, (SCU_MODE_INACT | SCU_MODE_FUNC4)}, //LED_RUN
		{0x5, 2, (SCU_MODE_INACT | SCU_MODE_FUNC4)}, // OLED_RES
		
		{0x5, 3, (SCU_MODE_INACT | SCU_MODE_FUNC2)}, //U3_TX
		{0x5, 4, (SCU_MODE_INACT | SCU_MODE_FUNC2)}, // U3_RX
		
		{0x5, 5, (SCU_MODE_INACT | SCU_MODE_FUNC4)}, //OLED_12ON
		{0x5, 6, (SCU_MODE_INACT | SCU_MODE_FUNC4)}, //OLED_28ON
		
		{0x5, 7, (SCU_MODE_INACT | SCU_MODE_FUNC4)}, //P2_8
		
		{0x5, 8, (SCU_MODE_INACT | SCU_MODE_FUNC4)}, //LED_TX
		{0x5, 9, (SCU_MODE_INACT | SCU_MODE_FUNC4)}, //LED_RX




		{0x1, 3,  (SCU_MODE_FUNC0)},//DC
		{0x1, 4,  (SCU_MODE_FUNC5| SCU_MODE_PULLUP)}, //MOSI spi1
		{0x1, 20, (SCU_MODE_FUNC2| SCU_MODE_INACT)},//SEL
		{0x2, 11, (SCU_MODE_FUNC0)},//RST
		{0xf, 4,  (SCU_MODE_FUNC0| SCU_MODE_INACT)},//SCK
		
		/* Board LEDs */
		{0x1, 1, (SCU_MODE_INBUFF_EN | SCU_MODE_PULLDOWN | SCU_MODE_FUNC0)},
#endif
};
#endif /* defined(CORE_M4) */

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Sets up system pin muxing */
void Board_SetupMuxing(void)
{
#if defined(CORE_M4)
	/* Setup system level pin muxing */
	Chip_SCU_SetPinMuxing(pinmuxing, sizeof(pinmuxing) / sizeof(PINMUX_GRP_T));

	/* SPIFI pin setup is done prior to setting up system clocking */
	Chip_SCU_SetPinMuxing(spifipinmuxing, sizeof(spifipinmuxing) / sizeof(PINMUX_GRP_T));
#endif /* defined(CORE_M4) */
}

/* Set up and initialize clocking prior to call to main */
void Board_SetupClocking(void)
{
#if defined(CORE_M4)
	 int i;

	//Chip_SetupCoreClock(CLKIN_CRYSTAL, MAX_CLOCK_FREQ, true);
	//Chip_SetupCoreClock(CLKIN_CRYSTAL, KEY_BOARD_CLK_HZ, true);
	Chip_SetupCoreClock(CLKIN_CRYSTAL, KEY_BOARD_CLK_HZ, true);

	/* Setup system base clocks and initial states. This won't enable and
	   disable individual clocks, but sets up the base clock sources for
	   each individual peripheral clock. */
//	for (i = 0; i < (sizeof(InitClkStates) / sizeof(InitClkStates[0])); i++) {
//		Chip_Clock_SetBaseClock(InitClkStates[i].clk, InitClkStates[i].clkin,
//								InitClkStates[i].autoblock_enab, InitClkStates[i].powerdn);
//	}
	Chip_Clock_SetBaseClock(CLK_BASE_MX, CLKIN_MAINPLL, true, false);

	/* Reset and enable 32Khz oscillator */
	LPC_CREG->CREG0 &= ~((1 << 3) | (1 << 2));
	LPC_CREG->CREG0 |= (1 << 1) | (1 << 0);

	/* Setup a divider E for main PLL clock switch SPIFI clock to that divider.
	   Divide rate is based on CPU speed and speed of SPI FLASH part. */
#if (KEY_BOARD_CLK_HZ > 180000000)
	Chip_Clock_SetDivider(CLK_IDIV_E, CLKIN_MAINPLL, 5);
#else
	Chip_Clock_SetDivider(CLK_IDIV_E, CLKIN_MAINPLL, 4);
#endif
	Chip_Clock_SetBaseClock(CLK_BASE_SPIFI, CLKIN_IDIVE, true, false);

	/* Attach main PLL clock to divider C with a divider of 2 */
	Chip_Clock_SetDivider(CLK_IDIV_C, CLKIN_MAINPLL, 2);
#endif /* defined(CORE_M4) */
}
#if 1
/* Setup external memories */
void Board_SetupExtMemory(void)
{
	/* Setup EMC Delays */
	/* Move all clock delays together */
//	LPC_CREG->EMCDLYCTL = (CLK0_DELAY) | (CLK0_DELAY << 8) | (CLK0_DELAY << 16 | (CLK0_DELAY << 24));
	LPC_SCU->EMCDELAYCLK = 0x4444;// 2ns delay

	/* Setup EMC Clock Divider for divide by 2 */
	/* Setup EMC clock for a divider of 2 from CPU clock. Enable EMC clock for
	   external memory setup of DRAM. *///
//	   //Chip_Clock_SetEMCClockDiv(SYSCTL_EMC_DIV2);
//	Chip_SYSCTL_PeriphReset(SYSCTL_RESET_EMC);

		LPC_CREG->CREG6 |= 0x1 <<16;	// CREG6 de EMC_CLK_SEL  EMC_CLK  二分频
		LPC_CCU1->CLKCCU[CLK_MX_EMC_DIV].CFG |= ((0x1<<0)|(0x1<<5));	// 使能emc clk，并二分频
			

	/* Init EMC Controller -Enable-LE mode- clock ratio 1:1 */
	Chip_EMC_Init(1, 0, 0);
	
	/* Init EMC Static Controller CS0 */
	Chip_EMC_Static_Init((IP_EMC_STATIC_CONFIG_T *) &SSD1322_config);


	/* EMC Shift Control */
	
}

#endif
/* Set up and initialize hardware prior to call to main */
void Board_SystemInit(void)
{
#if defined(CORE_M4)
	/* Setup system clocking and memory. This is done early to allow the
	   application and tools to clear memory and use scatter loading to
	   external memory. */
	Board_SetupMuxing();
	Board_SetupClocking();
	Board_SetupExtMemory();
#endif /* defined(CORE_M4) */
}
