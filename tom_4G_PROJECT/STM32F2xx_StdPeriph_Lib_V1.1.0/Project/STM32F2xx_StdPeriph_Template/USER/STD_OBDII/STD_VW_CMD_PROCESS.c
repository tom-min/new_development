#include "STD_VW_CMD_PROCESS.h"
#include "user_queue.h"
#include "STD_OBD_SendRecv.h"
#include "Fcar_Api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "STD_OBD_CMD_PROCESS.h"

static bool m_IsNewDtc;
static uint8_t m_CAN_ANS_T;
static uint8_t m_CAN_ANS_R;
static uint8_t m_can_comm_id[4];
static char m_cpu_ver[128];	//ECU版本信息
static uint8_t m_can_menu[128];
static uint8_t recvCount;
static bool m_first_vw = true;
static bool newFrame;
static char m_uds_text[64];//读取车辆零件信息TEXT
static char m_uds_val[64];//读取车辆零件信息VAL
static char Asam_data[64];
static char Asam_revi[64];
static intptr_t m_protocol_index_VW[6] = { -1,-1,-1,-1,-1,-1 };

extern DEFAULT_MSGATTR g_defaultMsgAttr;

extern Vehicle_diagnosis g_vehiclediagnosie;
extern Data_collection g_datacollection;

uint8_t m_iSystem_Mode_Size[] = { 0x01, 0x02, 0x03, 0x09, 0x15};//01-发动机,02-变速箱电控系统,03-防抱死制动,09-中央电气电子设备,05-安全气囊
//初始化IO命令
static const CMD_INIT_VW InitIoCmd_VW[] = \
{
	// 序号
	// 初始化IO的打包方式ORD 10 11 12 13 等
	// 进系统命令1
	// 进系统命令2
	// 进系统命令3
	// 进系统命令4
	// 退系统命令
	// 读码命令
	// 读码模式
	// 码号Pos位
	// 清码命令


	{//CAN TP
		{VW_MODE_01 },
		{ MSG_Init_CANTP_PackType_VW },
		{ 0x2C,0x01, 0x07, 0xA1, 0x20, 0x02, 0x81, 0x03, 0x00, 0x04, 0x00, 0x00, 0x02, 0x00, 0x05, 0x00, 0x00, 0x02, 0x01, 0x06, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x0C, 0xE6 },
		{ 0x2C,0x01, 0x07, 0xA1, 0x20, 0x02, 0x81, 0x03, 0x00, 0x04, 0x00, 0x00, 0x07, 0x40, 0x05, 0x00, 0x00, 0x03, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x0C, 0xE6 },
		{ 0x07, 0x01, 0xC0, 0x00, 0x10, 0x00, 0x03, 0x01 },
		{ 0x06, 0xA0, 0x0F, 0x8A, 0xFF, 0x32, 0xFF },
		{ 0x05, 0x10, 0x00, 0x02, 0x10, 0x89 },
		{ 0x07, 0x11, 0x00, 0x04, 0x31, 0xB8, 0x00, 0x00 },
		{ 0x01, 0xA8 },
		{ DTC_MODE_VW_MODE_01 },
		{ 0x07, 0x12, 0x00, 0x04, 0x18, 0x00, 0xFF, 0x00 },
		{ 0x06, 0x13, 0x00, 0x03, 0x14, 0xFF, 0x00 }
	},

	{//CAN UDS
		{ VW_MODE_02 },
		{ MSG_Init_CANUDS_PackType_VW },
		{ 0x2C,0x01, 0x07, 0xA1, 0x20, 0x02, 0x82, 0x03, 0x00, 0x04, 0x00, 0x00, 0x02, 0x00, 0x05, 0x00, 0x00, 0x02, 0x01, 0x06, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x0C, 0xE6 },
		{ 0x00},
		{ 0x02, 0x10, 0x03},
		{ 0x03, 0x22, 0xF1, 0x9E},
		{ 0x03, 0x22, 0xF1, 0xA2},
		{ 0x03, 0x22, 0xF1, 0x90},
		{ 0x01, 0xA8 },
		{ DTC_MODE_VW_MODE_01 },
		{ 0x03, 0x19, 0x02, 0xAE },
		{ 0x04, 0x14, 0xff, 0xff, 0xff }
	},
};

static uint8_t pro_TPsenddata(uint8_t* sendbuf)
{
	int datanum;
	int i = 0;
	int j = 0;
	int more = 0;
	uint8_t u8buf[64];
	//U8 sendbuf[256];
	uint8_t sendNum;
	uint8_t addStrNum = 21;
	int sendCount = 0;
	memcpy(u8buf, 0x00, sizeof(u8buf));
	//取出命令
	datanum = sendbuf[0];
	memcpy(u8buf+3, sendbuf+1,datanum);
	//datanum = getHexEx(ezxml_txt(msgxml)+addStrNum, u8buf+3);
	u8buf[0] = 0x80 + datanum;
	u8buf[1] = g_defaultMsgAttr.m_TGT;
	u8buf[2] = g_defaultMsgAttr.m_SRC;
	datanum += 3;
	//校验
	u8buf[datanum] = 0;
	for (j = 0; j < datanum; j++)
	{
		u8buf[datanum] += u8buf[j];
	}
	datanum++;
	//发送命令
	do
	{
		if (datanum <= 7)
		{
			memcpy(sendbuf + 1, u8buf + i * 7, datanum);
			if (more)
				sendbuf[0] = 0x30 + sendCount;
			else
				sendbuf[0] = 0x30;
			sendNum = datanum + 1;
			datanum = 0;
		}
		else
		{
			memcpy(sendbuf + 1, u8buf + i * 7, 7);
			datanum -= 7;
			sendbuf[0] = 0x20 + sendCount;
			sendNum = 8;
			more = 1;
			sendCount++;
			sendCount &= 0x0f;
		}
		sendNum = pro_packsenddataid(sendbuf, sendNum);
		vTaskDelay(1);
		USB_BULK_Queue_Write(&USB_RX_QueueData, sendbuf, sendNum);
		i++;
		vTaskDelay(10);
	} while (datanum > 0);
	return sendNum;
}


