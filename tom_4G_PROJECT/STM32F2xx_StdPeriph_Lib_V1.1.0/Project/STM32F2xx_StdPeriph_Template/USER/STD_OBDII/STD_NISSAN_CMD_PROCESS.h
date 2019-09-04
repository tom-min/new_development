#ifndef _OBD_NISSAN_PROCESS_H_
#define _OBD_NISSAN_PROCESS_H_


#include "Fcar_Api.h"
#include <stdint.h>
#include <stdbool.h>



typedef enum INIT_MODE_NISSAN
{
	NISSAN_MODE_00=0, //CAN
	NISSAN_MODE_01=1, //CAN
	NISSAN_MODE_02=2, //CAN
	NISSAN_MODE_03=3, //KWP
	NISSAN_MODE_04=4, //KWP
	NISSAN_MODE_05=5, //KWP	
	NISSAN_MODE_06=6, //CAN
	NISSAN_MODE_07=7, //CAN
	NISSAN_MODE_08=8, //CAN	
	NISSAN_MODE_09=9, //KWP
	NISSAN_MODE_10=10, //KWP
	NISSAN_MODE_11=11, //CAN	               
	NISSAN_MODE_12=12 , //CAN	               
	NISSAN_MODE_13=13  , //CAN
	NISSAN_MODE_14=14, //CAN
	NISSAN_MODE_15=15, //KWP	               
	NISSAN_MODE_16=16  , //KWP
	NISSAN_MODE_17=17, //CAN
	NISSAN_MODE_18=18, //CAN
	NISSAN_MODE_19=19, //CAN
	NISSAN_MODE_20=20, //KWP
	NISSAN_MODE_21=21, //KWP
	NISSAN_MODE_22=22, //CAN
	NISSAN_MODE_23=23, //CAN
	NISSAN_MODE_24=24, //CAN
	NISSAN_MODE_25=25, //CAN
	NISSAN_MODE_26=26, //KWP
	NISSAN_MODE_27=27, //KWP
	
}INIT_MODE_NISSAN;

typedef enum SYSTEM_NISSAN
{
	NISSAN_ENGINE=0x01,
	NISSAN_TRANSMISSION=0x02,
	NISSAN_ABS=0x03,
	NISSAN_BODY=0x04,
	NISSAN_SRS=0x05,
}SYSTEM_NISSAN;



typedef enum MSG_ORD_NISSAN
{
	MSG_Init_CAN_PackType_NISSAN=0x10, //
	MSG_Init_VPW_PackType_NISSAN=0x11,
	MSG_Init_PWM_PackType_NISSAN=0x12,
	MSG_Init_KWP_PackType_NISSAN=0x13, //
	
	MSG_Init_Ecu_KWP_PackType_NISSAN=0x23,

	MSG_ALL_CAN_PackType_NISSAN=0x30, //
	MSG_ALL_VPW_PackType_NISSAN=0x31,
	MSG_ALL_PWM_PackType_NISSAN=0x32,
	MSG_ALL_KWP_PackType_NISSAN=0x33, //
	
}MSG_ORD_NISSAN;

typedef enum DTC_MODE_NISSAN
{
	DTC_MODE_NISSAN_MODE_01=0x01, 	//buf[pos-1]如果为0，则无故障码，否则从POS开始，两个字节为一个故障码
	DTC_MODE_NISSAN_MODE_02=0x02,		//从POS开始，4个字节为一个故障码
}DTC_MODE_NISSAN;

//初始化IO数据类型
typedef struct
{
	INIT_MODE_NISSAN Init_Mode; // 初始化模式
	MSG_ORD_NISSAN Msg_Ord;       // msg All
  uint8_t CMD_DATA[128];       //初始化IO 命令
	uint8_t System_Enter[8];      // 系统进入命令
	uint8_t System_Exit[8];       // 退出系统命令         
	
	
} CMD_INIT_NISSAN;

typedef struct
{
	uint8_t cmdNo;
	uint8_t conNum;
	uint8_t Result;
	
}CalcResult;


typedef struct{
		uint8_t con;
		uint8_t conNum;
		uint8_t pos;
		uint8_t andNum;
		uint8_t en;
}SingleFilte;



typedef struct {
	uint8_t FitlerCmd[10];
	   // 文件编号0x00-0xff,添加到故障码库的前面
	uint8_t FilteCnt;
	SingleFilte  SingleFitle[30];

}ReadDtcTypeFilte;

