/*
 * @brief SPIM bus master example using interrupt mode
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

/** @defgroup SPIM_INT_5410X SPI master (interrupt mode) ROM API example
 * @ingroup EXAMPLES_ROM_5410X
 * @include "rom\spim_int\readme.txt"
 */

/**
 * @}
 */

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* 3MHz SPI bit-rate */
#define SPI_BITRATE         (3000000)

/* Use SPI0 port */
#define LPC_SPIM_PORT         LPC_SPI0
#define LPC_SPIM_CLOCK              SYSCON_CLOCK_SPI0
#define LPC_SPIM_RESET              RESET_SPI0
#define LPC_SPIM_INTHANDLER   SPI0_IRQHandler
#define LPC_SPIIRQNUM         SPI0_IRQn

/* ROM driver handle for SPI master */
static ROM_SPIM_HANDLE_T spimHandle;

#define BUFFSENDSIZE 8

/* SPI driver context area */
static uint32_t drvData[16];

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/* Made global so data can be seen in the debuuger */
ROM_SPIM_XFER_T mXfer;
uint16_t rx16[BUFFSENDSIZE], tx16[BUFFSENDSIZE];

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Initializes pin muxing for SPI interface - note that SystemInit() may
   already setup your pin muxing at system startup */
static void Init_SPI_PinMux(void)
{
#if defined(BOARD_NXP_LPCXPRESSO_54102)
	/* 0.11 = SPI0_SCK, 0.12 = SPI0_MOSI, 0.13 = SPI0_MISO, 0.14 = SPI0_SSELN0 */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 11, (IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 12, (IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 13, (IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 14, (IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP));

#else
	/* Configure your own I2C pin muxing here if needed */
#warning "No SPI pin muxing defined"
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
void LPC_SPIM_INTHANDLER(void)
{
	ROM_SPIM_TransferHandler(spimHandle);
}

void CBspiMasterXferCSAssertCB(ROM_SPIM_HANDLE_T spimHandle, ROM_SPIM_XFER_T *pXfer)
{
	/* SPI master assertion event */
}

void CBspiMMasterXferCSDeAssertCB(ROM_SPIM_HANDLE_T spimHandle, ROM_SPIM_XFER_T *pXfer)
{
	/* SPI master de-assertion event */
}

/**
 * @brief	Main routine for SPI example
 * @return	Function should not exit
 */
int main(void)
{
	uint16_t rx16[BUFFSENDSIZE], tx16[BUFFSENDSIZE];
	uint32_t memSize, *devMem;
	ROM_SPIM_INIT_T spimInit;
	ROM_SPIM_XFER_CONFIG_T spimConfig;
	int i;

	/* Generic Initialization */
	SystemCoreClockUpdate();
	Board_Init();

	/* Setup I2C pin muxing, enable I2C clock and reset I2C peripheral */
	Init_SPI_PinMux();
	Chip_Clock_EnablePeriphClock(LPC_SPIM_CLOCK);
	Chip_SYSCON_PeriphReset(LPC_SPIM_RESET);

	/* Get needed size for driver context memory */
	memSize = ROM_SPIM_GetMemSize();
	if (memSize > sizeof(drvData)) {
		errorOut("Can't allocate memory for driver context\r\n");
	}
	devMem = drvData;	/* Or just use malloc(memSize) */

	/* Initialize driver */
	spimInit.pUserData = NULL;
	spimInit.base = (uint32_t) LPC_SPIM_PORT;
	spimInit.baseClockRate = Chip_Clock_GetAsyncSyscon_ClockRate();
	spimInit.spiPol[0] = 0;	/* Active low select for SSEL0 */
	spimInit.spiPol[1] = 1;
	spimInit.spiPol[2] = 1;
	spimInit.spiPol[3] = 1;
	spimHandle = ROM_SPIM_Init(devMem, &spimInit);
	if (spimHandle == NULL) {
		/* Error initializing SPI */
		errorOut("Error initializing ROM\r\n");
	}

	/* Set SPI transfer configuration */
	spimConfig.dXferBitRate = SPI_BITRATE;
	spimConfig.mode = ROM_SPI_CLOCK_CPHA0_CPOL0;
	spimConfig.lsbFirst = 0;
	spimConfig.dataBits = 16;
	spimConfig.PreDelay = 0;
	spimConfig.PostDelay = 0;
	spimConfig.FrameDelay = 0;
	spimConfig.TransferDelay = 1;
	if (ROM_SPIM_SetupTransfer(spimHandle, &spimConfig) != LPC_OK) {
		errorOut("SPI configuration is invalid\r\n");
	}

	/* Show desired and actual SPI rates */
	DEBUGOUT("SPI rate = %d (actual %d)\r\n", spimConfig.dXferBitRate, spimConfig.rXferBitRate);

	/* Callback regiostyration  */
	ROM_SPIM_RegisterCallback(spimHandle, ROM_SPIM_ASSERTSSEL_CB, (void *) CBspiMasterXferCSAssertCB);
	ROM_SPIM_RegisterCallback(spimHandle, ROM_SPIM_DEASSERTSSEL_CB, (void *) CBspiMMasterXferCSDeAssertCB);

	/* Enable the interrupt for the SPI */
	NVIC_EnableIRQ(LPC_SPIIRQNUM);

	/* Read data as fast as possible in loop */
	while (1) {
		/* Populate some TX data and clear RX data */
		for (i = 0; i < BUFFSENDSIZE; i++) {
			tx16[i] = i + 1 + ((i + 1) << 8);
			rx16[i] =  0;
		}

		/* Setup buffers and sizes, use select 0 for transfer */
		mXfer.txBuff = tx16;
		mXfer.txSz = BUFFSENDSIZE;
		mXfer.rxBuff = rx16;
		mXfer.rxSz = BUFFSENDSIZE;
		mXfer.sselNum = 0;

		/* Setup for loopback mode */
		mXfer.flags = ROM_SPIM_FLAG_LOOPBACK;

		/* Start transfer. Will return immediately */
		ROM_SPIM_Transfer(spimHandle, &mXfer);

		/* Status will change from BUSY once transfer is complete */
		while (mXfer.status == ERR_SPI_BUSY) {}

		/* Check status of the transfer */
		if (mXfer.status != LPC_OK) {
			DEBUGOUT("-Error performing transfer = %x\r\n", mXfer.status);
		}
		else {
			DEBUGOUT("-SPI transfer completed: status = %x\r\n", mXfer.status);
			for (i = 0; i < BUFFSENDSIZE; i++) {
				DEBUGOUT("%04x %04x : ", tx16[i], rx16[i]);
			}
		}
	}

	/* Code never reaches here. Only used to satisfy standard main() */
	return 0;
}
