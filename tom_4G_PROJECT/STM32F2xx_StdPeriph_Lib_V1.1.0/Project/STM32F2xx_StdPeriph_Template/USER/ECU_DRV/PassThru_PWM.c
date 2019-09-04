#include "config.h"
#include "PassThru.h"
#include "interface.h"
#include "PortAssign.h"
#include "PassThru_ISO.h"
//#include "PDU_SAE_J1850_VPW.h"
//#include "PDU_SAE_J1850_PWM.h"
#include "hardwaretest.h"
#include "user_queue.h"
#include "conf_PDU.h"
#include "ProtocolDrives.h"
#include "FPGA_Drives.h"
#include "FreeRTOS.h"
#include "task.h"

void PWM_RX0_handler(uint32_t id, uint32_t mask);
void PWM_RX1_handler(uint32_t id, uint32_t mask);
#define zyIrqDisable  __disable_irq
#define zyIrqEnable   __enable_irq

uint8_t PDU_PWM_PreReply_Data[20];	
#define cpu_irq_disable zyIrqDisable 
#define cpu_irq_enable zyIrqEnable  
uint8_t PassThru_PWM_Indication(uint8_t *RXdata, uint16_t maxlen);

uint8 PassThru_VPW_RxPack_Frame(uint8 *Pack);
 uint8_t PDU_SAE_J1850_PWM_Init_Flag = PROTOCOL_CLOSE;
 extern uint8_t PDU_SAE_J1850_VPW_Init_Flag ;
  
extern void UARTAnswer(uint8 cmd);
extern void tx_CTRL_EN_channel2_write(void);
extern void tx_PINSEL_channel2(uint8_t JA_LINE0_Pin_Num,uint8_t JB_LINE1_Pin_Num);
extern uint32_t HARDWARETEST_BUS1_CMP_V_SERVICE_FUNCTION(float Output_Volt);
uint8 PassThru_PWM_RxPack_SingleFrame_MiniVci(uint8 *Pack);
void hal_init_PWM_RX0_IRQ(void (*p_handler) (uint32_t, uint32_t));
void PassThru_PWM(uint8_t command ,uint8_t channl);
#define NULL 0
extern uint8_t EcuComRxBuf[];

extern PASSTHRU_CONFIG PassThruConfig;
extern PASSTHRU_DRIVER PassThruDriver;

