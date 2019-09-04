#ifndef _CONFIG_PDU_H_
#define _CONFIG_PDU_H_
#include "common.h"
 

#define Command_QUEUE_LENGTH				(100)	//命令处理队列长度
#define Command_Task_MASK					(8001)	//命令处理任务码
#define Com_Proc_QUEUE_LENGTH				(1)		//USB接收队列长度
#define Com_Proc_Task_MASK					(8002)	//USB接收任务码
#define PDU_9141_OneByte_QUEUE_LENGTH		(1)		//9141 one byte mode接收队列长度
#define PDU_9141_OneBytec_Task_MASK			(8003)	//9141 one byte mode接收任务码

/*-----------------------------------------------------------------------------
 *         Headers
 *----------------------------------------------------------------------------*/
//#include "board.h"
//#include "ProtocolDrives.h"
/*-----------------------------------------------------------------------------
 *         define
 *----------------------------------------------------------------------------*/
typedef unsigned char  uint8;                   /* defined for unsigned 8-bits integer variable 	???8?????  */
typedef signed   char  int8;                    /* defined for signed 8-bits integer variable		???8?????  */
typedef unsigned short uint16;                  /* defined for unsigned 16-bits integer variable 	???16????? */
typedef signed   short int16;                   /* defined for signed 16-bits integer variable 		???16????? */
typedef unsigned int   uint32;                  /* defined for unsigned 32-bits integer variable 	???32????? */
typedef signed   int   int32;                   /* defined for signed 32-bits integer variable 		???32????? */
typedef float          fp32;                    /* single precision floating point variable (32bits) ??????(32???) */
typedef double         fp64;                    /* double precision floating point variable (64bits) ??????(64???) */
/*-----------------------------------------------------------------------------
 *        control interrupt define
 *----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 *        MINI VCI define
 *----------------------------------------------------------------------------*/
#define TXQUEUEBUFLEN	4110//6144//4096 //6144 //队列缓冲区大小4K

#define CAN_IRQn USB_LP_CAN1_RX0_IRQn
//#include   "LPC1700CAN.h"

//示波器
#define WAVE_DETING   1
#define EXIT_WAVE     0

//定时器时钟源常量 = MCK/2  
#define Fpclk	60000000

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
#define TXQUEUEBUFLEN	4110//6144//4096 //6144 //队列缓冲区大小6K

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
//逻辑电平定义
#define LOGIC_1    1
#define LOGIC_0    0
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
#define FLASH_MASK				   0x90
#define  KEEP_ALWAY			       0xC0 // 仪表显示测试
#define PROTOCOLTYPE			   0xD0

//#define  LKT4200_MASK							 0x90
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


////PORT_NUM>=16
//#define INIT_PORT_FUN(PORT_NUM,PORT_FUN)   LPC_PINCON->PINSEL1=(LPC_PINCON->PINSEL1& ~(3 << ((PORT_NUM-16) * 2)))|(PORT_FUN << ((PORT_NUM-16) * 2))
////PORT_NUM<=15
//#define INIT_PORT_FUNC(PORT_NUM,PORT_FUN)  LPC_PINCON->PINSEL0=(LPC_PINCON->PINSEL0& ~(3 <<( PORT_NUM * 2)))|(PORT_FUN <<( PORT_NUM * 2)) 

//波特率校准系数
#define BAUD_ADJUST_CONST    22
//LineLevelFlag
#define  LINE_UNVALID     0
#define  LINE_VALID       1

#define  WAIT_EDGE    0
#define  UP_EDGE      1
#define  DOWN_EDGE    2
#define 	LOW    0
#define 	HIGH   1

//#define zyIrqDisable()  __disable_irq()
//#define zyIrqEnable()   __enable_irq()

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

#define MAX_CANID_COUNT  256 				 //  设置　can id过滤
#define CAN_EXTID_MASK   0x1CFFF800  //CAN扩展ID掩码 用来识别标准ID与扩展ID
#define CAN_StructSize   72 				 //Atmel CAN结构体字节大小  自己修改配合原来的代码

