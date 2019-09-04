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
#include "STD_HYUNDAI_CMD_PROCESS.h"
#include "FreeRTOS.h"
#include "task.h"
#include "STD_OBD_CMD_PROCESS.h"


uint8_t m_iSystem_Mode_Size_Hyundai[5] = { 3,2,3,3,2 };
//uint8_t m_iSystem_Mode_Size_Hyundai[5] = { 4,3,4,9,2 };//每个系统存在的协议的个数，第一次扫描的时候要用到
intptr_t m_protocol_index_hyundai[5][2] = { {-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1} };   //记录侦测到的协议,5个系统的初始化IO和命令都为-1
bool m_first_hyundai = true;

extern DEFAULT_MSGATTR g_defaultMsgAttr;
extern Vehicle_diagnosis g_vehiclediagnosie;
extern Data_collection g_datacollection;
extern uint8_t m_CAN_ANS_T;
extern uint8_t m_CAN_ANS_R;
//extern uint8_t m_CAN_LISTEN_TYPE;

extern TaskHandle_t  Task_ObdiiCmdProcess_Proc_handle;


//初始化IO命令
const INIT_IO_CMD_HYUNDAI InitIO_Hyundai[] = \
{
	//	HYUNDAI_INIT_MODE Init_Mode;
	//	MSG_ORD_HYUNDAI Msg_Ord;
	//  uint8_t Init_IO[128];
	//	uint8_t enter_size;
	//	ENTER_SYSTEM_HYUNDAI enter_system_hyundai[10];


	{//engine CAN 1 
		{HYUNDAI_INIT_MODE_01},
		{ MSG_Init_CAN_PackType_Hyundai },
		{ 0x2C, 0x01, 0x07, 0xA1, 0x20, 0x02, 0x82, 0x03, 0x00, 0x04, 0x00, 0x00, 0x07, 0xE0, 0x05, 0x00, 0x00, 0x07, 0xE8, 0x06, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x0a, 0x0A, 0x85, 0x00, 0x0B, 0x00, 0x00, 0x0C, 0xe6 },
		{ 0x03 },
		{
			HYUNDAI_ENTER_SYSTEM_MODE_01,
			HYUNDAI_ENTER_SYSTEM_MODE_02,
			HYUNDAI_ENTER_SYSTEM_MODE_03,
		},
	},
	{//engine CAN 2
		{HYUNDAI_INIT_MODE_02},
		{MSG_Init_CAN_PackType_Hyundai},
		{0x2C, 0x01, 0x07, 0xA1, 0x20, 0x02, 0x82, 0x03, 0x00, 0x04, 0x00, 0x00, 0x07, 0xE7, 0x05, 0x00, 0x00, 0x07, 0xEF, 0x06, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x0a, 0x0A, 0x85, 0x00, 0x0B, 0x00, 0x00, 0x0C, 0xe6},
		{0x01},
		{
			HYUNDAI_ENTER_SYSTEM_MODE_04,
		},
	},
	//ECM KWP 
	{
		{HYUNDAI_INIT_MODE_03},
		{MSG_Init_KWP_PackType_Hyundai},
		{0x2B,0x01,0x00,0x28,0xa0,0x02,0x00,0x03,0xc0,0x04,0x77,0x00,0xc1,0xc1,0x05,0x14,0x32,0x37,0x05,0x04,0x00,0x19,0x06,0x00,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x35,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
		{0x07},
		{
			HYUNDAI_ENTER_SYSTEM_MODE_0D,
			HYUNDAI_ENTER_SYSTEM_MODE_0E,
			HYUNDAI_ENTER_SYSTEM_MODE_0F,
			HYUNDAI_ENTER_SYSTEM_MODE_10,
			HYUNDAI_ENTER_SYSTEM_MODE_11,
			HYUNDAI_ENTER_SYSTEM_MODE_12,
			HYUNDAI_ENTER_SYSTEM_MODE_13,
		},
	},
	//ECM 5bps
//	{
//		{HYUNDAI_INIT_MODE_0E},
//		{MSG_Init_KWP_PackType_Hyundai},
//		{0x27,0x01,0x00,0x00,0x00,0x02,0x00,0x03,0x80,0x04,0x77,0xff,0xc1,0xc1,0x05,0x14,0x32,0x37,0x05,0x04,0x00,0x19,0x06,0x00,0x07,0x00,0x08,0x35,0x09,0x03,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x00,0x00,0x00},
//		{0x01},
//		{
//			HYUNDAI_ENTER_SYSTEM_MODE_1F,
//		},
//	},

	{//TCM:TYPE 1
		{HYUNDAI_INIT_MODE_04},
		{MSG_Init_CAN_PackType_Hyundai},
		{0x2C, 0x01, 0x07, 0xA1, 0x20, 0x02, 0x82, 0x03, 0x00, 0x04, 0x00, 0x00, 0x07, 0xE1, 0x05, 0x00, 0x00, 0x07, 0xE9, 0x06, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x0a, 0x0A, 0x85, 0x00, 0x0B, 0x00, 0x00, 0x0C, 0xe6},
		{0x02},
		{
			HYUNDAI_ENTER_SYSTEM_MODE_01,
			HYUNDAI_ENTER_SYSTEM_MODE_0B,
		},
	},
	//AT KWP
	{
		{HYUNDAI_INIT_MODE_05},
		{MSG_Init_KWP_PackType_Hyundai},
		{0x2B,0x01,0x00,0x28,0xa0,0x02,0x00,0x03,0xc0,0x04,0x77,0x00,0xc1,0xc1,0x05,0x14,0x32,0x37,0x05,0x04,0x00,0x19,0x06,0x00,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x35,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
		{0x04},
		{ HYUNDAI_ENTER_SYSTEM_MODE_14,
			HYUNDAI_ENTER_SYSTEM_MODE_15,
			HYUNDAI_ENTER_SYSTEM_MODE_16,
			HYUNDAI_ENTER_SYSTEM_MODE_17,
		},
	},

//	//TCM 5bps
//	{
//		{HYUNDAI_INIT_MODE_0F},
//		{MSG_Init_KWP_PackType_Hyundai},
//		{0x27,0x01,0x00,0x00,0x00,0x02,0x00,0x03,0x80,0x04,0x77,0xff,0xc1,0xc1,0x05,0x14,0x32,0x37,0x05,0x04,0x00,0x19,0x06,0x00,0x07,0x00,0x08,0x35,0x09,0x03,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x00,0x00,0x00},
//		{0x01},
//		{
//			HYUNDAI_ENTER_SYSTEM_MODE_20,
//		},
//	},


	//ABS CAN 1
	{
		{HYUNDAI_INIT_MODE_06},
		{MSG_Init_CAN_PackType_Hyundai},
		{0x2C, 0x01, 0x07, 0xA1, 0x20, 0x02, 0x82, 0x03, 0x10, 0x04, 0x98, 0xda, 0x0B, 0xF9, 0x05, 0x98, 0xda, 0xF9, 0x0B, 0x06, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x0a, 0x0A, 0x85, 0x00, 0x0B, 0x00, 0x00, 0x0C, 0xe6},
		{0x01},
		{
			HYUNDAI_ENTER_SYSTEM_MODE_05,
		},
	},

	//ABS CAN 2
	{
		{HYUNDAI_INIT_MODE_07},
		{MSG_Init_CAN_PackType_Hyundai},
		{0x2C, 0x01, 0x07, 0xA1, 0x20, 0x02, 0x82, 0x03, 0x00, 0x04, 0x00, 0x00, 0x07, 0xD1, 0x05, 0x00, 0x00, 0x07, 0xD9, 0x06, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x0a, 0x0A, 0x85, 0x00, 0x0B, 0x00, 0x00, 0x0C, 0xe6},
		{0x02},
		{
			HYUNDAI_ENTER_SYSTEM_MODE_06,
			HYUNDAI_ENTER_SYSTEM_MODE_07,
		},
	},
	//ABS KWP
	{
		{HYUNDAI_INIT_MODE_08},
		{MSG_Init_KWP_PackType_Hyundai},
		{0x2B,0x01,0x00,0x28,0xa0,0x02,0x00,0x03,0xc0,0x04,0x88,0x00,0xc1,0xc1,0x05,0x14,0x32,0x7F,0x05,0x04,0x00,0x19,0x06,0x00,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x35,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
		{0x04},
		{
			HYUNDAI_ENTER_SYSTEM_MODE_18,
			HYUNDAI_ENTER_SYSTEM_MODE_19,
			HYUNDAI_ENTER_SYSTEM_MODE_1A,
			HYUNDAI_ENTER_SYSTEM_MODE_1B,
		},
	},

//	//ABS 5bps
//	{
//		{HYUNDAI_INIT_MODE_10},
//		{MSG_Init_KWP_PackType_Hyundai},
//		{0x27,0x01,0x00,0x00,0x00,0x02,0x00,0x03,0x80,0x04,0x88,0xff,0xc1,0xc1,0x05,0x14,0x32,0x37,0x05,0x04,0x00,0x19,0x06,0x00,0x07,0x00,0x08,0x35,0x09,0x03,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x00,0x00,0x00},
//		{0x01},
//		{
//			HYUNDAI_ENTER_SYSTEM_MODE_21,
//		},
//	},

	//
	{//SRS type1:
		{HYUNDAI_INIT_MODE_09},
		{MSG_Init_CAN_PackType_Hyundai},
		{0x2C, 0x01, 0x07, 0xA1, 0x20, 0x02, 0x82, 0x03, 0x10, 0x04, 0x98, 0xda, 0x53, 0xF9, 0x05, 0x98, 0xda, 0xF9, 0x53, 0x06, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x0a, 0x0A, 0x85, 0x00, 0x0B, 0x00, 0x00, 0x0C, 0xe6},
		{0x01},
		{
			HYUNDAI_ENTER_SYSTEM_MODE_08,
		},
	},
	{//SRS type2:
		{HYUNDAI_INIT_MODE_0A},
		{MSG_Init_CAN_PackType_Hyundai},
		{0x2C, 0x01, 0x07, 0xA1, 0x20, 0x02, 0x82, 0x03, 0x00, 0x04, 0x00, 0x00, 0x07, 0xD2, 0x05, 0x00, 0x00, 0x07, 0xDA, 0x06, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x0a, 0x0A, 0x85, 0x00, 0x0B, 0x00, 0x00, 0x0C, 0xe6},
		{0x02},
		{ HYUNDAI_ENTER_SYSTEM_MODE_09,
			HYUNDAI_ENTER_SYSTEM_MODE_0A,
		},
	},

	//SRS KWP
	{
		{HYUNDAI_INIT_MODE_0B},
		{MSG_Init_KWP_PackType_Hyundai},
		{0x2B,0x01,0x00,0x28,0xa0,0x02,0x00,0x03,0xc0,0x04,0xcc,0x00,0xc1,0xc1,0x05,0x14,0x32,0x37,0x06,0x04,0x00,0x19,0x06,0x00,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x35,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
		{0x02},
		{
			HYUNDAI_ENTER_SYSTEM_MODE_1C,
			HYUNDAI_ENTER_SYSTEM_MODE_1D,
		},
	},

//	//SRS 5bps
//	{
//		{HYUNDAI_INIT_MODE_11},
//		{MSG_Init_KWP_PackType_Hyundai},
//		{0x27,0x01,0x00,0x00,0x00,0x02,0x00,0x03,0x80,0x04,0xcc,0xff,0xc1,0xc1,0x05,0x04,0x1f,0x37,0x05,0x04,0x00,0x19,0x06,0x00,0x07,0x00,0x08,0x35,0x09,0x03,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x00,0x00,0x00},
//		{0x01},
//		{ HYUNDAI_ENTER_SYSTEM_MODE_22,
//		},
//	},
//	//SRS 5bps
//	{
//		{HYUNDAI_INIT_MODE_12},
//		{MSG_Init_KWP_PackType_Hyundai},
//		{0x27,0x01,0x00,0x00,0x00,0x02,0x00,0x03,0x80,0x04,0xdd,0xff,0xc1,0xc1,0x05,0x04,0x1f,0x37,0x05,0x04,0x00,0x19,0x06,0x00,0x07,0x00,0x08,0x35,0x09,0x03,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x00,0x00,0x00},
//		{0x01},
//		{ HYUNDAI_ENTER_SYSTEM_MODE_22,
//		},
//	},
//	//SRS 5bps
//	{
//		{HYUNDAI_INIT_MODE_13},
//		{MSG_Init_KWP_PackType_Hyundai},
//		{0x27,0x01,0x00,0x00,0x00,0x02,0x00,0x03,0x80,0x04,0xcc,0xff,0xc1,0xc1,0x05,0x14,0x32,0x37,0x05,0x04,0x00,0x19,0x06,0x00,0x07,0x00,0x08,0x35,0x09,0x03,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x00,0x00,0x00},
//		{0x01},
//		{ HYUNDAI_ENTER_SYSTEM_MODE_23,
//		},
//	},
//	//SRS 5bps
//	{
//		{HYUNDAI_INIT_MODE_14},
//		{MSG_Init_KWP_PackType_Hyundai},
//		{0x27,0x01,0x00,0x00,0x00,0x02,0x00,0x03,0x80,0x04,0xdd,0xff,0xc1,0xc1,0x05,0x14,0x32,0x37,0x05,0x04,0x00,0x19,0x06,0x00,0x07,0x00,0x08,0x35,0x09,0x03,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x00,0x00,0x00},
//		{0x01},
//		{ HYUNDAI_ENTER_SYSTEM_MODE_23,
//		},
//	},
//	//SRS 5bps
//	{
//		{HYUNDAI_INIT_MODE_15},
//		{MSG_Init_KWP_PackType_Hyundai},
//		{0x27,0x01,0x00,0x00,0x00,0x02,0x00,0x03,0x80,0x04,0xcc,0xff,0xc1,0xc1,0x05,0x14,0x32,0x37,0x05,0x04,0x00,0x19,0x06,0x00,0x07,0x00,0x08,0x35,0x09,0x03,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x00,0x00,0x00},
//		{0x01},
//		{ HYUNDAI_ENTER_SYSTEM_MODE_24,
//		},
//	},
//	//SRS 5bps
//	{
//		{HYUNDAI_INIT_MODE_16},
//		{MSG_Init_KWP_PackType_Hyundai},
//		{0x27,0x01,0x00,0x00,0x00,0x02,0x00,0x03,0x80,0x04,0xdd,0xff,0xc1,0xc1,0x05,0x14,0x32,0x37,0x05,0x04,0x00,0x19,0x06,0x00,0x07,0x00,0x08,0x35,0x09,0x03,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x00,0x00,0x00},
//		{0x01},
//		{ HYUNDAI_ENTER_SYSTEM_MODE_24,
//		},
//	},

	//BCM
	{
		{HYUNDAI_INIT_MODE_0C},
		{MSG_Init_CAN_PackType_Hyundai},
		{0x2C, 0x01, 0x07, 0xA1, 0x20, 0x02, 0x82, 0x03, 0x00, 0x04, 0x00, 0x00, 0x07, 0xA0, 0x05, 0x00, 0x00, 0x07, 0xA8, 0x06, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x0a, 0x0A, 0x85, 0x00, 0x0B, 0x00, 0x00, 0x0C, 0xe6},
		{0x01},
		{ HYUNDAI_ENTER_SYSTEM_MODE_0C,
		},
	},
	//BCM KWP
	{
		{HYUNDAI_INIT_MODE_0D},
		{MSG_Init_KWP_PackType_Hyundai},
		{0x2B,0x01,0x00,0x28,0xa0,0x02,0x00,0x03,0xc0,0x04,0xcc,0x00,0xc1,0xc1,0x05,0x14,0x32,0x37,0x05,0x04,0x00,0x19,0x06,0x00,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x35,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
		{0x01},
		{
			HYUNDAI_ENTER_SYSTEM_MODE_1E,
		},
	},

};


const ENTER_SYSTEM_HYUNDAI Enter_System_Hyundai[] = \
{
	//HYUNDAI_ENTER_SYSTEM_MODE System_Enter_Mode;
	//uint8_t System_Enter_Hyundai[8];//进系统命令
	//uint8_t ECUID_CMD_Hyundai[8];//匹配ECUID的命令
	//uint8_t System_Exit_Hyundai[8];//退系统命令
	//uint8_t Hyundai_ECUID;//匹配的ECUID

	//Engine CAN 1
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_01},
		{ 0x02,0x10,0x03 },
		{ 0x00,0x00 },
		{ 0x01,0x20 },
		{ HYUNDAI_ECUID_MODE_01 },
	},

		//Engine CAN 2
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_02},
		{0x02,0x10,0x81},
		{0x00,0x00},
		{0x01,0x20},
		{HYUNDAI_ECUID_MODE_02},
	},

	//Engine CAN 3
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_03},
		{0x02,0x10,0x90},
		{0x00,0x00},
		{0x01,0x20},
		{HYUNDAI_ECUID_MODE_03},//FI_
	},

	//Engine CAN 4
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_04},
		{0x02,0x10,0x90},
		{0x02,0x1A,0x91},
		{0x01,0x20},
		{HYUNDAI_ECUID_MODE_04},//DIE
	},

	//ABS CAN 1
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_05},
		{0x02,0x10,0x01},
		{0x00,0x00},
		{0x00,0x00},
		{HYUNDAI_ECUID_MODE_05},//PFI
	},

	//ABS CAN 2 
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_06},
		{0x02,0x10,0x01},
		{0x00,0x00},
		{0x00,0x00},
		{HYUNDAI_ECUID_MODE_06},//PFI
	},

	//ABS  CAN 3 
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_07},
		{0x02,0x10,0x81},
		{0x00,0x00},
		{0x01,0x20},
		{HYUNDAI_ECUID_MODE_07},//PFI
	},

	//SRS CAN type 1 
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_08},
		{0x02,0x10,0x03},
		{0x00,0x00},
		{0x01,0x20},
		{HYUNDAI_ECUID_MODE_08},//PFI
	},

	//SRS CAN type 2
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_09},
		{0x02,0x10,0x03},
		{0x00,0x00},
		{0x01,0x20},
		{HYUNDAI_ECUID_MODE_09},//PFI
	},

	//SRS CAN type 2 2-3
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_0A},
		{0x02,0x10,0x90},
		{0x00,0x00},
		{0x01,0x20},
		{HYUNDAI_ECUID_MODE_0A},//PFI
	},

	//Engine Hyundai INIT6 
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_0B},
		{0x02,0x10,0x90},
		{0x00,0x00},
		{0x01,0x20},
		{HYUNDAI_ECUID_MODE_0B},//PFI
	},

	//Engine Hyundai INIT6 
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_0C},
		{0x02,0x10,0x03},
		{0x00,0x00},
		{0x01,0x20},
		{HYUNDAI_ECUID_MODE_0C},//
	},

	//Engine kwp 13/F1/0F/02
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_0D},
		{0x01,0x81},//ORD=0x23
		{0x00,0x00},
		{0x00,0x00},
		{HYUNDAI_ECUID_MODE_0D},//18,00,00,00
	},

	//Engine kwp 13/F1/11/02
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_0E},
		{0x01,0x81},//ORD=0x23
		{0x00,0x00},
		{0x00,0x82},
		{HYUNDAI_ECUID_MODE_0E},//13,00,00
	},

	//Engine 13/F1/11/02 
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_0F},
		{0x01,0x81},//ORD=0x23
		{0x00,0x00},
		{0x00,0x82},
		{HYUNDAI_ECUID_MODE_0D},//18,00,00,00
	},

	//Engine 13/F1/11/02  
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_10},
		{0x01,0x81},//ORD=0x23
		{0x00,0x00},
		{0x00,0x82},
		{HYUNDAI_ECUID_MODE_04},//18,00,FF,00
	},

	//Engine 13/F1/11/02  
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_11},
		{0x01,0x81},//ORD=0x23
		{0x00,0x00},
		{0x00,0x82},
		{HYUNDAI_ECUID_MODE_0F},//19,50,FF,00
	},

	//ecm 13/F1/12/02  
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_12},
		{0x01,0x81},//ORD=0x23
		{0x00,0x00},
		{0x00,0x82},
		{HYUNDAI_ECUID_MODE_10},//18,02,FF,00
	},

	//ECM 13/F1/14/02    
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_13},
		{0x01,0x81},//ORD=0x23
		{0x00,0x00},
		{0x00,0x82},
		{HYUNDAI_ECUID_MODE_04},//18,00,FF,00
	},

	//AT 13/F1/0F/02  
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_14},
		{0x01,0x81},//ORD=0x23
		{0x00,0x00},
		{0x00,0x82},
		{HYUNDAI_ECUID_MODE_0D},//18,00,00,00	
	},

	//AT 13/F1/11/02 
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_15},
		{0x01,0x81},//ORD=0x23
		{0x00,0x00},
		{0x00,0x82},
		{HYUNDAI_ECUID_MODE_0F},//19,50,FF,00
	},

	//AT 13/F1/18/02  
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_16},
		{0x01,0x81},//ORD=0x23
		{0x00,0x00},
		{0x00,0x82},
		{HYUNDAI_ECUID_MODE_11},//13,FF,00
	},

	//AT 13/F1/18/02 
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_17},
		{0x01,0x81},//ORD=0x23
		{0x00,0x00},
		{0x00,0x82},
		{HYUNDAI_ECUID_MODE_12},//18,02,00,00
	},

	//ABS 13/F1/20/04  
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_18},
		{0x01,0x81},//ORD=0x23
		{0x02,0x10,0x81},
		{0x00,0x82},
		{HYUNDAI_ECUID_MODE_04},//18,00,FF,00
	},

	//ABS 13/F1/28/04  
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_19},
		{0x01,0x81},//ORD=0x23
		{0x02,0x10},
		{0x00,0x82},
		{HYUNDAI_ECUID_MODE_13},//13,40,00
	},

	//ABS 13/F1/28/04 
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_1A},
		{0x01,0x81},//ORD=0x23
		{0x02,0x10,0x81},
		{0x00,0x82},
		{HYUNDAI_ECUID_MODE_13},//13,40,00
	},

	//ABS 13/F1/28/0 
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_1B},
		{0x01,0x81},//ORD=0x23
		{0x02,0x10,0x81},
		{0x00,0x82},
		{HYUNDAI_ECUID_MODE_04},//18,00,FF,00
	},

	//SRS KWP  13/F1/58/04
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_1C},
		{0x01,0x81},//ORD=0x23
		{0x00,0x00},
		{0x00,0x82},
		{HYUNDAI_ECUID_MODE_14},//18,00,80,00 18,01,80,00
	},

	//SRS KWP 13/F1/58/04
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_1D},
		{0x01,0x81},//ORD=0x23
		{0x00,0x00},
		{0x00,0x82},
		{HYUNDAI_ECUID_MODE_04},//18,00,FF,00
	},

	//BCM 13/F1/40/04 
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_1E},
		{0x01,0x81},//ORD=0x23
		{0x02,0x1A,0x80},
		{0x00,0x82},
		{HYUNDAI_ECUID_MODE_14},//18,00,80,00  18,01,80,00
	},

	//ECM 5bps 13/F1/6A/05 
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_1F},
		{0x01,0x33}, //ord = 0x23
		{0x00,0x00},
		{0x00,0x00},
		{HYUNDAI_ECUID_MODE_15},//03
	},

	//AT 5bps  13/F1/6A/05
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_20},
		{0x01,0x33}, //ord = 0x23
		{0x02,0xA1,0x0A},
		{0x00,0x00},
		{HYUNDAI_ECUID_MODE_16},//13,FF,FF /13,FF,00
	},

	//ABS 5bps  13/F1/6A/05
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_21},
		{0x01,0x30}, //ord = 0x23
		{0x00,0x00},
		{0x00,0x00},
		{HYUNDAI_ECUID_MODE_17},//03
	},

	//SRS 13/F1/6A/05  
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_22},
		{0x01,0x36}, //ord = 0x23
		{0x00,0x00},
		{0x01,0x11},
		{HYUNDAI_ECUID_MODE_18},//B3
	},

	//SRS 13/F1/36/02  
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_23},
		{0x01,0x36}, //ord = 0x23
		{0x00,0x00},
		{0x01,0x82},
		{HYUNDAI_ECUID_MODE_19},//21,80
	},

	//Srs   13///00
	{
		{HYUNDAI_ENTER_SYSTEM_MODE_24},
		{0x01,0x00}, //ord = 0x23
		{0x00,0x00},
		{0x03,0x02,0x06,0x08},
		{HYUNDAI_ECUID_MODE_1A},//02,07,09	02,09,0B
	},



};



