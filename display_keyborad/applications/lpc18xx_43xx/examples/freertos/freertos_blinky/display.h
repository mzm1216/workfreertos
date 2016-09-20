#ifndef _DISPLAY_H
#define _DISPLAY_H

#ifdef  DISPLAY_GLOBALS
#define display_ext
#else
#define display_ext  extern
#endif

#include "board.h"
#include "config.h"
#include "FreeRTOS.h"
#include "task.h"
#include "GUI.h"
#include "GUIConf.h"

extern GUI_CONST_STORAGE GUI_FONT GUI_FontST12;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontHT24;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontHT29;


//-----------------------
//���¶����KeyTab�еõ�
//����KeyTabʱ����������µĶ���
//����˵����ӳ���!
#define MenuTopIndex       0
#define MenuIndex          7
#define MenuFuncIndex     14

//����ʼ��Flash
#define FlashMode_NoAction     	0
//ϵͳϵͳ�����ʼ��
#define FlashMode_AutoInit     	1
//�ֶ���ʼ��
#define FlashMode_ManualInit   	2
//�ػ滭��
#define FlashMode_ReFlash      	3
//���ܲ˵���
#define FlashMode_func      	4
//����
#define FlashMode_vol      		5
display_ext void func_display_area_1_3(uint16 index,void *pvoid);
display_ext void func_display_area_2(uint16 index,void *pvoid);
display_ext void func_display_area_3(uint16 index,void *pvoid);
display_ext void func_display_area_4(uint16 index,void *pvoid);
display_ext void func_display_area_5(uint16 index,void *pvoid);

display_ext void func_chnumber(uint16 index,void *pvoid);
display_ext void func_chnumber1(uint16 index,void *pvoid);
display_ext void func_chnumber2(uint16 index,void *pvoid);
display_ext void func_chnumber3(uint16 index,void *pvoid);
display_ext void func_chnumber4(uint16 index,void *pvoid);
display_ext void func_chnumber5(uint16 index,void *pvoid);
display_ext void func_chnumber6(uint16 index,void *pvoid);
display_ext void func_chnumber7(uint16 index,void *pvoid);
display_ext void Func_ChNum(uint8 key, uint8 *BackColor);

display_ext void DispMenuTop(void);
display_ext void DispMenuTopUp(void);
display_ext void DispMenuTopDown(void);
display_ext void DispMenuTopLeft(void);
display_ext void DispMenuTopRight(void);
display_ext void DispMenuTopOk(void);
display_ext void DispMenuTopEsc(void);

display_ext void DispIcoMenuOk(void);
display_ext void DispIcoMenuEsc(void);
display_ext void DispIcoMenuInit(void);
display_ext void DispIcoMenuUp(void);
display_ext void DispIcoMenuDown(void);
display_ext void DispIcoMenuLeft(void);
display_ext void DispIcoMenuRight(void);

display_ext void DispMenuFuncInit(void);
display_ext void DispMenuFuncOk(void);
display_ext void DispMenuFuncEsc(void);
display_ext void DispMenuFuncUp(void);
display_ext void DispMenuFuncDown(void);
display_ext void DispMenuFuncLeft(void);
display_ext void DispMenuFuncRight(void);
#define fun_print() {rt_kprintf(DEBUG_SYS,"FUN[%s()]\r\n",__FUNCTION__);}


uint8 IcoMenuNum[6]={4,4,4,4,4,4};//������

/*key value 0-6Ϊ-�ϡ�ack���¡����ҡ�ȷ�ϡ�ȡ��
��Ӧ״̬Ϊ:		   3   1    4   5   6   1     1
��״̬���������ʼ�������ڽ��в���ʱ������value
���Ҷ�Ӧ״̬��
*/
uint8_t find_key_state[]={3,1,4,5,6,1,2};
typedef struct{
        uint8 KeyStateIndex; //��ǰ״̬������
        uint8 KeyOKState;//����"ȷ��"��ʱת���״̬������1
        uint8 KeyESCState;//����"����"��ʱת���״̬������2
        uint8 KeyUpState; //����"����"��ʱת���״̬������3
        uint8 KeyDnState; //����"����"��ʱת���״̬������4
        uint8 KeyLState;  //����"����"��ʱת���״̬������5
        uint8 KeyRState;  //����"����"��ʱת���״̬������6
        void (*CurrentOperate)(); //��ǰ״̬Ӧ��ִ�еĹ��ܲ���
} KbdTabStruct;

