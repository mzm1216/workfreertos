
#define RTUDEV_GLOBALS
#include "rtudev.h"
#include "sky72310.h"

/*	?????????????????,??????????,
????????,????????????
???????????????,?????????,??????,
?????????????????0-3????,??????????*/
 FREQ_POWTYPE_GRAPH_STRUCT freq_powtype_graph[MAXNUMPPOT]=
{
		{0,0,361-FREQ_NUM_ADDEND},//???? ?? 361,?????FREQ_NUM_ADDEND ???????????????
		{1,1,363-FREQ_NUM_ADDEND},
		{2,2,365-FREQ_NUM_ADDEND},
		{3,3,367-FREQ_NUM_ADDEND},
		
		{4,4,361-FREQ_NUM_ADDEND},
		{5,5,361-FREQ_NUM_ADDEND},
		{6,6,361-FREQ_NUM_ADDEND},
		{7,7,361-FREQ_NUM_ADDEND},
		{8,8,361-FREQ_NUM_ADDEND},
		{9,9,361-FREQ_NUM_ADDEND},
		{10,10,361-FREQ_NUM_ADDEND},
		{11,11,361-FREQ_NUM_ADDEND},
		{12,12,361-FREQ_NUM_ADDEND},
		{13,13,361-FREQ_NUM_ADDEND},
		{14,14,361-FREQ_NUM_ADDEND},
		{15,15,361-FREQ_NUM_ADDEND},
		{16,16,361-FREQ_NUM_ADDEND},
		{17,17,361-FREQ_NUM_ADDEND},
		{18,18,361-FREQ_NUM_ADDEND},
		{19,19,361-FREQ_NUM_ADDEND}
		//{0xff,0xff-FREQ_NUM_ADDEND}
};
  CALIBRATE_CMD_GRAPH_STRUCT Calibrate_CMD_Graph[CALIBRATE_CMD_NUM]=
{
		//fn,	paramlen, cmdoffset, acountkcflg, first,	second
		//??,????,????, ????,   ????,????
		{0x4d,2,PARAM_OFFSET(VTR_VALUE_A_G_HL,Value_a),1,0,0},			//??????A
		{0x51,2,PARAM_OFFSET(VTR_VALUE_A_G_HL,Value_b),1,2000,5000},	//??????B
		{0x71,2,PARAM_OFFSET(VTR_VALUE_A_G_HL,Value_c),1,5000,10000},	//??????C
		{0x73,2,PARAM_OFFSET(VTR_VALUE_A_G_HL,Value_d),1,10000,20000},	//??????D
		{0xa5,2,PARAM_OFFSET(VTR_VALUE_A_G_HL,Value_e),1,20000,30000},	//??????E
		{0xa7,2,PARAM_OFFSET(VTR_VALUE_A_G_HL,Value_f),1,30000,40000},	//??????F
		{0xa9,2,PARAM_OFFSET(VTR_VALUE_A_G_HL,Value_g),1,40000,50000},	//??????G
		
		{0x55,2,PARAM_OFFSET(VTR_CALIBRATE_RSSI_STRCUT,wbValue_a),2,0,0},			//??????A
		{0x57,2,PARAM_OFFSET(VTR_CALIBRATE_RSSI_STRCUT,wbValue_b),2,-70,-85},		//??????B
		{0x75,2,PARAM_OFFSET(VTR_CALIBRATE_RSSI_STRCUT,wbValue_c),2,-85,-100},	//??????C
		{0x77,2,PARAM_OFFSET(VTR_CALIBRATE_RSSI_STRCUT,wbValue_d),2,-100,-115},	//??????D
		
		{0x5b,2,PARAM_OFFSET(VTR_CALIBRATE_TEMP_STRCUT,tempValue_a),3,0,0},			//??????A
		{0x5d,2,PARAM_OFFSET(VTR_CALIBRATE_TEMP_STRCUT,tempValue_b),3,0,0},			//??????B
		{0x79,2,PARAM_OFFSET(VTR_CALIBRATE_TEMP_STRCUT,tempValue_c),3,0,0}, 		//??????C
		{0x7b,2,PARAM_OFFSET(VTR_CALIBRATE_TEMP_STRCUT,tempValue_d),3,0,0}, 		//??????D

		{0x8b,2,PARAM_OFFSET(VTR_CALIBRATE_MUTE_STRCUT,wbValue_a),4,0,0},			//??????A
		{0x8d,2,PARAM_OFFSET(VTR_CALIBRATE_MUTE_STRCUT,wbValue_b),4,0,0},			//??????B

		
		{0x4b,0,PARAM_OFFSET(VTR_CALIBRATE_SYSTEM_PARAM,PowerValue),0,0,0}, 		//?VTR????????
		{0x53,0,PARAM_OFFSET(VTR_CALIBRATE_SYSTEM_PARAM,RssiValue),0,0,0},		//?VTR????????
		{0x59,0,PARAM_OFFSET(VTR_CALIBRATE_SYSTEM_PARAM,TempValue),0,0,0},		//?VTR??????????
		{0x6b,10,PARAM_OFFSET(VTR_CALIBRATE_SYSTEM_PARAM,VoiceValue),5,0,0},		//?VTR??????????
		{0x6d,8,PARAM_OFFSET(VTR_CALIBRATE_SYSTEM_PARAM,PowerValue),6,0,0},		//?VTR????POT???????
		{0x89,0,PARAM_OFFSET(VTR_CALIBRATE_SYSTEM_PARAM,MuteValue),0,0,0},		//?VTR??????????



};


RTU_SAVE_PARAM_GRAPH_T   rtu_save_param_graph[] = 
{
	/* 参数标识码，起始页面，结束页面，扇区号,结构中偏移，保存参数大小*/
	{SYS_PARAM_FLAGS,		0,	0,	IAP_LAST_SECTOR,PARAM_OFFSET(RTU_PARAM_SAVE_FLASH,sys_param_flags),		256},		//系统参数标志
	{SYS_PARAM_DAC_MOD,	1,	1,	IAP_LAST_SECTOR,PARAM_OFFSET(RTU_PARAM_SAVE_FLASH,sys_param_dac_mod),	256},		//DAC)MOD参数
	{SYS_PARAM_EXT_CLK,	2,	2,	IAP_LAST_SECTOR,PARAM_OFFSET(RTU_PARAM_SAVE_FLASH,sys_param_ext_clk),		256},		//外部时钟参数
	{SYS_PARAM_FREQ,		3,	3,	IAP_LAST_SECTOR,PARAM_OFFSET(RTU_PARAM_SAVE_FLASH,sys_param_freq),		256},		//频率参数
};
#define RTU_SAVE_PARAM_MAX_NUM		(sizeof(rtu_save_param_graph)/sizeof(RTU_SAVE_PARAM_GRAPH_T))

/*****************************************************************************
 * 函 数 名  : Get_system_param_offset_addr
 * 负 责 人  : MZM
 * 创建日期  : 2016年7月4日
 * 函数功能  : 通过参数标识码  返回保存的页面地址的偏移地址
 * 输入参数  : uint8_t param_flg  参数标识码
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
uint32_t Get_system_param_offset_addr(uint8_t param_flg)
{
	if(param_flg < RTU_SAVE_PARAM_MAX_NUM)
	{
		return rtu_save_param_graph[param_flg].start_page *SYS_PARAM_PAGE_SIZE;
	}
}
/*****************************************************************************
 * 函 数 名  : Get_system_param_start_page
 * 负 责 人  : MZM
 * 创建日期  : 2016年7月4日
 * 函数功能  : 通过参数标识码  返回保存的起始页面地址的偏移地址
 * 输入参数  : uint8_t param_flg  参数标识码
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
uint32_t Get_system_param_start_page(uint8_t param_flg)
{
	if(param_flg < RTU_SAVE_PARAM_MAX_NUM)
	{
		return rtu_save_param_graph[param_flg].start_page + SYS_PARAM_BASE_PAGES;
	}
	return 2047;
}
/*****************************************************************************
 * 函 数 名  : Get_system_param_end_page
 * 负 责 人  : MZM
 * 创建日期  : 2016年7月4日
 * 函数功能  : 通过参数标识码  返回保存的结束页面地址的偏移地址
 * 输入参数  : uint8_t param_flg  参数标识码
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
uint32_t Get_system_param_end_page(uint8_t param_flg)
{
	if(param_flg < RTU_SAVE_PARAM_MAX_NUM)
	{
		return rtu_save_param_graph[param_flg].end_page + SYS_PARAM_BASE_PAGES;
	}
	return 2047;
}
/*****************************************************************************
 * 函 数 名  : Get_system_param_offset_t
 * 负 责 人  : MZM
 * 创建日期  : 2016年7月4日
 * 函数功能  : 通过参数标识码  返回保存的参数在整个系统参数结构体中的偏移
 * 输入参数  : uint8_t param_flg  参数标识码
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
uint32_t Get_system_param_offset_t(uint8_t param_flg)
{
	if(param_flg < RTU_SAVE_PARAM_MAX_NUM)
	{
		return rtu_save_param_graph[param_flg].offset_t;
	}
	return 2047;
}

/*****************************************************************************
 * 函 数 名  : Get_system_param_size_t
 * 负 责 人  : MZM
 * 创建日期  : 2016年7月4日
 * 函数功能  : 通过参数标识码  返回保存的参数保存大小
 * 输入参数  : uint8_t param_flg  参数标识码
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
uint32_t Get_system_param_size_t(uint8_t param_flg)
{
	if(param_flg < RTU_SAVE_PARAM_MAX_NUM)
	{
		return rtu_save_param_graph[param_flg].size_t;
	}
	return 2047;
}


/*****************************************************************************
 * 函 数 名  : RTU_Save_SYS_Param_By_Param_Flg
 * 负 责 人  : MZM
 * 创建日期  : 2016年7月4日
 * 函数功能  : 通过参数标识码  保存参数
 * 输入参数  : uint8_t param_flg  参数标识码
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void RTU_Save_SYS_Param_By_Param_Flg(uint8_t param_flg)
{
	uint8_t *data = (uint8_t *) &rtudev->saveparam;
	uint32_t size = Get_system_param_size_t(param_flg);
	
	data += Get_system_param_offset_t(param_flg);
//	Rtu_Save_Data_To_Flash_Page(Get_system_param_start_page(param_flg),Get_system_param_end_page(param_flg),data, size);

}

/*****************************************************************************
 * 函 数 名  : RTU_Save_All_Param
 * 负 责 人  : MZM
 * 创建日期  : 2016年7月4日
 * 函数功能  : 保存所有餐数据
 * 输入参数  : uint8_t param_flg  参数标识码
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void RTU_Save_All_Param()
{
	uint32_t i;
	
	for(i=0;i<RTU_SAVE_PARAM_MAX_NUM;i++)
	{
		RTU_Save_SYS_Param_By_Param_Flg(i);
	}
}
/****************************************************************************
* ??:Calibrate_Get_Powtype_graph_by_powertype-
* ??:??????????,?????????
* ????:uint8 powertype,????
			FREQ_POWERTYPE_GRAPH *tmpgraph ??????
* ????:suc_respond ???fail_respond??
****************************************************************************/
uint8 Calibrate_Get_Powtype_graph_by_powertype(uint8 powertype,FREQ_POWERTYPE_GRAPH *tmpgraph)
{
	uint8 i,freqno,saveoffset;

	if(powertype <MAXNUMPPOT)
	{
		memcpy(tmpgraph,&freq_powtype_graph[powertype].freqgraph,sizeof(FREQ_POWERTYPE_GRAPH));
		return	suc_respond;//????
	}

	return fail_respond;//????
}

/****************************************************************************
* ??:Calibrate_Get_Powtype_graph_by_powertype-
* ??:????????????,?????????
* ????:uint8 powertype,????
			FREQ_POWERTYPE_GRAPH *tmpgraph ??????
* ????:suc_respond ???fail_respond??
****************************************************************************/
uint8 Calibrate_Get_Powtype_graph_by_offset(uint8 offset,FREQ_POWERTYPE_GRAPH *tmpgraph)
{
	uint8 i,freqno,saveoffset;

	for(i=0;i<MAXNUMPPOT;i++)
	{
		if(offset == freq_powtype_graph[i].freqgraph.saveoffset)//??????
		{
			memcpy(tmpgraph,&freq_powtype_graph[i].freqgraph,sizeof(FREQ_POWERTYPE_GRAPH));
			return	suc_respond;//????
		}
	}

	return fail_respond;//????
}

/****************************************************************************
* ??:Calibrate_Get_Powtype_graph_by_powertype-
* ??:??????????,??????? ????+300
* ????:uint8 powertype,????
			FREQ_POWERTYPE_GRAPH *tmpgraph ??????
* ????:suc_respond ???fail_respond??
****************************************************************************/
uint8 Calibrate_Get_Freqnum_By_Powtype(uint8 powertype)
{
	uint8 i;
	for(i=0;i<MAXNUMPPOT;i++)
	{
		if(powertype == freq_powtype_graph[i].freqgraph.powertype)//?????
		{
			return 	freq_powtype_graph[i].freqgraph.freqnum;//????
		}
	}

	return 0xff;//????
}

/**/
/****************************************************************************
* ??:Calibrate_Get_Graph_Fn-
* ??:???????Fn????????????
* ????:uint8 offset,????
			FREQ_POWERTYPE_GRAPH *tmpgraph ??????
* ????:?ff???0xff??
****************************************************************************/
uint8 Calibrate_Get_Graph_Fn(uint8 offset)
{
	uint8 i;
	if(offset<CALIBRATE_CMD_NUM)
	{
		return Calibrate_CMD_Graph[offset].calibratecmd.cmdfn;
	}

	return 0xff;//????
}

/**/
/****************************************************************************
* ??:Calibrate_Get_SaveOffset_By_Powtype-
* ??:???????????????
* ????:uint8 offset,????
* ????:?ff???0xff??
****************************************************************************/
uint8 Calibrate_Get_SaveOffset_By_Powtype(uint8 powertype)
{
	uint8 i;
	for(i=0;i<MAXNUMPPOT;i++)
	{
		if(powertype == freq_powtype_graph[i].freqgraph.powertype)//?????
		{
			return 	freq_powtype_graph[i].freqgraph.saveoffset;//??????
		}
	}

	return 0xff;//????
}

/****************************************************************************
* ??:Calibrate_Get_ParamLen_By_Fn-
* ??:???????Fn????????????
* ????:uint8 fn,??? 
* ????:?ff???0xff??
****************************************************************************/
uint8 Calibrate_Get_ParamLen_By_Fn(uint8 fn)
{
	uint8 i;

//	mzm_kprintf(12,"fn1",Calibrate_CMD_Graph->calibratecmd[0].cmdfn,"fn2",Calibrate_CMD_Graph->calibratecmd[1].cmdfn,1);
//	mzm_kprintf(13,"fn3",Calibrate_CMD_Graph->calibratecmd[2].cmdfn,"fn4",Calibrate_CMD_Graph->calibratecmd[3].cmdfn,1);
//	mzm_kprintf(3,"fn7",Calibrate_CMD_Graph->calibratecmd[6].cmdfn,"fn8",fn,1);
	for(i=0;i<CALIBRATE_CMD_NUM;i++)
	{
		if(fn == Calibrate_CMD_Graph[i].calibratecmd.cmdfn)//?????
		{
			return 	Calibrate_CMD_Graph[i].calibratecmd.paramlen;//??????
		}
	}

	return 0xff;//????
}

/**/
/****************************************************************************
* ??:Calibrate_Get_CMD_Offset_By_Fn-
* ??:???????Fn?? ?????????????,
* ????:uint8 fn,??? 
* ????:??ff,?????,?ff,????
****************************************************************************/
uint8 Calibrate_Get_CMD_Offset_By_Fn(uint8 fn)
{
	uint8 i;
	for(i=0;i<CALIBRATE_CMD_NUM;i++)
	{
		if(fn == Calibrate_CMD_Graph[i].calibratecmd.cmdfn)//?????
		{
			return 	Calibrate_CMD_Graph[i].calibratecmd.cmdoffset;//??????
		}
	}

	return 0xff;//????
}
/**/
/****************************************************************************
* ??:Calibrate_Get_AcountKC_FLG_By_Fn-
* ??:???????Fn?? ??KC???,
* ????:uint8 fn,??? 
* ????:???0,????KC,??0 ?????KC
****************************************************************************/

uint8 Calibrate_Get_AcountKC_FLG_By_Fn(uint8 fn)
{
	uint8 i;
	for(i=0;i<CALIBRATE_CMD_NUM;i++)
	{
		if(fn == Calibrate_CMD_Graph[i].calibratecmd.cmdfn)//?????
		{
			return Calibrate_CMD_Graph[i].calibratecmd.acountKCflg;//????
		}
	}

	return 0;//????
}

/**/
/****************************************************************************
* ??:Calibrate_Get_AcountKC_FirstValue_By_Fn-
* ??:???????Fn?? ??KC???????,
* ????:uint8 fn,??? 
* ????:ffff??,?ffff ????
****************************************************************************/

uint16 Calibrate_Get_AcountKC_FirstValue_By_Fn(uint8 fn)
{
	uint8 i;
	for(i=0;i<CALIBRATE_CMD_NUM;i++)
	{
		if(fn == Calibrate_CMD_Graph[i].calibratecmd.cmdfn)//?????
		{
			return 	Calibrate_CMD_Graph[i].calibratecmd.firstvalue;//????
		}
	}

	return 0xffff;//??
}

/****************************************************************************
* ??:Calibrate_Get_AcountKC_SecondValue_By_Fn-
* ??:???????Fn?? ??KC???????,
* ????:uint8 fn,??? 
* ????:ffff??,?ffff ????
****************************************************************************/
uint16 Calibrate_Get_AcountKC_SecondValue_By_Fn(uint8 fn)
{
	uint8 i;
	for(i=0;i<CALIBRATE_CMD_NUM;i++)
	{
		if(fn == Calibrate_CMD_Graph[i].calibratecmd.cmdfn)//?????
		{
			return 	Calibrate_CMD_Graph[i].calibratecmd.secondvalue;//????
		}
	}

	return 0xffff;//??
}

/****************************************************************************
* ??:Vtr_Fail_Return_Respond-
* ??:????????,????????????
* ????:uint8 ch,???:ch_pc ????,ch_chc?????
			uint8 ret_flg ????
#define RET_FAIL_F0		(0xFF-suc_respond)	//????.
#define RET_FAIL_F1		(0xFF-fail_type1)	//????:	??????
#define RET_FAIL_F2		(0xFF-fail_type2)	//????:	?EEPROM??
#define RET_FAIL_F3		(0xFF-fail_type3)	//????:	?FLASH??
#define RET_FAIL_F4		(0xFF-fail_type4)	//????:	???????,?????
#define RET_FAIL_F5		(0xFF-fail_type5)	//????:	????
#define RET_FAIL_F6		(0xFF-fail_type6)	//????:	??CRC????
#define RET_FAIL_FNULL	0xAA	//??chc????,????--0xf6

* ????:?
****************************************************************************/
void Vtr_Fail_Return_Respond(VTR_DEV_DATA *vtrdev,uint8 ret_flg)
{
	uint8 *temp_buf;//??????
	uint8 send_cmd;//????
	uint32 i;//????
	UintType check_sum;//???
	uint16 send_ptr;//????
	uint8 respond_state;//????

	temp_buf = vtrdev->txd_fun.txd_buf;

	send_ptr = 0;
	if(ret_flg==RET_FAIL_F0)
	{
		*(temp_buf+(send_ptr++))=suc_respond;//????
		*(temp_buf+(send_ptr++))=suc_type;//????
	}
	
	else if(ret_flg==RET_FAIL_FNULL)
	{
		return;//???? 
	}
	else
	{
		*(temp_buf+(send_ptr++))=fail_respond;//????
		*(temp_buf+(send_ptr++))=0xFF-ret_flg;//????
	}	

	if(vtrdev->txd_fun.cmd == 0xA1)//?????????????
	{
//		*(temp_buf+(send_ptr++))=(uint8)(updata_user_sequence/0x1000000);
//		*(temp_buf+(send_ptr++))=(uint8)(updata_user_sequence/0x10000);
//		*(temp_buf+(send_ptr++))=(uint8)(updata_user_sequence/0x100);
//		*(temp_buf+(send_ptr++))=(uint8)updata_user_sequence;
	}
	vtrdev->txd_fun.tx_len = send_ptr;//????

	Packet_Vtr_Up_Data(vtrdev);// ????

}


/**/
/****************************************************************************
* 名称：Vtr_Check_Freq_get_saveoffset
* 功能：通过传入频率，从表中查找该频率是哪个区间段的，即 
	在频率列表中找到最适合传入频率的点的KC,并返回该点的存储位置
* 入口参数：saveoffset 校准结构体中，增益参数的偏移位置
* 出口参数：五
****************************************************************************/
uint8 Vtr_Check_Freq_get_saveoffset(uint32 freq)
{
	uint8 i,saveoffset=0xff;
	FREQ_POWERTYPE_GRAPH tmpgraph;
	uint32 freqparam;
	

	for(i=0;i<MAXNUMPPOT;i++)
	{
		if(Calibrate_Get_Powtype_graph_by_offset(i,&tmpgraph)==fail_respond)//不存在该位置的数据，返回上次的位置
		{
			return saveoffset;
		}
		else
		{//找到该位置的数据信息
			freqparam=tmpgraph.freqnum+FREQ_NUM_ADDEND;//获得实际的频率
			freqparam*=1000000;//频率倍率
			if(freq>=freqparam)//传入频率，大于该位置处的频，继续往后查找，直到找到一个区间，存在关系如下:saveoffset的freq<freq<saveoffset+1.freq
			{
				saveoffset=i;
				continue;
			}
			else
			{
				break;//传入频率 小于该位置的频率，不用继续查找了
			}
		}
	}

	return saveoffset;
}

void change_fwdpowermv_to_fwdpowerw_361_367M_byw(uint16* dest_w,uint16 ad_value_w,UinthlType *tmp_36xM,fp64 *k1,fp64 *c1)
{
	fp64	temp_fwd_power;
	uint16 temp_fwd_power_data;
	UinthlType temp_fwd_power_hl;
	uint16 fwd_power_hl_b_361M;
	uint16 fwd_power_hl_c_361M;
	uint16 fwd_power_hl_d_361M;
	uint16 fwd_power_hl_e_361M;
	uint16 fwd_power_hl_f_361M;
	uint16 fwd_power_hl_g_361M;    
	
	fwd_power_hl_b_361M=(*(tmp_36xM)).w;
	fwd_power_hl_c_361M=(*(tmp_36xM+1)).w;
	fwd_power_hl_d_361M=(*(tmp_36xM+2)).w;
	fwd_power_hl_e_361M=(*(tmp_36xM+3)).w;
	fwd_power_hl_f_361M=(*(tmp_36xM+4)).w;
	fwd_power_hl_g_361M=(*(tmp_36xM+5)).w;
	    
	if(ad_value_w>0)  //500mw 的值
	{
		if(ad_value_w<=fwd_power_hl_b_361M)
		{
			temp_fwd_power=ad_value_w*(*k1)+(*c1);
		}
		else
		{
			if(ad_value_w<=fwd_power_hl_c_361M)
			{
				temp_fwd_power=ad_value_w*(*(k1+2))+(*(c1+2));			
			}
			else
			{
				if(ad_value_w<=fwd_power_hl_d_361M)
				{
					temp_fwd_power=ad_value_w*(*(k1+4))+(*(c1+4));			
				}
				else
				{
                    if(ad_value_w<=fwd_power_hl_e_361M)
                    {
                        temp_fwd_power=ad_value_w*(*(k1+6))+(*(c1+6));							
                    }
    				else
    				{
                        if(ad_value_w<=fwd_power_hl_f_361M)
                        {
                            temp_fwd_power=ad_value_w*(*(k1+8))+(*(c1+8));							
                        }
                        else
                        {                        
                            temp_fwd_power=ad_value_w*(*(k1+10))+(*(c1+10));							
                        }
                    }						
				}
			}
		}
	}
	else
	{
		temp_fwd_power=0;
	}
	
	if(temp_fwd_power>0)
	{
		temp_fwd_power_data=(uint16)temp_fwd_power;
		temp_fwd_power_hl.whl.high=(uint8)(temp_fwd_power_data/1000);// 47w
		temp_fwd_power_hl.whl.low=(uint8)(temp_fwd_power_data/10%100); //0.51w
		*dest_w=temp_fwd_power_hl.w;
	}
	else
	{
		*dest_w=0;
	}

	
}


