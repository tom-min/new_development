

#include <string.h>
#include "config.h"
#include "interface.h"
#include "PortAssign.h"
#include "hardware.h"
#include "hardwaretest.h"
#include "FPGA_Drives.h"
#include "conf_pdu.h"
#include "user_stm32f2xx.h"
#include "bluewifi.h"
#include "user_config.h"
extern uint8 set_compv;
void hal_SET_CMP_Volt(uint8 CompV)
{
	uint16 temp;
	uint16 DAV;

	if(set_compv!=0)
	{
		CompV=set_compv;
		//UARTAnswerErr(0xfc,CompV);
	}
	if(CompV>0xa0) CompV=0xA0;

	temp=((CompV&0xf0)>>4)*10+(CompV&0x0f);
	DAV =temp*100*4.7/14.7/3.3;
	Timer_PWM_Config(DAV);
}


void ADC1_Init (void)
{
	ADC_INIT();
	 
}


extern uint32 CanId_RxBuf[MAX_CANID_COUNT];
 
extern void tx_CTRL_EN_channel2_write(void);
extern void tx_MODE_EN_channel2(uint8_t TXMODE,uint8_t RXMODE,uint8_t TX_REV,uint8_t RX_REV);
void ALL_GPIO_Config(void)
{

}
extern uint8_t HAL_MINIVCI_UART_READ(uint16_t msDelayCounter,uint8_t *data);
extern void hal_kwp_send_byte_kwp(uint8_t data);
/*
*********************************************************************************************************
* 函数名称：GET_LINE_STATUS
* 函数功能：得到line0线上是逻辑0还是逻辑1
* 入口参数：
* 出口参数：0:逻辑0 1:逻辑1 
*********************************************************************************************************
*/
uint8 GET_LINE_STATUS(void)
{
	return GET_RX0_STATUS;
}


/*
*********************************************************************************************************
* 函数名称：SET_LINE_STATUS
* 函数功能：设置line0输出逻辑1或逻辑0
* 入口参数：0:逻辑0 1:逻辑1
* 出口参数：
*********************************************************************************************************
*/
void SET_LINE_STATUS(uint8 OUT_STATUS)
{
	switch(OUT_STATUS&0x01)
	{
		case SET_OUT_STATUS_1:
			SET_TX0_PRE_IO_HIGH;
			break;
		case SET_OUT_STATUS_0:
			SET_TX0_PRE_IO_LOW;
			break;
	}
}
/*
*********************************************************************************************************
* 函数名称：SET_Another_STATUS
* 函数功能：设置line0输出逻辑1或逻辑0
* 入口参数：0:逻辑0 1:逻辑1
* 出口参数：
*********************************************************************************************************
*/
void SET_Another_STATUS(uint8 OUT_STATUS)
{
	switch(OUT_STATUS&0x01)
	{
		case SET_OUT_STATUS_1:
			LINE0_OR_LINE1_HIGH;
			break;
		case SET_OUT_STATUS_0:
			LINE0_OR_LINE1_LOW;
			break;
	}
}

/*
*********************************************************************************************************
* 函数名称：SetCChannel()
* 函数功能：设置C通道
* 入口参数：通道号 JC0:LINE0 上拉;JC1:第二路can接通;JC2:line0下拉;JC3:LINE1下拉;JC4:CLK_CON;JC5:LINE0,CAN0切换;JC8(pull1):line1上拉
* 出口参数：
*********************************************************************************************************
*/
void  SetCChannel(uint8 ChannelNum)
{ 
	
	
	switch(ChannelNum)
	{
	case C_CHANNEL_ALL_OFF:
         printf_usb2("C_CHANNEL_ALL_OFF\r\n");
        tx_CTRL_EN_channel2_AllOff();
		break;
	case CAN_BUS_CHANNEL:
	printf_usb2("CAN_BUS_CHANNEL\r\n");
		can_fir_en;//M_JC05 
        tx_CTRL_EN_channel2(M_JC05,JC_HIGH);
		break;
	case KL_BUS_CHANNEL:
	printf_usb2("KL_BUS_CHANNEL\r\n");
		kl_en;
        tx_CTRL_EN_channel2(M_JC05,JC_LOW);
		break;
// 	case CAN_TWO_BUS_CHANNEL:
// 		can_sec_en;
// 		break;
// 	case CAN_TWO_BUS_CHANNEL_DIS:
// 		can_sec_dis;
// 		break;
	case NO_DRIVE:  	
		LINE_NO_LOAD;
        printf_usb2("NO_DRIVE\r\n");
        tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_LOW);//LINE 0  PULL UP
        tx_CTRL_EN_channel2(JC2_LINE0_PD,JC_LOW);//LINE 0  PULL DOWN

        tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_LOW);//LINE 0  PULL UP
        tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_LOW);//LINE 0  PULL DOWN

		break;
	case CLK_OUT_ENABLE:   //CLK_CON=1
		printf_usb2("CLK_OUT_ENABLE\r\n");
		clk_en;
        tx_CTRL_EN_channel2(M_JC04,JC_HIGH);
		break;
	case CLK_OUT_DISENABLE: //CLK_CON=0
		printf_usb2("CLK_OUT_DISENABLE\r\n");
		clk_dis;
        tx_CTRL_EN_channel2(M_JC04,JC_LOW);
		break;
	default:
		return;
	}
	//tx_c_channel();

    tx_CTRL_EN_channel2_write();

}
extern void PassThru_PWM(uint8_t command ,uint8_t channl);
extern void PassThru_VPW(uint8_t command ,uint8_t channl);
void Reset_Line(void)
{
	//G_RXD_DIS;'
	memset(kl_set,0,sizeof(kl_set));
	memset(ChannelBuf,0,sizeof(ChannelBuf));
    
	tx_CTRL_EN_channel2_AllOff();
	tx_PINSEL_channel2(DB26_PIN_NC,DB26_PIN_NC);
	SET_LINE_STATUS(SET_OUT_STATUS_1);
	SET_TX0_PRE_IO_HIGH;//
	LINE0_OR_LINE1_HIGH;
	SetEclk(0); //JDB8 PWM
	LINE0_OR_LINE1_HIGH;
//	SET_60RES;
	ecu_staus=ecu_stop;
	G_RXD_EN;
	//	CANx_Stop();FIRST_SINGLE_CAN_RX;FIRST_SINGLE_CAN_TX;
	// 	memset(KeepComBuf, 0, KEEP_COM_BUF_LEN);//清空通讯保持命令
	//   memset(KeepComBuf1, 0, KEEP_COM_BUF_LEN);//清空通讯保持命令
	// 	memset((uint8 * ) & PassThruConfig, 0, sizeof(PASSTHRU_CONFIG));
	//   memset((uint8 * ) & PassThruDriver, 0, sizeof(PASSTHRU_DRIVER));
	//  memset((uint8 * ) & ComCtl, 0, sizeof(ComCtl));
	// PassThru_Default_SetConfig();//设置缺省参数
	// 	Queue_Init(&CAN1_QueueData);

#if 0
	//PassThru_PWM(CAN_CHANNEL_CLOSE,CHANNL0);
	//PassThru_PWM(CAN_CHANNEL_CLOSE,CHANNL1);
	//PassThru_VPW(CAN_CHANNEL_CLOSE,CHANNL0);
	//PassThru_VPW(CAN_CHANNEL_CLOSE,CHANNL1);
   
	usart_disable_tx(KWP_TX_UART);
	usart_disable_rx(KWP_RX_UART);
	usart_disable_interrupt(KWP_RX_UART,US_IER_RXRDY);
#endif
   
}
void Reset_All_IO(void)
{
	//断开第二路can信号(包括单线can)
// 	Reset_Line();
// 	SET_SECOND_CAN(CLR_SECOND_CAN_TO_OBD);
	Reset_Line();
	
    memset((uint8 * ) & PassThruConfig, 0, sizeof(PASSTHRU_CONFIG));
    memset((uint8 * ) & PassThruDriver, 0, sizeof(PASSTHRU_DRIVER));
    memset((uint8 * ) & ComCtl, 0, sizeof(ComCtl));
    memset(ChannelBuf, 0, sizeof(ChannelBuf));
    memset(CanId_RxBuf,0,sizeof(CanId_RxBuf));
    PassThru_Default_SetConfig();//设置缺省参数
    ComCtl.bitnum = 8; //一字节等于8bit
}

