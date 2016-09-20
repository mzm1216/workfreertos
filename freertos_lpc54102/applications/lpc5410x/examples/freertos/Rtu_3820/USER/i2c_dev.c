/***********************************************************************************
 * 文 件 名   : i2c_dev.c
 * 负 责 人   : MZM
 * 创建日期   : 2016年2月25日
 * 文件描述   : i2c设备应用，SE98a，PCA0535芯片读取
 * 版权说明   : Copyright (c) 2008-2016   北京市万格数码通讯科技有限公司
 * 其    他   : 
 * 修改日志   : 
***********************************************************************************/
#define IIC_DEV_G
#include "i2c_dev.h"


void RTU_SE98A_Registers_init()
{
	uint16_t tmptemp;

	tmptemp = 0x0C00; // config reg--01:0209 hysteresis=1.5 event output=interrupt mode /enable
//	tmptemp = 0x0902; // config reg--01:0209 hysteresis=1.5 event output=interrupt mode /enable
	I2C1_Write(I2C1_TO_TEMP_DEVICE0_ADDR,SE98A_CONFIG_REG,2,(uint8_t *)&tmptemp);
	I2C1_Write(I2C1_TO_TEMP_DEVICE1_ADDR,SE98A_CONFIG_REG,2,(uint8_t *)&tmptemp);

	
	tmptemp = 0x5005;// 85°C upper boundary alarm
	I2C1_Write(I2C1_TO_TEMP_DEVICE0_ADDR,SE98A_UPPER_ALARM,2,(uint8_t *)&tmptemp);
	I2C1_Write(I2C1_TO_TEMP_DEVICE1_ADDR,SE98A_UPPER_ALARM,2,(uint8_t *)&tmptemp);

	
	tmptemp = 0x401F;// -20°C lower boundary alarm
	I2C1_Write(I2C1_TO_TEMP_DEVICE0_ADDR,SE98A_LOWER_ALARM,2,(uint8_t *)&tmptemp);
	I2C1_Write(I2C1_TO_TEMP_DEVICE1_ADDR,SE98A_LOWER_ALARM,2,(uint8_t *)&tmptemp);

	tmptemp = 0xF005;//95°C  critical alarm trip reg
	I2C1_Write(I2C1_TO_TEMP_DEVICE0_ADDR,SE98A_CRITICAL_ALARM,2,(uint8_t *)&tmptemp);
	I2C1_Write(I2C1_TO_TEMP_DEVICE1_ADDR,SE98A_CRITICAL_ALARM,2,(uint8_t *)&tmptemp);

//	tmptemp = (0x1<<7 |0x1);//disable SMBus
	tmptemp = 0x0000;		//enable smbus
	I2C1_Write(I2C1_TO_TEMP_DEVICE0_ADDR,SE98A_SMBUS_REG,2,(uint8_t *)&tmptemp);
	I2C1_Write(I2C1_TO_TEMP_DEVICE1_ADDR,SE98A_SMBUS_REG,2,(uint8_t *)&tmptemp);
	
}


/*****************************************************************************
 * 函 数 名  : RTU_GET_TX_DEVICE_ID
 * 负 责 人  : MZM
 * 创建日期  : 2016年5月24日
 * 函数功能  : 获取发射温度芯片的 设备ID
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
int32_t RTU_GET_TX_DEVICE_ID()
{
	static int32_t	tmptemp=0;
	
	I2C1_Read(I2C1_TO_TEMP_DEVICE0_ADDR,SE98A_DEVICD_ID_REG,2,(uint8_t *)&tmptemp);
	return tmptemp;
}

/*****************************************************************************
 * 函 数 名  : RTU_GET_TX_ID
 * 负 责 人  : MZM
 * 创建日期  : 2016年5月24日
 * 函数功能  : 获取发射温度芯片的 制造商ID
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
int32_t RTU_GET_TX_ID()
{
	static int32_t	tmptemp=0;
	
	I2C1_Read(I2C1_TO_TEMP_DEVICE0_ADDR,SE98A_MANU_ID_REG,2,(uint8_t *)&tmptemp);
	return tmptemp;
}

/*****************************************************************************
 * 函 数 名  : RTU_Get_TX_Temp
 * 负 责 人  : MZM
 * 创建日期  : 2016年5月24日
 * 函数功能  : 获取发射温度芯片 的 温度值
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
int32_t RTU_Get_TX_Temp()
{
	return RTU_Get_Temp(I2C1_TO_TEMP_DEVICE0_ADDR);
}

/*****************************************************************************
 * 函 数 名  : RTU_Get_TX_Temp
 * 负 责 人  : MZM
 * 创建日期  : 2016年5月24日
 * 函数功能  : 获取发射温度芯片 的 温度值
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/

int32_t RTU_Get_PA_Temp()
{
	return RTU_Get_Temp(I2C1_TO_TEMP_DEVICE1_ADDR);
}


int32_t RTU_Get_Temp(uint8_t addr)
{	
	int8_t sign=0;
	uint16_t temp=0,tmpdata=0;
	int32_t	tmptemp=0;
	
	I2C1_Read(addr,SE98A_TEMP_REG,2,(uint8_t *)&temp);

	swap(temp);
	
	rt_kprintf(0,"GET_TEMP:[%x][%d]\r\n",temp,temp);
	sign = (temp&(0x1<<12)?(-1):1);

	/*
		bitmap:
		
		bit0		:RFU 
		bit1-bit11	:TEMP
		bit12		:SIGN
		bit13		:BAW
		bit14		:AAW
		bit15		:ACT

		得到温度数值
	*/
