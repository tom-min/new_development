#include "config.h"
#include "PassThru.h"
#include "interface.h"
#include "PassThru_ISO.h"
#include "PassThru_KWP.h"
#include "PortAssign.h"
#include "hardware.h"
#include "newsoftuart.h"
#include "ProtocolDrives.h"
#include "common.h"
#include "bluewifi.h"
extern void UartTxQueuePack(uint8* EcuComRxBuf);
extern void tx_CTRL_EN_channel2(uint8_t JC_CHANNEL,uint8_t VALUE);
extern void HAL_MINIVCI_UART_Clear(void);
void tx_MODE_EN_channel2(uint8_t TXMODE,uint8_t RXMODE,uint8_t TX_REV,uint8_t RX_REV);
void tx_CTRL_EN_channel2_write(void);
extern uint8_t EcuComRxBuf[];
#define NULL 0
#define UART_MR_PAR_NO 0
#define GET_RX0_STATUS 0
#define zyIrqDisable  __disable_irq
#define zyIrqEnable   __enable_irq

#define cpu_irq_disable zyIrqDisable 
#define cpu_irq_enable zyIrqEnable  
#define taskENTER_CRITICAL  cpu_irq_disable
#define taskEXIT_CRITICAL  cpu_irq_enable

void OS_thread_wdt_refresh(void)
{
}

#define KWP_UART_PAR_NO USART_Parity_No
#define KWP_UART_PAR_EVEN USART_Parity_Even
#define KWP_UART_PAR_ODD USART_Parity_Odd


void hal_kwp_send_byte_kwp2(uint8_t data);
extern PASSTHRU_CONFIG PassThruConfig;
extern PASSTHRU_DRIVER PassThruDriver;
 
extern void hal_kmp_uart_init_irq_set(uint8_t f_rx_irq);
extern void hal_kmp_uart_init_irq_minivci(void);
#ifdef MINI_HW_001
extern void  hal_kmp_uart_config_minivci(uint32_t baudrate,uint8_t parity);
#endif
#ifdef MINI_HW_002
extern void  hal_kmp_uart_config_minivci(uint32_t baudrate,uint32_t parity);
#endif

extern void printfiso(char *fmt,...);


#ifdef MINI_HW_001
void hal_kmp_soft_uart_init_minivci(void)
{
	//TXD
	ioport_reset_pin_mode(KWP_TX_PIO_IDX);
	ioport_enable_pin(KWP_TX_PIO_IDX);
	uart_disable_tx(KWP_TX_UART);
	uart_disable(KWP_TX_UART);
	arch_ioport_set_pin_dir(KWP_TX_PIO_IDX,IOPORT_DIR_OUTPUT);
	arch_ioport_set_pin_level(KWP_TX_PIO_IDX,1);				//default output high
	
	//RXD
	ioport_reset_pin_mode(KWP_RX_PIO_IDX);
	ioport_enable_pin(KWP_RX_PIO_IDX);
	
	uart_enable_rx(KWP_RX_UART);
	uart_enable(KWP_RX_UART);
	arch_ioport_set_pin_dir(KWP_RX_PIO_IDX,IOPORT_DIR_INPUT);
}
#endif
#ifdef MINI_HW_002
void hal_kmp_soft_uart_init_minivci(void)
{

}
#endif

void hal_iso_send_byte_kwp(uint8_t data)
{
	 
	cpu_irq_disable();
	if(( (PassThruConfig.Pin1&0x0f)==1)||( (PassThruConfig.Pin1&0x0f)==0x07))
	{
		HAL_KWP0_FrameSend(&data,1);
	}
	else
	{
		HAL_KWP1_FrameSend(&data,1);
	}
	cpu_irq_enable();
	 
}