void INIT_All_IO(void)
{
 
	 
	GPIO_InitTypeDef  GPIO_InitStructure;
 
//////////////////////////////////////////////////////////////////////////
	/* Enable the GSM_POWER_PIN Clock */
	RCC_AHB1PeriphClockCmd(GSM_POWER_CLK, ENABLE);

	/* Configure the GSM_POWER_PIN pin */
	GPIO_InitStructure.GPIO_Pin = GSM_POWER_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GSM_POWER_PORT, &GPIO_InitStructure);

	/* Enable the GSM_PWR_PIN Clock */
	RCC_AHB1PeriphClockCmd(GSM_PWR_CLK, ENABLE);

	/* Configure the GSM_PWR_PIN pin */
	GPIO_InitStructure.GPIO_Pin = GSM_PWR_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GSM_PWR_PORT, &GPIO_InitStructure);
	
	
	
	GPIO_SetBits(GSM_POWER_PORT,GSM_POWER_PIN);
	GPIO_SetBits(GSM_PWR_PORT,GSM_PWR_PIN);
	
//////////////////////////////////////////////////////////////////////////
	/* Enable the   Clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1|GPIO_Pin_2 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	

	
    	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/////////////////////////////////////////////////////////////////
	GPIO_InitStructure.GPIO_Pin = BUS_5V_CTL_PIN;
	GPIO_Init(BUS_5V_CTL_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = BUS_8V_CTL_PIN;
	GPIO_Init(BUS_8V_CTL_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = BUS_VCCIN_CTL_PIN;
	GPIO_Init(BUS_VCCIN_CTL_PORT, &GPIO_InitStructure);
	
	//////////////////////////////////////////////////////
#ifndef M_NEWHARDWARE
	GPIO_InitStructure.GPIO_Pin = PB14_MUX_CTL_PIN;
	GPIO_Init(PB14_MUX_CTL_PORT, &GPIO_InitStructure);
#endif
	GPIO_InitStructure.GPIO_Pin = PC4_LINE_CTL_PIN;
	GPIO_Init(PC4_LINE_CTL_PORT, &GPIO_InitStructure);

///////////////////LINE0 LINE1 PU PD /////////////////////////////
	GPIO_InitStructure.GPIO_Pin = PD13_LINE0_PU_PIN|PD15_LINE1_PU_PIN|PD12_LINE0_PD_PIN|PD14_LINE1_PD_PIN;

	GPIO_Init(PD13_LINE0_PU_PORT, &GPIO_InitStructure);

///////////////////EC 20 /////////////////////////////	

	GPIO_InitStructure.GPIO_Pin = PA15_RESET_EC20_PIN;

	GPIO_Init(PA15_RESET_EC20_PORT, &GPIO_InitStructure);
	

	GPIO_InitStructure.GPIO_Pin = PA05_SLEEP_STATUS_EC20_PIN;

	GPIO_Init(PA05_SLEEP_STATUS_EC20_PORT, &GPIO_InitStructure);
	

	GPIO_InitStructure.GPIO_Pin = PB1_ON_OFF_EC20_PIN;

	GPIO_Init(PB1_ON_OFF_EC20_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PB3_WAKEUP_IN_EC20_PIN;

	GPIO_Init(PB3_WAKEUP_IN_EC20_PORT, &GPIO_InitStructure);
	
///////////////////BAT /////////////////////////////	
#ifndef M_NEWHARDWARE
       GPIO_InitStructure.GPIO_Pin = PB15_VBAT_OPEN_PIN;
	GPIO_Init(PB15_VBAT_OPEN_PORT, &GPIO_InitStructure);
#endif
	
 
//////////////////////////////////////////////////////////////
	GPIO_InitStructure.GPIO_Pin = PC5_W_CAN_CTL_PIN;
	GPIO_Init(PC5_W_CAN_CTL_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PA9_CAN_MODE_PIN;
	GPIO_Init(PA9_CAN_MODE_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PA10_CAN_MODE_PIN;
	GPIO_Init(PA10_CAN_MODE_PORT, &GPIO_InitStructure);


//////////////////////MUX0 /////////////////////
	GPIO_InitStructure.GPIO_Pin = DG1_A0_PIN|DG1_A1_PIN|DG1_A2_PIN|DG1_EN_PIN;
	GPIO_Init(DG1_PORT, &GPIO_InitStructure);
	
/////////////////MUX 1////////////////////////////////
	GPIO_InitStructure.GPIO_Pin = PC4_MUX1_CTL_PIN;
	GPIO_Init(PC4_MUX1_CTL_PORT, &GPIO_InitStructure);

	
	

///////////////////////////////////////////////////
	GPIO_InitStructure.GPIO_Pin = PC2_CAN0_CTL2_PIN;
	GPIO_Init(PC2_CAN0_CTL2_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PC1_CAN0_CTL1_PIN;
	GPIO_Init(PC1_CAN0_CTL1_PORT, &GPIO_InitStructure);




	///////////////////////////////////////
	
	GPIO_InitStructure.GPIO_Pin = RED_LED_PIN;
	GPIO_Init(LED_PORT, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin = GREEN_LED_PIN;
	GPIO_Init(LED_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = BLUE_LED_PIN;
	GPIO_Init(LED_PORT, &GPIO_InitStructure);
	
	
	//////////////////OBD//////////////////
	
	
	GPIO_ResetBits(PC4_LINE_CTL_PORT,PC4_LINE_CTL_PIN);
	#ifndef M_NEWHARDWARE
	GPIO_ResetBits(PB14_MUX_CTL_PORT,PB14_MUX_CTL_PIN);
	#endif
	GPIO_SetBits(PC5_W_CAN_CTL_PORT, PC5_W_CAN_CTL_PIN);
	GPIO_ResetBits(PC1_CAN0_CTL1_PORT,PC1_CAN0_CTL1_PIN);


	
       GPIO_SetBits(PA9_CAN_MODE_PORT, PA9_CAN_MODE_PIN);
	GPIO_SetBits(PA10_CAN_MODE_PORT,PA10_CAN_MODE_PIN);
	
	
	/////////////LED////////////////////
	GPIO_ResetBits(LED_PORT,RED_LED_PIN);
	GPIO_ResetBits(LED_PORT,GREEN_LED_PIN);
	GPIO_ResetBits(LED_PORT,BLUE_LED_PIN);
	
	
	//////////////////VCC//////////////////
	GPIO_ResetBits(BUS_5V_CTL_PORT,BUS_5V_CTL_PIN);
	GPIO_ResetBits(BUS_8V_CTL_PORT,BUS_8V_CTL_PIN);
	GPIO_ResetBits(BUS_VCCIN_CTL_PORT,BUS_VCCIN_CTL_PIN);

	//////////////////EC20//////////////////
	GPIO_ResetBits(PA15_RESET_EC20_PORT,PA15_RESET_EC20_PIN);
	GPIO_ResetBits(PB1_ON_OFF_EC20_PORT,PB1_ON_OFF_EC20_PIN);
	GPIO_ResetBits(PB3_WAKEUP_IN_EC20_PORT,PB3_WAKEUP_IN_EC20_PIN);
	GPIO_ResetBits(PA05_SLEEP_STATUS_EC20_PORT,PA05_SLEEP_STATUS_EC20_PIN);

	#ifndef M_NEWHARDWARE
	//////////////////BAT//////////////////
	GPIO_ResetBits(PB15_VBAT_OPEN_PORT,PB15_VBAT_OPEN_PIN);
	#endif
	

	//////////////////pwm//////////////////
	/*GPIOB_Pin_0, TIM3_Channel3 PWM*/ 
	GPIO_InitStructure.GPIO_Pin = PWM_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(PWM_PORT, &GPIO_InitStructure);
 
	GPIO_PinAFConfig(PWM_PORT, GPIO_PinSource0, GPIO_AF_TIM3);//GPIOB_Pin9 TIM4_Ch4N,


}
/*********************************************************************************************
* 名称    ： Set_DrvPort()
* 功能    ： 配置端口
* 入口参数： PIN1,PIN2,LOGIC_V1,LOGIC_V2
*
*
* 出口参数：
* 说明    ：
*
***********************************************************************************************/
uint8_t MCU_UARTGetByte (uint32 DelayCounter,uint8 *result)
{
	return HAL_MINIVCI_UART_READ(DelayCounter,result);
}
void MCU_UARTSendByte (uint8_t ucDat)
{
	hal_kwp_send_byte_kwp(ucDat);
}
extern void LinePinDirInit(void);
extern uint32 PIN_AND_VCC;
uint8 Set_DrvPort(uint8 PIN1, uint8 PIN2, uint8 LOGIC_V1, uint8 LOGIC_V2,uint8 GND_COM)
{
	 
	//pin1 ==0x77 pin2===0xff   77 ff c1 c1 00

	uint32 temp_PIN_AND_VCC;

	temp_PIN_AND_VCC=(PassThruConfig.Pin1<<24)|(PassThruConfig.Pin2<<16)|(PassThruConfig.LogicV1<<8)|PassThruConfig.LogicV2;
	if((PIN_AND_VCC==temp_PIN_AND_VCC)&&(PIN_AND_VCC!=0))//相同，直接返回，不需要设置
	{
		printf_usb2("return \r\n");
		//引脚设置
		switch((GND_COM) & 0x0f)
		{
			case 0:
			SetLine_ext(PassThruConfig.Pin1,PassThruConfig.Pin2);
			break;
			case 1:
			SetLine_ext(GND_COM& 0xf0,PassThruConfig.Pin2);
			break;
			case 2:
			SetLine_ext(PassThruConfig.Pin1,GND_COM & 0xf0);
			break;
			default:
			SetLine_ext(PassThruConfig.Pin1,PassThruConfig.Pin2);
			break;
		}
			
		return TRUE;
	}
	//Reset_Line();
	PIN_AND_VCC = temp_PIN_AND_VCC;
	
	#if 1
    SetCChannel(KL_BUS_CHANNEL);
	
	if(ComCtl.RxHardWare==USE_IO) //用软件接收
	{
		//PassThruDriver.RxByteDrv=SoftUartRxByte;
		//PassThruDriver.TxByteDrv=_SoftTxByte;
	}
	else
	{
		PassThruDriver.RxByteDrv = MCU_UARTGetByte;
		PassThruDriver.TxByteDrv = MCU_UARTSendByte;
	}
		
    switch(((LOGIC_V1 | LOGIC_V2) & 0xf0) >> 4)
    {
		case 5:  //5V
			SetVcc_Drv(DRIVE_5V);
			break;
		case 8:  //8V
			SetVcc_Drv(DRIVE_8V);
			break;
		case 12: //12V
			SetVcc_Drv(DRIVE_12V);
			break;
		default:
			SetCChannel(NO_DRIVE);
    }
 
    //发送方式配置nn
    if(((PIN1&0xf0)!=0)&&((PIN2&0xf0)!=0))
    {
        if(((LOGIC_V1&0x0f)==1)&&((LOGIC_V2&0x0f)==0))//RS485line0上拉输出
            //return FALSE;
        {
            DIFFERENTIAL_TX_LINE0UP;
			
			tx_MODE_EN_channel2(tx_mode_DIFFERENTIAL,0XF0,NORMAL,NORMAL);
			tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_HIGH);//LINE 0  PULL UP
			tx_CTRL_EN_channel2(JC2_LINE0_PD,JC_LOW);//LINE 0  PULL DOWN
			
			tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_LOW);//LINE 1  PULL UP
			tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_HIGH);//LINE 1  PULL DOWN
			tx_CTRL_EN_channel2_write();
			PassThruConfig.TxMode = MODE_DIFFERENTIAL_TX_LINE0UP;
			
			printf_usb2("DIFFERENTIAL_TX_LINE0UP\r\n");
            PassThruConfig.TxMode = MODE_DIFFERENTIAL_TX_LINE0UP;
			
        }
        if(((LOGIC_V1&0x0f)==0)&&((LOGIC_V2&0x0f)==1))//rs485line0下拉输出
        {
			DIFFERENTIAL_TX_LINE0DOWN;
			tx_MODE_EN_channel2(tx_mode_DIFFERENTIAL,0XF0,NORMAL, NORMAL);
			tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_LOW);//LINE 0  PULL UP
			tx_CTRL_EN_channel2(JC2_LINE0_PD, JC_HIGH);//LINE 0  PULL DOWN
						
			tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_HIGH);//LINE 1  PULL UP
			tx_CTRL_EN_channel2(JC3_LINE1_PD, JC_LOW);//LINE 1  PULL DOWN
			tx_CTRL_EN_channel2_write();		
			PassThruConfig.TxMode = MODE_DIFFERENTIAL_TX_LINE0DOWN;
			
			printf_usb2("DIFFERENTIAL_TX_LINE0DOWN\r\n");
            PassThruConfig.TxMode = MODE_DIFFERENTIAL_TX_LINE0DOWN;
        }
        if(((LOGIC_V1&0x0f)==1)&&((LOGIC_V2&0x0f)==1))
        {
            LINE0_LINE1_TX_NORMAL;//line0 line1同时正向发送
			
			tx_MODE_EN_channel2(tx_mode_LINE0ANDLINE1,0XF0,NORMAL,NORMAL);
			
			tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_HIGH);//LINE 0  PULL UP
			tx_CTRL_EN_channel2(JC2_LINE0_PD, JC_LOW);//LINE 0  PULL DOWN
						
			tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_HIGH);//LINE 1  PULL UP
			tx_CTRL_EN_channel2(JC3_LINE1_PD, JC_LOW);//LINE 1  PULL DOWN
			tx_CTRL_EN_channel2_write();			
			printf_usb2("LINE0_LINE1_TX_NORMAL\r\n");
            PassThruConfig.TxMode = MODE_LINE0_LINE1_TX_NORMAL;
        }
        if(((LOGIC_V1&0x0f)==0)&&((LOGIC_V2&0x0f)==0))
        {
			printf_usb2("LINE0_LINE1_TX_REVERSE\r\n");
			tx_MODE_EN_channel2(tx_mode_LINE0ANDLINE1,0XF0,REVERSE,NORMAL);
			tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_LOW);//LINE 0  PULL UP
			tx_CTRL_EN_channel2(JC2_LINE0_PD,  JC_HIGH);//LINE 0  PULL DOWN
							
			tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_LOW);//LINE 1  PULL UP
			tx_CTRL_EN_channel2(JC3_LINE1_PD,  JC_HIGH);//LINE 1  PULL DOWN
			tx_CTRL_EN_channel2_write();
				
			LINE0_LINE1_TX_REVERSE;//line0 line1同时反向发送
			PassThruConfig.TxMode = MODE_LINE0_LINE1_TX_REVERSE;
        }
    }
    else if((PIN1&0xf0)!=0)//line0发送
    {
        if((PassThruConfig.GndCom&0x0f)==0)
        {
            if((LOGIC_V1&0x0f)==1)
            {
				printf_usb2("ONLY_LINE0_TX_NORMAL\r\n");
				ONLY_LINE0_TX_NORMAL;//line0正向发送
				tx_MODE_EN_channel2(tx_mode_LINE0,0XF0,NORMAL,NORMAL);
				
				 
				tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_HIGH);//LINE 0  PULL UP
				tx_CTRL_EN_channel2(JC2_LINE0_PD,JC_LOW);//LINE 0  PULL DOWN
							
				tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_LOW);//LINE 1  PULL UP
				tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_LOW);//LINE 1  PULL DOWN
				tx_CTRL_EN_channel2_write();
				
				PassThruConfig.TxMode = MODE_ONLY_LINE0_TX_NORMAL;
            }
            else
            {
				printf_usb2("ONLY_LINE0_TX_REVERSE\r\n");
                ONLY_LINE0_TX_REVERSE;//line0反向发送
				tx_MODE_EN_channel2(tx_mode_LINE0,0XF0,REVERSE,NORMAL);
				
				tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_LOW);//LINE 0  PULL UP
				tx_CTRL_EN_channel2(JC2_LINE0_PD,JC_HIGH);//LINE 0  PULL DOWN
								
				tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_LOW);//LINE 1  PULL UP
				tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_LOW);//LINE 1  PULL DOWN
				tx_CTRL_EN_channel2_write();
				
                PassThruConfig.TxMode = MODE_ONLY_LINE0_TX_REVERSE;
            }
        }
        else if((PassThruConfig.GndCom&0x0f)==2)//line1接地
        {
            if((LOGIC_V1&0x0f)==1) //line0正向发送,line1接地
            {
				
				printf_usb2("LINE0_TX_LINE1_GND_NORMAL\r\n");
                LINE0_TX_LINE1_GND_NORMAL;
                
				tx_MODE_EN_channel2(tx_mode_LINE0,tx_mode_LINE0,REVERSE,NORMAL);
				tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_HIGH);//LINE 0  PULL UP
				tx_CTRL_EN_channel2(JC2_LINE0_PD,JC_LOW);//LINE 0  PULL DOWN
								
				tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_HIGH);//LINE 1  PULL UP
				tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_LOW);//LINE 1  PULL DOWN
				LINE0_OR_LINE1_LOW;
				tx_CTRL_EN_channel2_write();
				
               // LINE0_OR_LINE1_LOW;
                PassThruConfig.TxMode = MODE_LINE0_TX_LINE1_GND_NORMAL;
            }
            else
            {
				printf_usb2("LINE0_TX_LINE1_GND_REVERSE\r\n");
                		LINE0_TX_LINE1_GND_REVERSE;//line0反向发送，line1接地
				tx_MODE_EN_channel2(tx_mode_LINE0,0XF0,REVERSE,NORMAL);
				tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_LOW);//LINE 0  PULL UP
				tx_CTRL_EN_channel2(JC2_LINE0_PD, JC_HIGH);//LINE 0  PULL DOWN
							
				tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_HIGH);//LINE 1  PULL UP
				tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_LOW);//LINE 1  PULL DOWN
				tx_CTRL_EN_channel2_write();
               // LINE0_OR_LINE1_LOW;
                PassThruConfig.TxMode = MODE_LINE0_TX_LINE1_GND_REVERSE;
            }
        }
    }
    else if((PIN2&0xf0)!=0)
    {
        if((PassThruConfig.GndCom&0x0f)==0)
        {
            if((LOGIC_V2&0x0f)==1)
            {
                printf_usb2("ONLY_LINE1_TX_NORMAL\r\n");
                ONLY_LINE1_TX_NORMAL;//line1正向发送
                tx_MODE_EN_channel2(tx_mode_LINE1,0XF0,NORMAL,NORMAL);
 
                tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_HIGH);//LINE 0  PULL UP
                tx_CTRL_EN_channel2(JC2_LINE0_PD, JC_LOW);//LINE 0  PULL DOWN

                tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_LOW);//LINE 1  PULL UP
                tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_LOW);//LINE 1  PULL DOWN
 		   tx_CTRL_EN_channel2_write();
                PassThruConfig.TxMode = MODE_ONLY_LINE1_TX_NORMAL;
            }
            else
            {
                printf_usb2("ONLY_LINE1_TX_REVERSE\r\n");
                ONLY_LINE1_TX_REVERSE;//line1反向发送
                tx_MODE_EN_channel2(tx_mode_LINE1,0XF0,REVERSE,NORMAL);
                tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_LOW);//LINE 0  PULL UP
                tx_CTRL_EN_channel2(JC2_LINE0_PD, JC_LOW);//LINE 0  PULL DOWN

                tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_LOW);//LINE 1  PULL UP
                tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_HIGH);//LINE 1  PULL DOWN
 				tx_CTRL_EN_channel2_write();
                PassThruConfig.TxMode = MODE_ONLY_LINE1_TX_REVERSE;
            }
        }
        else if((PassThruConfig.GndCom&0x0f)==1)//line0接地
        {
            if((LOGIC_V2&0x0f)==1) //line1正向发送,line0接地
            {
				printf_usb2("LINE1_TX_LINE0_GND_NORMAL\r\n");
				LINE1_TX_LINE0_GND_NORMAL;
	 
				 	
                tx_MODE_EN_channel2(tx_mode_LINE1,0xf0,NORMAL,NORMAL);
                //tx_MODE_EN_channel2(tx_mode_LINE1,tx_mode_LINE1,REVERSE,NORMAL);
				tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_HIGH);//LINE 0  PULL UP
				tx_CTRL_EN_channel2(JC2_LINE0_PD,JC_LOW);//LINE 0  PULL DOWN
								
				tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_HIGH);//LINE 1  PULL UP
				tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_LOW);//LINE 1  PULL DOWN
				tx_CTRL_EN_channel2_write();
			    LINE0_OR_LINE1_LOW;
				PassThruConfig.TxMode = MODE_LINE1_TX_LINE0_GND_NORMAL;
            }
            else
            {
				printf_usb2("LINE1_TX_LINE0_GND_REVERSE\r\n");

                tx_MODE_EN_channel2(tx_mode_LINE1,0XF0,REVERSE,NORMAL);

                
                LINE1_TX_LINE0_GND_REVERSE;//line1反向发送，line0接地
				tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_HIGH);//LINE 0  PULL UP
				tx_CTRL_EN_channel2(JC2_LINE0_PD,  JC_LOW);//LINE 0  PULL DOWN
								
				tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_LOW);//LINE 1  PULL UP
				tx_CTRL_EN_channel2(JC3_LINE1_PD, JC_HIGH);//LINE 1  PULL DOWN
				tx_CTRL_EN_channel2_write();
               // LINE0_OR_LINE1_LOW;
                PassThruConfig.TxMode = MODE_LINE1_TX_LINE0_GND_REVERSE;
            }
        }
    }
    //输入模式设置

    
    if(((PIN1&0X0F)!=0)&((PIN2&0X0F)!=0))
    {

        if((PassThruConfig.Format&0x0f)==07)//volvo专用，上拉，但不发命令，接收命令反转
        {
            HIGH_RES;//输出设置成高阻
            LINE0_LINE1_RX_VOLVO;
            printf_usb2("LINE0_LINE1_RX_VOLVO\r\n");
			tx_MODE_EN_channel2(0XF0,1,NORMAL,REVERSE);
			tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_HIGH);//LINE 0  PULL UP
			tx_CTRL_EN_channel2(JC2_LINE0_PD, JC_LOW);//LINE 0  PULL DOWN
							
			tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_HIGH);//LINE 1  PULL UP
			tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_LOW);//LINE 1  PULL DOWN
			tx_CTRL_EN_channel2_write();
			PassThruConfig.RxMode=MODE_LINE0_LINE1_RX_VOLVO;
        }
        else
        {
              
            if(((LOGIC_V1&0x0f)==1)&&((LOGIC_V2&0x0f)==0))//差分line0上拉输入
            {
                DIFFERENTIAL_RX_LINE0UP;

                tx_MODE_EN_channel2(0XF0,0X00,NORMAL,NORMAL);

                //tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_HIGH);//LINE 0  PULL UP
                //tx_CTRL_EN_channel2(JC2_LINE0_PD, JC_LOW);//LINE 0  PULL DOWN

                //tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_HIGH);//LINE 1  PULL UP
                //tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_LOW);//LINE 1  PULL DOWN
 				tx_CTRL_EN_channel2_write();
            
                printf_usb2("DIFFERENTIAL_RX_LINE0UP\r\n");
				PassThruConfig.RxMode=MODE_DIFFERENTIAL_RX_LINE0UP;
            }
            if(((LOGIC_V1&0x0f)==0)&&((LOGIC_V2&0x0f)==1))//差分line0下拉输入
            {
                DIFFERENTIAL_RX_LINE0DOWN;
                tx_MODE_EN_channel2(0XF0,04,NORMAL,REVERSE);

                //tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_HIGH);//LINE 0  PULL UP
                //tx_CTRL_EN_channel2(JC2_LINE0_PD, JC_LOW);//LINE 0  PULL DOWN

                //tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_HIGH);//LINE 1  PULL UP
                //tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_LOW);//LINE 1  PULL DOWN
				tx_CTRL_EN_channel2_write();

                 printf_usb2("DIFFERENTIAL_RX_LINE0DOWN\r\n");
				 PassThruConfig.RxMode=MODE_DIFFERENTIAL_RX_LINE0DOWN;
            }
            if(((LOGIC_V1&0x0f)==1)&&((LOGIC_V2&0x0f)==1))
            {
				LINE0_LINE1_RX_NORMAL;
				tx_MODE_EN_channel2(0XF0,0X03,NORMAL,NORMAL);
                
				tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_HIGH);//LINE 0  PULL UP
				tx_CTRL_EN_channel2(JC2_LINE0_PD, JC_LOW);//LINE 0  PULL DOWN
                
				tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_HIGH);//LINE 1  PULL UP
				tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_LOW);//LINE 1  PULL DOWN
				tx_CTRL_EN_channel2_write();
				printf_usb2("LINE0_LINE1_RX_NORMAL\r\n");
				PassThruConfig.RxMode=MODE_LINE0_LINE1_RX_NORMAL;
            }
            if(((LOGIC_V1&0x0f)==0)&&((LOGIC_V2&0x0f)==0))
            {
                LINE0_LINE1_RX_REVERSE;
                tx_MODE_EN_channel2(0XF0,02,REVERSE,NORMAL);

                tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_LOW);//LINE 0  PULL UP
                tx_CTRL_EN_channel2(JC2_LINE0_PD, JC_HIGH);//LINE 0  PULL DOWN

                tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_LOW);//LINE 1  PULL UP
                tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_HIGH);//LINE 1  PULL DOWN
                tx_CTRL_EN_channel2_write();
                printf_usb2("LINE0_LINE1_RX_REVERSE\r\n");
				PassThruConfig.RxMode=MODE_LINE0_LINE1_RX_REVERSE;
            }
        }

    }
    else if((PIN1&0X0F)!=0)
    {

        if((LOGIC_V1&0x0f)==1) //line0正向发送,line1接地
        {

            if((PassThruConfig.Format &0x0f)==07)//volvo专用，上拉，但不发命令，接收命令反转
            {
                LINE0_RX_VOLVO;
                tx_MODE_EN_channel2(0XF0,0,NORMAL,REVERSE);
                printf_usb2("LINE0_RX_VOLVO\r\n");
                tx_MODE_EN_channel2(0,0,NORMAL,REVERSE);
                tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_HIGH);//LINE 0  PULL UP
                tx_CTRL_EN_channel2(JC2_LINE0_PD, JC_LOW);//LINE 0  PULL DOWN			
                tx_CTRL_EN_channel2_write();

                PassThruConfig.RxMode=MODE_LINE0_RX_VOLVO;
            }
            else
            {
                ONLY_LINE0_RX_NORMAL;
                tx_MODE_EN_channel2(0XF0,0,NORMAL,NORMAL);

                printf_usb2("ONLY_LINE0_RX_NORMAL\r\n");
                tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_HIGH);//LINE 0  PULL UP
                tx_CTRL_EN_channel2(JC2_LINE0_PD, JC_LOW);//LINE 0  PULL DOWN       
				tx_CTRL_EN_channel2_write();
                PassThruConfig.RxMode=MODE_ONLY_LINE0_RX_NORMAL;
            }
        }
        else
        {
            ONLY_LINE0_RX_REVERSE;//line0反向发送，line1接地
			tx_MODE_EN_channel2(0XF0,0,NORMAL,NORMAL);
			
			printf_usb2("ONLY_LINE0_RX_REVERSE\r\n");
			PassThruConfig.RxMode=MODE_ONLY_LINE0_RX_REVERSE;
        }
    }
    else if((PIN2&0X0F)!=0)
    {
        if((LOGIC_V2&0x0f)==1) //line0正向发送,line1接地
        {
            if(PassThruConfig.Format &0x0f==07)//volvo专用，上拉，但不发命令，接收命令反转
            {
                LINE1_RX_VOLVO;
                tx_MODE_EN_channel2(0XF0,0,NORMAL,REVERSE);
                printf_usb2("LINE1_RX_VOLVO\r\n");


                tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_HIGH);//LINE 1  PULL UP
                tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_LOW);//LINE 1  PULL DOWN
				tx_CTRL_EN_channel2_write();		
				PassThruConfig.RxMode=MODE_LINE1_RX_VOLVO;
            }
            else
            {
				ONLY_LINE1_RX_NORMAL;
				printf_usb2("ONLY_LINE1_RX_NORMAL\r\n");
				tx_MODE_EN_channel2(0XF0,1,NORMAL,NORMAL);
			
				tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_LOW);//LINE 0  PULL UP
				tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_HIGH);//LINE 0  PULL DOWN
				#if 1
				
				//<CMD>A5 A5 00 28 13 01 00 25 80 02 00 03 C0 04 D0 0C C3 C1 05 20 20 20 1A 06 00 19 06 03 07 00 08 70 09 00 0A 00 00 00 0B 00 0C 02 00 00 A3</CMD>
				//<RECE RD="初始化下位机端口成功">A5 A5 00 01 13 EB</RECE>
				//<SEND SD="回应版本号">A5 A5 00 02 33 EF DB</SEND>
					
				tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_HIGH);//LINE 1  PULL UP
				tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_LOW);//LINE 1  PULL DOWN
		        #endif 
				tx_CTRL_EN_channel2_write();

				PassThruConfig.RxMode=MODE_ONLY_LINE1_RX_NORMAL;
            }
        }
        else
        {
                ONLY_LINE1_RX_REVERSE;//line0反向发送，line1接地
                printf_usb2("ONLY_LINE1_RX_REVERSE\r\n");

                tx_MODE_EN_channel2(0XF0,0,NORMAL,REVERSE);

                tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_LOW);//LINE 1  PULL UP
                tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_HIGH);//LINE 1  PULL DOWN
 		        tx_CTRL_EN_channel2_write();
                PassThruConfig.RxMode=MODE_ONLY_LINE1_RX_REVERSE;
        }
    }
    else
        return FALSE;
		  
		//引脚设置
		 switch((GND_COM) & 0x0f)
		{
			case 0:
				SetLine_ext(PassThruConfig.Pin1,PassThruConfig.Pin2);
				break;
			case 1:
				SetLine_ext(GND_COM& 0xf0,PassThruConfig.Pin2);
				break;
			case 2:
				SetLine_ext(PassThruConfig.Pin1,GND_COM & 0xf0);
				break;   
			default:
				SetLine_ext(PassThruConfig.Pin1,PassThruConfig.Pin2);
				break;
		}
		#endif
    return TRUE;
}

