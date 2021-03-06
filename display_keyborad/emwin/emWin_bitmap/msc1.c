/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                           www.segger.com                           *
**********************************************************************
*                                                                    *
* C-file generated by                                                *
*                                                                    *
*        Bitmap Converter for emWin V5.26a.                          *
*        Compiled Aug 20 2014, 15:23:27                              *
*                                                                    *
*        (c) 1998 - 2014 Segger Microcontroller GmbH & Co. KG        *
*                                                                    *
**********************************************************************
*                                                                    *
* Source file: msc1                                                  *
* Dimensions:  16 * 16                                               *
* NumColors:   16                                                    *
*                                                                    *
**********************************************************************
*/

#include <stdlib.h>

#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

extern GUI_CONST_STORAGE GUI_BITMAP bmmsc1;

/*********************************************************************
*
*       Palette
*
*  Description
*    The following are the entries of the palette table.
*    The entries are stored as a 32-bit values of which 24 bits are
*    actually used according to the following bit mask: 0xBBGGRR
*
*    The lower   8 bits represent the Red   component.
*    The middle  8 bits represent the Green component.
*    The highest 8 bits represent the Blue  component.
*/
static GUI_CONST_STORAGE GUI_COLOR _Colorsmsc1[] = {
  0x000000, 0x101010, 0x202020, 0x303030,
  0x404040, 0x505050, 0x606060, 0x707070,
  0x808080, 0x909090, 0xA0A0A0, 0xB0B0B0,
  0xC0C0C0, 0xD0D0D0, 0xE0E0E0, 0xF0F0F0
};

static GUI_CONST_STORAGE GUI_LOGPALETTE _Palmsc1 = {
  16,  // Number of entries
  0,   // No transparency
  &_Colorsmsc1[0]
};

static GUI_CONST_STORAGE unsigned char _acmsc1[] = {
  0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33,
  0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33,
  0x33, 0x8E, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xA3,
  0x33, 0xEC, 0x88, 0x88, 0x88, 0x88, 0x8A, 0xF3,
  0x33, 0xE8, 0x33, 0x33, 0x33, 0x33, 0x36, 0xE3,
  0x33, 0xE9, 0x33, 0x33, 0x33, 0x33, 0x36, 0xE3,
  0x33, 0xE9, 0x33, 0x33, 0x33, 0x33, 0x36, 0xE3,
  0x33, 0xE9, 0x33, 0x33, 0x33, 0x33, 0x36, 0xE3,
  0x33, 0xE9, 0x33, 0x33, 0x33, 0x33, 0x36, 0xE3,
  0x33, 0xE8, 0x33, 0x33, 0x33, 0x33, 0x36, 0xE3,
  0x33, 0xDF, 0xDD, 0xDC, 0xCD, 0xDD, 0xDE, 0xE3,
  0x33, 0x5A, 0xBA, 0xAF, 0xFF, 0xBA, 0xAA, 0x73,
  0x33, 0x33, 0x34, 0xBF, 0xFF, 0xC5, 0x33, 0x33,
  0x33, 0x33, 0x3C, 0xFE, 0xEE, 0xFD, 0x33, 0x33,
  0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33,
  0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33
};

GUI_CONST_STORAGE GUI_BITMAP bmmsc1 = {
  16, // xSize
  16, // ySize
  8, // BytesPerLine
  4, // BitsPerPixel
  _acmsc1,  // Pointer to picture data (indices)
  &_Palmsc1   // Pointer to palette
};

/*************************** End of file ****************************/
