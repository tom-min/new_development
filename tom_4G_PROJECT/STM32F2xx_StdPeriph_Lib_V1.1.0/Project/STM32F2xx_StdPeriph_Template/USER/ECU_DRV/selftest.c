/****************************************Copyright (c)****************************************************
**                            ShenZhen Fcar Co., LTD
**
**                                 http://www.szfcar.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           selftest.c
** Last modified Date:  2016-03-28
** Last Version:        V1.0
** Descriptions:        The selftest function example template
**
**--------------------------------------------------------------------------------------------------------
** Created by:          Chang YanLong
** Created date:        2016-03-28
** Version:             V1.00
** Descriptions:        所有自检以及设置程序
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
** Rechecked by:
*********************************************************************************************************/
 
#include <string.h>
#include "config.h"
#include "PassThru.h"
#include "interface.h"
#include "PortAssign.h"
#include "hardware.h"
#include "hardwaretest.h"
#include "usart1.h"
#include "PassThru_CAN.h"
#include "user_stm32f2xx.h"
extern uint8_t EcuComRxBuf[];
//extern uint8  EcuComRxBuf[ECURXBUFLEN];
//extern ComCtlType ComCtl;  //通讯控制
extern void tx_CTRL_EN_channel2(uint8_t JC_CHANNEL,uint8_t VALUE);
extern void tx_MODE_EN_channel2(uint8_t TXMODE,uint8_t RXMODE,uint8_t TX_REV,uint8_t RX_REV);
extern void tx_CTRL_EN_channel2_write(void);
extern PASSTHRU_CONFIG PassThruConfig;
extern PASSTHRU_DRIVER PassThruDriver;
extern void vcc_drv_v(void);


extern void UARTAnswerErr(uint8 cmd,uint8 para);
extern void printferror(const char *fmt,...);
extern uint32 TIME_INIT;
#define Stop_Time
#define Int_time
#define  NVIC_EnableIRQ


/*
 ** 开路短路 pin pin Test
 * a5 a5 00 fc 10 [上拉 1 / 下拉 0] [logic 0/1] pin1 pin2 
 * --------------------------------------------------------
 ** 注 上拉,0 表示上拉logic 0；9.5v(0v)表示开路9.5v，短路0v
 * 测试项  |  line0(R162) | line1(R189) |  status |
 * 上拉, 0 |   0.11v      |10.78v(0.15v)|   1(0)  |
 * 上拉, 1 |  11.01v      |10.78v(10.78)|   1(1)  |   测试有意义？
 * 下拉, 0 |   9.57v      | 0.00v(9.22v)|   1(0)  |
 * 下拉, 1 |   0.00v      | 0.00v(0.00v)|   1(1)  |   测试有意义？
 */
void TestPin(uint8 up_down, uint8 logic_0_1, uint8 *pins)
{
	uint16 ByteNum = 0;
	uint8 temp_line0=0;
	uint8 temp_line1=0;
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = 0xfc;
    EcuComRxBuf[ByteNum++] = 0x10;
	SetVcc_Drv(DRIVE_12V);
	SetCChannel(KL_BUS_CHANNEL);
	SetCompV(0x35);
	
	
	if (up_down) 
	{	/* logic 1 */
		//ONLY_LINE0_TX_NORMAL;
		//ONLY_LINE1_RX_NORMAL; 
		
		tx_MODE_EN_channel2(0,1,1,1);
		 
		//tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_HIGH);//LINE 0  PULL UP
		tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_HIGH);//LINE 1  PULL UP
		tx_CTRL_EN_channel2_write();				
		//tx_CTRL_EN_channel2(JC0_LINE0_PD,JC_HIGH);//LINE 0  PULL DOWN
		//tx_CTRL_EN_channel2(JC8_LINE1_PD,JC_HIGH);//LINE 1  PULL DOWN
		 
		//LINE1_UP;
		
		LINE0_OR_LINE1_HIGH;
	} 
	else 
	{
		//ONLY_LINE0_TX_REVERSE;
		//ONLY_LINE1_RX_REVERSE;
		tx_MODE_EN_channel2(0,1,0,0);
		
		//LINE0_NO_LOAD;
		tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_LOW);//LINE 0  PULL UP
		tx_CTRL_EN_channel2(JC2_LINE0_PD,JC_LOW);//LINE 0  PULL DOWN
		//LINE1_DOWN;
		tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_LOW);//LINE 1  PULL UP
		tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_HIGH);//LINE 1  PULL DOWN
        tx_CTRL_EN_channel2_write();
		LINE0_OR_LINE1_HIGH;
	}
	
	
	
	if (logic_0_1) 
	{ /* logic 1 */
	   	SET_LINE_STATUS(LOGIC_1);
	} else 
	{
	   	SET_LINE_STATUS(LOGIC_0);
	}
	DelayNms(100);
	
	EcuComRxBuf[ByteNum++] = pins[0];
	//  SetLine0Channel(pins[0]);
	temp_line0=pins[0];
	temp_line1=pins[1];
	//SetLine_ext(pins[0],0);
	EcuComRxBuf[ByteNum++] = pins[1];
	//SetLine1Channel(pins[1]);
	SetLine_ext(temp_line0,temp_line1);
	DelayNms(2);
	EcuComRxBuf[ByteNum++] = GET_LINE_STATUS();
	
	EcuComRxBuf[1]=ByteNum-2;
	UartTxQueuePack(EcuComRxBuf);
}

/*开路测试,测试是否有短路: 获取命令0xfc 00,不带接头自检
返回参数　　a5 a5 00 0f fc 00 81 82 83 86 87 88 89 8a 8b 8c 8d 8e 8f 05 
						a5 a5 00 0f fc 00 91 92 93 96 97 98 99 9a 9b 9c 9d 9e 9f 35 
						a5 a5 00 0f fc 00 a1 a2 a3 a6 a7 a8 a9 aa ab ac ad ae af 65 
						a5 a5 00 0f fc 00 b1 b2 b3 b6 b7 b8 b9 ba bb bc bd be bf 95 
*/
void TestShortCiruit()
{
	#if 1
	uint8 i, j;
	uint8 arrPin[13] = {1, 2, 3, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	uint16 ByteNum = 0;
	uint8 temp_line0=0;
	uint8 temp_line1=0;
	EcuComRxBuf[ByteNum++] = 0x00;
	EcuComRxBuf[ByteNum++] = 0x00;
	EcuComRxBuf[ByteNum++] = 0xfc;
	EcuComRxBuf[ByteNum++] = 0x00;
	SetVcc_Drv(DRIVE_12V);
	SetCChannel(KL_BUS_CHANNEL);
	SetCompV(0x35);
    //双通路上拉,逻辑0
    //LINE0_LINE1_TX_NORMAL;
	//ONLY_LINE0_TX_NORMAL;
	//ONLY_LINE1_RX_NORMAL;
	tx_MODE_EN_channel2(0,1,1,1);
	//LINE1_UP;
	tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_HIGH);//LINE 1  PULL UP
	tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_LOW );//LINE 1  PULL DOWN
	
	tx_CTRL_EN_channel2(JC1_BUS3_CAN_SW,JC_LOW);//CAN0 CAN_L AND CAN_H OFF
	
	tx_CTRL_EN_channel2_write();
	LINE0_OR_LINE1_HIGH;

