#include "config.h"
 
#define uart_parity_no USART_Parity_No;
#define uart_parity_Even USART_Parity_Even;
#define uart_parity_Odd USART_Parity_Odd;
//stm32中，不可以实现自动波特率识别，关掉
/*
uint8 MCU_UART_AUTOBAUD(void)
{
	 LPC_UART1->ACR=0X03;//启动自动波特率，模式1	
	 LPC_UART1->IER=0X300;
	Int_time1(400000);//发送完数据后400ms未测到55,退出 fixby cyl 20160711 //
	while(((LPC_UART1->IIR)&0x300)==0)
	{
		if(TIME1_INIT==0)
		{
			return FALSE;
		}
	}
	LPC_UART1->ACR=0X300;
	return TRUE;
}
void MCU_UART_RETURN_BAUD( )
{
	
    LPC_UART1->LCR  |= 0x80; 
	UartCtlReg.Baud=(SysFreq/4/16)/(((LPC_UART1->DLM)<<8)+(LPC_UART1->DLL));
	LPC_UART1->LCR  &= 0x7f;
}*/
//PA2:和ecu通讯输出引脚;PA3:和ECU通讯输入引脚
uint8 MCU_UART_Init( UARTMODE set)
{
   
   GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* config USART2 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* USART2 GPIO config */
   /* Configure USART2 Tx (PA.02) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	    
  /* Configure USART2 Rx (PA.03) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	/* USART2 mode config */
	USART_InitStructure.USART_BaudRate = set.Baud;//115200;
	if(set.parity!=NO_PARITY)
		USART_InitStructure.USART_WordLength = uart_data9;//目前默认为8字节
 	else
 		USART_InitStructure.USART_WordLength = uart_data8;//奇偶校验是数据位为9位
	USART_InitStructure.USART_StopBits = uart_stopbit_1;//目前默认为一字节停止位，
	USART_InitStructure.USART_Parity = set.parity;//USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART2, &USART_InitStructure); 
  USART_Cmd(USART2, ENABLE);
	//NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
   /* GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

	// config USART1 clock 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    USART_Cmd(USART2, DISABLE);
	// USART1 GPIO config 
   // Configure USART1 Tx (PA.02) as alternate function push-pull 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);    
   Configure USART1 Rx (PA.03) as input floating 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	// USART1 mode config 
	USART_InitStructure.USART_BaudRate = 57600;//set.Baud;//115200;
	USART_InitStructure.USART_WordLength = uart_data8;//目前默认为8字节
	USART_InitStructure.USART_StopBits = uart_stopbit_1;//目前默认为一字节停止位，
	USART_InitStructure.USART_Parity = USART_Parity_No ;//set.parity;//
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure); 
  USART_Cmd(USART2, ENABLE);*/
    // Enable the USART2 Interrupt 
	//	 NVIC_EnableIRQ(USART2_IRQn);
			return 1;
}
/*********************************************************************************************************
** Function name:       uart1GetByte
** Descriptions:        UART3字节数据，使用查询方式接收
** input parameters:    无
** output parameters:   无
** Returned value:      ucRcvData:   接收到的数据
*********************************************************************************************************/
uint8_t MCU_UARTGetByte (uint32 DelayCounter,uint8 *result)
{

	uint32 DelayOneByte;
    DelayOneByte = ComCtl.RxSoftBaudCnter*11;
	Int_time(0,DelayCounter+DelayOneByte);//ms_count);
	while(TIME_INIT==1)//等超时//暂时屏蔽926
	{
	    if(USART_GetFlagStatus(USART2,USART_FLAG_RXNE) != RESET)
    {
        *result = USART_ReceiveData(USART2);//(USART1->DR);	
				return BYTE_OK;
    }
  }
	*result=0;
  return EOF_FLAG;
}

	
/*********************************************************************************************************
** Function name:       uart3SendByte
** Descriptions:        向串口发送子节数据，并等待数据发送完成，使用查询方式
** input parameters:    ucDat:   要发送的数据
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void MCU_UARTSendByte (uint8_t ucDat)
{       
// 	USART_SendData(USART2, ucDat);    
// 	while( USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET );       
// 	DelayNus(10);
//	USART_ClearFlag(USART2,USART_FLAG_RXNE);
	G_RXD_DIS;
	USART_SendData(USART2, ucDat);    
	while( USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET );  
	G_RXD_EN;
}

