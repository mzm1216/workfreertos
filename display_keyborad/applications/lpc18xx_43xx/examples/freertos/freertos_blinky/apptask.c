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
#include "trccom.h"

#define APPTASK_GLOBALS

#include "apptask.h"
#include "pinint.h" 
#include "Oled_driver.h"
extern uint8_t heartcnt;


/*****************************************************************************
 * 函 数 名  : KBD_dev_init
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月25日
 * 函数功能  : 按键初始化
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
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
 * 函数功能  : 液晶刷新程序，500ms定时刷新一次。
 * 输入参数  : void *pvParameters  传入参数
 * 输出参数  : 无
 * 返 回 值  : static
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
static void Oled_refresh_task(void *pvParameters) 
{
	portBASE_TYPE ret;
	
//	Oled_DMA_init();

	while (1)
	{
		ret = xSemaphoreTake(refresh_LCD_sem,3);// 获取信号量--刷新液晶
		
		if(ret == pdPASS)
		{
//			rt_kprintf()
//			OLED_refresh_fun();
//			OLED_Clear_screen(LCDBUFF_MZM[0][0]);
			OLED_Clear_screen_firstbuf();


		}
		
//		vTaskDelay(configTICK_RATE_HZ/20 );
		/* About a 3Hz on/off toggle rate */
		taskYIELD();//允许其它发送任务执行。 taskYIELD()通知调度器现在就切换到其它任务，而不必等到本任务的时间片耗尽
	}
}

