/*
 * @brief SPIS bus slave example using interrupt mode
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2014
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "board.h"
#include <stdlib.h>

/** @defgroup SPIS_INT_5410X SPI slave (interrupt mode) ROM API example
 * @ingroup EXAMPLES_ROM_5410X
 * @include "rom\spis_int\readme.txt"
 */

/**
 * @}
 */

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

#define LPC_SPIS_PORT         LPC_SPI0
#define LPC_IRQ_HANDLER             SPI0_IRQHandler
#define LPC_SPIIRQNUM         SPI0_IRQn
#define LPC_SPIS_CLOCK              SYSCON_CLOCK_SPI0
#define LPC_SPIS_RESET              RESET_SPI0

/* ROM driver handle for SPI slave */
static ROM_SPIS_HANDLE_T spisHandle;

/* SPI driver context area */
static uint32_t drvData[16];

#define BUFFSENDSIZE 128

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/* Made global so data can be seen in the debuuger */
uint8_t rx16[BUFFSENDSIZE], tx16[BUFFSENDSIZE];
ROM_SPIS_XFER_T sXfer;
volatile bool xferDone;

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Initializes pin muxing for SPI interface - note that SystemInit() may
   already setup your pin muxing at system startup */
static void Init_SPI_PinMux(void)
{
#if defined(BOARD_NXP_LPCXPRESSO_54102)
	/* 1.3 = SPI0_SCK, 0.14 = SPI0_SSELN0, 0.12 = SPI0_MOSI, 1.4 = SPI0_MISO */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 3,  (IOCON_FUNC5 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 14, (IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 12, (IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 4,  (IOCON_FUNC5 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));

#else
	/* Configure your own I2C pin muxing here if needed */
#warning "No I2C pin muxing defined"
#endif
}

/* Display error string and spin */
static void errorOut(char *errStr)
{
	DEBUGOUT(errStr);
	while (1) {}
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* SPI interrupt handler */
void LPC_IRQ_HANDLER(void)
{
	ROM_SPIS_TransferHandler(spisHandle);
}

void CBspiSlaveXferCSAssertCB(ROM_SPIS_HANDLE_T spisHandle, uint8_t slaveNum)
{
	/* Called when an enabled slave is asserted */
}

void CBspiSlaveXferCSDeAssertCB(ROM_SPIS_HANDLE_T spisHandle, ROM_SPIS_XFER_T *pXfer)
{
	/* Called when an enabled slave is de-asserted */
	bool *pXferDone = (bool *) ROM_SPIS_HANDLE_TOUDATA(spisHandle);

	*pXferDone = true;
}

/**
 * @brief	Main routine for SPI example
 * @return	Function should not exit
 */
int main(void)
{
	uint32_t memSize, *devMem;
	ROM_SPIS_INIT_T spisInit;
	ROM_SPIS_SLAVE_T spisConfig;
	int i;

	/* Generic Initialization */
	SystemCoreClockUpdate();
	Board_Init();

	/* Setup I2C pin muxing, enable S{I clock and reset I peripheral */
	Init_SPI_PinMux();
	Chip_Clock_EnablePeriphClock(LPC_SPIS_CLOCK);
	Chip_SYSCON_PeriphReset(LPC_SPIS_RESET);

	/* Get needed size for driver context memory */
	memSize = ROM_SPIS_GetMemSize();
	if (memSize > sizeof(drvData)) {
		errorOut("Can't allocate memory for SPIS driver context\r\n");
	}
	devMem = drvData;	/* Or just use malloc(memSize) */

	/* Initialize driver */
	spisInit.pUserData = (void *) &xferDone;
	spisInit.base = (uint32_t) LPC_SPIS_PORT;
	spisInit.spiPol[0] = 0;	/* Active low select for SSEL0 */
	spisInit.spiPol[1] = 0;
	spisInit.spiPol[2] = 0;
	spisInit.spiPol[3] = 0;
	spisHandle = ROM_SPIS_Init(devMem, &spisInit);
	if (spisHandle == NULL) {
		/* Error initializing SPI */
		errorOut("Error initializing ROM\r\n");
	}

	/* Set SPI transfer configuration */
	spisConfig.mode = ROM_SPI_CLOCK_MODE0;
	spisConfig.lsbFirst = 0;
	spisConfig.dataBits = 8;
	if (ROM_SPIS_SetupSlave(spisHandle, &spisConfig) != LPC_OK) {
		errorOut("SPI configuration is invalid\r\n");
	}

	/* Callback registration  */
	ROM_SPIS_RegisterCallback(spisHandle, ROM_SPIS_ASSERTSSEL_CB, (void *) CBspiSlaveXferCSAssertCB);
	ROM_SPIS_RegisterCallback(spisHandle, ROM_SPIS_DEASSERTSSEL_CB, (void *) CBspiSlaveXferCSDeAssertCB);

	DEBUGSTR("SPI slave example\r\n");

	/* Enable the interrupt for the SPI */
	NVIC_EnableIRQ(LPC_SPIIRQNUM);

	/* Read data as fast as possible in loop */
	while (1) {
		/* Setup transfer buffers and maximum transfer sizes, no transfer flags */
		sXfer.txBuff = tx16;
		sXfer.txSz = BUFFSENDSIZE;
		sXfer.rxBuff = rx16;
		sXfer.rxSz = BUFFSENDSIZE;
		sXfer.flags = 0;

		/* Start transfer. Will return immediately */
		xferDone = false;
		ROM_SPIS_Transfer(spisHandle, &sXfer);

		/* Can monitor transfer status or completion flag to determine
		   end of transfer */
		while (xferDone == false) {
			/* Can sleep while waiting for SPI interrupts and end of trnasfer */
			__WFI();
		}

		/* Check status of the transfer */
		if (sXfer.status != LPC_OK) {
			DEBUGOUT("-Error performing transfer = %x\r\n", sXfer.status);
		}
		else {
			DEBUGOUT("-SPI transfer completed: status = %x\r\n", sXfer.status);
			for (i = 0; i < sXfer.rxRecv; i++) {
				DEBUGOUT("T:%04x R:%04x : ", tx16[i], rx16[i]);
			}
			DEBUGSTR("\r\n");
		}
		for (i = 0; i < sXfer.rxRecv; i++) {
			tx16[i] = rx16[i];
		}
	}

	/* Code never reaches here. Only used to satisfy standard main() */
	return 0;
}