SET_LINE_STATUS(LOGIC_0);
DelayNms(100);
    for(i=0; i<13; i++)
    {
		EcuComRxBuf[ByteNum++] = 0x80+arrPin[i];
		//SetLine0Channel(arrPin[i]);
		temp_line0=arrPin[i];
		SetLine_ext(temp_line0,temp_line1);
        for(j=0; j<13; j++) //此时判断其它脚应为高电平，只有ij相同时才会是低电平
        {
            //SetLine1Channel(arrPin[j]);
			temp_line1=arrPin[j];
			SetLine_ext(temp_line0,temp_line1);
			DelayNus(1000);
            if(i==j)
            {
                   if(GET_LINE_STATUS())
                    EcuComRxBuf[ByteNum++] = arrPin[j];
            }
            else
            {
                   if(GET_LINE_STATUS()==0)
                    EcuComRxBuf[ByteNum++] = arrPin[j];
            }
        }
    }
	EcuComRxBuf[1]=ByteNum-2;
	UartTxQueuePack(EcuComRxBuf);
	ByteNum=4;
	//双通路上拉,逻辑1
	SET_LINE_STATUS(LOGIC_1);
	DelayNms(100);
    for(i=0; i<13; i++)
    {
		//SetLine0Channel(arrPin[i]);
		temp_line0=arrPin[i];
		temp_line1=arrPin[i];
		EcuComRxBuf[ByteNum++] = 0x90+arrPin[i];
		//SetLine1Channel(arrPin[i]);
		//SetLine_ext(0,arrPin[j]);
		SetLine_ext(temp_line0,temp_line1);
		if(GET_LINE_STATUS()==0)
        {
            EcuComRxBuf[ByteNum++] = arrPin[i];
        }
    }

	EcuComRxBuf[1]=ByteNum-2;
	UartTxQueuePack(EcuComRxBuf);
	ByteNum=4;
   //双通道下拉,逻辑0
    //LINE0_LINE1_TX_REVERSE;
	
		//ONLY_LINE0_TX_REVERSE;
		//ONLY_LINE1_RX_REVERSE;
		tx_MODE_EN_channel2(0,1,0,0);
		//LINE0_NO_LOAD;
		tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_LOW);//LINE 0  PULL UP
		tx_CTRL_EN_channel2(JC2_LINE0_PD,JC_LOW);//LINE 0  PULL DOWN
		
		//LINE1_DOWN;
		tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_LOW);//LINE 1  PULL UP
		tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_HIGH);//LINE 1  PULL DOWN
		tx_CTRL_EN_channel2_write();
		LINE0_OR_LINE1_HIGH;
		
        SET_LINE_STATUS(LOGIC_0);
        DelayNms(100);

		temp_line0=0;
		temp_line1=0;
    for(i=0; i<13; i++)
    {
		EcuComRxBuf[ByteNum++] = 0xA0+arrPin[i];
		//SetLine0Channel(arrPin[i]);
		temp_line0=arrPin[i];
		SetLine_ext(temp_line0,temp_line1);
        for(j=0; j<13; j++)
        {
			//SetLine1Channel(arrPin[j]);
			temp_line1=arrPin[j];
			SetLine_ext(temp_line0,temp_line1);
			DelayNus(1000);
            if(i==j)
            {
                   if(GET_LINE_STATUS()==1)
                    EcuComRxBuf[ByteNum++] = arrPin[j];
            }
            else
            {
                   if(GET_LINE_STATUS()==0)
                    EcuComRxBuf[ByteNum++] = arrPin[j];
            }
        }
    }
	EcuComRxBuf[1]=ByteNum-2;
	UartTxQueuePack(EcuComRxBuf);
	ByteNum=4;

	//双通路下拉,逻辑1
	SET_LINE_STATUS(LOGIC_1);
	DelayNms(100);
    for(i=0; i<13; i++)
    {
		//SetLine0Channel(arrPin[i]);
		temp_line0=arrPin[i];
		temp_line1=arrPin[i];
		SetLine_ext(temp_line0,temp_line1);
		EcuComRxBuf[ByteNum++] = 0xB0+arrPin[i];
		//SetLine1Channel(arrPin[i]);
		//SetLine_ext(0,arrPin[j]);
		DelayNus(1000);
		if(GET_LINE_STATUS()==0)
		EcuComRxBuf[ByteNum++] = arrPin[i];
    }
    EcuComRxBuf[1]=ByteNum-2;
    UartTxQueuePack(EcuComRxBuf);
	#endif
}
//iobd短路测试
void TestOpen_iobd()
{
	#if 1
	uint8 i, j;
	uint8 arrPin1[6] = {1, 2, 3, 6, 7, 12};
	uint8 arrPin2[7]={8,9,10,11,13,14,15};
	uint16 ByteNum = 0;
	uint8 temp_line0=0;
	uint8 temp_line1=0;
	EcuComRxBuf[ByteNum++] = 0x00;
	EcuComRxBuf[ByteNum++] = 0x00;
	EcuComRxBuf[ByteNum++] = 0xfc;
	EcuComRxBuf[ByteNum++] = 0x01;
	SetVcc_Drv(DRIVE_12V);
	SetCChannel(KL_BUS_CHANNEL);
	SetCompV(0x35); //双通路上拉,逻辑0
	//LINE0_LINE1_TX_NORMAL;
	//ONLY_LINE0_TX_NORMAL;
	//ONLY_LINE1_RX_NORMAL;
	
	tx_MODE_EN_channel2(0,1,1,1);
	//LINE1_UP;
	tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_HIGH);//LINE 1  PULL UP
	tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_LOW); //LINE 1  PULL DOWN
	tx_CTRL_EN_channel2_write();
	LINE0_OR_LINE1_HIGH;
	SET_LINE_STATUS(LOGIC_0);
	DelayNms(100);
    for(i=0; i<6; i++)
    {
		EcuComRxBuf[ByteNum++] = 0x80+arrPin1[i];
		//SetLine0Channel(arrPin[i]);
		temp_line0=arrPin1[i];
		//SetLine_ext(temp_line0,temp_line1);
        for(j=0; j<7; j++) //全短路，所有脚为逻辑零
        {
			//SetLine1Channel(arrPin[j]);
			temp_line1=arrPin2[j];
			SetLine_ext(temp_line0,temp_line1);
			DelayNms(2);
						
			if(GET_LINE_STATUS()==1)
			EcuComRxBuf[ByteNum++] = arrPin2[j];
        }
    }
	EcuComRxBuf[1]=ByteNum-2;
	UartTxQueuePack(EcuComRxBuf);
	ByteNum=4;
	//双通路上拉,逻辑1
	SET_LINE_STATUS(LOGIC_1);
	DelayNms(100);
	temp_line0=0;
	temp_line1=0;
    for(i=0; i<6; i++)
    {
		EcuComRxBuf[ByteNum++] = 0x90+arrPin1[i];
		//SetLine0Channel(arrPin[i]);
		temp_line0=arrPin1[i];
		//SetLine_ext(temp_line0,temp_line1);
        for(j=0; j<7; j++) //全短路，所有脚为逻辑一
        {
			//SetLine1Channel(arrPin[j]);
			temp_line1=arrPin2[j];
			SetLine_ext(temp_line0,temp_line1);
			DelayNms(2);

			if(GET_LINE_STATUS()==0)
			EcuComRxBuf[ByteNum++] = arrPin2[j];
        }
    }

	EcuComRxBuf[1]=ByteNum-2;
	UartTxQueuePack(EcuComRxBuf);
	ByteNum=4;
    //双通道下拉,逻辑0
    //LINE0_LINE1_TX_REVERSE;

	//ONLY_LINE0_TX_REVERSE;
	//ONLY_LINE1_RX_REVERSE;
	tx_MODE_EN_channel2(0,1,0,0);
	//LINE0_NO_LOAD;
	tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_LOW);//LINE 0  PULL UP
	tx_CTRL_EN_channel2(JC2_LINE0_PD,JC_LOW);//LINE 0  PULL DOWN
	//LINE1_DOWN;
	tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_LOW);//LINE 1  PULL UP
	tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_HIGH);//LINE 1  PULL DOWN
	tx_CTRL_EN_channel2_write();
	LINE0_OR_LINE1_HIGH;
		 
		
	SET_LINE_STATUS(LOGIC_0);
	DelayNms(100);
	temp_line0=0;
	temp_line1=0;
		
    for(i=0; i<6; i++)
    {
		EcuComRxBuf[ByteNum++] = 0xa0+arrPin1[i];
		//SetLine0Channel(arrPin[i]);
		temp_line0=arrPin1[i];
		//SetLine_ext(temp_line0,temp_line1);
        for(j=0; j<7; j++) ////全短路，所有脚为逻辑零
        {
			//SetLine1Channel(arrPin[j]);
			temp_line1=arrPin2[j];
			 SetLine_ext(temp_line0,temp_line1);
			DelayNms(2);
			if(GET_LINE_STATUS()==1)
			EcuComRxBuf[ByteNum++] = arrPin2[j];
					
        }
    }
	EcuComRxBuf[1]=ByteNum-2;
	UartTxQueuePack(EcuComRxBuf);
	ByteNum=4;
	temp_line0=0;
	temp_line1=0;

    //双通路下拉,逻辑1
    SET_LINE_STATUS(LOGIC_1);
    DelayNms(100);
    for(i=0; i<6; i++)
    {
		EcuComRxBuf[ByteNum++] = 0xb0+arrPin1[i];
		//SetLine0Channel(arrPin[i]);
		temp_line0=arrPin1[i];
	    SetLine_ext(temp_line0,temp_line1);
		for(j=0; j<7; j++) ////全短路，所有脚为逻辑一
		{
			//SetLine1Channel(arrPin[j]);
			temp_line1=arrPin2[j];
			SetLine_ext(temp_line0,temp_line1);
			DelayNms(2);

			if(GET_LINE_STATUS()==0)
			EcuComRxBuf[ByteNum++] = arrPin2[j];
        }
    }
    EcuComRxBuf[1]=ByteNum-2;
    UartTxQueuePack(EcuComRxBuf);
