#ifndef _DRIVER_IIC_H_
#define _DRIVER_IIC_H_


#ifdef  IIC_GLOBALS
#define I2C_ext
#else
#define I2C_ext  extern
#endif

#include "board.h"
#include "config.h"

#define  I2C_TX_FLAG     	 ((uint8_t)0x00)
#define  I2C_RX_FLAG         ((uint8_t)0x01)

#define SIMULATED_I2C0_ID 	0
#define SIMULATED_I2C1_ID 	1
//IIC所有操作函数
#define ACK  	0
#define NAK		1
I2C_ext void IIC_IO_Init(uint8_t ID);
I2C_ext void IIC_Start(uint8_t ID);
I2C_ext void IIC_Stop(uint8_t ID);
I2C_ext void IIC_SendACK(uint8_t ID,unsigned char ack);
I2C_ext unsigned char IIC_RecvACK(uint8_t ID);
I2C_ext void IIC_Send_Byte(uint8_t ID,unsigned char dat);
I2C_ext unsigned char IIC_Read_Byte(uint8_t ID);


// IO模拟i2c 时钟线 端口和管脚   
I2C_ext uint8_t simulation_i2c_sck_port[2]={I2C0_SCK_PORT,I2C1_SCK_PORT};
I2C_ext uint8_t simulation_i2c_sck_pin[2]={I2C0_SCK_PIN,I2C1_SCK_PIN};

// IO模拟i2c 数据线 端口和管脚   
I2C_ext uint8_t simulation_i2c_sda_port[2]={I2C0_SDA_PORT,I2C1_SDA_PORT};
I2C_ext uint8_t simulation_i2c_sda_pin[2]={I2C0_SDA_PIN,I2C1_SDA_PIN};



#endif

