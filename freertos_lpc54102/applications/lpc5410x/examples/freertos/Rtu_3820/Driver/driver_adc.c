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
#define 	 ADC_GLOBALS

#include "board.h"
#include "romapi_adc.h"
#include "driver_adc.h"
#include "hw_adc_rom_api.h"
/* ADC Driver internal data structure */
typedef struct {
	void            *pUserData;		/* Pointer to user data */
	ADC_REGS_T      *pREGS;			/* Pointer to Registers */
	uint32_t        valSeq[2];		/* Stored SEQ A/B Values */
	uint32_t        flags;			/* flags */
	uint32_t        regThrSel;		/* Threshold flags */
	uint32_t        regInt;			/* Interrupt register */
	uint32_t        flag1;			/* Flags Extra */
	void(*cbTable[ADC_CBIDX_RESERVED]) (ADC_HANDLE_T, ADC_CBINDEX_T, void *);
} ADC_DRIVER_T;

#ifndef MALLOC_PING_PONG
RTU_PING_PONG_DATA_T ping_pong_buf_ADC;
#endif
RTU_PING_PONG_DATA_T * ping_pong_buf_ptr ;  //»´æ÷±‰¡ø£¨”√”⁄÷∏œÚADC◊®”√µƒ∆π≈“buf°£

/* Internal defines */
#define SEQ_A_MASK     0x7C0C5FFF
#define SEQ_B_MASK     0x5C0C5FFF
#define CTRL_MASK      0x00007F00

#define ADC_STATE_INACTIVE   0
#define ADC_STATE_IDLE       1
#define ADC_STATE_ACTIVE     2

/** @brief	Channel flags offset */
#define CB_THRES(ch)   (1UL << (ch))
#define CB_DATA(ch)    (0x10000UL << (ch))

ADC_DRIVER_T *pADC;

//extern ALIGN(512)  ROM_DMA_DESC_T EXChip_DMA_Table[MAX_DMA_CHANNEL];
extern xQueueHandle Sem_Adc_Deal_A;//Adc  ˝æ›¥¶¿Ì–≈∫≈¡ø---10ms÷¥––“ª¥Œ
extern xQueueHandle Sem_Adc_Deal_B;//Adc  ˝æ›¥¶¿Ì–≈∫≈¡ø---10ms÷¥––“ª¥Œ

extern xQueueHandle Sem_Adc_Deal_A_End;//Adc  ˝æ›¥¶¿Ì–≈∫≈¡ø---10ms÷¥––“ª¥Œ
extern xQueueHandle Sem_Adc_Deal_B_End;//Adc  ˝æ›¥¶¿Ì–≈∫≈¡ø---10ms÷¥––“ª¥Œ

 ADC_CFG_T cfg = {
	ADC_SEQ_A_CONFIG,
	ADC_SEQ_B_CONFIG,
	ADC_CONFIG,
	CFG_DELAY /* Divider will be calculated during run time */
};

static void ADC_PinMuxSetup(void)
{
//	/* All pins to inactive, neither pull-up nor pull-down. */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 0, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_ANALOG_EN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 1, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_ANALOG_EN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 2, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_ANALOG_EN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 3, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_ANALOG_EN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 4, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_ANALOG_EN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 5, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_ANALOG_EN);
}

/*****************************************************************************
 * ∫Ø  ˝ √˚  : Get_ADC_ping_pong_buf
 * ∏∫ ‘ »À  : MZM
 * ¥¥Ω®»’∆⁄  : 2016ƒÍ6‘¬14»’
 * ∫Ø ˝π¶ƒ‹  : ªÒ»°ADC◊®”√µƒpingpong≤Ÿ◊˜µƒª∫≥Â«¯buf
 *  ‰»Î≤Œ ˝  : Œﬁ
 *  ‰≥ˆ≤Œ ˝  : Œﬁ
 * ∑µ ªÿ ÷µ  : void
 * µ˜”√πÿœµ  : 
 * ∆‰    À¸  : 

*****************************************************************************/
void * Get_ADC_ping_pong_buf()
{
#ifdef MALLOC_PING_PONG
		return &ping_pong_buf_ptr;			//∂ØÃ¨…Í«Î
#else
		return (void*)&ping_pong_buf_ADC;	//»´æ÷±‰¡ø
#endif

}

