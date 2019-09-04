#ifndef _OBD_HONDA_PROCESS_H_
#define _OBD_HONDA_PROCESS_H_


#include "Fcar_Api.h"
#include <stdint.h>
#include <stdbool.h>



typedef enum HONDA_INIT_MODE
{
	//engine
	HONDA_INIT_MODE_01=0x00, //
	HONDA_INIT_MODE_02=0x01, //
	HONDA_INIT_MODE_03=0x02, //
	HONDA_INIT_MODE_04=0x03, //
	HONDA_INIT_MODE_05=0x04, //
	HONDA_INIT_MODE_06=0x05, //
	HONDA_INIT_MODE_07=0x06, //
	
	//AT
	HONDA_INIT_MODE_08=0x07, //
	HONDA_INIT_MODE_09=0x08, //
	HONDA_INIT_MODE_0A=0x09, //
	HONDA_INIT_MODE_0B=0x0a, //
	HONDA_INIT_MODE_0C=0x0b, //
	HONDA_INIT_MODE_0D=0x0c, //
	HONDA_INIT_MODE_0E=0x0d, //
	
	//ABS
	HONDA_INIT_MODE_0F=0x0e, //
	HONDA_INIT_MODE_10=0x0f, //
	HONDA_INIT_MODE_11=0x10, //
	HONDA_INIT_MODE_12=0x11, //
	HONDA_INIT_MODE_13=0x12, //
	
	//BODY
	HONDA_INIT_MODE_14=0x13, //

	//SRS
	HONDA_INIT_MODE_15=0x14, //
	HONDA_INIT_MODE_16=0x15, //
	HONDA_INIT_MODE_17=0x16, //
	
}HONDA_INIT_MODE;

typedef enum HONDA_ENTER_SYSTEM_MODE
{
	HONDA_ENTER_SYSTEM_MODE_01=0x00, //
	HONDA_ENTER_SYSTEM_MODE_02=0x01, //
	HONDA_ENTER_SYSTEM_MODE_03=0x02, //
	HONDA_ENTER_SYSTEM_MODE_04=0x03, //
	HONDA_ENTER_SYSTEM_MODE_05=0x04, //
	HONDA_ENTER_SYSTEM_MODE_06=0x05, //
	HONDA_ENTER_SYSTEM_MODE_07=0x06, //
	HONDA_ENTER_SYSTEM_MODE_08=0x07, //
	HONDA_ENTER_SYSTEM_MODE_09=0x08, //
	HONDA_ENTER_SYSTEM_MODE_0A=0x09, //
	HONDA_ENTER_SYSTEM_MODE_0B=0x0a, //
	HONDA_ENTER_SYSTEM_MODE_0C=0x0b, //
	HONDA_ENTER_SYSTEM_MODE_0D=0x0c, //
	HONDA_ENTER_SYSTEM_MODE_0E=0x0d, //
	HONDA_ENTER_SYSTEM_MODE_0F=0x0e, //
	HONDA_ENTER_SYSTEM_MODE_10=0x0f, //
	HONDA_ENTER_SYSTEM_MODE_11=0x10, //
	HONDA_ENTER_SYSTEM_MODE_12=0x11, //
	HONDA_ENTER_SYSTEM_MODE_13=0x12, //
	HONDA_ENTER_SYSTEM_MODE_14=0x13, //
	HONDA_ENTER_SYSTEM_MODE_15=0x14, //
	HONDA_ENTER_SYSTEM_MODE_16=0x15, //
	HONDA_ENTER_SYSTEM_MODE_17=0x16, //
	HONDA_ENTER_SYSTEM_MODE_18=0x17, //
	HONDA_ENTER_SYSTEM_MODE_19=0x18, //
	HONDA_ENTER_SYSTEM_MODE_1A=0x19, //
	HONDA_ENTER_SYSTEM_MODE_1B=0x1a, //
	HONDA_ENTER_SYSTEM_MODE_1C=0x1b, //
	HONDA_ENTER_SYSTEM_MODE_1D=0x1c, //
	HONDA_ENTER_SYSTEM_MODE_1E=0x1d, //
	HONDA_ENTER_SYSTEM_MODE_1F=0x1e, //
	HONDA_ENTER_SYSTEM_MODE_20=0x1f, //
	HONDA_ENTER_SYSTEM_MODE_21=0x20, //
	HONDA_ENTER_SYSTEM_MODE_22=0x21, //
	HONDA_ENTER_SYSTEM_MODE_23=0x22, //
	HONDA_ENTER_SYSTEM_MODE_24=0x23, //
	HONDA_ENTER_SYSTEM_MODE_25=0x24, //
	HONDA_ENTER_SYSTEM_MODE_26=0x25, //
	HONDA_ENTER_SYSTEM_MODE_27=0x26, //
	HONDA_ENTER_SYSTEM_MODE_28=0x27, //
	HONDA_ENTER_SYSTEM_MODE_29=0x28, //
	HONDA_ENTER_SYSTEM_MODE_2A=0x29, //
	HONDA_ENTER_SYSTEM_MODE_2B=0x2a, //
	HONDA_ENTER_SYSTEM_MODE_2C=0x2b, //
	HONDA_ENTER_SYSTEM_MODE_2D=0x2c, //
	HONDA_ENTER_SYSTEM_MODE_2E=0x2d, //
	HONDA_ENTER_SYSTEM_MODE_2F=0x2e, //
	HONDA_ENTER_SYSTEM_MODE_30=0x2f, //
	HONDA_ENTER_SYSTEM_MODE_31=0x30, //
	HONDA_ENTER_SYSTEM_MODE_32=0x31, //
	HONDA_ENTER_SYSTEM_MODE_33=0x32, //
	HONDA_ENTER_SYSTEM_MODE_34=0x33, //
	
	HONDA_ENTER_SYSTEM_MODE_NULL=0xff, //未知
}HONDA_ENTER_SYSTEM_MODE;

