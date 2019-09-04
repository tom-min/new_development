/**------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
********************************************************************************************************/
#ifndef __CONFIG_H 
#define __CONFIG_H

//This segment should not be modified
 #ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif
 

typedef unsigned char  uint8;                   /* defined for unsigned 8-bits integer variable 	无符号8位整型变量  */
typedef signed   char  int8;                    /* defined for signed 8-bits integer variable		有符号8位整型变量  */
typedef unsigned short uint16;                  /* defined for unsigned 16-bits integer variable 	无符号16位整型变量 */
typedef signed   short int16;                   /* defined for signed 16-bits integer variable 		有符号16位整型变量 */
typedef unsigned int   uint32;                  /* defined for unsigned 32-bits integer variable 	无符号32位整型变量 */
typedef signed   int   int32;                   /* defined for signed 32-bits integer variable 		有符号32位整型变量 */
typedef float          fp32;                    /* single precision floating point variable (32bits) 单精度浮点数（32位长度） */
typedef double         fp64;                    /* double precision floating point variable (64bits) 双精度浮点数（64位长度） */



#define USART_WordLength_8b                  ((uint16 )0x0000)
#define USART_WordLength_9b                  ((uint16 )0x1000)
#define USART_StopBits_1                     ((uint16 )0x0000)
#define USART_StopBits_0_5                   ((uint16 )0x1000)
#define USART_StopBits_2                     ((uint16 )0x2000)
#define USART_StopBits_1_5                   ((uint16 )0x3000)
#define IS_USART_STOPBITS(STOPBITS) (((STOPBITS) == USART_StopBits_1) || \
                                     ((STOPBITS) == USART_StopBits_0_5) || \
                                     ((STOPBITS) == USART_StopBits_2) || \
                                     ((STOPBITS) == USART_StopBits_1_5))


#define GET_COUNTOF(a) (sizeof(a) / sizeof(a[0]))

/* 定义串口模式设置数据结构 */

typedef struct{
   uint8 BusType;
   uint8 (*ParseFunc)(uint8 *,uint8);
   uint8 (*PortInitFunc)(uint8);
	 uint8 ch;									//2015/8/12 增加一函数，在can设置时，0:设置can0通道，1:设置can1通道
   
}Init_var_type;
typedef struct{
  
  uint8 (*InitEcuFunc)(uint8 *);       //初始化ECU
  uint8 (*TxDataToEcuFunc)(uint8 *);   //发送数据到总线
  uint8 (*ReadBusFunc)(void);          //读总线数据
  uint8 (*KeepComFunc)(void);          //备用功能
  uint8 (*FuncBak0)(void);             //备用功能
  uint8 (*RxByteDrv)(uint8*,uint8);
  void  (*TxByteDrv)(uint8);
     
}EcuDrvFuncType;

/**************************************
STM32 与lpc1768不一样定义
***************************************/

#define NO_PARITY   USART_Parity_No//0
#define ODD         USART_Parity_Odd//1
#define EVEN        USART_Parity_Even//2
#define BIT9_1      3
#define BIT9_0      4  
#define uart_data_n 
#define uart
#define uart_data8 USART_WordLength_8b
#define uart_data9 USART_WordLength_9b
#define uart_stopbit_1 USART_StopBits_1;

#define USE_IO       0  //IO接收
#define USE_HARD     1  //硬件接收

typedef struct UartMode
{
	uint16 datab;//uint8 datab;		// 字长度，5/6/7/8可选
	uint16 stopb;//uint8 stopb;		// 停止位，1/2可选
	uint16 parity;//uint8 parity;		// 奇偶校验位，0-无校验,1-奇校验,2-偶校验
	uint32 Baud;        // 波特率
}UARTMODE;

typedef struct{
   //uint8 RxChannel;         //接收通道
   //uint8 TxChannel;         //发送通道
   uint16 SoftParity;        //奇偶交验模式,在1768中是8字节的，在stm32中是16字节的 
  // uint8 ChannelDriveMode;  // CH1 | CH0 ,
   uint8 ParityEnFlag;       //是否启动奇偶校验功能
   //uint8 curRxCh;           //当前通道
   uint32 RxSoftBaudCnter;    //伯特率计数器(用于接收)
   uint32 TxBaudCnter;      //用于发送
   uint8 bitnum;            //发送时一字节长度
   uint8 RxHardWare;        //接收硬件                         
   
}ComCtlType;




