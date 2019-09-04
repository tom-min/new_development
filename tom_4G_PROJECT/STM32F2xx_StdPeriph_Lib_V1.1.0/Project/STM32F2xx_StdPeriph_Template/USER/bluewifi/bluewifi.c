
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "user_stm32f2xx.h"
#include "bluewifi.h"
#include "user_queue.h"
#include "conf_PDU.h"
#include "interface.h"
#include "stm322xg_eval.h"
#include "user_config.h"
#define  MSB(u16)       (((uint8  *)&(u16))[1]) //!< Most significant byte of \a u16.
#define  LSB(u16)       (((uint8  *)&(u16))[0]) //!< Least significant byte of \a u16.
#if 1
//void printfuartdebug(const char *fmt,...);
void printftoolsdebug(const char *dat);
void printfecudebug(char *dat);
void printffflowcontroldebug(char *dat);
void printffflowcontroldebug2(char *dat,char len,uint32_t canid );

void printferror(const char *fmt,...);
void printfiso(char *fmt,...);
void printfkwp(const char *fmt,...);
void printf_hex(char *data,uint16_t len);
#endif


/////////////// user will two function ///////////////////////////
typedef unsigned char  uint8;
extern uint8 TempProcBuf[];
extern uint8 gswitchprotocol;

uint8 gprintfecutoolenable=0;
extern void Work_Thread(void);
extern void _MainRoute(void);

uint32 gsofttimeer=0;
uint32 gsofttimeerold=0;
volatile uint32 gEcuSelFTest=0;


void USART_TX(char *string,USART_TypeDef* USARTx)
{
	uint32_t i;
	for(i=0;i<strlen(string);i++)
	{
		USART_SendData(USARTx,(unsigned char)string[i]);        
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC) != SET);      
	}
}


void printfuartdebug(const char *fmt,...)
{
	char string[256];
	va_list ap;
       memset(string,0,sizeof(string));
	if(gEcuSelFTest==0)
	{
#ifndef M_ECU_DEBUG
		return;
#endif
	}
	va_start(ap,fmt);
	vsprintf(string,fmt,ap);  //Use It Will Increase the code size, Reduce the efficiency
	USART_TX(string,PRINTF_DEBUG2_UART);
	va_end(ap);
}

void printfUartObdiideBug(const char *fmt,...)
{
	char string[256]; 
	va_list ap;
	memset(string,0,sizeof(string));
#ifndef M_ODBII_DEBUG
	return;
#endif
	va_start(ap,fmt);
	vsprintf(string,fmt,ap);  //Use It Will Increase the code size, Reduce the efficiency
	USART_TX(string,PRINTF_DEBUG2_UART);
	va_end(ap);
}

void printftoolsdebug(const char *dat)
{
    uint16_t i,len;
    uint32_t dif;
 
    //if(gprintfecutoolenable==0)return;
    if(gEcuSelFTest==0)
    {
#ifndef M_ECU_DEBUG
        return;
#endif
    }

    dif=gsofttimeer-gsofttimeerold;
    gsofttimeerold=gsofttimeer;
    len = (dat[0] << 8) + dat[1];

    printfuartdebug("VCI %d %04d >> a5 a5 ",gsofttimeer,dif);
    for(i=0; i<len+3; i++)
    {
        printfuartdebug("%02x ",dat[i]);
    }
    printfuartdebug("\r\n");
}

void printfecudebug(char *dat)
{
    uint16_t len = 0;
    uint16_t i = 0;
    uint32_t dif;
    //if(gprintfecutoolenable==0)return;
    if(gEcuSelFTest==0)
    {
#ifndef M_ECU_DEBUG
        return;
#endif
    }

    dif=gsofttimeer-gsofttimeerold;
    gsofttimeerold=gsofttimeer;

    printfuartdebug("ECU %d %04d << ",gsofttimeer,dif);
    len = (dat[2]<<8) +  dat[3];
    for(i=0; i<len+5; i++)
        printfuartdebug("%02x ",dat[i]);

    printfuartdebug("\r\n");
}

void printfuartdebugtimestamp(const char *fmt,...)
{
	char string[256];
#if 1
	uint32_t dif;
	va_list ap;
#ifndef M_ECU_DEBUGTIMESTAMP
	return;
#endif

	memset(string,0,sizeof(string));
	dif=gsofttimeer-gsofttimeerold;
	gsofttimeerold=gsofttimeer;
	va_start(ap,fmt);
	vsprintf(string,fmt,ap);  //Use It Will Increase the code size, Reduce the efficiency

	USART_TX(string,PRINTF_DEBUG2_UART);
	va_end(ap);
	printfuartdebug(" %d\r\n",dif);
#endif
}