/*****************************************************************************
 * 函 数 名  : OLED_refresh_task_create
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月25日
 * 函数功能  : 创建刷新液晶任务
 * 输入参数  : uint16_t StackDepth  任务栈大小
               uint8_t Priority     优先级
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void OLED_refresh_task_create(uint16_t StackDepth, uint8_t Priority)
{
	vSemaphoreCreateBinary(refresh_LCD_sem);		//
	xSemaphoreTake(refresh_LCD_sem,10);				//
	
	OLED_Refresh_Flg_Set(ENABLE_REFRESH_OLED);	//上电使能 定时液晶刷新，按键操作将取消定时刷新

	xTaskCreate(Oled_refresh_task, "Oled_refresh_task", StackDepth,
			NULL, (tskIDLE_PRIORITY + Priority), (TaskHandle_t *) NULL);
		

}

/*****************************************************************************
 * 函 数 名  : OLED_enable_refresh_ram
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月25日
 * 函数功能  : 使能液晶刷新
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void OLED_enable_refresh_ram()
{
	OLED_Refresh_Flg_Set(ENABLE_REFRESH_OLED);	
	xSemaphoreGive(refresh_LCD_sem);
}

/*****************************************************************************
 * 函 数 名  : OLED_disable_refresh_ram
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月25日
 * 函数功能  : 禁用液晶刷新
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void OLED_disable_refresh_ram()
{
	OLED_Refresh_Flg_Set(DISABLE_REFRESH_OLED);	
}


/*****************************************************************************
 * 函 数 名  : get_key_value_task
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月17日
 * 函数功能  : 获得按键键值。防抖处理，按键持续10ms为按键有效，超过200ms为持续按键
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
	KB_QUEUE_VALUE_T key_last,key_now,tmp_key_now;
   	 const portTickType xTicksToWait = 1000 / portTICK_RATE_MS;
	uint32_t fall_edge_ticks,rise_edge_ticks,cur_ticks,check_key_valid_ticks;
	uint32_t key_cnt = 0xFF,fall_edge_flg = 0xFF;

	memset((void*)&key_now,0,sizeof(KB_QUEUE_VALUE_T));
	memset((void*)&key_last,0,sizeof(KB_QUEUE_VALUE_T));

	while (1)
	{
		xStatus = xQueueReceive( app_task_get_key_queue, &key_now, 3 );
		if(xStatus==pdPASS)
		{
			if(key_now.keyedge == KEY_EDGE_FALL)//当前检测到下降沿
			{
				fall_edge_ticks = xTaskGetTickCount();//获取tick
				fall_edge_flg = TRUE;//下降沿有效
				tmp_key_now = key_now;//备份下降沿键值
				key_cnt = 0;	//按键计数器
				rt_kprintf(DEBUG_TASP_APP,"KEY_EDGE_FALL[%d]\r\n",key_now.keyvalue);
			}
			else if(key_now.keyedge == KEY_EDGE_RISE)//当前检测到上升沿
			{
//				rise_edge_ticks = xTaskGetTickCount();//获取tick
//				
//				rt_kprintf(1,"KEY_EDGE_RISE:now[%d]--last[%d]\r\n",key_now.keyvalue,key_last.keyvalue);
//				rt_kprintf(1,"last keyedge[%d] now keyedge[%d]\r\n",key_last.keyedge,key_now.keyedge);
//				rt_kprintf(1,"fall_edge_ticks[%d] rise_edge_ticks[%d]\r\n",fall_edge_ticks,rise_edge_ticks);
				
//				if(key_now.keyvalue == key_last.keyvalue)// 键值一样
//				{
//					if(key_now.keyedge != key_last.keyedge)
//					{
//						if(rise_edge_ticks > (fall_edge_ticks + SHORT_KEY_DELAY))//两次按键之间延时的有效性判断。
//						{
//							key_value = key_now.keyvalue;
//							
//							rt_kprintf(1,"KEY_SHORT_RISE[%d]\r\n",key_value);
//							xStatus = xQueueSend(app_task_key_value_queue,&key_value,0);
//							
//							key_last = key_now;//拷贝按键
//							
//							if( xStatus != pdPASS )
//							{
//									//按键队列满
//							}
//							else
//							{
//								//按键数据加入队列。	
//							}
//						}
//					}
//				}
//				else
//				{
//					key_last = key_now;//拷贝按键
//				}
//				
				fall_edge_ticks = 0;
				rise_edge_ticks = 0;
				key_cnt = 0xFF;
				fall_edge_flg = FALSE;
			}
		}
		else
		{
			if(fall_edge_flg == TRUE)
			{
				cur_ticks = xTaskGetTickCount();//获取tick
				check_key_valid_ticks = cur_ticks - fall_edge_ticks;
				if((key_cnt > 0)&&((check_key_valid_ticks) > (LONG_KEY_DELAY)))
				{
					if((check_key_valid_ticks) > (LONG_KEY_DELAY+LONG_KEY_VALID*key_cnt))
					{
						rt_kprintf(DEBUG_TASP_APP,"KEY_LONG[%d]\r\n",tmp_key_now.keyvalue);					
//						rt_kprintf(DEBUG_TASP_APP,"cur_tick[%x],fall_tick[%x],---[%d]-key_cnt[%d]\r\n",cur_ticks,fall_edge_ticks,check_key_valid_ticks,key_cnt);
						//长按键
						key_value = tmp_key_now.keyvalue;
						xStatus = xQueueSend(app_task_key_value_queue,&key_value,0);
						key_cnt++;
					}

				}
				else if((key_cnt == 0) && ((check_key_valid_ticks) > (SHORT_KEY_DELAY)))
				{
					//短按键有效
					key_cnt=1;
					key_last = key_now;//拷贝按键
					
					rt_kprintf(DEBUG_TASP_APP,"KEY_SHORT[%d]\r\n",tmp_key_now.keyvalue);
//					rt_kprintf(DEBUG_TASP_APP,"cur_tick[%x],fall_tick[%x],---[%d]\r\n",cur_ticks,fall_edge_ticks,check_key_valid_ticks);
					key_value = tmp_key_now.keyvalue;
					xStatus = xQueueSend(app_task_key_value_queue,&key_value,0);
				}
			}
		}
		
		taskYIELD();//允许其它发送任务执行。 taskYIELD()通知调度器现在就切换到其它任务，而不必等到本任务的时间片耗尽
		/* About a 3Hz on/off toggle rate */
//		vTaskDelay(configTICK_RATE_HZ/10 );
	}
}

