#ifndef TOM_QUEUE_H_
#define TOM_QUEUE_H_

#include <stdbool.h>
#include <stdint.h>

#define TOM_Queue_SIZE 128


typedef struct
{
	uint16_t *rear;     //队尾
	uint16_t *front;    //队头
	uint16_t *start;    //开始位置
	uint16_t *end;      //结束位置
	uint16_t len;       //队列长度
	uint16_t size;      //队列总容量
	uint16_t buf[TOM_Queue_SIZE]; //队列缓冲区
} Tom_QueueData;     //队列结构体


extern void Tom_Queue_Init(Tom_QueueData* Tom_Queue);
extern bool Tom_Queue_Out(Tom_QueueData* Queue, uint16_t *RxMessage);
extern bool Tom_Queue_In(Tom_QueueData* Queue, uint16_t TxMessage);
extern bool Tom_Queue_IsEmpty(Tom_QueueData* Queue);

#endif /* Queue_H_ */

