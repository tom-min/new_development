#ifndef __PassThru_KWP_H__
#define __PassThru_KWP_H__

typedef unsigned char  uint8;                   /* defined for unsigned 8-bits integer variable 	无符号8位整型变量  */
typedef signed   char  int8;                    /* defined for signed 8-bits integer variable		有符号8位整型变量  */
typedef unsigned short uint16;                  /* defined for unsigned 16-bits integer variable 	无符号16位整型变量 */
typedef signed   short int16;                   /* defined for signed 16-bits integer variable 		有符号16位整型变量 */
typedef unsigned int   uint32;                  /* defined for unsigned 32-bits integer variable 	无符号32位整型变量 */
typedef signed   int   int32;                   /* defined for signed 32-bits integer variable 		有符号32位整型变量 */
typedef float          fp32;                    /* single precision floating point variable (32bits) 单精度浮点数（32位长度） */
typedef double         fp64;                    /* double precision floating point variable (64bits) 双精度浮点数（64位长度） */
#define USE_SOFT_UART   1

extern uint8 PassThru_KWP_Init_Fast(uint8 *TXdata);
extern uint8 PassThru_KWP_Init_Slow(uint8 *TXdata);
extern uint8 PassThru_KWP_TxPack_TxData(uint8 *TXdata);
extern uint8 PassThru_KWP_TxPack_Normal(uint8 *TXdata);
extern uint8 PassThru_KWP_TxPack_Delay(uint8 *TXdata);
extern uint8 PassThru_KWP_TxPack_Volvo(uint8 *TXdata);
extern uint8 PassThru_KWP_RxPack_Cater(void);
extern uint8 PassThru_KWP_RxPack_Kwp2000(void);
extern uint8 PassThru_KWP_RxPack_Length(void);
extern uint8 PassThru_KWP_RxPack_Mitsub(void);
extern uint8 PassThru_KWP_RxPack_Nissan(void);
extern uint8 PassThru_KWP_RxPack_Normal(void);
extern uint8 PassThru_KWP_RxPack_Volvo(void);
extern uint8 PassThru_KWP_RxPack_J1708MID(void);
extern uint8 PassThru_KWP_RxPack_MazdaKWP(void);
extern uint8 PassThru_KWP_RxPack_DirectRead(void);
extern uint8 PassThru_KWP_RxPack_Interception(void);
extern void PassThru_KWP_delay_parse(uint8 *PackBuf);
extern void PassThru_KWP_delay_sent(void);
extern uint8 PassThru_KWP_SetConfig(uint8 *PackBuf,uint8 CHANEL);
extern uint8 PassThru_KWP_InitPort(uint8 CHANEL);
extern uint8 PassThru_KWP_RxPack_Length3(void);



#endif