void printffflowcontroldebug(char *dat)
{
    uint16_t len = 0;
    uint16_t i = 0;
#ifndef M_ECU_DEBUG
    return;
#endif
#if 0
    //if(gprintfecutoolenable==0)return;
    printfuartdebug("flow >> ");
    len = (dat[2]<<8) +  dat[3];
    for(i=0; i<len+5; i++)
        printfuartdebug("%02x ",dat[i]);
    printfuartdebug("\r\n");
#endif
}

void printffflowcontroldebug2(char *dat, char len, uint32_t canid )
{

    uint16_t i = 0;
#ifndef M_ECU_DEBUG
    return;
#endif
    //if(gprintfecutoolenable==0)return;
#if 0
    printfuartdebug("flow >> ");

    for(i=0; i<len ; i++)
        printfuartdebug("%02x ",dat[i]);
    printfuartdebug(" %08x ",canid);
    printfuartdebug("\r\n");
#endif
}

void printf_hex(char *data,uint16_t len)
{
    uint16_t i;
    for(i=0; i<len; i++)
    {
//		while( uart_write((Uart *) UART1,data[i]) );
    }

}

void printf_usb(const char *fmt,...)
{
    char string[256];
    va_list ap;
    //if(gusbresume ==0)return;
    //if(gusbprintfeable == 0)return;
    return;
    va_start(ap,fmt);
    vsprintf(string,fmt,ap);  //Use It Will Increase the code size, Reduce the efficiency
    va_end(ap);
    USB_SendData((uint8_t *)&string[0],strlen(string)); //把格式化字符串从开发板串口送出去
}

void printferror(const char *fmt,...)
{
    char string[256];
    va_list ap;
    //if(gusbresume ==0)return;
    //if(gusbprintfeable == 0)return;
    return;
    //vTaskDelay(100);
    va_start(ap,fmt);
    vsprintf(string,fmt,ap);  //Use It Will Increase the code size, Reduce the efficiency
    va_end(ap);
    USB_SendData((uint8_t *)&string[0],strlen(string)); //把格式化字符串从开发板串口送出去
}
void printffatalerror(const char *fmt,...)
{
    char string[256];
    va_list ap;
    //if(gusbresume == 0)return;
    va_start(ap,fmt);
    vsprintf(string,fmt,ap);  //Use It Will Increase the code size, Reduce the efficiency
    va_end(ap);
    //USB_SendData((uint8_t *)&string[0],strlen(string)); //把格式化字符串从开发板串口送出去
}
void printfkwp(const char *fmt,...)
{
	char string[256];

	va_list ap;
#ifndef M_ECU_DEBUG
	return;
#endif
	memset(string,0,sizeof(string));
	va_start(ap,fmt);
	vsprintf(string,fmt,ap);  //Use It Will Increase the code size, Reduce the efficiency
	va_end(ap);
	 USART_TX(string,PRINTF_DEBUG2_UART);

}
void printfiso(char *fmt,...)
{
    char string[256];
    va_list ap;
    //if(gusbresume ==0)return;
    return;
    //if(gusbprintfeable == 0)return;
    va_start(ap,fmt);
    vsprintf(string,fmt,ap);  //Use It Will Increase the code size, Reduce the efficiency
    va_end(ap);
    //USB_SendData((uint8_t *)&string[0],strlen(string)); //把格式化字符串从开发板串口送出去
}
//uint8_t rxpacketbuf[USER_UDI_VENDOR_EPS_SIZE_BULK_HS] = { 0 };
uint8_t rxpacketbuf[TXQUEUEBUFLEN] = { 0 };
uint16_t seqnum = 0;
uint8_t interfacetype=0xff;


////////////////////////////////////

void bluewifidatatransmit(uint8_t *data,uint16_t len)
{
    uint16_t i;
    for(i=0; i<len; i++)
    {

        USART_SendData(EVAL_COM4,data[i]);
        while(USART_GetFlagStatus(EVAL_COM4,USART_FLAG_TC) != SET);
        if((i%512==0)&&(i!=0))vTaskDelay(30);
    }
}

void bluewifidatereceive(uint8_t *data)
{
    static uint16_t len=0;
    rxpacketbuf[seqnum] = *data;
    switch(seqnum)
    {
    case 0:
    case 1:
        if(*data != 0xa5)
        {
            seqnum=0;
            rxpacketbuf[seqnum]=0;
            len = 0;
        }
        else
        {
            seqnum++;
        }
        break;

    case 3:

        len = rxpacketbuf[2];
        len = (len<<8) +  rxpacketbuf[3];
        seqnum++;
        break;

    default:
        seqnum++;
        break;
    }
    if( (len + 4 + 1) == seqnum)
    {

#if 1
        USB_BULK_Queue_Write(&USB_RX_QueueData,(uint8_t *)&rxpacketbuf,seqnum);
        memset(rxpacketbuf,0,seqnum);
        seqnum=0;
        len=0;
#endif


    }
}


