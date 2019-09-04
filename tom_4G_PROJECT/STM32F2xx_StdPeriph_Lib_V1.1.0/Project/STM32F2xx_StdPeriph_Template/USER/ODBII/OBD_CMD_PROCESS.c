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
	OBDII_ENGINE_SPEED_PGN=0x00F004,//读取发动机转速
	
	OBDII_BATTRY_VOLTAGE_PGN=0x00FEF7,//读取发动机转速

	OBDII_ENGINE_START_TIME_PGN=0x00FDB8,//读取发动机转速
	
	OBDII_ENGINE_STOP_TIME_PGN=0xFFFF,//读取发动机转速

	OBDII_GAS_COMSUME_PGN=0x00FEF2,//读取燃油消耗量
	
	OBDII_INJECTON_QUANTITY_PGN=0xFFFF, // 喷油量
	
	OBDII_Engine_Coolant_Temperature_PGN=0x00FEEE,////读取发动机准确温度
	
	OBDII_Fuel_Tank_PGN=0x00FEEF,//读取油箱液位
	
	OBDII_Reactant_PGN=0xFFFF,//读取油箱液反应剂量

	OBDII_AIR_INPUT_Quantity_PGN=0x00F00A,//进气量
	
	OBDII_DPF1Differential_Pressure_PGN=0x00FDB2,//DPF1压差
	
	OBDII_DPF2Differential_Pressure_PGN=0x00FDAF,//DPF2压差
	
	OBDII_AfterTreat1_SCR_intake_temperature_PGN=0x00FD3E ,//后处理1 SCR进气温度
	
	OBDII_AfterTreat2_SCR_intake_temperature_PGN=0x00FD38,//后处理2 SCR进气温度
	
	OBDII_AfterTreat1_SCR_Output_Temperature_PGN=0x00FD3E,//后处理1 SCR出口温度
	
	OBDII_AfterTreat2_SCR_Output_Temperature_PGN=0x00FD38,//后处理2 SCR出口温度
	
	OBDII_Engine_Fuel_PGN=0x00FE81,//发动机燃料流量
	
	OBDII_Friction_Torque_PGN=0x00FEDF,//摩擦扭距
	
	OBDII_Engine_Net_Output_Torque_PGN=0x00F004,//发动机净输出扭距
	
	OBDII_Engine_Bench_Mark_Torque_PGN=0x00F003,//发动机最大基准扭距
	
	OBDII_Speed_PGN=0x00FE6C,//车速
	
	OBDII_Kilometres_PGN=0x00FEE0,//公里数

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
	OBDII_SET_VIN_FILTER=0x0100,//车辆VIN
	OBDII_READ_VIN=0x0001,//车辆VIN
	
	OBDII_SET_CRUUENT_FILTER=0x0102,//读取当前故障码
	OBDII_READ_CRUUENT_DTC=0x0002,//读取当前故障码
	
	OBDII_SET_HISTORY_DTC_FILTER=0x0103,//读取历史故障码
	OBDII_READ_HISTORY_DTC=0x0003,//读取当前故障码
	
	//////////////////////////////////////////////////////////////	
	OBDII_SET_ENGINE_SPEED_PGN_FILTER=0x0104,//读取发动机转速
	OBDII_READ_ENGINE_SPEED=0x0004,//读取当前故障码

	
	OBDII_SET_BATTRY_VOLTAGE_PGN_FILTER=0x0105,//读取发动机转速
	OBDII_READ_BATTRY_VOLTAGE=0x0005,//读取发动机转速
	
	OBDII_SET_ENGINE_START_TIME_PGN_FILTER=0x0106,//读取发动机转速
	OBDII_READ_ENGINE_START_TIME=0x0006,//读取发动机转速

	
	OBDII_SET_ENGINE_STOP_TIME_PGN_FILTER=0x0107,//读取发动机转速
	OBDII_READ_ENGINE_STOP_TIME=0x0007,//读取发动机转速

	OBDII_SET_GAS_COMSUME_PGN_FILTER=0x0108,//读取燃油消耗量
	OBDII_READ_GAS_COMSUME=0x0008,//读取燃油消耗量
	
	OBDII_SET_INJECTON_QUANTITY_PGN_FILTER=0x0109, // 喷油量
	OBDII_READ_INJECTON_QUANTITY=0x0009, // 喷油量
	
	OBDII_SET_Engine_Coolant_Temperature_PGN_FILTER=0x010A,////读取发动机准确温度
	OBDII_READ_Engine_Coolant_Temperature=0x000A,////读取发动机准确温度
	
	OBDII_SET_Fuel_Tank_PGN_FILTER=0x010B,//读取油箱液位
	OBDII_READ_Fuel_Tank=0x000B,//读取油箱液位

	
	OBDII_SET_Reactant_PGN_FILTER=0x010C,//读取油箱液反应剂量
	OBDII_READ_Reactant=0x000C,//读取油箱液反应剂量

	
	OBDII_SET_AIR_INPUT_Quantity_PGN_FILTER=0x010D,//进气量
	OBDII_READ_AIR_INPUT_Quantity=0x000D,//进气量
	
	OBDII_SET_DPF1Differential_Pressure_PGN_FILTER=0x010E,//DPF1压差
	OBDII_READ_DPF1Differential_Pressure=0x000E,//DPF1压差
	
	OBDII_SET_DPF2Differential_Pressure_PGN_FILTER=0x010F,//DPF2压差
	OBDII_READ_DPF2Differential_Pressure=0x000F,//DPF2压差
	
	OBDII_SET_AfterTreat1_SCR_intake_temperature_PGN_FILTER=0x0110 ,//后处理1 SCR进气温度
	OBDII_READ_AfterTreat1_SCR_intake_temperature=0x0010 ,//后处理1 SCR进气温度
	
	OBDII_SET_AfterTreat2_SCR_intake_temperature_PGN_FILTER=0x0111,//后处理2 SCR进气温度
	OBDII_READ_AfterTreat2_SCR_intake_temperature=0x0011,//后处理2 SCR进气温度
	
	OBDII_SET_AfterTreat1_SCR_Output_Temperature_PGN_FILTER=0x0112,//后处理1 SCR出口温度
	OBDII_READ_AfterTreat1_SCR_Output_Temperature=0x0012,//后处理1 SCR出口温度
	
	OBDII_SET_AfterTreat2_SCR_Output_Temperature_PGN_FILTER=0x0113,//后处理2 SCR出口温度
	OBDII_READ_AfterTreat2_SCR_Output_Temperature=0x0013,//后处理2 SCR出口温度
	
	OBDII_SET_Engine_Fuel_PGN_FILTER=0x0114,//发动机燃料流量
	OBDII_READ_Engine_Fuel=0x0014,//发动机燃料流量
	
	OBDII_SET_Friction_Torque_PGN_FILTER=0x0115,//摩擦扭距
	OBDII_READ_Friction_Torque=0x0015,//摩擦扭距
	
	OBDII_SET_Engine_Net_Output_Torque_PGN_FILTER=0x0116,//发动机净输出扭距
	OBDII_READ_Engine_Net_Output_Torque=0x0016,//发动机净输出扭距
	
	OBDII_SET_Engine_Bench_Mark_Torque_PGN_FILTER=0x0117,//发动机最大基准扭距
	OBDII_READ_Engine_Bench_Mark_Torque=0x0017,//发动机最大基准扭距
	
	OBDII_SET_Speed_PGN_FILTER=0x0118,//车速
	OBDII_READ_Speed=0x0018,//车速
	
	OBDII_SET_Kilometres_PGN_FILTER=0x0119,//公里数
	OBDII_READ_Kilometres=0x0019,//公里数

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
void OBDII_SET_VIN_FILTER_FUNCTON(uint8_t index)//车辆VIN
{
	OBDII_SEND(index);
}
void OBDII_READ_VIN_FUNCTON(uint8_t index)//车辆VIN
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

void OBDII_SET_CRUUENT_DTC_FILTER_FUNCTON(uint8_t index)//读取当前故障码
{	
	OBDII_SEND(index);

}
void OBDII_READ_CRUUENT_DTC_FUNCTON(uint8_t index)//读取当前故障码
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

void OBDII_SET_HISTORY_DTC_FILTER_FUNCTON(uint8_t index)//读取历史故障码
{
	OBDII_SEND(index);
}
void OBDII_READ_HISTORY_DTC_FUNCTON(uint8_t index)//读取当前故障码
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

void OBDII_SET_ENGINE_SPEED_PGN_FILTER_FUNCTON(uint8_t index)//读取发动机转速
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_ENGINE_SPEED_FUNCTON(uint8_t index)//读取当前故障码
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

void OBDII_SET_BATTRY_VOLTAGE_PGN_FILTER_FUNCTON(uint8_t index)//读取发动机转速
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_BATTRY_VOLTAGE_FUNCTON(uint8_t index)//读取发动机转速
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

void OBDII_SET_ENGINE_START_TIME_PGN_FILTER_FUNCTON(uint8_t index)//读取发动机转速
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_ENGINE_START_TIME_FUNCTON(uint8_t index)//读取发动机转速
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


void OBDII_SET_ENGINE_STOP_TIME_PGN_FILTER_FUNCTON(uint8_t index)//读取发动机转速
{
	
}

void OBDII_READ_ENGINE_STOP_TIME_FUNCTON(uint8_t index)//读取发动机转速
{
}

void OBDII_SET_GAS_COMSUME_PGN_FILTER_FUNCTON(uint8_t index)//读取燃油消耗量
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_GAS_COMSUME_FUNCTON(uint8_t index)//读取燃油消耗量
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

void OBDII_SET_INJECTON_QUANTITY_PGN_FILTER_FUNCTON(uint8_t index) // 喷油量
{
	
}
void OBDII_READ_INJECTON_QUANTITY_FUNCTON(uint8_t index) // 喷油量
{
	
}

void OBDII_SET_Engine_Coolant_Temperature_PGN_FILTER_FUNCTON(uint8_t index)////读取发动机准确温度
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_Engine_Coolant_Temperature_FUNCTON(uint8_t index)////读取发动机准确温度
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

void OBDII_SET_Fuel_Tank_PGN_FILTER_FUNCTON(uint8_t index)//读取油箱液位
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_Fuel_Tank_FUNCTON(uint8_t index)//读取油箱液位
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


void OBDII_SET_Reactant_PGN_FILTER_FUNCTON(uint8_t index)//读取油箱液反应剂量
{
	
}
void OBDII_READ_Reactant_FUNCTON(uint8_t index)//读取油箱液反应剂量
{

}


void OBDII_SET_AIR_INPUT_Quantity_PGN_FILTER_FUNCTON(uint8_t index)//进气量
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_AIR_INPUT_Quantity_FUNCTON(uint8_t index)//进气量
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

void OBDII_SET_DPF1Differential_Pressure_PGN_FILTER_FUNCTON(uint8_t index)//DPF1压差
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_DPF1Differential_Pressure_FUNCTON(uint8_t index)//DPF1压差
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

void OBDII_SET_DPF2Differential_Pressure_PGN_FILTER_FUNCTON(uint8_t index)//DPF2压差
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_DPF2Differential_Pressure_FUNCTON(uint8_t index)//DPF2压差
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


void OBDII_SET_AfterTreat1_SCR_intake_temperature_PGN_FILTER_FUNCTON(uint8_t index)//后处理1 SCR进气温度
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_AfterTreat1_SCR_intake_temperature_FUNCTON(uint8_t index)//后处理1 SCR进气温度
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

void OBDII_SET_AfterTreat2_SCR_intake_temperature_PGN_FILTER_FUNCTON(uint8_t index)//后处理2 SCR进气温度
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_AfterTreat2_SCR_intake_temperature_FUNCTON(uint8_t index)//后处理2 SCR进气温度
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

void OBDII_SET_AfterTreat1_SCR_Output_Temperature_PGN_FILTER_FUNCTON(uint8_t index)//后处理1 SCR出口温度
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_AfterTreat1_SCR_Output_Temperature_FUNCTON(uint8_t index)//后处理1 SCR出口温度
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

void OBDII_SET_AfterTreat2_SCR_Output_Temperature_PGN_FILTER_FUNCTON(uint8_t index)//后处理2 SCR出口温度
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_AfterTreat2_SCR_Output_Temperature_FUNCTON(uint8_t index)//后处理2 SCR出口温度
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

void OBDII_SET_Engine_Fuel_PGN_FILTER_FUNCTON(uint8_t index)//发动机燃料流量
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_Engine_Fuel_FUNCTON(uint8_t index)//发动机燃料流量
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

void OBDII_SET_Friction_Torque_PGN_FILTER_FUNCTON(uint8_t index)//摩擦扭距
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_Friction_Torque_FUNCTON(uint8_t index)//摩擦扭距
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