void EXTILine2_Config(void)
{
  EXTI_InitTypeDef   EXTI_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;

  /* Enable GPIOA clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  /* Configure PD2 pin as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* Connect EXTI Line0 to PD2 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource2);

  /* Configure EXTI Line2 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line2;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI Line2 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Configures EXTI Line15 (connected to PG15 pin) in interrupt mode
  * @param  None
  * @retval None
  */
void EXTILine7_Config(void)
{
  EXTI_InitTypeDef   EXTI_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;

  /* Enable GPIOC clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  /* Configure PC7 pin as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* Connect EXTI Line7 to PC7 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource7);

  /* Configure EXTI Line15 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line7;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI9_5 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);
}



void EXTI2_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line2) != RESET)
  {
     PWM_RX0_handler (0,0);
	  
    /* Clear the EXTI line 0 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line2);
  }
}

/**
  * @brief  This function handles External lines 15 to 10 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI9_5_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line7) != RESET)
  {
  	 PWM_RX1_handler (0,0);
	  // printfuartdebug( "EXTI9_5_IRQHandler\r\n" );
    /* Clear the EXTI line 9 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line7);
  }
}

void EXTILine2_IRQDisable(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	/* Configure EXTI Line2 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStructure);
}

void EXTILine7_IRQDisable(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	/* Configure EXTI Line2 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line7;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStructure);
}

void EXTILine2_IRQEnable(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	/* Configure EXTI Line2 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

void EXTILine7_IRQEnEnable(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	/* Configure EXTI Line2 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line7;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

#define ENABLE_J1850_RX0_IRQ EXTILine2_IRQEnable()
#define ENABLE_J1850_RX1_IRQ EXTILine7_IRQEnEnable()

#define DISABLE_J1850_RX0_IRQ EXTILine2_IRQDisable()
#define DISABLE_J1850_RX1_IRQ EXTILine7_IRQDisable()



uint8 PassThru_PWM_RxPack_Normal_minivci(void);
uint8 PassThru_PWM_TxPack_Normal_minivci(uint8 *Pack);
void tx_MODE_EN_channel2(uint8_t TXMODE,uint8_t RXMODE,uint8_t TX_REV,uint8_t RX_REV);

#define PWM_TP1    7  //Active phase 1
#define PWM_TP2    15 //Active phase 0
#define PWM_TP3    24 //Bit time
#define PWM_TP4    48 //SOF/EOD time
#define PWM_TP5    72 //EOF time
#define PWM_TP6    96 //IFS time
#define PWM_TP7    31 //Active SOF
#define PWM_TP8    39 //Active BRK
#define PWM_TP9    120//BRK to IFS time
#define PWM_TP10   48 //SOF to Data rising edge
#define PWM_TP11   16 //Passive to Next rising edge

#define  PWM_LOGIC1_H_TIME   ((Fpclk/1000000)*(PWM_TP1))
#define  PWM_LOGIC1_L_TIME   ((Fpclk/1000000)*(PWM_TP3-PWM_TP1-3))
#define  PWM_LOGIC0_H_TIME   ((Fpclk/1000000)*(PWM_TP2))
#define  PWM_LOGIC0_L_TIME   ((Fpclk/1000000)*(PWM_TP3-PWM_TP2-3))
#define  PWM_SOF_H_TIME      ((Fpclk/1000000)*(PWM_TP7-1))
#define  PWM_SOF_L_TIME      ((Fpclk/1000000)*(PWM_TP4-PWM_TP7-1))
#define  PWM_EOD_L_TIME      ((Fpclk/1000000)*(PWM_TP4-PWM_TP3+2))//((Fpclk/1000000)*(TP7))
#define  PWM_EOF_L_TIME      ((Fpclk/1000000)*(PWM_TP5-PWM_TP1-2))
#define  PWM_IFS_L_TIME      ((Fpclk/1000000)*(PWM_TP6-PWM_TP3-10))
#define  PWM_BRK_H_TIME      ((Fpclk/1000000)*(PWM_TP8+5))
#define  PWM_SOF_TIME		 ((Fpclk/1000000)*(PWM_TP10+2))



#define  PWM_POSTIVE_CHANNEL10   10
#define  PWM_NEGATIVE_CHANNEL2   2

#define PWM_SOF_MIN      29   //29us
#define PWM_SOF_MAX      36   //36us
#define PWM_SOF_CNT_MIN  ((Fpclk/1000000)*(PWM_SOF_MIN-10))
#define PWM_SOF_CNT_MAX  ((Fpclk/1000000)*(PWM_SOF_MAX+10))

#define PWM_BIT0_1_CRITICAL  10   //11，12，13(偶有错) 逻辑1和逻辑0临界值
#define PWM_CRITICAL_CNT     ((Fpclk/1000000)*PWM_BIT0_1_CRITICAL)
#define PWM_BIT0_MAX         20  //20us
#define PWM_BIT0_CNT_MAX     ((Fpclk/1000000)*(PWM_BIT0_MAX))
#define PWM_BIT_NOISE        ((Fpclk/1000000)*4)  //正常时一直用1，现尝试用2 //4) // 1)   //

#define RX0_LOW_LEVEL      0
#define RX0_HIGH_LEVEL     RX0_CS

#define PWM_SOF_EOD			(Fpclk/1000000)*48
#define PWM_ACTIVE_SOF 		(Fpclk/1000000)*31
#define PWM_ACTIVE_SOF_HID	(Fpclk/1000000)*17
#define PWM_BITTIME 		(Fpclk/1000000)*24
#define PWM_LOGIC_1			(Fpclk/1000000)*7
#define PWM_LOGIC_1_HID	    (Fpclk/1000000)*17
#define PWM_LOGIC_0 		(Fpclk/1000000)*15
#define PWM_LOGIC_0_HID		(Fpclk/1000000)*9
#define PWM_EOF				(Fpclk/1000000)*72
#define PWM_IFS				(Fpclk/1000000)*96
#define PWM_ACTIVE_BRK		(Fpclk/1000000)*39
#define PWM_BRK_TO_IFS  	(Fpclk/1000000)*120

#define PWM_LOGIC_1_MIN	(Fpclk/1000000)*4
#define PWM_LOGIC_1_MAX	(Fpclk/1000000)*10
#define PWM_LOGIC_0_MIN	(Fpclk/1000000)*12
#define PWM_LOGIC_0_MAX	(Fpclk/1000000)*18
#define PWM_BITTIME_MIN	(Fpclk/1000000)*21
#define PWM_BITTIME_MAX	(Fpclk/1000000)*27
#define PWM_SOF_EOD_MIN	(Fpclk/1000000)*(42+5)//实测车上时间为上一上升沿时间48us,按40us时算，采出板子时间为35us,此处改成47us
#define PWM_SOF_EOD_MAX	(Fpclk/1000000)*54
#define PWM_EOF_MIN			(Fpclk/1000000)*63
#define PWM_IFS_MIN			(Fpclk/1000000)*84
#define PWM_ACTIVE_SOF_MIN (Fpclk/1000000)*27
#define PWM_ACTIVE_SOF_MAX (Fpclk/1000000)*34
#define PWM_ACTIVE_BRK_MIN (Fpclk/1000000)*35
#define PWM_ACTIVE_BRK_MAX (Fpclk/1000000)*43
#define PWM_BRK_TO_IFS_MIN (Fpclk/1000000)*105


/*********************************************************************************************
* 名称    ：
* 功能    ：
* 入口参数：
*
*
* 出口参数：
* 说明    ：
*    (1) 驱动电压为+5V
*    (2）LINE0被设置为下拉，接到2号脚，LINE1 设置为上拉，接到10号脚
*    (3) TX00=1,TX01=0,TX11=1,TX10=0
*    (4) 初始化TIMER0工作模式
*    (5) 设置比较电压2.5V
***********************************************************************************************/
//extern uint8_t hal_mcan_DisableRxInterrupt(PDU_COMMON_DATA_STRUCT CommonData);
extern PDU_COMMON_DATA_STRUCT MinivciCan;
uint8 PassThru_PWM_InitPort(uint8 CHANEL)
{
	//==================先关闭所有通道，防止设置时对总线产生干扰=====
	float   compv=0;
	GPIO_InitTypeDef  GPIO_InitStructure;
	//hal_mcan_DisableRxInterrupt(MinivciCan);
	//Reset_All_IO();

	PWM_Queue_Init(&PWM_RX0_QueueData);
	PWM_Queue_Init(&PWM_RX1_QueueData);
	Reset_Line();
	//(5)设置比较电压
	
    if(PassThruConfig.CompV==0x00)	//暂时不使用比较电压选项，使用默认2.5V
	{
		SetCompV(0x25);
	}
    else
	{     
		SetCompV(PassThruConfig.CompV);
	}
    PassThru_PWM(CAN_CHANNEL_CLOSE,CHANNL0);
    PassThru_PWM(CAN_CHANNEL_START,CHANNL0);

     //quwenqi  NVIC_DisableIRQ(CAN_IRQn);       //禁止CAN 中断	  在使用过CAN 总线后，使用其他信号线时关闭，
    //禁止clk输出
    DelayNms(10);																	//changyl 2015 7 13
	   
    //预备Header，Src，Tgt
    PassThruConfig.HeaderID = PassThruConfig.Header | PassThruConfig.HeaderAR;
    PassThruConfig.SourcesAddr = PassThruConfig.Src | PassThruConfig.SrcAR;
    PassThruConfig.TargetAddr = PassThruConfig.Tgt | PassThruConfig.TgtAR;

    //设置总线驱动程序
    switch(PassThruConfig.Listen)
    {
		//XK 20121023 合并模拟和侦听模式
		case DIAGNOSE:
		case SIMULATION:
		case INTERCEPTION:
			PassThruDriver.InitEcuFunc     = NULL;
			PassThruDriver.TxDataToEcuFunc = PassThru_PWM_TxPack_Normal_minivci;
			PassThruDriver.ReadBusFunc     = PassThru_PWM_RxPack_Normal_minivci;
			PassThruDriver.KeepComFunc     = NULL;
			PassThruDriver.FuncBak0        = NULL;
			break;
		default:
			break;
    }

    //XK 20121101 如果是侦听模式，就把引脚悬空
    if(PassThruConfig.Listen == INTERCEPTION)
    {
       //quwenqi HIGH_RES;
    }
    else
    {
        SetVcc_Drv(DRIVE_5V);
		
		#if 1
		tx_MODE_EN_channel2(tx_mode_DIFFERENTIAL,0XF0,NORMAL,NORMAL);
		tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_HIGH);//LINE 0  PULL UP
		tx_CTRL_EN_channel2(JC2_LINE0_PD,JC_LOW);//LINE 0  PULL DOWN
					
		tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_LOW);//LINE 1  PULL UP
		tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_HIGH);//LINE 1  PULL DOWN
		tx_CTRL_EN_channel2_write();
		PassThruConfig.TxMode = MODE_DIFFERENTIAL_TX_LINE0UP;
		DIFFERENTIAL_TX_LINE0UP;
		#endif
    }
