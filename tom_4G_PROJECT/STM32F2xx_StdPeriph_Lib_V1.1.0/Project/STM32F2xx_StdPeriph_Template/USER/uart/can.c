
/*****************************************
  CAN1 Remap
*****************************************/
#include <stdio.h>
#include <string.h>
#include "CAN.h"
#include "usart1.h"
#include "user_stm32f2xx.h" 
#include "user_stm32f2xx.h"
extern PASSTHRU_CONFIG PassThruConfig;
extern PASSTHRU_DRIVER PassThruDriver;
extern uint32 CanId_RxNum;
extern uint32 CanId_RxBuf[MAX_CANID_COUNT]; //记忆

const CANBUS_BAUDRATE CanBaudRate[]=
{
    {1000000, CAN_SJW_2tq,CAN_BS1_13tq,CAN_BS2_1tq,2 }, // 1000K //qu 93.3%
    {900000, CAN_SJW_1tq,CAN_BS1_9tq,CAN_BS2_3tq,3 },   // 900K
    {800000, CAN_SJW_1tq,CAN_BS1_5tq,CAN_BS2_3tq,5 },   // 800K
    {600000, CAN_SJW_1tq,CAN_BS1_6tq,CAN_BS2_3tq,6 },   // 600K
    {500000, CAN_SJW_1tq,CAN_BS1_13tq,CAN_BS2_1tq,4 },  // 500K //qu 93.3%
    {400000, CAN_SJW_1tq,CAN_BS1_5tq,CAN_BS2_3tq,10 },  // 400K
    {300000, CAN_SJW_1tq,CAN_BS1_3tq,CAN_BS2_2tq,20 },  // 300K
    {250000, CAN_SJW_1tq,CAN_BS1_13tq,CAN_BS2_1tq,8 },  // 250K //qu 93.3% 250 error=0%
    {200000, CAN_SJW_1tq,CAN_BS1_3tq,CAN_BS2_2tq,30 },  // 200K
    {150000, CAN_SJW_1tq,CAN_BS1_8tq,CAN_BS2_1tq,20 },  // 150K //qu
    {125000, CAN_SJW_1tq,CAN_BS1_14tq,CAN_BS2_1tq,15 },  // 125K //qu 93.8% 250 error=0%
    {100000, CAN_SJW_1tq,CAN_BS1_3tq,CAN_BS2_2tq,10 },  // 100K
    {90000, CAN_SJW_1tq,CAN_BS1_4tq,CAN_BS2_3tq,50 },   // 90K
    {96000, CAN_SJW_1tq,CAN_BS1_3tq,CAN_BS2_1tq,75 },   // 96K
    {80000, CAN_SJW_1tq,CAN_BS1_3tq,CAN_BS2_2tq,75 },   // 80K
    {60000, CAN_SJW_1tq,CAN_BS1_6tq,CAN_BS2_3tq,60 },   // 60K
    {50000, CAN_SJW_1tq,CAN_BS1_3tq,CAN_BS2_2tq,120 },  // 50K
    {40000, CAN_SJW_1tq,CAN_BS1_3tq,CAN_BS2_2tq,150 },  // 40K
    {30000, CAN_SJW_1tq,CAN_BS1_6tq,CAN_BS2_3tq,120 },  // 30K
    {33300, CAN_SJW_1tq,CAN_BS1_6tq,CAN_BS2_2tq,120 },  // 33.3K
    {20000, CAN_SJW_1tq,CAN_BS1_3tq,CAN_BS2_2tq,300 },  // 20K
    {10000, CAN_SJW_1tq,CAN_BS1_3tq,CAN_BS2_2tq,600 },  // 10K
    {5000, CAN_SJW_2tq,CAN_BS1_6tq,CAN_BS2_4tq,600 },   // 5K
    {3000, CAN_SJW_2tq,CAN_BS1_6tq,CAN_BS2_4tq,1000 },  // 3K
    {2000, CAN_SJW_2tq,CAN_BS1_10tq,CAN_BS2_6tq,1000 }, // 2K
};