typedef enum HONDA_ECUID_MODE
{
	HONDA_ECUID_MODE_01=0x00, //Engine CAN CANFI/CANDIE   AT CAN CANEAT/CANCVT/CANDCT/DCT/CAN9AT
	HONDA_ECUID_MODE_02=0x01, //ABS CAN
	HONDA_ECUID_MODE_03=0x02, //Engine honda FI_
	HONDA_ECUID_MODE_04=0x03, //Engine honda DIE
	HONDA_ECUID_MODE_05=0x04, //Engine honda PFI
	HONDA_ECUID_MODE_06=0x05, //AT honda CVT
	HONDA_ECUID_MODE_07=0x06, //AT honda EAT
	HONDA_ECUID_MODE_08=0x07, //AT honda PAT
	HONDA_ECUID_MODE_09=0x08, //ABS honda ABSNK12/ABT
	HONDA_ECUID_MODE_0A=0x09, //ABS honda ABI
	HONDA_ECUID_MODE_0B=0x0a, //BODY KWP10400 
	HONDA_ECUID_MODE_0C=0x0b, //SRS Honda SRS03
	HONDA_ECUID_MODE_0D=0x0c, //SRS Honda SRS06
	HONDA_ECUID_MODE_0E=0x0d, //SRS Honda SRS94
	HONDA_ECUID_MODE_0F=0x0e, //SRS Honda SRS96
	HONDA_ECUID_MODE_10=0x0f, //SRS Honda SRS99
	HONDA_ECUID_MODE_11=0x10, //Engine honda PDI 和PFI命令一样，只是码库不一样
	HONDA_ECUID_MODE_12=0x11,	//Engine CAN CANDIE
	HONDA_ECUID_MODE_13=0x12,	//AT CAN CANEAT
	HONDA_ECUID_MODE_14=0x13,	//AT CAN CANCVT
	HONDA_ECUID_MODE_15=0x14,	//AT CAN CANDCT
	HONDA_ECUID_MODE_16=0x15,	//AT CAN DCT
	HONDA_ECUID_MODE_17=0x16,	//AT CAN CAN9AT
	HONDA_ECUID_MODE_18=0x17,	//SRS CAN
	HONDA_ECUID_MODE_19=0x18, //AT honda PVT
	
}HONDA_ECUID_MODE;


typedef enum MSG_ORD_HONDA
{
	MSG_Init_CAN_PackType_Honda=0x10, //
	MSG_Init_VPW_PackType_Honda=0x11,
	MSG_Init_PWM_PackType_Honda=0x12,
	MSG_Init_KWP_PackType_Honda=0x13, //
	
	MSG_Init_Ecu_KWP_PackType_Honda=0x23,

	MSG_ALL_CAN_PackType_Honda=0x30, //
	MSG_ALL_VPW_PackType_Honda=0x31,
	MSG_ALL_PWM_PackType_Honda=0x32,
	MSG_ALL_KWP_PackType_Honda=0x33, //
	
}MSG_ORD_HONDA;