uint8 PassThru_Default_SetConfig(void)
{
	PassThruConfig.BaudRate       = 10400;//通讯波特率 5-500000 BaudRate
	// PassThruConfig.Loopback       = 0; //0:OFF  1:ON
	// PassThruConfig.NodeAddress    = 0;//J1850PWM总线地址
	// PassThruConfig.NetworkLine    = 0;//J1850PWM总线设置 0 (BUS_NORMAL) 1 (BUS_PLUS) 2 (BUS_MINUS)
    PassThruConfig.P1Min          = 0;
    PassThruConfig.P1Max          = 20;//0x1-0xFFFF (0.5 ms per bit) 默认40 (20 ms)
    PassThruConfig.P2Min          = 0;
    PassThruConfig.P2Max          = 0;
    PassThruConfig.P3Min          = 55;//0x1-0xFFFF (0.5 ms per bit) 默认110 (55 ms)
    PassThruConfig.P3Max          = 0;
    PassThruConfig.P4Min          = 5;//0x1-0xFFFF (0.5 ms per bit) 默认10 (5 ms)
    PassThruConfig.P4Max          = 0;
    PassThruConfig.W0             = 300;//0x0-0xFFFF (1 ms per bit) 默认300 TOOL发送地址码之前总线的最小空闲时间 ISO 9141
    PassThruConfig.W1             = 1000;//0x0-0xFFFF (1 ms per bit) 默认300 ECU收到地址码之后回复55的最大等待时间 ISO 9141 or ISO 14230
    PassThruConfig.W2             = 1000;//0x0-0xFFFF (1 ms per bit) 默认20  ECU回复kb1的最大等待时间 ISO 9141 or ISO 14230
    PassThruConfig.W3             = 1000;//0x0-0xFFFF (1 ms per bit) 默认20  kb1和kb2之间的最大等待时间 ISO 9141 or ISO 14230
    PassThruConfig.W4             = 50;//0x0-0xFFFF (1 ms per bit) 默认50  TOOL发送kb2取反的最小等待时间 ISO 9141 or ISO 14230
    PassThruConfig.W5             = 300;//0x0-0xFFFF (1 ms per bit) 默认300 TOOL发送地址码之前总线的最小空闲时间 ISO 14230
    PassThruConfig.Tidle          = 300;//0x0-0xFFFF (1 ms per bit) 默认300 快速初始化开始之前总线的最小空闲时间  ISO 9141 or ISO 14230
    PassThruConfig.Tinil          = 25;//0x0-0xFFFF (1 ms per bit) 默认25  快速初始化开始之前低脉冲时间  ISO 9141 or ISO 14230
    PassThruConfig.TWUP           = 50; //0x0-0xFFFF (1 ms per bit) 默认50  快速初始化唤醒脉冲持续时间 ISO 9141 or ISO 14230
    PassThruConfig.Parity         = 0;//0 (NO_PARITY)  1 (ODD_PARITY) 2 (EVEN_PARITY)
//  PassThruConfig.BitSamplePoint = 80;//0-100 (1% per bit)  CAN协议采样率
//  PassThruConfig.SyncJumpWidth  = 15; //0-100 (1% per bit)  CAN协议同步宽度
    PassThruConfig.T1Max          = 20;//0x0-0xFFFF (1 ms per bit) 默认20   SCI_A_ENGINE, SCI_A_TRANS, SCI_B_ENGINE or SCI_B_TRANS
    PassThruConfig.T2Max          = 100;//0x0-0xFFFF (1 ms per bit) 默认100  SCI_A_ENGINE, SCI_A_TRANS, SCI_B_ENGINE or SCI_B_TRANS
    PassThruConfig.T3Max          = 50;//0x0-0xFFFF (1 ms per bit) 默认50   SCI_A_ENGINE, SCI_A_TRANS, SCI_B_ENGINE or SCI_B_TRANS
    PassThruConfig.T4Max          = 20;//0x0-0xFFFF (1 ms per bit) 默认20   SCI_A_ENGINE, SCI_A_TRANS, SCI_B_ENGINE or SCI_B_TRANS
    PassThruConfig.T5Max          = 100;//0x0-0xFFFF (1 ms per bit) 默认100  SCI_A_ENGINE, SCI_A_TRANS, SCI_B_ENGINE or SCI_B_TRANS
    PassThruConfig.ISO15765BS[0]     = 0;//中间层转换多帧数据块大小 暂时不确定
    PassThruConfig.ISO15765STmin[0]  = 0;//中间层转换多帧分割时间 暂时不确定
    PassThruConfig.ISO15765BS[1]     = 0;//中间层转换多帧数据块大小 暂时不确定
    PassThruConfig.ISO15765STmin[1]  = 0;//中间层转换多帧分割时间 暂时不确定
    PassThruConfig.BSTx           = 0xffff;//TOOL发送多帧数据块大小
    PassThruConfig.STMinTx        = 0xffff;//TOOL发送多帧分割时间
	PassThruConfig.DataBits       = 0;//0 (8 data bits) 1 (7 data bits)
	PassThruConfig.FiveBaudMod    = 0;//有些明白 0:ISO9141-2 and ISO14230-4 1:ISO9141kb2取反 2:ISO9141接收地址取反 3:ISO9141
//  PassThruConfig.ISO15765WftMax = 0;//不明白
    PassThruConfig.Listen=0;        //00诊断 01模拟 CANBUS:7F/80/81/82/83
    PassThruConfig.CompV=25;         //比较电压
    PassThruConfig.CAN_RES=0;       //canbus 10 是否加电阻   0 加 1 不加   ADD BY cyl
    PassThruConfig.CanAnsT=0; 			//canbus 0d号参数，
    PassThruConfig.CanAnsR=0;	//canbus 0e号参数第一个参数
    PassThruConfig.Send_Pass=0;
    PassThruConfig.Recv_Pass=0;
    PassThruConfig.CanFill=0x0000;	//00默认数据 01填充数据
    PassThruConfig.CanRecvTimeOut=200*2;//f6里默认为200,原来f8为50ms,统一一起改为200ms
    PassThruConfig.RxMode=0;
    PassThruConfig.TxMode=0;
    return TRUE;
}