KbdTabStruct KeyTab[]={
//       |-----------> Index
//       |  ok
//       |  |  esc
//       |  |  |  up
//       |  |  |  |  down   
//       |  |  |  |  |  left
//       |  |  |  |  |  |  right --->���ܺ���
//       |  |  |  |  |  |  |
       { 0, 1, 2, 3, 4, 5, 6,(*DispMenuTop)},// ��������
       { 1, 0, 0, 0, 0, 0, 0,(*DispMenuTopOk)},
       { 2, 0, 0, 0, 0, 0, 0,(*DispMenuTopEsc)},
       { 3, 0, 0, 0, 0, 0, 0,(*DispMenuTopUp)},
       { 4, 0, 0, 0, 0, 0, 0,(*DispMenuTopDown)},
       { 5, 0, 0, 0, 0, 0, 0,(*DispMenuTopLeft)},
       { 6, 0, 0, 0, 0, 0, 0,(*DispMenuTopRight)},

       { 7, 8, 9, 10, 11, 12, 13,(*DispIcoMenuInit)}, //ͼ���˵�
	   { 8, 0, 0, 0, 0, 0, 0,(*DispIcoMenuOk)},
       { 9, 0, 0, 0, 0, 0, 0,(*DispIcoMenuEsc)},
       { 10, 0, 0, 0, 0, 0, 0,(*DispIcoMenuUp)},   //
       { 11, 0, 0, 0, 0, 0, 0,(*DispIcoMenuDown)}, //
       { 12, 0, 0, 0, 0, 0, 0,(*DispIcoMenuLeft)}, //
       { 13, 0, 0, 0, 0, 0, 0,(*DispIcoMenuRight)},//       

       { 14, 15, 16, 17, 18, 19, 20,(*DispMenuFuncInit)}, //����˵�
	   { 15, 0, 0, 0, 0, 0, 0,(*DispMenuFuncOk)},
       { 16, 0, 0, 0, 0, 0, 0,(*DispMenuFuncEsc)},
       { 17, 0, 0, 0, 0, 0, 0,(*DispMenuFuncUp)},   //
       { 18, 0, 0, 0, 0, 0, 0,(*DispMenuFuncDown)}, //
       { 19, 0, 0, 0, 0, 0, 0,(*DispMenuFuncLeft)}, //
       { 20, 0, 0, 0, 0, 0, 0,(*DispMenuFuncRight)},//    

};

/* ����ͼ��˵����ݽṹ */
typedef __packed struct
{  uint32 x;			//  ͼ��˵�λ��(���Ͻǵ�x����)
   uint32 y;			//  ͼ��˵�λ��(���Ͻǵ�y����) 
   uint8 *title;        //  ��ر�����ʾ (42*13)
   void *title_font;	// ���� ����
   uint8 *ptext;
   void *picodat;		//  ͼ������ָ��(48*48 *2)
  // uint8_t titlesize;	
  // uint8_t titlestep;
   void (*IcoFunction)(uint16 index,void *pvoid);	    // ��Ӧ�ķ������
} ST_MENUITEM;

/* ����˵�������ݽṹ */
typedef __packed struct ST_MENU
{   
    uint8    CurrentIndex;      	// ��ǰ����˵����(0--5)��
	ST_MENUITEM *pmenuico;    	    // һ���˵����������
	
	struct ST_MENU *upper_menu;     // ��һ��   
	struct ST_MENU *down_menu;      // ��һ��
	
}MY_NEW_MENU;
typedef struct 
{
    uint16 xpos;
    uint16 ypos;
    void *pdot;                       // ָ�򱳵�ָ��
    MY_NEW_MENU *pMenu;                // ָ����ָ��Ĳ˵����
}WINDOWS_NEW;

/**********************************λͼ���***********************************************/
extern GUI_CONST_STORAGE GUI_BITMAP bmnet_white;	//��ͼ��----����
extern GUI_CONST_STORAGE GUI_BITMAP bmmsc_white;	//��ͼ��----��������
extern GUI_CONST_STORAGE GUI_BITMAP bmdb_white;		//��ͼ��----���ݿ�

extern GUI_CONST_STORAGE GUI_BITMAP bmnet_black;	//��ͼ��---����
extern GUI_CONST_STORAGE GUI_BITMAP bmmsc_black;	//��ͼ��---��������
extern GUI_CONST_STORAGE GUI_BITMAP bmdb_black;		//��ͼ��----���ݿ�

extern GUI_CONST_STORAGE GUI_BITMAP bmheartstep1;	//���� //
extern GUI_CONST_STORAGE GUI_BITMAP bmheartstep2;	//����
extern GUI_CONST_STORAGE GUI_BITMAP bmheartstep3;
extern GUI_CONST_STORAGE GUI_BITMAP bmheartstep4;
extern GUI_CONST_STORAGE GUI_BITMAP bmheartstep5;
extern GUI_CONST_STORAGE GUI_BITMAP bmheartstep6;
extern GUI_CONST_STORAGE GUI_BITMAP bmheartstep7;
extern GUI_CONST_STORAGE GUI_BITMAP bmheartstep8;

extern GUI_CONST_STORAGE GUI_BITMAP bmheartstep11;
extern GUI_CONST_STORAGE GUI_BITMAP bmheartstep12;
extern GUI_CONST_STORAGE GUI_BITMAP bmheartstep13;
extern GUI_CONST_STORAGE GUI_BITMAP bmheartstep14;
extern GUI_CONST_STORAGE GUI_BITMAP bmheartstep15;
extern GUI_CONST_STORAGE GUI_BITMAP bmheartstep16;
extern GUI_CONST_STORAGE GUI_BITMAP bmheartstep17;
extern GUI_CONST_STORAGE GUI_BITMAP bmheartstep18;

extern GUI_CONST_STORAGE GUI_BITMAP bmdown2;
extern GUI_CONST_STORAGE GUI_BITMAP bmup2;
extern GUI_CONST_STORAGE GUI_BITMAP bmmcx;
extern GUI_CONST_STORAGE GUI_BITMAP bmmcx1;
extern GUI_CONST_STORAGE GUI_BITMAP bmbmp1;
extern GUI_CONST_STORAGE GUI_BITMAP bmbmp5;
extern GUI_CONST_STORAGE GUI_BITMAP bmbmp3e;
extern GUI_CONST_STORAGE GUI_BITMAP bmcolortest;

//extern const unsigned char _acmcx1[6784UL + 1];



