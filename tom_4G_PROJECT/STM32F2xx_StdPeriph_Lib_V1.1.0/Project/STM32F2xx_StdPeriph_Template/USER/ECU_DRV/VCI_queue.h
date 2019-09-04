#ifndef __VCI_QUEUE_H
#define __VCI_QUEUE_H
#include "user_stm32f2xx.h"
#define TRUE 1
#define FALSE 0

#define QUEUE_SIZE 128

#define ElemType CanRxMsg

typedef struct
{
    CanRxMsg *rear;     //队尾
    CanRxMsg *front;    //队头
    CanRxMsg *start;    //开始位置
    CanRxMsg *end;      //结束位置
    int len;            //队列长度
    int size;           //队列总容量
    CanRxMsg buf[QUEUE_SIZE]; //队列缓冲区
} QueueData;     //队列结构

 extern QueueData CAN1_QueueData;
 extern QueueData CAN2_QueueData;

void Queue_Init(QueueData* Queue);
void Queue_Read(QueueData* Queue, CanRxMsg* RxMessage);
void Queue_Write(QueueData* Queue,  CanRxMsg* RxMessage);
void Queue_Empty(QueueData* Queue);

#endif /* __QUEUE_H */