#define CAN_TX_Request_MAXNUM	10			//CAN发送请求最大重试次数
#define USB_TX_Request_MAXNUM	10000		//USB发送请求最大重试次数
#define CAN0_CHANNL				0			//new 使用CAN0或CAN1的标志
#define CAN1_CHANNL				1			//
#define CHANNL0					0			//协议通道0 或 协议总线0  CAN PWM VPW有两通道  其他只有一个通道
#define CHANNL1					1			//协议通道1 或 协议总线1  
//#define CAN_SET_BAUDRATE			0X80	//CAN设置波特率
//#define CAN_SET_BitSamplePoint		0X82    //CAN设置采样点
//#define CAN_SET_SyncJumpWidth		0X8F	//CAN设置同步脉宽
#define NON_SYSTEM_CONFIRM			0X00	//非系统确认服务码
#define SYSTEM_CONFIRM				0X01	//系统确认服务码
#define INDICATION_CONFIRM			0X02	//指示确认服务码
#define ECU_CONFIRM					0X03	//ECU确认服务码
#define START_MsgFilter				0X01	//启动Filter
#define STOP_MsgFilter				0X02    //停止Filter
#define CLEAR_MsgFilter				0X03	//清除Filter
#define FLT_PASS 					1		//过滤器类型使能通过
#define FLT_BLOCK					2		//过滤器类型使能不通过
#define CAN_CHANNEL_START			0X01	//CAN协议通道启动
#define CAN_CHANNEL_CLOSE			0X02    //CAN协议通道关闭
#define CAN_PAUSE					0X03	//CAN暂停
#define CAN_CONTINUE				0X04	//CAN继续
#define CAN_STATUS_CHECK			0X05	//检查总线状态
#define TX_BUFFER_CLEAR				0X01	//清除TX BUFFER
#define RX_BUFFER_CLEAR				0X02    //CAN清除RX BUFFER
#define DEVICE_ACTIVE_STEP1			0X01	//设备激活步骤1
#define DEVICE_ACTIVE_STEP2			0X02    //设备激活步骤2
#define PROTOCOL_START				0X01	//协议启动允许传输
#define PROTOCOL_CLOSE				0X00	//协议关闭不允许传输
#define PROTOCOL_RX_START			0X01	//协议接收启动允许传输
#define PROTOCOL_RX_CLOSE			0X00	//协议接收关闭不允许传输
#define PRE_REPLY_START				0X01    //预回复功能开启
#define PRE_REPLY_CLOSE				0X02    //预回复功能关闭
#define PRE_REPLY_PAUSE				0X03    //预回复功能暂停
#define PRE_REPLY_CONTINUE			0X04    //预回复功能继续
#define PRE_REPLY_GROUPS_SET		0X05    //设置预回复组
#define PRE_REPLY_GROUPS_DEL		0X06    //删除预回复组
#define PRE_PROCESS_SET				0X07    //设置预处理 发送CAN多帧使用
#define PRE_PROCESS_DEL				0X08    //删除预处理
#define PWM_PRE_REPLY_DATA_MAX		12		//PWM预回复数据最大长度
#define CAN_STID_FILTER_OFFSET_NUM  0x00	//CAN ID过滤器组号 标准ID：0x00 - 0x7F  128组
#define CAN_EXTID_FILTER_OFFSET_NUM 0x80	//CAN ID过滤器组号 扩展ID：0x80 - 0xBF  64组
#define CAN_MAX_FILTER_OFFSET_NUM   0xC0	//超过0xC0无效
#define KWP_5_BAUD_INIT_SEQUENCE    1		//慢速初始化模式
#define KWP_FAST_INIT_SEQUENCE		2		//快速初始化模式
#define KWP_NO_INIT_SEQUENCE		3		//无初始化模式
#define PDU_CONFIRM_SW_ON			0x00	//上位机需要确认回复
#define PDU_CONFIRM_SW_OFF			0x01	//上位机不需要确认回复
#define PDU_CP_P2MAX_SW_ON			0x00	//CP_P2MAX生效
#define PDU_CP_P2MAX_SW_OFF			0x02	//CP_P2MAX失效
#define PDU_CP_SWCAN_HV_TX			0x04	//单线CAN高电压模式
#define PDU_INIT_ACTION				1       //初始化进行中
#define PDU_INIT_IDLE				0       //初始化未进行
#define PDU_GET_POWER_VOLT			0x02	//读PDU设备供电电压
#define PDU_SET_PORG_VOLT			0x03	//设置编程电压
#define PDU_GET_PORG_VOLT			0x04	//读取编程电压
#define PDU_GET_PIN_VOLT			0x05	//读取管脚电压
#define PDU_GET_CABLE_ID			0x06	//读取CableID
#define PDU_SET_PIN_FLOAT			0x07	//设置引脚悬空
#define PDU_SET_PIN_GROUND 			0x08	//设置引脚接地
#define ISO_STD_INIT				0x000000000  //9141 14230 5 Baud init 接收到地址取反初始化成功 标准初始化
#define ISO_INV_KB2					0x000000001  //9141 5 Baud init 发送KEY2取反码初始化成功
#define ISO_INV_ADD					0x000000002  //不发送KEY2取反码  接收到地址码取反 初始化成功
#define ISO_9141_STD				0x000000003  //接收完KEY2 初始化成功
#define PDU_CANID_EX_MASK			0x1FFFF800   //Can 扩展帧掩码
#define PDU_SET_CMP_MANUAL			1
#define PDU_SET_CMP_AUTO			0
#define PDU_DOIP_STATUS_INIT		1
#define PDU_DOIP_STATUS_CLOSE		2
#define PDU_DOIP_STATUS_SUSPEND		3
#define PDU_DOIP_STATUS_ERR			4
#define PDU_VCCIN_OFFSET_VAULE		900    //采集VCCIN电压补偿900mV
#define PDU_SET_CPVAULE_MAX_LEN		64	   //单次配置CP值的最大个数