#define HEART_ICON_NUM		8			//���� ͼ����� 
#define NET_ICON_NUM		2			//���� ͼ���ʽ
#define MSC_ICON_NUM		2			//msc  ͼ�����
#define DB_ICON_NUM			2			//���ݿ� ͼ�����
typedef struct 
{
	const GUI_BITMAP * heartstep[HEART_ICON_NUM];
//	const GUI_BITMAP * heartstep1[HEART_ICON_NUM];
	const GUI_BITMAP * net_pic[NET_ICON_NUM];
	const GUI_BITMAP * msc_pic[MSC_ICON_NUM];
	const GUI_BITMAP * db_pic[DB_ICON_NUM];



}KB_ICON_T;

KB_ICON_T kb_pic_struct =
{
//	&bmheartstep11,&bmheartstep12,&bmheartstep13,&bmheartstep14,&bmheartstep15,&bmheartstep16,&bmheartstep17,&bmheartstep18,
		&bmheartstep1,&bmheartstep2,&bmheartstep3,&bmheartstep4,&bmheartstep5,&bmheartstep6,&bmheartstep7,&bmheartstep8,
		&bmnet_black,&bmnet_white,
		&bmmsc_black,&bmmsc_white,
		&bmdb_black,&bmdb_white,
};


/************************** ����ģʽ  DMR PDt mpt normal ******************************/
#define WORK_MODE_DMR			0
#define WORK_MODE_PDT			1
#define WORK_MODE_MPT			2
#define WORK_MODE_NORMAL		3

uint8_t * work_mode_title[]=
{
	"DMR",
	"PDT",
	"MPT",
	"NORMAL",
};

#define WORK_MODE_1_REPEAT		0
uint8_t *DMR_work_mode_1_title[] =
{
	"Repeat",
	"OK",
};

#define WORK_MODE_1_TRUNK 		0 //��Ⱥ
uint8_t *PDT_work_mode_1_title[] =
{
	"Trunk",
	"OK",
};

#define WORK_MODE_1_MPT			0
uint8_t *MPT_work_mode_1_title[] =
{
	"Mpt",
	"OK",
};
uint8_t *Normal_work_mode_1_title[] =
{
	"Normal",
	"OK",
};

uint8_t *Ctr_CH_State_title[] =
{
	"Cch  si:",
	"Idle ",
	"Busy ",
	"OK   ",

};

typedef struct
{
	void *work_mode1_title[4];
}DISPLAY_WORK_MODE1_TITLE_T;

DISPLAY_WORK_MODE1_TITLE_T work_mode1_title =
{
	DMR_work_mode_1_title,
	PDT_work_mode_1_title,
	MPT_work_mode_1_title,
	Normal_work_mode_1_title,
};

//typedef struct
//{
//	DISPLAY_TITLE_T work_mode;
//}DISPLAY_WORK_MODE_TITLE_T;

//DISPLAY_WORK_MODE_TITLE_T work_mode_title =
//{
//	work_mode_title,DMR_work_mode_1_title,
//	work_mode_title,PDT_work_mode_1_title,
//	work_mode_title,MPT_work_mode_1_title,
//	work_mode_title,Normal_work_mode_1_title,

//};

#define CZ_FIRST_PIC_X_OFFSET		2
#define CZ_FIRST_PIC_Y_OFFSET		2

#define CZ_SECOND_PIC_X_OFFSET		22
#define CZ_SECOND_PIC_Y_OFFSET		2

#define CZ_THREE_PIC_X_OFFSET		42
#define CZ_THREE_PIC_Y_OFFSET		2

#define CZ_FOUR_PIC_X_OFFSET		62
#define CZ_FOUR_PIC_Y_OFFSET		2

#define CZ_AREA2_TITLE_X_LIMIT		80
#define CZ_AREA2_TITLE_Y_LIMIT		40

#define CZ_AREA2_TITLE_X_START		0
#define CZ_AREA2_TITLE_Y_START		24

#define CZ_AREA2_TITLE1_FONT			&GUI_FontHT24
#define CZ_AREA2_TITLE1_FONT_WIDTH			12
#define CZ_AREA2_TITLE1_FONT_HEIGHT			24

#define CZ_AREA2_TITLE2_FONT			GUI_FONT_8X16_ASCII
#define CZ_AREA2_TITLE2_FONT_WIDTH			8
#define CZ_AREA2_TITLE2_FONT_HEIGHT			16


//#define CZ_AREA2_TITLE1_SRTING		"PDT"
//#define CZ_AREA2_TITLE_FONT			&GUI_FontHT24

//#define CZ_AREA2_TITLE2_SRTING		"Trunk"
//#define CZ_AREA2_TITLE_FONT_1			GUI_FONT_8X16_ASCII

#define CZ_AREA3_TITLE_X_LIMIT		80
#define CZ_AREA3_TITLE_Y_LIMIT		20

#define CZ_AREA3_TITLE1_X_START		88+88+1
#define CZ_AREA3_TITLE1_Y_START		0
#define CZ_AREA3_TITLE2_X_START		88+88+1
#define CZ_AREA3_TITLE2_Y_START		21

//#define CZ_AREA4_TITLE_FONT			&GUI_FontST12
//#define CZ_AREA4_TITLE_FONT_WIDTH			6
//#define CZ_AREA4_TITLE_FONT_HEIGHT			12

#define CZ_AREA3_TITLE1_FONT			GUI_FONT_6X8_ASCII
#define CZ_AREA3_TITLE1_FONT_WIDTH			6
#define CZ_AREA3_TITLE1_FONT_HEIGHT			8

