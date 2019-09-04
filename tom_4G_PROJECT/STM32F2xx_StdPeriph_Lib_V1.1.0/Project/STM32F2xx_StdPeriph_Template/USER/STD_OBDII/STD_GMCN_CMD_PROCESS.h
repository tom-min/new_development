#ifndef _OBD_GMCN_PROCESS_H_
#define _OBD_GMCN_PROCESS_H_

#include "Fcar_Api.h"
#include <stdint.h>
#include <stdbool.h>

typedef enum GMCN_INIT_MODE
{
	//engine
	GMCN_INIT_MODE_01=0x00, //
	GMCN_INIT_MODE_02=0x01, //
	
	//AT
	GMCN_INIT_MODE_03=0x02, //
	GMCN_INIT_MODE_04=0x03, //
	
	//ABS
	GMCN_INIT_MODE_05=0x04, //
	GMCN_INIT_MODE_06=0x05, //
	GMCN_INIT_MODE_07=0x06, //
	
	//BODY
	GMCN_INIT_MODE_08=0x07, //
	GMCN_INIT_MODE_09=0x08, //
	GMCN_INIT_MODE_0A=0x09, //
	
	//SRS
	GMCN_INIT_MODE_0B=0x0A, //
	GMCN_INIT_MODE_0C=0x0B, //
	GMCN_INIT_MODE_0D=0x0C, //
	GMCN_INIT_MODE_0E=0x0D, //
	GMCN_INIT_MODE_0F=0x0E, //
	
}GMCN_INIT_MODE;

//typedef enum GMCN_ENTER_SYSTEM_MODE
//{
//	GMCN_ENTER_SYSTEM_MODE_01=0x00, //
//	GMCN_ENTER_SYSTEM_MODE_02=0x01, //
//	GMCN_ENTER_SYSTEM_MODE_03=0x02, //
//	GMCN_ENTER_SYSTEM_MODE_04=0x03, //

//	GMCN_ENTER_SYSTEM_MODE_NULL=0xff, //未知
//}GMCN_ENTER_SYSTEM_MODE;

typedef enum GMCN_READDTC_MODE
{
	GMCN_READDTC_MODE_01=0x00, 
	GMCN_READDTC_MODE_02=0x01, 
	GMCN_READDTC_MODE_03=0x02, 
	GMCN_READDTC_MODE_04=0x03, 
	GMCN_READDTC_MODE_05=0x04, 
	GMCN_READDTC_MODE_06=0x05, 
	GMCN_READDTC_MODE_07=0x06, 
	GMCN_READDTC_MODE_08=0x07, 
	
}GMCN_READDTC_MODE;


typedef enum MSG_ORD_GMCN
{
	MSG_Init_CAN_PackType_Gmcn=0x10, //
	MSG_Init_VPW_PackType_Gmcn=0x11,
	MSG_Init_PWM_PackType_Gmcn=0x12,
	MSG_Init_KWP_PackType_Gmcn=0x13, //
	
	MSG_Init_Ecu_KWP_PackType_Gmcn=0x23,

	MSG_ALL_CAN_PackType_Gmcn=0x30, //
	MSG_ALL_VPW_PackType_Gmcn=0x31,
	MSG_ALL_PWM_PackType_Gmcn=0x32,
	MSG_ALL_KWP_PackType_Gmcn=0x33, //
	
}MSG_ORD_GMCN;


typedef struct
{
	GMCN_READDTC_MODE Gmcn_ReadDTC;//读码类型
	uint8_t CMD_Read_DTC[8];//读码命令
	uint8_t DTC_Type;//和读码命令对应的读码的方式
	uint8_t DTC_Pos;//和读码命令对应的取码号的Pos位
	uint8_t DTC_Len;//单个故障码字节长度
	uint8_t CMD_Clear_DTC[8];//清码命令
} GMCN_READDTC;

typedef struct
{
	GMCN_INIT_MODE Init_Mode;
	MSG_ORD_GMCN Msg_Ord;
	uint8_t CMD_DATA[128];
	uint8_t System_Enter[8];//进系统命令
	uint8_t System_Exit[8];//退系统命令
	uint8_t DTCMode_Size;//读码方式个数
	GMCN_READDTC_MODE Gmcn_DTCMode_Group[8];//读码
} INIT_CMD_GMCN;


bool SetFrameTimeOut_Gmcn(uint8_t index);

void pro_msg_all_Gmcn(GMCN_INIT_MODE mode);
void SetCanbusDefault_Gmcn(GMCN_INIT_MODE mode);

//void ResetComPort();

bool Init_IO_Gmcn(uint8_t index);
intptr_t pro_much_init_Gmcn(uint8_t index);// 进系统
bool pro_canbus_select_module_gmcn(uint8_t index);//CAN
bool pro_kwp_select_module_gmcn(uint8_t index);//kwp的81进系统命令
bool pro_vpw_select_module_gmcn(uint8_t index);//vpw


//void pro_protocol_init_Toyota(INIT_MODE_TOYOTA model);
//void cmd_msg_all_canbus_11bit_Toyota();
//void cmd_msg_all_default_model_Toyota();
//uint8_t cmd_set_ecumodule_Toyota(uint8_t module);

//void SetCanbusFilter_Toyota(uint32_t filterID);

//void pro_set_can_id_Toyota(INIT_MODE_TOYOTA mode);//设置CAN协议的id


void pro_read_dtc_gmcn(uint8_t index,uint8_t iSystem,int* dtc_number,char* dtc_list,char* dtc_sys); //读码
void pro_clear_dtc_gmcn(uint8_t index); //清码
void pro_exit_system_gmcn(uint8_t index); //退出系统

void dtc_sprintf_Gmcn_Mode_1(char *strbuf,uint8_t *buf);
void status_sprintf_gmcn(char *statusbuf, uint8_t * recvbuf);

extern bool Much_Init_Gmcn(void);

uint16_t pro_SendRecv_Gmcn(uint8_t * sendbuf, uint8_t *recvbuf);
uint16_t pro_SendRecv_Gmcn_CAN(uint8_t * sendbuf, uint8_t *recvbuf);
uint16_t pro_SendRecv_CanGM(uint8_t * sendbuf, uint8_t *recvbuf);
uint16_t CommSendBufferCanbus_Gmcn(uint8_t* sendbuf, uint8_t sendnum);
uint16_t CommPackBufferCanbus_Gmcn(uint8_t *buffer, uint16_t count);
uint16_t CommPackBufferDefault_Gmcn(uint8_t *buffer, uint16_t count);
uint16_t CommReadBufferCanbusGM(uint8_t* readbuf, uint16_t maxlen, uint16_t delay);
uint16_t CommSendBufferDefault(uint8_t *buffer, uint16_t count);




#endif

