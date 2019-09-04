#include "config.h"
#include "CAN.h"
#include "VCI_queue.h"
#include "PassThru.h"
#include "PassThru_CAN.h"
#include "PortAssign.h"
#include "interface.h"


#define CTRLMODE_DIAG    0x01//诊断模式
#define CTRLMODE_SAMPLE  0x02//采样模式
#define NULL 0
#define Uart0TxQueuePack UartTxQueuePack
#define PassThru_CAN_TxData CAN_TxData
#define UARTAnswerErr_cyl UART0AnswerErr_cyl
//static uint32 CanId_RxBuf[MAX_CANID_COUNT]; //记忆
uint8 TxAddress;
uint8 CANMODE;
uint8 pgn_much_data_num=0;
//extern uint8 UartRxBuf[RXBUFLEN];  //用于和上位机通讯
//extern uint8 EcuComRxBuf[ECURXBUFLEN];
//extern uint8 EcuComTxBuf[ECUTXBUFLEN];

extern QueueData CAN1_QueueData;
extern uint8_t EcuComRxBuf[];
extern PASSTHRU_CONFIG PassThruConfig;
extern PASSTHRU_DRIVER PassThruDriver;
extern uint16 flash_record_data_num[8];
//static uint8 ErrStatus;
//static uint8 FrameFilter;   //滤波方式 单滤波 双滤波
uint32 CanId_RxNum;
uint32 CanId_RxBuf[MAX_CANID_COUNT]; //记忆

uint8 CanTransMode;

uint8 FLAG07DF = 0;        //用在02 82参数
uint8 SoftFilterMask;							//软件过滤为1需要软件过滤，0不需要软件过滤

int isHasData()
{
	if(CAN1_QueueData.len > 0)
	{
		return TRUE;
	}
	if(CAN2_QueueData.len > 0)
	{
		return TRUE;
	}
	return FALSE;
}

/*********************************************************************************
*    函数：PassThru_CAN_FilterID(uint32 LineID)
*    功能：对接受到的来自总线的ID根据滤波字进行判别，决定是否可以接受
*	 输入：LineID来自总线的ID
*	 输出： TRUE表示可以接收
*			FALSE表示不可以接收
*
***********************************************************************************/
uint8 PassThru_CAN_FilterID(uint32 LineID)
{
	uint32 MatchID0,MatchID1;

	MatchID0=PassThruConfig.CanAR1|PassThruConfig.CanMR1;
	MatchID1=PassThruConfig.CanAR2|PassThruConfig.CanMR2;
	switch(PassThruConfig.Format&0x01)
	{
		default:
		case  SINGLE_FILTER:
			//设置成单滤波器模式
			if((LineID|PassThruConfig.CanMR1)==MatchID0)
				return TRUE;
			else
				return FALSE;
			//        break;
		case DOUBLE_FILTER:
			//设置成双滤波模式
			if((LineID|PassThruConfig.CanMR1)==MatchID0)
				return TRUE;
			else if((LineID|PassThruConfig.CanMR2)==MatchID1)
				return TRUE;
			else
				return FALSE;
	}

}

//发送模式，是否需要自发自收
extern uint8 can_test_status;
/*********************************************************************************************
* 名称    ： PassThru_CAN_TxMuchPack_DelayTime
* 功能    ： CANBUS协议多帧发送时帧间间隔
* 入口参数：
*
*
* 出口参数：
* 说明    ：多帧数据发送时帧间间隔时间,如原来有设置0f参数，则用此参数，为零则根据车上回应流控制帧时间来判断
*
***********************************************************************************************/

void PassThru_CAN_TxMuchPack_DelayTime(uint8 FC_STmin) {

	if(FC_STmin==0)
		FC_STmin=0xf2;
	if (PassThruConfig.CanMidTime[PassThruConfig.Send_Pass] > 0)
		FC_STmin = PassThruConfig.CanMidTime[PassThruConfig.Send_Pass];
	if (FC_STmin <= 0x7F)
		DelayNms(FC_STmin);    //连续帧的帧间时间间隔 ms
	if (FC_STmin > 0xf0)
		DelayNus((FC_STmin & 0x0f) * 100);    //连续帧的帧间时间间隔 us
}

/*********************************************************************************************************
** Functoin name:       PassThru_CAN_SetCanBaud
** Descriptions:        设置CAN波特率,如果波特率的最高位不为1，则按标准波特率来设置，否则按照寄存器值来设置
** input paraments:     Ch:     CAN控制器号，0表示第一路CAN
** output paraments:    无
** Returned values:     0：     失败
**                      1:      成功
*********************************************************************************************************/
// uint8 PassThru_CAN_SetCanBaud(uint8 Ch, uint32 CanBaud)
// {
// 	if(CanBaud & 0x00800000)
// 	{
// 		//F3下位机设置参数 通过设置寄存器来设置波特率 SJ1000_BPS_1000K
// 		if (CanBaud == SJ1000_BPS_10K) CanBaud = BPS_10K;
// 		else if (CanBaud == SJ1000_BPS_20K) CanBaud = BPS_20K;
// 		else if (CanBaud == SJ1000_BPS_33K) CanBaud = BPS_33K;
// 		else if (CanBaud == SJ1000_BPS_50K) CanBaud = BPS_50K;
// 		else if (CanBaud == SJ1000_BPS_96K) CanBaud = BPS_96K;
// 		else if (CanBaud == SJ1000_BPS_100K) CanBaud = BPS_100K;
// 		else if (CanBaud == SJ1000_BPS_125K) CanBaud = BPS_125K; //125k
// 		else if (CanBaud == SJ1000_BPS_250K) CanBaud = BPS_250K; //250k
// 		else if (CanBaud == SJ1000_BPS_500K) CanBaud = BPS_500K; //500k
// 		else if (CanBaud == SJ1000_BPS_800K) CanBaud = BPS_800K;
// 		else if (CanBaud == SJ1000_BPS_1000K) CanBaud = BPS_1000K;
// 		else return FALSE;
// 	}
// 	CANx_ResetBaudRate(CAN1, CanBaud);
// 	return TRUE;
// }
/*********************************************************************************************
* 名称    ： PassThru_SET_EACH_FUN
* 功能    ： 设置各个函数
* 入口参数：type:7f 不带id发送和接收
*								 80 只侦听id
82: ISO 15765无扩展地址
83: ISO 15765 有扩展地址

*
* 出口参数：
* 说明    ：
*
***********************************************************************************************/
uint8 PassThru_SET_EACH_FUN(uint8 type,uint8 type1)//uint8 *Read_pass(void), uint8 *init_pass(uint8 *), uint8 *Send_pass(uint8*))
{
	uint8 (* Read_pass)(void);
	uint8 (* init_pass)(uint8 *);
	uint8 (* Send_pass)(uint8*);
	void (* Send_pass_flash)(uint8*);
	switch (type) {

	case 0x7f:

		Read_pass = PassThru_CAN_RxPack_NotID;
		init_pass = PassThru_CAN_TxPack_NotID;
		Send_pass = PassThru_CAN_TxPack_NotID;
		Send_pass_flash=PassThru_CAN_TxPack_NotID_Flash;
		break;
	case 0x80:
		Read_pass = PassThru_CAN_RxPack_GetID;
		init_pass = PassThru_CAN_TxPack_GetID;
		Send_pass = PassThru_CAN_TxPack_GetID;
		Send_pass_flash=NULL;
		break;
	case 0x81:
		Read_pass = PassThru_CAN_RxPack_Normal;
		init_pass = PassThru_CAN_TxPack_Normal;
		Send_pass = PassThru_CAN_TxPack_Normal;
		Send_pass_flash=NULL;
		break;
	case 0x82:
		//PassThruConfig.CanAnsT = 0x01;
		//PassThruConfig.CanRecAnswer=0x01;
		PassThruConfig.Can_ext_addr = 00;
		PassThruConfig.CanFill = 0x0100;
		Read_pass = PassThru_CAN_RxPack_Auto;
		init_pass = PassThru_CAN_TxPack_Auto;
		Send_pass = PassThru_CAN_TxPack_Auto;
		Send_pass_flash=NULL;
		break;
	case 0x83:
		//PassThruConfig.CanAnsT = 0x02;
		//PassThruConfig.CanRecAnswer=0x02;
		PassThruConfig.CanFill = 0x0100;
		PassThruConfig.Can_ext_addr = 01;
		Read_pass = PassThru_CAN_RxPack_Auto_Ext;
		init_pass = PassThru_CAN_TxPack_Auto_Ext;
		Send_pass = PassThru_CAN_TxPack_Auto_Ext;
		Send_pass_flash=NULL;
		break;

	case 0x84:
		Read_pass = PassThru_CAN_RxPack_J1939_ReadOnly;
		init_pass = PassThru_CAN_TxPack_J1939_SendOnly;
		Send_pass = PassThru_CAN_TxPack_J1939_SendOnly;
		Send_pass_flash=NULL;
		break;
	case 0x85:
		Read_pass = PassThru_CAN_RxPack_J1939;
		init_pass = PassThru_CAN_TxPack_J1939;
		Send_pass = PassThru_CAN_TxPack_J1939;
		Send_pass_flash=NULL;
		break;
	case 0x86:
		PassThruConfig.CtrlMode = 0x03;//采样模式 返回完整帧 对应82模式
		Read_pass = PassThru_CAN_RxPack_Auto_ReadOnly;
		init_pass = PassThru_CAN_TxPack_Auto;
		Send_pass = PassThru_CAN_TxPack_Auto;
		Send_pass_flash=NULL;
		break;
	case 0x87:
		PassThruConfig.CtrlMode = 0x03;//采样模式 返回完整帧 对应83模式
		Read_pass = PassThru_CAN_RxPack_AutoExt_ReadOnly;
		init_pass = PassThru_CAN_TxPack_Auto_Ext;
		Send_pass = PassThru_CAN_TxPack_Auto_Ext;
		Send_pass_flash=NULL;
		break;
	case 0x88:
		PassThruConfig.CtrlMode = 0x00;//诊断模式 返回完整帧 对应82模式
		if(PassThruConfig.CanFill == 0) PassThruConfig.CanFill = 0x0100;//默认补0
		Read_pass = PassThru_CAN_RxPack_Auto_ReadOnly;
		init_pass = PassThru_CAN_TxPack_Auto;
		Send_pass = PassThru_CAN_TxPack_Auto;
		Send_pass_flash=NULL;
		break;
	case 0x89:
		PassThruConfig.CtrlMode = 0x00;//诊断模式 返回完整帧 对应83模式
		if(PassThruConfig.CanFill == 0) PassThruConfig.CanFill = 0x0100;//默认补0
		Read_pass = PassThru_CAN_RxPack_AutoExt_ReadOnly;
		init_pass = PassThru_CAN_TxPack_Auto_Ext;
		Send_pass = PassThru_CAN_TxPack_Auto_Ext;
		Send_pass_flash=NULL;
		break;
	case 0x8A:
		Read_pass = PassThru_CAN_RxPack_Head;
		init_pass = PassThru_CAN_TxPack_Head;
		Send_pass = PassThru_CAN_TxPack_Head;
		Send_pass_flash=NULL;
		break;
	case 0x8B:
		CanTransMode = 0x20;
		Read_pass = PassThru_CAN_RxPack_J1939_ReadPGN;
		init_pass = PassThru_CAN_TxPack_J1939_SendPGN;
		Send_pass = PassThru_CAN_TxPack_J1939_SendPGN;
		Send_pass_flash=NULL;
		break;
   case 0x8C:
		if(PassThruConfig.CanFill == 0) PassThruConfig.CanFill = 0x0100;//默认补0
		Read_pass = PassThru_CAN_RxPack_Auto_GM;
		init_pass    = PassThru_CAN_TxPack_Auto_GM;
		Send_pass = PassThru_CAN_TxPack_Auto_GM;
		Send_pass_flash=NULL;
		break;
	case 0x8d:// 同0x7f,只是发送时发送同样的命令，后面带发送次数，多少帧打包一次回来
		if(PassThruConfig.CanFill == 0) PassThruConfig.CanFill = 0x0100;//默认补0
		Send_pass =PassThru_CAN_TxPack_flash_read;
		Read_pass = PassThru_CAN_RxPack_NotID;
		init_pass     = PassThru_CAN_TxPack_flash_read;
		Send_pass_flash=NULL;
		break;
	case 0x8e:// 同0x7f,只是发送时发送同样的命令，后面带发送次数，多少帧打包一次回来
		if(PassThruConfig.CanFill == 0) PassThruConfig.CanFill = 0x0100;//默认补0
		Send_pass =PassThru_CAN_TxPack_flash_read_pos;
		Read_pass = PassThru_CAN_RxPack_NotID;
		init_pass     = PassThru_CAN_TxPack_flash_read;
		Send_pass_flash=NULL;
		break;

	case 0x8F:// 同0x8f,只是发送时发送同样的命令，后面带发送次数，多少帧打包一次回来
		Read_pass = PassThru_CAN_RxPack_Normal_Multiple;
		init_pass = PassThru_CAN_TxPack_Normal_Multiple;
		Send_pass = PassThru_CAN_TxPack_Normal_Multiple;
		Send_pass_flash=NULL;
  
	default:
		break;
	}
	if(type1==0)
	{
		PassThruDriver.ReadBusFunc=Read_pass;
		PassThruDriver.InitEcuFunc=init_pass;
		PassThruDriver.TxDataToEcuFunc=Send_pass;
		PassThruDriver.TxFlashDataToEcuFunc=Send_pass_flash;
	}
// 	else
// 	{
// 		PassThruDriver.ReadBusFunc_TWO=Read_pass;
// 		PassThruDriver.InitEcuFunc_TWO=init_pass;
// 		PassThruDriver.TxDataToEcuFunc_TWO=Send_pass;
// 		PassThruDriver.TxFlashDataToEcuFunc_TWO=Send_pass_flash;
// 	}
	return TRUE;
}


/*********************************************************************************************
* 名称    ： PassThru_CAN_SetConfig
* 功能    ： CANBUS协议解析初始化IO参数
* 入口参数：
*
*
* 出口参数：
* 说明    ：
*
***********************************************************************************************/
uint8 PassThru_CAN_SetConfig(uint8 *Pack, uint8 CHANEL) 
{
	uint8 temp8;
	uint8 temp0, temp1, temp2, temp3;
	//uint32 temp_baud;
	uint16 PackLen;
	uint8 *PackEnd;
	// uint8 func1,func2,func3;
	PackLen = (Pack[0] << 8) + Pack[1];
	PackEnd = Pack + PackLen + 2;
	Pack = Pack + 3;
	//NVIC_DisableIRQ(CAN_IRQn);
	
	//initDoubleBuf();
	//printf("PassThru_CAN_SetConfig\r\n");
/*
 *********************************************
 * chenli_patch 1.2 start
 * 修复crc字节存储位置错误引起的CAN8D协议读euc flash失败
 * 测试方法:F7S蓝牙连接minivci->诊断->ECU刷写->博士->EDC17CV44/54刷写(CAN线)->
 *	->潍柴BOSCH->1M->读取
 *
 */	
 
	Reset_All_IO();
	
/*chenli_patch 1.2 end
 *********************************************	  
 */	
	
	Queue_Init(&CAN1_QueueData);
	Queue_Init(&CAN2_QueueData);
	SoftFilterMask = 0;
	CAN_ID_FILT_FLAG = 0;        //CAN ID
	//SET_60RES;//默认打开can电阻
	for (; ;) {
		temp8 = *Pack++;
		if (Pack >= PackEnd)
			break;
		//return TRUE;

		switch (temp8) {
		case 0x01:
			temp0 = *Pack++;
			temp1 = *Pack++;
			temp2 = *Pack++;
			PassThruConfig.BaudRate = (temp0 << 16) + (temp1 << 8) + temp2;
			break;

		case 0x02:
			PassThruConfig.Listen = *Pack;
			PassThru_SET_EACH_FUN(*Pack++,PassThruConfig.Send_Pass);
			//	PassThru_SET_EACH_FUN(*Pack++,&func1,&func2,&func3);

			//							if(PassThruConfig.Send_Pass==0)
			//							{
			//								PassThruDriver.ReadBusFunc=&func1;
			//								PassThruDriver.InitEcuFunc=&func2;
			//								PassThruDriver.TxDataToEcuFunc=&func3;
			//							}
			//							else
			//							{
			//								PassThruDriver.ReadBusFunc_TWO=&func1;
			//								PassThruDriver.InitEcuFunc_TWO=&func2;
			//								PassThruDriver.TxDataToEcuFunc_TWO=&func3;
			//							}
			//                if (PassThruConfig.Send_Pass == 0)
			//									  PassThru_SET_EACH_FUN(*Pack++,PassThruDriver.ReadBusFunc,ThruDriver.InitEcuFunc,PassThruDriver.TxDataToEcuFunc);
			//                else
			//                    PassThru_SET_EACH_FUN(*Pack++,PassThruDriver.ReadBusFunc_TWO,PassThruDriver.InitEcuFunc_TWO,PassThruDriver.TxDataToEcuFunc_TWO);
			break;

		case 0x03:
			PassThruConfig.Format = *Pack;
			FrameFormat[PassThruConfig.Send_Pass]=*Pack++;
			switch(FrameFormat[PassThruConfig.Send_Pass]&0x60)
			{
			case 0x20:
			case 0x40:
				Can_Send_Mode[PassThruConfig.Send_Pass]=1<<4;//0x10;//自发自收模式
				break;
			default:
				Can_Send_Mode[PassThruConfig.Send_Pass]=0x03;//正常模式
			}
			switch(FrameFormat[PassThruConfig.Send_Pass]&0x06)
			{
			case 0x00:
				FIRST_HIGH_CAN_RX;
				FIRST_HIGH_CAN_TX;
				SetCChannel(CAN_BUS_CHANNEL);
				break;
			case 0x02:
				FIRST_SINGLE_CAN_RX;
				FIRST_SINGLE_CAN_TX;
				SetVcc_Drv(DRIVE_5V);
				SetCompV(0x25);
				SetCChannel(KL_BUS_CHANNEL);
				//SetCChannel(KL_BUS_CHANNEL);
				break;
			}
			CANMODE=PassThruConfig.Format&0x80;//0x80:can侦听模式;0x00:正常接收发送模式
			//FrameFormat[PassThruConfig.Send_Pass]=PassThruConfig.Format&0xff;//(PassThruConfig.Format>>4)&0x01;
			//PassThru_CAN_SetCan_Format(*Pack++);
			break;

		case 0x04:
			temp0 = *Pack++;
			temp1 = *Pack++;
			temp2 = *Pack++;
			temp3 = *Pack++;
			PassThruConfig.CanTD = (temp0 << 24) + (temp1 << 16) + (temp2 << 8) + temp3; //+4/

			PassThruConfig.CanLastTD[PassThruConfig.Send_Pass] = PassThruConfig.CanTD;
			break;

		case 0x05:
			temp0 = *Pack++;
			temp1 = *Pack++;
			temp2 = *Pack++;
			temp3 = *Pack++;
			PassThruConfig.CanAR1 = (temp0 << 24) + (temp1 << 16) + (temp2 << 8) + temp3; //+4/
			break;

		case 0x06:
			temp0 = *Pack++;
			temp1 = *Pack++;
			temp2 = *Pack++;
			temp3 = *Pack++;
			PassThruConfig.CanMR1 = (temp0 << 24) + (temp1 << 16) + (temp2 << 8) + temp3; //+4/
			break;

		case 0x07:
			temp0 = *Pack++;
			temp1 = *Pack++;
			temp2 = *Pack++;
			temp3 = *Pack++;
			PassThruConfig.CanAR2 = (temp0 << 24) + (temp1 << 16) + (temp2 << 8) + temp3; //+4/
			break;

		case 0x08:
			temp0 = *Pack++;
			temp1 = *Pack++;
			temp2 = *Pack++;
			temp3 = *Pack++;
			PassThruConfig.CanMR2 = (temp0 << 24) + (temp1 << 16) + (temp2 << 8) + temp3; //+4/
			break;

		case 0x09:
			PassThruConfig.ISO15765BS[PassThruConfig.Send_Pass] = *Pack++;
			PassThruConfig.ISO15765STmin[PassThruConfig.Send_Pass] = *Pack++;
			break;

		case 0x0a:
			temp0 = *Pack++;
			temp1 = *Pack++;
			PassThruConfig.Keep[PassThruConfig.Send_Pass] = (temp0 << 8) + temp1;
			break;


		case 0x0b://不用，兼容以前，
			temp0 = *Pack++;
			temp1 = *Pack++;
			PassThruConfig.CanIdauto = (temp0 << 8) + temp1; //+2
			break;

		case 0x0c:
			temp0 = *Pack++;
		     
			 
			if (PassThruConfig.Send_Pass == 0)//can0设置
			{

				PassThruConfig.Pin1 = temp0 & 0x0f;
				PassThruConfig.Pin2 = temp0 >> 4;
				// SetLine0Channel(PassThruConfig.Pin1); //can+通道只能由SetLine0Channel来设置
				// SetLine1Channel(PassThruConfig.Pin2); //can-通道只能由SetLine1Channel来设置  cyl 2015 7 13日，所有的其它参数设置好再接引脚，不然会引起其它错误
				SetLine_ext(PassThruConfig.Pin1,PassThruConfig.Pin2);
			}
			break;

		case 0x0d://兼容以前，
			PassThruConfig.CanAnsT = *Pack++;
			break;

		case 0x0e://兼容以前，
			for (temp8 = 0; temp8 < 16; temp8++) {
				PassThruConfig.SendBuf[temp8] = *Pack++;
			}
			PassThruConfig.CanAnsR = PassThruConfig.SendBuf[0];
			break;

		case 0x0f:
			PassThruConfig.CanMidTime[PassThruConfig.Send_Pass] = *Pack++;//增加多帧时间控制
// 			if(PassThruConfig.CanMidTime[PassThruConfig.Send_Pass]==0)// 如果设置成不需等待时间
// 				PassThruConfig.CanMidTime[PassThruConfig.Send_Pass]=0xf5;
			break;
		case 0x10:
			PassThruConfig.CAN_RES = *Pack++;  ////10   是否加电阻   0 加 1 不加   ADD BY YWP
			// if (PassThruConfig.Send_Pass == 0) {
			//}
			break;

		case 0x11:
			PassThruConfig.TxAddress = *Pack++;
			PassThruConfig.RxAddress = *Pack++;
			break;

		case 0x12:
			temp0 = *Pack++;
			temp1 = *Pack++;
			PassThruConfig.CanFill = (temp0 << 8) + temp1;
			break;

		case 0x13:
			////CAN_SetACCF_sff(Pack);//设置一个过滤标准帧id
			temp0 = *Pack++;
			Pack += 2 * (temp0 & 0x7f);
			break;

		case 0x14:
			////CAN_SetACCF_sff_Group(Pack);//设置一组过滤标准帧范围
			temp0 = *Pack++;
			Pack += 4 * (temp0 & 0x7f);
			break;

		case 0x15:
			//CAN_SetACCF_eff(Pack);//设置一个过滤扩展帧id
			temp0 = *Pack++;
			Pack += 4 * (temp0 & 0x7f);
			break;

		case 0x16:
			//CAN_SetACCF_eff_Group(Pack);//设置一个过滤扩展帧id范围
			temp0 = *Pack++;
			Pack += 8 * (temp0 & 0x7f);
			break;
		case 0x17:
			//							PassThruConfig.CanRecvTimeOut[0] = *Pack++;//增加一个canbus 0x30接收超时时间设定;
			//							PassThruConfig.CanRecvTimeOut[1] = *Pack++;
			temp0 = *Pack++;
			temp1 = *Pack++;
			PassThruConfig.CanRecvTimeOut=(temp0<<8)+temp1;
			break;
		default:
			return FALSE;
		}
	}
	PassThruDriver.TxDataDelayFunc = NULL;
	PassThruDriver.KeepComFunc = NULL;
	PassThruDriver.FuncBak0 = NULL;
	KeepComTime[PassThruConfig.Send_Pass] = (PassThruConfig.Keep[PassThruConfig.Send_Pass] & 0x7fff);
	if (KeepComTime[PassThruConfig.Send_Pass] == 0) KeepComTime[PassThruConfig.Send_Pass] = 10;

	// ErrStatus = CAN_NO_ERR;
	//NVIC_EnableIRQ(CAN_IRQn);          /* 增加CAN 接收中断 BY YWP*/
	return TRUE;
}

