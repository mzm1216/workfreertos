/***********************************************************************************
 * 文 件 名   : trccom.c
 * 负 责 人   : MZM
 * 创建日期   : 2016年5月16日
 * 文件描述   : 串口操作接口，调试信息输出接口、协议解析、打包接口。
 * 版权说明   : Copyright (c) 2008-2016   北京市万格数码通讯科技有限公司
 * 其    他   : 
 * 修改日志   : 
***********************************************************************************/

#define  TRCCOM_GLOBALS

#include "trccom.h"
#include "config.h"
#include <stdarg.h>


static uint32 rt_debug_level=0;//调试级别
 uint32 rt_test_backlight=99;//调试级别

void rt_kprintf(uint32_t flg,const char *fmt, ...);
/*****************************************************************************
 * 函 数 名  : Set_Debug_Level
 * 负 责 人  : MZM
 * 创建日期  : 2016年5月16日
 * 函数功能  : 使能调试级别
 * 输入参数  : uint32 level  调试级别：bit 0-31 代表不同的调试信息
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void Set_back_light(uint32 level)
{
	rt_test_backlight = level ;
//	rt_debug_level=level;
}
uint32 Get_back_light()
{
	return rt_test_backlight%100;
}

/*****************************************************************************
 * 函 数 名  : Clear_Debug_Level
 * 负 责 人  : MZM
 * 创建日期  : 2016年5月16日
 * 函数功能  : 禁用所有调试级别。
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void Clear_Debug_Level()
{
	rt_debug_level=0;
}

/*****************************************************************************
 * 函 数 名  : Set_All_Debug_Level
 * 负 责 人  : MZM
 * 创建日期  : 2016年5月16日
 * 函数功能  : 使能所有调试信息
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void Set_All_Debug_Level()
{
	rt_debug_level = 0xFFFFFFFF;
}


/*****************************************************************************
 * 函 数 名  : Set_Debug_Level
 * 负 责 人  : MZM
 * 创建日期  : 2016年5月16日
 * 函数功能  : 使能调试级别
 * 输入参数  : uint32 level  调试级别：bit 0-31 代表不同的调试信息
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void Set_Debug_Level(uint32 level)
{
	rt_debug_level|=(0x1<<level);
//	rt_debug_level=level;
}

/*****************************************************************************
 * 函 数 名  : Get_Debug_Level
 * 负 责 人  : MZM
 * 创建日期  : 2016年5月16日
 * 函数功能  : 获取调试级别参数
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
uint32 Get_Debug_Level()
{
	return rt_debug_level;
}

#if 0
void TRC_COM_init_app(uint8 com)
{
	uint8 *ptr;
	TRC_COM_P tmpptr;

	if(com==0)
	{
		tmpptr=&trc_com0;
	}
	else{
		tmpptr=&trc_com1;
	}
	

	//tmpptr =(TRC_COM_P)ptr;
	tmpptr->rx_frame_buf.data = tmpptr->rxdata;
	tmpptr->rx_frame_buf.head=0;
	tmpptr->rx_frame_buf.tail=0;
	tmpptr->rx_frame_buf.itemSz=1;
	tmpptr->rx_frame_buf.count=TRC_COM_MAX_RX_NUM;
	
	tmpptr->mutex_sem_opt_buf=xSemaphoreCreateMutex();
	if(tmpptr->mutex_sem_opt_buf==NULL)
	{
	
		rt_kprintf("TRC_COM_init_app:Com[%d] NO SPACE ERROR--xSemaphoreCreateMutex\r\n",com);
		return;
	}
	
	if(com==0)
	{
		tmpptr->rx_driver_buf_ptr=&Rxring[0];
		trc_com_ptr0 =(TRC_COM_P)tmpptr;
	}
	else
	{
	
		tmpptr->rx_driver_buf_ptr=&Rxring[1];
		trc_com_ptr1 =(TRC_COM_P)tmpptr;
	}
	
	return;
	
}

TRC_COM_P Get_TRC_Com_Dev(uint8 com)
{
	if(com==0)
		{
		return (TRC_COM_P)&trc_com0;
		}
	else{
		
		return (TRC_COM_P)&trc_com1;
		}
}

#else

/*****************************************************************************
 * 函 数 名  : TRC_COM_init_app
 * 负 责 人  : MZM
 * 创建日期  : 2016年5月16日
 * 函数功能  : 串口结构初始化
 * 输入参数  : uint8 com  串口号
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void TRC_COM_init_app(uint8 com)
{
	uint8 *ptr;
	TRC_COM_P tmpptr;

//	ptr=(uint8*) pvPortMalloc(sizeof(TRC_COM_T));
	ptr=(uint8*) pvPortMalloc(sizeof(TRC_COM_T));
	if(ptr==NULL)
	{
		rt_kprintf(DEBUG_SYS,"TRC_COM_init_app:Com[%d] NO SPACE ERROR--pvPortMalloc\r\n",com);
		return;
	}
	
	tmpptr =(TRC_COM_P)ptr;
	tmpptr->rx_frame_buf.data = tmpptr->rxdata;
	tmpptr->rx_frame_buf.head=0;
	tmpptr->rx_frame_buf.tail=0;
	tmpptr->rx_frame_buf.itemSz=1;
	tmpptr->rx_frame_buf.count=TRC_COM_MAX_RX_NUM;
#if 0
	tmpptr->mutex_sem_read_ok=xSemaphoreCreateMutex();//2015年12月11日18:12:53 改二值信号量
	if(tmpptr->mutex_sem_read_ok==NULL)
	{
	
		rt_kprintf("TRC_COM_init_app:Com[%d] NO SPACE ERROR--xSemaphoreCreateMutex--mutex_sem_read_ok\r\n",com);
		return;
	}

	tmpptr->mutex_sem_write_ok=xSemaphoreCreateMutex();
		if(tmpptr->mutex_sem_write_ok==NULL)
		{
		
			rt_kprintf("TRC_COM_init_app:Com[%d] NO SPACE ERROR--xSemaphoreCreateMutex--mutex_sem_write_ok\r\n",com);
			return;
		}
	xSemaphoreTake(tmpptr->mutex_sem_write_ok,portMAX_DELAY);

	#else

	vSemaphoreCreateBinary(tmpptr->mutex_sem_read_ok);
	vSemaphoreCreateBinary(tmpptr->mutex_sem_write_ok);
	vSemaphoreCreateBinary(tmpptr->driver_sem_receive_data);
	
	//xSemaphoreGive(tmpptr->mutex_sem_read_ok);
	#endif
	if(com==0)
	{
		tmpptr->rx_driver_buf_ptr=&rxring;
		//tmpptr->rx_driver=&Rxring[0];
		trc_com_ptr0 =(TRC_COM_P)tmpptr;
	}
	else
	{
	
		//tmpptr->rx_driver_buf_ptr=&Rxring[1];
		//trc_com_ptr1 =(TRC_COM_P)tmpptr;
	}
#ifdef TESTFORJJJ
	tmpptr->test_data.flag = 0; 
#endif
	return;
	
}

TRC_COM_P Get_TRC_Com_Dev(uint8 com)
{
	if(com==0)
	{
		return (TRC_COM_P)trc_com_ptr0;
	}
	else
	{
		return (TRC_COM_P)trc_com_ptr1;
	}
}

#endif

LPC_USART_T* Get_TRC_Com_Dev_Uart(uint8 com)
{
	if(com==0)
		{
		return LPC_USART3;
		}
	else{
		
		return LPC_USART0;
		}
}

RINGBUFF_T * Get_TRC_Com_RxRing(TRC_COM_P ptr)
{
	return &ptr->rx_frame_buf;
}

/*****************************************************************************
 * 函 数 名  : rt_kprintf
 * 负 责 人  : MZM
 * 创建日期  : 2016年5月16日
 * 函数功能  : 可变调试信息输出
 * 输入参数  : uint32_t flg     调试信息标志
               const char *fmt  可变参数
               ...              可变调试信息参数 输出
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void rt_kprintf(uint32_t flg,const char *fmt, ...)
{

	uint32_t bytes;
    va_list args;
    uint32_t length;

	
//	if(Get_Debug_Level()==0)
//		{
//		return;
//		}
//   
//	if(Get_Debug_Level()&(0x1<<flg))
	{
//			va_start(args, fmt);
			/* the return value of vsnprintf is the number of bytes that would be
			 * written to buffer had if the size of the buffer been sufficiently
			 * large excluding the terminating null byte. If the output string
			 * would be larger than the rt_log_buf, we have to adjust the output
			 * length. */
		//	 rt_memset(rt_log_buf,0, sizeof(rt_log_buf));
			length = vsnprintf(rt_log_buf, sizeof(rt_log_buf) - 1, fmt, args);
			if (length > UART_SRB_SIZE - 1)
				length = UART_SRB_SIZE - 1;
			
				bytes = uart_send_data(rt_log_buf, length);//发送
