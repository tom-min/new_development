//#include <asf.h>
#include "config.h"
#include "PassThru.h"
#include "PassThru_KWP.h"
#include "interface.h"
#include "hardware.h"
#include "PortAssign.h"
#include "usart1.h"
#include "newsoftuart.h"
//#include "hal_uart.h"
#include "ProtocolDrives.h"
#include "PortAssign.h"
#include "conf_PDU.h"
#include "user_queue.h" 
#include "bluewifi.h" 
#include "usart.h" 
#include "user_stm32f2xx.h"
#include "stm322xg_eval.h"
#include "stm32f2xx_usart.h"
#include "time.h"
void HAL_MINIVCI_UART_Clear(void);
#define UART_MR_PAR_NONE USART_Parity_No
#define UART_MR_PAR_EVEN USART_Parity_Even
#define UART_MR_PAR_ODD USART_Parity_Odd


#define KWP_UART_PAR_NO USART_Parity_No
#define KWP_UART_PAR_EVEN USART_Parity_Even
#define KWP_UART_PAR_ODD USART_Parity_Odd

#define hal_tc2_ch0_Init T_delay_us_init
 
#define US_MR_MODE9 USART_WordLength_9b
 
uint8_t gUSART0_Handler=0; 
uint8_t gUSART1_Handler=0; 

#define zyIrqDisable  __disable_irq
#define zyIrqEnable   __enable_irq

#define cpu_irq_disable zyIrqDisable 
#define cpu_irq_enable zyIrqEnable  

 
#define taskENTER_CRITICAL  cpu_irq_disable
#define taskEXIT_CRITICAL  cpu_irq_enable


extern PASSTHRU_CONFIG PassThruConfig;
extern PASSTHRU_DRIVER PassThruDriver;
extern void UARTAnswerErr(uint8 cmd,uint8 para);
#define NULL 0
#define GET_RX0_STATUS 0
 
extern uint8_t EcuComRxBuf[];
extern  hal_detect_baud_minivci(CHANNEL_PARAM_ITEM *d_pdu);
extern void printferror(const char *fmt,...);

extern void hal_kmp_uart_init_irq_set(uint8_t f_rx_irq);
 
#define  NVIC_DisableIRQ  0 //quwenqi
//extern void printf_usb2(const char *fmt,...);
#define printf_usb2 
uint8_t hal_kwp_read_byte(uint16_t msDelayCounter,uint8_t *data);
uint32 k2kBaud_backup;
uint8 k2kTD1_backup;
uint8 k2kTD2_backup;
uint8 k2kTD3_backup;
uint8 k2kTD4_backup;


//uart参数结构体
typedef struct
{
	uint32_t baud;		//波特率
	uint8_t  len;		//数据长度
	uint8_t  parity;	//奇偶校验
	uint8_t  stop;		//停止位
}UART_PARAMS_STRUCT;

UART_PARAMS_STRUCT kwp_line0_param;
UART_PARAMS_STRUCT kwp_line1_param;
CHANNEL_PARAM_ITEM Channel_0_Param;
CHANNEL_PARAM_ITEM Channel_1_Param;


extern void hal_kmp_soft_uart_init_minivci(void);
extern void hal_kwp_soft_5bps_send_byte_minivci(uint8_t data,uint8_t parity);
extern uint8_t hal_kwp_read_byte(uint16_t msDelayCounter,uint8_t *data);
extern void hal_kwp_send_byte(uint8_t data);
extern void printfkwp(const char *fmt,...);

uint8_t hal_kwp0_wait_io_level_change(uint16_t msDelayCounter,uint8_t level,uint16_t *TC_Counter)
{
	//printfkwp(" msDelayCounter==%d \r\n",msDelayCounter);
	T_delay_ms_set(msDelayCounter);
	
	while( T_delay_ms_get_status() == RESET)  
	{ 
		if (GET_KWP_LINE0_STATUS == level) 
		{
			*TC_Counter = T_delay_ms_get_count();  
			return 1;
		}
	}
	return 0;
}


uint8_t hal_kwp1_wait_io_level_change(uint16_t msDelayCounter,uint8_t level,uint16_t *TC_Counter)
{
	T_delay_ms_set(msDelayCounter);
	
	while( T_delay_ms_get_status() == 0)  
	{ 
		if (GET_KWP_LINE1_STATUS == level)  
		{
			*TC_Counter = T_delay_ms_get_count();  
			return 1;
		}
	}
	return 0;
}


 uint32_t hal_detect_baud_kwp0_2000_minivci(CHANNEL_PARAM_ITEM *d_pdu)
{
	uint32_t temp=0;
	uint32_t baud=0;
	uint8_t  i;
	uint16_t delaytime = 1;
	uint16_t TC_Counter;
	uint16_t CP_W1 = d_pdu->CP_W1Max;        //CP参数W1
	 
	if(CP_W1 != 0)
	{
		delaytime = CP_W1;
	}
	delaytime = 500;
	
	 
	//等待起始位
	if(hal_kwp0_wait_io_level_change(delaytime,0,&TC_Counter)==0) //规定时间内没收到低电平开始数据，则直接退出
	{
		printfkwp(" hal_kwp0_wait_io_level_change error \r\n",ComCtl.TxBaudCnter);
		return 0;
	}
	taskENTER_CRITICAL(); 
	//75MHz 超时时间  873us
	hal_tc2_ch0_Init();
	while((GET_KWP_LINE0_STATUS)==LOGIC_0)
	{
		if( GET_TIMER2_CH0_COVFS == 1)  //定时到，错误退出
		{
			printfkwp(" GET_TIMER2_CH0_COVFS 11\r\n",ComCtl.TxBaudCnter);
			taskEXIT_CRITICAL(); 
			return 0;
		}
	}
	RESET_TIMER2_CH0_CV;
	//temp=TIM0_TC;
	i=0;
	for(i=0; i<4; i++)
	{
		while((GET_KWP_LINE0_STATUS)==LOGIC_1)
		{
			if( GET_TIMER2_CH0_COVFS == 1)	//定时到，错误退出
			{
				taskEXIT_CRITICAL(); 
				return 0;
			}
		}
		temp+=GET_TIMER2_CH0_CV;
		RESET_TIMER2_CH0_CV;
		while((GET_KWP_LINE0_STATUS)==LOGIC_0)
		{
			if(GET_TIMER2_CH0_COVFS == 1) 
			{
				taskEXIT_CRITICAL(); 
				//UARTAnswerErr(0xfc, 0xfc);
				return 0;
			}
		}
		temp+=GET_TIMER2_CH0_CV;
		RESET_TIMER2_CH0_CV;
	}

	temp=temp>>3; 				 
	baud = 1000000 / temp;

	T_delay_us_Set(temp);
		
	taskEXIT_CRITICAL(); 
	return baud;
}
 uint32_t hal_detect_baud_kwp1_2000_minivci(CHANNEL_PARAM_ITEM *d_pdu)
 {
	 uint32_t temp=0;
	 uint32_t baud=0;
	 uint8_t  i;
	 uint16_t delaytime = 1;
	 uint16_t TC_Counter;
	 uint16_t CP_W1 = d_pdu->CP_W1Max;  
	 
	 if(CP_W1 != 0)
	 {
		 delaytime = CP_W1;
	 }
	 delaytime = 500;
	 
	 
	 //等待起始位
	 if(hal_kwp1_wait_io_level_change(delaytime,0,&TC_Counter)==0) //规定时间内没收到低电平开始数据，则直接退出
	 {
		 printfkwp(" hal_kwp1_wait_io_level_change error \r\n",ComCtl.TxBaudCnter);
		 return 0;
	 }
	 taskENTER_CRITICAL(); 
	 //75MHz 超时时间  873us
	 hal_tc2_ch0_Init();
	 while((GET_KWP_LINE1_STATUS)==LOGIC_0)
	 {
		 if( GET_TIMER2_CH0_COVFS == 1)  //定时到，错误退出
		 {
			 taskEXIT_CRITICAL(); 
			 printfkwp(" GET_TIMER2_CH0_COVFS 11\r\n",ComCtl.TxBaudCnter);
			 return 0;
		 }
	 }
	 RESET_TIMER2_CH0_CV;
	 //temp=TIM0_TC;
	 i=0;
	 for(i=0; i<4; i++)
	 {
		 while((GET_KWP_LINE1_STATUS)==LOGIC_1)
		 {
			 if( GET_TIMER2_CH0_COVFS == 1) 
			 {
				 taskEXIT_CRITICAL(); 
				 return 0;
			 }
		 }
		 temp+=GET_TIMER2_CH0_CV;
		 RESET_TIMER2_CH0_CV;
		 while((GET_KWP_LINE1_STATUS)==LOGIC_0)
		 {
			 if(GET_TIMER2_CH0_COVFS == 1) 
			 {
				 taskEXIT_CRITICAL(); 
				 //UARTAnswerErr(0xfc, 0xfc);
				 return 0;
			 }
		 }
		 temp+=GET_TIMER2_CH0_CV;
		 RESET_TIMER2_CH0_CV;
	 }

	 temp=temp>>3; 						
	 baud = 1000000 / temp;
	taskEXIT_CRITICAL();  
	 return baud;
 }


 


uint8_t hal_kwp_wait_io_level_change(uint16_t msDelayCounter,uint8_t level,uint16_t *TC_Counter)
{
	
}


 
//#define  M_SoftFilterUsart  1
#define  M_KWPTIMECALC  1   //【1 byte的补偿时间】 vci是用模拟串口来做的，当判断串行数据的START信号就认为接收数据成功，pdu中的vci 部分是用硬件串口来做的，收到串行数据的END信号才认为接收数据成功。

 void hal_kmp0_txd_init(uint32_t baud,uint8_t len,uint8_t parity,uint8_t stop)
{
	kwp_line0_param.baud = baud;
	kwp_line0_param.len = len;
	kwp_line0_param.parity = parity;
	kwp_line0_param.stop = stop;
}
void hal_kmp1_txd_init(uint32_t baud,uint8_t len,uint8_t parity,uint8_t stop)
{
	kwp_line1_param.baud = baud;
	kwp_line1_param.len = len;
	kwp_line1_param.parity = parity;
	kwp_line1_param.stop = stop;
}


#ifdef MINI_HW_002
void  hal_kmp_uart_config_minivci(uint32_t baudrate,uint32_t parity)
{
	if(( (PassThruConfig.Pin1 & 0x07)==7)||( (PassThruConfig.Pin2 & 0x0f)==0x07))
	{
		hal_kmp0_txd_init(baudrate,8,0,1);
	}
	else
	{
		hal_kmp1_txd_init(baudrate,8,0,1);
	}
	OBD_EVAL_COMInit(baudrate,parity);
	
    HAL_MINIVCI_UART_Clear();
}
#endif