/*****************************************
  CAN1 Config  CAN1 remap
  FIFO_0
  返回：
*****************************************/

 void CAN1_Config(uint8_t sjw,uint8_t bs1,uint8_t bs2,uint16_t pres)
 {
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(CANx1_GPIO_CLK, ENABLE);

	/* Connect CAN pins to AF9 */
	GPIO_PinAFConfig(CANx1_GPIO_PORT, CANx1_RX_SOURCE, CANx1_AF_PORT);
	GPIO_PinAFConfig(CANx1_GPIO_PORT, CANx1_TX_SOURCE, CANx1_AF_PORT); 

	/* Configure CAN RX and TX pins */
	GPIO_InitStructure.GPIO_Pin = CANx1_RX_PIN | CANx1_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(CANx1_GPIO_PORT, &GPIO_InitStructure);
	 

    /* CAN1 Enabling interrupt */
    NVIC_InitStructure.NVIC_IRQChannel=CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure); 



    CAN_DeInit(CAN1);
	 
    CAN_StructInit(&CAN_InitStructure);

    CAN_InitStructure.CAN_TTCM=DISABLE;
    CAN_InitStructure.CAN_ABOM=DISABLE;
    CAN_InitStructure.CAN_AWUM=DISABLE;
    CAN_InitStructure.CAN_NART=DISABLE;
    CAN_InitStructure.CAN_RFLM=DISABLE;
    CAN_InitStructure.CAN_TXFP=DISABLE;
    CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;
    //CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;
    CAN_InitStructure.CAN_SJW=sjw;
    CAN_InitStructure.CAN_BS1=bs1;
    CAN_InitStructure.CAN_BS2=bs2;
    CAN_InitStructure.CAN_Prescaler=pres;


    CAN_Init(CAN1,&CAN_InitStructure);	// CAN1

    CAN_FilterInitStructure.CAN_FilterNumber=0;
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	 // 标识符屏蔽位模式
    CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;   // 32位过滤器
    CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;			// 过滤器标识符
    CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;		// 过滤器屏蔽标识符
    CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;	 // FIFO0指向过滤器
    CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);

   
    CAN_ITConfig(CAN1,CAN_IT_FPM0_EWG,ENABLE);//CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);  // CAN1
 
 }
 
 
 void CAN2_Config(uint8_t sjw,uint8_t bs1,uint8_t bs2,uint16_t pres)
 {
	 
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/* CAN configuration ********************************************************/  
	/* Enable CAN clock */
	RCC_APB1PeriphClockCmd(CANx2_CLK, ENABLE);


	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(CANx2_GPIO_CLK, ENABLE);

	/* Connect CAN pins to AF9 */
	GPIO_PinAFConfig(CANx2_GPIO_PORT, CANx2_RX_SOURCE, CANx2_AF_PORT);
	GPIO_PinAFConfig(CANx2_GPIO_PORT, CANx2_TX_SOURCE, CANx2_AF_PORT); 

	/* Configure CAN RX and TX pins */
	GPIO_InitStructure.GPIO_Pin = CANx2_RX_PIN | CANx2_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(CANx2_GPIO_PORT, &GPIO_InitStructure);

	/* CAN configuration ********************************************************/  
	/* Enable CAN clock */
	RCC_APB1PeriphClockCmd(CANx2_CLK, ENABLE);


	CAN_DeInit(CAN2);

	CAN_StructInit(&CAN_InitStructure);

	CAN_InitStructure.CAN_TTCM=DISABLE;
	CAN_InitStructure.CAN_ABOM=DISABLE;
	CAN_InitStructure.CAN_AWUM=DISABLE;
	CAN_InitStructure.CAN_NART=DISABLE;
	CAN_InitStructure.CAN_RFLM=DISABLE;
	CAN_InitStructure.CAN_TXFP=DISABLE;
	CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;
	//CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;
	CAN_InitStructure.CAN_SJW=sjw;
	CAN_InitStructure.CAN_BS1=bs1;
	CAN_InitStructure.CAN_BS2=bs2;
	CAN_InitStructure.CAN_Prescaler=pres;



	/* CAN1 Enabling interrupt */
	NVIC_InitStructure.NVIC_IRQChannel=CAN2_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


	CAN_Init(CAN2,&CAN_InitStructure);	// CAN1

	CAN_FilterInitStructure.CAN_FilterNumber=14;
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	 // 标识符屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;   // 32位过滤器
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;			// 过滤器标识符
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;		// 过滤器屏蔽标识符
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;	 // FIFO0指向过滤器
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);


	CAN_ITConfig(CAN2,CAN_IT_FPM0_EWG,ENABLE);//CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);  // CAN2
	 
 }
 
 