//fwdflg ； 61==361M  63=363M 65=365M  67=367M
//vtr_power_kc.tx_fwdpower_361M_k1表示1个AD采样值 对应功率值是几W

/****************************************************************************
* 名称：Vtr_change_fwdpowermv_to_fwdpowerw_saveoffset
* 功能：根据当前的前向功率和反向功率AD采样值，计算出对应的功率值。
//并将转换的值，写入当前信道参数中，前向功率、反向功率
* 入口参数：saveoffset 校准结构体中，增益参数的偏移位置
* 出口参数：五
****************************************************************************/
void Vtr_change_fwdpowermv_to_fwdpowerw_saveoffset(uint8 saveoffset)
{
	fp64	temp_fwd_power;
	uint16 temp_fwd_power_data;
	UinthlType temp_fwd_power_hl;
	uint16 fwd_power_hl_b_361M;
	uint16 fwd_power_hl_c_361M;
	uint16 fwd_power_hl_d_361M;
	uint16 fwd_power_hl_e_361M;
	uint16 fwd_power_hl_f_361M;
	uint16 fwd_power_hl_g_361M;    

	fp64	temp_refl_power;
	uint16 temp_refl_power_data;
	UinthlType temp_refl_power_hl;
	UinthlType *tmp_36xM;
	fp64 *k1,*c1;
//	uint8 saveoffset;	
	VTR_CALIBRATE_KC_STRCUT *calibrate_KC_strcut = Get_RTU_dev_ptr_KC_struct();
	VTR_CALIBRATE_SYSTEM_PARAM *calibrate_sys_param = Get_RTU_dev_ptr_sys_param();
	VTR_PARAM_UNION_F4 * vtr_ch_info_current = Get_RTU_dev_ptr_current_chinfo();
	RTU_PARAM_NOT_SAVE_ADVALUE *Vtr_AdcValue =  Get_RTU_dev_ptr_Adc_value();


	if(saveoffset ==0xFF)
	{
		return ;
	}
	
	tmp_36xM=&calibrate_sys_param->PowerValue[saveoffset].FWD_Value.Value_b;//.txpar.fwd_power_b_361M;
	k1=(fp64 *)&calibrate_KC_strcut->Fwd_KC_Value[saveoffset].KC_k1;//&vtr_power_kc.tx_fwdpower_361M_k1;
	c1=(fp64 *)&calibrate_KC_strcut->Fwd_KC_Value[saveoffset].KC_c1;

//y=(kvalue) * x + c  .	x为电压(mv)，y为功率(mw)
	change_fwdpowermv_to_fwdpowerw_361_367M_byw((uint16*)&vtr_ch_info_current->vtrchannelinfo.fwdp.propertyintfrac.w,Vtr_AdcValue->Advalue[PV_FWD_ADC_CH].w,tmp_36xM,k1,c1);
	change_fwdpowermv_to_fwdpowerw_361_367M_byw((uint16*)&vtr_ch_info_current->vtrchannelinfo.refp.propertyintfrac.w,Vtr_AdcValue->Advalue[PV_REF_ADC_CH].w,tmp_36xM,k1,c1);


}

/****************************************************************************
* 名称：Vtr_ADValue_Data_Deal_Step2
* 功能：AD转换完成后第二阶段，根据当前信道的频率，查表找到其存储位置，从而找到
		校准结构体中的校准参数的位置，并根据当前前向、反向功率AD0值，计算出
		前向、反向功率值，保存到当前信道参数结构体重
* 入口参数：freq:当前信道的频率
* 出口参数：五
****************************************************************************/
void Vtr_ADValue_Data_Deal_Step2(uint32 freq)
{
	uint8 saveoffset;
	
	saveoffset = Vtr_Check_Freq_get_saveoffset(freq);//获取该频率区间的点的存储位置
	
	if(saveoffset !=0xff)
	{
		Vtr_change_fwdpowermv_to_fwdpowerw_saveoffset(saveoffset);//根据存储位置进行反向功率、前向功率的计算
	}

	return;

}

/****************************************************************************
* 名称：Vtr_ADValue_Data_Deal_Step1
* 功能：AD转换完成后第一阶段，场强的处理时调用，根据当前场强强度和校准命令计算的KC值
		，来计算场强强度，并保存到当前信道参数结构体重,其中窄带和PDT模式采用
		窄带接收。
* 入口参数：wbnbflg  :宽带 0，窄带 1，PDT 2
* 出口参数：五
****************************************************************************/
void change_rssimv_to_rssidbm_wbnb(uint8 wbnbflg)
{
	fp64	temp_rssi_power;
	uint16 temp_rssi_power_data;
	UinthlType temp_rssi_power_hl;
	uint16 rssi_power_hl_a_nb;
	uint16 rssi_power_hl_b_nb;
	uint16 rssi_power_hl_c_nb;
	uint16 rssi_power_hl_d_nb;
	fp64 rx_rssi_nb_k1;
	fp64 rx_rssi_nb_c1;
	fp64 rx_rssi_nb_k2;
	fp64 rx_rssi_nb_c2;
	fp64 rx_rssi_nb_k3;
	fp64 rx_rssi_nb_c3;		
	RTU_PARAM_NOT_SAVE_ADVALUE *Vtr_AdcValue =  Get_RTU_dev_ptr_Adc_value();
	VTR_CALIBRATE_KC_STRCUT *calibrate_KC_strcut = Get_RTU_dev_ptr_KC_struct();
	VTR_CALIBRATE_SYSTEM_PARAM *calibrate_sys_param = Get_RTU_dev_ptr_sys_param();
	VTR_PARAM_UNION_F4 * vtr_ch_info_current = Get_RTU_dev_ptr_current_chinfo();
//y=(kvalue) * x + c  .	x为电压(mv)，y为功率(mw)

	if(wbnbflg==1 &&wbnbflg==2)//窄带和PDT
	{
		rssi_power_hl_a_nb=calibrate_sys_param->RssiValue.rssi_mv_a_nb.w;
		rssi_power_hl_b_nb=calibrate_sys_param->RssiValue.rssi_mv_b_nb.w;
		rssi_power_hl_c_nb=calibrate_sys_param->RssiValue.rssi_mv_c_nb.w;
		rssi_power_hl_d_nb=calibrate_sys_param->RssiValue.rssi_mv_d_nb.w;
		
		rx_rssi_nb_k1=calibrate_KC_strcut->Rssi_KC_Value.rx_rssi_nb_k1;
		rx_rssi_nb_c1=calibrate_KC_strcut->Rssi_KC_Value.rx_rssi_nb_c1;
		rx_rssi_nb_k2=calibrate_KC_strcut->Rssi_KC_Value.rx_rssi_nb_k2;
		rx_rssi_nb_c2=calibrate_KC_strcut->Rssi_KC_Value.rx_rssi_nb_c2;		
		rx_rssi_nb_k3=calibrate_KC_strcut->Rssi_KC_Value.rx_rssi_nb_k3;
		rx_rssi_nb_c3=calibrate_KC_strcut->Rssi_KC_Value.rx_rssi_nb_c3;

		
	}
	else
		{
		rssi_power_hl_a_nb=calibrate_sys_param->RssiValue.rssi_mv_a_wb.w;
		rssi_power_hl_b_nb=calibrate_sys_param->RssiValue.rssi_mv_b_wb.w;
		rssi_power_hl_c_nb=calibrate_sys_param->RssiValue.rssi_mv_c_wb.w;
		rssi_power_hl_d_nb=calibrate_sys_param->RssiValue.rssi_mv_d_wb.w;
		
		rx_rssi_nb_k1=calibrate_KC_strcut->Rssi_KC_Value.rx_rssi_wb_k1;
		rx_rssi_nb_c1=calibrate_KC_strcut->Rssi_KC_Value.rx_rssi_wb_c1;
		rx_rssi_nb_k2=calibrate_KC_strcut->Rssi_KC_Value.rx_rssi_wb_k2;
		rx_rssi_nb_c2=calibrate_KC_strcut->Rssi_KC_Value.rx_rssi_wb_c2;		
		rx_rssi_nb_k3=calibrate_KC_strcut->Rssi_KC_Value.rx_rssi_wb_k3;
		rx_rssi_nb_c3=calibrate_KC_strcut->Rssi_KC_Value.rx_rssi_wb_c3;
		}
	
	if(Vtr_AdcValue->Advalue[RSSI_ADC_CH].w>0)  //500mw 的值
	{
		if(Vtr_AdcValue->Advalue[RSSI_ADC_CH].w<=rssi_power_hl_c_nb)
		{
			temp_rssi_power=Vtr_AdcValue->Advalue[RSSI_ADC_CH].w*rx_rssi_nb_k3+rx_rssi_nb_c3;
		}
		else
		{
			if(Vtr_AdcValue->Advalue[RSSI_ADC_CH].w<=rssi_power_hl_b_nb)
			{
				temp_rssi_power=Vtr_AdcValue->Advalue[RSSI_ADC_CH].w*rx_rssi_nb_k2+rx_rssi_nb_c2;			
			}
			else
			{
				if(Vtr_AdcValue->Advalue[RSSI_ADC_CH].w<=rssi_power_hl_a_nb)
				{
					temp_rssi_power=Vtr_AdcValue->Advalue[RSSI_ADC_CH].w*rx_rssi_nb_k1+rx_rssi_nb_c1;			
				}
				else
				{
					temp_rssi_power=Vtr_AdcValue->Advalue[RSSI_ADC_CH].w*rx_rssi_nb_k1+rx_rssi_nb_c1;							
				}
			}
		}
	}
	else
	{
		temp_rssi_power=0;
	}
	
	if(temp_rssi_power>0)
	{
		vtr_ch_info_current->vtrchannelinfo.rssip.propertyintfrac.whl.high=NCRSSI_VALUE_H;
		vtr_ch_info_current->vtrchannelinfo.rssip.propertyintfrac.whl.low=NCRSSI_VALUE_L;		
	}
	else
	{
		temp_rssi_power=temp_rssi_power*(-1);
		if(temp_rssi_power>=128)
		{
			vtr_ch_info_current->vtrchannelinfo.rssip.propertyintfrac.whl.high=NCRSSI_VALUE_H;
			vtr_ch_info_current->vtrchannelinfo.rssip.propertyintfrac.whl.low=NCRSSI_VALUE_L;		
			if(wbnbflg==1 &&wbnbflg==2)
			{
//				//2014.8.14 xiaona 添加
//				if(slot_f==0x02){
//					slot2_buf[0] = vtr_ch_info_current.vtrchannelinfo.rssip.propertyintfrac.whl.high;
//					slot2_buf[1] = vtr_ch_info_current.vtrchannelinfo.rssip.propertyintfrac.whl.low;
//				} 
//				else if(slot_f==0x01){
//					slot1_buf[0] = vtr_ch_info_current.vtrchannelinfo.rssip.propertyintfrac.whl.high;
//					slot1_buf[1] = vtr_ch_info_current.vtrchannelinfo.rssip.propertyintfrac.whl.low;
//				}
			}
			return;			
		}
		else
		{
			temp_rssi_power_data=(uint16)((temp_rssi_power*100+0.5));
			temp_rssi_power_hl.whl.high=(uint8)(temp_rssi_power);//
			temp_rssi_power_hl.whl.high |=0x80;
			temp_rssi_power_hl.whl.low=(uint8)(temp_rssi_power_data%100); //
		}
		
		vtr_ch_info_current->vtrchannelinfo.rssip.propertyintfrac.whl.high=temp_rssi_power_hl.whl.high;
		vtr_ch_info_current->vtrchannelinfo.rssip.propertyintfrac.whl.low=temp_rssi_power_hl.whl.low;
		if(wbnbflg==1 &&wbnbflg==2)
		{
//			//2014.8.14 xiaona 添加
//			if(slot_f==0x02){
//				slot2_buf[0] = vtr_ch_info_current.vtrchannelinfo.rssip.propertyintfrac.whl.high;
//				slot2_buf[1] = vtr_ch_info_current.vtrchannelinfo.rssip.propertyintfrac.whl.low;
//			} 
//			else if(slot_f==0x01){
//				slot1_buf[0] = vtr_ch_info_current.vtrchannelinfo.rssip.propertyintfrac.whl.high;
//				slot1_buf[1] = vtr_ch_info_current.vtrchannelinfo.rssip.propertyintfrac.whl.low;
//			}
		}
	}
}

/**/
/****************************************************************************
* 名称：Vtr_ADValue_Data_Deal_Step1
* 功能：AD转换完成后调用，ad转换后 场强的处理
* 入口参数：无
* 出口参数：五
****************************************************************************/
void Vtr_ADValue_Data_Deal_Step1()
{
	RTU_PARAM_NOT_SAVE_ADVALUE *Vtr_AdcValue =  Get_RTU_dev_ptr_Adc_value();
	VTR_CALIBRATE_KC_STRCUT *calibrate_KC_strcut = Get_RTU_dev_ptr_KC_struct();
	VTR_CALIBRATE_SYSTEM_PARAM *calibrate_sys_param = Get_RTU_dev_ptr_sys_param();
	VTR_PARAM_UNION_F4 * vtr_ch_info_current = Get_RTU_dev_ptr_current_chinfo();

	if(1)//(((uint32)((Read_P2()&MUTE_IN))==0))//有接收，接收器不失锁，计算当前场强值，失锁 场强置无效
	{
//		if((vtr_ch_info_current.vtrchannelinfo.alarmstate.propertyint.w&RXLDUNLOCKALARM)==RXLDUNLOCKALARM)
//		{
//			vtr_ch_info_current.vtrchannelinfo.rssip.propertyintfrac.whl.high=NCRSSI_VALUE_H;
//			vtr_ch_info_current.vtrchannelinfo.rssip.propertyintfrac.whl.low=NCRSSI_VALUE_L;	
//		}
//		else
		{
			change_rssimv_to_rssidbm_wbnb(vtr_ch_info_current->vtrchannelinfo.wbnbstate.property);//RSSI处理，依据工作模式 计算出对应场强
		}
	}
	else
	{//无接受，场强值无效
		vtr_ch_info_current->vtrchannelinfo.rssip.propertyintfrac.whl.high=NCRSSI_VALUE_H;
		vtr_ch_info_current->vtrchannelinfo.rssip.propertyintfrac.whl.low=NCRSSI_VALUE_L;
	}

//	/*保存显示用到的 场强强度变量*/
//	rssi_value=vtr_ch_info_current.vtrchannelinfo.rssip.propertyintfrac.whl.high&0x7f;
//	if(rssi_value>VTR_RSSI5)
//	{
//		vtr_rssi_num=0;
//	}
//	else
//	{
//		if(rssi_value>VTR_RSSI4)
//		{
//			vtr_rssi_num=1;
//		}
//		else
//		{
//			if(rssi_value>VTR_RSSI3)
//			{
//				vtr_rssi_num=2;
//			}
//			else
//			{
//				if(rssi_value>VTR_RSSI2)
//				{
//					vtr_rssi_num=3;
//				}
//				else
//				{
//					vtr_rssi_num=4;
//				}
//			}
//		}
//	}

}

/****************************************************************************
* 名称：Step3_alarmflag_deal_with_LowFWD
* 功能：AD转换完成后，计算出前向功率后执行，用于比较当前的前向功率是否满足
		低前向功率告警的产生条件。当有关断时不检查低向前告警
* 入口参数：无
* 出口参数：五
****************************************************************************/
void Step3_alarmflag_deal_with_LowFWD()
{
//	RTU_PARAM_NOT_SAVE_ADVALUE *Vtr_AdcValue =  Get_RTU_dev_ptr_Adc_value();
//	VTR_CALIBRATE_KC_STRCUT *calibrate_KC_strcut = Get_RTU_dev_ptr_KC_struct();
//	VTR_CALIBRATE_SYSTEM_PARAM *calibrate_sys_param = Get_RTU_dev_ptr_sys_param();
//	VTR_PARAM_UNION_F4 * vtr_ch_info_current = Get_RTU_dev_ptr_current_chinfo();

//	if((vtr_ch_info_current.vtrchannelinfo.fwdp.propertyintfrac.whl.high*100+vtr_ch_info_current.vtrchannelinfo.fwdp.propertyintfrac.whl.low-vtr_ch_info_current.vtrchannelinfo.fwdp.propertyintfrac.whl.low%10)<low_fwdptxpoweralarm_set)
//	{
//		//if(((Read_P1())&PAPTT)==0)
//		{//功放为开

//			if(temp_cut_papower_f==0&&voltage_cut_papower_f==0)
//			{//有关断的时候不查前向功率 			
//				if(lowfwd_check_flag==2)//比高反射多检测一次,防止按下micptt后，查询到低前向
//				{
//					lowfwd_check_flag=0;
//					vtr_ch_info_current.vtrchannelinfo.alarmstate.propertyint.w |= LOWFWDPALARM;
//				}
//				else
//				{
//					lowfwd_check_flag++;
//				}					
//			}
//			else
//			{
//				vtr_ch_info_current.vtrchannelinfo.alarmstate.propertyint.w &= ~(LOWFWDPALARM|OTHERALARM);
//				lowfwd_check_flag=0;				
//			}
//			
//		}
//		else
//		{
//			vtr_ch_info_current.vtrchannelinfo.alarmstate.propertyint.w &= ~(LOWFWDPALARM|OTHERALARM);
//			lowfwd_check_flag=0;
//		}
//	}
//	else
//	{//功放关，不产生低前向告警
//		vtr_ch_info_current.vtrchannelinfo.alarmstate.propertyint.w &= ~(LOWFWDPALARM|OTHERALARM);
//		lowfwd_check_flag=0;
//	}

}

/****************************************************************************
* 名称：Step3_alarmflag_deal_with_HighREF-高驻波比
* 功能：AD转换完成后，计算出驻波比后执行，用于比较当前的驻波比是否满足
		高驻波比告警的产生条件。
* 入口参数：无
* 出口参数：五
****************************************************************************/
void Step3_alarmflag_deal_with_HighREF()
{
//	uint16 now_vswralarm_value; //当前驻波比

//	//高字节为整数部分，低字节为小数部分
//	now_vswralarm_value=vtr_ch_info_current.vtrchannelinfo.swrp.propertyintfrac.whl.high*100+vtr_ch_info_current.vtrchannelinfo.swrp.propertyintfrac.whl.low-vtr_ch_info_current.vtrchannelinfo.swrp.propertyintfrac.whl.low%10;
//	if(now_vswralarm_value>=high_refptxpoweralarm_set)//310)//high_refptxpoweralarm_set)	
//	{
//		if(((Read_P1())&PAPTT)==0)
//		{//两次查询adc信息
//			if(vtr_ch_info_current.vtrchannelinfo.alarmstate.propertyint.w==0)
//			{			
//				if(highrefp_check_flag==1)
//				{
//					highrefp_check_flag=0;
//					vtr_ch_info_current.vtrchannelinfo.alarmstate.propertyint.w |= HIGHREFPALARM;
//				}
//				else
//				{
//					highrefp_check_flag=1;
//				}
//			}
//			else
//			{
//				
//			}
//		}
//		else
//		{
//			vtr_ch_info_current.vtrchannelinfo.alarmstate.propertyint.w &= ~(HIGHREFPALARM|OTHERALARM);
//			highrefp_check_flag=0;
//		}		
//	}
//	else
//	{
//		//vtr_ch_info_current.vtrchannelinfo.alarmstate.propertyint.w &= ~(HIGHREFPALARM|OTHERALARM);
//		//highrefp_check_flag=0;	
//#if 1
//		if(now_vswralarm_value<=HIGH_VSWRALARM_SET)
//		{//<1.5
//			vtr_ch_info_current.vtrchannelinfo.alarmstate.propertyint.w &= ~(HIGHREFPALARM|OTHERALARM);
//			highrefp_check_flag=0;
//		}
//		else
//		{//>1.5 <3.0
//			if(((Read_P1())&PAPTT)==0)
//			{//两次查询adc信息
//				
//			}
//			else
//			{
//				vtr_ch_info_current.vtrchannelinfo.alarmstate.propertyint.w &= ~(HIGHREFPALARM|OTHERALARM);
//				highrefp_check_flag=0;					
//			}
//		}
//#endif
//	}		

}

