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
#include "trccom.h"

#define APPTASK_GLOBALS

#include "apptask.h"
#include "pinint.h" 
#include "Oled_driver.h"
extern uint8_t heartcnt;


/*****************************************************************************
 * �� �� ��  : KBD_dev_init
 * �� �� ��  : MZM
 * ��������  : 2016��3��25��
 * ��������  : ������ʼ��
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
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
 * ��������  : Һ��ˢ�³���500ms��ʱˢ��һ�Ρ�
 * �������  : void *pvParameters  �������
 * �������  : ��
 * �� �� ֵ  : static
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
static void Oled_refresh_task(void *pvParameters) 
{
	portBASE_TYPE ret;
	
//	Oled_DMA_init();

	while (1)
	{
		ret = xSemaphoreTake(refresh_LCD_sem,3);// ��ȡ�ź���--ˢ��Һ��
		
		if(ret == pdPASS)
		{
//			rt_kprintf()
//			OLED_refresh_fun();
//			OLED_Clear_screen(LCDBUFF_MZM[0][0]);
			OLED_Clear_screen_firstbuf();


		}
		
//		vTaskDelay(configTICK_RATE_HZ/20 );
		/* About a 3Hz on/off toggle rate */
		taskYIELD();//����������������ִ�С� taskYIELD()֪ͨ���������ھ��л����������񣬶����صȵ��������ʱ��Ƭ�ľ�
	}
}

/*****************************************************************************
 * �� �� ��  : OLED_refresh_task_create
 * �� �� ��  : MZM
 * ��������  : 2016��3��25��
 * ��������  : ����ˢ��Һ������
 * �������  : uint16_t StackDepth  ����ջ��С
               uint8_t Priority     ���ȼ�
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void OLED_refresh_task_create(uint16_t StackDepth, uint8_t Priority)
{
	vSemaphoreCreateBinary(refresh_LCD_sem);		//
	xSemaphoreTake(refresh_LCD_sem,10);				//
	
	OLED_Refresh_Flg_Set(ENABLE_REFRESH_OLED);	//�ϵ�ʹ�� ��ʱҺ��ˢ�£�����������ȡ����ʱˢ��

	xTaskCreate(Oled_refresh_task, "Oled_refresh_task", StackDepth,
			NULL, (tskIDLE_PRIORITY + Priority), (TaskHandle_t *) NULL);
		

}

/*****************************************************************************
 * �� �� ��  : OLED_enable_refresh_ram
 * �� �� ��  : MZM
 * ��������  : 2016��3��25��
 * ��������  : ʹ��Һ��ˢ��
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void OLED_enable_refresh_ram()
{
	OLED_Refresh_Flg_Set(ENABLE_REFRESH_OLED);	
	xSemaphoreGive(refresh_LCD_sem);
}

/*****************************************************************************
 * �� �� ��  : OLED_disable_refresh_ram
 * �� �� ��  : MZM
 * ��������  : 2016��3��25��
 * ��������  : ����Һ��ˢ��
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void OLED_disable_refresh_ram()
{
	OLED_Refresh_Flg_Set(DISABLE_REFRESH_OLED);	
}


/*****************************************************************************
 * �� �� ��  : get_key_value_task
 * �� �� ��  : MZM
 * ��������  : 2016��2��17��
 * ��������  : ��ð�����ֵ������������������10msΪ������Ч������200msΪ��������
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
			if(key_now.keyedge == KEY_EDGE_FALL)//��ǰ��⵽�½���
			{
				fall_edge_ticks = xTaskGetTickCount();//��ȡtick
				fall_edge_flg = TRUE;//�½�����Ч
				tmp_key_now = key_now;//�����½��ؼ�ֵ
				key_cnt = 0;	//����������
				rt_kprintf(DEBUG_TASP_APP,"KEY_EDGE_FALL[%d]\r\n",key_now.keyvalue);
			}
			else if(key_now.keyedge == KEY_EDGE_RISE)//��ǰ��⵽������
			{
//				rise_edge_ticks = xTaskGetTickCount();//��ȡtick
//				
//				rt_kprintf(1,"KEY_EDGE_RISE:now[%d]--last[%d]\r\n",key_now.keyvalue,key_last.keyvalue);
//				rt_kprintf(1,"last keyedge[%d] now keyedge[%d]\r\n",key_last.keyedge,key_now.keyedge);
//				rt_kprintf(1,"fall_edge_ticks[%d] rise_edge_ticks[%d]\r\n",fall_edge_ticks,rise_edge_ticks);
				
//				if(key_now.keyvalue == key_last.keyvalue)// ��ֵһ��
//				{
//					if(key_now.keyedge != key_last.keyedge)
//					{
//						if(rise_edge_ticks > (fall_edge_ticks + SHORT_KEY_DELAY))//���ΰ���֮����ʱ����Ч���жϡ�
//						{
//							key_value = key_now.keyvalue;
//							
//							rt_kprintf(1,"KEY_SHORT_RISE[%d]\r\n",key_value);
//							xStatus = xQueueSend(app_task_key_value_queue,&key_value,0);
//							
//							key_last = key_now;//��������
//							
//							if( xStatus != pdPASS )
//							{
//									//����������
//							}
//							else
//							{
//								//�������ݼ�����С�	
//							}
//						}
//					}
//				}
//				else
//				{
//					key_last = key_now;//��������
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
				cur_ticks = xTaskGetTickCount();//��ȡtick
				check_key_valid_ticks = cur_ticks - fall_edge_ticks;
				if((key_cnt > 0)&&((check_key_valid_ticks) > (LONG_KEY_DELAY)))
				{
					if((check_key_valid_ticks) > (LONG_KEY_DELAY+LONG_KEY_VALID*key_cnt))
					{
						rt_kprintf(DEBUG_TASP_APP,"KEY_LONG[%d]\r\n",tmp_key_now.keyvalue);					
//						rt_kprintf(DEBUG_TASP_APP,"cur_tick[%x],fall_tick[%x],---[%d]-key_cnt[%d]\r\n",cur_ticks,fall_edge_ticks,check_key_valid_ticks,key_cnt);
						//������
						key_value = tmp_key_now.keyvalue;
						xStatus = xQueueSend(app_task_key_value_queue,&key_value,0);
						key_cnt++;
					}

				}
				else if((key_cnt == 0) && ((check_key_valid_ticks) > (SHORT_KEY_DELAY)))
				{
					//�̰�����Ч
					key_cnt=1;
					key_last = key_now;//��������
					
					rt_kprintf(DEBUG_TASP_APP,"KEY_SHORT[%d]\r\n",tmp_key_now.keyvalue);
//					rt_kprintf(DEBUG_TASP_APP,"cur_tick[%x],fall_tick[%x],---[%d]\r\n",cur_ticks,fall_edge_ticks,check_key_valid_ticks);
					key_value = tmp_key_now.keyvalue;
					xStatus = xQueueSend(app_task_key_value_queue,&key_value,0);
				}
			}
		}
		
		taskYIELD();//����������������ִ�С� taskYIELD()֪ͨ���������ھ��л����������񣬶����صȵ��������ʱ��Ƭ�ľ�
		/* About a 3Hz on/off toggle rate */
