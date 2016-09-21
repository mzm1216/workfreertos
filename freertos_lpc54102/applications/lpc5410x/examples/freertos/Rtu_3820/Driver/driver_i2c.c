/***********************************************************************************
 * �� �� ��   : driver_i2c.c
 * �� �� ��   : MZM
 * ��������   : 2016��2��25��
 * �ļ�����   : ��IO��ģ��I2C0 ��I2C1����Ҫ������I2C��IO�ڣ���Ӧ���óɹ��ܿ�
                ��Ҫ������ͨgpio
 * ��Ȩ˵��   : Copyright (c) 2008-2016   �������������ͨѶ�Ƽ����޹�˾
 * ��    ��   : 
 * �޸���־   : 
***********************************************************************************/
#define IIC_GLOBALS
#include "driver_i2c.h"
//#include "config.h"



//����ʱ���� ���
void IIC_SCL_OUT(uint8_t ID)
{
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, simulation_i2c_sck_port[ID], simulation_i2c_sck_pin[ID]);
}
//����ʱ���� ����
void IIC_SCL_IN(uint8_t ID)
{
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, simulation_i2c_sck_port[ID], simulation_i2c_sck_pin[ID]);
}
//���� ʱ����Ϊ�ߵ�ƽ
void IIC_SCL_H(uint8_t ID)
{
	Chip_GPIO_SetPinState(LPC_GPIO, simulation_i2c_sck_port[ID], simulation_i2c_sck_pin[ID],1);
}
//���� ʱ����Ϊ�͵�ƽ
void IIC_SCL_L(uint8_t ID)
{
	Chip_GPIO_SetPinState(LPC_GPIO, simulation_i2c_sck_port[ID], simulation_i2c_sck_pin[ID],0);
}

//���������� Ϊ ���
void IIC_SDA_OUT(uint8_t ID)
{
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, simulation_i2c_sda_port[ID], simulation_i2c_sda_pin[ID]);
}
//���������� Ϊ ����
void IIC_SDA_IN(uint8_t ID)
{
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, simulation_i2c_sda_port[ID], simulation_i2c_sda_pin[ID]);
}
//���� ������Ϊ  ��  ��ƽ
void IIC_SDA_H(uint8_t ID)
{
	Chip_GPIO_SetPinState(LPC_GPIO, simulation_i2c_sda_port[ID], simulation_i2c_sda_pin[ID],1);
}
//���� ������Ϊ  ��  ��ƽ
void IIC_SDA_L(uint8_t ID)
{
	Chip_GPIO_SetPinState(LPC_GPIO, simulation_i2c_sda_port[ID], simulation_i2c_sda_pin[ID],0);
}

//��ȡ�����ߵ�ֵ
uint8_t IIC_SDA_READ(uint8_t ID)
{
	return Chip_GPIO_ReadPortBit(LPC_GPIO, simulation_i2c_sda_port[ID], simulation_i2c_sda_pin[ID]);
}

//����ʱ
void IIC_Delay()
{
	int i;
	for(i=0;i<40;i++);
	
}