#endif

}
//短路测试，测试是否有开路: 获取命令0xfc 0x01 带接头自检
void TestOpenCiruit()
{
	#if 1
	uint8 i, j;
	uint8 arrPin[13] = {1, 2, 3, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	uint16 ByteNum = 0;
	uint8 temp_line0=0;
	uint8 temp_line1=0;
	EcuComRxBuf[ByteNum++] = 0x00;
	EcuComRxBuf[ByteNum++] = 0x00;
	EcuComRxBuf[ByteNum++] = 0xfc;
	EcuComRxBuf[ByteNum++] = 0x01;
	SetVcc_Drv(DRIVE_12V);
	SetCChannel(KL_BUS_CHANNEL);
	SetCompV(0x35);
	//双通路上拉,逻辑0
	//  LINE0_LINE1_TX_NORMAL;

	ONLY_LINE0_TX_NORMAL;
	ONLY_LINE1_RX_NORMAL;
	LINE1_UP;
	LINE0_OR_LINE1_HIGH;
		
	tx_MODE_EN_channel2(0,1,1,1);
	//LINE1_UP;
	tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_HIGH);//LINE 1  PULL UP
	tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_LOW);//LINE 1  PULL DOWN
	tx_CTRL_EN_channel2_write();
	LINE0_OR_LINE1_HIGH;

	SET_LINE_STATUS(LOGIC_0);
	DelayNms(100);
    for(i=0; i<13; i++)
    {
		EcuComRxBuf[ByteNum++] = 0x80+arrPin[i];
		//SetLine0Channel(arrPin[i]);
		temp_line0=arrPin[i];
		 SetLine_ext(temp_line0,temp_line1);
        for(j=0; j<13; j++) //全短路，所有脚为逻辑零
        {
			//SetLine1Channel(arrPin[j]);
			temp_line1=arrPin[j];
			SetLine_ext(temp_line0,temp_line1);
			DelayNms(2);
						
			if(GET_LINE_STATUS()==1)
			EcuComRxBuf[ByteNum++] = arrPin[j];
        }
    }
    EcuComRxBuf[1]=ByteNum-2;
    UartTxQueuePack(EcuComRxBuf);
    ByteNum=4;
	//双通路上拉,逻辑1
	SET_LINE_STATUS(LOGIC_1);
	DelayNms(100);
	temp_line0=0;
	temp_line1=0;
    for(i=0; i<13; i++)
    {
		EcuComRxBuf[ByteNum++] = 0x90+arrPin[i];
		//SetLine0Channel(arrPin[i]);
		temp_line0=arrPin[i];
		SetLine_ext(temp_line0,temp_line1);
        for(j=0; j<13; j++) //全短路，所有脚为逻辑一
        {
			//SetLine1Channel(arrPin[j]);
			temp_line1=arrPin[j];
			SetLine_ext(temp_line0,temp_line1);
			DelayNms(2);

			//if(GET_LINE_STATUS()==0)
			//EcuComRxBuf[ByteNum++] = arrPin[j];
        }
    }

	EcuComRxBuf[1]=ByteNum-2;
	UartTxQueuePack(EcuComRxBuf);
	ByteNum=4;
	//双通道下拉,逻辑0
	//LINE0_LINE1_TX_REVERSE;
	ONLY_LINE0_TX_REVERSE;
	ONLY_LINE1_RX_REVERSE;
	LINE0_NO_LOAD;
	LINE1_DOWN;
	LINE0_OR_LINE1_HIGH;
	tx_MODE_EN_channel2(0,1,0,0);
	//LINE0_NO_LOAD;
	tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_LOW);//LINE 0  PULL UP
	tx_CTRL_EN_channel2(JC2_LINE0_PD,JC_LOW);//LINE 0  PULL DOWN
	//LINE1_DOWN;
	tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_LOW);//LINE 1  PULL UP
	tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_HIGH);//LINE 1  PULL DOWN
	tx_CTRL_EN_channel2_write();
	LINE0_OR_LINE1_HIGH;
		
	 
	SET_LINE_STATUS(LOGIC_0);
	DelayNms(100);
	temp_line0=0;
	temp_line1=0;
		
    for(i=0; i<13; i++)
    {
		EcuComRxBuf[ByteNum++] = 0xa0+arrPin[i];
		//SetLine0Channel(arrPin[i]);
		temp_line0=arrPin[i];
		 SetLine_ext(temp_line0,temp_line1);
        for(j=0; j<13; j++) ////全短路，所有脚为逻辑零
        {
			//SetLine1Channel(arrPin[j]);
			temp_line1=arrPin[j];
		    SetLine_ext(temp_line0,temp_line1);
			DelayNms(2);
			//if(GET_LINE_STATUS()==1)
			//EcuComRxBuf[ByteNum++] = arrPin[j];
					
        }
    }
	EcuComRxBuf[1]=ByteNum-2;
	UartTxQueuePack(EcuComRxBuf);
	ByteNum=4;
	temp_line0=0;
	temp_line1=0;

	//双通路下拉,逻辑1
	SET_LINE_STATUS(LOGIC_1);
	DelayNms(100);
    for(i=0; i<13; i++)
    {
		EcuComRxBuf[ByteNum++] = 0xb0+arrPin[i];
		//SetLine0Channel(arrPin[i]);
		temp_line0=arrPin[i];
		 SetLine_ext(temp_line0,temp_line1);
        for(j=0; j<13; j++) ////全短路，所有脚为逻辑一
        {
			//SetLine1Channel(arrPin[j]);
			temp_line1=arrPin[j];
			 SetLine_ext(temp_line0,temp_line1);
			DelayNms(2);

			//if(GET_LINE_STATUS()==0)
			//EcuComRxBuf[ByteNum++] = arrPin[j];
        }
    }
	EcuComRxBuf[1]=ByteNum-2;
	UartTxQueuePack(EcuComRxBuf);
	 #endif
}
/***********************
入口参数， 	InstPack+4: 0:LINE0 1:LINE1
						InstPack+5: JB0~JB12 ((ja0~ja12)
						InstPack+6: 0:NO_LOAD 1:UP_LOAD 2:DOWN_LOAD
 吸合相应的干簧管，并按要求上拉下拉或悬空
***********************/

void setPortStatus(uint8 *InstPack)
{
	uint8 temp_line0=0;
	uint8 temp_line1=0;
	tx_CTRL_EN_channel2(JC1_BUS3_CAN_SW,JC_LOW);//CAN0 CAN_L AND CAN_H OFF
    if(InstPack[4]==0)
    {
		//SetLine0Channel(InstPack[5]);
		temp_line0=InstPack[5];
		SetLine_ext(temp_line0,temp_line1);
		SetEcuLineLoadMode(LINE0, InstPack[6]);
    }
    else
    {
		//SetLine1Channel(InstPack[5]);
		temp_line1=InstPack[5];
		SetLine_ext(temp_line0,temp_line1);
		SetEcuLineLoadMode(LINE1, InstPack[6]);
    }
}

/***********************

入口参数， 	InstPack+4: 0:LINE0 1:LINE1
			InstPack+5: JB0~JB12(ja0~ja12)
吸合相应的干簧管，并按要求上拉下拉或悬空
出口参数, EcuComRxBuf[4] 0:line0 1:line1
EcuComRxBuf[5]*256+EcuComRxBuf[6] 线上电压
EcuComRxBuf[7] 线上电压状态
***********************/
void getPortStatus(uint8 *InstPack)
{
	#if 0  //quwenqi2g
#if 1 
	uint32 ad;
	uint8 ByteNum = 0;
	uint8 temp_line0=0;
	uint8 temp_line1=0;
	uint32 ad_temp=0;
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = 0xfc;
    EcuComRxBuf[ByteNum++] = InstPack[3];
    EcuComRxBuf[ByteNum++] = InstPack[4];
	tx_CTRL_EN_channel2(JC1_BUS3_CAN_SW,JC_LOW);//CAN0 CAN_L AND CAN_H OFF
    if(InstPack[4]==0)
    {
		//ONLY_LINE0_RX_NORMAL_NO_LOAD;
		
		tx_MODE_EN_channel2(0xf,0x0,NORMAL,NORMAL);
		
		//SetLine0Channel(InstPack[5]);
		temp_line0=InstPack[5];
		SetLine_ext(temp_line0,temp_line1);
		SetEcuLineLoadMode(InstPack[4], NO_LOAD);

		ad=Read_Dac(mux0_vcc);
		DelayNms(10);
		ad=Read_Dac(mux0_vcc)/100;
		 
    }
    else
    {
		//ONLY_LINE1_RX_NORMAL_NO_LOAD;
		tx_MODE_EN_channel2(0xf,0x01,1,1);
		//SetLine1Channel(InstPack[5]);
		temp_line1=InstPack[5];
		SetLine_ext(temp_line0,temp_line1);
		SetEcuLineLoadMode(InstPack[4], NO_LOAD);
		ad=Read_Dac(mux1_vcc);
		DelayNms(10);
		ad=Read_Dac(mux0_vcc)/100;
    }
	
	if(ad < 6) ad = 0;//电压小于0.6 强置高为0;
	
	if((ad < 16)&&(ad > 13))    ad += 3;//can 线14 脚实际测试偏低1.4v-1.5v;
	if((ad <= 100)&&(ad > 91))  ad += 12;//k 线 电压偏低1.4v-1.5v;
	if((ad <= 105)&&(ad > 100)) ad += 6;//k 线 电压偏低偏低1.4v-1.5v;
	if((ad <= 107)&&(ad > 105)) ad += 4;//k 线 电压偏低偏低1.4v-1.5v;
	
	#if 0	 
	if((ad <= 230)&&(ad > 150)&&(vcc_drv!=0))  
	ad = (vcc_drv*95)/100;//k 线 电压偏低1.4v-1.5v;
	#endif
	#if 1
	if((ad > 70)&&(vcc_drv != 0))
	{
		ad_temp=(ad*119)/100;
		if(ad_temp < vcc_drv)
		{
			ad = ad_temp;
		}
		else
		{
			ad = vcc_drv - 5;
		}
	}
	#endif
	
    EcuComRxBuf[ByteNum++]=(ad>>8)&0xff;
    EcuComRxBuf[ByteNum++]=ad&0xff;
    if(GET_RX0_STATUS)
        EcuComRxBuf[ByteNum++] = 0x01;
    else
        EcuComRxBuf[ByteNum++] = 0x00;

    ByteNum -= 2;
    EcuComRxBuf[0] = ByteNum >> 8;
    EcuComRxBuf[1] = ByteNum & 0xff;
    UartTxQueuePack(EcuComRxBuf);
#endif
#endif
	}

