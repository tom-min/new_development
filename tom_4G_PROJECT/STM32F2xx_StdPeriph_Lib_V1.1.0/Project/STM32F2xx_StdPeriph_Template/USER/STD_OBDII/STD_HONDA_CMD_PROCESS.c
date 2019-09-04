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
#include "STD_HONDA_CMD_PROCESS.h"
#include "FreeRTOS.h"
#include "task.h"
#include "STD_OBD_CMD_PROCESS.h"
#include "user_config.h"


uint8_t m_iSystem_Mode_Size_Honda[5] = {7,7,5,1,3};//每个系统存在的协议的个数，第一次扫描的时候要用到
intptr_t m_protocol_index_honda[5][2] = {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}};   //记录侦测到的协议,5个系统的初始化IO和命令都为-1
bool m_first_honda = true;

extern DEFAULT_MSGATTR g_defaultMsgAttr; 

////uint8_t data_flag = 0;
////uint8_t diag_flag = 0;

extern Vehicle_diagnosis g_vehiclediagnosie;
extern Data_collection g_datacollection;

//static uint32_t m_modulebuf[256];
//extern uint8_t m_modulenum;
//extern uint8_t m_setmodule;
//extern uint32_t m_canbussendid;
//extern uint32_t m_canbusflowid;

uint8_t	m_CAN_ANS_T_Honda = 0;
uint8_t	m_CAN_ANS_R_Honda = 0;