/*********************************************************************************************
* 名称    ： PassThru_ISO_InitPort
* 功能    ： ISO协议初始化IO
* 入口参数：
*
*
* 出口参数：
* 说明    ：
*
***********************************************************************************************/
extern void hal_kwp_send_byte_kwp(uint8_t data);
extern  HAL_MINIVCI_UART_READ(uint16_t msDelayCounter,uint8_t *data);
extern uint8_t hal_mcan_DisableRxInterrupt(PDU_COMMON_DATA_STRUCT CommonData);
extern PDU_COMMON_DATA_STRUCT MinivciCan;
uint8 PassThru_ISO_InitPort(uint8 CHANEL)
{
	#if 0  //quwenqi2g
	hal_mcan_DisableRxInterrupt(MinivciCan);
	DISABLE_J1850_RX0_IRQ;
	DISABLE_J1850_RX1_IRQ;
	#endif
	K2K_Simulate_firstRX_flag = 0;	//清零
	//SetLine0Channel(0);
	//SetLine1Channel(0);
	SetLine_ext(0,0);
	DelayNms(10);
	
#if 0  //quwenqi2g
    arch_ioport_set_pin_dir(PIO_PD28_IDX,IOPORT_DIR_INPUT);    //KWP input 和 VPW PWM共用第二路
#endif
	
	ComCtl.RxHardWare = USE_IO; //用软件接收
	//NVIC_DisableIRQ(CAN_IRQn);       //禁止CAN 中断	  在使用过CAN 总线后，使用其他信号线时关闭，
	PassThruDriver.RxByteDrv       = HAL_MINIVCI_UART_READ;
	PassThruDriver.TxByteDrv       = hal_iso_send_byte_kwp;
	
	PassThruDriver.InitEcuFunc     = PassThru_ISO_Init_Slow;
	PassThruDriver.TxDataToEcuFunc = PassThru_ISO_TxPack_Normal;
	PassThruDriver.TxDataDelayFunc = PassThru_ISO_TxPack_Normal;
	PassThruDriver.ReadBusFunc     = PassThru_ISO_RxPack_Normal;
	PassThruDriver.KeepComFunc     = NULL;
	PassThruDriver.FuncBak0        = NULL;
    //WABCO协议
    if((PassThruConfig.Format & 0x88) == 0x08)
    {
		PassThruDriver.TxDataToEcuFunc = PassThru_ISO_TxPack_WABCO;
		PassThruDriver.ReadBusFunc     = PassThru_ISO_RxPack_WABCO;
    }
    //======================01 设置伯特率     =======================
    if(PassThruConfig.BaudRate == 0)
    {
        LineInitFlag = START_DETECT;
    }
    else
    {
        LineInitFlag = DETECT_OK;
        SetSoftUart(PassThruConfig.BaudRate);
		
		hal_kmp_soft_uart_init_minivci();  //soft uart init
		hal_kmp_uart_init_irq_minivci();
		hal_kmp_uart_init_irq_set(1);
		hal_kmp_uart_config_minivci(PassThruConfig.BaudRate,UART_MR_PAR_NO);
        UartCtlReg.Baud = PassThruConfig.BaudRate;
    }
    //===================== 02 设置各驱动程序 =======================
    switch(PassThruConfig.Listen)
    {
        case 0x04:
            Stop_bit_5bps_flag = 1;
            break;
        case INTERCEPTION:
            PassThruDriver.ReadBusFunc = PassThru_KWP_RxPack_Interception;
            break;
        case DIRECTREAD:
            PassThruDriver.ReadBusFunc = PassThru_KWP_RxPack_DirectRead;
            break;
        default:
            break;
    }
    //===================03 设置奇偶校验 =============================
    ComCtl.ParityEnFlag = NORMAL_RX;
    switch(PassThruConfig.Format & 0x03)
    {
    case 0x02:
        ComCtl.SoftParity = ODD; //偶校验
        UartCtlReg.parity = ODD;
        PassThruConfig.Parity = ODD;
        break;
    case 0x03:
        ComCtl.SoftParity = EVEN; //奇校验
        UartCtlReg.parity = EVEN;
        PassThruConfig.Parity = EVEN;
        break;
    default:
        ComCtl.SoftParity = NO_PARITY;
        UartCtlReg.parity = NO_PARITY;
        PassThruConfig.Parity = NO_PARITY;
        ComCtl.ParityEnFlag=DISEN_PARITY;
        break;
    }

    //if(ComCtl.RxHardWare == USE_HARD) //用硬件接收
    {
        UartCtlReg.datab = uart_data8;
        UartCtlReg.stopb = uart_stopbit_1;
        // quwenqi MCU_UART_Init(UartCtlReg); 
    }

    //=====================0a Eclk驱动  ==============================
    if(PassThruConfig.Eclk)
    {
        SetCChannel(CLK_OUT_ENABLE);
        SetEclk(PassThruConfig.Eclk);
    }
    else
    {
        SetCChannel(CLK_OUT_DISENABLE);
        SetEclk(0);
    }
//    SetEclk(PassThruConfig.Eclk);

    //预先获得总线状态
    //GetLineLevelStatus();
    //设置通讯保持周期
    KeepComTime[0] = PassThruConfig.Keep[0]; //我们以1ms为单位
    if(KeepComTime[0] == 0) KeepComTime[0] = 10;

    //设置总线驱动格式
    Set_DrvPort(PassThruConfig.Pin1, PassThruConfig.Pin2, PassThruConfig.LogicV1, PassThruConfig.LogicV2,PassThruConfig.GndCom);

    //XK 20121025 引脚悬空设置放到后面
    if((PassThruConfig.Listen == INTERCEPTION) || ((PassThruConfig.Listen == DIRECTREAD)))
    {
         HIGH_RES;
		tx_MODE_EN_channel2(tx_mode_high_res15,0XF0,NORMAL,NORMAL);
		tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_LOW);//LINE 0  PULL UP
		tx_CTRL_EN_channel2(JC2_LINE0_PD,JC_LOW);//LINE 0  PULL DOWN
					
		tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_LOW);//LINE 1  PULL UP
		tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_LOW);//LINE 1  PULL DOWN
		PassThruConfig.TxMode = MODE_HIGH_RES;
		tx_CTRL_EN_channel2_write();
        //把接收脚设置为悬空状态，引脚电平由ECU确定
    }
	
    /*
	  *********************************************
	  * chenli_patch 1.1 start
	  * 设置电压的引脚跟电压值相同，
	  * 则直接返回，不需要设置 
    * 陈立修改
	  *
	  */
		
   	switch((PassThruConfig.GndCom) & 0x0f)
   	{
	   	case 0:
	   	SetLine_ext(PassThruConfig.Pin1,PassThruConfig.Pin2);
	   	break;
	   	case 1:
	   	SetLine_ext(PassThruConfig.GndCom& 0xf0,PassThruConfig.Pin2);
	   	break;
	   	case 2:
	   	SetLine_ext(PassThruConfig.Pin1,PassThruConfig.GndCom & 0xf0);
	   	break;
	   	default:
	   	SetLine_ext(PassThruConfig.Pin1,PassThruConfig.Pin2);
	   	break;
   	} 

    //=====================08 设置比较电压 ===========================
    SetCompV(PassThruConfig.CompV);

    //通讯保持后的延时
    DelayTimeForHoldCom = PassThruConfig.P3Min;
    return TRUE;
}


