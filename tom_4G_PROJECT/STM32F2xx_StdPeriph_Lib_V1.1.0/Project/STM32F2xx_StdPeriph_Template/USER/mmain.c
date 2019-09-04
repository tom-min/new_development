/****************************************Copyright (c)****************************************************
**                            Guangzhou ZLGMCU Development Co., LTD
**
**                                 http://www.zlgmcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           main.c
** Last modified Date:  2010-09-28
** Last Version:        V1.0
** Descriptions:        The main() function example template
**
**--------------------------------------------------------------------------------------------------------
** Created by:          Lan wuqiang
** Created date:        2010-09-28
** Version:             V1.00
** Descriptions:        整理模板，添加用户应用程序
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
#include "newsoftuart.h"
#include "conf_PDU.h"
#include "selftest.h"
#include "hardware.h"
#include "common.h"
#include "PassThru.h"
#include "config.h"
#include "PassThru_KWP.h"
#include "PassThru_PWM.h"
#include "PassThru_VPW.h"
#include "PassThru_CAN.h"
#include "PassThru_WAVE.h"
#include "PassThru_BENZ.h"
#include "PassThru_ISO.h"
#include "PassThru_PLUSE.h"
#include "PortAssign.h"
#include "code.h"
#include "can.h"
#include "user_queue.h"
#include "interface.h"
#include "common.h"
#include "FreeRTOS.h"
#include "timers.h" 
#include "stm322xg_eval.h"
#include "udp_queue.h"
#include "user_config.h"
 PDU_VERSION_STRUCT PDU_Version;			//PDU版本信息
 PDU_COMMON_DATA_STRUCT MinivciCan;
 uint8_t EcuComRxBuf[ECURXBUFLEN];
 
 
//#include "bluewifi.h"
extern void UARTAnswer(uint8 cmd);

extern uint8 PassThru_Default_SetConfig(void);

//返回到上位机信息包定义
#define INFO_PACK_TYPE        0xFF  //信息包ID
#define CHECKSUM_ERR     0xFE  //校验和失败
#define UNKNOW_TYPE      0xFD  //不认识的类型
#define UNKNOW_CMD       0xFC  //不认识的CMD类型
#define INIT_ECU_FAIL    0xFB  //初始化ECU失败
#define INIT_PORT_FAIL   0xFA  //初始化端口失败
#define REPEAT_ININT_ECU 0xF9  //正在重复初始化
#define INIT_PORT_OK     0xF8  //端口初始化成功
#define INIT_ECU_OK      0xF7  //ECU初始化完毕
#define WAITTING         0xF6  //请等待
#define ECU_COM_STOP     0xF5  //通讯中断
#define PACK_OK          0xF4  //数据包正确
#define CAN_TX_ERR       0xF3	//CAN 发送错误
extern PDU_COMMON_DATA_STRUCT MinivciCan;

extern void UARTAnswerErr(uint8 cmd,uint8 para);
//下位机软件版本

//extern PASSTHRU_CONFIG PassThruConfig;
//extern PASSTHRU_DRIVER PassThruDriver;

// QU extern FLASH_Status ErasePage(unsigned int StartPage,unsigned int EndPage);
// QU extern FLASH_Status ProgramDatatoFlash(unsigned int *Address,unsigned short *Data,unsigned int DataNum);

//extern UARTMODE UartCtlReg;
uint8 EntryReadFlag;
uint8 DelayTxSwitch = 0;

//4k数据记录
uint32 flash_pos;//用来存贮要发送的4k数据的地址组号，第次操作完成一组后才可以再操作另一组
uint16 flash_record_data_num[8];
uint16 last_data_num;
uint8 last_data[256];
uint8 flash_Erase_mark=0;
uint8 set_compv=0;
uint32 PIN_AND_VCC=0;//用一个32位的数来记录两个PIN引脚设置和设置的电压
#if 1
/*********************************************************************************************
* 名称    ：SetBusAndMode
* 功能    ：设置驱动方式
* 入口参数：来自上位机发来的buf
*
*
* 出口参数： DIAGNOSE ：诊断模式
*            SIMULATION：模拟模式
*            INTERCEPTION：侦听模式
*            CONTINUE：继续当前模式
* 说明    ：
*
***********************************************************************************************/
const Init_var_type SetBusModeLib[] =
{
	SET_CAN_BUS, PassThru_CAN_SetConfig, PassThru_CAN_InitPort, CHANEL0,//0x10
				//	SET_VPW_BUS, PassThru_VPW_SetConfig, PassThru_VPW_InitPort, CHANEL0,//0x11暂时用11替10
	SET_VPW_BUS, PassThru_VPW_SetConfig, PassThru_VPW_InitPort, CHANEL0,//0x11
	SET_PWM_BUS, PassThru_PWM_SetConfig, PassThru_PWM_InitPort, CHANEL0,//0x12
	SET_KWP2K_BUS, PassThru_KWP_SetConfig, PassThru_KWP_InitPort, CHANEL0,//0x13
	SET_AUDI_BUS, PassThru_ISO_SetConfig, PassThru_ISO_InitPort, CHANEL0,//0x14
	SET_BENZ_BUS, PassThru_BENZ_SetConfig, PassThru_BENZ_InitPort, CHANEL0,//0x15
	SET_FLASH_BUS, PassThru_PLUSE_SetConfig, PassThru_PLUSE_InitPort, CHANEL0,//0x16
   // SET_CAN_TWO_BUS, PassThru_CAN_SetConfig, PassThru_CAN_InitPort, CHANEL1,//0x17
	SET_WAVE_BUS, PassThru_WAVE_SetConfig, PassThru_WAVE_InitPort, CHANEL0,//0x1D
};

