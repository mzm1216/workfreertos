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

#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "apptask.h"
#include "queue.h"
#include "semphr.h" 
#include "driver_uart.h"
#include "driver_adc.h"
#include "sky72310.h"


extern xQueueHandle Sem_Adc_Deal_A;//Adc 数据处理信号量---10ms执行一次
extern xQueueHandle Sem_Adc_Deal_B;//Adc 数据处理信号量---10ms执行一次

extern xQueueHandle Sem_Adc_Deal_A_End;//Adc 数据处理信号量---10ms执行一次
extern xQueueHandle Sem_Adc_Deal_B_End;//Adc 数据处理信号量---10ms执行一次
extern xQueueHandle Sem_FGU_INI_Deal;//Adc 数据处理信号量---10ms执行一次

extern void RTU_Uart0Dev_Init(void);
extern void RTU_AdcDev_Init(void);
extern void RTU_I2CDev_Init(void);
extern void RTU_SpiDev_Init(void);
//extern uint32_t buff_A[NUM_SAMPLES_A][NUM_CHANNELS_A]; /* Memory to hold samples for SEQ_A */
void vUart_Server_Task(void *pvParameters);



void rt_kprintf_task_stack()
{
	UBaseType_t uxHighWaterMark;    
	
	/* 入口处检测一次 */  
	uxHighWaterMark =uxTaskGetStackHighWaterMark( NULL );
	rt_kprintf(1,"TASK--MAX unused stack[%d]\r\n",uxHighWaterMark);
}

void rt_kprintf_task_list()
{
	char *pframebuf=(char *) malloc(500);
		
#if ( configUSE_STATS_FORMATTING_FUNCTIONS == 1 )
	vTaskList(pframebuf);
	rt_kprintf(1,"Name		state	priority	stack	num\r\n");
	rt_kprintf(1,"************************************************************************\r\n");
	rt_kprintf(1,"%s \r\n",pframebuf);
#endif

#if ( configGENERATE_RUN_TIME_STATS == 1 )
	vTaskGetRunTimeStats(pframebuf);
	rt_kprintf(1,"Name		counter	used\r\n");
	rt_kprintf(1,"************************************************************************\r\n");
	rt_kprintf(1,"%s \r\n",pframebuf);
#endif
	free(pframebuf);

}
//??????
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
	else
	{
		return FALSE;
	}
}

APP_ADC_DEAL_BUFF * Get_app_adc_buf()
{
	return &app_adc_buf;
}

uint32_t Get_app_adc_cur_ticks(APP_ADC_DEAL_BUFF * buf)
{
	return buf->Cur_ticks;
}

uint32_t Set_app_adc_cur_ticks(APP_ADC_DEAL_BUFF * buf,uint32_t tick)
{
	 buf->Cur_ticks=tick;
}

uint32_t Set_send_delay0_ticks(APP_ADC_DEAL_BUFF * buf,uint32_t tick)
{
	 buf->Send_delay_timer0=tick;
}

uint32_t Get_send_delay0_ticks(APP_ADC_DEAL_BUFF * buf)
{
	return buf->Send_delay_timer0;
}


uint32_t Set_send_delay1_ticks(APP_ADC_DEAL_BUFF * buf,uint32_t tick)
{
	 buf->Send_delay_timer1=tick;
}

uint32_t Get_send_delay1_ticks(APP_ADC_DEAL_BUFF * buf)
{
	return buf->Send_delay_timer1;
}

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/



/* Print content of buffer_A to UART */
 void Data_buf_A_Deal(uint32_t *buff,uint32_t cnt,uint32_t chnum,uint32_t *appbuff)
{
	int i, j;
//	uint32_t(*buf)[chnum] = (uint32_t*)buff;
//	uint32_t(*appbuf)[chnum] = (uint32_t*)appbuff;

//	for (i = 0; i < cnt; i++)
//	{
//		//DEBUGSTR("SEQ_A: ");
//		for (j = 0; j < chnum; j++) 
//		{
//			appbuf[i][j]=buf[i][j];
//			//DEBUGOUT("%03X ", buf[i][j]);
//		}
//		//DEBUGSTR("\r\n");
//	}
	memcpy(appbuff,buff,cnt*chnum);
}


/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

//计算adc数值
void calculate_ADC_value()
{
#ifdef MALLOC_PING_PONG
	RTU_PING_PONG_DATA_P * adc_ping_pong = (RTU_PING_PONG_DATA_P *)Get_ADC_ping_pong_buf();
	RTU_PING_PONG_DATA_P * spi0_txmod_ping_pong = (RTU_PING_PONG_DATA_P *)Get_spi0_ping_pong_buf();
	RTU_PING_PONG_SPI1_DATA_P * spi1_dac_ping_pong = (RTU_PING_PONG_SPI1_DATA_P *)Get_spi1_ping_pong_buf();
	uint32_t i,adc_flg,spi_flg;

	adc_flg = ((*adc_ping_pong)->ping_pong_flg+1) % RTU_PING_PONG_BUF_CH_NUM;
	spi_flg = ((*spi0_txmod_ping_pong)->ping_pong_flg++) % RTU_PING_PONG_BUF_CH_NUM;

	for(i=0;i<RTU_PING_PONG_BUF_MAX_NUM;i++)
	{
		(*spi0_txmod_ping_pong)->ping_pong_buf[spi_flg].ping_pong_buf_1K[i] =
			((*adc_ping_pong)->ping_pong_buf[adc_flg].ping_pong_buf_1K[i])>>4;

		(*spi1_dac_ping_pong)->ping_pong_buf1[spi_flg].ping_pong_buf_1K[i] =
			((*adc_ping_pong)->ping_pong_buf[adc_flg].ping_pong_buf_1K[i])>>4;
	}
#else

	RTU_PING_PONG_DATA_T * adc_ping_pong = (RTU_PING_PONG_DATA_T *)Get_ADC_ping_pong_buf();
	RTU_PING_PONG_DATA_T * spi0_txmod_ping_pong = (RTU_PING_PONG_DATA_T *)Get_spi0_ping_pong_buf();
	RTU_PING_PONG_SPI1_DATA_T * spi1_dac_ping_pong = (RTU_PING_PONG_SPI1_DATA_T *)Get_spi1_ping_pong_buf();
	uint32_t i,adc_flg,spi_flg;
	int32_t tmpvalue;

	adc_flg = ((adc_ping_pong)->ping_pong_flg+1) % RTU_PING_PONG_BUF_CH_NUM;
	spi_flg = ((spi0_txmod_ping_pong)->ping_pong_flg++) % RTU_PING_PONG_BUF_CH_NUM;
//	spi_flg = ((spi1_dac_ping_pong)->pp_data.ping_pong_flg++) % RTU_PING_PONG_BUF_CH_NUM;
//	rt_kprintf(1,"calculate:adc_flg[%d]--spi_flg{%d]\r\n",adc_flg,spi_flg);

//	tmpvalue = ((adc_ping_pong)->ping_pong_buf[adc_flg].ping_pong_buf_1K[i])>>4;
	for(i=0;i<RTU_PING_PONG_BUF_MAX_NUM;i++)
	{
		(spi0_txmod_ping_pong)->ping_pong_buf[spi_flg].ping_pong_buf_1K[i] =
			(( ((adc_ping_pong)->ping_pong_buf[adc_flg].ping_pong_buf_1K[i])>>4)*Rtu_Get_tx_mod_K() /10) | (0x9000) ;

		(spi1_dac_ping_pong)->ping_pong_buf[spi_flg].ping_pong_buf_4K[4*i+RTU_SPI1_DAC_DMA_CH_B] = 
			(( ((adc_ping_pong)->ping_pong_buf[adc_flg].ping_pong_buf_1K[i])>>4)*Rtu_Get_spi_dac_K() /10) |(RTU_SPI1_DAC_DMA_CH_B <<14);
	}

	
//	restart_dma_spi_for_adc_calc(spi_flg); //判断是否进行dma重启
#endif
	
}

