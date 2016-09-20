#include "kb_dev.h"

#define DISPLAY_GLOBALS

#include "display.h"
#include "pinint.h"

DISPLAY_OPERATE_T * gui_display_dev;		
uint8 DisplayWindow(WINDOWS_NEW *pwin);

static uint8_t testbuf[10]={'0'};
 uint8_t heartcnt=0;
uint8_t get_option_index_select();


//返回上一级菜单
#define SelfMenuIndex FatherIndex[Layer-1]

//返回当前菜单
#define CurrentMenuIndex FatherIndex[Layer]

uint8 KeyFuncIndex = 0;        //索引
//也就是连续使用Push的个数
#define MenuSize 4          //菜单深度最大值

//3个备份数组
uint8 FatherIndex[MenuSize+1]; //父菜单备份数组
uint8 Layer = 0;               //菜单层数

uint8 ItemBackup[MenuSize];      //当前菜单项备份
uint8 ItemBackup_i = 0;          //

uint8 WhereBackup[MenuSize];     //反显位置备份
uint8 WhereBackup_i = 0;         //
uint8 Where;           //当前反显位置,[0,DispMax-1]  反映反显的Bar在屏幕上的位置

//GUI_SetBkColor(GUI_BLACK);
//GUI_SetTextMode(GUI_TM_NORMAL);

/*****************************************************************************
 * 函 数 名  : Disp_set_text_color
 * 负 责 人  : MZM
 * 创建日期  : 2016年5月20日
 * 函数功能  : 设置显示的字体
 * 输入参数  : uint32_t color  字体颜色
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void Disp_set_text_color(uint32_t color)
{
	GUI_SetColor(color);
}

/*****************************************************************************
 * 函 数 名  : Disp_set_text_mode
 * 负 责 人  : MZM
 * 创建日期  : 2016年5月20日
 * 函数功能  : 设置字体模式
 * 输入参数  : uint8_t textmode  设置字体模式
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void Disp_set_text_mode(uint8_t textmode)
{
	GUI_SetTextMode(textmode);
}

//一般情况下显示
/*****************************************************************************
 * 函 数 名  : Disp_text_mode_general
 * 负 责 人  : MZM
 * 创建日期  : 2016年5月20日
 * 函数功能  : 显示普通文本
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void Disp_text_mode_general()
{
	Disp_set_text_mode(GUI_TM_TRANS);
}

void Disp_text_mode_select()
{
	Disp_set_text_mode(GUI_TM_REV);
}
//GUI_DispStringAt("windows_func5",0,32);

void Disp_string_at_xy(uint16_t x,uint16_t y,uint8_t *string)
{
	GUI_DispStringAt(string,x,y);
}
void set_text_mode_by_select_flg(uint8_t select_flg)
{
		if(get_option_index_select()== select_flg)
		{			
			Disp_text_mode_select();
		}
		else
		{
			Disp_text_mode_general();
		}
}

/*
*****************************************************************************
* InitMenuVal - 初始化菜单系统
* DESCRIPTION: -
* 该函数必须在进入菜单前被调用   初始化全局变量
* @Para void:
* Return :
*
*****************************************************************************
*/
void InitMenuVal(void)
{
//    WhereBackup_i = 0;
//    ItemBackup_i = 0;
//    Layer = 0;
//    FatherIndex[Layer++] = MenuTopIndex;  //push index  待机界面作为起点
}
bool GUI_Display_init()
{
	gui_display_dev=(DISPLAY_OPERATE_T*) pvPortMalloc(sizeof(DISPLAY_OPERATE_T));
	if(gui_display_dev==NULL)
	{
		rt_kprintf("%s: malloc ERROR\r\n",__FUNCTION__);
		return FALSE;
	}
	else{
		rt_kprintf("%s: malloc SUCESS\r\n",__FUNCTION__);
		}

	memset(gui_display_dev,0,sizeof(DISPLAY_OPERATE_T));
}

void clear_display_dev()
{
	memset(gui_display_dev,0,sizeof(DISPLAY_OPERATE_T));

}
DISPLAY_OPERATE_T * Get_Display_dev()
{
	return gui_display_dev;
}
void set_key_fun_index(uint8_t index)
{
	gui_display_dev->key_fun_index = index;
}
uint8_t get_key_fun_index()
{
	return gui_display_dev->key_fun_index;
}

void set_windows_index(uint8_t index)
{
	gui_display_dev->windows_index = index;
}
uint8_t get_windows_index()
{
	return gui_display_dev->windows_index;
}

void set_menu_index(uint8_t index)
{
	gui_display_dev->menu_index = index;
}
uint8_t get_menu_index()
{
	return gui_display_dev->menu_index;
}
void set_option_index(uint8_t index)
{
	gui_display_dev->option_index = index;
}
uint8_t get_option_index()
{
	return gui_display_dev->option_index;
}
void set_option_index_select(uint8_t index)
{
	gui_display_dev->option_index_select= index;
}
uint8_t get_option_index_select()
{
	return gui_display_dev->option_index_select;
}
void set_flash_mode(uint8_t index)
{
	gui_display_dev->flash_mode= index;
}
uint8_t get_flash_mode()
{
	return gui_display_dev->flash_mode;
}
void set_current_menu_state(uint8_t index)
{
	gui_display_dev->current_menu_state= index;
}
uint8_t get_current_menu_state()
{
	return gui_display_dev->current_menu_state;
}

void set_item_max_num(uint8_t index)
{
	gui_display_dev->item_max_num= index;
}
uint8_t get_item_max_num()
{
	return gui_display_dev->item_max_num;
}


uint8_t * get_key_buf()
{
	return gui_display_dev->fun_key_str;
}

void set_key_offset(uint8_t index)
{
	gui_display_dev->fun_key_offset = index;
}

uint8_t get_key_offset()
{
	return gui_display_dev->fun_key_offset;
}

void set_key_oksec(uint8_t index)
{
	gui_display_dev->fun_key_okesc= index;
}

uint8_t get_key_oksec()
{
	return gui_display_dev->fun_key_okesc;
}