void CAN_Config(uint8_t sjw,uint8_t bs1,uint8_t bs2,uint16_t pres)
{
	CAN1_Config(sjw,bs1,bs2,pres);
	CAN2_Config(sjw,bs1,bs2,pres);
	 
}


void CANx_Stop(void)
{
	PassThru_CAN_InitPort(0);
// 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, DISABLE);
// 	CAN_DeInit(CAN1);
//  GPIO_PinRemapConfig(GPIO_Remap0_CAN1,DISABLE);			 //GPIOA
//  CAN_ITConfig(CAN1,CAN_IT_FMP0,DISABLE);  // CAN1
}


void CANx_ResetBaudRate(CAN_TypeDef* CANx, uint8_t FrameFormat)
{
    u32 loop;
    u32 BaudRate;
    CANBUS_BAUDRATE CAN_BaudRate;
   
    CAN_InitTypeDef        CAN_InitStructure;
  

    /* CAN  BaudRate = RCC_APB1PeriphClock/(CAN_SJW+CAN_BS1+CAN_BS2)/CAN_Prescaler */
    CAN_DeInit(CANx);
    CAN_StructInit(&CAN_InitStructure);

    CAN_InitStructure.CAN_TTCM=DISABLE;
    CAN_InitStructure.CAN_ABOM=DISABLE;
    CAN_InitStructure.CAN_AWUM=DISABLE;
    CAN_InitStructure.CAN_NART=DISABLE;
    CAN_InitStructure.CAN_RFLM=DISABLE;
    CAN_InitStructure.CAN_TXFP=DISABLE;
	
	switch(FrameFormat&0xe0)//switch(FrameFormat&0x60)
	{
		case 0x20:
// 				CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;//回环模式
// 			break;
		case 0x40:
			CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;// /* loopback combined with silent mode */
		break;
		case 0x80:
			CAN_InitStructure.CAN_Mode=CAN_Mode_Silent;//只听模式
			break;
		default:
			CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;//正常模式
		break;
	}
	
// 		if(PassThruConfig.Format&0x80)																			//更改此位为1，会直接产生一中断，lpccan1->icr->epi为1测波特率，暂时不做
//     	regmod.Bits.LOM=1;				//只听模式
//     else
//     	regmod.Bits.LOM=0;				//发送接收模式
//     if((FrameFormat&0x20) || (FrameFormat&0x40))
//     {
//         CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;//回环模式
//     }
//     else
//     {
//         CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;//正常模式
//     }
	BaudRate = PassThruConfig.BaudRate;
	for(loop=0; loop<GET_COUNTOF(CanBaudRate); loop++)
	{
		if(CanBaudRate[loop].BaudRate == BaudRate)
		{
			CAN_BaudRate = CanBaudRate[loop];
			break;
		}
	}
	CAN_InitStructure.CAN_SJW=CAN_BaudRate.CAN_SJW;
	CAN_InitStructure.CAN_BS1=CAN_BaudRate.CAN_BS1;
	CAN_InitStructure.CAN_BS2=CAN_BaudRate.CAN_BS2;
	CAN_InitStructure.CAN_Prescaler=CAN_BaudRate.CAN_Prescaler;

	CAN_Init(CANx,&CAN_InitStructure);   // CAN2
}