/*********************************************************************************************
* 名称    ： PassThru_CAN_InitPort
* 功能    ： CANBUS协议初始化IO
* 入口参数：
*
*
* 出口参数：
* 说明    ：
*
***********************************************************************************************/
uint8 PassThru_CAN_InitPort(uint8 CHANEL) 
{
	//unsigned char c;
	//uint32 temp1, temp2, read, mask;
	// uint8 Mode;
      
	uint32 CanBaud;

	CanId_RxNum = 0;                     //记忆ID
	CAN_ID_FILT_FLAG = 0;        //CAN ID 软件过滤标志位清零
       
	//10   是否加电阻   0 加 1 不加   ADD BY YWP
	if (PassThruConfig.CAN_RES == 0)
	{
		SET_60RES;
	}
	else
	{
		CLR_60RES;
	}

	//设置波特率
	CanBaud = PassThruConfig.BaudRate;
	if(CanBaud & 0x00800000)
	{
		//F3下位机设置参数 通过设置寄存器来设置波特率 SJ1000_BPS_1000K
		if (CanBaud == SJ1000_BPS_10K) CanBaud = BPS_10K;
		else if (CanBaud == SJ1000_BPS_20K) CanBaud = BPS_20K;
		else if (CanBaud == SJ1000_BPS_33K) CanBaud = BPS_33K;
		else if (CanBaud == SJ1000_BPS_50K) CanBaud = BPS_50K;
		else if (CanBaud == SJ1000_BPS_96K) CanBaud = BPS_96K;
		else if (CanBaud == SJ1000_BPS_100K) CanBaud = BPS_100K;
		else if (CanBaud == SJ1000_BPS_125K) CanBaud = BPS_125K; //125k
		else if (CanBaud == SJ1000_BPS_250K) CanBaud = BPS_250K; //250k
		else if (CanBaud == SJ1000_BPS_500K) CanBaud = BPS_500K; //500k
		else if (CanBaud == SJ1000_BPS_800K) CanBaud = BPS_800K;
		else if (CanBaud == SJ1000_BPS_1000K) CanBaud = BPS_1000K;
		else return FALSE;
	}
	PassThruConfig.BaudRate = CanBaud;
	if( (PassThruConfig.Pin1==6)||(PassThruConfig.Pin1==14)||(PassThruConfig.Pin1==11)||(PassThruConfig.Pin1==3) )
	CANx_ResetBaudRate(CAN2, FrameFormat[PassThruConfig.Send_Pass]);
	else 
       CANx_ResetBaudRate(CAN1, FrameFormat[PassThruConfig.Send_Pass]);
	//进入复位模式以便设置各参数

	switch ((FrameFormat[PassThruConfig.Send_Pass] >> 4) & 0x01) {
	case 0: //标准帧
		PassThruConfig.CanMR1 &= 0x7ff;
		PassThruConfig.CanAR1 &= 0x7ff;
		PassThruConfig.CanAR2 &= 0x7ff;
		PassThruConfig.CanMR2 &= 0x7ff;
		break;
	case 1:
		SoftFilterMask = 1;//扩展帧双滤波需要软件过滤
		PassThruConfig.CanMR1 &= 0x1Fffffff;
		PassThruConfig.CanAR1 &= 0x1Fffffff;
		PassThruConfig.CanMR2 &= 0x1ffffFff;
		PassThruConfig.CanAR2 &= 0x1Fffffff;
		break;
	}
if( (PassThruConfig.Pin1==6)||(PassThruConfig.Pin1==14)||(PassThruConfig.Pin1==11)||(PassThruConfig.Pin1==3) )
	CANx_ResetFilterMask(CAN2, FrameFormat[PassThruConfig.Send_Pass]);
else
	CANx_ResetFilterMask(CAN1, FrameFormat[PassThruConfig.Send_Pass]);
	return TRUE;
}

/*********************************************************************************************
* 名称    ： PassThru_CAN_RxData
* 功能    ： CANBUS协议接收一帧数据
* 入口参数：
*
*
* 出口参数：
* 说明    ：
*
***********************************************************************************************/
uint8 PassThru_CAN_RxData(uint8 *RXdata)
{
	uint32 CanID;
	uint8_t i, j;
	// uint8_t Buf[60];
	CanRxMsg RxMessage;
	IWDG_Feed();//此处加个喂狗，防止收多帧时重启
	if(!isHasData()) return FALSE;

	Queue_Read(&CAN1_QueueData, &RxMessage);
      Queue_Read(&CAN2_QueueData, &RxMessage);

	//memset(Buf,0x00,60);  // 清空

	i = 0;
	*RXdata++=0X00;//Buf[i++] = 0x00;
	*RXdata++=RxMessage.DLC+5;//Buf[i++] = RxMessage.DLC+5;
	*RXdata++=CAN_DATA;//Buf[i++] = CAN_DATA;
	if(RxMessage.IDE==CAN_ID_STD) // 标准帧
	{
		CanID=RxMessage.StdId;
		*RXdata++=(RxMessage.StdId>>24)&0xff;//Buf[i++] = (RxMessage.StdId>>24)&0xff;
		*RXdata++=(RxMessage.StdId>>16)&0xff;//Buf[i++] = (RxMessage.StdId>>16)&0xff;
		*RXdata++=(RxMessage.StdId>>8)&0xff;//Buf[i++] = (RxMessage.StdId>>8)&0xff;
		*RXdata++=(RxMessage.StdId)&0xff;//Buf[i++] = (RxMessage.StdId)&0xff;
		for(j=0; j<RxMessage.DLC; j++)
		{
			*RXdata++=RxMessage.Data[j];//Buf[i++] = RxMessage.Data[j];
		}
	}
	else // 扩展帧
	{
		CanID=RxMessage.ExtId|EFF_FLAG;  //最高位为1表示扩展帧
		*RXdata++=(RxMessage.ExtId>>24)&0xff | 0x80;//Buf[i++] = (RxMessage.ExtId>>24)&0xff | 0x80;
		*RXdata++=(RxMessage.ExtId>>16)&0xff;//Buf[i++] = (RxMessage.ExtId>>16)&0xff;
		*RXdata++=(RxMessage.ExtId>>8)&0xff;//Buf[i++] = (RxMessage.ExtId>>8)&0xff;
		*RXdata++=(RxMessage.ExtId)&0xff;//Buf[i++] = (RxMessage.ExtId)&0xff;
		for(j=0; j<RxMessage.DLC; j++)
		{
			*RXdata++=RxMessage.Data[j];// Buf[i++] = RxMessage.Data[j];
		}
	}
	//读数据
	//     len = i;
	//     for (i = 0; i < len; i++) {
	//         *RXdata++ = Buf[i];
	// }
	//硬件直接实现双CANID过滤，不需要软件过滤
	//     if(SoftFilterMask>0)
	// 		{
	//         if((PassThru_CAN_FilterID(CanID&0x1FFFFFFF))==FALSE)
	//         return FALSE;
	// 		}
	if(CAN_ID_FILT_FLAG == 1)	//CAN ID 软件过滤
	{
		if((CanID & 0x60000000) == 0x0)
		{
			for(i=0; i<CanId_RxNum; i++)
			{
				if((CanId_RxBuf[i] & 0x03ffffff) == (CanID & 0x03ffffff))//扩展帧优先级无关
					return TRUE;
			}
		}
		return FALSE;
	}
	return TRUE;
}


/*********************************************************************************************
* 名称    ： PassThru_CAN_RxData_Time
* 功能    ： CANBUS协议接收一帧数据
* 入口参数：
*
*
* 出口参数：
* 说明    ：等待超时50ms
* bug: 20160429 by wangs 赵书皓发现在刷写edc7 ecu时会发生tool发多针 ecu回复30流控制帧时间超过400ms的情况;
*						需要增加接收超时时间;
***********************************************************************************************/
uint8 PassThru_CAN_RxData_Time(uint8 *RXdata) 
{

	#if 1
	//等待50ms
	T_delay_ms_set(PassThruConfig.CanRecvTimeOut);

	for (; ;) 
	{
		if (T_delay_ms_get_status() == SET) 
		{
			//ErrStatus = CAN_TIME_OUT;
			return FALSE;   //防止死循环
		}
		if (isHasData()) 
		{
			if (PassThru_CAN_RxData((uint8 *) RXdata) == TRUE) //正常7.8us，有数据时96us
			{
				return TRUE;
			}
		}
	}
	#endif
}


/*********************************************************************************************
* 名称    ： PassThru_CAN_RxPack_NotID
* 功能    ： CANBUS协议接收一帧数据 去掉ID
* 入口参数：
*
*
* 出口参数：
* 说明    ：0x7F 多帧数据自动回复控制帧 不带ID
*
***********************************************************************************************/
uint8 PassThru_CAN_RxPack_NotID() {
	uint16 Frame_Length;//多帧数据帧长度
	uint16 Frame_Count0;//多帧数据长度
	uint16 Frame_Count1;//单次连续多帧计数
	uint16 Frame_Count2;//全部连续多帧计数
	uint8 Frame_FlagSN;//连续帧标志位
	uint8 Frame_Control = 0x03;//0x07;//每帧数据控制位位置,changyl,f6程序原来是直接把发送数据发送出去,没有对收到的数据处理,minivci减四位(id)处理,所以此处要减四
	uint8 Frame_DataNum = 0x07;//0x07;//每帧数据有效数据个数

	if (PassThruConfig.CanAnsR == 0x01) {
		Frame_Control = 0x03;
	}
	if (PassThruConfig.CanAnsR == 0x02) {
		Frame_Control = 0x04;
		Frame_DataNum = 0x06;//每帧数据有效数据个数
	}
	if (PassThru_CAN_RxData((uint8 *) EcuComRxBuf) == TRUE) {
		PassThru_SetKeepComSwitch2(EcuComRxBuf + Frame_Control + 1);
		UartTxQueuePack_DelCanID(EcuComRxBuf);
	}
	else {
		return FALSE;
	}
	if (PassThruConfig.CanAnsR == 0x00) {
		return TRUE;
	}
	//接收多帧数据
	if ((EcuComRxBuf[Frame_Control] & 0xf0) == CAN_15765_FF && PassThruConfig.CanAnsR > 0x00) {
		Frame_Count0 = ((EcuComRxBuf[Frame_Control] & 0x0f) << 8) + EcuComRxBuf[Frame_Control + 1];
		Frame_FlagSN = 1;
		Frame_Count1 = 0;
		Frame_Count2 = 0;
		Frame_Length = Frame_Count0 / Frame_DataNum;//流控制 连续帧 帧数
		while (Frame_Count2 < Frame_Length) {
			if (Frame_Count1 == 0x00 || Frame_Count1 == PassThruConfig.ISO15765BS[PassThruConfig.Send_Pass]) {
				Frame_Count1 = 0;
				//发送控制帧
				if (!PassThru_CAN_TxPack_FlowControl((uint8 *) EcuComRxBuf)) {
					return FALSE;
				}
			}
			//接收连续数据 按照实际接收
			if (PassThru_CAN_RxData_Time((uint8 *) EcuComRxBuf) == FALSE) {
				return FALSE;
			}
			UartTxQueuePack_DelCanID(EcuComRxBuf);
			if ((EcuComRxBuf[Frame_Control] & 0xf0) != CAN_15765_CF) {
				return FALSE;
			}
			if ((EcuComRxBuf[Frame_Control] & 0x0f) != Frame_FlagSN) {
				return FALSE;
			}
			Frame_Count1++;
			Frame_Count2++;
			Frame_FlagSN++;
			if (Frame_FlagSN > 0x0f) {
				Frame_FlagSN = 0x00;
			}
		}
	}
	return TRUE;
}

/*********************************************************************************************
* 名称    ： PassThru_CAN_RxPack_GetID
* 功能    ： CANBUS协议获取总线CANID
* 入口参数：
*
*
* 出口参数：
* 说明    ：0x80 返回完整帧 带ID
*
***********************************************************************************************/
uint8 PassThru_CAN_RxPack_GetID()
{
	uint32 loop;
	uint32 CanId_Code;

	if(PassThru_CAN_RxData((uint8 *)EcuComRxBuf) == TRUE)
	{
		CanId_Code = (EcuComRxBuf[3]<<24) + (EcuComRxBuf[4]<<16) + (EcuComRxBuf[5]<<8) + EcuComRxBuf[6];
		for(loop=0; loop<CanId_RxNum; loop++)
		{
			if(CanId_Code == CanId_RxBuf[loop])
				return TRUE;
		}
		if(loop == CanId_RxNum)
		{
			CanId_RxBuf[loop] = CanId_Code;
			CanId_RxNum++;
			UartTxQueuePack(EcuComRxBuf);
		}
		return TRUE;
	}
	return FALSE;
}

/*********************************************************************************************
* 名称    ： PassThru_CAN_RxPack_Normal
* 功能    ： CANBUS协议接受数据 正常模式
* 入口参数：
*
*
* 出口参数：
* 说明    ：0x81 返回完整帧 带ID
*
***********************************************************************************************/
uint8 PassThru_CAN_RxPack_Normal()
{
	uint16 Frame_Length;//多帧数据帧长度
	uint16 Frame_Count0;//多帧数据长度
	uint16 Frame_Count1;//单次连续多帧计数
	uint16 Frame_Count2;//全部连续多帧计数
	uint8 Frame_FlagSN;//连续帧标志位
	uint8 Frame_Control = 0x07;//每帧数据控制位位置
	uint8 Frame_DataNum = 0x07;//每帧数据有效数据个数

	if (PassThruConfig.CanAnsT == 0x01)
	{
		Frame_Control = 0x07;
	}
	if (PassThruConfig.CanAnsT == 0x02)
	{
		Frame_Control = 0x08;
	}
	if (PassThru_CAN_RxData((uint8 *) EcuComRxBuf) == TRUE) {
		PassThru_SetKeepComSwitch2(EcuComRxBuf + Frame_Control + 1);
		UartTxQueuePack(EcuComRxBuf);
	}
	else
	{
		return FALSE;
	}
	if (PassThruConfig.CanAnsT == 0x00)
	{
		return TRUE;
	}
	//接收多帧数据 add by wangs 20160418 仿照之前程序添加， 没有严格测试， 只是实现了 0x30自动回复， 请谨慎使用
	// PassThruConfig.CanAnsR
	if ((EcuComRxBuf[Frame_Control] & 0xf0) == CAN_15765_FF && PassThruConfig.CanAnsT > 0x00) {
		Frame_Count0 = ((EcuComRxBuf[Frame_Control] & 0x0f) << 8) + EcuComRxBuf[Frame_Control + 1];
		Frame_FlagSN = 1;
		Frame_Count1 = 0;
		Frame_Count2 = 0;
		Frame_Length = Frame_Count0 / Frame_DataNum;//流控制 连续帧 帧数 除号是取整
		while (Frame_Count2 < Frame_Length)
		{
			if (Frame_Count1 == 0x00 || Frame_Count1 == (uint16) PassThruConfig.ISO15765BS[0])
			{
				Frame_Count1 = 0;
				//发送控制帧
				if (!PassThru_CAN_TxPack_FlowControl((uint8 *) EcuComRxBuf))
				{
					return FALSE;
				}
			}
			//接收连续数据 按照实际接收
			if (PassThru_CAN_RxData_Time((uint8 *) EcuComRxBuf) == FALSE)
			{
				return FALSE;
			}
			UartTxQueuePack(EcuComRxBuf);
			if ((EcuComRxBuf[Frame_Control] & 0xf0) != CAN_15765_CF)
			{
				return FALSE;
			}
			if ((EcuComRxBuf[Frame_Control] & 0x0f) != Frame_FlagSN)
			{
				return FALSE;
			}
			Frame_Count1++;
			Frame_Count2++;
			Frame_FlagSN++;
			if (Frame_FlagSN > 0x0f) {
				Frame_FlagSN = 0x00;
			}
		}
	}
	return TRUE;
}

