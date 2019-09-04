#ifndef _OBD_CMD_PROCESS_H_
#define _OBD_CMD_PROCESS_H_


#include "Fcar_Api.h"
#include <stdint.h>
#include <stdbool.h>


typedef enum OBDII_MSG_ORD
{
	OBDII_MSG_Init_CAN_PackType=0x10, //
	OBDII_MSG_Init_VPW_PackType=0x11,
	OBDII_MSG_Init_PWM_PackType=0x12,
	OBDII_MSG_Init_KWP_PackType=0x13, //
	
	OBDII_MSG_Init_Ecu_KWP_PackType=0x23,

	OBDII_MSG_ALL_CAN_PackType=0x30, //
	OBDII_MSG_ALL_VPW_PackType=0x31,
	OBDII_MSG_ALL_PWM_PackType=0x32,
	OBDII_MSG_ALL_KWP_PackType=0x33, //
	
}OBDII_MSG_ORD;

typedef enum OBDII_INIT_MODE
{
	OBDII_MODE_01=0x00, //
	OBDII_MODE_02=0x01, //
	OBDII_MODE_03=0x02, //
	OBDII_MODE_04=0x03, //
	OBDII_MODE_05=0x04, //
	OBDII_MODE_06=0x05, //
	OBDII_MODE_07=0x06, //
	OBDII_MODE_08=0x07, //
	OBDII_MODE_09=0x08, //

}OBDII_INIT_MODE;

//��ʼ��IO��������
typedef struct
{
	OBDII_INIT_MODE Init_Mode;
	OBDII_MSG_ORD Msg_Ord;
  uint8_t CMD_DATA[64];
} OBDII_CMD_INIT;

//�������������ṹ
typedef struct
{
  uint8_t CMD_DATA[8];
} OBDII_CMD_STREAMSCAN;


//��������������
typedef enum OBDII_STREAM_CMD_INDEX
{
	OBDII_READ_VIN      =0x01,         //vin
	OBDII_READ_CALID    =0x02,         //calid
	OBDII_READ_CVN      =0x03,         //cvn
	OBDII_READ_IUPR1     =0x04,         //iupr 
	OBDII_READ_IUPR2     =0x05,         //iupr 2
	OBDII_READ_READY    =0x06,         //��Ͼ���
	OBDII_READ_DTC      =0x07,         //����
	OBDII_READ_KILOMETRE=0x08,         //���
	OBDII_READ_SPEED    =0x09,         //����
	OBDII_READ_MAX_STORQUE = 0x0a,     //������������Ť��
	OBDII_READ_OUT_STORQUE = 0x0b,     //�����������Ť��
	OBDII_READ_FRICTION_STORQUE= 0x0c, //Ħ��Ť��
	OBDII_READ_ENGINE_SPEED = 0x0d,    //������ת��
	OBDII_READ_ENGINE_FLOW = 0x0e,     //������ȼ������
	OBDII_READ_IN_TEMPERATURE = 0x0f,  //SCR����¶�
	OBDII_READ_OUT_TEMPERATURE = 0x10, //SCR�����¶�
	OBDII_READ_PRESSURE_DIFF1 = 0x11,   //DPFѹ��1
	OBDII_READ_PRESSURE_DIFF2 = 0x12,   //DPFѹ��2
	OBDII_READ_INTAKE_VOLUME = 0x13,   //������
	OBDII_READ_REACTANT_SUIPLUS = 0x14,//��Ӧ������
	OBDII_READ_TANK_LEVEL = 0x15,      //����Һλ
	OBDII_READ_COOLANT_TEMPERATURE = 0x16, //��������ȴҺ�¶�1
//	OBDII_READ_COOLANT_TEMPERATURE2 = 0x17, //��������ȴҺ�¶�2
	OBDII_READ_FUEL_CONSUMPTION = 0x18,   //ȼ��������
	OBDII_READ_FUEL_INJECTION = 0x19,     //������
	OBDII_READ_VOLTAGE = 0x1a,          	//��ƿ��ѹ
	OBDII_READ_ENGINE_START = 0x1b,       //����������ʱ��
	OBDII_READ_ENGINE_STOP  = 0x1c,      	//������ֹͣʱ��
	OBDII_READ_FRONTOCURRENT1 = 0x1d,      //ǰ������
	OBDII_READ_FRONTOCURRENT2 = 0x1e,      //ǰ������
	OBDII_READ_FRONTOCURRENT3 = 0x1f,      //ǰ������
	OBDII_READ_FRONTOCURRENT4 = 0x20,      //ǰ������
	OBDII_READ_FRONTOCURRENT5 = 0x21,      //ǰ������
	OBDII_READ_FRONTOCURRENT6 = 0x22,      //ǰ����ѹ
	OBDII_READ_FRONTOVOLTAGE1 = 0x23,      //ǰ����ѹ
	OBDII_READ_FRONTOVOLTAGE2 = 0x24,      //ǰ����ѹ
	OBDII_READ_FRONTOVOLTAGE3 = 0x25,      //ǰ����ѹ
	OBDII_READ_FRONTOVOLTAGE4 = 0x26,      //ǰ����ѹ
	OBDII_READ_FRONTOVOLTAGE5 = 0x27,      //ǰ����ѹ
	OBDII_READ_FRONTOVOLTAGE6 = 0x28,      //ǰ����ѹ
	OBDII_READ_AFTEROCURRENT1 = 0x29,      //��������
	OBDII_READ_AFTEROCURRENT2 = 0x2a,      //��������
	OBDII_READ_AFTEROCURRENT3 = 0x2b,      //��������
	OBDII_READ_AFTEROCURRENT4 = 0x2c,      //��������
	OBDII_READ_AFTEROCURRENT5 = 0x2d,      //��������
	OBDII_READ_AFTEROCURRENT6 = 0x2e,      //��������
	OBDII_READ_AFTEROVOLTAGE1 = 0x2f,      //������ѹ
	OBDII_READ_AFTEROVOLTAGE2 = 0x30,      //������ѹ
	OBDII_READ_AFTEROVOLTAGE3 = 0x31,      //������ѹ
	OBDII_READ_AFTEROVOLTAGE4 = 0x32,      //������ѹ
	OBDII_READ_AFTEROVOLTAGE5 = 0x33,      //������ѹ
	OBDII_READ_AFTEROVOLTAGE6 = 0x34,      //������ѹ
	OBDII_READ_ENGINELOAD			= 0x35,
	OBDII_READ_AIRINPRESSURE  = 0x36,
	OBDII_READ_AIRINTEMPERATURE = 0x37
}OBDII_STREAM_CMD_INDEX;

