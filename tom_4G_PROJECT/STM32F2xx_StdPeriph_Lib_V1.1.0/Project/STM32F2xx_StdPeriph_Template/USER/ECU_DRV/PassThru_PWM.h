#ifndef __PassThru_PWM__
#define __PassThru_PWM__

extern uint8 PassThru_PWM_InitPort(uint8 CHANEL);
extern uint8 PassThru_PWM_SetConfig(uint8 *Pack,uint8 CHANEL);
extern void PassThru_PWM_TXBYTE(uint8 Data,uint16 TIM_COUNT);
extern uint8 PassThru_PWM_READ_ANSWER_BYTE(uint16 TIM_COUNT,uint8* Data);
extern uint8 PassThru_PWM_TxPack_Normal_minivci(uint8 *Pack);
//extern static uint8 PassThru_PWM_RxPack_SingleFrame(uint8 *Pack);
extern uint8 PassThru_PWM_RxPack_MultiFrame(uint8 *Buf);
extern uint8  PassThru_PWM_TxRxProc(uint8 *TxBuf);
extern uint8 PassThru_PWM_RxPack_Normal_minivci(void);




#endif
