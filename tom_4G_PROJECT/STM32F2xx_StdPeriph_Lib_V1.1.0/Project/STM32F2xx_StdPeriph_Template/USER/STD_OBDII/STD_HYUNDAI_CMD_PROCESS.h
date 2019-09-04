#ifndef _OBD_HYUNDAI_PROCESS_H_
#define _OBD_HYUNDAI_PROCESS_H_


#include "Fcar_Api.h"
#include <stdint.h>
#include <stdbool.h>



typedef enum HYUNDAI_INIT_MODE
{
	HYUNDAI_INIT_MODE_01 = 0x00, //
	HYUNDAI_INIT_MODE_02 = 0x01, //
	HYUNDAI_INIT_MODE_03 = 0x02, //
	HYUNDAI_INIT_MODE_04 = 0x03, //
	HYUNDAI_INIT_MODE_05 = 0x04, //
	HYUNDAI_INIT_MODE_06 = 0x05, //
	HYUNDAI_INIT_MODE_07 = 0x06, //
	HYUNDAI_INIT_MODE_08 = 0x07, //
	HYUNDAI_INIT_MODE_09 = 0x08, //
	HYUNDAI_INIT_MODE_0A = 0x09, //
	HYUNDAI_INIT_MODE_0B = 0x0a, //
	HYUNDAI_INIT_MODE_0C = 0x0b, //
	HYUNDAI_INIT_MODE_0D = 0x0c, //
	HYUNDAI_INIT_MODE_0E = 0x0d, //
	HYUNDAI_INIT_MODE_0F = 0x0e, //
	HYUNDAI_INIT_MODE_10 = 0x0f, //
	HYUNDAI_INIT_MODE_11 = 0x10, //
	HYUNDAI_INIT_MODE_12 = 0x11, //
	HYUNDAI_INIT_MODE_13 = 0x12, //
	HYUNDAI_INIT_MODE_14 = 0x13, //
	HYUNDAI_INIT_MODE_15 = 0x14, //
	HYUNDAI_INIT_MODE_16 = 0x15, //

}HYUNDAI_INIT_MODE;

typedef enum HYUNDAI_ENTER_SYSTEM_MODE
{
	HYUNDAI_ENTER_SYSTEM_MODE_01 = 0x00, //
	HYUNDAI_ENTER_SYSTEM_MODE_02 = 0x01, //
	HYUNDAI_ENTER_SYSTEM_MODE_03 = 0x02, //
	HYUNDAI_ENTER_SYSTEM_MODE_04 = 0x03, //
	HYUNDAI_ENTER_SYSTEM_MODE_05 = 0x04, //
	HYUNDAI_ENTER_SYSTEM_MODE_06 = 0x05, //
	HYUNDAI_ENTER_SYSTEM_MODE_07 = 0x06, //
	HYUNDAI_ENTER_SYSTEM_MODE_08 = 0x07, //
	HYUNDAI_ENTER_SYSTEM_MODE_09 = 0x08, //
	HYUNDAI_ENTER_SYSTEM_MODE_0A = 0x09, //
	HYUNDAI_ENTER_SYSTEM_MODE_0B = 0x0a, //
	HYUNDAI_ENTER_SYSTEM_MODE_0C = 0x0b, //
	HYUNDAI_ENTER_SYSTEM_MODE_0D = 0x0c, //
	HYUNDAI_ENTER_SYSTEM_MODE_0E = 0x0d, //
	HYUNDAI_ENTER_SYSTEM_MODE_0F = 0x0e, //
	HYUNDAI_ENTER_SYSTEM_MODE_10 = 0x0f, //
	HYUNDAI_ENTER_SYSTEM_MODE_11 = 0x10, //
	HYUNDAI_ENTER_SYSTEM_MODE_12 = 0x11, //
	HYUNDAI_ENTER_SYSTEM_MODE_13 = 0x12, //
	HYUNDAI_ENTER_SYSTEM_MODE_14 = 0x13, //
	HYUNDAI_ENTER_SYSTEM_MODE_15 = 0x14, //
	HYUNDAI_ENTER_SYSTEM_MODE_16 = 0x15, //
	HYUNDAI_ENTER_SYSTEM_MODE_17 = 0x16, //
	HYUNDAI_ENTER_SYSTEM_MODE_18 = 0x17, //
	HYUNDAI_ENTER_SYSTEM_MODE_19 = 0x18, //
	HYUNDAI_ENTER_SYSTEM_MODE_1A = 0x19, //
	HYUNDAI_ENTER_SYSTEM_MODE_1B = 0x1a, //
	HYUNDAI_ENTER_SYSTEM_MODE_1C = 0x1b, //
	HYUNDAI_ENTER_SYSTEM_MODE_1D = 0x1c, //
	HYUNDAI_ENTER_SYSTEM_MODE_1E = 0x1d, //
	HYUNDAI_ENTER_SYSTEM_MODE_1F = 0x1e, //
	HYUNDAI_ENTER_SYSTEM_MODE_20 = 0x1f, //
	HYUNDAI_ENTER_SYSTEM_MODE_21 = 0x20, //
	HYUNDAI_ENTER_SYSTEM_MODE_22 = 0x21, //
	HYUNDAI_ENTER_SYSTEM_MODE_23 = 0x22, //
	HYUNDAI_ENTER_SYSTEM_MODE_24 = 0x23, //

	HYUNDAI_ENTER_SYSTEM_MODE_NULL = 0xff, //δ֪
}HYUNDAI_ENTER_SYSTEM_MODE;