//������żУ��λ
//����ֵ0:�͵�ƽ 1:�ߵ�ƽ 2��û�д�λ
uint8_t hal_calc_parity_mul(uint16_t Data,const uint8_t data_len,const uint8_t parity)
{
	uint8_t ret = 2;
	uint8_t num = 0;
	uint8_t i;
	
	if (parity == KWP_UART_PAR_NO)
	{
		return ret;
	}
	
	for (i=0;i<data_len;i++)
	{
		if (Data&0x01 == 0x01)  //����bit�Ƿ�Ϊ1
		{
			num++;
		}
		Data = Data >> 1;
	}
	
	if (parity == KWP_UART_PAR_ODD)
	{
		if (num%2 == 0)            //żУ�飺1�ĸ���λż�� У��λ��1
		{
			ret = 1;
		}
		else
		{
			ret = 0;
		}
	}
	else if(parity == KWP_UART_PAR_EVEN)
	{
		if (num%2 == 1)			   //��У�飺1�ĸ���λ���� У��λ��1
		{
			ret = 1;
		}
		else
		{
			ret = 0;
		}
	}
	else
	{
		ret = 2;
	}
	
	return ret;
}


 void hal_soft_kwp0_send_byte(UART_PARAMS_STRUCT UartParam,uint16_t data)
{
	uint8_t i;
	uint8_t parity_bit;
	//calc baud
	const uint32_t bit_time = 1000000 / UartParam.baud;
	//check parity
	if (UartParam.parity != KWP_UART_PAR_NO)
	{
		parity_bit = hal_calc_parity_mul(data,UartParam.len,UartParam.parity);
	}
	else
	{
		parity_bit = 2;
	}
	T_delay_us_Set(bit_time);	
	//start  bit
	KWP_LINE0_SET_0;			
 	 	
 	
	while(T_delay_us_get_status()==RESET);
	
	//data bit
	for (i=0;i<UartParam.len;i++)   
	{
		if(data&0x01)         //send lsb
		{
			KWP_LINE0_SET_1; 
		}
		else
		{
			KWP_LINE0_SET_0;
		}
		data = data >> 1;
		
		while(T_delay_us_get_status()==RESET);
	}
	//parity bit
	if (parity_bit == 0)
	{
		KWP_LINE0_SET_0;
		
		while(T_delay_us_get_status()==RESET);
		
	}
	else if (parity_bit == 1)
	{
		KWP_LINE0_SET_1;
		while(T_delay_us_get_status()==RESET);
	}
	//stop bit
	KWP_LINE0_SET_1;	
	
	while(T_delay_us_get_status()==RESET);
	
	if (UartParam.stop == 2)
		
	while(T_delay_us_get_status()==RESET);
		
	KWP_LINE0_SET_NULL;	
}
 void hal_soft_kwp1_send_byte(UART_PARAMS_STRUCT UartParam,uint16_t data)
{
	uint8_t i;
	uint8_t parity_bit;
	//calc baud
	const uint32_t bit_time = 1000000000 / UartParam.baud;
	//check parity
	if (UartParam.parity != KWP_UART_PAR_NO)
	{
		parity_bit = hal_calc_parity_mul(data,UartParam.len,UartParam.parity);
	}
	else
	{
		parity_bit = 2;
	}
	//start  bit
	KWP_LINE1_SET_0;
	hal_HW_Delay_ns(bit_time);
	//data bit
	for (i=0;i<UartParam.len;i++)
	{
		if(data&0x01)         //send lsb
		{
			KWP_LINE1_SET_1;
		}
		else
		{
			KWP_LINE1_SET_0;
		}
		data = data >> 1;
		hal_HW_Delay_ns(bit_time);
	}
	//parity bit
	if (parity_bit == 0)
	{
		KWP_LINE1_SET_0;
		hal_HW_Delay_ns(bit_time);
	}
	else if (parity_bit == 1)
	{
		KWP_LINE1_SET_1;
		hal_HW_Delay_ns(bit_time);
	}
	//stop bit
	KWP_LINE1_SET_1;
	hal_HW_Delay_ns(bit_time);
	if (UartParam.stop == 2)
	hal_HW_Delay_ns(bit_time);
	KWP_LINE1_SET_NULL;
}


//����500us�Ŀ��б���Ϊ��Ч���� ����ʱ500ms���˳�����
uint8_t HAL_KWP0_FrameSend(uint8_t *TXdata,uint16_t data_len)
{
	uint16_t i,k;
	uint8_t  temp = 0; 
	//��������Ƿ����
	//��������Ƿ�æ  ����500us�Ŀ��б���Ϊ��Ч����
	//��ʱ500ms���˳�����
	for(i=0; i<1000; i++)
	{
		temp = 0;
		for(k = 0; k < 5; k++)
		{
			if(GET_KWP_LINE0_STATUS == 1) 
			{
				temp++;
			}
			T_delay_us(100);
		}
		if(temp == 5)
		{
			break;
		}	
	}
	if(i>1000) return false;
		
	 
	 
	//send data
	for (i=0;i<data_len-1;i++)
	{
		
		hal_soft_kwp0_send_byte(kwp_line0_param,*TXdata++);
		
	}
	hal_soft_kwp0_send_byte(kwp_line0_param,*TXdata++);
	
	return true;	
}


//����500us�Ŀ��б���Ϊ��Ч���� ����ʱ500ms���˳�����
uint8_t HAL_KWP1_FrameSend(uint8_t *TXdata,uint16_t data_len)
{
	uint16_t i,k;
	uint8_t  temp = 0;
	//��������Ƿ����
	//��������Ƿ�æ  ����500us�Ŀ��б���Ϊ��Ч����
	//��ʱ500ms���˳�����
	for(i=0; i<1000; i++)
	{
		temp = 0;
		for(k = 0; k < 5; k++)
		{
			if(GET_KWP_LINE1_STATUS == 1)
			{
				temp++;
			}
			T_delay_us(100);
		}
		if(temp == 5)
		{
			break;
		}
	}
	if(i>1000) return false;
	
	//init timer
	hal_HW_Delay_ns_Init();
	//send data
	for (i=0;i<data_len-1;i++)
	{
		 
		hal_soft_kwp1_send_byte(kwp_line1_param,*TXdata++);
		//hal_HW_Delay_ms((Channel_1_Param.CP_P4Min / 1000));   //��ʱ��Ӳ����ʱ��
	}
	hal_soft_kwp1_send_byte(kwp_line1_param,*TXdata++);
	
	return true;
}

void hal_kwp_send_byte_kwp(uint8_t data)
{

	uint8_t send_data;
   // printfkwp("PassThruConfig.Pin1==%02x\r\n",PassThruConfig.Pin1);
	// printfkwp("PassThruConfig.Pin2==%02x\r\n",PassThruConfig.Pin2);
	cpu_irq_disable();
	 if(( (PassThruConfig.Pin1 & 0x07)==7)||( (PassThruConfig.Pin2 & 0x0f)==7))
	{
	//	printfkwp("HAL_KWP0_FrameSend\r\n");
		HAL_KWP0_FrameSend(&data,1);
	}
	else if(( (PassThruConfig.Pin1 & 0x07)==8)||( (PassThruConfig.Pin2 & 0x0f)==8)) 
	{
	HAL_KWP0_FrameSend(&data,1);
	}

	else if(( (PassThruConfig.Pin1 & 0x07)==9)||( (PassThruConfig.Pin2 & 0x0f)==0x07)) 
	{
	HAL_KWP0_FrameSend(&data,1);
	}

	else if(( (PassThruConfig.Pin1 & 0x07)==10)||( (PassThruConfig.Pin2 & 0x0f)==10)) 
	{
	HAL_KWP0_FrameSend(&data,1);
	}

	 else if(( (PassThruConfig.Pin1 & 0x07)==11)||( (PassThruConfig.Pin2 & 0x0f)==11)) 
	{
	HAL_KWP0_FrameSend(&data,1);
	}

	 else if(( (PassThruConfig.Pin1 & 0x07)==12)||( (PassThruConfig.Pin2 & 0x0f)==12)) 
	{
	HAL_KWP0_FrameSend(&data,1);
	}
	 else if(( (PassThruConfig.Pin1 & 0x07)==13)||( (PassThruConfig.Pin2 & 0x0f)==13)) 
	{
	HAL_KWP0_FrameSend(&data,1);
	}

	 else if(( (PassThruConfig.Pin1 & 0x07)==15)||( (PassThruConfig.Pin2 & 0x0f)==15)) 
	{
	HAL_KWP0_FrameSend(&data,1);
	}
		 
	else
	{
	//	printfkwp("HAL_KWP1FrameSend\r\n");
		HAL_KWP1_FrameSend(&data,1);
	}
    	cpu_irq_enable();	
       HAL_MINIVCI_UART_Clear();
	 
	
}
void hal_kwp_send_byte_kwp_softfilter(uint8_t data)
{
		//usart_disable_rx(KWP_RX_UART); quwenqi2g
		hal_kwp_send_byte_kwp(data);
		DelayNus(10);
		//NVIC_ClearPendingIRQ(KWP_RX_UART_IRQn); quwenqi2g
		//usart_enable_rx(KWP_RX_UART); quwenqi2g
}

#ifdef M_SoftFilterUsart
void hal_kwp_send_byte_kwp2(uint8_t data)
{
	hal_kmp_uart_init_irq_set(0);
	hal_kwp_send_byte_kwp(data);
	hal_kmp_uart_init_irq_set(1);
}
#endif
#ifndef M_SoftFilterUsart
#define  hal_kwp_send_byte_kwp2 hal_kwp_send_byte_kwp
#endif
void hal_kmp_uart_init_irq_minivci(void) 
{

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	 
	//STM_EVAL_COMInit(COM5,USART_InitStruct);
 	//STM_EVAL_COMInit(COM6,USART_InitStruct);

}

void hal_kmp_uart_init_irq_set(uint8_t f_rx_irq)
{
      
}
void HAL_MINIVCI_UART_Clear(void)
{
	if(gUSART1_Handler)
	{
		gUSART1_Handler=0;
		USART_Queue_Init(&KWP1_RX_QueueData);
		//printfkwp("HAL_MINIVCI_UART_Clear\r\n");
	}
		if(gUSART0_Handler)
	{
		gUSART0_Handler=0;
		USART_Queue_Init(&KWP0_RX_QueueData);
		//printfkwp("HAL_MINIVCI_UART_Clear\r\n");
	}
}
uint8_t HAL_MINIVCI_UART_READ_us(uint16_t ussDelayCounter,uint8_t *data)
{
	
	 
	uint32_t RawMessage;
	//printferror("ussDelayCounter=%d\r\n",ussDelayCounter);
	#ifdef M_KWPTIMECALC
	if(ussDelayCounter==0)
	{
		ussDelayCounter += (11000000 / PassThruConfig.BaudRate);//11bit
	}
	else
	{
		ussDelayCounter += (10000000 / PassThruConfig.BaudRate);//10bit
	}
	
	#endif
	if(ussDelayCounter==0)
	{
		cpu_irq_disable();
		if(USART_Queue_Read(&KWP0_RX_QueueData,&RawMessage) == true)
		{
			*data = (uint8_t)(RawMessage&0X1FF);
			cpu_irq_enable();
			if(gUSART0_Handler)gUSART0_Handler--;
			return BYTE_OK;
		}
		else if(USART_Queue_Read(&KWP1_RX_QueueData,&RawMessage) == true)
		{
			*data = (uint8_t)(RawMessage&0X1FF);
			cpu_irq_enable();
			if(gUSART1_Handler)gUSART1_Handler--;
			return BYTE_OK;
		}
		else
		cpu_irq_enable();
		
		*data = 0;
		return EOF_FLAG;
	}
	
		
	if (ussDelayCounter > 3000 || ussDelayCounter < 7)  //min 7us  max 3000us
	return EOF_FLAG;
	ussDelayCounter = ussDelayCounter - 6;  //指令耗时补偿
	
	
 	T_delay_us_Set( ussDelayCounter);
	
	while( T_delay_us_get_status() == RESET)  ////等待超时或接收到KWP数据
	{
		cpu_irq_disable();
		if(USART_Queue_Read(&KWP0_RX_QueueData,&RawMessage) == true)
		{
			*data = (uint8_t)(RawMessage&0X1FF);
			cpu_irq_enable();
			if(gUSART0_Handler)gUSART0_Handler--;
			return BYTE_OK;
		}
		else if (USART_Queue_Read(&KWP1_RX_QueueData,&RawMessage) == true)
		{
			if(gUSART1_Handler)gUSART1_Handler--;
			*data = (uint8_t)(RawMessage&0X1FF);
			cpu_irq_enable();
			return BYTE_OK;
		}
		
		else
		cpu_irq_enable();
		
	}
	*data = 0;
	return EOF_FLAG;
	 
}

uint8_t HAL_MINIVCI_UART_READ(uint16_t msDelayCounter,uint8_t *data)
{
	uint32_t RawMessage ;
	uint32_t ussDelayCounter;
	#if 1  //quwenqi2g
	if((msDelayCounter&0x80)&&( (PassThruConfig.Protocol >> 16)==0x05))
	{
		ussDelayCounter=(msDelayCounter&0x7f)*100;
		return HAL_MINIVCI_UART_READ_us(ussDelayCounter,data);
	}

	 
	#ifdef M_KWPTIMECALC
	
	if( (1000000 % PassThruConfig.BaudRate) == 00)
	{
		msDelayCounter += (1000000 / PassThruConfig.BaudRate) / 100;
		if(msDelayCounter==0) 
		return HAL_MINIVCI_UART_READ_us(msDelayCounter,data);
	}
	else
	{
		msDelayCounter += (1000000 / PassThruConfig.BaudRate) / 100;
		msDelayCounter += 1;
	}
	#endif
	if(msDelayCounter==0)
	{
		cpu_irq_disable();
		if(USART_Queue_Read(&KWP0_RX_QueueData,&RawMessage) == true)
		{
			*data = (uint8_t)(RawMessage&0X1FF);
			cpu_irq_enable();
			if(gUSART0_Handler)gUSART0_Handler--;
			return BYTE_OK;
		}
		else if(USART_Queue_Read(&KWP1_RX_QueueData,&RawMessage) == true)
		{
			*data = (uint8_t)(RawMessage&0X1FF);
			cpu_irq_enable();
			if(gUSART1_Handler)gUSART1_Handler--;
			return BYTE_OK;
		}
		else
		cpu_irq_enable();
		
		*data = 0;
		return EOF_FLAG;
	}
	
	T_delay_ms_set(msDelayCounter);

	while( T_delay_ms_get_status() == RESET)  ////等待超时或接收到KWP数据
	{
		cpu_irq_disable();
		if(USART_Queue_Read(&KWP0_RX_QueueData,&RawMessage) == true)
		{
			*data = (uint8_t)(RawMessage&0X1FF);
			cpu_irq_enable();
			if(gUSART0_Handler)gUSART0_Handler--;
			return BYTE_OK;
		}
		else if(USART_Queue_Read(&KWP1_RX_QueueData,&RawMessage) == true)
		{
			*data = (uint8_t)(RawMessage&0X1FF);
			cpu_irq_enable();
			if(gUSART1_Handler)gUSART1_Handler--;
			return BYTE_OK;
		}
		else
		cpu_irq_enable();
		
	}
	*data = 0;
	return EOF_FLAG;
	#endif
}