/*****************************************************************************
 * �� �� ��  : RTU_I2CDev_Init
 * �� �� ��  : MZM
 * ��������  : 2016��2��25��
 * ��������  : ��ʼ�� I2C 0 ��I2c1
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void RTU_I2CDev_Init()
{
	IIC_IO_Init(SIMULATED_I2C0_ID);//��ʼ�� i2c0
	IIC_IO_Init(SIMULATED_I2C1_ID);//��ʼ�� i2c1
}


/*****************************************************************************
 * �� �� ��  : IIC_IO_Init
 * �� �� ��  : MZM
 * ��������  : 2016��2��25��
 * ��������  : ����I2C�� IO���鷽��
 * �������  : uint8_t ID  0===i2c0  1==i2c1
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void IIC_IO_Init(uint8_t ID)
{			
	IIC_SCL_OUT(ID);
	IIC_SDA_OUT(ID);
}

//����һ��start�ź�
void IIC_Start(uint8_t ID)
{
	IIC_SDA_OUT(ID);     	//����sda�����
	
	IIC_SDA_H(ID);	  	  	// 1
	IIC_SCL_H(ID);			// 1
	IIC_Delay();
	
 	IIC_SDA_L(ID);			//0  START:when CLK is high,DATA change form high to low 
	IIC_Delay();
	
	IIC_SCL_L(ID);			//��סI2C���ߣ�׼�����ͻ�������� 
}

//����һ��ֹͣ�ź�
void IIC_Stop(uint8_t ID)
{
	IIC_SDA_OUT(ID);  		//����sda�����
	
	IIC_SDA_L(ID);
	IIC_SCL_H(ID);
 	IIC_Delay(); 
	
	IIC_SDA_H(ID);
	IIC_Delay();							   	
}
/*****************************************************************************
 * �� �� ��  : IIC_SendACK
 * �� �� ��  : MZM
 * ��������  : 2016��2��25��
 * ��������  : ����һ��Ӧ���ź�
 * �������  : uint8_t ID   i2c
               uint8_t ack   (0:ACK 1:NAK)
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void IIC_SendACK(uint8_t ID,uint8_t ack)
{
	if(ack==1)						//дӦ���ź�
	{
		IIC_SDA_H(ID);
	}
	else
	{
		IIC_SDA_L(ID);
	}
    IIC_SCL_H(ID);                  //����ʱ����
    IIC_Delay();                 	//��ʱ
    IIC_SCL_L(ID);                  //����ʱ����
    IIC_Delay();                 	//��ʱ
}
/*****************************************************************************
 * �� �� ��  : IIC_RecvACK
 * �� �� ��  : MZM
 * ��������  : 2016��2��25��
 * ��������  : ����һ��Ӧ���ź�
 * �������  : uint8_t ID   i2c
 * �������  : (0:ACK 1:NAK)
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
uint8_t IIC_RecvACK(uint8_t ID)
{

	IIC_SDA_IN(ID);
    IIC_SCL_H(ID);               //����ʱ����
    IIC_Delay();                 //��ʱ
    if(IIC_SDA_READ(ID))
	{
		IIC_SCL_L(ID);
		return NAK;
	}                			//��Ӧ���ź�
    IIC_SCL_L(ID);              //����ʱ����
    IIC_Delay();                //��ʱ
    return ACK;
	
	
	
}

/*****************************************************************************
 * �� �� ��  : IIC_Send_Byte
 * �� �� ��  : MZM
 * ��������  : 2016��2��25��
 * ��������  : ͨ��i2c����һ������ data
 * �������  : uint8_t ID   i2c0����i2c1
               uint8_t dat  ��������
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

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
 * �� �� ��  : IIC_Read_Byte
 * �� �� ��  : MZM
 * ��������  : 2016��2��25��
 * ��������  : ͨ��i2c��ȡһ���ֽڵ�����
 * �������  : uint8_t ID  i2c0����i2c1
 * �������  : ��ȡ����
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
uint8_t IIC_Read_Byte(uint8_t ID)
{
	uint8_t i,dat=0;
	IIC_SDA_IN(ID);//SDA����Ϊ����                
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
 * �� �� ��  : IIC_WriteBuffer
 * �� �� ��  : MZM
 * ��������  : 2016��2��25��
 * ��������  : ͨ��i2c��ӻ���ַaddr�豸��reg�Ĵ���дlen�����ݣ���������Ϊd-
               ataָ��
 * �������  : uint8_t ID      i2c0����i2c1
               uint8_t addr    �ӻ���ַ
               uint8_t reg     �Ĵ�����ַ
               uint8_t len     д���ݳ���
               uint8_t * data  д��������
 * �������  : TRUE FALSE
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
uint8_t IIC_WriteBuffer(uint8_t ID,uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data)
{
    int i;
    IIC_Start(ID);                  	 //��ʼ�ź�

    IIC_Send_Byte(ID,addr| I2C_TX_FLAG);
    if (IIC_RecvACK(ID)==NAK) 
	{
        IIC_Stop(ID);                    //ֹͣ�ź�
        return FALSE;
    }
    IIC_Send_Byte(ID,reg);
    IIC_RecvACK(ID);
    for (i = 0; i < len; i++) 
	{
        IIC_Send_Byte(ID,data[i]);
		if (IIC_RecvACK(ID)==NAK) 
		{
             IIC_Stop(ID);               //ֹͣ�ź�
            return FALSE;
        }
    }
    IIC_Stop(ID);                    	//ֹͣ�ź�
    return TRUE;
}

/*****************************************************************************
 * �� �� ��  : IIC_ReadBuffer
 * �� �� ��  : MZM
 * ��������  : 2016��2��25��
 * ��������  : ��i2c0����1�е�addr�ӻ���reg�Ĵ���������ȡlen�����ݣ���������
               ���浽buf��
 * �������  : uint8_t ID    i2c0����i2c1
               uint8_t addr  �ӻ���ַ
               uint8_t reg   �Ĵ�����ַ
               uint8_t len   ��ȡ���ݳ���
               uint8_t *buf  ���ݱ��滺����
 * �������  : TRUE FALSE
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
uint8_t IIC_ReadBuffer(uint8_t ID,uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    IIC_Start(ID);                   //��ʼ�ź�
    
    IIC_Send_Byte(ID,addr | I2C_TX_FLAG);
    if (IIC_RecvACK(ID)==NAK) 
	{
        IIC_Stop(ID);                    //ֹͣ                                                                                                           �ź�
        return FALSE;
    }
    IIC_Send_Byte(ID,reg);
    IIC_RecvACK(ID);
	
    IIC_Start(ID);                   //��ʼ�ź�
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
 * �� �� ��  : I2C0_Write
 * �� �� ��  : MZM
 * ��������  : 2016��2��25��
 * ��������  : ͨ��i2c0�豸 дlen�����ݵ��ӻ�addr��reg�Ĵ���
 * �������  : uint8_t addr    �ӻ���ַ
               uint8_t reg     �Ĵ�����ַ
               uint8_t len     ���ݳ���
               uint8_t * data  �������ݻ�����
 * �������  : TRUE FALSE
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
uint8_t I2C0_Write(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data)
{
	return IIC_WriteBuffer(SIMULATED_I2C0_ID,addr,reg,len,data);
}


/*****************************************************************************
 * �� �� ��  : I2C0_Read
 * �� �� ��  : MZM
 * ��������  : 2016��2��25��
 * ��������  : ͨ��i2c0��addr�豸��reg��ַ��len�����ݱ��浽buf
 * �������  : uint8_t addr  �ӻ���ַ
               uint8_t reg   �Ĵ�����ַ
               uint8_t len   ��ȡ����
               uint8_t *buf  ���滺����
 * �������  : TRUE FALSE
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
uint8_t I2C0_Read(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
	return IIC_ReadBuffer(SIMULATED_I2C0_ID,addr,reg,len,buf);
}

/*****************************************************************************
 * �� �� ��  : I2C1_Write
 * �� �� ��  : MZM
 * ��������  : 2016��2��25��
 * ��������  : ͨ��i2c1�豸 дlen�����ݵ��ӻ�addr��reg�Ĵ���
 * �������  : uint8_t addr    �ӻ���ַ
               uint8_t reg     �Ĵ�����ַ
               uint8_t len     ���ݳ���
               uint8_t * data  �������ݻ�����
 * �������  : TRUE FALSE
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/

uint8_t I2C1_Write(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data)
{
	return IIC_WriteBuffer(SIMULATED_I2C1_ID,addr,reg,len,data);
}

/*****************************************************************************
 * �� �� ��  : I2C1_Read
 * �� �� ��  : MZM
 * ��������  : 2016��2��25��
 * ��������  : ͨ��i2c1��addr�豸��reg��ַ��len�����ݱ��浽buf
 * �������  : uint8_t addr  �ӻ���ַ
               uint8_t reg   �Ĵ�����ַ
               uint8_t len   ��ȡ����
               uint8_t *buf  ���滺����
 * �������  : TRUE FALSE
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
uint8_t I2C1_Read(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
	return IIC_ReadBuffer(SIMULATED_I2C1_ID,addr,reg,len,buf);
}