/****************************************************************************
* 名称：Step3_alarmflag_deal_with_Volatage---高低电压报警
* 功能：AD转换完成后，PA电压后执行，比较当前功放电压产生哪个告警，并作处理
		正常情况下有4个电压节点限值，从小到大 的顺序依次是:
		低电压关断  <  低电压告警  <  高电压告警  <  高电压关断
		所以存在如下5中条件:当前电压Cur,
		1、curvol<低压关断
		2、低压关断 <Curvol<低压告警
		3、低压高精<curvol<高压告警
		4、高压告警<curvol<高压关断
		5、高压关断<curvol
		
* 入口参数：无
* 出口参数：五
****************************************************************************/
uint8 Step3_alarmflag_deal_with_Volatage()
{
//	uint16 now_pavoltage_value;//当前功放电压值
//	uint16 now_txvoltage_value;//当前激励板电压值

//	now_txvoltage_value=(addr4_data*1022)/220;
//	now_pavoltage_value=(addr6_data*118)/18;//1105
//	now_pavoltage_value=(now_pavoltage_value+5)/10;
//	vtr_ch_info_current.vtrchannelinfo.pavoltage.propertyintfrac.whl.high=(uint8)((now_pavoltage_value+5)/100);
//	vtr_ch_info_current.vtrchannelinfo.pavoltage.propertyintfrac.whl.low=(uint8)(((now_pavoltage_value+5)%100)/10); 
//	if(now_pavoltage_value<low_pavoltagecut_set)
//	{
//		vtr_ch_info_current.vtrchannelinfo.alarmstate.propertyint.w |= LOWVOLTAGEALARM;
//		voltage_cut_papower_f=1;
//	}
//	else
//	{
//		if(now_pavoltage_value<low_pavoltagehalf_set)
//		{
//			vtr_ch_info_current.vtrchannelinfo.alarmstate.propertyint.w |= LOWVOLTAGEALARM;
//			voltage_cut_papower_f=0;
//		}
//		else
//		{
//			if(now_pavoltage_value<high_pavoltagehalf_set)
//			{//正常
//				vtr_ch_info_current.vtrchannelinfo.alarmstate.propertyint.w &= ~(LOWVOLTAGEALARM|OTHERALARM);
//				vtr_ch_info_current.vtrchannelinfo.alarmstate.propertyint.w &= ~(HIGHVOLTAGEALARM|OTHERALARM);
//				voltage_cut_papower_f=0;
//			}
//			else
//			{
//				if(now_pavoltage_value<high_pavoltagecut_set)
//				{
//					vtr_ch_info_current.vtrchannelinfo.alarmstate.propertyint.w |= HIGHVOLTAGEALARM;
//					voltage_cut_papower_f=0;
//				}
//				else
//				{
//					vtr_ch_info_current.vtrchannelinfo.alarmstate.propertyint.w |= HIGHVOLTAGEALARM;
//					voltage_cut_papower_f=1;
//				}
//			}
//		}
//	}
//	
//	if(now_pavoltage_value<TEMP_CHECK_VALUE)//功放电压过低时，不查温度
//	{
//		return fail_respond;
//		//vtr_ch_info_current.vtrchannelinfo.patemp.propertyintfrac.whl.low=(uint8)(temp_data%10);		
//	}
//	return suc_respond;
}
//计算驻波比
void account_vswr_value(void)
{
	fp64  vswr_diviso;//除数
	fp64  vswr_dividend;//被除数
	fp64 temp_vswr_data;
	fp64 temp_vswr_data_a;
	uint16 temp_vswr_data_b;
	uint8 temp_vswr_data_c;

	static	uint32 check_time=0;
	
	VTR_PARAM_UNION_F4 * vtr_ch_info_current = Get_RTU_dev_ptr_current_chinfo();
	if(1)//(Read_P1()&PAPTT)==0)
	{
		if(vtr_ch_info_current->vtrchannelinfo.refp.propertyintfrac.whl.high==0&&vtr_ch_info_current->vtrchannelinfo.refp.propertyintfrac.whl.low==0x00)
		{
			if(vtr_ch_info_current->vtrchannelinfo.fwdp.propertyintfrac.whl.high==0&&vtr_ch_info_current->vtrchannelinfo.fwdp.propertyintfrac.whl.low==0x00)
			{
				/*
				if(check_time>2)
				{
					check_time=0;
					vtr_ch_info_current.vtrchannelinfo.swrp.propertyintfrac.whl.high=20;
					vtr_ch_info_current.vtrchannelinfo.swrp.propertyintfrac.whl.low=0;
				}
				else
				{
					vtr_ch_info_current.vtrchannelinfo.swrp.propertyintfrac.whl.high=1;
					vtr_ch_info_current.vtrchannelinfo.swrp.propertyintfrac.whl.low=0;					
				}
				check_time++;
				*/
				if(check_time<3)//检测太快后，导致快速的按下PTT后，驻波比20到1闪动。
				{
					check_time++;
				}
				else
				{
					check_time=0;
					vtr_ch_info_current->vtrchannelinfo.swrp.propertyintfrac.whl.high=20;
					vtr_ch_info_current->vtrchannelinfo.swrp.propertyintfrac.whl.low=0;					
				}
				
			}
			else
			{
				check_time=0;
				vtr_ch_info_current->vtrchannelinfo.swrp.propertyintfrac.whl.high=1;
				vtr_ch_info_current->vtrchannelinfo.swrp.propertyintfrac.whl.low=0;
			}
		}
		else
		{
			check_time=0;
			if(vtr_ch_info_current->vtrchannelinfo.fwdp.propertyintfrac.whl.high==0&&vtr_ch_info_current->vtrchannelinfo.fwdp.propertyintfrac.whl.low==0x00)
			{
				vtr_ch_info_current->vtrchannelinfo.swrp.propertyintfrac.whl.high=0;
				vtr_ch_info_current->vtrchannelinfo.swrp.propertyintfrac.whl.low=0;
			}
			else
			{		
				#if 1
				vswr_diviso=(vtr_ch_info_current->vtrchannelinfo.fwdp.propertyintfrac.whl.high*1000+vtr_ch_info_current->vtrchannelinfo.fwdp.propertyintfrac.whl.low*10);
				vswr_dividend=(vtr_ch_info_current->vtrchannelinfo.refp.propertyintfrac.whl.high*1000+vtr_ch_info_current->vtrchannelinfo.refp.propertyintfrac.whl.low*10);
				if(vswr_diviso<vswr_dividend)
				{
					temp_vswr_data_c=19;//19;//20					
					vtr_ch_info_current->vtrchannelinfo.swrp.propertyintfrac.whl.high=temp_vswr_data_c;
					temp_vswr_data_c=90;//90;//0
					vtr_ch_info_current->vtrchannelinfo.swrp.propertyintfrac.whl.low=temp_vswr_data_c;
				}
				else
				{
					temp_vswr_data=vswr_diviso/vswr_dividend;
					temp_vswr_data=sqrt(temp_vswr_data);
					temp_vswr_data_a=(temp_vswr_data+1)/(temp_vswr_data-1);
					temp_vswr_data_b=(uint16)(temp_vswr_data_a*100);
					temp_vswr_data_c=(uint8)(temp_vswr_data_b/100);
					vtr_ch_info_current->vtrchannelinfo.swrp.propertyintfrac.whl.high=temp_vswr_data_c;
					temp_vswr_data_c=(uint8)(temp_vswr_data_b%100);
					vtr_ch_info_current->vtrchannelinfo.swrp.propertyintfrac.whl.low=temp_vswr_data_c;
				}
				#else
					vswr_diviso=(vtr_ch_info_current->vtrchannelinfo.fwdp.propertyintfrac.whl.high*1000+vtr_ch_info_current.vtrchannelinfo.fwdp.propertyintfrac.whl.low*10);
					vswr_dividend=(vtr_ch_info_current->vtrchannelinfo.refp.propertyintfrac.whl.high*1000+vtr_ch_info_current.vtrchannelinfo.refp.propertyintfrac.whl.low*10);								
					temp_vswr_data=vswr_diviso/vswr_dividend;
					temp_vswr_data=sqrt(temp_vswr_data);
					temp_vswr_data_a=(temp_vswr_data+1)/(temp_vswr_data-1);
					temp_vswr_data_b=(uint16)(temp_vswr_data_a*100);
					temp_vswr_data_c=(uint8)(temp_vswr_data_b/100);
					vtr_ch_info_current->vtrchannelinfo.swrp.propertyintfrac.whl.high=temp_vswr_data_c;
					temp_vswr_data_c=(uint8)(temp_vswr_data_b%100);
					vtr_ch_info_current->vtrchannelinfo.swrp.propertyintfrac.whl.low=temp_vswr_data_c;				
				#endif
			}
		}
	}
	else
	{
				vtr_ch_info_current->vtrchannelinfo.fwdp.propertyintfrac.whl.high=0;
				vtr_ch_info_current->vtrchannelinfo.fwdp.propertyintfrac.whl.low=0;
				vtr_ch_info_current->vtrchannelinfo.refp.propertyintfrac.whl.high=0;
				vtr_ch_info_current->vtrchannelinfo.refp.propertyintfrac.whl.low=0;
				vtr_ch_info_current->vtrchannelinfo.swrp.propertyintfrac.whl.high=0;
				vtr_ch_info_current->vtrchannelinfo.swrp.propertyintfrac.whl.low=0;
	}
}

void rt_send_adc_value_New(void)
{
	uint8 *temp_buf;//要发送的数据
	uint8 send_cmd;//发送命令
	uint8 i;//校准计数
	UintType check_sum;//校验和
	uint8 send_ptr;//发送计数
	uint8 respond_state;//响应类型
	uint32 txfre_data,rxfre_data;
	RTU_PARAM_NOT_SAVE_ADVALUE *Vtr_AdcValue =  Get_RTU_dev_ptr_Adc_value();
	VTR_CALIBRATE_KC_STRCUT *calibrate_KC_strcut = Get_RTU_dev_ptr_KC_struct();
	VTR_CALIBRATE_SYSTEM_PARAM *calibrate_sys_param = Get_RTU_dev_ptr_sys_param();
	VTR_PARAM_UNION_F4 * vtr_ch_info_current = Get_RTU_dev_ptr_current_chinfo();

	
#if 0
	rt_kprintf(DEBUG_TASK_ADC,"rt_send_adc_value_New RSSI:	[%d] \r\n", Vtr_AdcValue.RSSI.w );
	rt_kprintf(DEBUG_TASK_ADC,"rt_send_adc_value_New FWD_P:	[%d] \r\n", Vtr_AdcValue.FWD_P.w);
	rt_kprintf(DEBUG_TASK_ADC,"rt_send_adc_value_New REFL_P:	[%d] \r\n", Vtr_AdcValue.REFL_P.w);
	rt_kprintf(DEBUG_TASK_ADC,"rt_send_adc_value_New TX_12V:	[%d] \r\n",Vtr_AdcValue.TX_12V.w);
	rt_kprintf(DEBUG_TASK_ADC,"rt_send_adc_value_New TEMP:	[%d] \r\n", Vtr_AdcValue.TEMP_VOLTS.w);
	rt_kprintf(DEBUG_TASK_ADC,"rt_send_adc_value_New PA_VOLTS:[%d] \r\n", Vtr_AdcValue.PA_VOLTS.w );
#endif

	
	/*第一步---场强计算*/
	Vtr_ADValue_Data_Deal_Step1();

	/*第二步---前向、反向功率计算*/
	txfre_data=(vtr_ch_info_current->vtrchannelinfo.txfre.propertyint.w)*1000000+(vtr_ch_info_current->vtrchannelinfo.txfre.propertyfrac.w)*125;
	Vtr_ADValue_Data_Deal_Step2(txfre_data);
	
	/*第三步---根据前向反向值，计算驻波比*/
	account_vswr_value();
		
//	if(vtr_work_state==0x01)//进入了校准模式,不报低前向报警
//	{
//	}
//	else
//	{/*第四步---根据当前值，处理告警标志*/
//		Vtr_ADValue_Data_Deal_Step3();//告警处理
//	}

}


//
/****************************************************************************
* ??:Vtr_Do_Calibrate_Task_By_Fn
* ??:?????????FN,?????????????,
* ????:fn:???????  
			recv_data:???????
* ????:RET_FAIL_F0:???????
****************************************************************************/
uint8 Vtr_Do_Calibrate_Task_By_Fn(uint8 fn,uint8* recv_data)
{
	uint8 powertype=0;			//????
	uint8 optflg=0;				//?????????1???2???3???4??
	uint8 powoffset=0;			//???????????,??????????
	uint8 paramsaveoffset=0;	//??????????????????
	uint8 wbnbflg=0;			//??????
	uint8 curvalueoffset = 0;	//????????????????????????,????????????
	uint8 config_ch_info_flg=0;//???? ??????,?????????pOt????????,??????????
	//Calibrate_CMD_Graph;
	VTR_CALIBRATE_SYSTEM_PARAM * calibrate_sys_param = Get_RTU_dev_ptr_sys_param();

	
	if(fn==0x4b||
		fn==0x53||
		fn==0x59||
		fn==0x6b||
		fn==0x6d||
		fn==0x89)
		{
		/*??????????*/
			if(fn==0x4b)
				{//??????????
					powertype=(uint8)*recv_data;
					optflg=1;
					powoffset=Calibrate_Get_SaveOffset_By_Powtype(powertype);//?????????
					paramsaveoffset = powoffset * sizeof(VTR_CALIBRATE_POWER_POINT_STRCUT);//??????????
					paramsaveoffset+=PARAM_OFFSET(VTR_CALIBRATE_POWER_POINT_STRCUT,Power_Value);//????????????????
					//curvalueoffset  ???????paramsaveoffset???
					curvalueoffset =PARAM_OFFSET(VTR_CALIBRATE_POWER_POINT_STRCUT,FWD_Value)-PARAM_OFFSET(VTR_CALIBRATE_POWER_POINT_STRCUT,Power_Value);
					config_ch_info_flg=1;//????????
				}
			if(fn==0x6D)
				{//POT ????
					VTR_PARAM_F6D *param=(VTR_PARAM_F6D *)recv_data;
					powertype = param->d5;//??????
					
					powoffset=Calibrate_Get_SaveOffset_By_Powtype(powertype);//???????????????????
					paramsaveoffset = powoffset * sizeof(VTR_CALIBRATE_POWER_POINT_STRCUT);// ??????????????
					paramsaveoffset+=PARAM_OFFSET(VTR_CALIBRATE_POWER_POINT_STRCUT,Refdoc_Value);//??????
					//curvalueoffset =PARAM_OFFSET(VTR_CALIBRATE_POWER_POINT_STRCUT,Refdoc_Value);//???KC?????
					config_ch_info_flg=1;//????????
				}

			if(fn==0x53)//??????
			{
				
				VTR_PARAM_F53*param=(VTR_PARAM_F53 *)recv_data;
				wbnbflg=param->d1;//??????: ?0,?1
				/*??????????*/
				paramsaveoffset =wbnbflg * (PARAM_OFFSET(VTR_CALIBRATE_RSSI_STRCUT,nbValue_a)-PARAM_OFFSET(VTR_CALIBRATE_RSSI_STRCUT,wbValue_a));
				/*??????????--??paramsaveoffset???*/
				curvalueoffset=PARAM_OFFSET(VTR_CALIBRATE_RSSI_STRCUT,rssi_mv_a_wb);
				optflg=2;//??????????,??????????
				
				config_ch_info_flg=1;//????????
			}
			if(fn==0x89)//????
			{
				VTR_PARAM_F89*param=(VTR_PARAM_F89 *)recv_data;
				wbnbflg=param->d1;//??????,0?1?
				paramsaveoffset =wbnbflg * (PARAM_OFFSET(VTR_CALIBRATE_MUTE_STRCUT,nbValue_a)-PARAM_OFFSET(VTR_CALIBRATE_MUTE_STRCUT,wbValue_a));
				
				optflg=4;//??????????,??????????
				
				config_ch_info_flg=1;//????????
			}

			if(fn==0x59)//????
			{  
				optflg=3;//??????????,??????????
 			}
			calibrate_tmp_struct.powtype	=powertype; 	//????
			calibrate_tmp_struct.freqnum	=Calibrate_Get_Freqnum_By_Powtype(powertype); //??
			calibrate_tmp_struct.optflg 	=optflg; 		//??????
			calibrate_tmp_struct.saveptr	=(uint8*)&calibrate_sys_param+Calibrate_Get_CMD_Offset_By_Fn(fn)+paramsaveoffset; //????????
			calibrate_tmp_struct.adcurvalue =calibrate_tmp_struct.saveptr+curvalueoffset; //AD???????
			calibrate_tmp_struct.paramptr   =recv_data;		//????
			calibrate_tmp_struct.paramlen	=Calibrate_Get_ParamLen_By_Fn(fn);//????
			calibrate_tmp_struct.cmdoffset     =0;				//????
			calibrate_tmp_struct.fn			=fn;			//?????
			calibrate_tmp_struct.wbnbflg	=wbnbflg; 		//??????
			calibrate_tmp_struct.saveoffset =powoffset;		//?????????????

			if(config_ch_info_flg==1)
			{
				Vtr_set_channel_info_calibrate(calibrate_tmp_struct.freqnum,calibrate_tmp_struct.wbnbflg,1);//??????
			}
			
			save_calibrate_param_data(&calibrate_tmp_struct);//????
			
			return RET_FAIL_F0;
		}
	
	calibrate_tmp_struct.cmdoffset 	=Calibrate_Get_CMD_Offset_By_Fn(fn);//??????????????????
	calibrate_tmp_struct.saveptr   +=calibrate_tmp_struct.cmdoffset;		//??????????
	calibrate_tmp_struct.adcurvalue+=calibrate_tmp_struct.cmdoffset;		//ad???????????
	calibrate_tmp_struct.paramptr	=recv_data;							//????
	calibrate_tmp_struct.paramlen	=Calibrate_Get_ParamLen_By_Fn(fn);	//????
	calibrate_tmp_struct.fn 		=fn;								//?????

	save_calibrate_param_data(&calibrate_tmp_struct); //????
	calibrate_tmp_struct.saveptr   -=calibrate_tmp_struct.cmdoffset;		//??????????
	calibrate_tmp_struct.adcurvalue-=calibrate_tmp_struct.cmdoffset;		//ad???????????
	return RET_FAIL_F0;
}





/****************************************************************************
* ??:vtr_Enter_Normal_Calibrate_new
* ??:??????,????????F49????????
* ????:
* ????:suc_respond ??,fail_respond ???
****************************************************************************/
void vtr_Enter_Normal_Calibrate_new(void)
{
	uint8 i;
	UintlhType *tmpptr;
	
	VTR_CALIBRATE_SYSTEM_PARAM * calibrate_sys_param = Get_RTU_dev_ptr_sys_param();
//	eeprom_par_read_new();
	
	memcpy((uint8*)&vtr_calibratevalue_normal.calibratepar.voice_fre,(uint8*)&calibrate_sys_param->VoiceValue.voice_fre,10);	//voice
	
	memcpy((uint8*)&vtr_calibratevalue_normal.calibratepar.refdcpot_value_361M,(uint8*)&calibrate_sys_param->PowerValue[0].Refdoc_Value,8);//pot	
	memcpy((uint8*)&vtr_calibratevalue_normal.calibratepar.refdcpot_value_363M,(uint8*)&calibrate_sys_param->PowerValue[1].Refdoc_Value,8);//pot	
	memcpy((uint8*)&vtr_calibratevalue_normal.calibratepar.refdcpot_value_365M,(uint8*)&calibrate_sys_param->PowerValue[2].Refdoc_Value,8);//pot	
	memcpy((uint8*)&vtr_calibratevalue_normal.calibratepar.refdcpot_value_367M,(uint8*)&calibrate_sys_param->PowerValue[3].Refdoc_Value,8);//pot	

	memcpy((uint8*)&vtr_calibratevalue_normal.calibratepar.power_value_a_361M,(uint8*)&calibrate_sys_param->PowerValue[0].Power_Value,14);//power	
	memcpy((uint8*)&vtr_calibratevalue_normal.calibratepar.power_value_a_363M,(uint8*)&calibrate_sys_param->PowerValue[1].Power_Value,14);//power	
	memcpy((uint8*)&vtr_calibratevalue_normal.calibratepar.power_value_a_365M,(uint8*)&calibrate_sys_param->PowerValue[2].Power_Value,14);//power	
	memcpy((uint8*)&vtr_calibratevalue_normal.calibratepar.power_value_a_367M,(uint8*)&calibrate_sys_param->PowerValue[3].Power_Value,14);//power	

	memcpy((uint8*)&vtr_calibratevalue_normal.calibratepar.temperature_value_a,(uint8*)&calibrate_sys_param->TempValue.tempValue_a,8);//pa_temp
	memcpy((uint8*)&vtr_calibratevalue_normal.calibratepar.rssi_value_a_wb,(uint8*)&calibrate_sys_param->RssiValue.rssi_mv_a_wb ,16);	//rssi
	
	memcpy((uint8*)&vtr_calibratevalue_normal.calibratepar.mute_value_a_wb,(uint8*)&calibrate_sys_param->MuteValue.wbValue_a,8);	//mute

	if(vtr_calibratevalue_normal.calibratepar.txptt_state)//?PTT??
	{
		vtr_calibratevalue_normal.calibratepar.txptt_state=0;		
	}
	
	vtr_calibratevalue_calibrate=vtr_calibratevalue_normal;//copy

	
		//memcpy(vtr_calibratevalue_calibrate.calibratepar.power_value_a_361M.w,
		//vtr_calibratevalue_normal.calibratepar.power_value_a_361M.w,
		//PARAM_OFFSET(CalibrateValue,power_value_g_367M)-PARAM_OFFSET(CalibrateValue,power_value_1_361M)+sizeof(UintlhType));
		//memcpy(&vtr_calibratevalue_calibrate.calibratepar.power_value_a_361M.w,
		//&vtr_calibratevalue_normal.calibratepar.power_value_a_361M.w,
		//sizeof(UinthlType)*28); //?????1K mzm
		//mzm_kprintf_str(vtr_calibratevalue_normal.buf,100);
		
		tmpptr = &vtr_calibratevalue_calibrate.calibratepar.power_value_a_361M;
		
		//mzm_kprintf(7,(tmpptr)->w,0,1);
		//mzm_kprintf(9,(tmpptr+1)->w,0,1);
		for(i=0;i<7*4;i++)
		{
			swap((tmpptr+i)->w);
		}
		//mzm_kprintf(8,(tmpptr)->w,0,1);
		//mzm_kprintf(10,(tmpptr+1)->w,0,1);

}

/****************************************************************************
* 名称：Vtr_Resume_Normal_Calibrate_new
* 功能：将校准参数保存到norflash中，并读取比较写入是否成功
* 入口参数：
* 出口参数：suc_respond 有效,fail_type2 读写失败
****************************************************************************/
uint8 Vtr_Resume_Normal_Calibrate_new(void)
{
	VTR_CALIBRATE_SYSTEM_PARAM tmp_calibrate_param;
	uint8 *p,*tmp;
	uint16 i;
	VTR_CALIBRATE_SYSTEM_PARAM * calibrate_sys_param = Get_RTU_dev_ptr_sys_param();

	vtr_calibratevalue_normal=vtr_calibratevalue_calibrate;

	/*?????????*/
	//Vtr_Set_Calibrate_Param_From_Flash((uint8*)&calibrate_sys_param);//?
	memset((uint8*)&tmp_calibrate_param,0,sizeof(VTR_CALIBRATE_SYSTEM_PARAM));
	//Vtr_Read_Calibrate_Param_From_Flash((uint8*)&tmp_calibrate_param);//?

	p=(uint8*)calibrate_sys_param;
	tmp=(uint8*)&tmp_calibrate_param;
	
	for(i=0;i<sizeof(VTR_CALIBRATE_SYSTEM_PARAM);i++)
	{
		if(*(p+i)!=*(tmp+i))
			{
			
//			mzm_kprintf(10,"ReadEEROR",i,"SetEEROR",i,1);
//			mzm_kprintf(11,"ReadEEROR",*(p+i),"SetEEROR",*(tmp+i),1);
			break;
			}
	}
	
	if(memcmp(&tmp_calibrate_param,calibrate_sys_param,sizeof(VTR_CALIBRATE_SYSTEM_PARAM))!=0)//??????
	{
//		mzm_kprintf(9,"ReadEEROR",0,"SetEEROR",1,1);
		return fail_type2;
	}
	//Initpar_new();//???????,????,????
	
	return suc_respond;

	
}

uint8 Vtr_Send_Power_Calibratepar(uint8* recv_data,uint8 * databuf,uint16 * len)
{
	VTR_PARAM_F4B *paramptr=(VTR_PARAM_F4B *)recv_data;

	vtr_power_work_type=paramptr->d1;
	
	vtr_pot_work_type=0xff;
	switch(vtr_power_work_type)
	{
		case 0x00:
			set_channel_info_test(1,0,1);//??361M??,??,??,?????
			break;
		case 0x01:
			set_channel_info_test(2,0,1);//??363M??,??,??,?????
			break;
		case 0x02:
			set_channel_info_test(3,0,1);//??365M??,??,??,?????
			break;
		case 0x03:
			set_channel_info_test(4,0,1);//??367M??,??,??,?????
			break;
		default:
			break;
	}
	
	return(RET_FAIL_F0);
}

/****************************************************************************
//* ??:power_calibrate_abcdefg()
//* ??:??????:??????????,??7?????,???????
//					????????,????
//* ????: uint8 recv_data:?????:0x4d/0x51/0x71/0x73/0xa5/0xa7/0xa9
//*										A	  B	    C    D    E    F    G
//* ????:?
//* ??:
//****************************************************************************/
//uint8 Vtr_power_calibrate_abcdefg(uint8 recv_cmd,uint8* recv_data,uint8 * databuf,uint16 * len)
//{
//	UintlhType *calibratepar,*txpar;
//	uint8 offset,flg;
//	uint8 *power_calibrate_flag = &power_calibrate_361M_flag;//????
//	
//	VTR_PARAM_F4D *paramptr=(VTR_PARAM_F4D *)recv_data;
//	switch(recv_cmd)
//	{
//		case 0x4d://??????		A
//			calibratepar = &vtr_calibratevalue_calibrate.calibratepar.power_value_a_361M;//??????
//			txpar = &vtr_eeprom_tx.txpar.power_value_a_361M;//??????
//			flg = 1;
//		break;
//		case 0x51://?????? 		B
//			calibratepar = &vtr_calibratevalue_calibrate.calibratepar.power_value_b_361M;//??????
//			txpar = &vtr_eeprom_tx.txpar.power_value_b_361M;//??????
//			flg = 2;
//		break;
//		case 0x71://?????? 		C
//			calibratepar = &vtr_calibratevalue_calibrate.calibratepar.power_value_c_361M;//??????
//			txpar = &vtr_eeprom_tx.txpar.power_value_c_361M;//??????
//			flg = 3;
//		break;
//		case 0x73://??????		D
//			calibratepar = &vtr_calibratevalue_calibrate.calibratepar.power_value_d_361M;//??????
//			txpar = &vtr_eeprom_tx.txpar.power_value_d_361M;//??????
//			flg = 4;
//		break;
//		case 0xa5://?????? 		E
//			calibratepar = &vtr_calibratevalue_calibrate.calibratepar.power_value_e_361M;//??????
//			txpar = &vtr_eeprom_tx.txpar.power_value_e_361M;//??????
//			flg = 5;
//		break;
//		case 0xa7://?????? 		F
//			calibratepar = &vtr_calibratevalue_calibrate.calibratepar.power_value_f_361M;//??????
//			txpar = &vtr_eeprom_tx.txpar.power_value_f_361M;//??????
//			flg = 6;
//		break;
//		case 0xa9://?????? 		G
//			calibratepar = &vtr_calibratevalue_calibrate.calibratepar.power_value_g_361M;//??????
//			txpar = &vtr_eeprom_tx.txpar.power_value_g_361M;//??????
//			flg = 7;
//		break;
//		default:
//			return(RET_FAIL_F4);
//			break;

