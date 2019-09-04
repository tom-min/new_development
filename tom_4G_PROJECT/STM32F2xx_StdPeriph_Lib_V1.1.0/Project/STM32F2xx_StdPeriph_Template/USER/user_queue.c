/*
 * USB_Queue.c
 *
 * Created: 2018/1/15 9:27:40
 *  Author: lizuquan
 */ 
 
#include "user_queue.h"
#include "conf_PDU.h"
 
#include "string.h" 
#include "user_config.h" 
 
USART_QueueData	KWP0_RX_QueueData;
USART_QueueData	KWP1_RX_QueueData;

USB_BULK_QueueData	USB_RX_QueueData;
USB_BULK_QueueData  USB_TX_QueueData;

USB_BULK_QueueData	OBD_RX_QueueData;

//*********************************************�����ָ���******************************************************/
//����PWM���ն��� 
PWM_QueueData	PWM_RX0_QueueData;
PWM_QueueData	PWM_RX1_QueueData;
PWM_QueueData	VPW_RX0_QueueData;
PWM_QueueData	VPW_RX1_QueueData;
/*
 * ��������PWM_Queue_Init
 * ����  �����г�ʼ��
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void PWM_Queue_Init(PWM_QueueData* PWM_Queue)
{
    PWM_Queue->len = 0;
    PWM_Queue->size = PWM_Queue_SIZE;
    PWM_Queue->start = PWM_Queue->buf;
    PWM_Queue->end = PWM_Queue->start + PWM_Queue_SIZE;
    PWM_Queue->rear = PWM_Queue->start;
    PWM_Queue->front = PWM_Queue->start;
}

/*
 * ��������PWM_Queue_Read
 * ����  �����г�ʼ��
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
bool PWM_Queue_Read(PWM_QueueData* PWM_Queue, uint8_t *RxMessage)
{
    uint8_t i;
	if(PWM_Queue->size == 0)
    {
        return false;
    }
    if(PWM_Queue->len > 0)
    {	
		uint8_t pwm_data_len = *PWM_Queue->front;
		*RxMessage= *PWM_Queue->front++;    //data[0]
		RxMessage++;
		if(PWM_Queue->front == PWM_Queue->end)
			PWM_Queue->front = PWM_Queue->start;
		
		for ( i=0;i<pwm_data_len;i++)
		{
			*RxMessage= *PWM_Queue->front++;    //data[0]
			RxMessage++;
			if(PWM_Queue->front == PWM_Queue->end)
				PWM_Queue->front = PWM_Queue->start;
		}
		PWM_Queue->len = PWM_Queue->len - pwm_data_len - 1;   
		return true;
    }
	else
	{
		return false;
	}
}


/*
 * ��������PWM_Queue_Write
 * ����  �����г�ʼ��
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
bool PWM_Queue_Write(PWM_QueueData* PWM_Queue, uint8_t *RxMessage)
{
	uint8_t i;
	uint8_t pwm_data_len ;
    if(PWM_Queue->size == 0) //??
    {
        return false;
    }
	//���ʣ��buf�Ƿ��㹻������һ֡����
	 pwm_data_len = *RxMessage;
	if((PWM_Queue->len + pwm_data_len + 1) < PWM_Queue->size)  //��ʱ new code ��Ҫʵ����֤�Ƿ���ȷ 
    {
		*PWM_Queue->rear = *RxMessage++;    //data[0]
		PWM_Queue->rear++;
		if(PWM_Queue->rear == PWM_Queue->end)
			PWM_Queue->rear = PWM_Queue->start;
		
		for (i=0;i<pwm_data_len;i++)
		{
			*PWM_Queue->rear = *RxMessage++;    //data[1] .. data[n] 
			PWM_Queue->rear++;
			if(PWM_Queue->rear == PWM_Queue->end)
				PWM_Queue->rear = PWM_Queue->start;	
		}
		PWM_Queue->len = PWM_Queue->len + pwm_data_len + 1;  
		return true;
    }
	else
	{
		return false;
	}

}

//*********************************************�����ָ��� usb******************************************************/
//����USB_BULK���ն���  8K byte
//USB_BULK_QueueData	USB_BULK_RX_QueueData;

/*
 * ��������USB_BULK_Queue_Init
 * ����  �����г�ʼ��
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void USB_BULK_Queue_Init(USB_BULK_QueueData* USB_BULK_Queue)
{
    USB_BULK_Queue->len = 0;
    USB_BULK_Queue->size = USB_BULK_Queue_SIZE;
    USB_BULK_Queue->start = USB_BULK_Queue->buf;
    USB_BULK_Queue->end = USB_BULK_Queue->start + USB_BULK_Queue_SIZE;
    USB_BULK_Queue->rear = USB_BULK_Queue->start;
    USB_BULK_Queue->front = USB_BULK_Queue->start;
}


uint32_t win_usb_cnt = 0;
/*
 * ��������USB_BULK_Queue_Read ��������
 * ֡���ݽṹ ��msgLen ����msg����
 */