/*****************************************************************************
 * ∫Ø  ˝ √˚  : adc_driver_config
 * ∏∫ ‘ »À  : MZM
 * ¥¥Ω®»’∆⁄  : 2016ƒÍ6‘¬14»’
 * ∫Ø ˝π¶ƒ‹  : ∏˘æ›¥´»Î≤Œ ˝pcfg£¨≈‰÷√adc£¨≈‰÷√–Ú¡–£¡°¢£¬◊™ªª
 *  ‰»Î≤Œ ˝  : ADC_DRIVER_T *pADC  ≈‰÷√ADCºƒ¥Ê∆˜£¨∏˘æ›¥´»Î≤Œ ˝pcfgΩ¯––≈‰÷√
               ADC_CFG_T *pCfg     ≈‰÷√¥´»Î≤Œ ˝
 *  ‰≥ˆ≤Œ ˝  : Œﬁ
 * ∑µ ªÿ ÷µ  : 
 * µ˜”√πÿœµ  : 
 * ∆‰    À¸  : 

*****************************************************************************/
void adc_driver_config(ADC_DRIVER_T *pADC,ADC_CFG_T *pCfg)
{

	pADC->pREGS->CTRL = (pCfg->flagCfg & CTRL_MASK) | (pCfg->clkDiv & ADC_CR_CLKDIV_MASK);
	pADC->pREGS->SEQ_CTRL[ADC_SEQ_A] |= ((pCfg->flagSeqA & SEQ_A_MASK)) & ~(ADC_SEQ_CTRL_SEQ_ENA | ADC_SEQ_CTRL_START);
	pADC->pREGS->SEQ_CTRL[ADC_SEQ_B] |= ((pCfg->flagSeqB & SEQ_B_MASK)) & ~(ADC_SEQ_CTRL_SEQ_ENA | ADC_SEQ_CTRL_START);

}

/*****************************************************************************
 * ∫Ø  ˝ √˚  : adc_dirver_dev_init
 * ∏∫ ‘ »À  : MZM
 * ¥¥Ω®»’∆⁄  : 2016ƒÍ6‘¬14»’
 * ∫Ø ˝π¶ƒ‹  : ≥ı ºªØADCµƒºƒ¥Ê∆˜£¨ πƒ‹ADC◊™ªª
 *  ‰»Î≤Œ ˝  : Œﬁ
 *  ‰≥ˆ≤Œ ˝  : Œﬁ
 * ∑µ ªÿ ÷µ  : 
 * µ˜”√πÿœµ  : 
 * ∆‰    À¸  : 

*****************************************************************************/
void adc_dirver_dev_init()
{
	pADC = (ADC_DRIVER_T *) start_of_ram_block0;
	memset(pADC, 0, sizeof(ADC_DRIVER_T));
	
	pADC->pREGS = (ADC_REGS_T *) LPC_ADC_BASE;
	pADC->pREGS->SEQ_CTRL[0] = 0x00;
	pADC->pREGS->SEQ_CTRL[1] = 0x00;
	
#ifdef MALLOC_PING_PONG
	/*∂ØÃ¨…Í«Îø’º‰*/
	ping_pong_buf_ptr = (RTU_PING_PONG_DATA_T*) pvPortMalloc(sizeof(RTU_PING_PONG_DATA_T));

	if(ping_pong_buf_ptr == NULL)
	{
		//while(1);
	}
	else
	{
		memset(ping_pong_buf_ptr,0,sizeof(RTU_PING_PONG_DATA_T));
	}

#else
	/* »´æ÷±‰¡ø */
	ping_pong_buf_ptr = &ping_pong_buf_ADC;
#endif
	
}

/*****************************************************************************
 * ∫Ø  ˝ √˚  : adc_dirver_ch_config
 * ∏∫ ‘ »À  : MZM
 * ¥¥Ω®»’∆⁄  : 2016ƒÍ6‘¬14»’
 * ∫Ø ˝π¶ƒ‹  : ADCÕ®µ¿≈‰÷√£¨≥ı ºªØ∏˜∏ˆÕ®µ¿ºƒ¥Ê∆˜œ‡πÿ
 *  ‰»Î≤Œ ˝  : uint32_t chanNum   Õ®µ¿∫≈
               uint32_t chanOpts  Õ®µ¿≤Ÿ◊˜≤Œ ˝
 *  ‰≥ˆ≤Œ ˝  : Œﬁ
 * ∑µ ªÿ ÷µ  : 
 * µ˜”√πÿœµ  : 
 * ∆‰    À¸  : 

*****************************************************************************/
void adc_dirver_ch_config( uint32_t chanNum, uint32_t chanOpts)
{
	/* Sanity check */
	if (chanNum > ADC_MAX_CHANNEL_NUM) {
		return ;
	}

	pADC->regInt &= ~(0x18 << (chanNum * 2));

	if (chanOpts & ADC_CH_THRES_SEL1) {
		pADC->regThrSel |= 1 << chanNum;
	}
	else {
		pADC->regThrSel &= ~(1 << chanNum);
	}

	if (chanOpts & ADC_CH_THRES_CROSSING) {
		pADC->regInt |= 0x10 << (chanNum * 2);
	}
	else if (chanOpts & ADC_CH_THRES_OUTSIDE) {
		pADC->regInt |= 0x08 << (chanNum * 2);
	}


}
/* EXPORTED API: Calibrate the ADC */