//	}
//	switch(vtr_power_work_type)
//	{
//		case 0x00:	
//		case 0x01:
//		case 0x02:	
//		case 0x03:

//			//offset = vtr_power_work_type*(PARAM_OFFSET(CalibrateValue,power_value_a_363M)-PARAM_OFFSET(CalibrateValue,power_value_a_361M));
//			offset = vtr_power_work_type*7*sizeof(UintlhType);
//			offset /= sizeof(UintlhType);
//			(*(calibratepar+offset)).wlh.high=paramptr->d1[0];//??????DAC????
//			(*(calibratepar+offset)).wlh.low=paramptr->d1[1];//??????DAC????
//			(*(txpar+offset)).w = (*(calibratepar+offset)).w;
//			//DAC_OUT((*(txpar+offset)).w, 0x00, 0x00, 0x00);				
//			//delay_208us(40);
//			//reset_adc_scan();	
//			*(power_calibrate_flag+vtr_power_work_type)=flg;

//			//mzm_kprintf(4,recv_cmd,vtr_power_work_type*2+361,1);
//			//mzm_kprintf(5,offset,sizeof(UintlhType),1);
//			//mzm_kprintf(6,calibratepar,calibratepar+offset,1);
//		

//			
//		//	mzm_kprintf(8,&vtr_calibratevalue_calibrate.calibratepar.power_value_a_361M,
//			//&vtr_calibratevalue_calibrate.calibratepar.power_value_g_361M,1);
//		//	mzm_kprintf(9,&vtr_calibratevalue_calibrate.calibratepar.power_value_a_363M,
//		//	&vtr_calibratevalue_calibrate.calibratepar.power_value_g_363M,1);

//		//	mzm_kprintf(11,&vtr_calibratevalue_calibrate.calibratepar.power_value_a_365M,
//		//	&vtr_calibratevalue_calibrate.calibratepar.power_value_g_365M,1);
//		//	mzm_kprintf(12,&vtr_calibratevalue_calibrate.calibratepar.power_value_a_367M,
//		//	&vtr_calibratevalue_calibrate.calibratepar.power_value_g_367M,1);
//			break;
//		
//		default:
//			break;
//	}
//	vol_calibrate();
//	return(RET_FAIL_F0);
//}
uint8 Vtr_Send_Rssi_Calibratepar(uint8* recv_data,uint8 * databuf,uint16 * len)
{
	VTR_PARAM_F53 *paramptr=(VTR_PARAM_F53 *)recv_data;
	vtr_rssi_work_type=paramptr->d1;
	switch(vtr_rssi_work_type)
	{
		case 0x00:
			set_channel_info_test(2,0,1);//??363M??,??,??,?????
			break;
		case 0x01:
			set_channel_info_test(2,1,1);//??363M??,??,??,?????
			break;
		default:
			break;
	}
	return(RET_FAIL_F0);
}

/****************************************************************************
//* ??:rssi_calibrate_abcd()
//* ??:??????:??????????,??4?????,???????
//					????????,????
//* ????: uint8 recv_cmd:?????:0x56/0x58/0x76/0x78/
//*										A	  B	    C    D 
//* ????:?
//* ??:
//****************************************************************************/
//uint8 Vtr_Rssi_Calibrate_abcd(uint8 recv_cmd,uint8* recv_data,uint8 * databuf,uint16 * len)
//{
//	UinthlType *calibratepar,*rxpar;
//	uint8 offset,flg;
//	uint8 *rssi_calibrate_flag = &rssi_calibrate_wbflag;//????
//	VTR_PARAM_F55 *paramptr=(VTR_PARAM_F55 *)recv_data;
//	
//	
//	switch(recv_cmd)
//	{
//		case 0x55://??????		A
//			calibratepar = &vtr_calibratevalue_calibrate.calibratepar.rssi_value_a_wb;//??????
//			rxpar = &vtr_eeprom_rx.rxpar.rssi_value_a_wb;//??????
//			flg = 1;
//		break;
//		case 0x57://?????? 		B
//			calibratepar = &vtr_calibratevalue_calibrate.calibratepar.rssi_value_b_wb;//??????
//			rxpar = &vtr_eeprom_rx.rxpar.rssi_value_b_wb;//??????
//			flg = 2;
//		break;
//		case 0x75://?????? 		C
//			calibratepar = &vtr_calibratevalue_calibrate.calibratepar.rssi_value_c_wb;//??????
//			rxpar = &vtr_eeprom_rx.rxpar.rssi_value_c_wb;//??????
//			flg = 3;
//		break;
//		case 0x77://??????		D
//			calibratepar = &vtr_calibratevalue_calibrate.calibratepar.rssi_value_d_wb;//??????
//			rxpar = &vtr_eeprom_rx.rxpar.rssi_value_d_wb;//??????
//			flg = 4;
//		break;
//		
//		default:
//			return(RET_FAIL_F4);
//			break;

//	}
//	switch(vtr_rssi_work_type)
//	{
//		case 0x00:
//		case 0x01:
//			offset = vtr_rssi_work_type*4*sizeof(UintlhType);
//			offset /= sizeof(UintlhType);
//			(*(calibratepar+offset)).whl.high=paramptr->d1[0];//??????DAC????
//			(*(calibratepar+offset)).whl.low=paramptr->d1[1];//??????DAC????
//			(*(rxpar+offset)).w = (*(calibratepar+offset)).w;
//			
//			*(rssi_calibrate_flag+vtr_rssi_work_type)=flg;
//			//delay_208us(40);
//			//reset_adc_scan();		
//			#if 0
//			mzm_kprintf(4,paramptr->d1[0],paramptr->d1[1],1);
//			mzm_kprintf(5,(*(rxpar+offset)).w,(*(calibratepar+offset)).w,1);
//			mzm_kprintf(6,calibratepar,calibratepar+offset,1);

//			mzm_kprintf(8,&vtr_calibratevalue_calibrate.calibratepar.rssi_value_a_wb,
//			&vtr_calibratevalue_calibrate.calibratepar.rssi_value_b_wb,1);
//			mzm_kprintf(9,&vtr_calibratevalue_calibrate.calibratepar.rssi_value_c_wb,
//			&vtr_calibratevalue_calibrate.calibratepar.rssi_value_d_wb,1);

//			mzm_kprintf(11,&vtr_calibratevalue_calibrate.calibratepar.rssi_value_a_nb,
//			&vtr_calibratevalue_calibrate.calibratepar.rssi_value_b_nb,1);
//			mzm_kprintf(12,&vtr_calibratevalue_calibrate.calibratepar.rssi_value_c_nb,
//			&vtr_calibratevalue_calibrate.calibratepar.rssi_value_d_nb,1);
//			#endif
//			break;
//		default:
//			break;
//	}
//	vol_calibrate();
//	return(RET_FAIL_F0);
//	
//}
uint8 Vtr_Send_Temperature_Calibrate(uint8* recv_data,uint8 * databuf,uint16 * len)
{
	return RET_FAIL_F0;
}

uint8 Vtr_Temperature_Calibrate_abcd(uint8 recv_cmd,uint8* recv_data,uint8 * databuf,uint16 * len)
{
	UinthlType *calibratepar,*papar;	
	
	VTR_PARAM_F5B *paramptr=(VTR_PARAM_F5B *)recv_data;
	
	switch(recv_cmd)
	{
		case 0x5b://??????		A
			calibratepar = &vtr_calibratevalue_calibrate.calibratepar.temperature_value_a;//??????
			papar = &vtr_eeprom_pa.papar.temperature_value_a;//??????
		break;
		case 0x5d://??????		B
			calibratepar = &vtr_calibratevalue_calibrate.calibratepar.temperature_value_b;//??????
			papar = &vtr_eeprom_pa.papar.temperature_value_b;//??????
		break;
		case 0x79://??????		C
			calibratepar = &vtr_calibratevalue_calibrate.calibratepar.temperature_value_c;//??????
			papar = &vtr_eeprom_pa.papar.temperature_value_c;//??????
		break;
		case 0x7b://??????		D
			calibratepar = &vtr_calibratevalue_calibrate.calibratepar.temperature_value_d;//??????
			papar = &vtr_eeprom_pa.papar.temperature_value_d;//??????
		break;
		
		default:
			return(0x01);
			break;

	}
	
	(*(calibratepar)).whl.high=paramptr->d1[0];//??????DAC????
	(*(calibratepar)).whl.low=paramptr->d1[1];//??????DAC????
	(*(papar)).w = (*(calibratepar)).w;
	
	if((*(papar)).whl.high&0x80)
	{//??
		
	}
	else
	{//??
	
	}
	return(RET_FAIL_F0);
}

uint8 Vtr_Send_Mute_Calibratepar(uint8* recv_data,uint8 * databuf,uint16 * len)
{
	VTR_PARAM_F89*paramptr=(VTR_PARAM_F89*)recv_data;
	vtr_mute_work_type=paramptr->d1;
	//vtr_mute_work_type
	//??????:
	//1)	0x00:??
	//2)	0x01:??
	switch(vtr_mute_work_type)
	{
		case 0x00:
			set_channel_info_test(2,0,1);//??363M??,??,??,?????
			break;
		case 0x01:
			set_channel_info_test(2,1,1);//??363M??,??,??,?????
			break;
		default:
			break;
	}
	return RET_FAIL_F0;	
}
/****************************************************************************
* 名称：save_calibrate_param_data
* 功能：执行 校准和数据保存
* 入口参数：VTR_CALIBRATE_TMP_STRCUT* tmpstruct:校准数据结构体指针
* 出口参数：RET_FAIL_F0:回应帧组确认帧
****************************************************************************/
uint8 save_calibrate_param_data(VTR_CALIBRATE_TMP_STRCUT* tmpstruct)
{
	uint8 acountkcflg;//KC????,1??KC??????????,2??rssi?????,???????
	
//	mzm_kprintf(5,"fn",tmpstruct->fn,"powtype",tmpstruct->powtype,1);
//	mzm_kprintf(6,"saveoffset",tmpstruct->saveoffset,"cmdoffset",tmpstruct->cmdoffset,1);
//	mzm_kprintf(7,"optflg",tmpstruct->optflg,"wbnbflg",tmpstruct->wbnbflg,1);
//	mzm_kprintf(8,"paramlen",tmpstruct->paramlen,"paramptr",tmpstruct->paramptr,1);
//	mzm_kprintf(9,"saveptr",tmpstruct->saveptr,"advalue",tmpstruct->adcurvalue,1);
//	
//	mzm_kprintf(10,"1paramptr",(uint16)*(tmpstruct->paramptr),"saveptr",(uint16)*(tmpstruct->saveptr),1);
	/*有效性判断，参数长度无效，保存缓冲区无效*/
	if(tmpstruct->paramlen==0|| tmpstruct->paramlen==0xff||tmpstruct->saveptr==NULL||tmpstruct->adcurvalue==NULL)
	{
		return 0;
	}
	
	acountkcflg=Calibrate_Get_AcountKC_FLG_By_Fn(tmpstruct->fn);
	/*判断功率校准命令是否有效:执行校准功率频率类型命令之后，在执行a-g的校准命令是有效。未执行校准频率类型命令，命令无效*/
	if((acountkcflg==1&&tmpstruct->optflg!=1)||
		(acountkcflg==2&&tmpstruct->optflg!=2)||
		(acountkcflg==3&&tmpstruct->optflg!=3)||
		(acountkcflg==4&&tmpstruct->optflg!=4))
	{
		return 0; //没有频率类型，没有射频工作模式的 功率校准、场强校准、静噪校准 ，是非法命令，不进行数据处理
	}
	memcpy(tmpstruct->saveptr,tmpstruct->paramptr,tmpstruct->paramlen);//保存数据
	mzm_kprintf(11,"2paramptr",(uint16)*(tmpstruct->paramptr),"saveptr",(uint16)*(tmpstruct->saveptr),1);
	Vtr_after_save_data_deal_with(tmpstruct);//数据保存后，各种命令的数据处理操作
	Vtr_Acount_Rssi_Fwd_KC(tmpstruct); //计算 场强和前向功率的增益  K 和C
	Vtr_Acount_Power_KC(tmpstruct); //计算 功率的增益  K 和C
	

	return RET_FAIL_F0;
	
}
//

uint8 Vtr_Mute_Calibrate_ab(uint8 recv_cmd,uint8* recv_data,uint8 * databuf,uint16 * len)
{
//	UinthlType *calibratepar,*rxpar;
//	uint8 offset,flg;
//	VTR_PARAM_F8B *paramptr=(VTR_PARAM_F8B *)recv_data;
//	

//	switch(recv_cmd)
//	{
//		case 0x8b://??????		A
//			calibratepar = &vtr_calibratevalue_calibrate.calibratepar.mute_value_a_wb;//??????
//			rxpar = &vtr_eeprom_rx.rxpar.mute_value_a_wb;//??????
//		break;
//		case 0x8d://?????? 		B
//			calibratepar = &vtr_calibratevalue_calibrate.calibratepar.mute_value_b_wb;//??????
//			rxpar = &vtr_eeprom_rx.rxpar.mute_value_b_wb;//??????
//		break;

//		default:
//			return(RET_FAIL_F4);
//			break;

//	}
//	switch(vtr_mute_work_type)
//	{
//		case 0x00:	
//		case 0x01:

//			offset = vtr_mute_work_type*2*sizeof(UintlhType);
//			offset /= sizeof(UintlhType);
//			(*(calibratepar+offset)).whl.high=paramptr->d1[0];//??????DAC????
//			(*(calibratepar+offset)).whl.low=paramptr->d1[1];//??????DAC????
//			(*(rxpar+offset)).w = (*(calibratepar+offset)).w;
//			set_mutelevel_pot=mutelevel_hex2dec((*(rxpar+offset)).whl.low);
//			
//			I22C_WriteNByte(MCP4651_1,ONE_BYTE_SUBA,MUTE,&set_mutelevel_pot,1);//?U5_1(mute)?????RAM 0x80
//			break;

//		default:
//			break;
//	}
	return(RET_FAIL_F0);
}


/****************************************************************************
* ??:Vtr_Acount_Power_KC
* ??:?????????,KC????calibrate_KC_strcut???
* ????:(VTR_CALIBRATE_TMP_STRCUT* tmpstruct:???????
* ????:?
****************************************************************************/
uint8 Vtr_Acount_Power_KC(VTR_CALIBRATE_TMP_STRCUT* tmpstruct)
{
	uint16 pre_tmp_pvalue=0;
	uint16 cur_tmp_pvalue=0;
	uint16* advaluecur=(uint16*)tmpstruct->saveptr;
	uint16* advaluelast;
	uint8 acountkcflg;//KC????,1??KC??????????,2??rssi?????
	uint16 firstvalue,secondvalue;//????,?????
	fp64* kvalueaddr;//????,??K??????
	fp64* cvalueaddr;//????,??C??????
	fp64  kvalue,cvalue,cvalue1,cvalue2;//????
	uint16 nboffset;// KC????:??????????
	
	VTR_CALIBRATE_KC_STRCUT *calibrate_KC_strcut = Get_RTU_dev_ptr_KC_struct();
	
	acountkcflg=Calibrate_Get_AcountKC_FLG_By_Fn(tmpstruct->fn);
	if(acountkcflg!=1)
	{
		return RET_FAIL_F0; //?????,????
	}
		
	kvalueaddr=(fp64*)&calibrate_KC_strcut->Power_KC_Value[tmpstruct->saveoffset].KC_k1;//??k?????
	cvalueaddr=(fp64*)&calibrate_KC_strcut->Power_KC_Value[tmpstruct->saveoffset].KC_c1;//??c?????
	
	firstvalue=Calibrate_Get_AcountKC_FirstValue_By_Fn(tmpstruct->fn);//?? ??????
	secondvalue=Calibrate_Get_AcountKC_SecondValue_By_Fn(tmpstruct->fn);//?????????
		
	if(firstvalue!=0&&secondvalue!=0)
	{
		advaluelast = advaluecur-1;//???0 ????a????????,???????-1?????????,?????
		pre_tmp_pvalue=*advaluelast;
		cur_tmp_pvalue=*advaluecur;
		/*????????????,?????,??????????????:A ??,??tmpstrct->cmdoffset ????0
			tmpstruct->cmdoffset,??????ABCD..???????2??,??cmdnum
			?2??????,
			?????sizeof(UinthlType),?????????;A-0,B-1,C-2,D-3,,,,
			???? -1???B?? ?? K1?C1,C?? ??K2-C2??
		*/
		kvalueaddr+=(tmpstruct->cmdoffset/sizeof(UinthlType)-1)*2;//?????????k???
		cvalueaddr+=(tmpstruct->cmdoffset/sizeof(UinthlType)-1)*2;//?????????c???
	}
	else{
		return RET_FAIL_F0;
		}
	if(pre_tmp_pvalue>=cur_tmp_pvalue)
	{
	#ifndef OPEN_ERROR_PC
		Send_Power_Calibrate_Response(fail_type1);
	#endif
	}
	else{
		// y=Kx+c
		firstvalue /=1000;
		secondvalue /=1000;
		//kvalue=(fp64)(secondvalue-firstvalue)/(cur_tmp_pvalue-pre_tmp_pvalue);//??K?
		//cvalue1=(fp64)kvalue*cur_tmp_pvalue;						
		//cvalue2=(fp64)secondvalue;						
		//cvalue=cvalue2-cvalue1;//??C ?,???????C ?


		kvalue=(fp64)(((cur_tmp_pvalue-pre_tmp_pvalue)*2500/1024)/(secondvalue-firstvalue));
		cvalue1=(fp64)kvalue*secondvalue;
		cvalue2=((fp64)(cur_tmp_pvalue*2500/1024));						
		cvalue=cvalue2-cvalue1;	
		
		*kvalueaddr=kvalue;
		*cvalueaddr=cvalue;
		}

}
void Send_Power_Calibrate_Response(uint8 fail_type)
{
	uint8 *temp_buf;//??????
	uint8 send_cmd;//????
	uint8 i;//????
	UintType check_sum;//???
	uint8 send_ptr;//????
	uint8 respond_state;//????
	
	temp_buf=(uint8*)malloc(20);
	
	send_ptr=0;	
//	*(temp_buf+(send_ptr++))=vtr_label;
//	*(temp_buf+(send_ptr++))=vrs_sequence;

	send_cmd=0x52;
	*(temp_buf+(send_ptr++))=send_cmd;

	respond_state=fail_type;
	if(respond_state==0x00)
	{
		*(temp_buf+(send_ptr++))=suc_respond;//????
		*(temp_buf+(send_ptr++))=suc_type;//????
	}
	else
	{
		*(temp_buf+(send_ptr++))=fail_respond;//????
		*(temp_buf+(send_ptr++))=fail_type;//????
	}
	
	check_sum.w=0;
	for(i=2;i<send_ptr;i++)
	{
		check_sum.w=check_sum.w+*(temp_buf+i);
	}
	*(temp_buf+(send_ptr++))=check_sum.bb[1];
	*(temp_buf+(send_ptr++))=check_sum.bb[0];
	
	//uart0_data_pc(temp_buf,send_ptr);
	
	free(temp_buf);	
}


/****************************************************************************
* 名称：Vtr_after_save_data_deal_with
* 功能：校准命令参数保存后，进行的必要操作。功率校准:输出功率值等等
* 入口参数：VTR_CALIBRATE_TMP_STRCUT* tmpstruct:校准数据结构体指针
* 出口参数：无
****************************************************************************/
void Vtr_after_save_data_deal_with(VTR_CALIBRATE_TMP_STRCUT* tmpstruct)
{
	uint8 acountkcflg;//KC????,1??KC??????????,2??rssi?????
	uint16 paramdata;// ???????,
	
	acountkcflg=Calibrate_Get_AcountKC_FLG_By_Fn(tmpstruct->fn);
	switch(acountkcflg)
		{
		case 1://功率校准命令
			paramdata=(uint16)(*((uint16*)tmpstruct->saveptr));
			//DAC_OUT((*(txpar+offset)).w, 0x00, 0x00, 0x00); 			
//			DAC_OUT(paramdata, 0x00, 0x00, 0x00); 			
			//delay_208us(40);
			//reset_adc_scan();	
			break;
			case 2://场强校准命令
//			delay_208us(40);
//			reset_adc_scan();	
			break;
		case 3://温度校准校准命令
//			delay_208us(40);
			//reset_adc_scan();	

			break;
		case 4://静噪校准校准命令
			paramdata=(uint16)(*((uint16*)tmpstruct->saveptr));
//			set_mutelevel_pot=mutelevel_hex2dec(paramdata&0xFF);
//			I22C_WriteNByte(MCP4651_1,ONE_BYTE_SUBA,MUTE,&set_mutelevel_pot,1);//?U5_1(mute)?????RAM 0x80
			
			break;
		case 5://音频校准校准命令
			//Vtr_Voice_Calibrate_deal_with(tmpstruct->saveptr);
			break;
			case 6:// POT校准命令
			//Vtr_POT_Calibrate_deal_with(tmpstruct->saveptr);
		default:

			break;
		}
}

void mzm_kprintf(uint8 id,uint8* str,uint16 data,uint8* str1,uint16 data1,uint8 flg)
{
#if 0
	uint8 tmpbuf[160];

//	LCD_PutString_New(5,16*id,(uint8 *)"                                                 ",FONT16|ZT_CS,(uint8*)GIMAGE_PDTPRESENT,YELLOW);
	if(flg==0)
	{
		sprintf(tmpbuf,"[%d]  [%d][%d]  [%x][%x]",id,data,data1,data,data1);
	}
	else if(flg==1)
	{
		sprintf(tmpbuf,"[%d] [%s]:[%x] [%s]:[%x]",id,str,data,str1,data1);
	}
	else{
		sprintf(tmpbuf,"[%d] [%d][%d]",id,data,data1);
		}

	//LCD_PutString_New(5,16*id,(uint8 *)tmpbuf,FONT16|ZT_CS,(uint8*)GIMAGE_PDTPRESENT,YELLOW);
	
	//Delay_10_MS();
	//Delay_10_MS();
	#endif
}


