/*
 * @brief ADC interrupt example using the ROM API
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
#include "romapi_adc.h"

/** @defgroup ADCALT_INT_5410X ADC interrupt example using the ROM API
 * @ingroup EXAMPLES_ROM_5410X
 * @include "rom\adc_int\readme.txt"
 */

/**
 * @}
 */

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
/* Get 32 samples for 3 channels CH0, CH3 and CH4 */
#define NUM_SAMPLES_A    32
#define NUM_CHANNELS_A    3
#define SAMPLE_COUNT_A    16/* Run 16 times */

/* Get 64 samples for 4 channels CH1, CH5, CH7 and CH10 */
#define NUM_SAMPLES_B    32
#define NUM_CHANNELS_B    4
#define SAMPLE_COUNT_B    32/* Run 32 times */

/* SEQ_A enables channels 0, 3 and 4; Uses software trigger; doesn't use BURST */
#define ADC_SEQ_A_CONFIG  \
	TRIG_SOFT |		/* Software trigger for SEQ_A */ \
	TRIG_POL_POS |	/* UM recommends this for S/W trigger */ \
	MODE_EOS |		/* Event generated after Sequence done */ \
	ENABLE_CH(0) | ENABLE_CH(3) | ENABLE_CH(4)	/* Associate channels 0, 2 and 3 to SEQ_A */

/* SEQ_B enables channels 1, 5, 7 and 10; Uses PININT1 as trigger and works on Burst mode */
#define ADC_SEQ_B_CONFIG  \
	TRIG_PININT1 |	/* Hardware trigger for SEQ_B (PININT1) */ \
	TRIG_POL_POS |	/* Use SW1 as the trigger for SEQ_B */ \
	MODE_EOS |		/* Event generated after Sequence done */ \
	MODE_BURST |	/* Enable burst mode */	\
	ENABLE_CH(1) | ENABLE_CH(5) | ENABLE_CH(7) | ENABLE_CH(10)	/* Associate channels 1, 5, 7 & 10 to SEQ_B */

/* General ADC configuration */
#define ADC_CONFIG \
	MODE_SYNC |	/* Enable synchronous mode */ \
	RESOL_12BIT |	/* Use 12-Bit resolution */	\
	SAMPLE_TIME(0)	/* No extra clocks */

/* Buffer pointers */
static uint16_t buff_A[NUM_SAMPLES_A][NUM_CHANNELS_A];	/* Memory to hold samples for SEQ_A */
static uint16_t buff_B[NUM_SAMPLES_B][NUM_CHANNELS_B];	/* Memory to hold samples for SEQ_B */
static uint16_t(*volatile pBufA)[NUM_CHANNELS_A];	/* Pointer to current active SEQ_A buffer */
static uint16_t(*volatile pBufB)[NUM_CHANNELS_B];	/* Pointer to current active SEQ_B buffer */

/* ADC Driver context memory */
#define RAMBLOCK_H          60
static uint32_t  start_of_ram_block0[RAMBLOCK_H];

/* ADC ROM Driver Handle */
static ADC_HANDLE_T *hADC;

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

static void ADC_PinMuxSetup(void)
{
#if defined(BOARD_NXP_LPCXPRESSO_54102)
	/* Enable PININT1, which will trigger SEQ_B */
	Chip_PININT_Init(LPC_PININT);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 24, IOCON_MODE_INACT | IOCON_FUNC0 | IOCON_DIGITAL_EN | IOCON_GPIO_MODE);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, 0, 24);
	LPC_INMUX->PINTSEL[PININTSELECT1] = 24;
	Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH(PININTSELECT1));
	Chip_PININT_SetPinModeEdge(LPC_PININT, PININTCH(PININTSELECT1));
	Chip_PININT_EnableIntLow(LPC_PININT, PININTCH(PININTSELECT1));
	NVIC_ClearPendingIRQ(PIN_INT1_IRQn);
	NVIC_EnableIRQ(PIN_INT1_IRQn);

	/* All pins to inactive, neither pull-up nor pull-down. */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 29, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_ANALOG_EN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 30, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_ANALOG_EN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 31, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_ANALOG_EN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 0, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_ANALOG_EN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 1, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_ANALOG_EN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 2, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_ANALOG_EN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 3, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_ANALOG_EN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 4, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_ANALOG_EN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 5, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_ANALOG_EN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 6, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_ANALOG_EN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 7, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_ANALOG_EN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 8, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_ANALOG_EN);