/*****************************************************************************
 * ∫Ø  ˝ √˚  : adc_dirver_Calibrate
 * ∏∫ ‘ »À  : MZM
 * ¥¥Ω®»’∆⁄  : 2016ƒÍ6‘¬14»’
 * ∫Ø ˝π¶ƒ‹  : –£◊ºADC
 *  ‰»Î≤Œ ˝  : uint32_t sysclk_freq  adcµƒ ±÷”∆µ¬ 
 *  ‰≥ˆ≤Œ ˝  : Œﬁ
 * ∑µ ªÿ ÷µ  : 
 * µ˜”√πÿœµ  : 
 * ∆‰    À¸  : 

*****************************************************************************/
void adc_dirver_Calibrate( uint32_t sysclk_freq)
{
	ADC_REGS_T *pREGS = pADC->pREGS;
	volatile uint32_t i;

	pREGS->STARTUP = ADC_STARTUP_ENABLE;
	for ( i = 0; i < 0x10; i++ ) {}
	if ( !(pREGS->STARTUP & ADC_STARTUP_ENABLE) ) {
		return ;
	}

	/* If not in by-pass mode do the calibration */
	if ( (pREGS->CALIBR & ADC_CALREQD) && !(pREGS->CTRL & ADC_CR_BYPASS) ) {
		uint32_t ctrl = pREGS->CTRL & (CTRL_MASK | 0xFF);
		uint32_t tmp = ctrl;

		/* Set ADC to SYNC mode */
		tmp &= ~ADC_CR_ASYNC_MODE;

		/* To be safe run calibration at 1MHz UM permits upto 30MHz */
		if (sysclk_freq > 1000000UL) {
			pREGS->CTRL = tmp | (((sysclk_freq / 1000000UL) - 1) & 0xFF);
		}

		/* Calibration is needed, do it now. */
		pREGS->CALIBR = ADC_CALIB;
		i = 0xF0000;
		while ( (pREGS->CALIBR & ADC_CALIB) && --i ) {}
		pREGS->CTRL = ctrl;
		return ;
	}

	/* A dummy conversion cycle will be performed. */
	pREGS->STARTUP = (pREGS->STARTUP | ADC_STARTUP_INIT) & 0x03;
	i = 0x7FFFF;
	while ( (pREGS->STARTUP & ADC_STARTUP_INIT) && --i ) {}
	return ;
}


/*****************************************************************************
 * ∫Ø  ˝ √˚  : adc_driver_int_enable
 * ∏∫ ‘ »À  : MZM
 * ¥¥Ω®»’∆⁄  : 2016ƒÍ6‘¬14»’
 * ∫Ø ˝π¶ƒ‹  : adc÷–∂œ πƒ‹ –Ú¡–£¡°¢–Ú¡–£‚
 *  ‰»Î≤Œ ˝  : void  adc÷–∂œ πƒ‹
 *  ‰≥ˆ≤Œ ˝  : Œﬁ
 * ∑µ ªÿ ÷µ  : void
 * µ˜”√πÿœµ  : 
 * ∆‰    À¸  : 

*****************************************************************************/
 void adc_driver_int_enable(void)
{
//	NVIC_SetPriority(ADC_THCMP_IRQn, ISR_PRIORITY_ADC);
//	NVIC_ClearPendingIRQ(ADC_THCMP_IRQn);
//	NVIC_EnableIRQ(ADC_THCMP_IRQn);

	NVIC_SetPriority(ADC_SEQA_IRQn, ISR_PRIORITY_ADC);
	NVIC_ClearPendingIRQ(ADC_SEQA_IRQn);
	NVIC_EnableIRQ(ADC_SEQA_IRQn);

	NVIC_SetPriority(ADC_SEQB_IRQn, ISR_PRIORITY_ADC);
	NVIC_ClearPendingIRQ(ADC_SEQB_IRQn);
	NVIC_EnableIRQ(ADC_SEQB_IRQn);
}