//跳转到指定菜单
void Jump2Menu(uint8 Index,uint8 FlashMode)
{
    set_key_fun_index(Index);             //jump to Menu index
    if(FlashMode){
		set_flash_mode(FlashMode);
    }
}
//-----------------------
//菜单备份函数
//备份当前菜单的各个参数
void PUSH(void)
{
//    WhereBackup[WhereBackup_i++] = get_option_index_select();      //备份Where--->高亮显示的位置
    WhereBackup[WhereBackup_i++] = Where;      //备份Where--->高亮显示的位置
    ItemBackup[ItemBackup_i++] = get_option_index();         //备份Item--->当前项的位置
    FatherIndex[Layer++] = get_key_fun_index();      //备份KeyFuncIndex--->状态机的状态

//-------------------------------

}
//菜单还原函数
void POP(void)
{
    Layer--;      //先弹出一层,因为使用的是++,因此当前Layer指向下一个数组位置
//    KeyFuncIndex = FatherIndex[Layer-1];     //恢复KeyFuncIndex
    Where = WhereBackup[--WhereBackup_i];    //恢复高亮显示位置Where
//    Item = ItemBackup[--ItemBackup_i];       //恢复项位置Item
//	set_option_index_select(WhereBackup[--WhereBackup_i]);

    set_key_fun_index(FatherIndex[Layer-1]);
	set_option_index(ItemBackup[--ItemBackup_i]);
//-------------------------------

//-------------------------------
//    Flash = FlashMode_ReFlash;                     //子项返回,请求刷新
}

void shortkey_check_option_index_select_state()
{
	uint8_t selectflg;

	selectflg = get_option_index_select();
	if(selectflg == SELECT_ITEM_NULL)
	{
		set_current_menu_state(0);
	}


}
void Shortkey_Okesc_Funcreturn()
{
	uint8_t selectflg;

	
	set_key_oksec(0);
	selectflg = get_option_index_select();
	if(selectflg != SELECT_ITEM_NULL)
	{
		set_option_index_select(--selectflg);
	}
	else
	{
		set_current_menu_state(0);
	}

}
/****************************************************************************
* 名称：MenuIcoDrawA()
* 功能：根据ico(MENUICO类型)的数据显示图标菜单。
* 入口参数：ico			单个图标菜单数据
*           sel			为0时表示非选中，为1时表示选中
* 出口参数：无
****************************************************************************/
void MenuDrawA(ST_MENUITEM *pmenuitem, uint8 sel, uint8 *pBackpic)
{
	if(pmenuitem->picodat != NULL)
	{
//		if(sel == 1)
//		{
//			Lcd_DisplayIco(pmenuitem->x, pmenuitem->y, pmenuitem->picodat + 48*48*2, 48, 48, pBackpic);
//		}
//		else
//		{
//			Lcd_DisplayIco(pmenuitem->x, pmenuitem->y, pmenuitem->picodat, 48, 48, pBackpic);
//		}
	}
	if(pmenuitem->title != NULL)
	{
		if(sel == 1)
		{			
			Disp_text_mode_select();
			Disp_string_at_xy(pmenuitem->x, pmenuitem->y, pmenuitem->title);			
		}
		else
		{
			Disp_text_mode_general();
			Disp_string_at_xy(pmenuitem->x, pmenuitem->y, pmenuitem->title);			
		}
	}
}


/*
*****************************************************************************
* DispMenuTop - 待机界面层5个函数
* DESCRIPTION: -
* DispMenuTop   DispMenuTopUp  DispMenuTopDown  DispMenuTopLeft DispMenuTopRight
*
*
*
*****************************************************************************
*/
void DispMenuTop(void)//待机界面
{
	fun_print();
	clear_display_dev();

	DisplayWindow(&TopWindows[0]);//显示待机界面窗体
	
	
}

void DispMenuTopOk(void)
{
	fun_print();

	Jump2Menu(0, FlashMode_NoAction);
}

void DispMenuTopEsc(void)
{
	fun_print();
	clear_display_dev();
	Jump2Menu(0, FlashMode_ReFlash);
}



void DispMenuTopUp(void)
{
//--------------------------
//状态跳转,进入子菜单
	fun_print();

    //jump to Menu index
    Jump2Menu(MenuIndex,FlashMode_ManualInit);

}
void DispMenuTopDown(void)
{		
//--------------------------
//状态跳转,进入子菜单
	fun_print();

    //jump to Menu index
    Jump2Menu(MenuIndex,FlashMode_AutoInit);
	
} 
    	
void DispMenuTopLeft(void)
{
	fun_print();
	Jump2Menu(0,FlashMode_NoAction);
}
void DispMenuTopRight(void)
{
	fun_print();
	Jump2Menu(0,FlashMode_NoAction);	
}

void DispIcoMenuInit(void)
{
    //ItemNum = IcoMenu[language].TextNum;//多少数据项
    //DispItem = IcoMenu[language].Text;  //数据
	WINDOWS_NEW *pwin_temp;
	MY_NEW_MENU *pmenu;
	uint8_t windows_index,item;
	uint8_t flashmode = get_flash_mode();
	
	fun_print();

	windows_index = get_windows_index();

    if(flashmode == FlashMode_AutoInit)
	{//从0进入,初始化Item等值
//		windows_index += 1;
//		windows_index %=WINDOWS_NUM;
//		
		PUSH();
//		set_option_index_select(0);

	    windows_index = ((windows_index == (WINDOWS_NUM - 1)) ? 0 : (windows_index + 1));

		set_windows_index(windows_index);
		
		set_option_index(0);
		DisplayWindow(&TopWindows[windows_index]);         
    }
    else
    {
    	if(flashmode == FlashMode_ManualInit)
    	{		
    	
			PUSH();
//			set_option_index_select(0);
		   windows_index = ((windows_index == 0) ? (WINDOWS_NUM - 1) : (windows_index - 1));
		   set_windows_index(windows_index);
		   
		   set_option_index(0);
		   DisplayWindow(&TopWindows[windows_index]);		  
	    }
	   
	}

}

void InitDg_Par(void)
{
//    tftset_chinfo_promise=0;
//    save_display_alarm_state=0xffffffff;
//    save_pc_link_f=0xff;
//    save_chcdlink_f=0xff;
//    save_key_lock_f=0xff;
//    
//	save_pavolatage_colorlump=0xff;	
//	save_fwdp_colorlump=0xff;
//	save_refp_colorlump=0xff;
//	save_temp_colorlump=0xff;
//	save_rssip_colorlump=0xff;
//	save_vswr_colorlump=0xff;
//	save_mutelevel_colorlump=0xff;
//	save_spklevel_colorlump=0xff;
//pdt(兼容报警取消后的状态)eg:有报警后，save_pdttime1statetype的状态为正常，pdttime1statetype的状态改为故障，再变为正常后，屏幕不刷新
//    save_pdttime1statetype=0xff;
//    save_pdttime2statetype=0xff;
//    save_boardno_pdt=0xff;
//    save_chnumber_pdt=0xffff;
//	save_gpsstate_pdt=0xff;
//	save_dbconnectstate_pdt=0xff;
//	save_bsconnectstate_pdt=0xff;
//	save_wltalklimit1_pdt=0xffff;
//	save_wltalklimit2_pdt=0xffff;
//	save_talktimeinfo2_pdt=0xffff;
//	save_talktimeinfo1_pdt=0xffff;
//	
//	save_pdt2_voice_f=0xff;
//	save_pdt1_voice_f=0xff;	
//    //save_gpsnumber_pdt=0xff;
//vsc
//	save_vscchstatetype=0xff;
//	save_vsc_chno=0xff;
//	save_vsc_groupno=0xff;
//	save_vsc_bsno=0xff;
//	save_vsc_centerconnect=0xff;
//	save_vsc_gpsstate=0xff;
//	save_vsc_e1ctlconnect=0xff;
//	save_vsc_vscmode=0xff;
//	
//mpt1327
//	save_mptchstatetype=0xff;
//	save_boardno=0xff;
//	save_chnumber=0xffff;
//	save_connectstate=0xffff;
//	save_wltalklimit1_mpt=0xffff;
//	save_talktimeinfo_mpt=0xffff;	
//	
//    save_mpt_voice_f=0xff;
}

