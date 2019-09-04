/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
* 文件名  ：usart1.c
* 描述    ：将printf函数重定向到USART1。这样就可以用printf函数将单片机的数据
*           打印到PC上的超级终端或串口调试助手。
* 实验平台：野火STM32开发板
* 硬件连接：------------------------
*          | PA9  - USART1(Tx)      |
*          | PA10 - USART1(Rx)      |
*           ------------------------
* 库版本  ：ST3.0.0
*
* 作者    ：fire  QQ: 313303034
* 博客    ：firestm32.blog.chinaunix.net
**********************************************************************************/


#include <stdio.h>
#include <string.h>
 
#include "config.h"
 
#include "user_queue.h"
#include "bluewifi.h"
#include "user_config.h"
static uint16 UartTxCount=0;
extern uint8_t EcuComRxBuf[];
extern uint8_t EcuComRxBuf_temp[];
 

#define  DelayNms hal_HW_Delay_ms
   
extern void bluewifidatatransmit(uint8_t *data,uint16_t len);
extern  uint32_t gusbresume;
extern  void printfecudebug(char *dat);
extern void uart_tx_xdmac_configure(uint32_t tx_size,uint32_t *tx_buff);
extern void uart_tx_xdmac_channel_software_request(void);
 
#define  M_AUDIOQ 1
 extern void printfuartdebugtimestamp(const char *fmt,...);
void UartTxQueuePack(uint8* EcuComRxBuf)
{
    uint8 chacknum = 0;
    uint16 len = 0;
    uint16 i = 0;

		uint8 EcuComTxBuf_INQUARY[7]={0xa5,0xa5,0x00,0x02,0x30,0xa3,0X2A};
		uint8 EcuComTxBuf_INQUARY_ACK[12]={0xa5,0xa5,0x00,0x07,0x30,0xa1,0x0f,0x64,0x9C,0x0A,0x9C,0x72};

//		printf("welcome to [%s]\r\n",__func__);
    if(UartTxSwitch == SEND_BLOCK)
    return;
		
    len = ( EcuComRxBuf[0]<<8) +  EcuComRxBuf[1];

    EcuComTxBuf[0]=0xA5;
    EcuComTxBuf[1]=0xA5;

    for(i=0; i<len + 2; i++)
    {
	    EcuComTxBuf[i+2]=(uint8)EcuComRxBuf[i];

	    chacknum = chacknum + (uint8)EcuComRxBuf[i];
    }

    EcuComTxBuf[i+2] = ~chacknum;
	
	
	#ifdef M_AUDIOQ
	if(memcmp(EcuComTxBuf,EcuComTxBuf_INQUARY,sizeof(EcuComTxBuf_INQUARY))==0)
	{
		USB_BULK_Queue_Write(&USB_RX_QueueData,(uint8_t *)&EcuComTxBuf_INQUARY_ACK,sizeof(EcuComTxBuf_INQUARY_ACK));
		return;
	}
	#endif
	
    #ifdef M_ECU_DEBUG
	//bluewifidatatransmit((uint8_t *)EcuComTxBuf, 2+2+len+1);
	#endif
	USB_BULK_Queue_Write(&OBD_RX_QueueData,EcuComTxBuf,2+2+len+1); 
 
	printfecudebug(EcuComTxBuf);
}