#define CZ_AREA3_TITLE2_FONT			GUI_FONT_6X8_ASCII
#define CZ_AREA3_TITLE2_FONT_WIDTH			6
#define CZ_AREA3_TITLE2_FONT_HEIGHT			8


#define CZ_AREA3_X_LIMIT			80
#define CZ_AREA3_Y_LIMIT			40

#define CZ_AREA3_PIC_X_BASE			80+88+4+4+1
#define CZ_AREA3_PIC_Y_BASE			0
#define CZ_AREA3_PIC_SIZE			16
#define CZ_AREA3_Y_OFFSET			

#define CZ_AREA3_FIRST_PIC_X		CZ_AREA3_PIC_X_BASE+4
#define CZ_AREA3_FIRST_PIC_Y		CZ_AREA3_PIC_Y_BASE+12

#define CZ_AREA3_SECOND_PIC_X		CZ_AREA3_FIRST_PIC_X+CZ_AREA3_PIC_SIZE
#define CZ_AREA3_SECOND_PIC_Y		CZ_AREA3_FIRST_PIC_Y

#define CZ_AREA3_THREE_PIC_X		CZ_AREA3_SECOND_PIC_X+CZ_AREA3_PIC_SIZE+8
#define CZ_AREA3_THREE_PIC_Y		CZ_AREA3_FIRST_PIC_Y

#define CZ_AREA3_FOUR_PIC_X			CZ_AREA3_THREE_PIC_X+CZ_AREA3_PIC_SIZE
#define CZ_AREA3_FOUR_PIC_Y			CZ_AREA3_FIRST_PIC_Y

#define CZ_AREA4_TITLE_X_LIMIT		80
#define CZ_AREA4_TITLE_Y_LIMIT		20

#define CZ_AREA4_TITLE_X_START		CZ_AREA3_PIC_X_BASE
#define CZ_AREA4_TITLE_Y_START		44

#define CZ_AREA4_TITLE_SRTING		"Disconnect"
#define CZ_AREA4_TITLE_FONT			&GUI_FontST12
#define CZ_AREA4_TITLE_FONT_WIDTH			6
#define CZ_AREA4_TITLE_FONT_HEIGHT			12


#define CZ_AREA5_TITLE_X_LIMIT		88
#define CZ_AREA5_TITLE_Y_LIMIT		64

#define CZ_AREA5_TITLE_X_START		84
#define CZ_AREA5_TITLE_Y_START		0

#define CZ_AREA5_TITLE1_FONT			&GUI_FontHT29
#define CZ_AREA5_TITLE1_FONT_WIDTH			16
#define CZ_AREA5_TITLE1_FONT_HEIGHT			29

#define CZ_AREA5_TITLE2_FONT			GUI_FONT_6X8
#define CZ_AREA5_TITLE2_FONT_WIDTH			6
#define CZ_AREA5_TITLE2_FONT_HEIGHT			8

ST_MENUITEM	 win1_test1[]=
{
//	CZ_FIRST_PIC_X_OFFSET	,CZ_FIRST_PIC_Y_OFFSET	,NULL,NULL,NULL,NULL,func_display_area_1_3,
		CZ_SECOND_PIC_X_OFFSET	,CZ_SECOND_PIC_Y_OFFSET ,NULL,NULL,NULL,NULL,func_display_area_5,
	CZ_SECOND_PIC_X_OFFSET	,CZ_SECOND_PIC_Y_OFFSET ,NULL,NULL,NULL,NULL,func_display_area_5,
	CZ_SECOND_PIC_X_OFFSET	,CZ_SECOND_PIC_Y_OFFSET ,NULL,NULL,NULL,NULL,func_display_area_5,
	CZ_SECOND_PIC_X_OFFSET	,CZ_SECOND_PIC_Y_OFFSET ,NULL,NULL,NULL,NULL,func_display_area_5,
		
//		0	,0	,NULL,NULL,NULL,NULL,func_display_area_3,
//		CZ_SECOND_PIC_X_OFFSET	,CZ_SECOND_PIC_Y_OFFSET ,NULL,NULL,NULL,NULL,func_display_area_4,
//		CZ_SECOND_PIC_X_OFFSET	,CZ_SECOND_PIC_Y_OFFSET ,NULL,NULL,NULL,NULL,func_display_area_5,
//	CZ_THREE_PIC_X_OFFSET	,CZ_THREE_PIC_Y_OFFSET	,NULL,NULL,NULL,(void *)&bmmsc1,func_chnumber1,
//	CZ_FOUR_PIC_X_OFFSET	,CZ_FOUR_PIC_Y_OFFSET	,NULL,NULL,NULL,(void *)&bmdb1,func_chnumber1,
//		
//	120, 0,  "1_2", NULL, NULL, func_chnumber7,
//	CHNUM_XCOORDINATE,CHNUM_YCOORDINATE,NULL,NULL,NULL,func_top0,
//	PAVOLTAG_XCOORDINATE_TITLE, PAVOLTAG_YCOORDINATE_TITLE,  "�����ѹ", " V ", NULL, func_top1,
//	PATEMP_XCOORDINATE_TITLE,PATEMP_YCOORDINATE_TITLE,	"�����¶�", " �� ", NULL, func_top2,
//	RSSI_XCOORDINATE_TITLE,RSSI_YCOORDINATE_TITLE,	"���ճ�ǿ", "dBm", NULL, func_top3,
//	FWDPOWER_XCOORDINATE_TITLE, FWDPOWER_YCOORDINATE_TITLE,  "ǰ����", " W ", NULL, func_top4, 
//	REFPOWER_XCOORDINATE_TITLE, REFPOWER_YCOORDINATE_TITLE,  "������", " W ", NULL, func_top5, 
//	VSWR_XCOORDINATE_TITLE,	VSWR_YCOORDINATE_TITLE,	"פ �� ��", NULL, NULL, func_top6,	
};

