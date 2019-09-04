#include "config.h"
#include "PassThru.h"
#include "interface.h"
#include "hardware.h"
#include "PortAssign.h"
#include "NewSoftUart.h"
#include "PassThru_KWP.h"
#include "usart1.h"
extern uint8_t EcuComRxBuf[];
 
#define MCU_UART_Init
 #define UART_MR_PAR_NO 1
extern void tx_MODE_EN_channel2(uint8_t TXMODE,uint8_t RXMODE,uint8_t TX_REV,uint8_t RX_REV);
extern  HARDWARETEST_VCC_DRV_SERVICE_FUNCTION(uint32_t VCC_DRV);
extern PASSTHRU_CONFIG PassThruConfig;
extern PASSTHRU_DRIVER PassThruDriver;
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

//extern void hal_kmp_soft_uart_init_minivci(void);
//extern void hal_kmp_uart_init(void);
#ifdef MINI_HW_001
extern void  hal_kmp_uart_config_minivci(uint32_t baudrate,uint8_t parity);
#endif
#ifdef MINI_HW_002
extern void  hal_kmp_uart_config_minivci(uint32_t baudrate,uint32_t parity);
#endif


void SimulateEcu5bps()
{
    uint8  ByteData;
    uint32 DlyInByteCounter;
    uint8 DlyInTxByte = PassThruConfig.P4Min;
    
    if(PassThruConfig.Listen == SIMULATION && PassThruConfig._BaudRate > 0)
    {
        SetSoftUart(PassThruConfig._BaudRate);
        UartCtlReg.Baud=PassThruConfig._BaudRate;
        MCU_UART_Init(UartCtlReg);//UART3_Init(UartCtlReg);
        DelayNms(PassThruConfig._WaitTime);
		
		
		//hal_kmp_soft_uart_init_minivci();  //soft uart init
		//閸掑洦宕查崚鎵€栨禒绂狝RT
		//hal_kmp_uart_init();
		////闁板秶鐤嗘稉绡岰U瑜版挸澧犲▔銏㈠閻?
		//hal_kmp_uart_config_minivci(PassThruConfig._BaudRate,UART_MR_PAR_NO);
		
		
        PassThruConfig.P4Min = PassThruConfig._ByteTime;
        PassThru_KWP_TxPack_TxData(PassThruConfig.SendBuf);
        PassThruConfig.P4Min = DlyInTxByte;
        UartTxQueuePack(PassThruConfig.SendBuf);
        PassThruConfig.BaudRate = PassThruConfig._BaudRate;
        //接收KB2取反
        if((PassThruConfig.Format & 0x80) == 0x80)
        {
            //接收取反字节
            DlyInByteCounter =   PassThruConfig.W4;
            if(BYTE_OK != PassThruDriver.RxByteDrv(DlyInByteCounter, (uint8 *)&ByteData))
            {
                return;
            }
            //发送
            EcuComRxBuf[0] = 0x00;
            EcuComRxBuf[1] = 0x02;
            EcuComRxBuf[2] = PassThruConfig.SendBuf[2];
            EcuComRxBuf[3] = ByteData;
            UartTxQueuePack(EcuComRxBuf);
        }
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
*           DlyInTxByte<0x80 DlyInTxByte*1ms
*           DlyInTxByte>=0x80 (DlyInTxByte-0x80)*100us
***********************************************************************************************/
void DelayTxByte(uint8 DlyInTxByte)
{
    if(DlyInTxByte & 0x80)
        DelayNus((DlyInTxByte&0x7f)*100);
    else
        DelayNms(DlyInTxByte);
}

/*********************************************************************************************
* 名称    ：
* 功能    ：
* 入口参数：
*
*
* 出口参数：
* 说明    ：监测总线是否忙  超过500us的空闲被认为有效空闲
*
***********************************************************************************************/
uint8 GetLineMonitor(void)
{
    //监测总线是否忙
    uint8  freeCounter;
    uint16 busyCounter;

    busyCounter = 0;
    freeCounter = 0;
    for(;;)
    {
        DelayNus(100);
        if(Get_Bus_status() ==TRUE )//输入为低表示有有郊信号
        {
            freeCounter = 0;
            busyCounter++;
        }
        else
        {
            freeCounter++;
        }
        if(freeCounter > 5) break;
        if(busyCounter > 5000) return FALSE;
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
* 说明    ：接收最后一个字节时间
*
***********************************************************************************************/
void SetEndmsCounter()
{
    endmsCounter = _1msCounter;
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
//发送之前等待时间 TD2/TD3
/***********************************************************************************************/
extern void printferror(const char *fmt,...);
void SendDelayCounter()
{
    unsigned int t = _1msCounter;
    unsigned int set_delay;
    if(PassThruConfig.Listen == DIAGNOSE && (PassThruConfig.Format & 0x10))
    {
        if(PassThruConfig.P3Min&0x80)
            set_delay=(PassThruConfig.P3Min&0x7f)/10;
        else
            set_delay=PassThruConfig.P3Min;
        if(endmsCounter + set_delay > t )
        {
            DelayNms(endmsCounter + set_delay - t);
        }
    }
    else if(PassThruConfig.Listen == SIMULATION && (PassThruConfig.Format & 0x10))
    {
        if(PassThruConfig.P2Min&0x80)
            set_delay=(PassThruConfig.P2Min&0x7f)/10;
        else
            set_delay=PassThruConfig.P2Min;
        if(endmsCounter + set_delay > t )
        {
            DelayNms(endmsCounter + set_delay - t);
			 
        }
    }
}

/*********************************************************************************************
* 名称    ： RunWithWait
* 功能    ： 带等待功能的函数接口器
* 入口参数：
*
*
* 出口参数：
* 说明    ：
*
***********************************************************************************************/
static uint8 RunWithWait(uint8 pass_select)
{
    KeepComTimeCounter[pass_select] = KeepComTime[pass_select];
    if(PassThruDriver.ReadBusFunc == NULL)
    {
        return TRUE;               //帧结束标志
    }
    while(PassThruDriver.ReadBusFunc() == FALSE) //等待起始位
    {
        if(KeepComTimeCounter[pass_select] == 0)
        {
            return FALSE;               //帧结束标记
        }
    }
    return TRUE;
}

/*********************************************************************************************
* 名称    ： ALL_KEEP_COM()
* 功能    ： 所有的通讯保持
* 入口参数：
*
*
* 出口参数：
* 说明    ：
*
***********************************************************************************************/
uint8 ALL_KEEP_COM(void)
{
    //if((K2Kparameter.KeepComMode&AUTO_KEEP_COM_SWITCH_BIT)==0) //是否要自动通讯保持
    if((KeepComSwitch & AUTO_KEEP_COM_SWITCH_BIT) == AUTO_KEEP_ON)
    {
        if(KeepComTimeCounter[0] == 0)
        {
					
            if(GetAddresses32(CanbusInsert+3) == 0x001c)
            {
                PassThruDriver.TxDataToEcuFunc((uint8 *)CanbusInsert);
                DelayNms(15);
            }
            PassThruDriver.TxDataToEcuFunc((uint8 *)KeepComBuf);
            if((KeepComSwitch & AUTO_KEEP_COM_ANSWER_BIT) == KEEP_COM_ANS_ON)
            {
                UartTxSwitch = SEND_BLOCK;

                if(FALSE == RunWithWait(0)) //接收，但不发送给上位机
                {
                    UartTxSwitch = SEND_RUN;
                    //  KeepComSwitch=AUTO_KEEP_OFF; //即使没有回应也继续 发送通讯保持命令
                    return FALSE;
                }

            }
            KeepComTimeCounter[0] = KeepComTime[0]; //K2Kparameter.TD5; //我们以1ms为单位
            //根据协议类型判断帧与帧之间时间间隔
            //增加此延时的目的是：在此函数的的运行过程中，收到来自上位机的发送指令 ,
            //则此函数一退出，就会把指令发出去，导致发送与接收的时间距离太近。
            switch(ProtocolType)
            {
            case KWP2K_BUS_MASK:
            case AUDI_BUS_MASK://常雁龙 2016 1 8日，统一延时函数
                if((DelayTimeForHoldCom&0x80)==0x00)
                    DelayNms(DelayTimeForHoldCom);
                else
                    DelayNus((DelayTimeForHoldCom&0x7f)*100);
                break;
            default:
                DelayNms(15);
            }
        }
    }
    if((KeepComSwitch1 & AUTO_KEEP_COM_SWITCH_BIT) == AUTO_KEEP_ON)
    {
        if(KeepComTimeCounter[1] == 0)
        {
          //  PassThruDriver.TxDataToEcuFunc_TWO((uint8 *)KeepComBuf1);
            if((KeepComSwitch1 & AUTO_KEEP_COM_ANSWER_BIT) == KEEP_COM_ANS_ON)
            {
                UartTxSwitch = SEND_BLOCK;

                if(FALSE == RunWithWait(1)) //接收，但不发送给上位机
                {
                    UartTxSwitch = SEND_RUN;
                    return FALSE;
                }
            }
            KeepComTimeCounter[1] = KeepComTime[1]; //K2Kparameter.TD5; //我们以1ms为单位
            DelayNms(15);
        }
    }
    UartTxSwitch = SEND_RUN;
    return TRUE;
}

//调整输出参数，只在版本号为5及波特率为115200时做调整
char adjustbit(uint8 Data)
{
   // char logic_adjust;
    if(((PassThruConfig.BaudRate == 115200)||(PassThruConfig.BaudRate == 57600)) && HardVersion < 7)
    {
        if(Data>0)
            return 60;
        else
            return 0;
    }
    else
    {
        return 0;
    }
}

/*********************************************************************************************
* 名称    ： PassThru_SetKeepComSwitch2
* 功能    ： CANBUS协议设置接收过滤器
* 入口参数：
*
*
* 出口参数：
* 说明    ： 0x70专用命令 主要用于J1939协议读取数据流
*
***********************************************************************************************/
uint8 PassThru_SetKeepComSwitch2(uint8 *RXdata)
{
    if(RXdata[0] == 0x7f && RXdata[2] == 0x78)
    {
        KeepComSwitch2 = AUTO_KEEP_OFF;
    }
    else

        KeepComSwitch2 = AUTO_KEEP_ON;
    return TRUE;
}

uint32 GetAddresses32(uint8 *buffer)
{
	uint32 address = 0;
	address = (buffer[0]<<24) + (buffer[1]<<16) + (buffer[2]<<8) + buffer[3];
	return address;
}


/*
*********************************************************************************************************
* 函数名称：DriveCchannel()
* 函数功能：驱动C组继电器
* 入口参数：
*           Buf[1].7 Buf[1].6 Buf[1].5 Buf[1].4 Buf[1].3 Buf[1].2 Buf[1].1 Buf[1].0	 Buf[0].7 Buf[0].6 Buf[0].5 Buf[0].4 Buf[0].3 Buf[0].2 Buf[0].1 Buf[0].0
* 出口参数：  JC7     JC6       JC5       JC4       JC3     JC2     JC1      JC0         JC15     JC14     JC113     JC12	   JC11	    JC10      JC9     JC8
*********************************************************************************************************
*/

/*********************************************************************************************************
* 函数名称：SetVcc_Drv(uint8 vcc_drv)
* 函数功能：设置驱动电压
* 入口参数：
* 出口参数：
*********************************************************************************************************
*/
extern void printferror(const char *fmt,...);
void SetVcc_Drv(uint8 vcc_drv)
{
	printferror("vcc_drv=%D",vcc_drv);
	switch(vcc_drv)
	{
		case DRIVE_8V:
			HARDWARETEST_VCC_DRV_SERVICE_FUNCTION_NO_CLOSE(8);
		break;
		case DRIVE_12V: 
			HARDWARETEST_VCC_DRV_SERVICE_FUNCTION_NO_CLOSE(12);
		break;
		case DRIVE_5V:	
			HARDWARETEST_VCC_DRV_SERVICE_FUNCTION_NO_CLOSE(5);
		break;
	}
}


/************************
使用spi读出发出数据
*************************/
void tx164dat(uint8 dat)
{
	uint8 i;
	#if 0  //quwenqi2g
	arch_ioport_set_pin_dir(PIN_LINE0,IOPORT_DIR_OUTPUT);    //KWP OUTPUT PIN UART鍙戦€?LINE0 K
	arch_ioport_set_pin_dir(PIN_LINE0_1,IOPORT_DIR_OUTPUT);    //KWP OUTPUT PIN UART鍙戦€?LINE0 K
	#endif
	//先空移位3BIT
	for(i = 0; i <8; i++)
	{
		LINE0_OR_LINE1_LOW;//LINE1=0
		DelayNms(1);
		if((dat & 0x80))
		{
			SET_TX0_PRE_IO_HIGH;  
		}
		else
		{
			SET_TX0_PRE_IO_LOW; //LINE0=0
		}
		DelayNms(1);
		LINE0_OR_LINE1_HIGH;//LINE1=1
		DelayNms(1);
		dat <<= 1;
	}
	SET_TX0_PRE_IO_HIGH;
}
/**********
读出spi导出的数据
*************/
uint8 rx164dat(void)
{
	uint8 cmddata = 0;
	uint8 bitcount = 0;
	uint8 flag = 0;

	counter_1ms = 0;
    
	while(1)
	{		
		//vTaskDelay(1);
		if(flag == 0)//开始，2ms内没电平变化退出
		{
			if(counter_1ms>3)
			{
				return 0;	
			}
		}
		else
		{
			if(_1msCounter > 25)// 开始后25ms没收完数据，出错
			{
				return 0;
			}
		}		
		#if 0  //quwenqi2g
		if(GET_LINE0_OR_LINE1==0)//变低电平，进入
		#endif
		{
			if(flag == 0)
			{
				flag = 1;
				_1msCounter = 0;
			}
			DelayNus(200);
			#if 0  //quwenqi2g
			if(GET_LINE0_OR_LINE1==0)
			#endif
			{
				DelayNus(1000);//延时1.2ms后读取当前值
				bitcount ++;
				cmddata <<= 1;
				#if 0  //quwenqi2g
				if(GET_RX0_STATUS==1)
					cmddata |= 0x01;
				else
					cmddata &= 0xFE;
					#endif
				counter_1ms = 0;
				#if 0  //quwenqi2g
				while(GET_LINE0_OR_LINE1==0)//等待clk变高
				#endif
				{	
					//vTaskDelay(1);
					if(counter_1ms > 5)//长时间不变，退出
					{
						cmddata = 0;
						bitcount = 0;
						return	0; 
					}
				}
			}
		}
		if(bitcount == 8)
		{
			return	cmddata;
		}
	}
}

/*
*********************************************************************************************************
* 函数名称：SetExtCh
* 函数功能：设置括展通道
* 入口参数：通道号
*           
* 出口参数：
*********************************************************************************************************
*/
//把实际通道转化成逻辑通道，实际通道与接头上的标号一一对应
const uint8 BenzChConv[37]=
{
	0xff,0xff,0xff,0xff,0x07,0x06,0x05,0x04,  //4~7
	0x03,0x02,0x01,0x00,0x0f,0x0e,0x0d,0x0c,  //8~15
	0x0b,0x0a,0x09,0x08,0x17,0x16,0x15,0x14,  //16~23
	0xff,0xff,0x13,0xff,0x12,0xff,0x11,0x10,  //n,n,26,n,28,n,30,31
	0xff,0xff,0x1f,0xff,0x1e
}; //n,n,34,n,36


/*
*********************************************************************************************************
* 函数名称：SetComLineLoadMode()
* 函数功能：单独设置通讯线上下拉模式
* 入口参数：LoadMode: 通讯模式代号
*           LineNum:  线号，有两条线可选0，1
* 出口参数：
*********************************************************************************************************
*/

void SetEcuLineLoadMode(uint8 LineNum,uint8 LoadMode)
{
	switch(LineNum)
	{
	case LINE0: 
		switch(LoadMode)
		{
		case NO_LOAD: 
			//LINE0_NO_LOAD;
			//LINE0_NO_LOAD;
			tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_LOW);//LINE 0  PULL UP
			tx_CTRL_EN_channel2(JC2_LINE0_PD,JC_LOW);//LINE 0  PULL DOWN
			break;
		case UP_LOAD:
			//LINE0_UP;
			//LINE0_ PULL UP;
			tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_HIGH);//LINE 0  PULL UP
			tx_CTRL_EN_channel2(JC2_LINE0_PD,JC_LOW);//LINE 0  PULL DOWN
			break;
		case DOWN_LOAD:
			//LINE0_DOWN;
			tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_LOW);//LINE 0  PULL UP
			tx_CTRL_EN_channel2(JC2_LINE0_PD,JC_HIGH);//LINE 0  PULL DOWN
			break;
		default:
			;
		}
		break;
	case LINE1:
		switch(LoadMode)
		{
		case NO_LOAD:
			//LINE1_NO_LOAD;
			tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_LOW);//LINE 1  PULL UP
			tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_LOW);//LINE 1  PULL DOWN
			
			break;
		case UP_LOAD:
			//LINE1_UP;
			tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_HIGH);//LINE 1  PULL UP
			tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_LOW);//LINE 1  PULL DOWN
			break;
		case DOWN_LOAD:
			//LINE1_DOWN;
			tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_LOW);//LINE 1  PULL UP
			tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_HIGH);//LINE 1  PULL DOWN
			break;
		default:
			;
		}
		break;
	default:
		break;
	}
	//tx_c_channel();
}
const uint8 NewBenzChConv[10][2] = 
{ 
	{4,8}, {5,14}, {6,7}, {7,10}, {8,6},
	{10,15}, {15,3}, {16,11}, {19,2}, {30,12}
}; 
void tx_CTRL_EN_channel2_write(void);