//示波器
#define WAVE_DETING   1
#define EXIT_WAVE     0
//常量
#define Fpclk 60000000 //SystemFrequency

//缓冲区长度定义
//#define CANBUSRXLEN  2048
//#define TXBUFLEN     280
//#define RXBUFLEN     280
//#define ECUTXBUFLEN  280
//#define ECURXBUFLEN  280
//#define TEMPBUFLEN   280
//#define KEEP_COM_BUF_LEN   100 //30
#define MAXLEN_4200  255
#define TXBUFLEN     280
//#define RXBUFLEN     4112
#define ECUTXBUFLEN  280
#define ECURXBUFLEN  280
#define TEMPBUFLEN   280
//#define MAX_SIZE     280//4112//256
#define KEEP_COM_BUF_LEN   100 //30
//#define maxlen 4112
#define TXQUEUEBUFLEN	411//6144//4096 //6144 //队列缓冲区大小6K

#define DMATXBUFLEN  250	//500，蓝牙最大250多个，所以设置成250
#define DMARXBUFLEN  250
#define DMATEMPLEN   250  //临时缓冲区大小 主要用于大量数据连续传输
//************获取电压控制
#define VCC_PASS_DRV 0//获取驱动电压 AD0.0
#define VCC_PASS_MUX1 1//获取line0上电压 AD0.1
#define VCC_PASS_MUX0 4//获取line1 上电压 AD0.4
#define VCC_PASS_CMP  2//获取比较电压 AD0.2

//定义通道号
#define CHANEL0 0 //设置mux0 mux1上can参数
#define CHANEL1 1 //设置 can1上参数
//C组继电器控制
#define C_CHANNEL_ALL_OFF  0   //打开所有
#define CAN_BUS_CHANNEL    1   //用CANBUS
#define KL_BUS_CHANNEL     2   //用KLBUS
#define ANALOG_CHANNEL_ON  3   //打开端口电压
#define ANALOG_CHANNEL_OFF 4   //关闭端口电压
#define DRIVE_8V           5   //驱动电压为设置为8V
#define DRIVE_12V          6   //驱动电压设置为12V
#define DRIVE_5V           7   //驱动电压设置为5V
#define NO_DRIVE           8   //驱动电压悬空
#define CLK_OUT_ENABLE     9   //允许时钟驱动输出
#define CLK_OUT_DISENABLE  10  //禁止CLK输出
#define CAN_TWO_BUS_CHANNEL 11 //第二路can接通
#define CAN_TWO_BUS_CHANNEL_DIS 12 //第二路can接通

//rs232 485控制
#define rs232 0
#define rs485 1
#define CLR_RS485_232_TO_OBD 2
//rs485 发送接收控制
#define RS485_TD 0
#define RS485_RX 1
//通讯线号
#define LINE0              0
#define LINE1              1
#define PWM_LINE           2   //这时会同时用到LINE0,LINE1
//通讯线上下拉设置
#define NO_LOAD            0   //悬空
#define UP_LOAD            1   //上拉
#define DOWN_LOAD          2   //下拉
#define PWM_LOAD           3   //上下拉
//通道设置
#define CHANNEL_OFF        0  //所有通道断开
//第二路can线设置
#define SECOND_CAN_SINGAL 0
#define SECOND_CAN_NORMAL 1
#define CLR_SECOND_CAN_TO_OBD 2
//设置输出端为强态和弱态 1:弱态,线上常状态;0:强态,线上有数据发出
#define SET_OUT_STATUS_0 0
#define SET_OUT_STATUS_1 1
//设置上拉，还是下拉
#define UP_PULL   1
#define DOWN_PULL 0
//命令类型掩模字
#define  SET_MCU_MASK              0x00
#define  SET_BUS_MASK              0x10
#define  INIT_ECU_MASK             0x20
#define  DATA_MASK                 0x30
#define  KEEP_COM_MASK             0x40
#define  DELAY_TX_MASK             0x50
#define  SPECIAL_MASK              0x60
#define  CAN_ID_FILT			   0x70
#define  FLASH_MASK				   0x90
#define  PROTOCOLTYPE			   0xD0
#define  KEEP_ALWAY			       0xC0 
#define  PDUTYPE			       0xE0 // MINI VCI 当中调式PDU 命令


