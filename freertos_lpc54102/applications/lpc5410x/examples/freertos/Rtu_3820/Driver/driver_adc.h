#ifndef _DRIVER_ADC_h
#define _DRIVER_ADC_h

#ifdef  ADC_GLOBALS
#define adc_ext
#else
#define adc_ext  extern
#endif

#include "board.h"
#include "config.h"

#if 0

/* Get 32 samples for 1 channels CH8 */
#define NUM_SAMPLES_A    1
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
	TRIG_SOFT |	/* Software trigger for SEQ_A () */ \
	TRIG_POL_POS |	/* Use SW1 as the trigger for SEQ_A */ \
	MODE_EOC |		/* Event generated after conversion is done */ \
//	MODE_SINGLE |		/* Event generated after Sequence done */ \
	ENABLE_CH(3) 	/* Associate channels 8 to SEQ_A */

/* SEQ_B enables channels 1, 5, 6 and 7; Uses PININT1 as trigger and works on Burst mode */
#define ADC_SEQ_B_CONFIG  \
	TRIG_SOFT |		/* Software trigger for SEQ_B */ \
	TRIG_POL_POS |	/* UM recommends this for S/W trigger */ \
	MODE_EOC |		/* Event generated after conversion is done */ \
	MODE_SINGLE |	/* Generate DMA Trigger for every channel read */ \
	ENABLE_CH(3) | ENABLE_CH(4)| ENABLE_CH(5) | ENABLE_CH(6) | ENABLE_CH(7)	/* Associate channels 3 4, 5, 6 & 7 to SEQ_B */

/* General ADC configuration */
#define ADC_CONFIG \
	MODE_SYNC |	/* Enable synchronous mode */ \
	RESOL_12BIT |	/* Use 12-Bit resolution */	\
	SAMPLE_TIME(0)	/* No extra clocks */
#else
	
	/*****************************************************************************
	 * Private types/enumerations/variables
	 ****************************************************************************/
	/* Get 32 samples for 3 channels CH0, CH3 and CH4 */
#define CFG_DELAY		0x1

#define NUM_SAMPLES_A    	1
#define NUM_CHANNELS_A    1
#define SAMPLE_COUNT_A    1/* Run 16 times */
	
	/* Get 64 samples for 4 channels CH1, CH5, CH7 and CH10 */
#define NUM_SAMPLES_B    16
#define NUM_CHANNELS_B    5
#define SAMPLE_COUNT_B    16/* Run 32 times */
#define DATA_32768_DEF		32768

#if 0
	/* SEQ_A enables channels 0, 3 and 4; Uses software trigger; doesn't use BURST MODE_SINGLE*/
#define ADC_SEQ_A_CONFIG  \
		TRIG_SOFT | 	/* Software trigger for SEQ_A */ \
		TRIG_POL_POS |	/* UM recommends this for S/W trigger */ \
		MODE_EOS |		/* Event generated after Sequence done */ \
		ENABLE_CH(8) 	/* Associate channels 0, 2 and 3 to SEQ_A */
	//ENABLE_CH(0) | ENABLE_CH(3) | ENABLE_CH(4)	/* Associate channels 0, 2 and 3 to SEQ_A */
	
	/* SEQ_B enables channels 1, 5, 7 and 10; Uses PININT1 as trigger and works on Burst mode */
#define ADC_SEQ_B_CONFIG  \
		TRIG_SOFT  |	/* Hardware trigger for SEQ_B (PININT1) */ \
		TRIG_POL_POS |	/* Use SW1 as the trigger for SEQ_B */ \
		MODE_EOS |		/* Event generated after Sequence done */ \
		ENABLE_CH(3) | ENABLE_CH(4) | ENABLE_CH(5) | ENABLE_CH(6)| ENABLE_CH(7)	/* Associate channels 1, 5, 7 & 10 to SEQ_B */
		
#define ADC_CONFIG \
			MODE_ASYNC | /* Enable synchronous mode */ \
			RESOL_12BIT |	/* Use 12-Bit resolution */ \
			SAMPLE_TIME(1)	/* No extra clocks */
	#else
	