void UartTxQueuePackDebug(uint8* EcuComRxBuf);
uint8 PassThru_CAN_RxPack_Normal_Multiple()
{
	uint16 Frame_Length;//多帧数据帧长度
	uint16 Frame_Count0;//多帧数据长度
	uint16 Frame_Count1;//单次连续多帧计数
	uint16 Frame_Count2;//全部连续多帧计数
	uint8 Frame_FlagSN;//连续帧标志位
	uint8 Frame_Control = 0x07;//每帧数据控制位位置
	uint8 Frame_DataNum = 0x07;//每帧数据有效数据个数
	uint8 normaldata[4096];
	memset(normaldata,0,sizeof(normaldata));
   	if (PassThruConfig.CanAnsT == 0x00)
	{
		return TRUE;
	} 
    

	if (PassThruConfig.CanAnsT == 0x01)
	{
		Frame_Control = 0x07;
	}
	if (PassThruConfig.CanAnsT == 0x02)
	{
		Frame_Control = 0x08;
	}
	if (PassThru_CAN_RxData((uint8 *) EcuComRxBuf) == TRUE) 
    {
		PassThru_SetKeepComSwitch2(EcuComRxBuf + Frame_Control + 1);
        
	    //UartTxQueuePackMultiple(EcuComRxBuf);	
	 
			
#if 0
			normaldata[0]=0xa5;
			normaldata[1]=0xa5; //head

			normaldata[2]=0x00;
			normaldata[3]=0x08; //len

			normaldata[4]=0x54;

			normaldata[5]=PassThruConfig.CanAnsT;
			normaldata[6]=EcuComRxBuf[Frame_Control] & 0xf0;
			UartTxQueuePackDebug(normaldata);
#endif
			
			
	}
	else
	{
		//UartTxQueuePackMultiple(EcuComRxBuf);
		return FALSE;
	}
	if (PassThruConfig.CanAnsT == 0x00)
	{
		return TRUE;
	}
	

	
	//接收多帧数据 add by wangs 20160418 仿照之前程序添加， 没有严格测试， 只是实现了 0x30自动回复， 请谨慎使用
	// PassThruConfig.CanAnsR
	if ((EcuComRxBuf[Frame_Control] & 0xf0) == CAN_15765_FF && PassThruConfig.CanAnsT > 0x00) 
    {
		Frame_Count0 = ((EcuComRxBuf[Frame_Control] & 0x0f) << 8) + EcuComRxBuf[Frame_Control + 1];
		Frame_FlagSN = 1;
		Frame_Count1 = 0;
		Frame_Count2 = 0;
		Frame_Length = Frame_Count0 / Frame_DataNum;//流控制 连续帧 帧数 除号是取整
		while (Frame_Count2 < Frame_Length)
		{
			if (Frame_Count1 == 0x00 || Frame_Count1 == (uint16) PassThruConfig.ISO15765BS[0])
			{
				Frame_Count1 = 0;
				//发送控制帧
				if (!PassThru_CAN_TxPack_FlowControl((uint8 *) EcuComRxBuf))
				{
					return FALSE;
				}
			}
			//接收连续数据 按照实际接收
			if (PassThru_CAN_RxData_Time((uint8 *) EcuComRxBuf) == FALSE)
			{
				return FALSE;
			}
            
			//UartTxQueuePackMultiple(EcuComRxBuf);
        	//	UartTxQueuePackEcuComRxBuf);    
			if ((EcuComRxBuf[Frame_Control] & 0xf0) != CAN_15765_CF)
			{
				return FALSE;
			}
			if ((EcuComRxBuf[Frame_Control] & 0x0f) != Frame_FlagSN)
			{
				return FALSE;
			}
			Frame_Count1++;
			Frame_Count2++;
			Frame_FlagSN++;
			if (Frame_FlagSN > 0x0f) 
            {
				Frame_FlagSN = 0x00;
			}
		}
	}
	
	else
	{
					
		 
	}
	return TRUE;


}


/*********************************************************************************************
* 名称    ： PassThru_CAN_RxPack_Auto
* 功能    ： 接收数据 自动回复控制帧
* 入口参数：
*
*
* 出口参数：
* 说明    ：0x82 多帧数据自动回复控制帧 带ID
*
***********************************************************************************************/
uint8 PassThru_CAN_RxPack_Auto()
{
	uint16 Frame_Length;//多帧数据帧长度
	uint16 Frame_Count0;//多帧数据长度
	uint16 Frame_Count1;//单次连续多帧计数
	uint16 Frame_Count2;//全部连续多帧计数
	uint8  Frame_FlagSN;//连续帧标志位
	uint8  Frame_Control = 0x07;//每帧数据控制位位置
	uint8  Frame_DataNum = 0x07;//每帧数据有效数据个数

	if(PassThru_CAN_RxData((uint8 *)EcuComRxBuf) == TRUE)
	{
		PassThru_SetKeepComSwitch2(EcuComRxBuf+Frame_Control+1);
		UartTxQueuePack(EcuComRxBuf);
	}
	else
	{
		return FALSE;
	}
	//接收多帧数据
	if((EcuComRxBuf[Frame_Control]&0xf0) == CAN_15765_FF)// && PassThruConfig.CanAnsR == 0x01)// PassThruConfig.CanAnst == 0x01)
	{
		Frame_Count0 = ((EcuComRxBuf[Frame_Control]&0x0f)<<8) + EcuComRxBuf[Frame_Control+1];
		Frame_FlagSN = 1;
		Frame_Count1 = 0;
		Frame_Count2 = 0;
		Frame_Length = Frame_Count0 / Frame_DataNum;//流控制 连续帧 帧数
		while (Frame_Count2 < Frame_Length)
		{
			if (Frame_Count1 == 0x00 || Frame_Count1 == PassThruConfig.ISO15765BS[PassThruConfig.Send_Pass])
			{
				Frame_Count1 = 0;
				//发送控制帧
				if (!PassThru_CAN_TxPack_FlowControl((uint8 *) EcuComRxBuf))
				{
					return FALSE;
				}
			}
			//接收连续数据 按照实际接收
			if (PassThru_CAN_RxData_Time((uint8 *) EcuComRxBuf) == FALSE)
			{
				return FALSE;
			}
			UartTxQueuePack(EcuComRxBuf);
			if ((EcuComRxBuf[Frame_Control] & 0xf0) != CAN_15765_CF)
			{
				return FALSE;
			}
			if ((EcuComRxBuf[Frame_Control] & 0x0f) != Frame_FlagSN)
			{
				return FALSE;
			}
			Frame_Count1++;
			Frame_Count2++;
			Frame_FlagSN++;
			if (Frame_FlagSN > 0x0f)
			{
				Frame_FlagSN = 0x00;
			}
		}
	}
	return TRUE;
}
/*********************************************************************************************
* 名称    ： PassThru_CAN_RxPack_Auto_ReadOnly
* 功能    ： 接收数据 自动回复控制帧
* 入口参数：
*
*
* 出口参数：
* 说明    ：0x86 ISO15765 多帧数据采样，自动处理控制帧 带ID
*
***********************************************************************************************/
uint8 PassThru_CAN_RxPack_Auto_ReadOnly()
{
	uint16 Frame_Length;//多帧数据帧长度
	uint16 Frame_Count0;//多帧数据长度
	uint16 Frame_Count1;//单次连续多帧计数
	uint16 Frame_Count2;//全部连续多帧计数
	uint8  Frame_FlagSN;//连续帧标志位
	uint8  Frame_Control = 0x07;//每帧数据控制位位置
	uint8  Frame_DataNum = 0x07;//每帧数据有效数据个数
	uint16 ByteNum, count, number;
	uint16 i, nCount;
	uint8  checknum = 0;

	if(PassThru_CAN_RxData((uint8 *)EcuComRxBuf) == TRUE)
	{
		PassThru_SetKeepComSwitch2(EcuComRxBuf+Frame_Control+1);
	}
	else
	{
		return FALSE;
	}
	//接收多帧数据
	if((EcuComRxBuf[Frame_Control]&0xf0) == CAN_15765_FF)// && PassThruConfig.CanAnsR == 0x01)// PassThruConfig.CanAnst == 0x01)
	{
		Frame_Count0 = ((EcuComRxBuf[Frame_Control]&0x0f)<<8) + EcuComRxBuf[Frame_Control+1];
		count = 0;
		ByteNum = 0;
		nCount = Frame_Count0 + 7;
		USART1_ClearQueue();
		USART_PushByte(0xA5);
		USART_PushByte(0xA5);
		EcuComRxBuf[ByteNum++] = nCount/0x100;
		EcuComRxBuf[ByteNum++] = nCount%0x100;
		EcuComRxBuf[ByteNum++] = CAN_DATA;
		EcuComRxBuf[ByteNum++] = EcuComRxBuf[3];
		EcuComRxBuf[ByteNum++] = EcuComRxBuf[4];
		EcuComRxBuf[ByteNum++] = EcuComRxBuf[5];
		EcuComRxBuf[ByteNum++] = EcuComRxBuf[6];
		for(i=0; i<ByteNum+8; i++)
		{
			USART_PushByte( (uint8)EcuComRxBuf[i]);
			checknum = checknum + (uint8)EcuComRxBuf[i];
		}
		count = 6;
		Frame_FlagSN = 1;
		Frame_Count1 = 0;
		Frame_Count2 = 0;
		Frame_Length = Frame_Count0/Frame_DataNum;//流控制 连续帧 帧数
		while(Frame_Count2 < Frame_Length)
		{
			//诊断模式
			if(PassThruConfig.CtrlMode == 0x00)
			{
				if(Frame_Count1 == 0x00 || Frame_Count1 == PassThruConfig.ISO15765BS[PassThruConfig.Send_Pass])
				{
					Frame_Count1 = 0;
					//发送控制帧
					if(!PassThru_CAN_TxPack_FlowControl((uint8 *)EcuComRxBuf))
					{
						return FALSE;
					}
				}
			}
			//接收连续数据 按照实际接收
			if(PassThru_CAN_RxData_Time((uint8 *)EcuComRxBuf) == FALSE)
			{
				break;
			}
			//采样模式
			if(PassThruConfig.CtrlMode == 0x03)
			{
				//30命令过滤
				if((EcuComRxBuf[Frame_Control]&0xf0) == 0x30)
				{
					continue;
				}
			}
			if((EcuComRxBuf[Frame_Control]&0xf0) != CAN_15765_CF)
			{
				break;
			}
			if((EcuComRxBuf[Frame_Control]&0x0f) != Frame_FlagSN)
			{
				break;
			}
			//最后一帧
			number = Frame_DataNum;
			if(count + number > Frame_Count0)
			{
				number = Frame_Count0 - count;
			}
			for(i=0; i<number; i++)
			{
				USART_PushByte( (uint8)EcuComRxBuf[Frame_Control+1+i]);
				checknum = checknum + (uint8)EcuComRxBuf[Frame_Control+1+i];
			}
			ByteNum += number;
			count += number;
			Frame_Count1++;
			Frame_Count2++;
			Frame_FlagSN++;
			if(Frame_FlagSN > 0x0f)
			{
				Frame_FlagSN = 0x00;
			}
		}
		USART_PushByte(~checknum);
		//USART_TxQueueEndBuf();//发送剩下的数据
		USART_DMA_Send_ext(UartTxQueueBuf,nCount+5);
		return TRUE;
	}
	UartTxQueuePack(EcuComRxBuf);
	return TRUE;
}
/*********************************************************************************************
* 名称    ： PassThru_CAN_RxPack_AutoExt_ReadOnly
* 功能    ： 接收数据 自动回复控制帧
* 入口参数：
*
*
* 出口参数：
* 说明    ：0x87 ISO15765 多帧数据采样，自动处理控制帧 带ID 带扩展地址
*
***********************************************************************************************/
uint8 PassThru_CAN_RxPack_AutoExt_ReadOnly()
{
	uint16 Frame_Length;//多帧数据帧长度
	uint16 Frame_Count0;//多帧数据长度
	uint16 Frame_Count1;//单次连续多帧计数
	uint16 Frame_Count2;//全部连续多帧计数
	uint8  Frame_FlagSN;//连续帧标志位
	//uint8  Frame_Address = 0x07;//每帧数据扩展地址码
	uint8  Frame_Control = 0x08;//每帧数据控制位起始位置
	uint8  Frame_DataNum = 0x06;//每帧数据有效数据个数
	//uint8  Check_Address = PassThruConfig.RxAddress;
	uint16 ByteNum, count, number;
	uint16 i, nCount;
	uint8  checknum = 0;

	if(PassThru_CAN_RxData((uint8 *)EcuComRxBuf) == TRUE)
	{
		PassThru_SetKeepComSwitch2(EcuComRxBuf+Frame_Control+1);
	}
	else
	{
		return FALSE;
	}
	//接收多帧数据
	if((EcuComRxBuf[Frame_Control]&0xf0) == CAN_15765_FF)// && PassThruConfig.CanAnsR == 0x01)// PassThruConfig.CanAnst == 0x01)
	{
		Frame_Count0 = ((EcuComRxBuf[Frame_Control]&0x0f)<<8) + EcuComRxBuf[Frame_Control+1];
		count = 0;
		ByteNum = 0;
		nCount = Frame_Count0 + 8;
		USART1_ClearQueue();
		USART_PushByte(0xA5);
		USART_PushByte(0xA5);
		EcuComRxBuf[ByteNum++] = nCount/0x100;
		EcuComRxBuf[ByteNum++] = nCount%0x100;
		EcuComRxBuf[ByteNum++] = CAN_DATA;
		EcuComRxBuf[ByteNum++] = EcuComRxBuf[3];
		EcuComRxBuf[ByteNum++] = EcuComRxBuf[4];
		EcuComRxBuf[ByteNum++] = EcuComRxBuf[5];
		EcuComRxBuf[ByteNum++] = EcuComRxBuf[6];
		for(i=0; i<ByteNum+8; i++)
		{
			USART_PushByte( (uint8)EcuComRxBuf[i]);
			checknum = checknum + (uint8)EcuComRxBuf[i];
		}
		count = 5;
		Frame_FlagSN = 1;
		Frame_Count1 = 0;
		Frame_Count2 = 0;
		Frame_Length = Frame_Count0/Frame_DataNum;//流控制 连续帧 帧数
		while(Frame_Count2 < Frame_Length)
		{
			//诊断模式
			if(PassThruConfig.CtrlMode == 0x00)
			{
				if(Frame_Count1 == 0x00 || Frame_Count1 == PassThruConfig.ISO15765BS[PassThruConfig.Send_Pass])
				{
					Frame_Count1 = 0;
					//发送控制帧
					if(!PassThru_CAN_TxPack_FlowControl((uint8 *)EcuComRxBuf))
					{
						return FALSE;
					}
				}
			}
			//接收连续数据 按照实际接收
			if(PassThru_CAN_RxData_Time((uint8 *)EcuComRxBuf) == FALSE)
			{
				break;
			}
			//采样模式
			if(PassThruConfig.CtrlMode == 0x03)
			{
				//30命令过滤
				if((EcuComRxBuf[Frame_Control]&0xf0) == 0x30)
				{
					continue;
				}
			}
			if((EcuComRxBuf[Frame_Control]&0xf0) != CAN_15765_CF)
			{
				break;
			}
			if((EcuComRxBuf[Frame_Control]&0x0f) != Frame_FlagSN)
			{
				break;
			}
			//最后一帧
			number = Frame_DataNum;
			if(count + number > Frame_Count0)
			{
				number = Frame_Count0 - count;
			}
			for(i=0; i<number; i++)
			{
				USART_PushByte( (uint8)EcuComRxBuf[Frame_Control+1+i]);
				checknum = checknum + (uint8)EcuComRxBuf[Frame_Control+1+i];
			}
			ByteNum += number;
			count += number;
			Frame_Count1++;
			Frame_Count2++;
			Frame_FlagSN++;
			if(Frame_FlagSN > 0x0f)
			{
				Frame_FlagSN = 0x00;
			}
		}
		USART_PushByte(~checknum);
		//USART_TxQueueEndBuf();//发送剩下的数据USART_TxQueueEndBuf
		USART_DMA_Send_ext(UartTxQueueBuf,nCount+5);
		return TRUE;
	}
	UartTxQueuePack(EcuComRxBuf);
	return TRUE;
}

/*********************************************************************************************
*********************************************************************************************
* 名称    ： PassThru_CAN_RxPack_Auto_Ext
* 功能    ： 接收数据 自动回复控制帧 带扩展地址
* 入口参数：
*
*
* 出口参数：
* 说明    ：0x83 多帧数据自动回复控制帧 带ID 带扩展地址
*
***********************************************************************************************/
uint8 PassThru_CAN_RxPack_Auto_Ext()
{
	uint16 Frame_Length;//多帧数据帧长度
	uint16 Frame_Count0;//多帧数据长度
	uint16 Frame_Count1;//单次连续多帧计数
	uint16 Frame_Count2;//全部连续多帧计数
	uint8  Frame_FlagSN;//连续帧标志位
	uint8  Frame_Address = 0x07;//每帧数据扩展地址码
	uint8  Frame_Control = 0x08;//每帧数据控制位起始位置
	uint8  Frame_DataNum = 0x06;//每帧数据有效数据个数
	uint8  Check_Address = PassThruConfig.RxAddress;

	if(PassThru_CAN_RxData((uint8 *)EcuComRxBuf) == TRUE)
	{
		PassThru_SetKeepComSwitch2(EcuComRxBuf+Frame_Control+1);
		UartTxQueuePack(EcuComRxBuf);
		if(Check_Address > 0x00 && EcuComRxBuf[Frame_Address] != Check_Address)
		{
			return FALSE;
		}
		Check_Address = EcuComRxBuf[Frame_Address];
	}
	else
	{
		return FALSE;
	}
	//接收多帧数据
	if((EcuComRxBuf[Frame_Control]&0xf0) == CAN_15765_FF)// && PassThruConfig.CanAnsR == 0x02 )//PassThruConfig.CanAnsT == 0x02))/
	{
		Frame_Count0 = ((EcuComRxBuf[Frame_Control]&0x0f)<<8) + EcuComRxBuf[Frame_Control+1];
		Frame_FlagSN = 1;
		Frame_Count1 = 0;
		Frame_Count2 = 0;
		Frame_Length = Frame_Count0 / Frame_DataNum;//流控制 连续帧 帧数
		while (Frame_Count2 < Frame_Length)
		{
			if (Frame_Count1 == 0x00 || Frame_Count1 == PassThruConfig.ISO15765BS[PassThruConfig.Send_Pass]) {
				Frame_Count1 = 0;
				//发送控制帧
				if(!PassThru_CAN_TxPack_FlowControl((uint8 *)EcuComRxBuf))
				{
					return FALSE;
				}
			}
			//接收连续数据 按照实际接收
			if (PassThru_CAN_RxData_Time((uint8 *) EcuComRxBuf) == FALSE)
			{
				return FALSE;
			}
			UartTxQueuePack(EcuComRxBuf);
			if(EcuComRxBuf[Frame_Address] != Check_Address)
			{
				return FALSE;
			}
			if((EcuComRxBuf[Frame_Control]&0xf0) != CAN_15765_CF)
			{
				return FALSE;
			}
			if((EcuComRxBuf[Frame_Control]&0x0f) != Frame_FlagSN)
			{
				return FALSE;
			}
			Frame_Count1++;
			Frame_Count2++;
			Frame_FlagSN++;
			if(Frame_FlagSN > 0x0f)
			{
				Frame_FlagSN = 0x00;
			}
		}
	}
	return TRUE;
}