void DispIcoMenuUp(void)
{
  //  InitDg_Par();
	uint8_t windows_index;
  	uint8_t menu_index;
  
  	fun_print();
  	menu_index = get_menu_index();
  	windows_index = get_windows_index();
	menu_index = get_key_fun_index();

//	windows_index -= 1;	
//	windows_index %=WINDOWS_NUM;
	
    windows_index = ((windows_index == 0) ? (WINDOWS_NUM - 1) : (windows_index - 1));
	set_windows_index(windows_index);
	DisplayWindow(&TopWindows[windows_index]);	 
	Jump2Menu(SelfMenuIndex,FlashMode_NoAction);
}

void DispIcoMenuDown(void)
{
	
	uint8_t windows_index;
	uint8_t menu_index;

	
  	fun_print();
	menu_index = get_menu_index();
  	windows_index = get_windows_index();
	
	menu_index = get_key_fun_index();
//	windows_index += 1; 
//	windows_index %=WINDOWS_NUM;
	
    windows_index = ((windows_index == (WINDOWS_NUM - 1)) ? 0 : (windows_index + 1));
	set_windows_index(windows_index);

	DisplayWindow(&TopWindows[windows_index]);	 
	
	Jump2Menu(SelfMenuIndex,FlashMode_NoAction);
	//Jump2Menu(SelfMenuIndex,FlashMode_NoAction);
}

void DispIcoMenuLeft(void)
{
  	fun_print();

	Jump2Menu(MenuIndex,FlashMode_NoAction);
}

void DispIcoMenuRight(void)
{
  	fun_print();

	Jump2Menu(MenuIndex,FlashMode_NoAction);
}



void DisplayMenu(WINDOWS_NEW *pwin, uint8 sel_no)
{
    MY_NEW_MENU *pmenu;
    uint8 i;
    uint16 xpos, ypos;
    
  	fun_print();
	
    pmenu = pwin->pMenu;
    if(pmenu != NULL)
    {
        while(1)
        {
            if(pmenu->CurrentIndex == sel_no)
            {
                MenuDrawA(pmenu->pmenuico, 1, pwin->pdot);
            }
            else
            {
                MenuDrawA(pmenu->pmenuico, 0, pwin->pdot);
            }

            if(pmenu->pmenuico->ptext != NULL)
            {   
                xpos = pmenu->pmenuico->x ;
                ypos = pmenu->pmenuico->y;
				Disp_string_at_xy(xpos,ypos,pmenu->pmenuico->ptext);
//                LCD_PutString_New(xpos, ypos, pmenu->pmenuico->ptext,FONT16|ZT_CS, pwin->pdot, BLACK);
            }
            
            if(pmenu->down_menu == NULL)
            {
                break;
            }

            pmenu = pmenu->down_menu;
        }
    }
    return;
}

void DispIcoMenuOk(void)
{
  	fun_print();

	switch(get_windows_index())
	{
	
		case 0x03:
			
			Jump2Menu(MenuFuncIndex,FlashMode_AutoInit);
			break;
		default:
			
			Jump2Menu(MenuIndex,FlashMode_NoAction);
			break;
	}
	
//	Jump2Menu(MenuFuncIndex,FlashMode_AutoInit);
	
}

void DispIcoMenuEsc(void)
{
	//POP();
	
  	fun_print();
	POP();
//	Jump2Menu(0,FlashMode_ReFlash);
}

void DispMenuFuncInit(void)
{
	WINDOWS_NEW *pwin_temp;
	uint8_t windows_index,item;
	uint8_t flashmode = get_flash_mode();

  	fun_print();

	item = get_option_index();
	windows_index = get_windows_index();

	set_item_max_num(IcoMenuNum[windows_index]);// 选项总数
	
	item = 0;
	pwin_temp=&TopWindows[windows_index];
	//Disp_text_mode_select();
	//DisplayMenu(pwin_temp, item);
	set_option_index_select(SELECT_ITEM_MENU);	
	DisplayWindow(&TopWindows[windows_index]);	 
	set_windows_index(windows_index);
	set_option_index(item);
	 if(flashmode == FlashMode_AutoInit){//从0进入,初始化Item等值
        PUSH();
		set_option_index(0);
//		set_option_index_select(0);
       Where = 0;         
    }
    else
    {
    	if(flashmode == FlashMode_ManualInit)
    	{
			PUSH();
			set_option_index(0);
//			set_option_index_select(0);
	        Where = 0; 		
	    }
	}	
	
}

void DispMenuFuncOk(void)
{
  	fun_print();

	Jump2Menu(MenuFuncIndex,FlashMode_func);

}

void DispMenuFuncEsc(void)
{
	uint8_t windows_index,item;
	
	fun_print();
//	
//	item = get_option_index();
//	windows_index = get_windows_index();
//	
//	set_option_index_select(SELECT_ITEM_NULL);	
//	DisplayWindow(&TopWindows[windows_index]);	

	if(get_option_index_select() == SELECT_ITEM_NULL)
		{
			windows_index = get_windows_index();
			
			POP();
			
			set_current_menu_state(0);
			DisplayWindow(&TopWindows[windows_index]);	
			
			Jump2Menu(MenuIndex,FlashMode_NoAction);
		}
	else{
		Jump2Menu(MenuFuncIndex,FlashMode_func);
		}

	
}

