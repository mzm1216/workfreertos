#ifndef _DRIVER_UART_h
#define _DRIVER_UART_h

#ifdef  UART_GLOBALS
#define uart_ext
#else
#define uart_ext  extern
#endif

#include "board.h"
#include "config.h"
#include "ring_buffer.h"

#define ABS(x) ((int) (x) < 0 ? -(x) : (x))

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

typedef struct UART_ISR_STATISTICStag {
	uint32_t rx_rdy;			/* receive character ready */
	uint32_t tx_rdy;			/* transmitter ready (shift register empty) */
	uint32_t tx_idle;			/* transmitter idle (shift and transmit buffer empty) */
	uint32_t tx_dis;			/* transmitter disabled */
	uint32_t er_frame;			/* framing error */
	uint32_t er_parity;			/* parity error */
	uint32_t er_auto_baud;		/* auto baud error */
	uint32_t er_rx_noise;		/* receive line noise error */
	uint32_t er_overrun;		/* over-run error */
	uint32_t stat_cts;			/* CTS status change error */
	uint32_t stat_break;		/* BREAK received */
	uint32_t stat_start;		/* start bit detected */
} UART_ISR_STATISTICS;

/* UART statistics */
uart_ext UART_ISR_STATISTICS u_stat;

/* UART Driver context memory */
uart_ext uint32_t drv_mem[16];

/* UART ROM Driver Handle */
uart_ext UART_HANDLE_T hUART;

#define UART_ID             0			/* UART used for example */
#define UART_BAUD_RATE      115200		/* Required UART Baud rate (tested at 921600, 460800 and 115200) */
#define UART_BUAD_ERR       1			/* Percentage of Error allowed in baud */

/* ring buffer data */
#define RB_COUNT            256			/* Ring buffer count */
uart_ext  uint8_t rx_buff[RB_COUNT];		/* Receive ring buffer */
uart_ext  uint8_t tx_buff[RB_COUNT];		/* Transmit ring buffer */
uart_ext  uint8_t io_buff[RB_COUNT];		/* I/O buffer for interrupts */
uart_ext  RINGBUFF_T  rx_rb, tx_rb;		/* Ring buffer structures */

/*	Event flags */
#define EVENT_RX_TH         0x01		/* set when rx fifo threshold interrupt is processed */
#define EVENT_RX_EMPTY      0x02		/* set when rx fifo empty is detected (not an interrupt)*/
#define EVENT_TX_TH         0x04		/* set when tx fifo threshold interrupt is processed */
#define EVENT_TX_EMPTY      0x08		/* set when tx fifo empty is detected (not an interrupt)*/
#define EVENT_BUS_ERROR     0x10		/* set when bus error is detected */
uart_ext volatile uint32_t event_flags;	/* event_flags: set in interrupt, cleared in main loop */

typedef struct 
{
	
	uint8_t rx_buf[RB_COUNT];		/* Receive ring buffer */
	uint32_t savecnt;
	uint32_t readcnt;
	xQueueHandle Sem_uart_recv_data;		//数据处理信号量
	xQueueHandle Sem_uart_protect_readcnt;	//数据数据指针互斥处理信号量
	
} RTU_UART_DEV_T;

uart_ext  RTU_UART_DEV_T rtu_uart_dev;
uart_ext RTU_UART_DEV_T * RTU_Get_Uart_Dev();
uart_ext uint32_t RTU_Uart0_Read(uint8_t *tx_buff, uint32_t len);


#endif