void PassThru_KWP_delay_parse(uint8 *PackBuf)
{
    uint8 temp0, temp1, temp2;
    uint32 temp32;

    kwp_5bpsdata_delay_sent_flag = 1;

    // k2kBaud TD1 TD2 TD3 TD4
    k2kBaud_backup = PassThruConfig.BaudRate;

    k2kTD1_backup = PassThruConfig.P1Max;
    k2kTD2_backup = PassThruConfig.P2Max;
    k2kTD3_backup = PassThruConfig.P3Max;
    k2kTD4_backup = PassThruConfig.P4Max;

    PackBuf = PackBuf + 3;

    temp0 = *PackBuf++;
    temp1 = *PackBuf++;
    temp2 = *PackBuf++;
    temp32 = (temp0 << 16) + (temp1 << 8) + temp2;

    PassThruConfig.BaudRate = temp32;
    SetSoftUart(PassThruConfig.BaudRate);

    PassThruConfig.P1Max =  *PackBuf++; //+1
    PassThruConfig.P2Min =  *PackBuf++; //+1
    PassThruConfig.P3Min =  *PackBuf++; //+1
    PassThruConfig.P4Min =  *PackBuf++; //+1
}

void PassThru_KWP_delay_sent(void)
{
    uint32 DelayCounter;
    uint8 temp;
    uint8 ret;

    DelayCounter =  PassThruConfig.P2Max ;
    //-------------------------------------------------------------------------
    counter_1ms = 0;

    while(1)
    {
        ret = PassThruDriver.RxByteDrv(DelayCounter, &temp);
        if(ret == EOF_FLAG)
        {
            break;
        }
        if(counter_1ms >= PassThruConfig.P2Max + (uint8)(PassThruConfig.Protocol))
        {
            break;
        }
    }
    PassThruConfig.W5 = 0;//已经根据协议要求等待100ms左右，发送5bps命令无需再次等待300ms
    PassThruConfig.BaudRate = k2kBaud_backup;
    SetSoftUart(PassThruConfig.BaudRate);

    PassThruConfig.P1Max = k2kTD1_backup;
    PassThruConfig.P2Min = k2kTD2_backup;
    PassThruConfig.P3Min = k2kTD3_backup;
    PassThruConfig.P4Min = k2kTD4_backup;
}
//??nissan pwm?????14????can?,????14?????1.8~3v?????,????1,???0
uint8 test_nisan_pwm(void)
{
		uint16 i;
        
		SetLine_ext(0,1);
		//i=Read_Dac(mux1_vcc)/100; //quwenqi
		SetLine_ext(0,1);
		if(18< i <30)//有两v左右电压，说明是can线
			return 1;
		else
			return 0;
}
extern uint8_t hal_kwp_read_byte(uint16_t msDelayCounter,uint8_t *data);
extern void hal_kwp_send_byte(uint8_t data);
/*********************************************************************************************
* 名称    ： PassThru_KWP_InitPort
* 功能    ： KWP协议初始化IO
* 入口参数：
*
*
* 出口参数：
* 说明    ：
*
***********************************************************************************************/
#define DISABLE_J1850_RX0_IRQ EXTILine2_IRQDisable()
#define DISABLE_J1850_RX1_IRQ EXTILine7_IRQDisable()
extern USART_TypeDef* COM_USART[];
extern PDU_COMMON_DATA_STRUCT MinivciCan;
uint8 PassThru_KWP_InitPort(uint8 CHANEL)
{
    //uint8 data;
    K2K_Simulate_firstRX_flag = 0;	//清零
	#if 1  //quwenqi2g
	DISABLE_J1850_RX0_IRQ;
	DISABLE_J1850_RX1_IRQ;
    
	USART_ITConfig(COM_USART[COM5], USART_IT_RXNE, DISABLE);
	USART_ITConfig(COM_USART[COM6], USART_IT_RXNE, DISABLE);
    
	GPIO_SetBits   ( PD13_LINE0_PU_PORT,PD13_LINE0_PU_PIN );
	GPIO_ResetBits      (PD12_LINE0_PD_PORT, PD12_LINE0_PD_PIN );

	GPIO_SetBits   (PD15_LINE1_PU_PORT, PD15_LINE1_PU_PIN );
	GPIO_ResetBits   ( PD14_LINE1_PD_PORT, PD14_LINE1_PD_PIN );

    
	#endif
	//hal_mcan_DisableRxInterrupt(MinivciCan);
	tx_CTRL_EN_channel2_AllOff();
    //DelayNms(10);
	//NVIC_DisableIRQ(CAN_IRQn);       //禁止CAN 中断	  在使用过CAN 总线后，使用其他信号线时关闭?
    
    PassThruDriver.InitEcuFunc     = PassThru_KWP_Init_Fast;
    PassThruDriver.TxDataToEcuFunc = PassThru_KWP_TxPack_Normal;
    PassThruDriver.TxDataDelayFunc = PassThru_KWP_TxPack_Delay;
    PassThruDriver.ReadBusFunc     = PassThru_KWP_RxPack_Normal;
    PassThruDriver.KeepComFunc     = NULL;
    PassThruDriver.FuncBak0        = NULL;

   PassThruDriver.RxByteDrv       = HAL_MINIVCI_UART_READ ;
   
   PassThruDriver.TxByteDrv       = hal_kwp_send_byte_kwp;
	//arch_ioport_set_pin_dir(MCU_TXD_FPGA_PD19,IOPORT_DIR_OUTPUT);
	//arch_ioport_set_pin_level(MCU_TXD_FPGA_PD19,1);
    //硬件接收
    switch(PassThruConfig.Format & 0x04)
    {
        case 0x04:
            ComCtl.RxHardWare = USE_HARD; //用硬件接收
            break;
        default:
            ComCtl.RxHardWare = USE_IO; //用软件接收
            break;
    }

    //======================01 设置伯特率     =======================
    if(PassThruConfig.BaudRate == 0)//尝试用硬件波特率自侦断,changyl,2015/8/6
    {
        LineInitFlag = START_DETECT;
    }
    else
    {
        LineInitFlag = DETECT_OK;
        SetSoftUart(PassThruConfig.BaudRate);
		
		hal_kmp_soft_uart_init_minivci();  //soft uart init
		//閸掑洦宕查崚鎵€栨禒绂狝RT
		hal_kmp_uart_init_irq_minivci();
		hal_kmp_uart_init_irq_set(1);
		//闁板秶鐤嗘稉绡岰U瑜版挸澧犲▔銏㈠閻?

		hal_kmp_uart_config_minivci(PassThruConfig.BaudRate,UART_MR_PAR_NONE);

        UartCtlReg.Baud = PassThruConfig.BaudRate;
    }
    
    //===================03 设置奇偶校验 =============================
    ComCtl.ParityEnFlag = NORMAL_RX;

	 
    switch(PassThruConfig.Format & 0x03)
    {
		case 0x02:
			printfkwp("ODD\r\n");
			hal_kmp_uart_config_minivci(PassThruConfig.BaudRate,UART_MR_PAR_EVEN);
			ComCtl.SoftParity = ODD; //奇校验
			UartCtlReg.parity = ODD;
			PassThruConfig.Parity = ODD;
			break;
		case 0x03:
			printfkwp("EVEN\r\n");
			hal_kmp_uart_config_minivci(PassThruConfig.BaudRate, UART_MR_PAR_ODD);
			ComCtl.SoftParity = EVEN; //奇校验
			UartCtlReg.parity = EVEN;
			PassThruConfig.Parity = EVEN;
			break;
		default:
			printfkwp("NO_PARITY\r\n");
			ComCtl.SoftParity = NO_PARITY;
			UartCtlReg.parity = NO_PARITY;
			PassThruConfig.Parity = NO_PARITY;
			ComCtl.ParityEnFlag =DISEN_PARITY;
			break;
    }
    
    //===================03 设置快速初始化 =============================
    //ComCtl.ParityEnFlag = NORMAL_RX;
	printfkwp("PassThruConfig.Format & 0xc0=%02x\r\n",PassThruConfig.Format & 0xc0);
    switch(PassThruConfig.Format & 0xc0)
    {
		case 0x40:
			//不回复kb2取反
			PassThruConfig.FiveBaudMod = FIVE_BAUD_MOD_NORMAL;
			PassThruDriver.InitEcuFunc = PassThru_KWP_Init_Slow;
			printfkwp("PassThru_KWP_Init_Slow\r\n");
			break;
		case 0x80:
			//回复kb2取反 接收后续数据
			PassThruConfig.FiveBaudMod = FIVE_BAUD_MOD_KEYBYTE|FIVE_BAUD_MOD_ADDRESS;
			PassThruDriver.InitEcuFunc = PassThru_KWP_Init_Slow;
			printfkwp("PassThru_KWP_Init_Slow\r\n");
			break;
		case 0xc0:
			PassThruConfig.FiveBaudMod = FIVE_BAUD_MOD_NORMAL;
			PassThruDriver.InitEcuFunc = PassThru_KWP_Init_Fast;
			printfkwp("PassThru_KWP_Init_Fast\r\n");
			break;
		default:
			PassThruConfig.FiveBaudMod = FIVE_BAUD_MOD_NORMAL;
			PassThruDriver.InitEcuFunc = PassThru_KWP_Init_Fast;
			printfkwp("PassThru_KWP_Init_Fast\r\n");
			break;
    }

    if(ComCtl.RxHardWare == USE_HARD) //用硬件接收
    {
        //XK 20130510 为了使TX00 为TXD1状态
        //IO0CLR = (1<<5);
        UartCtlReg.datab = uart_data8;
        UartCtlReg.stopb = uart_stopbit_1;
        UartCtlReg.Baud=PassThruConfig.BaudRate;
        //UART2_Init(UartCtlReg);//串口2和3 一起初始化
        // quwenqi MCU_UART_Init(UartCtlReg);// UART3_Init(UartCtlReg);
    }

    //=====================0a Eclk驱动  ==============================
    if(PassThruConfig.Eclk&&(test_nisan_pwm()==0))
    {
         SetCChannel(CLK_OUT_ENABLE);
         SetEclk(PassThruConfig.Eclk);
    }
    else
    {
       //quwenqi   SetCChannel(CLK_OUT_DISENABLE);
       //quwenqi   SetEclk(0);
    }
    //SetEclk(PassThruConfig.Eclk);

    //===================0c 设置特定协议驱动程序 ======================
	printfkwp("PassThruConfig.Protocol>>16=%d\r\n",PassThruConfig.Protocol >> 16);
	
	printfkwp("PassThruConfig.Protocol>>16=%08x\r\n",PassThruConfig.Protocol  );
	
    switch((uint8)(PassThruConfig.Protocol >> 16))
    {
    case PROTOCOL_FRAME_NORMAL:
        PassThruDriver.ReadBusFunc = PassThru_KWP_RxPack_Normal;
        break;
    case PROTOCOL_FRAME_KWP2000:
        PassThruDriver.ReadBusFunc = PassThru_KWP_RxPack_Kwp2000;
        break;
    case PROTOCOL_FRAME_NISSAN:
        PassThruDriver.ReadBusFunc = PassThru_KWP_RxPack_Nissan;
        break;
    case PROTOCOL_FRAME_MIESUB:
        PassThruDriver.ReadBusFunc = PassThru_KWP_RxPack_Mitsub;
        break;
    case PROTOCOL_FRAME_LENGTH:
	     //printferror("PROTOCOL_FRAME_LENGTH");
        PassThruDriver.ReadBusFunc = PassThru_KWP_RxPack_Length;
        break;
    case PROTOCOL_FRAME_VOLVO:
        PassThruDriver.ReadBusFunc = PassThru_KWP_RxPack_Volvo;
        PassThruDriver.TxDataDelayFunc = PassThru_KWP_TxPack_Volvo;
        break;
    case PROTOCOL_FRAME_CATER://以下类型咨询小毛后未使用，故暂时删除
        PassThruDriver.ReadBusFunc = PassThru_KWP_RxPack_Cater;
        PassThruDriver.TxDataDelayFunc = PassThru_KWP_TxPack_Delay;
        break;
    case PROTOCOL_FRAME_LENGTH2://五十铃srs系统5bps初始化之后ECU持续回复数据 8ms字节间隔 10ms帧间隔
        PassThruDriver.ReadBusFunc = PassThru_KWP_RxPack_Length;
    case 0x84:
        PassThruDriver.ReadBusFunc = PassThru_KWP_RxPack_Length;
        break;
    case PROTOCOL_FRAME_J1708MID:
        PassThruDriver.ReadBusFunc = PassThru_KWP_RxPack_J1708MID;
        PassThruDriver.TxDataDelayFunc = PassThru_KWP_TxPack_Delay;
        break;
    case PROTOCOL_FRAME_MAZDAKWP:
        PassThruDriver.ReadBusFunc = PassThru_KWP_RxPack_MazdaKWP;
        break;
	case PROTOCOL_FRAME_LENGTH3:
		PassThruDriver.ReadBusFunc = PassThru_KWP_RxPack_Length3;
		break;
    default:
        PassThruDriver.ReadBusFunc = PassThru_KWP_RxPack_Normal;
        break;
    }
    //设置通讯保持周期
    KeepComTime[0] = PassThruConfig.Keep[0]; //我们以1ms为单位
    if(KeepComTime[0] == 0) KeepComTime[0] = 10;

    //针对benz设置扩展通道
    if(PassThruConfig.ExtPort != 0)
    {
		SetCompV(0x35);
		DelayNus(200);
		SetExtCh(PassThruConfig.ExtPort);
    }
    else
    {
        //设置总线驱动格式及通道
        Set_DrvPort(PassThruConfig.Pin1, PassThruConfig.Pin2, PassThruConfig.LogicV1, PassThruConfig.LogicV2,PassThruConfig.GndCom);
        //====================04 设置通道=================================

   }

    //===================== 02 设置各驱动程序 =======================这个要放在设置总线驱动之后，在只侦听模式下，不设置上下拉
    switch(PassThruConfig.Listen)
    {
    case 0x04:
        Stop_bit_5bps_flag = 1;
        break;
    case INTERCEPTION:
        PassThruDriver.ReadBusFunc = PassThru_KWP_RxPack_Interception;
        SetCChannel(NO_DRIVE);
        break;
    case DIRECTREAD:
        PassThruDriver.ReadBusFunc = PassThru_KWP_RxPack_DirectRead;
        SetCChannel(NO_DRIVE);
        break;
    default:
        break;
    }
	//XK 20130702 ComFmt bit4强制设为1
	PassThruConfig.Format = PassThruConfig.Format | 0x10;
	//=====================08 设置比较电压 ===========================
	SetCompV(PassThruConfig.CompV);
	DelayNms(10);
	HAL_MINIVCI_UART_Clear();
	//通讯保持后的延时
	DelayTimeForHoldCom = PassThruConfig.P3Min;
    return TRUE;
}

