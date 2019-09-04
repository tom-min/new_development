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
#include "STD_GMCN_CMD_PROCESS.h"
#include "FreeRTOS.h"
#include "task.h"
#include "STD_OBD_CMD_PROCESS.h"

#define MAXCAN  64
#define MAXSIZE 256   

#define ERRORCODE_SUCCESS                  0x00
#define ERRORCODE_NORESPONSE               0x01
#define ERRORCODE_NEGATIVERESPONSE         0x02
#define ERRORCODE_SENDCOMMAND              0x03
#define ERRORCODE_SINGLEFRAME              0x04
#define ERRORCODE_FIRSTFRAME               0x05
#define ERRORCODE_CONSECUTIVEFRAME         0x06
#define ERRORCODE_FLOWCONTROL              0x07
#define ERRORCODE_CHECKSERVICE             0x08
#define ERRORCODE_CHECKPARAMETER           0x09
#define ERRORCODE_RESPONSELENGTH           0x0A
#define ERRORCODE_RESPONSEFORMAT           0x0B


uint8_t m_iSystem_Mode_Size_Gmcn[5] = {2,2,3,3,5};//每个系统存在的协议的个数，第一次扫描的时候要用到
intptr_t m_protocol_index_gmcn[5] = {-1,-1,-1,-1,-1};   //记录侦测到的协议,5个系统的初始化IO和命令都为-1
bool m_first_gmcn = true;

extern DEFAULT_MSGATTR g_defaultMsgAttr; 

////uint8_t data_flag = 0;
////uint8_t diag_flag = 0;

extern Vehicle_diagnosis g_vehiclediagnosie;
extern Data_collection g_datacollection;

//static uint32_t m_modulebuf[256];
//extern uint8_t m_modulenum;
//extern uint8_t m_setmodule;
extern uint32_t m_canbussendid;
extern uint32_t m_canbusreadid;/*当前系统接收ID*/
extern uint32_t m_canbusflowid;
uint32_t m_canbusdataid=0x000005e8;/*????????ID,??????????ID*/
uint32_t m_curcanreadid=0x000007e8;/*????????ID*/

uint16_t m_ResponseCode;
uint16_t m_NegativeCode;
uint8_t m_CAN_LISTEN_TYPE_GMCN = 0;
uint8_t	m_CAN_ANS_T_Gmcn = 0;
uint8_t	m_CAN_ANS_R_Gmcn = 0;

