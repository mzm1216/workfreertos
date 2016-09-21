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

#define RX_PLL_SELECT 	0	//����Ƶ�����໷ ѡ��
#define LO2_PLL_SELECT	1	//����LO2Ƶ�����໷ ѡ��
#define REF_PLL_SELECT	2	//�ο�Ƶ�����໷ ѡ��

// 3��spi�� Ƭѡ�ź��� �˿ں͹ܽ�   0--RX_pll 	1--LO2_Pll  2--REF_Pll
static uint8_t three_spi_cs_port[3]={PLL_RX_SEL_PORT,PLL_RX_SEL2_PORT,PLL_REF_SEL_PORT};
static uint8_t three_spi_cs_pin[3]={PLL_RX_SEL_PIN,PLL_RX_SEL2_PIN,PLL_REF_SEL_PIN};

// 3��spi�� ������ �˿ں͹ܽ� ����չΪ3·
static uint8_t three_spi_data_port[3]={PLL_RX_SCK_PORT,PLL_RX_SCK_PORT,PLL_RX_SCK_PORT};
static uint8_t three_spi_data_pin[3]={PLL_RX_SCK_PIN,PLL_RX_SCK_PIN,PLL_RX_SCK_PIN};

// 3��spi�� ʱ���� �˿ں͹ܽ� ����չ
static uint8_t three_spi_clk_port[3]={PLL_RX_DATA_PORT,PLL_RX_DATA_PORT,PLL_RX_DATA_PORT};
static uint8_t three_spi_clk_pin[3]={PLL_RX_DATA_PIN,PLL_RX_DATA_PIN,PLL_RX_DATA_PIN};

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
	Chip_GPIO_SetPinState(LPC_GPIO, three_spi_data_port[chip], three_spi_data_pin[chip], (bool)0);
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

    I3C_Set_CS_high(Chip);
	I3C_Set_Data_high(Chip);
	I3C_Set_CLK_low(Chip);
	//for (j = 0; j < 2; j++);
	
	I3C_Set_CS_high(Chip);//cs low
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
////		//for (j = 0; j < 50; j++);
////		T3TC=0;
////		T3TCR = 0x01;
////		//while((T3IR & 0x01) == 0);
////		T3IR = 0x01;
////		T3IR = 0x03;
		I3C_Set_CLK_high(Chip);
		//for (j = 0; j < 50; j++);
//////		T3TC=0;
//////		T3TCR = 0x01;
//////		//while((T3IR & 0x01) == 0);
//////		T3IR = 0x01;
//////		T3IR = 0x03;
	}
    
	I3C_Set_CLK_low(Chip);
	I3C_Set_Data_high(Chip);
	I3C_Set_CS_high(Chip);
	return;                                                                                
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
	uint32_t	DIVIDER_R=0,DIVIDENDM_R=0,DIVIDENDL_R=0,UNUSED1_R=0,UNUSED2_R=0,RFD_R=0,PD_CPC_R=0,PD_MOSC_R=0,MC_R=0,MD_R=0;
	uint32_t	i=0;	

	//u_short	Ps_Ld=0,Pdg=0;
	uint32_t	Fpd=0,Spd=0,Sm=0,Sf=0,Mos=0,Mpte=0;
	uint32_t	Mad;
	uint32_t	Nfractional=0;
	uint32_t	Nreg=0;
	uint32_t	Dividend_int=0;
	uint32_t m,n,x;
	
	fp64_t	Fdiv_ref=0;
	fp64_t 	Dividend=0;

	if(chip==RX_PLL_SELECT)
	{
		ulFreq=ulFreq+73350000;
	}
	else
	{
		ulFreq=ulFreq;
	}
	/*���ø����Ĵ����ĵ�ַ��*/
	DIVIDER_R=DIVIDER_R+((i++)<<12);
	DIVIDENDM_R=DIVIDENDM_R+((i++)<<12);
	DIVIDENDL_R=DIVIDENDL_R+((i++)<<12);
	UNUSED1_R=UNUSED1_R+((i++)<<12);
	UNUSED2_R=UNUSED2_R+((i++)<<12);
	
	RFD_R=RFD_R+((i++)<<12);
	PD_CPC_R=PD_CPC_R+((i++)<<12);
	PD_MOSC_R=PD_MOSC_R+((i++)<<12);
	MC_R=MC_R+((i++)<<12);
	
	MD_R=MD_R+((i++)<<12);