#define JA0  1
#define JA1  2
#define JA2  3
#define JA3  4
#define JA4  5
#define JA5  6
#define JA6  7
#define JA7  8
#define JA8  9
#define JA9  10
#define JA10 11
#define JA11 12
#define JA12 13
#define NC   14 
// #define JB0  0
// #define JB1  1
// #define JB2  2
// #define JB3  3
// #define JB4  4
// #define JB5  5
// #define JB6  6
// #define JB7  7
// #define JB8  8
// #define JB9  9
// #define JB10 10
// #define JB11 11
// #define JB12 12
// #define NC   16 
// //DB15pin映射到译码器输入值 
// static  const uint8 DB15_TO_Decoder[16]={
// 	NC,	   //DB15没有0脚
// 	//NC,   //16
// 	JA11, //11
// 	JA9,  //9
// 	JA7,  //7
// 	NC,	  //16
// 	NC,
// 	JA4,  //4
// 	JA2,  //2
// 	JA0,  //0
// 	JA12, //12
// 	JA10,  //10
// 	JA8,   //8
// 	JA6,   //6
// 	JA5,   //5
// 	JA3,   //3
// 	JA1	   //1
// };

static  const uint8 DB15_TO_Decoder[16]={
	NC,
	JA0,
	JA1,
	JA2,
	NC,
	NC,
	JA3,
	JA4,
	JA5,
	JA6,
	JA7,
	JA8,
	JA9,
	JA10,
	JA11,
	JA12
};