/* 用来存贮数据
   a5 a5 x1 x2 fd x3 x4 x?.....x? cs
   x1 x2 本串数据长度
   x3 bit7:1表示中间帧;0表示结束帧;bit6:1，表示要清除flash,0表示已清除过；低首六位表示本串数据存贮到第几个4k空间，最多存贮七个4k
   x4 计数器,表示存贮该4k空间第n次发送命令
   其它表示要发送的数据，8个字节为一组
*/
void record_data(uint8 *InstPack)
{
	uint8 flash_tmp[256];
	uint8 flash_record_group;
	uint16 j,len;
	uint8 start_record_flash;
	uint32 tmp_flash_addr;
	#if 0
	start_record_flash=(FLASH_BL_ADDR-FLASH_ADDRESS_START)/PAGE_SIZE;
	UartTxQueueBuf[0]=0xa5;
	UartTxQueueBuf[1]=0xa5;
	UartTxQueueBuf[2]=0x00;
	UartTxQueueBuf[3]=0x06;
	UartTxQueueBuf[4]=0xfd;
	UartTxQueueBuf[8]=0x00;
	UartTxQueueBuf[9]=0x00;
// 	UartTxQueueBuf[0]=0x00;
// 	UartTxQueueBuf[1]=0x06;
// 	UartTxQueueBuf[2]=0xfd;
// 	UartTxQueueBuf[6]=0x00;
// 	UartTxQueueBuf[7]=0x00;
	flash_record_group=InstPack[3]&0x3f;//记录的组号
	UartTxQueueBuf[5]=InstPack[3]&0xbf;
	UartTxQueueBuf[6]=InstPack[4];//记数器
	len=(InstPack[0]<<8)+InstPack[1]-3;//去掉0xfd以及两位标志位
	if((len>0x100)||(flash_record_group>0x02))
	{
		UartTxQueueBuf[7]=0x00;
		USART_DMA_Send_ext_1768();//数据太多或组号太大，出错
		return;
	}
	if(InstPack[3]&0x40)//刚开始存贮数据，需要先删除下flash才可以进入写入动作
	{
		//Erase_flash(FLASH_SECNUM_BL);
		ErasePage(start_record_flash,start_record_flash+8);//最后8k用来存贮数据
		
		//write_flash_only(FLASH_SECNUM_BL,flash_tmp,256,FLASH_BL_ADDR);//把0x68000开始的前256个数据写回 mini中无此项，不再增加
		flash_pos=0x00;//刚开始准备存入数据，所以地址位偏移为零
		memset(flash_record_data_num,0,sizeof(flash_record_data_num));
		flash_Erase_mark=0xff;
	}
	if(flash_Erase_mark!=0xff)
	{
		UartTxQueueBuf[7]=0x02;
		USART_DMA_Send_ext_1768();//没清除过flash,不可保存
		return;
	}
	flash_record_data_num[flash_record_group]+=len;
	
	if(flash_record_data_num[flash_record_group]>0x1000)//保存数据太多，大于4k,报错退出
	{
		UartTxQueueBuf[7]=0x0ff;
		USART_DMA_Send_ext_1768();//没清除过flash,不可保存
		return;
	}
	tmp_flash_addr=FLASH_BL_ADDR+0X1000*(flash_record_group)+flash_pos;
	if(last_data_num==0)
	{
		memcpy(last_data,InstPack+5,len);//直接拷贝到内存暂存
		if((len<0x100) && ((InstPack[3]&0x80)==0x80))//中间帧并且小于256个，暂存
		{
			last_data_num=len;
		}
		else
		{
			ProgramDatatoFlash((unsigned int *)(&tmp_flash_addr),(unsigned short*)last_data,0x100);
			//write_flash_only(FLASH_SECNUM_BL,flash_tmp,0x100,tmp_flash_addr);
			//结束帧把所有数据全部写入
		}
	}
	else//原来有暂存数据
	{
		if((len+last_data_num)>=0x100)
		{
			j=0x100-last_data_num;
			memcpy(last_data+last_data_num,InstPack+5,j);
			ProgramDatatoFlash((unsigned int *)(&tmp_flash_addr),(unsigned short*)last_data,0x100);
			//write_flash_only(FLASH_SECNUM_BL,last_data,0x100,tmp_flash_addr);//写入数据
			flash_pos+=0x100;
			memcpy(last_data,InstPack+5+j,len-j);
			last_data_num=len-j;
			tmp_flash_addr=FLASH_BL_ADDR+0X1000*(flash_record_group)+flash_pos;
		}
		else
		{
			memcpy(last_data+last_data_num,InstPack+5,len);
		}
		if((InstPack[3]&0x80)==0)//结束帧，所有数据存贮
		{
			ProgramDatatoFlash((unsigned int *)(&tmp_flash_addr),(unsigned short*)flash_tmp,0x100);
			//write_flash_only(FLASH_SECNUM_BL,last_data,0x100,tmp_flash_addr);
			last_data_num=0;
		}
			
	}
  	UartTxQueueBuf[7]=0x01;
	UartTxQueueBuf[8]=(flash_record_data_num[flash_record_group]>>8)&0xff;
	UartTxQueueBuf[9]=flash_record_data_num[flash_record_group]&0xff;
	//quwenqi 	USART_DMA_Send_ext_1768();//正确，
	#endif
}
static uint8 _SetBusAndMode(uint8 *InstPack) 
{
    uint8 temp8;
    uint8 i;
    temp8 = InstPack[2];
    
    ProtocolType = PROTOCOL_MASK & temp8;
    
    for (i = 0; i < GET_COUNTOF(SetBusModeLib); i++) 
    {
		if (SetBusModeLib[i].BusType == temp8) 
        {
            if (FALSE == SetBusModeLib[i].ParseFunc((uint8 *) InstPack, SetBusModeLib[i].ch))//参数解析
            {
				UARTAnswer(INIT_PORT_FAIL);     //初始化端口失败,因为参数不对
				return FALSE;
            }
            else 
            {
				UARTAnswer(temp8);   //应答成功
				if (ValidFlag == 1) 
				{
					SetBusModeLib[i].PortInitFunc(SetBusModeLib[i].ch);
					StatusByte |= PORT_INIT_OK;
				}
				
                // ecumsCounter = _1msCounter;
                return TRUE;
            }
        }
    }
    return FALSE;
}

//获取本版卡故障码
static void GetErrCode(void) 
{
}

 
extern void printferror(const char *fmt,...);
static uint8 _KeepAlWayFunc(uint8 *InstPack)
{
	uint16_t Length;
	uint8_t  *Message;
	
	Length = (uint16_t)((InstPack[0]<<8) + InstPack[1]);
	Message = InstPack+3;
	 
	printferror("gKeepalwaysFlag==%02x",*Message);
	if(*Message)
	PassThruDriver.KeepAlwayComFunc=PassThruDriver.TxDataToEcuFunc;
	else
	PassThruDriver.KeepAlwayComFunc=0;
	 
}

