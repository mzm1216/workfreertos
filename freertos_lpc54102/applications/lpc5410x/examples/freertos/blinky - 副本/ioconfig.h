#ifndef __IOCONFIG_h
#define __IOCONFIG_h

#ifdef  Io_GLOBALS
#define io_ext
#else
#define io_ext  extern
#endif
#define MACRO_PORT0			0
#define MACRO_PORT1			1
//p0.2 ��������SPIʱ����
#define PLL_RX_SCK_PORT 	MACRO_PORT0
#define PLL_RX_SCK_PIN		2

//p0.3 ��������SPI������
#define PLL_RX_DATA_PORT 	MACRO_PORT0
#define PLL_RX_DATA_PIN		3

//p0.4 ����SPI_RXƬѡ�ź�
#define PLL_RX_SEL_PORT 	MACRO_PORT0
#define PLL_RX_SEL_PIN		4

//p0.5 ����SPI_LO2Ƭѡ�ź�
#define PLL_RX_SEL2_PORT 	MACRO_PORT0
#define PLL_RX_SEL2_PIN		5

//p0.6 ����SPI_REFƬѡ�ź�
#define PLL_REF_SEL_PORT 	MACRO_PORT0
#define PLL_REF_SEL_PIN		6


//p0.23 I2C0ʱ���ź�
#define I2C0_SCK_PORT 		MACRO_PORT0
#define I2C0_SCK_PIN		23

//p0.24 I2C0�����ź�
#define I2C0_SDA_PORT 		MACRO_PORT0
#define I2C0_SDA_PIN		24

//p0.25 I2C1ʱ���ź�
#define I2C1_SCK_PORT 		MACRO_PORT0
#define I2C1_SCK_PIN		25

//p0.26 I2C1�����ź�
#define I2C1_SDA_PORT 		MACRO_PORT0
#define I2C1_SDA_PIN		26


#endif