void DispMenuFuncUp(void)
{
  
	WINDOWS_NEW *pwin_temp;
	MY_NEW_MENU *pmenu;
	
	uint8_t windows_index,item,item_max;
  	fun_print();

	
	item_max = get_item_max_num();
	item = get_option_index();
	windows_index = get_windows_index();
//    pwin_temp=&TopWindows[windows_index];    
//	pmenu = pwin_temp->pMenu+item;
	
//    if(pmenu->upper_menu != NULL)
//    {          
//        MenuDrawA(pmenu->pmenuico, 0, pwin_temp->pdot); //反显当前选项                   
//        pmenu = pmenu->upper_menu;
//        MenuDrawA(pmenu->pmenuico, 1, pwin_temp->pdot); //反显选中的选项 
//    }

	if (item == 0) {
		item = item_max-1;
//		MenuDrawA(pmenu->pmenuico, 0, pwin_temp->pdot); //反显选中的选项 
//		pmenu = pwin_temp->pMenu+item;
//		MenuDrawA(pmenu->pmenuico, 1, pwin_temp->pdot); //反显当前选项     
	} 
	else
	{
		item--;
	}

    set_option_index(item);
	
	DisplayWindow(&TopWindows[windows_index]);	 
    Jump2Menu(MenuFuncIndex,FlashMode_NoAction);
}

void DispMenuFuncDown(void)
{
	WINDOWS_NEW *pwin_temp;
	MY_NEW_MENU *pmenu;
	uint8_t windows_index,item,item_max;
	uint8_t flashmode = get_flash_mode();
	
  	fun_print();
	item = get_option_index();
	item_max= get_item_max_num();
	
	windows_index = get_windows_index();


	
//    pwin_temp=&TopWindows[windows_index];    
//	pmenu = pwin_temp->pMenu+item;
//	
//    if(pmenu->down_menu != NULL)
//    {
//        MenuDrawA(pmenu->pmenuico, 0, pwin_temp->pdot);// 反显
//        pmenu = pmenu->down_menu;
//        MenuDrawA(pmenu->pmenuico, 1, pwin_temp->pdot);// 显示 选中项
//    }

   // if (item == (ItemNum-1)) {
	if (item == (item_max-1))
	{
        item = 0;
//    	MenuDrawA(pmenu->pmenuico, 0, pwin_temp->pdot);// 反显
//        pmenu = pwin_temp->pMenu+item;
//        MenuDrawA(pmenu->pmenuico, 1, pwin_temp->pdot);// 显示 选中项
    }
	else
	{
        item++;
    }
    set_option_index(item);
 //   set_option_index(item);
	
	DisplayWindow(&TopWindows[windows_index]);	 
    Jump2Menu(MenuFuncIndex,FlashMode_NoAction);
}

void DispMenuFuncLeft(void)
{
  	fun_print();
	Jump2Menu(MenuFuncIndex,FlashMode_NoAction);
}
void DispMenuFuncRight(void)
{
  	fun_print();
	Jump2Menu(MenuFuncIndex,FlashMode_NoAction);
}



uint8 DisplayWindow(WINDOWS_NEW *pwin)
{
    MY_NEW_MENU *pmenu;
    uint16 xpos,ypos;
	uint8_t windows_index, item,tmpbuf[20];
    
  	fun_print();
	
	GUI_SetFont(&GUI_FontHT24);
	GUI_Clear();
    /*显示窗体*/
    GUI_DrawBitmap((void*)pwin->pdot,pwin->xpos, pwin->ypos);
	windows_index = get_windows_index();
	
	item = get_option_index();
#ifdef DIS_WORD	
	sprintf(tmpbuf,"windows[%d]",windows_index+1);
	GUI_DispStringAt(tmpbuf,130,00);
#endif
    //Lcd_FlushSCR();
    pmenu = pwin->pMenu;
    if(pmenu != NULL)
    {   /*显示窗体中的内容*/ 
        while(1)
        {           
            if(pmenu->pmenuico->title != NULL)
        	{/* 显示TITILE*/
        	    if(pmenu->CurrentIndex == item)
            	{
					set_text_mode_by_select_flg(SELECT_ITEM_MENU);

//					if(get_option_index_select()== SELECT_ITEM_MENU)
//					{			
//						Disp_text_mode_select();
//					}
//					else
//					{
//						Disp_text_mode_general();
//					}
         
	            }
        	    GUI_DispStringAt(pmenu->pmenuico->title,pmenu->pmenuico->x, pmenu->pmenuico->y);
	        }
	        
			Disp_text_mode_general();
            if(pmenu->pmenuico->ptext != NULL)
            {/* 显示 内容*/
               // xpos = pmenu->pmenuico->x + DANWEIOFFER;
                xpos = pmenu->pmenuico->x + 20;
                ypos = pmenu->pmenuico->y;
        	    GUI_DispStringAt(pmenu->pmenuico->title,pmenu->pmenuico->x, pmenu->pmenuico->y);
                //Lcd_FlushSCR();
            }

			if(pmenu->pmenuico->picodat != NULL)
            {/* 显示 内容*/
               // xpos = pmenu->pmenuico->x + DANWEIOFFER;
               
			   	GUI_DrawBitmap((void*)pmenu->pmenuico->picodat,pmenu->pmenuico->x, pmenu->pmenuico->y);
               
                //Lcd_FlushSCR();
            }
            if(pmenu->pmenuico->IcoFunction != NULL)
            {
            	pmenu->pmenuico->IcoFunction(0xFF,pwin->pdot);
            }           
            if(pmenu->down_menu == NULL)
            {   /* 如果 没有下一项 跳出*/
                break;
            }
            
            pmenu = pmenu->down_menu;   
            //Lcd_FlushSCR();
        }
    }
    
    return;
}

void func_chnumber(uint16 index,void *pvoid)
{
	uint8_t tmpbuf[20];
  	fun_print();
#ifdef DIS_WORD	

	sprintf(tmpbuf,"windows_fun[%d]",index);
	GUI_DispStringAt(tmpbuf,0,16);
	#endif

}
uint32_t CH_no_2_Freq(uint16_t chnum)
{
	uint32_t freqR=0,freqN=0;

	
	freqR += 348 + (chnum *25/2)/1000;
	freqN = (chnum *25/2)%1000;

	return freqR*1000+freqN;
}


void CH_2_Freq_str(uint16_t chnum,uint8_t *freqstr,uint8_t txflg)
{
	uint32_t freqR=0,freqN=0;

	if(txflg == 0)
	{
		freqR += 10;// 发射
	}
	
	freqR += 348 + (chnum *25/2)/1000;
	freqN = (chnum *25/2)%1000;

	sprintf(freqstr,"%d.%dMHZ",freqR,freqN);
	return;
}
void task_draw_net_icon(uint8_t flag)
{
	GUI_DrawBitmap(kb_pic_struct.net_pic[flag%NET_ICON_NUM],CZ_SECOND_PIC_X_OFFSET, CZ_SECOND_PIC_Y_OFFSET);
}

void task_draw_msc_icon(uint8_t flag)
{
	GUI_DrawBitmap(kb_pic_struct.msc_pic[flag%MSC_ICON_NUM],CZ_THREE_PIC_X_OFFSET, CZ_THREE_PIC_Y_OFFSET);
}

