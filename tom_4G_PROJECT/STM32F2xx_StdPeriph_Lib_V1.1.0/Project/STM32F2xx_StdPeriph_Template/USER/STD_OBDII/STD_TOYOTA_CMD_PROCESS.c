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
#include "STD_TOYOTA_CMD_PROCESS.h"
#include "FreeRTOS.h"
#include "task.h"
#include "STD_OBD_CMD_PROCESS.h"


uint8_t TOYOTA_iSystem_Mode_Size[6] = {0,6,3,3,1,3};//ÿ��ϵͳ���ڵ�Э��ĸ�������һ��ɨ���ʱ��Ҫ�õ�(��Ϊ�����ϵͳ�Ǵ�1��ʼ��ŵģ���������ĵڶ�����˵�0��)
intptr_t m_protocol_index_Toyota[6] = {-1,-1,-1,-1,-1,-1};   //��¼��⵽��Э��,5��ϵͳ��,��ʼ��Ϊ-1(��Ϊ�����ϵͳ�Ǵ�1��ʼ��ŵģ���������ĵڶ�����˵�0��)
bool m_first_toyota = true;

uint8_t m_CAN_ANS_T_Toyota;
uint8_t m_CAN_ANS_R_Toyota;
uint8_t m_CAN_LISTEN_TYPE_Toyota;
extern uint32_t m_canbussendid_Toyota;/*ϵͳĬ�Ϸ���ID*/
extern uint32_t m_canbusflowid_Toyota;/*��ǰϵͳ����ID*/
extern uint32_t m_canbusreadid;/*��ǰϵͳ����ID*/
extern uint32_t m_canbusflowidchang_Toyota;/*��Ҫ�ı�ķ���ID*/
extern uint32_t m_canbusreadidchang_Toyota;/*��Ҫ�ı�Ľ���ID*/

extern DEFAULT_MSGATTR g_defaultMsgAttr; 

//bool b_add_6a_toyota = false;

//uint8_t data_flag = 0;
//uint8_t diag_flag = 0;

extern Vehicle_diagnosis g_vehiclediagnosie;
extern Data_collection g_datacollection;

static uint32_t m_modulebuf[256];
extern uint8_t m_modulenum;
extern uint8_t m_setmodule;
extern uint32_t m_canbussendid;
extern uint32_t m_canbusflowid;

extern uint8_t data_flag;
extern uint8_t diag_flag;

