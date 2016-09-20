#ifndef _IIC_DEV_H_
#define _IIC_DEV_H_


#ifdef  IIC_DEV_G
#define I2Cdev_ext
#else
#define I2Cdev_ext  extern
#endif

#include "board.h"
#include "config.h"

#define SE98A_DEVICE0_ADDR			0x30 	//0x30|A0-A2=0
#define SE98A_DEVICE1_ADDR			0x32	//0x30|A0-A2==1
#define PCA9535_DEVICE_ADDR			0x40	//0x40|A0-A2==2

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

/*  SE98A REG--MAP */
#define 	SE98A_CAPABILITY_REG		0x00		
#define 	SE98A_CONFIG_REG			0x01
#define 	SE98A_UPPER_ALARM			0x02
#define 	SE98A_LOWER_ALARM			0x03
#define 	SE98A_CRITICAL_ALARM		0x04
#define 	SE98A_TEMP_REG				0x05
#define 	SE98A_MANU_ID_REG			0x06
#define 	SE98A_DEVICD_ID_REG			0x07
#define 	SE98A_SMBUS_REG				0x22


/* PCA9535 REG_MAP */
#define PCA9535_READ_INPUT_PORT0 				0x00	//读取输入端口0状态
#define PCA9535_READ_INPUT_PORT1 				0x01	//读取输入端口1状态
#define PCA9535_SET_OUTPUT_PORT0 				0x02	//设置输出端口0状态
#define PCA9535_SET_OUTPUT_PORT1 				0x03	//设置输出端口1状态
#define PCA9535_POLARITY_INVERSION_PORT0 		0x04	//设置端口0的极性反转寄存器
#define PCA9535_POLARITY_INVERSION_PORT1 		0x05	//设置端口1的极性反转寄存器
#define PCA9535_CONFIG_PORT0 					0x06	//配置端口0
#define PCA9535_CONFIG_PORT1 					0x07	//配置端口1

#define IO0_0	0
#define IO0_1	1
#define IO0_2	2
#define IO0_3	3
#define IO0_4	4
#define IO0_5	5
#define IO0_6	6
#define IO0_7	7

#define IO1_0	0
#define IO1_1	1
#define IO1_2	2
#define IO1_3	3
#define IO1_4	4
#define IO1_5	5
#define IO1_6	6
#define IO1_7	7

#define PS_TXVCO_H	0
#define PS_TXVCO_L	1
#define PS_RXVCO_H  2
#define PS_RXVCO_L  3
#define TXCV_READ	4
#define RXCV_READ	5
#define REFCV_READ	6
#define EXT_CLK		7
#define TXCH_SLOW	8
#define TXCH_FAST	9
#define RXCH_SLOW	10
#define RXCH_FAST	11
#define PLL_TX_LD	12	//输入-tx锁定
#define PLL_RX_LD	13	//输入-rx锁定
#define PLL_RX_LD2	14	//输入-rx2锁定
#define PLL_REF_LD	15	//输入-ref锁定

#define SELECT_TX_VCO_H0L1	0	
#define SELECT_TX_VCO_H1L0	1

#define SELECT_RX_VCO_H0L1	0	
#define SELECT_RX_VCO_H1L0	1

#define SET_REF_VCO_EXT_CLK_0	0
#define SET_REF_VCO_EXT_CLK_1	1

#define SET_CV_ADC_VALUE_MARK	(0x7 << 4)    	//ad读取 3个io
#define SET_CV_ADC_VALUE_TXCV	(0x1 << 4)		//读发射锁相环CP电压使能
#define SET_CV_ADC_VALUE_RXCV	(0x1 << 5)		//读接收锁相环CP电压使能
#define SET_CV_ADC_VALUE_REFCV	(0x1 << 6)		//读10M基准时钟锁相环CP电压使能

#define	TX_VCO_LIMIT_FREQ   435000000		//400 - 430 M 选择txvco h0l1  430--470 选择 h1lo
#define	RX_VCO_LIMIT_FREQ   435000000		//400 - 430 M 选择txvco h0l1  430--470 选择 h1lo

typedef struct{

	/* 发射vco控制 400-430  H0--L1,430-470 H1--L0*/
	uint8_t txvoc_h:1;		//port 0:0--
	uint8_t txvoc_l:1;		//port 0:1

	/*接收vco控制  326.65-356.65 H0_L1,356-396 H1-L0*/
	uint8_t rxvoc_h:1;		//port 0:2
	uint8_t rxvoc_l:1;		//port 0:3
	/* */
	uint8_t txcv_read:1;	//port 0:4 读发射锁相环CP电压使能
	uint8_t rxcv_read:1;	//port 0:5读接收锁相环CP电压使能
	uint8_t refcv_read:1;	//port 0:6读10M基准时钟锁相环CP电压使能
	
	uint8_t ext_clk:1;		//port 0:7有外部10M基准时钟时为1，没有时为0

	uint8_t tx_ch_slow:1;	//port 1:0发射正常锁定设置
	uint8_t tx_ch_fast:1;	//port 1:1发射快速锁定设置
	uint8_t rx_ch_slow:1;	//port 1:2接收正常锁定设置
	uint8_t rx_ch_fast:1;	//port 1:3接收快速锁定设置
	
	uint8_t tx_LD:1;		//port 1:4发射锁定检测
	uint8_t rx_LD:1;		//port 1:5接收一本振锁定检测
	uint8_t rx_LD2:1;		//port 1:6接收二本振锁定检测
	uint8_t ref_LD:1;		//port 1:710M基准时钟锁定检测
	
}RTU_PCA9535_PORT_STRUCT;

I2Cdev_ext RTU_PCA9535_PORT_STRUCT rtu_pca9535_port;
I2Cdev_ext void RTU_SE98_PCA9535_Init(void);
I2Cdev_ext int32_t RTU_Get_Temp(uint8_t addr);

#endif