void task_draw_db_icon(uint8_t flag)
{
	GUI_DrawBitmap(kb_pic_struct.db_pic[flag%DB_ICON_NUM],CZ_FOUR_PIC_X_OFFSET, CZ_FOUR_PIC_Y_OFFSET);
}

void test_draw_heart(uint8_t heartcnt)
{


#if 0
	if(heartcnt%5 == 0)
		{
			GUI_DrawBitmap((void*)&bmheartstep1,CZ_FIRST_PIC_X_OFFSET, CZ_FIRST_PIC_Y_OFFSET);
		}
		else if(heartcnt%5 == 1)
		{
			GUI_DrawBitmap((void*)&bmheartstep2,CZ_FIRST_PIC_X_OFFSET, CZ_FIRST_PIC_Y_OFFSET);
		}
		else if(heartcnt%5 == 2)
		{
			GUI_DrawBitmap((void*)&bmheartstep3,CZ_FIRST_PIC_X_OFFSET, CZ_FIRST_PIC_Y_OFFSET);
		}
		else if(heartcnt%5 == 3)
		{
			GUI_DrawBitmap((void*)&bmheartstep4,CZ_FIRST_PIC_X_OFFSET, CZ_FIRST_PIC_Y_OFFSET);
		}
	else{
			GUI_DrawBitmap((void*)&bmheartstep5,CZ_FIRST_PIC_X_OFFSET, CZ_FIRST_PIC_Y_OFFSET);
		}

	#else

	GUI_DrawBitmap(kb_pic_struct.heartstep[heartcnt%HEART_ICON_NUM],CZ_FIRST_PIC_X_OFFSET, CZ_FIRST_PIC_Y_OFFSET);
//	GUI_DrawBitmap(kb_pic_struct.heartstep[heartcnt%HEART_ICON_NUM],CZ_AREA3_FOUR_PIC_X, CZ_AREA3_FOUR_PIC_Y);

//	GUI_DrawBitmap(kb_pic_struct.heartstep1[heartcnt%HEART_ICON_NUM],CZ_AREA3_FIRST_PIC_X, CZ_AREA3_FIRST_PIC_Y);
//	GUI_DrawBitmap(&bmnet_black2,CZ_AREA3_SECOND_PIC_X, CZ_AREA3_SECOND_PIC_Y);
//	GUI_DrawBitmap(&bmmsc_black2,CZ_AREA3_THREE_PIC_X, CZ_AREA3_THREE_PIC_Y);
//	GUI_DrawBitmap(&bmdb_black2,CZ_AREA3_FOUR_PIC_X, CZ_AREA3_FOUR_PIC_Y);

	
	#endif
}


//返回 居中的位置  start_xy起始坐标，字符宽度，字符个数，显示区域极限值
uint8_t fun_display_align_center(uint8_t start_xy,uint8_t char_width,uint8_t char_nums,uint8_t limit_xy)
{
	uint8_t retvalue;

	if(limit_xy < char_width *char_nums)
	{
		retvalue = 0;//字符超出，从开始显示
	}
	else
	{
		retvalue = (limit_xy - char_nums*char_width);//计算中间位置
		retvalue /= 2;
	}

	retvalue += start_xy;

	return retvalue;
	
}
//获取工作模式的字符串
uint8_t * Get_string_work_mode_to_area2()
{
	return work_mode_title[Get_Comm_param_Work_Mode()];
}

//获取工作模式1的字符串
uint8_t * Get_string_work_mode_1_to_area2()
{
	uint8_t **tmpptr;
	tmpptr = work_mode1_title.work_mode1_title[Get_Comm_param_Work_Mode()];

	return tmpptr[Get_Comm_param_Work_Mode1()];
}
//获取控制通道状态的字符串
uint8_t * Get_string_ctr_ch_state1_to_area3()
{
	return Ctr_CH_State_title[Get_Ctr_CH_State_1_param()];
}
//获取控制通道状态的字符串
uint8_t * Get_string_ctr_ch_state2_to_area3()
{
	return Ctr_CH_State_title[Get_Ctr_CH_State_2_param()];
}


void func_display_area_1_3(uint16 index,void *pvoid)
{
  	fun_print();

	if(Get_Heart_Icon_State() == STATE_ENABLE)
	{
		test_draw_heart(0);
	}
	task_draw_net_icon(Get_Net_Icon_State());
	task_draw_msc_icon(Get_MSC_Icon_State());
	task_draw_db_icon(Get_DBS_Icon_State());
//	GUI_DrawBitmap(kb_pic_struct.heartstep[heartcnt%HEART_ICON_NUM],CZ_FIRST_PIC_X_OFFSET, CZ_FIRST_PIC_Y_OFFSET);
	//GUI_DrawBitmap((void*)&bmheart1,CZ_FIRST_PIC_X_OFFSET, CZ_FIRST_PIC_Y_OFFSET);
//	GUI_DrawBitmap((void*)&bmnet2,CZ_SECOND_PIC_X_OFFSET, CZ_SECOND_PIC_Y_OFFSET);
//	GUI_DrawBitmap((void*)&bmmsc2,CZ_THREE_PIC_X_OFFSET, CZ_THREE_PIC_Y_OFFSET);
//	GUI_DrawBitmap((void*)&bmdb2,CZ_FOUR_PIC_X_OFFSET, CZ_FOUR_PIC_Y_OFFSET);
//	GUI_SetColor(GUI_WHITE);
//	GUI_SetFont(GUI_FontHT24);
//	
//	GUI_DispStringAt("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 0, 16);
//	
//	GUI_DrawBitmap((void*)&bmDBblock,CZ_AREA3_FIRST_PIC_X, CZ_AREA3_FIRST_PIC_Y);
//	GUI_DrawBitmap((void*)&bmDBblock1,CZ_AREA3_SECOND_PIC_X, CZ_AREA3_SECOND_PIC_Y);


//	GUI_DrawBitmap((void*)&bmdown2,CZ_AREA3_FIRST_PIC_X, CZ_AREA3_FIRST_PIC_Y);
//	GUI_DrawBitmap((void*)&bmup2,CZ_AREA3_SECOND_PIC_X, CZ_AREA3_SECOND_PIC_Y);
//	
//	GUI_DrawBitmap((void*)&bmdown2,CZ_AREA3_THREE_PIC_X, CZ_AREA3_THREE_PIC_Y);
//	GUI_DrawBitmap((void*)&bmup2,CZ_AREA3_FOUR_PIC_X, CZ_AREA3_FOUR_PIC_Y);
}
void func_display_area_2(uint16 index,void *pvoid)
{
	uint8_t num,tmpx,tmpy;
	
  	fun_print();
	
	GUI_SetColor(GUI_WHITE);

	
	num = strlen(Get_string_work_mode_to_area2());
	GUI_SetFont(CZ_AREA2_TITLE1_FONT);
	
	tmpx = fun_display_align_center(CZ_AREA2_TITLE_X_START,CZ_AREA2_TITLE1_FONT_WIDTH,num,CZ_AREA2_TITLE_X_LIMIT);
	tmpy = fun_display_align_center(CZ_AREA2_TITLE_Y_START,CZ_AREA2_TITLE1_FONT_HEIGHT,1,24);
	
	GUI_DispStringAt(Get_string_work_mode_to_area2(), tmpx, tmpy);

	
	GUI_SetFont(CZ_AREA2_TITLE2_FONT);
	num = strlen(Get_string_work_mode_1_to_area2());
	
	tmpx = fun_display_align_center(CZ_AREA2_TITLE_X_START,CZ_AREA2_TITLE2_FONT_WIDTH,num,CZ_AREA2_TITLE_X_LIMIT);
	tmpy = fun_display_align_center(CZ_AREA2_TITLE_Y_START+22,CZ_AREA2_TITLE2_FONT_HEIGHT,1,20);
	GUI_DispStringAt(Get_string_work_mode_1_to_area2(), tmpx, tmpy);

}

