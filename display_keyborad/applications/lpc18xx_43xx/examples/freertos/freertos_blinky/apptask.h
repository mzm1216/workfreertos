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

apptask_ext xQueueHandle app_task_get_key_queue;//��ȡ���� ��������
apptask_ext xQueueHandle app_task_key_value_queue;//�洢��ֵ ����
apptask_ext xQueueHandle refresh_LCD_sem;//	ˢ��Һ���ź���--�������ź���

#endif
