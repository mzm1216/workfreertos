#ifndef __IOCONFIG_h
#define __IOCONFIG_h

#ifdef  Io_GLOBALS
#define io_ext
#else
#define io_ext  extern
#endif

//#define RTU_IO_SPI0   // IO�� ģ��spi0
//#undef RTU_IO_SPI0

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

//p0.11 ��������SPIʱ����
#define PLL_TX_SCK_PORT 	MACRO_PORT0
#define PLL_TX_SCK_PIN		11

//p0.12 ��������SPI������
#define PLL_TX_DATA_PORT 	MACRO_PORT0
#define PLL_TX_DATA_PIN		12

//p0.14 ����SPI_RXƬѡ�ź�
#define PLL_TX_SEL_PORT 	MACRO_PORT0
#define PLL_TX_SEL_PIN		14

#ifdef RTU_IO_SPI0
#define  I3C_CH_NUM_OF_SPI		4// ioģ��spiͨ���� 4· tx��rx��l02 ref
#else
#define  I3C_CH_NUM_OF_SPI		3// ioģ��spiͨ���� 4· ��rx��l02 ref
#endif


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

//#define PS_TX_PIN1		27	//p0_28  ����ֻ��
#define PS_TX_PIN		28	//p0_28  ����ֻ��
#define ATT_ENA_PIN		16	//p1_16  ˥����ʹ��
#define NB_WB_PIN		17	//p1_17  ��խ��ʹ��

//#define RTU_PS_TX(a) 	{Chip_GPIO_SetPinState(LPC_GPIO,MACRO_PORT0,PS_TX_PIN1, (bool)a);Chip_GPIO_SetPinState(LPC_GPIO,MACRO_PORT0,PS_TX_PIN, (bool)a);}
#define RTU_PS_TX(a) 		Chip_GPIO_SetPinState(LPC_GPIO,MACRO_PORT0,PS_TX_PIN, (bool)a)
#define RTU_ATT_ENA(a) 	Chip_GPIO_SetPinState(LPC_GPIO,MACRO_PORT1,ATT_ENA_PIN, (bool)a)
#define RTU_NB_WB(a) 	Chip_GPIO_SetPinState(LPC_GPIO,MACRO_PORT1,NB_WB_PIN, (bool)a)



#endif
