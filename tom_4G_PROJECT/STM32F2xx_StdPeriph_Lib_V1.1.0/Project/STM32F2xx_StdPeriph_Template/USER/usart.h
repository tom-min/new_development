#ifndef _USART_H_
#define _USART_H_


 

#define  UART_MR_PAR_ODD USART_PARITY_ODD
#define  UART_MR_PAR_NO USART_PARITY_NONE
#define  UART_MR_PAR_EVEN USART_PARITY_EVEN

#define  US_MR_MODE9 USART_CHARACTER_SIZE_9BITS

#define KWP_TX_UART USART_0

#define Usart struct usart_sync_descriptor 

//#define hal_kmp_uart_config(m_baud_rate,m_parity)  
//#define hal_kmp_uart_config2(m_baud_rate,m_size) 

#endif