bool USB_BULK_Queue_Read(USB_BULK_QueueData* USB_BULK_Queue, uint8_t *RxMessage, uint16_t *msgLen)
{
		uint16_t i;
	
    if(USB_BULK_Queue->size == 0)
    {
        return false;
    }
    if(USB_BULK_Queue->len > 2)
    {
		//printf("%u\r\n",win_usb_cnt++); //test code
		//printf("%u\r\n",USB_BULK_Queue->len); //test code
		uint16_t usb_bulk_data_len = *USB_BULK_Queue->front++;  //data[0]
		if(USB_BULK_Queue->front == USB_BULK_Queue->end)
		{
			USB_BULK_Queue->front = USB_BULK_Queue->start;
		}
			
		usb_bulk_data_len = (usb_bulk_data_len <<8) | *USB_BULK_Queue->front++;  //data[0]
		if(USB_BULK_Queue->front == USB_BULK_Queue->end)
		{
			USB_BULK_Queue->front = USB_BULK_Queue->start;
		}
		
		*msgLen = usb_bulk_data_len;
		
		for (  i=0;i<usb_bulk_data_len;i++)
		{
			*RxMessage= *USB_BULK_Queue->front++;              //data[1] .. data[n]
			RxMessage++;
			if(USB_BULK_Queue->front == USB_BULK_Queue->end)
			{
				USB_BULK_Queue->front = USB_BULK_Queue->start;
			}
		}
		USB_BULK_Queue->len = USB_BULK_Queue->len - usb_bulk_data_len - 2;   
		return true;
    }
	else
	{
		return false;
	}
}


/*
 * ��������USB_BULK_Queue_Write
 * ֡���ݽṹ ��msgLen message����
 */
bool USB_BULK_Queue_Write(USB_BULK_QueueData* USB_BULK_Queue, uint8_t *RxMessage, uint16_t msgLen)
{
		uint16_t i;
		uint16_t usb_bulk_data_len;
	
    if(USB_BULK_Queue->size == 0) //??
    {
        return false;
    }
		//���ʣ��buf�Ƿ��㹻������һ֡����
		 usb_bulk_data_len = msgLen;
    if((USB_BULK_Queue->len + usb_bulk_data_len + 1) < USB_BULK_Queue->size)  
    {
				*USB_BULK_Queue->rear = (msgLen>>8) & 0xff;      //data[0]
				USB_BULK_Queue->rear++;
				if(USB_BULK_Queue->rear == USB_BULK_Queue->end)
				{
						USB_BULK_Queue->rear = USB_BULK_Queue->start;
				}
			
				*USB_BULK_Queue->rear = msgLen & 0xff;      //data[0]
				USB_BULK_Queue->rear++;
				if(USB_BULK_Queue->rear == USB_BULK_Queue->end)
				{
						USB_BULK_Queue->rear = USB_BULK_Queue->start;
				}
		
		for (  i=0;i<usb_bulk_data_len;i++)
		{
			*USB_BULK_Queue->rear = *RxMessage++;    //data[1] .. data[n] 
			USB_BULK_Queue->rear++;
			if(USB_BULK_Queue->rear == USB_BULK_Queue->end)
			{
				USB_BULK_Queue->rear = USB_BULK_Queue->start;
			}	
		}
		USB_BULK_Queue->len = USB_BULK_Queue->len + usb_bulk_data_len + 2;   
		return true;
    }
	else
	{
		//printf("Boo\r\n"); //test code
		return false;
	}
}


void USART_Queue_Init(USART_QueueData* USART_Queue)
{
	USART_Queue->len = 0;
	USART_Queue->size = USB_Queue_SIZE;
	USART_Queue->start = USART_Queue->buf;
	USART_Queue->end = USART_Queue->start + USB_Queue_SIZE;
	USART_Queue->rear = USART_Queue->start;
	USART_Queue->front = USART_Queue->start;
}



bool USART_Queue_Read(USART_QueueData* Queue, uint32_t *RxMessage)
{
    if(Queue->size == 0)
    {
	    return false;
    }
    if(Queue->len > 0)
    {
	    *RxMessage = *Queue->front;  //��������
	    Queue->front++;				 //��ַ��һ
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


bool USART_Queue_Write(USART_QueueData* Queue, uint32_t *RxMessage)
{
    if(Queue->size == 0)
    {
	    return false;
    }
    if(Queue->len < Queue->size)
    {
	    *Queue->rear = *RxMessage;   //д�����
	    Queue->rear++;				//��ַ��һ
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
