#ifndef _DRIVER_GPIOINT_H_
#define _DRIVER_GPIOINT_H_


#ifdef  GPIOINT_GLOBALS
#define gpioint_ext
#else
#define gpioint_ext  extern
#endif

#include "board.h"
#include "config.h"

/* GPIO pin for PININT interrupt */
#define GPIO_PININT_PIN     27	/* GPIO pin number mapped to PININT */
#define GPIO_PININT_PORT    0	/* GPIO port number mapped to PININT */
#define GPIO_PININT_INDEX   PININTSELECT0	/* PININT index used for GPIO mapping */
#define PININT_IRQ_HANDLER  PIN_INT0_IRQHandler	/* GPIO interrupt IRQ function name */
#define PININT_NVIC_NAME    PIN_INT0_IRQn	/* GPIO interrupt NVIC interrupt name */


/* Select a mode of type 'POWER_MODE_T' for this example: mode available are
   POWER_SLEEP, POWER_DEEP_SLEEP, POWER_POWER_DOWN*/
#define PDOWNMODE   (POWER_POWER_DOWN)

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
/* Saved clock source */
gpioint_ext  CHIP_SYSCON_MAINCLKSRC_T saved_clksrc;

/*****************************************************************************
 * Private functions
 ****************************************************************************/


#endif