/*************************************
j1939中判断下接收的字节是否多帧
J1939ADJUST[13]={0x9c,0xec,0xf9,0x00,0x10,0x09,0x00,0x02,0xff,0x00,0xef,0x00};
除5.6 7字节外，其它一定要相同
*?***********************************/
uint8 AdjustJ1939(uint8 *rxbuf) {
	uint8 i;
	uint8 J1939ADJUST[8] = {0x10, 0x09, 0x00, 0x02, 0xff, 0x00, 0xef, 0x00};
	J1939ADJUST[1] = *(rxbuf + 1);                //记下有郊数据
	J1939ADJUST[2] = *(rxbuf + 2);
	J1939ADJUST[3] = *(rxbuf + 3);
	for (i = 0; i < 8; i++) {
		if (*rxbuf != J1939ADJUST[i])//J1939多帧接收中，除第1 2 3 字节不关心外，其它字节必须相同才认为多帧
		{
			return FALSE;
		}
		rxbuf++;
	}
	return TRUE;
}
/*********************************************************************************************
* 名称    ： PassThru_CAN_RxPack_Auto_Ext
* 功能    ： 接收数据 自动回复控制帧 带扩展地址
* 入口参数：
单帧模式
rece:a5 a5 00 0d 30 98 ef 00 f9 43 00 00 00 00 00 02 ff fe
rece:a5 a5 00 0d 30 98 ef f9 00 44 00 00 02 58 58 ff ff 4e
多帧模式
rece:a5 a5 00 0d 30 98 ec 00 f9 10 09 00 02 ff 00 ef 00 3c  仪器要发两帧，总有郊个数0009个　
rece:a5 a5 00 0d 30 98 ec f9 00 11 02 01 ff ff 00 ef 00 44　ECU允许发送，接收两帧数据，从第一帧开始接收
rece:a5 a5 00 0d 30 98 eb 00 f9 01 81 02 01 01 ff 01 ff c1　仪器发送的两条数据
rece:a5 a5 00 0d 30 98 eb 00 f9 02 00 00 ff ff ff ff ff 49   
rece:a5 a5 00 0d 30 98 ec f9 00 13 09 00 02 ff 00 ef 00 39　ecu确认收到两帧0009个数据

rece:a5 a5 00 0d 30 9c ec f9 00 10 09 00 02 ff 00 ef 00 38   ecu要发送两条0009个数据
rece:a5 a5 00 0d 30 9c ec 00 f9 11 02 01 ff ff 00 ef 00 40   仪器确定接收两个数据，从第一帧开始
rece:a5 a5 00 0d 30 9c eb f9 00 01 81 01 02 00 01 70 01 4b   　ecu发送的两条数据
rece:a5 a5 00 0d 30 9c eb f9 00 02 05 32 ff ff ff ff ff 0e
rece:a5 a5 00 0d 30 9c ec 00 f9 13 09 00 02 ff 00 ef 00 35   仪器确认收到两帧0009个数据
***********************************************************************************************/
uint8 PassThru_CAN_RxPack_J1939()
{
	uint16 i,ByteNum = 0;
	uint16 Frame_Count;//多帧数据帧长度
	uint16 Data_Count;//多帧数据长度
	uint16 Frame_Start;//单次连续多帧计数
	uint16 Frame_End;//全部连续多帧计数
	// uint32 PGN;
	//uint32 ReadID;
	uint32 CanID;
	uint16 count;
	uint16 number;
	uint8  PGN_KEY;
	uint8  Address_Target;
	uint8  Address_Source;
	uint8  Priority;
	uint8  Frame_FlagSN;//连续帧标志位
	uint8  Frame_Control = 0x07;//每帧数据控制位位置
	uint8  Frame_DataNum = 0x07;//每帧数据有效数据个数
	uint8  ReadCount;
	uint16  nCount;
	uint8  checknum = 0;
	uint8  errornum = 0;
	uint8  FrameGetCount = 0;
	uint8  MaxRxFrameCount = PassThruConfig.ISO15765BS[0];//连续帧的最多帧数　

	if(PassThru_CAN_RxData((uint8 *)EcuComRxBuf) == TRUE)
	{
	}
	else
	{
		return FALSE;
	}
	//接收多帧数据
	//a5 a5 00 0d 30 98 ec 00 f9 10 09 00 02 ff 00 ef 00 3c
if(EcuComRxBuf[4] == 0xec && EcuComRxBuf[Frame_Control] == 0x10)
	{
		Priority = (EcuComRxBuf[3]&0x1C)/4;
		Address_Target = EcuComRxBuf[Frame_Control-2];
		Address_Source = EcuComRxBuf[Frame_Control-1];
		PGN_KEY = EcuComRxBuf[Frame_Control+6];
		Data_Count = EcuComRxBuf[Frame_Control+1] + (EcuComRxBuf[Frame_Control+2]<<8);
		Frame_Count = EcuComRxBuf[Frame_Control+3];
		ReadCount = 0;
// 		while (1)
// 		{
// 			if(ReadCount++ > 3)
// 			{
// 				return FALSE;
// 			}
		ByteNum = 0;
		nCount = Data_Count + 5;
		//QueueWrite(Uart0TxQueueBuf,0xA5);
		//QueueWrite(Uart0TxQueueBuf,0xA5);
		UartTxQueueBuf[ByteNum++]=0xa5;
		UartTxQueueBuf[ByteNum++]=0xa5;
		UartTxQueueBuf[ByteNum++] = nCount/0x100;
		UartTxQueueBuf[ByteNum++] = nCount%0x100;
		UartTxQueueBuf[ByteNum++] = CAN_DATA;
		UartTxQueueBuf[ByteNum++] = 0x80+(Priority<<2);
		UartTxQueueBuf[ByteNum++] = EcuComRxBuf[Frame_Control+6];
		UartTxQueueBuf[ByteNum++] = Address_Target;
		UartTxQueueBuf[ByteNum++] = Address_Source;
			//for(i=0; i<ByteNum; i++)
			//{
			//    //QueueWrite(Uart0TxQueueBuf, (uint8)EcuComRxBuf[i]);
			//    chacknum = chacknum + (uint8)J1939RxBuf[i];
			//}
		while (1)
		{
			if(ReadCount++ > 3)
			{
				return FALSE;
			}
			ByteNum = 9;//ByteNum = 7;程兵兵修改，有a5 a5所以要加两个字节
			count = 0;
			Frame_Start = 0x01;
			if(MaxRxFrameCount == 0)
			{
				MaxRxFrameCount = 5;//默认接收帧数
			}

			while(Frame_Start < Frame_Count+1)
			{
				//a5 a5 00 0d 30 98 ec f9 00 11 02 01 ff ff 00 ef 00 44
				FrameGetCount = Frame_Count - Frame_Start + 1;
				if(FrameGetCount > MaxRxFrameCount)	// 00全收，
				{
					FrameGetCount = MaxRxFrameCount;
				}
				i = 0;
				EcuComTxBuf[i++] = 0x11;
				EcuComTxBuf[i++] = FrameGetCount;//Frame_Count;
				EcuComTxBuf[i++] = Frame_Start;
				EcuComTxBuf[i++] = 0xff;
				EcuComTxBuf[i++] = 0xff;
				EcuComTxBuf[i++] = 0x00;
				EcuComTxBuf[i++] = PGN_KEY;
				EcuComTxBuf[i++] = 0x00;
				CanID = 0x80ec0000 + (Priority<<26) + (Address_Source<<8) + Address_Target;
				//CanID = 0x98ec0000 + (Address_Source<<8) + Address_Target;
				if(!CAN_TxData(EcuComTxBuf, i, CanID))
				{
					return FALSE;
				}
				Frame_End = Frame_Start + FrameGetCount;
				while(Frame_Start < Frame_End)
				{
					if(PassThru_CAN_RxData_Time((uint8 *)EcuComRxBuf) == FALSE)
					{
						return FALSE;
					}
					if(EcuComRxBuf[4] == 0xeb && EcuComRxBuf[5] == Address_Target && EcuComRxBuf[6] == Address_Source)
					{
						if(EcuComRxBuf[Frame_Control] < Frame_Start)
						{
							continue;//序号不对继续收
						}
						if(EcuComRxBuf[Frame_Control] > Frame_Start)
						{
							break;//接收错误 直接退出
						}
						//最后一帧
						number = Frame_DataNum;
						if(count + number > Data_Count)
						{
							number = Data_Count - count;
						}
						for(i=0; i<number; i++)
						{
							//QueueWrite(Uart0TxQueueBuf, (uint8)EcuComRxBuf[Frame_Control+1+i]);
							UartTxQueueBuf[ByteNum++] = EcuComRxBuf[Frame_Control+1+i];
							//chacknum = chacknum + (uint8)EcuComRxBuf[Frame_Control+1+i];
						}
						//ByteNum += number;
						count += number;
						Frame_Start++;//接收下一帧数据
						continue;
					}
					return FALSE;
				}
				Frame_Start = Frame_End;//收完一组，继续收下一组
			}
			//收到完整的数据帧
			if(count == Data_Count)
			{
				break;
			}
		}
		checknum=0;
		for(i=2;i<ByteNum;i++)
		{
			checknum+=UartTxQueueBuf[i];
		}
		UartTxQueueBuf[ByteNum++]=(~checknum)&0xff;
		USART_DMA_Send_ext(UartTxQueueBuf_ext,ByteNum);
		//a5 a5 00 0d 30 98 ec f9 00 13 09 00 02 ff 00 ef 00 39
		ByteNum = 0;
		EcuComTxBuf[ByteNum++] = 0x13;
		EcuComTxBuf[ByteNum++] = Data_Count%0x100;
		EcuComTxBuf[ByteNum++] = Data_Count/0x100;
		EcuComTxBuf[ByteNum++] = Frame_Count;
		EcuComTxBuf[ByteNum++] = 0xff;
		EcuComTxBuf[ByteNum++] = 0x00;
		EcuComTxBuf[ByteNum++] = PGN_KEY;
		EcuComTxBuf[ByteNum++] = 0x00;
		CanID = 0x80ec0000 + (Priority<<26) + (Address_Source<<8) + Address_Target;
		//CanID = 0x98ec0000 + (Address_Source<<8) + Address_Target;
		if(!CAN_TxData(EcuComTxBuf, ByteNum, CanID))
		{
			return FALSE;
		}
		return TRUE;
	}
	//rece:a5 a5 00 0d 30 98 ec ff 3d 20 1e 00 05 ff ca fe 00 f8
	else if(EcuComRxBuf[4] == 0xec && EcuComRxBuf[5] == 0xff && EcuComRxBuf[Frame_Control] == 0x20)
	{
		Priority = (EcuComRxBuf[3]&0x1C)/4;
		Address_Source = EcuComRxBuf[Frame_Control-1];
		Data_Count = EcuComRxBuf[Frame_Control+1] + (EcuComRxBuf[Frame_Control+2]<<8);
		Frame_Count = EcuComRxBuf[Frame_Control+3];
		ByteNum = 0;
		nCount = Data_Count + 5;
		//QueueWrite(Uart0TxQueueBuf,0xA5);
		//QueueWrite(Uart0TxQueueBuf,0xA5);
		UartTxQueueBuf[ByteNum++]=0xa5;
		UartTxQueueBuf[ByteNum++]=0xa5;
		UartTxQueueBuf[ByteNum++] = nCount/0x100;
		UartTxQueueBuf[ByteNum++] = nCount%0x100;
		UartTxQueueBuf[ByteNum++] = CAN_DATA;
		UartTxQueueBuf[ByteNum++] = 0x80 + (Priority<<2) + EcuComRxBuf[Frame_Control+7];
		//UartTxQueueBuf[ByteNum++] = 0x98|EcuComRxBuf[Frame_Control+7];
		UartTxQueueBuf[ByteNum++] = EcuComRxBuf[Frame_Control+6];
		UartTxQueueBuf[ByteNum++] = EcuComRxBuf[Frame_Control+5];
		UartTxQueueBuf[ByteNum++] = Address_Source;
		//for(i=0; i<ByteNum; i++)
		//{
		//    //QueueWrite(Uart0TxQueueBuf, (uint8)EcuComRxBuf[i]);
		//    chacknum = chacknum + (uint8)J1939RxBuf[i];
		//}
		errornum = 0;
		count = 0;
		Frame_Start = 0x01;
		Frame_End = Frame_Start + Frame_Count;
		Frame_FlagSN = Frame_Start;
		while(Frame_FlagSN<Frame_End && errornum<10)
		{
			if(PassThru_CAN_RxData_Time((uint8 *)EcuComRxBuf) == FALSE)
			{
				return FALSE;
			}
			if(EcuComRxBuf[4] == 0xeb && EcuComRxBuf[5] == 0xff && EcuComRxBuf[6] == Address_Source)
			{
				if(EcuComRxBuf[Frame_Control] == Frame_FlagSN && EcuComRxBuf[Frame_Control] <= Frame_Count)
				{
					//最后一帧
					number = Frame_DataNum;
					if(count + number > Data_Count)
					{
						number = Data_Count - count;
					}
					for(i=0; i<number; i++)
					{
						//QueueWrite(Uart0TxQueueBuf, (uint8)EcuComRxBuf[Frame_Control+1+i]);
						UartTxQueueBuf[ByteNum++] = (uint8)EcuComRxBuf[Frame_Control+1+i];
						//chacknum = chacknum + (uint8)EcuComRxBuf[Frame_Control+1+i];
					}
					//ByteNum += number;
					count += number;
					Frame_FlagSN++;
					errornum = 0;
					continue;
				}
			}
			errornum++;
			//UartTxQueuePack(EcuComRxBuf);//单帧直接回复
		}
		checknum=0;
		for(i=2;i<ByteNum;i++)
		{
			checknum+=UartTxQueueBuf[i];
		}
		UartTxQueueBuf[ByteNum++]=(~checknum)&0xff;
		USART_DMA_Send_ext(UartTxQueueBuf_ext,ByteNum);
		return TRUE;
	}
	UartTxQueuePack(EcuComRxBuf);
	
	return TRUE;
}

/*********************************************************************************************
* 名称    ： PassThru_CAN_RxPack_Auto_Ext
* 功能    ： 接收数据 自动回复控制帧 带扩展地址
* 入口参数：
rece:a5 a5 00 0d 30 98 ef 00 f9 43 00 00 00 00 00 02 ff fe
rece:a5 a5 00 0d 30 98 ef f9 00 44 00 00 02 58 58 ff ff 4e
rece:a5 a5 00 0d 30 98 ec 00 f9 10 09 00 02 ff 00 ef 00 3c
rece:a5 a5 00 0d 30 98 ec f9 00 11 02 01 ff ff 00 ef 00 44
rece:a5 a5 00 0d 30 98 eb 00 f9 01 81 02 01 01 ff 01 ff c1
rece:a5 a5 00 0d 30 98 eb 00 f9 02 00 00 ff ff ff ff ff 49
rece:a5 a5 00 0d 30 98 ec f9 00 13 09 00 02 ff 00 ef 00 39
rece:a5 a5 00 0d 30 9c ec f9 00 10 09 00 02 ff 00 ef 00 38
rece:a5 a5 00 0d 30 9c ec 00 f9 11 02 01 ff ff 00 ef 00 40
rece:a5 a5 00 0d 30 9c eb f9 00 01 81 01 02 00 01 70 01 4b
rece:a5 a5 00 0d 30 9c eb f9 00 02 05 32 ff ff ff ff ff 0e
rece:a5 a5 00 0d 30 9c ec 00 f9 13 09 00 02 ff 00 ef 00 35

rece:a5 a5 00 0d 30 98 ec ff 3d 20 1e 00 05 ff ca fe 00 f8
rece:a5 a5 00 0d 30 98 eb ff 3d 01 04 ff 32 f0 e5 00 2c cc
rece:a5 a5 00 0d 30 98 eb ff 3d 02 f0 e5 00 2a f0 e5 07 26
rece:a5 a5 00 0d 30 98 eb ff 3d 03 2e f0 e5 00 6f f0 f3 ab
rece:a5 a5 00 0d 30 98 eb ff 3d 04 00 26 0b 13 00 26 0b 8a
rece:a5 a5 00 0d 30 98 eb ff 3d 05 13 00 ff ff ff ff ff f0
***********************************************************************************************/
uint8 PassThru_CAN_RxPack_J1939_ReadOnly()
{
	uint16 i,checknum,ByteNum = 0;
	uint16 Frame_Count;//多帧数据帧长度
	uint16 Data_Count;//多帧数据长度
	uint16 Frame_Start;//单次连续多帧计数
	uint16 Frame_End;//全部连续多帧计数
	uint32 PGN;
	uint16 count;
	uint16 number;
	uint8  Address_Target;
	uint8  Priority;
	uint8  Address_Source;
	uint8  Frame_FlagSN;//连续帧标志位
	uint8  Frame_Control = 0x07;//每帧数据控制位位置
	uint8  Frame_DataNum = 0x07;//每帧数据有效数据个数
	uint16 nCount, retry;
	//uint8  chacknum = 0;
	uint8  errornum = 0;

	if(PassThru_CAN_RxData((uint8 *)EcuComRxBuf) == TRUE)
	{
		//Uart0TxQueuePack(EcuComRxBuf);
	}
	else
	{
		return FALSE;
	}
	//接收多帧数据
	if(EcuComRxBuf[4] == 0xec && EcuComRxBuf[Frame_Control] == 0x10)
	{
		Priority = (EcuComRxBuf[3]&0x1C)/4;
		Address_Source = EcuComRxBuf[Frame_Control-1];
		PGN = EcuComRxBuf[Frame_Control+5] + (EcuComRxBuf[Frame_Control+6]<<8) + (EcuComRxBuf[Frame_Control+7]<<16);
		Data_Count = EcuComRxBuf[Frame_Control+1] + (EcuComRxBuf[Frame_Control+2]<<8);
		Frame_Count = EcuComRxBuf[Frame_Control+3];
		errornum = 0;
		retry = 10;
		while(errornum++ < retry)
		{
			if(PassThru_CAN_RxData_Time((uint8 *)EcuComRxBuf) == FALSE)
			{
				return FALSE;
			}
			if(EcuComRxBuf[4] == 0xec && EcuComRxBuf[Frame_Control] == 0x10)
			{
				return FALSE;
			}
			//a5 a5 00 0d 30 98 ec f9 00 11 02 01 ff ff 00 ef 00 44
			if(EcuComRxBuf[4] == 0xec && EcuComRxBuf[Frame_Control] == 0x11)
			{
				if(PGN == EcuComRxBuf[Frame_Control+5] + (EcuComRxBuf[Frame_Control+6]<<8) + (EcuComRxBuf[Frame_Control+7]<<16))
				{
					break;
				}
			}
			Uart0TxQueuePack(EcuComRxBuf);//单帧直接回复
		}
		if(errornum > retry)
		{
			return FALSE;
		}
		Frame_Start = EcuComRxBuf[Frame_Control+2];
		Frame_End = EcuComRxBuf[Frame_Control+1] + EcuComRxBuf[Frame_Control+2];
		Address_Target = EcuComRxBuf[Frame_Control-1];
		if(Frame_Start != 0x01)
		{
			return FALSE;
		}
		nCount = Data_Count + 5;
		//QueueWrite(Uart0TxQueueBuf,0xA5);
		//QueueWrite(Uart0TxQueueBuf,0xA5);
		UartTxQueueBuf[ByteNum++]=0xa5;
		UartTxQueueBuf[ByteNum++]=0xa5;
		UartTxQueueBuf[ByteNum++] = nCount/0x100;
		UartTxQueueBuf[ByteNum++] = nCount%0x100;
		UartTxQueueBuf[ByteNum++] = CAN_DATA;
		UartTxQueueBuf[ByteNum++] = 0x80+(Priority<<2);
		UartTxQueueBuf[ByteNum++] = EcuComRxBuf[Frame_Control+6];
		UartTxQueueBuf[ByteNum++] = Address_Target;
		UartTxQueueBuf[ByteNum++] = Address_Source;
		//for(i=0; i<ByteNum; i++)
		//{
		//    //QueueWrite(Uart0TxQueueBuf, (uint8)EcuComRxBuf[i]);
		//    chacknum = chacknum + (uint8)J1939RxBuf[i];
		//}
		count = 0;
		//for(Frame_FlagSN=Frame_Start; Frame_FlagSN<Frame_End; Frame_FlagSN++)
		//从第一帧开始，直到最后一帧数据，全部接收，中间过程也打印出来
		errornum = 0;
		Frame_Start = 0x01;
		Frame_End = Frame_Start + Frame_Count;
		Frame_FlagSN = Frame_Start;
		while(Frame_FlagSN<Frame_End && errornum<10)
		{
			if(PassThru_CAN_RxData_Time((uint8 *)EcuComRxBuf) == FALSE)
			{
				break;
			}
			if(EcuComRxBuf[4] == 0xeb && EcuComRxBuf[5] == Address_Target && EcuComRxBuf[6] == Address_Source)
			{
				if(EcuComRxBuf[Frame_Control] == Frame_FlagSN && EcuComRxBuf[Frame_Control] <= Frame_Count)
				{
					//最后一帧
					number = Frame_DataNum;
					if(count + number > Data_Count)
					{
						number = Data_Count - count;
					}
					for(i=0; i<number; i++)
					{
						//QueueWrite(Uart0TxQueueBuf, (uint8)EcuComRxBuf[Frame_Control+1+i]);
						UartTxQueueBuf[ByteNum++] = EcuComRxBuf[Frame_Control+1+i];
						//chacknum = chacknum + (uint8)EcuComRxBuf[Frame_Control+1+i];
					}
					//ByteNum += number;
					count += number;
					Frame_FlagSN++;
					errornum = 0;
					continue;
				}
			}
			errornum++;
			//UartTxQueuePack(EcuComRxBuf);//单帧直接回复
		}
		checknum=0;
		for(i=2;i<ByteNum;i++)
		{
			checknum+=UartTxQueueBuf[i];
		}
		UartTxQueueBuf[ByteNum++]=(~checknum)&0xff;
		USART_DMA_Send_ext(UartTxQueueBuf_ext,ByteNum);
		//13最后收 我们回复13的时间比较迟
		if(PassThru_CAN_RxData_Time((uint8 *)EcuComRxBuf) == FALSE)
		{
			return FALSE;
		}
		if(EcuComRxBuf[4] != 0xec || EcuComRxBuf[Frame_Control] != 0x13)
		{
			return FALSE;
		}
		return TRUE;
	}
	//rece:a5 a5 00 0d 30 98 ec ff 3d 20 1e 00 05 ff ca fe 00 f8
	else if(EcuComRxBuf[4] == 0xec && EcuComRxBuf[5] == 0xff && EcuComRxBuf[Frame_Control] == 0x20)
	{
		Priority = (EcuComRxBuf[3]&0x1C)/4;
		Address_Source = EcuComRxBuf[Frame_Control-1];
		Data_Count = EcuComRxBuf[Frame_Control+1] + (EcuComRxBuf[Frame_Control+2]<<8);
		Frame_Count = EcuComRxBuf[Frame_Control+3];
		ByteNum = 0;
		nCount = Data_Count + 5;
		//QueueWrite(Uart0TxQueueBuf,0xA5);
		//QueueWrite(Uart0TxQueueBuf,0xA5);
		UartTxQueueBuf[ByteNum++]=0xa5;
		UartTxQueueBuf[ByteNum++]=0xa5;
		UartTxQueueBuf[ByteNum++] = nCount/0x100;
		UartTxQueueBuf[ByteNum++] = nCount%0x100;
		UartTxQueueBuf[ByteNum++] = CAN_DATA;
		UartTxQueueBuf[ByteNum++] = 0x80+(Priority<<2)+EcuComRxBuf[Frame_Control+7];
		UartTxQueueBuf[ByteNum++] = EcuComRxBuf[Frame_Control+6];
		UartTxQueueBuf[ByteNum++] = EcuComRxBuf[Frame_Control+5];
		UartTxQueueBuf[ByteNum++] = Address_Source;
		//for(i=0; i<ByteNum; i++)
		//{
		//    //QueueWrite(Uart0TxQueueBuf, (uint8)EcuComRxBuf[i]);
		//    chacknum = chacknum + (uint8)J1939RxBuf[i];
		//}
		errornum = 0;
		count = 0;
		Frame_Start = 0x01;
		Frame_End = Frame_Start + Frame_Count;
		Frame_FlagSN = Frame_Start;
		while(Frame_FlagSN<Frame_End && errornum<10)
		{
			if(PassThru_CAN_RxData_Time((uint8 *)EcuComRxBuf) == FALSE)
			{
				return FALSE;
			}
			if(EcuComRxBuf[4] == 0xeb && EcuComRxBuf[5] == 0xff && EcuComRxBuf[6] == Address_Source)
			{
				if(EcuComRxBuf[Frame_Control] == Frame_FlagSN && EcuComRxBuf[Frame_Control] <= Frame_Count)
				{
					//最后一帧
					number = Frame_DataNum;
					if(count + number > Data_Count)
					{
						number = Data_Count - count;
					}
					for(i=0; i<number; i++)
					{
						//QueueWrite(Uart0TxQueueBuf, (uint8)EcuComRxBuf[Frame_Control+1+i]);
						UartTxQueueBuf[ByteNum++] = (uint8)EcuComRxBuf[Frame_Control+1+i];
						//chacknum = chacknum + (uint8)EcuComRxBuf[Frame_Control+1+i];
					}
					//ByteNum += number;
					count += number;
					Frame_FlagSN++;
					errornum = 0;
					continue;
				}
			}
			errornum++;
			//UartTxQueuePack(EcuComRxBuf);//单帧直接回复
		}
		checknum=0;
		for(i=2;i<ByteNum;i++)
		{
			checknum+=UartTxQueueBuf[i];
		}
		UartTxQueueBuf[ByteNum++]=(~checknum)&0xff;
		USART_DMA_Send_ext(UartTxQueueBuf_ext,ByteNum);
		return TRUE;
	}
	UartTxQueuePack(EcuComRxBuf);
	return TRUE;
}