//				pUART=(LPC_USART_T *)Get_TRC_Com_Dev_Uart(0);
//				bytes = Chip_UART_SendRB(pUART, &Txring[0], rt_log_buf, length);//发送
//			va_end(args);
	}
}

/*****************************************************************************
 * 函 数 名  : rt_kprintf
 * 负 责 人  : MZM
 * 创建日期  : 2016年5月16日
 * 函数功能  : 可变调试信息输出
 * 输入参数  : 格式化输出接口
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void rt_kprintf_dirver(const char *fmt, ...)
{

	uint32_t bytes;
	  va_list args;
    uint32_t length;

	
	
    //va_start(args, fmt);
    /* the return value of vsnprintf is the number of bytes that would be
     * written to buffer had if the size of the buffer been sufficiently
     * large excluding the terminating null byte. If the output string
     * would be larger than the rt_log_buf, we have to adjust the output
     * length. */
//	 rt_memset(rt_log_buf,0, sizeof(rt_log_buf));
    length = vsnprintf(rt_log_buf, sizeof(rt_log_buf) - 1, fmt, args);
    if (length > UART_SRB_SIZE - 1)
        length = UART_SRB_SIZE - 1;
		bytes = uart_send_data(rt_log_buf, length);//发送

//		pUART=(LPC_USART_T *)Get_TRC_Com_Dev_Uart(0);
//		bytes = Chip_UART_SendRB(pUART, &Txring[0], rt_log_buf, length);//发送
  //  va_end(args);
}

