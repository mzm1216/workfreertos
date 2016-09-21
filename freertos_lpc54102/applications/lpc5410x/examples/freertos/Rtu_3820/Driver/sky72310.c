/***********************************************************************************
 * �� �� ��   : sky72310.c
 * �� �� ��   : MZM
 * ��������   : 2016��2��25��
 * �ļ�����   : IO ��ģ������SPI�Ĳ����ӿڣ�����CS��CLK��DATA3���ߣ�3������
                ���ĵĲ��ݽӿڣ�����RX PLL��sky72310оƬ��LO2��REF���໷����
                ��ADF4001оƬ�����ļ��ṩ������оƬ�ĳ�ʼ���Ͳ����ӿ�
                
 * ��Ȩ˵��   : Copyright (c) 2008-2016   �������������ͨѶ�Ƽ����޹�˾
 * ��    ��   : 
 * �޸���־   : 
***********************************************************************************/

#define	Sky72310_GLOBALS
#include "ioconfig.h"
#include "sky72310.h"



// 3��spi�� Ƭѡ�ź��� �˿ں͹ܽ�   0--RX_pll 	1--LO2_Pll  2--REF_Pll  3--TX_PLL
static uint8_t three_spi_cs_port[] =
{
	PLL_RX_SEL_PORT,
	PLL_RX_SEL2_PORT,
	PLL_REF_SEL_PORT,
	PLL_TX_SEL_PORT
};
static uint8_t three_spi_cs_pin[]=
{
	PLL_RX_SEL_PIN,
	PLL_RX_SEL2_PIN,
	PLL_REF_SEL_PIN,
	PLL_TX_SEL_PIN

};

// 3��spi�� ������ �˿ں͹ܽ� ����չΪ3·
static uint8_t three_spi_clk_port[]=
{
	PLL_RX_SCK_PORT,
	PLL_RX_SCK_PORT,
	PLL_RX_SCK_PORT,
	PLL_TX_SCK_PORT
};
static uint8_t three_spi_clk_pin[]=
{
	PLL_RX_SCK_PIN,
	PLL_RX_SCK_PIN,
	PLL_RX_SCK_PIN,
	PLL_TX_SCK_PIN
};


// 3��spi�� ʱ���� �˿ں͹ܽ� ����չ
static uint8_t three_spi_data_port[]=
{
	PLL_RX_DATA_PORT,
	PLL_RX_DATA_PORT,
	PLL_RX_DATA_PORT,
	PLL_TX_DATA_PORT
};
static uint8_t three_spi_data_pin[]=
{
	PLL_RX_DATA_PIN,
	PLL_RX_DATA_PIN,
	PLL_RX_DATA_PIN,
	PLL_TX_DATA_PIN
};

void spi0_io_init()
{
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, three_spi_cs_port[TX_PLL_SELECT],three_spi_cs_pin[TX_PLL_SELECT] );
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, three_spi_data_port[TX_PLL_SELECT],three_spi_data_pin[TX_PLL_SELECT] );
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, three_spi_clk_port[TX_PLL_SELECT],three_spi_clk_pin[TX_PLL_SELECT] );

}
void I3c_GPIO_init()
{
	uint32_t i;

	for(i=0;i<I3C_CH_NUM_OF_SPI;i++)
	{
		Chip_GPIO_SetPinDIROutput(LPC_GPIO, three_spi_cs_port[i],three_spi_cs_pin[i] );
		Chip_GPIO_SetPinDIROutput(LPC_GPIO, three_spi_data_port[i],three_spi_data_pin[i] );
		Chip_GPIO_SetPinDIROutput(LPC_GPIO, three_spi_clk_port[i],three_spi_clk_pin[i] );
	}
	
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0,PS_TX_PIN );
//	Chip_GPIO_SetPinDIROutput
}