/*********************************************************************************************
* 名称    ： PassThru_CAN_TxPack_Auto_Ext
* 功能    ： CANBUS协议发送数据 带扩展地址
* 入口参数：
*
*
* 出口参数：
* 说明    ：0x85 多帧数据自动处理控制帧 带扩展地址
*
rece:a5 a5 00 0d 30 98 ef 00 f9 43 00 00 00 00 00 02 ff fe
rece:a5 a5 00 0d 30 98 ef f9 00 44 00 00 02 58 58 ff ff 4e
rece:a5 a5 00 0d 30 98 ec 00 f9 10 09 00 02 ff 00 ef 00 3c
rece:a5 a5 00 0d 30 98 ec f9 00 11 02 01 ff ff 00 ef 00 44
rece:a5 a5 00 0d 30 98 eb 00 f9 01 81 02 01 01 ff 01 ff c1
rece:a5 a5 00 0d 30 98 eb 00 f9 02 00 00 ff ff ff ff ff 49
rece:a5 a5 00 0d 30 98 ec f9 00 13 09 00 02 ff 00 ef 00 39
rece:a5 a5 00 0d 30 9c ec f9 00 10 09 00 02 ff 00 ef 00 38
rece:a5 a5 00 0d 30 9c ec 00 f9 11 02 01 ff ff 00 ef 00 40
rece:a5 a5 00 0d 30 9c eb f9 00 01 81 01 02 00 01 70 01 4b
rece:a5 a5 00 0d 30 9c eb f9 00 02 05 32 ff ff ff ff ff 0e
rece:a5 a5 00 0d 30 9c ec 00 f9 13 09 00 02 ff 00 ef 00 35

rece:a5 a5 00 0d 30 98 ef 00 f9 43 00 00 00 00 00 02 ff fe
rece:a5 a5 00 0d 30 98 ef f9 00 44 00 00 02 58 58 ff ff 4e
rece:a5 a5 00 0e 30 98 ef 00 f9 81 02 01 01 ff 01 ff 00 00 bd
rece:a5 a5 00 0e 30 98 ef f9 00 81 01 02 00 01 70 01 05 00 46
***********************************************************************************************/
uint8 PassThru_CAN_TxPack_J1939(uint8 *TXdata)
{
	uint8 result;
	//uint8 FC_BS;
	uint8 FC_STmin;
	//uint8 CF_SN; //连续帧序号
	//uint16 CF_DL; //连续帧长度
	//uint16 CF_CNT;//连续帧计数
	uint8 temp0, temp1;
	uint16 PackLen;
	uint8  Frame_Control = 0x07;//每帧数据控制位起始位置
	uint32 CanID = PassThruConfig.CanTD;

	uint16 ByteNum = 0;
	uint16 Frame_Count;//多帧数据帧长度
	uint16 Data_Count;//多帧数据长度
	uint16 Frame_Start;//单次连续多帧计数
	uint16 Frame_End;//全部连续多帧计数
	//uint32 PGN;
	//uint32 ReadID;
	//uint16 count;
	uint16 number;
	uint8  PGN_KEY;
	uint8  Address_Target;
	uint8  Address_Source;
	uint8  Frame_FlagSN;//连续帧标志位
	uint8  Frame_Watch = FALSE;
	uint8 *TXhead;
	uint8  Priority;
    printf("PassThru_CAN_TxPack_J1939\r\n");
	if(TXdata[4] > 0xef)
	{
		//命令中没有接收地址，改用广播发送多帧
		return PassThru_CAN_TxPack_J1939_SendOnly(TXdata);
	}
	temp0 = *TXdata++;
	temp1 = *TXdata++;
	PackLen = (temp0<<8) + temp1;
	temp0 = *TXdata++;//CANBUS ORD 0x30
	CanID = *TXdata++;             //AR1_H
	Priority = (CanID&0x1C)/4;
	CanID = (CanID << 8) + (*TXdata++);
	CanID = (CanID << 8) + (*TXdata++);
	CanID = (CanID << 8) + (*TXdata++); //AR1_L3
	PackLen -= 5;//去掉头和CANID
	//a5 a5 00 0d 30 98 ef 00 f9 43 00 00 00 00 00 02 ff fe
	if(PackLen < 0x09 )//只是一帧，直接发出后退出
	{
		result = CAN_TxData((uint8 *)TXdata, PackLen, CanID);
		return result;
	}
	//打包的多帧数据 自动处理控制帧
	//a5 a5 00 0e 30 98 ef 00 f9 81 02 01 01 ff 01 ff 00 00 bd
	Address_Source = (CanID)&0xff;
	Address_Target = (CanID>>8)&0xff;
	PGN_KEY = (CanID>>16)&0xff;
	Data_Count = PackLen;
	Frame_Count = (PackLen+6)/7;
	Frame_Start = 0x01;
	Frame_End = Frame_Count;

	//a5 a5 00 0d 30 98 ec 00 f9 10 09 00 02 ff 00 ef 00 3c
	ByteNum = 0;
	EcuComTxBuf[ByteNum++] = 0x10;
	EcuComTxBuf[ByteNum++] = Data_Count%0x100;
	EcuComTxBuf[ByteNum++] = Data_Count/0x100;
	EcuComTxBuf[ByteNum++] = Frame_Count;
	EcuComTxBuf[ByteNum++] = 0xff;
	EcuComTxBuf[ByteNum++] = 0x00;
	EcuComTxBuf[ByteNum++] = PGN_KEY;
	EcuComTxBuf[ByteNum++] = 0x00;
	CanID = 0x80ec0000 + (Priority<<26) + (Address_Target<<8) + Address_Source;
	//CanID = 0x98ec0000 + (Address_Target<<8) + Address_Source;
	if(!CAN_TxData(EcuComTxBuf, ByteNum, CanID))
	{
		return FALSE;
	}
	while(1)
	{
		if(PassThru_CAN_RxData_Time((uint8 *)EcuComRxBuf) == TRUE)
		{
			if(Frame_Watch) UartTxQueuePack(EcuComRxBuf);
		}
		else
		{
			return FALSE;
		}
		if(PGN_KEY != EcuComRxBuf[Frame_Control+6])
		{
			if(!Frame_Watch) UartTxQueuePack(EcuComRxBuf);
			return FALSE;
		}
		//a5 a5 00 0d 30 98 ec f9 00 11 02 01 ff ff 00 ef 00 44
		if(EcuComRxBuf[4] == 0xec && EcuComRxBuf[Frame_Control] == 0x11)
		{
			Frame_Start = EcuComRxBuf[Frame_Control+2];
			Frame_End = EcuComRxBuf[Frame_Control+1] + EcuComRxBuf[Frame_Control+2];
			TXhead = TXdata + (Frame_Start-1)*7;
			//a5 a5 00 0d 30 98 eb 00 f9 01 81 02 01 01 ff 01 ff c1
			//a5 a5 00 0d 30 98 eb 00 f9 02 00 00 ff ff ff ff ff 49
			for(Frame_FlagSN=Frame_Start; Frame_FlagSN<Frame_End; Frame_FlagSN++)
			{
				ByteNum = 0;
				EcuComTxBuf[ByteNum++] = Frame_FlagSN;
				EcuComTxBuf[ByteNum++] = *TXhead++;
				EcuComTxBuf[ByteNum++] = *TXhead++;
				EcuComTxBuf[ByteNum++] = *TXhead++;
				EcuComTxBuf[ByteNum++] = *TXhead++;
				EcuComTxBuf[ByteNum++] = *TXhead++;
				EcuComTxBuf[ByteNum++] = *TXhead++;
				EcuComTxBuf[ByteNum++] = *TXhead++;
				CanID = 0x80eb0000 + (Priority<<26) + (Address_Target<<8) + Address_Source;
				//CanID = 0x98eb0000 + (Address_Target<<8) + Address_Source;
				//最后一帧补充0xff
				if(Frame_FlagSN == Frame_Count)
				{
					number = Frame_Count*7 - Data_Count;
					memset(EcuComTxBuf+8-number, 0xff, number);
				}
				if(PassThruConfig.ISO15765STmin > 0)
				{
					FC_STmin = PassThruConfig.ISO15765STmin[PassThruConfig.Send_Pass];
					PassThru_CAN_TxMuchPack_DelayTime(FC_STmin);//帧间间隔时间 0f参数
				}
				if(!CAN_TxData((uint8 *)EcuComTxBuf, ByteNum, CanID))
				{
					return FALSE;
				}
			}
		}
		//a5 a5 00 0d 30 98 ec f9 00 13 09 00 02 ff 00 ef 00 39
		if(EcuComRxBuf[4] == 0xec && EcuComRxBuf[Frame_Control] == 0x13 && EcuComRxBuf[Frame_Control+3] == Frame_Count)
		{
			if(Data_Count != EcuComRxBuf[Frame_Control+2]*0x100+EcuComRxBuf[Frame_Control+1])
			{
				if(!Frame_Watch) UartTxQueuePack(EcuComRxBuf);
				return FALSE;
			}
			break;
		}
	}
	return TRUE;
}


/*********************************************************************************************
* 名称    ： PassThru_CAN_TxPack_Auto
* 功能    ： CANBUS协议发送数据
* 入口参数：
*
*
* 出口参数：
* 说明    ：0x82 多帧数据自动处理控制帧
*
***********************************************************************************************/
uint8 PassThru_CAN_TxPack_Auto(uint8 *TXdata)
{
	uint8 result;
	uint8 FC_BS;
	uint8 FC_STmin;
	uint16 CF_SN; //连续帧序号
	uint16 CF_DL; //连续帧长度
	uint16 CF_CNT;//连续帧计数
	uint8 temp0, temp1;
	uint16 PackLen;
	uint32 CanID;
	uint8  DataLen = 7;//每次发送数据长度
	uint8  DataBuf[8];
	uint8  Frame_Control = 0x07;//每帧数据控制位起始位置
    
	temp0 = *TXdata++;
	temp1 = *TXdata++;
	PackLen = (temp0<<8) + temp1;
	temp0 = *TXdata++;//CANBUS ORD 0x30
	CanID = *TXdata++;             //AR1_H
	CanID = (CanID << 8) + (*TXdata++);
	CanID = (CanID << 8) + (*TXdata++);
	CanID = (CanID << 8) + (*TXdata++); //AR1_L3
	PassThruConfig.CanLastTD[PassThruConfig.Send_Pass]=CanID;
	PackLen -= 5;//去掉头和CANID
	if(PackLen < 0x09)//单帧
	{
		result = CAN_TxData((uint8 *)TXdata, PackLen, CanID);
		return result;
	}

	if(PackLen > 0x08 && (*TXdata & 0xf0) == CAN_15765_FF )//&&  PassThruConfig.CanAnsR == 0x01)//PassThruConfig.CanAnsT == 0x01 )//多帧第一帧
	{
		//发送第一帧数据
		CF_DL = 8;
		if(!CAN_TxData((uint8 *)TXdata, CF_DL, CanID))
		{
			return FALSE;
		}
		PackLen -= CF_DL;
		TXdata += CF_DL;
		CF_SN    = 0x21;
		CF_CNT   = 0x01;
		FC_BS    = 0x01;	//连续帧的最大帧个数
		FC_STmin = 0x00;	//连续帧的帧间的最小时间间隔
		while(PackLen > 0)
		{
			if(FC_BS == CF_CNT)
			{
				//流控制帧 00 0d 30 00 00 07 e8 30 08 0a 00 00 00 00 00
				while(1)
				{
					if(!PassThru_CAN_RxData_Time((uint8 *)EcuComRxBuf))
					{
						return FALSE;
					}
					if((EcuComRxBuf[Frame_Control]&0xf0) != CAN_15765_FC)
					{
						return FALSE;
					}
					if((EcuComRxBuf[Frame_Control]&0x0f)==0)//继续收下一帧数据
					{
						break;
					}
					if((EcuComRxBuf[Frame_Control]&0x0f)==1)//等待，继续收30确认帧　
					{
						continue;
					}
					if((EcuComRxBuf[Frame_Control]&0x0f)==2)//溢出，出错退出
					{
						return FALSE;
					}
				}

				CF_CNT   = 0x00;
				FC_BS    = EcuComRxBuf[Frame_Control+1];	//连续帧的最大帧个数
				FC_STmin = EcuComRxBuf[Frame_Control+2];	//连续帧的帧间的最小时间间隔
			}
			PassThru_CAN_TxMuchPack_DelayTime(FC_STmin);
			//DelayNus(500);
			if(FC_BS == 0 || FC_BS > CF_CNT)
			{
				if(PackLen > DataLen)
					CF_DL = DataLen;
				else
					CF_DL = PackLen;
				PackLen -= CF_DL;
				//CF_SN++;
				DataBuf[0] = (uint8)CF_SN++;
				CF_SN &= 0x2f;
				
				memcpy(DataBuf+1, TXdata, CF_DL);
				if(!CAN_TxData((uint8 *)DataBuf, CF_DL+1, CanID))
				{
					return FALSE;
				}
				if(CF_SN==0X24)
					CF_SN=CF_SN;
				CF_CNT++;
				TXdata += CF_DL;
			}
		}
		return TRUE;
	}
	return FALSE;
}


