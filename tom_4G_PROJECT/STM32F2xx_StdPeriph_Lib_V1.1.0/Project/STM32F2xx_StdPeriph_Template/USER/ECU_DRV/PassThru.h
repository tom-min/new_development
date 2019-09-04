#ifndef __PASS_THRU_H__
#define __PASS_THRU_H__



typedef unsigned char  uint8;                   /* defined for unsigned 8-bits integer variable 	无符号8位整型变量  */
typedef signed   char  int8;                    /* defined for signed 8-bits integer variable		有符号8位整型变量  */
typedef unsigned short uint16;                  /* defined for unsigned 16-bits integer variable 	无符号16位整型变量 */
typedef signed   short int16;                   /* defined for signed 16-bits integer variable 		有符号16位整型变量 */
typedef unsigned int   uint32;                  /* defined for unsigned 32-bits integer variable 	无符号32位整型变量 */
typedef signed   int   int32;                   /* defined for signed 32-bits integer variable 		有符号32位整型变量 */
typedef float          fp32;                    /* single precision floating point variable (32bits) 单精度浮点数（32位长度） */
typedef double         fp64;                    /* double precision floating point variable (64bits) 双精度浮点数（64位长度） */


/*Connect Flag Values*/
#define CAN_29BIT_ID		 0x00000100
#define ISO9141_NO_CHECKSUM	 0x00000200
#define CAN_ID_BOTH			 0x00000800
#define ISO9141_K_LINE_ONLY  0x00001000

/*Protocol ID Values*/
#define PROTOCOL_J1850VPW			0xA1
#define PROTOCOL_J1850PWM			0xA2
#define PROTOCOL_ISO9141			0xA3
#define PROTOCOL_ISO14230			0xA4
#define PROTOCOL_CAN				0xA5
#define PROTOCOL_ISO15765			0xA6
#define PROTOCOL_SCI_A_ENGINE		0xA7
#define PROTOCOL_SCI_A_TRANS		0xA8
#define PROTOCOL_SCI_B_ENGINE		0xA9
#define PROTOCOL_SCI_B_TRANS		0xAA

/*Filter Type Values*/
#define PASS_FILTER			0x00000001
#define BLOCK_FILTER		0x00000002
#define FLOW_CONTROL_FILTER	0x00000003
//Reserved 0x00000004-0x00007FFF
//Reserved for SAE J2534-2 0x00008000-0x0000FFFF
//Tool manufacturer specific 0x00010000-0xFFFFFFFF

/*Voltage Values*/
#define P_VOLTAGE_MIM		0x00001388
#define P_VOLTAGE_MAX		0x00004E20
#define SHORT_TO_GROUND		0xFFFFFFFE
#define VOLTAGE_OFF			0xFFFFFFFF

/*Ioctl ID Values*/
#define GET_CONFIG					0x01
#define SET_CONFIG					0x02
#define READ_VBATT					0x03
#define FIVE_BAUD_INIT				0x04
#define KWP_FAST_INIT   			0x05
#define CLEAR_TX_BUFFER				0x07
#define CLEAR_RX_BUFFER				0x08
#define CLEAR_PERIODIC_MSGS			0x09
#define CLEAR_MSG_FILTERS			0x0A
#define CLEAR_FUNCT_MSG_LOOKUP_TABLE		0x0B
#define ADD_TO_FUNCT_MSG_LOOKUP_TABLE		0x0C
#define DELETE_FROM_FUNCT_MSG_LOOKUP_TABLE	0x0D
#define READ_PROG_VOLTAGE					0x0E
//Reserved for SAE 0x0F – 0x7FFF
//Reserved for SAE J2534-2 0x8000 – 0xFFFF
//Tool manufacturer specific 0x10000 – 0FFFFFFF