/*-----------------------------------------------------------------------------
 *         Tab
 *----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 *         enum
 *----------------------------------------------------------------------------*/
enum UartFormat				//UART协议格式
{
	 HEADER0          =0xA5,	//帧头值
	 HEADER1          =0xA5,	//帧头值
	 FrmHdr0Flg       =0x00,	//帧头0标志
	 FrmHdr1Flg       =0x01,	//帧头1标志
	 FrmLngthHFlg     =0x02,	//长度高位标志
	 FrmLngthLFlg     =0x03,	//长度低位标志
	 FrmLngthDat      =0x04,	//数据标志
	 FrmEndFlg        =0x05,	//
	 FrmPureBuf       =0x06,	//
};


/*-----------------------------------------------------------------------------
 *         global variable
 *----------------------------------------------------------------------------*/
//extern uint8_t 	UART4_RX_Flag;
//extern uint8_t 	UART4_RX_Temp;
//extern uint8_t 	UART4_RX_ErrFlag;
//extern uint8_t 	RxFrameFlag;
//extern uint8 		RxChecksum;
//extern uint16 	ByteCounter;
//extern uint16 	revPt;
//extern uint8 		USB_RxStackTop;
//extern uint8 		TempProcBuf[TXQUEUEBUFLEN];//[DMARXBUFLEN];
//extern uint8 		TempProcBuf[TXQUEUEBUFLEN];
//extern uint8	 	UartTxQueueBuf_ext[TXQUEUEBUFLEN];
//extern uint32_t	Uart4DMA_Channel;
//extern uint8 UartTxSwitch;
//extern uint8_t EcuComRxBuf_temp[ECURXBUFLEN];
extern uint8_t EcuComRxBuf[ECURXBUFLEN];
////extern uint8 *EcuComRxBuf;
//extern uint8 EcuComTxBuf[ECUTXBUFLEN];
//extern const uint32_t SystemFrequency;
//extern uint32_t CAN1_fifo_entries;
//extern PDU_CAN_RAW_DATA CAN1_rxMailbox;

extern uint32_t PDU_CAN0_Baudrate;
extern uint32_t PDU_CAN1_Baudrate;
extern PDU_VERSION_STRUCT PDU_Version;			//PDU版本信息


#endif // _CONFIG_PDU_H_