#define INFOPAR_LXSIZE	20
#define INFOPAR_RXSIZE	219

ST_MENUITEM	 win2_test2[]=
{
//	0	,0	,NULL,NULL,NULL,NULL,func_display_area_3,
		CZ_SECOND_PIC_X_OFFSET	,CZ_SECOND_PIC_Y_OFFSET ,NULL,NULL,NULL,NULL,func_display_area_2,
	CZ_SECOND_PIC_X_OFFSET	,CZ_SECOND_PIC_Y_OFFSET ,NULL,NULL,NULL,NULL,func_display_area_2,
	CZ_SECOND_PIC_X_OFFSET	,CZ_SECOND_PIC_Y_OFFSET ,NULL,NULL,NULL,NULL,func_display_area_2,
	CZ_SECOND_PIC_X_OFFSET	,CZ_SECOND_PIC_Y_OFFSET ,NULL,NULL,NULL,NULL,func_display_area_2,
//		CZ_SECOND_PIC_X_OFFSET	,CZ_SECOND_PIC_Y_OFFSET ,NULL,NULL,NULL,NULL,func_display_area_4,
//		CZ_SECOND_PIC_X_OFFSET	,CZ_SECOND_PIC_Y_OFFSET ,NULL,NULL,NULL,NULL,func_display_area_5,
//		
//	INFOPAR_RXSIZE, PAVOLTAG_YCOORDINATE_TITLE,  "�ŵ�����", NULL, NULL, Func_ChKey, 
//	INFOPAR_LXSIZE, PATEMP_YCOORDINATE_TITLE,  "����Ƶ��", NULL, NULL, Func_RcFreq,
//	INFOPAR_RXSIZE, PATEMP_YCOORDINATE_TITLE,  "��������", NULL, NULL, Func_RcSqFreq,
//	INFOPAR_LXSIZE, RSSI_YCOORDINATE_TITLE,  "����Ƶ��", NULL, NULL, Func_TransFreq,
//	INFOPAR_RXSIZE, RSSI_YCOORDINATE_TITLE,  "��������", NULL, NULL, Func_TransSqFreq,
//	INFOPAR_LXSIZE, FWDPOWER_YCOORDINATE_TITLE,  "��Ƶ����ģʽ", NULL, NULL, Func_RfWorkMode,
//	INFOPAR_RXSIZE, FWDPOWER_YCOORDINATE_TITLE,  "�������", NULL, NULL, Func_OutPower,
//	INFOPAR_LXSIZE, REFPOWER_YCOORDINATE_TITLE,  "�ŵ�����ģʽ", NULL, NULL, Func_ChWorkMode,
//	INFOPAR_RXSIZE, VSWR_YCOORDINATE_TITLE,  "��    ��", NULL, NULL, Func_CHinfoset,	
};

ST_MENUITEM	 win2_test3[]=
{
//	0	,0	,NULL,NULL,NULL,NULL,func_display_area_3,
//		CZ_SECOND_PIC_X_OFFSET	,CZ_SECOND_PIC_Y_OFFSET ,NULL,NULL,NULL,NULL,func_display_area_2,
		CZ_SECOND_PIC_X_OFFSET	,CZ_SECOND_PIC_Y_OFFSET ,NULL,NULL,NULL,NULL,func_display_area_4,
	CZ_SECOND_PIC_X_OFFSET	,CZ_SECOND_PIC_Y_OFFSET ,NULL,NULL,NULL,NULL,func_display_area_4,
	CZ_SECOND_PIC_X_OFFSET	,CZ_SECOND_PIC_Y_OFFSET ,NULL,NULL,NULL,NULL,func_display_area_4,
	CZ_SECOND_PIC_X_OFFSET	,CZ_SECOND_PIC_Y_OFFSET ,NULL,NULL,NULL,NULL,func_display_area_4,
//		CZ_SECOND_PIC_X_OFFSET	,CZ_SECOND_PIC_Y_OFFSET ,NULL,NULL,NULL,NULL,func_display_area_5,

//	INFOPAR_RXSIZE,  PAVOLTAG_YCOORDINATE_TITLE,  "����״̬", NULL, NULL, func_alarmstate, 
//	INFOPAR_LXSIZE, PATEMP_YCOORDINATE_TITLE,  "�����¶�", NULL, NULL, func_patemp,
//	INFOPAR_RXSIZE,  PATEMP_YCOORDINATE_TITLE,  "ָʾ״̬", NULL, NULL, func_ledstate,
//	INFOPAR_LXSIZE, RSSI_YCOORDINATE_TITLE,  "ǰ����", NULL, NULL, func_fwd,
//	INFOPAR_RXSIZE,  RSSI_YCOORDINATE_TITLE,  "����״̬", NULL, NULL, func_fan,
//	INFOPAR_LXSIZE, FWDPOWER_YCOORDINATE_TITLE,  "������", NULL, NULL, func_ref,
//	INFOPAR_RXSIZE,  FWDPOWER_YCOORDINATE_TITLE,  "פ �� ��", NULL, NULL, func_vswr,
//	INFOPAR_LXSIZE, REFPOWER_YCOORDINATE_TITLE,  "���ճ�ǿ", NULL, NULL, func_rssi,	
//	INFOPAR_RXSIZE, REFPOWER_YCOORDINATE_TITLE,  "PTT ״̬", NULL, NULL, func_pttstate,	
};