uint8_t pro_sendansdata(void)
{
	uint8_t number;
	uint8_t sendbuf[64];
	memset(sendbuf, 0, sizeof(sendbuf));
	sendbuf[0] = 0xb0 + recvCount + 1;
	if (sendbuf[0] == 0xc0)
	{
		sendbuf[0] = 0xb0;
	}
	number = 1;
	number = pro_packsenddataid(sendbuf, number, g_defaultMsgAttr.m_ORD);
	USB_BULK_Queue_Write(&USB_RX_QueueData, sendbuf, number);
	newFrame = TRUE;
	return number;
}
//TP协议收发数据---新打包收发模式
uint8_t pro_sendmissdata() //7F 78否定回答时固定发送的确认帧
{
	uint8_t number;
	uint8_t sendbuf[64];
	memset(sendbuf, 0, sizeof(sendbuf));
	sendbuf[0] = 0x30;
	number = 1;
	number = pro_packsenddataid(sendbuf, number, g_defaultMsgAttr.m_ORD);
	USB_BULK_Queue_Write(&USB_RX_QueueData, sendbuf, number);
	newFrame = TRUE;
	return number;
}

uint8_t pro_TPrecvdata_oneFrame(uint8_t *recvbuf)
{
	uint8_t number;
	int dataNum;
	uint8_t u8buf[128];
	uint8_t checkData;
	uint8_t buf[64];
	int first = 0;
	dataNum = 0;
	while (1)
	{
		number = pro_Uart_ReadBuf(u8buf);
		if (number == 0)
		{
			return 0;
		}
		if (u8buf[0] == 0x30 && u8buf[4] == 0x7f && u8buf[6] == 0x78)
		{
			pro_sendmissdata();
			continue;
		}
		recvCount = u8buf[0] & 0x0f;
		checkData = u8buf[0] & 0xf0;
		memcpy(buf + dataNum, u8buf + 1, number - 1);
		//memcpy(recvbuf+dataNum, u8buf+1, number-1);
		dataNum += number - 1;
		//继续接收数据
		if (checkData == 0x20)
		{
			continue;
		}
		//接收完成，发送确认帧
		if (checkData == 0x10)
		{
			pro_sendansdata();
			break;
			//if(u8buf[0] != 0x10)//回复=0x10时下位机自动回复确认0xb1
			//{
			//	pro_sendansdata();
			//	break;
			//}
		}
		//多帧一次收不完,发送一个确认帧,继续接收
		if (checkData == 0x00)
		{
			pro_sendansdata();
			continue;
		}
		//碰到3x，同7f xx 78,回复30确认，继续接受
		if (checkData == 0x30)
		{
			pro_sendmissdata();
			if (u8buf[0] > 0x30)
			{
				continue;
			}
			break;
			//if(u8buf[0] != 0x30)
			//{
			//	pro_sendmissdata();
			//	continue;
			//}
		}
		//接收到全部数据
		break;
	}
	//按CAN的模式将数据整理以便处理
	dataNum = dataNum - 2;
	recvbuf[0] = 0x00;
	recvbuf[1] = buf[0] - 0x80;
	if (dataNum < 2)
	{
		return 0;
	}
	memcpy(recvbuf + 2, buf + 3, dataNum - 2);
	number = recvbuf[1] + 2;
	//number = recvbuf[0]-0x80+4;
	//if(dataNum != number)
	//{
	//	number = 0;
	//}
	return number;
}

