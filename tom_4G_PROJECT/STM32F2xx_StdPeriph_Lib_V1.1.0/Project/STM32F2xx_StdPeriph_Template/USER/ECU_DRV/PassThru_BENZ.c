//#include <asf.h>
#include "config.h"
#include "PassThru.h"
#include "hardware.h"
#include "PassThru_KWP.h"
#include "NewSoftUart.h"
#include "interface.h"
#include "user_queue.h" 

#define zyIrqDisable  __disable_irq
#define zyIrqEnable   __enable_irq

#define cpu_irq_disable zyIrqDisable 
#define cpu_irq_enable zyIrqEnable  


#define UART_MR_PAR_NO 0
#define US_MR_MODE9 0

#define NULL 0
#ifdef MINI_HW_001
 
 extern void  hal_kmp_uart_config_minivci(uint32_t baudrate,uint8_t parity);
 #endif
 #ifdef MINI_HW_002
 extern void  hal_kmp_uart_config_minivci(uint32_t baudrate,uint32_t parity);
 #endif
extern uint8_t EcuComRxBuf[];


void UartTxQueuePack(uint8* EcuComRxBuf);
extern void hal_kmp_soft_uart_init_minivci(void);
void hal_kmp_soft_uart_init_minivci(void);
extern void hal_kmp_uart_init_irq_minivci(void);
extern void hal_kmp_uart_init_irq_set(uint8_t f_rx_irq);
extern PASSTHRU_CONFIG PassThruConfig;
extern PASSTHRU_DRIVER PassThruDriver;
 extern USART_QueueData	KWP1_RX_QueueData;