void UartTxQueuePack_DMA(uint8* EcuComRxBuf)
{
	uint8 chacknum = 0;
	uint16 len = 0;
	uint16 i = 0;

	uint8 EcuComTxBuf_INQUARY[7]={0xa5,0xa5,0x00,0x02,0x30,0xa3,0X2A};
	uint8 EcuComTxBuf_INQUARY_ACK[12]={0xa5,0xa5,0x00,0x07,0x30,0xa1,0x0f,0x64,0x9C,0x0A,0x9C,0x72};

	if(UartTxSwitch == SEND_BLOCK)
	return;

	len = ( EcuComRxBuf[0]<<8) +  EcuComRxBuf[1];

	EcuComTxBuf[0]=0xA5;
	EcuComTxBuf[1]=0xA5;

	for(i=0; i<len + 2; i++)
	{
		EcuComTxBuf[i+2]=(uint8)EcuComRxBuf[i];

		chacknum = chacknum + (uint8)EcuComRxBuf[i];
	}

	EcuComTxBuf[i+2] = ~chacknum;
	
	
	#ifdef M_AUDIOQ
	if(memcmp(EcuComTxBuf,EcuComTxBuf_INQUARY,sizeof(EcuComTxBuf_INQUARY))==0)
	{
		USB_BULK_Queue_Write(&USB_RX_QueueData,(uint8_t *)&EcuComTxBuf_INQUARY_ACK,sizeof(EcuComTxBuf_INQUARY_ACK));
		return;
	}
	#endif
	
 
	bluewifidatatransmit((uint8_t *)EcuComTxBuf, 2+2+len+1);
 
	 
	printfecudebug(EcuComTxBuf);
}

void UARTAnswer(uint8 cmd)
{
	memset(EcuComRxBuf,0,280);
	//uint8 EcuComRxBuf[16];
	EcuComRxBuf[0] = 0x00;
	EcuComRxBuf[1] = 0x01;
	EcuComRxBuf[2] = cmd;
	UartTxQueuePack(EcuComRxBuf);
}

void UARTAnswerErr(uint8 cmd,uint8 para)
{
	// uint8 EcuComRxBuf[16];
	memset(EcuComRxBuf,0,280);
	EcuComRxBuf[0] = 0x00;
	EcuComRxBuf[1] = 0x02;
	EcuComRxBuf[2] = cmd;
	EcuComRxBuf[3] = para;
	UartTxQueuePack(EcuComRxBuf);
}

void UartTxQueuePack_DelCanID(uint8 *queuebuf)
{
	uint16 len = 0;
	if(UartTxSwitch == SEND_BLOCK)
		return;
	len = (EcuComRxBuf[0]<<8) + EcuComRxBuf[1]-4;
	EcuComRxBuf[0]=(len>>8)&0xff;
	EcuComRxBuf[1]=len&0xff;
	memcpy(EcuComRxBuf+3,EcuComRxBuf+7,len);
	UartTxQueuePack(EcuComRxBuf);
}

/**********************************************************
*******************串口1发送数据***************************
**********************************************************/
void USART_PushByte(uint8 Data)
{
	if(UartTxCount < TXQUEUEBUFLEN)
	{
		UartTxQueueBuf[UartTxCount++] = Data;
	}
}

void USART_TxQueueEndBuf()
{
	while(UartTxCount > 0)
	{
		if(UartTxCount < DMATXBUFLEN)
		{
			//USART_DMA_Send_ext(UartTxQueueBuf_ext, UartTxCount);
			UartTxCount = 0;
		}
		else
		{
			//USART_DMA_Send_ext(UartTxQueueBuf_ext, DMATXBUFLEN);
			UartTxCount = UartTxCount - DMATXBUFLEN;
		}
	}
}

void USART1_ClearQueue()
{
	UartTxCount = 0;
}
void USART_DMA_Send_ext(uint8 *Dat,uint16_t len)
{
#ifdef M_ECU_DEBUG
	///bluewifidatatransmit((uint8_t *)Dat, len);
#endif


	USB_BULK_Queue_Write(&OBD_RX_QueueData,Dat,len); 	 
   
   printfecudebug(Dat);
}


void USART_DMA_Send_ext_1768(void)
{
	uint8_t *Dat=0;
	uint16_t len;
	len=(UartTxQueueBuf[2]<<8)+UartTxQueueBuf[3];
	Dat=UartTxQueueBuf_ext;
	len+=5;
 
	bluewifidatatransmit((uint8_t *)Dat, len);
	 
	
	printfecudebug(Dat);
}