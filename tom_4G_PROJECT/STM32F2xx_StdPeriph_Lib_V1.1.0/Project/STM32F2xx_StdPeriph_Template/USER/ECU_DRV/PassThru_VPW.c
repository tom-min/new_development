#include "config.h"
#include "PassThru.h"
#include "PassThru_VPW.h"
#include "interface.h"
#include "PortAssign.h"
#include "FPGA_Drives.h"
//#include "PDU_SAE_J1850_VPW.h"
//#include "PDU_SAE_J1850_PWM.h"
#include "hardwaretest.h"
#include "user_queue.h"
#include "conf_PDU.h"
#include "ProtocolDrives.h"
#include "bluewifi.h"
extern uint8_t PDU_PWM_PreReply_Data[];
#define ENABLE_J1850_RX0_IRQ EXTILine2_IRQEnable()
#define ENABLE_J1850_RX1_IRQ EXTILine7_IRQEnEnable()

#define DISABLE_J1850_RX0_IRQ EXTILine2_IRQDisable()
#define DISABLE_J1850_RX1_IRQ EXTILine7_IRQDisable()

extern uint8 PassThru_VPW_RxPack_Frame(uint8 *Pack);
extern uint8_t PassThru_VPW1_RxPack_Frame(uint8_t *Pack);
extern uint8_t PDU_SAE_J1850_PWM_Init_Flag;
 
uint8_t (*PDU_Transport_Protocol_Indication_CH0)(uint8_t *Data, uint16_t maxlen); //通道0函数指针
uint8_t (*PDU_Transport_Protocol_Indication_CH1)(uint8_t *Data, uint16_t maxlen); //通道1函数指针

uint32_t HARDWARETEST_BUS1_CMP_V_SERVICE_FUNCTION(float Output_Volt);
uint8 PassThru_VPW1_TxPack_Normal_Minivci(uint8 *TXdata);
void hal_SET_CMP_Volt(float Volt);
void VPW_RX0_handler(uint32_t id, uint32_t mask);
void VPW_RX1_handler(uint32_t id, uint32_t mask);
uint8_t SetPin_VPW1(uint8_t pin,uint8_t pin1);
uint8 PassThru_VPW_TxPack_Normal_minivci(uint8 *TXdata);
uint8 PassThru_VPW_RxPack_Normal_minivci();
void PassThru_VPW(uint8_t command ,uint8_t channl);
void hal_init_pwm_CMP_Volt(void);
void hal_init_VPW_RX0_IRQ(void (*p_handler) (uint32_t, uint32_t));
void hal_init_VPW_RX1_IRQ(void (*p_handler) (uint32_t, uint32_t));
uint8 PassThru_VPW1_ReadByte(uint8 *Byte,uint32 t,uint16 *len);
uint8_t PassThru_VPW_Indication(uint8_t *RXdata, uint16_t maxlen);
//void tx_PINSEL_channel2(uint8_t JA_LINE0_Pin_Num,uint8_t JB_LINE1_Pin_Num);  //LINE0-->CAN_H  LINE1-->CAN_L
extern void UartTxQueuePack(uint8* EcuComRxBuf);

extern void tx_CTRL_EN_channel2_write(void);
//xtern void tx_PINSEL_channel2(uint8_t JA_LINE0_Pin_Num,uint8_t JB_LINE1_Pin_Num);
//extern void tx_CTRL_EN_channel2_write(void);
extern void tx_MODE_EN_channel2(uint8_t TXMODE,uint8_t RXMODE,uint8_t TX_REV,uint8_t RX_REV);
#define NULL 0
extern uint8_t EcuComRxBuf[];
void SetPin_VPW0(void);
extern PASSTHRU_CONFIG PassThruConfig;
extern PASSTHRU_DRIVER PassThruDriver;
uint8 PassThru_VPW_ReadByte(uint8 *Byte,uint32 t,uint16 *len);

#define GET_TIMER2_CH0_CV	 T_delay_us_get_count()
#define RESET_TIMER2_CH0_CV	 T_delay_us_clear_count()
#define GET_TIMER2_CH0_COVFS T_delay_us_get_status()

#define us2cnt(us) ((unsigned int)((unsigned long)(us) / (1000000L / (float)((unsigned long)Fpclk / 1L))))

#define WAIT_100us	us2cnt(100)		// 100us, used to count 100ms

// define J1850 VPW timing requirements in accordance with SAE J1850 standard
// all pulse width times in us
// transmitting pulse width

#define  VPW_LONG_TIME       us2cnt(128)		// Long pulse nominal time
#define  VPW_SHORT_TIME      us2cnt(64)		    // Short pulse nominal time
#define  VPW_SOF_H_TIME      us2cnt(200)		// Start Of Frame nominal time
#define  VPW_EOD_L_TIME      us2cnt(200)		// End Of Data nominal time
#define  VPW_EOF_L_TIME      us2cnt(280)		// End Of Frame nominal time
#define  VPW_BRK_H_TIME      us2cnt(300)		// Break nominal time
#define  VPW_IFS_L_TIME      us2cnt(300)		// Inter Frame Separation nominal time

//低电平逻辑判断标准
//#include <stm32f10x_md.h>
#define CNT_VPW_BIT1_L_MIN    us2cnt(96)		// minimum long in frame respond pulse time      
#define CNT_VPW_BIT1_L_MAX    us2cnt(163)		// maximum long in frame respond pulse time

