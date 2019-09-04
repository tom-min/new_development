#include "config.h"
#include "PassThru.h"
#include "interface.h"
#include "PortAssign.h"
#include "hardware.h"
#include "NewSoftUart.h"
#include "Usart1.h"
 
#define TIM0_TC 1
extern uint8_t EcuComRxBuf[];
#define NULL 0
#define Int_time
extern PASSTHRU_CONFIG PassThruConfig;
extern PASSTHRU_DRIVER PassThruDriver;


#define PULSE_VALID     0
#define PULSE_UNVALID   0xff
#define END_OF_BIT      2
#define END_OF_NIBBLE   3
#define END_OF_BYTE     4

#define READ_TOYOTA_CODE          0  //读丰田闪光码
#define READ_TOYOTA_DATASTREAM    1  //读丰田数据流
#define READ_BENZ_DATA            2  //读BENZ闪光码数据
#define READ_FORD_CODE            3  //读福特闪光码

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
uint8 PassThru_PLUSE_SetConfig(uint8 *PackBuf,uint8 CHANEL)
{
    uint8 temp8;
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
        if(PackBuf >= PackEnd) //return TRUE;
            break;
        switch(temp8)
        {
        case 0x01:
            PassThruConfig.Format = *PackBuf++;
            break;
        case 0x02:
            PassThruConfig.Listen = *PackBuf++; //+1
            break;
        case 0x03:
            temp0 = *PackBuf++;
            temp1 = *PackBuf++;
            temp2 = *PackBuf++;
            temp32 = (temp0 << 16) + (temp1 << 8) + temp2;
            PassThruConfig.BaudRate = temp32; //
            break;
        case 0x04:
            temp0 = *PackBuf++;
            temp1 = *PackBuf++;
            PassThruConfig.T1Max = (temp0 << 8) + temp1;
            temp0 = *PackBuf++;
            temp1 = *PackBuf++;
            PassThruConfig.T2Max = (temp0 << 8) + temp1;
            temp0 = *PackBuf++;
            temp1 = *PackBuf++;
            PassThruConfig.T3Max = (temp0 << 8) + temp1;
            temp0 = *PackBuf++;
            temp1 = *PackBuf++;
            PassThruConfig.T4Max = (temp0 << 8) + temp1;
            temp0 = *PackBuf++;
            temp1 = *PackBuf++;
            PassThruConfig.T5Max = (temp0 << 8) + temp1;
            break;
        case 0x05:
            PassThruConfig.Pin1 =  *PackBuf++; //+1
            PassThruConfig.Pin2 =  *PackBuf++; //+1
            PassThruConfig.LogicV1 =  *PackBuf++; //+1
            PassThruConfig.LogicV2 =  *PackBuf++; //+1
            break;
        case 0x06:
            PassThruConfig.CompV = *PackBuf++;
            break;
        case 0x07:
            temp0 = *PackBuf++;
            temp1 = *PackBuf++;
            PassThruConfig.GndCom = (temp0 << 8) + temp1;
            break;
        default:
            return FALSE;
        }
    }
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
static uint8 _ReadPulseByte_SPD(uint8 *dat)
{
  
    uint8 add_tmp,TempDat,datok,num;
    uint32 TIM_tmp1,TIM_tmp2;


    //if((GET_RX0_STATUS)) return PULSE_UNVALID;//高电平直接退出
    num=0;
    while(1)
    {

        add_tmp=0x01;
        Int_time(2,25000);//定时20s，理论上所有数据全部收完
        datok=0;
        TempDat=0;
        TIM_tmp1=TIM0_TC;
        while(1)
        {
            while(GET_RX0_STATUS==0)//低电平，表示有郊电平
            {
               // if(RxFrameFlag ==FrmEndFlg) return PULSE_UNVALID;//死循环时上位机有发数据出来，便退出
							if(usart_len>0)return PULSE_UNVALID;//死循环时上位机有发数据出来，便退出
							if((TIM0_TC-TIM_tmp1)>PassThruConfig.T5Max+10)//低脉冲时间太长，出错中断
                    return PULSE_UNVALID;
            }
            TIM_tmp2=TIM0_TC;
            if((TIM_tmp2-TIM_tmp1)<PassThruConfig.T5Max-100)//低脉冲时间太短，出错中断
                return PULSE_UNVALID;
            TIM_tmp1=TIM_tmp2;
            TempDat+=add_tmp;
            while(GET_RX0_STATUS)
            {
                //if(RxFrameFlag ==FrmEndFlg) return PULSE_UNVALID;//死循环时上位机有发数据出来，便退出
								if(usart_len>0)return PULSE_UNVALID;//死循环时上位机有发数据出来，便退出
                if((TIM0_TC-TIM_tmp1)>PassThruConfig.T1Max)//高脉冲时间多于帧间间隔，退出
                {
                    *dat=TempDat;
                    num++;
                    return num;
                }
            }
            TIM_tmp2=TIM0_TC;
            if((TIM_tmp2-TIM_tmp1)>PassThruConfig.T2Max-10)//高脉冲时间多于字节间间隔，
            {
                if(	datok==1)//已收完低字节，在收高字节时得到的数据
                {

                    *dat++=TempDat;
                    num++;
                    TempDat=0;
                    break;	//退出第一个循环
                }
                else
                    return PULSE_UNVALID;
            }

            if((TIM_tmp2-TIM_tmp1)>PassThruConfig.T3Max-10)//高脉冲时间多于高低位间间隔，
            {
                datok=1;//一字节完成
                add_tmp=0x10;
            }
            if((TIM_tmp2-TIM_tmp1)<PassThruConfig.T4Max-10)//脉冲间间隔小设定的脉冲间间隔退出
            {
                return PULSE_UNVALID;
            }
            TIM_tmp1=TIM_tmp2;
        }
    }

    //return PULSE_UNVALID;

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
static uint8 PassThru_PLUSE_ReadCodeByte(void)
{
    uint8 num;

    if(GET_LINE_STATUS()==0)
    {
        num=_ReadPulseByte_SPD(EcuComRxBuf+3);
        if( num>0)
        {
            EcuComRxBuf[0] = 0;
            EcuComRxBuf[1] = num+1;
            EcuComRxBuf[2] = FLASH_DATA;
            //UartTxQueuePack(EcuComRxBuf);
            return TRUE;
        }
    }

    return FALSE;
}
uint8 PLUSE_RxPack_Normal()
{
    uint8  ByteStatus;
    uint8  ByteData;
    uint16 ByteNum = 0;
    uint32 DlyInByteCounter;
	  DlyInByteCounter = PassThruConfig.P1Max* (Fpclk / 1000);
		ByteNum=3;
    while(ByteNum < ECURXBUFLEN)
    {
		//IWDG_Feed();
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
        }
        else
        {
            return FALSE;
        }
    }
    EcuComRxBuf[0] = 0x00;
    EcuComRxBuf[1] = 0x00;
    EcuComRxBuf[2] = KWP2K_DATA;
    if(ByteNum == 0x03) return FALSE;//没有收到数据 过滤杂波
    counter_1ms = 0;
    ByteNum -= 2;
    EcuComRxBuf[0] = ByteNum >> 8;
    EcuComRxBuf[1] = ByteNum & 0xff;
    UartTxQueuePack(EcuComRxBuf);
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
static uint8 PassThru_PLUSE_ReadDataStream(void)
{
    if(GET_LINE_STATUS()==0)
    {
        SkipNbit(15);
        PassThruConfig.P1Max = PassThruConfig.T1Max;
			LineLevelFlag = LINE_UNVALID;
        return PLUSE_RxPack_Normal();//PassThru_KWP_RxPack_Normal();
    }

    return FALSE;
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
static uint8 PassThru_PLUSE_SendCodeBytes(uint8 *buf)
{
    uint16 Len;
    uint16 i;
    uint16 k;
    uint8 temp;
    uint8 PulseBuf[3];

    Len = (buf[0] << 8) + buf[1];
    if(Len < 2) return FALSE;
    Len = Len - 1;
    buf = buf + 3;
    PulseBuf[0] = 0x01;
    PulseBuf[1] = (PassThruConfig.T1Max >> 8) | 0x80;
    PulseBuf[2] = (PassThruConfig.T1Max & 0xff); //3200ms H
    SoftPulse((uint8 *)PulseBuf);
    for(k = 0; k < Len; k++)
    {
        temp = *buf++;
        temp &= 0x0f;

        for(i = 0; i<temp; i++)
        {
            PulseBuf[0] = 0x01;
            PulseBuf[1] = (PassThruConfig.T5Max >> 8) & 0x7f;//0x01
            PulseBuf[2] = (PassThruConfig.T5Max & 0xff); //0x90 400ms L
            SoftPulse((uint8 *)PulseBuf);
            PulseBuf[0] = 0x01;
            PulseBuf[1] = (PassThruConfig.T4Max >> 8) | 0x80; //0x81;
            PulseBuf[2] = (PassThruConfig.T4Max & 0xff); //0x90 400ms H
            SoftPulse((uint8 *)PulseBuf);
        }
        PulseBuf[0] = 0x01;
        PulseBuf[1] = ((PassThruConfig.T3Max-PassThruConfig.T4Max) >> 8) | 0x80; //0x86;
        PulseBuf[2] = ((PassThruConfig.T3Max-PassThruConfig.T4Max) & 0xff); //0x40; //1600ms H
        SoftPulse((uint8 *)PulseBuf);
    }
    PulseBuf[0] = 0x01;
    PulseBuf[1] = ((PassThruConfig.T2Max-PassThruConfig.T3Max) >> 8) | 0x80;
    PulseBuf[2] = ((PassThruConfig.T2Max-PassThruConfig.T3Max) & 0xff); //3200ms H
    SoftPulse((uint8 *)PulseBuf);
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
static void _TxBytePulse(uint8 dat)
{
    uint8 PulseBuf[3];
    uint8 i;

    for(i = 0; i<(dat >> 4); i++)
    {
        PulseBuf[0] = 0x01;
        PulseBuf[1] = (PassThruConfig.T5Max >> 8) & 0x7f; //0x01;
        PulseBuf[2] = (PassThruConfig.T5Max & 0xff); //0xf4; //500ms L
        SoftPulse((uint8 *)PulseBuf);
        PulseBuf[0] = 0x01;
        PulseBuf[1] = (PassThruConfig.T4Max >> 8) | 0x80; //0x81;
        PulseBuf[2] = (PassThruConfig.T4Max & 0xff); //0xf4; //500ms H
        SoftPulse((uint8 *)PulseBuf);
    }
    PulseBuf[0] = 0x01;
    PulseBuf[1] = (PassThruConfig.T3Max >> 8) | 0x80; //0x83;
    PulseBuf[2] = (PassThruConfig.T3Max & 0xff); //0xE8; //1000ms H
    SoftPulse((uint8 *)PulseBuf);
    for(i = 0; i < (dat & 0x0f); i++)
    {
        PulseBuf[0] = 0x01;
        PulseBuf[1] = (PassThruConfig.T5Max >> 8) & 0x7f; //0x01;
        PulseBuf[2] = (PassThruConfig.T5Max & 0xff); //0xf4; //500ms L
        SoftPulse((uint8 *)PulseBuf);
        PulseBuf[0] = 0x01;
        PulseBuf[1] = (PassThruConfig.T4Max >> 8) | 0x80; //0x81;
        PulseBuf[2] = (PassThruConfig.T4Max & 0xff); //0xf4; //500ms H
        SoftPulse((uint8 *)PulseBuf);
    }
    PulseBuf[0] = 0x01;
    PulseBuf[1] = (PassThruConfig.T2Max >> 8) | 0x80;
    PulseBuf[2] = (PassThruConfig.T2Max & 0xff); //2500ms
    SoftPulse((uint8 *)PulseBuf);

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
static uint8 PassThru_PLUSE_SendCodeByte(uint8 *buf)
{
    uint16 Len;
    uint16 k;
    uint8 temp;

    Len = (buf[0] << 8) + buf[1];
    if(Len < 2) return FALSE;
    Len = Len - 2;
    buf = buf + 3;
    for(k = 0; k < Len; k++)
    {
        temp = *buf++;
        _TxBytePulse(temp);
    }
    temp = *buf++;
    _TxBytePulse(temp);

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
static uint8 PassThru_PLUSE_SendDataStream(uint8 *buf,uint32 TxBaudCnter)
{
    uint16 Len;
    uint16 k;
    uint8 temp;

    Len = (buf[0] << 8) + buf[1];
    if(Len == 0) return FALSE;
    Len = Len - 1;
    buf = buf + 3;
    if(Len == 1)
    {
        _SoftTxByte(*buf++,TxBaudCnter);

    }
    else if(Len == 2)
    {
        _SoftTxByte(*buf++,TxBaudCnter);
        _SoftTxByte(*buf++,TxBaudCnter);
    }
    else
    {
        _SoftTxByte(*buf++,TxBaudCnter);
        ComCtl.bitnum = 3;
        _SoftTxByte(*buf++,TxBaudCnter);
        ComCtl.bitnum = 8;
        Len = Len - 2;
        for(k = 0; k < Len; k++)
        {
            temp = *buf++;
            _SoftTxByte(temp,TxBaudCnter);
        }
    }
    ComCtl.bitnum = 8;
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
uint8 PassThru_PLUSE_InitPort(uint8 CHANEL)
{
    Reset_Line();
//		Set_LINE0_IN_NORMAL();
    //	Set_LINE0_OUT_NORMAL();
    DelayNms(10);
    ComCtl.RxHardWare = USE_IO; //用软件接收
    //NVIC_DisableIRQ(CAN_IRQn);       //禁止CAN 中断	  在使用过CAN 总线后，使用其他信号线时关闭，
    //03 设置波特率
    SetSoftUart(PassThruConfig.BaudRate);
    Set_DrvPort(PassThruConfig.Pin1, PassThruConfig.Pin2, PassThruConfig.LogicV1, PassThruConfig.LogicV2,PassThruConfig.GndCom>>8);
    //侦听模式下，总线悬空
    if(PassThruConfig.Listen == INTERCEPTION | PassThruConfig.Listen == DIRECTREAD)
    {
        SetEcuLineLoadMode(LINE0, NO_LOAD);
        SetEcuLineLoadMode(LINE1, NO_LOAD);
    }
    //06设置比较电压
    ///SetCompV(0X20);//PassThruConfig.CompV);
    DelayNms(10);
// 		if(PassThruConfig.RxMode==MODE_ONLY_LINE0_RX_NORMAL)
// 		{
// 			ONLY_LINE0_TX_NORMAL;
// 		}
// 		else
// 		{
// 			ONLY_LINE1_TX_NORMAL;
// 		}
    LINE0_OR_LINE1_HIGH;
		//quwenqi TOYO_PULSE;
    DelayNms((PassThruConfig.GndCom & 0xff) * 100); //等待
    LINE0_OR_LINE1_LOW;
    //使用软件串口
    PassThruDriver.RxByteDrv = SoftUartRxByte;
    PassThruDriver.TxByteDrv = _SoftTxByte;

    //设置通讯保持周期
    KeepComTime[0] = PassThruConfig.Keep[0]; //我们以1ms为单位
    if(KeepComTime[0] == 0) KeepComTime[0] = 10;
		//quwenqi G_RXD_EN;
		
    //01 根据类型装载不同驱动程序
    switch(PassThruConfig.Format)
    {
    case READ_TOYOTA_CODE:
        PassThruDriver.ReadBusFunc    = NULL;//PassThru_PLUSE_ReadCodeByte;//42
        PassThruDriver.TxDataToEcuFunc = PassThru_PLUSE_SendCodeByte;
		Reset_All_IO();
		Reset_Fun();
        break;
    case READ_TOYOTA_DATASTREAM:
        PassThruDriver.ReadBusFunc     = PassThru_PLUSE_ReadDataStream;
        PassThruDriver.TxDataToEcuFunc = PassThru_PLUSE_SendDataStream;
		
        break;
    case READ_BENZ_DATA:
        PassThruDriver.ReadBusFunc      = NULL;
        break;
    case READ_FORD_CODE:
        PassThruDriver.ReadBusFunc     = PassThru_PLUSE_ReadCodeByte;//422
        PassThruDriver.TxDataToEcuFunc = PassThru_PLUSE_SendCodeBytes;
        break;
    }
    //NVIC_EnableIRQ(EINT3_IRQn);
    return TRUE;
}