static uint8_t pro_recvdata_oneFrame(uint8_t *recvbuf)
{
	uint8_t number;
	uint8_t dataNum;
	uint8_t u8buf[128];
	uint8_t checkData;
	dataNum = 0;
	while(1)
	{
		number = pro_Uart_ReadBuf(u8buf);
		if(number == 0)
		{
			return 0;
		}
		recvCount = u8buf[0]&0x0f;
		checkData = u8buf[0]&0xf0;
		memcpy(recvbuf+dataNum, u8buf+1, number-1);
		dataNum += number-1;
		//继续接收数据
		if(checkData == 0x20)
		{
			continue;
		}
		//接收完成，发送确认帧
		if(checkData == 0x10)
		{
			pro_sendansdata();
		}
		//多帧一次收不完,发送一个确认帧,继续接收
		if(checkData == 0x00)
		{
			pro_sendansdata();
			continue;
		}
		//接收到全部数据
		break;
	}/*
	if(recvbuf[0] == 0x80)
		number = recvbuf[1]+2;
	else
		number = recvbuf[0]*0x100+recvbuf[1]+2;*/
	number = (recvbuf[0]&0x0f)*0x100+recvbuf[1]+2;
	if(dataNum != number)
	{
		number = 0;
	}
	return number;
}
static int pro_Uart_ReadBuf(uint8_t *recvbuf)
{
	uint16_t number;
	uint8_t i;
	for(i=0;i<20;i++)
	{
		OBDII_ReadBuf(recvbuf, &number, g_defaultMsgAttr.m_AnswerFrameTimeOut*100);
		if(number < 11)
		{
			return 0;
		}
		number -= 10;
		memcpy(recvbuf, recvbuf+9, number);
		if(number == 1 && recvbuf[0] == 0xa3)
		{
			recvbuf[0] = 0xa1;
			recvbuf[1] = 0x0f;
			recvbuf[2] = 0x64;
			recvbuf[3] = 0x9c;
			recvbuf[4] = 0x0a;
			recvbuf[5] = 0x9c;
			number = pro_packsenddataid(recvbuf, 6);
			USB_BULK_Queue_Write(&USB_RX_QueueData, recvbuf, number);//A3
			continue;
		}
		break;
	}
	return number;
}


static uint8_t pro_senddata(uint8_t* sendbuf)
{
	uint16_t datanum;
	uint8_t i=0;
	uint8_t u8buf[64];
	uint8_t sendNum;
	
	memset(u8buf, 0x00, 64);
	//取出命令
	//datanum = getHexEx(ezxml_txt(msgxml)+addStrNum, u8buf+2);
	
	datanum = sendbuf[0];
	memcpy(u8buf,sendbuf+1,datanum);

	datanum = pro_packsenddataid(u8buf, datanum, g_defaultMsgAttr.m_ORD);
	USB_BULK_Queue_Write(&USB_RX_QueueData, u8buf, datanum);
	return 1;
}

static uint8_t pro_TPrecv_send(uint8_t* sendbuf, uint8_t* recvbuf)
{
	uint8_t number;
	uint8_t checkData;
	uint8_t buffer[128];
	uint8_t recvNum;
	//USB_BULK_Queue_Write(&USB_RX_QueueData, sendbuf, number);
	//OBDII_ReadBuf(recvbuf, &number, g_defaultMsgAttr.m_AnswerFrameTimeOut * 15);
	if(sendbuf[0]>0)
	{
		//发送命令
		number = pro_senddata(sendbuf);
		//收取确认帧
		number = pro_Uart_ReadBuf(recvbuf);
		if(number != 1)
		{
			return 0;
		}
		checkData = recvbuf[0];
		//校验确认帧
	}
	//接收命令
	recvNum = 0;
	do
	{
		number = pro_recvdata_oneFrame(buffer);
		if(number == 0)
		{
			return 0;
		}
		//解析否定回答 14 80 03 7f 22 78
		if(buffer[1] == 0x03 && buffer[2] == 0x7f && buffer[4] == 0x78)
		{
			continue;
		}
		if(recvNum == 0)
		{
			memcpy(recvbuf, buffer, number);
		}
		else
		{
			number -= 3;
			if(number < 0)
			{
				return 0;
			}
			memcpy(recvbuf+recvNum, buffer+3, number);
		}
		recvNum += number;
	}
	while(buffer[0] == 0x80);
	if(recvbuf[0] == 0x80) //当通过0X80收了多个数据块时,通过实际有效数据设置数据长度.
	{
		recvbuf[0] = (recvNum-2) / 0xFF;
		recvbuf[1] = (recvNum-2) % 0xFF;
	}
	recvbuf[0] &= 0x7f;
	return recvNum;
}

