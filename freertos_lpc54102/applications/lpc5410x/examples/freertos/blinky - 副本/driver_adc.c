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

#include "board.h"
#include "romapi_dma.h"
#include "romapi_adc.h"

/** @defgroup ADCALT_INT_DMA_5410X ADC interrupt/DMA example using the ROM API
 * @ingroup EXAMPLES_ROM_5410X
 * @include "rom\adc_int_dma\readme.txt"
 */

/**
 * @}
 */

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
#ifdef __ICCARM__
#define ALIGNSTR(x) # x
#define ALIGN(x) _Pragma(ALIGNSTR(data_alignment = ## x))
#else
#define ALIGN(x) __attribute__ ((aligned(x)))
#endif

/* Get 32 samples for 1 channels CH8 */
#define NUM_SAMPLES_A    32
#define NUM_CHANNELS_A    1
#define ADC_SEQ_A_DMA_CHNUM 0

/* DMA trigger mapping */
#define INMUX_ADC0_SEQA_DMA 0
#define INMUX_ADC0_SEQB_DMA 1

/* Get 64 samples for 5 channels CH3,  CH4, CH5, CH6 and CH7 */
#define NUM_SAMPLES_B    16
#define NUM_CHANNELS_B    5
#define ADC_SEQ_B_DMA_CHNUM 1

/* SEQ_A enables channels 0, 3 and 4; Uses software trigger; doesn't use BURST */
#define ADC_SEQ_A_CONFIG  \
	TRIG_SOFT |		/* Software trigger for SEQ_A */ \
	TRIG_POL_POS |	/* UM recommends this for S/W trigger */ \
	MODE_EOC |		/* Event generated after conversion is done */ \
	MODE_SINGLE |	/* Generate DMA Trigger for every channel read */ \
	ENABLE_CH(8) 	/* Associate channels 0, 2 and 3 to SEQ_A */

/* SEQ_B enables channels 1, 5, 6 and 7; Uses PININT1 as trigger and works on Burst mode */
#define ADC_SEQ_B_CONFIG  \
	TRIG_PININT1 |	/* Hardware trigger for SEQ_B (PININT1) */ \
	TRIG_POL_POS |	/* Use SW1 as the trigger for SEQ_B */ \
	MODE_BURST | MODE_EOS |		/* Event generated after Sequence done */ \
	ENABLE_CH(3) | ENABLE_CH(4)| ENABLE_CH(5) | ENABLE_CH(6) | ENABLE_CH(7)	/* Associate channels 3 4, 5, 6 & 7 to SEQ_B */

/* General ADC configuration */
#define ADC_CONFIG \
	MODE_SYNC |	/* Enable synchronous mode */ \
	RESOL_12BIT |	/* Use 12-Bit resolution */	\
	ENABLE_OVR |/* Enable overflow interrupt */	\
	SAMPLE_TIME(0)	/* No extra clocks */

/* Buffer pointers */
static uint32_t buff_A[NUM_SAMPLES_A][NUM_CHANNELS_A];	/* Memory to hold samples for SEQ_A */
static uint32_t buff_B[NUM_SAMPLES_B][NUM_CHANNELS_B];	/* Memory to hold samples for SEQ_B */

/* DMA Queues */
ROM_DMA_QUEUE_T seqAQueue, seqBQueue;

/* ADC Driver context memory */
#define RAMBLOCK_H          60
static uint32_t  start_of_ram_block0[RAMBLOCK_H];

/* ADC ROM Driver Handle */
static ADC_HANDLE_T *hADC;
 ROM_DMA_HANDLE_T *hDMA;

/* DMA Descriptor table memory */
extern ALIGN(512) ROM_DMA_DESC_T EXChip_DMA_Table[MAX_DMA_CHANNEL];
ALIGN(16) ROM_DMA_DESC_T dmaDescA[NUM_CHANNELS_A];
ALIGN(16) ROM_DMA_DESC_T dmaDescB[NUM_CHANNELS_B];

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

	/* Setup DMA Trigger inputs */