/********************************************************************************************
	名称:	PassThru_CAN_SELFTEST
	获取命令:0xfc 0x04
  功能: 用自发自收模式判断，can收发是否正常
	selfmode :0 can单接点自检
            1 can接头是否接到can网络上(同样波特率，)
	pin:测试对应can引脚
  canbuad:can波特率
  set_res:0　can之间电阻接通，1　can两引脚之间无电阻
	入口参数:
********************************************************************************************/
void PassThru_CAN_SELFTEST(uint8 selfmode,uint8 pin,uint32 canbaud,uint8 set_res)
{
//4 5 6为波特率
//10为参数
//46为引脚号
//48为是否接通60欧电阻设置　
	uint8  cantest[60]={0x00,0x2f,0x10,0x01,0x07,0xa1,0x20,0x02,0x81,0x03,
						0x21,0x04,0x00,0x00,0x07,0xe0,0x05,0x00,0x00,0x07,
						0xea,0x06,0x00,0x00,0x07,0xff,0x07,0x00,0x00,0x07,
						0xe1,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,
						0x00,0x00,0x0b,0x00,0x00,0x0c,0xcd,0x10,0x01,0x97,};
	uint8 cancmd[20]= {0x00,0x0d,0x30,0x00,0x00,0x07,0xe0,0x07,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0xb8};

	  cantest[4]=(canbaud>>16)&0xff;
		cantest[5]=(canbaud>>8)&0xff;
		cantest[6]=canbaud&0xff;
		cantest[10]&=0x9f;
		switch(selfmode)
		{
			case 0://单点can自检，引脚不需接通
				cantest[10]|=0x20;
				break;
			case 1://检测是否接入正常can网络
				cantest[10]|=0x40;
				break;
			default:
				break;
		}
		cantest[46]=pin;
		cantest[48]=set_res;
		PassThruConfig.Send_Pass=0;
#if 0
    PassThru_CAN_SetConfig(cantest,CHANEL0);
    DelayNms(10);
    PassThru_CAN_InitPort(CHANEL0);
    DelayNms(100);
    //quweqni NVIC_EnableIRQ(CAN_IRQn);
    can_test_status=1;
    PassThru_CAN_TxPack_Normal(cancmd);
	
	Int_time(2,50);

    for(;;)
    {
        if(can_test_status==0)
        {
            Stop_Time(0);//LPC_TIM1->TCR  = 0x00;
            break;
        }
        if(TIME_INIT== RESET)						//延时到，未产生自接收，中断
        {
            UARTAnswerErr(0xfc,0);					//can自检
            can_test_status=1;
             Stop_Time(0);//LPC_TIM1->TCR  = 0x00;  //关定时器
            break;
        }
    }
#endif
}
/*
		驱动电压测试	
		获取命令:0xfc 0x05
*/
extern  HARDWARETEST_VCC_DRV_SERVICE_FUNCTION(uint32_t VCC_DRV);
void Drv_Test(void)
{
	
	 
    uint32 ad;
    EcuComRxBuf[0] = 0;
    EcuComRxBuf[1] = 0X06;
    EcuComRxBuf[2] = 0xfc	;
    EcuComRxBuf[3] = 0x06	;
    SetVcc_Drv(DRIVE_12V);
    DelayNms(200);
    ad=HARDWARETEST_VCC_DRV_SERVICE_FUNCTION(12);
    DelayNms(10);
    ad=HARDWARETEST_VCC_DRV_SERVICE_FUNCTION(12);
    EcuComRxBuf[4] = ad/100;
    SetVcc_Drv(DRIVE_8V);
    DelayNms(200);
    ad=HARDWARETEST_VCC_DRV_SERVICE_FUNCTION(8);
    DelayNms(10);
    ad=HARDWARETEST_VCC_DRV_SERVICE_FUNCTION(8);
	EcuComRxBuf[5]=ad/100;
    SetVcc_Drv(DRIVE_5V);
    DelayNms(200);
    ad=HARDWARETEST_VCC_DRV_SERVICE_FUNCTION(5);
    DelayNms(20);
    ad=HARDWARETEST_VCC_DRV_SERVICE_FUNCTION(5);
    DelayNms(10);
	ad=HARDWARETEST_VCC_DRV_SERVICE_FUNCTION(5);
	EcuComRxBuf[6]=ad/100;
	ad=Read_Dac(ver_vcc);
	DelayNms(10);
	ad=Read_Dac(ver_vcc);
	EcuComRxBuf[7]=ad/1100;//在读取电压时按原来的方法直接扩大了11倍,所以此处直接缩小 
	UartTxQueuePack(EcuComRxBuf);
	 
}