void tx_rx_mode_change(void)
{
	switch(PassThruConfig.TxMode)
	{
		case MODE_DIFFERENTIAL_TX_LINE0UP: 
			DIFFERENTIAL_TX_LINE0DOWN;
			break; 
		case MODE_DIFFERENTIAL_TX_LINE0DOWN: 
			DIFFERENTIAL_TX_LINE0UP;
			break; 
		case MODE_ONLY_LINE0_TX_NORMAL:
			ONLY_LINE1_TX_NORMAL;
			break; 
		case MODE_ONLY_LINE0_TX_REVERSE: 
			ONLY_LINE1_TX_REVERSE;
			break; 
		case MODE_ONLY_LINE1_TX_NORMAL: 
			ONLY_LINE0_TX_NORMAL;
			break; 
		case MODE_ONLY_LINE1_TX_REVERSE: 
			ONLY_LINE0_TX_REVERSE;
			break; 
	}
	//UARTAnswerErr(0x02,PassThruConfig.RxMode);
	switch(PassThruConfig.RxMode)
	{
		// 		case MODE_LINE0_LINE1_RX_VOLVO:
		// 			 break;
		case MODE_DIFFERENTIAL_RX_LINE0UP:
			DIFFERENTIAL_RX_LINE0DOWN;
			break;
		case MODE_DIFFERENTIAL_RX_LINE0DOWN:
			DIFFERENTIAL_RX_LINE0UP;
			break;
		// 		case MODE_LINE0_LINE1_RX_NORMAL:
		// 			 break;
		// 		case MODE_LINE0_LINE1_RX_REVERSE:
		// 			 break;
		case MODE_LINE0_RX_VOLVO:
			LINE1_RX_VOLVO;
			break;
		case MODE_ONLY_LINE0_RX_NORMAL:
			ONLY_LINE1_RX_NORMAL;	
			break;
		case MODE_ONLY_LINE0_RX_REVERSE:
			ONLY_LINE1_RX_REVERSE;
			break;
		case MODE_LINE1_RX_VOLVO:
			LINE0_RX_VOLVO;
			break;
		case MODE_ONLY_LINE1_RX_NORMAL:
			ONLY_LINE0_RX_NORMAL;
			break;
		case MODE_ONLY_LINE1_RX_REVERSE:
			ONLY_LINE0_RX_REVERSE;
			break;
	}
}