/****************************************************************************
* ??:Vtr_Acount_Power_KC
* ??:?????????,KC????calibrate_KC_strcut???
* ????:(VTR_CALIBRATE_TMP_STRCUT* tmpstruct:???????
* ????:?
****************************************************************************/
uint8 Vtr_Acount_FWD_RSSI_KC_user(VTR_CALIBRATE_TMP_STRCUT* tmpstruct)
{
	uint16 pre_tmp_pvalue=0;
	uint16 cur_tmp_pvalue=0;
	uint16* advaluecur=(uint16*)tmpstruct->saveptr;
	uint16* advaluelast;
	uint8 acountkcflg;//KC????,1??KC??????????,2??rssi?????
	uint16 firstvalue,secondvalue;//????,?????
	fp64* kvalueaddr;//????,??K??????
	fp64* cvalueaddr;//????,??C??????
	fp64  kvalue,cvalue,cvalue1,cvalue2;//????
	uint16 nboffset;// KC????:??????????
	
	VTR_CALIBRATE_KC_STRCUT *calibrate_KC_strcut = Get_RTU_dev_ptr_KC_struct();
	
	acountkcflg=Calibrate_Get_AcountKC_FLG_By_Fn(tmpstruct->fn);
	if(acountkcflg!=1&&acountkcflg!=2)
	{
		return RET_FAIL_F0; //?????,????
	}


	if(acountkcflg==2)//????kc,???????
	{
		nboffset=tmpstruct->wbnbflg *(PARAM_OFFSET(RSSI_STRUCT_KC,rx_rssi_nb_k1)-PARAM_OFFSET(RSSI_STRUCT_KC,rx_rssi_wb_k1));
		kvalueaddr=(fp64*)&calibrate_KC_strcut->Rssi_KC_Value.rx_rssi_wb_k1+nboffset;//??k?????
		cvalueaddr=(fp64*)&calibrate_KC_strcut->Rssi_KC_Value.rx_rssi_wb_c1+nboffset;//??c?????
	}
	else if(acountkcflg==1)//??????kc,?????????
	{		
		kvalueaddr=(fp64*)&calibrate_KC_strcut->Fwd_KC_Value[tmpstruct->saveoffset].KC_k1;//??k?????
		cvalueaddr=(fp64*)&calibrate_KC_strcut->Fwd_KC_Value[tmpstruct->saveoffset].KC_c1;//??c?????
	}
	firstvalue=Calibrate_Get_AcountKC_FirstValue_By_Fn(tmpstruct->fn);//?? ??????
	secondvalue=Calibrate_Get_AcountKC_SecondValue_By_Fn(tmpstruct->fn);//?????????
		
	if(firstvalue!=0&&secondvalue!=0)
	{
		advaluelast = advaluecur-1;//???0 ????a????????,???????-1?????????,?????
		pre_tmp_pvalue=*advaluelast;
		cur_tmp_pvalue=*advaluecur;
		/*????????????,?????,??????????????:A ??,??tmpstrct->cmdoffset ????0
			tmpstruct->cmdoffset,??????ABCD..???????2??,??cmdnum
			?2??????,
			?????sizeof(UinthlType),?????????;A-0,B-1,C-2,D-3,,,,
			???? -1???B?? ?? K1?C1,C?? ??K2-C2??
		*/
		kvalueaddr+=(tmpstruct->cmdoffset/sizeof(UinthlType)-1)*2;//?????????k???
		cvalueaddr+=(tmpstruct->cmdoffset/sizeof(UinthlType)-1)*2;//?????????c???
	}
	else{
		return RET_FAIL_F0;
		}
	if(pre_tmp_pvalue>=cur_tmp_pvalue)
	{
		Send_Power_Calibrate_Response(fail_type1);
	}
	else{
		// y=Kx+c
		// y=Kx+c
			kvalue=(fp64)(secondvalue-firstvalue)/(cur_tmp_pvalue-pre_tmp_pvalue);//??K?
			cvalue1=(fp64)kvalue*cur_tmp_pvalue;						
			cvalue2=(fp64)secondvalue;						
			cvalue=cvalue2-cvalue1;//??C ?,???????C ?
		
			*kvalueaddr=kvalue;
			*cvalueaddr=cvalue;

		}

}



/****************************************************************************
* ??:Vtr_Acount_Power_KC
* ??:?????????,KC????calibrate_KC_strcut???
* ????:(VTR_CALIBRATE_TMP_STRCUT* tmpstruct:???????
* ????:?
****************************************************************************/
uint8 Vtr_Acount_Fwd_KC_user(VTR_CALIBRATE_TMP_STRCUT* tmpstruct)
{
	uint16 pre_tmp_pvalue=0;
	uint16 cur_tmp_pvalue=0;
	uint16* advaluecur=(uint16*)tmpstruct->saveptr;
	uint16* advaluelast;
	uint8 acountkcflg;//KC????,1??KC??????????,2??rssi?????
	uint16 firstvalue,secondvalue;//????,?????
	fp64* kvalueaddr;//????,??K??????
	fp64* cvalueaddr;//????,??C??????
	fp64  kvalue,cvalue,cvalue1,cvalue2;//????
	uint16 nboffset;// KC????:??????????
	
	VTR_CALIBRATE_KC_STRCUT *calibrate_KC_strcut = Get_RTU_dev_ptr_KC_struct();
	
	acountkcflg=Calibrate_Get_AcountKC_FLG_By_Fn(tmpstruct->fn);
	if(acountkcflg!=1)
	{
		return RET_FAIL_F0; //?????,????
	}
		
	kvalueaddr=(fp64*)&calibrate_KC_strcut->Fwd_KC_Value[tmpstruct->saveoffset].KC_k1;//??k?????
	cvalueaddr=(fp64*)&calibrate_KC_strcut->Fwd_KC_Value[tmpstruct->saveoffset].KC_c1;//??c?????
	
	firstvalue=Calibrate_Get_AcountKC_FirstValue_By_Fn(tmpstruct->fn);//?? ??????
	secondvalue=Calibrate_Get_AcountKC_SecondValue_By_Fn(tmpstruct->fn);//?????????
		
	if(firstvalue!=0&&secondvalue!=0)
	{
		advaluelast = advaluecur-1;//???0 ????a????????,???????-1?????????,?????
		pre_tmp_pvalue=*advaluelast;
		cur_tmp_pvalue=*advaluecur;
		/*????????????,?????,??????????????:A ??,??tmpstrct->cmdoffset ????0
			tmpstruct->cmdoffset,??????ABCD..???????2??,??cmdnum
			?2??????,
			?????sizeof(UinthlType),?????????;A-0,B-1,C-2,D-3,,,,
			???? -1???B?? ?? K1?C1,C?? ??K2-C2??
		*/
		kvalueaddr+=(tmpstruct->cmdoffset/sizeof(UinthlType)-1)*2;//?????????k???
		cvalueaddr+=(tmpstruct->cmdoffset/sizeof(UinthlType)-1)*2;//?????????c???
	}
	else{
		return RET_FAIL_F0;
		}
	if(pre_tmp_pvalue>=cur_tmp_pvalue)
	{
		Send_Power_Calibrate_Response(fail_type1);
	}
	else{
		// y=Kx+c
		// y=Kx+c
			kvalue=(fp64)(secondvalue-firstvalue)/(cur_tmp_pvalue-pre_tmp_pvalue);//??K?
			cvalue1=(fp64)kvalue*cur_tmp_pvalue;						
			cvalue2=(fp64)secondvalue;						
			cvalue=cvalue2-cvalue1;//??C ?,???????C ?
		
			*kvalueaddr=kvalue;
			*cvalueaddr=cvalue;

		}

}
/****************************************************************************
* 名称：Vtr_Acount_Rssi_Fwd_KC
* 功能：计算场强、前向功率的增益，KC并保存到calibrate_KC_strcut机构中
* 入口参数：(VTR_CALIBRATE_TMP_STRCUT* tmpstruct:校准结构体指针
* 出口参数：无
****************************************************************************/
uint8 Vtr_Acount_Rssi_Fwd_KC(VTR_CALIBRATE_TMP_STRCUT* tmpstruct)
{
	//uint32_t addr1_data_buf[200];
	uint16 pre_tmp_pvalue=0;
	uint16 cur_tmp_pvalue=0;
	uint16* advaluecur=(uint16*)tmpstruct->adcurvalue;
	uint16* advaluelast;
	uint8 acountkcflg;//KC计算标志，1计算KC且保存前向功率当前值，2保存rssi场强当前值
	uint16 firstvalue,secondvalue;//第一个值，和第二个值
	fp64* kvalueaddr;//结构体中，保存K值的地址指针
	fp64* cvalueaddr;//结构体重，保存C值的地址指针
	fp64  kvalue,cvalue,cvalue1,cvalue2;//临时变量
	uint16 nboffset;// KC起始位置:窄带相对于宽带的偏移
	
	VTR_CALIBRATE_KC_STRCUT *calibrate_KC_strcut = Get_RTU_dev_ptr_KC_struct();
	RTU_PARAM_NOT_SAVE_ADVALUE * rtu_advalue = Get_RTU_dev_ptr_Adc_value();
	
	acountkcflg=Calibrate_Get_AcountKC_FLG_By_Fn(tmpstruct->fn);
	if(acountkcflg!=1&&acountkcflg!=2)
	{
		return RET_FAIL_F0; //不需要校准，直接返回
	}
	
	rtu_advalue->converter=0;//重新开始计数转换

	while(rtu_advalue->converter==0)
	{
		;
	}
	
	
	if(acountkcflg==2)//计算场强kc，并保存当前场强
		{
			*(advaluecur) = rtu_advalue->Advalue[RSSI_ADC_CH].w;//保存当前值
			nboffset=tmpstruct->wbnbflg *(PARAM_OFFSET(RSSI_STRUCT_KC,rx_rssi_nb_k1)-PARAM_OFFSET(RSSI_STRUCT_KC,rx_rssi_wb_k1));
			kvalueaddr=(fp64*)&calibrate_KC_strcut->Rssi_KC_Value.rx_rssi_wb_k1+nboffset;//获取k的起始位置
			cvalueaddr=(fp64*)&calibrate_KC_strcut->Rssi_KC_Value.rx_rssi_wb_c1+nboffset;//获取c的起始位置
		}
		else if(acountkcflg==1)//计算前线功率kc，保存当前前向功率值
		{
		//	*(advaluecur) =Vtr_AdcValue.FWD_P.w;//保存当前值
		*(advaluecur) = rtu_advalue->Advalue[PV_FWD_ADC_CH].w;//保存当前值
			kvalueaddr=(fp64*)&calibrate_KC_strcut->Fwd_KC_Value[tmpstruct->saveoffset].KC_k1;//获取k的起始位置
			cvalueaddr=(fp64*)&calibrate_KC_strcut->Fwd_KC_Value[tmpstruct->saveoffset].KC_c1;//获取c的起始位置
		}
		
		firstvalue=Calibrate_Get_AcountKC_FirstValue_By_Fn(tmpstruct->fn);//获取 第一个点数值
		secondvalue=Calibrate_Get_AcountKC_SecondValue_By_Fn(tmpstruct->fn);//获取第二个点的数值
			
		if(firstvalue!=0&&secondvalue!=0)
		{
			advaluelast = advaluecur-1;//都不为0 表明校准a命令已经执行完毕，可以通过当前值-1的方式获得上次的值，不会越界。
			pre_tmp_pvalue=*advaluelast;
			cur_tmp_pvalue=*advaluecur;
			/*上面的判断条件已经保证了，执行到这里，当前校准命令不会是第一个命令:A 命令，所以tmpstrct->cmdnum 必定大于0
				tmpstruct->cmdnum，是校准命令的ABCD..的偏移每个命令2字节，所以cmdnum
				是2个字节的倍数，
				所以要除以sizeof(UinthlType)，获得命令的对应关系；A-0,B-1,C-2,D-3,,,,
				之所以要 -1是因为B命令 对应 K1和C1,C命令 对应K2-C2。。
			*/
			kvalueaddr+=(tmpstruct->cmdoffset/sizeof(UinthlType)-1)*2;//获取保存当前命令的k的位置
			cvalueaddr+=(tmpstruct->cmdoffset/sizeof(UinthlType)-1)*2;//获取保存当前命令的c的位置
		}
		else{
			return RET_FAIL_F0;
			}
		if(pre_tmp_pvalue>=cur_tmp_pvalue)
		{
			//功放有问题，提示报警
		}
		else{
			// y=Kx+c
			kvalue=(fp64)(secondvalue-firstvalue)/(cur_tmp_pvalue-pre_tmp_pvalue);//计算K值
			cvalue1=(fp64)kvalue*cur_tmp_pvalue;						
			cvalue2=(fp64)secondvalue;						
			cvalue=cvalue2-cvalue1;//计算C 值，带入第二个点求C 值
	
			*kvalueaddr=kvalue;
			*cvalueaddr=cvalue;
			}

}



/****************************************************************************
* ??:Vtr_set_channel_info_calibrate
* ??:?????:????????????????,?????????
		???????????,??????????,??????????
* ????:(VTR_CALIBRATE_TMP_STRCUT* tmpstruct:???????
* ????:?
****************************************************************************/
void Vtr_set_channel_info_calibrate(uint8 freqnum,uint8 wbnbstate_v,uint8 chwork_v)//???  ????
{
	
	//uint32 i;
	uint32 txfre_data,rxfre_data;
//	uint16 i;

//while(1)
////{
//	vtr_ch_info_current.vtrchannelinfo.name.ascbuf[0]=' ';
//	vtr_ch_info_current.vtrchannelinfo.name.ascbuf[1]=' ';
//	vtr_ch_info_current.vtrchannelinfo.name.ascbuf[2]=' ';
//	vtr_ch_info_current.vtrchannelinfo.name.ascbuf[3]=' ';	
//	vtr_ch_info_current.vtrchannelinfo.name.ascbuf[4]=' ';
//	vtr_ch_info_current.vtrchannelinfo.name.ascbuf[5]='C';
//	vtr_ch_info_current.vtrchannelinfo.name.ascbuf[6]='H';
//	vtr_ch_info_current.vtrchannelinfo.name.ascbuf[7]='1';	

//	vtr_ch_info_current.vtrchannelinfo.number.property=2 ;
//	
//	vtr_ch_info_current.vtrchannelinfo.edit.property=1;
//	
//	vtr_ch_info_current.vtrchannelinfo.txfre.propertyint.w=(uint16)(freqnum +FREQ_NUM_ADDEND);//????
//	vtr_ch_info_current.vtrchannelinfo.txfre.propertyfrac.w=0;
//			
//	vtr_ch_info_current.vtrchannelinfo.rxfre.propertyint.w=(uint16)(freqnum +FREQ_NUM_ADDEND-10);//???? ???????10M
//	vtr_ch_info_current.vtrchannelinfo.rxfre.propertyfrac.w=0;	
//	

//		
//	txfre_data=(vtr_ch_info_current.vtrchannelinfo.txfre.propertyint.w)*1000000+\
//	(vtr_ch_info_current.vtrchannelinfo.txfre.propertyfrac.w)*125;
//	rxfre_data=(vtr_ch_info_current.vtrchannelinfo.rxfre.propertyint.w)*1000000+\
//	(vtr_ch_info_current.vtrchannelinfo.rxfre.propertyfrac.w)*125;

//		switch(vtr_accessoriestype)
//		{
//			case 0x03:
//				Set_SKY72310_LD(1,txfre_data,19200000,  0,0,3,0,0x0f,0,0);//tx
//				Set_SKY72310_LD(1,txfre_data,19200000,  0,0,3,0,0x0f,0,0);//tx
//				check_tx_lock_flag=1;
//				break;
//			default:
//				Set_SKY72310_LD(1,txfre_data,19200152,  0,0,3,0,0x0f,0,0);//tx
//				Set_SKY72310_LD(1,txfre_data,19200152,  0,0,3,0,0x0f,0,0);//tx
//				check_tx_lock_flag=1;
//				break;
//			#if 0
//            case 0x00:
//                if(vtr_work_state==0x01)//????
//                {
//				Set_SKY72310_LD(1,txfre_data,19200152,  0,0,3,0,0x0f,0,0);//tx
//				Set_SKY72310_LD(1,txfre_data,19200152,  0,0,3,0,0x0f,0,0);//tx
//				check_tx_lock_flag=1;                
//                }
//                break;
//			#endif
//		}			
//		
//		//delay_208us(CHECK_TX_LD_TIME);
//		//check_tx_lock_state();//??????????PTT,???		

//		Set_SKY72310_LD(2,rxfre_data,19200000,  0,0,3,0,0x0f,0,0);//rx
//		Set_SKY72310_LD(2,rxfre_data,19200000,  0,0,3,0,0x0f,0,0);//rx	
//		ldstate_scan_cnt=0;
//		ldstate_scan_f=0;
//		check_rx_lock_flag=1;
//		//delay_208us(CHECK_RX_LD_TIME);
//		//check_rx_lock_state();		

//	//??????

//	vtr_ch_info_current.vtrchannelinfo.reservevalue1=0xff;
//	vtr_ch_info_current.vtrchannelinfo.txhtone.property=0x02;//??
//	vtr_ch_info_current.vtrchannelinfo.txhtonevalue.propertyint.wlh.high=(uint8)(1>>8);
//	vtr_ch_info_current.vtrchannelinfo.txhtonevalue.propertyint.wlh.low=(uint8)(1);

//	vtr_ch_info_current.vtrchannelinfo.reservevalue2=0xff;
// 	vtr_ch_info_current.vtrchannelinfo.rxhtone.property=0x02;//??
//	vtr_ch_info_current.vtrchannelinfo.rxhtonevalue.propertyint.wlh.high=(uint8)(51>>8);
//	vtr_ch_info_current.vtrchannelinfo.rxhtonevalue.propertyint.wlh.low=(uint8)(51);

//	vtr_ch_info_current.vtrchannelinfo.txpower.property=5;//POWER_VALUE_5W;  ?????txpowerpot,???5W???
//	//vtr_ch_info_current.vtrchannelinfo.txpowerpot.propertyint.w=change_power_to_powerpot_363M(vtr_ch_info_current.vtrchannelinfo.txpower.property);	
//	//DAC_OUT(vtr_ch_info_current.vtrchannelinfo.txpowerpot.property,0x00,0x00,0x00);//dac_tx_pwr
//	
//	vtr_ch_info_current.vtrchannelinfo.refoscpot.property=0x80;
//	vtr_ch_info_current.vtrchannelinfo.vmodpot.property=0x80;
//	vtr_ch_info_current.vtrchannelinfo.rmodpot.property=0x80;

//	vtr_ch_info_current.vtrchannelinfo.fwdp.propertyintfrac.whl.high=25;
//	vtr_ch_info_current.vtrchannelinfo.fwdp.propertyintfrac.whl.low=10;// 25.1
//	
//	vtr_ch_info_current.vtrchannelinfo.refp.propertyintfrac.whl.high=2;
//	vtr_ch_info_current.vtrchannelinfo.refp.propertyintfrac.whl.low=10;// 2.1
//	
//	vtr_ch_info_current.vtrchannelinfo.swrp.propertyintfrac.whl.high=1;
//	vtr_ch_info_current.vtrchannelinfo.swrp.propertyintfrac.whl.low=05;// 1.05
//	
//	vtr_ch_info_current.vtrchannelinfo.rssip.propertyintfrac.whl.high=0xe4;
//	vtr_ch_info_current.vtrchannelinfo.rssip.propertyintfrac.whl.low=0;//-100dbm

//	vtr_ch_info_current.vtrchannelinfo.pttstate.property=0x00;
//	vtr_ch_info_current.vtrchannelinfo.mutestate.property=0x01;

//	vtr_ch_info_current.vtrchannelinfo.patemp.propertyintfrac.whl.high=25;
//	vtr_ch_info_current.vtrchannelinfo.patemp.propertyintfrac.whl.low=5;// 25.5?

//	vtr_ch_info_current.vtrchannelinfo.rxvcov.propertyintfrac.whl.high=(uint8)(256>>8);
//	vtr_ch_info_current.vtrchannelinfo.rxvcov.propertyintfrac.whl.low=(uint8)(256);
//	vtr_ch_info_current.vtrchannelinfo.txvcov.propertyintfrac.whl.high=(uint8)(257>>8);	
//	vtr_ch_info_current.vtrchannelinfo.txvcov.propertyintfrac.whl.low=(uint8)(257);

//	vtr_ch_info_current.vtrchannelinfo.pavoltage.propertyintfrac.whl.high=13;
//	vtr_ch_info_current.vtrchannelinfo.pavoltage.propertyintfrac.whl.low=80;// 13.8v

//	vtr_ch_info_current.vtrchannelinfo.alarmstate.propertyint.w=0;

//	vtr_ch_info_current.vtrchannelinfo.ledstate.property=0x00;

//	vtr_ch_info_current.vtrchannelinfo.chwork.property=chwork_v;//????????????,?????????,?????
//	switch(vtr_ch_info_current.vtrchannelinfo.chwork.property)
//	{
//		#if clear_dangong
//		case 0x00://??    ??,??;??,??
//			#if 0
//			if((Read_P2()&MUTE_IN)==0)//?????
//			{
//				ttr_resume(0);//????????
//				paptt_resume();//??????						
//			}
//			else
//			{
//				
//			}			
//			break;
//			#endif
//			vtr_io_switch_normal.trcmodefled_ctl=0;
//			TRCMODE_FLED_OFF();			
//			break;
//		#endif
//		case 0x01://??
//			#if 0
//			if((Read_P2()&MUTE_IN)==0)//?????
//			{
//				ttr_resume(0);//????????
//				paptt_resume();//??????						
//			}
//			else
//			{
//				
//			}		
//			#endif
//			vtr_io_switch_normal.trcmodefled_ctl=0;
//			TRCMODE_FLED_OFF();
//			break;
//		case 0x02://??
//			vtr_io_switch_normal.trcmodefled_ctl=1;
//			TRCMODE_FLED_ON();
//			break;
//		default:
//			vtr_io_switch_normal.trcmodefled_ctl=0;
//			TRCMODE_FLED_OFF();		
//			break;
//	}	

//	vtr_ch_info_current.vtrchannelinfo.fanstate.property=0x00;	

//	vtr_ch_info_current.vtrchannelinfo.wbnbstate.property=wbnbstate_v;//??
//	switch(vtr_ch_info_current.vtrchannelinfo.wbnbstate.property)
//	{
//		case 0x00://??
//			vtr_io_switch_conner.rxwbnb_ctl=1;
//			RXWBNB_ON();
//			vtr_io_switch_conner.trcnbwb_ctl=1;
//			TRCNBWB_ON();
//			break;
//		case 0x01://??
//			vtr_io_switch_conner.rxwbnb_ctl=0;
//			RXWBNB_OFF();//mute_cont,filter
//			vtr_io_switch_conner.trcnbwb_ctl=0;
//			TRCNBWB_OFF();//dec agc			
//			break;
//		case 0x02://PDT??
//			vtr_io_switch_conner.rxwbnb_ctl=0;
//			RXWBNB_OFF();//mute_cont,filter
//			vtr_io_switch_conner.trcnbwb_ctl=1;
//			TRCNBWB_ON();//dec agc					
//			break;
//		default://??
//			vtr_io_switch_conner.rxwbnb_ctl=1;
//			RXWBNB_ON();
//			vtr_io_switch_conner.trcnbwb_ctl=1;
//			TRCNBWB_ON();		
//			break;
//	}

//	vtr_ch_info_current.vtrchannelinfo.scantype.property=0x00;

//	vtr_ch_info_current.vtrchannelinfo.reservevalue3=0xff;

//	vtr_ch_info_current.vtrchannelinfo.txpowerpot.propertyint.w=POWER_VALUE_5W;// ??
//	
//	vtr_ch_info_current.vtrchannelinfo.txpowerpothalf.propertyint.w=POWER_VALUE_5W;

}

