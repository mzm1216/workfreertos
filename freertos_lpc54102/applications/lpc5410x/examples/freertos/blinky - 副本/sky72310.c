/***********************************************************************************
 * 文 件 名   : sky72310.c
 * 负 责 人   : MZM
 * 创建日期   : 2016年2月25日
 * 文件描述   : IO 口模拟三线SPI的操作接口，包含CS、CLK、DATA3根线，3个锁相
                环的的操纵接口，其中RX PLL是sky72310芯片，LO2和REF锁相环采用
                的ADF4001芯片，本文件提供这两个芯片的初始化和操作接口
                
 * 版权说明   : Copyright (c) 2008-2016   北京市万格数码通讯科技有限公司
 * 其    他   : 
 * 修改日志   : 
***********************************************************************************/

#define	Sky72310_GLOBALS
#include "ioconfig.h"
#include "sky72310.h"

#define RX_PLL_SELECT 	0	//接收频率锁相环 选择
#define LO2_PLL_SELECT	1	//接收LO2频率锁相环 选择
#define REF_PLL_SELECT	2	//参考频率锁相环 选择

// 3线spi的 片选信号线 端口和管脚   0--RX_pll 	1--LO2_Pll  2--REF_Pll
static uint8_t three_spi_cs_port[3]={PLL_RX_SEL_PORT,PLL_RX_SEL2_PORT,PLL_REF_SEL_PORT};
static uint8_t three_spi_cs_pin[3]={PLL_RX_SEL_PIN,PLL_RX_SEL2_PIN,PLL_REF_SEL_PIN};

// 3线spi的 数据线 端口和管脚 可扩展为3路
static uint8_t three_spi_data_port[3]={PLL_RX_SCK_PORT,PLL_RX_SCK_PORT,PLL_RX_SCK_PORT};
static uint8_t three_spi_data_pin[3]={PLL_RX_SCK_PIN,PLL_RX_SCK_PIN,PLL_RX_SCK_PIN};

// 3线spi的 时钟线 端口和管脚 可扩展
static uint8_t three_spi_clk_port[3]={PLL_RX_DATA_PORT,PLL_RX_DATA_PORT,PLL_RX_DATA_PORT};
static uint8_t three_spi_clk_pin[3]={PLL_RX_DATA_PIN,PLL_RX_DATA_PIN,PLL_RX_DATA_PIN};