/*****************************************************************************
 * 函 数 名  : rt_pirntf_hex
 * 负 责 人  : MZM
 * 创建日期  : 2016年5月16日
 * 函数功能  : 将buf的数据按16进制打印出来
 * 输入参数  : uint32_t flg  调试信息标志
               uint8_t *buf  显示内容
               uint16_t len  显示长度
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void rt_pirntf_hex(uint32_t flg,uint8_t *buf,uint16_t len)
{
	uint32_t i;
	rt_kprintf(flg,"\r\nPintf_Hex_Len[%x]-Data:",len);
	for(i=0;i<len;i++)
		{
		rt_kprintf(flg,"[%02x]",buf[i]);
		}
	rt_kprintf(flg,"\r\n");
}

/*****************************************************************************
 * 函 数 名  : Send_Vtr_Up_Data
 * 负 责 人  : MZM
 * 创建日期  : 2016年5月16日
 * 函数功能  : 发送上报数据
 * 输入参数  : uint8 ch            上报信息通道
               uint8 *data_source  上报数据
               uint16 length       上报数据长度
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void Send_Vtr_Up_Data(uint8 ch,uint8 *data_source,uint16 length)
{
	uint16 new_length=1;
	uint16 i;
	uint8 temp;
	
	LPC_USART_T *pUART;
	
	if(length<(128+512-1))
	{
		memcpy(Send_Potocol_buf+1,data_source,length);
	}
	
	*Send_Potocol_buf=0x01;
	for(i=0;i<length;i++)
	{
		temp=*(data_source+i);
		switch(temp)
		{
			case 0x01:
			case 0x10:
			case 0x04:
				*(Send_Potocol_buf+new_length++)=0x10;
				*(Send_Potocol_buf+new_length++)=(uint8)(0x20+temp);
				break;
			default:
				*(Send_Potocol_buf+new_length++)=temp;
				break;
		}
	}
	*(Send_Potocol_buf+new_length++)=0x04;
	
	if(ch == 0)
	{
		uart_send_data(Send_Potocol_buf, new_length);//发送
	}
		
}

/****************************************************************************
* 名称：Packet_Vtr_Up_Data()
* 功能：将已经组好的报文(lab+seq+cmd+data),之后加上计算校验cs2个字节，
		并将组好的报文发送
* 入口参数：VTR_DEV_DATA *vtr_dev_data:vtr设备结构体
* 出口参数：返回0表示操作成功
****************************************************************************/
uint8 Packet_Vtr_Up_Data(VTR_PROTOCOL_DATA *vtr_dev_data)
{	
		UintType check_sum;//校验和
		uint16 i;
		uint8 *temp_buf = (uint8*) vtr_dev_data->txd_fun.txd_buf;
		uint16 datalen = vtr_dev_data->txd_fun.tx_len;
		
		
		vtr_dev_data->txd_fun.cmd+=1;//回应命令+1
	//	mzm_kprintf(1,vtr_dev_data->txd_fun.cmd,datalen,1);
		
		check_sum.w=vtr_dev_data->txd_fun.cmd;
		for(i=0;i<datalen;i++)
		{
			check_sum.w += temp_buf[i];
		}
		
		temp_buf[datalen]=check_sum.bb[1];
		temp_buf[datalen+1]=check_sum.bb[0];
		
		Send_Vtr_Up_Data(vtr_dev_data->ch,(uint8*)&vtr_dev_data->txd_fun,datalen+5);
		
		return suc_respond;

}


