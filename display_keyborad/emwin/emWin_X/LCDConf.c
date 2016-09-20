/*********************************************************************
*                SEGGER MICROCONTROLLER GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 2003-2012     SEGGER Microcontroller GmbH & Co KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File        : LCDConf.c
Purpose     : Display controller configuration (single layer)
---------------------------END-OF-HEADER------------------------------
*/
#include "GUI.h"
#include "GUIDRV_Template.h"
#include "GUIDRV_FlexColor.h"
#include "LCDConf.h"
//#include "bsp_tft_lcd.h"
#include "Oled_driver.h"
#include "GUIDRV_Lin.h"



/*
********************************************************************
*
*       Layer configuration (to be modified)
*
********************************************************************
*/
// Physical display size
//

//
//#define DISPLAY_ORIENTATION  GUI_SWAP_XY | GUI_MIRROR_X | GUI_MIRROR_Y
//#define DISPLAY_ORIENTATION  GUI_SWAP_XY// | GUI_MIRROR_X | GUI_MIRROR_Y
//
// Configures touch screen module
//
//#define GUI_TOUCH_AD_LEFT 	 35
//#define GUI_TOUCH_AD_RIGHT 	 980
//#define GUI_TOUCH_AD_TOP 	 915
//#define GUI_TOUCH_AD_BOTTOM  90


#define NUM_BUFFERS  1 // Number of multiple buffers to be used
#define NUM_VSCREENS 1 // Number of virtual screens to be used

/*********************************************************************
*
*       Configuration checking
*
**********************************************************************
*/
#ifdef USED_SSD2119
extern uint16_t LCDBUFF_MZM[];//256*64*4bit=64Row *64Column*2byte

#else
extern uint8_t LCDBUFF_MZM[];//256*64*4bit=64Row *64Column*2byte

#endif
//extern  __align(8) uint8_t LCDBUFF_MZM[120* 320] __attribute__ ((section ("VRAM"), zero_init));//256*64*4bit=64Row *64Column*2byte

#ifndef   VRAM_ADDR
  #define VRAM_ADDR (uint32_t)LCDBUFF_MZM// TBD by customer: This has to be the frame buffer start address
#endif


#ifndef   NUM_VSCREENS
  #define NUM_VSCREENS 1
#else
  #if (NUM_VSCREENS <= 0)
    #error At least one screeen needs to be defined!
  #endif
#endif
#if (NUM_VSCREENS > 1) && (NUM_BUFFERS > 1)
  #error Virtual screens and multiple buffers are not allowed!
#endif
/*
**********************************************************************
*
*       Configuration checking
*
**********************************************************************
*/
#if 1
#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif

#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif

#ifndef   VXSIZE_PHYS
  #define VXSIZE_PHYS XSIZE_PHYS
#endif

#ifndef   VYSIZE_PHYS
  #define VYSIZE_PHYS YSIZE_PHYS
#endif

#ifndef   VRAM_ADDR
  #define VRAM_ADDR 0
#endif

#ifndef   COLOR_CONVERSION
  #error Color conversion not defined!
#endif

#ifndef   DISPLAY_DRIVER
  #error No display driver defined!
#endif
#endif

/*
****************************************************************************
*
*       LCD_X_Config
*
* Purpose:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*
****************************************************************************   
*/
#if 0
void LCD_X_Config(void) 
{		
 	/* Set display driver and color conversion for 1st layer */
    GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER, COLOR_CONVERSION, 0, 0);
	
    /* Display driver configuration */
	LCD_SetSizeEx    (0, XSIZE_PHYS, YSIZE_PHYS);
	LCD_SetVSizeEx   (0, VXSIZE_PHYS, VYSIZE_PHYS);
}
#else
void LCD_X_Config(void) {
  //
  // At first initialize use of multiple buffers on demand
  //
	GUI_DEVICE *pdevice;
  	GUI_PORT_API hw_api;
 	CONFIG_FLEXCOLOR  config;
	
	  #if (NUM_BUFFERS > 1)
	    GUI_MULTIBUF_Config(NUM_BUFFERS);
	  #endif
	  //
	  // Set display driver and color conversion for 1st layer
	  //
	  pdevice = GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER, COLOR_CONVERSION, 0, 0);
