/***********************************************************************************
 * 文 件 名   : apptask.c
 * 负 责 人   : MZM
 * 创建日期   : 2016年3月2日
 * 文件描述   : apptask，所有应用程序的任务，都应放在这个文件中，由AppTaskCreate（
                ）函数进行任务的创建。
 * 版权说明   : Copyright (c) 2008-2016   北京市万格数码通讯科技有限公司
 * 其    他   : 
 * 修改日志   : 
***********************************************************************************/
#define APPTASK_GLOBALS

#include "apptask.h"
#include "pinint.h" 



void KBD_dev_init()
{
	app_task_get_key_queue = xQueueCreate(10,4); //防抖按键队列
	app_task_key_value_queue = xQueueCreate(20,1);//按键存储队列
	pin_interrupt_init();

}

/*****************************************************************************
 * 函 数 名  : Oled_flush_task
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月17日
 * 函数功能  : 液晶刷新程序
 * 输入参数  : void *pvParameters  传入参数
 * 输出参数  : 无
 * 返 回 值  : static
 * 调用关系  : 
 * 其    它  : 

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
 * 函 数 名  : Oled_flush_task
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月17日
 * 函数功能  : 液晶刷新程序
 * 输入参数  : void *pvParameters  传入参数
 * 输出参数  : 无
 * 返 回 值  : static
 * 调用关系  : 
 * 其    它  : 

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
			if(key_now.keyvalue == key_last.keyvalue)// 键值一样
			{
				if((key_now.keyedge != key_last.keyedge)&&(key_now.keyedge == KEY_EDGE_RISE))//当前检测到上升沿
				{
					if(key_now.ticks > (key_last.ticks + KEY_VALID_TIME_DELAY))//两次按键之间延时的有效性判断。
					{
						key_value = key_now.keyvalue;
						xStatus = xQueueSend(app_task_key_value_queue,&key_value,0);
						
						if( xStatus != pdPASS )
						{
								//按键队列满
						}
						else
						{
							//按键数据加入队列。	
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
//	KBD_dev_init();// 按键设备初始化

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
			
			xSemaphoreGive(Sem_FlushLCD_Deal);// 500ms 刷新一次
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

		taskYIELD();//允许其它发送任务执行。 taskYIELD()通知调度器现在就切换到其它任务，而不必等到本任务的时间片耗尽
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
 * 函 数 名  : AppTaskCreate
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月2日
 * 函数功能  : 由main（）函数直接调用，用于创建应用程序任务
 * 输入参数  : void  wu
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void AppTaskCreate(void)
{

	OLED_flush_task_create(128,1);	//创建刷新液晶任务
	
	Task_Create_10ms(128,2);
	Key_Board_task_create(128,2);

	/* Should never arrive here */
	return ;
}