void CANx_ResetFilterMask(CAN_TypeDef* CANx, uint8_t FrameFormat)
{
    uint32_t FilterID   = PassThruConfig.CanAR1;
    uint32_t FilterMask = PassThruConfig.CanMR1;
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;

    
    CAN_ITConfig(CANx,CAN_IT_FPM0_EWG,DISABLE);
	
    //if((FrameFormat&0x01) == 0x00)//单滤波
    {
        if((FrameFormat&0x10) == 0x10)
        {
            FilterID &= 0x1fffffff;
            FilterMask &= 0x1fffffff;
            FilterMask = ~FilterMask;//掩码取反 0x00000000唯一匹配 0xFFFFFFFF任意匹配
            FilterID <<= 3;
            FilterMask <<= 3;
            FilterID |= CAN_ID_EXT;
            FilterMask |= CAN_ID_EXT;
        }
        else if((FrameFormat&0x10) == 0x00)
        {
            FilterID &= 0x7ff;
            FilterMask &= 0x7ff;
            FilterMask = ~FilterMask;//掩码取反 0x00000000唯一匹配 0xFFFFFFFF任意匹配
            FilterID <<= 21;
            FilterMask <<= 21;
            FilterMask |= 0x1FFF8;//只接收标准帧
            FilterID |= CAN_ID_STD;
            FilterMask |= CAN_ID_STD;
        }
	 if(CANx==CAN1)
        CAN_FilterInitStructure.CAN_FilterNumber=0;	//
        else
	 CAN_FilterInitStructure.CAN_FilterNumber=14;	//	
	
        CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	 // 标识符屏蔽位模式
        CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;   // 32位过滤器
        CAN_FilterInitStructure.CAN_FilterIdHigh=(FilterID>>16)&0xffff;			// 过滤器标识符
        CAN_FilterInitStructure.CAN_FilterIdLow=FilterID&0xffff;
        CAN_FilterInitStructure.CAN_FilterMaskIdHigh=(FilterMask>>16)&0xffff;		// 过滤器屏蔽标识符
        CAN_FilterInitStructure.CAN_FilterMaskIdLow=FilterMask&0xffff;
        CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;	 // FIFO1指向过滤器
        CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
        CAN_FilterInit(&CAN_FilterInitStructure);
        CAN_ITConfig(CANx,CAN_IT_FPM0_EWG,ENABLE);//CAN_ITConfig(CANx,CAN_IT_FMP0,ENABLE);  // CAN1
    }
    if((FrameFormat&0x01) == 0x01)//双滤波
    {
        FilterID   = PassThruConfig.CanAR2;
        FilterMask = PassThruConfig.CanMR2;
        if((FrameFormat&0x10) == 0x10)
        {
            FilterID &= 0x1fffffff;
            FilterMask &= 0x1fffffff;
            FilterMask = ~FilterMask;//掩码取反 0x00000000唯一匹配 0xFFFFFFFF任意匹配
            FilterID <<= 3;
            FilterMask <<= 3;
            FilterID |= CAN_ID_EXT;
            FilterMask |= CAN_ID_EXT;
        }
        else if((FrameFormat&0x10) == 0x00)
        {
            FilterID &= 0x7ff;
            FilterMask &= 0x7ff;
            FilterMask = ~FilterMask;//掩码取反 0x00000000唯一匹配 0xFFFFFFFF任意匹配
            FilterID <<= 21;
            FilterMask <<= 21;
            FilterMask |= 0x1FFF8;//只接收标准帧
            FilterID |= CAN_ID_STD;
            FilterMask |= CAN_ID_STD;
        }
		
        CAN_FilterInitStructure.CAN_FilterNumber += 1;	//
        
        CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	 // 标识符屏蔽位模式
        CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;   // 32位过滤器
        CAN_FilterInitStructure.CAN_FilterIdHigh=(FilterID>>16)&0xffff;			// 过滤器标识符
        CAN_FilterInitStructure.CAN_FilterIdLow=FilterID&0xffff;
        CAN_FilterInitStructure.CAN_FilterMaskIdHigh=(FilterMask>>16)&0xffff;		// 过滤器屏蔽标识符
        CAN_FilterInitStructure.CAN_FilterMaskIdLow=FilterMask&0xffff;
        CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;	 // FIFO1指向过滤器
        CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
        CAN_FilterInit(&CAN_FilterInitStructure);
        CAN_ITConfig(CANx,CAN_IT_FPM0_EWG,ENABLE);//CAN_ITConfig(CANx,CAN_IT_FMP0,ENABLE);  // CAN1
    }
}

