#ifndef __IOCONFIG_h
#define __IOCONFIG_h

#ifdef  Io_GLOBALS
#define io_ext
#else
#define io_ext  extern
#endif

//#define RTU_IO_SPI0   // IO扣 模拟spi0
//#undef RTU_IO_SPI0

#define MACRO_PORT0			0
#define MACRO_PORT1			1
//p0.2 共用三线SPI时钟线
#define PLL_RX_SCK_PORT 	MACRO_PORT0
#define PLL_RX_SCK_PIN		2

//p0.3 共用三线SPI数据线
#define PLL_RX_DATA_PORT 	MACRO_PORT0
#define PLL_RX_DATA_PIN		3

//p0.4 三线SPI_RX片选信号
#define PLL_RX_SEL_PORT 	MACRO_PORT0
#define PLL_RX_SEL_PIN		4

//p0.5 三线SPI_LO2片选信号
#define PLL_RX_SEL2_PORT 	MACRO_PORT0
#define PLL_RX_SEL2_PIN		5

//p0.6 三线SPI_REF片选信号
#define PLL_REF_SEL_PORT 	MACRO_PORT0
#define PLL_REF_SEL_PIN		6

//p0.11 发射三线SPI时钟线
#define PLL_TX_SCK_PORT 	MACRO_PORT0
#define PLL_TX_SCK_PIN		11

//p0.12 发射三线SPI数据线
#define PLL_TX_DATA_PORT 	MACRO_PORT0
#define PLL_TX_DATA_PIN		12

//p0.14 发射SPI_RX片选信号
#define PLL_TX_SEL_PORT 	MACRO_PORT0
#define PLL_TX_SEL_PIN		14

#ifdef RTU_IO_SPI0
#define  I3C_CH_NUM_OF_SPI		4// io模拟spi通道数 4路 tx、rx、l02 ref
#else
#define  I3C_CH_NUM_OF_SPI		3// io模拟spi通道数 4路 、rx、l02 ref
#endif


//p0.23 I2C0时钟信号
#define I2C0_SCK_PORT 		MACRO_PORT0
#define I2C0_SCK_PIN		23

//p0.24 I2C0数据信号
#define I2C0_SDA_PORT 		MACRO_PORT0
#define I2C0_SDA_PIN		24

//p0.25 I2C1时钟信号
#define I2C1_SCK_PORT 		MACRO_PORT0
#define I2C1_SCK_PIN		25

//p0.26 I2C1数据信号
#define I2C1_SDA_PORT 		MACRO_PORT0
#define I2C1_SDA_PIN		26

//#define PS_TX_PIN1		27	//p0_28  发射只能
#define PS_TX_PIN		28	//p0_28  发射只能
#define ATT_ENA_PIN		16	//p1_16  衰减器使能
#define NB_WB_PIN		17	//p1_17  快窄带使能

//#define RTU_PS_TX(a) 	{Chip_GPIO_SetPinState(LPC_GPIO,MACRO_PORT0,PS_TX_PIN1, (bool)a);Chip_GPIO_SetPinState(LPC_GPIO,MACRO_PORT0,PS_TX_PIN, (bool)a);}
#define RTU_PS_TX(a) 		Chip_GPIO_SetPinState(LPC_GPIO,MACRO_PORT0,PS_TX_PIN, (bool)a)
#define RTU_ATT_ENA(a) 	Chip_GPIO_SetPinState(LPC_GPIO,MACRO_PORT1,ATT_ENA_PIN, (bool)a)
#define RTU_NB_WB(a) 	Chip_GPIO_SetPinState(LPC_GPIO,MACRO_PORT1,NB_WB_PIN, (bool)a)



#endif
