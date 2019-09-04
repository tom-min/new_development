#include "my_queue.h"



void Tom_Queue_Init(Tom_QueueData* Tom_Queue)
{
		Tom_Queue->len = 0;
		Tom_Queue->size = TOM_Queue_SIZE;
		Tom_Queue->start = Tom_Queue->buf;
		Tom_Queue->end = Tom_Queue->start + TOM_Queue_SIZE;
		Tom_Queue->rear = Tom_Queue->start;
		Tom_Queue->front = Tom_Queue->start;
}


//出队
bool Tom_Queue_Out(Tom_QueueData* Queue, uint16_t *RxMessage)
{
    if(Queue->size == 0)
    {
				return false;
    }
    if(Queue->len > 0)
    {
				*RxMessage = *Queue->front;  //读出队列
				Queue->front++;				 //地址加一
				if(Queue->front == Queue->end)
				{
						Queue->front = Queue->start;
				}
				Queue->len--;
				return true;
    }
    else
    {
				return false;
    }
}


//入队
bool Tom_Queue_In(Tom_QueueData* Queue, uint16_t TxMessage)
{
    if(Queue->size == 0)
    {
				return false;
    }
    if(Queue->len < Queue->size)
    {
				*Queue->rear = TxMessage;   //写入队列
				Queue->rear++;				//地址加一
				if(Queue->rear == Queue->end)
				{
						Queue->rear = Queue->start;
				}
				Queue->len++;
				return true;
    }
    else
    {
				return false;
    }
}


bool Tom_Queue_IsEmpty(Tom_QueueData* Queue)
{
		if( (Queue->len == 0) && (Queue->front == Queue->rear))
				return true;
		else
				return false;
}