/*********************************************************************************************
* 名称    ： PassThru_KWP_SetConfig
* 功能    ： KWP协议解析初始化IO参数
* 入口参数：
*
*
* 出口参数：
* 说明    ：
*
***********************************************************************************************/
uint8 PassThru_KWP_SetConfig(uint8 *PackBuf,uint8 CHANEL)
{
    uint8 j;
    uint8 temp8, Len;
    uint8 temp0, temp1, temp2;
    uint32 temp32;
    uint16 PackLen;
    uint8 *PackEnd;

    PackLen = (PackBuf[0] << 8) + PackBuf[1];
    PackEnd = PackBuf + PackLen + 2;
    PackBuf = PackBuf + 3;
		
    //Reset_Line();				//所有io恢复初始设置

    for(;;)
    {
        temp8 = *PackBuf++;
        if(PackBuf > PackEnd || PackBuf == PackEnd)
        {
            //TD7必须先预置
            if( PassThruConfig.W1 == 0)
            {
                AnswerFlag = ANSWER_OFF;    //5BPS后等待55的时间1000ms v6.8 lzs
                PassThruConfig.W1 = 1000;
            }
            break;
        }
        switch(temp8)
        {
        case 0x01:
            temp0 = *PackBuf++;
            temp1 = *PackBuf++;
            temp2 = *PackBuf++;
            temp32 = (temp0 << 16) + (temp1 << 8) + temp2;
            PassThruConfig.BaudRate = temp32; //
            break;
        case 0x02:
            PassThruConfig.Listen = *PackBuf++; //+1
            break;
        case 0x03:
            PassThruConfig.Format = *PackBuf++; //+1
            break;
        case 0x04:
            PassThruConfig.Pin1 =  *PackBuf++; //+1
            PassThruConfig.Pin2 =  *PackBuf++; //+1
            PassThruConfig.LogicV1 =  *PackBuf++; //+1
            PassThruConfig.LogicV2 =  *PackBuf++; //+1
            break;
        case 0x05:
            PassThruConfig.P1Max =  *PackBuf++; //+1
            PassThruConfig.P2Min =  *PackBuf++; //+1
            PassThruConfig.P3Min =  *PackBuf++; //+1
            PassThruConfig.P4Min =  *PackBuf++; //+1
            temp0 = *PackBuf++;
            temp1 = *PackBuf++;
            PassThruConfig.Keep[0] = (temp0 << 8) + temp1;
            PassThruConfig.W4 = *PackBuf++; //+1
            break;
        case 0x06:
            PassThruConfig.KeepComMode= *PackBuf++; //+1
            break;
        case 0x07:
            Len = (*PackBuf++) << 1;        //+1
            PassThruConfig.PulseBuf[0] = Len;
            for(j = 0; j < Len; j++)
            {
                PassThruConfig.PulseBuf[j+1] = *PackBuf++; //
            }
            break;
        case 0x08:
            PassThruConfig.CompV = *PackBuf++;
            break;
        case 0x09:
            PassThruConfig.KeyByteNum = *PackBuf++; //+1
            break;
        case 0x0a:
            temp0 = *PackBuf++;
            temp1 = *PackBuf++;
            temp2 = *PackBuf++;
            PassThruConfig.Eclk = (temp0 << 16) + (temp1 << 8) + temp2; //+3
            break;
        case 0x0b:
            PassThruConfig.GndCom = *PackBuf++;
            break;
        case 0x0c:
            temp0 = *PackBuf++;
            temp1 = *PackBuf++;
            temp2 = *PackBuf++;
            PassThruConfig.Protocol = (temp0 << 16) + (temp1 << 8) + temp2; //+3
            if(temp0 == 0x06 || temp0 == 0x08)
            {
                PassThruConfig.Header = temp1;
                PassThruConfig.HeaderAR = temp2;
                PassThruConfig.HeaderID = PassThruConfig.Header | PassThruConfig.HeaderAR;
            }
            break;
        case 0x0d:
            temp0 = *PackBuf++;
            PassThruConfig.ExtPort = temp0;
            break;
        case 0x0e:
            temp0 = *PackBuf++;
            temp1 = *PackBuf++;
            PassThruConfig.W1 = (temp0 << 8) + (temp1);
            AnswerFlag = ANSWER_ON;
            break;
        case 0x0f:
            PassThruConfig.PulseCom = *PackBuf++; //v7.4 lzs
            break;
        case 0x10:
            temp0 = *PackBuf++;
            temp1 = *PackBuf++;
            temp2 = *PackBuf++;
            temp32 = (temp0 << 16) + (temp1 << 8) + temp2;
            PassThruConfig._BaudRate = temp32;
            PassThruConfig._WaitTime = *PackBuf++;
            PassThruConfig._ByteTime = *PackBuf++;
            for(j = 0; j < 10; j++)
            {
                PassThruConfig.SendBuf[j] = *PackBuf++;
            }
            break;
        default:
            return FALSE;
        }
    }
    return TRUE;
}

/*********************************************************************************************
* 名称    ： PassThru_KWP_Init_Fast
* 功能    ： KWP协议快速初始化ECU
* 入口参数：
*
*
* 出口参数：
* 说明    ：
*
***********************************************************************************************/
uint8 PassThru_KWP_Init_Fast(uint8 *TXdata)
{
	printfkwp("PassThruConfig.Protocol==%08x \r\n",PassThruConfig.Protocol);
	printfkwp("PassThruConfig.GndCom==%d \r\n",PassThruConfig.GndCom);
    //V6.91 针对三菱，对引脚接地指令的特别处理： 接地70ms，释放700ms，然后再接地70ms
	hal_kmp_uart_init_irq_set(0);
	hal_kmp_soft_uart_init_minivci();  //soft uart init
	// 鍒囨崲鍒扮‖浠禪ART
	hal_kmp_uart_init_irq_minivci();
	hal_kmp_uart_init_irq_set(0);
		
    if((PassThruConfig.Protocol == 0x030000) && (PassThruConfig.GndCom != 0x00))
    {
		SET_Another_STATUS(SET_OUT_STATUS_0);
		vTaskDelay(E70B_vTaskDelay_Modulus(150));
		SET_Another_STATUS(SET_OUT_STATUS_1);
		vTaskDelay(E70B_vTaskDelay_Modulus(700));
		SET_Another_STATUS(SET_OUT_STATUS_0);
		vTaskDelay(E70B_vTaskDelay_Modulus(700));
    }
    else
	{
		//拉高300ms 拉低25ms 拉高25ms
		// unsigned int t = _1msCounter;
		vTaskDelay(E70B_vTaskDelay_Modulus(300));
		/*if(ecumsCounter + PassThruConfig.Tidle > t )
		{
		DelayNms(ecumsCounter + PassThruConfig.Tidle - t);
		}*/
		//DelayNms(PassThruConfig.Tidle);//快速初始化开始之前总线的最小空闲时间 300ms
		//DelayNms(PassThruConfig.Tinil);//快速初始化开始之前低脉冲时间 25ms
		//DelayNms(PassThruConfig.TWUP); //快速初始化唤醒脉冲持续时间 50ms
            OBD_EVAL_GPIOInit();
		SoftPulse(PassThruConfig.PulseBuf);//产生脉冲 25ms低 25ms高
		HAL_MINIVCI_UART_Clear();
    }
	 
    SetSoftUart(PassThruConfig.BaudRate); //设置波特率
//    if(PassThruConfig.PulseCom == 0x01)
//    {
//        ComCtl.TxChannel = ONLY_CH0TX;
//    }
//    else if(PassThruConfig.PulseCom == 0x10)
//    {
//        ComCtl.TxChannel = ONLY_CH1TX;
//    }

	//hal_kmp_uart_init();
	//閰嶇疆涓篍CU褰撳墠娉㈢壒鐜?
	hal_kmp_uart_config_minivci(PassThruConfig.BaudRate,UART_MR_PAR_NONE);
	PassThru_KWP_TxPack_TxData(TXdata);

    return TRUE;
}

/*********************************************************************************************
* 名称    ： PassThru_KWP_Init_Slow
* 功能    ： KWP协议慢速初始化
* 入口参数：
*
*
* 出口参数：
* 说明    ：
*
***********************************************************************************************/