INIT_MODE_VW pro_audi_can_readdtc_asam(SYSTEM_VW sys, INIT_MODE_VW index, int* dtc_number, char* dtc_list, char* dtc_sys)
{
	int i, number, dtcnumber;
	char strbuf[64];
	char dtcsystem[4];
	uint8_t recvbuf[64];
	uint8_t sendbuf[64];
	
	memset(sendbuf, 0x00, sizeof(sendbuf));
	memcpy(sendbuf, InitIoCmd_VW[index].DTC_Read, InitIoCmd_VW[index].DTC_Read[0] +1);
	number = pro_SendRecv(sendbuf, recvbuf);
	if (number == 0)
	{
		return index;
	}
	if (number == 3 && recvbuf[0] == 0x7F)
	{
		return index;
	}
	dtcnumber = (number - 3) / 4;
	if (dtcnumber > 50)		//最多显示50个故障码
		dtcnumber = 50;
	for (i = 0; i < dtcnumber; i++)
	{
		sprintf(strbuf, "%02X%02X%02X", recvbuf[i * 4 + 3], recvbuf[i * 4 + 4], recvbuf[i * 4 + 5]);
		//strcpy(dtc_list,strbuf,strlen(strbuf));
		if(*dtc_number != 0)
		{
			strcat(dtc_list,",");
			strcat(dtc_sys,",");
		}
		strcat(dtc_list,strbuf);
		sprintf(dtcsystem,"%d",sys);
		strcat(dtc_sys,dtcsystem);
		(*dtc_number)++;
	}
	printfUartObdiideBug("dtc_list = %s\r\n", dtc_list);
	printfUartObdiideBug("dtc_sys = %s\r\n", dtc_sys);
	return index;
}
INIT_MODE_VW pro_audi_can_readdtc(SYSTEM_VW sys, INIT_MODE_VW index, int* dtc_number, char* dtc_list, char* dtc_sys)
{
	char dtc[16], dtcun[16],dtcsystem[4];
	int number, dtcnumber;
	int i, j;
	int ncode;
	uint8_t recvbuf[128];
	uint8_t sendbuf[64];
	int dtcdata;
	bool HaveSta = true;

	dtcnumber = 0;
	j = 0;
	ncode = 0;
	//如果有18就用新的读码方式18,00,FF,00
	//否则就用旧的读码方式18,02,FF,00
	//此处在进入系统之前已经选择好了
	memset(sendbuf, 0x00, sizeof(sendbuf));
	memset(recvbuf, 0x00, sizeof(recvbuf));
	memcpy(sendbuf, InitIoCmd_VW[index].DTC_Read, InitIoCmd_VW[index].DTC_Read[0]+1);
	if (m_IsNewDtc == TRUE)
	{
		sendbuf[4] = 0x02;
	}
	number = pro_TPrecv_send(sendbuf, recvbuf);
	//number = pro_recv_send(msgxml, recvbuf);
	if (number == 0)
	{

		return -1;
	}
	if ( recvbuf[0] == 0x7f)
	{
		return -1;
	}
	if (recvbuf[1] < 5)//针对读码回复00 03 58 00 ff的情况在这里修改 
	{
		//没有故障码
		return index;
	}
	//故障码从收到的第四个数据开始计算
	for (i = 4; i < number; i += 3)
	{
		if ((recvbuf[i] == 0x00) && (recvbuf[i + 1] == 0x00))
		{
			continue;
		}
		//数据回复长度可以计算出n个dtc;//但58 xx中xx定义小于n;//后续的'故障码'不算;
		if (dtcnumber >= recvbuf[3])//e.g:00 09 58 02 00 42 2c 04 f9 69 39//用number可计算出3个dtc;//实际只有2个dtc;
		{
			break;
		}
		//大码
		if (m_IsNewDtc == TRUE)
		{
			sprintf(dtcun, "%05d", recvbuf[i] * 0x100 + recvbuf[i + 1]);
			sprintf(dtc, "000%05d", recvbuf[i] * 0x100 + recvbuf[i + 1]);
			ncode = recvbuf[i] * 0x100 + recvbuf[i + 1];

			//计算扩展编号
			//dtcdata = 0x4000 + ((recvbuf[i] / 0x10) << 10) + (recvbuf[i] % 0x10) * 100 + (recvbuf[i + 1] / 0x10) * 10 + recvbuf[i + 1] % 0x10;
			//sprintf(dtc, "%05d", dtcdata);
			//HaveSta = FALSE;
		}
		else
		{
			dtcdata = recvbuf[i] * 0x100 + recvbuf[i + 1];
			sprintf(dtc, "001%05d", recvbuf[i] * 0x100 + recvbuf[i + 1]);
			sprintf(dtcun, "%05d", recvbuf[i] * 0x100 + recvbuf[i + 1]);

			if ((dtcdata >= 0x4000) && (dtcdata < 0x8000))
			{
				//将扩展编码换算成PCB码
				dtcdata = dtcdata & 0x3FFF;
				//sprintf(dtc, "%X%03d", (dtcdata>>10)&0x0f, dtcdata&0x03ff);
				//sprintf(dtc, "X%05d",getHex(dtc)*0x100+getHex(dtc+2));

				//sprintf(dtc, "X%05d", ((dtcdata >> 10) & 0x0f) * 0x100 + (dtcdata & 0xff));
				//HaveSta = FALSE;
			}
		}
		if(*dtc_number != 0)
		{
			strcat(dtc_list,",");
			strcat(dtc_sys,",");
		}
		strcat(dtc_list,dtc);
		sprintf(dtcsystem,"%d",sys);
		strcat(dtc_sys,dtcsystem);
		(*dtc_number)++;
	}
	printfUartObdiideBug("dtc_list = %s\r\n", dtc_list);
	printfUartObdiideBug("dtc_sys = %s\r\n", dtc_sys);
	return index;
}

INIT_MODE_VW pro_read_dtc_VW(SYSTEM_VW sys, INIT_MODE_VW index, int* dtc_number, char* dtc_list, char* dtc_sys)
{
	if (index == 0)
	{
		return pro_audi_can_readdtc(sys, index, dtc_number, dtc_list, dtc_sys);
	}
	else if (index == 1)
	{
		return pro_audi_can_readdtc_asam(sys, index, dtc_number, dtc_list, dtc_sys);
	}
	else return index;
}