void func_get_ctr_ch_state_string(uint8_t state_flg)
{
	uint8_t state,*tmpstr;	
	uint8_t num,tmpx,tmpy;
	uint8_t string[20];
	uint16_t call_timer,sys_id;

	call_timer = Get_Call_Timer_Param();
	sys_id = Get_System_ID_Param();
	
	if(state_flg == 0)
	{
	
		GUI_SetFont(CZ_AREA3_TITLE1_FONT);
		state = Get_Ctr_CH_State_1_param();
		tmpstr = Get_string_ctr_ch_state1_to_area3();
		tmpx = CZ_AREA3_TITLE1_X_START;
		tmpy = fun_display_align_center(CZ_AREA3_TITLE1_Y_START,CZ_AREA3_TITLE1_FONT_HEIGHT,1,CZ_AREA3_TITLE_Y_LIMIT);
		
	}
	else
	{
	
		GUI_SetFont(CZ_AREA3_TITLE2_FONT);
		state = Get_Ctr_CH_State_2_param();
		tmpstr = Get_string_ctr_ch_state2_to_area3();
		
		tmpx = CZ_AREA3_TITLE2_X_START;
		tmpy = fun_display_align_center(CZ_AREA3_TITLE2_Y_START,CZ_AREA3_TITLE2_FONT_HEIGHT,1,CZ_AREA3_TITLE_Y_LIMIT);
	}
	
	if(state == 0
)
	{
//		sprintf(string,"%s%x",tmpstr,sys_id);
		sprintf(string,"%sB1F1",tmpstr);
	}
	else if(state == 2)
	{
		sprintf(string,"%s   %02d:%02d",tmpstr,call_timer/60,call_timer%60);
	}
	else if(state == 1)
	{
		
		GUI_DispStringAt(tmpstr, tmpx, tmpy);
		
		sprintf(string,"  %02d:%02d",call_timer/60,call_timer%60);
		GUI_SetColor(GUI_GRAY);

		num = strlen(tmpstr);
		
		tmpx += num *CZ_AREA3_TITLE1_FONT_WIDTH;
		
		GUI_DispStringAt(string, tmpx, tmpy);
		
		GUI_SetColor(GUI_WHITE);

		return;
	}
	
	GUI_SetColor(GUI_WHITE);
//	GUI_SetFont(CZ_AREA2_TITLE2_FONT);
	GUI_DispStringAt(string, tmpx, tmpy);
}
void func_display_area_3(uint16 index,void *pvoid)
{
	uint8_t num,tmpx,tmpy,workmode;
	
	uint8_t stringtmp1[20];
	
  	fun_print();

	workmode = Get_Comm_param_Work_Mode();
	if(workmode == WORK_MODE_PDT)
	{
		func_get_ctr_ch_state_string(0);
		func_get_ctr_ch_state_string(1);
	}
	else if(workmode == WORK_MODE_DMR)
		{
//		GUI_DrawArc(CZ_AREA3_TITLE1_X_START,CZ_AREA3_TITLE1_Y_START,CZ_AREA3_TITLE2_X_START,CZ_AREA3_TITLE2_Y_START,0x33);
	GUI_DrawBitmap((void*)&bmdown2,CZ_AREA3_FIRST_PIC_X, CZ_AREA3_FIRST_PIC_Y);
	GUI_DrawBitmap((void*)&bmup2,CZ_AREA3_SECOND_PIC_X, CZ_AREA3_SECOND_PIC_Y);
	
	GUI_DrawBitmap((void*)&bmdown2,CZ_AREA3_THREE_PIC_X, CZ_AREA3_THREE_PIC_Y);
	GUI_DrawBitmap((void*)&bmup2,CZ_AREA3_FOUR_PIC_X, CZ_AREA3_FOUR_PIC_Y);
		}
//	GUI_SetColor(GUI_WHITE);

//	
//	num = strlen(Get_string_ctr_ch_state1_to_area3());
//	sprintf(stringtmp1,"%s %d",Get_string_ctr_ch_state1_to_area3(),Get_System_ID_Param());

//	num = strlen(stringtmp1);
//	tmpx = fun_display_align_center(CZ_AREA3_TITLE1_X_START,CZ_AREA3_TITLE1_FONT_WIDTH,num,CZ_AREA3_TITLE_X_LIMIT);
//	tmpy = fun_display_align_center(CZ_AREA3_TITLE1_Y_START,CZ_AREA3_TITLE1_FONT_HEIGHT,1,CZ_AREA3_TITLE_Y_LIMIT);
//	
//	GUI_SetFont(CZ_AREA2_TITLE2_FONT);
//	GUI_DispStringAt(Get_string_ctr_ch_state1_to_area3(), tmpx, tmpy);

//	
//	GUI_SetFont(CZ_AREA2_TITLE2_FONT);
//	num = strlen(Get_string_ctr_ch_state2_to_area3());
//	
//	tmpx = fun_display_align_center(CZ_AREA3_TITLE2_X_START,CZ_AREA3_TITLE2_FONT_WIDTH,num,CZ_AREA3_TITLE_X_LIMIT);
//	tmpy = fun_display_align_center(CZ_AREA3_TITLE2_Y_START,CZ_AREA3_TITLE2_FONT_HEIGHT,1,CZ_AREA3_TITLE_Y_LIMIT);
//	GUI_DispStringAt(Get_string_ctr_ch_state2_to_area3(), tmpx, tmpy);

}

