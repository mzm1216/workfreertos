#ifndef __sky72310_h
#define __sky72310_h

#ifdef  Sky72310_GLOBALS
#define sky72310_ext
#else
#define sky72310_ext  extern
#endif

#include "board.h"
#include "config.h"

#define REF_PLL_REF_FREQ		(10000000)			//10M 
#define REF_PLL_FREQ			(19200000)			//19.2M
#define LO2_PLL_FREQ			(73350000-450000)  	//73.35M-450K

#define REF_PLL_PD_FREQ		5000  	// �ο�Ƶ�� �ļ���Ƶ�� Ϊ5K 
#define LO2_PLL_PD_FREQ     10000 	//  Lo2Ƶ�� �ļ���Ƶ��Ϊ 10K

#define RX_PLL_SELECT 	0	//����Ƶ�����໷ ѡ��
#define LO2_PLL_SELECT	1	//����LO2Ƶ�����໷ ѡ��
#define REF_PLL_SELECT	2	//�ο�Ƶ�����໷ ѡ��
#define TX_PLL_SELECT	3	//����Ƶ�����໷  ѡ��


#define TX_ENABLE 			RTU_PS_TX(1);
#define TX_DISABLE 		RTU_PS_TX(0);


sky72310_ext void Delay_ms(uint8_t dly);

sky72310_ext void I3C_Set_CS_high(uint8_t Chip);//chip : 0 - IF, 1 - TX, 2 - RX
sky72310_ext void I3C_Set_CS_low(uint8_t Chip);//chip : 0 - IF, 1 - TX, 2 - RX
sky72310_ext void I3C_Set_CLK_high(uint8_t Chip);
sky72310_ext void I3C_Set_CLK_low(uint8_t Chip);
sky72310_ext void I3C_Set_Data_high(uint8_t Chip);
sky72310_ext void I3C_Set_Data_low(uint8_t Chip);

sky72310_ext void Write_SKY72310_ADF4001_Data(uint8_t Chip,uint8_t BitLen,uint32_t ulD4113);
sky72310_ext void Set_SKY72310_FL(uint8_t chip, uint32_t ulFreq, uint32_t ulReferenece, uint32_t rvalue, uint8_t doubler, uint32_t ref_divider, uint8_t polar, uint8_t NoiseAndSpurMode, uint32_t mod_value,uint32_t ChcSpaceFreq);
sky72310_ext void Set_SKY72310_LD(uint8_t chip, uint32_t ulFreq, uint32_t ulReferenece, uint32_t rvalue, uint8_t doubler, uint32_t ref_divider, uint32_t Ps_Ld, uint32_t Pdg, uint32_t mod_value,uint32_t ChcSpaceFreq);

sky72310_ext  uint16_t  tmpreg_72310[2][9];

#endif