bool cmd_CAN_HAVEMENU(uint8_t code)
{
	uint8_t *buf;
	uint8_t len;
	int i;
	buf = m_can_menu;
	len = m_can_menu[63];
	if (buf[1] + 2 > len)
		return false;

	switch (code)
	{
	case 0x08:
	case 0x02:
	case 0x03:
	case 0x16:
	case 0x1A:
		return TRUE;
	}
	for (i = 4; i < buf[1] + 2; i += 2)
	{
		switch (buf[i + 1])
		{
		case 0x01:
			if (code == 0x04)
				return true;
			break;
		case 0x06:
			if (code == 0x15)
				return true;
			break;
		case 0x05:
			if (code == 0x11)
				return true;
			break;
		case 0x04:
			if (code == 0x07)
				return true;
			break;
		case 0x03:
			if (code == 0x10)
				return true;
			break;
		case 0x0a:
			if (code == 0x0A)
				return true;
			break;
		case 0x18:
			if (code == 0x18)
				return true;
			break;
		}
	}
	return false;
}

int pro_packsenddataid(uint8_t *sendbuf, uint16_t number, uint8_t msgord)
{
	uint8_t buf[64] = { 0 };
	memset(buf,0x00,64);
	memcpy(buf, m_can_comm_id, 4);
	memcpy(buf + 4, sendbuf, number);
	number+=4;
	memcpy(sendbuf,buf,number);
	return pro_packsenddata(sendbuf, number, msgord);
}


bool pro_canuds_select_module_VW(INIT_MODE_VW mode, uint8_t sys)
{
	int i = 0;
	uint8_t recvbuf[64];
	uint8_t sendbuf[64];
	uint8_t number;


	//初始化10 03	
	memset(sendbuf, 0x00, sizeof(sendbuf));
	memcpy(sendbuf, InitIoCmd_VW[mode].System_Enter1, InitIoCmd_VW[mode].System_Enter1[0] +1);
	number = pro_SendRecv(sendbuf, recvbuf);
	if (number == 0)
	{
		return false;
	}
	if (number == 3 && recvbuf[0] == 0x7f && recvbuf[1] == 0x10)
	{
		return false;
	}


	//ASAM Dataset
	memset(sendbuf, 0x00, sizeof(sendbuf));
	memcpy(sendbuf, InitIoCmd_VW[mode].System_Enter2, InitIoCmd_VW[mode].System_Enter2[0] +1);
	number = pro_SendRecv(sendbuf, recvbuf);
	if (number == 0)
	{
		return false;
	}
	if (number == 3 && recvbuf[0] == 0x7f && recvbuf[1] == 0x22)
	{
		return false;
	}
	for (i = 3; i < number; i++)
	{
		sprintf(Asam_data, "%s%02x,", Asam_data, recvbuf[i]);
	}
	recvbuf[number] = 0x00;//取到所有有效数据
	strcpy(m_uds_text, (char *)recvbuf + 3);

	//ASAM Dataset Revision	
	memset(sendbuf, 0x00, sizeof(sendbuf));
	memcpy(sendbuf, InitIoCmd_VW[mode].System_Enter3, InitIoCmd_VW[mode].System_Enter3[0] +1);
	number = pro_SendRecv(sendbuf, recvbuf);
	if (number == 0)
	{
		return false;
	}
	if (number == 3 && recvbuf[0] == 0x7f && recvbuf[1] == 0x22)
	{
		return false;
	}
	for (i = 3; i < number; i++)
	{
		sprintf(Asam_revi, "%s%02x,", Asam_revi, recvbuf[i]);
	}
	recvbuf[number] = 0x00;//取到所有有效数据
	strcpy(m_uds_val, "_");
	strcat(m_uds_val, (char *)recvbuf + 3);

	return true;
}