#if 1
	tx_MODE_EN_channel2(0XF0,0X00,NORMAL,NORMAL);

	//tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_HIGH);//LINE 0  PULL UP
	//tx_CTRL_EN_channel2(JC2_LINE0_PD, JC_LOW);//LINE 0  PULL DOWN

	//tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_HIGH);//LINE 1  PULL UP
	//tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_LOW);//LINE 1  PULL DOWN
	tx_CTRL_EN_channel2_write();

	DIFFERENTIAL_RX_LINE0UP;
	#endif
    PassThruDriver.TxDataDelayFunc = NULL;
    //KeepComTime设置
    KeepComTime[0] = (PassThruConfig.Keep[0] & 0x7fff);
    if(KeepComTime[0] == 0) KeepComTime[0] = 10;

//     SetLine1Channel(PWM_NEGATIVE_CHANNEL2);//设置引脚放在最后
//     SetLine0Channel(PWM_POSTIVE_CHANNEL10);
	tx_PINSEL_channel2(DB26_PIN_2,DB26_PIN_10);// JDA0 -->MUX0--- LINE0--1
	OBD_EVAL_GPIOInit();

	
	GPIO_ResetBits( TX00_CTL_PORT,TX00_CTL );
	GPIO_ResetBits(TX01_CTL_PORT, TX01_CTL );
	GPIO_ResetBits(TX10_CTL_PORT, TX10_CTL );
	GPIO_ResetBits(TX11_CTL_PORT, TX11_CTL );

   
	GPIO_ResetBits( PD13_LINE0_PU_PORT,PD13_LINE0_PU_PIN );
	GPIO_SetBits   (PD15_LINE1_PU_PORT, PD15_LINE1_PU_PIN );
	GPIO_SetBits   (PD12_LINE0_PD_PORT, PD12_LINE0_PD_PIN );
	GPIO_ResetBits( PD14_LINE1_PD_PORT, PD14_LINE1_PD_PIN );


    return TRUE;
}

