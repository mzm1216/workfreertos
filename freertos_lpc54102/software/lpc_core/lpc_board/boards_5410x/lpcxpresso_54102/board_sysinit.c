/*
 * @brief NXP LPCXpresso 54102 Sysinit file
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

/* The System initialization code is called prior to the application and
   initializes the board for run-time operation. Board initialization
   for the NXP LPC54102 board includes default pin muxing and clock setup
   configuration. */

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
   
/*****************************************************************************
 * 结 构 名  : pinmuxing
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月22日
 * 函数功能  : GPIO初始化
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : STATIC
 * 调用关系  : Board_SetupMuxing调用
 * 其    它  : 

*****************************************************************************/
#if 1
STATIC const PINMUX_GRP_T pinmuxing[] = {
	/* FAN_FG1-2	*/
	{0, 0,  (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)},	/* CT32B0_CAP0	FAN_FG1*/
	{0, 1,  (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)},	/* CT32B0_CAP1	FAN_FG2*/
	
	/* 3线SPI，IO口模拟实现	*/
	{0, 2,	(IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)},	/* PIO0_2	PLL_RX_SCK*/
	{0, 3,	(IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)},	/* PIO0_3	PLL_RX_DATA*/
	{0, 4,	(IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)},	/* PIO0_4	PLL_RX_SEL*/
	{0, 5,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)},	/* PIO0_5	PLL_RX_SEL2*/
	{0, 6,	(IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)},	/* PIO0_6	PLL_REF_SEL*/

	/*风扇控制*/
	{0, 7,	(IOCON_FUNC2 | IOCON_MODE_INACT | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF)},	/*  SCT0_OUT0	FAN_EN1*/
	{0, 8,	(IOCON_FUNC2 | IOCON_MODE_INACT | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF)},	/*	SCT0_OUT1	FAN_EN2	*/
	{0, 9,	(IOCON_FUNC2 | IOCON_MODE_INACT | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF)},	/*	SCT0_OUT2	FAN_EN3	*/
	{0, 10, (IOCON_FUNC2 | IOCON_MODE_INACT | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF)},	/*	SCT0_OUT3	FAN_PWM	*/

	/*SPI0---发射机72310*/
	/* 0.11 = SPI0_SCK, 0.12 = SPI0_MOSI, 0.13 = SPI0_MISO, 0.14 = SPI0_SSELN0 */
	{0, 11, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)},	/* SPI0_SCK		PLL_TX_SCK*/
	{0, 12, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)},/*(SPI0 MOSI) 	PLL_TX_DATA*/
//	{0, 13, (IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN)},/* NC */
	{0, 14, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)},/* SPI0_SSEL0 	PLL_TX_SEL*/	
//	{0, 15, (IOCON_FUNC1 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN)},/* NC*/

	/*调试管脚*/
//	{0, 16, (IOCON_FUNC5 | IOCON_MODE_PULLDOWN | IOCON_DIGITAL_EN)},/* SWCLK */
//	{0, 17, (IOCON_FUNC5 | IOCON_MODE_PULLDOWN | IOCON_DIGITAL_EN)},/* SWDIO */
	
//	{0, 18, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)},	/* NC*/
//	{0, 19, (IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN)},	/* NC*/
//	{0, 20, (IOCON_FUNC0 | IOCON_MODE_PULLDOWN | IOCON_DIGITAL_EN)},/* NC*/

	/*串口*/
//	{0, 21, (IOCON_FUNC2 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)},	/* U0_TXD*/
//	{0, 22, (IOCON_FUNC2 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)},	/* U0_RXD */
	
	/* I2C0-I2C1 */
	{0, 23, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN | IOCON_STDI2C_EN)}, /* I2C0_SCL (SCL) */
	{0, 24, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN | IOCON_STDI2C_EN)}, /* I2C0_SDA-WAKEUP (SDA) */
	{0, 25, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN | IOCON_STDI2C_EN)},	/* I2C1_SCL (SCL) */
	{0, 26, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN | IOCON_STDI2C_EN)},	/* I2C1_SDA-WAKEUP (SDA) */

	/* GPIO27-28 */
	{0, 27, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN )}, /* PIO0_27	FGU_INT	*/
	{0, 28, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN | IOCON_STDI2C_EN)}, /* PIO0_28	PS_TX	*/

