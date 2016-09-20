/*
 * @brief I2C bus monitor example
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

/** @defgroup I2CMON_5410X I2C bus monitor ROM API example
 * @ingroup EXAMPLES_ROM_5410X
 * @include "rom\i2cmon\readme.txt"
 */

/**
 * @}
 */

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

#define LPC_I2C_PORT         LPC_I2C2
#define LPC_I2C_INTHAND      I2C2_IRQHandler
#define LPC_IRQNUM           I2C2_IRQn
#define LPC_I2C_CLOCK                SYSCON_CLOCK_I2C2
#define LPC_I2C_RESET                RESET_I2C2

/* DMA driver context area */
static uint32_t drvData[16];

/* ROM driver handle for I2C monitor */
static ROM_I2CMON_HANDLE_T i2cmomHandle;

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/* Maximum number of capture queues */
#define MAXCAPS 3
ROM_I2CMON_CAP_T *pCurrentCap, *pNextCap, capQueue[MAXCAPS];

/* Capture buffer, maximum of 32 bytes */
#define MAXBUFFCAPLEN   32
uint16_t capBuffer[MAXCAPS][MAXBUFFCAPLEN];

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

/* Handle I2C interrupt by calling I2CMON interrupt transfer handler */
void LPC_I2C_INTHAND(void)
{
	/* Call I2CMON transfer handler function */
	ROM_I2CMON_TransferHandler(i2cmomHandle);
}

/* I2C monitor completion callback */
void i2cMonitorDoneCallback(ROM_I2CMON_HANDLE_T handle, ROM_I2CMON_CAP_T *pCap)
{
	int *capGetIndex = (int *) ROM_I2CMON_HANDLE_TOUDATA(handle);

	pCurrentCap = pCap;
	*capGetIndex++;
}

void i2cMonSetupDMACallback(ROM_I2CMON_HANDLE_T handle, ROM_I2CMON_CAP_T *pCap)
{
	/* This function is called to setup the I2C monitor, not used. */
}

/**
 * @brief	Main routine for I2C monitor example
 * @return	Function should not exit
 */
int main(void)
{
	uint32_t memSize, *devMem;
	ROM_I2CMON_INIT_T i2cmonInit;
	int i, curCapIndex = 0;
	volatile int curCapGetIndex;

	/* Generic Initialization */
	SystemCoreClockUpdate();
	Board_Init();

	/* Setup I2C pin muxing, enable I2C clock and reset I2C peripheral */
	Init_I2C_PinMux();
	Chip_Clock_EnablePeriphClock(LPC_I2C_CLOCK);
	Chip_SYSCON_PeriphReset(LPC_I2C_RESET);

	/* Get needed size for driver context memory */
	memSize = ROM_I2CMON_GetMemSize();
	if (memSize > sizeof(drvData)) {
		errorOut("Can't allocate memory for I2C monitor driver context\r\n");
	}
	devMem = drvData;	/* Or just use malloc(memSize) */

	/* Initialize driver */
	i2cmonInit.pUserData = (void *) &curCapGetIndex;
	i2cmonInit.base = (uint32_t) LPC_I2C_PORT;
	i2cmonInit.stretch = 1;	/* Enable clock stretching */
	i2cmomHandle = ROM_I2CMON_Init(devMem, &i2cmonInit);
	if (i2cmomHandle == NULL) {
		/* Error initializing I2C monitor */
		errorOut("Error initializing ROM\r\n");
	}

	/* Register the capture completion and DMA callbacks */
	ROM_I2CMON_RegisterCallback(i2cmomHandle, ROM_I2CMON_CAPTUREREADY_CB, (void *) i2cMonitorDoneCallback);
	ROM_I2CMON_RegisterCallback(i2cmomHandle, ROM_I2CMON_DMASETUP_CB, (void *) i2cMonSetupDMACallback);

	/* Enable the interrupt for the I2C */
	NVIC_EnableIRQ(LPC_IRQNUM);

	/* Read data as fast as possible in loop */
	while (1) {
		/* Next capture buffer */
		pNextCap = &capQueue[curCapIndex];

		/* Setup capture as non-blocking */
		pNextCap->startBuff = &capBuffer[curCapIndex][0];
		pNextCap->startBuffSz = MAXBUFFCAPLEN;
		pNextCap->flags = 0;

		/* Next expected capture index */
		curCapIndex++;
		if (curCapIndex >= MAXCAPS) {
			curCapIndex = 0;
		}

		/* Queue capture descriptor to start capture */
		ROM_I2CMON_StartLog(i2cmomHandle, pNextCap);

		/* Wait for capture to complete */
		while (pNextCap->status == ERR_I2C_BUSY) {}

		/* Display captured I2C data */
		if (pNextCap->status != LPC_OK) {
			DEBUGOUT("Error with monitor function (%x)\r\n", pNextCap->status);
		}
		else {
			if (curCapGetIndex != curCapIndex) {
				uint16_t data, *p16 = (uint16_t *) pCurrentCap->startBuff;

				/* Show captured data */
				for (i = 0; i < pCurrentCap->capStartBuffSz; i++) {
					data = *p16;
					p16++;
					if ((data & (I2C_MONRXDAT_MONSTART | I2C_MONRXDAT_MONRESTART)) != 0) {
						if ((data & I2C_MONRXDAT_MONSTART) != 0) {
							DEBUGSTR("[S:");
						}
						else {
							DEBUGSTR("[S2:");
						}
						DEBUGOUT("%02x:", ((data & I2C_MONRXDAT_DATA) >> 1));
						if ((data & 1) != 0) {
							DEBUGSTR("R:");
						}
						else {
							DEBUGSTR("W:");
						}
					}
					else {
						DEBUGOUT("[D:%02x:", (data & I2C_MONRXDAT_DATA));
					}
					if ((data & I2C_MONRXDAT_MONNACK) != 0) {
						DEBUGOUT("N] ");
					}
					else {
						DEBUGOUT("A] ");
					}
				}

				DEBUGSTR("\r\n");
			}
		}
	}

	/* Code never reaches here. Only used to satisfy standard main() */
	return 0;
}