void func_display_area_4(uint16 index,void *pvoid)
{
	uint8_t num,tmpx,tmpy;
	
  	fun_print();
	num = strlen(CZ_AREA4_TITLE_SRTING);
	
	GUI_SetColor(GUI_WHITE);
	GUI_SetFont(CZ_AREA4_TITLE_FONT);

//	tmpy = CZ_AREA4_TITLE_Y_BASE;	
	tmpx = fun_display_align_center(CZ_AREA4_TITLE_X_START,CZ_AREA4_TITLE_FONT_WIDTH,num,CZ_AREA4_TITLE_X_LIMIT);
//	tmpx = 80-num*CZ_AREA4_TITLE_FONT_WIDTH;
//	tmpx /= 2;
//	tmpx += CZ_AREA4_TITLE_X_BASE;

	tmpy = fun_display_align_center(CZ_AREA4_TITLE_Y_START,CZ_AREA4_TITLE_FONT_HEIGHT,1,CZ_AREA4_TITLE_Y_LIMIT);

	GUI_DispStringAt(CZ_AREA4_TITLE_SRTING, tmpx, tmpy);

}

void func_display_area_5(uint16 index,void *pvoid)
{
	uint8_t num,tmpx,tmpy;
	uint16_t chnum;
	uint32_t freq;
	uint8_t stringtmp[20];
	uint8_t stringtmp1[20];
	
  	fun_print();
	num = 5;
	GUI_SetColor(GUI_WHITE);
	GUI_SetFont(CZ_AREA5_TITLE1_FONT);
	
	tmpx = fun_display_align_center(CZ_AREA5_TITLE_X_START,CZ_AREA5_TITLE1_FONT_WIDTH,num,CZ_AREA5_TITLE_X_LIMIT);
	tmpy = fun_display_align_center(CZ_AREA5_TITLE_Y_START,CZ_AREA5_TITLE1_FONT_HEIGHT,1,42);

	chnum = Get_Comm_param_CH_No();
	sprintf(stringtmp,"CH%003d",chnum);
	GUI_DispStringAt(stringtmp, tmpx, tmpy);

	
	GUI_SetFont(CZ_AREA5_TITLE2_FONT);
//	CH_2_Freq_str(chnum,stringtmp,0);
//	sprintf(stringtmp1,"TX=%s",stringtmp);
	freq = Get_Comm_param_Tx_Freq();
	sprintf(stringtmp1,"TX=%d.%dMHZ",freq/1000,freq%1000);

	num = strlen(stringtmp1);
	tmpx = fun_display_align_center(CZ_AREA5_TITLE_X_START,CZ_AREA5_TITLE2_FONT_WIDTH,num,CZ_AREA5_TITLE_X_LIMIT);
	tmpy = fun_display_align_center(CZ_AREA5_TITLE_Y_START+40,CZ_AREA5_TITLE2_FONT_HEIGHT,1,16);
	GUI_DispStringAt(stringtmp1, tmpx, tmpy);
	
	tmpy += 2;
	tmpy +=CZ_AREA5_TITLE2_FONT_HEIGHT;
	
//	CH_2_Freq_str(chnum,stringtmp,1);
//	sprintf(stringtmp1,"RX=%s",stringtmp);
	freq = Get_Comm_param_Rx_Freq();
	sprintf(stringtmp1,"RX=%d.%dMHZ",freq/1000,freq%1000);

	GUI_DispStringAt(stringtmp1, tmpx, tmpy);

}

void func_chnumber1(uint16 index,void *pvoid)
{
  	fun_print();
#ifdef DIS_WORD	
	
	GUI_DispStringAt("windows_func1",0,32);
#endif

}

void func_chnumber2(uint16 index,void *pvoid)
{
  	fun_print();
#ifdef DIS_WORD	
	
	GUI_DispStringAt("windows_func2",0,32);
#endif

}

void func_chnumber3(uint16 index,void *pvoid)
{
  	fun_print();
#ifdef DIS_WORD	
	GUI_DispStringAt("windows_func3",0,32);
#endif

}

void func_chnumber4(uint16 index,void *pvoid)
{
  	fun_print();
#ifdef DIS_WORD	
	GUI_DispStringAt("windows_func4",0,32);
#endif

}

void func_chnumber5(uint16 index,void *pvoid)
{
  	fun_print();
#ifdef DIS_WORD	
	GUI_DispStringAt("windows_func5",0,32);
#endif

}

void func_chnumber6(uint16 index,void *pvoid)
{
  	fun_print();
#ifdef DIS_WORD	
	GUI_DispStringAt("windows_func6",0,32);
#endif

}
void func_chnumber7(uint16 index,void *pvoid)
{
  	fun_print();
	
	GUI_DispStringAt("windows_func7",0,48);

}

