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
#include "STD_GREATWALL_CMD_PROCESS.h"
#include "FreeRTOS.h"
#include "task.h"
#include "STD_OBD_CMD_PROCESS.h"
#include "App.h"


uint8_t GREATWALL_iSystem_Mode_Size[6] = {0,12,7,9,3,13};//每个系统存在的协议的个数，第一次扫描的时候要用到(因为下面的系统是从1开始编号的，所以这里的第额外加了第0个)
intptr_t m_protocol_index_GREATWALL[6] = {-1,-1,-1,-1,-1,-1};   //记录侦测到的协议,5个系统的,初始化为-1(因为下面的系统是从1开始编号的，所以这里的第额外加了第0个)
bool m_first_GREATWALL = true;

uint8_t m_CAN_ANS_T_GREATWALL;
uint8_t m_CAN_ANS_R_GREATWALL;
uint8_t m_CAN_LISTEN_TYPE_GREATWALL;
extern uint32_t m_canbussendid_GREATWALL;/*系统默认发送ID*/
extern uint32_t m_canbusflowid_GREATWALL;/*当前系统发送ID*/
extern uint32_t m_canbusreadid;/*当前系统接收ID*/
extern uint32_t m_canbusflowidchang_GREATWALL;/*需要改变的发送ID*/
extern uint32_t m_canbusreadidchang_GREATWALL;/*需要改变的接收ID*/
extern Vehicle_Model_Info tom_vehicle_model_data;
extern DEFAULT_MSGATTR g_defaultMsgAttr;

//bool b_add_6a_GREATWALL = false;

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

