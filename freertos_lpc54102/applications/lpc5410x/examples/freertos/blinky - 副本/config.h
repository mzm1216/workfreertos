
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


//typedef unsigned char  uint8_t;                                           /*  �޷�??λ���ͱ�??          */
//typedef signed   char  int8_t;                                            /*  �з�??λ���ͱ�??          */
//typedef unsigned short uint16_t;                                          /*  �޷�??6λ���ͱ�??         */
//typedef signed   short int16_t;                                           /*  �з�??6λ���ͱ�??         */
//typedef unsigned long   uint32_t;                                          /*  �޷�??2λ���ͱ�??         */
//typedef signed   long   int32_t;                                           /*  �з�??2λ���ͱ�??         */
typedef float          fp32_t;                                            /*  �����ȸ�����??2λ���ȣ�    */
typedef double         fp64_t;                                            /*  ˫���ȸ�����??4λ���ȣ�    */

#include "ioconfig.h"



#define PARAM_OFFSET(Strct, Field) ((unsigned long)(unsigned long*)&(((Strct *)0)->Field))

#endif /* MAINVTR_H */
