#ifndef __IOCONFIG_h
#define __IOCONFIG_h

#ifdef  Io_GLOBALS
#define io_ext
#else
#define io_ext  extern
#endif
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


#endif