extern  uint8_t gUSART1_Handler;
extern void HAL_MINIVCI_UART_Clear(void);
uint8_t HAL_MINIVCI_USART_READ(uint16_t msDelayCounter,uint8_t *data);
uint8 PassThru_BENZ_RxPack_DirectRead_temp();
extern void tx_CTRL_EN_channel2_write(void);
extern void printferror(const char *fmt,...);
uint8_t HAL_MINIVCI_USART_READ(uint16_t msDelayCounter,uint8_t *data)
{
	#if  1//quwenqi2g
	uint32_t RawMessage;
	if(gUSART1_Handler)gUSART1_Handler--;
	T_delay_ms(msDelayCounter);

	while( T_delay_ms_get_status() == RESET)  ////等待超时或接收到KWP数据
	{
		cpu_irq_disable();
		if(USART_Queue_Read(&KWP1_RX_QueueData,&RawMessage) == true)
		{
			*data = RawMessage&0X1FFu;
			UartBit9=(RawMessage&0X1FFu)&0x01;
			cpu_irq_enable();
			return BYTE_OK;
		}
		else
		{
			cpu_irq_enable();	
		}
	}
	*data = 0;
#endif
	return EOF_FLAG;
}
void hal_kwp_send_byte16_kwp(uint16_t data)
{
	uint16_t send_data;
	#ifdef MINI_HW_001
	while(uart_is_tx_empty(KWP_TX_UART) == 0);   //wait Transmitter is empty.
	hal_kwp_read_byte(1,&send_data); 			 //读走发送数据  1ms
	uart_write(KWP_TX_UART,data);				 //send data
	hal_kwp_read_byte(1,&send_data); 			 //读走发送数据  1ms
	hal_kwp_read_byte(1,&send_data); 			 //读走发送数据  1ms
	#endif
	#ifdef MINI_HW_002
	#if 0 //quwenqi
	cpu_irq_disable();
	while(usart_is_tx_empty(KWP_TX_UART) == 0);   //wait Transmitter is empty.
	usart_putchar(KWP_TX_UART,data);				 //send data
	while(usart_is_tx_empty(KWP_TX_UART) == 0);
	cpu_irq_enable();
	#endif
	#endif
}
/*********************************************************************************************
* 名称    ： PassThru_BENZ_SetConfig
* 功能    ： 奔驰专用协议解析初始化IO参数
* 入口参数：
*
*
* 出口参数：
* 说明    ：
*
***********************************************************************************************/
uint8 PassThru_BENZ_SetConfig(uint8 *PackBuf,uint8 CHANEL)
{
    uint8 j;
    uint8 temp8, Len;
    uint8 temp0, temp1, temp2;
    uint32 temp32;
    uint16 PackLen;
    uint8 *PackEnd;
		
    //Reset_Line();
    PassThruConfig.Pin1 =0;//切换can vpw pwm 时，要把对应引脚置零
    PassThruConfig.Pin2=0;
    PackLen = (PackBuf[0] << 8) + PackBuf[1];
    PackEnd = PackBuf + PackLen + 2;
    PackBuf = PackBuf + 3;
    for(;;)
    {

        temp8 = *PackBuf++;
        if(PackBuf > PackEnd || PackBuf == PackEnd)
        {
            break;
        }
        switch(temp8)
        {
        case 0x01:
            temp0 = *PackBuf++;
            temp1 = *PackBuf++;
            temp2 = *PackBuf++;
            temp32 = (temp0 << 16) + (temp1 << 8) + temp2;
            PassThruConfig.BaudRate = temp32; //
            break;
        case 0x02:
            PassThruConfig.Listen = *PackBuf++; //+1
            break;
        case 0x03:
            PassThruConfig.Format = *PackBuf++; //+1
            break;
        case 0x04:
            PassThruConfig.Pin1 =  *PackBuf++; //+1
            PassThruConfig.Pin2 =  *PackBuf++; //+1
            PassThruConfig.LogicV1 =  *PackBuf++; //+1
            PassThruConfig.LogicV2 =  *PackBuf++; //+1
            break;
        case 0x05:
            PassThruConfig.P1Max =  *PackBuf++; //+1
            PassThruConfig.P2Min =  *PackBuf++; //+1
            PassThruConfig.P3Min =  *PackBuf++; //+1
            PassThruConfig.P4Min =  *PackBuf++; //+1
            temp0 = *PackBuf++;
            temp1 = *PackBuf++;
            PassThruConfig.Keep[0] = (temp0 << 8) + temp1;
            break;
        case 0x06:
            PassThruConfig.KeepComMode = *PackBuf++; //+1
            break;
        case 0x07:
            Len = (*PackBuf++) << 1;        //+1
            PassThruConfig.PulseBuf[0] = Len;
            for(j = 0; j < Len; j++)
            {
                PassThruConfig.PulseBuf[j+1] = *PackBuf++; //
            }
            break;
        case 0x08:
            PassThruConfig.CompV = *PackBuf++;
            break;
        case 0x09:
            PassThruConfig.ExtPort = *PackBuf++;
            break;
        default:
            return FALSE;
        }
    }
    return TRUE;
}

/*********************************************************************************************
* 名称    ： PassThru_BENZ_InitEcu()
* 功能    ： 奔驰专用协议初始化ECU
* 入口参数：
*
*
* 出口参数：
* 说明    ：
*
***********************************************************************************************/
static uint8 PassThru_BENZ_InitEcu(uint8 *RecBuf)
{
	hal_kmp_soft_uart_init_minivci();
    SoftPulse((uint8 *)&PassThruConfig.PulseBuf[0]);
	HAL_MINIVCI_UART_Clear();
	hal_kmp_soft_uart_init_minivci();  //soft uart init
	hal_kmp_uart_init_irq_minivci();
	hal_kmp_uart_init_irq_set(1);
	hal_kmp_uart_config_minivci(PassThruConfig.BaudRate,US_MR_MODE9);

    return TRUE;
}