//��ʼ��IO����
const CMD_INIT_TOYOTA InitIoCmd_Toyota[]=\
{
	// ���
	// ��ʼ��IO�Ĵ����ʽORD 10 11 12 13 ��
	// ��ϵͳ����
	// ��ϵͳ����
	// ��������
	// ����ģʽ
	// ���Posλ
	// ��������
	
	//������
	{//CAN
		{TOYOTA_MODE_01},
		{MSG_Init_CAN_PackType_Toyota},
		{0x41,0x01,0x07,0xA1,0x20,0x02,0x7F,0x03,0x00,0x04,0x00,0x00,0x07,0xe0,0x05,0x00,0x00,0x07,0xe8,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6,0x0d,0x01,0x0e,0x01,0x00,0x09,0x30,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x0a},
		{0x01,0x3E},
		{0x00,0x00},
		{0x02,0x13,0xB0},
		{DTC_MODE_TOYOTA_MODE_01},
		{0x02},
		{0x01,0x04}
	},
	{//CAN
		{TOYOTA_MODE_02},
		{MSG_Init_CAN_PackType_Toyota},
		{0x41,0x01,0x07,0xA1,0x20,0x02,0x7F,0x03,0x00,0x04,0x00,0x00,0x07,0xe0,0x05,0x00,0x00,0x07,0xe8,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6,0x0d,0x01,0x0e,0x01,0x00,0x09,0x30,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x0a},
		{0x02,0x3E,0x01},
		{0x00,0x00},
		{0x02,0x13,0x81},
		{DTC_MODE_TOYOTA_MODE_01},
		{0x02},
		{0x01,0x14}
	},
	{//UDS
		{TOYOTA_MODE_03},
		{MSG_Init_CAN_PackType_Toyota},
		{0x41,0x01,0x07,0xA1,0x20,0x02,0x7F,0x03,0x00,0x04,0x00,0x00,0x07,0x00,0x05,0x00,0x00,0x07,0x08,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6,0x0d,0x01,0x0e,0x01,0x00,0x09,0x30,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x0a},
		{0x02,0x3E,0x00},
		{0x00,0x00},
		{0x03,0x19,0x02,0x0D},
		{DTC_MODE_TOYOTA_MODE_02},
		{0x03},
		{0x01,0x04}
	},
	{//KWP 9600 13
		{TOYOTA_MODE_04},
		{MSG_Init_KWP_PackType_Toyota},
		{0x2B,0x01,0x00,0x25,0x80,0x02,0x00,0x03,0xC0,0x04,0x77,0x00,0xC1,0xC1,0x05,0x0d,0x19,0x37,0x04,0x04,0x00,0x19,0x06,0x01,0x07,0x02,0x00,0x22,0x80,0x10,0x08,0x35,0x09,0x03,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
		{0x01,0x81},
		{0x01,0x82},
		{0x01,0x13},
		{DTC_MODE_TOYOTA_MODE_01},
		{0x02},
		{0x01,0x04}
	},
	{//KWP 10400 10
		{TOYOTA_MODE_05},
		{MSG_Init_KWP_PackType_Toyota},
		{0x2B,0x01,0x00,0x28,0xa0,0x02,0x00,0x03,0xC0,0x04,0x77,0x00,0xC1,0xC1,0x05,0x0d,0x19,0x37,0x04,0x04,0x00,0x19,0x06,0x01,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x35,0x09,0x03,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
		{0x01,0x81},
		{0x01,0x82},
		{0x02,0x13,0xB0},
		{DTC_MODE_TOYOTA_MODE_01},
		{0x02},
		{0x01,0x04}
	},
	{//KWP 10400 13
		{TOYOTA_MODE_06},
		{MSG_Init_KWP_PackType_Toyota},
		{0x2B,0x01,0x00,0x28,0xa0,0x02,0x00,0x03,0xC0,0x04,0x77,0x00,0xC1,0xC1,0x05,0x0d,0x19,0x37,0x04,0x04,0x00,0x19,0x06,0x01,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x35,0x09,0x03,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
		{0x01,0x81},
		{0x01,0x82},
		{0x02,0x13,0xB0},
		{DTC_MODE_TOYOTA_MODE_01},
		{0x02},
		{0x01,0x04}
	},
	
	//Trans
	{//CAN
		{TOYOTA_MODE_07},
		{MSG_Init_CAN_PackType_Toyota},
		{0x41,0x01,0x07,0xA1,0x20,0x02,0x7F,0x03,0x00,0x04,0x00,0x00,0x07,0x27,0x05,0x00,0x00,0x07,0x2f,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6,0x0d,0x01,0x0e,0x01,0x00,0x09,0x30,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x0a},
		{0x01,0x3e},		
		{0x00,0x00},
		{0x02,0x13,0x80},
		{DTC_MODE_TOYOTA_MODE_01},
		{0x02},
		{0x01,0x14}
	},
	{//CAN
		{TOYOTA_MODE_08},
		{MSG_Init_CAN_PackType_Toyota},
		{0x41,0x01,0x07,0xA1,0x20,0x02,0x7F,0x03,0x00,0x04,0x00,0x00,0x07,0x01,0x05,0x00,0x00,0x07,0x09,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6,0x0d,0x01,0x0e,0x01,0x00,0x09,0x30,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x0a},
		{0x02,0x3e,0x00},
		{0x00,0x00},
		{0x03,0x19,0x02,0x0D},
		{DTC_MODE_TOYOTA_MODE_02},
		{0x03},
		{0x02,0x10,0x01}
	},
	{//KWP 9600 1C
		{TOYOTA_MODE_09},
		{MSG_Init_KWP_PackType_Toyota},
		{0x2B,0x01,0x00,0x25,0x80,0x02,0x00,0x03,0xC0,0x04,0x77,0x00,0xC1,0xC1,0x05,0x0d,0x19,0x37,0x04,0x04,0x00,0x19,0x06,0x01,0x07,0x02,0x00,0x22,0x80,0x10,0x08,0x35,0x09,0x03,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
		{0x01,0x81},
		{0x01,0x82},
		{0x01,0x13},
		{DTC_MODE_TOYOTA_MODE_01},
		{0x02},
		{0x01,0x14}
	},
	
	//ABS
	{//CAN
		{TOYOTA_MODE_0A},
		{MSG_Init_CAN_PackType_Toyota},
		{0x41,0x01,0x07,0xA1,0x20,0x02,0x7F,0x03,0x00,0x04,0x00,0x00,0x07,0xb0,0x05,0x00,0x00,0x07,0xb8,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6,0x0d,0x01,0x0e,0x01,0x00,0x09,0x30,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x0a},
		{0x02,0x3E,0x01},
		{0x00,0x00},
		{0x02,0x13,0x80},
		{DTC_MODE_TOYOTA_MODE_01},
		{0x02},
		{0x01,0x14}
	},
	{//KWP 9600 29
		{TOYOTA_MODE_0B},
		{MSG_Init_KWP_PackType_Toyota},
		{0x2B,0x01,0x00,0x25,0x80,0x02,0x00,0x03,0xC0,0x04,0x77,0x00,0xC1,0xC1,0x05,0x0d,0x19,0x37,0x04,0x04,0x00,0x19,0x06,0x01,0x07,0x02,0x00,0x22,0x80,0x10,0x08,0x35,0x09,0x03,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
		{0x01,0x81},
		{0x01,0x82},
		{0x01,0x13},
		{DTC_MODE_TOYOTA_MODE_01},
		{0x02},
		{0x01,0x14}
	},
	{//KWP 10400 29
		{TOYOTA_MODE_0C},
		{MSG_Init_KWP_PackType_Toyota},
		{0x2B,0x01,0x00,0x28,0xa0,0x02,0x00,0x03,0xC0,0x04,0x77,0x00,0xC1,0xC1,0x05,0x0d,0x19,0x37,0x04,0x04,0x00,0x19,0x06,0x01,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x35,0x09,0x03,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
		{0x01,0x81},
		{0x01,0x82},
		{0x01,0x13},
		{DTC_MODE_TOYOTA_MODE_01},
		{0x02},
		{0x01,0x14}
	},
	
	//Body
	{//CAN
		{TOYOTA_MODE_0D},
		{MSG_Init_CAN_PackType_Toyota},
		{0x41,0x01,0x07,0xA1,0x20,0x02,0x7F,0x03,0x00,0x04,0x00,0x00,0x07,0x50,0x05,0x00,0x00,0x07,0x58,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6,0x0d,0x02,0x0e,0x02,0x00,0x09,0x30,0x40,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x0a},
		{0x01,0x3E},
		{0x00,0x00},
		{0x02,0x13,0x81},
		{DTC_MODE_TOYOTA_MODE_01},
		{0x02},
		{0x01,0x14}
	},
	
	//SRS
	{//CAN
		{TOYOTA_MODE_0E},
		{MSG_Init_CAN_PackType_Toyota},
		{0x41,0x01,0x07,0xA1,0x20,0x02,0x7F,0x03,0x00,0x04,0x00,0x00,0x07,0x80,0x05,0x00,0x00,0x07,0x88,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6,0x0d,0x01,0x0e,0x01,0x00,0x09,0x30,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x0a},
		{0x02,0x3E,0x01},
		{0x00,0x00},
		{0x02,0x13,0x81},
		{DTC_MODE_TOYOTA_MODE_01},
		{0x02},
		{0x01,0x14}
	},
	{//KWP 9600 58
		{TOYOTA_MODE_0F},
		{MSG_Init_KWP_PackType_Toyota},
		{0x2B,0x01,0x00,0x25,0x80,0x02,0x00,0x03,0xC0,0x04,0x77,0x00,0xC1,0xC1,0x05,0x0d,0x19,0x37,0x04,0x04,0x00,0x19,0x06,0x01,0x07,0x02,0x00,0x22,0x80,0x10,0x08,0x35,0x09,0x03,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
		{0x01,0x81},
		{0x01,0x82},
		{0x03,0x13,0x00,0x00},
		{DTC_MODE_TOYOTA_MODE_01},
		{0x02},
		{0x01,0x14}
	},
	{//KWP 10400 58
		{TOYOTA_MODE_10},
		{MSG_Init_KWP_PackType_Toyota},
		{0x2B,0x01,0x00,0x28,0xa0,0x02,0x00,0x03,0xC0,0x04,0x77,0x00,0xC1,0xC1,0x05,0x0d,0x19,0x37,0x04,0x04,0x00,0x19,0x06,0x01,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x35,0x09,0x03,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
		{0x01,0x81},
		{0x01,0x82},
		{0x02,0x13,0x81},
		{DTC_MODE_TOYOTA_MODE_01},
		{0x02},
		{0x01,0x14}
	},
	
	{//CAN ����Ƿ�������һ�������������TOYOTA_MODE_02��·��ȫһ���ģ�ֻ�Ƕ���������������һ���������TOYOTA_MODE_02�Ķ���ʧ����������
		{TOYOTA_MODE_11},
		{MSG_Init_CAN_PackType_Toyota},
		{0x41,0x01,0x07,0xA1,0x20,0x02,0x7F,0x03,0x00,0x04,0x00,0x00,0x07,0xe0,0x05,0x00,0x00,0x07,0xe8,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6,0x0d,0x01,0x0e,0x01,0x00,0x09,0x30,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x0a},
		{0x02,0x3E,0x01},
		{0x00,0x00},
		{0x02,0x13,0xB0},
		{DTC_MODE_TOYOTA_MODE_01},
		{0x02},
		{0x01,0x04}
	},
	
};



// ECU INIT
//const OBDII_CMD_INIT InitEcuCmd_Toyota[]=\
//{
//	{
//		{TOYOTA_MODE_01},
//		{0},
//		{0}
//	},
//	{
//		{TOYOTA_MODE_02},
//		{0},
//		{0}
//	},
//	{
//		{TOYOTA_MODE_03},
//		{MSG_Init_Ecu_KWP_PackType_Toyota},
//		{01,0x81}
//	},
//	{
//		{TOYOTA_MODE_04},
//		{MSG_Init_Ecu_KWP_PackType_Toyota},
//		{01,0x81}
//	},
//	{
//		{TOYOTA_MODE_05},
//		{MSG_Init_Ecu_KWP_PackType_Toyota},
//		{01,0x81}
//	}
//};

//ECU select
//const OBDII_CMD_INIT EcuSelectCmd_Toyota[]=\
//{
//	{
//		{TOYOTA_MODE_01},
//		{MSG_ALL_CAN_PackType_Toyota},
//		{0x01,0x3e}
//	},
//	{
//		{TOYOTA_MODE_02},
//		{MSG_ALL_CAN_PackType_Toyota},
//		{0x02,0x3e,0x00}
//	},
//	{
//		{TOYOTA_MODE_03},
//		{MSG_ALL_KWP_PackType_Toyota},
//		{0x02,0x01,0x00}
//	},
//	{
//		{TOYOTA_MODE_04},
//		{MSG_ALL_KWP_PackType_Toyota},
//		{0x02,0x01,0x00}
//	},
//	{
//		{TOYOTA_MODE_05},
//		{MSG_ALL_KWP_PackType_Toyota},
//		{0x02,0x01,0x00}
//	},

//};


//��������
bool SetFrameTimeOut_Toyota(INIT_MODE_TOYOTA index)
{
	uint8_t  ord;
	uint8_t initdata[128];
	uint8_t number,i=0;
	uint32_t canid;
	m_CAN_ANS_T_Toyota = 0;
	m_CAN_ANS_R_Toyota = 0;
	
	ord = InitIoCmd_Toyota[index].Msg_Ord;
	number = InitIoCmd_Toyota[index].CMD_DATA[0];
	memcpy(initdata,InitIoCmd_Toyota[index].CMD_DATA+1,number);
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
				m_CAN_LISTEN_TYPE_Toyota =initdata[i+1];
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
//				m_CAN_ANS_T_Toyota = initdata[i+1];
				i+=2;
				break;
			case 0x0e:
//				m_CAN_ANS_R_Toyota = initdata[i+1];
				i+=0x11;
				break;
			case 0x0F:
				i+=2;
				break;
			default:
				//����
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


//����ECU��ʼ��ORD
//void pro_set_init_ecu_ord_Toyota(INIT_MODE_TOYOTA mode)
//{
//	switch(mode)
//	{
//		case OBDII_MODE_05:
//		case OBDII_MODE_06:
//		case OBDII_MODE_07:
//				g_defaultMsgAttr.m_ORD = OBDII_MSG_Init_Ecu_KWP_PackType;
//		  break;
//		default:
//			break;
//	}
//}


//����ͨ�Ź���ORD
void pro_msg_all_Toyota(INIT_MODE_TOYOTA mode)
{
	cmd_set_two_id_toyota(0x00);
	switch(mode)
	{
		//CAN
		case TOYOTA_MODE_01:
		case TOYOTA_MODE_02:
		case TOYOTA_MODE_03:
		case TOYOTA_MODE_07:
		case TOYOTA_MODE_08:
		case TOYOTA_MODE_0A:
		case TOYOTA_MODE_0E:
			g_defaultMsgAttr.m_CS_MODE = 0;
			g_defaultMsgAttr.m_ORD     = 0x30;
			g_defaultMsgAttr.m_PCK_TYPE= 0x20;
			g_defaultMsgAttr.m_RWAYS   = 0;
			g_defaultMsgAttr.m_SRC     = 0;
			g_defaultMsgAttr.m_TGT     = 0;
			g_defaultMsgAttr.m_WAYS    = 0;
		break;
		
		//CAN
		//����Ƚ����⣬Ŀǰ��Ϊֻ����һ��ϵͳ�Ǵ��ڶ�id�ģ��ݹ�д������
		//toyota ����� main body �ĵڶ�id��40
		//a5 a5 00 0d 30 00 00 07 50 40 01 3e 00 00 00 00 00 ??
		case TOYOTA_MODE_0D:
			cmd_set_two_id_toyota(0x40);
			g_defaultMsgAttr.m_CS_MODE = 0;
			g_defaultMsgAttr.m_ORD     = 0x30;
			g_defaultMsgAttr.m_PCK_TYPE= 0x21;
			g_defaultMsgAttr.m_RWAYS   = 0;
			g_defaultMsgAttr.m_SRC     = 0;
			g_defaultMsgAttr.m_TGT     = 0;
			g_defaultMsgAttr.m_WAYS    = 0;
		break;
		
		//KWP
		case TOYOTA_MODE_04:
		case TOYOTA_MODE_06:
			g_defaultMsgAttr.m_CS_MODE = 0;
			g_defaultMsgAttr.m_ORD     = 0x33;
			g_defaultMsgAttr.m_PCK_TYPE= 0x02;
			g_defaultMsgAttr.m_RWAYS   = 0;
			g_defaultMsgAttr.m_SRC     = 0xF1;
			g_defaultMsgAttr.m_TGT     = 0x13;
			g_defaultMsgAttr.m_WAYS    = 0;
		break;
	
		case TOYOTA_MODE_05:
			g_defaultMsgAttr.m_CS_MODE = 0;
			g_defaultMsgAttr.m_ORD     = 0x33;
			g_defaultMsgAttr.m_PCK_TYPE= 0x02;
			g_defaultMsgAttr.m_RWAYS   = 0;
			g_defaultMsgAttr.m_SRC     = 0xF1;
			g_defaultMsgAttr.m_TGT     = 0x10;
			g_defaultMsgAttr.m_WAYS    = 0;
		break;
		
		case TOYOTA_MODE_09:
			g_defaultMsgAttr.m_CS_MODE = 0;
			g_defaultMsgAttr.m_ORD     = 0x33;
			g_defaultMsgAttr.m_PCK_TYPE= 0x02;
			g_defaultMsgAttr.m_RWAYS   = 0;
			g_defaultMsgAttr.m_SRC     = 0xF1;
			g_defaultMsgAttr.m_TGT     = 0x1C;
			g_defaultMsgAttr.m_WAYS    = 0;
		break;
		
		case TOYOTA_MODE_0B:
		case TOYOTA_MODE_0C:
			g_defaultMsgAttr.m_CS_MODE = 0;
			g_defaultMsgAttr.m_ORD     = 0x33;
			g_defaultMsgAttr.m_PCK_TYPE= 0x02;
			g_defaultMsgAttr.m_RWAYS   = 0;
			g_defaultMsgAttr.m_SRC     = 0xF1;
			g_defaultMsgAttr.m_TGT     = 0x29;
			g_defaultMsgAttr.m_WAYS    = 0;
		break;
		
		case TOYOTA_MODE_0F:
		case TOYOTA_MODE_10:
			g_defaultMsgAttr.m_CS_MODE = 0;
			g_defaultMsgAttr.m_ORD     = 0x33;
			g_defaultMsgAttr.m_PCK_TYPE= 0x02;
			g_defaultMsgAttr.m_RWAYS   = 0;
			g_defaultMsgAttr.m_SRC     = 0xF1;
			g_defaultMsgAttr.m_TGT     = 0x58;
			g_defaultMsgAttr.m_WAYS    = 0;
		break;
		
		default:
		break;
	}
}

//ֱ����OBD��
//void ResetComPort()



//��ʼ��IO
bool Init_IO_Toyota(INIT_MODE_TOYOTA index)
{
	uint16_t number = 0;
	INIT_MODE_TOYOTA mode = 0;
	uint8_t sendbuf[128];
	uint8_t recvbuf[32];
	if(!SetFrameTimeOut_Toyota(index))
	{
		return false;
	}
	memset(recvbuf,0,sizeof(recvbuf));
	number = InitIoCmd_Toyota[index].CMD_DATA[0];
	memcpy(sendbuf, InitIoCmd_Toyota[index].CMD_DATA+1, number);
	mode = InitIoCmd_Toyota[index].Init_Mode;
	g_defaultMsgAttr.m_PCK_TYPE = 0x00;
	number = pro_packsenddata(sendbuf, number, InitIoCmd_Toyota[index].Msg_Ord);
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
		printfuartdebug("InitIO false\r\n");
		return false;
	}
	vTaskDelay(1000);
	pro_set_can_id_Toyota(index);
	printfuartdebug("InitIO true\r\n");
	return true;
}


void pro_set_can_id_Toyota(INIT_MODE_TOYOTA mode)
{
	switch(mode)
	{
		case TOYOTA_MODE_01:
				m_canbussendid = 0x000007E0;
				m_canbusreadid = 0x000007E8;
				m_canbusflowid = 0x000007E0;
			break;
		
		case TOYOTA_MODE_02:
				m_canbussendid = 0x00000700;
				m_canbusreadid = 0x000007E8;
				m_canbusflowid = 0x00000700;
			break;

		default:
			break;
	}
}

bool pro_much_init_Toyota(INIT_MODE_TOYOTA mode)// CANЭ���ϵͳ
{
	uint8_t sendbuf[128] = {0};
	bool bRet = false;
	MSG_ORD_TOYOTA PackType =  InitIoCmd_Toyota[mode].Msg_Ord;
	
	memset(sendbuf,0,sizeof(sendbuf));
	
	switch(PackType)
	{
		//CAN
		case MSG_Init_CAN_PackType_Toyota:
			pro_msg_all_Toyota(mode);
			bRet = pro_canbus_select_module_Toyota(mode);
			break;
		
		//KWP
		case MSG_Init_KWP_PackType_Toyota:
			pro_msg_all_Toyota(mode);
			bRet = pro_kwp_select_module_Toyota(mode);//kwp��81��ϵͳ����
			break;
			
		default:
			break;
	}
	
	return bRet;
}
//p0001,p0002,
bool pro_kwp_select_module_Toyota(INIT_MODE_TOYOTA mode)//kwp��81��ϵͳ����
{
	uint16_t number = 0;
	uint8_t sendbuf[128] = {0};
	uint8_t recvbuf[256] = {0};
	uint8_t ord = 0;
	
	memcpy(sendbuf,InitIoCmd_Toyota[mode].System_Enter,InitIoCmd_Toyota[mode].System_Enter[0]+1);
	
	ord = g_defaultMsgAttr.m_ORD;//��ʱ����
	g_defaultMsgAttr.m_ORD = 0x23;//��ϵͳ ��ʱ�ĳ�0x23

	number = pro_SendRecv(sendbuf, recvbuf);
	
	g_defaultMsgAttr.m_ORD = ord;//��ԭ��ȥ
	
	if(number == 0)
	{
		return false;
	}

	return true;
}

bool pro_canbus_select_module_Toyota(INIT_MODE_TOYOTA mode)
{
	int i, j;
	uint8_t sendbuf[128];
	uint8_t recvbuf[256];
	uint8_t sendnum =0;
	uint16_t number=0;
	uint32_t canid;
	
	sendnum = InitIoCmd_Toyota[mode].System_Enter[0];
	memset(sendbuf,0,sizeof(sendbuf));
	memcpy(sendbuf, InitIoCmd_Toyota[mode].System_Enter,sendnum+1);
	
	number = pro_SendRecv(sendbuf, recvbuf); //��ϵͳ ������
	
	if(number<1)
	{
		return false;
	}

	return true;
}


//Ŀǰ���ֵĹ�����AVANZA��EFI��������������ͨ�ķ����������Ƚϴ󣬵�����˼�����ϲ��
//0700�ĺ�07E0��������Ƚϴ� �������Ͳ���
//0001��003d���������
//��Ҫ�˶���Engine	00ce.xml EFI	0246.xml EFI	024f.xml ��ϵͳ�����Ƿ��д���,�˶Ժ�������
//EU	CAN	F65#	1508-1609	A/T		AVANZA	182A	024f.xml
//EU	CAN	B10#LA	1KR-DE			M/T			WIGO	1BFD 0247.xml

INIT_MODE_TOYOTA pro_read_dtc_Toyota(SYSTEM_TOYOTA sys,INIT_MODE_TOYOTA index,int* dtc_number,char* dtc_list,char* dtc_sys)//����
{
	uint8_t sendcmd[8] = {0}; //����ǰ��
	uint8_t recvbuf[128] = {0}; //
	uint8_t pos = InitIoCmd_Toyota[index].DTC_Pos;
	uint8_t number = 0;
	uint8_t dtcnumber = 0;
	uint8_t ndtcnumber = 0;
	bool bFlag = TRUE;
	char dtc[16] = {0};
	char dtcname[32][16] = {0};//���32����
	char dtcsystem[4] = {0};
	int i = 0;
	int total = 0;

	sprintf(dtcsystem, "%d", sys);//ϵͳ
	memcpy(sendcmd, InitIoCmd_Toyota[index].DTC_Read,InitIoCmd_Toyota[index].DTC_Read[0]+1);
	number = pro_SendRecv(sendcmd, recvbuf); //��ǰ������
	if(number == 0)
	{
		//TOYOTA_MODE_11 
		if(index == TOYOTA_MODE_02)
		{//����Ƿ�������һ�������������TOYOTA_MODE_02��·��ȫһ���ģ�ֻ�Ƕ���������������һ���������TOYOTA_MODE_02�Ķ���ʧ����������
			memcpy(sendcmd, InitIoCmd_Toyota[TOYOTA_MODE_11].DTC_Read,InitIoCmd_Toyota[TOYOTA_MODE_11].DTC_Read[0]+1);
			number = pro_SendRecv(sendcmd, recvbuf); //��ǰ������
			if(number == 0)
			{
				return index;
			}
			index = TOYOTA_MODE_11;//�������ɹ����޸�ΪTOYOTA_MODE_11
		}
		return index;
	}
	
	switch(InitIoCmd_Toyota[index].DTC_Mode)
	{
		case DTC_MODE_TOYOTA_MODE_01:
			
			dtcnumber = (number-pos) / 2;
			for(i = 0; i < dtcnumber; i++)
			{
				if((recvbuf[pos + i * 2] == 0) && (recvbuf[pos + 1 + i * 2] == 0))//0000��������˵�
				{
					continue;
				}

				dtc_sprintf_Toyota_Mode_1((char *)dtc, recvbuf + i * 2 + pos);
				dtc_sprintf_Toyota_Mode_1((char *)dtcname[ndtcnumber], recvbuf + i * 2 + pos); // �ѹ����������
				ndtcnumber++;
				bFlag = TRUE;
				for (total = 0; total < ndtcnumber - 1; total++) // ȥ�ظ�
				{
					if (strcmp((char*)dtc, (char*)dtcname[total]) == 0)
					{
						bFlag = FALSE; // �ظ�
						break;
					}
				}
				if(bFlag) // ���ظ�����ӽ�ȥ
				{
					if(*dtc_number != 0)
					{//60���ֽڣ���","�ָ�
						strcat(dtc_list,",");
						strcat(dtc_sys,",");
					}
					strcat(dtc_list,dtc);
					strcat(dtc_sys,dtcsystem);
					(*dtc_number)++;
					printfUartObdiideBug("toyota add dtc:%s system:%s \r\n",dtc,dtcsystem);
					printfUartObdiideBug("toyota dtc_list:%s dtc_number:%d \r\n",dtc_list,*dtc_number);
				}
			}
			break;
		
		case DTC_MODE_TOYOTA_MODE_02:
			dtcnumber = (number - pos) / 4;
			for(i = 0; i < dtcnumber; i++)
			{
				dtc_sprintf_Toyota_Mode_2((char *)dtc, recvbuf + i * 4 + pos);
				dtc_sprintf_Toyota_Mode_2((char *)dtcname[ndtcnumber], recvbuf + i * 4 + pos); // ???????
				bFlag = TRUE;
				ndtcnumber++;
				for(total = 0; total < ndtcnumber - 1; total++) // ?????????,????,???????
				{
					if(strcmp((char*)dtc, (char*)dtcname[total]) == 0)
					{
						bFlag = FALSE; // ?????????
						break;
					}
				}
				if(bFlag) // ?????
				{
					if(*dtc_number != 0)
					{//60���ֽڣ���","�ָ�
						strcat(dtc_list,",");
						strcat(dtc_sys,",");
					}
					strcat(dtc_list,dtc);
					strcat(dtc_sys,dtcsystem);
					(*dtc_number)++;
					printfUartObdiideBug("toyota add dtc:%s system:%s \r\n",dtc,dtcsystem);
					printfUartObdiideBug("toyota dtc_list:%s dtc_number:%d \r\n",dtc_list,*dtc_number);
				}
			}
			break;
				
		default:
			break;
	}
	
	return index;
}

void dtc_sprintf_Toyota_Mode_1(char *strbuf,uint8_t *buf)
{
	sprintf(strbuf, "X%04X", buf[0] * 0x100 + buf[1]);
//	switch(buf[0] & 0xC0)
//	{
//	case 0x00: // p
//		sprintf(strbuf, "P%04X", (buf[0] & 0x3F) * 0x100 + buf[1]);
//		break;
//	case 0x40: // c
//		sprintf(strbuf, "C%04X", (buf[0] & 0x3F) * 0x100 + buf[1]);
//		break;
//	case 0x80: // b
//		sprintf(strbuf, "B%04X", (buf[0] & 0x3F) * 0x100 + buf[1]);
//		break;
//	case 0xC0: // u
//		sprintf(strbuf, "U%04X", (buf[0] & 0x3F) * 0x100 + buf[1]);
//	}
}

void dtc_sprintf_Toyota_Mode_2(char *strbuf,uint8_t *buf)
{
	sprintf(strbuf, "X%06X", (buf[0]) * 0x10000 + buf[1] * 0x100 + buf[2]);
}


void pro_clear_dtc_Toyota(INIT_MODE_TOYOTA index) //����
{
	uint8_t sendcmd[8] = {0}; //����
	uint8_t recvbuf[64] = {0}; //
	uint8_t number = 0;

	memcpy(sendcmd, InitIoCmd_Toyota[index].DTC_Clear,InitIoCmd_Toyota[index].DTC_Clear[0]+1);
	number = pro_SendRecv(sendcmd, recvbuf); //����������ж�
}

void pro_exit_system_Toyota(INIT_MODE_TOYOTA index) //�˳�ϵͳ
{
	uint8_t sendcmd[8] = {0}; //�˳�
	uint8_t recvbuf[64] = {0}; //
	uint8_t number = 0;

	if(InitIoCmd_Toyota[index].System_Exit[0]>0)
	{
		memcpy(sendcmd, InitIoCmd_Toyota[index].System_Exit,InitIoCmd_Toyota[index].System_Exit[0]+1);
		number = pro_SendRecv(sendcmd, recvbuf); //�˳�ϵͳ������ж�
	}
}


bool Much_Init_Toyota()
{
	uint8_t i = 0,j = 0;
	uint8_t initstart = 0,initend = 0;
	INIT_MODE_TOYOTA mode = 0;
	bool enter = false;
	//int num = 0;
	//char buf[60] = {0};

	uint8_t iSystem = 5;//һ��5��ϵͳ
	
	g_vehiclediagnosie.fault_total = 0;
	memset(g_vehiclediagnosie.fault_list,0,sizeof(g_vehiclediagnosie.fault_list));//������
	memset(g_vehiclediagnosie.sys_list,0,sizeof(g_vehiclediagnosie.sys_list));//ϵͳ��� 1������ 2���� 3ABS 4���� 5��ȫ

	printfUartObdiideBug("welcome to [%s]\r\n",__func__);
	if(m_first_toyota)
	{
		//��һ�ν���
		m_first_toyota = false;
		
		initstart = 0;//�ӵ�һ��ϵͳ��ʼɨ��ʱ����
		initend = 0;//�ӵ�һ��ϵͳ��ʼɨ��ʱ����
		for (i = TOYOTA_ENGINE;i <= TOYOTA_SRS;i++)
		{
			initstart = initend;
			initend += TOYOTA_iSystem_Mode_Size[i];
			for (j = initstart; j < initend; j++)
			{
				//reset_global_data();//����ȫ�ֱ�������
				if(Init_IO_Toyota(InitIoCmd_Toyota[j].Init_Mode)) //��ʼ��IO
				{
					if(pro_much_init_Toyota(InitIoCmd_Toyota[j].Init_Mode)) //��ʼ��ECU //��ϵͳ
					{
						printfUartObdiideBug("toyota enter success system:%d init:%d \r\n",i,j);
						//m_protocol_index_Toyota[i] = InitIoCmd_Toyota[j].Init_Mode;												//�����ǵڼ���Э��
						m_protocol_index_Toyota[i] = pro_read_dtc_Toyota(i,InitIoCmd_Toyota[j].Init_Mode,&(g_vehiclediagnosie.fault_total),g_vehiclediagnosie.fault_list,g_vehiclediagnosie.sys_list);	//����
						pro_clear_dtc_Toyota(m_protocol_index_Toyota[i]);															//����
						pro_exit_system_Toyota(m_protocol_index_Toyota[i]);														//�˳�
						break;
					}
					else
					{
						printfUartObdiideBug("toyota enter false system:%d init:%d (%d-%d) \r\n",i,j,initstart,initend-1);
					}
				}
			}//for (j = initstart; j < initend; j++)
		}//for (i = 0;i<iSystem;i++)
		for(i = TOYOTA_ENGINE;i <= TOYOTA_SRS;i++)
		{
				if(m_protocol_index_Toyota[i] != -1)
						return true;
		}
	}
	else
	{
		vTaskDelay(3000);
		enter = false;
		for (i = TOYOTA_ENGINE;i <= TOYOTA_SRS;i++)
		{
			if(m_protocol_index_Toyota[i] != -1)//ֱ��ʹ�ü�¼��Э�飬���Ϊ-1����˵�������ڴ�ϵͳ
			{
				if(Init_IO_Toyota(InitIoCmd_Toyota[m_protocol_index_Toyota[i]].Init_Mode)) //��ʼ��IO
				{
					if(pro_much_init_Toyota(InitIoCmd_Toyota[m_protocol_index_Toyota[i]].Init_Mode)) //��ʼ��ECU
					{

//						if(diag_flag == 1 && data_flag == 0)//�������
//						{
//								pro_clear_dtc_Toyota(m_protocol_index_Toyota[i]);															//����
//								vTaskDelay(100);
								enter = true;
								pro_read_dtc_Toyota(i,m_protocol_index_Toyota[i],&(g_vehiclediagnosie.fault_total),g_vehiclediagnosie.fault_list,g_vehiclediagnosie.sys_list);	//����
								pro_exit_system_Toyota(m_protocol_index_Toyota[i]);	
//								diag_flag = 0;
//						}
					}
				}
			}
		}//for (i = 0;i<iSystem;i++)
//		return enter;
	}

		return enter;
}





void pro_protocol_init_Toyota(INIT_MODE_TOYOTA model)
{
	switch(model)
	{
		case OBDII_MODE_01:
		case OBDII_MODE_02:
			cmd_msg_all_canbus_11bit_Toyota();
			break;
		case OBDII_MODE_03:
		case OBDII_MODE_04:
		case OBDII_MODE_05:
			cmd_msg_all_default_model_Toyota();
			break;
		
		default:
			break;
	}
}


void cmd_msg_all_canbus_11bit_Toyota()
{
	uint32_t canread, canflow;
	
	canread = m_modulebuf[0];
	canflow = canread-8;
	
	if((g_defaultMsgAttr.m_PCK_TYPE == 0x22) || (g_defaultMsgAttr.m_PCK_TYPE == 0x20))
	{
		//m_canbussendid=getHex32(ezxml_attr(msgxml, "CANSEND"));
		m_canbusreadid=canread;
		m_canbusflowid=canflow;
	}
	//SetCanbusFilter(canread);//���ﵥ��ϵͳ����ID,�ò���
}

void cmd_msg_all_default_model_Toyota()
{
	cmd_set_ecumodule_Toyota(m_modulebuf[0]);
}

uint8_t cmd_set_ecumodule_Toyota(uint8_t module)
{
	m_setmodule = module;
	return m_setmodule;
}


void SetCanbusFilter_Toyota(uint32_t filterID)
{
	int i;
	int number;
	uint8_t buffer[128];
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
	printfUartObdiideBug("sendbuf:%s\r\n",buffer);
	USB_BULK_Queue_Write(&USB_RX_QueueData,buffer,number);
	vTaskDelay(40);	
}







/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
