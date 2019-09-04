/******************** (C) COPYRIGHT 2011 Ұ��Ƕ��ʽ���������� ********************
 * �ļ���  ��SysTick.c
 * ����    ��SysTick ϵͳ�δ�ʱ��10us�жϺ�����,�ж�ʱ����������ã�
 *           ���õ��� 1us 10us 1ms �жϡ�
 * ʵ��ƽ̨��Ұ��STM32������
 * Ӳ�����ӣ�-----------------
 *          |                 |
 *          |      ��         |
 *          |                 |
 *           -----------------
 * ��汾  ��ST3.0.0
 *
 * ����    ��fire  QQ: 313303034
 * ����    ��firestm32.blog.chinaunix.net
**********************************************************************************/

#include "VCI_queue.h"
 
 QueueData CAN1_QueueData;
 QueueData CAN2_QueueData;
 
/*
 * ��������Queue_Init
 * ����  �����г�ʼ��
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
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
 * ��������Queue_Read
 * ����  �����г�ʼ��
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
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
        RxMessage->RTR=Queue->front->RTR;  // ����֡orԶ��
        RxMessage->IDE=Queue->front->IDE;	 // ��׼or��չ
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
 * ��������Queue_Write
 * ����  �����г�ʼ��
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
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
        Queue->rear->RTR=RxMessage->RTR;  // ����֡orԶ��
        Queue->rear->IDE=RxMessage->IDE;	 // ��׼or��չ
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
 * ��������Queue_Empty
 * ����  �����г�ʼ��
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void Queue_Empty(QueueData* Queue)
{

}


/******************* (C) COPYRIGHT 2011 Ұ��Ƕ��ʽ���������� *****END OF FILE****/
