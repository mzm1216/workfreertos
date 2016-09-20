/*
 * @brief I2CM bus master example using interrupt mode
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

/** @defgroup I2CM_INT_5410X I2C master (interrupt mode) ROM API example
 * @ingroup EXAMPLES_ROM_5410X
 * @include "rom\i2cm_int\readme.txt"
 */

/**
 * @}
 */

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* 400KHz I2C bit-rate */
#define I2C_BITRATE         (400000)

/** I2C interface setup */
#define I2C_ADDR_7BIT       (0x4A)
#define LPC_I2C_PORT         LPC_I2C0
#define LPC_I2C_INTHAND      I2C0_IRQHandler
#define LPC_IRQNUM           I2C0_IRQn
#define LPC_I2CM_CLOCK              SYSCON_CLOCK_I2C0
#define LPC_I2CM_RESET              RESET_I2C0

/* ROM driver handle for I2C master */
static ROM_I2CM_HANDLE_T i2cmHandle;

/* I2C driver context area */
static uint32_t drvData[16];

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/* Made global so data can be seen in the debuuger */
uint8_t rx[10], tx[4];
ROM_I2CM_XFER_T mXfer;
volatile bool done;
uint32_t actualRate;

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Initializes pin muxing for I2C interface - note that SystemInit() may
   already setup your pin muxing at system startup */
static void Init_I2C_PinMux(void)
{
#if defined(BOARD_NXP_LPCXPRESSO_54102)
	/* Connect the I2C_SDA and I2C_SCL signals to port pins */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 23, (IOCON_FUNC1 | IOCON_DIGITAL_EN));	/* I2C0 */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 24, (IOCON_FUNC1 | IOCON_DIGITAL_EN));	/* I2C0 */

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

void i2cMasterDoneCallback(ROM_I2CM_HANDLE_T handle, ROM_I2CM_XFER_T *pXfer)
{
	bool *done = (bool *) ROM_I2CM_HANDLE_TOUDATA(handle);
	*done = true;
}

/**
 * @brief	Handle I2C1 interrupt by calling I2CM interrupt transfer handler
 * @return	Nothing
 */
void LPC_I2C_INTHAND(void)
{
	/* Call I2CM transfer handler function */
	ROM_I2CM_TransferHandler(i2cmHandle);
}

/**
 * @brief	Main routine for I2C example
 * @return	Function should not exit
 */
int main(void)
{
	uint32_t memSize, *devMem;
	ROM_I2CM_INIT_T i2cmInit;

	/* Generic Initialization */
	SystemCoreClockUpdate();
	Board_Init();

	/* Setup I2C pin muxing, enable I2C clock and reset I2C peripheral */
	Init_I2C_PinMux();
	Chip_Clock_EnablePeriphClock(LPC_I2CM_CLOCK);
	Chip_SYSCON_PeriphReset(LPC_I2CM_RESET);

	/* Get needed size for driver context memory */
	memSize = ROM_I2CM_GetMemSize();
	if (memSize > sizeof(drvData)) {
		errorOut("Can't allocate memory for I2C driver context\r\n");
	}
	devMem = drvData;	/* Or just use malloc(memSize) */

	/* Initialize driver */
	i2cmInit.pUserData = (void *) &done;
	i2cmInit.base = (uint32_t) LPC_I2C_PORT;
	i2cmHandle = ROM_I2CM_Init(devMem, &i2cmInit);
	if (i2cmHandle == NULL) {
		/* Error initializing I2C */
		errorOut("Error initializing ROM\r\n");
	}

	/* Set I2C clock rate */
	actualRate = ROM_I2CM_SetClockRate(i2cmHandle,
									   Chip_Clock_GetAsyncSyscon_ClockRate(), I2C_BITRATE);
	DEBUGOUT("Actual I2C master rate = %dHz\r\n", actualRate);

	/* Enable the interrupt for the I2C */
	NVIC_EnableIRQ(LPC_IRQNUM);

	/* Register the transfer completion callback */
	ROM_I2CM_RegisterCallback(i2cmHandle, ROM_I2CM_DATACOMPLETE_CB, (void *) i2cMasterDoneCallback);

	/* Read data as fast as possible in loop */
	while (1) {
		/* Write address (2), read 8 bytes back, no flags */
		mXfer.slaveAddr = I2C_ADDR_7BIT;
		tx[0] = 2;
		mXfer.txBuff = tx;
		mXfer.rxBuff = rx;
		mXfer.txSz = 1;
		mXfer.rxSz = 8;
		mXfer.flags = 0;

		/* Start transfer */
		done = false;
		ROM_I2CM_Transfer(i2cmHandle, &mXfer);

		/* Sleep while waiting for transfer to complete */
		while ((done == false) && (mXfer.status == ERR_I2C_BUSY)) {
			/* If it appears that this example is not working and is hanging here,
			   then the I2C interrupt may be firing before going into sleep mode,
			   so there is nothing to wakeup the device from sleep. Try commenting
			   out the line below. */
			__WFI();
		}

		/* Check status of the transfer */
		DEBUGOUT("I2C transfer completed: status = %x\r\n", mXfer.status);
		if (mXfer.status != LPC_OK) {
			errorOut("Error during I2CM transfer\r\n");
		}
	}

	/* Code never reaches here. Only used to satisfy standard main() */
	return 0;
}