void CANx_MultiFilterMask(CAN_TypeDef* CANx, uint8_t FrameFormat)
{
    uint16_t i, j;
    uint32_t FilterID   = PassThruConfig.CanAR1;
    uint32_t FilterMask = PassThruConfig.CanMR1;
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;

    j = 0;
    CAN_ITConfig(CANx,CAN_IT_FPM0_EWG,DISABLE);//CAN_ITConfig(CANx,CAN_IT_FMP0,DISABLE);  // CAN1
    if((FrameFormat&0x10) == 0x10)
    {
        for(i=0; i<MAXSIZE_FTNUM; i++)
        {
            //if(j >= CanId_RxNum) break;
            FilterID = CanId_RxBuf[j++]<<3;
            FilterMask = 0x1fffffff;//不关心优先级  ~(0x1c000000<<3)
            FilterID |= CAN_ID_EXT;
            FilterMask |= CAN_ID_EXT;
            CAN_FilterInitStructure.CAN_FilterNumber=i;	//
            CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	 // 标识符列表模式
            CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;   // 32位过滤器
            CAN_FilterInitStructure.CAN_FilterIdHigh=(FilterID>>16)&0xffff;			// 过滤器标识符
            CAN_FilterInitStructure.CAN_FilterIdLow=FilterID&0xffff;
            CAN_FilterInitStructure.CAN_FilterMaskIdHigh=(FilterMask>>16)&0xffff;		// 过滤器屏蔽标识符
            CAN_FilterInitStructure.CAN_FilterMaskIdLow=FilterMask&0xffff;
            CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;	 // FIFO1指向过滤器
            CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
            CAN_FilterInit(&CAN_FilterInitStructure);
            //CAN_ITConfig(CANx,CAN_IT_FMP0,ENABLE);  // CAN1
        }
    }
    else if((FrameFormat&0x10) == 0x00)
    {
        for(i=0; i<MAXSIZE_FTNUM; i++)
        {
            //if(j >= CanId_RxNum) break;
            FilterID = CanId_RxBuf[j++]<<21;
            FilterMask = CanId_RxBuf[j++]<<21;
            FilterID |= CanId_RxBuf[j++]<<5;
            FilterMask |= CanId_RxBuf[j++]<<5;
            CAN_FilterInitStructure.CAN_FilterNumber=i;	//
            CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdList;	 // 标识符列表模式
            CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_16bit;   // 16位过滤器
            CAN_FilterInitStructure.CAN_FilterIdHigh=(FilterID>>16)&0xffff;			// 过滤器标识符
            CAN_FilterInitStructure.CAN_FilterIdLow=FilterID&0xffff;
            CAN_FilterInitStructure.CAN_FilterMaskIdHigh=(FilterMask>>16)&0xffff;		// 过滤器屏蔽标识符
            CAN_FilterInitStructure.CAN_FilterMaskIdLow=FilterMask&0xffff;
            CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;	 // FIFO1指向过滤器
            CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
            CAN_FilterInit(&CAN_FilterInitStructure);
            //CAN_ITConfig(CANx,CAN_IT_FMP0,ENABLE);  // CAN1
        }
    }
    CAN_ITConfig(CANx,CAN_IT_FPM0_EWG,ENABLE);//CAN_ITConfig(CANx,CAN_IT_FMP0,ENABLE);  // CAN1
}