//#define  LKT4200_MASK			  0x90
#define  SYS_FUN_MASK              0xF0
//协议类型掩模字
#define  PROTOCOL_MASK             0x0F
#define  CAN_BUS_MASK              0x00
#define  VPW_BUS_MASK              0x01
#define  PWM_BUS_MASK              0x02
#define  KWP2K_BUS_MASK            0x03
#define  AUDI_BUS_MASK             0x04
#define  BENZ_BUS_MASK             0x05
#define  FLASH_BUS_MASK            0x06
#define  CAN_TWO_BUS_MASK          0x07
#define  WAVE_BUS_MASK             0x0d
//CAN TWO ID设置
#define SET_CAN_TWO_ID             0X81 //第二个id的设置
//实际的控制字必须由命令类型和协议类型掩模字组合而成,
//如果在掩模字里没有定义，则不存在该项值
//1开头
#define  SET_CAN_BUS               SET_BUS_MASK|CAN_BUS_MASK    //CAN
#define  SET_PWM_BUS               SET_BUS_MASK|PWM_BUS_MASK    //PWM
#define  SET_VPW_BUS               SET_BUS_MASK|VPW_BUS_MASK    //VPWM
#define  SET_KWP2K_BUS             SET_BUS_MASK|KWP2K_BUS_MASK  //kwp2000
#define  SET_AUDI_BUS              SET_BUS_MASK|AUDI_BUS_MASK   //奥迪
#define  SET_BENZ_BUS              SET_BUS_MASK|BENZ_BUS_MASK   //奥迪
#define  SET_FLASH_BUS             SET_BUS_MASK|FLASH_BUS_MASK   //奥迪
#define  SET_CAN_TWO_BUS           SET_BUS_MASK|CAN_TWO_BUS_MASK    //CAN
#define  SET_WAVE_BUS              SET_BUS_MASK|WAVE_BUS_MASK    //波形

//2开头
#define  KWP2K_ECU_INIT            INIT_ECU_MASK|KWP2K_BUS_MASK  //初始化
#define  AUDI_ECU_INIT             INIT_ECU_MASK|AUDI_BUS_MASK
#define  BENZ_ECU_INIT             INIT_ECU_MASK|BENZ_BUS_MASK
//3开头
#define  CAN_DATA                  DATA_MASK|CAN_BUS_MASK    //CAN
#define  PWM_DATA                  DATA_MASK|PWM_BUS_MASK    //PWM
#define  VPW_DATA                  DATA_MASK|VPW_BUS_MASK    //VPWM
#define  KWP2K_DATA                DATA_MASK|KWP2K_BUS_MASK  //kwp2000
#define  AUDI_DATA                 DATA_MASK|AUDI_BUS_MASK   //奥迪
#define  BENZ_DATA                 DATA_MASK|BENZ_BUS_MASK   //奔驰
#define  FLASH_DATA                DATA_MASK|FLASH_BUS_MASK   //脉冲码
#define  CAN_DATA_TWO              DATA_MASK|CAN_TWO_BUS_MASK    //CAN
#define  WAVE_DATA                 DATA_MASK|WAVE_BUS_MASK   //示波器
//4开头
#define  CAN_KEEP_COM              KEEP_COM_MASK|CAN_BUS_MASK    //CAN
#define  PWM_KEEP_COM              KEEP_COM_MASK|PWM_BUS_MASK    //PWM
#define  VPW__KEEP_COM             KEEP_COM_MASK|VPW_BUS_MASK    //VPWM
#define  K2K_KEEP_COM              KEEP_COM_MASK|KWP2K_BUS_MASK  //kwp2000
#define  AUDI_KEEP_COM             KEEP_COM_MASK|AUDI_BUS_MASK   //奥迪

//F开头
#define   CHECKSUM_ERR             0xFE
#define   UNKNOW_TYPE              0xFD
#define   INIT_ERR                 0xFC

//状态字StatusByte
#define  NO_ANY_INIT               0x00
#define  PORT_INIT_OK              0x01  //bit0
#define  ECU_INIT_OK               0x02  //bit1
//
#define ECU_BUF_LEN  30

#define UART_WAIT_TIME   500   // 串口等待时间50*10ms 

//通道模式
#define NO_RX_CHANNEL         0
#define ONLY_CH0RX            1      //只有通道0收
#define ONLY_CH1RX            2      //只有通道1接收
#define CH0_CH1_RX            3      //通道0和通道1同时接收

