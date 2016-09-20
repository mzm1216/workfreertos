/***********************************************************************************
 * 文 件 名   : driver_i2c.c
 * 负 责 人   : MZM
 * 创建日期   : 2016年2月25日
 * 文件描述   : 用IO口模拟I2C0 和I2C1，主要在配置I2C的IO口，不应配置成功能口
                ，要配置普通gpio
 * 版权说明   : Copyright (c) 2008-2016   北京市万格数码通讯科技有限公司
 * 其    他   : 
 * 修改日志   : 
***********************************************************************************/
#define IIC_GLOBALS
#include "driver_i2c.h"
//#include "config.h"



//配置时钟线 输出
void IIC_SCL_OUT(uint8_t ID)
{
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, simulation_i2c_sck_port[ID], simulation_i2c_sck_pin[ID]);
}
//配置时钟线 输如
void IIC_SCL_IN(uint8_t ID)
{
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, simulation_i2c_sck_port[ID], simulation_i2c_sck_pin[ID]);
}
//设置 时钟线为高电平
void IIC_SCL_H(uint8_t ID)
{
	Chip_GPIO_SetPinState(LPC_GPIO, simulation_i2c_sck_port[ID], simulation_i2c_sck_pin[ID],1);
}
//设置 时钟线为低电平
void IIC_SCL_L(uint8_t ID)
{
	Chip_GPIO_SetPinState(LPC_GPIO, simulation_i2c_sck_port[ID], simulation_i2c_sck_pin[ID],0);
}

//配置数据线 为 输出
void IIC_SDA_OUT(uint8_t ID)
{
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, simulation_i2c_sda_port[ID], simulation_i2c_sda_pin[ID]);
}
//配置数据线 为 输人
void IIC_SDA_IN(uint8_t ID)
{
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, simulation_i2c_sda_port[ID], simulation_i2c_sda_pin[ID]);
}
//设置 数据线为  高  电平
void IIC_SDA_H(uint8_t ID)
{
	Chip_GPIO_SetPinState(LPC_GPIO, simulation_i2c_sda_port[ID], simulation_i2c_sda_pin[ID],1);
}
//设置 数据线为  低  电平
void IIC_SDA_L(uint8_t ID)
{
	Chip_GPIO_SetPinState(LPC_GPIO, simulation_i2c_sda_port[ID], simulation_i2c_sda_pin[ID],0);
}

//读取数据线的值
uint8_t IIC_SDA_READ(uint8_t ID)
{
	return Chip_GPIO_ReadPortBit(LPC_GPIO, simulation_i2c_sda_port[ID], simulation_i2c_sda_pin[ID]);
}

//短延时
void IIC_Delay()
{
	int i;
	for(i=0;i<40;i++);
	
}