/*****************************************************************************
 * 函 数 名  : I3C_Set_CS_high
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月25日
 * 函数功能  : 设置锁相环的片选信号为高
 * 输入参数  : uint8_t chip  操纵锁相环选择---0=接收锁相环，1=接收LO2,2=参考
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void I3C_Set_CS_high(uint8_t chip)
{
	Chip_GPIO_SetPinState(LPC_GPIO, three_spi_cs_port[chip], three_spi_cs_pin[chip], (bool)1);
	return;
}

/*****************************************************************************
 * 函 数 名  : I3C_Set_CS_low
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月25日
 * 函数功能  : 设置锁相环的片选信号为 低
 * 输入参数  : uint8_t chip  操纵锁相环选择---0=接收锁相环，1=接收LO2,2=参考
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void I3C_Set_CS_low(uint8_t chip)//chip : 0 - IF, 1 - TX, 2 - RX
{
	Chip_GPIO_SetPinState(LPC_GPIO, three_spi_cs_port[chip], three_spi_cs_pin[chip], (bool)0);
	return;
	
}

/*****************************************************************************
 * 函 数 名  : I3C_Set_CLK_high
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月25日
 * 函数功能  :  设置锁相环的 时钟 为 高
 * 输入参数  : uint8_t chip  操纵锁相环选择---0=接收锁相环，1=接收LO2,2=参考
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void I3C_Set_CLK_high(uint8_t chip)
{
	Chip_GPIO_SetPinState(LPC_GPIO, three_spi_clk_port[chip], three_spi_clk_pin[chip], (bool)1);
	return;
}

/*****************************************************************************
 * 函 数 名  : I3C_Set_CLK_LOW
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月25日
 * 函数功能  :  设置锁相环的 时钟 为 低
 * 输入参数  : uint8_t chip  操纵锁相环选择---0=接收锁相环，1=接收LO2,2=参考
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void I3C_Set_CLK_low(uint8_t chip)
{
	Chip_GPIO_SetPinState(LPC_GPIO, three_spi_clk_port[chip], three_spi_clk_pin[chip], (bool)0);
	return;

}

/*****************************************************************************
 * 函 数 名  : I3C_Set_Data_high
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月25日
 * 函数功能  :  设置锁相环的 数据线 高
 * 输入参数  : uint8_t chip  操纵锁相环选择---0=接收锁相环，1=接收LO2,2=参考
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/

void I3C_Set_Data_high(uint8_t chip)
{
	Chip_GPIO_SetPinState(LPC_GPIO, three_spi_data_port[chip], three_spi_data_pin[chip], (bool)0);
	return;
}

/*****************************************************************************
 * 函 数 名  : I3C_Set_Data_low
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月25日
 * 函数功能  :  设置锁相环的 数据线 低
 * 输入参数  : uint8_t chip  操纵锁相环选择---0=接收锁相环，1=接收LO2,2=参考
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void I3C_Set_Data_low(uint8_t chip)
{
	Chip_GPIO_SetPinState(LPC_GPIO, three_spi_data_port[chip], three_spi_data_pin[chip], (bool)0);
	return;
}


/*****************************************************************************
 * 函 数 名  : Delay_ms
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月25日
 * 函数功能  : 死等延时
 * 输入参数  : uint8_t dly  延时大小
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void Delay_ms(uint8_t dly)
{
    uint32_t i;
    
    for (; dly > 0; dly--)
        for (i = 0; i < 0xfff; i++);
}


/*****************************************************************************
 * 函 数 名  : Write_SKY72310_ADF4001_Data
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月25日
 * 函数功能  : IO口模拟三线spi，向指定锁相环发送特定长度的数据。
 * 输入参数  : uint8_t Chip      PLL类型，0==接收，1==接收LO2, 2==参考
               uint8_t BitLen    发送bit个数
               uint32_t regdata  发送数据
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void Write_SKY72310_ADF4001_Data(uint8_t Chip,uint8_t BitLen,uint32_t regdata)//chip : 0 - IF, 1 - TX, 2 - RX;BitLen: write bitlen;ulD4113:write to reg data
{//需要配置成16位数据，4位地址位，12位数据位
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
		I3C_Set_CLK_low(Chip);             //CLOCK拉低
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
//调用	
//Set_SKY72310_FL(1,361000000,19200000,  0,0,0,0,0,0,0)
//Set_ADF4153(1, 364025000, 10000000, 1, 1, 0, 0, 2,4091);//LOCK
//发射芯片,		PLL_fre(Hz), 		REF_fre,			,				,			ref_divider的分频位,快锁功能(1)锁定指示(0),Pdg泵电流(0~31),

/*****************************************************************************
 * 函 数 名  : Set_SKY72310_LD
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月25日
 * 函数功能  : 设置SKY72310芯片，获得期望频率
 * 输入参数  : uint8_t chip           PLL类型，0==接收，1==LO2,2==参考
               uint32_t ulFreq        期望得到的频率
               uint32_t ulReferenece  参考频率
               uint32_t rvalue        rvalue
               uint8_t doubler        doubule
               uint32_t ref_divider   ref-divider分频位
               uint32_t Ps_Ld         快锁功能
               uint32_t Pdg           锁定指示
               uint32_t mod_value     
               uint32_t ChcSpaceFreq  
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

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
	/*配置各个寄存器的地址域*/
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

	Fdiv_ref=(fp64_t)(ulReferenece/(ref_divider+1));// 19200000 4分频
	Nfractional=((uint32_t)((fp64_t)ulFreq/Fdiv_ref+0.5));//计算整数部分
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
 * 函 数 名  : Get_GCD
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月25日
 * 函数功能  : 辗转相除法---得到两个数的 最大公约数数
 				用辗转相除法求几个数的最大公约数，可以先求出其中任意两个数的
 				最大公约数，再求这个最大公约数与第三个数的最大公约数，
 				依次求下去，直到最后一个数为止。最后所得的那个最大公约数，
 				就是所有这些数的最大公约数
 * 输入参数  : uint32_t ref  参考频率
               uint32_t vco  期望得到频率
 * 输出参数  : 无
 * 返 回 值  : 最大公约数
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
uint32_t Get_GCD(uint32_t ref,uint32_t vco)
{
	uint32_t max,min,tmp;
	
	if(ref<vco)//获取较大值
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
	while(1)//余数不为0 未找到最大公约数
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
	return min;//返回最大公约数
}


