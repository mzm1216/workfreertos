#ifndef __RTUDEV_H_
#define __RTUDEV_H_

#ifdef  RTUDEV_GLOBALS
#define rtudev_ext
#else
#define rtudev_ext  extern
#endif

#include "lpc_types.h"
#include "config.h"
#include "driver_adc.h"
#include "i2c_dev.h"

/* LAST SECTOR */
#define IAP_FIRST_SECTOR             14

#define IAP_LAST_SECTOR             15

#define MAXUSERLEN0 2048//32*4
#define MAXNUMPPOT	20 //����У׼�����Ƶ�����
typedef struct
{
     uint32    len;
     char      buf[MAXUSERLEN0];
}RSData0;

#define MAXQ  2048// 20

typedef struct
{
uint32 front;
uint32 rear;
uint8 QArray[MAXQ];
}Q;//????


/*������ع��ܵĽṹ��*/
typedef struct 
{
	bit01 	rxd_start_f;//s�յ�01
	bit01 	rxd_end_f;//�յ�04 ��1

	bit01 	rxd_full;
	bit01 	rxd_escape_f;
	RSData0 rxd_backupbuf;
	uint16 	rx_len;
}VTR_DEV_RX_FUN;

/*������ع��ܵĽṹ��*/
typedef struct 
{
	uint8 		lab;//��ǩ
	uint8 		seq;//֡���	
	uint8 		cmd;//����
	uint8 		txd_buf[512];
	uint16		tx_len;//�������ݳ���

}VTR_DEV_TX_FUN;

/*?????????*/
typedef struct 
{
	uint8 		link_f;//PC ??? 1,????0
	uint8 		save_link_f;//PC ??? 1,????0
	uint16 		link_cnt;//?????

}VTR_DEV_LINK_FUN;

/*???????????????*/
typedef struct 
{
	bit01 		picktime_f;//????????????
	bit01 		pickdeal_f;//?pick_time_f =1 ?,?250ms?PC?CHC ?????????????????
	uint8		edit_chnum_ptr;//???????,??????????
	uint8		picktime_cnt;//???
}VTR_DEV_PICKCH_FUN;

/*?????????????????*/
typedef struct 
{
	bit01 		auto_display_flg;//?????????? 0 ? 1?
	bit01 		auto_display_deal;//?dgdisplay_deal =1 ?,????????
	uint8		auto_display_time;////vtr????????(?)
	uint16		auto_display_cnt;//???
}VTR_DEV_AUTODISPLAY_FUN;

/*?????????????????*/
typedef struct 
{
	uint8 		updata_flg;//????
	uint16		updata_cnt;//???
}VTR_DEV_UPDATA_FUN;

typedef struct
{
	uint8 						ch;				//???? 0 pc 1 chc
	VTR_DEV_TX_FUN 				txd_fun;		//??????
	VTR_DEV_RX_FUN 				rxd_fun;		//????????
	Q 							rxd_buf;		//?????
	
	VTR_DEV_LINK_FUN			link_fun;		//??????
	VTR_DEV_PICKCH_FUN			pick_fun;		//??????
	VTR_DEV_AUTODISPLAY_FUN  	display_fun;	//??????
	VTR_DEV_UPDATA_FUN			updata_fun;		//????	

}VTR_DEV_DATA;

#define VTR_DEV_SIZE   sizeof(VTR_DEV_DATA)
#define PARAM_FN_SIZE(x)   (VTR_PARAM_F##x)
typedef struct
{
	uint16 mintxfreq;//z��С����Ƶ��
	uint16 maxtxfreq;//z�����Ƶ��
	uint16 minrxfreq;//z��С����Ƶ��
	uint16 maxrxfreq;//z������Ƶ��
	uint16 cs;		//4��Ƶ��У���
}VTR_TXRX_FREQ_DATA;

//�¶�У׼�ṹ��
typedef __packed struct
{
	UinthlType tempValue_a;//�¶�У׼����A
	UinthlType tempValue_b;//�¶�У׼����B
	UinthlType tempValue_c;//�¶�У׼����C
	UinthlType tempValue_d;//�¶�У׼����D;
	uint8	reserved[24];//32�ֽڶ���
}VTR_CALIBRATE_TEMP_STRCUT;

//��ǿУ׼+��ǰֵ�ṹ��
typedef __packed struct
{
	UinthlType	wbValue_a;// �����ǿУ׼����A
	UinthlType	wbValue_b;// �����ǿУ׼����B
	UinthlType	wbValue_c;//�����ǿУ׼����C
	UinthlType	wbValue_d;//�����ǿУ׼����D;

	UinthlType	nbValue_a;// խ����ǿУ׼����A
	UinthlType  nbValue_b;// խ����ǿУ׼����B
	UinthlType	nbValue_c;//խ����ǿУ׼����C
	UinthlType	nbValue_d;//խ����ǿУ׼����D;

	UinthlType	rssi_mv_a_wb;//�����ǿУ׼Aʱ����ǿ�ĵ�ʱֵ
	UinthlType	rssi_mv_b_wb; ;//�����ǿУ׼bʱ����ǿ�ĵ�ʱֵ
	UinthlType	rssi_mv_c_wb; ;//�����ǿУ׼cʱ����ǿ�ĵ�ʱֵ
	UinthlType	rssi_mv_d_wb;//80~87;//�����ǿУ׼dʱ����ǿ�ĵ�ʱֵ
		
	UinthlType	rssi_mv_a_nb; ;// խ����ǿУ׼Aʱ����ǿ�ĵ�ʱֵ
	UinthlType	rssi_mv_b_nb; ;// խ����ǿУ׼bʱ����ǿ�ĵ�ʱֵ
	UinthlType	rssi_mv_c_nb; ;// խ����ǿУ׼cʱ����ǿ�ĵ�ʱֵ
	UinthlType	rssi_mv_d_nb;//88~;//խ����ǿУ׼dʱ����ǿ�ĵ�ʱֵ
}VTR_CALIBRATE_RSSI_STRCUT;

//����У׼�ṹ��
typedef __packed struct
{
	UinthlType	wbValue_a;// �������У׼����A
	UinthlType	wbValue_b;// �������У׼����B
	
	UinthlType	nbValue_a;// խ������У׼����A
	UinthlType  nbValue_b;// խ������У׼����B
	uint8		reserved[24];

}VTR_CALIBRATE_MUTE_STRCUT;

//��ƵУ׼�ṹ��
typedef __packed struct
{
	UinthlType	voice_fre;		//У׼��ƵƵ��
	UinthlType	voice_dac;		//У׼��Ƶ����
	UinthlType	voice_dac_dc;	//У׼��Ƶֱ��ƫ��
	uint8		voice_fre_state;//У׼��Ƶ״̬	//6	
	uint8		txptt_state;	//���俪��			//0~7	//���ô�
	uint8		chwork;			//������ʽ   ���� ˫�� �м�
	uint8		reserved[23];	//�ǽṹ32�ֽڶ���
}VTR_CALIBRATE_VOICE_STRCUT;

//7��У׼ֵУ׼�ṹ��
typedef __packed struct
{
	UintlhType  Value_a;//aָ��
	UintlhType  Value_b;//bָ��
	UintlhType  Value_c; //cָ��
	UintlhType  Value_d; //dָ��
	UintlhType  Value_e;//eָ��
	UintlhType  Value_f;//fָ��
	UintlhType  Value_g;//gָ��
}VTR_VALUE_A_G_LH;

//7��У׼ֵУ׼�ṹ��
typedef __packed struct
{
	UinthlType  Value_a;//aָ��
	UinthlType  Value_b;//bָ��
	UinthlType  Value_c; //cָ��
	UinthlType  Value_d; //dָ��
	UinthlType  Value_e;//eָ��
	UinthlType  Value_f;//fָ��
	UinthlType  Value_g;//gָ��
}VTR_VALUE_A_G_HL;
//һ��Ƶ��Ĺ���У׼�ṹ��
typedef __packed struct
{
	UinthlType	Refdoc_Value;//�ز�Ƶ��ƫ��У׼
	UinthlType	Refpot_Value;//�ֿ۵���Ƶƫƫ��У׼
	UinthlType	Vcopot_Value;//�߿ڵ���Ƶƫƫ��У׼
	UinthlType	Dacpot_Value;//����ֱ��ƫ��dac
	uint8 		reserved[24];//  32

	VTR_VALUE_A_G_LH Power_Value;//���书��DACֵ
	VTR_VALUE_A_G_HL FWD_Value;//ǰ���ʲɼ�ֵ
	uint8		reserved1[4];// 64
	
} VTR_CALIBRATE_POWER_POINT_STRCUT;


//������Ҫ����ı���
typedef __packed struct
{
	UintType testfre_value;//������ƵƵ��
//	uint8 	reservevalue[2048+638];//�չ�4K
	
} VTR_CALIBRATE_RESERVED_STRCUT;