ST_MENUITEM	 win2_test4[]=
{
//	0	,0	,NULL,NULL,NULL,NULL,func_display_area_3,
//		CZ_SECOND_PIC_X_OFFSET	,CZ_SECOND_PIC_Y_OFFSET ,NULL,NULL,NULL,NULL,func_display_area_2,
//		CZ_SECOND_PIC_X_OFFSET	,CZ_SECOND_PIC_Y_OFFSET ,NULL,NULL,NULL,NULL,func_display_area_4,
		CZ_SECOND_PIC_X_OFFSET	,CZ_SECOND_PIC_Y_OFFSET ,NULL,NULL,NULL,NULL,func_display_area_5,
	CZ_SECOND_PIC_X_OFFSET	,CZ_SECOND_PIC_Y_OFFSET ,NULL,NULL,NULL,NULL,func_display_area_5,
	CZ_SECOND_PIC_X_OFFSET	,CZ_SECOND_PIC_Y_OFFSET ,NULL,NULL,NULL,NULL,func_display_area_5,
	CZ_SECOND_PIC_X_OFFSET	,CZ_SECOND_PIC_Y_OFFSET ,NULL,NULL,NULL,NULL,func_display_area_5,
//	INFOPAR_RXSIZE,  PAVOLTAG_YCOORDINATE_TITLE,  "��˷�ѹ��", NULL, NULL, Func_MicVC, 
//	INFOPAR_LXSIZE, PATEMP_YCOORDINATE_TITLE,  "�������", NULL, NULL, Func_SqCtrlStatus,
//	INFOPAR_RXSIZE,  PATEMP_YCOORDINATE_TITLE,  "������ʾ��", NULL, NULL, Func_AlarmToneState,
//	INFOPAR_LXSIZE, RSSI_YCOORDINATE_TITLE,  "��ʾ����", NULL, NULL, Func_TftLight,
//	INFOPAR_RXSIZE,  RSSI_YCOORDINATE_TITLE,  "������ʾ��", NULL, NULL, Func_KeyToneState,
//	INFOPAR_LXSIZE, FWDPOWER_YCOORDINATE_TITLE,  "�ָ���������", NULL, NULL, Func_ManuDefaut,
//	INFOPAR_RXSIZE,  FWDPOWER_YCOORDINATE_TITLE,  "�̼��汾��", NULL, NULL, Func_DisVer,
//	INFOPAR_LXSIZE, REFPOWER_YCOORDINATE_TITLE,  "����ȼ�", NULL, NULL, Func_SqClass,
//	INFOPAR_LXSIZE, VSWR_YCOORDINATE_TITLE,  "��������", NULL, NULL, Func_Speakervol,	
};

ST_MENUITEM	 win2_test5[]=
{
	
	CZ_SECOND_PIC_X_OFFSET	,CZ_SECOND_PIC_Y_OFFSET ,NULL,NULL,NULL,NULL,func_display_area_4,
	CZ_SECOND_PIC_X_OFFSET	,CZ_SECOND_PIC_Y_OFFSET ,NULL,NULL,NULL,NULL,func_display_area_2,
//	0	,0	,NULL,NULL,NULL,NULL,func_display_area_3,
//		120, 0,  "5_2", NULL,NULL, NULL, func_chnumber7,
//	INFOPAR_LXSIZE, PATEMP_YCOORDINATE_TITLE,  "��̬פ������", NULL, NULL, Func_DgdisState, 
};

ST_MENUITEM	 win2_test6[]=
{
//	0	,0	,NULL,NULL,NULL,NULL,func_display_area_3,
//	CZ_SECOND_PIC_X_OFFSET	,CZ_SECOND_PIC_Y_OFFSET ,NULL,NULL,NULL,NULL,func_display_area_2,
	CZ_SECOND_PIC_X_OFFSET	,CZ_SECOND_PIC_Y_OFFSET ,NULL,NULL,NULL,NULL,func_display_area_4,
	CZ_SECOND_PIC_X_OFFSET	,CZ_SECOND_PIC_Y_OFFSET ,NULL,NULL,NULL,NULL,func_display_area_4,
	CZ_SECOND_PIC_X_OFFSET	,CZ_SECOND_PIC_Y_OFFSET ,NULL,NULL,NULL,NULL,func_display_area_4,
	CZ_SECOND_PIC_X_OFFSET	,CZ_SECOND_PIC_Y_OFFSET ,NULL,NULL,NULL,NULL,func_display_area_4,
//	CZ_SECOND_PIC_X_OFFSET	,CZ_SECOND_PIC_Y_OFFSET ,NULL,NULL,NULL,NULL,func_display_area_5,
//	INFOPAR_RXSIZE,  PAVOLTAG_YCOORDINATE_TITLE,NULL, NULL, NULL, NULL, 
//	INFOPAR_LXSIZE, PATEMP_YCOORDINATE_TITLE,  NULL, NULL, NULL, NULL,
//	INFOPAR_RXSIZE,  PATEMP_YCOORDINATE_TITLE,  NULL, NULL, NULL, NULL, 
//	INFOPAR_LXSIZE, RSSI_YCOORDINATE_TITLE,  NULL, NULL, NULL, NULL,
//	INFOPAR_RXSIZE,  RSSI_YCOORDINATE_TITLE,  NULL, NULL, NULL, NULL, 
//	INFOPAR_LXSIZE, FWDPOWER_YCOORDINATE_TITLE, NULL, NULL, NULL, NULL,
//	INFOPAR_RXSIZE,  FWDPOWER_YCOORDINATE_TITLE,NULL, NULL, NULL, NULL, 
//	INFOPAR_LXSIZE, REFPOWER_YCOORDINATE_TITLE, NULL, NULL, NULL, NULL,
//	INFOPAR_RXSIZE, VSWR_YCOORDINATE_TITLE,  NULL, NULL, NULL, NULL, 	
};