extern void hardware_test(uint8_t Protocol, uint16_t wParam, uint8_t *lParam);
static uint8 _SysFunc(uint8 *InstPack) 
{
	uint16 i;
	uint32 ad;
	uint16_t Length;
	uint8_t  *Message;
	//printf_usb2("_SysFunc InstPack[2]==%02x",InstPack[2]);
	Length = (uint16_t)((InstPack[0]<<8) + InstPack[1]);
	Message = InstPack+4;
   //GetVer
    
	switch (InstPack[2])
	{
        case 0xf0:   //版本号
             GetVer(0x05);                //获取版本信息
            break;
			
        case 0xf1:

          //quwenqi   Reset_Line();
           //quwenqi  Reset_All_IO();//所有io初始化成初始状态(和obd引脚连接处)
            Reset_Fun();//所有功能函数置空
//             LPC_CANAF->AFMR = ACCF_OFF;
//             CAN_CLEAR_ACCF();
//             LPC_CANAF->AFMR = ACCF_FULLCAN;
           // InitTxBufToQueue();//重置接收队列
            memset((uint8 * ) & PassThruConfig, 0, sizeof(PASSTHRU_CONFIG));
            memset((uint8 * ) & PassThruDriver, 0, sizeof(PASSTHRU_DRIVER));
            memset((uint8 * ) & ComCtl, 0, sizeof(ComCtl));
            memset(ChannelBuf, 0, sizeof(ChannelBuf));
           //quwenqi  PassThru_Default_SetConfig();//设置缺省参数
            ComCtl.bitnum = 8; //一字节等于8bit

           //quwenqi  UARTAnswerErr(0xf1, 0x01);
						break;
            //PassThru_CAN_GetCanPara();			//获取CAN控制器数据
        case 0xf2:
           //quwenqi  Read_Ver(InstPack);    //读产品序列号
		   hardware_test(InstPack[3], Length, Message);		//设备硬件测试
            break;
        case 0xf3:
          //quwenqi   USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//关闭中断接收
          //quwenqi   Write_Ver(InstPack);    //写产品序列号
          //quwenqi   USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断接收
            break;
        case 0xf4:

           //quwenqi  SetCompV(0x35);
            SetVcc_Drv(DRIVE_5V);
            BENZ_TX;
            ONLY_LINE0_RX_NORMAL;
			//             SetLine0Channel(1);    //DAT
			//             SetLine1Channel(9);    //CLK
			//quwenqi 	SetLine_ext(1,9);
             DelayNms(300);
            tx164dat(0x58);
            i = rx164dat();
          //quwenqi   UARTAnswerErr(0xf4, i & 0xff);
            //GetADvalue(InstPack[3]);	//读取端口电压
            break;
        case 0xf5:
            //DelayNms((InstPack[3] << 8) + InstPack[4]);	//延时若干ms
            // i=Set_Line(InstPack);
            UARTAnswerErr(0xf5, i);
            break;
        case 0xf6:
            // GetP0status();			//读取端口状态
            //SetEclk(100,InstPack[3],InstPack[4]);
            //Get_Line0_Line1_status(InstPack);
            //quwen GetP0status();
            break;
        case 0xf7:
            //SetTxPort(InstPack);	//设置驱动端口状态
            //	Set_Line0_Line1_Status(InstPack);
            //Set_Another_Line(InstPack);//暂时未用
            SetTxPort(InstPack);
            break;
        case 0xf8:
            GetErrCode();            //获取板卡故障码
            break;
        case 0xf9:
           //quwenqi  USART_ClearFlag(USART1,USART_FLAG_TC);
           //quwenqi  TIM_Cmd(TIM2, DISABLE);
            //IWDG_Enable();
           //quwenqi  __set_FAULTMASK(1);      
          //quwenqi   NVIC_SystemReset();//  复位
            break;
        case 0xfa:
            //InitTxBufToQueue();////重置接收队列
            PassThruDriver.ReadBusFunc = NULL;    //停止接收
			PassThruDriver.KeepAlwayComFunc = NULL;	 
			
			PassThruDriver.TxDataToEcuFunc= NULL;
			PassThruDriver.KeepComFunc= NULL;
			KeepComSwitch2= AUTO_KEEP_OFF;
			KeepComSwitch = AUTO_KEEP_OFF;
			UartTxSwitch = SEND_RUN;

            break;
				
        case 0xfb://获取loader关键字 pt[8]==0xed,pt[12]==0xef,pt[0x188]==0x04,表示最新loader
            //DMA_Cmd(DMA1_Channel5, DISABLE); //关闭DMA接收
            //update_load(InstPack);    //写产品序列号
            //DMA_Cmd(DMA1_Channel5, ENABLE); //开始DMA接收
            //quwenqi  addr08_return();
            break;
        case 0xfd:
            record_data(InstPack);
            break;
        case 0xfc:

           Reset_All_IO();
            switch (InstPack[3]) 
             {

                case 0x00://短路测试
				 
                    Reset_All_IO();
                     TestShortCiruit();
                    break;
                case 0x01://断路测试
				 
                    Reset_All_IO();
				   
					switch(HardVersion)
					{
						case 0x05:
						case 0x06:
							 TestOpen_iobd();
						break;
						case 0x02:
						case 0x03:
						default:
							 TestOpenCiruit();
						break;
					}
					 
                    break;
                case 0x02://设置端口状态
                    /***********************
                    入口参数， 	InstPack+4: 0:LINE0 1:LINE1
                    InstPack+5: JB0~JB12 (ja0~ja12)
                    InstPack+6: 0:NO_LOAD 1:UP_LOAD 2:DOWN_LOAD
                    吸合相应的干簧管，并按要求上拉下拉或悬空
                    ***********************/
                    setPortStatus(InstPack);

                    break;
                case 0x03://得到端口状态
                    getPortStatus(InstPack);
                    break;
                case 0x04://can单点自检
					//can1 是否接在can网络上 
					  Reset_All_IO();
					//can_res_test();
					// Reset_All_IO();
					  PassThru_CAN_SELFTEST(InstPack[4], InstPack[5],
					  (InstPack[6] << 16) + (InstPack[7] << 8) + InstPack[8], InstPack[9]);
					 Reset_All_IO();
                    break;
                case 0x05://驱动电压测试
                    Reset_All_IO();
                    Drv_Test();                    //同步ok
                    break;
                case 0x06://比较电压测试,同步ok
                    Reset_All_IO();
                    Cmp_Test();
                    break;
                case 0x07://上下拉电压测试
									
                    Reset_All_IO();

					switch(HardVersion)
					{
 							//case 0x05:
 							//case 0x06:
 							//up_down_test_iobd();
 							//break;
 							//case 0x02:
 							//case 0x03:
						default:
							up_down_test();
						break;
					}
				
                    break;
                case 0x09://方波输出测试
                    Reset_All_IO();
                      nissan_pulse_test(InstPack);
                    break;
                case 0x0a://kl线和can信号切换继电器测试
                    Reset_All_IO();
                    test_kl_can();
                    break;
                case 0x0b:// KL线上设置,注意，stm32定时器为16位数据，故此时byte12应为0
                   //quwenqi Reset_All_IO();
                 //quwen   kl_setup(InstPack);
                    break;
                case 0x0c://kl线上脉冲测试
                    Reset_All_IO();
					//quwen  kl_pulse();
                   //quwenqi UartTxQueuePack(EcuComRxBuf);
                    break;
                case 0x0d://根据初始化io来判断需要调整的脉冲宽度
                    Reset_All_IO();
                 //quwen   adjust_pulse_width();

                  //quwenqi  UartTxQueuePack(EcuComRxBuf);
                    break;
                case 0x0E://控制网络接口接通
//                     if (InstPack[4] == 1) {
//                         Reset_All_IO();
//                         SET_TP;
//                     }
//                     else
//                         CLR_TP;

                    UARTAnswerErr(0xfc, 0x0e);
                    break;
								case 0x10://获取当前设置的驱动电压
										  drv_vcc_test();
								break;
// 								case 0x11://会在初始化io后读出line0 line1和供电压,如果相差不大,可能信号经接到电源上,会自动发上来,并断开引脚切换
// 									break;
								case 0x12://读取蓝牙地址
									//quwenqi Read_bl_addr();
									break;
								case 0x13://读取所有版本号
									//quwenqi GetVer(10);
									break;
								case 0x16://设置比较电压，设死
									set_compv=InstPack[4];
									 SetCompV(set_compv);
									 DelayNms(1000);
									 ad=Read_Dac(cmp_vcc);
									EcuComRxBuf[0] = 0x00;
									EcuComRxBuf[1] = 0x03;
									EcuComRxBuf[2] = 0xfc;
									EcuComRxBuf[3] = 0x14;
									EcuComRxBuf[4] =ad/100;
									//EcuComRxBuf[4]=((EcuComRxBuf[4]/10)<<4)+(EcuComRxBuf[4]
									 UartTxQueuePack(EcuComRxBuf);
									break;
								case 0x17://设置13和14模式的波特率和发送接收模式,
									//第一个字节表示发送接收方式0x00:软串口 0x01:硬串口
									//后续三个字节表示波特率：
								switch(InstPack[4])
								{
									case 0x00:
										ComCtl.RxHardWare = USE_IO; //用软件接收
										PassThruDriver.RxByteDrv       = SoftUartRxByte;
										PassThruDriver.TxByteDrv       = _SoftTxByte;
										PassThruConfig.BaudRate=(InstPack[5]<16)+(InstPack[6]<8)+InstPack[7];
										SetSoftUart(PassThruConfig.BaudRate); //设置波特率
										break;
									case 0x01:
										ComCtl.RxHardWare = USE_HARD; //用硬件接收
										//quwenqiPassThruDriver.RxByteDrv       = MCU_UARTGetByte;
										//quwenqiPassThruDriver.TxByteDrv       = MCU_UARTSendByte;
										UartCtlReg.Baud=(InstPack[5]<16)+(InstPack[6]<8)+InstPack[7];
										UartCtlReg.datab = uart_data8;
										UartCtlReg.stopb = uart_stopbit_1;
										//quwenqiMCU_UART_Init(UartCtlReg);
										break;
									}
									
								
							
							break;
//                 case 0xf0://获取硬件测试结果
//                    // getAutoTestStatus(InstPack, (InstPack[4] & 0x0f) * MAXSIZE_TESTFLAG);
//                     break;
				
                default://else if(InstPack[3] > 0xf0)//工装测试（执行部门：委托加工厂）
                    //setAutoTestStatus(InstPack, (InstPack[3] & 0x0f) * MAXSIZE_TESTFLAG);
                    break;
            }
            break;
        case 0xff: //激活
		
            //InitTxBufToQueue();////重置接收队列
            KeepComSwitch = AUTO_KEEP_OFF;
            UartTxSwitch = SEND_RUN;

            if (InstPack[3] == 0x00) 
             {
                ValidFlag = 1;
                UARTAnswerErr(0xff, 0x01);
            }
            else 
            {
                ValidFlag = 1;
                UARTAnswerErr(0xff, 0x00);
            }
            break;
        default:
            break;
    }

    return TRUE;
}

