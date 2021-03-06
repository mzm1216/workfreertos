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
* Source file: xm2                                                   *
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

extern GUI_CONST_STORAGE GUI_BITMAP bmxm2;

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
static GUI_CONST_STORAGE GUI_COLOR _Colorsxm2[] = {
  0x000000, 0x111111, 0x222222, 0x333333,
  0x444444, 0x555555, 0x666666, 0x777777,
  0x888888, 0x999999, 0xAAAAAA, 0xBBBBBB,
  0xCCCCCC, 0xDDDDDD, 0xEEEEEE, 0xFFFFFF
};

static GUI_CONST_STORAGE GUI_LOGPALETTE _Palxm2 = {
  16,  // Number of entries
  0,   // No transparency
  &_Colorsxm2[0]
};

static GUI_CONST_STORAGE unsigned char _acxm2[] = {
  0x44, 0x45, 0x45, 0x44, 0x53, 0x43, 0x35, 0x66, 0x66, 0x65, 0x45, 0x45, 0x44, 0x55, 0x44, 0x64, 0x44, 0x69, 0x85, 0x87, 0x53, 0x34, 0x45, 0x65, 0x44, 0x65, 0x43, 0x22, 0x22, 0x11, 0x22, 0x22, 0x22, 0x12, 0x33, 0x32, 0x32, 0x32, 0x22, 0x22, 0x23, 0x32, 
        0x23, 0x33, 0x22, 0x23, 0x33, 0x53, 0x3A, 0x83, 0x44, 0x4C, 0xED, 0xC0,
  0x45, 0x44, 0x35, 0x45, 0x55, 0x44, 0x33, 0x46, 0x66, 0x54, 0x45, 0x54, 0x43, 0x45, 0x55, 0x55, 0x45, 0x67, 0x98, 0x78, 0x44, 0x44, 0x47, 0x64, 0x44, 0x54, 0x23, 0x32, 0x22, 0x11, 0x22, 0x22, 0x22, 0x22, 0x23, 0x33, 0x23, 0x23, 0x32, 0x12, 0x22, 0x23, 
        0x22, 0x23, 0x32, 0x22, 0x33, 0x44, 0x4B, 0x53, 0x34, 0x57, 0xDB, 0xC0,
  0x44, 0x33, 0x54, 0x44, 0x45, 0x43, 0x44, 0x44, 0x44, 0x55, 0x54, 0x55, 0x44, 0x44, 0x55, 0x35, 0x55, 0x66, 0x88, 0x65, 0x34, 0x43, 0x34, 0x54, 0x44, 0x42, 0x23, 0x31, 0x12, 0x11, 0x12, 0x22, 0x22, 0x22, 0x22, 0x23, 0x22, 0x32, 0x33, 0x21, 0x12, 0x22, 
        0x22, 0x22, 0x23, 0x22, 0x23, 0x34, 0x43, 0x43, 0x35, 0x48, 0x99, 0x50,
  0x33, 0x33, 0x43, 0x34, 0x45, 0x44, 0x43, 0x34, 0x66, 0x34, 0x56, 0x54, 0x45, 0x53, 0x44, 0x48, 0x66, 0x45, 0x87, 0x54, 0x44, 0x76, 0x44, 0x54, 0x55, 0x23, 0x33, 0x31, 0x11, 0x21, 0x12, 0x32, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x23, 0x22, 0x11, 0x22, 
        0x22, 0x23, 0x22, 0x22, 0x22, 0x34, 0x43, 0x44, 0x46, 0x55, 0x54, 0x40,
  0x33, 0x33, 0x33, 0x33, 0x55, 0x54, 0x33, 0x34, 0x47, 0x56, 0x53, 0x43, 0x44, 0x44, 0x34, 0x45, 0x75, 0x44, 0x65, 0x55, 0x44, 0x55, 0x55, 0x44, 0x54, 0x22, 0x32, 0x22, 0x11, 0x22, 0x12, 0x23, 0x12, 0x22, 0x22, 0x22, 0x22, 0x13, 0x22, 0x33, 0x32, 0x12, 
        0x22, 0x22, 0x22, 0x22, 0x22, 0x23, 0x32, 0x23, 0x54, 0x44, 0x45, 0x70,
  0x33, 0x43, 0x33, 0x23, 0x57, 0xB6, 0x43, 0x33, 0x44, 0x46, 0x54, 0x54, 0x44, 0x34, 0x33, 0x43, 0x44, 0x54, 0x55, 0x45, 0x34, 0x55, 0x67, 0x45, 0x52, 0x32, 0x32, 0x21, 0x11, 0x12, 0x21, 0x22, 0x21, 0x12, 0x22, 0x22, 0x22, 0x22, 0x32, 0x33, 0x33, 0x21, 
        0x22, 0x22, 0x22, 0x22, 0x22, 0x33, 0x42, 0x22, 0x33, 0x42, 0x3A, 0x70,
  0x33, 0x33, 0x33, 0x23, 0x44, 0x65, 0x43, 0x33, 0x33, 0x34, 0x43, 0x44, 0x44, 0x45, 0x33, 0x42, 0x32, 0x34, 0x43, 0x44, 0x33, 0x54, 0x65, 0x44, 0x32, 0x22, 0x22, 0x21, 0x21, 0x22, 0x22, 0x12, 0x21, 0x11, 0x22, 0x22, 0x32, 0x23, 0x22, 0x23, 0x33, 0x33, 
        0x22, 0x22, 0x32, 0x22, 0x12, 0x23, 0x43, 0x22, 0x45, 0x34, 0x74, 0x50,
  0x33, 0x34, 0x43, 0x34, 0x64, 0x44, 0x33, 0x33, 0x33, 0x33, 0x44, 0x43, 0x33, 0x44, 0x43, 0x33, 0x33, 0x32, 0x33, 0x34, 0x43, 0x44, 0x46, 0x54, 0x22, 0x22, 0x22, 0x21, 0x21, 0x22, 0x21, 0x12, 0x22, 0x12, 0x12, 0x22, 0x23, 0x22, 0x32, 0x22, 0x33, 0x44, 
        0x44, 0x43, 0x34, 0x43, 0x22, 0x23, 0x43, 0x22, 0x44, 0x44, 0x76, 0x80,
  0x53, 0x33, 0x44, 0x43, 0x55, 0x54, 0x43, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x34, 0x34, 0x44, 0x33, 0x33, 0x33, 0x33, 0x43, 0x34, 0x46, 0x53, 0x22, 0x22, 0x22, 0x21, 0x11, 0x12, 0x21, 0x11, 0x12, 0x22, 0x11, 0x11, 0x22, 0x32, 0x23, 0x32, 0x23, 0x34, 
        0x66, 0x66, 0x67, 0x78, 0x63, 0x24, 0x63, 0x32, 0x55, 0x49, 0x9A, 0xB0,
  0x64, 0x33, 0x34, 0x34, 0x55, 0x56, 0x43, 0x33, 0x32, 0x33, 0x33, 0x33, 0x33, 0x34, 0x43, 0x55, 0x33, 0x44, 0x44, 0x44, 0x33, 0x33, 0x44, 0x32, 0x22, 0x22, 0x22, 0x21, 0x12, 0x11, 0x22, 0x11, 0x12, 0x22, 0x21, 0x11, 0x22, 0x23, 0x32, 0x32, 0x22, 0x46, 
        0x78, 0x87, 0x88, 0xAB, 0xBA, 0x75, 0x66, 0x34, 0x64, 0x59, 0x97, 0x90,
  0x54, 0x33, 0x34, 0x34, 0x44, 0x56, 0x54, 0x33, 0x33, 0x33, 0x43, 0x34, 0x44, 0x33, 0x23, 0x44, 0x44, 0x32, 0x33, 0x33, 0x43, 0x33, 0x43, 0x32, 0x22, 0x22, 0x21, 0x11, 0x11, 0x21, 0x22, 0x11, 0x11, 0x22, 0x31, 0x11, 0x11, 0x22, 0x33, 0x22, 0x24, 0x56, 
        0x88, 0x88, 0x89, 0xAB, 0xBB, 0xAA, 0x79, 0x49, 0x53, 0x34, 0x35, 0x50,
  0x54, 0x33, 0x33, 0x33, 0x33, 0x44, 0x34, 0x43, 0x34, 0x33, 0x53, 0x33, 0x34, 0x32, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x34, 0x43, 0x47, 0x62, 0x22, 0x22, 0x22, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x22, 0x23, 0x11, 0x11, 0x12, 0x22, 0x22, 0x34, 0x57, 
        0x88, 0x76, 0x79, 0xAA, 0xBB, 0xBA, 0xA9, 0x77, 0x43, 0x46, 0x9A, 0x70,
  0x43, 0x33, 0x34, 0x33, 0x33, 0x44, 0x43, 0x33, 0x33, 0x45, 0x66, 0x66, 0x65, 0x54, 0x43, 0x33, 0x34, 0x33, 0x33, 0x33, 0x53, 0x43, 0x45, 0x62, 0x22, 0x22, 0x21, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x12, 0x23, 0x22, 0x11, 0x12, 0x22, 0x22, 0x35, 0x78, 
        0x65, 0x36, 0x99, 0xAB, 0xBA, 0xBB, 0xA9, 0x85, 0x43, 0x6B, 0xB9, 0x90,
  0x34, 0x33, 0x44, 0x33, 0x33, 0x33, 0x33, 0x45, 0x78, 0x9A, 0xBA, 0xAA, 0xAA, 0xA9, 0x86, 0x54, 0x45, 0x33, 0x34, 0x33, 0x34, 0x53, 0x37, 0x32, 0x22, 0x21, 0x21, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x22, 0x33, 0x22, 0x12, 0x22, 0x34, 0x66, 0x74, 
        0x25, 0x68, 0x9A, 0xAA, 0xBB, 0xBB, 0xAA, 0x63, 0x48, 0xAE, 0xEB, 0x90,
  0x33, 0x34, 0x34, 0x44, 0x33, 0x33, 0x56, 0x8A, 0xAA, 0xBB, 0xBB, 0xBB, 0xCC, 0xCC, 0xCB, 0xA8, 0x75, 0x43, 0x33, 0x35, 0x46, 0x43, 0x44, 0x22, 0x22, 0x21, 0x21, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x12, 0x22, 0x24, 0x33, 0x22, 0x22, 0x23, 0x56, 0x52, 
        0x24, 0x78, 0x89, 0xAB, 0xBB, 0xBB, 0xBA, 0xA5, 0x7C, 0xCE, 0xD8, 0x80,
  0x33, 0x33, 0x44, 0x64, 0x34, 0x45, 0x89, 0xAB, 0xAB, 0xBB, 0xCC, 0xCC, 0xDD, 0xDC, 0xCD, 0xDD, 0xCA, 0x63, 0x22, 0x33, 0x34, 0x53, 0x44, 0x22, 0x22, 0x21, 0x21, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x12, 0x22, 0x22, 0x44, 0x42, 0x23, 0x45, 0x56, 0x32, 
        0x25, 0x88, 0x8A, 0xAA, 0xBB, 0xBB, 0xBA, 0xA9, 0x9B, 0xAA, 0x97, 0xA0,
  0x33, 0x34, 0x54, 0x44, 0x33, 0x68, 0xAA, 0xAA, 0xBB, 0xBB, 0xCC, 0xCC, 0xDD, 0xEE, 0xEE, 0xEE, 0xEE, 0xB8, 0x53, 0x23, 0x34, 0x45, 0x43, 0x22, 0x11, 0x21, 0x21, 0x11, 0x11, 0x11, 0x12, 0x11, 0x11, 0x11, 0x33, 0x43, 0x34, 0x44, 0x44, 0x66, 0x65, 0x65, 
        0x57, 0x77, 0x9A, 0xAB, 0xBB, 0xBA, 0xAA, 0xAA, 0x99, 0x89, 0x9A, 0x90,
  0x45, 0x45, 0x43, 0x43, 0x46, 0x89, 0xAA, 0xAA, 0xBB, 0xBB, 0xCC, 0xCC, 0xDD, 0xEE, 0xEE, 0xEE, 0xEE, 0xCA, 0x95, 0x33, 0x33, 0x44, 0x42, 0x22, 0x11, 0x21, 0x11, 0x11, 0x10, 0x11, 0x12, 0x12, 0x11, 0x11, 0x23, 0x55, 0x55, 0x66, 0x66, 0x77, 0x65, 0x77, 
        0x67, 0x68, 0x9A, 0xAB, 0xBB, 0xAA, 0xAA, 0xAA, 0xAA, 0xC9, 0x66, 0x70,
  0x44, 0x44, 0x42, 0x33, 0x68, 0x99, 0xAA, 0xAA, 0xAB, 0xBB, 0xBB, 0xCC, 0xCD, 0xEE, 0xEE, 0xEE, 0xEE, 0xED, 0xB8, 0x64, 0x23, 0x33, 0x23, 0x21, 0x11, 0x21, 0x11, 0x11, 0x11, 0x11, 0x21, 0x21, 0x11, 0x11, 0x13, 0x56, 0x77, 0x77, 0x77, 0x77, 0x65, 0x56, 
        0x66, 0x79, 0x9A, 0xAB, 0xAA, 0xAA, 0xA9, 0x99, 0xAA, 0xCA, 0x95, 0x60,
  0x33, 0x23, 0x32, 0x35, 0x78, 0x99, 0xAA, 0xAA, 0xAB, 0xBB, 0xBB, 0xCC, 0xCC, 0xCD, 0xDC, 0xDE, 0xED, 0xDD, 0xB9, 0x85, 0x34, 0x44, 0x33, 0x21, 0x11, 0x21, 0x11, 0x11, 0x11, 0x11, 0x11, 0x21, 0x11, 0x11, 0x12, 0x45, 0x67, 0x88, 0x87, 0x77, 0x76, 0x66, 
        0x67, 0x99, 0x9A, 0xAA, 0xAA, 0x99, 0x99, 0x99, 0x9A, 0xA7, 0x65, 0x60,
  0x34, 0x33, 0x33, 0x46, 0x88, 0x9A, 0xAA, 0xAA, 0xAB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBC, 0xCC, 0xCC, 0xBC, 0xB8, 0x87, 0x66, 0x33, 0x32, 0x21, 0x10, 0x11, 0x11, 0x11, 0x11, 0x11, 0x12, 0x12, 0x22, 0x21, 0x11, 0x24, 0x67, 0x88, 0x88, 0x88, 0x77, 0x78, 
        0x89, 0x9A, 0xAA, 0xAA, 0x99, 0x99, 0x98, 0x89, 0x99, 0x99, 0x66, 0x60,
  0x22, 0x33, 0x33, 0x46, 0x88, 0x9A, 0xAA, 0xAA, 0xAA, 0xBB, 0xBB, 0xBB, 0xBB, 0xAA, 0xAB, 0xBB, 0xBB, 0xAB, 0xA8, 0x87, 0x62, 0x35, 0x33, 0x21, 0x10, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22, 0x46, 0x78, 0x88, 0x88, 0x98, 0x89, 
        0x99, 0x9A, 0xA9, 0x99, 0x99, 0x98, 0x88, 0x89, 0x99, 0xA7, 0x65, 0x60,
  0x34, 0x43, 0x23, 0x57, 0x88, 0x99, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x9A, 0x98, 0x77, 0x73, 0x43, 0x53, 0x31, 0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x21, 0x12, 0x23, 0x32, 0x33, 0x46, 0x78, 0x88, 0x79, 0x99, 0x99, 
        0x99, 0x99, 0x99, 0x89, 0x88, 0x88, 0x88, 0x88, 0x99, 0x98, 0x45, 0x50,
  0x22, 0x32, 0x23, 0x67, 0x88, 0x99, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x99, 0x99, 0x99, 0x88, 0x77, 0x74, 0x45, 0x64, 0x52, 0x00, 0x11, 0x11, 0x10, 0x11, 0x11, 0x11, 0x12, 0x11, 0x22, 0x34, 0x45, 0x67, 0x77, 0x87, 0x79, 0x99, 0xAA, 
        0xA9, 0x98, 0x77, 0x77, 0x77, 0x77, 0x67, 0x89, 0x99, 0x97, 0x48, 0x70,
  0x22, 0x33, 0x24, 0x67, 0x88, 0x99, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xA9, 0x99, 0x99, 0x99, 0x98, 0x88, 0x76, 0x65, 0x67, 0x94, 0x32, 0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x12, 0x21, 0x12, 0x34, 0x55, 0x66, 0x67, 0x78, 0x89, 0xAA, 0xAA, 
        0xA9, 0x87, 0x76, 0x67, 0x77, 0x77, 0x66, 0x89, 0x99, 0x98, 0x55, 0x70,
  0x22, 0x22, 0x34, 0x67, 0x88, 0x99, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xA9, 0x99, 0x99, 0x99, 0x98, 0x87, 0x76, 0x54, 0x87, 0x53, 0x33, 0x10, 0x01, 0x11, 0x11, 0x01, 0x11, 0x01, 0x11, 0x22, 0x23, 0x45, 0x55, 0x66, 0x66, 0x77, 0x89, 0x9A, 0xAA, 
        0xAA, 0x9A, 0xA8, 0x77, 0x77, 0x87, 0x66, 0x88, 0x89, 0x98, 0x54, 0x60,
  0x34, 0x42, 0x34, 0x67, 0x88, 0x99, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xA9, 0x99, 0x99, 0x99, 0x88, 0x87, 0x76, 0x54, 0x6A, 0x53, 0x35, 0x20, 0x00, 0x10, 0x11, 0x11, 0x11, 0x01, 0x11, 0x13, 0x45, 0x56, 0x55, 0x45, 0x67, 0x77, 0x78, 0x9A, 0xAB, 
        0xBB, 0xBB, 0xA9, 0x78, 0x88, 0x86, 0x67, 0x88, 0x89, 0x88, 0x45, 0x50,
  0x42, 0x33, 0x35, 0x67, 0x88, 0x99, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xA9, 0x99, 0x99, 0x99, 0x88, 0x87, 0x76, 0x53, 0x38, 0x76, 0x36, 0x31, 0x00, 0x11, 0x11, 0x11, 0x00, 0x10, 0x11, 0x14, 0x66, 0x65, 0x32, 0x24, 0x67, 0x77, 0x77, 0x89, 0xAB, 
        0xBB, 0xBA, 0x87, 0x88, 0x88, 0x73, 0x67, 0x78, 0x88, 0x87, 0x44, 0x40,
  0x22, 0x35, 0x45, 0x77, 0x88, 0x99, 0xAA, 0xAA, 0xAB, 0xBB, 0xAA, 0xAA, 0xAA, 0xA9, 0x99, 0x99, 0x99, 0x99, 0x98, 0x86, 0x54, 0x37, 0x94, 0x33, 0x31, 0x00, 0x10, 0x11, 0x10, 0x00, 0x11, 0x11, 0x25, 0x66, 0x53, 0x22, 0x46, 0x67, 0x77, 0x77, 0x79, 0xAA, 
        0xBA, 0xA8, 0x67, 0x89, 0x87, 0x53, 0x67, 0x78, 0x88, 0x88, 0x54, 0x40,
  0x21, 0x24, 0x45, 0x67, 0x89, 0x99, 0x9A, 0xAA, 0xAA, 0xAA, 0xAA, 0x99, 0x9A, 0xA9, 0x99, 0x99, 0x99, 0x99, 0x99, 0x87, 0x64, 0x38, 0xC6, 0x32, 0x32, 0x00, 0x10, 0x11, 0x00, 0x00, 0x01, 0x11, 0x35, 0x65, 0x36, 0x54, 0x66, 0x68, 0x87, 0x77, 0x78, 0x99, 
        0xA9, 0x96, 0x67, 0x88, 0x87, 0x85, 0x68, 0x78, 0x88, 0x89, 0x96, 0x50,
  0x22, 0x24, 0x45, 0x67, 0x89, 0x99, 0xAA, 0xAA, 0xAA, 0xA9, 0x99, 0x99, 0x99, 0x99, 0x99, 0x98, 0x88, 0x88, 0x89, 0x88, 0x64, 0x34, 0x56, 0x23, 0x22, 0x10, 0x01, 0x10, 0x00, 0x00, 0x00, 0x12, 0x45, 0x64, 0x24, 0x67, 0x66, 0x78, 0x88, 0x76, 0x78, 0x99, 
        0x98, 0x86, 0x78, 0x88, 0x79, 0x86, 0x78, 0x88, 0x88, 0x88, 0x9A, 0xA0,
  0x11, 0x23, 0x34, 0x67, 0x89, 0x9A, 0xAA, 0xAA, 0xAA, 0xA9, 0x99, 0x99, 0x99, 0x99, 0x99, 0x88, 0x88, 0x88, 0x88, 0x88, 0x64, 0x23, 0x42, 0x33, 0x23, 0x60, 0x01, 0x00, 0x01, 0x00, 0x00, 0x02, 0x45, 0x53, 0x35, 0x66, 0x67, 0x88, 0x88, 0x66, 0x77, 0x88, 
        0x88, 0x77, 0x78, 0x77, 0x8A, 0x77, 0x78, 0x88, 0x88, 0x88, 0x99, 0xA0,
  0x22, 0x22, 0x34, 0x67, 0x89, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xA9, 0x88, 0x99, 0x99, 0x99, 0x88, 0x88, 0x89, 0x98, 0x87, 0x74, 0x24, 0x34, 0x32, 0x35, 0x81, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x35, 0x54, 0x56, 0x66, 0x78, 0x89, 0x88, 0x57, 0x77, 0x78, 
        0x77, 0x76, 0x77, 0x77, 0xBA, 0x67, 0x78, 0x88, 0x88, 0x88, 0x99, 0x90,
  0x13, 0x23, 0x34, 0x67, 0x99, 0xAA, 0xAB, 0xBB, 0xAA, 0xAA, 0xA9, 0x88, 0x89, 0x99, 0x98, 0x88, 0x78, 0x99, 0x98, 0x87, 0x75, 0x23, 0x33, 0x32, 0x23, 0x42, 0x00, 0x10, 0x00, 0x00, 0x00, 0x01, 0x24, 0x55, 0x66, 0x78, 0x88, 0x89, 0x98, 0x67, 0x87, 0x66, 
        0x67, 0x77, 0x66, 0x8B, 0xB9, 0x78, 0x78, 0x99, 0x98, 0x88, 0x99, 0x90,
  0x23, 0x21, 0x34, 0x67, 0x9A, 0xAA, 0xAB, 0xBB, 0xAA, 0xA9, 0x98, 0x88, 0x89, 0x99, 0x98, 0x87, 0x78, 0x88, 0x88, 0x77, 0x76, 0x33, 0x33, 0x11, 0x42, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x55, 0x67, 0x78, 0x88, 0x99, 0x88, 0x66, 0x76, 0x66, 
        0x76, 0x76, 0x67, 0xAC, 0xB6, 0x88, 0x78, 0x99, 0x99, 0x88, 0x99, 0x90,
  0x33, 0x22, 0x34, 0x67, 0x9A, 0xAA, 0xAB, 0xAA, 0x85, 0x53, 0x32, 0x37, 0x89, 0xAA, 0x99, 0x87, 0x77, 0x42, 0x23, 0x47, 0x76, 0x32, 0x22, 0x32, 0x22, 0x52, 0x11, 0x11, 0x10, 0x00, 0x00, 0x00, 0x17, 0x55, 0x67, 0x78, 0x88, 0x99, 0x88, 0x77, 0x77, 0x77, 
        0x66, 0x66, 0x8A, 0xBB, 0x87, 0x77, 0x78, 0x99, 0x99, 0x88, 0x99, 0x90,
  0x51, 0x23, 0x44, 0x57, 0x9A, 0xAA, 0xBB, 0xAB, 0xCC, 0xA8, 0x65, 0x46, 0x9A, 0xAB, 0xA9, 0x98, 0x96, 0x54, 0x47, 0x88, 0x77, 0x43, 0x23, 0x72, 0x45, 0x32, 0x11, 0x11, 0x11, 0x10, 0x01, 0x00, 0x15, 0x65, 0x66, 0x77, 0x88, 0x88, 0x88, 0x77, 0x77, 0x76, 
        0x66, 0x68, 0x9B, 0xA7, 0x77, 0x77, 0x78, 0x99, 0x99, 0x88, 0x99, 0x90,
  0x23, 0x54, 0x34, 0x67, 0x9A, 0xAB, 0xBB, 0xCC, 0xBA, 0xA9, 0x98, 0x89, 0xAB, 0xBB, 0xBA, 0xA9, 0x9A, 0x98, 0x88, 0x99, 0x98, 0x55, 0x22, 0x32, 0x33, 0x33, 0x11, 0x11, 0x11, 0x11, 0x10, 0x10, 0x04, 0x44, 0x56, 0x67, 0x77, 0x88, 0x88, 0x77, 0x77, 0x66, 
        0x67, 0x9A, 0xA8, 0x77, 0x77, 0x77, 0x88, 0x99, 0x99, 0x88, 0x99, 0x90,
  0x14, 0x99, 0x96, 0x67, 0x9A, 0xAB, 0xBB, 0xBB, 0xBA, 0xAA, 0xAA, 0xAB, 0xBB, 0xCC, 0xCB, 0xBA, 0xAA, 0xAA, 0x99, 0xAA, 0xA9, 0x76, 0x55, 0x32, 0x13, 0x21, 0x21, 0x31, 0x21, 0x11, 0x10, 0x11, 0x10, 0x13, 0x76, 0x66, 0x77, 0x88, 0x88, 0x67, 0x77, 0x63, 
        0x25, 0x66, 0x55, 0x77, 0x76, 0x77, 0x89, 0x99, 0x98, 0x88, 0x99, 0x90,
  0x27, 0x67, 0x99, 0x88, 0x99, 0xAB, 0xBB, 0xBB, 0xCB, 0xBB, 0xBB, 0xBC, 0xCC, 0xCC, 0xCC, 0xBA, 0xAA, 0xAA, 0xAB, 0xBB, 0xA9, 0x75, 0x33, 0x32, 0x36, 0x32, 0x22, 0x13, 0x11, 0x11, 0x11, 0x11, 0x11, 0x12, 0x78, 0x66, 0x77, 0x88, 0x88, 0x76, 0x77, 0x65, 
        0x55, 0x55, 0x66, 0x66, 0x67, 0x88, 0x89, 0x99, 0x98, 0x88, 0x89, 0x90,
  0x66, 0x57, 0x88, 0x99, 0x99, 0xAB, 0xBB, 0xBC, 0xCC, 0xCB, 0xBB, 0xBC, 0xCC, 0xCC, 0xCC, 0xCA, 0xAA, 0xAA, 0xBB, 0xBB, 0xA9, 0x85, 0x34, 0x33, 0x33, 0x22, 0x22, 0x23, 0x11, 0x12, 0x22, 0x24, 0x11, 0x11, 0x23, 0x45, 0x67, 0x78, 0x88, 0x88, 0x78, 0x77, 
        0x66, 0x55, 0x66, 0x67, 0x78, 0x88, 0x89, 0x88, 0x87, 0x88, 0x89, 0x90,
  0x46, 0x67, 0x97, 0x89, 0x99, 0xAB, 0xBB, 0xBC, 0xCC, 0xBB, 0xBB, 0xBA, 0xBB, 0xCC, 0xCC, 0xBA, 0xA9, 0x99, 0xAB, 0xBA, 0xA9, 0x85, 0x45, 0x66, 0x42, 0x21, 0x11, 0x33, 0x12, 0x13, 0x45, 0x24, 0x31, 0x21, 0x23, 0x32, 0x46, 0x77, 0x88, 0x88, 0x88, 0x88, 
        0x77, 0x77, 0x77, 0x88, 0x88, 0x88, 0x88, 0x88, 0x77, 0x78, 0x99, 0x90,
  0x37, 0x67, 0x98, 0x99, 0x99, 0xAA, 0xBB, 0xBB, 0xBB, 0xBB, 0xA9, 0x9A, 0xCC, 0xCC, 0xBB, 0xAA, 0xAA, 0x88, 0x9A, 0xAA, 0xA9, 0x85, 0x53, 0x43, 0x32, 0x22, 0x12, 0x22, 0x35, 0x44, 0x84, 0x24, 0x42, 0x12, 0x38, 0x75, 0x34, 0x67, 0x78, 0x88, 0x88, 0x88, 
        0x88, 0x88, 0x78, 0x88, 0x88, 0x88, 0x88, 0x88, 0x77, 0x78, 0x88, 0x80,
  0x58, 0x78, 0x99, 0x86, 0x99, 0xAA, 0xAB, 0xBB, 0xBB, 0xA9, 0x98, 0x8A, 0xBB, 0x89, 0xA9, 0x97, 0x78, 0x77, 0x89, 0xA9, 0x99, 0x86, 0x63, 0x33, 0x11, 0x12, 0x12, 0x42, 0x13, 0x23, 0x42, 0x27, 0x85, 0x55, 0x55, 0x82, 0x22, 0x43, 0x57, 0x78, 0x88, 0x88, 
        0x77, 0x78, 0x78, 0x88, 0x88, 0x88, 0x88, 0x77, 0x78, 0x88, 0x88, 0x80,
  0x36, 0x88, 0x86, 0x66, 0x89, 0x9A, 0xAA, 0xAA, 0xAA, 0x98, 0x77, 0x88, 0x97, 0x67, 0x88, 0x76, 0x56, 0x67, 0x89, 0x99, 0x98, 0x76, 0x68, 0x72, 0x21, 0x21, 0x11, 0x32, 0x11, 0x02, 0x14, 0x56, 0x33, 0x85, 0x31, 0x13, 0x22, 0x22, 0x31, 0x23, 0x56, 0x56, 
        0x77, 0x77, 0x77, 0x78, 0x88, 0x88, 0x87, 0x77, 0x88, 0x88, 0x88, 0x80,
  0x22, 0x78, 0x86, 0x77, 0x99, 0x99, 0xAA, 0xAA, 0xA9, 0x88, 0x78, 0xAA, 0x98, 0x88, 0x77, 0x77, 0x77, 0x88, 0x88, 0x99, 0x88, 0x76, 0x69, 0xAA, 0x96, 0x46, 0x42, 0x30, 0x21, 0x12, 0x1B, 0x72, 0x22, 0x21, 0x11, 0x13, 0x33, 0x22, 0x12, 0x43, 0x32, 0x23, 
        0x44, 0x45, 0x66, 0x66, 0x67, 0x66, 0x67, 0x78, 0x88, 0x88, 0x88, 0x80,
  0x53, 0x58, 0x97, 0x89, 0x99, 0x99, 0xAA, 0xAA, 0x99, 0x87, 0x8A, 0xBB, 0xBB, 0xAA, 0x98, 0x88, 0x88, 0x98, 0x88, 0x99, 0x88, 0x76, 0x67, 0x78, 0x97, 0x89, 0x77, 0x88, 0x76, 0x33, 0x42, 0x24, 0x22, 0x22, 0x21, 0x22, 0x32, 0x22, 0x32, 0x23, 0x94, 0x24, 
        0x44, 0x33, 0x45, 0x56, 0x66, 0x66, 0x67, 0x78, 0x88, 0x88, 0x88, 0x90,
  0x38, 0x57, 0x99, 0x89, 0x99, 0x99, 0x99, 0xA9, 0x98, 0x87, 0x8A, 0xBB, 0xBB, 0xA8, 0x88, 0x77, 0x78, 0x88, 0x88, 0x88, 0x87, 0x76, 0x67, 0x99, 0x88, 0x98, 0x77, 0x89, 0x98, 0x55, 0x99, 0x85, 0x53, 0x21, 0x12, 0x22, 0x62, 0x24, 0x32, 0x22, 0x23, 0x45, 
        0x44, 0x33, 0x34, 0x45, 0x55, 0x66, 0x66, 0x77, 0x88, 0x88, 0x88, 0x90,
  0x53, 0x22, 0x89, 0x9A, 0xA9, 0x99, 0x99, 0x99, 0x88, 0x77, 0x9A, 0xBB, 0xA8, 0x76, 0x66, 0x55, 0x56, 0x67, 0x78, 0x98, 0x87, 0x66, 0x68, 0xA9, 0x89, 0x97, 0x78, 0x89, 0x85, 0x57, 0x98, 0x77, 0x86, 0x55, 0x66, 0x66, 0x42, 0x33, 0x22, 0x32, 0x37, 0x74, 
        0x44, 0x33, 0x33, 0x33, 0x33, 0x45, 0x66, 0x77, 0x78, 0x88, 0x98, 0x90,
  0x64, 0x23, 0x59, 0xAA, 0xA9, 0x99, 0x99, 0x99, 0x88, 0x78, 0x89, 0x98, 0x66, 0x77, 0x86, 0x76, 0x55, 0x56, 0x68, 0x99, 0x87, 0x66, 0x79, 0xA9, 0x9A, 0x87, 0x78, 0x78, 0x65, 0x69, 0x88, 0x67, 0x76, 0x55, 0x66, 0x77, 0x65, 0x55, 0x55, 0x57, 0xA9, 0x95, 
        0x33, 0x33, 0x32, 0x23, 0x44, 0x55, 0x66, 0x77, 0x78, 0x88, 0x88, 0x90,
  0x25, 0x54, 0x47, 0xAA, 0xA9, 0x99, 0x99, 0x99, 0x98, 0x88, 0x76, 0x46, 0x78, 0x9A, 0xA8, 0x98, 0x52, 0x25, 0x79, 0x98, 0x87, 0x65, 0x78, 0x99, 0x98, 0x76, 0x77, 0x87, 0x55, 0x88, 0x77, 0x57, 0x65, 0x55, 0x66, 0x66, 0x55, 0x56, 0x78, 0x89, 0xAA, 0x87, 
        0x53, 0x33, 0x33, 0x34, 0x45, 0x56, 0x66, 0x77, 0x78, 0x88, 0x99, 0x90,
  0x45, 0x42, 0x36, 0x68, 0x75, 0x89, 0x99, 0x99, 0x99, 0x98, 0x76, 0x33, 0x46, 0x77, 0x77, 0x64, 0x43, 0x47, 0x89, 0x98, 0x77, 0x61, 0x22, 0x34, 0x56, 0x66, 0x76, 0x65, 0x57, 0x87, 0x66, 0x66, 0x65, 0x55, 0x56, 0x65, 0x55, 0x56, 0x88, 0x88, 0x88, 0x87, 
        0x76, 0x34, 0x44, 0x45, 0x55, 0x66, 0x67, 0x77, 0x88, 0x88, 0x99, 0x90,
  0x34, 0x33, 0x55, 0x22, 0x23, 0x79, 0x99, 0x99, 0x99, 0x98, 0x88, 0x97, 0x77, 0x77, 0x77, 0x66, 0x66, 0x78, 0x99, 0x88, 0x77, 0x41, 0x11, 0x11, 0x11, 0x22, 0x34, 0x55, 0x67, 0x76, 0x65, 0x66, 0x55, 0x55, 0x55, 0x55, 0x55, 0x7A, 0xA9, 0x88, 0x88, 0x77, 
        0x78, 0x75, 0x55, 0x56, 0x66, 0x66, 0x77, 0x78, 0x88, 0x99, 0x99, 0x90,
  0x65, 0x64, 0x42, 0x12, 0x20, 0x28, 0x99, 0x99, 0x99, 0x98, 0x89, 0xAA, 0x99, 0x98, 0x88, 0x87, 0x77, 0x89, 0x98, 0x87, 0x76, 0x43, 0x21, 0x11, 0x11, 0x11, 0x11, 0x11, 0x23, 0x44, 0x55, 0x66, 0x65, 0x55, 0x55, 0x55, 0x57, 0xBD, 0xDA, 0x78, 0x88, 0x77, 
        0x88, 0x98, 0x66, 0x66, 0x77, 0x77, 0x77, 0x88, 0x89, 0x99, 0x99, 0x90,
  0x14, 0x20, 0x02, 0x13, 0x10, 0x04, 0x89, 0x99, 0x99, 0x98, 0x9A, 0xAA, 0xBA, 0x98, 0x88, 0x77, 0x78, 0x89, 0x98, 0x77, 0x65, 0x55, 0x55, 0x44, 0x32, 0x11, 0x11, 0x11, 0x11, 0x11, 0x12, 0x23, 0x44, 0x55, 0x55, 0x68, 0xAA, 0xAA, 0xB9, 0x87, 0x89, 0x88, 
        0x99, 0xAA, 0x96, 0x67, 0x77, 0x77, 0x77, 0x88, 0x89, 0x99, 0x99, 0x90,
  0x42, 0x21, 0x13, 0x22, 0x11, 0x00, 0x58, 0x89, 0x99, 0x99, 0x9A, 0xBB, 0xAA, 0x99, 0x88, 0x88, 0x89, 0x98, 0x88, 0x76, 0x55, 0x55, 0x55, 0x55, 0x55, 0x54, 0x33, 0x21, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x23, 0x79, 0xCE, 0xDC, 0x98, 0x88, 0x89, 0xA8, 
        0xAA, 0xAB, 0xBA, 0x76, 0x77, 0x77, 0x88, 0x88, 0x99, 0x99, 0x9A, 0xA0,
  0x61, 0x34, 0x12, 0x22, 0x32, 0x10, 0x36, 0x88, 0x88, 0x89, 0xAA, 0xBB, 0xBB, 0xAA, 0xA9, 0x99, 0x99, 0x87, 0x77, 0x65, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x44, 0x32, 0x11, 0x11, 0x11, 0x11, 0x34, 0x99, 0x9A, 0xAA, 0x99, 0x99, 0x89, 0xAA, 
        0xAB, 0xBB, 0xCB, 0xB8, 0x77, 0x88, 0x88, 0x89, 0x99, 0x99, 0x9A, 0xA0,
  0x52, 0x02, 0x22, 0x32, 0x00, 0x11, 0x12, 0x88, 0x88, 0x89, 0x9A, 0xAB, 0xBB, 0xBB, 0xBA, 0xAA, 0x98, 0x77, 0x65, 0x44, 0x45, 0x55, 0x55, 0x45, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x43, 0x32, 0x11, 0x99, 0xAA, 0xA9, 0xAA, 0xAB, 0xAA, 0x9A, 0xAA, 
        0xAB, 0xBB, 0xBC, 0xCC, 0x88, 0x88, 0x88, 0x99, 0x99, 0x99, 0x9A, 0xA0,
  0x10, 0x02, 0x31, 0x31, 0x00, 0x01, 0x10, 0x58, 0x88, 0x88, 0x99, 0xAA, 0xBB, 0xBB, 0xBA, 0xA9, 0x87, 0x66, 0x54, 0x44, 0x44, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x56, 0x99, 0x9A, 0xAA, 0xAB, 0xBB, 0xBB, 0xAA, 0xAC, 
        0xAC, 0xBC, 0xCC, 0xDD, 0xC9, 0x88, 0x89, 0x99, 0x99, 0x99, 0xAA, 0xA0,
  0x31, 0x23, 0x23, 0x52, 0x00, 0x01, 0x01, 0x58, 0x88, 0x88, 0x88, 0x99, 0xAA, 0xAA, 0xA9, 0x88, 0x76, 0x54, 0x44, 0x44, 0x55, 0x55, 0x44, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x58, 0xAA, 0xAA, 0xAA, 0xAB, 0xCB, 0xBB, 0xCB, 0xAB, 
        0xBC, 0xCC, 0xCC, 0xCD, 0xDD, 0x99, 0x99, 0x99, 0x99, 0x99, 0xAA, 0xA0,
  0x27, 0x54, 0x32, 0x20, 0x00, 0x12, 0x48, 0x78, 0x88, 0x88, 0x88, 0x88, 0x99, 0x99, 0x98, 0x87, 0x65, 0x44, 0x44, 0x44, 0x55, 0x55, 0x54, 0x55, 0x55, 0x54, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x69, 0xAA, 0xAA, 0xBB, 0xBA, 0xCD, 0xCB, 0xBC, 0xBB, 
        0xCC, 0xDC, 0xCC, 0xCD, 0xEE, 0xDA, 0x99, 0x99, 0x99, 0x9A, 0xAA, 0xA0,
  0x56, 0x57, 0x32, 0x11, 0x11, 0x04, 0x96, 0x58, 0x98, 0x88, 0x88, 0x77, 0x77, 0x87, 0x77, 0x66, 0x55, 0x44, 0x44, 0x44, 0x44, 0x55, 0x55, 0x55, 0x44, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x79, 0xAA, 0xAA, 0xBB, 0xBC, 0xCD, 0xDC, 0xBC, 0xCB, 
        0xCC, 0xDD, 0xCD, 0xDD, 0xDE, 0xEE, 0xA9, 0x99, 0xAA, 0xAA, 0xAA, 0xA0,
  0x23, 0x42, 0x22, 0x01, 0x12, 0x4A, 0x63, 0x67, 0x88, 0x88, 0x88, 0x87, 0x77, 0x66, 0x65, 0x55, 0x55, 0x54, 0x45, 0x96, 0x34, 0x44, 0x55, 0x54, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x89, 0xBA, 0xBB, 0xCC, 0xCC, 0xCC, 0xDD, 0xCC, 0xDC, 
        0xCC, 0xDD, 0xDD, 0xDD, 0xDD, 0xEE, 0xEB, 0xAA, 0xAA, 0xAA, 0xAA, 0xA0,
  0x22, 0x11, 0x12, 0x58, 0xAA, 0xB9, 0x46, 0x77, 0x88, 0x88, 0x88, 0x77, 0x66, 0x66, 0x66, 0x65, 0x55, 0x54, 0x44, 0x68, 0x63, 0x22, 0x34, 0x44, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x8A, 0xBB, 0xBB, 0xBC, 0xCC, 0xCD, 0xCC, 0xCC, 0xCC, 
        0xBC, 0xDD, 0xDD, 0xDD, 0xDD, 0xDE, 0xEE, 0xCA, 0xAA, 0xAA, 0xAA, 0xB0
};

GUI_CONST_STORAGE GUI_BITMAP bmxm2 = {
  107, // xSize
  64, // ySize
  54, // BytesPerLine
  4, // BitsPerPixel
  _acxm2,  // Pointer to picture data (indices)
  &_Palxm2   // Pointer to palette
};

/*************************** End of file ****************************/