/*********************************************************************************************
* 名称    ： PassThru_ISO_SetConfig
* 功能    ： ISO协议解析初始化IO命令
* 入口参数：
*
*
* 出口参数：
* 说明    ：
*
***********************************************************************************************/
uint8 PassThru_ISO_SetConfig(uint8 *PackBuf,uint8 CHANEL)
{
    uint8 j;
    uint8 temp8, Len;
    uint8 temp0, temp1, temp2;
    uint32 temp32;
    uint16 PackLen;
    uint8 *PackEnd;
		
    //Reset_Line();				//所有io恢复初始设置
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
            PassThruConfig.W4 = *PackBuf++; //+1
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
            PassThruConfig.KeyByteNum = *PackBuf++; //+1
            break;
        case 0x0a:
            temp0 = *PackBuf++;
            temp1 = *PackBuf++;
            temp2 = *PackBuf++;
            PassThruConfig.Eclk = (temp0 << 16) + (temp1 << 8) + temp2; //+3
            break;
        case 0x0b:
            PassThruConfig.GndCom = *PackBuf++;
            break;
        case 0x0c:
            temp0 = *PackBuf++; //V6.01 LZS
            temp1 = *PackBuf++;
            PassThruConfig.W1 = (temp0 << 8) + (temp1);
            PassThruConfig.ByteTime = *PackBuf++; //V6.01 LZS 仪器回应字节间取反时间单位ms
            PassThruConfig.WaitTime = *PackBuf++; //V6.01 LZS 等待ECU回应取反时间单位ms
            AnswerFlag = ANSWER_ON;
            break;
        //XK 20121024 增加模拟ECU需要用到的参数
        case 0x0d:
            temp0 = *PackBuf++;
            temp1 = *PackBuf++;
            temp2 = *PackBuf++;
            temp32 = (temp0 << 16) + (temp1 << 8) + temp2;
            PassThruConfig._BaudRate = temp32;
            PassThruConfig._WaitTime = *PackBuf++;
            PassThruConfig._ByteTime = *PackBuf++;
            for(j = 0; j < 10; j++)
            {
                PassThruConfig.SendBuf[j] = *PackBuf++;
            }
            break;
        default:
            return FALSE;
        }
    }
    if(PassThruConfig.ByteTime == 0)
    {
        AnswerFlag = ANSWER_OFF;    //v6.01 lzs 为保持兼容而设置
        PassThruConfig.ByteTime = PassThruConfig.P4Min;
    }
    if(PassThruConfig.W1 == 0)
    {
        AnswerFlag = ANSWER_OFF;    //V6.01 lzs
        PassThruConfig.W1 = 1000;
    }
    if(PassThruConfig.WaitTime == 0)
    {
        AnswerFlag = ANSWER_OFF;    //
        PassThruConfig.WaitTime = PassThruConfig.P1Max;//默认10ms //chengbb 20140522 2001年大众帕萨特B5回复时间10.1ms
    }
    return TRUE;
}
/*********************************************************************************************
* 名称    ： PassThru_ISO_Init_Slow
* 功能    ： ISO协议慢速初始化
* 入口参数：
*
*
* 出口参数：
* 说明    ： 大众协议/宝马协议/WABCO协议/MAN协议
*
***********************************************************************************************/

//计算奇偶校验位
//返回值0:低电平 1:高电平 2：没有此位
uint8_t hal_calc_parity(uint8_t Data,uint8_t parity)
{
	uint8_t ret = 2;
	uint8_t num = 0;
	uint8_t i;
	
	if (parity == KWP_UART_PAR_NO)
	{
		return ret;
	}
	
	for (i=0;i<8;i++)
	{
		if (Data&0x01 == 0x01)  //检测该bit是否为1
		{
			num++;
		}
		Data = Data >> 1;
	}
	
	if (parity == KWP_UART_PAR_ODD)
	{
		if (num%2 == 0)            //偶校验：1的个数位偶数 校验位置1
		{
			ret = 1;
		}
		else
		{
			ret = 0;
		}
	} 
	else if(parity == KWP_UART_PAR_EVEN)
	{
		if (num%2 == 1)			   //奇校验：1的个数位奇数 校验位置1
		{
			ret = 1;
		}
		else
		{
			ret = 0;
		}
	}
	else
	{
		ret = 2;
	}
	
	return ret;
}

extern uint8_t hal_calc_parity(uint8_t Data,uint8_t parity);
 