//	LPC_INMUX->DMA_ITRIG_INMUX[ADC_SEQ_A_DMA_CHNUM] = INMUX_ADC0_SEQA_DMA;
//	LPC_INMUX->DMA_ITRIG_INMUX[ADC_SEQ_B_DMA_CHNUM] = INMUX_ADC0_SEQB_DMA;

	/* All pins to inactive, neither pull-up nor pull-down. */
//	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 29, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_ANALOG_EN);
//	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 30, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_ANALOG_EN);
//	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 31, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_ANALOG_EN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 0, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_ANALOG_EN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 1, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_ANALOG_EN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 2, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_ANALOG_EN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 3, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_ANALOG_EN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 4, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_ANALOG_EN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 5, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_ANALOG_EN);
//	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 6, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_ANALOG_EN);
//	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 7, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_ANALOG_EN);
//	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 8, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_ANALOG_EN);

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

static uint32_t idxA;

static void adcrom_setDMA_A(uint32_t *arg)
{
	static uint32_t *src;
	int i;
	ROM_DMA_XFERDESC_CFG_T XferCfg;
	ROM_DMA_DESC_T *pPrevDesc;

	/* clear stack structure before use */
	memset(&XferCfg, 0, sizeof(ROM_DMA_XFERDESC_CFG_T));

	/* Store it during the first start */
	if (!src) {
		src = arg;
	}

	/* Setup source to desination copy for trigger for memory */
	XferCfg.xferCount = 1;							/* Transfer 1 data values of size width */
	XferCfg.swTrig = 0;							/* No software triggering */
	XferCfg.clrTrig = 1;							/* Clear trigger after this descriptor completes */
	XferCfg.fireDescCB = 1;						/* Fire descriptor callback on complettion of this descriptor */
	XferCfg.enabCirc = 0;							/* Not a circular buffer */
	XferCfg.dmaCh = seqAQueue.dmaCh;
	XferCfg.stallDesc = 0;
	XferCfg.width = ROM_DMA_WIDTH_4;			/* Width is 4 bytes */
	XferCfg.srcInc = ROM_DMA_ADDRINC_0X;	/* Increment source address by width for each data transfer */
	XferCfg.dstInc = ROM_DMA_ADDRINC_1X;	/* Increment destination address by width for each data transfer */
	XferCfg.src  = (void *) ADC_DMA_ADDR(src, ADC_DMA_SRC_SEQA);	/* Source address */
	pPrevDesc = NULL;
	for (i = 0; i < NUM_CHANNELS_A; i++) {
		XferCfg.dest = (void *) &buff_A[idxA][i];	/* Destination address */
		if (ROM_DMA_BuildDescriptorChain(hDMA, &XferCfg, &dmaDescA[i], pPrevDesc) != LPC_OK) {
			DEBUGSTR("Error building descriptor chain (single link)\r\n");
		}
		pPrevDesc = &dmaDescA[i];
	}

	/* Queue descriptor for transfer */
	ROM_DMA_QueueDescriptor(hDMA, &seqAQueue, &dmaDescA[0]);

	/* Start queue, won't transfer until trigger occurs */
	if (ROM_DMA_StartQueue(hDMA, &seqAQueue) != LPC_OK) {
		DEBUGSTR("Error starting queue\r\n");
	}
}

static uint32_t idxB;