//初始化IO命令
const INIT_IO_CMD_HONDA InitIO_Honda[]=\
{
//	HONDA_INIT_MODE Init_Mode;
//	MSG_ORD_HONDA Msg_Ord;
//  uint8_t Init_IO[128];
//	uint8_t enter_size;
//	ENTER_SYSTEM_HONDA enter_system_honda[10];
	
	//engine CAN 
	{
		{HONDA_INIT_MODE_01},
		{MSG_Init_CAN_PackType_Honda},
		{0x41,0x01,0x07,0xa1,0x20,0x02,0x7f,0x03,0x10,0x04,0x98,0xda,0x11,0xf1,0x05,0x98,0xda,0xf1,0x11,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xe6,0x0D,0x01,0x0E,0x01,0x00,0x09,0x30,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x0a},
		{0x02},
		{ HONDA_ENTER_SYSTEM_MODE_01,
			HONDA_ENTER_SYSTEM_MODE_02,
		},
	},
	{
		{HONDA_INIT_MODE_02},
		{MSG_Init_CAN_PackType_Honda},
		{0x41,0x01,0x07,0xa1,0x20,0x02,0x7f,0x03,0x10,0x04,0x98,0xda,0x10,0xf1,0x05,0x98,0xda,0xf1,0x10,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x03,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xe6,0x0D,0x01,0x0E,0x01,0x00,0x09,0x30,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x0a},
		{0x02},
		{ HONDA_ENTER_SYSTEM_MODE_01,
			HONDA_ENTER_SYSTEM_MODE_02,
		},
	},
	{
		{HONDA_INIT_MODE_03},
		{MSG_Init_CAN_PackType_Honda},
		{0x41,0x01,0x07,0xa1,0x20,0x02,0x7f,0x03,0x10,0x04,0x98,0xda,0x0e,0xf1,0x05,0x98,0xda,0xf1,0x0e,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x03,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xe6,0x0D,0x01,0x0E,0x01,0x00,0x09,0x30,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x0a},
		{0x02},
		{ HONDA_ENTER_SYSTEM_MODE_01,
			HONDA_ENTER_SYSTEM_MODE_02,
		},
	},
	
	//engine honda
	{
		{HONDA_INIT_MODE_04},
		{MSG_Init_KWP_PackType_Honda},
		{0x2B,0x01,0x00,0x28,0xa0,0x02,0x00,0x03,0xc0,0x04,0x77,0x00,0xc1,0xc1,0x05,0x0d,0x19,0x24,0x04,0x04,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x46,0x80,0x96,0x08,0x25,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x04,0x01,0x00,},
		{0x02},
		{ HONDA_ENTER_SYSTEM_MODE_03,
			HONDA_ENTER_SYSTEM_MODE_04,
		},
	},
	
	//engine honda
	{
		{HONDA_INIT_MODE_05},
		{MSG_Init_KWP_PackType_Honda},
		{0x2B,0x01,0x00,0x28,0xa0,0x02,0x00,0x03,0xc0,0x04,0x77,0x00,0xc1,0xc1,0x05,0x0d,0x19,0x24,0x01,0x04,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x46,0x80,0x96,0x08,0x25,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x04,0x01,0x00,},
		{0x0F},
		{ HONDA_ENTER_SYSTEM_MODE_03,
			HONDA_ENTER_SYSTEM_MODE_04,
			HONDA_ENTER_SYSTEM_MODE_05,
			HONDA_ENTER_SYSTEM_MODE_04,
			HONDA_ENTER_SYSTEM_MODE_06,
			HONDA_ENTER_SYSTEM_MODE_07,
			HONDA_ENTER_SYSTEM_MODE_08,
			HONDA_ENTER_SYSTEM_MODE_09,
			HONDA_ENTER_SYSTEM_MODE_0A,
			HONDA_ENTER_SYSTEM_MODE_0B,
			HONDA_ENTER_SYSTEM_MODE_0C,
			HONDA_ENTER_SYSTEM_MODE_0D,
			HONDA_ENTER_SYSTEM_MODE_0E,
			HONDA_ENTER_SYSTEM_MODE_0F,
			HONDA_ENTER_SYSTEM_MODE_10,
		},
	},
	
	//engine honda
	{
		{HONDA_INIT_MODE_06},
		{MSG_Init_KWP_PackType_Honda},
		{0x2B,0x01,0x00,0x25,0x80,0x02,0x00,0x03,0xc0,0x04,0x77,0xee,0x51,0x51,0x05,0x0d,0x19,0x24,0x01,0x04,0x00,0x19,0x06,0x03,0x07,0x00,0x08,0x25,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x04,0x01},
		{0x04},
		{ HONDA_ENTER_SYSTEM_MODE_03,
			HONDA_ENTER_SYSTEM_MODE_04,
			HONDA_ENTER_SYSTEM_MODE_05,
			HONDA_ENTER_SYSTEM_MODE_04,
		},
	},
	
	//AT CAN
	{
		{HONDA_INIT_MODE_07},
		{MSG_Init_CAN_PackType_Honda},
		{0x41,0x01,0x07,0xa1,0x20,0x02,0x7f,0x03,0x10,0x04,0x98,0xda,0x1d,0xf1,0x05,0x98,0xda,0xf1,0x1d,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x03,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xe6,0x0D,0x01,0x0E,0x01,0x00,0x09,0x30,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x0a},
		{0x02},
		{ HONDA_ENTER_SYSTEM_MODE_11,
			HONDA_ENTER_SYSTEM_MODE_12,
		},
	},
	
	//AT CAN
	{
		{HONDA_INIT_MODE_08},
		{MSG_Init_CAN_PackType_Honda},
		{0x41,0x01,0x07,0xa1,0x20,0x02,0x7f,0x03,0x10,0x04,0x98,0xda,0x0e,0xf1,0x05,0x98,0xda,0xf1,0x0e,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x03,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xe6,0x0D,0x01,0x0E,0x01,0x00,0x09,0x30,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x0a},
		{0x02},
		{ HONDA_ENTER_SYSTEM_MODE_11,
			HONDA_ENTER_SYSTEM_MODE_12,
		},
	},
	
	//AT CAN
	{
		{HONDA_INIT_MODE_09},
		{MSG_Init_CAN_PackType_Honda},
		{0x41,0x01,0x07,0xa1,0x20,0x02,0x7f,0x03,0x10,0x04,0x98,0xda,0x1c,0xf1,0x05,0x98,0xda,0xf1,0x1c,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x03,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xe6,0x0D,0x01,0x0E,0x01,0x00,0x09,0x30,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x0a},
		{0x01},
		{ HONDA_ENTER_SYSTEM_MODE_13,
		},
	},
	
	//AT CAN
	{
		{HONDA_INIT_MODE_0A},
		{MSG_Init_CAN_PackType_Honda},
		{0x41,0x01,0x07,0xa1,0x20,0x02,0x7f,0x03,0x10,0x04,0x98,0xda,0x1E,0xf1,0x05,0x98,0xda,0xf1,0x1E,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x03,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xe6,0x0D,0x01,0x0E,0x01,0x00,0x09,0x30,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x0a},
		{0x02},
		{ HONDA_ENTER_SYSTEM_MODE_14,
			HONDA_ENTER_SYSTEM_MODE_15,
		},
	},
	
	//AT CAN
	{
		{HONDA_INIT_MODE_0B},
		{MSG_Init_CAN_PackType_Honda},
		{0x41,0x01,0x07,0xa1,0x20,0x02,0x7f,0x03,0x10,0x04,0x98,0xda,0x00,0xf1,0x05,0x98,0xda,0xf1,0x00,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x03,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xe6,0x0D,0x01,0x0E,0x01,0x00,0x09,0x30,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x0a},
		{0x01},
		{ HONDA_ENTER_SYSTEM_MODE_16,
		},
	},
	
	//AT CAN
	{
		{HONDA_INIT_MODE_0C},
		{MSG_Init_CAN_PackType_Honda},
		{0x41,0x01,0x07,0xa1,0x20,0x02,0x7f,0x03,0x10,0x04,0x98,0xda,0x02,0xf1,0x05,0x98,0xda,0xf1,0x02,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x03,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xe6,0x0D,0x01,0x0E,0x01,0x00,0x09,0x30,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x0a},
		{0x01},
		{ HONDA_ENTER_SYSTEM_MODE_17,
		},
	},
	
	//AT honda
	{
		{HONDA_INIT_MODE_0D},
		{MSG_Init_KWP_PackType_Honda},
		{0x2B,0x01,0x00,0x25,0x80,0x02,0x00,0x03,0xc0,0x04,0x77,0xee,0x51,0x51,0x05,0x0d,0x19,0x24,0x01,0x04,0x00,0x19,0x06,0x03,0x07,0x00,0x08,0x25,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x04,0x01,0x00},
		{0x04},
		{ HONDA_ENTER_SYSTEM_MODE_18,
			HONDA_ENTER_SYSTEM_MODE_19,
			HONDA_ENTER_SYSTEM_MODE_1A,
			HONDA_ENTER_SYSTEM_MODE_1B,
		},
	},
	
	//AT honda
	{
		{HONDA_INIT_MODE_0E},
		{MSG_Init_KWP_PackType_Honda},
		{0x2B,0x01,0x00,0x28,0xa0,0x02,0x00,0x03,0xc0,0x04,0x77,0x00,0xc1,0xc1,0x05,0x0d,0x19,0x24,0x01,0x04,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x46,0x80,0x96,0x08,0x25,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x04,0x01,0x00},
		{0x0d},
		{ HONDA_ENTER_SYSTEM_MODE_1A,
			//HONDA_ENTER_SYSTEM_MODE_1A,
			HONDA_ENTER_SYSTEM_MODE_1B,
			HONDA_ENTER_SYSTEM_MODE_1C,
			HONDA_ENTER_SYSTEM_MODE_1D,
			HONDA_ENTER_SYSTEM_MODE_1E,
			HONDA_ENTER_SYSTEM_MODE_1F,
			HONDA_ENTER_SYSTEM_MODE_20,
			HONDA_ENTER_SYSTEM_MODE_18,
			HONDA_ENTER_SYSTEM_MODE_19,
			//HONDA_ENTER_SYSTEM_MODE_1A,
			HONDA_ENTER_SYSTEM_MODE_21,
			HONDA_ENTER_SYSTEM_MODE_22,
			HONDA_ENTER_SYSTEM_MODE_23,
			HONDA_ENTER_SYSTEM_MODE_24,
		},
	},
	
	//ABS CAN
	{
		{HONDA_INIT_MODE_0F},
		{MSG_Init_CAN_PackType_Honda},
		{0x41,0x01,0x07,0xa1,0x20,0x02,0x7f,0x03,0x10,0x04,0x98,0xda,0x28,0xf1,0x05,0x98,0xda,0xf1,0x28,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x03,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xe6,0x0D,0x01,0x0E,0x01,0x00,0x09,0x30,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x0a},
		{0x01},
		{ HONDA_ENTER_SYSTEM_MODE_25,
		},
	},
	
	//ABS honda
	{
		{HONDA_INIT_MODE_10},
		{MSG_Init_KWP_PackType_Honda},
		{0x2B,0x01,0x00,0x28,0xa0,0x02,0x00,0x03,0xc0,0x04,0x77,0x00,0xc1,0xc1,0x05,0x0d,0x19,0x24,0x01,0x04,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x46,0x80,0x96,0x08,0x25,0x09,0x00,0x0A,0x00,0x00,0x00,0x0b,0x00,0x0c,0x04,0x01,0x00},
		{0x02},
		{ 
			HONDA_ENTER_SYSTEM_MODE_26,
			//HONDA_ENTER_SYSTEM_MODE_26,
			HONDA_ENTER_SYSTEM_MODE_27,
		},
	},
	
	//ABS honda
	{
		{HONDA_INIT_MODE_11},
		{MSG_Init_KWP_PackType_Honda},
		{0x2B,0x001,0x00,0x25,0x80,0x02,0x00,0x03,0xc0,0x04,0x77,0xee,0x51,0x51,0x05,0x0d,0x19,0x24,0x01,0x04,0x00,0x19,0x06,0x03,0x07,0x00,0x08,0x25,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x04,0x01,0x00},
		{0x01},
		{ HONDA_ENTER_SYSTEM_MODE_26,
		},
	},
	
	//ABS honda
	{
		{HONDA_INIT_MODE_12},
		{MSG_Init_KWP_PackType_Honda},
		{0x2B,0x01,0x00,0x28,0xa0,0x02,0x00,0x03,0xc0,0x04,0x77,0x00,0xc1,0xc1,0x05,0x0d,0x19,0x24,0x01,0x04,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x46,0x80,0x96,0x08,0x25,0x09,0x00,0x0A,0x00,0x00,0x00,0x0b,0x00,0x0c,0x04,0x01,0x00},
		{0x02},
		{ HONDA_ENTER_SYSTEM_MODE_26,
			HONDA_ENTER_SYSTEM_MODE_28,
		},
	},
	
	//ABS honda
	{
		{HONDA_INIT_MODE_13},
		{MSG_Init_KWP_PackType_Honda},
		{0x2B,0x01,0x00,0x28,0xa0,0x02,0x00,0x03,0xc0,0x04,0x77,0x00,0xc1,0xc1,0x05,0x0d,0x19,0x24,0x01,0x04,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x25,0x09,0x00,0x0A,0x00,0x00,0x00,0x0b,0x00,0x0c,0x04,0x01,0x00,},
		{0x01},
		{ HONDA_ENTER_SYSTEM_MODE_26,
		},
	},
	
//	//ABS 博世 暂未实现
//	{
//		{HONDA_INIT_MODE_},
//		{0x00},
//		{0x00,0x01,0x07,0xa1,0x20,0x02,0x7f,0x03,0x10,0x04,0x98,0xda,0x0e,0xf1,0x05,0x98,0xda,0xf1,0x0e,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x03,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xe6,0x0D,0x01,0x0E,0x01,0x00,0x09,0x30,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x0a},
//		{0x00},
//		{ 0x00,
//		},
//	},
	
	//BODY KWP10400 下面的多个命令只是地址不一样 分别为13/10/18/17
	{
		{HONDA_INIT_MODE_14},
		{MSG_Init_KWP_PackType_Honda},
		{0x2B,0x01,0x00,0x28,0xa0,0x02,0x00,0x03,0xc0,0x04,0x77,0x00,0xc1,0xc1,0x05,0x0f,0x19,0x37,0x01,0x04,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x45,0x09,0x03,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
		{0x04},
		{ HONDA_ENTER_SYSTEM_MODE_29,
			HONDA_ENTER_SYSTEM_MODE_2A,
			HONDA_ENTER_SYSTEM_MODE_2B,
			HONDA_ENTER_SYSTEM_MODE_2C,
		},
	},
	
	//SRS CAN
	{
		{HONDA_INIT_MODE_15},
		{MSG_Init_CAN_PackType_Honda},
		{0x41,0x01,0x07,0xa1,0x20,0x02,0x7f,0x03,0x10,0x04,0x98,0xda,0x53,0xf1,0x05,0x98,0xda,0xf1,0x53,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x03,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xe6,0x0D,0x01,0x0E,0x01,0x00,0x09,0x30,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x0a},
		{0x01},
		{ HONDA_ENTER_SYSTEM_MODE_2D,
		},
	},
	
	//SRS Honda
	{
		{HONDA_INIT_MODE_16},
		{MSG_Init_KWP_PackType_Honda},
		{0x2B,0x01,0x00,0x28,0xa0,0x02,0x00,0x03,0xc0,0x04,0x77,0x00,0xc1,0xc1,0x05,0x01,0x19,0x24,0x01,0x04,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x46,0x80,0x90,0x08,0x35,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x04,0x01,0x00},
		{0x04},
		{ HONDA_ENTER_SYSTEM_MODE_2E,
			HONDA_ENTER_SYSTEM_MODE_2F,
			HONDA_ENTER_SYSTEM_MODE_30,
			HONDA_ENTER_SYSTEM_MODE_31,
		},
	},
	
	//SRS Honda
	{
		{HONDA_INIT_MODE_17},
		{MSG_Init_KWP_PackType_Honda},
		{0x2B,0x01,0x00,0x25,0x80,0x02,0x00,0x03,0xc0,0x04,0x77,0xee,0x51,0x51,0x05,0x0d,0x19,0x24,0x01,0x04,0x00,0x19,0x06,0x03,0x07,0x00,0x08,0x25,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x04,0x01,0x00},
		{0x02},
		{ HONDA_ENTER_SYSTEM_MODE_32,
			HONDA_ENTER_SYSTEM_MODE_33,
		},
	},
	
};


const ENTER_SYSTEM_HONDA Enter_System_Honda[]=\
{
	//HONDA_ENTER_SYSTEM_MODE System_Enter_Mode;
	//uint8_t System_Enter_Honda[8];//进系统命令
	//uint8_t ECUID_CMD_Honda[8];//匹配ECUID的命令
	//uint8_t System_Exit_Honda[8];//退系统命令
	//uint8_t Honda_ECUID;//匹配的ECUID
	
	//Engine CAN
	{
		{HONDA_ENTER_SYSTEM_MODE_01},
		{0x03,0x22,0x26,0x00},
		{0x00,0x00},
		{0x00,0x00},
		{HONDA_ECUID_MODE_01},
	},
	
	//Engine CAN
	{
		{HONDA_ENTER_SYSTEM_MODE_02},
		{0x03,0x22,0x25,0x00},
		{0x00,0x00},
		{0x00,0x00},
		{HONDA_ECUID_MODE_12},
	},
	
	//Engine Honda INIT4/INIT5 1
	{
		{HONDA_ENTER_SYSTEM_MODE_03},
		{0x04,0x20,0x05,0x00,0x02},//ORD=0x23
		{0x04,0x20,0x05,0x78,0x05},
		{0x03,0x2e,0x04,0xf1},
		{HONDA_ECUID_MODE_03},//FI_
	},
	
	//Engine Honda INIT4/INIT5 2
	{
		{HONDA_ENTER_SYSTEM_MODE_04},
		{0x04,0x20,0x05,0x00,0x02},//ORD=0x23
		{0x05,0x25,0x06,0x80,0x4b,0x05},
		{0x03,0x2e,0x04,0xf1},
		{HONDA_ECUID_MODE_04},//DIE
	},
	
	//Engine Honda INIT6 1
	{
		{HONDA_ENTER_SYSTEM_MODE_05},
		{0x04,0x20,0x05,0x00,0x02},//ORD=0x23
		{0x06,0x25,0x07,0x72,0x00,0x00,0x05},
		{0x03,0x2e,0x04,0xf1},
		{HONDA_ECUID_MODE_05},//PFI
	},
	
	//Engine Honda INIT6 
	{
		{HONDA_ENTER_SYSTEM_MODE_06},
		{0x03,0xfe,0x04,0x31},//ORD=0x23
		{0x06,0x25,0x07,0x72,0x00,0x00,0x05},
		{0x03,0x2e,0x04,0xf1},
		{HONDA_ECUID_MODE_05},//PFI
	},
	
	//Engine Honda INIT6 
	{
		{HONDA_ENTER_SYSTEM_MODE_07},
		{0x03,0xfe,0x04,0x31},//ORD=0x23
		{0x05,0x25,0x06,0x80,0x4b,0x05},
		{0x03,0x2e,0x04,0xf1},
		{HONDA_ECUID_MODE_05},//PFI
	},
	
	//Engine Honda INIT6 
	{
		{HONDA_ENTER_SYSTEM_MODE_08},
		{0x03,0xfe,0x04,0x32},//ORD=0x23
		{0x06,0x25,0x07,0x72,0x00,0x00,0x05},
		{0x03,0x2e,0x04,0xf1},
		{HONDA_ECUID_MODE_05},//PFI
	},

	//Engine Honda INIT6 
	{
		{HONDA_ENTER_SYSTEM_MODE_09},
		{0x03,0xfe,0x04,0x32},//ORD=0x23
		{0x05,0x25,0x06,0x80,0x4b,0x05},
		{0x03,0x2e,0x04,0xf1},
		{HONDA_ECUID_MODE_05},//PFI
	},

	//Engine Honda INIT6 
	{
		{HONDA_ENTER_SYSTEM_MODE_0A},
		{0x03,0xfe,0x04,0x36},//ORD=0x23
		{0x06,0x25,0x07,0x72,0x00,0x00,0x05},
		{0x03,0x2e,0x04,0xf1},
		{HONDA_ECUID_MODE_05},//PFI
	},
	
	//Engine Honda INIT6 
	{
		{HONDA_ENTER_SYSTEM_MODE_0B},
		{0x03,0xfe,0x04,0x36},//ORD=0x23
		{0x05,0x25,0x06,0x80,0x4b,0x05},
		{0x03,0x2e,0x04,0xf1},
		{HONDA_ECUID_MODE_05},//PFI
	},
	
	//Engine Honda INIT6 
	{
		{HONDA_ENTER_SYSTEM_MODE_0C},
		{0x03,0xfe,0x04,0x3f},//ORD=0x23
		{0x06,0x25,0x07,0x72,0x00,0x00,0x05},
		{0x03,0x2e,0x04,0xf1},
		{HONDA_ECUID_MODE_05},//PFI
	},
	
	//Engine Honda INIT6 
	{
		{HONDA_ENTER_SYSTEM_MODE_0D},
		{0x03,0xfe,0x04,0x3f},//ORD=0x23
		{0x05,0x25,0x06,0x80,0x4b,0x05},
		{0x03,0x2e,0x04,0xf1},
		{HONDA_ECUID_MODE_05},//PFI
	},
	
	//Engine Honda INIT6 
	{
		{HONDA_ENTER_SYSTEM_MODE_0E},
		{0x03,0xfe,0x04,0x3c},//ORD=0x23
		{0x06,0x25,0x07,0x72,0x00,0x00,0x05},
		{0x03,0x2e,0x04,0xf1},
		{HONDA_ECUID_MODE_05},//PFI
	},
	
	//Engine Honda INIT6 
	{
		{HONDA_ENTER_SYSTEM_MODE_0F},
		{0x03,0xfe,0x04,0x3c},//ORD=0x23
		{0x05,0x25,0x06,0x80,0x4b,0x05},
		{0x03,0x2e,0x04,0xf1},
		{HONDA_ECUID_MODE_05},//PFI
	},
	
	//Engine Honda INIT6 
	{
		{HONDA_ENTER_SYSTEM_MODE_10},
		{0x03,0xfe,0x04,0x34},//ORD=0x23
		{0x06,0x25,0x07,0x8a,0x00,0x00,0x05},
		{0x03,0x2e,0x04,0xf1},
		{HONDA_ECUID_MODE_11},//PDI
	},
	
	//AT CAN  
	{
		{HONDA_ENTER_SYSTEM_MODE_11},
		{0x02,0x3e,0x00},
		{0x03,0x22,0x22,0x00},
		{0x00,0x00,},
		{HONDA_ECUID_MODE_13},//PFI
	},
	
	//AT CAN  
	{
		{HONDA_ENTER_SYSTEM_MODE_12},
		{0x02,0x3e,0x00},
		{0x03,0x22,0x23,0x00},
		{0x00,0x00,},
		{HONDA_ECUID_MODE_14},//PFI
	},
	
	//AT CAN  
	{
		{HONDA_ENTER_SYSTEM_MODE_13},
		{0x02,0x3e,0x00},
		{0x03,0x22,0x30,0x20},
		{0x00,0x00,},
		{HONDA_ECUID_MODE_15},//PFI
	},
	
	//AT CAN  
	{
		{HONDA_ENTER_SYSTEM_MODE_14},
		{0x02,0x3e,0x00},
		{0x03,0x22,0x24,0x00},
		{0x00,0x00,},
		{HONDA_ECUID_MODE_16},//PFI
	},
	
	//AT CAN  
	{
		{HONDA_ENTER_SYSTEM_MODE_15},
		{0x02,0x3e,0x00},
		{0x03,0x22,0x30,0x80},
		{0x00,0x00,},
		{HONDA_ECUID_MODE_17},//PFI
	},
	
	//AT CAN  
	{
		{HONDA_ENTER_SYSTEM_MODE_16},
		{0x03,0x22,0x22,0x00},
		{0x00,0x00,},
		{0x00,0x00,},
		{HONDA_ECUID_MODE_13},//PFI
	},
	
	//AT CAN  
	{
		{HONDA_ENTER_SYSTEM_MODE_17},
		{0x03,0x22,0x23,0x00},
		{0x00,0x00,},
		{0x00,0x00,},
		{HONDA_ECUID_MODE_14},//PFI
	},
	
	//AT Honda  
	{
		{HONDA_ENTER_SYSTEM_MODE_18},
		{0x04,0x20,0x05,0x00,0x02,},
		{0x04,0x48,0x05,0x78,0x05,},
		{0x03,0x2e,0x04,0xf1},
		{HONDA_ECUID_MODE_06},//CVT
	},
	
	//AT Honda  
	{
		{HONDA_ENTER_SYSTEM_MODE_19},
		{0x04,0x48,0x05,0x00,0x02,},
		{0x04,0x48,0x05,0x78,0x05,},
		{0x03,0x2e,0x04,0xf1},
		{HONDA_ECUID_MODE_06},//CVT
	},
	
	//AT Honda  
	{
		{HONDA_ENTER_SYSTEM_MODE_1A},
		{0x04,0x20,0x05,0x00,0x02,},
		{0x04,0x40,0x05,0x78,0x05,},
		{0x03,0x2e,0x04,0xf1},
		{HONDA_ECUID_MODE_07},//CVT
	},
	
	//AT Honda  
	{
		{HONDA_ENTER_SYSTEM_MODE_1B},
		{0x04,0x40,0x05,0x00,0x02,},
		{0x04,0x40,0x05,0x78,0x05,},
		{0x03,0x2e,0x04,0xf1},
		{HONDA_ECUID_MODE_07},//CVT
	},
	
	//AT Honda  
	{
		{HONDA_ENTER_SYSTEM_MODE_1C},
		{0x04,0x20,0x05,0x00,0x02,},
		{0x06,0x45,0x07,0x72,0x00,0x00,0x05},
		{0x03,0x2e,0x04,0xf1},
		{HONDA_ECUID_MODE_08},//CVT
	},
	
	//AT Honda  
	{
		{HONDA_ENTER_SYSTEM_MODE_1D},
		{0x03,0xfe,0x04,0x36,},
		{0x06,0x45,0x07,0x72,0x00,0x00,0x05},
		{0x03,0x2e,0x04,0xf1},
		{HONDA_ECUID_MODE_08},//CVT
	},
	
	//AT Honda  
	{
		{HONDA_ENTER_SYSTEM_MODE_1E},
		{0x04,0x45,0x05,0x71,0x00},
		{0x06,0x45,0x07,0x72,0x00,0x00,0x05},
		{0x03,0x2e,0x04,0xf1},
		{HONDA_ECUID_MODE_08},//CVT
	},
	
	//AT Honda  
	{
		{HONDA_ENTER_SYSTEM_MODE_1F},
		{0x03,0xfe,0x04,0x3f,},
		{0x06,0x45,0x07,0x72,0x00,0x00,0x05},
		{0x03,0x2e,0x04,0xf1},
		{HONDA_ECUID_MODE_08},//CVT
	},
	
	//AT Honda  
	{
		{HONDA_ENTER_SYSTEM_MODE_20},
		{0x03,0xfe,0x04,0x3c,},
		{0x06,0x45,0x07,0x72,0x00,0x00,0x05},
		{0x03,0x2e,0x04,0xf1},
		{HONDA_ECUID_MODE_08},//CVT
	},
	
	//AT Honda  
	{
		{HONDA_ENTER_SYSTEM_MODE_21},
		{0x04,0x20,0x05,0x00,0x02},
		{0x06,0x4d,0x07,0x72,0x00,0x00,0x05},
		{0x03,0x2e,0x04,0xf1},
		{HONDA_ECUID_MODE_19},//CVT
	},
	
	//AT Honda  
	{
		{HONDA_ENTER_SYSTEM_MODE_22},
		{0x03,0xfe,0x04,0x21,},
		{0x06,0x4d,0x07,0x72,0x00,0x00,0x05},
		{0x03,0x2e,0x04,0xf1},
		{HONDA_ECUID_MODE_19},//
	},
	
	//AT Honda  
	{
		{HONDA_ENTER_SYSTEM_MODE_23},
		{0x04,0x4d,0x05,0x71,0x00},
		{0x06,0x4d,0x07,0x72,0x00,0x00,0x05},
		{0x03,0x2e,0x04,0xf1},
		{HONDA_ECUID_MODE_19},//CVT
	},
	
	//AT Honda  
	{
		{HONDA_ENTER_SYSTEM_MODE_24},
		{0x03,0xfe,0x04,0x3f,},
		{0x06,0x4d,0x07,0x72,0x00,0x00,0x05},
		{0x03,0x2e,0x04,0xf1},
		{HONDA_ECUID_MODE_19},//CVT
	},
	
	//ABS CAN
	{
		{HONDA_ENTER_SYSTEM_MODE_25},
		{0x03,0x22,0x40,0x00,},
		{0x00,0x00,},
		{0x00,0x00,},
		{HONDA_ECUID_MODE_02},
	},
	
	//ABS Honda  
	{
		{HONDA_ENTER_SYSTEM_MODE_26},
		{0x03,0xa0,0x05,0x00,0x01},
		{0x06,0xa0,0x05,0x70,0x05},
		{0x03,0x2e,0x04,0xf1},
		{HONDA_ECUID_MODE_09},
	},
	
	//ABS Honda  
	{
		{HONDA_ENTER_SYSTEM_MODE_27},
		{0x03,0xfe,0x04,0x91,},
		{0x06,0xa2,0x07,0x72,0x00,0x00,0x05},
		{0x03,0x2e,0x04,0xf1},
		{HONDA_ECUID_MODE_0A},
	},
	
	//ABS Honda  
	{
		{HONDA_ENTER_SYSTEM_MODE_28},
		{0x03,0xfe,0x04,0x91,},
		{0x06,0xa0,0x05,0x70,0x05,},
		{0x03,0x2e,0x04,0xf1},
		{HONDA_ECUID_MODE_09},
	},
	
	//BODY KWP10400 注意要分13/10/18/17共4个id
	{
		{HONDA_ENTER_SYSTEM_MODE_29},
		{0x01,0x81,},
		{0x03,0x21,0x00,0x01,},
		{0x01,0x82,},
		{HONDA_ECUID_MODE_0B},
	},
	
	//BODY KWP10400 注意要分13/10/18/17共4个id
	{
		{HONDA_ENTER_SYSTEM_MODE_2A},
		{0x01,0x81,},
		{0x03,0x21,0x00,0x01,},
		{0x01,0x82,},
		{HONDA_ECUID_MODE_0B},
	},
	
	//BODY KWP10400 注意要分13/10/18/17共4个id
	{
		{HONDA_ENTER_SYSTEM_MODE_2B},
		{0x01,0x81,},
		{0x03,0x21,0x00,0x01,},
		{0x01,0x82,},
		{HONDA_ECUID_MODE_0B},
	},
	
	//BODY KWP10400 注意要分13/10/18/17共4个id
	{
		{HONDA_ENTER_SYSTEM_MODE_2C},
		{0x01,0x81,},
		{0x03,0x21,0x00,0x01,},
		{0x01,0x82,},
		{HONDA_ECUID_MODE_0B},
	},

	//SRS CAN
	{
		{HONDA_ENTER_SYSTEM_MODE_2D},
		{0x03,0x22,0x80,0x00},
		{0x00,0x00,},
		{0x00,0x00,},
		{HONDA_ECUID_MODE_18},
	},
	
	//ABS Honda  
	{
		{HONDA_ENTER_SYSTEM_MODE_2E},
		{0x04,0x60,0x05,0x70,0x02,},
		{0x04,0x60,0x05,0x70,0x0c,},
		{0x03,0x6e,0x04,0xf1},
		{HONDA_ECUID_MODE_0C},
	},
	
	//ABS Honda  
	{
		{HONDA_ENTER_SYSTEM_MODE_2F},
		{0x04,0x60,0x05,0x70,0x02,},
		{0x04,0x60,0x05,0x70,0x0f,},
		{0x03,0x6e,0x04,0xf1},
		{HONDA_ECUID_MODE_0D},
	},
	
	//ABS Honda  
	{
		{HONDA_ENTER_SYSTEM_MODE_30},
		{0x03,0xfe,0x04,0x71,},
		{0x04,0x60,0x05,0x70,0x0c,},
		{0x03,0x6e,0x04,0xf1},
		{HONDA_ECUID_MODE_0C},
	},
	
	//ABS Honda  
	{
		{HONDA_ENTER_SYSTEM_MODE_31},
		{0x03,0xfe,0x04,0x71,},
		{0x04,0x60,0x05,0x70,0x0f,},
		{0x03,0x6e,0x04,0xf1},
		{HONDA_ECUID_MODE_0D},
	},
	
	//ABS Honda  
	{
		{HONDA_ENTER_SYSTEM_MODE_32},
		{0x04,0x60,0x05,0x70,0x02,},
		{0x04,0x60,0x05,0x70,0x05,},
		{0x03,0x6e,0x04,0xf1},
		{HONDA_ECUID_MODE_0E},
	},
	
	//ABS Honda  
	{
		{HONDA_ENTER_SYSTEM_MODE_33},
		{0x04,0x60,0x05,0x70,0x02,},
		{0x04,0x60,0x05,0x70,0x0c,},
		{0x03,0x6e,0x04,0xf1},
		{HONDA_ECUID_MODE_0C},
	},
	
	
};



const HONDA_ECUID Honda_EcuId[]=\
{
//	uint8_t ECUID_Honda;//匹配的ECUID
//	uint8_t CMD_Read_DTC_Size;//读码命令个数
//	uint8_t CMD_Read_DTC[8][8];//读码命令 最多有8条读码命令
//	uint8_t DCT_Type[8];//和读码命令对应的读码的方式
//	uint8_t DCT_Pos[8];//和读码命令对应的取码号的Pos位
//	uint8_t CMD_Clear_DTC_Size;//清码命令个数
//	uint8_t CMD_Clear_DTC[8][8];//清码命令
	
	//Engine CAN CANFI/CANDIE
	//AT CAN CANEAT/CANCVT/CANDCT/DCT/CAN9AT
	{
		{HONDA_ECUID_MODE_01},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x03,0x19,0x02,0x08,0x00,0x00,0x00,0x00},
		},
		{0x27,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//DCT_Type
		{0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//DCT_Pos
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x02,0xA4,0x10,0x00,0x00,0x00,0x00,0x00},
		},
	},
	
	//ABS CAN 
	{
		{HONDA_ECUID_MODE_02},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x03,0x19,0x02,0x08,0x00,0x00,0x00,0x00},
		},
		{0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//DCT_Type
		{0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//DCT_Pos
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x04,0x14,0xff,0xff,0xff,0x00,0x00,0x00},
		},
	},
	
	//Engine honda FI_
	{
		{HONDA_ECUID_MODE_03},
		{0x02},//CMD_Read_DTC_Size
		{
			{0x04,0x20,0x05,0x40,0x10,0x00,0x00,0x00},
			{0x04,0x20,0x05,0x50,0x10,0x00,0x00,0x00},
		},
		{0x08,0x08,0x00,0x00,0x00,0x00,0x00,0x00},//DCT_Type
		{0x02,0x02,0x00,0x00,0x00,0x00,0x00,0x00},//DCT_Pos
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x03,0x21,0x04,0x01,0x00,0x00,0x00,0x00},
		},
	},
	
	//Engine honda DIE
	{
		{HONDA_ECUID_MODE_04},
		{0x04},//CMD_Read_DTC_Size
		{
			{0x05,0x25,0x06,0x80,0x50,0x10,0x00,0x00},
			{0x05,0x25,0x06,0x80,0x60,0x0d,0x00,0x00},
			{0x05,0x25,0x06,0x80,0x70,0x10,0x00,0x00},
			{0x05,0x25,0x06,0x80,0x85,0x0a,0x00,0x00},
		},
		{0x08,0x08,0x08,0x08,0x00,0x00,0x00,0x00},//DCT_Type
		{0x02,0x02,0x02,0x02,0x00,0x00,0x00,0x00},//DCT_Pos
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x03,0x21,0x04,0x01,0x00,0x00,0x00,0x00},
		},
	},
	
	//Engine honda PFI
	{
		{HONDA_ECUID_MODE_05},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x04,0x25,0x05,0x73,0x01},
		},
		{0x11,},//DCT_Type
		{0x05,},//DCT_Pos
		{0x03},//CMD_Clear_DTC_Size
		{
			{0x03,0x21,0x04,0x01,0x00,0x00,0x00,0x00},
			{0x04,0x8c,0x05,0x01,0x10,},
			{0x04,0x2c,0x05,0x01,0x10,},
		},
	},
	
	//AT honda CVT
	{
		{HONDA_ECUID_MODE_06},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x04,0x48,0x05,0x20,0x07},
		},
		{0x02,},//DCT_Type
		{0x02,},//DCT_Pos
		{0x02},//CMD_Clear_DTC_Size
		{
			{0x04,0x8c,0x05,0x01,0x00,},
			{0x03,0x41,0x04,0x01,},
		},
	},
	
	//AT honda EAT
	{
		{HONDA_ECUID_MODE_07},
		{0x02},//CMD_Read_DTC_Size
		{
			{0x04,0x40,0x05,0x14,0x0c},
			{0x04,0x40,0x05,0x40,0x0c},
		},
		{0x02,0x02,},//DCT_Type
		{0x02,0x02,},//DCT_Pos
		{0x02},//CMD_Clear_DTC_Size
		{
			{0x04,0x8c,0x05,0x01,0x00,},
			{0x03,0x41,0x04,0x01,},
		},
	},
	
	//AT honda PAT
	{
		{HONDA_ECUID_MODE_08},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x04,0x45,0x05,0x73,0x01},

		},
		{0x11,},//DCT_Type
		{0x05,},//DCT_Pos
		{0x04},//CMD_Clear_DTC_Size
		{
			{0x04,0x8c,0x05,0x01,0x00,},
			{0x04,0x8c,0x05,0x01,0x10,},
			{0x03,0x41,0x04,0x01,},
			{0x03,0x41,0x04,0x30,},
		},
	},
	
	//ABS honda ABSNK12/ABT
	{
		{HONDA_ECUID_MODE_09},
		{0x02},//CMD_Read_DTC_Size
		{
			{0x04,0xa0,0x05,0x00,0x0f},
			{0x04,0xa0,0x05,0x80,0x0d},
		},
		{0x02,0x02,},//DCT_Type
		{0x02,0x02,},//DCT_Pos
		{0x05},//CMD_Clear_DTC_Size
		{
			{0x04,0xa0,0x05,0x30,0x01,},
			{0x04,0xa0,0x05,0x31,0x01,},
			{0x04,0xa0,0x05,0x32,0x01,},
			{0x04,0xa0,0x05,0x33,0x01,},
			{0x03,0xa1,0x04,0x01,},
		},
	},
	
	//ABS honda ABI
	{
		{HONDA_ECUID_MODE_0A},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x04,0xa2,0x05,0x73,0x01},
		},
		{0x11,},//DCT_Type
		{0x05,},//DCT_Pos
		{0x05},//CMD_Clear_DTC_Size
		{
			{0x06,0xa2,0x07,0x72,0x10,0x00,0x01,},
			{0x06,0xa2,0x07,0x72,0x10,0x01,0x01,},
			{0x06,0xa2,0x07,0x72,0x10,0x02,0x01,},
			{0x06,0xa2,0x07,0x72,0x10,0x03,0x01,},
			{0x03,0xa1,0x04,0x01,},
		},
	},
	
	//BODY KWP10400 
	{
		{HONDA_ECUID_MODE_0B},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x02,0x13,0x00,}
		},
		{0x04,},//DCT_Type
		{0x02,},//DCT_Pos
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x02,0x14,0x02,},
		},
	},
	
	//SRS Honda SRS03
	{
		{HONDA_ECUID_MODE_0C},
		{0x04},//CMD_Read_DTC_Size
		{
			{0x04,0x60,0x05,0x08,0x02,},
			{0x04,0x60,0x05,0x0a,0x02,},
			{0x04,0x60,0x05,0x0c,0x02,},
			{0x04,0x60,0x05,0x80,0x09,}
		},
		{0x01,0x01,0x01,0x07,},//DCT_Type
		{0x02,0x02,0x02,0x03,},//DCT_Pos
		{0x02},//CMD_Clear_DTC_Size
		{
			{0x03,0x61,0x04,0x01},
			{0x03,0x61,0x04,0x11},
		},
	},
	
	//SRS Honda SRS06
	{
		{HONDA_ECUID_MODE_0D},
		{0x06},//CMD_Read_DTC_Size
		{
			{0x04,0x60,0x05,0x08,0x02,},
			{0x04,0x60,0x05,0x0a,0x02,},
			{0x04,0x60,0x05,0x0c,0x02,},
			{0x04,0x60,0x05,0x80,0x04,},
			{0x04,0x60,0x05,0x90,0x05,},
			{0x04,0x60,0x05,0x80,0x0a,},
		},
		{0x01,0x01,0x01,0x07,0x09,0x07,},//DCT_Type
		{0x02,0x02,0x02,0x05,0x05,0x02,},//DCT_Pos
		{0x02},//CMD_Clear_DTC_Size
		{
			{0x03,0x61,0x04,0x01},
			{0x03,0x61,0x04,0x11},
		},
	},
	
	//SRS Honda SRS94
	{
		{HONDA_ECUID_MODE_0E},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x04,0x60,0x05,0x04,0x0a,},
		},
		{0x08,},//DCT_Type
		{0x02,},//DCT_Pos
		{0x02},//CMD_Clear_DTC_Size
		{
			{0x03,0x61,0x04,0x01},
			{0x03,0x61,0x04,0x11},
		},
	},
	
	//SRS Honda SRS96
	{
		{HONDA_ECUID_MODE_0F},
		{0x02},//CMD_Read_DTC_Size
		{
			{0x04,0x60,0x05,0x10,0x09,},
			{0x04,0x60,0x05,0x20,0x01,},
		},
		{0x02,0x02},//DCT_Type
		{0x02,0x02},//DCT_Pos
		{0x02},//CMD_Clear_DTC_Size
		{
			{0x03,0x61,0x04,0x01},
			{0x03,0x61,0x04,0x11},
		},
	},
	
	//SRS Honda SRS99
	{
		{HONDA_ECUID_MODE_10},
		{0x08},//CMD_Read_DTC_Size
		{
			{0x04,0x60,0x05,0x04,0x04,},
			{0x04,0x60,0x05,0x10,0x09,},
			{0x04,0x60,0x05,0x20,0x01,},
			{0x04,0x60,0x05,0x50,0x01,},
			{0x04,0x60,0x05,0x70,0x10,},
			{0x04,0x60,0x05,0x08,0x01,},
			{0x04,0x60,0x05,0x0a,0x01,},
			{0x04,0x60,0x05,0x0c,0x01,},
		},
		{0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,},//DCT_Type
		{0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,},//DCT_Pos
		{0x02},//CMD_Clear_DTC_Size
		{
			{0x03,0x61,0x04,0x01},
			{0x03,0x61,0x04,0x11},
		},
	},
	
	//Engine honda PDI 和PFI命令都一样，只是码库不一样
	{
		{HONDA_ECUID_MODE_11},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x04,0x25,0x05,0x73,0x01},
		},
		{0x11,},//DCT_Type
		{0x05,},//DCT_Pos
		{0x03},//CMD_Clear_DTC_Size
		{
			{0x03,0x21,0x04,0x01,0x00,0x00,0x00,0x00},
			{0x04,0x8c,0x05,0x01,0x10,},
			{0x04,0x2c,0x05,0x01,0x10,},
		},
	},
	
	//Engine CAN CANDIE  命令和CANFI一样，就是码库不一样
	//AT CAN CANEAT/CANCVT/CANDCT/DCT/CAN9AT
	{
		{HONDA_ECUID_MODE_12},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x03,0x19,0x02,0x08,0x00,0x00,0x00,0x00},
		},
		{0x27,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//DCT_Type
		{0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//DCT_Pos
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x02,0xA4,0x10,0x00,0x00,0x00,0x00,0x00},
		},
	},
	
	//AT CAN CANEAT
	{
		{HONDA_ECUID_MODE_13},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x03,0x19,0x02,0x08,0x00,0x00,0x00,0x00},
		},
		{0x27,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//DCT_Type
		{0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//DCT_Pos
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x02,0xA4,0x10,0x00,0x00,0x00,0x00,0x00},
		},
	},
	
	//AT CAN CANCVT
	{
		{HONDA_ECUID_MODE_14},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x03,0x19,0x02,0x08,0x00,0x00,0x00,0x00},
		},
		{0x27,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//DCT_Type
		{0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//DCT_Pos
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x02,0xA4,0x10,0x00,0x00,0x00,0x00,0x00},
		},
	},
	
	//AT CAN CANDCT
	{
		{HONDA_ECUID_MODE_15},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x03,0x19,0x02,0x08,0x00,0x00,0x00,0x00},
		},
		{0x27,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//DCT_Type
		{0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//DCT_Pos
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x02,0xA4,0x10,0x00,0x00,0x00,0x00,0x00},
		},
	},
	
	//AT CAN DCT
	{
		{HONDA_ECUID_MODE_16},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x03,0x19,0x02,0x08,0x00,0x00,0x00,0x00},
		},
		{0x27,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//DCT_Type
		{0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//DCT_Pos
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x02,0xA4,0x10,0x00,0x00,0x00,0x00,0x00},
		},
	},
	
	//AT CAN CAN9AT
	{
		{HONDA_ECUID_MODE_17},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x03,0x19,0x02,0x08,0x00,0x00,0x00,0x00},
		},
		{0x27,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//DCT_Type
		{0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//DCT_Pos
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x02,0xA4,0x10,0x00,0x00,0x00,0x00,0x00},
		},
	},
	
	//SRS CAN 
	{
		{HONDA_ECUID_MODE_18},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x03,0x19,0x02,0x08,0x00,0x00,0x00,0x00},
		},
		{0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//DCT_Type
		{0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//DCT_Pos
		{0x01},//CMD_Clear_DTC_Size
		{
			{0x04,0x14,0xff,0xff,0xff,0x00,0x00,0x00},
		},
	},
	
	//AT honda PVT
	{
		{HONDA_ECUID_MODE_19},
		{0x01},//CMD_Read_DTC_Size
		{
			{0x04,0x4d,0x05,0x73,0x01},

		},
		{0x11,},//DCT_Type
		{0x05,},//DCT_Pos
		{0x04},//CMD_Clear_DTC_Size
		{
			{0x04,0x8c,0x05,0x01,0x00,},
			{0x04,0x8c,0x05,0x01,0x10,},
			{0x03,0x41,0x04,0x01,},
			{0x03,0x41,0x04,0x30,},
		},
	},
	
	
};