//����������ṹ
typedef struct
{
	OBDII_STREAM_CMD_INDEX  Cmd_Index;
  uint8_t CMD_DATA[16];
} OBDII_STREAM_CMD;

bool OBDII_Much_Init(void);
bool OBDII_Init_IO(OBDII_INIT_MODE index);

bool OBDII_SetFrameTimeOut(OBDII_INIT_MODE index);
void ResetComPort();
bool pro_much_init(OBDII_INIT_MODE index);
bool pro_canbus_select_module(OBDII_INIT_MODE mode);
bool pro_default_select_module(OBDII_INIT_MODE mode);
void pro_set_init_ecu_ord(OBDII_INIT_MODE mode);
void pro_msg_all(OBDII_INIT_MODE mode);
void pro_set_can_id(OBDII_INIT_MODE mode);
bool pro_initecu_much_5bps(uint8_t *sendbuf);
bool pro_initecu_much(uint8_t *sendbuf);
void pro_autokeep_obdii();
void SetCanbusFilter(uint32_t filterID);

//void pro_open_keep(OBDII_INIT_MODE index);
void pro_read_dtc(OBDII_INIT_MODE index,int* dtc_number,char* dtc_list); //����
void pro_read_info(OBDII_INIT_MODE index); //���汾��Ϣ
void pro_read_data(void); //��������

void pro_get_result(OBDII_STREAM_CMD_INDEX index,uint8_t *recvbuf,uint16_t number,uint8_t pos,char *result);
void pro_read_iupr1(uint8_t *recvbuf,uint16_t number,char* buf);
void pro_read_iupr2(uint8_t *recvbuf,uint16_t number,char* buf);
void pro_read_ready(uint8_t *recvbuf,uint16_t number,char* buf);
void pro_read_kilometet(uint8_t *recvbuf,uint16_t number);
void pro_read_speed(uint8_t *recvbuf,uint16_t number);
void pro_read_max_storque(uint8_t *recvbuf,uint16_t number);
void cmd_msg_all_canbus_11bit();
void pro_obdii_protocol_init(OBDII_INIT_MODE model);
void pro_read_vin(uint8_t *recvbuf,uint16_t number,uint8_t pos);
void pro_read_calid(uint8_t *recvbuf,uint16_t number);  //�� calid
void pro_read_cvn(uint8_t *recvbuf,uint16_t number,uint8_t pos);    //��cvn
void pro_obdii_getpid_diag();                           //���������
void pro_obdii_getpid_infotype(OBDII_INIT_MODE index);  //�汾��Ϣ���
void reset_global_data();
void cmd_msg_all_default_model();
uint8_t cmd_set_ecumodule(uint8_t module);

//�������5���ӿ�
void get_vehicle_vin(char* vin);
void get_ready_status(char* buf);
void get_mil_status(char status);
void get_vehicle_calid(char* calid);
void get_vehicle_cvn(char* calid);
void get_dtcnumber(int* dtcnumber);
void get_dtc_list(char* dtc_list);
void get_ready_status(char* buf);
void get_vehicle_iupr(OBDII_INIT_MODE index,char* buf);
void get_agreement(char* index);
#endif