#if USE_FLEXCOLOR_DRIVER_FLG
	  hw_api.pfWrite8_A0 		=	oled_wirte_8_A0;
	  hw_api.pfWrite8_A1 		=	oled_wirte_8_A1;
	  hw_api.pfWriteM8_A1		=	oled_write_M8_A1;
	  hw_api.pfReadM8_A1 		=	oled_read_M8_A1;
	  	
	  GUIDRV_FlexColor_SetFunc(pdevice,&hw_api,GUIDRV_FLEXCOLOR_F66714,GUIDRV_FLEXCOLOR_M16C0B8);

	config.FirstCOM = 0x78;
	config.FirstSEG = 0;
	config.Orientation = 0;
	config.RegEntryMode = 0x6870;
	
	  GUIDRV_FlexColor_Config(pdevice,&config)  ;
#endif
	  //
	  // Display driver configuration, required for Lin-driver
	  //
	  if (LCD_GetSwapXY()) {
	    LCD_SetSizeEx (0, YSIZE_PHYS, XSIZE_PHYS);
	    LCD_SetVSizeEx(0, YSIZE_PHYS * NUM_VSCREENS, XSIZE_PHYS);
	  } else {
	    LCD_SetSizeEx (0, XSIZE_PHYS, YSIZE_PHYS);
	    LCD_SetVSizeEx(0, XSIZE_PHYS, YSIZE_PHYS * NUM_VSCREENS);
	  }
#if USE_FLEXCOLOR_DRIVER_FLG

#else
	  LCD_SetVRAMAddrEx(0, (void *)VRAM_ADDR);
#endif
	  //
	  // Set user palette data (only required if no fixed palette is used)
	  //
		 // #if defined(PALETTE)
		 //   LCD_SetLUTEx(0, PALETTE);
		 // #endif
}

#endif
/*
*********************************************************************************
*
*       LCD_X_DisplayDriver
*
* Purpose:
*   This function is called by the display driver for several purposes.
*   To support the according task the routine needs to be adapted to
*   the display controller. Please note that the commands marked with
*   'optional' are not cogently required and should only be adapted if 
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Index of layer to be configured
*   Cmd        - Please refer to the details in the switch statement below
*   pData      - Pointer to a LCD_X_DATA structure
*
* Return Value:
*   < -1 - Error
*     -1 - Command not handled
*      0 - Ok
*******************************************************************************
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) 
{
	int r;
	switch (Cmd) 
	{
		case LCD_X_INITCONTROLLER: 
		{
			//
			// Called during the initialization process in order to set up the
			// display controller and put it into operation. If the display
			// controller is not initialized by any external routine this needs
			// to be adapted by the customer...
			//
			// ...
			//  已经在前面初始化了，这里不再初始化
			
			return 0;
		}
		case LCD_X_SETVRAMADDR: 
		{
			//
			// Required for setting the address of the video RAM for drivers
			// with memory mapped video RAM which is passed in the 'pVRAM' element of p
			//
			LCD_X_SETVRAMADDR_INFO * p;
			(void)p;
			p = (LCD_X_SETVRAMADDR_INFO *)pData;
			//...
			return 0;
		}
		case LCD_X_SETORG: 
		{
			//
			// Required for setting the display origin which is passed in the 'xPos' and 'yPos' element of p
			//
			LCD_X_SETORG_INFO * p;
			(void)p;
			p = (LCD_X_SETORG_INFO *)pData;

			//...
			return 0;
		}
		case LCD_X_SHOWBUFFER: 
		{
			//
			// Required if multiple buffers are used. The 'Index' element of p contains the buffer index.
			//
			LCD_X_SHOWBUFFER_INFO * p;
			(void)p;
			p = (LCD_X_SHOWBUFFER_INFO *)pData;
			//...
			return 0;
		}
		case LCD_X_SETLUTENTRY: 
		{
			//
			// Required for setting a lookup table entry which is passed in the 'Pos' and 'Color' element of p
			//
			LCD_X_SETLUTENTRY_INFO * p;
			(void)p;
			p = (LCD_X_SETLUTENTRY_INFO *)pData;
			//...
			return 0;
		}
		case LCD_X_ON: 
		{
			//
			// Required if the display controller should support switching on and off
			//
			return 0;
		}
		case LCD_X_OFF: 
		{
			//
			// Required if the display controller should support switching on and off
			//
			// ...
			return 0;
		}
		default:
		r = -1;
	}
	return r;
}
/*************************** End of file ****************************/
