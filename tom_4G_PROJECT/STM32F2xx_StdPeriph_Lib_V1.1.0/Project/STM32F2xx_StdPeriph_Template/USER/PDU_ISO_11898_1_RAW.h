#ifndef __ISO_11898_1_RAW_H
#define __ISO_11898_1_RAW_H
#include "common.h"


#define CAN_CBFF                    ((uint8_t)0x00)  /* Classical Base Frame Format*/
#define CAN_CEFF                    ((uint8_t)0x01)  /* Classical Extended Frame Format*/
#define CAN_FBFF                    ((uint8_t)0x02)  /* FD Base Frame Format*/
#define CAN_FEFF                    ((uint8_t)0x03)  /* FD Extended Frame Format*/
#define CAN_FD_BRS                  ((uint8_t)0x04)  /* Bit Rate Switch Enabled*/
#define CAN_FD_ESI                  ((uint8_t)0x08)  /* Error State Indicator Enabled*/
#define CAN_REMOTE                  ((uint8_t)0x10)  /* Remote frame*/

#define CAN_EX_ADR_MASK             ((uint8_t)0x08)  

#define CAN_ID_SIZE_MASK			(1U<<0)			 /*bit0 标志是can id 是11bit 还是29bit*/

#define CAN_SEND_RETRY_NUM_MAX		6				//can发送重试最大次数6
#if 0
extern PDU_CAN_FORMAT_STRUCT Channel_0_CanFormat; 
extern PDU_CAN_FORMAT_STRUCT Channel_1_CanFormat;
extern PDU_PRE_REPLY_STRUCT  Channel_0_PreReply;   
extern PDU_PRE_REPLY_STRUCT  Channel_1_PreReply;   
#endif

typedef enum
{
	CAN_TRANSFER_NOT_COMPLETE = 0,
	CAN_TRANSFER_COMPLETE = 1,
	CAN_TRANSFER_ABORTED = 2,
}PDU_CAN_TRANSFER_STATUS;

extern uint8_t g_mcan_channel_start_flag; 

extern uint8_t PDU_ISO_11898_1_RAW_Init(void);//设备初始化 
extern uint8_t PDU_ISO_11898_1_RAW_Reset(void);//重置 悬空状态
extern uint8_t PDU_ISO_11898_1_RAW_SetComParams(PDU_COMMON_DATA_STRUCT CommonData,uint8_t type,uint32_t baudrate);
extern uint8_t PDU_ISO_11898_1_RAW_SetFilter(PDU_COMMON_DATA_STRUCT CommonData,PDU_CAN_FILTER_STRUCT CanFilter);
extern uint8_t PDU_ISO_11898_1_RAW_GetDataLength(uint8_t CanDlc);
extern uint8_t PDU_ISO_11898_1_RAW_Request(PDU_COMMON_DATA_STRUCT CommonData,PDU_CAN_RAW_DATA RawMessage);
extern uint8_t PDU_ISO_11898_1_RAW_Indication(PDU_COMMON_DATA_STRUCT *CommonData,PDU_CAN_RAW_DATA *RawMessage);
extern uint8_t PDU_ISO_11898_1_RAW_Confirm(uint32_t Identifier, uint8_t Transfer_Status);
extern uint8_t PDU_ISO_11898_1_RAW_ProtocolControl(PDU_COMMON_DATA_STRUCT CommonData,uint8_t command);
extern uint8_t PDU_ISO_11898_1_RAW_ClearBuffer(PDU_COMMON_DATA_STRUCT CommonData,uint8_t command);
extern uint8_t PDU_ISO_11898_1_Calc_PreReplyGroupBlockSize(PDU_COMMON_DATA_STRUCT *CommonData,uint32_t ECU_ID,uint32_t *PreReplyID);
extern uint8_t PDU_ISO_11898_1_Calc_PreReplyGroupFilter(PDU_COMMON_DATA_STRUCT *CommonData,uint32_t ECU_ID,uint32_t *PreReplyID);

#endif
/*******************END OF FILE*****************/