/*********************************************************************************************
* 名称    ：
* 功能    ：
* 入口参数：
*
*
* 出口参数：
* 说明    ：
*
***********************************************************************************************/
uint8 PassThru_PWM_SetConfig(uint8 *Pack,uint8 CHANEL)
{
    uint8 j;
    uint8 temp8, Len;
    uint8 temp0, temp1;
    uint8 *PackEnd;
    uint16 PackLen;
		
    //Reset_Line();
    PassThruConfig.Pin1 =0;//切换can vpw pwm 时，要把对应引脚置零
    PassThruConfig.Pin2=0;
    PackLen = (Pack[0] << 8) + Pack[1];
    PackEnd = Pack + PackLen + 2;
    Pack = Pack + 3;
    for(;;)
    {
        temp8 = *Pack++;
        if(Pack >=PackEnd) //return TRUE;
            break;
        switch(temp8)
        {
        case 0x01:
            temp0 = *Pack++;
            temp1 = *Pack++;
            PassThruConfig.BaudRate = (temp0 << 8) + temp1;
            break;
        case 0x02:
            PassThruConfig.Listen = *Pack++;
            break;
        case 0x03:             //T_Num
            PassThruConfig.Format = *Pack++;
            break;
        case 0x04:
            Len = *Pack++;
            PassThruConfig.SendBuf[0] = Len;
            for(j = 0; j < Len; j++)
            {
                PassThruConfig.SendBuf[j+1] = *Pack++;
            }
            break;
        case 0x05:
            temp0 = *Pack++;
            temp1 = *Pack++;
            PassThruConfig.Keep[0] = (temp0 << 8) + temp1;
            break;
        case 0x06:
            PassThruConfig.Header = *Pack++;
            break;
        case 0x07:
            PassThruConfig.HeaderAR = *Pack++;
            break;
        case 0x08:
            PassThruConfig.Src = *Pack++;
            break;
        case 0x09:
            PassThruConfig.SrcAR = *Pack++;
            break;
        case 0x0a:
            PassThruConfig.Tgt = *Pack++;
            break;
        case 0x0b:
            PassThruConfig.TgtAR = *Pack++;
            break;
        case 0x0c:
            PassThruConfig.CompV = *Pack++;
            break;
        case 0x0d:
            PassThruConfig.IFR_Time = *Pack++;
            break;
        default:
            return FALSE;
        }
    }
    return TRUE;
}
 
 
void PassThru_PWM_TXBYTE(uint8 Data,uint16 TIM_COUNT)
{
	uint8 j;
	uint16 temp;
	  
    for(j=0; j<8; j++)
    {
			
        //SET_LINE_STATUS(SET_OUT_STATUS_0);
		RESET_J1850_TX0_PORT;
		RESET_TIMER2_CH0_CV;
        if(Data&0x80)
        {
            temp=PWM_LOGIC_1;
        }
        else
        {
            temp=PWM_LOGIC_0;
        }
        while(GET_TIMER2_CH0_CV<temp);
        //SET_LINE_STATUS(SET_OUT_STATUS_1);
		SET_J1850_TX0_PORT;
		RESET_TIMER2_CH0_CV;
        if(Data&0x80)
        {
            temp=PWM_LOGIC_1_HID;
        }
        else
        {
            temp=PWM_LOGIC_0_HID;
        }
        while(GET_TIMER2_CH0_CV<temp);
        Data<<=1;
    }
}


uint8 PassThru_PWM_READ_ANSWER_BYTE(uint16 TIM_COUNT,uint8* Data)
{
    uint32 temp2;
    uint8 k;
    uint16 l,temp;
    temp=TIM_COUNT;
    l=0;
    *Data=0;
    k=0;
    while(1)
    {
			
		RESET_TIMER2_CH0_CV;
        while((GET_J1850_RX0_STATUS)==1)   //改成正线接收逻辑反过来
        {
            if(GET_TIMER2_CH0_CV>PWM_LOGIC_0_MAX)//如果太大，则表示错误
            {
                return BYTE_FALSE;
            }
        }
        temp2=GET_TIMER2_CH0_CV;
				//
        if((temp2<PWM_LOGIC_1_MIN))//||(temp2>PWM_LOGIC_0_MAX))//低电平太少，判断为杂波,或太多，
        {
            return BYTE_FALSE;
        }
        else //以下为判断是1还是0,大于PWM_LOGIC_0_MIN为0，否则为1，逻辑分析仪抓原来板子波形，低电平最多可达10.5us,
        {
            if(temp2>PWM_LOGIC_0_MIN)
            {
                l&=0xfffe;
            }
            else
            {
                l|=0x01;
            }
            l<<=1;
            k++;
        }
        while((GET_J1850_RX0_STATUS)==0)
        {
            if(GET_TIMER2_CH0_CV>PWM_SOF_EOD_MAX)//得到eod
            {
                return BYTE_OK;
            }
        }
        temp2=GET_TIMER2_CH0_CV;
        if(temp2<PWM_BITTIME_MIN)//一位时间太短，错误一个低电平一个高电平
        {
            return BYTE_FALSE;
        }
        temp+=temp2;
        if(k==7)
        {
            *Data=l;
            return BYTE_OK;
        }
    }
}