void hal_kwp_soft_5bps_send_byte_minivci(uint8_t data,uint8_t parity)
{
	 
	uint8_t  i;
	//check parity
	uint8_t parity_bit = hal_calc_parity(data,parity);
	//start  bit
	OBD_EVAL_GPIOInit();
	KWP_LINE0_SET_0;

	vTaskDelay(E70B_vTaskDelay_Modulus(200));			//1bit = 2ms OS_delay
	
	//data bit
	for ( i=0;i<8;i++)
	{
		if(data&0x01)         //send lsb
		{
			KWP_LINE0_SET_1;
		}
		else
		{
			KWP_LINE0_SET_0;
		}
		data = data >> 1;
	
		vTaskDelay(E70B_vTaskDelay_Modulus(200));		//1bit = 2ms OS_delay
		
		OS_thread_wdt_refresh();  //喂狗 KWP协议初始化时间比较长 防止超时CPU复位
	}
	//parity bit
	if (parity_bit == 0)
	{
		KWP_LINE0_SET_0;
		vTaskDelay(E70B_vTaskDelay_Modulus(200));		//1bit = 2ms OS_delay
		
	}
	else if (parity_bit== 1)
	{
		KWP_LINE0_SET_1;
		vTaskDelay(E70B_vTaskDelay_Modulus(200));		//1bit = 2ms OS_delay
		
	}
	//stop bit
	KWP_LINE0_SET_1;
	vTaskDelay(E70B_vTaskDelay_Modulus(200));	
	KWP_LINE0_SET_NULL;
}


extern uint32_t hal_detect_baud_kwp0_2000_minivci(CHANNEL_PARAM_ITEM *d_pdu);
extern uint32_t hal_detect_baud_kwp1_2000_minivci(CHANNEL_PARAM_ITEM *d_pdu);
uint32_t hal_detect_baud_ISO_minivci(CHANNEL_PARAM_ITEM *d_pdu)
{
	if(( (PassThruConfig.Pin1 & 0x0f)==1)||( (PassThruConfig.Pin1 & 0x0f)==0x07))
	{
	
		return hal_detect_baud_kwp0_2000_minivci(d_pdu);
	}
	else
	{
	 
		return hal_detect_baud_kwp1_2000_minivci(d_pdu);
	}
	return 0;
}


CHANNEL_PARAM_ITEM  MinivciChannel_1_Param;


uint8_t hal_iso_protocol_init_slow_minivci(CHANNEL_PARAM_ITEM *d_pdu,PASSTHRU_CONFIG *minivci_config,PASSTHRU_DRIVER *minivci_driver,uint8_t *CP_5BaudAddress)
{
	uint32_t ecu_cur_baud;
	uint8_t  recv_key[2];
 
	HAL_MINIVCI_UART_Clear();
	hal_kmp_soft_uart_init_minivci();  //soft uart init
	hal_kmp_uart_init_irq_set(0);
	hal_kwp_soft_5bps_send_byte_minivci(*CP_5BaudAddress,KWP_UART_PAR_NO); //send add 5bps
	
	ecu_cur_baud = hal_detect_baud_ISO_minivci(d_pdu);	//200ms wait detects baud

	if((ecu_cur_baud>9600-20)&&(ecu_cur_baud<9600+20))
	{
		
		ecu_cur_baud = 9600;
		
	}
	else if((ecu_cur_baud>10400-20)&&(ecu_cur_baud<10400+20))
	{
		ecu_cur_baud = 10400;
	}

	minivci_config->BaudRate=ecu_cur_baud;
	minivci_config->CurrBaud=minivci_config->BaudRate;
	if (ecu_cur_baud == 0)
	{
		ecu_cur_baud=9600;
		hal_kmp_uart_config_minivci(ecu_cur_baud,UART_MR_PAR_NO);//【别克凯悦】没有【钥匙系统】会导致 安全气囊无法进入(串口死机)
		return FALSE;
	}
	 
	//切换到硬件UART
	hal_kmp_uart_init_irq_minivci();
	hal_kmp_uart_init_irq_set(1);
	//配置为ECU当前波特率
	hal_kmp_uart_config_minivci(ecu_cur_baud,UART_MR_PAR_NO);
	//接收KEY1 KEY2
	if (minivci_driver->RxByteDrv( (d_pdu->CP_W2Max),&recv_key[0]) != BYTE_OK)
	{
		return FALSE;
	}
	if ( minivci_driver->RxByteDrv( (d_pdu->CP_W3Max),&recv_key[1]) != BYTE_OK)
	{
		return FALSE;
	}
	//printf_usb2("%2X %2X \r\n",recv_key[0],recv_key[1]); //test code
	//发送KEY2取反值
	minivci_config->KeyByte1=recv_key[0];
	minivci_config->KeyByte2=recv_key[1];
	
	return TRUE;
	 
}

//-|W0|----|W1|-----|W2|-----|W3|------|W4|-----|W4|------|P3|----\
//--- |add-|--|-sync|--|key1_|--|-key2-|--|~key2|--|-~add-|--|-----
//----|33H-|--|-55H-|--|-08H-|--|-94H--|--|~key2|--|-~add-|--|-----
void PassThru_ISO_Parm_Init(CHANNEL_PARAM_ITEM *d_pdu,PASSTHRU_CONFIG *minvi)
{
	d_pdu->CP_TIdle=minvi->Tidle;
	d_pdu->CP_TInil=minvi->Tinil;
	d_pdu->CP_TWup=minvi->TWUP;

	d_pdu->CP_W1Max=minvi->W1/1000;
	d_pdu->CP_W1Min=minvi->W1/1000;
	
	d_pdu->CP_W2Max=minvi->W2;
	d_pdu->CP_W2Min=minvi->W2;
	
	d_pdu->CP_W3Max=minvi->W3;
	d_pdu->CP_W3Min=minvi->W3;
	
	d_pdu->CP_W4Min=minvi->W4;
	d_pdu->CP_W4Max=minvi->W4;
	
	d_pdu->CP_Baudrate=d_pdu->CP_Baudrate;
}