#define NO_TX_CHANNEL         0
#define ONLY_CH0TX            1      //只有通道0收
#define ONLY_CH1TX            2      //只有通道1接收
#define CH0_CH1_TX            3      //通道0和通道1同时发送

//驱动模式
#define  STRONG_PULL_DOWN     0   //强下拉
#define  STRONG_PULL_UP       1   //强上拉
#define  PUSH_PULL_P          2   //推挽式，输入1强上拉，输入0强下拉，常态强上拉
#define  PUSH_PULL_N          3   //推挽式，输入1强下拉，输入0强上拉，常态强下拉（NISSAN即如此）
 
//
#define NORMAL_RX     0     //允许奇偶校验功能
#define DISEN_PARITY  1     //屏蔽奇偶校验功能
//J1860 标记
#define BYTE_FALSE       0
#define BYTE_OK          1
#define EOD_FLAG         2
#define EOF_FLAG         3
#define IFS_FLAG         4
#define BRK_FLAG         5
#define SOF_FLAG         6
#define NOISE_FLAG       7

//
#define DIAGNOSE      0
#define SIMULATION    1
#define INTERCEPTION  2
#define DIRECTREAD    3

//
#define  START_DETECT   0  
#define  DETECT_OK      1
#define  DETECT_FALSE   2

//通讯保持
#define AUTO_KEEP_COM_SWITCH_BIT        0x01  //BIT0  下位机自动通讯保持位
#define AUTO_KEEP_COM_ANSWER_BIT        0x02  //BIT1  是否等待回应
#define AUTO_KEEP_COM_RXTX_DELAY_BIT    0X04  //BIT2  读到ECU数据/或发送数据后，是否延时自动保持
//bit0定义
#define AUTO_KEEP_OFF                   0x00  //关闭自动保持
#define AUTO_KEEP_ON                    0x01  //启动自动保持
//bit1定义
#define KEEP_COM_ANS_OFF                0x00  //不屏蔽自动通讯时ECU的回应
#define KEEP_COM_ANS_ON                 0x02  //屏蔽自动通讯保持时ECU的回应
//bit2定义
#define KEEP_COM_R_DELAY_ON            0x00  //读后延时
#define KEEP_COM_R_DELAY_OFF           0x04  //读后不延时，定时插入发送,这种情况一般是只发送不接收，如VPW,某些CAN 


//PORT_NUM>=16
#define INIT_PORT_FUN // qu (PORT_NUM,PORT_FUN)   LPC_PINCON->PINSEL1=(LPC_PINCON->PINSEL1& ~(3 << ((PORT_NUM-16) * 2)))|(PORT_FUN << ((PORT_NUM-16) * 2))
//PORT_NUM<=15
#define INIT_PORT_FUNC// qu (PORT_NUM,PORT_FUN)  LPC_PINCON->PINSEL0=(LPC_PINCON->PINSEL0& ~(3 <<( PORT_NUM * 2)))|(PORT_FUN <<( PORT_NUM * 2)) 

//波特率校准系数
#define BAUD_ADJUST_CONST    22
//LineLevelFlag
#define  LINE_UNVALID     0
#define  LINE_VALID       1

#define  WAIT_EDGE    0
#define  UP_EDGE      1
#define  DOWN_EDGE    2
#define LOW    0 
#define HIGH   1
//
#define FIRST_BYTE      0
#define NOT_FIRST_BYTE  1

//IAP
#define IAP_ENTER_ADR   0x1FFF1FF1  // IAP入口地址定义

/* 定义IAP命令字 */
                                    //   命令           参数
#define     IAP_SELECTOR        50  // 选择扇区     【起始扇区号、结束扇区号】
#define     IAP_RAMTOFLASH      51  // 拷贝数据     【FLASH目标地址、RAM源地址、写入字节数、系统时钟频率】
#define     IAP_ERASESECTOR     52  // 擦除扇区     【起始扇区号、结束扇区号、系统时钟频率】
#define     IAP_BLANKCHK        53  // 查空扇区     【起始扇区号、结束扇区号】
#define     IAP_READPARTID      54  // 读器件ID     【无】
#define     IAP_BOOTCODEID      55  // 读Boot版本号 【无】
#define     IAP_COMPARE         56  // 比较命令     【Flash起始地址、RAM起始地址、需要比较的字节数】
#define     IAP_ReadSN          58  // 比较命令     【Flash起始地址、RAM起始地址、需要比较的字节数】

