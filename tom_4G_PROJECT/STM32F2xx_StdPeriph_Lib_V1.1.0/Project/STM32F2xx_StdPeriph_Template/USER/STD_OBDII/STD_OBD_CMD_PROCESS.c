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
#include "STD_OBD_SendRecv.h"
#include "Fcar_Api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "STD_OBD_CMD_PROCESS.h"
#include "STD_TOYOTA_CMD_PROCESS.h"
#include "STD_NISSAN_CMD_PROCESS.h"
#include "STD_VW_CMD_PROCESS.h"
#include "STD_HONDA_CMD_PROCESS.h"
#include "STD_HYUNDAI_CMD_PROCESS.h"
#include "STD_FORD_CMD_PROCESS.h"
#include "STD_GMCN_CMD_PROCESS.h"
#include "FreeRTOS.h"
#include "task.h"
#include "user_config.h"
#include "App.h"


static uint32_t m_modulebuf[256];
extern uint8_t m_modulenum;//ECU单元个数
static uint8_t m_infotype[256] = {0};
static uint8_t m_diagdatapid[256] = {0};
intptr_t m_protocol_index = -1;   //记录侦测到的协议

extern uint8_t m_modulenum;
extern uint8_t m_setmodule;
extern uint8_t m_refuseretry;
extern uint32_t m_canbussendid;
extern uint32_t m_canbusflowid;
extern uint32_t m_canbusreadid;
extern uint32_t m_canbusflowidchang;
extern uint32_t m_canbusreadidchang;
bool b_add_6a = false;

uint8_t data_flag = 0;
uint8_t diag_flag = 0;
uint8_t ready_status_buf[20] = {0};
uint8_t iupr_buf[20] = {0};

int vehiclediagnosie_flag;

extern int OBD_STATUS;
extern int Vehicle_fire;
extern Vehicle_Model_Info tom_vehicle_model_data;

extern TaskHandle_t  Task_ObdiiCmdProcess_Proc_handle;

void pro_exit_dialog(OBDII_INIT_MODE mode);

//初始化IO命令
const OBDII_CMD_INIT ObdiiInitIoCmd[]=\
{

////////////////////////////////////////////////////////////////////////
	{
		{OBDII_MODE_01},
		{OBDII_MSG_Init_CAN_PackType},
		{0x2c,0x01,0x07,0xa1,0x20,0x02,0x82,0x03,0x00,0x04,0x00,0x00,0x07,0xdf,0x05,0x00,0x00,0x07,0xe8,0x06,0x00,0x00,0x00,0x07,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xe6}
	},
	{
		{OBDII_MODE_02},
		{OBDII_MSG_Init_CAN_PackType},
		{0x2c,0x01,0x07,0xa1,0x20,0x02,0x82,0x03,0x10,0x04,0x98,0xdb,0x33,0xf1,0x05,0x98,0xda,0xf1,0x10,0x06,0x1c,0x00,0x00,0xff,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xe6}
	},
	{
		{OBDII_MODE_03},
		{OBDII_MSG_Init_CAN_PackType},
		{0x2c,0x01,0x03,0xd0,0x90,0x02,0x82,0x03,0x00,0x04,0x00,0x00,0x07,0xdf,0x05,0x00,0x00,0x07,0xe8,0x06,0x00,0x00,0x00,0x07,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xe6}
	},
	{
		{OBDII_MODE_04},
		{OBDII_MSG_Init_CAN_PackType},
		{0x2c,0x01,0x03,0xd0,0x90,0x02,0x82,0x03,0x10,0x04,0x98,0xdb,0x33,0xf1,0x05,0x98,0xda,0xf1,0x10,0x06,0x1c,0x00,0x00,0xff,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xe6}
	},
	{
		{OBDII_MODE_05},
		{OBDII_MSG_Init_KWP_PackType},
		{0x1f,0x01,0x00,0x00,0x00,0x02,0x00,0x03,0x80,0x04,0x77,0x00,0xc1,0xc0,0x05,0x0b,0x19,0x40,0x04,0x04,0x00,0x28,0x06,0x01,0x08,0x32,0x09,0x03,0x0c,0x0a,0x00,0x0b}
	},
	{
		{OBDII_MODE_06},
		{OBDII_MSG_Init_KWP_PackType},
		{0x1f,0x01,0x00,0x00,0x00,0x02,0x00,0x03,0x80,0x04,0x77,0x00,0xc1,0xc0,0x05,0x0b,0x19,0x40,0x04,0x04,0x00,0x28,0x06,0x01,0x08,0x32,0x09,0x03,0x0c,0x0a,0x00,0x0b}
	},
	{
		{OBDII_MODE_07},
		{OBDII_MSG_Init_KWP_PackType},
		{0x2b,0x01,0x00,0x28,0xa0,0x02,0x00,0x03,0xc0,0x04,0x77,0x00,0xc1,0xc1,0x05,0x0b,0x19,0x40,0x04,0x04,0x00,0x28,0x06,0x03,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x25,0x09,0x03,0x0a,0x00,0x00,0x00,0x0b,0x00,0x0c,0x01,0x00,0x00}
	},
	{
		{OBDII_MODE_08},
		{OBDII_MSG_Init_PWM_PackType},
		{0x13,0x02,0x00,0x03,0x01,0x04,0x01,0xf1,0x06,0x41,0x07,0x00,0x08,0x00,0x09,0x6b,0x0a,0x00,0x0b,0xff}
	},
	{
		{OBDII_MODE_09},
		{OBDII_MSG_Init_VPW_PackType},
		{0x18,0x02,0x00,0x03,0x01,0x04,0x01,0x01,0x05,0x00,0x00,0x06,0x48,0x07,0x00,0x08,0x6b,0x09,0x00,0x0a,0x10,0x0b,0xff,0x0e,0x35}
	},
};

// ECU INIT
const OBDII_CMD_INIT ObdiiInitEcuCmd[]=\
{
	{
		{OBDII_MODE_01},
		{0},
		{0}
	},
	{
		{OBDII_MODE_02},
		{0},
		{0}
	},
	{
		{OBDII_MODE_03},
		{0},
		{0}
	},
	{
		{OBDII_MODE_04},
		{0},
		{0}
	},
	{
		{OBDII_MODE_05},
		{OBDII_MSG_Init_Ecu_KWP_PackType},
		{01,0x33}
	},
	{
		{OBDII_MODE_06},
		{OBDII_MSG_Init_Ecu_KWP_PackType},
		{01,0x33}
	},
	{
		{OBDII_MODE_07},
		{OBDII_MSG_Init_Ecu_KWP_PackType},
		{01,0x81}
	},
	{
		{OBDII_MODE_08},
		{0x00},
		{0x00}
	},
	{
		{OBDII_MODE_09},
		{0x00},
		{0x00}
	},
};

//ECU select
const OBDII_CMD_INIT ObdiiEcuSelectCmd[]=\
{
	{
		{OBDII_MODE_01},
		{OBDII_MSG_ALL_CAN_PackType},
		{0x02,0x01,0x00}
	},
	{
		{OBDII_MODE_02},
		{OBDII_MSG_ALL_CAN_PackType},
		{0x02,0x01,0x00}
	},
	{
		{OBDII_MODE_03},
		{OBDII_MSG_ALL_CAN_PackType},
		{0x02,0x01,0x00}
	},
	{
		{OBDII_MODE_04},
		{OBDII_MSG_ALL_CAN_PackType},
		{0x02,0x01,0x00}
	},
	{
		{OBDII_MODE_05},
		{OBDII_MSG_ALL_KWP_PackType},
		{0x02,0x01,0x00}
	},
	{
		{OBDII_MODE_06},
		{OBDII_MSG_ALL_KWP_PackType},
		{0x02,0x01,0x00}
	},
	{
		{OBDII_MODE_07},
		{OBDII_MSG_ALL_KWP_PackType},
		{0x02,0x01,0x00}
	},
	{
		{OBDII_MODE_08},
		{OBDII_MSG_ALL_PWM_PackType},
		{0x02,0x01,0x00}
	},
	{
		{OBDII_MODE_09},
		{OBDII_MSG_ALL_VPW_PackType},
		{0x02,0x01,0x00}
	},
};


//版本信息命令
const OBDII_STREAM_CMD  ReadInfoCmd[]=\
{
	{
	   {OBDII_READ_VIN},
	   {0x02,0x09,0x02}
	},
	{
	   {OBDII_READ_CALID},
	   {0x02,0x09,0x04}
	},
	{
	   {OBDII_READ_CVN},
	   {0x02,0x09,0x06}
//			{0x02,0x09,0x04}
	},
};

//读IUPR命令
const OBDII_STREAM_CMD  ReadIuprCmd[]=\
{
	{
	   {OBDII_READ_IUPR1},
	   {0x02,0x09,0x08}
	},
	{
	   {OBDII_READ_IUPR2},
	   {0x02,0x09,0x0b}
	},
};


//数据流侦测命令
const OBDII_CMD_STREAMSCAN StreamScan[]={\
{0x02,0x01,0x00},\
{0x02,0x01,0x20},\
{0x02,0x01,0x40},\
{0x02,0x01,0x60},\
{0x02,0x01,0x80},\
{0x02,0x01,0xa0},\
{0x02,0x01,0xc0},\
{0x02,0x01,0xe0},\
};


//数据流命令
const OBDII_STREAM_CMD  ReadDataCmd[]=\
{
	{
	   {OBDII_READ_READY},
	   {0x02,0x01,0x01}
	},
	{
	   {OBDII_READ_KILOMETRE},
	   {0x02,0x01,0xa6}
	},
	{
	   {OBDII_READ_SPEED},
	   {0x02,0x01,0x0d}
	},
	{
	   {OBDII_READ_MAX_STORQUE},
	   {0x02,0x01,0x63}
		 
	},
	{
	   {OBDII_READ_OUT_STORQUE},
	   {0x02,0x01,0x62}
	},
	{
	   {OBDII_READ_FRICTION_STORQUE},
	   {0x02,0x01,0x8e}
	},
	{
	   {OBDII_READ_ENGINE_SPEED},
	   {0x02,0x01,0x0c}
	},
	{
	   {OBDII_READ_ENGINE_FLOW},
	   {0x02,0x01,0x5e}
	},
	{
	   {OBDII_READ_IN_TEMPERATURE},
	   {0x02,0x01,0x7c}
	},
	{
	   {OBDII_READ_OUT_TEMPERATURE},
	   {0x02,0x01,0x7c}
	},
	{
	   {OBDII_READ_PRESSURE_DIFF1},
	   {0x02,0x01,0x7a}
	},
	{
	   {OBDII_READ_PRESSURE_DIFF2},
	   {0x02,0x01,0x7b}
	},
	{
	   {OBDII_READ_INTAKE_VOLUME},
	   {0x02,0x01,0x10}
	},
	{
	   {OBDII_READ_REACTANT_SUIPLUS},
	   {0x02,0x01,0x85}
	},
	{
	   {OBDII_READ_TANK_LEVEL},
	   {0x02,0x01,0x2f}
	},
	{
	   {OBDII_READ_COOLANT_TEMPERATURE},
	   {0x02,0x01,0x05}
	},
//	{
//	   {OBDII_READ_COOLANT_TEMPERATURE2},
//	   {0x02,0x01,0x67}
//	},
//	{
//	   {OBDII_READ_FUEL_CONSUMPTION},
//	   {}
//	},
	{
	   {OBDII_READ_FUEL_INJECTION},
	   {0x02,0x01,0x5e}
	},
	{
	   {OBDII_READ_VOLTAGE},
	   {0x02,0x01,0x42}
	},
	{
	   {OBDII_READ_ENGINE_START},
	   {0x02,0x01,0x1f}
	},
//	{
//	   {OBDII_READ_ENGINE_STOP},
//	   {}
//	},
	{
	   {OBDII_READ_FRONTOCURRENT1},
	   {0x02,0x01,0x34}
	},
	{
	   {OBDII_READ_FRONTOCURRENT2},
	   {0x02,0x01,0x38}
	},
//	{
//	   {OBDII_READ_FRONTOCURRENT3},
//	   {0x02,0x01,0x34}
//	},
//	{
//	   {OBDII_READ_FRONTOCURRENT4},
//	   {0x02,0x01,0x36}
//	},
//	{
//	   {OBDII_READ_FRONTOCURRENT5},
//	   {0x02,0x01,0x38}
//	},
//	{
//	   {OBDII_READ_FRONTOCURRENT6},
//	   {0x02,0x01,0x3a}
//	},
	{
	   {OBDII_READ_FRONTOVOLTAGE1},
	   {0x02,0x01,0x24}
	},
	{
	   {OBDII_READ_FRONTOVOLTAGE2},
	   {0x02,0x01,0x28}
	},
//	{
//	   {OBDII_READ_FRONTOVOLTAGE3},
//	   {0x02,0x01,0x24}
//	},
//	{
//	   {OBDII_READ_FRONTOVOLTAGE4},
//	   {0x02,0x01,0x26}
//	},
//	{
//	   {OBDII_READ_FRONTOVOLTAGE5},
//	   {0x02,0x01,0x28}
//	},
//	{
//	   {OBDII_READ_FRONTOVOLTAGE6},
//	   {0x02,0x01,0x2a}
//	},
	{
	   {OBDII_READ_AFTEROCURRENT1},
	   {0x02,0x01,0x35}
	},
	{
	   {OBDII_READ_AFTEROCURRENT2},
	   {0x02,0x01,0x39}
	},
//	{
//	   {OBDII_READ_AFTEROCURRENT3},
//	   {0x02,0x01,0x35}
//	},
//	{
//	   {OBDII_READ_AFTEROCURRENT4},
//	   {0x02,0x01,0x37}
//	},
//	{
//	   {OBDII_READ_AFTEROCURRENT5},
//	   {0x02,0x01,0x39}
//	},
//	{
//	   {OBDII_READ_AFTEROCURRENT6},
//	   {0x02,0x01,0x3b}
//	},
	{
	   {OBDII_READ_AFTEROVOLTAGE1},
	   {0x02,0x01,0x25}
	},
	{
	   {OBDII_READ_AFTEROVOLTAGE2},
	   {0x02,0x01,0x29}
	},
//	{
//	   {OBDII_READ_AFTEROVOLTAGE3},
//	   {0x02,0x01,0x25}
//	},
//	{
//	   {OBDII_READ_AFTEROVOLTAGE4},
//   {0x02,0x01,0x27}
//	},
//	{
//	   {OBDII_READ_AFTEROVOLTAGE5},
//	   {0x02,0x01,0x29}
//	},
//	{
//	   {OBDII_READ_AFTEROVOLTAGE6},
//	   {0x02,0x01,0x2b}
//	},
	{
	   {OBDII_READ_ENGINELOAD},
	   {0x02,0x01,0x04}
	},
	{
	   {OBDII_READ_AIRINPRESSURE},
	   {0x02,0x01,0x0b}
	},
	{
	   {OBDII_READ_AIRINTEMPERATURE},
	   {0x02,0x01,0x0f}
	},
	
};


//ECU exit
const OBDII_CMD_INIT ObdiiEcuExitCmd[] = \
{
	{
		{OBDII_MODE_01},
		{0},
		{0}
	},
	{
		{OBDII_MODE_02},
		{0},
		{0}
	},
	{
		{OBDII_MODE_03},
		{0},
		{0}
	},
	{
		{OBDII_MODE_04},
		{0},
		{0}
	},
	{
		{OBDII_MODE_05},
		{0},
		{0}
	},
	{
		{OBDII_MODE_06},
		{0},
		{0}
	},
	{
		{OBDII_MODE_07},
		{OBDII_MSG_ALL_KWP_PackType},
		{01,0x82}
	},
	{
		{OBDII_MODE_08},
		{0x00},
		{0x00}
	},
	{
		{OBDII_MODE_09},
		{0x00},
		{0x00}
	},
};


typedef struct
{
	OBDII_INIT_MODE Init_Mode;
	OBDII_MSG_ORD Msg_Ord;
  uint8_t CMD_DATA[64];
  
} CYOBDII_CMD_INIT;

typedef struct
{
	OBDII_MSG_ORD Msg_Ord;
  uint8_t CMD_DATA[64];
  
} OBDII_MSG_CMD;

extern DEFAULT_MSGATTR g_defaultMsgAttr; 



Vehicle_diagnosis g_vehiclediagnosie={0};
Data_collection g_datacollection={0};


uint8_t m_CAN_ANS_T;
uint8_t m_CAN_ANS_R;
uint8_t m_CAN_LISTEN_TYPE;
extern uint32_t m_canbussendid;/*系统默认发送ID*/
extern uint32_t m_canbusflowid;/*当前系统发送ID*/
extern uint32_t m_canbusreadid;/*当前系统接收ID*/
extern uint32_t m_canbusflowidchang;/*需要改变的发送ID*/
extern uint32_t m_canbusreadidchang;/*需要改变的接收ID*/