/*****************************************************************************
 * ∫Ø  ˝ √˚  : adc_driver_start
 * ∏∫ ‘ »À  : MZM
 * ¥¥Ω®»’∆⁄  : 2016ƒÍ6‘¬14»’
 * ∫Ø ˝π¶ƒ‹  : ∆Ù∂ØÕ®µ¿◊™ªª
 *  ‰»Î≤Œ ˝  : uint8_t seqIndex  0--–Ú¡–£¡°°°°£±£≠£≠–Ú¡–£¬
 *  ‰≥ˆ≤Œ ˝  : Œﬁ
 * ∑µ ªÿ ÷µ  : void
 * µ˜”√πÿœµ  : 
 * ∆‰    À¸  : 

*****************************************************************************/
void adc_driver_StartConversion(uint8_t seqIndex)
{
	 pADC->pREGS->SEQ_CTRL[seqIndex] |= (ADC_SEQ_CTRL_SEQ_ENA | ADC_SEQ_CTRL_START);
}
 
/*****************************************************************************
 * ∫Ø  ˝ √˚  : adc_driver_init
 * ∏∫ ‘ »À  : MZM
 * ¥¥Ω®»’∆⁄  : 2016ƒÍ6‘¬14»’
 * ∫Ø ˝π¶ƒ‹  : ADC «˝∂Ø≈‰÷√
 *  ‰»Î≤Œ ˝  : Œﬁ
 *  ‰≥ˆ≤Œ ˝  : Œﬁ
 * ∑µ ªÿ ÷µ  : 
 * µ˜”√πÿœµ  : 
 * ∆‰    À¸  : 

*****************************************************************************/
void adc_driver_init()
{

	adc_dirver_dev_init();

	Chip_SYSCON_PowerUp(SYSCON_PDRUNCFG_PD_ADC0 | SYSCON_PDRUNCFG_PD_VDDA_ENA | SYSCON_PDRUNCFG_PD_VREFP);
	Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_ADC0);	
	
	adc_driver_config(pADC,&cfg);
	adc_dirver_Calibrate(Chip_Clock_GetSystemClockRate());
	adc_dirver_ch_config(8,ADC_CH_THRES_DATA);
	
	pADC->pREGS->INTEN = ADC_INTEN_SEQA_ENABLE |ADC_INTEN_SEQB_ENABLE;

	adc_driver_int_enable();
}

/*****************************************************************************
 * ∫Ø  ˝ √˚  : adc_driver_ReadData_seqA
 * ∏∫ ‘ »À  : MZM
 * ¥¥Ω®»’∆⁄  : 2016ƒÍ6‘¬14»’
 * ∫Ø ˝π¶ƒ‹  : ∂¡»°–Ú¡–£¡£‰£Â÷µ
 *  ‰»Î≤Œ ˝  : int16_t *buff  ∂¡»°–Ú¡–◊™ªªµƒ÷µ
 *  ‰≥ˆ≤Œ ˝  : Œﬁ
 * ∑µ ªÿ ÷µ  : 
 * µ˜”√πÿœµ  : 
 * ∆‰    À¸  : 

*****************************************************************************/
ErrorCode_t adc_driver_ReadData_seqA(int16_t *buff )
{
	ADC_REGS_T *pREGS = pADC->pREGS;
	uint32_t data = pREGS->SEQ_GDAT[0];

	/* If data is not vaild something is wrong! */
	if (!(data & ADC_SEQ_GDAT_DATAVALID))
	{
		*buff = 0xffff;
		return ERR_FAILED;
	}

//	data >>= 4;
	*buff = (int16_t)(data & 0xFFF0  - 0x8000);

	return LPC_OK;


//	/* Read channels having conversion data */
//	for (i = 0; i < sizeof(pREGS->DAT) / sizeof(pREGS->DAT[0]); i++) {
//		if (pADC->valSeq[seqIndex] & ADC_SEQ_CTRL_CHANSEL(i)) {
//			if (_ADC_GetData(pADC, seqIndex, pREGS->DAT[i]) != LPC_OK) {
//				return ERR_FAILED;
//			}
//		}
//	}
//	return LPC_OK;
}


