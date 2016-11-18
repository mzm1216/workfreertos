/***********************************************************************************
 * �� �� ��   : trccom.c
 * �� �� ��   : MZM
 * ��������   : 2016��5��16��
 * �ļ�����   : ���ڲ����ӿڣ�������Ϣ����ӿڡ�Э�����������ӿڡ�
 * ��Ȩ˵��   : Copyright (c) 2008-2016   �������������ͨѶ�Ƽ����޹�˾
 * ��    ��   : 
 * �޸���־   : 
***********************************************************************************/

#define  TRCCOM_GLOBALS

#include "trccom.h"
#include "config.h"
#include <stdarg.h>


static uint32 rt_debug_level=0;//���Լ���
 uint32 rt_test_backlight=99;//���Լ���

void rt_kprintf(uint32_t flg,const char *fmt, ...);
/*****************************************************************************
 * �� �� ��  : Set_Debug_Level
 * �� �� ��  : MZM
 * ��������  : 2016��5��16��
 * ��������  : ʹ�ܵ��Լ���
 * �������  : uint32 level  ���Լ���bit 0-31 ����ͬ�ĵ�����Ϣ
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

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
 * �� �� ��  : Clear_Debug_Level
 * �� �� ��  : MZM
 * ��������  : 2016��5��16��
 * ��������  : �������е��Լ���
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void Clear_Debug_Level()
{
	rt_debug_level=0;
}

/*****************************************************************************
 * �� �� ��  : Set_All_Debug_Level
 * �� �� ��  : MZM
 * ��������  : 2016��5��16��
 * ��������  : ʹ�����е�����Ϣ
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void Set_All_Debug_Level()
{
	rt_debug_level = 0xFFFFFFFF;
}


/*****************************************************************************
 * �� �� ��  : Set_Debug_Level
 * �� �� ��  : MZM
 * ��������  : 2016��5��16��
 * ��������  : ʹ�ܵ��Լ���
 * �������  : uint32 level  ���Լ���bit 0-31 ����ͬ�ĵ�����Ϣ
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void Set_Debug_Level(uint32 level)
{
	rt_debug_level|=(0x1<<level);
//	rt_debug_level=level;
}

/*****************************************************************************
 * �� �� ��  : Get_Debug_Level
 * �� �� ��  : MZM
 * ��������  : 2016��5��16��
 * ��������  : ��ȡ���Լ������
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

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
 * �� �� ��  : TRC_COM_init_app
 * �� �� ��  : MZM
 * ��������  : 2016��5��16��
 * ��������  : ���ڽṹ��ʼ��
 * �������  : uint8 com  ���ں�
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

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
	tmpptr->mutex_sem_read_ok=xSemaphoreCreateMutex();//2015��12��11��18:12:53 �Ķ�ֵ�ź���
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
 * �� �� ��  : rt_kprintf
 * �� �� ��  : MZM
 * ��������  : 2016��5��16��
 * ��������  : �ɱ������Ϣ���
 * �������  : uint32_t flg     ������Ϣ��־
               const char *fmt  �ɱ����
               ...              �ɱ������Ϣ���� ���
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

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
			
				bytes = uart_send_data(rt_log_buf, length);//����
//				pUART=(LPC_USART_T *)Get_TRC_Com_Dev_Uart(0);
//				bytes = Chip_UART_SendRB(pUART, &Txring[0], rt_log_buf, length);//����
//			va_end(args);
	}
}

/*****************************************************************************
 * �� �� ��  : rt_kprintf
 * �� �� ��  : MZM
 * ��������  : 2016��5��16��
 * ��������  : �ɱ������Ϣ���
 * �������  : ��ʽ������ӿ�
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

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
		bytes = uart_send_data(rt_log_buf, length);//����

//		pUART=(LPC_USART_T *)Get_TRC_Com_Dev_Uart(0);
//		bytes = Chip_UART_SendRB(pUART, &Txring[0], rt_log_buf, length);//����
  //  va_end(args);
}

/*****************************************************************************
 * �� �� ��  : rt_pirntf_hex
 * �� �� ��  : MZM
 * ��������  : 2016��5��16��
 * ��������  : ��buf�����ݰ�16���ƴ�ӡ����
 * �������  : uint32_t flg  ������Ϣ��־
               uint8_t *buf  ��ʾ����
               uint16_t len  ��ʾ����
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

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
 * �� �� ��  : Send_Vtr_Up_Data
 * �� �� ��  : MZM
 * ��������  : 2016��5��16��
 * ��������  : �����ϱ�����
 * �������  : uint8 ch            �ϱ���Ϣͨ��
               uint8 *data_source  �ϱ�����
               uint16 length       �ϱ����ݳ���
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

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
		uart_send_data(Send_Potocol_buf, new_length);//����
	}
		
}

/****************************************************************************
* ���ƣ�Packet_Vtr_Up_Data()
* ���ܣ����Ѿ���õı���(lab+seq+cmd+data),֮����ϼ���У��cs2���ֽڣ�
		������õı��ķ���
* ��ڲ�����VTR_DEV_DATA *vtr_dev_data:vtr�豸�ṹ��
* ���ڲ���������0��ʾ�����ɹ�
****************************************************************************/
uint8 Packet_Vtr_Up_Data(VTR_PROTOCOL_DATA *vtr_dev_data)
{	
		UintType check_sum;//У���
		uint16 i;
		uint8 *temp_buf = (uint8*) vtr_dev_data->txd_fun.txd_buf;
		uint16 datalen = vtr_dev_data->txd_fun.tx_len;
		
		
		vtr_dev_data->txd_fun.cmd+=1;//��Ӧ����+1
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
* ���ƣ�Packet_Vtr_Up_Data()
* ���ܣ�һЩ��Ҫ�ȷ���Ӧ֡���ڴ�������������ﴦ��:��A4��������ŵ�������
* ��ڲ�����VTR_DEV_DATA *vtr_dev_data:vtr�豸�ṹ��
* ���ڲ���������0��ʾ�����ɹ�
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
* ���ƣ�Process_Vtr_Param()
* ���ܣ�Э�鴦��:�������������Ӧ֡.
* ��ڲ�����VTR_DEV_DATA *vtr_dev_data:vtr�豸�ṹ��
* ���ڲ���������:0x0:��Ӧ�����Ѿ���ã���ֱ�ӷ���
			����:0xff:��Ӧ֡δ��ã�����ȷ��֡
			����:0xfe:��Ӧ֡δ��ã��������֡ ��������--1
			����:0xfd:��Ӧ֡δ��ã��������֡ ��������--2
			����:0xfc:��Ӧ֡δ��ã��������֡ ��������--3
			����:0xfb:��Ӧ֡δ��ã��������֡ ��������--4
			����:0xfa:��Ӧ֡δ��ã��������֡ ��������--5
			����:0xf9:��Ӧ֡δ��ã��������֡ ��������--6
****************************************************************************/
uint8 Process_Vtr_Param(VTR_PROTOCOL_DATA *vtr_dev_data)
{
	uint8 ret=0;

	uint8 *recvdata =  &vtr_dev_data->rxd_buf[4];
	uint8 *databuf = vtr_dev_data->txd_fun.txd_buf;
	uint16 *datalen = &vtr_dev_data->txd_fun.tx_len;
	uint8	ch = vtr_dev_data->ch;
	
	switch(vtr_dev_data->txd_fun.cmd)//�����
	{
#if 0		
		case 0x01://���Ͳ�������
			*datalen = 1;
			vtr_dev_data->txd_fun.txd_buf[0] =vtr_edit_chnum;
			Vtr_Set_Link_Fun(ch,1);
			Vtr_Set_Link_Fun_Cnt(ch,0);
			ret = suc_respond;
			break;
		

		case 0x05://ѡ��������
			{
				ret=Vtr_Read_AccessoriesType_Info(databuf,sizeof(VTR_PARAM_F6));
				*datalen = sizeof(VTR_PARAM_F6);
			}
			break;
		case 0x07://�豸�̼���Ϣ
			{
				ret=Vtr_Read_Ch_IdInfo(databuf,sizeof(VTR_PARAM_F8));
				*datalen = sizeof(VTR_PARAM_F8);
			}
			break;
		case 0x09://��ѹ��������
			{
				ret=Vtr_Read_VoltageAlarm_Info(databuf,sizeof(VTR_PARAM_FA));
				*datalen = sizeof(VTR_PARAM_FA);
			}
			break;
		case 0x0b://�����¶ȱ�������
			{
				ret=Vtr_Read_PaTempAlarm_Info(databuf,sizeof(VTR_PARAM_FC));
				*datalen = sizeof(VTR_PARAM_FC);
			}
			break;
		case 0x0d://���书�ʱ�������
			{
				ret=Vtr_Read_TxPowerAlarm_Info((uint8*)&vtr_dev_data->txd_fun.txd_buf);
				*datalen = sizeof(VTR_PARAM_FE);
				break;
			}
		case 0x11://��ѯ������Ϣ��Դ
			{
				ret=Vtr_Read_AlarmSourceLed_Info(databuf,datalen);
			}
			break;
		case 0x13://4.18��0x13����VTR���Ͳ�ѯEEPROM��Ϣ
			{
				ret=Vtr_Read_Eeprom_Data(recvdata,databuf,datalen);
			}
			break;
		case 0x15://4.20��0x15,��VTR���Ͳ�ѯMCU flash��Ϣ
			{
				ret=Vtr_Read_Flash_Data(recvdata,databuf,datalen);
			}
			break;
			
		case 0x17://��ѯ���������Ϣ
			{
				ret=Vtr_Read_SoftWareSetPar_Info(databuf,datalen);
			}
			break;
		case 0x19://��ѯӲ��������Ϣ
			{
				ret=Vtr_Read_HardWarePar_Info(databuf,datalen);
			}
			break;
		case 0x1b://��ѯ��ʾ����Ϣ
			{
				ret=Vtr_Read_PwmTonepar_Info(databuf,datalen);
			}
			break;
		case 0x1d://��ѯ������������
			{
				ret=Vtr_Read_Appendix_Info(databuf,datalen);
			}
			break;
	
		case 0x21://��ѯѡ��������
			{
				ret=Vtr_Set_AccessoriesType_InfoPar(recvdata,databuf,datalen);
			}
			break;
		case 0x23://�����ŵ����ĵ�ǰͨ��
			{
				ret=Vtr_Set_Softwarech_No(ch,*recvdata,databuf,datalen);
				//*datalen=(uint16)0x1;
			}
			break;
			
		case 0x03://���ŵ���Ϣ
			{
				vtr_Read_CH_info(ch,recvdata,databuf,sizeof(VTR_PARAM_F4));
				*datalen = sizeof(VTR_PARAM_F4);
			}
			break;
		case 0x25://�����ŵ�����ͨ������
			{
				ret=Vtr_Set_Channel_InfoPar(ch,recvdata,databuf,datalen);
			}
			break;


		case 0x27://���÷��书�ʲ�����������
			{	
				ret=Vtr_Set_TxPowerAlarm_Info(recvdata,databuf,datalen);
			}
			break;
		case 0x29://���õ�ѹ������������
			{	
				ret=vtr_Set_VoltageAlarm_Info(recvdata,databuf,datalen);
			}
			break;
		case 0x2b://���ù����¶ȱ�����������Vtr_set_patempalarm_info
			{	
				ret=Vtr_Set_PatempAlarm_Info(recvdata,databuf,datalen);
			}
			break;
		case 0x2d://����VTR�������ñ����ܽźͱ����Ƶ������ԴVtr_set_alarmsourceled_info
			{	
				ret=Vtr_Set_AlarmSourceLed_Info(recvdata,databuf,datalen);
			}
			break;
			
		case 0x31://��VTR��������EEPROM��ϢVtr_write_eeprom_data
			{	
				ret=Vtr_Write_Eeprom_Data(recvdata,databuf,datalen);
			}
			break;
		case 0x33://��VTR��������MCU flash��Ϣ
			{	
				ret=Vtr_Write_Flash_Data(recvdata,databuf,datalen);
			}
			break;
		case 0x35://�������������ϢVtr_set_softwaresetpar_info
			{	
				ret=Vtr_Set_SoftWareSetpar_Info(recvdata,databuf,datalen);
			}
			break;
		case 0x37://������ʾ������Vtr_set_pwmtonepar_info
			{	
				ret=Vtr_Set_PwmTonepar_Info(recvdata,databuf,datalen);
			}
			break;
		case 0x39://���ø������ܽӿ�Vtr_set_appendix_info
			{	
				ret=Vtr_Set_Appendix_Info(recvdata,databuf,datalen);
			}
			break;
			
		case 0x3b://��VTR���ͻָ�Ĭ�ϳ������� ��ʱδʵ��
			//Resume_Factory_Defaults();
			break;
		case 0x3d://�Զ���ʾ��ǰͨ����̬��ϢVtr_dynamic_display_func
			{	
				ret=Vtr_Dynamic_Display_Func(ch,recvdata,databuf,datalen);
			}
			break;	
		case 0x41://�������ģʽVtr_Goahead_CH_Conner
			{	
				ret=Vtr_Goahead_CH_Conner(recvdata,databuf,datalen);
			}
			break;
		case 0x43://����PTT״̬Vtr_control_sptt
			{	
				ret=Vtr_Control_Sptt(recvdata,databuf,datalen);
			}
			break;
		case 0x45://���ƾ���״̬Vtr_control_mute
			{	
				ret=Vtr_Control_Mute(recvdata,databuf,datalen);
			}
			break;
		case 0x47://���Ʒ��ȹ���ģʽ
			{	
				ret=Vtr_Control_Fan(recvdata,databuf,datalen);
			}
			break;
		case 0x49://����У׼ģʽVtr_Goahead_CH_Calibrate
			{	
				
				ret=Vtr_Goahead_CH_Calibrate(recvdata,databuf,datalen);
	
			}
			break;
			
			#endif
#if 1
		case 0x4b:		//����У׼����A
		case 0x51:		//����У׼����B
		case 0x71:		//����У׼����C
		case 0x73:		//����У׼����D
		case 0xa5:		//����У׼����E
		case 0xa7:		//����У׼����F
		case 0xa9:		//����У׼����G
			
		case 0x55:		//��ǿУ׼����A
		case 0x57:		//��ǿУ׼����B
		case 0x75:		//��ǿУ׼����C
		case 0x77:		//��ǿУ׼����D
			
		case 0x5b:		//�¶�У׼����A
		case 0x5d:		//�¶�У׼����B
		case 0x79:		//�¶�У׼����C
		case 0x7b:		//�¶�У׼����D
			
		case 0x8b:		//����У׼����A
		case 0x8d:		//����У׼����A
			
		case 0x4d:		//��VTR���͹���У׼ָ��
		case 0x53:		//��VTR���ͳ�ǿУ׼ָ��
		case 0x59:		//��VTR���͹����¶�У׼ָ��
		case 0x6b:		//��VTR����������ƵУ׼ָ��
		case 0x6d:		//��VTR��������POT��λ��У׼ָ��
		case 0x89:		//��VTR���;���ȼ�У׼ָ��
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
		//TXУ׼����
		case 0x6b://����������ƵУ׼ָ��Vtr_set_voice_calibratepar
			{	
				ret=Vtr_Set_Voice_Calibratepar(recvdata,databuf,datalen);
			}
			break;
		case 0x6d://��������POT��λ��У׼ָ��Vtr_set_pot_calibratepar
			{	
				ret=Vtr_Set_Pot_Calibratepar(recvdata,databuf,datalen);
			}
			break;
			
		case 0x4b://���͹���У׼ָ��Vtr_send_power_calibratepar
			{	
				ret=Vtr_Send_Power_Calibratepar(recvdata,databuf,datalen);
			}
			break;
		case 0x4d://���͹���У׼ָ��Vtr_power_calibrate_abcdefg
			{	
				ret=Vtr_power_calibrate_abcdefg(vtr_dev_data->txd_fun.cmd,recvdata,databuf,datalen);
			}
			break;	
		case 0x51://���͹���У׼ָ��B
			{
				ret=Vtr_power_calibrate_abcdefg(vtr_dev_data->txd_fun.cmd,recvdata,databuf,datalen);
				//���㹦������
				calculate_power_graph(1,vtr_power_work_type);
			}
			break;
		case 0x71:
			{
				ret=Vtr_power_calibrate_abcdefg(vtr_dev_data->txd_fun.cmd,recvdata,databuf,datalen);
				//���㹦������
				calculate_power_graph(2,vtr_power_work_type);
			}
			break;
		case 0x73:
			{
				ret=Vtr_power_calibrate_abcdefg(vtr_dev_data->txd_fun.cmd,recvdata,databuf,datalen);
				//���㹦������
				calculate_power_graph(3,vtr_power_work_type);
			}
			break;
	
		case 0xa5:
			{
				ret=Vtr_power_calibrate_abcdefg(vtr_dev_data->txd_fun.cmd,recvdata,databuf,datalen);
				//���㹦������
				calculate_power_graph(4,vtr_power_work_type);
			}
			break;
		case 0xa7:
			{
				ret=Vtr_power_calibrate_abcdefg(vtr_dev_data->txd_fun.cmd,recvdata,databuf,datalen);
				//���㹦������
				calculate_power_graph(5,vtr_power_work_type);
			}		
		break;
		case 0xa9:
			{
				ret=Vtr_power_calibrate_abcdefg(vtr_dev_data->txd_fun.cmd,recvdata,databuf,datalen);
				//���㹦������
				calculate_power_graph(6,vtr_power_work_type);
			}
			break;					  
			
		case 0x53://���ͳ�ǿУ׼ָ��
			{
				ret=Vtr_Send_Rssi_Calibratepar(recvdata,databuf,datalen);
			}
			break;
		case 0x55://���ͳ�ǿУ׼ָ��A
			{
				ret=Vtr_Rssi_Calibrate_abcd(vtr_dev_data->txd_fun.cmd,recvdata,databuf,datalen);
			}
			break;
		case 0x57://���ͳ�ǿУ׼ָ��B
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
		case 0x59://���͹����¶�У׼ָ��
			{
				ret =Vtr_Send_Temperature_Calibrate(recvdata,databuf,datalen);
			}
			break;
		case 0x5b://���͹����¶�У׼ָ��A
			{
				ret=Vtr_Temperature_Calibrate_abcd(vtr_dev_data->txd_fun.cmd,recvdata,databuf,datalen);
			}
			break;
		case 0x5d://���͹����¶�У׼ָ��B
			{
				ret=Vtr_Temperature_Calibrate_abcd(vtr_dev_data->txd_fun.cmd,recvdata,databuf,datalen);
				calculate_temperature_graph(1);
			}
			break;		
		case 0x79://���͹����¶�У׼ָ��c
			{
				ret=Vtr_Temperature_Calibrate_abcd(vtr_dev_data->txd_fun.cmd,recvdata,databuf,datalen);
				calculate_temperature_graph(2);
			}
			break;
		case 0x7b://���͹����¶�У׼ָ��D
			{
				ret=Vtr_Temperature_Calibrate_abcd(vtr_dev_data->txd_fun.cmd,recvdata,databuf,datalen);
				calculate_temperature_graph(3);
			}	
			break;
		case 0x89://4.96����VTR���;���ȼ�У׼ָ�У׼ģʽ�£�
			{
				ret =Vtr_Send_Mute_Calibratepar(recvdata,databuf,datalen);
			}
			break;
		case 0x8b://4.98����VTR���;���ȼ�У׼ָ��A��У׼ģʽ�£�
			{
				ret=Vtr_Mute_Calibrate_ab(vtr_dev_data->txd_fun.cmd,recvdata,databuf,datalen);
			}
			break;
		case 0x8d://4.100����VTR���;���ȼ�У׼ָ��B��У׼ģʽ�£�
			{
				ret=Vtr_Mute_Calibrate_ab(vtr_dev_data->txd_fun.cmd,recvdata,databuf,datalen);
				calculate_mute_graph(1,vtr_mute_work_type);
			}
			break;
	#endif
		
		
		
		#if 0
			case 0x61://ģ�⿪������Vtr_set_switch_state
			{
				ret =Vtr_Set_Switch_State(recvdata,databuf,datalen);
			}
			break;
		case 0x63://4.88��0x63��VTR����LED����ָ��Vtr_set_led_state
			{
				ret =Vtr_Set_Led_State(recvdata,databuf,datalen);
			}
			break;
		case 0x65://����У׼״̬��ӦVtr_Set_TestVoice_Fre
			{
				ret =Vtr_Set_TestVoice_Fre_new(recvdata,databuf,datalen);
			}
			break;
		case 0x67://VTR��������POT��λ������ָ��
			{
				//set_potvalue();
				ret = 0xFF;
			}
			break;
			
		case 0x69://��VTR��������Ӳ����Ϣ����Ƶ����Vtr_set_hardwarepar_info
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
	
		case 0xa1://4.140��0xa1  ��VTR���������ļ��������ͨѶ����115200���ļ���ʽ *.bin��
			{
				ret = Vtr_Pc_Updata_UserCode(vtr_dev_data);//�����û�����
			}
			break;
		case 0xa3://4.142��0xa3  ��VTR����ǿ��TRC�����г���������			
			{
				ret =vtr_Appoint_UserCode_Info(recvdata,databuf,datalen);
			}
			break;	
			
			/*******************************��������ΪCHC��������*********************************/
			/******************************����ch�Ƿ�������pc���в���*****************************/
			
		case 0xad://4.152��PDT���ư��ѯTRC���ŵ�����Ϣ
			{
				ret=Vtr_Chc_Read_TRC_Info(ch,recvdata,databuf,datalen);
			}
			break;
		case 0xaf://4.154��PDT���ư���TRC������������
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
			/*******************************��������ΪVSC��������*********************************/
			/******************************����ch�Ƿ�������pc���в���*****************************/
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