/*********************************************************************************************
* 名称    ：
* 功能    ： 发送PWM格式的数据到ECU
* 入口参数：
*
*
* 出口参数：
* 说明    ：Pack格式：长度H+长度L+CMD+数据0+数据1+。。+数据N
*
***********************************************************************************************/


uint8 PassThru_PWM_TxPack_Normal_minivci(uint8 *Pack)
{
     uint8 i=0;
     uint32 temp;
     uint8 len;
     //检测SOF
	 
 
     len = (Pack[0]<<8)+Pack[1];   //max 256
	 len-=1;
	 Pack = Pack + 3;
     if(len < 1) return false;
	 zyIrqDisable();
     Timer_PWM_VPM_init();//最大16位数
     temp=0;
     while(1)
     {
	     if((GET_J1850_RX0_STATUS)==1)
	     {
		     temp=J1850_TIMER_CV;
		     if(J1850_TIMER_CV>(0xf000))//(Fpclk>>2))//长时间处于底电平，信号线错误
		     {
			     zyIrqEnable();
			     return false;
		     }
	     }
	     if((GET_J1850_RX0_STATUS)==0)
	     {
		     if((J1850_TIMER_CV-temp)>PWM_EOF_MIN)//等一个eof周期，没数据
		     break;
	     }
     }
     //发送sof命令
     //SET_LINE_STATUS(SET_OUT_STATUS_0);
     RESET_J1850_TX0_PORT;
     RESET_J1850_TIMER_CV;
     while((J1850_TIMER_CV)<PWM_ACTIVE_SOF);
     RESET_J1850_TIMER_CV;//temp=J1850_TIMER_CV;
     //SET_LINE_STATUS(SET_OUT_STATUS_1);
     SET_J1850_TX0_PORT;
     while((J1850_TIMER_CV)<PWM_ACTIVE_SOF_HID);
     //发送数据
     //temp=J1850_TIMER_CV;

    for(i=0; i<len; i++)
    {
		 PassThru_PWM_TXBYTE(*Pack++,0);//temp);
    }
	
	//发送完成设置总线为悬空
	SETNULL_J1850_TX0_PORT;
	
	  RESET_J1850_TIMER_CV;//temp=J1850_TIMER_CV;
	  while((GET_J1850_RX0_STATUS)==0)
	  {
		  if(J1850_TIMER_CV>PWM_SOF_EOD_MAX)//eod结束后一直没数据
		  {
			  zyIrqEnable();
			  return true;
		  }
	  }
	  
    PassThru_PWM_READ_ANSWER_BYTE(J1850_TIMER_CV,&len);//wait ack
    zyIrqEnable();
    return TRUE; 
}

/*********************************************************************************************
* 名称    ：
* 功能    ：
* 入口参数：
*
*
* 出口参数：
* 说明    ：
*
***********************************************************************************************/

/*********************************************************************************************
* 名称    ：
* 功能    ： 接收多帧来自ECU的PWM格式的数据
* 入口参数：
*
*
* 出口参数：返回帧个数
* 说明    ：Pack格式：帧个数+ 帧1长度N+数据0+数据1+。。+数据N+帧2长度M+数据0+。。数据M+...
*
***********************************************************************************************/
// uint8 PassThru_PWM_RxPack_MultiFrame(uint8 *Buf)
// {

//     uint8 *BufSt;
//     uint16 FrameNum = 0;
//     uint8 temp;
//     uint8 i;

// 		
//     BufSt=Buf;
// 		Buf+=3;//BufSt = Buf++;
//     for(;;)
//     {
//         temp = PassThru_PWM_RxPack_SingleFrame(Buf);

//         switch(temp)
//         {
//         case  EOD_FLAG:

//             Buf = Buf + Buf[0] + 1;
//             FrameNum++;
//             //应答

//             for(i = 0; i < PassThruConfig.SendBuf[0]; i++);
//             //PassThru_PWM_SendByte(PassThruConfig.SendBuf[i+1]);
//             break;
//         case  IFS_FLAG:

//             //*BufSt = FrameNum;
// 						BufSt[0]=(FrameNum>>8)&0xff;
// 						BufSt[1]=FrameNum&0xff;
// 						BufSt[2]=PWM_DATA;
//             return FrameNum;
//         case  BYTE_FALSE:
//             *BufSt = 0;
//             return 0;
//         //case  NOISE_FLAG:
//         //      *BufSt = 0;
//         //      return 0;
//         default:
// 					return 0;
//             ;
//         }
//     }

//     
// }

/*********************************************************************************************
* 名称    ：
* 功能    ： 发送和接收PWM数据
* 入口参数：
*
*
* 出口参数：
* 说明    ：
*
***********************************************************************************************/
// uint8  PassThru_PWM_TxRxProc(uint8 *TxBuf)
// {
//     uint8 FrameNum;
//     uint8  temp1;