//特殊功能
static uint8 SpecialFun(uint8 *buf) 
{
    switch (buf[2]) 
    {
        case 0x60:
            UartTxSwitch = SEND_RUN;
            KeepComSwitch = AUTO_KEEP_OFF;
            PassThruDriver.KeepComFunc = NULL;//在设置0x60命令后清空自动保持赋值
				 UARTAnswerErr(0x60, buf[3]); 	//v7.0
            break;
        case 0x61:
            switch (buf[3]) 
            {
                case 0x00:
                    KeepComSwitch = KEEP_COM_ANS_OFF | AUTO_KEEP_ON; //=0X03 //V7.01 //不屏蔽自动通讯时ECU的回应
                    break;
                case 0x01:
                    KeepComSwitch = KEEP_COM_ANS_ON | AUTO_KEEP_ON; //=0X01 //V7.01 LZS //屏蔽自动通讯保持时ECU的回应
                    break;
                case 0x02:
                    KeepComSwitch = KEEP_COM_ANS_OFF | AUTO_KEEP_ON |
                                    KEEP_COM_R_DELAY_OFF; //读后不延时，定时插入发送,这种情况一般是只发送不接收，如VPW,某些CAN
                    break;
                default:
                    break;
            }
            PassThruDriver.KeepComFunc = ALL_KEEP_COM;//在设置0x61命令后再将自动何持赋值
             UARTAnswerErr(0x61, buf[3]); //v7.0
            break;
        case 0x62:
            switch (buf[3]) 
            {
                case 0x00:
                    KeepComSwitch1 = KEEP_COM_ANS_OFF | AUTO_KEEP_ON; //=0X03 //V7.01 //不屏蔽自动通讯时ECU的回应
                    break;
                case 0x01:
                    KeepComSwitch1 = KEEP_COM_ANS_ON | AUTO_KEEP_ON; //=0X01 //V7.01 LZS //屏蔽自动通讯保持时ECU的回应
                    break;
                case 0x02:
                    KeepComSwitch1 = KEEP_COM_ANS_OFF | AUTO_KEEP_ON |
                                     KEEP_COM_R_DELAY_OFF; //读后不延时，定时插入发送,这种情况一般是只发送不接收，如VPW,某些CAN
                    break;
                default:
                    break;
            }
            PassThruDriver.KeepComFunc = ALL_KEEP_COM;//在设置0x61命令后再将自动何持赋值
            UARTAnswerErr(0x62, buf[3]); //v7.0
            break;
        default:
            break;
    }
    return TRUE;
}