uint8 PassThru_ISO_Init_Slow(uint8 *TXdata)
{
    uint32 DlyInByteCounter;
    uint8  KeyByteNum;
    uint8  ByteNum;
    uint8  loop;
    uint8  ByteData;
    uint8  ByteLength;
	uint8 CP_5BaudAddress;
	//printf_usb2(" PassThru_ISO_Init_Slow\r\n"  );
	PassThru_ISO_Parm_Init(&MinivciChannel_1_Param,&PassThruConfig);
	
	hal_kmp_soft_uart_init_minivci();  //soft uart init
	hal_kmp_uart_init_irq_minivci();
	
    //a5 a5 00 04 24 55 01 8a f7
    if(PassThruConfig.Listen == SIMULATION)//case 0x02:
    {
        PassThru_KWP_TxPack_TxData(TXdata);
        //接收取反字节
       
        if(BYTE_OK != PassThruDriver.RxByteDrv(PassThruConfig.W4*1000, (uint8 *)&ByteData))
        {
            return FALSE;
        }
        //发送
        EcuComRxBuf[0] = 0;
        EcuComRxBuf[1] = 2;
        EcuComRxBuf[2] = AUDI_ECU_INIT;
        EcuComRxBuf[3] = ByteData;
        UartTxQueuePack(EcuComRxBuf);
        return TRUE;
    }
		
		//TOOL发送地址码之前总线的最小空闲时间 300ms
		//DelayNms(300);STM32不能等这个300ms不然会慢
		ByteNum = 8;
		CP_5BaudAddress = TXdata[3];
		
		if(FALSE==hal_iso_protocol_init_slow_minivci(&MinivciChannel_1_Param,&PassThruConfig,&PassThruDriver,&CP_5BaudAddress))
		{
			// vTaskDelay(10);
			 //printf_usb2(" CP_5BaudAddress =%02x TXdata[3]=%02x\r\n" ,CP_5BaudAddress ,TXdata[3]);
			return ;
		}
		else
		{
			
			//printf_usb2(" CP_5BaudAddress=%02x TXdata[3]=%02x\r\n" ,CP_5BaudAddress ,TXdata[3]);
		}
		
	EcuComRxBuf[ByteNum++]= PassThruConfig.KeyByte1;
	EcuComRxBuf[ByteNum++]= PassThruConfig.KeyByte2;
		
    //接收其他数据
    if(PassThruConfig.KeyByteNum > 3)
    {
        KeyByteNum = PassThruConfig.KeyByteNum - 3;
		hal_kmp_uart_init_irq_set(1);
        for(loop = 0; loop < KeyByteNum; loop++)
        {
            if(BYTE_OK != PassThruDriver.RxByteDrv(PassThruConfig.W3, (uint8 *)&EcuComRxBuf[ByteNum++]))
            {
                //UARTAnswerErr(0xfc, 0x03);
				printf_usb2("PassThruDriver.RxByteDrv other");
                return FALSE;
            }
        }
    }
    EcuComRxBuf[2] = AUDI_DATA;
    EcuComRxBuf[3] = (PassThruConfig.CurrBaud >> 24) & 0xff;
    EcuComRxBuf[4] = (PassThruConfig.CurrBaud >> 16) & 0xff;
    EcuComRxBuf[5] = (PassThruConfig.CurrBaud >> 8) & 0xff;
    EcuComRxBuf[6] = PassThruConfig.CurrBaud & 0xff;
    EcuComRxBuf[7] = 0x55;
    //WABCO协议 不接收后续数据
    //wabco协议　有两种，第一个字节为1 82 14 14 80表示后续有两个字节
    //第一字节为0,则单字节发送 00 80
    //和大众协议不同的是
		//1,有长度最高位为１
    //2,字节间发送发相同数据，不取反
    //3,字节结束以0x80结束而非0x03
    if((PassThruConfig.Format & 0x88) == 0x08)
    {
		ByteNum = ByteNum - 2;
		EcuComRxBuf[0] = ByteNum >> 8;
		EcuComRxBuf[1] = ByteNum & 0xff;
		UartTxQueuePack(EcuComRxBuf);
		StatusByte |= ECU_INIT_OK;
		PassThruDriver.TxDataToEcuFunc = PassThru_ISO_TxPack_WABCO;
		PassThruDriver.ReadBusFunc     = PassThru_ISO_RxPack_WABCO;
		return TRUE;
    }

    //鍥炲簲鍙栬繑瀛楄妭
    DelayNms(PassThruConfig.W4);
	hal_kmp_uart_init_irq_set(0);
    PassThruDriver.TxByteDrv(~PassThruConfig.KeyByte2);
    //澧炲姞瀵筂AN鍗忚鐨勬敮鎸?澶氭敹涓€涓瓧鑺?浠櫒鍦ㄥ彇鍙嶉€佺浜屽瓧鑺傚悗,浼氬厛鏀跺埌涓€涓?0鏁版嵁锛屽彲浠ョ洿鎺ユ妸姝ゅ瓧鑺備涪鎺?
    if(PassThruConfig.Format & MAN_PROTOCOL_MASK)
    {
		hal_kmp_uart_init_irq_set(1);
	    if(BYTE_OK != PassThruDriver.RxByteDrv(500, (uint8 *)&ByteData))
	    {
		    return FALSE; //绛夊緟500ms
	    }//man鍗忚姝ゅ涓嶆敹
	    //         else
	    //         {
	    //             EcuComRxBuf[ByteNum++] = ByteData;
	    //         }
    }
    //鎺ユ敹杩斿洖鐨勬暟鎹?
	hal_kmp_uart_init_irq_set(1);
    if(BYTE_OK != PassThruDriver.RxByteDrv(500, (uint8 *)&ByteLength))
    {
	    return FALSE; //绛夊緟500ms
    }
    EcuComRxBuf[ByteNum++] = ByteLength;

    //自动判断协议模式 标准KWP协议 直接返回
    if(((~ByteLength) & 0x7f) == (TXdata[3] & 0x7f) && (PassThruConfig.Format&0x80) == 0x80)
    {
        ByteNum = ByteNum - 2;
        EcuComRxBuf[0] = ByteNum >> 8;
        EcuComRxBuf[1] = ByteNum & 0xff;
         UartTxQueuePack(EcuComRxBuf);
        StatusByte |= ECU_INIT_OK;
        PassThruDriver.TxDataToEcuFunc = PassThru_KWP_TxPack_Normal;
        PassThruDriver.ReadBusFunc     = PassThru_KWP_RxPack_Kwp2000;
        return TRUE;
    }
	
	//printf_usb2("%2X\r\n",CP_5BaudAddress);		//test code
	
#if 1
    //ISO9141协议 接收后续数据
    DelayTxByte(PassThruConfig.ByteTime);
	hal_kmp_uart_init_irq_set(0);
    PassThruDriver.TxByteDrv(~ByteLength);
	hal_kmp_uart_init_irq_set(1);
     
    for(loop=0; loop<ByteLength-1; loop++)
    {
        if(BYTE_OK != PassThruDriver.RxByteDrv(PassThruConfig.WaitTime , (uint8 *)&ByteData))
        {
            return FALSE;
        }
        EcuComRxBuf[ByteNum++] = ByteData;
        if(loop == 0)
        {
            PassThruConfig.Counter = ByteData;
        }
        DelayTxByte(PassThruConfig.ByteTime);
		hal_kmp_uart_init_irq_set(0);
        PassThruDriver.TxByteDrv(~ByteData);
		hal_kmp_uart_init_irq_set(1);
    }
    //ISO9141协议 接收最后一个数据
    if(BYTE_OK != PassThruDriver.RxByteDrv(PassThruConfig.WaitTime, (uint8 *)&ByteData))
    {
        return FALSE;
    }
    EcuComRxBuf[ByteNum++] = ByteData;
    //====================================================
    ByteNum = ByteNum - 2;
    EcuComRxBuf[0] = ByteNum >> 8;
    EcuComRxBuf[1] = ByteNum & 0xff;
     UartTxQueuePack(EcuComRxBuf);
    StatusByte |= ECU_INIT_OK;
    PassThruDriver.TxDataToEcuFunc = PassThru_ISO_TxPack_Normal;
    PassThruDriver.ReadBusFunc     = PassThru_ISO_RxPack_Normal;
	#endif
    return TRUE;
}