bool pro_cantp_select_module_VW(INIT_MODE_VW mode, uint8_t sys)
{
	int i, j, count;
	int errCount = 0;
	uint8_t sendbuf[64];
	uint8_t recvbuf[128];
	uint8_t buf[64];
	uint8_t u8buf[128];
	uint8_t sendnum = 0;
	uint16_t number = 0;
	uint32_t canid;
	sendnum = InitIoCmd_VW[mode].System_Enter1[0];
	memset(sendbuf, 0, sizeof(sendbuf));
	memcpy(sendbuf, InitIoCmd_VW[mode].System_Enter1 + 1, sendnum);
	
	g_defaultMsgAttr.m_PCK_TYPE = 0x00;
	if	(mode == 0)
	{
		sendbuf[0] = m_iSystem_Mode_Size[sys];
}
	number = pro_packsenddataid(sendbuf, sendnum, 0x30);
	USB_BULK_Queue_Write(&USB_RX_QueueData, sendbuf, number);

	for (count = 0; count < 40; count++)
	{
		OBDII_ReadBuf(buf, &number, g_defaultMsgAttr.m_AnswerFrameTimeOut * 10);
		if (number == 0)
		{
			errCount += 1;
			if (errCount == 2)
			{
				return 0;
			}
			else
			{
				vTaskDelay(400);//是否有必要;
				//Uart_ClearOutput(g_carportHandle);
				//number = pack_type(msgxml, buf, 255);
				USB_BULK_Queue_Write(&USB_RX_QueueData, sendbuf, number);
				continue;//未收到回复时，可重复2次;
			}
			//UDS协议重新做初始化 下面的内容全部不需要了
		}
		number = pro_unpackrecvdata(buf, number);
		if (number == 11 && (buf[4] == 0x00 || buf[4] == 0x02) && buf[5] == 0xd0)
		{
			m_can_comm_id[2] = buf[9];
			m_can_comm_id[3] = buf[8];
			break;
		}
	}
	vTaskDelay(20);
	Init_IO_VW_2(mode);


	//请求数据 A0,0F,8A,FF,32,FF
	sendnum = InitIoCmd_VW[mode].System_Enter2[0];
	memset(sendbuf, 0, sizeof(sendbuf));
	memcpy(sendbuf, InitIoCmd_VW[mode].System_Enter2+1, sendnum);
	number = pro_packsenddataid(sendbuf, sendnum, 0x30);
	USB_BULK_Queue_Write(&USB_RX_QueueData, sendbuf, number);
	OBDII_ReadBuf(recvbuf, &number, g_defaultMsgAttr.m_AnswerFrameTimeOut * 15);
	//number = unpack_buf(pack_t, buf, number);
	if (number == 0)
		return FALSE;


	//依据VAG-COM添加一个进入命令 10 89
	sendnum = InitIoCmd_VW[mode].System_Enter3[0];
	memset(sendbuf, 0, sizeof(sendbuf));
	memcpy(sendbuf, InitIoCmd_VW[mode].System_Enter3,  InitIoCmd_VW[mode].System_Enter3[0] +1);
	//number = pro_packsenddataid(sendbuf, number, InitIoCmd_VW[mode].Msg_Ord);
	//USB_BULK_Queue_Write(&USB_RX_QueueData, sendbuf, number);
	//OBDII_ReadBuf(u8buf, &number, g_defaultMsgAttr.m_AnswerFrameTimeOut * 15);
	number = pro_TPrecv_send(sendbuf, u8buf);
	if (number == 0)
	{
		return 0;
	}

	memset(m_cpu_ver, 0x00, sizeof(m_cpu_ver));
	memcpy(m_cpu_ver, u8buf, u8buf[0] * 256 + u8buf[1] + 2);

	//动态菜单
	vTaskDelay(100);

	//31 b8 00 动态菜单
	sendnum = InitIoCmd_VW[mode].System_Enter4[0];
	memset(sendbuf, 0, sizeof(sendbuf));
	memset(m_can_menu, 0x00, sizeof(m_can_menu));
	memcpy(sendbuf, InitIoCmd_VW[mode].System_Enter4, sendnum+1);
	//number = pro_packsenddataid(sendbuf, number, InitIoCmd_VW[mode].Msg_Ord);
	//USB_BULK_Queue_Write(&USB_RX_QueueData, sendbuf, number);
	//OBDII_ReadBuf(m_can_menu, &number, g_defaultMsgAttr.m_AnswerFrameTimeOut * 15);
	number = pro_TPrecv_send(sendbuf, m_can_menu);
	//number = pro_recv_send(msgxml, m_can_menu);
	if (number == 0)
	{
		return 0;
	}
	m_can_menu[127] = number;
	//读取故障码，如果有0x18,则发,18 00 ff 00命令,否则发18 02 ff 00命令
	if (cmd_CAN_HAVEMENU(0x18))
	{
		m_IsNewDtc = true;
		m_can_menu[126] = 0xFF;
	}
	else
	{
		m_IsNewDtc = false;
		m_can_menu[126] = 0;
	}

	vTaskDelay(10);
	return true;
}
void pro_msg_all_VW(INIT_MODE_VW mode)
{
	//cmd_set_two_id_VW(0x00);
	switch (mode)
	{
		//CAN
	case VW_MODE_01:
		g_defaultMsgAttr.m_ORD = 0x30;
		g_defaultMsgAttr.m_PCK_TYPE = 0x00;
		break;

	case VW_MODE_02:
		g_defaultMsgAttr.m_ORD = 0x30;
		g_defaultMsgAttr.m_PCK_TYPE = 0x22;
		break;
	default:
		break;
	}
}
bool pro_much_init_VW(INIT_MODE_VW mode, uint8_t sys)
{
	uint8_t sendbuf[64] = { 0 };
	bool bRet = false;
	MSG_ORD_VW PackType = InitIoCmd_VW[mode].Msg_Ord;

	memset(sendbuf, 0, sizeof(sendbuf));

	switch (mode)
	{
		//CANTP
	case VW_MODE_01:
		pro_msg_all_VW(mode);
		bRet = pro_cantp_select_module_VW(mode, sys);
		break;

		//CANUDS
	case VW_MODE_02:
		pro_msg_all_VW(mode);
		bRet = pro_canuds_select_module_VW(mode,sys);
		break;

	default:
		break;
	}

	return bRet;
}
//初始化IO
bool Init_IO_VW(INIT_MODE_VW index, uint8_t sys)
{
	uint16_t number = 0, count;
	INIT_MODE_VW mode = 0;
	uint8_t sendbuf[64];
	uint8_t recvbuf[64];

	memset(recvbuf, 0, sizeof(recvbuf));
	number = InitIoCmd_VW[index].CMD_DATA[0];
	memcpy(sendbuf, InitIoCmd_VW[index].CMD_DATA + 1, number);
	mode = InitIoCmd_VW[index].Init_Mode;
	g_defaultMsgAttr.m_PCK_TYPE = 0x00;
	if (index == 0)
	{
		m_can_comm_id[2] = sendbuf[0x0b];
		m_can_comm_id[3] = sendbuf[0x0c];
		sendbuf[17] = m_iSystem_Mode_Size[sys];
	}
	if (index == 0x01)
	{
		pro_set_udsid(sendbuf,sys);
	}
	
	if (!SetFrameTimeOut_VW(sendbuf, number, InitIoCmd_VW[index].Msg_Ord))
	{
		return false;
	}
	number = pro_packsenddata(sendbuf, number, InitIoCmd_VW[index].Msg_Ord);
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
		printfuartdebug("InitIO false\r\n");
		return false;
	}
	//vTaskDelay(1000);
	//pro_set_can_id_VW(index);
	printfuartdebug("InitIO true\r\n");
	return true;
}