/*********************************************************************************************
* 名称    ： PassThru_BENZ_TxPack_Normal()
* 功能    ： 奔驰专用协议发送数据包程序
* 入口参数：
*
*
* 出口参数：
* 说明    ：此发送程序发送的字节是9bit,数据包的格式是：字节0+bit9+字节1+bit9
*
***********************************************************************************************/
static uint8 PassThru_BENZ_TxPack_Normal(uint8 *TXdata)
{
    uint8  DlyInTxByte;
    uint8  SendByte;
	uint16  SendByte16;
    uint16 PackLength;
    uint16 loop;

    if(!GetLineMonitor()) return FALSE;//超过500us的空闲被认为有效空闲 ，超时500ms，退出发送

    PackLength = (TXdata[0] << 8) + TXdata[1];
    if(PackLength < 2) return FALSE;

    DlyInTxByte = PassThruConfig.P4Min;
    PackLength = PackLength - 1;
   // PackLength = PackLength - 2;
    TXdata = TXdata + 3;
    for(loop = 0; loop < PackLength; loop+=2)
    //for(loop = 0; loop < PackLength; loop++)
    {
        SendByte = *TXdata++;
        if(*TXdata++)
        {
			
            ComCtl.SoftParity = BIT9_1;
			SendByte16=SendByte;
			SendByte16=(SendByte16|(0x001<<8));
			printferror("SendByte16==%04x",SendByte16);
        }
        else
        {
            ComCtl.SoftParity = BIT9_0;
			SendByte16=SendByte;
			SendByte16=(SendByte16|(0x000<<8));
				printferror("SendByte16 2==%04x",SendByte16);
        }
		if(loop > 0) 
		{
			DelayTxByte(DlyInTxByte);
		}
		hal_kwp_send_byte16_kwp(SendByte16);
		//PassThruDriver.TxByteDrv((uint16)SendByte16);
		//if(DlyInTxByte & 0x80)
		//	DelayNus((DlyInTxByte&0x7f)*100);
		//else
		//	DelayNms(DlyInTxByte);
    }
    //PassThruDriver.TxByteDrv(*TXdata++);//发送最后一个字节
    SetEndmsCounter();//endmsCounter = _1msCounter;
    return TRUE;
}

/*********************************************************************************************
* 名称    ： PassThru_BENZ_TxPack_WithRev()
* 功能    ： 带接收功能包发送程序
* 入口参数：
*
*
* 出口参数：
* 说明    ：此发送程序发送的字节是9bit,数据包的格式是：字节0+bit9+字节1+bit9
*
***********************************************************************************************/
static uint8 PassThru_BENZ_TxPack_WithRev(uint8 *TXdata)
{
    uint8  SendByte, ByteStatus;
	uint16  SendByte16;
    uint16 PackLength;
    uint16 loop;
    uint16 ByteNum = 0;
    uint32 DlyInByteCounter;

    if(!GetLineMonitor()) return FALSE;//超过500us的空闲被认为有效空闲 ，超时500ms，退出发送

    PackLength = (TXdata[0] << 8) + TXdata[1];
    if(PackLength < 2) return FALSE;

    DlyInByteCounter = 500 ;
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = BENZ_DATA;
    PackLength = PackLength - 1;
    TXdata = TXdata + 3;
    for(loop = 0; loop < PackLength; )
    {
        SendByte = *TXdata++;
        if(*TXdata++)
        {
            ComCtl.SoftParity = BIT9_1;
			SendByte16=SendByte;
			SendByte16=SendByte16|(0x001<<8);
        }
        else
        {
            ComCtl.SoftParity = BIT9_0;
			SendByte16=SendByte;
			SendByte16=SendByte16|(0x000<<8);
        }
        //PassThruDriver.TxByteDrv((uint16)SendByte16);
		hal_kwp_send_byte16_kwp(SendByte16);
        //接收回应
        ByteStatus = PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&EcuComRxBuf[ByteNum++]);
        if(BYTE_OK != ByteStatus)
        {
            return FALSE;
        }
        loop = loop + 3;
        if(loop < PackLength)
        {
             DelayNms(*TXdata++); //又前延时改为后延时,在延时过程中，下位机可以接收上位机新的指令。
        }
        else
        {
            if(KeepComSwitch == 0) //如果通讯保持，则不往上位机发送收到的数据
            {
                ByteNum -= 2;
                EcuComRxBuf[0] = ByteNum >> 8;
                EcuComRxBuf[1] = ByteNum & 0xff;
                 UartTxQueuePack(EcuComRxBuf);
            }
             DelayNms(*TXdata++); //又前延时改为后延时,在延时过程中，下位机可以接收上位机新的指令。
            break;
        }
    }
    //endmsCounter = _1msCounter;
		SetEndmsCounter();
    return TRUE;
}