//// ECU INIT
////const OBDII_CMD_INIT InitEcuCmd_Toyota[]=\
////{
////	{
////		{TOYOTA_MODE_01},
////		{0},
////		{0}
////	},
////	{
////		{TOYOTA_MODE_02},
////		{0},
////		{0}
////	},
////	{
////		{TOYOTA_MODE_03},
////		{MSG_Init_Ecu_KWP_PackType_Toyota},
////		{01,0x81}
////	},
////	{
////		{TOYOTA_MODE_04},
////		{MSG_Init_Ecu_KWP_PackType_Toyota},
////		{01,0x81}
////	},
////	{
////		{TOYOTA_MODE_05},
////		{MSG_Init_Ecu_KWP_PackType_Toyota},
////		{01,0x81}
////	}
////};

////ECU select
////const OBDII_CMD_INIT EcuSelectCmd_Toyota[]=\
////{
////	{
////		{TOYOTA_MODE_01},
////		{MSG_ALL_CAN_PackType_Toyota},
////		{0x01,0x3e}
////	},
////	{
////		{TOYOTA_MODE_02},
////		{MSG_ALL_CAN_PackType_Toyota},
////		{0x02,0x3e,0x00}
////	},
////	{
////		{TOYOTA_MODE_03},
////		{MSG_ALL_KWP_PackType_Toyota},
////		{0x02,0x01,0x00}
////	},
////	{
////		{TOYOTA_MODE_04},
////		{MSG_ALL_KWP_PackType_Toyota},
////		{0x02,0x01,0x00}
////	},
////	{
////		{TOYOTA_MODE_05},
////		{MSG_ALL_KWP_PackType_Toyota},
////		{0x02,0x01,0x00}
////	},

