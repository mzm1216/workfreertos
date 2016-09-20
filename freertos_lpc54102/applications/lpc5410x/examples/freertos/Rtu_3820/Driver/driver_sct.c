

#include "board.h"



#define SCT_PWM            LPC_SCT

#define SCT_PWM_PIN_FAN_EN1        0		/* 风扇EN1 */
#define SCT_PWM_PIN_FAN_EN2        1		/* 风扇EN2 */
#define SCT_PWM_PIN_FAN_EN3        2		/* 风扇EN3*/
#define SCT_PWM_PIN_FAN_EN4        3		/* FAN_PWM*/

#define SCT_PWM_INDEX_FAN_EN1        1		/* 风扇EN1--INDEX */
#define SCT_PWM_INDEX_FAN_EN2        2		/* 风扇EN1--INDEX */
#define SCT_PWM_INDEX_FAN_EN3        3		/* 风扇EN1--INDEX */
#define SCT_PWM_INDEX_FAN_EN4        4		/* 风扇EN1--INDEX */

#define SCT_PWM_RATE   20000		/* PWM frequency 10 KHz */
#define TICKRATE_HZ     1000		/* 1 ms Tick rate */

#define LED_STEP_CNT      10		/* Change LED duty cycle every 20ms */
#define OUT_STEP_CNT    1000		/* Change duty cycle every 1 second */

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Setup board specific pin muxing */
static void PWM_setup_pin(void)
{

	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 7, IOCON_FUNC2 | IOCON_MODE_INACT | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 8, IOCON_FUNC2 | IOCON_MODE_INACT | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF);
#ifndef TEST_LED  //正式版本 将ifdef 改成 ifndef
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 9, IOCON_FUNC2 | IOCON_MODE_INACT | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 10, IOCON_FUNC2 | IOCON_MODE_INACT | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF);
#else	
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 29, IOCON_FUNC2 | IOCON_MODE_INACT | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF); 	   
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 30, IOCON_FUNC2 | IOCON_MODE_INACT | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF);
#endif
}


/*****************************************************************************
 * 函 数 名  : RTU_PWMDev_Init
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月25日
 * 函数功能  : 初始化 pwm,SCT0_COUT0-4,初始化占空比0%
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void RTU_PWMDev_Init()
{
	

	/* Initialize the SCT as PWM and set frequency */
	Chip_SCTPWM_Init(SCT_PWM);
	Chip_SCTPWM_SetRate(SCT_PWM, SCT_PWM_RATE);
	PWM_setup_pin();
	/* Use SCT0_OUT0-3 pin */
	Chip_SCTPWM_SetOutPin(SCT_PWM, SCT_PWM_INDEX_FAN_EN1, SCT_PWM_PIN_FAN_EN1);
	Chip_SCTPWM_SetOutPin(SCT_PWM, SCT_PWM_INDEX_FAN_EN2, SCT_PWM_PIN_FAN_EN2);
	Chip_SCTPWM_SetOutPin(SCT_PWM, SCT_PWM_INDEX_FAN_EN3, SCT_PWM_PIN_FAN_EN3);
	Chip_SCTPWM_SetOutPin(SCT_PWM, SCT_PWM_INDEX_FAN_EN4, SCT_PWM_PIN_FAN_EN4);

	/* Start with 0% duty cycle */
	Chip_SCTPWM_SetDutyCycle(SCT_PWM, SCT_PWM_INDEX_FAN_EN1, 0);
	Chip_SCTPWM_SetDutyCycle(SCT_PWM, SCT_PWM_INDEX_FAN_EN2, 0);
	Chip_SCTPWM_SetDutyCycle(SCT_PWM, SCT_PWM_INDEX_FAN_EN3, 0);
	Chip_SCTPWM_SetDutyCycle(SCT_PWM, SCT_PWM_INDEX_FAN_EN4, 0);
	Chip_SCTPWM_Start(SCT_PWM);
	
}




/* Example entry point */
int test_Pwm(void)
{
	uint32_t cnt1 = 0;
	int led_out_0, led_out_1, led_dp = 0, led_step = 1, currLed = 0;

	RTU_PWMDev_Init();
	SysTick_Config(SystemCoreClock / TICKRATE_HZ);

	while (1) {
		cnt1++;
		if (cnt1 >= LED_STEP_CNT) {
			led_dp += led_step;
			if (led_dp < 0) {
				led_dp = 0;
				led_step = 1;
			}
			if (led_dp > 100) {
				led_dp = 100;
				led_step = -1;
				currLed = 1 - currLed;
			}

			/* Increment or Decrement Dutycycle by 0.5% every 10ms */
			if (currLed) {
				led_out_0 = led_dp;
				led_out_1 = 100;
			}
			else {
				led_out_0 = 100;
				led_out_1 = led_dp;
			}
			Chip_SCTPWM_SetDutyCycle(SCT_PWM, 1,
									 Chip_SCTPWM_PercentageToTicks(SCT_PWM, led_out_0));
			Chip_SCTPWM_SetDutyCycle(SCT_PWM, 2,
									 Chip_SCTPWM_PercentageToTicks(SCT_PWM, led_out_1));
				Chip_SCTPWM_SetDutyCycle(SCT_PWM, 3,
									 Chip_SCTPWM_PercentageToTicks(SCT_PWM, led_out_0));
			Chip_SCTPWM_SetDutyCycle(SCT_PWM, 4,
									 Chip_SCTPWM_PercentageToTicks(SCT_PWM, led_out_1));
			cnt1 = 0;
		}
		__WFI();
	}
	return 0;
}

void sctpwm(uint8_t index,uint32_t level)
{
	Chip_SCTPWM_SetDutyCycle(SCT_PWM, index,Chip_SCTPWM_PercentageToTicks(SCT_PWM, level));
}