void PassThru_KWP_Parm_Init(CHANNEL_PARAM_ITEM *d_pdu,PASSTHRU_CONFIG *minvi)
{
	
	d_pdu->CP_TIdle=minvi->Tidle;
	d_pdu->CP_TInil=minvi->Tinil;
	d_pdu->CP_TWup=minvi->TWUP;

	d_pdu->CP_W1Max=minvi->W1 ;
	d_pdu->CP_W1Min=minvi->W1 ;
	
	d_pdu->CP_W2Max=minvi->W2;
	d_pdu->CP_W2Min=minvi->W2;
	
	d_pdu->CP_W3Max=minvi->W3;
	d_pdu->CP_W3Min=minvi->W3;
	
	d_pdu->CP_W4Min=minvi->W4;
	d_pdu->CP_W4Max=minvi->W4;

	
	d_pdu->CP_Baudrate=d_pdu->CP_Baudrate;
}
CHANNEL_PARAM_ITEM  Minivci_KWP_Param;

 
uint32_t hal_detect_baud_kwp200_minivci(CHANNEL_PARAM_ITEM *d_pdu)
{
	//printfkwp("hal_detect_baud_kwp200_minivci PassThruConfig.Pin1==%d\r\n",PassThruConfig.Pin1);
	//printfkwp("hal_detect_baud_kwp200_minivci PassThruConfig.Pin2==%d\r\n",PassThruConfig.Pin2);
	  if(( (PassThruConfig.Pin1 & 0x07)==7)||( (PassThruConfig.Pin2 & 0x0f)==0x07))
	  {
		//  	printfkwp("hal_detect_baud_kwp0_2000_minivci\r\n");
		 return  hal_detect_baud_kwp0_2000_minivci(d_pdu); 
	  }
	  else  if(( (PassThruConfig.Pin1 & 0x07)==15)||( (PassThruConfig.Pin2 & 0x0f)==15))
	  {
		  //  printfkwp("hal_detect_baud_kwp1_2000_minivci\r\n");
		 return  hal_detect_baud_kwp0_2000_minivci(d_pdu); 
	  }
	  else
	  {
		//  printfkwp("hal_detect_baud_kwp1_2000_minivci\r\n");
		return  hal_detect_baud_kwp1_2000_minivci(d_pdu); 
	  }
	  return 0;
}

uint8_t hal_kwp_protocol_init_slow_minivci(CHANNEL_PARAM_ITEM *d_pdu,PASSTHRU_CONFIG *minivci_config,PASSTHRU_DRIVER *minivci_driver,uint8_t *CP_5BaudAddress,uint8_t * RX_DATA,uint8_t *RX_DATA_POS)
{
	uint32_t ecu_cur_baud=0;
	uint8_t  recv_key[2];
	uint8_t  non_key2;
	uint8_t  recv_add;
	uint8_t  recv_data;
	uint8  loop=0;
	uint8  KeyByteNum=0;
	 
	HAL_MINIVCI_UART_Clear();
	hal_kmp_soft_uart_init_minivci();  //soft uart init
	  
	if((minivci_config->BaudRate == 0)||(minivci_config->BaudRate<200))
	{
		hal_kwp_soft_5bps_send_byte_minivci(*CP_5BaudAddress,UART_MR_PAR_NONE); //send add 5bps
	}
	else
	{
		ecu_cur_baud=minivci_config->BaudRate;
		
		hal_kmp_uart_init_irq_minivci();
		hal_kmp_uart_config_minivci(ecu_cur_baud,UART_MR_PAR_NONE);
		hal_kwp_send_byte_kwp2(*CP_5BaudAddress);
	}
	
	ecu_cur_baud = hal_detect_baud_kwp200_minivci(d_pdu);	//200ms wait detects baud
	
	
	if((ecu_cur_baud>(9600-20))&&(ecu_cur_baud<(9600+20)))
	{
		ecu_cur_baud = 9600;
	}
	else if((ecu_cur_baud>10400-20)&&(ecu_cur_baud<10400+20))
	{
		ecu_cur_baud = 10400;
	}
	//printfuartdebug("baud:%d\r\n",ecu_cur_baud);  //test code
	
	minivci_config->BaudRate=ecu_cur_baud;
	minivci_config->CurrBaud=minivci_config->BaudRate;
	if (ecu_cur_baud == 0)
	{
		ecu_cur_baud=9600;
		hal_kmp_uart_config_minivci(ecu_cur_baud,UART_MR_PAR_NONE);//【别克凯悦】没有【钥匙系统】会导致 安全气囊无法进入(串口死机)
		printfuartdebug("ecu_cur_baud error\r\n");
		return false;
	}
    //  ecu_cur_baud=9600;
	hal_kmp_uart_init_irq_minivci();
	hal_kmp_uart_init_irq_set(1);
	hal_kmp_uart_config_minivci(ecu_cur_baud,UART_MR_PAR_NONE);

	//KEY1 KEY2
	if (minivci_driver->RxByteDrv( (d_pdu->CP_W2Max),&recv_key[0]) != BYTE_OK)
	{
		printfuartdebug("KEY1 error\r\n");
		return false;
	}
	else
	{
		RX_DATA[*RX_DATA_POS]=recv_key[0];
		*RX_DATA_POS+=1;
	}
	if ( minivci_driver->RxByteDrv( (d_pdu->CP_W3Max),&recv_key[1]) != BYTE_OK)
	{
		printfuartdebug("KEY2 error\r\n");
		return false;
	}
	else
	{
		RX_DATA[*RX_DATA_POS]=recv_key[1];
		*RX_DATA_POS+=1;
	}
	//printf_usb2("%2X %2X \r\n",recv_key[0],recv_key[1]); //test code
	 
	minivci_config->KeyByte1=recv_key[0];
	minivci_config->KeyByte2=recv_key[1];
	
	non_key2 = ~recv_key[1];
	
	//接收其他数据
	if(minivci_config->KeyByteNum > 3)
	{
		KeyByteNum = minivci_config->KeyByteNum - 3;
		for(loop = 0; loop < KeyByteNum; loop++)
		{
			if(BYTE_OK != minivci_driver->RxByteDrv(minivci_config->W3, (uint8 *)&RX_DATA[*RX_DATA_POS]))
			{
				*RX_DATA_POS+=1;
				//UARTAnswerErr(0xfc, 0x03);
				printfuartdebug("PassThruDriver.RxByteDrv other\r\n");
				return FALSE;
			}
			else *RX_DATA_POS+=1;
		}
	}
	
	if(minivci_config->FiveBaudMod&FIVE_BAUD_MOD_KEYBYTE)
	{
		DelayNms(d_pdu->CP_W4Min); 
		hal_kwp_send_byte_kwp2(non_key2);
		//printfuartdebug("non_key2==%02x\r\n",non_key2);
	}
	
#if 0
	*CP_5BaudAddress=0;
	if (minivci_driver->RxByteDrv(500,&recv_add) != BYTE_OK)
	{
		return false;
	}
	*CP_5BaudAddress=recv_add;
#endif
	//printf_usb2("%2X\r\n",recv_add);		//test code
	return true;
 
}

uint8 PassThru_KWP_RxData_Time(uint16 TimeOutMs)
{
	//printfkwp(" msDelayCounter==%d \r\n",msDelayCounter);
	T_delay_ms_set(TimeOutMs);
	
	while( T_delay_ms_get_status() == RESET)  
	{ 
		if(Get_Bus_status()==TRUE)
		{
			return TRUE;
		}
	}
	return FALSE;
	
}

uint8 PassThru_KWP_Init_Slow(uint8 *TXdata)
{
    uint32 DlyInByteCounter;
	uint8  KeyByteNum;
	uint8  ByteStatus;
	uint8  ByteData;
	uint8  ByteNum;
	uint8  loop;
	uint16 ByteLength;
	uint8 CP_5BaudAddress;
	PassThru_KWP_Parm_Init(&Minivci_KWP_Param,&PassThruConfig);
	printfkwp(" Protocol==%08x  GndCom=%d\r\n",PassThruConfig.Protocol,PassThruConfig.GndCom);
    //V6.91 针对三菱，对引脚接地指令的特别处理： 接地70ms，释放700ms，然后再接地70ms
    if((PassThruConfig.Protocol == 0x030000) && (PassThruConfig.GndCom != 0x00))
    {
	 printfkwp(" Protocol start \r\n");
        SET_Another_STATUS(SET_OUT_STATUS_0);;
        vTaskDelay(E70B_vTaskDelay_Modulus(150));

        SET_Another_STATUS(SET_OUT_STATUS_1);
        vTaskDelay(E70B_vTaskDelay_Modulus(700));
		
        SET_Another_STATUS(SET_OUT_STATUS_0);
        vTaskDelay(E70B_vTaskDelay_Modulus(700));
    }
    else
    {
        //TOOL发送地址码之前总线的最小空闲时间 300ms STM32不能等这个300ms不然会慢
       // DelayNms(300);
    }
    PassThruConfig.endmsCounter = 0;
    if((PassThruConfig.BaudRate == 0)||(PassThruConfig.BaudRate<200))
    {
        SetSoftUart(5);
		CP_5BaudAddress = TXdata[3];
        //_SoftTxByte(TXdata[3],0);	//硬串口中能发送50baud上的数据，此处用软串口发送
    }
    else
    {
        CP_5BaudAddress = TXdata[3];
        SetSoftUart(PassThruConfig.BaudRate);  
    }
	
    
    PassThruConfig.endmsCounter = 0;
    ByteNum = 8;

	if(FALSE==hal_kwp_protocol_init_slow_minivci(&Minivci_KWP_Param,&PassThruConfig,&PassThruDriver,&CP_5BaudAddress,EcuComRxBuf,&ByteNum))
	{
		// vTaskDelay(10);
		//printf_usb2(" CP_5BaudAddress error =%02x TXdata[3]=%02x\r\n" ,CP_5BaudAddress ,TXdata[3]);
		return ;
	}
	else
	{	
		//printfkwp(" CP_5BaudAddress=%02x TXdata[3]=%02x\r\n" ,CP_5BaudAddress ,TXdata[3]);
	}
 
    EcuComRxBuf[2] = KWP2K_ECU_INIT;
    EcuComRxBuf[3] = (PassThruConfig.CurrBaud >> 24) & 0xff;
    EcuComRxBuf[4] = (PassThruConfig.CurrBaud >> 16) & 0xff;
    EcuComRxBuf[5] = (PassThruConfig.CurrBaud >> 8) & 0xff;
    EcuComRxBuf[6] = PassThruConfig.CurrBaud & 0xff;
    EcuComRxBuf[7] = 0x55;
	 
	
    //接收后续数据
    if((PassThruConfig.FiveBaudMod&FIVE_BAUD_MOD_ADDRESS) && ((PassThruConfig.Protocol) & 0xff0000) != 0x840000)
    {
 
		if(PassThru_KWP_RxData_Time(2000)==FALSE)return;
		 
		LineLevelFlag =LINE_UNVALID;//已等到停止位，所以此时直接收数据
		DlyInByteCounter = PassThruConfig.P1Max ;
        //带长度控制的协议 解决五十铃安全气囊接收第一帧数据的问题
        if(((PassThruConfig.Protocol>>16)&0xff) == PROTOCOL_FRAME_LENGTH2)//按照长度接收
        {					
			ByteLength = (PassThruConfig.Protocol>>8)&0xff;//接收长度位
            ByteLength++;
            if(ByteNum + ByteLength > ECURXBUFLEN)
            {
                return FALSE;
            }
            for(loop=0; loop<ByteLength; loop++)
            {
                if(BYTE_OK == PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData))
                {
                    EcuComRxBuf[ByteNum++] = ByteData;
                }
                else
                {
                    return FALSE;
                }
            }
            ByteLength = ByteData + (PassThruConfig.Protocol&0xff) - ByteLength;//接收后面数据
            if(ByteNum + ByteLength > ECURXBUFLEN)
            {
                return FALSE;
            }
            for(loop=0; loop<ByteLength; loop++)
            {
                if(BYTE_OK == PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData))
                {
                    EcuComRxBuf[ByteNum++] = ByteData;
                }
                else
                {
                    return FALSE;
                }
            }
        }
        else
        {					
            while(ByteNum < ECURXBUFLEN)
            {
                ByteStatus = PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData);
                if(ByteStatus == BYTE_FALSE)
                {
                    return FALSE;
                }
                else if(ByteStatus == EOF_FLAG)
                {
                    break;
                }
                else if(ByteStatus == BYTE_OK)
                {
                    SetEndmsCounter();
                    EcuComRxBuf[ByteNum++] = ByteData;
                }
                else
                {
                    return FALSE;
                }
            }
        }
    }
    //====================================================
    ByteNum = ByteNum - 2;
    EcuComRxBuf[0] = ByteNum >> 8;
    EcuComRxBuf[1] = ByteNum & 0xff;
    UartTxQueuePack(EcuComRxBuf);
    StatusByte |= ECU_INIT_OK;
    return TRUE;
}