const HYUNDAI_ECUID Hyundai_EcuId[] = \
{

	//	HYUNDAI_ECUID_MODE Hyundai_ECUID;//匹配的ECUID
	//	uint8_t CMD_Read_DTC_Size;//读码命令个数
	//	uint8_t CMD_Read_DTC[8][8];//读码命令 最多有8条读码命令
	//	uint8_t DTC_Pos[8];//DCT_TYPE_Pos_LEN_DTCLEN
	//	uint8_t CMD_Clear_DTC_Size;//清码命令个数
	//	uint8_t CMD_Clear_DTC[8][8];//清码命令
		//
		//
	{
		{HYUNDAI_ECUID_MODE_01},
		{ 0x03 },//CMD_Read_DTC_Size
		{
			{0x03,0x19,0x02,0x09},
			{0x03,0x19,0x02,0x0D},
			{0x03,0x19,0x02,0x8d},
		},
		{ 0x02,0x03,0x04,0x03 },//DCT_TYPE_Pos_LEN_DTCLEN
		{ 0x01 },//CMD_Clear_DTC_Size
		{
			{0x04,0x14,0xFF,0xFF,0xFF},
		},
	},

	//
	{
		{HYUNDAI_ECUID_MODE_02},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x04,0x18,0x00,0xFF,0x00},
		},
		{0x02,0x02,0x03,0x02},//DCT_TYPE_Pos_LEN_DTCLEN
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x03,0x14,0xff,0x00},
		},
	},

	//
	{
		{HYUNDAI_ECUID_MODE_03},
		{0x02},//CMD_Read_DTC_Size
		{
			{0x04,0x18,0x00,0x00,0x00},
			{0x04,0x18,0x00,0xFF,0x00},
		},
		{0x02,0x02,0x03,0x02},//DCT_TYPE_Pos_LEN_DTCLEN
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x03,0x14,0xFF,0x00},
		},
	},

	//
	{
		{HYUNDAI_ECUID_MODE_04},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x04,0x18,0x00,0xFF,0x00},
		},
		{0x02,0x02,0x03,0x02},//DCT_TYPE_Pos_LEN_DTCLEN
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x03,0x14,0xFF,0x00},
		},
	},

	//ABS CAN 1
	{
		{HYUNDAI_ECUID_MODE_05},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x03,0x19,0x02,0x89},
		},
		{0x02,0x03,0x04,0x03,},//DCT_Pos
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x04,0x14,0xFF,0xFF,0xFF},
		},
	},

	//ABS CAN 2
	{
		{HYUNDAI_ECUID_MODE_06},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x03,0x19,0x02,0x08},
		},
		{0x02,0x03,0x04,0x03},//DCT_Pos
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x04,0x14,0xFF,0xFF,0xFF},
		},
	},

	//AT Hyundai EAT
	{
		{HYUNDAI_ECUID_MODE_07},
		{0x02},//CMD_Read_DTC_Size
		{
			{0x04,0x18,0x00,0x40,0x00},
			{0x04,0x18,0x02,0x40,0x00},
		},
		{0x02,0x02,0x03,0x02},//DCT_Pos
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x04,0x14,0x40,0x00},
		},
	},

	//SRS type1
	{
		{HYUNDAI_ECUID_MODE_08},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x03,0x19,0x02,0x09},
		},
		{0x02,0x03,0x04,0x03},//DCT_TYPE_Pos_LEN_DTCLEN
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x04,0x14,0xFF,0xFF,0xFF},
		},
	},

	//SRS type2
	{
		{HYUNDAI_ECUID_MODE_09},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x03,0x19,0x02,0x08},
		},
		{0x02,0x03,0x04,0x03},//DCT_TYPE_Pos_LEN_DTCLEN
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x04,0x14,0xFF,0xFF,0xFF},
		},
	},

	//SRS type2 2-3
	{
		{HYUNDAI_ECUID_MODE_0A},
		{0x02},//CMD_Read_DTC_Size
		{
			{0x04,0x18,0x00,0xFF,0x00},
			{0x04,0x18,0x60,0xFF,0x00},
		},
		{0x02,0x02,0x03,0x02},//DCT_Pos
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x04,0x14,0xFF,0x00},
		},
	},

	//TCMCAM 
	{
		{HYUNDAI_ECUID_MODE_0B},
		{0x02},//CMD_Read_DTC_Size
		{
			{0x04,0x18,0x00,0xFF,0x00},
			{0x04,0x18,0x20,0xFF,0x00},
		},
		{0x02,0x02,0x03,0x02},//DCT_Pos
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x04,0x14,0xFF,0x00},
		},
	},

	//BCM
	{
		{HYUNDAI_ECUID_MODE_0C},
		{0x02},//CMD_Read_DTC_Size
		{
			{0x03,0x19,0x02,0x08},
			{0x03,0x19,0x02,0x09},
		},
		{0x02,0x03,0x04,0x03},//DCT_TYPE_Pos_LEN_DTCLEN
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x04,0x14,0xFF,0xFF,0xFF},
		},
	},

	//
	{
		{HYUNDAI_ECUID_MODE_0D},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x04,0x18,0x00,0x00,0x00},
		},
		{0x02,0x02,0x03,0x02},//DCT_TYPE_Pos_LEN_DTCLEN
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x03,0x14,0xFF,0x00},
		},
	},

	//
	{
		{HYUNDAI_ECUID_MODE_0E},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x03,0x13,0x00,0x00},
		},
		{0x02,0x02,0x02,0x02},//DCT_TYPE_Pos_LEN_DTCLEN
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x03,0x14,0x00,0x00},
		},
	},

	//
	{
		{HYUNDAI_ECUID_MODE_0F},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x04,0x18,0x50,0xFF,0x00},
		},
		{0x02,0x02,0x04,0x02},//DCT_TYPE_Pos_LEN_DTCLEN
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x03,0x14,0xFF,0x00},
		},
	},

	//
	{
		{HYUNDAI_ECUID_MODE_10},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x04,0x18,0x02,0xFF,0x00},
		},
		{0x02,0x02,0x03,0x02},//DCT_TYPE_Pos_LEN_DTCLEN
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x03,0x14,0xFF,0x00},
		},
	},

	//
	{
		{HYUNDAI_ECUID_MODE_11},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x03,0x13,0xFF,0x00},
		},
		{0x02,0x02,0x02,0x02},//DCT_TYPE_Pos_LEN_DTCLEN
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x03,0x14,0xFF,0x00},
		},
	},

	//
	{
		{HYUNDAI_ECUID_MODE_12},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x04,0x18,0x02,0x00,0x00},
		},
		{0x02,0x02,0x03,0x02},//DCT_TYPE_Pos_LEN_DTCLEN
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x03,0x14,0xFF,0x00},
		},
	},

	//AT CAN CANEAT
	{
		{HYUNDAI_ECUID_MODE_13},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x03,0x13,0x40,0x00},
		},
		{0x02,0x02,0x02,0x02},//DCT_TYPE_Pos_LEN_DTCLEN
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x03,0x14,0x40,0x00},
		},
	},

	//
	{
		{HYUNDAI_ECUID_MODE_14},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x04,0x18,0x00,0x80,0x00},
			{0x04,0x18,0x01,0x80,0x00},
		},
		{0x02,0x02,0x03,0x02},//DCT_Pos
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x03,0x14,0x80,0x00},
		},
	},

	//ECM
	{
		{HYUNDAI_ECUID_MODE_15},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x01,0x03},
		},
		{0x02,0x01,0x02,0x02},//DCT_Pos
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x01,0x04},
		},
	},

	//AT CAN DCT
	{
		{HYUNDAI_ECUID_MODE_16},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x03,0x13,0xFF,0xFF},
			{0x03,0x13,0xFF,0xFF},
		},
		{0x02,0x01,0x02,0x02},//DCT_Pos
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x01,0x14},
		},
	},

	//
	{
		{HYUNDAI_ECUID_MODE_17},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x01,0x03},
		},
		{0x02,0x01,0x02,0x02},//DCT_Pos
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x01,0x14},
		},
	},

	//SRS  
	{
		{HYUNDAI_ECUID_MODE_18},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x01,0xB3},
		},
		{0x02,0x01,0x02,0x02},//DCT_Pos
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x01,0x14},
		},
	},
	//SRS  
	{
		{HYUNDAI_ECUID_MODE_19},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x02,0x21,0x80},
		},
		{0x02,0x03,0x06,0x02},//DCT_Pos
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x01,0x14},
		},
	},
	//SRS  
	{
		{HYUNDAI_ECUID_MODE_19},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x03,0x02,0x07,0x09},
			{0x03,0x02,0x09,0x0B},
		},
		{0x02,0x02,0x05,0x02},//DCT_Pos
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x03,0x02,0x05,0x07},
		},
	},

};



