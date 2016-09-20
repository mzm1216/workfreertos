
#ifndef CONFIG_H
#define CONFIG_H

#include "lpc_types.h"
#include "stdint.h"


#ifdef  __cplusplus
extern "C" {
#endif

#define		VTR_VERX	2
#define		VTR_VERY	8
#define		VTR_VERZ	7


//typedef unsigned char  uint8_t;                                           /*  无符??位整型变??          */
//typedef signed   char  int8_t;                                            /*  有符??位整型变??          */
//typedef unsigned short uint16_t;                                          /*  无符??6位整型变??         */
//typedef signed   short int16_t;                                           /*  有符??6位整型变??         */
//typedef unsigned long   uint32_t;                                          /*  无符??2位整型变??         */
//typedef signed   long   int32_t;                                           /*  有符??2位整型变??         */
typedef float          fp32_t;                                            /*  单精度浮点数??2位长度）    */
typedef double         fp64_t;                                            /*  双精度浮点数??4位长度）    */

#include "ioconfig.h"



#define PARAM_OFFSET(Strct, Field) ((unsigned long)(unsigned long*)&(((Strct *)0)->Field))

#endif /* MAINVTR_H */