void SetExtCh(uint8 ch)
{

	uint8 temp;
	uint8 i;
	uint8 temp_line0,temp_line1;
	//先检测新奔驰板，悬空
	SetVcc_Drv(DRIVE_5V);
	BENZ_TX_RX;
	tx_MODE_EN_channel2(tx_mode_LINE0,0X00,NORMAL,NORMAL);
	tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_HIGH);//LINE 0  PULL UP
	tx_CTRL_EN_channel2(JC2_LINE0_PD,JC_LOW);//LINE 0  PULL DOWN
	 			
	tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_HIGH);//LINE 1  PULL UP
	tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_LOW);//LINE 1  PULL DOWN
	tx_CTRL_EN_channel2(M_JC04,JC_LOW);
	tx_CTRL_EN_channel2(M_JC05,JC_LOW);
	tx_CTRL_EN_channel2_write();
	 
#if 0
	ONLY_LINE0_RX_NORMAL;
	tx_MODE_EN_channel2(0XF0,0,NORMAL,NORMAL);

	tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_HIGH);//LINE 0  PULL UP
	tx_CTRL_EN_channel2(JC2_LINE0_PD, JC_LOW);//LINE 0  PULL DOWN

	PassThruConfig.RxMode=MODE_ONLY_LINE0_RX_NORMAL;
