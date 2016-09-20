#ifndef _APP_TASK_H
#define _APP_TASK_H

#ifdef  APPTASK_GLOBALS
#define apptask_ext
#else
#define apptask_ext  extern
#endif

#include "board.h"
#include "config.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h" 

apptask_ext xQueueHandle app_task_get_key_queue;//获取按键 防抖队列
apptask_ext xQueueHandle app_task_key_value_queue;//存储键值 队列
apptask_ext xQueueHandle refresh_LCD_sem;//	刷新液晶信号量--二进制信号量

#endif
