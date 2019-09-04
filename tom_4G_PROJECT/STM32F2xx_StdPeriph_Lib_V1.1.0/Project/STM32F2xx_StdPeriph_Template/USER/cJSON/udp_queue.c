#include "udp_queue.h"

//初始化队列
void InitQueue(pudp_queue *Q)
{
    *Q = (pudp_queue)malloc(sizeof(udp_queue));
    if(!(*Q))
    {
        printf("InitQueue malloc failed\r\n");
        return;
    }
    (*Q)->front = NULL;
    (*Q)->rear  = (*Q)->front;

    printf("InitQueue malloc successful.\r\n");
}


//判断队列是否为空
bool EmptyQueue(pudp_queue q)
{
    if(q->front == NULL && q->front == q->rear)
        return true;
    else
        return false;
}


//入队
void UDP_Queue_In(pudp_queue q, cJSON *data_json)
{
    pudp_node new;
    new = (pudp_node)malloc(sizeof(udp_node));
    if(new == NULL)//内存分配失败
    {
        printf("InsertQueue malloc failed\r\n");
        return;
    }
    new->data_json = data_json;
    new->next = NULL;
    //这种队列的q->front为第一个元素，与我以前的q->front->next为第一个元素有点区别
    if(q->rear == NULL)
    {
        q->front = new;
        q->rear = new;
    }
    else
    {
        q->rear->next = new;//让new成为当前的尾部节点下一节点
        q->rear = new;				//尾部指针指向new
    }
}


//出队
void UDP_Queue_Out(pudp_queue q,pudp_node* tom_queue)
{
	pudp_node tmp;
	tmp = q->front;
	if(q->front == NULL)//判断队列是否为空
	{
		return;
	}
	if(q->front == q->rear)//是否只有一个元素
	{
		q->front = NULL;
		q->rear = NULL;
		//free(tmp);
	}
	else
	{
		q->front = q->front->next;
		//free(tmp);
	}
	*tom_queue = tmp;
}