//void  fun_print()
//{
//	rt_kprintf(DEBUG_SYS,"ENTER---FUN[%s()]\r\n",__FUNCTION__);
//}
/*****************************************************************************
 * 函 数 名  : get_key_value_task
 * 负 责 人  : MZM
 * 创建日期  : 2016年2月17日
 * 函数功能  : 液晶刷新程序
 * 输入参数  : void *pvParameters  传入参数
 * 输出参数  : 无
 * 返 回 值  : static
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
static void display_key_menu_task(void *pvParameters) 
{
	uint8_t key_value;
	portBASE_TYPE xStatus;	
	KB_QUEUE_VALUE_T key_last,key_now;
    const portTickType xTicksToWait = 1000 / portTICK_RATE_MS;
	
	//oled_top_windows();
	//OLED_enable_refresh_ram();
	GUI_Display_init();
	Disp_text_mode_general();
	DispMenuTop();
	OLED_enable_refresh_ram();
	while (1)
	{
		xStatus = xQueueReceive( app_task_key_value_queue, &key_value, 3 );
		if(xStatus==pdPASS)
		{
		
			//test_key_oled(key_value);//测试按键
			keyoperation(key_value);

//			key_value += '0';
//			uart_send_data("KEY[",4);
//			uart_send_data(&key_value,1);
//			uart_send_data("]\r\n",4);
		}
		else
		{
		
		}
		
		taskYIELD();//允许其它发送任务执行。 taskYIELD()通知调度器现在就切换到其它任务，而不必等到本任务的时间片耗尽
//		/* About a 3Hz on/off toggle rate */
//		vTaskDelay(configTICK_RATE_HZ/10 );
	}
}