// У׼ϵͳ����,�ṹ4K ��С
typedef __packed struct
{
	VTR_CALIBRATE_TEMP_STRCUT			TempValue; 				//�¶�У׼�ṹ��--���λ��PA--eeprom   ��С32���ֽ�
	VTR_CALIBRATE_RSSI_STRCUT			RssiValue;				//��ǿУ׼����--���λ��RX--eeprom	   ��С32�ֽ�
	VTR_CALIBRATE_MUTE_STRCUT			MuteValue;				//����У׼����---��RX--eeprom		��С32�ֽ�
	
	VTR_CALIBRATE_VOICE_STRCUT			VoiceValue;				//��ƵУ׼����----���eeprom--TX	��С32�ֽ�
	
	VTR_CALIBRATE_POWER_POINT_STRCUT	PowerValue[MAXNUMPPOT];	//����У׼����----���eeprom--TX	��С32�ֽ�
	VTR_CALIBRATE_RESERVED_STRCUT		Reserved;				//�����ռ䣬Ϊ������Ҫ����Ļ�����

}VTR_CALIBRATE_SYSTEM_PARAM;


// У׼ʱʹ�õ���ʱ�ṹ
typedef __packed struct
{
	uint8 	fn;			//�������
	uint8   powtype;	//�������͡��Ǳ���λ�ã�
	uint8   saveoffset; //Ƶ���Ӧ�Ĵ洢λ��
	uint8	freqnum;	//Ƶ�ʺ��룬ʵ����ֵ��+300
	uint8   cmdoffset;		//�������,У׼�����Ӧ����ţ�Ҳ�Ǳ���λ��
	uint8   optflg;		//������־�����ʺ�pot����ı�־
	uint8   wbnbflg;	//���խ��ģʽ��0Ϊ��1Ϊխ
	
	uint8   paramlen;	//��������
	uint8*  paramptr;	//����ָ��	
	uint8*  saveptr;	//����ָ�룬���ݱ���ָ��
	uint8*	adcurvalue; //��ǰֵ����λ�ã���ǰֵ�ı���λ��
}VTR_CALIBRATE_TMP_STRCUT;

//���� KC ��ֵ������
typedef  __packed struct{
	fp64	KC_k1;
	fp64	KC_c1;
	fp64	KC_k2;
	fp64	KC_c2;
	fp64	KC_k3;
	fp64	KC_c3;
	fp64	KC_k4;
	fp64	KC_c4;
	fp64	KC_k5;
	fp64	KC_c5;
	fp64	KC_k6;
	fp64	KC_c6;
}POWER_STRUCT_KC;

typedef POWER_STRUCT_KC  FWD_STRUCT_KC;
//���㳡ǿ������KC ��ֵ
typedef  __packed struct{
	fp64	rx_rssi_wb_k1; //���У׼
	fp64 	rx_rssi_wb_c1;
	fp64	rx_rssi_wb_k2;
	fp64 	rx_rssi_wb_c2;
	fp64 	rx_rssi_wb_k3;
	fp64	rx_rssi_wb_c3;		

	fp64 	rx_rssi_nb_k1;//խ��У׼
	fp64 	rx_rssi_nb_c1;
	fp64 	rx_rssi_nb_k2;
	fp64 	rx_rssi_nb_c2;
	fp64 	rx_rssi_nb_k3;
	fp64 	rx_rssi_nb_c3;		
}RSSI_STRUCT_KC;//
typedef   __packed struct
{
	POWER_STRUCT_KC	Power_KC_Value[MAXNUMPPOT];//DACֵ��KC�ṹ��
	FWD_STRUCT_KC	Fwd_KC_Value[MAXNUMPPOT]; //ǰ����KC�ṹ��
	RSSI_STRUCT_KC	Rssi_KC_Value; //��ǿУ׼KC�ṹ��
	uint8 release[150]//�չ�44K
} VTR_CALIBRATE_KC_STRCUT;

#define suc_respond	0x00//????	??
#define fail_respond	0x01//				??
#define suc_type		0xff//????
#define fail_type1	0x01//??????
#define fail_type2	0x02//?EEPROM??
#define fail_type3	0x03//?FLASH??
#define fail_type4	0x04//???????,?????
#define fail_type5	0x05//????
#define fail_type6	0x06//??CRC????
#define holdvalue	0xff

#define RET_FAIL_F0		(0xFF-suc_respond)	//????.
#define RET_FAIL_F1		(0xFF-fail_type1)	//????:	??????
#define RET_FAIL_F2		(0xFF-fail_type2)	//????:	?EEPROM??
#define RET_FAIL_F3		(0xFF-fail_type3)	//????:	?FLASH??
#define RET_FAIL_F4		(0xFF-fail_type4)	//????:	???????,?????
#define RET_FAIL_F5		(0xFF-fail_type5)	//????:	????
#define RET_FAIL_F6		(0xFF-fail_type6)	//????:	??CRC????
#define RET_FAIL_FNULL	0xAA	//??chc????,????--0xf6






typedef  struct
{
	uint8 ascbuf[8];
}NAMETYPE;

typedef  struct
{
	uint8 property;
}NUMTYPE;

typedef  struct
{
	uint8 property;
}EDITSTATETYPE;

typedef  __packed struct
{
	UinthlType propertyint;
	UinthlType propertyfrac;	
}TXFRETYPE;

typedef  __packed struct
{
	UinthlType propertyint;
	UinthlType propertyfrac;
}RXFRETYPE;

typedef  struct
{
	uint8 property;
}TXHTONETYPE;

typedef  struct
{
	UintlhType propertyint;
}TXHTONEVALUETYPE;

typedef  struct
{
	uint8 property;
}RXHTONETYPE;
//0x00:ctcss
//0x01:dsc
//??

typedef  struct
{
	UintlhType propertyint;
}RXHTONEVALUETYPE;

typedef  struct
{
	UintlhType propertyint;
}TXPOWERPOTTYPE;

typedef  struct
{
	uint8 property;
}TXPOWERTYPE;
/*
typedef  struct
{
	uint8 property;
}TXPOWERPOTTYPE_BYTE;
*/
typedef  struct
{
	uint8 property;
}REFOSCPOTTYPE;

typedef  struct
{
	uint8 property;
}VMODPOTTYPE;

typedef  struct
{
	uint8 property;
}RMODPOTTYPE;

typedef  struct
{
	UinthlType propertyintfrac;
}FWDPTYPE;

typedef  struct
{
	UinthlType propertyintfrac;
}REFPTYPE;

typedef  struct
{
	UinthlType propertyintfrac;
}SWRPTYPE;

typedef  struct
{
	UinthlType propertyintfrac;//?bb
}RSSITYPE;

typedef  struct
{
	uint8 property;
}PTTSTATETYPE;

typedef  struct
{
	UinthlType propertyintfrac;
}PATEMPTYPE;

typedef  struct
{
	uint8 property;
}MUTESTATETYPE;

typedef  struct
{
	UinthlType propertyintfrac;
}RXVCOVTYPE;

typedef  struct
{
	UinthlType propertyintfrac;
}TXVCOVTYPE;

typedef  struct
{
	uint8 property;
}FANSTATETYPE;

typedef  struct
{
	UinthlType propertyintfrac;
}PAVOLTAGETYPE;

typedef  struct
{
	UinthlType propertyint;
}ALARMSTATETYPE;

typedef  struct
{
	uint8 property;
}LEDSTATETYPE;

typedef  struct
{
	uint8 property;
}CHWORKTYPE;

typedef  struct
{
	uint8 property;
}WBNBSTATETYPE;

typedef  struct
{
	uint8 property;
}SCANTYPE;
typedef  struct
{
	NAMETYPE			name;//????(8)?????  ascbuf[0]="c"
	NUMTYPE				number;//???(1)
	EDITSTATETYPE		edit;//????(1)
	
	TXFRETYPE			txfre;//????(4)
	RXFRETYPE			rxfre;//????(4)
	uint8				reservevalue1;//		1
	TXHTONETYPE			txhtone;//??????(1)
	TXHTONEVALUETYPE	txhtonevalue;//????(2)
	uint8				reservevalue2;//   1
	RXHTONETYPE			rxhtone;//??????(1)
	RXHTONEVALUETYPE	rxhtonevalue;//????(2)
	
	TXPOWERTYPE			txpower;//?????(1)
	REFOSCPOTTYPE		refoscpot;//REF OSC FRE????(1)
	VMODPOTTYPE			vmodpot;//VCO MODE????(1)
	RMODPOTTYPE			rmodpot;//REF MODE????(1)
	
	FWDPTYPE			fwdp;//????(2)
	REFPTYPE			refp;//????(2)
	SWRPTYPE			swrp;//???(2),
	RSSITYPE			rssip;//????(2)

	PTTSTATETYPE		pttstate;//PTT??(1),
	MUTESTATETYPE		mutestate;//????(1)
	
	PATEMPTYPE			patemp;//????(2)
	RXVCOVTYPE			rxvcov;//RXVCO??
	TXVCOVTYPE			txvcov;//TXVCO??(2)

	PAVOLTAGETYPE		pavoltage;//pa????
	ALARMSTATETYPE		alarmstate;//????
	
	LEDSTATETYPE		ledstate;//,led??(1)
	CHWORKTYPE			chwork;//??????(1)
	FANSTATETYPE		fanstate;//????(1),
	WBNBSTATETYPE		wbnbstate;//??????(1)
	SCANTYPE			scantype;//????(1)
	uint8				reservevalue3;
	TXPOWERPOTTYPE		txpowerpot;//????????(2),
	TXPOWERPOTTYPE		txpowerpothalf;//?????????(2)
}VtrChInfo,VTR_PARAM_F4;