/*
*********************************************************************************************************
* 函数名称：SetLine_ext(uint8 Line0_Channel,uint8 Line1_channel)
* 函数功能：设置Line0通道(对应ODBII）
* 入口参数：通道号
* 出口参数：HardVersion:mini vci 3.3v,100k和10k分压,0.2v,0.3v版本
                        mini iobd 3.3v 100k和20k分压,0.5v,0.6v版本
*********************************************************************************************************
*/
void SetLine_ext(uint8 Line0_Channel,uint8 Line1_channel)
{
	uint8 temp;
	uint8 temp_line0;
	uint8 temp_line1;
 
	switch(HardVersion)
	{
		case 2:
		case 3:
// 		case 5:
// 		case 6:
			//line0配置
			temp=(((Line0_Channel>>4)&0x0f)|(Line0_Channel&0x0f));
            temp_line0=temp;

			ChannelBuf[4]&=0xf0;
			ChannelBuf[4]|=DB15_TO_Decoder[temp]&0xf;
			if(temp<9)
				LINE0_0_5;
			else
				LINE0_6_12;
			//line1配置
			temp=(((Line1_channel>>4)&0x0f)|(Line1_channel&0x0f));
            temp_line1=temp;
			ChannelBuf[4]&=0x0f;
			ChannelBuf[4]|=(DB15_TO_Decoder[temp]<<4)&0xf0;;
			if(temp<9)
				LINE1_0_5;
			else
				LINE1_6_12;
			break;
// 			case 7:
// 			case 8:
		case 5:
		case 6:
			temp=(((Line1_channel>>4)&0x0f)|(Line1_channel&0x0f))|(((Line0_Channel<<4)&0xf0)|(Line0_Channel&0xf0));

			break;
		default:
			break;
	}
	//tx_PINSEL_channel();
	 
	tx_PINSEL_channel2(temp_line0,temp_line1);
}
/*
*********************************************************************************************************
* 函数名称：SetLine0Channel()
* 函数功能：设置Line0通道(对应ODBII）
* 入口参数：通道号
* 出口参数：
*********************************************************************************************************
*/