#if 1

	
//	tmptemp = ((temp&0xFFE) >> 1);
	tmptemp = ((temp) >> 1);
	tmptemp &=0x7FF ;
	
//	if(sign != 1)//负温度处理
//	{
//		tmptemp = 4096 - tmptemp;
//	}
	/* 温度扩大100倍，用于显示小数点后2位*/
	tmptemp *= 100;	
	tmptemp = tmptemp >> 3;  // 0.125°C ===1 
	// The 16-bit value used in the following Trip Point Set and Temperature Read-Back
	//registers is 2’s complement with the Least Significant Bit (LSB) equal to 0.0625 °C
#else
	temp>>=1;
	temp &=0x7FF ;
		
	/* 温度扩大100倍，用于显示小数点后2位*/
	temp *= 100;
	temp >>= 3;	// The 16-bit value used in the following Trip Point Set and Temperature Read-Back
				//registers is 2’s complement with the Least Significant Bit (LSB) equal to 0.0625 °C
#endif
	return (tmptemp*sign);
	
}

uint8_t RTU_Send_Data_PCA9535(uint8_t reg,uint8_t *data)
{
	return I2C1_Write(I2C1_TO_IOS_DEVICD_ADDR,reg,2,data);
}

uint8_t RTU_Read_Data_PCA9535(uint8_t reg,uint8_t *data)
{
	return I2C1_Read(I2C1_TO_IOS_DEVICD_ADDR,reg,2,data);
}


void RTU_Config_PC9535_Port()
{
	uint16_t configport;
	RTU_PCA9535_PORT_STRUCT *tmpcfg = (RTU_PCA9535_PORT_STRUCT *)&configport;

	configport = 0x0;//全部配置为输出

	tmpcfg->tx_LD = 1;	//输入
	tmpcfg->rx_LD = 1;	//输入
	tmpcfg->rx_LD2 = 1;	//输入
	tmpcfg->ref_LD = 1;	//输入
	
	RTU_Send_Data_PCA9535(PCA9535_CONFIG_PORT0,(uint8_t*)&configport);
//	RTU_Send_Data_PCA9535(PCA9535_SET_OUTPUT_PORT0,(uint8_t*)&configport);//全部拉低 输出
}

void RTU_Get_Port_Input( RTU_PCA9535_PORT_STRUCT *portstate)
{
	RTU_Read_Data_PCA9535(PCA9535_READ_INPUT_PORT0,(uint8_t*)portstate);
}

void RTU_Get_Port_Output( RTU_PCA9535_PORT_STRUCT *portstate)
{
	RTU_Read_Data_PCA9535(PCA9535_SET_OUTPUT_PORT0,(uint8_t*)portstate);
}
void RTU_Set_Port_Output( RTU_PCA9535_PORT_STRUCT *portstate)
{
	RTU_Send_Data_PCA9535(PCA9535_SET_OUTPUT_PORT0,(uint8_t*)portstate);
}


uint8_t RTU_Set_Output_Pin(uint8_t pin,uint8_t value)
{
	uint16_t portstate;
	
	RTU_Get_Port_Output((RTU_PCA9535_PORT_STRUCT *)&portstate);
	if(value == 1)
	{
		portstate |= 0x1<<pin;
	}
	else
	{
		portstate &= ~(0x1<<pin);
	}

	RTU_Set_Port_Output((RTU_PCA9535_PORT_STRUCT *)&portstate);
}


void RTU_Set_Output_Value_2_3Pins(uint16_t mark,uint16_t value)
{
	uint16_t portstate;
	
	RTU_Get_Port_Output((RTU_PCA9535_PORT_STRUCT *)&portstate);

	portstate &= ~(mark);
	
	portstate |= value;

	RTU_Set_Port_Output((RTU_PCA9535_PORT_STRUCT *)&portstate);
}


