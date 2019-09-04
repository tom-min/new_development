#ifndef __UDP_QUEUE
#define __UDP_QUEUE



#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cJSON.h"


//定义节点
typedef struct node{
    cJSON *data_json;
    struct node *next;
}udp_node,*pudp_node;
 
 
//定义队列（保存队首和队尾指针）
typedef struct queue_link{
    pudp_node front,rear;
}udp_queue,*pudp_queue;


extern void InitQueue(pudp_queue *Q);
extern bool EmptyQueue(pudp_queue q);
extern void UDP_Queue_In(pudp_queue q, cJSON *data_json);
extern void UDP_Queue_Out(pudp_queue q,pudp_node* tom_queue);


#endif