void  SetLine0Channel(uint8 ChannelNum)//jb
{
	uint8 temp;
    uint8 i=0;
	uint8 temp_line0;
	switch(HardVersion)
	{
		case 0:
			temp=(((ChannelNum>>4)&0x0f)|(ChannelNum&0x0f));
			temp_line0=temp;
			ChannelBuf[4]&=0xf0;
			ChannelBuf[4]|=DB15_TO_Decoder[temp]&0xf;
			if(temp<9)
				LINE0_0_5;
			else
				LINE0_6_12;
            tx_PINSEL_channel2(temp_line0,0);

            
			break;
		case 1:
			break;
		default:
			break;
	}

}
/*
*********************************************************************************************************
* 函数名称：SetLine1Channel()
* 函数功能：设置LINE1通道(对应ODBII）
* 入口参数：通道号
* 出口参数：
* 说明 :   通道号为0时悬空
*********************************************************************************************************
*/
void SetLine1Channel(uint8 ChannelNum)//ja,低两个数据控制ja
{ 
	uint8 temp;
	uint8 temp_line1;
	if(HardVersion==1)
		return; 

    //BIT0~BIT3：
    //00：DISABLE；
    //BIT0~BIT3：1:K1  2:K2
    //BIT4~BIT7:  通道号
     
	temp=(((ChannelNum>>4)&0x0f)|(ChannelNum&0x0f));
	temp_line1=temp;
	ChannelBuf[4]&=0x0f;
	ChannelBuf[4]|=(DB15_TO_Decoder[temp]<<4)&0xf0;;
	if(temp<9)
		LINE1_0_5;
	else
		LINE1_6_12;
    
 	tx_PINSEL_channel2(0,temp_line1);
 	

}
/*********************************************************************************************
* 名称    ： DelayN_Count_Get_Logic()
* 功能    ： 延时若干计数器时间,并在此时间内判断线上有无电平变化，有下降沿表明线上有有郊数据
* 入口参数：n_Count:计数器值  
*            line_status:线上状态变化值，0或1两种,表示线上状态和line_status一样时退出时间等待,表示有数据在传播
*            
* 出口参数： 0:表示时间到线上电平没变化，others:表示计数器计时到此数据时，线上有数据变化
* 说明    ：
***********************************************************************************************/

