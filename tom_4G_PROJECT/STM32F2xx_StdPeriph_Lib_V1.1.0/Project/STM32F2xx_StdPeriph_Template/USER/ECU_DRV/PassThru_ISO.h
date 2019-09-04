#ifndef __PassThru_ISO_H__
#define __PassThru_ISO_H__
extern uint8 PassThru_ISO_InitPort(uint8 CHANEL);
extern uint8 PassThru_ISO_SetConfig(uint8 *PackBuf,uint8 CHANEL);
extern uint8 PassThru_ISO_Init_Slow(uint8 *TXdata);
extern uint8 PassThru_ISO_TxPack_Normal(uint8 *TXdata);
extern uint8 PassThru_ISO_TxPack_WABCO(uint8 *TXdata);
extern uint8 PassThru_ISO_RxPack_Normal(void);
extern uint8 PassThru_ISO_RxPack_WABCO(void);





#endif
