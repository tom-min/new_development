<<<<<<< .mine
/**
  ******************************************************************************
  * @file    I2C/I2C_TwoBoards/MasterReceiverInterrupt/main.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    13-April-2012
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
 
#include "user_queue.h"
  
typedef enum OBDII_PGN
{
	//////////////////////////////////////////////////////////////	
	OBDII_ENGINE_SPEED_PGN=0x00F004,//��ȡ������ת��
	
	OBDII_BATTRY_VOLTAGE_PGN=0x00FEF7,//��ȡ������ת��

	OBDII_ENGINE_START_TIME_PGN=0x00FDB8,//��ȡ������ת��
	
	OBDII_ENGINE_STOP_TIME_PGN=0xFFFF,//��ȡ������ת��

	OBDII_GAS_COMSUME_PGN=0x00FEF2,//��ȡȼ��������
	
	OBDII_INJECTON_QUANTITY_PGN=0xFFFF, // ������
	
	OBDII_Engine_Coolant_Temperature_PGN=0x00FEEE,////��ȡ������׼ȷ�¶�
	
	OBDII_Fuel_Tank_PGN=0x00FEEF,//��ȡ����Һλ
	
	OBDII_Reactant_PGN=0xFFFF,//��ȡ����Һ��Ӧ����

	OBDII_AIR_INPUT_Quantity_PGN=0x00F00A,//������
	
	OBDII_DPF1Differential_Pressure_PGN=0x00FDB2,//DPF1ѹ��
	
	OBDII_DPF2Differential_Pressure_PGN=0x00FDAF,//DPF2ѹ��
	
	OBDII_AfterTreat1_SCR_intake_temperature_PGN=0x00FD3E ,//����1 SCR�����¶�
	
	OBDII_AfterTreat2_SCR_intake_temperature_PGN=0x00FD38,//����2 SCR�����¶�
	
	OBDII_AfterTreat1_SCR_Output_Temperature_PGN=0x00FD3E,//����1 SCR�����¶�
	
	OBDII_AfterTreat2_SCR_Output_Temperature_PGN=0x00FD38,//����2 SCR�����¶�
	
	OBDII_Engine_Fuel_PGN=0x00FE81,//������ȼ������
	
	OBDII_Friction_Torque_PGN=0x00FEDF,//Ħ��Ť��
	
	OBDII_Engine_Net_Output_Torque_PGN=0x00F004,//�����������Ť��
	
	OBDII_Engine_Bench_Mark_Torque_PGN=0x00F003,//����������׼Ť��
	
	OBDII_Speed_PGN=0x00FE6C,//����
	
	OBDII_Kilometres_PGN=0x00FEE0,//������

} OBDII_PGN;


typedef struct
{
  uint8_t PGN_INDEX;
  OBDII_PGN PGN;
  
} OBDII_PGN_Type;



OBDII_PGN_Type OBDII_PGN_DATA[]=\
{\
	{0,OBDII_ENGINE_SPEED_PGN},\
	{0,OBDII_ENGINE_SPEED_PGN},\
	
	{1,OBDII_BATTRY_VOLTAGE_PGN},\
	{1,OBDII_BATTRY_VOLTAGE_PGN},\
	
	{2,OBDII_ENGINE_START_TIME_PGN},\
	{2,OBDII_ENGINE_START_TIME_PGN},\
	
	{3,OBDII_ENGINE_STOP_TIME_PGN},\
	{3,OBDII_ENGINE_STOP_TIME_PGN},\
	
	{4,OBDII_GAS_COMSUME_PGN},\
	{4,OBDII_GAS_COMSUME_PGN},\
	
	{5,OBDII_INJECTON_QUANTITY_PGN},\
	{5,OBDII_INJECTON_QUANTITY_PGN},\
	
	{6,OBDII_Engine_Coolant_Temperature_PGN},\
	{6,OBDII_Engine_Coolant_Temperature_PGN},\
	
	{7,OBDII_Fuel_Tank_PGN},\
	{7,OBDII_Fuel_Tank_PGN},\
	
	{8,OBDII_Reactant_PGN},\
	{8,OBDII_Reactant_PGN},\
	{9,OBDII_AIR_INPUT_Quantity_PGN},\
	{9,OBDII_AIR_INPUT_Quantity_PGN},\
	
	{10,OBDII_DPF1Differential_Pressure_PGN},\
	{10,OBDII_DPF1Differential_Pressure_PGN},\
	
	{11,OBDII_DPF2Differential_Pressure_PGN},\
	{11,OBDII_DPF2Differential_Pressure_PGN},\
	
	{12,OBDII_AfterTreat1_SCR_intake_temperature_PGN},\
	{12,OBDII_AfterTreat1_SCR_intake_temperature_PGN},\
	{13,OBDII_AfterTreat2_SCR_intake_temperature_PGN},\
	{13,OBDII_AfterTreat2_SCR_intake_temperature_PGN},\
		
	{14,OBDII_AfterTreat1_SCR_Output_Temperature_PGN},\
	{14,OBDII_AfterTreat1_SCR_Output_Temperature_PGN},\
	
	{15,OBDII_AfterTreat2_SCR_Output_Temperature_PGN},\
	{15,OBDII_AfterTreat2_SCR_Output_Temperature_PGN},\
		
	{16,OBDII_Engine_Fuel_PGN},\
	{16,OBDII_Engine_Fuel_PGN},\
	
	{17,OBDII_Friction_Torque_PGN},\
	{17,OBDII_Friction_Torque_PGN},\
	
	{18,OBDII_Engine_Net_Output_Torque_PGN},\
	{18,OBDII_Engine_Net_Output_Torque_PGN},\
	
	{19,OBDII_Engine_Bench_Mark_Torque_PGN},\
	{19,OBDII_Engine_Bench_Mark_Torque_PGN},\
	
	{20,OBDII_Speed_PGN},\
	{20,OBDII_Speed_PGN},\
		
	{21,OBDII_Kilometres_PGN},\
       {21,OBDII_Kilometres_PGN},\
};





typedef enum OBDII_CMD_INDEX
{
	OBDII_INIT_IO=0x0000,
	//////////////////////////////////////////////////////////////	
	OBDII_SET_VIN_FILTER=0x0100,//����VIN
	OBDII_READ_VIN=0x0001,//����VIN
	
	OBDII_SET_CRUUENT_FILTER=0x0102,//��ȡ��ǰ������
	OBDII_READ_CRUUENT_DTC=0x0002,//��ȡ��ǰ������
	
	OBDII_SET_HISTORY_DTC_FILTER=0x0103,//��ȡ��ʷ������
	OBDII_READ_HISTORY_DTC=0x0003,//��ȡ��ǰ������
	
	//////////////////////////////////////////////////////////////	
	OBDII_SET_ENGINE_SPEED_PGN_FILTER=0x0104,//��ȡ������ת��
	OBDII_READ_ENGINE_SPEED=0x0004,//��ȡ��ǰ������

	
	OBDII_SET_BATTRY_VOLTAGE_PGN_FILTER=0x0105,//��ȡ������ת��
	OBDII_READ_BATTRY_VOLTAGE=0x0005,//��ȡ������ת��
	
	OBDII_SET_ENGINE_START_TIME_PGN_FILTER=0x0106,//��ȡ������ת��
	OBDII_READ_ENGINE_START_TIME=0x0006,//��ȡ������ת��

	
	OBDII_SET_ENGINE_STOP_TIME_PGN_FILTER=0x0107,//��ȡ������ת��
	OBDII_READ_ENGINE_STOP_TIME=0x0007,//��ȡ������ת��

	OBDII_SET_GAS_COMSUME_PGN_FILTER=0x0108,//��ȡȼ��������
	OBDII_READ_GAS_COMSUME=0x0008,//��ȡȼ��������
	
	OBDII_SET_INJECTON_QUANTITY_PGN_FILTER=0x0109, // ������
	OBDII_READ_INJECTON_QUANTITY=0x0009, // ������
	
	OBDII_SET_Engine_Coolant_Temperature_PGN_FILTER=0x010A,////��ȡ������׼ȷ�¶�
	OBDII_READ_Engine_Coolant_Temperature=0x000A,////��ȡ������׼ȷ�¶�
	
	OBDII_SET_Fuel_Tank_PGN_FILTER=0x010B,//��ȡ����Һλ
	OBDII_READ_Fuel_Tank=0x000B,//��ȡ����Һλ

	
	OBDII_SET_Reactant_PGN_FILTER=0x010C,//��ȡ����Һ��Ӧ����
	OBDII_READ_Reactant=0x000C,//��ȡ����Һ��Ӧ����

	
	OBDII_SET_AIR_INPUT_Quantity_PGN_FILTER=0x010D,//������
	OBDII_READ_AIR_INPUT_Quantity=0x000D,//������
	
	OBDII_SET_DPF1Differential_Pressure_PGN_FILTER=0x010E,//DPF1ѹ��
	OBDII_READ_DPF1Differential_Pressure=0x000E,//DPF1ѹ��
	
	OBDII_SET_DPF2Differential_Pressure_PGN_FILTER=0x010F,//DPF2ѹ��
	OBDII_READ_DPF2Differential_Pressure=0x000F,//DPF2ѹ��
	
	OBDII_SET_AfterTreat1_SCR_intake_temperature_PGN_FILTER=0x0110 ,//����1 SCR�����¶�
	OBDII_READ_AfterTreat1_SCR_intake_temperature=0x0010 ,//����1 SCR�����¶�
	
	OBDII_SET_AfterTreat2_SCR_intake_temperature_PGN_FILTER=0x0111,//����2 SCR�����¶�
	OBDII_READ_AfterTreat2_SCR_intake_temperature=0x0011,//����2 SCR�����¶�
	
	OBDII_SET_AfterTreat1_SCR_Output_Temperature_PGN_FILTER=0x0112,//����1 SCR�����¶�
	OBDII_READ_AfterTreat1_SCR_Output_Temperature=0x0012,//����1 SCR�����¶�
	
	OBDII_SET_AfterTreat2_SCR_Output_Temperature_PGN_FILTER=0x0113,//����2 SCR�����¶�
	OBDII_READ_AfterTreat2_SCR_Output_Temperature=0x0013,//����2 SCR�����¶�
	
	OBDII_SET_Engine_Fuel_PGN_FILTER=0x0114,//������ȼ������
	OBDII_READ_Engine_Fuel=0x0014,//������ȼ������
	
	OBDII_SET_Friction_Torque_PGN_FILTER=0x0115,//Ħ��Ť��
	OBDII_READ_Friction_Torque=0x0015,//Ħ��Ť��
	
	OBDII_SET_Engine_Net_Output_Torque_PGN_FILTER=0x0116,//�����������Ť��
	OBDII_READ_Engine_Net_Output_Torque=0x0016,//�����������Ť��
	
	OBDII_SET_Engine_Bench_Mark_Torque_PGN_FILTER=0x0117,//����������׼Ť��
	OBDII_READ_Engine_Bench_Mark_Torque=0x0017,//����������׼Ť��
	
	OBDII_SET_Speed_PGN_FILTER=0x0118,//����
	OBDII_READ_Speed=0x0018,//����
	
	OBDII_SET_Kilometres_PGN_FILTER=0x0119,//������
	OBDII_READ_Kilometres=0x0019,//������

} OBDII_CMD_INDEX;

typedef struct
{
  OBDII_CMD_INDEX CMD_INDEX;
  uint8_t CMD_DATA[64];
  
} OBDII_CMD_Type;


const OBDII_CMD_Type ObdiiCmd[]=\
{\
/////////////////////////////////////////////////////////////////
	{\
    {OBDII_INIT_IO},\
	//{0xA5,0xA5,0x00,0x2d,0x10,0x01,0x03,0xd0,0x90,0x02,0x80,0x03,0x10,0x04,0x00,0x00,0x00,0x00,0x05,0x98,0xee,0xff,0x00,0x06,0x1c,0xff,0xff,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0x01,0xff}
{0xa5,0xa5,0x00,0x28 ,0x13 ,0x01 ,0x00 ,0x00 ,0x00 ,0x02 ,0x00 ,0x03 ,0x80 ,0x04 ,0x00 ,0x77 ,0xc1 ,0xc1 ,0x05 ,0x08 ,0x0a ,0x0a ,0x03 ,0x03 ,0xe8 ,0x19 ,0x06 ,0x01 ,0x07 ,0x00 ,0x08 ,0x25 ,0x09 ,0x03 ,0x0a ,0x00 ,0x00 ,0x00 ,0x0b ,0x00 ,0x0c ,0x00 ,0x00 ,0x00 ,0xb1}

    },\
////////////////////////////////////////////
	{\
    {OBDII_SET_VIN_FILTER},\
	{0xa5,0xa5,0x00,0x1d,0x70,0x98,0xfe,0xec,0x00,0x98,0xec,0xf9,0x00,0x98,0xeb,0xf9,0x00,0x98,0xe8,0xf9,0x00,0x98,0xec,0xff,0x00,0x98,0xeb,0xff,0x00,0x98,0xe8,0xff,0x00,0xfa}
    },\
	
	{\
    {OBDII_READ_VIN},\
	{0xa5,0xa5,0x00,0x08,0x30,0x98,0xea,0xff,0xf9,0xec,0xfe,0x00,0x63}
    },\
	{\
    {OBDII_SET_CRUUENT_FILTER},\
	{0xa5,0xa5,0x00,0x1d,0x70,0x98,0xfe,0xca,0x00,0x98,0xec,0xf9,0x00,0x98,0xeb,0xf9,0x00,0x98,0xe8,0xf9,0x00,0x98,0xec,0xff,0x00,0x98,0xeb,0xff,0x00,0x98,0xe8,0xff,0x00,0x1c}
    },\
	{\
    {OBDII_READ_CRUUENT_DTC},\
	{0xa5,0xa5,0x00,0x08,0x30,0x98,0xea,0xff,0xf9,0xca,0xfe,0x00,0x85}
    },\
	{\
    {OBDII_SET_HISTORY_DTC_FILTER},\
 	{0xa5,0xa5,0x00,0x1d,0x70,0x98,0xfe,0xcb,0x00,0x98,0xec,0xf9,0x00,0x98,0xeb,0xf9,0x00,0x98,0xe8,0xf9,0x00,0x98,0xec,0xff,0x00,0x98,0xeb,0xff,0x00,0x98,0xe8,0xff,0x00,0x1b}
     },\
	{\
    {OBDII_READ_HISTORY_DTC},\
	{0xa5,0xa5,0x00,0x08,0x30,0x98,0xea,0xff,0xf9,0xcb,0xfe,0x00,0x84}
    },\
///////////////////////////////////////////////////////////////////////	
	///0//
	{\
    {OBDII_SET_ENGINE_SPEED_PGN_FILTER},\
	{ 0xa5,0xa5,0x00,0x0d,0x70,0x98,0xf0,0x04,0x00,0x98,0xe8,0xf9,0x00,0x98,0xe8,0xff,0x00,0xfe}
    },\
	{\
    {OBDII_READ_ENGINE_SPEED},\
	{0xa5,0xa5,0x00,0x08,0x30,0x98,0xea,0xff,0xf9,0x04,0xf0,0x00,0x59}
    },\
	
	///1//
	{\
    {OBDII_SET_BATTRY_VOLTAGE_PGN_FILTER},\
	{ 0xa5,0xa5,0x00,0x0d,0x70,0x98,0xf0,0x04,0x00,0x98,0xe8,0xf9,0x00,0x98,0xe8,0xff,0x00,0xfe}
    },\
	
	{\
    {OBDII_READ_BATTRY_VOLTAGE},\
	 {0xa5,0xa5,0x00,0x08,0x30,0x98,0xea,0xff,0xf9,0x04,0xf0,0x00,0x59}
    },\
	///2///
	{\
    {OBDII_SET_ENGINE_START_TIME_PGN_FILTER},\
	{ 0xa5,0xa5,0x00,0x0d,0x70,0x98,0xf0,0x04,0x00,0x98,0xe8,0xf9,0x00,0x98,0xe8,0xff,0x00,0xfe}
    },\
	{\
    {OBDII_READ_ENGINE_START_TIME},\
	{0xa5,0xa5,0x00,0x08,0x30,0x98,0xea,0xff,0xf9,0x04,0xf0,0x00,0x59}
    },\
	//3//
	{\
    {OBDII_SET_ENGINE_STOP_TIME_PGN_FILTER},\
	{ 0xa5,0xa5,0x00,0x0d,0x70,0x98,0xf0,0x04,0x00,0x98,0xe8,0xf9,0x00,0x98,0xe8,0xff,0x00,0xfe}
    },\
	{\
    {OBDII_READ_ENGINE_STOP_TIME},\
	{0xa5,0xa5,0x00,0x08,0x30,0x98,0xea,0xff,0xf9,0x04,0xf0,0x00,0x59}
    },\
	///4/
	{\
    {OBDII_SET_GAS_COMSUME_PGN_FILTER},\
	{ 0xa5,0xa5,0x00,0x0d,0x70,0x98,0xf0,0x04,0x00,0x98,0xe8,0xf9,0x00,0x98,0xe8,0xff,0x00,0xfe}
    },\
	{\
    {OBDII_READ_GAS_COMSUME},\
	 {0xa5,0xa5,0x00,0x08,0x30,0x98,0xea,0xff,0xf9,0x04,0xf0,0x00,0x59}
    },\
	//5//
	{\
    {OBDII_SET_INJECTON_QUANTITY_PGN_FILTER},\
	{ 0xa5,0xa5,0x00,0x0d,0x70,0x98,0xf0,0x04,0x00,0x98,0xe8,0xf9,0x00,0x98,0xe8,0xff,0x00,0xfe}
    },\
	{\
    {OBDII_READ_INJECTON_QUANTITY},\
	 {0xa5,0xa5,0x00,0x08,0x30,0x98,0xea,0xff,0xf9,0x04,0xf0,0x00,0x59}
    },\
	
	
	///6///
	
	{\
    {OBDII_SET_Engine_Coolant_Temperature_PGN_FILTER},\
	{ 0xa5,0xa5,0x00,0x0d,0x70,0x98,0xf0,0x04,0x00,0x98,0xe8,0xf9,0x00,0x98,0xe8,0xff,0x00,0xfe}
    },\
	{\
    {OBDII_READ_Engine_Coolant_Temperature},\
	 {0xa5,0xa5,0x00,0x08,0x30,0x98,0xea,0xff,0xf9,0x04,0xf0,0x00,0x59}
    },\
	///7///
	
	{\
    {OBDII_SET_Fuel_Tank_PGN_FILTER},\
	{ 0xa5,0xa5,0x00,0x0d,0x70,0x98,0xf0,0x04,0x00,0x98,0xe8,0xf9,0x00,0x98,0xe8,0xff,0x00,0xfe}
    },\
	
	{\
    {OBDII_READ_Fuel_Tank},\
     {0xa5,0xa5,0x00,0x08,0x30,0x98,0xea,0xff,0xf9,0x04,0xf0,0x00,0x59}
    },\
	////8/////
	{\
    {OBDII_SET_Reactant_PGN_FILTER},\
	{ 0xa5,0xa5,0x00,0x0d,0x70,0x98,0xf0,0x04,0x00,0x98,0xe8,0xf9,0x00,0x98,0xe8,0xff,0x00,0xfe}
    },\
	{\
    {OBDII_READ_Reactant},\
	 {0xa5,0xa5,0x00,0x08,0x30,0x98,0xea,0xff,0xf9,0x04,0xf0,0x00,0x59}
    },\
	///9////
	{\
    {OBDII_SET_AIR_INPUT_Quantity_PGN_FILTER},\
	{ 0xa5,0xa5,0x00,0x0d,0x70,0x98,0xf0,0x04,0x00,0x98,0xe8,0xf9,0x00,0x98,0xe8,0xff,0x00,0xfe}
    },\
		{\
    {OBDII_READ_AIR_INPUT_Quantity},\
	 {0xa5,0xa5,0x00,0x08,0x30,0x98,0xea,0xff,0xf9,0x04,0xf0,0x00,0x59}
    },\
	////10////
	{\
    {OBDII_SET_DPF1Differential_Pressure_PGN_FILTER},\
    { 0xa5,0xa5,0x00,0x0d,0x70,0x98,0xf0,0x04,0x00,0x98,0xe8,0xf9,0x00,0x98,0xe8,0xff,0x00,0xfe}
    },\
	{\
    {OBDII_READ_DPF1Differential_Pressure},\
	 {0xa5,0xa5,0x00,0x08,0x30,0x98,0xea,0xff,0xf9,0x04,0xf0,0x00,0x59}
    },\
	/////11////
	{\
    {OBDII_SET_DPF2Differential_Pressure_PGN_FILTER},\
	{ 0xa5,0xa5,0x00,0x0d,0x70,0x98,0xf0,0x04,0x00,0x98,0xe8,0xf9,0x00,0x98,0xe8,0xff,0x00,0xfe}
    },\
	{\
    {OBDII_READ_DPF2Differential_Pressure},\
	{0xa5,0xa5,0x00,0x08,0x30,0x98,0xea,0xff,0xf9,0x04,0xf0,0x00,0x59}
    },\
	////12/////
	{\
    {OBDII_SET_AfterTreat1_SCR_intake_temperature_PGN_FILTER},\
	{ 0xa5,0xa5,0x00,0x0d,0x70,0x98,0xf0,0x04,0x00,0x98,0xe8,0xf9,0x00,0x98,0xe8,0xff,0x00,0xfe}
    },\
	{\
    {OBDII_READ_AfterTreat1_SCR_intake_temperature},\
    {0xa5,0xa5,0x00,0x08,0x30,0x98,0xea,0xff,0xf9,0x04,0xf0,0x00,0x59}
    },\
	////13////
	{\
    {OBDII_SET_AfterTreat2_SCR_intake_temperature_PGN_FILTER},\
	{ 0xa5,0xa5,0x00,0x0d,0x70,0x98,0xf0,0x04,0x00,0x98,0xe8,0xf9,0x00,0x98,0xe8,0xff,0x00,0xfe}
    },\
	{\
    {OBDII_READ_AfterTreat2_SCR_intake_temperature},\
	{0xa5,0xa5,0x00,0x08,0x30,0x98,0xea,0xff,0xf9,0x04,0xf0,0x00,0x59}
    },\
	////14////
	{\
    {OBDII_SET_AfterTreat1_SCR_Output_Temperature_PGN_FILTER},\
    { 0xa5,0xa5,0x00,0x0d,0x70,0x98,0xf0,0x04,0x00,0x98,0xe8,0xf9,0x00,0x98,0xe8,0xff,0x00,0xfe}
    },\
	{\
    {OBDII_READ_AfterTreat1_SCR_Output_Temperature},\
	 {0xa5,0xa5,0x00,0x08,0x30,0x98,0xea,0xff,0xf9,0x04,0xf0,0x00,0x59}
    },\
	////15////
	{\
    {OBDII_SET_AfterTreat2_SCR_Output_Temperature_PGN_FILTER},\
	{ 0xa5,0xa5,0x00,0x0d,0x70,0x98,0xf0,0x04,0x00,0x98,0xe8,0xf9,0x00,0x98,0xe8,0xff,0x00,0xfe}
    },\
	{\
    {OBDII_READ_AfterTreat2_SCR_Output_Temperature},\
	 {0xa5,0xa5,0x00,0x08,0x30,0x98,0xea,0xff,0xf9,0x04,0xf0,0x00,0x59}
    },\
	///16///
	{\
    {OBDII_SET_Engine_Fuel_PGN_FILTER},\
	{ 0xa5,0xa5,0x00,0x0d,0x70,0x98,0xf0,0x04,0x00,0x98,0xe8,0xf9,0x00,0x98,0xe8,0xff,0x00,0xfe}
    },\
	{\
    {OBDII_READ_Engine_Fuel},\
	 {0xa5,0xa5,0x00,0x08,0x30,0x98,0xea,0xff,0xf9,0x04,0xf0,0x00,0x59}
    },\
	///17//
	{\
    {OBDII_SET_Friction_Torque_PGN_FILTER},\
	 { 0xa5,0xa5,0x00,0x0d,0x70,0x98,0xf0,0x04,0x00,0x98,0xe8,0xf9,0x00,0x98,0xe8,0xff,0x00,0xfe}
    },\
	
	{\
    {OBDII_READ_Friction_Torque},\
	 {0xa5,0xa5,0x00,0x08,0x30,0x98,0xea,0xff,0xf9,0x04,0xf0,0x00,0x59}
    },\
	///18//
	{\
    {OBDII_SET_Engine_Net_Output_Torque_PGN_FILTER},\
	{ 0xa5,0xa5,0x00,0x0d,0x70,0x98,0xf0,0x04,0x00,0x98,0xe8,0xf9,0x00,0x98,0xe8,0xff,0x00,0xfe}
    },\
	{\
    {OBDII_READ_Engine_Net_Output_Torque},\
	 {0xa5,0xa5,0x00,0x08,0x30,0x98,0xea,0xff,0xf9,0x04,0xf0,0x00,0x59}
    },\
	
	///19//
	{\
    {OBDII_SET_Engine_Bench_Mark_Torque_PGN_FILTER},\
	{ 0xa5,0xa5,0x00,0x0d,0x70,0x98,0xf0,0x04,0x00,0x98,0xe8,0xf9,0x00,0x98,0xe8,0xff,0x00,0xfe}
    },\
	{\
    {OBDII_READ_Engine_Bench_Mark_Torque},\
	 {0xa5,0xa5,0x00,0x08,0x30,0x98,0xea,0xff,0xf9,0x04,0xf0,0x00,0x59}
    },\
	
	
	
	/////20//////
	{\
    {OBDII_SET_Speed_PGN_FILTER},\
	{ 0xa5,0xa5,0x00,0x0d,0x70,0x98,0xf0,0x04,0x00,0x98,0xe8,0xf9,0x00,0x98,0xe8,0xff,0x00,0xfe}
    },\
	{\
    {OBDII_READ_Speed},\
	 {0xa5,0xa5,0x00,0x08,0x30,0x98,0xea,0xff,0xf9,0x04,0xf0,0x00,0x59}
    },\
	/////21//////
	{\
    {OBDII_SET_Kilometres_PGN_FILTER},\
	{ 0xa5,0xa5,0x00,0x0d,0x70,0x98,0xf0,0x04,0x00,0x98,0xe8,0xf9,0x00,0x98,0xe8,0xff,0x00,0xfe}
    },\
	{\
    {OBDII_READ_Kilometres},\
	 {0xa5,0xa5,0x00,0x08,0x30,0x98,0xea,0xff,0xf9,0x04,0xf0,0x00,0x59}
    },\

};


  
void OBDII_SEND(uint8_t index)
{
	uint8_t *p_data;
	uint8_t p_data_len = 0;
	uint8_t len;
	uint8_t i,chacknum=0;
	uint32_t pgn;
	uint8_t cmd_data_buff[64];

	p_data=(uint8_t *)ObdiiCmd[index].CMD_DATA;
	memset(cmd_data_buff,0,sizeof(cmd_data_buff));
	memcpy(cmd_data_buff,p_data,sizeof(cmd_data_buff));
	p_data=cmd_data_buff;
	
	p_data_len=cmd_data_buff[3]+5;

    len = ( p_data[2]<<8) +  p_data[3];
	
    for(i=0; i<len + 2; i++)
    {
	    chacknum +=  (uint8_t)p_data[i+2];
    }
     
	p_data[i+2] = ~chacknum;
	//printfuartdebug( "p_data[i+2]==%02x len==%d \r\n", p_data[i+2]);
	USB_BULK_Queue_Write(&USB_RX_QueueData,p_data,p_data_len);
}
uint8_t OBDII_RECEIVE(uint8_t obdii_outtime,uint8_t *p_obdii_buff,uint16_t obdii_buff_len)
{


	while(obdii_outtime--) 
	{
		if (USB_BULK_Queue_Read(&OBD_RX_QueueData, p_obdii_buff, &obdii_buff_len) == true) 
		{
		      
			return 0;
		}
		else
		{
		      
			vTaskDelay(2);
		}
	}
	return 1;
	
}

void OBDII_SEND_PGN_FILTER(uint8_t index)
{
	uint8_t *p_data;
	uint8_t seqnum = 0;
	uint32_t pgn;
	uint8_t cmd_data_buff[64];
	uint8_t len;
	uint8_t i,chacknum=0;
	
	p_data=(uint8_t *)ObdiiCmd[index].CMD_DATA;
	memset(cmd_data_buff,0,sizeof(cmd_data_buff));
	memcpy(cmd_data_buff,p_data,sizeof(cmd_data_buff));
	
	pgn = OBDII_PGN_DATA[index-7].PGN;

     // printfuartdebug("pgn==%08x index-7==%d\r\n",pgn,index-7); 	
	
	
	cmd_data_buff[6]=pgn>>8;
	cmd_data_buff[7]=pgn>>0;
	//cmd_data_buff[8]=pgn;

	p_data=cmd_data_buff;
	
	seqnum=cmd_data_buff[3]+5;

    len = ( p_data[2]<<8) +  p_data[3];
	
    for(i=0; i<len + 2; i++)
    {
	    chacknum +=  (uint8_t)p_data[i+2];
    }
     
	p_data[i+2] = ~chacknum;

	
	
	USB_BULK_Queue_Write(&USB_RX_QueueData,p_data,seqnum);
}
void OBDII_SEND_PGN(uint8_t index)
{
	uint8_t *p_data;
	uint8_t seqnum = 0;
	uint32_t pgn;
	uint8_t cmd_data_buff[64];
	uint8_t len;
	uint8_t i,chacknum=0;
	
	p_data=(uint8_t *)ObdiiCmd[index].CMD_DATA;
	memset(cmd_data_buff,0,sizeof(cmd_data_buff));
	memcpy(cmd_data_buff,p_data,sizeof(cmd_data_buff));
	
	pgn = OBDII_PGN_DATA[index-7].PGN;
	
	cmd_data_buff[9]=pgn>>0;
	cmd_data_buff[10]=pgn>>8;
	//cmd_data_buff[8]=pgn;

	p_data=cmd_data_buff;
	
	seqnum=cmd_data_buff[3]+5;


    len = ( p_data[2]<<8) +  p_data[3];
	
    for(i=0; i<len + 2; i++)
    {
	    chacknum +=  (uint8_t)p_data[i+2];
    }
     
    p_data[i+2] = ~chacknum;


USB_BULK_Queue_Write(&USB_RX_QueueData,p_data,seqnum);
}


//////////////////////////////////////////////////////////
void OBDII_INIT_IO_FUNCTON(uint8_t index)
{
	uint8_t obdii_outtime=10;
	uint8_t obdii_buff[64];
	uint16_t obdii_buff_len = 0;
	static uint8_t  gobdinitsuccess=0;
	if(gobdinitsuccess==1) return;
	OBDII_SEND(index);
	vTaskDelay(100);
	
	if(OBDII_RECEIVE(obdii_outtime,obdii_buff,obdii_buff_len)==0)
	{
	   //gobdinitsuccess=1;   
	   //return;
	}
	//else
	{
	 	//printfuartdebug("OBDII_INIT_IO_FUNCTON error\r\n");
	}
}
//////////////////////////////////////////////////////////////	
void OBDII_SET_VIN_FILTER_FUNCTON(uint8_t index)//����VIN
{
	OBDII_SEND(index);
}
void OBDII_READ_VIN_FUNCTON(uint8_t index)//����VIN
{
	uint8_t obdii_outtime=300;
	uint8_t obdii_buff[64];
	uint16_t obdii_buff_len = 0;
	
	OBDII_SEND(index);
	
	if(OBDII_RECEIVE(obdii_outtime,obdii_buff,obdii_buff_len)==0)
	{
	        //  printfuartdebug("OBDII_READ_VIN_FUNCTON\r\n");
	}
}

void OBDII_SET_CRUUENT_DTC_FILTER_FUNCTON(uint8_t index)//��ȡ��ǰ������
{	
	OBDII_SEND(index);

}
void OBDII_READ_CRUUENT_DTC_FUNCTON(uint8_t index)//��ȡ��ǰ������
{
	uint8_t obdii_outtime=10;
	uint8_t obdii_buff[64];
	uint16_t obdii_buff_len = 0;
	
	OBDII_SEND(index);

	if(OBDII_RECEIVE(obdii_outtime,obdii_buff,obdii_buff_len)==0)
	{
	   // printfuartdebug("OBDII_READ_CRUUENT_DTC_FUNCTON\r\n");
	}
}

void OBDII_SET_HISTORY_DTC_FILTER_FUNCTON(uint8_t index)//��ȡ��ʷ������
{
	OBDII_SEND(index);
}
void OBDII_READ_HISTORY_DTC_FUNCTON(uint8_t index)//��ȡ��ǰ������
{
	uint8_t obdii_outtime=10;
	uint8_t obdii_buff[64];
	uint16_t obdii_buff_len = 0;
	
	OBDII_SEND(index);

	if(OBDII_RECEIVE(obdii_outtime,obdii_buff,obdii_buff_len)==0)
	{
	    //printfuartdebug("OBDII_READ_HISTORY_DTC_FUNCTON\r\n");
	}
}

//////////////////////////////////////////////////////////////	

extern USB_BULK_QueueData	USB_RX_QueueData;

void OBDII_SET_ENGINE_SPEED_PGN_FILTER_FUNCTON(uint8_t index)//��ȡ������ת��
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_ENGINE_SPEED_FUNCTON(uint8_t index)//��ȡ��ǰ������
{
	uint8_t obdii_outtime=200;
	uint8_t obdii_buff[64];
	uint16_t obdii_buff_len = 0;
	char * pcmp;
	char * punit;
	pcmp="0102[X04X03]&lt;fb00:CMP0002[X04X03]/8;0";
	punit="rpm";	
	OBDII_SEND_PGN(index);
	if(OBDII_RECEIVE(obdii_outtime,obdii_buff,obdii_buff_len)==0)
	{
	   getCMPstrResult(pcmp,&obdii_buff[9],0,punit,0);
	     
	}
}

void OBDII_SET_BATTRY_VOLTAGE_PGN_FILTER_FUNCTON(uint8_t index)//��ȡ������ת��
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_BATTRY_VOLTAGE_FUNCTON(uint8_t index)//��ȡ������ת��
{
	 
	uint8_t obdii_outtime=200;
	uint8_t obdii_buff[64];
	uint16_t obdii_buff_len = 0;
	char * pcmp;
	char * punit;
	
	pcmp="0102[X05X04]&lt;FB00:CMP0002[X05X04]/20;NA" ;
	punit="V";


	OBDII_SEND_PGN(index);
	if(OBDII_RECEIVE(obdii_outtime,obdii_buff,obdii_buff_len)==0)
	{
	   getCMPstrResult(pcmp,&obdii_buff[9],0,punit,0);
	}
}

void OBDII_SET_ENGINE_START_TIME_PGN_FILTER_FUNCTON(uint8_t index)//��ȡ������ת��
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_ENGINE_START_TIME_FUNCTON(uint8_t index)//��ȡ������ת��
{
	uint8_t obdii_outtime=200;
	uint8_t obdii_buff[64];
	uint16_t obdii_buff_len = 0;
	char * pcmp;
	char * punit;
	
	punit="Seconds" ;
	pcmp="0102[X01X00]&lt;FB00:CMP0000[X01X00];0";
	
	OBDII_SEND_PGN(index);
	if(OBDII_RECEIVE(obdii_outtime,&obdii_buff[9],obdii_buff_len)==0)
	{
	   getCMPstrResult(pcmp,obdii_buff,0,punit,0);
	}
}


void OBDII_SET_ENGINE_STOP_TIME_PGN_FILTER_FUNCTON(uint8_t index)//��ȡ������ת��
{
	
}

void OBDII_READ_ENGINE_STOP_TIME_FUNCTON(uint8_t index)//��ȡ������ת��
{
}

void OBDII_SET_GAS_COMSUME_PGN_FILTER_FUNCTON(uint8_t index)//��ȡȼ��������
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_GAS_COMSUME_FUNCTON(uint8_t index)//��ȡȼ��������
{
	uint8_t obdii_outtime=200;
	uint8_t obdii_buff[64];
	uint16_t obdii_buff_len = 0;
	char * pcmp;
	char * punit;

	pcmp="0102[X05X04]&lt;FB00:CMP0002[X05X04]/20;NA" ;
	punit="V";
	
	OBDII_SEND_PGN(index);
	if(OBDII_RECEIVE(obdii_outtime,&obdii_buff[9],obdii_buff_len)==0)
	{
	   getCMPstrResult(pcmp,obdii_buff,0,punit,0);
	}
}

void OBDII_SET_INJECTON_QUANTITY_PGN_FILTER_FUNCTON(uint8_t index) // ������
{
	
}
void OBDII_READ_INJECTON_QUANTITY_FUNCTON(uint8_t index) // ������
{
	
}

void OBDII_SET_Engine_Coolant_Temperature_PGN_FILTER_FUNCTON(uint8_t index)////��ȡ������׼ȷ�¶�
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_Engine_Coolant_Temperature_FUNCTON(uint8_t index)////��ȡ������׼ȷ�¶�
{
	uint8_t obdii_outtime=200;
	uint8_t obdii_buff[64];
	uint16_t obdii_buff_len = 0;
	char * pcmp;
	char * punit;
	punit="degC" ;
	pcmp="0102[X00]&lt;fb:CMP0000[X00]-40;0";
	OBDII_SEND_PGN(index);
	if(OBDII_RECEIVE(obdii_outtime,&obdii_buff[9],obdii_buff_len)==0)
	{
	   getCMPstrResult(pcmp,obdii_buff,0,punit,0);
	}
}

void OBDII_SET_Fuel_Tank_PGN_FILTER_FUNCTON(uint8_t index)//��ȡ����Һλ
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_Fuel_Tank_FUNCTON(uint8_t index)//��ȡ����Һλ
{
	uint8_t obdii_outtime=200;
	uint8_t obdii_buff[64];
	uint16_t obdii_buff_len = 0;
	char * pcmp;
	char * punit;
	
	punit="%" ;
	pcmp="0102[X02]&lt;fb:CMP0000[X02]/2.5;0";
		
	OBDII_SEND_PGN(index);
	if(OBDII_RECEIVE(obdii_outtime,&obdii_buff[9],obdii_buff_len)==0)
	{
	   getCMPstrResult(pcmp,obdii_buff,0,punit,0);
	}
}


void OBDII_SET_Reactant_PGN_FILTER_FUNCTON(uint8_t index)//��ȡ����Һ��Ӧ����
{
	
}
void OBDII_READ_Reactant_FUNCTON(uint8_t index)//��ȡ����Һ��Ӧ����
{

}


void OBDII_SET_AIR_INPUT_Quantity_PGN_FILTER_FUNCTON(uint8_t index)//������
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_AIR_INPUT_Quantity_FUNCTON(uint8_t index)//������
{
	uint8_t obdii_outtime=200;
	uint8_t obdii_buff[64];
	uint16_t obdii_buff_len = 0;
	char * pcmp;
	char * punit;

	punit="kg/h" ;
	pcmp="0102[X03X02]&lt;fb00:CMP0002[X03X02]/20;0";
	
	OBDII_SEND_PGN(index);
	if(OBDII_RECEIVE(obdii_outtime,obdii_buff,obdii_buff_len)==0)
	{
	   //getCMPstrResult(pcmp,&obdii_buff[9],0,punit,0);
	}
}

void OBDII_SET_DPF1Differential_Pressure_PGN_FILTER_FUNCTON(uint8_t index)//DPF1ѹ��
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_DPF1Differential_Pressure_FUNCTON(uint8_t index)//DPF1ѹ��
{
	uint8_t obdii_outtime=10;
	uint8_t obdii_buff[64];
	uint16_t obdii_buff_len = 0;
	char * pcmp;
	char * punit;
	punit="kg/h" ;
	pcmp="0102[X03X02]&lt;fb00:CMP0002[X03X02]/20;0";
		
	OBDII_SEND_PGN(index);
	if(OBDII_RECEIVE(obdii_outtime,obdii_buff,obdii_buff_len)==0)
	{
	   //getCMPstrResult(pcmp,&obdii_buff[9],0,punit,0);
	}
}

void OBDII_SET_DPF2Differential_Pressure_PGN_FILTER_FUNCTON(uint8_t index)//DPF2ѹ��
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_DPF2Differential_Pressure_FUNCTON(uint8_t index)//DPF2ѹ��
{
	uint8_t obdii_outtime=10;
	uint8_t obdii_buff[64];
	uint16_t obdii_buff_len = 0;
	char * pcmp;
	char * punit;
	punit="kPa" ;
	pcmp="0102[X03X02]&lt;fb00:CMP0002[X03X02]/20;0";
		
	OBDII_SEND_PGN(index);
	if(OBDII_RECEIVE(obdii_outtime,obdii_buff,obdii_buff_len)==0)
	{
	   //getCMPstrResult(pcmp,obdii_buff,0,punit,0);
	}
}


void OBDII_SET_AfterTreat1_SCR_intake_temperature_PGN_FILTER_FUNCTON(uint8_t index)//����1 SCR�����¶�
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_AfterTreat1_SCR_intake_temperature_FUNCTON(uint8_t index)//����1 SCR�����¶�
{
	uint8_t obdii_outtime=10;
	uint8_t obdii_buff[64];
	uint16_t obdii_buff_len = 0;
	char * pcmp;
	char * punit;
	punit="degC" ;
 	pcmp="0102[X01X00]&lt;fb00:CMP0002[X01X00]*0.03125-273;0";
	OBDII_SEND_PGN(index);
	if(OBDII_RECEIVE(obdii_outtime,obdii_buff,obdii_buff_len)==0)
	{
	  // getCMPstrResult(pcmp,&obdii_buff[9],0,punit,0);
	}
}

void OBDII_SET_AfterTreat2_SCR_intake_temperature_PGN_FILTER_FUNCTON(uint8_t index)//����2 SCR�����¶�
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_AfterTreat2_SCR_intake_temperature_FUNCTON(uint8_t index)//����2 SCR�����¶�
{
	uint8_t obdii_outtime=10;
	uint8_t obdii_buff[64];
	uint16_t obdii_buff_len = 0;
	char * pcmp;
	char * punit;
	punit="degC" ;
 	pcmp="0102[X01X00]&lt;fb00:CMP0002[X01X00]*0.03125-273;0";
	OBDII_SEND_PGN(index);
	if(OBDII_RECEIVE(obdii_outtime,obdii_buff,obdii_buff_len)==0)
	{
	  // getCMPstrResult(pcmp,&obdii_buff[9],0,punit,0);
	}
}

void OBDII_SET_AfterTreat1_SCR_Output_Temperature_PGN_FILTER_FUNCTON(uint8_t index)//����1 SCR�����¶�
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_AfterTreat1_SCR_Output_Temperature_FUNCTON(uint8_t index)//����1 SCR�����¶�
{
	uint8_t obdii_outtime=10;
	uint8_t obdii_buff[64];
	uint16_t obdii_buff_len = 0;
	char * pcmp;
	char * punit;
	punit="degC" ;
 	pcmp="0102[X04X03]&lt;fb00:CMP0002[X04X03]*0.03125-273;0";
	OBDII_SEND_PGN(index);
	if(OBDII_RECEIVE(obdii_outtime,obdii_buff,obdii_buff_len)==0)
	{
	   //getCMPstrResult(pcmp,&obdii_buff[9],0,punit,0);
	}
}

void OBDII_SET_AfterTreat2_SCR_Output_Temperature_PGN_FILTER_FUNCTON(uint8_t index)//����2 SCR�����¶�
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_AfterTreat2_SCR_Output_Temperature_FUNCTON(uint8_t index)//����2 SCR�����¶�
{
	uint8_t obdii_outtime=10;
	uint8_t obdii_buff[64];
	uint16_t obdii_buff_len = 0;
	char * pcmp;
	char * punit;
	punit="degC" ;
 	pcmp="0102[X04X03]&lt;fb00:CMP0002[X04X03]*0.03125-273;0";
	OBDII_SEND_PGN(index);
	if(OBDII_RECEIVE(obdii_outtime,obdii_buff,obdii_buff_len)==0)
	{
	  // getCMPstrResult(pcmp,&obdii_buff[9],0,punit,0);
	}
}

void OBDII_SET_Engine_Fuel_PGN_FILTER_FUNCTON(uint8_t index)//������ȼ������
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_Engine_Fuel_FUNCTON(uint8_t index)//������ȼ������
{
	uint8_t obdii_outtime=10;
	uint8_t obdii_buff[64];
	uint16_t obdii_buff_len = 0;
	char * pcmp;
	char * punit;
	punit="m^3/h" ;
 	pcmp="0102[X01X00]&lt;fb00:CMP0001[X01X00]/10;0";
	OBDII_SEND_PGN(index);
	if(OBDII_RECEIVE(obdii_outtime,obdii_buff,obdii_buff_len)==0)
	{
	  // getCMPstrResult(pcmp,&obdii_buff[9],0,punit,0);
	}
}

void OBDII_SET_Friction_Torque_PGN_FILTER_FUNCTON(uint8_t index)//Ħ��Ť��
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_Friction_Torque_FUNCTON(uint8_t index)//Ħ��Ť��
{
	uint8_t obdii_outtime=10;
	uint8_t obdii_buff[64];
	uint16_t obdii_buff_len = 0;

	char * pcmp;
	char * punit;
	punit="%" ;
	pcmp="0102[X00]&lt;FB:CMP0000[X00]-125;0";
	OBDII_SEND_PGN(index);
	if(OBDII_RECEIVE(obdii_outtime,obdii_buff,obdii_buff_len)==0)
	{
	   //getCMPstrResult(pcmp,&obdii_buff[9],0,punit,0);
	}
}

void OBDII_SET_Engine_Net_Output_Torque_PGN_FILTER_FUNCTON(uint8_t index)//�����������Ť��
{
		OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_Engine_Net_Output_Torque_FUNCTON(uint8_t index)//�����������Ť��
{
	uint8_t obdii_outtime=10;
	uint8_t obdii_buff[64];
	uint16_t obdii_buff_len = 0;

	char * pcmp;
	char * punit;
	punit="%";
	pcmp="0003([X00]&amp;f0)/128";
	
	OBDII_SEND_PGN(index);
	if(OBDII_RECEIVE(obdii_outtime,obdii_buff,obdii_buff_len)==0)
	{
	   //getCMPstrResult(pcmp,&obdii_buff[9],0,punit,0);
	}
}

void OBDII_SET_Engine_Bench_Mark_Torque_PGN_FILTER_FUNCTON(uint8_t index)//����������׼Ť��
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_Engine_Bench_Mark_Torque_FUNCTON(uint8_t index)//����������׼Ť��
{
	uint8_t obdii_outtime=10;
	uint8_t obdii_buff[64];
	uint16_t obdii_buff_len = 0;
		
	char * pcmp;
	char * punit;
	punit="%" ;
	pcmp="0102[X06]&lt;fb:CMP0000[X06]*0.4;0";
	OBDII_SEND_PGN(index);
	
	if(OBDII_RECEIVE(obdii_outtime,obdii_buff,obdii_buff_len)==0)
	{
	   //getCMPstrResult(pcmp,&obdii_buff[9],0,punit,0);
	}
}

void OBDII_SET_Speed_PGN_FILTER_FUNCTON(uint8_t index)//����
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_Speed_FUNCTON(uint8_t index)//����
{
	uint8_t obdii_outtime=10;
	uint8_t obdii_buff[64];
	uint16_t obdii_buff_len = 0;
	char * pcmp;
	char * punit;
	punit="km/h" ;
	pcmp="0102[X07X06]&lt;fb00:CMP0002[X07X06]/256;0";
	OBDII_SEND_PGN(index);
	if(OBDII_RECEIVE(obdii_outtime,obdii_buff,obdii_buff_len)==0)
	{
	   //getCMPstrResult(pcmp,&obdii_buff[9],0,punit,0);
	}
}

void OBDII_SET_Kilometres_PGN_FILTER_FUNCTON(uint8_t index)//������
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_Kilometres_FUNCTON(uint8_t index)//������
{
	uint8_t obdii_outtime=10;
	uint8_t obdii_buff[64];
	uint16_t obdii_buff_len = 0;
	char * pcmp;
	char * punit;
	punit="km" ;
	pcmp="0102[X07]&lt;FB:CMP0002[X07X06X05X04]/8;NA";
		
	OBDII_SEND_PGN(index);
	if(OBDII_RECEIVE(obdii_outtime,obdii_buff,obdii_buff_len)==0)
	{
	   //getCMPstrResult(pcmp,&obdii_buff[9],0,punit,0);
	}
}


typedef struct
{
   uint8_t CmdProcessIndex;
   uint8_t (*SendFunc)(uint8_t  );
   uint8_t (*ReceiveFunc)(uint8_t );
  
} OBDII_CMD_PROCESS_Type;

#define M_PROCESS_MAX sizeof(ObdiiCmd)/sizeof(OBDII_CMD_Type)

const OBDII_CMD_PROCESS_Type ObdiiCmdProcess[M_PROCESS_MAX]=
{\
	{0,OBDII_INIT_IO_FUNCTON,0},\

	{1,OBDII_SET_VIN_FILTER_FUNCTON,0},\
	{2,OBDII_READ_VIN_FUNCTON,0},\

	{3,OBDII_SET_CRUUENT_DTC_FILTER_FUNCTON,0},\
	{4,OBDII_READ_CRUUENT_DTC_FUNCTON,0},\

	{5,OBDII_SET_HISTORY_DTC_FILTER_FUNCTON,0},\
	{6,OBDII_READ_HISTORY_DTC_FUNCTON,0},\

//////////////////////
	{0,OBDII_SET_ENGINE_SPEED_PGN_FILTER_FUNCTON,0},\
	{1,OBDII_READ_ENGINE_SPEED_FUNCTON,0},\
	
	{2,OBDII_SET_BATTRY_VOLTAGE_PGN_FILTER_FUNCTON,0},\
	{3,OBDII_READ_BATTRY_VOLTAGE_FUNCTON,0},\
	
	{4,OBDII_SET_ENGINE_START_TIME_PGN_FILTER_FUNCTON,0},\
	{5,OBDII_READ_ENGINE_START_TIME_FUNCTON,0},\
	
	{6,OBDII_SET_ENGINE_STOP_TIME_PGN_FILTER_FUNCTON,0},\
	{7,OBDII_READ_ENGINE_STOP_TIME_FUNCTON,0},\
	
	{8,OBDII_SET_GAS_COMSUME_PGN_FILTER_FUNCTON,0},\
	{9,OBDII_READ_GAS_COMSUME_FUNCTON,0},\
	{10,OBDII_SET_INJECTON_QUANTITY_PGN_FILTER_FUNCTON,0},\
	{11,OBDII_READ_INJECTON_QUANTITY_FUNCTON,0},\
	{12,OBDII_SET_Engine_Coolant_Temperature_PGN_FILTER_FUNCTON,0},\
	{13,OBDII_READ_Engine_Coolant_Temperature_FUNCTON,0},\
	{14,OBDII_SET_Fuel_Tank_PGN_FILTER_FUNCTON,0},\
	{15,OBDII_READ_Fuel_Tank_FUNCTON,0},\
	{16,OBDII_SET_Reactant_PGN_FILTER_FUNCTON,0},\
	{17,OBDII_READ_Reactant_FUNCTON,0},\
	{18,OBDII_SET_AIR_INPUT_Quantity_PGN_FILTER_FUNCTON,0},\
	{19,OBDII_READ_AIR_INPUT_Quantity_FUNCTON,0},\
	{20,OBDII_SET_DPF1Differential_Pressure_PGN_FILTER_FUNCTON,0},\
	{21,OBDII_READ_DPF1Differential_Pressure_FUNCTON,0},\
	 
	
	{0,OBDII_SET_DPF2Differential_Pressure_PGN_FILTER_FUNCTON,0},\
	{1,OBDII_READ_DPF2Differential_Pressure_FUNCTON,0},\
	{2,OBDII_SET_AfterTreat1_SCR_intake_temperature_PGN_FILTER_FUNCTON,0},\
	{3,OBDII_READ_AfterTreat1_SCR_intake_temperature_FUNCTON,0},\
	{4,OBDII_SET_AfterTreat2_SCR_intake_temperature_PGN_FILTER_FUNCTON,0},\
	{5,OBDII_READ_AfterTreat2_SCR_intake_temperature_FUNCTON,0},\
	{6,OBDII_SET_AfterTreat1_SCR_Output_Temperature_PGN_FILTER_FUNCTON,0},\
	{7,OBDII_READ_AfterTreat1_SCR_Output_Temperature_FUNCTON,0},\
	{8,OBDII_SET_AfterTreat2_SCR_Output_Temperature_PGN_FILTER_FUNCTON,0},\
	{9,OBDII_READ_AfterTreat2_SCR_Output_Temperature_FUNCTON,0},\
	{10,OBDII_SET_Engine_Fuel_PGN_FILTER_FUNCTON,0},\
	{11,OBDII_READ_Engine_Fuel_FUNCTON,0},\
	{12,OBDII_SET_Friction_Torque_PGN_FILTER_FUNCTON,0},\
	{13,OBDII_READ_Friction_Torque_FUNCTON,0},\
	{14,OBDII_SET_Engine_Net_Output_Torque_PGN_FILTER_FUNCTON,0},\
	{15,OBDII_READ_Engine_Net_Output_Torque_FUNCTON,0},\
	{16,OBDII_SET_Engine_Bench_Mark_Torque_PGN_FILTER_FUNCTON,0},\
	{17,OBDII_READ_Engine_Bench_Mark_Torque_FUNCTON,0},\
	{18,OBDII_SET_Speed_PGN_FILTER_FUNCTON,0},\
	{19,OBDII_READ_Speed_FUNCTON,0},\
	{20,OBDII_SET_Kilometres_PGN_FILTER_FUNCTON,0},\
	{21,OBDII_READ_Kilometres_FUNCTON,0},\
	 
 
};


void Task_ObdiiCmdProcess_Proc(void)
{
	uint8_t i=0;
	static uint8_t delay=0;
	 
	for (;;) 
	{
		if(delay++<40)
		{
			vTaskDelay(100);
		}
		else
		{
			
			delay=102;	
		}
		
		for(i=0;i<1;i++)	
		{
			 	
			if(ObdiiCmdProcess[i].SendFunc)
			{
				ObdiiCmdProcess[i].SendFunc(i);
				vTaskDelay(100);
			}
			 
		 }
		 
		i=0;
		 
		
	}
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
||||||| .r470
b#e� D   E-SafeNet   LOCK            ���w y�3�����)��R��f                                                                                                                    �تPV�c�#F���
S񗴵�S��Vz�o��^����E�d�p����l%K��vX�O�+�[�`�'H9�-�+��q�]<(���n�X� �_��~8�(|�1W�u��ԡ�\���-�Q�*�Qya!QD@�O,�G�b���z�0����;����d99pZs���4��}ǚsU���ꔧk�!@�{���;鮬��`�	�$EC6u�1[����ͮm�;�A�~�yo䍎�<%��� hU*�����(JC���=�&S{�淳�|�����	/4�+2�j���lM��r�_��(1�0!a��`���(l�j�`�6��^6�������Gz�<ȵ��)��s�v��g����cv\��TQ�K�>�	�g�*?�y�/��q�[����%�u��]ړUz�n1~�3�s��EH��<<�W�L�Er"[�7(>i=9\!��+�߫��r>�����s�ɢ��b1mwIn�͈w��j�YȾs�Q�44w4��m�˔��)�f7�0Cu*=�Rf�Ӻ;���m�W'�.�Z�{���>&���4&��#�X��LqW�#;[��x�:CIo�����y�·���?$�&w��r���^ۍ1~�N��=�ku2P��Z�\Vk�R!�C������(�M�Q�a۵�g@�>{Oʸ\��%U��x�1Z�����/�k���'e�6(�g�B��F�GD��_6e�23�u�<��IU�*��2#g
}g	A����\����P��D��]���9E�A�(����~wr�t�Ԋ��6n�G�EA?�q�"�0(�?��u�G=����R���()�-�f�b��'�����P��9z�e��L��p�0�9��5��HC��'|��� �w��$�ha�Vs�lЈ5�����%��E�U��2�%r:�x�6D���-��P^p�4�$�Uhf?�{@AQ:=>Q,��&���e<����R�p����� z4-1P���9��4�+]X��;Ԧ��#�p{?Zm��6�볪��L�9
N:*�R6�Ѫ ��(�K)�CS������;4ץ�>t5��J�h��l\*�GE;���w�zT!�����v�ˈ���ObXi�}=��3ۈ�[��R8�Ҝ52�7z_Dn�A�O�1&h9H�;e?�-������+�[�.�,���?'�,0j��$v�Ŭ]��U�����k�+����!|�62��T��&�cj�*�jX=�:qL�C�dƫY��U�p�u�������)�ݱ�v����{��-,��.���Mi�m���/Ka%�S��GD\�gC�����!�h�����\�X	�v��
�)����`���%@��S-�+țA�񲺘�	 ���.�c�����]��+6��^	|��wh�o��g�>�r
�:k�i��(��, ;
��~Ѝu5��3W�KY�x�Y-��Λ�da��9 �i�g�i�;
i����(:�ؽ���Xz�����8�堲�l9m_|y�ēu��k�Y 2��&��ЕK�z=Xo�|�g)"~�d��j���ɇC�{����̳�[b�>
�]�EM̦߽'a��;�>{U\c�B�$sƃ�\�B8���Q�?~wQ�糢�8�����]} �T�˪5o ��ZǠ�[��,_}aHk�*�)q�qy=X8Z�&s1�������+�P�A�[�ӬwV�E��X`��_m�G�i�7A�����"�D�ٖ�W�
��n���Gr�2�&x�r8�4�|��&bL�u��5 J ^`����i����X�������kS�W�3�͑�\F)m�%���-G\_��'S���t8�%?F�Q���Tʮ'�4����`���5}�(�\�{��u8�����e��j���(��z��5f�6V��B:�ְ��Yf�~'�T�#�A�s�=?�#��m�_E����]�E��Y��_u�j*q�'D�C>�������CLs�y����q�Tg�̏����ɚ=�߾���Xl����/�ǻ��^K_gi	�ԕK��t�cήd�԰�@�m."G��^��:;�y���:0c.̹�>��Kj�X���D�lC�(�U�oߚ��+c��=�'!��w�c��R4R�>:@����hKO>�����v�^TP�W� ��>�6i  }��tX�o��EX�<b��?~�vRiF:�7Q�L�I?&�&��{�:*�M�`:O��B��'x�'�E�*g�����6�l����Xe�3 �h�� ���~x�$
�8�O���J@���p\3Z"3p����\�����ԛT$ѺI�����g�~�.)E��ܱ^�껣faz?��((
�\�Im�p�9�'�Y�:�$����}���JkO�s�/E��ZU��+`p!D�1?���M�f��P��X�Z˯L����J���o��߽4��F�uT�O��[�wnۚ��b�X�
� l��(�?hV��3ARo@+{�-��z	�{�`�{/aj�8/9CijCe�Q�x�Ū��EH����
�3�����]W_aR9�����9�D4<�F����I�f[W`��f���āJ�<P�E7YtC���X�Ǿ�qW�/�J�=Ī����0�< ;��>�:��~\*�1����
>yyG�Ǎ��۠����a+K�[i��^���ro:��3R�x��_Y�H>mx�d�m$�CN^4N�;s"�7������7�_�4�;���kK�`:{o��I��;Y��y�!^�����#�r���4.�~k�N�9��(�kd�0�j_N�(�T���;az���c$M#,uvk����f����{��0#��"���\i�c�<��̫v�&���/":/��&(;�a�|m�S�����_�b̈��z���4@��P>�F��N�����H�>P�B������S�LűL��-$��@`��j@�v�W�R�"� ~�#���P�mdݶ��H�Q�3�u5��3c�#R��U ��Ð���1:�m�v�c15�<^eRQw5H�~�A����@f��ǻ+��ၠ�QZ	%t×�[��P�Y5ݲx�ԣ�a�<8f(��k�����q�3G�T rEG�D8����ڿ#�Gi�>�\�{s����2!���!1%��M���(X-�C4����$qKk�݆��2����ȼ<@}O�e>��U���XO��y�t��T`�DXzbL.��La�|b+N$o�X�����ѱ'��
�G���^�8Va=��e��#U�b�&�ws�罇�/�}����!m�"2��j���[r�2�P:\�HH�*���?{d��/�o|u2;J����f�����b��mo����Vt�m���Vs�g���
/��";�j�k|1�B�[�*w���.���w���)l�7�b�F��t*�����r���E��f��k��U�W˹@��$[��k0Z��JI�K��[�}�7H.�,�;���7J��ڊM�e�0�/z��S~�j*u�&_�p1����DR�� 3��$�-rB&*�%jvTZl=g�R�_�Ҿ�Bp�����6�����}2ka\i���S��5�z|��$�Ģ�G�t%��i��걗P�>H�tA:�d�Ю&����iI�M�9�a,�ĭ���?�6&.��b���a�$;S���,�8RLo�����	٪ڐ�jDw�~]�y�U���YO��v���YO�q;
*A��^�JJdo�"k.�O�󠀥�q�e�E�$���H�Ge,��n��d;H��d�
x������G�ݐ�F�'!�:�W��c��:�i_I�'�t�!��z� �9�J]7_#A����Xܽ��n��N��w���M���ֈ�M:a�g���
A/��3>;�@�sf)�n�#�%3�W��}�ʤ�H�¬5M�
�B�s��u<�ì��v��|/�?��3��7��h�g��z��%'٨Q[5��)7�Q�k�f�J�e�0�#��j�Rn����#�!�f�N
��x\�FU�5_�y1����ag��\(R�0�#�_N�S|lkozPh�Y�_�Ů��At�忮r�T�����OPIwZ�}��F�iϾI�̮�"�P\zJ�*���J�3R�}
N +�-[�ֿ'�ڮ"�_r�>�}�6A����ni��<�1?��j�R��G%�?;B����'
B[Q�Ŝ�����|Nj�V�t�Y���CO+��t�t��hv�L:4A�>�.S�d&h;�^c"�*�������A�����&e�5go,��n��*f��!�sװ���)�f����F�9� �v��g�+V�E�bTD�/�c�7��1jX��l�6$M#,uk����p����S����-���5j�.�a���2 +$�Z���"48��%<�j�k|1�B�[�jK�?��_�m\�ʤ�J���=)�-�nJ�T��N�����{��kW�4��e����E�]ˤ@��_Yܴ=f��.4��r��!�md�Sv�iՍ0����� ��@�P��7� w?�}�3A�����_[p�0�#�k=f�DpqkT]l:H�c�Y臧��x!�� G�3���שB]IaC/��K��W�6E��'դ�`�� [9M�����J�3D�N<c.*�&K�����}�6�Sr�P��*]SJ��{P�W�^^s��G�~��gd�	g����/t>�����u��}}1����v��5����'#;��4^�x��EU�K?9{�:�?h�t'CK:�BP�O��Hd<��o�p�4WwJ�%qD��Y��;S��d�'W����������G�Q��2��x��Ɠ� 	�mnS�)�|ūY
�l�I�hxhu�����粦���KMΥR���6��n���=T6:�껣$)79��2$!�r�bw�N���|��3�����ǴVt&�W���WX��{|G��<��w���-����S�VݣO��&T��=f�� �z�M�7�8��w�T�K��7~ҷ��P�i�0�>h��cG�@D�s�L)��ƍ�ee��@o�	��ɉ��E�����ʹ���>�I�����Pw��Ҧ+������L\=dM���8������m�Bq7y�P\zJ�Y���ߛF�3R�]*n9 � @����ڪ
�qA�-
�a�*e��һs~�3�]����E�4�"e/[)�{����$rpI�̗��߶����-Jv�b"��3�`��ҬC�Wx��_yfK-�,�)g�kn6N3D�;b%�0������6�A�6�4���R�Wxn��<��z��7��ѳ$X$k�+����!|�2'��E��$�}~�:�rBI�|L�6�cݨY�\�f�a���Ѐ���ڶ�0����a��0=��.���Te�i��Տ�T2UA�Gځ�[ZCH��Nא��R�l���B�Q�[O�p���5����r���5Z��>Z�&��?u��(T� T���Q��Pů�\��V��1,��Du��pr�u��g�G�e�2���Z�����6��@���p~���Q_��C<��ě�j��$;�h�h�p6vl�op13S!R�K�ҟ�E:��׻6����j>wew_�̀z��M�k Ⱥb����^�	T>��I����3�@8�5L�����FĻFX�U24� �
�h�w�SB٭�� �����b�C��G<P�8.F���e�-KO������_,&����s��4�R����  }��9^�t��NB�m04E��0i�gm+]"O�O�R񼆦�+�&��u�)/R��c|B��T�� D�;�G�*i�����V�������XֵJ�9�|��( �|�'O�5�U���EF�"��on	up����A��������@Y�YlG��e�c�QS~�rVW�\���.*7/�� %/
��T|�]�~��T�m���Ws}�6?wQ�J�	c�T��N�����|��z���=��4��n�LҷQ��+%��_k٘��rE�6�@�7A�3���M�uϲ��P�b�:� m��Tl�{!-�*�,*��yI��
X}~V�3�f�`6p�Cjad,WZg�E�e�޾��cV�����萛�A\R,���Q��6�V6Dkb����A�[(��_�����G�
q�hN."�"f�Ѽ'���}�7�$r��J[�9[?�\��s�q>��\�r��ff�&T���=�!VRQ�𷲍,�����?>
�@��,ŗ�[��;%h�O]�W4"{�+�$q�hmPUl�^��������m��V��w'�F/��9c��G�!�1J�����#�}�ڐ�w��v�X��5�LY��rbe�2$�r�!��&ck��*�z{z/wk��;)/ӝ�7!�7�Pp[ۀ)�H�Ѕ�[$KP�Q���$�� `7�g�E\8�u��4+�W̪!�4��ֆ�ǠVt&�+��J'ذDlOXmy�~ >P�B��#����I�Gݱy��=��l"��go�C�)�B�Q�'W;�8�3��@�yϵ��[�~�_�O��7����w�ۭ<=��=<w��	Q�yU�v�m�m j�HtC@,oGA`��X����<pX����8�ǻ��hj)x,L���;>C�ƝG��U�6����zJ��܉�āJ�?R�E"F=�u����ץ�Zw��V�jx����-4��9�'6��Q���`�e`�e4� d[d}\1tm0�(,����"	>� W�y�]���_~��	&�x��ER�l%p��\�L^tK:�BQ�O��z;�/�"�9Hc#����T���� R��n�!I������C����+q�8�7����RH��Xde�2�@���$�s�W�h�������z�{k6�n�o�2���Pi�a��ӧ�w	Mg�t���8(!/��$:;�P�\W>�j�#�lv���f�H~Z4� Š)[� ٤e�0�Pܽ���q��l	�����4��\�v�O��=/��Ym��\��q�G�7큱}���c��48߱��F�~�.� y��Sv�Pe�>�d^�����iQ�ϥO��iӾ�"[�?,{YZz#R�c�M�޼��'l���
��၂�DMEz!M���%��6����{�Gk���[>��Z����`�x�E7O:+�Bl͒�g��Չ��;��#>������-�,��m�h��Vo�>?G���7�$Ra^�̍��ʡ����P|Fp�}=M�R�O]���[����R�\94a�*�%@�QEN8o�-c�������
�{�A�[ˤ�k8�ݑ��%b�	}���:�ˢt�����"�k�7f��"�I��	�}`��rBo�34�V���?hq��V�v{hu�Zf�&)z}�{��_jp���[r�m��Ӆ�W~|�p���/��
�D�I|�.�`�}i��}�A��s]�KE���I�`�Y��N�����f��V>���3����H�#��.ۚNF�7��欢-r�n��{�O�i;�=�v��/���Ep�h!�f�u5��~Q�PU�}�p�����B��>+�u�l�O0`�.
��ܶ��=�D����Tr��؛�0�ҽ��0ga0-D��έ/p�t�}��k�Ģ�G�_3��_������n�E_<:�w���)���m�lC�(�~�ETŬ����!�6"ѯ	���L%�>oq����:l:߹���y��ڜ��$>�V�i�l���''~��x�X��U_�Q/(`�:�K�wIxI�w=�?���ա�T�1�^�DԺ�h8�E/q��$��zK4�a��Q'�����I����G�XI�h�$��E��Z�9/�HH��sʤV_j��/�vg~YԌ��0��湾��T��Q��)�\�%����"W)m�$ˁ�[X_@��QMa6]��T~�.�i�}v���e�Va��ֆ���Jk%�;�3F�n��+i�����M��j�v��\��?��b�u��gڒNYܴ;zٙG�R�;��h�n
3�Lu�>ʕ3�F��ƽ?� ��O��
(�?  �~�,^������@Xo�	��\Oqa�'	1-#Ly�V�;��﹌0!�����m����ug),dMۍ�l��5�~������;�m.sBlѲ+����;�@'�+P PO7~�R8���b���5�'�Q&��6-��ЭsaɵN�Tc,Jق/�Oь}�z{���h�uWF9����:�ċ���Qm[v�b'��dğ���];�S��:1�`eCKm�^�@�@NZ1Z:�C@�S�Μ����/�]�ˢ�k'�Z r��;��yTc�{��8�����V�ꌜ�	�G�w�'��Z���6,�:q�*�+ڴV�s�F�Qwwkj\����	�������U�p���9��L���=T6:�:Ȟ�DD\_��NN~aK��c�1�~�~i���z�H~�ͧ�o���Z{)��G#�I��S�����|��v�c�f��j��/�#��*ƒY��=.ńE��m��"�:\.�L!�%֒g�����#�T��ODŊ(�?h<�*�0^����IF��@o��Q�SxW4�;]Rp1#KL1��2����+=����O�m����5{)xM~Q�هr��a�6\E��.ջ��6�m.tlѲ+����b�a�:C SC�{������m�|�#�z�HXʬ��̩"�rZ/J��6���}�z���`�e"�����i�Ɛ��+[v�4+��y���UL��'���,.�_PKa�B�m$�CwoV*�RW/�'������$�L�$�%���[�/a}r��ez�b�S�K$����V���ԣ_�G��;���U��n0�nq�>�c���%�J�Gwg?c����[�������TRͺ���)N��q����")%�%���G[ŧQ7(H�M�|�'�}�)#���3�Ka�����θVkq�{�3�p��iᛮ�V��9z�o��@�P��F���?��-+��D	m��`b�x��g�Q�y6� T�O��g�����#�T�_�OD��(�<h<�*�0^����IF��@9��Q�ESxW7�;]y1#KD4�:�+����9����u�}�����IP_{Y)��G��K�Y4!��P�Ǵ�\�q
}`�+�����{�Y;�bSPO7�^$���~���u��r�K�x1��ҽsaɼF�H+1ƒ{�ŌZa�z7��� �7WFh����i��߅��P5~�bj��,�����Q'�N��*e�~y@q�^�Y�SsZ:�JL��Ά�ԤC�.��G���!;�0r��:��Xn;�n��RT�����k��Ġ'a�>/��J��"�qy�'�rRT�?m ��(��	�;�S�Qw{w"����
��������M���5�B�m���>Kas�9�ɚ[GC��l[nD�� nA�1�~�~i���z�H~�聙�دUt&�l�,E�9��(v�������u�`��C��u�5�<��0ōMF��"y��.4��m���9fF�\y�f��]�wxϠ��P�b�&�:c��]�PW�p�P:�����<*PD'���EJd)�'Lyy?6��s���.!���S�%����4o50PyE���r��)�6@Y��/��Ыk�qfj&��c�����e��:C \c�x���X���m�aE�(�a�[Yԭ����-�=���>��Z0�z7S��� �eWF>����i��߅��P5'�bj�[�,�����Q'�M͟*e�(y@t�c�@�VNM{ �&i�L�υ���r�B�{�K۵�U�#z_��T��0I��d�*I�����(�d��:^�+k�N�+�����&x�rmL�6���A	�;�^�Q!{w"����	������٦M��5��m����>Kap�9���GC��MQ)(�8�l�c��[O�8��	�mq���|���3[��\(�B��^�����r���E����;��*�r��/��RE��=n����d��v�#{�?�v��&�I����w��_�O��B-�rU�r�aB�Ɋ͞��_[R�0�u�._hq�Xwa} B@v(^�y�B����	Cg��ڿ:��萛�A\RU0!���o��a�3\Eº#ջ��>�m."lѲ,����;�\o�*OO{b�
$͎�*���}�K?�Q&�
�*e��ν'h��N�kxJ��e���}�,*O���x�ub�����S�ć���"	>� W�y�Y���co+��<E�t��WX�4	}aH_��U�@JW=Fr�B P�S�Қ�W�f�I�[���k'�U,n��2��-T/�~�M�R8����������H�
J�J���Y� �|�q
�ba\�I���)w`��(�vgut����|���Ǚx��0'��Qʅ�M�� ����sN5%�%ם�[\��VQ}~��J*�2�)�ej�J��y�h���V�۰=0�s�3Z�&��?u�������Uv�7��@��<���3��?��>d��v
2���Q��`�Q�{�9���@�j~����B�Q�3�u5؃By�#hh�g�d^�谼�	��CDs�A��-N}�oSCyeu
P1��'�Ϲ��xi����J� ج���-*5\%t��� ���},x��6٫��L�M$��\���ÇN�9Z�E3g2 	�&Q������6��Tr�K�/1��ҽsaɵ�H+cJƒ{�ŌZ7�z7��� �eWF7��⹛a���ڌ�P5Gv�bj��,���Q'�͟*e�}(}aa�N�Lr�2"M{ �R@�,������>�Z�6�8���
D�'v}r��.Tc�{��=�����J�Z�����[V�~�'�����&x�{mL�6���	�;�V�Qr*JP����ŗ����Ko�ʅ�9���ֈ�M(\A�\���(?<>��4 -�l�|u�X�����_�qпσ���Ow9�s�/Z�&��7!懲����"�����v��*�?��&ҎR��!fGǑ��q�Q�6�rB.�Pi� ��/��Î�j�!�O�_ޟf�#L�X�<N���x��10 �w�l�p0p �V{q}Dn �>�p�Ͼ��xiR����O�q����=/*0,���$��x�6��&����>�mb>G8��7微��3�Y.�i \_o3�.���X���D�(�Nq�>����R[��^�>��\�r��g?G�$*m���4�;hjk�򷳟-�����.1�W�@�@���V
D��H;�S��6-�(e\[9�B��(N&�
JP�S�Ξ���K�2�	�Sץ�"/�ZKfg��s��yTc�v��0�����J�Z����E�
k�g�+��F�/'��  �ba�D���$ad���VG"u(5H����mͥ��_����!���9M�\�t����'W)m�%ˁ�[S_@޸QMa6]��TW-�.�7�}v�T��e�We�������Jk%�;�:���'y���<��m�`��f��P��F���?��-+��D	l��{{�O��H�e�}�:���b�b<����?�M�C�]��
`�?t �bR�,�������o�	��O0a�'Me-kVD-�O�2�����d!K�����e�����!ga00M�Ǆi��9�&P������[W`��`���Ɂ\�)V�E%U����_���m�|�?��b|����oa���@dْ/�OЄaX�+c���t�->�����!�ԋ�͠PaG>�ko�7�<ԇ�vy��R8�Ŝ52�7Xzb:x�d�@�Cd U?C�!i4�1�������N�?�-���E�FoT��+��4Q7�6��Np�����V�����D�N^�w�m��Z��E�&,�:xD�6�6ҨF\C�o��wRnv����	Ю���F��HR��녟9��m̟˨i'�5Ǒ�$)79��3$
;�P�OF-�j�}�\L���-�R}пσ���Jw9�s�/Z�%��7!菲��P��"�)��Ҏv��*�?��yڎR��!fG��Y0�
�}�T�R�H3d�S`�iՍ�7J����/�� �;u��i]�PY��L;��ѱ�_T��	+�w�o�h+},�K8/=2=#K4��j����x=Ǩ��O�j����5{)xN,Q��� ��a�6\E��.ջ��6�m.tNlѲ+����;�\o�"O)b�
$͎�*���@�'�A#�o�����Uq��^�73��Q�v��cr�b���	�4CWz�����S��܌��UaG>�g>�E�,ؗ�X��N'���"1�`0Gq\��PV�RiF>�BT�O񆚹���'�|�K۵�:;�g9	q��$��zK4�C�"�"������`����;f�("��O���PH��Lz_�0$�s�6��1jz�
�2�mTkvZMz����	Щ���F͞HR��M���)R��v����+C5%�sם�[G\��QQ}~��J)�2�)�hj�J��y�h���V�۰=o�s�3Z�&��b$ܽ�����oW�F��a��z�a�QƹV��'(��Iy��M[�O�8�]�o�j9�*�4��3�3;�Ӎ�w��_�	��B(�#hh�j�d]�谼�	��s�A�O�.}�oyeuP1��'����E����S�P߯��"x(13R������&PU��Y�ϩ�Q�M3��_����f�~�v<p<=� q���ó�kU�M�9�a=����sa��K�H+dJƒ{�SŌZf�z7��� �eWF>����i��߅��P5~�bj��,�����Q'�Nş*e�~y@q�^�K�5!B1V*�^pm�j������[�e�3�"���V�5wP��b��0~�:�J�!X������_����aX��k�sćF�Z�Y�U&8�r9O�*�+ӼZ]�"�V�YOk/!cȨ�������ԛTN��H�����<��̫T6:�$՞�DD\}��=\DD��W}�W���sؿ0�3#��ڇl���%2g�"�f5�s��b+�����j��}���)��v���h��g��NY��8z٘�R�9��9�n
3�Du�>��3�F��ƽ?� ��G��_ �? v�~�,^�������HXo�_��\Oxa�qPX9C=3\|�k��̃��h1��ܰ;����RtMn)�Ā`��J�T/ԯw�k�-3`4��~���#�L7�*AIS6�G8���b���}�7�Rn��#%�ؚ�>}���Tc,ӎ3�ݐ)F�fJ���h�`BJ3����u�魵��ObFu�}=�0����'
T��X�o��EN�L
15A�:�O�
t1R$U�X�������	�j��4���Q�&gE�W��\;�6��Np�����V���ԿD�@V�w�2��Z�D�E�",�:qL�6�jҨF\@�o��wRgwv�郟	Э���F��HR��M���S�)�a����>'�n���KKSP��#%;�p�hm�j�#�##�F���5����X���6y�1�w�s��[T۾��T��"�z����v��*�?��,ڎR��!fG���L�q�Q�7�rB{�Pi� ʎ/��Î�6�!�O�_ޟf�w?�}�3A��θJ|��PT�B�k�m6|�HtC@,oGA`��X����<aF��Ǘ�8�ǻ��hIGfC;��Q��5�zU��6��Ыo�qfju��c�����3�@'�-SLvv�Bl���b��a�V7�Mn�
�6-�ڡo)׼R� cdV��:���Gi�2)T���h�yKOXk�����y���บi6+K�r2��S���du&��9H�M��hI�}4Zw�<�/[�H^N"o�I������v��
����wo�_0&��'��e\7�6��Np�������Ă�D�_�w�;��Z�D�E�&,�:tE��sʤVY	�N�o�hbhu�ݱ�B�ŗ����+ ��4���Mi�B�$�Ƴ�s+J�V�����5<>�T�Y@)�N���f��(�;\��Ֆ���Vkq�v�3�&��i������Kc�c��
��j��.�#��/ƒP��=.Z��E��m��"�:s�L!�~֒g�X����#�T�
�r6��8�#�[M�d����~b��9+�x�v�B-]�rTQNPa#N�C�~����8tA����q���Ԁvgaa0M���8��)�*@��:����"�%o*[p��z����/�q�6SXLKc~�$ђ�b���5�z�k~��g1���p~��H�W|Yp��_�=ɜq[�{����
>YYg�獇�<�����&;� o�a��܁R��Mo�уb-�4e[%��P�@Z=Fr�B P�S�Қ�W�f�I�[��k'�
,n�;��-]#�~�N�R8������ꌜ�^�}F�g�v��g�+V�x�6 �9>�O���7`z�-��Zv_2+'�ݱ��������Wͺ���)N��q��" -�%���G[��Q7wH�M�|�x�}�)v���`�\�玖���wQ �l�2]�9��"������|���?����s�g�q����h	o��{{�f��T�R�H3k�\i�'ώ/�_�Î�?��_�Sۄ
4�wa(�bN�,Bۼ��X��\D'���AGd)�.Ly-?8��s����.w���S�%�òЪ-w9 U0!�����b�zU��6�Ģ�G�W6��i����m�8x�kESC�{4���3���5�+�n��b-����la���@d��/�O��aX�sct�->�����!��ڱ��@mW;��4�3ۖ�y��K�!ݓ:=�W4"{�+�?k�_Bt)D�s/�������	�k�.�,���[�/a}r��uTc�{��=�����J�Z�����[V�~�'����E�&x�rmL�?���	�;�_�Qw{w"����\���X��RѦM���d;��a����qn�푊KK?��((!�q�i\+�p��42�}˿%�����F���&V�c�3�#��8䛮���Kj�c��_��j��"�#��~ƒ¨=.Y��E��m�O�"�:
{�L!������ڙ�%�e�P��7�R�;�<N�����ui��#1�|�N�Jm4�t]zy(oxl(n�F�n����X����S�P߯���u6,,d���l��5�~@��nλ��v�ez"&��+�����{�\;�bZPO7+�^$���~���}��Yr�K�"1����saɵN�H+2��	�ɜ_}|�@FIʍ�x�u[LqL�˛��ߠ����	#�7|�U�Q���r~��r����ce
t��U�@R=Fr�J P�P�Қ�W�f��[���k'�
,n�?��-+�~��R8��˸�l�朙�4.�~k�N���`��Z�&/�mnS��(��|�c�)�{`u{����]���Ǚx��0'��Qʅ�M�� ����sN5%�%ם�[\��VQ}~��J*�2�)�ej�J��y�h���V�۰=0�s�3Z�&��?u�������Uv�7��@��<���3��?��>d��v
2���Q��`�Q�{�/)�#��3�3;�Ӎ�w��_�	��B(�#hh�j�d]�谼�	��s�A�O�.}�oyeuP1��'����E����S�P߯��"x+13R�����b�zU��6�Ģ�G�W#��w�����f�3G�T<f53�8���[��5�+�?��b-����o5���HdӚ/�O��aX�~ct�-B>�����!�݋�͠PaG>�j>�E�$ؗ�KLX��M'���`�uPKaY�2�j'�d&h1V*�	c"�*������$�w�����4j�6>
W��;��`H+�/��8��½�^�����X[�OJ�?�j���D��:0�ruP�~�cƴ�s�S�B$M
wgzYԌ��4�����3��nB��w���}�f��͒�\?1`�{���K��H\D�?�Y["�&�%�{6�V߿4�j[���G���Z+V�"�b5�s��:yጓ��@��Gb�;��	ڌWƫo�}��@��E��l5T��I ��W�\�`�q�\)�(���KX��Κ{�O��]��ez�i>K�fk�d�Ή��UA��M\*�W��P_b�s\Lw$yHZe�O�%�����	Er�����0�û��`3FdIh�Ôr�)�uߴp�荷Q�<"s("��}�淚�
�	z�j+�`���'���=�Jf� r�Z�r����;��
�'1!��*�:�RD�>.����4Val����S�魵��,'�>w� �q���L^��q�p��1(�_}aa�N�@�J1U�X�Q����P�>�.�
���tJ�g9	S��+��3i�'��^a⧗��M�����C
�}F�g�p��c���Luk�7,\�;�s��JQ��O�`M&^63ʍ��3�轩�3��DB��p���FR�P�����2F9k�}���K��hN~>�[�JG-�j�4�$!���
�%��ǝ�ʽ_k;�c�f�+݇'�����W;�.��G��s���FѲ@��."��x3J��sO�^�u�|�L�b�	<�#��k�Aϗ��n�\��]��_v�4U�_<�u ��߰�~b��9+�z�`�aW=F?�/jQ&wZ^�B��ں��!G����U��ī��R5lfN0�ϕ%��m�iӲI����Q�88;zJ���̓��t�{�2B&�`�ˢ7���m�$�lT�:�|=���� ��5�)&1��Q�V��b�j���=�0?VJo�����;������+[f�=p�T�C���Ku��T7���hh�1u}aH-�c�i'�B1V�{%i����猭@��x�f�"{�>
W����\D;�n��t����f�Ѕ�@�Lk�N�v��c�/'��Ycr�bpG��^��{.(�5��fnw����i��Ϗr��6J����m�J�$�̫iv�|�Œ4SZ�� %/_�?�Y["�&�%�"#�\˦u�Ga�ʤ�F���IiV�c�d���r0�æ����W�.��3��<��,�N����мR"��J� �(�G�6�6"�1�%��j������%�0�Hm�a,(�&�5!����\D��:�d�j�` n�,8/=$x~@u�T�o�Ӿ��sF����S�4�à��n:}_L}	���a��0�=}��s�萪&�02M!��D�����s�v�{OS*!�Zw�Ɛ6���[a�Hw�>����1q��1�<%��M�s��c
I�.*J���v�53�����y�ˈ���	# �3`�Y�~���ZА@2���ss�}-]\|�
�r�v&�H�N�ב���q��x�a���#H�Ga��m��c9&�)��F3������F�ȅ�
V�=��{��T���9c�&�r���B~���QJ ZMST����X����Z��=���?�.�2���Fzx�J���
4��:ReQ�8�!8l�>�=�?f���%�0����n�˸S{"�c�\�b��4꺔��?��z� ߽��a��&�v��4ʐYIͯ&2���Q�W�	�.�!R*�7�+ډ"�嚘�7�X��]��aq�=+�X�<N�Ɋ� ��z}/�]�]�c�*[W!|pXt��ؽ֘�A9���� ��퐌�h"|_n���2��[�T(*�s�z�q&M!��7����n�W�gn*
&*�=V�����	�~@�I+�]�"B�ȋ�;4ɬs�^^s��m���{P�..F���R� YJ6���(�ʜ��PvzL�'{�I�.�Ӂ]M��w�B��"B�86@��?L�^M9'BW�&i�
����ؽ�p�|�b���3/�ir��j��;v�~�&�w����C����W�(�m�D���a@�.�Cre�o?�B���7(�J��LL34*>{����fʽ����������}i�E� ���tR0.����	��*�T�NG*�2��02�͢/�7Ȥ��l���k�%�*C���
S������q���+����[�z�z��U4��J��-0�#�0�v�o�#e)�?���o�P�����O��]��ez�i>K�bk�l	�����<*��_2�f�H�E *Z7�Lx	9z}q�=�{�Ӿ��ur^������ө�� ]"�Џa�z�b/ۯw�镵h�n9B{���˔��#� r�:^ T_?�`������d�'��R�{F��ـUX��^�Xs2��j�ҜKmL�$o���x�~P3����T�퇜��%%�<g��!ԇ�XCʕ"c�[��{i�C<[Y�d�@�E&h1V*�R!j�C�������E�Z�6ۨ�9t�Pk0��0��Xn��i�<]������G�떲F�Q3��Y��;�GX�1�Yw,�%�g� ��QH�x�l�2#J ZMu�����粦���KMΥR���6��n���=T6:�:Ȟ�DD\_��NN~aK��A�h�8�q	�v��
�)����l���9F�k�j�.��'0����8>P�:����.��x�z�p�� ��0g��.4�C�)�v�.� ^"�#;� ��)�c
����f�X�Y���^q�P:e�4i�y ����<*+�Z�\�Jpp%�7RJV-t}Zr�T�n��⥇E:�����9�Ȧ��x4ze[o@���4��m�tN���ɤ�G�\3h�������	�C�id.7�C$͋�_���G�Ga�.�z�EOΫ��	���:��w�Z��M3D�#P���t�7^WQ�����5������I@}O�_��<ۈ�BH��c�^��n�%dKKa�d�@�t�m�j�󠀃��m�|�b���N7�Ep,����4o�+�^�V*�����9�l�軒'|�"(��E���M_��C4)�OKu��.�Y
�l�I�hxhu�����粦���KMΥR���6��n���=T6:�:Ȟ�DD\_��hl[8�Q�cp�W���{��+����f���5!�*�wR�b؊i=�Ϸ���})������S��^�ZݣZ��J ��h.��.@� �2�M�A�s�.���V�awŽ��@�����q�k=h�},��U�Ɋ�;[��y6�M�v�*K8�HZP@=t~VA2��߾���<)l˸���4�ĺ��Va-]w�Ԉz��/�rPؿ�Ԃ�h�:w`��+�˔��	�#U�S*/:
�z�Ƿ{���D�$�v�q�STԺ����V�7=��v�C��G}O�.&[���>�yUZg�ݰ��?���Ҝ�]}^K�[i�7�<ԇ�
T��R7�[��ti�m4%A��H�zo+X)X�3h?�*������/�P��ټ�J�>
#����:��o�7W�����4�w��,w�( ��N��,�ln�:�ci�6y#�&�=��
���b�����:;��B�ŗ��|��-=��3Ҡ�wR�\�z��̫ovc�|���)/:9��$ '�`�i|�Z���q��8����G��>q�l���o�MR��h}KZu���HW�F������u�w�@����hk��)7�_�3��z�-X/��3��D�
l����z�B�^� H��X|�fr�4P�p���
-��}~V�{�`�{,a�~[AQ14(>v�>�m�����Na��ֻ)�u�ī��R8lt\uݍ�v��p�d��y�Q�(0t(,��2�����	��C \P`n� }�֩'���)�Lr�I|�q�STԺ����+�= %��@�q��ao�=n���c�_rJ3�����/�Ü��$>�F�u�O���tu0��"Q�g��HB�M3?j��Z�g_Bxn�
O�� 6IVW�W��x����J�%qD��X��Lr�*�S�EԶ�ϋ	�F��!j�%#��C��>�pt�1�nIF�(�H�&��z�c��ZQnx��Ye
t�HFա�o����!i��#����}ma�x���[P~z��%v;��NV%�A�$�7���X�n$��ڇ���9m�*�a�p��b7񊾥�8��>P� ��%��	��^�z�z��Yd��_Vǀk�c��l�K�c�=�/��j�JX��ڪm�E�0�
ZşUz�f1O�'P�C�����	��yR�0�	�Pg_#�yACA(ogWc�P�)�����Cv��̺6�	�����YFF}R>��H��w�/Kx��k���Wy=Xo��4�鱯�,�C8�5LSP`a�];ҍ�}���b�(�Nq��52��͢p~��Q�W|{Uō
�=�G)T�8!���� 7|a_�秤�8ꅮ���?/�'w�X�}�ƚ  y��~���^T�G5?a�)�.k�k{'T2U�5b?�*������.�]�>�C���3/�i0��s��z��7��ѳ$X$k�Y˶�*g�>9��O��-�}k�9�hD(�,%�/�^��{.S�*�F�}mce����~��֕n�� =��3���W�M�5�͵�{}p�<Ȟ����L�؛��X޲&IA��8�%~�F��7�8����G���Gi9�x�	c���?�����\��Q/�)��X�Wƫo�}��)��B��d?`��B[�F�3��>�O0B�8�fЂo�SA����l�M�O�_L��Sl�R�1[�!L�����i��@G�U��FOx�G{FKHY��$�ﷇE:������֢��Z	g^9��G��]�V7;��x���5�W_{h��_���ҍ@�%A�2B&�`�ˢ7��)�GX�8��xy⁽�*7���ziGڋ�>��(X �j(W����!	e[}���:󔋚��$�'t�f�Aؗ�]_��Q'��=�8uPfK-�c�#�5!~n�=n$�*������:�J�(�=���P�5cG��B��6D��h�1F�����^�V�Պ�[�XI1���u��I��
�OKu�D���%ak��!�`of����A�󐃻3����2���Pi�a��Ѐ�F)@J�Z���,.,6��"5 L�\�X
�>�?�4>��}�2��Bq�]K�V�N�
J��n7����Z��Z3� ����?�*�(������yvP��MT�H�;�r�:�y7A�	0�2˔@�^򗟬P�Y�	�Y͓8�4U�1^�nN����A��y7�K��T
&F%�?,9CcPq��:兄͌}I�ŵ�������@)0.&���L��D�4@N��4ٰ���(8{}Ü9�˔���eX�^*i#,
 �-D���;��5�
�h7��XY¡����(�P<6��\�B��K<E�%-V���:�3Ql�뻞�,������IpJv�_�F����M��0G�_��n�t!X"I�B�L�QB=sl�)=�S�����W�7�|�b���:�g9v1��+��-R���;F�����5�c�軏!n�(6�	�M��>�pr� �nBO�j4�r���JA�;�I�菋���C*l�qŗ��4��&&��"���]i�c��ۍ�W)1|�q�ɃPfy��8*D�w�e{�[���{��
�3����z���<G��MB���?�����P��u�e4��wX�v�[���h`��c"��R�N�4�F�z�+W.�Ni�}���W_�˩�/�N�� ^��\C�;+�X?�u �����O�� �L�O�H&l�'(>@~{R!��h��䘶Ar[���_�(�ϻ�^
!l�ȕ)��|�i��6��~�0&/Up��)�����3�11�nXf>Ot�?D͒�b���|�7�Zn��������!�=%��M�^��G)	�GE;���2~a\�����������?"2�;��s���Du��L�v��xy�q
'B���K|�&i�C�����+�j�#����og�C,1��b��7}�b��f�����]�(����.�zl�M�d��J�`i�<�~ST�;�H���9tz�
�#�on	up����l��ƈ��
��	���}S��n&S	 �ٯ�[�0a�fy��hl[3i�8�Z]%�>���`���"����l���*]��\,�X��H�������3�+��E���j�k�4_�uȢ}�-2��)7� �2�M�A�s�/���L�}~ݠ��J�|�!�9u��J�I^�y�4�����E ��'��i�ހ��?�Ο���9�>�I�����_w��Լ ��򊏈d9}eP5F�����v�bP:��_�Բ�O�S$��V���ŗE�"T�U-	!:�-`�ˡ6���b���6���+��1,#2R[ʈt�::��w�X��K8�?;F���e�eK3�����a������	$($�4t��AϪ�$_��L!�_��xy�q
'B���Y|�M�I���ܦv���ۿ�7b�G;S����6	k�l��LS��ǝ�R���ߝ*�M%��;��1��E�p92�y=�&�^��QK�7�D�J ^JP-����p���ǂa��*J����";�-�'�݃�[2FG�V���.C��>>;�\�Ij�|�8�$�Tؐl�K>����l���e�-�>W�?��"ܽ�����G�����)��n�c�o�� ��R4J���Z�3�]�>�y7A�T�;���H^��֊M�e�0�:h��tR�L_�g�] ��ԝ�ag��6=�|�v�q1{�?@LZ=%LG\h�S�s��𺜍��rY,�u�P߯��� ]ooAx]��]��K�G4*�\�ȴ�@�S9��B���΋W�"?�sTD ;n�p���r��ڈQ��?�S�H��⟀U\���s8��J�d��}N�#!W���7�4CaZ��8�����'( �F�o�Z���ye:��y���:t�|0Bn�AP��I��Ծ��A���*�w6��אq�Q�5�"���	S�:tN��G��Lr�*�S�EԶ�ϋ	�F��!j�%#��N���Gr��Awn��v�!��V@��(�kf	".4P��ϓPƬ�����K� B�!*#��+�y�'\�vn����K/ҫ5'�Z�XF%�{�c�a}�8��<�i����Q���%9|�%�5^�-��,��꤁j��\8�&����<���3��/ѯh`��l$��T^�Z�P� �f�0a�,�/���7A坟�2�H��]��0�l5`�p�,\�����L��e�t��Owa�Jr-1q�x�O�����Ul�����9����^pf S?��K��Z�O&0��t�≌a�)"{%��t����a�
q�#>+'�-v�ĩ���d�7�lT�H�����:%��.�!��o���O!�(+[���-�3W~�컵�i�ߪ���mG
K�_�R�x���ic=��8C�m��vB�y;4c�1�)b�}y=W#D�&c.���ޱ��[�p��BԺ*��n��ϺL"�	�_�G�i�7A�����9�e�㢒$w�%N�)�nݛQ�(P��Bd�8�O���2{c�&�9�^G,ug����L��鱙J��
��T��#��p��t@{o�vn����K/ҫ5'�Z�XF%�{�c�a}�8��<�i����Q���%9|�%�5^�-��,��꤁j��\8�&����<���3��/ѯh`��l$��T^�Z�P� �f�0a�,�/���7<����a�X�M�]��0�l5`�p�,\�����L��e�t��Owa�J|&#G�>�߾���Xl��ݶ ������h/0;%t���[��P�Y"0��_��ȼl�4?M5��r�����a�~�xF$v�v�˦��+�Bb�Hc�����Vq���*��q�R��N%�)"B���<�<$UKh���,��Ӑ��[@}O�_�0����[Eڍ2U�b��IX�G
"P� �?~�vt$X:^� s&�-���Ǆ��j�.�K���"o�E��V9�x���H�z���a����l�/����L�F��B��8�ci�'�Lut�,5#�S���8P�-�^�K	.D3""��r�	PF"�бG6�W7�M�.�K�ˬvc�|���)/:9��$5-�j�bb�A�$�?2�Fǔ�)����v���/J��+�x��X-�����M���H�UTӹ�!���QƹV��''��]q��mq�o�u�G�k�kF�T�0��{�qsԺ��]�m�0�6n��tH�[A�3X�u��Ƒ�rt��X6�M�v�&K4�>
�������L��¶��pNG�����ӻ��`2$2,F���a��m�YUչr�⿱{�;$C��ܳ��2�3c�uD,�r�Ϊ<���}�#�6�A�0=����d\���!tP��v�^��/[-�C?G���e�>V,�����)��ׁ��Q}E�b!��A���L��GT�{̓*/�@eC3q�A�P�o�{c"�*������$�Y�Y����n,�`:i8��I��;I��b�; �����9�W�ȍ���.�i���MO��rtu�$�h�n״_)/�8�l�	gx ?P����Mښ���R؃��Q���}_�{�4����OW)9�`���G[ZK��h\D�?�&D#�z���m��#����U���5}�"�\:�s��r+�����j�����9����2�z�'��B ��h.��`m�c�J���O0B�>���Z�zn����I�`�*�WU��_`�4U�_<�s��φ�xi��55�}�o�`.I4�r[Q](qLcd�D�y����E|���e����i2a)39����=��x��c���Q�}9p)��t����f�]'�i)	!:�-`�ͭ6��/�Ha�Wj������ng��^�7=��v�Q��G? �j	���;�4	.�῱�T������@gW6�<{�����CC ��p�Cߓ!�%1�^�P�`QIF8�T@��Κ���+�.�C�3˦�w%�E0e��0��\m�G�i�7A�����9�e����F�^]�M�bÚ%�fd�*�hUE�T�d�:��QQ�*��PK#C>8Q����[̡�֤K��;��T���0;�-�L���2T6r�a���"��%f�X� #�z�8�33�T���W}����G�ǩAV�>�	���q6�Ӿ��q��l	���<���|�a�k��6��h%L��Ab�m��`�Z�e�2�	��j�PE����f�H��P��|*V����XM�g��č�ns��4+�w�o�h+}y�yQ@L`&9�A�b�����Xl��Ҷ ������k2keFh�;D��j�s�C�ȴ�@�(?|x��;����*�CS�\Q��͡C�	���;����h�7�l�oi����b`��s�q&=��\���F8I�(:T�۟l�nv=7{����������	3�4M�X�<ɇ�'~��v�דj~�hn}aHG��@�H^x1�x%��������>�|�b���7*�Z0l��;��eVF�"��8�񱈭V����[{�E;�u�0��Q�($�x�$YB�#�C���1j�-��J ^.<����p���ʃw��L�����lB�M�$����{Fw�s���	��7�[�|��X�\L���z� 4����@���(|�7�s	�fԌe=�����S��v�:����s���n��b�oc��{9Vňk�c��l�Q�$N.�(+�'��@�}cϚ��n�I��L��[l�}=O�x�Z'��Ŗ�wu��$;�L�G� <8�sP]f2��ȧWک+��(D���}.[�u�P߯���DFSmR9��Z��P�R5'��x���5�W+}6�����āJ�>R�^<a*<� q����ƨ�@s�
;�G�wm�*#��8�6 5��v�Y��}% �$+Www�ϑ�㳢����f"_rw&�@}=�X�T�h���E��H�^��sp�%d@PL.��Z�g_B~n�s������v���ʣ�37�Wi7��~��
~�n��E�����̇��}o�/�y��J�WC�� 	�7/�;�7��5�N�F�OJ*Zjej����	�������_��M���I��Ȣ��"KD?�;ׂ�Y^^B��ZQsui�<�nv�A���u��<�4�9Ğ�RK�
�\8�U��Q�����\��F.�&��@��>��E�f�3�� ��o#Y��HX��`� ��97A�\y�i��z�}|����]�_��WL��J4�`:t�;i�i�������+�	��.v5"[�?(>?rzW\N�s�B����	wG��ǀ�)�����R>wtIw�Մy��k�r߄F�ſ�G�	@(��X��Ш�j�/�nCI*6�];GQ{�]h�m�mUC��;�$�Ն♀UX��:�1?��\�p��dl����=�|@:4s�����=������?2�S�I�n���L^F��>E�B��{v�G!1A��\�gm7_5^�<��������{�X�DAf(��"XS�%�tO���7C�P�t�����9��ހ�|��.�n��Z�/'� �Cb8�6a�b�:��BC�u�;�2#N_>).ΏϓVʬ���\��=�����)�.�"��8[iv�eܼ�bԫKA!;
�A�!8E�k�8�ld�Wوw�\\���P���Xk8�q�[Z�N��Z�å����p���^��j��*�N��1ڑS[��?a��� ��m�G��#	���q�[I�����J� �;u��h]�JF�zY�x����EI��\=�P�v�Q.>�s\Lk+huU#m�Y�6����Aj>����P�}�û��]$mrzy�͕<��t�*Vؿ�Ԃ�h�oO[pͺn����*�a�gn*rEC�{�������}B�>�G�hI����n��,�7& 
��\�R��G#A�?=W����2{jK�ݔ��ʫ���9O�r{�Y�d݈����m�&�x���,�݉)��j'�zo+X)Y�<h?�$������>�6�����J�g9v1��+��-R��j�!I������C����+q�8�7����RH��Xde�2�R���JQ��F�QM"R~hu�dsԙ��ʔ�Z��#<����<�Q�/䪞�2{q�|���öPQjCn�@�BFt�j�>�5/�mܾ3�<gԚ��9Ă�/8��#�r��X;�����P��z�t��e��;��0�c�oѯh`��l$��TH�C�f�#�~�,S?�^=�!��?�3;����b��_�O��
,�?kM�>B�z�����a��B/���yUxa�&{* L#�:������Bv��̢8�u�ȫ��$l
!u��P��F�C30�S�䂷g�9g4��~����j�b�|OO&'�a�Đ>���p�.�kW�>�=��͢84��3�'&(��v�C��A<P�l P���� Qe7߮��,��Ӑ��[@}O�_�0����DNT��9^�t��NB�~!V��Rq�{y=^~�X?��������{�!�%���C�8lF��_��}r�:�t�^a�����I��5B��$%��+�|�l� ��'O�5�U���&ck��*�z{oC9#?\�붹D����Z��&&��"���]i�B�$�Ƴ�s+J�V�����5<>�T�Y@)�X��	�ˢ;�_����W���Ut�N�9Ç�6��U�'D[u� �9!�E����b��:�q�v����`3��0�#�4�]�Q�bU)�0�$��y������E��IcדUz�f1O�'P�C�������y7�K��T%_j�<F\(o3\q�Y��՜�=Z˥������Ъ-^icEl@���$��A�2(E��0��۵l�mlQ:��+�ư�[�_J�*MON}{�@#Ι�p���D�lC�(�v�^Bۯ��6?���C^^s��+�u��kr�	
{�Ł7�1^aa�����<������?/ �~}�Y�u���KL+��>�͚�cXzba�A�K�{f2bx�_����ܱ]�|��4���!L�70r��e��yT2�C�"�s������K����,j�(5��N���Gr� �AIP�:�J���0qk��(�J@)^o.����A��>d���]K�WT�;�_�K�݃�[2FF�[���,%,6��5$f�Q�Tw��\�')�V���.����w���2g��v�I��I��н�\���o����s㍭~�-K�"��es��2��)7�_�3��z�-X/��3��v�[�����Y��GcדUz�f1O�'P�GXߙ�ɻ8U��Ei�M�F�@!p3�qSzX,s3\1�:��߾�b1C����r�T�ή��'wiuFu	�����w�rMWׅ%����5�Wv#��&�����X�\&�*S}Z;u�$͘�͵�}�u_�P��5,��үd\��1�<%��M�h��lyI�.*J���R�<qL�˛��ݡ���//�}�I�u�BH��"u�M��u�q</	4B�1�@�R8{ �	!j�C�х����S�����2{�BCc3�-��1r�,�M�%1�����K����S.�~�M���`�MD�U�oRI�9�Y�!��MJ��	�N\"u {����|���܈}��*J����m�J�$���ݫ�%�YQ�a~z��#%;�p�hm�P���`��;�)����N���2m��G#�I��F�����A��]� ������Y�\��j��Q��-?Q��\�v�J�5�?�7A�qS�3��k�aE����f�s��U��)�4U�'_�hV�����UI��9�b��rB&$�yAi3\e�^�i�ѹ��-����r�P߯���w3 XȚ���q�tP_��c����T&g)��9�侻�	�t�j^LN|�*$���>龼�mJ�Qn��B-��ϼmd��\�uY]5��J�h��l����=�|@:4�����׭����%!�=p�T�C���YCہu�B��xh�~y	%M�1�H�gG_7�Bm�j�󠀌�[�1��(���3e�@u2�{��%H=�,�B�!j�����;��̑�W�GO�J�ؿ`�/'��Bd�8�O� ��)aK�*��qL3u2.T����Vڹ��n��;$��)���_c�g�޺��|!J�5���Z_�<�����$�uȁ�=�.��[=�8���#����}���=V�
�W/�>��c<�����H��E5�+��.����_�Wݵq����h"`��PM�^�	�[�{�|�?���v�J	χ֬a�I�F�P���}���5��Z��Ɋ��;)�� g�M�F�@!p>�cALY, "G�>�b�����h~Q�����;�����6Z	]u̐�K�v�b�c��k�}k2G{������q�F7�yPGrEG�u���r��$�
�h.�]�n ����R[���nvJڒ0�l���'?	�w�gC�����ܠ����%3�\�T�x�߈'~��?�i��SB�]5"a��G�gDec� ��������q�����!q�Vnw��;��_m`�D��^'񬗱�+�Q�ζ�V�N�$�{���FD�*�Xpf�{P�*�&���x�l�B$MWZMW.��������a��0=�����Ft�J�)�ߠ�y$Mz�d���;,=/��-5;�`�of�6�8�%~�F��;�)���{X;���	�$���\]�T�̓��z��z���(����\�_ֵM����$m2��)7�E�9�f�J�e�=���v�[nҖ��g�a�� h��Km�PE�b�RF��nT�:����甫%��֏�ʯ�ؿw:�pؽ���&e�쳝�4�����y>te-M�����w�>/��t�≌l�;0IAt������w�ZH�:B&�r���7��m�
�hW�9�yu�§!���uY]��q���W?I�qB8���6�!F,�����)��ׁ��Q}E�b$��h���i9�M'�p��,@�({DPq�c�ia�|b+N%O�6s0�-�����R��x�a���oX�.zI��H��!3�,�B�!g������M�؍�7A� �(�o��5�WK�*�Hx)�qU��(��l��Kj
z$3(v����M��������"���m�y�q���{5%�.껣fy��lk'!��nv�A���Wۯ
� ����g���<G��MB���?�����P��u�fLa��WƫU�W˹@��,-��J`��hi�x�4�M�v�y7A�qS�)��?�|uٺ��J�h�<�Y��|M�L_�zC�rӛ���_D��Y[p4��.E%X�~[Qi3\e�^�d�ë��-,����v�4�����-8{dAu"�ԇr߆-�=}������Q�}9p)��y���ߤf�L*�*X-vv,&� 4ׂ�1���@�'d� ,��jh偖�R[���'iX��,�ɇ/[)�)"B���i�g o9ڲ��5���ޅ�Z:�b"�f�,���w[��K,�	ƾ�Z9"w� �?~�vo�[m�j������2�A�4�.���ox�Zi����87�,�B�!j�����K�ㆮE��n�6��g�+V��6 �m&�E���v@�6��OJ*Z{a5F����[ݮ���c��H��⤥0�.�<����vc�|���)/:9��$5�Z�IF>�m���{��8����|���5}��#�r��.v�5UX�5�9!�E��.����_�WݠX��$ ��H��@X��P�T��-S/�3���M�uϸ��`�I��c��t[�@8�;X�$1����TX��_��G�	ԥ.E%X�~[Qi3\e�^�d�ë��-,����v�4�����-8{dAu"�ԇr߆-�=}������Q�}9p)��y���ߤf�L*�*X-vv,&� 4ׂ�1���@�'d� ,��jh偖�R[���'iX��!��+!C�:r���j�K c(��釼cݩ����R/v�
"�e�)���pL��<5�&���2/m�=�.j�hl,9d�TI�n�����9�W�.�.���R�Qd7��d��!v�b�I�a�����J�@�Ս�
V�9�"�"��Z�/'��' �bnS�c���PW�&��K7I:7v����P����e��HR�����5��K��̫ov������#;�8�W?F�>�$�?2�m�������@���>q�N�#J�c��sa�þ��f��W�!μD��4��E�3��$�BI��d8K��P� �8�L�x�O%�Uq�/��'�J����/�!��0~��sG�BO� y�Y=��Խ�T��}~|�\�@�A_p�Xv`gB^r$!�^�n�����,xZ����P�4�à��B]IaC>��@��|�zx��y����A�[(��D���ÍP�3C�j 3+'�1y��=���>�cX�3�v�IBƩ��b\��"�rZ/J��A�~��ki�}����4y>�����T��܍��"	>�W�b�U���df ��"Q�e��US�((\7L.��La�|b+N$O�6s6�-������4�2��7���M�YO��B��0D��~�0\����� �n����.v�42�	�;؞6�($�A�bTD�.�G���#q`��"�|vk�����ŗ��7מH-��4���\b�l��ݓ�M?MV�S���.9,6��"5 H�H�p?F�e�}��{��#����|���>J��M)�Y��7$�듟�?��u�`��C��u�5�<��0ōoc��=zp��mt�y�	�l�I��,���X�awٿ��]�j�!�+s�
e�SU�)�S,��ɛ�ta��5:�w�v�t:k�B{f`S?-�:�ؽ��d^q��ڭ,�	�䎉�_FVgP)��K��W�@99�D�͵�M�>G=͖����L�(^�E1e=;�&@����ɮ�kX�4�g�T1����R[��t�gx5��J�h��ve�|����/hjG�Ǎ��������21�F�s�,���  }ŘQX�o��EO�Y/.c� �?}�yy6N"C�7s&�-���ǀ��B�{�f��q;�(wI��N��*\� �t�*G�����#�r��!o�24��E��%��Y� 	�um3�B���3ea��!�qly
{����f��ʒq�T��p���?���ֈ�M(\A�R���($>#��$>	*�s�`f	�A���|��(�;\��Տ���3V��G5�W��D�����j��}� �\ɛ���"��P��+ ��H`��nx�~��x�O�s�#	���V�jt¬��A�x�!�OA��7�ti!�t�U'��Ņ�ni��57�w�x�e1f�Hspz
I\}P1��߾���d^q��ڭ,�	�㡺�c2FCGs�ϕK��t�cήd�԰�@�^#��]���ԇM�\j�Fn*u~}�=V�����	�ki�0�l�ur牌� ���
2 ��\�b��vn�z2ʍ�Q�dOqL�˛��ʻ����?(�B�s�Z���hx+��3T�d��*`�DXzb:�B�"j�qt0T7N�4Y�<������.�]�>�G���cH1h��I��;H��y�k�����6�l����<f�( �	�_��F�_�x�$m1�n>�O���7`z�&��^G3ug����	�����7ך\N��9���Js�{��͈�B.MJ�`���
/��/>(�p�st�]��}v���_�`����w���(H��J8�_��R�����A��J���8��v��6���dؒN&��D`��pb�z�l�@�h�0_%�8���z�MD▩�H�s�&�+y��|M�L_�~�02�Ή�� ��4=�k�h�{;i`�~SCQ;x}G`�h�y�Ĭ��-Nu��Ц0�q��ね^9"p���$��[�O9*�B�ϰ�<�40t2��o���ߘq�d�h,8�;X���д�aI�Q#�o�������!�=%��E�s��D4R�$;[����0DK|�ݔ��ʫ鐅�LzL�) �r�X���~o ��c�Y��|�)
#({� �E�g_|o�X�������2�J�#�-���X�F}r��fHT�
�b�,M������P�΁��$%��e���}Y��Hda�73�@���9j	�>�:�5 <{`����|����[����O���Ki�J� ����wip�t���4#7��'(!�j�y|�Q�}�,j�?��.�K����l���>H�7�q>�s��5�����[��X?�;����;��h�LĥQ��-'��pzc��-F���`�Q�n�-�4��z�J Ϡ��P�Y��c��Wh�}9d� S�L)��ƍ�ee��6!�m�g�ds\�N_Wz5^�r�O�����:EA���� ������}"m_|y�ēu��k�Y6 ��B����s�[~;��T���ɗP�8H�|E+=+�&�� �ʏ9�[s�5;�C�h|���� ��!�1 ?��E�y��m�7cn���#�y4uzG�݀��������4?'�`M�~�C���]_ ��z�N��nh�}
6>g�!�L�woq�B /�'������$�p�����"{�:tN��G��;]� ��0$�ޙ�l�Y��Ȕ*g�>9��O���KC�*�Xsl�2�R�ƴy�I��h{����ڡ���Q��0�����^x�b��ד�T.WV�Z���G/}��Pbb+�q�em�_��#/�Fפ;�3>����l���9F�o�~F���|h����|��`�����?���Lͅk��6��'J��tz�u��}�\�o�(�j��3�3;����#�n�&� n��~G�a?y�7i�y�����DT��.�\�o�j<`�'H	hDHM7�x�O�����Nv������Ȭ��@6kkwH�Дq��^�Y6<��S�Ԧ�@�	\[p��G�̗��4�#U�S*.:
�7z�ˡ7�ǟ#�FX� ,�l�uo������*�6d���>��}o�{����R[j�ҕ��ר����&9�\��0���$QE��2U�b��HX�\
#$A�1�5`�ld,=Fw�.!j��Ά���2�A�4�4���(z�Ae-��L��-W��t�+F�����V������[)��B��/�fr��B{e�0$�@���9j	�>�:�5 g']gW.�붹4�ŗ��Wȋ0��"���p_�I�����aFE�z���Y��l[G�[�m8�w�a�\L�Aʪ!�q�������>e�:�3Q���
S������z�E��a��S��2�v�f��^]�� \6��)7�#�+�H�e�'V*�Th�vә�78�����@�
�u5��7�Q�[?�x�������}~V�D�#�-rB&X�xG]t-(Z@0�^� ��ҟ�Aj>����_�P߯���kVbLu�̅D��z�u.ӆ8�H�35;zJ���˔��
�#u�s
c<�q�є;���(�JA�=�Z�!���V'���<68��2���/[)�C2?���Q�Xq>7.�����y�ܭPvzL�[2�7����P'~àw�&��07�2ZAk�D�J�H;\ �X@� ������)�Y�%�8���$e�_e=��d��6;�d��;F�뽃� �n����B�}=======
b#e� D   E-SafeNet   LOCK            ���w y�3�����)��R��f                                                                                                                    �تPV�c�#F���
S񗴵�S��Vz�o��^����E�d�p����l%K��vX�O�+�[�`�'H9�-�+��q�]<(���n�X� �_��~8�(|�1W�u��ԡ�\���-�Q�*�Qya!QD@�O,�G�b���z�0����;����d99pZs���4��}ǚsU���ꔧk�!@�{���;鮬��`�	�$EC6u�1[����ͮm�;�A�~�yo䍎�<%��� hU*�����(JC���=�&S{�淳�|�����	/4�+2�j���lM��r�_��(1�0!a��`���(l�j�`�6��^6�������Gz�<ȵ��)��s�v��g����cv\��TQ�K�>�	�g�*?�y�/��q�[����%�u��]ړUz�n1~�3�s��EH��<<�W�L�Er"[�7(>i=9\!��+�߫��r>�����s�ɢ��b1mwIn�͈w��j�YȾs�Q�44w4��m�˔��)�f7�0Cu*=�Rf�Ӻ;���m�W'�.�Z�{���>&���4&��#�X��LqW�#;[��x�:CIo�����y�·���?$�&w��r���^ۍ1~�N��=�ku2P��Z�\Vk�R!�C������(�M�Q�a۵�g@�>{Oʸ\��%U��x�1Z�����/�k���'e�6(�g�B��F�GD��_6e�23�u�<��IU�*��2#g
}g	A����\����P��D��]���9E�A�(����~wr�t�Ԋ��6n�G�EA?�q�"�0(�?��u�G=����R���()�-�f�b��'�����P��9z�e��L��p�0�9��5��HC��'|��� �w��$�ha�Vs�lЈ5�����%��E�U��2�%r:�x�6D���-��P^p�4�$�Uhf?�{@AQ:=>Q,��&���e<����R�p����� z4-1P���9��4�+]X��;Ԧ��#�p{?Zm��6�볪��L�9
N:*�R6�Ѫ ��(�K)�CS������;4ץ�>t5��J�h��l\*�GE;���w�zT!�����v�ˈ���ObXi�}=��3ۈ�[��R8�Ҝ52�7z_Dn�A�O�1&h9H�;e?�-������+�[�.�,���?'�,0j��$v�Ŭ]��U�����k�+����!|�62��T��&�cj�*�jX=�:qL�C�dƫY��U�p�u�������)�ݱ�v����{��-,��.���Mi�m���/Ka%�S��GD\�gC�����!�h�����\�X	�v��
�)����`���%@��S-�+țA�񲺘�	 ���.�c�����]��+6��^	|��wh�o��g�>�r
�:k�i��(��, ;
��~Ѝu5��3W�KY�x�Y-��Λ�da��9 �i�g�i�;
i����(:�ؽ���Xz�����8�堲�l9m_|y�ēu��k�Y 2��&��ЕK�z=Xo�|�g)"~�d��j���ɇC�{����̳�[b�>
�]�EM̦߽'a��;�>{U\c�B�$sƃ�\�B8���Q�?~wQ�糢�8�����]} �T�˪5o ��ZǠ�[��,_}aHk�*�)q�qy=X8Z�&s1�������+�P�A�[�ӬwV�E��X`��_m�G�i�7A�����"�D�ٖ�W�
��n���Gr�2�&x�r8�4�|��&bL�u��5 J ^`����i����X�������kS�W�3�͑�\F)m�%���-G\_��'S���t8�%?F�Q���Tʮ'�4����`���5}�(�\�{��u8�����e��j���(��z��5f�6V��B:�ְ��Yf�~'�T�#�A�s�=?�#��m�_E����]�E��Y��_u�j*q�'D�C>�������CLs�y����q�Tg�̏����ɚ=�߾���Xl����/�ǻ��^K_gi	�ԕK��t�cήd�԰�@�m."G��^��:;�y���:0c.̹�>��Kj�X���D�lC�(�U�oߚ��+c��=�'!��w�c��R4R�>:@����hKO>�����v�^TP�W� ��>�6i  }��tX�o��EX�<b��?~�vRiF:�7Q�L�I?&�&��{�:*�M�`:O��B��'x�'�E�*g�����6�l����Xe�3 �h�� ���~x�$
�8�O���J@���p\3Z"3p����\�����ԛT$ѺI�����g�~�.)E��ܱ^�껣faz?��((
�\�Im�p�9�'�Y�:�$����}���JkO�s�/E��ZU��+`p!D�1?���M�f��P��X�Z˯L����J���o��߽4��F�uT�O��[�wnۚ��b�X�
� l��(�?hV��3ARo@+{�-��z	�{�`�{/aj�8/9CijCe�Q�x�Ū��EH����
�3�����]W_aR9�����9�D4<�F����I�f[W`��f���āJ�<P�E7YtC���X�Ǿ�qW�/�J�=Ī����0�< ;��>�:��~\*�1����
>yyG�Ǎ��۠����a+K�[i��^���ro:��3R�x��_Y�H>mx�d�m$�CN^4N�;s"�7������7�_�4�;���kK�`:{o��I��;Y��y�!^�����#�r���4.�~k�N�9��(�kd�0�j_N�(�T���;az���c$M#,uvk����f����{��0#��"���\i�c�<��̫v�&���/":/��&(;�a�|m�S�����_�b̈��z���4@��P>�F��N�����H�>P�B������S�LűL��-$��@`��j@�v�W�R�"� ~�#���P�mdݶ��H�Q�3�u5��3c�#R��U ��Ð���1:�m�v�c15�<^eRQw5H�~�A����@f��ǻ+��ၠ�QZ	%t×�[��P�Y5ݲx�ԣ�a�<8f(��k�����q�3G�T rEG�D8����ڿ#�Gi�>�\�{s����2!���!1%��M���(X-�C4����$qKk�݆��2����ȼ<@}O�e>��U���XO��y�t��T`�DXzbL.��La�|b+N$o�X�����ѱ'��
�G���^�8Va=��e��#U�b�&�ws�罇�/�}����!m�"2��j���[r�2�P:\�HH�*���?{d��/�o|u2;J����f�����b��mo����Vt�m���Vs�g���
/��";�j�k|1�B�[�*w���.���w���)l�7�b�F��t*�����r���E��f��k��U�W˹@��$[��k0Z��JI�K��[�}�7H.�,�;���7J��ڊM�e�0�/z��S~�j*u�&_�p1����DR�� 3��$�-rB&*�%jvTZl=g�R�_�Ҿ�Bp�����6�����}2ka\i���S��5�z|��$�Ģ�G�t%��i��걗P�>H�tA:�d�Ю&����iI�M�9�a,�ĭ���?�6&.��b���a�$;S���,�8RLo�����	٪ڐ�jDw�~]�y�U���YO��v���YO�q;
*A��^�JJdo�"k.�O�󠀥�q�e�E�$���H�Ge,��n��d;H��d�
x������G�ݐ�F�'!�:�W��c��:�i_I�'�t�!��z� �9�J]7_#A����Xܽ��n��N��w���M���ֈ�M:a�g���
A/��3>;�@�sf)�n�#�%3�W��}�ʤ�H�¬5M�
�B�s��u<�ì��v��|/�?��3��7��h�g��z��%'٨Q[5��)7�Q�k�f�J�e�0�#��j�Rn����#�!�f�N
��x\�FU�5_�y1����ag��\(R�0�#�_N�S|lkozPh�Y�_�Ů��At�忮r�T�����OPIwZ�}��F�iϾI�̮�"�P\zJ�*���J�3R�}
N +�-[�ֿ'�ڮ"�_r�>�}�6A����ni��<�1?��j�R��G%�?;B����'
B[Q�Ŝ�����|Nj�V�t�Y���CO+��t�t��hv�L:4A�>�.S�d&h;�^c"�*�������A�����&e�5go,��n��*f��!�sװ���)�f����F�9� �v��g�+V�E�bTD�/�c�7��1jX��l�6$M#,uk����p����S����-���5j�.�a���2 +$�Z���"48��%<�j�k|1�B�[�jK�?��_�m\�ʤ�J���=)�-�nJ�T��N�����{��kW�4��e����E�]ˤ@��_Yܴ=f��.4��r��!�md�Sv�iՍ0����� ��@�P��7� w?�}�3A�����_[p�0�#�k=f�DpqkT]l:H�c�Y臧��x!�� G�3���שB]IaC/��K��W�6E��'դ�`�� [9M�����J�3D�N<c.*�&K�����}�6�Sr�P��*]SJ��{P�W�^^s��G�~��gd�	g����/t>�����u��}}1����v��5����'#;��4^�x��EU�K?9{�:�?h�t'CK:�BP�O��Hd<��o�p�4WwJ�%qD��Y��;S��d�'W����������G�Q��2��x��Ɠ� 	�mnS�)�|ūY
�l�I�hxhu�����粦���KMΥR���6��n���=T6:�껣$)79��2$!�r�bw�N���|��3�����ǴVt&�W���WX��{|G��<��w���-����S�VݣO��&T��=f�� �z�M�7�8��w�T�K��7~ҷ��P�i�0�>h��cG�@D�s�L)��ƍ�ee��@o�	��ɉ��E�����ʹ���>�I�����Pw��Ҧ+������L\=dM���8������m�Bq7y�P\zJ�Y���ߛF�3R�]*n9 � @����ڪ
�qA�-
�a�*e��һs~�3�]����E�4�"e/[)�{����$rpI�̗��߶����-Jv�b"��3�`��ҬC�Wx��_yfK-�,�)g�kn6N3D�;b%�0������6�A�6�4���R�Wxn��<��z��7��ѳ$X$k�+����!|�2'��E��$�}~�:�rBI�|L�6�cݨY�\�f�a���Ѐ���ڶ�0����a��0=��.���Te�i��Տ�T2UA�Gځ�[ZCH��Nא��R�l���B�Q�[O�p���5����r���5Z��>Z�&��?u��(T� T���Q��Pů�\��V��1,��Du��pr�u��g�G�e�2���Z�����6��@���p~���Q_��C<��ě�j��$;�h�h�p6vl�op13S!R�K�ҟ�E:��׻6����j>wew_�̀z��M�k Ⱥb����^�	T>��I����3�@8�5L�����FĻFX�U24� �
�h�w�SB٭�� �����b�C��G<P�8.F���e�-KO������_,&����s��4�R����  }��9^�t��NB�m04E��0i�gm+]"O�O�R񼆦�+�&��u�)/R��c|B��T�� D�;�G�*i�����V�������XֵJ�9�|��( �|�'O�5�U���EF�"��on	up����A��������@Y�YlG��e�c�QS~�rVW�\���.*7/�� %/
��T|�]�~��T�m���Ws}�6?wQ�J�	c�T��N�����|��z���=��4��n�LҷQ��+%��_k٘��rE�6�@�7A�3���M�uϲ��P�b�:� m��Tl�{!-�*�,*��yI��
X}~V�3�f�`6p�Cjad,WZg�E�e�޾��cV�����萛�A\R,���Q��6�V6Dkb����A�[(��_�����G�
q�hN."�"f�Ѽ'���}�7�$r��J[�9[?�\��s�q>��\�r��ff�&T���=�!VRQ�𷲍,�����?>
�@��,ŗ�[��;%h�O]�W4"{�+�$q�hmPUl�^��������m��V��w'�F/��9c��G�!�1J�����#�}�ڐ�w��v�X��5�LY��rbe�2$�r�!��&ck��*�z{z/wk��;)/ӝ�7!�7�Pp[ۀ)�H�Ѕ�[$KP�Q���$�� `7�g�E\8�u��4+�W̪!�4��ֆ�ǠVt&�+��J'ذDlOXmy�~ >P�B��#����I�Gݱy��=��l"��go�C�)�B�Q�'W;�8�3��@�yϵ��[�~�_�O��7����w�ۭ<=��=<w��	Q�yU�v�m�m j�HtC@,oGA`��X����<pX����8�ǻ��hj)x,L���;>C�ƝG��U�6����zJ��܉�āJ�?R�E"F=�u����ץ�Zw��V�jx����-4��9�'6��Q���`�e`�e4� d[d}\1tm0�(,����"	>� W�y�]���_~��	&�x��ER�l%p��\�L^tK:�BQ�O��z;�/�"�9Hc#����T���� R��n�!I������C����+q�8�7����RH��Xde�2�@���$�s�W�h�������z�{k6�n�o�2���Pi�a��ӧ�w	Mg�t���8(!/��$:;�P�\W>�j�#�lv���f�H~Z4� Š)[� ٤e�0�Pܽ���q��l	�����4��\�v�O��=/��Ym��\��q�G�7큱}���c��48߱��F�~�.� y��Sv�Pe�>�d^�����iQ�ϥO��iӾ�"[�?,{YZz#R�c�M�޼��'l���
��၂�DMEz!M���%��6����{�Gk���[>��Z����`�x�E7O:+�Bl͒�g��Չ��;��#>������-�,��m�h��Vo�>?G���7�$Ra^�̍��ʡ����P|Fp�}=M�R�O]���[����R�\94a�*�%@�QEN8o�-c�������
�{�A�[ˤ�k8�ݑ��%b�	}���:�ˢt�����"�k�7f��"�I��	�}`��rBo�34�V���?hq��V�v{hu�Zf�&)z}�{��_jp���[r�m��Ӆ�W~|�p���/��
�D�I|�.�`�}i��}�A��s]�KE���I�`�Y��N�����f��V>���3����H�#��.ۚNF�7��欢-r�n��{�O�i;�=�v��/���Ep�h!�f�u5��~Q�PU�}�p�����B��>+�u�l�O0`�.
��ܶ��=�D����Tr��؛�0�ҽ��0ga0-D��έ/p�t�}��k�Ģ�G�_3��_������n�E_<:�w���)���m�lC�(�~�ETŬ����!�6"ѯ	���L%�>oq����:l:߹���y��ڜ��$>�V�i�l���''~��x�X��U_�Q/(`�:�K�wIxI�w=�?���ա�T�1�^�DԺ�h8�E/q��$��zK4�a��Q'�����I����G�XI�h�$��E��Z�9/�HH��sʤV_j��/�vg~YԌ��0��湾��T��Q��)�\�%����"W)m�$ˁ�[X_@��QMa6]��T~�.�i�}v���e�Va��ֆ���Jk%�;�3F�n��+i�����M��j�v��\��?��b�u��gڒNYܴ;zٙG�R�;��h�n
3�Lu�>ʕ3�F��ƽ?� ��O��
(�?  �~�,^������@Xo�	��\Oqa�'	1-#Ly�V�;��﹌0!�����m����ug),dMۍ�l��5�~������;�m.sBlѲ+����;�@'�+P PO7~�R8���b���5�'�Q&��6-��ЭsaɵN�Tc,Jق/�Oь}�z{���h�uWF9����:�ċ���Qm[v�b'��dğ���];�S��:1�`eCKm�^�@�@NZ1Z:�C@�S�Μ����/�]�ˢ�k'�Z r��;��yTc�{��8�����V�ꌜ�	�G�w�'��Z���6,�:q�*�+ڴV�s�F�Qwwkj\����	�������U�p���9��L���=T6:�:Ȟ�DD\_��NN~aK��c�1�~�~i���z�H~�ͧ�o���Z{)��G#�I��S�����|��v�c�f��j��/�#��*ƒY��=.ńE��m��"�:\.�L!�%֒g�����#�T��ODŊ(�?h<�*�0^����IF��@o��Q�SxW4�;]Rp1#KL1��2����+=����O�m����5{)xM~Q�هr��a�6\E��.ջ��6�m.tlѲ+����b�a�:C SC�{������m�|�#�z�HXʬ��̩"�rZ/J��6���}�z���`�e"�����i�Ɛ��+[v�4+��y���UL��'���,.�_PKa�B�m$�CwoV*�RW/�'������$�L�$�%���[�/a}r��ez�b�S�K$����V���ԣ_�G��;���U��n0�nq�>�c���%�J�Gwg?c����[�������TRͺ���)N��q����")%�%���G[ŧQ7(H�M�|�'�}�)#���3�Ka�����θVkq�{�3�p��iᛮ�V��9z�o��@�P��F���?��-+��D	m��`b�x��g�Q�y6� T�O��g�����#�T�_�OD��(�<h<�*�0^����IF��@9��Q�ESxW7�;]y1#KD4�:�+����9����u�}�����IP_{Y)��G��K�Y4!��P�Ǵ�\�q
}`�+�����{�Y;�bSPO7�^$���~���u��r�K�x1��ҽsaɼF�H+1ƒ{�ŌZa�z7��� �7WFh����i��߅��P5~�bj��,�����Q'�N��*e�~y@q�^�Y�SsZ:�JL��Ά�ԤC�.��G���!;�0r��:��Xn;�n��RT�����k��Ġ'a�>/��J��"�qy�'�rRT�?m ��(��	�;�S�Qw{w"����
��������M���5�B�m���>Kas�9�ɚ[GC��l[nD�� nA�1�~�~i���z�H~�聙�دUt&�l�,E�9��(v�������u�`��C��u�5�<��0ōMF��"y��.4��m���9fF�\y�f��]�wxϠ��P�b�&�:c��]�PW�p�P:�����<*PD'���EJd)�'Lyy?6��s���.!���S�%����4o50PyE���r��)�6@Y��/��Ыk�qfj&��c�����e��:C \c�x���X���m�aE�(�a�[Yԭ����-�=���>��Z0�z7S��� �eWF>����i��߅��P5'�bj�[�,�����Q'�M͟*e�(y@t�c�@�VNM{ �&i�L�υ���r�B�{�K۵�U�#z_��T��0I��d�*I�����(�d��:^�+k�N�+�����&x�rmL�6���A	�;�^�Q!{w"����	������٦M��5��m����>Kap�9���GC��MQ)(�8�l�c��[O�8��	�mq���|���3[��\(�B��^�����r���E����;��*�r��/��RE��=n����d��v�#{�?�v��&�I����w��_�O��B-�rU�r�aB�Ɋ͞��_[R�0�u�._hq�Xwa} B@v(^�y�B����	Cg��ڿ:��萛�A\RU0!���o��a�3\Eº#ջ��>�m."lѲ,����;�\o�*OO{b�
$͎�*���}�K?�Q&�
�*e��ν'h��N�kxJ��e���}�,*O���x�ub�����S�ć���"	>� W�y�Y���co+��<E�t��WX�4	}aH_��U�@JW=Fr�B P�S�Қ�W�f�I�[���k'�U,n��2��-T/�~�M�R8����������H�
J�J���Y� �|�q
�ba\�I���)w`��(�vgut����|���Ǚx��0'��Qʅ�M�� ����sN5%�%ם�[\��VQ}~��J*�2�)�ej�J��y�h���V�۰=0�s�3Z�&��?u�������Uv�7��@��<���3��?��>d��v
2���Q��`�Q�{�9���@�j~����B�Q�3�u5؃By�#hh�g�d^�谼�	��CDs�A��-N}�oSCyeu
P1��'�Ϲ��xi����J� ج���-*5\%t��� ���},x��6٫��L�M$��\���ÇN�9Z�E3g2 	�&Q������6��Tr�K�/1��ҽsaɵ�H+cJƒ{�ŌZ7�z7��� �eWF7��⹛a���ڌ�P5Gv�bj��,���Q'�͟*e�}(}aa�N�Lr�2"M{ �R@�,������>�Z�6�8���
D�'v}r��.Tc�{��=�����J�Z�����[V�~�'�����&x�{mL�6���	�;�V�Qr*JP����ŗ����Ko�ʅ�9���ֈ�M(\A�\���(?<>��4 -�l�|u�X�����_�qпσ���Ow9�s�/Z�&��7!懲����"�����v��*�?��&ҎR��!fGǑ��q�Q�6�rB.�Pi� ��/��Î�j�!�O�_ޟf�#L�X�<N���x��10 �w�l�p0p �V{q}Dn �>�p�Ͼ��xiR����O�q����=/*0,���$��x�6��&����>�mb>G8��7微��3�Y.�i \_o3�.���X���D�(�Nq�>����R[��^�>��\�r��g?G�$*m���4�;hjk�򷳟-�����.1�W�@�@���V
D��H;�S��6-�(e\[9�B��(N&�
JP�S�Ξ���K�2�	�Sץ�"/�ZKfg��s��yTc�v��0�����J�Z����E�
k�g�+��F�/'��  �ba�D���$ad���VG"u(5H����mͥ��_����!���9M�\�t����'W)m�%ˁ�[S_@޸QMa6]��TW-�.�7�}v�T��e�We�������Jk%�;�:���'y���<��m�`��f��P��F���?��-+��D	l��{{�O��H�e�}�:���b�b<����?�M�C�]��
`�?t �bR�,�������o�	��O0a�'Me-kVD-�O�2�����d!K�����e�����!ga00M�Ǆi��9�&P������[W`��`���Ɂ\�)V�E%U����_���m�|�?��b|����oa���@dْ/�OЄaX�+c���t�->�����!�ԋ�͠PaG>�ko�7�<ԇ�vy��R8�Ŝ52�7Xzb:x�d�@�Cd U?C�!i4�1�������N�?�-���E�FoT��+��4Q7�6��Np�����V�����D�N^�w�m��Z��E�&,�:xD�6�6ҨF\C�o��wRnv����	Ю���F��HR��녟9��m̟˨i'�5Ǒ�$)79��3$
;�P�OF-�j�}�\L���-�R}пσ���Jw9�s�/Z�%��7!菲��P��"�)��Ҏv��*�?��yڎR��!fG��Y0�
�}�T�R�H3d�S`�iՍ�7J����/�� �;u��i]�PY��L;��ѱ�_T��	+�w�o�h+},�K8/=2=#K4��j����x=Ǩ��O�j����5{)xN,Q��� ��a�6\E��.ջ��6�m.tNlѲ+����;�\o�"O)b�
$͎�*���@�'�A#�o�����Uq��^�73��Q�v��cr�b���	�4CWz�����S��܌��UaG>�g>�E�,ؗ�X��N'���"1�`0Gq\��PV�RiF>�BT�O񆚹���'�|�K۵�:;�g9	q��$��zK4�C�"�"������`����;f�("��O���PH��Lz_�0$�s�6��1jz�
�2�mTkvZMz����	Щ���F͞HR��M���)R��v����+C5%�sם�[G\��QQ}~��J)�2�)�hj�J��y�h���V�۰=o�s�3Z�&��b$ܽ�����oW�F��a��z�a�QƹV��'(��Iy��M[�O�8�]�o�j9�*�4��3�3;�Ӎ�w��_�	��B(�#hh�j�d]�谼�	��s�A�O�.}�oyeuP1��'����E����S�P߯��"x(13R������&PU��Y�ϩ�Q�M3��_����f�~�v<p<=� q���ó�kU�M�9�a=����sa��K�H+dJƒ{�SŌZf�z7��� �eWF>����i��߅��P5~�bj��,�����Q'�Nş*e�~y@q�^�K�5!B1V*�^pm�j������[�e�3�"���V�5wP��b��0~�:�J�!X������_����aX��k�sćF�Z�Y�U&8�r9O�*�+ӼZ]�"�V�YOk/!cȨ�������ԛTN��H�����<��̫T6:�$՞�DD\}��=\DD��W}�W���sؿ0�3#��ڇl���%2g�"�f5�s��b+�����j��}���)��v���h��g��NY��8z٘�R�9��9�n
3�Du�>��3�F��ƽ?� ��G��_ �? v�~�,^�������HXo�_��\Oxa�qPX9C=3\|�k��̃��h1��ܰ;����RtMn)�Ā`��J�T/ԯw�k�-3`4��~���#�L7�*AIS6�G8���b���}�7�Rn��#%�ؚ�>}���Tc,ӎ3�ݐ)F�fJ���h�`BJ3����u�魵��ObFu�}=�0����'
T��X�o��EN�L
15A�:�O�
t1R$U�X�������	�j��4���Q�&gE�W��\;�6��Np�����V���ԿD�@V�w�2��Z�D�E�",�:qL�6�jҨF\@�o��wRgwv�郟	Э���F��HR��M���S�)�a����>'�n���KKSP��#%;�p�hm�j�#�##�F���5����X���6y�1�w�s��[T۾��T��"�z����v��*�?��,ڎR��!fG���L�q�Q�7�rB{�Pi� ʎ/��Î�6�!�O�_ޟf�w?�}�3A��θJ|��PT�B�k�m6|�HtC@,oGA`��X����<aF��Ǘ�8�ǻ��hIGfC;��Q��5�zU��6��Ыo�qfju��c�����3�@'�-SLvv�Bl���b��a�V7�Mn�
�6-�ڡo)׼R� cdV��:���Gi�2)T���h�yKOXk�����y���บi6+K�r2��S���du&��9H�M��hI�}4Zw�<�/[�H^N"o�I������v��
����wo�_0&��'��e\7�6��Np�������Ă�D�_�w�;��Z�D�E�&,�:tE��sʤVY	�N�o�hbhu�ݱ�B�ŗ����+ ��4���Mi�B�$�Ƴ�s+J�V�����5<>�T�Y@)�N���f��(�;\��Ֆ���Vkq�v�3�&��i������Kc�c��
��j��.�#��/ƒP��=.Z��E��m��"�:s�L!�~֒g�X����#�T�
�r6��8�#�[M�d����~b��9+�x�v�B-]�rTQNPa#N�C�~����8tA����q���Ԁvgaa0M���8��)�*@��:����"�%o*[p��z����/�q�6SXLKc~�$ђ�b���5�z�k~��g1���p~��H�W|Yp��_�=ɜq[�{����
>YYg�獇�<�����&;� o�a��܁R��Mo�уb-�4e[%��P�@Z=Fr�B P�S�Қ�W�f�I�[��k'�
,n�;��-]#�~�N�R8������ꌜ�^�}F�g�v��g�+V�x�6 �9>�O���7`z�-��Zv_2+'�ݱ��������Wͺ���)N��q��" -�%���G[��Q7wH�M�|�x�}�)v���`�\�玖���wQ �l�2]�9��"������|���?����s�g�q����h	o��{{�f��T�R�H3k�\i�'ώ/�_�Î�?��_�Sۄ
4�wa(�bN�,Bۼ��X��\D'���AGd)�.Ly-?8��s����.w���S�%�òЪ-w9 U0!�����b�zU��6�Ģ�G�W6��i����m�8x�kESC�{4���3���5�+�n��b-����la���@d��/�O��aX�sct�->�����!��ڱ��@mW;��4�3ۖ�y��K�!ݓ:=�W4"{�+�?k�_Bt)D�s/�������	�k�.�,���[�/a}r��uTc�{��=�����J�Z�����[V�~�'����E�&x�rmL�?���	�;�_�Qw{w"����\���X��RѦM���d;��a����qn�푊KK?��((!�q�i\+�p��42�}˿%�����F���&V�c�3�#��8䛮���Kj�c��_��j��"�#��~ƒ¨=.Y��E��m�O�"�:
{�L!������ڙ�%�e�P��7�R�;�<N�����ui��#1�|�N�Jm4�t]zy(oxl(n�F�n����X����S�P߯���u6,,d���l��5�~@��nλ��v�ez"&��+�����{�\;�bZPO7+�^$���~���}��Yr�K�"1����saɵN�H+2��	�ɜ_}|�@FIʍ�x�u[LqL�˛��ߠ����	#�7|�U�Q���r~��r����ce
t��U�@R=Fr�J P�P�Қ�W�f��[���k'�
,n�?��-+�~��R8��˸�l�朙�4.�~k�N���`��Z�&/�mnS��(��|�c�)�{`u{����]���Ǚx��0'��Qʅ�M�� ����sN5%�%ם�[\��VQ}~��J*�2�)�ej�J��y�h���V�۰=0�s�3Z�&��?u�������Uv�7��@��<���3��?��>d��v
2���Q��`�Q�{�/)�#��3�3;�Ӎ�w��_�	��B(�#hh�j�d]�谼�	��s�A�O�.}�oyeuP1��'����E����S�P߯��"x+13R�����b�zU��6�Ģ�G�W#��w�����f�3G�T<f53�8���[��5�+�?��b-����o5���HdӚ/�O��aX�~ct�-B>�����!�݋�͠PaG>�j>�E�$ؗ�KLX��M'���`�uPKaY�2�j'�d&h1V*�	c"�*������$�w�����4j�6>
W��;��`H+�/��8��½�^�����X[�OJ�?�j���D��:0�ruP�~�cƴ�s�S�B$M
wgzYԌ��4�����3��nB��w���}�f��͒�\?1`�{���K��H\D�?�Y["�&�%�{6�V߿4�j[���G���Z+V�"�b5�s��:yጓ��@��Gb�;��	ڌWƫo�}��@��E��l5T��I ��W�\�`�q�\)�(���KX��Κ{�O��]��ez�i>K�fk�d�Ή��UA��M\*�W��P_b�s\Lw$yHZe�O�%�����	Er�����0�û��`3FdIh�Ôr�)�uߴp�荷Q�<"s("��}�淚�
�	z�j+�`���'���=�Jf� r�Z�r����;��
�'1!��*�:�RD�>.����4Val����S�魵��,'�>w� �q���L^��q�p��1(�_}aa�N�@�J1U�X�Q����P�>�.�
���tJ�g9	S��+��3i�'��^a⧗��M�����C
�}F�g�p��c���Luk�7,\�;�s��JQ��O�`M&^63ʍ��3�轩�3��DB��p���FR�P�����2F9k�}���K��hN~>�[�JG-�j�4�$!���
�%��ǝ�ʽ_k;�c�f�+݇'�����W;�.��G��s���FѲ@��."��x3J��sO�^�u�|�L�b�	<�#��k�Aϗ��n�\��]��_v�4U�_<�u ��߰�~b��9+�z�`�aW=F?�/jQ&wZ^�B��ں��!G����U��ī��R5lfN0�ϕ%��m�iӲI����Q�88;zJ���̓��t�{�2B&�`�ˢ7���m�$�lT�:�|=���� ��5�)&1��Q�V��b�j���=�0?VJo�����;������+[f�=p�T�C���Ku��T7���hh�1u}aH-�c�i'�B1V�{%i����猭@��x�f�"{�>
W����\D;�n��t����f�Ѕ�@�Lk�N�v��c�/'��Ycr�bpG��^��{.(�5��fnw����i��Ϗr��6J����m�J�$�̫iv�|�Œ4SZ�� %/_�?�Y["�&�%�"#�\˦u�Ga�ʤ�F���IiV�c�d���r0�æ����W�.��3��<��,�N����мR"��J� �(�G�6�6"�1�%��j������%�0�Hm�a,(�&�5!����\D��:�d�j�` n�,8/=$x~@u�T�o�Ӿ��sF����S�4�à��n:}_L}	���a��0�=}��s�萪&�02M!��D�����s�v�{OS*!�Zw�Ɛ6���[a�Hw�>����1q��1�<%��M�s��c
I�.*J���v�53�����y�ˈ���	# �3`�Y�~���ZА@2���ss�}-]\|�
�r�v&�H�N�ב���q��x�a���#H�Ga��m��c9&�)��F3������F�ȅ�
V�=��{��T���9c�&�r���B~���QJ ZMST����X����Z��=���?�.�2���Fzx�J���
4��:ReQ�8�!8l�>�=�?f���%�0����n�˸S{"�c�\�b��4꺔��?��z� ߽��a��&�v��4ʐYIͯ&2���Q�W�	�.�!R*�7�+ډ"�嚘�7�X��]��aq�=+�X�<N�Ɋ� ��z}/�]�]�c�*[W!|pXt��ؽ֘�A9���� ��퐌�h"|_n���2��[�T(*�s�z�q&M!��7����n�W�gn*
&*�=V�����	�~@�I+�]�"B�ȋ�;4ɬs�^^s��m���{P�..F���R� YJ6���(�ʜ��PvzL�'{�I�.�Ӂ]M��w�B��"B�86@��?L�^M9'BW�&i�
����ؽ�p�|�b���3/�ir��j��;v�~�&�w����C����W�(�m�D���a@�.�Cre�o?�B���7(�J��LL34*>{����fʽ����������}i�E� ���tR0.����	��*�T�NG*�2��02�͢/�7Ȥ��l���k�%�*C���
S������q���+����[�z�z��U4��J��-0�#�0�v�o�#e)�?���o�P�����O��]��ez�i>K�bk�l	�����<*��_2�f�H�E *Z7�Lx	9z}q�=�{�Ӿ��ur^������ө�� ]"�Џa�z�b/ۯw�镵h�n9B{���˔��#� r�:^ T_?�`������d�'��R�{F��ـUX��^�Xs2��j�ҜKmL�$o���x�~P3����T�퇜��%%�<g��!ԇ�XCʕ"c�[��{i�C<[Y�d�@�E&h1V*�R!j�C�������E�Z�6ۨ�9t�Pk0��0��Xn��i�<]������G�떲F�Q3��Y��;�GX�1�Yw,�%�g� ��QH�x�l�2#J ZMu�����粦���KMΥR���6��n���=T6:�:Ȟ�DD\_��NN~aK��A�h�8�q	�v��
�)����l���9F�k�j�.��'0����8>P�:����.��x�z�p�� ��0g��.4�C�)�v�.� ^"�#;� ��)�c
����f�X�Y���^q�P:e�4i�y ����<*+�Z�\�Jpp%�7RJV-t}Zr�T�n��⥇E:�����9�Ȧ��x4ze[o@���4��m�tN���ɤ�G�\3h�������	�C�id.7�C$͋�_���G�Ga�.�z�EOΫ��	���:��w�Z��M3D�#P���t�7^WQ�����5������I@}O�_��<ۈ�BH��c�^��n�%dKKa�d�@�t�m�j�󠀃��m�|�b���N7�Ep,����4o�+�^�V*�����9�l�軒'|�"(��E���M_��C4)�OKu��.�Y
�l�I�hxhu�����粦���KMΥR���6��n���=T6:�:Ȟ�DD\_��hl[8�Q�cp�W���{��+����f���5!�*�wR�b؊i=�Ϸ���})������S��^�ZݣZ��J ��h.��.@� �2�M�A�s�.���V�awŽ��@�����q�k=h�},��U�Ɋ�;[��y6�M�v�*K8�HZP@=t~VA2��߾���<)l˸���4�ĺ��Va-]w�Ԉz��/�rPؿ�Ԃ�h�:w`��+�˔��	�#U�S*/:
�z�Ƿ{���D�$�v�q�STԺ����V�7=��v�C��G}O�.&[���>�yUZg�ݰ��?���Ҝ�]}^K�[i�7�<ԇ�
T��R7�[��ti�m4%A��H�zo+X)X�3h?�*������/�P��ټ�J�>
#����:��o�7W�����4�w��,w�( ��N��,�ln�:�ci�6y#�&�=��
���b�����:;��B�ŗ��|��-=��3Ҡ�wR�\�z��̫ovc�|���)/:9��$ '�`�i|�Z���q��8����G��>q�l���o�MR��h}KZu���HW�F������u�w�@����hk��)7�_�3��z�-X/��3��D�
l����z�B�^� H��X|�fr�4P�p���
-��}~V�{�`�{,a�~[AQ14(>v�>�m�����Na��ֻ)�u�ī��R8lt\uݍ�v��p�d��y�Q�(0t(,��2�����	��C \P`n� }�֩'���)�Lr�I|�q�STԺ����+�= %��@�q��ao�=n���c�_rJ3�����/�Ü��$>�F�u�O���tu0��"Q�g��HB�M3?j��Z�g_Bxn�
O�� 6IVW�W��x����J�%qD��X��Lr�*�S�EԶ�ϋ	�F��!j�%#��C��>�pt�1�nIF�(�H�&��z�c��ZQnx��Ye
t�HFա�o����!i��#����}ma�x���[P~z��%v;��NV%�A�$�7���X�n$��ڇ���9m�*�a�p��b7񊾥�8��>P� ��%��	��^�z�z��Yd��_Vǀk�c��l�K�c�=�/��j�JX��ڪm�E�0�
ZşUz�f1O�'P�C�����	��yR�0�	�Pg_#�yACA(ogWc�P�)�����Cv��̺6�	�����YFF}R>��H��w�/Kx��k���Wy=Xo��4�鱯�,�C8�5LSP`a�];ҍ�}���b�(�Nq��52��͢p~��Q�W|{Uō
�=�G)T�8!���� 7|a_�秤�8ꅮ���?/�'w�X�}�ƚ  y��~���^T�G5?a�)�.k�k{'T2U�5b?�*������.�]�>�C���3/�i0��s��z��7��ѳ$X$k�Y˶�*g�>9��O��-�}k�9�hD(�,%�/�^��{.S�*�F�}mce����~��֕n�� =��3���W�M�5�͵�{}p�<Ȟ����L�؛��X޲&IA��8�%~�F��7�8����G���Gi9�x�	c���?�����\��Q/�)��X�Wƫo�}��)��B��d?`��B[�F�3��>�O0B�8�fЂo�SA����l�M�O�_L��Sl�R�1[�!L�����i��@G�U��FOx�G{FKHY��$�ﷇE:������֢��Z	g^9��G��]�V7;��x���5�W_{h��_���ҍ@�%A�2B&�`�ˢ7��)�GX�8��xy⁽�*7���ziGڋ�>��(X �j(W����!	e[}���:󔋚��$�'t�f�Aؗ�]_��Q'��=�8uPfK-�c�#�5!~n�=n$�*������:�J�(�=���P�5cG��B��6D��h�1F�����^�V�Պ�[�XI1���u��I��
�OKu�D���%ak��!�`of����A�󐃻3����2���Pi�a��Ѐ�F)@J�Z���,.,6��"5 L�\�X
�>�?�4>��}�2��Bq�]K�V�N�
J��n7����Z��Z3� ����?�*�(������yvP��MT�H�;�r�:�y7A�	0�2˔@�^򗟬P�Y�	�Y͓8�4U�1^�nN����A��y7�K��T
&F%�?,9CcPq��:兄͌}I�ŵ�������@)0.&���L��D�4@N��4ٰ���(8{}Ü9�˔���eX�^*i#,
 �-D���;��5�
�h7��XY¡����(�P<6��\�B��K<E�%-V���:�3Ql�뻞�,������IpJv�_�F����M��0G�_��n�t!X"I�B�L�QB=sl�)=�S�����W�7�|�b���:�g9v1��+��-R���;F�����5�c�軏!n�(6�	�M��>�pr� �nBO�j4�r���JA�;�I�菋���C*l�qŗ��4��&&��"���]i�c��ۍ�W)1|�q�ɃPfy��8*D�w�e{�[���{��
�3����z���<G��MB���?�����P��u�e4��wX�v�[���h`��c"��R�N�4�F�z�+W.�Ni�}���W_�˩�/�N�� ^��\C�;+�X?�u �����O�� �L�O�H&l�'(>@~{R!��h��䘶Ar[���_�(�ϻ�^
!l�ȕ)��|�i��6��~�0&/Up��)�����3�11�nXf>Ot�?D͒�b���|�7�Zn��������!�=%��M�^��G)	�GE;���2~a\�����������?"2�;��s���Du��L�v��xy�q
'B���K|�&i�C�����+�j�#����og�C,1��b��7}�b��f�����]�(����.�zl�M�d��J�`i�<�~ST�;�H���9tz�
�#�on	up����l��ƈ��
��	���}S��n&S	 �ٯ�[�0a�fy��hl[3i�8�Z]%�>���`���"����l���*]��\,�X��H�������3�+��E���j�k�4_�uȢ}�-2��)7� �2�M�A�s�/���L�}~ݠ��J�|�!�9u��J�I^�y�4�����E ��'��i�ހ��?�Ο���9�>�I�����_w��Լ ��򊏈d9}eP5F�����v�bP:��_�Բ�O�S$��V���ŗE�"T�U-	!:�-`�ˡ6���b���6���+��1,#2R[ʈt�::��w�X��K8�?;F���e�eK3�����a������	$($�4t��AϪ�$_��L!�_��xy�q
'B���Y|�M�I���ܦv���ۿ�7b�G;S����6	k�l��LS��ǝ�R���ߝ*�M%��;��1��E�p92�y=�&�^��QK�7�D�J ^JP-����p���ǂa��*J����";�-�'�݃�[2FG�V���.C��>>;�\�Ij�|�8�$�Tؐl�K>����l���e�-�>W�?��"ܽ�����G�����)��n�c�o�� ��R4J���Z�3�]�>�y7A�T�;���H^��֊M�e�0�:h��tR�L_�g�] ��ԝ�ag��6=�|�v�q1{�?@LZ=%LG\h�S�s��𺜍��rY,�u�P߯��� ]ooAx]��]��K�G4*�\�ȴ�@�S9��B���΋W�"?�sTD ;n�p���r��ڈQ��?�S�H��⟀U\���s8��J�d��}N�#!W���7�4CaZ��8�����'( �F�o�Z���ye:��y���:t�|0Bn�AP��I��Ծ��A���*�w6��אq�Q�5�"���	S�:tN��G��Lr�*�S�EԶ�ϋ	�F��!j�%#��N���Gr��Awn��v�!��V@��(�kf	".4P��ϓPƬ�����K� B�!*#��+�y�'\�vn����K/ҫ5'�Z�XF%�{�c�a}�8��<�i����Q���%9|�%�5^�-��,��꤁j��\8�&����<���3��/ѯh`��l$��T^�Z�P� �f�0a�,�/���7A坟�2�H��]��0�l5`�p�,\�����L��e�t��Owa�Jr-1q�x�O�����Ul�����9����^pf S?��K��Z�O&0��t�≌a�)"{%��t����a�
q�#>+'�-v�ĩ���d�7�lT�H�����:%��.�!��o���O!�(+[���-�3W~�컵�i�ߪ���mG
K�_�R�x���ic=��8C�m��vB�y;4c�1�)b�}y=W#D�&c.���ޱ��[�p��BԺ*��n��ϺL"�	�_�G�i�7A�����9�e�㢒$w�%N�)�nݛQ�(P��Bd�8�O���2{c�&�9�^G,ug����L��鱙J��
��T��#��p��t@{o�vn����K/ҫ5'�Z�XF%�{�c�a}�8��<�i����Q���%9|�%�5^�-��,��꤁j��\8�&����<���3��/ѯh`��l$��T^�Z�P� �f�0a�,�/���7<����a�X�M�]��0�l5`�p�,\�����L��e�t��Owa�J|&#G�>�߾���Xl��ݶ ������h/0;%t���[��P�Y"0��_��ȼl�4?M5��r�����a�~�xF$v�v�˦��+�Bb�Hc�����Vq���*��q�R��N%�)"B���<�<$UKh���,��Ӑ��[@}O�_�0����[Eڍ2U�b��IX�G
"P� �?~�vt$X:^� s&�-���Ǆ��j�.�K���"o�E��V9�x���H�z���a����l�/����L�F��B��8�ci�'�Lut�,5#�S���8P�-�^�K	.D3""��r�	PF"�бG6�W7�M�.�K�ˬvc�|���)/:9��$5-�j�bb�A�$�?2�Fǔ�)����v���/J��+�x��X-�����M���H�UTӹ�!���QƹV��''��]q��mq�o�u�G�k�kF�T�0��{�qsԺ��]�m�0�6n��tH�[A�3X�u��Ƒ�rt��X6�M�v�&K4�>
�������L��¶��pNG�����ӻ��`2$2,F���a��m�YUչr�⿱{�;$C��ܳ��2�3c�uD,�r�Ϊ<���}�#�6�A�0=����d\���!tP��v�^��/[-�C?G���e�>V,�����)��ׁ��Q}E�b!��A���L��GT�{̓*/�@eC3q�A�P�o�{c"�*������$�Y�Y����n,�`:i8��I��;I��b�; �����9�W�ȍ���.�i���MO��rtu�$�h�n״_)/�8�l�	gx ?P����Mښ���R؃��Q���}_�{�4����OW)9�`���G[ZK��h\D�?�&D#�z���m��#����U���5}�"�\:�s��r+�����j�����9����2�z�'��B ��h.��`m�c�J���O0B�>���Z�zn����I�`�*�WU��_`�4U�_<�s��φ�xi��55�}�o�`.I4�r[Q](qLcd�D�y����E|���e����i2a)39����=��x��c���Q�}9p)��t����f�]'�i)	!:�-`�ͭ6��/�Ha�Wj������ng��^�7=��v�Q��G? �j	���;�4	.�῱�T������@gW6�<{�����CC ��p�Cߓ!�%1�^�P�`QIF8�T@��Κ���+�.�C�3˦�w%�E0e��0��\m�G�i�7A�����9�e����F�^]�M�bÚ%�fd�*�hUE�T�d�:��QQ�*��PK#C>8Q����[̡�֤K��;��T���0;�-�L���2T6r�a���"��%f�X� #�z�8�33�T���W}����G�ǩAV�>�	���q6�Ӿ��q��l	���<���|�a�k��6��h%L��Ab�m��`�Z�e�2�	��j�PE����f�H��P��|*V����XM�g��č�ns��4+�w�o�h+}y�yQ@L`&9�A�b�����Xl��Ҷ ������k2keFh�;D��j�s�C�ȴ�@�(?|x��;����*�CS�\Q��͡C�	���;����h�7�l�oi����b`��s�q&=��\���F8I�(:T�۟l�nv=7{����������	3�4M�X�<ɇ�'~��v�דj~�hn}aHG��@�H^x1�x%��������>�|�b���7*�Z0l��;��eVF�"��8�񱈭V����[{�E;�u�0��Q�($�x�$YB�#�C���1j�-��J ^.<����p���ʃw��L�����lB�M�$����{Fw�s���	��7�[�|��X�\L���z� 4����@���(|�7�s	�fԌe=�����S��v�:����s���n��b�oc��{9Vňk�c��l�Q�$N.�(+�'��@�}cϚ��n�I��L��[l�}=O�x�Z'��Ŗ�wu��$;�L�G� <8�sP]f2��ȧWک+��(D���}.[�u�P߯���DFSmR9��Z��P�R5'��x���5�W+}6�����āJ�>R�^<a*<� q����ƨ�@s�
;�G�wm�*#��8�6 5��v�Y��}% �$+Www�ϑ�㳢����f"_rw&�@}=�X�T�h���E��H�^��sp�%d@PL.��Z�g_B~n�s������v���ʣ�37�Wi7��~��
~�n��E�����̇��}o�/�y��J�WC�� 	�7/�;�7��5�N�F�OJ*Zjej����	�������_��M���I��Ȣ��"KD?�;ׂ�Y^^B��ZQsui�<�nv�A���u��<�4�9Ğ�RK�
�\8�U��Q�����\��F.�&��@��>��E�f�3�� ��o#Y��HX��`� ��97A�\y�i��z�}|����]�_��WL��J4�`:t�;i�i�������+�	��.v5"[�?(>?rzW\N�s�B����	wG��ǀ�)�����R>wtIw�Մy��k�r߄F�ſ�G�	@(��X��Ш�j�/�nCI*6�];GQ{�]h�m�mUC��;�$�Ն♀UX��:�1?��\�p��dl����=�|@:4s�����=������?2�S�I�n���L^F��>E�B��{v�G!1A��\�gm7_5^�<��������{�X�DAf(��"XS�%�tO���7C�P�t�����9��ހ�|��.�n��Z�/'� �Cb8�6a�b�:��BC�u�;�2#N_>).ΏϓVʬ���\��=�����)�.�"��8[iv�eܼ�bԫKA!;
�A�!8E�k�8�ld�Wوw�\\���P���Xk8�q�[Z�N��Z�å����p���^��j��*�N��1ڑS[��?a��� ��m�G��#	���q�[I�����J� �;u��h]�JF�zY�x����EI��\=�P�v�Q.>�s\Lk+huU#m�Y�6����Aj>����P�}�û��]$mrzy�͕<��t�*Vؿ�Ԃ�h�oO[pͺn����*�a�gn*rEC�{�������}B�>�G�hI����n��,�7& 
��\�R��G#A�?=W����2{jK�ݔ��ʫ���9O�r{�Y�d݈����m�&�x���,�݉)��j'�zo+X)Y�<h?�$������>�6�����J�g9v1��+��-R��j�!I������C����+q�8�7����RH��Xde�2�R���JQ��F�QM"R~hu�dsԙ��ʔ�Z��#<����<�Q�/䪞�2{q�|���öPQjCn�@�BFt�j�>�5/�mܾ3�<gԚ��9Ă�/8��#�r��X;�����P��z�t��e��;��0�c�oѯh`��l$��TH�C�f�#�~�,S?�^=�!��?�3;����b��_�O��
,�?kM�>B�z�����a��B/���yUxa�&{* L#�:������Bv��̢8�u�ȫ��$l
!u��P��F�C30�S�䂷g�9g4��~����j�b�|OO&'�a�Đ>���p�.�kW�>�=��͢84��3�'&(��v�C��A<P�l P���� Qe7߮��,��Ӑ��[@}O�_�0����DNT��9^�t��NB�~!V��Rq�{y=^~�X?��������{�!�%���C�8lF��_��}r�:�t�^a�����I��5B��$%��+�|�l� ��'O�5�U���&ck��*�z{oC9#?\�붹D����Z��&&��"���]i�B�$�Ƴ�s+J�V�����5<>�T�Y@)�X��	�ˢ;�_����W���Ut�N�9Ç�6��U�'D[u� �9!�E����b��:�q�v����`3��0�#�4�]�Q�bU)�0�$��y������E��IcדUz�f1O�'P�C�������y7�K��T%_j�<F\(o3\q�Y��՜�=Z˥������Ъ-^icEl@���$��A�2(E��0��۵l�mlQ:��+�ư�[�_J�*MON}{�@#Ι�p���D�lC�(�v�^Bۯ��6?���C^^s��+�u��kr�	
{�Ł7�1^aa�����<������?/ �~}�Y�u���KL+��>�͚�cXzba�A�K�{f2bx�_����ܱ]�|��4���!L�70r��e��yT2�C�"�s������K����,j�(5��N���Gr� �AIP�:�J���0qk��(�J@)^o.����A��>d���]K�WT�;�_�K�݃�[2FF�[���,%,6��5$f�Q�Tw��\�')�V���.����w���2g��v�I��I��н�\���o����s㍭~�-K�"��es��2��)7�_�3��z�-X/��3��v�[�����Y��GcדUz�f1O�'P�GXߙ�ɻ8U��Ei�M�F�@!p3�qSzX,s3\1�:��߾�b1C����r�T�ή��'wiuFu	�����w�rMWׅ%����5�Wv#��&�����X�\&�*S}Z;u�$͘�͵�}�u_�P��5,��үd\��1�<%��M�h��lyI�.*J���R�<qL�˛��ݡ���//�}�I�u�BH��"u�M��u�q</	4B�1�@�R8{ �	!j�C�х����S�����2{�BCc3�-��1r�,�M�%1�����K����S.�~�M���`�MD�U�oRI�9�Y�!��MJ��	�N\"u {����|���܈}��*J����m�J�$���ݫ�%�YQ�a~z��#%;�p�hm�P���`��;�)����N���2m��G#�I��F�����A��]� ������Y�\��j��Q��-?Q��\�v�J�5�?�7A�qS�3��k�aE����f�s��U��)�4U�'_�hV�����UI��9�b��rB&$�yAi3\e�^�i�ѹ��-����r�P߯���w3 XȚ���q�tP_��c����T&g)��9�侻�	�t�j^LN|�*$���>龼�mJ�Qn��B-��ϼmd��\�uY]5��J�h��l����=�|@:4�����׭����%!�=p�T�C���YCہu�B��xh�~y	%M�1�H�gG_7�Bm�j�󠀌�[�1��(���3e�@u2�{��%H=�,�B�!j�����;��̑�W�GO�J�ؿ`�/'��Bd�8�O� ��)aK�*��qL3u2.T����Vڹ��n��;$��)���_c�g�޺��|!J�5���Z_�<�����$�uȁ�=�.��[=�8���#����}���=V�
�W/�>��c<�����H��E5�+��.����_�Wݵq����h"`��PM�^�	�[�{�|�?���v�J	χ֬a�I�F�P���}���5��Z��Ɋ��;)�� g�M�F�@!p>�cALY, "G�>�b�����h~Q�����;�����6Z	]u̐�K�v�b�c��k�}k2G{������q�F7�yPGrEG�u���r��$�
�h.�]�n ����R[���nvJڒ0�l���'?	�w�gC�����ܠ����%3�\�T�x�߈'~��?�i��SB�]5"a��G�gDec� ��������q�����!q�Vnw��;��_m`�D��^'񬗱�+�Q�ζ�V�N�$�{���FD�*�Xpf�{P�*�&���x�l�B$MWZMW.��������a��0=�����Ft�J�)�ߠ�y$Mz�d���;,=/��-5;�`�of�6�8�%~�F��;�)���{X;���	�$���\]�T�̓��z��z���(����\�_ֵM����$m2��)7�E�9�f�J�e�=���v�[nҖ��g�a�� h��Km�PE�b�RF��nT�:����甫%��֏�ʯ�ؿw:�pؽ���&e�쳝�4�����y>te-M�����w�>/��t�≌l�;0IAt������w�ZH�:B&�r���7��m�
�hW�9�yu�§!���uY]��q���W?I�qB8���6�!F,�����)��ׁ��Q}E�b$��h���i9�M'�p��,@�({DPq�c�ia�|b+N%O�6s0�-�����R��x�a���oX�.zI��H��!3�,�B�!g������M�؍�7A� �(�o��5�WK�*�Hx)�qU��(��l��Kj
z$3(v����M��������"���m�y�q���{5%�.껣fy��lk'!��nv�A���Wۯ
� ����g���<G��MB���?�����P��u�fLa��WƫU�W˹@��,-��J`��hi�x�4�M�v�y7A�qS�)��?�|uٺ��J�h�<�Y��|M�L_�zC�rӛ���_D��Y[p4��.E%X�~[Qi3\e�^�d�ë��-,����v�4�����-8{dAu"�ԇr߆-�=}������Q�}9p)��y���ߤf�L*�*X-vv,&� 4ׂ�1���@�'d� ,��jh偖�R[���'iX��,�ɇ/[)�)"B���i�g o9ڲ��5���ޅ�Z:�b"�f�,���w[��K,�	ƾ�Z9"w� �?~�vo�[m�j������2�A�4�.���ox�Zi����87�,�B�!j�����K�ㆮE��n�6��g�+V��6 �m&�E���v@�6��OJ*Z{a5F����[ݮ���c��H��⤥0�.�<����vc�|���)/:9��$5�Z�IF>�m���{��8����|���5}��#�r��.v�5UX�5�9!�E��.����_�WݠX��$ ��H��@X��P�T��-S/�3���M�uϸ��`�I��c��t[�@8�;X�$1����TX��_��G�	ԥ.E%X�~[Qi3\e�^�d�ë��-,����v�4�����-8{dAu"�ԇr߆-�=}������Q�}9p)��y���ߤf�L*�*X-vv,&� 4ׂ�1���@�'d� ,��jh偖�R[���'iX��!��+!C�:r���j�K c(��釼cݩ����R/v�
"�e�)���pL��<5�&���2/m�=�.j�hl,9d�TI�n�����9�W�.�.���R�Qd7��d��!v�b�I�a�����J�@�Ս�
V�9�"�"��Z�/'��' �bnS�c���PW�&��K7I:7v����P����e��HR�����5��K��̫ov������#;�8�W?F�>�$�?2�m�������@���>q�N�#J�c��sa�þ��f��W�!μD��4��E�3��$�BI��d8K��P� �8�L�x�O%�Uq�/��'�J����/�!��0~��sG�BO� y�Y=��Խ�T��}~|�\�@�A_p�Xv`gB^r$!�^�n�����,xZ����P�4�à��B]IaC>��@��|�zx��y����A�[(��D���ÍP�3C�j 3+'�1y��=���>�cX�3�v�IBƩ��b\��"�rZ/J��A�~��ki�}����4y>�����T��܍��"	>�W�b�U���df ��"Q�e��US�((\7L.��La�|b+N$O�6s6�-������4�2��7���M�YO��B��0D��~�0\����� �n����.v�42�	�;؞6�($�A�bTD�.�G���#q`��"�|vk�����ŗ��7מH-��4���\b�l��ݓ�M?MV�S���.9,6��"5 H�H�p?F�e�}��{��#����|���>J��M)�Y��7$�듟�?��u�`��C��u�5�<��0ōoc��=zp��mt�y�	�l�I��,���X�awٿ��]�j�!�+s�
e�SU�)�S,��ɛ�ta��5:�w�v�t:k�B{f`S?-�:�ؽ��d^q��ڭ,�	�䎉�_FVgP)��K��W�@99�D�͵�M�>G=͖����L�(^�E1e=;�&@����ɮ�kX�4�g�T1����R[��t�gx5��J�h��ve�|����/hjG�Ǎ��������21�F�s�,���  }ŘQX�o��EO�Y/.c� �?}�yy6N"C�7s&�-���ǀ��B�{�f��q;�(wI��N��*\� �t�*G�����#�r��!o�24��E��%��Y� 	�um3�B���3ea��!�qly
{����f��ʒq�T��p���?���ֈ�M(\A�R���($>#��$>	*�s�`f	�A���|��(�;\��Տ���3V��G5�W��D�����j��}� �\ɛ���"��P��+ ��H`��nx�~��x�O�s�#	���V�jt¬��A�x�!�OA��7�ti!�t�U'��Ņ�ni��57�w�x�e1f�Hspz
I\}P1��߾���d^q��ڭ,�	�㡺�c2FCGs�ϕK��t�cήd�԰�@�^#��]���ԇM�\j�Fn*u~}�=V�����	�ki�0�l�ur牌� ���
2 ��\�b��vn�z2ʍ�Q�dOqL�˛��ʻ����?(�B�s�Z���hx+��3T�d��*`�DXzb:�B�"j�qt0T7N�4Y�<������.�]�>�G���cH1h��I��;H��y�k�����6�l����<f�( �	�_��F�_�x�$m1�n>�O���7`z�&��^G3ug����	�����7ך\N��9���Js�{��͈�B.MJ�`���
/��/>(�p�st�]��}v���_�`����w���(H��J8�_��R�����A��J���8��v��6���dؒN&��D`��pb�z�l�@�h�0_%�8���z�MD▩�H�s�&�+y��|M�L_�~�02�Ή�� ��4=�k�h�{;i`�~SCQ;x}G`�h�y�Ĭ��-Nu��Ц0�q��ね^9"p���$��[�O9*�B�ϰ�<�40t2��o���ߘq�d�h,8�;X���д�aI�Q#�o�������!�=%��E�s��D4R�$;[����0DK|�ݔ��ʫ鐅�LzL�) �r�X���~o ��c�Y��|�)
#({� �E�g_|o�X�������2�J�#�-���X�F}r��fHT�
�b�,M������P�΁��$%��e���}Y��Hda�73�@���9j	�>�:�5 <{`����|����[����O���Ki�J� ����wip�t���4#7��'(!�j�y|�Q�}�,j�?��.�K����l���>H�7�q>�s��5�����[��X?�;����;��h�LĥQ��-'��pzc��-F���`�Q�n�-�4��z�J Ϡ��P�Y��c��Wh�}9d� S�L)��ƍ�ee��6!�m�g�ds\�N_Wz5^�r�O�����:EA���� ������}"m_|y�ēu��k�Y6 ��B����s�[~;��T���ɗP�8H�|E+=+�&�� �ʏ9�[s�5;�C�h|���� ��!�1 ?��E�y��m�7cn���#�y4uzG�݀��������4?'�`M�~�C���]_ ��z�N��nh�}
6>g�!�L�woq�B /�'������$�p�����"{�:tN��G��;]� ��0$�ޙ�l�Y��Ȕ*g�>9��O���KC�*�Xsl�2�R�ƴy�I��h{����ڡ���Q��0�����^x�b��ד�T.WV�Z���G/}��Pbb+�q�em�_��#/�Fפ;�3>����l���9F�o�~F���|h����|��`�����?���Lͅk��6��'J��tz�u��}�\�o�(�j��3�3;����#�n�&� n��~G�a?y�7i�y�����DT��.�\�o�j<`�'H	hDHM7�x�O�����Nv������Ȭ��@6kkwH�Дq��^�Y6<��S�Ԧ�@�	\[p��G�̗��4�#U�S*.:
�7z�ˡ7�ǟ#�FX� ,�l�uo������*�6d���>��}o�{����R[j�ҕ��ר����&9�\��0���$QE��2U�b��HX�\
#$A�1�5`�ld,=Fw�.!j��Ά���2�A�4�4���(z�Ae-��L��-W��t�+F�����V������[)��B��/�fr��B{e�0$�@���9j	�>�:�5 g']gW.�붹4�ŗ��Wȋ0��"���p_�I�����aFE�z���Y��l[G�[�m8�w�a�\L�Aʪ!�q�������>e�:�3Q��h+񟥮���:!�E��ҟ>��{�8��+ڋoc��v[5��-K�K�6�L�o�j{�Ub�L�b�48����j�&�f�r6��3�Q�7Z�eS�����<*��yU�0�#�-']y�*]u,(ZW*�>�ܾ���kxU����_��鋘�NK_|S$���4���y|��p�Ă�g�;v'2��~����-�	y�\NVBD�{���X���D�Lc��W�hr荑�8�-�7��+�ұ(X)�C9f����9N?�����S��ȝj@}e�<v�[����IO��0H�d��E^�J
>"w� �@'�2"kc�Zc�
�������{�����"y�,Fn=����\m`�D�"�wG������r�Ӈ�P�;��e���A�\� 	�KH
�g�8��E\�r�V�$M#^NSY�ڶ�ͦ��3�������F{�`��ӓ�Z4WQ�5껣bbz��.5'�X�|@#�{�"�8�aۥ1�?�:���V�J�
%�r��D4�����P��h3�a����/��2�:���k`��l%T��H\��m� ��K3B�T�K��z�ZX������e�v޾0�/U�[_�,U�Ή��;]��}~R���Ub{�=c79V+��!���֕hR|����8�	�����n%veDy�ӎz��j�,Z_��S����H�^2j��1�˔>>>>>>> .r472