#define CHINFOPARLENGTH	38
#define CHINFOLENGTH	sizeof(VTR_PARAM_F4)//60
typedef union
{
	VtrChInfo 	vtrchannelinfo;
	uint8 		buf[CHINFOLENGTH];
}VtrChInfoUnion,VTR_PARAM_UNION_F4;







typedef struct
{
	uint8 reservevalue;//����
}VTR_PARAM_F5;

typedef struct
{
	uint8 d1;// ѡ�������ͣ�1��
	uint8 d2;//��ѡ����TFTҺ����̬��ʾפ�����棨1����
	uint8 d3[6];//TFTҺ���ŵ��������루6����ÿ�ֽ�Ϊ����ASC�룬��Χ"0~9"��

}VTR_PARAM_F6;

typedef struct
{
	uint8 x;
	uint8 y;
	uint8 z;
}VERTYPE;
#define VTRACCESSORIESTYPELENGTH		1
#define VTRACCESSORIESDGDISTYPELENGTH		1
#define VTRACCESSORIESTFTMMLENGTH		6




typedef struct
{
	uint8 	model;
	uint8	reservevalue1;
	NAMETYPE	name;	
	uint8		versionx;
	uint8		versiony;
	uint8		versionz;
	uint8	reservevalue2;
}VtrIdInfoPar,VTR_PARAM_F8;

#define VTRIDINFOPARLENGTH		14
typedef union
{
	VtrIdInfoPar 	vtridinfo;
	uint8 		buf[VTRIDINFOPARLENGTH];	
}VtrIdInfoUnion,VTR_PARAM_UNION_F8;
#define VTRVOLTAGEALARMPARLENGTH	10

//4.11��0xa VTR��Ӧ��ѯ��ѹ������������
typedef struct
{
	uint8 	alarmtype;//�������ͣ�1��
	uint8	alarmoperate;//����������1��
	UinthlType	lowvoltagealarm;	//�͵�ѹ���Ʊ�����2��
	UinthlType	lowvoltagecut;//�͵�ѹ���ƹضϣ�2��
	UinthlType	highvoltagealarm;	//�ߵ�ѹ���Ʊ�����2��
	UinthlType	highvoltagecut;//�ߵ�ѹ���ƹضϣ�2��
}VtrVoltageAlarmPar,VTR_PARAM_FA;
typedef union
{
	VtrVoltageAlarmPar 	vtrvoltagealarm;
	uint8 		buf[VTRVOLTAGEALARMPARLENGTH];	
}VtrVoltageAlarmUnion,VTR_PARAM_UNION_FA;
#define VTRPATEMPALARMPARLENGTH	12
typedef struct
{
	uint8 	alarmtype;
	uint8	alarmoperate;
	UinthlType	fanontemp;	
	UinthlType	fanofftemp;
	UinthlType	papowerreducetemp;	
	UinthlType	papowercuttemp;
	UinthlType	hightempalarm;
}VtrPaTempAlarmPar,VTR_PARAM_FC;
typedef union
{
	VtrPaTempAlarmPar 	vtrpatempalarm;
	uint8 		buf[VTRPATEMPALARMPARLENGTH];	
}VtrPaTempAlarmUnion,VTR_PARAM_UNION_FC;

#define VTRTXPOWERALARMPARLENGTH		6
typedef struct
{
	uint8 	alarmtype;
	uint8	alarmoperate;
	UinthlType	lowfwdalarm;	
	UinthlType	highrefpalarm;
}VtrTxPowerAlarmPar,VTR_PARAM_FE;
typedef union
{
	VtrTxPowerAlarmPar 	vtrtxpoweralarm;
	uint8 		buf[VTRTXPOWERALARMPARLENGTH];	
}VtrTxPowerAlarmUnion;

#define VTRALARMSOURCELEDPARLENGTH	2
typedef struct
{
	UinthlType	alarmsourceled;
}VtrAlarmSourceLedPar,VTR_PARAM_F2D;
typedef union
{
	VtrAlarmSourceLedPar 	vtralarmsourceled;
	uint8 		buf[VTRALARMSOURCELEDPARLENGTH];	
}VtrAlarmSourceLedUnion,VTR_PARAM_UNION_F2D;

#define VTRSOFTWARESETPARPARLENGTH	24 
typedef struct
{
	UinthlType	toneholdtimelength;//��������ʱ�䳤�ȣ�2��ms
	UinthlType	txholdtimelength;//���䱣��ʱ�䳤�ȣ�2��ms
	uint8		txvoicepathtype;//TX��Ƶ·����1��
	uint8		reservevalue1;//
	UinthlType	txlimittime;//������ʱ��2��s
	UinthlType	scanspeed;//ɨ���ٶȣ�2��ms
	UinthlType	scanholdtime;	//ɨ�豣��ʱ�䣨2��ms
	uint8		ctcsstonedelaysendtime;//ģ������ʱ������ӳٷ���ʱ�䣨1��ms��������1�����������������ӳٷ���ʱ�䣨1��ms����������
	uint8		dcstonedelaysendtime;//�������������ӳٷ���ʱ�䣨1��ms
	uint8 		rxintermittentenable;//����״̬�µ����ŵ�����Ъ����ʹ�ܿ��ƣ�1��
	uint8		reservevalue2;
	UinthlType	rxintermittenttime;//�೤ʱ�������ŵ�����Ъ����״̬��2��s
	UinthlType	rxmoduleopentime;//����ģ�鿪��ʱ�䣨2��ms
	UinthlType	rxmoduleclosetime;//����ģ��ر�ʱ�䣨2��ms
	uint8 		txintermittentenable;//����״̬�µ����ŵ�����Ъ����ʹ�ܿ��ƣ�1����
	uint8		fanmode;//ͬƵͬ����txvco����ѡ��1��
}VtrSoftWareSetParPar,VTR_PARAM_F18;
typedef union
{
	VtrSoftWareSetParPar 	vtrsoftwaresetpar;
	uint8 		buf[VTRSOFTWARESETPARPARLENGTH];	
}VtrSoftWareSetParUnion,VTR_PARAM_UNION_F18;
typedef struct
{
	uint8	d1;			//������1��
	uint8	d2;			//EEPROM���ͣ�1��0x00��PA EEPROM		2��0x01��RX EEPROM3��0x02��TX EEPROM
	uint8	d3[2];		//EEPROM��ַ:a����һ�ֽڣ���ַ���ֽ�b���ڶ��ֽڣ���ַ���ֽ�
}VTR_PARAM_F13;
typedef struct
{
	uint8	d1;			//EEPROM���ͣ�1��0x00��PA EEPROM		2��0x01��RX EEPROM3��0x02��TX EEPROM
	uint8	d2[2];		//EEPROM��ַ:a����һ�ֽڣ���ַ���ֽ�b���ڶ��ֽڣ���ַ���ֽ�
	uint8	d3[32];		//EEPROM���ݣ�32��
}VTR_PARAM_F14;
#define VTRHARDWAREPARPARLENGTH	16
typedef struct
{
	uint8 		mutelevel;//�����ƽ
	uint8 		compstate;//��˷�ѹ����1��
	TXFRETYPE	txreffreq;//�����Ƶ��4��
	RXFRETYPE	rxreffreq;//���ջ�Ƶ��4��
	RXFRETYPE	rxfirstif;//���յ�һ��Ƶ��4��
	uint8		muteoff;//������ƣ�1�� 0 kai 1guan
	uint8 		reservevalue2;
}VtrHardWareParPar,VTR_PARAM_F69;
typedef union
{
	VtrHardWareParPar 	vtrhardwarepar;
	uint8 		buf[VTRHARDWAREPARPARLENGTH];	
}VtrHardWareParUnion,VTR_PARAM_UNION_F69;












typedef struct
{
	uint8	d1;			//			??????:	1)	0x00:??	2)	0x01:??	
}VTR_PARAM_F53;

typedef struct
{
	
	
	uint8 led1_ctl;
	
	uint8 led2_ctl;
	
	uint8 led3_ctl;
	
	uint8 led4_ctl;
	
	uint8 rxfled_ctl;
	
	uint8 alarmfled_ctl;
	
	uint8 trcmodefled_ctl;
	
	uint8 txfled_ctl;
	
			
}VTR_PARAM_F63;
typedef struct
{
	uint8	d1[2];			//			??????:			a)????:???????	a)????:???????
	
}VTR_PARAM_F65;