#else
#warning "No ADC setup for this example"
#endif
}

/* Initialize the ADC ROM Driver */
static int adcrom_init(void)
{
	volatile int size_in_bytes;

	ADC_PinMuxSetup();
	Chip_SYSCON_PowerUp(SYSCON_PDRUNCFG_PD_ADC0 | SYSCON_PDRUNCFG_PD_VDDA_ENA | SYSCON_PDRUNCFG_PD_VREFP);
	Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_ADC0);

//	Chip_Clock_SetADCClockSource(SYSCON_ADCCLKSELSRC_MAINCLK);
//	Chip_Clock_SetADCClockDiv(0x2);

	size_in_bytes =  ROM_ADC_GetMemSize();

	if (RAMBLOCK_H < (size_in_bytes / 4)) {
		return 1;
	}

	hADC = ROM_ADC_Init(start_of_ram_block0, LPC_ADC_BASE, 0);

	return 0;
}

/* Configure ADC ROM Driver and pripheral */
static int adcrom_config(void)
{
	ADC_CFG_T cfg = {
		ADC_SEQ_A_CONFIG,
		ADC_SEQ_B_CONFIG,
		ADC_CONFIG,
		0	/* Divider will be calculated during run time */
	};

	cfg.clkDiv = 0xFF;

	/* Configure the ADC */
	ROM_ADC_Configure(hADC, &cfg);

	/* Calibrate the ADC */
	if (ROM_ADC_Calibrate(hADC, Chip_Clock_GetSystemClockRate()) != LPC_OK) {
		DEBUGSTR("ERROR: Calibrating ADC\r\n");
		while (1) {}
	}
	DEBUGSTR("ADC Initialized and Calibrated successfully!\r\n");

	/* Channel configurations */
	ROM_ADC_ConfigureCh(hADC, 0, ADC_CH_THRES_DATA | ADC_CH_THRES_SEL1 | ADC_CH_THRES_CROSSING);
	ROM_ADC_ConfigureCh(hADC, 1, ADC_CH_THRES_DATA);

	return 0;
}

/* Enable ADC interrupts */
static void adcrom_int_enable(void)
{
	NVIC_SetPriority(ADC_THCMP_IRQn, 0x00);
	NVIC_ClearPendingIRQ(ADC_THCMP_IRQn);
	NVIC_EnableIRQ(ADC_THCMP_IRQn);

	NVIC_SetPriority(ADC_SEQA_IRQn, 0x00);
	NVIC_ClearPendingIRQ(ADC_SEQA_IRQn);
	NVIC_EnableIRQ(ADC_SEQA_IRQn);

	NVIC_SetPriority(ADC_SEQB_IRQn, 0x00);
	NVIC_ClearPendingIRQ(ADC_SEQB_IRQn);
	NVIC_EnableIRQ(ADC_SEQB_IRQn);
}

/* Call-Back function for ADC Start/Stop event */
static void adcrom_startstop(ADC_HANDLE_T hADC, ADC_CBINDEX_T idx, void *arg)
{
	switch (idx) {
	case ADC_START_SEQ_A:
		DEBUGSTR("Sequence A Started!\r\n");
		break;

	case ADC_START_SEQ_B:
		DEBUGSTR("Sequence B Started!\r\n");
		break;

	case ADC_STOP_SEQ_A:
		DEBUGSTR("Sequence A Stopped!\r\n");
		break;

	case ADC_STOP_SEQ_B:
		DEBUGSTR("Sequence B Stopped!\r\n");
		break;

	default:
		break;
	}
}

/* Callback function for Buffer Done event */
static void adcrom_bufdone(ADC_HANDLE_T hADC, ADC_CBINDEX_T idx, void *arg)
{
	static uint32_t idxA, idxB;

	if (idx == ADC_BUFFER_DONE_A) {
		/* Buffer A Complte */
		pBufA = arg;
		if (++idxA >= SAMPLE_COUNT_A) {
			return;
		}

		ROM_ADC_StartConversion(hADC, ADC_SEQ_A, &buff_A[idxA & 1][0], (NUM_SAMPLES_A >> 1) * NUM_CHANNELS_A);
	}
	else {
		pBufB = arg;
		/* Buffer B Complete */
		if (++idxB >= SAMPLE_COUNT_B) {
			return;
		}

		ROM_ADC_StartConversion(hADC, ADC_SEQ_B, &buff_B[idxB & 1][0], (NUM_SAMPLES_B >> 1) * NUM_CHANNELS_B);
	}
}