const OBDII_MSG_CMD ObdiiInitECUCmd[]=\
{
	{//Null
    {0},
		{0x00}
},

	{//mode 1
    {OBDII_MSG_ALL_CAN_PackType},
		{0x03,0x22,0xf1,0x94}
},
	{//Mode 2
    {OBDII_MSG_ALL_CAN_PackType},
		{0x02,0x1a,0x94}
},
	{//mode 3
    {OBDII_MSG_ALL_KWP_PackType},
		{0x02,0x1a,0x94}
},
{//mode 4
    {OBDII_MSG_ALL_KWP_PackType},
		{0x02,0x1a,0x94}
},
};

const CYOBDII_CMD_INIT CYObdiiInitIoCmd[]=\
{
//////////////////////////Mode 1///////////////////////////////////////
	{
		{OBDII_MODE_01},
    {OBDII_MSG_Init_CAN_PackType},
		{0x2C,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x10,0x04,0x98,0xDA,0x00,0xF1,0x05,0x98,0xda,0xf1,0x00,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x83,0xe0,0x0b,0x00,0x00,0x0c,0xE6}
},

	{
		{OBDII_MODE_01},
    {OBDII_MSG_Init_CAN_PackType},
		{0x2C,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x10,0x04,0x98,0xDA,0x00,0xF1,0x05,0x98,0xda,0xf1,0x00,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x83,0xe0,0x0b,0x00,0x00,0x0c,0x91}
},

	{
		{OBDII_MODE_01},
    {OBDII_MSG_Init_CAN_PackType},
		{0x2C,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x10,0x04,0x98,0xDA,0x00,0xF1,0x05,0x98,0xda,0xf1,0x00,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x83,0xe0,0x0b,0x00,0x00,0x0c,0xb3}
},

	{
		{OBDII_MODE_01},
    {OBDII_MSG_Init_CAN_PackType},
		{0x2C,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x10,0x04,0x98,0xDA,0x00,0xFA,0x05,0x98,0xda,0xfA,0x00,0x06,0x00,0x00,0x01,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x83,0xe0,0x0b,0x00,0x00,0x0c,0xE6}
},

	{
		{OBDII_MODE_01},
    {OBDII_MSG_Init_CAN_PackType},
		{0x2C,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x10,0x04,0x98,0xDA,0x00,0xFA,0x05,0x98,0xda,0xfA,0x00,0x06,0x00,0x00,0x01,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x83,0xe0,0x0b,0x00,0x00,0x0c,0x91}
},

	{
		{OBDII_MODE_01},
    {OBDII_MSG_Init_CAN_PackType},
		{0x2C,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x10,0x04,0x98,0xDA,0x00,0xFA,0x05,0x98,0xda,0xfA,0x00,0x06,0x00,0x00,0x01,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x83,0xe0,0x0b,0x00,0x00,0x0c,0xb3}
},
	{
		{OBDII_MODE_01},
    {OBDII_MSG_Init_CAN_PackType},
		{0x2C,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x00,0x04,0x00,0x00,0x07,0xE0,0x05,0x00,0x00,0x07,0xe8,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x87,0xd0,0x0b,0x00,0x00,0x0c,0xe6}
},

	{
		{OBDII_MODE_01},
    {OBDII_MSG_Init_CAN_PackType},
		{0x2C,0x01,0x03,0xD0,0x90,0x02,0x82,0x03,0x10,0x04,0x98,0xDA,0x00,0xFA,0x05,0x98,0xda,0xfA,0x00,0x06,0x00,0x00,0x01,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x83,0xe0,0x0b,0x00,0x00,0x0c,0xE6}
},

	{
		{OBDII_MODE_01},
    {OBDII_MSG_Init_CAN_PackType},
		{0x2C,0x01,0x03,0xD0,0x90,0x02,0x82,0x03,0x00,0x04,0x00,0x00,0x07,0xE0,0x05,0x00,0x00,0x07,0xe8,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x87,0xd0,0x0b,0x00,0x00,0x0c,0xe6}
},

	{
		{OBDII_MODE_01},
    {OBDII_MSG_Init_CAN_PackType},
		{0x2C,0x01,0x80,0x00,0x14,0x02,0x82,0x03,0x00,0x04,0x00,0x00,0x06,0xF1,0x05,0x00,0x00,0x06,0x10,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0x91}
},

	{
		{OBDII_MODE_01},
    {OBDII_MSG_Init_CAN_PackType},
		{0x2C,0x01,0x80,0x00,0x14,0x02,0x82,0x03,0x00,0x04,0x00,0x00,0x06,0xF1,0x05,0x00,0x00,0x06,0x10,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xe6}
},
	/***mode02***/
	{
		{OBDII_MODE_02},
    {OBDII_MSG_Init_CAN_PackType},
		{0x2C,0x01,0x03,0xD0,0x90,0x02,0x82,0x03,0x10,0x04,0x98,0xDA,0x33,0xF1,0x05,0x98,0xDA,0xF1,0x10,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0A,0x00,0x00,0x0B,0x00,0x00,0x0C,0xB3}
},

{
		{OBDII_MODE_02},
    {OBDII_MSG_Init_CAN_PackType},
		{0x2C,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x10,0x04,0x98,0xDA,0x00,0xFB,0x05,0x98,0xDA,0xFB,0x00,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0A,0x85,0x00,0x0B,0x00,0x00,0x0C,0xE6}
},

{
		{OBDII_MODE_02},
    {OBDII_MSG_Init_CAN_PackType},
		{0x2C,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x10,0x04,0x98,0xDA,0x00,0xFB,0x05,0x98,0xDA,0xFB,0x00,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0A,0x85,0x00,0x0B,0x00,0x00,0x0C,0xB3}
},
	/***mode03***/
	{
		{OBDII_MODE_03},
    {OBDII_MSG_Init_KWP_PackType},
		{0x2B,0x01,0x00,0x28,0xA0,0x02,0x00,0x03,0xC0,0x04,0x77,0x00,0xC1,0xC1,0x05,0x12,0x20,0x38,0x04,0x07,0xd0,0x19,0x06,0x03,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x50,0x09,0x03,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0C,0x01,0x00,0x00}
},
/***mode04***/
{
		{OBDII_MODE_04},
    {OBDII_MSG_Init_KWP_PackType},
		{0x2B,0x01,0x00,0x28,0xA0,0x02,0x00,0x03,0xC0,0x04,0x77,0x00,0xC1,0xC1,0x05,0x12,0x20,0x38,0x04,0x07,0xd0,0x19,0x06,0x03,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x50,0x09,0x03,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0C,0x01,0x00,0x00}
},
};







bool SetFrameTimeOut(uint8_t index)
{
	uint8_t  ord;
	uint8_t initdata[64];
	uint8_t number,i=0;
	uint32_t canid;
	m_CAN_ANS_T = 0;
	m_CAN_ANS_R = 0;
	ord = CYObdiiInitIoCmd[index].Msg_Ord;
	number=CYObdiiInitIoCmd[index].CMD_DATA[0];
	memcpy(initdata,CYObdiiInitIoCmd[index].CMD_DATA+1,number);
	switch(ord)
	{
	case 0x13:
		while(i<number)
		{
			switch(initdata[i])
			{
			case 0x01: i+=4;	 break;
			case 0x02: i+=2;	break;
			case 0x03: i+=2;	 break;
			case 0x04: i+=5;	 break;
			case 0x05:
				g_defaultMsgAttr.m_AnswerByteTimeOut = initdata[i+1];
				g_defaultMsgAttr.m_AnswerFrameTimeOut = initdata[i+2];
				g_defaultMsgAttr.m_RequestFrameTimeOut = initdata[i+3];
				g_defaultMsgAttr.m_RequestByteTimeOut = initdata[i+4];
				g_defaultMsgAttr.m_AutoKeepTimeOut= initdata[i+5];
				g_defaultMsgAttr.m_AutoKeepTimeOut = initdata[i+6]*256+initdata[i+7];
				i+=8;

				break;
			case 0x06:
				g_defaultMsgAttr.m_KeepFlag = initdata[i+1];
				i+=2;
				break;
			case 0x07: i+=initdata[i+1]*2+2; break;
			case 0x08: i+=2;	 break;
			case 0x09:
				g_defaultMsgAttr.m_ISOAnswerNum = initdata[i+1];
				i+=2;
				break;
			case 0x0a: i+=4;	 break;
			case 0x0b: i+=2;	 break;
			case 0x0c: i+=4;	 break;
			case 0x0d: i+=2;	 break;
			case 0x0e: i+=3;	 break;
			case 0x0f: i+=2;	 break;
			default:
				//??
				return FALSE;

			}
		}
		break;
	case 0x10:
		g_defaultMsgAttr.m_AnswerByteTimeOut = 02;
		g_defaultMsgAttr.m_AnswerFrameTimeOut = 0xd;
		g_defaultMsgAttr.m_RequestFrameTimeOut = 15;
		g_defaultMsgAttr.m_RequestByteTimeOut = 02;
		g_defaultMsgAttr.m_AutoKeepTimeOut= 15;
		g_defaultMsgAttr.m_KeepFlag =00;
		while(i<number)
		{
			switch(initdata[i])
			{
			case 0x01: i+=4;	 break;
			case 0x02: 
				m_CAN_LISTEN_TYPE =initdata[i+1];
				i+=2;	
				break;
			case 0x03: i+=2;	 break;
			case 0x04: 
				canid=0;
				canid += initdata[i+1]<<24;
				canid += initdata[i+2]<<16;
				canid += initdata[i+3]<<8;
				canid += initdata[i+4];
				setcanbusflowid(canid);
				i+=5;
			break;
			case 0x05:
				canid=0;
				canid += initdata[i+1]<<24;
				canid += initdata[i+2]<<16;
				canid += initdata[i+3]<<8;
				canid += initdata[i+4];
				setcanbusreadid(canid);
				i+=5;	 break;
			case 0x06: i+=5;	 break;
			case 0x07: i+=5;	 break;
			case 0x08: i+=5;	 break;
			case 0x09: i+=3;	 break;
			case 0x0a: i+=3;	 break;
			case 0x0b: i+=3;	 break;
			case 0x0c: i+=2;	 break;
			case 0x0d:
				m_CAN_ANS_T = initdata[i+1];
				i+=2;
				break;
			case 0x0e:
				m_CAN_ANS_R = initdata[i+1];
				i+=0x11;
				break;
			case 0x0F:
				i+=2;
				break;
			default:
				//??
				return FALSE;
				
			}
		}
		break;
	default:
		return FALSE;
	}

	return TRUE;
}

bool OBDII_SetFrameTimeOut(OBDII_INIT_MODE index)
{
	uint8_t  ord;
	uint8_t initdata[64];
	uint8_t number,i=0;
	uint32_t canid;
	m_CAN_ANS_T = 0;
	m_CAN_ANS_R = 0;
	
	ord = ObdiiInitIoCmd[index].Msg_Ord;
	number =ObdiiInitIoCmd[index].CMD_DATA[0];
	memcpy(initdata,ObdiiInitIoCmd[index].CMD_DATA+1,number);
	switch(ord)
	{
	case 0x13:
		while(i<number)
		{
			switch(initdata[i])
			{
			case 0x01: i+=4;	 break;
			case 0x02: i+=2;	break;
			case 0x03: i+=2;	 break;
			case 0x04: i+=5;	 break;
			case 0x05:
				g_defaultMsgAttr.m_AnswerByteTimeOut = initdata[i+1];
				g_defaultMsgAttr.m_AnswerFrameTimeOut = initdata[i+2];
				g_defaultMsgAttr.m_RequestFrameTimeOut = initdata[i+3];
				g_defaultMsgAttr.m_RequestByteTimeOut = initdata[i+4];
				g_defaultMsgAttr.m_AutoKeepTimeOut= initdata[i+5];
				g_defaultMsgAttr.m_AutoKeepTimeOut = initdata[i+6]*256+initdata[i+7];
				i+=8;

				break;
			case 0x06:
				g_defaultMsgAttr.m_KeepFlag = initdata[i+1];
				i+=2;
				break;
			case 0x07: i+=initdata[i+1]*2+2; break;
			case 0x08: i+=2;	 break;
			case 0x09:
				g_defaultMsgAttr.m_ISOAnswerNum = initdata[i+1];
				i+=2;
				break;
			case 0x0a: i+=4;	 break;
			case 0x0b: i+=2;	 break;
			case 0x0c: i+=4;	 break;
			case 0x0d: i+=2;	 break;
			case 0x0e: i+=3;	 break;
			case 0x0f: i+=2;	 break;
			default:
				//??
				return FALSE;

			}
		}
		break;
	case 0x14:
		while(i<number)
		{
			switch(initdata[i])
			{
			case 0x01: i+=4;	 break;
			case 0x02: i+=2;	break;
			case 0x03: i+=2;	 break;
			case 0x04: i+=5;	 break;
			case 0x05:
				g_defaultMsgAttr.m_AnswerByteTimeOut = initdata[i+1];
				g_defaultMsgAttr.m_AnswerFrameTimeOut = initdata[i+2];
				g_defaultMsgAttr.m_RequestFrameTimeOut = initdata[i+3];
				g_defaultMsgAttr.m_RequestByteTimeOut = initdata[i+4];
				g_defaultMsgAttr.m_AutoKeepTimeOut= initdata[i+5];
				g_defaultMsgAttr.m_AutoKeepTimeOut = initdata[i+6]*256+initdata[i+7];
				i+=8;

				break;
			case 0x06:
				g_defaultMsgAttr.m_KeepFlag = initdata[i+1];
				i+=2;
				break;
			case 0x07: i+=initdata[i+1]*2+2; break;
			case 0x08: i+=2;	 break;
			case 0x09:
				g_defaultMsgAttr.m_ISOAnswerNum = initdata[i+1];
				i+=2;
				break;
			case 0x0a: i+=4;	 break;
			case 0x0b: i+=2;	 break;
			case 0x0c: i+=5;	 break;
			default:
				//??
				return FALSE;
			}
		}
		break;	
	
	case 0x10:
		g_defaultMsgAttr.m_AnswerByteTimeOut = 02;
		g_defaultMsgAttr.m_AnswerFrameTimeOut = 0xd;
		g_defaultMsgAttr.m_RequestFrameTimeOut = 15;
		g_defaultMsgAttr.m_RequestByteTimeOut = 02;
		g_defaultMsgAttr.m_AutoKeepTimeOut= 15;
		g_defaultMsgAttr.m_KeepFlag =00;
		while(i<number)
		{
			switch(initdata[i])
			{
			case 0x01: i+=4;	 break;
			case 0x02: 
				m_CAN_LISTEN_TYPE =initdata[i+1];
				i+=2;	
				break;
			case 0x03: i+=2;	 break;
			case 0x04: 
				canid=0;
				canid += initdata[i+1]<<24;
				canid += initdata[i+2]<<16;
				canid += initdata[i+3]<<8;
				canid += initdata[i+4];
				setcanbusflowid(canid);
				i+=5;
			break;
			case 0x05:
				canid=0;
				canid += initdata[i+1]<<24;
				canid += initdata[i+2]<<16;
				canid += initdata[i+3]<<8;
				canid += initdata[i+4];
				setcanbusreadid(canid);
				i+=5;	 break;
			case 0x06: i+=5;	 break;
			case 0x07: i+=5;	 break;
			case 0x08: i+=5;	 break;
			case 0x09: i+=3;	 break;
			case 0x0a: i+=3;	 break;
			case 0x0b: i+=3;	 break;
			case 0x0c: i+=2;	 break;
			case 0x0d:
				m_CAN_ANS_T = initdata[i+1];
				i+=2;
				break;
			case 0x0e:
				m_CAN_ANS_R = initdata[i+1];
				i+=0x11;
				break;
			case 0x0F:
				i+=2;
				break;
			default:
				//??
				return FALSE;
			}
		}
		break;
	case 0x11: 
		g_defaultMsgAttr.m_AnswerByteTimeOut = 02;
		g_defaultMsgAttr.m_AnswerFrameTimeOut = 0xd;
		g_defaultMsgAttr.m_RequestFrameTimeOut = 15;
		g_defaultMsgAttr.m_RequestByteTimeOut = 02;
		g_defaultMsgAttr.m_AutoKeepTimeOut= 15;
		g_defaultMsgAttr.m_KeepFlag =00;

		break;
	case 0x12:

		g_defaultMsgAttr.m_AnswerByteTimeOut = 02;
		g_defaultMsgAttr.m_AnswerFrameTimeOut = 50;
		g_defaultMsgAttr.m_RequestFrameTimeOut = 15;
		g_defaultMsgAttr.m_RequestByteTimeOut = 02;
		g_defaultMsgAttr.m_AutoKeepTimeOut= 15;
		g_defaultMsgAttr.m_KeepFlag =00;

		break;
	case 0x15:
	case 0x16:
		g_defaultMsgAttr.m_AnswerByteTimeOut = 02;
		g_defaultMsgAttr.m_AnswerFrameTimeOut = 50;
		g_defaultMsgAttr.m_RequestFrameTimeOut = 15;
		g_defaultMsgAttr.m_RequestByteTimeOut = 02;
		g_defaultMsgAttr.m_AutoKeepTimeOut= 15;
		g_defaultMsgAttr.m_KeepFlag =00;
		break;
	case 0x17:
	case 0x18:
	case 0x19:
	case 0x1a:
	case 0x1b:
	case 0x1c:
	case 0x1d:
	case 0x1e:
	case 0x1f:
		break;
	
		
	default:
		return FALSE;
	}

	return TRUE;
}



