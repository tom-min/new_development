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
	OBDII_ENGINE_SPEED_PGN=0x00F004,//¶ÁÈ¡·¢¶¯»ú×ªËÙ
	
	OBDII_BATTRY_VOLTAGE_PGN=0x00FEF7,//¶ÁÈ¡·¢¶¯»ú×ªËÙ

	OBDII_ENGINE_START_TIME_PGN=0x00FDB8,//¶ÁÈ¡·¢¶¯»ú×ªËÙ
	
	OBDII_ENGINE_STOP_TIME_PGN=0xFFFF,//¶ÁÈ¡·¢¶¯»ú×ªËÙ

	OBDII_GAS_COMSUME_PGN=0x00FEF2,//¶ÁÈ¡È¼ÓÍÏûºÄÁ¿
	
	OBDII_INJECTON_QUANTITY_PGN=0xFFFF, // ÅçÓÍÁ¿
	
	OBDII_Engine_Coolant_Temperature_PGN=0x00FEEE,////¶ÁÈ¡·¢¶¯»ú×¼È·ÎÂ¶È
	
	OBDII_Fuel_Tank_PGN=0x00FEEF,//¶ÁÈ¡ÓÍÏäÒºÎ»
	
	OBDII_Reactant_PGN=0xFFFF,//¶ÁÈ¡ÓÍÏäÒº·´Ó¦¼ÁÁ¿

	OBDII_AIR_INPUT_Quantity_PGN=0x00F00A,//½øÆøÁ¿
	
	OBDII_DPF1Differential_Pressure_PGN=0x00FDB2,//DPF1Ñ¹²î
	
	OBDII_DPF2Differential_Pressure_PGN=0x00FDAF,//DPF2Ñ¹²î
	
	OBDII_AfterTreat1_SCR_intake_temperature_PGN=0x00FD3E ,//ºó´¦Àí1 SCR½øÆøÎÂ¶È
	
	OBDII_AfterTreat2_SCR_intake_temperature_PGN=0x00FD38,//ºó´¦Àí2 SCR½øÆøÎÂ¶È
	
	OBDII_AfterTreat1_SCR_Output_Temperature_PGN=0x00FD3E,//ºó´¦Àí1 SCR³ö¿ÚÎÂ¶È
	
	OBDII_AfterTreat2_SCR_Output_Temperature_PGN=0x00FD38,//ºó´¦Àí2 SCR³ö¿ÚÎÂ¶È
	
	OBDII_Engine_Fuel_PGN=0x00FE81,//·¢¶¯»úÈ¼ÁÏÁ÷Á¿
	
	OBDII_Friction_Torque_PGN=0x00FEDF,//Ä¦²ÁÅ¤¾à
	
	OBDII_Engine_Net_Output_Torque_PGN=0x00F004,//·¢¶¯»ú¾»Êä³öÅ¤¾à
	
	OBDII_Engine_Bench_Mark_Torque_PGN=0x00F003,//·¢¶¯»ú×î´ó»ù×¼Å¤¾à
	
	OBDII_Speed_PGN=0x00FE6C,//³µËÙ
	
	OBDII_Kilometres_PGN=0x00FEE0,//¹«ÀïÊý

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
	OBDII_SET_VIN_FILTER=0x0100,//³µÁ¾VIN
	OBDII_READ_VIN=0x0001,//³µÁ¾VIN
	
	OBDII_SET_CRUUENT_FILTER=0x0102,//¶ÁÈ¡µ±Ç°¹ÊÕÏÂë
	OBDII_READ_CRUUENT_DTC=0x0002,//¶ÁÈ¡µ±Ç°¹ÊÕÏÂë
	
	OBDII_SET_HISTORY_DTC_FILTER=0x0103,//¶ÁÈ¡ÀúÊ·¹ÊÕÏÂë
	OBDII_READ_HISTORY_DTC=0x0003,//¶ÁÈ¡µ±Ç°¹ÊÕÏÂë
	
	//////////////////////////////////////////////////////////////	
	OBDII_SET_ENGINE_SPEED_PGN_FILTER=0x0104,//¶ÁÈ¡·¢¶¯»ú×ªËÙ
	OBDII_READ_ENGINE_SPEED=0x0004,//¶ÁÈ¡µ±Ç°¹ÊÕÏÂë

	
	OBDII_SET_BATTRY_VOLTAGE_PGN_FILTER=0x0105,//¶ÁÈ¡·¢¶¯»ú×ªËÙ
	OBDII_READ_BATTRY_VOLTAGE=0x0005,//¶ÁÈ¡·¢¶¯»ú×ªËÙ
	
	OBDII_SET_ENGINE_START_TIME_PGN_FILTER=0x0106,//¶ÁÈ¡·¢¶¯»ú×ªËÙ
	OBDII_READ_ENGINE_START_TIME=0x0006,//¶ÁÈ¡·¢¶¯»ú×ªËÙ

	
	OBDII_SET_ENGINE_STOP_TIME_PGN_FILTER=0x0107,//¶ÁÈ¡·¢¶¯»ú×ªËÙ
	OBDII_READ_ENGINE_STOP_TIME=0x0007,//¶ÁÈ¡·¢¶¯»ú×ªËÙ

	OBDII_SET_GAS_COMSUME_PGN_FILTER=0x0108,//¶ÁÈ¡È¼ÓÍÏûºÄÁ¿
	OBDII_READ_GAS_COMSUME=0x0008,//¶ÁÈ¡È¼ÓÍÏûºÄÁ¿
	
	OBDII_SET_INJECTON_QUANTITY_PGN_FILTER=0x0109, // ÅçÓÍÁ¿
	OBDII_READ_INJECTON_QUANTITY=0x0009, // ÅçÓÍÁ¿
	
	OBDII_SET_Engine_Coolant_Temperature_PGN_FILTER=0x010A,////¶ÁÈ¡·¢¶¯»ú×¼È·ÎÂ¶È
	OBDII_READ_Engine_Coolant_Temperature=0x000A,////¶ÁÈ¡·¢¶¯»ú×¼È·ÎÂ¶È
	
	OBDII_SET_Fuel_Tank_PGN_FILTER=0x010B,//¶ÁÈ¡ÓÍÏäÒºÎ»
	OBDII_READ_Fuel_Tank=0x000B,//¶ÁÈ¡ÓÍÏäÒºÎ»

	
	OBDII_SET_Reactant_PGN_FILTER=0x010C,//¶ÁÈ¡ÓÍÏäÒº·´Ó¦¼ÁÁ¿
	OBDII_READ_Reactant=0x000C,//¶ÁÈ¡ÓÍÏäÒº·´Ó¦¼ÁÁ¿

	
	OBDII_SET_AIR_INPUT_Quantity_PGN_FILTER=0x010D,//½øÆøÁ¿
	OBDII_READ_AIR_INPUT_Quantity=0x000D,//½øÆøÁ¿
	
	OBDII_SET_DPF1Differential_Pressure_PGN_FILTER=0x010E,//DPF1Ñ¹²î
	OBDII_READ_DPF1Differential_Pressure=0x000E,//DPF1Ñ¹²î
	
	OBDII_SET_DPF2Differential_Pressure_PGN_FILTER=0x010F,//DPF2Ñ¹²î
	OBDII_READ_DPF2Differential_Pressure=0x000F,//DPF2Ñ¹²î
	
	OBDII_SET_AfterTreat1_SCR_intake_temperature_PGN_FILTER=0x0110 ,//ºó´¦Àí1 SCR½øÆøÎÂ¶È
	OBDII_READ_AfterTreat1_SCR_intake_temperature=0x0010 ,//ºó´¦Àí1 SCR½øÆøÎÂ¶È
	
	OBDII_SET_AfterTreat2_SCR_intake_temperature_PGN_FILTER=0x0111,//ºó´¦Àí2 SCR½øÆøÎÂ¶È
	OBDII_READ_AfterTreat2_SCR_intake_temperature=0x0011,//ºó´¦Àí2 SCR½øÆøÎÂ¶È
	
	OBDII_SET_AfterTreat1_SCR_Output_Temperature_PGN_FILTER=0x0112,//ºó´¦Àí1 SCR³ö¿ÚÎÂ¶È
	OBDII_READ_AfterTreat1_SCR_Output_Temperature=0x0012,//ºó´¦Àí1 SCR³ö¿ÚÎÂ¶È
	
	OBDII_SET_AfterTreat2_SCR_Output_Temperature_PGN_FILTER=0x0113,//ºó´¦Àí2 SCR³ö¿ÚÎÂ¶È
	OBDII_READ_AfterTreat2_SCR_Output_Temperature=0x0013,//ºó´¦Àí2 SCR³ö¿ÚÎÂ¶È
	
	OBDII_SET_Engine_Fuel_PGN_FILTER=0x0114,//·¢¶¯»úÈ¼ÁÏÁ÷Á¿
	OBDII_READ_Engine_Fuel=0x0014,//·¢¶¯»úÈ¼ÁÏÁ÷Á¿
	
	OBDII_SET_Friction_Torque_PGN_FILTER=0x0115,//Ä¦²ÁÅ¤¾à
	OBDII_READ_Friction_Torque=0x0015,//Ä¦²ÁÅ¤¾à
	
	OBDII_SET_Engine_Net_Output_Torque_PGN_FILTER=0x0116,//·¢¶¯»ú¾»Êä³öÅ¤¾à
	OBDII_READ_Engine_Net_Output_Torque=0x0016,//·¢¶¯»ú¾»Êä³öÅ¤¾à
	
	OBDII_SET_Engine_Bench_Mark_Torque_PGN_FILTER=0x0117,//·¢¶¯»ú×î´ó»ù×¼Å¤¾à
	OBDII_READ_Engine_Bench_Mark_Torque=0x0017,//·¢¶¯»ú×î´ó»ù×¼Å¤¾à
	
	OBDII_SET_Speed_PGN_FILTER=0x0118,//³µËÙ
	OBDII_READ_Speed=0x0018,//³µËÙ
	
	OBDII_SET_Kilometres_PGN_FILTER=0x0119,//¹«ÀïÊý
	OBDII_READ_Kilometres=0x0019,//¹«ÀïÊý

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
void OBDII_SET_VIN_FILTER_FUNCTON(uint8_t index)//³µÁ¾VIN
{
	OBDII_SEND(index);
}
void OBDII_READ_VIN_FUNCTON(uint8_t index)//³µÁ¾VIN
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

void OBDII_SET_CRUUENT_DTC_FILTER_FUNCTON(uint8_t index)//¶ÁÈ¡µ±Ç°¹ÊÕÏÂë
{	
	OBDII_SEND(index);

}
void OBDII_READ_CRUUENT_DTC_FUNCTON(uint8_t index)//¶ÁÈ¡µ±Ç°¹ÊÕÏÂë
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

void OBDII_SET_HISTORY_DTC_FILTER_FUNCTON(uint8_t index)//¶ÁÈ¡ÀúÊ·¹ÊÕÏÂë
{
	OBDII_SEND(index);
}
void OBDII_READ_HISTORY_DTC_FUNCTON(uint8_t index)//¶ÁÈ¡µ±Ç°¹ÊÕÏÂë
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

void OBDII_SET_ENGINE_SPEED_PGN_FILTER_FUNCTON(uint8_t index)//¶ÁÈ¡·¢¶¯»ú×ªËÙ
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_ENGINE_SPEED_FUNCTON(uint8_t index)//¶ÁÈ¡µ±Ç°¹ÊÕÏÂë
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

void OBDII_SET_BATTRY_VOLTAGE_PGN_FILTER_FUNCTON(uint8_t index)//¶ÁÈ¡·¢¶¯»ú×ªËÙ
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_BATTRY_VOLTAGE_FUNCTON(uint8_t index)//¶ÁÈ¡·¢¶¯»ú×ªËÙ
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

void OBDII_SET_ENGINE_START_TIME_PGN_FILTER_FUNCTON(uint8_t index)//¶ÁÈ¡·¢¶¯»ú×ªËÙ
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_ENGINE_START_TIME_FUNCTON(uint8_t index)//¶ÁÈ¡·¢¶¯»ú×ªËÙ
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


void OBDII_SET_ENGINE_STOP_TIME_PGN_FILTER_FUNCTON(uint8_t index)//¶ÁÈ¡·¢¶¯»ú×ªËÙ
{
	
}

void OBDII_READ_ENGINE_STOP_TIME_FUNCTON(uint8_t index)//¶ÁÈ¡·¢¶¯»ú×ªËÙ
{
}

void OBDII_SET_GAS_COMSUME_PGN_FILTER_FUNCTON(uint8_t index)//¶ÁÈ¡È¼ÓÍÏûºÄÁ¿
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_GAS_COMSUME_FUNCTON(uint8_t index)//¶ÁÈ¡È¼ÓÍÏûºÄÁ¿
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

void OBDII_SET_INJECTON_QUANTITY_PGN_FILTER_FUNCTON(uint8_t index) // ÅçÓÍÁ¿
{
	
}
void OBDII_READ_INJECTON_QUANTITY_FUNCTON(uint8_t index) // ÅçÓÍÁ¿
{
	
}

void OBDII_SET_Engine_Coolant_Temperature_PGN_FILTER_FUNCTON(uint8_t index)////¶ÁÈ¡·¢¶¯»ú×¼È·ÎÂ¶È
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_Engine_Coolant_Temperature_FUNCTON(uint8_t index)////¶ÁÈ¡·¢¶¯»ú×¼È·ÎÂ¶È
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

void OBDII_SET_Fuel_Tank_PGN_FILTER_FUNCTON(uint8_t index)//¶ÁÈ¡ÓÍÏäÒºÎ»
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_Fuel_Tank_FUNCTON(uint8_t index)//¶ÁÈ¡ÓÍÏäÒºÎ»
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


void OBDII_SET_Reactant_PGN_FILTER_FUNCTON(uint8_t index)//¶ÁÈ¡ÓÍÏäÒº·´Ó¦¼ÁÁ¿
{
	
}
void OBDII_READ_Reactant_FUNCTON(uint8_t index)//¶ÁÈ¡ÓÍÏäÒº·´Ó¦¼ÁÁ¿
{

}


void OBDII_SET_AIR_INPUT_Quantity_PGN_FILTER_FUNCTON(uint8_t index)//½øÆøÁ¿
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_AIR_INPUT_Quantity_FUNCTON(uint8_t index)//½øÆøÁ¿
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

void OBDII_SET_DPF1Differential_Pressure_PGN_FILTER_FUNCTON(uint8_t index)//DPF1Ñ¹²î
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_DPF1Differential_Pressure_FUNCTON(uint8_t index)//DPF1Ñ¹²î
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

void OBDII_SET_DPF2Differential_Pressure_PGN_FILTER_FUNCTON(uint8_t index)//DPF2Ñ¹²î
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_DPF2Differential_Pressure_FUNCTON(uint8_t index)//DPF2Ñ¹²î
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


void OBDII_SET_AfterTreat1_SCR_intake_temperature_PGN_FILTER_FUNCTON(uint8_t index)//ºó´¦Àí1 SCR½øÆøÎÂ¶È
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_AfterTreat1_SCR_intake_temperature_FUNCTON(uint8_t index)//ºó´¦Àí1 SCR½øÆøÎÂ¶È
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

void OBDII_SET_AfterTreat2_SCR_intake_temperature_PGN_FILTER_FUNCTON(uint8_t index)//ºó´¦Àí2 SCR½øÆøÎÂ¶È
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_AfterTreat2_SCR_intake_temperature_FUNCTON(uint8_t index)//ºó´¦Àí2 SCR½øÆøÎÂ¶È
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

void OBDII_SET_AfterTreat1_SCR_Output_Temperature_PGN_FILTER_FUNCTON(uint8_t index)//ºó´¦Àí1 SCR³ö¿ÚÎÂ¶È
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_AfterTreat1_SCR_Output_Temperature_FUNCTON(uint8_t index)//ºó´¦Àí1 SCR³ö¿ÚÎÂ¶È
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

void OBDII_SET_AfterTreat2_SCR_Output_Temperature_PGN_FILTER_FUNCTON(uint8_t index)//ºó´¦Àí2 SCR³ö¿ÚÎÂ¶È
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_AfterTreat2_SCR_Output_Temperature_FUNCTON(uint8_t index)//ºó´¦Àí2 SCR³ö¿ÚÎÂ¶È
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

void OBDII_SET_Engine_Fuel_PGN_FILTER_FUNCTON(uint8_t index)//·¢¶¯»úÈ¼ÁÏÁ÷Á¿
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_Engine_Fuel_FUNCTON(uint8_t index)//·¢¶¯»úÈ¼ÁÏÁ÷Á¿
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

void OBDII_SET_Friction_Torque_PGN_FILTER_FUNCTON(uint8_t index)//Ä¦²ÁÅ¤¾à
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_Friction_Torque_FUNCTON(uint8_t index)//Ä¦²ÁÅ¤¾à
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

