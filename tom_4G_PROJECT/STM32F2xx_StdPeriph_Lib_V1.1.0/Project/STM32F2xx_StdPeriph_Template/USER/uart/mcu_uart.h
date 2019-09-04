
#ifndef __UART1_H__
#define __UART1_H__

extern void MCU_UARTSendByte (uint8 dat);
extern uint8 MCU_UART_Init (UARTMODE set);
extern uint8 MCU_UARTGetByte (uint32 DelayCounter,uint8 *result);
extern uint8 MCU_UART_Data_Arrival(void);
extern void MCU_UARTClear(void);
extern void MCU_UART_RETURN_BAUD(void);
//extern uint8 UART1_AUTOBAUD(void);
#endif
/* end of uart1.h */