/*****************************************************************************
 * 函 数 名  : vAdc_Sequence_A_Data_Task
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月2日
 * 函数功能  : ADC通道8，即 序列A 的数据处理，等待采集数据信号量SEm_Adc_Deal_A
               ,,然后根据延时，启动定时器1，分别把数据发送到两路DAC上。
 * 输入参数  : void *pvParameters  任务传入参数
 * 输出参数  : 无
 * 返 回 值  : static
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
static void vAdc_Sequence_A_Data_Task(void *pvParameters) {
	bool LedState = false;

	uint32_t ticket;
	uint16_t tmpdata=0;
	portBASE_TYPE ret;
	TRC_COM_P pcom_data;

	/*
		等待接收pingpangbuf满的信号量，对数据进行处理，
		并拷贝到txpll和spidac的发送缓冲区中
	*/
	while (1) 
	{
		ret=xSemaphoreTake(Sem_Adc_Deal_A,0);
		if(ret==pdPASS)
		{					
			calculate_ADC_value();//计算 adc value，并拷贝到txpll和spidac的sendbuf
		}
		else
		{
			//DEBUGINFO();
		}
		vTaskDelay(configTICK_RATE_HZ / 1000);
	}
}

/*****************************************************************************
 * 函 数 名  : vAdc_Sequence_B_Data_Task
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月2日
 * 函数功能  : 序列B的数据处理任务
 * 输入参数  : void *pvParameters  任务传入参数
 * 输出参数  : 无
 * 返 回 值  : static
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
static void vAdc_Sequence_B_Data_Task(void *pvParameters) {
	bool LedState = false;
	uint32_t i,j,tmpvalue;
	uint32_t ticket;
	portBASE_TYPE ret;
	RTU_PARAM_NOT_SAVE_ADVALUE * rtu_advalue = Get_RTU_dev_ptr_Adc_value();

	ticket = xTaskGetTickCount();
	while (1) 
	{
		ret=xSemaphoreTake(Sem_Adc_Deal_B,5);
		if(ret==pdPASS)
		{					
			//rt_kprintf(DEBUG_SYS,"ADC_TASK_B \r\n");
			//Data_buf_B_Deal(&buff_B[0][0],NUM_SAMPLES_B,NUM_CHANNELS_B,&app_adc_buf.app_buff_B[0][0]); //保存数据
			for(i=0;i<NUM_CHANNELS_B;i++)
			{
				tmpvalue=0;
				for(j=0;j<NUM_SAMPLES_B;j++)
				{
					tmpvalue += buff_B[i][j];
				}
				tmpvalue /=NUM_SAMPLES_B;
				rtu_advalue->Advalue[i].w = (tmpvalue&0xFFFF);

			//	rt_kprintf(DEBUG_SYS,"buff[%d] = [%d][%x]\r\n",i,tmpvalue,tmpvalue);
				
			}
			rtu_advalue->converter ++;//转换次数自加1
			rt_send_adc_value_New(); //计算 各种值
//			xSemaphoreGive(Sem_Adc_Deal_B_End);
		}
		else
		{
				
		}
		taskYIELD();//允许其它发送任务执行。 taskYIELD()通知调度器现在就切换到其它任务，而不必等到本任务的时间片耗尽
		
		
		//vTaskDelay(configTICK_RATE_HZ / 6);
	}
}


/* LED1 toggle thread */
//static void vADC_Buff_Send(void *pvParameters) {
//	bool LedState = false;

//	uint32_t ticket,maxtick,savetick;
//	portBASE_TYPE ret;
//	APP_ADC_DEAL_BUFF * dataopt=Get_app_adc_buf();

//	
//	ticket = xTaskGetTickCount();

//	while (1) 
//	{
//		ret=xSemaphoreTake(Sem_Adc_Deal_A_End,0);
//		if(ret==pdPASS)
//		{					
//			//Data_buf_A_Deal(buff_A,NUM_SAMPLES_A,NUM_CHANNELS_A,app_adc_buf.app_buff_A); //保存数据
//			maxtick=MAX(Get_send_delay0_ticks(dataopt),Get_send_delay1_ticks(dataopt));
//			savetick=Get_app_adc_cur_ticks(dataopt);
//			ticket = xTaskGetTickCount();//获得当前tick
//			while(maxtick+savetick <ticket)
//			{
//				if(savetick+Get_send_delay0_ticks(dataopt)<ticket)
//				{
//				//send data to 0
//				}

//				if(savetick+Get_send_delay1_ticks(dataopt)<ticket)
//				{
//				//send data to 1
//				}
//			}
//			
//			//xSemaphoreGive(Sem_Adc_Deal_A_End);
//		}
//		else
//		{
//				
//		}
//		taskYIELD();//允许其它发送任务执行。 taskYIELD()通知调度器现在就切换到其它任务，而不必等到本任务的时间片耗尽
//		
//		
//		//vTaskDelay(configTICK_RATE_HZ / 6);
//	}
//}