/*Ioctl GET_CONFIG Parameter*/
#define PSP_DATA_RATE					0x01
#define PSP_LOOPBACK					0x03
#define PSP_NODE_ADDRESS				0x04
#define PSP_NETWORK_LINE				0x05
#define PSP_P1_MIN						0x06
#define PSP_P1_MAX						0x07
#define PSP_P2_MIN						0x08
#define PSP_P2_MAX						0x09
#define PSP_P3_MIN						0x0A
#define PSP_P3_MAX						0x0B
#define PSP_P4_MIN						0x0C
#define PSP_P4_MAX						0x0D
#define PSP_W0							0x19
#define PSP_W1							0x0E
#define PSP_W2							0x0F
#define PSP_W3							0x10
#define PSP_W4							0x11
#define PSP_W5							0x12
#define PSP_TIDLE						0x13
#define PSP_TINIL						0x14
#define PSP_TWUP						0x15
#define PSP_PARITY						0x16
#define PSP_BIT_SAMPLE_POINT			0x17
#define PSP_SYNC_JUMP_WIDTH				0x18
#define PSP_T1_MAX						0x1A
#define PSP_T2_MAX						0x1B
#define PSP_T3_MAX						0x24
#define PSP_T4_MAX						0x1C
#define PSP_T5_MAX						0x1D
#define PSP_ISO15765_BS					0x1E
#define PSP_ISO15765_STMIN				0x1F
#define PSP_BS_TX						0x22
#define PSP_STMIN_TX					0x23
#define PSP_DATA_BITS					0x20
#define PSP_FIVE_BAUD_MOD				0x21
#define PSP_ISO15765_WFT_MAX			0x25

#define SERIAL_PORT     1
#define SERIAL_BAUDRATE 115200

#define BAUDRATE_MIN    5         //设置最小波特率 5bps
#define BAUDRATE_MAX    500000    //设置最大波特率 500kbps

#define CAN_MODE_BIT     0x80
#define RTR_MODE         0x40
//工作模式
#define SFF_MODE     0
#define EFF_MODE     1
#define EFF_FORMAT   0x01
#define SFF_FORMAT   0x00

//滤波模式
#define SINGLE_FILTER    0
#define DOUBLE_FILTER    1

//错误代码
#define CAN_NO_ERR      0
#define CAN_TIME_OUT    1
#define CAN_LEN_SHORT   2
#define NO_MATCH_CMD    3

#define MAX_CANID_COUNT 256 //  设置　can id过滤

#define CAN_15765_SF		0x00//单帧   Single Frame
#define CAN_15765_FF		0x10//首帧   First Frame
#define CAN_15765_CF		0x20//连续帧 Consecutive Frame
#define CAN_15765_FC		0x30//流控制 Flow Control

#define EFF_FLAG 0x80000000  //用以标志这是29bitID

#define         BasicCAN
typedef int BOOL; 	  

/************************************************************************
*SJA1000与微处理器的接口是以外部存储器的方式，所以以下的基址定义，用户 *
*应根据自己的实际电路来进行调整                                        *
************************************************************************/
#define         SJA_BaseAdr       0x00     //定义sja1000的片选基址

//计算SJA1000在电路中的实际地址:基址+内部寄存器地址

/************************************************************************
* 以下的定义为SJA1000的内部寄存器的地址，不可修改                      *
*     各寄存器的具体功能清参阅sja1000的数据手册                        *
************************************************************************/
#define         REG_CONTROL_0     SJA_BaseAdr+0x00       //内部控制寄存器
#define         REG_COMMAND       SJA_BaseAdr+0x01       //命令寄存器
#define         REG_STATUS        SJA_BaseAdr+0x02       //状态寄存器
#define         REG_INTERRUPT     SJA_BaseAdr+0x03       //中断寄存器
#define         REG_ACR           SJA_BaseAdr+0x04       //验收代码寄存器
#define         REG_AMR           SJA_BaseAdr+0x05       //验收屏蔽寄存器
#define         REG_BTR0          SJA_BaseAdr+0x06       //总线定时寄存器0 
#define         REG_BTR1          SJA_BaseAdr+0x07       //总线定时寄存器1 
#define         REG_OCR           SJA_BaseAdr+0x08       //输出控制寄存器
#define         REG_TEST          SJA_BaseAdr+0x09       //测试寄存器