//	{0, 29, (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_DIGITAL_EN )}, /* NC */
//	{0, 30, (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_DIGITAL_EN )}, /* NC */
//	{0, 31, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN )}, /* PIO0_31	ISP_EN */


	/* ADC0通道3-8*/
	{1, 0,  (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_ANALOG_EN)},	/* ADC3 */
	{1, 1,  (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_ANALOG_EN)},	/* ADC4 */
	{1, 2,  (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_ANALOG_EN)},	/* ADC5 */
	{1, 3,  (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_ANALOG_EN)},	/* ADC6 */
	{1, 4,  (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_ANALOG_EN)},	/* ADC7 */
	{1, 5,  (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_ANALOG_EN)},	/* ADC8 */
	

//	{1, 6,  (IOCON_FUNC0 | IOCON_MODE_INACT)},	/* NC */
//	{1, 7,  (IOCON_FUNC0 | IOCON_MODE_INACT)},	/* NC */
//	{1, 8,  (IOCON_FUNC0 | IOCON_MODE_INACT)},	/* NC */

	/*CT32B0_CAP2*/
	{1, 9,  (IOCON_FUNC3 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)},	/* CT32B0_CAP2 	FAN_FG3*/
	
//	{1, 10, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)},	/* NC */
//	{1, 11, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)},	/* NC */

	/*SPI1---spi转4路DAC*/
	{1, 12, (IOCON_FUNC4 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN)},	/* SPI1_SCK 	DAC_SCK	*/
	{1, 13, (IOCON_FUNC4 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN)},	/* SPI1_MOSI	DAC_DATA*/
//	{1, 14, (IOCON_FUNC4 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN)},/* SPI1_MISO 	NC	*/
	{1, 15, (IOCON_FUNC4 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN)},/* SPI1_SSEL0 	DAC_SLE*/

	/*两路GPIO*/
	{1, 16, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)},	/* PIO1_16	ATT_ENA*/
	{1, 17, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)},	/* PIO1_17	NB/WB	*/
};

#else
STATIC const PINMUX_GRP_T pinmuxing[] = {
	/* FAN_FG1-2	*/
	{0, 0,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)},	/* CT32B0_CAP0	FAN_FG1*/
	{0, 1,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)},	/* CT32B0_CAP1	FAN_FG2*/
	
	/* 3线SPI，IO口模拟实现	*/
	{0, 2,	(IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)},	/* PIO0_2	PLL_RX_SCK*/
	{0, 3,	(IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)},	/* PIO0_3	PLL_RX_DATA*/
	{0, 4,	(IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)},	/* PIO0_4	PLL_RX_SEL*/
	{0, 5,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)},	/* PIO0_5	PLL_RX_SEL2*/
	{0, 6,	(IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)},	/* PIO0_6	PLL_REF_SEL*/

	/*风扇控制*/
	{0, 7,	(IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF)},	/*  SCT0_OUT0	FAN_EN1*/
	{0, 8,	(IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF)},	/*	SCT0_OUT1	FAN_EN2	*/
	{0, 9,	(IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF)},	/*	SCT0_OUT2	FAN_EN3	*/
	{0, 10, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF)},	/*	SCT0_OUT3	FAN_PWM	*/

	/*SPI0---发射机72310*/
	/* 0.11 = SPI0_SCK, 0.12 = SPI0_MOSI, 0.13 = SPI0_MISO, 0.14 = SPI0_SSELN0 */
	{0, 11, (IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN)},	/* SPI0_SCK		PLL_TX_SCK*/
	{0, 12, (IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN)},/*(SPI0 MOSI) 	PLL_TX_DATA*/