/*********************************************************************************************
* 名称    ： PassThru_BENZ_RxPack_WithSend()
* 功能    ： 奔驰接收程序
* 入口参数：
*
*
* 出口参数：
* 说明    ：
*
***********************************************************************************************/
static uint8 PassThru_BENZ_RxPack_WithSend(void)
{
    uint8  ByteStatus;
    uint8  ByteData;
    uint16 ByteNum = 0;
    uint32 DlyInByteCounter;
     uint16  SendByte16;
     if(!Get_Bus_status())
    {
        LineLevelFlag = LINE_UNVALID;
        return FALSE;
    }
	
    if(LineLevelFlag == LINE_VALID)
    {
        return FALSE;
    }
	
		printferror("PassThru_BENZ_RxPack_WithSend" );
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = BENZ_DATA;
    DlyInByteCounter = PassThruConfig.P1Max ;
    while(ByteNum < ECURXBUFLEN)
    {
        ByteStatus = PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData);
        if(ByteStatus == BYTE_FALSE)
        {
            return FALSE;
        }
        else if(ByteStatus == EOF_FLAG)
        {
            break;
        }
        else if(ByteStatus == BYTE_OK)
		{
			SetEndmsCounter();
			EcuComRxBuf[ByteNum++] = ByteData;
			EcuComRxBuf[ByteNum++] = UartBit9;
			if(UartBit9 == 0x01)
			{
				ComCtl.SoftParity = BIT9_1;
				DelayTxByte(PassThruConfig.P4Min);
				//PassThruDriver.TxByteDrv(~ByteData);
				SendByte16=~ByteData;
				SendByte16=(SendByte16|(0x001<<8));
				hal_kwp_send_byte16_kwp(SendByte16);
				SetEndmsCounter();
			}
			else
			{
				break;
			}
        }
        else
        {
            return FALSE;
        }
    }
    counter_1ms = 0;
    ByteNum -= 2;
    EcuComRxBuf[0] = ByteNum >> 8;
    EcuComRxBuf[1] = ByteNum & 0xff;
    UartTxQueuePack(EcuComRxBuf);
    //GetLineLevelStatus();
    return TRUE;
}
/*********************************************************************************************
* 名称    ： PassThru_BENZ_RxPack_Normal()
* 功能    ： 奔驰接收程序
* 入口参数：
*
*
* 出口参数：
* 说明    ：
*
***********************************************************************************************/
static uint8 PassThru_BENZ_RxPack_Normal(void)
{
    uint8  ByteStatus;
    uint8  ByteData;
    uint16 ByteNum = 0;
    uint32 DlyInByteCounter;

	if(PassThruConfig.Format == 0x02)
	{
		return PassThru_BENZ_RxPack_WithSend();
	}
    if(Get_Bus_status()==FALSE)
		 return FALSE;
		 	printferror("PassThru_BENZ_RxPack_Normal" );
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = BENZ_DATA;
    DlyInByteCounter = PassThruConfig.P1Max ;
	//printf_usb3("PassThruConfig.P1Max==%d",PassThruConfig.P1Max);
    while(ByteNum < ECURXBUFLEN)
    {
        ByteStatus = PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData);
        if(ByteStatus == BYTE_FALSE)
        {
            return FALSE;
        }
        else if(ByteStatus == EOF_FLAG)
        {
            break;
        }
        else if(ByteStatus == BYTE_OK)
        {
            SetEndmsCounter();
            EcuComRxBuf[ByteNum++] = ByteData;
            EcuComRxBuf[ByteNum++] = UartBit9;
        }
        else
        {
            return FALSE;
        }
    }
	counter_1ms = 0;
	ByteNum -= 2;
	if(ByteNum==01)//没收到数据
	return FALSE;
	EcuComRxBuf[0] = ByteNum >> 8;
	EcuComRxBuf[1] = ByteNum & 0xff;
	UartTxQueuePack(EcuComRxBuf);
	//GetLineLevelStatus();
	return TRUE;
}