static void adcrom_setDMA_B(uint32_t *arg)
{
	static uint32_t *src;
	ROM_DMA_XFERDESC_CFG_T XferCfg;

	/* clear stack structure before use */
	memset(&XferCfg, 0, sizeof(ROM_DMA_XFERDESC_CFG_T));

	/* Store it during the first start */
	if (!src) {
		src = arg;
	}

	/* Setup source to desination copy for trigger for memory */
	XferCfg.xferCount = 1;							/* Transfer 1 data values of size width */
	XferCfg.swTrig = 0;							/* No software triggering */
	XferCfg.clrTrig = 0;							/* Clear trigger after this descriptor completes */
	XferCfg.fireDescCB = 0;						/* Fire descriptor callback on complettion of this descriptor */
	XferCfg.enabCirc = 0;							/* Not a circular buffer */
	XferCfg.dmaCh = seqBQueue.dmaCh;
	XferCfg.stallDesc = 0;
	XferCfg.width = ROM_DMA_WIDTH_4;			/* Width is 4 bytes */
	XferCfg.srcInc = ROM_DMA_ADDRINC_0X;	/* Increment source address by width for each data transfer */
	XferCfg.dstInc = ROM_DMA_ADDRINC_1X;	/* Increment destination address by width for each data transfer */
	XferCfg.src  = (void *) ADC_DMA_ADDR(src, ADC_DMA_SRC_CH1);	/* Source address */
	XferCfg.dest = (void *) &buff_B[idxB][0];
	if (ROM_DMA_BuildDescriptorChain(hDMA, &XferCfg, &dmaDescB[0], NULL) != LPC_OK) {
		DEBUGSTR("Error building descriptor chain (single link)\r\n");
	}

	XferCfg.xferCount = 5;					/* Transfer 3 data values of size width */
	XferCfg.clrTrig = 1;					/* Clear trigger after this descriptor completes */
	XferCfg.fireDescCB = 1;				/* Fire descriptor callback on complettion of this descriptor */
	XferCfg.srcInc = ROM_DMA_ADDRINC_1X;	/* Increment source address by width for each data transfer */
	XferCfg.src  = (void *) ADC_DMA_ADDR(src, ADC_DMA_SRC_CH3);	/* Source address */
	XferCfg.dest = (void *) &buff_B[idxB][1];
	if (ROM_DMA_BuildDescriptorChain(hDMA, &XferCfg, &dmaDescB[1], &dmaDescB[0]) != LPC_OK) {
		DEBUGSTR("Error building descriptor chain (single link)\r\n");
	}

	/* Queue descriptor for transfer */
	ROM_DMA_QueueDescriptor(hDMA, &seqBQueue, &dmaDescB[0]);

	/* Start queue, won't transfer until trigger occurs */
	if (ROM_DMA_StartQueue(hDMA, &seqBQueue) != LPC_OK) {
		DEBUGSTR("Error starting queue\r\n");
	}
}

/**
 * @brief	DMA controller interrupt handler
 * @return	Nothing
 */


/* DMA controller transfer descriptor chain complete callback */
static void adcrom_xfer_done_A(ROM_DMA_HANDLE_T dmaHandle, struct ROM_DMA_QUEUE *pQueue, ROM_DMA_DESC_T *pTranDesc)
{
	/* Re-create descriptors for next xfer */
	idxA++;
	adcrom_setDMA_A(0);

	/* Stop SEQ-A when we are done! */
	if (idxA >= NUM_SAMPLES_A) {
		ROM_ADC_StopConversion(hADC, ADC_SEQ_A);
	}
}

/* DMA controller transfer descriptor complete callback */
static void adcrom_desc_done_A(ROM_DMA_HANDLE_T dmaHandle, struct ROM_DMA_QUEUE *pQueue, ROM_DMA_DESC_T *pTranDesc)
{
	ROM_ADC_Handler(hADC, ADC_EV_SEQ_A_DMA);
}

/*DMA controller transfer descriptor error callback */
static void adcrom_xfer_err_A(ROM_DMA_HANDLE_T dmaHandle, struct ROM_DMA_QUEUE *pQueue, ROM_DMA_DESC_T *pTranDesc)
{
	DEBUGSTR("DMA ERR (A)!\r\n");
	ROM_ADC_StopConversion(hADC, ADC_SEQ_A);
}