#define CNT_VPW_BIT0_L_MIN    us2cnt(34)		// minimum short in frame respond pulse time
#define CNT_VPW_BIT0_L_MAX    us2cnt(96)		// maximum short in frame respond pulse time

//高电平逻辑判断标准
#define CNT_VPW_BIT1_H_MIN    us2cnt(34)		// minimum short in frame respond pulse time
#define CNT_VPW_BIT1_H_MAX    us2cnt(96)		// maximum short in frame respond pulse time

#define CNT_VPW_BIT0_H_MIN    us2cnt(96)		// minimum long in frame respond pulse time
#define CNT_VPW_BIT0_H_MAX    us2cnt(163)		// maximum long in frame respond pulse time

#define CNT_VPW_EOD_MIN       us2cnt(163)	// minimum end of data time

#define VPW_RX0_LOW_LEVEL      0

#define VPW_SOF_CNT_MIN  us2cnt(163)	// minimum start of frame time
#define VPW_SOF_CNT_MAX  us2cnt(239)	// maximum start of frame time


#define zyIrqDisable  __disable_irq
#define zyIrqEnable   __enable_irq

#define cpu_irq_disable zyIrqDisable 
#define cpu_irq_enable zyIrqEnable  

#define UBaseType_t uint32
 
uint8_t PDU_SAE_J1850_VPW_Init_Flag = PROTOCOL_CLOSE;
/*********************************************************************************************
* 名称    ：
* 功能    ：
* 入口参数：
*
*
* 出口参数：
* 说明    ： (1) 驱动电压为+5V
*            (2) LINE0被设置为下拉，接到2号脚，LINE1 设置为上拉，接到10号脚
*            (3) TX00=1,TX01=0,TX11=1,TX10=0
*            (4) 初始化TIMER0工作模式
*            (5) 设置比较电压2.5V
*
***********************************************************************************************/
//extern uint8_t hal_mcan_DisableRxInterrupt(PDU_COMMON_DATA_STRUCT CommonData);





extern PDU_COMMON_DATA_STRUCT MinivciCan;
uint8 PassThru_VPW_InitPort(uint8 CHANEL)
{
	//==================先关闭所有通道，防止设置时对总线产生干扰=====
	//Reset_All_IO();
	//(1)
	float   compv=0;
	//hal_mcan_DisableRxInterrupt(MinivciCan);
	Reset_Line();
	//InitTxBufToQueue();
	
	Reset_Fun();

	//memset((uint8 *)&PassThruDriver, 0, sizeof(PASSTHRU_DRIVER));
	SetVcc_Drv(DRIVE_8V);
	
	PWM_Queue_Init(&VPW_RX0_QueueData);
	PWM_Queue_Init(&VPW_RX1_QueueData);
	
    //quwenqi   NVIC_DisableIRQ(CAN_IRQn);       //禁止CAN 中断	  在使用过CAN 总线后，使用其他信号线时关闭，
    //(5)设置比较电压
    if(PassThruConfig.CompV == 0x00)
	{
         SetCompV(0x25);
	}
    else
	{
		SetCompV(PassThruConfig.CompV);  //s
	}
  
		//	LINE0_TX_VPW;
		ONLY_LINE0_TX_REVERSE;
		tx_MODE_EN_channel2(tx_mode_LINE0,0XF0,REVERSE,NORMAL);	
		tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_LOW);//LINE 0  PULL UP
		tx_CTRL_EN_channel2(JC2_LINE0_PD,JC_HIGH);//LINE 0  PULL DOWN
 			
		tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_LOW);//LINE 1  PULL UP
		tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_LOW);//LINE 1  PULL DOWN
		tx_CTRL_EN_channel2_write();
			 
		ONLY_LINE0_RX_REVERSE;	 
		tx_MODE_EN_channel2(0XF0,0,NORMAL,REVERSE);
		printf_usb2("ONLY_LINE0_RX_REVERSE");
		PassThruConfig.RxMode=MODE_ONLY_LINE0_RX_REVERSE;
			 
			 
       DelayNms(10);											//将设置通道放在其它引脚已设置好的情况下
    if(PassThruConfig.Pin1 == 0x00)
    {
		PassThru_VPW(CAN_CHANNEL_CLOSE,CHANNL0);
		PassThru_VPW(CAN_CHANNEL_START,CHANNL0);
		SetPin_VPW0(); 
    }
    else
    {
		 
         PassThru_VPW(CAN_CHANNEL_CLOSE,CHANNL1);
         PassThru_VPW(CAN_CHANNEL_START,CHANNL1);

		 SetPin_VPW1((((PassThruConfig.Pin1>>4)&0x0f)|(PassThruConfig.Pin1&0x0f)),DB26_PIN_NC);
    }
    
    //预备Header，Src，Tgt
    PassThruConfig.HeaderID = PassThruConfig.Header | PassThruConfig.HeaderAR;
    PassThruConfig.SourcesAddr = PassThruConfig.Src | PassThruConfig.SrcAR;
    PassThruConfig.TargetAddr = PassThruConfig.Tgt | PassThruConfig.TgtAR;

    //设置总线驱动程序
    switch(PassThruConfig.Listen)
    {
    case DIAGNOSE:
    case SIMULATION:
    case INTERCEPTION:
        PassThruDriver.InitEcuFunc     = NULL;


        if(PassThruConfig.Pin1 == 0x00)
        {
            PassThruDriver.TxDataToEcuFunc = PassThru_VPW_TxPack_Normal_minivci;
            PassThruDriver.ReadBusFunc     = PassThru_VPW_RxPack_Normal_minivci;//channel 0
        }
        else
        {
            PassThruDriver.TxDataToEcuFunc = PassThru_VPW1_TxPack_Normal_Minivci; 
            PassThruDriver.ReadBusFunc     = PassThru_VPW1_RxPack_Normal_minivci;//channel 1
        }

        
        PassThruDriver.KeepComFunc     = NULL;
        PassThruDriver.FuncBak0        = NULL;
        break;
    default:
        break;
    }
    PassThruDriver.TxDataDelayFunc = NULL;
    //KeepComTime设置
    KeepComTime[0] = (PassThruConfig.Keep[0]& 0x7fff);
    if(KeepComTime[0] == 0) KeepComTime[0] = 10;
	
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



