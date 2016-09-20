/***********************************************************************************
 * �� �� ��   : apptask.c
 * �� �� ��   : MZM
 * ��������   : 2016��3��2��
 * �ļ�����   : apptask������Ӧ�ó�������񣬶�Ӧ��������ļ��У���AppTaskCreate��
                ��������������Ĵ�����
 * ��Ȩ˵��   : Copyright (c) 2008-2016   �������������ͨѶ�Ƽ����޹�˾
 * ��    ��   : 
 * �޸���־   : 
***********************************************************************************/
#define APPTASK_GLOBALS

#include "apptask.h"
#include "pinint.h" 



void KBD_dev_init()
{
	app_task_get_key_queue = xQueueCreate(10,4); //������������
	app_task_key_value_queue = xQueueCreate(20,1);//�����洢����
	pin_interrupt_init();

}

/*****************************************************************************
 * �� �� ��  : Oled_flush_task
 * �� �� ��  : MZM
 * ��������  : 2016��2��17��
 * ��������  : Һ��ˢ�³���
 * �������  : void *pvParameters  �������
 * �������  : ��
 * �� �� ֵ  : static
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
static void Oled_flush_task(void *pvParameters) 
{
	bool LedState = false;
	portBASE_TYPE ret;

	while (1)
	{
		ret=xSemaphoreTake(Sem_FlushLCD_Deal,1);//
		if(ret==pdPASS)
		{
			OLCD_Flush_Fun();

		}
		
		/* About a 3Hz on/off toggle rate */
		vTaskDelay(configTICK_RATE_HZ/10 );
	}
}
void OLED_flush_task_create(uint16_t StackDepth, uint8_t Priority)
{
	vSemaphoreCreateBinary(Sem_FlushLCD_Deal);//
	LCD_Flush_Flg_Set(0);	
	xSemaphoreTake(Sem_FlushLCD_Deal,10);//

	xTaskCreate(Oled_flush_task, "Oled_flush_task", StackDepth,
			NULL, (tskIDLE_PRIORITY + Priority), (TaskHandle_t *) NULL);
		

}

void flush_Oled_ram()
{
	LCD_Flush_Flg_Set(1);	
	xSemaphoreGive(Sem_FlushLCD_Deal);
}

/*****************************************************************************
 * �� �� ��  : Oled_flush_task
 * �� �� ��  : MZM
 * ��������  : 2016��2��17��
 * ��������  : Һ��ˢ�³���
 * �������  : void *pvParameters  �������
 * �������  : ��
 * �� �� ֵ  : static
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
static void get_key_value_task(void *pvParameters) 
{
	uint8_t key_value;
	portBASE_TYPE xStatus;	
	KB_QUEUE_VALUE_T key_last,key_now;
    const portTickType xTicksToWait = 1000 / portTICK_RATE_MS;

	while (1)
	{
		xStatus = xQueueReceive( app_task_get_key_queue, &key_now, xTicksToWait );
		if(xStatus==pdPASS)
		{
			if(key_now.keyvalue == key_last.keyvalue)// ��ֵһ��
			{
				if((key_now.keyedge != key_last.keyedge)&&(key_now.keyedge == KEY_EDGE_RISE))//��ǰ��⵽������
				{
					if(key_now.ticks > (key_last.ticks + KEY_VALID_TIME_DELAY))//���ΰ���֮����ʱ����Ч���жϡ�
					{
						key_value = key_now.keyvalue;
						xStatus = xQueueSend(app_task_key_value_queue,&key_value,0);
						
						if( xStatus != pdPASS )
						{
								//����������
						}
						else
						{
							//�������ݼ�����С�	
						}
					}
				}
			}
			key_last = key_now;
		}
		else
		{
		
		}
		/* About a 3Hz on/off toggle rate */
		vTaskDelay(configTICK_RATE_HZ/10 );
	}
}

void Key_Board_task_create(uint16_t StackDepth, uint8_t Priority)
{
//	KBD_dev_init();// �����豸��ʼ��

	xTaskCreate(get_key_value_task, "get_key_value_task", StackDepth,
			NULL, (tskIDLE_PRIORITY + Priority), (TaskHandle_t *) NULL);
		

}


void v10msTask(void *pvParameters) 
{
	uint32_t lasttick;
	uint32_t flg10ms,flg500ms,flg1000ms,flg2500ms,flg5000ms;
	TRC_COM_P pcom_data;

	lasttick=xTaskGetTickCount();
	flg10ms=0;
	
//	rt_kprintf(DEBUG_SYS,"FUNCTION:	[%s()]-	Tick[%d] \r\n",__FUNCTION__,Getsystick());
	while (1)
	{
		if(xTaskGetTickCount()-lasttick >=10)
		{
			flg10ms++;
			lasttick=xTaskGetTickCount();
		}
		else
		{
			taskYIELD();
			//vTaskDelay(configTICK_RATE_HZ/20);
			continue;
		}
		flg500ms =(flg10ms %50==0)?1:0;
		flg1000ms =(flg10ms %100==0)?1:0;
		flg5000ms =(flg10ms %500==0)?1:0;
		if(flg500ms)
		{
			flg500ms = 0;
			
			xSemaphoreGive(Sem_FlushLCD_Deal);// 500ms ˢ��һ��
		}
		if(flg1000ms)
		{
			//do_1000ms_task();
			flg1000ms=0;
			
		}
		if(flg5000ms)
		{
			//do_1000ms_task();
			flg1000ms=0;
			
		}

		taskYIELD();//����������������ִ�С� taskYIELD()֪ͨ���������ھ��л����������񣬶����صȵ��������ʱ��Ƭ�ľ�
		vTaskDelay(configTICK_RATE_HZ/10);
		
	}
}


void Task_Create_10ms(uint16_t StackDepth, uint8_t Priority)
{
	portBASE_TYPE ret;
	
		ret=xTaskCreate(v10msTask, (signed char *) "v10msTask",
							StackDepth, NULL, (tskIDLE_PRIORITY + Priority),
							(xTaskHandle *) NULL);
//		if(ret==pdTRUE)
//		{
//			rt_kprintf(DEBUG_SYS,"Create Task Sucess:--v250msTask\r\n");
//		}
//		else
//		{
//			rt_kprintf(DEBUG_SYS,"Create Task FAIL:--v250msTask\r\n");
//		}

}


/*****************************************************************************
 * �� �� ��  : AppTaskCreate
 * �� �� ��  : MZM
 * ��������  : 2016��3��2��
 * ��������  : ��main��������ֱ�ӵ��ã����ڴ���Ӧ�ó�������
 * �������  : void  wu
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void AppTaskCreate(void)
{

	OLED_flush_task_create(128,1);	//����ˢ��Һ������
	
	Task_Create_10ms(128,2);
	Key_Board_task_create(128,2);

	/* Should never arrive here */
	return ;
}