extern uint32_t Bus1CmpAndDrvAdcValueGet(void);
extern void Bus1CmpAndDrvAdcInit(void);


void vcc_drv_v(void)
{
    SetVcc_Drv(DRIVE_12V);//启动时测试下供电电压,用来测试时和信号电压比较,看是否接到电源线上
    Bus1CmpAndDrvAdcInit();
    DelayNms(5);
    vcc_drv= (Bus1CmpAndDrvAdcValueGet()+500)/100;
	#if 1
	if((vcc_drv<=(120))&&(vcc_drv>=(120-5)))
	{
		vcc_drv=120;
	}
	if((vcc_drv<=(240))&&(vcc_drv>=(240-5)))
	{
		vcc_drv=240;
	}
	#endif
}
//LINE0和line1脚接完后试下引脚,看下引脚是否接到电源上,如果接到电源上,则复位引脚和函数
void detcect_singnal_vcc(void)
{
    uint16 ByteNum=0;
    uint16 temp1,temp2;
    //EN_Singal_det;
    DelayNms(10);
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = 0xfc;
    EcuComRxBuf[ByteNum++] = 0x11;
	
	DelayNms(5);
	Read_Dac(mux0_vcc);
	DelayNms(5);
	temp1=Read_Dac(mux0_vcc)/100;
	 
    EcuComRxBuf[ByteNum++]=(temp1>>8)&0xff;//Read_Dac(mux0_vcc)/100;
    EcuComRxBuf[ByteNum++]=temp1&0xff;
    DelayNms(5);
    Read_Dac(mux1_vcc);
    DelayNms(5);
     temp2=Read_Dac(mux1_vcc)/100;
    EcuComRxBuf[ByteNum++]=(temp2>>8)&0xff;//Read_Dac(mux1_vcc)/100;
    EcuComRxBuf[ByteNum++]=temp2&0xff;
    //     DelayNms(5);
    //     Read_Dac(drv_vcc);
    //     DelayNms(5);
    EcuComRxBuf[ByteNum++]=(vcc_drv>>8)&0xff;//Read_Dac(drv_vcc)/100;
    EcuComRxBuf[ByteNum++]=vcc_drv&0xff;//Read_Dac(drv_vcc)/100;
    ByteNum-=2;
    EcuComRxBuf[0] = ByteNum >> 8;
    EcuComRxBuf[1] = ByteNum & 0xff;
    //	USART_Send_DMA(USART1,EcuComRxBuf,256);

    if(((vcc_drv-temp1)<0x5)||((vcc_drv-temp2)<0x05))
    {
    	
    	UartTxQueuePack(EcuComRxBuf);
    	Reset_Line();
    }

    //UartTxQueuePack(EcuComRxBuf);
    //Dis_Singnal_det;
}

#if 1
void CmdMcufunction(uint8 chanel,uint8 Ord_type, uint8 *TempProcBuf)
{
}



void CmdBusfunction(uint8 chanel,uint8 Ord_type, uint8 *TempProcBuf)//quwenqi
{
	 
    endmsCounter = 0;//chengbb 20130531 初始化计数器
     //Reset_Line();
     Reset_All_IO();
	 
    // vcc_drv_v();//单线can 的第一帧会发出12v 电压 ，异常
	 
     G_RXD_DIS;
     _SetBusAndMode((uint8 *) TempProcBuf);
     G_RXD_EN;
     
     //detcect_singnal_vcc();//测试下是否接到电源线,如果接到电源线,则所有引脚断开
     //
     KeepComSwitch = AUTO_KEEP_OFF;
     memset(KeepComBuf, 0, sizeof(KeepComBuf));
     memset(CanbusInsert, 0, KEEP_COM_BUF_LEN);
	  

}

void CmdEcufunction(uint8 chanel,uint8 Ord_type, uint8 *TempProcBuf)
{

	uint8 (*init_pass)(uint8*);
	uint8 (*Send_pass)(uint8*);
		 
		 
	if(chanel==0)
	{
	//   Read_pass=PassThruDriver.ReadBusFunc;
		 
		init_pass=PassThruDriver.InitEcuFunc;
		Send_pass=PassThruDriver.TxDataToEcuFunc;
	}
	//  else
	//  {
	//      //Read_pass=PassThruDriver.ReadBusFunc_TWO;
	//      init_pass=PassThruDriver.InitEcuFunc_TWO;
	//      Send_pass=PassThruDriver.TxDataToEcuFunc_TWO;
	//  }


      
    if (kwp_5bpsdata_delay_sent_flag == 1) 
    {
        kwp_5bpsdata_delay_sent_flag = 0;
        PassThru_KWP_delay_sent();
    }
	if (init_pass != NULL)
	{
		
		init_pass((uint8 *) TempProcBuf);
		
	}

    //modfiy by wangs 20160418 重新初始化ecu需要继续发送链路保持。
    if ((KeepComSwitch & AUTO_KEEP_COM_SWITCH_BIT) != AUTO_KEEP_ON) 
    {
		KeepComSwitch = AUTO_KEEP_OFF;
		memset(KeepComBuf, 0, sizeof(KeepComBuf));
    }
    else
    {
		KeepComTimeCounter[0] = KeepComTime[0];
		KeepComTimeCounter[1] = KeepComTime[1];
    }
    DelayTxSwitch = 0;

}
void CmdDatfunction(uint8 chanel,uint8 Ord_type, uint8 *TempProcBuf)
{
	uint8 (*init_pass)(uint8*);
	uint8 (*Send_pass)(uint8*);
    
	 
    if(chanel==0)
    {
        //Read_pass=PassThruDriver.ReadBusFunc;
        init_pass = PassThruDriver.InitEcuFunc;
        Send_pass = PassThruDriver.TxDataToEcuFunc;
    }
    //  else
    //  {
    //      Read_pass=PassThruDriver.ReadBusFunc_TWO;
    //      init_pass=PassThruDriver.InitEcuFunc_TWO;
    //      Send_pass=PassThruDriver.TxDataToEcuFunc_TWO;
    //  }

    if (Send_pass != NULL)
    {
		 
        Send_pass((uint8 *) TempProcBuf);
	 
    }
    
    if((KeepComSwitch & AUTO_KEEP_COM_RXTX_DELAY_BIT) != KEEP_COM_R_DELAY_OFF)
    {
		//if (KeepComSwitch != AUTO_KEEP_OFF) 
        KeepComTimeCounter[0] = KeepComTime[0];
        KeepComTimeCounter[1] = KeepComTime[1];
    }
    if(TempProcBuf[7]==0xf1)
    DelayTxSwitch = 0;
    
    DelayTxSwitch = 0;

}   
void CmdKeepComfunction(uint8 chanel,uint8 Ord_type, uint8 *TempProcBuf)
{
    //00 0D 40 00 00 00 1C 02 3E 02 00 00 00 00 00
    if(TempProcBuf[2] == 0x40 && GetAddresses32(TempProcBuf+3) == 0x001c)
    {
       memcpy(CanbusInsert, TempProcBuf, TempProcBuf[0] + TempProcBuf[1] + 3);
       UARTAnswerErr(TempProcBuf[2], 0x01); //chengbb 回应40/41/42/43/44  //quwenqi
    }
    else
    {
        memcpy(KeepComBuf, TempProcBuf, TempProcBuf[0] + TempProcBuf[1] + 3);
        KeepComBuf[2] &= 0x0f;
        KeepComBuf[2] |= 0x30;
        UARTAnswerErr(TempProcBuf[2], 0x00); //chengbb 回应40/41/42/43/44
    }

}
void CmdDelayTxfunction(uint8 chanel,uint8 Ord_type, uint8 *TempProcBuf)
{
    //PassThruDriver.TxDataDelayFunc((uint8 *)TempProcBuf);
              DelayTxSwitch = 1;
			  

}
void CmdDelayTx5kfunction(uint8 chanel,uint8 Ord_type, uint8 *TempProcBuf)
{
    //K2K_5bpsdata_delay_parse(TempProcBuf);
        PassThru_KWP_delay_parse(TempProcBuf);
        UARTAnswerErr(TempProcBuf[2], 01);
}