//初始化IO命令
const CMD_INIT_GREATWALL InitIoCmd_GREATWALL[]=\
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
        /*pos = 03 len = 4  故障码三个字节 前2位PCBU + 小码   TYPE25 TYPE15 */
        {GREATWALL_MODE_00}, //初始化模式
        {MSG_Init_CAN_PackType_GREATWALL},//打包方式
        {0x2c,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x07,0xE0,0x05,0x00,0x00,0x07,0xE8,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x01,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6},
        {0x02,0x10,0x03},
        {0x03,0x19,0x01,0xaf},
        {0x03,0x19,0x02,0x0f},
        {12,624,493,2001,492,2304,2122,2090,2120,3454,2615,2123,4485}, 
				/*长城M2 联电发动机(ME788) = 长城M4 联电发动机(ME788), C20R=C30,C50 H1 ,H2 H6,M6*/
        {DTC_MODE_GREATWALL_MODE_01}
    },
    {   //CAN
        /*pos = 03 len = 4  故障码2个字节 前2位PCBU + 第四位小码 TYPE 11 */
        {GREATWALL_MODE_01}, //初始化模式
        {MSG_Init_CAN_PackType_GREATWALL},//打包方式
        {0x2c,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x07,0xE0,0x05,0x00,0x00,0x07,0xE8,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x01,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6},
        {0x02,0x10,0x01},
        {0x03,0x19,0x01,0x01},
        {0x03,0x19,0x02,0x01},
        {5,2462,3101,2027,2123,3481}, 
				/*风骏5 风骏6 H5 H6，H6COUPE*/ 
        {DTC_MODE_GREATWALL_MODE_02}
    },


    {   //CAN
        /*pos = 03 len = 4  故障码三个字节 type 24  */
        {GREATWALL_MODE_02}, //初始化模式
        {MSG_Init_CAN_PackType_GREATWALL},//打包方式
        {0x2c,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x07,0xE0,0x05,0x00,0x00,0x07,0xE8,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x01,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6},
        {0x02,0x10,0x03},
        {0x00,0x00,0x00,0x00},
        {0x03,0x19,0x02,0x01},
        {7,2462,3101,4168,3481,3074,4252,4441} ,
				/*风骏5 F6 H2S H6COUPE,H7,VV7 ,P8*/
        {DTC_MODE_GREATWALL_MODE_01}
    },

    {   //CAN
        /*pos = 03 len = 4  故障码2个字节 16进制显示  TYPE 16 */
        {GREATWALL_MODE_03}, //初始化模式
        {MSG_Init_CAN_PackType_GREATWALL},//打包方式
        {0x2c,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x07,0xE0,0x05,0x00,0x00,0x07,0xE8,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x01,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6},
        {0x02,0x10,0x01},
        {0x00,0x00,0x00,0x00},
        {0x03,0x19,0x02,0x01},
        {4,4608,2027,2123,4253} , 
				/*H4 H5,H6, VV5，*/ //
        {DTC_MODE_GREATWALL_MODE_03}
    },

    {   //CAN
        /*pos = 03 len = 4  前三位16进制表示故障码  TYPE 12 */
        {GREATWALL_MODE_04}, //初始化模式
        {MSG_Init_CAN_PackType_GREATWALL},//打包方式
        {0x2c,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x07,0xE0,0x05,0x00,0x00,0x07,0xE8,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x01,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6},
        {0x02,0x10,0x01},
        {0x03,0x19,0x01,0x0c},
        {0x03,0x19,0x02,0x0c},
        {6,2462,3101,2027,2123,3481,4485}, 
				/*风骏5 风骏6 H5 H6，H6COUPE M6*/ //
        {DTC_MODE_GREATWALL_MODE_03}
    },

    {   //CAN
        /*pos = 03 len = 4  前三位16进制表示故障码  TYPE 2 */
        {GREATWALL_MODE_05}, //初始化模式
        {MSG_Init_CAN_PackType_GREATWALL},//打包方式
        {0x2c,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x07,0xE0,0x05,0x00,0x00,0x07,0xE8,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x01,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6},
        {0x02,0x10,0x03},
        {0x00,0x00,0x00,0x00},
        {0x04,0x17,0xff,0xff,0xff},
        {1,2462},  /*风骏5 */ //
        {DTC_MODE_GREATWALL_MODE_03}
    },
    {   //CAN
        /*pos = 03 len = 4    TYPE 4  两个字节一个故障码 */
        {GREATWALL_MODE_06}, //初始化模式
        {MSG_Init_CAN_PackType_GREATWALL},//打包方式
        {0x2c,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x07,0xE0,0x05,0x00,0x00,0x07,0xE8,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x01,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6},
        {0x02,0x10,0x03},
        {0x00,0x00,0x00,0x00},
        {0x04,0x19,0x02,0xff,0x00},
        {2,4485,3481} , /*H6 H6COUPE */ //
        {DTC_MODE_GREATWALL_MODE_04}
    },


    /*  发动机KWP 协议  */

    {   //KWP 10400 13       TYPE57 TYPE3  02位开始3位一个故障码
        {GREATWALL_MODE_07},
        {MSG_Init_KWP_PackType_GREATWALL},
        {0x2B,0x01,0x00,0x28,0xA0,0x02,0x00,0x03,0xC0,0x04,0x77,0xff,0xc1,0xc1,0x05,0x12,0x20,0x38,0x04,0x04,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x25,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
        {0x01,0x81},
        {0x00,0x00,0x00},
        {0x04,0x18,0x00,0x00,0x00},
        {9,624,536,2200,493,2001,492,2304,2122,2090}, 
				/* 嘉誉,V80  M2,M4 C20R C30*/
        {DTC_MODE_GREATWALL_MODE_05}

    },

    {   //KWP 10400 13       TYPE3  02位开始3位一个故障码 P 开头
        {GREATWALL_MODE_08},
        {MSG_Init_KWP_PackType_GREATWALL},
        {0x2B,0x01,0x00,0x28,0xA0,0x02,0x00,0x03,0xC0,0x04,0x77,0xff,0xc1,0xc1,0x05,0x12,0x20,0x38,0x04,0x04,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x25,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
        {0x01,0x81},
        {0x00,0x00,0x00},
        {0x04,0x18,0x00,0x00,0x00},
        {1,2462},  /*风骏 5*/
        {DTC_MODE_GREATWALL_MODE_05}

    },

    {   //KWP 10400 13       TYPE57 TYPE3 TYPE36 02位开始2位一个故障码 故障码长度为4个字节
        {GREATWALL_MODE_09},  //TGT 11/F1
        {MSG_Init_KWP_PackType_GREATWALL},
        {0x2B,0x01,0x00,0x28,0xA0,0x02,0x00,0x03,0xC0,0x04,0x77,0xff,0xc1,0xc1,0x05,0x12,0x20,0x38,0x04,0x04,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x25,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
        {0x01,0x81},
        {0x00,0x00,0x00},
        {0x04,0x18,0x00,0xFF,0x00},
        {12,624,536,2200,492,2304,2122,2090,2462,3101,2027,2123,4485}, 
				/*嘉誉 ,v80 ,M4,C20R,C30,风骏5 ,风骏6 H5,H6 M6 */
        {DTC_MODE_GREATWALL_MODE_05}

    },
    {   //KWP 10400 13       TYPE57 TYPE3 TYPE36 02位开始3位一个故障码
        {GREATWALL_MODE_10},  //TGT 10/F1
        {MSG_Init_KWP_PackType_GREATWALL},
        {0x2B,0x01,0x00,0x28,0xA0,0x02,0x00,0x03,0xC0,0x04,0x77,0xff,0xc1,0xc1,0x05,0x12,0x20,0x38,0x04,0x04,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x25,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
        {0x01,0x81},
        {0x00,0x00,0x00},
        {0x04,0x18,0x00,0xFF,0x00},
        {1,2462},  /*风骏5 */
        {DTC_MODE_GREATWALL_MODE_05}

    },
    {   //KWP 10400 13        TYPE36 02位开始2位一个故障码,故障码长度4个字节
        {GREATWALL_MODE_11},  //TGT 11/F1
        {MSG_Init_KWP_PackType_GREATWALL},
        {0x2B,0x01,0x00,0x28,0xA0,0x02,0x00,0x03,0xC0,0x04,0x77,0x88,0xc1,0xc1,0x05,0x12,0x20,0x38,0x04,0x04,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x25,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
        {0x01,0x81},
        {0x00,0x00,0x00},
        {0x04,0x19,0x02,0xFF,0x00}, //修改故障码命令
        {6,536,2200,2462,2027,2123,4485}, 
				/*嘉誉，V80,风骏5 H5,H6,M6*/
        {DTC_MODE_GREATWALL_MODE_05}
    },

    /*变速箱系统*/
    //transmission
    {
        {GREATWALL_MODE_12},  //TYPE 8 TYPE3
        {MSG_Init_CAN_PackType_GREATWALL},
        {0x2c,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x07,0xE1,0x05,0x00,0x00,0x07,0xE9,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x01,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6},
        {0x02,0x10,0x81},
        {0x00,0x00,0x00},
        {0x04,0x18,0x00,0x00,0x00},
        {8,624,493,2001,492,2304,2122,2090,2027}, 
				//M2 M4 C20R C30 H5
        {DTC_MODE_GREATWALL_MODE_05}
    },
    {
        {GREATWALL_MODE_13},  //TYPE 8
        {MSG_Init_CAN_PackType_GREATWALL},
        {0x2c,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x07,0xE1,0x05,0x00,0x00,0x07,0xE9,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x01,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6},
        {0x02,0x10,0x81},
        {0x00,0x00,0x00},
        {0x04,0x18,0x00,0xFF,0x00},
        {1,2120},  //C50
        {DTC_MODE_GREATWALL_MODE_05}
    },

    {
        {GREATWALL_MODE_14},  //TYPE 11  2个字节故障码
        {MSG_Init_CAN_PackType_GREATWALL},
        {0x2c,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x07,0xE1,0x05,0x00,0x00,0x07,0xE9,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x01,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6},
        {0x02,0x10,0x01},
        {0x19,0x01,0x01},
        {0x03,0x19,0x02,0x01},
        {5,493,2001,492,2304,2090}, 
				//M2,M4,C30
        {DTC_MODE_GREATWALL_MODE_02}
    },

    {
        {GREATWALL_MODE_15}, // TYPE 15 TYPE24 TYPE 25 3个字节故障码
        {MSG_Init_CAN_PackType_GREATWALL},
        {0x2c,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x07,0xE1,0x05,0x00,0x00,0x07,0xE9,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x01,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6},
        {0x02,0x10,0x03},
        {0X03,0x19,0x01,0x01},
        {0x03,0x19,0x02,0x01},
        {15,624,2122,2120,2615,4168,395,4608,2027,2123,3481,3074,4485,4253,4252,4441},  //C20R,C50,H2,H2S,H3,H4,H5,H6,H6COUPE,H7,M6 VV5 VV7 P8
        {DTC_MODE_GREATWALL_MODE_01}
    },

    {
        {GREATWALL_MODE_16},  //TYPE 12 3个字节故障码
        {MSG_Init_CAN_PackType_GREATWALL},
        {0x2c,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x06,0x92,0x05,0x00,0x00,0x06,0x93,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x01,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6},
        {0x02,0x10,0x03},
        {0X03,0x19,0x01,0x01},
        {0x03,0x19,0x02,0x01},
        {2,2124,3298},  
				//H8 H9
        {DTC_MODE_GREATWALL_MODE_03}
    },


//变速箱 KWP 系统
    {
        {GREATWALL_MODE_17}, //TYPE 3
        {MSG_Init_KWP_PackType_GREATWALL},
        {0x2B,0x01,0x00,0x28,0xA0,0x02,0x00,0x03,0xC0,0x04,0x77,0xff,0xc1,0xc1,0x05,0x0f,0x21,0x37,0x06,0x04,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x25,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
        {0x01,0x81},
        {0x00},
        {0x4,0x18,0x00,0xff,0x00},
        {4,492,2304,2090,3454},
				//M4,c30,H1
        {DTC_MODE_GREATWALL_MODE_05}
    },

    {
        {GREATWALL_MODE_18}, //Type 3
        {MSG_Init_KWP_PackType_GREATWALL},
        {0x2B,0x01,0x00,0x28,0xA0,0x02,0x00,0x03,0xC0,0x04,0x77,0xff,0xc1,0xc1,0x05,0x0f,0x21,0x37,0x06,0x04,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x25,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
        {0x01,0x81},
        {0x00},
        {0x4,0x18,0x00,0x00,0x00},
        {3,536,2200,2123}, //嘉誉,V80 H6
        {DTC_MODE_GREATWALL_MODE_05}
    },


    //ABS
    {   //CAN
        {GREATWALL_MODE_19}, //TYPE 21 TYPE 25 15 24
        {MSG_Init_CAN_PackType_GREATWALL},
        {0x2c,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x07,0x82,0x05,0x00,0x00,0x07,0xC2,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x01,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6},
        {0x02,0x10,0x03},
        {0X03,0x19,0x01,0x01},
        {0x03,0x19,0x02,0x01},
        {22,624,492,493,2001,2304,2122,2090,2120,2462,3101,3454,2615,4168,4608,2027,2123,3074,2124,3298,4485,4253,4252}, 
					//M2 M4,c20R,C30,C50,F5,F6,h1,h2,H2S,H4,H5,H6,H7,H8,H9,M6 VV5 VV7
        {DTC_MODE_GREATWALL_MODE_01}
    },
    {   //CAN
        {GREATWALL_MODE_20}, //TYPE 19
        {MSG_Init_CAN_PackType_GREATWALL},
        {0x2c,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x07,0x82,0x05,0x00,0x00,0x07,0xC2,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x01,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6},
        {0x02,0x10,0x03},
        {0X03,0x19,0x01,0x01},
        {0x03,0x19,0x02,0x01},
        {2,3481,4441},  //H6COPE,P8
        {DTC_MODE_GREATWALL_MODE_03}
    },
    {   //CAN
        {GREATWALL_MODE_21}, //TYPE 25
        {MSG_Init_CAN_PackType_GREATWALL},
        {0x2c,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x06,0x94,0x05,0x00,0x00,0x06,0x95,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x01,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6},
        {0x02,0x10,0x01},
        {0X03,0x19,0x01,0x01},
        {0x03,0x19,0x02,0x01},
        {2,2124,3298},  //H8,H9
        {DTC_MODE_GREATWALL_MODE_01}
    },


    {   //KWP 10400 29
        {GREATWALL_MODE_22},//TGT=28 SRC=F1
        {MSG_Init_KWP_PackType_GREATWALL},
        {0x2B,0x01,0x00,0x28,0xA0,0x02,0x00,0x03,0xC0,0x04,0x77,0xff,0xc1,0xc1,0x05,0x0f,0x21,0x37,0x06,0x04,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x25,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
        {0x01,0x81},
        {0X00},
        {0x04,0x18,0X00,0XFF,0x00},
        {9,624,492,493,2001,2304,2122,2090,395,2027}, 
				//M2,M4 C20R ,C30 F5 H3 H5
        {DTC_MODE_GREATWALL_MODE_05}
    },

    {   //KWP 10400 29
        {GREATWALL_MODE_23},//TGT=22 SRC=F1
        {MSG_Init_KWP_PackType_GREATWALL},
        {0x2B,0x01,0x00,0x28,0xA0,0x02,0x00,0x03,0xC0,0x04,0x77,0x88,0xc1,0xc1,0x05,0x0f,0x21,0x37,0x06,0x04,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x25,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
        {0x01,0x81},
        {0X00},
        {0x04,0x18,0X00,0XFF,0x00},
        {5,536,2200,2462,395,2027},  //嘉誉,V80 F5 H3 H5
        {DTC_MODE_GREATWALL_MODE_05}
    },

    {   //KWP 10400 29
        {GREATWALL_MODE_24},
        {MSG_Init_KWP_PackType_GREATWALL},
        {0x2B,0x01,0x00,0x28,0xA0,0x02,0x00,0x03,0xC0,0x04,0xbb,0xdd,0xc1,0xc1,0x05,0x0f,0x21,0x37,0x06,0x04,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x25,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
        {0x01,0x81},
        {0X00},
        {0x04,0x18,0X00,0XFF,0x00},
        {5,536,2200,2462,395,2027} , //嘉誉,V80 F5 H3 H5
        {DTC_MODE_GREATWALL_MODE_05}
    },

    {   //KWP 10400 29
        {GREATWALL_MODE_25},
        {MSG_Init_KWP_PackType_GREATWALL},
        {0x2B,0x01,0x00,0x28,0xA0,0x02,0x00,0x03,0xC0,0x04,0xcc,0xff,0xc1,0xc1,0x05,0x0f,0x21,0x37,0x06,0x04,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x25,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
        {0x01,0x81},
        {0X00},
        {0x04,0x18,0X00,0XFF,0x00},
        {5,536,2200,2462,395,2027},  //嘉誉,V80 F5 H3 H5
        {DTC_MODE_GREATWALL_MODE_05}
    },

    {   //KWP 10400 29
        {GREATWALL_MODE_26},
        {MSG_Init_KWP_PackType_GREATWALL},
        {0x2B,0x01,0x00,0x28,0xA0,0x02,0x00,0x03,0xC0,0x04,0x11,0xff,0xc1,0xc1,0x05,0x0f,0x21,0x37,0x06,0x04,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x25,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
        {0x01,0x81},
        {0X00},
        {0x04,0x18,0X00,0XFF,0x00},
        {5,536,2200,2462,395,2027},  //嘉誉,V80 F5 H3 H5
        {DTC_MODE_GREATWALL_MODE_05}
    },
    {   //KWP 10400 29
        {GREATWALL_MODE_27},
        {MSG_Init_KWP_PackType_GREATWALL},
        {0x2B,0x01,0x00,0x28,0xA0,0x02,0x00,0x03,0xC0,0x04,0x33,0xff,0xc1,0xc1,0x05,0x0f,0x21,0x37,0x06,0x04,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x25,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
        {0x01,0x81},
        {0X00},
        {0x04,0x18,0X00,0XFF,0x00},
        {5,536,2200,2462,395,2027},  //嘉誉,V80 F5 H3 H5
        {DTC_MODE_GREATWALL_MODE_05}
    },




    //Body  //BCM
    {   //CAN
        {GREATWALL_MODE_28}, //TYPE 25 TYPE 15 TYPE 24 TYPE 22
        {MSG_Init_CAN_PackType_GREATWALL},
        {0x2c,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x07,0x65,0x05,0x00,0x00,0x07,0xA5,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x01,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6},
        {0x02,0x10,0x01},
        {0X03,0x19,0x01,0x01},
        {0x03,0x19,0x02,0x01},
        {11,492,2304,2120,2462,3101,3454,2615,4168,2123,3074,4252}, 
				//M4,C50,F5,F6,H1,H2,H2S,H6,H7,VV7

        {DTC_MODE_GREATWALL_MODE_01}

    },
    {   //CAN
        {GREATWALL_MODE_29}, //TYPE 12 TYPE 16 TYPE 19
        {MSG_Init_CAN_PackType_GREATWALL},
        {0x2c,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x07,0x65,0x05,0x00,0x00,0x07,0xA5,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x01,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6},
        {0x02,0x10,0x01},
        {0X03,0x19,0x01,0x01},
        {0x03,0x19,0x02,0x01},
        {7,2090,395,4608,2027,3481,4253,4441}, 
				//C30,H3,H4,H5,H6COUPE,VV5,P8
        {DTC_MODE_GREATWALL_MODE_03}

    },

    {   //CAN
        {GREATWALL_MODE_30}, //TYPE 25
        {MSG_Init_CAN_PackType_GREATWALL},
        {0x2c,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x06,0x12,0x05,0x00,0x00,0x06,0x13,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x01,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6},
        {0x02,0x10,0x40},
        {0X03,0x19,0x01,0x20},
        {0x03,0x19,0x02,0x20},
        {2,2124,3298},  //H8,H9
        {DTC_MODE_GREATWALL_MODE_01}

    },

    {
        //AIRBAG
        {GREATWALL_MODE_31},  //TYPE 15 TYPE 25 TYPE 3 TYPE 24
        {MSG_Init_CAN_PackType_GREATWALL},
        {0x2c,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x07,0x6A,0x05,0x00,0x00,0x07,0xAA,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x01,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6},
        {0x02,0x10,0x03},
        {0x03,0x19,0x01,0x01},
        {0x03,0x19,0x02,0x01},
        {13,2120,2462,3101,2615,4168,395,4608,2027,2123,3481,3074,4485,4252}, 
				//C50 F5 F5 H2 H2S H3 H4 H5 ,H6  H6COUPE,H7,M6,VV7
        {DTC_MODE_GREATWALL_MODE_01}
    },

		//AIRBAG AIRBAG AIRBAG AIRBAG AIRBAG AIRBAG AIRBAG
    {
        
        {GREATWALL_MODE_32},  //TYPE 16
        {MSG_Init_CAN_PackType_GREATWALL},
        {0x2c,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x07,0x6A,0x05,0x00,0x00,0x07,0xAA,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x01,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6},
        {0x02,0x10,0x03},
        {0x03,0x19,0x01,0x01},
        {0x03,0x19,0x02,0x01},
        {2,4253,4441},  
				//,VV5,P8
        {DTC_MODE_GREATWALL_MODE_03}
    },

    {
        //AIRBAG
        {GREATWALL_MODE_33},  //TYPE 25
        {MSG_Init_CAN_PackType_GREATWALL},
        {0x2c,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x06,0xA2,0x05,0x00,0x00,0x06,0xA3,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x01,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6},
        {0x02,0x10,0x03},
        {0x03,0x19,0x01,0x01},
        {0x03,0x19,0x02,0x01},
        {2,2124,3298},  //,H8,H9
        {DTC_MODE_GREATWALL_MODE_01}
    },

    {
        //AIRBAG
        {GREATWALL_MODE_34},  //TYPE 15
        {MSG_Init_CAN_PackType_GREATWALL},
        {0x2c,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x06,0x6A,0x05,0x00,0x00,0x07,0xAA,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x01,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6},
        {0x02,0x10,0x03},
        {0x03,0x19,0x01,0x08},
        {0x03,0x19,0x02,0x08},
        {1,2123},
				//,H6
        {DTC_MODE_GREATWALL_MODE_01}
    },

    {
        //AIRBAG
        {GREATWALL_MODE_35},  //TYPE 15
        {MSG_Init_CAN_PackType_GREATWALL},
        {0x2c,0x01,0x07,0xA1,0x20,0x02,0x82,0x03,0x01,0x04,0x00,0x00,0x06,0x6A,0x05,0x00,0x00,0x07,0xAA,0x06,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x09,0x01,0x00,0x0a,0x00,0x00,0x0b,0x00,0x00,0x0c,0xE6},
        {0x02,0x10,0x03},
        {0x03,0x19,0x01,0x09},
        {0x03,0x19,0x02,0x09},
        {1,2027}, 
				//,H5
        {DTC_MODE_GREATWALL_MODE_01}
    },


    {
        {GREATWALL_MODE_36}, //TYPE 40
        {MSG_Init_KWP_PackType_GREATWALL},
        {0x2B,0x01,0x00,0x28,0xA0,0x02,0x00,0x03,0xC0,0x04,0x77,0xff,0xc1,0xc1,0x05,0x0f,0x21,0x37,0x06,0x04,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x25,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
        {0x01,0x81},
        {0x00},
        {0x03,0x17,0xff,0x00},
        {6,624,492,493,2001,2304,2122},
				//M2 M4 C20R
        {DTC_MODE_GREATWALL_MODE_07}
    },
    {
        {GREATWALL_MODE_37}, //TYPE 40
        {MSG_Init_KWP_PackType_GREATWALL},
        {0x2B,0x01,0x00,0x28,0xA0,0x02,0x00,0x03,0xC0,0x04,0x33,0xff,0xc1,0xc1,0x05,0x0f,0x21,0x37,0x06,0x04,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x25,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
        {0x01,0x81},
        {0x00},
        {0x03,0x17,0xff,0x00},
        {6,624,492,493,2001,2304,2122},
				//M2 M4 C20R
        {DTC_MODE_GREATWALL_MODE_07}
    },
    {
        {GREATWALL_MODE_38}, //TYPE 40
        {MSG_Init_KWP_PackType_GREATWALL},
        {0x2B,0x01,0x00,0x28,0xA0,0x02,0x00,0x03,0xC0,0x04,0x88,0xff,0xc1,0xc1,0x05,0x0f,0x21,0x37,0x06,0x04,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x25,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
        {0x01,0x81},
        {0x00},
        {0x03,0x17,0xff,0x00},
        {6,624,492,493,2001,2304,2122},
				//M2 M4 C20R
        {DTC_MODE_GREATWALL_MODE_07}
    },
    {
        {GREATWALL_MODE_39}, //TYPE 40
        {MSG_Init_KWP_PackType_GREATWALL},
        {0x2B,0x01,0x00,0x28,0xA0,0x02,0x00,0x03,0xC0,0x04,0x11,0xff,0xc1,0xc1,0x05,0x0f,0x21,0x37,0x06,0x04,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x25,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
        {0x01,0x81},
        {0x00},
        {0x03,0x17,0xff,0x00},
        {6,624,492,493,2001,2304,2122},//M2 M4 C20R
        {DTC_MODE_GREATWALL_MODE_07}
    },
    {
        {GREATWALL_MODE_40}, //TYPE 3
        {MSG_Init_KWP_PackType_GREATWALL},
        {0x2B,0x01,0x00,0x28,0xA0,0x02,0x00,0x03,0xC0,0x04,0x77,0xff,0xc1,0xc1,0x05,0x0f,0x21,0x37,0x06,0x04,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x25,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
        {0x01,0x81},
        {0x00},
        {0x04,0x18,0X00,0x80,0x00},
        {5,536,2200,2462,395,2027},//嘉誉,V80,F5 H3 H5
        {DTC_MODE_GREATWALL_MODE_05}
    },
    {
        {GREATWALL_MODE_41}, //TYPE 12
        {MSG_Init_KWP_PackType_GREATWALL},
        {0x2B,0x01,0x00,0x28,0xA0,0x02,0x00,0x03,0xC0,0x04,0x77,0xff,0xc1,0xc1,0x05,0x0f,0x21,0x37,0x06,0x04,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x25,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
        {0x01,0x81},
        {0x00},
        {0x04,0x18,0X00,0xFF,0x00},
        {6,624,492,2304,2122,2090,3454},//M4 C20R C30 H1
        {DTC_MODE_GREATWALL_MODE_02}
    },
    {
        {GREATWALL_MODE_42}, //TYPE 3
        {MSG_Init_KWP_PackType_GREATWALL},
        {0x2B,0x01,0x00,0x28,0xA0,0x02,0x00,0x03,0xC0,0x04,0x77,0xff,0xc1,0xc1,0x05,0x0f,0x21,0x37,0x06,0x04,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x25,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
        {0x01,0x81},
        {0x00},
        {0x04,0x18,0X00,0xFF,0x00},
        {2,395,2027},//M3 H5
        {DTC_MODE_GREATWALL_MODE_05}
    },
    {
        {GREATWALL_MODE_43}, //TYPE 36
        {MSG_Init_KWP_PackType_GREATWALL},
        {0x2B,0x01,0x00,0x28,0xA0,0x02,0x00,0x03,0xC0,0x04,0x77,0xff,0xc1,0xc1,0x05,0x0f,0x21,0x37,0x06,0x04,0x00,0x19,0x06,0x03,0x07,0x02,0x00,0x19,0x80,0x19,0x08,0x25,0x09,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x0c,0x01,0x00,0x00},
        {0x01,0x81},
        {0x00},
        {0x04,0x18,0X02,0xFF,0x00},
        {1,2462},//F5
        {DTC_MODE_GREATWALL_MODE_08}
    }

};



// ECU INIT
//const OBDII_CMD_INIT InitEcuCmd_GREATWALL[]=\
//{
//	{
//		{GREATWALL_MODE_01},
//		{0},
//		{0}
//	},
//	{
//		{GREATWALL_MODE_02},
//		{0},
//		{0}
//	},
//	{
//		{GREATWALL_MODE_03},
//		{MSG_Init_Ecu_KWP_PackType_GREATWALL},
//		{01,0x81}
//	},
//	{
//		{GREATWALL_MODE_04},
//		{MSG_Init_Ecu_KWP_PackType_GREATWALL},
//		{01,0x81}
//	},
//	{
//		{GREATWALL_MODE_05},
//		{MSG_Init_Ecu_KWP_PackType_GREATWALL},
//		{01,0x81}
//	}
//};

//ECU select
//const OBDII_CMD_INIT EcuSelectCmd_GREATWALL[]=\
//{
//	{
//		{GREATWALL_MODE_01},
//		{MSG_ALL_CAN_PackType_GREATWALL},
//		{0x01,0x3e}
//	},
//	{
//		{GREATWALL_MODE_02},
//		{MSG_ALL_CAN_PackType_GREATWALL},
//		{0x02,0x3e,0x00}
//	},
//	{
//		{GREATWALL_MODE_03},
//		{MSG_ALL_KWP_PackType_GREATWALL},
//		{0x02,0x01,0x00}
//	},
//	{
//		{GREATWALL_MODE_04},
//		{MSG_ALL_KWP_PackType_GREATWALL},
//		{0x02,0x01,0x00}
//	},
//	{
//		{GREATWALL_MODE_05},
//		{MSG_ALL_KWP_PackType_GREATWALL},
//		{0x02,0x01,0x00}
//	},

//};


//公共函数
bool SetFrameTimeOut_GREATWALL(INIT_MODE_GREATWALL index)
{
    uint8_t  ord;
    uint8_t initdata[128];
    uint8_t number,i=0;
    uint32_t canid;
    m_CAN_ANS_T_GREATWALL = 0;
    m_CAN_ANS_R_GREATWALL = 0;

    ord = InitIoCmd_GREATWALL[index].Msg_Ord;
    number = InitIoCmd_GREATWALL[index].CMD_DATA[0];
    memcpy(initdata,InitIoCmd_GREATWALL[index].CMD_DATA+1,number);
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
                m_CAN_LISTEN_TYPE_GREATWALL =initdata[i+1];
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
//				m_CAN_ANS_T_GREATWALL = initdata[i+1];
                i+=2;
                break;
            case 0x0e:
//				m_CAN_ANS_R_GREATWALL = initdata[i+1];
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
//void pro_set_init_ecu_ord_GREATWALL(INIT_MODE_GREATWALL mode)
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


//设置通信过程ORD
void pro_msg_all_GREATWALL(INIT_MODE_GREATWALL mode)
{
    //cmd_set_two_id_GREATWALL(0x00);
    switch(mode)
    {
        //CAN  ENG
    case GREATWALL_MODE_00:
    case GREATWALL_MODE_01:
    case GREATWALL_MODE_02:
    case GREATWALL_MODE_03:
    case GREATWALL_MODE_04:
    case GREATWALL_MODE_05:
    case GREATWALL_MODE_06:

    case GREATWALL_MODE_12://AT
    case GREATWALL_MODE_13:
    case GREATWALL_MODE_14:
    case GREATWALL_MODE_15:
    case GREATWALL_MODE_16:

    case GREATWALL_MODE_19: //ABS
    case GREATWALL_MODE_20:
    case GREATWALL_MODE_21:

    case GREATWALL_MODE_28://BCM
    case GREATWALL_MODE_29:
    case GREATWALL_MODE_30:

    case GREATWALL_MODE_31://AIRBIG
    case GREATWALL_MODE_32:
    case GREATWALL_MODE_33:
    case GREATWALL_MODE_34:
    case GREATWALL_MODE_35:


        g_defaultMsgAttr.m_CS_MODE = 0;
        g_defaultMsgAttr.m_ORD     = 0x30;
        g_defaultMsgAttr.m_PCK_TYPE= 0x22;
        g_defaultMsgAttr.m_RWAYS   = 0;
        g_defaultMsgAttr.m_SRC     = 0;
        g_defaultMsgAttr.m_TGT     = 0;
        g_defaultMsgAttr.m_WAYS    = 0;
        break;


        //KWP
    case GREATWALL_MODE_07:
    case GREATWALL_MODE_08:
    case GREATWALL_MODE_09:
    case GREATWALL_MODE_11:

        g_defaultMsgAttr.m_CS_MODE = 0;
        g_defaultMsgAttr.m_ORD     = 0x33;
        g_defaultMsgAttr.m_PCK_TYPE= 0x12;
        g_defaultMsgAttr.m_RWAYS   = 0;
        g_defaultMsgAttr.m_SRC     = 0xF1;
        g_defaultMsgAttr.m_TGT     = 0x11;
        g_defaultMsgAttr.m_WAYS    = 0;
        break;

    case GREATWALL_MODE_10:
        g_defaultMsgAttr.m_CS_MODE = 0;
        g_defaultMsgAttr.m_ORD     = 0x33;
        g_defaultMsgAttr.m_PCK_TYPE= 0x12;
        g_defaultMsgAttr.m_RWAYS   = 0;
        g_defaultMsgAttr.m_SRC     = 0xF1;
        g_defaultMsgAttr.m_TGT     = 0x10;
        g_defaultMsgAttr.m_WAYS    = 0;
        break;

        //变速箱KWP
    case GREATWALL_MODE_17:
    case GREATWALL_MODE_18:
        g_defaultMsgAttr.m_CS_MODE = 0;
        g_defaultMsgAttr.m_ORD     = 0x33;
        g_defaultMsgAttr.m_PCK_TYPE= 0x12;
        g_defaultMsgAttr.m_RWAYS   = 0;
        g_defaultMsgAttr.m_SRC     = 0xF1;
        g_defaultMsgAttr.m_TGT     = 0x18;
        g_defaultMsgAttr.m_WAYS    = 0;
        break;

    case GREATWALL_MODE_22:
        g_defaultMsgAttr.m_CS_MODE = 0;
        g_defaultMsgAttr.m_ORD     = 0x33;
        g_defaultMsgAttr.m_PCK_TYPE= 0x12;
        g_defaultMsgAttr.m_RWAYS   = 0;
        g_defaultMsgAttr.m_SRC     = 0xF1;
        g_defaultMsgAttr.m_TGT     = 0x28;
        g_defaultMsgAttr.m_WAYS    = 0;
        break;


    case GREATWALL_MODE_24:
    case GREATWALL_MODE_25:
    case GREATWALL_MODE_26:
    case GREATWALL_MODE_27:
        g_defaultMsgAttr.m_CS_MODE = 0;
        g_defaultMsgAttr.m_ORD     = 0x33;
        g_defaultMsgAttr.m_PCK_TYPE= 0x12;
        g_defaultMsgAttr.m_RWAYS   = 0;
        g_defaultMsgAttr.m_SRC     = 0xF1;
        g_defaultMsgAttr.m_TGT     = 0x22;
        g_defaultMsgAttr.m_WAYS    = 0;
        break;
    case GREATWALL_MODE_36:
    case GREATWALL_MODE_37:
    case GREATWALL_MODE_38:
    case GREATWALL_MODE_39:
        g_defaultMsgAttr.m_CS_MODE = 0;
        g_defaultMsgAttr.m_ORD     = 0x33;
        g_defaultMsgAttr.m_PCK_TYPE= 0x12;
        g_defaultMsgAttr.m_RWAYS   = 0;
        g_defaultMsgAttr.m_SRC     = 0xF1;
        g_defaultMsgAttr.m_TGT     = 0x2C;
        g_defaultMsgAttr.m_WAYS    = 0;
        break;

    case GREATWALL_MODE_40:
    case GREATWALL_MODE_43:
        g_defaultMsgAttr.m_CS_MODE = 0;
        g_defaultMsgAttr.m_ORD     = 0x33;
        g_defaultMsgAttr.m_PCK_TYPE= 0x12;
        g_defaultMsgAttr.m_RWAYS   = 0;
        g_defaultMsgAttr.m_SRC     = 0xF1;
        g_defaultMsgAttr.m_TGT     = 0x59;
        g_defaultMsgAttr.m_WAYS    = 0;
        break;
    case GREATWALL_MODE_41:
    case GREATWALL_MODE_42:
        g_defaultMsgAttr.m_CS_MODE = 0;
        g_defaultMsgAttr.m_ORD     = 0x33;
        g_defaultMsgAttr.m_PCK_TYPE= 0x12;
        g_defaultMsgAttr.m_RWAYS   = 0;
        g_defaultMsgAttr.m_SRC     = 0xF1;
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
bool Init_IO_GREATWALL(INIT_MODE_GREATWALL index)
{
    uint16_t number = 0;
    INIT_MODE_GREATWALL mode = 0;
    uint8_t sendbuf[128];
    uint8_t recvbuf[32];
    if(!SetFrameTimeOut_GREATWALL(index))
    {
        return false;
    }
    memset(recvbuf,0,sizeof(recvbuf));
    number = InitIoCmd_GREATWALL[index].CMD_DATA[0];
    memcpy(sendbuf, InitIoCmd_GREATWALL[index].CMD_DATA+1, number);
    mode = InitIoCmd_GREATWALL[index].Init_Mode;
    g_defaultMsgAttr.m_PCK_TYPE = 0x00;
    number = pro_packsenddata(sendbuf, number, InitIoCmd_GREATWALL[index].Msg_Ord);
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
    m_canbussendid = InitIoCmd_GREATWALL[index].CMD_DATA[12] * 0x100 + InitIoCmd_GREATWALL[index].CMD_DATA[13];
//	pro_set_can_id_GREATWALL(index);
    printfuartdebug("InitIO true\r\n");
    return true;
}


void pro_set_can_id_GREATWALL(INIT_MODE_GREATWALL mode)
{
    switch(mode)
    {
    case GREATWALL_MODE_01:
        m_canbussendid = 0x000007E0;
        m_canbusreadid = 0x000007E8;
        m_canbusflowid = 0x000007E0;
        break;

    case GREATWALL_MODE_02:
        m_canbussendid = 0x00000700;
        m_canbusreadid = 0x000007E8;
        m_canbusflowid = 0x00000700;
        break;

    default:
        break;
    }
}

bool pro_much_init_GREATWALL(INIT_MODE_GREATWALL mode)// CAN协议进系统
{
    uint8_t sendbuf[128] = {0};
    bool bRet = false;
    MSG_ORD_GREATWALL PackType =  InitIoCmd_GREATWALL[mode].Msg_Ord;

    memset(sendbuf,0,sizeof(sendbuf));

    switch(PackType)
    {
        //CAN
    case MSG_Init_CAN_PackType_GREATWALL:
        pro_msg_all_GREATWALL(mode);
        bRet = pro_canbus_select_module_GREATWALL(mode);
        break;

        //KWP
    case MSG_Init_KWP_PackType_GREATWALL:
        pro_msg_all_GREATWALL(mode);
        bRet = pro_kwp_select_module_GREATWALL(mode);//kwp的81进系统命令
				printfUartObdiideBug("bRet =============  %d\r\n" , bRet);
        break;

    default:
        break;
    }

    return bRet;
}
//p0001,p0002,
bool pro_kwp_select_module_GREATWALL(INIT_MODE_GREATWALL mode)//kwp的81进系统命令
{
    uint16_t number = 0;
    uint8_t sendbuf[128] = {0};
    uint8_t recvbuf[256] = {0};
    uint8_t ord = 0;

    memcpy(sendbuf,InitIoCmd_GREATWALL[mode].System_Enter,InitIoCmd_GREATWALL[mode].System_Enter[0]+1);

    ord = g_defaultMsgAttr.m_ORD;//临时保存
    g_defaultMsgAttr.m_ORD = 0x23;//进系统 临时改成0x23

    number = pro_SendRecv(sendbuf, recvbuf);

    g_defaultMsgAttr.m_ORD = ord;//还原回去

    if(number == 0)
    {
        return false;
    }

    return true;
}

bool pro_canbus_select_module_GREATWALL(INIT_MODE_GREATWALL mode)
{
    int i, j;
    uint8_t sendbuf[128];
    uint8_t recvbuf[256];
    uint8_t sendnum =0;
    uint16_t number=0;
    uint32_t canid;

    sendnum = InitIoCmd_GREATWALL[mode].System_Enter[0];
    memset(sendbuf,0,sizeof(sendbuf));
    memcpy(sendbuf, InitIoCmd_GREATWALL[mode].System_Enter,sendnum+1);

    number = pro_SendRecv(sendbuf, recvbuf); //进系统 带长度

    if(number<1)
    {
        return false;
    }

    return true;
}


//目前发现的规律是AVANZA的EFI发动机的码库和普通的发动机码库差距比较大，但是意思总体上差不多
//0700的和07E0的码库差异比较大 发动机和波箱
//0001和003d的码库类似
//需要核对下Engine	00ce.xml EFI	0246.xml EFI	024f.xml 等系统命令是否有错误,核对后发现无误
//EU	CAN	F65#	1508-1609	A/T		AVANZA	182A	024f.xml
//EU	CAN	B10#LA	1KR-DE			M/T			WIGO	1BFD 0247.xml

INIT_MODE_GREATWALL pro_read_dtc_GREATWALL(SYSTEM_GREATWALL sys,INIT_MODE_GREATWALL index,int* dtc_number,char* dtc_list,char* dtc_sys)//读码
{
    uint8_t sendDtcNumCmd[8] = {0}; //读当前码
    uint8_t sendcmd[8] = {0}; //读当前码
    uint8_t recvbuf[128] = {0}; //

    uint8_t number = 0;
    uint8_t dtcnumber = 0;
    uint8_t ndtcnumber = 0;
    bool bFlag = TRUE;
    char dtc[16] = {0};
    char dtcname[32][16] = {0};//最多32个码
    char dtcsystem[4] = {0};
    int i = 0;
    int total = 0;

    sprintf(dtcsystem, "%d", sys);//系统
    if(InitIoCmd_GREATWALL[index].DTC_ReadNum[0] > 0)
    {
        memcpy(sendDtcNumCmd, InitIoCmd_GREATWALL[index].DTC_ReadNum,InitIoCmd_GREATWALL[index].DTC_ReadNum[0]+1);
        number = pro_SendRecv(sendDtcNumCmd, recvbuf);
        if(number == 0)
            return GREATWALL_MODE_50;
    }
    memcpy(sendcmd, InitIoCmd_GREATWALL[index].DTC_Read,InitIoCmd_GREATWALL[index].DTC_Read[0]+1);
    number = pro_SendRecv(sendcmd, recvbuf); //当前故障码
    if(number == 0)
    {
        return GREATWALL_MODE_50;
    }

    switch(InitIoCmd_GREATWALL[index].DTC_Mode)
    {
    case DTC_MODE_GREATWALL_MODE_01:

        dtcnumber = ( number - 3) / 4;
        for(i = 0; i < dtcnumber; i++)
        {
            dtc_sprintf_GREATWALL_Mode_1((char *)dtc, recvbuf + i * 4 + 3);

            sprintf(dtc,"%s%02X%d",dtc,recvbuf[i * 4 + 2 + 3],tom_vehicle_model_data.child_vehicle_model);
            if(*dtc_number != 0)
            {   //60个字节，用","分隔
                strcat(dtc_list,",");
                strcat(dtc_sys,",");
            }
            strcat(dtc_list,dtc);
            strcat(dtc_sys,dtcsystem);
            (*dtc_number)++;
        }
        printfUartObdiideBug("GREATWALL add dtc:%s system:%s \r\n",dtc,dtc_sys);
        printfUartObdiideBug("GREATWALL dtc_list:%s dtc_number:%d \r\n",dtc_list,*dtc_number);
        break;

    case DTC_MODE_GREATWALL_MODE_02:
        dtcnumber = ( number - 3) / 4;
        for(i = 0; i < dtcnumber; i++)
        {
            dtc_sprintf_GREATWALL_Mode_1((char *)dtc, recvbuf + i * 4 + 3);

            sprintf(dtc,"%s%d",dtc,tom_vehicle_model_data.child_vehicle_model);
            if(*dtc_number != 0)
            {   //60个字节，用","分隔
                strcat(dtc_list,",");
                strcat(dtc_sys,",");
            }
            strcat(dtc_list,dtc);
            strcat(dtc_sys,dtcsystem);
            (*dtc_number)++;


        }
        printfUartObdiideBug("GREATWALL add dtc:%s system:%s \r\n",dtc,dtc_sys);
        printfUartObdiideBug("GREATWALL dtc_list:%s dtc_number:%d \r\n",dtc_list,*dtc_number);
        break;
    case DTC_MODE_GREATWALL_MODE_03:
        dtcnumber = ( number - 3) / 4;
        for(i = 0; i < dtcnumber; i++)
        {


            sprintf(dtc,"%02X%02X%02X%d",recvbuf[i*4 + 3],recvbuf[i*4 + 4],recvbuf[i*4 + 5],tom_vehicle_model_data.child_vehicle_model);
            if(*dtc_number != 0)
            {   //60个字节，用","分隔
                strcat(dtc_list,",");
                strcat(dtc_sys,",");
            }
            strcat(dtc_list,dtc);
            strcat(dtc_sys,dtcsystem);
            (*dtc_number)++;


        }
        printfUartObdiideBug("GREATWALL add dtc:%s system:%s \r\n",dtc,dtc_sys);
        printfUartObdiideBug("GREATWALL dtc_list:%s dtc_number:%d \r\n",dtc_list,*dtc_number);
        break;
    case DTC_MODE_GREATWALL_MODE_04:
        dtcnumber = ( number - 2) / 4;
        for(i = 0; i < dtcnumber; i++)
        {

            dtc_sprintf_GREATWALL_Mode_1((char *)dtc, recvbuf + i * 2 + 2);

            sprintf(dtc,"%s%d",dtc,tom_vehicle_model_data.child_vehicle_model);

            if(*dtc_number != 0)
            {   //60个字节，用","分隔
                strcat(dtc_list,",");
                strcat(dtc_sys,",");
            }
            strcat(dtc_list,dtc);
            strcat(dtc_sys,dtcsystem);
            (*dtc_number)++;


        }
        printfUartObdiideBug("GREATWALL add dtc:%s system:%s \r\n",dtc,dtc_sys);
        printfUartObdiideBug("GREATWALL dtc_list:%s dtc_number:%d \r\n",dtc_list,*dtc_number);
        break;
    case DTC_MODE_GREATWALL_MODE_05:
        dtcnumber = ( number - 2) / 4;
        for(i = 0; i < dtcnumber; i++)
        {

            dtc_sprintf_GREATWALL_Mode_1((char *)dtc, recvbuf + i * 4 + 2);

            sprintf(dtc,"%s%d",dtc,tom_vehicle_model_data.child_vehicle_model);

            if(*dtc_number != 0)
            {   //60个字节，用","分隔
                strcat(dtc_list,",");
                strcat(dtc_sys,",");
            }
            strcat(dtc_list,dtc);
            strcat(dtc_sys,dtcsystem);
            (*dtc_number)++;


        }
        printfUartObdiideBug("GREATWALL add dtc:%s system:%s \r\n",dtc,dtc_sys);
        printfUartObdiideBug("GREATWALL dtc_list:%s dtc_number:%d \r\n",dtc_list,*dtc_number);
        break;
    case DTC_MODE_GREATWALL_MODE_06:

        break;
    case DTC_MODE_GREATWALL_MODE_07:


        dtcnumber = ( number - 2) / 3;
        for(i = 0; i < dtcnumber; i++)
        {

            sprintf(dtc,"%02X%02X%d",recvbuf[i*3 + 1],recvbuf[i*3 + 2],tom_vehicle_model_data.child_vehicle_model);

            if(*dtc_number != 0)
            {   //60个字节，用","分隔
                strcat(dtc_list,",");
                strcat(dtc_sys,",");
            }
            strcat(dtc_list,dtc);
            strcat(dtc_sys,dtcsystem);
            (*dtc_number)++;


        }
        printfUartObdiideBug("GREATWALL add dtc:%s system:%s \r\n",dtc,dtc_sys);
        printfUartObdiideBug("GREATWALL dtc_list:%s dtc_number:%d \r\n",dtc_list,*dtc_number);
        break;
    case DTC_MODE_GREATWALL_MODE_08:
        dtcnumber = ( number - 2) / 3;
        for(i = 0; i < dtcnumber; i++)
        {

            dtc_sprintf_GREATWALL_Mode_1((char *)dtc, recvbuf + i * 3 + 2);

            sprintf(dtc,"%s%d",dtc,tom_vehicle_model_data.child_vehicle_model);

            if(*dtc_number != 0)
            {   //60个字节，用","分隔
                strcat(dtc_list,",");
                strcat(dtc_sys,",");
            }
            strcat(dtc_list,dtc);
            strcat(dtc_sys,dtcsystem);
            (*dtc_number)++;


        }
        printfUartObdiideBug("GREATWALL add dtc:%s system:%s \r\n",dtc,dtc_sys);
        printfUartObdiideBug("GREATWALL dtc_list:%s dtc_number:%d \r\n",dtc_list,*dtc_number);
        break;
    default:
        break;
    }
    printfUartObdiideBug("zhangnf  come here 3\r\n");
    return index;
}

void dtc_sprintf_GREATWALL_Mode_1(char *strbuf,uint8_t *buf)
{

    switch(buf[0] & 0xC0)
    {
    case 0x00: // p
        sprintf(strbuf, "P%04X", (buf[0] & 0x3F) * 0x100 + buf[1]);
        break;
    case 0x40: // c
        sprintf(strbuf, "C%04X", (buf[0] & 0x3F) * 0x100 + buf[1]);
        break;
    case 0x80: // b
        sprintf(strbuf, "B%04X", (buf[0] & 0x3F) * 0x100 + buf[1]);
        break;
    case 0xC0: // u
        sprintf(strbuf, "U%04X", (buf[0] & 0x3F) * 0x100 + buf[1]);
    }
}

void dtc_sprintf_GREATWALL_Mode_2(char *strbuf,uint8_t *buf)
{
    sprintf(strbuf, "X%06X", (buf[0]) * 0x10000 + buf[1] * 0x100 + buf[2]);
}


void pro_clear_dtc_GREATWALL(INIT_MODE_GREATWALL index) //清码
{

    uint8_t recvbuf[128] = {0}; //
    uint8_t number = 0;

    number =	pro_SendRecv("\x04\x14\xff\xff\xff", recvbuf); //清码命令不做判断
    if(number > 0 && recvbuf[0] != 0x7f)
        return;
    number = 	pro_SendRecv("\x03\x14\xff\x00", recvbuf); //清码命令不做判断
    if(number > 0 && recvbuf[0] != 0x7f)
        return;
    number = pro_SendRecv("\x03\x14\x00\x00", recvbuf); //清码命令不做判断
    if(number >0 && recvbuf[0] != 0x7f)
        return;
    number = pro_SendRecv("\x03\x14\x80\x00", recvbuf); //清码命令不做判断
    //memcpy(sendcmd, InitIoCmd_GREATWALL[index].DTC_Clear,InitIoCmd_GREATWALL[index].DTC_Clear[0]+1);


}

void pro_exit_system_GREATWALL(INIT_MODE_GREATWALL index) //退出系统
{
    uint8_t sendcmd[8] = {0}; //退出
    uint8_t recvbuf[64] = {0}; //
    uint8_t number = 0;

//	if(InitIoCmd_GREATWALL[index].System_Exit[0]>0)
//	{
//		memcpy(sendcmd, InitIoCmd_GREATWALL[index].System_Exit,InitIoCmd_GREATWALL[index].System_Exit[0]+1);
//		number = pro_SendRecv(sendcmd, recvbuf); //退出系统命令不做判断
//	}
}


bool Much_Init_GREATWALL()
{
    uint8_t i = 0,j = 0;
    uint8_t initstart = 0,initend = 0;
    INIT_MODE_GREATWALL mode = 0;
    bool enter = false;
    //int num = 0;
    //char buf[60] = {0};

    uint8_t iSystem = 5;//一共5个系统

    g_vehiclediagnosie.fault_total = 0;
    memset(g_vehiclediagnosie.fault_list,0,sizeof(g_vehiclediagnosie.fault_list));//故障码
    memset(g_vehiclediagnosie.sys_list,0,sizeof(g_vehiclediagnosie.sys_list));//系统编号 1发动机 2传动 3ABS 4车身 5安全

    printfUartObdiideBug("welcome to [%s]\r\n",__func__);
//		get_vehicle_info(&tom_vehicle_model_data);
    if(m_first_GREATWALL)
    {
        //第一次进入
        m_first_GREATWALL = false;

        initstart = 0;//从第一个系统开始扫描时重置
        initend = 0;//从第一个系统开始扫描时重置
        for (i = GREATWALL_ENGINE; i <= GREATWALL_SRS; i++)
        {
            initstart = initend;
            initend += GREATWALL_iSystem_Mode_Size[i];
            for (j = initstart; j < initend; j++)
            {
                //tom_vehicle_model_data.child_vehicle_model = 4168;
						
								printfUartObdiideBug("[%s] tom_vehicle_model_data.child_vehicle_model === %d\r\n",__func__,tom_vehicle_model_data.child_vehicle_model);
							
                if(! IsTypeOfVechile( tom_vehicle_model_data.child_vehicle_model,InitIoCmd_GREATWALL[j].VechileSubTypes))
                    continue;
                //reset_global_data();//重置全局变量数据
                if(Init_IO_GREATWALL(InitIoCmd_GREATWALL[j].Init_Mode)) //初始化IO
                {
                    if(pro_much_init_GREATWALL(InitIoCmd_GREATWALL[j].Init_Mode)) //初始化ECU //进系统
                    {
                        printfUartObdiideBug("GREATWALL enter success system:%d init:%d \r\n",i,j);
                        //m_protocol_index_GREATWALL[i] = InitIoCmd_GREATWALL[j].Init_Mode;												//保存是第几个协议
                        m_protocol_index_GREATWALL[i] = pro_read_dtc_GREATWALL(i,InitIoCmd_GREATWALL[j].Init_Mode,&(g_vehiclediagnosie.fault_total),g_vehiclediagnosie.fault_list,g_vehiclediagnosie.sys_list);	//读码
                        if(m_protocol_index_GREATWALL[i] != -1 && m_protocol_index_GREATWALL[i] != GREATWALL_MODE_50)
                        {
                            pro_clear_dtc_GREATWALL(m_protocol_index_GREATWALL[i]);															//清码
                            //pro_exit_system_GREATWALL(m_protocol_index_GREATWALL[i]);														//退出
                            break;
                        }
                    }
                    else
                    {
                        printfUartObdiideBug("GREATWALL enter false system:%d init:%d (%d-%d) \r\n",i,j,initstart,initend-1);
                    }
                }
            }//for (j = initstart; j < initend; j++)
        }//for (i = 0;i<iSystem;i++)
        for(i = GREATWALL_ENGINE;i <= GREATWALL_SRS; i++)
        {		printfUartObdiideBug("come here two ------------\r\n");
            if(m_protocol_index_GREATWALL[i] != -1 && m_protocol_index_GREATWALL[i] != GREATWALL_MODE_50)
                return true;
        }
    }
    else
    {
			
        enter = false;
        for (i = GREATWALL_ENGINE; i <= GREATWALL_SRS; i++)
        {
           
            if(m_protocol_index_GREATWALL[i] != -1 && m_protocol_index_GREATWALL[i] != GREATWALL_MODE_50)//直接使用记录的协议，如果为-1，则说明不存在此系统
            {
                printfUartObdiideBug("m_protocol_index_GREATWALL[i]====%d \r\n" ,m_protocol_index_GREATWALL[i]);
                if(Init_IO_GREATWALL(InitIoCmd_GREATWALL[m_protocol_index_GREATWALL[i]].Init_Mode)) //初始化IO
                {
                    if(pro_much_init_GREATWALL(InitIoCmd_GREATWALL[m_protocol_index_GREATWALL[i]].Init_Mode)) //初始化ECU
                    {

//						if(diag_flag == 1 && data_flag == 0)//车辆诊断
//						{
//								pro_clear_dtc_GREATWALL(m_protocol_index_GREATWALL[i]);															//清码
//								vTaskDelay(100);
                        enter = true;
                        pro_read_dtc_GREATWALL(i,m_protocol_index_GREATWALL[i],&(g_vehiclediagnosie.fault_total),g_vehiclediagnosie.fault_list,g_vehiclediagnosie.sys_list);	//读码
                        //pro_exit_system_GREATWALL(m_protocol_index_GREATWALL[i]);
//								diag_flag = 0;
//						}
                       
                    }
                }
            }
        }//for (i = 0;i<iSystem;i++)
       
//        return enter;
    }

		return enter;
}







//void cmd_msg_all_canbus_11bit_GREATWALL()
//{
//    uint32_t canread, canflow;

//    canread = m_modulebuf[0];
//    canflow = canread-8;

//    if((g_defaultMsgAttr.m_PCK_TYPE == 0x22) || (g_defaultMsgAttr.m_PCK_TYPE == 0x20))
//    {
//        //m_canbussendid=getHex32(ezxml_attr(msgxml, "CANSEND"));
//        m_canbusreadid=canread;
//        m_canbusflowid=canflow;
//    }
//    //SetCanbusFilter(canread);//丰田单个系统单个ID,用不到
//}

//void cmd_msg_all_default_model_GREATWALL()
//{
//    cmd_set_ecumodule_GREATWALL(m_modulebuf[0]);
//}

//uint8_t cmd_set_ecumodule_GREATWALL(uint8_t module)
//{
//    m_setmodule = module;
//    return m_setmodule;
//}


void SetCanbusFilter_GREATWALL(uint32_t filterID)
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
    for(i=2; i<number-1; i++)
    {
        buffer[number-1] += buffer[i];
    }
    buffer[number-1] = ~buffer[number-1];
    printfUartObdiideBug("sendbuf:%s\r\n",buffer);
    USB_BULK_Queue_Write(&USB_RX_QueueData,buffer,number);
    vTaskDelay(40);
}


bool IsTypeOfVechile(int CurVel,const int* pVechArray)
{
    int i = 1;
    while(i <= pVechArray[0])
    {
        if(CurVel == pVechArray[i])
            return true;
        i++;
    }
    return false;
}




/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