void SetPin_VPW0(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;
	tx_PINSEL_channel2(DB26_PIN_2,DB26_PIN_10);  //用户配置DB26脚位

	OBD_EVAL_GPIOInit();
	GPIO_ResetBits( TX00_CTL_PORT,TX00_CTL );
	GPIO_ResetBits(TX01_CTL_PORT, TX01_CTL );
	GPIO_ResetBits(TX10_CTL_PORT, TX10_CTL );
	GPIO_ResetBits(TX11_CTL_PORT, TX11_CTL );


	GPIO_ResetBits( PD13_LINE0_PU_PORT,PD13_LINE0_PU_PIN );
	GPIO_ResetBits   (PD15_LINE1_PU_PORT, PD15_LINE1_PU_PIN );
	GPIO_SetBits   (PD12_LINE0_PD_PORT, PD12_LINE0_PD_PIN );
	GPIO_ResetBits( PD14_LINE1_PD_PORT, PD14_LINE1_PD_PIN );



}


uint8_t SetPin_VPW1(uint8_t pin,uint8_t pin1)
{
	////不属于J1850协议的总线类型 返回错误  暂时不做该功能
	//if ((PDU_LineType.line0_type != PROTOCOL_LINE_PLUS) || (PDU_LineType.line0_type != PROTOCOL_LINE_MINUS)\
	//||(PDU_LineType.line1_type != PROTOCOL_LINE_PLUS) || (PDU_LineType.line1_type != PROTOCOL_LINE_MINUS))
	//{
	//return false;
	//}
	
	//Set Pin 最好初始化对应协议的硬件IO 防止IO在其它协议没切过来 重要

	OBD_EVAL_GPIOInit();//MUST HAVE 

	hal_SET_CMP_Volt(0x40);  
//	tx_PINSEL_channel(pin,pin1);  //用户配置DB26脚位

}

uint8 PassThru_VPW_SetConfig(uint8 *Pack,uint8 CHANEL)
{
    uint8 j;
    uint8 temp8, Len;
    uint8 temp0, temp1;
    uint16 PackLen;
    uint8 *PackEnd;
	
    //Reset_Line();
    PassThruConfig.Pin1 =0;//切换can vpw pwm 时，要把对应引脚置零
    PassThruConfig.Pin2=0;
    PackLen = (Pack[0] << 8) + Pack[1];
    PackEnd = Pack + PackLen + 2;
    Pack = Pack + 3;
    for(;;)
    {
        temp8 = *Pack++;
        if(Pack >= PackEnd)
            break;
        //return TRUE;
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
        case 0x03:
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
        case 0x0e:
            PassThruConfig.CompV = *Pack++;
            break;
        case 0x0f:
            PassThruConfig.Pin1 = *Pack++;
            break;
        default:
            return FALSE;
        }

    }
    return TRUE;
}

/*********************************************************************************************
* 名称    ：
* 功能    ： 产生VPW逻辑EOD电平
* 入口参数：
*
*
* 出口参数：
* 说明    ：
*
***********************************************************************************************/
void PassThru_VPW_EOD(void)
{

     //quwenqi  SET_LINE_STATUS(SET_OUT_STATUS_1);//反转高电平输出0
     //quwenqi  Int_time(0,VPW_EOD_L_TIME);
     //quwenqi  while (TIME_INIT==1);
}


