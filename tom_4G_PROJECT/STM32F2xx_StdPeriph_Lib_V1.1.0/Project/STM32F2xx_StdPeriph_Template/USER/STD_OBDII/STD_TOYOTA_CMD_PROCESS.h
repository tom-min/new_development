#ifndef _OBD_TOYOTA_PROCESS_H_
#define _OBD_TOYOTA_PROCESS_H_


#include "Fcar_Api.h"
#include <stdint.h>
#include <stdbool.h>



typedef enum INIT_MODE_TOYOTA
{
	TOYOTA_MODE_01=0x00, //
	TOYOTA_MODE_02=0x01, //
	TOYOTA_MODE_03=0x02, //
	TOYOTA_MODE_04=0x03, //
	TOYOTA_MODE_05=0x04, //
	TOYOTA_MODE_06=0x05, //
	
	TOYOTA_MODE_07=0x06, //
	TOYOTA_MODE_08=0x07, //
	TOYOTA_MODE_09=0x08, //
	
	TOYOTA_MODE_0A=0x09, //
	TOYOTA_MODE_0B=0x0A, //
	TOYOTA_MODE_0C=0x0B, //
	
	TOYOTA_MODE_0D=0x0C, //
	
	TOYOTA_MODE_0E=0x0D, //
	TOYOTA_MODE_0F=0x0E, //
	TOYOTA_MODE_10=0x0F, //
	
	TOYOTA_MODE_11=0x10, //CAN 这个是发动机的一个特殊情况，和TOYOTA_MODE_02链路完全一样的，只是读码命令和清码命令不一样，即如果TOYOTA_MODE_02的读码失败则调用这个
	
}INIT_MODE_TOYOTA;

typedef enum SYSTEM_TOYOTA
{
	TOYOTA_ENGINE=0x01,
	TOYOTA_TRANSMISSION=0x02,
	TOYOTA_ABS=0x03,
	TOYOTA_BODY=0x04,
	TOYOTA_SRS=0x05,
}SYSTEM_TOYOTA;



typedef enum MSG_ORD_TOYOTA
{
	MSG_Init_CAN_PackType_Toyota=0x10, //
	MSG_Init_VPW_PackType_Toyota=0x11,
	MSG_Init_PWM_PackType_Toyota=0x12,
	MSG_Init_KWP_PackType_Toyota=0x13, //
	
	MSG_Init_Ecu_KWP_PackType_Toyota=0x23,

	MSG_ALL_CAN_PackType_Toyota=0x30, //
	MSG_ALL_VPW_PackType_Toyota=0x31,
	MSG_ALL_PWM_PackType_Toyota=0x32,
	MSG_ALL_KWP_PackType_Toyota=0x33, //
	
}MSG_ORD_TOYOTA;

typedef enum DTC_MODE_TOYOTA
{
	DTC_MODE_TOYOTA_MODE_01=0x01, 	//buf[pos-1]如果为0，则无故障码，否则从POS开始，两个字节为一个故障码
	DTC_MODE_TOYOTA_MODE_02=0x02,		//从POS开始，4个字节为一个故障码
}DTC_MODE_TOYOTA;

//初始化IO数据类型
typedef struct
{
	INIT_MODE_TOYOTA Init_Mode;
	MSG_ORD_TOYOTA Msg_Ord;
  uint8_t CMD_DATA[128];
	uint8_t System_Enter[8];
	uint8_t System_Exit[8];
	uint8_t DTC_Read[8];
	uint8_t DTC_Mode;
	uint8_t DTC_Pos;
	uint8_t DTC_Clear[8];
} CMD_INIT_TOYOTA;


bool SetFrameTimeOut_Toyota(INIT_MODE_TOYOTA index);

//void pro_set_init_ecu_ord_Toyota(INIT_MODE_TOYOTA mode);
void pro_msg_all_Toyota(INIT_MODE_TOYOTA mode);

//void ResetComPort();

bool Init_IO_Toyota(INIT_MODE_TOYOTA index);
//bool pro_much_init(INIT_MODE_TOYOTA index);
bool pro_much_init_Toyota(INIT_MODE_TOYOTA mode);
bool pro_canbus_select_module_Toyota(INIT_MODE_TOYOTA mode);//CAN
bool pro_kwp_select_module_Toyota(INIT_MODE_TOYOTA mode);//kwp的81进系统命令

void pro_protocol_init_Toyota(INIT_MODE_TOYOTA model);
void cmd_msg_all_canbus_11bit_Toyota();
void cmd_msg_all_default_model_Toyota();
uint8_t cmd_set_ecumodule_Toyota(uint8_t module);

void SetCanbusFilter_Toyota(uint32_t filterID);

void pro_set_can_id_Toyota(INIT_MODE_TOYOTA mode);//设置CAN协议的id




INIT_MODE_TOYOTA pro_read_dtc_Toyota(SYSTEM_TOYOTA sys,INIT_MODE_TOYOTA index,int* dtc_number,char* dtc_list,char* dtc_sys);//读码
void pro_clear_dtc_Toyota(INIT_MODE_TOYOTA index); //清码
void pro_exit_system_Toyota(INIT_MODE_TOYOTA index); //退出系统


void dtc_sprintf_Toyota_Mode_1(char *strbuf,uint8_t *buf);
void dtc_sprintf_Toyota_Mode_2(char *strbuf,uint8_t *buf);

bool Much_Init_Toyota();

#endif