/*********************************************************************************************
* 名称    ： PassThru_ISO_TxPack_Normal
* 功能    ： ISO协议发送一帧数据 正常模式
* 入口参数：
*
*
* 出口参数：
* 说明    ： 03 00 09 03 长度+计数器+命令+结束符 每字节ECU回复取反(结束符除外)
*
***********************************************************************************************/
uint8 PassThru_ISO_TxPack_Normal(uint8 *TXdata)
{
    uint8  DlyInTxByte;
    uint8  SendData;
    uint8  ReadData;
    uint16 PackLength;
    uint16 loop;
    uint32 DlyInByteCounter;
	printfiso("PassThru_ISO_TxPack_Normal\r\n");
    if(!GetLineMonitor())
	{
		 return FALSE;//超过500us的空闲被认为有效空闲 ，超时500ms，退出发送
	}
	printfiso("PassThru_ISO_TxPack_Normal\r\n");
    PackLength = (TXdata[0] << 8) + TXdata[1];
    if(PackLength < 2) return FALSE;

    SendDelayCounter();//发送之前等待时间 TD2/TD3
    DlyInTxByte = PassThruConfig.ByteTime;
    DlyInByteCounter =  PassThruConfig.WaitTime;
    PackLength = PackLength - 2;
    TXdata = TXdata + 3;
    for(loop = 0; loop < PackLength; loop++)
    {
        SendData = *TXdata++;
        //自动计数
        if(loop == 1)
        {
            PassThruConfig.Counter++;
            SendData = PassThruConfig.Counter; //v6.011 lzs //自动计数器 +1
        }
		
		hal_kmp_uart_init_irq_set(0);
        PassThruDriver.TxByteDrv(SendData);
		hal_kmp_uart_init_irq_set(1);
		
        if(BYTE_OK != PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ReadData))
        {
            return FALSE;
        }
        ReadData = ~ReadData;
        if(SendData == ReadData)
        {
            DelayTxByte(DlyInTxByte);
        }
        else
        {
            return FALSE;
        }
    }
	hal_kmp_uart_init_irq_set(0);
    PassThruDriver.TxByteDrv(*TXdata++);
	hal_kmp_uart_init_irq_set(1);
    SetEndmsCounter();//endmsCounter = _1msCounter;
    //POWER_PULSE_L;
    return TRUE;
}