/*********************************************************************************************
* 名称    ： PassThru_CAN_TxPack_J1939_SendOnly
* 功能    ： CANBUS协议发送数据 带扩展地址
* 入口参数：
*
*
* 出口参数：
* 说明    ：0x84 多帧数据自动处理控制帧 带扩展地址
*
单帧数据
rece:a5 a5 00 0d 30 98 ef 00 f9 43 00 00 00 00 00 02 ff fe
rece:a5 a5 00 0d 30 98 ef f9 00 44 00 00 02 58 58 ff ff 4e
广播数据
rece:a5 a5 00 0d 30 98 ec ff 3d 20 1e 00 05 ff ca fe 00 f8
rece:a5 a5 00 0d 30 98 eb ff 3d 01 04 ff 32 f0 e5 00 2c cc
rece:a5 a5 00 0d 30 98 eb ff 3d 02 f0 e5 00 2a f0 e5 07 26
rece:a5 a5 00 0d 30 98 eb ff 3d 03 2e f0 e5 00 6f f0 f3 ab
rece:a5 a5 00 0d 30 98 eb ff 3d 04 00 26 0b 13 00 26 0b 8a
rece:a5 a5 00 0d 30 98 eb ff 3d 05 13 00 ff ff ff ff ff f0
***********************************************************************************************/
uint8 PassThru_CAN_TxPack_J1939_SendOnly(uint8 *TXdata)
{
	uint8 result;
	//uint8 FC_BS;
	uint8 FC_STmin;
	//uint8 CF_SN; //连续帧序号
	//uint16 CF_DL; //连续帧长度
	//uint16 CF_CNT;//连续帧计数
	uint8 temp0, temp1;
	uint16 PackLen;
	//uint8  Frame_Control = 0x07;//每帧数据控制位起始位置
	uint32 CanID = PassThruConfig.CanTD;

	uint16 ByteNum = 0;
	uint16 Frame_Count;//多帧数据帧长度
	uint16 Data_Count;//多帧数据长度
	uint16 Frame_Start;//单次连续多帧计数
	uint16 Frame_End;//全部连续多帧计数
	uint8  Priority;
	//uint32 PGN;
	//uint32 ReadID;
	//uint16 count;
	uint16 number;
	//uint8  PGN_KEY;
	//uint8  Address_Target;
	uint8  Address_Source;
	uint8  Frame_FlagSN;//连续帧标志位
	// uint8  Frame_Watch = FALSE;

	temp0 = *TXdata++;
	temp1 = *TXdata++;
	PackLen = (temp0<<8) + temp1;
	temp0 = *TXdata++;//CANBUS ORD 0x30
	Priority = (CanID&0x1C)/4;
	CanID = *TXdata++;             //AR1_H
	CanID = (CanID << 8) + (*TXdata++);
	CanID = (CanID << 8) + (*TXdata++);
	CanID = (CanID << 8) + (*TXdata++); //AR1_L3
	PackLen -= 5;//去掉头和CANID
	//a5 a5 00 0d 30 98 ef 00 f9 43 00 00 00 00 00 02 ff fe
	if(PackLen < 0x09 )//只是一帧，直接发出后退出
	{
		result = CAN_TxData((uint8 *)TXdata, PackLen, CanID);
		return result;
	}
	//打包的多帧数据 自动处理控制帧
	//a5 a5 00 0d 30 98 ec ff 3d 20 1e 00 05 ff ca fe 00 f8
	Address_Source = (CanID)&0xff;
	Data_Count = PackLen;
	Frame_Count = (PackLen+6)/7;
	Frame_Start = 0x01;
	Frame_End = Frame_Count + Frame_Start;

	//a5 a5 00 0d 30 98 ec 00 f9 10 09 00 02 ff 00 ef 00 3c
	ByteNum = 0;
	EcuComTxBuf[ByteNum++] = 0x20;
	EcuComTxBuf[ByteNum++] = Data_Count%0x100;
	EcuComTxBuf[ByteNum++] = Data_Count/0x100;
	EcuComTxBuf[ByteNum++] = Frame_Count;
	EcuComTxBuf[ByteNum++] = 0xff;
	EcuComTxBuf[ByteNum++] = (CanID>>8)&0xff;
	EcuComTxBuf[ByteNum++] = (CanID>>16)&0xff;;
	EcuComTxBuf[ByteNum++] = (CanID>>24)&0x01;;
	CanID = 0x80ecff00 + (Priority<<26) + Address_Source;
	//CanID = 0x98ecff00 + Address_Source;
	if(!CAN_TxData(EcuComTxBuf, ByteNum, CanID))
	{
		return FALSE;
	}
	//a5 a5 00 0d 30 98 eb 00 f9 01 81 02 01 01 ff 01 ff c1
	//a5 a5 00 0d 30 98 eb 00 f9 02 00 00 ff ff ff ff ff 49
	for(Frame_FlagSN=Frame_Start; Frame_FlagSN<Frame_End; Frame_FlagSN++)
	{
		ByteNum = 0;
		EcuComTxBuf[ByteNum++] = Frame_FlagSN;
		EcuComTxBuf[ByteNum++] = *TXdata++;
		EcuComTxBuf[ByteNum++] = *TXdata++;
		EcuComTxBuf[ByteNum++] = *TXdata++;
		EcuComTxBuf[ByteNum++] = *TXdata++;
		EcuComTxBuf[ByteNum++] = *TXdata++;
		EcuComTxBuf[ByteNum++] = *TXdata++;
		EcuComTxBuf[ByteNum++] = *TXdata++;
		CanID = 0x80ebff00 + (Priority<<26) + Address_Source;
		//CanID = 0x98ebff00 + Address_Source;
		//最后一帧补充0xff
		if(Frame_FlagSN == Frame_Count)
		{
			number = Frame_Count*7 - Data_Count;
			memset(EcuComTxBuf+8-number, 0xff, number);
		}
		if(PassThruConfig.ISO15765STmin > 0)
		{
			FC_STmin = PassThruConfig.ISO15765STmin[PassThruConfig.Send_Pass];
			PassThru_CAN_TxMuchPack_DelayTime(FC_STmin);//帧间间隔时间 0f参数
		}
		if(!CAN_TxData((uint8 *)EcuComTxBuf, ByteNum, CanID))
		{
			return FALSE;
		}
	}
	return TRUE;
}
/*********************************************************************************************
* 名称    ： PassThru_CAN_TxPack_Auto_Ext
* 功能    ： CANBUS协议发送数据 带扩展地址
* 入口参数：
*
*
* 出口参数：
* 说明    ：0x83 多帧数据自动处理控制帧 带扩展地址
*
***********************************************************************************************/
uint8 PassThru_CAN_TxPack_Auto_Ext(uint8 *TXdata)
{
	uint8 result;
	uint8 FC_BS;
	uint8 FC_STmin;
	uint16 CF_SN; //连续帧序号
	uint16 CF_DL; //连续帧长度
	uint16 CF_CNT;//连续帧计数
	uint8 temp0, temp1;
	uint16 PackLen;
	uint32 CanID;
	uint8  DataLen = 6;//每次发送数据长度
	uint8  DataBuf[8];
	uint8  Frame_Control = 0x08;//每帧数据控制位起始位置
	uint8  Module_Address = PassThruConfig.TxAddress;

	temp0 = *TXdata++;
	temp1 = *TXdata++;
	PackLen = (temp0<<8) + temp1;
	temp0 = *TXdata++;//CANBUS ORD 0x30
	CanID = *TXdata++;             //AR1_H
	CanID = (CanID << 8) + (*TXdata++);
	CanID = (CanID << 8) + (*TXdata++);
	CanID = (CanID << 8) + (*TXdata++); //AR1_L3
	PassThruConfig.CanLastTD[PassThruConfig.Send_Pass]=CanID;
	PassThruConfig.CanExtID=*TXdata;			//记下扩展id
	PackLen -= 5;//去掉头和CANID
	if(PackLen < 0x09)//单帧
	{
		result = CAN_TxData((uint8 *)TXdata, PackLen, CanID);
		return result;
	}
	if(PackLen > 0x08 && (*(TXdata+1) & 0xf0) == CAN_15765_FF )//&&  PassThruConfig.CanAnsR == 0x02)//PassThruConfig.CanAnsT == 0x02 )//多帧第一帧
	{
		Module_Address = *TXdata;//从发送命令获取模块ID
		//发送第一帧数据
		CF_DL = 8;
		if(!CAN_TxData((uint8 *)TXdata, CF_DL, CanID))
		{
			return FALSE;
		}
		PackLen -= CF_DL;
		TXdata += CF_DL;
		CF_SN    = 0x21;
		CF_CNT   = 0x01;
		FC_BS    = 0x01;	//连续帧的最大帧个数
		FC_STmin = 0x00;	//连续帧的帧间的最小时间间隔
		while(PackLen > 0)
		{
			if(FC_BS == CF_CNT)
			{
				//流控制帧 00 0d 30 00 00 07 e8 30 08 0a 00 00 00 00 00
				while(1)
				{
					if(!PassThru_CAN_RxData_Time((uint8 *)EcuComRxBuf))
					{
						return FALSE;
					}
					if((EcuComRxBuf[Frame_Control]&0xf0) != CAN_15765_FC)
					{
						return FALSE;
					}
					if((EcuComRxBuf[Frame_Control]&0x0f)==0)//继续收下一帧数据
					{
						break;
					}
					if((EcuComRxBuf[Frame_Control]&0x0f)==1)//等待，继续收30确认帧　
					{
						continue;
					}
					if((EcuComRxBuf[Frame_Control]&0x0f)==2)//溢出，出错退出
					{
						return FALSE;
					}
				}
				CF_CNT   = 0x00;
				FC_BS    = EcuComRxBuf[Frame_Control+1];	//连续帧的最大帧个数
				FC_STmin = EcuComRxBuf[Frame_Control+2];	//连续帧的帧间的最小时间间隔
			}
			PassThru_CAN_TxMuchPack_DelayTime(FC_STmin);

			if(FC_BS == 0 || FC_BS > CF_CNT)
			{
				if(PackLen > DataLen)
					CF_DL = DataLen;
				else
					CF_DL = PackLen;
				PackLen -= CF_DL;
				DataBuf[0] = Module_Address;
				DataBuf[1] = CF_SN++;
				CF_SN &= 0x2f;
				memcpy(DataBuf+2, TXdata, CF_DL);
				if(!CAN_TxData((uint8 *)DataBuf, CF_DL+2, CanID))
				{
					return FALSE;
				}
				CF_CNT++;
				TXdata += CF_DL;
			}
		}
		return TRUE;
	}
	return FALSE;
}
/*********************************************************************************************
* 名称    ： PassThru_CAN_TxPack_GetID
* 功能    ： CANBUS协议发送一帧数据
* 入口参数：
*
*
* 出口参数：
* 说明    ： 0x80 发送一帧数据
*
***********************************************************************************************/
uint8 PassThru_CAN_TxPack_GetID(uint8 *TXdata)
{
	uint8 result;
	uint8 temp0, temp1;
	uint16 PackLen;
	uint32 CanID;

	temp0 = *TXdata++;
	temp1 = *TXdata++;
	PackLen = (temp0 << 8) + temp1;
	temp0 = *TXdata++;//CANBUS ORD 0x30
	CanID = *TXdata++;             //AR1_H
	CanID = (CanID << 8) + (*TXdata++);
	CanID = (CanID << 8) + (*TXdata++);
	CanID = (CanID << 8) + (*TXdata++); //AR1_L3
	PackLen -= 5;//去掉头和CANID
	if (PackLen < 0x09)//
	{
		result = CAN_TxData((uint8 *) TXdata, PackLen, CanID);
		return result;
	}
	return FALSE;
}

/*********************************************************************************************
* 名称    ： PassThru_CAN_TxPack_Normal
* 功能    ： CANBUS协议发送一帧数据
* 入口参数：
*
*
* 出口参数：
* 说明    ： 0x81 发送一帧数据
*
***********************************************************************************************/
uint8 PassThru_CAN_TxPack_Normal(uint8 *TXdata)
{
	uint8 result;
	uint8 FC_BS;
	uint8 FC_STmin;
	//uint8 CF_SN; //连续帧序号
	uint16 CF_DL; //连续帧长度
	uint16 CF_CNT;//连续帧计数
	uint8 temp0, temp1;
	uint16 PackLen;
	uint8  Frame_Control = 0x07;//每帧数据控制位起始位置
	uint32 CanID = PassThruConfig.CanTD;

	temp0 = *TXdata++;
	temp1 = *TXdata++;
	PackLen = (temp0<<8) + temp1;
	temp0 = *TXdata++;//CANBUS ORD 0x30或x037
	CanID = *TXdata++;             //AR1_H
	CanID = (CanID << 8) + (*TXdata++);
	CanID = (CanID << 8) + (*TXdata++);
	CanID = (CanID << 8) + (*TXdata++); //AR1_L3
	PackLen -= 5;//去掉头和CANID
	if (PackLen < 0x09)//只是一帧，直接发出后退出
	{
		result = CAN_TxData((uint8 *) TXdata, PackLen, CanID);
		return result;
	}
	if(PassThruConfig.CanAnsT == 0x00)
	{
		return FALSE;
	}
	if(PassThruConfig.CanAnsT == 0x01)
	{
		Frame_Control = 0x07;
	}
	if(PassThruConfig.CanAnsT == 0x02)
	{
		Frame_Control = 0x08;
	}
	//打包的多帧数据 自动确认30发送
	if((PassThruConfig.CanAnsT == 0x01 && (*TXdata & 0xf0) == CAN_15765_FF)
		||(PassThruConfig.CanAnsT == 0x02 && (*(TXdata+1) & 0xf0) == CAN_15765_FF))
	{
		CF_CNT   = 0x00;
		FC_BS    = 0x01;	//连续帧的最大帧个数
		FC_STmin = 0x00;	//连续帧的帧间的最小时间间隔
		while(PackLen > 0)
		{

			if(FC_BS == CF_CNT)//收确认帧
			{
				//流控制帧 00 0d 30 00 00 07 e8 30 08 0a 00 00 00 00 00
				while(1)
				{
					if(!PassThru_CAN_RxData_Time((uint8 *)EcuComRxBuf))
					{
						return FALSE;
					}
					if((EcuComRxBuf[Frame_Control]&0xf0) != CAN_15765_FC)
					{
						return FALSE;
					}
					if((EcuComRxBuf[Frame_Control]&0x0f)==0)//继续收下一帧数据
					{
						break;
					}
					if((EcuComRxBuf[Frame_Control]&0x0f)==1)//等待，继续收30确认帧　
					{
						continue;
					}
					if((EcuComRxBuf[Frame_Control]&0x0f)==2)//溢出，出错退出
					{
						return FALSE;
					}
				}
				CF_CNT   = 0x00;
				FC_BS    = EcuComRxBuf[Frame_Control+1];	//连续帧的最大帧个数
				FC_STmin = EcuComRxBuf[Frame_Control+2];	//连续帧的帧间的最小时间间隔
			}
			PassThru_CAN_TxMuchPack_DelayTime(FC_STmin);
			if(FC_BS == 0 || FC_BS > CF_CNT)
			{
				if(PackLen > 0x08)
					CF_DL = 0x08;
				else
					CF_DL = PackLen;
				PackLen -= CF_DL;
				if(!CAN_TxData((uint8 *)TXdata, CF_DL, CanID))
				{
					return FALSE;
				}
				CF_CNT++;
				TXdata += CF_DL;
			}
		}
		return TRUE;
	}
	return FALSE;
}



extern int16 EcuComRxBuf_MultipleRxNum;
extern int16 EcuComRxBuf_MultipleTxNum;
extern uint16 EcuComRxBuf_MultipleLen;




void UartTxQueuePackDebug(uint8* EcuComRxBuf)
{
	uint8_t checkbyte;
	uint16_t i;
	uint16_t number = 0;


	checkbyte = 0x00;
	number = (uint16_t)(EcuComRxBuf[2]<<8) +(uint16_t)(EcuComRxBuf[3]) + 5;
	for(i=2; i<number-1; i++)
	{
		checkbyte += EcuComRxBuf[i] ;
	}
	EcuComRxBuf[number-1] = ~checkbyte;
	USART_DMA_Send_ext(EcuComRxBuf, number);
}



uint8 PassThru_CAN_TxPack_Normal_Multiple(uint8 *TXdata)
{
 
#if 0
    uint8 temp0, temp1;
    uint16 PackLenTotal;

    uint8 normaldata[4096];
    uint32 i=0;
    uint8 multiple_singleframetype;
    uint16 multiple_singleframelen;
    uint8 multiple_singleframenum;
    EcuComRxBuf_MultipleLen=0;
    memset(normaldata,0,sizeof(normaldata));
	temp0 = *TXdata++;
	temp1 = *TXdata++;
     
    
	PackLenTotal = (temp0<<8) + temp1;

	temp0 = *TXdata++;//CANBUS ORD 0x30或x037

	multiple_singleframenum = *TXdata++; //total //

	EcuComRxBuf_MultipleTxNum = multiple_singleframenum;
    EcuComRxBuf_MultipleRxNum = multiple_singleframenum;
    
     
     //len+cmd +total  + [singleframetype+len+cmd+data]
     //                       + [singleframetype+len+cmd+data]
     //                       + [singleframetype+len+cmd+data]

     //a5 a5 00 12 30 01 00 [00 0d 30 00 00 07 df 02 09 02 00 00 00 00 00] cf //
     
        PackLenTotal -=2;

        while(PackLenTotal) 
        {
            
            multiple_singleframetype = *TXdata++;
            
            if(multiple_singleframetype==0) 
            {

							  
                normaldata[0]=0xa5;
                normaldata[1]=0xa5; //head
                
                multiple_singleframelen = *TXdata++*256;
                multiple_singleframelen += *TXdata++;


                 normaldata[2]=multiple_singleframelen>>8;
                 normaldata[3]=multiple_singleframelen; //len
							
							 
                 

                memcpy(&normaldata[4],TXdata,multiple_singleframelen); //cmd+data

                TXdata += multiple_singleframelen;
                

                PackLenTotal -= 1;//multiple_singleframetype
                
                PackLenTotal -= 2;//multiple_singleframelen

                PackLenTotal -= multiple_singleframelen;//multiple_singleframelen data

                
                //UartTxQueuePackDebug(normaldata);
                if (TRUE == PassThru_CAN_TxPack_Normal(normaldata+2))
                {

                }
                else
                {
                    EcuComRxBuf_MultipleTxNum=0;
                    break;
                }
                
           }
            else if(multiple_singleframetype==1)
            {


                    normaldata[0]=0xa5;
                normaldata[1]=0xa5; //head
                
                multiple_singleframelen = *TXdata++*256;
                multiple_singleframelen += *TXdata++;


                 normaldata[2]=multiple_singleframelen>>8;
                 normaldata[3]=multiple_singleframelen; //len
                 

                memcpy(&normaldata[4],TXdata,multiple_singleframelen); //cmd+data

                TXdata += multiple_singleframelen;
                

                PackLenTotal -= 1;//multiple_singleframetype
                
                PackLenTotal -= 2;//multiple_singleframelen

                PackLenTotal -= multiple_singleframelen;//multiple_singleframelen data
                PackLenTotal -=2;
                if (TRUE==PassThru_CAN_TxPack_Normal(normaldata+2))
                {

                }
                else
                {
                    EcuComRxBuf_MultipleTxNum=0;
                    break;
                }

            }
            else
                
                {
                    EcuComRxBuf_MultipleTxNum=0;

                    break;
                }

            
        }
        
    
#endif
}



/*********************************************************************************************
* 名称    ： PassThru_CAN_TxPack_NotID
* 功能    ： CANBUS协议发送数据 不带ID
* 入口参数：
*
*
* 出口参数：
* 说明    ： 0x7F 多帧数据自动处理控制帧
*
***********************************************************************************************/
uint8 PassThru_CAN_TxPack_NotID(uint8 *TXdata)
{
	uint8 result;
	uint8 FC_BS;
	uint8 FC_STmin;
	//uint8 CF_SN; //连续帧序号
	uint16 CF_DL; //连续帧长度
	uint16 CF_CNT;//连续帧计数
	uint8 temp0, temp1;
	uint8 check_flag;
	uint16 PackLen;
	uint8  Frame_Control = 0x07;//每帧数据控制位起始位置
	uint32 CanID = PassThruConfig.CanTD;

	temp0 = *TXdata++;
	temp1 = *TXdata++;
	PackLen = (temp0<<8) + temp1;
	temp0 = *TXdata++;//CANBUS ORD 0x30
	PackLen -= 1;//去掉头和CANID
	PassThruConfig.CanLastTD[PassThruConfig.Send_Pass] = CanID;
	//只有一帧数据 直接发送
	if(PackLen < 0x09)
	{
		result = CAN_TxData((uint8 *)TXdata, PackLen, CanID);
		return result;
	}
	if(PassThruConfig.CanAnsT == 0x00)
	{
		return FALSE;
	}
	check_flag = *TXdata;
	if(PassThruConfig.CanAnsT == 0x01)
	{
		check_flag = *TXdata;
		Frame_Control = 0x07;
	}
	else if(PassThruConfig.CanAnsT == 0x02)
	{
		check_flag = *(TXdata+1);
		Frame_Control = 0x08;
	}
	else
	{
		check_flag = *TXdata;
		Frame_Control = 0x07;
	}
	//打包的多帧数据 自动确认30发送
	//if((PassThruConfig.CanAnsT == 0x01 && (*TXdata & 0xf0) == CAN_15765_FF)
	//      ||(PassThruConfig.CanAnsT == 0x02 && (*(TXdata+1) & 0xf0) == CAN_15765_FF))
	if((PassThruConfig.CanAnsT > 0 && (check_flag & 0xf0) == CAN_15765_FF))
	{
		CF_CNT   = 0x00;
		FC_BS    = 0x01;	//连续帧的最大帧个数
		FC_STmin = 0x00;	//连续帧的帧间的最小时间间隔
		while(PackLen > 0)
		{
			if(FC_BS == CF_CNT)
			{
				//流控制帧 00 0d 30 00 00 07 e8 30 08 0a 00 00 00 00 00
				while(1)
				{
					if(!PassThru_CAN_RxData_Time((uint8 *)EcuComRxBuf))
					{
						return FALSE;
					}
					if((EcuComRxBuf[Frame_Control]&0xf0) != CAN_15765_FC)//不是流控制帧　　
					{
						return FALSE;
					}
					if((EcuComRxBuf[Frame_Control]&0x0f)==0)//继续收下一帧数据
					{
						break;
					}
					if((EcuComRxBuf[Frame_Control]&0x0f)==1)//等待，继续收30确认帧　
					{
						continue;
					}
					if((EcuComRxBuf[Frame_Control]&0x0f)==2)//溢出，出错退出
					{
						return FALSE;
					}
				}
				CF_CNT   = 0x00;
				FC_BS    = EcuComRxBuf[Frame_Control+1];	//连续帧的最大帧个数
				FC_STmin = EcuComRxBuf[Frame_Control+2];	//连续帧的帧间的最小时间间隔
			}
			PassThru_CAN_TxMuchPack_DelayTime(FC_STmin);
			if(FC_BS == 0 || FC_BS > CF_CNT)
			{
				if(PackLen > 0x08)
					CF_DL = 0x08;
				else
					CF_DL = PackLen;
				PackLen -= CF_DL;
				if(!CAN_TxData((uint8 *)TXdata, CF_DL, CanID))
				{
					return FALSE;
				}
				CF_CNT++;
				TXdata += CF_DL;
			}
		}
		return TRUE;
	}
	return FALSE;
}

