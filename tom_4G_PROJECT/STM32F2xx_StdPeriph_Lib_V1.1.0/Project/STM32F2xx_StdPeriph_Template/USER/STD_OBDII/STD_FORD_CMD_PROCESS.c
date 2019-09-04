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
#include "STD_FORD_CMD_PROCESS.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdint.h"


uint8_t FORD_iSystem_Mode_Size[6] = {0,1,1,2,3,4};//每个系统存在的协议的个数，第一次扫描的时候要用到
intptr_t m_protocol_index_FORD[6] = {-1,-1,-1,-1,-1,-1};   //记录侦测到的协议,5个系统的,初始化为-1

bool m_first_FORD = true;

uint8_t m_CAN_ANS_T_FORD;
uint8_t m_CAN_ANS_R_FORD;
uint8_t m_CAN_LISTEN_TYPE_FORD;
uint32_t m_Dtcnum;
extern uint32_t m_canbussendid_FORD;/*系统默认发送ID*/
extern uint32_t m_canbusflowid_FORD;/*当前系统发送ID*/
extern uint32_t m_canbusreadid;/*当前系统接收ID*/
extern uint32_t m_canbusflowidchang_FORD;/*需要改变的发送ID*/
extern uint32_t m_canbusreadidchang_FORD;/*需要改变的接收ID*/

extern DEFAULT_MSGATTR g_defaultMsgAttr;

//bool b_add_6a_FORD = false;

//uint8_t data_flag = 0;
//uint8_t diag_flag = 0;

extern Vehicle_diagnosis g_vehiclediagnosie;
extern Data_collection g_datacollection;

static uint32_t m_modulebuf[256];
extern uint8_t m_modulenum;
extern uint8_t m_setmodule;
extern uint32_t m_canbussendid;
extern uint32_t m_canbusflowid;
extern uint8_t m_KwpWord1;
//初始化IO命令
const CMD_INIT_FORD InitIoCmd_FORD[]=\
{
    // 序号
    // 初始化IO的打包方式ORD 10 11 12 13 等
    // 进系统命令
    // 退系统命令
    // 读码命令
    // 读码模式
    // 码号Pos位
    // 清码命令

    //发动机

    {   //CAN
        {FORD_MODE_00}, //初始化模式
        {MSG_Init_CAN_PackType_FORD},//打包方式
        {0x2c,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x07,0xE0,0x05,0x00,0x00,0x07,0xE8,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x01,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6},
       
       
    },
   
    
    //transmission
    {
        {FORD_MODE_01},
        {MSG_Init_CAN_PackType_FORD},
        {0x2c,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x07,0xE1,0x05,0x00,0x00,0x07,0xE9,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x01,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6},
        
    },
		
   
    //ABS
    {   //CAN
        {FORD_MODE_02},
        {MSG_Init_CAN_PackType_FORD},
        {0x2c,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x07,0x60,0x05,0x00,0x00,0x07,0x68,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x01,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6},
        
    },
    {   //CAN
        {FORD_MODE_03},
        {MSG_Init_KWP_PackType_FORD},
        {0x2B,0x01,0x00,0x28,0xA0,0x02,0x00,0x03,0xC0,0x04,0x77,0x00,0xc1,0xc1,0x05,0x0f,0x19,0x37,0x04,0x04,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x25,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x00,0x00,0x00},
        
       
    },

    //Body  //BCM
    {   //CAN
        {FORD_MODE_04},
        {MSG_Init_CAN_PackType_FORD},
        {0x2c,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x07,0x26,0x05,0x00,0x00,0x07,0x2e,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x01,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6},
       

    },


    {   //CAN
        {FORD_MODE_05},
        {MSG_Init_CAN_PackType_FORD},
        {0x2c,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x02,0x71,0x05,0x00,0x00,0x02,0x79,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x01,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6},
        

    },
    {   //CAN
        {FORD_MODE_06},
        {MSG_Init_CAN_PackType_FORD},
        {0x2c,0x01,0x01,0xe8,0x48,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x07,0x26,0x05,0x00,0x00,0x07,0x2e,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x01,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xb3},
        
       

    },
   

    {
        //AIRBAG
        {FORD_MODE_07},
        {MSG_Init_CAN_PackType_FORD},
        {0x2c,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x07,0x37,0x05,0x00,0x00,0x07,0x3f,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x01,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6},
        
    },
    {
        {FORD_MODE_08},
        {MSG_Init_CAN_PackType_FORD},
        {0x2c,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x07,0x37,0x05,0x00,0x00,0x07,0x3f,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x01,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xb3},
        
    },
   
    {
        {FORD_MODE_09},
        {MSG_Init_KWP_PackType_FORD},
        {0x2B,0x01,0x00,0x28,0xA0,0x02,0x00,0x03,0xC0,0x04,0x77,0x00,0xc1,0xc1,0x05,0x0f,0x19,0x37,0x04,0x04,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x25,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x00,0x00,0x00},
       
    },
	  {
        {FORD_MODE_10},
        {MSG_Init_CAN_PackType_FORD},
        {0x2c,0x01,0x01,0xe8,0x48,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x07,0x37,0x05,0x00,0x00,0x07,0x3f,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x01,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xb3},
        
    }
  

};


