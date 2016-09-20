#ifndef _PIN_INT_H
#define _PIN_INT_H

#ifdef  PININT_GLOBALS
#define pin_int_ext
#else
#define pin_int_ext  extern
#endif

#include "board.h"
#include "config.h"


/* GPIO pin for PININT interrupt.   This is JP1.  Install JP1 to invoke interrupt. */
//#define TEST_INPUT_PORT         5	/* GPIO port number mapped to PININT */
//#define TEST_INPUT_PIN          7	/* GPIO pin number mapped to PININT */

//#define TEST_INPUT_PIN_PORT     2
//#define TEST_INPUT_PIN_BIT      8
//#define TEST_INPUT_MODE_FUNC    SCU_MODE_FUNC4
//#define PININT_INDEX   			0	/* PININT index used for GPIO mapping */

//#define PININT_IRQ_HANDLER  	GPIO0_IRQHandler	/* GPIO interrupt IRQ function name */
//#define PININT_NVIC_NAME    	PIN_INT0_IRQn	/* GPIO interrupt NVIC interrupt name */

#define PININT_INDEX0				0
#define PININT_INPUT_PORT        	0	/* GPIO port number mapped to PININT */
#define PININT_INPUT_PIN          	0	/* GPIO pin number mapped to PININT */
#define PININT_INPUT_PIN_PORT     	0
#define PININT_INPUT_PIN_BIT      	0
#define PININT_INPUT_MODE_FUNC    	SCU_MODE_FUNC0


#define PININT_INDEX1		1
#define PININT_INDEX2		2
#define PININT_INDEX3		3
#define PININT_INDEX4		4
#define PININT_INDEX5		5
#define PININT_INDEX6		6
#define PININT_INDEX7		7

#define KEY_UP				PININT_INDEX0
#define KEY_ACK				PININT_INDEX1
#define KEY_DOWN				PININT_INDEX2
#define KEY_LEFT				PININT_INDEX3
#define KEY_RIGHT				PININT_INDEX4
#define KEY_ENTER				PININT_INDEX5
#define KEY_CANNEL			PININT_INDEX6

#define KEY_EDGE_FALL		0	//下降沿
#define KEY_EDGE_RISE		1	//上升沿

#define SHORT_KEY_DELAY		20 //10 ms
#define LONG_KEY_DELAY		500 // 500ms 按键有效

#define LONG_KEY_VALID		400 //50 ms	发送一次按键

#define KEY_VALID_TIME_DELAY		20	//20 ms延时以上的上升沿和下降沿为有效按键，防抖处理，

typedef struct 
{
	uint8_t 	pinint_index;	//引脚中断标签
	uint8_t 	input_port;		// gpio端口
	uint8_t 	input_pin;		//gpio管脚
	uint8_t 	input_pin_port;	//符号
	uint8_t 	input_pin_bit;	//符号
	uint8_t 	input_mode_func;//类型
	IRQn_Type  	irqn;		//中断号
}PIN_INT_KB_T;



typedef struct
{
	uint8_t  	keyvalue:4;		//键值
	uint8_t		keyedge:4;		//key 上升下降沿 
	uint8_t     reserved[3]; 	//保留3字节
	
	uint32_t 	ticks;			//系统时钟
}KB_QUEUE_VALUE_T;


pin_int_ext void pin_interrupt_init(void);
pin_int_ext void pinint_init(PIN_INT_KB_T config);
pin_int_ext void KBD_dev_init(void);


#define KEY_UP_PORT			0
#define KEY_UP_PIN			0

#define KEY_ACK_PORT		0
#define KEY_ACK_PIN			1

#define KEY_DOWN_PORT		0
#define KEY_DOWN_PIN		2

#define KEY_LEFT_PORT		0
#define KEY_LEFT_PIN		3

#define KEY_RIGHT_PORT		0
#define KEY_RIGHT_PIN		12

#define KEY_ENTER_PORT		0
#define KEY_ENTER_PIN		13

#define KEY_CANCEL_PORT		0
#define KEY_CANCEL_PIN		15
#endif