/* 定义IAP返回状态字 */
#define     CMD_SUCCESS          0
#define     INVALID_COMMAND      1
#define     SRC_ADDR_ERROR       2 
#define     DST_ADDR_ERROR       3
#define     SRC_ADDR_NOT_MAPPED  4
#define     DST_ADDR_NOT_MAPPED  5
#define     COUNT_ERROR          6
#define     INVALID_SECTOR       7
#define     SECTOR_NOT_BLANK     8
#define     SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION 9
#define     COMPARE_ERROR        10
//#define     BUSY                 11

//AnswerFlag
#define ANSWER_OFF   0
#define ANSWER_ON    1

//Uart0TxSwitch
#define SEND_BLOCK  1 //停止往上位机发送数据
#define SEND_RUN    0 //允许往上位机发送数据
//ecu通讯类型,用在ecu发送数据时，中断里的标志
#define ecu_send 0
#define ecu_recv 1
#define ecu_stop 2


#include "PassThru.h"

extern uint16 usart_len;																	//2
extern uint32 ECU_Byte;			//要发的数据									//4
extern uint8  ECU_Byte_num;  //要发的数据的bit位					//1
extern uint8  ecu_staus;			//发送数据成功							//1

extern uint8  RxFrameFlag;

extern uint16 USART1_ReceiveNum;													//2
extern uint8 USART1_TxFlag;																//1
extern uint8 USART1_RxFlag;																//1
extern uint8 USART1_TxBuff[TXQUEUEBUFLEN];//[DMATXBUFLEN];									//250
extern uint8 USART1_RxBuff[TXQUEUEBUFLEN];//[DMARXBUFLEN];									//250
//extern uint8 USART1_RxBuff_temp[TXQUEUEBUFLEN];//USART1_RxBuff_temp[DMARXBUFLEN]; 4110  4622
extern uint32 timeflag, timeidle;																//8

extern uint16 ErrCode;//错误码																	//2
extern PASSTHRU_CONFIG PassThruConfig;													//200
extern PASSTHRU_DRIVER PassThruDriver;												//

extern volatile uint16 KeepComTimeCounter[2];      //ECU通讯保持数据
//extern volatile uint16 UartTimeLimited;            //串口通讯时间，默认为500ms


extern volatile uint32 counter_1ms;													//4
extern volatile uint32 counter_1ms_;												//4

extern ComCtlType ComCtl;  //通讯控制
extern UARTMODE UartCtlReg;
extern uint8 USART1_RxFlag;																	//1
extern uint16 KeepComTime[2];																//2
extern uint8 KeepComSwitch;																	//1
extern uint8 KeepComSwitch1;																//1
extern uint8 KeepComSwitch2;//接收到7F78否定回答关闭下位机自动通讯保持//1

extern uint8  CanbusInsert[KEEP_COM_BUF_LEN];								//100
extern uint8 EcuComRxBuf_temp[ECURXBUFLEN];									//250
//extern uint8 *EcuComRxBuf;
extern uint8 EcuComTxBuf[ECUTXBUFLEN];											//250
extern uint8 TempProcBuf[TXQUEUEBUFLEN];												//4110
//;TEMPBUFLEN
extern uint8 KeepComBuf[KEEP_COM_BUF_LEN];									//100
extern uint8 KeepComBuf1[KEEP_COM_BUF_LEN];									//100
extern uint8 ProtocolType;   //协议类型
extern uint8 StatusByte;     //状态记忆字节
extern uint8 ValidFlag;
extern uint8 AnswerFlag;
extern uint8 LineInitFlag;
extern uint8 LineLevelFlag;
extern uint32 _1msCounter;
extern uint32 _10msCounter;
extern uint32 endmsCounter;
extern uint32 ecumsCounter;
extern uint8 K2K_Simulate_firstRX_flag;    //模拟模式第一次接收标志
extern uint8 Stop_bit_5bps_flag;
extern uint8 DelayTimeForHoldCom; //通讯保持后的延时
extern uint8 UartTxQueueBuf_ext[TXQUEUEBUFLEN];													//4k
extern uint8 *UartTxQueueBuf;
extern uint8 RxChecksum;
extern uint8 TxFrameFlag;
extern uint16 ByteCounter;
extern uint16 revPt;
extern uint8 RxFrameFlag;
extern uint8 UartTxSwitch;
extern uint8 CAN_ID_FILT_FLAG;        //CAN ID 软件过滤标志位
extern uint8 ChannelBuf[8];//低四个字节控制上下拉及发送接收模式，高四个字节控制接通obd的引脚号
//23 5baud前在空闲时间发波形
extern uint8 kwp_5bpsdata_delay_sent_flag ;
extern uint8 kl_set[4];
extern uint32 kl_set_time;
extern uint8 adjust_width[4];//0:line0上拉时或line0 line1同时上拉时的调整参数;1:line1上拉;2：line0下拉或line0 line1同时下拉;3:line1下拉
extern uint8 adjust_width_select;
extern uint8 ExitWaveFlag;
extern uint8 WaveNum;
extern uint8 FrameFormat[2];   //帧格式SFF 或 EFF
extern uint8 Can_Send_Mode[2];
extern uint8 UartBit9;
extern uint8 can_test_status;
extern uint16 vcc_drv;