void set_channel_info_test(uint8 channel_number,uint8 wbnbstate_v,uint8 chwork_v)//???  ????
{
	
//	//uint32 i;
//	uint32 txfre_data,rxfre_data;
////	uint16 i;

////while(1)
////{
//	vtr_ch_info_current.vtrchannelinfo.name.ascbuf[0]=' ';
//	vtr_ch_info_current.vtrchannelinfo.name.ascbuf[1]=' ';
//	vtr_ch_info_current.vtrchannelinfo.name.ascbuf[2]=' ';
//	vtr_ch_info_current.vtrchannelinfo.name.ascbuf[3]=' ';	
//	vtr_ch_info_current.vtrchannelinfo.name.ascbuf[4]=' ';
//	vtr_ch_info_current.vtrchannelinfo.name.ascbuf[5]='C';
//	vtr_ch_info_current.vtrchannelinfo.name.ascbuf[6]='H';
//	vtr_ch_info_current.vtrchannelinfo.name.ascbuf[7]='1';	

//	vtr_ch_info_current.vtrchannelinfo.number.property=channel_number;
//	
//	vtr_ch_info_current.vtrchannelinfo.edit.property=1;
//	switch(channel_number)
//	{
//		case 1://361--351
//		case 2://363--353
//		case 3://365--355
//		case 4://367--357		
//			vtr_ch_info_current.vtrchannelinfo.txfre.propertyint.whl.high=(uint8)((360+channel_number*2-1)>>8);
//			vtr_ch_info_current.vtrchannelinfo.txfre.propertyint.whl.low=(uint8)((360+channel_number*2-1));
//			vtr_ch_info_current.vtrchannelinfo.txfre.propertyfrac.w=0;
//			
//			vtr_ch_info_current.vtrchannelinfo.rxfre.propertyint.whl.high=(uint8)((350+channel_number*2-1)>>8);
//			vtr_ch_info_current.vtrchannelinfo.rxfre.propertyint.whl.low=(uint8)((350+channel_number*2-1));
//			vtr_ch_info_current.vtrchannelinfo.rxfre.propertyfrac.w=0;	
//		break;
//		default:
//			break;
//	}

//		txfre_data=(vtr_ch_info_current.vtrchannelinfo.txfre.propertyint.whl.high*256+vtr_ch_info_current.vtrchannelinfo.txfre.propertyint.whl.low)*1000000+\
//		(vtr_ch_info_current.vtrchannelinfo.txfre.propertyfrac.whl.high*256+vtr_ch_info_current.vtrchannelinfo.txfre.propertyfrac.whl.low)*125;
//		rxfre_data=(vtr_ch_info_current.vtrchannelinfo.rxfre.propertyint.whl.high*256+vtr_ch_info_current.vtrchannelinfo.rxfre.propertyint.whl.low)*1000000+\
//		(vtr_ch_info_current.vtrchannelinfo.rxfre.propertyfrac.whl.high*256+vtr_ch_info_current.vtrchannelinfo.rxfre.propertyfrac.whl.low)*125;

//		switch(vtr_accessoriestype)
//		{
//			case 0x03:
//				Set_SKY72310_LD(1,txfre_data,19200000,  0,0,3,0,0x0f,0,0);//tx
//				Set_SKY72310_LD(1,txfre_data,19200000,  0,0,3,0,0x0f,0,0);//tx
//				check_tx_lock_flag=1;
//				break;
//			default:
//				Set_SKY72310_LD(1,txfre_data,19200152,  0,0,3,0,0x0f,0,0);//tx
//				Set_SKY72310_LD(1,txfre_data,19200152,  0,0,3,0,0x0f,0,0);//tx
//				check_tx_lock_flag=1;
//				break;
//			#if 0
//            case 0x00:
//                if(vtr_work_state==0x01)//????
//                {
//				Set_SKY72310_LD(1,txfre_data,19200152,  0,0,3,0,0x0f,0,0);//tx
//				Set_SKY72310_LD(1,txfre_data,19200152,  0,0,3,0,0x0f,0,0);//tx
//				check_tx_lock_flag=1;                
//                }
//                break;
//			#endif
//		}			
//		
//		//delay_208us(CHECK_TX_LD_TIME);
//		//check_tx_lock_state();//??????????PTT,???		

//		Set_SKY72310_LD(2,rxfre_data,19200000,  0,0,3,0,0x0f,0,0);//rx
//		Set_SKY72310_LD(2,rxfre_data,19200000,  0,0,3,0,0x0f,0,0);//rx	
//		ldstate_scan_cnt=0;
//		ldstate_scan_f=0;
//		check_rx_lock_flag=1;
//		//delay_208us(CHECK_RX_LD_TIME);
//		//check_rx_lock_state();		

//	//??????

//	vtr_ch_info_current.vtrchannelinfo.reservevalue1=0xff;
//	vtr_ch_info_current.vtrchannelinfo.txhtone.property=0x02;//??
//	vtr_ch_info_current.vtrchannelinfo.txhtonevalue.propertyint.wlh.high=(uint8)(1>>8);
//	vtr_ch_info_current.vtrchannelinfo.txhtonevalue.propertyint.wlh.low=(uint8)(1);

//	vtr_ch_info_current.vtrchannelinfo.reservevalue2=0xff;
// 	vtr_ch_info_current.vtrchannelinfo.rxhtone.property=0x02;//??
//	vtr_ch_info_current.vtrchannelinfo.rxhtonevalue.propertyint.wlh.high=(uint8)(51>>8);
//	vtr_ch_info_current.vtrchannelinfo.rxhtonevalue.propertyint.wlh.low=(uint8)(51);

//	vtr_ch_info_current.vtrchannelinfo.txpower.property=5;//POWER_VALUE_5W;  ?????txpowerpot,???5W???
//	//vtr_ch_info_current.vtrchannelinfo.txpowerpot.propertyint.w=change_power_to_powerpot_363M(vtr_ch_info_current.vtrchannelinfo.txpower.property);	
//	//DAC_OUT(vtr_ch_info_current.vtrchannelinfo.txpowerpot.property,0x00,0x00,0x00);//dac_tx_pwr
//	
//	vtr_ch_info_current.vtrchannelinfo.refoscpot.property=0x80;
//	vtr_ch_info_current.vtrchannelinfo.vmodpot.property=0x80;
//	vtr_ch_info_current.vtrchannelinfo.rmodpot.property=0x80;

//	vtr_ch_info_current.vtrchannelinfo.fwdp.propertyintfrac.whl.high=25;
//	vtr_ch_info_current.vtrchannelinfo.fwdp.propertyintfrac.whl.low=10;// 25.1
//	
//	vtr_ch_info_current.vtrchannelinfo.refp.propertyintfrac.whl.high=2;
//	vtr_ch_info_current.vtrchannelinfo.refp.propertyintfrac.whl.low=10;// 2.1
//	
//	vtr_ch_info_current.vtrchannelinfo.swrp.propertyintfrac.whl.high=1;
//	vtr_ch_info_current.vtrchannelinfo.swrp.propertyintfrac.whl.low=05;// 1.05
//	
//	vtr_ch_info_current.vtrchannelinfo.rssip.propertyintfrac.whl.high=0xe4;
//	vtr_ch_info_current.vtrchannelinfo.rssip.propertyintfrac.whl.low=0;//-100dbm

//	vtr_ch_info_current.vtrchannelinfo.pttstate.property=0x00;
//	vtr_ch_info_current.vtrchannelinfo.mutestate.property=0x01;

//	vtr_ch_info_current.vtrchannelinfo.patemp.propertyintfrac.whl.high=25;
//	vtr_ch_info_current.vtrchannelinfo.patemp.propertyintfrac.whl.low=5;// 25.5?

//	vtr_ch_info_current.vtrchannelinfo.rxvcov.propertyintfrac.whl.high=(uint8)(256>>8);
//	vtr_ch_info_current.vtrchannelinfo.rxvcov.propertyintfrac.whl.low=(uint8)(256);
//	vtr_ch_info_current.vtrchannelinfo.txvcov.propertyintfrac.whl.high=(uint8)(257>>8);	
//	vtr_ch_info_current.vtrchannelinfo.txvcov.propertyintfrac.whl.low=(uint8)(257);

//	vtr_ch_info_current.vtrchannelinfo.pavoltage.propertyintfrac.whl.high=13;
//	vtr_ch_info_current.vtrchannelinfo.pavoltage.propertyintfrac.whl.low=80;// 13.8v

//	vtr_ch_info_current.vtrchannelinfo.alarmstate.propertyint.w=0;

//	vtr_ch_info_current.vtrchannelinfo.ledstate.property=0x00;

//	vtr_ch_info_current.vtrchannelinfo.chwork.property=chwork_v;//????????????,?????????,?????
//	switch(vtr_ch_info_current.vtrchannelinfo.chwork.property)
//	{
//		#if clear_dangong
//		case 0x00://??    ??,??;??,??
//			#if 0
//			if((Read_P2()&MUTE_IN)==0)//?????
//			{
//				ttr_resume(0);//????????
//				paptt_resume();//??????						
//			}
//			else
//			{
//				
//			}			
//			break;
//			#endif
//			vtr_io_switch_normal.trcmodefled_ctl=0;
//			TRCMODE_FLED_OFF();			
//			break;
//		#endif
//		case 0x01://??
//			#if 0
//			if((Read_P2()&MUTE_IN)==0)//?????
//			{
//				ttr_resume(0);//????????
//				paptt_resume();//??????						
//			}
//			else
//			{
//				
//			}		
//			#endif
//			vtr_io_switch_normal.trcmodefled_ctl=0;
//			TRCMODE_FLED_OFF();
//			break;
//		case 0x02://??
//			vtr_io_switch_normal.trcmodefled_ctl=1;
//			TRCMODE_FLED_ON();
//			break;
//		default:
//			vtr_io_switch_normal.trcmodefled_ctl=0;
//			TRCMODE_FLED_OFF();		
//			break;
//	}	

//	vtr_ch_info_current.vtrchannelinfo.fanstate.property=0x00;	

//	vtr_ch_info_current.vtrchannelinfo.wbnbstate.property=wbnbstate_v;//??
//	switch(vtr_ch_info_current.vtrchannelinfo.wbnbstate.property)
//	{
//		case 0x00://??
//			vtr_io_switch_conner.rxwbnb_ctl=1;
//			RXWBNB_ON();
//			vtr_io_switch_conner.trcnbwb_ctl=1;
//			TRCNBWB_ON();
//			break;
//		case 0x01://??
//			vtr_io_switch_conner.rxwbnb_ctl=0;
//			RXWBNB_OFF();//mute_cont,filter
//			vtr_io_switch_conner.trcnbwb_ctl=0;
//			TRCNBWB_OFF();//dec agc			
//			break;
//		case 0x02://PDT??
//			vtr_io_switch_conner.rxwbnb_ctl=0;
//			RXWBNB_OFF();//mute_cont,filter
//			vtr_io_switch_conner.trcnbwb_ctl=1;
//			TRCNBWB_ON();//dec agc					
//			break;
//		default://??
//			vtr_io_switch_conner.rxwbnb_ctl=1;
//			RXWBNB_ON();
//			vtr_io_switch_conner.trcnbwb_ctl=1;
//			TRCNBWB_ON();		
//			break;
//	}

//	vtr_ch_info_current.vtrchannelinfo.scantype.property=0x00;

//	vtr_ch_info_current.vtrchannelinfo.reservevalue3=0xff;

//	vtr_ch_info_current.vtrchannelinfo.txpowerpot.propertyint.w=POWER_VALUE_5W;// ??
//	
//	vtr_ch_info_current.vtrchannelinfo.txpowerpothalf.propertyint.w=POWER_VALUE_5W;

}

