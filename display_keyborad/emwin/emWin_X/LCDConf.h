/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2012  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.16 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software has been licensed to  ARM LIMITED whose registered office
is situated at  110 Fulbourn Road,  Cambridge CB1 9NJ,  England solely
for  the  purposes  of  creating  libraries  for  ARM7, ARM9, Cortex-M
series,  and   Cortex-R4   processor-based  devices,  sublicensed  and
distributed as part of the  MDK-ARM  Professional  under the terms and
conditions  of  the   End  User  License  supplied  with  the  MDK-ARM
Professional. 
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : LCDConf.h
Purpose     : Display driver configuration file
----------------------------------------------------------------------
*/

#ifndef LCDCONF_H
#define LCDCONF_H

#define USE_FLEXCOLOR_DRIVER_FLG    	(0)//0 不使用gui的flexcolor驱动，
/*
********************************************************************
*
*       Layer configuration (to be modified)
*
********************************************************************
*/
// Physical display size
//
#ifdef USED_SSD2119
#define XSIZE_PHYS  320
#define YSIZE_PHYS  120
#define LCD_BITSPERPIXEL        16
#define COLOR_CONVERSION   GUICC_M565

#if USE_FLEXCOLOR_DRIVER_FLG
#define DISPLAY_DRIVER GUIDRV_FLEXCOLOR
#else
#define DISPLAY_DRIVER GUIDRV_LIN_16
#endif
//#define DISPLAY_DRIVER GUIDRV_LIN_4 


#else
#define XSIZE_PHYS  256
#define YSIZE_PHYS  64
#define LCD_BITSPERPIXEL        4
#define COLOR_CONVERSION   GUICC_4
//#define DISPLAY_DRIVER &GUIDRV_Template_APIGUIDRV_FLEXCOLOR
//#define DISPLAY_DRIVER GUIDRV_LIN_16
#define DISPLAY_DRIVER GUIDRV_LIN_4



#endif
#define LCD_XSIZE	XSIZE_PHYS
#define LCD_YSIZE	YSIZE_PHYS

#define VXSIZE_PHYS XSIZE_PHYS
#define VYSIZE_PHYS YSIZE_PHYS

//#define LCD_CONTROLLER          ssd1322
#define LCD_USE_PARALLEL_16     0
//
// Color conversion
//
//
// Display driver
//

 


//
// Orientation
//
//#define DISPLAY_ORIENTATION  GUI_SWAP_XY | GUI_MIRROR_X | GUI_MIRROR_Y
//#define DISPLAY_ORIENTATION  GUI_SWAP_XY// | GUI_MIRROR_X | GUI_MIRROR_Y
//#define LCD_MIRROR_Y	0

// Configures touch screen module
//
//#define GUI_TOUCH_AD_LEFT 	 35
//#define GUI_TOUCH_AD_RIGHT 	 980
//#define GUI_TOUCH_AD_TOP 	 915
//#define GUI_TOUCH_AD_BOTTOM  90

#endif /* LCDCONF_H */

/*************************** End of file ****************************/