typedef struct
{
	UinthlType	voice_fre;//??????
	UinthlType	voice_dac;//??????
	UinthlType	voice_dac_dc;//????????
	uint8		voice_fre_state;//??????	//6	
	uint8		txptt_state;//????			//0~7	//???
	uint8		chwork;//????   ?? ?? ??
	uint8       reservevalue1;//??

	
}VTR_PARAM_F6B;
typedef struct
{
	UinthlType 	d1;//REF_MOD_DC????:0x00-0xffff;
	UinthlType	d2;//REF_MOD????:0x0000-0xffff;
	UinthlType	d3;//VCO_MOD????:0x0000-0xffff;	
	UinthlType	d4;//REFMOD??????DAC?:0x0000-0xffff;
	
	uint8    d5;//????:	1)0x00:361M2)0x01:363M3)0x02:365M4)0x03:367M	
}VTR_PARAM_F6D;
typedef struct
{
	uint8	 d1;//0x89,??????(1)	??????:1)	0x00:??2)	0x01:??	
}VTR_PARAM_F89;
typedef  struct
{	
	/*tx_power_361M_k1 ?c1 ???????????    ???????   ??-change_power_to_powerpot_361M*/
	fp64	tx_power_361M_k1;
	fp64	tx_power_361M_c1;	
	fp64	tx_power_361M_k2;
	fp64	tx_power_361M_c2;	
	fp64	tx_power_361M_k3;	
	fp64	tx_power_361M_c3;	
	fp64	tx_power_361M_k4;
	fp64	tx_power_361M_c4;	
	fp64	tx_power_361M_k5;
	fp64	tx_power_361M_c5;	
	fp64	tx_power_361M_k6;	
	fp64	tx_power_361M_c6;	    
	/*tx_fwdpower_361M_k1 ?c1 ???????????    ?????   --change_fwdpowermv_to_fwdpowerw_361_367M_byw*/
	fp64 tx_fwdpower_361M_k1;
	fp64 tx_fwdpower_361M_c1;
	fp64 tx_fwdpower_361M_k2;
	fp64 tx_fwdpower_361M_c2;
	fp64 tx_fwdpower_361M_k3;
	fp64 tx_fwdpower_361M_c3;
	fp64 tx_fwdpower_361M_k4;
	fp64 tx_fwdpower_361M_c4;
	fp64 tx_fwdpower_361M_k5;
	fp64 tx_fwdpower_361M_c5;
	fp64 tx_fwdpower_361M_k6;
	fp64 tx_fwdpower_361M_c6;    
	/*tx_fwdpower_361M_k1 ?c1 ???????????    ?????   --change_fwdpowermv_to_fwdpowerw_361_367M_byw*/
	fp64 tx_refpower_361M_k1;
	fp64 tx_refpower_361M_c1;
	fp64 tx_refpower_361M_k2;
	fp64 tx_refpower_361M_c2;
	fp64 tx_refpower_361M_k3;
	fp64 tx_refpower_361M_c3;
	fp64 tx_refpower_361M_k4;
	fp64 tx_refpower_361M_c4;
	fp64 tx_refpower_361M_k5;
	fp64 tx_refpower_361M_c5;
	fp64 tx_refpower_361M_k6;
	fp64 tx_refpower_361M_c6;    
	
	fp64	tx_power_363M_k1;
	fp64	tx_power_363M_c1;	
	fp64	tx_power_363M_k2;
	fp64	tx_power_363M_c2;	
	fp64	tx_power_363M_k3;	
	fp64	tx_power_363M_c3;
	fp64	tx_power_363M_k4;
	fp64	tx_power_363M_c4;	
	fp64	tx_power_363M_k5;
	fp64	tx_power_363M_c5;	
	fp64	tx_power_363M_k6;	
	fp64	tx_power_363M_c6;    

	fp64 tx_fwdpower_363M_k1;
	fp64 tx_fwdpower_363M_c1;
	fp64 tx_fwdpower_363M_k2;
	fp64 tx_fwdpower_363M_c2;
	fp64 tx_fwdpower_363M_k3;
	fp64 tx_fwdpower_363M_c3;
	fp64 tx_fwdpower_363M_k4;
	fp64 tx_fwdpower_363M_c4;
	fp64 tx_fwdpower_363M_k5;
	fp64 tx_fwdpower_363M_c5;
	fp64 tx_fwdpower_363M_k6;
	fp64 tx_fwdpower_363M_c6;    

	fp64 tx_refpower_363M_k1;
	fp64 tx_refpower_363M_c1;
	fp64 tx_refpower_363M_k2;
	fp64 tx_refpower_363M_c2;
	fp64 tx_refpower_363M_k3;
	fp64 tx_refpower_363M_c3;
	fp64 tx_refpower_363M_k4;
	fp64 tx_refpower_363M_c4;
	fp64 tx_refpower_363M_k5;
	fp64 tx_refpower_363M_c5;
	fp64 tx_refpower_363M_k6;
	fp64 tx_refpower_363M_c6;	    
	
	fp64	tx_power_365M_k1;
	fp64	tx_power_365M_c1;	
	fp64	tx_power_365M_k2;
	fp64	tx_power_365M_c2;	
	fp64	tx_power_365M_k3;	
	fp64	tx_power_365M_c3;
	fp64	tx_power_365M_k4;
	fp64	tx_power_365M_c4;	
	fp64	tx_power_365M_k5;
	fp64	tx_power_365M_c5;	
	fp64	tx_power_365M_k6;	
	fp64	tx_power_365M_c6;     

	fp64 tx_fwdpower_365M_k1;
	fp64 tx_fwdpower_365M_c1;
	fp64 tx_fwdpower_365M_k2;
	fp64 tx_fwdpower_365M_c2;
	fp64 tx_fwdpower_365M_k3;
	fp64 tx_fwdpower_365M_c3;	
	fp64 tx_fwdpower_365M_k4;
	fp64 tx_fwdpower_365M_c4;
	fp64 tx_fwdpower_365M_k5;
	fp64 tx_fwdpower_365M_c5;
	fp64 tx_fwdpower_365M_k6;
	fp64 tx_fwdpower_365M_c6;    

	fp64 tx_refpower_365M_k1;
	fp64 tx_refpower_365M_c1;
	fp64 tx_refpower_365M_k2;
	fp64 tx_refpower_365M_c2;
	fp64 tx_refpower_365M_k3;
	fp64 tx_refpower_365M_c3;
	fp64 tx_refpower_365M_k4;
	fp64 tx_refpower_365M_c4;
	fp64 tx_refpower_365M_k5;
	fp64 tx_refpower_365M_c5;
	fp64 tx_refpower_365M_k6;
	fp64 tx_refpower_365M_c6;    
	
	fp64	tx_power_367M_k1;
	fp64	tx_power_367M_c1;	
	fp64	tx_power_367M_k2;
	fp64	tx_power_367M_c2;	
	fp64	tx_power_367M_k3;	
	fp64	tx_power_367M_c3;
	fp64	tx_power_367M_k4;
	fp64	tx_power_367M_c4;	
	fp64	tx_power_367M_k5;
	fp64	tx_power_367M_c5;	
	fp64	tx_power_367M_k6;	
	fp64	tx_power_367M_c6;     

	fp64 tx_fwdpower_367M_k1;
	fp64 tx_fwdpower_367M_c1;
	fp64 tx_fwdpower_367M_k2;
	fp64 tx_fwdpower_367M_c2;
	fp64 tx_fwdpower_367M_k3;
	fp64 tx_fwdpower_367M_c3;	
	fp64 tx_fwdpower_367M_k4;
	fp64 tx_fwdpower_367M_c4;
	fp64 tx_fwdpower_367M_k5;
	fp64 tx_fwdpower_367M_c5;
	fp64 tx_fwdpower_367M_k6;
	fp64 tx_fwdpower_367M_c6;    

	fp64 tx_refpower_367M_k1;
	fp64 tx_refpower_367M_c1;
	fp64 tx_refpower_367M_k2;
	fp64 tx_refpower_367M_c2;
	fp64 tx_refpower_367M_k3;
	fp64 tx_refpower_367M_c3;
	fp64 tx_refpower_367M_k4;
	fp64 tx_refpower_367M_c4;
	fp64 tx_refpower_367M_k5;
	fp64 tx_refpower_367M_c5;
	fp64 tx_refpower_367M_k6;
	fp64 tx_refpower_367M_c6;    

	fp64 rx_rssi_wb_k1;
	fp64 rx_rssi_wb_c1;
	fp64 rx_rssi_wb_k2;
	fp64 rx_rssi_wb_c2;
	fp64 rx_rssi_wb_k3;
	fp64 rx_rssi_wb_c3;		

	fp64 rx_rssi_nb_k1;
	fp64 rx_rssi_nb_c1;
	fp64 rx_rssi_nb_k2;
	fp64 rx_rssi_nb_c2;
	fp64 rx_rssi_nb_k3;
	fp64 rx_rssi_nb_c3;			
}Power_KC;









