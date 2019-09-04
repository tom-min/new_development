#include "config.h"
#include "PassThru.h"
#include "interface.h"
#include "PortAssign.h"
 
#define TIM0_TC 1
extern uint8_t EcuComRxBuf[];
#define NULL 0
extern PASSTHRU_CONFIG PassThruConfig;
extern PASSTHRU_DRIVER PassThruDriver;
 
extern void tx_MODE_EN_channel2(uint8_t TXMODE,uint8_t RXMODE,uint8_t TX_REV,uint8_t RX_REV);
#define WAVE_RATE  2
#define EORBIT   0x00010800  //0x00000600	  
uint8 PassThru_WAVE_ReadWave(void);
//耗时3.3us
void SendBufUseInt(uint8 dat)
{
    //uint8 dat1;
    //rflag=0;
    if(WaveNum==0)
    {
        EcuComRxBuf[WaveNum++] = 0x00;
        EcuComRxBuf[WaveNum++] = 0x00;
        EcuComRxBuf[WaveNum++] = WAVE_DATA;
    }
    EcuComRxBuf[WaveNum++] = dat;
    if(WaveNum >130)
    {
        WaveNum -= 2;
        EcuComRxBuf[0] = WaveNum >> 8;
        EcuComRxBuf[1] = WaveNum & 0xff;
      //quwenqi     UartTxQueuePack(EcuComRxBuf);
        WaveNum=0;
    }


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
uint8 PassThru_WAVE_InitPort(uint8 CHANEL)
{
//		Reset_All_IO();
     //quwenqi  Reset_Line();
    //quwenqi   SetCompV(PassThruConfig.CompV);
    //初始化串口到高速队列模式
    //InitTxBufToQueue();
    ExitWaveFlag = WAVE_DETING;
    //装载驱动程序
    PassThruDriver.InitEcuFunc     = NULL;
    PassThruDriver.TxDataToEcuFunc = NULL;
    PassThruDriver.TxDataDelayFunc = NULL;
    PassThruDriver.ReadBusFunc     = NULL;
    PassThruDriver.KeepComFunc     = NULL;
    PassThruDriver.FuncBak0        = PassThru_WAVE_ReadWave;
    WaveNum=0;

    //quwenqi   DelayNms(10);										//将设置通道放在其它引脚已设置好的情况下
//     SetLine0Channel(PassThruConfig.Pin1);
//     SetLine1Channel(PassThruConfig.Pin2);
	
	  //quwenqi 	SetLine_ext(PassThruConfig.Pin1,PassThruConfig.Pin2);
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
uint8 PassThru_WAVE_SetConfig(uint8 *PackBuf,uint8 CHANEL)
{
    uint8  temp8;
    uint16 PackLen;
    uint8 *PackEnd;
		
    //Reset_Line();
    PackLen = (PackBuf[0] << 8) + PackBuf[1];
    PackEnd = PackBuf + PackLen + 2;
    PackBuf = PackBuf + 3;
    for(;;)
    {
        temp8 = *PackBuf++;
        if(PackBuf > PackEnd || PackBuf == PackEnd)
            break;
        //return TRUE;
        switch(temp8)
        {
        case 0x04:
            PassThruConfig.Pin1 =  *PackBuf++; //+1
            PassThruConfig.Pin2 =  *PackBuf++; //+1
            PassThruConfig.LogicV1 =  *PackBuf++; //+1
            PassThruConfig.LogicV2 =  *PackBuf++; //+1
            break;
        case 0x08:
            PassThruConfig.CompV = *PackBuf++;
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
uint8 PassThru_WAVE_ReadWave(void)
{
	#if 0  //quwenqi2g
    //register uint32 iostatus,iobuf;
    uint8 LINE0_STATUS,LINE1_STATUS;
    uint32 tim_tmp2;
    EcuComRxBuf[0] = 0x00;
    EcuComRxBuf[1] = 0x06;
    EcuComRxBuf[2] = WAVE_DATA;
    //quwenqi ONLY_LINE0_RX_NORMAL_NO_LOAD;
	
	tx_MODE_EN_channel2(0xf,0x0,1,1);
	
    LINE0_STATUS=GET_RX0_STATUS;
    LINE1_STATUS=GET_LINE0_OR_LINE1;
	
    //quwenqi   Int_time(1,255);//(1000000);//定时1s
    while(1)
    {
        if((LINE0_STATUS!=GET_RX0_STATUS)||(LINE1_STATUS!=GET_LINE0_OR_LINE1))//有电平跳变
        {
            tim_tmp2=TIM0_TC;
            LINE0_STATUS=GET_RX0_STATUS;
            LINE1_STATUS=GET_LINE0_OR_LINE1;
            EcuComRxBuf[1]=(LINE0_STATUS<<1)+LINE1_STATUS;
            EcuComRxBuf[0]=0x04;
            EcuComRxBuf[2]=(tim_tmp2>>16)&0xff;
            EcuComRxBuf[3]=(tim_tmp2>>8)&0xff;
            EcuComRxBuf[4]=tim_tmp2&0xff;
			  //quwenqi 		USART_DMA_Send_ext(EcuComRxBuf_temp,5);
        }
         //quwenqi  if(TIME_INIT==0)
        {
            //quwenqi   Int_time(2,1000);
            LINE0_STATUS=GET_RX0_STATUS;
            LINE1_STATUS=GET_LINE0_OR_LINE1;
						EcuComRxBuf[0]=0x01;
						EcuComRxBuf[0]=(LINE0_STATUS<<1)+LINE1_STATUS;
					  //quwenqi USART_DMA_Send_ext(EcuComRxBuf,2);
        }
        if(ExitWaveFlag == EXIT_WAVE)
        {
            PassThruDriver.FuncBak0        = NULL;
            break;
        }
    }
		return TRUE;
		#endif
}