//公共函数
bool SetFrameTimeOut_FORD(INIT_MODE_FORD index)
{
    uint8_t  ord  = 0;
    uint8_t initdata[128] = {0};
    uint8_t number = 0,i=0;
    uint32_t canid = 0 ;
    m_CAN_ANS_T_FORD = 0;
    m_CAN_ANS_R_FORD = 0;

    ord = InitIoCmd_FORD[index].Msg_Ord;
    number = InitIoCmd_FORD[index].CMD_DATA[0];
    memcpy(initdata,InitIoCmd_FORD[index].CMD_DATA+1,number);
    switch(ord)
    {
    case 0x13:
        while(i<number)
        {
            switch(initdata[i])
            {
            case 0x01:
                i+=4;
                break;
            case 0x02:
                i+=2;
                break;
            case 0x03:
                i+=2;
                break;
            case 0x04:
                i+=5;
                break;
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
            case 0x07:
                i+=initdata[i+1]*2+2;
                break;
            case 0x08:
                i+=2;
                break;
            case 0x09:
                g_defaultMsgAttr.m_ISOAnswerNum = initdata[i+1];
                i+=2;
                break;
            case 0x0a:
                i+=4;
                break;
            case 0x0b:
                i+=2;
                break;
            case 0x0c:
                i+=4;
                break;
            case 0x0d:
                i+=2;
                break;
            case 0x0e:
                i+=3;
                break;
            case 0x0f:
                i+=2;
                break;
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
            case 0x01:
                i+=4;
                break;
            case 0x02:
                i+=2;
                break;
            case 0x03:
                i+=2;
                break;
            case 0x04:
                i+=5;
                break;
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
            case 0x07:
                i+=initdata[i+1]*2+2;
                break;
            case 0x08:
                i+=2;
                break;
            case 0x09:
                g_defaultMsgAttr.m_ISOAnswerNum = initdata[i+1];
                i+=2;
                break;
            case 0x0a:
                i+=4;
                break;
            case 0x0b:
                i+=2;
                break;
            case 0x0c:
                i+=5;
                break;
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
            case 0x01:
                i+=4;
                break;
            case 0x02:
                m_CAN_LISTEN_TYPE_FORD =initdata[i+1];
                i+=2;
                break;
            case 0x03:
                i+=2;
                break;
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
                i+=5;
                break;
            case 0x06:
                i+=5;
                break;
            case 0x07:
                i+=5;
                break;
            case 0x08:
                i+=5;
                break;
            case 0x09:
                i+=3;
                break;
            case 0x0a:
                i+=3;
                break;
            case 0x0b:
                i+=3;
                break;
            case 0x0c:
                i+=2;
                break;
            case 0x0d:
//				m_CAN_ANS_T_FORD = initdata[i+1];
                i+=2;
                break;
            case 0x0e:
//				m_CAN_ANS_R_FORD = initdata[i+1];
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


//设置ECU初始化ORD
void pro_set_init_ecu_ord_FORD(INIT_MODE_FORD mode)
{
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
}


//设置通信过程ORD
void pro_msg_all_FORD(INIT_MODE_FORD mode)
{
    //cmd_set_two_id_FORD(0x00);
    switch(mode)
    {
        //CAN
    case FORD_MODE_00: //CAN
    
    case FORD_MODE_01: //CAN
    case FORD_MODE_02://CAN
    case FORD_MODE_04://CAN
    case FORD_MODE_05: //CAN
    case FORD_MODE_06: //CAN
    case FORD_MODE_07: //CAN
    case FORD_MODE_08: //CAN
		case FORD_MODE_10:
        g_defaultMsgAttr.m_CS_MODE = 0;
        g_defaultMsgAttr.m_ORD     = 0x30;
        g_defaultMsgAttr.m_PCK_TYPE= 0x22;
        g_defaultMsgAttr.m_RWAYS   = 0;
        g_defaultMsgAttr.m_SRC     = 0;
        g_defaultMsgAttr.m_TGT     = 0;
        g_defaultMsgAttr.m_WAYS    = 0;
        break;

        //CAN
        //这个比较特殊，目前因为只有这一个系统是带第二id的，暂固写在这里
        //FORD 里面的 main body 的第二id是40
        //a5 a5 00 0d 30 00 00 07 50 40 01 3e 00 00 00 00 00 ??
			// pwm
   

        //KWP

    case FORD_MODE_03:
        g_defaultMsgAttr.m_CS_MODE = 0;
        g_defaultMsgAttr.m_ORD     = 0x33;
        g_defaultMsgAttr.m_PCK_TYPE= 0x53;
        g_defaultMsgAttr.m_RWAYS   = 0;
        g_defaultMsgAttr.m_SRC     = 0xF5;
        g_defaultMsgAttr.m_TGT     = 0x28;
        g_defaultMsgAttr.m_WAYS    = 0;
        break;

    case FORD_MODE_09:
        g_defaultMsgAttr.m_CS_MODE = 0;
        g_defaultMsgAttr.m_ORD     = 0x33;
        g_defaultMsgAttr.m_PCK_TYPE= 0x53;
        g_defaultMsgAttr.m_RWAYS   = 0;
        g_defaultMsgAttr.m_SRC     = 0xF5;
        g_defaultMsgAttr.m_TGT     = 0x58;
        g_defaultMsgAttr.m_WAYS    = 0;
        break;


    default:
        break;
    }
}

//直接用OBD的
//void ResetComPort()



//初始化IO
bool Init_IO_FORD(INIT_MODE_FORD index)
{
    uint16_t number = 0;
    INIT_MODE_FORD mode = 0;
    uint8_t sendbuf[128] = {0};
    uint8_t recvbuf[32] = {0};
    if(!SetFrameTimeOut_FORD(index))
    {
        return false;
    }
    memset(recvbuf,0,sizeof(recvbuf));
    number = InitIoCmd_FORD[index].CMD_DATA[0];
    memcpy(sendbuf, InitIoCmd_FORD[index].CMD_DATA+1, number);
    mode = InitIoCmd_FORD[index].Init_Mode;
    g_defaultMsgAttr.m_PCK_TYPE = 0x00;
    number = pro_packsenddata(sendbuf, number, InitIoCmd_FORD[index].Msg_Ord);
    if(number == 0)
    {
        return 0;
    }
    ResetComPort();
    vTaskDelay(20);

    USB_BULK_Queue_Write(&USB_RX_QueueData,sendbuf,number);
    printfUartObdiideBug("index is : %d\r\n",(int)index);
    OBDII_ReadBuf(recvbuf, &number,1500);
    //number = pro_SendRecv((uint8_t *)ObdiiInitIoCmd[index].CMD_DATA,(uint8_t *)sendbuf);
    if (number < 1)
    {
        printfUartObdiideBug("InitIO false\r\n");
        return false;
    }
    vTaskDelay(1000);
    m_canbussendid = InitIoCmd_FORD[index].CMD_DATA[12] * 0x100 + InitIoCmd_FORD[index].CMD_DATA[13];
    //m_canbussendid = InitIoCmd_FORD[index].CMD_DATA[]
    //pro_set_can_id_FORD(index);
    printfUartObdiideBug("InitIO true\r\n");
    return true;
}


void pro_set_can_id_FORD(INIT_MODE_FORD mode)
{
    switch(mode)
    {
    case FORD_MODE_01:
        m_canbussendid = 0x000007E0;
        m_canbusreadid = 0x000007E8;
        m_canbusflowid = 0x000007E0;
        break;

    case FORD_MODE_02:
        m_canbussendid = 0x00000700;
        m_canbusreadid = 0x000007E8;
        m_canbusflowid = 0x00000700;
        break;

    default:
        break;
    }
}

bool pro_much_init_FORD(INIT_MODE_FORD mode)// CAN协议进系统
{
    uint8_t sendbuf[128] = {0};
    bool bRet = false;
    MSG_ORD_FORD PackType =  InitIoCmd_FORD[mode].Msg_Ord;

    memset(sendbuf,0,sizeof(sendbuf));

    switch(PackType)
    {
        //CAN
    case MSG_Init_CAN_PackType_FORD:
        pro_msg_all_FORD(mode);
        bRet = pro_canbus_select_module_FORD(mode);
        break;

        //KWP
    case MSG_Init_KWP_PackType_FORD:
        pro_msg_all_FORD(mode);
        bRet = pro_kwp_select_module_FORD(mode);//kwp的81进系统命令
        break;

    default:
        break;
    }

    return bRet;
}


//p0001,p0002,
bool pro_kwp_select_module_FORD(INIT_MODE_FORD mode)//kwp的81进系统命令
{
    uint16_t number = 0;
    uint8_t recvbuf[256] = {0};
    uint8_t ord = 0;

   

    ord = g_defaultMsgAttr.m_ORD;//临时保存
    g_defaultMsgAttr.m_ORD = 0x23;//进系统 临时改成0x23
   
    number = pro_SendRecv("\x03\x22\x02\x00", recvbuf);

    g_defaultMsgAttr.m_ORD = ord;//还原回去

    if(number == 0)
    {
        return false;
    }
    m_Dtcnum = recvbuf[3];
    return true;
}

bool pro_canbus_select_module_FORD(INIT_MODE_FORD mode)
{
    // int i, j;
   // uint8_t sendbuf[128] = {0};
    uint8_t recvbuf[256] = {0};
   // uint8_t sendnum =0;
    uint16_t number=0;
//    uint32_t canid;

    //sendnum = InitIoCmd_FORD[mode].System_Enter[0];
  //  memset(sendbuf,0,sizeof(sendbuf));
    //memcpy(sendbuf, InitIoCmd_FORD[mode].System_Enter,sendnum+1);

			number = pro_SendRecv("\x03\x22\x02\x00", recvbuf); //进系统 带长度
		if(number == 0)
			number = pro_SendRecv("\x03\x22\xd1\x02", recvbuf); //进系统 带长度
		else
			return true;
    if(number == 0)
		  number = pro_SendRecv("\x03\x22\x02\x02", recvbuf); //进系统 带长度
		else
			return true;
		if(number == 0)
			return false;
		else
    return true;
}


//读码处理函数

INIT_MODE_FORD pro_read_dtc_FORD(SYSTEM_FORD sys,INIT_MODE_FORD index,int* dtc_number,char* dtc_list,char* dtc_sys)//读码

{
    char recvbuf[256] = {0};
		char strbuf[20] = {0};
		int number = 0;
		int dtcnumber = 0;
		int dtccount = 0;
    int i = 0;
		int x = 0;
    int SingalSysDtcCout = 0;
    char dtcsystem[4] = {0};
    sprintf(dtcsystem, "%d", sys);
		printfUartObdiideBug("*dtc_number is %d dtcsystem is :%s \r\n ", *dtc_number,dtcsystem);
    switch(index)
    {
    case FORD_MODE_00:
		case FORD_MODE_01:
		case FORD_MODE_02:
	  case FORD_MODE_04:
		case FORD_MODE_05:
		case FORD_MODE_06:
		case FORD_MODE_07:
		case FORD_MODE_08:
		case FORD_MODE_10:
			 number = pro_SendRecv("\x03\x19\x02\x8f",recvbuf);
		   if(number > 0)
			 {
				dtcnumber=(number - 3)/4;
					for(i=0; i<dtcnumber; i++)
					{
						//sprintf(strbuf, "%02X%02X%02X%02X", recvbuf[i*4+3], recvbuf[i*4+4], 0, 0);
						if ((recvbuf[i*4+3]!=0) || (recvbuf[i*4+4]!=0))
						{
							sprintf(strbuf, "%02X%02X%02X%02X", recvbuf[i*4+3], recvbuf[i*4+4], recvbuf[i*4+5], recvbuf[i*4+6]);
							if((strcmp(strbuf, "00001000") == 0)||(strcmp(strbuf, "10000000") == 0))
							{
								dtcnumber--;
							}
							else
							{
							if(*dtc_number != 0)
								{
									strcat(dtc_list, ",");
									strcat(dtc_sys,",");
									strcat(dtc_list,strbuf);
									strcat(dtc_sys,dtcsystem);
							
								}
								else
								{
									strcat(dtc_list,strbuf);
									strcat(dtc_sys,dtcsystem);	
								}
								(*dtc_number)++;
							
								
							}
						}
					}
				}	
				else
				{ // 18 00 ff 00 读码
				  number = pro_SendRecv("\x04\x18\x00\xff\0x00",recvbuf);
					if(number > 0)
					{
					dtcnumber = recvbuf[1];
					for(i=0; i<dtcnumber; i++)
					{
						if (((recvbuf[i*3+2]!=0) || (recvbuf[i*3+3]!=0) || (recvbuf[i*3+4]!=0)) && ((i*3+3) < number))
						{
							sprintf(strbuf, "%02X%02X%02X%02X", 0, 0, recvbuf[i*3+2], recvbuf[i*3+3]);
							if((strcmp(strbuf, "00001000") == 0)||(strcmp(strbuf, "10000000") == 0))
							{
								dtcnumber--;
							}
							else
							{
								if(*dtc_number != 0)
								{
									strcat(dtc_list, ",");
									strcat(dtc_sys,",");
									strcat(dtc_list,strbuf);
									strcat(dtc_sys,dtcsystem);
							
								}
								else
								{
									strcat(dtc_list,strbuf);
									strcat(dtc_sys,dtcsystem);	
								}
								(*dtc_number)++;
								
									
							}
						}
					}
				}
			}
        break;
 
		case FORD_MODE_03:
		case FORD_MODE_09:
					
					i = 0;
					dtcnumber = m_Dtcnum;
					number  = pro_SendRecv("\x01\x13",recvbuf);
		     

					if(number > 3)
					{
					while (dtccount < dtcnumber)
					{
					
						if (recvbuf[i*2+1] || recvbuf[i*2+2])
						{
							sprintf(strbuf, "%02X%02X%02X%02X", 0, 0, recvbuf[i*2+1], recvbuf[i*2+2]);
							if((strcmp(strbuf, "00001000") == 0)||(strcmp(strbuf, "10000000") == 0))
							{
								dtcnumber--;
							}
							else
							{	
							
								if(*dtc_number != 0)
								{
									strcat(dtc_list, ",");
									strcat(dtc_sys,",");
									strcat(dtc_list,strbuf);
									strcat(dtc_sys,dtcsystem);
							
								}
								else
								{
									strcat(dtc_list,strbuf);
									strcat(dtc_sys,dtcsystem);	
								}
								(*dtc_number)++;
								 dtccount ++;	
								
								
							}
						}
						if ( i * 2 + 2 > number)
						{
							break;
						}
						i++;
					}
				}
			
        break;
			default:
				break;
		}
    printfUartObdiideBug("FORD enter dtcmode:%d,dtcnumber is :%d,dtcs is %s dtc_sys is %s \r\n",index,*dtc_number,dtc_list,dtc_sys);
    return index;
}





//void pro_exit_system_FORD(INIT_MODE_FORD index) //退出系统
//{
//    uint8_t sendcmd[128] = {0}; //退出
//    uint8_t recvbuf[128] = {0}; //
//    uint8_t number = 0;

//    if(InitIoCmd_FORD[index].System_Exit[0]>0)
//    {

//        memcpy(sendcmd, InitIoCmd_FORD[index].System_Exit,InitIoCmd_FORD[index].System_Exit[0]+1);

//        number = pro_SendRecv(sendcmd, recvbuf); //退出系统命令不做判断

//    }
//}


void pro_clear_dtc_FORD() //清码
{
      uint8_t number = 0;
      uint8_t recvbuf[128] = {0}; //
			number = pro_SendRecv("\x04\x14\xFF\xFF\xFF",recvbuf);
			if(number > 0 && recvbuf[0] != 0x7f)
					return;
			number = pro_SendRecv("\x03\x14\xFF\x00",recvbuf);
      if (number > 0 && recvbuf[0] != 0x7f)
					return;
			number = pro_SendRecv("\x01\x14",recvbuf);
					return;			
}


bool Much_Init_FORD()
{
    uint8_t i = 0,j = 0;
    uint8_t initstart = 0,initend = 0;
//    INIT_MODE_FORD mode = 0;
    //int num = 0;
    //char buf[60] = {0};

    //uint8_t iSystem = 5;//一共5个系统
		bool enter = false;
		
    g_vehiclediagnosie.fault_total = 0;
    memset(g_vehiclediagnosie.fault_list,0,sizeof(g_vehiclediagnosie.fault_list));//故障码
    memset(g_vehiclediagnosie.sys_list,0,sizeof(g_vehiclediagnosie.sys_list));//系统编号 1发动机 2传动 3ABS 4车身 5安全

    printfUartObdiideBug("welcome to [%s]\r\n",__func__);
    if(m_first_FORD)
    {
        //第一次进入
//        m_first_FORD = false;
        initstart = 0;//从第一个系统开始扫描时重置
        initend = 0;//从第一个系统开始扫描时重置
        for (i = FORD_ENGINE  ; i <= FORD_SRS; i++)
        {
            initstart = initend;
            initend += FORD_iSystem_Mode_Size[i];
            for (j = initstart; j < initend; j++)
            {
                //reset_global_data();//重置全局变量数据
                if(Init_IO_FORD(InitIoCmd_FORD[j].Init_Mode)) //初始化IO
                {
                    if(pro_much_init_FORD(InitIoCmd_FORD[j].Init_Mode)) //初始化ECU //进系统
                    {
                        printfUartObdiideBug("FORD enter success system:%d init:%d \r\n",i,j);
                        m_protocol_index_FORD[i] = InitIoCmd_FORD[j].Init_Mode;												//保存是第几个协议
                        m_protocol_index_FORD[i] = pro_read_dtc_FORD(i,InitIoCmd_FORD[j].Init_Mode,&(g_vehiclediagnosie.fault_total),g_vehiclediagnosie.fault_list,g_vehiclediagnosie.sys_list);	//读码
                        pro_clear_dtc_FORD();															//清码
                       // pro_exit_system_FORD(m_protocol_index_FORD[i]);														//退出
                        printfUartObdiideBug("tom_min leaving \r\n");
                        break;
                    }
                    else
                    {
                        printfUartObdiideBug("FORD enter false system:%d init:%d (%d-%d) \r\n",i,j,initstart,initend-1);
                    }
                }
            }//for (j = initstart; j < initend; j++)
        }//for (i = 0;i<iSystem;i++)
				
				m_first_FORD = false;
				
				for(i = FORD_ENGINE;i <= FORD_SRS;i++)
				{
						if(m_protocol_index_FORD[i] != -1)
								return true;
				}
    }
    else
    {
				vTaskDelay(3000);
				enter = false;
        for (i = FORD_ENGINE; i <= FORD_SRS; i++)
        {
            if(m_protocol_index_FORD[i] != -1)//直接使用记录的协议，如果为-1，则说明不存在此系统
            {
                if(Init_IO_FORD(InitIoCmd_FORD[m_protocol_index_FORD[i]].Init_Mode)) //初始化IO
                {
                    if(pro_much_init_FORD(InitIoCmd_FORD[m_protocol_index_FORD[i]].Init_Mode)) //初始化ECU
                    {

//														pro_clear_dtc_FORD(m_protocol_index_FORD[i]);
//														vTaskDelay(100);
												enter = true;
                        pro_read_dtc_FORD(i,m_protocol_index_FORD[i],&(g_vehiclediagnosie.fault_total),g_vehiclediagnosie.fault_list,g_vehiclediagnosie.sys_list);	//读码
                       // pro_exit_system_FORD(m_protocol_index_FORD[i]);
                        vTaskDelay(20);
                    }
                }
            }
        }
        printfUartObdiideBug("FORD leaving \r\n");
    }

		return enter;
}








void cmd_msg_all_canbus_11bit_FORD()
{
    uint32_t canread = 0, canflow = 0;

    canread = m_modulebuf[0];
    canflow = canread-8;

    if((g_defaultMsgAttr.m_PCK_TYPE == 0x22) || (g_defaultMsgAttr.m_PCK_TYPE == 0x20))
    {
        //m_canbussendid=getHex32(ezxml_attr(msgxml, "CANSEND"));
        m_canbusreadid=canread;
        m_canbusflowid=canflow;
    }
    //SetCanbusFilter(canread);//丰田单个系统单个ID,用不到
}

void cmd_msg_all_default_model_FORD()
{
    cmd_set_ecumodule_FORD(m_modulebuf[0]);
}

uint8_t cmd_set_ecumodule_FORD(uint8_t module)
{
    m_setmodule = module;
    return m_setmodule;
}


void SetCanbusFilter_FORD(uint32_t filterID)
{
    int i = 0;
    int number = 0;
    uint8_t buffer[128] = {0};
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
    for(i=2; i<number-1; i++)
    {
        buffer[number-1] += buffer[i];
    }
    buffer[number-1] = ~buffer[number-1];
    printfUartObdiideBug("sendbuf:%s\r\n",buffer);
    USB_BULK_Queue_Write(&USB_RX_QueueData,buffer,number);
    vTaskDelay(40);
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