/*****************************************************************************
 * 函 数 名  : vAdc_Sequence_A_Data_Task
 * 负 责 人  : MZM
 * 创建日期  : 2016年3月2日
 * 函数功能  : ADC通道8，即 序列A 的数据处理，等待采集数据信号量SEm_Adc_Deal_A
               ,,然后根据延时，启动定时器1，分别把数据发送到两路DAC上。
 * 输入参数  : void *pvParameters  任务传入参数
 * 输出参数  : 无
 * 返 回 值  : static
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
static void vUart_Server_Task1(void *pvParameters) {
	bool LedState = false;

	
	uint32_t recvcnt;
	uint16_t tmpdata;
	portBASE_TYPE ret;
	RTU_UART_DEV_T *uart_dev = RTU_Get_Uart_Dev();
	
	while (1) 
	{
		ret=xSemaphoreTake(uart_dev->Sem_uart_recv_data,10);
		if(ret==pdPASS)
		{					
			recvcnt = RTU_Uart0_Read(&uart_dev->rx_buf[uart_dev->savecnt%RB_COUNT], RB_COUNT);
			uart_dev->savecnt += recvcnt;
			//协议解析函数、数据处理接口
			//协议解析函数、数据处理接口
			//协议解析函数、数据处理接口
			//协议解析函数、数据处理接口
		}
		else
		{
			//DEBUGINFO();
		}
		taskYIELD();//允许其它发送任务执行。 taskYIELD()通知调度器现在就切换到其它任务，而不必等到本任务的时间片耗尽
		
	}
}

void test_flash()
{
	VTR_FLAGS flag_first,flag_last;
	portBASE_TYPE  ret;
	Rtu_Read_flag_from_flash(IAP_FIRST_SECTOR,&flag_first,sizeof(VTR_FLAGS));//读第14个扇区的标志
	Rtu_Read_flag_from_flash(IAP_LAST_SECTOR,&flag_last,sizeof(VTR_FLAGS));//读第15个扇区的标志
	
	rt_kprintf(1,"flag_first-save[%d] valid[%d]\r\n",flag_first.save_flag,flag_first.valid_flag);
	rt_kprintf(1,"flag_last-save[%d] valid[%d]\r\n",flag_last.save_flag,flag_last.valid_flag);

	rt_kprintf(1,"txmod_delay[%d] tx_mod_k[%d]\r\n",rtudev->saveparam.sys_param_dac_mod.tx_mod_delay,rtudev->saveparam.sys_param_dac_mod.tx_mod_k);
	

}

void app_test_cpu_idle_add()
{
	app_test_cpu++;
}

void app_test_cpu_idle_zero()
{
	app_test_cpu = 0;
}

/*****************************************************************************
 * 函 数 名  : task_before_start_adc_seq_B
 * 负 责 人  : MZM
 * 创建日期  : 2016年6月15日
 * 函数功能  : 读取序列B AD值之前，进行的相关操作。设置cv读取通道等
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void  task_before_start_adc_seq_B()
{
	VTR_SYS_PARAM_EXT_CLK * ext_clk_ptr = (VTR_SYS_PARAM_EXT_CLK *)Get_RTU_dev_ptr_Ext_clk();
	
	if(ext_clk_ptr->refcv_valid_flg == REF_CV_VALID_FLG1234) //refcv-value 有效?
	{
		return ;	//有效 不再进行读取refcv。
	}
	else
	{
		if(ext_clk_ptr->ext_clk_state == ENABLE_EXT_CLK ) //refcv_value 无效，并且使用外部晶振10M
		{
			RTU_Set_CV_Read_REF_CV();// 设置读取外部时钟
		}
//		else if()
//			{
//			
//			}
	}

	return;
}

/*****************************************************************************
 * 函 数 名  : task_for_pre_10ms_func
 * 负 责 人  : MZM
 * 创建日期  : 2016年6月15日
 * 函数功能  : 每10ms读取一次 序列B的ad值
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void task_for_pre_10ms_func()
{	
#if 0	
	task_before_start_adc_seq_B();
	Rtu_Start_ADC_B_Conversion();// 10 ms do it
#endif
}

void test_printf_pll_ld()
{
	RTU_PCA9535_PORT_STRUCT * rtu_pll_ld_in = (RTU_PCA9535_PORT_STRUCT *) Get_RTU_dev_ptr_PCA9535_Inputstate();
	
	rt_kprintf(0,"--TX[%d],RX[%d],LO2[%d],REF[%d]\r\n",rtu_pll_ld_in->tx_LD,rtu_pll_ld_in->rx_LD,rtu_pll_ld_in->rx_LD2,rtu_pll_ld_in->ref_LD);
}

void test_printf_temp()
{
	rt_kprintf(DEBUG_SYS,"tempID:       [%x] tempdeviceID: [%x]\r\n",RTU_GET_TX_ID(),RTU_GET_TX_DEVICE_ID());
	rt_kprintf(DEBUG_SYS,"tempTX:       [%d] \r\n", RTU_Get_TX_Temp());
	rt_kprintf(DEBUG_SYS,"tempPA:       [%d] \r\n",RTU_Get_PA_Temp());
}

void test_param_save()
{
	rt_kprintf(DEBUG_SYS,"TXFREQ:	[%d] \r\n", Rtu_Get_tx_freq());
	rt_kprintf(DEBUG_SYS,"RXFREQ:	[%d] \r\n",Rtu_Get_rx_freq());
	rt_kprintf(DEBUG_SYS,"DELAY:	[%d] \r\n",Rtu_Get_tx_mod_delay());
	rt_kprintf(DEBUG_SYS,"TXMOD:	[%d] \r\n",Rtu_Get_tx_mod_K());
	rt_kprintf(DEBUG_SYS,"SPIDA:	[%d] \r\n",Rtu_Get_spi_dac_K());
		
}

//void test_stack_addr(uint32_t test_stack1,uint32_t test_stack2,uint32_t test_stack3,uint32_t test_stack4)
void test_stack_addr()
{
	uint32_t test_stack1=1;
	uint32_t test_stack2=2;
	uint32_t test_stack3=3;
	uint32_t test_stack4=4;
	uint32_t test_stack5=5;
	rt_kprintf(DEBUG_SYS,"1:	[%x] \r\n",&test_stack1);
	rt_kprintf(DEBUG_SYS,"2:	[%x] \r\n",&test_stack2);
	rt_kprintf(DEBUG_SYS,"3:	[%x] \r\n",&test_stack3);
	rt_kprintf(DEBUG_SYS,"4:	[%x] \r\n",&test_stack4);
	rt_kprintf(DEBUG_SYS,"5:	[%x] \r\n",&test_stack5);
	
}
void rt_kprintf_test_info()
{

//	rt_kprintf_task_list();
//	test_printf_temp();
//	test_printf_pll_ld();	
	test_param_save();
//	 test_stack_addr( test_stack1, test_stack2, test_stack3, test_stack4);
//	test_stack_addr( );


}

void v10msTask(void *pvParameters) 
{
	uint8_t i=0,j=0;
	uint32_t lasttick,tmpcnt=0,tmptemptx,tmptemppa,tmptempid;
	uint32_t flg10ms,flg500ms,flg1000ms,flg2500ms,flg5000ms;
	TRC_COM_P pcom_data;

	lasttick=xTaskGetTickCount();
	flg10ms=0;

//	rt_kprintf_task_stack();

//	rt_kprintf(DEBUG_SYS,"FUNCTION:	[%s()]-	Tick[%d] \r\n",__FUNCTION__,lasttick);
	while (1)
	{
		if(xTaskGetTickCount()-lasttick >= 100)
		{
		
//		rt_kprintf(DEBUG_SYS,"idletimes[%d],cpu[%d]\r\n",app_test_cpu,app_test_cpu/100);
			flg10ms++;
			lasttick=xTaskGetTickCount();
		
			#ifdef TESTFORJJJ
//			/*ADC3CH:读取cv-adc的通道选择，0-2--tx/rx/refcv*/
//				pcom_data=(TRC_COM_P)Get_TRC_Com_Dev(0);

