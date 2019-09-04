#ifndef __PassThru_CAN_H__
#define __PassThru_CAN_H__
extern void PassThru_CAN_SELFTEST(uint8 selfmode,uint8 pin,uint32 canbaud,uint8 set_res);
extern uint8 PassThru_CAN_InitPort(uint8 CHANEL);
extern uint8 PassThru_CAN_SetConfig(uint8 *Pack, uint8 CHANEL);
//extern uint8 PassThru_CAN_SetCanBaud(uint8 Ch,uint32 CanBaud);
extern uint8 PassThru_CAN_GetCanPara(void);
extern uint8 PassThru_CAN_SetBaudRate(void);
extern uint8 PassThru_CAN_SetFilter(uint32 id, uint32 mask);
extern uint8 PassThru_CAN_RxPack_Auto(void);
extern uint8 PassThru_CAN_RxPack_Auto_Ext(void);
extern uint8 PassThru_CAN_RxPack_GetID(void);
extern uint8 PassThru_CAN_RxPack_Head(void);
extern uint8 PassThru_CAN_RxPack_Normal(void);
extern uint8 PassThru_CAN_RxPack_NotID(void);
extern uint8 PassThru_CAN_RxData(uint8 *RXdata);
extern uint8 PassThru_CAN_RxPack_J1939(void);
extern uint8 PassThru_CAN_RxPack_J1939_ReadPGN(void);
extern uint8 PassThru_CAN_RxPack_J1939_ReadOnly(void);
extern uint8 PassThru_CAN_TxPack_J1939(uint8 *TXdata);
extern uint8 PassThru_CAN_TxData(uint8 *TXdata, uint8 TXLen, uint32 CanID);
extern uint8 PassThru_CAN_TxPack_NotID(uint8 *TXdata);
extern uint8 PassThru_CAN_TxPack_Auto(uint8 *TXdata);
extern uint8 PassThru_CAN_TxPack_Auto_Ext(uint8 *TXdata);
extern uint8 PassThru_CAN_TxPack(uint8 *TXdata);//82 83ºÏ²¢
extern uint8 PassThru_CAN_TxPack_GetID(uint8 *TXdata);
extern uint8 PassThru_CAN_TxPack_Head(uint8 *TXdata);
extern uint8 PassThru_CAN_TxPack_Normal(uint8 *TXdata);
extern uint8 PassThru_CAN_TxPack_FlowControl(uint8 *TXdata);
extern uint8 PassThru_CAN_RxPack_Auto_ReadOnly(void);
extern uint8 PassThru_CAN_RxPack_AutoExt_ReadOnly(void);
extern uint8 PassThru_CAN_TxPack_J1939_SendPGN(uint8 *TXdata);
extern uint8 PassThru_CAN_TxPack_J1939_SendOnly(uint8 *TXdata);
extern uint32 GetAddresses32(uint8 *buffer);
extern uint8 PassThru_CAN_TxPack_SetFilter(uint8 *TXdata);
extern uint8 PassThru_CAN_FilterID(uint32 LineID);
extern uint8 PassThru_CAN_TxPack_Auto_GM(uint8 *TXdata);
extern uint8 PassThru_CAN_TxPack_flash_read(uint8 *TXdata);
extern uint8 PassThru_CAN_TxPack_flash_read_pos(uint8 *TXdata);
extern void PassThru_CAN_TxPack_NotID_Flash(uint8 *TXdata);
uint8 PassThru_CAN_RxPack_Auto_GM(void);

extern uint8 PassThru_CAN_TxPack_Normal_Multiple(uint8 *TXdata);
extern uint8 PassThru_CAN_RxPack_Normal_Multiple();

 

int isHasData(void);
#endif