//本田每ECUID确定了之后，读码清码命令都是一样的了
typedef struct
{
	HONDA_ECUID_MODE Honda_ECUID;//匹配的ECUID
	uint8_t CMD_Read_DTC_Size;//读码命令个数
	uint8_t CMD_Read_DTC[8][8];//读码命令 最多有8条读码命令
	uint8_t DTC_Type[8];//和读码命令对应的读码的方式
	uint8_t DTC_Pos[8];//和读码命令对应的取码号的Pos位
	uint8_t CMD_Clear_DTC_Size;//清码命令个数
	uint8_t CMD_Clear_DTC[8][8];//清码命令
} HONDA_ECUID;

typedef struct
{
	HONDA_ENTER_SYSTEM_MODE System_Enter_Mode;
	uint8_t System_Enter[8];//进系统命令
	uint8_t ECUID_CMD[8];//匹配ECUID的命令
	uint8_t System_Exit[8];//退系统命令
	HONDA_ECUID_MODE Honda_ECUID;//匹配的ECUID
} ENTER_SYSTEM_HONDA;

typedef struct
{
	HONDA_INIT_MODE Init_Mode;
	MSG_ORD_HONDA Msg_Ord;
  uint8_t CMD_DATA[128];
	uint8_t enter_size;
	HONDA_ENTER_SYSTEM_MODE Enter_System_mode[15];
} INIT_IO_CMD_HONDA;



bool SetFrameTimeOut_Honda(HONDA_INIT_MODE index);

void pro_msg_all_Honda(HONDA_INIT_MODE mode);

//void ResetComPort();

bool Init_IO_Honda(HONDA_INIT_MODE index);
intptr_t pro_much_init_honda(HONDA_INIT_MODE Index,HONDA_ENTER_SYSTEM_MODE mode);// 进系统
bool pro_canbus_select_module_honda(HONDA_ENTER_SYSTEM_MODE mode);//CAN
bool pro_kwp_select_module_honda(HONDA_ENTER_SYSTEM_MODE mode);//kwp的81进系统命令
bool pro_honda_select_module_honda(HONDA_ENTER_SYSTEM_MODE mode);//本田协议
bool pro_enter_system_Honda(HONDA_ENTER_SYSTEM_MODE mode); //进入系统
bool pro_init_ECU_id_Honda(HONDA_ENTER_SYSTEM_MODE mode); //读取id

//void pro_protocol_init_Toyota(INIT_MODE_TOYOTA model);
//void cmd_msg_all_canbus_11bit_Toyota();
//void cmd_msg_all_default_model_Toyota();
//uint8_t cmd_set_ecumodule_Toyota(uint8_t module);

//void SetCanbusFilter_Toyota(uint32_t filterID);

//void pro_set_can_id_Toyota(INIT_MODE_TOYOTA mode);//设置CAN协议的id



void pro_read_dtc_Honda(HONDA_ENTER_SYSTEM_MODE enter_system_mode,uint8_t iSystem,int* dtc_number,char* dtc_list,char* dtc_sys); //读码
void pro_clear_dtc_Honda(HONDA_ENTER_SYSTEM_MODE enter_system_mode); //清码
void pro_exit_system_Honda(HONDA_ENTER_SYSTEM_MODE enter_system_mode); //退出系统

void dtc_sprintf_Honda_Mode_1(char *strbuf,uint8_t *buf);
//void dtc_sprintf_Toyota_Mode_2(char *strbuf,uint8_t *buf);
uint8_t GetBitDtc(uint8_t *recvbuf,uint8_t pos,uint8_t *Bit);//本田0x08读码模式用到，获取这个命令有效的位


extern bool Much_Init_Honda(void);

//本田的打包方式
int pack_typef_honda(uint8_t *u8bufin, uint8_t *u8buf, intptr_t bufsize);

//本田CAN协议命令收发
uint16_t cmd_CAN_RECV_SEND_ANS_Honda(uint8_t *cmdbuf,uint8_t *recvbuf);
uint16_t pro_SendRecv_Honda(uint8_t * sendbuf, uint8_t *recvbuf);





#endif