uint32 DelayN_Count_Get_Logic_fpclk_1000(uint32 n_Count,uint8 line_status)
{ 
		//ms
		//TC_CMR_TCCLKS_TIMER_CLOCK5 = SLCK = 32.768KHz
		//?????? = 32.768 : 1
		//???1 - 1900ms
		uint16_t ms=1;
		if (ms > 1900 || ms < 1)  //min 1ms  max 1900ms
		return 0;
		#if 0
		tc_init(TC1, 1, TC_CMR_TCCLKS_TIMER_CLOCK5 |TC_CMR_CPCTRG);  //TC_CMR_TCCLKS_TIMER_CLOCK5
		uint32_t rc_temp;
		rc_temp = ms * 34;  //??????????
		tc_write_rc(TC1,1,(uint16_t)rc_temp);
		tc_start(TC1, 1);
		
		while(1)
		{
			if( (tc_get_status(TC1,1) & TC_SR_CPCS) == 0)
			{
				return 0;
			}
			if(GET_LINE_STATUS()==line_status)//等到线上有电平变化，表明有数据在传播
			{
				return TIM0_TC;
			}
		}
	#endif
}
/*********************************************************************************************
* 名称    ： DelayN_Count_Get_Logic()
* 功能    ： 延时若干计数器时间,并在此时间内判断线上有无电平变化，有下降沿表明线上有有郊数据
* 入口参数：n_Count:计数器值  
*            line_status:线上状态变化值，0或1两种,表示线上状态和line_status一样时退出时间等待,表示有数据在传播
*            
* 出口参数： 0:表示时间到线上电平没变化，others:表示计数器计时到此数据时，线上有数据变化
* 说明    ：
***********************************************************************************************/
uint32 DelayN_Count_Get_Logic_count(uint32 n_Count,uint8 line_status)
{ 
	// 定时器1初始化 
	if(GET_LINE_STATUS()==line_status)
		return TIM0_TC;
	//Int_time(0,n_Count);		        // 启动定时器	  						                        
	//while (TIME_INIT==1)// 等待定时时间到  
	{		
		if(GET_LINE_STATUS()==line_status)//等到线上有电平变化，表明有数据在传播
		{
			return TIM0_TC;	
		}
	}
	//GetLineLevelStatus();
	return 0;
}
extern  uint8_t gUSART1_Handler;
extern  uint8_t gUSART0_Handler;
extern void printferror(const char *fmt,...);
uint8 Get_Bus_status(void)
{
	if((gUSART1_Handler==0)&&(gUSART0_Handler==0))return FALSE;
	else
	return TRUE;
}

extern uint8 set_compv;
extern void hal_SET_CMP_Volt(uint8 Volt);
void SetCompV(uint8 CompV)
{
	hal_SET_CMP_Volt(CompV);
}

//extern void AdcPinInit(ioport_pin_t pin,enum ioport_direction dir,ioport_mode_t mode);
extern void Bus1AdcPc31Init(void);
extern uint32_t Bus1AdcPc31ValueGet(void);
extern  HARDWARETEST_BUS1_CMP_V_SERVICE_FUNCTION(float Output_Volt);
extern  HARDWARETEST_VCC_DRV_SERVICE_FUNCTION(uint32_t VCC_DRV);

uint32 Read_Dac(uint8 Pass_Select)
{
	uint32 temp_v;

	if(Pass_Select==mux0_vcc)
	{
		temp_v = ADC_GET(Pass_Select);
	}
	else if(Pass_Select==mux1_vcc)
	{
		temp_v = ADC_GET(Pass_Select);
	}
	else if(Pass_Select==cmp_vcc)
	{
		temp_v = ADC_GET(Pass_Select);
	}
	else if(Pass_Select==drv_vcc)
	{
		temp_v = ADC_GET(Pass_Select);
	}
	else if(Pass_Select==ver_vcc)
	{
		temp_v = ADC_GET(Pass_Select);
	}
	else if(Pass_Select==bat_vcc)
	{
		temp_v = ADC_GET(Pass_Select );
	}
	 
	return temp_v;
}