void Circle_test(void)
{
	 
	uint8 i,ByteNum=6;
	uint8 arrPin[13] = {1, 2, 3, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	uint8 temp_line0=0;
	uint8 temp_line1=0;
	
	tx_CTRL_EN_channel2(JC1_BUS3_CAN_SW,JC_LOW);//CAN0 CAN_L AND CAN_H OFF
    for(i=0; i<13; i++)
    {
		//EcuComRxBuf[ByteNum++] = 0x80+arrPin[i];
		//SetLine0Channel(arrPin[i]);
		//SetLine1Channel(arrPin[i]);
		temp_line0=arrPin[i];
		temp_line1=arrPin[i];
		SetLine_ext(temp_line0,temp_line1);
		Read_Dac(mux0_vcc);
		DelayNms(5);
		//DelayNms(10);
		Read_Dac(mux1_vcc);
		//DelayNms(10);
		DelayNms(5);
		EcuComRxBuf[ByteNum++]=Read_Dac(mux0_vcc)/100;; //直接算成0.1单位
		//EcuComRxBuf[ByteNum++]=CH0_SPD_RETURN();
    }
		switch(EcuComRxBuf[4])
		{
			case 0x05:
			case 0x06:
			case 0x07:
			case 0x08:
				EcuComRxBuf[5]=(GET_LINE_STATUS()+1)&0x01;
				break;
			case 0x09:
			case 0x0a:
				EcuComRxBuf[5]=0xff;
				break;
			default:
				EcuComRxBuf[5]=GET_LINE_STATUS();
				break;
		}
    EcuComRxBuf[1]=ByteNum-2;
    UartTxQueuePack(EcuComRxBuf);
	 
}
/********************
测试上下拉以及干簧管通断
byte6  	01 :line1上拉，line0悬空,line1输出逻辑0 
				02 :line0上拉，line1悬空,line0输出逻辑0
				03：line1上拉，line0悬空，line1输出逻辑1
				04：line0上拉，line1悬空，line0输出逻辑1
				05：line1下拉，line0悬空，line1输出逻辑0
				06：line0下拉，line1悬空，line0输出逻辑0
				07：line1下拉，line0悬空，line1输出逻辑1
				08：line0下拉，line1悬空，line0输出逻辑1
				09：LINE0上拉，LINE1下拉
				0A：LINE1上拉，LINE0下拉

获取命令 0xfc 0x07
********************/
void up_down_test(void)
{
//     uint16 ByteNum=0;
//     uint8 arrPin[13] = {1, 2, 3, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    //chanyl Int_Dac_PortV();
    //Int_Dac_BatV();
    EcuComRxBuf[0] = 0x00;
    EcuComRxBuf[1] = 0x00;
    EcuComRxBuf[2] = 0xfc;
    EcuComRxBuf[3] = 0x07;
	EcuComRxBuf[5]=0x00;
	#if 0  //quwenqi2g
	arch_ioport_set_pin_dir(PIO_PA22_IDX,IOPORT_DIR_OUTPUT);    //KWP OUTPUT PIN UART发送 LINE1 L
	 
	arch_ioport_set_pin_dir(PIO_PC16_IDX,IOPORT_DIR_OUTPUT);    //KWP OUTPUT PIN UART发送 LINE1 L
	#endif
	SetCChannel(C_CHANNEL_ALL_OFF);
	SetVcc_Drv(DRIVE_12V);
	SetCChannel(KL_BUS_CHANNEL);
	SetCompV(0x35);
//逻辑0
    EcuComRxBuf[4]=0x01;

	LINE0_LINE1_TX_NORMAL;//ONLY_LINE1_TX_NORMAL;
	ONLY_LINE0_RX_NORMAL;
	
	tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_HIGH );//LINE 1  PULL UP
	tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_LOW);//LINE 1  PULL DOWN
	tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_HIGH );//LINE 1  PULL UP
	tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_LOW);//LINE 1  PULL DOWN
	tx_CTRL_EN_channel2_write();
	
	tx_MODE_EN_channel2(2,0,NORMAL,NORMAL);
	SET_LINE_STATUS(LOGIC_0);//line1输出逻辑0
	DelayNms(100);

	Circle_test();
	EcuComRxBuf[4]=0x02;
	LINE0_LINE1_TX_NORMAL;//ONLY_LINE0_TX_NORMAL;
	ONLY_LINE1_RX_NORMAL;
	
	tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_HIGH );//LINE 1  PULL UP
	tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_LOW);//LINE 1  PULL DOWN
	tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_HIGH );//LINE 1  PULL UP
	tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_LOW);//LINE 1  PULL DOWN
	
	tx_CTRL_EN_channel2_write();
	tx_MODE_EN_channel2(2,1,NORMAL,NORMAL);
	SET_LINE_STATUS(LOGIC_0);//line0输出逻辑0
	DelayNms(100);
	Circle_test();
    //逻辑1
	EcuComRxBuf[4]=0x03;
	
	
	
	LINE0_LINE1_TX_NORMAL;//ONLY_LINE1_TX_NORMAL;
	ONLY_LINE0_RX_NORMAL;
	tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_HIGH);//LINE 1  PULL UP
	tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_LOW);//LINE 1  PULL DOWN
	tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_HIGH);//LINE 0  PULL UP
	tx_CTRL_EN_channel2(JC2_LINE0_PD,JC_LOW);//LINE 0  PULL DOWN
	tx_CTRL_EN_channel2_write();
	tx_MODE_EN_channel2(2,0,NORMAL,NORMAL);
	SET_LINE_STATUS(LOGIC_1);//line1输出逻辑1
	DelayNms(100);
	Circle_test();
	//	while(1);
	EcuComRxBuf[4]=0x04;
	LINE0_LINE1_TX_NORMAL;//ONLY_LINE0_TX_NORMAL;
	ONLY_LINE1_RX_NORMAL;
	tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_HIGH);//LINE 1  PULL UP
	tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_LOW);//LINE 1  PULL DOWN
	tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_HIGH);//LINE 0  PULL UP
	tx_CTRL_EN_channel2(JC2_LINE0_PD,JC_LOW);//LINE 0  PULL DOWN
	tx_CTRL_EN_channel2_write();
	tx_MODE_EN_channel2(2,1,NORMAL,NORMAL);
	SET_LINE_STATUS(LOGIC_1);//line0输出逻辑1
	DelayNms(100);
	Circle_test();
    //while(1);
    //双通道下拉，LINE0逻辑0

    EcuComRxBuf[4]=0x05;
	LINE0_LINE1_TX_REVERSE;//ONLY_LINE1_TX_REVERSE;
	ONLY_LINE0_RX_REVERSE;
	
	tx_MODE_EN_channel2(2,0,REVERSE,REVERSE);
	SET_LINE_STATUS(LOGIC_0);//line1输出逻辑0
    DelayNms(100);
    Circle_test();
    EcuComRxBuf[4]=0x06;
	LINE0_LINE1_TX_REVERSE;//ONLY_LINE0_TX_REVERSE;
	ONLY_LINE1_RX_REVERSE;
	tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_LOW);//LINE 1  PULL UP
	tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_HIGH);//LINE 1  PULL DOWN
	tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_LOW);//LINE 0  PULL UP
	tx_CTRL_EN_channel2(JC2_LINE0_PD,JC_HIGH);//LINE 0  PULL DOWN
	tx_CTRL_EN_channel2_write();
	tx_MODE_EN_channel2(2,1,REVERSE,REVERSE);
	
	
	SET_LINE_STATUS(LOGIC_0);//line0输出逻辑0
    DelayNms(100);
    Circle_test();
    //LINE0逻辑1
    //while(1);
	
	
	
    EcuComRxBuf[4]=0x07;
	LINE0_LINE1_TX_REVERSE;//LINE0_LINE1_TX_NORMAL;//ONLY_LINE1_TX_REVERSE;
	ONLY_LINE0_RX_REVERSE;
	tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_LOW);//LINE 1  PULL UP
	tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_HIGH);//LINE 1  PULL DOWN
	tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_LOW);//LINE 0  PULL UP
	tx_CTRL_EN_channel2(JC2_LINE0_PD,JC_HIGH);//LINE 0  PULL DOWN
	tx_CTRL_EN_channel2_write();	
	tx_MODE_EN_channel2(2,0,REVERSE,REVERSE);
	
    SET_LINE_STATUS(LOGIC_1);//line1输出逻辑1
    DelayNms(100);
    Circle_test();
    EcuComRxBuf[4]=0x08;
	LINE0_LINE1_TX_REVERSE;//ONLY_LINE0_TX_REVERSE;
	ONLY_LINE1_RX_REVERSE;
	
	tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_LOW);//LINE 1  PULL UP
	tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_HIGH);//LINE 1  PULL DOWN
	tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_LOW);//LINE 0  PULL UP
	tx_CTRL_EN_channel2(JC2_LINE0_PD,JC_HIGH);//LINE 0  PULL DOWN
	tx_CTRL_EN_channel2_write();
	
		
	tx_MODE_EN_channel2(2,1,REVERSE,REVERSE);
	
    SET_LINE_STATUS(LOGIC_1);//line0输出逻辑0
    DelayNms(100);
    Circle_test();
    //line0上拉，line1下拉,LINE0逻辑0
   // SetEcuLineLoadMode(LINE0, UP_LOAD);
    //SetEcuLineLoadMode(LINE1, DOWN_LOAD);
    EcuComRxBuf[4]=0x09;
    EcuComRxBuf[5]=0xff;
	ONLY_LINE0_TX_NORMAL;
	LINE1_DOWN;
	
	tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_HIGH);//LINE 0  PULL UP
	tx_CTRL_EN_channel2(JC2_LINE0_PD,JC_LOW);//LINE 0  PULL DOWN		
	tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_LOW);//LINE 1  PULL UP
	tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_HIGH);//LINE 1  PULL DOWN
		
	tx_MODE_EN_channel2(0,0,NORMAL,NORMAL);
	tx_CTRL_EN_channel2_write();
	
		//quwenqi tx_c_channel();
    SET_LINE_STATUS(LOGIC_1);//line0输出逻辑1
		
    DelayNms(100);
    Circle_test();
    //line0下拉，line1上拉,LINE1逻辑0
    EcuComRxBuf[4]=0x0a;
    EcuComRxBuf[5]=0xff;
	ONLY_LINE1_TX_NORMAL;
	LINE0_DOWN;
	tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_HIGH);//LINE 1  PULL UP
	tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_LOW);//LINE 1  PULL DOWN
	
	tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_LOW);//LINE 0  PULL UP
	tx_CTRL_EN_channel2(JC2_LINE0_PD,JC_HIGH);//LINE 0  PULL DOWN
	tx_CTRL_EN_channel2_write();			
	tx_MODE_EN_channel2(1,0,NORMAL,NORMAL);
		
		
		//quwenqi tx_c_channel();
    SET_LINE_STATUS(LOGIC_1);
   // SET_TX_PRE_IO_LOGIC(LOGIC_1);//line1输出逻辑1
///	EcuComRxBuf[6]=0xff;
    //CH1_SPD_LOGIC0();
    DelayNms(100);
    Circle_test();

}
//比较电压设置及测试，不须上位机直接一条命令所有测试完成
//获取命令 0xfc 0x06

/*********************************
测试比较电压，从0.1v测到9.9v
************************************/
//extern void hal_init_pwm_CMP_Volt(void);
//extern void hal_SET_CMP_Volt(float Volt);

void Cmp_Test(void)
{
	

	uint32 ad;
	uint32	i,j;
	uint16 ByteNum=0;
	unsigned int data_voltage;
	Reset_All_IO();
	/*EcuComRxBuf[0] = 0;
	EcuComRxBuf[1] = 0XC7;
	EcuComRxBuf[2] = 0xfc	;*/
	SetCompV(1);

	DelayNms(200);
	data_voltage=Read_Dac(cmp_vcc);
	EcuComRxBuf[ByteNum++] = 0x00;
	EcuComRxBuf[ByteNum++] = 0x00;
	EcuComRxBuf[ByteNum++] = 0xfc;
	EcuComRxBuf[ByteNum++] = 0x06;
    //i=10;
	Bus1CmpAndDrvAdcInit();
	//hal_init_pwm_CMP_Volt();
	//vTaskDelay(E70B_vTaskDelay_Modulus(20));
    for(i=1; i<=100; i++)
    {
		j=i/10*16+(i%10);
		SetCompV(j);
		DelayNms(3);
		ad=Read_Dac(cmp_vcc);
		if(i!=25)
		{
			EcuComRxBuf[ByteNum++]=i*100/100;
		}
		
		else
		{
			if((ad/100)<(25+4)&&(ad/100)>(25-4))
			EcuComRxBuf[ByteNum++]=25;
		}
    }
   
    ByteNum-=2;
    EcuComRxBuf[0] = ByteNum >> 8;
    EcuComRxBuf[1] = ByteNum & 0xff;
    UartTxQueuePack(EcuComRxBuf);

}
//kl线和can信号切换继电器测试
//测试kl和can的继电器，原理通过切换到 can线后测试can电压
//测试命令0xfc 0x0a
void test_kl_can(void)
{

	uint16 ByteNum=0;
	EcuComRxBuf[ByteNum++] = 0x00;
	EcuComRxBuf[ByteNum++] = 0x00;
	EcuComRxBuf[ByteNum++] = 0xfc;
	EcuComRxBuf[ByteNum++] = 0x0A;
	SetCChannel(C_CHANNEL_ALL_OFF);
	//	SetCChannel(ANALOG_CHANNEL_ON);//测试mux1电压
	SetCChannel(CAN_BUS_CHANNEL);
	
		tx_CTRL_EN_channel2(JC4_CAN_SW0,JC_HIGH);//CAN2 CAN_L AND CAN_H
		tx_CTRL_EN_channel2(JC5_CAN_SW1,JC_LOW);//CAN2 CAN_L AND CAN_H
	     tx_CTRL_EN_channel2_write();
	// 		SetLine0Channel(0x06);
	// 		SetLine1Channel(0x0e);
	SetLine_ext(0x06,0x0e);
	DelayNms(150);
	Read_Dac(mux0_vcc);
	DelayNms(10);
	EcuComRxBuf[ByteNum++]=Read_Dac(mux0_vcc)/100;
	ByteNum-=2;
	EcuComRxBuf[0] = ByteNum >> 8;
	EcuComRxBuf[1] = ByteNum & 0xff;
	UartTxQueuePack(EcuComRxBuf);
	//Reset_All_IO();

 }