//	ref_divider=3;
//	Ps_Ld=0;Pdg=0x0f;
	Mpte=0;Mos=0x01;Sf=0;Sm=0;Spd=0;Fpd=0;
	Mad=0;

	Fdiv_ref=(fp64_t)(ulReferenece/(ref_divider+1));// 19200000 4��Ƶ
	Nfractional=((uint32_t)((fp64_t)ulFreq/Fdiv_ref+0.5));//������������
	Nreg=Nfractional-32;

	//aa=(((fp32)ulFreq/Fdiv_ref)-Nreg-32);
	Dividend=262144*(((fp64_t)ulFreq/Fdiv_ref)-(fp64_t)Nreg-(fp64_t)32);
	//Dividend=262144*aa;
	if(Dividend>0)
	{
		Dividend_int=(uint32_t)(Dividend+0.5);
	}
	else
	{
		Dividend_int=(uint32_t)((Dividend-0.5)*(-1));
		Dividend_int=262144-Dividend_int;
	}

	x=Nreg;	
	m=Dividend_int/256;
	n=(Dividend_int%256);
	
	DIVIDER_R |= x;
	if(DIVIDER_R<6)DIVIDER_R=6;
	if(DIVIDER_R>505)DIVIDER_R=505;
	DIVIDENDM_R |= (m<<0);
	DIVIDENDL_R |= (n<<0);	
	RFD_R |= ( ref_divider<<0 );
	PD_CPC_R |= ( Ps_Ld << 5 ) | ( Pdg << 0 );
	PD_MOSC_R |= ( Mpte << 9 ) | ( Mos << 6 ) |( Sf << 3 ) | ( Sm << 2 ) |( Spd << 1 ) | ( Fpd << 0 );
	MC_R |= (Mad<<9);
	//while(1)
	//{
	Write_SKY72310_ADF4001_Data(chip, 16, DIVIDER_R); // clear noise and spur mode
//	Delay_ms(5);
	Write_SKY72310_ADF4001_Data(chip, 16, DIVIDENDM_R);
//	Delay_ms(5);
	Write_SKY72310_ADF4001_Data(chip, 16, DIVIDENDL_R);
//	Delay_ms(5);
	Write_SKY72310_ADF4001_Data(chip, 16, UNUSED1_R);
//	Delay_ms(5);
	Write_SKY72310_ADF4001_Data(chip, 16, UNUSED2_R);
//	Delay_ms(5);
	Write_SKY72310_ADF4001_Data(chip, 16, RFD_R);
//	Delay_ms(5);
	Write_SKY72310_ADF4001_Data(chip, 16, PD_CPC_R);
//	Delay_ms(5);
	Write_SKY72310_ADF4001_Data(chip, 16, PD_MOSC_R);
//	Delay_ms(5);
	Write_SKY72310_ADF4001_Data(chip, 16, MC_R);
//	Delay_ms(5);
    //}
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
	unsigned long int m,n;
	
	Write_SKY72310_ADF4001_Data(chip, 24, 0x1ff843);
	Delay_ms(200);
	
	m=REF_divide<<2;
	Write_SKY72310_ADF4001_Data(chip, 24, 0x000000|m);//����R 
	Delay_ms(200);
	
	n=N_divide<<8;
	Write_SKY72310_ADF4001_Data(chip, 24, 0x000001|n);//����N
	Delay_ms(200);
	
	Write_SKY72310_ADF4001_Data(chip, 24, 0x1ff842);
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
	uint32_t tmpgcd;
	uint32_t Rreg, Nreg;
	tmpgcd = Get_GCD(ulFreq,ulReferenece);

	Rreg=ulReferenece/tmpgcd;
	Nreg=ulFreq/tmpgcd;
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

 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 
//Set_SKY72310_FL(1,361000000,19200000,  0,0,0,0,0,0,0)

*****************************************************************************/
void Set_PLL_Freq(uint8_t chip, uint32_t ulFreq, uint32_t ulReferenece, uint32_t rvalue, uint8_t doubler, uint32_t ref_divider, uint32_t Ps_Ld, uint32_t Pdg, uint32_t mod_value,uint32_t ChcSpaceFreq)
{
	if(chip == RX_PLL_SELECT)
	{
		//����72310
		Set_SKY72310_LD( chip,ulFreq,ulReferenece,rvalue,doubler,ref_divider,Ps_Ld,Pdg,mod_value,ChcSpaceFreq);
	}
	else
	{
		//����ADF4001
		Set_ADF4001_Freq(chip,ulFreq,ulReferenece);
	}
}