/*****************************************************************************
 * �� �� ��  : I3C_Set_CS_high
 * �� �� ��  : MZM
 * ��������  : 2016��2��25��
 * ��������  : �������໷��Ƭѡ�ź�Ϊ��
 * �������  : uint8_t chip  �������໷ѡ��---0=�������໷��1=����LO2,2=�ο�
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void I3C_Set_CS_high(uint8_t chip)
{
	Chip_GPIO_SetPinState(LPC_GPIO, three_spi_cs_port[chip], three_spi_cs_pin[chip], (bool)1);
	return;
}

/*****************************************************************************
 * �� �� ��  : I3C_Set_CS_low
 * �� �� ��  : MZM
 * ��������  : 2016��2��25��
 * ��������  : �������໷��Ƭѡ�ź�Ϊ ��
 * �������  : uint8_t chip  �������໷ѡ��---0=�������໷��1=����LO2,2=�ο�
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void I3C_Set_CS_low(uint8_t chip)//chip : 0 - IF, 1 - TX, 2 - RX
{
	Chip_GPIO_SetPinState(LPC_GPIO, three_spi_cs_port[chip], three_spi_cs_pin[chip], (bool)0);
	return;
	
}

/*****************************************************************************
 * �� �� ��  : I3C_Set_CLK_high
 * �� �� ��  : MZM
 * ��������  : 2016��2��25��
 * ��������  :  �������໷�� ʱ�� Ϊ ��
 * �������  : uint8_t chip  �������໷ѡ��---0=�������໷��1=����LO2,2=�ο�
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void I3C_Set_CLK_high(uint8_t chip)
{
	Chip_GPIO_SetPinState(LPC_GPIO, three_spi_clk_port[chip], three_spi_clk_pin[chip], (bool)1);
	return;
}

/*****************************************************************************
 * �� �� ��  : I3C_Set_CLK_LOW
 * �� �� ��  : MZM
 * ��������  : 2016��2��25��
 * ��������  :  �������໷�� ʱ�� Ϊ ��
 * �������  : uint8_t chip  �������໷ѡ��---0=�������໷��1=����LO2,2=�ο�
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void I3C_Set_CLK_low(uint8_t chip)
{
	Chip_GPIO_SetPinState(LPC_GPIO, three_spi_clk_port[chip], three_spi_clk_pin[chip], (bool)0);
	return;

}

/*****************************************************************************
 * �� �� ��  : I3C_Set_Data_high
 * �� �� ��  : MZM
 * ��������  : 2016��2��25��
 * ��������  :  �������໷�� ������ ��
 * �������  : uint8_t chip  �������໷ѡ��---0=�������໷��1=����LO2,2=�ο�
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/

void I3C_Set_Data_high(uint8_t chip)
{
	Chip_GPIO_SetPinState(LPC_GPIO, three_spi_data_port[chip], three_spi_data_pin[chip], (bool)1);
	return;
}

/*****************************************************************************
 * �� �� ��  : I3C_Set_Data_low
 * �� �� ��  : MZM
 * ��������  : 2016��2��25��
 * ��������  :  �������໷�� ������ ��
 * �������  : uint8_t chip  �������໷ѡ��---0=�������໷��1=����LO2,2=�ο�
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void I3C_Set_Data_low(uint8_t chip)
{
	Chip_GPIO_SetPinState(LPC_GPIO, three_spi_data_port[chip], three_spi_data_pin[chip], (bool)0);
	return;
}


/*****************************************************************************
 * �� �� ��  : Delay_ms
 * �� �� ��  : MZM
 * ��������  : 2016��2��25��
 * ��������  : ������ʱ
 * �������  : uint8_t dly  ��ʱ��С
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void Delay_ms(uint8_t dly)
{
    uint32_t i;
    
    for (; dly > 0; dly--)
        for (i = 0; i < 0xfff; i++);
}


/*****************************************************************************
 * �� �� ��  : Write_SKY72310_ADF4001_Data
 * �� �� ��  : MZM
 * ��������  : 2016��2��25��
 * ��������  : IO��ģ������spi����ָ�����໷�����ض����ȵ����ݡ�
 * �������  : uint8_t Chip      PLL���ͣ�0==���գ�1==����LO2, 2==�ο�
               uint8_t BitLen    ����bit����
               uint32_t regdata  ��������
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void Write_SKY72310_ADF4001_Data(uint8_t Chip,uint8_t BitLen,uint32_t regdata)//chip : 0 - IF, 1 - TX, 2 - RX;BitLen: write bitlen;ulD4113:write to reg data
{//��Ҫ���ó�16λ���ݣ�4λ��ַλ��12λ����λ
	int32_t i;
	uint32_t temp;
	    
	temp=0x00000001;
	temp = temp << (BitLen -1);

//   	I3C_Set_CS_high(Chip);
//	I3C_Set_Data_high(Chip);
//	I3C_Set_CLK_low(Chip);
	//for (j = 0; j < 2; j++);
	
	I3C_Set_CS_low(Chip);//cs low
	//for (j = 0; j < 2; j++);
    
	for (i = 0; i < BitLen; i++)
	{
		I3C_Set_CLK_low(Chip);             //CLOCK����
		if (regdata & temp) //0x00800000)
		{
			I3C_Set_Data_high(Chip);//data high
		}
		else 
		{
			I3C_Set_Data_low(Chip);//data low
		}

		regdata <<= 1;
		Delay_ms(1);
		I3C_Set_CLK_high(Chip);
		Delay_ms(1);

	}
    
	I3C_Set_CLK_low(Chip);
//	I3C_Set_Data_high(Chip);
	I3C_Set_CS_high(Chip);
	
	Delay_ms(50);
	return;                                                                                
}


void Set_sky72310_mod_reg(uint16_t modvalue)
{
	uint32_t MD_R=0;

	
	MD_R=MD_R+((9)<<12);

	MD_R |= (modvalue&0xFFF);
	Write_SKY72310_ADF4001_Data(TX_PLL_SELECT, 16, MD_R);
}

/*****************************************************************************
 * �� �� ��  : freq_calc_fix
 * �� �� ��  : MZM
 * ��������  : 2016��6��23��
 * ��������  : ͨ������Ƶ�ʺͼ���Ƶ�� ���� 72310оƬ�������Ĵ�����С������MSB
               ��LSB��ֵ
 * �������  : int32_t fvco      ����Ƶ��
               int32_t fpd       ����Ƶ��
               uint16_t *nreg    ��������ָ�� 
               uint16_t *ndiv_m  ����С������MSB 
               uint16_t *ndiv_l  ����С������LSB
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
int	freq_calc_fix(int32_t fvco, int32_t fpd,uint16_t *nreg, uint16_t *ndiv_m, uint16_t *ndiv_l)
{
	int64_t delta;
	uint32_t div;
	uint16_t nr;

	/* Ƶ����Ч���ж� */
	if (fvco < 0)
		fvco = 0 - fvco;
	if (fpd < 0)
		fpd = 0 - fpd;

	/* ���� ����  С��������������*/
	nr = (fvco + (fpd >> 1)) / fpd;
	*nreg = nr - 32;
	
	/* ���� С����������2^18   */
	delta = (int64_t)(1 << 18) * (fvco - nr * fpd);

	/* �������  ���ϰ������Ƶ��  
		����  ��ȥ�������Ƶ�� */
	if (delta < 0)
		delta -= (fpd >> 1);
	else
		delta += (fpd >> 1);
	
	/*  ����  С������ MSB LSB*/
	div = (uint32_t)(delta / fpd) & 0x3ffff;

	*ndiv_m = div >> 8;
	*ndiv_l = div & 0xff;

	return 0;
}