/* Print content of buffer_A to UART */
static void print_buf_A(uint32_t cnt)
{
	int i, j;
	uint16_t(*buf)[NUM_CHANNELS_A] = buff_A;

	pBufA = 0;
	if (!buf) {
		return;
	}

	for (i = 0; i < cnt; i++) {
		DEBUGSTR("SEQ_A: ");
		for (j = 0; j < NUM_CHANNELS_A; j++) {
			DEBUGOUT("%03X ", buf[i][j]);
		}
		DEBUGSTR("\r\n");
	}
}

/* Print content of buffer_B to UART */
static void print_buf_B(uint32_t cnt)
{
	int i, j;
	uint16_t(*buf)[NUM_CHANNELS_B] = pBufB;

	pBufB = 0;
	if (!buf) {
		return;
	}

	for (i = 0; i < cnt; i++) {
		DEBUGSTR("SEQ_B: ");
		for (j = 0; j < NUM_CHANNELS_B; j++) {
			DEBUGOUT("%03X ", buf[i][j]);
		}
		DEBUGSTR("\r\n");
	}
}

/* Register ADC event Callback functions */
static void adcrom_regcb(void)
{
	ROM_ADC_RegisterCB(hADC, ADC_BUFFER_DONE_A, adcrom_bufdone);/* Buffer A Done */
	ROM_ADC_RegisterCB(hADC, ADC_BUFFER_DONE_B, adcrom_bufdone);/* Buffer B Done */
	ROM_ADC_RegisterCB(hADC, ADC_STOP_SEQ_A, adcrom_startstop);	/* SEQ_A Stop */
	ROM_ADC_RegisterCB(hADC, ADC_START_SEQ_A, adcrom_startstop);/* SEQ_A Start */
	ROM_ADC_RegisterCB(hADC, ADC_STOP_SEQ_B, adcrom_startstop);	/* SEQ_B Stop */
	ROM_ADC_RegisterCB(hADC, ADC_START_SEQ_B, adcrom_startstop);/* SEQ_B start */
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* ADC SEQ_A interrupt handler */
void ADC_SEQA_IRQHandler(void)
{
	ROM_ADC_Handler(hADC, ADC_EV_SEQ_A_INT);
}

/* ADC SEQ_B interrupt handler */
void ADC_SEQB_IRQHandler(void)
{
	ROM_ADC_Handler(hADC, ADC_EV_SEQ_B_INT);
}

/* ADC Overflow or THCMP handler */
void ADC_THCMP_IRQHandler(void) {
	ROM_ADC_Handler(hADC, ADC_EV_OVR_INT);
}

/* PININT1 IRQ handler */
void PIN_INT1_IRQHandler(void)
{
	Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH(PININTSELECT1));
}

/**
 * @brief	Main routine for I2C example
 * @return	Function should not exit
 */
int main(void)
{
	int i = 1;
	/* Generic Initialization */
	SystemCoreClockUpdate();
	Board_Init();

	/* Initialize the PinMux and setup the memory for ROM driver */
	if (adcrom_init()) {
		return 1;
	}

	/* Configure the ADC */
	if (adcrom_config()) {
		return 1;
	}

	adcrom_regcb();	/* Register call-back functions */

	adcrom_int_enable();/* Enable interrupts */

	if (ROM_ADC_StartConversion(hADC, ADC_SEQ_B, &buff_B[0][0], (NUM_SAMPLES_B >> 1) * NUM_CHANNELS_B) != LPC_OK) {
		DEBUGSTR("ERROR: Starting conversion in SEQ_B\r\n");
	}

	/* Fill the first half of buffer with conversion samples */
	if (ROM_ADC_StartConversion(hADC, ADC_SEQ_A, &buff_A[0][0], (NUM_SAMPLES_A >> 1) * NUM_CHANNELS_A) != LPC_OK) {
		DEBUGSTR("ERROR: Starting conversion in SEQ_A\r\n");
	}

	while (i) {
		print_buf_A(1);	/* Print first sample from the buffer */
		print_buf_B(1);	/* Print first sample from the buffer */
		__WFI();
	}
	return 0;
}