#if 0
//
/********************************/
/*Application Program Configurations*/
/*     应用程序配置             */
/********************************/
//This segment could be modified as needed.
//以下根据需要改动
#include    <string.h> //memcpy



#include    "PortAssign.h"              //This line may not be deleted 这一句不能删除
//所有串口头函数
//#include "mcu_uart.h"				//和ecu通讯的串口
//#include "uart.h"					//和上位机通讯的串口
//#include "4200_uart.h"				//和加密芯片通讯的串口
//#include "pwm_output.h"				//PWM输出定义
//#include "stm32f10x_conf.h"  
//#include "time.h"
//#include "adc.h"
//#include "hardware.h"
#include "mmain.h"
#include "PassThru_KWP.h"
#include "PassThru_ISO.h"
#include "PassThru_PLUSE.h"
#include "PassThru_PWM.h"
#include "PassThru_VPW.h"
#include "PassThru_BENZ.h"
#include "PassThru_WAVE.h"
#include "PassThru_CAN.h"
#include "selftest.h"
//#include    "target_ext.h"
//#include    "uart1.h"
//#include    "uart0.h"
//#include    "uart3.h"
//#include    "uart2.h"
//#include   "hardware.h"
#include "NewSoftUart.h"
#include "PassThru.h"

//#include "code.h"

//#include "usart1.h"

#define CAN_IRQn 1 // qu USB_LP_CAN1_RX0_IRQn
//#include   "LPC1700CAN.h"




typedef unsigned char ElemType;

// int ByteToBit(ElemType ch,ElemType bit[8]);
// int BitToByte(ElemType bit[8],ElemType *ch);
// int Char8ToBit64(ElemType ch[8],ElemType bit[64]);
// int Bit64ToChar8(ElemType bit[64],ElemType ch[8]);
// int DES_MakeSubKeys(ElemType key[64],ElemType subKeys[16][48]);
// int DES_PC1_Transform(ElemType key[64], ElemType tempbts[56]);
// int DES_PC2_Transform(ElemType key[56], ElemType tempbts[48]);
// int DES_ROL(ElemType data[56], int time);
// int DES_IP_Transform(ElemType data[64]);
// int DES_IP_1_Transform(ElemType data[64]);
// int DES_E_Transform(ElemType data[48]);
// int DES_P_Transform(ElemType data[32]);
// int DES_SBOX(ElemType data[48]);
// int DES_XOR(ElemType R[48], ElemType L[48],int count);
// int DES_Swap(ElemType left[32],ElemType right[32]);
// int DES_EncryptBlock(ElemType plainBlock[8], ElemType subKeys[16][48], ElemType cipherBlock[8]);
// int DES_DecryptBlock(ElemType cipherBlock[8], ElemType subKeys[16][48], ElemType plainBlock[8]);
// int DES_Encrypt(char *plainFile, char *keyStr,char *cipherFile);
// int DES_Decrypt(char *cipherFile, char *keyStr,char *plainFile);
// int XOR_DecryptBlock(ElemType cipherBlock[8], ElemType Encrypt_Key[256], ElemType Encrypt_Pos);

// extern int DES_Encrypt_UpdateKey(ElemType *rxbuf, uint16 rlen);
// extern int DES_Decrypt_UpdateKey(ElemType *rxbuf, uint16 rlen);
#endif
#endif
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