void vol_calibrate(void)
{
//	u_long addr1_data_buf[200];
//	uint16 pre_fwd_pvalue=0;
//	uint16 pre_rssi_pvalue=0;

//	memset(addr1_data_buf,0x0,200);
//	
//	while(1)
//	{
//		delay_208us(40);
//	//	Initadc_multiple();

//		addr1_data_buf[adc_hits_cnt]=addr1_data;

//		sum_addr0_data +=addr0_data;
//		sum_addr1_data +=addr1_data;
//		sum_addr2_data +=addr2_data;
//		sum_addr4_data +=addr4_data;
//		sum_addr5_data +=addr5_data;
//		sum_addr6_data +=addr6_data;
//		
//		adc_hits_cnt++;
//		if(adc_hits_cnt==100)
//		{
//			addr0_data=sum_addr0_data/100;
//			addr1_data=sum_addr1_data/100;
//			addr2_data=sum_addr2_data/100;
//			addr4_data=sum_addr4_data/100;
//			addr5_data=sum_addr5_data/100;
//			addr6_data=sum_addr6_data/100;	

//			
//			Vtr_AdcValue.RSSI.w=(addr0_data);//addr0_data??
//			Vtr_AdcValue.FWD_P.w=(addr1_data);
//			Vtr_AdcValue.REFL_P.w=(addr2_data);
//			Vtr_AdcValue.TX_12V.w=(addr4_data);
//			Vtr_AdcValue.TEMP_VOLTS.w=(addr5_data);//???
//			Vtr_AdcValue.PA_VOLTS.w=(addr6_data);

//			adc_hits_cnt=0;

//			break;
//		}
//		
//	}
//	reset_adc_scan();
//			if(rssi_calibrate_wbflag>0)
//			{
//				switch(rssi_calibrate_wbflag)
//				{
//					case 0x01:
//						rssi_calibrate_wbflag=0;
//						vtr_eeprom_rx.rxpar.rssi_mv_a_wb.whl.high=Vtr_AdcValue.RSSI.bb[1];
//						vtr_eeprom_rx.rxpar.rssi_mv_a_wb.whl.low=Vtr_AdcValue.RSSI.bb[0];	
//						#if Debug_rx_rssi_kc	
//						vtr_eeprom_rx.rxpar.rssi_mv_a_wb.whl.high=0x05;
//						vtr_eeprom_rx.rxpar.rssi_mv_a_wb.whl.low=0xdc;	
//						#endif			
//						break;
//					case 0x02:
//						rssi_calibrate_wbflag=0;
//						vtr_eeprom_rx.rxpar.rssi_mv_b_wb.whl.high=Vtr_AdcValue.RSSI.bb[1];
//						vtr_eeprom_rx.rxpar.rssi_mv_b_wb.whl.low=Vtr_AdcValue.RSSI.bb[0];	
//						#if Debug_rx_rssi_kc	
//						Vtr_AdcValue.RSSI.bb[1]=0x04;
//						Vtr_AdcValue.RSSI.bb[0]=0xb0;	
//						vtr_eeprom_rx.rxpar.rssi_mv_b_wb.whl.high=0x04;
//						vtr_eeprom_rx.rxpar.rssi_mv_b_wb.whl.low=0xb0;							
//						#endif							
//						pre_rssi_pvalue=(uint16)(vtr_eeprom_rx.rxpar.rssi_mv_a_wb.whl.high*256+vtr_eeprom_rx.rxpar.rssi_mv_a_wb.whl.low);
//						account_rssi_kcvalue(0x00,0x01,pre_rssi_pvalue,Vtr_AdcValue.RSSI.w);	
//						break;		
//					case 0x03:
//						rssi_calibrate_wbflag=0;
//						vtr_eeprom_rx.rxpar.rssi_mv_c_wb.whl.high=Vtr_AdcValue.RSSI.bb[1];
//						vtr_eeprom_rx.rxpar.rssi_mv_c_wb.whl.low=Vtr_AdcValue.RSSI.bb[0];		
//						#if Debug_rx_rssi_kc	
//						Vtr_AdcValue.RSSI.bb[1]=0x03;
//						Vtr_AdcValue.RSSI.bb[0]=0xe8;	
//						vtr_eeprom_rx.rxpar.rssi_mv_c_wb.whl.high=0x03;
//						vtr_eeprom_rx.rxpar.rssi_mv_c_wb.whl.low=0xe8;								
//						#endif							
//						pre_rssi_pvalue=(uint16)(vtr_eeprom_rx.rxpar.rssi_mv_b_wb.whl.high*256+vtr_eeprom_rx.rxpar.rssi_mv_b_wb.whl.low);
//						account_rssi_kcvalue(0x00,0x02,pre_rssi_pvalue,Vtr_AdcValue.RSSI.w);							
//						break;		
//					case 0x04:
//						rssi_calibrate_wbflag=0;
//						vtr_eeprom_rx.rxpar.rssi_mv_d_wb.whl.high=Vtr_AdcValue.RSSI.bb[1];
//						vtr_eeprom_rx.rxpar.rssi_mv_d_wb.whl.low=Vtr_AdcValue.RSSI.bb[0];	
//						#if Debug_rx_rssi_kc	
//						Vtr_AdcValue.RSSI.bb[1]=0x02;
//						Vtr_AdcValue.RSSI.bb[0]=0x58;	
//						vtr_eeprom_rx.rxpar.rssi_mv_d_wb.whl.high=0x02;
//						vtr_eeprom_rx.rxpar.rssi_mv_d_wb.whl.low=0x58;							
//						#endif							
//						pre_rssi_pvalue=(uint16)(vtr_eeprom_rx.rxpar.rssi_mv_c_wb.whl.high*256+vtr_eeprom_rx.rxpar.rssi_mv_c_wb.whl.low);
//						account_rssi_kcvalue(0x00,0x03,pre_rssi_pvalue,Vtr_AdcValue.RSSI.w);								
//						break;	
//					default:
//						break;						
//				}
//			}
//			else
//			{
//				if(rssi_calibrate_nbflag>0)
//				{
//					switch(rssi_calibrate_nbflag)
//					{
//						case 0x01:
//							rssi_calibrate_nbflag=0;
//							vtr_eeprom_rx.rxpar.rssi_mv_a_nb.whl.high=Vtr_AdcValue.RSSI.bb[1];
//							vtr_eeprom_rx.rxpar.rssi_mv_a_nb.whl.low=Vtr_AdcValue.RSSI.bb[0];	
//							#if Debug_rx_rssi_kc	
//							vtr_eeprom_rx.rxpar.rssi_mv_a_nb.whl.high=0x05;
//							vtr_eeprom_rx.rxpar.rssi_mv_a_nb.whl.low=0xdc;	
//							#endif									
//							break;
//						case 0x02:
//							rssi_calibrate_nbflag=0;
//							vtr_eeprom_rx.rxpar.rssi_mv_b_nb.whl.high=Vtr_AdcValue.RSSI.bb[1];
//							vtr_eeprom_rx.rxpar.rssi_mv_b_nb.whl.low=Vtr_AdcValue.RSSI.bb[0];	
//							#if Debug_rx_rssi_kc	
//							Vtr_AdcValue.RSSI.bb[1]=0x04;
//							Vtr_AdcValue.RSSI.bb[0]=0xb0;	
//							vtr_eeprom_rx.rxpar.rssi_mv_b_nb.whl.high=0x04;
//							vtr_eeprom_rx.rxpar.rssi_mv_b_nb.whl.low=0xb0;							
//							#endif								
//							pre_rssi_pvalue=(uint16)(vtr_eeprom_rx.rxpar.rssi_mv_a_nb.whl.high*256+vtr_eeprom_rx.rxpar.rssi_mv_a_nb.whl.low);
//							account_rssi_kcvalue(0x01,0x01,pre_rssi_pvalue,Vtr_AdcValue.RSSI.w);								
//							break;		
//						case 0x03:
//							rssi_calibrate_nbflag=0;
//							vtr_eeprom_rx.rxpar.rssi_mv_c_nb.whl.high=Vtr_AdcValue.RSSI.bb[1];
//							vtr_eeprom_rx.rxpar.rssi_mv_c_nb.whl.low=Vtr_AdcValue.RSSI.bb[0];		
//							#if Debug_rx_rssi_kc	
//							Vtr_AdcValue.RSSI.bb[1]=0x03;
//							Vtr_AdcValue.RSSI.bb[0]=0xe8;	
//							vtr_eeprom_rx.rxpar.rssi_mv_c_nb.whl.high=0x03;
//							vtr_eeprom_rx.rxpar.rssi_mv_c_nb.whl.low=0xe8;								
//							#endif							
//							pre_rssi_pvalue=(uint16)(vtr_eeprom_rx.rxpar.rssi_mv_b_nb.whl.high*256+vtr_eeprom_rx.rxpar.rssi_mv_b_nb.whl.low);
//							account_rssi_kcvalue(0x01,0x02,pre_rssi_pvalue,Vtr_AdcValue.RSSI.w);									
//							break;		
//						case 0x04:
//							rssi_calibrate_nbflag=0;
//							vtr_eeprom_rx.rxpar.rssi_mv_d_nb.whl.high=Vtr_AdcValue.RSSI.bb[1];
//							vtr_eeprom_rx.rxpar.rssi_mv_d_nb.whl.low=Vtr_AdcValue.RSSI.bb[0];	
//							#if Debug_rx_rssi_kc	
//							Vtr_AdcValue.RSSI.bb[1]=0x02;
//							Vtr_AdcValue.RSSI.bb[0]=0x58;	
//							vtr_eeprom_rx.rxpar.rssi_mv_d_nb.whl.high=0x02;
//							vtr_eeprom_rx.rxpar.rssi_mv_d_nb.whl.low=0x58;							
//							#endif								
//							pre_rssi_pvalue=(uint16)(vtr_eeprom_rx.rxpar.rssi_mv_c_nb.whl.high*256+vtr_eeprom_rx.rxpar.rssi_mv_c_nb.whl.low);
//							account_rssi_kcvalue(0x01,0x03,pre_rssi_pvalue,Vtr_AdcValue.RSSI.w);										
//							break;	
//						default:
//							break;						
//					}					
//				}
//				else
//				{
//				}
//			}	
//			if(power_calibrate_361M_flag>0)
//			{
//				switch(power_calibrate_361M_flag)
//				{
//					case 0x01:
//						power_calibrate_361M_flag=0;
//						vtr_eeprom_tx.txpar.fwd_power_a_361M.whl.high=Vtr_AdcValue.FWD_P.bb[1];
//						vtr_eeprom_tx.txpar.fwd_power_a_361M.whl.low=Vtr_AdcValue.FWD_P.bb[0];		
//						break;
//					case 0x02:
//						power_calibrate_361M_flag=0;		
//						vtr_eeprom_tx.txpar.fwd_power_b_361M.whl.high=Vtr_AdcValue.FWD_P.bb[1];
//						vtr_eeprom_tx.txpar.fwd_power_b_361M.whl.low=Vtr_AdcValue.FWD_P.bb[0];	
//						pre_fwd_pvalue=(uint16)(vtr_eeprom_tx.txpar.fwd_power_a_361M.whl.high*256+vtr_eeprom_tx.txpar.fwd_power_a_361M.whl.low);
//						account_fwdpower_kcvalue(0x00, 0x01,pre_fwd_pvalue, Vtr_AdcValue.FWD_P.w );
//						break;
//					case 0x03:
//						power_calibrate_361M_flag=0;	
//						vtr_eeprom_tx.txpar.fwd_power_c_361M.whl.high=Vtr_AdcValue.FWD_P.bb[1];
//						vtr_eeprom_tx.txpar.fwd_power_c_361M.whl.low=Vtr_AdcValue.FWD_P.bb[0];	
//						pre_fwd_pvalue=(uint16)(vtr_eeprom_tx.txpar.fwd_power_b_361M.whl.high*256+vtr_eeprom_tx.txpar.fwd_power_b_361M.whl.low);
//						account_fwdpower_kcvalue(0x00, 0x02,pre_fwd_pvalue, Vtr_AdcValue.FWD_P.w );						
//						break;
//					case 0x04:
//						power_calibrate_361M_flag=0;	
//						vtr_eeprom_tx.txpar.fwd_power_d_361M.whl.high=Vtr_AdcValue.FWD_P.bb[1];
//						vtr_eeprom_tx.txpar.fwd_power_d_361M.whl.low=Vtr_AdcValue.FWD_P.bb[0];		
//						pre_fwd_pvalue=(uint16)(vtr_eeprom_tx.txpar.fwd_power_c_361M.whl.high*256+vtr_eeprom_tx.txpar.fwd_power_c_361M.whl.low);
//						account_fwdpower_kcvalue(0x00, 0x03,pre_fwd_pvalue, Vtr_AdcValue.FWD_P.w );							
//						break;
//					case 0x05:
//						power_calibrate_361M_flag=0;		
//						vtr_eeprom_tx.txpar.fwd_power_e_361M.whl.high=Vtr_AdcValue.FWD_P.bb[1];
//						vtr_eeprom_tx.txpar.fwd_power_e_361M.whl.low=Vtr_AdcValue.FWD_P.bb[0];	
//						pre_fwd_pvalue=(uint16)(vtr_eeprom_tx.txpar.fwd_power_d_361M.whl.high*256+vtr_eeprom_tx.txpar.fwd_power_d_361M.whl.low);
//						account_fwdpower_kcvalue(0x00, 0x04,pre_fwd_pvalue, Vtr_AdcValue.FWD_P.w );
//						break;
//					case 0x06:
//						power_calibrate_361M_flag=0;	
//						vtr_eeprom_tx.txpar.fwd_power_f_361M.whl.high=Vtr_AdcValue.FWD_P.bb[1];
//						vtr_eeprom_tx.txpar.fwd_power_f_361M.whl.low=Vtr_AdcValue.FWD_P.bb[0];	
//						pre_fwd_pvalue=(uint16)(vtr_eeprom_tx.txpar.fwd_power_d_361M.whl.high*256+vtr_eeprom_tx.txpar.fwd_power_d_361M.whl.low);
//						account_fwdpower_kcvalue(0x00, 0x05,pre_fwd_pvalue, Vtr_AdcValue.FWD_P.w );						
//						break;
//					case 0x07:
//						power_calibrate_361M_flag=0;	
//						vtr_eeprom_tx.txpar.fwd_power_g_361M.whl.high=Vtr_AdcValue.FWD_P.bb[1];
//						vtr_eeprom_tx.txpar.fwd_power_g_361M.whl.low=Vtr_AdcValue.FWD_P.bb[0];		
//						pre_fwd_pvalue=(uint16)(vtr_eeprom_tx.txpar.fwd_power_d_361M.whl.high*256+vtr_eeprom_tx.txpar.fwd_power_d_361M.whl.low);
//						account_fwdpower_kcvalue(0x00, 0x06,pre_fwd_pvalue, Vtr_AdcValue.FWD_P.w );							
//						break;						
//					default:
//						break;
//				}
//			}
//			else
//			{
//				if(power_calibrate_363M_flag>0)
//				{
//					switch(power_calibrate_363M_flag)
//					{
//						case 0x01:
//							power_calibrate_363M_flag=0;
//							vtr_eeprom_tx.txpar.fwd_power_a_363M.whl.high=Vtr_AdcValue.FWD_P.bb[1];
//							vtr_eeprom_tx.txpar.fwd_power_a_363M.whl.low=Vtr_AdcValue.FWD_P.bb[0];
//							break;
//						case 0x02:
//							power_calibrate_363M_flag=0;		
//							vtr_eeprom_tx.txpar.fwd_power_b_363M.whl.high=Vtr_AdcValue.FWD_P.bb[1];
//							vtr_eeprom_tx.txpar.fwd_power_b_363M.whl.low=Vtr_AdcValue.FWD_P.bb[0];		
//							pre_fwd_pvalue=(uint16)(vtr_eeprom_tx.txpar.fwd_power_a_363M.whl.high*256+vtr_eeprom_tx.txpar.fwd_power_a_363M.whl.low);
//							account_fwdpower_kcvalue(0x01, 0x01,pre_fwd_pvalue, Vtr_AdcValue.FWD_P.w );
//							break;
//						case 0x03:
//							power_calibrate_363M_flag=0;	
//							vtr_eeprom_tx.txpar.fwd_power_c_363M.whl.high=Vtr_AdcValue.FWD_P.bb[1];
//							vtr_eeprom_tx.txpar.fwd_power_c_363M.whl.low=Vtr_AdcValue.FWD_P.bb[0];	
//							pre_fwd_pvalue=(uint16)(vtr_eeprom_tx.txpar.fwd_power_b_363M.whl.high*256+vtr_eeprom_tx.txpar.fwd_power_b_363M.whl.low);
//							account_fwdpower_kcvalue(0x01, 0x02,pre_fwd_pvalue, Vtr_AdcValue.FWD_P.w );							
//							break;
//						case 0x04:
//							power_calibrate_363M_flag=0;	
//							vtr_eeprom_tx.txpar.fwd_power_d_363M.whl.high=Vtr_AdcValue.FWD_P.bb[1];
//							vtr_eeprom_tx.txpar.fwd_power_d_363M.whl.low=Vtr_AdcValue.FWD_P.bb[0];		
//							pre_fwd_pvalue=(uint16)(vtr_eeprom_tx.txpar.fwd_power_c_363M.whl.high*256+vtr_eeprom_tx.txpar.fwd_power_c_363M.whl.low);
//							account_fwdpower_kcvalue(0x01, 0x03,pre_fwd_pvalue, Vtr_AdcValue.FWD_P.w );							
//							break;
//						case 0x05:
//							power_calibrate_363M_flag=0;		
//							vtr_eeprom_tx.txpar.fwd_power_e_363M.whl.high=Vtr_AdcValue.FWD_P.bb[1];
//							vtr_eeprom_tx.txpar.fwd_power_e_363M.whl.low=Vtr_AdcValue.FWD_P.bb[0];		
//							pre_fwd_pvalue=(uint16)(vtr_eeprom_tx.txpar.fwd_power_d_363M.whl.high*256+vtr_eeprom_tx.txpar.fwd_power_d_363M.whl.low);
//							account_fwdpower_kcvalue(0x01, 0x04,pre_fwd_pvalue, Vtr_AdcValue.FWD_P.w );
//							break;
//						case 0x06:
//							power_calibrate_363M_flag=0;	
//							vtr_eeprom_tx.txpar.fwd_power_f_363M.whl.high=Vtr_AdcValue.FWD_P.bb[1];
//							vtr_eeprom_tx.txpar.fwd_power_f_363M.whl.low=Vtr_AdcValue.FWD_P.bb[0];	
//							pre_fwd_pvalue=(uint16)(vtr_eeprom_tx.txpar.fwd_power_e_363M.whl.high*256+vtr_eeprom_tx.txpar.fwd_power_e_363M.whl.low);
//							account_fwdpower_kcvalue(0x01, 0x05,pre_fwd_pvalue, Vtr_AdcValue.FWD_P.w );							
//							break;
//						case 0x07:
//							power_calibrate_363M_flag=0;	
//							vtr_eeprom_tx.txpar.fwd_power_g_363M.whl.high=Vtr_AdcValue.FWD_P.bb[1];
//							vtr_eeprom_tx.txpar.fwd_power_g_363M.whl.low=Vtr_AdcValue.FWD_P.bb[0];		
//							pre_fwd_pvalue=(uint16)(vtr_eeprom_tx.txpar.fwd_power_f_363M.whl.high*256+vtr_eeprom_tx.txpar.fwd_power_f_363M.whl.low);
//							account_fwdpower_kcvalue(0x01, 0x06,pre_fwd_pvalue, Vtr_AdcValue.FWD_P.w );							
//							break;                                   
//						default:
//							break;
//					}
//				}
//				else
//				{
//					if(power_calibrate_365M_flag>0)
//					{
//						switch(power_calibrate_365M_flag)
//						{
//							case 0x01:
//								power_calibrate_365M_flag=0;
//								vtr_eeprom_tx.txpar.fwd_power_a_365M.whl.high=Vtr_AdcValue.FWD_P.bb[1];
//								vtr_eeprom_tx.txpar.fwd_power_a_365M.whl.low=Vtr_AdcValue.FWD_P.bb[0];						
//								break;
//							case 0x02:
//								power_calibrate_365M_flag=0;		
//								vtr_eeprom_tx.txpar.fwd_power_b_365M.whl.high=Vtr_AdcValue.FWD_P.bb[1];
//								vtr_eeprom_tx.txpar.fwd_power_b_365M.whl.low=Vtr_AdcValue.FWD_P.bb[0];	
//								pre_fwd_pvalue=(uint16)(vtr_eeprom_tx.txpar.fwd_power_a_365M.whl.high*256+vtr_eeprom_tx.txpar.fwd_power_a_365M.whl.low);
//								account_fwdpower_kcvalue(0x02, 0x01,pre_fwd_pvalue, Vtr_AdcValue.FWD_P.w );								
//								break;
//							case 0x03:
//								power_calibrate_365M_flag=0;	
//								vtr_eeprom_tx.txpar.fwd_power_c_365M.whl.high=Vtr_AdcValue.FWD_P.bb[1];
//								vtr_eeprom_tx.txpar.fwd_power_c_365M.whl.low=Vtr_AdcValue.FWD_P.bb[0];		
//								pre_fwd_pvalue=(uint16)(vtr_eeprom_tx.txpar.fwd_power_b_365M.whl.high*256+vtr_eeprom_tx.txpar.fwd_power_b_365M.whl.low);
//								account_fwdpower_kcvalue(0x02, 0x02,pre_fwd_pvalue, Vtr_AdcValue.FWD_P.w );								
//								break;
//							case 0x04:
//								power_calibrate_365M_flag=0;	
//								vtr_eeprom_tx.txpar.fwd_power_d_365M.whl.high=Vtr_AdcValue.FWD_P.bb[1];
//								vtr_eeprom_tx.txpar.fwd_power_d_365M.whl.low=Vtr_AdcValue.FWD_P.bb[0];		
//								pre_fwd_pvalue=(uint16)(vtr_eeprom_tx.txpar.fwd_power_c_365M.whl.high*256+vtr_eeprom_tx.txpar.fwd_power_c_365M.whl.low);
//								account_fwdpower_kcvalue(0x02, 0x03,pre_fwd_pvalue, Vtr_AdcValue.FWD_P.w );								
//								break;
//							case 0x05:
//								power_calibrate_365M_flag=0;		
//								vtr_eeprom_tx.txpar.fwd_power_e_365M.whl.high=Vtr_AdcValue.FWD_P.bb[1];
//								vtr_eeprom_tx.txpar.fwd_power_e_365M.whl.low=Vtr_AdcValue.FWD_P.bb[0];	
//								pre_fwd_pvalue=(uint16)(vtr_eeprom_tx.txpar.fwd_power_d_365M.whl.high*256+vtr_eeprom_tx.txpar.fwd_power_d_365M.whl.low);
//								account_fwdpower_kcvalue(0x02, 0x04,pre_fwd_pvalue, Vtr_AdcValue.FWD_P.w );								
//								break;
//							case 0x06:
//								power_calibrate_365M_flag=0;	
//								vtr_eeprom_tx.txpar.fwd_power_f_365M.whl.high=Vtr_AdcValue.FWD_P.bb[1];
//								vtr_eeprom_tx.txpar.fwd_power_f_365M.whl.low=Vtr_AdcValue.FWD_P.bb[0];		
//								pre_fwd_pvalue=(uint16)(vtr_eeprom_tx.txpar.fwd_power_e_365M.whl.high*256+vtr_eeprom_tx.txpar.fwd_power_e_365M.whl.low);
//								account_fwdpower_kcvalue(0x02, 0x05,pre_fwd_pvalue, Vtr_AdcValue.FWD_P.w );								
//								break;
//							case 0x07:
//								power_calibrate_365M_flag=0;	
//								vtr_eeprom_tx.txpar.fwd_power_g_365M.whl.high=Vtr_AdcValue.FWD_P.bb[1];
//								vtr_eeprom_tx.txpar.fwd_power_g_365M.whl.low=Vtr_AdcValue.FWD_P.bb[0];		
//								pre_fwd_pvalue=(uint16)(vtr_eeprom_tx.txpar.fwd_power_f_365M.whl.high*256+vtr_eeprom_tx.txpar.fwd_power_f_365M.whl.low);
//								account_fwdpower_kcvalue(0x02, 0x06,pre_fwd_pvalue, Vtr_AdcValue.FWD_P.w );								
//								break;                                    
//							default:
//								break;
//						}
//					}
//					else
//					{
//						if(power_calibrate_367M_flag>0)
//						{

//							switch(power_calibrate_367M_flag)
//							{
//								case 0x01:
//									power_calibrate_367M_flag=0;
//									vtr_eeprom_tx.txpar.fwd_power_a_367M.whl.high=Vtr_AdcValue.FWD_P.bb[1];
//									vtr_eeprom_tx.txpar.fwd_power_a_367M.whl.low=Vtr_AdcValue.FWD_P.bb[0];								
//									break;
//								case 0x02:
//									power_calibrate_367M_flag=0;		
//									vtr_eeprom_tx.txpar.fwd_power_b_367M.whl.high=Vtr_AdcValue.FWD_P.bb[1];
//									vtr_eeprom_tx.txpar.fwd_power_b_367M.whl.low=Vtr_AdcValue.FWD_P.bb[0];		
//									pre_fwd_pvalue=(uint16)(vtr_eeprom_tx.txpar.fwd_power_a_367M.whl.high*256+vtr_eeprom_tx.txpar.fwd_power_a_367M.whl.low);
//									account_fwdpower_kcvalue(0x03, 0x01,pre_fwd_pvalue, Vtr_AdcValue.FWD_P.w );									
//									break;
//								case 0x03:
//									power_calibrate_367M_flag=0;	
//									vtr_eeprom_tx.txpar.fwd_power_c_367M.whl.high=Vtr_AdcValue.FWD_P.bb[1];
//									vtr_eeprom_tx.txpar.fwd_power_c_367M.whl.low=Vtr_AdcValue.FWD_P.bb[0];		
//									pre_fwd_pvalue=(uint16)(vtr_eeprom_tx.txpar.fwd_power_b_367M.whl.high*256+vtr_eeprom_tx.txpar.fwd_power_b_367M.whl.low);
//									account_fwdpower_kcvalue(0x03, 0x02,pre_fwd_pvalue, Vtr_AdcValue.FWD_P.w );									
//									break;
//								case 0x04:
//									power_calibrate_367M_flag=0;	
//									vtr_eeprom_tx.txpar.fwd_power_d_367M.whl.high=Vtr_AdcValue.FWD_P.bb[1];
//									vtr_eeprom_tx.txpar.fwd_power_d_367M.whl.low=Vtr_AdcValue.FWD_P.bb[0];		
//									pre_fwd_pvalue=(uint16)(vtr_eeprom_tx.txpar.fwd_power_c_367M.whl.high*256+vtr_eeprom_tx.txpar.fwd_power_c_367M.whl.low);
//									account_fwdpower_kcvalue(0x03, 0x03,pre_fwd_pvalue, Vtr_AdcValue.FWD_P.w );									
//									break;
//								case 0x05:
//									power_calibrate_367M_flag=0;		
//									vtr_eeprom_tx.txpar.fwd_power_e_367M.whl.high=Vtr_AdcValue.FWD_P.bb[1];
//									vtr_eeprom_tx.txpar.fwd_power_e_367M.whl.low=Vtr_AdcValue.FWD_P.bb[0];		
//									pre_fwd_pvalue=(uint16)(vtr_eeprom_tx.txpar.fwd_power_d_367M.whl.high*256+vtr_eeprom_tx.txpar.fwd_power_d_367M.whl.low);
//									account_fwdpower_kcvalue(0x03, 0x04,pre_fwd_pvalue, Vtr_AdcValue.FWD_P.w );									
//									break;
//								case 0x06:
//									power_calibrate_367M_flag=0;	
//									vtr_eeprom_tx.txpar.fwd_power_f_367M.whl.high=Vtr_AdcValue.FWD_P.bb[1];
//									vtr_eeprom_tx.txpar.fwd_power_f_367M.whl.low=Vtr_AdcValue.FWD_P.bb[0];		
//									pre_fwd_pvalue=(uint16)(vtr_eeprom_tx.txpar.fwd_power_e_367M.whl.high*256+vtr_eeprom_tx.txpar.fwd_power_e_367M.whl.low);
//									account_fwdpower_kcvalue(0x03, 0x05,pre_fwd_pvalue, Vtr_AdcValue.FWD_P.w );									
//									break;
//								case 0x07:
//									power_calibrate_367M_flag=0;	
//									vtr_eeprom_tx.txpar.fwd_power_g_367M.whl.high=Vtr_AdcValue.FWD_P.bb[1];
//									vtr_eeprom_tx.txpar.fwd_power_g_367M.whl.low=Vtr_AdcValue.FWD_P.bb[0];		
//									pre_fwd_pvalue=(uint16)(vtr_eeprom_tx.txpar.fwd_power_f_367M.whl.high*256+vtr_eeprom_tx.txpar.fwd_power_f_367M.whl.low);
//									account_fwdpower_kcvalue(0x03, 0x06,pre_fwd_pvalue, Vtr_AdcValue.FWD_P.w );									
//									break;                                     
//								default:
//									break;
//							}							
//						}
//						else
//						{
//							
//						}
//					}
//				}
//			}
}



/****************************************************************************
* 名称：Read_CH_info()
* 功能：VTR响应查询信道信息 
*		vrs_rdchc_type 读取类型 0x00:读取一个通道的信道信息0x01:读取所有通道的信道信息
* 入口参数： 无
* 出口参数：无
* 说明：
****************************************************************************/
uint8 vtr_Read_CH_info(uint8 ch ,uint8 *recv_data,uint8 *databuf,uint16 len)
{
	uint8 vrs_rdchc_type,vrs_rdchc_num;
	//VTR_PARAM_F3 *paramf3 =(VTR_PARAM_F3 *)recv_data;
	
	VTR_PARAM_UNION_F4 * rtu_ch_info = Get_RTU_dev_ptr_current_chinfo();
	
	memcpy(recv_data,rtu_ch_info,sizeof(VTR_PARAM_UNION_F4)); //
	return suc_respond;
}

/****************************************************************************
* 名称：Vtr_Set_Channel_InfoPar---0x23
* 功能：//设置信道机的通道参数
* 入口参数：(uint8 ch,			通道号:0-PC,1-chc
			uint8* recv_data,	参数数据
			uint8 * databuf,	回应帧数据
			uint16 * len)		回应帧数据长度
* 出口参数：suc_respond 有效,fail_respond 无效。
****************************************************************************/
uint8 Vtr_Set_Channel_InfoPar(uint8 ch,uint8* recv_data,uint8 * databuf,uint16 * len)
{
	VTR_PARAM_UNION_F4 * rtu_ch_info = Get_RTU_dev_ptr_current_chinfo();
		
	memcpy(rtu_ch_info,recv_data,sizeof(VTR_PARAM_UNION_F4)); //中间变量将来要删掉，此处先保留，否则显示异常
	return suc_respond;
}	

//Param0(DST): Destination flash address where data bytes are to be written. This address should be a 256 byte boundary.
//Param1(SRC): Source RAM address from which data bytes are to be read. This address should be a word boundary.
//Param2: Number of bytes to be written. Should be 256 | 512 | 1024 | 4096
/*****************************************************************************
 * 函 数 名  : Rtu_Save_Data_To_Flash
 * 负 责 人  : MZM
 * 创建日期  : 2016年6月28日
 * 函数功能  : 将数据保存至扇区sector的addr地址处，数据长度为size，数据内容
               为data，
 * 输入参数  : uint8_t sector  扇区号 14-15
               uint32_t addr   保存位置，地址
               uint8_t * data  保存数据缓冲区
               uint32_t size   保存数据长度  256的倍数
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 该接口 采用页面擦除编程，一个page为256byte，时间较慢，磨损较小

*****************************************************************************/
uint8_t Rtu_Save_Data_To_Flash_Page(uint32_t start_page,uint32_t end_page,uint8_t * data,uint32_t size)
{
	int i;
	uint32_t ret_code,numpage=0,sizeleft=0;
	uint32_t addr;


	numpage = end_page - start_page+1;
	if(size >(numpage*SYS_PARAM_PAGE_SIZE))
	{
		return FALSE;
	}
	
	/* Disable interrupt mode so it doesn't fire during FLASH updates */
	__disable_irq();	
	/* IAP Flash programming */ /* Prepare to write/erase the last sector */	
	ret_code = Chip_IAP_PreSectorForReadWrite(IAP_LAST_SECTOR, IAP_LAST_SECTOR);
	/* Error checking */	
	if (ret_code != IAP_CMD_SUCCESS)
	{	
		DEBUGOUT("Command failed to execute, return code is: %x\r\n", ret_code);	
	}	

	/* Erase the last sector */ 
	ret_code = Chip_IAP_ErasePage(start_page, end_page);
	/* Error checking */
	if (ret_code != IAP_CMD_SUCCESS) 
	{		
		DEBUGOUT("Command failed to execute, return code is: %x\r\n", ret_code);
	}	
	/* Prepare to write/erase the last sector */
	ret_code = Chip_IAP_PreSectorForReadWrite(IAP_LAST_SECTOR, IAP_LAST_SECTOR);
	/* Error checking */	
	if (ret_code != IAP_CMD_SUCCESS) 
	{		
		DEBUGOUT("Command failed to execute, return code is: %x\r\n", ret_code);	
	}	

	/* Write to the last sector */	

	addr = start_page*SYS_PARAM_PAGE_SIZE;
	//Param0(DST): Destination flash address where data bytes are to be written. This address should be a 256 byte boundary.
	//Param1(SRC): Source RAM address from which data bytes are to be read. This address should be a word boundary.
	//Param2: Number of bytes to be written. Should be 256 | 512 | 1024 | 4096
	for(i=0;i<numpage;i++)
	{
		ret_code = Chip_IAP_CopyRamToFlash(addr, (uint32_t *)data+SYS_PARAM_PAGE_SIZE*i, SYS_PARAM_PAGE_SIZE); 
		/* Error checking */	
		if (ret_code != IAP_CMD_SUCCESS) 
		{		
			DEBUGOUT("Command failed to execute, return code is: %x\r\n", ret_code);	
		}
		addr += SYS_PARAM_PAGE_SIZE*i;
	}

	/* Re-enable interrupt mode */
	__enable_irq();
	
}

//Param0(DST): Destination flash address where data bytes are to be written. This address should be a 256 byte boundary.
//Param1(SRC): Source RAM address from which data bytes are to be read. This address should be a word boundary.
//Param2: Number of bytes to be written. Should be 256 | 512 | 1024 | 4096
/*****************************************************************************
 * 函 数 名  : Rtu_Save_Data_To_Flash
 * 负 责 人  : MZM
 * 创建日期  : 2016年6月28日
 * 函数功能  : 将数据保存至扇区sector的addr地址处，数据长度为size，数据内容
               为data，
 * 输入参数  : uint8_t sector  扇区号 14-15
               uint32_t addr   保存位置，地址
               uint8_t * data  保存数据缓冲区
               uint32_t size   保存数据长度
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 该接口 采用页面擦除编程，一个page为256byte，时间较慢，磨损较小

*****************************************************************************/
uint8_t Rtu_Save_Data_To_Flash(uint8_t sector,uint32_t addr,uint8_t * data,uint32_t size)
{
#define PAGE_SIZE_OF_FLASH		256
	int i;
	uint32_t ret_code,numpage=0,sizeleft=0,startpage,endpage,num4k;
	uint32_t part_id;


	numpage = size/PAGE_SIZE_OF_FLASH;
	sizeleft = size%PAGE_SIZE_OF_FLASH;
	startpage = addr / PAGE_SIZE_OF_FLASH;
	endpage = startpage + numpage +((sizeleft==0)?0:1);
	

//	/* Read Part Identification Number*/	
//	part_id = Chip_IAP_ReadPID();	
//	DEBUGOUT("Part ID is: %x\r\n", part_id);
	
	/* Disable interrupt mode so it doesn't fire during FLASH updates */
	__disable_irq();	
	/* IAP Flash programming */ /* Prepare to write/erase the last sector */	
	ret_code = Chip_IAP_PreSectorForReadWrite(sector, sector);
	/* Error checking */	
	if (ret_code != IAP_CMD_SUCCESS)
	{	
		DEBUGOUT("Command failed to execute, return code is: %x\r\n", ret_code);	
	}	

	/* Erase the last sector */ 
	ret_code = Chip_IAP_ErasePage(startpage, endpage);
	/* Error checking */
	if (ret_code != IAP_CMD_SUCCESS) 
	{		
		DEBUGOUT("Command failed to execute, return code is: %x\r\n", ret_code);
	}	
	/* Prepare to write/erase the last sector */
	ret_code = Chip_IAP_PreSectorForReadWrite(sector, sector);
	/* Error checking */	
	if (ret_code != IAP_CMD_SUCCESS) 
	{		
		DEBUGOUT("Command failed to execute, return code is: %x\r\n", ret_code);	
	}	

	/* Write to the last sector */	
	num4k = size/4096;
	sizeleft = (size%4096)/4;

	if(sizeleft > 256)
	{
		sizeleft = ((sizeleft >512)?1024:512);
	}
	else
	{
		sizeleft = 256;
	}
	//Param0(DST): Destination flash address where data bytes are to be written. This address should be a 256 byte boundary.
	//Param1(SRC): Source RAM address from which data bytes are to be read. This address should be a word boundary.
	//Param2: Number of bytes to be written. Should be 256 | 512 | 1024 | 4096
	for(i=0;i<numpage;i++)
	{
		ret_code = Chip_IAP_CopyRamToFlash(addr, (uint32_t *)data+4096*i, 1024); 
		/* Error checking */	
		if (ret_code != IAP_CMD_SUCCESS) 
		{		
			DEBUGOUT("Command failed to execute, return code is: %x\r\n", ret_code);	
		}
		addr += 4096*i;
	}
	if(sizeleft)
	{
			ret_code = Chip_IAP_CopyRamToFlash(addr+4096*i, (uint32_t *)data+4096*i, sizeleft); 
			/* Error checking */	
			if (ret_code != IAP_CMD_SUCCESS) 
			{		
				DEBUGOUT("Command failed to execute, return code is: %x\r\n", ret_code);	
			}
	}
	/* Re-enable interrupt mode */
	__enable_irq();
	
}