//����	
//Set_SKY72310_FL(1,361000000,19200000,  0,0,0,0,0,0,0)
//Set_ADF4153(1, 364025000, 10000000, 1, 1, 0, 0, 2,4091);//LOCK
//����оƬ,		PLL_fre(Hz), 		REF_fre,			,				,			ref_divider�ķ�Ƶλ,��������(1)����ָʾ(0),Pdg�õ���(0~31),

/*****************************************************************************
 * �� �� ��  : Set_SKY72310_LD
 * �� �� ��  : MZM
 * ��������  : 2016��2��25��
 * ��������  : ����SKY72310оƬ���������Ƶ��
 * �������  : uint8_t chip           PLL���ͣ�0==���գ�1==LO2,2==�ο�
               uint32_t ulFreq        �����õ���Ƶ��
               uint32_t ulReferenece  �ο�Ƶ��
               uint32_t rvalue        rvalue
               uint8_t doubler        doubule
               uint32_t ref_divider   ref-divider��Ƶλ
               uint32_t Ps_Ld         ��������
               uint32_t Pdg           ����ָʾ
               uint32_t mod_value     
               uint32_t ChcSpaceFreq  
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void Set_SKY72310_LD(uint8_t chip, uint32_t ulFreq, uint32_t ulReferenece, uint32_t rvalue, uint8_t doubler, uint32_t ref_divider, uint32_t Ps_Ld, uint32_t Pdg, uint32_t mod_value,uint32_t ChcSpaceFreq)
{
	volatile uint16_t 	nreg,n_msb,n_lsb;
	 uint32_t tmpflg;
	 uint32_t	Fdiv_ref=0;

	if(chip==RX_PLL_SELECT)
	{
		ulFreq=ulFreq-73350000;
		tmpflg = 0;
	}
	else
	{
		ulFreq=ulFreq;
		tmpflg = 1;
	}

	Fdiv_ref=(fp64_t)(ulReferenece/(ref_divider+1));// 19200000 4��ƵΪ ����Ƶ��
	freq_calc_fix(ulFreq,Fdiv_ref,&nreg,&n_msb,&n_lsb);//���� ������С��msb��С��lsb

	if(chip==RX_PLL_SELECT || chip==TX_PLL_SELECT)
	{
		/*tmpreg_72310 ������*/
		tmpreg_72310[tmpflg][0] = 0x0000 | nreg;
		tmpreg_72310[tmpflg][1] = 0x1000 | n_msb;
		tmpreg_72310[tmpflg][2] =  0x2000 | n_lsb;
		
		Write_SKY72310_ADF4001_Data(chip, 16, 0x5000 | ref_divider);
		Write_SKY72310_ADF4001_Data(chip, 16, 0x6000 | ( Ps_Ld << 5 ) | ( Pdg << 0 ));	// 6 ��λ���bit5 =0 ����ʧ�� bit0-bit4 =F);
		Write_SKY72310_ADF4001_Data(chip, 16, 0x7000);
		Write_SKY72310_ADF4001_Data(chip, 16, 0x8000);
		Write_SKY72310_ADF4001_Data(chip, 16, 0x9000);
		
		Write_SKY72310_ADF4001_Data(chip, 16, 0x0000 | nreg);			 // ��������
		Write_SKY72310_ADF4001_Data(chip, 16, 0x2000 | n_lsb);			//С�����ֵ�LSB -- 8bit--Note����˳��: Ӧ�ȷ���LSB  �ڷ���MSB
		Write_SKY72310_ADF4001_Data(chip, 16, 0x1000 | n_msb);		//С�����ֵ�MSB --10bit
 	   }
	else
	{
	}
}

