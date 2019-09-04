/**
  ******************************************************************************
  * @file    Project/STM32F2xx_StdPeriph_Template/stm32f2xx_it.c 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    13-April-2012
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_it.h"
#include "main.h"
#include "config.h"
#include "user_stm32f2xx.h"
#include "VCI_queue.h"
#include "user_queue.h"
#include "FreeRTOS.h"
#include "task.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler_nouse(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler_nouse(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler_nouse(void)
{
   
}

 

#define USARTx1_IRQHANDLER   USART1_IRQHandler
#define USARTx2_IRQHANDLER   USART2_IRQHandler
#define USARTx3_IRQHANDLER   USART3_IRQHandler
#define USARTx4_IRQHANDLER   UART4_IRQHandler
#define USARTx5_IRQHANDLER   UART5_IRQHandler
#define USARTx6_IRQHANDLER   USART6_IRQHandler
 #define TIM2_IRQHANDLER  TIM2_IRQHandler
 #define TIM3_IRQHANDLER  TIM3_IRQHandler

///////////////////////////////////////////////////////////////////////

__IO uint16_t RxCounter = 0; 
uint16_t NbrOfDataToRead = RXBUFFERSIZE;
uint8_t RxBuffer [RXBUFFERSIZE];

///////////////////////////////////////
uint8_t RxBuffer2 [RXBUFFERSIZE_UBOX];

uint16_t NbrOfDataToRead2 = RXBUFFERSIZE_UBOX;

__IO uint16_t RxCounter2 = 0; 
///////////////////////////////////////////////////

void USARTx1_IRQHANDLER(void)
{
  if(USART_GetITStatus(EVAL_COM1, USART_IT_RXNE) != RESET)
  {
    /* Read one byte from the receive data register */
    RxBuffer[RxCounter++] = (USART_ReceiveData(EVAL_COM1) & 0x7F);

    if(RxCounter == NbrOfDataToRead)
    {
      /* Disable the EVAL_COM1 Receive interrupt */
     RxCounter=0;
    }
  }

}
void USARTx2_IRQHANDLER(void)
{
	
	UBaseType_t uxSaveIRQ_Status;
	uxSaveIRQ_Status = taskENTER_CRITICAL_FROM_ISR();
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		/* Read one byte from the receive data register */
		RxBuffer[RxCounter++] = USART_ReceiveData(USART2);
		if(RxCounter == NbrOfDataToRead)
		{
			/* Disable the EVAL_COM1 Receive interrupt */
			RxCounter=0;
		}
	}
	taskEXIT_CRITICAL_FROM_ISR(uxSaveIRQ_Status);  
}
	
void USARTx3_IRQHANDLER(void)////u-blox M
{
	UBaseType_t uxSaveIRQ_Status;
	uxSaveIRQ_Status = taskENTER_CRITICAL_FROM_ISR();
 	
 	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		/* Read one byte from the receive data register */
		RxBuffer2[RxCounter2++] = USART_ReceiveData(USART3);

		if(RxCounter2 == NbrOfDataToRead2)
		{
			RxCounter2=0;
		}
	}
	taskEXIT_CRITICAL_FROM_ISR(uxSaveIRQ_Status);  

}


void USARTx4_IRQHANDLER(void)
{
	uint8_t data;
	if(USART_GetITStatus(EVAL_COM4, USART_IT_RXNE) != RESET)
	{
		/* Read one byte from the receive data register */
		data = (USART_ReceiveData(EVAL_COM4));
		 
		//printf("%USARTx4_IRQHANDLER\r\n ",data);
		bluewifidatereceive(&data);
		return;
	}

    
}


extern uint8_t gUSART0_Handler; 
extern uint8_t gUSART1_Handler; 

void USARTx5_IRQHANDLER(void)
{
	uint32_t USART_RecvBuf;
	 // printfkwp("USARTx5_IRQHANDLER\r\n");
	if(USART_GetITStatus(EVAL_COM5, USART_IT_RXNE) != RESET)
	{
		gUSART0_Handler++;
		
		USART_RecvBuf=(USART_ReceiveData(EVAL_COM5));
		// printfkwp("5=% 02X \r\n",USART_RecvBuf);
		USART_Queue_Write(&KWP0_RX_QueueData,&USART_RecvBuf);
	}
}

void USARTx6_IRQHANDLER(void)
{
	uint32_t USART_RecvBuf;
	 
	if(USART_GetITStatus(EVAL_COM6, USART_IT_RXNE) != RESET)
	{
		gUSART1_Handler++;
		USART_RecvBuf=(USART_ReceiveData(EVAL_COM6));
		 //printfkwp("6=%02X \r\n",USART_RecvBuf);
		USART_Queue_Write(&KWP1_RX_QueueData,&USART_RecvBuf);
	}
}

 void TIM2_IRQHANDLER(void)
 {
 	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 
 	{
 		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
 		 
 		//printf( "TIM2_IRQHANDLER\r\n" );
 	}
 }
  
 void TIM3_IRQHANDLER(void)
 {
 	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) 
 	{
 		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
 		 
 		//printf( "TIM3_IRQHANDLER\r\n" );
 	}
 }

extern uint8 can_test_status;
void CAN1_RX0_IRQHandler(void)
{
	
	CanRxMsg RxMessage;
	uint32_t can_it_status=CAN1->ESR; 

	if(( can_it_status&0x00008100)==0)//非错误中断
	{
		if (can_test_status == 0)//正常接收，直接写数据上去
		{
			CAN_Receive(CAN1,CAN_FIFO0, &RxMessage);	 // CAN接收数据
			Queue_Write(&CAN1_QueueData, &RxMessage);
		}
		else
		{
			UARTAnswerErr(0xfc, 01);                    //can自检
			can_test_status = 0;                         //置零表示自发发收成功
			CAN_Receive(CAN1,CAN_FIFO0, &RxMessage);	 // CAN接收数据
		}
	}
	else
	{
		CAN_ITConfig(CAN1,CAN_IT_FPM0_EWG,DISABLE);
	}
}

void CAN2_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;
	uint32_t can_it_status=CAN2->ESR; 

	if(( can_it_status&0x00008100)==0)//非错误中断
	{
		if (can_test_status == 0)//正常接收，直接写数据上去
		{
			CAN_Receive(CAN2,CAN_FIFO0, &RxMessage);	 // CAN接收数据
			Queue_Write(&CAN2_QueueData, &RxMessage);
		}
		else
		{
			UARTAnswerErr(0xfc, 01);                    //can自检
			can_test_status = 0;                         //置零表示自发发收成功
			CAN_Receive(CAN2,CAN_FIFO0, &RxMessage);	 // CAN接收数据
		}
	}
	else
	{
		CAN_ITConfig(CAN2,CAN_IT_FPM0_EWG,DISABLE);
	} 
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