/*********************************************************************************************
* 名称    ： PassThru_BENZ_RxPack_SimuEcu()
* 功能    ： 模拟奔驰ECU
* 入口参数：
*
*
* 出口参数：
* 说明    ：奔驰专用协议
*
***********************************************************************************************/
static uint8 PassThru_BENZ_RxPack_SimuEcu(void)
{
    uint32 temp;

    uint32 delay90ms = 90;
    uint32 delay110ms = 110;
		 
      if(GET_LINE_STATUS())
    {
		 	printferror("PassThru_BENZ_RxPack_SimuEcu" );
        switch(LineInitFlag)
        {
        case  START_DETECT: //跳过引导脉冲
            temp = DelayN_Count_Get_Logic_count(Fpclk,SET_OUT_STATUS_0);
			if(temp > delay90ms && temp < delay110ms)
			{
            EcuComRxBuf[0] = 0;
            EcuComRxBuf[1] = 0x05;
            EcuComRxBuf[2] = BENZ_ECU_INIT;
            EcuComRxBuf[3] = temp >> 24;
            EcuComRxBuf[4] = temp >> 16;
            EcuComRxBuf[5] = temp >> 8;
            EcuComRxBuf[6] = temp & 0xff;
             UartTxQueuePack(EcuComRxBuf);
            LineInitFlag = DETECT_OK;
			}
            return TRUE;
        case  DETECT_OK:
            PassThru_BENZ_RxPack_Normal();
            break;
        case  DETECT_FALSE:
            break;
			default: break;
        }
    }
    return FALSE;
}

/*********************************************************************************************
* 名称    ： PassThru_BENZ_RxPack_DirectRead()
* 功能    ： 侦听字节
* 入口参数：
*
*
* 出口参数：
* 说明    ：一次接收一个字节 + UartBit9
*
***********************************************************************************************/
static uint8 PassThru_BENZ_RxPack_DirectRead(void)
{
    ComCtl.SoftParity = BIT9_0;

    //AudiDirectRead();

    PassThru_BENZ_RxPack_DirectRead_temp();

    //hal_kwp_send_byte16_kwp(UartBit9);
    return TRUE;
}