bool Init_IO_VW_2(INIT_MODE_VW index)
{
	uint16_t number = 0;
	int count;
	INIT_MODE_VW mode = 0;
	uint8_t sendbuf[64];
	uint8_t recvbuf[64];

	memset(recvbuf, 0, sizeof(recvbuf));
	number = InitIoCmd_VW[index].CMD_DATA2[0];
	memcpy(sendbuf, InitIoCmd_VW[index].CMD_DATA2 + 1, number);
	sendbuf[0x09] = m_can_comm_id[0];
	sendbuf[0x0a] = m_can_comm_id[1];
	sendbuf[0x0b] = m_can_comm_id[2];
	sendbuf[0x0c] = m_can_comm_id[3];
	if (!SetFrameTimeOut_VW(sendbuf,number,InitIoCmd_VW[index].Msg_Ord))
	{
		return false;
	}
	mode = InitIoCmd_VW[index].Init_Mode;
	g_defaultMsgAttr.m_PCK_TYPE = 0x00;
	number = pro_packsenddata(sendbuf, number, InitIoCmd_VW[index].Msg_Ord);
	if (number == 0)
	{
		return 0;
	}
	ResetComPort();
	vTaskDelay(20);


	USB_BULK_Queue_Write(&USB_RX_QueueData, sendbuf, number);
	OBDII_ReadBuf(recvbuf, &number, 1500);

	for (count = 0; count < 20; count++)
	{
		OBDII_ReadBuf(recvbuf, &number, 1500);
		if (!number)
			return false;
		if ((recvbuf[4] & 0x7f) != 0x10)
			continue;
		else
			break;
	}
	vTaskDelay(20);

	if (number < 1)
	{
		printfuartdebug("InitIO false\r\n");
		return false;
	}
	//vTaskDelay(1000);
	//pro_set_can_id_VW(index);
	printfuartdebug("InitIO true\r\n");
	return true;
}

bool pro_set_udsid(uint8_t* initio, uint8_t sys)
{
	int number;
	uint8_t idnum = sys;
	uint16_t udsid[] =
	{
		0x01, 0x07e0, 0x07e8,
		0x02, 0x07e1, 0x07e9,
		0x03, 0x0713, 0x077d,
		0x09, 0x070e, 0x0778,
		0x15, 0x0715, 0x077f
	};
	if (udsid[idnum * 3 + 1] == 0 || udsid[idnum * 3 + 2] == 0)
	{
		return FALSE;
	}

	initio[11] = udsid[idnum * 3 + 1] / 0x100;
	initio[12] = udsid[idnum * 3 + 1] % 0x100;
	initio[16] = udsid[idnum * 3 + 2] / 0x100;
	initio[17] = udsid[idnum * 3 + 2] % 0x100;

	return TRUE;
}

static void pro_clear_dtc_VW(INIT_MODE_VW index, SYSTEM_VW sys) //
{
	uint8_t sendbuf[64] = {0}; //
	uint8_t recvbuf[128] = {0}; //
	uint8_t number = 0;

	if(index == -1) return;
	memcpy(sendbuf, InitIoCmd_VW[index].DTC_Clear,InitIoCmd_VW[index].DTC_Clear[0]+1);
	if(index == 0)
		number = pro_TPrecv_send(sendbuf, recvbuf); 
	else if(index == 1)
		number = pro_SendRecv(sendbuf, recvbuf); 
	return;
}

void pro_exit_system_VW(INIT_MODE_VW index, SYSTEM_VW sys)
{
	uint8_t sendbuf[64] = { 0 };
	uint8_t recvbuf[128] = { 0 };
	uint8_t number = 0;
	if(index == -1) return;
	memcpy(sendbuf, InitIoCmd_VW[index].System_Exit, InitIoCmd_VW[index].System_Exit[0] + 1);
	if (InitIoCmd_VW[index].System_Exit[0] > 0)
	{
		if(index == 0)
			number = pro_TPrecv_send(sendbuf, recvbuf); 
		else if(index == 1)
			number = pro_SendRecv(sendbuf, recvbuf); 
	}
	return;
}

