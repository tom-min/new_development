#ifndef _OBD_FORD_PROCESS_H_
#define _OBD_FORD_PROCESS_H_


#include "Fcar_Api.h"
#include <stdint.h>
#include <stdbool.h>



typedef enum INIT_MODE_FORD
{
	FORD_MODE_00=0, //CAN
	FORD_MODE_01=1, //CAN
	FORD_MODE_02=2, //CAN
	FORD_MODE_03=3, //KWP
	FORD_MODE_04=4, //KWP
	FORD_MODE_05=5, //KWP	
	FORD_MODE_06=6, //CAN
	FORD_MODE_07=7, //CAN
	FORD_MODE_08=8, //CAN	
	FORD_MODE_09=9, //
	FORD_MODE_10=10, //KWP

}INIT_MODE_FORD;

typedef enum SYSTEM_FORD
{
	FORD_ENGINE=0x01,
	FORD_TRANSMISSION=0x02,
	FORD_ABS=0x03,
	FORD_BODY=0x04,
	FORD_SRS=0x05,
}SYSTEM_FORD;



typedef enum MSG_ORD_FORD
{
	MSG_Init_CAN_PackType_FORD=0x10, //
	MSG_Init_VPW_PackType_FORD=0x11,
	MSG_Init_PWM_PackType_FORD=0x12,
	MSG_Init_KWP_PackType_FORD=0x13, //
	
	MSG_Init_Ecu_KWP_PackType_FORD=0x23,

	MSG_ALL_CAN_PackType_FORD=0x30, //
	MSG_ALL_VPW_PackType_FORD=0x31,
	MSG_ALL_PWM_PackType_FORD=0x32,
	MSG_ALL_KWP_PackType_FORD=0x33, //
	
}MSG_ORD_FORD;

typedef enum DTC_MODE_FORD
{
	DTC_MODE_FORD_MODE_01=0x01, 	//buf[pos-1]如果为0，则无故障码，否则从POS开始，两个字节为一个故障码
	DTC_MODE_FORD_MODE_02=0x02,		//从POS开始，4个字节为一个故障码
}DTC_MODE_FORD;

//初始化IO数据类型
typedef struct
{
	INIT_MODE_FORD Init_Mode; // 初始化模式
	MSG_ORD_FORD Msg_Ord;       // msg All
  uint8_t CMD_DATA[128];       //初始化IO 命令
	//uint8_t System_Enter[8];      // 系统进入命令
	//uint8_t System_Exit[8];       // 退出系统命令         
	
	
} CMD_INIT_FORD;






bool SetFrameTimeOut_FORD(INIT_MODE_FORD index);

void pro_set_init_ecu_ord_FORD(INIT_MODE_FORD mode);
void pro_msg_all_FORD(INIT_MODE_FORD mode);

//void ResetComPort();

bool Init_IO_FORD(INIT_MODE_FORD index);
//bool pro_much_init(INIT_MODE_FORD index);
bool pro_much_init_FORD(INIT_MODE_FORD mode);
bool pro_canbus_select_module_FORD(INIT_MODE_FORD mode);//CAN
bool pro_kwp_select_module_FORD(INIT_MODE_FORD mode);//kwp的81进系统命令

void pro_protocol_init_FORD(INIT_MODE_FORD model);
void cmd_msg_all_canbus_11bit_FORD();
void cmd_msg_all_default_model_FORD();
uint8_t cmd_set_ecumodule_FORD(uint8_t module);

void SetCanbusFilter_FORD(uint32_t filterID);

void pro_set_can_id_FORD(INIT_MODE_FORD mode);//设置CAN协议的id



INIT_MODE_FORD pro_read_dtc_FORD(SYSTEM_FORD sys,INIT_MODE_FORD index,int* dtc_number,char* dtc_list,char* dtc_sys);//读码
void pro_clear_dtc_FORD(); //清码
void pro_exit_system_FORD(INIT_MODE_FORD index); //退出系统
bool pro_pwm_select_module_FORD(INIT_MODE_FORD mode);


//add by znf

bool Much_Init_FORD();

#endif