void OBDII_MSG_ALL(OBDII_INIT_MODE mode, uint8_t index)
{
	switch(mode)
	{
		case OBDII_MODE_01:
		case OBDII_MODE_02:
			g_defaultMsgAttr.m_CS_MODE = 0;
			g_defaultMsgAttr.m_ORD     = 0x30;
			g_defaultMsgAttr.m_PCK_TYPE= 0x22;
			g_defaultMsgAttr.m_RWAYS   = 0;
			g_defaultMsgAttr.m_SRC     = 0;
			g_defaultMsgAttr.m_TGT     = 0;
			g_defaultMsgAttr.m_WAYS    = 0;
		break;
		case OBDII_MODE_03:
			g_defaultMsgAttr.m_CS_MODE = 0;
			g_defaultMsgAttr.m_ORD     = 0x33;
			g_defaultMsgAttr.m_PCK_TYPE= 0x02;
			g_defaultMsgAttr.m_RWAYS   = 0;
			g_defaultMsgAttr.m_SRC     = 0xF1;
			g_defaultMsgAttr.m_TGT     = 0x10;
			g_defaultMsgAttr.m_WAYS    = 0;
		break;
		case OBDII_MODE_04:
			g_defaultMsgAttr.m_CS_MODE = 0;
			g_defaultMsgAttr.m_ORD     = 0x33;
			g_defaultMsgAttr.m_PCK_TYPE= 0x02;
			g_defaultMsgAttr.m_RWAYS   = 0;
			g_defaultMsgAttr.m_SRC     = 0xF1;
			g_defaultMsgAttr.m_TGT     = 0x11;
			g_defaultMsgAttr.m_WAYS    = 0;
		break;
		default:
		break;
	}
}

//设置ECU初始化ORD
void pro_set_init_ecu_ord(OBDII_INIT_MODE mode)
{
	switch(mode)
	{
		case OBDII_MODE_05:
		case OBDII_MODE_06:
		case OBDII_MODE_07:
				g_defaultMsgAttr.m_ORD = OBDII_MSG_Init_Ecu_KWP_PackType;
		  break;
		default:
			break;
	}
}

//设置通讯过程ORD
void pro_msg_all(OBDII_INIT_MODE mode)
{
	switch(mode)
	{
		case OBDII_MODE_01:
		case OBDII_MODE_02:
		case OBDII_MODE_03:
		case OBDII_MODE_04:
				g_defaultMsgAttr.m_CS_MODE = 0;
				g_defaultMsgAttr.m_ORD     = 0x30;
				g_defaultMsgAttr.m_PCK_TYPE= 0x22;
				g_defaultMsgAttr.m_RWAYS   = 0;
				g_defaultMsgAttr.m_SRC     = 0;
				g_defaultMsgAttr.m_TGT     = 0;
				g_defaultMsgAttr.m_WAYS    = 0;
			break;

		case OBDII_MODE_05:
				g_defaultMsgAttr.m_CS_MODE = 0;
				g_defaultMsgAttr.m_ORD     = 0x33;
				g_defaultMsgAttr.m_PCK_TYPE= 0x0d;
				g_defaultMsgAttr.m_RWAYS   = 0x01;
				g_defaultMsgAttr.m_SRC     = 0xF1;
				g_defaultMsgAttr.m_TGT     = 0x33;
				g_defaultMsgAttr.m_WAYS    = 0;
				if(b_add_6a)
				{
					g_defaultMsgAttr.m_PCK_TYPE= 0x05;
					g_defaultMsgAttr.m_SRC     = 0xf1;
					g_defaultMsgAttr.m_TGT     = 0x6a;
				}
				else
				{
					g_defaultMsgAttr.m_PCK_TYPE= 0x0d;
					g_defaultMsgAttr.m_SRC     = 0xf1;
					g_defaultMsgAttr.m_TGT     = 0x33;
				}
		  break;
		case OBDII_MODE_06:
				g_defaultMsgAttr.m_CS_MODE = 0;
				g_defaultMsgAttr.m_ORD     = 0x33;
				g_defaultMsgAttr.m_PCK_TYPE= 0x05;
				g_defaultMsgAttr.m_RWAYS   = 0x01;
				g_defaultMsgAttr.m_SRC     = 0xF1;
				g_defaultMsgAttr.m_TGT     = 0x6A;
				g_defaultMsgAttr.m_WAYS    = 0;
				if(b_add_6a)
				{
					g_defaultMsgAttr.m_PCK_TYPE= 0x05;
					g_defaultMsgAttr.m_SRC     = 0xf1;
					g_defaultMsgAttr.m_TGT     = 0x6a;
				}
				else
				{
					g_defaultMsgAttr.m_PCK_TYPE= 0x0d;
					g_defaultMsgAttr.m_SRC     = 0xf1;
					g_defaultMsgAttr.m_TGT     = 0x33;
				}
		  break;
		case OBDII_MODE_07:
				g_defaultMsgAttr.m_CS_MODE = 0;
				g_defaultMsgAttr.m_ORD     = 0x33;
				g_defaultMsgAttr.m_PCK_TYPE= 0x0D;
				g_defaultMsgAttr.m_RWAYS   = 0x01;
				g_defaultMsgAttr.m_SRC     = 0xF1;
				g_defaultMsgAttr.m_TGT     = 0x33;
				g_defaultMsgAttr.m_WAYS    = 0;
		  break;
		case OBDII_MODE_08:
				g_defaultMsgAttr.m_CS_MODE = 0;
				g_defaultMsgAttr.m_ORD     = 0x32;
				g_defaultMsgAttr.m_PCK_TYPE= 0x09;
				g_defaultMsgAttr.m_RWAYS   = 0x01;
				g_defaultMsgAttr.m_SRC     = 0xF1;
				g_defaultMsgAttr.m_TGT     = 0x6A;
				g_defaultMsgAttr.m_WAYS    = 0;
		  break;
		case OBDII_MODE_09:
				g_defaultMsgAttr.m_CS_MODE = 0;
				g_defaultMsgAttr.m_ORD     = 0x31;
				g_defaultMsgAttr.m_PCK_TYPE= 0x0b;
				g_defaultMsgAttr.m_RWAYS   = 0x01;
				g_defaultMsgAttr.m_SRC     = 0xF1;
				g_defaultMsgAttr.m_TGT     = 0x6A;
				g_defaultMsgAttr.m_WAYS    = 0;
		  break;
		
		default:
			break;
	}
}

void ResetComPort()
{
	//a5 a5 00 01 fa 04 
	uint16_t number;
	uint8_t sendbuf[16] = { 0xa5,0xa5,0x00,0x01,0xfa,0x04 };

	number = 6;
	vTaskDelay(10);
	USB_BULK_Queue_Write(&USB_RX_QueueData, sendbuf , number);
	vTaskDelay(200);
}

OBDII_INIT_MODE OBDII_INIT_IO_FUNC(uint8_t index)
{
	uint16_t number = 0;
	OBDII_INIT_MODE mode = 0;
	uint8_t sendbuf[64];
	
	SetFrameTimeOut(index);
	number = CYObdiiInitIoCmd[index].CMD_DATA[0];
	memcpy(sendbuf, CYObdiiInitIoCmd[index].CMD_DATA+1, number);
	mode=ObdiiInitIoCmd[index].Init_Mode;
	g_defaultMsgAttr.m_PCK_TYPE = 0x00;
	//g_defaultMsgAttr.m_ORD = ObdiiInitIoCmd[index].Msg_Ord;
	number = pro_packsenddata(sendbuf, number, CYObdiiInitIoCmd[index].Msg_Ord);
	
	if(number == 0)
	{
		return 0;
	}
	
	ResetComPort();
	
	USB_BULK_Queue_Write(&USB_RX_QueueData,sendbuf,number);
	OBDII_ReadBuf(sendbuf, &number,1500);
	//number = pro_SendRecv((uint8_t *)ObdiiInitIoCmd[index].CMD_DATA,(uint8_t *)sendbuf);
	if (number < 1)
	{
		return 0;
	}
	vTaskDelay(1000);
	OBDII_MSG_ALL(mode,index);
	if(g_defaultMsgAttr.m_PCK_TYPE == 0x22)
	{
		//m_canbussendid=0x000007df;/*协议默认发送ID*/
		//m_canbusreadid=0x000007e8;/*协议当前系统发送ID*/
		//m_canbusflowid=0x000007e0;/*协议当前系统接收ID*/
	}
	return mode;
}



int cypro_initecu_much(OBDII_INIT_MODE mode)
{
	char gIdeBuf[16];
	uint16_t number = 0;
	uint8_t sendbuf[64];
	int delay = g_defaultMsgAttr.m_AnswerFrameTimeOut;
	if(ObdiiInitECUCmd[mode].Msg_Ord == 0x33)
	{
		if(mode == 0x03)
		{
		 uint8_t buf1[16] ={0xa5, 0xa5, 0x00 , 0x06, 0x33, 0x81, 0x10, 0xf1, 0x81, 0x04, 0xbe };
		 USB_BULK_Queue_Write(&USB_RX_QueueData,buf1,11);
		}
		if(mode == 0x04)
		{
		 uint8_t buf2[16]={0xa5, 0xa5, 0x00 , 0x06, 0x33, 0x81, 0x11, 0xf1, 0x81, 0x03, 0xc0 };
		 USB_BULK_Queue_Write(&USB_RX_QueueData,buf2,11);
		}
		OBDII_ReadBuf(sendbuf, &number,2000);
		if(number == 0)
		{
			return 0;
		}
		vTaskDelay(delay);
	}
//	number = pro_SendRecv((uint8_t *)ObdiiInitECUCmd[mode].CMD_DATA, (uint8_t *)sendbuf);
	
	if(number == 0)
	{
		return 0;
	}
	if(sendbuf[0] == 0x7f)
	{
		return 0;
	}

	if	(mode==OBDII_MODE_01)
	{
		sprintf(gIdeBuf,"%c%c%c%c%c%c%c%c",sendbuf[3],sendbuf[4],sendbuf[5],sendbuf[6],sendbuf[7],sendbuf[8],sendbuf[9],sendbuf[10]);
	}	
	else if	(mode==OBDII_MODE_02)
	{
		sprintf(gIdeBuf,"%c%c%c%c%c%c%c%c%c%c%c%c%c%c",sendbuf[3],sendbuf[4],sendbuf[5],sendbuf[6],sendbuf[7],sendbuf[8],sendbuf[9],sendbuf[10],sendbuf[11],sendbuf[12],sendbuf[13],sendbuf[14],sendbuf[15],sendbuf[16]);
	}
	else	if	(mode==OBDII_MODE_03)
	{
		sprintf(gIdeBuf,"%c%c%c%c%c%c%c%c%c%c%c%c%c%c",sendbuf[3],sendbuf[4],sendbuf[5],sendbuf[6],sendbuf[7],sendbuf[8],sendbuf[9],sendbuf[10],sendbuf[11],sendbuf[12],sendbuf[13],sendbuf[14],sendbuf[15],sendbuf[16]);
	}
	else	if	(mode==OBDII_MODE_04)
	{
		sprintf(gIdeBuf,"%c%c%c%c%c%c%c%c%c%c%c%c%c%c",sendbuf[3],sendbuf[4],sendbuf[5],sendbuf[6],sendbuf[7],sendbuf[8],sendbuf[9],sendbuf[10],sendbuf[11],sendbuf[12],sendbuf[13],sendbuf[14],sendbuf[15],sendbuf[16]);
	}
	else
	{
		return 0;
	}
//	checkEcu(gIdeBuf);
//	Data_collection_pack_to_json(g_datacollection);
	return 1;
}



bool OBDII_Much_Init()
{
	uint8_t i =0;
	uint8_t initnum = 0;
	OBDII_INIT_MODE  mode = 0;
	int num = 0;
	char buf[40] = {0};

	get_task_info(Task_ObdiiCmdProcess_Proc_handle);
	printfUartObdiideBug("[%s] m_protocol_index : %d\r\n",__func__,m_protocol_index);
	if(m_protocol_index != -1) //直接使用记录的协议
	{
		vTaskDelay(3000);
		if(OBDII_Init_IO(m_protocol_index))
		{
				//熄火初始化ECU失败
				if(pro_much_init(m_protocol_index)) //初始化ECU
				{
						pro_obdii_protocol_init(m_protocol_index);
//						if(data_flag == 1 && diag_flag == 0)//数据采集
//						if(data_flag == 1)//数据采集
						{
								pro_read_data();  //读数据流
								pro_read_info(m_protocol_index);
								memset(ready_status_buf,0,20);
								memset(iupr_buf,0,20);
								get_ready_status(ready_status_buf);
								get_vehicle_iupr(m_protocol_index,iupr_buf);
//								data_flag = 0;
						}				
						pro_exit_dialog(m_protocol_index);
						return true;
				}
				else
				{
						return false;
				}
		}
		else
				return false;
	}
	
	initnum = sizeof(ObdiiInitIoCmd)/sizeof(OBDII_CMD_INIT);
	printfUartObdiideBug("InitIO Number == %d\r\n",initnum);
	for(i=0;i <initnum;i++)
	{
		reset_global_data();//重置全局变量数据
		printfUartObdiideBug("InitIO mode== %d\r\n",ObdiiInitIoCmd[i].Init_Mode);
		if(OBDII_Init_IO(ObdiiInitIoCmd[i].Init_Mode)) //初始化IO
		{
			if(pro_much_init(ObdiiInitIoCmd[i].Init_Mode)) //初始化ECU
			{
				Vehicle_fire = 1;
				m_protocol_index = i;            //记录协议
				pro_obdii_protocol_init(ObdiiInitIoCmd[i].Init_Mode);
//				pro_read_dtc(m_protocol_index,&num,buf);   //读码
				pro_obdii_getpid_infotype(ObdiiInitIoCmd[i].Init_Mode);//版本信息侦测
				pro_read_info(ObdiiInitIoCmd[i].Init_Mode);   //读版本信息
				pro_obdii_getpid_diag(); //数据流侦测
				pro_read_data();  //读数据流
				pro_exit_dialog(ObdiiInitIoCmd[i].Init_Mode);
				return true;
			}
		}
	}
	
	return false;
}


//初始化IO
bool OBDII_Init_IO(OBDII_INIT_MODE index)
{
	uint16_t number = 0;
	OBDII_INIT_MODE mode = 0;
	uint8_t sendbuf[128];
	uint8_t recvbuf[32];
	
	printfUartObdiideBug("welcome to [%s]\r\n",__func__);
	if(!OBDII_SetFrameTimeOut(index))
	{
		return false;
	}
	memset(recvbuf,0,sizeof(recvbuf));
	number = ObdiiInitIoCmd[index].CMD_DATA[0];
	memcpy(sendbuf, ObdiiInitIoCmd[index].CMD_DATA+1, number);
	mode = ObdiiInitIoCmd[index].Init_Mode;
	g_defaultMsgAttr.m_PCK_TYPE = 0x00;
	number = pro_packsenddata(sendbuf, number, ObdiiInitIoCmd[index].Msg_Ord);
	if(number == 0)
	{
		return 0;
	}
	ResetComPort();
	vTaskDelay(20);
	USB_BULK_Queue_Write(&USB_RX_QueueData,sendbuf,number);
	OBDII_ReadBuf(recvbuf, &number,1500);
	//number = pro_SendRecv((uint8_t *)ObdiiInitIoCmd[index].CMD_DATA,(uint8_t *)sendbuf);
	if (number < 1)
	{
		printfUartObdiideBug("InitIO false\r\n");
		return false;
	}
	//vTaskDelay(1000);
	pro_set_can_id(index);
	printfUartObdiideBug("InitIO true\r\n");
	return true;
}

bool pro_much_init(OBDII_INIT_MODE mode)
{
	uint8_t *p_data;
	uint8_t p_data_len = 0;
	uint8_t sendbuf[64];

	memset(sendbuf,0,sizeof(sendbuf));
	switch(mode)
	{
		case OBDII_MODE_01:
		case OBDII_MODE_02:
		case OBDII_MODE_03:
		case OBDII_MODE_04:
			pro_msg_all(mode);
			pro_canbus_select_module(mode);
		break;
		case OBDII_MODE_05:
			pro_set_init_ecu_ord(mode);
			memset(sendbuf,0,sizeof(sendbuf));
			memcpy(sendbuf, ObdiiInitEcuCmd[OBDII_MODE_05].CMD_DATA, ObdiiInitEcuCmd[OBDII_MODE_05].CMD_DATA[0]+1);
			pro_initecu_much_5bps(sendbuf);
			pro_msg_all(mode);
			pro_default_select_module(mode);
		break;
		case OBDII_MODE_06:
			pro_set_init_ecu_ord(mode);
			memset(sendbuf,0,sizeof(sendbuf));
			memcpy(sendbuf, ObdiiInitEcuCmd[OBDII_MODE_06].CMD_DATA, ObdiiInitEcuCmd[OBDII_MODE_06].CMD_DATA[0]+1);
			pro_initecu_much_5bps(sendbuf);
			pro_msg_all(mode);
			pro_default_select_module(mode);
		break;
		case OBDII_MODE_07:
			pro_set_init_ecu_ord(mode);
			memset(sendbuf,0,sizeof(sendbuf));
			memcpy(sendbuf, ObdiiInitEcuCmd[OBDII_MODE_07].CMD_DATA, ObdiiInitEcuCmd[OBDII_MODE_07].CMD_DATA[0]+1);
			pro_initecu_much(sendbuf);
			pro_msg_all(mode);
			pro_default_select_module(mode);
		break;
		case OBDII_MODE_08:
		case OBDII_MODE_09:
			pro_msg_all(mode);
			pro_default_select_module(mode);
			break;
		default:
			break;
	}
}