typedef  __packed struct
{	
	UinthlType	voice_fre;//??????
	UinthlType	voice_dac;//??????
	UinthlType	voice_dac_dc;//????????
	uint8		voice_fre_state;//??????	//6	
	uint8		txptt_state;//????			//0~7	//???
	uint8		chwork;//????   ?? ?? ??
	uint8              reservevalue1;//??
	
	UinthlType	refdcpot_value_361M;	
	UinthlType	refpot_value_361M;	
	UinthlType	vcopot_value_361M;
	UinthlType	dacpot_value_361M;	//10~17
	UinthlType	refdcpot_value_363M;	
	UinthlType	refpot_value_363M;	
	UinthlType	vcopot_value_363M;
	UinthlType	dacpot_value_363M;//18~25
	UinthlType	refdcpot_value_365M;	
	UinthlType	refpot_value_365M;	
	UinthlType	vcopot_value_365M;
	UinthlType	dacpot_value_365M;// 26~33
	UinthlType	refdcpot_value_367M;	
	UinthlType	refpot_value_367M;	
	UinthlType	vcopot_value_367M;
	UinthlType	dacpot_value_367M;	// 34~41	//?ex_eeprom
	
	UintlhType	power_value_a_361M;//1024
	UintlhType	power_value_b_361M;
	UintlhType	power_value_c_361M;
	UintlhType	power_value_d_361M;// 42~49
	UintlhType	power_value_e_361M;
	UintlhType	power_value_f_361M;
	UintlhType	power_value_g_361M;
    
	UintlhType	power_value_a_363M;//1024
	UintlhType	power_value_b_363M;
	UintlhType	power_value_c_363M;
	UintlhType	power_value_d_363M;// 50~57
	UintlhType	power_value_e_363M;
	UintlhType	power_value_f_363M;
	UintlhType	power_value_g_363M;
    
	UintlhType	power_value_a_365M;//1024
	UintlhType	power_value_b_365M;
	UintlhType	power_value_c_365M;
	UintlhType	power_value_d_365M;// 58~65
	UintlhType	power_value_e_365M;
	UintlhType	power_value_f_365M;
	UintlhType	power_value_g_365M;
    
	UintlhType	power_value_a_367M;//1024
	UintlhType	power_value_b_367M;
	UintlhType	power_value_c_367M;
	UintlhType	power_value_d_367M;// 66~73
	UintlhType	power_value_e_367M;
	UintlhType	power_value_f_367M;
	UintlhType	power_value_g_367M;

		
	UinthlType	temperature_value_a;
	UinthlType	temperature_value_b;
	UinthlType	temperature_value_c;
	UinthlType	temperature_value_d;//74~81	
	
	UinthlType	rssi_value_a_wb;
	UinthlType	rssi_value_b_wb;
	UinthlType	rssi_value_c_wb;
	UinthlType	rssi_value_d_wb;//82~890
	
	UinthlType	rssi_value_a_nb;
	UinthlType	rssi_value_b_nb;
	UinthlType	rssi_value_c_nb;
	UinthlType	rssi_value_d_nb;//90~97

	
	UinthlType	mute_value_a_wb;
	UinthlType	mute_value_b_wb;
	UinthlType	mute_value_a_nb;
	UinthlType	mute_value_b_nb;//98~105
}CalibrateValue;







