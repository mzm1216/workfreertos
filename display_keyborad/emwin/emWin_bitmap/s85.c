/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                           www.segger.com                           *
**********************************************************************
*                                                                    *
* C-file generated by                                                *
*                                                                    *
*        Bitmap Converter for emWin (Demo version) V5.22.            *
*        Compiled Jul  4 2013, 12:18:31                              *
*        (c) 1998 - 2013 Segger Microcontroller GmbH && Co. KG       *
*                                                                    *
*        May not be used in a product                                *
*                                                                    *
**********************************************************************
*                                                                    *
* Source file: 85                                                    *
* Dimensions:  85 * 64                                               *
* NumColors:   16                                                    *
*                                                                    *
**********************************************************************
*/

#include <stdlib.h>

#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

extern GUI_CONST_STORAGE GUI_BITMAP bm85;

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
static GUI_CONST_STORAGE GUI_COLOR _Colors85[] = {
  0x000000, 0x111111, 0x222222, 0x333333,
  0x444444, 0x555555, 0x666666, 0x777777,
  0x888888, 0x999999, 0xAAAAAA, 0xBBBBBB,
  0xCCCCCC, 0xDDDDDD, 0xEEEEEE, 0xFFFFFF
};

static GUI_CONST_STORAGE GUI_LOGPALETTE _Pal85 = {
  16,  // Number of entries
  0,   // No transparency
  &_Colors85[0]
};

