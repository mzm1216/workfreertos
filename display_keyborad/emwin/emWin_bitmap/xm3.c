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
* Source file: xm3                                                   *
* Dimensions:  108 * 64                                              *
* NumColors:   16                                                    *
*                                                                    *
**********************************************************************
*/

#include <stdlib.h>

#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

extern GUI_CONST_STORAGE GUI_BITMAP bmxm3;

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
static GUI_CONST_STORAGE GUI_COLOR _Colorsxm3[] = {
  0x000000, 0x111111, 0x222222, 0x333333,
  0x444444, 0x555555, 0x666666, 0x777777,
  0x888888, 0x999999, 0xAAAAAA, 0xBBBBBB,
  0xCCCCCC, 0xDDDDDD, 0xEEEEEE, 0xFFFFFF
};

static GUI_CONST_STORAGE GUI_LOGPALETTE _Palxm3 = {
  16,  // Number of entries
  0,   // No transparency
  &_Colorsxm3[0]
};

static GUI_CONST_STORAGE unsigned char _acxm3[] = {
  0x44, 0x44, 0x45, 0x44, 0x44, 0x45, 0x33, 0x54, 0x32, 0x45, 0x42, 0x23, 0x44, 0x54, 0x32, 0x33, 0x34, 0x42, 0x22, 0x21, 0x21, 0x22, 0x22, 0x11, 0x12, 0x22, 0x22, 0x22, 0x12, 0x23, 0x32, 0x33, 0x23, 0x44, 0x44, 0x44, 0x43, 0x33, 0x33, 0x22, 0x22, 0x23, 
        0x44, 0x44, 0x78, 0x22, 0x22, 0x22, 0x23, 0x33, 0x22, 0x37, 0x64, 0x49,
  0x44, 0x44, 0x44, 0x45, 0x44, 0x55, 0x44, 0x44, 0x44, 0x43, 0x52, 0x33, 0x44, 0x33, 0x33, 0x44, 0x45, 0x33, 0x22, 0x21, 0x11, 0x22, 0x22, 0x21, 0x11, 0x22, 0x22, 0x21, 0x12, 0x22, 0x33, 0x23, 0x43, 0x34, 0x43, 0x45, 0x55, 0x54, 0x34, 0x32, 0x21, 0x12, 
        0x35, 0x44, 0x79, 0x62, 0x33, 0x22, 0x23, 0x33, 0x22, 0x87, 0x44, 0x45,
  0x33, 0x44, 0x44, 0x45, 0x54, 0x55, 0x54, 0x44, 0x34, 0x56, 0x33, 0x54, 0x64, 0x54, 0x54, 0x44, 0x34, 0x22, 0x22, 0x21, 0x11, 0x11, 0x21, 0x11, 0x11, 0x22, 0x23, 0x21, 0x11, 0x22, 0x34, 0x33, 0x33, 0x33, 0x45, 0x67, 0x77, 0x77, 0x66, 0x66, 0x43, 0x22, 
        0x23, 0x66, 0x89, 0x52, 0x32, 0x33, 0x33, 0x32, 0x25, 0x86, 0x33, 0x46,
  0x23, 0x45, 0x44, 0x47, 0x55, 0x55, 0x54, 0x44, 0x43, 0x45, 0x54, 0x44, 0x56, 0x43, 0x54, 0x54, 0x23, 0x22, 0x22, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x12, 0x23, 0x21, 0x11, 0x12, 0x23, 0x33, 0x33, 0x23, 0x46, 0x78, 0x88, 0x88, 0x88, 0x99, 0x97, 0x53, 
        0x23, 0x57, 0xA9, 0x43, 0x33, 0x43, 0x33, 0x33, 0x48, 0x75, 0x43, 0x66,
  0x44, 0x43, 0x33, 0x47, 0x55, 0x65, 0x45, 0x45, 0x34, 0x54, 0x44, 0x55, 0x53, 0x35, 0x85, 0x44, 0x33, 0x22, 0x22, 0x21, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x23, 0x32, 0x12, 0x12, 0x32, 0x22, 0x23, 0x35, 0x67, 0x89, 0x99, 0x89, 0x89, 0xAB, 0xBB, 0xA8, 
        0x54, 0x57, 0xAB, 0x53, 0x33, 0x43, 0x33, 0x23, 0xA5, 0x44, 0x35, 0x7C,
  0x55, 0x43, 0x46, 0x44, 0x45, 0x57, 0x55, 0x44, 0x44, 0x44, 0x44, 0x53, 0x56, 0x34, 0x44, 0x35, 0x75, 0x22, 0x22, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x12, 0x22, 0x43, 0x22, 0x12, 0x23, 0x32, 0x22, 0x45, 0x78, 0x99, 0x88, 0x87, 0x89, 0xAB, 0xBB, 0xBA, 
        0x97, 0x57, 0x8A, 0x74, 0x33, 0x33, 0x32, 0x35, 0x54, 0x44, 0x8A, 0xCE,
  0x43, 0x33, 0x47, 0x44, 0x45, 0x56, 0x76, 0x55, 0x45, 0x64, 0x44, 0x44, 0x45, 0x57, 0x43, 0x36, 0x74, 0x21, 0x12, 0x21, 0x11, 0x11, 0x11, 0x11, 0x11, 0x12, 0x22, 0x44, 0x32, 0x22, 0x23, 0x33, 0x35, 0x56, 0x88, 0x75, 0x54, 0x47, 0x9A, 0xAB, 0xBB, 0xBB, 
        0xBA, 0x88, 0x9A, 0x73, 0x33, 0x33, 0x22, 0x55, 0x64, 0x39, 0xCC, 0xDE,
  0x33, 0x33, 0x45, 0x47, 0x67, 0x54, 0x97, 0x66, 0x45, 0x54, 0x55, 0x54, 0x45, 0x66, 0x54, 0x43, 0x33, 0x31, 0x11, 0x11, 0x11, 0x11, 0x11, 0x21, 0x11, 0x12, 0x22, 0x25, 0x54, 0x23, 0x22, 0x34, 0x56, 0x78, 0x74, 0x23, 0x56, 0x89, 0xAA, 0xBB, 0xBB, 0xBB, 
        0xBA, 0x99, 0x99, 0x53, 0x33, 0x32, 0x23, 0x54, 0x44, 0x7A, 0xDB, 0xCE,
  0x34, 0x34, 0x33, 0x45, 0x65, 0x46, 0x87, 0x76, 0x55, 0x54, 0x85, 0x44, 0x43, 0x32, 0x22, 0x24, 0x44, 0x31, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x12, 0x23, 0x34, 0x56, 0x43, 0x33, 0x33, 0x56, 0x67, 0x32, 0x22, 0x78, 0x79, 0xAA, 0xBB, 0xBB, 0xBB, 
        0xBA, 0xA9, 0x86, 0x33, 0x32, 0x32, 0x33, 0x33, 0x57, 0x89, 0xBA, 0x99,
  0x33, 0x45, 0x43, 0x41, 0x23, 0x34, 0x55, 0x67, 0x45, 0x44, 0x55, 0x56, 0x66, 0x66, 0x65, 0x54, 0x34, 0x41, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x23, 0x44, 0x56, 0x64, 0x35, 0x56, 0x66, 0x65, 0x33, 0x25, 0x88, 0x89, 0xAA, 0xBB, 0xBB, 0xBB, 
        0xAA, 0xBA, 0x54, 0x33, 0x22, 0x23, 0x23, 0x56, 0x88, 0xA9, 0x99, 0xA8,
  0x42, 0x43, 0x34, 0x66, 0x55, 0x76, 0x44, 0x66, 0x55, 0x68, 0x99, 0x99, 0x99, 0x98, 0x88, 0x87, 0x75, 0x31, 0x10, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x24, 0x55, 0x66, 0x77, 0x56, 0x77, 0x76, 0x66, 0x85, 0x57, 0x87, 0x8A, 0xAB, 0xBB, 0xBB, 0xBB, 
        0xBA, 0xBB, 0x74, 0x33, 0x23, 0x32, 0x26, 0x89, 0xAA, 0xBB, 0xBC, 0x97,
  0x33, 0x43, 0x24, 0x33, 0x34, 0x45, 0x43, 0x45, 0x68, 0xAA, 0xAA, 0xAA, 0xAA, 0xA9, 0x99, 0x98, 0x87, 0x65, 0x21, 0x01, 0x11, 0x11, 0x11, 0x11, 0x21, 0x11, 0x23, 0x56, 0x67, 0x87, 0x77, 0x88, 0x76, 0x66, 0x87, 0x78, 0x77, 0x9A, 0xBB, 0xBB, 0xBB, 0xBB, 
        0xBA, 0xAB, 0xA5, 0x33, 0x33, 0x32, 0x56, 0xBB, 0xCB, 0x8A, 0xCD, 0xCB,
  0x34, 0x45, 0x45, 0x35, 0x34, 0x44, 0x44, 0x57, 0x9A, 0xAB, 0xBB, 0xBB, 0xBB, 0xBA, 0xAA, 0x99, 0x99, 0x87, 0x64, 0x10, 0x01, 0x11, 0x11, 0x11, 0x22, 0x21, 0x12, 0x56, 0x77, 0x88, 0x88, 0x88, 0x76, 0x65, 0x67, 0x76, 0x79, 0xAA, 0xBB, 0xBB, 0xBB, 0xBB, 
        0xAB, 0xAA, 0xA7, 0x33, 0x33, 0x34, 0x76, 0xDD, 0xEA, 0x89, 0xD9, 0x97,
  0x34, 0x44, 0x66, 0x33, 0x34, 0x45, 0x55, 0x89, 0xAA, 0xAB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xAA, 0xA9, 0x88, 0x76, 0x52, 0x01, 0x11, 0x11, 0x11, 0x23, 0x22, 0x22, 0x35, 0x78, 0x88, 0x88, 0x88, 0x77, 0x66, 0x66, 0x68, 0x99, 0xAA, 0xBB, 0xBB, 0xBB, 0xBA, 
        0xAA, 0xBA, 0xAB, 0x43, 0x33, 0x45, 0x66, 0xA9, 0x8C, 0x8A, 0xB8, 0x95,
  0x44, 0x43, 0x33, 0x33, 0x45, 0x55, 0x57, 0x9A, 0xAA, 0xAB, 0xBB, 0xBB, 0xCC, 0xCC, 0xCB, 0xBB, 0xBA, 0x99, 0x88, 0x76, 0x30, 0x11, 0x11, 0x11, 0x23, 0x43, 0x22, 0x24, 0x67, 0x88, 0x88, 0x88, 0x88, 0x77, 0x67, 0x89, 0x9A, 0xAB, 0xBB, 0xBB, 0xBA, 0xAA, 
        0xAA, 0xAA, 0xBB, 0x83, 0x33, 0x58, 0x55, 0x96, 0x6A, 0x97, 0x89, 0x76,
  0x43, 0x94, 0x23, 0x33, 0x45, 0x54, 0x78, 0x9A, 0xAA, 0xAB, 0xBB, 0xCC, 0xCC, 0xCC, 0xCC, 0xBB, 0xBB, 0xAA, 0x98, 0x77, 0x63, 0x11, 0x11, 0x11, 0x12, 0x33, 0x34, 0x45, 0x67, 0x88, 0x99, 0x99, 0x99, 0x89, 0x99, 0x99, 0xAA, 0xAA, 0xBB, 0xAA, 0xAA, 0xAA, 
        0x9A, 0xAA, 0xBB, 0xB4, 0x34, 0x48, 0x77, 0x85, 0x9C, 0x96, 0x67, 0x86,
  0x66, 0x63, 0x33, 0x34, 0x44, 0x56, 0x89, 0xAA, 0xAA, 0xAB, 0xBB, 0xCC, 0xCC, 0xCC, 0xCC, 0xBB, 0xBB, 0xBA, 0x99, 0x87, 0x76, 0x21, 0x10, 0x11, 0x11, 0x23, 0x44, 0x56, 0x67, 0x78, 0x87, 0x89, 0x99, 0x99, 0x99, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xA9, 0x99, 
        0x99, 0x9A, 0xBB, 0xB8, 0x56, 0x68, 0xA9, 0x76, 0x8A, 0x76, 0x68, 0x56,
  0x48, 0x33, 0x33, 0x53, 0x34, 0x57, 0x89, 0xAA, 0xAA, 0xAB, 0xBB, 0xCC, 0xCC, 0xCC, 0xCC, 0xBB, 0xBB, 0xBA, 0xA9, 0x88, 0x76, 0x51, 0x10, 0x01, 0x11, 0x34, 0x56, 0x67, 0x77, 0x77, 0x87, 0x9A, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x99, 0x99, 
        0x99, 0x99, 0xAA, 0xAB, 0x78, 0x8A, 0x98, 0xB8, 0x78, 0x68, 0x66, 0x68,
  0x23, 0x33, 0x34, 0x55, 0x44, 0x67, 0x89, 0xAA, 0xAA, 0xAB, 0xBB, 0xCC, 0xCC, 0xCC, 0xCB, 0xBB, 0xBB, 0xBB, 0xAA, 0x98, 0x77, 0x64, 0x10, 0x01, 0x13, 0x55, 0x66, 0x66, 0x66, 0x77, 0x88, 0x9A, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x99, 0x99, 0x99, 0x99, 0x99, 
        0x99, 0x99, 0xAA, 0xAA, 0x64, 0x55, 0x78, 0x88, 0x8A, 0xA6, 0x54, 0x58,
  0x22, 0x22, 0x46, 0x43, 0x34, 0x68, 0x89, 0xAA, 0xAA, 0xAB, 0xBB, 0xBC, 0xCC, 0xCB, 0xBB, 0xBB, 0xBB, 0xBB, 0xAA, 0x98, 0x87, 0x65, 0x20, 0x01, 0x24, 0x56, 0x65, 0x44, 0x56, 0x77, 0x78, 0x9A, 0xAA, 0xAB, 0xA9, 0x88, 0x88, 0x89, 0x99, 0x99, 0x99, 0x88, 
        0x89, 0x99, 0x9A, 0xAA, 0x74, 0x44, 0x56, 0x66, 0x78, 0x77, 0x54, 0x55,
  0x22, 0x11, 0x22, 0x42, 0x35, 0x78, 0x99, 0xAA, 0xAA, 0xAB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBA, 0xAA, 0x99, 0x87, 0x66, 0x40, 0x01, 0x24, 0x66, 0x33, 0x22, 0x45, 0x77, 0x77, 0x99, 0xAB, 0xBB, 0xA9, 0x98, 0x76, 0x67, 0x77, 0x88, 0x88, 0x88, 
        0x89, 0x99, 0x9A, 0xAA, 0x95, 0x36, 0x65, 0x66, 0x46, 0x76, 0x45, 0x65,
  0x23, 0x12, 0x21, 0x32, 0x35, 0x78, 0x99, 0xAA, 0xAA, 0xAB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xAA, 0xAA, 0x99, 0x87, 0x76, 0x51, 0x00, 0x25, 0x53, 0x62, 0x24, 0x66, 0x77, 0x77, 0x89, 0xAB, 0xBB, 0xBA, 0xAB, 0xA8, 0x67, 0x88, 0x77, 0x77, 0x78, 
        0x99, 0x99, 0x99, 0xAA, 0x96, 0x58, 0x56, 0x54, 0x54, 0x55, 0x54, 0x46,
  0x12, 0x42, 0x11, 0x11, 0x46, 0x78, 0x99, 0xAA, 0xAA, 0xAB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBA, 0xAA, 0xAA, 0xAA, 0xA9, 0x99, 0x88, 0x76, 0x53, 0x00, 0x24, 0x23, 0x66, 0x56, 0x66, 0x77, 0x77, 0x89, 0xAB, 0xBC, 0xBB, 0xBB, 0xA9, 0x78, 0x88, 0x88, 0x87, 0x68, 
        0x99, 0x99, 0x99, 0xAA, 0x98, 0x76, 0x64, 0x44, 0x46, 0x55, 0x74, 0x44,
  0x35, 0x63, 0x21, 0x11, 0x56, 0x78, 0x99, 0xAA, 0xAA, 0xAB, 0xBB, 0xBB, 0xBB, 0xBA, 0xAA, 0xAA, 0xAA, 0xAA, 0xA9, 0x99, 0x88, 0x77, 0x64, 0x00, 0x24, 0x13, 0x56, 0x76, 0x67, 0x88, 0x78, 0x89, 0xAB, 0xBC, 0xBB, 0xBA, 0x97, 0x88, 0x88, 0x88, 0x87, 0x77, 
        0x99, 0x99, 0x99, 0x99, 0xA8, 0x66, 0x86, 0x44, 0x44, 0x45, 0x65, 0x44,
  0x21, 0x54, 0x52, 0x32, 0x57, 0x78, 0x99, 0xAA, 0xAA, 0xAB, 0xBB, 0xBB, 0xBB, 0xBB, 0xAA, 0xAA, 0xAA, 0xA9, 0x99, 0x98, 0x88, 0x77, 0x64, 0x10, 0x23, 0x24, 0x66, 0x66, 0x78, 0x88, 0x87, 0x79, 0x9A, 0xBB, 0xBA, 0x97, 0x67, 0x89, 0x99, 0x88, 0x77, 0x78, 
        0x99, 0x99, 0x99, 0x99, 0x9A, 0x85, 0x66, 0x45, 0x55, 0x54, 0x44, 0x55,
  0x27, 0x34, 0x64, 0x44, 0x56, 0x78, 0x99, 0xAA, 0xAA, 0xAA, 0xBB, 0xBB, 0xBB, 0xAA, 0xAA, 0xAA, 0xAA, 0xA9, 0x99, 0x98, 0x88, 0x77, 0x64, 0x10, 0x12, 0x35, 0x56, 0x67, 0x88, 0x88, 0x86, 0x78, 0x9A, 0xAA, 0xA9, 0x67, 0x78, 0x99, 0x99, 0x87, 0x45, 0x88, 
        0x89, 0x99, 0x99, 0x99, 0x9A, 0xAA, 0x64, 0x75, 0x56, 0x54, 0x43, 0x34,
  0x46, 0x44, 0x43, 0x54, 0x57, 0x78, 0x99, 0xAA, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBA, 0xAA, 0xAA, 0xAA, 0xA9, 0x99, 0x88, 0x88, 0x76, 0x64, 0x10, 0x13, 0x45, 0x67, 0x88, 0x89, 0x99, 0x76, 0x78, 0x9A, 0x99, 0x98, 0x77, 0x89, 0x99, 0x98, 0x76, 0x46, 0x88, 
        0x89, 0x99, 0x99, 0x99, 0x99, 0xAA, 0xBA, 0x86, 0x55, 0x55, 0x64, 0x55,
  0x23, 0x32, 0x44, 0x44, 0x57, 0x89, 0x9A, 0xAA, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xAA, 0xAA, 0xAA, 0xAA, 0x99, 0x99, 0x88, 0x88, 0x77, 0x64, 0x10, 0x12, 0x55, 0x67, 0x88, 0x89, 0x99, 0x77, 0x77, 0x89, 0x98, 0x87, 0x78, 0x88, 0x98, 0x87, 0x88, 0x67, 0x88, 
        0x88, 0x89, 0x99, 0x99, 0x9A, 0x9A, 0xAB, 0xCD, 0xDD, 0xDD, 0xDE, 0xEE,
  0x22, 0x34, 0x43, 0x33, 0x67, 0x89, 0xAA, 0xAB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xAA, 0xAA, 0xAA, 0xAA, 0x99, 0x99, 0x88, 0x88, 0x77, 0x64, 0x10, 0x02, 0x45, 0x67, 0x88, 0x99, 0x99, 0x77, 0x87, 0x78, 0x88, 0x77, 0x77, 0x88, 0x87, 0x7A, 0x96, 0x77, 0x88, 
        0x98, 0x98, 0x99, 0x99, 0x9A, 0xA9, 0xAB, 0xBC, 0xDE, 0xEE, 0xEE, 0xEE,
  0x43, 0x34, 0x43, 0x44, 0x67, 0x9A, 0xAA, 0xAB, 0xBB, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xA9, 0x99, 0x98, 0x88, 0x77, 0x64, 0x10, 0x01, 0x45, 0x67, 0x78, 0x99, 0x98, 0x77, 0x76, 0x66, 0x77, 0x77, 0x78, 0x77, 0x79, 0xBB, 0x97, 0x78, 0x99, 
        0x99, 0x99, 0x99, 0x89, 0x9A, 0xAA, 0xAA, 0xBC, 0xDE, 0xEE, 0xEE, 0xED,
  0x45, 0x43, 0x43, 0x34, 0x58, 0x9A, 0xAA, 0xBB, 0xBB, 0xBA, 0xAA, 0x99, 0xAA, 0xAA, 0xAA, 0xAA, 0xA9, 0x99, 0x9A, 0x99, 0x98, 0x77, 0x63, 0x11, 0x00, 0x34, 0x56, 0x77, 0x89, 0x98, 0x77, 0x77, 0x67, 0x77, 0x77, 0x77, 0x78, 0xAA, 0xC9, 0x78, 0x78, 0x99, 
        0x99, 0x99, 0x89, 0x88, 0x99, 0xAA, 0xAB, 0xBB, 0xCD, 0xDE, 0xEE, 0xDD,
  0x23, 0x32, 0x32, 0x34, 0x68, 0xAA, 0xAA, 0xBB, 0xBB, 0xBB, 0xBA, 0x99, 0xAA, 0xAA, 0xAA, 0xA9, 0x99, 0x99, 0x99, 0x99, 0x98, 0x76, 0x53, 0x11, 0x11, 0x14, 0x55, 0x67, 0x88, 0x88, 0x77, 0x77, 0x67, 0x77, 0x77, 0x67, 0x7B, 0xCC, 0xA8, 0x88, 0x78, 0x99, 
        0x99, 0x99, 0x98, 0x88, 0x99, 0xAB, 0xBB, 0xBB, 0xCD, 0xDD, 0xDD, 0xDD,
  0x32, 0x34, 0x66, 0x44, 0x68, 0xAA, 0xAB, 0xBB, 0xBB, 0xBB, 0xBA, 0xA9, 0x9A, 0xAA, 0xAA, 0x99, 0x99, 0x88, 0x89, 0x99, 0x88, 0x76, 0x53, 0x21, 0x11, 0x12, 0x45, 0x67, 0x88, 0x88, 0x77, 0x77, 0x66, 0x76, 0x66, 0x7A, 0xAB, 0xCB, 0x87, 0x98, 0x79, 0x99, 
        0x99, 0x99, 0x98, 0x88, 0x89, 0x9A, 0xBB, 0xBC, 0xDD, 0xDD, 0xDD, 0xDD,
  0x23, 0x28, 0x9A, 0x96, 0x69, 0xAB, 0xBB, 0xBB, 0xBB, 0xAA, 0xA9, 0x99, 0x9A, 0xAA, 0xAA, 0x99, 0x88, 0x89, 0x99, 0x99, 0x88, 0x76, 0x53, 0x22, 0x52, 0x52, 0x25, 0x66, 0x78, 0x88, 0x77, 0x77, 0x66, 0x66, 0x78, 0x9B, 0xBB, 0xA8, 0x78, 0x87, 0x89, 0x99, 
        0x99, 0x99, 0x88, 0x88, 0x89, 0x9A, 0xBB, 0xBC, 0xCD, 0xDD, 0xDD, 0xDC,
  0x33, 0x57, 0x68, 0xA9, 0x79, 0xAB, 0xBB, 0xA9, 0x86, 0x65, 0x66, 0x88, 0x9A, 0xAA, 0xA9, 0x99, 0x88, 0x99, 0xAA, 0x99, 0x88, 0x76, 0x54, 0x22, 0x43, 0x12, 0x23, 0x56, 0x77, 0x88, 0x77, 0x77, 0x65, 0x24, 0x9A, 0xBA, 0xA8, 0x77, 0x88, 0x77, 0x99, 0x9A, 
        0x99, 0x99, 0x88, 0x88, 0x89, 0xAA, 0xAB, 0xBC, 0xDD, 0xDD, 0xDD, 0xCC,
  0x24, 0x76, 0x78, 0x9A, 0x89, 0xAB, 0xBC, 0xCC, 0xCC, 0xA6, 0x42, 0x28, 0x9A, 0xAA, 0xA9, 0x98, 0x88, 0x99, 0x99, 0x99, 0x88, 0x87, 0x63, 0x21, 0x51, 0x11, 0x22, 0x35, 0x67, 0x88, 0x88, 0x77, 0x75, 0x43, 0x46, 0x65, 0x67, 0x88, 0x87, 0x78, 0x89, 0xAA, 
        0xA9, 0x98, 0x88, 0x88, 0x89, 0x9A, 0xAA, 0xAB, 0xDD, 0xDD, 0xCC, 0xBB,
  0x12, 0x66, 0x79, 0x98, 0x99, 0xAB, 0xCC, 0xCC, 0xCB, 0xBA, 0xA8, 0x78, 0xAB, 0xBB, 0xA9, 0x98, 0x88, 0x75, 0x57, 0x88, 0x88, 0x87, 0x63, 0x22, 0x10, 0x23, 0x11, 0x44, 0x56, 0x77, 0x88, 0x88, 0x87, 0x65, 0x55, 0x56, 0x77, 0x77, 0x77, 0x88, 0x9A, 0xAA, 
        0xA9, 0x98, 0x88, 0x88, 0x89, 0x99, 0xAA, 0xAB, 0xBC, 0xCC, 0xBB, 0xBB,
  0x11, 0x58, 0x7A, 0xA9, 0x99, 0xBB, 0xBC, 0xCC, 0xBB, 0xAA, 0x99, 0xAB, 0xBB, 0xBB, 0xBA, 0x99, 0x98, 0x43, 0x11, 0x34, 0x78, 0x87, 0x63, 0x44, 0x21, 0x21, 0x38, 0x33, 0x35, 0x67, 0x77, 0x88, 0x88, 0x77, 0x65, 0x56, 0x67, 0x77, 0x78, 0x89, 0x9A, 0xAA, 
        0xA9, 0x87, 0x88, 0x88, 0x88, 0x99, 0x9A, 0xAA, 0xBB, 0xBB, 0xBB, 0xBB,
  0x21, 0x39, 0x8A, 0x99, 0x89, 0xBB, 0xBC, 0xCC, 0xCB, 0xBB, 0xBB, 0xBB, 0xBC, 0xCC, 0xBB, 0xA9, 0xAA, 0xA9, 0x99, 0xAA, 0x87, 0x77, 0x63, 0x55, 0x65, 0x44, 0x21, 0x24, 0x32, 0x36, 0x77, 0x78, 0x88, 0x87, 0x77, 0x66, 0x77, 0x78, 0x88, 0x99, 0x99, 0x9A, 
        0x99, 0x78, 0x88, 0x88, 0x78, 0x99, 0x9A, 0xAA, 0xAA, 0xBB, 0xBB, 0xBB,
  0x11, 0x07, 0x99, 0x77, 0x8A, 0xBB, 0xBC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCB, 0xAA, 0xAA, 0xA9, 0x99, 0x9A, 0xAA, 0x87, 0x64, 0x65, 0x55, 0x68, 0x88, 0x76, 0x31, 0x22, 0x46, 0x77, 0x87, 0x78, 0x87, 0x77, 0x88, 0x88, 0x89, 0x99, 0x99, 0x99, 
        0x97, 0x78, 0x88, 0x88, 0x78, 0x89, 0x9A, 0xAA, 0xAB, 0xBB, 0xBB, 0xBB,
  0x11, 0x15, 0x99, 0x77, 0x8A, 0xAB, 0xBC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCD, 0xDD, 0xCC, 0xBA, 0xAB, 0xAA, 0xAA, 0xAA, 0xAA, 0x98, 0x66, 0x64, 0x46, 0x88, 0x89, 0x7A, 0xCA, 0x64, 0x22, 0x35, 0x67, 0x77, 0x77, 0x78, 0x88, 0x88, 0x99, 0x99, 0x99, 0x99, 
        0x87, 0x88, 0x88, 0x88, 0x77, 0x89, 0xAA, 0xAA, 0xBB, 0xBB, 0xBB, 0xBB,
  0x22, 0x13, 0x9A, 0x89, 0xA9, 0xAB, 0xBB, 0xCC, 0xCC, 0xCC, 0xCB, 0xCC, 0xCC, 0xDD, 0xDC, 0xBA, 0xBB, 0xBA, 0xBB, 0xBB, 0xBA, 0x98, 0x65, 0x54, 0x69, 0xA8, 0x99, 0x8A, 0xB9, 0x9B, 0xBA, 0x75, 0x34, 0x56, 0x77, 0x77, 0x88, 0x88, 0x89, 0x99, 0x99, 0x98, 
        0x78, 0x88, 0x88, 0x88, 0x87, 0x78, 0x9A, 0xAA, 0xBB, 0xBB, 0xBB, 0xBB,
  0x34, 0x43, 0x7A, 0x99, 0x99, 0xAB, 0xBB, 0xBB, 0xBB, 0xBA, 0xAA, 0xBC, 0xCC, 0xDD, 0xDC, 0xBA, 0xAB, 0xAA, 0xBB, 0xBB, 0xBA, 0x98, 0x66, 0x54, 0x8A, 0x99, 0x98, 0x9A, 0x97, 0x9A, 0xAB, 0xCD, 0xC9, 0x54, 0x55, 0x67, 0x78, 0x88, 0x88, 0x99, 0x98, 0x77, 
        0x78, 0x88, 0x99, 0x99, 0x98, 0x78, 0x99, 0xAA, 0xBB, 0xBB, 0xBB, 0xBB,
  0x21, 0x13, 0x4A, 0xAA, 0xA9, 0xAA, 0xBB, 0xBB, 0xAA, 0x99, 0x9A, 0xCC, 0xCC, 0xCC, 0xCB, 0xAA, 0xAA, 0xAB, 0xBB, 0xBB, 0xBA, 0x98, 0x66, 0x56, 0xAA, 0x98, 0x87, 0x99, 0x88, 0x99, 0x9A, 0xBC, 0xCB, 0x86, 0x97, 0x43, 0x56, 0x88, 0x88, 0x98, 0x77, 0x77, 
        0x78, 0x88, 0x99, 0x99, 0x99, 0x88, 0x99, 0xAA, 0xAB, 0xBB, 0xBB, 0xBB,
  0x01, 0x13, 0x38, 0xAB, 0xA9, 0xAA, 0xAB, 0xAA, 0xA9, 0x88, 0x89, 0xAB, 0xAB, 0xBB, 0xBA, 0xAA, 0xA9, 0x9A, 0xBB, 0xBB, 0xAA, 0x98, 0x66, 0x67, 0x98, 0x77, 0x68, 0xA9, 0x78, 0x78, 0x9B, 0xBB, 0xA8, 0x66, 0x77, 0x65, 0x43, 0x34, 0x45, 0x55, 0x66, 0x66, 
        0x78, 0x88, 0x99, 0x99, 0x99, 0x98, 0x99, 0xAA, 0xAB, 0xBB, 0xBB, 0xBC,
  0x00, 0x23, 0x37, 0xAB, 0x99, 0x9A, 0xAA, 0xAA, 0x98, 0x89, 0xAA, 0x98, 0x78, 0x9A, 0x99, 0x79, 0x97, 0x89, 0xAB, 0xBA, 0xA9, 0x97, 0x66, 0x55, 0x67, 0x77, 0x78, 0x97, 0x66, 0x78, 0x9B, 0xB9, 0x86, 0x66, 0x67, 0x54, 0x33, 0x33, 0x33, 0x44, 0x55, 0x66, 
        0x67, 0x78, 0x89, 0x99, 0x99, 0x99, 0x9A, 0xAA, 0xAB, 0xBB, 0xBB, 0xBB,
  0x01, 0x33, 0x34, 0xAA, 0x99, 0x9A, 0xAA, 0x99, 0x88, 0x9A, 0xBC, 0xBA, 0x98, 0x77, 0x76, 0x66, 0x77, 0x88, 0xAA, 0xAA, 0x99, 0x87, 0x65, 0x21, 0x12, 0x45, 0x67, 0x66, 0x66, 0x68, 0xAA, 0x99, 0x86, 0x67, 0x77, 0x63, 0x33, 0x33, 0x33, 0x33, 0x33, 0x45, 
        0x66, 0x78, 0x89, 0x99, 0x9A, 0x99, 0x9A, 0xAA, 0xAA, 0xAB, 0xBB, 0xBB,
  0x02, 0x33, 0x31, 0x25, 0x79, 0x99, 0x99, 0x99, 0x88, 0x9B, 0xBC, 0xBB, 0xBA, 0x99, 0x98, 0x88, 0x89, 0x99, 0x9A, 0xA9, 0x98, 0x87, 0x63, 0x11, 0x11, 0x11, 0x23, 0x45, 0x66, 0x79, 0x99, 0xA9, 0x77, 0x88, 0x88, 0x74, 0x33, 0x33, 0x32, 0x23, 0x34, 0x55, 
        0x66, 0x77, 0x88, 0x99, 0x9A, 0xAA, 0x9A, 0xAA, 0xAA, 0xAA, 0xBB, 0xBB,
  0x12, 0x33, 0x20, 0x11, 0x59, 0x99, 0x99, 0x98, 0x89, 0xAB, 0xCC, 0xBB, 0xA9, 0x99, 0x98, 0x99, 0x99, 0x99, 0x99, 0x99, 0x88, 0x87, 0x54, 0x21, 0x11, 0x11, 0x11, 0x11, 0x24, 0x67, 0x88, 0x87, 0x88, 0x87, 0x77, 0x76, 0x42, 0x22, 0x23, 0x33, 0x45, 0x56, 
        0x77, 0x88, 0x88, 0x99, 0x99, 0xAA, 0xAA, 0xAA, 0xAA, 0xAB, 0xBB, 0xBB,
  0x33, 0x35, 0x52, 0x11, 0x49, 0x99, 0x99, 0x99, 0x89, 0x9A, 0xA9, 0x88, 0x78, 0x87, 0x77, 0x88, 0x99, 0x98, 0x99, 0x98, 0x88, 0x76, 0x65, 0x55, 0x42, 0x11, 0x11, 0x11, 0x11, 0x12, 0x36, 0x89, 0x77, 0x77, 0x77, 0x76, 0x74, 0x33, 0x44, 0x44, 0x55, 0x67, 
        0x77, 0x88, 0x88, 0x89, 0x9A, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xBB, 0xBB,
  0x33, 0x46, 0x66, 0x54, 0x38, 0x99, 0x99, 0x99, 0x99, 0x86, 0x57, 0x79, 0x88, 0x76, 0x66, 0x67, 0x89, 0x88, 0x99, 0x98, 0x88, 0x76, 0x55, 0x55, 0x55, 0x54, 0x31, 0x11, 0x11, 0x12, 0x79, 0xAA, 0x87, 0x77, 0x77, 0x77, 0x77, 0x54, 0x55, 0x56, 0x66, 0x77, 
        0x88, 0x88, 0x89, 0x99, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xBB, 0xBB,
  0x34, 0x55, 0x55, 0x56, 0x57, 0x99, 0x9A, 0xA9, 0x99, 0x87, 0x44, 0x89, 0xAB, 0xA9, 0x87, 0x55, 0x57, 0x78, 0x99, 0x98, 0x87, 0x76, 0x55, 0x55, 0x55, 0x55, 0x55, 0x43, 0x23, 0x35, 0x77, 0x88, 0x77, 0x78, 0x87, 0x78, 0x88, 0x85, 0x56, 0x67, 0x77, 0x88, 
        0x88, 0x88, 0x89, 0x9A, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xBB, 0xBB,
  0x34, 0x55, 0x55, 0x55, 0x56, 0x99, 0x9A, 0xAA, 0x99, 0x9A, 0x97, 0x76, 0x69, 0x77, 0x86, 0x42, 0x24, 0x78, 0xAA, 0x98, 0x87, 0x75, 0x55, 0x55, 0x55, 0x55, 0x55, 0x65, 0x68, 0x89, 0x87, 0x88, 0x87, 0x78, 0x98, 0x79, 0x9A, 0xA9, 0x66, 0x77, 0x88, 0x88, 
        0x88, 0x88, 0x99, 0x9A, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAB, 0xBB, 0xBB,
  0x35, 0x55, 0x55, 0x55, 0x55, 0x69, 0x9A, 0xAA, 0x99, 0xAB, 0xB9, 0x99, 0x99, 0x87, 0x76, 0x65, 0x57, 0x8A, 0xA9, 0x98, 0x77, 0x65, 0x55, 0x55, 0x55, 0x55, 0x55, 0x66, 0x87, 0x78, 0x98, 0x88, 0x88, 0x87, 0xAA, 0x89, 0xAA, 0xBB, 0xA6, 0x77, 0x88, 0x88, 
        0x88, 0x89, 0x99, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xBB, 0xBB,
  0x45, 0x65, 0x55, 0x55, 0x55, 0x57, 0x99, 0x99, 0x9A, 0xBB, 0xBB, 0xA9, 0x99, 0x99, 0x88, 0x77, 0x89, 0x9A, 0xA9, 0x88, 0x77, 0x65, 0x55, 0x55, 0x55, 0x55, 0x67, 0x98, 0x88, 0x88, 0x99, 0x99, 0xAA, 0x98, 0xAB, 0xA9, 0xBB, 0xBC, 0xCA, 0x77, 0x88, 0x88, 
        0x89, 0x99, 0x99, 0xAA, 0xAA, 0xAA, 0xBA, 0xAA, 0xAA, 0xAB, 0xBB, 0xBB,
  0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x79, 0x99, 0xAA, 0xBB, 0xBB, 0xBA, 0x99, 0x88, 0x87, 0x88, 0x99, 0x99, 0x98, 0x88, 0x76, 0x55, 0x55, 0x55, 0x55, 0x55, 0x68, 0x89, 0x99, 0x99, 0x9A, 0xAA, 0xAB, 0xBA, 0xAB, 0xBA, 0xCC, 0xCD, 0xDD, 0xA8, 0x88, 0x89, 
        0x99, 0x99, 0xAA, 0xAA, 0xAA, 0xAB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB,
  0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x98, 0x99, 0xAA, 0xBB, 0xBB, 0xBA, 0xA9, 0x99, 0x98, 0x99, 0x99, 0x99, 0x88, 0x87, 0x65, 0x55, 0x55, 0x55, 0x55, 0x56, 0x68, 0x98, 0x99, 0xAA, 0xBB, 0xBB, 0xBB, 0xBB, 0xAC, 0xCB, 0xCD, 0xDC, 0xED, 0xD9, 0x99, 0x99, 
        0x99, 0x9A, 0xAA, 0xAA, 0xAA, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB,
  0x55, 0x57, 0x89, 0x9A, 0x98, 0x8A, 0x88, 0x88, 0x9A, 0xAB, 0xBC, 0xCC, 0xBB, 0xBA, 0xAA, 0xA9, 0x99, 0x98, 0x87, 0x76, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x99, 0x99, 0x99, 0x9A, 0xAB, 0xCC, 0xBC, 0xBC, 0xBD, 0xDC, 0xCD, 0xDD, 0xDE, 0xED, 0x99, 0x99, 
        0x9A, 0x9A, 0xAA, 0xAA, 0xAA, 0xAB, 0xBB, 0xBB, 0xBB, 0xBB, 0xAA, 0xBB,
  0x69, 0xBC, 0xBB, 0xBB, 0xBB, 0xC8, 0x58, 0x98, 0x89, 0xAA, 0xBB, 0xCC, 0xCB, 0xBB, 0xAA, 0xA9, 0x88, 0x88, 0x76, 0x33, 0x45, 0x55, 0x55, 0x55, 0x55, 0x56, 0x99, 0x99, 0x9A, 0xAB, 0xBB, 0xBD, 0xDC, 0xCB, 0xDD, 0xDC, 0xCD, 0xDD, 0xDE, 0xEE, 0xCA, 0xAA, 
        0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xBB, 0xBB, 0xBB, 0xBB, 0xBA, 0xAA, 0xAB,
  0xCD, 0xCB, 0xBC, 0xCC, 0xBC, 0xA5, 0x68, 0x99, 0x98, 0x89, 0xAB, 0xBB, 0xBB, 0xBA, 0xA9, 0x98, 0x77, 0x76, 0x31, 0x11, 0x12, 0x34, 0x45, 0x55, 0x55, 0x57, 0x99, 0x9A, 0xAB, 0xBB, 0xCC, 0xCD, 0xEE, 0xDC, 0xCD, 0xDD, 0xCE, 0xEE, 0xED, 0xEE, 0xEB, 0xAA, 
        0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xBB, 0xBB, 0xBB, 0xBB, 0xBA, 0xAA, 0xAB,
  0xDC, 0xBB, 0xCC, 0xCB, 0xBC, 0x74, 0x88, 0x99, 0x98, 0x88, 0x9A, 0xAA, 0xAA, 0xA9, 0x98, 0x87, 0x66, 0x52, 0x10, 0x12, 0x32, 0x11, 0x23, 0x45, 0x55, 0x68, 0x9A, 0xAA, 0xBB, 0xCC, 0xCC, 0xDD, 0xEE, 0xED, 0xCD, 0xED, 0xCE, 0xEE, 0xEE, 0xEE, 0xEE, 0xBA, 
        0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xBB, 0xBB, 0xBB, 0xBB, 0xBA, 0xAA, 0xBE,
  0xCB, 0xCC, 0xCC, 0xBB, 0xDB, 0x47, 0x88, 0x88, 0x88, 0x88, 0x88, 0x99, 0x99, 0x98, 0x87, 0x66, 0x55, 0x45, 0x42, 0x13, 0x42, 0x12, 0x11, 0x12, 0x34, 0x78, 0xBA, 0xBB, 0xBC, 0xCC, 0xCD, 0xDD, 0xDE, 0xED, 0xCD, 0xEE, 0xCE, 0xEE, 0xEE, 0xEE, 0xEE, 0xDB, 
        0xBB, 0xBB, 0xAB, 0xBA, 0xAB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBA, 0xAA, 0xDE,
  0xDC, 0xCC, 0xCC, 0xBB, 0xE9, 0x58, 0x88, 0x98, 0x88, 0x88, 0x77, 0x77, 0x87, 0x77, 0x66, 0x66, 0x55, 0x44, 0x55, 0x53, 0x21, 0x11, 0x01, 0x11, 0x12, 0x78, 0xAB, 0xAB, 0xBB, 0xCC, 0xDD, 0xDD, 0xDD, 0xDD, 0xCC, 0xDE, 0xCD, 0xEE, 0xEE, 0xEE, 0xEE, 0xEC, 
        0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBA, 0xAD, 0xEE,
  0xDC, 0xCC, 0xCC, 0xBD, 0xE7, 0x78, 0x88, 0x99, 0x88, 0x88, 0x88, 0x77, 0x77, 0x77, 0x66, 0x66, 0x54, 0x44, 0x65, 0x55, 0x54, 0x31, 0x11, 0x11, 0x23, 0x78, 0xAB, 0xAB, 0xCC, 0xDD, 0xDE, 0xEE, 0xEE, 0xDD, 0xCB, 0xDE, 0xDD, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 
        0xCB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBA, 0xCE, 0xEE
};

GUI_CONST_STORAGE GUI_BITMAP bmxm3 = {
  108, // xSize
  64, // ySize
  54, // BytesPerLine
  4, // BitsPerPixel
  _acxm3,  // Pointer to picture data (indices)
  &_Palxm3   // Pointer to palette
};

/*************************** End of file ****************************/
