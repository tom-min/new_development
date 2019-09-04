 #ifndef __PassThru_VPW__
 #define __PassThru_VPW__

extern uint8 PassThru_VPW_TxPack_Normal_minivci(uint8 *TXdata);
extern uint8 PassThru_VPW_RxPack_Normal_minivci(void);
extern uint8 PassThru_VPW1_RxPack_Normal_minivci(void);
extern uint8 PassThru_VPW_InitPort(uint8 CHANEL);
extern uint8 PassThru_VPW_SetConfig(uint8 *Pack,uint8 CHANEL);
extern void PassThru_VPW_EOD(void);
extern uint8 PassThru_VPW_ReadByte_minivci(uint8 *Byte,uint32 t,uint16 *len);
//extern static uint8 PassThru_PWM_RxPack_SingleFrame(uint8 *Pack);
extern uint8 PassThru_VPW_RxPack_Frame_minivci(uint8 *Pack);

#endif