/* DMA controller transfer descriptor chain complete callback */
static void adcrom_xfer_done_B(ROM_DMA_HANDLE_T dmaHandle, struct ROM_DMA_QUEUE *pQueue, ROM_DMA_DESC_T *pTranDesc)
{
	/* Re-create descriptors for next xfer */
	idxB++;
	adcrom_setDMA_B(0);

	/* Stop SEQ-A when we are done! */
	if (idxB >= NUM_SAMPLES_B) {
		ROM_ADC_StopConversion(hADC, ADC_SEQ_B);
	}
}

/* DMA controller transfer descriptor complete callback */
static void adcrom_desc_done_B(ROM_DMA_HANDLE_T dmaHandle, struct ROM_DMA_QUEUE *pQueue, ROM_DMA_DESC_T *pTranDesc)
{
	ROM_ADC_Handler(hADC, ADC_EV_SEQ_B_DMA);
}

/*DMA controller transfer descriptor error callback */
static void adcrom_xfer_err_B(ROM_DMA_HANDLE_T dmaHandle, struct ROM_DMA_QUEUE *pQueue, ROM_DMA_DESC_T *pTranDesc)
{
	DEBUGSTR("DMA ERR (B)!\r\n");
	ROM_ADC_StopConversion(hADC, ADC_SEQ_B);
}

/* DMA Initialization */
static void adcrom_dma_init(void)
{
	static uint32_t memDMA[32];
	ROM_DMA_INIT_T dmaInit = {
		0,
		LPC_DMA_BASE,
		0
	};
	ROM_DMA_CHAN_CFG_T chanCfgA = {
		0,											/* Use peripheral DMA request */
		1,											/* Hardware trigger polarity high */
		0,											/* Hardware trigger edge triggered */
		1,											/* Burst transfer on each trigger */
		ROM_DMA_BURSTPOWER_1,						/* Burst size of 1 datum */
		0,											/* Disable source burst wrap */
		0,											/* Disable destination burst wrap */
		2,											/* Channel priority = 0 (highest) */
		0,											/* reserved */
	};
	ROM_DMA_CHAN_CFG_T chanCfgB = {
		0,											/* Use peripheral DMA request */
		1,											/* Hardware trigger polarity high */
		0,											/* Hardware trigger edge triggered */
		1,											/* Burst transfer on each trigger */
		ROM_DMA_BURSTPOWER_1,						/* Burst size of 1 datum */
		0,											/* Disable source burst wrap */
		0,											/* Disable destination burst wrap */
		1,											/* Channel priority = 0 (highest) */
		0,											/* reserved */
	};
	dmaInit.sramBase = (uint32_t) EXChip_DMA_Table;

	/* Enable DMA clocking prior to calling DMA init functions */
	Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_DMA);

	if (sizeof(memDMA) < ROM_DMA_GetMemSize()) {
		DEBUGSTR("Memory for DMA too small!\r\n");
		while (1) {}
	}

	hDMA = ROM_DMA_Init(memDMA, &dmaInit);
	if (hDMA == NULL) {
		DEBUGSTR("Unable to initialize DMA ROM Driver!\r\n");
		while (1) {}
	}
	ROM_DMA_SetupChannel(hDMA, &chanCfgA, ADC_SEQ_A_DMA_CHNUM);
	ROM_DMA_InitQueue(hDMA, ADC_SEQ_A_DMA_CHNUM, &seqAQueue);
	ROM_DMA_SetupChannel(hDMA, &chanCfgB, ADC_SEQ_B_DMA_CHNUM);
	ROM_DMA_InitQueue(hDMA, ADC_SEQ_B_DMA_CHNUM, &seqBQueue);

	/* Register error, descriptor completion, and descriptor chain completion callbacks for SEQ_A */
	ROM_DMA_RegisterQueueCallback(hDMA, &seqAQueue, ROM_DMA_XFERCOMPLETE_CB, (void *) adcrom_xfer_done_A);
	ROM_DMA_RegisterQueueCallback(hDMA, &seqAQueue, ROM_DMA_XFERDESCCOMPLETE_CB, (void *) adcrom_desc_done_A);
	ROM_DMA_RegisterQueueCallback(hDMA, &seqAQueue, ROM_DMA_XFERERROR_CB, (void *) adcrom_xfer_err_A);

	/* Register error, descriptor completion, and descriptor chain completion callbacks for SEQ_A */
	ROM_DMA_RegisterQueueCallback(hDMA, &seqBQueue, ROM_DMA_XFERCOMPLETE_CB, (void *) adcrom_xfer_done_B);
	ROM_DMA_RegisterQueueCallback(hDMA, &seqBQueue, ROM_DMA_XFERDESCCOMPLETE_CB, (void *) adcrom_desc_done_B);
	ROM_DMA_RegisterQueueCallback(hDMA, &seqBQueue, ROM_DMA_XFERERROR_CB, (void *) adcrom_xfer_err_B);

	/* Enable the interrupt for the DMA controller */
	NVIC_EnableIRQ(DMA_IRQn);
}