/*****************************************************************************
 * 函 数 名  : Key_Board_task_create
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月25日
 * 函数功能  : 创建按键处理所需的两个线程：获取键值、按键处理
 * 输入参数  : uint16_t StackDepth  栈大小
               uint8_t Priority     优先级
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void Key_Board_task_create(uint16_t StackDepth, uint8_t Priority)
{
	KBD_dev_init();// 按键设备初始化

	xTaskCreate(get_key_value_task, "get_key_value_task", StackDepth,
			NULL, (tskIDLE_PRIORITY + Priority), (TaskHandle_t *) NULL);
	xTaskCreate(display_key_menu_task, "display_key_menu_task", StackDepth*4,
				NULL, (tskIDLE_PRIORITY + Priority), (TaskHandle_t *) NULL);

}


void v10msTask(void *pvParameters) 
{
	uint32_t lasttick;
	uint32_t flg10ms,flg500ms,flg1000ms,flg2500ms,flg5000ms;
	//TRC_COM_P pcom_data;
	bool LedState = false;

	lasttick=xTaskGetTickCount();
	flg10ms=0;
	
	rt_kprintf(DEBUG_SYS,"FUNCTION:	[%s()]-	Tick[%d] \r\n",__FUNCTION__,lasttick);
	while (1)
	{
		if(xTaskGetTickCount()-lasttick >=100)
		{
			flg10ms++;
			lasttick=xTaskGetTickCount();

			flg500ms =(flg10ms %5==0)?1:0;
			flg1000ms =(flg10ms %10==0)?1:0;
			flg5000ms =(flg10ms %50==0)?1:0;
			if(flg500ms)
			{
				flg500ms = 0;
				heartcnt++;
//				test_draw_heart(heartcnt);
//				test_ssd2119(lasttick);
				xSemaphoreGive(refresh_LCD_sem);// 500ms 刷新一次
			}
			if(flg1000ms)
			{
				//do_1000ms_task();
				rt_kprintf(DEBUG_SYS,"FUNCTION: 1S \r\n");
				flg1000ms=0;
				 
				 Board_LED_Set(LED_WARN, LedState);
				 Board_LED_Set(LED_TX, LedState);
				 Board_LED_Set(LED_RX, LedState);
//				task_draw_net_icon(LedState);
//				task_draw_msc_icon(LedState);
//				task_draw_db_icon(LedState);
//				xSemaphoreGive(refresh_LCD_sem);// 500ms 刷新一次
//				Board_LED_Set(0, LedState);
				LedState = (bool) !LedState;
			}
			if(flg5000ms)
			{
				//do_1000ms_task();
				flg1000ms=0;
				
			}
		}
	

//		taskYIELD();//允许其它发送任务执行。 taskYIELD()通知调度器现在就切换到其它任务，而不必等到本任务的时间片耗尽
		vTaskDelay(configTICK_RATE_HZ/50);
		
	}
}


void Task_Create_10ms(uint16_t StackDepth, uint8_t Priority)
{
	portBASE_TYPE ret;
	
		ret=xTaskCreate(v10msTask, (signed char *) "v10msTask",
							StackDepth, NULL, (tskIDLE_PRIORITY + Priority),
							(xTaskHandle *) NULL);
		if(ret==pdTRUE)
		{
			rt_kprintf(DEBUG_SYS,"Create Task Sucess:--v250msTask\r\n");
		}
		else
		{
			rt_kprintf(DEBUG_SYS,"Create Task FAIL:--v250msTask\r\n");
		}

}

uint8 do_check_frame_cs(uint8 *databuf,uint16 len)
{
	uint16 i;
	UintType check_sum;
	check_sum.w=0;
	for(i=3;i<(len-3);i++)//????
	{
		check_sum.w=(uint16)(check_sum.w+databuf[i]);//????cs=??+??????
	}
	
	if((check_sum.bb[0]==databuf[len-2])&&(check_sum.bb[1]==databuf[len-3]))
		{
		return TRUE;
		}
	else{
		return FALSE;
		}
}

/*****************************************************************************
 * 函 数 名  : check_user_cmd
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月25日
 * 函数功能  : 检查接收缓冲区中是否存在自定义命令
 * 输入参数  : TRC_COM_P pcom_data  串口结构体
               uint8_t *tmpbuf      接收缓冲区
               uint32_t len         长度
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void check_user_cmd(TRC_COM_P pcom_data,uint8_t *tmpbuf,uint32_t len)
{
	uint32_t j;

	for(j=0;j<len;j++)
	{
		if(tmpbuf[j]=='D'
			&&tmpbuf[j+1]=='E'
			&&tmpbuf[j+2]=='B'
			&&tmpbuf[j+3]=='U'
			&&tmpbuf[j+4]=='G'
			&&tmpbuf[j+5]==':')
			{
				if(tmpbuf[j+6]=='0')
					{
						Clear_Debug_Level();
					}
				else if(tmpbuf[j+6]=='F')
					{
						Set_All_Debug_Level();
					}
				else{
					Set_Debug_Level(atoi(&tmpbuf[j+6]));
					}
				
			}
#ifdef  TESTFORJJJ 	//为金晶晶调试射频做的扩展命令---正式版本要去掉
		else 
		if(	tmpbuf[j+0]=='D'&&
			tmpbuf[j+1]=='E'&&
			tmpbuf[j+2]=='L'&&
			tmpbuf[j+3]=='A'&&
			tmpbuf[j+4]=='Y'&&
			tmpbuf[j+5]==':')
			{
				pcom_data->test_data.delay0 = atoi(&tmpbuf[j+6]);
				pcom_data->test_data.flag |= 0x1<<0;
			}
		else 
		if( tmpbuf[j+0]=='A'&&
			tmpbuf[j+1]=='D'&&
			tmpbuf[j+2]=='3'&&
			tmpbuf[j+3]=='C'&&
			tmpbuf[j+4]=='H'&&
			tmpbuf[j+5]==':')
			{
				pcom_data->test_data.ad_3ch= atoi(&tmpbuf[j+6]);
				pcom_data->test_data.flag |= 0x1<<1;
			}






#endif
	}

}

/*****************************************************************************
 * 函 数 名  : vUart_Server_Task
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月25日
 * 函数功能  : 串口服务程序
 * 输入参数  : void *pvParameters  串口服务程序
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void vUart_Server_Task(void *pvParameters) 
{
	uint32 bytes;
	uint16 i,j,tmpflg;
	uint8 *tmpbuf,*readbufhead;

	TRC_COM_P pcom_data;
	LPC_USART_T *pUART;
	
	BaseType_t  ret;

	rt_kprintf(DEBUG_SYS,"FUNCTION:	[%s()]-	Tick[%d] \r\n",__FUNCTION__,xTaskGetTickCount());
	
	tmpbuf=(uint8*) pvPortMalloc(APP_READ_NUM);

	if(tmpbuf==NULL)
		{
		rt_kprintf(DEBUG_SYS,"vuartserverTask malloc[tmpbuf]  error!!\r\n");
		}
	else{
		
		rt_kprintf(DEBUG_SYS,"vuartserverTask malloc[tmpbuf]  SUCESS!!\r\n");
		}
	for(i=0;i<MAX_COM_PORT;i++)
	{
		TRC_COM_init_app(i);
	}
	while(1)
	{
		for(i=0;i<MAX_COM_PORT;i++)
		{
			pcom_data=(TRC_COM_P)Get_TRC_Com_Dev(i);
			if(pcom_data==NULL)
				{
				
				vTaskDelay(configTICK_RATE_HZ/40);
				continue;
				}
		//	pUART=(LPC_USART_T *)Get_TRC_Com_Dev_Uart(i);
			//bytes = Chip_UART_ReadRB(pUART, pcom_data->rx_driver_buf_ptr, tmpbuf, APP_READ_NUM);//读取数据
			
		ret = xSemaphoreTake(pcom_data->driver_sem_receive_data,3);
		if(ret != pdPASS)
		{					
			taskYIELD();//允许其它发送任务执行。 taskYIELD()通知调度器现在就切换到其它任务，而不必等到本任务的时间片耗尽
		}
		
		bytes = uart_read_data(tmpbuf, APP_READ_NUM);
			if (bytes > 0) 
			{
			
				rt_kprintf(DEBUG_TASK_UART,"RECV--Num[%d] buf[%s]\r\n",bytes,tmpbuf);
				for(j=0;j<bytes;j++)
				{

					check_user_cmd(pcom_data,tmpbuf+j,bytes-j);
//					if(tmpbuf[j]=='D'
//						&&tmpbuf[j+1]=='E'
//						&&tmpbuf[j+2]=='B'
//						&&tmpbuf[j+3]=='U'
//						&&tmpbuf[j+4]=='G'
//						&&tmpbuf[j+5]==':')
//						{
//							if(tmpbuf[j+6]=='0')
//								{
//									Clear_Debug_Level();
//								}
//							else if(tmpbuf[j+6]=='F')
//								{
//									Set_All_Debug_Level();
//								}
//							else{
//								Set_Debug_Level(atoi(&tmpbuf[j+6]));
//								}
//							
//						}
					
					if(tmpbuf[j]==STX)//判断 起始标志
					{
						rt_kprintf(DEBUG_TASK_UART,"1 read_ok---us-take-start\r\n");
//						ret=xSemaphoreTake(pcom_data->mutex_sem_read_ok,portTICK_RATE_MS*10);
						ret=xSemaphoreTake(pcom_data->mutex_sem_read_ok,10);
						if(ret==pdFALSE)
						{
								//vTaskDelay(configTICK_RATE_HZ/500);
							continue;
						}
						
						rt_kprintf(DEBUG_TASK_UART,"2 read_ok---us-take-Sucess\r\n");
						pcom_data->start_flg=1;
						pcom_data->end_flg=0;
						pcom_data->escape_flg=0;
						RingBuffer_Flush(&pcom_data->rx_frame_buf);
						RingBuffer_Insert(&pcom_data->rx_frame_buf,&tmpbuf[j]);
						
					}
					else if(tmpbuf[j]==ETX)//结束标志
					{
						if(pcom_data->start_flg==1)
						{
							pcom_data->start_flg=0;
							pcom_data->end_flg=1;
							
							RingBuffer_Insert(&pcom_data->rx_frame_buf,&tmpbuf[j]);
							
							//rt_pirntf_hex(pcom_data->rx_frame_buf.data,pcom_data->rx_frame_buf.head);
							//xSemaphoreTake(pcom_data->mutex_sem_read_ok,portMAX_DELAY);
							rt_kprintf(DEBUG_TASK_UART,"3 write_ok---us-Give-start\r\n");
							
							rt_kprintf(DEBUG_TASK_UART,"4 write_ok---us-Give-Sucess\r\n");
							xSemaphoreGive(pcom_data->mutex_sem_write_ok);
							//rt_kprintf("Givedata--Num[%d] buf[%s]\r\n",pcom_data->rx_frame_buf.head,pcom_data->rx_ringbuf.data);

							
						}
					}
					else if(tmpbuf[j]==ELP)//转换字符
					{
						if(pcom_data->start_flg==1)
						{
							pcom_data->escape_flg=1;
						}
					}

					else
					{
						if(pcom_data->start_flg==1)
						{
							if(pcom_data->escape_flg==1)
							{
								tmpbuf[j]=(uint8)(tmpbuf[j]-0x20);
								pcom_data->escape_flg=0;
							}
							
							RingBuffer_Insert(&pcom_data->rx_frame_buf,&tmpbuf[j]);
						}
						else
						{
							//RingBuffer_Flush(&pcom_data->rx_frame_buf);
						}
						
					}
					
				}
			}
			
		}
			
			
			
			vTaskDelay(configTICK_RATE_HZ/40);
		
	}
}

/*****************************************************************************
 * 函 数 名  : Task_Create_Uart0_Server
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月25日
 * 函数功能  : 创建串口服务程序
 * 输入参数  : uint16_t StackDepth  栈大小
               uint8_t Priority     优先级
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void Task_Create_Uart0_Server(uint16_t StackDepth, uint8_t Priority)
{
	portBASE_TYPE ret;
	
		/* 创建ADC0的独立转换序列A 的数据处理任务 */
	ret = xTaskCreate(vUart_Server_Task, "vUart_Server_Task",
				StackDepth, NULL, (tskIDLE_PRIORITY + Priority),
				(xTaskHandle *) NULL);
		if(ret==pdTRUE)
		{
			rt_kprintf(DEBUG_SYS,"Create Task Sucess:--Task_Create_Uart0_Server\r\n");
		}
		else
		{
			rt_kprintf(DEBUG_SYS,"Create Task FAIL:--Task_Create_Uart0_Server\r\n");
		}

}