/*****************************************************************************
 * ∫Ø  ˝ √˚  : adc_driver_ReadData_seqB
 * ∏∫ ‘ »À  : MZM
 * ¥¥Ω®»’∆⁄  : 2016ƒÍ6‘¬14»’
 * ∫Ø ˝π¶ƒ‹  : ∂¡»°–Ú¡–B£‰£Â÷µ
 *  ‰»Î≤Œ ˝  : int16_t *buff  ∂¡»°–Ú¡–◊™ªªµƒ÷µ
 *  ‰≥ˆ≤Œ ˝  : Œﬁ
 * ∑µ ªÿ ÷µ  : 
 * µ˜”√πÿœµ  : 
 * ∆‰    À¸  : 

*****************************************************************************/
ErrorCode_t adc_driver_ReadData_seqB(uint16_t *buff )
{
	ADC_REGS_T *pREGS = pADC->pREGS;
	uint32_t i;
	uint32_t data = pREGS->SEQ_GDAT[1]; //∂¡reg «Â÷–∂œ
	
	/* If data is not vaild something is wrong! */
	if (!(data & ADC_SEQ_GDAT_DATAVALID))
	{
//		*buff = 0xffff;
		return ERR_FAILED;
	}

//	/* Read channels having conversion data */
	for (i = 3; i < 8; i++) 
	{
		data = pREGS->DAT[i];
		if (!(data & ADC_SEQ_GDAT_DATAVALID))
		{
			*buff++ = 0xffff;
			continue;
		}
		
		data >>= 4;
		*buff++  = data;
	}
	
	
	return LPC_OK;
}

/* Initialize the ADC ROM Driver */
static int adcrom_init(void)
{
	volatile int size_in_bytes;

	//ADC_PinMuxSetup();
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

	//cfg.clkDiv = 0x01;

	/* Configure the ADC */
	ROM_ADC_Configure(hADC, &cfg);

	/* Calibrate the ADC */
	if (ROM_ADC_Calibrate(hADC, Chip_Clock_GetSystemClockRate()) != LPC_OK) {
//		DEBUGSTR("ERROR: Calibrating ADC\r\n");
		//while (1) {}
	}
//	DEBUGSTR("ADC Initialized and Calibrated successfully!\r\n");

	/* Channel configurations */
	//ROM_ADC_ConfigureCh(hADC, 0, ADC_CH_THRES_DATA | ADC_CH_THRES_SEL1 | ADC_CH_THRES_CROSSING);
	ROM_ADC_ConfigureCh(hADC, 8,  ADC_CH_THRES_DATA );
//	ROM_ADC_ConfigureCh(hADC, 1, ADC_CH_THRES_DATA);

	return 0;
}

/* Enable ADC interrupts */
static void adcrom_int_enable(void)
{
//	NVIC_SetPriority(ADC_THCMP_IRQn, ISR_PRIORITY_ADC);
//	NVIC_ClearPendingIRQ(ADC_THCMP_IRQn);
//	NVIC_EnableIRQ(ADC_THCMP_IRQn);

	NVIC_SetPriority(ADC_SEQA_IRQn, ISR_PRIORITY_ADC);
	NVIC_ClearPendingIRQ(ADC_SEQA_IRQn);
	NVIC_EnableIRQ(ADC_SEQA_IRQn);

	NVIC_SetPriority(ADC_SEQB_IRQn, ISR_PRIORITY_ADC);
	NVIC_ClearPendingIRQ(ADC_SEQB_IRQn);
	NVIC_EnableIRQ(ADC_SEQB_IRQn);
}

/* Call-Back function for ADC Start/Stop event */
static void adcrom_startstop(ADC_HANDLE_T hADC, ADC_CBINDEX_T idx, void *arg)
{

	return;
	#if 0
	signed portBASE_TYPE xadcTaskWoken = pdFALSE;

	switch (idx) {
	case ADC_START_SEQ_A:
//		rt_kprintf(DEBUG_SYS,"Sequence A Started!\r\n");
//		NVIC_EnableIRQ(ADC_SEQA_IRQn);

		break;

	case ADC_START_SEQ_B:
//		rt_kprintf(DEBUG_SYS,"Sequence B Started!\r\n");
		break;

	case ADC_STOP_SEQ_A:
//		rt_kprintf(DEBUG_SYS,"Sequence A Stopped!\r\n");
//		NVIC_DisableIRQ(ADC_SEQA_IRQn);
		//xSemaphoreGive(Sem_Adc_Deal_A);
//		xSemaphoreGiveFromISR(Sem_Adc_Deal_A,&xadcTaskWoken);
//		portYIELD_FROM_ISR(xadcTaskWoken);
	//	tmpdata = *((uint16_t*)arg);

//		SPI_Send_One_Data(1,1,buff_A[0][0]);

		break;

	case ADC_STOP_SEQ_B:
//		rt_kprintf(DEBUG_SYS,"Sequence B Stopped!\r\n");
		break;

	default:
		break;
	}
	#endif
}