//				if(pcom_data->test_data.flag &0x1<<1)
//				{
//					 RTU_Set_ADC_Read_CH((uint8_t)pcom_data->test_data.ad_3ch&0x7);
//				}
				
			#endif
			//rt_kprintf(DEBUG_SYS,"sssssS \r\n");
			flg500ms =(flg10ms %5==0)?1:0;
			flg1000ms =(flg10ms %10==0)?1:0;
			flg5000ms =(flg10ms %30==0)?1:0;
// 							SPI_Send_One_Data(0,1,0x5555);
//				SPI_Send_One_Data(1,0,0x555);
//				SPI_Send_One_Data(1,1,0);
//				SPI_Send_One_Data(1,2,0x555);
//				SPI_Send_One_Data(1,3,0x555);
			if(flg1000ms)
			{
				//do_1000ms_task();
				tmpcnt++;
				
//				TX_ENABLE(tmpcnt%2);
//				RTU_NB_WB(tmpcnt%2);
				rt_kprintf(DEBUG_SYS,"1S-TASK: [%d]S [%x]\r\n",tmpcnt,buff_A1[0]);
//				{
//					i = tmpcnt%2;
//					for(j=0;j<3;j++)
//					{
//						rt_kprintf(DEBUG_SYS,"[%d]: addr[%d]    reg[%x] [%d]\r\n",i,(tmpreg_72310[i][j]>>12),tmpreg_72310[i][j],tmpreg_72310[i][j]&0xFFF);
//						
//					}
//				}

//				rt_kprintf(DEBUG_SYS,"idletimes[%d],cpu[%d]\r\n",app_test_cpu,app_test_cpu/100);
//				#define JJJHZRX (370000000+73350000)
//				#define JJJHZTX (420000000)
//					Set_PLL_Freq(LO2_PLL_SELECT,LO2_PLL_FREQ,REF_PLL_FREQ,	0,0,3,0,0x0f,0,0);//L02电压 73.35M-455K
//					
////					Set_PLL_Freq(REF_PLL_SELECT,REF_PLL_FREQ,REF_PLL_FREQ,	0,0,3,0,0x0f,0,0);//参考电压
//				Set_PLL_Freq(TX_PLL_SELECT,JJJHZTX,REF_PLL_FREQ,0,0,3,0,0x0f,0,0);
//				Set_PLL_Freq(RX_PLL_SELECT,JJJHZRX,REF_PLL_FREQ,0,0,3,0,0x0f,0,0);
				
				
//				test_Pwm();
//				SPI_Send_One_Data(0,1,0x5555);
//				SPI_Send_One_Data(1,0,0x5555);
//				SPI_Send_One_Data(1,1,2048);
//				SPI_Send_One_Data(1,2,0x5555);
//				SPI_Send_One_Data(1,3,0x5555);
//				dmacnt_printf();
//			
//				Board_LED_Toggle(1);

//				SPI_Send_One_Data(1,1,0x55A);
//				Board_LED_Toggle(1);


//				testprintf(0);
//				testprintf(1);
//				SPI_Send_One_Data(1,1,0x55A);

				flg1000ms=0;

				
			}
			if(flg5000ms)
			{
				//do_1000ms_task();
				flg5000ms=0;


				rt_kprintf_test_info();
//				Set_PLL_Freq(TX_PLL_SELECT,JJJHZTX,REF_PLL_FREQ,0,0,3,0,0x0f,0,0);
//				Set_PLL_Freq(RX_PLL_SELECT,JJJHZRX,REF_PLL_FREQ,0,0,3,0,0x0f,0,0);
//				tmptempid = RTU_GET_TX_ID();

//				tmptemptx = RTU_Get_TX_Temp();
//				tmptemppa = RTU_Get_PA_Temp();
				
//				rt_kprintf(DEBUG_SYS,"tempID:       [%x] \r\n",tmptempid);
//				rt_kprintf(DEBUG_SYS,"tempdeviceID: [%x] \r\n",RTU_GET_TX_DEVICE_ID());
//				rt_kprintf(DEBUG_SYS,"tempTX:       [%d] \r\n",tmptemptx);
//				rt_kprintf(DEBUG_SYS,"tempPA:       [%d] \r\n",tmptemppa);
//				testprintf();
//				SPI_Send_One_Data(1,1,0x55A);
//				Set_PLL_Freq(TX_PLL_SELECT,JJJHZTX,REF_PLL_FREQ,0,0,3,0,0x0f,0,0);
//				test_cpu_flash();

//				test_flash();
//				i++;
//				if(i%2==0)
//					{
//				stop_dma_spi_send();
//					}
//				else{
//					
//					SPI_DMA_App_Send_Start(0);	//启动spi0--并不启动dma发送
//					SPI_DMA_App_Send_Start(1);	//启动spi1--启动dma发送
//					}
//				rt_kprintf(DEBUG_SYS,"Chip_Clock_GetMainClockSource: [%d]\r\n",Chip_Clock_GetMainClockSource());
//				rt_kprintf(DEBUG_SYS,"Chip_Clock_GetSystemClockRate: [%d]\r\n",Chip_Clock_GetSystemClockRate());

//				
//				rt_kprintf(DEBUG_SYS,"Chip_Clock_GetMain_A_ClockRate: [%d]\r\n",Chip_Clock_GetMain_A_ClockRate());
//				rt_kprintf(DEBUG_SYS,"Chip_Clock_GetMain_B_ClockRate: [%d]\r\n",Chip_Clock_GetMain_B_ClockRate());
			}
		}

//		taskYIELD();//允许其它发送任务执行。 taskYIELD()通知调度器现在就切换到其它任务，而不必等到本任务的时间片耗尽

//		if(tmpcnt == 1)
//			{
//			app_test_cpu_idle_zero();
//			}
//		tmpcnt=0;

		task_for_pre_10ms_func();

		taskYIELD();
		vTaskDelay(configTICK_RATE_HZ/1000);
	}
}