void Func_ChNum(uint8 key, uint8 *BackColor)
{
	uint8 i=0;
	uint8 j=0;
	uint8 	num1;
	uint8	num2;

	uint8 cursor[2]={0,0};
	static uint8 lastkey[3] = {'0','1',0};
	uint8 ChNum_keycont;
	uint8_t *tmpkey=NULL,selectflg;

	fun_print();
//	rt_kprintf("key[%d]\r\n",key+'0');
	tmpkey = get_key_buf();
	ChNum_keycont= get_key_offset();
	#if 1
	
			switch(key)
			{
				case KEY_ACK:
				case KEY_ENTER:
					if(!get_key_oksec())
					{
						strcpy(tmpkey,lastkey);
						set_key_offset(0);
						//ChNum_keycont = 0;
						cursor[0] = tmpkey[0];
						set_key_oksec(1);
						set_option_index_select(SELECT_ITEM_FUNC);
					}
					else
					{						
//						vtr_ch_infopar_set_tft.vtrchannelinfopar.number.property=(uint8)atoi(tmpkey);						
//						if(vtr_ch_infopar_set_tft.vtrchannelinfopar.number.property>0&&vtr_ch_infopar_set_tft.vtrchannelinfopar.number.property<100)
//						{
//						
//						}
//						else
//						{//esc
//							tmpkey[0]=vtr_now_channel/10+0x30;
//							tmpkey[1]=vtr_now_channel%10+0x30;
//							tmpkey[2]=0;
//							strcpy(lastkey,tmpkey);
//							LCD_PutString_New(CHNOICO_X_TITLE, CHNOICO_Y_TITLE, tmpkey, FONT16|ZT_CS, BackColor, TextFont_Color);
//							vtr_ch_infopar_set_tft.vtrchannelinfopar.number.property=(uint8)atoi(tmpkey);						
//							ChNum_keycont = 0;	
//							
//						}

						memcpy(testbuf,tmpkey,10);
						Shortkey_Okesc_Funcreturn();
						return;
					}
					break;
				case KEY_CANNEL:
					strcpy(tmpkey,lastkey);
//					LCD_PutString_New(CHNOICO_X_TITLE, CHNOICO_Y_TITLE, tmpkey, FONT16|ZT_CS, BackColor, TextFont_Color);
//					vtr_ch_infopar_set_tft.vtrchannelinfopar.number.property=(uint8)atoi(tmpkey);						
					ChNum_keycont = 0;		
					set_key_offset(0);
					
					Shortkey_Okesc_Funcreturn();
//					set_key_oksec(0);
//					selectflg = get_option_index_select();
//					if(selectflg!=SELECT_ITEM_NULL)
//					{
//						set_option_index_select(--selectflg);
//					}
//					else{
//						set_current_menu_state(0);
//						}

					return;
					break;
				case KEY_UP:
					tmpkey[ChNum_keycont] += 1;	
					if(tmpkey[ChNum_keycont] < '0')
					{
						tmpkey[ChNum_keycont] = '9';
					}
					else if(tmpkey[ChNum_keycont] > '9')
					{
						tmpkey[ChNum_keycont] = '0';
					}
					cursor[0] = tmpkey[ChNum_keycont];
					break;
				case KEY_DOWN:
					tmpkey[ChNum_keycont] -= 1;	
					if(tmpkey[ChNum_keycont] < '0')
					{
						tmpkey[ChNum_keycont] = '9';
					}
					else if(tmpkey[ChNum_keycont] > '9')
					{
						tmpkey[ChNum_keycont] = '0';
					}
					cursor[0] = tmpkey[ChNum_keycont];		
					break;
				case KEY_RIGHT:
					if(ChNum_keycont == 1)
					{
						ChNum_keycont = 0;
					}
					else //if(keycont == 0)
					{
						ChNum_keycont = 1;
					}
					cursor[0] = tmpkey[ChNum_keycont];
					break;
				case KEY_LEFT:
					if(ChNum_keycont == 0)
					{
						ChNum_keycont = 1;			
					}
					else //if(keycont == 1)
					{
						ChNum_keycont = 0;
					}
					cursor[0] = tmpkey[ChNum_keycont];
					break;					
				default:
							
					break;			
			}
			set_key_offset(ChNum_keycont);
			set_text_mode_by_select_flg(SELECT_ITEM_FUNC);
			Disp_string_at_xy(130,50,tmpkey);
			Disp_text_mode_general();

//	GUI_SetColor(RED,WHITE);//GUI_ExchangeColor();
//	LCD_PutString(CHNOICO_X_TITLE, CHNOICO_Y_TITLE, tmpkey, FONT16, BackColor);
//	//LCD_PutString(32+16*4, 16, str, FONT16, (uint8 *)&gColor.st_WHITE);
//	GUI_SetColor(TextFont_Color,WHITE);

//	LCD_PutString(CHNOICO_X_TITLE + ChNum_keycont *8, CHNOICO_Y_TITLE, cursor, FONT16, (uint8 *)&gColor.st_BLACK);
//	GUI_SetColor(TextFont_Color,BLACK);
	#endif
}


void func_operation(uint8 key,uint8 sel)
{
    MY_NEW_MENU *pmenu;
    WINDOWS_NEW *pwin_temp;  
	uint8_t windows_index = get_windows_index();

  	fun_print();
//	func_chnumber(key+10,NULL);
    pwin_temp=&TopWindows[windows_index];
    pmenu = pwin_temp->pMenu + sel;

	if(pmenu->pmenuico->IcoFunction != NULL)
	{		
		pmenu->pmenuico->IcoFunction(key,pwin_temp->pdot);
	} 	
	
	DisplayWindow(&TopWindows[windows_index]);		   
}

void keyoperation(uint8_t key)
{
//	uint8   key;
//	uint8   i;
	uint8_t KeyFuncIndex;
	uint8_t opt_index,flash;
	static uint32 s_KeyStatus = 0; //0表示未进入连击模式 1表示进入连击模式

	
  //	fun_print();
	flash = get_flash_mode();
	opt_index = get_option_index();
	
	rt_kprintf("start---key[%d]opt_index[%d] Flash[%d]\r\n",key,opt_index,flash);
	
	OLED_disable_refresh_ram();
	if(get_current_menu_state() == FlashMode_func)
	{
		func_operation(key,opt_index);
		shortkey_check_option_index_select_state();
	}
	else
	{
		KeyFuncIndex = get_key_fun_index();
		KeyFuncIndex=KeyTab[KeyFuncIndex].KeyOKState+find_key_state[key]-1;// okstate==1 ,所以查找状态需-1
		KeyFuncPtr=KeyTab[KeyFuncIndex].CurrentOperate;
		set_key_fun_index(KeyFuncIndex);
		(*KeyFuncPtr)();//执行当前按键的操作	
	}

	flash = get_flash_mode();

	if(flash == FlashMode_func)
	{
		func_operation(key,opt_index);
		set_current_menu_state(FlashMode_func);
		flash = 0;
	}
	else
	{					
		if(flash)
		{
			KeyFuncIndex = get_key_fun_index();
	        KeyFuncPtr=KeyTab[KeyFuncIndex].CurrentOperate;
	        (*KeyFuncPtr)();//执行当前按键的操作
	        flash = 0;					
		}
	}

	set_flash_mode(flash);
	
	OLED_enable_refresh_ram();
//	rt_kprintf("end---key[%d]opt_index[%d] Flash[%d]\r\n",key,opt_index,flash);

	
}

/*****************************************************************************
 * 函 数 名  : is_work_mode_CH_No_freq
 * 负 责 人  : MZM
 * 创建日期  : 2016年5月16日
 * 函数功能  : 检查工作模式 是否为频率 pdt集群模式，是返回 true，否返回false
               
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
uint8_t is_work_mode_CH_No_freq()
{
	if((Get_Comm_param_Work_Mode() == WORK_MODE_PDT) && (Get_Comm_param_Work_Mode1() == WORK_MODE_1_TRUNK))
	{
		return TRUE;
	}

	return FALSE;
}

//保存频率编号chnum、pdt集群模式
void kb_save_pdt_trunk_chnum(uint16_t chnum)
{
	kb_save_work_mode(WORK_MODE_PDT,WORK_MODE_1_REPEAT); // pdt 模式，集群模式
	kb_save_ch_no_txrx_freq(chnum,0,0);	//pdt频率编号355

}
//保存频率编号chnum、pdt集群模式
void kb_save_dmr_repeat_chnum(uint16_t chnum,uint32_t txfreq,uint32_t rxfreq)
{
	kb_save_work_mode(WORK_MODE_DMR,WORK_MODE_1_REPEAT); // pdt 模式，集群模式
	kb_save_ch_no_txrx_freq(chnum,txfreq,rxfreq);	//pdt频率编号355

}