void CANx_MultiFilterMask_PGN(CAN_TypeDef* CANx, uint8_t *TXdata, uint8_t maxsize)
{
	uint16_t i;
	uint8 ModelID, ModelMask;
	uint8_t temp0,temp1,temp2;
	uint32_t CanId_PGN[MAXSIZE_FTNUM];
	uint32_t FilterID   = PassThruConfig.CanAR1;
	uint32_t FilterMask = PassThruConfig.CanMR1;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;

		memset(CanId_PGN, 0x98ecff00, sizeof(CanId_PGN));
		if(maxsize > MAXSIZE_FTPGN)
		{
			UARTAnswerErr(0xf3, 0x02);
			return;
		}
		ModelID = FilterID&0xff;
		ModelMask = FilterMask&0xff;
		//eb,fe,00,ec,fe,00,da,fe,00,c5,fd,00
		for(i=0;i<maxsize;i++)
		{
			temp0 = *TXdata++;
			temp1 = *TXdata++;
			temp2 = *TXdata++;
			CanId_PGN[i] = ((0x18|temp2)<<24) + (temp1<<16) + (temp0<<8) + ModelID;
		}
    //quwenqi CAN_ITConfig(CANx,CAN_IT_FPM0_EWG,DISABLE);//CAN_ITConfig(CANx,CAN_IT_FMP0,DISABLE);  // CAN1
		//单帧
		for(i=0; i<maxsize; i++)
		{
				FilterID = CanId_PGN[i++]<<3;
				//FilterMask = 0x1ffff800;//不关心优先级和发送ID ~(0x1c0000ff<<3)
				FilterMask = ~((ModelMask|0x1c000000)<<3);
				FilterID |= CAN_ID_EXT;
				FilterMask |= CAN_ID_EXT;
				CAN_FilterInitStructure.CAN_FilterNumber=i;	//
				CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	 // 标识符列表模式
				CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;   // 32位过滤器
				CAN_FilterInitStructure.CAN_FilterIdHigh=(FilterID>>16)&0xffff;			// 过滤器标识符
				CAN_FilterInitStructure.CAN_FilterIdLow=FilterID&0xffff;
				CAN_FilterInitStructure.CAN_FilterMaskIdHigh=(FilterMask>>16)&0xffff;		// 过滤器屏蔽标识符
				CAN_FilterInitStructure.CAN_FilterMaskIdLow=FilterMask&0xffff;
				CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;	 // FIFO1指向过滤器
				CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
				CAN_FilterInit(&CAN_FilterInitStructure);
				//CAN_ITConfig(CANx,CAN_IT_FMP0,ENABLE);  // CAN1
		}
		//多帧
		CanId_PGN[i++] = 0x98e8ff00;
		CanId_PGN[i++] = 0x98eaff00;
		CanId_PGN[i++] = 0x98ebff00;
		CanId_PGN[i++] = 0x98ecff00;
		for(i=maxsize; i<maxsize+4; i++)
		{
				FilterID = CanId_PGN[i++]<<3;
				//FilterMask = 0x1ffff800;//不关心优先级和发送ID ~(0x1c00ffff<<3)
				FilterMask = ~((ModelMask|0x1c00ff00)<<3);
				FilterID |= CAN_ID_EXT;
				FilterMask |= CAN_ID_EXT;
				CAN_FilterInitStructure.CAN_FilterNumber=i;	//
				CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	 // 标识符列表模式
				CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;   // 32位过滤器
				CAN_FilterInitStructure.CAN_FilterIdHigh=(FilterID>>16)&0xffff;			// 过滤器标识符
				CAN_FilterInitStructure.CAN_FilterIdLow=FilterID&0xffff;
				CAN_FilterInitStructure.CAN_FilterMaskIdHigh=(FilterMask>>16)&0xffff;		// 过滤器屏蔽标识符
				CAN_FilterInitStructure.CAN_FilterMaskIdLow=FilterMask&0xffff;
				CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;	 // FIFO1指向过滤器
				CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
				CAN_FilterInit(&CAN_FilterInitStructure);
				//CAN_ITConfig(CANx,CAN_IT_FMP0,ENABLE);  // CAN1
		}
		
    CAN_ITConfig(CANx,CAN_IT_FPM0_EWG,ENABLE);//CAN_ITConfig(CANx,CAN_IT_FMP0,ENABLE);  // CAN1
}

