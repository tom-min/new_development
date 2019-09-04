
/*
 * USB_Queue.h
 *
 * Created: 2018/1/15 9:29:08
 *  Author: ppcc-02
 */ 
#ifndef __USER_QUEUE_H__
#define __USER_QUEUE_H__

#include <stdint.h>
#include <stdbool.h>
#include "stm32f2xx_can.h"
#include "VCI_queue.h"

#define USER_UDI_VENDOR_EPS_SIZE_BULK_HS  512
//USB���ն��д�С10K  1024*10 ����󻺴�10������
#define USB_Queue_SIZE 512


//*******************************PWM recv queue*************************/
//PWM���ն��д�С0.5K 
#define PWM_Queue_SIZE 512 
typedef struct
{
	uint8_t *rear;     //��β
	uint8_t *front;    //��ͷ
	uint8_t *start;    //��ʼλ��
	uint8_t *end;      //����λ��
	int len;            //���г���
	int size;           //����������
	uint8_t buf[PWM_Queue_SIZE]; //���л�����
} PWM_QueueData;     //���нṹ��

typedef struct
{
	uint32_t *rear;     //��β
	uint32_t *front;    //��ͷ
	uint32_t *start;    //��ʼλ��
	uint32_t *end;      //����λ��
	int len;            //���г���
	int size;           //����������
	uint32_t buf[USB_Queue_SIZE]; //���л�����
} USART_QueueData;     //���нṹ��
//*******************************USB BULK recv queue*************************/
//���ն��д�С8K
#define USB_BULK_Queue_SIZE 1024*2  //CAN��֡�������ܴ� Ŀǰ���2000֡
typedef struct
{
	uint8_t *rear;	   //��β
	uint8_t *front;    //��ͷ
	uint8_t *start;    //��ʼλ��
	uint8_t *end;      //����λ��
	int len;           //���г���
	int size;          //����������
	uint8_t buf[USB_BULK_Queue_SIZE]; //���л�����
} USB_BULK_QueueData;     //���нṹ��


typedef struct 
{
	uint8_t m_TGT;
	uint8_t m_SRC;
	uint8_t m_CS_MODE;
	uint8_t m_PCK_TYPE;
	uint8_t m_ORD;
	uint8_t m_WAYS;
	uint8_t m_RWAYS;
	uint8_t m_AnswerByteTimeOut;
	uint8_t m_RequestByteTimeOut;
	uint8_t m_AnswerFrameTimeOut;
	uint8_t m_RequestFrameTimeOut;
	uint8_t m_ISOAnswerNum;
	uint8_t m_AutoKeepTimeOut;
	uint8_t m_KeepFlag;
	uint8_t m_SUBTYPE;
}DEFAULT_MSGATTR;


extern USB_BULK_QueueData OBD_RX_QueueData;
extern USB_BULK_QueueData USB_RX_QueueData;
extern USB_BULK_QueueData USB_TX_QueueData;


extern PWM_QueueData	PWM_RX0_QueueData;
extern PWM_QueueData	VPW_RX0_QueueData;
extern PWM_QueueData	PWM_RX1_QueueData;
extern PWM_QueueData	VPW_RX1_QueueData;


extern USART_QueueData	KWP0_RX_QueueData;
extern USART_QueueData	KWP1_RX_QueueData;
 

extern void PWM_Queue_Init(PWM_QueueData* PWM_Queue);
extern bool PWM_Queue_Write(PWM_QueueData* PWM_Queue, uint8_t *RxMessage);
extern bool PWM_Queue_Read(PWM_QueueData* PWM_Queue, uint8_t *RxMessage);

extern void USB_BULK_Queue_Init(USB_BULK_QueueData* USB_BULK_Queue);
extern bool USB_BULK_Queue_Read(USB_BULK_QueueData* USB_BULK_Queue, uint8_t *RxMessage, uint16_t *msgLen);
extern bool USB_BULK_Queue_Write(USB_BULK_QueueData* USB_BULK_Queue, uint8_t *RxMessage, uint16_t msgLen);

extern void USART_Queue_Init(USART_QueueData* USART_Queue);
extern bool USART_Queue_Read(USART_QueueData* USART_Queue, uint32_t *RxMessage);
extern bool USART_Queue_Write(USART_QueueData* USART_Queue, uint32_t *RxMessage);
#endif /* Queue_H_ */