//以下为发送缓冲区寄存器定义
#define         REG_TxBuffer1     SJA_BaseAdr+0x0A         //发送缓冲区1
#define         REG_TxBuffer2     SJA_BaseAdr+0x0B         //发送缓冲区2
#define         REG_TxBuffer3     SJA_BaseAdr+0x0C         //发送缓冲区3
#define         REG_TxBuffer4     SJA_BaseAdr+0x0D         //发送缓冲区4
#define         REG_TxBuffer5     SJA_BaseAdr+0x0E         //发送缓冲区5
#define         REG_TxBuffer6     SJA_BaseAdr+0x0F         //发送缓冲区6
#define         REG_TxBuffer7     SJA_BaseAdr+0x10         //发送缓冲区7
#define         REG_TxBuffer8     SJA_BaseAdr+0x11         //发送缓冲区8
#define         REG_TxBuffer9     SJA_BaseAdr+0x12         //发送缓冲区9
#define         REG_TxBuffer10    SJA_BaseAdr+0x13         //发送缓冲区10

//以下为接收缓冲区寄存器定义
#define         REG_RxBuffer1     SJA_BaseAdr+0x14         //接收缓冲区1
#define         REG_RxBuffer2     SJA_BaseAdr+0x15         //接收缓冲区2
#define         REG_RxBuffer3     SJA_BaseAdr+0x16         //接收缓冲区3
#define         REG_RxBuffer4     SJA_BaseAdr+0x17         //接收缓冲区4
#define         REG_RxBuffer5     SJA_BaseAdr+0x18         //接收缓冲区5
#define         REG_RxBuffer6     SJA_BaseAdr+0x19         //接收缓冲区6
#define         REG_RxBuffer7     SJA_BaseAdr+0x1A         //接收缓冲区7
#define         REG_RxBuffer8     SJA_BaseAdr+0x1B         //接收缓冲区8
#define         REG_RxBuffer9     SJA_BaseAdr+0x1C         //接收缓冲区9
#define         REG_RxBuffer10    SJA_BaseAdr+0x1D         //接收缓冲区10

/************************************************************************
*内部地址 01EH 备用                                                    *
************************************************************************/

/************************************************************************
*时钟分频寄存器(内部地址01FH)                                          *
************************************************************************/
#define         REG_CDR           SJA_BaseAdr+0x1F          //时钟分频寄存器

/************************************************************************
*定义命令字                                                            *
************************************************************************/
#define         TR_CMD           0x01          //发送请求命令
//#define         AT_CMD           0x02          //夭折发送命令
#define         RRB_CMD          0x04          //释放接收缓冲区
#define         COS_CMD          0x08          //清除超载状态
#define         GTS_CMD          0x10          //进入睡眠状态命令  

/************************************************************************
*定义12M的晶体下，CAN总线波特率                                        *
************************************************************************/
#define         ByteRate_20k      0x00          //波特率20kbps
#define         ByteRate_40k      0x01          //波特率40kbps
#define         ByteRate_50k      0x02          //波特率50kbps
#define         ByteRate_80k      0x03          //波特率80kbps
#define         ByteRate_100k     0x04          //波特率100kbps
#define         ByteRate_125k     0x05          //波特率125kbps
#define         ByteRate_200k     0x06          //波特率200kbps
#define         ByteRate_250k     0x07          //波特率250kbps
#define         ByteRate_400k     0x08          //波特率400kbps
#define         ByteRate_500k     0x09          //波特率500kbps
#define         ByteRate_666k     0x0a          //波特率666kbps
#define         ByteRate_800k     0x0b          //波特率800kbps
#define         ByteRate_1000k    0x0c          //波特率1000kbps

/*-------------------------------------------------------------------------------------------
*  以下为BASICCAN SJA1000的错误字                                                          *
-------------------------------------------------------------------------------------------*/
#define     CAN_INTERFACE_0K    0                                                        /*SJA1000接口正常                       */
#define     CAN_BUS_0K          0                                                       /*CAN总线工作正常                       */
#define     CAN_INTERFACE_ERR   0XFF                                                    /*SJA1000接口错误?                     */
#define     CAN_ENTRESET_ERR    0XFE                                                    /*SJA1000不能进入复位模式   */
#define     CAN_QUITRESET_ERR   0XFD                                                    /*SJA1000不能退出复位模式       */
#define     CAN_INITOBJECT_ERR  0XFC                                                    /*初始化报文验收滤波器错    */
#define     CAN_INITBTR_ERR     0XFB                                                    /*初始化总线定时器器错          */
#define     CAN_INITOUTCTL_ERR  0XFA                                                    /*初始化输出控制错误            */
#define     CAN_INTCLKDIV_ERR   0XF9                                                    /*初始化时钟分频控制器错        */
#define     CAN_BUS_ERR         0XF8                                                    /*SJA1000发送数据错                     */