typedef enum HYUNDAI_ECUID_MODE
{
	HYUNDAI_ECUID_MODE_01 = 0x00, //
	HYUNDAI_ECUID_MODE_02 = 0x01, //
	HYUNDAI_ECUID_MODE_03 = 0x02, //
	HYUNDAI_ECUID_MODE_04 = 0x03, //
	HYUNDAI_ECUID_MODE_05 = 0x04, //
	HYUNDAI_ECUID_MODE_06 = 0x05, //
	HYUNDAI_ECUID_MODE_07 = 0x06, //
	HYUNDAI_ECUID_MODE_08 = 0x07, //
	HYUNDAI_ECUID_MODE_09 = 0x08, //
	HYUNDAI_ECUID_MODE_0A = 0x09, //
	HYUNDAI_ECUID_MODE_0B = 0x0a, //
	HYUNDAI_ECUID_MODE_0C = 0x0b, //
	HYUNDAI_ECUID_MODE_0D = 0x0c, //
	HYUNDAI_ECUID_MODE_0E = 0x0d, //
	HYUNDAI_ECUID_MODE_0F = 0x0e, //
	HYUNDAI_ECUID_MODE_10 = 0x0f, //
	HYUNDAI_ECUID_MODE_11 = 0x10, //
	HYUNDAI_ECUID_MODE_12 = 0x11,	//
	HYUNDAI_ECUID_MODE_13 = 0x12,	//
	HYUNDAI_ECUID_MODE_14 = 0x13,	//
	HYUNDAI_ECUID_MODE_15 = 0x14,	//
	HYUNDAI_ECUID_MODE_16 = 0x15,	//
	HYUNDAI_ECUID_MODE_17 = 0x16,	//
	HYUNDAI_ECUID_MODE_18 = 0x17,	//
	HYUNDAI_ECUID_MODE_19 = 0x18,	//
	HYUNDAI_ECUID_MODE_1A = 0x19,	//

}HYUNDAI_ECUID_MODE;


typedef enum MSG_ORD_HYUNDAI
{
	MSG_Init_CAN_PackType_Hyundai = 0x10, //
	MSG_Init_VPW_PackType_Hyundai = 0x11,
	MSG_Init_PWM_PackType_Hyundai = 0x12,
	MSG_Init_KWP_PackType_Hyundai = 0x13, //

	MSG_Init_Ecu_KWP_PackType_Hyundai = 0x23,

	MSG_ALL_CAN_PackType_Hyundai = 0x30, //
	MSG_ALL_VPW_PackType_Hyundai = 0x31,
	MSG_ALL_PWM_PackType_Hyundai = 0x32,
	MSG_ALL_KWP_PackType_Hyundai = 0x33, //

}MSG_ORD_HYUNDAI;

typedef struct SYS_MODE_SIZE_HYUNDAI
{
	uint8_t SYSNUM;//��ʼ������
	HYUNDAI_INIT_MODE INIT_MODE[16];
}SYS_MODE_SIZE_HYUNDAI;

typedef struct
{
	HYUNDAI_ECUID_MODE Hyundai_ECUID;//ƥ���ECUID
	uint8_t CMD_Read_DTC_Size;//�����������
	uint8_t CMD_Read_DTC[8][8];//�������� �����8����������
	uint8_t DTC_Pos[8];//����ķ�ʽ�Ͷ��������Ӧ��ȡ��ŵ�Posλ
	uint8_t CMD_Clear_DTC_Size;//�����������
	uint8_t CMD_Clear_DTC[8][8];//��������
} HYUNDAI_ECUID;

typedef struct
{
	HYUNDAI_ENTER_SYSTEM_MODE System_Enter_Mode;
	uint8_t System_Enter[8];//��ϵͳ����
	uint8_t ECUID_CMD[8];//ƥ��ECUID������
	uint8_t System_Exit[8];//��ϵͳ����
	HYUNDAI_ECUID_MODE Hyundai_ECUID;//ƥ���ECUID
} ENTER_SYSTEM_HYUNDAI;

typedef struct
{
	HYUNDAI_INIT_MODE Init_Mode;
	MSG_ORD_HYUNDAI Msg_Ord;
	uint8_t CMD_DATA[128];
	uint8_t enter_size;
	HYUNDAI_ENTER_SYSTEM_MODE Enter_System_mode[15];
} INIT_IO_CMD_HYUNDAI;



bool SetFrameTimeOut_Hyundai(HYUNDAI_INIT_MODE index);

void pro_msg_all_Hyundai(HYUNDAI_ENTER_SYSTEM_MODE mode);

bool Init_IO_Hyundai(HYUNDAI_INIT_MODE index);
intptr_t pro_much_init_hyundai(HYUNDAI_INIT_MODE Index, HYUNDAI_ENTER_SYSTEM_MODE mode);// ��ϵͳ
bool pro_canbus_select_module_hyundai(HYUNDAI_ENTER_SYSTEM_MODE mode);//CAN
bool pro_kwp_select_module_hyundai(HYUNDAI_ENTER_SYSTEM_MODE mode);//kwp��81��ϵͳ����

void pro_read_dtc_Hyundai(HYUNDAI_ECUID_MODE ECUID_mode, uint8_t iSystem, int* dtc_number, char* dtc_list, char* dtc_sys); //����
void pro_clear_dtc_Hyundai(HYUNDAI_ECUID_MODE ECUID_mode); //����
void pro_exit_system_Hyundai(HYUNDAI_ENTER_SYSTEM_MODE index); //�˳�ϵͳ


bool Much_Init_Hyundai(void);






#endif

