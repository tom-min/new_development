#ifndef __VCI_QUEUE_H
#define __VCI_QUEUE_H
#include "user_stm32f2xx.h"
#define TRUE 1
#define FALSE 0

#define QUEUE_SIZE 128

#define ElemType CanRxMsg

typedef struct
{
    CanRxMsg *rear;     //��β
    CanRxMsg *front;    //��ͷ
    CanRxMsg *start;    //��ʼλ��
    CanRxMsg *end;      //����λ��
    int len;            //���г���
    int size;           //����������
    CanRxMsg buf[QUEUE_SIZE]; //���л�����
} QueueData;     //���нṹ

 extern QueueData CAN1_QueueData;
 extern QueueData CAN2_QueueData;

void Queue_Init(QueueData* Queue);
void Queue_Read(QueueData* Queue, CanRxMsg* RxMessage);
void Queue_Write(QueueData* Queue,  CanRxMsg* RxMessage);
void Queue_Empty(QueueData* Queue);

#endif /* __QUEUE_H */