/*****************************************************************************
 * �� �� ��  : Get_GCD
 * �� �� ��  : MZM
 * ��������  : 2016��2��25��
 * ��������  : շת�����---�õ��������� ���Լ����
 				��շת������󼸸��������Լ�����������������������������
 				���Լ��������������Լ����������������Լ����
 				��������ȥ��ֱ�����һ����Ϊֹ��������õ��Ǹ����Լ����
 				����������Щ�������Լ��
 * �������  : uint32_t ref  �ο�Ƶ��
               uint32_t vco  �����õ�Ƶ��
 * �������  : ��
 * �� �� ֵ  : ���Լ��
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
uint32_t Get_GCD(uint32_t ref,uint32_t vco)
{
	uint32_t max,min,tmp;
	
	if(ref<vco)//��ȡ�ϴ�ֵ
	{
		max=vco;
		min=ref;
	}
	else
	{
		max=ref;
		min=vco;
	}
	tmp = max%min;
	while(1)//������Ϊ0 δ�ҵ����Լ��
	{
		tmp=max%min;
		if(tmp==0)
		{
			break;
		}
		else
		{
			max=min;
			min=tmp;
		}
	}
	return min;//�������Լ��
}


//uint8_t calculator_R_N(uint32_t ulFreq, uint32_t ulReferenece)
//{	
//}

/*****************************************************************************
 * �� �� ��  : ADF4001_Config
 * �� �� ��  : MZM
 * ��������  : 2016��2��25��
 * ��������  : ��R��N���õ�ADF4001��
 * �������  : uint8_t chip                  PLL ���ͣ�0==���գ�1==LO2,2==REF
               unsigned long int REF_divide  R��������ֵ
               unsigned long int N_divide    N��������ֵ
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void ADF4001_Config(uint8_t chip,unsigned long int REF_divide,unsigned long int N_divide)
{
	unsigned long int r,n;
	
	Write_SKY72310_ADF4001_Data(chip, 24, 0x1Df893);
	Delay_ms(20);
	
	r = (REF_divide&0x3FFF)<<2; // R1-R14
	Write_SKY72310_ADF4001_Data(chip, 24, 0x000000|r);//����R 
	Delay_ms(20);
	
	n = (N_divide&0x1FFF)<<8;// N1--N13
	Write_SKY72310_ADF4001_Data(chip, 24, 0x000001|n);//����N
	Delay_ms(20);
	
	Write_SKY72310_ADF4001_Data(chip, 24, 0x1Df892);
}

/*****************************************************************************
 * �� �� ��  : Set_ADF4001_Freq
 * �� �� ��  : MZM
 * ��������  : 2016��2��25��
 * ��������  : ��������Ƶ�ʺͲο�Ƶ�� �����R��Nֵ�������õ�adf4001�ļĴ���
               ��
 * �������  : uint8_t chip           PLL ����:0==j���գ�1==LO2��2==REF
               uint32_t ulFreq        ����Ƶ��
               uint32_t ulReferenece  �ο�Ƶ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void Set_ADF4001_Freq(uint8_t chip,uint32_t ulFreq, uint32_t ulReferenece)
{
	uint32_t tmppdfreq;
	uint32_t Rreg, Nreg;
	
	if(chip == REF_PLL_SELECT)
	{
		/*10M ����19.2M ����ָ��-�ο�Ƶ�� ����ָ������Ƶ��Ϊ 5K��
		���Թ̶�R=3840,n=2000 2016��5��20��16:30:08*/
		tmppdfreq = REF_PLL_PD_FREQ;	//�ο�����Ƶ��  5 K 
	}
	else if(chip == LO2_PLL_SELECT)		//PLLоƬ������  ������?
	{
		tmppdfreq = LO2_PLL_PD_FREQ;	//������ ����Ƶ��Ϊ10 K
	}
	else
	{
//		tmpgcd = Get_GCD(ulFreq,ulReferenece);
//		Rreg=ulReferenece/tmpgcd;
//		Nreg=ulFreq/tmpgcd;
	}
	
	Rreg=ulReferenece/tmppdfreq;  	//���� Ref counter--14bit
	Nreg=ulFreq/tmppdfreq;			//����  N counter  13bit
	
	ADF4001_Config(chip,Rreg,Nreg);
	
	return ;
}

