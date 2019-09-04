#ifndef _OBD_VW_PROCESS_H_
#define _OBD_VW_PROCESS_H_


#include "Fcar_Api.h"
#include <stdint.h>
#include <stdbool.h>

typedef enum INIT_MODE_VW
{
	VW_MODE_01 = 0x00, //
	VW_MODE_02 = 0x01, //

}INIT_MODE_VW;

typedef enum SYSTEM_VW
{
	VW_ENGINE = 0x00,
	VW_TRANSMISSION = 0x01,
	VW_ABS = 0x02,
	VW_BODY = 0x03,
	VW_SRS = 0x04,
}SYSTEM_VW;



typedef enum MSG_ORD_VW
{
	MSG_Init_CANDef_PackType_VW = 0x10, //
	MSG_Init_CANTP_PackType_VW = 0x10, //
	MSG_Init_CANUDS_PackType_VW = 0x10,
	MSG_Init_Audi_PackType_VW = 0x14, //
	MSG_Init_Ecu_KWP_PackType_VW = 0x23,
	MSG_ALL_CAN_PackType_VW = 0x30, //
	MSG_ALL_KWP_PackType_VW = 0x33, //
	MSG_ALL_Audi_PackType_VW = 0x34, //

}MSG_ORD_VW;

typedef enum DTC_MODE_VW
{
	DTC_MODE_VW_MODE_01 = 0x01, 	//buf[pos-1]如果为0，则无故障码，否则从POS开始，两个字节为一个故障码
	DTC_MODE_VW_MODE_02 = 0x02,		//从POS开始，4个字节为一个故障码
}DTC_MODE_VW;

//初始化IO数据类型
typedef struct
{
	INIT_MODE_VW Init_Mode;
	MSG_ORD_VW Msg_Ord;
	uint8_t CMD_DATA[64];
	uint8_t CMD_DATA2[64];
	uint8_t System_Enter1[8];
	uint8_t System_Enter2[8];
	uint8_t System_Enter3[8];
	uint8_t System_Enter4[8];
	uint8_t System_Exit[8];
	uint8_t DTC_Mode;
	uint8_t DTC_Read[8];
	uint8_t DTC_Clear[8];
} CMD_INIT_VW;


bool SetFrameTimeOut_VW(uint8_t* initdata, uint16_t number, uint8_t ord);

//void pro_set_init_ecu_ord_VW(INIT_MODE_VW mode);
void pro_msg_all_VW(INIT_MODE_VW mode);

//void ResetComPort();

bool Init_IO_VW(INIT_MODE_VW index, SYSTEM_VW sys);
bool Init_IO_VW_2(INIT_MODE_VW index);
//bool pro_much_init(INIT_MODE_VW index);
bool pro_much_init_VW(INIT_MODE_VW mode, SYSTEM_VW sys);
bool pro_canbus_select_module_VW(INIT_MODE_VW mode);//CAN
bool pro_set_udsid(uint8_t* initio, SYSTEM_VW sys);
void pro_protocol_init_VW(INIT_MODE_VW model);
bool pro_cantp_select_module_VW(INIT_MODE_VW mode, SYSTEM_VW sys);
void cmd_msg_all_canbus_11bit_VW();
void cmd_msg_all_default_model_VW();
uint8_t cmd_set_ecumodule_VW(uint8_t module);

void SetCanbusFilter_VW(uint32_t filterID);

void pro_set_can_id_VW(INIT_MODE_VW mode);//设置CAN协议的id




INIT_MODE_VW pro_read_dtc_VW(SYSTEM_VW sys, INIT_MODE_VW index, int* dtc_number, char* dtc_list, char* dtc_sys);//读码
void pro_clear_dtc_VW(INIT_MODE_VW index, SYSTEM_VW sys); //清码
INIT_MODE_VW pro_audi_can_readdtc(SYSTEM_VW sys, INIT_MODE_VW index, int* dtc_number, char* dtc_list, char* dtc_sys);
INIT_MODE_VW pro_audi_can_readdtc_asam(SYSTEM_VW sys, INIT_MODE_VW index, int* dtc_number, char* dtc_list, char* dtc_sys);
void pro_exit_system_VW(INIT_MODE_VW index, SYSTEM_VW sys); //退出系统


void dtc_sprintf_VW_Mode_1(char *strbuf, uint8_t *buf);
void dtc_sprintf_VW_Mode_2(char *strbuf, uint8_t *buf);

bool Much_Init_VW();

#endif