#define MaxEepromParBufLen		896
typedef  struct
{	
	UintType powerdac2w;
	UintType powerdac5w;
	UintType powerdac20w;
	UintType powerdac50w;//8	
	
	UintType vcoamppoth;//??
	UintType refamppotl;//????	
	UintType refdcpotl;//??????	
	UintType txpwr_value;//?????????
	
	UintType txaudiofredac;//??????	
	UintType testfre_value;//??????
	UintType refmod_value;//??????dtmf_dac?
	UintType	reservevalue1;			

	UintType	reservevalue2;	
	UintType	reservevalue3;	
	UintType	reservevalue4;	
	UintType	reservevalue5;	//32

	uint8 buf[96];				//128

	UinthlType	refdcpot_value_361M;	
	UinthlType	refpot_value_361M;	
	UinthlType	vcopot_value_361M;
	UinthlType	dacpot_value_361M;	//
	UinthlType	refdcpot_value_363M;	
	UinthlType	refpot_value_363M;	
	UinthlType	vcopot_value_363M;
	UinthlType	dacpot_value_363M;//
	UinthlType	refdcpot_value_365M;	
	UinthlType	refpot_value_365M;	
	UinthlType	vcopot_value_365M;
	UinthlType	dacpot_value_365M;// 
	UinthlType	refdcpot_value_367M;	
	UinthlType	refpot_value_367M;	
	UinthlType	vcopot_value_367M;
	UinthlType	dacpot_value_367M;	// 159

/*???????????DAC???*/
	UintlhType	power_value_a_361M;//1024
	UintlhType	power_value_b_361M;
	UintlhType	power_value_c_361M;
	UintlhType	power_value_d_361M;// 40~47
	UintlhType	power_value_e_361M;	
	UintlhType	power_value_f_361M;
	UintlhType	power_value_g_361M;
    
	UintlhType	power_value_a_363M;//1024
	UintlhType	power_value_b_363M;
	UintlhType	power_value_c_363M;
	UintlhType	power_value_d_363M;// 48~55
	UintlhType	power_value_e_363M;	
	UintlhType	power_value_f_363M;
	UintlhType	power_value_g_363M;
    
	UintlhType	power_value_a_365M;//1024
	UintlhType	power_value_b_365M;
	UintlhType	power_value_c_365M;
	UintlhType	power_value_d_365M;// 56~63
	UintlhType	power_value_e_365M;
	UintlhType	power_value_f_365M;
	UintlhType	power_value_g_365M;
    
	UintlhType	power_value_a_367M;//1024
	UintlhType	power_value_b_367M;
	UintlhType	power_value_c_367M;
	UintlhType	power_value_d_367M;// 64~71	//191	
	UintlhType	power_value_e_367M;
	UintlhType	power_value_f_367M;
	UintlhType	power_value_g_367M;

    uint8 buf1[8];//  216-223

	
	//??????,??k?c??  ?? ???????
	UinthlType	fwd_power_a_361M;
	UinthlType	fwd_power_b_361M;
	UinthlType	fwd_power_c_361M;
	UinthlType	fwd_power_d_361M;
	UinthlType	fwd_power_e_361M;
	UinthlType	fwd_power_f_361M;
	UinthlType	fwd_power_g_361M;    
	UinthlType	fwd_power_a_363M;
	UinthlType	fwd_power_b_363M;
	UinthlType	fwd_power_c_363M;
	UinthlType	fwd_power_d_363M;
	UinthlType	fwd_power_e_363M;
	UinthlType	fwd_power_f_363M;
	UinthlType	fwd_power_g_363M;    
	UinthlType	fwd_power_a_365M;
	UinthlType	fwd_power_b_365M;
	UinthlType	fwd_power_c_365M;
	UinthlType	fwd_power_d_365M;
	UinthlType	fwd_power_e_365M;
	UinthlType	fwd_power_f_365M;
	UinthlType	fwd_power_g_365M;    
	UinthlType	fwd_power_a_367M;
	UinthlType	fwd_power_b_367M;
	UinthlType	fwd_power_c_367M;
	UinthlType	fwd_power_d_367M;	//224
	UinthlType	fwd_power_e_367M;
	UinthlType	fwd_power_f_367M;
	UinthlType	fwd_power_g_367M;	

    uint8 buf2[8];//  280-287
	//??????,??k?c???? ???????
	UinthlType	ref_power_a_361M;
	UinthlType	ref_power_b_361M;
	UinthlType	ref_power_c_361M;    
	UinthlType	ref_power_d_361M; 
	UinthlType	ref_power_e_361M;
	UinthlType	ref_power_f_361M;
	UinthlType	ref_power_g_361M;    
	UinthlType	ref_power_a_363M;
	UinthlType	ref_power_b_363M;
	UinthlType	ref_power_c_363M;
	UinthlType	ref_power_d_363M;
	UinthlType	ref_power_e_363M;
	UinthlType	ref_power_f_363M;
	UinthlType	ref_power_g_363M;    
	UinthlType	ref_power_a_365M;
	UinthlType	ref_power_b_365M;
	UinthlType	ref_power_c_365M;
	UinthlType	ref_power_d_365M;
	UinthlType	ref_power_e_365M;
	UinthlType	ref_power_f_365M;
	UinthlType	ref_power_g_365M;    
	UinthlType	ref_power_a_367M;
	UinthlType	ref_power_b_367M;
	UinthlType	ref_power_c_367M;
	UinthlType	ref_power_d_367M;	//256	
	UinthlType	ref_power_e_367M;
	UinthlType	ref_power_f_367M;
	UinthlType	ref_power_g_367M;	

    uint8 buf3[8];//  344-351
/*??????,????????k?c*/
	UinthlType	power_garph_361M_k1;
	UinthlType	power_garph_361M_c1;
	UinthlType	power_garph_361M_k2;
	UinthlType	power_garph_361M_c2;
	UinthlType	power_garph_361M_k3;
	UinthlType	power_garph_361M_c3;
	UinthlType	power_garph_361M_k4;
	UinthlType	power_garph_361M_c4;
	UinthlType	power_garph_361M_k5;
	UinthlType	power_garph_361M_c5;
	UinthlType	power_garph_361M_k6;
	UinthlType	power_garph_361M_c6;
    
	//UintType	reservevalue6;	
	//UintType	reservevalue7;	
    uint8 buf4[8];//  376-383
	
	UinthlType	power_garph_363M_k1;
	UinthlType	power_garph_363M_c1;
	UinthlType	power_garph_363M_k2;
	UinthlType	power_garph_363M_c2;
	UinthlType	power_garph_363M_k3;
	UinthlType	power_garph_363M_c3;
	UinthlType	power_garph_363M_k4;
	UinthlType	power_garph_363M_c4;
	UinthlType	power_garph_363M_k5;
	UinthlType	power_garph_363M_c5;
	UinthlType	power_garph_363M_k6;
	UinthlType	power_garph_363M_c6;    
	//UintType	reservevalue8;	
	//UintType	reservevalue9;				//288
    uint8 buf6[8];//  408-415
	
	UinthlType	power_garph_365M_k1;
	UinthlType	power_garph_365M_c1;
	UinthlType	power_garph_365M_k2;
	UinthlType	power_garph_365M_c2;
	UinthlType	power_garph_365M_k3;
	UinthlType	power_garph_365M_c3;
	UinthlType	power_garph_365M_k4;
	UinthlType	power_garph_365M_c4;
	UinthlType	power_garph_365M_k5;
	UinthlType	power_garph_365M_c5;
	UinthlType	power_garph_365M_k6;
	UinthlType	power_garph_365M_c6;     
//	UintType	reservevalue10;	
//	UintType	reservevalue11;	
    uint8 buf7[8];//  440-447
	
	UinthlType	power_garph_367M_k1;
	UinthlType	power_garph_367M_c1;
	UinthlType	power_garph_367M_k2;
	UinthlType	power_garph_367M_c2;
	UinthlType	power_garph_367M_k3;
	UinthlType	power_garph_367M_c3;
	UinthlType	power_garph_367M_k4;
	UinthlType	power_garph_367M_c4;
	UinthlType	power_garph_367M_k5;
	UinthlType	power_garph_367M_c5;
	UinthlType	power_garph_367M_k6;
	UinthlType	power_garph_367M_c6;      
	//UintType	reservevalue12;	
	//UintType	reservevalue13;				//320
	uint8 buf8[8];//  472-479
	/*?????????????? k?c*/
	UinthlType	fwdpower_garph_361M_k1;
	UinthlType	fwdpower_garph_361M_c1;
	UinthlType	fwdpower_garph_361M_k2;
	UinthlType	fwdpower_garph_361M_c2;
	UinthlType	fwdpower_garph_361M_k3;
	UinthlType	fwdpower_garph_361M_c3;
	UinthlType	fwdpower_garph_361M_k4;
	UinthlType	fwdpower_garph_361M_c4;
	UinthlType	fwdpower_garph_361M_k5;
	UinthlType	fwdpower_garph_361M_c5;
	UinthlType	fwdpower_garph_361M_k6;
	UinthlType	fwdpower_garph_361M_c6;    
	//UintType	reservevalue14;	
	//UintType	reservevalue15;	
	uint8 buf9[8];//  504-511
	
	UinthlType	fwdpower_garph_363M_k1;
	UinthlType	fwdpower_garph_363M_c1;
	UinthlType	fwdpower_garph_363M_k2;
	UinthlType	fwdpower_garph_363M_c2;
	UinthlType	fwdpower_garph_363M_k3;
	UinthlType	fwdpower_garph_363M_c3;
	UinthlType	fwdpower_garph_363M_k4;
	UinthlType	fwdpower_garph_363M_c4;
	UinthlType	fwdpower_garph_363M_k5;
	UinthlType	fwdpower_garph_363M_c5;
	UinthlType	fwdpower_garph_363M_k6;
	UinthlType	fwdpower_garph_363M_c6;     
	//UintType	reservevalue16;	
	//UintType	reservevalue17;				//352
	uint8 buf10[8];//  536-543
	
	UinthlType	fwdpower_garph_365M_k1;
	UinthlType	fwdpower_garph_365M_c1;
	UinthlType	fwdpower_garph_365M_k2;
	UinthlType	fwdpower_garph_365M_c2;
	UinthlType	fwdpower_garph_365M_k3;
	UinthlType	fwdpower_garph_365M_c3;
	UinthlType	fwdpower_garph_365M_k4;
	UinthlType	fwdpower_garph_365M_c4;
	UinthlType	fwdpower_garph_365M_k5;
	UinthlType	fwdpower_garph_365M_c5;
	UinthlType	fwdpower_garph_365M_k6;
	UinthlType	fwdpower_garph_365M_c6;     
	//UintType	reservevalue18;	
	//UintType	reservevalue19;	
	uint8 buf11[8];//  568-575
	
	UinthlType	fwdpower_garph_367M_k1;
	UinthlType	fwdpower_garph_367M_c1;
	UinthlType	fwdpower_garph_367M_k2;
	UinthlType	fwdpower_garph_367M_c2;
	UinthlType	fwdpower_garph_367M_k3;
	UinthlType	fwdpower_garph_367M_c3;
	UinthlType	fwdpower_garph_367M_k4;
	UinthlType	fwdpower_garph_367M_c4;
	UinthlType	fwdpower_garph_367M_k5;
	UinthlType	fwdpower_garph_367M_c5;
	UinthlType	fwdpower_garph_367M_k6;
	UinthlType	fwdpower_garph_367M_c6;      
	//UintType	reservevalue20;	
	//UintType	reservevalue21;				//384		
	uint8 buf12[8];//  600-607
/*????????????????k?c*/
	UinthlType	refpower_garph_361M_k1;
	UinthlType	refpower_garph_361M_c1;
	UinthlType	refpower_garph_361M_k2;
	UinthlType	refpower_garph_361M_c2;
	UinthlType	refpower_garph_361M_k3;
	UinthlType	refpower_garph_361M_c3;
	UinthlType	refpower_garph_361M_k4;
	UinthlType	refpower_garph_361M_c4;
	UinthlType	refpower_garph_361M_k5;
	UinthlType	refpower_garph_361M_c5;
	UinthlType	refpower_garph_361M_k6;
	UinthlType	refpower_garph_361M_c6;    
	//UintType	reservevalue22;	
	//UintType	reservevalue23;	
	uint8 buf13[8];//  632-639
	
	UinthlType	refpower_garph_363M_k1;
	UinthlType	refpower_garph_363M_c1;
	UinthlType	refpower_garph_363M_k2;
	UinthlType	refpower_garph_363M_c2;
	UinthlType	refpower_garph_363M_k3;
	UinthlType	refpower_garph_363M_c3;
	UinthlType	refpower_garph_363M_k4;
	UinthlType	refpower_garph_363M_c4;
	UinthlType	refpower_garph_363M_k5;
	UinthlType	refpower_garph_363M_c5;
	UinthlType	refpower_garph_363M_k6;
	UinthlType	refpower_garph_363M_c6;     
	//UintType	reservevalue24;	
	//UintType	reservevalue25;				//416
	uint8 buf14[8];//  664-671
	
	UinthlType	refpower_garph_365M_k1;
	UinthlType	refpower_garph_365M_c1;
	UinthlType	refpower_garph_365M_k2;
	UinthlType	refpower_garph_365M_c2;
	UinthlType	refpower_garph_365M_k3;
	UinthlType	refpower_garph_365M_c3;
	UinthlType	refpower_garph_365M_k4;
	UinthlType	refpower_garph_365M_c4;
	UinthlType	refpower_garph_365M_k5;
	UinthlType	refpower_garph_365M_c5;
	UinthlType	refpower_garph_365M_k6;
	UinthlType	refpower_garph_365M_c6;     
	//UintType	reservevalue26;	
	//UintType	reservevalue27;	
	uint8 buf15[8];//  696-703
	
	UinthlType	refpower_garph_367M_k1;
	UinthlType	refpower_garph_367M_c1;
	UinthlType	refpower_garph_367M_k2;
	UinthlType	refpower_garph_367M_c2;
	UinthlType	refpower_garph_367M_k3;
	UinthlType	refpower_garph_367M_c3;
	UinthlType	refpower_garph_367M_k4;
	UinthlType	refpower_garph_367M_c4;
	UinthlType	refpower_garph_367M_k5;
	UinthlType	refpower_garph_367M_c5;
	UinthlType	refpower_garph_367M_k6;
	UinthlType	refpower_garph_367M_c6;     
	//UintType	reservevalue28;	
	//UintType	reservevalue29;				//448
	uint8 buf16[8];//  728-735
	
	UinthlType	vswr_garph_361M_k1;
	UinthlType	vswr_garph_361M_c1;
	UinthlType	vswr_garph_361M_k2;
	UinthlType	vswr_garph_361M_c2;
	UinthlType	vswr_garph_361M_k3;
	UinthlType	vswr_garph_361M_c3;
	//UintType	reservevalue30;	
	//UintType	reservevalue31;	
	uint8 buf17[16];//  752-767
	
	UinthlType	vswr_garph_363M_k1;
	UinthlType	vswr_garph_363M_c1;
	UinthlType	vswr_garph_363M_k2;
	UinthlType	vswr_garph_363M_c2;
	UinthlType	vswr_garph_363M_k3;
	UinthlType	vswr_garph_363M_c3;
	//UintType	reservevalue32;	
	//UintType	reservevalue33;				//416
	uint8 buf18[16];//  784-799
	
	UinthlType	vswr_garph_365M_k1;
	UinthlType	vswr_garph_365M_c1;
	UinthlType	vswr_garph_365M_k2;
	UinthlType	vswr_garph_365M_c2;
	UinthlType	vswr_garph_365M_k3;
	UinthlType	vswr_garph_365M_c3;
	//UintType	reservevalue34;	
	//UintType	reservevalue35;	
	uint8 buf19[16];//  816-831
	
	UinthlType	vswr_garph_367M_k1;
	UinthlType	vswr_garph_367M_c1;
	UinthlType	vswr_garph_367M_k2;
	UinthlType	vswr_garph_367M_c2;
	UinthlType	vswr_garph_367M_k3;
	UinthlType	vswr_garph_367M_c3;
	//UintType	reservevalue36;	
	//UintType	reservevalue37;					//512
	uint8 buf20[16];//  848-863

	UinthlType	voice_fre;//??????
	UinthlType	voice_dac;//??????
	UinthlType	voice_dac_dc;//????????
	uint8		voice_fre_state;//??????	//6	
	uint8		txptt_state;//????			//0~7	//???
	uint8		chwork;//????   ?? ?? ??
	uint8              reservevalue38;//??		//521

	
}TxEepromPar;