//	{0, 13, (IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN)},/* NC */
	{0, 14, (IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN)},/* SPI0_SSEL0 	PLL_TX_SEL*/	
//	{0, 15, (IOCON_FUNC1 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN)},/* NC*/

	/*调试管脚*/
//	{0, 16, (IOCON_FUNC5 | IOCON_MODE_PULLDOWN | IOCON_DIGITAL_EN)},/* SWCLK */
//	{0, 17, (IOCON_FUNC5 | IOCON_MODE_PULLDOWN | IOCON_DIGITAL_EN)},/* SWDIO */
	
//	{0, 18, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)},	/* NC*/
//	{0, 19, (IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN)},	/* NC*/
//	{0, 20, (IOCON_FUNC0 | IOCON_MODE_PULLDOWN | IOCON_DIGITAL_EN)},/* NC*/

	/*串口*/
	{0, 21, (IOCON_FUNC2 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)},	/* U0_TXD*/
	{0, 22, (IOCON_FUNC2 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)},	/* U0_RXD */
	
	/* I2C0-I2C1 */
	{0, 23, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN | IOCON_STDI2C_EN)}, /* I2C0_SCL (SCL) */
	{0, 24, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN | IOCON_STDI2C_EN)}, /* I2C0_SDA-WAKEUP (SDA) */
	{0, 25, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN | IOCON_STDI2C_EN)},	/* I2C1_SCL (SCL) */
	{0, 26, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN | IOCON_STDI2C_EN)},	/* I2C1_SDA-WAKEUP (SDA) */

	/* GPIO27-28 */
	{0, 27, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN )}, /* PIO0_27	FGU_INT	*/
	{0, 28, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN | IOCON_STDI2C_EN)}, /* PIO0_28	PS_TX	*/

//	{0, 29, (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_DIGITAL_EN )}, /* NC */
//	{0, 30, (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_DIGITAL_EN )}, /* NC */
//	{0, 31, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN )}, /* PIO0_31	ISP_EN */


	/* ADC0通道3-8*/
	{1, 0,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_ANALOG_EN)},	/* ADC3 */
	{1, 1,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_ANALOG_EN)},	/* ADC4 */
	{1, 2,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_ANALOG_EN)},	/* ADC5 */
	{1, 3,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_ANALOG_EN)},	/* ADC6 */
	{1, 4,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_ANALOG_EN)},	/* ADC7 */
	{1, 5,  (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_ANALOG_EN)},	/* ADC8 */
	

//	{1, 6,  (IOCON_FUNC0 | IOCON_MODE_INACT)},	/* NC */
//	{1, 7,  (IOCON_FUNC0 | IOCON_MODE_INACT)},	/* NC */
//	{1, 8,  (IOCON_FUNC0 | IOCON_MODE_INACT)},	/* NC */

	/*CT32B0_CAP2*/
	{1, 9,  (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)},	/* CT32B0_CAP2 	FAN_FG3*/
	
//	{1, 10, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)},	/* NC */
//	{1, 11, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)},	/* NC */

	/*SPI1---spi转4路DAC*/
	{1, 12, (IOCON_FUNC4 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN)},	/* SPI1_SCK 	DAC_SCK	*/
	{1, 13, (IOCON_FUNC4 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN)},	/* SPI1_MOSI	DAC_DATA*/
//	{1, 14, (IOCON_FUNC4 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN)},/* SPI1_MISO 	NC	*/
	{1, 15, (IOCON_FUNC4 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN)},/* SPI1_SSEL0 	DAC_SLE*/

	/*两路GPIO*/
	{1, 16, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)},	/* PIO1_16	ATT_ENA*/
	{1, 17, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)},	/* PIO1_17	NB/WB	*/
};