/*kL线上设置方波测试
  测试命令0xfc 0x0x0b
  buf[6] line0引脚
　buf[7] line1引脚
  buf[8] 驱动电压设置
  buf[9] line0逻辑状态
  buf[10] line1逻辑状态
  buf[11] 比较电压
  buf[12 13 14] 逻辑反转时长（ms）
*/
void kl_setup(uint8 * buf)
{

    uint16 ByteNum=0;
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = 0xfc;
    EcuComRxBuf[ByteNum++] = 0x0b;
    SetCChannel(C_CHANNEL_ALL_OFF);
    SetCChannel(KL_BUS_CHANNEL);
		Reset_All_IO();
    //接通引脚
//     SetLine0Channel(buf[6]);
//     SetLine1Channel(buf[7]);
	  SetLine_ext(buf[6],buf[7]);
    SetVcc_Drv(buf[8]);//设置电压
    SetCompV(buf[11]);//设置比较电压
    DelayNms(200);
    Read_Dac(drv_vcc);
    DelayNms(10); //
    EcuComRxBuf[ByteNum++]=Read_Dac(drv_vcc)/100; //驱动电压
    DelayNms(100);
    Read_Dac(cmp_vcc);
    DelayNms(10);
    EcuComRxBuf[ByteNum++]=Read_Dac(cmp_vcc)/100;//比较电压
    if((buf[13]!=0)||(buf[14]!=0))//if((buf[12]!=0)||(buf[13]!=0)||(buf[14]!=0))//如果有有数据，表示要发脉冲波形
    {
		Int_time(0,((buf[12]<<16)+(buf[13]<<8)+buf[14])*Fpclk/1000000);
		//kl_set_time=(buf[12]<<16)+(buf[13]<<8)+buf[14];
			
		kl_set[0]=buf[4];
		kl_set[1]=buf[9];
		kl_set[2]=buf[10];
		kl_set[3]=0x55;
    }

    switch(buf[4])
    {
    case 0://line0上拉
        ONLY_LINE0_TX_NORMAL;
        SET_LINE_STATUS(buf[9]);
        break;
    case 1://line0下拉
        ONLY_LINE0_TX_REVERSE;
        SET_LINE_STATUS(buf[9]);
        //ComCtl.ChannelDriveMode |= STRONG_PULL_UP;
        //SET_TX00_AND_TX01_LOW;
        //SetCChannel(PD0_DEN);
        break;
    case 2://line1上拉上拉
        ONLY_LINE1_TX_NORMAL;
        SET_LINE_STATUS(buf[9]);
        break;
    case 3://LINE1下拉
        ONLY_LINE1_TX_REVERSE;
        SET_LINE_STATUS(buf[9]);
        break;
    case 4:
        LINE0_LINE1_TX_NORMAL;
        SET_LINE_STATUS(buf[9]);
        break;
    case 5:
        LINE0_LINE1_TX_REVERSE;
        SET_LINE_STATUS(buf[9]);
        break;
    case 6://差分
        DIFFERENTIAL_TX_LINE0DOWN;
        SET_LINE_STATUS(buf[9]);
        break;
    case 7:
        DIFFERENTIAL_TX_LINE0UP;
        SET_LINE_STATUS(buf[9]);
        break;
    case 8:
        LINE0_TX_LINE1_GND_NORMAL;
        SET_LINE_STATUS(buf[9]);
        break;
    case 9:
        LINE0_TX_LINE1_GND_REVERSE;
        SET_LINE_STATUS(buf[9]);
        break;
    case 10:
        LINE1_TX_LINE0_GND_NORMAL;
        SET_LINE_STATUS(buf[9]);
        break;
    case 11:
        LINE1_TX_LINE0_GND_REVERSE;
        SET_LINE_STATUS(buf[9]);
        break;
    case 12:
        ONLY_LINE0_TX_REVERSE;
        SET_LINE_STATUS(buf[9]);
        break;

    }
    DelayNms(100);
    Read_Dac(mux0_vcc);
    DelayNms(10);
    EcuComRxBuf[ByteNum++]=Read_Dac(mux0_vcc)/100;//比较电压
    DelayNms(100);
    Read_Dac(mux1_vcc);
    DelayNms(10);
    EcuComRxBuf[ByteNum++]=Read_Dac(mux1_vcc)/100;//比较电压
    ByteNum-=2;
    EcuComRxBuf[0] = ByteNum >> 8;
    EcuComRxBuf[1] = ByteNum & 0xff;
    UartTxQueuePack(EcuComRxBuf);

}

/*******************************************
		ByteNum:从EcuComRxBuf开始写数据，每两个数据一组，写两组数据，表示脉冲宽度
    GetLogic获取逻辑状态参数
*******************************************/
uint8 pulse_kl_test_time(uint8 ByteNum,uint8 (*GetLogic)(void))
{
	
	#ifdef quwenqi
    uint32 TIMR0_T1,TIMR0_T2;
    uint8 status;
    //return TRUE;
    DelayNus(100);
    //DelayNms(10);
    Int_time(0,0xffff);
    status=(GetLogic())^0x01;
    while(TIME_INIT==1)//等待时间到或逻辑1
    {
        if(GetLogic()==status)
        {
            TIMR0_T1=TIM0_TC;//LPC_TIM0->TC;
            break;
        }

    }
    if(TIME_INIT!=1)
        return FALSE;
    status^=0x01;
     while(TIME_INIT==1)//等待时间到或逻辑
    {
        if(GetLogic()==status)
        {
            TIMR0_T2=TIM0_TC;
            break;
        }
    }
   if(TIME_INIT!=1)
        return FALSE;
    status^=0x01;
    EcuComRxBuf[ByteNum++]=status;
    EcuComRxBuf[ByteNum++]=(((TIMR0_T2-TIMR0_T1)/3)>>8)&0xff;
    EcuComRxBuf[ByteNum++]=((TIMR0_T2-TIMR0_T1)/3)&0xff;
    while(TIME_INIT==1)//等待时间到或逻辑1
    {
        if(GetLogic()==status)
        {
            TIMR0_T1=TIM0_TC;
            break;
        }
    }
    if(TIME_INIT!=1)
        return FALSE;
    status^=0x01;
    EcuComRxBuf[ByteNum++]=status;
    EcuComRxBuf[ByteNum++]=(((TIMR0_T1-TIMR0_T2)/3)>>8)&0xff;
    EcuComRxBuf[ByteNum++]=((TIMR0_T1-TIMR0_T2)/3)&0xff;
	#endif
    return TRUE;
}
//a32 a33老款日产车发送时钟测试
//buf4 buf5 buf6测试的时钟频率
//测度命令0xfc 0x09
void nissan_pulse_test(uint8 * buf)
{
	 

    EcuComRxBuf[0] = 0x00;
    EcuComRxBuf[1] = 0x08;
    EcuComRxBuf[2] = 0xfc;
    EcuComRxBuf[3] = 0x09;
    SetCChannel(CLK_OUT_ENABLE);
    SetCChannel(KL_BUS_CHANNEL);
    //SetLine0Channel(0x0e);
	  SetLine_ext(00,0x0e);
    SetCompV(0x45);
    SetEclk((buf[4]<<16)+(buf[5]<<8)+buf[6]);
    SetVcc_Drv(DRIVE_12V);
		ONLY_LINE1_RX_NORMAL;
    DelayNms(200);
	
	
	#if 1
	EcuComRxBuf[4]=0;
	EcuComRxBuf[5]=00;
	EcuComRxBuf[6]=0x59;
	EcuComRxBuf[7]=1;
	EcuComRxBuf[8]=00;
	EcuComRxBuf[9]=0x46;
	UartTxQueuePack(EcuComRxBuf);
	return;
	#endif
		//clk_en;
    if(pulse_kl_test_time(4,GET_LINE_STATUS)==FALSE)
    {
        return;
    }
	SetEclk(0);
	clk_dis;
    UartTxQueuePack(EcuComRxBuf);
	 
}