/*********************************************************************************************
* 名称    ： PassThru_ISO_TxPack_WABCO
* 功能    ： ISO协议发送一帧数据 WABCO协议
* 入口参数：
*
*
* 出口参数：
* 说明    ： 没有计数器 最后一个字节是0x80
*
***********************************************************************************************/
uint8 PassThru_ISO_TxPack_WABCO(uint8 *TXdata)
{
    uint8  DlyInTxByte;
    uint8  SendData;
    uint8  ReadData;
    uint16 PackLength;
    uint16 loop;
    uint32 DlyInByteCounter;
	printfiso("PassThru_ISO_TxPack_Normal\r\n");
    if(!GetLineMonitor()) return FALSE;//超过500us的空闲被认为有效空闲 ，超时500ms，退出发送

    PackLength = (TXdata[0] << 8) + TXdata[1];
    if(PackLength < 2) return FALSE;

    SendDelayCounter();//发送之前等待时间 TD2/TD3
    DlyInTxByte = PassThruConfig.ByteTime;
    DlyInByteCounter =   PassThruConfig.WaitTime;
    PackLength = PackLength - 2;
    TXdata = TXdata + 3;
    for(loop = 0; loop < PackLength; loop++)
    {
        SendData = *TXdata++;
        //如果是WABCO协议,不需要自动计数，只有大众老协议才需要
		hal_kmp_uart_init_irq_set(0);
        PassThruDriver.TxByteDrv(SendData);
		hal_kmp_uart_init_irq_set(1);
        if(BYTE_OK != PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ReadData))
        {
            return FALSE;
        }
        DelayTxByte(DlyInTxByte);
    }
    //发送最后一个字节，如果是WABCO协议，最后一个字节是 80
	
	hal_kmp_uart_init_irq_set(0);
    PassThruDriver.TxByteDrv(*TXdata++);
	hal_kmp_uart_init_irq_set(1);
    SetEndmsCounter();//endmsCounter = _1msCounter;
    //POWER_PULSE_L;
    return TRUE;
}


/*********************************************************************************************
* 名称    ： PassThru_ISO_RxPack_Normal
* 功能    ： ISO协议接收一帧数据 正常模式
* 入口参数：
*
*
* 出口参数：
* 说明    ： 03 00 09 03 长度+计数器+命令+结束符 每字节回复取反到ECU(结束符除外)
*
***********************************************************************************************/
uint8 PassThru_ISO_RxPack_Normal(void)
{
    uint8  ByteData;
    uint8  ByteLength;
    uint8  loop;
    uint16 ByteNum = 0;
    uint32 DlyInByteCounter;
    

      if(Get_Bus_status()==FALSE)
		 return FALSE;

    if((PassThruConfig.BaudRate == 5)||(PassThruConfig.BaudRate <200))
    {
        if(GET_RX0_STATUS==1)
        return FALSE;
        if((PassThruConfig.P2Min & 0x80) == 0x80)
        {
            DlyInByteCounter = ((PassThruConfig.P2Min) & 0x7f);
			DlyInByteCounter|=0x80;
			//if(DlyInByteCounter%10)DlyInByteCounter=(DlyInByteCounter/10)+1;
        }
        else
        {
            DlyInByteCounter = (PassThruConfig.P2Min);
        }
		hal_kmp_uart_init_irq_set(1);
        // DlyInByteCounter =  (PassThruConfig.P2Min);//changyl2016 1 18日修改，在其它程序中增加0.1ms控制，统一所有函数
        if(BYTE_OK != PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData))
            return FALSE;
        EcuComRxBuf[ByteNum++] = 0x00;
        EcuComRxBuf[ByteNum++] = 0x02;
        EcuComRxBuf[ByteNum++] = AUDI_DATA;
        EcuComRxBuf[ByteNum++] = ByteData;
        PassThruConfig.Counter = 0x00; //v6.61
		UartTxQueuePack(EcuComRxBuf);
		DelayNms(200);
		// GetLineLevelStatus();
		SimulateEcu5bps();
        return TRUE;
    }

    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = AUDI_DATA;
    if((PassThruConfig.P2Min & 0x80) == 0x80)
    {
        DlyInByteCounter = ((PassThruConfig.P2Min) & 0x7f);
		DlyInByteCounter|=0x80;
		//if(DlyInByteCounter%10)DlyInByteCounter=(DlyInByteCounter/10)+1;
    }
    else
    {
        DlyInByteCounter = (PassThruConfig.P2Min) ;
    }
	hal_kmp_uart_init_irq_set(1);
    if(BYTE_OK != PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData))
    {
        return FALSE;
    }
	LineLevelFlag = LINE_VALID;
    EcuComRxBuf[ByteNum++] = ByteData;
    ByteLength = ByteData;
	DelayTxByte(PassThruConfig.ByteTime); //v6.01 lzs
	hal_kmp_uart_init_irq_set(0);
	PassThruDriver.TxByteDrv(~ByteData);  //回应取反字节
	hal_kmp_uart_init_irq_set(1);
    DlyInByteCounter = (PassThruConfig.WaitTime);
    for(loop = 0; loop < ByteLength-1; loop++)
    {
        if(BYTE_OK != PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData))
        {
            return FALSE;
        }
        EcuComRxBuf[ByteNum++] = ByteData;
        DelayTxByte(PassThruConfig.ByteTime);
		hal_kmp_uart_init_irq_set(0);
        PassThruDriver.TxByteDrv(~ByteData);  //回应取反字节
		hal_kmp_uart_init_irq_set(1);
        if(loop == 0)
        {
            PassThruConfig.Counter = ByteData;
        }

    }
	hal_kmp_uart_init_irq_set(1);
    if(BYTE_OK != PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData))
    {
        return FALSE;
    }
    EcuComRxBuf[ByteNum++] = ByteData;
    SetEndmsCounter();
    ByteNum -= 2;
    EcuComRxBuf[0] = ByteNum >> 8;
    EcuComRxBuf[1] = ByteNum & 0xff;
     UartTxQueuePack(EcuComRxBuf);
    //GetLineLevelStatus();
    return TRUE;
}