//     PassThru_PWM_TxPack_Normal((uint8 *)TxBuf);
//     //接收回应


//     //等待回
//     Int_time(1,5000);
//     while(GET_RX0_STATUS)
//     {
//         if(TIME_INIT==0) return FALSE;
//     }
// //    temp1 = PassThru_PWM_ReadAnsByte((uint8 *)&temp);
//     if(BYTE_OK != temp1) return FALSE;
//     Int_time(2,500);
//     while(GET_RX0_STATUS)
//     {
//         if(TIME_INIT==0) return FALSE;
//     }
//     FrameNum = PassThru_PWM_RxPack_MultiFrame(EcuComRxBuf);
//     if(FrameNum != 0)
//     {
//         UartTxQueuePack(EcuComRxBuf);//UartWriteMultiPack(EcuComRxBuf, PWM_DATA);原来长度为一，类型统一，现在这些全放在EcuComRxBuf里
//         return TRUE;
//     }


//     return FALSE;
// }

/*********************************************************************************************
* 名称    ：
* 功能    ： 读一帧数据（诊断模式）
* 入口参数：
*
*
* 出口参数：
* 说明    ：带IFS，帧间应答,接收完毕到帧间应答时间实测为36us
*
***********************************************************************************************/




uint8 PassThru_PWM_RxPack_Normal_minivci(void)
{
 
	return	 PassThru_PWM_Indication(0,0);
}


uint8_t PassThru_PWM_FrameReceive_MiniVci(uint8_t *channl,uint8_t *RawMessage)
{
	*channl=0;
	if(PWM_Queue_Read(&PWM_RX0_QueueData,RawMessage) == true)
	{ 
		 
		*channl = CHANNL0;
		return DEVICE_RETURN_CODE_OK;
	}
	else if(PWM_Queue_Read(&PWM_RX1_QueueData,RawMessage) == true)
	{
		 
		*channl = CHANNL1;
		return DEVICE_RETURN_CODE_OK;
	}
	else 
		return DEVICE_RETURN_CODE_NO_DATA;
}


void UartTxQueuePack(uint8* EcuComRxBuf);

//形参*RXdata和maxlen是在其他通信实体中使用的
uint8_t PassThru_PWM_Indication(uint8_t *RXdata, uint16_t maxlen)
{
	uint8_t ErrorCode;
	uint8_t pwm_data_len;
	uint8_t RawMessage[256];
	uint8_t channl;
	uint8_t buffer[256];
	uint8_t *buffer_P = buffer;  //buffer_P buffer pointer
	memset(&RawMessage, 0x00, sizeof(RawMessage));
	memset(&buffer, 0x00, sizeof(buffer));
	channl=0;
	ErrorCode = PassThru_PWM_FrameReceive_MiniVci(&channl,RawMessage);
	
	pwm_data_len = RawMessage[0];
	
	if(ErrorCode == DEVICE_RETURN_CODE_OK)
	{
		//get data len
		buffer_P[0]=00;//
		buffer_P[1]=pwm_data_len+1;//+cmd
		buffer_P[2]=PWM_DATA;//
		
		memcpy(&buffer_P[3], &RawMessage[1], pwm_data_len);		//copy data
			 
		UartTxQueuePack(buffer_P);	
		 
	}
	return ErrorCode;
}


uint8_t PassThru_PWM_Indication_NULL(uint8_t *RXdata, uint16_t maxlen)
{
	return DEVICE_RETURN_CODE_UNKNOWN;
}

void PassThru_PWM(uint8_t command ,uint8_t channl)
{
    uint8_t ErrorCode;
  
    if (command == CAN_CHANNEL_START)
    {  
	    hal_init_PWM_RX0_IRQ(0);
        PDU_Transport_Protocol_Indication_CH0 = PassThru_PWM_Indication; 
        if (channl == CHANNL0)
            ENABLE_J1850_RX0_IRQ;                                   //打开接收中断，上电默认关闭中断
        else
            ENABLE_J1850_RX1_IRQ;
        PDU_SAE_J1850_PWM_Init_Flag = PROTOCOL_START;               //与VPW互斥关系
        PDU_SAE_J1850_VPW_Init_Flag = PROTOCOL_CLOSE;
        ErrorCode = DEVICE_RETURN_CODE_OK;
    }
    else if(command == CAN_CHANNEL_CLOSE)
    {
        hal_init_PWM_RX0_IRQ(0);
        PDU_Transport_Protocol_Indication_CH0 = PassThru_PWM_Indication_NULL;
        if (channl == CHANNL0)
            DISABLE_J1850_RX0_IRQ;
        else
            DISABLE_J1850_RX1_IRQ;
        PDU_SAE_J1850_PWM_Init_Flag = PROTOCOL_CLOSE;
        ErrorCode = DEVICE_RETURN_CODE_OK;
    }
	 
}