bool pro_default_select_module(OBDII_INIT_MODE mode)
{
	uint8_t sendbuf[64];
	uint16_t number =0,count =0;
	uint8_t recvbuf[256];
	uint8_t module =0;
	int i, j;
	
	number = ObdiiEcuSelectCmd[mode].CMD_DATA[0];
	memset(sendbuf,0,sizeof(sendbuf));
	memcpy(sendbuf, ObdiiEcuSelectCmd[mode].CMD_DATA, number+1);
	
	number = pro_SendRecv(sendbuf, recvbuf);
	while(number > 0 && count < 256)
	{
		module = cmd_get_ecumodule();
		//
		for(i=0;i<count;i++)
		{
			if(m_modulebuf[i] == module)
			{
				break;
			}
		}
		if(i<count && m_modulebuf[i] == module)
		{
			break;
		}
		m_modulebuf[count++] = cmd_get_ecumodule();
		number = pro_OBDII_ReadBuffer(recvbuf);
	}
	cmd_set_modulenum(count);
	//调整顺序
	for (i = 0; i < count-1; i++)
	{
		for(j=1;j<count;j++)
		{
			if(m_modulebuf[i] > m_modulebuf[j])
			{
				uint8_t temp = m_modulebuf[i];
				m_modulebuf[i] = m_modulebuf[j];
				m_modulebuf[j] = temp;
			}
		}
	}
	if(count == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool pro_canbus_select_module(OBDII_INIT_MODE mode)
{
	int i, j;
	uint8_t sendbuf[64];
	uint8_t recvbuf[256];
	uint8_t sendnum =0,check;
	uint16_t count, number=0;
	uint32_t canid;
	
	count = 0;
	sendnum = ObdiiEcuSelectCmd[mode].CMD_DATA[0];
	memset(sendbuf,0,sizeof(sendbuf));
	memcpy(sendbuf, ObdiiEcuSelectCmd[mode].CMD_DATA+1,sendnum);

	check = sendbuf[1]+0x40;
	number = CommSendBufferCanbus(sendbuf, sendnum);
	
	while(count < 256)
	{
		number = CommReadBufferDefault(recvbuf, 256, 1000);
		number = CommUnpackBufferDeafult(recvbuf, number);
		
		if(number < 4)
		{
			break;
		}
		canid = (recvbuf[0]<<24) + (recvbuf[1]<<16) + (recvbuf[2]<<8) + recvbuf[3];
		for(i=0;i<count;i++)
		{
			if(m_modulebuf[i] == canid)
			{
				break;
			}
		}
		if(i<count && m_modulebuf[i] == canid)
		{
			break;
		}
		m_modulebuf[count++] = canid;
	}
	cmd_set_modulenum(count);
	
	for (i = 0; i < count-1; i++)
	{
		for(j=1;j<count;j++)
		{
			if(m_modulebuf[i] > m_modulebuf[j])
			{
				uint8_t temp = m_modulebuf[i];
				m_modulebuf[i] = m_modulebuf[j];
				m_modulebuf[j] = temp;
			}
		}
	}
	
	if(count == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void pro_set_can_id(OBDII_INIT_MODE mode)
{
	switch(mode)
	{
		case OBDII_MODE_01:
				m_canbussendid = 0x000007DF;
				m_canbusreadid = 0x000007E8;
				m_canbusflowid = 0x000007E0;
			break;
		
		case OBDII_MODE_02:
				m_canbussendid = 0x98DB33F1;
				m_canbusreadid = 0x98DAF111;
				m_canbusflowid = 0x98DA11F1;
			break;
		
		case OBDII_MODE_03:
				m_canbussendid = 0x000007DF;
				m_canbusreadid = 0x000007E8;
				m_canbusflowid = 0x000007E0;
			break;
		
		case OBDII_MODE_04:
				m_canbussendid = 0x98DB33F1;
				m_canbusreadid = 0x98DAF111;
				m_canbusflowid = 0x98DA11F1;
			break;
		default:
			break;
	}
}

void pro_autokeep_obdii()
{
	uint16_t number;
	uint8_t recvbuf[256];
	number = 0;
	if(g_defaultMsgAttr.m_PCK_TYPE == 0x05)
	{
		number = 12;
		memcpy(recvbuf, "\xa5\xa5\x00\x07\x43\x68\x6a\xf1\x01\x00\xc4\x2d", number);
	}
	else if(g_defaultMsgAttr.m_PCK_TYPE == 0x0d)
	{
		number = 11;
		memcpy(recvbuf, "\xa5\xa5\x00\x06\x43\xc1\x33\xf1\x3e\x23\x70", number);
	}
	if (number > 0)
	{
		USB_BULK_Queue_Write(&USB_RX_QueueData,recvbuf,number);
		number = CommReadBufferDefault(recvbuf, 256, 200);
		number = CommUnpackBufferDeafult(recvbuf, number);
	}
}

bool pro_initecu_much_5bps(uint8_t *cmdbuf)
{
	uint16_t number =0;
	uint8_t  recvbuf[512];
	uint8_t mode = 0xFF;
	number = pro_SendRecv(cmdbuf, recvbuf);
	if(number > 0 && number < 8)
	{
		number = CommReadBufferDefault(recvbuf, 256, 3000);
		number = CommUnpackBufferDeafult(recvbuf, number);
	}
	if(number != 8)
	{
		return FALSE;
	}
	if(mode == 0xff)
	{
		if(recvbuf[5] == 0x08 && recvbuf[6] == 0x08)
		{
			g_defaultMsgAttr.m_PCK_TYPE= 0x05;
			g_defaultMsgAttr.m_SRC     = 0xf1;
			g_defaultMsgAttr.m_TGT     = 0x6a;
			b_add_6a = true;
		}
		else
		{
			g_defaultMsgAttr.m_PCK_TYPE= 0x0d;
			g_defaultMsgAttr.m_SRC     = 0xf1;
			g_defaultMsgAttr.m_TGT     = 0x33;
			b_add_6a = false;
		}
	}
	pro_autokeep_obdii();
	return TRUE;
}

bool pro_initecu_much(uint8_t * sendbuf)
{
	uint16_t number;
	uint8_t recvbuf[256];
	
//只有model7用到,写死
	g_defaultMsgAttr.m_CS_MODE = 0;
	g_defaultMsgAttr.m_ORD     = 0x23;
	g_defaultMsgAttr.m_PCK_TYPE= 0x0D;
	g_defaultMsgAttr.m_RWAYS   = 0x01;
	g_defaultMsgAttr.m_SRC     = 0xF1;
	g_defaultMsgAttr.m_TGT     = 0x33;
	g_defaultMsgAttr.m_WAYS    = 0;
	
	number = pro_SendRecv(sendbuf, recvbuf);
	
	if(number == 0)
	{
		return false;
	}
	pro_autokeep_obdii();
	return true;
}

void pro_read_dtc(OBDII_INIT_MODE index,int* dtc_number,char* dtc_list)
{
	static uint8_t  recvbuf[256];
	uint8_t  sendcmd[2] = {01,03}; //读当前码
	uint8_t  sendcmd2[2] = {01,07}; //历史
	uint8_t  sendcmd3[3] = {02,01,01};

	char dtcTemp[32]={0};
	uint8_t dtclen =0;
	uint8_t pos = 0;
	uint16_t number =0;
	uint8_t dtcnumber=0, msgnum=0;
	uint8_t i=0,ipos1=0,ipos2=0,j=0;
	uint32_t canread;	

	g_vehiclediagnosie.fault_total =0;
//	*dtc_number = 0;
//	memset(g_vehiclediagnosie.fault_list,0,sizeof(g_vehiclediagnosie.fault_list));
	memset(recvbuf,0,sizeof(recvbuf));
	switch(index)
	{
		case OBDII_MODE_01:
		case OBDII_MODE_02:
		case OBDII_MODE_03:
		case OBDII_MODE_04:
			for(i=0;i<cmd_get_modulenum();i++)
			{
					dtcnumber = 0;
					if(i>0)
					{
						canread = m_modulebuf[i];
						m_canbusreadid = m_modulebuf[i];
						printfUartObdiideBug("i = %X\r\n",i);
						printfUartObdiideBug("canread = %X\r\n",canread);
						SetCanbusFilter(canread);
					}
					number = pro_SendRecv(sendcmd, recvbuf); //当前故障码
					printfUartObdiideBug("number = %d\r\n",number);
					if(number ==0)
					{
						m_canbusreadid = m_modulebuf[0];
						SetCanbusFilter(m_modulebuf[0]);
						return;
					}
					dtclen = 2;
					pos = 2;
					dtcnumber = recvbuf[pos-1];
					if(dtcnumber>10)
					{
						dtcnumber=10;
					}
					*dtc_number += dtcnumber;
					
					printfUartObdiideBug("dtcnumber1 = %d\r\n",dtcnumber);
					printfUartObdiideBug("fault_total 11 = %d\r\n",*dtc_number);
					for(j=0;j<dtcnumber;j++)
					{
						ipos1 = pos + j*dtclen;
						ipos2 = pos + j*dtclen +1;
						sprintf(dtcTemp,"%02X%02X",recvbuf[ipos1],recvbuf[ipos2]);
						strcat(dtc_list,dtcTemp);
//						strcat(dtc_list,";");
						//有两个及以上故障码时加上,
						strcat(dtc_list,",");
						printfUartObdiideBug("dtcTemp == %s\r\n",dtcTemp);
					}
					
					number = pro_SendRecv(sendcmd2, recvbuf); //历史故障码
					printfUartObdiideBug("number = %d\r\n",number);
					if(number ==0)
					{
						m_canbusreadid = m_modulebuf[0];
						SetCanbusFilter(m_modulebuf[0]);
						return;
					}
					dtclen = 2;
					pos = 2;
					dtcnumber = 0;
					dtcnumber = recvbuf[pos-1];
					if(dtcnumber>(10-*dtc_number))
					{
						dtcnumber=10-*dtc_number;
					}
					*dtc_number += dtcnumber;
					printfUartObdiideBug("dtcnumber2 = %d\r\n",dtcnumber);
					printfUartObdiideBug("fault_total 22 = %d\r\n",*dtc_number);
					for(j=0;j<dtcnumber;j++)
					{
						ipos1 = pos + j*dtclen;
						ipos2 = pos + j*dtclen +1;
						sprintf(dtcTemp,"%02X%02X",recvbuf[ipos1],recvbuf[ipos2]);
						memcpy(dtc_list,dtcTemp,4);
						strcat(dtc_list,",");
						printfUartObdiideBug("dtcTemp == %s\r\n",dtcTemp);
					}
				}
			
				m_canbusreadid = m_modulebuf[0];
				if ((cmd_get_modulenum() >1))
				{
					SetCanbusFilter(m_canbusreadid);
				}
				break;
				
			case OBDII_MODE_05:
			case OBDII_MODE_06:
			case OBDII_MODE_07:
			case OBDII_MODE_08:
			case OBDII_MODE_09:
				printfUartObdiideBug("8888888888888888888888888888888\r\n");
				number = pro_SendRecv(sendcmd3,recvbuf);
				printfUartObdiideBug("DTC recvnumber== %d\r\n",number);
				dtc_number += number;
			  if(number ==0) //系统正常
				{
					return;
				}
				if(recvbuf[0] == 0x7f)
				{
					return;
				}
				dtcnumber = recvbuf[2]&0x7f;
				
				if(dtcnumber>10)
				{
					dtcnumber=10;
				}
				msgnum = dtcnumber/3 + 1;
				number=0;
				number = pro_SendRecv_OBDII_MORE(sendcmd, recvbuf, msgnum+1); //当前
				
				printfUartObdiideBug("DTC pro_SendRecv_OBDII_MORE== %d\r\n",number);
			
				if(number > 0)
				{
					memset(recvbuf+number, 0x00, 256-number);
					for(i=0;i<dtcnumber;i++)
					{
						int dtccode = recvbuf[i*2]*0x100 + recvbuf[i*2+1];
						sprintf(dtcTemp,"%02X%02X",recvbuf[i*2],recvbuf[i*2+1]);
						memcpy(dtc_list,dtcTemp,4);
						strcat(dtc_list,",");
						if(dtccode == 0)
						{
							break;
						}
						printfUartObdiideBug("dtcTemp == %s\r\n",dtcTemp);
					}
				}
				dtclen = 2;
				number = pro_SendRecv(sendcmd2, recvbuf); //历史故障码
				while(number >0)
				{
					if(recvbuf[0] == 0x7f)
					{
						return;
					}
					for(i=0;i<3;i++)
					{
						int dtccode = recvbuf[i*2]*0x100 + recvbuf[i*2+1];
						sprintf(dtcTemp,"%02X%02X",recvbuf[i*2],recvbuf[i*2+1]);
						if(dtccode == 0)
						{
							break;
						}
						memcpy(dtc_list,dtcTemp,4);
						strcat(dtc_list,",");
						printfUartObdiideBug("dtcTemp == %s\r\n",dtcTemp);
					}

					OBDII_ReadBuf(recvbuf, &number,200);
					if(number > 9)
					{
						number -= 9;
						memcpy(recvbuf, recvbuf+8, number);
					}
					else
					{
						number = 0;
					}
				}
				
				break;
			default:
				break;
	}
}

//数据流侦测
void pro_obdii_getpid_diag()
{
	uint8_t cmdnum =0;
	uint8_t i =0,j=0,k=0,count;
	uint8_t sendbuf[16];
	uint16_t number =0;
	uint8_t recvbuf[256];
	
	cmdnum = sizeof(StreamScan)/sizeof(OBDII_CMD_STREAMSCAN);
	m_diagdatapid[0] = 0xff;	
	count = 1;
	for(k =0;k<cmdnum;k++)
	{
		memset(sendbuf,0,sizeof(sendbuf));
		memcpy(sendbuf,StreamScan[k].CMD_DATA,3);
		if(k>0)
		{
			if((recvbuf[5]&0x01)!=0x01)
			{
				return;
			}
		}
		number = pro_SendRecv(sendbuf, recvbuf);
		if(number == 0)
		{
			return;
		}
		for(i=0;i<4;i++)
		{
			for(j=0;j<8;j++)
			{				
				uint8_t bit = 0x80 >> j;
				if((recvbuf[2+i]&bit) == bit)
				{
					m_diagdatapid[count] = 0xff;
				}	
				count++;
			}
		}
	}
}


void pro_obdii_getpid_infotype(OBDII_INIT_MODE index)
{
	uint8_t  sendbuf1[16]={0x02,0x09,0x00};
	uint8_t  sendbuf2[16]={0x02,0x09,0x20};

	uint16_t number =0;
	uint8_t recvbuf[256];
	uint8_t pos=0;
	uint16_t i=0,j=0,k=0,count=1;
	
	
	switch(index)
	{
		case OBDII_MODE_01:
		case OBDII_MODE_02:
		case OBDII_MODE_03:
		case OBDII_MODE_04:
			pos = 0x02;
			break;
		case OBDII_MODE_05:
		case OBDII_MODE_06:
		case OBDII_MODE_07:
		case OBDII_MODE_08:
		case OBDII_MODE_09:
			pos = 0x03;
		default:
				break;
	}
	m_infotype[0] = 0xff;	
	memset(recvbuf,0,sizeof(recvbuf));
	for(k=0;k<2;k++)
	{
		
		if(k==0)
		{
			number = pro_SendRecv(sendbuf1, recvbuf);
		}
		if(k==1)
		{
			if((recvbuf[0x05]&0x01)==0x01)
			{
				number = pro_SendRecv(sendbuf2, recvbuf);
			}
			else
			{
				return;
			}
		}
		
		if(number == 0)
		{
			return;
		}
		if(number == 0x03 && recvbuf[0] == 0x7f)
		{
			return;
		}
		for(i=0;i<4;i++)
		{
			for(j=0;j<8;j++)
			{				
				uint8_t bit = 0x80 >> j;
				if((recvbuf[pos+i]&bit) == bit)
				{
					m_infotype[count] = 0xff;
				}
				count++;
			}
		}
	}
}


void pro_read_iupr2(uint8_t *recvbuf,uint16_t number,char* buf)
{
	uint16_t num=0;
	double  iupr;
	
	if(number > 0x3)
	{		
		num = (recvbuf[0x3]*0x100+recvbuf[0x4]);   //OBD监控遇到条件次数
	}
	if(number > 0x5)
	{
		num = (recvbuf[0x5]*0x100+recvbuf[0x6]);   //点火循环计数器
	}
	if(number > 0x7) 
	{
		num = (recvbuf[0x7]*0x100+recvbuf[0x8]);   //非甲烷烃催化剂监控器完成条件计数
	}
	if(number > 0x9) 
	{
		num = (recvbuf[0x9]*0x100+recvbuf[0xa]);   //非甲烷烃催化剂监控器遇到的条件计数
		iupr =(recvbuf[0x7]*0x100+recvbuf[0x8])/(recvbuf[0x9]*0x100+recvbuf[0xa]); //非甲烷烃催化剂监控器实际监视频率
		buf[0] = (recvbuf[0x7]*0x100+recvbuf[0x8])/(recvbuf[0x9]*0x100+recvbuf[0xa]);
	}
	if(number > 0x0b)
	{
		num = (recvbuf[0x0b]*0x100+recvbuf[0x0c]); //氮氧化物/选择性催化还原催化剂监控器完成条件计数
	}
	if(number > 0x0d) 
	{
		num = (recvbuf[0x0d]*0x100+recvbuf[0x0e]);  //氮氧化物/选择性催化还原催化剂监控器遇到的条件计数
		iupr =(recvbuf[0x0b]*0x100+recvbuf[0x0c])/(recvbuf[0x0d]*0x100+recvbuf[0x0e]);//氮氧化物/选择性催化还原催化剂监控器实际监视频率
		buf[1] = (recvbuf[0x0b]*0x100+recvbuf[0x0c])/(recvbuf[0x0d]*0x100+recvbuf[0x0e]);
	}
	if(number > 0x0f)
	{
		num = (recvbuf[0x0f]*0x100+recvbuf[0x10]); //氮氧化物吸收器监控器完成条件计数
	}
	if(number > 0x11)
	{
		num = (recvbuf[0x11]*0x100+recvbuf[0x12]); //氮氧化物吸收器监控器遇到的条件计数
		iupr = (recvbuf[0x0f]*0x100+recvbuf[0x10]) /(recvbuf[0x11]*0x100+recvbuf[0x12]); //氮氧化物吸收器监控器实际监视频率
		buf[2] = (recvbuf[0x0f]*0x100+recvbuf[0x10]) /(recvbuf[0x11]*0x100+recvbuf[0x12]);
	}
	if(number > 0x13)
	{
		num = (recvbuf[0x13]*0x100+recvbuf[0x14]); //微粒过滤器监控器完成条件计数
	}
	if(number > 0x15)
	{
		num = (recvbuf[0x15]*0x100+recvbuf[0x16]); //微粒过滤器监控器遇到的条件计数
		iupr = (recvbuf[0x13]*0x100+recvbuf[0x14]) /(recvbuf[0x15]*0x100+recvbuf[0x16]); //微粒过滤器监控器实际监视频率
		buf[3] = (recvbuf[0x13]*0x100+recvbuf[0x14]) /(recvbuf[0x15]*0x100+recvbuf[0x16]);
	}
	if(number > 0x17)
	{
		num = (recvbuf[0x17]*0x100+recvbuf[0x18]); //废气传感器监控器完成条件计数
	}
	if(number > 0x19)
	{
		num = (recvbuf[0x19]*0x100+recvbuf[0x1a]); //废气传感器监控器完成条件计数
		iupr = (recvbuf[0x17]*0x100+recvbuf[0x18]) /(recvbuf[0x19]*0x100+recvbuf[0x1a]); //废气传感器监控器实际监视频率
		buf[4] = (recvbuf[0x17]*0x100+recvbuf[0x18]) /(recvbuf[0x19]*0x100+recvbuf[0x1a]);
	}
	if(number > 0x1b)
	{
		num = (recvbuf[0x1b]*0x100+recvbuf[0x1c]); //废气再循环和/或可变气门正时系统监控器完成条件计数
	}
	if(number > 0x1d)
	{
		num = (recvbuf[0x1d]*0x100+recvbuf[0x1e]); //废气再循环和/或可变气门正时系统监控器完成条件计数
		iupr = (recvbuf[0x1b]*0x100+recvbuf[0x1c])/(recvbuf[0x1d]*0x100+recvbuf[0x1e]); //废气再循环和/或可变气门正时系统实际监视频率
		buf[5] = (recvbuf[0x1b]*0x100+recvbuf[0x1c])/(recvbuf[0x1d]*0x100+recvbuf[0x1e]);
	}
	if(number > 0x1f)
	{
		num = (recvbuf[0x1f]*0x100+recvbuf[0x20]); //增压压力监控器完成条件计数
	}
	if(number > 0x21)
	{
		num = (recvbuf[0x21]*0x100+recvbuf[0x22]); //增压压力监控器遇到的条件计数
		iupr = (recvbuf[0x1f]*0x100+recvbuf[0x20])/(recvbuf[0x21]*0x100+recvbuf[0x22]); //增压压力监控器实际监视频率
		buf[6] = (recvbuf[0x1f]*0x100+recvbuf[0x20])/(recvbuf[0x21]*0x100+recvbuf[0x22]);
	}
	if(number > 0x23)
	{
		num = (recvbuf[0x23]*0x100+recvbuf[0x24]); //燃油监控器完成条件计数
	}
	if(number > 0x25)
	{
		num = (recvbuf[0x25]*0x100+recvbuf[0x26]); //燃油监控器完成条件计数
		iupr = (recvbuf[0x23]*0x100+recvbuf[0x24])/(recvbuf[0x25]*0x100+recvbuf[0x26]); //燃油监控器实际监视频率
		buf[7] = (recvbuf[0x23]*0x100+recvbuf[0x24])/(recvbuf[0x25]*0x100+recvbuf[0x26]); 
	}
}

void pro_read_iupr1(uint8_t *recvbuf,uint16_t number,char* buf)
{
	uint16_t num=0;
	double  iupr;
	
	if(number > 0x3)
	{		
		num = (recvbuf[0x3]*0x100+recvbuf[0x4]);   //OBD监控遇到条件次数
	}
	if(number > 0x5)
	{
		num = (recvbuf[0x5]*0x100+recvbuf[0x6]);   //点火循环计数器
	}
	if(number > 0x7) 
	{
		num = (recvbuf[0x7]*0x100+recvbuf[0x8]);   //催化剂监控器完成计数 组1
	}
	if(number > 0x9) 
	{
		num = (recvbuf[0x9]*0x100+recvbuf[0xa]);   //催化剂监控器遇到的条件计数 组1
		iupr =(recvbuf[0x7]*0x100+recvbuf[0x8])/(recvbuf[0x9]*0x100+recvbuf[0xa]);       //催化剂监控器实际监视频率 组1
		buf[8] =(recvbuf[0x7]*0x100+recvbuf[0x8])/(recvbuf[0x9]*0x100+recvbuf[0xa]); 
	}
	if(number > 0x0b)
	{
		num = (recvbuf[0x0b]*0x100+recvbuf[0x0c]); //催化剂监控器完成计数 组2
	}
	if(number > 0x0d) 
	{
		num = (recvbuf[0x0d]*0x100+recvbuf[0x0e]);   //催化剂监控器遇到的条件计数 组2
		iupr =(recvbuf[0x0b]*0x100+recvbuf[0x0c])/(recvbuf[0x0d]*0x100+recvbuf[0x0e]);       //催化剂监控器实际监视频率 组2
		buf[9] = (recvbuf[0x0b]*0x100+recvbuf[0x0c])/(recvbuf[0x0d]*0x100+recvbuf[0x0e]);
	}
	if(number > 0x0f)
	{
		num = (recvbuf[0x0f]*0x100+recvbuf[0x10]); //氧传感器监控器完成计数 组1
	}
	if(number > 0x11)
	{
		num = (recvbuf[0x11]*0x100+recvbuf[0x12]); //氧传感器监控器遇到的状况计数 组1
		iupr = (recvbuf[0x0f]*0x100+recvbuf[0x10]) /(recvbuf[0x11]*0x100+recvbuf[0x12]); //氧传感器实际监视频率 组1
		buf[10] = (recvbuf[0x0f]*0x100+recvbuf[0x10]) /(recvbuf[0x11]*0x100+recvbuf[0x12]);
	}
	if(number > 0x13)
	{
		num = (recvbuf[0x13]*0x100+recvbuf[0x14]); //氧传感器监控器完成计数 组2
	}
	if(number > 0x15)
	{
		num = (recvbuf[0x15]*0x100+recvbuf[0x16]); //氧传感器监控器遇到的状况计数 组2
		iupr = (recvbuf[0x13]*0x100+recvbuf[0x14]) /(recvbuf[0x15]*0x100+recvbuf[0x16]); //氧传感器实际监视频率 组2
		buf[11] = (recvbuf[0x13]*0x100+recvbuf[0x14]) /(recvbuf[0x15]*0x100+recvbuf[0x16]);
	}
	if(number > 0x17)
	{
		num = (recvbuf[0x17]*0x100+recvbuf[0x18]); //废气再循环和/或可变气门正时系统监控器完成条件计数
	}
	if(number > 0x19)
	{
		num = (recvbuf[0x19]*0x100+recvbuf[0x1a]); //废气再循环和/或可变气门正时系统监控器遇到的条件计数
		iupr = (recvbuf[0x17]*0x100+recvbuf[0x18]) /(recvbuf[0x19]*0x100+recvbuf[0x1a]); //废气再循环和/或可变气门正时系统实际监视频率
		buf[12] = (recvbuf[0x17]*0x100+recvbuf[0x18]) /(recvbuf[0x19]*0x100+recvbuf[0x1a]);
	}
	if(number > 0x1b)
	{
		num = (recvbuf[0x1b]*0x100+recvbuf[0x1c]); //空气监控器完成条件次数(二次空气)
	}
	if(number > 0x1d)
	{
		num = (recvbuf[0x1d]*0x100+recvbuf[0x1e]); //空气监控器遇到的条件计数(二次空气)
		iupr = (recvbuf[0x1b]*0x100+recvbuf[0x1c])/(recvbuf[0x1d]*0x100+recvbuf[0x1e]); //空气监控器实际监视频率(二次空气)
		buf[13] = (recvbuf[0x1b]*0x100+recvbuf[0x1c])/(recvbuf[0x1d]*0x100+recvbuf[0x1e]);
	}
	if(number > 0x1f)
	{
		num = (recvbuf[0x1f]*0x100+recvbuf[0x20]); //蒸发监控器完成条件计数
	}
	if(number > 0x21)
	{
		num = (recvbuf[0x21]*0x100+recvbuf[0x22]); //蒸发监控器遇到的条件计数
		iupr = (recvbuf[0x1f]*0x100+recvbuf[0x20])/(recvbuf[0x21]*0x100+recvbuf[0x22]); //蒸发器实际监视频率
		buf[14] = (recvbuf[0x1f]*0x100+recvbuf[0x20])/(recvbuf[0x21]*0x100+recvbuf[0x22]);
	}
	if(number > 0x23)
	{
		num = (recvbuf[0x23]*0x100+recvbuf[0x24]); //二次氧传感器监控器完成计数 组1
	}
	if(number > 0x25)
	{
		num = (recvbuf[0x25]*0x100+recvbuf[0x26]); //二次氧传感器遇到的状况计数 组1
		iupr = (recvbuf[0x23]*0x100+recvbuf[0x24])/(recvbuf[0x25]*0x100+recvbuf[0x26]); //二次氧传感器实际监视频率 组1
		buf[15] = (recvbuf[0x23]*0x100+recvbuf[0x24])/(recvbuf[0x25]*0x100+recvbuf[0x26]); 
	}
	if(number > 0x27)
	{
		num = (recvbuf[0x27]*0x100+recvbuf[0x28]); //二次氧传感器监控器完成计数 组2
	}
	if(number > 0x29)
	{
		num = (recvbuf[0x29]*0x100+recvbuf[0x2a]); //二次氧传感器遇到的状况计数 组2
		iupr = (recvbuf[0x27]*0x100+recvbuf[0x28])/(recvbuf[0x29]*0x100+recvbuf[0x2a]); //二次氧传感器实际监视频率 组2
		buf[16] = (recvbuf[0x27]*0x100+recvbuf[0x28])/(recvbuf[0x29]*0x100+recvbuf[0x2a]);
	}
}


//就绪监控状态
void pro_read_ready(uint8_t *recvbuf,uint16_t number,char* buf)
{
	uint8_t result;
	
	if(number > 0x02)                 //故障灯状态
	{
		printfUartObdiideBug("pos==  %d\r\n",2);
		if((recvbuf[02]&0x80) == 0x80)  //打开
		{
			result = 1;
			g_vehiclediagnosie.mil_status=0x01;
			printfUartObdiideBug("故障灯状态 点亮\r\n");
		}
		else
		{
			result = 0;
			g_vehiclediagnosie.mil_status=0x00;
			printfUartObdiideBug("故障灯状态 熄灭\r\n");
		}
	}
	if(number > 0x03) 
	{
		printfUartObdiideBug("pos==  %d\r\n",3);
		if((recvbuf[03]&0x01) == 0x00)  //不点火监控就绪
		{
			result = 0; //不支持
			buf[0] = 0x00;
			printfUartObdiideBug("不点火监控就绪 不支持\r\n");
		}
		else
		{
			if((recvbuf[03]&0x10) == 0x00)
			{
				result = 1; //完成
				buf[0] = 0x01;
				printfUartObdiideBug("不点火监控就绪 完成\r\n");
			}
			else
			{
				result = 2; //未完成
				buf[0] = 0x02;
				printfUartObdiideBug("不点火监控就绪 未完成\r\n");
			}
		}
		if((recvbuf[03]&0x02) == 0x00)  //燃油系统监控就绪
		{
			result = 0; //不支持
			buf[1] = 0x00;
		}
		else
		{
			if((recvbuf[03]&0x20) == 0x00)
			{
				result = 1; //完成
				buf[1] = 0x01;
			}
			else
			{
				result = 2; //未完成
				buf[1] = 0x02;
			}
		}
		if((recvbuf[03]&0x04) == 0x00)  //综合组件监控就绪
		{
			result = 0; //不支持
			buf[2] = 0x00;
		}
		else
		{
			if((recvbuf[03]&0x40) == 0x00)
			{
				result = 1; //完成
				buf[2] = 0x01;
			}
			else
			{
				result = 2; //未完成
				buf[2] = 0x02;
			}
		}
	}
	if(number > 0x04) 
	{
		printfUartObdiideBug("pos==  %d\r\n",4);
		if((recvbuf[0x03]&0x08) == 0x00)     
		{
			if((recvbuf[0x04]&0x01) == 0x00)   //催化剂监控就绪
			{
				result = 0; //不支持
				buf[3] = 0x00;
			}
			else
			{
				if((recvbuf[0x05]&0x01) == 0x00)
				{
					result = 1; //完成
					buf[3] = 0x01;
				}
				else
				{
					result = 0; //未完成
					buf[3] = 0x02;
				}
			}
			
			if((recvbuf[0x04]&0x02) == 0x00)   //加热型催化剂监控就绪
			{
				result = 0; //不支持
				buf[4] = 0x00;
			}
			else
			{
				if((recvbuf[0x05]&0x02) == 0x00)
				{
					result = 1; //完成
					buf[4] = 0x01;
				}
				else
				{
					result = 0; //未完成
					buf[4] = 0x02;
				}
			}
			if((recvbuf[0x04]&0x04) == 0x00)   //蒸发系统监控就绪
			{
				result = 0; //不支持
				buf[5] = 0x00;
			}
			else
			{
				if((recvbuf[0x05]&0x04) == 0x00)
				{
					result = 1; //完成
					buf[5] = 0x01;
				}
				else
				{
					result = 0; //未完成
					buf[5] = 0x02;
				}
			}
			if((recvbuf[0x04]&0x08) == 0x00)   //二次空气系统监控就绪
			{
				result = 0; //不支持
				buf[6] = 0x00;
			}
			else
			{
				if((recvbuf[0x05]&0x08) == 0x00)
				{
					result = 1; //完成
					buf[6] = 0x01;
				}
				else
				{
					result = 0; //未完成
					buf[6] = 0x02;
				}
			}
			if((recvbuf[0x04]&0x20) == 0x00)   //氧传感器监控就绪
			{
				result = 0; //不支持
				buf[7] = 0x00;
			}
			else
			{
				if((recvbuf[0x05]&0x20) == 0x00)
				{
					result = 1; //完成
					buf[7] = 0x01;
				}
				else
				{
					result = 0; //未完成
					buf[7] = 0x02;
				}
			}
			if((recvbuf[0x04]&0x40) == 0x00)   //氧传感器加热器监控就绪
			{
				result = 0; //不支持
				buf[8] = 0x00;
			}
			else
			{
				if((recvbuf[0x05]&0x40) == 0x00)
				{
					result = 1; //完成
					buf[8] = 0x01;
				}
				else
				{
					result = 0; //未完成
					buf[8] = 0x02;
				}
			}
			
			if((recvbuf[0x04]&0x80) == 0x00)   //废气再循环和/或可变气门正时系统监控就绪
			{
				result = 0; //不支持
				buf[9] = 0x00;
			}
			else
			{
				if((recvbuf[0x05]&0x80) == 0x00)
				{
					result = 1; //完成
					buf[9] = 0x01;
				}
				else
				{
					result = 0; //未完成
					buf[9] = 0x02;
				}
			}
		}
		else
		{
			if((recvbuf[0x04]&0x01) == 0x00)   //非甲烷烃催化剂监控就绪
			{
				result = 0; //不支持
				buf[10] = 0x00;
			}
			else
			{
				if((recvbuf[0x05]&0x01) == 0x00)
				{
					result = 1; //完成
					buf[10] = 0x01;
				}
				else
				{
					result = 0; //未完成
					buf[10] = 0x02;
				}
			}
			if((recvbuf[0x04]&0x02) == 0x00)   //氮氧化物/选择性催化还原后处理系统监控就绪
			{
				result = 0; //不支持
				buf[11] = 0x00;
			}
			else
			{
				if((recvbuf[0x05]&0x02) == 0x00)
				{
					result = 1; //完成
					buf[11] = 0x01;
				}
				else
				{
					result = 0; //未完成
					buf[11] = 0x02;
				}
			}
			if((recvbuf[0x04]&0x08) == 0x00)   //增压压力系统监控就绪
			{
				result = 0; //不支持
				buf[12] = 0x00;
			}
			else
			{
				if((recvbuf[0x05]&0x08) == 0x00)
				{
					result = 1; //完成
					buf[12] = 0x01;
				}
				else
				{
					result = 0; //未完成
					buf[12] = 0x02;
				}
			}
			if((recvbuf[0x04]&0x20) == 0x00)   //废气传感器监控就绪
			{
				result = 0; //不支持
				buf[13] = 0x00;
			}
			else
			{
				if((recvbuf[0x05]&0x20) == 0x00)
				{
					result = 1; //完成
					buf[13] = 0x01;
				}
				else
				{
					result = 0; //未完成
					buf[13] = 0x02;
				}
			}
			if((recvbuf[0x04]&0x40) == 0x00)   //微粒过滤器监控系统
			{
				result = 0; //不支持
				buf[14] = 0x00;
			}
			else
			{
				if((recvbuf[0x05]&0x40) == 0x00)
				{
					result = 1; //完成
					buf[14] = 0x01;
				}
				else
				{
					result = 0; //未完成
					buf[14] = 0x02;
				}
			}
		}
	}
}

//里程
void pro_read_kilometet(uint8_t *recvbuf,uint16_t number)
{
	
	double result;
	
	if(number>0x06)
	{
		if(recvbuf[02]&0x80)
		{
			result = 0x7FFFFFFF - (recvbuf[0x02]*0x1000000+recvbuf[0x03]*0x10000+recvbuf[0x04]*0x100+recvbuf[0x05]);
//			printfUartObdiideBug("pro_read_kilometet1 : %d\r\n",(recvbuf[0x02]*0x1000000+recvbuf[0x03]*0x10000+recvbuf[0x04]*0x100+recvbuf[0x05]));
		}
		else
		{
			result = (recvbuf[0x02]*0x1000000+recvbuf[0x03]*0x10000+recvbuf[0x04]*0x100+recvbuf[0x05]);
//			printfUartObdiideBug("pro_read_kilometet2 : %d\r\n",(recvbuf[0x02]*0x1000000+recvbuf[0x03]*0x10000+recvbuf[0x04]*0x100+recvbuf[0x05]));
		}
	}
	g_datacollection.kilometres = result;
}

//车速
void pro_read_speed(uint8_t *recvbuf,uint16_t number)
{
	double result;
	if(number>0x02)
	{
		result = recvbuf[2];
	}
	g_datacollection.speed = result;
}

//最大扭矩
void pro_read_max_storque(uint8_t *recvbuf,uint16_t number)
{
	int result;
	if(number>0x03)
	{
		result = recvbuf[0x02]*0x100+recvbuf[0x03];
	}
	g_datacollection.max_torque = result;
}

//输出扭矩
void pro_read_out_storque(uint8_t *recvbuf,uint16_t number)
{
	int result;
	if(number>0x02)
	{
		result = recvbuf[0x02]-125;
	}
	g_datacollection.out_torque = result;
}

//摩擦扭矩
void pro_read_friction_storque(uint8_t *recvbuf,uint16_t number)
{
	int result;
	if(number>0x02)
	{
		result = recvbuf[0x02]-125;
	}
	g_datacollection.friction_torque = result;
}

//发动机转速
void pro_read_engine_speed(uint8_t *recvbuf,uint16_t number)
{
	int result;
	if(number>0x03)
	{
		result = (recvbuf[0x02]*0x100+recvbuf[0x03])/4;
	}
	g_datacollection.engine_speed = result;
}

//燃料流量
void pro_read_engine_flow(uint8_t *recvbuf,uint16_t number)
{
	double result;
	if(number>0x03)
	{
		result = (recvbuf[0x02]*0x100+recvbuf[0x03])/20.0;
	}
	g_datacollection.engine_flow = result;
}

//SCR入口温度
void pro_read_in_temperature(uint8_t *recvbuf,uint16_t number)
{
	double result;
	if(number>0x03)
	{
		result = (recvbuf[0x03]*0x100+recvbuf[0x04])/10.0-40;
	}
	g_datacollection.put_temperature = result;
}

//SCR出口温度
void pro_read_out_temperature(uint8_t *recvbuf,uint16_t number)
{
	double result;
	if(number>0x04)
	{
		result = (recvbuf[0x05]*0x100+recvbuf[0x06])/10.0-40;
	}
	g_datacollection.out_temperature = result;
}

//dpf压差 组1
void pro_read_pressure_diff1(uint8_t *recvbuf,uint16_t number)
{
	double result;
	if(number>0x04)
	{
		if(recvbuf[03]&0x80)
		{
			result = 0x7FFF - (recvbuf[0x03]*0x100+recvbuf[0x04])/100.0;
		}
		else
		{
			result = (recvbuf[0x03]*0x100+recvbuf[0x04])/100.0;
		}
	}
	g_datacollection.pressure_diff = result;
}

//dpf压差 组2
void pro_read_pressure_diff2(uint8_t *recvbuf,uint16_t number)
{
	double result;
	if(number>0x04)
	{
		if(recvbuf[03]&0x80)
		{
			result = 0x7FFF - (recvbuf[0x03]*0x100+recvbuf[0x04])/100.0;
		}
		else
		{
			result = (recvbuf[0x03]*0x100+recvbuf[0x04])/100.0;
		}
	}
}

//进气量
void pro_read_intrake(uint8_t *recvbuf,uint16_t number)
{
	double result;
	if(number>0x03)
	{
		result = (recvbuf[0x02]*0x100+recvbuf[0x03])/100.0;
	}
	g_datacollection.intake_volume = result;
}

//反应剂余量
void pro_read_reactant_suiplus(uint8_t *recvbuf,uint16_t number)
{
	double result=0;
	if(number>0x06)
	{
		if((recvbuf[0x02]&0x04)== 0x04)
		{
			result = (recvbuf[0x07]*100/255.0);
		}
	}
	g_datacollection.reactant_surplus = result;
}

//油箱液位
void pro_read_tank_leverl(uint8_t *recvbuf,uint16_t number)
{
	double result;
	if(number>0x02)
	{
		result = (recvbuf[0x02]*100/255.0);
	}
	g_datacollection.tank_level = result;
}

//发动机冷却液温度
void pro_read_coolant_temperature1(uint8_t *recvbuf,uint16_t number)
{
	int result;
	if(number>0x02)
	{
		result = (recvbuf[0x02]-40);
	}
	g_datacollection.coolant_temperature = result;
}

//发动机冷却液温度2
void pro_read_coolant_temperature2(uint8_t *recvbuf,uint16_t number)
{
	uint16_t result;
	if(number>0x04)
	{
		if((recvbuf[0x02]&0x02)==0x02)
		{
			result = (recvbuf[0x04]-40);
		}
		
	}
}

//喷油量
void pro_read_fuel_injection(uint8_t *recvbuf,uint16_t number)
{
	double result;
	if(number>0x03)
	{
		result = (recvbuf[0x02]*0x100+ recvbuf[0x03])/20.0;
	}
	g_datacollection.fuel_injection_quantity = result;
}

//电瓶电压
void pro_read_voltage(uint8_t *recvbuf,uint16_t number)
{
	double result;

	if(number>0x03)
	{
		result = (recvbuf[0x02]*0x100+ recvbuf[0x03])/1000.0;
	}
	g_datacollection.voltage = result;
}

//
void pro_read_engine_start(uint8_t *recvbuf,uint16_t number)
{
	
	int result;
	if(number>0x03)
	{
		result = (recvbuf[0x02]*0x100+ recvbuf[0x03]);
	}
	g_datacollection.engine_start = result;
}


//氧传感器电流(组1-传感器1)
void pro_read_frontocurrent1(uint8_t *recvbuf,uint16_t number)
{
	double result=0;
	char tmp_buf[20] = {0};
	if(number>0x04)
	{
		if(m_diagdatapid[0x13] == 0xFF)
		{
			if(recvbuf[04]&0x80)
			{
				result =(0x7FFF - (recvbuf[0x04]*0x100+recvbuf[0x05]))*128.0/32768.0;
			}
			else
			{
				result = (recvbuf[0x04]*0x100+recvbuf[0x05])*128.0/32768.0 ;
			}
		}
	}
	sprintf(tmp_buf,"%d:%.2f,",1,result);
	strcat(g_datacollection.frontOCurrent,tmp_buf);
//	g_datacollection.frontOCurrent[0] = result;
}

//氧传感器电流(组2-传感器1)
void pro_read_frontocurrent2(uint8_t *recvbuf,uint16_t number)
{
	double result=0;
	char tmp_buf[20] = {0};
	if(number>0x04)
	{
		if(m_diagdatapid[0x13] == 0xFF)
		{
			if(recvbuf[04]&0x80)
			{
				result =(0x7FFF - (recvbuf[0x04]*0x100+recvbuf[0x05]))*128.0/32768.0;
			}
			else
			{
				result = (recvbuf[0x04]*0x100+recvbuf[0x05])*128.0/32768.0 ;
			}
		}
	}
	//sprintf(tmp_buf,"%.2f,",result);
	sprintf(tmp_buf,"%d:%.2f,",2,result);
	
	strcat(g_datacollection.frontOCurrent,tmp_buf);
//	g_datacollection.frontOCurrent[1] = result;
}

//氧传感器电流(组1-传感器1)
void pro_read_frontocurrent3(uint8_t *recvbuf,uint16_t number)
{
	double result=0;
	char tmp_buf[20] = {0};
	if(number>0x04)
	{
		if(m_diagdatapid[0x1D] == 0xFF)
		{
			if(recvbuf[04]&0x80)
			{
				result =(0x7FFF - (recvbuf[0x04]*0x100+recvbuf[0x05]))*128/32768;
			}
			else
			{
				result = (recvbuf[0x04]*0x100+recvbuf[0x05])*128/32768 ;
			}
		}
	}
	sprintf(tmp_buf,"%d:%.2f,",1,result);
	strcat(g_datacollection.frontOCurrent,tmp_buf);
//	g_datacollection.frontOCurrent[2] = result;
}

//氧传感器电流(组2-传感器1)
void pro_read_frontocurrent4(uint8_t *recvbuf,uint16_t number)
{
	double result=0;
	char tmp_buf[20] = {0};
	if(number>0x04)
	{
		if(m_diagdatapid[0x1D] == 0xFF)
		{
			if(recvbuf[04]&0x80)
			{
				result =(0x7FFF - (recvbuf[0x04]*0x100+recvbuf[0x05]))*128.0/32768.0;
			}
			else
			{
				result = (recvbuf[0x04]*0x100+recvbuf[0x05])*128.0/32768.0;
			}
		}
	}
	sprintf(tmp_buf,"%d:%.2f,",2,result);
	strcat(g_datacollection.frontOCurrent,tmp_buf);
//	g_datacollection.frontOCurrent[3] = result;
}

//氧传感器电流(组3-传感器1)
void pro_read_frontocurrent5(uint8_t *recvbuf,uint16_t number)
{
	double result;
	char tmp_buf[20] = {0};
	if(number>0x04)
	{
		if(m_diagdatapid[0x1D] == 0xFF)
		{
			if(recvbuf[04]&0x80)
			{
				result =(0x7FFF - (recvbuf[0x04]*0x100+recvbuf[0x05]))*128.0/32768.0;
			}
			else
			{
				result = (recvbuf[0x04]*0x100+recvbuf[0x05])*128.0/32768.0;
			}
		}
	}
	sprintf(tmp_buf,"%d:%.2f,",3,result);
	strcat(g_datacollection.frontOCurrent,tmp_buf);
//	g_datacollection.frontOCurrent[4] = result;
}

//氧传感器电流(组4-传感器1)
void pro_read_frontocurrent6(uint8_t *recvbuf,uint16_t number)
{
	double result=0;
	char tmp_buf[20] = {0};
	if(number>0x04)
	{
		if(m_diagdatapid[0x1D] == 0xFF)
		{
			if(recvbuf[04]&0x80)
			{
				result =(0x7FFF - (recvbuf[0x04]*0x100+recvbuf[0x05]))*128.0/32768.0;
			}
			else
			{
				result = (recvbuf[0x04]*0x100+recvbuf[0x05])*128.0/32768.0;
			}
		}
	}
	sprintf(tmp_buf,"%d:%.2f",4,result);
	strcat(g_datacollection.frontOCurrent,tmp_buf);
//	g_datacollection.frontOCurrent[5] = result;
}


void str_deal(char *buf)
{
		int i,len;
	
		len = strlen(buf);
		for(i=0;i<len;i++)
		{
				if(buf[i] >= 0x80)
					buf[i] = 32;
		}
}


//读VIN
void pro_read_vin(uint8_t *recvbuf,uint16_t number,uint8_t pos)
{
	uint8_t result[32];
	memset(result,0,sizeof(result));
	if(pos ==4)
	{
		//特殊处理
		if(((recvbuf[1]) !=0 )&&((recvbuf[2])!=0)&&((recvbuf[3])!=0))
		{
			pos = 1;
		}
	}
	memcpy(result,recvbuf+pos,17);
	str_deal(result);
	memcpy(g_datacollection.vin_sn,result,17);
	memcpy(g_vehiclediagnosie.vin_sn,result,17);
	printfUartObdiideBug("VIN==  %s\r\n",result);
}	
	
//CALID
void pro_read_calid(uint8_t *recvbuf,uint16_t number)
{
	uint16_t  numcalid =0;
	uint8_t result[128];
	uint8_t temp[32];
	int i=0,j=0;
	uint8_t pos =3;
	
	printfUartObdiideBug("welcome to [%s]\r\n",__func__);
	memset(result,0,sizeof(result));
	numcalid = (number-pos)/16;
	for(i =0;i<numcalid;i++)
	{
		memset(temp,0,sizeof(temp));
		memcpy(temp,recvbuf+(pos + numcalid*i),16);
		if(i < (numcalid-1))
		{
			strcat(result,"-");
		}
		strcat(result,temp);
	}
//	memcpy(g_vehiclediagnosie.software_identification_code,result,20);
	str_deal(result);
	memcpy(g_datacollection.software_identification_code,result,20);
//	str_deal(g_datacollection.software_identification_code);
	
	printfUartObdiideBug("CALID==  %s\r\n",result);
}

//CVN
void pro_read_cvn(uint8_t *recvbuf,uint16_t number,uint8_t pos)
{
	char temp[32];
	
	uint16_t  numcvn =0;
	uint8_t result[200]={0};
	int i=0;
	
	numcvn = (number-pos)/4;
	printfUartObdiideBug("numcvn==  %d\r\n",numcvn);
	for(i =0;i<numcvn;i++)
	{
		sprintf(temp,"%02X%02X%02X%02X",recvbuf[pos+4*i],recvbuf[pos+4*i+1],recvbuf[pos+4*i+2],recvbuf[pos+4*i+3]);
		if(i < (numcvn-1))
		{
			strcat(result,"-");
		}
		strcat(result,temp);
	}
//	memcpy(g_vehiclediagnosie.cvn,result,20);
	str_deal(result);
	//超过ASCII码值会导致数据发送到服务器失败
	memcpy(g_datacollection.cvn,result,20);
	printfUartObdiideBug("CVN==  %s\r\n",result);
}

//氧传感器电压 组1-传感器1
void pro_read_frontovoltage1(uint8_t *recvbuf,uint16_t number)
{
	double result=0;
	char tmp_buf[20] = {0};
	if(number>0x04)
	{
		if(m_diagdatapid[0x13] == 0xFF)
		{
			result = (recvbuf[0x04]*0x100+recvbuf[0x05])*8.0/65535.0;
		}
	}
	sprintf(tmp_buf,"%d:%.2f,",1,result);
	strcat(g_datacollection.frontOVoltage,tmp_buf);
//	g_datacollection.frontOVoltage[0] = result;
}

//氧传感器电压 组2-传感器1
void pro_read_frontovoltage2(uint8_t *recvbuf,uint16_t number)
{
	double result=0;
	char tmp_buf[20] = {0};
	if(number>0x04)
	{
		if(m_diagdatapid[0x13] == 0xFF)
		{
			result = (recvbuf[0x04]*0x100+recvbuf[0x05])*8.0/65535.0;
		}
	}
	sprintf(tmp_buf,"%d:%.2f,",2,result);
	strcat(g_datacollection.frontOVoltage,tmp_buf);
//	g_datacollection.frontOVoltage[1] = result;
}

//氧传感器电压 组1-传感器1
void pro_read_frontovoltage3(uint8_t *recvbuf,uint16_t number)
{
	double result;
	char tmp_buf[20] = {0};
	if(number>0x04)
	{
		if(m_diagdatapid[0x1d] == 0xFF)
		{
			result = (recvbuf[0x04]*0x100+recvbuf[0x05])*8.0/65535.0;
		}
	}
	
	sprintf(tmp_buf,"%d:%.2f,",1,result);
	strcat(g_datacollection.frontOVoltage,tmp_buf);
//	g_datacollection.frontOVoltage[2] = result;
}

//氧传感器电压 组2-传感器1
void pro_read_frontovoltage4(uint8_t *recvbuf,uint16_t number)
{
	double result=0;
	char tmp_buf[20] = {0};
	if(number>0x04)
	{
		if(m_diagdatapid[0x1d] == 0xFF)
		{
			result = (recvbuf[0x04]*0x100+recvbuf[0x05])*8.0/65535.0;
		}
	}

	sprintf(tmp_buf,"%d:%.2f,",2,result);
	strcat(g_datacollection.frontOVoltage,tmp_buf);
//	g_datacollection.frontOVoltage[3] = result;
}

//氧传感器电压 组3-传感器1
void pro_read_frontovoltage5(uint8_t *recvbuf,uint16_t number)
{
	double result=0;
	char tmp_buf[20] = {0};
	if(number>0x04)
	{
		if(m_diagdatapid[0x1d] == 0xFF)
		{
			result = (recvbuf[0x04]*0x100+recvbuf[0x05])*8.0/65535.0;
		}
	}
	sprintf(tmp_buf,"%d:%.2f,",3,result);
	strcat(g_datacollection.frontOVoltage,tmp_buf);
//	g_datacollection.frontOVoltage[4] = result;
}

//氧传感器电压 组4-传感器1
void pro_read_frontovoltage6(uint8_t *recvbuf,uint16_t number)
{
	double result;
	char tmp_buf[20] = {0};
	if(number>0x04)
	{
		if(m_diagdatapid[0x1d] == 0xFF)
		{
			result = (recvbuf[0x04]*0x100+recvbuf[0x05])*8.0/65535.0;
		}
	}

	sprintf(tmp_buf,"%d:%.2f",4,result);
	strcat(g_datacollection.frontOVoltage,tmp_buf);
//	g_datacollection.frontOVoltage[5] = result;
}


//后氧传感器电流(组1-传感器2)
void pro_read_afterocurrent1(uint8_t *recvbuf,uint16_t number)
{
	uint16_t result=0;
	char tmp_buf[20] = {0};
	if(number>0x04)
	{
		if(m_diagdatapid[0x13] == 0xFF)
		{
			if(recvbuf[04]&0x80)
			{
				result =(0x7FFF - (recvbuf[0x04]*0x100+recvbuf[0x05]))*128/32768;
			}
			else
			{
				result = (recvbuf[0x04]*0x100+recvbuf[0x05])*128/32768;
			}
		}
	}

	sprintf(tmp_buf,"%d:%.2f,",1,result);
	strcat(g_datacollection.backOCurrent,tmp_buf);
//	g_datacollection.backOCurrent[0] = result;
}

//氧传感器电流(组2-传感器2)
void pro_read_afterocurrent2(uint8_t *recvbuf,uint16_t number)
{
	uint16_t result=0;
	char tmp_buf[20] = {0};
	if(number>0x04)
	{
		if(m_diagdatapid[0x13] == 0xFF)
		{
			if(recvbuf[04]&0x80)
			{
				result =(0x7FFF - (recvbuf[0x04]*0x100+recvbuf[0x05]))*128/32768;
			}
			else
			{
				result = (recvbuf[0x04]*0x100+recvbuf[0x05])*128/32768 ;
			}
		}
	}
	sprintf(tmp_buf,"%d:%.2f,",2,result);
	strcat(g_datacollection.backOCurrent,tmp_buf);
//	g_datacollection.backOCurrent[1] = result;
}


//氧传感器电流(组1-传感器2)
void pro_read_afterocurrent3(uint8_t *recvbuf,uint16_t number)
{
	uint16_t result=0;
	char tmp_buf[20] = {0};
	if(number>0x04)
	{
		if(m_diagdatapid[0x1d] == 0xFF)
		{
			if(recvbuf[04]&0x80)
			{
				result =(0x7FFF - (recvbuf[0x04]*0x100+recvbuf[0x05]))*128/32768;
			}
			else
			{
				result = (recvbuf[0x04]*0x100+recvbuf[0x05])*128/32768 ;
			}
		}
	}

	sprintf(tmp_buf,"%d:%.2f,",1,result);
	strcat(g_datacollection.backOCurrent,tmp_buf);
//	g_datacollection.backOCurrent[2] = result;
}

//氧传感器电流(组2-传感器2)
void pro_read_afterocurrent4(uint8_t *recvbuf,uint16_t number)
{
	uint16_t result=0;
	char tmp_buf[20] = {0};
	if(number>0x04)
	{
		if(m_diagdatapid[0x1d] == 0xFF)
		{
			if(recvbuf[04]&0x80)
			{
				result =(0x7FFF - (recvbuf[0x04]*0x100+recvbuf[0x05]))*128/32768;
			}
			else
			{
				result = (recvbuf[0x04]*0x100+recvbuf[0x05])*128/32768 ;
			}
		}
	}

	sprintf(tmp_buf,"%d:%.2f,",2,result);
	strcat(g_datacollection.backOCurrent,tmp_buf);
//	g_datacollection.backOCurrent[3] = result;
}

//氧传感器电流(组3-传感器2)
void pro_read_afterocurrent5(uint8_t *recvbuf,uint16_t number)
{
	uint16_t result=0;
	char tmp_buf[20] = {0};
	if(number>0x04)
	{
		if(m_diagdatapid[0x1d] == 0xFF)
		{
			if(recvbuf[04]&0x80)
			{
				result =(0x7FFF - (recvbuf[0x04]*0x100+recvbuf[0x05]))*128/32768;
			}
			else
			{
				result = (recvbuf[0x04]*0x100+recvbuf[0x05])*128/32768 ;
			}
		}
	}
	sprintf(tmp_buf,"%d:%.2f,",3,result);
	strcat(g_datacollection.backOCurrent,tmp_buf);
//	g_datacollection.backOCurrent[4] = result;
}


//氧传感器电流(组4-传感器2)
void pro_read_afterocurrent6(uint8_t *recvbuf,uint16_t number)
{
	uint16_t result=0;
	char tmp_buf[20] = {0};
	if(number>0x04)
	{
		if(m_diagdatapid[0x1d] == 0xFF)
		{
			if(recvbuf[04]&0x80)
			{
				result =(0x7FFF - (recvbuf[0x04]*0x100+recvbuf[0x05]))*128/32768;
			}
			else
			{
				result = (recvbuf[0x04]*0x100+recvbuf[0x05])*128/32768 ;
			}
		}
	}
	sprintf(tmp_buf,"%d:%.2f",4,result);
	strcat(g_datacollection.backOCurrent,tmp_buf);
//	g_datacollection.backOCurrent[5] = result;
}


//氧传感器电压 组1-传感器2
void pro_read_afterovoltage1(uint8_t *recvbuf,uint16_t number)
{
	double result=0;
	char tmp_buf[20] = {0};
	if(number>0x04)
	{
		if(m_diagdatapid[0x13] == 0xFF)
		{
			result = (recvbuf[0x04]*0x100+recvbuf[0x05])*8/65535;
		}
	}
	sprintf(tmp_buf,"%d:%.2f,",1,result);
	strcat(g_datacollection.backOVoltage,tmp_buf);
//	g_datacollection.backOVoltage[0] = result;
}

//氧传感器电压 组2-传感器2
void pro_read_afterovoltage2(uint8_t *recvbuf,uint16_t number)
{
	double result=0;
	char tmp_buf[20] = {0};
	if(number>0x04)
	{
		if(m_diagdatapid[0x13] == 0xFF)
		{
			result = (recvbuf[0x04]*0x100+recvbuf[0x05])*8/65535;
		}
	}

	sprintf(tmp_buf,"%d:%.2f,",2,result);
	strcat(g_datacollection.backOVoltage,tmp_buf);
//	g_datacollection.backOVoltage[1] = result;
}

//氧传感器电压 组1-传感器2
void pro_read_afterovoltage3(uint8_t *recvbuf,uint16_t number)
{
	double result=0;
	char tmp_buf[20] = {0};
	if(number>0x04)
	{
		if(m_diagdatapid[0x1d] == 0xFF)
		{
			result = (recvbuf[0x04]*0x100+recvbuf[0x05])*8/65535;
		}
	}

	sprintf(tmp_buf,"%d:%.2f,",1,result);
	strcat(g_datacollection.backOVoltage,tmp_buf);
//	g_datacollection.backOVoltage[2] = result;
}

//氧传感器电压 组2-传感器2
void pro_read_afterovoltage4(uint8_t *recvbuf,uint16_t number)
{
	double result=0;
	char tmp_buf[20] = {0};
	if(number>0x04)
	{
		if(m_diagdatapid[0x1d] == 0xFF)
		{
			result = (recvbuf[0x04]*0x100+recvbuf[0x05])*8/65535;
		}
	}

	sprintf(tmp_buf,"%d:%.2f,",1,result);
	strcat(g_datacollection.backOVoltage,tmp_buf);
//	g_datacollection.backOVoltage[3] = result;
}

//氧传感器电压 组3-传感器2
void pro_read_afterovoltage5(uint8_t *recvbuf,uint16_t number)
{
	double result=0;
	char tmp_buf[20] = {0};
	if(number>0x04)
	{
		if(m_diagdatapid[0x1d] == 0xFF)
		{
			result = (recvbuf[0x04]*0x100+recvbuf[0x05])*8/65535;
		}
	}

	sprintf(tmp_buf,"%d:%.2f,",1,result);
	strcat(g_datacollection.backOVoltage,tmp_buf);
//	g_datacollection.backOVoltage[4] = result;
}

//氧传感器电压 组4-传感器2
void pro_read_afterovoltage6(uint8_t *recvbuf,uint16_t number)
{
	double result=0;
	char tmp_buf[20] = {0};
	if(number>0x04)
	{
		if(m_diagdatapid[0x1d] == 0xFF)
		{
			result = (recvbuf[0x04]*0x100+recvbuf[0x05])*8/65535;
		}
	}

	sprintf(tmp_buf,"%d:%.2f",1,result);
	strcat(g_datacollection.backOVoltage,tmp_buf);
//	g_datacollection.backOVoltage[5] = result;
}

//发动机负载
void pro_read_engineload(uint8_t *recvbuf,uint16_t number)
{
	double result=0;
	if(number>0x02)
	{
		result = recvbuf[0x02]*100/255.0;
	}
	g_datacollection.engineLoad = result;
}

//进气歧管压力
void pro_read_airinpressure(uint8_t *recvbuf,uint16_t number)
{
	double result=0;
	if(number>0x02)
	{
		result = recvbuf[0x02];
	}
	g_datacollection.airInPressure = result;
}

//进气温度
void pro_read_airintemperature(uint8_t *recvbuf,uint16_t number)
{
	double result =0;
	if(number >0x02)
	{
		result = recvbuf[0x02]-40;
	}
	g_datacollection.airInTemperature = result;
}

//


void pro_get_result(OBDII_STREAM_CMD_INDEX index,uint8_t *recvbuf,uint16_t number,uint8_t pos,char *result)
{
	
	uint16_t  numcvn =0;
	uint8_t i =0;
	char temp[64];
	char str[32];
	
	memset(result,0,sizeof(result));
	memset(str,0,32);
	memset(temp,0,sizeof(temp));
	
	switch(index)
	{
		case OBDII_READ_VIN:
			pro_read_vin(recvbuf,number,pos);
			break;
		
		case OBDII_READ_CALID:
			pro_read_calid(recvbuf,number);
			break;
		
		case OBDII_READ_CVN:
			pro_read_cvn(recvbuf,number,pos);
			break;
		
//		case OBDII_READ_IUPR1:
//			pro_read_iupr1(recvbuf,number);
//			break;
		
//		case OBDII_READ_IUPR2:
//			pro_read_iupr2(recvbuf,number);
//			break;
		
//		case OBDII_READ_READY:
//			pro_read_ready(recvbuf,number);
//			break;
		case OBDII_READ_KILOMETRE:
			pro_read_kilometet(recvbuf,number);
			break;
		
		case OBDII_READ_SPEED:
			pro_read_speed(recvbuf,number);
			break;
		
		case OBDII_READ_MAX_STORQUE:
			pro_read_max_storque(recvbuf,number);
			break;
		
		case OBDII_READ_OUT_STORQUE:
			pro_read_out_storque(recvbuf,number);
			break;
		
		case OBDII_READ_FRICTION_STORQUE:
			pro_read_friction_storque(recvbuf,number);
			break;
		
		case OBDII_READ_ENGINE_SPEED:
			pro_read_engine_speed(recvbuf,number);
			break;
		
		case OBDII_READ_ENGINE_FLOW:
			pro_read_engine_flow(recvbuf,number);
			break;
		
		case OBDII_READ_IN_TEMPERATURE:
			pro_read_in_temperature(recvbuf,number);
			break;
		
		case  OBDII_READ_OUT_TEMPERATURE:
			pro_read_out_temperature(recvbuf,number);
			break;
		
		case OBDII_READ_PRESSURE_DIFF1:
			pro_read_pressure_diff1(recvbuf,number);
			break;
		
		case OBDII_READ_PRESSURE_DIFF2:
			pro_read_pressure_diff2(recvbuf,number);
			break;
		
		case OBDII_READ_INTAKE_VOLUME:
			pro_read_intrake(recvbuf,number);
			break;
		
		case OBDII_READ_REACTANT_SUIPLUS:
			pro_read_reactant_suiplus(recvbuf,number);
			break;
		
		case OBDII_READ_TANK_LEVEL:
			pro_read_tank_leverl(recvbuf,number);
			break;
		
		case OBDII_READ_COOLANT_TEMPERATURE:
			pro_read_coolant_temperature1(recvbuf,number);
			break;
		
//		case OBDII_READ_COOLANT_TEMPERATURE2:
//			pro_read_coolant_temperature2(recvbuf,number);
//			break;
		
		case OBDII_READ_FUEL_INJECTION:
			pro_read_fuel_injection(recvbuf,number);
			break;
		case OBDII_READ_VOLTAGE:
			pro_read_voltage(recvbuf,number);
			break;
		
		case OBDII_READ_ENGINE_START:
			pro_read_engine_start(recvbuf,number);
			break;
		
		case OBDII_READ_FRONTOCURRENT1:
			pro_read_frontocurrent1(recvbuf,number);
			break;
		
		case OBDII_READ_FRONTOCURRENT2:
			pro_read_frontocurrent2(recvbuf,number);
			break;
		
		case OBDII_READ_FRONTOCURRENT3:
			pro_read_frontocurrent3(recvbuf,number);
			break;
		
		case OBDII_READ_FRONTOCURRENT4:
			pro_read_frontocurrent4(recvbuf,number);
			break;
		
		case OBDII_READ_FRONTOCURRENT5:
			pro_read_frontocurrent5(recvbuf,number);
			break;
		
		case OBDII_READ_FRONTOCURRENT6:
			pro_read_frontocurrent6(recvbuf,number);
			break;
		
		case OBDII_READ_FRONTOVOLTAGE1:
			pro_read_frontovoltage1(recvbuf,number);
			break;
		
		case OBDII_READ_FRONTOVOLTAGE2:
			pro_read_frontovoltage2(recvbuf,number);
			break;
		
		case OBDII_READ_FRONTOVOLTAGE3:
			pro_read_frontovoltage3(recvbuf,number);
			break;
		
		case OBDII_READ_FRONTOVOLTAGE4:
			pro_read_frontovoltage4(recvbuf,number);
			break;
		
		case OBDII_READ_FRONTOVOLTAGE5:
			pro_read_frontovoltage5(recvbuf,number);
			break;
		
		case OBDII_READ_FRONTOVOLTAGE6:
			pro_read_frontovoltage6(recvbuf,number);
			break;
		
		case OBDII_READ_AFTEROCURRENT1:
			pro_read_afterocurrent1(recvbuf,number);
			break;
		
		case OBDII_READ_AFTEROCURRENT2:
			pro_read_afterocurrent2(recvbuf,number);
			break;
		
		case OBDII_READ_AFTEROCURRENT3:
			pro_read_afterocurrent3(recvbuf,number);
			break;
		
		case OBDII_READ_AFTEROCURRENT4:
			pro_read_afterocurrent4(recvbuf,number);
			break;
		
		case OBDII_READ_AFTEROCURRENT5:
			pro_read_afterocurrent5(recvbuf,number);
			break;
		
		case OBDII_READ_AFTEROCURRENT6:
			pro_read_afterocurrent6(recvbuf,number);
			break;
		
		case OBDII_READ_AFTEROVOLTAGE1:
			pro_read_afterovoltage1(recvbuf,number);
			break;
		
		case OBDII_READ_AFTEROVOLTAGE2:
			pro_read_afterovoltage2(recvbuf,number);
			break;
		
		case OBDII_READ_AFTEROVOLTAGE3:
			pro_read_afterovoltage3(recvbuf,number);
			break;
		
		case OBDII_READ_AFTEROVOLTAGE4:
			pro_read_afterovoltage4(recvbuf,number);
			break;
		
		case OBDII_READ_AFTEROVOLTAGE5:
			pro_read_afterovoltage5(recvbuf,number);
			break;
		
		case OBDII_READ_AFTEROVOLTAGE6:
			pro_read_afterovoltage6(recvbuf,number);
			break;
		
		case OBDII_READ_ENGINELOAD:
			pro_read_engineload(recvbuf,number);
			break;
		
		case OBDII_READ_AIRINPRESSURE:
			pro_read_airinpressure(recvbuf,number);
			break;
		
		case OBDII_READ_AIRINTEMPERATURE:
			pro_read_airintemperature(recvbuf,number);
			break;
		
		default:
			break;
	}
//	strcpy(result,temp);
}

void pro_read_data(void)
{
	
	uint8_t cmdtnum =0;
	uint8_t pid =0;
	uint8_t i =0;
	uint16_t number =0;
	uint8_t recvbuf[256];
	uint8_t sendbuf[64];
	char result[256];
	uint8_t pos =0x02;
	
	cmdtnum = sizeof(ReadDataCmd)/sizeof(OBDII_STREAM_CMD);
	
	memset(g_datacollection.frontOCurrent,0,sizeof(g_datacollection.frontOCurrent));
	memset(g_datacollection.frontOVoltage,0,sizeof(g_datacollection.frontOVoltage));
	memset(g_datacollection.backOCurrent,0,sizeof(g_datacollection.backOCurrent));
	memset(g_datacollection.backOVoltage,0,sizeof(g_datacollection.backOVoltage));
	for(i=0;i< cmdtnum;i++)
	{
		pid = ReadDataCmd[i].CMD_DATA[2];
		if(m_diagdatapid[pid] == 0xff)
		{
			printfUartObdiideBug("pid == %02X\r\n",pid);
			memset(sendbuf,0,sizeof(sendbuf));
			memcpy(sendbuf,ReadDataCmd[i].CMD_DATA,(ReadDataCmd[i].CMD_DATA[0]+1));
			number = pro_SendRecv(sendbuf, recvbuf);
			if(number >0)
			{
				pro_get_result(ReadDataCmd[i].Cmd_Index,recvbuf,number,pos,result);
			}
		}
	}
}

void get_ready_status(char* buf)
{
	uint8_t pid =0;
	uint16_t number =0;
	uint8_t recvbuf[128];
	uint8_t sendbuf[10];

	pid = ReadDataCmd[0].CMD_DATA[2];
	
	memset(sendbuf,0,sizeof(sendbuf));
	memcpy(sendbuf,ReadDataCmd[0].CMD_DATA,(ReadDataCmd[0].CMD_DATA[0]+1));
	number = pro_SendRecv(sendbuf, recvbuf);
	if(number >0)
	{
		pro_read_ready(recvbuf,number,buf);
	}
}

void get_vehicle_iupr(OBDII_INIT_MODE index,char* buf)
{
	uint8_t sendbuf2[3] ={0x02,0x09,0x00};
	uint8_t cmdtnum =0;
	uint8_t i =0;
	uint8_t mid =0;
	uint16_t number =0;
	uint8_t sendbuf[64];
	uint8_t recvbuf[256];
	uint8_t msgnum = 0;
	
	cmdtnum = sizeof(ReadIuprCmd)/sizeof(ReadIuprCmd);
	
	for(i=0;i< cmdtnum;i++)
	{
		mid = ReadIuprCmd[i].CMD_DATA[2];
		
		if(m_infotype[mid] == 0xff)
		{
			printfUartObdiideBug("mid == %d\r\n",mid);
			memset(sendbuf,0,sizeof(sendbuf));
			memcpy(sendbuf,ReadIuprCmd[i].CMD_DATA,(ReadIuprCmd[i].CMD_DATA[0]+1));
			
			if(mid==0x08)
			{
				switch(index)
				{
					 case OBDII_MODE_01:
					 case OBDII_MODE_02:
					 case OBDII_MODE_03:
					 case OBDII_MODE_04:
					 number = pro_SendRecv(sendbuf, recvbuf);
					 break;
					 case OBDII_MODE_05:
					 case OBDII_MODE_06:
					 case OBDII_MODE_07:
					 case OBDII_MODE_08:
					 case OBDII_MODE_09:
						sendbuf2[2] = mid-0x01;
						number = pro_SendRecv(sendbuf2, recvbuf);
					  if(number==0) 
						{
							break;
						}
						if(number >2)
						{
							msgnum = recvbuf[2];
						}
						printfUartObdiideBug("msgnum == %d\r\n",msgnum);
						number = pro_SendRecv_OBDII_MORE(sendbuf,recvbuf, msgnum); 
					 break;
						
					 default:
					 break;
				}
				
				pro_read_iupr1(recvbuf,number,buf);
			}
			else
			{
				 number = pro_SendRecv(sendbuf, recvbuf);
				 pro_read_iupr2(recvbuf,number,buf);
			}
		}
		
		
	}

}

void pro_read_info(OBDII_INIT_MODE index)
{
	uint8_t cmdtnum =0;
	uint8_t mid =0;
	uint8_t i =0;
	uint16_t number =0;
	uint8_t recvbuf[256];
	uint8_t sendbuf[64];
	char result[256];
	uint8_t sendbuf2[3] ={0x02,0x09,0x00};
	uint8_t msgnum = 0;
	uint8_t pos = 0;
	memset(recvbuf,0,sizeof(recvbuf));
	cmdtnum = sizeof(ReadInfoCmd)/sizeof(OBDII_STREAM_CMD);
	printfUartObdiideBug("hello tom.\r\n");
	for(i=0;i< cmdtnum;i++)
	{
		mid = ReadInfoCmd[i].CMD_DATA[2];
		if(m_infotype[mid] == 0xff)
		{
			printfUartObdiideBug("mid == %d\r\n",mid);
			memset(sendbuf,0,sizeof(sendbuf));
			memcpy(sendbuf,ReadInfoCmd[i].CMD_DATA,(ReadInfoCmd[i].CMD_DATA[0]+1));
			
			if( (mid==0x02)||(mid==0x04)||(mid==0x06))
			{
				switch(index)
				{
					case OBDII_MODE_01:
					case OBDII_MODE_02:
					case OBDII_MODE_03:
					case OBDII_MODE_04:
						pos =0x03;
						number = pro_SendRecv(sendbuf, recvbuf);
						break;
					case OBDII_MODE_05:
					case OBDII_MODE_06:
					case OBDII_MODE_07:
					case OBDII_MODE_08:
					case OBDII_MODE_09:
						sendbuf2[2] = mid-0x01;
						number = pro_SendRecv(sendbuf2, recvbuf);
					  if(number==0) 
						{
							break;
						}
						if(number >2)
						{
							msgnum = recvbuf[2];
						}
						printfUartObdiideBug("msgnum == %d\r\n",msgnum);
						number = pro_SendRecv_OBDII_MORE(sendbuf,recvbuf, msgnum);
						if(mid==0x02)
						{
							pos =0x04;
						}
						if(mid==0x04)
						{
							pos = 0x01;
						}
						if(mid==0x06)
						{
							pos = 0x01;
						}
						break;
					default:
						break;
				}
			}
			else
			{
				pos = 0x03;
				number = pro_SendRecv(sendbuf, recvbuf);
			}
			printfUartObdiideBug("number==  %d\r\n",number);
			if(number >0)
			{
				pro_get_result(ReadInfoCmd[i].Cmd_Index,recvbuf,number,pos,result);
			}
		}
	}
	printfUartObdiideBug("hello tom188.\r\n");
}

void SetCanbusFilter(uint32_t filterID)
{
	int i;
	int number;
	uint8_t buffer[64];
	number = 0;
	buffer[number++] = 0xa5;
	buffer[number++] = 0xa5;
	buffer[number++] = 0x00;
	buffer[number++] = 0x05;
	buffer[number++] = 0x70;
	buffer[number++] = (uint8_t)(filterID>>24);
	buffer[number++] = (uint8_t)(filterID>>16);
	buffer[number++] = (uint8_t)(filterID>>8);
	buffer[number++] = (uint8_t)(filterID&0xff);
	buffer[number++] = 0x00;
	for(i=2;i<number-1;i++)
	{
		buffer[number-1] += buffer[i];
	}
	buffer[number-1] = ~buffer[number-1];
	USB_BULK_Queue_Write(&USB_RX_QueueData,buffer,number);
	vTaskDelay(40);	
}

void cmd_msg_all_canbus_11bit()
{
	uint32_t canread, canflow;
	
	canread = m_modulebuf[0];
	canflow = canread-8;
	
	if(g_defaultMsgAttr.m_PCK_TYPE== 0x22)
	{
		//m_canbussendid=getHex32(ezxml_attr(msgxml, "CANSEND"));
		m_canbusreadid=canread;
		m_canbusflowid=canflow;
	}
	SetCanbusFilter(canread);
}

void cmd_msg_all_canbus_29bit()
{
	
	uint32_t canread, canflow;
	
	canread = m_modulebuf[0];
	canflow = (canread&0xffff0000) + ((canread&0xff00)>>8) + ((canread&0xff)<<8);
	
	if(g_defaultMsgAttr.m_PCK_TYPE= 0x22)
	{
		//m_canbussendid=getHex32(ezxml_attr(msgxml, "CANSEND"));
		m_canbusreadid=canread;
		m_canbusflowid=canflow;
	}
	SetCanbusFilter(canread);
	
}

//uint8_t cmd_set_ecumodule(uint8_t module)
//{
//	m_setmodule = module;
//	return m_setmodule;
//}


void cmd_msg_all_default_model()
{
	cmd_set_ecumodule(m_modulebuf[0]);
}


//重置全局变量
void reset_global_data()
{
	m_refuseretry=10;
	m_canbussendid=0x000007df;/*系统默认发送ID*/
	m_canbusflowid=0x000007e0;/*当前系统发送ID*/
	m_canbusreadid=0x000007e8;/*当前系统接收ID*/
	m_canbusflowidchang=0;/*需要改变的发送ID*/
	m_canbusreadidchang=0;/*需要改变的接收ID*/
	cmd_set_modulenum(0);
	m_protocol_index = -1;
	
	memset(m_infotype,0,sizeof(m_infotype));
	memset(m_diagdatapid,0,sizeof(m_diagdatapid));
}

void pro_obdii_protocol_init(OBDII_INIT_MODE model)
{
	switch(model)
	{
		case OBDII_MODE_01:
		case OBDII_MODE_03:
			cmd_msg_all_canbus_11bit();
			break;
		case OBDII_MODE_02:
		case OBDII_MODE_04:
			cmd_msg_all_canbus_29bit();
			break;
		
		case OBDII_MODE_05:
		case OBDII_MODE_06:
		case OBDII_MODE_07:
		case OBDII_MODE_08:
		case OBDII_MODE_09:
			cmd_msg_all_default_model();
			break;
		
		default:
			break;
	}
}


bool  vehicle_select(void)
{
		bool vehicle_flag = false;
	
#ifdef  M_MODE_CAR_TOYOTA  		
				
		vehicle_flag = Much_Init_Toyota();
#endif
			
#ifdef  M_MODE_CAR_NISSAN  
			
		vehicle_flag = Much_Init_NISSAN();
#endif			
			
#ifdef  M_MODE_CAR_HONDA
			
		vehicle_flag = Much_Init_Honda();
#endif		

#ifdef  M_MODE_CAR_FORD
			
		vehicle_flag = Much_Init_FORD();
#endif

#ifdef  M_MODE_CAR_VW  
			
		vehicle_flag = Much_Init_VW();
#endif

#ifdef  M_MODE_CAR_HYUNDAI  
			
		vehicle_flag = Much_Init_Hyundai();
#endif	

#ifdef  M_MODE_CAR_GMCN  
			
		vehicle_flag = Much_Init_Gmcn();
#endif
		
#ifdef  M_MODE_CAR_GREATWALL  
			
		vehicle_flag = Much_Init_GREATWALL();
#endif		
		
		return vehicle_flag;
}


void Task_ObdiiCmdProcess_Proc(void)
{
		bool  m_first_time = false;
		uint8_t obd_flag = 0,vehicle_flag = 0;
		
		static uint8_t delay=0;
		 
		printf("welcome to [%s]\r\n",__func__);
//	vTaskSuspend(Task_ObdiiCmdProcess_Proc_handle); 
			
		get_vehicle_info(&tom_vehicle_model_data);
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
				
				if(!m_first_time)
				{
						obd_flag = OBDII_Much_Init();
					
#if defined(APP_APPLACATION)					
						vehicle_flag = vehicle_select();
#endif		
					
						if( (obd_flag == 1) || (vehicle_flag == 1) )
						{
								m_first_time = true;
						}
				}
				else
				{
						while(1)
						{
								vTaskDelay(10);
								
								if(data_flag == 1 && diag_flag == 0)//数据采集
								{
										obd_flag = OBDII_Much_Init();
										if(obd_flag == 1)
										{
												data_flag = 0;
										}
										else
										{
												printf("OBDII_Much_Init failed\r\n");
										}
								}
								
#if defined(APP_APPLACATION)								
								else if(diag_flag == 1 && data_flag == 0)//车辆诊断
								{
										vehicle_flag = vehicle_select();
										if(vehicle_flag == 1)
										{
												diag_flag = 0;
										}
										else
										{
												printf("Much_Init_XXX failed\r\n");
										}
								}
#endif									
								if(!obd_flag && !vehicle_flag)//退出，重新初始化
								{
										m_protocol_index = -1;
										m_first_time = false;
										break;
								}
						}
				}
		}
}


void get_vehicle_vin(char* vin)
{
	memcpy(vin,g_datacollection.vin_sn,20);
}

void get_mil_status(char status)
{
	status = g_vehiclediagnosie.mil_status;
}

void get_vehicle_calid(char* calid)
{
//	memcpy(calid,g_vehiclediagnosie.software_identification_code,20);
	memcpy(calid,g_datacollection.software_identification_code,20);
}

void get_vehicle_cvn(char* cvn)
{
//	memcpy(cvn,g_vehiclediagnosie.cvn,20);
	memcpy(cvn,g_datacollection.cvn,20);
}

void get_datacollection(Data_collection *tom_data_collection)
{
		*tom_data_collection = g_datacollection;
}

void get_dtcnumber(int* dtcnumber)
{
		*dtcnumber = g_vehiclediagnosie.fault_total;
}


void get_dtc_list(char* dtc_list)
{
		memcpy(dtc_list,g_vehiclediagnosie.fault_list,40);
}


void get_agreement(char* index)
{
		switch(m_protocol_index)
		{
				case 0x00:
				case 0x01:
				case 0x02:
				case 0x03:
					*index = 0;   //0:ISO15765
					break;
				
				case 0x04:
				case 0x05:
					*index = 1;   //1:1SO9141
					break;
				
				case 0x06:
					*index = 2;  //1:ISO14230
					break;
				
				case 0x07:
				case 0x08:
					*index = 3;  //J1850
					break;
				
				default:
					*index = -1;
					break;
		}
}


void get_Vehicle_diagnosis(Vehicle_diagnosis *tom_Vehicle_diagnosis)
{
		*tom_Vehicle_diagnosis = g_vehiclediagnosie;
}


//退出系统
void pro_exit_dialog(OBDII_INIT_MODE mode)
{
	uint16_t number=0;
	uint8_t sendbuf[48];
	uint8_t recvbuf[32];
	
	switch(mode)
	{
		case OBDII_MODE_07:
		number = ObdiiEcuExitCmd[mode].CMD_DATA[0];
		memcpy(sendbuf, ObdiiEcuExitCmd[mode].CMD_DATA, number + 1);
		number = pro_SendRecv(sendbuf, recvbuf);

		break;
		
		default:
		break;
	}
}