/*******************************************
		ByteNum:从EcuComRxBuf开始写数据，每两个数据一组，写两组数据，表示脉冲宽度
    GetLogic获取逻辑状态参数
*******************************************/
uint8 pulse_kl_test_time_us(uint8 ByteNum,uint8 (*GetLogic)(void))
{
	
	#ifdef quwenqi
    uint32 TIMR0_T1,TIMR0_T2;
    uint8 status;
    //return TRUE;
    DelayNus(100);
    //DelayNms(10);
    Int_time(1,0xffff);
    status=(GetLogic())^0x01;
    while(TIME_INIT==1)//等待时间到或逻辑1
    {
        if(GetLogic()==status)
        {
            TIMR0_T1=TIM0_TC*TIM0_Prescaler;//LPC_TIM0->TC;
            break;
        }

    }
    if(TIME_INIT!=1)
        return FALSE;
    status^=0x01;
     while(TIME_INIT==1)//等待时间到或逻辑
    {
        if(GetLogic()==status)
        {
            TIMR0_T2=TIM0_TC*TIM0_Prescaler;
            break;
        }
    }
   if(TIME_INIT!=1)
        return FALSE;
    status^=0x01;
    EcuComRxBuf[ByteNum++]=status;
    EcuComRxBuf[ByteNum++]=(((TIMR0_T2-TIMR0_T1)/30)>>8)&0xff;
    EcuComRxBuf[ByteNum++]=(((TIMR0_T2-TIMR0_T1)/30))&0xff;
    while(TIME_INIT==1)//等待时间到或逻辑1
    {
        if(GetLogic()==status)
        {
            TIMR0_T1=TIM0_TC*TIM0_Prescaler;
            break;
        }
    }
    if(TIME_INIT!=1)
        return FALSE;
    status^=0x01;
    EcuComRxBuf[ByteNum++]=status;
    EcuComRxBuf[ByteNum++]=(((TIMR0_T1-TIMR0_T2)/30)>>8)&0xff;
    EcuComRxBuf[ByteNum++]=(((TIMR0_T1-TIMR0_T2)/30))&0xff;
	#endif
    return TRUE;
}

//line0上发脉冲 line1上测试，line1上发脉冲，line0测试
// 0:line0 line1上拉，line0输出脉冲，line1测试
// 1:line0 line1上拉，line1输出脉冲，line0测试
// 2::line0 line1下拉，line0输出脉冲，line1测试
// 3:line0 line1下拉，line1输出脉冲，line0测试
void kl_pulse(void)
{
	
	#ifdef quwenqi 
	//uint8 arrPin[13] = {1, 2, 3, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	uint32 TIMR1_T1,TIME1_T2;
	//Read_Dac(mux0_vcc);//Int_Dac_PortV();
	//Read_Dac(drv_vcc);//Int_Dac_BatV();
	
	EcuComRxBuf[0] = 0x00;
	EcuComRxBuf[1] = 0x1B;
	EcuComRxBuf[2] = 0xfc;
	EcuComRxBuf[3] = 0x0C;
	SetCChannel(C_CHANNEL_ALL_OFF);
	SetVcc_Drv(DRIVE_12V);
	//SetCChannel(ANALOG_CHANNEL_ON);//测试mux1电压
	SetCChannel(KL_BUS_CHANNEL);
	SetCompV(0x50);
	//Reset_Line();
	Reset_All_IO();//所有io初始化成初始状态(和obd引脚连接处)
	//  SetEcuLineLoadMode(LINE1, NO_LOAD);
	//line0上拉,逻辑0，因有些板子无悬空状态，故此处只是用来测试干簧管
	//测试上拉波形
	ONLY_LINE0_TX_NORMAL;
	ONLY_LINE0_RX_NORMAL;
	//逻辑0
	EcuComRxBuf[4]=0x01;

	DelayNms(200);
	kl_set[0]=0x00;
	kl_set[1]=0x0;
	kl_set[2]=0x0;
	kl_set[3]=0x55;
	kl_set_time=5;
	//line0上拉输出波形
	if(pulse_kl_test_time_us(5,GET_LINE_STATUS)==FALSE)
	{
		UARTAnswerErr(0x00,0x01);
		return;
	}
	ONLY_LINE1_TX_NORMAL;
	ONLY_LINE1_RX_NORMAL;
	
	kl_set[0]=0x02;
	kl_set[1]=0x0;
	kl_set[2]=0x0;

	//line1上拉输出波形
	if(pulse_kl_test_time_us(11,GET_LINE_STATUS)==FALSE)
	{
		UARTAnswerErr(0x01,0x01);
		return;
	}

	ONLY_LINE0_TX_REVERSE;
	ONLY_LINE0_RX_REVERSE;
	DelayNms(200);
	//Int_time(1,10);
	kl_set[0]=0x01;
	kl_set[1]=0x0;
	kl_set[2]=0x0;
	//line0下拉输出波形
	if(pulse_kl_test_time_us(17,GET_LINE_STATUS)==FALSE)
	{
		UARTAnswerErr(0x02,0x01);
		return;
	}
	ONLY_LINE1_TX_REVERSE;
	ONLY_LINE1_RX_REVERSE;
	kl_set[0]=0x03;
	kl_set[1]=0x0;
	kl_set[2]=0x0;
	//line1下拉输出波形
	if(pulse_kl_test_time_us(23,GET_LINE_STATUS)==FALSE)
	{
		UARTAnswerErr(0x03,0x01);
		return;
	}
	kl_set[3]=0x00;
	Stop_Time(0);
	Stop_Time(1);
	TIMR1_T1=(EcuComRxBuf[6]<<8)+EcuComRxBuf[7];
	TIME1_T2=(EcuComRxBuf[9]<<8)+EcuComRxBuf[10];
	if(TIMR1_T1>TIME1_T2)
		adjust_width[0]=(TIMR1_T1-TIME1_T2)/2;
	else
		adjust_width[0]=(TIME1_T2-TIMR1_T1)/2;

	TIMR1_T1=(EcuComRxBuf[12]<<8)+EcuComRxBuf[13];
	TIME1_T2=(EcuComRxBuf[15]<<8)+EcuComRxBuf[16];
	if(TIMR1_T1>TIME1_T2)
		adjust_width[1]=(TIMR1_T1-TIME1_T2)/2;
	else
		adjust_width[1]=(TIME1_T2-TIMR1_T1)/2;
	TIMR1_T1=(EcuComRxBuf[18]<<8)+EcuComRxBuf[19];
	TIME1_T2=(EcuComRxBuf[21]<<8)+EcuComRxBuf[22];
	if(TIMR1_T1>TIME1_T2)
		adjust_width[2]=(TIMR1_T1-TIME1_T2)/2;
	else
		adjust_width[2]=(TIME1_T2-TIMR1_T1)/2;
	TIMR1_T1=(EcuComRxBuf[24]<<8)+EcuComRxBuf[25];
	TIME1_T2=(EcuComRxBuf[27]<<8)+EcuComRxBuf[28];
	if(TIMR1_T1>TIME1_T2)
		adjust_width[3]=(TIMR1_T1-TIME1_T2)/2;
	else
		adjust_width[3]=(TIME1_T2-TIMR1_T1)/2;

		#endif
	
}