//初始化IO命令
const INIT_CMD_GMCN InitIO_Gmcn[]=\
{	
	//ECM 
	{
		{GMCN_INIT_MODE_01},
		{MSG_Init_CAN_PackType_Gmcn},
		{0x2F,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x07,0xE0,0x05,0x00,0x00,0x07,0xE8,0x06,0x00,0x00,0x02,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0A,0x87,0xD0,0x0B,0x00,0x00,0x0C,0xE6,0x12,0x01,0x00},
		{0x01,0x3E},
		{0x01,0x20},
		{0x02},
		{	
			GMCN_READDTC_MODE_01,
			GMCN_READDTC_MODE_02,
		},
	},
	{
		{GMCN_INIT_MODE_02},
		{MSG_Init_KWP_PackType_Gmcn},
		{0x2B,0x01,0x00,0x28,0xA0,0x02,0x00,0x03,0xC0,0x04,0x77,0x00,0xC1,0xC1,0x05,0x0F,0x19,0x40,0x05,0x03,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x25,0x09,0x03,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0C,0x01,0x00,0x00},
		{0x01,0x81},
		{0x01,0x82},
		{0x01},
		{	
			GMCN_READDTC_MODE_03,
		},
	},

	//TCM
	{
		{GMCN_INIT_MODE_03},
		{MSG_Init_CAN_PackType_Gmcn},
		{0x2F,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x07,0xE2,0x05,0x00,0x00,0x07,0xEA,0x06,0x00,0x00,0x02,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0A,0x87,0xD0,0x0B,0x00,0x00,0x0C,0xE6,0x12,0x01,0x00},
		{0x01,0x3E},
		{0x01,0x20},
		{0x01},
		{	
			GMCN_READDTC_MODE_01,
		},
	},
	{
		{GMCN_INIT_MODE_04},
		{MSG_Init_KWP_PackType_Gmcn},
		{0x2B,0x01,0x00,0x28,0xA0,0x02,0x00,0x03,0xC0,0x04,0x77,0x00,0xC1,0xC1,0x05,0x0F,0x19,0x40,0x05,0x03,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x25,0x09,0x03,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0C,0x01,0x00,0x00},
		{0x01,0x81},
		{0x01,0x82},
		{0x01},
		{	
			GMCN_READDTC_MODE_03,
		},
	},
	
	//EBCM
	{
		{GMCN_INIT_MODE_05},
		{MSG_Init_CAN_PackType_Gmcn},
		{0x2F,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x02,0x43,0x05,0x00,0x00,0x05,0x43,0x06,0x00,0x00,0x03,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0A,0x87,0xD0,0x0B,0x00,0x00,0x0C,0xE6,0x12,0x01,0x00},
		{0x01,0x3E},
		{0x01,0x20},
		{0x01},
		{	
			GMCN_READDTC_MODE_02,
		},
	},
	{
		{GMCN_INIT_MODE_06},
		{MSG_Init_CAN_PackType_Gmcn},
		{0x2F,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x07,0xE5,0x05,0x00,0x00,0x07,0xED,0x06,0x00,0x00,0x02,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0A,0x87,0xD0,0x0B,0x00,0x00,0x0C,0xE6,0x12,0x01,0x00},
		{0x01,0x3E},
		{0x01,0x20},
		{0x01},
		{	
			GMCN_READDTC_MODE_02,
		},
	},
	{
		{GMCN_INIT_MODE_07},
		{MSG_Init_KWP_PackType_Gmcn},
		{0x2B,0x01,0x00,0x28,0xA0,0x02,0x00,0x03,0xC0,0x04,0x77,0x00,0xC1,0xC1,0x05,0x0F,0x19,0x40,0x05,0x03,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x25,0x09,0x03,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0C,0x01,0x00,0x00},
		{0x01,0x81},
		{0x01,0x82},
		{0x01},
		{	
			GMCN_READDTC_MODE_04,
		},
	},
	
	//BCM
	{
		{GMCN_INIT_MODE_08},
		{MSG_Init_CAN_PackType_Gmcn},
		{0x2F,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x02,0x41,0x05,0x00,0x00,0x05,0x41,0x06,0x00,0x00,0x03,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0A,0x87,0xD0,0x0B,0x00,0x00,0x0C,0xE6,0x12,0x01,0x00},
		{0x01,0x3E},
		{0x01,0x20},
		{0x01},
		{	
			GMCN_READDTC_MODE_02,
		},
	},
	{
		{GMCN_INIT_MODE_09},
		{MSG_Init_CAN_PackType_Gmcn},
		{0x2F,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x02,0x41,0x05,0x00,0x00,0x05,0x41,0x06,0x00,0x00,0x03,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0A,0x87,0xD0,0x0B,0x00,0x00,0x0C,0xB3,0x12,0x02,0x00},
		{0x02,0x10,0x03},
		{0x01,0x20},
		{0x02},
		{	
			GMCN_READDTC_MODE_05,
			GMCN_READDTC_MODE_06,
		},
	},
	{
		{GMCN_INIT_MODE_0A},
		{MSG_Init_VPW_PackType_Gmcn},
		{0x1B,0x01,0x00,0x00,0x02,0x00,0x03,0x01,0x04,0x01,0x01,0x05,0x87,0xD0,0x06,0x0C,0x07,0xE0,0x08,0xF1,0x09,0xFF,0x0A,0x10,0x0B,0xFF,0x0E,0x35},
		{0x01,0x20},
		{0x01,0x20},
		{0x01},
		{	
			GMCN_READDTC_MODE_07,
		},
	},
	
	//IRSDM
	{
		{GMCN_INIT_MODE_0B},
		{MSG_Init_CAN_PackType_Gmcn},
		{0x2F,0x01,0x80,0x0B,0x2F,0x02,0x82,0x03,0x03,0x04,0x00,0x00,0x02,0x47,0x05,0x00,0x00,0x05,0x47,0x06,0x00,0x00,0x03,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0A,0x87,0xD0,0x0B,0x00,0x00,0x0C,0x01,0x12,0x01,0x00},
		{0x01,0x3E},
		{0x01,0x20},
		{0x01},
		{	
			GMCN_READDTC_MODE_02,
		},
	},
	{
		{GMCN_INIT_MODE_0C},
		{MSG_Init_CAN_PackType_Gmcn},
		{0x2F,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x02,0x47,0x05,0x00,0x00,0x05,0x47,0x06,0x00,0x00,0x03,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0A,0x87,0xD0,0x0B,0x00,0x00,0x0C,0xDC,0x12,0x01,0x00},
		{0x01,0x3E},
		{0x01,0x20},
		{0x01},
		{	
			GMCN_READDTC_MODE_02,
		},
	},
	{
		{GMCN_INIT_MODE_0D},
		{MSG_Init_CAN_PackType_Gmcn},
		{0x2F,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x02,0x47,0x05,0x00,0x00,0x05,0x47,0x06,0x00,0x00,0x03,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0A,0x87,0xD0,0x0B,0x00,0x00,0x0C,0xE6,0x12,0x01,0x00},
		{0x01,0x3E},
		{0x01,0x20},
		{0x01},
		{	
			GMCN_READDTC_MODE_02,
		},
	},
	{
		{GMCN_INIT_MODE_0E},
		{MSG_Init_KWP_PackType_Gmcn},
		{0x2B,0x01,0x00,0x28,0xA0,0x02,0x00,0x03,0xC0,0x04,0xDD,0x00,0xC1,0xC1,0x05,0x0F,0x19,0x40,0x05,0x03,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x25,0x09,0x03,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0C,0x01,0x00,0x00},
		{0x01,0x20},
		{0x01,0x20},
		{0x01},
		{	
			GMCN_READDTC_MODE_08,
		},
	},
	{
		{GMCN_INIT_MODE_0F},
		{MSG_Init_VPW_PackType_Gmcn},
		{0x1B,0x01,0x00,0x00,0x02,0x00,0x03,0x01,0x04,0x01,0x01,0x05,0x87,0xD0,0x06,0x0C,0x07,0xE0,0x08,0xF1,0x09,0xFF,0x0A,0x10,0x0B,0xFF,0x0E,0x35},
		{0x01,0x20},
		{0x01,0x20},
		{0x01},
		{	
			GMCN_READDTC_MODE_07,
		},
	},
	
};



const GMCN_READDTC Gmcn_ReadDtc[]=\
{
//	uint8_t ECUID_Honda;//匹配的ECUID
//	uint8_t CMD_Read_DTC_Size;//读码命令个数
//	uint8_t CMD_Read_DTC[2][8];//读码命令 最多有2条读码命令
//	uint8_t DCT_Type[2];//和读码命令对应的读码的方式
//	uint8_t DCT_Pos[2];//和读码命令对应的取码号的Pos位
//	uint8_t CMD_Clear_DTC_Size;//清码命令个数
//	uint8_t CMD_Clear_DTC[2][8];//清码命令
	
	//Engine CAN CANFI/CANDIE
	//AT CAN CANEAT/CANCVT/CANDCT/DCT/CAN9AT
	{
		{GMCN_READDTC_MODE_01},
		{0x03,0xA9,0x81,0x1A,0x00,0x00,0x00,0x00},
		{0x04},//DCT_Type
		{0x01},//DCT_Pos
		{0x03},//DCT_Len
		{0x01,0x04,0x00,0x00,0x00,0x00,0x00,0x00},
	},
	
	{
		{GMCN_READDTC_MODE_02},
		{0x03,0xA9,0x81,0x12,0x00,0x00,0x00,0x00},
		{0x04},//DCT_Type
		{0x01},//DCT_Pos
		{0x03},//DCT_Len
		{0x01,0x04,0x00,0x00,0x00,0x00,0x00,0x00},
	},
	
	{
		{GMCN_READDTC_MODE_03},
		{0x04,0x18,0x00,0xFF,0x00,0x00,0x00,0x00},
		{0x03},//DCT_Type
		{0x02},//DCT_Pos
		{0x03},//DCT_Len
		{0x01,0x04,0x00,0x00,0x00,0x00,0x00,0x00},
	},
	
	{
		{GMCN_READDTC_MODE_04},
		{0x04,0x18,0x02,0x40,0x00,0x00,0x00,0x00},
		{0x03},//DCT_Type
		{0x02},//DCT_Pos
		{0x03},//DCT_Len
		{0x03,0x14,0x40,0x00,0x00,0x00,0x00,0x00},
	},
	
	{
		{GMCN_READDTC_MODE_05},
		{0x03,0x19,0x02,0x09,0x00,0x00,0x00,0x00},
		{0x02},//DCT_Type
		{0x03},//DCT_Pos
		{0x04},//DCT_Len
		{0x01,0x14,0x00,0x00,0x00,0x00,0x00,0x00},
	},
	
	{
		{GMCN_READDTC_MODE_06},
		{0x03,0x19,0x02,0x08,0x00,0x00,0x00,0x00},
		{0x02},//DCT_Type
		{0x03},//DCT_Pos
		{0x04},//DCT_Len
		{0x04,0x14,0xff,0xff,0xff,0x00,0x00,0x00},
	},
	
	{
		{GMCN_READDTC_MODE_07},
		{0x04,0x19,0x12,0xff,0x00,0x00,0x00,0x00},
		{0x04},//DCT_Type
		{0x01},//DCT_Pos
		{0x03},//DCT_Len
		{0x01,0x14,0x00,0x00,0x00,0x00,0x00,0x00},
	},
	
	{
		{GMCN_READDTC_MODE_08},
		{0x04,0x18,0x00,0xFF,0x00,0x00,0x00,0x00},
		{0x03},//DCT_Type
		{0x02},//DCT_Pos
		{0x03},//DCT_Len
		{0x03,0x14,0xff,0x00,0x00,0x00,0x00,0x00},
	},

};




