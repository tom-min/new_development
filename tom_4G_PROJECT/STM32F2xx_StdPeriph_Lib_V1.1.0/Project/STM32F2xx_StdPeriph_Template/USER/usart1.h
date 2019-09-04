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
#define FLASH_ADDRESS_SERIAL  0x8003800//���к�
#define FLASH_ADDRESS_UPFLAG  0x8003C00//����У��
#define	FLASH_ADDRESS_PROGRAM 0x8004000//��ϳ���
#define FLASH_BL_ADDR         0x8019000//���32k������flash����

#define MAX_RDATA_TIME  1000   //�����ֽ����ʱ1000ms
#define MAX_DELAY_TIME  3000   //�����ȴ����ʱ3000ms



typedef unsigned char  uint8;                   /* defined for unsigned 8-bits integer variable 	�޷���8λ���ͱ���  */
typedef signed   char  int8;                    /* defined for signed 8-bits integer variable		�з���8λ���ͱ���  */
typedef unsigned short uint16;                  /* defined for unsigned 16-bits integer variable 	�޷���16λ���ͱ��� */
typedef signed   short int16;                   /* defined for signed 16-bits integer variable 		�з���16λ���ͱ��� */
typedef unsigned int   uint32;                  /* defined for unsigned 32-bits integer variable 	�޷���32λ���ͱ��� */
typedef signed   int   int32;                   /* defined for signed 32-bits integer variable 		�з���32λ���ͱ��� */
typedef float          fp32;                    /* single precision floating point variable (32bits) �����ȸ�������32λ���ȣ� */
typedef double         fp64;                    /* double precision floating point variable (64bits) ˫���ȸ�������64λ���ȣ� */



void UartTxQueuePack(uint8* EcuComRxBuf);
void USART_DMA_Send_ext(uint8 *Dat,uint16_t len);
void USART_DMA_Send_ext_1768(void);
#endif /* __USART1_H */