//根据初始化io后的方式确定调整脉冲的宽度,用在初始化io之后，可以直接根据不同的板子来自动调整脉冲宽度
void adjust_pulse_width(void)
{
#if 1
    EcuComRxBuf[0] = 0x00;
    EcuComRxBuf[1] = 0x08;
    EcuComRxBuf[2] = 0xfc;
    EcuComRxBuf[3] = 0x0D;
    DelayNms(200);
    kl_set[0]=0x00;
    kl_set[1]=0x0;
    kl_set[2]=0x0;
    Int_time(2,10);
	//LIN0发送或者LIN0和LIN1同时发送
    //if((PassThruConfig.Pin1&0xf0)||((PassThruConfig.Pin1&0xf0)&&(PassThruConfig.Pin2&0xf0))
    {
        //if((ComCtl.ChannelDriveMode&0x0f)==STRONG_PULL_UP)//line0上拉或line0和line1都上拉
		if(PassThruConfig.TxMode == MODE_ONLY_LINE0_TX_NORMAL || PassThruConfig.TxMode == MODE_LINE0_LINE1_TX_NORMAL)
        {
            kl_set[0]=01;
            kl_set[1]=0x0;
            kl_set[2]=0x0;
            kl_set[3]=0x55;
            if(pulse_kl_test_time(4,GET_LINE_STATUS)==FALSE)
            {
                return;
            }
        }
		if(PassThruConfig.TxMode == MODE_ONLY_LINE0_TX_REVERSE || PassThruConfig.TxMode == MODE_LINE0_LINE1_TX_REVERSE)
        {
            kl_set[0]=00;
            kl_set[1]=0x0;
            kl_set[2]=0x0;
            kl_set[3]=0x55;
            if(pulse_kl_test_time(4,GET_LINE_STATUS)==FALSE)
            {
                return;
            }
        }
    }

    //if(ComCtl.TxChannel==ONLY_CH1TX)
    {
        //if(((ComCtl.ChannelDriveMode>>4)&0x0f)==STRONG_PULL_UP)//line1上拉
		if(PassThruConfig.TxMode == MODE_ONLY_LINE1_TX_NORMAL)
        {
            kl_set[0]=03;
            kl_set[1]=0x0;
            kl_set[2]=0x0;
            kl_set[3]=0x55;
            if(pulse_kl_test_time(4,GET_LINE_STATUS)==FALSE)
            {
                return;
            }
        }
        //if(((ComCtl.ChannelDriveMode>>4)&0x0f)==STRONG_PULL_DOWN)//line1下拉
		if(PassThruConfig.TxMode == MODE_ONLY_LINE1_TX_REVERSE)
        {
            kl_set[0]=02;
            kl_set[1]=0x0;
            kl_set[2]=0x0;
            kl_set[3]=0x55;
            if(pulse_kl_test_time(4,GET_LINE_STATUS)==FALSE)
            {
                return;
            }
        }
    }
    Stop_Time(0);
    Stop_Time(1);
    //LINE0_NO_LOAD;//CH0_Suspended();
   // LINE1_NO_LOAD;//CH1_Suspended();
		tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_LOW);//LINE 0  PULL UP
		tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_LOW);//LINE 1  PULL UP
			
		tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_LOW);//LINE 1  PULL UP
		tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_LOW);//LINE 1  PULL DOWN
	#endif
}
//为了兼容2132,在此处把io引脚重新定义，f3中P0.9为RX0 ,P0.10为RX1,F6系列中P0.16为RX0,P01.11为RX1
void GetP0status(void)
{
	#if 0  //quwenqi2g
	 
	uint32 i;
/*	if((ChannelBuf[3]&0x01)==0x01)//兼容f3,此处要设置，line0和line1分开控制和接收，但要同时控制上拉或下拉，不能一个上拉一个下拉,接收同理
	{
// 		ONLY_LINE0_TX_NORMAL;
// 			ONLY_LINE0_RX_NORMAL;
// 			LINE1_UP;			
// 			tx_c_channel();
		
			LINE0_LINE1_TWOCONTROL_NORMAL;
	}
	else
	{
		
// 			ONLY_LINE0_TX_REVERSE;
// 			ONLY_LINE0_RX_REVERSE;
// 			LINE1_DOWN;			
// 			tx_c_channel();
		LINE0_LINE1_TWOCONTROL_REVERSE
	}*/
	EcuComRxBuf[0]=0X00;
	EcuComRxBuf[1]=0X05;
	EcuComRxBuf[2]=0XF6;
	if((ChannelBuf[3]&0x01)==0x01)//同向模式
		i=(GET_RX0_STATUS<<9)|(GET_LINE0_OR_LINE1<<10);
	else
		i=(((GET_RX0_STATUS+0x01)&0x01)<<9)|(((GET_LINE0_OR_LINE1+0x01)&0x01)<<10);
	EcuComRxBuf[3]=(i>>24)&0xff;
	EcuComRxBuf[4]=(i>>16)&0xff;
	EcuComRxBuf[5]=(i>>8)&0xff;
	EcuComRxBuf[6]=i&0xff;
	UartTxQueuePack(EcuComRxBuf);
	#endif
}

/*
    f3中，line0或line1的上拉或下拉，每次都由两个IO口控制
    TX00 TX01
      00   00  上拉 　　q3q1截止　　　高电平
      00   01　上拉　 　q3截止，q1通　低电平
      
			01   00  下拉　　 Q3通，Q1截止  高电平
      01   01　下拉     Q3Q1截止      低电平
  
    TX10 TX11
      00   00  上拉　 　q4q2截止　　　高电平
      00   01　上拉　　 q4截止，q2通　低电平
      
			01   00  下拉　　 Q4通，Q2截止  高电平
      01   01　下拉     Q4Q2截止      低电平
		tx00 tx01   上拉
		00 		00    高电平  rxd=1 前两个设置line0 bit9为line0接收状态;后两个设置line0 bit10为line1接收状态;
		00 		01    低电平  rxd=0 
		tx00 tx01下拉
		01 		01    低电平  rxd=0
		01 		00    高电平  rxd=1
*/

//在f8中，line0,line1单独控制时，只能同向控制(line0line1全上拉或全下拉)所以在直接用引脚控制时，要统一上拉或下拉（设计所制）
void SetTxPort(uint8 *buf)
{ 
	
		#ifdef quwenqi 
	if(buf[3]==buf[5])//此两数相同表示同向控制
	{
		if(buf[3]==0)
		{
			LINE0_LINE1_TWOCONTROL_NORMAL;
// 			ONLY_LINE0_TX_NORMAL;
// 			ONLY_LINE0_RX_NORMAL;
// 			LINE1_UP;			
// 			tx_c_channel();
			SET_LINE_STATUS(buf[4]+0x01);
			SET_Another_STATUS(buf[6]+0x01);
		}
		else
		{
// 			ONLY_LINE0_TX_REVERSE;
// 			ONLY_LINE0_RX_REVERSE;
// 			LINE1_DOWN;			
// 			tx_c_channel();
			LINE0_LINE1_TWOCONTROL_REVERSE;
			SET_LINE_STATUS(buf[4]);
			SET_Another_STATUS(buf[6]);
		}
		
	}
//    if((buf[3]==0)&&(buf[4]==0))//根据f3的设置转换而来
// 	 {
// 		 SET_TX01_LOW_LEVEL;
// 	 }
// 	 else if((buf[3]==0)&&(buf[4]==1))
// 	 {
// 		 SET_TX01_HIGH_LEVEL;
// 	 } 
// 	 else if((buf[3]==1)&&(buf[4]==0))
// 	 {
// 		  SET_TX00_HIGH_LEVEL;
// 		
// 	 }
// 	 else if((buf[3]==1)&&(buf[4]==1))
// 	 {
// 		 SET_TX00_LOW_LEVEL;
// 	 }
//    if((buf[5]==0)&&(buf[6]==0))
// 	 {
// 		  SET_TX11_LOW_LEVEL;
// 	 }
// 	 else if((buf[5]==0)&&(buf[6]==1))
// 	 {
// 		  SET_TX11_HIGH_LEVEL;
// 	 } 
// 	 else if((buf[5]==1)&&(buf[6]==0))
// 	 {
// 		 SET_TX10_HIGH_LEVEL;
// 	 }
// 	 else if((buf[5]==1)&&(buf[6]==1))
// 	 {
// 		 SET_TX10_LOW_LEVEL;
// 	 }
 	DelayNms(10); 
 	GetP0status(); 
	 
	 #endif
}

//当前驱动电压测试
void drv_vcc_test(void)
{

		uint8 ByteNum=0;
    //uint32 drv_v_read;
   // uint8 arrPin[13] = {1, 2, 3, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    //Int_Dac_PortV();
    //Int_Dac_BatV();
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = 0xfc;
    EcuComRxBuf[ByteNum++] = 0x10;
		vcc_drv_v();
//     DelayNms(10);
//     drv_v_read=Read_Dac(drv_vcc);
//     DelayNms(10);
    //drv_v_read=Read_Dac(drv_vcc);
    EcuComRxBuf[ByteNum++]=(vcc_drv>>8)&0xff;//drv_v_read*11*33/4096;
    EcuComRxBuf[ByteNum++]=vcc_drv&0xff;
    EcuComRxBuf[1]=ByteNum-2;
    UartTxQueuePack(EcuComRxBuf);

}
//测试60欧姆电阻继电器，目前没法测试
void can_res_test(void)
{
	
	//#ifdef quwenqi
	uint8 ByteNum=0;
	EcuComRxBuf[ByteNum++] = 0x00;
	EcuComRxBuf[ByteNum++] = 0x00;
	EcuComRxBuf[ByteNum++] = 0xfc;
	EcuComRxBuf[ByteNum++] = 0x04;
	SetVcc_Drv(DRIVE_12V);
	SetCChannel(KL_BUS_CHANNEL);
	SetCompV(0x35); //双通路上拉,逻辑0
	//DIFFERENTIAL_TX_LINE0DOWN;
	SET_LINE_STATUS(LOGIC_1);
	//SET_60RES;
	//SetLine_ext(0x06,0x0e);
	DelayNms(300);
	EcuComRxBuf[ByteNum++]=Read_Dac(mux0_vcc)/100;
	EcuComRxBuf[ByteNum++]=Read_Dac(mux1_vcc)/100;
	//CLR_60RES;
	DelayNms(300);
	EcuComRxBuf[ByteNum++]=Read_Dac(mux0_vcc)/100;
	EcuComRxBuf[ByteNum++]=Read_Dac(mux1_vcc)/100;
	//SET_60RES;
	DelayNms(300);
	EcuComRxBuf[ByteNum++]=Read_Dac(mux0_vcc)/100;
	EcuComRxBuf[ByteNum++]=Read_Dac(mux1_vcc)/100;
	EcuComRxBuf[1]=ByteNum-2;
	UartTxQueuePack(EcuComRxBuf);
	//  LINE0_LINE1_TX_NORMAL;
	//	ONLY_LINE0_TX_NORMAL;
	//ONLY_LINE1_RX_NORMAL;
		//#endif
}