MY_NEW_MENU WIN_MENU_1[] =
{
    0 ,	&win1_test1[0], NULL,           NULL  ,
	1 , &win1_test1[1], &WIN_MENU_1[0],  &WIN_MENU_1[2]  ,
	2 , &win1_test1[2], &WIN_MENU_1[1],  &WIN_MENU_1[3]  ,
	3 , &win1_test1[3], &WIN_MENU_1[2],  &WIN_MENU_1[4]  ,
	4 , &win1_test1[4], &WIN_MENU_1[3],  NULL  ,
//    1 , &win1_test1[1], &WIN_MENU_1[0],  &WIN_MENU_1[2]  ,
//    2 , &win1_test1[2], &WIN_MENU_1[1],  &WIN_MENU_1[3]  ,
//    3 , &win1_test1[3], &WIN_MENU_1[2],  &WIN_MENU_1[4]  ,
//    4 , &win1_test1[4], &WIN_MENU_1[3],  &WIN_MENU_1[5]  ,
//    5 , &win1_test1[5], &WIN_MENU_1[4],  &WIN_MENU_1[6]  ,
//    6 , &win1_test1[6], &WIN_MENU_1[5],  &WIN_MENU_1[7]  ,
//    7 , &win1_test1[7], &WIN_MENU_1[6],  NULL,
};



MY_NEW_MENU WIN_MENU_2[] =
{
    0 ,	&win2_test2[0], NULL,            &WIN_MENU_2[1]  ,
	1 , &win2_test2[1], &WIN_MENU_2[0],  &WIN_MENU_2[2]  ,
	2 , &win2_test2[2], &WIN_MENU_2[1],	 &WIN_MENU_2[3]  ,
	3 , &win2_test2[3], &WIN_MENU_2[2],  NULL  ,

//    0 ,	&win2_test2[0],  	NULL,            &WIN_MENU_2[1],
//    1 ,	&win2_test2[1],  	&WIN_MENU_2[0],  &WIN_MENU_2[2],
//    2 ,	&win2_test2[2],  	&WIN_MENU_2[1],  &WIN_MENU_2[3],
//    3 ,	&win2_test2[3],  	&WIN_MENU_2[2],  &WIN_MENU_2[4],
//    4 ,	&win2_test2[4],  	&WIN_MENU_2[3],  &WIN_MENU_2[5],
//    5 ,	&win2_test2[5],  	&WIN_MENU_2[4],  &WIN_MENU_2[6],
//    6 ,	&win2_test2[6],  	&WIN_MENU_2[5],  &WIN_MENU_2[7],
//    7 ,	&win2_test2[7],  	&WIN_MENU_2[6],  &WIN_MENU_2[8],
//    8 ,	&win2_test2[8],  	&WIN_MENU_2[7],  &WIN_MENU_2[9],
//    9 ,	&win2_test2[9],  	&WIN_MENU_2[8],  NULL,
};

MY_NEW_MENU WIN_MENU_3[] =
{
  0 ,	&win2_test3[0], NULL,            &WIN_MENU_3[1]  ,
	1 , &win2_test3[1], &WIN_MENU_3[0],  &WIN_MENU_3[2]  ,
	2 , &win2_test3[2], &WIN_MENU_3[1],	 &WIN_MENU_3[3]  ,
	3 , &win2_test3[3], &WIN_MENU_3[2],  NULL  ,
//    0 ,	&win2_test3[0],  	NULL,            &WIN_MENU_3[1],
//    1 ,	&win2_test3[1],  	&WIN_MENU_3[0],  &WIN_MENU_3[2],
//    2 ,	&win2_test3[2],  	&WIN_MENU_3[1],  &WIN_MENU_3[3],
//    3 ,	&win2_test3[3],  	&WIN_MENU_3[2],  &WIN_MENU_3[4],
//    4 ,	&win2_test3[4],  	&WIN_MENU_3[3],  &WIN_MENU_3[5],
//    5 ,	&win2_test3[5],  	&WIN_MENU_3[4],  &WIN_MENU_3[6],
//    6 ,	&win2_test3[6],  	&WIN_MENU_3[5],  &WIN_MENU_3[7],
//    7 ,	&win2_test3[7],  	&WIN_MENU_3[6],  &WIN_MENU_3[8],
//    8 ,	&win2_test3[8],  	&WIN_MENU_3[7],  &WIN_MENU_3[9],
//    9 ,	&win2_test3[9],  	&WIN_MENU_3[8],  NULL,
};



