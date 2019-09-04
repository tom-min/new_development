#ifndef _OBD_GREATWALL_PROCESS_H_
#define _OBD_GREATWALL_PROCESS_H_


#include "Fcar_Api.h"
#include <stdint.h>
#include <stdbool.h>



typedef enum INIT_MODE_GREATWALL
{
	GREATWALL_MODE_00=0, //
	GREATWALL_MODE_01=1, //
	GREATWALL_MODE_02=2, //
	GREATWALL_MODE_03=3, //
	GREATWALL_MODE_04=4, //
	GREATWALL_MODE_05=5, //	
	GREATWALL_MODE_06=6, //
	GREATWALL_MODE_07=7, //
	GREATWALL_MODE_08=8, //	
	GREATWALL_MODE_09=9, //
	GREATWALL_MODE_10=10, //
	GREATWALL_MODE_11=11, //	
	GREATWALL_MODE_12=12, //	
	GREATWALL_MODE_13=13, //
	GREATWALL_MODE_14=14, //
	GREATWALL_MODE_15=15, //	
	GREATWALL_MODE_16=16, 
	GREATWALL_MODE_17=17, //
	GREATWALL_MODE_18=18, //
	GREATWALL_MODE_19=19, //
	GREATWALL_MODE_20=20, //
	GREATWALL_MODE_21=21, //
	GREATWALL_MODE_22=22, //	
	GREATWALL_MODE_23=23, //
	GREATWALL_MODE_24=24, //
	GREATWALL_MODE_25=25, //	
	GREATWALL_MODE_26=26, //
	GREATWALL_MODE_27=27, //
	GREATWALL_MODE_28=28, //	
	GREATWALL_MODE_29=29, //	
	GREATWALL_MODE_30=30, //
	GREATWALL_MODE_31=31, //
	GREATWALL_MODE_32=32, //	
	GREATWALL_MODE_33=33, 
	GREATWALL_MODE_34=34, //
	GREATWALL_MODE_35=35, //
	GREATWALL_MODE_36=36, //
	GREATWALL_MODE_37=37, //
	GREATWALL_MODE_38=38, //
	GREATWALL_MODE_39=39, //	
	GREATWALL_MODE_40=40, //
	GREATWALL_MODE_41=41, //
	GREATWALL_MODE_42=42, //	
	GREATWALL_MODE_43=43, //
	GREATWALL_MODE_44=44, //
	GREATWALL_MODE_45=45, //	
	GREATWALL_MODE_46=46, //	
	GREATWALL_MODE_47=47, //
	GREATWALL_MODE_48=48, //
	GREATWALL_MODE_49=49, //	
	GREATWALL_MODE_50=50, 
	
}INIT_MODE_GREATWALL;

typedef enum SYSTEM_GREATWALL
{
	GREATWALL_ENGINE=0x01,
	GREATWALL_TRANSMISSION=0x02,
	GREATWALL_ABS=0x03,
	GREATWALL_BODY=0x04,
	GREATWALL_SRS=0x05,
}SYSTEM_GREATWALL;



typedef enum MSG_ORD_GREATWALL
{
	MSG_Init_CAN_PackType_GREATWALL=0x10, //
	MSG_Init_VPW_PackType_GREATWALL=0x11,
	MSG_Init_PWM_PackType_GREATWALL=0x12,
	MSG_Init_KWP_PackType_GREATWALL=0x13, //
	
	MSG_Init_Ecu_KWP_PackType_GREATWALL=0x23,

	MSG_ALL_CAN_PackType_GREATWALL=0x30, //
	MSG_ALL_VPW_PackType_GREATWALL=0x31,
	MSG_ALL_PWM_PackType_GREATWALL=0x32,
	MSG_ALL_KWP_PackType_GREATWALL=0x33, //
	
}MSG_ORD_GREATWALL;

typedef enum DTC_MODE_GREATWALL
{
	DTC_MODE_GREATWALL_MODE_01=0x01, 	//buf[pos-1]如果为0，则无故障码，否则从POS开始，两个字节为一个故障码
	DTC_MODE_GREATWALL_MODE_02=0x02,		//从POS开始，4个字节为一个故障码
	DTC_MODE_GREATWALL_MODE_03=0x03, 	//buf[pos-1]如果为0，则无故障码，否则从POS开始，两个字节为一个故障码
	DTC_MODE_GREATWALL_MODE_04=0x04,		//从POS开始，4个字节为一个故障码
	DTC_MODE_GREATWALL_MODE_05=0x05, 	//buf[pos-1]如果为0，则无故障码，否则从POS开始，两个字节为一个故障码
	DTC_MODE_GREATWALL_MODE_06=0x06,		//从POS开始，4个字节为一个故障码
	DTC_MODE_GREATWALL_MODE_07=0x07, 	//buf[pos-1]如果为0，则无故障码，否则从POS开始，两个字节为一个故障码
	DTC_MODE_GREATWALL_MODE_08=0x08,		//从POS开始，4个字节为一个故障码
}DTC_MODE_GREATWALL;

//初始化IO数据类型
typedef struct
{
	INIT_MODE_GREATWALL Init_Mode; // 初始化模式
	MSG_ORD_GREATWALL Msg_Ord;       // msg All
  uint8_t CMD_DATA[128];       //初始化IO 命令
	uint8_t System_Enter[8];      // 系统进入命令
	//uint8_t System_Exit[8];       // 退出系统命令   
	
	uint8_t DTC_ReadNum[10];
	uint8_t DTC_Read[8];
	int   VechileSubTypes[30];
	uint8_t DTC_Mode;
	
} CMD_INIT_GREATWALL;


bool SetFrameTimeOut_GREATWALL(INIT_MODE_GREATWALL index);

//void pro_set_init_ecu_ord_GREATWALL(INIT_MODE_GREATWALL mode);
void pro_msg_all_GREATWALL(INIT_MODE_GREATWALL mode);

//void ResetComPort();

bool Init_IO_GREATWALL(INIT_MODE_GREATWALL index);
//bool pro_much_init(INIT_MODE_GREATWALL index);
bool pro_much_init_GREATWALL(INIT_MODE_GREATWALL mode);
bool pro_canbus_select_module_GREATWALL(INIT_MODE_GREATWALL mode);//CAN
bool pro_kwp_select_module_GREATWALL(INIT_MODE_GREATWALL mode);//kwp的81进系统命令


//void cmd_msg_all_canbus_11bit_GREATWALL();
//void cmd_msg_all_default_model_GREATWALL();
//uint8_t cmd_set_ecumodule_GREATWALL(uint8_t module);

void SetCanbusFilter_GREATWALL(uint32_t filterID);

void pro_set_can_id_GREATWALL(INIT_MODE_GREATWALL mode);//设置CAN协议的id




INIT_MODE_GREATWALL pro_read_dtc_GREATWALL(SYSTEM_GREATWALL sys,INIT_MODE_GREATWALL index,int* dtc_number,char* dtc_list,char* dtc_sys);//读码
void pro_clear_dtc_GREATWALL(INIT_MODE_GREATWALL index); //清码
void pro_exit_system_GREATWALL(INIT_MODE_GREATWALL index); //退出系统


void dtc_sprintf_GREATWALL_Mode_1(char *strbuf,uint8_t *buf);
void dtc_sprintf_GREATWALL_Mode_2(char *strbuf,uint8_t *buf);
bool IsTypeOfVechile(int CurVel,const int* pVechArray);
bool Much_Init_GREATWALL();

#endif