/* Callback function for Buffer Done event */
static void adcrom_bufdone(ADC_HANDLE_T hADC, ADC_CBINDEX_T idx, void *arg)
{
	static uint32_t idxA, idxB,i;
	static uint32_t tmpdata=0;
	signed portBASE_TYPE xadcTaskWoken = pdFALSE;

	if (idx == ADC_BUFFER_DONE_A) 
	{
		/* Buffer A Complte */
//		pBufA = arg;
//		tmpdata = *((uint16_t*)arg);
//		rt_kprintf(0,"A[%d]\r\n",tmpdata);

//		SPI_Send_One_Data(0,1,tmpdata);
//		SPI_Send_One_Data(1,0,tmpdata);
//		SPI_Send_One_Data(1,1,tmpdata);
//		SPI_Send_One_Data(1,2,tmpdata);
//		SPI_Send_One_Data(1,1,buff_A[0][0]);
//	for(i=0;i<NUM_CHANNELS_A;i++)
//		{
//		tmpdata +=buff_A1[i];
//		}
//		SPI_Send_One_Data(1,1,tmpdata/NUM_CHANNELS_A);
//		buff_A2[A2_cnt++%96] =buff_A1[NUM_SAMPLES_A-1];
	//	SPI_Send_One_Data(1,0,buff_A1[NUM_SAMPLES_A-1]);
	
//		Board_LED_Toggle(0);
//		Board_LED_Toggle(1);
//		Board_LED_Toggle(0);
		SPI_Set_DAC_B(buff_A1[NUM_SAMPLES_A-1]);	
//		Board_LED_Toggle(0);
//		Board_LED_Toggle(1);
//		Board_LED_Toggle(0);
		
//		SPI_Set_DAC_A(buff_A1[NUM_SAMPLES_A-1]);	
//		RTU_Timer2Dev_Restart(5);
//		SPI_Set_DAC_A(buff_A1[NUM_SAMPLES_A-1]);	
//		SPI_Set_DAC_D(buff_A1[NUM_SAMPLES_A-1]);	
//		SPI_Send_Data_Buff(1,1,buff_A1,NUM_SAMPLES_A);
//		xSemaphoreGiveFromISR(Sem_Adc_Deal_A,&xadcTaskWoken);
//		portYIELD_FROM_ISR(xadcTaskWoken);
	}
	else if (idx == ADC_BUFFER_DONE_B) 
	{
		pBufB = arg;
		/* Buffer B Complete */
//		if (++idxB >= SAMPLE_COUNT_B) {
//			return;
//		}
		//xSemaphoreGive(Sem_Adc_Deal_B);
//		Rtu_Stop_ADC_B_Conversion();
		xSemaphoreGiveFromISR(Sem_Adc_Deal_B,&xadcTaskWoken);
		portYIELD_FROM_ISR(xadcTaskWoken);

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

//	for (i = 0; i < cnt; i++) {
//		DEBUGSTR("SEQ_A: ");
//		for (j = 0; j < NUM_CHANNELS_A; j++) {
//			DEBUGOUT("%03X ", buf[i][j]);
//		}
//		DEBUGSTR("\r\n");
//	}
}

/* Print content of buffer_B to UART */
 void print_buf_B(uint32_t cnt)
{
	int i, j;
	uint16_t(*buf)[NUM_CHANNELS_B] = pBufB;

	pBufB = 0;
	if (!buf) {
		return;
	}

//	for (i = 0; i < cnt; i++) {
//		DEBUGSTR("SEQ_B: ");
//		for (j = 0; j < NUM_CHANNELS_B; j++) {
//			DEBUGOUT("%03X ", buf[i][j]);
//		}
//		DEBUGSTR("\r\n");
//	}
}

/* Register ADC event Callback functions */
static void adcrom_regcb(void)
{
	ROM_ADC_RegisterCB(hADC, ADC_BUFFER_DONE_A, adcrom_bufdone);/* Buffer A Done */
	ROM_ADC_RegisterCB(hADC, ADC_BUFFER_DONE_B, adcrom_bufdone);/* Buffer B Done */
//	ROM_ADC_RegisterCB(hADC, ADC_STOP_SEQ_A, adcrom_startstop);	/* SEQ_A Stop */
//	ROM_ADC_RegisterCB(hADC, ADC_START_SEQ_A, adcrom_startstop);/* SEQ_A Start */
//	ROM_ADC_RegisterCB(hADC, ADC_STOP_SEQ_B, adcrom_startstop);	/* SEQ_B Stop */
//	ROM_ADC_RegisterCB(hADC, ADC_START_SEQ_B, adcrom_startstop);/* SEQ_B start */
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* ADC SEQ_A interrupt handler */
void ADC_SEQA_IRQHandler(void)
{
	signed portBASE_TYPE xadcTaskWoken = pdFALSE;
//	uint16_t save_flg, save_num,tmpdata;
	static uint16_t save_num=0;
	uint16_t *pbuf = (uint16_t *)ping_pong_buf_ptr->ping_pong_buf;

#ifdef rom_adc_func

	ROM_ADC_Handler(hADC, ADC_EV_SEQ_A_INT);
#else
//#ifdef MALLOC_PING_PONG

//	save_flg = ping_pong_buf_ptr->ping_pong_flg;		//pingpong ±£¥ÊŒª÷√±Í÷æ
//	save_num = ping_pong_buf_ptr->ping_pong_save_num;	//pingpong buf Œª÷√º∆ ˝∆˜

//	adc_driver_ReadData_seqA(&tmpdata);
	adc_driver_ReadData_seqA(&pbuf[save_num++]);

	if(save_num % RTU_PING_PONG_BUF_MAX_NUM == 0)		//pingª∫≥Â«¯¬˙£¨«–ªªµΩpongª∫≥Â«¯
	{
		
		ping_pong_buf_ptr->ping_pong_flg++;//«–ªª ±Í÷æ
		ping_pong_buf_ptr->ping_pong_flg &= 0x01;
		save_num %= 2 * RTU_PING_PONG_BUF_MAX_NUM;
		
//		Board_LED_Toggle(1);
		restart_dma_spi_for_adc_calc(0); //≈–∂œ «∑ÒΩ¯––dma÷ÿ∆

		xSemaphoreGiveFromISR(Sem_Adc_Deal_A,&xadcTaskWoken);
		portYIELD_FROM_ISR(xadcTaskWoken);
	}


//#else
//#endif
//	Board_LED_Toggle(0);
//	Board_LED_Toggle(1);
//	Board_LED_Toggle(0);

#endif

}

/* ADC SEQ_B interrupt handler */
void ADC_SEQB_IRQHandler(void)
{

	signed portBASE_TYPE xadcTaskWoken = pdFALSE;

#ifdef rom_adc_func

	ROM_ADC_Handler(hADC, ADC_EV_SEQ_B_INT);
#else
	adc_driver_ReadData_seqB((uint16_t *)&rtu_adc_seq_b);

	xSemaphoreGiveFromISR(Sem_Adc_Deal_B,&xadcTaskWoken);
	portYIELD_FROM_ISR(xadcTaskWoken);

#endif
}

/* ADC Overflow or THCMP handler */
void ADC_THCMP_IRQHandler(void) {
	ROM_ADC_Handler(hADC, ADC_EV_OVR_INT);
}


/*****************************************************************************
 * ∫Ø  ˝ √˚  : RTU_AdcDev_Init
 * ∏∫ ‘ »À  : MZM
 * ¥¥Ω®»’∆⁄  : 2016ƒÍ3‘¬2»’
 * ∫Ø ˝π¶ƒ‹  : ADC”≤º˛≥ı ºªØ
 *  ‰»Î≤Œ ˝  : void  wu 
 *  ‰≥ˆ≤Œ ˝  : Œﬁ
 * ∑µ ªÿ ÷µ  : 
 * µ˜”√πÿœµ  : 
 * ∆‰    À¸  : 

*****************************************************************************/
void RTU_AdcDev_Init(void)
{
	int i = 1;

#ifndef ROM_ADC_FUNC
	adc_driver_init();

#else
	/* Initialize the PinMux and setup the memory for ROM driver */
	adcrom_init();

	/* Configure the ADC */
	adcrom_config();

	adcrom_regcb();	/* Register call-back functions */

	adcrom_int_enable();/* Enable interrupts */

#endif
	return ;
}


void restart_adc_init()
{
//	adcrom_config();
//ADC_CFG_T cfg = {
//		ADC_SEQ_A_CONFIG,
//		ADC_SEQ_B_CONFIG,
//		ADC_CONFIG,
//		0x1	/* Divider will be calculated during run time */
//	};

//	NVIC_SetPriority(ADC_SEQA_IRQn, 0x01);
//	NVIC_ClearPendingIRQ(ADC_SEQA_IRQn);
//	NVIC_EnableIRQ(ADC_SEQA_IRQn);
// 	RTU_AdcDev_Init();

#if 0
	adcrom_init();
/* Configure the ADC */

	
	//	cfg.clkDiv = 0xFF;
	
		/* Configure the ADC */
		ROM_ADC_Configure(hADC, &cfg);
	
		/* Calibrate the ADC */
		if (ROM_ADC_Calibrate(hADC, Chip_Clock_GetSystemClockRate()) != LPC_OK)
		{
			//DEBUGSTR("ERROR: Calibrating ADC\r\n");
			//while (1) {}
		}
//		DEBUGSTR("ADC Initialized and Calibrated successfully!\r\n");
	
		/* Channel configurations */
//		//ROM_ADC_ConfigureCh(hADC, 0, ADC_CH_THRES_DATA | ADC_CH_THRES_SEL1 | ADC_CH_THRES_CROSSING);
//		ROM_ADC_ConfigureCh(hADC, 8, ADC_CH_THRES_DATA);
//		ROM_ADC_ConfigureCh(hADC, 1, ADC_CH_THRES_DATA);

	adcrom_regcb();	/* Register call-back functions */

	adcrom_int_enable();/* Enable interrupts */

#endif
}

uint8_t Rtu_Start_ADC_A_Conversion()
{
	uint8_t i;
	//return ROM_ADC_StartConversion(hADC, ADC_SEQ_A, &buff_A1, (NUM_SAMPLES_A >> 1) * NUM_CHANNELS_A);
	//RTU_AdcDev_Init();
//	restart_adc_init();
//	buff_A[0][0] = 0;
	//buff_A1[0] = 0;
//	for(i=0;i<NUM_SAMPLES_A;i++)
//	{
//		buff_A1[i]=0;
//	}
//	Board_LED_Toggle(0);
//	Board_LED_Toggle(1);
//	Board_LED_Toggle(0);
#ifdef ROM_ADC_FUNC
	return ROM_ADC_StartConversion(hADC, ADC_SEQ_A, buff_A1, 1);
#else
	adc_driver_StartConversion(ADC_SEQ_A);
	return 0;
#endif

}
uint8_t Rtu_Stop_ADC_A_Conversion()
{
	return ROM_ADC_StopConversion(hADC, ADC_SEQ_A);
}

uint8_t Rtu_Start_ADC_B_Conversion()
{
#ifdef ROM_ADC_FUNC
		return ROM_ADC_StartConversion(hADC, ADC_SEQ_B, &buff_B[0][0], (NUM_SAMPLES_B >> 1) * NUM_CHANNELS_B);
#else
		adc_driver_StartConversion(ADC_SEQ_B);
		return 0;
#endif

}
uint8_t Rtu_Stop_ADC_B_Conversion()
{
	return ROM_ADC_StopConversion(hADC, ADC_SEQ_B);
}

//int m11rain(void)
//{
//	int i = 1;
//	/* Generic Initialization */
//	SystemCoreClockUpdate();
//	Board_Init();

//	/* Initialize the PinMux and setup the memory for ROM driver */
//	if (adcrom_init()) {
//		return 1;
//	}

//	/* Configure the ADC */
//	if (adcrom_config()) {
//		return 1;
//	}

//	adcrom_regcb();	/* Register call-back functions */

//	adcrom_int_enable();/* Enable interrupts */

//	if (ROM_ADC_StartConversion(hADC, ADC_SEQ_B, &buff_B[0][0], (NUM_SAMPLES_B >> 1) * NUM_CHANNELS_B) != LPC_OK) {
//		DEBUGSTR("ERROR: Starting conversion in SEQ_B\r\n");
//	}

//	/* Fill the first half of buffer with conversion samples */
//	if (ROM_ADC_StartConversion(hADC, ADC_SEQ_A, &buff_A[0][0], (NUM_SAMPLES_A >> 1) * NUM_CHANNELS_A) != LPC_OK) {
//		DEBUGSTR("ERROR: Starting conversion in SEQ_A\r\n");
//	}

//	while (i) {
//		print_buf_A(1);	/* Print first sample from the buffer */
//		print_buf_B(1);	/* Print first sample from the buffer */
//		__WFI();
//	}
//	return 0;
//}
