/*
 * @brief I2C bus slave example using interrupt mode
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

/** @defgroup I2CS_INT_5410X I2C slave (interrupt) ROM API example
 * @ingroup EXAMPLES_ROM_5410X
 * @include "rom\i2cs_int\readme.txt"
 */

/**
 * @}
 */

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/** I2C setup */
#define I2C_ADDR_7BIT                   (0x18)
#define LPC_I2C_PORT                    LPC_I2C2
#define LPC_I2CS_CLOCK              SYSCON_CLOCK_I2C2
#define LPC_I2CS_RESET              RESET_I2C2
#define LPC_I2CS_INT                    I2C2_IRQn
#define LPC_I2CS_HANDLER            I2C2_IRQHandler

/* ROM driver handle for I2C slave*/
static ROM_I2CS_HANDLE_T i2csHandle;

/* I2C driver context area */
static uint32_t drvData[16];

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/* Made global so data can be seen in the debuuger */
ROM_I2CS_XFER_T sXfer;
uint8_t slaveRx[12], slaveTx[12];
volatile bool xferDone;

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Initializes pin muxing for I2C interface - note that SystemInit() may
   already setup your pin muxing at system startup */
static void Init_I2C_PinMux(void)
{
#if defined(BOARD_NXP_LPCXPRESSO_54102)
	/* Connect the I2C_SDA and I2C_SCL signals to port pins */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 27, (IOCON_FUNC1 | IOCON_DIGITAL_EN));	/* I2C2 */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 28, (IOCON_FUNC1 | IOCON_DIGITAL_EN));	/* I2C2 */

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

static volatile bool inXfer;
void i2cSlaveCompleteFunc(ROM_I2CS_HANDLE_T i2csHandle, ROM_I2CS_XFER_T *pXfer)
{
	/* Event callback called when a transfer for an enabled slave is done */
	bool *pxferDone = (bool *) ROM_I2CS_HANDLE_TOUDATA(i2csHandle);

	*pxferDone = true;
}

void i2cSlaveStartFunc(ROM_I2CS_HANDLE_T i2csHandle, uint16_t addr)
{
	/* Event callback called when a transfer for an enabled slave is started */
}

/* I2C interrupt handler */
void LPC_I2CS_HANDLER(void)
{
	ROM_I2CS_TransferHandler(i2csHandle);
}

/**
 * @brief	Main routine for I2C example
 * @return	Function should not exit
 */
int main(void)
{
	uint32_t memSize, *devMem, optimalDev;
	ROM_I2CS_INIT_T i2csInit;
	ROM_I2CS_SLAVE_T slaveSetup;
	int i;

	/* Generic Initialization */
	SystemCoreClockUpdate();
	Board_Init();

	/* Setup I2C pin muxing, enable I2C clock and reset I2C peripheral */
	Init_I2C_PinMux();
	Chip_Clock_EnablePeriphClock(LPC_I2CS_CLOCK);
	Chip_SYSCON_PeriphReset(LPC_I2CS_RESET);

	/* Get needed size for driver context memory */
	memSize = ROM_I2CS_GetMemSize();
	if (memSize > sizeof(drvData)) {
		errorOut("Can't allocate memory for I2C driver context\r\n");
	}
	devMem = drvData;	/* Or just use malloc(memSize) */

	/* Initialize driver */
	i2csInit.pUserData = (void *) &xferDone;
	i2csInit.base = (uint32_t) LPC_I2C_PORT;
	i2csHandle = ROM_I2CS_Init(devMem, &i2csInit);
	if (i2csHandle == NULL) {
		/* Error initializing I2C */
		errorOut("Error initializing ROM\r\n");
	}

	/* Register slave start and completion callbacks */
	ROM_I2CS_RegisterCallback(i2csHandle, ROM_I2CS_DONE_CB, (void *) i2cSlaveCompleteFunc);
	ROM_I2CS_RegisterCallback(i2csHandle, ROM_I2CS_START_CB, (void *) i2cSlaveStartFunc);

	/* Setup slave address to respond to */
	slaveSetup.slaveAddr = I2C_ADDR_7BIT;
	slaveSetup.SlaveIndex = 0;
	slaveSetup.EnableSlave = 1;
	ROM_I2CS_SetupSlave(i2csHandle, &slaveSetup);

	/* Setup clock rate for I2C - this must be done for master or slave modes.
	   This function is not set by the ROM API code. */
	optimalDev = Chip_Clock_GetAsyncSyscon_ClockRate() / 4000000;	/* 250nS */
	LPC_I2C_PORT->CLKDIV = optimalDev;

	/* Enable the interrupt for the I2C */
	NVIC_EnableIRQ(LPC_I2CS_INT);

	while (1) {
		/* Populate some TX data and clear RX data */
		for (i = 0; i < sizeof(slaveTx); i++) {
			slaveTx[i] = 0x70 + i;
			slaveRx[i] = 0;
		}
		sXfer.txBuff = slaveTx;
		sXfer.rxBuff = slaveRx;
		sXfer.txSz = sizeof(slaveTx);
		sXfer.rxSz = sizeof(slaveRx);

		/* Wait for a transfer from the master */
		xferDone = false;
		ROM_I2CS_Transfer(i2csHandle, &sXfer);	/* Never blocks, always returns LPC_OK */
		DEBUGOUT("Waiting for master\r\n");

		/* Wait for transfer to complete. The slave data completion callback
		   will set xferDone when ready via it's completion callback. */
		while (xferDone == false) {
			__WFI();
		}

		/* Check status of the transfer */
		DEBUGOUT("Transfer complete (%x)\r\n", sXfer.status);
		DEBUGOUT("RX [%d]", sXfer.bytesRecv);
		for (i = 0; i < sXfer.bytesRecv; i++) {
			DEBUGOUT(" : %02x", slaveRx[i]);
		}
		DEBUGOUT("\r\n");
		DEBUGOUT("TX [%d]", sXfer.bytesSent);
		for (i = 0; i < sXfer.bytesSent; i++) {
			DEBUGOUT(" : %02x", slaveTx[i]);
		}
		DEBUGOUT("\r\n");
	}

	/* Code never reaches here. Only used to satisfy standard main() */
	return 0;
}