void CmdSpecialMaskfunction(uint8 chanel,uint8 Ord_type, uint8 *TempProcBuf)
{
    SpecialFun(TempProcBuf);
}

void CmdConIdfunction(uint8 chanel,uint8 Ord_type, uint8 *TempProcBuf)
{
    CAN_ID_FILT_FLAG = 1;
     PassThru_CAN_TxPack_SetFilter(TempProcBuf);

}
void CmdFlashMaskfunction(uint8 chanel,uint8 Ord_type, uint8 *TempProcBuf)
{
    if(PassThruDriver.TxFlashDataToEcuFunc!=NULL)
    PassThruDriver.TxFlashDataToEcuFunc((uint8 *)TempProcBuf);
}


void CmdSysFunfunction(uint8 chanel,uint8 Ord_type, uint8 *TempProcBuf)
{
      _SysFunc(TempProcBuf);
}  

void CmdSwitchFunfunction(uint8 chanel,uint8 Ord_type, uint8 *TempProcBuf)
{
	//GetProtocolVer(&TempProcBuf[3]);    
}  


//extern void Work_Thread_PDU(uint8_t *pPduPacket);
//extern uint32_t gMinivciPduPacketFlag;

void CmdKeepAlwayFunfunction(uint8 chanel,uint8 Ord_type, uint8 *TempProcBuf)
{
	// 仪表显示
	_KeepAlWayFunc(TempProcBuf);
}

void PduPackketFunfunction(uint8 chanel,uint8 Ord_type, uint8 *TempProcBuf)
{
	//PDU诊断入口
	//gMinivciPduPacketFlag = 1;
	//Work_Thread_PDU(&TempProcBuf[3]);
}

extern void printferror(const char *fmt,...);
extern void printftoolsdebug(const char *dat);
//extern  uint32_t gusbresume;
void Transmit_Pass(uint8 chanel,uint8 Ord_type, uint8 *TempProcBuf) 
{	
   
//	uint8 (*Read_pass)(void);
	uint8 (*init_pass)(uint8*);
	uint8 (*Send_pass)(uint8*);
	 //gMinivciPduPacketFlag = 0 ;
	if(chanel==0)
	{
//	 Read_pass=PassThruDriver.ReadBusFunc;
	 init_pass=PassThruDriver.InitEcuFunc;
	 Send_pass=PassThruDriver.TxDataToEcuFunc;
	}
// 	else
// 	{
// 		//Read_pass=PassThruDriver.ReadBusFunc_TWO;
// 		init_pass=PassThruDriver.InitEcuFunc_TWO;
// 		Send_pass=PassThruDriver.TxDataToEcuFunc_TWO;
// 	}
   // if(gusbresume ==1)vTaskDelay(4);
	 
	printftoolsdebug(TempProcBuf);
    switch ((Ord_type & 0xf0) >> 4) //不包括 a5 a5 头，所以这里指的是命令类型
    {
        case (SET_MCU_MASK >> 4): //MCU操作
            CmdMcufunction(chanel,Ord_type,TempProcBuf);
            break;
        case (SET_BUS_MASK >> 4): //设备初始化 InitIo
            CmdBusfunction(chanel,Ord_type,TempProcBuf);
            break;
        case (INIT_ECU_MASK >> 4): //ECU初始化
            CmdEcufunction(chanel,Ord_type,TempProcBuf);
            break;
        case (DATA_MASK >> 4):    //数据通讯
		 
            CmdDatfunction(chanel,Ord_type,TempProcBuf);
            break;
        case (FLASH_MASK>>4)://将flash中的数据一次性发出去
            CmdFlashMaskfunction(chanel,Ord_type,TempProcBuf);
            break;
        case (KEEP_COM_MASK >> 4)://通讯保持
            CmdKeepComfunction(chanel,Ord_type,TempProcBuf);
            break;
        case (DELAY_TX_MASK >> 4):    //延时发送 53命令
            CmdDelayTxfunction(chanel,Ord_type,TempProcBuf);
            break;
        case (SPECIAL_MASK >> 4): //特殊功能
             CmdSpecialMaskfunction(chanel,Ord_type,TempProcBuf);
              break;
        case (CAN_ID_FILT >> 4): //CAN ID 过滤
             CmdConIdfunction(chanel,Ord_type,TempProcBuf);
              break;
        case (0x80 >> 4): //23命令 5bps数据 延时发送
             CmdDelayTx5kfunction(chanel,Ord_type,TempProcBuf);
            break;

        //case (LKT4200_MASK >> 4):    //系统功能
        //UART_4200_Function(TempProcBuf);
        //break;


        case (PROTOCOLTYPE>> 4):
			CmdSwitchFunfunction(chanel,Ord_type,TempProcBuf);                
			break;
			
           case (PDUTYPE>> 4):
           PduPackketFunfunction(chanel,Ord_type,TempProcBuf);
           break;
		   
		case (KEEP_ALWAY>> 4):
			CmdKeepAlwayFunfunction(chanel,Ord_type,TempProcBuf);
			break;
			
		case (SYS_FUN_MASK >> 4):    //系统功能
			CmdSysFunfunction(chanel,Ord_type,TempProcBuf);
			break;
			
		default: 
			break;
    }
}