/*********************************************************************************************
* 名称    ： PassThru_CAN_TxPack_FlowControl
* 功能    ： CANBUS协议发送控制帧
* 入口参数：
*
*
* 出口参数：
* 说明    ： Listen/CanAnsT/Address/ISO15765BS/ISO15765STmin
*
***********************************************************************************************/
uint8 PassThru_CAN_TxPack_FlowControl(uint8 *TXdata)
{
	uint8 temp0, temp1;
	uint16 PackLen;
	uint32 CanID;
	uint32 tempID;

	PassThru_CAN_TxMuchPack_DelayTime(0);			//by changyl:在测孔会计车时，会因为发太快而引起车上不回应，增加此延时:2015/7/6
	if(PassThruConfig.CanAnsR > 0 && PassThruConfig.Listen == 0x7f)
	{
		temp0 = PassThruConfig.SendBuf[1];
		temp1 = PassThruConfig.SendBuf[2];
		PackLen = (temp0<<8) + temp1;
		CanID = PassThruConfig.CanTD;
		memcpy(EcuComTxBuf, PassThruConfig.SendBuf+4, 8);
		if(!CAN_TxData(EcuComTxBuf, PackLen-1, CanID))
		{
			return FALSE;
		}
	}
	else if(PassThruConfig.CanAnsR > 0 && PassThruConfig.Listen == 0x81)
	{
		temp0 = PassThruConfig.SendBuf[1];
		temp1 = PassThruConfig.SendBuf[2];
		PackLen = (temp0<<8) + temp1;
		CanID = (PassThruConfig.SendBuf[4]<<24) + (PassThruConfig.SendBuf[5]<<16) + (PassThruConfig.SendBuf[6]<<8) + PassThruConfig.SendBuf[7];
		memcpy(EcuComTxBuf, PassThruConfig.SendBuf+8, 8);
		if(!CAN_TxData(EcuComTxBuf, PackLen-5, CanID))
		{
			return FALSE;
		}
	}

	else if(PassThruConfig.CanAnsR > 0 && PassThruConfig.Listen == 0x8F)
	{
		temp0 = PassThruConfig.SendBuf[1];
		temp1 = PassThruConfig.SendBuf[2];
		PackLen = (temp0<<8) + temp1;
		CanID = (PassThruConfig.SendBuf[4]<<24) + (PassThruConfig.SendBuf[5]<<16) + (PassThruConfig.SendBuf[6]<<8) + PassThruConfig.SendBuf[7];
		memcpy(EcuComTxBuf, PassThruConfig.SendBuf+8, 8);
		if(!CAN_TxData(EcuComTxBuf, PackLen-5, CanID))
		{
			return FALSE;
		}
	}

    
	else if(PassThruConfig.Listen==0x82 || PassThruConfig.Listen==0x88)//PassThruConfig.CanAnsR == 0x01)//CanAnsT == 0x01)
	{
		EcuComTxBuf[0] = 0x30;
		EcuComTxBuf[1] = PassThruConfig.ISO15765BS[PassThruConfig.Send_Pass];
		EcuComTxBuf[2] = PassThruConfig.ISO15765STmin[PassThruConfig.Send_Pass];
		CanID=PassThruConfig.CanLastTD[PassThruConfig.Send_Pass];
		if(CanID==0x07DF)//增加在0x82的情况下，自动回答确认帧;直接在回应的数据在07e8和07ef之间时，减08回确认
		{
			tempID=(TXdata[3]<<24)+(TXdata[4]<<16)+(TXdata[5]<<8)+TXdata[6];
			if((tempID>0x07e7)&&(tempID<0X07F0))
			{
				CanID=tempID-8;//PassThruConfig.TxAddress;
			}
		}
		else if(((CanID&0x01fffff0)==0x00DBFFF0)||((CanID&0x01fffff0)==0x00DB33F0))
		{
			CanID=(TXdata[3]<<24)+(TXdata[4]<<16)+(TXdata[6]<<8)+TXdata[5];
		}
		if(!CAN_TxData(EcuComTxBuf, 3, CanID))
		{
			return FALSE;
		}
	}
	else if(PassThruConfig.Listen==0x83 || PassThruConfig.Listen==0x89)//PassThruConfig.CanAnsR==0x02)//CanAnsT == 0x02)
	{
		//增加在0x83的情况下，自动回答确认帧; 00 0d 30 00 00 06 78 f1 10 21 62 d8 6f 00 00 回应 00 ?? 30 00 00 06 f1 78 30 00 *
		/*有三种情况　1.宝马发 a5 a5 00 ?? 30 00 00 07 df 78 03 22 d8 6f *汽车会有多系统回应
		a5 a5 00 0a 30 00 00 06 78 f1 10 *
		a5 a5 00 0a 30 00 00 06 88 f1 10 *回应，故此时要根据回应的id和第二id发确认帧
		2.丰田 仪器:00 0d 30 00 00 06 78 f1 10 21 62 d8 6f 00 00
		汽车:00 0a 30 00 00 06 58 f1 10 *   此时第二id和发送相同
		3.克莱斯勒 仪器:00 0d 30 00 00 06 78 f1 10 21 62 d8 6f 00 00
		汽车:00 0a 30 00 00 06 58 aa 10 *   此时第二id是任意数，故在初始化io中设置　*/
		CanID=PassThruConfig.CanLastTD[PassThruConfig.Send_Pass];
		if((CanID==0x06F1)&&((PassThruConfig.CanExtID==0xDF)||(PassThruConfig.CanExtID==0xEF)))
		{
			CanID=(TXdata[3]<<24)+(TXdata[4]<<16)+(TXdata[5]<<8)+TXdata[7];
			EcuComTxBuf[0] = TXdata[6];//PassThruConfig.TxAddress;
		}
		else
		{
			//CanID=(TXdata[3]<<24)+(TXdata[4]<<16)+(TXdata[5]<<8)+TXdata[7];
			CanID=PassThruConfig.CanLastTD[PassThruConfig.Send_Pass];
			EcuComTxBuf[0]=PassThruConfig.CanExtID;//PassThruConfig.TxAddress;
		}
		EcuComTxBuf[1] = 0x30;
		EcuComTxBuf[2] = PassThruConfig.ISO15765BS[PassThruConfig.Send_Pass];
		EcuComTxBuf[3] = PassThruConfig.ISO15765STmin[PassThruConfig.Send_Pass];
		if(!CAN_TxData(EcuComTxBuf, 4, CanID))//PassThruConfig.CanTD))
		{
			return FALSE;
		}
	}
	return TRUE;
}


/*********************************************************************************************
* 名称    ： PassThru_CAN_TxPack_Head
* 功能    ： CANBUS协议发送多帧数据
* 入口参数：
*
*
* 出口参数：
* 说明    ： 0x8A 发送多帧数据
*
***********************************************************************************************/
uint8 PassThru_CAN_TxPack_Head(uint8 *TXdata)
{
	uint8 result;
	uint8 temp0, temp1;
	uint16 PackLen;
	uint16 nCount;
	uint32 CanID = PassThruConfig.CanTD;

	temp0 = *TXdata++;
	temp1 = *TXdata++;
	PackLen = (temp0<<8) + temp1;
	temp0 = *TXdata++;//CANBUS ORD 0x30
	CanID = *TXdata++;             //AR1_H
	CanID = (CanID << 8) + (*TXdata++);
	CanID = (CanID << 8) + (*TXdata++);
	CanID = (CanID << 8) + (*TXdata++); //AR1_L3
	PackLen -= 5;//去掉头和CANID
	nCount = 0;
	while(nCount+8 < PackLen)
	{
		result = CAN_TxData((uint8 *)TXdata, 8, CanID);
		nCount += 8;
		TXdata += 8;
	}
	nCount = PackLen - nCount;
	result = CAN_TxData((uint8 *)TXdata, nCount, CanID);
	return result;
}

/*********************************************************************************************
* 名称    ： PassThru_CAN_RxPack_Head
* 功能    ： CANBUS协议接收多帧数据
* 入口参数：
*
*
* 出口参数：
* 说明    ：0x8A 返回完整帧 带ID
*
***********************************************************************************************/
uint8 PassThru_CAN_RxPack_Head()
{
	uint32 CANID, CANID2;
	uint16 nCount;
	uint16 ByteNum = 0;
	uint8  Frame_Control = 0x07;//每帧数据控制位位置
	//uint8  Frame_DataNum = 0x08;//每帧数据有效数据个数

	if(!PassThru_CAN_RxData((uint8 *)EcuComRxBuf) == TRUE)
	{
		return FALSE;
	}
	ByteNum = (EcuComRxBuf[0]<<8) + EcuComRxBuf[1];
	if(ByteNum < 5 || ByteNum > 0x0d)
	{
		return FALSE;
	}
	nCount = ByteNum - 5;
	CANID = (EcuComRxBuf[3]<<24) + (EcuComRxBuf[4]<<16) + (EcuComRxBuf[5]<<8) + EcuComRxBuf[6];
	while(nCount == 8 && ByteNum < 0x100 && PassThru_CAN_RxData_Time((uint8 *)EcuComTxBuf) == TRUE)
	{
		nCount = (EcuComTxBuf[0]<<8) + EcuComTxBuf[1];
		if(nCount < 5 || nCount > 0x0d)
		{
			break;
		}
		CANID2 = (EcuComTxBuf[3]<<24) + (EcuComTxBuf[4]<<16) + (EcuComTxBuf[5]<<8) + EcuComTxBuf[6];
		if(CANID2 != CANID)
		{
			break;
		}
		if(EcuComTxBuf[Frame_Control] == EcuComRxBuf[Frame_Control])
		{
			EcuComRxBuf[0] = ByteNum/0x100;
			EcuComRxBuf[1] = ByteNum%0x100;
			UartTxQueuePack(EcuComRxBuf);
			ByteNum = nCount;
			memcpy(EcuComRxBuf, EcuComTxBuf, nCount+2);
		}
		else
		{
			nCount -= 5;
			memcpy(EcuComRxBuf+ByteNum+2, EcuComTxBuf+Frame_Control, nCount);
			ByteNum += nCount;
		}
	}
	EcuComRxBuf[0] = ByteNum/0x100;
	EcuComRxBuf[1] = ByteNum%0x100;
	UartTxQueuePack(EcuComRxBuf);
	return TRUE;
}

/*********************************************************************************************
* 名称    ： PassThru_CAN_TxPack_SetFilter
* 功能    ： CANBUS协议设置接收过滤器
* 入口参数：
*
*
* 出口参数：
* 说明    ： 0x70专用命令 主要用于J1939协议读取数据流
*
***********************************************************************************************/
uint8 PassThru_CAN_TxPack_SetFilter(uint8 *TXdata)
{
	uint16 i;
	uint16 PackLen;

	PackLen = (TXdata[0] << 8) + TXdata[1];
	CanId_RxNum = (PackLen-1)/4;

	TXdata = TXdata + 3;

	for(i=0; i<CanId_RxNum; i++)
	{
		if(i == MAX_CANID_COUNT) break;
		CanId_RxBuf[i] = *TXdata++;
		CanId_RxBuf[i] = (CanId_RxBuf[i] << 8) + (*TXdata++);
		CanId_RxBuf[i] = (CanId_RxBuf[i] << 8) + (*TXdata++);
		CanId_RxBuf[i] = (CanId_RxBuf[i] << 8) + (*TXdata++);
	}
	for(i=CanId_RxNum; i<MAX_CANID_COUNT; i++)
	{
		CanId_RxBuf[i] = 0;
	}
	//改成硬件过滤
	CANx_MultiFilterMask(CAN1, FrameFormat[PassThruConfig.Send_Pass]);
	return TRUE;
}


/*********************************************************************************************
* 名称    ： PassThru_CAN_RxPack_J1939_ReadPGN
* 功能    ： 接收数据J1939总线数据，分析PGN
* 入口参数：
***********************************************************************************************/
uint8 PassThru_CAN_RxPack_J1939_ReadPGN()
{
	//uint8  RxFrame;
	uint8  CheckMID;
	uint32 loop;
	uint32 CanId_Code;

	CheckMID = 0xff;
	if (isHasData()) 
	{
		if(PassThru_CAN_RxData((uint8 *)EcuComRxBuf) == TRUE)
		{
			//00 0d 30 8c f0 04 00 fe 7d 7d 00 00 00 00 7d 
			CanId_Code = (EcuComRxBuf[3]<<24) + (EcuComRxBuf[4]<<16) + (EcuComRxBuf[5]<<8) + EcuComRxBuf[6];
			if(EcuComRxBuf[4] == 0xEC && EcuComRxBuf[7] == 0x20)//0xec首帧头帧，重新定时
			{
				//00 0d 30 98 ec ff 00 20 1e 00 05 ff ca fe 00
				CanId_Code = ((EcuComRxBuf[3]|EcuComRxBuf[0x0e])<<24) + (EcuComRxBuf[0x0d]<<16) + (EcuComRxBuf[0x0c]<<8) + EcuComRxBuf[6];
				if(CheckMID < 0xff) CheckMID = EcuComRxBuf[6];
				//RxFrame = EcuComRxBuf[0x0a];
				pgn_much_data_num=EcuComRxBuf[0x0a];
			}
			else if(EcuComRxBuf[4] == 0xE8 || EcuComRxBuf[4] == 0xEA)
			{
				//00 0d 30 98 ec ff 00 20 1e 00 05 ff ca fe 00
				//CanId_Code = ((EcuComRxBuf[3]|EcuComRxBuf[0x0e])<<24) + (EcuComRxBuf[0x0d]<<16) + (EcuComRxBuf[0x0c]<<8) + EcuComRxBuf[6];
				return TRUE;
			}
			else if(EcuComRxBuf[4] == 0xEB)//多帧中间帧，
			{
				//if(pgn_much_data_num==EcuComRxBuf[7])//多帧最后一帧，退
					//pgn_much_data_num-=EcuComRxBuf[7];//Int_time(2, 100);//增加定时器时间 继续接收后续数据
				if(pgn_much_data_num>0)
				pgn_much_data_num--;
				return TRUE;
			}
			CanId_Code&=0x03ffffff;
			for(loop=0; loop<CanId_RxNum; loop++)
			{
				if(CanId_Code == CanId_RxBuf[loop])
				{
					break;
				}
			}
			if(loop == CanId_RxNum)
			{
				CanId_RxBuf[loop] = CanId_Code;
				CanId_RxNum++;
			}
		}
	}

	return TRUE;
}
//pgn多帧发送，在发送时，需要把线上多帧的最后一帧收完后才能发送，不然可能不回应
uint8  PassThru_CAN_PGN_TxPack(uint8 *TXdata,uint8 TXLen, uint32 CanID)
{
	uint8 result=FALSE;
	uint16 delay_time;
  uint8 enter_pgn=0;
		_1msCounter=0;
		if(pgn_much_data_num==0)
			delay_time=1000;
		else
			delay_time=pgn_much_data_num*100;
		while(_1msCounter<delay_time)//1000)//TIME_INIT)
		{
			IWDG_Feed();
			PassThru_CAN_RxPack_J1939_ReadPGN();
			if(pgn_much_data_num==0)//多帧必须收到尾帧才能发送命令
			{
				//counter_1ms=0;
				_1msCounter=0;
					result = PassThru_CAN_TxData((uint8 *) TXdata, 3, CanID);
        enter_pgn++;

			}
			if(result)
			{
// 				result=FALSE;
 				break;//发送成功
			}
      if(enter_pgn>20)
        break;
    }
		
		if(_1msCounter>1000)
		{
			UARTAnswerErr(0xf3, 0x02);//没有收到完整的多帧的数据
		}
		return TRUE;
}

/*********************************************************************************************
* 名称    ： PassThru_CAN_TxPack_J1939_SendPGN
* 功能    ： 发送数据 J1939总线数据，分析PGN
* 入口参数：
***********************************************************************************************/
uint8 PassThru_CAN_TxPack_J1939_SendPGN(uint8 *TXdata)
{
	uint8 result=FALSE;
	uint16 TX_POS=0;
	uint8 i, number;
	uint8 temp0, temp1, checknum=0;
	uint16 PackLen;
	uint16 loop, nCount;
	uint32 CanID;
	temp0 = *TXdata++;
	temp1 = *TXdata++;
	PackLen = (temp0 << 8) + temp1;
	//CanTransMode = 
	*TXdata++;//CANBUS ORD 0x30
	CanID = *TXdata++;             //AR1_H
	CanID = (CanID << 8) + (*TXdata++);
	CanID = (CanID << 8) + (*TXdata++);
	CanID = (CanID << 8) + (*TXdata++); //AR1_L3
	PackLen -= 5;//去掉头和CANID
	if((CanID&0x00FF0000) == 0x00EA0000)
	{
		loop = 0;
		nCount = PackLen/3;
		while(loop<nCount)
		{
// 			if((loop%MAXSIZE_FTPGN) == 0)
// 			{
			IWDG_Feed();
			#if 0
				if((nCount-loop) < MAXSIZE_FTPGN) number = nCount-loop;
				else number = MAXSIZE_FTPGN;
			#endif
				loop+=number;
				CANx_MultiFilterMask_PGN( CAN1,TXdata+loop*3, number);
//			}
			for(i=0;i<number;i++)
			{
				
			//	UARTAnswerErr(loop, number);
				//等待100ms
				//PassThru_CAN_PGN_TxPack(TXdata+TX_POS,3,CanID);
				//PassThru_CAN_PGN_TxPack(TXdata+TX_POS,3,CanID);
				PassThru_CAN_TxData(TXdata+TX_POS, 3, CanID);//自己发送不判断多帧接收情况
				TX_POS+= 3;
				//loop++;
				counter_1ms=0;
				while(counter_1ms<20)
					PassThru_CAN_RxPack_J1939_ReadPGN();//等待接收
			}
		}
		nCount = CanId_RxNum*4 + 1;
		EcuComRxBuf[0]=nCount/0x100;
		EcuComRxBuf[1]=nCount%0x100;
		EcuComRxBuf[2]=CAN_DATA;//	
		checknum = CAN_DATA;
		for(loop=0; loop<CanId_RxNum; loop++)
		{
			EcuComRxBuf[3+loop*4]=CanId_RxBuf[loop]>>24;
			EcuComRxBuf[4+loop*4]=CanId_RxBuf[loop]>>16;
			EcuComRxBuf[5+loop*4]=CanId_RxBuf[loop]>>8;
			EcuComRxBuf[6+loop*4]=CanId_RxBuf[loop]&0xff;
			checknum+=EcuComRxBuf[3+loop*4]+EcuComRxBuf[4+loop*4]+EcuComRxBuf[5+loop*4]+EcuComRxBuf[6+loop*4];
		}
		memset(CanId_RxBuf,0,sizeof(CanId_RxBuf));
		CanId_RxNum=0;
		EcuComRxBuf[loop*4+3]=(~checknum)&0xff;
		Uart0TxQueuePack(EcuComRxBuf);//发送剩下的数据
	}
	else
		if (PackLen < 0x09)//
		{
			result = PassThru_CAN_PGN_TxPack((uint8 *) TXdata, PackLen, CanID);
			return result;
		}
		return TRUE;
}
/*********************************************************************************************
* 名称    ： PassThru_CAN_RxPack_Auto_GM
* 功能    ： 接收数据 自动回复控制帧
* 入口参数：
*
*
* 出口参数：
* 说明    ：0x82 多帧数据自动回复控制帧 带ID
*
***********************************************************************************************/
uint8 PassThru_CAN_RxPack_Auto_GM()
{
    uint16 Frame_Length;//多帧数据帧长度
    uint16 Frame_Count0;//多帧数据长度
    uint16 Frame_Count1;//单次连续多帧计数
    uint16 Frame_Count2;//全部连续多帧计数
    uint8  Frame_FlagSN;//连续帧标志位
    uint8  Frame_Control = 0x07;//每帧数据控制位位置
    uint8  Frame_DataNum = 0x07;//每帧数据有效数据个数
	uint16 missCount;

    if(PassThru_CAN_RxData((uint8 *)EcuComRxBuf) == TRUE)
    {
        PassThru_SetKeepComSwitch2(EcuComRxBuf+Frame_Control+1);
        Uart0TxQueuePack(EcuComRxBuf);
    }
    else
    {
        return FALSE;
    }
	//GM诊断模式 数据流故障码相关数据直接返回
	if(EcuComRxBuf[5] == 0x05)
	{
		return TRUE;
	}
    //接收多帧数据
    if((EcuComRxBuf[Frame_Control]&0xf0) == CAN_15765_FF)// && PassThruConfig.CanAnsR == 0x01)// PassThruConfig.CanAnst == 0x01)
    {
        Frame_Count0 = ((EcuComRxBuf[Frame_Control]&0x0f)<<8) + EcuComRxBuf[Frame_Control+1];
        Frame_FlagSN = 1;
        Frame_Count1 = 0;
        Frame_Count2 = 0;
        Frame_Length = Frame_Count0/Frame_DataNum;//流控制 连续帧 帧数
		missCount = 0;
        while(Frame_Count2 < Frame_Length)
        {
            if((Frame_Count1 )== 0x00 || Frame_Count1 == (PassThruConfig.ISO15765BS[PassThruConfig.Send_Pass]))
            {
                Frame_Count1 = 0;
                //发送控制帧
                if(!PassThru_CAN_TxPack_FlowControl((uint8 *)EcuComRxBuf))
                {
                    return FALSE;
                }
            }

            //接收连续数据 按照实际接收
            if(PassThru_CAN_RxData_Time((uint8 *)EcuComRxBuf) == FALSE)
            {
                return FALSE;
			}
			//GM诊断模式 数据流故障码相关数据直接返回
			if(EcuComRxBuf[5] == 0x05)
			{
				if(missCount++ > 100) return FALSE;
				continue;
			}
            Uart0TxQueuePack(EcuComRxBuf);
            if((EcuComRxBuf[Frame_Control]&0xf0) != CAN_15765_CF)
            {
                return FALSE;
            }
            if((EcuComRxBuf[Frame_Control]&0x0f) != Frame_FlagSN)
            {
                return FALSE;
            }
			missCount = 0;
            Frame_Count1++;
            Frame_Count2++;
            Frame_FlagSN++;
            if(Frame_FlagSN > 0x0f)
            {
                Frame_FlagSN = 0x00;
            }
        }
    }
    return TRUE;
}