//uint8_t calculator_R_N(uint32_t ulFreq, uint32_t ulReferenece)
//{	
//}

/*****************************************************************************
 * 函 数 名  : ADF4001_Config
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月25日
 * 函数功能  : 将R和N配置到ADF4001中
 * 输入参数  : uint8_t chip                  PLL 类型：0==接收，1==LO2,2==REF
               unsigned long int REF_divide  R计数器的值
               unsigned long int N_divide    N计数器的值
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void ADF4001_Config(uint8_t chip,unsigned long int REF_divide,unsigned long int N_divide)
{
	unsigned long int m,n;
	
	Write_SKY72310_ADF4001_Data(chip, 24, 0x1ff843);
	Delay_ms(200);
	
	m=REF_divide<<2;
	Write_SKY72310_ADF4001_Data(chip, 24, 0x000000|m);//设置R 
	Delay_ms(200);
	
	n=N_divide<<8;
	Write_SKY72310_ADF4001_Data(chip, 24, 0x000001|n);//设置N
	Delay_ms(200);
	
	Write_SKY72310_ADF4001_Data(chip, 24, 0x1ff842);
}

/*****************************************************************************
 * 函 数 名  : Set_ADF4001_Freq
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月25日
 * 函数功能  : 根据期望频率和参考频率 计算出R、N值，并配置到adf4001的寄存器
               中
 * 输入参数  : uint8_t chip           PLL 类型:0==j接收，1==LO2，2==REF
               uint32_t ulFreq        期望频率
               uint32_t ulReferenece  参考频率
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

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
 * 函 数 名  : Set_PLL_Freq
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月25日
 * 函数功能  : 配置sky72310或者adf4001，得到期望的频率
 * 输入参数  : uint8_t chip           PLL类型，0==接收，1==LO2, 2==参考
               uint32_t ulFreq        期望得到的频率
               uint32_t ulReferenece  参考频率
               uint32_t rvalue        rvalue
               uint8_t doubler        doubule
               uint32_t ref_divider   ref-divider分频位
               uint32_t Ps_Ld         快锁功能
               uint32_t Pdg           锁定指示
               uint32_t mod_value     
               uint32_t ChcSpaceFreq  

 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 
//Set_SKY72310_FL(1,361000000,19200000,  0,0,0,0,0,0,0)

*****************************************************************************/
void Set_PLL_Freq(uint8_t chip, uint32_t ulFreq, uint32_t ulReferenece, uint32_t rvalue, uint8_t doubler, uint32_t ref_divider, uint32_t Ps_Ld, uint32_t Pdg, uint32_t mod_value,uint32_t ChcSpaceFreq)
{
	if(chip == RX_PLL_SELECT)
	{
		//设置72310
		Set_SKY72310_LD( chip,ulFreq,ulReferenece,rvalue,doubler,ref_divider,Ps_Ld,Pdg,mod_value,ChcSpaceFreq);
	}
	else
	{
		//设置ADF4001
		Set_ADF4001_Freq(chip,ulFreq,ulReferenece);
	}
}