#define     NO_BaudRate_SIZE    0xe7

#define PROTOCOL_FRAME_NORMAL    0x00//正常模拟 按照时间接收数据
#define PROTOCOL_FRAME_KWP2000   0x01//标准KWP协议
#define PROTOCOL_FRAME_NISSAN    0x02//日产老协议
#define PROTOCOL_FRAME_MIESUB    0x03//三菱老协议
#define PROTOCOL_FRAME_LENGTH    0x04//带长度控制的协议
#define PROTOCOL_FRAME_VOLVO     0x05//SAE J1708 for volvo
#define PROTOCOL_FRAME_CATER     0x06//卡特
#define PROTOCOL_FRAME_LENGTH2   0x07//带长度控制的协议 解决五十铃安全气囊接收第一帧数据的问题
#define PROTOCOL_FRAME_J1708MID  0x08//标准J1708协议接收，带MID过滤功能
#define PROTOCOL_FRAME_MAZDAKWP  0x09//福特马自达KWP协议44 58 F5 10 A1
#define PROTOCOL_FRAME_LENGTH3   0x0a//标准OBDII J1979协议 每帧最多11字节 3字节头 8字节数据

#define FmtLenBit   0x3F
#define FIVE_BAUD_MOD_NORMAL   0//正常情况
#define FIVE_BAUD_MOD_KEYBYTE  1//发送KB2取反
#define FIVE_BAUD_MOD_ADDRESS  2//接收地址取反

#define MAN_PROTOCOL_MASK  0x10   //常雁龙，函数和数据库全部定义成0x10

#define MAXLEN_OBDIIKWP    11   //标准OBDII J1979协议 每帧最多11字节 3字节头 8字节数据
//typedef struct
//{
//    uint32 ProtocolID;
//    uint32 RxStatus;
//    uint32 TxFlags;
//    uint32 Timestamp;
//    uint32 DataSize;
//    uint32 ExtraDataIndex;
//    uint32 Data[4128];
//} PASSTHRU_MSG;

typedef struct
{
    uint8 BusType;
    uint8 (*ParseFunc)(uint8 *);
    uint8 (*PortInitFunc)(void);
} PASSTHRU_INIT;

typedef struct
{
    uint8 (*InitEcuFunc)(uint8 *);       //初始化ECU
    uint8 (*TxDataToEcuFunc)(uint8 *);   //发送数据到总线 即时发送
    uint8 (*TxDataDelayFunc)(uint8 *);   //发送数据到总线 延迟发送
     void (*TxFlashDataToEcuFunc)(uint8 *);
    uint8 (*ReadBusFunc)(void);          //读总线数据
    uint8 (*KeepComFunc)(void);          //备用功能
    uint8 (*KeepAlwayComFunc)(uint8 *);          //备用功能
    uint8 (*FuncBak0)(void);             //备用功能
    uint8 (*RxByteDrv)(uint32, uint8 * );
    void  (*TxByteDrv)(uint8);
//     uint8 (*InitEcuFunc_TWO)(uint8 *);       //初始化ECU
//     uint8 (*TxDataToEcuFunc_TWO)(uint8 *);   //发送数据到总线 即时发送
//     uint8 (*ReadBusFunc_TWO)(void);          //读总线数据
// 		void (*TxFlashDataToEcuFunc_TWO)(uint8 *);
} PASSTHRU_DRIVER;