/*********************************************************************************************
* 名称    ：
* 功能    ：
* 入口参数：
*
*
* 出口参数：
* 说明    ：超过500us的空闲被认为有效空闲 ，超时500ms，退出发送
*
***********************************************************************************************/
uint8 PassThru_VPW_TxPack_Normal_minivci(uint8 *TXdata)
{
	uint16 i,k, Len;
	uint8 temp;
	uint32 tmp1;
	#if 1  //quwenqi2g
	zyIrqDisable(); //暂时不加进入临界区

	//检测总线是否空闲
	for(i=0; i<100; i++)
	{
		temp = 0;
		for(k = 0; k < 5; k++)
		{
			if(GET_J1850_VPW_RX0_STATUS == 0) temp++;
			hal_HW_Delay_us(50);
		}
		if(temp == 5)
		break;
	}
	if(i>100)
	{
		zyIrqEnable();
		return false;
		
	}
	
	//
	Len = (TXdata[0]<<8)+TXdata[1];
    

 
	if(Len < 1) 
	{
		zyIrqEnable();
		return false;
	}
	Len = Len - 2;
    TXdata = TXdata + 3;
	Timer_PWM_VPM_init();//Fpclk);//1秒之内数据一定发送完毕
	RESET_J1850_VPW_TX0_PORT;//反转，0输出为1
	tmp1=VPW_SOF_H_TIME;
	while(GET_TIMER2_CH0_CV<VPW_SOF_H_TIME);    //send sof
	
	//PassThru_VPW_SOF();
	for(k = 0; k<=Len; k++)
	{
		temp = *TXdata++;

		// PassThru_VPW_SendByte(temp);
		for(i=0; i<8;) //以下为发送一字节的程序
		{
			// if((k&i)==0)//第一次进入时，要等eof命令，放在此处，不放在for外，计时准确
			// while(J1850_TIMER_CV<VPW_SOF_H_TIME);//EOF时间到
			if((GET_TIMER2_CH0_CV)>=tmp1)
			{
				
				RESET_TIMER2_CH0_CV;
				i++;//偶数位时为低，奇数位为高，反相故在此加1
				
				//SET_LINE_STATUS(i&0x01);			
				if (i&0x01)
				{
					SET_J1850_VPW_TX0_PORT;
				} 
				else
				{
					RESET_J1850_VPW_TX0_PORT;
				}

				if(i&0x01)//低时
				{
					if(temp & 0x80)
					tmp1=VPW_LONG_TIME;
					else
					tmp1=VPW_SHORT_TIME;
				}
				else
				{
					if(temp & 0x80)
					tmp1=VPW_SHORT_TIME;
					else
					tmp1=VPW_LONG_TIME;
				}
				temp<<=1;
			}
		}

	}
	while(GET_TIMER2_CH0_CV<tmp1);//等最后一位完成因为最后一个字节在退出发字节命令时刚把时间附值，并没有等，所以在最后要等下
	SET_J1850_VPW_TX0_PORT;//设置成常态
	RESET_TIMER2_CH0_CV;
	//tmp1=VPW_EOF_L_TIME;//和f6同步VPW_EOD_L_TIME;//fix by cyl 20160711
	while(GET_TIMER2_CH0_CV<VPW_EOF_L_TIME);
	//PassThru_VPW_EOD();
	SETNULL_J1850_VPW_TX0_PORT;
	zyIrqEnable();
	return true;
#endif
}