//pwm0 vpw0 都在该中断接收
void PWM_RX0_handler(uint32_t id, uint32_t mask)
{
	 
	 
	//if (J1850_RX0_ID == id && J1850_RX0_MASK == mask)
	{
		//UBaseType_t uxSaveIRQ_Status;
		//uxSaveIRQ_Status = taskENTER_CRITICAL_FROM_ISR();				//中断级进入临界区
		cpu_irq_disable();
		  
		if (PDU_SAE_J1850_PWM_Init_Flag == PROTOCOL_START)
		{
			//pwm
			uint8_t j;
			uint8_t PWM_RecvBuf[20];
			if(PassThru_PWM_RxPack_SingleFrame_MiniVci(&PWM_RecvBuf[0]) == BYTE_OK)  //接收到数据
			{
				 
				PWM_Queue_Write(&PWM_RX0_QueueData,&PWM_RecvBuf[0]);  //写入PWM队列
			}
		} 
		else if(PDU_SAE_J1850_VPW_Init_Flag == PROTOCOL_START)
		{
			
			//vpw
			uint8_t VPW_RecvBuf[21];
			uint8_t VPW_RecvBuf_Temp[21];
		
			if(PassThru_VPW_RxPack_Frame(&VPW_RecvBuf[0]) == EOD_FLAG)			//接收到数据
			{
					 
				
				memcpy(VPW_RecvBuf_Temp,&VPW_RecvBuf[1],20);				//偏移一位 去掉data_len
				 if(PassThru_VPW_filter_calc(VPW_RecvBuf_Temp))			    //VPW ID 过滤器
				{
					 
					PWM_Queue_Write(&VPW_RX0_QueueData,&VPW_RecvBuf[0]);        //VPW写入接收队列 VPW和PWM的队列格式一致 故共用一个队列函数
				}
			}
		}
		
		cpu_irq_enable();
		//taskEXIT_CRITICAL_FROM_ISR(uxSaveIRQ_Status);					//退出级进入临界区
	}
	 
}

//pwm1 vpw1 都在该中断接收
void PWM_RX1_handler(uint32_t id, uint32_t mask)
{
	 
	//if (J1850_RX1_ID == id && J1850_RX1_MASK == mask)
	{
		//UBaseType_t uxSaveIRQ_Status;
		//uxSaveIRQ_Status = taskENTER_CRITICAL_FROM_ISR();				//中断级进入临界区
		cpu_irq_disable();
		
		if (PDU_SAE_J1850_PWM_Init_Flag == PROTOCOL_START)
		{
			//pwm
			uint8_t j;
			uint8_t PWM_RecvBuf[20];
			if(PassThru_PWM_RxPack_SingleFrame_MiniVci(&PWM_RecvBuf[0]) == BYTE_OK)  //接收到数据
			{
				PWM_Queue_Write(&PWM_RX1_QueueData,&PWM_RecvBuf[0]);  //写入PWM队列  暂时通道0 和 通道1 共用一组队列
			}
		}
		else if(PDU_SAE_J1850_VPW_Init_Flag == PROTOCOL_START)
		{
			//vpw
			uint8_t ret;
			uint8_t VPW_RecvBuf[20];
			uint8_t VPW_RecvBuf_Temp[20];
			if(PassThru_VPW_RxPack_Frame(&VPW_RecvBuf[0]) == EOD_FLAG)			//接收到数据
			{
				memcpy(VPW_RecvBuf_Temp,&VPW_RecvBuf[1],20);				//偏移一位 去掉data_len
				 if(PassThru_VPW_filter_calc(VPW_RecvBuf_Temp))			    //VPW ID 过滤器
				{
					PWM_Queue_Write(&VPW_RX1_QueueData,&VPW_RecvBuf[0]);        //VPW写入接收队列 VPW和PWM的队列格式一致 故共用一个队列函数
				}
			}	
		}
		
		cpu_irq_enable();
		//taskEXIT_CRITICAL_FROM_ISR(uxSaveIRQ_Status);					//退出级进入临界区
	}
	 
}
 

uint8_t hal_pwm_filter_calc_fun_minivci(uint8_t *Recv_data)
{
	if((Recv_data[0] | PassThruConfig.HeaderAR) == PassThruConfig.HeaderID
	&& (Recv_data[1] | PassThruConfig.SrcAR)==PassThruConfig.SourcesAddr
	&& (Recv_data[2] | PassThruConfig.TgtAR) == PassThruConfig.TargetAddr)
	{
		return TRUE;
	}
	
	else
	{ 
		 FALSE;
	}
}