/* Enable ADC interrupts */
static void adcrom_int_enable(void)
{
	NVIC_SetPriority(ADC_THCMP_IRQn, 0x00);
	NVIC_ClearPendingIRQ(ADC_THCMP_IRQn);
	NVIC_EnableIRQ(ADC_THCMP_IRQn);
}

/* Call-Back function for ADC Start/Stop event */
static void adcrom_startstop(ADC_HANDLE_T hADC, ADC_CBINDEX_T idx, void *arg)
{
	switch (idx) {
	case ADC_START_SEQ_A:
		DEBUGSTR("Sequence A Started!\r\n");
		adcrom_setDMA_A(arg);
		break;

	case ADC_START_SEQ_B:
		adcrom_setDMA_B(arg);
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

/* Print content of buffer_A to UART */
static void print_buf_A(uint32_t cnt)
{
	int i, j;
	uint32_t(*buf)[NUM_CHANNELS_A] = buff_A;

	if (idxA < NUM_SAMPLES_A) {
		return;
	}

	idxA = 0;
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
	uint32_t(*buf)[NUM_CHANNELS_B] = buff_B;

	if (idxB < NUM_SAMPLES_B) {
		return;
	}

	idxB = 0;
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
	ROM_ADC_RegisterCB(hADC, ADC_STOP_SEQ_A,  adcrom_startstop);	/* SEQ_A Stop */
	ROM_ADC_RegisterCB(hADC, ADC_START_SEQ_A, adcrom_startstop);/* SEQ_A Start */
	ROM_ADC_RegisterCB(hADC, ADC_STOP_SEQ_B,  adcrom_startstop);	/* SEQ_B Stop */
	ROM_ADC_RegisterCB(hADC, ADC_START_SEQ_B, adcrom_startstop);/* SEQ_B start */
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* ADC Overflow or THCMP handler */
void ADC_THCMP_IRQHandler(void) {
	ROM_ADC_Handler(hADC, ADC_EV_OVR_INT);
}

/* PININT1 IRQ handler */
void PIN_INT1_IRQHandler(void)
{
	Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH(PININTSELECT1));
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
	
		adcrom_regcb(); /* Register call-back functions */
	
		/* adcrom_int_enable(); */ /* ADC interrupts are not required DMA interrupts will handle data transfers */
		adcrom_dma_init();
	
		if (ROM_ADC_StartConversion(hADC, ADC_SEQ_B, &buff_B[0][0], NUM_SAMPLES_B * NUM_CHANNELS_B) != LPC_OK) {
			DEBUGSTR("ERROR: Starting conversion in SEQ_B\r\n");
		}
	
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
	adcrom_dma_init();

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