/*****************************************************************************
 * �� �� ��  : Set_PLL_Freq
 * �� �� ��  : MZM
 * ��������  : 2016��2��25��
 * ��������  : ����sky72310����adf4001���õ�������Ƶ��
 * �������  : uint8_t chip           PLL���ͣ�0==���գ�1==LO2, 2==�ο�
               uint32_t ulFreq        �����õ���Ƶ��
               uint32_t ulReferenece  �ο�Ƶ��
               uint32_t rvalue        rvalue
               uint8_t doubler        doubule
               uint32_t ref_divider   ref-divider��Ƶλ
               uint32_t Ps_Ld         ��������
               uint32_t Pdg           ����ָʾ
               uint32_t mod_value     
               uint32_t ChcSpaceFreq  

 * �������  : TRUE �����ɹ�  FALSE  ����ʧ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 
//Set_SKY72310_FL(1,361000000,19200000,  0,0,0,0,0,0,0)

*****************************************************************************/
uint8_t  Set_PLL_Freq(uint8_t chip, uint32_t ulFreq, uint32_t ulReferenece, uint32_t rvalue, uint8_t doubler, uint32_t ref_divider, uint32_t Ps_Ld, uint32_t Pdg, uint32_t mod_value,uint32_t ChcSpaceFreq)
{
	uint16_t *rtu_pll_ld,delay=0;
	rtu_pll_ld = (uint16_t *)Get_RTU_dev_ptr_PCA9535_Inputstate();
	
	if(chip == TX_PLL_SELECT)
	{
		Set_TX_PLL_Start_Init();
		Rtu_Select_TX_VCO_By_TXfreq(ulFreq);
		TX_ENABLE;

		(*rtu_pll_ld) &= ~(0x1<<12);
	}
	else if(chip == RX_PLL_SELECT ||chip == LO2_PLL_SELECT || chip == REF_PLL_SELECT)
	{
		(*rtu_pll_ld) &= ~(0x1<<(13+chip));
		if(chip == RX_PLL_SELECT)
		{
			Rtu_Select_RX_VCO_By_RXfreq(ulFreq);
			
//			TX_DISABLE;
		}
		else if(chip == REF_PLL_SELECT)
		{
			Rtu_Select_REF_VCO_By_EXTCLK_1();
		}
		
	}
	else
	{
		return;
	}
	
	if(chip == RX_PLL_SELECT ||chip == TX_PLL_SELECT)
	{
		//����72310
		Set_SKY72310_LD( chip,ulFreq,ulReferenece,rvalue,doubler,ref_divider,Ps_Ld,Pdg,mod_value,ChcSpaceFreq);
	}
	else
	{
		//����ADF4001
		Set_ADF4001_Freq(chip,ulFreq,ulReferenece);
	}
	
	if(chip == TX_PLL_SELECT)
	{
		Set_TX_PLL_End_Init();// ����dma spi---txmod�Ĵ�������
	}
	while(delay<10)
	{
		if(chip == TX_PLL_SELECT && ((*rtu_pll_ld)&(0x1<<12)))
		{
			return TRUE;
		}
		else 
		{
			if((*rtu_pll_ld)&(13+chip))
			{
				return TRUE;
			}
		}
		delay++;
		Delay_ms(10);
	}

	return FALSE;
}