/*********************************************************************************************
* 说明    ：  *Pack = len(1byte) + data[0] + ... + data[n]  //new
***********************************************************************************************/
uint8 PassThru_PWM_RxPack_SingleFrame_MiniVci(uint8 *Pack)
{
    uint16 Data=0,i=0,k=0;
    uint32 temp2;
    uint8 buf[20];    //PWM MAX massage len 12Byte!!
    //检测SOF
	#if 1  //quwenqi2g
	uint8_t i_send  ;
	uint8_t len=0;
    Timer_PWM_VPM_init();//定最大数据

    while((GET_J1850_RX0_STATUS)==0)
    {
        if(J1850_TIMER_CV>PWM_IFS_L_TIME)//等一个eof周期，没数据
        {
			 
            return IFS_FLAG;
        }
    }
    RESET_J1850_TIMER_CV;

    /**************************以下是判断eof信号***********/
    while((GET_J1850_RX0_STATUS)==1)
    {
        if(J1850_TIMER_CV>PWM_ACTIVE_SOF_MAX)//如太大，便表明是BRK信号
        {
			 
            return BRK_FLAG;
        }
    }
	temp2=J1850_TIMER_CV;
    RESET_J1850_TIMER_CV;//此处太小不判断，f6原来修正时直接加32然后要求大于19,所以太少没判断，只是判段了太大
    while((GET_J1850_RX0_STATUS)==0)//高电平，
    {
        if(J1850_TIMER_CV>PWM_SOF_EOD_MAX)//sof信号周期过长，表明可能是BRK//PWM_LOGIC1_H_TIME
        {
			temp2=J1850_TIMER_CV;
			//UARTAnswer(0x01);
			 
            return BRK_FLAG;
        }
    }//此时等到下降沿，表示sof结束
		
    /**********************SOF_FLAG判断正常，以下为数据接收***********/
    for(;;)//以下全在收数据
    {
					
		RESET_J1850_TIMER_CV;
			
        /*************以下对以10号脚上信号来看,只判断低电平********************/
        while((GET_J1850_RX0_STATUS)==1)//低电平，判断是1还是0
        {
            if(J1850_TIMER_CV>PWM_LOGIC_0_MAX)//如果太大，则表示错误
            {
				 
                return BYTE_FALSE;
            }
        }
        temp2=J1850_TIMER_CV;
				//RESET_J1850_TIMER_CV;
        if((temp2<PWM_LOGIC_1_MIN))//||(temp2>PWM_LOGIC_0_MAX))//低电平太少，判断为杂波,或太多，
        {
			 
            return BYTE_FALSE;
        }
        else //以下为判断是1还是0,大于PWM_LOGIC_0_MIN为0，否则为1，逻辑分析仪抓原来板子波形，低电平最多可达10.5us,
        {
            if(temp2>PWM_LOGIC_0_MIN)
            {
                Data&=0xfffe;
            }
            else
            {
                Data|=0x01;
            }
            if(k==7)
            {
                k=0;
                //*Pack=Data&0xff;
                //tmp2[j]=Data&0xff;
                //j++;
                buf[len]=Data&0xff;
                len++;
                Data=0;
            }
            else
            {
                Data<<=1;
                k++;
            }
        }
        //********以下只是把高电平过下，判断下是否是杂波
        while((GET_J1850_RX0_STATUS)==0)
        {
            if(J1850_TIMER_CV>PWM_SOF_EOD_MIN)//得到eod
            {
				//暂时
                ////temp=J1850_TIMER_CV;
                if(hal_pwm_filter_calc_fun_minivci(buf))   //PWM ID 过滤器
                {
					//暂时
                    ////XK 20121023 如果是诊断模式，就需要帧应答，如果是模拟或侦听模式就不需要
                    //if(PassThruConfig.Listen <INTERCEPTION)
                    //{
                        ////帧应答
											////	RESET_J1850_TIMER_CV;
                        //for(i = 0; i < PassThruConfig.SendBuf[0]; i++);
                        //PassThru_PWM_TXBYTE(PassThruConfig.SendBuf[1],J1850_TIMER_CV);
                    //}
					//if (PDU_PWM_PreReply_Start_Flag == PROTOCOL_START)  //诊断模式，需要帧应答,要预先开启预回复
					{
						if(PassThruConfig.Listen <INTERCEPTION)
						{
							for( i_send  =0;i_send<PassThruConfig.SendBuf[0];i_send++)
							PassThru_PWM_TXBYTE(PassThruConfig.SendBuf[1+i_send],J1850_TIMER_CV);  //回复预回复的数据 
						}
					}
					
                    *Pack++ = len;
                    for(k=0; k<len; k++)
                    {
                        *Pack++ = buf[k];
                    }
					//printf_usb("len ==%d ",len);
					return BYTE_OK;
                }
										
                return EOD_FLAG;
            }
        }
				//temp2=J1850_TIMER_CV-temp2;
        if(J1850_TIMER_CV<PWM_BITTIME_MIN)//一位时间太短，错误一个低电平一个高电平
        {
			//UARTAnswer(0x06);//UartTxQueuePack(EcuComRxBuf);
			 
            return BYTE_FALSE;
        }

    }
	#endif
    //return BYTE_FALSE; //不可能跳到这里。防止编译警告
}



void hal_init_PWM_RX0_IRQ(void (*p_handler) (uint32_t, uint32_t))
{
	
	DISABLE_J1850_RX0_IRQ;
	memset(PDU_PWM_PreReply_Data,0,20);  //初始化预回复数据 len + ndata
	EXTILine2_Config();
}

void hal_init_PWM_RX1_IRQ(void (*p_handler) (uint32_t, uint32_t))
{
	DISABLE_J1850_RX1_IRQ;
	memset(PDU_PWM_PreReply_Data,0,20);  //初始化预回复数据 len + ndata
	
	EXTILine7_Config();
}

 