//Param0(DST): Destination flash address where data bytes are to be written. This address should be a 256 byte boundary.
//Param1(SRC): Source RAM address from which data bytes are to be read. This address should be a word boundary.
//Param2: Number of bytes to be written. Should be 256 | 512 | 1024 | 4096
uint8_t Rtu_Save_Data_To_Flash1(uint8_t sector,uint32_t addr,uint8_t * data,uint32_t size)
{
	int i=0;

	uint8_t ret_code,num4k=0,sizeleft=0;
	uint32_t part_id;


	num4k = size/4096;
	sizeleft = (size%4096)/4;

	if(sizeleft > 256)
	{
		sizeleft = ((sizeleft >512)?1024:512);
	}
	else
	{
		sizeleft = 256;
	}
	/* Read Part Identification Number*/	
	part_id = Chip_IAP_ReadPID();	
	DEBUGOUT("Part ID is: %x\r\n", part_id);
	
	/* Disable interrupt mode so it doesn't fire during FLASH updates */
	__disable_irq();	
	/* IAP Flash programming */ /* Prepare to write/erase the last sector */	
	ret_code = Chip_IAP_PreSectorForReadWrite(sector, sector);
	/* Error checking */	
	if (ret_code != IAP_CMD_SUCCESS)
	{	
		DEBUGOUT("Command failed to execute, return code is: %x\r\n", ret_code);	
	}	

	/* Erase the last sector */ 
	ret_code = Chip_IAP_EraseSector(sector, sector);
	/* Error checking */
	if (ret_code != IAP_CMD_SUCCESS) 
	{		
		DEBUGOUT("Command failed to execute, return code is: %x\r\n", ret_code);
	}	
	/* Prepare to write/erase the last sector */
	ret_code = Chip_IAP_PreSectorForReadWrite(sector, sector);
	/* Error checking */	
	if (ret_code != IAP_CMD_SUCCESS) 
	{		
		DEBUGOUT("Command failed to execute, return code is: %x\r\n", ret_code);	
	}	

	/* Write to the last sector */	
	
	//Param0(DST): Destination flash address where data bytes are to be written. This address should be a 256 byte boundary.
	//Param1(SRC): Source RAM address from which data bytes are to be read. This address should be a word boundary.
	//Param2: Number of bytes to be written. Should be 256 | 512 | 1024 | 4096
	for(i=0;i<num4k;i++)
	{
		ret_code = Chip_IAP_CopyRamToFlash(addr, (uint32_t *)data+4096*i, 1024); 
		/* Error checking */	
		if (ret_code != IAP_CMD_SUCCESS) 
		{		
			DEBUGOUT("Command failed to execute, return code is: %x\r\n", ret_code);	
		}
		addr += 4096*i;
	}
	if(sizeleft)
		{
				ret_code = Chip_IAP_CopyRamToFlash(addr+4096*i, (uint32_t *)data+4096*i, sizeleft); 
				/* Error checking */	
				if (ret_code != IAP_CMD_SUCCESS) 
				{		
					DEBUGOUT("Command failed to execute, return code is: %x\r\n", ret_code);	
				}
		}
	/* Re-enable interrupt mode */
	__enable_irq();
	
}

/*****************************************************************************
 * 函 数 名  : Rtu_Read_Data_From_Flash
 * 负 责 人  : MZM
 * 创建日期  : 2016年6月28日
 * 函数功能  : 从cpuflash中读取size个数据到data缓冲区
 * 输入参数  : uint32_t addr   cpuflash的地址-读地址
               uint8_t * data  数据返回缓冲区
               uint32_t size   读取长度
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void Rtu_Read_Data_From_Flash(uint32_t addr,uint8_t * data,uint32_t size)
{
	memcpy(data,(void *)addr,size);
}

/*****************************************************************************
 * 函 数 名  : Rtu_Read_flag_from_flash
 * 负 责 人  : MZM
 * 创建日期  : 2016年6月28日
 * 函数功能  : 从cpuflash中读取 参数标志
 * 输入参数  : uint8_t sector  扇区号
               VTR_FLAGS *buf  标志返回指针
               uint32_t size   读取长度
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void Rtu_Read_flag_from_flash(uint8_t sector,VTR_FLAGS *buf,uint32_t size)
{
	Rtu_Read_Data_From_Flash(sector*32*1024,(uint8_t *)buf,size);
}
// 保存参数到flash，14和15扇区，写平衡。
/*****************************************************************************
 * 函 数 名  : RTU_Save_Param_to_Flash
 * 负 责 人  : MZM
 * 创建日期  : 2016年6月28日
 * 函数功能  : 参数保存
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void RTU_Save_Param_to_Flash()
{
	uint32_t savesector;

	// 14�/15 扇区 写平衡，
//	savesector = ((rtudev->saveparam.sys_param_flags.param_flag.save_flag>14)?14:15);
	rtudev->saveparam.sys_param_flags.param_flag.save_flag = 15;	
	rtudev->saveparam.sys_param_flags.param_flag.valid_flag += 1;
	
#if 0
	Rtu_Save_Data_To_Flash(savesector,savesector*32*1024,(uint8_t *)&rtudev->saveparam,sizeof(RTU_PARAM_SAVE_FLASH));
#else
//	Rtu_Save_Data_To_Flash(savesector,savesector*32*1024,(uint8_t *)&rtudev->saveparam,PARAM_OFFSET(RTU_PARAM_SAVE_FLASH,sys_param_freq));
	RTU_Save_All_Param();

#endif	

}

void Rtu_Set_restart_dma_flg(uint16_t  value)
{
	 rtudev->saveparam.sys_param_dac_mod.restart_dma_flg = value;
}
uint16_t Rtu_Get_restart_dma_flg()
{
	return rtudev->saveparam.sys_param_dac_mod.restart_dma_flg;
}


/*****************************************************************************
 * 函 数 名  : Rtu_Set_tx_mod_delay
 * 负 责 人  : MZM
 * 创建日期  : 2016年6月28日
 * 函数功能  : 设置spiDACB和TX MOD的延时，延时时间=value*10us
 * 输入参数  : uint16_t  value  保存延时的参数
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void Rtu_Set_tx_mod_delay(uint16_t  value)
{
	 rtudev->saveparam.sys_param_dac_mod.tx_mod_delay = value;
}
/*****************************************************************************
 * 函 数 名  : Rtu_Get_tx_mod_delay
 * 负 责 人  : MZM
 * 创建日期  : 2016年6月28日
 * 函数功能  : 获取 txmod发射dma数据的延时
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
uint16_t Rtu_Get_tx_mod_delay()
{
	return rtudev->saveparam.sys_param_dac_mod.tx_mod_delay;
}
/*****************************************************************************
 * 函 数 名  : Rtu_Set_tx_mod_K
 * 负 责 人  : MZM
 * 创建日期  : 2016年6月28日
 * 函数功能  : 设置txmod的值的系数,系数是10倍放大，如：系数0.5参数应该为5
 * 输入参数  : uint16_t  value10  设置txMOD计算数值系数，value是系数的10倍
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void Rtu_Set_tx_mod_K(uint16_t  value10)
{
	 rtudev->saveparam.sys_param_dac_mod.tx_mod_k = value10;
}
/*****************************************************************************
 * 函 数 名  : Rtu_Get_tx_mod_K
 * 负 责 人  : MZM
 * 创建日期  : 2016年6月28日
 * 函数功能  : 获取TXmod的计算系数
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
uint16_t Rtu_Get_tx_mod_K()
{
	return rtudev->saveparam.sys_param_dac_mod.tx_mod_k;
}

/*****************************************************************************
 * 函 数 名  : Rtu_Set_spi_dac_K
 * 负 责 人  : MZM
 * 创建日期  : 2016年6月28日
 * 函数功能  : 设置计算系数，为实际系数的10倍
 * 输入参数  : uint16_t  value10  设置spiDACB的计算系数，value10是实际系数的10倍
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void Rtu_Set_spi_dac_K(uint16_t  value10)
{
	 rtudev->saveparam.sys_param_dac_mod.spi_dac_k= value10;
}

/*****************************************************************************
 * 函 数 名  : Rtu_Get_spi_dac_K
 * 负 责 人  : MZM
 * 创建日期  : 2016年6月28日
 * 函数功能  : 获取 spidac的计算系数
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
uint16_t Rtu_Get_spi_dac_K()
{
	return rtudev->saveparam.sys_param_dac_mod.spi_dac_k;
}

/*****************************************************************************
 * 函 数 名  : Rtu_Set_tx_freq
 * 负 责 人  : MZM
 * 创建日期  : 2016年6月28日
 * 函数功能  : 设置发射频率
 * 输入参数  : uint32_t  value  设置发射频率
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void Rtu_Set_tx_freq(uint32_t  value)
{
	 rtudev->saveparam.sys_param_freq.tx_freq= value;
}

/*****************************************************************************
 * 函 数 名  : Rtu_Get_tx_freq
 * 负 责 人  : MZM
 * 创建日期  : 2016年6月28日
 * 函数功能  : 读取发射频率
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
uint32_t Rtu_Get_tx_freq()
{
	return rtudev->saveparam.sys_param_freq.tx_freq;
}
/*****************************************************************************
 * 函 数 名  : Rtu_Set_rx_freq
 * 负 责 人  : MZM
 * 创建日期  : 2016年6月28日
 * 函数功能  : 设置而接收频率
 * 输入参数  : uint32_t  value  设置接收频率
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void Rtu_Set_rx_freq(uint32_t  value)
{
	 rtudev->saveparam.sys_param_freq.rx_freq= value;
}
/*****************************************************************************
 * 函 数 名  : Rtu_Get_rx_freq
 * 负 责 人  : MZM
 * 创建日期  : 2016年6月28日
 * 函数功能  : 读取接收频率
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
uint32_t Rtu_Get_rx_freq()
{
	return rtudev->saveparam.sys_param_freq.rx_freq;
}


/*****************************************************************************
 * 函 数 名  : Get_RTU_dev_devptr
 * 负 责 人  : MZM
 * 创建日期  : 2016年6月28日
 * 函数功能  : 获取RTU设备结构体指针
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : RTU_PARAM_FLASH_T
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
RTU_PARAM_FLASH_T * Get_RTU_dev_devptr()
{
	return rtudev;
}

/*****************************************************************************
 * 函 数 名  : Get_RTU_dev_ptr_KC_struct
 * 负 责 人  : MZM
 * 创建日期  : 2016年6月28日
 * 函数功能  : 获取保存数据的--校准数据K和C 结构体指针
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : VTR_CALIBRATE_KC_STRCUT
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
VTR_CALIBRATE_KC_STRCUT * Get_RTU_dev_ptr_KC_struct()
{
	if(rtudev != NULL)
	{
		return (VTR_CALIBRATE_KC_STRCUT * )&rtudev->saveparam.calibrate_KC_strcut;
	}
	return NULL;
}

/*****************************************************************************
 * 函 数 名  : Get_RTU_dev_ptr_sys_param
 * 负 责 人  : MZM
 * 创建日期  : 2016年6月28日
 * 函数功能  : 获取保存数据的--校准系统参数
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : VTR_CALIBRATE_SYSTEM_PARAM
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
VTR_CALIBRATE_SYSTEM_PARAM * Get_RTU_dev_ptr_sys_param()
{
	if(rtudev != NULL)
	{
		return (VTR_CALIBRATE_SYSTEM_PARAM *)&rtudev->saveparam.calibrate_sys_param;
	}
	
	return NULL;
}

RTU_PARAM_NOT_SAVE_ADVALUE * Get_RTU_dev_ptr_Adc_value()
{
	if(rtudev != NULL)
	{
		return (RTU_PARAM_NOT_SAVE_ADVALUE *)&rtudev->notsaveparam.rtu_AdcValue;
	}
	
	return NULL;
}

VTR_PARAM_UNION_F4 * Get_RTU_dev_ptr_current_chinfo()
{
	if(rtudev != NULL)
	{
		return (VTR_PARAM_UNION_F4 *)&rtudev->notsaveparam.vtr_ch_info_current;
	}
	
	return NULL;
}

RTU_PCA9535_PORT_STRUCT * Get_RTU_dev_ptr_PCA9535_Inputstate()
{
	if(rtudev != NULL)
	{
		return (RTU_PCA9535_PORT_STRUCT *)(&rtudev->notsaveparam.rtu_pca9535_port[0]);
	}
	
	return NULL;
}
RTU_PCA9535_PORT_STRUCT * Get_RTU_dev_ptr_PCA9535_Outputstate()
{
	if(rtudev != NULL)
	{
		return (RTU_PCA9535_PORT_STRUCT *)(&rtudev->notsaveparam.rtu_pca9535_port[1]);
	}
	
	return NULL;
}


/*****************************************************************************
 * 函 数 名  : Get_RTU_dev_ptr_Ext_clk
 * 负 责 人  : MZM
 * 创建日期  : 2016年6月15日
 * 函数功能  : a
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : void
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void * Get_RTU_dev_ptr_Ext_clk()
{
	if(rtudev != NULL)
	{
		return (void *)(&rtudev->saveparam.sys_param_ext_clk);
	}
	
	return NULL;

}
/****************************************************************************
* 名称：configuration_channel_info_step1()--
* 功能 通过发送和接受频率 分别设置发射机和接收机的sky72310的寄存器来产生相应频率
		等待PLL锁定。
* 入口参数： uint32 txfre_data:发射频率
			,uint32 rxfre_data:接受频率
* 出口参数：无
* 说明：
****************************************************************************/
void configuration_channel_info_step1(uint32 txfre_data,uint32 rxfre_data)
{
	Set_PLL_Freq(TX_PLL_SELECT,txfre_data,REF_PLL_FREQ,	0,0,3,0,0x0f,0,0);//tx
//	Set_PLL_Freq(TX_PLL_SELECT,txfre_data,19200000,	0,0,3,0,0x0f,0,0);//tx
	//	configuration_after_check_txld();					

//	Set_PLL_Freq(RX_PLL_SELECT,rxfre_data,19200000,	0,0,3,0,0x07,0,0);//rx
	Set_PLL_Freq(RX_PLL_SELECT,rxfre_data,REF_PLL_FREQ,	0,0,3,0,0x07,0,0);//rx		
				//等待us时间后查LD
	//	configuration_after_check_rxld();
}


/****************************************************************************
* 名称：Vtr_change_change_power_to_powerpot_saveoffset
* 功能：根据提供的发射功率，计算出发射功率和发射半功率
//并将转换的值，写入当前信道参数中，前向功率、反向功率
* 入口参数：saveoffset 校准结构体中，增益参数的偏移位置
* 出口参数：五
****************************************************************************/
uint16 Vtr_change_change_power_to_powerpot_saveoffset(uint8 saveoffset,uint8 txpowerpar)
{
	fp64	temp_fwd_power;
	uint16 temp_fwd_power_data;
	UinthlType temp_fwd_power_hl;
	uint16 fwd_power_hl_b_361M;
	uint16 fwd_power_hl_c_361M;
	uint16 fwd_power_hl_d_361M;
	uint16 fwd_power_hl_e_361M;
	uint16 fwd_power_hl_f_361M;
	uint16 fwd_power_hl_g_361M;    

	fp64	temp_refl_power;
	uint16 temp_refl_power_data;
	UinthlType temp_refl_power_hl;
	UintlhType *tmp_36xM;
	fp64 k1,c1;
	fp64 temp_power_mv;
	uint16 volatile txpowerpotpar;
	VTR_CALIBRATE_KC_STRCUT *calibrate_KC_strcut = Get_RTU_dev_ptr_KC_struct();
	VTR_CALIBRATE_SYSTEM_PARAM *calibrate_sys_param = Get_RTU_dev_ptr_sys_param();

	txpowerpotpar=0;

	if(saveoffset ==0xFF)
	{
		return  0;
	}
	
	tmp_36xM=&calibrate_sys_param->PowerValue[saveoffset].Power_Value.Value_a;//.txpar.fwd_power_b_361M;
	k1=calibrate_KC_strcut->Power_KC_Value[saveoffset].KC_k1;//&vtr_power_kc.tx_fwdpower_361M_k1;
	c1=calibrate_KC_strcut->Power_KC_Value[saveoffset].KC_c1;

	//txpowerpar 1字节，范围:0-50W ，信道参数的发射功率值
	//根据vtr_power_kc.tx_power_361M_k1和c1，计算出发射功率的DAC寄存器的值

		if(txpowerpar==2)
		{
			txpowerpotpar=(*tmp_36xM).w;
		}
		else if(txpowerpar==5)
		{
			txpowerpotpar=(*(tmp_36xM+1)).w;
		}	
		else if(txpowerpar==10)
		{
			txpowerpotpar=(*(tmp_36xM+2)).w;
		}		
		else if(txpowerpar==20)
		{
			txpowerpotpar=(*(tmp_36xM+3)).w;
		}		
		else if(txpowerpar==30)
		{
			txpowerpotpar=(*(tmp_36xM+4)).w;
		}
		else if(txpowerpar==40)
		{
			txpowerpotpar=(*(tmp_36xM+5)).w;
		}
		else if(txpowerpar==50)
		{
			txpowerpotpar=(*(tmp_36xM+6)).w;
		}	 
		else{
		
			temp_power_mv=(k1*txpowerpar)+c1;
			txpowerpotpar=(uint16)(temp_power_mv*1024/2500);	
		}
		return(txpowerpotpar);

}

/****************************************************************************
* 名称：configuration_channel_info_step3()--
* 功能 根据发射频率计算功率值、半功率值
* 入口参数： 无
* 出口参数： 无
* 说明：
****************************************************************************/
void configuration_channel_info_step3(uint32 txfre_data)
{

	uint8 saveoffset;
	VTR_PARAM_UNION_F4 * vtr_ch_info_current = Get_RTU_dev_ptr_current_chinfo();
	VTR_CALIBRATE_SYSTEM_PARAM *calibrate_sys_param = Get_RTU_dev_ptr_sys_param();


	saveoffset = Vtr_Check_Freq_get_saveoffset(txfre_data);//获取该频率区间的点的存储位置
	
	if(saveoffset==0xff)
	{
		return;
	}

	Vtr_change_change_power_to_powerpot_saveoffset(saveoffset,txfre_data);
	vtr_ch_info_current->vtrchannelinfo.txpowerpot.propertyint.w=Vtr_change_change_power_to_powerpot_saveoffset(saveoffset,vtr_ch_info_current->vtrchannelinfo.txpower.property); 	
	vtr_ch_info_current->vtrchannelinfo.txpowerpothalf.propertyint.w=Vtr_change_change_power_to_powerpot_saveoffset(saveoffset,vtr_ch_info_current->vtrchannelinfo.txpower.property/2);

	
//	get_upcos_value_txpwr(vtr_ch_info_current->vtrchannelinfo.txpowerpot.propertyint.w);
//	get_upcos_value_txpwr_half(vtr_ch_info_current->vtrchannelinfo.txpowerpothalf.propertyint.w);
				
	vtr_ch_info_current->vtrchannelinfo.refoscpot.property=calibrate_sys_param->PowerValue[saveoffset].Refdoc_Value.whl.low;
	vtr_ch_info_current->vtrchannelinfo.rmodpot.property=calibrate_sys_param->PowerValue[saveoffset].Refpot_Value.whl.low;
	vtr_ch_info_current->vtrchannelinfo.vmodpot.property=calibrate_sys_param->PowerValue[saveoffset].Vcopot_Value.whl.low;
				


}

//
/****************************************************************************
* 名称：configuration_channel_info_new()
* 功能 配置当前通道信息:lock、每5S执行一次
* 入口参数： uint8 channel_number  通道号
* 出口参数：无
* 说明：
****************************************************************************/
void configuration_channel_info_new(uint8 channel_number)
{
	//uint16 i,j;
	uint32 txfre_data,rxfre_data;
	//VtrChInfoparUnion vtr_ch_infopar_temp;
	uint16 txhtonevalue_temp,rxhtonevalue_temp;
	
	VTR_PARAM_UNION_F4 * vtr_ch_info_current = Get_RTU_dev_ptr_current_chinfo();
	
	{
		if(vtr_ch_info_current->vtrchannelinfo.edit.property==0x01)
		{
			//vtr_ch_info_current.vtrchannelinfo.alarmstate.propertyint.w=0;	
		
			txfre_data=(vtr_ch_info_current->vtrchannelinfo.txfre.propertyint.whl.high*256+vtr_ch_info_current->vtrchannelinfo.txfre.propertyint.whl.low)*1000000+\
			(vtr_ch_info_current->vtrchannelinfo.txfre.propertyfrac.whl.high*256+vtr_ch_info_current->vtrchannelinfo.txfre.propertyfrac.whl.low)*125;
			rxfre_data=(vtr_ch_info_current->vtrchannelinfo.rxfre.propertyint.whl.high*256+vtr_ch_info_current->vtrchannelinfo.rxfre.propertyint.whl.low)*1000000+\
			(vtr_ch_info_current->vtrchannelinfo.rxfre.propertyfrac.whl.high*256+vtr_ch_info_current->vtrchannelinfo.rxfre.propertyfrac.whl.low)*125;

			/*******************************************************************************************************/
			configuration_channel_info_step1(txfre_data,rxfre_data);	
//			configuration_channel_info_step2();
			configuration_channel_info_step3(txfre_data);
//			configuration_channel_info_step4();
//			configuration_channel_info_step5();						
		}
	
	}
}



void test_cpu_flash()
{
#define TEST_CPU_FLASH_SIZE		1024
	uint32_t i,addr,flg=0;
	uint8_t writebuf[TEST_CPU_FLASH_SIZE],*readptr;
//	uint8_t readbuf[TEST_CPU_FLASH_SIZE];

	addr = IAP_LAST_SECTOR*32*1024;

	readptr = (uint8_t *)addr;

	memset(writebuf,0xA5,TEST_CPU_FLASH_SIZE);
	Rtu_Save_Data_To_Flash(IAP_LAST_SECTOR,addr,(uint8_t *)writebuf,TEST_CPU_FLASH_SIZE);

	for(i=0;i<TEST_CPU_FLASH_SIZE;i++)
		{
			if(writebuf[i] != readptr[i])
			{
				flg=1;
				break;
			}
		}
	if(flg==0)
	{
		rt_kprintf(1,"SUCESS---TEST_SIZE:[%d]r\n",TEST_CPU_FLASH_SIZE);
	}
	else{
		rt_kprintf(1,"ERROR---addr[%d]--write[%d],read[%d],TEST_SIZE:[%d]\r\n",i,writebuf[i],readptr[i],TEST_CPU_FLASH_SIZE);
		}

}