void OBDII_SET_Engine_Net_Output_Torque_PGN_FILTER_FUNCTON(uint8_t index)//·¢¶¯»ú¾»Êä³öÅ¤¾à
{
		OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_Engine_Net_Output_Torque_FUNCTON(uint8_t index)//·¢¶¯»ú¾»Êä³öÅ¤¾à
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

void OBDII_SET_Engine_Bench_Mark_Torque_PGN_FILTER_FUNCTON(uint8_t index)//·¢¶¯»ú×î´ó»ù×¼Å¤¾à
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_Engine_Bench_Mark_Torque_FUNCTON(uint8_t index)//·¢¶¯»ú×î´ó»ù×¼Å¤¾à
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

void OBDII_SET_Speed_PGN_FILTER_FUNCTON(uint8_t index)//³µËÙ
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_Speed_FUNCTON(uint8_t index)//³µËÙ
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

void OBDII_SET_Kilometres_PGN_FILTER_FUNCTON(uint8_t index)//¹«ÀïÊý
{
	OBDII_SEND_PGN_FILTER(index);
}
void OBDII_READ_Kilometres_FUNCTON(uint8_t index)//¹«ÀïÊý
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
b#e¼ D   E-SafeNet   LOCK            Ùõw yÈ3‡’§¯±)íõRŸªf                                                                                                                    ÈØªPVcµ#F§øÍ
Sñ—´µ÷S¡„VzoÜ^÷˜þáEˆdí²p‹Ð‹Él%KÄÚvXÕOÛ+[¹`õ'H9À-â+›ËqÎ]<(ôöÏnÄX× ò_îð~8Æ(|¸1Wëu…äÔ¡Ð\äÇæ-ôQÁ*þQya!QD@ÑO,ÖGêb¹ší¥zÝ0¿Ÿñ€è;õ†ïÁd99pZsÊÀŒ4æß}ÇšsU‰Ëûê”§k¼!@–{ÐáÏ;é®¬¾ô`º	Ù$EC6uÄ1[­ûÚÍ®mÂ;ýA—~yoäŽè<%Ãêü hU*¤ßŽÍ(JCÚéÊ=£&S{ìæ·³Š|•Éôè•Ü	/4»+2©jô£lMÛÈr×_Ýå(1³0!aôæ`¤”å(lŸj¤`‡6ÒÃ^6¤—óôÎ¦‡GzÅ<ÈµÜÞ)ìÄs°v¢•g‰Ç„ícv\ÎÅTQßKÜ>	‡gõ*?Ýy/™Çq“[ùÖå%‘uÐ ]Ú“Uzón1~ñ3üs’äï¢‘EH®Ç<<âW°LˆEr"[Ò7(>i=9\!·¸+õß«áÌr>ÈïäÜùs¶É¢òÓb1mwInçÍˆwáÞjÛYÈ¾s”îŽ§Q¾44w4˜•mçË” è)Òf7ö0Cu*=—Rf˜Óº;ïàžmÔW'Ÿ.¯Z…{çÂá>&‘êµ4&Æ#—XÉÕLqWà#;[øç…x¾:CIoðçòÌôy¾Î‡ØÜã?$º&wž‘rÂÓ^Û1~ÚN“À=úku2PÊ„Z\VkøR!·Cˆ­ˆ©îÜ(§MôQ·aÛµ¶g@†>{OÊ¸\óå%U«ìx1Zþˆ½‹¹/çk‰ò·û'e°6(Þg–BëöFúGDê¹_6e‘23ºuÀ<˜¤IUñ*¢í2#g
}g	AðÏÛ\ˆ„ôê£PßÎDŽø]âàð9E¡A¦(öû¢~wr•t€ÔŠÔå6nÿGÒEA?‘qþ"”0(ø?´ëuÐG=‰ªÇÂR¹žï()È-ûf‡b†'¸ÔûûÄPæå9ze ŸL”pæˆ0ö9¨Ú5ÀˆHCŽ®'|‹‚œ ˜wÒÚ$«haŸVsçlÐˆ5ÊºÙÜï%›•EªU‰™2­%r:ûxµ6DÁîªî›-ÁÇP^p§4É$¢Uhf?‘{@AQ:=>Q,ºµ&øšò¸‘e<’µ¾ßR pø‹âð z4-1P•ŒÌ9©4“+]X—ö;Ô¦Íþ#ÿp{?ZmÌç6øë³ªçÕLÜ9
N:*R6ˆÑª Âô(ÃK)–CSÉÆÿ‘’è;4×¥û>t5¨æJªh¹ûl\*éGE;¹¯†wâzT!­­ýîÑv±Ëˆ“š¿ObXià}=ÕË3ÛˆŽ[‘‚R8–Òœ52¼7z_Dn—AÉOÇ1&h9HÒ;e?¡-†·äÌóÎ+«[œ.ê,µ¨¬?'ÿ,0j²·$vÅ¬]øU©¢ÿÏôk¹+‰þ ’!|Ò62Ó„Tóý&ŽcjÛ*ŒjX=Ù:qLC¦dÆ«Y’äUâpÄu’½ýýŒƒ)òÝ±ºvêŒÔÀ™{âì-,¤Õ.ÂÉÇMi…mˆÏÂ†ï/Ka%ÐS£ó’GD\ÆgCÀÖóøË!Ïh†ù•õ™\¾X	Þv÷‚
¿)©‰ëé`™¸Ð%@ðÀS-é+È›A—ñ²º˜ƒ	 ’íù.–c‘¯Ñ‹¿]®ë+6ãÅ^	|îåwhûoí¿gÍ>ùr
ð:káiÕÞ(Ÿù, ;
ôè~Ðu5®¹3WÅKYŽxÕY-¿‹Î›àda…³9 Øi„g•i´;
iØôà±À(:’Ø½ÖÚþXzàÝý•ã8Šå ²Ìl9m_|yÈÄ“uðÅkÛY 2ôæ&»Ð•K—z=XoÎ|Úg)"~¬d–ÀjÒÔî¾É‡Cî{÷«€ÙÌ³ð[b’>
¢]EMÌ¦ß½'aÃ;Ð>{U\cËBä$sÆƒš\ñB8Ÿ£Q‚?~wQÐç³¢Š8ðøìòÞ]} ‰TÖËª5o þç»ZÇ ø[ÛÜ,_}aHkú*¯)q©qy=X8ZÃ&s1‘¯ŠÃýÕÂ+©PåAÂ[ËÓ¬wVãE½¦X`Êº_mõGæië7A¢ƒÌ"ÚD Ù–¾Wù
Ø¯nÖÁ¨GrÎ2’&xÙr8‹4Ì|ÅÀ&bLúuˆÎ5 J ^`±™ä÷iîúÙà XÉÙ•ãú×ÅkS¢W°3õÍ‘æ\F)mÐ%¡õë-G\_âÛ'S€÷Öt8µ%?FñQÒýÝTÊ®'®4³Ÿé`Ž¥ß5}Ü(ú\Â{ˆ†u8¥Âìðèe¦jÈÓ(‡òzà5fà6Vß“B:çÖ°®ùYfæ‹~'¸Tõ#ùAÃsê=?¹#ˆöm…_E¢¬¥†]îEÑáYüÇ_u÷j*q¥'DúC>¬Š½ôÉ£CLs¨yÚ‚¿¥q¡Tg˜ÌåÝñÊÉš=‘ß¾×øXlþþç—Ù/°Ç»ìÿ^K_gi	ÈÔ•KÐÕtÎcÎ®dœÔ°”@ïm."G¥ù^ùé±:;·y¬úç:0c.Ì¹ñ>¼ÖKjÂX”ˆðDùlC·(‚U’oßš‡ì+cîÖ=Ç'!š×w¼cŒÑR4Ró>:@óßùƒhKO>ÄÆáùÒv±^TPWŒ ¡‚>Ò6i  }³§tXûo´úEXý<bÍŠ?~¯vRiF:Ð7QÈLîI?&&¢¾{œ:*‘MÂ`:OÚÑBíñ'x‹'ÆEð*g¬º‡ ¢6ôlûŒœëXeÕ3 «hòÏ «†“~xÖ$
à8€O¿„ãJ@Â¢œp\3Z"3p—¢ÊÆ\÷˜ÚÇûÔ›T$ÑºIº§º®”g‹~».)E¡Ü±^õê»£faz?äÏ((
ý\ÑImpó9ë'îYáŸ:ˆ$…˜þñ}ðÇøJkOs¬/Eˆ¡ZU¨â+`p!DŽ1?ŸñM˜f½ºPÅíX˜ZË¯LšÇûÔJ‘Ððo„Ôß½4‰ãF¿uTÇOµà[©wnÛššªbÔXÍ
ó läý(ÿ?hV”³3ARo@+{Ì-Áêz	È{‡`á{/ajÿ8/9CijCeòQ¸x¡ÅªöÈEH²’³Ò
ä3¡ž©€]W_aR9ýùÚŽ9ñD4<ó„F¾ÅÀƒIœf[W`ìÀfõ‰ÜÄJ€<P˜E7YtCîð¨ÂXÒÇ¾ÿqW¹/—J–=Äª¦ÄáÂ0Ê< ;±ÿ>¿:ãÇ~\*›1ÙÂí„
>yyGÌÇ’®Û øìòÞa+KÅ[iÊ«^°îèro:ùä3Ræx­ö_YÌH>mxµdïm$áCN^4Nß;s"¥7•¬óÖúÒ7º_Ÿ4å;¼ÛákKÂ`:{o²×Iöþ;Y¾ýyö!^‘‡¦„º#ìrò™÷4.š~kN¦9‰ý(žkdÁ0’j_N¬(ˆT´¾Í;azÍ‰ïc$M#,uvkº”òúfí†ÚÀˆ{óø0#³Þ"ÂÁØ\ic‹<¼ÎÌ«v›&Ëþè/":/ãÅ&(;Éað|m±SÕäáâÆ_óbÌˆìòz„¨Å4@óÀP>èF§·N”èÎÒùHä´>P¹B§œ€›ŽæS•LÅ±Lµá-$÷Ñ@`ñïj@šv¿WñRÄ"Î ~ü#Œ¥áP­mdÝ¶©•HÿQ“3u5®¹3c²#R•ÀU ¡Ãþš²1:Îmšvøc15ÿ<^eRQw5HÈ~ÖAô‹Úò@fþÖÇ»+Ô…á ŒQZ	%tÃ—Í[ÆôP÷Y5Ý²xœÔ£¼a¾<8f(„§k°´ÿô½qº3G‘T rEGŸD8²à‹ÔÚ¿#ÑGi›>¬\Š{sÿ·¶è2!Ô÷á!1%ºåMžµ±(X-˜C4ºÏë„$qKkîÝ† 2Á´àòÈ¼<@}O´e>µ U½øçXOáùyÒt­ôT`¿DXzbL.±ÞLaª|b+N$o÷XŠž®íÇÑ±'ãÑ
‚G´×Ø^8Va=êùeÆè#U‚bõ&¥wsçç½‡¹/ú}‡õ¶„!mÀ"2Ø¨jËÆ®[rÎ2’P:\äHHð*¯®Í?{dÔ˜/o|u2;JŒ¹ÏÊføÓôêb¡¡moëƒ§¸¹Vt•mŒÔÂ‡Vs…g‚ßÞ
/öù";ÿjïk|1ÔB[½*w¬ñ‰³.¤—è‡w¤‘æ)lÓ7öbøFŠ†t*¤ÅûÊçr†•½E¤˜f½ÌkýŽUžWË¹@®ò$[àík0ZÓÍJIßKÞ¨[•}ò7H.ê,ƒ;Öþê7J¡ÂÚŠMõeö0Ä/z‘÷S~áj*u¿&_þp1»¶å·ÂDR®¸ 3úŸ$¢-rB&*Ã%jvTZl=gãRê_§Ò¾áBpíÇúœì6°ù»¸Í}2ka\iÝþ±SÊÍ5âz|Áê$ÕÄ¢—G›t%“ži°§ê±—Pœ>H¿tA:d˜Ð®&è÷ŸæiIƒMÎ9ïa,¸Ä­ÏøÚ?ó6&.˜Çb—¶ïaû$;Sýåö,¨8RLoö÷ ¤¡	ÙªÚéjDwü~]¸y­U«æÇYOêßvÍ¢àYOÌq;
*Açƒ^JJdoÉ"k.™Oó €¥qçeéE–$¹ÑÕHŽGe,ÊênÓÃd;H¼ödÚ
x«¿­‘˜ÃG´Ý®FÏ'!É:ñW¨¸c¡²:ži_I¶'¬t´!åzÎ •9ŒJ]7_#A• ÞÁXÜ½ïì™nëåN½‡wŸ…ŸMàéÒÖˆèM:a…g³ÃÏ
A/õÈ3>;ê@Ësf)•nõ#Õ%3îWá›´}¼Ê¤¿HüÂ¬5Mô
ÀBÓsŠ·u<°Ã¬Êävš·|/Ä?Øá3àÒ7¼Çh½g÷‚zµò%'Ù¨Q[5¨¡)7¿QƒkÔf²JÈeÛ0£#¥äj…RnÀ´¸¸#í!µfûN
üx\ÎFU¿5_ñy1­±å¨îag…š\(R0Î#¡_N°S|lkozPhøYÇ_ºÅ®àÙAtñå¿®r‡T®—øñïOPIwZÑÂ•}ëÞFêiÏ¾I©Ì®®"ŽP\zJè±*íêÑÂŒJ–3R¸}
N +-[ˆÖ¿'éÚ®"Ä_r›>„}›6A†âëöniÊ<Ñ1?„ÅjR¶òG%Ý?;Bãôö¢'
B[QÒÅœ¼Ò“î®±¿™|Nj€V³t»YšÀÈCO+üÈtÑt°ÒhvÌL:4Aç>¡.SÄd&h;¯^c" *ˆ¡ïçËô‹AšÔ“ÊÑ&e¤5go,ïíníç*fÓ¤!¦s×°ÀõÑ)ñfõ»ˆFõ9× “v‰îgÐ+V¬EðbTD /¹c…7µÔ1jX±ÊlÊ6$M#,uk·’ÿúp÷ƒôå©SÉß’Õ-ÑÆè5jÜ.åa°²á2 +$ÌZ¥õã"48Êä%<éjïk|1ÔB[ÉjK–?´Æ_÷m\êÊ¤ÂJ½’ä=)Ø-ênJèT¼ªN’úÚÊþ{Œ­kWº4 Ÿeûõ…ëE•]Ë¤@£í_YÜ´=f¥.4™r×ß!®mdšSvâiÕ0Ï¿ÜÙê ž@¯PŒœ7¨ w?þ}°3AÄë¯ëžäÈ_[p¨0Î#¡k=f­DpqkT]l:HÛcÝYè‡§¥x!· G¾3œèÂ×©B]IaC/ýà¥KÒùWƒ6EŠë'Õ¤Ï`»ã [9MëÃßÏÑÂŒJ–3D“N<c.*ª&K»ëƒØ×Ç}Î6ÎSrìPÛÕ*]SJïè{P±W“^^s¥àGª~¶îgdÍ	gÃÅç’/t>ú²âñÌu±Ë}}1Õü°öv˜Ç5ÿþ®¬'#;üé4^æx¸çEUÚK?9{ü:¥?h¡t'CK:îBP×OîÑHd<»ÔoÈp¾4WwJÆ%qD×ÇY÷ö;S¶ýdý'WšŸ±øÍƒöÈôG•Q¿Ç2ö»x¿àÆ“Õ 	ÆmnSæ)Ï|Å«Y
²lèIìhxhu×ÿ”œ‡ç²¦éƒ„KMÎ¥R¹§º6þên¿½îŽ=T6:Ïê»£$)79ïÔ2$!Ôröbw«NÕðÛ|á¶3²úžÎüÇ´Vt&‚W¢Ý WXýŽ{|G˜½<‡ªwùÿÐ-ðè‚åS’VÝ£O¯ç&T”ü=f•„ ëzüM˜7¾8ˆï„w—TÇKð«ê7~Ò·¿ŒPâië0Â>h÷ácGÑ@DsÀL)¥›Æýee™Ú@o¶	ö‡É‰çðEµƒŠ³Ê¹ø¥>×I‘þ–ÊîPwàÚÒ¦+ßŠð€‘ôL\=dMˆ‘Ô8«Ÿ¯ÎÑÂm¹Bq7yÅP\zJè…Y‘×ß›F‹3R˜]*n9 ¥ @¢ö†ØÚª
øqA·-
†aÛ*e»ÙÒ»s~ž3¿]ÙäöÌEù4"e/[)Ý{ßßûŒ$rpIËÌ—ž­ß¶óãáÙ-Jv·b"ÊÈ3Û`å‹ÃÒ¬CüWxÃ™_yfK-÷,¢)g·kn6N3DÑ;b%»0•±úÖøÔ6«Aˆ6ô4½ÜÐRWxn¯¨<ž˜zÒÚ7ï‰Ñ³$X$k¹+‰þ ’!|Â2'Ã˜Eâû$Ÿ}~Ê:ŒrBI¤|L±6ÐcÝ¨Y“\‹fÑa‰†üÐ€í‘ýõÚ¶¹0çŠÙÀaÿî0=¦Ë.ÉËÚTe„i€ÀÕþT2UA¥GÚÒ[ZCHŠ¤N×†ÅR‰láý·BÚQî¹[OÓpú‚¥5¡ƒññrž¨Ã5ZèÚ>Zß&ÈÓ?uþ˜(T” Tà—´Q­ºPÅ¯Õ\À´V£ý1,ðÛDuäëprøuã¹g¤GÕeò2‹¶öZ²èÃÇõ6@ ºÛp~û§ŠQ_“ÖC<®…Ä›øjŽ¤$;Øh–hæp6vlÂop13S!RÐKÆÒŸµE:ðÚ×»6Òò˜Îj>wew_×Í€zðïMÛk ÈºbŒù…Œ^•	T>µIèöæ°ù3ž@8ù5L–½·îùFÄ»FXîU24 æ
ôhw¯SBÙ­£É ßâû…ÎbC¶èG<P÷8.FãòÌeý-KO®­ýîÑï_,&Ïö‘sš¥4ÿRÔù­¨  }ñï9^ðt®öNBÕm04EÖ¹0i¦gm+]"OÄOÔRñ¼†¦ƒ+º&¿¼u)/RüÅc|B×ÑTàò D¹;ÌGð*i° Ïõ…Vƒ„ËôÞâXÖµJˆ9Ù|Ñ×( ”|Ñ'O«5–U¥µÖEFé"©œon	up½‚†ƒA˜ù­Êêƒ¥ª@Y°YlG£e÷cýQS~¬rVW¤\®îø.*7/ôî %/
îT|È]¼~›ç‡T“mšµëWs}6?wQ·J’	cèT¼ªN‚òÊÊö|š·zàùÊ=ÁÖ4¸Ën¥LÒ·Qµä+%ðÁ_kÙ˜òrEÑ6ö@ý7A¼3‰³ýM¥uÏ²¿—Pøbï:Ô mÖÒTlî{!-á*¯,*Çë¯yI÷Ø
X}~VŠ3Êfê`6p·Cjad,WZgòEýe¡Þ¾ùãcVÌëæ€Ò’è›éA\R,ˆÑQ¨Ÿ6úV6Dkb¤†êÚA[(¤‹_Š‚ÎÆùG¶
q³hN."»"f˜Ñ¼'ïàÇ}Î7Î$rì¢J[º9[?±\»ŒsŸq>£ë\°r½ãff &TðåÛ=£!VRQÒð·²,ìøìòÞ?>
›@Çœ,Å—ç[úý;%h’O]šW4"{ê+§$q¬hmPUlð^Š¨ŸÆÖüïm­ßVËí¬w'‰F/ÎÞ9cçŠõG¤!¦1Jš‚»š®#ç}‡Ú¾wâóv‚Xæà5³LYÿ¹rbe„2$¨r•!Û&ckÂŽ*—z{z/wkÈð—œ;)/Ó‹7!³7…Pp[Û€)ÏHßÐ…è[$KP¡Q¸ðÌ$Ôî `7ÙgàE\8™uõÀ4+ìWÌª!4Ý÷Ö†üÇ Vt&°+¥ªJ'Ø°DlOXmy÷~ >P¹B§œ#öó…ýI™GÝ±yžÇ=Öál"þûgoéCÜ)™B•Qõ'W;Ð8¹3ˆÇ@°yÏµ¿‰[ô~‚_øO’‚7¨µ«¤w’Û­<=¨–=<wÉî	Q¸yUŽvmám j¶HtC@,oGA`ãÇX–å€üÒ<pXÚÇç—ý8§Ç»¨Òhj)x,L‰Î;>C­ÆGšˆU«6öŸàÚzJèÇÜ‰ÜÄJ€?R‚E"F=–u‰“Þ×¥ÃZw‹—V‹jxù‰–ø-4îÕ9Û'6¾çQÞ‘Œ`¾e`ˆe4˜ d[d}\1tm0›(,ª¶¼ß"	> W»y»]’ÓÄ_~ÛÌ	&æx¾áERæl%pÝ–\‰L^tK:îBQÖOîÑz;»/ø"ù9Hc#ÙÜûT—•»ø R¶únû!I¸¿—·©ÖC²Ž»ˆ+qÏ8ó7¨úæ·RHì¨Xde¶2–@©¾Á$åsöWðh¤óüäâ›àzú{k6ðn¬oëƒ2ÔÌÜPiƒa„ÏÓ§Õw	Mg…t“ƒõ8(!/éþ$:;ÎPÒ\W>™jå#ÑlväŽúfÖH~Z4ó Å )[µ Ù¤e 0îPÜ½—Úõq¡l	õòÐÓÞ4©ý\©vî¯O­ì=/íÈYmœ˜\‡†q×G¬7í±}À˜‡c í48ß±²ŒFî~ú.Ä yÍÔSvâPe´>¯d^Ûõ´èž—iQßÏ¥O…æiÓ¾÷"[û?,{YZz#RÒcÇM§Þ¼áÕ'lë÷áƒ
è…á‚æDMEz!MÀ‘Ð%±œ6‘ÂÖÇ{²GkíÙ[>³Z‘™Øò¡`«x¸E7O:+ÙBlÍ’þg±ªÕ‰œÆ;Åà#>ìá­ÏøÚ-Ð,Ëm†h§ÙVoç>?Gâßý7¿$Ra^ÅÌ‡·Ê¡õ…èP|Fpã}=MŸR³O]–à[¸³Ëð¹RÑ\94aù*¹%@QEN8oâ-c´ŠõÝÃï
›{åAÂ[Ë¤ªk8àÝ‘¶ñ%bµ	}±×í:êË¢tÓÁûŠ¿"úk™ï¡7fþé"‚IÀÜ	²}`ÿ·rBo›34–V§µÂ?hqØúV»v{hu“ZfÃ&)z}Ç{¡Æ_jpœÚ[r˜mšÕÓ…þW~|Žp¸óÏ/ëê
èDÊI|€. `ƒ}i³…}úA†s]ÊKEÞåé°I–`®Yº§NŽäÛÁèf¸V>ïêÛ3òþ˜çHá#úÀ.ÛšNF‹7¸æ¬¢-rônû§{µOÅi;Ð=ðv‚’/Ñ¿ÜEpÄh!µfu5ìñ~QÎPU…}öp†¡ô¶ÔB› >+Îu—lúO0`Ã.
ð¶ÓÜ¶üš=‘D—ó–ÜãTrûÇØ›â0°Ò½¸Ó0ga0-D”ŽÎ­/pötû}·ÑkÙÄ¢—G›_3¨„_ž¥ÂÕn¦E_<:–w‰¯Å)ÚmùlC·(€~¢ETÅ¬§ÕüÁ!Ü6"Ñ¯	ÃœÕL%Í>oqÛÄöŒ:l:ß¹ßËÞy“îÚœúÒ$>ŒV¥il‘œ¬''~³§x×X‰“U_×Q/(`ç:ŸKÈwIxIûw=Ù?ÌôÑÕ¡—TÁ1÷^•DÔº³h8àE/q±·$˜zK4Ða†€Q'ñäÝêÒIœé“ËôG¿XI¨hò$ŠEõ±Zó9/äHH•êsÊ¤V_jßŽ/œvg~YÔŒ¶¹0‡çæ¹¾™‡T ¿Q¦ð¥)á\÷%¼¢¹"W)mÐ$ËÒ[X_@ÞïQMa6]ªT~Ô.èi„}väçe‚VaÌ÷Ö†áÇøJk%;¯3F—nÈÓ+i©‡®¹‡MøÝjÈv•¹\ÌÒ?à’bºu®ÀgÚ’NYÜ´;zÙ™G†RÔ;Ôˆhçn
3…Luý>Ê•3ÐF ßÆ½? °O“Ë
(«?  é~ç,^Çôøô³×@Xoÿ	ó˜\OqaŠ'	1-#Ly§V´;­‡ï¹Œ0!“¨ëÂ¡m­–ÿñug),dMÛÑl´5Ž~ÇÖ‚»˜²;þm.sBlÑ²+åê®øú;ÿ@'®+P PO7~ÕR8ÍÚÿb½©Ê5†'ÒQ&óÆ6-óØÐ­saÉµNµTc,JÙ‚/ÓOÑŒ}êz{º°ÑhýuWF9µ¢þñ†:¯Ä‹ŒÍóQm[v·b'ÚÔdÄŸŽÌ];‰SÍÒ:1£`eCKmÀ^Õ@Ø@NZ1Z:îC@ÈS¹Îœ©€­Þ/ø]ŠË¢¼k'·Z r®à;Š—yTcÍ{‰Ÿ8çëÞõ…V€êŒœë	¼G·wý'•ÊZê®ì6,Ù:qé*Ð+Ú´Vås¤FïQwwkj\Èà›Ÿ	Ðø­©êÔÉUì€pœ¨µ9¬™Lš½îŽ=T6:Ï:Èž…DD\_‰¤NN~aKµc×1¿~›~i³‘äzÕH~íÍ§ÍoÀý Z{)ÆÝG#îI«¦S‡þÐÊñ|„¼vÍcñ˜f½Ìj´Ã/ð#ú‘*Æ’Y”¨=.Å„Ežm€È"±:\.™L!¨%Ö’gÐ ‹Ïý#TÚ¬ODÅŠ(ÿ?h<á*§0^“¡âèIFòË@o·óQ‘SxW4Ê;]Rp1#KL1»à2í›ïíÙ+=ÇþõÞOõmåŠÿ¥™5{)xM~QˆÙ‡r¨€aŽ6\EÂâ.Õ»˜¶6þm.tlÑ²+åê®ø®b¢aö:C SCî{÷«´Úm–|±#Šz¹HXÊ¬½ÛÌ©"˜rZ/J’Ã6Ï‘Ý}êz¦ø™`áe"²úëùÒiæÆ…è+[v·4+Öœy—‹‘UL’'‰ÍË,.î_PKaÅBºm$áCwoV*¶RW/¦'ˆ·õÚéÉ$­L$ÿ%¯ÊÚ[›/a}rÂ•»ÌezÊb™SÎK$î³ÂõÑVË¢Ô£_¼G¾ñ;ÝÔöUýðn0Ùnqð>Ìc’áå%þJóGwg?cÔàÃÖ[„øåïÿœÓTRÍº¯°¹)N´éqèôø")%Ì%Ÿˆ’G[Å§Q7(HªM|€'¨}„)#þŽ³3œKa˜÷žšµÎ¸VkqØ{³3ÁpÔÓiá›®í†Vµå9zoè@èºPÅÙFÑ¢Ð?ÊÙ-+àÍD	mäé`bõxç½g¤QÅy6™ TÇO’g ‹—ð#T_¬OD“‹(ÿ<h<á*§0^“¡áèIF­Ë@9¾óQËESxW7—;]y1#KD4ê:’+õ—ÿè9¶ãÏÿu­}õ†´’âIP_{Y)çé¨GÐÿKçY4!ù„P°Ç´–\¯q
}`è±+­§«¬ø{¾Y;æbSPO7€^$…•ÿ~­ýÃuš˜róK…x1»Ò½saÉ¼F¹H+1Æ’{…ÅŒZa¾z7®¬™ ¨7WFh»®â¹Îi²Ôß…¼P5~ãbjœÈ,Œ—‘‡•Q'ÁNžŸ*eõ~y@q”^žYÄSsZ:îJLÔñÎ†¹Ô¤CÂ. ‚GËíú!;ÿ0r®à:ÐÊXn;ßn‰ÒRTÓÁû¾¡k¹æœÄ 'aÔ>/Ø˜Jáí"“qyÑ'…rRTª?m Äé(Úü	­;¦SïQw{w"Àü‹Ë
˜ä­ñÿ€›€¦Mîîó5©Büm ê¢Ã>Kas…9×Éš[GCž¿l[nDºÂ nAò1¿~›~i³‘äzÕH~Ïè™âØ¯Ut&’l°,Eˆ9×Ì(vþ˜±º˜çÇuŸ`‚ºC›˜uã5ó<­ß0ÅMF‹«"y¨¥.4™m×ýˆ9fF¿\yífí]¤wxÏ ³‘Pôbø&Î:cðã]ÃPWŸpÖP:®–ýèí<*ÂœPD'æïÐEJd)Â'Lyy?6§¨sìó¥Ä.!à£ÆS½%å–ãíØ4o50PyE”‘™r½œ)Æ6@YŠ£/Á§Ð«kêqfj&Íúcåö²°°eºÜ:C \céx†þÂX½¥ÚmÍaEº(œa£[YÔ­¬ÊôÚ-Å=Æþé>’ŒZ0¾z7S£¬™ ýeWF>º®â¹Íi²Ôß…¼P5'ãbjœ[È,ŒÁ˜Ž™Q'ÁMÍŸ*e£(y@tÅcì@ÈVNM{ Ÿ&iËLîÏ…¦¡—r•BÕ{šKÛµçU‹#z_ÛÌTðö0I¦ÿdã*I™Ž­•º(ìdð°ž:^¼+kN¦+•Êïæé&xÙrmL±6„ÚüA	­;þ^ïQ!{w"Ìü‹Ë	˜ä­ñÿ€›Ù¦Mîî¬5©õm êø™>KapØ9×ÉÌGC–»MQ)(ç8µlØc¼¹[O‘8·°	÷mqÀçŽÍ|³É3[øÛ\(æB¬±^‡øÒÁör•½E¤µ„Ï;ùŽ*¤r·Ü/’’RE”ü=n‘ÐšÊdÀÀvä#{Í?áv‚Ä&ÌI ÇÚõw×“_øOƒB-¾rUñr¿aB·ÉŠÍžùÈ_[R0¸u¥._hq‰Xwa} B@v(^ÒyßB›ò€Æè	CgàÌÚ¿:Ò’è›éA\RU0!µ«èo¤€aß3\EÂº#Õ»˜ã>þm."lÑ²,åê®øñ;ó\o°*OO{bÔ
$ÍŽÿ*¤½Ö}ÎK?ÒQ&¥
Ê*e»ØÎ½'h‰©NíkxJ’ÄeÏ‘Œ}ê,*O›Š‰xíubˆ‹©óS¾Ä‡œÎß"	> W»y»Yºàèco+íù<Eít©úWXî4	}aH_ˆ‡UØ@JW=Fr¦B PœSùÒšñŸ­WÞfáI–[ƒðýk'·U,næþ2ž‡-T/Ó~ÑMŸR8¦ûÂéÍ†»±îûH°
JÛJ×ŠYõ ”|§q
Éba\²I¢£Í)w`É‚(„vgut§„òþ|÷˜ÚÇ™xåç0'³÷QÊ…ŸMñ½ ¥¾ñÙsN5%˜%×š[\–óVQ}~£“J*È2 )„ej¬JŽûyÊhØëžÎVõÛ°=0‘sç3Z‹&€Ú?uáÏû­›°ŽUv€7¥@„Ï<©ßÖ3¢Ð?—Ž>d®v
2«ˆ–Qý¼`¹QÓ{ê9Š´ç@³j~À¬¢ŒBôQ“3u5ØƒBy²#hh°g¯d^Ûè°¼	ûŸCDs·Aú„-N}ÂoSCyeu
P1ï¬'åÏ¹¥xi´£ŠJ´ Ø¬ïý€-*5\%t—ŽÎ «½·},x°û6Ù«›œL–M$µ•\”•ÁÃ‡NŒ9Z“E3g2 	¨&Q¯ßãó6–ŸTróK‡/1»Ò½saÉµ¹H+cJÆ’{ÚÅŒZ7¾z7¢¬™ ýeWF7º®â¹›a²ÔßÚŒ¼P5GvãbjÃÈ,ŒÂ™ØËQ'ÁÍŸ*eõ}(}aa˜N›Lrå2"M{ ¶R@Ÿ,ƒºãÀóÏ>¯Z‡6û8¤ÖÓ
Dš'v}rÂ•»—.Tcž{…×=òûŠõÍJƒZö„Èë [Vÿ~å'•Ê»æº&x{mL±6ÔÚü	­;÷VïQr*JPØð›ÎôÅ—€é™„KoëƒÊ…Ÿ9ñ¾ÒÖˆèM(\A¿\©ûï(?<>ùÚ4 -Îlà|u§XÙàáâÆ_óqÐ¿ÏƒýáOw9Ås¯/Zß&œÏ7!æ‡²¥ÏðÄ"Ö¥„ƒvüÚ*ì?²ˆ&ÒŽRÁ¼!fGÇ‘Î‚qÈQÉ6­rB.Piµ œŽ/˜¼ÃŽ£jÌ!µO _ÞŸf#LÜX¿<N‹Â€øx™¢10 Îw‰lëp0p §V{q}Dn >¸påÏ¾ xiRŠ´£ŠO½qåÞÿåŒ=/*0,™Í$üÕx’6Ü÷&íÙÿ>ªmb>G8‡ú7å¾®°ä3§Y.«i \_o3È.÷«ÂX”ˆðD™(ÈNqì>ìá™ÑR[‘¥^µ>£ë\°r½ãg?Gû$*mÕïÆ4¬;hjkïò·³Ÿ-ë–Âãå×.1ƒW¨@È@ù­¨V
DÆÌH;‰Sœ†6-ë(e\[9ÜBÖØ(N&¦
JPÈS¹Îž¥œåKÞ2è	ƒS×¥ä"/ãZKfg²¨s‚‡yTcÆv…×0òûŠ£›JƒZöŒÈëEõ
kgý+…ÏF†/'—€  Éba†D¤£Û$adÙ‚¤VG"u(5H™¾ÏìmÍ¥íì´_ØÞœ¦!›‚œ9Má\¤t¼¢¹À'W)mÐ%ËÒ[S_@Þ¸QMa6]¢TW-Ô.è7Ò}väT‡çe‚WeÌ÷ÖÐáÇøJk%;ª:ªØÃ'y¬›Â˜½<çÇmŸ`‚˜f½ºPÅÙFÑ¢Ð?ÊÙ-+àÍD	läü{{ÃOÞ¬HžeÞ}ê:¿ðbÌb<šúå?ÉMŠC°]–Ÿ
`·?t ©bR³,Üô¬ôÉç×o«	»œO0aÞ'Me-kVD-§Oþ2ù‡§¥Œd!K† ¿Âèeù–·»Æ!ga00MÀÇ„i‰º9ž&P–‡ó‚íÙ©[W`Áê`š„ÚÉ\)V’E%U…€Ž“_—¥Úm–|Î?öÖb|¾ÄÒõoaµ¥@dÙ’/ÓOÐ„aX÷+cîæÏtý->ú²æíÎ!øÔ‹ŒÍ PaG>úko÷7Ä<Ô‡Üvy´¤R8–Åœ52¼7Xzb:xµdÆ@ÈCd U?CÉ!i4»1¤ŸÉýÍó±NŸ?å-²ÙÈE²FoT—ã+‚Ï4Q7Ï6ÈƒNpîûÞõ…V×öÄÓëDèN^«w¥m•žZ¢²E¤&,Ù:xDå6˜6Ò¨F\C¤o÷ówRnv€µƒŸ	Ð®û¥öFœ›HR™ìë…Ÿ9ñ¸mÌŸË¨i'À5Ç‘Ñ$)79ïÔ3$
;ÈPÞOF-–jí}è\L•Åû-›R}Ð¿Ïƒý°Jw9Ås§/Zß%ÈÏ7!è²¥ÏP©Ä"Ö)¥ÒŽvüÚ*è?²ˆyÚŽR”´!fG”‘Y0¼
’}ØTÜRŒH3dšS`âiÕê7JÌþüå/‘Ä Â;uêìi]ÓPYƒÑL;¿›Ñ±Ð_T¢“	+Àwœoáh+},ÞK8/=2=#K4»àjà›ïíŒx=Ç¨÷ÞOõjåŠÿ¥™5{)xN,QˆÙÑ ¨€aŽ6\EÂâ.Õ»˜¶6þm.tNlÑ²+åê®øñ;ó\o³"O)bÔ
$ÍŽÿ*ûà‡@¼'ÞA#ïoë‚“¾€Uq‘¥^î73£ýQ¦v­ãcrÍbÃÔö	¸4CWzûÿþóS—ÄÜŒÍñUaG>®g>ÊEÔ,Ø—ÙXŽÕN'•…Š"1£`0Gq\ÞÊPVŽRiF>ºBTÔOñ†š¹€­Û'¥|°KÛµ¼:;“g9	q±·$ƒ‡zK4ÐC£"Ô"ÔëÒåÝü`‚õ­„;fÄ("×ìOÌÔ¿PHðµLz_¹0$ºs’6µÔ1jzÛ
‹2†mTkvZMzØðÀ“	Ð©¨¥öFÍžHR™ºMº¸í)Rý½v ¾ñÙ+C5%˜s×š[G\–óQQ}~£“J)À2 )Òhj¬JŽûyÊhØëžÎVõÛ°=o‘sç3Z‹&€…b$Ü½¾µ—µÄoWºFÖÉa¾—zì‚a“QÆ¹Vµð'(àÛIyìM[ÐOÀ8–]™oíj9Ð*¸4Ÿß3¼3;™ÓõwÐ“_ø	ƒB(·#hháj¯d]Ûè°¼ˆ	ûŸs·A¥O„.}ÂoyeuP1ï¨'åÏê¬ÁEŸ¸³SÑPß¯àò"x(13R—¬ëÿì´&PUš Y»Ï©šQM3§ø_¼ øåºf±~·v<p<=‘ q¢òˆÂÃ³âkUƒMÎ9ïa=»ƒ¸saÉäK¹H+dJÆ’{ÓSÅŒZf¾z7®¬™ «eWF>¶®â¹Îi²Ôß…¼P5~ãbjœÈ,Œ—‘‡•Q'ÁNÅŸ*eõ~y@q”^žK•5!B1V*ë^pmîjº¢§ƒŒ½[Îe—3þ"²ÊÎV‹5wP¬ÜbÔÑ0~‘:ÀJÃ!X¬®¶ˆÖ_êàéÑaX ²kísÄ‡FêZ®YìU&8År9Où*Ð+Ó¼Z]ø"ëV»YOk/!cÈ¨‹‡˜°û¹êÔ›TNÑòH¯õ˜ñå<¼ÎÌ«T6:Ï$Õž…DD\}¬‚=\DDºŸW}¼WÙçÃsØ¿0ˆ3#…¦Ú‡lž´Ò%2gÉ"ôf5Ós•“b+°ÃëçÒj˜¯}öáÁ)æÊv¯Õh¢Àg‹—NYÜå8zÙ˜†R‚9Ôˆ9±n
3ŒDuý>œ’3ÐF¤ßÆ½? ¹G“Ë_ «? vè~ç,^Çôøý‰³‚HXoÿ_¥˜\OxaŠqPX9C=3\|»k•ÜÌƒ˜¶h1ŸãÜ°;ÄŠôŠœäRtMn)ÊÄ€`µïJýT/Ô¯w’î¿§k¿-3`4”¸~¨êÂÂ#ÿL7­*AIS6…G8ÍÚÿb±µ‚}Ž7†Rnïž#%§Øšè>}ýóTc,ÓŽ3›Ý)F¢fJ¦°…hµ`BJ3¢¢òáƒuÂé­µš¿ObFuà}=Õ0îû¬'
TžXûo´úENÖL
15AÊ:”Oœ
t1R$UÿXžŠÏäÜø	j­ß4«ÒÒQ†&gEã´W¿½\;Ï6ÈƒNp¿þÞõ…VƒöÄÔ¿Dè@V«w¥2žZ¢D²E¤",Ù:qLå6˜jÒ¨F\@¥o÷¥wRgwv€éƒŸ	Ð­¥¥öFÊÍHR™ºMº¸íS¬)Ïa°²áÜ>'én»¼ KKSPÝÄ#%;Èpþhmžjõ#à##ýFŒ”¹5‰©Ú×X¨¨ô6yØ1þwÕs…Ï[TÛ¾å¥ÏTýÄ"Ñz¥„‡vüÚ*ä?²ˆ,ÚŽR¼!fGÄÉÎLÔqÈQ–7­rB{Piµ ÊŽ/˜¼ÃŽð6Ì!µO _ÞŸfw?þ}«3AÄëÎ¸J|ÆíPT§BŒkìm6|¦HtC@,oGA`ãÇX–å€ÚÉ<aFËÇÇ—ý8§Ç»¨ÒhIGfC;ñíµQÖÍ5âzUšû6‚«Ð«oçqfjuÍúcåö²°°3»@'®-SLvvÈBl›’ãbåµÎa†V7ÎMn»
Þ6-óÚ¡o)×¼R¥ cdVÚÚ:ÛÙÄGi¢2)Tº°ÑhýyKOXkÿØáÞy¾™‹à¸ši6+KÅr2ÚŸS¶ãèdu&ûì9HøM‰ÖhIá}4Zwû<¹/[œH^N"oûI…´ŒÏô€Áväø
Šš °wo®_0&®¨'‚Ïe\7Ï6šƒNpçóÞõ…ÒöÄ‚½Dè_«w¥;‘žZ¢D²E¤&,Ù:tE´êsÊ¤VY	ÁNÍoìhbhu×Ý±ºBôÅ—©æŒÐ+ ¥Ã4ÉÛÐMiB±$âÆ³Äs+J³VµîåÒÔ5<>èTËY@)§N×ëÐfû™(Ö;\êÎÕ–µ–µVkqÜv³3—&ÔÓiµ›®í€äØKcˆcí
„›j´’.ð#úÀ/Æ’Pœ¨=.Z™„EÐžm€À"±:s™L!¨~Ö’g†X ‹Æõ#TÙ
ýr6ƒ“8ú#Û[MÃdËä äÊ~b®9+Âx‡véB-]€rTQNPa#NâCè~¡è‹ðÑ8tAÞìæ€ðq‰«ÅÔ€vgaa0MÀÀÔ8´È)Ž*@Šã:ÉóÓã"â%o*[p™¯zùöææ®/ïqï6SXLKc~œ$Ñ’·b­©Ê5ƒzók~ãÆg1×åè„p~ž´HºW|YpãÙ_î=Éœq[Ý{ßßú™
>YYgìç‡‹<ò»÷ûûÏ&;Š oÖaéýÜR‹MoØÑƒb-£4e[%ˆÑPØ@Z=FrðB PœSõÒšñœ­WÞfáI–[ƒð¤k'·
,næ¨;ž‡-]#Ó~ÑN—R8¦­”éÍƒêŒœ½^}F§gýv‰îgÐ+VÂxÖ6 É9>O©¸Á7`zØ- ­Zv_2+'¤Ý±ºÓøåèóœÓWÍº¦¸¹)Náéqè¡ñ" -Ì%ŸÔËG[À§Q7wHªM|€x }„)v¬Ž³`Ã\êçŽ–°ÛÜwQ ’l°2]ˆ9õé"º”µ—è“|ôäÊ?ñãŠÐs¿gëŸqµöÕñh	oææ{{ÿfæªTÜRŒH3kÎ\iµ'ÏŽ/˜_¼ÃŽõ?Ç_äSÛ„
4·wa(ýbNù,BÛ¼°ðXû×\D'¾ïÐAGd)”.Ly-?8¯¨s°ó¥Ä.wà£ÂS½%³Ã²Ðª-w9 U0!µ«èŽ¹bâzUšû6‚Ä¢—G›W6¾Œi¼¥êé§m€8x¤kESCî{4†’·3¨©Ê5×+ÎnóÖb-³ÄÒõlaµ¬@dÃ/ÓOÚaXôscî°tý->ú²âíÎ!«ÝÚ±¿°@mW;ãð4Ë3Û–™y´¤K´!Ý“:=èW4"{ë+²?k†_Bt)Dós/‘±‹ÞÖøò	Ÿk½.ê,µÊÚ[›/a}rÂ•»ÌuTcž{…×=òûŠõÍJƒZöØÈë [Vÿ~å'•ÊêæEè&xÙrmL±?ØÚü	­;¡_ïQw{w"Àü‹Ë\ä­ñ X€›RÑ¦Mîîðd;Ûåa°ïíýqn¼í‘ŠKK?äÏ((!Ûqài\+‘põú42Ã}Ë¿%´¨ÜÇF¨Š¬&V´cä3Æ#ÔÓ8ä›®í‡äØKjˆcí_„›j´›"ð#ú•~Æ’Â¨=.Y˜„E†žm€OÀ"±:
{™L!ø‡¯À°ŽÚ™»%²e‰PŒ‚7¨Rª;•<NËäû‹óui‚¸#1Ø|­NÁJm4œt]zy(oxl(nåFínŠç˜ÛãXëÝÁSÑPß¯´ýu6,,dÑl´€5Ž~@–ënÎ»Ìãvëez"&Ñæ+­öª¬ø{ï\;æbZPO7+Ü^$…Äö~­ýÊ}šÇYróKƒ"1»„ësaÉµN¹H+2—¯	ÃÉœ_}|Ÿ@FIÊ£xíu[LqLÆË›ž¬ß øùÛ÷	#7|™U»Q•ÕÊr~ÌÜrÄ¡¾³ce
tˆ‡UØ@R=Fr¦J PœPñÒšñ•¥WÞf½–[ƒóúk'·
,næ¨?ž‡-+Ó~ÑŸR8¦þË¸ðl“æœ™÷4.š~kNÐ¬¿`Ó±Zó&/ÆmnSæé(¶‰|½c¼){`u{« ÞÖ]÷˜ÚÇ™xåç0'³÷QÊ…ŸMñ½ ¥¾ñÙsN5%˜%×š[\–óVQ}~£“J*È2 )„ej¬JŽûyÊhØëžÎVõÛ°=0‘sç3Z‹&€Ú?uáÏû­›°ŽUv€7¥@„Ï<©ßÖ3¢Ð?—Ž>d®v
2«ˆ–Qý¼`¹QÓ{ê/)¨#žß3¼3;™ÓõwÐ“_ø	ƒB(·#hháj¯d]Ûè°¼ˆ	ûŸs·A¥O„.}ÂoyeuP1ï¨'åÏê¬ÁEŸ¸³SÑPß¯àò"x+13R—ŽÎŽ¹bâzUšû6‚Ä¢—G›W#ª£wº«ûôºf¬3G‘T<f53¶8¡¯Å[æ¥Ê5×+Î?öÖb-»ÄÒõo5µ¢HdÓš/ÓOŒaX¢~cî°™tý-B>úçêíÎ!øÝ‹ŒÍ PaG>öj>ÊE$Ø—ÙKLXŽÕM'•…Õ`žuPKaY”2ëj'“d&h1V*¶	c" *ˆ¡øÌíÙ$¥w´×çù4jã6>
W¾ã;ÊÖ`H+‡/œŸ8îçÂ½Í^Ÿ¾Ô÷X[©OJ·?¸j‰‚¼D®º:0‘ruPù~†cÆ´±s¿SúB$M
wgzYÔŒ¶¹4¢µ¦„Ì3¦¦nBÁ‡wàçü}žfÙÍ’ä\?1`‰{“‰õKÂîH\D—?¶Y["Œ&Ï%”{6ÃVß¿4Áj[é²ÇØGõ¨ôZ+VÙ"ëb5Ës–Ã:yáŒ“Ÿ¾@¡†Gbï;ù	ÚŒWÆ«oµ}öÈ@ž‚EÇìl5TÏÝI †¿Wñ\™`õqÁ\)ª(Á¯éKXþ‡Îš{‘OÒß]×Òezòi>Kçfk¤dæÎ‰´îUA¿†M\*îW·÷P_bs\Lw$yHZeòOÅ%–ú›Êø	Er„•™ûè0¦Ã»õÃ`3FdIhçÃ”râœ)’uß´pÑè·Q¶<"s("”¬}üï¥Â
²	zµj+€`œý­'ûãÖ=éJfŠ r°ZœríÀà;Õä
ô'1!Œ‹*Ø:ãµRDó>.õíÍ©4Val÷ä´úóS—é­µÅÏ,'>w” ‡qøÅL^áÏqßpÎî1(¨_}aa˜NŠ@ÈJ1UòX¿QœÂ–±…½PÎ>¨.Þ
ôÇtJôg9	S”¸+’—3i×'””^aâ§—«ÝM“ýœðC
}F§gýp¨¸cú¾´Luk‡7,\â;ÀsÂñJQ¥³O³`M&^63Ê€¾3ˆè½©»3¦‹DBÁªpœåFR°P¤ù¹óü2F9kƒ}†ÒÁK«hN~>ó[ËJG-Šjô4Ö$!´œ»
ž%œÇÊ½_k;Åcófš+Ý‡'£ëð·›¸·W;Ä.öüG†ês÷¯ÕFÑ²@¨÷."ûÕx3JÄ÷sOß^×uÞ|£LÞbä	<¸#¾ÃkAÏ——±n\àá]üß_v®4U¬_<êu Ÿüß°‘~b®9+Âz†`þaW=F?†/jQ&wZ^øBì¼Úº¹É!G‡ÇçÒUýºÄ«´ÉR5lfN0ÑÏ•%²ïmžiÓ²I›þ†µQ¾88;zJšÇØÌ“ŠÁt·{³2B&‹`‰Ë¢7°¨Óm»$…lTÊ:|=£½±Ï äÉ5Ê)&1ýQ†V”bÖjÉÑÜ=¸0?VJo®¢¢ž‘;úÎã×å+[fé=pžTC–ÒÇKuÛÃT7„ÝÇhhö1u}aH-Ãcìi'ÈB1Vœ{%i–µ‹ØçŒ­@ãÑxÇfñœ•"{¼>
W—ã¸¾\D;ßn‰¢t×Â„‘œØf£Ð…¢@¹LkNÔv¨¸c§/'—¹Ycr‡bpGÄé^àù{.(—5¨§fnw½žÿìiï†ÂÏrøî6J”ãâ°Êm¸J¡$è»Ì«iv•|‰Å’4SZÖÔ %/_—?¶Y["Œ&Ï%”"#í\Ë¦uÇGaÛÊ¤¿F¤™ôIiVÉcïdœòêr0¿Ã¦ÊÃ«…WÔ.Ùô3ÖÂ<ªù,èN¹ýã×Ð¼R"ÍÍJ» »(‘G„6Þ6"™1¬%‘Ìj«þüÌ»%Ï0äHÂŽmîa,(Ž&µ5!‰ é­ò\DŽ:ÿdíjå` n³,8/=$x~@u¿TõoŠÓ¾áÝsFÙþ¿ÂSþ4¯Ã »ˆn:}_L}	ÙþƒaâÖ0—=}³¶s”èª&±02M!•«D·³øæäs€v¢{OS*!‚ZwÆ6üñ›Ô[a˜Hwø>ìá’ê1qŒ¥1×<%ºåM¼s¨èc
Iü.*J»·ôv53ˆòáÞy¾ËˆœÅâ	# º3`ŽY~À‰ZÐ@2‰…“ss÷}-]\|
ºr†v&ÿHœNö×‘©¥qçÒx·aòöñ#H«GaüímÔìc9&)Ç‘F3ÓÁû¦ìF§È…„
Vö=°à{ÂÜTä“Õ9c„&¨rˆñB~¥ú¤QJ ZMST§´ÚÇX•«ðí™ZÍß=ƒÿð³˜?Ü.Ì2õã¯ÔFzx„JƒÐÞ
4Àí:ReQ¡8µ!8lØ>°=Ñ?f¡–ë%¥0”¦õ„nñË¸S{"cï\Æb™¸4êº”œº?èÈzÖ ß½‰‡aìË&°vìÐ4ÊYIÍ¯&2«ˆ–Q¿Wñ	Ð.¡!R*Ö7¸+Ú‰"Àåš˜±7îX–ß]×Òaq¬=+ÜX¿<N–ÉŠä‘ ëêz}/Ø]¢]ÉcÒ*[W!|pXtú•Ø½Ö˜¶A9¶ÍÀ° Ï™íŒÕh"|_nÌÄÉ2Ñã[áT(*ë®sŒî¤²z³q&M!•«7¦£ïî½nöWÜgn*
&*Ä=V¹ë†ÎÀ´	é~@°I+ª]’"BÿÈ‹ã;4É¬sŸ^^sŸËm—¶È{PÍ..F÷»¤RÄ YJ6Ýöò²›(ð‘Êœˆ°PvzLÆ'{”I×.«Ó]M… wÌB“Ç"Bç86@Ù‡?L^M9'BW­&i‘
¯Š’ÖØ½‹pã|°bŽüò3/irýðjÑÜ;vÂ~’&¥wÔÂ‚š™ÇCû”‘²W¨(§môDÇÖ³a@ú.µCre‘o?„B¿«Ð7(—Jª®LL34*>{œ±ÏÒfÊ½ûïê€Ø„å¾ëø}iµE± Ïð´ÇtR0.íîÜÏ	Žè*îTàNG*ž2àÐ02ýÍ¢/Ÿ7È¤ÃÒl©–ôkÈ%ù*Cœòê
SØÇùû—è§qùòÅ+úèö[‡zì”z’U4ŠÔJ¬¢-0¼#Ñ0œv”oõ#e)À?–§Ÿo‡P®ÃÍÈ¸OÒß]×Òezòi>Kàbk¢l	…ú¾üŠ<*ÂÈ_2ãf§HÜE *Z7”Lx	9z}qš=‘{ŠÓ¾áÝur^ÛÇ÷“ì·Ó©»› ]"ÝÐaézÓb/Û¯w¦é•µh‰n9B{ìÀßË” è#ÿ r¸:^ T_?€`œùý¡¹Âd–'Þ§R’{F¸µÙ€UX¼^µXs2˜ŠjÞÒœKmL÷$o¶²’x¤~P3¢¢òá…T”í‡œ•°%%¤<g—Ï!Ô‡‰XCÊ•"c[¢×{iòC<[YµdÆ@ÈE&h1V*¶R!jÄCáÞÚÖÈüE±Zˆ6Û¨¼9t§Pk0ëõ0¿½Xnõúið<]’€­”ˆÆG™ë–²F¸Q3Ô‚Yýí;¯GXû1½Yw,™%½gÌ õQH´xÊl¾2#J ZMu×ÿ”œ‡ç²¦éƒ„KMÎ¥R¹§º6þên¿½îŽ=T6:Ï:Èž…DD\_‰¤NN~aKµAòhÿ8Ðq	Þv÷‚
³)´˜çùl‹¢Î9FókêjÓ.§—'0¿Óûíž8Â“>P¹:ÄûŒè.ìÍx¸zë¯pŸÖ Éá0gš¥.4ÃCÜ)Àv„.î ^"Ü#;¸ œù)Ôc
ùÿ°fßXŽYßÌÑ^qîP:e·4ióy Ëù ôŠ<*Â”+îZã\ÁJpp%Ò7RJV-t}ZrâTûn¦Äâ¥‡E:Öþ»•ï9¼È¦©Óx4ze[o@…È4öÕmËtN·Ñ¶É¤šG\3hˆ¤°¾·»Å	ÖC·id.7ÌC$Í‹ô_—Œ÷G¿GaÖ.‡z¯EOÎ«§Ä	™êñ:ŸÖwŠZŒM3Dû#PãæÏt¢7^WQà÷´§¡5ûŠŽˆ I@}O´_óÄ<ÛˆÆBH×ÃcÊ^žÐnà%dKKaµdï@Ètämîj¼ó €ƒ²‚m½|°b€˜–N7ÆEp,÷öÔÂ4o›+Ë^ÈV*‘‰¶Œ´9úlè»’'|Ö"(Ä’E…×¨M_Â€C4)ÒOKu´ê.çŽY
²lèIìhxhu×ÿ”œ‡ç²¦éƒ„KMÎ¥R¹§º6þên¿½îŽ=T6:Ï:Èž…DD\_‰¤hl[8óQŸcp±WÏñÊ{ð”³+¥•ñðfƒ´Ô5!È*ñwRøbØŠi=´Ï·º˜†})ù Ÿ¹½Sƒà^•ZÝ£Z¤æJ Êàh.š¥.@» Ä2‘MÐAÃsê.Œ¥ôV®awÅ½µ‘@ÿÊîüÇqék=hø},©¯U’ÉŠ¼;[Æíy6éMûvÜ*K8›HZP@=t~VA2§£ß¾ªüÒ<)lË¸üä4ŠÄº»ÆVa-]w±Ôˆzð/árPØ¿Ô‚¦h´:w`Üê+îË”‰Å	Ö#U’S*/:
Ìz™Ç·{¦ˆðD»$—vŒq¢STÔº§ÎçÀVú7=µÍv—C€ÑG}Oð.&[ÉâÜ>«yUZgëÝ°´˜?ÁˆÂÒœ­]}^KÅ[i÷7í<Ô‡
Tž‚R7™[Útiõm4%AÚH§zo+X)XÓ3h?²*¡ìÜâÞ/¡P„æÙ¼§JÆ>
#“’¸Á:ßëoæ7WŽ¦š¾4æwƒò°„,wÓ( Î‰N÷í,lnÊ:’ci‡6y#½&‰=Žá
²õ®bŠ˜€šîë:;¶¹B¡Å—€‰|èâ-=²Ï3Ò üwR´\ìz˜Ì«ovc|ƒ‘å)/:9ùÙ$ 'È`êi|§ZÄëÒqê„Ò8Ž³–éGížî>q”l°µ«o·MRÀéh}KZuÞÅâHWºFØüÀ¸‚u¾wë™@…×Íéhk‘“)7¿_Û3Œ¯z¡-X/Ü¯3œÄDÖ
l«þüÌzØBË^¶ HƒÜX|îfr¤4PÀp…ä½ä
-Áî}~VÈ{‡`á{,aÚ~[AQ14(>v>ñmýøÑõNaúÛÖ»)ÈuºÄ«´ÉR8lt\uÝŽvàÙpádÜ÷y›ï‰ºQ°(0t(,„¤2èû®©Å	ÖÜC \P`n” }“Ö©'ü÷Ž)ÓLr™I|Œq¢STÔº§Ìò×+À= %®ö@¼q¼òaoÜ=nø¢€cÀ_rJ3ÿØÌô/ñÃœúÒ$>œF¥u­O èótu0êî"Qðg©öHBÕM3?jZÐg_Bxnó
OËÕ 6IVWÌWÔ¾x€öŸçJÆ%qD×ÇX÷ùLr‘*ÌS†EÔ¶ÿÏ‹	ÚFæó¦Ÿ!jÏ%#Æ‚Cìá>•ptÁ1ˆnIF¼(†HÈ&ƒêzéc®§ZQnxñ›ìYe
t‰HFÕ¡¡oëƒÿæá!i¥ª#ôû¨þ}ma‰x‚Œ›[P~z¯þ%v;îÐNV%‘Aò$Ò7ªãðXðn$‰©Ú‡’ƒ 9mÔ*ÀaÁp§b7ñŠ¾¥Œ8Âá>P¹ ïÑ%ýè	‰ì^ôzì”z’‹Yd®‰_VÇ€kòcûªl³KÈcÚ=¤/¥Íj”JXý–ÚªmÕEÖ0â
ZÅŸUzãf1O³'PùCŽª©ùŒ	ÆíyR0ã	ˆPg_#›yACA(ogWcâP°)šõ›ÜõCvþÜÌº6Þ	šô–‚äYFF}R>ìî¯Höìwœ/Kx°ÒkôÞßWy=XoÎå4úé±¯ç,ðC8ù5LSP`aË];Òà}²ªÕb™(ÑNqìÉ52¤ÇÍ¢p~žªQºW|{UÅ
î=ä¶G)T÷8!ÃÓë 7|a_÷ç§¤º8ê…®éæÒ?/ž'wX }€Æš  y´Û~Ý²ñ^TÚG5?aö)¯.k·k{'T2UÆ5b?¢*ªïÛóÛ. ]Œ>ôCŽüò3/i0úýs›˜zÒÚ7ï‰Ñ³$X$k¹YË¶í”*gÙ>9Ô“Oúâ-”}k×9ˆhD(€,%±/Û^àù{.Sò*£FŒ}mce¼þý~á†ØÖ•néî =§ß3ÕÜÚW¤M«5¨Íµ{}p˜<Èžª£ÒÅLÑØ››«XÞ²&IAòå8Ú%~ÃFž¤7ž8¿¨ÛÂG¤šåGi9x’	cÒ–—?¥—ñ÷Ó\¡·Q/Ö)ö£XéŒWÆ«oµ}öÁ)µÖBÆàd?`ÃÝB[éF×3ØÐ>ºO0BÖ8¿fÐ‚oƒSA«þüÌlÙMÍOª_LÖÝSl¼R¡1[ï!LÛõ°öêiÿ¿@G¡U·ÎFOx¿G{FKHY§Å$íŒï·‡E:Ïíý›°§Ö¢ÿ›Z	g^9ñè¾GÁþ]áV7;’²xî˜ú5ßW_{h®ˆ_œÁÒ@š%A“2B&‹`‰Ë¢7±ê˜)ßGXœ8¥‰xyâ½ï*7×ÚðziGÚ‹é>’±(X ²j(WâÃä¾!	e[}÷î¦éŽ:ó”‹šÚò$­'tœfÝAØ—]_×ÙQ'ð¹=³8uPfK-Åcì#â5!~n¶=n$­*ž­ïÝóß:ºJŠ(å=´ÙÈPŠ5cGÁÞBþã6D¹çhû1Fö¾›«‰^ìVæÕŠ¿[¹XI1†ªÜu™×Ißô
’OKu†D¤£Û%akÙ—!`of‘¾ßÖAóƒ»3¦Ý…ª2ÔÌÜPiƒa„ÏÐ€õF)@J¶Z«åë,.,6óÅ"5 Lï\ÑX
Œ>ù?Ð4>µ‘}”2ÆæBqÉ]K£VÆN•
Jªñ–n7¥Áá—ZªŒZ3ï ØáÝÚ?ñ*ì(úŸËœÛyvPÃÌMTéHÇ;žrÆ:Üy7A¼	0£2Ë”@”^ò—Ÿ¬PÓYÙ	ßYÍ“8·4UØ1^þnNÁäð§ÜAÆíy7æKãˆT
&F%É?,9CcPqª¨:å…„ÍŒ}I‹Åµž¶—–ÿçã@)0.&à‘ÔL´„D‘4@Nôš4Ù°íÙ¢(8{}Ãœ9îË”ÂÕeX”^*i#,
  -Dºìç;óáŸ5Ÿ
ôh7¥©XYÂ¡½ßôÌ(ÐP<6ƒË\ŒBÈK<E¾%-Vÿéö:¸3Qlæë»žœ,ø‚øÐÐþIpJvæ_óFéý‡MÊî0GÊ_ánæt!X"IÈBÀLŒQB=slð)=ÈSíŽßçÅéWÞ7ã|°b†˜–:Åg9v1÷ü+ýõ-R ìð;F™‚¼€¢5çc”è»!nÕ(6À	‚Mìþ>ŸprØ ’nBO§j4§rØž¤JAø;îIì‰è‹àåì‹C*lòqÅ—òË4¥ä&&¨Ã"ÅÍÛ]ic‹ÖÛõW)1|Žq‚ÉƒPfy«8*DÕwûe{ª[ÑëÛ{ðŽ
©3²“ñâz€²ß<GþÐMBÒ–—?¥—÷ûÓP°ÁuŽe4ÛõwXÍvØ[ý‘¯h`Ñíc"þÜRÔNÛ4§F…zõ+W.ˆNiý}÷¨•W_äË©±/ÞNÛé ^ÖÕ\C±;+ÜX?êu Ÿõ¶›ÅO©ƒ åL¥O÷H&lÒ'(>@~{R!½èh¸Çä˜¶Ar[Þê³Ø_ý(»Ï»æ­^
!lÖÈ•)¦ã|ÝiÉù6Â†êÚ~±0&/UpÐú)Žž®±3ï11ºnXf>Ot§?DÍ’ÿbÆÝÊ|î7£Znáë†âëÂøÌ!Æ=%ºåMË^‡ØG)	©GE;ÿæ2~a\ÇÁ—ˆ¨¶‹ÅØÜù?"2»;ŸÂs–ÃÈDuËËL€vÑÜxyúq
'BçƒÕK|í&iÄCá™ÏýïÐ+jª#ßŽùèog¬C,1üübÛè7}™b™ßf·¿ÞõÔ]¾(ÏÁéÑ.šzlŠM«dÌÖJ•`i×<ƒ~ST¶;€H¥¹Ð9tzÉ
Š#œon	up½‚äõlæ‹ÉÆˆÙÂ
ÙÕ	¶áû}S©ên&S	 ¥Ù¯º[ï0a²fy«hl[3i8µZ]%œ>ßðÃ`ûŠ¥"§Žàólž£Ï*]ôÚ\,òX»·HùÂ÷ûÃ—œ3Þ+ÈíE›˜ìj»k±4_¤uÈ¢}‰-2«Õ)7» Ä2‘MÐAÃsê/™½ãL¿}~Ý £ˆJî|ø!ß9uïçJØI^’yÑ4‚ªôüîE ¢‰'®ìŸiìÞ€“‚?ØÎŸðˆ¢9>×I‘þ–Êï_wàÈÔ¼ Ë™òŠˆd9}eP5Fµ«œŽÆv×bP:øŸ_°Ô²–O–S$¢…V†“ÓÅ—EŠ"T‚U-	!:Ü-`ÝË¡6øýÓb™˜Æ6À–à+Õæ1,#2R[Êˆtœ::ÒýwÃX‹ØK8ý?;FÿíÌeÿeK3‹÷»¯ŠaÁ‡Ó×ô	$($º4t¡ÐAÏª«$_ÐÙL!æ_ÝÜxyúq
'BçƒÕY|õMÄIáŽÉäÜ¦vä»ÛÛ¿¼7b¡G;S”•»Ç6	kÂl™ŸLS†ûÇÍRîªÈß* M%Êí;•€1‚ÆEèp92Ùy=ë&Û^àQKô7úD•J ^JP-·’ÿúp÷›ØÇ‚aüì*Jˆäóð¼";Û-¬'¸Ýƒå[2FG¥V¢øü.CÂâ>>;î\ÒIj—|ô8Ý$éTØl§K>‚£Çßl¯‚æeØ-¶>W—?õé"Ü½—µ—¯GýÞáæÒ)¹ÎnôcáoÆÍ ÍíR4JÇÎšZÇ3‘]Ü>¨y7A¼TÇ;÷¨êH^ù—ÖŠMõeö0Ó:hüútRÂL_ŸgÊ] ¿Ôîag…¸6=Ó|‘vîq1{¼?@LZ=%LG\hùSýsü—ðºœöàrY,ÿuöPß¯Â×Ý ]ooAx]÷ã¥]ÍïKûG4*ó•\¼È´œ@S9¨žBŠ€ËÎ‹W"?£sTD ;np˜Úær²ªÚˆQýÊ?ÞSÉHë†âŸ€U\»óüs8®ëJ¼d¬è}Nõ#!WÉÃÆ7¡4CaZçï¢¤Œ8ê‘ÕÙêÀ'( †F¿o»Z¡éâye:–ØyÍ¢Ç:tý|0Bn—AP¡æI‰Ô¾ÍðAÎä×*¡w6§ƒ×qçQš5ó"¤ÆÙ	S:tNØÑGæòLr‘*ÌS†EÔ¶ÿÏ‹	ÚFæó¦Ÿ!jÏ%#Æ‚NËÕ´GrÝ³Awn¤v…!‹ðV@Â’(€kf	".4PÀÏ“PÆ¬øñïƒ„KÔ BÜ!*#¶+óy‰'\ÚvníîÄÃK/Ò«5'ÅZÊXF%•{­c„a}‘8·¾<”i¿³ŽÙQ©žé%9|Û%Ä5^ú-õé,¸Ùê¤j¼È\8Ô&ÄÊÁÑ<“Î²3¿Ð/Ñ¯h`Çìl$‹ˆT^ÛZ‰Pò “fà0a•,£/Ž™ê7AåŸ±2“HÚÃ]˜¾0÷l5`ìp®,\°œ°ôìL¿ÜeÄt“˜OwaÂJr-1qžxÚOœþ€ÆùUlïßÝÚã9°Þææ­^pf S?üè¨KÖõZûO&0’´tâ‰Œa§)"{%Íìt·¢÷é—aª
q#>+'-vˆÄ©ñà”d‹7×lTÊHë«ÈÂê:%òÈ.æ!™×o—™ßO!ý(+[ÿßË-«3W~÷ì»µÒi·ßª¶¼ímG
KÅ_ŒRxÔèãic=áþ8CæmˆÖvBÇy;4cö1 )b¼}y=W#DÕ&c.Ì¨Þ±óé[‡p¼ÂBÔº*†ßn¹þÏºL"Å	º_õGæië7A˜·‹¹9ãeˆã¢’$wÕ%Nî)¹nÝ›Q×(P“ªBdÉ8€O¿¯Å2{cè&«9—^G,ug¬Ÿõ›LÁ¦é±™JŒÂ
„òT¹§#Øþp÷Žt@{o©vníîÄÃK/Ò«5'ÅZÊXF%•{­c„a}‘8·¾<”i¿³ŽÙQ©žé%9|Û%Ä5^ú-õé,¸Ùê¤j¼È\8Ô&ÄÊÁÑ<“Î²3¿Ð/Ñ¯h`Çìl$‹ˆT^ÛZ‰Pò “fà0a•,£/Ž™ê7<šú†°aØX‚M¥]˜¾0÷l5`ìp®,\°œ°öìL¿ÜeÄt“˜OwaÀJ|&#G>‘ß¾×øXlìÝÝ¶ Ý›Ž¦³Äh/0;%tÑÇÉ[ÆôP÷Y"0ùž_¯ÎÈ¼l¶4?M5•¾r¸£²¦§a»~‰xF$vÈv™Ë¦ÿðœ+éBbHcþÏ‚“ï‡Vq‘¥ð*™Öq±RšÉN%â)"BºïË<¤<$UKhä®âíŽ,ðÓ…¹[@}O²_‡0îþª«[EÚ2Uýb´ìIXÇG
"PÙ ’?~¯vt$X:^Ó s&±-‚ªåÇ„è€jà.ÎK’ûø"oæE¶ŸV9Øx±¶¡Húz¿ÉaÆø¾ðlº/ÌÁéÑLùFÈ™Bìí8ŸciÁ'¹Lutˆ,5#S®¾Ë8Pô-³^œK	.D3""×ÿrñ	PF"ìÐ±G6¼W7˜MÜ.ÈKíŸË¬vc|ƒ‘å)/:9ùØ$5-Èjöbb¬AÁ$Õ?2õFÇ”½)¦ŽââvŸ¨Æ/JéÑ+ÎxŒÛX-ñÞðñÒMáÇçH‰UTÓ¹½!Á¨“QÆ¹Vµñ''àÛ]qþîmqâoàu‘G”kùkF¿TÇ0•Ë{ÀqsÔº¿š]ômû0Á6nüútHÒ[A¤3Xëu’›Æ‘ÿrt„©X6éMûvÜ&K4Š>
‰±ÛëòÃL•ÜÂ¶ûÈpNGŸ÷ñ–äºÓ»©É`2$2,Fµ«èaíÞm†YUÕ¹râ¿±{´;$CÚÇÜ³÷î¼2é3cöuD,‘r¢Îª<½¸Ú}#÷6¢AÆ0=û‹ýd\»Œý!tPÊÒv^‡/[-˜C?GøéÝeï>V,¢¹ßË÷)ý‰×— Q}E—b!¢ÖAÒËÕLŽGTô{Íƒ*/È@eC3qåAÔPØoœ{c" *ˆ¡ùÌâÙ$¾Y–YÓŸðän,Â`:i8¶×Iöþ;Iºìbù; ±©–¬”9ÜW²È¶ÿî.‚iÐÔöMOúµrtu$¬hÉn×´_)/”8ÊlÊ	gx ?P»ëÀMÚšøú³RØƒŒúQ°ç÷}_¸{§4öôš˜OW)9`‰ØÞG[ZK«h\D—?²&D#‘z°öÕmíŽ¥#¦öêßU«’ò5}Ô"ó\:Õs‹r+´èÎÒùjŽ¡õòÓ9úôƒì2©zì„'µÖB Êàh.Ž‡`mðcäJÇýúO0Bú>„¥ñZ®znÀ´¸šIø`ë*ÒWUÍ×_`®4U¬_<ésäÏ†õxi”µ55Ø}“o™`.I4€r[Q](qLcdäDíy°è™ÀòE|ñ°æ›ùeŠÒï´Îi2a)39èçÐÅ=÷³x°Òcå”ëQ¦}9p)ˆ•t ²êé¥fâ]'íi)	!:Ü-`ÝÍ­6ôì¥/ÃHa¥WjžëþŒùngîñ^ú7=µÀv…Q¶ÐG? ¯j	›Š ;¥4	.òá¿±ÅT”íÄÔÔâ@gW6º<{Žéý×ÔCC ƒp–Cß“!™%1š^×P³`QIF8ËT@ß£Îš³ïÐ+Þ.èCá3Ë¦Äw%’E0e®º0¿½\mõGæië7A˜·‹¹9ãeˆ”µFè^]ŠMÔbÃš%˜fd×*ŽhUE T¦d„:ƒÛQQé*ªïPK#C>8Qž¶—Ü[Ì¡ôÖ¤KÊÍ;„äT«µ¥0;Û-¾Lš›á2T6r…a¤üú"Ãø%fùXÏ #šzù8ë33úTåòÖW}²ÀßGáÇ©AV´>’	ªõéq6¸Ó¾Úõq¡l	õòÑ<ò…¥Ä|¹açžkƒÃ6ôöh%LÔÚAbæmü¾`¼ZÄeà2™	´Šj‰PE¨¬‚åfßHÚ©Pçã|*V¶êþÜXM’g¤†ÄønsŽ©4+Àwœoáh+}y›yQ@L`&9A÷b±—×øXlíÝÒ¶ É“”‹´Æk2keFhÙÍ¾DöÕjÍsåC·È´œ@ú(?|x¾¾;¼¨úå°*ðCS†\QñÅÍ¡Cî	÷«º;óñÂÂhœ7ªl‰oiÿèb`¾sŸq&=žš\—†ÞF8IÍ(:TðÛŸlnv=7{ëì¦ðÈêÄÈÞÑù	3©4M–XŠ<É‡‘'~·ÎvË×“j~þhn}aHGÐ”@ÈH^x1›x%‘¨Š—«ÇÍÌ>ã|°b‹öñ7*íZ0lÅÀ;ïeVFÙ"ÝÉ8îñ±ˆ­Vƒôç¼ë[{ E;¨uí0•Q×($—xÖ$YB­#šC®µÔ1jô-£»J ^.<·’ÿúp÷šØÊƒwúîLƒîÿ×úlB¥M¨$¼ý£Å{Fw•sÅ	ùé7ÿ[–|ÑšXÏ\L•žëzÕ 4”„ãæ@¹…Ò(|Ñ7·s	ÊfÔŒe=¸ÞÁ÷ÂS®ÄvÀ:Ãü˜‡s÷¯Õnúbç¨oc©Ž{9VÅˆkòcû«l¤QÀ$N.Ç(+¨'Ž“@³}cÏš˜±nÚIàåLÆÁ[lò}=OxÀZ'§Å–îwu…¤$;¯LªGÜ <8œsP]f2©ÀÈ§WÚ©+†ô(DŽéý}.[ÿuöPß¯€ŸäDFSmR9çñ¦ZÛöPòR5'’²xî˜ú5ßW+}6Ž£õ‰ÜÄJ€>R—^<a*<° qœÖþÎÆ¨ß@sŸ
;œGƒwmîšƒù*#ÔÚ8À6 5¤ŠvŠY„}% û$+Wî©†ww¦Ï‘þã³¢‚¬äf"_rw&¨@}=ÂXTŠhÌøÕEÚÄHÖ^‰Çspö%d@PL.±ZÐg_B~nÿs‘§¥œ½ñ¦vä­ÔÊ£Ã37 Wi7Áú~ÔÑ
~‘n”ŸEÔÂ‘­œ“æÌ‡¶}oä/¼y…˜JªWC÷ç 	™7/½;Â7ã5¦NÍFÊOJ*ZjejÈâàë	™­¹›Àß_ƒºM¬ËØIá÷È¢ðù"KD?Ð;×‚›Y^^B‘¸ZQsui<ðnv±AÃúÌuðã<”4˜î•»9ÄžæRKù
Ö\8âU½ªQùØüñÞ\—‡F.Ä&Àð@ÛÕ>¥ËE¾fä–3…À ÍÛo#YÇ÷HXØ`È ýˆ97A¼\yâiÚÅz”}|À€‚·]Ô_ÊôWLÀÞJ4¡`:t¸;iýiŸ¹™»’+«	ê¥.v5"[?(>?rzW\NÕsÑBŠäšÁã	wGÚêÇ€ì)çùœžòR>wtIwçÕ„yôÕkßrß„F¾Å¿•Gž	@(´„X‰Ð¨½j±/‰nCI*6Í];GQ{ô]hÈmåmUC™˜;ý$¬Õ†â™€UXþÇ:Ü1?¤æ\³p§ãdlÆÿîÍ=µ|@:4sˆ¤®—=¾«åøüÙ?2‹SœIn ÕÄL^Fáþ>EæB“Ç{vöG!1AÊ’\gm7_5^Ù<µÆõýŒôŠ{ X•DAf(á×"XSÌ%ótO›¦­7C£P¢t«¢œ±Å9Ç©Þ€²|ÿó.°n˜ƒZá/'— µCb8¶6a«b‰:µæBCÆuó;ø2#N_>).ÎÏ“VÊ¬ôà™\ÙÍ=ï¶µµ)Ü.Ì"øó³8[iveÜ¼ bÔ«KA!;
óA„!8Eˆkþ8ÀldøWÙˆwÚ\\êç§ÆP ‡½Xk8qÄ[Z–NÈÓZ½Ã¥óÕøÒpà¥^ïïjýú*ìN¨À1Ú‘S[‘©?aš˜» »ºm¹GÞñ#	ŠÒËq„[I¹ÈûÏØJ— Â;uêìh]ÄJF”zYýx‚›ï±ÅEI¦‚\=ãPªvÊQ.>s\Lk+huU#mòY±6è‡ö˜¶Aj>µ‘³ÒP¢}²Ã»ží]$mrzyÍÍ•<ôÓtÎ*VØ¿Ô‚¦h´oO[pÍºn»¯ê¬ø*äaßgn*rECî{”ÆïßÁ³é}Bª>¥GƒhIùƒùnâÆ,Ê7& 
ŸÖ\·R„ÌG#Aæ?=WÉÐî’2{jKÐÝ””°Ê«é”Àù9O»r{”YdÝˆŽ—ÙÀm&™x¾á©ë,Â›²Ý‰)²ëj'§zo+X)YÓ<h?´$¡ìÀàÉ>¼6±Þƒ¼§J²g9v1÷ü+ýõ-R ìjë!I¸¿—·©ÖC²»ˆ+qÏ8ó7¨úæ·RHì¨Xde¶2ŠR¯ÂñJQ¥³FªQM"R~huždsÔ™èÎÊ”Z¾¬#<µ›‚î<ØQ¬/äªžÕ2{q‰|¸ÞßÃ¶PQjCn“@ÖBFt§j°>Ö5/õmÜ¾3œ<gÔš•»9Ä‚é/8‹ë#År‘ŠX;¤ÑøÊÛP¦Èz€t Ÿe×ß;¾‚0ücáoÑ¯h`Çìl$‹ˆTHØCÆfõ#ù~ô,S?ˆ^=¨!¹€?Û3;°ú†¦bÁ_±Oøë
,ß?kM÷>B¤zÛôº‡üaû×B/·››yUxaÁ&{* L#¬:’šõ›ÜõBvñÜÌ¢8Ãu¼È«¸Ø$l
!uî£PÍùFìC30óSÑä‚·g»9g4ˆ§~ù©üä¡j€b°|OO&'»a›Ä>øëÓp‹.ókW¸>ì=«ÈÍ¢84ÅÆ3Å'&(ÑvCÁÌA<P¾l PòéÀ¯ Qe7ß®âíŽ,ðÓ…¹[@}O²_‡0îþÑÎDNTñï9^ðt®öNBÒ~!VÝ’Rq»{y=^~æX?°¬ŽÏûÍéœ{‡!ý%¤ÓÕCŠ8lFÐÛ_ýù}r‘:‘tÛ^a°¯—½ÔIœ˜5B…°$%Õô+´|¨lê ”Ñ'O«5–U¥®Û&ckÂŽ*—z{oC9#?\Ñë¶¹D¥Âëæ¯ZŒä&&¨Ã"ÄÍÔ]iB±$âÆ³Äs+J³VµîåÒÔ5<>èTËY@)§XÅ÷	ÒË¢;Ž_”çÇØW¨©Ut³N÷9Ã‡•6« Uê'D[uƒ å9!½E¤àÚÃb“Ö:³qæ™vµÍÐí`3˜0¼#Ç4–]ÈQõbU)Ñ0’$Äy»ÍÈûÏÄEÑ±Ic×“Uzãf1O³'PùCŽª ù‘Æíy7æKãˆT%_jÿ<F\(o3\qâYñîºÕœÌ=ZË¥±–ê÷†ôÐª-^icEl@š‘Ð$¶ëAŽ2(E‰†0•ÿÛµlâmlQ:Ñú+çÆ°ü[ï_Jü*MON}{É@#Î™ÿp¦ˆðDùlC·(v¨^BÛ¯¬¥6?Õà¼C^^sƒÄ+¬u­õkr×	
{ÀÅ7¯1^aa÷ö¦¨“<²‹ÅØÜù?/ ©~}˜Yu«ÅÔKL+ÒÈ>„ÍššcXzba—AÆKœ{f2bxÄ_ˆéŽÉäÜ±]|¼Ó4™àú!Lö70rîíeÛÃyT2ÄC£"Òs£ÆøÈ÷ÜK¢œ«™,jÙ(5Â‚NËÕ´GrØ ¹AIP®:€J´¸Û0qkÞˆ(ëJ@)^o.‘¾ßÖAç²>dˆžª]K²WT¦;Û_ÈK™Ýƒå[2FF¥[£îú,%,6ïÇ5$fôQÚTwõí\¾')õVž„¾.¿•ë÷w’²î2gØÙvËI¾¶I…øÐ½Â\¦œÄoÄûÑÏsã­~¥-Kå"£¦esÌé2«Ó)7¿_Û3Œ¯z¡-X/Ü¢3ŽÖv[¡ÃÍÈ¸YÖôGc×“Uzãf1O³'PùGXß™»É»8U¢‰EiØMãFÊ@!p3‡qSzX,s3\1¬:’¶ß¾çœb1CÜõãÉr‡T¶Î®¯€'wiuFu	ƒ¬ëôÅw×rMW×…%ÖãÂó5ßWv#Œº&÷ö¯°úX‡\&Ž*S}Z;u‚$Í˜ŒÍµÊ}‡u_ÎPó»5,»ÓÒ¯d\»Œ1×<%¹çM§h¹ûlyIü.*J¿»¤RÄ<qLÆË›ž¬Ý¡îêð¸//¦}Iu™ÂBH×Ä"uÌM›Ÿu÷q</	4BÞ1Š@ÁR8{ Ÿ	!jíCáÑ…©Ëø­SˆÎ©ðï2{»BCc3î´-ÝÕ1r ,ÜMÉ%1ƒçÂéÝK²ÔòS.š~ŠM ¯¿`¬MDúU“oRI 9Y¦!ƒçMJó“	±N\"u {¾™÷ç|ú—ÛÜˆ}øä*J”ãâ°Êm¸J¡$è»îŽÖÝ«Ô%±YQ§a~z¯Ä#%;Épñhm¿PÏýÙ`–¢;ž)Éü£¼NÀýö2mÝG#îIª¦FŽñìüÔA¡‡]ä ßäÑè™ìYˆ\ÌØjƒÌQûð-?QÅÍ\™vûJè5ª?¢7AÎqSÄ3“ÌkØaE°œ”¡fØsÐôUÎÖ)·4UØ'_ñhV´° «ÓUI¢¸9ábõõrB&$›yAi3\eþ^Çi Ñ¹ÊÐ-‚¸£Ér‡Pß¯¬µÁw3 XÈšìÓqßtP_š«c—â”èØT&g)•÷9ðä¾»Å	Öt»j^LN|¿*$Íÿé>é¾¼ŒmJ®Qnó½B-»µÏ¼mdŠµ\®uY]5¨æJªhºùlÂÿîÍ=µ|@:4ëäúŽ¼×­øîðÓ%!ç=pžTC›ÒÕYCÛuÝB”ìxhõ~y	%MÑ1„HŽgG_7«Bmîjºó €Œ½[Á1¿Î(¶Åï3e@u2ê°{ÑÚ%H=,ÍBÆ!j«­”žÄ;ŸêÌ‘µW¼GO¼J×Ø¿`§/'“ªBdÉ8€O¿ ¯Ð)aKú*©œqL3u2.T¤äçVÚ¹èì™nëå;$¨Æ)ÓÚÊ_cŸg‘Þº´È|!J”5ŽßÎZ_‰<Ã×þõž$ŽuÈº=º.±¹[=‘8·Â¸#©˜ýó}‰¨Ð=Vû
ÓW/õ>‘c<©ž¥˜½HÅâE5Ù+Ú.ðþ“ð_WÝµqËûÊh"`îÝPMÃ^í	—[{ä|û?‚Ò×vŽJ	Ï‡Ö¬aÕIÇF¯P‹•£}±ãÚ5áÀZ¸ÐÉŠ¿¼;)¾Ž gØMãFÊ@!p>‡cALY, "G>íb»ÃçÊÈh~QÛñú­ø;³ýùéý6Z	]uÌ×KðvÜbå¹cŸí¿¿k¼}k2G{ìÀßÏýè©qÿF7¦yPGrEG‡u‚åríð”$Â
ôh.¶]n ©ÍÀ¶R[¸õønvJÚ’0Ël±Œó'?	ð°€wügCˆ‹ßË÷Ü îõêÃ%3Ÿ\ÒTŠx‘ßˆ'~·Ä?öi¹úSBÁ]5"a„GgDecû †¨—õëÙûÂqºÓ¤÷é!qVnw£¥;›º_m`òD ^'ñ¬—±¾+ãQ²Î¶¾VüN÷$°{‰”¸FD÷*¾Xpf²{Pù*&„í´xÊlÊB$MWZMW.Ž¿Ò×çŠÙÀaÿî0=¤äÿæðFt´J¦)Ïß Óy$Mz’d’Ôõ;,=/àÂ-5;Ü`ñof¶6å8Ú%~ÃFž¢;ž)Éè‘{X;€Œç	°$ú½Ó\]çTÛÌ“Ÿ¾zŠ¬zïèÛ(ëç‚ý\•_ÖµMÂËÁü$m2«Õ)7ÀEÛ9Øf²JÈeð=’”ÅvŽ[nÒ–˜¦gîaÞë hÌÁKmâPEŸbÐRFž­î°‰nTëŽ:ÿì†Éç”«%ùÖ°Ê¯öØ¿w:’pØ½ÖàÕ&eàì³é4¼ù ¨Ôy>te-Mƒ¬ëñÙwÊ>/š´tâ‰Œl§;0IAt¼ñßÏëé¦wîZH¢:B&†r›ý£7ó¥Çm†
ôhWÎ9ïyuêšÂ§!Òè®uY]‚ÃqÃÉÌW?IæqB8ŸðÜ6¤!F,¢¹ßË÷)ý‰×— Q}E—b$§ˆhÏÁÃi9îM'‰p¥ƒ,@¹({DPqƒcìiaª|b+N%OØ6s0£-é—ÄíÉåRÕÒx·aòüúoX.zIÌÝH÷þ!3,ÍBÆ!g«¿†¬ŸM¤Ø²7Aå «(¿oÌÛ5¸WKø*°Hx)ÔqUÄé(çŽ½lè¦Kj
z$3(v£ÎßM€¸þä¶ŠÄˆã"ôýómèyéq¼â´Ï{5%É.ê»£fy«lk'!þðnv±AÃàìWÛ¯
ª ¿çúgˆ¥ß<GþÐMBÒ–—?¥—÷ûÓP°ÁuúfLa¾ÌWÆ«UžWË¹@¹ç,-ûÔJ`çáhióxš4–M•v¨y7AÈqS»)“Æ?¯|uÙº©—Jðhà<ðYÇì|MÉL_ŸzCörÓ›ôäØ_D®ŸY[p4Œð¥.E%X‡~[Qi3\eþ^Çd Ã«üÑ-,£žøvø4»Ò÷‚Ô-8{dAu"ÚÔ‡rß†-ã=}³®—ÿÑåQ¦}9p)ˆ•y  øß¤f±L*ö*X-vv,&… 4×‚¿1ðõÁ@¼'d– ,ãÆjhå–¶R[¸õû'iXÏ,‹É‡/[)â)"B«¢™iýg o9Ú²äœØ5êßÁÞ… Z:ÿb"Èf¼,Ãÿ‘w[Œ˜K,‰	Æ¾ÜZ9"wý ¢?~¯voî[mîj¨˜‚ÆîÙ2§AŠ4ù.²ÃÙox­ZiñíÆÞ87,ÍBÆ!j«­”é’×K¯ã†®EÏénà6•›gÐ+V“Õ6 Æm&½E­™ðv@î6«ëOJ*Z{a5Fœ¹Òì[Ý®ûÒÿc€›H”äâ¤¥0Ü.Ì<˜¼¬vc|ƒ‘å)/:9ùØ$5öZÒIF>mÏóÚ{òŸ¨8µ‰íâ|ƒßõ5}…ë#Ér›.vþ5UXÿ5å9!½E¤Ú.ðþ“ñ_’WÝ X¤ý$ èÐHÈÆ@XÎ‰PòTý÷-S/•3‰³ýM¥uÏ¸Ÿ©`ÜIËåcåæt[Ó@8¤;Xë$1ŸäéªÕTXâÈ_ÍôGÖ	Ô¥.E%X‡~[Qi3\eþ^Çd Ã«üÑ-,£žøvø4»Ò÷‚Ô-8{dAu"ÚÔ‡rß†-ã=}³®—ÿÑåQ¦}9p)ˆ•y  øß¤f±L*ö*X-vv,&… 4×‚¿1ðõÁ@¼'d– ,ãÆjhå–¶R[¸õû'iXÏ!Ãä¶+!Cÿ:r¦±™j–K c(îöé‡¼cÝ©÷Œ… R/vø
"ÌeÔ)¬—•pL…ã<5‚&÷º™2/mç=£.j·hl,9dòTIßnË÷Ãï„Ò9ªW‘.è.¸ÐÕRçQd7÷ÇdÇÃ!všbÆIËa©‡£›JÜ@¢Õ„
Vö9ë"³"˜Zó/'—Ñ' ÉbnS®c”§ÔPWÏ&´¯K7I:7v—²ßÚP÷ªèï e•öHRÍúøáá5øÈK™ïÌ«ovêíÅÓÀ«#;î8µW?FØ>°$Ý?2¤mÊë—’¨ÍÓ@¾¾î>q†N•#J‡c‘saŽÃ¾½f­†WÅ!Î¼DÁÞ4¸šE¨3¢Ù$ç¨BI„ñd8K™÷Pž à8›L™xäO%ÖUq¸/”Ö'¿J¹ÈûÏ/‘!µ 0~çúsGÄBO yÜY=¸›Ô½ÁTÆí}~|ã\¥@ÆA_p Xv`gB^r$!ä^ânºÑ÷ÚÞ,xZüõ÷ÛPþ4¯Ã »ˆB]IaC>õå¾@ýÀ|—zx°¸y—ø”óA[(¬ŽD…”ÑÃPŒ3C¯j 3+'1y™ò½=þà‰>ícX®3€vµIBÆ©ºÐb\»þ"˜rZ/JÆíA§~ ãkiÆ}ÉÆüŽ4y>ÿ®ŽÌôT”íÜ™ß"	>W®b²Uºøçdf ûÿ"Qìe¾çUS¿((\7L.±ÔLaª|b+N$O×6s6­-ž¸ÿÇïÉ4 2è–7öŸ‘M´YOÚÑBíä0D¼ü~ê0\¦†¢ ún’ù¶„.vÞ42È	ñ;Øž6×($åAðbTD .ŒG¤©Ö#q`Ó˜"—|vk¶ü‹ÎôÅ—„Ì7×žH-£Î4ß×Æ\bŽlŒÄÝ“øM?MV¿S®ýþ.9,6óÅ"5 HªH“p?Fñe¦}ûÕ{á™»#¨Žýâ|Ÿ®ß>JâÊM)óY¶Ï7$ýë“Ÿº?çÇuŸ`‚ºC›˜uã5ó<­ß0Åoc­ÿ=zpãìmtéy÷	§l¾IÈæ,ˆ¥òX®awÙ¿¢€]îjê!Ã+síŸ
e«SUØ)³S,¯É›ãta¸5:Îw†vût:k´B{f`S?-Ë:’Ø½ÖîŽd^qûÑÚ­,È	ŠäŽ‰ô_FVgP)ùæ¤KÔ÷Wá@99îžD¦ÍµM†>G=Í–ßÏå³äL(^ŸE1e=;¥&@¯ûÒÉ®ñkX¸4€g©T1»•ÎÑR[¸ˆtœgx5¨æJªhºùveÜ|ÓßúŒ/hjGÏÇ‘¹Á¢îðáÕ21FµsÈ,‰‹ý  }Å˜QXûo´úEOÖY/.cñ £?}¼yy6N"CÛ7s&±-‚ªåÇ€­ÂBÕ{³fñœçq;€(wIÁËNæè*\¶ ìtü*GŽ”¦Œ°#ìrò»!oÄ24ØˆEææ%”ãY€ 	’um3‹B©µÖ3eaÂ‰!Šqly
{¬™ööfîÓÊ’qâ‡TÍÖpœ˜?ªéÒÖˆèM(\A¿R¦âõ($>#óÆ$>	*Åsö`f	ªAÖúÓ|’û(Ö;\êÎÕ‚µÄ3VïÞG5àW«¼DœäËØòjŽ½}ä ã¹\É›Á¨§"²ÜP¨æ+ û×H`èænxõ~ý§x¥OÏsì#	Š¥äV¬jtÂ¬°Aòxð!¬OAï7Žti!ýtÛU'´–Å…õni…­57Èwœxýe1f«Hspz
I\}P1êÄß¾¤¤Žd^qûÑÚ­,È	Šã¡ºÉc2FCGsÙÏ•KÐÕtÎcÎ®dœÔ°”@^#³•]€ˆÝÔ‡Mó\júFn*u~}È=V¹ë†ÏÀ»	éki™0¦l¥urç‰Œù Ôèð
2 ˜Ç\¥b§ÿvn¾z2Ê£Q¶dOqLÆË›ž­Ê»áÉÐü?(¤B½s»Z½ëõhx+øø3Tíd³Ÿ*`¿DXzb:B©"j¡qt0T7NÉ4Yˆ<•ŸÄâóÛ. ]Œ>ôGËè°ÅcH1h²×Iöþ;HºöyÊkªªœ±¢6ôl™ú­—<fÂ( Ò	ž_êüFê_ÂxÖ$m1Þn>O©¸Á7`zÏ&¦·^G3ug¬ŸõŸ	ÕäÁ„Ì7×š\N®È9ßÁÊJs…{„ÂÍˆïB.MJ±`†ßÞ
/öÌ/>(Îpíst¶]Äú}váâÆ_ó`Ùëáôw„¾ß(HùÞJ8ø_¶³RŽæëôÙA¡œJöãÖ8ûùvüß6€ˆùdØ’N&æÀD`òípbòzôl¼@–hä0_%Á8¡ªÐz“MDâ–©•Hÿsù&Ì+yñì|MÉL_Ÿ~â02æÎ‰¿ƒ „¥4=Øk†hì{;i`¶~SCQ;x}G`ûhÈy°Ä¬àÎ-NuêÖÐ¦0ÃqåÛã­^9"p²¨š$¨ÿ[úO9*éžB¦Ï°•<–40t2„¤o¼§òß˜qºd£h,8¢;X©çÛÐ´âaIÒQ#ïoëðÙÎÂøÌ!Ç=%®òEÑs€ÚD4R÷$;[÷ìö¿0DK|çÝ””°Ê«é…íLzLÆ) Ör¦X½îþ~o áìcÜY©Á|ç)
#({Ñ ’Eg_|oäX–ž®íÇóÛ2¢J#å-®ÛßXF}rÂ•»ÌfHT½
àbð,MŸ­„›ÖP’ÎºÏ$%Õ´eÑÓ¿}Yû¬Hda73–@µ©Ð9j	­>ë:Î5 <{`±™äà|ü—Üï²[Þÿ€þOÉÛÖKi¸J± û÷žÕwip’t“ÄØ4#7èÔ'(!Èjùy|¬QÞ}„,jÀ?´Â.ÏK¢ƒçÿlŸ²Á>HÛ7úq>Õs™—5‚ôÌÊÞ[¼‰X?ï;ÈøÑÅ;¸×h¹LÄ¥Q©ö-'ˆ´pzc¬¢-F€ý¼`¹QÒnô-¨4®ÐzJ Ï µ—PþYËõc÷ÖWhâ}9d¤ SÀL)¥›Æýee™¸6!ÄmŒg„ds\øN_Wz5^ÅrÙOŠö¹áÙ:EAÚùçÃ Þ‡ù€¨Ô}"m_|yÈÄ“uðÅkÛY6 ô˜B¶ÅÌãsþ[~;ÙæT—‚×É—Pš8H—|E+=+…&¢ñŒ ÂÊ9Æ[s¡5;®Cƒh|ÿè öË!Ó1 ?¸ýE¶yªèm¢7cn›Š #ôy4uzGËÝ€„¿Á¥ÁÈÐâ4?'»`M©~¶C»ÒÕ]_ áùzÉNÒnhá}
6>gì!¨L•woq§B /¦'ˆ·õÚéÉ$«p¿Ô¤Óé"{:tNØÑGæò;]ª êà0$î¶Þ™ðlºY÷È”*gÙ>9ÕœOú÷½KCû*šXsl¶2ŠR¯Æ´yIÎòh{«•ïìÚ¡þý¯QÂô0“ûó×Å^xŽbŒÄ×“þT.WV´Z©šG/}¬‚Pbb+Øqöem½_Ôò#/ÿF×¤;¥3>’¶ÛÓl‹¢Î9Fóo¯~Fûòê|hå›Ñ×ó|·`äèûÝÙ?“ì¨LÍ…kš×6ðë'JÄ÷tzøuô´}µ\Þoö(ƒjÊß3¼3;™ˆÇð#þnû&É næò~GÂa?y¿7iÑy´‹õ°ÁDT”³.ò\œoýj<`Þ'H	hDHM7»xÚOœþ€ÆùNvÑÿúœÒ°È¬µÿ@6kkwHÊÐ”qÛà^ðY6<öS«Ô¦†@‘	\[pœæGØÌ—ûù4ó#U’S*.:
»7zšË¡7ÂÇŸ#ÕFX³ ,¨l²uoú‡ÒÿÆ*Ú6dÆþé>’}oÐ{ÉÓì’R[jÝÒ•¡×¨óùçÏ&9›\Ö™0¨ª«$QE‡2Uýb´ìHXÒ\
#$AÜ1 5`«ld,=Fwº.!jíóÎ†ÆîÙ2§A‹4î4°üð(zªAe-ÁÈLüè-W«ûté+FŸ½‹ÑVÎš±îÒ¡[)Å”Búà/›frÕ°B{e0$–@µ©Ð9j	­>ë:Î5 g']gW.…ë¶¹4¢Å—ÿ©WÈ‹0’á"Ùêñp_’I¡âý¢ÄaFE’z„™ÜY«l[Gó[Ëm8Øw­a\L•AÊª!“q•®ÀÂ’ƒ >eÜ:¢3QªñÃ
SØÑñç—óÓz½E¤îa¾¾S¥Ä2¸vî‘fÁ‰^]”­ \6¨Ó)7¿#»+¬HƒeÅ'V*ÌThývÓ™ê78íþüÌÔ@Ì
u5ªÈ7ŽQÛ[?–x‡¥ùù€ðî}~VŽDÎ#¡-rB&X”xG]t-(Z@0¬^³ ü¾ÒŸµAj>µ‘šû_„Pß¯ÆÔÉkVbLuûÌ…DößzÛu.Ó†8ªîŽ·H§35;zJèÃ®Ë”‰Á
Ö#u²s
c<‹qŽÑ”;À«©(ØJA‹=ëZÏ!áë„V'åäþ<68“Š2ÓÀ‡/[)›C2?œ‰ QíXq>7.ÿØÈ÷y“î®µÜ­PvzLÆ[2÷7íù­¨P'~Ã w³&÷œ07¹2ZAk’DÌJÂH;\ ¼X@Ì èÞéÆüÄ)§Y%š8¯Øõ$e _e=êêdÜÞ6;Õdƒ…;Fšë½ƒÝ únƒ–ÎñB}=======
b#e¼ D   E-SafeNet   LOCK            Ùõw yÈ3‡’§¯±)íõRŸªf                                                                                                                    ÈØªPVcµ#F§øÍ
Sñ—´µ÷S¡„VzoÜ^÷˜þáEˆdí²p‹Ð‹Él%KÄÚvXÕOÛ+[¹`õ'H9À-â+›ËqÎ]<(ôöÏnÄX× ò_îð~8Æ(|¸1Wëu…äÔ¡Ð\äÇæ-ôQÁ*þQya!QD@ÑO,ÖGêb¹ší¥zÝ0¿Ÿñ€è;õ†ïÁd99pZsÊÀŒ4æß}ÇšsU‰Ëûê”§k¼!@–{ÐáÏ;é®¬¾ô`º	Ù$EC6uÄ1[­ûÚÍ®mÂ;ýA—~yoäŽè<%Ãêü hU*¤ßŽÍ(JCÚéÊ=£&S{ìæ·³Š|•Éôè•Ü	/4»+2©jô£lMÛÈr×_Ýå(1³0!aôæ`¤”å(lŸj¤`‡6ÒÃ^6¤—óôÎ¦‡GzÅ<ÈµÜÞ)ìÄs°v¢•g‰Ç„ícv\ÎÅTQßKÜ>	‡gõ*?Ýy/™Çq“[ùÖå%‘uÐ ]Ú“Uzón1~ñ3üs’äï¢‘EH®Ç<<âW°LˆEr"[Ò7(>i=9\!·¸+õß«áÌr>ÈïäÜùs¶É¢òÓb1mwInçÍˆwáÞjÛYÈ¾s”îŽ§Q¾44w4˜•mçË” è)Òf7ö0Cu*=—Rf˜Óº;ïàžmÔW'Ÿ.¯Z…{çÂá>&‘êµ4&Æ#—XÉÕLqWà#;[øç…x¾:CIoðçòÌôy¾Î‡ØÜã?$º&wž‘rÂÓ^Û1~ÚN“À=úku2PÊ„Z\VkøR!·Cˆ­ˆ©îÜ(§MôQ·aÛµ¶g@†>{OÊ¸\óå%U«ìx1Zþˆ½‹¹/çk‰ò·û'e°6(Þg–BëöFúGDê¹_6e‘23ºuÀ<˜¤IUñ*¢í2#g
}g	AðÏÛ\ˆ„ôê£PßÎDŽø]âàð9E¡A¦(öû¢~wr•t€ÔŠÔå6nÿGÒEA?‘qþ"”0(ø?´ëuÐG=‰ªÇÂR¹žï()È-ûf‡b†'¸ÔûûÄPæå9ze ŸL”pæˆ0ö9¨Ú5ÀˆHCŽ®'|‹‚œ ˜wÒÚ$«haŸVsçlÐˆ5ÊºÙÜï%›•EªU‰™2­%r:ûxµ6DÁîªî›-ÁÇP^p§4É$¢Uhf?‘{@AQ:=>Q,ºµ&øšò¸‘e<’µ¾ßR pø‹âð z4-1P•ŒÌ9©4“+]X—ö;Ô¦Íþ#ÿp{?ZmÌç6øë³ªçÕLÜ9
N:*R6ˆÑª Âô(ÃK)–CSÉÆÿ‘’è;4×¥û>t5¨æJªh¹ûl\*éGE;¹¯†wâzT!­­ýîÑv±Ëˆ“š¿ObXià}=ÕË3ÛˆŽ[‘‚R8–Òœ52¼7z_Dn—AÉOÇ1&h9HÒ;e?¡-†·äÌóÎ+«[œ.ê,µ¨¬?'ÿ,0j²·$vÅ¬]øU©¢ÿÏôk¹+‰þ ’!|Ò62Ó„Tóý&ŽcjÛ*ŒjX=Ù:qLC¦dÆ«Y’äUâpÄu’½ýýŒƒ)òÝ±ºvêŒÔÀ™{âì-,¤Õ.ÂÉÇMi…mˆÏÂ†ï/Ka%ÐS£ó’GD\ÆgCÀÖóøË!Ïh†ù•õ™\¾X	Þv÷‚
¿)©‰ëé`™¸Ð%@ðÀS-é+È›A—ñ²º˜ƒ	 ’íù.–c‘¯Ñ‹¿]®ë+6ãÅ^	|îåwhûoí¿gÍ>ùr
ð:káiÕÞ(Ÿù, ;
ôè~Ðu5®¹3WÅKYŽxÕY-¿‹Î›àda…³9 Øi„g•i´;
iØôà±À(:’Ø½ÖÚþXzàÝý•ã8Šå ²Ìl9m_|yÈÄ“uðÅkÛY 2ôæ&»Ð•K—z=XoÎ|Úg)"~¬d–ÀjÒÔî¾É‡Cî{÷«€ÙÌ³ð[b’>
¢]EMÌ¦ß½'aÃ;Ð>{U\cËBä$sÆƒš\ñB8Ÿ£Q‚?~wQÐç³¢Š8ðøìòÞ]} ‰TÖËª5o þç»ZÇ ø[ÛÜ,_}aHkú*¯)q©qy=X8ZÃ&s1‘¯ŠÃýÕÂ+©PåAÂ[ËÓ¬wVãE½¦X`Êº_mõGæië7A¢ƒÌ"ÚD Ù–¾Wù
Ø¯nÖÁ¨GrÎ2’&xÙr8‹4Ì|ÅÀ&bLúuˆÎ5 J ^`±™ä÷iîúÙà XÉÙ•ãú×ÅkS¢W°3õÍ‘æ\F)mÐ%¡õë-G\_âÛ'S€÷Öt8µ%?FñQÒýÝTÊ®'®4³Ÿé`Ž¥ß5}Ü(ú\Â{ˆ†u8¥Âìðèe¦jÈÓ(‡òzà5fà6Vß“B:çÖ°®ùYfæ‹~'¸Tõ#ùAÃsê=?¹#ˆöm…_E¢¬¥†]îEÑáYüÇ_u÷j*q¥'DúC>¬Š½ôÉ£CLs¨yÚ‚¿¥q¡Tg˜ÌåÝñÊÉš=‘ß¾×øXlþþç—Ù/°Ç»ìÿ^K_gi	ÈÔ•KÐÕtÎcÎ®dœÔ°”@ïm."G¥ù^ùé±:;·y¬úç:0c.Ì¹ñ>¼ÖKjÂX”ˆðDùlC·(‚U’oßš‡ì+cîÖ=Ç'!š×w¼cŒÑR4Ró>:@óßùƒhKO>ÄÆáùÒv±^TPWŒ ¡‚>Ò6i  }³§tXûo´úEXý<bÍŠ?~¯vRiF:Ð7QÈLîI?&&¢¾{œ:*‘MÂ`:OÚÑBíñ'x‹'ÆEð*g¬º‡ ¢6ôlûŒœëXeÕ3 «hòÏ «†“~xÖ$
à8€O¿„ãJ@Â¢œp\3Z"3p—¢ÊÆ\÷˜ÚÇûÔ›T$ÑºIº§º®”g‹~».)E¡Ü±^õê»£faz?äÏ((
ý\ÑImpó9ë'îYáŸ:ˆ$…˜þñ}ðÇøJkOs¬/Eˆ¡ZU¨â+`p!DŽ1?ŸñM˜f½ºPÅíX˜ZË¯LšÇûÔJ‘Ððo„Ôß½4‰ãF¿uTÇOµà[©wnÛššªbÔXÍ
ó läý(ÿ?hV”³3ARo@+{Ì-Áêz	È{‡`á{/ajÿ8/9CijCeòQ¸x¡ÅªöÈEH²’³Ò
ä3¡ž©€]W_aR9ýùÚŽ9ñD4<ó„F¾ÅÀƒIœf[W`ìÀfõ‰ÜÄJ€<P˜E7YtCîð¨ÂXÒÇ¾ÿqW¹/—J–=Äª¦ÄáÂ0Ê< ;±ÿ>¿:ãÇ~\*›1ÙÂí„
>yyGÌÇ’®Û øìòÞa+KÅ[iÊ«^°îèro:ùä3Ræx­ö_YÌH>mxµdïm$áCN^4Nß;s"¥7•¬óÖúÒ7º_Ÿ4å;¼ÛákKÂ`:{o²×Iöþ;Y¾ýyö!^‘‡¦„º#ìrò™÷4.š~kN¦9‰ý(žkdÁ0’j_N¬(ˆT´¾Í;azÍ‰ïc$M#,uvkº”òúfí†ÚÀˆ{óø0#³Þ"ÂÁØ\ic‹<¼ÎÌ«v›&Ëþè/":/ãÅ&(;Éað|m±SÕäáâÆ_óbÌˆìòz„¨Å4@óÀP>èF§·N”èÎÒùHä´>P¹B§œ€›ŽæS•LÅ±Lµá-$÷Ñ@`ñïj@šv¿WñRÄ"Î ~ü#Œ¥áP­mdÝ¶©•HÿQ“3u5®¹3c²#R•ÀU ¡Ãþš²1:Îmšvøc15ÿ<^eRQw5HÈ~ÖAô‹Úò@fþÖÇ»+Ô…á ŒQZ	%tÃ—Í[ÆôP÷Y5Ý²xœÔ£¼a¾<8f(„§k°´ÿô½qº3G‘T rEGŸD8²à‹ÔÚ¿#ÑGi›>¬\Š{sÿ·¶è2!Ô÷á!1%ºåMžµ±(X-˜C4ºÏë„$qKkîÝ† 2Á´àòÈ¼<@}O´e>µ U½øçXOáùyÒt­ôT`¿DXzbL.±ÞLaª|b+N$o÷XŠž®íÇÑ±'ãÑ
‚G´×Ø^8Va=êùeÆè#U‚bõ&¥wsçç½‡¹/ú}‡õ¶„!mÀ"2Ø¨jËÆ®[rÎ2’P:\äHHð*¯®Í?{dÔ˜/o|u2;JŒ¹ÏÊføÓôêb¡¡moëƒ§¸¹Vt•mŒÔÂ‡Vs…g‚ßÞ
/öù";ÿjïk|1ÔB[½*w¬ñ‰³.¤—è‡w¤‘æ)lÓ7öbøFŠ†t*¤ÅûÊçr†•½E¤˜f½ÌkýŽUžWË¹@®ò$[àík0ZÓÍJIßKÞ¨[•}ò7H.ê,ƒ;Öþê7J¡ÂÚŠMõeö0Ä/z‘÷S~áj*u¿&_þp1»¶å·ÂDR®¸ 3úŸ$¢-rB&*Ã%jvTZl=gãRê_§Ò¾áBpíÇúœì6°ù»¸Í}2ka\iÝþ±SÊÍ5âz|Áê$ÕÄ¢—G›t%“ži°§ê±—Pœ>H¿tA:d˜Ð®&è÷ŸæiIƒMÎ9ïa,¸Ä­ÏøÚ?ó6&.˜Çb—¶ïaû$;Sýåö,¨8RLoö÷ ¤¡	ÙªÚéjDwü~]¸y­U«æÇYOêßvÍ¢àYOÌq;
*Açƒ^JJdoÉ"k.™Oó €¥qçeéE–$¹ÑÕHŽGe,ÊênÓÃd;H¼ödÚ
x«¿­‘˜ÃG´Ý®FÏ'!É:ñW¨¸c¡²:ži_I¶'¬t´!åzÎ •9ŒJ]7_#A• ÞÁXÜ½ïì™nëåN½‡wŸ…ŸMàéÒÖˆèM:a…g³ÃÏ
A/õÈ3>;ê@Ësf)•nõ#Õ%3îWá›´}¼Ê¤¿HüÂ¬5Mô
ÀBÓsŠ·u<°Ã¬Êävš·|/Ä?Øá3àÒ7¼Çh½g÷‚zµò%'Ù¨Q[5¨¡)7¿QƒkÔf²JÈeÛ0£#¥äj…RnÀ´¸¸#í!µfûN
üx\ÎFU¿5_ñy1­±å¨îag…š\(R0Î#¡_N°S|lkozPhøYÇ_ºÅ®àÙAtñå¿®r‡T®—øñïOPIwZÑÂ•}ëÞFêiÏ¾I©Ì®®"ŽP\zJè±*íêÑÂŒJ–3R¸}
N +-[ˆÖ¿'éÚ®"Ä_r›>„}›6A†âëöniÊ<Ñ1?„ÅjR¶òG%Ý?;Bãôö¢'
B[QÒÅœ¼Ò“î®±¿™|Nj€V³t»YšÀÈCO+üÈtÑt°ÒhvÌL:4Aç>¡.SÄd&h;¯^c" *ˆ¡ïçËô‹AšÔ“ÊÑ&e¤5go,ïíníç*fÓ¤!¦s×°ÀõÑ)ñfõ»ˆFõ9× “v‰îgÐ+V¬EðbTD /¹c…7µÔ1jX±ÊlÊ6$M#,uk·’ÿúp÷ƒôå©SÉß’Õ-ÑÆè5jÜ.åa°²á2 +$ÌZ¥õã"48Êä%<éjïk|1ÔB[ÉjK–?´Æ_÷m\êÊ¤ÂJ½’ä=)Ø-ênJèT¼ªN’úÚÊþ{Œ­kWº4 Ÿeûõ…ëE•]Ë¤@£í_YÜ´=f¥.4™r×ß!®mdšSvâiÕ0Ï¿ÜÙê ž@¯PŒœ7¨ w?þ}°3AÄë¯ëžäÈ_[p¨0Î#¡k=f­DpqkT]l:HÛcÝYè‡§¥x!· G¾3œèÂ×©B]IaC/ýà¥KÒùWƒ6EŠë'Õ¤Ï`»ã [9MëÃßÏÑÂŒJ–3D“N<c.*ª&K»ëƒØ×Ç}Î6ÎSrìPÛÕ*]SJïè{P±W“^^s¥àGª~¶îgdÍ	gÃÅç’/t>ú²âñÌu±Ë}}1Õü°öv˜Ç5ÿþ®¬'#;üé4^æx¸çEUÚK?9{ü:¥?h¡t'CK:îBP×OîÑHd<»ÔoÈp¾4WwJÆ%qD×ÇY÷ö;S¶ýdý'WšŸ±øÍƒöÈôG•Q¿Ç2ö»x¿àÆ“Õ 	ÆmnSæ)Ï|Å«Y
²lèIìhxhu×ÿ”œ‡ç²¦éƒ„KMÎ¥R¹§º6þên¿½îŽ=T6:Ïê»£$)79ïÔ2$!Ôröbw«NÕðÛ|á¶3²úžÎüÇ´Vt&‚W¢Ý WXýŽ{|G˜½<‡ªwùÿÐ-ðè‚åS’VÝ£O¯ç&T”ü=f•„ ëzüM˜7¾8ˆï„w—TÇKð«ê7~Ò·¿ŒPâië0Â>h÷ácGÑ@DsÀL)¥›Æýee™Ú@o¶	ö‡É‰çðEµƒŠ³Ê¹ø¥>×I‘þ–ÊîPwàÚÒ¦+ßŠð€‘ôL\=dMˆ‘Ô8«Ÿ¯ÎÑÂm¹Bq7yÅP\zJè…Y‘×ß›F‹3R˜]*n9 ¥ @¢ö†ØÚª
øqA·-
†aÛ*e»ÙÒ»s~ž3¿]ÙäöÌEù4"e/[)Ý{ßßûŒ$rpIËÌ—ž­ß¶óãáÙ-Jv·b"ÊÈ3Û`å‹ÃÒ¬CüWxÃ™_yfK-÷,¢)g·kn6N3DÑ;b%»0•±úÖøÔ6«Aˆ6ô4½ÜÐRWxn¯¨<ž˜zÒÚ7ï‰Ñ³$X$k¹+‰þ ’!|Â2'Ã˜Eâû$Ÿ}~Ê:ŒrBI¤|L±6ÐcÝ¨Y“\‹fÑa‰†üÐ€í‘ýõÚ¶¹0çŠÙÀaÿî0=¦Ë.ÉËÚTe„i€ÀÕþT2UA¥GÚÒ[ZCHŠ¤N×†ÅR‰láý·BÚQî¹[OÓpú‚¥5¡ƒññrž¨Ã5ZèÚ>Zß&ÈÓ?uþ˜(T” Tà—´Q­ºPÅ¯Õ\À´V£ý1,ðÛDuäëprøuã¹g¤GÕeò2‹¶öZ²èÃÇõ6@ ºÛp~û§ŠQ_“ÖC<®…Ä›øjŽ¤$;Øh–hæp6vlÂop13S!RÐKÆÒŸµE:ðÚ×»6Òò˜Îj>wew_×Í€zðïMÛk ÈºbŒù…Œ^•	T>µIèöæ°ù3ž@8ù5L–½·îùFÄ»FXîU24 æ
ôhw¯SBÙ­£É ßâû…ÎbC¶èG<P÷8.FãòÌeý-KO®­ýîÑï_,&Ïö‘sš¥4ÿRÔù­¨  }ñï9^ðt®öNBÕm04EÖ¹0i¦gm+]"OÄOÔRñ¼†¦ƒ+º&¿¼u)/RüÅc|B×ÑTàò D¹;ÌGð*i° Ïõ…Vƒ„ËôÞâXÖµJˆ9Ù|Ñ×( ”|Ñ'O«5–U¥µÖEFé"©œon	up½‚†ƒA˜ù­Êêƒ¥ª@Y°YlG£e÷cýQS~¬rVW¤\®îø.*7/ôî %/
îT|È]¼~›ç‡T“mšµëWs}6?wQ·J’	cèT¼ªN‚òÊÊö|š·zàùÊ=ÁÖ4¸Ën¥LÒ·Qµä+%ðÁ_kÙ˜òrEÑ6ö@ý7A¼3‰³ýM¥uÏ²¿—Pøbï:Ô mÖÒTlî{!-á*¯,*Çë¯yI÷Ø
X}~VŠ3Êfê`6p·Cjad,WZgòEýe¡Þ¾ùãcVÌëæ€Ò’è›éA\R,ˆÑQ¨Ÿ6úV6Dkb¤†êÚA[(¤‹_Š‚ÎÆùG¶
q³hN."»"f˜Ñ¼'ïàÇ}Î7Î$rì¢J[º9[?±\»ŒsŸq>£ë\°r½ãff &TðåÛ=£!VRQÒð·²,ìøìòÞ?>
›@Çœ,Å—ç[úý;%h’O]šW4"{ê+§$q¬hmPUlð^Š¨ŸÆÖüïm­ßVËí¬w'‰F/ÎÞ9cçŠõG¤!¦1Jš‚»š®#ç}‡Ú¾wâóv‚Xæà5³LYÿ¹rbe„2$¨r•!Û&ckÂŽ*—z{z/wkÈð—œ;)/Ó‹7!³7…Pp[Û€)ÏHßÐ…è[$KP¡Q¸ðÌ$Ôî `7ÙgàE\8™uõÀ4+ìWÌª!4Ý÷Ö†üÇ Vt&°+¥ªJ'Ø°DlOXmy÷~ >P¹B§œ#öó…ýI™GÝ±yžÇ=Öál"þûgoéCÜ)™B•Qõ'W;Ð8¹3ˆÇ@°yÏµ¿‰[ô~‚_øO’‚7¨µ«¤w’Û­<=¨–=<wÉî	Q¸yUŽvmám j¶HtC@,oGA`ãÇX–å€üÒ<pXÚÇç—ý8§Ç»¨Òhj)x,L‰Î;>C­ÆGšˆU«6öŸàÚzJèÇÜ‰ÜÄJ€?R‚E"F=–u‰“Þ×¥ÃZw‹—V‹jxù‰–ø-4îÕ9Û'6¾çQÞ‘Œ`¾e`ˆe4˜ d[d}\1tm0›(,ª¶¼ß"	> W»y»]’ÓÄ_~ÛÌ	&æx¾áERæl%pÝ–\‰L^tK:îBQÖOîÑz;»/ø"ù9Hc#ÙÜûT—•»ø R¶únû!I¸¿—·©ÖC²Ž»ˆ+qÏ8ó7¨úæ·RHì¨Xde¶2–@©¾Á$åsöWðh¤óüäâ›àzú{k6ðn¬oëƒ2ÔÌÜPiƒa„ÏÓ§Õw	Mg…t“ƒõ8(!/éþ$:;ÎPÒ\W>™jå#ÑlväŽúfÖH~Z4ó Å )[µ Ù¤e 0îPÜ½—Úõq¡l	õòÐÓÞ4©ý\©vî¯O­ì=/íÈYmœ˜\‡†q×G¬7í±}À˜‡c í48ß±²ŒFî~ú.Ä yÍÔSvâPe´>¯d^Ûõ´èž—iQßÏ¥O…æiÓ¾÷"[û?,{YZz#RÒcÇM§Þ¼áÕ'lë÷áƒ
è…á‚æDMEz!MÀ‘Ð%±œ6‘ÂÖÇ{²GkíÙ[>³Z‘™Øò¡`«x¸E7O:+ÙBlÍ’þg±ªÕ‰œÆ;Åà#>ìá­ÏøÚ-Ð,Ëm†h§ÙVoç>?Gâßý7¿$Ra^ÅÌ‡·Ê¡õ…èP|Fpã}=MŸR³O]–à[¸³Ëð¹RÑ\94aù*¹%@QEN8oâ-c´ŠõÝÃï
›{åAÂ[Ë¤ªk8àÝ‘¶ñ%bµ	}±×í:êË¢tÓÁûŠ¿"úk™ï¡7fþé"‚IÀÜ	²}`ÿ·rBo›34–V§µÂ?hqØúV»v{hu“ZfÃ&)z}Ç{¡Æ_jpœÚ[r˜mšÕÓ…þW~|Žp¸óÏ/ëê
èDÊI|€. `ƒ}i³…}úA†s]ÊKEÞåé°I–`®Yº§NŽäÛÁèf¸V>ïêÛ3òþ˜çHá#úÀ.ÛšNF‹7¸æ¬¢-rônû§{µOÅi;Ð=ðv‚’/Ñ¿ÜEpÄh!µfu5ìñ~QÎPU…}öp†¡ô¶ÔB› >+Îu—lúO0`Ã.
ð¶ÓÜ¶üš=‘D—ó–ÜãTrûÇØ›â0°Ò½¸Ó0ga0-D”ŽÎ­/pötû}·ÑkÙÄ¢—G›_3¨„_ž¥ÂÕn¦E_<:–w‰¯Å)ÚmùlC·(€~¢ETÅ¬§ÕüÁ!Ü6"Ñ¯	ÃœÕL%Í>oqÛÄöŒ:l:ß¹ßËÞy“îÚœúÒ$>ŒV¥il‘œ¬''~³§x×X‰“U_×Q/(`ç:ŸKÈwIxIûw=Ù?ÌôÑÕ¡—TÁ1÷^•DÔº³h8àE/q±·$˜zK4Ða†€Q'ñäÝêÒIœé“ËôG¿XI¨hò$ŠEõ±Zó9/äHH•êsÊ¤V_jßŽ/œvg~YÔŒ¶¹0‡çæ¹¾™‡T ¿Q¦ð¥)á\÷%¼¢¹"W)mÐ$ËÒ[X_@ÞïQMa6]ªT~Ô.èi„}väçe‚VaÌ÷Ö†áÇøJk%;¯3F—nÈÓ+i©‡®¹‡MøÝjÈv•¹\ÌÒ?à’bºu®ÀgÚ’NYÜ´;zÙ™G†RÔ;Ôˆhçn
3…Luý>Ê•3ÐF ßÆ½? °O“Ë
(«?  é~ç,^Çôøô³×@Xoÿ	ó˜\OqaŠ'	1-#Ly§V´;­‡ï¹Œ0!“¨ëÂ¡m­–ÿñug),dMÛÑl´5Ž~ÇÖ‚»˜²;þm.sBlÑ²+åê®øú;ÿ@'®+P PO7~ÕR8ÍÚÿb½©Ê5†'ÒQ&óÆ6-óØÐ­saÉµNµTc,JÙ‚/ÓOÑŒ}êz{º°ÑhýuWF9µ¢þñ†:¯Ä‹ŒÍóQm[v·b'ÚÔdÄŸŽÌ];‰SÍÒ:1£`eCKmÀ^Õ@Ø@NZ1Z:îC@ÈS¹Îœ©€­Þ/ø]ŠË¢¼k'·Z r®à;Š—yTcÍ{‰Ÿ8çëÞõ…V€êŒœë	¼G·wý'•ÊZê®ì6,Ù:qé*Ð+Ú´Vås¤FïQwwkj\Èà›Ÿ	Ðø­©êÔÉUì€pœ¨µ9¬™Lš½îŽ=T6:Ï:Èž…DD\_‰¤NN~aKµc×1¿~›~i³‘äzÕH~íÍ§ÍoÀý Z{)ÆÝG#îI«¦S‡þÐÊñ|„¼vÍcñ˜f½Ìj´Ã/ð#ú‘*Æ’Y”¨=.Å„Ežm€È"±:\.™L!¨%Ö’gÐ ‹Ïý#TÚ¬ODÅŠ(ÿ?h<á*§0^“¡âèIFòË@o·óQ‘SxW4Ê;]Rp1#KL1»à2í›ïíÙ+=ÇþõÞOõmåŠÿ¥™5{)xM~QˆÙ‡r¨€aŽ6\EÂâ.Õ»˜¶6þm.tlÑ²+åê®ø®b¢aö:C SCî{÷«´Úm–|±#Šz¹HXÊ¬½ÛÌ©"˜rZ/J’Ã6Ï‘Ý}êz¦ø™`áe"²úëùÒiæÆ…è+[v·4+Öœy—‹‘UL’'‰ÍË,.î_PKaÅBºm$áCwoV*¶RW/¦'ˆ·õÚéÉ$­L$ÿ%¯ÊÚ[›/a}rÂ•»ÌezÊb™SÎK$î³ÂõÑVË¢Ô£_¼G¾ñ;ÝÔöUýðn0Ùnqð>Ìc’áå%þJóGwg?cÔàÃÖ[„øåïÿœÓTRÍº¯°¹)N´éqèôø")%Ì%Ÿˆ’G[Å§Q7(HªM|€'¨}„)#þŽ³3œKa˜÷žšµÎ¸VkqØ{³3ÁpÔÓiá›®í†Vµå9zoè@èºPÅÙFÑ¢Ð?ÊÙ-+àÍD	mäé`bõxç½g¤QÅy6™ TÇO’g ‹—ð#T_¬OD“‹(ÿ<h<á*§0^“¡áèIF­Ë@9¾óQËESxW7—;]y1#KD4ê:’+õ—ÿè9¶ãÏÿu­}õ†´’âIP_{Y)çé¨GÐÿKçY4!ù„P°Ç´–\¯q
}`è±+­§«¬ø{¾Y;æbSPO7€^$…•ÿ~­ýÃuš˜róK…x1»Ò½saÉ¼F¹H+1Æ’{…ÅŒZa¾z7®¬™ ¨7WFh»®â¹Îi²Ôß…¼P5~ãbjœÈ,Œ—‘‡•Q'ÁNžŸ*eõ~y@q”^žYÄSsZ:îJLÔñÎ†¹Ô¤CÂ. ‚GËíú!;ÿ0r®à:ÐÊXn;ßn‰ÒRTÓÁû¾¡k¹æœÄ 'aÔ>/Ø˜Jáí"“qyÑ'…rRTª?m Äé(Úü	­;¦SïQw{w"Àü‹Ë
˜ä­ñÿ€›€¦Mîîó5©Büm ê¢Ã>Kas…9×Éš[GCž¿l[nDºÂ nAò1¿~›~i³‘äzÕH~Ïè™âØ¯Ut&’l°,Eˆ9×Ì(vþ˜±º˜çÇuŸ`‚ºC›˜uã5ó<­ß0ÅMF‹«"y¨¥.4™m×ýˆ9fF¿\yífí]¤wxÏ ³‘Pôbø&Î:cðã]ÃPWŸpÖP:®–ýèí<*ÂœPD'æïÐEJd)Â'Lyy?6§¨sìó¥Ä.!à£ÆS½%å–ãíØ4o50PyE”‘™r½œ)Æ6@YŠ£/Á§Ð«kêqfj&Íúcåö²°°eºÜ:C \céx†þÂX½¥ÚmÍaEº(œa£[YÔ­¬ÊôÚ-Å=Æþé>’ŒZ0¾z7S£¬™ ýeWF>º®â¹Íi²Ôß…¼P5'ãbjœ[È,ŒÁ˜Ž™Q'ÁMÍŸ*e£(y@tÅcì@ÈVNM{ Ÿ&iËLîÏ…¦¡—r•BÕ{šKÛµçU‹#z_ÛÌTðö0I¦ÿdã*I™Ž­•º(ìdð°ž:^¼+kN¦+•Êïæé&xÙrmL±6„ÚüA	­;þ^ïQ!{w"Ìü‹Ë	˜ä­ñÿ€›Ù¦Mîî¬5©õm êø™>KapØ9×ÉÌGC–»MQ)(ç8µlØc¼¹[O‘8·°	÷mqÀçŽÍ|³É3[øÛ\(æB¬±^‡øÒÁör•½E¤µ„Ï;ùŽ*¤r·Ü/’’RE”ü=n‘ÐšÊdÀÀvä#{Í?áv‚Ä&ÌI ÇÚõw×“_øOƒB-¾rUñr¿aB·ÉŠÍžùÈ_[R0¸u¥._hq‰Xwa} B@v(^ÒyßB›ò€Æè	CgàÌÚ¿:Ò’è›éA\RU0!µ«èo¤€aß3\EÂº#Õ»˜ã>þm."lÑ²,åê®øñ;ó\o°*OO{bÔ
$ÍŽÿ*¤½Ö}ÎK?ÒQ&¥
Ê*e»ØÎ½'h‰©NíkxJ’ÄeÏ‘Œ}ê,*O›Š‰xíubˆ‹©óS¾Ä‡œÎß"	> W»y»Yºàèco+íù<Eít©úWXî4	}aH_ˆ‡UØ@JW=Fr¦B PœSùÒšñŸ­WÞfáI–[ƒðýk'·U,næþ2ž‡-T/Ó~ÑMŸR8¦ûÂéÍ†»±îûH°
JÛJ×ŠYõ ”|§q
Éba\²I¢£Í)w`É‚(„vgut§„òþ|÷˜ÚÇ™xåç0'³÷QÊ…ŸMñ½ ¥¾ñÙsN5%˜%×š[\–óVQ}~£“J*È2 )„ej¬JŽûyÊhØëžÎVõÛ°=0‘sç3Z‹&€Ú?uáÏû­›°ŽUv€7¥@„Ï<©ßÖ3¢Ð?—Ž>d®v
2«ˆ–Qý¼`¹QÓ{ê9Š´ç@³j~À¬¢ŒBôQ“3u5ØƒBy²#hh°g¯d^Ûè°¼	ûŸCDs·Aú„-N}ÂoSCyeu
P1ï¬'åÏ¹¥xi´£ŠJ´ Ø¬ïý€-*5\%t—ŽÎ «½·},x°û6Ù«›œL–M$µ•\”•ÁÃ‡NŒ9Z“E3g2 	¨&Q¯ßãó6–ŸTróK‡/1»Ò½saÉµ¹H+cJÆ’{ÚÅŒZ7¾z7¢¬™ ýeWF7º®â¹›a²ÔßÚŒ¼P5GvãbjÃÈ,ŒÂ™ØËQ'ÁÍŸ*eõ}(}aa˜N›Lrå2"M{ ¶R@Ÿ,ƒºãÀóÏ>¯Z‡6û8¤ÖÓ
Dš'v}rÂ•»—.Tcž{…×=òûŠõÍJƒZö„Èë [Vÿ~å'•Ê»æº&x{mL±6ÔÚü	­;÷VïQr*JPØð›ÎôÅ—€é™„KoëƒÊ…Ÿ9ñ¾ÒÖˆèM(\A¿\©ûï(?<>ùÚ4 -Îlà|u§XÙàáâÆ_óqÐ¿ÏƒýáOw9Ås¯/Zß&œÏ7!æ‡²¥ÏðÄ"Ö¥„ƒvüÚ*ì?²ˆ&ÒŽRÁ¼!fGÇ‘Î‚qÈQÉ6­rB.Piµ œŽ/˜¼ÃŽ£jÌ!µO _ÞŸf#LÜX¿<N‹Â€øx™¢10 Îw‰lëp0p §V{q}Dn >¸påÏ¾ xiRŠ´£ŠO½qåÞÿåŒ=/*0,™Í$üÕx’6Ü÷&íÙÿ>ªmb>G8‡ú7å¾®°ä3§Y.«i \_o3È.÷«ÂX”ˆðD™(ÈNqì>ìá™ÑR[‘¥^µ>£ë\°r½ãg?Gû$*mÕïÆ4¬;hjkïò·³Ÿ-ë–Âãå×.1ƒW¨@È@ù­¨V
DÆÌH;‰Sœ†6-ë(e\[9ÜBÖØ(N&¦
JPÈS¹Îž¥œåKÞ2è	ƒS×¥ä"/ãZKfg²¨s‚‡yTcÆv…×0òûŠ£›JƒZöŒÈëEõ
kgý+…ÏF†/'—€  Éba†D¤£Û$adÙ‚¤VG"u(5H™¾ÏìmÍ¥íì´_ØÞœ¦!›‚œ9Má\¤t¼¢¹À'W)mÐ%ËÒ[S_@Þ¸QMa6]¢TW-Ô.è7Ò}väT‡çe‚WeÌ÷ÖÐáÇøJk%;ª:ªØÃ'y¬›Â˜½<çÇmŸ`‚˜f½ºPÅÙFÑ¢Ð?ÊÙ-+àÍD	läü{{ÃOÞ¬HžeÞ}ê:¿ðbÌb<šúå?ÉMŠC°]–Ÿ
`·?t ©bR³,Üô¬ôÉç×o«	»œO0aÞ'Me-kVD-§Oþ2ù‡§¥Œd!K† ¿Âèeù–·»Æ!ga00MÀÇ„i‰º9ž&P–‡ó‚íÙ©[W`Áê`š„ÚÉ\)V’E%U…€Ž“_—¥Úm–|Î?öÖb|¾ÄÒõoaµ¥@dÙ’/ÓOÐ„aX÷+cîæÏtý->ú²æíÎ!øÔ‹ŒÍ PaG>úko÷7Ä<Ô‡Üvy´¤R8–Åœ52¼7Xzb:xµdÆ@ÈCd U?CÉ!i4»1¤ŸÉýÍó±NŸ?å-²ÙÈE²FoT—ã+‚Ï4Q7Ï6ÈƒNpîûÞõ…V×öÄÓëDèN^«w¥m•žZ¢²E¤&,Ù:xDå6˜6Ò¨F\C¤o÷ówRnv€µƒŸ	Ð®û¥öFœ›HR™ìë…Ÿ9ñ¸mÌŸË¨i'À5Ç‘Ñ$)79ïÔ3$
;ÈPÞOF-–jí}è\L•Åû-›R}Ð¿Ïƒý°Jw9Ås§/Zß%ÈÏ7!è²¥ÏP©Ä"Ö)¥ÒŽvüÚ*è?²ˆyÚŽR”´!fG”‘Y0¼
’}ØTÜRŒH3dšS`âiÕê7JÌþüå/‘Ä Â;uêìi]ÓPYƒÑL;¿›Ñ±Ð_T¢“	+Àwœoáh+},ÞK8/=2=#K4»àjà›ïíŒx=Ç¨÷ÞOõjåŠÿ¥™5{)xN,QˆÙÑ ¨€aŽ6\EÂâ.Õ»˜¶6þm.tNlÑ²+åê®øñ;ó\o³"O)bÔ
$ÍŽÿ*ûà‡@¼'ÞA#ïoë‚“¾€Uq‘¥^î73£ýQ¦v­ãcrÍbÃÔö	¸4CWzûÿþóS—ÄÜŒÍñUaG>®g>ÊEÔ,Ø—ÙXŽÕN'•…Š"1£`0Gq\ÞÊPVŽRiF>ºBTÔOñ†š¹€­Û'¥|°KÛµ¼:;“g9	q±·$ƒ‡zK4ÐC£"Ô"ÔëÒåÝü`‚õ­„;fÄ("×ìOÌÔ¿PHðµLz_¹0$ºs’6µÔ1jzÛ
‹2†mTkvZMzØðÀ“	Ð©¨¥öFÍžHR™ºMº¸í)Rý½v ¾ñÙ+C5%˜s×š[G\–óQQ}~£“J)À2 )Òhj¬JŽûyÊhØëžÎVõÛ°=o‘sç3Z‹&€…b$Ü½¾µ—µÄoWºFÖÉa¾—zì‚a“QÆ¹Vµð'(àÛIyìM[ÐOÀ8–]™oíj9Ð*¸4Ÿß3¼3;™ÓõwÐ“_ø	ƒB(·#hháj¯d]Ûè°¼ˆ	ûŸs·A¥O„.}ÂoyeuP1ï¨'åÏê¬ÁEŸ¸³SÑPß¯àò"x(13R—¬ëÿì´&PUš Y»Ï©šQM3§ø_¼ øåºf±~·v<p<=‘ q¢òˆÂÃ³âkUƒMÎ9ïa=»ƒ¸saÉäK¹H+dJÆ’{ÓSÅŒZf¾z7®¬™ «eWF>¶®â¹Îi²Ôß…¼P5~ãbjœÈ,Œ—‘‡•Q'ÁNÅŸ*eõ~y@q”^žK•5!B1V*ë^pmîjº¢§ƒŒ½[Îe—3þ"²ÊÎV‹5wP¬ÜbÔÑ0~‘:ÀJÃ!X¬®¶ˆÖ_êàéÑaX ²kísÄ‡FêZ®YìU&8År9Où*Ð+Ó¼Z]ø"ëV»YOk/!cÈ¨‹‡˜°û¹êÔ›TNÑòH¯õ˜ñå<¼ÎÌ«T6:Ï$Õž…DD\}¬‚=\DDºŸW}¼WÙçÃsØ¿0ˆ3#…¦Ú‡lž´Ò%2gÉ"ôf5Ós•“b+°ÃëçÒj˜¯}öáÁ)æÊv¯Õh¢Àg‹—NYÜå8zÙ˜†R‚9Ôˆ9±n
3ŒDuý>œ’3ÐF¤ßÆ½? ¹G“Ë_ «? vè~ç,^Çôøý‰³‚HXoÿ_¥˜\OxaŠqPX9C=3\|»k•ÜÌƒ˜¶h1ŸãÜ°;ÄŠôŠœäRtMn)ÊÄ€`µïJýT/Ô¯w’î¿§k¿-3`4”¸~¨êÂÂ#ÿL7­*AIS6…G8ÍÚÿb±µ‚}Ž7†Rnïž#%§Øšè>}ýóTc,ÓŽ3›Ý)F¢fJ¦°…hµ`BJ3¢¢òáƒuÂé­µš¿ObFuà}=Õ0îû¬'
TžXûo´úENÖL
15AÊ:”Oœ
t1R$UÿXžŠÏäÜø	j­ß4«ÒÒQ†&gEã´W¿½\;Ï6ÈƒNp¿þÞõ…VƒöÄÔ¿Dè@V«w¥2žZ¢D²E¤",Ù:qLå6˜jÒ¨F\@¥o÷¥wRgwv€éƒŸ	Ð­¥¥öFÊÍHR™ºMº¸íS¬)Ïa°²áÜ>'én»¼ KKSPÝÄ#%;Èpþhmžjõ#à##ýFŒ”¹5‰©Ú×X¨¨ô6yØ1þwÕs…Ï[TÛ¾å¥ÏTýÄ"Ñz¥„‡vüÚ*ä?²ˆ,ÚŽR¼!fGÄÉÎLÔqÈQ–7­rB{Piµ ÊŽ/˜¼ÃŽð6Ì!µO _ÞŸfw?þ}«3AÄëÎ¸J|ÆíPT§BŒkìm6|¦HtC@,oGA`ãÇX–å€ÚÉ<aFËÇÇ—ý8§Ç»¨ÒhIGfC;ñíµQÖÍ5âzUšû6‚«Ð«oçqfjuÍúcåö²°°3»@'®-SLvvÈBl›’ãbåµÎa†V7ÎMn»
Þ6-óÚ¡o)×¼R¥ cdVÚÚ:ÛÙÄGi¢2)Tº°ÑhýyKOXkÿØáÞy¾™‹à¸ši6+KÅr2ÚŸS¶ãèdu&ûì9HøM‰ÖhIá}4Zwû<¹/[œH^N"oûI…´ŒÏô€Áväø
Šš °wo®_0&®¨'‚Ïe\7Ï6šƒNpçóÞõ…ÒöÄ‚½Dè_«w¥;‘žZ¢D²E¤&,Ù:tE´êsÊ¤VY	ÁNÍoìhbhu×Ý±ºBôÅ—©æŒÐ+ ¥Ã4ÉÛÐMiB±$âÆ³Äs+J³VµîåÒÔ5<>èTËY@)§N×ëÐfû™(Ö;\êÎÕ–µ–µVkqÜv³3—&ÔÓiµ›®í€äØKcˆcí
„›j´’.ð#úÀ/Æ’Pœ¨=.Z™„EÐžm€À"±:s™L!¨~Ö’g†X ‹Æõ#TÙ
ýr6ƒ“8ú#Û[MÃdËä äÊ~b®9+Âx‡véB-]€rTQNPa#NâCè~¡è‹ðÑ8tAÞìæ€ðq‰«ÅÔ€vgaa0MÀÀÔ8´È)Ž*@Šã:ÉóÓã"â%o*[p™¯zùöææ®/ïqï6SXLKc~œ$Ñ’·b­©Ê5ƒzók~ãÆg1×åè„p~ž´HºW|YpãÙ_î=Éœq[Ý{ßßú™
>YYgìç‡‹<ò»÷ûûÏ&;Š oÖaéýÜR‹MoØÑƒb-£4e[%ˆÑPØ@Z=FrðB PœSõÒšñœ­WÞfáI–[ƒð¤k'·
,næ¨;ž‡-]#Ó~ÑN—R8¦­”éÍƒêŒœ½^}F§gýv‰îgÐ+VÂxÖ6 É9>O©¸Á7`zØ- ­Zv_2+'¤Ý±ºÓøåèóœÓWÍº¦¸¹)Náéqè¡ñ" -Ì%ŸÔËG[À§Q7wHªM|€x }„)v¬Ž³`Ã\êçŽ–°ÛÜwQ ’l°2]ˆ9õé"º”µ—è“|ôäÊ?ñãŠÐs¿gëŸqµöÕñh	oææ{{ÿfæªTÜRŒH3kÎ\iµ'ÏŽ/˜_¼ÃŽõ?Ç_äSÛ„
4·wa(ýbNù,BÛ¼°ðXû×\D'¾ïÐAGd)”.Ly-?8¯¨s°ó¥Ä.wà£ÂS½%³Ã²Ðª-w9 U0!µ«èŽ¹bâzUšû6‚Ä¢—G›W6¾Œi¼¥êé§m€8x¤kESCî{4†’·3¨©Ê5×+ÎnóÖb-³ÄÒõlaµ¬@dÃ/ÓOÚaXôscî°tý->ú²âíÎ!«ÝÚ±¿°@mW;ãð4Ë3Û–™y´¤K´!Ý“:=èW4"{ë+²?k†_Bt)Dós/‘±‹ÞÖøò	Ÿk½.ê,µÊÚ[›/a}rÂ•»ÌuTcž{…×=òûŠõÍJƒZöØÈë [Vÿ~å'•ÊêæEè&xÙrmL±?ØÚü	­;¡_ïQw{w"Àü‹Ë\ä­ñ X€›RÑ¦Mîîðd;Ûåa°ïíýqn¼í‘ŠKK?äÏ((!Ûqài\+‘põú42Ã}Ë¿%´¨ÜÇF¨Š¬&V´cä3Æ#ÔÓ8ä›®í‡äØKjˆcí_„›j´›"ð#ú•~Æ’Â¨=.Y˜„E†žm€OÀ"±:
{™L!ø‡¯À°ŽÚ™»%²e‰PŒ‚7¨Rª;•<NËäû‹óui‚¸#1Ø|­NÁJm4œt]zy(oxl(nåFínŠç˜ÛãXëÝÁSÑPß¯´ýu6,,dÑl´€5Ž~@–ënÎ»Ìãvëez"&Ñæ+­öª¬ø{ï\;æbZPO7+Ü^$…Äö~­ýÊ}šÇYróKƒ"1»„ësaÉµN¹H+2—¯	ÃÉœ_}|Ÿ@FIÊ£xíu[LqLÆË›ž¬ß øùÛ÷	#7|™U»Q•ÕÊr~ÌÜrÄ¡¾³ce
tˆ‡UØ@R=Fr¦J PœPñÒšñ•¥WÞf½–[ƒóúk'·
,næ¨?ž‡-+Ó~ÑŸR8¦þË¸ðl“æœ™÷4.š~kNÐ¬¿`Ó±Zó&/ÆmnSæé(¶‰|½c¼){`u{« ÞÖ]÷˜ÚÇ™xåç0'³÷QÊ…ŸMñ½ ¥¾ñÙsN5%˜%×š[\–óVQ}~£“J*È2 )„ej¬JŽûyÊhØëžÎVõÛ°=0‘sç3Z‹&€Ú?uáÏû­›°ŽUv€7¥@„Ï<©ßÖ3¢Ð?—Ž>d®v
2«ˆ–Qý¼`¹QÓ{ê/)¨#žß3¼3;™ÓõwÐ“_ø	ƒB(·#hháj¯d]Ûè°¼ˆ	ûŸs·A¥O„.}ÂoyeuP1ï¨'åÏê¬ÁEŸ¸³SÑPß¯àò"x+13R—ŽÎŽ¹bâzUšû6‚Ä¢—G›W#ª£wº«ûôºf¬3G‘T<f53¶8¡¯Å[æ¥Ê5×+Î?öÖb-»ÄÒõo5µ¢HdÓš/ÓOŒaX¢~cî°™tý-B>úçêíÎ!øÝ‹ŒÍ PaG>öj>ÊE$Ø—ÙKLXŽÕM'•…Õ`žuPKaY”2ëj'“d&h1V*¶	c" *ˆ¡øÌíÙ$¥w´×çù4jã6>
W¾ã;ÊÖ`H+‡/œŸ8îçÂ½Í^Ÿ¾Ô÷X[©OJ·?¸j‰‚¼D®º:0‘ruPù~†cÆ´±s¿SúB$M
wgzYÔŒ¶¹4¢µ¦„Ì3¦¦nBÁ‡wàçü}žfÙÍ’ä\?1`‰{“‰õKÂîH\D—?¶Y["Œ&Ï%”{6ÃVß¿4Áj[é²ÇØGõ¨ôZ+VÙ"ëb5Ës–Ã:yáŒ“Ÿ¾@¡†Gbï;ù	ÚŒWÆ«oµ}öÈ@ž‚EÇìl5TÏÝI †¿Wñ\™`õqÁ\)ª(Á¯éKXþ‡Îš{‘OÒß]×Òezòi>Kçfk¤dæÎ‰´îUA¿†M\*îW·÷P_bs\Lw$yHZeòOÅ%–ú›Êø	Er„•™ûè0¦Ã»õÃ`3FdIhçÃ”râœ)’uß´pÑè·Q¶<"s("”¬}üï¥Â
²	zµj+€`œý­'ûãÖ=éJfŠ r°ZœríÀà;Õä
ô'1!Œ‹*Ø:ãµRDó>.õíÍ©4Val÷ä´úóS—é­µÅÏ,'>w” ‡qøÅL^áÏqßpÎî1(¨_}aa˜NŠ@ÈJ1UòX¿QœÂ–±…½PÎ>¨.Þ
ôÇtJôg9	S”¸+’—3i×'””^aâ§—«ÝM“ýœðC
}F§gýp¨¸cú¾´Luk‡7,\â;ÀsÂñJQ¥³O³`M&^63Ê€¾3ˆè½©»3¦‹DBÁªpœåFR°P¤ù¹óü2F9kƒ}†ÒÁK«hN~>ó[ËJG-Šjô4Ö$!´œ»
ž%œÇÊ½_k;Åcófš+Ý‡'£ëð·›¸·W;Ä.öüG†ês÷¯ÕFÑ²@¨÷."ûÕx3JÄ÷sOß^×uÞ|£LÞbä	<¸#¾ÃkAÏ——±n\àá]üß_v®4U¬_<êu Ÿüß°‘~b®9+Âz†`þaW=F?†/jQ&wZ^øBì¼Úº¹É!G‡ÇçÒUýºÄ«´ÉR5lfN0ÑÏ•%²ïmžiÓ²I›þ†µQ¾88;zJšÇØÌ“ŠÁt·{³2B&‹`‰Ë¢7°¨Óm»$…lTÊ:|=£½±Ï äÉ5Ê)&1ýQ†V”bÖjÉÑÜ=¸0?VJo®¢¢ž‘;úÎã×å+[fé=pžTC–ÒÇKuÛÃT7„ÝÇhhö1u}aH-Ãcìi'ÈB1Vœ{%i–µ‹ØçŒ­@ãÑxÇfñœ•"{¼>
W—ã¸¾\D;ßn‰¢t×Â„‘œØf£Ð…¢@¹LkNÔv¨¸c§/'—¹Ycr‡bpGÄé^àù{.(—5¨§fnw½žÿìiï†ÂÏrøî6J”ãâ°Êm¸J¡$è»Ì«iv•|‰Å’4SZÖÔ %/_—?¶Y["Œ&Ï%”"#í\Ë¦uÇGaÛÊ¤¿F¤™ôIiVÉcïdœòêr0¿Ã¦ÊÃ«…WÔ.Ùô3ÖÂ<ªù,èN¹ýã×Ð¼R"ÍÍJ» »(‘G„6Þ6"™1¬%‘Ìj«þüÌ»%Ï0äHÂŽmîa,(Ž&µ5!‰ é­ò\DŽ:ÿdíjå` n³,8/=$x~@u¿TõoŠÓ¾áÝsFÙþ¿ÂSþ4¯Ã »ˆn:}_L}	ÙþƒaâÖ0—=}³¶s”èª&±02M!•«D·³øæäs€v¢{OS*!‚ZwÆ6üñ›Ô[a˜Hwø>ìá’ê1qŒ¥1×<%ºåM¼s¨èc
Iü.*J»·ôv53ˆòáÞy¾ËˆœÅâ	# º3`ŽY~À‰ZÐ@2‰…“ss÷}-]\|
ºr†v&ÿHœNö×‘©¥qçÒx·aòöñ#H«GaüímÔìc9&)Ç‘F3ÓÁû¦ìF§È…„
Vö=°à{ÂÜTä“Õ9c„&¨rˆñB~¥ú¤QJ ZMST§´ÚÇX•«ðí™ZÍß=ƒÿð³˜?Ü.Ì2õã¯ÔFzx„JƒÐÞ
4Àí:ReQ¡8µ!8lØ>°=Ñ?f¡–ë%¥0”¦õ„nñË¸S{"cï\Æb™¸4êº”œº?èÈzÖ ß½‰‡aìË&°vìÐ4ÊYIÍ¯&2«ˆ–Q¿Wñ	Ð.¡!R*Ö7¸+Ú‰"Àåš˜±7îX–ß]×Òaq¬=+ÜX¿<N–ÉŠä‘ ëêz}/Ø]¢]ÉcÒ*[W!|pXtú•Ø½Ö˜¶A9¶ÍÀ° Ï™íŒÕh"|_nÌÄÉ2Ñã[áT(*ë®sŒî¤²z³q&M!•«7¦£ïî½nöWÜgn*
&*Ä=V¹ë†ÎÀ´	é~@°I+ª]’"BÿÈ‹ã;4É¬sŸ^^sŸËm—¶È{PÍ..F÷»¤RÄ YJ6Ýöò²›(ð‘Êœˆ°PvzLÆ'{”I×.«Ó]M… wÌB“Ç"Bç86@Ù‡?L^M9'BW­&i‘
¯Š’ÖØ½‹pã|°bŽüò3/irýðjÑÜ;vÂ~’&¥wÔÂ‚š™ÇCû”‘²W¨(§môDÇÖ³a@ú.µCre‘o?„B¿«Ð7(—Jª®LL34*>{œ±ÏÒfÊ½ûïê€Ø„å¾ëø}iµE± Ïð´ÇtR0.íîÜÏ	Žè*îTàNG*ž2àÐ02ýÍ¢/Ÿ7È¤ÃÒl©–ôkÈ%ù*Cœòê
SØÇùû—è§qùòÅ+úèö[‡zì”z’U4ŠÔJ¬¢-0¼#Ñ0œv”oõ#e)À?–§Ÿo‡P®ÃÍÈ¸OÒß]×Òezòi>Kàbk¢l	…ú¾üŠ<*ÂÈ_2ãf§HÜE *Z7”Lx	9z}qš=‘{ŠÓ¾áÝur^ÛÇ÷“ì·Ó©»› ]"ÝÐaézÓb/Û¯w¦é•µh‰n9B{ìÀßË” è#ÿ r¸:^ T_?€`œùý¡¹Âd–'Þ§R’{F¸µÙ€UX¼^µXs2˜ŠjÞÒœKmL÷$o¶²’x¤~P3¢¢òá…T”í‡œ•°%%¤<g—Ï!Ô‡‰XCÊ•"c[¢×{iòC<[YµdÆ@ÈE&h1V*¶R!jÄCáÞÚÖÈüE±Zˆ6Û¨¼9t§Pk0ëõ0¿½Xnõúið<]’€­”ˆÆG™ë–²F¸Q3Ô‚Yýí;¯GXû1½Yw,™%½gÌ õQH´xÊl¾2#J ZMu×ÿ”œ‡ç²¦éƒ„KMÎ¥R¹§º6þên¿½îŽ=T6:Ï:Èž…DD\_‰¤NN~aKµAòhÿ8Ðq	Þv÷‚
³)´˜çùl‹¢Î9FókêjÓ.§—'0¿Óûíž8Â“>P¹:ÄûŒè.ìÍx¸zë¯pŸÖ Éá0gš¥.4ÃCÜ)Àv„.î ^"Ü#;¸ œù)Ôc
ùÿ°fßXŽYßÌÑ^qîP:e·4ióy Ëù ôŠ<*Â”+îZã\ÁJpp%Ò7RJV-t}ZrâTûn¦Äâ¥‡E:Öþ»•ï9¼È¦©Óx4ze[o@…È4öÕmËtN·Ñ¶É¤šG\3hˆ¤°¾·»Å	ÖC·id.7ÌC$Í‹ô_—Œ÷G¿GaÖ.‡z¯EOÎ«§Ä	™êñ:ŸÖwŠZŒM3Dû#PãæÏt¢7^WQà÷´§¡5ûŠŽˆ I@}O´_óÄ<ÛˆÆBH×ÃcÊ^žÐnà%dKKaµdï@Ètämîj¼ó €ƒ²‚m½|°b€˜–N7ÆEp,÷öÔÂ4o›+Ë^ÈV*‘‰¶Œ´9úlè»’'|Ö"(Ä’E…×¨M_Â€C4)ÒOKu´ê.çŽY
²lèIìhxhu×ÿ”œ‡ç²¦éƒ„KMÎ¥R¹§º6þên¿½îŽ=T6:Ï:Èž…DD\_‰¤hl[8óQŸcp±WÏñÊ{ð”³+¥•ñðfƒ´Ô5!È*ñwRøbØŠi=´Ï·º˜†})ù Ÿ¹½Sƒà^•ZÝ£Z¤æJ Êàh.š¥.@» Ä2‘MÐAÃsê.Œ¥ôV®awÅ½µ‘@ÿÊîüÇqék=hø},©¯U’ÉŠ¼;[Æíy6éMûvÜ*K8›HZP@=t~VA2§£ß¾ªüÒ<)lË¸üä4ŠÄº»ÆVa-]w±Ôˆzð/árPØ¿Ô‚¦h´:w`Üê+îË”‰Å	Ö#U’S*/:
Ìz™Ç·{¦ˆðD»$—vŒq¢STÔº§ÎçÀVú7=µÍv—C€ÑG}Oð.&[ÉâÜ>«yUZgëÝ°´˜?ÁˆÂÒœ­]}^KÅ[i÷7í<Ô‡
Tž‚R7™[Útiõm4%AÚH§zo+X)XÓ3h?²*¡ìÜâÞ/¡P„æÙ¼§JÆ>
#“’¸Á:ßëoæ7WŽ¦š¾4æwƒò°„,wÓ( Î‰N÷í,lnÊ:’ci‡6y#½&‰=Žá
²õ®bŠ˜€šîë:;¶¹B¡Å—€‰|èâ-=²Ï3Ò üwR´\ìz˜Ì«ovc|ƒ‘å)/:9ùÙ$ 'È`êi|§ZÄëÒqê„Ò8Ž³–éGížî>q”l°µ«o·MRÀéh}KZuÞÅâHWºFØüÀ¸‚u¾wë™@…×Íéhk‘“)7¿_Û3Œ¯z¡-X/Ü¯3œÄDÖ
l«þüÌzØBË^¶ HƒÜX|îfr¤4PÀp…ä½ä
-Áî}~VÈ{‡`á{,aÚ~[AQ14(>v>ñmýøÑõNaúÛÖ»)ÈuºÄ«´ÉR8lt\uÝŽvàÙpádÜ÷y›ï‰ºQ°(0t(,„¤2èû®©Å	ÖÜC \P`n” }“Ö©'ü÷Ž)ÓLr™I|Œq¢STÔº§Ìò×+À= %®ö@¼q¼òaoÜ=nø¢€cÀ_rJ3ÿØÌô/ñÃœúÒ$>œF¥u­O èótu0êî"Qðg©öHBÕM3?jZÐg_Bxnó
OËÕ 6IVWÌWÔ¾x€öŸçJÆ%qD×ÇX÷ùLr‘*ÌS†EÔ¶ÿÏ‹	ÚFæó¦Ÿ!jÏ%#Æ‚Cìá>•ptÁ1ˆnIF¼(†HÈ&ƒêzéc®§ZQnxñ›ìYe
t‰HFÕ¡¡oëƒÿæá!i¥ª#ôû¨þ}ma‰x‚Œ›[P~z¯þ%v;îÐNV%‘Aò$Ò7ªãðXðn$‰©Ú‡’ƒ 9mÔ*ÀaÁp§b7ñŠ¾¥Œ8Âá>P¹ ïÑ%ýè	‰ì^ôzì”z’‹Yd®‰_VÇ€kòcûªl³KÈcÚ=¤/¥Íj”JXý–ÚªmÕEÖ0â
ZÅŸUzãf1O³'PùCŽª©ùŒ	ÆíyR0ã	ˆPg_#›yACA(ogWcâP°)šõ›ÜõCvþÜÌº6Þ	šô–‚äYFF}R>ìî¯Höìwœ/Kx°ÒkôÞßWy=XoÎå4úé±¯ç,ðC8ù5LSP`aË];Òà}²ªÕb™(ÑNqìÉ52¤ÇÍ¢p~žªQºW|{UÅ
î=ä¶G)T÷8!ÃÓë 7|a_÷ç§¤º8ê…®éæÒ?/ž'wX }€Æš  y´Û~Ý²ñ^TÚG5?aö)¯.k·k{'T2UÆ5b?¢*ªïÛóÛ. ]Œ>ôCŽüò3/i0úýs›˜zÒÚ7ï‰Ñ³$X$k¹YË¶í”*gÙ>9Ô“Oúâ-”}k×9ˆhD(€,%±/Û^àù{.Sò*£FŒ}mce¼þý~á†ØÖ•néî =§ß3ÕÜÚW¤M«5¨Íµ{}p˜<Èžª£ÒÅLÑØ››«XÞ²&IAòå8Ú%~ÃFž¤7ž8¿¨ÛÂG¤šåGi9x’	cÒ–—?¥—ñ÷Ó\¡·Q/Ö)ö£XéŒWÆ«oµ}öÁ)µÖBÆàd?`ÃÝB[éF×3ØÐ>ºO0BÖ8¿fÐ‚oƒSA«þüÌlÙMÍOª_LÖÝSl¼R¡1[ï!LÛõ°öêiÿ¿@G¡U·ÎFOx¿G{FKHY§Å$íŒï·‡E:Ïíý›°§Ö¢ÿ›Z	g^9ñè¾GÁþ]áV7;’²xî˜ú5ßW_{h®ˆ_œÁÒ@š%A“2B&‹`‰Ë¢7±ê˜)ßGXœ8¥‰xyâ½ï*7×ÚðziGÚ‹é>’±(X ²j(WâÃä¾!	e[}÷î¦éŽ:ó”‹šÚò$­'tœfÝAØ—]_×ÙQ'ð¹=³8uPfK-Åcì#â5!~n¶=n$­*ž­ïÝóß:ºJŠ(å=´ÙÈPŠ5cGÁÞBþã6D¹çhû1Fö¾›«‰^ìVæÕŠ¿[¹XI1†ªÜu™×Ißô
’OKu†D¤£Û%akÙ—!`of‘¾ßÖAóƒ»3¦Ý…ª2ÔÌÜPiƒa„ÏÐ€õF)@J¶Z«åë,.,6óÅ"5 Lï\ÑX
Œ>ù?Ð4>µ‘}”2ÆæBqÉ]K£VÆN•
Jªñ–n7¥Áá—ZªŒZ3ï ØáÝÚ?ñ*ì(úŸËœÛyvPÃÌMTéHÇ;žrÆ:Üy7A¼	0£2Ë”@”^ò—Ÿ¬PÓYÙ	ßYÍ“8·4UØ1^þnNÁäð§ÜAÆíy7æKãˆT
&F%É?,9CcPqª¨:å…„ÍŒ}I‹Åµž¶—–ÿçã@)0.&à‘ÔL´„D‘4@Nôš4Ù°íÙ¢(8{}Ãœ9îË”ÂÕeX”^*i#,
  -Dºìç;óáŸ5Ÿ
ôh7¥©XYÂ¡½ßôÌ(ÐP<6ƒË\ŒBÈK<E¾%-Vÿéö:¸3Qlæë»žœ,ø‚øÐÐþIpJvæ_óFéý‡MÊî0GÊ_ánæt!X"IÈBÀLŒQB=slð)=ÈSíŽßçÅéWÞ7ã|°b†˜–:Åg9v1÷ü+ýõ-R ìð;F™‚¼€¢5çc”è»!nÕ(6À	‚Mìþ>ŸprØ ’nBO§j4§rØž¤JAø;îIì‰è‹àåì‹C*lòqÅ—òË4¥ä&&¨Ã"ÅÍÛ]ic‹ÖÛõW)1|Žq‚ÉƒPfy«8*DÕwûe{ª[ÑëÛ{ðŽ
©3²“ñâz€²ß<GþÐMBÒ–—?¥—÷ûÓP°ÁuŽe4ÛõwXÍvØ[ý‘¯h`Ñíc"þÜRÔNÛ4§F…zõ+W.ˆNiý}÷¨•W_äË©±/ÞNÛé ^ÖÕ\C±;+ÜX?êu Ÿõ¶›ÅO©ƒ åL¥O÷H&lÒ'(>@~{R!½èh¸Çä˜¶Ar[Þê³Ø_ý(»Ï»æ­^
!lÖÈ•)¦ã|ÝiÉù6Â†êÚ~±0&/UpÐú)Žž®±3ï11ºnXf>Ot§?DÍ’ÿbÆÝÊ|î7£Znáë†âëÂøÌ!Æ=%ºåMË^‡ØG)	©GE;ÿæ2~a\ÇÁ—ˆ¨¶‹ÅØÜù?"2»;ŸÂs–ÃÈDuËËL€vÑÜxyúq
'BçƒÕK|í&iÄCá™ÏýïÐ+jª#ßŽùèog¬C,1üübÛè7}™b™ßf·¿ÞõÔ]¾(ÏÁéÑ.šzlŠM«dÌÖJ•`i×<ƒ~ST¶;€H¥¹Ð9tzÉ
Š#œon	up½‚äõlæ‹ÉÆˆÙÂ
ÙÕ	¶áû}S©ên&S	 ¥Ù¯º[ï0a²fy«hl[3i8µZ]%œ>ßðÃ`ûŠ¥"§Žàólž£Ï*]ôÚ\,òX»·HùÂ÷ûÃ—œ3Þ+ÈíE›˜ìj»k±4_¤uÈ¢}‰-2«Õ)7» Ä2‘MÐAÃsê/™½ãL¿}~Ý £ˆJî|ø!ß9uïçJØI^’yÑ4‚ªôüîE ¢‰'®ìŸiìÞ€“‚?ØÎŸðˆ¢9>×I‘þ–Êï_wàÈÔ¼ Ë™òŠˆd9}eP5Fµ«œŽÆv×bP:øŸ_°Ô²–O–S$¢…V†“ÓÅ—EŠ"T‚U-	!:Ü-`ÝË¡6øýÓb™˜Æ6À–à+Õæ1,#2R[Êˆtœ::ÒýwÃX‹ØK8ý?;FÿíÌeÿeK3‹÷»¯ŠaÁ‡Ó×ô	$($º4t¡ÐAÏª«$_ÐÙL!æ_ÝÜxyúq
'BçƒÕY|õMÄIáŽÉäÜ¦vä»ÛÛ¿¼7b¡G;S”•»Ç6	kÂl™ŸLS†ûÇÍRîªÈß* M%Êí;•€1‚ÆEèp92Ùy=ë&Û^àQKô7úD•J ^JP-·’ÿúp÷›ØÇ‚aüì*Jˆäóð¼";Û-¬'¸Ýƒå[2FG¥V¢øü.CÂâ>>;î\ÒIj—|ô8Ý$éTØl§K>‚£Çßl¯‚æeØ-¶>W—?õé"Ü½—µ—¯GýÞáæÒ)¹ÎnôcáoÆÍ ÍíR4JÇÎšZÇ3‘]Ü>¨y7A¼TÇ;÷¨êH^ù—ÖŠMõeö0Ó:hüútRÂL_ŸgÊ] ¿Ôîag…¸6=Ó|‘vîq1{¼?@LZ=%LG\hùSýsü—ðºœöàrY,ÿuöPß¯Â×Ý ]ooAx]÷ã¥]ÍïKûG4*ó•\¼È´œ@S9¨žBŠ€ËÎ‹W"?£sTD ;np˜Úær²ªÚˆQýÊ?ÞSÉHë†âŸ€U\»óüs8®ëJ¼d¬è}Nõ#!WÉÃÆ7¡4CaZçï¢¤Œ8ê‘ÕÙêÀ'( †F¿o»Z¡éâye:–ØyÍ¢Ç:tý|0Bn—AP¡æI‰Ô¾ÍðAÎä×*¡w6§ƒ×qçQš5ó"¤ÆÙ	S:tNØÑGæòLr‘*ÌS†EÔ¶ÿÏ‹	ÚFæó¦Ÿ!jÏ%#Æ‚NËÕ´GrÝ³Awn¤v…!‹ðV@Â’(€kf	".4PÀÏ“PÆ¬øñïƒ„KÔ BÜ!*#¶+óy‰'\ÚvníîÄÃK/Ò«5'ÅZÊXF%•{­c„a}‘8·¾<”i¿³ŽÙQ©žé%9|Û%Ä5^ú-õé,¸Ùê¤j¼È\8Ô&ÄÊÁÑ<“Î²3¿Ð/Ñ¯h`Çìl$‹ˆT^ÛZ‰Pò “fà0a•,£/Ž™ê7AåŸ±2“HÚÃ]˜¾0÷l5`ìp®,\°œ°ôìL¿ÜeÄt“˜OwaÂJr-1qžxÚOœþ€ÆùUlïßÝÚã9°Þææ­^pf S?üè¨KÖõZûO&0’´tâ‰Œa§)"{%Íìt·¢÷é—aª
q#>+'-vˆÄ©ñà”d‹7×lTÊHë«ÈÂê:%òÈ.æ!™×o—™ßO!ý(+[ÿßË-«3W~÷ì»µÒi·ßª¶¼ímG
KÅ_ŒRxÔèãic=áþ8CæmˆÖvBÇy;4cö1 )b¼}y=W#DÕ&c.Ì¨Þ±óé[‡p¼ÂBÔº*†ßn¹þÏºL"Å	º_õGæië7A˜·‹¹9ãeˆã¢’$wÕ%Nî)¹nÝ›Q×(P“ªBdÉ8€O¿¯Å2{cè&«9—^G,ug¬Ÿõ›LÁ¦é±™JŒÂ
„òT¹§#Øþp÷Žt@{o©vníîÄÃK/Ò«5'ÅZÊXF%•{­c„a}‘8·¾<”i¿³ŽÙQ©žé%9|Û%Ä5^ú-õé,¸Ùê¤j¼È\8Ô&ÄÊÁÑ<“Î²3¿Ð/Ñ¯h`Çìl$‹ˆT^ÛZ‰Pò “fà0a•,£/Ž™ê7<šú†°aØX‚M¥]˜¾0÷l5`ìp®,\°œ°öìL¿ÜeÄt“˜OwaÀJ|&#G>‘ß¾×øXlìÝÝ¶ Ý›Ž¦³Äh/0;%tÑÇÉ[ÆôP÷Y"0ùž_¯ÎÈ¼l¶4?M5•¾r¸£²¦§a»~‰xF$vÈv™Ë¦ÿðœ+éBbHcþÏ‚“ï‡Vq‘¥ð*™Öq±RšÉN%â)"BºïË<¤<$UKhä®âíŽ,ðÓ…¹[@}O²_‡0îþª«[EÚ2Uýb´ìIXÇG
"PÙ ’?~¯vt$X:^Ó s&±-‚ªåÇ„è€jà.ÎK’ûø"oæE¶ŸV9Øx±¶¡Húz¿ÉaÆø¾ðlº/ÌÁéÑLùFÈ™Bìí8ŸciÁ'¹Lutˆ,5#S®¾Ë8Pô-³^œK	.D3""×ÿrñ	PF"ìÐ±G6¼W7˜MÜ.ÈKíŸË¬vc|ƒ‘å)/:9ùØ$5-Èjöbb¬AÁ$Õ?2õFÇ”½)¦ŽââvŸ¨Æ/JéÑ+ÎxŒÛX-ñÞðñÒMáÇçH‰UTÓ¹½!Á¨“QÆ¹Vµñ''àÛ]qþîmqâoàu‘G”kùkF¿TÇ0•Ë{ÀqsÔº¿š]ômû0Á6nüútHÒ[A¤3Xëu’›Æ‘ÿrt„©X6éMûvÜ&K4Š>
‰±ÛëòÃL•ÜÂ¶ûÈpNGŸ÷ñ–äºÓ»©É`2$2,Fµ«èaíÞm†YUÕ¹râ¿±{´;$CÚÇÜ³÷î¼2é3cöuD,‘r¢Îª<½¸Ú}#÷6¢AÆ0=û‹ýd\»Œý!tPÊÒv^‡/[-˜C?GøéÝeï>V,¢¹ßË÷)ý‰×— Q}E—b!¢ÖAÒËÕLŽGTô{Íƒ*/È@eC3qåAÔPØoœ{c" *ˆ¡ùÌâÙ$¾Y–YÓŸðän,Â`:i8¶×Iöþ;Iºìbù; ±©–¬”9ÜW²È¶ÿî.‚iÐÔöMOúµrtu$¬hÉn×´_)/”8ÊlÊ	gx ?P»ëÀMÚšøú³RØƒŒúQ°ç÷}_¸{§4öôš˜OW)9`‰ØÞG[ZK«h\D—?²&D#‘z°öÕmíŽ¥#¦öêßU«’ò5}Ô"ó\:Õs‹r+´èÎÒùjŽ¡õòÓ9úôƒì2©zì„'µÖB Êàh.Ž‡`mðcäJÇýúO0Bú>„¥ñZ®znÀ´¸šIø`ë*ÒWUÍ×_`®4U¬_<ésäÏ†õxi”µ55Ø}“o™`.I4€r[Q](qLcdäDíy°è™ÀòE|ñ°æ›ùeŠÒï´Îi2a)39èçÐÅ=÷³x°Òcå”ëQ¦}9p)ˆ•t ²êé¥fâ]'íi)	!:Ü-`ÝÍ­6ôì¥/ÃHa¥WjžëþŒùngîñ^ú7=µÀv…Q¶ÐG? ¯j	›Š ;¥4	.òá¿±ÅT”íÄÔÔâ@gW6º<{Žéý×ÔCC ƒp–Cß“!™%1š^×P³`QIF8ËT@ß£Îš³ïÐ+Þ.èCá3Ë¦Äw%’E0e®º0¿½\mõGæië7A˜·‹¹9ãeˆ”µFè^]ŠMÔbÃš%˜fd×*ŽhUE T¦d„:ƒÛQQé*ªïPK#C>8Qž¶—Ü[Ì¡ôÖ¤KÊÍ;„äT«µ¥0;Û-¾Lš›á2T6r…a¤üú"Ãø%fùXÏ #šzù8ë33úTåòÖW}²ÀßGáÇ©AV´>’	ªõéq6¸Ó¾Úõq¡l	õòÑ<ò…¥Ä|¹açžkƒÃ6ôöh%LÔÚAbæmü¾`¼ZÄeà2™	´Šj‰PE¨¬‚åfßHÚ©Pçã|*V¶êþÜXM’g¤†ÄønsŽ©4+Àwœoáh+}y›yQ@L`&9A÷b±—×øXlíÝÒ¶ É“”‹´Æk2keFhÙÍ¾DöÕjÍsåC·È´œ@ú(?|x¾¾;¼¨úå°*ðCS†\QñÅÍ¡Cî	÷«º;óñÂÂhœ7ªl‰oiÿèb`¾sŸq&=žš\—†ÞF8IÍ(:TðÛŸlnv=7{ëì¦ðÈêÄÈÞÑù	3©4M–XŠ<É‡‘'~·ÎvË×“j~þhn}aHGÐ”@ÈH^x1›x%‘¨Š—«ÇÍÌ>ã|°b‹öñ7*íZ0lÅÀ;ïeVFÙ"ÝÉ8îñ±ˆ­Vƒôç¼ë[{ E;¨uí0•Q×($—xÖ$YB­#šC®µÔ1jô-£»J ^.<·’ÿúp÷šØÊƒwúîLƒîÿ×úlB¥M¨$¼ý£Å{Fw•sÅ	ùé7ÿ[–|ÑšXÏ\L•žëzÕ 4”„ãæ@¹…Ò(|Ñ7·s	ÊfÔŒe=¸ÞÁ÷ÂS®ÄvÀ:Ãü˜‡s÷¯Õnúbç¨oc©Ž{9VÅˆkòcû«l¤QÀ$N.Ç(+¨'Ž“@³}cÏš˜±nÚIàåLÆÁ[lò}=OxÀZ'§Å–îwu…¤$;¯LªGÜ <8œsP]f2©ÀÈ§WÚ©+†ô(DŽéý}.[ÿuöPß¯€ŸäDFSmR9çñ¦ZÛöPòR5'’²xî˜ú5ßW+}6Ž£õ‰ÜÄJ€>R—^<a*<° qœÖþÎÆ¨ß@sŸ
;œGƒwmîšƒù*#ÔÚ8À6 5¤ŠvŠY„}% û$+Wî©†ww¦Ï‘þã³¢‚¬äf"_rw&¨@}=ÂXTŠhÌøÕEÚÄHÖ^‰Çspö%d@PL.±ZÐg_B~nÿs‘§¥œ½ñ¦vä­ÔÊ£Ã37 Wi7Áú~ÔÑ
~‘n”ŸEÔÂ‘­œ“æÌ‡¶}oä/¼y…˜JªWC÷ç 	™7/½;Â7ã5¦NÍFÊOJ*ZjejÈâàë	™­¹›Àß_ƒºM¬ËØIá÷È¢ðù"KD?Ð;×‚›Y^^B‘¸ZQsui<ðnv±AÃúÌuðã<”4˜î•»9ÄžæRKù
Ö\8âU½ªQùØüñÞ\—‡F.Ä&Àð@ÛÕ>¥ËE¾fä–3…À ÍÛo#YÇ÷HXØ`È ýˆ97A¼\yâiÚÅz”}|À€‚·]Ô_ÊôWLÀÞJ4¡`:t¸;iýiŸ¹™»’+«	ê¥.v5"[?(>?rzW\NÕsÑBŠäšÁã	wGÚêÇ€ì)çùœžòR>wtIwçÕ„yôÕkßrß„F¾Å¿•Gž	@(´„X‰Ð¨½j±/‰nCI*6Í];GQ{ô]hÈmåmUC™˜;ý$¬Õ†â™€UXþÇ:Ü1?¤æ\³p§ãdlÆÿîÍ=µ|@:4sˆ¤®—=¾«åøüÙ?2‹SœIn ÕÄL^Fáþ>EæB“Ç{vöG!1AÊ’\gm7_5^Ù<µÆõýŒôŠ{ X•DAf(á×"XSÌ%ótO›¦­7C£P¢t«¢œ±Å9Ç©Þ€²|ÿó.°n˜ƒZá/'— µCb8¶6a«b‰:µæBCÆuó;ø2#N_>).ÎÏ“VÊ¬ôà™\ÙÍ=ï¶µµ)Ü.Ì"øó³8[iveÜ¼ bÔ«KA!;
óA„!8Eˆkþ8ÀldøWÙˆwÚ\\êç§ÆP ‡½Xk8qÄ[Z–NÈÓZ½Ã¥óÕøÒpà¥^ïïjýú*ìN¨À1Ú‘S[‘©?aš˜» »ºm¹GÞñ#	ŠÒËq„[I¹ÈûÏØJ— Â;uêìh]ÄJF”zYýx‚›ï±ÅEI¦‚\=ãPªvÊQ.>s\Lk+huU#mòY±6è‡ö˜¶Aj>µ‘³ÒP¢}²Ã»ží]$mrzyÍÍ•<ôÓtÎ*VØ¿Ô‚¦h´oO[pÍºn»¯ê¬ø*äaßgn*rECî{”ÆïßÁ³é}Bª>¥GƒhIùƒùnâÆ,Ê7& 
ŸÖ\·R„ÌG#Aæ?=WÉÐî’2{jKÐÝ””°Ê«é”Àù9O»r{”YdÝˆŽ—ÙÀm&™x¾á©ë,Â›²Ý‰)²ëj'§zo+X)YÓ<h?´$¡ìÀàÉ>¼6±Þƒ¼§J²g9v1÷ü+ýõ-R ìjë!I¸¿—·©ÖC²»ˆ+qÏ8ó7¨úæ·RHì¨Xde¶2ŠR¯ÂñJQ¥³FªQM"R~huždsÔ™èÎÊ”Z¾¬#<µ›‚î<ØQ¬/äªžÕ2{q‰|¸ÞßÃ¶PQjCn“@ÖBFt§j°>Ö5/õmÜ¾3œ<gÔš•»9Ä‚é/8‹ë#År‘ŠX;¤ÑøÊÛP¦Èz€t Ÿe×ß;¾‚0ücáoÑ¯h`Çìl$‹ˆTHØCÆfõ#ù~ô,S?ˆ^=¨!¹€?Û3;°ú†¦bÁ_±Oøë
,ß?kM÷>B¤zÛôº‡üaû×B/·››yUxaÁ&{* L#¬:’šõ›ÜõBvñÜÌ¢8Ãu¼È«¸Ø$l
!uî£PÍùFìC30óSÑä‚·g»9g4ˆ§~ù©üä¡j€b°|OO&'»a›Ä>øëÓp‹.ókW¸>ì=«ÈÍ¢84ÅÆ3Å'&(ÑvCÁÌA<P¾l PòéÀ¯ Qe7ß®âíŽ,ðÓ…¹[@}O²_‡0îþÑÎDNTñï9^ðt®öNBÒ~!VÝ’Rq»{y=^~æX?°¬ŽÏûÍéœ{‡!ý%¤ÓÕCŠ8lFÐÛ_ýù}r‘:‘tÛ^a°¯—½ÔIœ˜5B…°$%Õô+´|¨lê ”Ñ'O«5–U¥®Û&ckÂŽ*—z{oC9#?\Ñë¶¹D¥Âëæ¯ZŒä&&¨Ã"ÄÍÔ]iB±$âÆ³Äs+J³VµîåÒÔ5<>èTËY@)§XÅ÷	ÒË¢;Ž_”çÇØW¨©Ut³N÷9Ã‡•6« Uê'D[uƒ å9!½E¤àÚÃb“Ö:³qæ™vµÍÐí`3˜0¼#Ç4–]ÈQõbU)Ñ0’$Äy»ÍÈûÏÄEÑ±Ic×“Uzãf1O³'PùCŽª ù‘Æíy7æKãˆT%_jÿ<F\(o3\qâYñîºÕœÌ=ZË¥±–ê÷†ôÐª-^icEl@š‘Ð$¶ëAŽ2(E‰†0•ÿÛµlâmlQ:Ñú+çÆ°ü[ï_Jü*MON}{É@#Î™ÿp¦ˆðDùlC·(v¨^BÛ¯¬¥6?Õà¼C^^sƒÄ+¬u­õkr×	
{ÀÅ7¯1^aa÷ö¦¨“<²‹ÅØÜù?/ ©~}˜Yu«ÅÔKL+ÒÈ>„ÍššcXzba—AÆKœ{f2bxÄ_ˆéŽÉäÜ±]|¼Ó4™àú!Lö70rîíeÛÃyT2ÄC£"Òs£ÆøÈ÷ÜK¢œ«™,jÙ(5Â‚NËÕ´GrØ ¹AIP®:€J´¸Û0qkÞˆ(ëJ@)^o.‘¾ßÖAç²>dˆžª]K²WT¦;Û_ÈK™Ýƒå[2FF¥[£îú,%,6ïÇ5$fôQÚTwõí\¾')õVž„¾.¿•ë÷w’²î2gØÙvËI¾¶I…øÐ½Â\¦œÄoÄûÑÏsã­~¥-Kå"£¦esÌé2«Ó)7¿_Û3Œ¯z¡-X/Ü¢3ŽÖv[¡ÃÍÈ¸YÖôGc×“Uzãf1O³'PùGXß™»É»8U¢‰EiØMãFÊ@!p3‡qSzX,s3\1¬:’¶ß¾çœb1CÜõãÉr‡T¶Î®¯€'wiuFu	ƒ¬ëôÅw×rMW×…%ÖãÂó5ßWv#Œº&÷ö¯°úX‡\&Ž*S}Z;u‚$Í˜ŒÍµÊ}‡u_ÎPó»5,»ÓÒ¯d\»Œ1×<%¹çM§h¹ûlyIü.*J¿»¤RÄ<qLÆË›ž¬Ý¡îêð¸//¦}Iu™ÂBH×Ä"uÌM›Ÿu÷q</	4BÞ1Š@ÁR8{ Ÿ	!jíCáÑ…©Ëø­SˆÎ©ðï2{»BCc3î´-ÝÕ1r ,ÜMÉ%1ƒçÂéÝK²ÔòS.š~ŠM ¯¿`¬MDúU“oRI 9Y¦!ƒçMJó“	±N\"u {¾™÷ç|ú—ÛÜˆ}øä*J”ãâ°Êm¸J¡$è»îŽÖÝ«Ô%±YQ§a~z¯Ä#%;Épñhm¿PÏýÙ`–¢;ž)Éü£¼NÀýö2mÝG#îIª¦FŽñìüÔA¡‡]ä ßäÑè™ìYˆ\ÌØjƒÌQûð-?QÅÍ\™vûJè5ª?¢7AÎqSÄ3“ÌkØaE°œ”¡fØsÐôUÎÖ)·4UØ'_ñhV´° «ÓUI¢¸9ábõõrB&$›yAi3\eþ^Çi Ñ¹ÊÐ-‚¸£Ér‡Pß¯¬µÁw3 XÈšìÓqßtP_š«c—â”èØT&g)•÷9ðä¾»Å	Öt»j^LN|¿*$Íÿé>é¾¼ŒmJ®Qnó½B-»µÏ¼mdŠµ\®uY]5¨æJªhºùlÂÿîÍ=µ|@:4ëäúŽ¼×­øîðÓ%!ç=pžTC›ÒÕYCÛuÝB”ìxhõ~y	%MÑ1„HŽgG_7«Bmîjºó €Œ½[Á1¿Î(¶Åï3e@u2ê°{ÑÚ%H=,ÍBÆ!j«­”žÄ;ŸêÌ‘µW¼GO¼J×Ø¿`§/'“ªBdÉ8€O¿ ¯Ð)aKú*©œqL3u2.T¤äçVÚ¹èì™nëå;$¨Æ)ÓÚÊ_cŸg‘Þº´È|!J”5ŽßÎZ_‰<Ã×þõž$ŽuÈº=º.±¹[=‘8·Â¸#©˜ýó}‰¨Ð=Vû
ÓW/õ>‘c<©ž¥˜½HÅâE5Ù+Ú.ðþ“ð_WÝµqËûÊh"`îÝPMÃ^í	—[{ä|û?‚Ò×vŽJ	Ï‡Ö¬aÕIÇF¯P‹•£}±ãÚ5áÀZ¸ÐÉŠ¿¼;)¾Ž gØMãFÊ@!p>‡cALY, "G>íb»ÃçÊÈh~QÛñú­ø;³ýùéý6Z	]uÌ×KðvÜbå¹cŸí¿¿k¼}k2G{ìÀßÏýè©qÿF7¦yPGrEG‡u‚åríð”$Â
ôh.¶]n ©ÍÀ¶R[¸õønvJÚ’0Ël±Œó'?	ð°€wügCˆ‹ßË÷Ü îõêÃ%3Ÿ\ÒTŠx‘ßˆ'~·Ä?öi¹úSBÁ]5"a„GgDecû †¨—õëÙûÂqºÓ¤÷é!qVnw£¥;›º_m`òD ^'ñ¬—±¾+ãQ²Î¶¾VüN÷$°{‰”¸FD÷*¾Xpf²{Pù*&„í´xÊlÊB$MWZMW.Ž¿Ò×çŠÙÀaÿî0=¤äÿæðFt´J¦)Ïß Óy$Mz’d’Ôõ;,=/àÂ-5;Ü`ñof¶6å8Ú%~ÃFž¢;ž)Éè‘{X;€Œç	°$ú½Ó\]çTÛÌ“Ÿ¾zŠ¬zïèÛ(ëç‚ý\•_ÖµMÂËÁü$m2«Õ)7ÀEÛ9Øf²JÈeð=’”ÅvŽ[nÒ–˜¦gîaÞë hÌÁKmâPEŸbÐRFž­î°‰nTëŽ:ÿì†Éç”«%ùÖ°Ê¯öØ¿w:’pØ½ÖàÕ&eàì³é4¼ù ¨Ôy>te-Mƒ¬ëñÙwÊ>/š´tâ‰Œl§;0IAt¼ñßÏëé¦wîZH¢:B&†r›ý£7ó¥Çm†
ôhWÎ9ïyuêšÂ§!Òè®uY]‚ÃqÃÉÌW?IæqB8ŸðÜ6¤!F,¢¹ßË÷)ý‰×— Q}E—b$§ˆhÏÁÃi9îM'‰p¥ƒ,@¹({DPqƒcìiaª|b+N%OØ6s0£-é—ÄíÉåRÕÒx·aòüúoX.zIÌÝH÷þ!3,ÍBÆ!g«¿†¬ŸM¤Ø²7Aå «(¿oÌÛ5¸WKø*°Hx)ÔqUÄé(çŽ½lè¦Kj
z$3(v£ÎßM€¸þä¶ŠÄˆã"ôýómèyéq¼â´Ï{5%É.ê»£fy«lk'!þðnv±AÃàìWÛ¯
ª ¿çúgˆ¥ß<GþÐMBÒ–—?¥—÷ûÓP°ÁuúfLa¾ÌWÆ«UžWË¹@¹ç,-ûÔJ`çáhióxš4–M•v¨y7AÈqS»)“Æ?¯|uÙº©—Jðhà<ðYÇì|MÉL_ŸzCörÓ›ôäØ_D®ŸY[p4Œð¥.E%X‡~[Qi3\eþ^Çd Ã«üÑ-,£žøvø4»Ò÷‚Ô-8{dAu"ÚÔ‡rß†-ã=}³®—ÿÑåQ¦}9p)ˆ•y  øß¤f±L*ö*X-vv,&… 4×‚¿1ðõÁ@¼'d– ,ãÆjhå–¶R[¸õû'iXÏ,‹É‡/[)â)"B«¢™iýg o9Ú²äœØ5êßÁÞ… Z:ÿb"Èf¼,Ãÿ‘w[Œ˜K,‰	Æ¾ÜZ9"wý ¢?~¯voî[mîj¨˜‚ÆîÙ2§AŠ4ù.²ÃÙox­ZiñíÆÞ87,ÍBÆ!j«­”é’×K¯ã†®EÏénà6•›gÐ+V“Õ6 Æm&½E­™ðv@î6«ëOJ*Z{a5Fœ¹Òì[Ý®ûÒÿc€›H”äâ¤¥0Ü.Ì<˜¼¬vc|ƒ‘å)/:9ùØ$5öZÒIF>mÏóÚ{òŸ¨8µ‰íâ|ƒßõ5}…ë#Ér›.vþ5UXÿ5å9!½E¤Ú.ðþ“ñ_’WÝ X¤ý$ èÐHÈÆ@XÎ‰PòTý÷-S/•3‰³ýM¥uÏ¸Ÿ©`ÜIËåcåæt[Ó@8¤;Xë$1ŸäéªÕTXâÈ_ÍôGÖ	Ô¥.E%X‡~[Qi3\eþ^Çd Ã«üÑ-,£žøvø4»Ò÷‚Ô-8{dAu"ÚÔ‡rß†-ã=}³®—ÿÑåQ¦}9p)ˆ•y  øß¤f±L*ö*X-vv,&… 4×‚¿1ðõÁ@¼'d– ,ãÆjhå–¶R[¸õû'iXÏ!Ãä¶+!Cÿ:r¦±™j–K c(îöé‡¼cÝ©÷Œ… R/vø
"ÌeÔ)¬—•pL…ã<5‚&÷º™2/mç=£.j·hl,9dòTIßnË÷Ãï„Ò9ªW‘.è.¸ÐÕRçQd7÷ÇdÇÃ!všbÆIËa©‡£›JÜ@¢Õ„
Vö9ë"³"˜Zó/'—Ñ' ÉbnS®c”§ÔPWÏ&´¯K7I:7v—²ßÚP÷ªèï e•öHRÍúøáá5øÈK™ïÌ«ovêíÅÓÀ«#;î8µW?FØ>°$Ý?2¤mÊë—’¨ÍÓ@¾¾î>q†N•#J‡c‘saŽÃ¾½f­†WÅ!Î¼DÁÞ4¸šE¨3¢Ù$ç¨BI„ñd8K™÷Pž à8›L™xäO%ÖUq¸/”Ö'¿J¹ÈûÏ/‘!µ 0~çúsGÄBO yÜY=¸›Ô½ÁTÆí}~|ã\¥@ÆA_p Xv`gB^r$!ä^ânºÑ÷ÚÞ,xZüõ÷ÛPþ4¯Ã »ˆB]IaC>õå¾@ýÀ|—zx°¸y—ø”óA[(¬ŽD…”ÑÃPŒ3C¯j 3+'1y™ò½=þà‰>ícX®3€vµIBÆ©ºÐb\»þ"˜rZ/JÆíA§~ ãkiÆ}ÉÆüŽ4y>ÿ®ŽÌôT”íÜ™ß"	>W®b²Uºøçdf ûÿ"Qìe¾çUS¿((\7L.±ÔLaª|b+N$O×6s6­-ž¸ÿÇïÉ4 2è–7öŸ‘M´YOÚÑBíä0D¼ü~ê0\¦†¢ ún’ù¶„.vÞ42È	ñ;Øž6×($åAðbTD .ŒG¤©Ö#q`Ó˜"—|vk¶ü‹ÎôÅ—„Ì7×žH-£Î4ß×Æ\bŽlŒÄÝ“øM?MV¿S®ýþ.9,6óÅ"5 HªH“p?Fñe¦}ûÕ{á™»#¨Žýâ|Ÿ®ß>JâÊM)óY¶Ï7$ýë“Ÿº?çÇuŸ`‚ºC›˜uã5ó<­ß0Åoc­ÿ=zpãìmtéy÷	§l¾IÈæ,ˆ¥òX®awÙ¿¢€]îjê!Ã+síŸ
e«SUØ)³S,¯É›ãta¸5:Îw†vût:k´B{f`S?-Ë:’Ø½ÖîŽd^qûÑÚ­,È	ŠäŽ‰ô_FVgP)ùæ¤KÔ÷Wá@99îžD¦ÍµM†>G=Í–ßÏå³äL(^ŸE1e=;¥&@¯ûÒÉ®ñkX¸4€g©T1»•ÎÑR[¸ˆtœgx5¨æJªhºùveÜ|ÓßúŒ/hjGÏÇ‘¹Á¢îðáÕ21FµsÈ,‰‹ý  }Å˜QXûo´úEOÖY/.cñ £?}¼yy6N"CÛ7s&±-‚ªåÇ€­ÂBÕ{³fñœçq;€(wIÁËNæè*\¶ ìtü*GŽ”¦Œ°#ìrò»!oÄ24ØˆEææ%”ãY€ 	’um3‹B©µÖ3eaÂ‰!Šqly
{¬™ööfîÓÊ’qâ‡TÍÖpœ˜?ªéÒÖˆèM(\A¿R¦âõ($>#óÆ$>	*Åsö`f	ªAÖúÓ|’û(Ö;\êÎÕ‚µÄ3VïÞG5àW«¼DœäËØòjŽ½}ä ã¹\É›Á¨§"²ÜP¨æ+ û×H`èænxõ~ý§x¥OÏsì#	Š¥äV¬jtÂ¬°Aòxð!¬OAï7Žti!ýtÛU'´–Å…õni…­57Èwœxýe1f«Hspz
I\}P1êÄß¾¤¤Žd^qûÑÚ­,È	Šã¡ºÉc2FCGsÙÏ•KÐÕtÎcÎ®dœÔ°”@^#³•]€ˆÝÔ‡Mó\júFn*u~}È=V¹ë†ÏÀ»	éki™0¦l¥urç‰Œù Ôèð
2 ˜Ç\¥b§ÿvn¾z2Ê£Q¶dOqLÆË›ž­Ê»áÉÐü?(¤B½s»Z½ëõhx+øø3Tíd³Ÿ*`¿DXzb:B©"j¡qt0T7NÉ4Yˆ<•ŸÄâóÛ. ]Œ>ôGËè°ÅcH1h²×Iöþ;HºöyÊkªªœ±¢6ôl™ú­—<fÂ( Ò	ž_êüFê_ÂxÖ$m1Þn>O©¸Á7`zÏ&¦·^G3ug¬ŸõŸ	ÕäÁ„Ì7×š\N®È9ßÁÊJs…{„ÂÍˆïB.MJ±`†ßÞ
/öÌ/>(Îpíst¶]Äú}váâÆ_ó`Ùëáôw„¾ß(HùÞJ8ø_¶³RŽæëôÙA¡œJöãÖ8ûùvüß6€ˆùdØ’N&æÀD`òípbòzôl¼@–hä0_%Á8¡ªÐz“MDâ–©•Hÿsù&Ì+yñì|MÉL_Ÿ~â02æÎ‰¿ƒ „¥4=Øk†hì{;i`¶~SCQ;x}G`ûhÈy°Ä¬àÎ-NuêÖÐ¦0ÃqåÛã­^9"p²¨š$¨ÿ[úO9*éžB¦Ï°•<–40t2„¤o¼§òß˜qºd£h,8¢;X©çÛÐ´âaIÒQ#ïoëðÙÎÂøÌ!Ç=%®òEÑs€ÚD4R÷$;[÷ìö¿0DK|çÝ””°Ê«é…íLzLÆ) Ör¦X½îþ~o áìcÜY©Á|ç)
#({Ñ ’Eg_|oäX–ž®íÇóÛ2¢J#å-®ÛßXF}rÂ•»ÌfHT½
àbð,MŸ­„›ÖP’ÎºÏ$%Õ´eÑÓ¿}Yû¬Hda73–@µ©Ð9j	­>ë:Î5 <{`±™äà|ü—Üï²[Þÿ€þOÉÛÖKi¸J± û÷žÕwip’t“ÄØ4#7èÔ'(!Èjùy|¬QÞ}„,jÀ?´Â.ÏK¢ƒçÿlŸ²Á>HÛ7úq>Õs™—5‚ôÌÊÞ[¼‰X?ï;ÈøÑÅ;¸×h¹LÄ¥Q©ö-'ˆ´pzc¬¢-F€ý¼`¹QÒnô-¨4®ÐzJ Ï µ—PþYËõc÷ÖWhâ}9d¤ SÀL)¥›Æýee™¸6!ÄmŒg„ds\øN_Wz5^ÅrÙOŠö¹áÙ:EAÚùçÃ Þ‡ù€¨Ô}"m_|yÈÄ“uðÅkÛY6 ô˜B¶ÅÌãsþ[~;ÙæT—‚×É—Pš8H—|E+=+…&¢ñŒ ÂÊ9Æ[s¡5;®Cƒh|ÿè öË!Ó1 ?¸ýE¶yªèm¢7cn›Š #ôy4uzGËÝ€„¿Á¥ÁÈÐâ4?'»`M©~¶C»ÒÕ]_ áùzÉNÒnhá}
6>gì!¨L•woq§B /¦'ˆ·õÚéÉ$«p¿Ô¤Óé"{:tNØÑGæò;]ª êà0$î¶Þ™ðlºY÷È”*gÙ>9ÕœOú÷½KCû*šXsl¶2ŠR¯Æ´yIÎòh{«•ïìÚ¡þý¯QÂô0“ûó×Å^xŽbŒÄ×“þT.WV´Z©šG/}¬‚Pbb+Øqöem½_Ôò#/ÿF×¤;¥3>’¶ÛÓl‹¢Î9Fóo¯~Fûòê|hå›Ñ×ó|·`äèûÝÙ?“ì¨LÍ…kš×6ðë'JÄ÷tzøuô´}µ\Þoö(ƒjÊß3¼3;™ˆÇð#þnû&É næò~GÂa?y¿7iÑy´‹õ°ÁDT”³.ò\œoýj<`Þ'H	hDHM7»xÚOœþ€ÆùNvÑÿúœÒ°È¬µÿ@6kkwHÊÐ”qÛà^ðY6<öS«Ô¦†@‘	\[pœæGØÌ—ûù4ó#U’S*.:
»7zšË¡7ÂÇŸ#ÕFX³ ,¨l²uoú‡ÒÿÆ*Ú6dÆþé>’}oÐ{ÉÓì’R[jÝÒ•¡×¨óùçÏ&9›\Ö™0¨ª«$QE‡2Uýb´ìHXÒ\
#$AÜ1 5`«ld,=Fwº.!jíóÎ†ÆîÙ2§A‹4î4°üð(zªAe-ÁÈLüè-W«ûté+FŸ½‹ÑVÎš±îÒ¡[)Å”Búà/›frÕ°B{e0$–@µ©Ð9j	­>ë:Î5 g']gW.…ë¶¹4¢Å—ÿ©WÈ‹0’á"Ùêñp_’I¡âý¢ÄaFE’z„™ÜY«l[Gó[Ëm8Øw­a\L•AÊª!“q•®ÀÂ’ƒ >eÜ:¢3Qªñ…h+ñŸ¥®žÅâ:!½E¤œÒŸ>©Î{¥8©Ì+Ú‹oc­v[5¨¡-KâKÁ6¼Lœoøj{…UbÀLó«bí48™–š¶j¼&¶fûr6ªº3Qµ7ZþeSÚô²ÿ¸<*ÂîyUŽ0Î#¡-']y›*]u,(ZW*¾>•Ü¾¤˜¶kxUÛýõÒ_À˜é‹˜ÿNK_|S$÷õ 4¤¹´y|³²pÑÄ‚·g»;v'2Ž©~¦µÅé•-Œ	y²\NVBDí{ôÙÂX”ŒóDùLc—®W¶hrè‘þ8ì«-ð7„Á+ŠÒ±(X)›C9f÷óÂ¨9N?²²ûúóS—í®µÈj@}eª<v“[é×ÎÇIOž0Hôd¹öE^ÒJ
>"wù Æ@'å2"kcðZc€
¨½Çíüï{«á¦»Ï"y«,Fn=·•»¾\m`òD "¦wG¼¯›¬¾×r´Ó‡¾PË;©¸eÁô´A÷\ç 	àKH
g“8®áE\µr÷Vê$M#^NSYõÚ¶¹Í¦ùà 3¦ˆ…ï¶¨ØF{ž`€ÓÓ“þZ4WQ¡5ê»£bbzÀ£.5'ÙXÛ|@#›{ã"ï8²aÛ¥1¼?ƒî£¼:Äþ‰V´J–
%År‘ŠD4µçìúÔP»›h3íaþðÐñ/¢Á2µ:¹ýã«k`ÒÐl%TåÍH\ÏƒmÈ Ë‹K3B¼TÇKðzŽZXöþüÌ¸²e‰vÞ¾0Ž/UØ[_¢,UæÎ‰¹¼;]Æí}~Ré‚Ub{Ø=c79V+½²!ÿ—÷Ö•hR|ïÁÁ»8Å	õõ›Én%veDyÌÓŽzíÓjž,Z_ñS·ÏÀœHò^2jËà1úË”>>>>>>> .r472