/*********************************************************************************************
* 名称    ：
* 功能    ：
* 入口参数：
*
*
* 出口参数：
* 说明    ：超过500us的空闲被认为有效空闲 ，超时500ms，退出发送
*
***********************************************************************************************/
uint8 PassThru_VPW1_TxPack_Normal_Minivci(uint8 *TXdata)
{
	uint16 i,k, Len;
	uint8 temp;
	uint32 tmp1;
	#if 1  //quwenqi2g
	zyIrqDisable(); //暂时不加进入临界区

	//检测总线是否空闲
	for(i=0; i<100; i++)
	{
		temp = 0;
		for(k = 0; k < 5; k++)
		{
			if(GET_J1850_VPW_RX1_STATUS == 0) temp++;
			hal_HW_Delay_us(50);
		}
		if(temp == 5)
		break;
	}
	if(i>100)
	{
		zyIrqEnable();
		return false;
	}

	//
	Len = (TXdata[0]<<8)+TXdata[1];
	if(Len < 1) 
	{
		zyIrqEnable();
		return false;
	}
	
	Len = Len - 2;
	TXdata = TXdata + 3;
	Timer_PWM_VPM_init();//Fpclk);//1秒之内数据一定发送完毕
	RESET_J1850_VPW_TX1_PORT;//反转，0输出为1
	tmp1=VPW_SOF_H_TIME;
	while(J1850_TIMER_CV<VPW_SOF_H_TIME);    //send sof
	//PassThru_VPW_SOF();
	for(k = 0; k<=Len; k++)
	{
		temp = *TXdata++;

		// PassThru_VPW_SendByte(temp);
		for(i=0; i<8;) //以下为发送一字节的程序
		{
			// if((k&i)==0)//第一次进入时，要等eof命令，放在此处，不放在for外，计时准确
			// while(J1850_TIMER_CV<VPW_SOF_H_TIME);//EOF时间到
			if((J1850_TIMER_CV)>=tmp1)
			{
				
				RESET_J1850_TIMER_CV;
				i++;//偶数位时为低，奇数位为高，反相故在此加1
				//SET_LINE_STATUS(i&0x01);
				if (i&0x01)
				{
					SET_J1850_VPW_TX1_PORT;
				}
				else
				{
					RESET_J1850_VPW_TX1_PORT;
				}

				if(i&0x01)//低时
				{
					if(temp & 0x80)
					tmp1=VPW_LONG_TIME;
					else
					tmp1=VPW_SHORT_TIME;
				}
				else
				{
					if(temp & 0x80)
					tmp1=VPW_SHORT_TIME;
					else
					tmp1=VPW_LONG_TIME;

				}
				temp<<=1;
			}
		}

	}
	while(J1850_TIMER_CV<tmp1);//等最后一位完成因为最后一个字节在退出发字节命令时刚把时间附值，并没有等，所以在最后要等下
	SET_J1850_VPW_TX1_PORT;		//设置成常态
	RESET_J1850_TIMER_CV;
	//tmp1=VPW_EOF_L_TIME;//和f6同步VPW_EOD_L_TIME;//fix by cyl 20160711
	while(J1850_TIMER_CV<VPW_EOF_L_TIME);
	//PassThru_VPW_EOD();
	zyIrqEnable();
	return true;
	#endif
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
uint8 PassThru_VPW_RxPack_Normal_minivci()
{
	return PassThru_VPW_Indication(0,0);
 
}

uint8 PassThru_VPW1_RxPack_Normal_minivci()
{
	return PassThru_VPW_Indication(0,0);
 
}

uint8_t PassThru_VPW_FrameReceive_MiniVci(uint8_t *channl,uint8_t *RawMessage)
{
	 
	if(PWM_Queue_Read(&VPW_RX0_QueueData,RawMessage) == true)
	{ 
		*channl = CHANNL0;
		return DEVICE_RETURN_CODE_OK;
	}
	else if(PWM_Queue_Read(&VPW_RX1_QueueData,RawMessage) == true)
	{
		*channl = CHANNL1;
		return DEVICE_RETURN_CODE_OK;
	}
	else 
		return DEVICE_RETURN_CODE_NO_DATA;
}

void UartTxQueuePack(uint8* EcuComRxBuf);
//形参*RXdata和maxlen是在其他通信实体中使用的
uint8_t PassThru_VPW_Indication(uint8_t *RXdata, uint16_t maxlen)
{
	uint8_t ErrorCode;
	uint8_t pwm_data_len;
	uint8_t RawMessage[256];
	uint8_t channl;
	uint8_t buffer[256];
	uint8_t *buffer_P = buffer;  //buffer_P buffer pointer
	memset(&RawMessage, 0x00, sizeof(RawMessage));
	
	ErrorCode = PassThru_VPW_FrameReceive_MiniVci(&channl,&RawMessage[0]);
	
	pwm_data_len = RawMessage[0];
	
	if(ErrorCode == DEVICE_RETURN_CODE_OK)
	{
		//get data len
		buffer_P[0]=00;//
		buffer_P[1]=pwm_data_len+1;//+cmd
		buffer_P[2]=VPW_DATA;//
		
		memcpy(&buffer_P[3], &RawMessage[1], pwm_data_len);		//copy data
			 
		UartTxQueuePack(buffer_P);	
		 
	}
	return ErrorCode;
}


uint8_t PassThru_VPW_Indication_NULL(uint8_t *RXdata, uint16_t maxlen)
{
	return DEVICE_RETURN_CODE_UNKNOWN;
}

void PassThru_VPW(uint8_t command ,uint8_t channl)
{
    uint8_t ErrorCode;
	 
    if (command == CAN_CHANNEL_START)
    {
       
		if (channl == CHANNL0)
		{
			PDU_Transport_Protocol_Indication_CH0 = PassThru_VPW_Indication; 
			 hal_init_VPW_RX0_IRQ(VPW_RX0_handler);
			 ENABLE_J1850_RX0_IRQ;                             //打开接收中断，上电默认关闭中断  
		}
		else
		{
			 PDU_Transport_Protocol_Indication_CH1 = PassThru_VPW_Indication; 
			 hal_init_VPW_RX1_IRQ(VPW_RX1_handler);
			 ENABLE_J1850_RX1_IRQ;
		}
       

        PDU_SAE_J1850_VPW_Init_Flag = PROTOCOL_START;
		PDU_SAE_J1850_PWM_Init_Flag = PROTOCOL_CLOSE;
        
        ErrorCode = DEVICE_RETURN_CODE_OK;
    }
    else if(command == CAN_CHANNEL_CLOSE)
    {
        hal_init_VPW_RX0_IRQ(VPW_RX0_handler);
		hal_init_VPW_RX1_IRQ(VPW_RX0_handler);
		
        PDU_Transport_Protocol_Indication_CH0 = PassThru_VPW_Indication_NULL;
		PDU_Transport_Protocol_Indication_CH1 = PassThru_VPW_Indication_NULL;
		 
        if (channl == CHANNL0)
            DISABLE_J1850_RX0_IRQ;
        else
            DISABLE_J1850_RX1_IRQ;
		 
		
        PDU_SAE_J1850_PWM_Init_Flag = PROTOCOL_CLOSE;
        ErrorCode = DEVICE_RETURN_CODE_OK;
    }
	 
}

uint8_t PassThru_VPW_filter_calc(uint8_t *Recv_data)
{
    //判断收到数据是否对应发送,的命令：根据地址来判断
    if(   (Recv_data[0] | PassThruConfig.HeaderAR) == PassThruConfig.HeaderID
       && (Recv_data[1] | PassThruConfig.SrcAR)==PassThruConfig.SourcesAddr 
       && (Recv_data[2] | PassThruConfig.TgtAR) == PassThruConfig.TargetAddr)
       return true;
    else
       return false;

}

void hal_init_VPW_RX0_IRQ(void (*p_handler) (uint32_t, uint32_t))
{
	#if 0  //quwenqi2g
	pmc_enable_periph_clk(J1850_RX0_ID);
	//ioport_set_pin_mode(J1850_RX0, IOPORT_MODE_PULLUP);
	pio_handler_set_pin(J1850_RX0,PIO_IT_RISE_EDGE,p_handler);   //PDU改成上升沿触发
	pio_handler_set_priority(J1850_RX0_PIOx,J1850_RX0_IRQ,3);
	NVIC_EnableIRQ((IRQn_Type)J1850_RX0_ID);
	#endif
	DISABLE_J1850_RX0_IRQ;
	//ENABLE_J1850_RX0_IRQ;    //test code
	memset(PDU_PWM_PreReply_Data,0,20);  //初始化预回复数据 len + ndata
	EXTILine2_Config();
}


void hal_init_VPW_RX1_IRQ(void (*p_handler) (uint32_t, uint32_t))
{
	#if 0  //quwenqi2g
	pmc_enable_periph_clk(J1850_RX1_ID);
	//ioport_set_pin_mode(J1850_RX1, IOPORT_MODE_PULLUP);
	pio_handler_set_pin(J1850_RX1,PIO_IT_RISE_EDGE,p_handler);   //PDU改成上升沿触发
	pio_handler_set_priority(J1850_RX1_PIOx,J1850_RX1_IRQ,3);
	NVIC_EnableIRQ((IRQn_Type)J1850_RX1_ID);
	#endif
	DISABLE_J1850_RX1_IRQ;
	//ENABLE_J1850_RX1_IRQ;    //test code
	memset(PDU_PWM_PreReply_Data,0,20);  //初始化预回复数据 len + ndata
	EXTILine7_Config();
}

void VPW_RX0_handler(uint32_t id, uint32_t mask)
{
	 #if 1  //quwenqi2g
	//if (J1850_RX0_ID == id && J1850_RX0_MASK == mask)
	{
		//UBaseType_t uxSaveIRQ_Status;
		//uxSaveIRQ_Status = taskENTER_CRITICAL_FROM_ISR();				//中断级进入临界区

		uint8_t VPW_RecvBuf[21];
		uint8_t VPW_RecvBuf_Temp[21];
		cpu_irq_disable();
		 if(PDU_SAE_J1850_VPW_Init_Flag == PROTOCOL_START)
		{
			if(PassThru_VPW_RxPack_Frame(VPW_RecvBuf) == EOD_FLAG)			//接收到数据
			{
				memcpy(VPW_RecvBuf_Temp,&VPW_RecvBuf[1],20);      //偏移一位 去掉data_len
				if(PassThru_VPW_filter_calc(VPW_RecvBuf_Temp))     //VPW ID 过滤器
				{
					PWM_Queue_Write(&VPW_RX0_QueueData,VPW_RecvBuf);         //VPW写入接收队列 VPW和PWM的队列格式一致 故共用一个队列函数
				}
			}
	    }
		cpu_irq_enable();
		//taskEXIT_CRITICAL_FROM_ISR(uxSaveIRQ_Status);			     //退出级进入临界区
	}
	#endif
}

extern uint8_t  PassThru_VPW1_RxPack_Frame(uint8 *Pack);

// vpw1 都在该中断接收
void VPW_RX1_handler(uint32_t id, uint32_t mask)
{
	
	//if (J1850_RX1_ID == id && J1850_RX1_MASK == mask)
	{
		//UBaseType_t uxSaveIRQ_Status;
		//uxSaveIRQ_Status = taskENTER_CRITICAL_FROM_ISR();				//中断级进入临界区
		cpu_irq_disable();
		
		if(PDU_SAE_J1850_VPW_Init_Flag == PROTOCOL_START)
		{
			//vpw
			uint8_t ret;
			uint8_t VPW_RecvBuf[21];
			uint8_t VPW_RecvBuf_Temp[21];
			if(PassThru_VPW1_RxPack_Frame(&VPW_RecvBuf[0]) == EOD_FLAG)			//接收到数据
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





/*********************************************************************************************
* 名称    ：
* 功能    ：接收来自ECU的VPW格式的数据
* 入口参数：
*
*
* 出口参数：
* 说明    ：Pack格式：长度H+长度L+CMD+数据0+数据1+。。+数据N
*
***********************************************************************************************/
uint8_t PassThru_VPW1_RxPack_Frame(uint8_t *Pack)
{
	uint16 len = 0;
	uint32 temp32;
	uint8 *StartPt;
	#if 1
	//uint32 t0,t1,t2;
	//检测SOF
	StartPt = Pack;
	Pack = Pack + 1;

	Timer_PWM_VPM_init();//VPW_IFS_L_TIME);//时间等长点，确认每次进来都可以收以一帧数据cyl 2016 8 31
	//	Int_time(2,1);
	//     while(GET_J1850_VPW_RX1_STATUS==0)
	//     {
	//         if(J1850_TIMER_CV>VPW_IFS_L_TIME)// GET_J1850_TIMER_COVFS == 1)
	//         {
	//             return IFS_FLAG;               //如果电平没有变化，则退出，防止死循环
	//         }
	//     }
	//		if(GET_J1850_VPW_RX1_STATUS==0)
	//			return IFS_FLAG;
	//Int_time(0,VPW_BRK_H_TIME);
	//TIM0_SET_TIM_Period(VPW_BRK_H_TIME);
	//RESET_J1850_TIMER_CV;
	while(GET_J1850_VPW_RX1_STATUS==1)
	{
		if(J1850_TIMER_CV>VPW_BRK_H_TIME)//if( GET_J1850_TIMER_COVFS == 1)
		{
			return BRK_FLAG;               //如果电平没有变化，则退出，防止死循环
		}

	}
	//取出计数器
	temp32 = J1850_TIMER_CV;
	if((temp32 < VPW_SOF_CNT_MIN) || (temp32 > VPW_SOF_CNT_MAX))//判断是否sof,不是退出
	{
		//printf("CV:%d\r\n",temp32); //test code
		///		UARTAnswerErr((temp32>>8)&0xff,temp32&0xff);
		return BYTE_FALSE;
	}
	//	TIM0_SET_TIM_Period(0xffff);
	//Int_time(0,0xffff);//定最大时间
	RESET_J1850_TIMER_CV;
	for(;;)
	{
		switch(PassThru_VPW1_ReadByte(Pack,J1850_TIMER_CV,&len))// vpw1 fail quwenqi
		{
			case EOD_FLAG:
			*StartPt++ = len;
			return EOD_FLAG;
			
			case BYTE_FALSE:
			*StartPt++ = len;
			//UARTAnswerErr(0xfd,0x04);
			return BYTE_FALSE;
			default:
			break;
		}
		len++;
	}
	#endif
	//	return EOD_FLAG;
}



uint8 PassThru_VPW_RxPack_Frame(uint8 *Pack)
{
	uint16 len = 0;
	uint32 temp32;
	uint8 *StartPt;
	#if 1
	//uint32 t0,t1,t2;
	//检测SOF
	StartPt = Pack;
	Pack = Pack + 1;

	Timer_PWM_VPM_init();//VPW_IFS_L_TIME);//时间等长点，确认每次进来都可以收以一帧数据cyl 2016 8 31
	//	Int_time(2,1);
	//     while(GET_J1850_VPW_RX0_STATUS==0)
	//     {
	//         if(J1850_TIMER_CV>VPW_IFS_L_TIME)// GET_J1850_TIMER_COVFS == 1)
	//         {
	//             return IFS_FLAG;               //如果电平没有变化，则退出，防止死循环
	//         }
	//     }
	//		if(GET_J1850_VPW_RX0_STATUS==0)
	//			return IFS_FLAG;
	//Int_time(0,VPW_BRK_H_TIME);
	//TIM0_SET_TIM_Period(VPW_BRK_H_TIME);
	//RESET_J1850_TIMER_CV;
	while(GET_J1850_VPW_RX0_STATUS==1)
	{
		if(J1850_TIMER_CV>VPW_BRK_H_TIME)//if( GET_J1850_TIMER_COVFS == 1)
		{
			return BRK_FLAG;               //如果电平没有变化，则退出，防止死循环
		}

	}
	//取出计数器
	temp32 = J1850_TIMER_CV;
	if((temp32 < VPW_SOF_CNT_MIN) || (temp32 > VPW_SOF_CNT_MAX))//判断是否sof,不是退出
	{
		//printf("CV:%d\r\n",temp32); //test code
		///		UARTAnswerErr((temp32>>8)&0xff,temp32&0xff);
		return BYTE_FALSE;
	}
	//	TIM0_SET_TIM_Period(0xffff);
	//Int_time(0,0xffff);//定最大时间
	RESET_J1850_TIMER_CV;
	for(;;)
	{
		switch(PassThru_VPW_ReadByte(Pack,J1850_TIMER_CV,&len))
		{
			case EOD_FLAG:
			*StartPt++ = len;
			return EOD_FLAG;
			
			case BYTE_FALSE:
			*StartPt++ = len;
			//UARTAnswerErr(0xfd,0x04);
			return BYTE_FALSE;
			default:
			break;
		}
		len++;
	}
	#endif
	//	return EOD_FLAG;
}
/*********************************************************************************************
* 名称    ：
* 功能    ： 读取一个字节
* 入口参数：
*
*
* 出口参数：
* 说明    ：通过识别宽度来识别1和0，计时器中间不停止，具有剔除正脉冲干扰能力
*
***********************************************************************************************/
uint8 PassThru_VPW_ReadByte(uint8 *Byte,uint32 t,uint16 *len)
{
	uint32 t1;
	uint8  BitCounter = 0;
	uint16  Data = 0;
	uint8  FirstBitFlag = 1;
	*len=0;
	if( (GET_J1850_VPW_RX0_STATUS) == 0)  //与MINIVCI不同，改成正线接收，高低电平全部翻转
	{

		while(1)//每次循环判断一个高电平和低电平，两位两位判断
		{
			//t0 = t2;
			while((GET_J1850_VPW_RX0_STATUS)==0)//此时得到高电平时间t1
			{
				if(FirstBitFlag == 1)
				{
					// temp_t = GET_TIMER2_CH0_CV;
					if(GET_TIMER2_CH0_CV >=CNT_VPW_EOD_MIN)
					{
						return EOD_FLAG;

					}
				}

				if( GET_TIMER2_CH0_COVFS == 1)  //若时钟溢出报错
				{
					return BYTE_FALSE;

				}
			}
			t1 = GET_TIMER2_CH0_CV;
			RESET_TIMER2_CH0_CV;//
			if(t1 < CNT_VPW_BIT0_L_MIN)// CNT_VPW_BIT1_L_MIN)
			{
				//UARTAnswerErr(0x01,Data);
				return BYTE_FALSE;
			}
			else if(t1 <CNT_VPW_BIT0_L_MAX) //CNT_VPW_BIT1_L_MAX)
			{
				//bit=0
				Data &= 0xfe;
				Data <<= 1;
				BitCounter++;
				//UARTAnswerErr(0x02,Data);
			}
			else if(t1 < CNT_VPW_BIT1_L_MAX)//CNT_VPW_BIT0_L_MAX)
			{
				//bit=1
				Data |= 0x01;
				Data <<= 1;
				BitCounter++;
			}
			else
			{
				//UARTAnswerErr(0x03,Data);
				return BYTE_FALSE;
			}
			FirstBitFlag = 0;
			while(GET_J1850_VPW_RX0_STATUS==1)//得到低电平时间t2
			{

				if(GET_TIMER2_CH0_COVFS == 1)
				{
					// UARTAnswerErr(0xff,0xfb);
					return BYTE_FALSE;
				}
			}
			t1 = GET_TIMER2_CH0_CV;
			RESET_TIMER2_CH0_CV;
			//判断是否为噪声
			if(t1 <CNT_VPW_BIT1_H_MIN)// CNT_VPW_BIT0_H_MIN)
			{
				//UARTAnswerErr(0xff,0xfd);
				return BYTE_FALSE;
			}
			//判断低电平是bit=1还是0
			// temp_t = t1 - t0;
			
			//判断高电平是bit=1还是0
			//temp_t = t2 - t1;
			if(t1 < CNT_VPW_BIT1_H_MAX)//CNT_VPW_BIT0_H_MAX)
			{
				//bit=1
				Data |= 0x01;
				Data <<= 1;
				BitCounter++;

			}
			else if(t1 < CNT_VPW_BIT0_H_MAX)//CNT_VPW_BIT1_H_MAX)
			{
				Data &= 0xfe;
				Data <<= 1;
				BitCounter++;
			}
			else
			{
				//UARTAnswerErr(0x04,Data);
				return BYTE_FALSE;
			}
			if(BitCounter > 7)
			{

				Data >>= 1;
				*Byte++ = Data;
				*len+=1;
				Data=0;
				BitCounter=0;
				FirstBitFlag = 1;
			}

		}

	}
	return BYTE_FALSE;
}


/*********************************************************************************************
* 名称    ：
* 功能    ： 读取一个字节
* 入口参数：
*
*
* 出口参数：
* 说明    ：通过识别宽度来识别1和0，计时器中间不停止，具有剔除正脉冲干扰能力
*
***********************************************************************************************/
uint8 PassThru_VPW1_ReadByte(uint8 *Byte,uint32 t,uint16 *len)
{
	uint32 t1;
	uint8  BitCounter = 0;
	uint16  Data = 0;
	uint8  FirstBitFlag = 1;
	*len=0;
	if( (GET_J1850_VPW_RX1_STATUS) == 0)  //与MINIVCI不同，改成正线接收，高低电平全部翻转
	{

		while(1)//每次循环判断一个高电平和低电平，两位两位判断
		{
			//t0 = t2;
			while((GET_J1850_VPW_RX1_STATUS)==0)//此时得到高电平时间t1
			{
				if(FirstBitFlag == 1)
				{
					// temp_t = GET_TIMER2_CH0_CV;
					if(GET_TIMER2_CH0_CV >=CNT_VPW_EOD_MIN)
					{
						return EOD_FLAG;

					}
				}

				if( GET_TIMER2_CH0_COVFS == 1)  //若时钟溢出报错
				{
					return BYTE_FALSE;

				}
			}
			t1 = GET_TIMER2_CH0_CV;
			RESET_TIMER2_CH0_CV;//
			if(t1 < CNT_VPW_BIT0_L_MIN)// CNT_VPW_BIT1_L_MIN)
			{
				//UARTAnswerErr(0x01,Data);
				return BYTE_FALSE;
			}
			else if(t1 <CNT_VPW_BIT0_L_MAX) //CNT_VPW_BIT1_L_MAX)
			{
				//bit=0
				Data &= 0xfe;
				Data <<= 1;
				BitCounter++;
				//UARTAnswerErr(0x02,Data);
			}
			else if(t1 < CNT_VPW_BIT1_L_MAX)//CNT_VPW_BIT0_L_MAX)
			{
				//bit=1
				Data |= 0x01;
				Data <<= 1;
				BitCounter++;
			}
			else
			{
				//UARTAnswerErr(0x03,Data);
				return BYTE_FALSE;
			}
			FirstBitFlag = 0;
			while(GET_J1850_VPW_RX1_STATUS==1)//得到低电平时间t2
			{

				if(GET_TIMER2_CH0_COVFS == 1)
				{
					// UARTAnswerErr(0xff,0xfb);
					return BYTE_FALSE;
				}
			}
			t1 = GET_TIMER2_CH0_CV;
			RESET_TIMER2_CH0_CV;
			//判断是否为噪声
			if(t1 <CNT_VPW_BIT1_H_MIN)// CNT_VPW_BIT0_H_MIN)
			{
				//UARTAnswerErr(0xff,0xfd);
				return BYTE_FALSE;
			}
			//判断低电平是bit=1还是0
			// temp_t = t1 - t0;
			
			//判断高电平是bit=1还是0
			//temp_t = t2 - t1;
			if(t1 < CNT_VPW_BIT1_H_MAX)//CNT_VPW_BIT0_H_MAX)
			{
				//bit=1
				Data |= 0x01;
				Data <<= 1;
				BitCounter++;

			}
			else if(t1 < CNT_VPW_BIT0_H_MAX)//CNT_VPW_BIT1_H_MAX)
			{
				Data &= 0xfe;
				Data <<= 1;
				BitCounter++;
			}
			else
			{
				//UARTAnswerErr(0x04,Data);
				return BYTE_FALSE;
			}
			if(BitCounter > 7)
			{

				Data >>= 1;
				*Byte++ = Data;
				*len+=1;
				Data=0;
				BitCounter=0;
				FirstBitFlag = 1;
			}

		}

	}
	return BYTE_FALSE;
}