/*****************************************************************************
 * 函 数 名  : vprotocolTask
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月25日
 * 函数功能  : 协议处理任务
 * 输入参数  : void *pvParameters  传入参数
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void vprotocolTask(void *pvParameters) 
{
	uint32 bytes,num=0;
	uint16 i,j;
	//uint8 tmpbuf[TRC_COM_MAX_RX_NUM];

	TRC_COM_P pcom_data;
	LPC_USART_T *pUART;
	portBASE_TYPE  ret;

	
	VTR_PROTOCOL_DATA *pframebuf=(VTR_PROTOCOL_DATA *) malloc(sizeof(VTR_PROTOCOL_DATA));
	rt_kprintf(DEBUG_SYS,"FUNCTION:	[%s()]-	Tick[%d] \r\n",__FUNCTION__,xTaskGetTickCount());
	//VTR_PROTOCOL_DATA *pframebuf=(VTR_PROTOCOL_DATA *) malloc(sizeof(VTR_PROTOCOL_DATA));
	if(pframebuf==NULL)
	{
		rt_kprintf(DEBUG_SYS,"vprotocolTask malloc[tmpbuf]	error!!\r\n");
	}
	else
	{
		rt_kprintf(DEBUG_SYS,"vprotocolTask malloc[tmpbuf]	SUCESS!!\r\n");
	}
	
	while(1)
	{
		for(i=0;i<MAX_COM_PORT;i++)
		{
			pcom_data=(TRC_COM_P)Get_TRC_Com_Dev(i);
			if(pcom_data==NULL)
			{
				vTaskDelay(configTICK_RATE_HZ/10);
				continue;
			}
			
//			ret=xSemaphoreTake(pcom_data->mutex_sem_write_ok,portTICK_RATE_MS*5);
			ret=xSemaphoreTake(pcom_data->mutex_sem_write_ok,3);
			if(ret==pdFALSE)
			{
				
				vTaskDelay(configTICK_RATE_HZ/100);
				continue;
			}
			
			rt_kprintf(DEBUG_TASK_UART,"5 write_ok---Poto-Take-Sucess\r\n");
			bytes=RingBuffer_PopMult(&pcom_data->rx_frame_buf,pframebuf->rxd_buf,TRC_COM_MAX_RX_NUM);
			
			rt_kprintf(DEBUG_TASK_UART,"6 read_ok---Poto-Give-Sucess\r\n");
			xSemaphoreGive(pcom_data->mutex_sem_read_ok);
			
			if(bytes>0)
			{
				num++;
				rt_kprintf(DEBUG_TASK_UART,"\r\nNUM[%d]",num);
				rt_pirntf_hex(DEBUG_TASK_UART,pframebuf->rxd_buf,bytes);
			}
			else
			{
				vTaskDelay(configTICK_RATE_HZ/500);
				continue;
			}
			
			if ((bytes >= 7)&& (do_check_frame_cs(pframebuf->rxd_buf,bytes)==TRUE))
			{
				pframebuf->ch = i;
				pframebuf->txd_fun.lab=pframebuf->rxd_buf[1];
				pframebuf->txd_fun.seq=pframebuf->rxd_buf[2];
				pframebuf->txd_fun.cmd=pframebuf->rxd_buf[3];
				pframebuf->txd_fun.lab=0x80;
				ret = Process_Vtr_Param(pframebuf);

				if(ret == suc_respond)
				{
					Packet_Vtr_Up_Data(pframebuf);//直接发送已经组好的报文
				}
				else
				{
//						Vtr_Fail_Return_Respond(pframebuf,ret);//组织确认否认帧并发送
				}

				Do_Somethings_After_Send_Data(pframebuf);
			}
			else{
				//rt_kprintf("Protocol Recv CS--ERROR\r\n");
				//收包错误
			//	Vtr_Fail_Return_Respond(pframebuf,RET_FAIL_F6);//组织否认帧，并通过uart0发送给PC
			//	check_sum_failcnt++;
				}
			
		}
		
		vTaskDelay(configTICK_RATE_HZ/100);
		
	}
}

/*****************************************************************************
 * 函 数 名  : Task_Create_Com_Protocol
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月25日
 * 函数功能  : 创建协议处理任务
 * 输入参数  : uint16 StackDepth  栈大小
               uint8 Priority     优先级
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void Task_Create_Com_Protocol(uint16 StackDepth, uint8 Priority)
{
	portBASE_TYPE ret;

	ret=xTaskCreate(vprotocolTask, (signed char *) "vprotocol",
						StackDepth, NULL, (tskIDLE_PRIORITY + Priority),
						(xTaskHandle *) NULL);
	if(ret==pdTRUE)
	{
		rt_kprintf(DEBUG_SYS,"Create Task Sucess:--vprotocolTask\r\n");
	}
	else
	{
		rt_kprintf(DEBUG_SYS,"Create Task FAIL:--vprotocolTask\r\n");
	}

		
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
	KB_OLED_App_init();						//按键设备初始化
	OLED_refresh_task_create(256,0);		//创建刷新液晶任务
	Task_Create_10ms(128,2);				//10 ms 任务创建
	Key_Board_task_create(256,2);			//按键和显示 相关任务
	Task_Create_Uart0_Server(256,1);		// 串口服务程序
	Task_Create_Com_Protocol(256,1);		//协议解析处理程序

	return ;
}
