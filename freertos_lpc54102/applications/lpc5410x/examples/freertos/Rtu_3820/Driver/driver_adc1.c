/*
 * @brief ADC interrupt/DMA example using the ROM API
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
#define ADC_GLOBALS

#include "board.h"
#include "romapi_dma.h"
#include "romapi_adc.h"
#include "FreeRTOS.h"

#include "driver_spi.h"
#include "driver_adc.h"
#include "queue.h"
#include "semphr.h" 

//extern ALIGN(512)  ROM_DMA_DESC_T EXChip_DMA_Table[MAX_DMA_CHANNEL];
extern xQueueHandle Sem_Adc_Deal_A;//Adc 数据处理信号量---10ms执行一次
extern xQueueHandle Sem_Adc_Deal_B;//Adc 数据处理信号量---10ms执行一次

extern xQueueHandle Sem_Adc_Deal_A_End;//Adc 数据处理信号量---10ms执行一次
extern xQueueHandle Sem_Adc_Deal_B_End;//Adc 数据处理信号量---10ms执行一次



/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

static void ADC_PinMuxSetup(void)
{
#if defined(BOARD_NXP_LPCXPRESSO_54102)
//	/* Enable PININT1, which will trigger SEQ_B */
//	Chip_PININT_Init(LPC_PININT);
//	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 24, IOCON_MODE_INACT | IOCON_FUNC0 | IOCON_DIGITAL_EN | IOCON_GPIO_MODE);
//	Chip_GPIO_SetPinDIRInput(LPC_GPIO, 0, 24);
//	LPC_INMUX->PINTSEL[PININTSELECT1] = 24;
//	Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH(PININTSELECT1));
//	Chip_PININT_SetPinModeEdge(LPC_PININT, PININTCH(PININTSELECT1));
//	Chip_PININT_EnableIntLow(LPC_PININT, PININTCH(PININTSELECT1));
//	NVIC_ClearPendingIRQ(PIN_INT1_IRQn);
//	NVIC_EnableIRQ(PIN_INT1_IRQn);

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

	Chip_Clock_SetADCClockSource(SYSCON_ADCCLKSELSRC_MAINCLK);
	Chip_Clock_SetADCClockDiv(0x1);

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
	NVIC_SetPriority(ADC_THCMP_IRQn, 0x02);
	NVIC_ClearPendingIRQ(ADC_THCMP_IRQn);
	NVIC_EnableIRQ(ADC_THCMP_IRQn);

	NVIC_SetPriority(ADC_SEQA_IRQn, 0x02);
	NVIC_ClearPendingIRQ(ADC_SEQA_IRQn);
	NVIC_EnableIRQ(ADC_SEQA_IRQn);

	NVIC_SetPriority(ADC_SEQB_IRQn, 0x02);
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
		if (++idxA >= NUM_SAMPLES_A) {
			return;
		}

		ROM_ADC_StartConversion(hADC, ADC_SEQ_A, &buff_A[idxA & 1][0], (NUM_SAMPLES_A >> 1) * NUM_CHANNELS_A);
	}
	else {
		pBufB = arg;
		/* Buffer B Complete */
		if (++idxB >= NUM_SAMPLES_A) {
			return;
		}

		ROM_ADC_StartConversion(hADC, ADC_SEQ_B, &buff_B[idxB & 1][0], (NUM_SAMPLES_B >> 1) * NUM_CHANNELS_B);
	}
}

/* Print content of buffer_A to UART */
 void print_buf_A(uint32_t cnt)
{
	int i, j;
	uint16_t(*buf)[NUM_CHANNELS_A] = pBufA;

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
int ma1112in(void)
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


/*****************************************************************************
 * 函 数 名  : RTU_AdcDev_Init
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月25日
 * 函数功能  : ADC初始化，romapi
 * 输入参数  : void  wu 
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void RTU_AdcDev_Init(void)
{
	
	/* Initialize the PinMux and setup the memory for ROM driver */
	if (adcrom_init()) {
		return ;
	}

	/* Configure the ADC */
	if (adcrom_config()) {
		return ;
	}

	adcrom_regcb();	/* Register call-back functions */

	adcrom_int_enable();/* Enable interrupts */
		/* adcrom_int_enable(); */ /* ADC interrupts are not required DMA interrupts will handle data transfers */
	//	adcrom_dma_init();
	
//		if (ROM_ADC_StartConversion(hADC, ADC_SEQ_B, &buff_B[0][0], NUM_SAMPLES_B * NUM_CHANNELS_B) != LPC_OK) {
//			DEBUGSTR("ERROR: Starting conversion in SEQ_B\r\n");
//		}
//	
//		/* Fill the first half of buffer with conversion samples */
//		if (ROM_ADC_StartConversion(hADC, ADC_SEQ_A, &buff_A[0][0], NUM_SAMPLES_A * NUM_CHANNELS_A) != LPC_OK) {
//			DEBUGSTR("ERROR: Starting conversion in SEQ_A\r\n");
//		}

		/* Fill the first half of buffer with conversion samples */
			if (ROM_ADC_StartConversion(hADC, ADC_SEQ_A, &buff_A[0][0], NUM_SAMPLES_A * NUM_CHANNELS_A) != LPC_OK) {
				DEBUGSTR("ERROR: Starting conversion in SEQ_A\r\n");
			}

}

/**
 * @brief	Main routine for I2C example
 * @return	Function should not exit
 */
int main11(void)
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

	/* adcrom_int_enable(); */ /* ADC interrupts are not required DMA interrupts will handle data transfers */
	//adcrom_dma_init();

	if (ROM_ADC_StartConversion(hADC, ADC_SEQ_B, &buff_B[0][0], NUM_SAMPLES_B * NUM_CHANNELS_B) != LPC_OK) {
		DEBUGSTR("ERROR: Starting conversion in SEQ_B\r\n");
	}

	/* Fill the first half of buffer with conversion samples */
	if (ROM_ADC_StartConversion(hADC, ADC_SEQ_A, &buff_A[0][0], NUM_SAMPLES_A * NUM_CHANNELS_A) != LPC_OK) {
		DEBUGSTR("ERROR: Starting conversion in SEQ_A\r\n");
	}

	while (i) {
		print_buf_A(NUM_SAMPLES_A);	/* Print first sample from the buffer */
		print_buf_B(NUM_SAMPLES_B);	/* Print first sample from the buffer */
		__WFI();
	}
	return 0;
}