/*********************************************************************************************
* 名称    ： PassThru_CAN_TxPack_Auto_GM
* 功能    ： CANBUS协议发送数据
* 入口参数：
*
*
* 出口参数：
* 说明    ：0x8C 多帧数据自动处理控制帧
*
***********************************************************************************************/
uint8 PassThru_CAN_TxPack_Auto_GM(uint8 *TXdata)
{
    uint8 result;
    uint8 FC_BS;
    uint8 FC_STmin;
    uint16 CF_SN; //连续帧序号
    uint16 CF_DL; //连续帧长度
    uint16 CF_CNT;//连续帧计数
    uint8 temp0, temp1;
    uint16 PackLen;
    uint32 CanID;
    uint8  DataLen = 7;//每次发送数据长度
    uint8  DataBuf[8];
    uint8  Frame_Control = 0x07;//每帧数据控制位起始位置
	uint16 missCount;

    temp0 = *TXdata++;
    temp1 = *TXdata++;
    PackLen = (temp0<<8) + temp1;
    temp0 = *TXdata++;//CANBUS ORD 0x30
    CanID = *TXdata++;             //AR1_H
    CanID = (CanID << 8) + (*TXdata++);
    CanID = (CanID << 8) + (*TXdata++);
    CanID = (CanID << 8) + (*TXdata++); //AR1_L3
    PassThruConfig.CanLastTD[PassThruConfig.Send_Pass]=CanID;
    PackLen -= 5;//去掉头和CANID
    if(PackLen < 0x09)//单帧
    {
        result = PassThru_CAN_TxData((uint8 *)TXdata, PackLen, CanID);
        return result;
    }

    if(PackLen > 0x08 && (*TXdata & 0xf0) == CAN_15765_FF )//&&  PassThruConfig.CanAnsR == 0x01)//PassThruConfig.CanAnsT == 0x01 )//多帧第一帧
    {
        //发送第一帧数据
        CF_DL = 8;
        if(!PassThru_CAN_TxData((uint8 *)TXdata, CF_DL, CanID))
        {
            return FALSE;
        }
        PackLen -= CF_DL;
        TXdata += CF_DL;
        CF_SN    = 0x21;
        CF_CNT   = 0x01;
        FC_BS    = 0x01;	//连续帧的最大帧个数
        FC_STmin = 0x00;	//连续帧的帧间的最小时间间隔
        while(PackLen > 0)
        {
            if(FC_BS == CF_CNT)
            {
				missCount = 0;
                //流控制帧 00 0d 30 00 00 07 e8 30 08 0a 00 00 00 00 00
                while(missCount < 100)
				{
					missCount++;
                    if(!PassThru_CAN_RxData_Time((uint8 *)EcuComRxBuf))
                    {
                        return FALSE;
					}
					//GM诊断模式 数据流故障码相关数据直接返回
					if(EcuComRxBuf[5] == 0x05)
					{
						continue;
					}
                    if((EcuComRxBuf[Frame_Control]&0xf0) != CAN_15765_FC)
                    {
                        return FALSE;
                    }
                    if((EcuComRxBuf[Frame_Control]&0x0f)==0)//继续收下一帧数据
                    {
                        break;
                    }
                    if((EcuComRxBuf[Frame_Control]&0x0f)==1)//等待，继续收30确认帧　
                    {
                        continue;
                    }
                    if((EcuComRxBuf[Frame_Control]&0x0f)==2)//溢出，出错退出
                    {
                        return FALSE;
                    }
                }

                CF_CNT   = 0x00;
                FC_BS    = EcuComRxBuf[Frame_Control+1];	//连续帧的最大帧个数
                FC_STmin = EcuComRxBuf[Frame_Control+2];	//连续帧的帧间的最小时间间隔
            }
            PassThru_CAN_TxMuchPack_DelayTime(FC_STmin);
            if(FC_BS == 0 || FC_BS > CF_CNT)
            {
                if(PackLen > DataLen)
                    CF_DL = DataLen;
                else
                    CF_DL = PackLen;
                PackLen -= CF_DL;
                DataBuf[0] = CF_SN++;
                CF_SN &= 0x2f;
                memcpy(DataBuf+1, TXdata, CF_DL);
                if(!PassThru_CAN_TxData((uint8 *)DataBuf, CF_DL+1, CanID))
                {
                    return FALSE;
                }
                CF_CNT++;
                TXdata += CF_DL;
            }
        }
        return TRUE;
    }
    return FALSE;
}


/*********************************************************************************************
* 名称    ： PassThru_CAN_TxPack_NotID
* 功能    ： CANBUS协议发送数据 不带ID
* 入口参数：
*
*
* 出口参数：
* 说明    ： 0x8d模式，can专用刷写
					上位机发送下来的有效数据如果小于等于8个，直接发送
           04 00 05 00 00 00 00 00 
					如多于8个，那一定为11个
           04 00 05 00 00 00 00 00 aa bb cc
　　　　　0xaabbcc表示本条命令要发送0xaa*(0xbbcc)次，每bbcc次把接收到的数据打包成一帧后发给上位机，一共发给上位机aa次
         
          例02 00 08表示要发送本条命令02*0x0008次，每8次把所有收到的8次数据打包给上位机，一共上传二次
					
*
***********************************************************************************************/
uint8 PassThru_CAN_TxPack_flash_read(uint8 *TXdata)
{
	uint8 result,temp0,temp1,check_sum;
	uint32 CanID = PassThruConfig.CanTD;
	uint16 i,PackLen,bufnum=0;
	uint16 patch_num,every_patch_send,patch_num_i,every_patch_send_i;
	temp0 = *TXdata++;
	temp1 = *TXdata++;
	PackLen = (temp0<<8) + temp1;
	temp0 = *TXdata++;//CANBUS ORD 0x30
	PackLen -= 1;//去掉头和CANID
	//只有一帧数据 直接发送
	if(PackLen <=0x09)
	{
		result = CAN_TxData((uint8 *)TXdata, PackLen, CanID);
		return result;
	}
	PackLen=0x08;//每次8个
	patch_num=TXdata[8];
	every_patch_send=TXdata[9]*256+TXdata[10];
	UartTxQueueBuf[bufnum++]=0xa5;
	UartTxQueueBuf[bufnum++]=0xa5;
	UartTxQueueBuf[bufnum++]=0x00;
	UartTxQueueBuf[bufnum++]=0x00;
	UartTxQueueBuf[bufnum++]=CAN_DATA;
	for(patch_num_i=0;patch_num_i<patch_num;patch_num_i++)//循环多少次，
	{
		UartTxQueueBuf[5]=patch_num_i;
		UartTxQueueBuf[4]=CAN_DATA;
		bufnum=6;
		for(every_patch_send_i=0;every_patch_send_i<every_patch_send;every_patch_send_i++)//一次打包多少帧上去
		{
			
					result = CAN_TxData((uint8 *)TXdata, PackLen, CanID);
					//return result;
					_1msCounter=0;
					while(_1msCounter<100)
					{
						result=PassThru_CAN_RxData_Time((uint8 *)EcuComRxBuf);
						if(result)
						{
							memcpy(UartTxQueueBuf+bufnum,EcuComRxBuf+7,8);
							bufnum+=8;
							break;
						}
					}
					if(_1msCounter>=100)
					{
						UARTAnswerErr(0x8d,patch_num_i);
						UARTAnswerErr((every_patch_send_i>>8)&0xff,every_patch_send_i&0xff);
						return result;
					}
		}
	bufnum-=2;
		UartTxQueueBuf[2]=(bufnum>>8)&0xff;
		UartTxQueueBuf[3]=bufnum&0xff;
		check_sum=0;
		for(i=2;i<bufnum+2;i++)
		{
			check_sum+=UartTxQueueBuf[i];
		}
		check_sum=~check_sum;

/*
 *********************************************
 * chenli_patch 1.2 start
 * 修复crc字节存储位置错误引起的CAN8D协议读euc flash失败
 * 测试方法:F7S蓝牙连接minivci->诊断->ECU刷写->博士->EDC17CV44/54刷写(CAN线)->
 *	->潍柴BOSCH->1M->读取
 *
 */		
		
		UartTxQueueBuf[bufnum+4]=check_sum;
		USART_DMA_Send_ext(UartTxQueueBuf,bufnum+5);
		
/*chenli_patch 1.2 end
 *********************************************	  
 */
		  
	}		
	return FALSE;
}


/*********************************************************************************************
* 名称    ： PassThru_CAN_TxPack_flash_read_pos
* 功能    ： CANBUS协议发送数据 不带ID
* 入口参数：
*
*
* 出口参数：
* 说明    ： 0x8e模式，can读取flash数据专用
					上位机发送下来的有效数据如果小于等于8个，直接发送
           07 3e ?? ?? 00 00 00 00 
					如多于8个，那一定为10个
            xx xx xx xx xx xx xx xx aa bb cc dd ee
						07 3e 09 ec 00 00 00 00 02 02 04 04 00
　　　　 aa bit7为1表示地址位是低位在前;bit7为零表示高位在前.bit6~bit0表示地址位的偏侈地址，第一个为xx地址为0
         bb 表示地址个数
         cc 表示两个地址的差额
         ddee表示要增加的次数
         
         
          
					
*
***********************************************************************************************/
uint8 PassThru_CAN_TxPack_flash_read_pos(uint8 *TXdata)
{
	uint8 result,temp0,temp1,check_sum;
	uint16 i,j,up_data_num;
	uint32 CanID = PassThruConfig.CanTD;
	uint16 PackLen,bufnum=0;
	uint16 addr_pos,addr_dir,addr_data_num,addr_sub,send_num;
	uint32 addr_start_data;
	temp0 = *TXdata++;
	temp1 = *TXdata++;
	PackLen = (temp0<<8) + temp1;
	temp0 = *TXdata++;//CANBUS ORD 0x30
	PackLen -= 1;//去掉头和CANID
	//只有一帧数据 直接发送
	if(PackLen <=0x09)
	{
		result = PassThru_CAN_TxData((uint8 *)TXdata, PackLen, CanID);
		return result;
	}
	PackLen=0x08;//每次8个
	addr_pos=TXdata[8]&0x7f;//起始地址
	addr_dir=(TXdata[8]>>7)&0x01;//低位在前还是高位在前
	addr_data_num=TXdata[9];//发送地址的个数
	addr_sub=TXdata[10];//每次发送的地址增加值 
	send_num=(TXdata[11]<<8)+TXdata[12];//发送的个数
//	UART0AnswerErr_cyl(send_num>>8,send_num&0xff);
	addr_start_data=0;
	if(addr_dir)//高位在前
	{
		for(i=0;i<addr_data_num;i++)
			addr_start_data=(addr_start_data<<8)+TXdata[addr_data_num+i];
	}
	else//低位在前
	{
		for(i=0;i<addr_data_num;i++)
			addr_start_data+=TXdata[addr_data_num+i]<<(i*8);
	}
	UartTxQueueBuf[bufnum++]=0xa5;
	UartTxQueueBuf[bufnum++]=0xa5;
	UartTxQueueBuf[bufnum++]=0x00;
	UartTxQueueBuf[bufnum++]=0x00;
	UartTxQueueBuf[bufnum++]=CAN_DATA;
	UartTxQueueBuf[bufnum++]=0x80;	//  最高位为一表示中间帧，最高位为零表示结束帧
	bufnum=7;
	up_data_num=0x8000;
	for(j=0;j<send_num;j++)//循环多少次，
	{
			if(addr_dir)//高位在前
			{
				for(i=0;i<addr_data_num;i++)
					TXdata[addr_pos+i]=(addr_start_data>>((addr_data_num-1-i)*8))&0xff;
				
			}
			else//低位在前
			{
				for(i=0;i<addr_data_num;i++)
				{
					TXdata[addr_pos+i]=(addr_start_data>>(i*8))&0xff;
					
				}
			}
			result = PassThru_CAN_TxData((uint8 *)TXdata, PackLen, CanID);
			addr_start_data+=addr_sub;
			_1msCounter=0;
			while(_1msCounter<100)
			{
				result=PassThru_CAN_RxData_Time((uint8 *)EcuComRxBuf);
				if(result)
				{
					memcpy(UartTxQueueBuf+bufnum,EcuComRxBuf+7,8);
					bufnum+=8;
					if((bufnum>=0xf0)&&(j!=send_num-1))
					{
						bufnum-=4;
						UartTxQueueBuf[2]=(bufnum>>8)&0xff;
						UartTxQueueBuf[3]=bufnum&0xff;
						UartTxQueueBuf[5]=(up_data_num>>8)&0xff;
						UartTxQueueBuf[6]=up_data_num&0xff;
						check_sum=0;
						for(i=2;i<bufnum+2;i++)
						{
							check_sum+=UartTxQueueBuf[i];
						}
						check_sum=~check_sum;
						UartTxQueueBuf[bufnum+6]=check_sum;
						//UartTxQueuePack(UartTxQueueBuf);
						USART_DMA_Send_ext(UartTxQueueBuf_ext,bufnum+5);
						up_data_num++;
						bufnum=7;
					}
					break;
				}
			}
			if(_1msCounter>=100)
			{
				UARTAnswerErr(0x8e,i);
				return result;
			}

	}		
	bufnum-=4;
		UartTxQueueBuf[2]=(bufnum>>8)&0xff;
		UartTxQueueBuf[3]=bufnum&0xff;
		UartTxQueueBuf[5]=(up_data_num>>8)&0x7f;
		UartTxQueueBuf[6]=up_data_num&0xff;
		check_sum=0;
		for(i=2;i<bufnum+2;i++)
		{
			check_sum+=UartTxQueueBuf[i];
		}
		check_sum=~check_sum;
		UartTxQueueBuf[bufnum+6]=check_sum;
		//UartTxQueuePack(UartTxQueueBuf);
		USART_DMA_Send_ext(UartTxQueueBuf_ext,bufnum+5);
		DelayNms(200);
	return TRUE;
}

/*********************************************************************************************
* 名称    ： PassThru_CAN_TxPack_NotID_Flash
* 功能    ： 将flash中的数据发出，用来发送对时间要求特别严谨的can数据
* 入口参数：
*
*
* 出口参数：
* 说明    ： 0x7f模式，can写flash数据专用
          a5 a5 len0 len1 90 x1 x2 x3 x4 x5 cs
          x1表示发送flash中的第几组数据，
          x2 x3表示从flash的第几个偏移位开始发送
          x4 表示发送几组
          x5 表示每组发送几个数据
*
***********************************************************************************************/
void PassThru_CAN_TxPack_NotID_Flash(uint8 *TXdata)
{
	uint8 result,flash_group,every_send_num,temp0,temp1;
	uint16 j,send_data_pos,up_data_num,send_num;
	uint32 CanID = PassThruConfig.CanTD;
	uint16 bufnum=0;
	uint16 addr_sub;
	uint32 addr_start_data;
	uint8 *pt;
	uint8 txdata[8];
	TXdata+=3;//去掉头和canord
	flash_group=*TXdata++;//要发送flash中第几组数据
	temp0=*TXdata++;
	temp1=*TXdata++;
	send_data_pos=(temp0<<8)+temp1;//flash偏移地址
	temp0=*TXdata++;
	temp1=*TXdata++;
	send_num=(temp0<<8)+temp1;//循环发送次数
	every_send_num=*TXdata++;//每次发送数据个数
	UartTxQueueBuf[bufnum++]=0xa5;
	UartTxQueueBuf[bufnum++]=0xa5;
	UartTxQueueBuf[bufnum++]=0x00;
	UartTxQueueBuf[bufnum++]=0x00;
	UartTxQueueBuf[bufnum++]=CAN_DATA;
	UartTxQueueBuf[bufnum++]=0x80;	//  最高位为一表示中间帧，最高位为零表示结束帧
	bufnum=7;
	up_data_num=0x8000;
	//pt=(uint8*)FLASH_BL_ADDR+(0x1000*flash_group)+send_data_pos;
	if(flash_record_data_num[flash_group]<every_send_num*send_num)//发送数据太多，超出已存的，退出
	{
		UARTAnswerErr(0x90,0xff);
		return;
	}
	for(j=0;j<send_num;j++)//循环多少次，
	{
			memcpy(txdata,pt,every_send_num);
			pt+=every_send_num;
			result = PassThru_CAN_TxData((uint8 *)txdata, every_send_num, CanID);
			addr_start_data+=addr_sub;
			_1msCounter=0;
			while(_1msCounter<100)
			{
				result=PassThru_CAN_RxData_Time((uint8 *)EcuComRxBuf);
				if(result)
				{
					memcpy(UartTxQueueBuf+bufnum,EcuComRxBuf+7,8);
					bufnum+=8;
					if((bufnum>=0xf0)&&(j!=send_num-1))
					{
						bufnum-=2;
						UartTxQueueBuf[2]=(bufnum>>8)&0xff;
						UartTxQueueBuf[3]=bufnum&0xff;
						UartTxQueueBuf[5]=(up_data_num>>8)&0xff;
						UartTxQueueBuf[6]=up_data_num&0xff;
						//Uart0TxQueuePack(J1939RxBuf);
						USART_DMA_Send_ext_1768();
						up_data_num++;
						bufnum=7;
					}
					break;
				}
			}
			if(_1msCounter>=100)
			{
				UARTAnswerErr(j>>8,j);
				return ;
			}

	}
		if(bufnum!=7)
		{
			bufnum-=4;
// 			UartTxQueueBuf[0]=0xa5;
// 			UartTxQueueBuf[1]=0xa5;
			UartTxQueueBuf[2]=(bufnum>>8)&0xff;
			UartTxQueueBuf[3]=bufnum&0xff;
			UartTxQueueBuf[5]=(up_data_num>>8)&0x7f;
			UartTxQueueBuf[6]=up_data_num&0xff;
//			check_sum=0;
// 			for(i=2;i<bufnum+2;i++)
// 			{
// 				check_sum+=UartTxQueueBuf[i];
// 			}
// // 			check_sum=~check_sum;
// // 			UartTxQueueBuf[bufnum+8]=check_sum;
			USART_DMA_Send_ext_1768();
			DelayNms(200);
		}
	return ;
}