/*********************************************************************************************
* 名称    ： PassThru_KWP_TxPack_TxData
* 功能    ： KWP协议发送数据
* 入口参数：
*
*
* 出口参数：
* 说明    ：
*
***********************************************************************************************/
uint8 PassThru_KWP_TxPack_TxData(uint8 *TXdata)
{
    uint8 DlyInTxByte;
    uint16 PackLength;
    uint16 loop;

    PackLength = (TXdata[0] << 8) + TXdata[1];
	 
    if(PackLength < 2) return FALSE;
	hal_kmp_uart_init_irq_set(0);
//	InitTxBufToQueue();//发送命令之前清空缓冲区
    DlyInTxByte = PassThruConfig.P4Min;
    PackLength = PackLength - 2;
    TXdata = TXdata + 3;
    for(loop = 0; loop < PackLength; loop++)
    {
        PassThruDriver.TxByteDrv(*TXdata++);
        DelayTxByte(DlyInTxByte);
		
    }
    PassThruDriver.TxByteDrv(*TXdata++);	 
    SetEndmsCounter();//endmsCounter = _1msCounter;
	//vTaskDelay(0);
	hal_kmp_uart_init_irq_set(1);  
    return TRUE;
}

/*********************************************************************************************
* 名称    ： PassThru_KWP_TxPack_Normal
* 功能    ： KWP协议发送一帧数据 正常模式
* 入口参数：
*
*
* 出口参数：
* 说明    ：
*
***********************************************************************************************/
extern void printferror(const char *fmt,...);
uint8 PassThru_KWP_TxPack_Normal(uint8 *TXdata)
{
// 	NVIC_DisableIRQ(EINT3_IRQn);
// 	NVIC_ClearPendingIRQ(EINT3_IRQn); 
    if(!GetLineMonitor()) return FALSE;//����500us�Ŀ��б���Ϊ��Ч���� ����ʱ500ms���˳�����
//	InitTxBufToQueue(); 
    SendDelayCounter(); 
    PassThru_KWP_TxPack_TxData(TXdata);
    return TRUE;
}

/*********************************************************************************************
* 名称    ： PassThru_KWP_TxPack_Delay
* 功能    ： KWP协议发送一帧数据 延迟发送
* 入口参数：
*
*
* 出口参数：
* 说明    ：
*
***********************************************************************************************/
uint8 PassThru_KWP_TxPack_Delay(uint8 *TXdata)
{
    uint32 DlyInByteCounter;
    uint8  ByteData;
    uint8  ByteStatus;
//	InitTxBufToQueue();//发送命令之前清空缓冲区
    if((PassThruConfig.P3Min & 0x80) == 0x80)
    {
        DlyInByteCounter = ((PassThruConfig.P3Min) & 0x7f);
		DlyInByteCounter|=0x80;
		//if(DlyInByteCounter%10)DlyInByteCounter=(DlyInByteCounter/10)+1;
    }
    else
    {
		if(PassThruConfig.P3Min==0)
			DlyInByteCounter = 0x2;
		else
			DlyInByteCounter = (PassThruConfig.P3Min) ;
    }
    if(((PassThruConfig.Protocol) & 0x00ff0000) == 0x00840000)	//XK 20130619 ISUZU HONGKONG
    {
        if(counter_1ms > (PassThruConfig.P3Min >> 1))
        {
			UARTAnswerErr(0x38, 0x04);
			return FALSE;
        }
    }
    counter_1ms = 0;
	
    while(1)
    {
		
        ByteStatus = PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData);
        if(ByteStatus == EOF_FLAG)
        {
            break;
        }
        if(counter_1ms > 1000)
        {
            UARTAnswerErr(0x38, 0x01);
            return FALSE;
        }
    }
 
    PassThru_KWP_TxPack_TxData(TXdata);
    return TRUE;
}

/*********************************************************************************************
* 名称    ： PassThru_KWP_TxPack_Volvo
* 功能    ： J1708协议延迟发送一帧数据
* 入口参数：
*
*
* 出口参数：
* 说明    ：
*
***********************************************************************************************/
uint8 PassThru_KWP_TxPack_Volvo(uint8 *TXdata)
{
    uint8  ByteStatus;
    uint8  ByteData;
//  uint8  ByteFlag = 0;
    uint32 DlyInByteCounter;
//	InitTxBufToQueue();//发送命令之前清空缓冲区
    if((PassThruConfig.P2Min & 0x80) == 0x80)
    {
        DlyInByteCounter = ((PassThruConfig.P2Min) & 0x7f);
		DlyInByteCounter|=0x80;
		//if(DlyInByteCounter%10) DlyInByteCounter=(DlyInByteCounter/10+1);
    }
    else
    {
        DlyInByteCounter = (PassThruConfig.P2Min >> 1) ;
    }

    counter_1ms = 0;

    while(1)
    {
        ByteStatus = PassThruDriver.RxByteDrv(DlyInByteCounter, &ByteData);
        //if(ByteStatus == BYTE_OK)
        //{
        //	ByteFlag = 1;
        //}
        //if(ByteStatus == EOF_FLAG && ByteFlag == 1)
        //{
        //	break;
        //}
        if(ByteStatus == EOF_FLAG)
        {
            break;
        }
        if(counter_1ms > 1000)
        {
            UARTAnswerErr(0x38, 0x02);
            return FALSE;
        }
    }
	zyIrqDisable();
    PassThru_KWP_TxPack_TxData(TXdata);
	zyIrqEnable();
    return TRUE;
}

/*********************************************************************************************
* 名称    ： PassThru_KWP_RxPack_Normal
* 功能    ： KWP协议接收一帧数据 正常模式
* 入口参数：
*
*
* 出口参数：
* 说明    ：按照时间接收 P1Max
*
***********************************************************************************************/
extern void printferror(const char *fmt,...);
uint8 PassThru_KWP_RxPack_Normal()
{
	uint8  ByteStatus;
	uint8  ByteData;
	uint16 ByteNum = 0;
	uint32 DlyInByteCounter;
	uint8_t EcuComRxBufTemp[ECURXBUFLEN];
    
	if(Get_Bus_status()==FALSE)
	return FALSE;
    if((PassThruConfig.BaudRate == 5)||((PassThruConfig.BaudRate <200)&&(ComCtl.RxHardWare == USE_HARD)))
    {
		printfkwp("PassThru_KWP_RxPack_Normal1");
		if(GET_RX0_STATUS==1)
		return FALSE;
		DlyInByteCounter =   (PassThruConfig.P1Max);
		if(BYTE_OK != PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData))
		return FALSE;
		memset(EcuComRxBufTemp,0,sizeof(EcuComRxBufTemp));
        EcuComRxBufTemp[ByteNum++] = 0x00;
        EcuComRxBufTemp[ByteNum++] = 0x02;
        EcuComRxBufTemp[ByteNum++] = KWP2K_DATA;
        EcuComRxBufTemp[ByteNum++] = ByteData;
        vTaskDelay(E70B_vTaskDelay_Modulus(200)); //接收的时候延迟(50ms) 1/4bit
        PassThruConfig.Counter = 0x00; //v6.61
        UartTxQueuePack(EcuComRxBufTemp);
        SimulateEcu5bps();
        return TRUE;
    }
	DlyInByteCounter = PassThruConfig.P1Max ;
	#if 1
	if((DlyInByteCounter==9)&&(PassThruConfig.P2Min==0x32))
	{
		DlyInByteCounter+=1;//解决：bike (凯越)Excelle 2012 遥控功能启动 显示--bug //
	}
	#endif
	
	
	ByteNum=3;
    while(ByteNum < ECURXBUFLEN)
    {
        ByteStatus = PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData);
        if(ByteStatus == BYTE_FALSE)
        {
            return FALSE;
        }
        else if(ByteStatus == EOF_FLAG)
        {
            break;
        }
        else if(ByteStatus == BYTE_OK)
        {
            SetEndmsCounter();
            EcuComRxBuf[ByteNum++] = ByteData;
        }
        else
        {
            return FALSE;
        }
    }
    EcuComRxBuf[0] = 0x00;
    EcuComRxBuf[1] = 0x00;
    EcuComRxBuf[2] = KWP2K_DATA;

    if(ByteNum == 0x03) return FALSE;//没有收到数据 过滤杂波
    counter_1ms = 0;
    ByteNum -= 2;
    EcuComRxBuf[0] = ByteNum >> 8;
    EcuComRxBuf[1] = ByteNum & 0xff;
    UartTxQueuePack(EcuComRxBuf);
    return TRUE;
}

/*********************************************************************************************
* 名称    ： PassThru_KWP_RxPack_Kwp2000
* 功能    ： KWP协议接收一帧数据 标准KWP2000
* 入口参数：
*
*
* 出口参数：
* 说明    ：
*
***********************************************************************************************/
uint8 PassThru_KWP_RxPack_Kwp2000()//仪器_audio14_kwp.xml
{
    uint8  ByteFormat;
    uint8  CheckSum;
    uint8  CheckStart;
    uint8  ByteLength;
    uint16 ByteNum = 0;
    uint16 loop;
    uint8	ByteData;
    uint32 DlyInByteCounter;

	if(Get_Bus_status()==FALSE)
	return FALSE;
	
    if((PassThruConfig.BaudRate == 5)||(PassThruConfig.BaudRate <200))
    {
		//if(GET_RX0_STATUS==1)
		//return FALSE;
        DlyInByteCounter =  (PassThruConfig.P1Max);
        if(BYTE_OK != PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData))
            return FALSE;
        EcuComRxBuf[ByteNum++] = 0x00;
        EcuComRxBuf[ByteNum++] = 0x02;
        EcuComRxBuf[ByteNum++] = AUDI_DATA;
        EcuComRxBuf[ByteNum++] = ByteData;
        vTaskDelay(E70B_vTaskDelay_Modulus(200)); //接收的时候延迟(50ms) 1/4bit
        PassThruConfig.Counter = 0x00; //v6.61
        UartTxQueuePack(EcuComRxBuf);
        SimulateEcu5bps();
        return TRUE;
    }
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = KWP2K_DATA;
    DlyInByteCounter = PassThruConfig.P1Max ;
    //接收第一个字节,判断格式
    if(BYTE_OK != PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteFormat))
        return FALSE;
    ByteLength = ByteFormat&FmtLenBit;
    EcuComRxBuf[ByteNum++] = ByteFormat;
    if((ByteFormat&0xC0) == 0x00)
    {
        if(ByteLength == 0x00)//格式是：Fmt,LenN,dat0,dat1...,datN,Checksum
        {
            if(BYTE_OK != PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteLength))
                return FALSE;
            EcuComRxBuf[ByteNum++] = ByteLength;
        }
        else
        {
        }
    }
    else if((ByteFormat&0xC0) == 0x40 || (ByteFormat&0xC0) == 0x80 || (ByteFormat&0xC0) == 0xC0)
    {
        if(ByteLength == 0x00)
        {
            if(BYTE_OK != PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&EcuComRxBuf[ByteNum++]))
                return FALSE;
            if(BYTE_OK != PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&EcuComRxBuf[ByteNum++]))
                return FALSE;
            if(BYTE_OK != PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteLength))
                return FALSE;
            EcuComRxBuf[ByteNum++] = ByteLength;
        }
        else
        {
            if(BYTE_OK != PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&EcuComRxBuf[ByteNum++]))
                return FALSE;
            if(BYTE_OK != PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&EcuComRxBuf[ByteNum++]))
                return FALSE;
        }
    }
    if(ByteNum + ByteLength > ECURXBUFLEN - 1)
    {
        return FALSE;
    }
    CheckStart = ByteNum;
    while(ByteLength-- > 0)
    {
        if(BYTE_OK != PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&EcuComRxBuf[ByteNum++]))
            return FALSE;
    }
    //接受checksum
    if(BYTE_OK != PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&EcuComRxBuf[ByteNum++]))
        return FALSE;
    //计算checksum
    CheckSum = 0x00;
    for(loop=3; loop<ByteNum-1; loop++)
    {
        CheckSum = CheckSum + EcuComRxBuf[loop];
    }
    if(EcuComRxBuf[ByteNum-1] != CheckSum)
        return FALSE;
    SetEndmsCounter();
    if(ByteNum == 0x03) return FALSE;//没有收到数据
    ByteNum -= 2;
    EcuComRxBuf[0] = ByteNum >> 8;
    EcuComRxBuf[1] = ByteNum & 0xff;
    PassThru_SetKeepComSwitch2(EcuComRxBuf+CheckStart);
    UartTxQueuePack(EcuComRxBuf);
    return TRUE;
}