#endif

// 用来判断是否收到一帧完整数据
// void Adjust_usar_data(void)
// {
// 	uint8 chk_sum=0;
// 	uint16 i;
// 	uint16 use_pos=0;
// 	uint16 num;
//   if(usart_len>=5)//去掉废字节
// 	{
// 		while(use_pos<usart_len-1)
// 		{
// 			if((USART1_RxBuff_temp[use_pos]!=0xA5)||(USART1_RxBuff_temp[use_pos+1]!=0xA5))
// 			{
// 				use_pos++;
// 			}
// 			else
// 				break;
// 			if(use_pos==(use_pos-1))//废字节太多,也需要跳出
// 				break;
// 		}
// 	}
// 	
// 	if((usart_len-use_pos)>5)
// 	{
// 		zyIrqDisable();
// 		num=(USART1_RxBuff_temp[2+use_pos]<<8) + USART1_RxBuff_temp[3+use_pos] + 5;
// 		if(num<=(usart_len-use_pos))
// 		{
// 			memcpy(TempProcBuf,USART1_RxBuff_temp+use_pos+2,num-2);
// 			usart_len=usart_len-use_pos-num;
// 			memcpy(USART1_RxBuff_temp,USART1_RxBuff_temp+num+use_pos,usart_len);
// 			for(i=0;i<num-3;i++)
// 			{
// 				chk_sum+=TempProcBuf[i];
// 			}
// 			chk_sum=~chk_sum;
// 			if(chk_sum==TempProcBuf[num-3])//收到正确的完整的一帧?
// 			{
// 				USART1_RxFlag = 1;           //接收数据标志位置1
// 			}
// 			else
// 			{
// 				UARTAnswerErr(CHECKSUM_ERR, chk_sum);
// 			}
// 		}
// 		zyIrqEnable();
// 	}
// }
//一个循环周期： 1.36us，735.3Khz

//void TimerCreate(uint32 T_Period);
uint8 f_MainRoute=0;
uint8 f_MainRoute1=0;
extern void printfuartdebugtimestamp(const char *fmt,...);

void _MainRoute(void);
void TaskCom_Proc(void *pvParameters)
{
	uint8_t		ui_usb_bulk_in[USER_UDI_VENDOR_EPS_SIZE_BULK_HS];
	uint16_t	i;
	uint8_t		*RXdata;
	uint16_t    msglen = 0;
 
	uint8_t		ch,RxFrameFlag,RxChecksum;
	uint16_t	revPt,ByteCounter;
	unsigned long ulReceivedValue;
 
	RxFrameFlag=FrmHdr0Flg;				//初始化解析数据流状态
	 
	for (;;) 
	{
		ulReceivedValue = 0U;
		//读USB接收队列
		while (USB_BULK_Queue_Read(&USB_RX_QueueData,(uint8_t *)ui_usb_bulk_in, &msglen) == true)
		{	
			 
			for (i=0;i<msglen;i++)
			{
				ch = ui_usb_bulk_in[i];
				switch(RxFrameFlag)
				{
					case FrmHdr0Flg   : //=0
					{
						if( ch==HEADER0)
						{
							RxFrameFlag++;
							RxChecksum=0;
						}
						else
						RxFrameFlag=FrmHdr0Flg;
					}
					break;
					
					case FrmHdr1Flg   : //=1
					{
						if(ch==HEADER1)
						{
							RxFrameFlag++;
							break;
						}
						else
						RxFrameFlag = FrmHdr0Flg;    //复位帧字节标志
					}
					break;
					
					case FrmLngthHFlg :  //=2
					{
						TempProcBuf[0] = ch;
						RxChecksum = RxChecksum + ch;
						RxFrameFlag++;
					}
					break;
					
					case FrmLngthLFlg :  //=3
					{
						TempProcBuf[1] = ch;
						RxChecksum = RxChecksum + ch;
						RxFrameFlag++;
						ByteCounter = (TempProcBuf[0] << 8) + TempProcBuf[1] + 3; //2: 偏移量已经为2，1：校验和，字节长度不包括校验和 2+1=3
						revPt = 2;
					}
					break;

					case FrmLngthDat  :  //=4
					{
						TempProcBuf[revPt] = ch;
						if(revPt < ByteCounter - 1)
						{
							RxChecksum = RxChecksum + ch;
						}
						revPt++;
						if(revPt == ByteCounter)
						{
							RxFrameFlag = FrmEndFlg;
							
							if (RxFrameFlag == FrmEndFlg)
							{
								RxChecksum = ~RxChecksum;
								if (RxChecksum == TempProcBuf[revPt-1])  //check Cs
								{
																		
									//command action 诊断入口
			                                         
										_MainRoute();
								 
								}	
							}
							//准备解析下一帧
							RxFrameFlag = FrmHdr0Flg;
							revPt=0;
							ByteCounter=0;
							
							
						}
					}
					break;
					
					default:
					{
							
					}
					break;
				}
			}
		}
	 	
	 vTaskDelay(1);
	}/*for(;;)*/
}

 
void _MainRoute(void)
{
 
	while(f_MainRoute==1)
	{
		vTaskDelay(1);
	}
	while(f_MainRoute1==1)
	{
		vTaskDelay(1);
	}
	 
    f_MainRoute=1;
    DelayTxSwitch = 0;
	UartTxSwitch = SEND_RUN; //v7.0 lzs
	KeepComSwitch2 = AUTO_KEEP_ON;
	switch (TempProcBuf[2] & 0x0f)
	{
		case 7:
			PassThruConfig.Send_Pass = 1;
			break;
		default:
			PassThruConfig.Send_Pass = 0;
			break;
	}
	printfuartdebugtimestamp("rx");
	Transmit_Pass(PassThruConfig.Send_Pass,TempProcBuf[2], (uint8 *) TempProcBuf);
    f_MainRoute=0;
    
}


void PassThruDriverAlwaysSend(void)
{
	#if 0
	if((PassThruDriver.KeepAlwayComFunc!= NULL)&&(TempProcBuf[2]==0x30))
	{
		PassThruDriver.KeepAlwayComFunc(TempProcBuf);
		hal_mcan_DisableRxInterrupt(MinivciCan);
		hal_mcan_clear_RX_buffer(MinivciCan);
	}
	#endif
}

TimerHandle_t freeostime1;
 