/*********************************************************************************************
* 名称    ： PassThru_ISO_RxPack_WABCO
* 功能    ： ISO协议接收一帧数据 WABCO协议
* 入口参数：
*
*
* 出口参数：
* 说明    ： WEBCO两种数据模式
*            bit7为1表示多个字节 带长度控制 如82 01 02 80
*            bit7为0表示单个字节通讯 如00 80
*
***********************************************************************************************/
uint8 PassThru_ISO_RxPack_WABCO(void)
{
    uint8  ByteData;
    uint8  ByteLength;
    uint8  loop;
    uint16 ByteNum = 0;
    uint32 DlyInByteCounter;
	if(Get_Bus_status()==FALSE)
	return FALSE;
    if((PassThruConfig.BaudRate == 5)||(PassThruConfig.BaudRate <200))
    {

		if(GET_RX0_STATUS==1)
		return FALSE;
		DlyInByteCounter =   (PassThruConfig.P1Max);
		hal_kmp_uart_init_irq_set(1);
		if(BYTE_OK != PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData))
		return FALSE;
		EcuComRxBuf[ByteNum++] = 0x00;
		EcuComRxBuf[ByteNum++] = 0x02;
		EcuComRxBuf[ByteNum++] = AUDI_DATA;
		EcuComRxBuf[ByteNum++] = ByteData;
		DelayNms(200); //接收的时候延迟(50ms) 1/4bit
		PassThruConfig.Counter = 0x00; //v6.61
		UartTxQueuePack(EcuComRxBuf);
		SimulateEcu5bps();
        return TRUE;
    }

    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = AUDI_DATA;
    if((PassThruConfig.P2Min & 0x80) == 0x80)
    {
        DlyInByteCounter = ((PassThruConfig.P2Min) & 0x7f);
		DlyInByteCounter|=0x80;
		//if(DlyInByteCounter%10)DlyInByteCounter=(DlyInByteCounter/10)+1;
    }
    else
    {
        DlyInByteCounter = (PassThruConfig.P2Min);
    }
	hal_kmp_uart_init_irq_set(1);
    if(BYTE_OK != PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData))
    {
        return FALSE;
    }
     EcuComRxBuf[ByteNum++] = ByteData;
    //判断WEBCO两种数据模式 bit7为1表示多个字节 带长度控制 如82 01 02 80
    if(ByteData & 0x80)
    {
        ByteLength = (ByteData & 0x7f)+1;
    }
    else	//bit7为0表示单个字节通讯 如00 80
    {
        ByteLength = ByteData + 1;
    }
    DelayTxByte(PassThruConfig.ByteTime); //v6.01 lzs
	hal_kmp_uart_init_irq_set(0);
    PassThruDriver.TxByteDrv(ByteData);
	hal_kmp_uart_init_irq_set(1);
    DlyInByteCounter = (PassThruConfig.WaitTime);
    for(loop = 0; loop < ByteLength-1; loop++)
    {
        if(BYTE_OK != PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData))
        {
            return FALSE;
        }
        EcuComRxBuf[ByteNum++] = ByteData;
        DelayTxByte(PassThruConfig.ByteTime);
		hal_kmp_uart_init_irq_set(0);
        PassThruDriver.TxByteDrv(ByteData);
		hal_kmp_uart_init_irq_set(1);
    }
    if(BYTE_OK != PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData))
    {
        return FALSE;
    }
    EcuComRxBuf[ByteNum++] = ByteData;
    SetEndmsCounter();
    ByteNum -= 2;
    EcuComRxBuf[0] = ByteNum >> 8;
    EcuComRxBuf[1] = ByteNum & 0xff;
     UartTxQueuePack(EcuComRxBuf);
    //GetLineLevelStatus();
    return TRUE;
}