void OBDII_SET_Engine_Net_Output_Torque_PGN_FILTER_FUNCTON(uint8_t index)//发动机净输出扭距
{
		OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_Engine_Net_Output_Torque_FUNCTON(uint8_t index)//发动机净输出扭距
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

void OBDII_SET_Engine_Bench_Mark_Torque_PGN_FILTER_FUNCTON(uint8_t index)//发动机最大基准扭距
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_Engine_Bench_Mark_Torque_FUNCTON(uint8_t index)//发动机最大基准扭距
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

void OBDII_SET_Speed_PGN_FILTER_FUNCTON(uint8_t index)//车速
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_Speed_FUNCTON(uint8_t index)//车速
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

void OBDII_SET_Kilometres_PGN_FILTER_FUNCTON(uint8_t index)//公里数
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_Kilometres_FUNCTON(uint8_t index)//公里数
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
b#e� D   E-SafeNet   LOCK            脔漺 y�3噿Н�)��R煪f                                                                                                                    蓉狿V漜�#F�
S駰吹鱏Vz恛嵻^鳂E坉聿p嬓嬌l%K内vX誒�+漑筦�'H9�-�+浰q蝅<(赧蟦腦� 騙铕~8�(|�1W雞呬浴衆淝�-鬛�*�Qya!QD@�O,諫阞箽恁峼�0繜駙�;鯁飷羋99pZs世�4孢}菤sU壦敡k�!@杮嗅�;楫鬬�	�$EC6u�1[�谕甿�;鼳梸弝o鋶庤<%藐� hU*��亷�(JC陂�=�&S{戽烦妡暽翳曑	/4�+2﹋簦乴M廴r譥蒎(1�0!a�鎌�(l焜�`�6颐^6篝�Gz�<鹊苻)炷s皏g壡勴cv\闻TQ逰�>�	噂�*?�y�/櫱q揫濝皱%憉��]趽Uz髇1~�3黶掍铫慐H<<釽癓圗r"[�7(>i=9\!��+踹蘲>蕊滠鵶渡Ⅱ觔1mwIn缤坵徂j踄染s旑帶Q�44w4槙m缢敔�)襢7�0Cu*=桼f樣�;镟瀖訵'�.痁厈鐛箩>&戧�4&徠#梄烧LqW�#;[厁�:CIo痃蛱魕疚囏茔?$�&w�憆惵�^蹗1~贜摾=鷎u2P��Z峔Vk鳵!稢埈埄钴(鬛穉鄣秅@�>{O矢\箦%U�靫�1Z䦂綃�/鏺夠符'e�6(辡朆膂F鶪D�筥6e�23簎�<槫IU�*��2#g
}g	A濔羡\垊絷呶D庿]忄�9E�(鳆~wr晅�詩藻6n�G褽A?憅�"�0(�?措u蠫=壀锹R篂�()�-鹒嘼悊'冈腜驽9z恊牊L敐p鎴0�9ㄚ5缊HC幃'|媯� 榳��$玥a烿s鏻袌5�嘿茱%�旹猆墮2�%r:鹸�6D令�-燎P^p�4�$�Uhf?憑@AQ:=>Q,��&鴼蚋慹<挼具R爌鴭怵� z4-1P晫�94�+]X楒;驭威#�p{?Zm嚏6唱�誏�9
N:*丷6堁� 卖�(肒)朇S���憭�;4抓�>t5ㄦJ猦果l\*镚E;汞唚鈠T!裿彼垞毧ObXi鄛=��3蹐�[憘R8�覝52�7z_Dn桝蒓�1&h9H�;e?�-喎涮笪+玔�.�,胆�?'�,0j卜$v澟琞�怳�萧k�+夻爳!|�62�凾簖&巆j�*宩X=�:qL廋偏Y掍U鈖膗悞烬龑�)蜉焙v陮岳檣忪-,ふ.律荕i卪�下嗭/Ka%蠸ｓ扜D\苂C乐篪�!蟞嗼曱橽綳	辷鱾
�)〾腴`櫢�%@�繱-�+葲A楍埠槂	 掜�.朿���嬁]+6闩^	|铄wh鹢�縢�>鵵
�:k醝��(燍, ;
�鑯袓u53W臟Y�x誝-繈螞郿a叧9 豬刧�i�;
i佤啾�(:�亟众�Xz噍龝�8婂牪蘬9m_|y饶搖鹋k踄 2翩&伝袝K�z=Xo蝲�g)"~琩柪j以罹蓢C顊鳙�偬�餥b�>
岴M苔呓'a伱;�>{U\c薆�$s苾歕馚8煃�?~wQ戌尝�8饜蜣]} �T�霜5o 籞菭鳾圮,_}aHk�*�)q﹒y=X8Z�&s1�瘖谬章+㏄錋耓擞瑆V鉋溅X`�篲m鮃鎖�7A亸�"贒犢柧W�
�痭至℅r�2�&x賠8�4蘾爬&bL鷘埼5 J ^`睓澉i铤汆燲少曘舓S�3跬戞\F)m�%□�-G\_廑'S�髦t8�%?F馫��軹十'�4伋熼`帴�5}�(鶿聓垎u8ヂ祓鑕彟j�澯(囼z鄭5f�6V�揃:缰爱鵜f鎷~'窽�#鵄�s�=?�#場m卂E]頔��Y_u鱦*q�'D鶦>瑠紧�崳CLs�y�偪�qg樚忓蓠噬�=�呔愖�Xl鐥�/扒混�^K_gi	仍昁姓t蝐萎d溤皵@飉."G^�:;穣�:0c.坦�>贾Kj耎攬餌鵯C�(俇�o邭囲+c钪=�'!氉w糲屟R4R�>:@筮�僪KO>钠狴襳盺TP�W� ��>�6i  }厂tX鹢代EX�<b��?~痸RiF:�7Q萀領?&�&�緖�:*慚耟:O谘B眈'x�'艵�*g嚑�6鬺麑滊Xe�3 玥蛳 �啌~x�$
�8�O�勩J@��減\3Z"3p棦势\鳂谇�詻T$押I骇寒攇媬�.)E鼙^蹶唬faz?湎((
齖袸m漰�9�'頨釤:�$厴}鹎鳭kO峴�/E垺ZUㄢ+`p!D�1?彑馦榝胶P彭X榋睡L毲J懶餺��呓4夈F縰T荗掂[﹚n蹥毆b訶�
� l潺(�?hV��3ARo@+{�-陵z	葅嘸醷/aj�8/9CijCe騋竫∨菶H矑骋
�3惄�]W_aR9�帎9馜4<髣F九纼I渇[W`炖f鯄苣丣�<P楨7YtC�皎耎仪��qW�/桱�=莫δ崧0�< ;��>�:闱~\*�1俾��
>yyG糖崚�蹱蜣a+K臶i�玘邦鑢o:3R鎥_Y蘃>mx礵飉$酑N^4N�;s"�7暚笾7篲�4�;价醟K耟:{o沧I鳋;Y�齳�!^憞�#靣侐欦4.殈k峃�9夶(瀔d�0抝_N�(圱�就;az��颿$M#,uvk簲蝥f韱诶坽篪0#侈"铝豛i乧�<嘉太v�&塔�/":/闩&(;蒩饇m盨���馄_�b虉祢z劏�4@�繮>鐵ХN旇我鵋浯>P笲��庢S昄疟L滇-$餮@`耧j@歷縒馬�"� ~�#�メP璵d荻⿻H�Q�3島53c�#R�繳 脨�毑1:蝝歷鴆15�<^eRQw5H葉諥愻嬟�@f腔+�呩仩孮Z	%t脳蚚启P鱕5莶x溤＜a�<8f(劎k按�艚q�3G慣 rEG烡8侧�在�#袵i�>琝妠s�范�2!憎�!1%哄M�当(X-楥4合��$qKk钶啝�2链囹燃<@}O磂>�燯进鏧O狴y襱T`緿XzbL.�轑a獆b+N$o�X�灝砬驯'��
侴醋�^�8Va=犍e畦#U俠�&s珑絿�/鷠圂秳!m�"2�╦似甗r�2扨:\銱H�*�?{d��/峯|u2;J尮鲜f鴱郁阞　mo雰Ц筕t昺�月噽Vs単傔�
/鳄";�j飇|1訠漑�*w�駢�.鑷w�)l�7鯾鳩妴t*づ鐁啎紼f教k龓U濿斯@$[囗k0Z油JI逰�╗晑�7H.�,�;筑�7J÷趭M鮡�0�/z戺S~醞*u�&_䅟1欢宸翫R 3��$�-rB&*�%jvTZl=g鉘阓б踞�Bp砬鷾�6谤桓蛚2ka\i蔺盨释5�z|陵$漳G�t%摓i哀瓯桺�>H縯A:�d樞�&梓�鎖I僊�9颽,改?�6&.樓b�讹a�$;S�,�8RLo鲼牑�	侏趷闈jDw鼅]竬璘荵O赀v�⑧YO蘱;
*A��^岼Jdo�"k.橭濗爛q鏴镋�$寡�H�Ge,赎n用d;H�鰀�
x瓚�肎摧惍F�'!�:馱ǜc��:瀒_I�'瑃�!忓z� �9孞]7_#A暊蘖X芙镬檔脲N絿w焻�M��抑堣M:a単趁�
A/跞3>;闌藄f)昻�#�%3頦釠�}际たH�5M�
繠觭姺u<懊鋠毞|/�?蒯3嘁7记h絞鱾z凋%'侉Q[5ā)7縌僰詅睯�e�0�#ヤj匯n来父#�!礷鸑
忺x\蜦U�5_駓1濞頰g厷\(R�0�#N癝|lkozPh鳼莀号�At皴慨r嘥畻颫PIwZ崖晑朕F阨暇I┨"嶱\zJ璞*黻崖孞�3R竲
N +�-[堉�'橼�"腳r�>剗�6A嗏膂ni澥<�1?勁j峈厄G%�?;B泗��'
B[Q遗溂�擃繖|Nj�V硉籝毨菴O+t裻耙hv蘈:4A�>�.S膁&h;痎c"�*垺镧唆婣��撌�&e�5go,镯n礴*f��!装栗�)駀忰粓F�9� 搗夘g�+V珽餬TD�/筩�7翟1jX�蔿�6$M#,uk窉�鷓鲀翦㏒蛇捳-哑�5j�.錫安醽2 +$蘘ヵ�"48输%<閖飇|1訠漑蒵K�?雌_鱩\晔ぢJ綊�=)�-阯J鑄吉N掹谑䓖尛kW�4牊e呺E昡摔@ｍ_Y艽=f崶.4�漴��!甿d歋v鈏諐0�寇訇 �怈疨寽7� w?䙌�3A碾�淙_[p�0�#=f璂pqkT]l:H踓軾鑷Д峹!惙 G�3滆伦〣]IaC/淫W�6E婋'栅蟕�� [9M朊呦崖孞�3D揘<c.*�&K浑�刈莭�6蜸r�P壅*]SJ镨{P盬揯^sムG獈额gd�	g门��/t>怦蘵彼}}1拯蚌v樓5��'#;4^鎥哥EU贙?9{�:�?h'CK:頑P譕钛Hd<�詏萷�4WwJ�%qD浊Y黯;S�齞�'W殶兵��鰪若G昋壳2��x�嗥�� 	苖nS�)蟶奴Y
瞝鐸�hxhu��敎囩拨�儎KM违R恭�6�阯拷顜=T6:�昊�$)79镌2$!詒鯾w玁��踻釐�3产炍碫t&俉⑤燱X龓{|G樈<嚜w���-痂傚S扸荩O&T旤=f晞 雤麺𪺞堬剋桾荎皤�7~曳繉P鈏�0�>h麽cG袬D�s繪)茘齟e欄@o�	��蓧琊E祪姀呈锅��>譏扊柺�Pw嘹姚+�婐�戶L\=dM垜�8珶�窝�m笲q7y�P\zJ鑵Y憦走汧�3R榏*n9 � @Ⅵ�刳�
鴔A�-
哸�*e毁一s~�3縘黉鎏E�4�"e/[)�{哌��$rpI颂棡�叨筱豳-Jv穊"��3�`鍕�椰C黈x��_yfK-�,�)g穔n6N3D�;b%�0暠6獳�6�4杰�R漌xn<灅z亿7��殉$X$k�+夻爳!|�2'�楨恹$焳~�:宺BI�|L�6衏莰Y揬媐補墕�響诙�0鐘倮廰��0=λ.伤赥e刬�勒怇T2UA趤襕ZCH姢N讗喤R塴猃稡赒罟[O觩鷤�5耨r灗�5Z��>Z�&扔?u䴓(T� T鄺�Q�篜暖誠来V｝1,疔Du潆pr鴘�筭�e�2�饿Z�杳酋6�怈牶踦~�Q_�諧<畢臎�j帳$;豩杊鎝6vl耾p13S!R蠯�覠礒:疒谆6�愹悩蝚>wew_淄�z痫M踜 群b岡厡^�	T>祻I桷姘�3濦8�5L柦奉鵉幕FX頤24� �
鬶亀疭B侪Ｉ 哜�呂b岰惰G<P�8.F泸蘣�-KO扬_,&霄崙s殽4�R赠  }耧9^餿NB誱04E��0im+]"O腛訰窦啨�+�&�紆�)/R�與|B籽T囹 D�;蘂�*i盃硝匳�剱唆掴X值J�9�|炎( 攟�'O�5朥�抵EF�"�渙n	up絺唭A橓��オ@Y癥lGe鱟齉S~�rVW�.*7/纛 %/
�廡|萞紐涚嘥搈�惦Ws}�6?wQ稪�	c鑄吉N傭适鰘毞z�=林4杆n曳Q典+%鹆_k贅�漴E�6鯜�7A�3�除M�u喜織P鴅�:� m忠Tl顊!-�*�,*请痽I髫
X}~V�3蔲阘6p稢jad,WZg駿齟∞均�cV屉鎬�掕悰锳\R,垚裃6鶹6Dkb�嗞贏�[(_妭纹鵊�
q砲N."�"f樠�'镟莭�7�$r�[�9[?盶粚s焣>ｋ\皉姐ff�&T疱�=�!VRQ茵凡�,靵蜣?>
�@��,艞�[;%h扥]歐4"{�+�$q琱mPUl�^�浦漨�遃隧瑆'塅/无9c鐘鮃�!�1J殏粴�#鐌囑惥w�髒俋驵5矻Y�箁be�2$╮�!徾&ck��*梲{z/wk瑞棞;)/訚�7!�7匬p[踿)螲咝呰[$KP葛�$灶 `7賕郋\8檜��4+霿酞!�4蓣謫燰t&�+オJ'匕DlOXmy鱺 >P笲#鲶咠I橤荼y炃=轴l"go镃�)橞昋�'W;�8�3埱@皔系繅[魚俖鳲拏7ǖw捽�<==<w深	Q竬U巚乵醡 j禜tC@,oGA`�荴栧�<pX谇鐥�8花襤j)x,L墠�;>C�茲G殘U�6�熰趜J枨軌苣丣�?R侲"F=�u墦�拮�肸w�梀媕x鶋桒-4钫9�'6剧Q�憣`緀`坋4� d[d}\1tm0�(,歼"	>� W粂籡捰腳~厶	&鎥踞ER鎙%p��\塋^tK:頑Q諳钛z;�/�"�9Hc#澷茺T棔圾 R�鷑�!I缚椃�諧矌粓+q�8�7�稲H�╔de�2朄�玖$錽鯳�h悿簏�鉀鄗鷞k6餹琽雰2蕴躊i僡�嫌дw	Mg卼搩�8(!/辋$:;蜳襖W>檍�#裭v�廁f諬~Z4� 艩)[� 伽e�0�P芙椱鮭仭l	�蛐愚4\﹙畀O=/砣Ym湗\�唓�G�7韥眪罉�c犿48弑矊F額�.� y驮Sv釶e�>痙^埘磋�梚Q呦�鎖泳�"[�?,{YZz#R襝荕м坚�'l膑醿
�呩亗鍰MEz!M缿�%睖6懧智{睪k碣�[>硰Z憴仳�x窫7O:+貰l蛼豹諌溒;培#>�佱-�,嵥m唄зVo�>?G膺�7�$Ra^盘崌�省鮼呰P|Fp銄=M烺砄]栢�[赋损�R裓94a�*�%@廞EN8o�-c�磰踺蔑
泏錋耓摔猭8噍懚�%b�	}弊�:�刷t恿麏�"鷎欙7f��"侷儡	瞹`�穜Bo�34朧�德?hq�鶹�v{hu揨f�&)z}莧∑_jp渷赱r榤�沼咡W~|巔阁�/腙
鐳蔍|�.燻儅i硡}鶤潌s]�KE掊榘I�`甕骇N庝哿鑖笉V>�贳3蟒樼H�#.蹥NF�7笣娆�-r鬾�礝�i;�=饁倰/�寇Ep膆!礷島5祚~Q蜳U�}鰌啞舳訠洜>+蝩條�O0`�.
鸲榆饵�=慏楏栜�Tr貨�0耙礁�0ga0-D攷苇/p鰐鹽费k倌G�_3▌_悶��n�E_<:�w壇�)悘趍鵯C�(�~T努д!�6"询	�溦L%�>oq勰��:l:吖咚辻擃跍$>�V漧憸�''~厂x譞墦U_譗/(`�:�K葁IxI�w=�?挑颜T�1鱚旸院砲8郋/q狈$潣zK4衋��Q'皲蓐襂�閾唆G縓I╤�$姖E�盳�9/銱H�阺胜V_j��/渧g~Y詫豆0囩婀�檱T牽Q︷�)醆�%饥箰"W)m�$藖襕X_@揎QMa6]�廡~�.鑙剗v�嶇e俈a眺謫崆鳭k%�;�3F梟扔+i﹪嘙j葀暪\桃?鄴b簎g趻NY艽;z贆G哛�;�坔鏽
3匧u�>蕰3蠪犨平?� �癘撍
(�?  閪�,^囚�匙@Xo�	�榎Oqa�'	1-#Ly�;瓏锕�0!摠肼瓥�駩ug),dM蹗裭磥5巭侵偦槻;.sBl巡+尻�;�@'�+P PO7~誖8挖�b僵�5�'襋&��6-筘协sa傻N礣c,J賯/覱褜}陑{喊裩齯WF9耽�:媽腕Qm[v穊'�詃臒�幪];塖鸵:1eCKm繼誁谸NZ1Z:頒@萐刮湬���/鴀�刷糼'穁 r;姉yTc蛖��8珉搋匳�陮滊	糋穡�'暿Z���6,�:q�*�+诖V錽�Qwwkj\揉洘	续�陨U靲p湪�9�橪毥顜=T6:�:葹匘D\_墹NN~aK��c�1縹泘i�戜z誋~硗o例燴{)�軬#領S圑惺駖劶v蚦駱f教j疵/�#鷳*茠Y敤=.艅E�瀖��"�:\.橪!�%謷g�爧淆#乀�琌D艎(�?h<�*�0^摗忤両F蛩@o�驫�SxW4�;]Rp1#KL1��2頉镯�+=渠蹀O鮩鍔�5{)xM~Q堎噐█a�6\E骡.栈樁6.tl巡+尻産�:C SC顊鳙�悘趍�|�#妟笻X尸桔泰"榬Z/J捗6�戄}陑檂醗"产膣襥鎭茞呰+[v�4+�測棆慤L挐'�退,.�_PKa臖簃$酑woV*禦W/�'埛踮樯$璍�$�%�[�/a}r聲惶ez蔮橲蜬$畛迈裋�裕_糋��;菰�U��n0賜q�>蘡掅�%﨡驡wg?c脏弥[匄屣�溣TR秃�)N�閝梏鴯")%�%焾扜[脓Q7(H狹�|�'▆�)#�幊3淜a橑灇滴竀kq貃�3羛杂i釠哣靛9z恛嶈@韬P刨F�⑿?寿-+嗤D	m溟`b鮴�絞�y6� T荗亽g�爧楌#乀廮琌D搵(�<h<�*�0^摗徼両F@9�驫薊SxW7�;]y1#KD4�:�+鯒�钀9躲��u瓆鯁磼釯P_{Y)玳℅��K鏨4!鶆P扒礀\痲
}`璞+鴞綴;鎎SPO7�^$厱�~胾��r驥厁1粣医sa杉F笻+1茠{�艑Za緕7� �7WFh划夤蝘苍邊嵓P5~鉨j��,寳�嚂Q'罭灍*e鮺y@q擽瀁�SsZ:�JL�裎喒预C�.�侴隧�!;�0r:惺Xn;遪�襌T恿�鏈臓'a�>/�楯犴"搎y�'卹RT�?m ��(邳	�;�Qw{w"傈嬎
樹����铑�5〣黰犼⒚>Kas�9咨歔GC灴l[nD�� nA�1縹泘i�戜z誋~翔仚庳疷t&抣�,E�9滋(v䴓焙�缜u焋偤C洏u銔5�<0艒MF嫬"yē.4�漨���9fF縗y韋來]x蠣硲P鬮�&�:c疸]肞W�p諴:畺�<*聹PD'��蠩Jd)�'Lyy?6�╯鞆螗�.!忇ＦS�%鍠沩�4o50PyE攽檙綔)�6@Y姡/璃蝎k阸fj&旺c弼舶癳��:C \c閤嘅耎渐趍蚢E�(渁Y原糈-�=掐�>拰Z0緕7S，� 齟WF>寒夤蚷苍邊嵓P5'鉨j淸�,屃�帣Q'罬蜔*e�(y@t與霡�VNM{ �&i薒钕叇r旴調欿鄣�U�#z_厶T瘀0I��d�*I檸瓡�(靌忦盀:^�+k峃�+暿���&x賠mL�6�邳A	�;⺗�Q!{w"厅嬎	樹���佴M铑�5�鮩犼鴻>Kap�9咨�GC柣MQ)(�8�l豤�筟O�8钒	鱩q犁幫|彸�3[�踈(鍮^國伊鰎崟紼さ勏;鶐*奋/拻RE旤=n懶�蔰�纕�#{�?醰偰&�I犌邗w�揰鳲弮B-緍U駌縜B飞娡�_[R�0竨�._hq塜wa} B@v(^襶連涷�畦	Cg嗵诳:�掕悰锳\RU0!但鑟a�3\E潞#栈樸>."l巡,尻�;骪o�*OO{b�
$蛶�*そ謢蜬?襋&�
�*e回谓'h墿N�kxJ捘e�憣}�,*O泭墄韚b垕骃灸嚋芜"	>� W粂籝亨鑓o+睐<E韙WX�4	}aH_�嘦谸JW=Fr P淪汃煭W辠酙朳凁齥'�U,n纩2瀲-T/觺袽烺8麻��槐铥H�
J跩�姖Y� 攟
蒪a\睮�Ｍ)w`��(剉gut蟒|鳂谇檟彗0'橱Q蕝�M�� ゾ褓sN5%�%诐�[\栿VQ}~��J*�2�)別j琂廂y�h仉炍V踣�=0憇�3Z�&��?u嵯�皫Uv�7潵@勏<┻�3⑿?棊>d畭v
2珗朡�糮筈�{�9�寸@砵~垃B鬛�3島5貎By�#hh癵痙^坭凹�	麩CDs稟��-N}耾SCyeu
P1��'逑攻恱i彺J� 噩稞�-*5\%t棊� 穧,x胞6佾洔L�M$禃\敃撩嘚�9Z揈3g2 	�&Q�悘�6�烼r驥�/1粣医sa傻笻+cJ茠{�艑Z7緕7� 齟WF7寒夤沘苍呲尲P5Gv鉨j��,屄�厮Q'�蜔*e鮹(}aa楴汱r�2"M{ 禦@�,兒憷笙>痁�6�8ぶ�
D�'v}r聲粭.Tc瀧��=螓婖蚃僙鰟入燵V�~�'暿���&x弡mL�6�邳	�;鱒�Qr*JP仞浳襞梹�檮Ko雰蕝�9��抑堣M(\A縗�(?<>4 -蝜鄚u���馄_�q锌蟽龔酧w9舠�/Z�&溝7!鎳播�鹉"�仴剝v*�?矆&規R良!fG菓�俼萉�6璻B.峆i� 湈/�济帲j�!礝燺逕f�#L躕�<N悑聙鴛櫌10 蝫塴雙0p {q}Dn �>竝逑緺恱iR姶O絨遛�鍖=/*0,仚�$x�6荀&來��>猰b>G8圍7寰�3.�i \_o3�.鳙耎攬餌�(萅q�>�佱櫻R[懃^�>ｋ\皉姐g?G�$*m诊�4�;hjk矧烦�-霒裸遄.1�Wˊ菮╒
D铺H;塖渾6-�(e\[9蹷��(N&�
JP萐刮灔滃K�2�	僑抓�"/鉠Kfg波s倗yTc苬��0螓姡汮僙鰧入E�
k峠�+呄F�/'��  蒪a咲�［$ad��G"u(5H櫨响m庭盱確剞湨!泜�9M醆饥估'W)m�%藖襕S_@薷QMa6]�廡W-�.�7襺v銽囩e俉e眺中崆鳭k%�;�:�孛'y瑳聵�<缜m焋倶f胶P刨F�⑿?寿-+嗤D	l潼{{肙�琀瀍�}�:�筐b蘠<汑嶅?蒑奀�]枱
`�?t ゜R�,荇�缱o�	��O0a�'Me-kVD-�2鶉Д宒!K啝柯鑕鶘坊�!ga00M狼刬壓9�&P枃髠碣�[W`陵`殑谏乗�)V扙%U��帗_棩趍�|�?�謆|灸阴oa澋d賿/覱袆aX�+c铈蟭�->骓�!媽蜖PaG>鷎o�7�<試�vy搐R8�艤52�7Xzb:x礵艪菴d U?C�!i4�1升腕盢�?�-操�E睩oT椼+傁4Q7�6�僋p铥搋匳�瞿与D鐽^玾暈Z�睧�&,�:xD�6�6舀F\C��wRnv�祪�	挟鯢湜HR欖雲�9�竚虩栓i'�5菓�$)79镌3$
;萈轔F-杍韢鑌L�披-汻}锌蟽龔癑w9舠�/Z�%认7!鑿播螾┠"�)仴規v*�?矆y趲R敶!fG攽Y0�
拀豑躌孒3d歋`鈏諐�7J烃/�� �;u觎i]覲Y�袻;繘驯衉T	+纖渙醜+},轐8/=2=#K4�鄇鄾镯寈=迁鬓O鮦鍔�5{)xN,Q堎� █a�6\E骡.栈樁6.tNl巡+尻�;骪o�"O)b�
$蛶�*嘆�'轆#飋�倱線Uq懃^�73｝Qcr�b迷�	�4CWz��䴘骃椖軐婉UaG>甮>蔈�,貤�X幷N'�厞"10Gq\�蔖V�RiF>築T設駟毠���'癒鄣�:;揼9	q狈$儑zK4蠧�"�"噪义�黗傰瓌;f�("�霴淘縋H�礚z_�0$簊�6翟1jz�
�2唌TkvZMz仞罁	些ē鯢蜑HR櫤M焊�)R�絭牼褓+C5%榮诐�[G\栿QQ}~��J)�2�)襤j琂廂y�h仉炍V踣�=o憇�3Z�&�卋$芙镜�的oW篎稚a緱z靷a換乒V叼'(噗Iy愳M[蠴�8朷檕�j9�*�4熯3�3;櫽嶕w�揰�	弮B(�#hh醞痙]坭凹�	麩s稟�.}耾yeuP1��'逑戡罞煾硰S裀忒囹�"x(13R棳����&PU殸Y幌Q�M3_紶篺�~穠<p<=� qⅡ�旅�鈑U僊�9颽=粣兏sa射K笻+dJ茠{覵艑Zf緕7� 玡WF>懂夤蝘苍邊嵓P5~鉨j��,寳�嚂Q'罭艧*e鮺y@q擽�K�5!B1V*隵pm頹孩尳[蝒�3�"彩�V�5wPb匝0~�:繨�!X伓�謃赅檠aX�瞜韘膰F闦甕霼&8舝9O�*�+蛹Z]�"隫籝Ok/!c权媷槹�詻TN羊H���<嘉太T6:�$諡匘D\}瑐=\DD�焀}糤��胹乜0�3#叇趪l灤�%2g�"鬴5觭晸b+懊腌襧槸}�崃)媸v惎説⒗g嫍NY苠8z贅哛�9��9眓
3孌u�>湌3蠪み平?� �笹撍_ �? v鑯�,^囚�硞HXo�_�榎Oxa妐PX9C=3\|籯�芴儤秇1熴馨;�婔姕銻tMn)誓�`碉J齌/辕w掝咖k�-3`4敻~聧�#�L7�*AIS6匞8挖�b钡倉�7哛n��#%ж氳>}価骉c,訋3�輴)FJΠ卙礰BJ3ⅱ蜥僽麻毧ObFu鄛=�0�忹�'
T瀺X鹢代EN諰
15A�:�O�
t1R$U�X�瀶箱茗	廽��4�Q�&gE愦W拷\;�6�僋p傀搋匳�瞿钥D鐯V玾�2潪Z睧�",�:qL�6榡舀F\@��wRgwv�閮�	协ゥ鯢释HR櫤M焊�S�)蟖安彳>'閚患燢KSP菽#%;萷㭎m瀓�#�##鼺寯�5墿谧Xè�6y�1䎱誷呄[T劬濂蟃"褄仴剣v*�?矆,趲R澕!fG纳蜭詑萉�7璻B{丳i� 蕩/�济庰6�!礝燺逕f�w?䙌�3A碾嵨窲|祈PT宬靘6|tC@,oGA`�荴栧�谏<aF饲菞�8花襤IGfC;耥礠滞5�zU汒6偒蝎o鐀fju旺c弼舶�3籃'�-SLvv菳l洅鉨宓蝍哣7蜯n�
�6-髰凇o)准R� cdV谮:�倌Gi�2)T喊裩齳KOXk�徹徂y緳嬥笟i6+K舝2�烻躲鑔u&9H鳰壷hI醹4Zw�<�/[淗^N"o�I�磳萧�羦��
�殸皐o甠0&'傁e\7�6�僋p珞搋��瞿偨D�_玾�;憺Z睧�&,�:tE�阺胜VY	罭蚾�hbhu纵焙B襞棭�屝+ ッ4邵蠱i怋�$馄衬s+J砎殿�以5<>鑄薡@)��衒麢(�;\晡諙禆礦kq躹�3�&杂i禌�湄Kc坈濏
剾j磼.�#/茠P湪=.Z檮E�瀖��"�:s橪!▇謷g哫爧契#乀�
齬6儞8�#踇M�d虽犱蕕b彯9+聏噕锽-]�rTQNPa#N釩鑯¤嬸�8tA揿鎬餼壂旁�vgaa0M览�8慈)�*@娿:审鱼"�%o*[p櫙z骀�/�q�6SXLKc~�$褣穊�5�z髃~�苂1族鑴p~灤H篧|Yp阗_�=蓽q[�{哌��
>YYg扃崌�<蚧鼷&;� o謅��R媮Mo�褍b-�4e[%�裀谸Z=Fr養 P淪跻汃湱W辠酙朳凁'�
,n妯;瀲-]#觺袾桼8Ν旈��陮溄^潁F齰夘g�+V聏�6 �9>峅�噶7`z�-�璟v_2+'ぽ焙峪彖�溣W秃Ω�)N�閝琛駦" -�%熢薌[困Q7wH狹�|�x爙�)v�幊`�\赙帠佰躻Q 抣�2]�9蹰"嵑數�钃|�涫?胥娦s縢霟q钓振h	o骀{{�f�猅躌孒3k蝄i�'蠋/榑济庻?�莀銼蹌
4穡a(齜N�,B奂梆�X\D'��蠥Gd)�.Ly-?8�╯皬螗�.w忇ＢS�%趁残�-w9 U0!但�幑b�zU汒6偰G�W6緦i讥觊�8xESC顊4啋�3ī�5�+�n�謆-衬阴la澋珸d徝/覱徻aX魋c畎漷�->忭�!诒堪@mW;��4�3蹡�y搐K�!輷:=鑇4"{�+�?k哶Bt)D�s/�眿拗	焝�.�,凳�[�/a}r聲惶uTc瀧��=螓婖蚃僙鲐入燵V�~�'暿�鍱�&x賠mL�?�邳	�;�Qw{w"傈嬎\愪燲��R薛M铑餯;�錫帮睚qn�響奒K?湎((!踧鄆\+憄��42脈丝%�川芮F▕�&V碿�3�#杂8錄�湄Kj坈濏_剾j礇"�#鷷~茠篓=.Y槃E�瀖�O�"�:
{橪!�嚡�皫跈�%瞖塒寕7�R�;�<N虽麐髐i偢#1貄璑罦m4渢]zy(oxl(n錐韓婄樭�X胼翉S裀忒待恥6,,d崓裭磤5巭@栯n位蹄v雃z"&焰+鴞颸;鎎ZPO7+躛$吥鰚蕔�荵r驥�"1粣勲sa傻N笻+2棷	�蓽_}|烜FI蕧韚[LqL扑洖�郀埙	#�7|橴籕曊蕆~誊r�【砪e
t�嘦谸R=Fr P淧褚汃暐W辠�朳凅鷎'�
,n妯?瀲-+觺�烺8烁餷�鏈欦4.殈k峃�`�盳�&/苖nS��(秹|絚�)亄`u{珷拗]鳂谇檟彗0'橱Q蕝�M�� ゾ褓sN5%�%诐�[\栿VQ}~��J*�2�)別j琂廂y�h仉炍V踣�=0憇�3Z�&��?u嵯�皫Uv�7潵@勏<┻�3⑿?棊>d畭v
2珗朡�糮筈�{�/)�#炦3�3;櫽嶕w�揰�	弮B(�#hh醞痙]坭凹�	麩s稟�.}耾yeuP1��'逑戡罞煾硰S裀忒囹�"x+13R棊�幑b�zU汒6偰G�W#w韩篺�3G慣<f53�8’臶妤�5�+�?�謆-荒阴o5澋d託/覱弻aXc畎檛�-B>觏�!媽蜖PaG>鰆>蔈�$貤貹LX幷M'�呎`�uPKaY�2雑'揹&h1V*�	c"�*垺碣$��忕�4j�6>
W俱;手`H+�/��8铉陆蚟�緩憎X[㎡J�?竕墏糄��:0憆uP鶁哻拼眘縎鶥$M
wgzY詫豆4⒌�3ΖnB羾w噻鼄瀎�偻掍\?1`墈搲�K骂H\D�?禮["�&�%攞6肰呖4羓[椴秦G酲鬦+V�"隻5藄柮:y釋摕継Gb�;嶚	趯W偏o祡鋈@瀭E庆l5T陷I �縒馶檂鮭羂)�(怜镵X䥽螝{慜��]滓ez騣>K鏵k�d嫖壌頤A繂M\*頦�鱌_b恠\Lw$yHZe騉�%桗浭�	Er剷欫�0γ货胉3FdIh缑攔鉁)抲叽p谚嵎Q�<"s("敩}�
�	z礿+�`滮�'�=镴f� r癦�r砝佮;珍
�'1!寢*�:愕RD�>.蹴��4Val麂代骃楅畔,'�>w� 噏慀臠^嵯q遬晤1(�_}aa楴�@�J1U�X縌溌柋吔P�>�.�
忯莟J鬵9	S敻+挆3i�'�擽a猝棲軲�郎嶐C
潁F齪ǜc��碙uk�7,\�;纒埋JQ�砄砢M&^63蕧��3堣僵�3DB联p渷錐R癙�簏2F9k儅喴�K珌hN~>骩薐G-妀�4�$!�溁
�%仠菨愂絖k;與骹�+輫'ｋ鸱�阜W;�.鳇G嗞s鳢誇巡@."x3J镊sO過譽迀�b�	<�#久k�A蠗棻n漒��]_v�4U琠<陁 燑甙憕b彯9+聑哷W=F?�/jQ&wZ^鳥�稼汗�!G嚽缫U�耗蒖5lfN0严�%诧m瀒硬I淊喌Q�88;zJ毲靥搳羣�{�2B&�`壦�7皑觤�$卨T�:弢=＝毕 渖5�)&1忼Q哣崝b�j裳�=�0?VJo�;鷯毋族+[f�=p濼岰栆荎u勖T7�萸hh�1u}aH-胏靑'�B1V渰%i�祴冂尛@�褁莊駵�"{�>
W椼妇\D;遪�茁剳�豧Ｐ參@筁k峃詖ǜc�/'�筜cr嘼pG�閊帙{.(�5��fnw綖�靑飭孪弐6J斻獍蕀窲�$杌太iv晐壟�4SZ衷 %/_�?禮["�&�%�"#韁甩u荊a凼たF鬒iV蒫飀�蜿r0棵κ�珔W�.亵3致<,鐽过阕屑R"屯J� �(慓�6�6"�1�%懱j��%�0�H聨m頰,(�&�5!墵榄騖D悗:�d韏錪 n�,8/=$x~@u縏鮫娪踞�sF冼柯S�4牷坣:}_L}	冼僡庵0�=}扯s旇惇&�02M!暙D烦鋝�vOS*!俍w惼�6�訹a楬w�>�佱掙1q尌1�<%哄M約ㄨc
I�.*J环魐�53張蜥辻舅垳赔	# �3`嶻亊伬�Z袗@2�厯ss鱹-]\|�
�r�v&�H淣鲎懇q�襵穉蝣�#H�Gam造c9&�)�慒3恿�霧厔
V�=�鄘萝T���9c�&╮�堮B~��J ZMSTТ谇X暙痦橺瓦=��鸪�?�.�2蹉Fzx凧冃�
4理:ReQ�8�!8l�>�=�?f�栯%�0敠鮿n袼窼{"漜颸芺櫢4旰敎�?枞z� 呓墖a焖&皏煨4蕫YI童&2珗朡縒�	�.�!R*�7�+趬"�鍤槺7頧��]滓aq�=+躕�<N柹婁� 腙z}/豜�c�*[W!|pXt��亟謽禔9锻腊 �欗悓説"|_n棠�2雁[酺(*氘s岊げz硄&M!暙7Γ镱絥鯳躦n*
&*�=V闺�卫�	閪@癐+猐�"B�葖�;4涩s�^^s熕m�度{P�..F骰� YJ6蓥虿�(饝蕼埌PvzL�'{擨�.乚M厾w藼撉"B�86@��?L漗M9'BW�&i�
瘖捴亟媝銃癰廃�3/�irj衍;v聗�&月倸�荂麛懖W�(鬌侵砤@�.礐re�o?凚�7(桱�甃L34*>{湵弦f式���勫倦鴠i礒� 橡辞tR0.�钴�	庤*頣郚G*�2��02�廷/�7趣靡l〇�k�%�*C�蜿
S厍�瑙q�蚺+嶖[噝鞌z拸U4娫J-0�#�0渧攐�#e)�?�o嘝腿窸��]滓ez騣>K郻k	咜军�<*氯_2鉬蹺 *Z7擫x	9z}q�=憑娪踞輚r^矍鲹�酚┗� ]"菪廰閸z觔/郫w﹂暤h塶9B{炖咚敔�#� r�:^ T_?�`滪�」耫�'�抺F傅賭UX紡^礨s2槉j�覝KmL�$o恫抶P3ⅱ蜥匱旐嚋暟%%�<g��!試塜C蕰"c怺⒆{i駽<[Y礵艪菶&h1V*禦!j腃徂谥赛廍盳�6郇�9t�Pk0膈0拷Xn�鷌�<]拃瓟�艷欕柌F窺3�俌;疓X�1結w,�%絞� 忰QH磝蔿�2#J ZMu��敎囩拨�儎KM违R恭�6�阯拷顜=T6:�:葹匘D\_墹NN~aK��A騢�8衠	辷鱾
�)礃琦l嫝�9F髃阩�.'0坑�8聯>P�:柠岃.焱x竮氙p熤 舍0g殽.4肅�)纕�.� ^"�#;� 滪)詂
濝�癴遆嶻�萄^q頟:e�4i髖 所狋�<*聰+頩鉢罦pp%�7RJV-t}Zr釺鹡δ猊嘐:筑粫�9既Ι觴4ze[o@厫�4稣m藅N费渡G�\3h垽熬坊�	�C穒d.7藽$蛬鬫棇鱃縂a�.噝疎O潍	欔�:熤w奪寪M3D�#P沔蟭�7^WQ圜揣�5麏巵垹I@}O確��<蹐艬H酌c蔨炐n�%dKKa礵顯�t�m頹俭爛儾俶絴癰�槚N7艵p,黯月4o�+薧萔*憠秾�9鷏忚粧'|�"(�扙呑∕_��C4)襉Ku��.鐜Y
瞝鐸�hxhu��敎囩拨�儎KM违R恭�6�阯拷顜=T6:�:葹匘D\_墹hl[8驫焎p盬��蕒饠�+耩f兇�5!�*駑R鴅貖i=聪泛槅})��牊菇S冟^昛荩ZゆJ 枢h.殽.@� �2慚蠥�s�.�ヴV產w沤祽@���q閗=h鴠,┋U捝娂;[祈y6镸鹶�*K8汬ZP@=t~VA2��呔�<)l烁鼝�4娔夯芕a-]w痹坺饋/醨P乜愒偊h�:w`荜+钏攭�	�#U扴*/:
�z櫱穥餌�$�v宷T院缋V�7=低v桟�袵}O�.&[赦�>珁UZg胼按�?翀乱湱]}^K臶i�7�<試�
T瀭R7橻徻ti鮩4%A�丠o+X)X�3h?�*彙燔廪/��偌�>
#搾噶:�雘�7W崕�4鎤凃皠,w�( �塏黜,弆n�:�ci�6y#�&�=庒
蝉産姌�氼崗�:;豆B∨梹墊桠-=蚕3覡黽R碶靭潣太ovc弢儜�)/:9$ '萡阨|��襮陝�8幊栭G頌�>q攍暗玱稭R篱h}KZu夼釮W篎攸缽競u緒霗@呑烷hk憮)7縚�3�痾�-X/��3溎D�
lz谺薧� H冘X|頵r�4P纏呬戒�
-令}~V葅嘸醷,a趡[AQ14(>v�>駇澭�Na只)萿耗蒖8lt\u輱巚噘p醖荀y涳壓Q�(0t(,劋2棼�	��C \P`n� }撝�'�)覮r橧|宷T院蜃+�= %@紂简ao�=n�c繽rJ3�徹挑/駦脺$>�F璒犺髏u0觐"Q餲HB誐3?j��Z術_Bxn�
O苏 6IVW蘔�緓�鰺鏙�%qD浊X鼯Lr�*蘏咵远�蠇	贔骟!j�%#�侰灬>昿t�1坣IF�(咹�&冴z閏�Qnx駴靁e
t�HF铡�o雰�驷!i��#酐}ma墄倢沎P~z%v;�蠳V%慉�$�7�沭X餹$墿趪拑�9m�*繿羛b7駣茎�8箩>P� 镅%	夓^魖鞌z拫Yd畨_V莯k騝�猯矺�c�=�/ネj擩X龞讵m誆�0�
Z艧Uz鉬1O�'P鵆帾�	祈yR�0�	�Pg_#泍ACA(ogWc釶�)汋涇�Cv毯6�	汈杺鋂FF}R>祛疕鲮w�/Kx耙k魜澽遅y=Xo五4悲�,餋8�5LSP`a薦;覎鄛勃誦�(袾q��52で廷p~灙Q篧|{U艒
�=涠G)T�8!糜�� 7|a_麋Г�8陞嬉?/�'w廥爙�茪  y篡~�柴^T贕5?a�)�.k穔{'T2U�5b?�*崻镗筵.燷�>鬋廃�3/�i0s洏z亿7��殉$X$k筜硕頂*g�>9�揙-攠k�9坔D(�,%�/踍帙{.S�*寎mce紡~釂刂昻轭 =н3哲赪�5ㄍ祦{}p�<葹遗L沿洓玐薏&IA��8�%~肍灓7�8卡勐G錑i9峹�	c�枟?聍覾》Q/�)觯X閷W偏o祡隽)抵B凄d?`幂B[镕�3��>篛0B�8縡袀o僑Al費蚈猒L州Sl�R�1[�!L埘蚌阨�緻G�蜦Ox縂{FKHY��$韺锓嘐:享龥�е���Z	g^9耔綠窿]酼7;挷x濐橔5遅_{h畧_湉烈岪�%A�2B&�`壦�7标�)逩X�8�墄y鈦斤*7宗�ziG趮�>挶(X 瞛(W饷��!	e[}黝﹂�:髷嫐隍$�'t渇軦貤峕_踪Q'�鸸=�8uPfK-與�#�5!~n�=n$�*灜镙筮:篔�(�=促�P�5cG赁B6D�鏷�1F鼍洬塣霽嬲娍[筙I1��躸欁I��
扥Ku咲�［%ak��!峘of懢咧A侒悆�3叒2蕴躊i僡�闲�鮂)@J禯�,.,6笈"5 L颸裍
�>�?�4>�憓�2奇Bq�]K芅�
J�駯n7玲梈獙Z3� 蒯葳?駩*�(忷熕溮yvP锰MT镠�;瀝�:躽7A�	0�2藬@�^驐煬P覻�	�Y蛽8�4U�1^⺪N龄皈蹵祈y7鍷�圱
&F%�?,9CcPq��:鍏勍寎I嬇禐�棖�玢@)0.&鄳訪磩D�4@N魵4侔碣�(8{}脺9钏攳�誩X擽*i#,
 �-D红�;筢�5�
鬶7�Y隆竭籼(蠵<6兯\孊澣K<E�%-V�轹:�3Ql骐粸�,鴤玄IpJv鎋驠�龂�M暑0G蔩忈n鎡!X"I菳�L孮B=sl�)=萐韼哏砰W�7銃癰啒�:舋9v1鼽+-R���;F檪紑�5鏲旇粡!n�(6�	侻忐>焢r� 抧BO4�灓JA�;領靿鑿嬥屐婥*l騫艞蛩4ヤ&&"磐踋i乧�舟嶕W)1|巕偵働fy珌8*D誻鹐{猍��踸饚
�3矒疋z�策<G�蠱B�枟?鼷覲傲u巈4�鮳X蛌豙�懐h`秧c"R訬�4厇�+W.圢i齷鳕昗_渌┍/轓�� ^终\C�;+躕?陁 燉稕�O﹥ 錖鱄&l�'(>@~{R!�鑘盖錁禔r[揸池_�(幌绘�^
!l秩�)︺|輎声6聠贲~�0&/Up喧)帪𑌑1簄Xf>Ot�?D蛼�b戚蕓�7n��嗏肼!�=%哄M薧囏G)	〨E;�鎭�2~a\橇棃�秼咆荠?"2�;�聅柮菵u怂L�v衍xy鷔
'B���K|�&i腃釞淆镄+漥�#�廀鑟g�C,1b坭7}檅��f房搋註�(狭檠.歾l奙玠讨J昤i�<儈ST�;�H�剐9tz�
�#渙n	up絺漉l鎷善�俾
僬	夺鹽S�阯&S	 ベ[�0a瞗y珌hl[3i�8礪]%�>��胉麏�"囿l灒�*]�赲,騒环H鼷�棞3�+软E洏�j籯�4_�u娶}�-2)7� �2慚蠥�s�/�姐L縸~轄J顋�!�9u镧J豂^�y�4偑酎頔 '�鞜i燹�搨?匚燄垻9�>譏扊柺�_w嗳约 �欜姀坉9}eP5F但�幤v譩P:鵁_霸矕O�S$V啌优桬�"T俇-	!:�-`菟�6觔櫂�6罇�+真1,#2R[蕡t�::引w肵嬝K8�?;F�硖e�eK3嬿化奱翋囉佐	$($�4t�蠥溪�$_匈L!鎋蒈xy鷔
'B���Y|�M腎釒射堞v���劭�7b�G;S敃磺6	k耹�烲S嘂菨蚏�邼*燤%��;晙1�艵鑠92賧=�&踍鄭QK�7鶧�J ^JP-窉�鷓鳑厍俛*J堜箴�";�-�'篙冨[2FGⅧ�.C骡>>;頫襂j梶�8�$門貝l>偅沁l瘋�e�-�>W�?蹰"芙椀�瘝G�掎嬉)刮n鬰釢o仆 晚R4J俏歓�3慮�>▂7A�T�;鳕闔^鶙謯M鮡�0�:htR翷_�g蔧 繊詽頰g吀6=觸憊顀1{�?@LZ=%LG\h鵖齭鼦鸷湇鲟rY,�u鯬忒伦� ]ooAx]縻惋K鸊4*髸\既礈@�S9B妧宋媁�"?TD ;n�p樬鎟勃趫Q?轘蒆�嗏焵U\惑�s8J糳}N�#!W擅򸖈CaZ顼ⅳ�8陸召昀'( �F縪籞￠鈟e:栘y�⑶:t齶0Bn桝P℃I弶跃宛A武�*6讗q鏠�5�"て�	S�:tN匮G骝Lr�*蘏咵远�蠇	贔骟!j�%#�侼苏碐r�矨wn��!嬸V@��(�kf	".4P缽蠐P片�儎K� B�!*#秿+髖�'\趘n�钅�K/耀5'臵蔢F%晎璫刟}�8肪<�i砍庂Q�%9|�%�5^�-蹰,纲辘乯既\8�&氖裂<撐�3啃/询h`庆l$媹T^踆塒� 揻�0a�,�/帣�7A鍧煴2揌�胅樉0鱨5`靝�,\皽棒�L寇e膖��Owa翵r-1q瀤贠滯�迄Ul镞葳�9稗骀�^pf S?↘瞩Z鸒&0挻t濃墝a�)"{%挽t发鏖梐�
q�#>+'�-v埬�襦攄�7譴T蔋�玛:%蛉.�!欁o�欉O!�(+[�咚-�3W~黛坏襥愤柬mG
K臺孯峹澡鉯c=狺8C鎚堉vB莥;4c�1�)b紏y=W#D�&c.�◥薇箝[噋�翨院*嗊n哈虾L"�	篲鮃鎖�7A仒穻�9鉫堛$w�%N�)筺輿Q�(P�狟d�8�O�2{c�&�9梌G,ug瑹鯖L力楸橨屄
勻T恭#佝p�巘@{o﹙n�钅�K/耀5'臵蔢F%晎璫刟}�8肪<�i砍庂Q�%9|�%�5^�-蹰,纲辘乯既\8�&氖裂<撐�3啃/询h`庆l$媹T^踆塒� 揻�0a�,�/帣�7<汑啺a豖侻樉0鱨5`靝�,\皽蚌�L寇e膖��Owa繨|&#G�>�呔愖�Xl燧荻 �泿Τ膆/0;%t亚蒣启P鱕"0鶠_燃l�4?M5暰r福拨�~墄F$v�v櫵��饻+锽b怘c��倱飮Vq懃�*欀q盧毶N%�)"B猴�<�<$UKh洚忭�,饙訍吂[@}O瞋�0�玔E趰2U齜挫IX荊
"P� �?~痸t$X:^� s&�-偑迩勮�j�.蜬掻�"o鍱稛V9�x倍鷝�可a气攫l�/塘檠L�F�橞祉8焎i�'筁ut�,5#廠�舅8P�-砠淜	.D3""��r�	PF"�斜G6糤7�M�.菿頍爽vc弢儜�)/:9$5-萰鯾b珹�$�?2鮂菙�)忖v煥�/J��+蝬屰X-褶瘃襇崆鏗塙T庸�!哩換乒V雕''噗]qmq鈕鄒慓攌鵮F�T�0曀{纐s院繗]鬽�0�6ntH襕A�3X雞挍茟�rt劑X6镸鹶�&K4�>
壉垭蛎滾�苈尔萷NG燋駯�河哗蒨2$2,F但鑑磙m哬U展r愨勘{�;$C谇艹黝򘎋c鰑D,�r⑽�<礁趠�#�6�0=麐忼d\粚�!tP室v峖潎/[-楥?G輊�>V,⒐咚�)龎讈棤Q}E梑!�諥宜�L帩GT魗蛢*/菮eC3q錋訮�o渰c"�*垺赓$綴朰�燄鋘,耟:i8蹲I鳋;I�靊�; 暴柆�9躓踩嵍��.俰性鯩O�祌tu�$琱蒼状_)/�8蔿�	gx ?P粷肜M跉砇貎岤Q扮鱹_竰�4鲷殬OW)9恅壺轌[ZK珌h\D�?�&D#憐��誱韼�#赀U珤�5}�"骪:誷嫄r+磋我鵭帯�蛴9冹2﹝靹'抵B 枢h.巼`m�c銳驱鶲0B�>�ヱZ畓n来笟I鴃�*襑U妥_`�4U琠<閟忎蠁鮴i數55貆搊檂.I4�r[Q](qLcd銬韞拌櫪�E|癜鏇鵨娨锎蝘2a)39桤信=鞒x耙c愬旊Q9p)垥t牪觊鈃'�i)	!:�-`萃�6綮�/肏aj��䜩岡ng铖^�7=道v匭缎G? 痡	泭�;�4	.蜥勘臫旐脑遭@gW6�<{��訡C 儚p朇邠!�%1歗譖砢QIF8薚@�Ｎ毘镄+�.鐲�3甩膚%扙0e0拷\m鮃鎖�7A仒穻�9鉫垟嵉F鑎]奙詁脷%榝d�*巋UE�T�:冔QQ�*�颬K#C>8Q灦椳[獭糁释;勪T�0;�-綥殯醽2T6r卆�"螟%f鵛� #歾�8�33鶷弪諻}惒肋G崆〢V�>�	�蹰q6赣沮鮭仭l	�蜓<騾ツ|筧鐬k兠6赧h%L在Ab鎚�綻糧�e�2�	磰j塒Eì傚f逪�㏄玢|*V蛾XM�g膷鴑s帺4+纖渙醜+}y泍Q@L`&9滱鱞睏愖�Xl磔叶 �摂嫶苉2keFh偻綝稣j蛃鍧C啡礈@�(?|x揪;绩�*餋S哱Q衽汀C�	鳙�;篑��h�7猯塷i�亸鑒`伨s焣&=灇\�嗈F8I�(:T疔焞恘v=7{腱︷�昴绒样	3�4M朮�<蓢�'~肺v�讚j~㭎n}aHG�擛菻^x1泋%�▕棲峭�>銃癰嬾�7*鞿0l爬;侊eVF�"��8铖眻璙�翮茧[{燛;╱�0晲Q�($梮�$YB�#欳�翟1j�-��J ^.<窉�鷓鳉厥僿L冾�lB�$箭Ｅ{Fw晄仢�	7�[�|�歑蟎L�炿z� 4攧沔@箙�(|�7穝	蔲詫e=皋流耂v�:命槆s鳢課忷b绋oc⿴{9V艌k騝�玪�$N.�(+�'帗@硙c蠚槺n贗��L屏[l騷=O�x繸'艝顆u叅$;疞狦� <8渟P]f2├颧W讴+嗶�(D庨齷.[�u鯬忒�熶DFSmR9珩埚P騌5'挷x濐橔5遅+}6帲鯄苣丣�>R梌<a*<� q溨�纹�這s�
;淕僿m顨凒*#在8�6 5v奩潉}% �$+W瞟唚wο扊愠偓鋐"_rw&ˊ}=耎廡奾跳�E谀H謂壡sp�%d@PL.��Z術_B~n�s�Д溄瘭v���剩�37�Wi7龙~匝
~憂�烢月懎��嫣嚩潁o�/紋厴J猈C�� 	�7/�;�7忋5虵蔕J*Zjej肉嚯	檺�肋_兒M豂��娶瘗"KD?�;讉沋^^B懜ZQsui�<餹v盇��蘵疸<�4橆暬9臑鍾K�
謀8釻姜Q轡棁F.�&鲤@壅>ニE緁鋿3吚 哇o#Y趋HX�廯� ��97A糪y鈏谂z攠|纮偡]訽�鬢L擂J4:t�;i齣崯箼�粧+�	��.v5"[�?(>?rzW\N誷袯婁毩�	wG陉莯�)琦湠騌>wtIw缯剏粽k遰邉F九繒G�	@(磩X亯楔絡�/塶CI*6蚞;GQ{鬩h萴錷UC櫂;�$嗏檧UX:�1?ゆ\硃сdl��钔=祙@:4s張ぎ�=精屮?2�S淚乶犝腖^F狺>E鍮撉{v鯣!1A��\峠m7_5^�<�灯觚岕妠燲旸Af(嶙"XS�%髏O洣�7C湵�9�┺��|��.皀槂Z�/'� 礐b8�6a玝�:垫BC苪�;�2#N_>).螐蠐V尸羿橽偻=嶏兜�)�.�"硜8[iv峞芗燽垣KA!;
驛�!8E坘�8纋d鳺賵w赲\赙P爣絏k8峲腫Z朜扔Z矫ン�p�潵^镲j*霳ɡ1趹S[懇?a殬� �簃笹��#	�宜q刐I谷豃� �;u觎h]腏F攝Y齲倹锉臙I\=鉖獀蔘.>恠\Lk+huU#m験�6鑷鰳禔j>祽骋P裁粸韂$mrzy屯�<粲t�*V乜愒偊h�oO[p秃n化戡�*鋋遟n*rEC�{斊�吡�閩B�>僪I鶏凒n馄,�7& 
熤\稲勌G#A�?=W尚��2{jK休敂�诗閿砾9O籸{擸乨輬帡倮m�&檟踞╇,聸草�)�雑'o+X)Y�<h?�$彙炖嗌>�6��兗瞘9v1鼽+-R�靔�!I缚椃�諧矋粓+q�8�7�稲H�╔de�2奟�埋JQ�矲猀M"R~hu�ds詸栉蕯峑粳#<禌傤<豎�/洫炚2{q墊皋�枚PQjCn揁諦Ft�>�5/鮩芫3�<g詺暬9膫�/8��#舝憡X;ぱ跴θz�t牊e走;緜0點釢o询h`庆l$媹TH谻苀�#鶁�,S?坁=�!箑?�3;苞啨b�漘監
,�?kM�>B埕簢黙B/��泍Uxa�&{* L#�:�汋涇�Bv褴挞8胾既�$l
!u愵往F霤30髰S唁偡g�9g4埀~�b皘OO&'�a浤�>觩�.髃W�>�=廷84牌3�'&(徰v廋撂A<P緇 P蜷�� Qe7弋忭�,饙訍吂[@}O瞋�0�蜠NT耧9^餿NB襼!V�扲q粄y=^~�X?�瑤消烷渰�!�%び�C�8lF雄_}r�:憈踍a隘椊訧湗5B��$%蒸+�|╨� ��'O�5朥�&ck��*梲{oC9#?\央豆Dヂ腈痁屼&&"耐註i怋�$馄衬s+J砎殿�以5<>鑄薡@)��	�刷;巁旂秦W◤︰t砃�9脟�6珷U�'D[u� �9!紼む诿b撝:硄鏅v低许`3悩0�#�4朷萉鮞U)�0�$從y�腿腅�盜c讚Uz鉬1O�'P鵆帾狔�祈y7鍷�圱%_j�<F\(o3\q釿�詈諟�=Z衰睎�鲉粜�-^icEl@殤�$峨A�2(E墕0��鄣l鈓lQ:漾+鐫瓢黐颻J�*MON}{葽#螜�p餌鵯C�(恦╚B郫6?锗糃^^s兡+瑄kr�	
{琅⚁^aa黯Θ�<矉咆荠?/ ﹡}榊島訩L+胰>�蜌歝Xzba桝�K渰f2bx�_�閹射鼙]亅��4權�!L�70r铐e勖yT2腃�"襰Ｆ�躃珯,j�(5�侼苏碐r� 笰IP�:�J�港0qk��(隞@)^o.懢咧A佺�>d�灙]K瞁T�;踎菿欇冨[2FFｎ�,%,6锴5$f鬛赥w�韁�')鮒瀯�.繒膑w挷�2g�賤薎径I咗薪耚膐柠严s銔瓇�-K�"＆es涕2)7縚�3�痾�-X/��3幹v峓∶腿竃�鬐c讚Uz鉬1O�'P鵊X邫簧�8UEi豈鉌蔃!p3噏SzX,s3\1�:�哆剧渂1C荃闵r嘥段�'wiuFu	儸�襞w譺MW讌%帚麦5遅v#尯&黯鶻嘰&�*S}Z;u�$蜆�偷蕔噓_蜳��5,挥爷d\粚1�<%圭M果lyI�.*J炕�<qL扑洖�荨铌鸶//�}廔恥櫬岯H啄"u蘉洘u鱭</	4B�1�@�R8{ �	!j鞢嵫叐锁璖���2{籅Cc3畲-菡1r�,躆�%1冪麻�軰矏则S.殈奙�`琈D鶸搊RI�9漎�!冪MJ��	盢\"u {緳麋|鷹圮坿*J斻獍蕀窲�$杌顜州%盰Q~z#%;蓀駂m縋��賎枹;�)牲＜N例�2m�軬#領F庱禳訟]� 咪谚欖Y圽特j兲Q-?Q磐\�v鸍�5�?�7A蝢S�3撎k豠E皽敗f豷��U沃)�4U�'_駂V窗牜覷I⒏9醔��rB&$泍Ai3\e⺗莍犙故�-偢Ｉr嘝忒�w3 X葰�嵱q遲P_毇c椻旇豑&g)曶9痄净�	�t籮^LN|�*$���>榫�宮J甉n�紹-坏霞md姷\畊Y]5ㄦJ猦葫l��钔=祙@:4脘鷰�篆鹩%!�=p濼岰浺誝C蹃u軧旍xh鮺y	%M�1�H巊G_7獴m頹后爛尳[�1��(杜�3e�@u2臧{掩%H=�,虰�!j敒�;�晏懙W糋O糐�乜`�/'�狟d�8�O� )aK�*�渜L3u2.T崵溏V诠桁檔脲;$ㄆ)于蔩c焔�藓慈|!J�5庍�Z_�<米�$巙葋�=�.惫[=�8仿�#}墾�=V�
覹/�>憤c<紿赔E5�+嵹.瘙擆_漌莸q嵥h"`钶PM胇�	梉亄�|�?�易v嶫	蠂脂a誌荈疨�暎}便�5崂Z感蓨考;)編 g豈鉌蔃!p>嘽ALY, "G�>韇幻缡萮~Q垴�;除�6Z	]u虗譑饜v躡骞c燀靠k紏k2G{炖呦﹒�F7PGrEG�u弬錼眇�$�
鬶.禲弉 ┩蓝R[铬�nvJ趻0薼睂�'?	鸢�w黦C垕咚�軤铛昝%3�\襎妜戇�'~纺?鰅国SB羃5"a��G乬Dec� �蹼冫聁���!q�Vnw％;浐_m`駾�廭'瘳棻�+鉗参毒V�N�$皗墧窮D�*綳pf瞷P�*�&勴磝蔿蔅$MWZMW.幙易鐘倮廰��0=や�骛Ft碕�)线犛y$Mz抎捲�;,=/嗦-5;躟駉f򆿈�%~肍灑;�)设�憑X;�岀	�$覾]鏣厶摕緕姮z�枸(腌傹\昣值M滤咙$m2)7繣�9豧睯�e�=�斉v嶽n覗槮g頰�� h塘Km釶E�b蠷F灜畎塶T霂:��喩鐢�%鶃謴笆乜w:抪亟粥�&e囔碀�4践牗詙>te-M儸�褓w�>/毚t濃墝l�;0IAt捡呦腴頩H�:B&�r淉�7螗莔�
鬶W�9飝u隁搂!诣畊Y]偯q�商W?I鎞B8燄�6�!F,⒐咚�)龎讈棤Q}E梑$�坔狭�i9顫M'塸,@�({DPq僣靑a獆b+N%O�6s0�-闂捻慑R�襵穉螯鷒X�.zI梯H齄!3�,虰�!g啲�烳へ嵅7A� �(縪疼5竁K�*癏x)�qU��(鐜絣�j
z$3(v潱芜M�羹涠娔堛"酏�m鑩閝尖聪{5%�.昊�fy珌lk'!�餹v盇��霿郫
� 縼琥g垾�<G�蠱B�枟?鼷覲傲u鷉La咎W偏U濿斯@圭,-J`玑hi髕�4朚晇▂7A萹S�)撈?瘄u俸J餳�<�Y庆|M蒐_焭C鰎記翡豞D疅Y[p4�馥.E%X噡[Qi3\e⺗莇犆�-,彛烒v�4灰鱾�-8{dAu"谠噐邌-�=}钞��彦Q9p)垥y牋盠*�*X-vv,&� 4讉�1瘐罖�'d� ,�苆h鍋柖R[铬�'iX佅,�蓢/[)�)"B檌齡 o9诓錅�5赀赁厾Z:�b"萬�,���w[寴K,�	凭躗9"w� �?~痸o頪m頹偲钯2�4�.裁賝x�Zi耥妻87�,虰�!j旈�譑啴E�閚�6暃g�+V��6 苖&紼�欚v@�6�隣J*Z{a5F湽异[莓�c�汬斾猡�0�.�<潣棘vc弢儜�)/:9$5鯶襂F>漨��趝驘�8祲礅|冞�5}��#蓃潧.v�5UX�5�9!紼ぺ.瘙擇_扺轄X$ 栊H绕@X�塒騎��-S/�3�除M�u细煩`躀��c彐t[覢8�;X�$1熶楠誘X馊_汪G�	豫.E%X噡[Qi3\e⺗莇犆�-,彛烒v�4灰鱾�-8{dAu"谠噐邌-�=}钞��彦Q9p)垥y牋盠*�*X-vv,&� 4讉�1瘐罖�'d� ,�苆h鍋柖R[铬�'iX佅!�涠+!C�:rΡ檍�K c(铞閲糲荸鲗厾R/v�
"蘣�)瑮昿L呫<5�&骱�2/m�=�.j穐l,9d�TI遪索蔑勔9猈�.�.感�R�Qd7髑d敲!v歜艻�a仼嚕汮蹳⒄崉
V�9�"�"槒Z�/'��' 蒪nS甤�гPW�&�疜7I:7v棽呲P鳘栾爀曵HR旺�5�菿欙太ov�砼�阔#;�8礧?F�>�$�?2孰�挩陀@揪�>q哊�#J嘽憤sa幟窘漟瓎W�!渭D赁4笟E�3①$绋BI勸d8K欦P� �8汱檟�O%諹q�/斨'縅谷/�!��0~琥sG腂O� y躖=笡越罷祈}~|鉢艫_p燲v`gB^r$!鋇鈔貉髭�,xZ髹P�4牷圔]IaC>蹂継|�zx案y楕旙A�[(瑤D厰衙峆�3C痡 3+'�1y欜�=�>韈X�3�v礗B譬盒b\箕"榬Z/J祈A犮ki�}善��4y>�畮挑T旐軑欉"	>�W産睻壶鏳f ��"Q靍剧US�((\7L.�訪a獆b+N$O�6s6�-灨�秋󀩠��7鰺慚碮O谘B礓0D�鼅�0\亸� 鷑掶秳.v�42�	�;貫6�($錋餬TD�.孏�┲#q`��"梶vk饵嬑襞梽�7诪H-Ｎ4咦芢b巐�妮擑M?MV縎�.9,6笈"5 H狧損?F馿�調釞�#◣|煯�>J�蔒)骙断7$摕�?缜u焋偤C洏u銔5�<0艒oc��=zp沆mt閥�	綢��,�ヲX產w倏]頹�!�+s頍
e玈U�)砈,瘝蓻鉻a徃5:蝫唙鹴:k碆{f`S?-�:�亟诸巇^q诃,�	婁帀鬫FVgP)憎W酅99顬Dν禎M�>G=蜄呦宄銵�(^烢1e=;�&@�疑�駅X�4�g㏕1粫窝R[笀t�gx5ㄦJ猦葫ve�|舆��/hjG锨崙�立铕嵴21�F祍�,墜�  }艠QX鹢代EO諽/.c� �?}紋y6N"C�7s&�-偑迩��翨調砯駵鐀;�(wI了N骅*\� 靦�*G帞�#靣侐粷!o�24�圗骀%�鉟� 	抲m3婤�抵3ea��!妐ly
{瑱鲻f顫邮抭鈬T椭p渷�?��抑堣M(\A縍︹�($>#笃$>	*舠鯼f	狝��觸掻(�;\晡諒偟�3V�轌5郬D滀素騤幗}� 愎\蓻哩�"曹Pㄦ+ H`桄nx鮺��s�#	�ヤV琷t卢皭A騲�!琌A忥7巘i!�t踀'礀艆鮪i叚57葁渪齟1f獺spz
I\}P1��呔い巇^q诃,�	娿『蒫2FCGs傧昁姓t蝐萎d溤皵@�^#硶]�堓試M骪j鶩n*u~}�=V闺�侠�	閗i�0r鐗岡 澡�
2 樓\��vn緕2蕧禿OqL扑洖�驶嵘悬?(�B絪籞诫鮤x+3T韉碂*`緿Xzb:岯�"jt0T7N�4Y�<暉拟筵.燷�>鬐髓�與H1h沧I鳋;H�鰕�k湵�6鬺欪瓧<f�( �	瀇挈F阓聏�$m1辬>峅�噶7`z�&�穅G3ug瑹鯚	珍羷�7讱\N9吡蔎s厈�峦堬B.MJ盽嗊�
/鎏/>(蝡韘t禲�鷠v�馄_�`匐狒w劸�(H�轏8鴂冻R庢媵貯J�阒8v6�堸d貟N&胬D`蝽pb騴鬺粿杊�0_%�8�z揗D鈻⿻H�s�&�+y耢|M蒐_焴�02嫖壙� 劌4=豮唄靮;i`秪SCQ;x}G`鹔葃澳�-Nu曛笑0胵遨銇�^9"p波�$��[鶲9*闉Bο皶<�40t2劋o姬蜻榪�d,8�;X╃�坌�鈇I襋#飋�鹳温!�=%E裺�贒4R�$;[黛��0DK|巛敂�诗閻呿LzL�) 謗筋o 犰c躖┝|�)
#({� �E峠_|o�X�灝砬筵2�#�-�X丗}r聲惶fHT�
郻�,M煆瓌�諴捨伜�$%�磂延縸Y�琀da�73朄�┬9j	�>�:�5 <{`睓溧|鼦茱瞇���﨩邵諯i窲� 炚wip抰撃�4#7柙'(!萰鵼|琎迃�,j�?绰.螷��l煵�>H�7鷔>誷櫁5傯淌轠級X?�;萨雅;缸h筁磨Q-'埓pzc-F��糮筈�n�-�4z丣 蠣禇P��c髦Wh鈣9d� S繪)茘齟e櫢6!膍実�ds\�N_Wz5^舝貽婗贯�:EA邬缑 �圌�ㄔ}"m_|y饶搖鹋k踄6 魳B杜蹄s�[~;冁T梻咨桺�8H梶E+=+�&Ⅰ� 率�9芠s�5;瓹僪|�潗� 鏊!�1 ?庚E秠m�7cn泭�#魕4uzG溯�効隶寥锈4?'籤M﹡禖灰誡_ 狴z蒒徱nh醹
6>g�!↙�woq /�'埛踮樯$玴��び�"{�:tN匮G骝;]� ��0$疃迿餷篩鲘葦*g�>9�淥終C�*歑sl�2奟�拼y怚��h{珪镬凇疩卖0擕笞臹x巄�淖據T.WV碯�G/}瑐Pbb+豵鰁m絖��#/�F驻;�3>挾塾l嫝�9F髈瘇F�蜿|h鍥炎髚伔`�棼葙?撿↙蛥k氉6痣'J镊tz鴘�磢礬�o�(僯蔬3�3;檲丘#⺪�&� n骝~G耡?y�7i褃磱醢罝T敵.騖渙齤<`�'H	hDHM7粁贠滯�迄Nv��鷾�叭�@6kkwH市攓坂^餣6<鰪S@�	\[p滄G靥楙�4�#U扴*.:
�7z毸�7虑�#誇X� ,╨瞮o鷿囈��*�6d掐�>拲}o�{捎��R[j菀晱�专簌缦&9�\��0í�$QE噥2U齜挫HX襖
#$A󺋇`玪d,=Fw�.!j�笪喥钯2�4�4包�(z�Ae-寥L-W�鹴�+F潫綃裋�毐钜)�擝/沠r�癇{e�0$朄�┬9j	�>�:�5 g']gW.呺豆4⑴��￤葖0掅"訇駊_扞�恺⒛aFE抸剻�Y珌l[G骩�m8豾璦廫L旳湿!�q暜缆拑�>e�:�3Q�衩
S匮耒�笥z紼ゎa揪Sツ2竩顟f翂^]敪 \6ㄓ)7�#�+琀僥�'V*蘐h齰訖�78睨訞�
島57�Q踇?杧嚗�痤}~V嶥�#�-rB&X攛G]t-(Z@0琟� 覠礎j>祽汒_凱忒圃蒶VbLu匘鲞z踰.訂8幏H�35;zJ杳攭�
�#u瞫
c<�q幯�;阔�(豃A�=隯�!佱雱V'邃�<68搳2�绹/[)汣2?湁燪鞽q>7.�徹洒y擃墉PvzL芠2�7�≒'~脿w�&鳒07�2ZAk扗蘆�H;\ 糥@� 柁槠)�%�8�$e�_e=觋d苻6;誨��;F氹絻� 鷑儢务B潁=======
b#e� D   E-SafeNet   LOCK            脔漺 y�3噿Н�)��R煪f                                                                                                                    蓉狿V漜�#F�
S駰吹鱏Vz恛嵻^鳂E坉聿p嬓嬌l%K内vX誒�+漑筦�'H9�-�+浰q蝅<(赧蟦腦� 騙铕~8�(|�1W雞呬浴衆淝�-鬛�*�Qya!QD@�O,諫阞箽恁峼�0繜駙�;鯁飷羋99pZs世�4孢}菤sU壦敡k�!@杮嗅�;楫鬬�	�$EC6u�1[�谕甿�;鼳梸弝o鋶庤<%藐� hU*��亷�(JC陂�=�&S{戽烦妡暽翳曑	/4�+2﹋簦乴M廴r譥蒎(1�0!a�鎌�(l焜�`�6颐^6篝�Gz�<鹊苻)炷s皏g壡勴cv\闻TQ逰�>�	噂�*?�y�/櫱q揫濝皱%憉��]趽Uz髇1~�3黶掍铫慐H<<釽癓圗r"[�7(>i=9\!��+踹蘲>蕊滠鵶渡Ⅱ觔1mwIn缤坵徂j踄染s旑帶Q�44w4槙m缢敔�)襢7�0Cu*=桼f樣�;镟瀖訵'�.痁厈鐛箩>&戧�4&徠#梄烧LqW�#;[厁�:CIo痃蛱魕疚囏茔?$�&w�憆惵�^蹗1~贜摾=鷎u2P��Z峔Vk鳵!稢埈埄钴(鬛穉鄣秅@�>{O矢\箦%U�靫�1Z䦂綃�/鏺夠符'e�6(辡朆膂F鶪D�筥6e�23簎�<槫IU�*��2#g
}g	A濔羡\垊絷呶D庿]忄�9E�(鳆~wr晅�詩藻6n�G褽A?憅�"�0(�?措u蠫=壀锹R篂�()�-鹒嘼悊'冈腜驽9z恊牊L敐p鎴0�9ㄚ5缊HC幃'|媯� 榳��$玥a烿s鏻袌5�嘿茱%�旹猆墮2�%r:鹸�6D令�-燎P^p�4�$�Uhf?憑@AQ:=>Q,��&鴼蚋慹<挼具R爌鴭怵� z4-1P晫�94�+]X楒;驭威#�p{?Zm嚏6唱�誏�9
N:*丷6堁� 卖�(肒)朇S���憭�;4抓�>t5ㄦJ猦果l\*镚E;汞唚鈠T!裿彼垞毧ObXi鄛=��3蹐�[憘R8�覝52�7z_Dn桝蒓�1&h9H�;e?�-喎涮笪+玔�.�,胆�?'�,0j卜$v澟琞�怳�萧k�+夻爳!|�62�凾簖&巆j�*宩X=�:qL廋偏Y掍U鈖膗悞烬龑�)蜉焙v陮岳檣忪-,ふ.律荕i卪�下嗭/Ka%蠸ｓ扜D\苂C乐篪�!蟞嗼曱橽綳	辷鱾
�)〾腴`櫢�%@�繱-�+葲A楍埠槂	 掜�.朿���嬁]+6闩^	|铄wh鹢�縢�>鵵
�:k醝��(燍, ;
�鑯袓u53W臟Y�x誝-繈螞郿a叧9 豬刧�i�;
i佤啾�(:�亟众�Xz噍龝�8婂牪蘬9m_|y饶搖鹋k踄 2翩&伝袝K�z=Xo蝲�g)"~琩柪j以罹蓢C顊鳙�偬�餥b�>
岴M苔呓'a伱;�>{U\c薆�$s苾歕馚8煃�?~wQ戌尝�8饜蜣]} �T�霜5o 籞菭鳾圮,_}aHk�*�)q﹒y=X8Z�&s1�瘖谬章+㏄錋耓擞瑆V鉋溅X`�篲m鮃鎖�7A亸�"贒犢柧W�
�痭至℅r�2�&x賠8�4蘾爬&bL鷘埼5 J ^`睓澉i铤汆燲少曘舓S�3跬戞\F)m�%□�-G\_廑'S�髦t8�%?F馫��軹十'�4伋熼`帴�5}�(鶿聓垎u8ヂ祓鑕彟j�澯(囼z鄭5f�6V�揃:缰爱鵜f鎷~'窽�#鵄�s�=?�#場m卂E]頔��Y_u鱦*q�'D鶦>瑠紧�崳CLs�y�偪�qg樚忓蓠噬�=�呔愖�Xl鐥�/扒混�^K_gi	仍昁姓t蝐萎d溤皵@飉."G^�:;穣�:0c.坦�>贾Kj耎攬餌鵯C�(俇�o邭囲+c钪=�'!氉w糲屟R4R�>:@筮�僪KO>钠狴襳盺TP�W� ��>�6i  }厂tX鹢代EX�<b��?~痸RiF:�7Q萀領?&�&�緖�:*慚耟:O谘B眈'x�'艵�*g嚑�6鬺麑滊Xe�3 玥蛳 �啌~x�$
�8�O�勩J@��減\3Z"3p棦势\鳂谇�詻T$押I骇寒攇媬�.)E鼙^蹶唬faz?湎((
齖袸m漰�9�'頨釤:�$厴}鹎鳭kO峴�/E垺ZUㄢ+`p!D�1?彑馦榝胶P彭X榋睡L毲J懶餺��呓4夈F縰T荗掂[﹚n蹥毆b訶�
� l潺(�?hV��3ARo@+{�-陵z	葅嘸醷/aj�8/9CijCe騋竫∨菶H矑骋
�3惄�]W_aR9�帎9馜4<髣F九纼I渇[W`炖f鯄苣丣�<P楨7YtC�皎耎仪��qW�/桱�=莫δ崧0�< ;��>�:闱~\*�1俾��
>yyG糖崚�蹱蜣a+K臶i�玘邦鑢o:3R鎥_Y蘃>mx礵飉$酑N^4N�;s"�7暚笾7篲�4�;价醟K耟:{o沧I鳋;Y�齳�!^憞�#靣侐欦4.殈k峃�9夶(瀔d�0抝_N�(圱�就;az��颿$M#,uvk簲蝥f韱诶坽篪0#侈"铝豛i乧�<嘉太v�&塔�/":/闩&(;蒩饇m盨���馄_�b虉祢z劏�4@�繮>鐵ХN旇我鵋浯>P笲��庢S昄疟L滇-$餮@`耧j@歷縒馬�"� ~�#�メP璵d荻⿻H�Q�3島53c�#R�繳 脨�毑1:蝝歷鴆15�<^eRQw5H葉諥愻嬟�@f腔+�呩仩孮Z	%t脳蚚启P鱕5莶x溤＜a�<8f(劎k按�艚q�3G慣 rEG烡8侧�在�#袵i�>琝妠s�范�2!憎�!1%哄M�当(X-楥4合��$qKk钶啝�2链囹燃<@}O磂>�燯进鏧O狴y襱T`緿XzbL.�轑a獆b+N$o�X�灝砬驯'��
侴醋�^�8Va=犍e畦#U俠�&s珑絿�/鷠圂秳!m�"2�╦似甗r�2扨:\銱H�*�?{d��/峯|u2;J尮鲜f鴱郁阞　mo雰Ц筕t昺�月噽Vs単傔�
/鳄";�j飇|1訠漑�*w�駢�.鑷w�)l�7鯾鳩妴t*づ鐁啎紼f教k龓U濿斯@$[囗k0Z油JI逰�╗晑�7H.�,�;筑�7J÷趭M鮡�0�/z戺S~醞*u�&_䅟1欢宸翫R 3��$�-rB&*�%jvTZl=g鉘阓б踞�Bp砬鷾�6谤桓蛚2ka\i蔺盨释5�z|陵$漳G�t%摓i哀瓯桺�>H縯A:�d樞�&梓�鎖I僊�9颽,改?�6&.樓b�讹a�$;S�,�8RLo鲼牑�	侏趷闈jDw鼅]竬璘荵O赀v�⑧YO蘱;
*A��^岼Jdo�"k.橭濗爛q鏴镋�$寡�H�Ge,赎n用d;H�鰀�
x瓚�肎摧惍F�'!�:馱ǜc��:瀒_I�'瑃�!忓z� �9孞]7_#A暊蘖X芙镬檔脲N絿w焻�M��抑堣M:a単趁�
A/跞3>;闌藄f)昻�#�%3頦釠�}际たH�5M�
繠觭姺u<懊鋠毞|/�?蒯3嘁7记h絞鱾z凋%'侉Q[5ā)7縌僰詅睯�e�0�#ヤj匯n来父#�!礷鸑
忺x\蜦U�5_駓1濞頰g厷\(R�0�#N癝|lkozPh鳼莀号�At皴慨r嘥畻颫PIwZ崖晑朕F阨暇I┨"嶱\zJ璞*黻崖孞�3R竲
N +�-[堉�'橼�"腳r�>剗�6A嗏膂ni澥<�1?勁j峈厄G%�?;B泗��'
B[Q遗溂�擃繖|Nj�V硉籝毨菴O+t裻耙hv蘈:4A�>�.S膁&h;痎c"�*垺镧唆婣��撌�&e�5go,镯n礴*f��!装栗�)駀忰粓F�9� 搗夘g�+V珽餬TD�/筩�7翟1jX�蔿�6$M#,uk窉�鷓鲀翦㏒蛇捳-哑�5j�.錫安醽2 +$蘘ヵ�"48输%<閖飇|1訠漑蒵K�?雌_鱩\晔ぢJ綊�=)�-阯J鑄吉N掹谑䓖尛kW�4牊e呺E昡摔@ｍ_Y艽=f崶.4�漴��!甿d歋v鈏諐0�寇訇 �怈疨寽7� w?䙌�3A碾�淙_[p�0�#=f璂pqkT]l:H踓軾鑷Д峹!惙 G�3滆伦〣]IaC/淫W�6E婋'栅蟕�� [9M朊呦崖孞�3D揘<c.*�&K浑�刈莭�6蜸r�P壅*]SJ镨{P盬揯^sムG獈额gd�	g门��/t>怦蘵彼}}1拯蚌v樓5��'#;4^鎥哥EU贙?9{�:�?h'CK:頑P譕钛Hd<�詏萷�4WwJ�%qD浊Y黯;S�齞�'W殶兵��鰪若G昋壳2��x�嗥�� 	苖nS�)蟶奴Y
瞝鐸�hxhu��敎囩拨�儎KM违R恭�6�阯拷顜=T6:�昊�$)79镌2$!詒鯾w玁��踻釐�3产炍碫t&俉⑤燱X龓{|G樈<嚜w���-痂傚S扸荩O&T旤=f晞 雤麺𪺞堬剋桾荎皤�7~曳繉P鈏�0�>h麽cG袬D�s繪)茘齟e欄@o�	��蓧琊E祪姀呈锅��>譏扊柺�Pw嘹姚+�婐�戶L\=dM垜�8珶�窝�m笲q7y�P\zJ鑵Y憦走汧�3R榏*n9 � @Ⅵ�刳�
鴔A�-
哸�*e毁一s~�3縘黉鎏E�4�"e/[)�{哌��$rpI颂棡�叨筱豳-Jv穊"��3�`鍕�椰C黈x��_yfK-�,�)g穔n6N3D�;b%�0暠6獳�6�4杰�R漌xn<灅z亿7��殉$X$k�+夻爳!|�2'�楨恹$焳~�:宺BI�|L�6衏莰Y揬媐補墕�響诙�0鐘倮廰��0=λ.伤赥e刬�勒怇T2UA趤襕ZCH姢N讗喤R塴猃稡赒罟[O觩鷤�5耨r灗�5Z��>Z�&扔?u䴓(T� T鄺�Q�篜暖誠来V｝1,疔Du潆pr鴘�筭�e�2�饿Z�杳酋6�怈牶踦~�Q_�諧<畢臎�j帳$;豩杊鎝6vl耾p13S!R蠯�覠礒:疒谆6�愹悩蝚>wew_淄�z痫M踜 群b岡厡^�	T>祻I桷姘�3濦8�5L柦奉鵉幕FX頤24� �
鬶亀疭B侪Ｉ 哜�呂b岰惰G<P�8.F泸蘣�-KO扬_,&霄崙s殽4�R赠  }耧9^餿NB誱04E��0im+]"O腛訰窦啨�+�&�紆�)/R�與|B籽T囹 D�;蘂�*i盃硝匳�剱唆掴X值J�9�|炎( 攟�'O�5朥�抵EF�"�渙n	up絺唭A橓��オ@Y癥lGe鱟齉S~�rVW�.*7/纛 %/
�廡|萞紐涚嘥搈�惦Ws}�6?wQ稪�	c鑄吉N傭适鰘毞z�=林4杆n曳Q典+%鹆_k贅�漴E�6鯜�7A�3�除M�u喜織P鴅�:� m忠Tl顊!-�*�,*请痽I髫
X}~V�3蔲阘6p稢jad,WZg駿齟∞均�cV屉鎬�掕悰锳\R,垚裃6鶹6Dkb�嗞贏�[(_妭纹鵊�
q砲N."�"f樠�'镟莭�7�$r�[�9[?盶粚s焣>ｋ\皉姐ff�&T疱�=�!VRQ茵凡�,靵蜣?>
�@��,艞�[;%h扥]歐4"{�+�$q琱mPUl�^�浦漨�遃隧瑆'塅/无9c鐘鮃�!�1J殏粴�#鐌囑惥w�髒俋驵5矻Y�箁be�2$╮�!徾&ck��*梲{z/wk瑞棞;)/訚�7!�7匬p[踿)螲咝呰[$KP葛�$灶 `7賕郋\8檜��4+霿酞!�4蓣謫燰t&�+オJ'匕DlOXmy鱺 >P笲#鲶咠I橤荼y炃=轴l"go镃�)橞昋�'W;�8�3埱@皔系繅[魚俖鳲拏7ǖw捽�<==<w深	Q竬U巚乵醡 j禜tC@,oGA`�荴栧�<pX谇鐥�8花襤j)x,L墠�;>C�茲G殘U�6�熰趜J枨軌苣丣�?R侲"F=�u墦�拮�肸w�梀媕x鶋桒-4钫9�'6剧Q�憣`緀`坋4� d[d}\1tm0�(,歼"	>� W粂籡捰腳~厶	&鎥踞ER鎙%p��\塋^tK:頑Q諳钛z;�/�"�9Hc#澷茺T棔圾 R�鷑�!I缚椃�諧矌粓+q�8�7�稲H�╔de�2朄�玖$錽鯳�h悿簏�鉀鄗鷞k6餹琽雰2蕴躊i僡�嫌дw	Mg卼搩�8(!/辋$:;蜳襖W>檍�#裭v�廁f諬~Z4� 艩)[� 伽e�0�P芙椱鮭仭l	�蛐愚4\﹙畀O=/砣Ym湗\�唓�G�7韥眪罉�c犿48弑矊F額�.� y驮Sv釶e�>痙^埘磋�梚Q呦�鎖泳�"[�?,{YZz#R襝荕м坚�'l膑醿
�呩亗鍰MEz!M缿�%睖6懧智{睪k碣�[>硰Z憴仳�x窫7O:+貰l蛼豹諌溒;培#>�佱-�,嵥m唄зVo�>?G膺�7�$Ra^盘崌�省鮼呰P|Fp銄=M烺砄]栢�[赋损�R裓94a�*�%@廞EN8o�-c�磰踺蔑
泏錋耓摔猭8噍懚�%b�	}弊�:�刷t恿麏�"鷎欙7f��"侷儡	瞹`�穜Bo�34朧�德?hq�鶹�v{hu揨f�&)z}莧∑_jp渷赱r榤�沼咡W~|巔阁�/腙
鐳蔍|�.燻儅i硡}鶤潌s]�KE掊榘I�`甕骇N庝哿鑖笉V>�贳3蟒樼H�#.蹥NF�7笣娆�-r鬾�礝�i;�=饁倰/�寇Ep膆!礷島5祚~Q蜳U�}鰌啞舳訠洜>+蝩條�O0`�.
鸲榆饵�=慏楏栜�Tr貨�0耙礁�0ga0-D攷苇/p鰐鹽费k倌G�_3▌_悶��n�E_<:�w壇�)悘趍鵯C�(�~T努д!�6"询	�溦L%�>oq勰��:l:吖咚辻擃跍$>�V漧憸�''~厂x譞墦U_譗/(`�:�K葁IxI�w=�?挑颜T�1鱚旸院砲8郋/q狈$潣zK4衋��Q'皲蓐襂�閾唆G縓I╤�$姖E�盳�9/銱H�阺胜V_j��/渧g~Y詫豆0囩婀�檱T牽Q︷�)醆�%饥箰"W)m�$藖襕X_@揎QMa6]�廡~�.鑙剗v�嶇e俈a眺謫崆鳭k%�;�3F梟扔+i﹪嘙j葀暪\桃?鄴b簎g趻NY艽;z贆G哛�;�坔鏽
3匧u�>蕰3蠪犨平?� �癘撍
(�?  閪�,^囚�匙@Xo�	�榎Oqa�'	1-#Ly�;瓏锕�0!摠肼瓥�駩ug),dM蹗裭磥5巭侵偦槻;.sBl巡+尻�;�@'�+P PO7~誖8挖�b僵�5�'襋&��6-筘协sa傻N礣c,J賯/覱褜}陑{喊裩齯WF9耽�:媽腕Qm[v穊'�詃臒�幪];塖鸵:1eCKm繼誁谸NZ1Z:頒@萐刮湬���/鴀�刷糼'穁 r;姉yTc蛖��8珉搋匳�陮滊	糋穡�'暿Z���6,�:q�*�+诖V錽�Qwwkj\揉洘	续�陨U靲p湪�9�橪毥顜=T6:�:葹匘D\_墹NN~aK��c�1縹泘i�戜z誋~硗o例燴{)�軬#領S圑惺駖劶v蚦駱f教j疵/�#鷳*茠Y敤=.艅E�瀖��"�:\.橪!�%謷g�爧淆#乀�琌D艎(�?h<�*�0^摗忤両F蛩@o�驫�SxW4�;]Rp1#KL1��2頉镯�+=渠蹀O鮩鍔�5{)xM~Q堎噐█a�6\E骡.栈樁6.tl巡+尻産�:C SC顊鳙�悘趍�|�#妟笻X尸桔泰"榬Z/J捗6�戄}陑檂醗"产膣襥鎭茞呰+[v�4+�測棆慤L挐'�退,.�_PKa臖簃$酑woV*禦W/�'埛踮樯$璍�$�%�[�/a}r聲惶ez蔮橲蜬$畛迈裋�裕_糋��;菰�U��n0賜q�>蘡掅�%﨡驡wg?c脏弥[匄屣�溣TR秃�)N�閝梏鴯")%�%焾扜[脓Q7(H狹�|�'▆�)#�幊3淜a橑灇滴竀kq貃�3羛杂i釠哣靛9z恛嶈@韬P刨F�⑿?寿-+嗤D	m溟`b鮴�絞�y6� T荗亽g�爧楌#乀廮琌D搵(�<h<�*�0^摗徼両F@9�驫薊SxW7�;]y1#KD4�:�+鯒�钀9躲��u瓆鯁磼釯P_{Y)玳℅��K鏨4!鶆P扒礀\痲
}`璞+鴞綴;鎎SPO7�^$厱�~胾��r驥厁1粣医sa杉F笻+1茠{�艑Za緕7� �7WFh划夤蝘苍邊嵓P5~鉨j��,寳�嚂Q'罭灍*e鮺y@q擽瀁�SsZ:�JL�裎喒预C�.�侴隧�!;�0r:惺Xn;遪�襌T恿�鏈臓'a�>/�楯犴"搎y�'卹RT�?m ��(邳	�;�Qw{w"傈嬎
樹����铑�5〣黰犼⒚>Kas�9咨歔GC灴l[nD�� nA�1縹泘i�戜z誋~翔仚庳疷t&抣�,E�9滋(v䴓焙�缜u焋偤C洏u銔5�<0艒MF嫬"yē.4�漨���9fF縗y韋來]x蠣硲P鬮�&�:c疸]肞W�p諴:畺�<*聹PD'��蠩Jd)�'Lyy?6�╯鞆螗�.!忇ＦS�%鍠沩�4o50PyE攽檙綔)�6@Y姡/璃蝎k阸fj&旺c弼舶癳��:C \c閤嘅耎渐趍蚢E�(渁Y原糈-�=掐�>拰Z0緕7S，� 齟WF>寒夤蚷苍邊嵓P5'鉨j淸�,屃�帣Q'罬蜔*e�(y@t與霡�VNM{ �&i薒钕叇r旴調欿鄣�U�#z_厶T瘀0I��d�*I檸瓡�(靌忦盀:^�+k峃�+暿���&x賠mL�6�邳A	�;⺗�Q!{w"厅嬎	樹���佴M铑�5�鮩犼鴻>Kap�9咨�GC柣MQ)(�8�l豤�筟O�8钒	鱩q犁幫|彸�3[�踈(鍮^國伊鰎崟紼さ勏;鶐*奋/拻RE旤=n懶�蔰�纕�#{�?醰偰&�I犌邗w�揰鳲弮B-緍U駌縜B飞娡�_[R�0竨�._hq塜wa} B@v(^襶連涷�畦	Cg嗵诳:�掕悰锳\RU0!但鑟a�3\E潞#栈樸>."l巡,尻�;骪o�*OO{b�
$蛶�*そ謢蜬?襋&�
�*e回谓'h墿N�kxJ捘e�憣}�,*O泭墄韚b垕骃灸嚋芜"	>� W粂籝亨鑓o+睐<E韙WX�4	}aH_�嘦谸JW=Fr P淪汃煭W辠酙朳凁齥'�U,n纩2瀲-T/觺袽烺8麻��槐铥H�
J跩�姖Y� 攟
蒪a\睮�Ｍ)w`��(剉gut蟒|鳂谇檟彗0'橱Q蕝�M�� ゾ褓sN5%�%诐�[\栿VQ}~��J*�2�)別j琂廂y�h仉炍V踣�=0憇�3Z�&��?u嵯�皫Uv�7潵@勏<┻�3⑿?棊>d畭v
2珗朡�糮筈�{�9�寸@砵~垃B鬛�3島5貎By�#hh癵痙^坭凹�	麩CDs稟��-N}耾SCyeu
P1��'逑攻恱i彺J� 噩稞�-*5\%t棊� 穧,x胞6佾洔L�M$禃\敃撩嘚�9Z揈3g2 	�&Q�悘�6�烼r驥�/1粣医sa傻笻+cJ茠{�艑Z7緕7� 齟WF7寒夤沘苍呲尲P5Gv鉨j��,屄�厮Q'�蜔*e鮹(}aa楴汱r�2"M{ 禦@�,兒憷笙>痁�6�8ぶ�
D�'v}r聲粭.Tc瀧��=螓婖蚃僙鰟入燵V�~�'暿���&x弡mL�6�邳	�;鱒�Qr*JP仞浳襞梹�檮Ko雰蕝�9��抑堣M(\A縗�(?<>4 -蝜鄚u���馄_�q锌蟽龔酧w9舠�/Z�&溝7!鎳播�鹉"�仴剝v*�?矆&規R良!fG菓�俼萉�6璻B.峆i� 湈/�济帲j�!礝燺逕f�#L躕�<N悑聙鴛櫌10 蝫塴雙0p {q}Dn �>竝逑緺恱iR姶O絨遛�鍖=/*0,仚�$x�6荀&來��>猰b>G8圍7寰�3.�i \_o3�.鳙耎攬餌�(萅q�>�佱櫻R[懃^�>ｋ\皉姐g?G�$*m诊�4�;hjk矧烦�-霒裸遄.1�Wˊ菮╒
D铺H;塖渾6-�(e\[9蹷��(N&�
JP萐刮灔滃K�2�	僑抓�"/鉠Kfg波s倗yTc苬��0螓姡汮僙鰧入E�
k峠�+呄F�/'��  蒪a咲�［$ad��G"u(5H櫨响m庭盱確剞湨!泜�9M醆饥估'W)m�%藖襕S_@薷QMa6]�廡W-�.�7襺v銽囩e俉e眺中崆鳭k%�;�:�孛'y瑳聵�<缜m焋倶f胶P刨F�⑿?寿-+嗤D	l潼{{肙�琀瀍�}�:�筐b蘠<汑嶅?蒑奀�]枱
`�?t ゜R�,荇�缱o�	��O0a�'Me-kVD-�2鶉Д宒!K啝柯鑕鶘坊�!ga00M狼刬壓9�&P枃髠碣�[W`陵`殑谏乗�)V扙%U��帗_棩趍�|�?�謆|灸阴oa澋d賿/覱袆aX�+c铈蟭�->骓�!媽蜖PaG>鷎o�7�<試�vy搐R8�艤52�7Xzb:x礵艪菴d U?C�!i4�1升腕盢�?�-操�E睩oT椼+傁4Q7�6�僋p铥搋匳�瞿与D鐽^玾暈Z�睧�&,�:xD�6�6舀F\C��wRnv�祪�	挟鯢湜HR欖雲�9�竚虩栓i'�5菓�$)79镌3$
;萈轔F-杍韢鑌L�披-汻}锌蟽龔癑w9舠�/Z�%认7!鑿播螾┠"�)仴規v*�?矆y趲R敶!fG攽Y0�
拀豑躌孒3d歋`鈏諐�7J烃/�� �;u觎i]覲Y�袻;繘驯衉T	+纖渙醜+},轐8/=2=#K4�鄇鄾镯寈=迁鬓O鮦鍔�5{)xN,Q堎� █a�6\E骡.栈樁6.tNl巡+尻�;骪o�"O)b�
$蛶�*嘆�'轆#飋�倱線Uq懃^�73｝Qcr�b迷�	�4CWz��䴘骃椖軐婉UaG>甮>蔈�,貤�X幷N'�厞"10Gq\�蔖V�RiF>築T設駟毠���'癒鄣�:;揼9	q狈$儑zK4蠧�"�"噪义�黗傰瓌;f�("�霴淘縋H�礚z_�0$簊�6翟1jz�
�2唌TkvZMz仞罁	些ē鯢蜑HR櫤M焊�)R�絭牼褓+C5%榮诐�[G\栿QQ}~��J)�2�)襤j琂廂y�h仉炍V踣�=o憇�3Z�&�卋$芙镜�的oW篎稚a緱z靷a換乒V叼'(噗Iy愳M[蠴�8朷檕�j9�*�4熯3�3;櫽嶕w�揰�	弮B(�#hh醞痙]坭凹�	麩s稟�.}耾yeuP1��'逑戡罞煾硰S裀忒囹�"x(13R棳����&PU殸Y幌Q�M3_紶篺�~穠<p<=� qⅡ�旅�鈑U僊�9颽=粣兏sa射K笻+dJ茠{覵艑Zf緕7� 玡WF>懂夤蝘苍邊嵓P5~鉨j��,寳�嚂Q'罭艧*e鮺y@q擽�K�5!B1V*隵pm頹孩尳[蝒�3�"彩�V�5wPb匝0~�:繨�!X伓�謃赅檠aX�瞜韘膰F闦甕霼&8舝9O�*�+蛹Z]�"隫籝Ok/!c权媷槹�詻TN羊H���<嘉太T6:�$諡匘D\}瑐=\DD�焀}糤��胹乜0�3#叇趪l灤�%2g�"鬴5觭晸b+懊腌襧槸}�崃)媸v惎説⒗g嫍NY苠8z贅哛�9��9眓
3孌u�>湌3蠪み平?� �笹撍_ �? v鑯�,^囚�硞HXo�_�榎Oxa妐PX9C=3\|籯�芴儤秇1熴馨;�婔姕銻tMn)誓�`碉J齌/辕w掝咖k�-3`4敻~聧�#�L7�*AIS6匞8挖�b钡倉�7哛n��#%ж氳>}価骉c,訋3�輴)FJΠ卙礰BJ3ⅱ蜥僽麻毧ObFu鄛=�0�忹�'
T瀺X鹢代EN諰
15A�:�O�
t1R$U�X�瀶箱茗	廽��4�Q�&gE愦W拷\;�6�僋p傀搋匳�瞿钥D鐯V玾�2潪Z睧�",�:qL�6榡舀F\@��wRgwv�閮�	协ゥ鯢释HR櫤M焊�S�)蟖安彳>'閚患燢KSP菽#%;萷㭎m瀓�#�##鼺寯�5墿谧Xè�6y�1䎱誷呄[T劬濂蟃"褄仴剣v*�?矆,趲R澕!fG纳蜭詑萉�7璻B{丳i� 蕩/�济庰6�!礝燺逕f�w?䙌�3A碾嵨窲|祈PT宬靘6|tC@,oGA`�荴栧�谏<aF饲菞�8花襤IGfC;耥礠滞5�zU汒6偒蝎o鐀fju旺c弼舶�3籃'�-SLvv菳l洅鉨宓蝍哣7蜯n�
�6-髰凇o)准R� cdV谮:�倌Gi�2)T喊裩齳KOXk�徹徂y緳嬥笟i6+K舝2�烻躲鑔u&9H鳰壷hI醹4Zw�<�/[淗^N"o�I�磳萧�羦��
�殸皐o甠0&'傁e\7�6�僋p珞搋��瞿偨D�_玾�;憺Z睧�&,�:tE�阺胜VY	罭蚾�hbhu纵焙B襞棭�屝+ ッ4邵蠱i怋�$馄衬s+J砎殿�以5<>鑄薡@)��衒麢(�;\晡諙禆礦kq躹�3�&杂i禌�湄Kc坈濏
剾j磼.�#/茠P湪=.Z檮E�瀖��"�:s橪!▇謷g哫爧契#乀�
齬6儞8�#踇M�d虽犱蕕b彯9+聏噕锽-]�rTQNPa#N釩鑯¤嬸�8tA揿鎬餼壂旁�vgaa0M览�8慈)�*@娿:审鱼"�%o*[p櫙z骀�/�q�6SXLKc~�$褣穊�5�z髃~�苂1族鑴p~灤H篧|Yp阗_�=蓽q[�{哌��
>YYg扃崌�<蚧鼷&;� o謅��R媮Mo�褍b-�4e[%�裀谸Z=Fr養 P淪跻汃湱W辠酙朳凁'�
,n妯;瀲-]#觺袾桼8Ν旈��陮溄^潁F齰夘g�+V聏�6 �9>峅�噶7`z�-�璟v_2+'ぽ焙峪彖�溣W秃Ω�)N�閝琛駦" -�%熢薌[困Q7wH狹�|�x爙�)v�幊`�\赙帠佰躻Q 抣�2]�9蹰"嵑數�钃|�涫?胥娦s縢霟q钓振h	o骀{{�f�猅躌孒3k蝄i�'蠋/榑济庻?�莀銼蹌
4穡a(齜N�,B奂梆�X\D'��蠥Gd)�.Ly-?8�╯皬螗�.w忇ＢS�%趁残�-w9 U0!但�幑b�zU汒6偰G�W6緦i讥觊�8xESC顊4啋�3ī�5�+�n�謆-衬阴la澋珸d徝/覱徻aX魋c畎漷�->忭�!诒堪@mW;��4�3蹡�y搐K�!輷:=鑇4"{�+�?k哶Bt)D�s/�眿拗	焝�.�,凳�[�/a}r聲惶uTc瀧��=螓婖蚃僙鲐入燵V�~�'暿�鍱�&x賠mL�?�邳	�;�Qw{w"傈嬎\愪燲��R薛M铑餯;�錫帮睚qn�響奒K?湎((!踧鄆\+憄��42脈丝%�川芮F▕�&V碿�3�#杂8錄�湄Kj坈濏_剾j礇"�#鷷~茠篓=.Y槃E�瀖�O�"�:
{橪!�嚡�皫跈�%瞖塒寕7�R�;�<N虽麐髐i偢#1貄璑罦m4渢]zy(oxl(n錐韓婄樭�X胼翉S裀忒待恥6,,d崓裭磤5巭@栯n位蹄v雃z"&焰+鴞颸;鎎ZPO7+躛$吥鰚蕔�荵r驥�"1粣勲sa傻N笻+2棷	�蓽_}|烜FI蕧韚[LqL扑洖�郀埙	#�7|橴籕曊蕆~誊r�【砪e
t�嘦谸R=Fr P淧褚汃暐W辠�朳凅鷎'�
,n妯?瀲-+觺�烺8烁餷�鏈欦4.殈k峃�`�盳�&/苖nS��(秹|絚�)亄`u{珷拗]鳂谇檟彗0'橱Q蕝�M�� ゾ褓sN5%�%诐�[\栿VQ}~��J*�2�)別j琂廂y�h仉炍V踣�=0憇�3Z�&��?u嵯�皫Uv�7潵@勏<┻�3⑿?棊>d畭v
2珗朡�糮筈�{�/)�#炦3�3;櫽嶕w�揰�	弮B(�#hh醞痙]坭凹�	麩s稟�.}耾yeuP1��'逑戡罞煾硰S裀忒囹�"x+13R棊�幑b�zU汒6偰G�W#w韩篺�3G慣<f53�8’臶妤�5�+�?�謆-荒阴o5澋d託/覱弻aXc畎檛�-B>觏�!媽蜖PaG>鰆>蔈�$貤貹LX幷M'�呎`�uPKaY�2雑'揹&h1V*�	c"�*垺碣$��忕�4j�6>
W俱;手`H+�/��8铉陆蚟�緩憎X[㎡J�?竕墏糄��:0憆uP鶁哻拼眘縎鶥$M
wgzY詫豆4⒌�3ΖnB羾w噻鼄瀎�偻掍\?1`墈搲�K骂H\D�?禮["�&�%攞6肰呖4羓[椴秦G酲鬦+V�"隻5藄柮:y釋摕継Gb�;嶚	趯W偏o祡鋈@瀭E庆l5T陷I �縒馶檂鮭羂)�(怜镵X䥽螝{慜��]滓ez騣>K鏵k�d嫖壌頤A繂M\*頦�鱌_b恠\Lw$yHZe騉�%桗浭�	Er剷欫�0γ货胉3FdIh缑攔鉁)抲叽p谚嵎Q�<"s("敩}�
�	z礿+�`滮�'�=镴f� r癦�r砝佮;珍
�'1!寢*�:愕RD�>.蹴��4Val麂代骃楅畔,'�>w� 噏慀臠^嵯q遬晤1(�_}aa楴�@�J1U�X縌溌柋吔P�>�.�
忯莟J鬵9	S敻+挆3i�'�擽a猝棲軲�郎嶐C
潁F齪ǜc��碙uk�7,\�;纒埋JQ�砄砢M&^63蕧��3堣僵�3DB联p渷錐R癙�簏2F9k儅喴�K珌hN~>骩薐G-妀�4�$!�溁
�%仠菨愂絖k;與骹�+輫'ｋ鸱�阜W;�.鳇G嗞s鳢誇巡@."x3J镊sO過譽迀�b�	<�#久k�A蠗棻n漒��]_v�4U琠<陁 燑甙憕b彯9+聑哷W=F?�/jQ&wZ^鳥�稼汗�!G嚽缫U�耗蒖5lfN0严�%诧m瀒硬I淊喌Q�88;zJ毲靥搳羣�{�2B&�`壦�7皑觤�$卨T�:弢=＝毕 渖5�)&1忼Q哣崝b�j裳�=�0?VJo�;鷯毋族+[f�=p濼岰栆荎u勖T7�萸hh�1u}aH-胏靑'�B1V渰%i�祴冂尛@�褁莊駵�"{�>
W椼妇\D;遪�茁剳�豧Ｐ參@筁k峃詖ǜc�/'�筜cr嘼pG�閊帙{.(�5��fnw綖�靑飭孪弐6J斻獍蕀窲�$杌太iv晐壟�4SZ衷 %/_�?禮["�&�%�"#韁甩u荊a凼たF鬒iV蒫飀�蜿r0棵κ�珔W�.亵3致<,鐽过阕屑R"屯J� �(慓�6�6"�1�%懱j��%�0�H聨m頰,(�&�5!墵榄騖D悗:�d韏錪 n�,8/=$x~@u縏鮫娪踞�sF冼柯S�4牷坣:}_L}	冼僡庵0�=}扯s旇惇&�02M!暙D烦鋝�vOS*!俍w惼�6�訹a楬w�>�佱掙1q尌1�<%哄M約ㄨc
I�.*J环魐�53張蜥辻舅垳赔	# �3`嶻亊伬�Z袗@2�厯ss鱹-]\|�
�r�v&�H淣鲎懇q�襵穉蝣�#H�Gam造c9&�)�慒3恿�霧厔
V�=�鄘萝T���9c�&╮�堮B~��J ZMSTТ谇X暙痦橺瓦=��鸪�?�.�2蹉Fzx凧冃�
4理:ReQ�8�!8l�>�=�?f�栯%�0敠鮿n袼窼{"漜颸芺櫢4旰敎�?枞z� 呓墖a焖&皏煨4蕫YI童&2珗朡縒�	�.�!R*�7�+趬"�鍤槺7頧��]滓aq�=+躕�<N柹婁� 腙z}/豜�c�*[W!|pXt��亟謽禔9锻腊 �欗悓説"|_n棠�2雁[酺(*氘s岊げz硄&M!暙7Γ镱絥鯳躦n*
&*�=V闺�卫�	閪@癐+猐�"B�葖�;4涩s�^^s熕m�度{P�..F骰� YJ6蓥虿�(饝蕼埌PvzL�'{擨�.乚M厾w藼撉"B�86@��?L漗M9'BW�&i�
瘖捴亟媝銃癰廃�3/�irj衍;v聗�&月倸�荂麛懖W�(鬌侵砤@�.礐re�o?凚�7(桱�甃L34*>{湵弦f式���勫倦鴠i礒� 橡辞tR0.�钴�	庤*頣郚G*�2��02�廷/�7趣靡l〇�k�%�*C�蜿
S厍�瑙q�蚺+嶖[噝鞌z拸U4娫J-0�#�0渧攐�#e)�?�o嘝腿窸��]滓ez騣>K郻k	咜军�<*氯_2鉬蹺 *Z7擫x	9z}q�=憑娪踞輚r^矍鲹�酚┗� ]"菪廰閸z觔/郫w﹂暤h塶9B{炖咚敔�#� r�:^ T_?�`滪�」耫�'�抺F傅賭UX紡^礨s2槉j�覝KmL�$o恫抶P3ⅱ蜥匱旐嚋暟%%�<g��!試塜C蕰"c怺⒆{i駽<[Y礵艪菶&h1V*禦!j腃徂谥赛廍盳�6郇�9t�Pk0膈0拷Xn�鷌�<]拃瓟�艷欕柌F窺3�俌;疓X�1結w,�%絞� 忰QH磝蔿�2#J ZMu��敎囩拨�儎KM违R恭�6�阯拷顜=T6:�:葹匘D\_墹NN~aK��A騢�8衠	辷鱾
�)礃琦l嫝�9F髃阩�.'0坑�8聯>P�:柠岃.焱x竮氙p熤 舍0g殽.4肅�)纕�.� ^"�#;� 滪)詂
濝�癴遆嶻�萄^q頟:e�4i髖 所狋�<*聰+頩鉢罦pp%�7RJV-t}Zr釺鹡δ猊嘐:筑粫�9既Ι觴4ze[o@厫�4稣m藅N费渡G�\3h垽熬坊�	�C穒d.7藽$蛬鬫棇鱃縂a�.噝疎O潍	欔�:熤w奪寪M3D�#P沔蟭�7^WQ圜揣�5麏巵垹I@}O確��<蹐艬H酌c蔨炐n�%dKKa礵顯�t�m頹俭爛儾俶絴癰�槚N7艵p,黯月4o�+薧萔*憠秾�9鷏忚粧'|�"(�扙呑∕_��C4)襉Ku��.鐜Y
瞝鐸�hxhu��敎囩拨�儎KM违R恭�6�阯拷顜=T6:�:葹匘D\_墹hl[8驫焎p盬��蕒饠�+耩f兇�5!�*駑R鴅貖i=聪泛槅})��牊菇S冟^昛荩ZゆJ 枢h.殽.@� �2慚蠥�s�.�ヴV產w沤祽@���q閗=h鴠,┋U捝娂;[祈y6镸鹶�*K8汬ZP@=t~VA2��呔�<)l烁鼝�4娔夯芕a-]w痹坺饋/醨P乜愒偊h�:w`荜+钏攭�	�#U扴*/:
�z櫱穥餌�$�v宷T院缋V�7=低v桟�袵}O�.&[赦�>珁UZg胼按�?翀乱湱]}^K臶i�7�<試�
T瀭R7橻徻ti鮩4%A�丠o+X)X�3h?�*彙燔廪/��偌�>
#搾噶:�雘�7W崕�4鎤凃皠,w�( �塏黜,弆n�:�ci�6y#�&�=庒
蝉産姌�氼崗�:;豆B∨梹墊桠-=蚕3覡黽R碶靭潣太ovc弢儜�)/:9$ '萡阨|��襮陝�8幊栭G頌�>q攍暗玱稭R篱h}KZu夼釮W篎攸缽競u緒霗@呑烷hk憮)7縚�3�痾�-X/��3溎D�
lz谺薧� H冘X|頵r�4P纏呬戒�
-令}~V葅嘸醷,a趡[AQ14(>v�>駇澭�Na只)萿耗蒖8lt\u輱巚噘p醖荀y涳壓Q�(0t(,劋2棼�	��C \P`n� }撝�'�)覮r橧|宷T院蜃+�= %@紂简ao�=n�c繽rJ3�徹挑/駦脺$>�F璒犺髏u0觐"Q餲HB誐3?j��Z術_Bxn�
O苏 6IVW蘔�緓�鰺鏙�%qD浊X鼯Lr�*蘏咵远�蠇	贔骟!j�%#�侰灬>昿t�1坣IF�(咹�&冴z閏�Qnx駴靁e
t�HF铡�o雰�驷!i��#酐}ma墄倢沎P~z%v;�蠳V%慉�$�7�沭X餹$墿趪拑�9m�*繿羛b7駣茎�8箩>P� 镅%	夓^魖鞌z拫Yd畨_V莯k騝�猯矺�c�=�/ネj擩X龞讵m誆�0�
Z艧Uz鉬1O�'P鵆帾�	祈yR�0�	�Pg_#泍ACA(ogWc釶�)汋涇�Cv毯6�	汈杺鋂FF}R>祛疕鲮w�/Kx耙k魜澽遅y=Xo五4悲�,餋8�5LSP`a薦;覎鄛勃誦�(袾q��52で廷p~灙Q篧|{U艒
�=涠G)T�8!糜�� 7|a_麋Г�8陞嬉?/�'w廥爙�茪  y篡~�柴^T贕5?a�)�.k穔{'T2U�5b?�*崻镗筵.燷�>鬋廃�3/�i0s洏z亿7��殉$X$k筜硕頂*g�>9�揙-攠k�9坔D(�,%�/踍帙{.S�*寎mce紡~釂刂昻轭 =н3哲赪�5ㄍ祦{}p�<葹遗L沿洓玐薏&IA��8�%~肍灓7�8卡勐G錑i9峹�	c�枟?聍覾》Q/�)觯X閷W偏o祡隽)抵B凄d?`幂B[镕�3��>篛0B�8縡袀o僑Al費蚈猒L州Sl�R�1[�!L埘蚌阨�緻G�蜦Ox縂{FKHY��$韺锓嘐:享龥�е���Z	g^9耔綠窿]酼7;挷x濐橔5遅_{h畧_湉烈岪�%A�2B&�`壦�7标�)逩X�8�墄y鈦斤*7宗�ziG趮�>挶(X 瞛(W饷��!	e[}黝﹂�:髷嫐隍$�'t渇軦貤峕_踪Q'�鸸=�8uPfK-與�#�5!~n�=n$�*灜镙筮:篔�(�=促�P�5cG赁B6D�鏷�1F鼍洬塣霽嬲娍[筙I1��躸欁I��
扥Ku咲�［%ak��!峘of懢咧A侒悆�3叒2蕴躊i僡�闲�鮂)@J禯�,.,6笈"5 L颸裍
�>�?�4>�憓�2奇Bq�]K芅�
J�駯n7玲梈獙Z3� 蒯葳?駩*�(忷熕溮yvP锰MT镠�;瀝�:躽7A�	0�2藬@�^驐煬P覻�	�Y蛽8�4U�1^⺪N龄皈蹵祈y7鍷�圱
&F%�?,9CcPq��:鍏勍寎I嬇禐�棖�玢@)0.&鄳訪磩D�4@N魵4侔碣�(8{}脺9钏攳�誩X擽*i#,
 �-D红�;筢�5�
鬶7�Y隆竭籼(蠵<6兯\孊澣K<E�%-V�轹:�3Ql骐粸�,鴤玄IpJv鎋驠�龂�M暑0G蔩忈n鎡!X"I菳�L孮B=sl�)=萐韼哏砰W�7銃癰啒�:舋9v1鼽+-R���;F檪紑�5鏲旇粡!n�(6�	侻忐>焢r� 抧BO4�灓JA�;領靿鑿嬥屐婥*l騫艞蛩4ヤ&&"磐踋i乧�舟嶕W)1|巕偵働fy珌8*D誻鹐{猍��踸饚
�3矒疋z�策<G�蠱B�枟?鼷覲傲u巈4�鮳X蛌豙�懐h`秧c"R訬�4厇�+W.圢i齷鳕昗_渌┍/轓�� ^终\C�;+躕?陁 燉稕�O﹥ 錖鱄&l�'(>@~{R!�鑘盖錁禔r[揸池_�(幌绘�^
!l秩�)︺|輎声6聠贲~�0&/Up喧)帪𑌑1簄Xf>Ot�?D蛼�b戚蕓�7n��嗏肼!�=%哄M薧囏G)	〨E;�鎭�2~a\橇棃�秼咆荠?"2�;�聅柮菵u怂L�v衍xy鷔
'B���K|�&i腃釞淆镄+漥�#�廀鑟g�C,1b坭7}檅��f房搋註�(狭檠.歾l奙玠讨J昤i�<儈ST�;�H�剐9tz�
�#渙n	up絺漉l鎷善�俾
僬	夺鹽S�阯&S	 ベ[�0a瞗y珌hl[3i�8礪]%�>��胉麏�"囿l灒�*]�赲,騒环H鼷�棞3�+软E洏�j籯�4_�u娶}�-2)7� �2慚蠥�s�/�姐L縸~轄J顋�!�9u镧J豂^�y�4偑酎頔 '�鞜i燹�搨?匚燄垻9�>譏扊柺�_w嗳约 �欜姀坉9}eP5F但�幤v譩P:鵁_霸矕O�S$V啌优桬�"T俇-	!:�-`菟�6觔櫂�6罇�+真1,#2R[蕡t�::引w肵嬝K8�?;F�硖e�eK3嬿化奱翋囉佐	$($�4t�蠥溪�$_匈L!鎋蒈xy鷔
'B���Y|�M腎釒射堞v���劭�7b�G;S敃磺6	k耹�烲S嘂菨蚏�邼*燤%��;晙1�艵鑠92賧=�&踍鄭QK�7鶧�J ^JP-窉�鷓鳑厍俛*J堜箴�";�-�'篙冨[2FGⅧ�.C骡>>;頫襂j梶�8�$門貝l>偅沁l瘋�e�-�>W�?蹰"芙椀�瘝G�掎嬉)刮n鬰釢o仆 晚R4J俏歓�3慮�>▂7A�T�;鳕闔^鶙謯M鮡�0�:htR翷_�g蔧 繊詽頰g吀6=觸憊顀1{�?@LZ=%LG\h鵖齭鼦鸷湇鲟rY,�u鯬忒伦� ]ooAx]縻惋K鸊4*髸\既礈@�S9B妧宋媁�"?TD ;n�p樬鎟勃趫Q?轘蒆�嗏焵U\惑�s8J糳}N�#!W擅򸖈CaZ顼ⅳ�8陸召昀'( �F縪籞￠鈟e:栘y�⑶:t齶0Bn桝P℃I弶跃宛A武�*6讗q鏠�5�"て�	S�:tN匮G骝Lr�*蘏咵远�蠇	贔骟!j�%#�侼苏碐r�矨wn��!嬸V@��(�kf	".4P缽蠐P片�儎K� B�!*#秿+髖�'\趘n�钅�K/耀5'臵蔢F%晎璫刟}�8肪<�i砍庂Q�%9|�%�5^�-蹰,纲辘乯既\8�&氖裂<撐�3啃/询h`庆l$媹T^踆塒� 揻�0a�,�/帣�7A鍧煴2揌�胅樉0鱨5`靝�,\皽棒�L寇e膖��Owa翵r-1q瀤贠滯�迄Ul镞葳�9稗骀�^pf S?↘瞩Z鸒&0挻t濃墝a�)"{%挽t发鏖梐�
q�#>+'�-v埬�襦攄�7譴T蔋�玛:%蛉.�!欁o�欉O!�(+[�咚-�3W~黛坏襥愤柬mG
K臺孯峹澡鉯c=狺8C鎚堉vB莥;4c�1�)b紏y=W#D�&c.�◥薇箝[噋�翨院*嗊n哈虾L"�	篲鮃鎖�7A仒穻�9鉫堛$w�%N�)筺輿Q�(P�狟d�8�O�2{c�&�9梌G,ug瑹鯖L力楸橨屄
勻T恭#佝p�巘@{o﹙n�钅�K/耀5'臵蔢F%晎璫刟}�8肪<�i砍庂Q�%9|�%�5^�-蹰,纲辘乯既\8�&氖裂<撐�3啃/询h`庆l$媹T^踆塒� 揻�0a�,�/帣�7<汑啺a豖侻樉0鱨5`靝�,\皽蚌�L寇e膖��Owa繨|&#G�>�呔愖�Xl燧荻 �泿Τ膆/0;%t亚蒣启P鱕"0鶠_燃l�4?M5暰r福拨�~墄F$v�v櫵��饻+锽b怘c��倱飮Vq懃�*欀q盧毶N%�)"B猴�<�<$UKh洚忭�,饙訍吂[@}O瞋�0�玔E趰2U齜挫IX荊
"P� �?~痸t$X:^� s&�-偑迩勮�j�.蜬掻�"o鍱稛V9�x倍鷝�可a气攫l�/塘檠L�F�橞祉8焎i�'筁ut�,5#廠�舅8P�-砠淜	.D3""��r�	PF"�斜G6糤7�M�.菿頍爽vc弢儜�)/:9$5-萰鯾b珹�$�?2鮂菙�)忖v煥�/J��+蝬屰X-褶瘃襇崆鏗塙T庸�!哩換乒V雕''噗]qmq鈕鄒慓攌鵮F�T�0曀{纐s院繗]鬽�0�6ntH襕A�3X雞挍茟�rt劑X6镸鹶�&K4�>
壉垭蛎滾�苈尔萷NG燋駯�河哗蒨2$2,F但鑑磙m哬U展r愨勘{�;$C谇艹黝򘎋c鰑D,�r⑽�<礁趠�#�6�0=麐忼d\粚�!tP室v峖潎/[-楥?G輊�>V,⒐咚�)龎讈棤Q}E梑!�諥宜�L帩GT魗蛢*/菮eC3q錋訮�o渰c"�*垺赓$綴朰�燄鋘,耟:i8蹲I鳋;I�靊�; 暴柆�9躓踩嵍��.俰性鯩O�祌tu�$琱蒼状_)/�8蔿�	gx ?P粷肜M跉砇貎岤Q扮鱹_竰�4鲷殬OW)9恅壺轌[ZK珌h\D�?�&D#憐��誱韼�#赀U珤�5}�"骪:誷嫄r+磋我鵭帯�蛴9冹2﹝靹'抵B 枢h.巼`m�c銳驱鶲0B�>�ヱZ畓n来笟I鴃�*襑U妥_`�4U琠<閟忎蠁鮴i數55貆搊檂.I4�r[Q](qLcd銬韞拌櫪�E|癜鏇鵨娨锎蝘2a)39桤信=鞒x耙c愬旊Q9p)垥t牪觊鈃'�i)	!:�-`萃�6綮�/肏aj��䜩岡ng铖^�7=道v匭缎G? 痡	泭�;�4	.蜥勘臫旐脑遭@gW6�<{��訡C 儚p朇邠!�%1歗譖砢QIF8薚@�Ｎ毘镄+�.鐲�3甩膚%扙0e0拷\m鮃鎖�7A仒穻�9鉫垟嵉F鑎]奙詁脷%榝d�*巋UE�T�:冔QQ�*�颬K#C>8Q灦椳[獭糁释;勪T�0;�-綥殯醽2T6r卆�"螟%f鵛� #歾�8�33鶷弪諻}惒肋G崆〢V�>�	�蹰q6赣沮鮭仭l	�蜓<騾ツ|筧鐬k兠6赧h%L在Ab鎚�綻糧�e�2�	磰j塒Eì傚f逪�㏄玢|*V蛾XM�g膷鴑s帺4+纖渙醜+}y泍Q@L`&9滱鱞睏愖�Xl磔叶 �摂嫶苉2keFh偻綝稣j蛃鍧C啡礈@�(?|x揪;绩�*餋S哱Q衽汀C�	鳙�;篑��h�7猯塷i�亸鑒`伨s焣&=灇\�嗈F8I�(:T疔焞恘v=7{腱︷�昴绒样	3�4M朮�<蓢�'~肺v�讚j~㭎n}aHG�擛菻^x1泋%�▕棲峭�>銃癰嬾�7*鞿0l爬;侊eVF�"��8铖眻璙�翮茧[{燛;╱�0晲Q�($梮�$YB�#欳�翟1j�-��J ^.<窉�鷓鳉厥僿L冾�lB�$箭Ｅ{Fw晄仢�	7�[�|�歑蟎L�炿z� 4攧沔@箙�(|�7穝	蔲詫e=皋流耂v�:命槆s鳢課忷b绋oc⿴{9V艌k騝�玪�$N.�(+�'帗@硙c蠚槺n贗��L屏[l騷=O�x繸'艝顆u叅$;疞狦� <8渟P]f2├颧W讴+嗶�(D庨齷.[�u鯬忒�熶DFSmR9珩埚P騌5'挷x濐橔5遅+}6帲鯄苣丣�>R梌<a*<� q溨�纹�這s�
;淕僿m顨凒*#在8�6 5v奩潉}% �$+W瞟唚wο扊愠偓鋐"_rw&ˊ}=耎廡奾跳�E谀H謂壡sp�%d@PL.��Z術_B~n�s�Д溄瘭v���剩�37�Wi7龙~匝
~憂�烢月懎��嫣嚩潁o�/紋厴J猈C�� 	�7/�;�7忋5虵蔕J*Zjej肉嚯	檺�肋_兒M豂��娶瘗"KD?�;讉沋^^B懜ZQsui�<餹v盇��蘵疸<�4橆暬9臑鍾K�
謀8釻姜Q轡棁F.�&鲤@壅>ニE緁鋿3吚 哇o#Y趋HX�廯� ��97A糪y鈏谂z攠|纮偡]訽�鬢L擂J4:t�;i齣崯箼�粧+�	��.v5"[�?(>?rzW\N誷袯婁毩�	wG陉莯�)琦湠騌>wtIw缯剏粽k遰邉F九繒G�	@(磩X亯楔絡�/塶CI*6蚞;GQ{鬩h萴錷UC櫂;�$嗏檧UX:�1?ゆ\硃сdl��钔=祙@:4s張ぎ�=精屮?2�S淚乶犝腖^F狺>E鍮撉{v鯣!1A��\峠m7_5^�<�灯觚岕妠燲旸Af(嶙"XS�%髏O洣�7C湵�9�┺��|��.皀槂Z�/'� 礐b8�6a玝�:垫BC苪�;�2#N_>).螐蠐V尸羿橽偻=嶏兜�)�.�"硜8[iv峞芗燽垣KA!;
驛�!8E坘�8纋d鳺賵w赲\赙P爣絏k8峲腫Z朜扔Z矫ン�p�潵^镲j*霳ɡ1趹S[懇?a殬� �簃笹��#	�宜q刐I谷豃� �;u觎h]腏F攝Y齲倹锉臙I\=鉖獀蔘.>恠\Lk+huU#m験�6鑷鰳禔j>祽骋P裁粸韂$mrzy屯�<粲t�*V乜愒偊h�oO[p秃n化戡�*鋋遟n*rEC�{斊�吡�閩B�>僪I鶏凒n馄,�7& 
熤\稲勌G#A�?=W尚��2{jK休敂�诗閿砾9O籸{擸乨輬帡倮m�&檟踞╇,聸草�)�雑'o+X)Y�<h?�$彙炖嗌>�6��兗瞘9v1鼽+-R�靔�!I缚椃�諧矋粓+q�8�7�稲H�╔de�2奟�埋JQ�矲猀M"R~hu�ds詸栉蕯峑粳#<禌傤<豎�/洫炚2{q墊皋�枚PQjCn揁諦Ft�>�5/鮩芫3�<g詺暬9膫�/8��#舝憡X;ぱ跴θz�t牊e走;緜0點釢o询h`庆l$媹TH谻苀�#鶁�,S?坁=�!箑?�3;苞啨b�漘監
,�?kM�>B埕簢黙B/��泍Uxa�&{* L#�:�汋涇�Bv褴挞8胾既�$l
!u愵往F霤30髰S唁偡g�9g4埀~�b皘OO&'�a浤�>觩�.髃W�>�=廷84牌3�'&(徰v廋撂A<P緇 P蜷�� Qe7弋忭�,饙訍吂[@}O瞋�0�蜠NT耧9^餿NB襼!V�扲q粄y=^~�X?�瑤消烷渰�!�%び�C�8lF雄_}r�:憈踍a隘椊訧湗5B��$%蒸+�|╨� ��'O�5朥�&ck��*梲{oC9#?\央豆Dヂ腈痁屼&&"耐註i怋�$馄衬s+J砎殿�以5<>鑄薡@)��	�刷;巁旂秦W◤︰t砃�9脟�6珷U�'D[u� �9!紼む诿b撝:硄鏅v低许`3悩0�#�4朷萉鮞U)�0�$從y�腿腅�盜c讚Uz鉬1O�'P鵆帾狔�祈y7鍷�圱%_j�<F\(o3\q釿�詈諟�=Z衰睎�鲉粜�-^icEl@殤�$峨A�2(E墕0��鄣l鈓lQ:漾+鐫瓢黐颻J�*MON}{葽#螜�p餌鵯C�(恦╚B郫6?锗糃^^s兡+瑄kr�	
{琅⚁^aa黯Θ�<矉咆荠?/ ﹡}榊島訩L+胰>�蜌歝Xzba桝�K渰f2bx�_�閹射鼙]亅��4權�!L�70r铐e勖yT2腃�"襰Ｆ�躃珯,j�(5�侼苏碐r� 笰IP�:�J�港0qk��(隞@)^o.懢咧A佺�>d�灙]K瞁T�;踎菿欇冨[2FFｎ�,%,6锴5$f鬛赥w�韁�')鮒瀯�.繒膑w挷�2g�賤薎径I咗薪耚膐柠严s銔瓇�-K�"＆es涕2)7縚�3�痾�-X/��3幹v峓∶腿竃�鬐c讚Uz鉬1O�'P鵊X邫簧�8UEi豈鉌蔃!p3噏SzX,s3\1�:�哆剧渂1C荃闵r嘥段�'wiuFu	儸�襞w譺MW讌%帚麦5遅v#尯&黯鶻嘰&�*S}Z;u�$蜆�偷蕔噓_蜳��5,挥爷d\粚1�<%圭M果lyI�.*J炕�<qL扑洖�荨铌鸶//�}廔恥櫬岯H啄"u蘉洘u鱭</	4B�1�@�R8{ �	!j鞢嵫叐锁璖���2{籅Cc3畲-菡1r�,躆�%1冪麻�軰矏则S.殈奙�`琈D鶸搊RI�9漎�!冪MJ��	盢\"u {緳麋|鷹圮坿*J斻獍蕀窲�$杌顜州%盰Q~z#%;蓀駂m縋��賎枹;�)牲＜N例�2m�軬#領F庱禳訟]� 咪谚欖Y圽特j兲Q-?Q磐\�v鸍�5�?�7A蝢S�3撎k豠E皽敗f豷��U沃)�4U�'_駂V窗牜覷I⒏9醔��rB&$泍Ai3\e⺗莍犙故�-偢Ｉr嘝忒�w3 X葰�嵱q遲P_毇c椻旇豑&g)曶9痄净�	�t籮^LN|�*$���>榫�宮J甉n�紹-坏霞md姷\畊Y]5ㄦJ猦葫l��钔=祙@:4脘鷰�篆鹩%!�=p濼岰浺誝C蹃u軧旍xh鮺y	%M�1�H巊G_7獴m頹后爛尳[�1��(杜�3e�@u2臧{掩%H=�,虰�!j敒�;�晏懙W糋O糐�乜`�/'�狟d�8�O� )aK�*�渜L3u2.T崵溏V诠桁檔脲;$ㄆ)于蔩c焔�藓慈|!J�5庍�Z_�<米�$巙葋�=�.惫[=�8仿�#}墾�=V�
覹/�>憤c<紿赔E5�+嵹.瘙擆_漌莸q嵥h"`钶PM胇�	梉亄�|�?�易v嶫	蠂脂a誌荈疨�暎}便�5崂Z感蓨考;)編 g豈鉌蔃!p>嘽ALY, "G�>韇幻缡萮~Q垴�;除�6Z	]u虗譑饜v躡骞c燀靠k紏k2G{炖呦﹒�F7PGrEG�u弬錼眇�$�
鬶.禲弉 ┩蓝R[铬�nvJ趻0薼睂�'?	鸢�w黦C垕咚�軤铛昝%3�\襎妜戇�'~纺?鰅国SB羃5"a��G乬Dec� �蹼冫聁���!q�Vnw％;浐_m`駾�廭'瘳棻�+鉗参毒V�N�$皗墧窮D�*綳pf瞷P�*�&勴磝蔿蔅$MWZMW.幙易鐘倮廰��0=や�骛Ft碕�)线犛y$Mz抎捲�;,=/嗦-5;躟駉f򆿈�%~肍灑;�)设�憑X;�岀	�$覾]鏣厶摕緕姮z�枸(腌傹\昣值M滤咙$m2)7繣�9豧睯�e�=�斉v嶽n覗槮g頰�� h塘Km釶E�b蠷F灜畎塶T霂:��喩鐢�%鶃謴笆乜w:抪亟粥�&e囔碀�4践牗詙>te-M儸�褓w�>/毚t濃墝l�;0IAt捡呦腴頩H�:B&�r淉�7螗莔�
鬶W�9飝u隁搂!诣畊Y]偯q�商W?I鎞B8燄�6�!F,⒐咚�)龎讈棤Q}E梑$�坔狭�i9顫M'塸,@�({DPq僣靑a獆b+N%O�6s0�-闂捻慑R�襵穉螯鷒X�.zI梯H齄!3�,虰�!g啲�烳へ嵅7A� �(縪疼5竁K�*癏x)�qU��(鐜絣�j
z$3(v潱芜M�羹涠娔堛"酏�m鑩閝尖聪{5%�.昊�fy珌lk'!�餹v盇��霿郫
� 縼琥g垾�<G�蠱B�枟?鼷覲傲u鷉La咎W偏U濿斯@圭,-J`玑hi髕�4朚晇▂7A萹S�)撈?瘄u俸J餳�<�Y庆|M蒐_焭C鰎記翡豞D疅Y[p4�馥.E%X噡[Qi3\e⺗莇犆�-,彛烒v�4灰鱾�-8{dAu"谠噐邌-�=}钞��彦Q9p)垥y牋盠*�*X-vv,&� 4讉�1瘐罖�'d� ,�苆h鍋柖R[铬�'iX佅,�蓢/[)�)"B檌齡 o9诓錅�5赀赁厾Z:�b"萬�,���w[寴K,�	凭躗9"w� �?~痸o頪m頹偲钯2�4�.裁賝x�Zi耥妻87�,虰�!j旈�譑啴E�閚�6暃g�+V��6 苖&紼�欚v@�6�隣J*Z{a5F湽异[莓�c�汬斾猡�0�.�<潣棘vc弢儜�)/:9$5鯶襂F>漨��趝驘�8祲礅|冞�5}��#蓃潧.v�5UX�5�9!紼ぺ.瘙擇_扺轄X$ 栊H绕@X�塒騎��-S/�3�除M�u细煩`躀��c彐t[覢8�;X�$1熶楠誘X馊_汪G�	豫.E%X噡[Qi3\e⺗莇犆�-,彛烒v�4灰鱾�-8{dAu"谠噐邌-�=}钞��彦Q9p)垥y牋盠*�*X-vv,&� 4讉�1瘐罖�'d� ,�苆h鍋柖R[铬�'iX佅!�涠+!C�:rΡ檍�K c(铞閲糲荸鲗厾R/v�
"蘣�)瑮昿L呫<5�&骱�2/m�=�.j穐l,9d�TI遪索蔑勔9猈�.�.感�R�Qd7髑d敲!v歜艻�a仼嚕汮蹳⒄崉
V�9�"�"槒Z�/'��' 蒪nS甤�гPW�&�疜7I:7v棽呲P鳘栾爀曵HR旺�5�菿欙太ov�砼�阔#;�8礧?F�>�$�?2孰�挩陀@揪�>q哊�#J嘽憤sa幟窘漟瓎W�!渭D赁4笟E�3①$绋BI勸d8K欦P� �8汱檟�O%諹q�/斨'縅谷/�!��0~琥sG腂O� y躖=笡越罷祈}~|鉢艫_p燲v`gB^r$!鋇鈔貉髭�,xZ髹P�4牷圔]IaC>蹂継|�zx案y楕旙A�[(瑤D厰衙峆�3C痡 3+'�1y欜�=�>韈X�3�v礗B譬盒b\箕"榬Z/J祈A犮ki�}善��4y>�畮挑T旐軑欉"	>�W産睻壶鏳f ��"Q靍剧US�((\7L.�訪a獆b+N$O�6s6�-灨�秋󀩠��7鰺慚碮O谘B礓0D�鼅�0\亸� 鷑掶秳.v�42�	�;貫6�($錋餬TD�.孏�┲#q`��"梶vk饵嬑襞梽�7诪H-Ｎ4咦芢b巐�妮擑M?MV縎�.9,6笈"5 H狧損?F馿�調釞�#◣|煯�>J�蔒)骙断7$摕�?缜u焋偤C洏u銔5�<0艒oc��=zp沆mt閥�	綢��,�ヲX產w倏]頹�!�+s頍
e玈U�)砈,瘝蓻鉻a徃5:蝫唙鹴:k碆{f`S?-�:�亟诸巇^q诃,�	婁帀鬫FVgP)憎W酅99顬Dν禎M�>G=蜄呦宄銵�(^烢1e=;�&@�疑�駅X�4�g㏕1粫窝R[笀t�gx5ㄦJ猦葫ve�|舆��/hjG锨崙�立铕嵴21�F祍�,墜�  }艠QX鹢代EO諽/.c� �?}紋y6N"C�7s&�-偑迩��翨調砯駵鐀;�(wI了N骅*\� 靦�*G帞�#靣侐粷!o�24�圗骀%�鉟� 	抲m3婤�抵3ea��!妐ly
{瑱鲻f顫邮抭鈬T椭p渷�?��抑堣M(\A縍︹�($>#笃$>	*舠鯼f	狝��觸掻(�;\晡諒偟�3V�轌5郬D滀素騤幗}� 愎\蓻哩�"曹Pㄦ+ H`桄nx鮺��s�#	�ヤV琷t卢皭A騲�!琌A忥7巘i!�t踀'礀艆鮪i叚57葁渪齟1f獺spz
I\}P1��呔い巇^q诃,�	娿『蒫2FCGs傧昁姓t蝐萎d溤皵@�^#硶]�堓試M骪j鶩n*u~}�=V闺�侠�	閗i�0r鐗岡 澡�
2 樓\��vn緕2蕧禿OqL扑洖�驶嵘悬?(�B絪籞诫鮤x+3T韉碂*`緿Xzb:岯�"jt0T7N�4Y�<暉拟筵.燷�>鬐髓�與H1h沧I鳋;H�鰕�k湵�6鬺欪瓧<f�( �	瀇挈F阓聏�$m1辬>峅�噶7`z�&�穅G3ug瑹鯚	珍羷�7讱\N9吡蔎s厈�峦堬B.MJ盽嗊�
/鎏/>(蝡韘t禲�鷠v�馄_�`匐狒w劸�(H�轏8鴂冻R庢媵貯J�阒8v6�堸d貟N&胬D`蝽pb騴鬺粿杊�0_%�8�z揗D鈻⿻H�s�&�+y耢|M蒐_焴�02嫖壙� 劌4=豮唄靮;i`秪SCQ;x}G`鹔葃澳�-Nu曛笑0胵遨銇�^9"p波�$��[鶲9*闉Bο皶<�40t2劋o姬蜻榪�d,8�;X╃�坌�鈇I襋#飋�鹳温!�=%E裺�贒4R�$;[黛��0DK|巛敂�诗閻呿LzL�) 謗筋o 犰c躖┝|�)
#({� �E峠_|o�X�灝砬筵2�#�-�X丗}r聲惶fHT�
郻�,M煆瓌�諴捨伜�$%�磂延縸Y�琀da�73朄�┬9j	�>�:�5 <{`睓溧|鼦茱瞇���﨩邵諯i窲� 炚wip抰撃�4#7柙'(!萰鵼|琎迃�,j�?绰.螷��l煵�>H�7鷔>誷櫁5傯淌轠級X?�;萨雅;缸h筁磨Q-'埓pzc-F��糮筈�n�-�4z丣 蠣禇P��c髦Wh鈣9d� S繪)茘齟e櫢6!膍実�ds\�N_Wz5^舝貽婗贯�:EA邬缑 �圌�ㄔ}"m_|y饶搖鹋k踄6 魳B杜蹄s�[~;冁T梻咨桺�8H梶E+=+�&Ⅰ� 率�9芠s�5;瓹僪|�潗� 鏊!�1 ?庚E秠m�7cn泭�#魕4uzG溯�効隶寥锈4?'籤M﹡禖灰誡_ 狴z蒒徱nh醹
6>g�!↙�woq /�'埛踮樯$玴��び�"{�:tN匮G骝;]� ��0$疃迿餷篩鲘葦*g�>9�淥終C�*歑sl�2奟�拼y怚��h{珪镬凇疩卖0擕笞臹x巄�淖據T.WV碯�G/}瑐Pbb+豵鰁m絖��#/�F驻;�3>挾塾l嫝�9F髈瘇F�蜿|h鍥炎髚伔`�棼葙?撿↙蛥k氉6痣'J镊tz鴘�磢礬�o�(僯蔬3�3;檲丘#⺪�&� n骝~G耡?y�7i褃磱醢罝T敵.騖渙齤<`�'H	hDHM7粁贠滯�迄Nv��鷾�叭�@6kkwH市攓坂^餣6<鰪S@�	\[p滄G靥楙�4�#U扴*.:
�7z毸�7虑�#誇X� ,╨瞮o鷿囈��*�6d掐�>拲}o�{捎��R[j菀晱�专簌缦&9�\��0í�$QE噥2U齜挫HX襖
#$A󺋇`玪d,=Fw�.!j�笪喥钯2�4�4包�(z�Ae-寥L-W�鹴�+F潫綃裋�毐钜)�擝/沠r�癇{e�0$朄�┬9j	�>�:�5 g']gW.呺豆4⑴��￤葖0掅"訇駊_扞�恺⒛aFE抸剻�Y珌l[G骩�m8豾璦廫L旳湿!�q暜缆拑�>e�:�3Q�駞h+駸ギ�赔:!紼覠>┪{�8┨+趮oc瓖v[5ā-K釱�6糒渙鴍{匲b繪螳b�48櫀毝j�&秄鹯63�Q�7Z㧐S隰���<*骂yU�0�#�-']y�*]u,(ZW*�>�芫秌xU埤跻_�橀嫎�NK_|S$黪�4す�y|巢p涯偡g�;v'2帺~Φ砰�-�	y瞈NVBD韠糍耎攲驞鵏c�甒秇r鑽扊8飓-�7劻+�冶(X)汣9f黧��9N?膊骃楉葷j@}e�<v揫�孜荌O瀺0H鬱滚E^襃
>"w� 艪'�2"kc餤c�
ń琼峽���"y�,Fn=窌痪\m`駾�"G集洭�譺从嚲P�;�竐留碅鱘� 	郖H
漡�8E\祌鱒�$M#^NSY踮豆挺�3咃定谾{瀈�佑據Z4WQ�5昊bz溃.5'賆踻@#泏�"�8瞐邾1�?冾＜:宁�V碕�
%舝憡D4电禚訮粵h3韆旭/⒘2�:过惬k`倚l%T逋H\�僲� �婯3B�T荎饋z嶼X鳋�瞖塿蘧0�/U豙_�,U嫖壒�;]祈}~R���Ub{�=c79V+��!�楓謺hR|锪粱8�	貂洂蒼%veDy逃巣碛j�,Z_愸S废罍H�^2j肃1�>>>>>>> .r472