extern uint32 gsofttimeer ;
extern uint32 gsofttimeerold ;
void  TimerCallback( TimerHandle_t xTimer )
{

	gsofttimeer++;
	counter_1ms++;
	_1msCounter ++;
 
	if(KeepComTimeCounter[0]!=0)
	KeepComTimeCounter[0]--;
	if(KeepComTimeCounter[1]!=0)
	KeepComTimeCounter[1]--;
}



void TimerCreate(void)
{
	if( freeostime1 != NULL )
	{
		return;
	}
	freeostime1=xTimerCreate( "time1",(TickType_t)1, (UBaseType_t)pdTRUE, (void *)1,(TimerCallbackFunction_t)TimerCallback);
	if( freeostime1 != NULL ) 
	{
		xTimerStart(freeostime1,0); 
	}
}


void _MainRoute1(void)
{
	f_MainRoute1=1;
		EntryReadFlag = FIRST_BYTE;
		//收发ECU数据
		if (PassThruDriver.ReadBusFunc != NULL)
		{
			if (PassThruDriver.ReadBusFunc() == TRUE)
			{
				counter_1ms = 0;
				if ((KeepComSwitch & AUTO_KEEP_COM_RXTX_DELAY_BIT) !=KEEP_COM_R_DELAY_OFF) //v7.03,解决VPW自动保持。VPW数据流自动保持特点是下位机有连续的数据包发送上来，必须在中间插入通讯保持指令才能使之不中断。
				KeepComTimeCounter[0] = KeepComTime[0];
			} 
		}
		//接收到完整数据帧之后1ms之内发送命令，此处处理J1708协议和赛欧气囊协议
		//超过50ms没有收到完整数据帧，就直接发送命令,此处处理J1708总线只有WABCO ABS的情况
		//由于发送前设置的等待时间多样化，正好卡在一帧中间的命令暂停发送
		
		if (counter_1ms < 1 || counter_1ms > 50)
		{
			//20121212 XK 53命令发送前不再判断是否接收到完整一帧 (朱燕玲)
			if (DelayTxSwitch == 1 && ((TempProcBuf[2] & 0xf0) == 0x50) && PassThruDriver.TxDataDelayFunc!=NULL)
			{ 
				PassThruDriver.TxDataDelayFunc((uint8 *) TempProcBuf);
				DelayTxSwitch = 0;
				counter_1ms = 0;
			}
		}
		
		//通讯保持
		if ((PassThruDriver.KeepComFunc != NULL) && (KeepComSwitch2 == AUTO_KEEP_ON))
		{
			PassThruDriver.KeepComFunc();
		}
		#if 0
				if((PassThruDriver.KeepAlwayComFunc!= NULL)&&(TempProcBuf[2]==0x30))
				{
					PassThruDriver.KeepAlwayComFunc(TempProcBuf);
					hal_mcan_DisableRxInterrupt(MinivciCan);
					hal_mcan_clear_RX_buffer(MinivciCan);
				}
		#endif
		if (PassThruDriver.FuncBak0 != NULL)
		PassThruDriver.FuncBak0();
	f_MainRoute1=0;
	
}
 extern void STM_EVAL_LEDToggle(Led_TypeDef Led);
void _MainRoute2(void *pvParameters)
{

	while(1)
	{
		while(f_MainRoute==1)
		{
			vTaskDelay(1);
		}

		_MainRoute1();
		vTaskDelay(1);
	}
}

void mmain(void) 
{
	//uint8 buf[256];
	//uint8 Mode;
	//初始化运行环境

	// InitRun();
	// NVIC_DisableIRQ(CAN_IRQn);
	// 初始化关键变量
	// SetSoftUart(10400);                    //默认波特率10400
 
	UartTxQueueBuf=UartTxQueueBuf_ext;//+2;
	UartTxQueueBuf_ext[0]=0xa5;
	UartTxQueueBuf_ext[1]=0xa5;
	ADC1_Init();
	TIM_Configuration();
    ByteCounter = 0;
    _10msCounter = 0;
    endmsCounter = 0;//chengbb 20130531 初始化计数器
    KeepComTime[0] = 0;
    KeepComTime[0] = 1;
    //主程序流程控制
    StatusByte = NO_ANY_INIT;         //没有任何初始化
    KeepComSwitch = AUTO_KEEP_OFF;

    //初始化关键变量
    //RxFrameFlag = FrmHdr0Flg;            //默认PWM驱动线为高电平
    //Uart_Init_Default();
    //设置默认状态
	
	INIT_All_IO();

	//SetCompV(0x70);                    //默认比较电压为7V
	hal_SET_CMP_Volt(0x85);
	Reset_Line();
	Reset_All_IO();
	USR_I2C_Config( );
    memset(KeepComBuf, 0, KEEP_COM_BUF_LEN);//清空通讯保持命令
    memset(KeepComBuf1, 0, KEEP_COM_BUF_LEN);//清空通讯保持命令
    ComCtl.bitnum = 8; //一字节等于8bit
    //
    //Mode = DIAGNOSE;
    ProtocolType = 0x0f;
    //
//    ErrCode = 0;
    ValidFlag = 1;
    //TxFrameFlag = FrmEndFlg; // 防止上电后首次时出现A5 A5 00 00 FF
   // AnswerFlag = ANSWER_OFF;

    //InitTxBufToQueue();//重置接收队列
    //init_PULL_Con();

	memset((uint8 * ) & PassThruConfig, 0, sizeof(PASSTHRU_CONFIG));
	memset((uint8 * ) & PassThruDriver, 0, sizeof(PASSTHRU_DRIVER));

	USB_BULK_Queue_Init(&USB_RX_QueueData);
	USB_BULK_Queue_Init(&USB_TX_QueueData);
	USB_BULK_Queue_Init(&OBD_RX_QueueData);
	// CAN  
	CAN_Config(SET_CAN_SJW,SET_CAN_BS1,SET_CAN_BS2,SET_CAN_PRES);

	Queue_Init(&CAN1_QueueData);
	Queue_Init(&CAN2_QueueData);

	USART_Queue_Init(&KWP0_RX_QueueData);
	USART_Queue_Init(&KWP1_RX_QueueData);


	//memset(ChannelBuf,0,sizeof(ChannelBuf));

	PassThru_Default_SetConfig();//设置缺省参数
	// TimerCreate(100);
	//NVIC_EnableIRQ(TIMER1_IRQn);
	//NVIC_EnableIRQ(TIMER0_IRQn);
	//NVIC_SetPriority(TIMER0_IRQn, 7);  //6
	//NVIC_SetPriority(TIMER1_IRQn, 6);  //6
	TimerCreate();
    
}
//https://blog.csdn.net/hisense20112784/article/details/49104843
#endif
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
