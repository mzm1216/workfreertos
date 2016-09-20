#ifndef _UART3__H
#define _UART3__H

#ifdef  UART3_GLOBALS
#define uart3_ext
#else
#define uart3_ext  extern
#endif

#include "board.h"
#include "config.h"



/* Transmit and receive ring buffers */
uart3_ext RINGBUFF_T txring, rxring;

/* Transmit and receive ring buffer sizes */
#define UART_SRB_SIZE 256	/* Send */
#define UART_RRB_SIZE 128	/* Receive */

/* Transmit and receive buffers */
uart3_ext uint8_t rxbuff[UART_RRB_SIZE], txbuff[UART_SRB_SIZE];
//uart3_ext char rt_log_buf[UART_SRB_SIZE];

//uart3_ext  char inst1[] = "LPC18xx/43xx UART example using ring buffers\r\n";
//uart3_ext  char inst2[] = "Press a key to echo it back or ESC to quit\r\n";


/* Use UART3 for Keil boards */
#define LPC_UARTX       LPC_USART3
#define UARTx_IRQn      USART3_IRQn
#define UARTx_IRQHandler UART3_IRQHandler

#endif
