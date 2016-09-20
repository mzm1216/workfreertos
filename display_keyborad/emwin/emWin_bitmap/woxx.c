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
* Source file: woxx                                                  *
* Dimensions:  107 * 64                                              *
* NumColors:   16                                                    *
*                                                                    *
**********************************************************************
*/

#include <stdlib.h>

#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

extern GUI_CONST_STORAGE GUI_BITMAP bmwoxx;

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
static GUI_CONST_STORAGE GUI_COLOR _Colorswoxx[] = {
  0x000000, 0x111111, 0x222222, 0x333333,
  0x444444, 0x555555, 0x666666, 0x777777,
  0x888888, 0x999999, 0xAAAAAA, 0xBBBBBB,
  0xCCCCCC, 0xDDDDDD, 0xEEEEEE, 0xFFFFFF
};

static GUI_CONST_STORAGE GUI_LOGPALETTE _Palwoxx = {
  16,  // Number of entries
  0,   // No transparency
  &_Colorswoxx[0]
};

static GUI_CONST_STORAGE unsigned char _acwoxx[] = {
  0x11, 0x11, 0x11, 0x22, 0x79, 0x46, 0x75, 0x23, 0x44, 0x45, 0x43, 0x33, 0x32, 0x23, 0x33, 0x44, 0x22, 0x33, 0x34, 0x43, 0x23, 0x22, 0x22, 0x22, 0x22, 0x22, 0x21, 0x11, 0x11, 0x12, 0x11, 0x33, 0x22, 0x22, 0x32, 0x22, 0x33, 0x22, 0x22, 0x22, 0x22, 0x22, 
        0x11, 0x22, 0x22, 0x33, 0x32, 0x22, 0x22, 0x22, 0x21, 0x22, 0x11, 0x20,
  0x33, 0x11, 0x11, 0x12, 0x86, 0x46, 0x62, 0x33, 0x54, 0x55, 0x33, 0x44, 0x44, 0x45, 0x55, 0x55, 0x44, 0x43, 0x33, 0x32, 0x22, 0x12, 0x22, 0x22, 0x22, 0x22, 0x22, 0x21, 0x11, 0x11, 0x13, 0x22, 0x22, 0x22, 0x22, 0x23, 0x34, 0x22, 0x22, 0x22, 0x32, 0x21, 
        0x11, 0x22, 0x12, 0x34, 0x42, 0x22, 0x22, 0x23, 0x21, 0x22, 0x11, 0x30,
  0x22, 0x11, 0x11, 0x13, 0x84, 0x34, 0x33, 0x33, 0x64, 0x55, 0x34, 0x57, 0x88, 0x99, 0x99, 0xA9, 0x87, 0x65, 0x43, 0x21, 0x22, 0x11, 0x12, 0x11, 0x22, 0x22, 0x22, 0x22, 0x11, 0x11, 0x21, 0x22, 0x22, 0x22, 0x22, 0x33, 0x33, 0x32, 0x23, 0x33, 0x22, 0x22, 
        0x22, 0x22, 0x22, 0x34, 0x33, 0x22, 0x53, 0x23, 0x21, 0x22, 0x11, 0x10,
  0x21, 0x11, 0x11, 0x16, 0x72, 0x22, 0x23, 0x34, 0x75, 0x65, 0x57, 0xAB, 0xBB, 0xCD, 0xDD, 0xDC, 0xBB, 0xA9, 0x75, 0x42, 0x11, 0x22, 0x22, 0x22, 0x32, 0x22, 0x22, 0x22, 0x11, 0x11, 0x12, 0x22, 0x22, 0x22, 0x23, 0x44, 0x43, 0x43, 0x43, 0x33, 0x22, 0x22, 
        0x21, 0x21, 0x22, 0x34, 0x22, 0x22, 0x12, 0x22, 0x21, 0x12, 0x11, 0x10,
  0x12, 0x11, 0x11, 0x29, 0x32, 0x22, 0x23, 0x35, 0x66, 0x66, 0x8B, 0xCC, 0xDD, 0xDD, 0xEE, 0xEE, 0xDC, 0xCB, 0xA9, 0x74, 0x21, 0x11, 0x22, 0x22, 0x22, 0x32, 0x22, 0x21, 0x11, 0x11, 0x22, 0x22, 0x22, 0x34, 0x55, 0x54, 0x34, 0x54, 0x33, 0x33, 0x43, 0x32, 
        0x22, 0x21, 0x12, 0x43, 0x21, 0x32, 0x11, 0x21, 0x21, 0x12, 0x22, 0x10,
  0x13, 0x31, 0x11, 0x46, 0x12, 0x12, 0x23, 0x35, 0x66, 0x89, 0xBC, 0xDE, 0xEE, 0xED, 0xDD, 0xEE, 0xDD, 0xCC, 0xCC, 0xB7, 0x53, 0x21, 0x22, 0x22, 0x33, 0x32, 0x22, 0x11, 0x11, 0x12, 0x22, 0x22, 0x45, 0x45, 0x55, 0x55, 0x54, 0x45, 0x43, 0x22, 0x33, 0x35, 
        0x32, 0x21, 0x23, 0x42, 0x11, 0x22, 0x11, 0x21, 0x21, 0x23, 0x21, 0x20,
  0x11, 0x21, 0x11, 0x52, 0x12, 0x11, 0x12, 0x33, 0x68, 0x9B, 0xCD, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xDB, 0x87, 0x33, 0x23, 0x33, 0x33, 0x42, 0x11, 0x11, 0x11, 0x12, 0x22, 0x25, 0x65, 0x56, 0x56, 0x66, 0x44, 0x43, 0x43, 0x23, 0x34, 0x34, 
        0x43, 0x22, 0x13, 0x21, 0x11, 0x22, 0x11, 0x21, 0x21, 0x13, 0x22, 0x30,
  0x23, 0x43, 0x21, 0x33, 0x21, 0x11, 0x22, 0x33, 0x79, 0xAC, 0xDE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xED, 0xA9, 0x62, 0x23, 0x33, 0x44, 0x43, 0x22, 0x11, 0x11, 0x12, 0x23, 0x56, 0x55, 0x56, 0x66, 0x65, 0x43, 0x33, 0x43, 0x33, 0x44, 0x44, 
        0x34, 0x43, 0x22, 0x21, 0x11, 0x22, 0x12, 0x22, 0x23, 0x32, 0x22, 0x30,
  0x24, 0x43, 0x34, 0x44, 0x43, 0x33, 0x35, 0x66, 0x9A, 0xBC, 0xDD, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xDD, 0xC9, 0x83, 0x33, 0x33, 0x44, 0x43, 0x21, 0x11, 0x11, 0x22, 0x36, 0x76, 0x65, 0x56, 0x55, 0x65, 0x33, 0x33, 0x44, 0x33, 0x35, 0x54, 
        0x54, 0x55, 0x32, 0x21, 0x11, 0x22, 0x11, 0x22, 0x24, 0x32, 0x22, 0x30,
  0x11, 0x23, 0x45, 0x54, 0x43, 0x45, 0x55, 0x56, 0x9B, 0xBB, 0xCD, 0xDE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xED, 0xDC, 0xCA, 0x86, 0x33, 0x44, 0x44, 0x53, 0x11, 0x11, 0x12, 0x22, 0x47, 0x76, 0x55, 0x56, 0x56, 0x65, 0x33, 0x44, 0x55, 0x54, 0x33, 0x55, 
        0x66, 0x55, 0x55, 0x21, 0x11, 0x22, 0x21, 0x12, 0x22, 0x12, 0x32, 0x40,
  0x22, 0x22, 0x24, 0x54, 0x34, 0x54, 0x44, 0x56, 0x9A, 0xAB, 0xBC, 0xDD, 0xDD, 0xEE, 0xEE, 0xDD, 0xDD, 0xDD, 0xDD, 0xCB, 0xBA, 0x87, 0x34, 0x44, 0x44, 0x43, 0x22, 0x11, 0x12, 0x24, 0x67, 0x65, 0x57, 0x89, 0x99, 0x86, 0x65, 0x66, 0x55, 0x54, 0x43, 0x45, 
        0x56, 0x75, 0x45, 0x41, 0x12, 0x23, 0x11, 0x22, 0x21, 0x22, 0x32, 0x30,
  0x22, 0x32, 0x24, 0x33, 0x34, 0x44, 0x54, 0x57, 0x9A, 0xAA, 0xBC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xBB, 0xBA, 0x88, 0x55, 0x44, 0x44, 0x44, 0x44, 0x43, 0x33, 0x35, 0x76, 0x55, 0x9B, 0xBC, 0xCB, 0xA9, 0x88, 0x88, 0x66, 0x54, 0x33, 0x35, 
        0x34, 0x65, 0x54, 0x53, 0x22, 0x12, 0x11, 0x12, 0x21, 0x22, 0x22, 0x30,
  0x22, 0x22, 0x34, 0x45, 0x66, 0x54, 0x65, 0x56, 0x9A, 0x9A, 0xAB, 0xCB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xAA, 0x99, 0x87, 0x54, 0x45, 0x55, 0x46, 0x55, 0x33, 0x23, 0x56, 0x65, 0x48, 0xCC, 0xDE, 0xED, 0xDB, 0xAB, 0xA9, 0x87, 0x76, 0x54, 0x44, 
        0x33, 0x44, 0x43, 0x44, 0x33, 0x12, 0x11, 0x12, 0x21, 0x21, 0x12, 0x20,
  0x33, 0x24, 0x56, 0x77, 0x87, 0x86, 0x67, 0x57, 0x99, 0x99, 0xAB, 0xBB, 0xBB, 0xBA, 0xAA, 0xA9, 0xAA, 0x9A, 0xAA, 0xA9, 0x98, 0x77, 0x44, 0x34, 0x55, 0x33, 0x33, 0x33, 0x33, 0x46, 0x54, 0x6C, 0xDE, 0xEE, 0xEE, 0xDC, 0xCC, 0xBA, 0xA9, 0x98, 0x76, 0x54, 
        0x43, 0x34, 0x32, 0x34, 0x53, 0x22, 0x12, 0x22, 0x11, 0x11, 0x22, 0x20,
  0x24, 0x34, 0x67, 0x88, 0x68, 0x96, 0x87, 0x57, 0x89, 0x99, 0xAB, 0xBB, 0xBA, 0xAA, 0x99, 0xA9, 0x99, 0x99, 0x99, 0x99, 0x98, 0x76, 0x32, 0x45, 0x43, 0x44, 0x43, 0x33, 0x34, 0x56, 0x44, 0xAE, 0xEE, 0xEE, 0xEE, 0xDC, 0xCC, 0xBB, 0xBA, 0xA9, 0x98, 0x75, 
        0x44, 0x32, 0x32, 0x13, 0x54, 0x22, 0x11, 0x11, 0x11, 0x22, 0x22, 0x20,
  0x34, 0x54, 0x79, 0x97, 0x69, 0xA5, 0x86, 0x67, 0x78, 0x89, 0xAA, 0xAA, 0xAA, 0xA9, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x88, 0x66, 0x54, 0x77, 0x32, 0x23, 0x44, 0x22, 0x34, 0x65, 0x46, 0xCE, 0xEE, 0xEE, 0xEE, 0xDC, 0xCC, 0xCB, 0xBB, 0xAA, 0x99, 0x87, 
        0x54, 0x32, 0x22, 0x12, 0x43, 0x22, 0x22, 0x22, 0x11, 0x22, 0x22, 0x20,
  0x44, 0x44, 0x89, 0x96, 0x68, 0x86, 0x85, 0x76, 0x78, 0x89, 0xAB, 0xBB, 0xAA, 0x99, 0x99, 0x99, 0x99, 0x99, 0x9A, 0xA9, 0x98, 0x65, 0x44, 0x57, 0x43, 0x22, 0x45, 0x33, 0x45, 0x65, 0x59, 0xDE, 0xEE, 0xEE, 0xEE, 0xED, 0xDD, 0xDC, 0xCB, 0xBA, 0xA9, 0x98, 
        0x74, 0x33, 0x21, 0x12, 0x34, 0x43, 0x33, 0x22, 0x21, 0x22, 0x22, 0x20,
  0x45, 0x54, 0x8A, 0xA5, 0x78, 0x75, 0x65, 0x75, 0x67, 0x8A, 0xBB, 0xBB, 0xB9, 0x98, 0x88, 0x99, 0x99, 0x98, 0x89, 0x99, 0xA9, 0x75, 0x23, 0x46, 0x43, 0x44, 0x34, 0x34, 0x55, 0x54, 0x5B, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xED, 0xDC, 0xBB, 0xAA, 0x99, 
        0x75, 0x33, 0x22, 0x11, 0x35, 0x32, 0x22, 0x12, 0x22, 0x12, 0x22, 0x20,
  0x33, 0x56, 0x8A, 0x95, 0x79, 0x88, 0x66, 0x75, 0x57, 0x8A, 0xCC, 0xCC, 0xBA, 0x98, 0x88, 0x89, 0x99, 0x88, 0x88, 0x99, 0xA9, 0x85, 0x23, 0x45, 0x33, 0x44, 0x43, 0x23, 0x55, 0x54, 0x8D, 0xDD, 0xDD, 0xDE, 0xEE, 0xEE, 0xEE, 0xED, 0xDC, 0xBB, 0xAA, 0x99, 
        0x85, 0x33, 0x22, 0x22, 0x34, 0x32, 0x32, 0x22, 0x22, 0x11, 0x22, 0x20,
  0x32, 0x47, 0x8A, 0x96, 0x89, 0x98, 0x57, 0x77, 0x57, 0x8B, 0xCC, 0xCB, 0xA9, 0x99, 0x88, 0x88, 0x98, 0x88, 0x89, 0x99, 0x99, 0x95, 0x24, 0x44, 0x24, 0x44, 0x44, 0x33, 0x35, 0x44, 0xBC, 0xAA, 0xA9, 0x8A, 0xCD, 0xDD, 0xED, 0xDD, 0xDC, 0xCB, 0xAA, 0x99, 
        0x85, 0x32, 0x23, 0x33, 0x34, 0x22, 0x22, 0x22, 0x11, 0x11, 0x22, 0x20,
  0x23, 0x47, 0x89, 0x98, 0x8A, 0xA7, 0x59, 0xCC, 0xC8, 0x8B, 0xCC, 0xCA, 0x98, 0x66, 0x78, 0x89, 0x88, 0x87, 0x66, 0x78, 0x99, 0x96, 0xAA, 0x54, 0x32, 0x45, 0x32, 0x24, 0x34, 0x38, 0xBC, 0xCD, 0xDA, 0x98, 0x9B, 0xCC, 0xCB, 0x9A, 0xAB, 0xCB, 0xBA, 0x99, 
        0x85, 0x33, 0x22, 0x32, 0x23, 0x22, 0x32, 0x22, 0x11, 0x12, 0x22, 0x20,
  0x34, 0x57, 0x99, 0x89, 0xAB, 0xB6, 0x69, 0x88, 0x99, 0x9B, 0xCC, 0xCA, 0x97, 0x43, 0x68, 0x9A, 0x99, 0x97, 0x34, 0x77, 0x8A, 0xA8, 0x97, 0x53, 0x12, 0x44, 0x32, 0x25, 0x36, 0x77, 0x6C, 0xCD, 0xDC, 0xB9, 0x9B, 0xBB, 0xA9, 0x87, 0x77, 0x99, 0xAA, 0x99, 
        0x85, 0x32, 0x22, 0x22, 0x33, 0x33, 0x32, 0x22, 0x11, 0x12, 0x22, 0x20,
  0x44, 0x58, 0x99, 0x9B, 0xBC, 0xB5, 0x78, 0x78, 0x89, 0x9B, 0xCC, 0xDC, 0xCC, 0xA9, 0x9A, 0xBB, 0xBB, 0xAA, 0x99, 0xAB, 0xBB, 0xA7, 0x66, 0x52, 0x12, 0x43, 0x23, 0x26, 0x56, 0x86, 0x8C, 0x98, 0x77, 0x89, 0xAC, 0xCB, 0xA9, 0xAA, 0xBA, 0x97, 0x89, 0x99, 
        0x85, 0x33, 0x22, 0x22, 0x22, 0x32, 0x22, 0x22, 0x21, 0x22, 0x22, 0x30,
  0x34, 0x58, 0xAA, 0xAB, 0xCC, 0xA5, 0x97, 0x88, 0x88, 0x8B, 0xCC, 0xDC, 0xCB, 0xAA, 0xBC, 0xCD, 0xDC, 0xCB, 0xAA, 0xAB, 0xBB, 0x97, 0x66, 0x63, 0x12, 0x35, 0x24, 0x26, 0x59, 0x86, 0xAC, 0xAA, 0x86, 0x68, 0xBD, 0xEB, 0xAA, 0xBC, 0xCC, 0xCA, 0x88, 0x99, 
        0x86, 0x32, 0x22, 0x22, 0x22, 0x32, 0x22, 0x11, 0x21, 0x23, 0x22, 0x20,
  0x34, 0x68, 0x89, 0xAA, 0xBA, 0x76, 0x97, 0x78, 0x77, 0x8B, 0xCC, 0xCC, 0xCB, 0xBC, 0xCC, 0xCD, 0xED, 0xCB, 0xBB, 0xBB, 0xBB, 0x96, 0x65, 0x53, 0x22, 0x34, 0x24, 0x26, 0x67, 0x87, 0xCE, 0xCA, 0x99, 0x9B, 0xBD, 0xDA, 0x99, 0x87, 0x78, 0x99, 0x98, 0x99, 
        0x87, 0x42, 0x22, 0x22, 0x33, 0x22, 0x22, 0x11, 0x13, 0x37, 0x42, 0x20,
  0x35, 0x68, 0x89, 0xAA, 0xA8, 0x67, 0x97, 0x79, 0x87, 0x9B, 0xBC, 0xCC, 0xCC, 0xCC, 0xCB, 0xCD, 0xED, 0xCA, 0xBB, 0xCC, 0xBA, 0x86, 0x76, 0x63, 0x23, 0x34, 0x24, 0x25, 0x77, 0xAB, 0xEE, 0xED, 0xCC, 0xCD, 0xBE, 0xC9, 0x99, 0x88, 0x75, 0x57, 0x99, 0x99, 
        0x98, 0x52, 0x22, 0x22, 0x23, 0x23, 0x22, 0x22, 0x22, 0x47, 0x32, 0x20,
  0x35, 0x68, 0x89, 0xAA, 0x97, 0x68, 0x96, 0x78, 0xA9, 0x9A, 0xBB, 0xCC, 0xCC, 0xCB, 0xBB, 0xCA, 0xAA, 0x9A, 0xAB, 0xCB, 0xBA, 0x87, 0x65, 0x66, 0x43, 0x36, 0x24, 0x35, 0x68, 0xBE, 0xEE, 0xEE, 0xEE, 0xED, 0xEE, 0xCA, 0xAB, 0xB9, 0x99, 0x87, 0x99, 0x77, 
        0x88, 0x52, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x42, 0x22, 0x20,
  0x25, 0x77, 0x8A, 0xBA, 0xA7, 0x67, 0x86, 0x76, 0xBC, 0xCB, 0xBB, 0xBC, 0xCB, 0xBB, 0xAA, 0x86, 0x87, 0x68, 0xAB, 0xBB, 0xA9, 0x99, 0x54, 0x44, 0x54, 0x44, 0x33, 0x55, 0x6A, 0xCE, 0xEE, 0xEE, 0xED, 0xCE, 0xEE, 0xCA, 0xBC, 0xDC, 0xBA, 0xAB, 0xBA, 0x88, 
        0x77, 0x42, 0x21, 0x12, 0x23, 0x22, 0x22, 0x22, 0x25, 0x53, 0x22, 0x20,
  0x26, 0x77, 0x9A, 0xCB, 0xA7, 0x77, 0x76, 0x75, 0x9B, 0xBB, 0xBB, 0xBB, 0xBA, 0xAA, 0xBB, 0xAA, 0xA9, 0x9A, 0xAA, 0xAA, 0x99, 0x8A, 0xA8, 0x65, 0x66, 0x65, 0x42, 0x55, 0x6B, 0xCD, 0xDD, 0xCC, 0xA9, 0xAE, 0xEE, 0xDA, 0xBD, 0xDD, 0xDD, 0xDD, 0xCB, 0xA9, 
        0x88, 0x52, 0x13, 0x42, 0x11, 0x22, 0x32, 0x22, 0x24, 0x23, 0x22, 0x20,
  0x27, 0x87, 0x9A, 0xCB, 0x99, 0x87, 0x76, 0x65, 0x67, 0x87, 0x9B, 0xBA, 0xAA, 0xAA, 0xBB, 0xBB, 0xBA, 0xAA, 0xAA, 0x99, 0x98, 0x77, 0x99, 0x77, 0x65, 0x55, 0x64, 0x65, 0x6C, 0xBB, 0xBB, 0xA9, 0x8A, 0xEE, 0xED, 0xC9, 0x8A, 0xCD, 0xEE, 0xEE, 0xDB, 0xA9, 
        0x87, 0x53, 0x46, 0x65, 0x12, 0x22, 0x22, 0x22, 0x12, 0x22, 0x21, 0x20,
  0x48, 0x88, 0x9A, 0xBA, 0x98, 0x86, 0x55, 0x66, 0x77, 0x66, 0x7A, 0xAA, 0xA9, 0x9A, 0xAA, 0x98, 0x77, 0x79, 0x99, 0x98, 0x88, 0x76, 0x44, 0x33, 0x65, 0x45, 0x45, 0x76, 0x7B, 0xAB, 0xAA, 0x98, 0x8B, 0xDD, 0xCB, 0xBC, 0xA7, 0x9B, 0xCC, 0xCC, 0xCB, 0xA9, 
        0x87, 0x54, 0x66, 0x65, 0x23, 0x32, 0x22, 0x11, 0x21, 0x23, 0x21, 0x20,
  0x58, 0x89, 0x97, 0x79, 0x96, 0x78, 0x9A, 0x99, 0x67, 0x76, 0x59, 0xAA, 0x99, 0x99, 0x98, 0x66, 0x65, 0x56, 0x99, 0x88, 0x86, 0x45, 0x67, 0x54, 0x34, 0x65, 0x44, 0x87, 0x8B, 0xA9, 0x98, 0x78, 0xAA, 0x79, 0x98, 0x8A, 0x97, 0x8A, 0xBB, 0xBB, 0xBA, 0x98, 
        0x87, 0x56, 0x67, 0x65, 0x22, 0x22, 0x22, 0x11, 0x21, 0x21, 0x21, 0x30,
  0x58, 0x88, 0x63, 0x35, 0x76, 0x77, 0x67, 0x78, 0x66, 0x67, 0x77, 0x9A, 0x99, 0x99, 0x87, 0x77, 0x77, 0x76, 0x88, 0x88, 0x75, 0x33, 0x24, 0x55, 0x33, 0x33, 0x35, 0x67, 0xBB, 0x99, 0x87, 0x8A, 0xBB, 0xA8, 0x76, 0x66, 0x67, 0x89, 0xAA, 0xAA, 0xA9, 0x88, 
        0x77, 0x66, 0x57, 0x74, 0x22, 0x21, 0x12, 0x22, 0x22, 0x21, 0x31, 0x30,
  0x68, 0x76, 0x64, 0x44, 0x57, 0x89, 0x64, 0x69, 0x86, 0x54, 0x44, 0x99, 0x99, 0x99, 0x88, 0x88, 0x77, 0x88, 0x88, 0x87, 0x55, 0x54, 0x43, 0x32, 0x32, 0x22, 0x23, 0x35, 0xBA, 0x99, 0x98, 0x8A, 0xBB, 0xB9, 0x87, 0x77, 0x78, 0x88, 0x99, 0x99, 0x98, 0x87, 
        0x77, 0x77, 0x67, 0x73, 0x12, 0x21, 0x12, 0x22, 0x12, 0x11, 0x21, 0x30,
  0x58, 0x86, 0x44, 0x55, 0x78, 0x74, 0x33, 0x57, 0x55, 0x78, 0x94, 0x79, 0x99, 0x98, 0x88, 0x88, 0x88, 0x88, 0x88, 0x76, 0x32, 0x22, 0x22, 0x22, 0x22, 0x23, 0x43, 0x23, 0x99, 0x99, 0xA9, 0x89, 0xAA, 0xAA, 0x98, 0x88, 0x88, 0x88, 0x89, 0x98, 0x88, 0x87, 
        0x77, 0x77, 0x88, 0x61, 0x12, 0x11, 0x12, 0x22, 0x12, 0x11, 0x21, 0x30,
  0x57, 0x99, 0x63, 0x23, 0x75, 0x33, 0x34, 0x56, 0x45, 0x32, 0x23, 0x99, 0x98, 0x88, 0x88, 0x99, 0x88, 0x88, 0x77, 0x66, 0x54, 0x32, 0x22, 0x22, 0x12, 0x22, 0x35, 0x35, 0x79, 0x99, 0xA9, 0x77, 0x66, 0x67, 0x77, 0x78, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 
        0x87, 0x78, 0x98, 0x31, 0x23, 0x21, 0x21, 0x11, 0x12, 0x12, 0x22, 0x20,
  0x57, 0x99, 0x97, 0x22, 0x54, 0x22, 0x34, 0x45, 0x44, 0x22, 0x4A, 0xCA, 0x88, 0x77, 0x88, 0x99, 0x88, 0x87, 0x66, 0x44, 0x33, 0x42, 0x22, 0x22, 0x23, 0x21, 0x22, 0x33, 0x68, 0x99, 0xA9, 0x87, 0x49, 0xA9, 0x87, 0x66, 0x67, 0x77, 0x88, 0x88, 0x88, 0x88, 
        0x77, 0x89, 0x86, 0x32, 0x24, 0x12, 0x22, 0x22, 0x22, 0x23, 0x22, 0x20,
  0x77, 0x89, 0x98, 0x32, 0x43, 0x22, 0x54, 0x45, 0x45, 0x57, 0xBD, 0xCB, 0x97, 0x66, 0x65, 0x66, 0x66, 0x55, 0x66, 0x44, 0x33, 0x32, 0x22, 0x22, 0x33, 0x42, 0x21, 0x23, 0x56, 0x99, 0x98, 0x89, 0x78, 0xAB, 0xBA, 0x98, 0x53, 0x57, 0x88, 0x88, 0x88, 0x87, 
        0x77, 0x8A, 0x93, 0x22, 0x22, 0x22, 0x21, 0x22, 0x22, 0x22, 0x22, 0x10,
  0x35, 0x99, 0x88, 0x67, 0x84, 0x22, 0x35, 0xAB, 0xDE, 0xEE, 0xED, 0xCC, 0xB9, 0x86, 0x55, 0x56, 0x66, 0x66, 0x67, 0x54, 0x22, 0x22, 0x22, 0x23, 0x22, 0x22, 0x11, 0x23, 0x56, 0x88, 0x88, 0x89, 0x99, 0x98, 0x88, 0x87, 0x44, 0x78, 0x98, 0x88, 0x88, 0x77, 
        0x77, 0x8A, 0x82, 0x22, 0x11, 0x11, 0x11, 0x11, 0x11, 0x22, 0x32, 0x10,
  0x36, 0x77, 0x86, 0x69, 0x85, 0x24, 0xAA, 0xAA, 0xBB, 0xBE, 0xEE, 0xED, 0xAA, 0x98, 0x76, 0x55, 0x55, 0x66, 0x6C, 0xED, 0xB7, 0x33, 0x22, 0x33, 0x12, 0x11, 0x12, 0x24, 0x54, 0x78, 0x89, 0x9A, 0x98, 0x99, 0x88, 0x77, 0x77, 0x78, 0x88, 0x88, 0x87, 0x66, 
        0x66, 0x56, 0x42, 0x21, 0x11, 0x21, 0x11, 0x11, 0x12, 0x33, 0x21, 0x10,
  0x54, 0x67, 0x65, 0x66, 0x63, 0x49, 0xBE, 0xEE, 0xEE, 0xC9, 0xCE, 0xEE, 0xEC, 0x98, 0x77, 0x66, 0x66, 0x67, 0x79, 0xDE, 0xEE, 0xD8, 0x32, 0x22, 0x12, 0x11, 0x23, 0x36, 0x44, 0x58, 0x89, 0xAA, 0xA9, 0x87, 0x77, 0x77, 0x77, 0x88, 0x88, 0x88, 0x77, 0x66, 
        0x65, 0x33, 0x32, 0x21, 0x12, 0x21, 0x11, 0x11, 0x11, 0x32, 0x22, 0x20,
  0x43, 0x34, 0x57, 0x76, 0x66, 0x9B, 0xEE, 0xEE, 0xEE, 0xEC, 0x7A, 0xEE, 0xEE, 0xEC, 0x87, 0x77, 0x77, 0x77, 0x88, 0x9B, 0xEE, 0xDE, 0xC6, 0x22, 0x23, 0x35, 0x67, 0x55, 0x43, 0x37, 0x89, 0xBB, 0xBA, 0x87, 0x77, 0x77, 0x88, 0x88, 0x77, 0x77, 0x66, 0x66, 
        0x64, 0x22, 0x22, 0x11, 0x11, 0x21, 0x11, 0x11, 0x13, 0x22, 0x21, 0x10,
  0x22, 0x12, 0x23, 0x23, 0x69, 0xAE, 0xEE, 0xEE, 0xEE, 0xDD, 0xB7, 0x9D, 0xEE, 0xEE, 0xEC, 0x97, 0x66, 0x77, 0x88, 0x89, 0xDE, 0xEC, 0xDD, 0xB6, 0x32, 0x34, 0x66, 0x54, 0x43, 0x45, 0x78, 0xBC, 0xCB, 0xA9, 0x98, 0x88, 0x88, 0x87, 0x77, 0x76, 0x66, 0x66, 
        0x52, 0x22, 0x21, 0x11, 0x12, 0x21, 0x11, 0x11, 0x22, 0x11, 0x12, 0x20,
  0x11, 0x12, 0x22, 0x22, 0x89, 0xEE, 0xEE, 0xEE, 0xDD, 0xCC, 0xCA, 0x89, 0xDE, 0xEE, 0xEE, 0xED, 0xCA, 0x98, 0x77, 0x78, 0xCE, 0xED, 0xAB, 0xCD, 0xDA, 0x65, 0x33, 0x34, 0x78, 0xAB, 0x77, 0x9B, 0xCB, 0xAA, 0xAA, 0x99, 0x98, 0x77, 0x76, 0x66, 0x66, 0x66, 
        0x42, 0x23, 0x33, 0x33, 0x23, 0x21, 0x11, 0x12, 0x31, 0x11, 0x11, 0x10,
  0x42, 0x11, 0x22, 0x25, 0x8D, 0xEE, 0xEE, 0xED, 0xDC, 0xCC, 0xDD, 0xB9, 0x9D, 0xEE, 0xEE, 0xEE, 0xED, 0xDD, 0xDD, 0xDE, 0xEE, 0xED, 0x89, 0xAB, 0xCD, 0xEE, 0xA9, 0xBE, 0xEE, 0xEE, 0x87, 0x89, 0x99, 0x99, 0x99, 0xA9, 0x87, 0x66, 0x66, 0x66, 0x66, 0x55, 
        0x43, 0x33, 0x45, 0x56, 0x66, 0x65, 0x44, 0x43, 0x32, 0x22, 0x22, 0x10,
  0x47, 0x21, 0x22, 0x27, 0xAE, 0xEE, 0xEE, 0xDD, 0xCC, 0xCC, 0xDE, 0xEC, 0xAA, 0xDE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0x87, 0x89, 0xAB, 0xCD, 0xEE, 0xEE, 0xED, 0xDC, 0x87, 0x77, 0x77, 0x89, 0x99, 0x88, 0x66, 0x66, 0x66, 0x66, 0x66, 0x55, 
        0x43, 0x22, 0x33, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x20,
  0x46, 0x72, 0x22, 0x39, 0xEE, 0xEE, 0xED, 0xDC, 0xCC, 0xCC, 0xCE, 0xED, 0xC9, 0xBE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0x86, 0x67, 0x89, 0xAB, 0xBC, 0xCC, 0xCB, 0xBA, 0x87, 0x77, 0x76, 0x67, 0x76, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x55, 
        0x32, 0x23, 0x33, 0x33, 0x32, 0x22, 0x22, 0x22, 0x22, 0x11, 0x11, 0x10,
  0x33, 0x55, 0x11, 0x4C, 0xEE, 0xED, 0xDD, 0xCC, 0xCC, 0xCB, 0xBC, 0xCC, 0xBA, 0xAC, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0x95, 0x56, 0x68, 0x89, 0x89, 0x9A, 0x99, 0x99, 0x97, 0x77, 0x77, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x65, 
        0x33, 0x33, 0x34, 0x34, 0x33, 0x33, 0x32, 0x22, 0x22, 0x22, 0x21, 0x20,
  0x23, 0x23, 0x41, 0x5E, 0xEE, 0xDD, 0xDD, 0xCC, 0xCC, 0xBA, 0xAA, 0x9A, 0xA9, 0xAC, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xB4, 0x45, 0x66, 0x66, 0x56, 0x67, 0x9A, 0xCC, 0x98, 0x77, 0x77, 0x76, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x65, 
        0x23, 0x33, 0x44, 0x44, 0x44, 0x44, 0x33, 0x33, 0x22, 0x22, 0x22, 0x20,
  0x22, 0x25, 0x52, 0x6E, 0xEE, 0xDD, 0xDC, 0xCC, 0xCB, 0xA8, 0x87, 0x89, 0x99, 0xAC, 0xEE, 0xEE, 0xEE, 0xEE, 0xED, 0xDE, 0xEE, 0xEE, 0xD4, 0x45, 0x54, 0x33, 0x68, 0x9C, 0xEE, 0xEC, 0x98, 0x77, 0x77, 0x76, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x64, 
        0x23, 0x34, 0x44, 0x44, 0x33, 0x33, 0x33, 0x33, 0x32, 0x22, 0x22, 0x20,
  0x35, 0x56, 0x31, 0xAE, 0xED, 0xDD, 0xDC, 0xCC, 0xBA, 0x97, 0x67, 0x88, 0x89, 0x8D, 0xEE, 0xEE, 0xEE, 0xEE, 0xDD, 0xEE, 0xEE, 0xEE, 0xE4, 0x32, 0x22, 0x36, 0x77, 0xAE, 0xEE, 0xDB, 0x99, 0x87, 0x77, 0x76, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x65, 
        0x23, 0x44, 0x44, 0x44, 0x44, 0x44, 0x33, 0x33, 0x33, 0x22, 0x22, 0x20,
  0x33, 0x21, 0x23, 0xCE, 0xED, 0xDD, 0xDC, 0xCB, 0xA9, 0x85, 0x67, 0x78, 0x99, 0x9E, 0xEE, 0xED, 0xDE, 0xEE, 0xDE, 0xEE, 0xEE, 0xEE, 0xE5, 0x23, 0x45, 0x67, 0x77, 0xCE, 0xEE, 0xDA, 0x99, 0x88, 0x77, 0x76, 0x66, 0x66, 0x55, 0x66, 0x66, 0x67, 0x76, 0x66, 
        0x43, 0x44, 0x44, 0x44, 0x44, 0x43, 0x33, 0x33, 0x33, 0x32, 0x22, 0x20,
  0x11, 0x12, 0x47, 0xEE, 0xDD, 0xDD, 0xCC, 0xBB, 0xA9, 0x74, 0x78, 0x9A, 0x88, 0xCE, 0xDC, 0xCD, 0xEE, 0xDE, 0xEE, 0xEE, 0xEE, 0xEE, 0xE8, 0x44, 0x56, 0x77, 0x67, 0xEE, 0xEE, 0xCB, 0x99, 0x88, 0x87, 0x77, 0x76, 0x66, 0x66, 0x66, 0x67, 0x77, 0x66, 0x67, 
        0x75, 0x44, 0x44, 0x44, 0x44, 0x43, 0x33, 0x33, 0x33, 0x32, 0x22, 0x10,
  0x11, 0x22, 0x3A, 0xEE, 0xDD, 0xDD, 0xCC, 0xBA, 0x98, 0x56, 0x9A, 0x97, 0x9C, 0xCB, 0xBD, 0xEE, 0xED, 0xCE, 0xEE, 0xEE, 0xEE, 0xEE, 0xE9, 0x55, 0x66, 0x76, 0x67, 0xEE, 0xEE, 0xCB, 0x99, 0x88, 0x87, 0x77, 0x76, 0x66, 0x67, 0x77, 0x77, 0x76, 0x66, 0x78, 
        0x85, 0x54, 0x44, 0x44, 0x44, 0x33, 0x33, 0x33, 0x33, 0x32, 0x22, 0x10,
  0x11, 0x12, 0x3B, 0xEE, 0xDD, 0xDC, 0xCB, 0xAA, 0x97, 0x67, 0x78, 0x9B, 0xCD, 0xDE, 0xEE, 0xEE, 0xEC, 0xBE, 0xEE, 0xEE, 0xED, 0xEE, 0xE9, 0x45, 0x66, 0x66, 0x66, 0xDE, 0xEE, 0xCB, 0xA8, 0x88, 0x88, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x66, 0x77, 0x87, 
        0x54, 0x55, 0x54, 0x44, 0x33, 0x33, 0x33, 0x33, 0x33, 0x22, 0x22, 0x10,
  0x11, 0x11, 0x3C, 0xDD, 0xDD, 0xDC, 0xCB, 0xA9, 0x86, 0x9C, 0xDE, 0xEE, 0xDD, 0xDE, 0xEE, 0xDC, 0xAC, 0xBE, 0xEE, 0xEE, 0xDD, 0xEE, 0xEA, 0x56, 0x66, 0x66, 0x66, 0xAE, 0xEE, 0xDB, 0xA9, 0x98, 0x88, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x78, 0x54, 
        0x44, 0x44, 0x55, 0x43, 0x33, 0x33, 0x33, 0x33, 0x22, 0x22, 0x22, 0x10,
  0x11, 0x11, 0x6D, 0xDD, 0xDD, 0xCC, 0xBA, 0xA9, 0x88, 0xAB, 0xCD, 0xDD, 0xDE, 0xEE, 0xDB, 0xBA, 0x87, 0x9D, 0xDE, 0xEE, 0xEE, 0xEE, 0xEA, 0x56, 0x56, 0x66, 0x56, 0x6D, 0xEE, 0xED, 0xCB, 0xA9, 0x88, 0x87, 0x87, 0x87, 0x87, 0x77, 0x77, 0x87, 0x64, 0x44, 
        0x44, 0x44, 0x55, 0x54, 0x33, 0x33, 0x33, 0x32, 0x22, 0x22, 0x21, 0x10,
  0x11, 0x23, 0x8D, 0xDD, 0xDD, 0xCB, 0xBA, 0x99, 0xAD, 0xEE, 0xDD, 0xDE, 0xEE, 0xEC, 0xBA, 0xBB, 0xA8, 0xAD, 0xEE, 0xEE, 0xEE, 0xEE, 0xDC, 0x66, 0x56, 0x66, 0x56, 0x68, 0xEE, 0xEE, 0xDC, 0xBB, 0xA9, 0x99, 0x98, 0x88, 0x87, 0x77, 0x88, 0x64, 0x34, 0x44, 
        0x44, 0x44, 0x55, 0x55, 0x54, 0x33, 0x33, 0x32, 0x22, 0x22, 0x21, 0x10,
  0x12, 0x44, 0x8D, 0xDD, 0xDC, 0xCB, 0xA9, 0x97, 0x9E, 0xEE, 0xEE, 0xEE, 0xED, 0xB9, 0x99, 0x89, 0xBB, 0xBC, 0xA9, 0x9A, 0xCD, 0xCB, 0xAA, 0x65, 0x56, 0x66, 0x65, 0x56, 0x9E, 0xEE, 0xED, 0xCC, 0xBB, 0xAA, 0x99, 0x98, 0x88, 0x88, 0x64, 0x34, 0x44, 0x44, 
        0x44, 0x44, 0x55, 0x55, 0x56, 0x53, 0x33, 0x32, 0x22, 0x22, 0x11, 0x10,
  0x33, 0x33, 0x9D, 0xDD, 0xCC, 0xBA, 0xA9, 0x88, 0xBD, 0xBB, 0xB9, 0x88, 0x87, 0x67, 0x77, 0xAA, 0xAA, 0x87, 0x78, 0x9D, 0xED, 0xBA, 0xAB, 0x65, 0x66, 0x66, 0x66, 0x55, 0x57, 0xDE, 0xED, 0xDC, 0xCB, 0xBA, 0xAA, 0x99, 0x97, 0x53, 0x33, 0x44, 0x44, 0x34, 
        0x44, 0x54, 0x45, 0x55, 0x66, 0x55, 0x32, 0x22, 0x22, 0x22, 0x11, 0x10,
  0x23, 0x33, 0xAD, 0xDD, 0xCC, 0xBA, 0x99, 0x88, 0x77, 0x78, 0x9A, 0x97, 0x77, 0x78, 0x99, 0xAB, 0xCB, 0xAB, 0xBC, 0xDE, 0xED, 0xEE, 0xED, 0x55, 0x65, 0x66, 0x65, 0x55, 0x45, 0x69, 0xCD, 0xDD, 0xCC, 0xCB, 0xA8, 0x75, 0x33, 0x34, 0x44, 0x44, 0x43, 0x44, 
        0x45, 0x44, 0x55, 0x55, 0x66, 0x66, 0x64, 0x22, 0x22, 0x21, 0x11, 0x10,
  0x23, 0x33, 0xBD, 0xDD, 0xCB, 0xBA, 0x99, 0x79, 0x66, 0x79, 0xBC, 0xEE, 0xDD, 0xEE, 0xEE, 0xEE, 0xDC, 0xDE, 0xEE, 0xEE, 0xEE, 0xEE, 0xED, 0x56, 0x55, 0x65, 0x55, 0x55, 0x54, 0x44, 0x55, 0x67, 0x76, 0x54, 0x44, 0x43, 0x44, 0x44, 0x44, 0x44, 0x44, 0x45, 
        0x54, 0x45, 0x56, 0x55, 0x66, 0x67, 0x76, 0x53, 0x22, 0x21, 0x11, 0x10,
  0x23, 0x34, 0xCD, 0xDD, 0xCB, 0xAA, 0x98, 0x7A, 0x8C, 0xEE, 0xDC, 0xBB, 0xCD, 0xEE, 0xDD, 0xED, 0xDE, 0xEE, 0xEE, 0xEE, 0xDD, 0xDC, 0xB8, 0x55, 0x55, 0x56, 0x55, 0x55, 0x55, 0x54, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x45, 0x55, 
        0x44, 0x55, 0x56, 0x56, 0x66, 0x67, 0x76, 0x66, 0x41, 0x11, 0x11, 0x10,
  0x22, 0x36, 0xDD, 0xDD, 0xBB, 0xA9, 0x98, 0x8C, 0xDC, 0xBB, 0xAA, 0xAA, 0xBA, 0xA9, 0x88, 0x88, 0x99, 0xAB, 0xAA, 0xBC, 0xDE, 0xED, 0x96, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x54, 0x45, 0x55, 0x55, 
        0x55, 0x55, 0x66, 0x56, 0x66, 0x77, 0x66, 0x67, 0x75, 0x11, 0x11, 0x10
};

GUI_CONST_STORAGE GUI_BITMAP bmwoxx = {
  107, // xSize
  64, // ySize
  54, // BytesPerLine
  4, // BitsPerPixel
  _acwoxx,  // Pointer to picture data (indices)
  &_Palwoxx   // Pointer to palette
};

/*************************** End of file ****************************/