////};


//公共函数
bool SetFrameTimeOut_Honda(HONDA_INIT_MODE index)
{
	uint8_t  ord;
	uint8_t initdata[128];
	uint8_t number,i=0;
	uint32_t canid;
	m_CAN_ANS_T_Honda = 0;
	m_CAN_ANS_R_Honda = 0;
	
	ord = InitIO_Honda[index].Msg_Ord;
	number = InitIO_Honda[index].CMD_DATA[0];
	memcpy(initdata,InitIO_Honda[index].CMD_DATA+1,number);
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
				//m_CAN_LISTEN_TYPE_Toyota =initdata[i+1];
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
				m_CAN_ANS_T_Honda = initdata[i+1];
				i+=2;
				break;
			case 0x0e:
				m_CAN_ANS_R_Honda = initdata[i+1];
				i+=0x11;
				break;
			case 0x0F:
				i+=2;
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




//设置通信过程ORD
void pro_msg_all_Honda(HONDA_INIT_MODE mode)
{
	switch(mode)
	{
		//CAN
		case HONDA_INIT_MODE_01:
		case HONDA_INIT_MODE_02:
		case HONDA_INIT_MODE_03:
		case HONDA_INIT_MODE_07:
		case HONDA_INIT_MODE_08:
		case HONDA_INIT_MODE_09:
		case HONDA_INIT_MODE_0A:
		case HONDA_INIT_MODE_0B:	
		case HONDA_INIT_MODE_0C:	
		case HONDA_INIT_MODE_0F:
		case HONDA_INIT_MODE_15:
			
			g_defaultMsgAttr.m_CS_MODE = 0;
			g_defaultMsgAttr.m_ORD     = 0x30;
			g_defaultMsgAttr.m_PCK_TYPE= 0x20;
			g_defaultMsgAttr.m_RWAYS   = 0;
			g_defaultMsgAttr.m_SRC     = 0;
			g_defaultMsgAttr.m_TGT     = 0;
			g_defaultMsgAttr.m_WAYS    = 0;
		break;
		
		//KWP 10400
		case HONDA_INIT_MODE_14:
			g_defaultMsgAttr.m_CS_MODE = 0;
			g_defaultMsgAttr.m_ORD     = 0x33;
			g_defaultMsgAttr.m_PCK_TYPE= 0x04;
			g_defaultMsgAttr.m_RWAYS   = 0;
			g_defaultMsgAttr.m_SRC     = 0xF0;
			g_defaultMsgAttr.m_TGT     = 0x13;
			g_defaultMsgAttr.m_WAYS    = 0;
		break;
	
		//本田协议
		case HONDA_INIT_MODE_04:
		case HONDA_INIT_MODE_05:
		case HONDA_INIT_MODE_06:
		case HONDA_INIT_MODE_0D:
		case HONDA_INIT_MODE_0E:
		case HONDA_INIT_MODE_10:
		case HONDA_INIT_MODE_11:
		case HONDA_INIT_MODE_12:
		case HONDA_INIT_MODE_13:
		case HONDA_INIT_MODE_16:
		case HONDA_INIT_MODE_17:
			
			g_defaultMsgAttr.m_CS_MODE = 0;
			g_defaultMsgAttr.m_ORD     = 0x33;
			g_defaultMsgAttr.m_PCK_TYPE= 0x0f;
			g_defaultMsgAttr.m_RWAYS   = 0;
			g_defaultMsgAttr.m_SRC     = 0;
			g_defaultMsgAttr.m_TGT     = 0;
			g_defaultMsgAttr.m_WAYS    = 0;
		break;
		
		default:
		break;
	}
}

////直接用OBD的
////void ResetComPort()



//初始化IO
bool Init_IO_Honda(HONDA_INIT_MODE index)
{
	uint16_t number = 0;
	HONDA_INIT_MODE mode = 0;
	uint8_t sendbuf[128];
	uint8_t recvbuf[32];
	if(!SetFrameTimeOut_Honda(index))
	{
		return false;
	}
	memset(recvbuf,0,sizeof(recvbuf));
	number = InitIO_Honda[index].CMD_DATA[0];
	memcpy(sendbuf, InitIO_Honda[index].CMD_DATA+1, number);
	mode = InitIO_Honda[index].Init_Mode;
	//g_defaultMsgAttr.m_PCK_TYPE = 0x00;
	number = pro_packsenddata(sendbuf, number, InitIO_Honda[index].Msg_Ord);
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
	vTaskDelay(1000);
	//pro_set_can_id_Toyota(index);
	printfUartObdiideBug("InitIO true\r\n");
	return true;
}


//void pro_set_can_id_Toyota(INIT_MODE_TOYOTA mode)
//{
//	switch(mode)
//	{
//		case TOYOTA_MODE_01:
//				m_canbussendid = 0x000007E0;
//				m_canbusreadid = 0x000007E8;
//				m_canbusflowid = 0x000007E0;
//			break;
//		
//		case TOYOTA_MODE_02:
//				m_canbussendid = 0x00000700;
//				m_canbusreadid = 0x000007E8;
//				m_canbusflowid = 0x00000700;
//			break;

//		default:
//			break;
//	}
//}

intptr_t pro_much_init_honda(HONDA_INIT_MODE Index,HONDA_ENTER_SYSTEM_MODE mode)// 进系统
{
	bool bRet = false;
	MSG_ORD_HONDA PackType = InitIO_Honda[Index].Msg_Ord;
	uint8_t iEnter = 0;
	
	pro_msg_all_Honda(Index);
	if(mode == HONDA_ENTER_SYSTEM_MODE_NULL)//第一次进的时候
	{
		for	(iEnter = 0; iEnter < InitIO_Honda[Index].enter_size; iEnter++)
		{
			//同一个初始化IO的命令的不同进系统命令
			mode = InitIO_Honda[Index].Enter_System_mode[iEnter];//获取具体的是哪个进系统的命令的mode
			switch(PackType)
			{
				//CAN
				case MSG_Init_CAN_PackType_Honda:
					bRet = pro_canbus_select_module_honda(mode);
					break;
				
				//K-Line
				case MSG_Init_KWP_PackType_Honda:
					if(g_defaultMsgAttr.m_PCK_TYPE == 0x0f)//本田协议
					{
						bRet = pro_honda_select_module_honda(mode);//
					}
					else if(g_defaultMsgAttr.m_PCK_TYPE == 0x04)//KWP2000 10400bps
					{
						bRet = pro_kwp_select_module_honda(mode);//kwp的81进系统命令
					}
					else
					{
						;//其他协议未处理
					}
					break;

				default:
					break;
			}
			if(bRet)
			{
				return mode;
			}
		}
	}
	else
	{
			switch(PackType)
			{
				//CAN
				case MSG_Init_CAN_PackType_Honda:
					bRet = pro_canbus_select_module_honda(mode);
					break;
				
				//K-Line
				case MSG_Init_KWP_PackType_Honda:
					//if(InitIO_Honda[Index].CMD_DATA[41] == 0x04)//本田协议
					if(g_defaultMsgAttr.m_PCK_TYPE == 0x0f)//本田协议
					{
						bRet = pro_honda_select_module_honda(mode);//
					}
					//else if(InitIO_Honda[Index].CMD_DATA[41] == 0x01)//KWP2000 10400bps
					else if(g_defaultMsgAttr.m_PCK_TYPE == 0x04)//KWP2000 10400bps
					{
						bRet = pro_kwp_select_module_honda(mode);//kwp的81进系统命令
					}
					else
					{
						;//其他协议未处理
					}
					break;

				default:
					break;
			}
			if(bRet)
			{
				return mode;
			}
	}
	
	return -1;
}

bool pro_honda_select_module_honda(HONDA_ENTER_SYSTEM_MODE mode)//本田协议进系统
{
	bool bRet = false;
	
	//进系统命令
	bRet = pro_enter_system_Honda(mode);
	if(!bRet)
	{
		return bRet;
	}

	//匹配型号命令
	bRet = pro_init_ECU_id_Honda(mode);
	return bRet;
}

bool pro_kwp_select_module_honda(HONDA_ENTER_SYSTEM_MODE mode)//kwp的81进系统命令
{
	uint16_t number = 0;
	uint8_t sendbuf[128] = {0};
	uint8_t recvbuf[256] = {0};
	uint8_t ord = 0;
	uint8_t TGT = 0;
	uint8_t PCK_TYPE = 0;
	
	//进系统命令
	memcpy(sendbuf,Enter_System_Honda[mode].System_Enter,Enter_System_Honda[mode].System_Enter[0]+1);
	
	ord = g_defaultMsgAttr.m_ORD;//临时保存
	TGT = g_defaultMsgAttr.m_TGT;//临时保存
	PCK_TYPE = g_defaultMsgAttr.m_PCK_TYPE;//临时保存

	g_defaultMsgAttr.m_ORD = 0x23;//进系统 临时改成0x23
	g_defaultMsgAttr.m_TGT = 0x46;//进系统 临时改成0x46
	g_defaultMsgAttr.m_PCK_TYPE = 0x02;//进系统 临时改成0x02
	
	number = pro_SendRecv_Honda(sendbuf, recvbuf);
	
	g_defaultMsgAttr.m_ORD = ord;//还原回去
	g_defaultMsgAttr.m_TGT = TGT;//还原回去
	g_defaultMsgAttr.m_PCK_TYPE = PCK_TYPE;//还原回去
	
	if(number == 0)
	{
		return false;
	}
	
	
	//匹配型号命令
	memcpy(sendbuf,Enter_System_Honda[mode].ECUID_CMD,Enter_System_Honda[mode].ECUID_CMD[0]+1);
	
	//这段没有写到命令里面，直接写到了代码判断里面
	if(HONDA_ENTER_SYSTEM_MODE_29 == mode)
	{
		g_defaultMsgAttr.m_TGT     = 0x13;
	}
	else if(HONDA_ENTER_SYSTEM_MODE_2A == mode)
	{
		g_defaultMsgAttr.m_TGT     = 0x10;
	}
	else if(HONDA_ENTER_SYSTEM_MODE_2B == mode)
	{
		g_defaultMsgAttr.m_TGT     = 0x18;
	}
	else if(HONDA_ENTER_SYSTEM_MODE_2C == mode)
	{
		g_defaultMsgAttr.m_TGT     = 0x17;
	}
	else
	{
		;//不变
	}
	
	number = pro_SendRecv_Honda(sendbuf, recvbuf);

	if(number == 0)
	{
		return false;
	}

	return true;
}

bool pro_canbus_select_module_honda(HONDA_ENTER_SYSTEM_MODE mode)
{
//	int i, j;
	uint8_t sendbuf[128];
	uint8_t recvbuf[256];
//	uint8_t sendnum =0;
	uint16_t number=0;
//	uint32_t canid;
	
	memset(sendbuf,0,sizeof(sendbuf));
	memcpy(sendbuf, Enter_System_Honda[mode].System_Enter,Enter_System_Honda[mode].System_Enter[0]+1);
	
	number = pro_SendRecv_Honda(sendbuf, recvbuf); //进系统 带长度
	if(number == 0)
	{
		return false;
	}
	
	if(Enter_System_Honda[mode].ECUID_CMD[0]>0)
	{
		memset(sendbuf,0,sizeof(sendbuf));
		memcpy(sendbuf, Enter_System_Honda[mode].ECUID_CMD,Enter_System_Honda[mode].ECUID_CMD[0]+1);
	
		number = pro_SendRecv_Honda(sendbuf, recvbuf); //进系统 带长度
	
		if(number == 0)
		{
			return false;
		}
	}
	
	return true;
}



void pro_read_dtc_Honda(HONDA_ENTER_SYSTEM_MODE enter_system_mode,uint8_t iSystem,int* dtc_number,char* dtc_list,char* dtc_sys) //读码
{
	uint8_t sendcmd[8] = {0}; //读当前码
	uint8_t recvbuf[128] = {0}; //
	uint8_t number = 0;
	uint8_t bitnumber = 0;
	uint8_t dtcnumber = 0;
	uint8_t ndtcnumber = 0;
	bool bFlag = TRUE;
	char dtc[16] = {0};
	char dtcname[32][16] = {0};//最多32个码
	int i = 0,j = 0;
	int total = 0;
	int dtccmd_size = 0;
	uint8_t pos = 0;
	char system[4] = {0};
	uint8_t msgno=0;
	uint8_t Bit[50] = 0;
	uint8_t KWD = 0;
	char EcuId[3] = {0};
	HONDA_ECUID_MODE ECUID_mode = Enter_System_Honda[enter_system_mode].Honda_ECUID;
	
	//
	uint8_t FEATURE[10] = {0};
	uint8_t FEATURE_SRS03[10] = {0x81,0x82,0x83,0x84,0x85,0x86,0x00,0x88};
	uint8_t FEATURE_SRS06[10] = {0x81,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89};
	uint8_t FEATURE_SRS06_TYPE09[10]={0xD1,0xD2};
	uint8_t FEATURE_size = 0;
	
	//TYPE="07"(27)用到
	uint8_t feature = 0;
	if(HONDA_ECUID_MODE_01 == ECUID_mode)//CANFI
	{
		feature = 0;
	}
	else if(HONDA_ECUID_MODE_12 == ECUID_mode)//CANDIE
	{
		feature = 1;
	}
	else if(HONDA_ECUID_MODE_13 == ECUID_mode)//CANEAT
	{
		feature = 2;
	}
	else if(HONDA_ECUID_MODE_14 == ECUID_mode)//CANCVT
	{
		feature = 2;
	}
	else if(HONDA_ECUID_MODE_15 == ECUID_mode)//CANDCT
	{
		feature = 2;
	}
	else if(HONDA_ECUID_MODE_16 == ECUID_mode)//CVT
	{
		feature = 2;
	}
	else if(HONDA_ECUID_MODE_17 == ECUID_mode)//CAN9AT
	{
		feature = 3;
	}
	else
	{
		feature = 0;
	}

	//HONDA_ECUID_MODE ECUID_Honda_Mode = Enter_System_Honda[index].Honda_ECUID;

	sprintf(system, "%d", iSystem);//系统
	sprintf(EcuId, "%02X", ECUID_mode);//由于不同id的故障码库不一样，所以在故障码编号前面添加了这个
		
	//SDATA都为3

	//多个读码命令
	for(dtccmd_size = 0 ;dtccmd_size < Honda_EcuId[ECUID_mode].CMD_Read_DTC_Size; dtccmd_size++)
	{
		memcpy(sendcmd, Honda_EcuId[ECUID_mode].CMD_Read_DTC[dtccmd_size], Honda_EcuId[ECUID_mode].CMD_Read_DTC[dtccmd_size][0]+1);

		pos = Honda_EcuId[ECUID_mode].DTC_Pos[dtccmd_size];
		switch(Honda_EcuId[ECUID_mode].DTC_Type[dtccmd_size])
		{
			case 0x00:
				number = pro_SendRecv_Honda(sendcmd, recvbuf);
				break;//不做处理
			
			case 0x01:
				//pro_honda_read_dtc()
				//POS位开始，两位为一个故障码
				number = pro_SendRecv_Honda(sendcmd, recvbuf);
				for(i=pos;i<number - 2;i+=2)
				{
					if(recvbuf[i]|recvbuf[i+1])
					{
						sprintf((char *)dtc,"X%02X%02X",recvbuf[i],recvbuf[i+1]);
						sprintf((char *)dtcname[ndtcnumber],"X%02X%02X",recvbuf[i],recvbuf[i+1]); // 把故障码存起来
						
						bFlag = TRUE;
						for (total=0; total<ndtcnumber; total++)//比较故障码是否重复
						{
							if (strcmp((char*)dtc,(char*)dtcname[total]) == 0)
							{
								bFlag = FALSE;//此故障码重复
								break;
							}
						}
						if(bFlag)//不重复的添加进去
						{
							ndtcnumber++;
							if(*dtc_number != 0)
							{//60个字节，用","分隔
								strcat(dtc_list,",");
								strcat(dtc_sys,",");
							}
							strcat(dtc_list,EcuId);//由于不同id的故障码库不一样，所以在故障码编号前面添加了这个
							strcat(dtc_list,dtc);
							strcat(dtc_sys,system);
							(*dtc_number)++;
							printfUartObdiideBug("honda add dtc:%s system:%s \r\n",dtc,system);
							printfUartObdiideBug("honda dtc_list:%s dtc_number:%d \r\n",dtc_list,*dtc_number);
						}
					}
				}
				break;//
			
			case 0x02://02和08是一样的
			case 0x08://FI_型号
				//KWD,LENTH pro_honda_read_dtc()
				//按照位计算
				number = pro_SendRecv_Honda(sendcmd, recvbuf);
				if (number==0)
				{
					break;
				}
				for	(i = pos;i<number;i++)
				{
					bitnumber = GetBitDtc(recvbuf,pos,Bit);
					KWD = sendcmd[sendcmd[0]-1];
					
					for	(j = 0;j<bitnumber;j++)
					{
						sprintf(dtc, "XP%02X%02X", (KWD|(i-2)),Bit[j]);//这个不去重复
						
						ndtcnumber++;
						if(*dtc_number != 0)
						{//60个字节，用","分隔
							strcat(dtc_list,",");
							strcat(dtc_sys,",");
						}
						strcat(dtc_list,EcuId);//由于不同id的故障码库不一样，所以在故障码编号前面添加了这个
						strcat(dtc_list,dtc);
						strcat(dtc_sys,system);
						(*dtc_number)++;
						printfUartObdiideBug("honda add dtc:%s system:%s \r\n",dtc,system);
						printfUartObdiideBug("honda dtc_list:%s dtc_number:%d \r\n",dtc_list,*dtc_number);
					}
				}
				break;
			
			case 0x04:
				//pro_honda_read_dtc_body()
			
				number = pro_SendRecv_Honda(sendcmd, recvbuf);
				if (number==0)
				{
					continue;
				}
				dtcnumber=recvbuf[pos-1];
				if(dtcnumber>50)		//????50????
				{
					dtcnumber=50;
				}
				for(i=0;i<dtcnumber;i++)
				{
					dtc_sprintf_Honda_Mode_1((char *)dtc,recvbuf+i*2+pos);
					dtc_sprintf_Honda_Mode_1((char *)dtcname[ndtcnumber], recvbuf+i*2+pos); // 把故障码存起来
					bFlag = TRUE;
					for (total=0; total<ndtcnumber; total++)//比较故障码是否重复
					{
						if (strcmp((char*)dtc,(char*)dtcname[total]) == 0)
						{
							bFlag = FALSE;//此故障码重复
							break;
						}
					}
					if(bFlag)//不重复的添加进去
					{
						ndtcnumber++;
						if(*dtc_number != 0)
						{//60个字节，用","分隔
							strcat(dtc_list,",");
							strcat(dtc_sys,",");
						}
						strcat(dtc_list,EcuId);//由于不同id的故障码库不一样，所以在故障码编号前面添加了这个
						strcat(dtc_list,dtc);
						strcat(dtc_sys,system);
						(*dtc_number)++;
						printfUartObdiideBug("honda add dtc:%s system:%s \r\n",dtc,system);
						printfUartObdiideBug("honda dtc_list:%s dtc_number:%d \r\n",dtc_list,*dtc_number);
					}
				}
				break;
				
			case 0x07:
				//pro_honda_read_dtc()
				//SRS06 
				//从pos位开始，按位与60或者a0,如果结果是60、a0，则故障码编号是Xdtcmsgbuf[i]60或者Xdtcmsgbuf[i]A0,dtcmsgbuf[i]的值在库中已填好
				
				if(ECUID_mode == HONDA_ECUID_MODE_0C)
				{//目前只有这两个模式用到
					memcpy(FEATURE,FEATURE_SRS03,10);
					FEATURE_size = 8;
				}
				else if(ECUID_mode == HONDA_ECUID_MODE_0D)
				{
					memcpy(FEATURE,FEATURE_SRS06,10);
					FEATURE_size = 10;
				}
				else
				{
					continue;
				}
				
				number = pro_SendRecv_Honda(sendcmd, recvbuf);
				if (number==0)
				{
					continue;
				}
				
				for (i = 0; i < FEATURE_size && i <(number-pos); i++)
				{
					if (recvbuf[i+pos] && FEATURE[i])
					{
						if ((recvbuf[i+pos]&0x60) == 0x60)
						{
							sprintf(dtc, "X%02X60", FEATURE[i]);
							
							ndtcnumber++;
							if(*dtc_number != 0)
							{//60个字节，用","分隔
								strcat(dtc_list,",");
								strcat(dtc_sys,",");
							}
							strcat(dtc_list,EcuId);//由于不同id的故障码库不一样，所以在故障码编号前面添加了这个
							strcat(dtc_list,dtc);
							strcat(dtc_sys,system);
							(*dtc_number)++;
							printfUartObdiideBug("honda add dtc:%s system:%s \r\n",dtc,system);
							printfUartObdiideBug("honda dtc_list:%s dtc_number:%d \r\n",dtc_list,*dtc_number);
						}
						if ((recvbuf[i+pos]&0xa0) == 0xa0)
						{
							sprintf(dtc, "X%02XA0", FEATURE[i]);
							
							ndtcnumber++;
							if(*dtc_number != 0)
							{//60个字节，用","分隔
								strcat(dtc_list,",");
								strcat(dtc_sys,",");
							}
							strcat(dtc_list,EcuId);//由于不同id的故障码库不一样，所以在故障码编号前面添加了这个
							strcat(dtc_list,dtc);
							strcat(dtc_sys,system);
							(*dtc_number)++;
							printfUartObdiideBug("honda add dtc:%s system:%s \r\n",dtc,system);
							printfUartObdiideBug("honda dtc_list:%s dtc_number:%d \r\n",dtc_list,*dtc_number);
						}		
					}		
				}

				break;
			
			
			case 0x09:
				//pro_honda_read_dtc()
				//SRS06
				//第二位与08，若不为0显示两条故障码，否则与80，若不为0显示一条，故障码编号是Xdtcmsgbuf[i]08,
				if(ECUID_mode == HONDA_ECUID_MODE_0D)
				{//目前只有这一个模式用到
					memcpy(FEATURE,FEATURE_SRS06_TYPE09,10);
					FEATURE_size = 2;
				}
				else
				{
					continue;
				}
				
				number = pro_SendRecv_Honda(sendcmd, recvbuf);
				if (number==0)
				{
					continue;
				}
				
				for (i = 0; i < FEATURE_size && i <(number-pos); i++)
				{
					if (recvbuf[i+pos] && FEATURE[i])
					{
						if (i==0)
						{
							if ((recvbuf[i+pos]&0x60) == 0x60)
							{
								sprintf(dtc, "X%02X60", FEATURE[i]);
								
								ndtcnumber++;
								if(*dtc_number != 0)
								{//60个字节，用","分隔
									strcat(dtc_list,",");
									strcat(dtc_sys,",");
								}
								strcat(dtc_list,EcuId);//由于不同id的故障码库不一样，所以在故障码编号前面添加了这个
								strcat(dtc_list,dtc);
								strcat(dtc_sys,system);
								(*dtc_number)++;
								printfUartObdiideBug("honda add dtc:%s system:%s \r\n",dtc,system);
								printfUartObdiideBug("honda dtc_list:%s dtc_number:%d \r\n",dtc_list,*dtc_number);
								
								sprintf(dtc, "X%02X61", FEATURE[i]);
								
								ndtcnumber++;
								if(*dtc_number != 0)
								{//60个字节，用","分隔
									strcat(dtc_list,",");
									strcat(dtc_sys,",");
								}
								strcat(dtc_list,EcuId);//由于不同id的故障码库不一样，所以在故障码编号前面添加了这个
								strcat(dtc_list,dtc);
								strcat(dtc_sys,system);
								(*dtc_number)++;
								printfUartObdiideBug("honda add dtc:%s system:%s \r\n",dtc,system);
								printfUartObdiideBug("honda dtc_list:%s dtc_number:%d \r\n",dtc_list,*dtc_number);
							}
							if ((recvbuf[i+pos]&0xa0) == 0xa0)
							{
								sprintf(dtc, "X%02XA0", FEATURE[i]);
								
								ndtcnumber++;
								if(*dtc_number != 0)
								{//60个字节，用","分隔
									strcat(dtc_list,",");
									strcat(dtc_sys,",");
								}
								strcat(dtc_list,EcuId);//由于不同id的故障码库不一样，所以在故障码编号前面添加了这个
								strcat(dtc_list,dtc);
								strcat(dtc_sys,system);
								(*dtc_number)++;
								printfUartObdiideBug("honda add dtc:%s system:%s \r\n",dtc,system);
								printfUartObdiideBug("honda dtc_list:%s dtc_number:%d \r\n",dtc_list,*dtc_number);
							}
						}
						else
						{
							if ((recvbuf[i+pos]&0x08) == 0x08)
							{
								sprintf(dtc, "X%02X80", FEATURE[i]);
								
								ndtcnumber++;
								if(*dtc_number != 0)
								{//60个字节，用","分隔
									strcat(dtc_list,",");
									strcat(dtc_sys,",");
								}
								strcat(dtc_list,EcuId);//由于不同id的故障码库不一样，所以在故障码编号前面添加了这个
								strcat(dtc_list,dtc);
								strcat(dtc_sys,system);
								(*dtc_number)++;
								printfUartObdiideBug("honda add dtc:%s system:%s \r\n",dtc,system);
								printfUartObdiideBug("honda dtc_list:%s dtc_number:%d \r\n",dtc_list,*dtc_number);
								
								sprintf(dtc, "X%02X08", FEATURE[i]);
								
								ndtcnumber++;
								if(*dtc_number != 0)
								{//60个字节，用","分隔
									strcat(dtc_list,",");
									strcat(dtc_sys,",");
								}
								strcat(dtc_list,EcuId);//由于不同id的故障码库不一样，所以在故障码编号前面添加了这个
								strcat(dtc_list,dtc);
								strcat(dtc_sys,system);
								(*dtc_number)++;
								printfUartObdiideBug("honda add dtc:%s system:%s \r\n",dtc,system);
								printfUartObdiideBug("honda dtc_list:%s dtc_number:%d \r\n",dtc_list,*dtc_number);
							}
							else if ((recvbuf[i+pos]&0x80) == 0x80)
							{
								sprintf(dtc, "X%02X80", FEATURE[i]);
								
								ndtcnumber++;
								if(*dtc_number != 0)
								{//60个字节，用","分隔
									strcat(dtc_list,",");
									strcat(dtc_sys,",");
								}
								strcat(dtc_list,EcuId);//由于不同id的故障码库不一样，所以在故障码编号前面添加了这个
								strcat(dtc_list,dtc);
								strcat(dtc_sys,system);
								(*dtc_number)++;
								printfUartObdiideBug("honda add dtc:%s system:%s \r\n",dtc,system);
								printfUartObdiideBug("honda dtc_list:%s dtc_number:%d \r\n",dtc_list,*dtc_number);
							}
						}
					}
				}

				break;
			
				
			case 0x11://为了和不循环发送的0x01区分，手动改成了0x11
				//pro_honda_read_dtc_cycle()
				//类型0x01:程序会循环发送命令，直到回复的第4字节为0为止，两位为一个故障码
				for	(msgno = 0; msgno < 0xff; msgno++)
				{
					if(ndtcnumber>=50)//防溢出
					{
						break;
					}
					sendcmd[4] = 1 + msgno;
					number = pro_SendRecv_Honda(sendcmd, recvbuf);
					if (number==0)
					{
						continue;
					}
					
					for(i=pos;i<number-1;i+=2)
					{
						if (ndtcnumber >= 50) //防溢出
						{
							break;
						}
						if(recvbuf[i]|recvbuf[i+1])
						{
							sprintf((char*)dtc,"X%02X%02X",recvbuf[i],recvbuf[i+1]);
							sprintf((char*)dtcname[ndtcnumber],"X%02X%02X",recvbuf[i],recvbuf[i+1]);//把故障码存起来
							bFlag = TRUE;
							for (total=0; total<ndtcnumber; total++)//比较故障码是否重复
							{
								if (strcmp((char*)dtc,(char*)dtcname[total]) == 0)
								{
									bFlag = FALSE;//此故障码重复
									break;
								}
							}
							if(bFlag)//不重复的添加进去
							{
								ndtcnumber++;
								if(*dtc_number != 0)
								{//60个字节，用","分隔
									strcat(dtc_list,",");
									strcat(dtc_sys,",");
								}
								strcat(dtc_list,EcuId);//由于不同id的故障码库不一样，所以在故障码编号前面添加了这个
								strcat(dtc_list,dtc);
								strcat(dtc_sys,system);
								(*dtc_number)++;
								printfUartObdiideBug("honda add dtc:%s system:%s \r\n",dtc,system);
								printfUartObdiideBug("honda dtc_list:%s dtc_number:%d \r\n",dtc_list,*dtc_number);
							}
						}
					}
					if(recvbuf[pos-1] == 0)
					{
						break;
					}
				}
				break;
				
				
			case 0x27: //防止和本田协议的冲突，由0x07手动改成0x27
			//pro_can_read_dtc()
			//POS位开始，每4字节一个故障码，前两个字节有效，第三字节必须为Feature
			//Feature 

				number = pro_SendRecv_Honda(sendcmd, recvbuf);
				if (number==0)
				{
					break;
				}
				
				for(i=pos;i<number;i+=4)
				{
					if (i + 4 > number)
							break;
					if((recvbuf[i]|recvbuf[i+1])&&((int)recvbuf[i+2] == feature))
					{
						dtc_sprintf_Honda_Mode_1((char *)dtc, recvbuf + i);
						dtc_sprintf_Honda_Mode_1((char *)dtcname[ndtcnumber], recvbuf + i); // 把故障码存起来
						
						bFlag = TRUE;
						for (total=0; total<ndtcnumber; total++)//比较故障码是否重复
						{
							if (strcmp((char*)dtc,(char*)dtcname[total]) == 0)
							{
								bFlag = FALSE;//此故障码重复
								break;
							}
						}
						if(bFlag)//不重复的添加进去
						{
							ndtcnumber++;
							if(*dtc_number != 0)
							{//60个字节，用","分隔
								strcat(dtc_list,",");
								strcat(dtc_sys,",");
							}
							strcat(dtc_list,EcuId);//由于不同id的故障码库不一样，所以在故障码编号前面添加了这个
							strcat(dtc_list,dtc);
							strcat(dtc_sys,system);
							(*dtc_number)++;
							printfUartObdiideBug("honda add dtc:%s system:%s \r\n",dtc,system);
							printfUartObdiideBug("honda dtc_list:%s dtc_number:%d \r\n",dtc_list,*dtc_number);
						}
					}
				}
				break;
			
			case 0x29://防止和本田协议的冲突，由0x09手动改成0x29
				//pro_can_read_dtc
				//CANABS
				//4个字节为1个故障码，前3有效
				number = pro_SendRecv_Honda(sendcmd, recvbuf);
				if (number==0)
				{
					break;
				}
				for (i = pos; i < number; i = i + 4)
				{
					if (i + 4 > number)
						break;
					if (recvbuf[i] || recvbuf[i + 1])
					{
						dtc_sprintf_Honda_Mode_1(dtc, recvbuf + i);
						sprintf(dtc, "%s%02X", dtc, recvbuf[i+2]);
						
						ndtcnumber++;
						if(*dtc_number != 0)
						{//60个字节，用","分隔
							strcat(dtc_list,",");
							strcat(dtc_sys,",");
						}
						strcat(dtc_list,EcuId);//由于不同id的故障码库不一样，所以在故障码编号前面添加了这个
						strcat(dtc_list,dtc);
						strcat(dtc_sys,system);
						(*dtc_number)++;
						printfUartObdiideBug("honda add dtc:%s system:%s \r\n",dtc,system);
						printfUartObdiideBug("honda dtc_list:%s dtc_number:%d \r\n",dtc_list,*dtc_number);
					}
				}
				break;
				
				
							
			default:
				break;
		}
		
	}

	return;
}

void dtc_sprintf_Honda_Mode_1(char *strbuf,uint8_t *buf)
{
	switch(buf[0] & 0xC0)
	{
	case 0x00: // p
		sprintf(strbuf, "XP%04X", (buf[0] & 0x3F) * 0x100 + buf[1]);
		break;
	case 0x40: // c
		sprintf(strbuf, "XC%04X", (buf[0] & 0x3F) * 0x100 + buf[1]);
		break;
	case 0x80: // b
		sprintf(strbuf, "XB%04X", (buf[0] & 0x3F) * 0x100 + buf[1]);
		break;
	case 0xC0: // u
		sprintf(strbuf, "XU%04X", (buf[0] & 0x3F) * 0x100 + buf[1]);
	}
}

//void dtc_sprintf_Honda_Mode_2(char *strbuf,uint8_t *buf)
//{
//	sprintf(strbuf, "%06X", (buf[0]) * 0x10000 + buf[1] * 0x100 + buf[2]);
//}


void pro_clear_dtc_Honda(HONDA_ENTER_SYSTEM_MODE enter_system_mode) //清码
{
	uint8_t sendcmd[8] = {0}; //清码
	uint8_t recvbuf[64] = {0}; //
	uint8_t number = 0;
	uint8_t i = 0;

	HONDA_ECUID_MODE ECUID_mode = Enter_System_Honda[enter_system_mode].Honda_ECUID;

	//多个命令轮流发送，不做判断
	for	(i = 0; i<Honda_EcuId[ECUID_mode].CMD_Clear_DTC_Size; i++)
	{
		memcpy(sendcmd, Honda_EcuId[ECUID_mode].CMD_Clear_DTC[i], Honda_EcuId[ECUID_mode].CMD_Clear_DTC[i][0]+1);
		number = pro_SendRecv_Honda(sendcmd, recvbuf);
	}
}

void pro_exit_system_Honda(HONDA_ENTER_SYSTEM_MODE enter_system_mode) //退出系统
{
	uint8_t sendcmd[8] = {0}; //退出
	uint8_t recvbuf[64] = {0}; //
	uint8_t number = 0;

	if(Enter_System_Honda[enter_system_mode].System_Exit[0]>0)
	{
		memcpy(sendcmd, Enter_System_Honda[enter_system_mode].System_Exit, Enter_System_Honda[enter_system_mode].System_Exit[0]+1);
		number = pro_SendRecv_Honda(sendcmd, recvbuf);
	}
}


bool Much_Init_Honda(void)
{
	uint8_t i = 0,j = 0;
	uint8_t initstart = 0,initend = 0;
	HONDA_INIT_MODE mode = 0;
	intptr_t Enter_System_Mode = -1;
	bool enter = false;
	//int num = 0;
	//char buf[60] = {0};

	uint8_t iSystem = 5;//一共5个系统
	
	memset(g_vehiclediagnosie.fault_list,0,40);
	memset(g_vehiclediagnosie.sys_list,0,40);
	g_vehiclediagnosie.fault_total = 0;
	
	printfUartObdiideBug("welcome to [%s]\r\n",__func__);
	if(m_first_honda)
	{
		//第一次进入
		m_first_honda = false;
		
		initstart = 0;//从第一个系统开始扫描时重置
		initend = 0;//从第一个系统开始扫描时重置
		for (i = 0;i<iSystem;i++)
		{
			initstart = initend;
			initend += m_iSystem_Mode_Size_Honda[i];
			for (j = initstart; j < initend; j++)
			//for (j = 0x15; j < 0x16; j++)//测试用
			{
				//reset_global_data();//重置全局变量数据
				if(Init_IO_Honda(InitIO_Honda[j].Init_Mode)) //初始化IO
				{
					Enter_System_Mode = pro_much_init_honda(InitIO_Honda[j].Init_Mode,HONDA_ENTER_SYSTEM_MODE_NULL);
					if(Enter_System_Mode != -1) //初始化ECU //进系统
					{
						printfUartObdiideBug("honda enter success system:%d init:%d enter:%d Enter_System_Mode:%d\r\n",i,j,InitIO_Honda[j].Init_Mode,Enter_System_Mode);
						m_protocol_index_honda[i][0] = InitIO_Honda[j].Init_Mode;					//
						m_protocol_index_honda[i][1] = Enter_System_Mode;
						
						if(g_defaultMsgAttr.m_PCK_TYPE == 0x0f)//本田协议
						{
							pro_exit_system_Honda(m_protocol_index_honda[i][1]);//退出
							pro_enter_system_Honda(m_protocol_index_honda[i][1]);//进系统命令
						}
						
						pro_read_dtc_Honda(m_protocol_index_honda[i][1],i+1,&(g_vehiclediagnosie.fault_total),g_vehiclediagnosie.fault_list,g_vehiclediagnosie.sys_list);	//读码
						pro_clear_dtc_Honda(m_protocol_index_honda[i][1]);															//清码
						pro_exit_system_Honda(m_protocol_index_honda[i][1]);														//退出
						break;
					}
					else
					{
						printfUartObdiideBug("honda enter false system:%d init:%d (%d-%d) \r\n",i,j,initstart,initend-1);
					}
				}
			}//for (j = initstart; j < initend; j++)
		}//for (i = 0;i<iSystem;i++)
		for(i = 0;i<iSystem;i++)
		{
			if(m_protocol_index_honda[i][0] != -1)
				return true;
		}
	}
	else
	{
		vTaskDelay(3000);
		enter = false;
		for (i = 0;i<iSystem;i++)
		{
			if(m_protocol_index_honda[i][0] != -1)//直接使用记录的协议，如果为-1，则说明不存在此系统
			{
				if(Init_IO_Honda(InitIO_Honda[m_protocol_index_honda[i][0]].Init_Mode)) //初始化IO
				{
					Enter_System_Mode = pro_much_init_honda(m_protocol_index_honda[i][0],m_protocol_index_honda[i][1]);
					if(Enter_System_Mode != -1) //初始化ECU
					{
//						if(diag_flag == 1 && data_flag == 0)//车辆诊断
//						{
								enter = true;
								if(g_defaultMsgAttr.m_PCK_TYPE == 0x0f)//本田协议
								{
									pro_exit_system_Honda(m_protocol_index_honda[i][1]);//退出
									pro_enter_system_Honda(m_protocol_index_honda[i][1]);//进系统命令
								}									
								pro_read_dtc_Honda(m_protocol_index_honda[i][1],i+1,&(g_vehiclediagnosie.fault_total),g_vehiclediagnosie.fault_list,g_vehiclediagnosie.sys_list);	//读码
								pro_exit_system_Honda(m_protocol_index_honda[i][1]);														//退出
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





//void pro_protocol_init_Toyota(INIT_MODE_TOYOTA model)
//{
//	switch(model)
//	{
//		case OBDII_MODE_01:
//		case OBDII_MODE_02:
//			cmd_msg_all_canbus_11bit_Toyota();
//			break;
//		case OBDII_MODE_03:
//		case OBDII_MODE_04:
//		case OBDII_MODE_05:
//			cmd_msg_all_default_model_Toyota();
//			break;
//		
//		default:
//			break;
//	}
//}


//void cmd_msg_all_canbus_11bit_Toyota()
//{
//	uint32_t canread, canflow;
//	
//	canread = m_modulebuf[0];
//	canflow = canread-8;
//	
//	if((g_defaultMsgAttr.m_PCK_TYPE == 0x22) || (g_defaultMsgAttr.m_PCK_TYPE == 0x20))
//	{
//		//m_canbussendid=getHex32(ezxml_attr(msgxml, "CANSEND"));
//		m_canbusreadid=canread;
//		m_canbusflowid=canflow;
//	}
//	//SetCanbusFilter(canread);//丰田单个系统单个ID,用不到
//}

//void cmd_msg_all_default_model_Toyota()
//{
//	cmd_set_ecumodule_Toyota(m_modulebuf[0]);
//}

//uint8_t cmd_set_ecumodule_Toyota(uint8_t module)
//{
//	m_setmodule = module;
//	return m_setmodule;
//}


//void SetCanbusFilter_Toyota(uint32_t filterID)
//{
//	int i;
//	int number;
//	uint8_t buffer[128];
//	number = 0;
//	buffer[number++] = 0xa5;
//	buffer[number++] = 0xa5;
//	buffer[number++] = 0x00;
//	buffer[number++] = 0x05;
//	buffer[number++] = 0x70;
//	buffer[number++] = (uint8_t)(filterID>>24);
//	buffer[number++] = (uint8_t)(filterID>>16);
//	buffer[number++] = (uint8_t)(filterID>>8);
//	buffer[number++] = (uint8_t)(filterID&0xff);
//	buffer[number++] = 0x00;
//	for(i=2;i<number-1;i++)
//	{
//		buffer[number-1] += buffer[i];
//	}
//	buffer[number-1] = ~buffer[number-1];
//	printfUartObdiideBug("sendbuf:%s\r\n",buffer);
//	USB_BULK_Queue_Write(&USB_RX_QueueData,buffer,number);
//	vTaskDelay(40);	
//}


int pack_typef_honda(uint8_t *u8bufin, uint8_t *u8buf, intptr_t bufsize)
{
	int number = 5;
	int i;
	char *p;
	char *p0 ;

	memcpy(u8buf+number,u8bufin+1,u8bufin[0]);
	
	//处理
	number += u8bufin[0];
	if(u8buf[6]+5==number)
		number=number-1;
	
	//校验
	u8buf[number] = 0;
	for(i = 5; i<number; i++)
	{
		u8buf[number] += u8buf[i];
	}
	u8buf[number]=~u8buf[number]+1;
	number++;

	//头
	u8buf[0] = 0xa5;
	u8buf[1] = 0xa5;
	u8buf[2] = (number-4) / 0x100;
	u8buf[3] = (number-4) % 0x100;
//	p = (char*)ezxml_attr(xml,"ORD");
//	if(p)
//		u8buf[4] = getHex(p);
//	else
		u8buf[4] = g_defaultMsgAttr.m_ORD;

	
	//校验和
	u8buf[number] = 0;
	for(i = 2; i<number; i++)
	{
		u8buf[number] += u8buf[i];
	}
	u8buf[number] = ~u8buf[number];	

	return number+1;
}


//本田的0x20打包的CAN和其他的车型的不一样，不调用公共的
/*????CAN??,???????????,????30????,?????????????*/
uint16_t cmd_CAN_RECV_SEND_ANS_Honda(uint8_t *cmdbuf,uint8_t *recvbuf)
{
		uint16_t number,i,j;
		uint8_t sendbuf[256];
		uint8_t tag=0;
		uint8_t ord;
		int count=0;
		int sendnum=0;
		int iConuntNo7fNo40 = 0;

		ord = 0 ;
		if(m_CAN_ANS_R_Honda != 0x01)
		{
			return 0;
		}
		//if(msgxml != NULL && ezxml_txt(msgxml)!= NULL)
		{
				memset(sendbuf, 0, sizeof(sendbuf));
				//sendnum = getHexEx(ezxml_txt(msgxml), sendbuf);
				memcpy(sendbuf,cmdbuf+1,cmdbuf[0]);
				sendnum = cmdbuf[0];
				ord = sendbuf[0];
				recvbuf[0] = sendnum;
				if(sendnum > 7)
				{
						recvbuf[0] = 0x10;
						recvbuf[1] = sendnum;
						memcpy(recvbuf+2, sendbuf, 6);
						i = 6;
						tag = 0x20;
						for (j=1; i<sendnum; j++)
						{
								if (tag++ >= 0x30)
								{
										tag = 0x21;
								}
								recvbuf[j*8] = tag;
								memcpy(recvbuf+j*8+1, sendbuf+i, 7);
								i += 7;
						}
						sendnum = j*8;
				}
				else
				{
						memcpy(recvbuf+1, sendbuf, 7);
						sendnum = 8;
				}
				sendnum++;
				memset(sendbuf, 0, sizeof(sendbuf));
				sendbuf[0] = 0xa5;
				sendbuf[1] = 0xa5;
				sendbuf[2] = sendnum/0x100;
				sendbuf[3] = sendnum%0x100;
//				if(ezxml_attr(msgxml, "ORD"))
//				{
//					sendbuf[4] = getHex(ezxml_attr(msgxml, "ORD"));
//					m_sysname[m_autoNum].ordnum = sendbuf[4];
//				}
//				else
//				{
					sendbuf[4] = g_defaultMsgAttr.m_ORD;
//				}
				memcpy(sendbuf+5, recvbuf, sendnum);
				sendnum += 5;
				sendbuf[sendnum-1] = 0;
				for(i=2; i<sendnum-1; i++)
				{
					sendbuf[sendnum-1] += sendbuf[i];
				}
				sendbuf[sendnum-1] = ~sendbuf[sendnum-1];
				//Uart_ClearInput(g_carportHandle);
				//Uart_ClearOutput(g_carportHandle);	
				//Uart_SendBuf(g_carportHandle, sendbuf, sendnum);
				USB_BULK_Queue_Write(&USB_RX_QueueData, sendbuf ,sendnum);
			}
		//????,???????????? cheng 20081128
		//number = Uart_ReadBuf(g_carportHandle, recvbuf, 256, 1000);
		OBDII_ReadBuf(recvbuf, &number,1000);
loop:
		if(number < 6)
		{
				return 0;
		}
		number -= 6;
		memcpy(recvbuf, recvbuf+5, number);
		recvbuf[number] = 0;
		//if(recvbuf[0] == 0x10)
		if ((recvbuf[0] & 0xf0) == 0x10)
		{
				if ((ord > 0) && ((ord +0x40) != recvbuf[2]))
				{
						return 0;
				}
				count = recvbuf[1];
				memcpy(recvbuf, recvbuf+2, 6);
				i = 6;
				for(j=1; i<count; j++)
				{
						//GUI_X_Delay(20);
						//number = Uart_ReadBuf(g_carportHandle, sendbuf, 256, 1000);
						OBDII_ReadBuf(sendbuf, &number,1000);
						if(number == 0)
						{
								return 0;
						}
						if((sendbuf[5]&0xf0) != 0x20)
						{
								return 0;
						}
						memcpy(recvbuf+j*7-1, sendbuf+6, 7);
						i += 7;
				}
				number = count;
		}
		else if((recvbuf[0] < 0x10) && (recvbuf[0] > 0))/*******************************************************************opern8**************/
		{
				if((ord > 0) && ((ord +0x40) != recvbuf[1]))
				{
						if((recvbuf[1] == 0x7f) && (recvbuf[3] == 0x78))
						{
								if(++count < 4)
								{
										//????,??????,??3600ms
										//number = Uart_ReadBuf(g_carportHandle, recvbuf, 256, 4000);
										OBDII_ReadBuf(recvbuf, &number,1000);
										goto loop;
								}
						}
						else if((recvbuf[1] == 0x7f) && (recvbuf[3] == 0x21))
						{
							//Uart_SendBuf(g_carportHandle, sendbuf, sendnum);
							USB_BULK_Queue_Write(&USB_RX_QueueData, sendbuf ,sendnum);
							//number = Uart_ReadBuf(g_carportHandle, recvbuf, 256, 100);
							OBDII_ReadBuf(recvbuf, &number,1000);
							goto loop;
						}
						else if(recvbuf[1] == 0x7f)//??????,??7f78,7f21?,????7fxx??????0
						{
							return 0;
						}
						else if(recvbuf[1] != 0x7f)
						{
						// yyh 2018?11?2? ????????????OBD???????01xx?????,?????0x40???0x7f?????
						if (iConuntNo7fNo40 > 3)
						{
							return 0;
						}
						//number = Uart_ReadBuf(g_carportHandle, recvbuf, 256, 4000);
						OBDII_ReadBuf(recvbuf, &number,1000);
						iConuntNo7fNo40++;
						goto loop;
						//return 0;
					}
				}
				number = recvbuf[0];
				memcpy(recvbuf, recvbuf+1, number);
		}
//		if (ezxml_attr(msgxml,"DL"))
//		{
//			GUI_X_Delay(getHex(ezxml_attr(msgxml,"DL")));
//		}
		return number;
}

uint16_t pro_SendRecv_Honda(uint8_t * sendbuf, uint8_t *recvbuf)
{
	int i,ret;
	
	if((g_defaultMsgAttr.m_ORD&0x0F) == 0x00)//CAN
	{
#if defined(TOM_RDWR_DEBUG)		
			printfUartObdiideBug("[%s] sendbuf: \r\n",__func__);
			for(i=0;i<=sendbuf[0];i++)
				printfUartObdiideBug("%02X ",sendbuf[i]);
			printfUartObdiideBug("\r\n");
#endif		
			ret = cmd_CAN_RECV_SEND_ANS_Honda(sendbuf,recvbuf);
#if defined(TOM_RDWR_DEBUG)			
			printfUartObdiideBug("[%s] recvbuf: \r\n",__func__);
			for(i=0;i<ret;i++)
				printfUartObdiideBug("%02X ",recvbuf[i]);
			printfUartObdiideBug("\r\n");
#endif		
			return ret;
	}
	else
	{
			return pro_SendRecv(sendbuf, recvbuf);
	}
}

uint8_t GetBitDtc(uint8_t *recvbuf,uint8_t pos,uint8_t *Bit)//本田0x08读码模式用到，获取这个命令有效的位
{
	uint8_t j = 0;
	uint8_t iBit = 0;
	uint8_t mask = 0x01;

	for	(j=1;j<8;j++)
	{
		if(recvbuf[pos]&mask)
		{
			Bit[iBit] = recvbuf[pos]&mask;
			iBit++;
			mask = mask<<1;
		}
	}

	return iBit;
}


bool pro_enter_system_Honda(HONDA_ENTER_SYSTEM_MODE mode) //进入系统
{
	uint16_t number = 0;
	uint8_t sendbuf[8] = {0};
	uint8_t recvbuf[64] = {0};
	uint8_t ord = 0;
	
	//进系统命令
	memcpy(sendbuf,Enter_System_Honda[mode].System_Enter,Enter_System_Honda[mode].System_Enter[0]+1);
	
	ord = g_defaultMsgAttr.m_ORD;//临时保存
	g_defaultMsgAttr.m_ORD = 0x23;//进系统 临时改成0x23
	number = pro_SendRecv_Honda(sendbuf, recvbuf);
	g_defaultMsgAttr.m_ORD = ord;//还原回去

	if(number == 0)
	{
		return false;
	}

	return true;
}
bool pro_init_ECU_id_Honda(HONDA_ENTER_SYSTEM_MODE mode) //读取id
{
	uint16_t number = 0;
	uint8_t sendbuf[8] = {0};
	uint8_t recvbuf[64] = {0};
	
	//匹配型号命令
	memcpy(sendbuf,Enter_System_Honda[mode].ECUID_CMD,Enter_System_Honda[mode].ECUID_CMD[0]+1);
	
	number = pro_SendRecv_Honda(sendbuf, recvbuf);
	if(number <= 3)
	{
		return false;
	}

	return true;
}











/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