//		vTaskDelay(configTICK_RATE_HZ/10 );
	}
}

//void  fun_print()
//{
//	rt_kprintf(DEBUG_SYS,"ENTER---FUN[%s()]\r\n",__FUNCTION__);
//}
/*****************************************************************************
 * �� �� ��  : get_key_value_task
 * �� �� ��  : MZM
 * ��������  : 2016��2��17��
 * ��������  : Һ��ˢ�³���
 * �������  : void *pvParameters  �������
 * �������  : ��
 * �� �� ֵ  : static
 * ���ù�ϵ  : 
 * ��    ��  : 

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
		
			//test_key_oled(key_value);//���԰���
			keyoperation(key_value);

//			key_value += '0';
//			uart_send_data("KEY[",4);
//			uart_send_data(&key_value,1);
//			uart_send_data("]\r\n",4);
		}
		else
		{
		
		}
		
		taskYIELD();//����������������ִ�С� taskYIELD()֪ͨ���������ھ��л����������񣬶����صȵ��������ʱ��Ƭ�ľ�
//		/* About a 3Hz on/off toggle rate */
//		vTaskDelay(configTICK_RATE_HZ/10 );
	}
}

/*****************************************************************************
 * �� �� ��  : Key_Board_task_create
 * �� �� ��  : MZM
 * ��������  : 2016��3��25��
 * ��������  : ����������������������̣߳���ȡ��ֵ����������
 * �������  : uint16_t StackDepth  ջ��С
               uint8_t Priority     ���ȼ�
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void Key_Board_task_create(uint16_t StackDepth, uint8_t Priority)
{
	KBD_dev_init();// �����豸��ʼ��

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
				xSemaphoreGive(refresh_LCD_sem);// 500ms ˢ��һ��
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
//				xSemaphoreGive(refresh_LCD_sem);// 500ms ˢ��һ��
//				Board_LED_Set(0, LedState);
				LedState = (bool) !LedState;
			}
			if(flg5000ms)
			{
				//do_1000ms_task();
				flg1000ms=0;
				
			}
		}
	

//		taskYIELD();//����������������ִ�С� taskYIELD()֪ͨ���������ھ��л����������񣬶����صȵ��������ʱ��Ƭ�ľ�
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
 * �� �� ��  : check_user_cmd
 * �� �� ��  : MZM
 * ��������  : 2016��3��25��
 * ��������  : �����ջ��������Ƿ�����Զ�������
 * �������  : TRC_COM_P pcom_data  ���ڽṹ��
               uint8_t *tmpbuf      ���ջ�����
               uint32_t len         ����
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

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
#ifdef  TESTFORJJJ 	//Ϊ�𾧾�������Ƶ������չ����---��ʽ�汾Ҫȥ��
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
 * �� �� ��  : vUart_Server_Task
 * �� �� ��  : MZM
 * ��������  : 2016��3��25��
 * ��������  : ���ڷ������
 * �������  : void *pvParameters  ���ڷ������
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

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
			//bytes = Chip_UART_ReadRB(pUART, pcom_data->rx_driver_buf_ptr, tmpbuf, APP_READ_NUM);//��ȡ����
			
		ret = xSemaphoreTake(pcom_data->driver_sem_receive_data,3);
		if(ret != pdPASS)
		{					
			taskYIELD();//����������������ִ�С� taskYIELD()֪ͨ���������ھ��л����������񣬶����صȵ��������ʱ��Ƭ�ľ�
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
					
					if(tmpbuf[j]==STX)//�ж� ��ʼ��־
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
					else if(tmpbuf[j]==ETX)//������־
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
					else if(tmpbuf[j]==ELP)//ת���ַ�
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
 * �� �� ��  : Task_Create_Uart0_Server
 * �� �� ��  : MZM
 * ��������  : 2016��3��25��
 * ��������  : �������ڷ������
 * �������  : uint16_t StackDepth  ջ��С
               uint8_t Priority     ���ȼ�
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void Task_Create_Uart0_Server(uint16_t StackDepth, uint8_t Priority)
{
	portBASE_TYPE ret;
	
		/* ����ADC0�Ķ���ת������A �����ݴ������� */
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
 * �� �� ��  : vprotocolTask
 * �� �� ��  : MZM
 * ��������  : 2016��3��25��
 * ��������  : Э�鴦������
 * �������  : void *pvParameters  �������
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

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
					Packet_Vtr_Up_Data(pframebuf);//ֱ�ӷ����Ѿ���õı���
				}
				else
				{
//						Vtr_Fail_Return_Respond(pframebuf,ret);//��֯ȷ�Ϸ���֡������
				}

				Do_Somethings_After_Send_Data(pframebuf);
			}
			else{
				//rt_kprintf("Protocol Recv CS--ERROR\r\n");
				//�հ�����
			//	Vtr_Fail_Return_Respond(pframebuf,RET_FAIL_F6);//��֯����֡����ͨ��uart0���͸�PC
			//	check_sum_failcnt++;
				}
			
		}
		
		vTaskDelay(configTICK_RATE_HZ/100);
		
	}
}

/*****************************************************************************
 * �� �� ��  : Task_Create_Com_Protocol
 * �� �� ��  : MZM
 * ��������  : 2016��3��25��
 * ��������  : ����Э�鴦������
 * �������  : uint16 StackDepth  ջ��С
               uint8 Priority     ���ȼ�
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

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
	KB_OLED_App_init();						//�����豸��ʼ��
	OLED_refresh_task_create(256,0);		//����ˢ��Һ������
	Task_Create_10ms(128,2);				//10 ms ���񴴽�
	Key_Board_task_create(256,2);			//��������ʾ �������
	Task_Create_Uart0_Server(256,1);		// ���ڷ������
	Task_Create_Com_Protocol(256,1);		//Э������������

	return ;
}
