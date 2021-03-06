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
* Source file: heart1                                                *
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

extern GUI_CONST_STORAGE GUI_BITMAP bmheart1;

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
static GUI_CONST_STORAGE GUI_COLOR _Colorsheart1[] = {
  0x000000, 0x101010, 0x202020, 0x303030,
  0x404040, 0x505050, 0x606060, 0x707070,
  0x808080, 0x909090, 0xA0A0A0, 0xB0B0B0,
  0xC0C0C0, 0xD0D0D0, 0xE0E0E0, 0xF0F0F0
};

static GUI_CONST_STORAGE GUI_LOGPALETTE _Palheart1 = {
  16,  // Number of entries
  0,   // No transparency
  &_Colorsheart1[0]
};

static GUI_CONST_STORAGE unsigned char _acheart1[] = {
  0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33,
  0x33, 0x33, 0x33, 0x36, 0x63, 0x33, 0x33, 0x33,
  0x33, 0x34, 0x33, 0x37, 0x73, 0x33, 0x33, 0x33,
  0x33, 0x4E, 0x93, 0x37, 0x73, 0x36, 0x83, 0x33,
  0x33, 0x38, 0xF8, 0x37, 0x73, 0x59, 0x53, 0x33,
  0x33, 0x33, 0x7F, 0x75, 0x55, 0x95, 0x33, 0x33,
  0x33, 0x33, 0x37, 0x73, 0x35, 0x53, 0x33, 0x33,
  0x39, 0xCC, 0xC8, 0x33, 0x33, 0x9C, 0xCC, 0x93,
  0x38, 0xBB, 0xB8, 0x33, 0x33, 0x8B, 0xBB, 0x83,
  0x33, 0x33, 0x38, 0x83, 0x38, 0x83, 0x33, 0x33,
  0x33, 0x33, 0x9F, 0x79, 0x87, 0xF8, 0x33, 0x33,
  0x33, 0x39, 0xF7, 0x3C, 0xB3, 0x7F, 0x93, 0x33,
  0x33, 0x4D, 0x83, 0x3B, 0xB3, 0x38, 0xD3, 0x33,
  0x33, 0x33, 0x33, 0x3C, 0xB3, 0x33, 0x33, 0x33,
  0x33, 0x33, 0x33, 0x39, 0x83, 0x33, 0x33, 0x33,
  0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33
};

GUI_CONST_STORAGE GUI_BITMAP bmheart1 = {
  16, // xSize
  16, // ySize
  8, // BytesPerLine
  4, // BitsPerPixel
  _acheart1,  // Pointer to picture data (indices)
  &_Palheart1   // Pointer to palette
};

/*************************** End of file ****************************/