#endif
		#if 0  //quwenqi2g
	//quwenqi G_RXD_EN;
// 	SetLine0Channel(1);	//DAT
// 	SetLine1Channel(9);	//CLK
	temp_line0=1;
	temp_line1=9;
	SetLine_ext(temp_line0,temp_line1);
	SetCompV(0x40);
	vTaskDelay(E70B_vTaskDelay_Modulus(20));
	tx164dat(0x58);
	DelayNus(200); 
	temp = rx164dat();
	printferror("temp==%02x",temp);
	if(temp == 0x4B)
	{
		DelayNus(1000); 
		tx164dat(ch);
		//SetLine0Channel(13); 
		//SetLine_ext(13,9);
		temp_line0=13;
		SetLine_ext(temp_line0,temp_line1);
		for(i=0; i<10; i++)
		{
			if(ch == NewBenzChConv[i][0])
			{
				temp_line0=NewBenzChConv[i][1];
				//SetLine0Channel(NewBenzChConv[i][1]);
				SetLine_ext(temp_line0,temp_line1);
				break;
			}
		}
	}
	else
	{
		
		SetLine0Channel(15);	//DAT
		SetLine1Channel(14);	//CLK
		vTaskDelay(E70B_vTaskDelay_Modulus(20));
		temp_line0=15;
		temp_line1=14;
		SetLine_ext(temp_line0,temp_line1);
		tx164dat(BenzChConv[ch]);
		
		SetLine_ext(13,00);
		arch_ioport_set_pin_dir(DB26_PIN13_PWR,IOPORT_DIR_OUTPUT);    //
		arch_ioport_set_pin_level(DB26_PIN13_PWR,0);
		
	}
	//SetLine1Channel(0);
	//SetLine_ext(NewBenzChConv[i][1],0);
//	ONLY_LINE0_TX_NORMAL;
	temp_line1=0;
	//SetLine_ext(temp_line0,temp_line1);
	//SetEcuLineLoadMode(LINE0, UP_LOAD);
	SetVcc_Drv(DRIVE_12V);
	#endif
}
void Reset_Fun(void) 
{
	
    PassThruDriver.InitEcuFunc = NULL;       //初始化ECU
    PassThruDriver.TxDataToEcuFunc = NULL;   //发送数据到总线 即时发送
    PassThruDriver.TxDataDelayFunc = NULL;   //发送数据到总线 延迟发送
    PassThruDriver.ReadBusFunc = NULL;          //读总线数据
    PassThruDriver.KeepComFunc = NULL;          //备用功能
	PassThruDriver.KeepAlwayComFunc = NULL;
    PassThruDriver.FuncBak0 = NULL;             //备用功能
    PassThruDriver.RxByteDrv = NULL;
    PassThruDriver.TxByteDrv = NULL;
    PassThruDriver.InitEcuFunc = NULL;       //初始化ECU
//     PassThruDriver.TxDataToEcuFunc_TWO = NULL;   //发送数据到总线 即时发送
//     PassThruDriver.ReadBusFunc_TWO = NULL;          //读总线数据*/
}