typedef struct{
	uint8_t RdTypeFilteCmdCnt;
	uint8_t DtcFile;
	const ReadDtcTypeFilte *pRdTypeFilte;	
	uint8_t pos;	
	uint8_t dtclen;
	uint8_t ReadDtcCmd[10];  
	//uint8_t ClearDtcCmd[10];
}ReadDtcMode;



bool SetFrameTimeOut_NISSAN(INIT_MODE_NISSAN index);

void pro_set_init_ecu_ord_NISSAN(INIT_MODE_NISSAN mode);
void pro_msg_all_NISSAN(INIT_MODE_NISSAN mode);

//void ResetComPort();

bool Init_IO_NISSAN(INIT_MODE_NISSAN index);
//bool pro_much_init(INIT_MODE_NISSAN index);
bool pro_much_init_NISSAN(INIT_MODE_NISSAN mode);
bool pro_canbus_select_module_NISSAN(INIT_MODE_NISSAN mode);//CAN
bool pro_kwp_select_module_NISSAN(INIT_MODE_NISSAN mode);//kwp的81进系统命令

void pro_protocol_init_NISSAN(INIT_MODE_NISSAN model);
void cmd_msg_all_canbus_11bit_NISSAN();
void cmd_msg_all_default_model_NISSAN();
uint8_t cmd_set_ecumodule_NISSAN(uint8_t module);

void SetCanbusFilter_NISSAN(uint32_t filterID);

void pro_set_can_id_NISSAN(INIT_MODE_NISSAN mode);//设置CAN协议的id



INIT_MODE_NISSAN pro_read_dtc_NISSAN(SYSTEM_NISSAN sys,INIT_MODE_NISSAN index,int* dtc_number,char* dtc_list,char* dtc_sys);//读码
void pro_clear_dtc_NISSAN(INIT_MODE_NISSAN index); //清码
void pro_exit_system_NISSAN(INIT_MODE_NISSAN index); //退出系统


void dtc_sprintf_NISSAN_Mode_1(char *strbuf,uint8_t *buf);
void dtc_sprintf_NISSAN_Mode_2(char *strbuf,uint8_t *buf);

//add by znf
int  prossDtcCmd(INIT_MODE_NISSAN index,const ReadDtcMode readdtcmode[],char* pDtcList) ;
int  SpeReadDtcMode(INIT_MODE_NISSAN index,uint8_t LineNo,char* pDtcList) ;
uint8_t GetFaultCodeAll_ENG_F564_D21A1(uint8_t*  pRecvBuf , uint8_t* DtcBuf);
uint8_t GetFaultCodeAll_ENG_F564_D21AF(uint8_t*  pRecvBuf , uint8_t* DtcBuf);
uint8_t GetFaultCodeAll_ENG_F564_D21A0(uint8_t*  pRecvBuf , uint8_t* DtcBuf);
uint8_t GetFaultCodeAll_ENG_F564_D21BD(uint8_t*  pRecvBuf , uint8_t* DtcBuf);
uint8_t GetFaultCodeAll_ENG_F564_D21AD(uint8_t*  pRecvBuf , uint8_t* DtcBuf);
uint8_t GetFaultCodeAll_ENG_F564_D21AC(uint8_t*  pRecvBuf , uint8_t* DtcBuf);
uint8_t GetFaultCodeAll_BCM_D2194_0(uint8_t * pRecvBuf , uint8_t* DtcBuf);
uint8_t GetFaultCodeAll_BCM_D2194_1(uint8_t * pRecvBuf , uint8_t* DtcBuf);
uint8_t GetFaultCodeAll_BCM_D2194_2(uint8_t * pRecvBuf , uint8_t* DtcBuf);
uint8_t GetFaultCodeAll_BCM_D2194_3(uint8_t * pRecvBuf , uint8_t* DtcBuf);
uint8_t GetFaultCodeAll_BCM_D2194_4(uint8_t * pRecvBuf , uint8_t* DtcBuf);
uint8_t GetFaultCodeAll_BCM_D2194_5(uint8_t * pRecvBuf , uint8_t* DtcBuf);
uint8_t GetFaultCodeAll_TRANSMISSION_D21C2(uint8_t* pRecvBuf , uint8_t* DtcBuf);
bool Much_Init_NISSAN();
bool IsThisTypeOfReadDtc(uint8_t Cnt , const ReadDtcTypeFilte *pRdType);


   

#endif