MY_NEW_MENU WIN_MENU_4[] =
{
	0 ,    &win2_test4[0], NULL,			&WIN_MENU_4[1]	,
	   1 , &win2_test4[1], &WIN_MENU_4[0],	&WIN_MENU_4[2]	,
	   2 , &win2_test4[2], &WIN_MENU_4[1],	&WIN_MENU_4[3]	,
	   3 , &win2_test4[3], &WIN_MENU_4[2],	NULL  ,

//    0 ,	&win2_test4[0],  	NULL,            &WIN_MENU_4[1],
//    1 ,	&win2_test4[1],  	&WIN_MENU_4[0],  &WIN_MENU_4[2],
//    2 ,	&win2_test4[2],  	&WIN_MENU_4[1],  &WIN_MENU_4[3],
//    3 ,	&win2_test4[3],  	&WIN_MENU_4[2],  &WIN_MENU_4[4],
//    4 ,	&win2_test4[4],  	&WIN_MENU_4[3],  &WIN_MENU_4[5],
//    5 ,	&win2_test4[5],  	&WIN_MENU_4[4],  &WIN_MENU_4[6],
//    6 ,	&win2_test4[6],  	&WIN_MENU_4[5],  &WIN_MENU_4[7],
//    7 ,	&win2_test4[7],  	&WIN_MENU_4[6],  &WIN_MENU_4[8],
//    8 ,	&win2_test4[8],  	&WIN_MENU_4[7],  &WIN_MENU_4[9],
//    9 ,	&win2_test4[9],  	&WIN_MENU_4[8],  NULL,
};

MY_NEW_MENU WIN_MENU_5[] =
{
    0 ,	&win2_test5[0], NULL,            &WIN_MENU_5[1]  ,
    1 , &win2_test5[1], &WIN_MENU_5[0],  NULL            ,
};



MY_NEW_MENU WIN_MENU_6[] =
{
	0 ,    &win2_test6[0], NULL,			&WIN_MENU_6[1]	,
	1 , &win2_test6[1], &WIN_MENU_6[0],	&WIN_MENU_6[2]	,
	2 , &win2_test6[2], &WIN_MENU_6[1],	&WIN_MENU_6[3]	,
	3 , &win2_test6[3], &WIN_MENU_6[2],	NULL  ,

//    0 ,	&win2_test6[0],  	NULL,            &WIN_MENU_6[1],
//    1 ,	&win2_test6[1],  	&WIN_MENU_6[0],  &WIN_MENU_6[2],
//    2 ,	&win2_test6[2],  	&WIN_MENU_6[1],  &WIN_MENU_6[3],
//    3 ,	&win2_test6[3],  	&WIN_MENU_6[2],  &WIN_MENU_6[4],
//    4 ,	&win2_test6[4],  	&WIN_MENU_6[3],  &WIN_MENU_6[5],
//    5 ,	&win2_test6[5],  	&WIN_MENU_6[4],  &WIN_MENU_6[6],
//    6 ,	&win2_test6[6],  	&WIN_MENU_6[5],  &WIN_MENU_6[7],
//    7 ,	&win2_test6[7],  	&WIN_MENU_6[6],  &WIN_MENU_6[8],
//    8 ,	&win2_test6[8],  	&WIN_MENU_6[7],  &WIN_MENU_6[9],
//    9 ,	&win2_test6[9],  	&WIN_MENU_6[8],  NULL,
};

extern GUI_CONST_STORAGE GUI_BITMAP bmcz2;
extern GUI_CONST_STORAGE GUI_BITMAP bmback_pic;
extern GUI_CONST_STORAGE GUI_BITMAP bmcz4;
extern GUI_CONST_STORAGE GUI_BITMAP bmbjs;
//extern GUI_CONST_STORAGE GUI_BITMAP bmczp;
//extern GUI_CONST_STORAGE GUI_BITMAP bm48;
//extern GUI_CONST_STORAGE GUI_BITMAP bmwoxx;
//extern GUI_CONST_STORAGE GUI_BITMAP bmxxgd;


WINDOWS_NEW TopWindows[6]=
{
    {0, 0,  	(void *)&bmbmp1,    	WIN_MENU_1},
    {0, 0,  	(void *)&bmbmp5,    	WIN_MENU_2},
    {0, 0,  	(void *)&bmcolortest,    	WIN_MENU_3},
    {0, 0, 	(void *)&bmbmp3e,    	WIN_MENU_4},
    {0, 0, 	(void *)&bmbmp5,    	WIN_MENU_5},
    {0, 0,  	(void *)&bmbmp3e,    	WIN_MENU_6},

};
#define WINDOWS_NUM 	(sizeof(TopWindows)/sizeof(WINDOWS_NEW))
// ��ʾ�����ṹ��
typedef struct
{
	uint8_t windows_index;			//���ں�		
	uint8_t menu_index;				//�˵���
	uint8_t option_index:6;			//ѡ���
	uint8_t option_index_select:2;			//ѡ��
	uint8_t item_max_num;			//ѡ������
	
	uint8_t flash_mode;				//��ת�����Ӳ˵� ˢ�±�־  >1ˢ��
	uint8_t current_menu_state;				//��ǰ�˵�״̬ 0�˵�������1���ܲ���
	
	uint8_t fun_key_offset;		//ѡ��ƫ��λ��
	uint8_t key_fun_index;			//�������������� 7 menu,14 func
	uint8_t fun_key_str[10];		//ѡ������
	
	uint8_t fun_key_okesc;		//ȷ�Ϻ�ȡ��������־
}DISPLAY_OPERATE_T;

display_ext void (*KeyFuncPtr)(); //��������ָ��

#define SELECT_ITEM_NULL	0		//δѡ���κ���Ŀ
#define SELECT_ITEM_MENU	1		//ѡ�в˵�
#define SELECT_ITEM_FUNC	2		//ѡ�й��ܺ�����
#endif