//公共函数
bool SetFrameTimeOut_Gmcn(uint8_t index)
{
	uint8_t  ord;
	uint8_t initdata[128];
	uint8_t number,i=0;
	uint32_t canid;
	m_CAN_ANS_T_Gmcn = 0;
	m_CAN_ANS_R_Gmcn = 0;
	
	ord = InitIO_Gmcn[index].Msg_Ord;
	number = InitIO_Gmcn[index].CMD_DATA[0];
	memcpy(initdata,InitIO_Gmcn[index].CMD_DATA+1,number);
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
				m_CAN_LISTEN_TYPE_GMCN =initdata[i+1];
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
				m_CAN_ANS_T_Gmcn = initdata[i+1];
				i+=2;
				break;
			case 0x0e:
				m_CAN_ANS_R_Gmcn = initdata[i+1];
				i+=0x11;
				break;
			case 0x0F:
				i+=2;
				break;
			case 0x12:
				i+=3;
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
void pro_msg_all_Gmcn(GMCN_INIT_MODE mode)
{
	
	SetCanbusDefault_Gmcn(mode);
	switch(mode)
	{
		//CAN
		case GMCN_INIT_MODE_01:
		case GMCN_INIT_MODE_03:
		case GMCN_INIT_MODE_05:
		case GMCN_INIT_MODE_06:
		case GMCN_INIT_MODE_08:
		case GMCN_INIT_MODE_09:
		case GMCN_INIT_MODE_0B:
		case GMCN_INIT_MODE_0C:
		case GMCN_INIT_MODE_0D:
			g_defaultMsgAttr.m_CS_MODE = 0;
			g_defaultMsgAttr.m_ORD     = 0x30;
			g_defaultMsgAttr.m_PCK_TYPE= 0x22;
			g_defaultMsgAttr.m_RWAYS   = 0;
			g_defaultMsgAttr.m_SRC     = 0;
			g_defaultMsgAttr.m_TGT     = 0;
			g_defaultMsgAttr.m_WAYS    = 0;
		break;
		
		//KWP
		case GMCN_INIT_MODE_02:
			g_defaultMsgAttr.m_CS_MODE = 0;
			g_defaultMsgAttr.m_ORD     = 0x33;
			g_defaultMsgAttr.m_PCK_TYPE= 0x02;
			g_defaultMsgAttr.m_RWAYS   = 0;
			g_defaultMsgAttr.m_SRC     = 0xF1;
			g_defaultMsgAttr.m_TGT     = 0x11;
			g_defaultMsgAttr.m_WAYS    = 0;
		break;
		
		//KWP
		case GMCN_INIT_MODE_04:
			g_defaultMsgAttr.m_CS_MODE = 0;
			g_defaultMsgAttr.m_ORD     = 0x33;
			g_defaultMsgAttr.m_PCK_TYPE= 0x02;
			g_defaultMsgAttr.m_RWAYS   = 0;
			g_defaultMsgAttr.m_SRC     = 0xF1;
			g_defaultMsgAttr.m_TGT     = 0x18;
			g_defaultMsgAttr.m_WAYS    = 0;
		break;
		
		//KWP
		case GMCN_INIT_MODE_07:
			g_defaultMsgAttr.m_CS_MODE = 0;
			g_defaultMsgAttr.m_ORD     = 0x33;
			g_defaultMsgAttr.m_PCK_TYPE= 0x02;
			g_defaultMsgAttr.m_RWAYS   = 0;
			g_defaultMsgAttr.m_SRC     = 0xF1;
			g_defaultMsgAttr.m_TGT     = 0x28;
			g_defaultMsgAttr.m_WAYS    = 0;
		break;
		
		//KWP
		case GMCN_INIT_MODE_0E:
			g_defaultMsgAttr.m_CS_MODE = 0;
			g_defaultMsgAttr.m_ORD     = 0x33;
			g_defaultMsgAttr.m_PCK_TYPE= 0x02;
			g_defaultMsgAttr.m_RWAYS   = 0;
			g_defaultMsgAttr.m_SRC     = 0xF1;
			g_defaultMsgAttr.m_TGT     = 0x58;
			g_defaultMsgAttr.m_WAYS    = 0;
		break;
		
		
		//VPW
		case GMCN_INIT_MODE_0A:
			g_defaultMsgAttr.m_CS_MODE = 0;
			g_defaultMsgAttr.m_ORD     = 0x31;
			g_defaultMsgAttr.m_PCK_TYPE= 0x0B;
			g_defaultMsgAttr.m_RWAYS   = 0;
			g_defaultMsgAttr.m_SRC     = 0xF1;
			g_defaultMsgAttr.m_TGT     = 0x40;
			g_defaultMsgAttr.m_WAYS    = 0;
		break;
		
		//VPW
		case GMCN_INIT_MODE_0F:
			g_defaultMsgAttr.m_CS_MODE = 0;
			g_defaultMsgAttr.m_ORD     = 0x31;
			g_defaultMsgAttr.m_PCK_TYPE= 0x0B;
			g_defaultMsgAttr.m_RWAYS   = 0;
			g_defaultMsgAttr.m_SRC     = 0xF1;
			g_defaultMsgAttr.m_TGT     = 0x58;
			g_defaultMsgAttr.m_WAYS    = 0;
		break;
		
		default:
		break;
	}
}

////直接用OBD的
////void ResetComPort()


//初始化IO
bool Init_IO_Gmcn(uint8_t index)
{
	uint16_t number = 0;
	uint8_t sendbuf[128];
	uint8_t recvbuf[32];

	if(!SetFrameTimeOut_Gmcn(index))
	{
		return false;
	}
	memset(recvbuf,0,sizeof(recvbuf));
	number = InitIO_Gmcn[index].CMD_DATA[0];
	memcpy(sendbuf, InitIO_Gmcn[index].CMD_DATA+1, number);
	//g_defaultMsgAttr.m_PCK_TYPE = 0x00;
	number = pro_packsenddata(sendbuf, number, InitIO_Gmcn[index].Msg_Ord);
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


void SetCanbusDefault_Gmcn(GMCN_INIT_MODE mode)
{
	switch(mode)
	{
		case GMCN_INIT_MODE_01:
				m_canbussendid = 0x000007E0;
				m_canbusreadid = 0x000007E8;
				m_canbusflowid = 0x000005E8;
				m_canbusdataid = 0x000005E8;
			break;
		
		case GMCN_INIT_MODE_03:
				m_canbussendid = 0x000007E2;
				m_canbusreadid = 0x000007EA;
				m_canbusflowid = 0x000005EA;
				m_canbusdataid = 0x000005EA;
			break;
		
		case GMCN_INIT_MODE_05:
				m_canbussendid = 0x00000243;
				m_canbusreadid = 0x00000643;
				m_canbusflowid = 0x00000243;
				m_canbusdataid = 0x00000543;
			break;
		
		case GMCN_INIT_MODE_06:
				m_canbussendid = 0x000007E5;
				m_canbusreadid = 0x000007ED;
				m_canbusflowid = 0x000005ED;
				m_canbusdataid = 0x000005ED;
			break;
		
		case GMCN_INIT_MODE_08:
		case GMCN_INIT_MODE_09:
				m_canbussendid = 0x00000241;
				m_canbusreadid = 0x00000641;
				m_canbusflowid = 0x00000241;
				m_canbusdataid = 0x00000541;
			break;
		
		case GMCN_INIT_MODE_0B:
		case GMCN_INIT_MODE_0C:
		case GMCN_INIT_MODE_0D:
				m_canbussendid = 0x00000247;
				m_canbusreadid = 0x00000647;
				m_canbusflowid = 0x00000247;
				m_canbusdataid = 0x00000547;
			break;

		default:
			break;
	}
}

bool pro_much_init_gmcn(uint8_t index)// 进系统
{
	bool bRet = false;
	MSG_ORD_GMCN PackType = InitIO_Gmcn[index].Msg_Ord;
	uint8_t mode = InitIO_Gmcn[index].Init_Mode;
	
	pro_msg_all_Gmcn(mode);

	switch(PackType)
	{
		//CAN
		case MSG_Init_CAN_PackType_Gmcn:
			bRet = pro_canbus_select_module_gmcn(index);
			break;
		
		//K-Line
		case MSG_Init_KWP_PackType_Gmcn:
			bRet = pro_kwp_select_module_gmcn(index);//kwp的81进系统命令
			break;
			
		//VPW
		case MSG_Init_VPW_PackType_Gmcn:
			bRet = pro_vpw_select_module_gmcn(index);//vpw

		default:
			break;
	}
	
	return bRet;
}

bool pro_kwp_select_module_gmcn(uint8_t index)//kwp的81进系统命令
{
	uint16_t number = 0;
	uint8_t sendbuf[128] = {0};
	uint8_t recvbuf[256] = {0};
	uint8_t ord = 0;
	uint8_t PCK_TYPE = 0;
	
	//进系统命令
	memcpy(sendbuf,InitIO_Gmcn[index].System_Enter,InitIO_Gmcn[index].System_Enter[0]+1);
	
	ord = g_defaultMsgAttr.m_ORD;//临时保存
	g_defaultMsgAttr.m_ORD = 0x23;//进系统 临时改成0x23
	
	number = pro_SendRecv_Gmcn(sendbuf, recvbuf);
	
	g_defaultMsgAttr.m_ORD = ord;//还原回去
	
	if(number == 0)
	{
		return false;
	}

	return true;
}

bool pro_vpw_select_module_gmcn(uint8_t index)//vpw
{
	uint16_t number = 0;
	uint8_t sendbuf[128] = {0};
	uint8_t recvbuf[256] = {0};
	uint8_t ord = 0;
	uint8_t PCK_TYPE = 0;
	
	//进系统命令
	memcpy(sendbuf,InitIO_Gmcn[index].System_Enter,InitIO_Gmcn[index].System_Enter[0]+1);

	number = pro_SendRecv_Gmcn(sendbuf, recvbuf);

	if(number == 0)
	{
		return false;
	}

	return true;
}



bool pro_canbus_select_module_gmcn(uint8_t index)
{
	uint8_t sendbuf[128];
	uint8_t recvbuf[256];
	uint16_t number=0;
	
	memset(sendbuf,0,sizeof(sendbuf));
	memcpy(sendbuf,InitIO_Gmcn[index].System_Enter,InitIO_Gmcn[index].System_Enter[0]+1);
	
	number = pro_SendRecv_Gmcn(sendbuf, recvbuf); //进系统 带长度
	
	if(number == 0)
	{
		return false;
	}

	return true;
}



void pro_read_dtc_gmcn(uint8_t index,uint8_t iSystem,int* dtc_number,char* dtc_list,char* dtc_sys) //读码
{
	uint8_t sendcmd[8] = {0}; //读当前码
	uint8_t recvbuf[128] = {0}; //
	uint16_t number = 0;
	uint8_t bitnumber = 0;
	uint8_t dtcnumber = 0;
	uint8_t ndtcnumber = 0;
	bool bFlag = TRUE;
	char dtc[16] = {0};
	char dtcname[32][16] = {0};//最多32个码
	int i = 0,j = 0;
	int total = 0;
	int dtccmd_count = 0;
	uint8_t pos = 0;
	uint8_t dtclen = 0;
	char system[4] = {0};
	GMCN_READDTC_MODE Dtc_mode;
	uint8_t ord = (g_defaultMsgAttr.m_ORD)&0x0f;
	
	sprintf(system, "%d", iSystem);//系统

	//多个读码命令
	for(dtccmd_count = 0 ;dtccmd_count < InitIO_Gmcn[index].DTCMode_Size; dtccmd_count++)
	{
		Dtc_mode = InitIO_Gmcn[index].Gmcn_DTCMode_Group[dtccmd_count];
		memcpy(sendcmd, Gmcn_ReadDtc[Dtc_mode].CMD_Read_DTC, Gmcn_ReadDtc[Dtc_mode].CMD_Read_DTC[0]+1);

		pos = Gmcn_ReadDtc[Dtc_mode].DTC_Pos;
		dtclen = Gmcn_ReadDtc[Dtc_mode].DTC_Len;
		switch(Gmcn_ReadDtc[Dtc_mode].DTC_Type)
		{
			case 0x00:
				number = pro_SendRecv_Gmcn(sendcmd, recvbuf);
				break;//不做处理
			
			
			case 0x02:
				number = pro_SendRecv_Gmcn(sendcmd, recvbuf);
				if(dtclen < 2)
				{
					dtclen = 2;
				}
				dtcnumber = (number-pos)/dtclen;
				for(i=0;i<dtcnumber;i++)
				{
					dtc_sprintf_Gmcn_Mode_1(dtc,recvbuf+i*dtclen+pos);
					//sprintf(dtcbuf, "%s%s,",dtcbuf, strbuf);
					//status_sprintf(statusbuf,recvbuf+i*dtclen+pos);
					
					ndtcnumber++;
					if(*dtc_number != 0)
					{//60个字节，用","分隔
						strcat(dtc_list,",");
						strcat(dtc_sys,",");
					}
					strcat(dtc_list,dtc);
					strcat(dtc_sys,system);
					(*dtc_number)++;
					printfUartObdiideBug("gmcn add dtc:%s system:%s \r\n",dtc,system);
				}
				break;
				
			case 0x03:
				number = pro_SendRecv_Gmcn(sendcmd, recvbuf);
				if(dtclen < 2)
				{
					dtclen = 2;
				}
				dtcnumber = recvbuf[pos-1];
				for(i=0;i<dtcnumber;i++)
				{
					dtc_sprintf_Gmcn_Mode_1(dtc,recvbuf+i*dtclen+pos);
					//sprintf(dtcbuf, "%s%s,",dtcbuf, strbuf);
					//status_sprintf(statusbuf,recvbuf+i*dtclen+pos);
					
					ndtcnumber++;
					if(*dtc_number != 0)
					{//60个字节，用","分隔
						strcat(dtc_list,",");
						strcat(dtc_sys,",");
					}
					strcat(dtc_list,dtc);
					strcat(dtc_sys,system);
					(*dtc_number)++;
					printfUartObdiideBug("gmcn add dtc:%s system:%s \r\n",dtc,system);
				}
				break;
				
			
			case 0x04:
//			<RECE>a5 a5 00 0? 30 00 00 07 e0 03 a9 81 1a *</RECE>
//			<SEND>a5 a5 00 0d 30 00 00 05 e8 81 06 07 00 25 00 00 00 ??</SEND>
//			<SEND>a5 a5 00 0d 30 00 00 05 e8 81 16 77 00 25 00 00 00 ??</SEND>
//			<SEND>a5 a5 00 0d 30 00 00 05 e8 81 00 00 00 ff 00 00 00 ??</SEND>
				number = pro_SendRecv_Gmcn(sendcmd, recvbuf);
				while(1)
				{		
					if(recvbuf[pos] == 0 && recvbuf[pos+1] == 0)
					break;
					
					if(dtcnumber <= 50)//????MAXDTCNUM????
					{
						dtc_sprintf_Gmcn_Mode_1(dtc,recvbuf+pos);
						//status_sprintf(statusbuf,recvbuf+pos);
						//sprintf(dtcbuf, "%s%s,",dtcbuf, strbuf);
					}
					dtcnumber++;
					if(*dtc_number != 0)
					{//60个字节，用","分隔
						strcat(dtc_list,",");
						strcat(dtc_sys,",");
					}
					strcat(dtc_list,dtc);
					strcat(dtc_sys,system);
					(*dtc_number)++;
					printfUartObdiideBug("gmcn add dtc:%s system:%s \r\n",dtc,system);
					
					OBDII_ReadBuf(recvbuf, &number,g_defaultMsgAttr.m_AnswerFrameTimeOut*20);
					number = pro_unpackrecvdata(recvbuf, number);
					if(number == 0)
					{
						break;
					}
					if (ord == 0x01)
					{
						memcpy(recvbuf, recvbuf+3, number);
						number -= 3;
					}
					else if(ord == 0x00)
					{
						memcpy(recvbuf, recvbuf+4, number);
						number -= 4;
					}
				}
				break;//
				
			default:
				break;
		}
		
	}

	printfUartObdiideBug("gmcn dtc_list:%s dtc_number:%d dtc_sys:%s\r\n",dtc_list,*dtc_number,dtc_sys);
	return;
}

void dtc_sprintf_Gmcn_Mode_1(char *strbuf,uint8_t *buf)
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


void status_sprintf_gmcn(char *statusbuf, uint8_t * recvbuf)
{
//	ezxml_t xml;
//	ezxml_t statusxml;
//	U8 strexpress[2048];
//	U8 dtcstatus[512];
//	int statuscode;
//	xml = m_threaddata.xml;
//	statusxml = ezxml_child(xml,"S");
//	m_scount = 0;
//	while(statusxml)
//	{
//		memset(dtcstatus,0,sizeof(dtcstatus));
//		memset(strexpress,0,sizeof(strexpress));
//		strcpy((char *)strexpress,ezxml_attr(statusxml,"CMP"));
//		getCMPstrResult((char *)strexpress,recvbuf,NULL,NULL,NULL);
//		strcpy((char *)dtcstatus,ezxml_txt(statusxml));
//		/*while(strlen((char*)dtcstatus)<20)
//		strcat((char*)dtcstatus," ");*/
//		strcat((char*)dtcstatus,(char*)strexpress);
//		/*while(strlen((char*)dtcstatus)<40)
//			strcat((char*)dtcstatus," ");*/
//		statuscode = pro_statusset((char*)dtcstatus);
//		sprintf(statusbuf, "%s%d,", statusbuf, statuscode);
//		statusxml=ezxml_next(statusxml);
//		m_scount ++;
//	}
}


//void dtc_sprintf_Honda_Mode_2(char *strbuf,uint8_t *buf)
//{
//	sprintf(strbuf, "%06X", (buf[0]) * 0x10000 + buf[1] * 0x100 + buf[2]);
//}


void pro_clear_dtc_gmcn(uint8_t index) //清码
{
	uint8_t sendcmd[8] = {0}; //清码
	uint8_t recvbuf[64] = {0}; //
	uint8_t number = 0;
	uint8_t i = 0;

	//多个命令轮流发送，不做判断
	for	(i = 0; i<InitIO_Gmcn[index].DTCMode_Size; i++)
	{
		memcpy(sendcmd, Gmcn_ReadDtc[i].CMD_Clear_DTC, Gmcn_ReadDtc[i].CMD_Clear_DTC[0]+1);
		number = pro_SendRecv_Gmcn(sendcmd, recvbuf);
	}
}

void pro_exit_system_gmcn(uint8_t index) //退出系统
{
	uint8_t sendcmd[8] = {0}; //退出
	uint8_t recvbuf[64] = {0}; //
	uint8_t number = 0;

	memcpy(sendcmd, InitIO_Gmcn[index].System_Exit, InitIO_Gmcn[index].System_Exit[0]+1);
	number = pro_SendRecv_Gmcn(sendcmd, recvbuf);
}


bool Much_Init_Gmcn(void)
{
	uint8_t i = 0,j = 0;
	uint8_t initstart = 0,initend = 0;

	uint8_t iSystem = 5;//一共5个系统
	
	bool enter = false;
	
	memset(g_vehiclediagnosie.fault_list,0,40);
	memset(g_vehiclediagnosie.sys_list,0,40);
	g_vehiclediagnosie.fault_total = 0;
	
	printfUartObdiideBug("welcome to [%s]\r\n",__func__);
	if(m_first_gmcn)
	{
		//第一次进入
		m_first_gmcn = false;
		
		initstart = 0;//从第一个系统开始扫描时重置
		initend = 0;//从第一个系统开始扫描时重置
		for (i = 0;i<iSystem;i++)
		{
			initstart = initend;
			initend += m_iSystem_Mode_Size_Gmcn[i];
			for (j = initstart; j < initend; j++)
			{
				//reset_global_data();//重置全局变量数据
				
				if(Init_IO_Gmcn(j)) //初始化IO
				{
					if(pro_much_init_gmcn(j)) //初始化ECU //进系统
					{
						printfUartObdiideBug("gmcn enter success system:%d init:%d \r\n",i,j);
						m_protocol_index_gmcn[i] = j;					//
						pro_read_dtc_gmcn(j,i+1,&(g_vehiclediagnosie.fault_total),g_vehiclediagnosie.fault_list,g_vehiclediagnosie.sys_list);	//读码
						pro_clear_dtc_gmcn(j);															//清码
						pro_exit_system_gmcn(j);														//退出
						break;
					}
					else
					{
						printfUartObdiideBug("gmcn enter false system:%d init:%d (%d-%d) \r\n",i,j,initstart,initend-1);
					}
				}
			}//for (j = initstart; j < initend; j++)
		}//for (i = 0;i<iSystem;i++)
		
		for(i = 0;i < iSystem;i++)
		{
				if(m_protocol_index_gmcn[i] != -1)
						return true;
		}
		
	}
	else
	{
		vTaskDelay(3000);
		enter = false;
		for (i = 0;i<iSystem;i++)
		{
			if(m_protocol_index_gmcn[i] != -1)//直接使用记录的协议，如果为-1，则说明不存在此系统
			{
				if(Init_IO_Gmcn(InitIO_Gmcn[m_protocol_index_gmcn[i]].Init_Mode)) //初始化IO
				{
					if(pro_much_init_gmcn(m_protocol_index_gmcn[i])) //初始化ECU
					{
//					if(diag_flag == 1 && data_flag == 0)//车辆诊断
//					{
							enter = true;
							pro_read_dtc_gmcn(m_protocol_index_gmcn[i],i+1,&(g_vehiclediagnosie.fault_total),g_vehiclediagnosie.fault_list,g_vehiclediagnosie.sys_list);	//读码
							pro_exit_system_gmcn(m_protocol_index_gmcn[i]);														//退出
//						diag_flag = 0;
//					}
					}
				}
			}
		}//for (i = 0;i<iSystem;i++)
	}

	return enter;
}


uint16_t pro_SendRecv_Gmcn(uint8_t * sendbuf, uint8_t *recvbuf)
{
	int i,ret;
	
#if defined(TOM_RDWR_DEBUG)		
		printfUartObdiideBug("[%s] sendbuf: \r\n",__func__);
		for(i=0;i<=sendbuf[0];i++)
			printfUartObdiideBug("%02X ",sendbuf[i]);
		printfUartObdiideBug("\r\n");
#endif		
	
	if((g_defaultMsgAttr.m_ORD&0x0F) == 0x00)//CAN
	{
			ret = pro_SendRecv_Gmcn_CAN(sendbuf,recvbuf);
	}
	else
	{	
		if(g_defaultMsgAttr.m_PCK_TYPE == 0x0b)//VPW
		{
			ret = pro_SendRecv(sendbuf, recvbuf);
		}
		else
		{
			ret = pro_SendRecv(sendbuf, recvbuf);
		}
	}
	
#if defined(TOM_RDWR_DEBUG)			
		printfUartObdiideBug("[%s] recvbuf: \r\n",__func__);
		for(i=0;i<ret;i++)
			printfUartObdiideBug("%02X ",recvbuf[i]);
		printfUartObdiideBug("\r\n");
#endif		
	
	return ret;
}

uint16_t pro_SendRecv_Gmcn_CAN(uint8_t * sendbuf, uint8_t *recvbuf)
{
	int number,i;
	uint8_t pckType;
	uint8_t delay = 20;

	pckType = g_defaultMsgAttr.m_PCK_TYPE;

#if defined(TOM_RDWR_DEBUG)
	printfUartObdiideBug("[%s] sendbuf: \r\n",__func__);
	for(i=0;i<=sendbuf[0];i++)
			printfUartObdiideBug("%02x ",sendbuf[i]);
	printfUartObdiideBug("\r\n");
#endif
	
	switch(pckType)
	{
	//之前通用和上号通用里面PCK_TYPE=27
//	case 0x27:
//		number = pro_SendRecv_CanNormal(sendbuf, recvbuf);
//		break;
		
	case 0x22:
		number = pro_SendRecv_CanGM(sendbuf, recvbuf);
		break;
	
	default:
		number = 0;
		break;
	}

	return number;
}

uint16_t pro_SendRecv_CanGM(uint8_t * sendbuf, uint8_t *recvbuf)
{
	uint16_t cmdReturn=0;
	uint16_t maxlen=MAXSIZE;
	uint8_t sendnum=0;
	//uint8_t sendbuf[256];
	uint16_t cansendid, canreadid, candataid, canid = 0;
	uint8_t checkData2;
	
	sendnum = sendbuf[0];
	sendbuf = sendbuf + 1;
	checkData2 = sendbuf[0]+0x40;
	cansendid = m_canbussendid;
	canreadid = m_canbusreadid;
	candataid = m_canbusdataid;
	m_curcanreadid = 0;

	cmdReturn = CommSendBufferCanbus_Gmcn(sendbuf, sendnum);
	if(cmdReturn == 0x00)
	{
		m_canbussendid = cansendid;
		m_canbusreadid = canreadid;
		m_canbusdataid = candataid;
		return 0;
	}
	cmdReturn = CommReadBufferCanbusGM(recvbuf, maxlen, 1000);
	while(cmdReturn > 0x02 && recvbuf[0x00] == 0x7f)
	{
		if(recvbuf[0x02] == 0x78) //chengbb 20130423 ?????????? ????
		{
			vTaskDelay(20);
			cmdReturn = CommReadBufferCanbusGM(recvbuf, maxlen, 5000);
		}
		else if(recvbuf[0x02] == 0x21) //chengbb 20130423 ?????????? ????
		{
			vTaskDelay(20);
			cmdReturn = CommSendBufferCanbus_Gmcn(sendbuf, sendnum);
			cmdReturn = CommReadBufferCanbusGM(recvbuf, maxlen, 1000);
		}
		//chengbb 20130523 ????bug ??????????
		else
		{
			m_NegativeCode = recvbuf[0x02];
			m_ResponseCode = ERRORCODE_NEGATIVERESPONSE;/*????*/
			m_canbussendid = cansendid;
			m_canbusreadid = canreadid;
			m_canbusdataid = candataid;
			return cmdReturn;
		}
	}
	if (cmdReturn >0 && ((sendbuf[0] != 0x3e&&recvbuf[0] == 0x7e)||(sendbuf[0] != 0x20&&recvbuf[0] == 0x60)))//???60??,??????????????????20????????60?? add by liuwf 20170322
	{
		vTaskDelay(20);
		cmdReturn = CommReadBufferCanbusGM(recvbuf, maxlen, 1000);
	}
	if(cmdReturn == 0x00 && m_ResponseCode != ERRORCODE_SUCCESS)
	{
		m_ResponseCode = ERRORCODE_NORESPONSE;/*???*/
		m_canbussendid = cansendid;
		m_canbusreadid = canreadid;
		m_canbusdataid = candataid;
		return 0;
	}
	if (m_curcanreadid == m_canbusreadid)
	{
		/*??SID*/
		if(recvbuf[0] != checkData2)
		{
			m_ResponseCode = ERRORCODE_CHECKSERVICE;/*????ID??*/
			m_canbussendid = cansendid;
			m_canbusreadid = canreadid;
			m_canbusdataid = candataid;
			return 0;
		}
		/*??PID TID MID*/
		if(sendbuf[0] < 0x09 && sendnum > 1 && sendbuf[1] != recvbuf[1])
		{
			m_ResponseCode = ERRORCODE_CHECKPARAMETER;/*????????*/
			m_canbussendid = cansendid;
			m_canbusreadid = canreadid;
			m_canbusdataid = candataid;
			return 0;
		}
	}
	m_canbussendid = cansendid;
	m_canbusreadid = canreadid;
	m_canbusdataid = candataid;
	
	//memset(recvbuf+cmdReturn, 0x00, maxlen-cmdReturn);
	return cmdReturn;
}


uint16_t CommSendBufferCanbus_Gmcn(uint8_t* sendbuf, uint8_t sendnum)
{  
	uint16_t  count=0;
	uint16_t  cmdReturn=0;
	uint8_t framecount; 
	uint8_t frameflag;
	uint8_t flowBS=0;
	uint8_t flowST=0;
	uint8_t buffer[MAXCAN];
	memset(buffer, 0x00, sizeof(buffer));
	if(sendnum == 0)
	{
		m_ResponseCode = ERRORCODE_SENDCOMMAND;/*??????*/
		return 0;
	}
	/*?????? ??OBDII???????*/
	if(sendnum < 8)
	{
		buffer[0] = sendnum;
		memcpy(buffer+1, sendbuf, sendnum);
		count = CommPackBufferCanbus_Gmcn(buffer, sendnum + 1);
		CommSendBufferDefault(buffer, count);
	}
	/*?????? ??OBDII???????  ??????*/
	else
	{ 
		framecount = 6;
		frameflag = 1;
		buffer[0] = sendnum/0x100 + 0x10;
		buffer[1] = sendnum%0x100;
		if (m_CAN_LISTEN_TYPE_GMCN == 0x81)
		{
			memcpy(buffer+2,sendbuf, 6);
			count = CommPackBufferCanbus_Gmcn(buffer, 8);
			CommSendBufferDefault(buffer, count);
			while(framecount<sendnum)
			{
				if(flowBS == 0)
				{
					/*??30???*/
					count = CommReadSingleCanbus(buffer, MAXCAN, 200);
					if(count < 7 || (buffer[4]&0xf0) != 0x30)
					{
						m_ResponseCode = ERRORCODE_FLOWCONTROL;/*CANBUS??????*/
						return 0;
					}
					flowBS=buffer[5];/*?????? 00 ??????*/
					flowST=buffer[6];/*????????  00 ??????*/
					if(flowBS == 0x00)
					{
						flowBS = (sendnum+1-framecount)/7+1;
					}
				}
				buffer[0] = 0x20 + frameflag;
				memcpy(buffer+1,sendbuf+framecount, 7); 
				if(framecount+7 > sendnum)
				{
					count = sendnum-framecount;
					memset(buffer+count+1, 0x00, 7-count);
				}
				vTaskDelay(flowST);
				count = CommPackBufferCanbus_Gmcn(buffer, 8);
				CommSendBufferDefault(buffer, count);
				framecount += 7;
				if(frameflag++ > 0x0f)
				{
					frameflag = 0;
				}
				flowBS--;
			}
		}
		else if ((m_CAN_LISTEN_TYPE_GMCN == 0x82) || (m_CAN_LISTEN_TYPE_GMCN == 0x83))
		{
			memcpy(buffer + 2, sendbuf, sendnum);
			count = CommPackBufferCanbus_Gmcn(buffer, sendnum + 2);
			CommSendBufferDefault(buffer, count);
		}
	}
	cmdReturn = 1;
	return cmdReturn;
}


uint16_t CommPackBufferCanbus_Gmcn(uint8_t *buffer, uint16_t count)
{ 
	uint16_t  cmdReturn=count;
	while(cmdReturn-- > 0)
	{
		buffer[cmdReturn+5] = buffer[cmdReturn];
	}
	buffer[0x00] = 0x30;
	buffer[0x01] = (m_canbussendid>>24)&0xff;
	buffer[0x02] = (m_canbussendid>>16)&0xff;
	buffer[0x03] = (m_canbussendid>>8)&0xff;
	buffer[0x04] = m_canbussendid&0xff;
	cmdReturn = CommPackBufferDefault_Gmcn(buffer, count+5);
	return cmdReturn;
}

uint16_t CommPackBufferDefault_Gmcn(uint8_t *buffer, uint16_t count)
{
	uint16_t  i;
	uint16_t  cmdReturn=count;
	while(cmdReturn-- > 0)
	{
		buffer[cmdReturn+4] = buffer[cmdReturn];
	}
	cmdReturn = count; 
	buffer[0x00] = 0xa5;
	buffer[0x01] = 0xa5;
	buffer[0x02] = cmdReturn/0x100;
	buffer[0x03] = cmdReturn%0x100;
	cmdReturn = count+5; 
	buffer[cmdReturn-1] = 0x00;
	for(i=2;i<cmdReturn-1;i++)
	{
		buffer[cmdReturn-1] += buffer[i];
	}
	buffer[cmdReturn-1] = ~buffer[cmdReturn-1];
	return cmdReturn;
}


uint16_t CommReadBufferCanbusGM(uint8_t* readbuf, uint16_t maxlen, uint16_t delay)
{
	uint16_t cmdReturn=0;
	uint16_t count=0;
	uint16_t canid=0;
	uint16_t errnum=0;
	uint16_t framecount; 
	uint8_t frameflag;
	uint8_t flowBS=0x00;/*??????  ??????*/
	uint8_t flowST=0x00;/*????????  ??10ms*/
	uint8_t buffer[MAXCAN];
	while((m_canbusreadid != canid) && (m_canbusdataid != canid))
	{
		count = CommReadBufferDefault(buffer, maxlen, delay);
		count = CommUnpackBufferDeafult(buffer, count);
		if(count < 4 || errnum++ > 20)
		{
			m_ResponseCode = ERRORCODE_NORESPONSE;/*???*/
			return 0;
		}
		canid = (buffer[0]<<24) + (buffer[1]<<16) + (buffer[2]<<8) + buffer[3];	

	}
	if (m_canbusreadid == canid)
	{
		if(buffer[4] < 0x08)
		{	
			cmdReturn = buffer[4];
			memcpy(readbuf, buffer+5, cmdReturn);
		}
		else if((buffer[4]&0xf0) == 0x10)
		{
			cmdReturn = (buffer[4]&0x0f)*0x100 + buffer[5];
			memcpy(readbuf, buffer+6, 6);
			if(m_CAN_LISTEN_TYPE_GMCN == 0x81)
			{
				memset(buffer, 0x00, sizeof(buffer));
				buffer[0x00] = 0x30;
				buffer[0x01] = flowBS;
				buffer[0x02] = flowST;
				/*??30???*/
				count = CommPackBufferCanbus_Gmcn(buffer, 8);
				CommSendBufferDefault(buffer, count);
			}
			framecount = 6;
			frameflag  = 1;
			while(framecount < cmdReturn)
			{
				count = CommReadBufferDefault(buffer, maxlen, delay);
				count = CommUnpackBufferDeafult(buffer, count);
				if(count < 4)
				{
					m_ResponseCode = ERRORCODE_FLOWCONTROL;/*CANBUS??????*/
					return 0;
				}
				if(buffer[4] != 0x20+frameflag)
				{
					m_ResponseCode = ERRORCODE_CONSECUTIVEFRAME;/*CANBUS??????*/
					return 0;
				}
				memcpy(readbuf+framecount, buffer+5, 7);
				frameflag++;
				if(frameflag > 0x0f)
				{
					frameflag = 0;
				}
				framecount += 7;
			}
		}
	}
	else if (m_canbusdataid == canid)
	{
		cmdReturn = 8;
		memcpy(readbuf, buffer+4, cmdReturn);
	}
	m_curcanreadid = canid;
	return cmdReturn;
}


uint16_t CommSendBufferDefault(uint8_t *buffer, uint16_t count)
{
	//Uart_ClearInput(g_carportHandle);
	//Uart_ClearOutput(g_carportHandle);
	//cmdReturn = Device_SendBuf(g_carportHandle, buffer , count);
	uint16_t number = USB_BULK_Queue_Write(&USB_RX_QueueData,buffer,count);
	
	return number;
}


//uint8_t GetBitDtc(uint8_t *recvbuf,uint8_t pos,uint8_t *Bit)//本田0x08读码模式用到，获取这个命令有效的位
//{
//	uint8_t j = 0;
//	uint8_t iBit = 0;
//	uint8_t mask = 0x01;

//	for	(j=1;j<8;j++)
//	{
//		if(recvbuf[pos]&mask)
//		{
//			Bit[iBit] = recvbuf[pos]&mask;
//			iBit++;
//			mask = mask<<1;
//		}
//	}

//	return iBit;
//}
















/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