static GUI_CONST_STORAGE unsigned char _ac85[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 0x11, 0x00, 0x01, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x10, 0x01, 0x11, 0x11, 0x11, 0x11, 0x13, 0x76, 0x32, 0x11, 
        0x20,
  0x00, 0x00, 0x00, 0x00, 0x01, 0x10, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 0x22, 0x00, 0x01, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x11, 0x22, 0x13, 0x77, 0x42, 0x11, 
        0x20,
  0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 0x33, 0x33, 0x33, 0x22, 0x11, 0x11, 0x26, 0x21, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x22, 0x11, 0x22, 0x13, 0x78, 0x64, 0x21, 
        0x20,
  0x00, 0x01, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x10, 0x35, 0x44, 0x56, 0x65, 0x45, 0x43, 0x22, 0x74, 0x22, 0x21, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x10, 0x11, 0x11, 0x11, 0x11, 0x22, 0x23, 0x79, 0x63, 0x22, 
        0x20,
  0x01, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x36, 0x77, 0x54, 0x34, 0x44, 0x44, 0x55, 0x45, 0x44, 0x22, 0x22, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x21, 0x22, 0x23, 0x89, 0x74, 0x32, 
        0x20,
  0x00, 0x01, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x27, 0x87, 0x76, 0x42, 0x11, 0x24, 0x45, 0x45, 0x33, 0x54, 0x22, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x01, 0x11, 0x11, 0x12, 0x31, 0x22, 0x24, 0x89, 0x96, 0x42, 
        0x20,
  0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x13, 0x78, 0x77, 0x66, 0x41, 0x11, 0x11, 0x24, 0x45, 0x21, 0x14, 0x32, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x10, 0x01, 0x11, 0x11, 0x13, 0x21, 0x22, 0x24, 0x89, 0x97, 0x52, 
        0x20,
  0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x01, 0x11, 0x11, 0x37, 0x88, 0x76, 0x65, 0x52, 0x11, 0x11, 0x12, 0x24, 0x52, 0x25, 0x52, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x10, 0x00, 0x11, 0x11, 0x13, 0x21, 0x22, 0x24, 0x89, 0x98, 0x63, 
        0x20,
  0x00, 0x01, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 0x00, 0x13, 0x66, 0x67, 0x66, 0x45, 0x43, 0x11, 0x11, 0x11, 0x22, 0x22, 0x36, 0x75, 0x32, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x10, 0x11, 0x11, 0x12, 0x21, 0x21, 0x23, 0x79, 0x98, 0x64, 
        0x30,
  0x00, 0x01, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x36, 0x67, 0x65, 0x45, 0x43, 0x33, 0x31, 0x11, 0x11, 0x33, 0x43, 0x67, 0x76, 0x42, 0x21, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x21, 0x23, 0x69, 0x98, 0x76, 
        0x50,
  0x00, 0x01, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x11, 0x13, 0x66, 0x55, 0x64, 0x43, 0x43, 0x21, 0x11, 0x11, 0x11, 0x12, 0x75, 0x78, 0x76, 0x74, 0x21, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x01, 0x11, 0x11, 0x11, 0x21, 0x23, 0x69, 0x97, 0x77, 
        0x60,
  0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x26, 0x76, 0x44, 0x43, 0x21, 0x32, 0x11, 0x10, 0x11, 0x11, 0x45, 0x53, 0x67, 0x77, 0x76, 0x21, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 0x11, 0x12, 0x21, 0x21, 0x23, 0x79, 0x87, 0x66, 
        0x50,
  0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x58, 0x65, 0x53, 0x20, 0x01, 0x33, 0x11, 0x11, 0x11, 0x22, 0x5A, 0x53, 0x67, 0x9A, 0x87, 0x42, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x12, 0x11, 0x21, 0x24, 0x88, 0x86, 0x55, 
        0x30,
  0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x13, 0x87, 0x65, 0x31, 0x00, 0x02, 0x32, 0x22, 0x22, 0x24, 0x44, 0x44, 0x43, 0x56, 0x8C, 0xC8, 0x73, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x21, 0x25, 0x88, 0x87, 0x55, 
        0x20,
  0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x16, 0x86, 0x97, 0x20, 0x00, 0x12, 0x22, 0x22, 0x23, 0x34, 0x44, 0x55, 0x55, 0x67, 0x9C, 0xEB, 0x85, 0x22, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x12, 0x11, 0x11, 0x21, 0x26, 0x88, 0x87, 0x66, 
        0x20,
  0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x38, 0x7C, 0xC7, 0x21, 0x12, 0x44, 0x43, 0x23, 0x33, 0x34, 0x44, 0x55, 0x65, 0x67, 0x9D, 0xDB, 0x87, 0x32, 0x21, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x22, 0x11, 0x22, 0x27, 0x88, 0x87, 0x76, 
        0x30,
  0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x57, 0x9D, 0xD7, 0x33, 0x45, 0x43, 0x43, 0x33, 0x44, 0x44, 0x55, 0x56, 0x76, 0x66, 0x69, 0xDC, 0x86, 0x52, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x12, 0x11, 0x22, 0x37, 0x78, 0x88, 0x87, 
        0x40,
  0x11, 0x11, 0x11, 0x11, 0x11, 0x12, 0x11, 0x11, 0x11, 0x66, 0xBC, 0xD9, 0x55, 0x44, 0x44, 0x44, 0x44, 0x44, 0x45, 0x55, 0x67, 0x77, 0x87, 0x77, 0x7B, 0x85, 0x52, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x21, 0x12, 0x11, 0x22, 0x37, 0x78, 0x88, 0x87, 
        0x40,
  0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x21, 0x11, 0x57, 0xBC, 0x86, 0x43, 0x33, 0x34, 0x55, 0x55, 0x55, 0x66, 0x66, 0x78, 0x88, 0x99, 0x88, 0x87, 0x74, 0x53, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x22, 0x37, 0x78, 0x88, 0x88, 
        0x50,
  0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x21, 0x01, 0x36, 0xA8, 0x95, 0x32, 0x23, 0x34, 0x44, 0x56, 0x66, 0x66, 0x66, 0x77, 0x88, 0xA9, 0x99, 0x98, 0x75, 0x33, 0x21, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x22, 0x36, 0x79, 0x99, 0x88, 
        0x50,
  0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x21, 0x36, 0x99, 0x74, 0x22, 0x33, 0x44, 0x55, 0x66, 0x66, 0x66, 0x66, 0x67, 0x78, 0x98, 0xA9, 0xA9, 0x99, 0x63, 0x21, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x22, 0x26, 0x89, 0x99, 0x98, 
        0x50,
  0x22, 0x22, 0x12, 0x22, 0x22, 0x22, 0x23, 0x34, 0x33, 0x37, 0xA9, 0x53, 0x23, 0x44, 0x55, 0x66, 0x66, 0x66, 0x66, 0x66, 0x77, 0x78, 0x88, 0x99, 0x9A, 0xAA, 0x95, 0x21, 0x11, 0x11, 0x11, 0x11, 0x01, 0x11, 0x11, 0x11, 0x11, 0x22, 0x35, 0x89, 0x99, 0x88, 
        0x50,
  0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x34, 0x44, 0x44, 0x34, 0x88, 0x52, 0x34, 0x45, 0x67, 0x77, 0x77, 0x66, 0x66, 0x66, 0x67, 0x78, 0x88, 0x88, 0x9A, 0xAB, 0xA7, 0x31, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x22, 0x34, 0x99, 0x99, 0x88, 
        0x50,
  0x11, 0x11, 0x11, 0x21, 0x21, 0x11, 0x54, 0x33, 0x43, 0x44, 0x66, 0x33, 0x44, 0x56, 0x77, 0x67, 0x77, 0x76, 0x66, 0x66, 0x67, 0x78, 0x99, 0x9A, 0x99, 0xAA, 0xA7, 0x21, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x22, 0x34, 0x99, 0x98, 0x88, 
        0x50,
  0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x33, 0x33, 0x44, 0x44, 0x35, 0x44, 0x34, 0x56, 0x66, 0x66, 0x77, 0x77, 0x66, 0x67, 0x77, 0x88, 0x99, 0x9A, 0xA9, 0x89, 0x97, 0x32, 0x11, 0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x11, 0x11, 0x23, 0x35, 0x99, 0x99, 0x88, 
        0x50,
  0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x23, 0x33, 0x44, 0x33, 0x22, 0x55, 0x34, 0x55, 0x55, 0x66, 0x67, 0x77, 0x76, 0x67, 0x78, 0x88, 0x99, 0x9A, 0xAA, 0x89, 0x87, 0x44, 0x32, 0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x21, 0x11, 0x23, 0x36, 0x99, 0x99, 0x88, 
        0x50,
  0x11, 0x11, 0x11, 0x11, 0x12, 0x21, 0x12, 0x33, 0x33, 0x33, 0x22, 0x54, 0x34, 0x45, 0x55, 0x55, 0x56, 0x67, 0x76, 0x77, 0x77, 0x88, 0x89, 0x9A, 0xAA, 0x98, 0x97, 0x44, 0x44, 0x21, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x27, 0x99, 0x99, 0x88, 
        0x60,
  0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x33, 0x44, 0x43, 0x22, 0x22, 0x34, 0x45, 0x56, 0x65, 0x55, 0x56, 0x67, 0x77, 0x77, 0x77, 0x78, 0x88, 0x9A, 0x98, 0x87, 0x43, 0x45, 0x41, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x38, 0x99, 0x99, 0x98, 
        0x70,
  0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x23, 0x55, 0x43, 0x22, 0x13, 0x44, 0x55, 0x44, 0x33, 0x34, 0x45, 0x67, 0x77, 0x76, 0x65, 0x55, 0x78, 0x98, 0x88, 0x86, 0x43, 0x45, 0x52, 0x11, 0x11, 0x01, 0x22, 0x22, 0x23, 0x32, 0x12, 0x49, 0x99, 0x98, 0x98, 
        0x60,
  0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x23, 0x34, 0x43, 0x22, 0x24, 0x45, 0x44, 0x32, 0x55, 0x11, 0x44, 0x56, 0x76, 0x76, 0x55, 0x43, 0x59, 0xA9, 0x78, 0x86, 0x43, 0x44, 0x42, 0x11, 0x11, 0x01, 0x11, 0x12, 0x33, 0x32, 0x12, 0x59, 0x99, 0x99, 0x98, 
        0x50,
  0x33, 0x22, 0x22, 0x22, 0x23, 0x32, 0x32, 0x23, 0x34, 0x43, 0x22, 0x34, 0x45, 0x44, 0x34, 0x66, 0x21, 0x34, 0x56, 0x78, 0x75, 0x55, 0x41, 0x24, 0x99, 0x78, 0x86, 0x43, 0x44, 0x42, 0x11, 0x11, 0x01, 0x11, 0x11, 0x11, 0x11, 0x13, 0x79, 0x99, 0x99, 0x98, 
        0x40,
  0x22, 0x22, 0x33, 0x33, 0x33, 0x33, 0x33, 0x22, 0x34, 0x43, 0x22, 0x34, 0x44, 0x55, 0x44, 0x55, 0x22, 0x45, 0x67, 0x79, 0x65, 0x79, 0x62, 0x75, 0x79, 0x98, 0x76, 0x43, 0x44, 0x42, 0x11, 0x11, 0x11, 0x21, 0x11, 0x11, 0x11, 0x13, 0x89, 0x99, 0x99, 0x88, 
        0x30,
  0x22, 0x22, 0x33, 0x33, 0x33, 0x33, 0x33, 0x32, 0x33, 0x33, 0x33, 0x34, 0x45, 0x55, 0x55, 0x54, 0x44, 0x55, 0x67, 0x79, 0x75, 0x56, 0x54, 0x79, 0x99, 0x98, 0x76, 0x43, 0x44, 0x31, 0x11, 0x11, 0x11, 0x22, 0x21, 0x12, 0x32, 0x14, 0x99, 0x89, 0x98, 0x87, 
        0x30,
  0x22, 0x23, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x22, 0x23, 0x45, 0x54, 0x45, 0x55, 0x66, 0x55, 0x55, 0x66, 0x77, 0x7A, 0x96, 0x55, 0x56, 0x8A, 0xA9, 0x99, 0x75, 0x44, 0x44, 0x21, 0x11, 0x11, 0x11, 0x22, 0x21, 0x23, 0x32, 0x25, 0x99, 0x89, 0x99, 0x97, 
        0x30,
  0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x34, 0x44, 0x46, 0x44, 0x55, 0x55, 0x66, 0x66, 0x66, 0x66, 0x77, 0x89, 0xA8, 0x76, 0x67, 0x89, 0x99, 0x89, 0x86, 0x43, 0x43, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x12, 0x21, 0x15, 0x99, 0x99, 0x99, 0x87, 
        0x30,
  0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x56, 0x56, 0x55, 0x55, 0x55, 0x55, 0x66, 0x76, 0x77, 0x77, 0x89, 0xA9, 0x99, 0x88, 0x89, 0x98, 0x89, 0x85, 0x43, 0x32, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x16, 0x99, 0x99, 0x99, 0x87, 
        0x20,
  0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x45, 0x56, 0x65, 0x54, 0x55, 0x55, 0x56, 0x77, 0x77, 0x77, 0x8A, 0xA9, 0x99, 0x98, 0x89, 0x88, 0x89, 0x75, 0x33, 0x21, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x18, 0x99, 0x99, 0x98, 0x87, 
        0x20,
  0x33, 0x34, 0x44, 0x44, 0x44, 0x43, 0x44, 0x43, 0x33, 0x45, 0x56, 0x66, 0x44, 0x55, 0x55, 0x66, 0x77, 0x77, 0x77, 0x8A, 0xA9, 0x99, 0x99, 0x88, 0x88, 0x99, 0x74, 0x32, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x28, 0x99, 0x99, 0x98, 0x87, 
        0x20,
  0x23, 0x44, 0x43, 0x33, 0x33, 0x34, 0x33, 0x33, 0x33, 0x45, 0x55, 0x65, 0x54, 0x44, 0x55, 0x67, 0x77, 0x77, 0x77, 0x8A, 0xAA, 0x99, 0x98, 0x78, 0x88, 0x99, 0x63, 0x21, 0x11, 0x11, 0x11, 0x11, 0x10, 0x11, 0x11, 0x11, 0x11, 0x39, 0x99, 0x99, 0x98, 0x86, 
        0x20,
  0x44, 0x44, 0x33, 0x32, 0x23, 0x32, 0x22, 0x23, 0x33, 0x45, 0x56, 0x65, 0x44, 0x44, 0x56, 0x67, 0x77, 0x77, 0x77, 0x89, 0xAA, 0xA8, 0x87, 0x78, 0x88, 0x98, 0x52, 0x11, 0x11, 0x11, 0x11, 0x11, 0x10, 0x11, 0x11, 0x11, 0x11, 0x49, 0x99, 0x99, 0x98, 0x86, 
        0x20,
  0x44, 0x43, 0x33, 0x32, 0x33, 0x22, 0x21, 0x22, 0x33, 0x35, 0x56, 0x76, 0x44, 0x44, 0x46, 0x67, 0x76, 0x55, 0x66, 0x68, 0x78, 0x98, 0x77, 0x77, 0x88, 0x87, 0x42, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x69, 0x99, 0x99, 0x98, 0x85, 
        0x10,
  0x44, 0x44, 0x44, 0x43, 0x44, 0x33, 0x32, 0x22, 0x33, 0x45, 0x56, 0x75, 0x54, 0x44, 0x45, 0x67, 0x76, 0x65, 0x55, 0x65, 0x68, 0x88, 0x76, 0x67, 0x88, 0x87, 0x42, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x10, 0x11, 0x11, 0x79, 0x99, 0x99, 0x99, 0x84, 
        0x10,
  0x55, 0x54, 0x44, 0x45, 0x44, 0x43, 0x33, 0x12, 0x33, 0x46, 0x76, 0x66, 0x64, 0x44, 0x44, 0x57, 0x77, 0x77, 0x65, 0x79, 0x89, 0x87, 0x66, 0x67, 0x88, 0x76, 0x42, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x10, 0x11, 0x12, 0x8A, 0xA9, 0x99, 0x99, 0x84, 
        0x10,
  0x66, 0x55, 0x55, 0x55, 0x55, 0x54, 0x32, 0x12, 0x33, 0x48, 0x77, 0x66, 0x54, 0x33, 0x34, 0x56, 0x77, 0x77, 0x66, 0x79, 0x99, 0x76, 0x55, 0x67, 0x88, 0x75, 0x43, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x10, 0x01, 0x13, 0x9A, 0xAA, 0xA9, 0x99, 0x94, 
        0x10,
  0x55, 0x54, 0x45, 0x55, 0x44, 0x55, 0x42, 0x11, 0x33, 0x58, 0x87, 0x65, 0x54, 0x32, 0x33, 0x46, 0x67, 0x77, 0x66, 0x78, 0x99, 0x65, 0x45, 0x56, 0x77, 0x64, 0x43, 0x11, 0x11, 0x11, 0x11, 0x10, 0x00, 0x11, 0x10, 0x01, 0x14, 0x9A, 0xAA, 0xA9, 0x99, 0x83, 
        0x10,
  0x44, 0x44, 0x44, 0x44, 0x43, 0x44, 0x32, 0x11, 0x34, 0x78, 0x87, 0x65, 0x43, 0x33, 0x33, 0x34, 0x67, 0x76, 0x66, 0x78, 0x99, 0x54, 0x45, 0x66, 0x66, 0x64, 0x33, 0x11, 0x11, 0x11, 0x11, 0x10, 0x00, 0x00, 0x00, 0x01, 0x16, 0x9A, 0xAA, 0xA9, 0x99, 0x83, 
        0x10,
  0x43, 0x33, 0x34, 0x43, 0x33, 0x34, 0x33, 0x22, 0x35, 0x78, 0x97, 0x65, 0x44, 0x34, 0x33, 0x11, 0x45, 0x66, 0x66, 0x89, 0x98, 0x44, 0x45, 0x66, 0x66, 0x53, 0x32, 0x11, 0x10, 0x00, 0x01, 0x10, 0x00, 0x00, 0x00, 0x01, 0x17, 0xAA, 0xAA, 0xAA, 0x99, 0x82, 
        0x10,
  0x43, 0x32, 0x23, 0x33, 0x33, 0x44, 0x22, 0x11, 0x25, 0x56, 0x97, 0x66, 0x64, 0x34, 0x43, 0x43, 0x55, 0x54, 0x45, 0x56, 0x52, 0x25, 0x55, 0x66, 0x66, 0x41, 0x11, 0x11, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0xAA, 0xAA, 0xAA, 0x99, 0x82, 
        0x10,
  0x42, 0x33, 0x23, 0x33, 0x33, 0x42, 0x00, 0x00, 0x12, 0x25, 0x87, 0x66, 0x64, 0x44, 0x43, 0x43, 0x34, 0x44, 0x43, 0x33, 0x22, 0x45, 0x55, 0x66, 0x66, 0x41, 0x11, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0xAA, 0xAA, 0xAA, 0xA9, 0x71, 
        0x10,
  0x42, 0x44, 0x44, 0x42, 0x22, 0x10, 0x00, 0x00, 0x01, 0x14, 0x87, 0x66, 0x65, 0x43, 0x43, 0x45, 0x43, 0x44, 0x34, 0x45, 0x44, 0x55, 0x56, 0x66, 0x66, 0x41, 0x11, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x39, 0xAA, 0xAA, 0xAA, 0xA9, 0x71, 
        0x10,
  0x43, 0x45, 0x55, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0x77, 0x65, 0x66, 0x43, 0x44, 0x45, 0x54, 0x43, 0x33, 0x46, 0x75, 0x56, 0x66, 0x65, 0x66, 0x41, 0x11, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x59, 0xAA, 0xAA, 0xAA, 0xAA, 0x61, 
        0x10,
  0x66, 0x77, 0x63, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x67, 0x64, 0x56, 0x43, 0x33, 0x45, 0x55, 0x44, 0x44, 0x47, 0x87, 0x66, 0x77, 0x66, 0x66, 0x42, 0x21, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x6A, 0xAA, 0xAA, 0xAA, 0xA9, 0x51, 
        0x10,
  0x77, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x46, 0x54, 0x45, 0x44, 0x33, 0x44, 0x56, 0x65, 0x55, 0x67, 0x88, 0x77, 0x77, 0x55, 0x66, 0x43, 0x32, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x8A, 0xAA, 0xAA, 0xAA, 0xA9, 0x51, 
        0x10,
  0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x55, 0x44, 0x55, 0x33, 0x44, 0x56, 0x66, 0x67, 0x89, 0x98, 0x78, 0x76, 0x33, 0x56, 0x54, 0x43, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x9A, 0xAA, 0xAA, 0xAA, 0x99, 0x41, 
        0x10,
  0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x56, 0x43, 0x33, 0x33, 0x34, 0x45, 0x55, 0x67, 0x89, 0x87, 0x77, 0x65, 0x21, 0x24, 0x44, 0x54, 0x10, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x9A, 0xAA, 0xAA, 0xA9, 0x99, 0x31, 
        0x10,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x45, 0x44, 0x34, 0x33, 0x33, 0x44, 0x44, 0x45, 0x67, 0x77, 0x77, 0x62, 0x11, 0x11, 0x11, 0x23, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x9A, 0xA9, 0x99, 0x99, 0xA9, 0x21, 
        0x10,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x35, 0x44, 0x34, 0x34, 0x32, 0x22, 0x23, 0x45, 0x56, 0x66, 0x76, 0x41, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x9A, 0xA9, 0x99, 0x99, 0xA8, 0x21, 
        0x10,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x45, 0x44, 0x44, 0x32, 0x22, 0x12, 0x33, 0x45, 0x56, 0x66, 0x21, 0x11, 0x11, 0x11, 0x11, 0x11, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xAA, 0xA9, 0x99, 0x99, 0x97, 0x11, 
        0x10,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x45, 0x44, 0x33, 0x32, 0x12, 0x21, 0x33, 0x45, 0x55, 0x54, 0x11, 0x01, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0xAA, 0xAA, 0x99, 0x99, 0x96, 0x11, 
        0x10,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x33, 0x35, 0x33, 0x33, 0x23, 0x23, 0x43, 0x44, 0x53, 0x43, 0x01, 0x01, 0x11, 0x12, 0x11, 0x11, 0x11, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x19, 0x99, 0x9A, 0x99, 0x99, 0x96, 0x11, 
        0x10,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0x35, 0x43, 0x33, 0x34, 0x44, 0x64, 0x22, 0x23, 0x32, 0x01, 0x11, 0x11, 0x11, 0x21, 0x11, 0x11, 0x11, 0x10, 0x00, 0x00, 0x00, 0x00, 0x29, 0xAA, 0xAA, 0x99, 0x99, 0x95, 0x11, 
        0x20,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x44, 0x43, 0x33, 0x34, 0x64, 0x53, 0x10, 0x12, 0x21, 0x00, 0x11, 0x10, 0x11, 0x12, 0x21, 0x11, 0x11, 0x10, 0x00, 0x00, 0x00, 0x00, 0x49, 0xA9, 0xAA, 0x99, 0x9A, 0x94, 0x11, 
        0x30,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x33, 0x33, 0x33, 0x34, 0x54, 0x42, 0x00, 0x11, 0x11, 0x01, 0x11, 0x10, 0x01, 0x12, 0x21, 0x11, 0x22, 0x11, 0x00, 0x00, 0x00, 0x00, 0x5A, 0xAA, 0xAA, 0xAA, 0xAA, 0x93, 0x11, 
        0x30,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x22, 0x33, 0x33, 0x34, 0x32, 0x10, 0x01, 0x11, 0x00, 0x11, 0x11, 0x10, 0x01, 0x12, 0x21, 0x11, 0x22, 0x21, 0x11, 0x00, 0x00, 0x00, 0x69, 0xAA, 0xAA, 0xAA, 0xAA, 0x93, 0x12, 
        0x30
};

GUI_CONST_STORAGE GUI_BITMAP bm85 = {
  85, // xSize
  64, // ySize
  43, // BytesPerLine
  4, // BitsPerPixel
  _ac85,  // Pointer to picture data (indices)
  &_Pal85   // Pointer to palette
};

/*************************** End of file ****************************/