/*****************************************
  CAN发送数据
*****************************************/
void CAN_SendData(CAN_TypeDef* CANx,CanTxMsg* CanData)
{
    uint8_t retrys=0;
    uint8_t mailbox=0;

    do
    {
        mailbox=CAN_Transmit(CANx,CanData);
        retrys++;
    }
    while((mailbox==CAN_NO_MB)&&(retrys<0xFE));
    retrys=0;
}

/*********************************************************************************************
* 名称    ： PassThru_CAN_TxData
* 功能    ： CANBUS协议发送数据
* 入口参数：
*
*
* 出口参数：
* 说明    ：
*
***********************************************************************************************/
uint8 CAN_TxData(uint8 *TXdata, uint8 TXLen, uint32 CanID)
{
    uint8_t num;
    uint32_t retrys=0;
    uint8_t mailbox=0;
    //uint32_t CANID;
    CanTxMsg CanData;

    uint16 j;
    uint8 loop;
    uint8 can_len;
    uint8 FillFlag, FillData;
    uint8 tempbuf[8];
	
    can_len = TXLen;
    FillFlag = PassThruConfig.CanFill>>8;
    if(FillFlag ==  1)//00默认数据 01填充数据
    {
        can_len = 8;
        FillData = PassThruConfig.CanFill&0xff;
    }
    for(j=0; j<TXLen; j++)
    {
        tempbuf[j]=*TXdata++;	//	=dat
    }
    //一帧不足8个数据的 填充其他数据
    for(loop = TXLen; loop < can_len; loop++)
    {
        tempbuf[loop] = FillData;
    }
    if((CanID&0x80000000) == 0x00)
    {
        CanData.StdId=CanID&0x7fff;
        CanData.ExtId=0x00;
        CanData.IDE=CAN_ID_STD;	 // 标准or扩展
    }
    else if((CanID&0x80000000) == 0x80000000)
    {
        CanData.StdId=0x00;
        CanData.ExtId=CanID&0x1fffffff;
        CanData.IDE=CAN_ID_EXT;	 // 标准or扩展
    }
    else
    {
        return TRUE;
    }
    CanData.RTR=CAN_RTR_DATA;  // 数据帧or远程
    CanData.DLC=can_len;
    for(num=0; num<CanData.DLC; num++)
    {
        CanData.Data[num]=tempbuf[num];
    }

    do
    {
		if( (PassThruConfig.Pin1==6)||(PassThruConfig.Pin1==14)||(PassThruConfig.Pin1==3)||(PassThruConfig.Pin1==11) )
		{
			mailbox = CAN_Transmit(CAN2,&CanData);//只用一个邮箱发送数据
			 
		}
		else
		{
		    
			mailbox=CAN_Transmit(CAN1,&CanData);//只用一个邮箱发送数据
			 
		}
        retrys++;
    }
    while((mailbox==CAN_NO_MB)&&(retrys<0xFFFFE)); 
	
	if(tempbuf[0]==0x23)
	tempbuf[0]=tempbuf[0];
	if(mailbox==CAN_NO_MB)
	return FALSE;
	
    retrys=0;
    return TRUE;
}