/*********************************************************************************************
* 名称    ： PassThru_BENZ_InitPort
* 功能    ： 奔驰专用协议初始化IO
* 入口参数：
*
*
* 出口参数：
* 说明    ：
*
***********************************************************************************************/
extern void printferror(const char *fmt,...);
uint8 PassThru_BENZ_InitPort(uint8 CHANEL)
{

    //==================先关闭所有通道，防止设置时对总线产生干扰=====
    //硬件接收or 软件接收
    //quwenqi   NVIC_DisableIRQ(CAN_IRQn);       //禁止CAN 中断	  在使用过CAN 总线后，使用其他信号线时关闭，
    ComCtl.RxHardWare = USE_IO; //用软件接收
	   // arch_ioport_set_pin_dir(PIO_PD28_IDX,IOPORT_DIR_INPUT);    //KWP input 鍜?VPW PWM鍏辩敤绗簩璺?
	PassThruDriver.RxByteDrv = HAL_MINIVCI_USART_READ;
	PassThruDriver.TxByteDrv = hal_kwp_send_byte16_kwp;

    //======================01 设置伯特率     =======================
    LineInitFlag = START_DETECT;
    SetSoftUart(PassThruConfig.BaudRate);
	hal_kmp_soft_uart_init_minivci();  //soft uart init
	
		//切换到硬件UART
	hal_kmp_uart_init_irq_minivci();
	hal_kmp_uart_init_irq_set(1);
		//配置为ECU当前波特率
	hal_kmp_uart_config_minivci(PassThruConfig.BaudRate,US_MR_MODE9);
		
    //===================== 02 设置各驱动程序 =======================
    switch(PassThruConfig.Listen)
    {
    //XK 20121025 合并诊断和侦听数据包模式
    case DIAGNOSE:
    case INTERCEPTION:
        PassThruDriver.InitEcuFunc     = PassThru_BENZ_InitEcu;
        PassThruDriver.TxDataToEcuFunc = PassThru_BENZ_TxPack_Normal;
        PassThruDriver.ReadBusFunc     = PassThru_BENZ_RxPack_Normal;
        PassThruDriver.KeepComFunc     = NULL;
        PassThruDriver.FuncBak0        = NULL;
        break;

    case SIMULATION:
        PassThruDriver.InitEcuFunc     = PassThru_BENZ_InitEcu;
        PassThruDriver.TxDataToEcuFunc = PassThru_BENZ_TxPack_Normal;
        PassThruDriver.ReadBusFunc     = PassThru_BENZ_RxPack_SimuEcu;
        PassThruDriver.KeepComFunc     = NULL;
        PassThruDriver.FuncBak0        = NULL;
        break;

    case DIRECTREAD:
        PassThruDriver.InitEcuFunc     = PassThru_BENZ_InitEcu;
        PassThruDriver.TxDataToEcuFunc = PassThru_BENZ_TxPack_Normal;
        PassThruDriver.ReadBusFunc     = PassThru_BENZ_RxPack_DirectRead;
        PassThruDriver.KeepComFunc     = NULL;
        PassThruDriver.FuncBak0        = NULL;
        break;

    default:
        break;
    }

    if(PassThruConfig.Format == 0x01)// Format==00
    {
        PassThruDriver.TxDataToEcuFunc = PassThru_BENZ_TxPack_WithRev;
				 	printferror("PassThruDriver.TxDataToEcuFunc" );
    }
    //针对benz设置扩展通道
    if(PassThruConfig.ExtPort != 0) // ExtPort==04
    {
           SetCompV(0x45);
           DelayNus(200);
		   SetExtCh(PassThruConfig.ExtPort);
    }
    else		//XK 2012.10.17 新奔驰板
    {
         Set_DrvPort(PassThruConfig.Pin1, PassThruConfig.Pin2, PassThruConfig.LogicV1, PassThruConfig.LogicV2,0);
		//	SetLine_ext(PassThruConfig.Pin1,PassThruConfig.Pin2);
    }
	HAL_MINIVCI_UART_Clear();

    //XK 20121025 引脚悬空设置放到后面
    if((PassThruConfig.Listen == INTERCEPTION) || ((PassThruConfig.Listen == DIRECTREAD)))//Listen==0
    {
        //把接收脚设置为悬空状态，引脚电平由ECU确定
        SetCChannel(NO_DRIVE);
        SetEcuLineLoadMode(LINE0, UP_LOAD);
        SetEcuLineLoadMode(LINE1, UP_LOAD);
		tx_CTRL_EN_channel2_write();
    }

    //=====================08 设置比较电压 ===========================
    SetCompV(PassThruConfig.CompV);	//设置比较电压

    //预先获得总线状态
    //设置通讯保持周期
    KeepComTime[0] = PassThruConfig.Keep[0]; //我们以1ms为单位///Keep==0x0100
    if(KeepComTime[0] == 0) KeepComTime[0] = 10;
    return TRUE;
}



/*********************************************************************************************
* 名称    ： PassThru_KWP_RxPack_DirectRead
* 功能    ： 侦听字节
* 入口参数：
*
*
* 出口参数：
* 说明    ：一次接收一个字节
*
***********************************************************************************************/
uint8 PassThru_BENZ_RxPack_DirectRead_temp()
{
	uint8  ByteData;
	uint32 DlyInByteCounter;
    uint16  ByteData16;
	if(Get_Bus_status()==FALSE)
	return FALSE;
	DlyInByteCounter =  10;
	if(BYTE_OK == PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData))
	{
		if(ComCtl.SoftParity = BIT9_0)
		{
			ByteData16=ByteData;
			ByteData16=ByteData16|(0x001<<8);
		}
		else
		{
			ByteData16=ByteData;
			ByteData16=ByteData|(0x000<<8);
		}
		hal_kwp_send_byte16_kwp(ByteData16);
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}