#endif
#ifndef BOARD_USECLKINSRC
#define BOARD_USECLKINSRC   (0)
#endif

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
// * Private functions
// ****************************************************************************/
//#define BRIDGE_SSEL_PORT 0
//#define BRIDGE_SSEL_PIN 14
//static void ConfigureBridgeSSEL(void)
//{
//	PINMUX_GRP_T pinMuxBridgeSSEL[] = {
//		{BRIDGE_SSEL_PORT, BRIDGE_SSEL_PIN, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN)}
//	};
//	/* Default bits to Link processor powered down. */
//	uint32_t functionBits = (IOCON_FUNC1 | IOCON_MODE_PULLDOWN | IOCON_DIGITAL_EN);

//	/* Set the bridge SSEL pin to GPIO pull down so we can read the state */
//	Chip_IOCON_SetPinMuxing(LPC_IOCON, pinMuxBridgeSSEL, sizeof(pinMuxBridgeSSEL) / sizeof(PINMUX_GRP_T));

//	/* Drive the bridge SSEL pin low and then read it back */
//	Chip_GPIO_SetPinDIROutput(LPC_GPIO, BRIDGE_SSEL_PORT, BRIDGE_SSEL_PIN);
//	Chip_GPIO_SetPinState(LPC_GPIO, BRIDGE_SSEL_PORT, BRIDGE_SSEL_PIN, false);

//	/* Set direction back to input and if the pin reads high, we know the link processor is powered */
//	Chip_GPIO_SetPinDIRInput(LPC_GPIO, BRIDGE_SSEL_PORT, BRIDGE_SSEL_PIN);
//	if (Chip_GPIO_GetPinState(LPC_GPIO, BRIDGE_SSEL_PORT, BRIDGE_SSEL_PIN)) {

//		/* Set function bits when Link processor present */
//		functionBits = (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_DIGITAL_EN);
//	}

//	pinMuxBridgeSSEL[0].modefunc = functionBits;
//	Chip_IOCON_SetPinMuxing(LPC_IOCON, pinMuxBridgeSSEL, sizeof(pinMuxBridgeSSEL) / sizeof(PINMUX_GRP_T));
//}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Sets up system pin muxing */
void Board_SetupMuxing(void)
{
	/* Enable IOCON clock */
	Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_IOCON);

	Chip_IOCON_SetPinMuxing(LPC_IOCON, pinmuxing, sizeof(pinmuxing) / sizeof(PINMUX_GRP_T));

	/* Bridge SSEL requires detection to set state correctly */
//	ConfigureBridgeSSEL();

	/* IOCON clock left on, this is needed if CLKIN is used. */
}
/* Set up and initialize clocking prior to call to main */
void Board_SetupClocking(void)
{
	/* The IRC is always the first clock source even if CLK_IN is used later.
	   Once CLK_IN is selected as the clock source. We can turned off the IRC later.
	   Turn on the IRC by clearing the power down bit */
	Chip_SYSCON_PowerUp(SYSCON_PDRUNCFG_PD_IRC_OSC | SYSCON_PDRUNCFG_PD_IRC);

#if BOARD_USECLKINSRC == (0)
	/* Setup PLL based on (internal) IRC clocking */
//	Chip_SetupIrcClocking(BOARD_MAIN_CLOKKRATE_48M);
	Chip_SetupIrcClocking(BOARD_MAINCLOCKRATE);
#elif BOARD_USECLKINSRC == (1)
	Chip_SetupRtcClocking(BOARD_MAINCLOCKRATE);
#else
	/* Setup PLL based on (external) CLKIN clocking */
	Chip_SetupExtInClocking(BOARD_MAINCLOCKRATE);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 21,
				 (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_DIGITAL_EN));

#endif


	/* Select the CLKOUT clocking source */
	Chip_Clock_SetCLKOUTSource(SYSCON_CLKOUTSRC_CLKIN, 1);

}

/* Set up and initialize hardware prior to call to main */
void Board_SystemInit(void)
{
	/* Setup system clocking and muxing */
	Board_SetupMuxing();	
	Board_SetupClocking();
}