typedef  struct
{		
	UintType	tempadc2w;
	UintType	tempadc5w;	
	UintType	tempadc20w;
	UintType	tempadc50w;//8		
	UintType	fwdpadc2w;
	UintType	fwdpadc5w;
	UintType	fwdpadc20w;
	UintType	fwdpadc50w;//8
	UintType	refpadc2w;
	UintType	refpadc5w;
	UintType	refpadc20w;
	UintType	refpadc50w;//8
	UintType	swrpadc2w;
	UintType	swrpadc5w;
	UintType	swrpadc20w;
	UintType	swrpadc50w;//8	
	
	UinthlType	temperature_value_a;
	UinthlType	temperature_value_b;
	UinthlType	temperature_value_c;
	UinthlType	temperature_value_d;//32	
	uint8 buf1[24];
	
	UinthlType	fwd_value_a_361M;
	UinthlType	fwd_value_b_361M;
	UinthlType	fwd_value_c_361M;
	UinthlType	fwd_value_d_361M;	
	
	UinthlType	fwd_value_a_363M;
	UinthlType	fwd_value_b_363M;
	UinthlType	fwd_value_c_363M;
	UinthlType	fwd_value_d_363M;
	
	UinthlType	fwd_value_a_365M;
	UinthlType	fwd_value_b_365M;
	UinthlType	fwd_value_c_365M;
	UinthlType	fwd_value_d_365M;
	
	UinthlType	fwd_value_a_367M;
	UinthlType	fwd_value_b_367M;
	UinthlType	fwd_value_c_367M;
	UinthlType	fwd_value_d_367M;		

	UinthlType	ref_value_a_361M;
	UinthlType	ref_value_b_361M;
	UinthlType	ref_value_c_361M;
	UinthlType	ref_value_d_361M;
	
	UinthlType	ref_value_a_363M;
	UinthlType	ref_value_b_363M;
	UinthlType	ref_value_c_363M;
	UinthlType	ref_value_d_363M;
	
	UinthlType	ref_value_a_365M;
	UinthlType	ref_value_b_365M;
	UinthlType	ref_value_c_365M;
	UinthlType	ref_value_d_365M;
	
	UinthlType	ref_value_a_367M;
	UinthlType	ref_value_b_367M;
	UinthlType	ref_value_c_367M;
	UinthlType	ref_value_d_367M;	

	UinthlType	swr_value_a_361M;
	UinthlType	swr_value_b_361M;
	UinthlType	swr_value_c_361M;
	UinthlType	swr_value_d_361M;
	
	UinthlType	swr_value_a_363M;
	UinthlType	swr_value_b_363M;
	UinthlType	swr_value_c_363M;
	UinthlType	swr_value_d_363M;
	
	UinthlType	swr_value_a_365M;
	UinthlType	swr_value_b_365M;
	UinthlType	swr_value_c_365M;
	UinthlType	swr_value_d_365M;	
	
	UinthlType	swr_value_a_367M;
	UinthlType	swr_value_b_367M;
	UinthlType	swr_value_c_367M;
	UinthlType	swr_value_d_367M;		
	
}PaEepromPar;

typedef  union
{
	PaEepromPar	papar;
	uint8	buf[MaxEepromParBufLen];
}VtrPaEepromPar;

typedef  struct
{		
	UintType	rssidadc2w;
	UintType	rssidadc5w;
	UintType	rssidadc20w;
	UintType	rssidadc50w;//8
	
	UintType	mutepot;
	UintType	reservevalue1;
	UintType	reservevalue2;
	UintType	reservevalue3;
	
	UintType	reservevalue4;
	UintType	reservevalue5;
	UintType	reservevalue6;
	UintType	reservevalue7;
	
	UintType	reservevalue8;
	UintType	reservevalue9;	
	UintType	reservevalue10;
	UintType	reservevalue11;		//32

	uint8 buf[32];

	UinthlType	rssi_value_a_wb;
	UinthlType	rssi_value_b_wb;
	UinthlType	rssi_value_c_wb;
	UinthlType	rssi_value_d_wb;//64~71
	
	UinthlType	rssi_value_a_nb;
	UinthlType	rssi_value_b_nb;
	UinthlType	rssi_value_c_nb;
	UinthlType	rssi_value_d_nb;//72~79

	UinthlType	rssi_mv_a_wb;
	UinthlType	rssi_mv_b_wb;
	UinthlType	rssi_mv_c_wb;
	UinthlType	rssi_mv_d_wb;//80~87
	UinthlType	rssi_mv_a_nb;
	UinthlType	rssi_mv_b_nb;
	UinthlType	rssi_mv_c_nb;
	UinthlType	rssi_mv_d_nb;//88~95
		
	UinthlType	mute_value_a_wb;
	UinthlType	mute_value_b_wb;
	UinthlType	mute_value_a_nb;
	UinthlType	mute_value_b_nb;//96~103

	UinthlType	rssi_garph_wb_k1;
	UinthlType	rssi_garph_wb_c1;
	UinthlType	rssi_garph_wb_k2;
	UinthlType	rssi_garph_wb_c2;
	UinthlType	rssi_garph_wb_k3;
	UinthlType	rssi_garph_wb_c3;	
	UinthlType	rssi_garph_nb_k1;
	UinthlType	rssi_garph_nb_c1;
	UinthlType	rssi_garph_nb_k2;
	UinthlType	rssi_garph_nb_c2;
	UinthlType	rssi_garph_nb_k3;
	UinthlType	rssi_garph_nb_c3;	//103~127

}RxEepromPar;

typedef  union
{
	RxEepromPar	rxpar;
	uint8	buf[MaxEepromParBufLen];
}VtrRxEepromPar;
typedef  union
{
	TxEepromPar	txpar;
	uint8	buf[MaxEepromParBufLen];
}VtrTxEepromPar;


typedef struct
{
	uint8	d1;			//	??????:			1)0x00:361M	2)0x01:363M	3)0x02:365M4)0x03:367M
	
}VTR_PARAM_F4B;
typedef struct
{
	uint8	d1[2];			//		????DAC?:0x0000-0xffff;			a)????:????DAC????	b)????:????DAC????	
}VTR_PARAM_F4D;

typedef struct
{
	uint8	d1[2];			//		??????:			a)????:??????????	b)????:??????????
}VTR_PARAM_F55;
typedef struct
{
	uint8	d1[2];			//		??????:			a)????:????????	b)????:????????
	
}VTR_PARAM_F5B;
typedef  union
{
	CalibrateValue	calibratepar;
	uint8	buf[256];
}VtrCalibrateValue;













rtudev_ext VtrCalibrateValue vtr_calibratevalue_normal;//???????_????
rtudev_ext VtrCalibrateValue vtr_calibratevalue_calibrate;//???????_????
rtudev_ext uint8 vtr_power_work_type;//?????????361M  363M  365M  367M  ?????
rtudev_ext uint8 vtr_rssi_work_type;//????????????
rtudev_ext uint8 vtr_mute_work_type;//???????????
rtudev_ext uint8 vtr_pot_work_type;//???pot??????
rtudev_ext uint8 power_calibrate_361M_flag;//???????361M
rtudev_ext VtrTxEepromPar	vtr_eeprom_tx; 
rtudev_ext VtrRxEepromPar	vtr_eeprom_rx; 
rtudev_ext VtrPaEepromPar	vtr_eeprom_pa; 

rtudev_ext uint8 rssi_calibrate_wbflag;//????????1-4??abcd4???
rtudev_ext uint8 rssi_calibrate_nbflag;//????????
rtudev_ext uint8 save_calibrate_param_data(VTR_CALIBRATE_TMP_STRCUT* tmpstruct);
rtudev_ext uint8 Vtr_Acount_Power_KC(VTR_CALIBRATE_TMP_STRCUT* tmpstruct);
rtudev_ext uint8 Vtr_Acount_Rssi_Fwd_KC(VTR_CALIBRATE_TMP_STRCUT* tmpstruct);
rtudev_ext void Send_Power_Calibrate_Response(uint8 fail_type);
rtudev_ext void Vtr_after_save_data_deal_with(VTR_CALIBRATE_TMP_STRCUT* tmpstruct);
rtudev_ext void Vtr_set_channel_info_calibrate(uint8 freqnum,uint8 wbnbstate_v,uint8 chwork_v);
rtudev_ext void mzm_kprintf(uint8 id,uint8* str,uint16 data,uint8* str1,uint16 data1,uint8 flg);
rtudev_ext void set_channel_info_test(uint8 channel_number,uint8 wbnbstate_v,uint8 chwork_v);
rtudev_ext void vol_calibrate(void);

#define STATE_INFO_MPT_NUM	(20)