/*****************************

*****************************/
// static void Char2Str(char *Datout,char *Datin,unsigned char len)
// {
//     unsigned char j;

//     for(j=0; j<len; j++)
//     {
//         sprintf(Datout,",%02X",Datin[j]);
//         Datout+=3;
//     }
// }

/*****************************

  向串口发送一条CAN数据 字符串
  port    CAN端口 1：CAN1  2：CAN2
  RxMsg	  CAN数据
*****************************/
// // void Comm_Send_CANmsg_str(uint8_t port,CanRxMsg *RxMsg)
// // {
// //     char Buf[60];

// //     memset(Buf,0x00,60);  // 清空

// //     sprintf(Buf,"%d",port);
// //     USART_STR(USART1,Buf);

// //     if(RxMsg->IDE==CAN_ID_STD) // 标准帧
// //     {
// //         sprintf(Buf,",S0x%08X",RxMsg->StdId);
// //         USART_STR(USART1,Buf);

// //         sprintf(Buf,",%d",RxMsg->DLC);
// //         USART_STR(USART1,Buf);
// //         Char2Str(Buf,(char*)RxMsg->Data,8);
// //         USART_STR(USART1,Buf);
// //     }
// //     else // 扩展帧
// //     {
// //         sprintf(Buf,",E0x%08X",RxMsg->ExtId);
// //         USART_STR(USART1,Buf);

// //         sprintf(Buf,",%d",RxMsg->DLC);
// //         USART_STR(USART1,Buf);
// //         Char2Str(Buf,(char*)RxMsg->Data,8);
// //         USART_STR(USART1,Buf);
// //     }

// //     USART_STR(USART1,"\r\n");
// // }

/*****************************

  向串口发送一条CAN数据 buffer
  port    CAN端口 1：CAN1  2：CAN2
  RxMsg	  CAN数据
*****************************/
// void Comm_Send_CANmsg_buf(uint8_t port,CanRxMsg *RxMsg)
// {
//     uint8_t i, j;
//     uint8_t Buf[60];

//     memset(Buf,0x00,60);  // 清空

//     i = 0;
// //     Buf[i++] = 0xa5;
// //     Buf[i++] = 0xa5;
//     Buf[i++] = 0x00;
//     Buf[i++] = 0x00;
//     Buf[i++] = port;
//     if(RxMsg->IDE==CAN_ID_STD) // 标准帧
//     {
//         Buf[i++] = (RxMsg->StdId>>24)&0xff;
//         Buf[i++] = (RxMsg->StdId>>16)&0xff;
//         Buf[i++] = (RxMsg->StdId>>8)&0xff;
//         Buf[i++] = (RxMsg->StdId)&0xff;
//         for(j=0; j<RxMsg->DLC; j++)
//         {
//             Buf[i++] = RxMsg->Data[j];
//         }
//     }
//     else // 扩展帧
//     {
//         Buf[i++] = (RxMsg->ExtId>>24)&0xff | 0x80;
//         Buf[i++] = (RxMsg->ExtId>>16)&0xff;
//         Buf[i++] = (RxMsg->ExtId>>8)&0xff;
//         Buf[i++] = (RxMsg->ExtId)&0xff;
//         for(j=0; j<RxMsg->DLC; j++)
//         {
//             Buf[i++] = RxMsg->Data[j];
//         }
//     }
//     Buf[i++] = 0x00;
//     Buf[0x00] = (i - 5)/0x100;
//     Buf[0x01] = (i - 5)%0x100;
//     //USART_Send(USART1, Buf, i);
//     //USART1_SendByte(0x55);
//     UartTxQueuePack(Buf);
// }