/****************************************************************************
* 名称：Packet_Vtr_Up_Data()
* 功能：一些需要先发回应帧，在处理的事情在这里处理:如A4命令，重启信道机操作
* 入口参数：VTR_DEV_DATA *vtr_dev_data:vtr设备结构体
* 出口参数：返回0表示操作成功
****************************************************************************/
void Do_Somethings_After_Send_Data(VTR_PROTOCOL_DATA * vtrdev)
{
	if(vtrdev->txd_fun.cmd ==0xA4)
	{
//		if(updata_boot_flag == 1)
//		{
//		}
//		else
//		{
//			root_sys();	
//		}
//	
	}
	return;
}

/****************************************************************************
* 名称：Process_Vtr_Param()
* 功能：协议处理:分析、处理、组回应帧.
* 入口参数：VTR_DEV_DATA *vtr_dev_data:vtr设备结构体
* 出口参数：返回:0x0:回应报文已经组好，可直接发送
			返回:0xff:回应帧未组好，请组确认帧
			返回:0xfe:回应帧未组好，请组否认帧 错误类型--1
			返回:0xfd:回应帧未组好，请组否认帧 错误类型--2
			返回:0xfc:回应帧未组好，请组否认帧 错误类型--3
			返回:0xfb:回应帧未组好，请组否认帧 错误类型--4
			返回:0xfa:回应帧未组好，请组否认帧 错误类型--5
			返回:0xf9:回应帧未组好，请组否认帧 错误类型--6
****************************************************************************/
uint8 Process_Vtr_Param(VTR_PROTOCOL_DATA *vtr_dev_data)
{
	uint8 ret=0;

	uint8 *recvdata =  &vtr_dev_data->rxd_buf[4];
	uint8 *databuf = vtr_dev_data->txd_fun.txd_buf;
	uint16 *datalen = &vtr_dev_data->txd_fun.tx_len;
	uint8	ch = vtr_dev_data->ch;
	
	switch(vtr_dev_data->txd_fun.cmd)//命令处理
	{
#if 0		
		case 0x01://发送测试连接
			*datalen = 1;
			vtr_dev_data->txd_fun.txd_buf[0] =vtr_edit_chnum;
			Vtr_Set_Link_Fun(ch,1);
			Vtr_Set_Link_Fun_Cnt(ch,0);
			ret = suc_respond;
			break;
		

		case 0x05://选件板类型
			{
				ret=Vtr_Read_AccessoriesType_Info(databuf,sizeof(VTR_PARAM_F6));
				*datalen = sizeof(VTR_PARAM_F6);
			}
			break;
		case 0x07://设备固件信息
			{
				ret=Vtr_Read_Ch_IdInfo(databuf,sizeof(VTR_PARAM_F8));
				*datalen = sizeof(VTR_PARAM_F8);
			}
			break;
		case 0x09://电压报警参数
			{
				ret=Vtr_Read_VoltageAlarm_Info(databuf,sizeof(VTR_PARAM_FA));
				*datalen = sizeof(VTR_PARAM_FA);
			}
			break;
		case 0x0b://功放温度报警参数
			{
				ret=Vtr_Read_PaTempAlarm_Info(databuf,sizeof(VTR_PARAM_FC));
				*datalen = sizeof(VTR_PARAM_FC);
			}
			break;
		case 0x0d://发射功率报警参数
			{
				ret=Vtr_Read_TxPowerAlarm_Info((uint8*)&vtr_dev_data->txd_fun.txd_buf);
				*datalen = sizeof(VTR_PARAM_FE);
				break;
			}
		case 0x11://查询报警信息来源
			{
				ret=Vtr_Read_AlarmSourceLed_Info(databuf,datalen);
			}
			break;
		case 0x13://4.18　0x13，向VTR发送查询EEPROM信息
			{
				ret=Vtr_Read_Eeprom_Data(recvdata,databuf,datalen);
			}
			break;
		case 0x15://4.20　0x15,向VTR发送查询MCU flash信息
			{
				ret=Vtr_Read_Flash_Data(recvdata,databuf,datalen);
			}
			break;
			
		case 0x17://查询软件设置信息
			{
				ret=Vtr_Read_SoftWareSetPar_Info(databuf,datalen);
			}
			break;
		case 0x19://查询硬件参数信息
			{
				ret=Vtr_Read_HardWarePar_Info(databuf,datalen);
			}
			break;
		case 0x1b://查询提示音信息
			{
				ret=Vtr_Read_PwmTonepar_Info(databuf,datalen);
			}
			break;
		case 0x1d://查询附件引脚类型
			{
				ret=Vtr_Read_Appendix_Info(databuf,datalen);
			}
			break;
	
		case 0x21://查询选件板类型
			{
				ret=Vtr_Set_AccessoriesType_InfoPar(recvdata,databuf,datalen);
			}
			break;
		case 0x23://设置信道机的当前通道
			{
				ret=Vtr_Set_Softwarech_No(ch,*recvdata,databuf,datalen);
				//*datalen=(uint16)0x1;
			}
			break;
			
		case 0x03://读信道信息
			{
				vtr_Read_CH_info(ch,recvdata,databuf,sizeof(VTR_PARAM_F4));
				*datalen = sizeof(VTR_PARAM_F4);
			}
			break;
		case 0x25://设置信道机的通道参数
			{
				ret=Vtr_Set_Channel_InfoPar(ch,recvdata,databuf,datalen);
			}
			break;


		case 0x27://设置发射功率参数报警设置
			{	
				ret=Vtr_Set_TxPowerAlarm_Info(recvdata,databuf,datalen);
			}
			break;
		case 0x29://设置电压报警参数设置
			{	
				ret=vtr_Set_VoltageAlarm_Info(recvdata,databuf,datalen);
			}
			break;
		case 0x2b://设置功放温度报警参数设置Vtr_set_patempalarm_info
			{	
				ret=Vtr_Set_PatempAlarm_Info(recvdata,databuf,datalen);
			}
			break;
		case 0x2d://，向VTR发送设置报警管脚和报警灯的输出来源Vtr_set_alarmsourceled_info
			{	
				ret=Vtr_Set_AlarmSourceLed_Info(recvdata,databuf,datalen);
			}
			break;
			
		case 0x31://向VTR发送设置EEPROM信息Vtr_write_eeprom_data
			{	
				ret=Vtr_Write_Eeprom_Data(recvdata,databuf,datalen);
			}
			break;
		case 0x33://向VTR发送设置MCU flash信息
			{	
				ret=Vtr_Write_Flash_Data(recvdata,databuf,datalen);
			}
			break;
		case 0x35://设置软件设置信息Vtr_set_softwaresetpar_info
			{	
				ret=Vtr_Set_SoftWareSetpar_Info(recvdata,databuf,datalen);
			}
			break;
		case 0x37://设置提示音参数Vtr_set_pwmtonepar_info
			{	
				ret=Vtr_Set_PwmTonepar_Info(recvdata,databuf,datalen);
			}
			break;
		case 0x39://设置附件功能接口Vtr_set_appendix_info
			{	
				ret=Vtr_Set_Appendix_Info(recvdata,databuf,datalen);
			}
			break;
			
		case 0x3b://向VTR发送恢复默认出厂设置 暂时未实现
			//Resume_Factory_Defaults();
			break;
		case 0x3d://自动显示当前通道动态信息Vtr_dynamic_display_func
			{	
				ret=Vtr_Dynamic_Display_Func(ch,recvdata,databuf,datalen);
			}
			break;	
		case 0x41://进入测试模式Vtr_Goahead_CH_Conner
			{	
				ret=Vtr_Goahead_CH_Conner(recvdata,databuf,datalen);
			}
			break;
		case 0x43://控制PTT状态Vtr_control_sptt
			{	
				ret=Vtr_Control_Sptt(recvdata,databuf,datalen);
			}
			break;
		case 0x45://控制静噪状态Vtr_control_mute
			{	
				ret=Vtr_Control_Mute(recvdata,databuf,datalen);
			}
			break;
		case 0x47://控制风扇工作模式
			{	
				ret=Vtr_Control_Fan(recvdata,databuf,datalen);
			}
			break;
		case 0x49://进入校准模式Vtr_Goahead_CH_Calibrate
			{	
				
				ret=Vtr_Goahead_CH_Calibrate(recvdata,databuf,datalen);
	
			}
			break;
			
			#endif
#if 1
		case 0x4b:		//功率校准命令A
		case 0x51:		//功率校准命令B
		case 0x71:		//功率校准命令C
		case 0x73:		//功率校准命令D
		case 0xa5:		//功率校准命令E
		case 0xa7:		//功率校准命令F
		case 0xa9:		//功率校准命令G
			
		case 0x55:		//场强校准命令A
		case 0x57:		//场强校准命令B
		case 0x75:		//场强校准命令C
		case 0x77:		//场强校准命令D
			
		case 0x5b:		//温度校准命令A
		case 0x5d:		//温度校准命令B
		case 0x79:		//温度校准命令C
		case 0x7b:		//温度校准命令D
			
		case 0x8b:		//静噪校准命令A
		case 0x8d:		//静噪校准命令A
			
		case 0x4d:		//向VTR发送功率校准指令
		case 0x53:		//向VTR发送场强校准指令
		case 0x59:		//向VTR发送功放温度校准指令
		case 0x6b:		//向VTR发送设置音频校准指令
		case 0x6d:		//向VTR发送设置POT电位器校准指令
		case 0x89:		//向VTR发送静噪等级校准指令
//				ret=Vtr_Do_Calibrate_Task_By_Fn(vtr_dev_data->txd_fun.cmd,recvdata);
//				RTU_Save_Param_to_Flash();
		break;
#else
 	case 0x59:

		{
			uint16 i,count=0,offset=0;
			uint8* tmptr=(uint8*)&calibrate_sys_param;
			uint8 tmbuf[4096];
			uint8 tmbuf1[4096];
				
			for(i=0;i<sizeof(VTR_CALIBRATE_SYSTEM_PARAM);i++)
				{
				tmbuf[i]=i;
				*tmptr++=i;
				}

			Vtr_Set_Calibrate_Param_From_Flash((uint8*)&calibrate_sys_param);
			Vtr_Read_Calibrate_Param_From_Flash(tmbuf1);
			for(i=0;i<sizeof(VTR_CALIBRATE_SYSTEM_PARAM);i++)
			{
				if(tmbuf[i] !=tmbuf1[i])
				{
					if(offset==0)
					{
						offset = i;
					}
					count++;
				}
			}		

			mzm_kprintf(12,"erroroffset",offset,"count",count,1);
			mzm_kprintf(13,"voice",sizeof(VTR_CALIBRATE_VOICE_STRCUT),"power",sizeof(VTR_CALIBRATE_POWER_POINT_STRCUT),1);
			mzm_kprintf(0,"temp",sizeof(VTR_CALIBRATE_TEMP_STRCUT),"rssi",sizeof(VTR_CALIBRATE_RSSI_STRCUT),1);
 		}
		//TX校准测试
		case 0x6b://发送设置音频校准指令Vtr_set_voice_calibratepar
			{	
				ret=Vtr_Set_Voice_Calibratepar(recvdata,databuf,datalen);
			}
			break;
		case 0x6d://发送设置POT电位器校准指令Vtr_set_pot_calibratepar
			{	
				ret=Vtr_Set_Pot_Calibratepar(recvdata,databuf,datalen);
			}
			break;
			
		case 0x4b://发送功率校准指令Vtr_send_power_calibratepar
			{	
				ret=Vtr_Send_Power_Calibratepar(recvdata,databuf,datalen);
			}
			break;
		case 0x4d://发送功率校准指令Vtr_power_calibrate_abcdefg
			{	
				ret=Vtr_power_calibrate_abcdefg(vtr_dev_data->txd_fun.cmd,recvdata,databuf,datalen);
			}
			break;	
		case 0x51://发送功率校准指令B
			{
				ret=Vtr_power_calibrate_abcdefg(vtr_dev_data->txd_fun.cmd,recvdata,databuf,datalen);
				//计算功率曲线
				calculate_power_graph(1,vtr_power_work_type);
			}
			break;
		case 0x71:
			{
				ret=Vtr_power_calibrate_abcdefg(vtr_dev_data->txd_fun.cmd,recvdata,databuf,datalen);
				//计算功率曲线
				calculate_power_graph(2,vtr_power_work_type);
			}
			break;
		case 0x73:
			{
				ret=Vtr_power_calibrate_abcdefg(vtr_dev_data->txd_fun.cmd,recvdata,databuf,datalen);
				//计算功率曲线
				calculate_power_graph(3,vtr_power_work_type);
			}
			break;
	
		case 0xa5:
			{
				ret=Vtr_power_calibrate_abcdefg(vtr_dev_data->txd_fun.cmd,recvdata,databuf,datalen);
				//计算功率曲线
				calculate_power_graph(4,vtr_power_work_type);
			}
			break;
		case 0xa7:
			{
				ret=Vtr_power_calibrate_abcdefg(vtr_dev_data->txd_fun.cmd,recvdata,databuf,datalen);
				//计算功率曲线
				calculate_power_graph(5,vtr_power_work_type);
			}		
		break;
		case 0xa9:
			{
				ret=Vtr_power_calibrate_abcdefg(vtr_dev_data->txd_fun.cmd,recvdata,databuf,datalen);
				//计算功率曲线
				calculate_power_graph(6,vtr_power_work_type);
			}
			break;					  
			
		case 0x53://发送场强校准指令
			{
				ret=Vtr_Send_Rssi_Calibratepar(recvdata,databuf,datalen);
			}
			break;
		case 0x55://发送场强校准指令A
			{
				ret=Vtr_Rssi_Calibrate_abcd(vtr_dev_data->txd_fun.cmd,recvdata,databuf,datalen);
			}
			break;
		case 0x57://发送场强校准指令B
			{
				ret=Vtr_Rssi_Calibrate_abcd(vtr_dev_data->txd_fun.cmd,recvdata,databuf,datalen);
				calculate_rssi_graph(1,vtr_rssi_work_type);
			}
			break;
		case 0x75:
			{
				ret=Vtr_Rssi_Calibrate_abcd(vtr_dev_data->txd_fun.cmd,recvdata,databuf,datalen);
				calculate_rssi_graph(2,vtr_rssi_work_type);
			}			
			break;
		case 0x77:
			{
				ret=Vtr_Rssi_Calibrate_abcd(vtr_dev_data->txd_fun.cmd,recvdata,databuf,datalen);
				calculate_rssi_graph(3,vtr_rssi_work_type);
			}			
			break;
		case 0x59://发送功放温度校准指令
			{
				ret =Vtr_Send_Temperature_Calibrate(recvdata,databuf,datalen);
			}
			break;
		case 0x5b://发送功放温度校准指令A
			{
				ret=Vtr_Temperature_Calibrate_abcd(vtr_dev_data->txd_fun.cmd,recvdata,databuf,datalen);
			}
			break;
		case 0x5d://发送功放温度校准指令B
			{
				ret=Vtr_Temperature_Calibrate_abcd(vtr_dev_data->txd_fun.cmd,recvdata,databuf,datalen);
				calculate_temperature_graph(1);
			}
			break;		
		case 0x79://发送功放温度校准指令c
			{
				ret=Vtr_Temperature_Calibrate_abcd(vtr_dev_data->txd_fun.cmd,recvdata,databuf,datalen);
				calculate_temperature_graph(2);
			}
			break;
		case 0x7b://发送功放温度校准指令D
			{
				ret=Vtr_Temperature_Calibrate_abcd(vtr_dev_data->txd_fun.cmd,recvdata,databuf,datalen);
				calculate_temperature_graph(3);
			}	
			break;
		case 0x89://4.96　向VTR发送静噪等级校准指令（校准模式下）
			{
				ret =Vtr_Send_Mute_Calibratepar(recvdata,databuf,datalen);
			}
			break;
		case 0x8b://4.98　向VTR发送静噪等级校准指令A（校准模式下）
			{
				ret=Vtr_Mute_Calibrate_ab(vtr_dev_data->txd_fun.cmd,recvdata,databuf,datalen);
			}
			break;
		case 0x8d://4.100　向VTR发送静噪等级校准指令B（校准模式下）
			{
				ret=Vtr_Mute_Calibrate_ab(vtr_dev_data->txd_fun.cmd,recvdata,databuf,datalen);
				calculate_mute_graph(1,vtr_mute_work_type);
			}
			break;
	#endif
		
		
		
		#if 0
			case 0x61://模拟开关设置Vtr_set_switch_state
			{
				ret =Vtr_Set_Switch_State(recvdata,databuf,datalen);
			}
			break;
		case 0x63://4.88　0x63向VTR发送LED测试指令Vtr_set_led_state
			{
				ret =Vtr_Set_Led_State(recvdata,databuf,datalen);
			}
			break;
		case 0x65://增加校准状态响应Vtr_Set_TestVoice_Fre
			{
				ret =Vtr_Set_TestVoice_Fre_new(recvdata,databuf,datalen);
			}
			break;
		case 0x67://VTR发送设置POT电位器测试指令
			{
				//set_potvalue();
				ret = 0xFF;
			}
			break;
			
		case 0x69://向VTR发送设置硬件信息及射频参数Vtr_set_hardwarepar_info
			{
				ret =Vtr_Set_Hardwarepar_Info(recvdata,databuf,datalen);
			}
			break;
			
		case 0x81://Vtr_Pc_Write_VtrChStartCmd
			{
				ret =Vtr_Pc_Write_VtrChStartCmd(recvdata,databuf,datalen);
			}
			break;
		case 0x83://Vtr_Pc_Write_VtrChOverCmd
			{
				ret =Vtr_Pc_Write_VtrChOverCmd(ch,recvdata,databuf,datalen);
			}
			break;
		case 0x85://Vtr_pc_read_vtrchstart
			{
				ret =Vtr_Pc_Read_VtrChStart(recvdata,databuf,datalen);
			}
			break;	
		case 0x87://Vtr_pc_read_vtrchover
			{
				ret =Vtr_Pc_Read_VtrChOver(recvdata,databuf,datalen);
			}
			break;
	
		case 0x91://Vtr_pc_write_vtrconfstart
			{
				ret =Vtr_Pc_Write_VtrConfStart(recvdata,databuf,datalen);
			}
			break;
		case 0x93://Vtr_pc_write_vtrconfover
			{
				ret =Vtr_Pc_Write_VtrConFover(recvdata,databuf,datalen);
			}
			break;
		case 0x95://pc_read_vtrconfstart
			{
				ret =Vtr_Pc_Read_VtrConfStart(recvdata,databuf,datalen);
			}
			break;	
		case 0x97:
			{
				ret =Vtr_Pc_Read_VtrConfOver(recvdata,databuf,datalen);
			}
			break;
	
		case 0x99://Vtr_pc_set_vtrchid
			{
				ret =Vtr_Pc_Set_VtrChId(recvdata,databuf,datalen);
			}
			break;						
	
		case 0x9b://Vtr_Pc_Query_VtrEepromCmd
			{
				ret =Vtr_Pc_Query_VtrEepromCmd(recvdata,databuf,datalen);
			}
			break;
	
		case 0x9d://Vtr_Pc_Set_Uart0Par
			{
				ret =Vtr_Pc_Set_Uart0Par(recvdata,databuf,datalen);
			}
			break;
	
		case 0xa1://4.140　0xa1  向VTR发送升级文件命令（串口通讯速率115200，文件格式 *.bin）
			{
				ret = Vtr_Pc_Updata_UserCode(vtr_dev_data);//升级用户程序
			}
			break;
		case 0xa3://4.142　0xa3  向VTR发送强制TRC板运行程序区命令			
			{
				ret =vtr_Appoint_UserCode_Info(recvdata,databuf,datalen);
			}
			break;	
			
			/*******************************以下命令为CHC独有命令*********************************/
			/******************************参数ch是方便利用pc进行测试*****************************/
			
		case 0xad://4.152　PDT控制板查询TRC和信道机信息
			{
				ret=Vtr_Chc_Read_TRC_Info(ch,recvdata,databuf,datalen);
			}
			break;
		case 0xaf://4.154　PDT控制板向TRC发送配置数据
			{
				ret=Vtr_Chc_Set_TRC_Info(ch,recvdata,databuf,datalen);
			}
			break;			

		case 0xf1:
			if(vtr_accessoriestype==0x01)
			{

				ret=Vtr_Mpt_Chc_response(ch,recvdata,databuf,datalen);
			}
			else
			{
				ret=RET_FAIL_FNULL;
			}
			
			break;
		case 0xf2:
			{
				ret=Vtr_Mpt_Basic_Info_Chc_Save(ch,recvdata,databuf,datalen);
			}
			//Basic_Info_chc_mpt(do_rxd1_bake.buf);
			break;
		case 0xf4:
			{
				ret=Vtr_Mpt_State_Info_Chc_Save(ch,recvdata,databuf,datalen);
			}
			break;
		case 0xf6:
			{
				ret=Vtr_Talk_Time_Chc_Mpt_Pdt(ch,recvdata,databuf,datalen);
			}
			break;
//pdt			
		case 0xf7:
			if(vtr_accessoriestype==0x02)
			{
				ret=Vtr_Pdt_Chc_Response(ch,recvdata,databuf,datalen);
			}
		else
			{
				ret=RET_FAIL_FNULL;
			}
			
			break;				
		case 0xf8:
			{
				ret=Vtr_Pdt_Basic_Info_Chc_Save(ch,recvdata,databuf,datalen);
			}
			break;	
		case 0xfa:
			{
				ret=Vtr_Pdt_state_info_save(ch,recvdata,databuf,datalen);
			}
			break;	
		case 0xfc://Vtr_Led_State_Pdt
			{
				ret=Vtr_Pdt_Led_State_Ctrl(ch,recvdata,databuf,datalen);
			}
			break;
			/*******************************以下命令为VSC独有命令*********************************/
			/******************************参数ch是方便利用pc进行测试*****************************/
		case 0xe1:
			{
			 	ret=Vtr_Vsc_Chc_response(ch,recvdata,databuf,datalen);
			}
			break;
		case 0xe2:
			{
			 	ret=Vtr_Vsc_Basic_Info_Chc_Save(ch,recvdata,databuf,datalen);
			}
			break;
		case 0xe4:
			{
				ret=Vtr_Vsc_State_Info_Chc_Save(ch,recvdata,databuf,datalen);
			}
			break;					
#endif
		default:
//				ret=RET_FAIL_F1;
			break;
	}

	return ret;
}