/*********************************************************************************************
* 名称    ： PassThru_KWP_RxPack_Nissan
* 功能    ： 日产老协议接收数据
* 入口参数：
*
*
* 出口参数：
* 说明    ：接收一个字节 0xFF接收一帧数据
*
***********************************************************************************************/
uint8 PassThru_KWP_RxPack_Nissan()
{
    uint8  ByteLength;
    uint8  ByteFormat;
    uint16 ByteNum = 0;
    uint8 ByteData;
    uint32 DlyInByteCounter;

   if(Get_Bus_status()==FALSE)
		 return FALSE;
    if((PassThruConfig.BaudRate == 5)||(PassThruConfig.BaudRate<200))
    {
		if(GET_RX0_STATUS==1)
		return FALSE;
		DlyInByteCounter =  (PassThruConfig.P2Min);
		if(BYTE_OK != PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData))
		return FALSE;
		EcuComRxBuf[ByteNum++] = 0x00;
		EcuComRxBuf[ByteNum++] = 0x02;
		EcuComRxBuf[ByteNum++] = KWP2K_DATA;
		EcuComRxBuf[ByteNum++] = ByteData;
		vTaskDelay(E70B_vTaskDelay_Modulus(200)); //接收的时候延迟(50ms) 1/4bit
		PassThruConfig.Counter = 0x00; //v6.61
		UartTxQueuePack(EcuComRxBuf);
		SimulateEcu5bps();
		return TRUE;
    }
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = KWP2K_DATA;
    DlyInByteCounter = PassThruConfig.P1Max ;
    //接收第一个字节,判断格式
    if(BYTE_OK != PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteFormat))
        return FALSE;
    EcuComRxBuf[ByteNum++] = ByteFormat;
    if(ByteFormat == 0xff)
    {
        if(BYTE_OK != PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteLength))
            return FALSE;
        EcuComRxBuf[ByteNum++] = ByteLength;
        if(ByteNum + ByteLength > ECURXBUFLEN)
        {
            return FALSE;
        }
        while(ByteLength-- > 0)
        {
            if(BYTE_OK != PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&EcuComRxBuf[ByteNum++]))
                return FALSE;
        }
    }
    ByteNum -= 2;
    EcuComRxBuf[0] = ByteNum >> 8;
    EcuComRxBuf[1] = ByteNum & 0xff;
    UartTxQueuePack(EcuComRxBuf);
    return TRUE;
}

/*********************************************************************************************
* 名称    ： PassThru_KWP_RxPack_Mitsub
* 功能    ： 三菱协议接收一帧数据
* 入口参数：
*
*
* 出口参数：
* 说明    ：接收一个字节
*
***********************************************************************************************/
uint8 PassThru_KWP_RxPack_Mitsub()
{
    uint8  ByteData;
    uint16 ByteNum = 0;
    uint32 DlyInByteCounter;
	uint8_t EcuComRxBufTemp[ECURXBUFLEN];
   if(Get_Bus_status()==FALSE)
   return FALSE;

    if((PassThruConfig.BaudRate == 5)||(PassThruConfig.BaudRate<200))
    {
		if(GET_RX0_STATUS==1)
		return FALSE;
		DlyInByteCounter =  PassThruConfig.P2Min ;
		if(BYTE_OK != PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData))
		return FALSE;
		memset(EcuComRxBufTemp,0,sizeof(EcuComRxBufTemp));
		EcuComRxBufTemp[ByteNum++] = 0x00;
		EcuComRxBufTemp[ByteNum++] = 0x02;
		EcuComRxBufTemp[ByteNum++] = KWP2K_DATA;
		EcuComRxBufTemp[ByteNum++] = ByteData;
		vTaskDelay(E70B_vTaskDelay_Modulus(200)); //接收的时候延迟(50ms) 1/4bit
		PassThruConfig.Counter = 0x00; //v6.61
        UartTxQueuePack(EcuComRxBufTemp);
        SimulateEcu5bps();
        return TRUE;
    }
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = KWP2K_DATA;
    DlyInByteCounter = PassThruConfig.P1Max ;
	 
    if(BYTE_OK == PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData))
    {
        EcuComRxBuf[ByteNum++] = ByteData;
    }
    else
    {
        return FALSE;
    }
    SetEndmsCounter();
    ByteNum -= 2;
    EcuComRxBuf[0] = ByteNum >> 8;
    EcuComRxBuf[1] = ByteNum & 0xff;
    UartTxQueuePack(EcuComRxBuf);
    return TRUE;
}

/*********************************************************************************************
* 名称    ： PassThru_KWP_RxPack_Length
* 功能    ： 按照长度接收一帧数据
* 入口参数：
*
*
* 出口参数：
* 说明    ：
*
***********************************************************************************************/
uint8 PassThru_KWP_RxPack_Length()//仪器_bmw.xml
{
    uint8  ByteData;
    uint16 ByteLength;
    uint16 loop;
    uint16 ByteNum = 0;
    uint32 DlyInByteCounter;
	uint8_t EcuComRxBufTemp[ECURXBUFLEN];	
	if(Get_Bus_status()==FALSE)
	return FALSE;
    if((PassThruConfig.BaudRate == 5)||(PassThruConfig.BaudRate<200))
    {
		if(GET_RX0_STATUS==1)
		return FALSE;
		DlyInByteCounter =  (PassThruConfig.P2Min);
		if(BYTE_OK != PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData))
		return FALSE;
		memset(EcuComRxBufTemp,0,sizeof(EcuComRxBufTemp));	
        EcuComRxBufTemp[ByteNum++] = 0x00;
        EcuComRxBufTemp[ByteNum++] = 0x02;
        EcuComRxBufTemp[ByteNum++] = KWP2K_DATA;
        EcuComRxBufTemp[ByteNum++] = ByteData;
        PassThruConfig.Counter = 0x00; //v6.61
        UartTxQueuePack(EcuComRxBufTemp);

        vTaskDelay(E70B_vTaskDelay_Modulus(200));
        SimulateEcu5bps();
        return TRUE;
    }
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = KWP2K_DATA;
    DlyInByteCounter = PassThruConfig.P1Max;
	 
    ByteLength = (PassThruConfig.Protocol>>8)&0xff;
    ByteLength++;
    if(ByteNum + ByteLength > ECURXBUFLEN)
    {
        return FALSE;
    }
	
	#ifndef M_KWPTIMECALC//10年本田 奥德赛 安全气囊 无法进入 【未进行测试验证】
	if(DlyInByteCounter==1)
	{
		if(PassThruConfig.BaudRate == 0x28a0)
		{
			(DlyInByteCounter+=2);
		}
	}
	#endif 
	
    for(loop=0; loop<ByteLength; loop++)
    {
        if(BYTE_OK == PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData))
        {
            EcuComRxBuf[ByteNum++] = ByteData;
        }
        else
        {
            return FALSE;
        }
    }
    
    ByteLength = ByteData + (PassThruConfig.Protocol&0xff) - ByteLength;
    if(ByteNum + ByteLength > ECURXBUFLEN)
    {
        return FALSE;
    }
    for(loop=0; loop<ByteLength; loop++)
    {
        if(BYTE_OK == PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData))
        {
            EcuComRxBuf[ByteNum++] = ByteData; 
        }
        else
        {
            return FALSE;
        }
    }

    counter_1ms = 0;
    SetEndmsCounter();
    ByteNum -= 2;
    EcuComRxBuf[0] = ByteNum >> 8;
    EcuComRxBuf[1] = ByteNum & 0xff;
    UartTxQueuePack(EcuComRxBuf);
    return TRUE;
}

/*********************************************************************************************
* 名称    ： PassThru_KWP_RxPack_Length3
* 功能    ： 标准OBDII J1979协议 每帧最多11字节 3字节头 8字节数据
* 入口参数：
*
*
* 出口参数：
* 说明    ：
*
***********************************************************************************************/
uint8 PassThru_KWP_RxPack_Length3()
{
	uint8  ByteData;
	uint16 ByteLength;
	uint16 loop;
	uint16 ByteNum = 0;
	uint32 DlyInByteCounter;
   if(Get_Bus_status()==FALSE)
		 return FALSE;
	EcuComRxBuf[ByteNum++] = 0x00;
	EcuComRxBuf[ByteNum++] = 0x00;
	EcuComRxBuf[ByteNum++] = KWP2K_DATA;
	DlyInByteCounter = PassThruConfig.P1Max ;
	ByteLength = PassThruConfig.Protocol&0x0fff;//最多4095字节
	if(ByteNum + ByteLength + 1 > ECURXBUFLEN)//根据实际buffer缓冲区接收数据
	{
		return FALSE;
	}
	for(loop=0; loop<ByteLength; loop++)
	{
		if(BYTE_OK == PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData))
		{
			EcuComRxBuf[ByteNum++] = ByteData;
		}
		else
		{
			break;
		}
	}
	counter_1ms = 0;
	SetEndmsCounter();
	ByteNum -= 2;
	EcuComRxBuf[0] = ByteNum >> 8;
	EcuComRxBuf[1] = ByteNum & 0xff;
	 UartTxQueuePack(EcuComRxBuf);
	//GetLineLevelStatus();
	return TRUE;
}
/*********************************************************************************************
* 名称    ： PassThru_KWP_RxPack_MazdaKWP
* 功能    ： 按照长度接收一帧数据
* 入口参数：
*
*
* 出口参数：
* 说明    ：
*
***********************************************************************************************/
uint8 PassThru_KWP_RxPack_MazdaKWP()
{
    uint8  ByteData;
    uint8  CheckSum;
    uint16 ByteLength;
    uint16 loop;
    uint16 ByteNum = 0;
    uint32 DlyInByteCounter;
	if(Get_Bus_status()==FALSE)
	return FALSE;
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = KWP2K_DATA;
    DlyInByteCounter = PassThruConfig.P1Max ;
    if(BYTE_OK == PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData))
    {
        EcuComRxBuf[ByteNum++] = ByteData;
        if((ByteData&0x0f) != 0x04)
        {
             //GetLineLevelStatus();
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
    ByteLength = (ByteData>>4)&0xff;
    if(ByteNum + ByteLength > ECURXBUFLEN)
    {
        return FALSE;
    }
    for(loop=0; loop<ByteLength; loop++)
    {
        if(BYTE_OK == PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData))
        {
            EcuComRxBuf[ByteNum++] = ByteData;
        }
        else
        {
           // GetLineLevelStatus();
            return FALSE;
        }
    }
    //计算checksum
    CheckSum = 0x00;
    for(loop=3; loop<ByteNum-1; loop++)
    {
        CheckSum = CheckSum + EcuComRxBuf[loop];
    }
    if(EcuComRxBuf[ByteNum-1] != CheckSum)
        return FALSE;
    SetEndmsCounter();
    if(ByteNum == 0x03) return FALSE;//没有收到数据
    ByteNum -= 2;
    EcuComRxBuf[0] = ByteNum >> 8;
    EcuComRxBuf[1] = ByteNum & 0xff;
    UartTxQueuePack(EcuComRxBuf);
    //GetLineLevelStatus();
    return TRUE;
}
/*********************************************************************************************
* 名称    ： PassThru_KWP_RxPack_Volvo
* 功能    ： 接收一帧数据 J1708协议
* 入口参数：
*
*
* 出口参数：
* 说明    ： 用时间间隔来判断是否收到一帧，时间间隔为1ms
*            打包后的结果格式是：
*            数据长度H＋数据长度L+CMD+ DAT0+..+DATn
*
***********************************************************************************************/
extern void UartTxQueuePack_DMA(uint8* EcuComRxBuf);
uint8 PassThru_KWP_RxPack_Volvo()
{
    uint8  ByteStatus;
    uint8  ByteData;
    uint8  ByteCheck;
    uint16 ByteNum = 0;
    uint32 DlyInByteCounter;
	if(Get_Bus_status()==FALSE)
	return FALSE;
//     if((PassThruConfig.BaudRate == 5)||(PassThruConfig.BaudRate<200))
//     {
//         if(GET_RX0_STATUS==1)
//             return FALSE;
//         DlyInByteCounter = (Fpclk / 1000) * (PassThruConfig.P2Min);
//         if(BYTE_OK != SoftUartRxByte(DlyInByteCounter, (uint8 *)&ByteData))
//             return FALSE;
//         EcuComRxBuf[ByteNum++] = 0x00;
//         EcuComRxBuf[ByteNum++] = 0x02;
//         EcuComRxBuf[ByteNum++] = KWP2K_DATA;
//         EcuComRxBuf[ByteNum++] = ByteData;
//         DelayNms(200); //接收的时候延迟(50ms) 1/4bit
//         PassThruConfig.Counter = 0x00; //v6.61
//         UartTxQueuePack(EcuComRxBuf);
//         SimulateEcu5bps();
//         return TRUE;
//     } 
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = KWP2K_DATA;
    if((PassThruConfig.Protocol & 0x008000) == 0x008000)
    {
        DlyInByteCounter = ((PassThruConfig.Protocol>>8)&0x7f);
		DlyInByteCounter|=0x80;
		//if(DlyInByteCounter%10)DlyInByteCounter=(DlyInByteCounter/10)+1;
    }
    else
    {
        //DlyInByteCounter = (((PassThruConfig.Protocol>>8)&0x7f)+1) * (Fpclk / 1000);//原来f6这里没有+1这项，估计优化后f8程序运行较快，郊率高，故在这里多等1ms
        DlyInByteCounter = ((uint8)(PassThruConfig.Protocol>>8))  ;
		#ifndef M_KWPTIMECALC
		 DlyInByteCounter = +2 ;//测试发现沃尔沃柴油 ecu 长帧收不到 增加读取时间
		#endif
    }
    ByteCheck = 0x00;//检查校验位 一字节和取补
    while(ByteNum < ECURXBUFLEN)
    {
        ByteStatus = PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData);
        if(ByteStatus == BYTE_FALSE)
        {
            return FALSE;
        }
        else if(ByteStatus == EOF_FLAG)
        {
            break;
        }
        else if(ByteStatus == BYTE_OK)
        {
            SetEndmsCounter();
            EcuComRxBuf[ByteNum++] = ByteData;
            ByteCheck += ByteData;
        }
        else
        {
            return FALSE;
        }
    }
    if(ByteNum == 0x03) return FALSE;//没有收到数据 过滤杂波
    if(ByteNum < 0x06) return FALSE;//数据长度不正确 过滤杂波
    if(ByteCheck > 0x00) return FALSE;//校验位错误 过滤杂波
    zyIrqDisable();
    ByteNum -= 2;
    EcuComRxBuf[0] = ByteNum >> 8;
    EcuComRxBuf[1] = ByteNum & 0xff;
    UartTxQueuePack(EcuComRxBuf);
    zyIrqEnable();
    return TRUE;
}