bool SetFrameTimeOut_VW(uint8_t* initdata, uint16_t number, uint8_t ord)
{
	uint8_t i = 0;
	uint32_t canid;
	m_CAN_ANS_T = 0;
	m_CAN_ANS_R = 0;	
	
	switch (ord)
	{
	case 0x13:
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
				//错误
				return 0;

			}
		}
		break;
	case 0x10:
		g_defaultMsgAttr.m_AnswerByteTimeOut = 02;
		//g_defaultMsgAttr.m_AnswerFrameTimeOut = 0xd;
		g_defaultMsgAttr.m_AnswerFrameTimeOut = 0x32;//放大超时时间;
		g_defaultMsgAttr.m_RequestFrameTimeOut = 15;
		g_defaultMsgAttr.m_RequestByteTimeOut = 02;
		g_defaultMsgAttr.m_AutoKeepTimeOut = 15;
		g_defaultMsgAttr.m_KeepFlag = 00;
		while (i < number)
		{
			switch (initdata[i])
			{
			case 0x01: i += 4;	 break;
			case 0x02: i += 2;	 break;
			case 0x03: i += 2;	 break;
			case 0x04: 
				canid=0;
				canid += initdata[i+1]<<24;
				canid += initdata[i+2]<<16;
				canid += initdata[i+3]<<8;
				canid += initdata[i+4];
				//setcanbusflowid(canid);
				setcanbussendid(canid);
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
				return 0;

			}
		}
		break;
	case 0x11: //别克?
		g_defaultMsgAttr.m_AnswerByteTimeOut = 02;
		g_defaultMsgAttr.m_AnswerFrameTimeOut = 0xd;
		g_defaultMsgAttr.m_RequestFrameTimeOut = 15;
		g_defaultMsgAttr.m_RequestByteTimeOut = 02;
		g_defaultMsgAttr.m_AutoKeepTimeOut = 15;
		g_defaultMsgAttr.m_KeepFlag = 00;
		break;
	case 0x12://福特

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
		return 0;
	}

	return 1;
}

bool Much_Init_VW()
{
	uint8_t i = 0, j = 0;
	uint8_t initstart = 0, initend = 0;
	INIT_MODE_VW mode = 0;
	//int num = 0;
	//char buf[60] = {0};

	uint8_t iSystem = 5;//一共5个系统
	
	bool enter = false;

	g_vehiclediagnosie.fault_total = 0;
	memset(g_vehiclediagnosie.fault_list, 0, sizeof(g_vehiclediagnosie.fault_list));//故障码
	memset(g_vehiclediagnosie.sys_list, 0, sizeof(g_vehiclediagnosie.sys_list));//系统编号 1发动机 2传动 3ABS 4车身 5安全

	printfUartObdiideBug("welcome to [%s]\r\n", __func__);
	if (m_first_vw)
	{
		//第一次进入
		m_first_vw = false;

		initstart = 0;//从第一个系统开始扫描时重置
		initend = 0;//从第一个系统开始扫描时重置
		for (i = VW_ENGINE; i <= VW_SRS; i++)
		{
			printfUartObdiideBug("SYSTEM_VW = [%d]\r\n",i);
			initstart = 0;
			initend = 2;
			for (j = initstart; j < initend; j++)
			{
				printfUartObdiideBug("initstart to [%d]\r\n", j);
				//reset_global_data();//重置全局变量数据
				if (Init_IO_VW(InitIoCmd_VW[j].Init_Mode, i)) //初始化IO
				{
					if (pro_much_init_VW(InitIoCmd_VW[j].Init_Mode, i)) //初始化ECU //进系统
					{
						printfUartObdiideBug("VW enter success system:%d init:%d \r\n", i, j);
						//m_protocol_index_VW[i] = InitIoCmd_VW[j].Init_Mode;												//保存是第几个协议
						m_protocol_index_VW[i] = pro_read_dtc_VW(i, InitIoCmd_VW[j].Init_Mode, &(g_vehiclediagnosie.fault_total), g_vehiclediagnosie.fault_list, g_vehiclediagnosie.sys_list);	//读码
						pro_clear_dtc_VW(m_protocol_index_VW[i],i);															//清码
						pro_exit_system_VW(m_protocol_index_VW[i],i);														//退出
						break;
					}
					else
					{
						printfUartObdiideBug("VW enter false system:%d init:%d (%d-%d) \r\n", i, j, initstart, initend - 1);
					}
				}
			}
		}
		
		for(i = VW_ENGINE;i <= VW_SRS;i++)
		{
				if(m_protocol_index_VW[i] != -1)
						return true;
		}
		
	}
	else
	{
		vTaskDelay(3000);
		enter = false;
		for (i = VW_ENGINE; i <= VW_SRS; i++)
		{
			if (m_protocol_index_VW[i] != -1)//直接使用记录的协议，如果为-1，则说明不存在此系统
			{
				if (Init_IO_VW(InitIoCmd_VW[m_protocol_index_VW[i]].Init_Mode, i)) //初始化IO
				{
					if (pro_much_init_VW(InitIoCmd_VW[m_protocol_index_VW[i]].Init_Mode, i)) //初始化ECU
					{
						enter = true;
						pro_read_dtc_VW(i, m_protocol_index_VW[i], &(g_vehiclediagnosie.fault_total), g_vehiclediagnosie.fault_list, g_vehiclediagnosie.sys_list);	//读码
					}
				}
			}
		}
	}

	return enter;
}



//void main() {}