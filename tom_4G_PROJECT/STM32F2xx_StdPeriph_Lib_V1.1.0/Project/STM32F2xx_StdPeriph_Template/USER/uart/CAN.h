
#ifndef __CAN_H__
#define __CAN_H__
#include <stdint.h>
 #include "user_stm32f2xx.h"
#include "config.h"

#define EFF_FLAG 0x80000000  //用以标志这是29bitID

#define SJ1000_BPS_1000K    0x800014//1000K 0x0f4240
#define SJ1000_BPS_800K        0x800016//800K  0x0c3500
#define SJ1000_BPS_500K     0x80001c//500K  0x07a120
#define SJ1000_BPS_250K     0x80011c//250K  0x03d090
#define SJ1000_BPS_125K     0x80031c//125K  0x01e840
#define SJ1000_BPS_100K     0x80032f//100K  0x0186a0
#define SJ1000_BPS_96K      0x80033f//96K   0x017700
#define SJ1000_BPS_50K      0x80072f//50K   0x00c350
#define SJ1000_BPS_33K      0x800b2f//33K   0x0080e8
#define SJ1000_BPS_20K      0x80123f//20K   0x004e20
#define SJ1000_BPS_10K      0x80253f//10K   0x002710


#define BPS_1000K	1000000
#define BPS_800K	800000
#define BPS_500K	500000
#define BPS_250K	250000
#define BPS_125K	125000
#define BPS_100K	100000
#define BPS_96K		96000
#define BPS_50K		50000
#define BPS_33K		33300
#define BPS_20K		20000
#define BPS_10K		10000

typedef struct
{
    uint32_t BaudRate;
    uint8_t CAN_SJW;
    uint8_t CAN_BS1;
    uint8_t CAN_BS2;
    uint16_t CAN_Prescaler;
} CANBUS_BAUDRATE;




// CAN波特率  BaudRate = RCC_APB1PeriphClock/(CAN_SJW+CAN_BS1+CAN_BS2)/CAN_Prescaler
#define    SET_CAN_SJW   CAN_SJW_1tq
#define    SET_CAN_BS1   CAN_BS1_3tq	// 3
#define    SET_CAN_BS2   CAN_BS2_2tq	// 2
#define    SET_CAN_PRES  24    				// 波特率分频器 12-500K 24-250K 48-125K 


#define GET_COUNTOF(a) (sizeof(a) / sizeof(a[0]))

#define MAXSIZE_FTNUM  14 //最大过滤CANID个数
#define MAXSIZE_FTPGN  10 //最大过滤PGN个数

//void CANx_Reset(CAN_TypeDef* CANx, uint8_t* ReceiveBuff);
void CANx_ResetBaudRate(CAN_TypeDef* CANx, uint8_t FrameFormat);
void CANx_ResetFilterMask(CAN_TypeDef* CANx, uint8_t FrameFormat);
void CANx_MultiFilterMask(CAN_TypeDef* CANx, uint8_t FrameFormat);
void CANx_MultiFilterMask_PGN(CAN_TypeDef* CANx, uint8_t *TXdata, uint8_t maxsize);
void CAN2_Config(uint8_t sjw,uint8_t bs1,uint8_t bs2,uint16_t pres);
uint8 CAN_TxData(uint8 *TXdata, uint8 TXLen, uint32 CanID);
void CAN1_Config(uint8_t sjw,uint8_t bs1,uint8_t bs2,uint16_t pres);
//void CAN_SendDataFrame(CAN_TypeDef* CANx,uint8_t* ReceiveBuff);
void CANx_Stop(void);

 
#define CANx1                       CAN1
#define CANx1_CLK                    RCC_APB1Periph_CAN1
#define CANx1_RX_PIN                 GPIO_Pin_0
#define CANx1_TX_PIN                 GPIO_Pin_1
#define CANx1_GPIO_PORT              GPIOD
#define CANx1_GPIO_CLK               RCC_AHB1Periph_GPIOD
#define CANx1_AF_PORT                GPIO_AF_CAN1
#define CANx1_RX_SOURCE              GPIO_PinSource0
#define CANx1_TX_SOURCE              GPIO_PinSource1       

#define CANx2                        CAN2
#define CANx2_CLK                     (RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2)
#define CANx2_RX_PIN                 GPIO_Pin_12
#define CANx2_TX_PIN                 GPIO_Pin_13
#define CANx2_GPIO_PORT              GPIOB
#define CANx2_GPIO_CLK               RCC_AHB1Periph_GPIOB
#define CANx2_AF_PORT                GPIO_AF_CAN2
#define CANx2_RX_SOURCE              GPIO_PinSource12
#define CANx2_TX_SOURCE              GPIO_PinSource13    

#endif
