//#include <asf.h>
#include "config.h"
#include "PassThru.h"
#include "PortAssign.h"
#include "interface.h"
#include "hardware.h"
#include "bluewifi.h"
extern PASSTHRU_CONFIG PassThruConfig;
extern PASSTHRU_DRIVER PassThruDriver;
//extern PASSTHRU_DRIVER PassThruDriver;
//extern ComCtlType ComCtl;  //通讯控制
//进入标志
//extern uint8 EntryReadFlag;

 void hal_HW_Delay_us_2(uint16_t us);
  void hal_HW_Delay_ms_2(uint16_t us);
void UARTAnswerErr(uint8 cmd,uint8 para);
#define TXDATA_DELAY    1
//SPU = STRONG PULL UP
//SPD = STRONG PULL DOWN
extern uint32 TIME_INIT;
extern uint32 TIME_NUMBER;
extern void TimerCreate(uint32 T_Period);

void DataSendRecive(uint32 TxBaudCnter);
void DelayN_Count(uint32 n_Count);
uint32 DetectBaud(void);
void SetSoftUart(uint32 SoftBaud);
void SkipNbit(uint8 Nbit);
uint8 SoftPulse(uint8 *PulseBuf);
#define IOPORT_DIR_OUTPUT  1
void DelayN_Count(uint32 n_Count)
{
	#if 0  //quwenqi2g
        float rc_temp;
		uint16_t us;
		us=	n_Count/72;
		//us
		//MCK = 150MHz
		//TC_CMR_TCCLKS_TIMER_CLOCK2 = MCK/8 = 18.75 MHz
		//?????? = 18.75 : 1
		//???6 - 3495us
		if (us > 3000 || us < 7)  //min 7us  max 3000us
		return;
		
		us = us - 6;  //?????????
		tc_init(TC1, 1, TC_CMR_TCCLKS_TIMER_CLOCK2 |TC_CMR_CPCTRG);  //TC_CMR_TCCLKS_TIMER_CLOCK2
		
		rc_temp = us * 18.75;
		tc_write_rc(TC1,1,(uint16_t)rc_temp);
		tc_start(TC1, 1);
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
// uint8 GetLineLevelStatus()
// {
//     //用硬件接收
// //     if(ComCtl.RxHardWare == USE_HARD)
// //     {
// //         return TRUE;
// //     }
//     if(GET_LINE_STATUS())
//     {
//         LineLevelFlag = LINE_VALID;
//         return TRUE;
//     }
//     else
//     {
//         LineLevelFlag = LINE_UNVALID;
//         return FALSE;
//     }
// }

void SetSoftUart(uint32 SoftBaud)
{
    if(SoftBaud==5)
        ComCtl.ParityEnFlag=DISEN_PARITY;
    else
        ComCtl.ParityEnFlag=NORMAL_RX;

    if(SoftBaud==0)
    {
        ComCtl.RxSoftBaudCnter=0;
        return;
    }
    ComCtl.RxSoftBaudCnter=1000000/SoftBaud; //记忆波特率  24000000/    e424: 411    f424	:384
    ComCtl.TxBaudCnter=ComCtl.RxSoftBaudCnter;
}
//计算奇偶校验位
//返回值0:低电平 1:高电平，2：没有此位
static uint8 _Cal_parity(uint8 Data)
{
    if(ComCtl.ParityEnFlag==DISEN_PARITY)
    {
        Data=2;
        return 2;
    }
    Data = ((Data>>4)|(Data<<4))^Data;
    Data = (Data>>2)^Data;
    Data = (Data>>1)^Data;
    switch(ComCtl.SoftParity)
    {
			case ODD:
    //case EVEN:
        Data&=0x01;
        break;
    //case ODD:
			case EVEN:
        Data=~Data;
        Data&=0x01;
        break;
    case BIT9_1:
        Data=1;
        break;
    case BIT9_0:
        Data=0;
        break;
    default:
        Data=2;
        break;
    }
    return Data;
}




void DataSendRecive(uint32 TxBaudCnter)
{
	#if 0  //quwenqi2g
  while(1)
    {
	while((tc_get_status(TC1,1) & TC_SR_CPCS) == 0)
	{
	   	if(TxBaudCnter<1000)
	   	hal_HW_Delay_us_2(TxBaudCnter); 	//定时//定时
	   	else
	   	hal_HW_Delay_ms_2(TxBaudCnter/1000); 	//定时//定时
		   
        printf_usb2("ecu_staus=%d %d ",ecu_staus,TxBaudCnter,ECU_Byte_num);
		switch(ecu_staus)
		{
			case ecu_send:
    			SET_LINE_STATUS(ECU_Byte&0x1);
    			ECU_Byte=ECU_Byte>>1;//有郊数据右移
    			ECU_Byte_num--;
    			printf_usb2("ECU_Byte_num--");
    			if(ECU_Byte_num==0)
    			{
    				ecu_staus=ecu_stop;
    				return;
    			}
			break;
			case ecu_recv:
    			if(GET_LINE_STATUS()==LOGIC_1)
    			{
    				ECU_Byte|=0x8000;
    			}
    			else
    			{
    				ECU_Byte&=0x7fff;
    			}
    			ECU_Byte=ECU_Byte>>1;
    			ECU_Byte_num--;
    			if(ECU_Byte_num==0)
    			{
    				ecu_staus=ecu_stop;
    				return;
    			}
			break;
			default:
			
    			if(kl_set[3]==0x55)//系统定时来发脉冲
    			{
    				if(kl_set[1]!=0xff)//最高位为1，表示悬空
    				kl_set[1]^=0x01;
    				SET_LINE_STATUS(kl_set[1]);

    			}
			return;
			
		}
	}
    }
	#endif
}

void _SoftTxByte(uint8 Data, uint32 TxBaudCnter)
{
	
//  register uint8 i;
// 	uint32 temp_tc;
    unsigned int TxBaudCnter_buf;
    register uint8  ParityBit;

    TxBaudCnter_buf=ComCtl.TxBaudCnter;
    if(TxBaudCnter_buf==0)//为零，直接退出
        return;
    //判断是要奇偶校验,先计算好
    ParityBit=_Cal_parity(Data);
    if(ParityBit!=2)//有奇偶校验位或bit_1 bit9_0,此时多加一位
    {
        ECU_Byte_num=ComCtl.bitnum+1;
        ECU_Byte=(Data|0x200)|(ParityBit<<8);//多设一个位，停止位和第九位
    }
    else
    {
        ECU_Byte=Data|0x100;//多设一个位，停止位
        ECU_Byte_num=ComCtl.bitnum;
    }
		ECU_Byte_num++;//多一次中断,默认数据为8位,此时要多一次停止位
	
	if(TxBaudCnter_buf<1000)	
		hal_HW_Delay_us_2(TxBaudCnter_buf); 	//定时//定时
	else
		hal_HW_Delay_ms_2(TxBaudCnter_buf/1000); 	//定时//定时
   
	SET_LINE_STATUS(SET_OUT_STATUS_0);//起始位
	ecu_staus=ecu_send;
	
	DataSendRecive(TxBaudCnter_buf);
	
	while(ecu_staus==ecu_send);
	SET_LINE_STATUS(SET_OUT_STATUS_1);

    if(Stop_bit_5bps_flag == 0)
    {
        DelayN_Count(TxBaudCnter_buf);
    }
    Stop_bit_5bps_flag = 0;
}
/*
*********************************************************************************************************
* 函数名称： SoftPulse()
* 函数功能： 产生特定波形的脉冲
* 入口参数：
*
* 出口参数： pulseBuf格式：长度 + 脉冲1时间+脉冲2时间 最高位表示电平，1表示逻辑1，0表示逻辑0
*********************************************************************************************************
*/

#define POLARITY_BIT         0x8000
#define POLARITY_NEGATIVE    0x0000
#define POLARITY_POSITIVE    0x1000
uint8 SoftPulse(uint8 *PulseBuf)
{
    uint8 PulseNum,i;
    uint16 PulseTime;
    //产生前导脉冲
    PulseNum = PulseBuf[0]+1;
	printf_usb2("SoftPulse=%d",PulseNum);
	//arch_ioport_set_pin_dir(PIN_LINE0,IOPORT_DIR_OUTPUT);    //KWP OUTPUT PIN UART发送 LINE0 K
    for(i=1; i<PulseNum;)
    {
        PulseTime=(PulseBuf[i]<<8)+PulseBuf[i+1];
		printf_usb2("SoftPulse PulseTime==%04x",PulseTime);
        if(!PulseTime) return TRUE;
        if((PulseTime&POLARITY_BIT)==POLARITY_NEGATIVE)
            SET_LINE_STATUS(SET_OUT_STATUS_0);
        else
            SET_LINE_STATUS(SET_OUT_STATUS_1);
        //和悦RS遥控匹配需要us级别时间，故将脉冲时间的次高位定义为ms模式或us模式
        //当脉冲时间的次高位为1时，表示us级别（能表示的时间范围0-0x3fff us）
        //否则表示ms级别（能表示的时间范围0-0x3fff ms）
        if((PulseTime & 0x4000) == 0x4000)
            DelayNus(PulseTime & 0x3fff);
        else
            DelayNms(PulseTime & 0x3fff);
        i=i+2;
    }
    return TRUE;
}




void hal_HW_Delay_ms_2(uint16_t ms)
{
	//ms
	//TC_CMR_TCCLKS_TIMER_CLOCK5 = SLCK = 32.768KHz
	//?????? = 32.768 : 1
	//???1 - 1900ms
	#if 0  //quwenqi2g
    
	if (ms > 1900 || ms < 1)  //min 1ms  max 1900ms
	return;
	tc_init(TC1, 1, TC_CMR_TCCLKS_TIMER_CLOCK5 |TC_CMR_CPCTRG);  //TC_CMR_TCCLKS_TIMER_CLOCK5
	uint32_t rc_temp;
	rc_temp = ms * 34;  //??????????
	tc_write_rc(TC1,1,(uint16_t)rc_temp);
	tc_start(TC1, 1);
	//while( (tc_get_status(TC1,1) & TC_SR_CPCS) == 0);  //???CPC??????? ???
#endif
}



void hal_HW_Delay_us_2(uint16_t us)
{
	//us
	//MCK = 150MHz
	//TC_CMR_TCCLKS_TIMER_CLOCK2 = MCK/8 = 18.75 MHz
	//?????? = 18.75 : 1
	//???6 - 3495us
	 #if 0  //quwenqi2g
	if (us > 3000 || us < 7)  //min 7us  max 3000us
	return;
	
	us = us - 6;  //?????????
	tc_init(TC1, 1, TC_CMR_TCCLKS_TIMER_CLOCK2 |TC_CMR_CPCTRG);  //TC_CMR_TCCLKS_TIMER_CLOCK2
	float rc_temp;
	rc_temp = us * 18.75;
	tc_write_rc(TC1,1,(uint16_t)rc_temp);
	tc_start(TC1, 1);
#endif
}
/*
*********************************************************************************************************
* 函数名称：SoftUartRxByte()
* 函数功能：软件接收LINE0上数据,
* 入口参数：
*
* 出口参数：
*********************************************************************************************************
*/
uint8 SoftUartRxByte(uint32 DelayCounter,uint8 *dat)
{  
	register uint32 DelayOneBit;
	DelayOneBit = ComCtl.RxSoftBaudCnter*11;
	if(LineLevelFlag != LINE_UNVALID	)
	{	
	if(DelayN_Count_Get_Logic_count(DelayOneBit+DelayCounter,SET_OUT_STATUS_0)==0)//规定时间内没收到低电平开始数据，则直接退出,此时已有数据故此处省
		return EOF_FLAG;
	}
	LineLevelFlag = LINE_VALID;
	DelayN_Count(ComCtl.RxSoftBaudCnter/4);
	if(ComCtl.RxSoftBaudCnter==0)
	return BYTE_FALSE;
	
	hal_HW_Delay_us_2(ComCtl.RxSoftBaudCnter);
	
	ECU_Byte_num=9;//8位，起始位跳掉，只有八个数据位
	ECU_Byte=0;
	ecu_staus=ecu_recv;
	while(ecu_staus==ecu_recv);
	UartBit9=GET_LINE_STATUS();

     *dat=((ECU_Byte>>6)&0xff);
    if(ComCtl.ParityEnFlag==NORMAL_RX)//正好8字节的话有可能有第九字节，等
    {
        if(ComCtl.SoftParity!=NO_PARITY)
        {
            DelayN_Count(ComCtl.RxSoftBaudCnter);
        }
    }
    if(GET_LINE_STATUS()==0)//此时为低电平，说明状态不对
        return BYTE_FALSE;
    return BYTE_OK;
}
/*
*********************************************************************************************************
* 函数名称：DetectBaud()
* 函数功能：检测波特率
* 入口参数：
*
* 出口参数：
*********************************************************************************************************
*/

uint32 DetectBaud(void)
{
	uint32 temp=0;
	uint8 i;
	uint32 delaytime = 1000;
	
	if(PassThruConfig.W1 != 0){
	delaytime = PassThruConfig.W1;
	} 
	
	//等待起始位
	//if(DelayN_Count_Get_Logic(PassThruConfig.W1*Fpclk/1000,SET_OUT_STATUS_0)==0)//规定时间内没收到低电平开始数据，则直接退出
	//bugfix by w.s 20160806 原有计算值过大, 导致卡死. 原有计算公式PassThruConfig.W1*Fpclk/1000 原因未知;
	if(DelayN_Count_Get_Logic_fpclk_1000(delaytime,SET_OUT_STATUS_0)==0)//规定时间内没收到低电平开始数据，则直接退出
	{  
		//	UARTAnswerErr(0xfc, 0xff);
		return 0;
	}
		
    hal_HW_Delay_us_2(0XFFFF);	
	#if 0  //quwenqi2g
    while((GET_RX0_STATUS)==LOGIC_0)
    {
			if( (tc_get_status(TC1,1) & TC_SR_CPCS) == 0)
			{
				 
				return 0;
				
			}
    }
	#endif
	 hal_HW_Delay_us_2(0XFFFF);
     
    i=0;
    for(i=0; i<4; i++)
    {
		#if 0  //quwenqi2g
        while((GET_RX0_STATUS)==LOGIC_1)
        {
			if( (tc_get_status(TC1,1) & TC_SR_CPCS) == 0)
			{ 
				return 0;
			}
        }
		 temp+=tc_read_cv(TC1,1);
		 hal_HW_Delay_us_2(0XFFFF);
		while((GET_RX0_STATUS)==LOGIC_0)
		{
				if( (tc_get_status(TC1,1) & TC_SR_CPCS) == 0)
				{
					UARTAnswerErr(0xfc, 0xfc);
					return 0;
				}
		}
		temp+=TIME_NUMBER;
		hal_HW_Delay_us_2(0XFFFF);
		#endif
    }

	//temp2=TIM0_TC;
	//temp=TIM0_TC-temp;
	temp=temp>>3; 				 //除以8，求平均得到每bit的时间
	LineLevelFlag = LINE_VALID;
	return temp;
}
/*
*********************************************************************************************************
* 函数名称：SkipNbit()
* 函数功能：跳过Nbit,丰田数据流里用到
* 入口参数：
*
* 出口参数：
*********************************************************************************************************
*/
 void SkipNbit(uint8 Nbit)
{
	if(!ComCtl.RxSoftBaudCnter) return;
	//IWDG_Feed();
	DelayN_Count(ComCtl.RxSoftBaudCnter *  Nbit );
}


