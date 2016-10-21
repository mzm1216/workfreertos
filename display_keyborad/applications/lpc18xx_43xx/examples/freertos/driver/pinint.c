
#define PININT_GLOBALS
#include "apptask.h"
#include "pinint.h"

#ifndef USED_SSD1322
PIN_INT_KB_T pin_int_config[8]=
{

#ifdef USED_SSD2119
	//pinindex-  	  端口 管脚-  符号- 符号- 类型-   中断号
	{PININT_INDEX0,		3,	 0,		6,	1,		0,	PIN_INT0_IRQn},// 	up
	{PININT_INDEX1,		3,	 1,		6,	 2,		0,	PIN_INT1_IRQn},//	ack
	{PININT_INDEX2,		0,	 15,		1,	20,		0,	PIN_INT2_IRQn},//	down
	{PININT_INDEX3,		0,	 13,		1,	18,		0,	PIN_INT3_IRQn},//	left
	{PININT_INDEX4,		0,	12,		1,	17,		0,	PIN_INT4_IRQn},//	right
	{PININT_INDEX5,		3,	 4,		6,	5,		0,	PIN_INT5_IRQn},//	enter
	{PININT_INDEX6,		3,	3,		6,	4,		0,	PIN_INT6_IRQn},//	cancel
	{	 0xff	  ,			3,	3,		6,	4,		0,	PIN_INT6_IRQn}

#else
	/*便携式的 按键跟 tftssd2119的按键布局不一致*/
	//pinindex-  	  端口 管脚-  符号- 符号- 类型-   中断号

	{PININT_INDEX0,		3,	1,		6,	2,		0,	PIN_INT0_IRQn},// 	up
	{PININT_INDEX1,		0,	12,		1,	17,		0,	PIN_INT1_IRQn},//	ack
	{PININT_INDEX2,		0,	 13,		1,	18,		0,	PIN_INT2_IRQn},//	down
	{PININT_INDEX3,		0,	 15,		1,	20,		0,	PIN_INT3_IRQn},//	left
	{PININT_INDEX4,		3,	 0,		6,	1,		0,	PIN_INT4_IRQn},//	right
	{PININT_INDEX5,		3,	 4,		6,	5,		0,	PIN_INT5_IRQn},//	enter
	{PININT_INDEX6,		3,	3,		6,	4,		0,	PIN_INT6_IRQn},//	cancel
	{	 0xff	  ,			3,	3,		6,	4,		0,	PIN_INT6_IRQn}

	#endif
};

#else
PIN_INT_KB_T pin_int_config[8]=
{
	//pinindex-  	  端口 管脚-  符号- 符号- 类型-   中断号
	{PININT_INDEX0,		0,	 0,		0,	 0,		0,	PIN_INT0_IRQn},// 	up
	{PININT_INDEX1,		0,	 1,		0,	 1,		0,	PIN_INT1_IRQn},//	ack
	{PININT_INDEX2,		0,	 2,		1,	15,		0,	PIN_INT2_IRQn},//	down
	{PININT_INDEX3,		0,	 3,		1,	16,		0,	PIN_INT3_IRQn},//	left
	{PININT_INDEX4,		0,	12,		1,	17,		0,	PIN_INT4_IRQn},//	right
	{PININT_INDEX5,		0,	13,		1,	18,		0,	PIN_INT5_IRQn},//	enter
	{PININT_INDEX6,		0,	15,		1,	20,		0,	PIN_INT6_IRQn},//	cancel
	{	 0xff	  ,		0,	15,		1,	20,		0,	PIN_INT6_IRQn}
};
#endif


void check_irq_send_key_queue(uint8_t Key)
{
	KB_QUEUE_VALUE_T key_queue;
	uint32_t intreg;
	long xCoRoutinePreviouslyWoken;

	/*中断是沿触发模式，不是上升沿，就是下降沿，所以只判断一个沿即可。*/
	intreg = Chip_PININT_GetRiseStates(LPC_GPIO_PIN_INT);// 获取上升沿

	if(intreg & PININTCH(Key))
	{
		key_queue.keyedge = KEY_EDGE_RISE;//上升沿
	}
	else
	{
		key_queue.keyedge = KEY_EDGE_FALL;//下降沿
	}
	
	key_queue.keyvalue = Key;
	
	xQueueSendFromISR(app_task_get_key_queue,&key_queue,&xCoRoutinePreviouslyWoken);//
	portYIELD_FROM_ISR(xCoRoutinePreviouslyWoken);

}

/**
 * @brief	Handle interrupt from GPIO pin or GPIO pin mapped to PININT
 * @return	Nothing
 */
void GPIO0_IRQHandler(void)
{
	check_irq_send_key_queue(KEY_UP);
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(KEY_UP));
//	Board_LED_Toggle(0);
}
void GPIO1_IRQHandler(void)
{
	check_irq_send_key_queue(KEY_ACK);
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(KEY_ACK));

}

void GPIO2_IRQHandler(void)
{
	check_irq_send_key_queue(KEY_DOWN);
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(KEY_DOWN));

}

void GPIO3_IRQHandler(void)
{
	check_irq_send_key_queue(KEY_LEFT);
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(KEY_LEFT));

}
void GPIO4_IRQHandler(void)
{
	check_irq_send_key_queue(KEY_RIGHT);
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(KEY_RIGHT));

}
void GPIO5_IRQHandler(void)
{
	check_irq_send_key_queue(KEY_ENTER);
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(KEY_ENTER));

}
void GPIO6_IRQHandler(void)
{
	check_irq_send_key_queue(KEY_CANNEL);
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(KEY_CANNEL));

}


void pinint_init(PIN_INT_KB_T config)
{

	/* Set pin back to GPIO (on some boards may have been changed to something
	   else by Board_Init()) */
	Chip_SCU_PinMuxSet(config.input_pin_port, config.input_pin_bit,
					   (SCU_MODE_INBUFF_EN | SCU_MODE_INACT | config.input_mode_func) );

	/* Configure GPIO pin as input */
	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, config.input_port, config.input_pin);

	/* Configure interrupt channel for the GPIO pin in SysCon block */
	Chip_SCU_GPIOIntPinSel(config.pinint_index, config.input_port, config.input_pin);

	/* Configure channel interrupt as edge sensitive and falling edge interrupt */
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(config.pinint_index));
	Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH(config.pinint_index));	// 边沿触发
	Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH(config.pinint_index));		// 下降沿触发
	Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH(config.pinint_index));	// 上升沿触发
	

	/* Enable interrupt in the NVIC */
	NVIC_ClearPendingIRQ(config.irqn);
	NVIC_EnableIRQ(config.irqn);

}

//void Key_gpio_init()
//{
//	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, KEY_UP_PORT, KEY_UP_PIN);
//	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, KEY_ACK_PORT, KEY_ACK_PIN);
//	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, KEY_DOWN_PORT, KEY_DOWN_PIN);	

//	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, KEY_LEFT_PORT, KEY_LEFT_PIN);
//	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, KEY_RIGHT_PORT, KEY_RIGHT_PIN);
//	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, KEY_ENTER_PORT, KEY_ENTER_PIN);
//	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, KEY_CANCEL_PORT, KEY_CANCEL_PIN);


//}
void pin_interrupt_init()
{
	uint8_t i;

	for(i=0;i<7;i++)
	{
		pinint_init(pin_int_config[i]);
	}

}