/*********************************************************************************************
* 名称    ： PassThru_KWP_RxPack_J1708MID
* 功能    ： 接收一帧数据 J1708协议
* 入口参数：
*
*
* 出口参数：
* 说明    ： 用时间间隔来判断是否收到一帧，时间间隔为1ms
*            打包后的结果格式是：
*            数据长度H＋数据长度L+CMD+ DAT0+..+DATn
*
***********************************************************************************************/
uint8 PassThru_KWP_RxPack_J1708MID()
{
    uint8  ByteStatus;
    uint8  ByteData;
    uint8  ByteCheck;
    uint16 ByteNum = 0;
    uint32 DlyInByteCounter;
	if(Get_Bus_status()==FALSE)
	return FALSE;
    if(PassThruConfig.BaudRate == 5)
    {
		if(GET_RX0_STATUS==1)
		return FALSE;
		DlyInByteCounter =   (PassThruConfig.P2Min);
		if(BYTE_OK != PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData))
		return FALSE;
        EcuComRxBuf[ByteNum++] = 0x00;
        EcuComRxBuf[ByteNum++] = 0x02;
        EcuComRxBuf[ByteNum++] = KWP2K_DATA;
        EcuComRxBuf[ByteNum++] = ByteData;
        vTaskDelay(E70B_vTaskDelay_Modulus(200)); //接收的时候延迟(50ms) 1/4bit
        PassThruConfig.Counter = 0x00; //v6.61
        UartTxQueuePack(EcuComRxBuf);
        SimulateEcu5bps();
        return TRUE;
    }
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = KWP2K_DATA;
    if((PassThruConfig.P2Min & 0x80) == 0x80)
    {
        DlyInByteCounter = ((PassThruConfig.P2Min) & 0x7f);
		DlyInByteCounter|=0x80;
		//if(DlyInByteCounter%10)DlyInByteCounter=(DlyInByteCounter/10)+1;
    }
    else
    {
        DlyInByteCounter = (PassThruConfig.P2Min) ;
    }
    ByteCheck = 0x00;//检查校验位 一字节和取补
    while(ByteNum < ECURXBUFLEN)
    {
        ByteStatus = PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData);
        if(ByteStatus == BYTE_FALSE)
        {
            return FALSE;
        }
        else if(ByteStatus == EOF_FLAG)
        {
            break;
        }
        else if(ByteStatus == BYTE_OK)
        {
            SetEndmsCounter();
            EcuComRxBuf[ByteNum++] = ByteData;
            ByteCheck += ByteData;
        }
        else
        {
            return FALSE;
        }
    }
    if(ByteNum == 0x03) return FALSE;//没有收到数据 过滤杂波
    if(ByteNum < 0x06) return FALSE;//数据长度不正确 过滤杂波
    if(ByteCheck > 0x00) return FALSE;//校验位错误 过滤杂波
    zyIrqDisable();
    ByteNum -= 2;
    EcuComRxBuf[0] = ByteNum >> 8;
    EcuComRxBuf[1] = ByteNum & 0xff;
    //Header=0的时候全部接收
    if(PassThruConfig.Header == 0x00)
    {
        UartTxQueuePack(EcuComRxBuf);
    }
    //Header=0的时候过滤数据
    else if((EcuComRxBuf[3] | PassThruConfig.HeaderAR) == PassThruConfig.HeaderID)
    {
        UartTxQueuePack(EcuComRxBuf);
    }
    //模拟测试的时候返回AC开头数据
    else if((PassThruConfig.Listen == SIMULATION )&&( EcuComRxBuf[3] == 0xac))
    {
        UartTxQueuePack(EcuComRxBuf);
    }
    zyIrqEnable();
    return TRUE;
}

/*********************************************************************************************
* 名称    ： PassThru_KWP_RxPack_Cater
* 功能    ： 接收一帧数据
* 入口参数：
*
*
* 出口参数：
* 说明    ：卡特彼勒车型专用
*
***********************************************************************************************/
uint8 PassThru_KWP_RxPack_Cater()
{
    uint8  ByteStatus;
    uint8  ByteData;
    uint8  ByteCheck;
    uint16 ByteNum = 0;
    uint32 DlyInByteCounter;
	uint8_t EcuComRxBufTemp[ECURXBUFLEN];	
	if(Get_Bus_status()==FALSE)
	return FALSE;
	printferror("PassThru_KWP_RxPack_Cater");
    if((PassThruConfig.BaudRate == 5)||(PassThruConfig.BaudRate<200))
    {
		if(GET_RX0_STATUS==1)
		return FALSE;
		DlyInByteCounter =   (PassThruConfig.P2Min);
		if(BYTE_OK != PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData))
		return FALSE;
		memset(EcuComRxBufTemp,0,sizeof(EcuComRxBufTemp));
		EcuComRxBufTemp[ByteNum++] = 0x00;
		EcuComRxBufTemp[ByteNum++] = 0x02;
		EcuComRxBufTemp[ByteNum++] = KWP2K_DATA;
		EcuComRxBufTemp[ByteNum++] = ByteData;
		vTaskDelay(E70B_vTaskDelay_Modulus(150)); //接收的时候延迟(50ms) 1/4bit
		PassThruConfig.Counter = 0x00; //v6.61
		UartTxQueuePack(EcuComRxBufTemp);
		return TRUE;
    }
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = KWP2K_DATA;
    if((PassThruConfig.Protocol & 0x008000) == 0x008000)
    {
        DlyInByteCounter = ((uint8)((PassThruConfig.Protocol&0xff7fff)>>8));
		DlyInByteCounter|=0x80;
		//if(DlyInByteCounter%10)DlyInByteCounter=(DlyInByteCounter/10)+1;
    }
    else
    {
        DlyInByteCounter = ((uint8)(PassThruConfig.Protocol>>8));
    }
    ByteCheck = 0x00;//检查校验位 一字节和取补
    while(ByteNum < ECURXBUFLEN)
    {
        ByteStatus = PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData);
        if(ByteStatus == BYTE_FALSE)
        {
            return FALSE;
        }
        else if(ByteStatus == EOF_FLAG)
        {
            break;
        }
        else if(ByteStatus == BYTE_OK)
        {
            SetEndmsCounter();
            EcuComRxBuf[ByteNum++] = ByteData;
            ByteCheck += ByteData;
        }
        else
        {
            return FALSE;
        }
    }
    if(ByteNum == 0x03) return FALSE;//没有收到数据 过滤杂波
    if(ByteNum < 0x08) return FALSE;//数据长度不正确 过滤杂波
    if(ByteCheck > 0x00) return FALSE;//校验位错误 过滤杂波
    zyIrqDisable();
    ByteNum -= 2;
    EcuComRxBuf[0] = ByteNum >> 8;
    EcuComRxBuf[1] = ByteNum & 0xff;
    if((EcuComRxBuf[3] == 0x80) &&( EcuComRxBuf[4] == 0xfe )&& (EcuComRxBuf[5] == 0xac))
    {
         UartTxQueuePack(EcuComRxBuf);
    }
    //模拟测试的时候返回AC开头数据
    if(PassThruConfig.Listen == SIMULATION && EcuComRxBuf[3] == 0xac && EcuComRxBuf[4] == 0xfe && EcuComRxBuf[5] == 0x80)
    {
         UartTxQueuePack(EcuComRxBuf);
    }
    zyIrqEnable();
    return TRUE;
}

/*********************************************************************************************
* 名称    ： PassThru_KWP_RxPack_Interception
* 功能    ： 侦听数据
* 入口参数：
*
*
* 出口参数：
* 说明    ：一次接收一帧数据
*
***********************************************************************************************/
uint8 PassThru_KWP_RxPack_Interception()
{
    uint8  ByteData;
    uint32 DlyInByteCounter;

   if(Get_Bus_status()==FALSE)
		 return FALSE;

    if(LineInitFlag == START_DETECT)
    {
        SetSoftUart(5);
        DlyInByteCounter =   10;
        if(BYTE_OK == PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData))
        {
            ComCtl.RxSoftBaudCnter = DetectBaud(); //v6.01 lzs
            ComCtl.TxBaudCnter=ComCtl.RxSoftBaudCnter;
            if(ComCtl.RxSoftBaudCnter == 0)
            {
                LineInitFlag = DETECT_FALSE;
            }
            else
            {
                LineInitFlag = DETECT_OK;
            }
            ComCtl.ParityEnFlag = NORMAL_RX; //恢复到正常接收状态
            PassThruConfig.CurrBaud = Fpclk / ComCtl.RxSoftBaudCnter;

            UartCtlReg.Baud=PassThruConfig.CurrBaud;
            //quwenqi MCU_UART_Init(UartCtlReg);
            EcuComRxBuf[0] = 0;
            EcuComRxBuf[1] = 7;
            EcuComRxBuf[2] = KWP2K_DATA;
            EcuComRxBuf[3] = (PassThruConfig.CurrBaud >> 24) & 0xff;
            EcuComRxBuf[4] = (PassThruConfig.CurrBaud >> 16) & 0xff;
            EcuComRxBuf[5] = (PassThruConfig.CurrBaud >> 8) & 0xff;
            EcuComRxBuf[6] = PassThruConfig.CurrBaud & 0xff;
            EcuComRxBuf[7] = ByteData;
            EcuComRxBuf[8] = 0x55;
             UartTxQueuePack(EcuComRxBuf);
        }
        else
        {
            return FALSE;
        }
    }
    if(LineInitFlag == DETECT_OK)
    {
			PassThru_KWP_RxPack_Normal();
    }
    return TRUE;
}

/*********************************************************************************************
* 名称    ： PassThru_KWP_RxPack_DirectRead
* 功能    ： 侦听字节
* 入口参数：
*
*
* 出口参数：
* 说明    ：一次接收一个字节
*
***********************************************************************************************/
uint8 PassThru_KWP_RxPack_DirectRead()
{
    uint8  ByteData;
    uint32 DlyInByteCounter;

    if(Get_Bus_status()==FALSE)
		 return FALSE;
    DlyInByteCounter =  10;
    if(BYTE_OK == PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData))
    {
         PassThruDriver.TxByteDrv(ByteData);
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}