void RTU_Set_Output_CV_Read_Value(uint16_t value)
{
	RTU_Set_Output_Value_2_3Pins(SET_CV_ADC_VALUE_MARK,value);
}

void RTU_Set_CV_Read_TX_CV()
{
	RTU_Set_Output_CV_Read_Value(SET_CV_ADC_VALUE_TXCV);
}
void RTU_Set_CV_Read_RX_CV()
{
	RTU_Set_Output_CV_Read_Value(SET_CV_ADC_VALUE_RXCV);
}
void RTU_Set_CV_Read_REF_CV()
{
	RTU_Set_Output_CV_Read_Value(SET_CV_ADC_VALUE_REFCV);
}


void Rtu_Set_TXVCO_H(uint8_t value)
{
	RTU_Set_Output_Pin(PS_TXVCO_H,value);
}
void Rtu_Set_TXVCO_L(uint8_t value)
{
	RTU_Set_Output_Pin(PS_TXVCO_L,value);
}


/*****************************************************************************
 * 函 数 名  : Rtu_Select_TX_VCO
 * 负 责 人  : MZM
 * 创建日期  : 2016年4月25日
 * 函数功能  : 发射vco类型控制
 * 输入参数  : uint8_t flag  标志类型，h0l1或者 h1l0
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void Rtu_Select_TX_VCO(uint8_t flag)
{
	if(flag == SELECT_TX_VCO_H0L1)
	{
		Rtu_Set_TXVCO_H(0);
		Rtu_Set_TXVCO_L(1);
		
		RTU_Set_Output_Pin(TXCH_SLOW,0);
		RTU_Set_Output_Pin(TXCH_FAST,0);
	}
	else if(flag == SELECT_TX_VCO_H1L0)
	{
		Rtu_Set_TXVCO_H(1);
		Rtu_Set_TXVCO_L(0);
		
		RTU_Set_Output_Pin(TXCH_SLOW,0);
		RTU_Set_Output_Pin(TXCH_FAST,0);
	}
}

/*****************************************************************************
 * 函 数 名  : Rtu_Select_TX_VCO_By_TXfreq
 * 负 责 人  : MZM
 * 创建日期  : 2016年4月25日
 * 函数功能  : 通过判断发射频率的大小 来选择vco，定 430以下选用 hoL1的，430
               以上选用h1l0
 * 输入参数  : uint32_t txfreq  发射频率
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void Rtu_Select_TX_VCO_By_TXfreq(uint32_t txfreq)
{
	if(txfreq >= TX_VCO_LIMIT_FREQ)
	{
		Rtu_Select_TX_VCO(SELECT_TX_VCO_H1L0);// 430以上
	}
	else
	{
		Rtu_Select_TX_VCO(SELECT_TX_VCO_H0L1);//430 以下 选用 hol1 的vco
	}
}

void Rtu_Set_RXVCO_H(uint8_t value)
{
	RTU_Set_Output_Pin(PS_RXVCO_H,value);
}
void Rtu_Set_RXVCO_L(uint8_t value)
{
	RTU_Set_Output_Pin(PS_RXVCO_L,value);
}

void Rtu_Set_REFVCO_EXT_CLK(uint8_t value)
{
	RTU_Set_Output_Pin(EXT_CLK,value);
}

/*****************************************************************************
 * 函 数 名  : Rtu_Select_TX_VCO
 * 负 责 人  : MZM
 * 创建日期  : 2016年4月25日
 * 函数功能  : 发射vco类型控制
 * 输入参数  : uint8_t flag  标志类型，h0l1或者 h1l0
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void Rtu_Select_RX_VCO(uint8_t flag)
{
	if(flag == SELECT_RX_VCO_H0L1)
	{
		Rtu_Set_RXVCO_H(0);
		Rtu_Set_RXVCO_L(1);
		
		RTU_Set_Output_Pin(RXCH_SLOW,0);
		RTU_Set_Output_Pin(RXCH_FAST,0);
	}
	else if(flag == SELECT_RX_VCO_H1L0)
	{
		Rtu_Set_RXVCO_H(1);
		Rtu_Set_RXVCO_L(0);
		
		RTU_Set_Output_Pin(RXCH_SLOW,0);
		RTU_Set_Output_Pin(RXCH_FAST,0);
	}
}

void Rtu_Select_RX_VCO_By_RXfreq(uint32_t rxfreq)
{
	if(rxfreq >= RX_VCO_LIMIT_FREQ)
	{
		Rtu_Select_RX_VCO(SELECT_RX_VCO_H1L0);// 430以上
	}
	else
	{
		Rtu_Select_RX_VCO(SELECT_RX_VCO_H0L1);//430 以下 选用 hol1 的vco
	}
}

void Rtu_Select_REF_VCO_By_EXTCLK_1()
{
	Rtu_Set_REFVCO_EXT_CLK(1);
}
void Rtu_Select_REF_VCO_By_EXTCLK_0()
{
	Rtu_Set_REFVCO_EXT_CLK(0);
}



//bit 0 --txcv,bit1 rxcv bit2 refcv
/*****************************************************************************
 * 函 数 名  : RTU_Set_ADC_Read_CH
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月14日
 * 函数功能  : 设置cv的通道号：0x1--txcv 2---rxcv 4--- refcv
 * 输入参数  : uint8_t value  txcv/rxv/refcv 
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
uint8_t RTU_Set_ADC_Read_CH(uint8_t value)
{
	//RTU_PCA9535_PORT_STRUCT ;
	uint16_t portstate;
	
	RTU_Get_Port_Output((RTU_PCA9535_PORT_STRUCT *)&portstate);
	
	portstate &= ~(0x7<<TXCV_READ);//清空 value
	
	portstate |= ((value&0x7)<<TXCV_READ);

	RTU_Set_Port_Output((RTU_PCA9535_PORT_STRUCT *)&portstate);
}

/*****************************************************************************
 * 函 数 名  : Rtu_Set_ADC_Read_TXCH
 * 负 责 人  : MZM
 * 创建日期  : 2016年5月4日
 * 函数功能  : 设置ADC读取通道为：TXCV
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void Rtu_Set_ADC_Read_TXCH()
{
	RTU_Set_ADC_Read_CH(0x1);
}
/*****************************************************************************
 * 函 数 名  : Rtu_Set_ADC_Read_RXCH
 * 负 责 人  : MZM
 * 创建日期  : 2016年5月4日
 * 函数功能  : 设置ADC读取通道为：RXCV
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/

void Rtu_Set_ADC_Read_RXCH()
{
	RTU_Set_ADC_Read_CH(0x2);
}
/*****************************************************************************
 * 函 数 名  : Rtu_Set_ADC_Read_REFCH
 * 负 责 人  : MZM
 * 创建日期  : 2016年5月4日
 * 函数功能  : 设置ADC读取通道为：REFCV
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/

void Rtu_Set_ADC_Read_REFCH()
{
	RTU_Set_ADC_Read_CH(0x4);
}


/*****************************************************************************
 * 函 数 名  : Rtu_get_PLL_LD
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月3日
 * 函数功能  : 获取当前锁相环的锁定状态：0没有锁定，其他值位锁定通道，13-15
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
uint8_t Rtu_get_PLL_LD()
{
	RTU_PCA9535_PORT_STRUCT portstate;

	RTU_Get_Port_Input(&portstate);	

	if(portstate.tx_LD)
	{
		return PLL_TX_LD;
	}
	else if(portstate.rx_LD)
	{
		return PLL_RX_LD;
	}
	else if(portstate.rx_LD2)
	{
		return PLL_RX_LD2;
	}
	else if(portstate.ref_LD)
	{
		return PLL_REF_LD;
	}
	else
	{
		return 0;
	}



}

uint8_t Rtu_Get_TX_LD()
{
	RTU_PCA9535_PORT_STRUCT portstate;

	RTU_Get_Port_Input(&portstate);

	return portstate.tx_LD;	
}

uint8_t Rtu_Get_RX_LD()
{
	RTU_PCA9535_PORT_STRUCT portstate;

	RTU_Get_Port_Input(&portstate);

	return portstate.rx_LD;	
}

uint8_t Rtu_Get_RX2_LD()
{
	RTU_PCA9535_PORT_STRUCT portstate;

	RTU_Get_Port_Input(&portstate);

	return portstate.rx_LD2;	
}

uint8_t Rtu_Get_REF_LD()
{
	RTU_PCA9535_PORT_STRUCT portstate;

	RTU_Get_Port_Input(&portstate);

	return portstate.ref_LD;	
}

/*****************************************************************************
 * 函 数 名  : RTU_Se98_PCA9535_Init
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月3日
 * 函数功能  : 初始化I2c的设备：SE98A和PCA9535的初始化
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 倍main调用
 * 其    它  : 

*****************************************************************************/
void RTU_SE98_PCA9535_Init()
{
	RTU_Config_PC9535_Port();//config io 
}

