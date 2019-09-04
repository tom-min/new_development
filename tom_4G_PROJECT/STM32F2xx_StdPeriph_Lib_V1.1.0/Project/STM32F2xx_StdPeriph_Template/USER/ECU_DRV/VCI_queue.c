/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名  ：SysTick.c
 * 描述    ：SysTick 系统滴答时钟10us中断函数库,中断时间可自由配置，
 *           常用的有 1us 10us 1ms 中断。
 * 实验平台：野火STM32开发板
 * 硬件连接：-----------------
 *          |                 |
 *          |      无         |
 *          |                 |
 *           -----------------
 * 库版本  ：ST3.0.0
 *
 * 作者    ：fire  QQ: 313303034
 * 博客    ：firestm32.blog.chinaunix.net
**********************************************************************************/

#include "VCI_queue.h"
 
 QueueData CAN1_QueueData;
 QueueData CAN2_QueueData;
 
/*
 * 函数名：Queue_Init
 * 描述  ：队列初始化
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void Queue_Init(QueueData* Queue)
{
    Queue->len = 0;
    Queue->size = QUEUE_SIZE;
    Queue->start = Queue->buf;
    Queue->end = Queue->start + QUEUE_SIZE;
    Queue->rear = Queue->start;
    Queue->front = Queue->start;
}


/*
 * 函数名：Queue_Read
 * 描述  ：队列初始化
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void Queue_Read(QueueData* Queue, CanRxMsg* RxMessage)
{
    if(Queue->size == 0)
    {
        return;
    }
    if(Queue->len > 0)
    {
        //Data = Queue->front;
        RxMessage->StdId=Queue->front->StdId;
        RxMessage->ExtId=Queue->front->ExtId;
        RxMessage->RTR=Queue->front->RTR;  // 数据帧or远程
        RxMessage->IDE=Queue->front->IDE;	 // 标准or扩展
        RxMessage->DLC=Queue->front->DLC;
        RxMessage->FMI=Queue->front->FMI;
        RxMessage->Data[0]=Queue->front->Data[0];
        RxMessage->Data[1]=Queue->front->Data[1];
        RxMessage->Data[2]=Queue->front->Data[2];
        RxMessage->Data[3]=Queue->front->Data[3];
        RxMessage->Data[4]=Queue->front->Data[4];
        RxMessage->Data[5]=Queue->front->Data[5];
        RxMessage->Data[6]=Queue->front->Data[6];
        RxMessage->Data[7]=Queue->front->Data[7];
        if(Queue->front == Queue->end)
        {
            Queue->front = Queue->start;
        }
        else
        {
            Queue->front++;
        }
        Queue->len--;
    }
}


/*
 * 函数名：Queue_Write
 * 描述  ：队列初始化
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void Queue_Write(QueueData* Queue, CanRxMsg* RxMessage)
{
    if(Queue->size == 0)
    {
        return;
    }
    if(Queue->len < Queue->size)
    {
        //Queue->rear = Data;
        Queue->rear->StdId=RxMessage->StdId;
        Queue->rear->ExtId=RxMessage->ExtId;
        Queue->rear->RTR=RxMessage->RTR;  // 数据帧or远程
        Queue->rear->IDE=RxMessage->IDE;	 // 标准or扩展
        Queue->rear->DLC=RxMessage->DLC;
        Queue->rear->FMI=RxMessage->FMI;
        Queue->rear->Data[0]=RxMessage->Data[0];
        Queue->rear->Data[1]=RxMessage->Data[1];
        Queue->rear->Data[2]=RxMessage->Data[2];
        Queue->rear->Data[3]=RxMessage->Data[3];
        Queue->rear->Data[4]=RxMessage->Data[4];
        Queue->rear->Data[5]=RxMessage->Data[5];
        Queue->rear->Data[6]=RxMessage->Data[6];
        Queue->rear->Data[7]=RxMessage->Data[7];
        if(Queue->rear == Queue->end)
        {
            Queue->rear = Queue->start;
        }
        else
        {
            Queue->rear++;
        }
        Queue->len++;
    }

}


/*
 * 函数名：Queue_Empty
 * 描述  ：队列初始化
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void Queue_Empty(QueueData* Queue)
{

}


/******************* (C) COPYRIGHT 2011 野火嵌入式开发工作室 *****END OF FILE****/