//five_baud_mod=0-Initialization as defined in ISO 9141-2 and ISO 14230-4
//five_baud_mod=1-ISO 9141 initialization followed by interface sending inverted Key Byte 2
//five_baud_mod=2-ISO 9141 initialization followed by ECU sending inverted address
//five_baud_mod=3-Initialization as defined in ISO 9141TxDataDelayFunc
typedef struct
{
    uint32 BaudRate;      //通讯波特率 5-500000 BaudRate
   // uint32 Loopback;      //0:OFF 1:ON
   // uint32 NodeAddress;   //J1850PWM总线地址
   // uint32 NetworkLine;   //J1850PWM总线设置 0 (BUS_NORMAL) 1 (BUS_PLUS) 2 (BUS_MINUS)
   
    uint16 P1Min;         //
    uint16 P1Max;         //0x1-0xFFFF (0.5 ms per bit) 默认40 (20 ms)
    uint16 P2Min;         //
    uint16 P2Max;         //
    uint16 P3Min;         //0x1-0xFFFF (0.5 ms per bit) 默认110 (55 ms)
    uint16 P3Max;         //
    uint16 P4Min;         //0x1-0xFFFF (0.5 ms per bit) 默认10 (5 ms)
    uint16 P4Max;         //
    
    uint16 W0;            //0x0-0xFFFF (1 ms per bit) 默认300 TOOL发送地址码之前总线的最小空闲时间 ISO 9141
    uint16 W1;            //0x0-0xFFFF (1 ms per bit) 默认300 ECU收到地址码之后回复55的最大等待时间 ISO 9141 or ISO 14230
    uint16 W2;            //0x0-0xFFFF (1 ms per bit) 默认20 ECU回复kb1的最大等待时间 ISO 9141 or ISO 14230
    uint16 W3;            //0x0-0xFFFF (1 ms per bit) 默认20 kb1和kb2之间的最大等待时间 ISO 9141 or ISO 14230
    uint16 W4;            //0x0-0xFFFF (1 ms per bit) 默认50 TOOL发送kb2取反的最小等待时间 ISO 9141 or ISO 14230
    uint16 W5;            //0x0-0xFFFF (1 ms per bit) 默认300 TOOL发送地址码之前总线的最小空闲时间 ISO 14230
    uint16 Tidle;         //0x0-0xFFFF (1 ms per bit) 默认300 快速初始化开始之前总线的最小空闲时间 ISO 9141 or ISO 14230
    uint16 Tinil;         //0x0-0xFFFF (1 ms per bit) 默认25 快速初始化开始之前低脉冲时间 ISO 9141 or ISO 14230
    uint16 TWUP;          //0x0-0xFFFF (1 ms per bit) 默认50 快速初始化唤醒脉冲持续时间 ISO 9141 or ISO 14230
    uint16 Parity;        //0 (NO_PARITY) 1 (ODD_PARITY) 2 (EVEN_PARITY)
//     uint16 BitSamplePoint;//0-100 (1% per bit) CAN协议采样率
//     uint16 SyncJumpWidth; //0-100 (1% per bit) CAN协议同步宽度
    uint16 T1Max;         //0x0-0xFFFF (1 ms per bit) 默认20 SCI_A_ENGINE, SCI_A_TRANS, SCI_B_ENGINE or SCI_B_TRANS
    uint16 T2Max;         //0x0-0xFFFF (1 ms per bit) 默认100 SCI_A_ENGINE, SCI_A_TRANS, SCI_B_ENGINE or SCI_B_TRANS
    uint16 T3Max;         //0x0-0xFFFF (1 ms per bit) 默认50 SCI_A_ENGINE, SCI_A_TRANS, SCI_B_ENGINE or SCI_B_TRANS
    uint16 T4Max;         //0x0-0xFFFF (1 ms per bit) 默认20 SCI_A_ENGINE, SCI_A_TRANS, SCI_B_ENGINE or SCI_B_TRANS
    uint16 T5Max;         //0x0-0xFFFF (1 ms per bit) 默认100 SCI_A_ENGINE, SCI_A_TRANS, SCI_B_ENGINE or SCI_B_TRANS
    uint16 ISO15765BS[2];    //中间层转换多帧数据块大小 暂时不确定
    uint16 ISO15765STmin[2]; //中间层转换多帧分割时间 暂时不确定
   uint16 BSTx;          //TOOL发送多帧数据块大小
   uint16 STMinTx;       //TOOL发送多帧分割时间
   uint16 DataBits;      //0 (8 data bits) 1 (7 data bits)
   uint16 FiveBaudMod;   //有些明白 0:ISO9141-2 and ISO14230-4 1:ISO9141kb2取反 2:ISO9141接收地址取反 3:ISO9141
//   uint16 ISO15765WftMax;//不明白
    uint8  Listen;        //00诊断 01模拟 CANBUS:7F/80/81/82/83
    uint8  Format;        //
    uint8  Pin1;          //检测线1通道号
    uint8  Pin2;          //检测线2通道号
    uint8  LogicV1;       //检测线1配置
    uint8  LogicV2;       //检测线2配置
    uint8  CompV;         //比较电压
    uint8  KeyByteNum;    //
    uint8  KeyByte1;      //
    uint8  KeyByte2;      //
    uint32 Eclk;          //
    uint16 GndCom;        //
    uint8  PulseCom;      //初始化ECU 时特定波形的输出端口 01：PIN1 10：PIN2
    uint8  Counter;       //ISO9141协议计数器
    uint8  Header;        //包头
    uint8  HeaderAR;      //包头屏蔽字
    uint8  Src;           //源地址
    uint8  SrcAR;         //源地址屏蔽字
    uint8  Tgt;           //源地址
    uint8  TgtAR;         //源地址屏蔽字
    uint8  HeaderID;
    uint8  SourcesAddr;
    uint8  TargetAddr;
    uint32 _BaudRate;     //模拟波特率 ECU 发送55 XX XX 的波特率
   uint32 CurrBaud;      //当前波特率
   uint8  WaitTime;      //大众ISO协议:等待ECU回应取反时间单位ms 模拟发送55时间
   uint8  ByteTime;      //大众ISO协议:仪器回应字节间取反时间单位ms 模拟发送Kb1时间
   uint8  _WaitTime;     //模拟发送55 XX XX 前的等待时间
   uint8  _ByteTime;     //模拟发送55 XX XX 的字节间隔时间
    uint8  SendBuf[16];   //00 25 80 b2 02 00 04 34 55 79 80 00 00 00 00
    uint16 Keep[2];          //下位机自动通讯保持时间
    uint8  KeepComMode;   //通讯保持模式
    uint8  PulseBuf[200];  //kwp协议初始化之前发送脉冲 最多17个脉冲
    uint8  ExtPort;       //BENZ 38PIN接头 扩展引脚
    uint32 Protocol;      //kwp协议区分通讯模式
    uint32 endmsCounter;  //发送或者接收最后一个字节时间
    uint32 CanTD;  	      //canbus 04 ECU接受ID
    uint32 CanLastTD[2];			//上次发送的CANID,用于82 83正常情况下流控制
    uint8  CanExtID;   //83正常情况下流控制的发送小id
    uint16 CanIdauto;     //canbus 0b ID_Auto
    uint8  CanAnsT;       //canbus 0d 自动等待下位机控制
    uint8  CanAnsR;  //canbus 0e 自动回应确认帧
    uint8  CanMidTime[2];		//canbus 0f 多帧发送时字节间间隔
    uint16 CanFill;       //canbus 12 填充数字 默认填充0x00
    uint8  CAN_RES;       //canbus 10 是否加电阻   0 加 1 不加   ADD BY YWP
    uint8 IFR_Time;       //PWM协议 ECU帧间应答时间(ms) XK 20140609
    uint8 Send_Pass;			//下位机通讯过程中，发送通道
    uint8 Recv_Pass;			//下位机通讯过程中，接收通道
    uint32 CanAR1; 	      //canbus 05 ECU发送ID
    uint32 CanMR1; 	      //canbus 06 屏蔽寄存器
    uint32 CanAR2; 	      //canbus 07 ECU发送ID
    uint32 CanMR2; 	      //canbus 08 屏蔽寄存器
    uint8 	Can_ext_addr;	//一帧can的有郊数据，为7时表示无扩展地址的，为6时表示有扩展地址
    uint8 TxAddress;
    uint8 RxAddress;
    uint8 TxMode;//数据发送模式
    uint8 RxMode;//数据发送模式
		uint8 CtrlMode;       //00 诊断模式 01 模拟模式 02 单帧模式 03 采样模式
		uint16 CanRecvTimeOut;//canbus 17 多帧通讯 接收控制帧和连续帧等待超时
} PASSTHRU_CONFIG;


