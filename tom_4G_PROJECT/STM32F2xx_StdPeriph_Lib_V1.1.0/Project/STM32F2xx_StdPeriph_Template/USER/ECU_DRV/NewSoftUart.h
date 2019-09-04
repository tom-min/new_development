#ifndef __NEWSOFTUART_H__
#define __NEWSOFTUART_H__
typedef unsigned char  uint8;                   /* defined for unsigned 8-bits integer variable 	无符号8位整型变量  */
typedef signed   char  int8;                    /* defined for signed 8-bits integer variable		有符号8位整型变量  */
typedef unsigned short uint16;                  /* defined for unsigned 16-bits integer variable 	无符号16位整型变量 */
typedef signed   short int16;                   /* defined for signed 16-bits integer variable 		有符号16位整型变量 */
typedef unsigned int   uint32;                  /* defined for unsigned 32-bits integer variable 	无符号32位整型变量 */
typedef signed   int   int32;                   /* defined for signed 32-bits integer variable 		有符号32位整型变量 */
typedef float          fp32;                    /* single precision floating point variable (32bits) 单精度浮点数（32位长度） */
typedef double         fp64;                    /* double precision floating point variable (64bits) 双精度浮点数（64位长度） */



#define USE_SOFT_UART   1
//设置函数
 void  SetSoftUart(uint32 SoftBaud);

//发送函数
 void SoftUartTxByte(uint8 dat);
 uint8 SoftPulse(uint8 *PulseBuf);
//接收函数
 uint8 DetectS(void);
 uint8 SoftUartRxByte(uint32 DelayCounter,uint8 * dat);
 void SkipNbit(uint8 Nbit);
 uint32 DetectBaud(void);
 void _SoftTxByte(uint8 Data,uint32 TxBaudCnter);
//uint8 GetLineLevelStatus(void);

#endif