#define ADC_SEQ_A_CONFIG  (TRIG_SOFT| ENABLE_CH(8))
#define ADC_SEQ_B_CONFIG  (TRIG_SOFT |MODE_EOC |TRIG_POL_POS| ENABLE_CH(3) | ENABLE_CH(4) | ENABLE_CH(5) | ENABLE_CH(6)| ENABLE_CH(7) )
#define ADC_CONFIG			(MODE_SYNC |RESOL_12BIT |SAMPLE_TIME(0))

	#endif
	/* General ADC configuration */

#endif
/* Buffer pointers */
adc_ext uint16_t A2_cnt;	/* Memory to hold samples for SEQ_A */
adc_ext uint16_t buff_A1[NUM_SAMPLES_A];	/* Memory to hold samples for SEQ_A */
adc_ext uint16_t buff_A[NUM_SAMPLES_A][NUM_CHANNELS_A];	/* Memory to hold samples for SEQ_A */
adc_ext uint32_t buff_B[NUM_SAMPLES_B][NUM_CHANNELS_B];	/* Memory to hold samples for SEQ_B */
adc_ext uint16_t(*volatile pBufA)[NUM_CHANNELS_A];	/* Pointer to current active SEQ_A buffer */
adc_ext uint16_t(*volatile pBufB)[NUM_CHANNELS_B];	/* Pointer to current active SEQ_B buffer */


//adc_ext uint16_t adc_ping_pong_buf[2][1000];	/* ADC pingpangbuf */
adc_ext uint32_t buff_B[NUM_SAMPLES_B][NUM_CHANNELS_B];	/* Memory to hold samples for SEQ_B */


/* DMA Queues */
adc_ext ROM_DMA_QUEUE_T seqAQueue, seqBQueue;

/* ADC Driver context memory */
#define RAMBLOCK_H          60
adc_ext uint32_t  start_of_ram_block0[RAMBLOCK_H];

/* ADC ROM Driver Handle */
adc_ext ADC_HANDLE_T *hADC;
adc_ext ROM_DMA_HANDLE_T *hDMA;

/* DMA Descriptor table memory */
//extern ALIGN(512) ROM_DMA_DESC_T EXChip_DMA_Table[MAX_DMA_CHANNEL];
adc_ext ALIGN(16) ROM_DMA_DESC_T dmaDescA[NUM_CHANNELS_A];
adc_ext ALIGN(16) ROM_DMA_DESC_T dmaDescB[NUM_CHANNELS_B];
adc_ext uint32_t idxA;
adc_ext uint32_t idxB;

//static  ROM_DMA_INIT_T dmaInit = {
//	0,
//	LPC_DMA_BASE,
//	0
//};

//static ROM_DMA_CHAN_CFG_T chanCfgA = {
//	0,											/* Use peripheral DMA request */
//	1,											/* Hardware trigger polarity high */
//	0,											/* Hardware trigger edge triggered */
//	1,											/* Burst transfer on each trigger */
//	ROM_DMA_BURSTPOWER_2,						/* Burst size of 1 datum */
//	0,											/* Disable source burst wrap */
//	0,											/* Disable destination burst wrap */
//	2,											/* Channel priority = 0 (highest) */
//	0,											/* reserved */
//};
//static ROM_DMA_CHAN_CFG_T chanCfgB = {
//	0,											/* Use peripheral DMA request */
//	1,											/* Hardware trigger polarity high */
//	0,											/* Hardware trigger edge triggered */
//	1,											/* Burst transfer on each trigger */
//	ROM_DMA_BURSTPOWER_1,						/* Burst size of 1 datum */
//	0,											/* Disable source burst wrap */
//	0,											/* Disable destination burst wrap */
//	1,											/* Channel priority = 0 (highest) */
//	0,											/* reserved */
//};
adc_ext uint8_t Rtu_Stop_ADC_B_Conversion(void);

typedef struct
{
	uint16_t CH_3_CV;			
	uint16_t CH_4_RSSI;			//场强
	uint16_t CH_5_PV_FD;		//前向功率
	uint16_t CH_6_PV_REF;		//反向功率
	uint16_t CH_7_LO_BAT;		//供电电压
}RTU_ADC_SEQ_B_T;

adc_ext RTU_ADC_SEQ_B_T  rtu_adc_seq_b;

#endif
