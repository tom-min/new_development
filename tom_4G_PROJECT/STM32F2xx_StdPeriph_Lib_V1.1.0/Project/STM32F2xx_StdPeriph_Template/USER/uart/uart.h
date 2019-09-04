#ifndef __UART_H
#define	__UART_H
#include "config.h"
// typedef struct
// {
//   uint16_t GPIO_TX //串口发送引脚
// 	uint16_t GPIO_RX;//接收引脚
// 	GPIO_TypeDef* GPIOx;　//串口对应引脚的a b c
// 	USART_TypeDef* USARTx; //使用哪个串口
// 	DMA_Channel_TypeDef* DMAy_Channelx;//使用的dma通道
// }uart_parameter;
// ;

#define HEADER0          0xA5
#define HEADER1          0xA5
#define FrmHdr0Flg       0x00
#define FrmHdr1Flg       0x01
#define FrmLngthHFlg     0x02
#define FrmLngthLFlg     0x03
#define FrmLngthDat      0x04
#define FrmEndFlg        0x05
#define FrmPureBuf       0x06


//返回到上位机信息包定义
#define INFO_PACK_TYPE        0xFF  //信息包ID
#define CHECKSUM_ERR     0xFE  //校验和失败
#define UNKNOW_TYPE      0xFD  //不认识的类型
#define UNKNOW_CMD       0xFC  //不认识的CMD类型
#define INIT_ECU_FAIL    0xFB  //初始化ECU失败
#define INIT_PORT_FAIL   0xFA  //初始化端口失败
#define REPEAT_ININT_ECU 0xF9  //正在重复初始化
#define INIT_PORT_OK     0xF8  //端口初始化成功
#define INIT_ECU_OK      0xF7  //ECU初始化完毕
#define WAITTING         0xF6  //请等待
#define ECU_COM_STOP     0xF5  //通讯中断
#define PACK_OK          0xF4  //数据包正确
#define CAN_TX_ERR       0xF3	//CAN 发送错误

#define USARTy                   USART1
#define USARTy_GPIO              GPIOA
#define USARTy_CLK               RCC_APB2Periph_USART1
#define USARTy_GPIO_CLK          RCC_APB2Periph_GPIOA
#define USARTy_RxPin             GPIO_Pin_10
#define USARTy_TxPin             GPIO_Pin_9
#define USARTy_IRQn              USART1_IRQn
#define USARTy_IRQHandler        USART1_IRQHandler

extern void UARTWritePack(uint8 *Data, uint16 NByte,uint8 FrameType);
extern void __irq IRQ_UART0 (void);
extern void UartAnswer(uint8 cmd);
extern void UARTAnswerErr(uint8 cmd,uint8 para);
extern void UARTWriteMultiPack(uint8 *Buf,uint8 FrameType);

extern void Uart_SendByte (uint8 dat);
//队列方式发送
extern void InitTxBufToQueue(void);
extern void SendBufUseInt(uint8 dat);
extern void UartTxQueuePack(uint8 *queuebuf);
//
extern uint8 Uart_Init_Default(void);//uint16 GPIO_TX,uint16 GPIO_RX,GPIO_TypeDef* GPIOx,USART_TypeDef* USARTx);
extern void NVIC_Configuration(void);
extern void Usart_TX_DMA_Conf(void);
extern void USART_DMA_Sent(uint8 *Send_Data_Buff);
extern void Usarty_TX_DMA_NVIC_Conf(void);
extern void UartTxQueuePack(uint8 *queuebuf);
extern void UartWriteMultiPack(uint8 *Buf,uint8 FrameType);
extern void USART_DMA_Sent_ext(uint8 *Dat,uint8 len);
#endif