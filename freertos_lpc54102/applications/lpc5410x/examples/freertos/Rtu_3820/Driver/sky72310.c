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



// 3线spi的 片选信号线 端口和管脚   0--RX_pll 	1--LO2_Pll  2--REF_Pll  3--TX_PLL
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

// 3线spi的 数据线 端口和管脚 可扩展为3路
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


// 3线spi的 时钟线 端口和管脚 可扩展
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
	Chip_GPIO_SetPinState(LPC_GPIO, three_spi_data_port[chip], three_spi_data_pin[chip], (bool)1);
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

//   	I3C_Set_CS_high(Chip);
//	I3C_Set_Data_high(Chip);
//	I3C_Set_CLK_low(Chip);
	//for (j = 0; j < 2; j++);
	
	I3C_Set_CS_low(Chip);//cs low
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
 * 函 数 名  : freq_calc_fix
 * 负 责 人  : MZM
 * 创建日期  : 2016年6月23日
 * 函数功能  : 通过期望频率和鉴相频率 计算 72310芯片的整数寄存器、小数部分MSB
               和LSB的值
 * 输入参数  : int32_t fvco      期望频率
               int32_t fpd       鉴相频率
               uint16_t *nreg    传出整数指针 
               uint16_t *ndiv_m  传出小数部分MSB 
               uint16_t *ndiv_l  传出小数部分LSB
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
int	freq_calc_fix(int32_t fvco, int32_t fpd,uint16_t *nreg, uint16_t *ndiv_m, uint16_t *ndiv_l)
{
	int64_t delta;
	uint32_t div;
	uint16_t nr;

	/* 频率有效性判断 */
	if (fvco < 0)
		fvco = 0 - fvco;
	if (fpd < 0)
		fpd = 0 - fpd;

	/* 计算 正数  小数部分四舍五入*/
	nr = (fvco + (fpd >> 1)) / fpd;
	*nreg = nr - 32;
	
	/* 计算 小数部分扩大2^18   */
	delta = (int64_t)(1 << 18) * (fvco - nr * fpd);

	/* 如果正数  加上半个鉴相频率  
		负数  减去半个鉴相频率 */
	if (delta < 0)
		delta -= (fpd >> 1);
	else
		delta += (fpd >> 1);
	
	/*  计算  小数部分 MSB LSB*/
	div = (uint32_t)(delta / fpd) & 0x3ffff;

	*ndiv_m = div >> 8;
	*ndiv_l = div & 0xff;

	return 0;
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

	Fdiv_ref=(fp64_t)(ulReferenece/(ref_divider+1));// 19200000 4分频为 鉴相频率
	freq_calc_fix(ulFreq,Fdiv_ref,&nreg,&n_msb,&n_lsb);//计算 正数、小数msb、小数lsb

	if(chip==RX_PLL_SELECT || chip==TX_PLL_SELECT)
	{
		/*tmpreg_72310 测试用*/
		tmpreg_72310[tmpflg][0] = 0x0000 | nreg;
		tmpreg_72310[tmpflg][1] = 0x1000 | n_msb;
		tmpreg_72310[tmpflg][2] =  0x2000 | n_lsb;
		
		Write_SKY72310_ADF4001_Data(chip, 16, 0x5000 | ref_divider);
		Write_SKY72310_ADF4001_Data(chip, 16, 0x6000 | ( Ps_Ld << 5 ) | ( Pdg << 0 ));	// 6 相位检测bit5 =0 快锁失能 bit0-bit4 =F);
		Write_SKY72310_ADF4001_Data(chip, 16, 0x7000);
		Write_SKY72310_ADF4001_Data(chip, 16, 0x8000);
		Write_SKY72310_ADF4001_Data(chip, 16, 0x9000);
		
		Write_SKY72310_ADF4001_Data(chip, 16, 0x0000 | nreg);			 // 整数部分
		Write_SKY72310_ADF4001_Data(chip, 16, 0x2000 | n_lsb);			//小数部分的LSB -- 8bit--Note发送顺序: 应先发送LSB  在发送MSB
		Write_SKY72310_ADF4001_Data(chip, 16, 0x1000 | n_msb);		//小数部分的MSB --10bit
 	   }
	else
	{
	}
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
	unsigned long int r,n;
	
	Write_SKY72310_ADF4001_Data(chip, 24, 0x1Df893);
	Delay_ms(20);
	
	r = (REF_divide&0x3FFF)<<2; // R1-R14
	Write_SKY72310_ADF4001_Data(chip, 24, 0x000000|r);//设置R 
	Delay_ms(20);
	
	n = (N_divide&0x1FFF)<<8;// N1--N13
	Write_SKY72310_ADF4001_Data(chip, 24, 0x000001|n);//设置N
	Delay_ms(20);
	
	Write_SKY72310_ADF4001_Data(chip, 24, 0x1Df892);
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
	uint32_t tmppdfreq;
	uint32_t Rreg, Nreg;
	
	if(chip == REF_PLL_SELECT)
	{
		/*10M 锁定19.2M 王工指出-参考频率 必须指定鉴相频率为 5K，
		所以固定R=3840,n=2000 2016年5月20日16:30:08*/
		tmppdfreq = REF_PLL_PD_FREQ;	//参考鉴相频率  5 K 
	}
	else if(chip == LO2_PLL_SELECT)		//PLL芯片类型是  二本振?
	{
		tmppdfreq = LO2_PLL_PD_FREQ;	//二本振 鉴相频率为10 K
	}
	else
	{
//		tmpgcd = Get_GCD(ulFreq,ulReferenece);
//		Rreg=ulReferenece/tmpgcd;
//		Nreg=ulFreq/tmpgcd;
	}
	
	Rreg=ulReferenece/tmppdfreq;  	//计算 Ref counter--14bit
	Nreg=ulFreq/tmppdfreq;			//计算  N counter  13bit
	
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

 * 输出参数  : TRUE 锁定成功  FALSE  锁定失败
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 
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
		//设置72310
		Set_SKY72310_LD( chip,ulFreq,ulReferenece,rvalue,doubler,ref_divider,Ps_Ld,Pdg,mod_value,ChcSpaceFreq);
	}
	else
	{
		//设置ADF4001
		Set_ADF4001_Freq(chip,ulFreq,ulReferenece);
	}
	
	if(chip == TX_PLL_SELECT)
	{
		Set_TX_PLL_End_Init();// 启动dma spi---txmod寄存器设置
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



