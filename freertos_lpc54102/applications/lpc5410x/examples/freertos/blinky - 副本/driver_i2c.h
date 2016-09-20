#ifndef _DRIVER_IIC_H_
#define _DRIVER_IIC_H_
#include "board.h"
#include "config.h"

#define PCA9535_DEVICE_ADDR			0x40
#define SE98A_DEVICE0_ADDR			0x30
#define SE98A_DEVICE1_ADDR			0x31

#define I2C1_TO_IOS_DEVICD_ADDR 	PCA9535_DEVICE_ADDR
#define I2C1_TO_TEMP_DEVICE0_ADDR 	SE98A_DEVICE0_ADDR
#define I2C1_TO_TEMP_DEVICE1_ADDR 	SE98A_DEVICE1_ADDR

#define  I2C_TX_FLAG     	 ((uint8_t)0x00)
#define  I2C_RX_FLAG         ((uint8_t)0x01)

#define SIMULATED_I2C0_ID 	0
#define SIMULATED_I2C1_ID 	1
//IIC所有操作函数
#define ACK  	0
#define NAK		1
void IIC_IO_Init(uint8_t ID);
void IIC_Start(uint8_t ID);
void IIC_Stop(uint8_t ID);
void IIC_SendACK(uint8_t ID,unsigned char ack);
unsigned char IIC_RecvACK(uint8_t ID);
void IIC_Send_Byte(uint8_t ID,unsigned char dat);
unsigned char IIC_Read_Byte(uint8_t ID);





#endif