#define  FREQ_NUM_ADDEND		300//???? ?????? ??????,?????????????
/*????????????*/
typedef struct
{
	uint8 	powertype;			//????
	uint8   	saveoffset;			//????, ???????????????
	uint8	freqnum;		//????,???? ? freqnum +FREQ_NUM_ADDEND,???1????
}FREQ_POWERTYPE_GRAPH;

typedef struct
{
	FREQ_POWERTYPE_GRAPH freqgraph;
}FREQ_POWTYPE_GRAPH_STRUCT;



typedef __packed struct
{
	uint8 cmdfn;			//?????
	uint8 paramlen;		//????
	uint8 cmdoffset;			// cmdseq????--??????????????????
	uint8 acountKCflg;		//????:?????KC?????? ???,??????????? 1 ???????,2??KC???????,3,??????,4????,5,???? ,0,????
	int16 firstvalue; 		//??kc?????
	int16 secondvalue;		// ??kc?????
}CALIBRATE_CMD_STRUCT;

#define CALIBRATE_CMD_NUM 25
#define SYS_PARAM_SECTOR_SIZE			32*1024
#define SYS_PARAM_4K_SIZE				4096
#define SYS_PARAM_PAGE_SIZE			256
//PAGE_SIZE_OF_FLASH
/*sectornum:15����ʼ-����pagenums:1920-2047 */
#define SYS_PARAM_BASE_PAGES			(1920) // 1920-2047 sectornum:15
#define SYS_PARAM_FLAGS				0	//
#define SYS_PARAM_DAC_MOD			1
#define SYS_PARAM_EXT_CLK				2
#define SYS_PARAM_FREQ				3


typedef __packed struct
{
	uint8_t param_flg;		//������ʶ
	uint8_t start_page;		//��ʼҳ��
	uint8_t end_page;		//����ҳ��
	uint8_t  sector;		//������
	uint32_t offset_t;		//�ṹ���е�ƫ��
	uint32_t size_t;		//�ṹ��Ա��С
}RTU_SAVE_PARAM_GRAPH_T;
typedef __packed struct
{
	CALIBRATE_CMD_STRUCT calibratecmd;
}CALIBRATE_CMD_GRAPH_STRUCT;

typedef __packed struct {
	uint32_t valid_flag; //������Ч��־��0xffffffff,������Ч������ֵ��Ч,���������ɺ󣬸�ֵ������1�������������Ĳ�����Ч��14������15���� ���ڱ���2�ֲ�����
	uint32_t save_flag;//����λ�á�0===14����λ�ã�1==15����λ�á�
	uint8_t   Reserved[256-8];
} VTR_FLAGS;

typedef  __packed  struct {
	VTR_FLAGS param_flag;
} VTR_SYS_PARAM_FLAGS;

#define TX_MOD_DELAY_MAX		10
typedef __packed struct
{
	uint16_t tx_mod_delay;	//�����spi_dac��txmodregdma���͵���ʱ
	uint16_t tx_mod_k;		//���� txmodvalue�Ĺ�ʽ ϵ��
	uint16_t tx_mod_c;		//����txmodvalue�Ĺ�ʽ ����
	uint16_t spi_dac_k;		//���� spidacvalue�Ĺ�ʽ����
	uint16_t spi_dac_c;		//���� spidacvalue�Ĺ�ʽ����
	uint16_t restart_dma_flg;		//��������dma���ݰ��Ʊ�־ 0---�رհ��ƣ�1����������
	
	uint8_t   Reserved[256-12];
}VTR_SYS_PARAM_DAC_MOD;

#define REF_CV_VALID_FLG1234  0x1234
#define ENABLE_EXT_CLK		1
#define DISABLE_EXT_CLK		0
typedef __packed struct
{
	uint16_t refcv_valid_flg;	//������Ч��־ 0x1234��������Ч
	uint16_t refcv_value;		// AD ��ȡ��ֵ
	uint16_t ext_clk_state;		//Ext_clk state  
	uint16_t ref_clk_LD;		//�ο�״̬����
	
	uint8_t   Reserved[256-8];
}VTR_SYS_PARAM_EXT_CLK;

typedef __packed struct
{
	uint32_t tx_freq;	//������Ч��־ 0x1234��������Ч
	uint32_t rx_freq;		// AD ��ȡ��ֵ
	
	uint8_t   Reserved[256-8];
}VTR_SYS_PARAM_FREQ_PARAM;

typedef  __packed struct
{
	VTR_SYS_PARAM_FLAGS				sys_param_flags;//������־
	VTR_SYS_PARAM_DAC_MOD			sys_param_dac_mod;		//ϵͳ����
	VTR_SYS_PARAM_EXT_CLK				sys_param_ext_clk;		//ͬ������
	VTR_SYS_PARAM_FREQ_PARAM 		sys_param_freq;
	VTR_CALIBRATE_SYSTEM_PARAM		calibrate_sys_param;//У׼�ýṹ�壬���浽norflash��0x7d000λ�ô�
	VTR_CALIBRATE_KC_STRCUT			calibrate_KC_strcut;//������������KC��ֵ
	
}RTU_PARAM_SAVE_FLASH;
#define CV_ADC_CH			0
#define RSSI_ADC_CH			1
#define PV_FWD_ADC_CH		2
#define PV_REF_ADC_CH		3
#define LO_BAT_ADC_CH		4

#define	VTR_RSSI1	70
#define	VTR_RSSI2	85
#define	VTR_RSSI3	100
#define	VTR_RSSI4	115
#define	VTR_RSSI5	121
#define	NCRSSI_VALUE_H	0x80-1+0x80//�޽���ʱ RSSIֵ
#define	NCRSSI_VALUE_L	0x00

typedef  struct
{
//	UintType RSSI;			//�ź�ǿ�� ADC0-4
//	UintType FWD_P;			//ǰ����5 ADC0-5
//	UintType REFL_P;		//������ ADC0-6
//	UintType LO_BAT;		//�����ѹ ADC0_7
//	UintType CV_ADC;		//ADC0-3 tx-rx-ref
//	UintType MOD;		//ADC0-8 modValue

	UintType Advalue[NUM_CHANNELS_B];
	uint32_t converter;//ת������
	
}RTU_PARAM_NOT_SAVE_ADVALUE;

typedef   struct
{
	RTU_PARAM_NOT_SAVE_ADVALUE rtu_AdcValue;
	VTR_PARAM_UNION_F4	vtr_ch_info_current;
	RTU_PCA9535_PORT_STRUCT rtu_pca9535_port[2];//i2cתio��״̬
}RTU_PARAM_NOT_SAVE_FLASH;

/*RTU�豸�������ϵ���г�ʼ��������2���֣����浽falsh�еĲ��֣��ϵ�֮���flash�ж�ȡ��
������Ĳ��֣��ϵ��ʼ��Ϊ0���������ʼ��*/
typedef struct
{
	RTU_PARAM_SAVE_FLASH 	 		saveparam;//���浽flash ��
	RTU_PARAM_NOT_SAVE_FLASH	notsaveparam;//������
}RTU_PARAM_FLASH_T;

rtudev_ext VTR_DEV_DATA *tmp_vtr_dev;// 
//rtudev_ext VTR_CALIBRATE_SYSTEM_PARAM  	calibrate_sys_param;//У׼�ýṹ�壬���浽norflash��0x7d000λ�ô�
rtudev_ext VTR_CALIBRATE_TMP_STRCUT		calibrate_tmp_struct;//У׼������ʱ�ṹ��
//rtudev_ext VTR_CALIBRATE_KC_STRCUT		calibrate_KC_strcut;//������������KC��ֵ


rtudev_ext RTU_PARAM_FLASH_T *rtudev;// �豸ָ�룬���в�����ͨ���ӿڽ��з��ʡ� 




rtudev_ext	uint8 pc_software_type;//Ϊ0x00ʱд2��洢�� Ϊ0x01ʱд1��洢��
//rtudev_ext 	Power_KC	vtr_power_kc;//У׼�õ�KCֵ
//rtudev_ext 	VTR_DEV_DATA vtr_dev_data_pc;
//rtudev_ext 	__align(4)  uint8 par_buf_f[4096];//��һ��������
rtudev_ext 	uint8_t Rtu_Save_Data_To_Flash(uint8_t sector,uint32_t addr,uint8_t * data,uint32_t size);
rtudev_ext 	void Rtu_Read_Data_From_Flash(uint32_t addr,uint8_t * data,uint32_t size);
rtudev_ext VTR_CALIBRATE_KC_STRCUT * Get_RTU_dev_ptr_KC_struct(void);
rtudev_ext VTR_CALIBRATE_SYSTEM_PARAM * Get_RTU_dev_ptr_sys_param(void);
rtudev_ext RTU_PARAM_NOT_SAVE_ADVALUE * Get_RTU_dev_ptr_Adc_value(void);
rtudev_ext VTR_PARAM_UNION_F4 * Get_RTU_dev_ptr_current_chinfo(void);
rtudev_ext void RTU_Save_Param_to_Flash(void);



rtudev_ext void RTU_Save_Param_to_Flash(void);
rtudev_ext uint8_t Rtu_Save_Data_To_Flash_Page(uint32_t start_page,uint32_t end_page,uint8_t * data,uint32_t size);


#endif /* __RTUDEV_H_ */