//公共函数
bool SetFrameTimeOut_Hyundai(HYUNDAI_INIT_MODE index)
{
	uint8_t  ord;
	uint8_t initdata[128];
	uint8_t number, i = 0;
	uint32_t canid;
	m_CAN_ANS_T = 0;
	m_CAN_ANS_R = 0;

	ord = InitIO_Hyundai[index].Msg_Ord;
	number = InitIO_Hyundai[index].CMD_DATA[0];
	memcpy(initdata, InitIO_Hyundai[index].CMD_DATA + 1, number);
	switch (ord)
	{
	case 0x13:
		while (i < number)
		{
			switch (initdata[i])
			{
			case 0x01: i += 4;	 break;
			case 0x02: i += 2;	break;
			case 0x03: i += 2;	 break;
			case 0x04: i += 5;	 break;
			case 0x05:
				g_defaultMsgAttr.m_AnswerByteTimeOut = initdata[i + 1];
				g_defaultMsgAttr.m_AnswerFrameTimeOut = initdata[i + 2];
				g_defaultMsgAttr.m_RequestFrameTimeOut = initdata[i + 3];
				g_defaultMsgAttr.m_RequestByteTimeOut = initdata[i + 4];
				g_defaultMsgAttr.m_AutoKeepTimeOut = initdata[i + 5];
				g_defaultMsgAttr.m_AutoKeepTimeOut = initdata[i + 6] * 256 + initdata[i + 7];
				i += 8;

				break;
			case 0x06:
				g_defaultMsgAttr.m_KeepFlag = initdata[i + 1];
				i += 2;
				break;
			case 0x07: i += initdata[i + 1] * 2 + 2; break;
			case 0x08: i += 2;	 break;
			case 0x09:
				g_defaultMsgAttr.m_ISOAnswerNum = initdata[i + 1];
				i += 2;
				break;
			case 0x0a: i += 4;	 break;
			case 0x0b: i += 2;	 break;
			case 0x0c: i += 4;	 break;
			case 0x0d: i += 2;	 break;
			case 0x0e: i += 3;	 break;
			case 0x0f: i += 2;	 break;
			default:
				//??
				return FALSE;

			}
		}
		break;
	case 0x14:
		while (i < number)
		{
			switch (initdata[i])
			{
			case 0x01: i += 4;	 break;
			case 0x02: i += 2;	break;
			case 0x03: i += 2;	 break;
			case 0x04: i += 5;	 break;
			case 0x05:
				g_defaultMsgAttr.m_AnswerByteTimeOut = initdata[i + 1];
				g_defaultMsgAttr.m_AnswerFrameTimeOut = initdata[i + 2];
				g_defaultMsgAttr.m_RequestFrameTimeOut = initdata[i + 3];
				g_defaultMsgAttr.m_RequestByteTimeOut = initdata[i + 4];
				g_defaultMsgAttr.m_AutoKeepTimeOut = initdata[i + 5];
				g_defaultMsgAttr.m_AutoKeepTimeOut = initdata[i + 6] * 256 + initdata[i + 7];
				i += 8;

				break;
			case 0x06:
				g_defaultMsgAttr.m_KeepFlag = initdata[i + 1];
				i += 2;
				break;
			case 0x07: i += initdata[i + 1] * 2 + 2; break;
			case 0x08: i += 2;	 break;
			case 0x09:
				g_defaultMsgAttr.m_ISOAnswerNum = initdata[i + 1];
				i += 2;
				break;
			case 0x0a: i += 4;	 break;
			case 0x0b: i += 2;	 break;
			case 0x0c: i += 5;	 break;
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
		g_defaultMsgAttr.m_AutoKeepTimeOut = 15;
		g_defaultMsgAttr.m_KeepFlag = 00;
		while (i < number)
		{
			switch (initdata[i])
			{
			case 0x01: i += 4;	 break;
			case 0x02:
				//m_CAN_LISTEN_TYPE_Toyota =initdata[i+1];
				i += 2;
				break;
			case 0x03: i += 2;	 break;
			case 0x04:
				canid = 0;
				canid += initdata[i + 1] << 24;
				canid += initdata[i + 2] << 16;
				canid += initdata[i + 3] << 8;
				canid += initdata[i + 4];
				setcanbusflowid(canid);
				setcanbussendid(canid);
				i += 5;
				break;
			case 0x05:
				canid = 0;
				canid += initdata[i + 1] << 24;
				canid += initdata[i + 2] << 16;
				canid += initdata[i + 3] << 8;
				canid += initdata[i + 4];
				setcanbusreadid(canid);
				i += 5;	 break;
			case 0x06: i += 5;	 break;
			case 0x07: i += 5;	 break;
			case 0x08: i += 5;	 break;
			case 0x09: i += 3;	 break;
			case 0x0a: i += 3;	 break;
			case 0x0b: i += 3;	 break;
			case 0x0c: i += 2;	 break;
			case 0x0d:
				m_CAN_ANS_T = initdata[i + 1];
				i += 2;
				break;
			case 0x0e:
				m_CAN_ANS_R = initdata[i + 1];
				i += 0x11;
				break;
			case 0x0F:
				i += 2;
				break;
			default:
				//错误
				return FALSE;
			}
		}
		break;
	case 0x11:
		g_defaultMsgAttr.m_AnswerByteTimeOut = 02;
		g_defaultMsgAttr.m_AnswerFrameTimeOut = 0xd;
		g_defaultMsgAttr.m_RequestFrameTimeOut = 15;
		g_defaultMsgAttr.m_RequestByteTimeOut = 02;
		g_defaultMsgAttr.m_AutoKeepTimeOut = 15;
		g_defaultMsgAttr.m_KeepFlag = 00;

		break;
	case 0x12:

		g_defaultMsgAttr.m_AnswerByteTimeOut = 02;
		g_defaultMsgAttr.m_AnswerFrameTimeOut = 50;
		g_defaultMsgAttr.m_RequestFrameTimeOut = 15;
		g_defaultMsgAttr.m_RequestByteTimeOut = 02;
		g_defaultMsgAttr.m_AutoKeepTimeOut = 15;
		g_defaultMsgAttr.m_KeepFlag = 00;

		break;
	case 0x15:
	case 0x16:
		g_defaultMsgAttr.m_AnswerByteTimeOut = 02;
		g_defaultMsgAttr.m_AnswerFrameTimeOut = 50;
		g_defaultMsgAttr.m_RequestFrameTimeOut = 15;
		g_defaultMsgAttr.m_RequestByteTimeOut = 02;
		g_defaultMsgAttr.m_AutoKeepTimeOut = 15;
		g_defaultMsgAttr.m_KeepFlag = 00;
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




//设置通信过程ORD
void pro_msg_all_Hyundai(HYUNDAI_ENTER_SYSTEM_MODE mode)
{
	switch (mode)
	{
		//CAN
	case HYUNDAI_ENTER_SYSTEM_MODE_01:
	case HYUNDAI_ENTER_SYSTEM_MODE_02:
	case HYUNDAI_ENTER_SYSTEM_MODE_03:
	case HYUNDAI_ENTER_SYSTEM_MODE_04:
	case HYUNDAI_ENTER_SYSTEM_MODE_05:
	case HYUNDAI_ENTER_SYSTEM_MODE_06:
	case HYUNDAI_ENTER_SYSTEM_MODE_07:
	case HYUNDAI_ENTER_SYSTEM_MODE_08:
	case HYUNDAI_ENTER_SYSTEM_MODE_09:
	case HYUNDAI_ENTER_SYSTEM_MODE_0A:
	case HYUNDAI_ENTER_SYSTEM_MODE_0B:
	case HYUNDAI_ENTER_SYSTEM_MODE_0C:

		g_defaultMsgAttr.m_CS_MODE = 0;
		g_defaultMsgAttr.m_ORD = 0x30;
		g_defaultMsgAttr.m_PCK_TYPE = 0x22;
		g_defaultMsgAttr.m_RWAYS = 0;
		g_defaultMsgAttr.m_SRC = 0;
		g_defaultMsgAttr.m_TGT = 0;
		g_defaultMsgAttr.m_WAYS = 0;
		break;



		//Engine kwp 13/F1/0F/02
		//AT 13/F1/0F/02  
	case HYUNDAI_ENTER_SYSTEM_MODE_0D:
	case HYUNDAI_ENTER_SYSTEM_MODE_14:
		g_defaultMsgAttr.m_CS_MODE = 0;
		g_defaultMsgAttr.m_ORD = 0x33;
		g_defaultMsgAttr.m_PCK_TYPE = 0x02;
		g_defaultMsgAttr.m_RWAYS = 0;
		g_defaultMsgAttr.m_SRC = 0xF1;
		g_defaultMsgAttr.m_TGT = 0x0F;
		g_defaultMsgAttr.m_WAYS = 0;
		break;

		//Engine kwp 13/F1/11/02
		//Engine 13/F1/11/02
		//Engine 13/F1/11/02 
		//Engine 13/F1/11/02  
		//AT 13/F1/11/02 
	case HYUNDAI_ENTER_SYSTEM_MODE_0E:
	case HYUNDAI_ENTER_SYSTEM_MODE_0F:
	case HYUNDAI_ENTER_SYSTEM_MODE_10:
	case HYUNDAI_ENTER_SYSTEM_MODE_11:
	case HYUNDAI_ENTER_SYSTEM_MODE_15:
		g_defaultMsgAttr.m_CS_MODE = 0;
		g_defaultMsgAttr.m_ORD = 0x33;
		g_defaultMsgAttr.m_PCK_TYPE = 0x02;
		g_defaultMsgAttr.m_RWAYS = 0;
		g_defaultMsgAttr.m_SRC = 0xF1;
		g_defaultMsgAttr.m_TGT = 0x11;
		g_defaultMsgAttr.m_WAYS = 0;
		break;

		//ecm 13/F1/12/02  
	case HYUNDAI_ENTER_SYSTEM_MODE_12:
		g_defaultMsgAttr.m_CS_MODE = 0;
		g_defaultMsgAttr.m_ORD = 0x33;
		g_defaultMsgAttr.m_PCK_TYPE = 0x02;
		g_defaultMsgAttr.m_RWAYS = 0;
		g_defaultMsgAttr.m_SRC = 0xF1;
		g_defaultMsgAttr.m_TGT = 0x12;
		g_defaultMsgAttr.m_WAYS = 0;
		break;

		//ECM 13/F1/14/02    
	case HYUNDAI_ENTER_SYSTEM_MODE_13:
		g_defaultMsgAttr.m_CS_MODE = 0;
		g_defaultMsgAttr.m_ORD = 0x33;
		g_defaultMsgAttr.m_PCK_TYPE = 0x02;
		g_defaultMsgAttr.m_RWAYS = 0;
		g_defaultMsgAttr.m_SRC = 0xF1;
		g_defaultMsgAttr.m_TGT = 0x14;
		g_defaultMsgAttr.m_WAYS = 0;
		break;

		//AT 13/F1/18/02 
	case HYUNDAI_ENTER_SYSTEM_MODE_16:
	case HYUNDAI_ENTER_SYSTEM_MODE_17:
		g_defaultMsgAttr.m_CS_MODE = 0;
		g_defaultMsgAttr.m_ORD = 0x33;
		g_defaultMsgAttr.m_PCK_TYPE = 0x02;
		g_defaultMsgAttr.m_RWAYS = 0;
		g_defaultMsgAttr.m_SRC = 0xF1;
		g_defaultMsgAttr.m_TGT = 0x18;
		g_defaultMsgAttr.m_WAYS = 0;
		break;

		//ABS 13/F1/20/04  
	case HYUNDAI_ENTER_SYSTEM_MODE_18:
		g_defaultMsgAttr.m_CS_MODE = 0;
		g_defaultMsgAttr.m_ORD = 0x33;
		g_defaultMsgAttr.m_PCK_TYPE = 0x04;
		g_defaultMsgAttr.m_RWAYS = 0;
		g_defaultMsgAttr.m_SRC = 0xF1;
		g_defaultMsgAttr.m_TGT = 0x20;
		g_defaultMsgAttr.m_WAYS = 0;
		break;

		//ABS 13/F1/28/04 
	case HYUNDAI_ENTER_SYSTEM_MODE_19:
	case HYUNDAI_ENTER_SYSTEM_MODE_1A:
	case HYUNDAI_ENTER_SYSTEM_MODE_1B:
		g_defaultMsgAttr.m_CS_MODE = 0;
		g_defaultMsgAttr.m_ORD = 0x33;
		g_defaultMsgAttr.m_PCK_TYPE = 0x04;
		g_defaultMsgAttr.m_RWAYS = 0;
		g_defaultMsgAttr.m_SRC = 0xF1;
		g_defaultMsgAttr.m_TGT = 0x13;
		g_defaultMsgAttr.m_WAYS = 0;
		break;

		//SRS KWP  13/F1/58/04
	case HYUNDAI_ENTER_SYSTEM_MODE_1C:
	case HYUNDAI_ENTER_SYSTEM_MODE_1D:
		g_defaultMsgAttr.m_CS_MODE = 0;
		g_defaultMsgAttr.m_ORD = 0x33;
		g_defaultMsgAttr.m_PCK_TYPE = 0x04;
		g_defaultMsgAttr.m_RWAYS = 0;
		g_defaultMsgAttr.m_SRC = 0xF1;
		g_defaultMsgAttr.m_TGT = 0x58;
		g_defaultMsgAttr.m_WAYS = 0;
		break;

		//BCM 13/F1/40/04 
	case HYUNDAI_ENTER_SYSTEM_MODE_1E:
		g_defaultMsgAttr.m_CS_MODE = 0;
		g_defaultMsgAttr.m_ORD = 0x33;
		g_defaultMsgAttr.m_PCK_TYPE = 0x04;
		g_defaultMsgAttr.m_RWAYS = 0;
		g_defaultMsgAttr.m_SRC = 0xF1;
		g_defaultMsgAttr.m_TGT = 0x40;
		g_defaultMsgAttr.m_WAYS = 0;
		break;

		//ECM 5bps 13/F1/6A/05 
		//AT 5bps  13/F1/6A/05
		//ABS 5bps  13/F1/6A/05
		//SRS 13/F1/6A/05 
	case HYUNDAI_ENTER_SYSTEM_MODE_1F:
	case HYUNDAI_ENTER_SYSTEM_MODE_20:
	case HYUNDAI_ENTER_SYSTEM_MODE_21:
	case HYUNDAI_ENTER_SYSTEM_MODE_22:
		g_defaultMsgAttr.m_CS_MODE = 0;
		g_defaultMsgAttr.m_ORD = 0x33;
		g_defaultMsgAttr.m_PCK_TYPE = 0x05;
		g_defaultMsgAttr.m_RWAYS = 0;
		g_defaultMsgAttr.m_SRC = 0xF1;
		g_defaultMsgAttr.m_TGT = 0x6A;
		g_defaultMsgAttr.m_WAYS = 0;
		break;

		//SRS 13/F1/36/02  
	case HYUNDAI_ENTER_SYSTEM_MODE_23:
		g_defaultMsgAttr.m_CS_MODE = 0;
		g_defaultMsgAttr.m_ORD = 0x33;
		g_defaultMsgAttr.m_PCK_TYPE = 0x02;
		g_defaultMsgAttr.m_RWAYS = 0;
		g_defaultMsgAttr.m_SRC = 0xF1;
		g_defaultMsgAttr.m_TGT = 0x36;
		g_defaultMsgAttr.m_WAYS = 0;
		break;

		//Srs   13///00
	case HYUNDAI_ENTER_SYSTEM_MODE_24:
		g_defaultMsgAttr.m_CS_MODE = 0;
		g_defaultMsgAttr.m_ORD = 0x33;
		g_defaultMsgAttr.m_PCK_TYPE = 0x00;
		g_defaultMsgAttr.m_RWAYS = 0;
		g_defaultMsgAttr.m_SRC = 0;
		g_defaultMsgAttr.m_TGT = 0;
		g_defaultMsgAttr.m_WAYS = 0;
		break;
	default:
		break;
	}
}

////直接用OBD的
////void ResetComPort()



//初始化IO
bool Init_IO_Hyundai(HYUNDAI_INIT_MODE index)
{
	uint16_t number = 0;
	HYUNDAI_INIT_MODE mode = 0;
	uint8_t sendbuf[128];
	uint8_t recvbuf[32];
	if (!SetFrameTimeOut_Hyundai(index))
	{
		return false;
	}
	memset(recvbuf, 0, sizeof(recvbuf));
	number = InitIO_Hyundai[index].CMD_DATA[0];
	memcpy(sendbuf, InitIO_Hyundai[index].CMD_DATA + 1, number);
	mode = InitIO_Hyundai[index].Init_Mode;
	//g_defaultMsgAttr.m_PCK_TYPE = 0x00;
	number = pro_packsenddata(sendbuf, number, InitIO_Hyundai[index].Msg_Ord);
	if (number == 0)
	{
		return 0;
	}
	ResetComPort();
	vTaskDelay(20);

	USB_BULK_Queue_Write(&USB_RX_QueueData, sendbuf, number);
	OBDII_ReadBuf(recvbuf, &number, 1500);
	//number = pro_SendRecv((uint8_t *)ObdiiInitIoCmd[index].CMD_DATA,(uint8_t *)sendbuf);
	if (number < 1)
	{
		printfUartObdiideBug("InitIO false\r\n");
		return false;
	}
	vTaskDelay(1000);
	//pro_set_can_id_Toyota(index);
	printfUartObdiideBug("InitIO true\r\n");
	return true;
}


intptr_t pro_much_init_hyundai(HYUNDAI_INIT_MODE Index, HYUNDAI_ENTER_SYSTEM_MODE mode)// 进系统
{
	bool bRet = false;
	MSG_ORD_HYUNDAI PackType = InitIO_Hyundai[Index].Msg_Ord;
	uint8_t iEnter = 0;

	if (mode == HYUNDAI_ENTER_SYSTEM_MODE_NULL)//第一次进的时候
	{
		for (iEnter = 0; iEnter < InitIO_Hyundai[Index].enter_size; iEnter++)
		{
			//同一个初始化IO的命令的不同进系统命令
			mode = InitIO_Hyundai[Index].Enter_System_mode[iEnter];//获取具体的是哪个进系统的命令的mode
			
			pro_msg_all_Hyundai(mode);
	
			switch (PackType)
			{
				//CAN
			case MSG_Init_CAN_PackType_Hyundai:
				bRet = pro_canbus_select_module_hyundai(mode);
				break;

				//K-Line
			case MSG_Init_KWP_PackType_Hyundai:
				if (g_defaultMsgAttr.m_PCK_TYPE == 0x04 || g_defaultMsgAttr.m_PCK_TYPE == 0x02 || g_defaultMsgAttr.m_PCK_TYPE == 0x05|| g_defaultMsgAttr.m_PCK_TYPE == 0x00)//KWP2000 10400bps
				{
					bRet = pro_kwp_select_module_hyundai(mode);//kwp的81进系统命令
				}
				else
				{
					;//其他协议未处理
				}
				break;

			default:
				break;
			}
			if (bRet)
			{
				return Enter_System_Hyundai[mode].Hyundai_ECUID;
			}
		}
	}
	else
	{
		pro_msg_all_Hyundai(mode);
		switch (PackType)
		{
			//CAN
		case MSG_Init_CAN_PackType_Hyundai:
			bRet = pro_canbus_select_module_hyundai(mode);
			break;

			//K-Line
		case MSG_Init_KWP_PackType_Hyundai:
			if (g_defaultMsgAttr.m_PCK_TYPE == 0x04 || g_defaultMsgAttr.m_PCK_TYPE == 0x02 || g_defaultMsgAttr.m_PCK_TYPE == 0x05|| g_defaultMsgAttr.m_PCK_TYPE == 0x00)//KWP2000 10400bps
			{
				bRet = pro_kwp_select_module_hyundai(mode);//kwp的81进系统命令
			}
			else
			{
				;//其他协议未处理
			}
			break;

		default:
			break;
		}
		if (bRet)
		{
			return Enter_System_Hyundai[mode].Hyundai_ECUID;
		}
	}

	return -1;
}

bool pro_kwp_select_module_hyundai(HYUNDAI_ENTER_SYSTEM_MODE mode)//kwp的81进系统命令
{
	uint16_t number = 0;
	uint8_t sendbuf[128] = { 0 };
	uint8_t recvbuf[256] = { 0 };
	uint8_t ord = 0;
	uint8_t TGT = 0;
	uint8_t PCK_TYPE = 0;

	//进系统命令
	memcpy(sendbuf, Enter_System_Hyundai[mode].System_Enter, Enter_System_Hyundai[mode].System_Enter[0] + 1);

	ord = g_defaultMsgAttr.m_ORD;//临时保存
	PCK_TYPE = g_defaultMsgAttr.m_PCK_TYPE;//临时保存

	g_defaultMsgAttr.m_ORD = 0x23;//进系统 临时改成0x23
	if(g_defaultMsgAttr.m_ISOAnswerNum>0)
		g_defaultMsgAttr.m_PCK_TYPE = 0x00;

	number = pro_SendRecv(sendbuf, recvbuf);

	g_defaultMsgAttr.m_ORD = ord;//临时保存
	g_defaultMsgAttr.m_PCK_TYPE = PCK_TYPE;//还原回去

	if (number == 0)
	{
		return false;
	}


	if(Enter_System_Hyundai[mode].ECUID_CMD[0]>0)	
	{
		memcpy(sendbuf, Enter_System_Hyundai[mode].ECUID_CMD, Enter_System_Hyundai[mode].ECUID_CMD[0] + 1);
		number = pro_SendRecv(sendbuf, recvbuf);

		if (number == 0)
		{
			return false;
		}
		if (recvbuf[0] == 0x7F)
		{
			return false;
		}
	}
	return true;
}

bool pro_canbus_select_module_hyundai(HYUNDAI_ENTER_SYSTEM_MODE mode)
{
	//	int i, j;
	uint8_t sendbuf[64];
	uint8_t recvbuf[256];
	//	uint8_t sendnum =0;
	uint16_t number = 0;
	//	uint32_t canid;

	memset(sendbuf, 0, sizeof(sendbuf));
	memcpy(sendbuf, Enter_System_Hyundai[mode].System_Enter, Enter_System_Hyundai[mode].System_Enter[0] + 1);

	number = pro_SendRecv(sendbuf, recvbuf); //进系统 带长度

	//printfUartObdiideBug("pro_SendRecv:num=%d|%02X%02X\r\n",number,recvbuf[0],recvbuf[1]);
	if (number == 0)
	{
		return false;
	}
	if (recvbuf[0] == 0x7f)
	{
		return false;
	}
	
	//printfUartObdiideBug("pro_SendRecv:end\r\n");
	if(Enter_System_Hyundai[mode].ECUID_CMD[0]>0)	
	{
		memcpy(sendbuf, Enter_System_Hyundai[mode].ECUID_CMD, Enter_System_Hyundai[mode].ECUID_CMD[0] + 1);
		number = pro_SendRecv(sendbuf, recvbuf);

		if (number == 0)
		{
			return false;
		}
		if (recvbuf[0] == 0x7F)
		{
			return false;
		}
	}
	//printfUartObdiideBug("pro_SendRecv1:num=%d|%02X%02X\r\n",number,recvbuf[0],recvbuf[1]);
	return true;
}



void pro_read_dtc_Hyundai(HYUNDAI_ECUID_MODE ECUID_mode, uint8_t iSystem, int* dtc_number, char* dtc_list, char* dtc_sys) //读码
{
	uint8_t sendcmd[8] = { 0 }; //读当前码
	uint8_t recvbuf[256] = { 0 }; //
	uint8_t number = 0;
	uint8_t bitnumber = 0;
	uint8_t dtcnumber = 0;
	uint8_t ndtcnumber = 0;
	bool bFlag = TRUE;
	char dtc[16] = { 0 };
	char dtcname[32][16] = { 0 };//最多32个码
	char system[4] = {0};
	int total = 0;
	int i = 0, j = 0;
	int dtccmd_size = 0;
	uint8_t pos = 0, codelen = 0, dtclen = 0;



	sprintf(system, "%d", iSystem);//系统

	//多个读码命令
	for (dtccmd_size = 0; dtccmd_size < Hyundai_EcuId[ECUID_mode].CMD_Read_DTC_Size; dtccmd_size++)
	{
		memcpy(sendcmd, Hyundai_EcuId[ECUID_mode].CMD_Read_DTC[dtccmd_size], Hyundai_EcuId[ECUID_mode].CMD_Read_DTC[dtccmd_size][0] + 1);

		pos = Hyundai_EcuId[ECUID_mode].DTC_Pos[1];
		dtclen = Hyundai_EcuId[ECUID_mode].DTC_Pos[2];
		codelen = Hyundai_EcuId[ECUID_mode].DTC_Pos[3];
		switch (Hyundai_EcuId[ECUID_mode].DTC_Pos[0])
		{
		case 0x00:
			number = pro_SendRecv(sendcmd, recvbuf);
			break;//不做处理
		case 0x01:
			break;
		case 0x02:
			//pro_hyundai_read_dtc()
			//POS位开始，两位为一个故障码
			number = pro_SendRecv(sendcmd, recvbuf);
			if (dtclen < 2)
			{
				dtclen = 2;
			}
			dtcnumber = (number - pos) / dtclen;
			for (i = 0; i < dtcnumber; i++)
			{
				memset(dtc, 0, sizeof(dtc));
				for (j = 0; j < codelen; j++)
				{
					if (recvbuf[i*dtclen + pos + j])
					{
						break;
					}
				}
				if (j != codelen)
				{
					for (j = 0; j < codelen; j++)
					{
						if(j == 0)
						{
							switch(recvbuf[i*dtclen + pos]&0xC0)
							{
								case 00:
									sprintf(dtc, "P%02X", recvbuf[i*dtclen + pos]);
									break;
								case 0x40:
									sprintf(dtc, "C%02X", recvbuf[i*dtclen + pos]&0x3F);
									break;
								case 0x80:
									sprintf(dtc, "B%02X", recvbuf[i*dtclen + pos]&0x3F);
									break;
								case 0xC0:
									sprintf(dtc, "U%02X", recvbuf[i*dtclen + pos]&0x3F);
									break;
							}
						}
						else
							sprintf(dtc, "%s%02X", dtc, recvbuf[i*dtclen + pos + j]);
					}
					sprintf((char *)dtcname[ndtcnumber], "%s", dtc); // 把故障码存起来
					bFlag = TRUE;
					for (total = 0; total < ndtcnumber; total++)//比较故障码是否重复
					{
						if (strcmp((char*)dtc, (char*)dtcname[total]) == 0)
						{
							bFlag = FALSE;//此故障码重复
							break;
						}
					}
					if (bFlag)//不重复的添加进去
					{
						ndtcnumber++;
						if (*dtc_number != 0)
						{//60个字节，用","分隔
							strcat(dtc_list, ",");
							strcat(dtc_sys, ",");
						}
						strcat(dtc_list, dtc);
						strcat(dtc_sys, system);
						(*dtc_number)++;
						printfUartObdiideBug("hyundai add dtc:%s system:%s \r\n", dtc, system);
						printfUartObdiideBug("hyundai dtc_list:%s dtc_number:%d \r\n", dtc_list, *dtc_number);
					}
				}
			}
			break;

//		case 0x0b:
//			number = pro_SendRecv(sendcmd, recvbuf);
//			msgxml2 = ezxml_child(xml, "CMD");
//			pos1 = getHex(ezxml_attr(msgxml2, "POS"));//???????
//			start = getHex(ezxml_attr(msgxml2, "START"));
//			end = getHex(ezxml_attr(msgxml2, "END"));
//			dtclen1 = getHex(ezxml_attr(msgxml2, "DTCLEN"));
//			codelen1 = getHex(ezxml_attr(msgxml2, "CODELEN"));
//			num = number / dtclen;
//			for (i = 0; i < num; i++)
//			{
//				strcpy(buf, ezxml_txt(msgxml2));
//				for (j = 0; j < codelen; j++)
//				{
//					sprintf(strbuf, "%02X", recvbuf[pos + i*dtclen + j]);
//					buf[(pos1+j)*3] = strbuf[0];
//					buf[(pos1+j)*3+1] = strbuf[1];
//				}
//				ezxml_set_txt(msgxml2, buf);
//				number = pro_SendRecv(msgxml2, recvbuf2);
//				if (number == 0)
//				{
//					number = pro_SendRecv(msgxml2, recvbuf2);
//					if (number == 0)
//					{
//						continue;
//					}
//				}
//				if(sta == 0x08)
//				{
//					switch(recvbuf2[4]&0x60)
//					{
//					case 0x60:
//						sprintf(strsta, trr(String_Active));
//						break;
//					case 0x20:
//						sprintf(strsta, trr(String_Historic));
//						break;
//					default :
//						break;
//					}
//				}
//				for (n = 0; n < (end-start+1)/2 && (n*dtclen1 + start) < number-1; n++)
//				{
//					memset(strbuf, 0, sizeof(strbuf));
//					memset(strcode, 0, sizeof(strcode));
//					for (m = 0; m < codelen1; m++)
//					{
//						sprintf(strcode, "%s%02X", strcode, 0);
//						sprintf(strbuf, "%s%02X", strbuf, recvbuf2[start + n*dtclen1 + m]);
//					}
//					if (strcmp(strcode, strbuf) == 0)
//					{
//						continue;
//					}

//					sprintf(dtconebuf, "%s%s", strbuf, strsta);
//					sprintf(dtcallbuf[numdtc], "%s%s", strbuf, strsta);
//					dtcflag = TRUE;
//					numdtc++;
//					for(total=0; total<(numdtc-1); total++)
//					{
//						if (strcmp((char*)dtconebuf, (char*)dtcallbuf[total]) == 0)//??????
//						{
//							dtcflag = FALSE;//?????????
//							break;
//						}
//					}
//					if(dtcflag)
//					{
//						sprintf(dtcbuf, "X%s", strbuf);
//						pro_FindDTC(dtcbuf, entrypoint, strsta, faultallxml, faultj2012xml);
//						count++;
//					}
//				}
//			}
//			break;

		default:
			break;
		}

	}

	return;
}


void pro_clear_dtc_Hyundai(HYUNDAI_ECUID_MODE ECUID_mode) //清码
{
	uint8_t sendcmd[8] = { 0 }; //清码
	uint8_t recvbuf[256] = { 0 }; //
	uint8_t number = 0;
	uint8_t i = 0;

	//多个命令轮流发送，不做判断
	for (i = 0; i < Hyundai_EcuId[ECUID_mode].CMD_Clear_DTC_Size; i++)
	{
		memcpy(sendcmd, Hyundai_EcuId[ECUID_mode].CMD_Clear_DTC[i], Hyundai_EcuId[ECUID_mode].CMD_Clear_DTC[i][0] + 1);
		number = pro_SendRecv(sendcmd, recvbuf);
	}
}

void pro_exit_system_Hyundai(HYUNDAI_ENTER_SYSTEM_MODE index) //退出系统
{
	uint8_t sendcmd[8] = { 0 }; //退出
	uint8_t recvbuf[256] = { 0 }; //
	uint8_t number = 0;

	if (Enter_System_Hyundai[index].System_Exit[0] > 0)
	{
		memcpy(sendcmd, Enter_System_Hyundai[index].System_Exit, Enter_System_Hyundai[index].System_Exit[0] + 1);
		number = pro_SendRecv(sendcmd, recvbuf);
	}
}


bool Much_Init_Hyundai(void)
{
	uint8_t i = 0, j = 0;
	uint8_t initstart = 0, initend = 0;
	HYUNDAI_INIT_MODE mode = 0;
	intptr_t ECUid = -1;
	bool enter = false;
	//int num = 0;
	//char buf[60] = {0};

	uint8_t iSystem = 5;//一共5个系统

	memset(g_vehiclediagnosie.fault_list, 0, 40);
	memset(g_vehiclediagnosie.sys_list, 0, 40);
	g_vehiclediagnosie.fault_total = 0;

	get_task_info(Task_ObdiiCmdProcess_Proc_handle);
	printfUartObdiideBug("welcome to [%s]\r\n", __func__);
	if (m_first_hyundai)
	{
		//第一次进入
		m_first_hyundai = false;

		initstart = 0;//从第一个系统开始扫描时重置
		initend = 0;//从第一个系统开始扫描时重置
		for (i = 0; i < iSystem; i++)
		{
			initstart = initend;
			initend += m_iSystem_Mode_Size_Hyundai[i];
			for (j = initstart; j < initend; j++)
				//for (j = 0x15; j < 0x16; j++)//测试用
			{
				//printfUartObdiideBug("InitIO_Hyundai = %d \r\n",j);
				//reset_global_data();//重置全局变量数据
				if (Init_IO_Hyundai(InitIO_Hyundai[j].Init_Mode)) //初始化IO
				{
					ECUid = pro_much_init_hyundai(InitIO_Hyundai[j].Init_Mode, HYUNDAI_ENTER_SYSTEM_MODE_NULL);
					if (ECUid != -1) //初始化ECU //进系统
					{
						printfUartObdiideBug("hyundai enter success system:%d init:%d enter:%d ECUID:%d\r\n", i, j, InitIO_Hyundai[j].Init_Mode, ECUid);
						m_protocol_index_hyundai[i][0] = InitIO_Hyundai[j].Init_Mode;					//
						m_protocol_index_hyundai[i][1] = ECUid;
						pro_read_dtc_Hyundai(ECUid, i + 1, &(g_vehiclediagnosie.fault_total), g_vehiclediagnosie.fault_list, g_vehiclediagnosie.sys_list);	//读码
						pro_clear_dtc_Hyundai(ECUid);															//清码
						pro_exit_system_Hyundai(InitIO_Hyundai[j].Init_Mode);														//退出
						break;
					}
					else
					{
						printfUartObdiideBug("hyundai enter false system:%d init:%d (%d-%d) \r\n", i, j, initstart, initend - 1);
					}
				}
			}//for (j = initstart; j < initend; j++)
		}//for (i = 0;i<iSystem;i++)
		for (i = 0; i < iSystem; i++)
		{
			if (m_protocol_index_hyundai[i][0] != -1)
				return true;
		}
	}
	else
	{
		vTaskDelay(3000);
		enter = false;
		for (i = 0; i < iSystem; i++)
		{
			if (m_protocol_index_hyundai[i][0] != -1)//直接使用记录的协议，如果为-1，则说明不存在此系统
			{
				if (Init_IO_Hyundai(InitIO_Hyundai[m_protocol_index_hyundai[i][0]].Init_Mode)) //初始化IO
				{
					ECUid = pro_much_init_hyundai(m_protocol_index_hyundai[i][0], m_protocol_index_hyundai[i][1]);
					if (ECUid != -1) //
					{
						//						if(diag_flag == 1 && data_flag == 0)//车辆诊断
						//						{
						enter = true;
						pro_read_dtc_Hyundai(m_protocol_index_hyundai[i][1], i + 1, &(g_vehiclediagnosie.fault_total), g_vehiclediagnosie.fault_list, g_vehiclediagnosie.sys_list);	//读码
						pro_exit_system_Hyundai(m_protocol_index_hyundai[i][0]);														//退出
//							diag_flag = 0;
//						}
					}
				}
			}
		}//for (i = 0;i<iSystem;i++)
//		return enter;
	}
	return enter;
}













/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