extern uint32 GetAddresses32(uint8 *buffer);
//extern void tx_PINSEL_channel(void);
 // extern void tx_c_channel(void);
extern void tx_spi_one_byte(uint8 dat,uint8 num);
extern void SetVcc_Drv(uint8 vcc_drv);
extern void SetExtCh(uint8 ch);
extern void SetEndmsCounter(void);
extern void DelayTxByte(uint8 DlyInTxByte);
extern uint8 GetLineMonitor(void);
extern void SendDelayCounter(void);
extern void SimulateEcu5bps(void);
extern uint8 PassThru_SetKeepComSwitch2(uint8 *RXdata);
extern void SetEcuLineLoadMode(uint8 LineNum,uint8 LoadMode);
extern void Reset_Fun(void);
extern uint8 ALL_KEEP_COM(void);
extern void tx164dat(uint8 dat);
extern uint8 rx164dat(void);
//PASSTHRU_CONFIG PassThruConfig;
//PASSTHRU_DRIVER PassThruDriver;
/*

extern void  SoftUartTxByte_(uint8 dat);
extern void  SetEndmsCounter(void);
extern void  SendDelayCounter(void);
extern void  SimulateEcu5bps(void);
extern void  DelayTxByte(uint8 DlyInTxByte);
extern uint8 GetLineMonitor(void);
extern uint8 GetDetectStatus(void);
extern uint8 GetLineLevelStatus(void);
extern uint8 PassThru_Default_SetConfig(void);
extern uint8 PassThru_SetKeepComSwitch2(uint8 *RXdata);
extern uint8 RECV_5BAUD(uint8 type);

extern uint8 PassThru_CAN_InitPort(uint8 CHANEL);
extern uint8 PassThru_CAN_SetConfig(uint8 *Pack,uint8 CHANEL);

extern uint8 PassThru_VPW_InitPort(uint8 CHANEL);
extern uint8 PassThru_VPW_SetConfig(uint8 *Pack,uint8 CHANEL);
extern uint8 PassThru_VPW_RxPack_Normal(void);
extern uint8 PassThru_VPW_RxPack_Frame(uint8 *Pack);
extern uint8 PassThru_VPW_TxPack_Normal(uint8 *TXdata);

extern uint8 PassThru_PWM_ReadByte(uint8 *Byte);
extern uint8 PassThru_PWM_ReadAnsByte(uint8 *Byte);
extern uint8 PassThru_PWM_InitPort(uint8 CHANEL);
extern uint8 PassThru_PWM_SetConfig(uint8 *Pack,uint8 CHANEL);
extern uint8 PassThru_PWM_RxPack_Normal(void);
extern uint8 PassThru_PWM_TxPack_Normal(uint8 *TXdata);

extern uint8 PassThru_KWP_InitPort(uint8 CHANEL);
extern uint8 PassThru_KWP_SetConfig(uint8 *Pack,uint8 CHANEL);

extern uint8 PassThru_ISO_InitPort(uint8 CHANEL);
extern uint8 PassThru_ISO_SetConfig(uint8 *Pack,uint8 CHANEL);
extern uint8 PassThru_ISO_Init_Slow(uint8 *TXdata);
extern uint8 PassThru_ISO_RxPack_WABCO(void);
extern uint8 PassThru_ISO_RxPack_Normal(void);
extern uint8 PassThru_ISO_TxPack_WABCO(uint8 *TXdata);
extern uint8 PassThru_ISO_TxPack_Normal(uint8 *TXdata);

extern uint8 PassThru_CAN_SetCanBaud(uint8 Ch,uint32 CanBaud);
extern uint8 PassThru_CAN_GetCanPara(void);
extern uint8 PassThru_CAN_SetBaudRate(void);
extern uint8 PassThru_CAN_SetFilter(uint32 id, uint32 mask);
extern uint8 PassThru_CAN_RxPack_Auto(void);
extern uint8 PassThru_CAN_RxPack_Auto_Ext(void);
extern uint8 PassThru_CAN_RxPack_GetID(void);
extern uint8 PassThru_CAN_RxPack_Normal(void);
extern uint8 PassThru_CAN_RxPack_NotID(void);
extern uint8 PassThru_CAN_RxData(uint8 *RXdata);
extern uint8 PassThru_CAN_RxPack_J1939(void);
extern uint8 PassThru_CAN_RxPack_J1939_ReadOnly();
extern uint8 PassThru_CAN_TxPack_J1939(uint8 *TXdata);
extern uint8 PassThru_CAN_TxData(uint8 *TXdata, uint8 TXLen, uint32 CanID);
extern uint8 PassThru_CAN_TxPack_NotID(uint8 *TXdata);
extern uint8 PassThru_CAN_TxPack_Auto(uint8 *TXdata);
extern uint8 PassThru_CAN_TxPack_Auto_Ext(uint8 *TXdata);
extern uint8 PassThru_CAN_TxPack(uint8 *TXdata);//82 83合并
extern uint8 PassThru_CAN_TxPack_GetID(uint8 *TXdata);
extern uint8 PassThru_CAN_TxPack_Normal(uint8 *TXdata);
extern uint8 PassThru_CAN_TxPack_FlowControl(uint8 *TXdata);
extern void CAN_SetACCF_sff_Group( uint8 *Pack);
extern void CAN_SetACCF_sff( uint8 *Pack);
extern void CAN_SetACCF_eff_Group( uint8 *Pack);
extern void CAN_SetACCF_eff( uint8 *Pack);
extern void CAN_SetACCF( uint32_t ACCFMode );

extern uint8 PassThru_KWP_Init_Fast(uint8 *TXdata);
extern uint8 PassThru_KWP_Init_Slow(uint8 *TXdata);
extern uint8 PassThru_KWP_TxPack_TxData(uint8 *TXdata);
extern uint8 PassThru_KWP_TxPack_Normal(uint8 *TXdata);
extern uint8 PassThru_KWP_TxPack_Delay(uint8 *TXdata);
extern uint8 PassThru_KWP_TxPack_Volvo(uint8 *TXdata);
extern uint8 PassThru_KWP_RxPack_Cater(void);
extern uint8 PassThru_KWP_RxPack_Kwp2000(void);
extern uint8 PassThru_KWP_RxPack_Length(void);
extern uint8 PassThru_KWP_RxPack_Mitsub(void);
extern uint8 PassThru_KWP_RxPack_Nissan(void);
extern uint8 PassThru_KWP_RxPack_Normal(void);
extern uint8 PassThru_KWP_RxPack_Volvo(void);
extern uint8 PassThru_KWP_RxPack_J1708MID(void);
extern uint8 PassThru_KWP_RxPack_MazdaKWP(void);
extern uint8 PassThru_KWP_RxPack_DirectRead(void);
extern uint8 PassThru_KWP_RxPack_Interception(void);
extern void PassThru_KWP_delay_parse(uint8 *PackBuf);
extern void PassThru_KWP_delay_sent(void);


extern uint8 PassThru_BENZ_InitPort(uint8 CHANEL);
extern uint8 PassThru_BENZ_SetConfig(uint8 *Pack,uint8 CHANEL);

extern uint8 PassThru_WAVE_ReadWave(void);
extern uint8 PassThru_WAVE_InitPort(uint8 CHANEL);
extern uint8 PassThru_WAVE_SetConfig(uint8 *Pack,uint8 CHANEL);

extern uint8 PassThru_PLUSE_InitPort(uint8 CHANEL);
extern uint8 PassThru_PLUSE_SetConfig(uint8 *Pack,uint8 CHANEL);
//extern void PassThru_CAN_SELFTEST(uint8 can_select,uint8 selfmode,uint8 pin,uint32 canbaud,uint8 set_res);


//extern uint8  KeepComSwitch;
//extern uint8  KeepComSwitch2;
//extern uint8  KeepComSwitch1;
//extern UARTMODE UartCtlReg;
extern uint8 rx164dat(void);
extern void tx164dat(uint8 dat);
//extern uint8  LineLevelFlag;

extern uint8 ALL_KEEP_COM(void);
//extern uint8 Stop_bit_5bps_flag;


extern uint8 PassThru_CAN_RxPack_Auto_ReadOnly(void);
extern uint8 PassThru_CAN_RxPack_AutoExt_ReadOnly(void);
extern uint8 PassThru_CAN_TxPack_J1939_SendOnly(uint8 *TXdata);
extern uint8 PassThru_KWP_RxPack_Length3(void);
extern uint32 GetAddresses32(uint8 *buffer);*/
#endif
