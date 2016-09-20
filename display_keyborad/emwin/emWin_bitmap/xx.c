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
* Source file: xx                                                    *
* Dimensions:  92 * 64                                               *
* NumColors:   16                                                    *
*                                                                    *
**********************************************************************
*/

#include <stdlib.h>

#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

extern GUI_CONST_STORAGE GUI_BITMAP bmxx;

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
static GUI_CONST_STORAGE GUI_COLOR _Colorsxx[] = {
  0x000000, 0x111111, 0x222222, 0x333333,
  0x444444, 0x555555, 0x666666, 0x777777,
  0x888888, 0x999999, 0xAAAAAA, 0xBBBBBB,
  0xCCCCCC, 0xDDDDDD, 0xEEEEEE, 0xFFFFFF
};

static GUI_CONST_STORAGE GUI_LOGPALETTE _Palxx = {
  16,  // Number of entries
  0,   // No transparency
  &_Colorsxx[0]
};

static GUI_CONST_STORAGE unsigned char _acxx[] = {
  0x47, 0x72, 0x22, 0x22, 0x32, 0x12, 0x12, 0x11, 0x32, 0x32, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x11, 0x11, 0x12, 0x12, 0x11, 0x11, 0x10, 0x10, 0x01, 0x12, 0x34, 0x54, 0x44, 0x43, 0x33, 0x33, 0x22, 0x11, 0x10, 0x01, 0x22, 0x22, 0x22, 0x23, 0x22, 
        0x21, 0x11, 0x11, 0x11,
  0x66, 0x53, 0x32, 0x22, 0x22, 0x22, 0x11, 0x12, 0x22, 0x22, 0x11, 0x22, 0x22, 0x23, 0x22, 0x11, 0x12, 0x12, 0x11, 0x02, 0x12, 0x22, 0x22, 0x11, 0x00, 0x02, 0x46, 0x66, 0x65, 0x54, 0x44, 0x44, 0x44, 0x44, 0x44, 0x32, 0x22, 0x21, 0x11, 0x11, 0x34, 0x12, 
        0x22, 0x11, 0x11, 0x11,
  0x54, 0x42, 0x23, 0x22, 0x22, 0x22, 0x12, 0x11, 0x22, 0x11, 0x11, 0x11, 0x11, 0x25, 0x32, 0x21, 0x11, 0x22, 0x21, 0x01, 0x12, 0x22, 0x22, 0x22, 0x11, 0x26, 0x56, 0x66, 0x65, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x11, 0x12, 0x34, 0x44, 0x33, 
        0x21, 0x11, 0x11, 0x11,
  0x54, 0x33, 0x32, 0x23, 0x21, 0x21, 0x11, 0x11, 0x13, 0x11, 0x11, 0x11, 0x11, 0x12, 0x33, 0x11, 0x11, 0x21, 0x12, 0x12, 0x22, 0x23, 0x32, 0x22, 0x22, 0x66, 0x45, 0x66, 0x55, 0x54, 0x43, 0x44, 0x44, 0x56, 0x54, 0x44, 0x45, 0x52, 0x02, 0x64, 0x45, 0x32, 
        0x11, 0x11, 0x11, 0x11,
  0x44, 0x33, 0x22, 0x33, 0x22, 0x22, 0x22, 0x21, 0x12, 0x21, 0x11, 0x12, 0x11, 0x11, 0x21, 0x11, 0x11, 0x21, 0x10, 0x22, 0x22, 0x33, 0x32, 0x22, 0x25, 0x76, 0x64, 0x45, 0x55, 0x55, 0x44, 0x45, 0x45, 0xA8, 0x54, 0x55, 0x78, 0x87, 0x65, 0x54, 0x44, 0x33, 
        0x11, 0x11, 0x11, 0x11,
  0x23, 0x32, 0x22, 0x33, 0x21, 0x12, 0x22, 0x21, 0x11, 0x11, 0x11, 0x21, 0x22, 0x11, 0x21, 0x11, 0x11, 0x11, 0x11, 0x12, 0x23, 0x33, 0x33, 0x32, 0x36, 0x67, 0x55, 0x44, 0x55, 0x45, 0x45, 0x44, 0x59, 0x86, 0x55, 0x57, 0xA9, 0x98, 0x87, 0x65, 0x44, 0x44, 
        0x21, 0x11, 0x11, 0x11,
  0x43, 0x32, 0x22, 0x33, 0x22, 0x22, 0x22, 0x21, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22, 0x21, 0x11, 0x21, 0x11, 0x23, 0x23, 0x33, 0x33, 0x33, 0x67, 0x66, 0x66, 0x54, 0x45, 0x45, 0x44, 0x54, 0x68, 0x95, 0x55, 0x6A, 0xBA, 0xA9, 0x88, 0x76, 0x65, 0x44, 
        0x21, 0x11, 0x11, 0x11,
  0x53, 0x33, 0x23, 0x32, 0x33, 0x22, 0x22, 0x22, 0x21, 0x21, 0x22, 0x32, 0x22, 0x22, 0x21, 0x22, 0x11, 0x11, 0x11, 0x33, 0x23, 0x33, 0x33, 0x35, 0x66, 0x76, 0x65, 0x54, 0x54, 0x54, 0x44, 0x65, 0x56, 0x65, 0x56, 0x9C, 0xBB, 0xAA, 0x99, 0x88, 0x66, 0x66, 
        0x31, 0x11, 0x11, 0x11,
  0x54, 0x33, 0x23, 0x32, 0x32, 0x32, 0x33, 0x21, 0x11, 0x12, 0x44, 0x33, 0x32, 0x22, 0x21, 0x22, 0x12, 0x21, 0x11, 0x33, 0x33, 0x33, 0x33, 0x46, 0x66, 0x66, 0x65, 0x54, 0x44, 0x54, 0x35, 0x55, 0x66, 0x66, 0x66, 0xBC, 0xCC, 0xBB, 0xAA, 0x98, 0x87, 0x78, 
        0x75, 0x22, 0x22, 0x22,
  0x54, 0x54, 0x33, 0x33, 0x31, 0x23, 0x33, 0x33, 0x11, 0x13, 0x42, 0x12, 0x22, 0x22, 0x13, 0x55, 0x42, 0x22, 0x11, 0x33, 0x33, 0x44, 0x43, 0x45, 0x55, 0x66, 0x66, 0x53, 0x34, 0x44, 0x45, 0x55, 0x66, 0x66, 0x69, 0xDD, 0xDC, 0xCB, 0xBB, 0xA9, 0x88, 0x78, 
        0x76, 0x43, 0x33, 0x33,
  0x44, 0x44, 0x32, 0x23, 0x22, 0x13, 0x22, 0x22, 0x22, 0x54, 0x21, 0x12, 0x22, 0x12, 0x13, 0x67, 0x77, 0x53, 0x11, 0x33, 0x33, 0x44, 0x45, 0x55, 0x55, 0x55, 0x55, 0x54, 0x32, 0x34, 0x44, 0x56, 0x66, 0x65, 0x6B, 0xED, 0xDD, 0xDC, 0xCB, 0xA9, 0x88, 0x88, 
        0x76, 0x53, 0x33, 0x33,
  0x44, 0x43, 0x32, 0x12, 0x21, 0x11, 0x11, 0x11, 0x14, 0x45, 0x32, 0x22, 0x22, 0x22, 0x22, 0x46, 0x68, 0x98, 0x31, 0x32, 0x33, 0x44, 0x44, 0x44, 0x45, 0x56, 0x66, 0x65, 0x43, 0x22, 0x34, 0x46, 0x66, 0x66, 0x7D, 0xED, 0xDE, 0xEE, 0xDC, 0xBA, 0x88, 0x88, 
        0x76, 0x53, 0x34, 0x44,
  0x22, 0x22, 0x22, 0x12, 0x22, 0x11, 0x21, 0x21, 0x24, 0x65, 0x33, 0x33, 0x33, 0x45, 0x66, 0x66, 0x77, 0x77, 0x42, 0x22, 0x23, 0x34, 0x54, 0x44, 0x44, 0x56, 0x66, 0x65, 0x43, 0x21, 0x13, 0x35, 0x66, 0x66, 0x7D, 0xEE, 0xDD, 0xEE, 0xED, 0xCA, 0x88, 0x77, 
        0x77, 0x53, 0x34, 0x44,
  0x11, 0x11, 0x21, 0x11, 0x22, 0x21, 0x11, 0x12, 0x44, 0x53, 0x34, 0x67, 0x77, 0x88, 0x78, 0x88, 0x78, 0x76, 0x65, 0x43, 0x32, 0x26, 0x74, 0x44, 0x44, 0x55, 0x66, 0x65, 0x53, 0x22, 0x21, 0x23, 0x55, 0x66, 0x7D, 0xEE, 0xDC, 0xCD, 0xED, 0xCA, 0x98, 0x77, 
        0x76, 0x64, 0x34, 0x44,
  0x33, 0x33, 0x22, 0x12, 0x11, 0x11, 0x21, 0x13, 0x34, 0x54, 0x47, 0x88, 0x88, 0x88, 0x79, 0x98, 0x88, 0x87, 0x66, 0x55, 0x55, 0x56, 0x85, 0x43, 0x44, 0x55, 0x66, 0x65, 0x44, 0x44, 0x33, 0x33, 0x23, 0x44, 0x6B, 0xDD, 0xCB, 0xBB, 0xDE, 0xDB, 0x98, 0x77, 
        0x77, 0x64, 0x23, 0x34,
  0x32, 0x22, 0x22, 0x22, 0x11, 0x12, 0x22, 0x23, 0x34, 0x33, 0x68, 0x99, 0xA9, 0x88, 0x8A, 0xB9, 0x88, 0x88, 0x76, 0x66, 0x66, 0x66, 0x67, 0x33, 0x34, 0x55, 0x56, 0x65, 0x55, 0x54, 0x44, 0x33, 0x33, 0x23, 0x22, 0x33, 0x43, 0x33, 0x8E, 0xDC, 0xA8, 0x87, 
        0x67, 0x65, 0x22, 0x22,
  0x23, 0x42, 0x23, 0x32, 0x11, 0x11, 0x22, 0x22, 0x23, 0x44, 0x79, 0xAA, 0xA8, 0x98, 0x8C, 0xCA, 0x88, 0x88, 0x86, 0x66, 0x66, 0x67, 0x67, 0x53, 0x44, 0x45, 0x55, 0x65, 0x65, 0x55, 0x44, 0x44, 0x44, 0x33, 0x32, 0x22, 0x22, 0x22, 0x6E, 0xEC, 0xB9, 0x87, 
        0x66, 0x66, 0x22, 0x22,
  0x44, 0x43, 0x33, 0x32, 0x22, 0x22, 0x22, 0x23, 0x23, 0x55, 0x89, 0xBB, 0xB8, 0xB6, 0x8C, 0xCA, 0x98, 0x88, 0x87, 0x66, 0x66, 0x66, 0x76, 0x73, 0x34, 0x45, 0x55, 0x66, 0x66, 0x55, 0x55, 0x44, 0x44, 0x43, 0x32, 0x22, 0x22, 0x22, 0x5E, 0xED, 0xBA, 0x88, 
        0x76, 0x66, 0x32, 0x22,
  0x34, 0x34, 0x44, 0x33, 0x32, 0x23, 0x22, 0x33, 0x34, 0x57, 0x9B, 0xBC, 0xB8, 0xB7, 0x9D, 0xB9, 0x88, 0x88, 0x87, 0x76, 0x66, 0x66, 0x77, 0x75, 0x34, 0x45, 0x55, 0x56, 0x66, 0x65, 0x55, 0x55, 0x44, 0x43, 0x33, 0x32, 0x22, 0x22, 0x3D, 0xEE, 0xCA, 0x98, 
        0x76, 0x66, 0x31, 0x22,
  0x34, 0x34, 0x43, 0x33, 0x33, 0x33, 0x22, 0x34, 0x33, 0x48, 0xAB, 0xCD, 0xB8, 0xA7, 0xAD, 0x98, 0x88, 0x66, 0x78, 0x76, 0x66, 0x66, 0x77, 0x77, 0x34, 0x45, 0x55, 0x56, 0x66, 0x66, 0x55, 0x55, 0x54, 0x44, 0x33, 0x22, 0x22, 0x33, 0x3C, 0xEE, 0xDB, 0x98, 
        0x76, 0x66, 0x41, 0x11,
  0x23, 0x32, 0x32, 0x33, 0x23, 0x33, 0x23, 0x34, 0x43, 0x48, 0xBC, 0xCE, 0xC9, 0x98, 0xAA, 0x87, 0x78, 0x55, 0x78, 0x86, 0x66, 0x65, 0x77, 0x88, 0x44, 0x44, 0x55, 0x56, 0x68, 0xC8, 0x65, 0x55, 0x54, 0x44, 0x33, 0x33, 0x33, 0x33, 0x39, 0xEE, 0xDB, 0x98, 
        0x76, 0x66, 0x41, 0x11,
  0x43, 0x22, 0x32, 0x22, 0x22, 0x23, 0x22, 0x35, 0x65, 0x69, 0xCC, 0xDE, 0xCA, 0xBA, 0xAA, 0xA7, 0x78, 0x77, 0x78, 0x88, 0x66, 0x66, 0x67, 0x88, 0x54, 0x44, 0x55, 0x56, 0x6A, 0xDE, 0xA6, 0x65, 0x54, 0x44, 0x33, 0x33, 0x33, 0x22, 0x37, 0xEE, 0xEC, 0xA8, 
        0x76, 0x66, 0x51, 0x11,
  0x34, 0x22, 0x32, 0x22, 0x11, 0x22, 0x32, 0x35, 0x65, 0x7B, 0xDD, 0xED, 0xAA, 0xCC, 0xDD, 0xB7, 0x77, 0x88, 0x77, 0x98, 0x66, 0x66, 0x67, 0x88, 0x74, 0x45, 0x55, 0x56, 0x6B, 0xCC, 0xDB, 0xA7, 0x55, 0x44, 0x33, 0x33, 0x33, 0x33, 0x35, 0xDE, 0xED, 0xB9, 
        0x87, 0x66, 0x51, 0x11,
  0x32, 0x32, 0x33, 0x11, 0x11, 0x22, 0x22, 0x25, 0x56, 0x7B, 0xEE, 0xED, 0x98, 0x9B, 0xDD, 0xC8, 0x87, 0x98, 0x78, 0x98, 0x66, 0x67, 0x77, 0x89, 0x94, 0x44, 0x55, 0x55, 0x8B, 0xB9, 0xDB, 0xAA, 0xA8, 0x54, 0x34, 0x44, 0x44, 0x44, 0x44, 0xBE, 0xED, 0xB9, 
        0x87, 0x66, 0x51, 0x11,
  0x33, 0x31, 0x32, 0x21, 0x12, 0x23, 0x22, 0x23, 0x55, 0x6A, 0xDE, 0xEC, 0x8A, 0x6A, 0xDA, 0xB9, 0xA7, 0x98, 0x89, 0x98, 0x67, 0x77, 0x78, 0x8A, 0xA4, 0x44, 0x55, 0x55, 0x7D, 0xDC, 0xEB, 0x99, 0xA9, 0x99, 0x75, 0x44, 0x44, 0x44, 0x43, 0x9E, 0xED, 0xB9, 
        0x87, 0x66, 0x51, 0x11,
  0x33, 0x13, 0x42, 0x32, 0x11, 0x12, 0x32, 0x22, 0x55, 0x58, 0xCE, 0xEC, 0xAB, 0x7A, 0xDA, 0x89, 0xA7, 0x68, 0x9B, 0xA8, 0x77, 0x78, 0x88, 0x9B, 0xB4, 0x44, 0x55, 0x55, 0x5A, 0xCC, 0xDB, 0xBB, 0x98, 0x88, 0x98, 0x89, 0x75, 0x55, 0x54, 0x6E, 0xED, 0xB9, 
        0x87, 0x66, 0x51, 0x11,
  0x31, 0x43, 0x33, 0x12, 0x11, 0x11, 0x22, 0x22, 0x36, 0x65, 0x7B, 0xDC, 0xAB, 0x9C, 0xEB, 0x88, 0x87, 0x88, 0x9A, 0x97, 0x77, 0x88, 0x88, 0xAC, 0xB4, 0x45, 0x55, 0x55, 0x57, 0xDC, 0xCB, 0xBB, 0xA9, 0x69, 0x77, 0x9A, 0x89, 0x98, 0x65, 0x5D, 0xED, 0xB9, 
        0x87, 0x66, 0x51, 0x11,
  0x23, 0x31, 0x52, 0x23, 0x21, 0x11, 0x12, 0x12, 0x24, 0x66, 0x55, 0x7A, 0xB9, 0xBD, 0xEC, 0x98, 0x89, 0x88, 0x88, 0x77, 0x78, 0x88, 0x89, 0xBD, 0xB4, 0x45, 0x55, 0x55, 0x58, 0xCB, 0xCB, 0x98, 0x97, 0x58, 0x9A, 0xA8, 0x89, 0x99, 0xA9, 0x6B, 0xEE, 0xB9, 
        0x87, 0x66, 0x51, 0x11,
  0x42, 0x13, 0x43, 0x53, 0x21, 0x11, 0x11, 0x11, 0x12, 0x25, 0x55, 0x54, 0x67, 0x69, 0xDC, 0xA9, 0x89, 0x98, 0x75, 0xBC, 0x9B, 0xAA, 0xAB, 0xDE, 0x84, 0x55, 0x55, 0x55, 0x58, 0xC9, 0x99, 0x9B, 0xA6, 0x58, 0xBA, 0x99, 0xA9, 0x79, 0xA9, 0x79, 0xEE, 0xC9, 
        0x87, 0x66, 0x53, 0x44,
  0x33, 0x35, 0x46, 0x21, 0x11, 0x11, 0x11, 0x11, 0x11, 0x13, 0x34, 0x55, 0x46, 0x87, 0x9A, 0xAA, 0x98, 0x76, 0x43, 0x9D, 0x9B, 0xDD, 0xDD, 0xEB, 0x54, 0x55, 0x55, 0x55, 0x59, 0xAC, 0xB8, 0x99, 0x98, 0x76, 0x67, 0x9A, 0xA8, 0x9B, 0xA8, 0x8B, 0xEE, 0xC9, 
        0x87, 0x66, 0x43, 0x33,
  0x44, 0x54, 0x65, 0x41, 0x11, 0x11, 0x11, 0x11, 0x11, 0x12, 0x22, 0x44, 0x34, 0x66, 0x55, 0x67, 0x76, 0x44, 0x54, 0x8D, 0xA9, 0xEE, 0xEE, 0xC6, 0x45, 0x55, 0x55, 0x55, 0x59, 0xDB, 0x9A, 0xA9, 0x87, 0xBC, 0xCA, 0x97, 0x79, 0xBA, 0x9A, 0xCC, 0xEE, 0xC9, 
        0x87, 0x66, 0x54, 0x57,
  0x34, 0x57, 0x66, 0x21, 0x11, 0x21, 0x11, 0x21, 0x11, 0x14, 0x23, 0x33, 0x22, 0x33, 0x34, 0x66, 0x33, 0x21, 0x35, 0x5D, 0xA7, 0x9B, 0xB9, 0x65, 0x55, 0x55, 0x55, 0x55, 0x6C, 0xBD, 0xDB, 0x89, 0xCB, 0x77, 0x9A, 0xAB, 0xB9, 0x68, 0xAB, 0xAA, 0xEE, 0xDA, 
        0x87, 0x76, 0x55, 0x43,
  0x35, 0x67, 0x75, 0x31, 0x11, 0x21, 0x11, 0x21, 0x11, 0x14, 0x33, 0x43, 0x32, 0x23, 0x33, 0x44, 0x23, 0x21, 0x26, 0x4D, 0x95, 0x76, 0x66, 0x56, 0x55, 0x55, 0x55, 0x55, 0x6C, 0xEC, 0xB9, 0xBB, 0x8A, 0xDA, 0x89, 0xAA, 0xAC, 0xCA, 0x76, 0x8B, 0xDE, 0xDA, 
        0x87, 0x76, 0x55, 0x58,
  0x56, 0x88, 0x64, 0x31, 0x11, 0x11, 0x22, 0x22, 0x33, 0x44, 0x33, 0x24, 0x44, 0x44, 0x45, 0x44, 0x21, 0x21, 0x14, 0x7C, 0x95, 0x47, 0x66, 0x66, 0x65, 0x55, 0x55, 0x56, 0xBC, 0xEE, 0xCC, 0x99, 0xC8, 0x8A, 0x9C, 0xB9, 0x9A, 0xAB, 0xCB, 0x97, 0xBE, 0xDA, 
        0x88, 0x76, 0x73, 0x23,
  0x58, 0x87, 0x34, 0x21, 0x22, 0x21, 0x12, 0x22, 0x34, 0x44, 0x52, 0x33, 0x31, 0x22, 0x35, 0x53, 0x21, 0x21, 0x12, 0xAA, 0x85, 0x25, 0x66, 0x56, 0x55, 0x55, 0x55, 0x55, 0xCD, 0xCD, 0xA9, 0xBC, 0x8C, 0xCA, 0x87, 0xAC, 0xA8, 0x8A, 0xAC, 0xCB, 0xCE, 0xEB, 
        0x87, 0x76, 0x62, 0x25,
  0x88, 0x86, 0x53, 0x22, 0x21, 0x11, 0x11, 0x12, 0x33, 0x34, 0x45, 0x55, 0x32, 0x23, 0x55, 0x44, 0x21, 0x22, 0x13, 0xC9, 0x65, 0x23, 0x56, 0x55, 0x55, 0x55, 0x54, 0x45, 0xCD, 0xCA, 0xBC, 0x96, 0x65, 0x8B, 0x98, 0x77, 0xAA, 0x97, 0x8B, 0x9A, 0xAE, 0xEC, 
        0x98, 0x76, 0x51, 0x15,
  0x88, 0x74, 0x33, 0x32, 0x11, 0x11, 0x11, 0x22, 0x23, 0x34, 0x34, 0x43, 0x66, 0x55, 0x47, 0x55, 0x21, 0x12, 0x28, 0xA7, 0x54, 0x34, 0x67, 0x87, 0x97, 0x6A, 0xA8, 0x44, 0x6A, 0xDD, 0xCD, 0xB4, 0x33, 0x4A, 0xA6, 0x66, 0x66, 0x89, 0x97, 0x8B, 0xCE, 0xEC, 
        0x98, 0x76, 0x54, 0x32,
  0x87, 0x56, 0x63, 0x21, 0x11, 0x12, 0x21, 0x22, 0x22, 0x36, 0x54, 0x43, 0x67, 0x66, 0x68, 0x64, 0x32, 0x22, 0x4C, 0xA9, 0xAB, 0xDE, 0xEE, 0xD9, 0xDC, 0xAC, 0xDE, 0xB9, 0x99, 0xAB, 0xDE, 0xE6, 0x34, 0x7B, 0x99, 0x75, 0x67, 0x65, 0x8B, 0xBC, 0xCE, 0xEC, 
        0x98, 0x76, 0x56, 0x65,
  0x78, 0x85, 0x34, 0x31, 0x22, 0x33, 0x21, 0x11, 0x36, 0x88, 0x76, 0x67, 0x88, 0x87, 0x88, 0x74, 0x42, 0x32, 0x9D, 0xEE, 0xEE, 0xEE, 0xEE, 0xEB, 0xDD, 0xDC, 0xCD, 0xAC, 0x9C, 0xDA, 0x9D, 0xE7, 0x23, 0x5B, 0x75, 0x56, 0x65, 0x8B, 0xBD, 0xA8, 0xBE, 0xEC, 
        0x98, 0x77, 0x55, 0x66,
  0x86, 0x43, 0x33, 0x32, 0x22, 0x22, 0x22, 0x12, 0x7A, 0xA9, 0x88, 0x89, 0x99, 0xBA, 0x98, 0x87, 0x44, 0x33, 0xDE, 0xDE, 0xEE, 0xEE, 0xEC, 0xEC, 0xDD, 0xDC, 0xBC, 0xAC, 0xBE, 0xD9, 0xBD, 0xD8, 0x78, 0x87, 0x55, 0x68, 0x8A, 0xAA, 0x9A, 0xBA, 0x9E, 0xED, 
        0xA8, 0x87, 0x55, 0x66,
  0x87, 0x66, 0x55, 0x54, 0x43, 0x33, 0x32, 0x26, 0xBC, 0xBA, 0x98, 0x98, 0x88, 0xAB, 0xB9, 0x88, 0x76, 0x46, 0xD9, 0x9E, 0xED, 0xDE, 0xED, 0xDA, 0xDD, 0xCD, 0xAB, 0xBB, 0xCE, 0xC8, 0xCC, 0xC8, 0x77, 0x64, 0x68, 0xCD, 0xAA, 0xB8, 0x9A, 0x9C, 0xCE, 0xED, 
        0xA8, 0x87, 0x65, 0x67,
  0x88, 0x65, 0x54, 0x44, 0x44, 0x44, 0x43, 0x49, 0xDD, 0xCB, 0x98, 0x88, 0x76, 0x9B, 0xA9, 0x98, 0x87, 0x57, 0x98, 0x9E, 0xED, 0xCE, 0xED, 0xE8, 0xCE, 0xBE, 0xAA, 0xBA, 0xDE, 0x98, 0xDC, 0x87, 0x65, 0x49, 0x8A, 0xDD, 0xC8, 0xCA, 0x89, 0x9D, 0xEE, 0xEC, 
        0xA8, 0x87, 0x65, 0x77,
  0x78, 0x88, 0x73, 0x34, 0x22, 0x23, 0x33, 0x7A, 0xDE, 0xDB, 0x99, 0x88, 0x76, 0xAA, 0x99, 0x97, 0x88, 0x66, 0x78, 0x9D, 0xEE, 0xDD, 0xED, 0xC9, 0xAE, 0xAD, 0xBA, 0xCA, 0xEE, 0x7A, 0xCB, 0x86, 0x55, 0x8B, 0x8B, 0xCD, 0xD9, 0xAC, 0x87, 0xCE, 0xDC, 0xDB, 
        0xA8, 0x87, 0x75, 0x78,
  0x55, 0x55, 0x43, 0x32, 0x33, 0x33, 0x33, 0x8B, 0xDE, 0xDB, 0x99, 0x88, 0x88, 0xBB, 0x88, 0x85, 0x78, 0x75, 0x67, 0x9D, 0xEE, 0xED, 0xBA, 0x89, 0xAD, 0x9D, 0xB9, 0xDB, 0xED, 0x7B, 0xDC, 0xA6, 0x58, 0xCB, 0x8B, 0xCD, 0xCB, 0x8C, 0x99, 0xED, 0xCB, 0xBA, 
        0x98, 0x87, 0x76, 0x78,
  0x65, 0x33, 0x33, 0x32, 0x33, 0x32, 0x24, 0xAD, 0xDE, 0xDB, 0x99, 0x98, 0x89, 0xCD, 0x99, 0x86, 0x78, 0x75, 0x66, 0xBE, 0xEE, 0xED, 0xDA, 0x99, 0xBC, 0x9C, 0xC8, 0xEC, 0xED, 0x8C, 0xDC, 0xB7, 0x78, 0xDB, 0x8A, 0xCC, 0xBC, 0x89, 0x9C, 0xEC, 0xBA, 0xA9, 
        0x88, 0x87, 0x76, 0x78,
  0x67, 0x75, 0x32, 0x22, 0x22, 0x22, 0x35, 0xBD, 0xDE, 0xDB, 0x99, 0x88, 0x89, 0xBC, 0x99, 0x96, 0x88, 0x75, 0x67, 0xDE, 0xEE, 0xEC, 0xEC, 0xA8, 0xBD, 0x8C, 0xD7, 0xED, 0xED, 0x9D, 0xCA, 0xA9, 0x88, 0xDA, 0x8A, 0xCA, 0x9E, 0xB6, 0x9E, 0xDC, 0xBB, 0x98, 
        0x88, 0x87, 0x76, 0x78,
  0x65, 0x68, 0x74, 0x33, 0x22, 0x22, 0x25, 0xBD, 0xDE, 0xEB, 0xA9, 0x88, 0x87, 0xAB, 0xAA, 0x98, 0x88, 0x75, 0x79, 0xEE, 0xEE, 0xED, 0xCD, 0xA8, 0x9D, 0x8C, 0xC7, 0xDC, 0xEC, 0xBE, 0xA9, 0x9A, 0x9A, 0xDA, 0x8A, 0xA8, 0x9E, 0xB7, 0xBD, 0xCB, 0xBB, 0x98, 
        0x88, 0x77, 0x64, 0x55,
  0x66, 0x46, 0x76, 0x43, 0x21, 0x22, 0x25, 0xBD, 0xDE, 0xEB, 0xA9, 0x88, 0x75, 0x9B, 0xAA, 0x99, 0x88, 0x66, 0x8D, 0xEE, 0xCB, 0xBD, 0xBE, 0xC8, 0x9D, 0x9D, 0xB8, 0xDC, 0xEB, 0xDD, 0x98, 0x9C, 0xBC, 0xC9, 0x99, 0x97, 0x9D, 0xA9, 0xDC, 0xBA, 0xBB, 0x98, 
        0x88, 0x77, 0x62, 0x22,
  0x77, 0x86, 0x53, 0x32, 0x22, 0x11, 0x25, 0xAD, 0xEE, 0xDB, 0xA9, 0x98, 0x87, 0xAB, 0xB9, 0x99, 0x98, 0x78, 0xAE, 0xEA, 0x99, 0x88, 0xBD, 0xEA, 0x8D, 0x9D, 0xA8, 0xDD, 0xEC, 0xED, 0x87, 0x8D, 0xCD, 0xDB, 0x99, 0x87, 0xAD, 0xCD, 0xDA, 0x9B, 0xCC, 0x98, 
        0x88, 0x77, 0x62, 0x22,
  0x58, 0x78, 0x66, 0x53, 0x34, 0x32, 0x24, 0x9D, 0xEE, 0xDB, 0xBA, 0xAA, 0x9A, 0xBC, 0xCA, 0x99, 0x98, 0x9A, 0xDE, 0xAA, 0xB9, 0x88, 0x9C, 0xDB, 0x8C, 0x9D, 0xA9, 0xDD, 0xEE, 0xED, 0x87, 0x7C, 0xDC, 0xDD, 0xB9, 0x88, 0xAB, 0xEE, 0xB9, 0x8B, 0xDC, 0xA8, 
        0x88, 0x77, 0x51, 0x22,
  0x76, 0x87, 0x74, 0x45, 0x54, 0x33, 0x33, 0x8C, 0xED, 0xCB, 0xBA, 0xAB, 0xBB, 0xCC, 0xBB, 0xA9, 0x77, 0xBC, 0xEA, 0x8D, 0xC8, 0x88, 0x8C, 0xCC, 0x9B, 0x9D, 0xA9, 0xCE, 0xEE, 0xED, 0x87, 0x6A, 0xC9, 0xAD, 0xDB, 0x88, 0xA9, 0xED, 0xA8, 0x89, 0xCC, 0xA8, 
        0x88, 0x66, 0x43, 0x32,
  0x67, 0x78, 0x65, 0x43, 0x45, 0x44, 0x33, 0x49, 0xCC, 0xBA, 0x99, 0xAB, 0xCC, 0xCB, 0xBA, 0x87, 0x54, 0x8D, 0xC8, 0xBC, 0xB9, 0x67, 0x8B, 0xCC, 0x9B, 0x9D, 0xA9, 0xDE, 0xEE, 0xED, 0x97, 0x6B, 0xAC, 0x8A, 0xDD, 0xA8, 0x9B, 0xED, 0xA9, 0x99, 0xAD, 0xB9, 
        0x88, 0x76, 0x45, 0x44,
  0x86, 0x77, 0x65, 0x54, 0x33, 0x34, 0x45, 0x57, 0x9B, 0xA9, 0x98, 0x89, 0x9A, 0x99, 0x9A, 0x67, 0x54, 0x4D, 0xAC, 0xCC, 0xB9, 0x76, 0x69, 0xCD, 0x7C, 0xAC, 0xB9, 0xEE, 0xEE, 0xEC, 0xC9, 0x6B, 0xAC, 0x98, 0xBC, 0xDA, 0x8A, 0xAD, 0xDB, 0xA9, 0xAE, 0xC9, 
        0x88, 0x65, 0x55, 0x55,
  0x79, 0x87, 0x64, 0x56, 0x65, 0x43, 0x34, 0x56, 0x89, 0xA9, 0x98, 0x77, 0x78, 0x87, 0x9A, 0x76, 0x64, 0x3C, 0xCD, 0xCC, 0xCB, 0xA8, 0x77, 0xAD, 0x79, 0xBA, 0xC8, 0xDE, 0xEE, 0xEB, 0xCC, 0x79, 0xCB, 0x99, 0xBA, 0xBC, 0xBA, 0x79, 0xED, 0xB9, 0xDD, 0xA7, 
        0x97, 0x55, 0x65, 0x65,
  0x57, 0x8A, 0x97, 0x45, 0x66, 0x63, 0x34, 0x55, 0x46, 0x77, 0x77, 0x65, 0x98, 0x88, 0x86, 0x67, 0x75, 0x59, 0xDE, 0xDC, 0xCC, 0xBA, 0x98, 0x87, 0x88, 0xB9, 0xC9, 0xCC, 0xEE, 0xEB, 0xAD, 0x98, 0xDA, 0x9A, 0xAB, 0x99, 0xCB, 0x85, 0xAD, 0xCB, 0xED, 0x98, 
        0xA8, 0x65, 0x66, 0x56,
  0x44, 0x45, 0x67, 0x86, 0x44, 0x66, 0x42, 0x23, 0x24, 0x54, 0x66, 0x66, 0x98, 0x77, 0x66, 0x68, 0x54, 0x47, 0xCE, 0xED, 0xCC, 0xCB, 0xAA, 0x98, 0x87, 0x97, 0xBC, 0xDB, 0xEE, 0xEB, 0xAC, 0xC7, 0xC9, 0x8A, 0xBA, 0xA9, 0x9C, 0xA4, 0x6D, 0xCC, 0xEB, 0x79, 
        0x97, 0x86, 0x66, 0x67,
  0x54, 0x44, 0x42, 0x57, 0x85, 0x44, 0x65, 0x22, 0x12, 0x44, 0x67, 0x66, 0x66, 0x78, 0x87, 0x68, 0x53, 0x44, 0xAE, 0xEE, 0xDC, 0xCC, 0xCB, 0xA9, 0x98, 0x88, 0x88, 0xBB, 0xDD, 0xED, 0xCB, 0xD9, 0xC9, 0x8A, 0xBA, 0xA9, 0x8A, 0xC6, 0x5C, 0xCE, 0xD9, 0x7A, 
        0x98, 0x88, 0x77, 0x66,
  0x75, 0x77, 0x75, 0x54, 0x57, 0x87, 0x44, 0x21, 0x12, 0x44, 0x78, 0x77, 0x89, 0x76, 0x67, 0x87, 0x73, 0x22, 0x59, 0xDE, 0xED, 0xDD, 0xDC, 0xBB, 0xA9, 0x98, 0x88, 0x99, 0x9D, 0xDC, 0xDB, 0xDC, 0xC8, 0xA9, 0xAB, 0xA9, 0x88, 0xB9, 0x5A, 0xDD, 0xA8, 0x8A, 
        0x89, 0x89, 0x99, 0x77,
  0x35, 0x56, 0x34, 0x44, 0x54, 0x55, 0x77, 0x41, 0x13, 0x34, 0x77, 0x68, 0x8A, 0xB9, 0x88, 0x87, 0x66, 0x35, 0x57, 0x8C, 0xEE, 0xDD, 0xDD, 0xCC, 0xBB, 0xA9, 0x89, 0xA9, 0x99, 0x8B, 0xCD, 0xBD, 0xD9, 0x9C, 0xA9, 0xAA, 0x88, 0x99, 0x69, 0xDA, 0x88, 0x99, 
        0x88, 0x9B, 0xBB, 0x99,
  0x44, 0x33, 0x52, 0x44, 0x43, 0x21, 0x12, 0x46, 0x34, 0x43, 0x77, 0x66, 0x79, 0xA9, 0x9A, 0xA7, 0x43, 0x26, 0x65, 0x36, 0x9C, 0xDD, 0xDD, 0xDD, 0xCC, 0xCB, 0xAB, 0xBB, 0xA9, 0x87, 0x77, 0xAB, 0xDC, 0xAB, 0xCB, 0x99, 0x99, 0x87, 0x57, 0xA9, 0x88, 0x53, 
        0x22, 0x27, 0xAB, 0xAB,
  0x33, 0x21, 0x25, 0x54, 0x32, 0x11, 0x11, 0x11, 0x23, 0x43, 0x58, 0x89, 0x88, 0x99, 0x9A, 0xA5, 0x43, 0x67, 0x55, 0x12, 0x12, 0x37, 0xAC, 0xCC, 0xDD, 0xDD, 0xDD, 0xBB, 0xBA, 0x98, 0x76, 0x46, 0xAB, 0xBB, 0xBB, 0x98, 0x89, 0x88, 0x77, 0x88, 0x95, 0x11, 
        0x11, 0x11, 0x25, 0x8C,
  0x43, 0x22, 0x22, 0x54, 0x21, 0x11, 0x11, 0x11, 0x12, 0x21, 0x45, 0x67, 0x88, 0x88, 0x88, 0x87, 0x66, 0x88, 0x63, 0x11, 0x24, 0x42, 0x22, 0x45, 0x68, 0xAB, 0xDE, 0xDC, 0xBA, 0xA9, 0x88, 0x87, 0x7B, 0xBA, 0x98, 0x78, 0x8A, 0x89, 0x88, 0x87, 0x42, 0x11, 
        0x11, 0x11, 0x11, 0x13,
  0x54, 0x34, 0x43, 0x54, 0x21, 0x12, 0x11, 0x12, 0x24, 0x22, 0x23, 0x44, 0x45, 0x56, 0x67, 0x88, 0x78, 0x88, 0x52, 0x13, 0x62, 0x41, 0x11, 0x13, 0x23, 0x22, 0x4A, 0xDD, 0xCB, 0xBA, 0xA9, 0x98, 0x86, 0x8B, 0xBA, 0xA9, 0x88, 0x88, 0x64, 0x33, 0x33, 0x21, 
        0x11, 0x11, 0x11, 0x11,
  0x63, 0x34, 0x33, 0x65, 0x21, 0x12, 0x12, 0x11, 0x12, 0x12, 0x23, 0x43, 0x23, 0x33, 0x33, 0x46, 0x65, 0x63, 0x42, 0x14, 0x32, 0x31, 0x11, 0x12, 0x22, 0x21, 0x12, 0x6B, 0xDD, 0xCC, 0xBB, 0xBA, 0x98, 0x8B, 0xDC, 0x98, 0x89, 0x83, 0x12, 0x22, 0x22, 0x21, 
        0x11, 0x11, 0x11, 0x11
};

GUI_CONST_STORAGE GUI_BITMAP bmxx = {
  92, // xSize
  64, // ySize
  46, // BytesPerLine
  4, // BitsPerPixel
  _acxx,  // Pointer to picture data (indices)
  &_Palxx   // Pointer to palette
};

/*************************** End of file ****************************/