/*****************************************************************************
 * 函 数 名  : RTU_I2CDev_Init
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月25日
 * 函数功能  : 初始化 I2C 0 和I2c1
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void RTU_I2CDev_Init()
{
	IIC_IO_Init(SIMULATED_I2C0_ID);//初始化 i2c0
	IIC_IO_Init(SIMULATED_I2C1_ID);//初始化 i2c1
}


/*****************************************************************************
 * 函 数 名  : IIC_IO_Init
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月25日
 * 函数功能  : 配置I2C的 IO口书方向
 * 输入参数  : uint8_t ID  0===i2c0  1==i2c1
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void IIC_IO_Init(uint8_t ID)
{			
	IIC_SCL_OUT(ID);
	IIC_SDA_OUT(ID);
}

//产生一个start信号
void IIC_Start(uint8_t ID)
{
	IIC_SDA_OUT(ID);     	//配置sda线输出
	
	IIC_SDA_H(ID);	  	  	// 1
	IIC_SCL_H(ID);			// 1
	IIC_Delay();
	
 	IIC_SDA_L(ID);			//0  START:when CLK is high,DATA change form high to low 
	IIC_Delay();
	
	IIC_SCL_L(ID);			//锁住I2C总线，准备发送或接收数据 
}

//产生一个停止信号
void IIC_Stop(uint8_t ID)
{
	IIC_SDA_OUT(ID);  		//配置sda线输出
	
	IIC_SDA_L(ID);
	IIC_SCL_H(ID);
 	IIC_Delay(); 
	
	IIC_SDA_H(ID);
	IIC_Delay();							   	
}
/*****************************************************************************
 * 函 数 名  : IIC_SendACK
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月25日
 * 函数功能  : 发射一个应答信号
 * 输入参数  : uint8_t ID   i2c
               uint8_t ack   (0:ACK 1:NAK)
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void IIC_SendACK(uint8_t ID,uint8_t ack)
{
	if(ack==1)						//写应答信号
	{
		IIC_SDA_H(ID);
	}
	else
	{
		IIC_SDA_L(ID);
	}
    IIC_SCL_H(ID);                  //拉高时钟线
    IIC_Delay();                 	//延时
    IIC_SCL_L(ID);                  //拉低时钟线
    IIC_Delay();                 	//延时
}
/*****************************************************************************
 * 函 数 名  : IIC_RecvACK
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月25日
 * 函数功能  : 接收一个应答信号
 * 输入参数  : uint8_t ID   i2c
 * 输出参数  : (0:ACK 1:NAK)
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
uint8_t IIC_RecvACK(uint8_t ID)
{

	IIC_SDA_IN(ID);
    IIC_SCL_H(ID);               //拉高时钟线
    IIC_Delay();                 //延时
    if(IIC_SDA_READ(ID))
	{
		IIC_SCL_L(ID);
		return NAK;
	}                			//读应答信号
    IIC_SCL_L(ID);              //拉低时钟线
    IIC_Delay();                //延时
    return ACK;
	
	
	
}

/*****************************************************************************
 * 函 数 名  : IIC_Send_Byte
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月25日
 * 函数功能  : 通过i2c发送一个数据 data
 * 输入参数  : uint8_t ID   i2c0或者i2c1
               uint8_t dat  发送数据
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void IIC_Send_Byte(uint8_t ID,uint8_t dat)
{                        
    uint8_t i,a;   
	IIC_SDA_OUT(ID);
    for (i=0; i<8; i++)         
    {
		a=(dat>>(7-i))&1;
		if(a==1)
		{
			IIC_SDA_H(ID);
		}
		else
		{
			IIC_SDA_L(ID);
		}
        IIC_SCL_H(ID);              
        IIC_Delay();           
        IIC_SCL_L(ID);           
        IIC_Delay();         
    }

} 	    

/*****************************************************************************
 * 函 数 名  : IIC_Read_Byte
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月25日
 * 函数功能  : 通过i2c读取一个字节的数据
 * 输入参数  : uint8_t ID  i2c0或者i2c1
 * 输出参数  : 读取数据
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
uint8_t IIC_Read_Byte(uint8_t ID)
{
	uint8_t i,dat=0;
	IIC_SDA_IN(ID);//SDA设置为输入                
    for(i=0;i<8;i++)
    {
        dat<<=1;
		IIC_SCL_H(ID);
		IIC_Delay();
		if(IIC_SDA_READ(ID)==1)
			dat|=1;
		else
			dat|=0;
		IIC_SCL_L(ID);
		IIC_Delay();
    }
    IIC_SDA_OUT(ID);

    return dat;
}

/*****************************************************************************
 * 函 数 名  : IIC_WriteBuffer
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月25日
 * 函数功能  : 通过i2c向从机地址addr设备的reg寄存器写len个数据，数据内容为d-
               ata指针
 * 输入参数  : uint8_t ID      i2c0或者i2c1
               uint8_t addr    从机地址
               uint8_t reg     寄存器地址
               uint8_t len     写数据长度
               uint8_t * data  写数据内容
 * 输出参数  : TRUE FALSE
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
uint8_t IIC_WriteBuffer(uint8_t ID,uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data)
{
    int i;
    IIC_Start(ID);                  	 //起始信号

    IIC_Send_Byte(ID,addr| I2C_TX_FLAG);
    if (IIC_RecvACK(ID)==NAK) 
	{
        IIC_Stop(ID);                    //停止信号
        return FALSE;
    }
    IIC_Send_Byte(ID,reg);
    IIC_RecvACK(ID);
    for (i = 0; i < len; i++) 
	{
        IIC_Send_Byte(ID,data[i]);
		if (IIC_RecvACK(ID)==NAK) 
		{
             IIC_Stop(ID);               //停止信号
            return FALSE;
        }
    }
    IIC_Stop(ID);                    	//停止信号
    return TRUE;
}

/*****************************************************************************
 * 函 数 名  : IIC_ReadBuffer
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月25日
 * 函数功能  : 从i2c0或者1中的addr从机的reg寄存器连续读取len个数据，并将数据
               保存到buf中
 * 输入参数  : uint8_t ID    i2c0或者i2c1
               uint8_t addr  从机地址
               uint8_t reg   寄存器地址
               uint8_t len   读取数据长度
               uint8_t *buf  数据保存缓冲区
 * 输出参数  : TRUE FALSE
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
uint8_t IIC_ReadBuffer(uint8_t ID,uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    IIC_Start(ID);                   //起始信号
    
    IIC_Send_Byte(ID,addr | I2C_TX_FLAG);
    if (IIC_RecvACK(ID)==NAK) 
	{
        IIC_Stop(ID);                    //停止                                                                                                           信号
        return FALSE;
    }
    IIC_Send_Byte(ID,reg);
    IIC_RecvACK(ID);
	
    IIC_Start(ID);                   //起始信号
    IIC_Send_Byte(ID,addr | I2C_RX_FLAG);
    IIC_RecvACK(ID);
	
    while (len)
	{
        *buf = IIC_Read_Byte(ID);
        if (len == 1)
            IIC_SendACK(ID,1);
        else
            IIC_SendACK(ID,0);
        buf++;
        len--;
    }
    IIC_Stop(ID);
    return TRUE;
}

/*****************************************************************************
 * 函 数 名  : I2C0_Write
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月25日
 * 函数功能  : 通过i2c0设备 写len个数据到从机addr的reg寄存器
 * 输入参数  : uint8_t addr    从机地址
               uint8_t reg     寄存器地址
               uint8_t len     数据长度
               uint8_t * data  发送数据缓冲区
 * 输出参数  : TRUE FALSE
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
uint8_t I2C0_Write(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data)
{
	return IIC_WriteBuffer(SIMULATED_I2C0_ID,addr,reg,len,data);
}


/*****************************************************************************
 * 函 数 名  : I2C0_Read
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月25日
 * 函数功能  : 通过i2c0从addr设备的reg地址读len个数据保存到buf
 * 输入参数  : uint8_t addr  从机地址
               uint8_t reg   寄存器地址
               uint8_t len   读取长度
               uint8_t *buf  保存缓冲区
 * 输出参数  : TRUE FALSE
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
uint8_t I2C0_Read(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
	return IIC_ReadBuffer(SIMULATED_I2C0_ID,addr,reg,len,buf);
}

/*****************************************************************************
 * 函 数 名  : I2C1_Write
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月25日
 * 函数功能  : 通过i2c1设备 写len个数据到从机addr的reg寄存器
 * 输入参数  : uint8_t addr    从机地址
               uint8_t reg     寄存器地址
               uint8_t len     数据长度
               uint8_t * data  发送数据缓冲区
 * 输出参数  : TRUE FALSE
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/

uint8_t I2C1_Write(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data)
{
	return IIC_WriteBuffer(SIMULATED_I2C1_ID,addr,reg,len,data);
}

/*****************************************************************************
 * 函 数 名  : I2C1_Read
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月25日
 * 函数功能  : 通过i2c1从addr设备的reg地址读len个数据保存到buf
 * 输入参数  : uint8_t addr  从机地址
               uint8_t reg   寄存器地址
               uint8_t len   读取长度
               uint8_t *buf  保存缓冲区
 * 输出参数  : TRUE FALSE
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
uint8_t I2C1_Read(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
	return IIC_ReadBuffer(SIMULATED_I2C1_ID,addr,reg,len,buf);
}



