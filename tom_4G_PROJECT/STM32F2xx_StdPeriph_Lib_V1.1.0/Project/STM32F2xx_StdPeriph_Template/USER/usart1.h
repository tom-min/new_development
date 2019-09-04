#ifndef __USART1_H
#define	__USART1_H

#include <stdio.h>
#include <stdint.h>

#define FRAME_ORD  0x04
#define FRAME_CANID  0x05
#define FRAME_CANDT  0x09

#define BOARD_KEY   0x4D //'M'

#define MAX_SIZE_DATA   4096
#define MAX_SIZE_FLAG   16
#define MAX_SIZE_SNUM   16
#define FLASH_ADDRESS_START   0x8000000
#define FLASH_ADDRESS_SERIAL  0x8003800//序列号
#define FLASH_ADDRESS_UPFLAG  0x8003C00//升级校验
#define	FLASH_ADDRESS_PROGRAM 0x8004000//诊断程序
#define FLASH_BL_ADDR         0x8019000//最后32k用来存flash数据

#define MAX_RDATA_TIME  1000   //接收字节最大超时1000ms
#define MAX_DELAY_TIME  3000   //升级等待最大超时3000ms



typedef unsigned char  uint8;                   /* defined for unsigned 8-bits integer variable 	无符号8位整型变量  */
typedef signed   char  int8;                    /* defined for signed 8-bits integer variable		有符号8位整型变量  */
typedef unsigned short uint16;                  /* defined for unsigned 16-bits integer variable 	无符号16位整型变量 */
typedef signed   short int16;                   /* defined for signed 16-bits integer variable 		有符号16位整型变量 */
typedef unsigned int   uint32;                  /* defined for unsigned 32-bits integer variable 	无符号32位整型变量 */
typedef signed   int   int32;                   /* defined for signed 32-bits integer variable 		有符号32位整型变量 */
typedef float          fp32;                    /* single precision floating point variable (32bits) 单精度浮点数（32位长度） */
typedef double         fp64;                    /* double precision floating point variable (64bits) 双精度浮点数（64位长度） */



void UartTxQueuePack(uint8* EcuComRxBuf);
void USART_DMA_Send_ext(uint8 *Dat,uint16_t len);
void USART_DMA_Send_ext_1768(void);
#endif /* __USART1_H */