void vCheck_PLL_LD_Task(void *pvParameters) 
{
	portBASE_TYPE ret;
	//uint8_t pll_ld;
	//RTU_DEV_T * rtudev = RTU_DEV_Get();
	uint32_t ticket,lasttick;
	//	ticket = xTaskGetTickCount();
	RTU_PCA9535_PORT_STRUCT * rtu_pll_ld_in = (RTU_PCA9535_PORT_STRUCT *) Get_RTU_dev_ptr_PCA9535_Inputstate();
	
	RTU_PCA9535_PORT_STRUCT  rtu_pll_ld; 
	RTU_PCA9535_PORT_STRUCT  rtu_pll_ld_out ;
	
	RTU_Get_Port_Input(rtu_pll_ld_in);//读取 pll_ld
	lasttick = xTaskGetTickCount();

	while (1) 
	{
		ret=xSemaphoreTake(Sem_FGU_INI_Deal,5);
		if(ret==pdPASS)
		{					
//			rt_kprintf(DEBUG_FOR_CHECK_PLL,"FGU_INIT\r\n");
//			
//			rt_kprintf(DEBUG_FOR_CHECK_PLL,"Read-before[%d][%x]\r\n",*((uint16*)rtu_pll_ld),*((uint16*)rtu_pll_ld));
			RTU_Get_Port_Input(rtu_pll_ld_in);//读取 pll_ld

			
			RTU_Get_Port_Input(&rtu_pll_ld);//读取 pll_ld
			RTU_Get_Port_Output(&rtu_pll_ld_out);
			
			lasttick = xTaskGetTickCount();
//			rt_kprintf(DEBUG_FOR_CHECK_PLL,"After--Read[%d][%x]\r\n",*((uint16*)rtu_pll_ld),*((uint16*)rtu_pll_ld));
			
		}
		else
		{
			//DEBUGINFO();
			ticket = xTaskGetTickCount();
			if(ticket > (lasttick + 2000) || (Rtu_check_FGU_int() == 0))
			{
			
//				rt_kprintf_task_stack();
				RTU_Get_Port_Input(rtu_pll_ld_in);//读取 pll_ld
				RTU_Get_Port_Input(&rtu_pll_ld);//读取 pll_ld
	 			RTU_Get_Port_Output(&rtu_pll_ld_out);
				lasttick = ticket;
				if(rtu_pll_ld_in->tx_LD == 0)
				{
//					Set_PLL_Freq(TX_PLL_SELECT,JJJHZTX,REF_PLL_FREQ,0,0,3,0,0x0f,0,0);
				}
				rt_kprintf(0,"TX[%d],RX[%d],LO2[%d],REF[%d]\r\n",rtu_pll_ld_in->tx_LD,rtu_pll_ld_in->rx_LD,rtu_pll_ld_in->rx_LD2,rtu_pll_ld_in->ref_LD);
			}
		}
//		taskYIELD();//允许其它发送任务执行。 taskYIELD()通知调度器现在就切换到其它任务，而不必等到本任务的时间片耗尽
		vTaskDelay(configTICK_RATE_HZ/100); 

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
void Task_Create_ADC_A(uint16_t StackDepth, uint8_t Priority)
{
	portBASE_TYPE ret;
	
		/* 创建ADC0的独立转换序列A 的数据处理任务 */
	ret = xTaskCreate(vAdc_Sequence_A_Data_Task, "vAdcATask",
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

void Task_Create_ADC_B(uint16_t StackDepth, uint8_t Priority)
{
	portBASE_TYPE ret;
	
		/* 创建ADC0的独立转换序列A 的数据处理任务 */
	ret = xTaskCreate(vAdc_Sequence_B_Data_Task, "vAdcBTask",
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
void Task_Create_Uart0_Server(uint16_t StackDepth, uint8_t Priority)
{
	portBASE_TYPE ret;
	
		/* 创建ADC0的独立转换序列A 的数据处理任务 */
	ret = xTaskCreate(vUart_Server_Task, "vUSTask",
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

void Task_Create_Check_PLL_LD(uint16_t StackDepth, uint8_t Priority)
{
	portBASE_TYPE ret;
	
		/* 创建ADC0的独立转换序列A 的数据处理任务 */
	ret = xTaskCreate(vCheck_PLL_LD_Task, "vLD_Task",
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

void vUart_Protocol_Task(void *pvParameters) 
{
	uint32 bytes,num=0;
	uint16 i,j;
	//uint8 tmpbuf[TRC_COM_MAX_RX_NUM];

	TRC_COM_P pcom_data;
	LPC_USART_T *pUART;
	portBASE_TYPE  ret;

	
	VTR_PROTOCOL_DATA *pframebuf=(VTR_PROTOCOL_DATA *) malloc(sizeof(VTR_PROTOCOL_DATA));
//	rt_kprintf(DEBUG_SYS,"FUNCTION:	[%s()]-	Tick[%d] \r\n",__FUNCTION__,xTaskGetTickCount());
	//VTR_PROTOCOL_DATA *pframebuf=(VTR_PROTOCOL_DATA *) malloc(sizeof(VTR_PROTOCOL_DATA));
	if(pframebuf==NULL)
			{
//			rt_kprintf(DEBUG_SYS,"vprotocolTask malloc[tmpbuf]	error!!\r\n");
			}
		else{
			
//			rt_kprintf(DEBUG_SYS,"vprotocolTask malloc[tmpbuf]	SUCESS!!\r\n");
			}
	while(1)
	{
		for(i=0;i<MAX_COM_PORT;i++)
		{
			pcom_data=(TRC_COM_P)Get_TRC_Com_Dev(i);
				if(pcom_data==NULL)
				{
				
				vTaskDelay(configTICK_RATE_HZ/3);
				continue;
				}
		//	pUART=(LPC_USART_T *)Get_TRC_Com_Dev_Uart(i);
			
			ret=xSemaphoreTake(pcom_data->mutex_sem_write_ok,portTICK_RATE_MS*5);
			if(ret==pdFALSE)
			{
				//vTaskDelay(configTICK_RATE_HZ/500);
				continue;
			}
			
			rt_kprintf(DEBUG_TASK_UART,"5 write_ok---Poto-Take-Sucess\r\n");
			bytes=RingBuffer_PopMult(&pcom_data->rx_frame_buf,pframebuf->rxd_buf,TRC_COM_MAX_RX_NUM);
			
			rt_kprintf(DEBUG_TASK_UART,"6 read_ok---Poto-Give-Sucess\r\n");
			xSemaphoreGive(pcom_data->mutex_sem_read_ok);
			if(bytes>0)
				{
				//vTaskDelay(configTICK_RATE_HZ/50);
				num++;
				rt_kprintf(DEBUG_TASK_UART,"\4\nNUM[%d]",num);
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
						Vtr_Fail_Return_Respond(pframebuf,ret);//组织确认否认帧并发送
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

void Task_Create_Com_Protocol(uint16 StackDepth, uint8 Priority)
{
	portBASE_TYPE ret;

	ret=xTaskCreate(vUart_Protocol_Task, (signed char *) "vUProTask",
						StackDepth, NULL, (tskIDLE_PRIORITY + Priority),
						(xTaskHandle *) NULL);
//		if(ret==pdTRUE)
//		{
//			rt_kprintf(DEBUG_SYS,"Create Task Sucess:--vprotocolTask\r\n");
//		}
//		else
//		{
//			rt_kprintf(DEBUG_SYS,"Create Task FAIL:--vprotocolTask\r\n");
//		}

		
}


////DEBUGOUT
//　//RTU_UART_DEV_T *uart_dev = RTU_Get_Uart_Dev();
//	
//	while (1) 
//	{
//		ret=xSemaphoreTake(uart_dev->Sem_uart_recv_data,10);
//		if(ret==pdPASS)
//		{					
//			recvcnt = RTU_Uart0_Read(&uart_dev->rx_buf[uart_dev->savecnt%RB_COUNT], RB_COUNT);
//			uart_dev->savecnt += recvcnt;

void check_user_cmd(TRC_COM_P pcom_data,uint8_t *tmpbuf,uint32_t len)
{
	uint32_t j;
	uint8_t tmpflg = 0xFF;

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
				else
				{
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
				Rtu_Set_tx_mod_delay(atoi(&tmpbuf[j+6]));
				stop_dma_spi_for_adc_calc(); //停止dma搬数
				
				tmpflg = SYS_PARAM_DAC_MOD;				
				RTU_Save_SYS_Param_By_Param_Flg(tmpflg);
			}
		else 
		if( tmpbuf[j+0]=='T'&&
			tmpbuf[j+1]=='X'&&
			tmpbuf[j+2]=='M'&&
			tmpbuf[j+3]=='O'&&
			tmpbuf[j+4]=='D'&&
			tmpbuf[j+5]==':')
			{
				tmpflg = SYS_PARAM_DAC_MOD;
				
				
				RTU_Save_SYS_Param_By_Param_Flg(tmpflg);
				Rtu_Set_tx_mod_K( atoi(&tmpbuf[j+6]))  ;
				
			}
		else 
		if( tmpbuf[j+0]=='S'&&
			tmpbuf[j+1]=='P'&&
			tmpbuf[j+2]=='I'&&
			tmpbuf[j+3]=='D'&&
			tmpbuf[j+4]=='A'&&
			tmpbuf[j+5]==':')
			{
				tmpflg = SYS_PARAM_DAC_MOD;
				Rtu_Set_spi_dac_K( atoi(&tmpbuf[j+6])) ;
				
				RTU_Save_SYS_Param_By_Param_Flg(tmpflg);
			}
		else 
			if( tmpbuf[j+0]=='T'&&
				tmpbuf[j+1]=='X'&&
				tmpbuf[j+2]=='F'&&
				tmpbuf[j+3]=='R'&&
				tmpbuf[j+4]=='E'&&
				tmpbuf[j+5]=='Q'&&
				tmpbuf[j+6]==':')
				{
					tmpflg = SYS_PARAM_FREQ;
					Rtu_Set_tx_freq(atoi(&tmpbuf[j+7]));
					Set_PLL_Freq(TX_PLL_SELECT,Rtu_Get_tx_freq(),REF_PLL_FREQ,0,0,3,0,0x0f,0,0);
					
					RTU_Save_SYS_Param_By_Param_Flg(tmpflg);
				}

			else 
			if( tmpbuf[j+0]=='R'&&
				tmpbuf[j+1]=='X'&&
				tmpbuf[j+2]=='F'&&
				tmpbuf[j+3]=='R'&&
				tmpbuf[j+4]=='E'&&
				tmpbuf[j+5]=='Q'&&
				tmpbuf[j+6]==':')
				{
					tmpflg = SYS_PARAM_FREQ;
					Rtu_Set_rx_freq(atoi(&tmpbuf[j+7]));
					Set_PLL_Freq(RX_PLL_SELECT,Rtu_Get_rx_freq(),REF_PLL_FREQ,0,0,3,0,0x0f,0,0);
					
					RTU_Save_SYS_Param_By_Param_Flg(tmpflg);
				}

			if(0xFF != tmpflg)
			{
//				RTU_Save_Param_to_Flash();
//				RTU_Save_SYS_Param_By_Param_Flg(tmpflg);
//				rt_kprintf(1,"RECV:[%s]\r\n",tmpbuf);				
			}
#endif
	}

}
void vUart_Server_Task(void *pvParameters) 
{
	uint32 bytes;
	uint16 i,j,tmpflg;
	uint8 *tmpbuf,*readbufhead;

	TRC_COM_P pcom_data;
	LPC_USART_T *pUART;
	
	portBASE_TYPE  ret;

//	rt_kprintf(DEBUG_SYS,"FUNCTION:	[%s()]-	Tick[%d] \r\n",__FUNCTION__,xTaskGetTickCount());
	
	tmpbuf=(uint8*) pvPortMalloc(APP_READ_NUM);

	if(tmpbuf==NULL)
	{
//		rt_kprintf(DEBUG_SYS,"vuartserverTask malloc[tmpbuf]  error!!\r\n");
	}
	else
	{		
//		rt_kprintf(DEBUG_SYS,"vuartserverTask malloc[tmpbuf]  SUCESS!!\r\n");
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
				vTaskDelay(configTICK_RATE_HZ/300);
				continue;
			}
			
			ret = xSemaphoreTake(pcom_data->driver_sem_receive_data,5);
			if(ret != pdPASS)
			{				
//				vTaskDelay(configTICK_RATE_HZ/30);
				continue;
			}
		
			bytes = RTU_Uart0_Read(tmpbuf, APP_READ_NUM);
			if (bytes > 0) 
			{
			
				check_user_cmd(pcom_data,tmpbuf,bytes);
//				rt_kprintf(DEBUG_TASK_UART,"RECV--Num[%d] buf[%s]\r\n",bytes,tmpbuf);
				for(j=0;j<bytes;j++)
				{

					
//					check_user_cmd(pcom_data,tmpbuf+j,bytes-j);
					if(tmpbuf[j]==STX)//判断 起始标志
					{
						rt_kprintf(DEBUG_TASK_UART,"1 read_ok---us-take-start\r\n");
						ret=xSemaphoreTake(pcom_data->mutex_sem_read_ok,portTICK_RATE_MS*10);
						if(ret==pdFALSE)
						{
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
			else{
				RTU_Uart0_Read_Over();
				}
			
		}
			
		vTaskDelay(configTICK_RATE_HZ/100);
		
	}
}

/*****************************************************************************
 * 函 数 名  : RTU_check_system_param
 * 负 责 人  : MZM
 * 创建日期  : 2016年6月6日
 * 函数功能  : 检查系统参数的有效性
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void RTU_check_system_param()
{
	uint8_t saveflg = FALSE;
	uint16_t delay = (rtudev->saveparam.sys_param_dac_mod.tx_mod_delay);
	
//	if(delay == 0 || delay > RTU_PING_PONG_BUF_MAX_NUM*10)
//	{
//		rtudev->saveparam.sys_param_dac_mod.tx_mod_delay = RTU_PING_PONG_BUF_MAX_NUM*10/2;//10 us*100
//		saveflg = TRUE;
//	}
//	if(rtudev->saveparam.sys_param_dac_mod.spi_dac_k == 0)
//	{
//		rtudev->saveparam.sys_param_dac_mod.spi_dac_k = 10;
//		
//		saveflg = TRUE;
//	}
//	if(rtudev->saveparam.sys_param_dac_mod.tx_mod_k == 0)
//	{
//		rtudev->saveparam.sys_param_dac_mod.tx_mod_k = 10;
//		saveflg = TRUE;
//	}

	if(Rtu_Get_tx_freq() == 0 ||Rtu_Get_tx_freq() == 0xFFFFFFFF)
	{
		Rtu_Set_tx_freq(JJJHZTX);		
		saveflg = TRUE;
	}
	
	if(Rtu_Get_rx_freq() == 0 ||Rtu_Get_rx_freq() == 0xFFFFFFFF)
	{
		Rtu_Set_rx_freq(JJJHZRX);		
		saveflg = TRUE;
	}

	if(TRUE == saveflg)
	{
		RTU_Save_Param_to_Flash();
	}
}

/*****************************************************************************
 * 函 数 名  : RTU_dev_init
 * 负 责 人  : MZM
 * 创建日期  : 2016年5月23日
 * 函数功能  : rtu设备参数初始化
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void RTU_dev_init1()
{
	uint32 bytes;
	uint16 i,j,tmpflg;
	uint8 *tmpbuf,*readbufhead;
	TRC_COM_P pcom_data;
	//RTU_PARAM_FLASH_T *rtudev;
	VTR_FLAGS flag_first,flag_last;
	portBASE_TYPE  ret;
	
	rtudev=(RTU_PARAM_FLASH_T*) pvPortMalloc(sizeof(RTU_PARAM_FLASH_T));

	if(rtudev==NULL)
	{
//		rt_kprintf(DEBUG_SYS,"RTU_dev_init malloc[tmpbuf]  error!!\r\n");
	}
	else
	{
//		rt_kprintf(DEBUG_SYS,"RTU_dev_init malloc[tmpbuf]  SUCESS!!\r\n");
	}

	Rtu_Read_flag_from_flash(IAP_FIRST_SECTOR,&flag_first,sizeof(VTR_FLAGS));//读第14个扇区的标志
	Rtu_Read_flag_from_flash(IAP_LAST_SECTOR,&flag_last,sizeof(VTR_FLAGS));//读第15个扇区的标志
	
	if(flag_first.valid_flag != 0xFFFFFFFF && flag_last.valid_flag != 0xFFFFFFFF)
	{
		if(flag_first.valid_flag >= flag_last.valid_flag)//标志大的一个，表明参数是最新的
		{
			Rtu_Read_Data_From_Flash(IAP_FIRST_SECTOR*32*1024,(uint8_t *)&rtudev->saveparam,sizeof(RTU_PARAM_SAVE_FLASH));
			rtudev->saveparam.sys_param_flags.param_flag.save_flag = IAP_FIRST_SECTOR;
		}
		else
		{
			Rtu_Read_Data_From_Flash(IAP_LAST_SECTOR*32*1024,(uint8_t *)&rtudev->saveparam,sizeof(RTU_PARAM_SAVE_FLASH));				
			rtudev->saveparam.sys_param_flags.param_flag.save_flag = IAP_LAST_SECTOR;
		}
		
	//			rtudev->saveparam.txfreq = 470000000;
	//			rtudev->saveparam.sys_param_flags.param_flag.valid_flag = 1;
	//			rtudev->saveparam.sys_param_flags.param_flag.save_flag = IAP_FIRST_SECTOR;		
	//			Rtu_Save_Data_To_Flash(IAP_FIRST_SECTOR,IAP_FIRST_SECTOR*32*1024,(uint8_t *)&rtudev->saveparam,sizeof(RTU_PARAM_SAVE_FLASH));
	}
	else
	{
		flag_first.valid_flag = 1;
		flag_last.valid_flag = 0;
		//设置默认值。。此处
		/*第一次上电 无参数时，默认将参数写在第14个扇区。第15个扇区清0*/
		memset(rtudev,0x0,sizeof(RTU_PARAM_SAVE_FLASH));

		rtudev->saveparam.sys_param_flags.param_flag.valid_flag = 0;
		rtudev->saveparam.sys_param_flags.param_flag.save_flag = IAP_LAST_SECTOR;		
		Rtu_Save_Data_To_Flash(IAP_LAST_SECTOR,IAP_LAST_SECTOR*32*1024,(uint8_t *)&rtudev->saveparam,sizeof(RTU_PARAM_SAVE_FLASH));
			
		rtudev->saveparam.sys_param_flags.param_flag.valid_flag = 1;
		rtudev->saveparam.sys_param_flags.param_flag.save_flag = IAP_FIRST_SECTOR;		
		Rtu_Save_Data_To_Flash(IAP_FIRST_SECTOR,IAP_FIRST_SECTOR*32*1024,(uint8_t *)&rtudev->saveparam,sizeof(RTU_PARAM_SAVE_FLASH));
		
		
	}
	RTU_check_system_param();

//	#define JJJHZTX (470000000)

//	rtudev->notsaveparam.vtr_ch_info_current.vtrchannelinfo.rxfre.propertyint.w = ;
//	rtudev->notsaveparam.vtr_ch_info_current.vtrchannelinfo.rxfre.propertyint.w = ;
//		rxfre_data=(vtr_ch_infopar_set_chc.vtrchannelinfopar.rxfre.propertyint.whl.high*256+vtr_ch_infopar_set_chc.vtrchannelinfopar.rxfre.propertyint.whl.low)*1000000+\
//		(vtr_ch_infopar_set_chc.vtrchannelinfopar.rxfre.propertyfrac.whl.high*256+vtr_ch_infopar_set_chc.vtrchannelinfopar.rxfre.propertyfrac.whl.low)*125; 					
//		
//		txfre_data=(vtr_ch_infopar_set_chc.vtrchannelinfopar.txfre.propertyint.whl.high*256+vtr_ch_infopar_set_chc.vtrchannelinfopar.txfre.propertyint.whl.low)*1000000+\
//		(vtr_ch_infopar_set_chc.vtrchannelinfopar.txfre.propertyfrac.whl.high*256+vtr_ch_infopar_set_chc.vtrchannelinfopar.txfre.propertyfrac.whl.low)*125; 					


		Set_PLL_Freq(TX_PLL_SELECT,Rtu_Get_tx_freq(),REF_PLL_FREQ,0,0,3,0,0x0f,0,0);
		Set_PLL_Freq(RX_PLL_SELECT,Rtu_Get_rx_freq(),REF_PLL_FREQ,0,0,3,0,0x0f,0,0);
//		Set_PLL_Freq(LO2_PLL_SELECT,LO2_PLL_FREQ,REF_PLL_FREQ,	0,0,3,0,0x0f,0,0);//L02电压 73.35M-455K
//		Set_PLL_Freq(REF_PLL_SELECT,REF_PLL_FREQ,REF_PLL_REF_FREQ,	0,0,3,0,0x0f,0,0);//参考电压

		RTU_SE98A_Registers_init();
		RTU_NB_WB(1);

//		RTU_Save_Param_to_Flash();

}

/*****************************************************************************
 * 函 数 名  : RTU_dev_init
 * 负 责 人  : MZM
 * 创建日期  : 2016年5月23日
 * 函数功能  : rtu设备参数初始化
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void RTU_dev_init()
{
	uint32 bytes;
	uint16 i,j,tmpflg;
	uint8 *tmpbuf,*readbufhead;
	TRC_COM_P pcom_data;
	//RTU_PARAM_FLASH_T *rtudev;
	VTR_FLAGS flag_first,flag_last;
	portBASE_TYPE  ret;
	
	rtudev=(RTU_PARAM_FLASH_T*) pvPortMalloc(sizeof(RTU_PARAM_FLASH_T));

	if(rtudev==NULL)
	{
//		rt_kprintf(DEBUG_SYS,"RTU_dev_init malloc[tmpbuf]  error!!\r\n");
	}
	else
	{
//		rt_kprintf(DEBUG_SYS,"RTU_dev_init malloc[tmpbuf]  SUCESS!!\r\n");
	}

//	Rtu_Read_flag_from_flash(IAP_FIRST_SECTOR,&flag_first,sizeof(VTR_FLAGS));//读第14个扇区的标志
	Rtu_Read_flag_from_flash(IAP_LAST_SECTOR,&flag_last,sizeof(VTR_FLAGS));//读第15个扇区的标志
	
	if( flag_last.valid_flag != 0xFFFFFFFF)
	{

			Rtu_Read_Data_From_Flash(IAP_LAST_SECTOR*32*1024,(uint8_t *)&rtudev->saveparam,sizeof(RTU_PARAM_SAVE_FLASH));				
			rtudev->saveparam.sys_param_flags.param_flag.save_flag = IAP_LAST_SECTOR;
		
	//			rtudev->saveparam.txfreq = 470000000;
	//			rtudev->saveparam.sys_param_flags.param_flag.valid_flag = 1;
	//			rtudev->saveparam.sys_param_flags.param_flag.save_flag = IAP_FIRST_SECTOR;		
	//			Rtu_Save_Data_To_Flash(IAP_FIRST_SECTOR,IAP_FIRST_SECTOR*32*1024,(uint8_t *)&rtudev->saveparam,sizeof(RTU_PARAM_SAVE_FLASH));
	}
	else
	{
		flag_last.valid_flag = 0;
		//设置默认值。。此处
		/*第一次上电 无参数时，默认将参数写在第14个扇区。第15个扇区清0*/
		memset(rtudev,0x0,sizeof(RTU_PARAM_SAVE_FLASH));

		rtudev->saveparam.sys_param_flags.param_flag.valid_flag = 1;
		rtudev->saveparam.sys_param_flags.param_flag.save_flag = IAP_LAST_SECTOR;		
		Rtu_Save_Data_To_Flash(IAP_LAST_SECTOR,IAP_LAST_SECTOR*32*1024,(uint8_t *)&rtudev->saveparam,sizeof(RTU_PARAM_SAVE_FLASH));		
		
	}
	RTU_check_system_param();

//	#define JJJHZTX (470000000)

//	rtudev->notsaveparam.vtr_ch_info_current.vtrchannelinfo.rxfre.propertyint.w = ;
//	rtudev->notsaveparam.vtr_ch_info_current.vtrchannelinfo.rxfre.propertyint.w = ;
//		rxfre_data=(vtr_ch_infopar_set_chc.vtrchannelinfopar.rxfre.propertyint.whl.high*256+vtr_ch_infopar_set_chc.vtrchannelinfopar.rxfre.propertyint.whl.low)*1000000+\
//		(vtr_ch_infopar_set_chc.vtrchannelinfopar.rxfre.propertyfrac.whl.high*256+vtr_ch_infopar_set_chc.vtrchannelinfopar.rxfre.propertyfrac.whl.low)*125; 					
//		
//		txfre_data=(vtr_ch_infopar_set_chc.vtrchannelinfopar.txfre.propertyint.whl.high*256+vtr_ch_infopar_set_chc.vtrchannelinfopar.txfre.propertyint.whl.low)*1000000+\
//		(vtr_ch_infopar_set_chc.vtrchannelinfopar.txfre.propertyfrac.whl.high*256+vtr_ch_infopar_set_chc.vtrchannelinfopar.txfre.propertyfrac.whl.low)*125; 					


		Set_PLL_Freq(TX_PLL_SELECT,Rtu_Get_tx_freq(),REF_PLL_FREQ,0,0,3,0,0x0f,0,0);
		Set_PLL_Freq(RX_PLL_SELECT,Rtu_Get_rx_freq(),REF_PLL_FREQ,0,0,3,0,0x0f,0,0);
//		Set_PLL_Freq(LO2_PLL_SELECT,LO2_PLL_FREQ,REF_PLL_FREQ,	0,0,3,0,0x0f,0,0);//L02电压 73.35M-455K
//		Set_PLL_Freq(REF_PLL_SELECT,REF_PLL_FREQ,REF_PLL_REF_FREQ,	0,0,3,0,0x0f,0,0);//参考电压

		RTU_SE98A_Registers_init();
		RTU_NB_WB(1);

//		RTU_Save_Param_to_Flash();

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
	RTU_dev_init();// RTU设备
	
#ifndef DISABLE_ADC_TIMER_FOR_JJJ
	Task_Create_ADC_A(256,5);
#endif
//	Task_Create_ADC_B(128,1);
	Task_Create_Uart0_Server(256,4);		//串口服务程序
	Task_Create_Com_Protocol(256,4);		//协议解析处理程序
	Task_Create_10ms(256,1);				//定时任务
	
	Task_Create_Check_PLL_LD(256,3);		//锁定状态监测
	

	return ;
}
