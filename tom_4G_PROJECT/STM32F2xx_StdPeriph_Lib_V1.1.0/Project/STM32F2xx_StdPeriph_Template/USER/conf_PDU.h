#ifndef _CONFIG_PDU_H_
#define _CONFIG_PDU_H_
#include "common.h"
 

#define Command_QUEUE_LENGTH				(100)	//�������г���
#define Command_Task_MASK					(8001)	//�����������
#define Com_Proc_QUEUE_LENGTH				(1)		//USB���ն��г���
#define Com_Proc_Task_MASK					(8002)	//USB����������
#define PDU_9141_OneByte_QUEUE_LENGTH		(1)		//9141 one byte mode���ն��г���
#define PDU_9141_OneBytec_Task_MASK			(8003)	//9141 one byte mode����������

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
#define TXQUEUEBUFLEN	4110//6144//4096 //6144 //���л�������С4K

#define CAN_IRQn USB_LP_CAN1_RX0_IRQn
//#include   "LPC1700CAN.h"

//ʾ����
#define WAVE_DETING   1
#define EXIT_WAVE     0

//��ʱ��ʱ��Դ���� = MCK/2  
#define Fpclk	60000000

//���������ȶ���
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
#define TXQUEUEBUFLEN	4110//6144//4096 //6144 //���л�������С6K

#define DMATXBUFLEN  250	//500���������250������������ó�250
#define DMARXBUFLEN  250
#define DMATEMPLEN   250  //��ʱ��������С ��Ҫ���ڴ���������������
//************��ȡ��ѹ����
#define VCC_PASS_DRV 0//��ȡ������ѹ AD0.0
#define VCC_PASS_MUX1 1//��ȡline0�ϵ�ѹ AD0.1
#define VCC_PASS_MUX0 4//��ȡline1 �ϵ�ѹ AD0.4
#define VCC_PASS_CMP  2//��ȡ�Ƚϵ�ѹ AD0.2

//����ͨ����
#define CHANEL0 0 //����mux0 mux1��can����
#define CHANEL1 1 //���� can1�ϲ���
//C��̵�������
#define C_CHANNEL_ALL_OFF  0   //������
#define CAN_BUS_CHANNEL    1   //��CANBUS
#define KL_BUS_CHANNEL     2   //��KLBUS
#define ANALOG_CHANNEL_ON  3   //�򿪶˿ڵ�ѹ
#define ANALOG_CHANNEL_OFF 4   //�رն˿ڵ�ѹ
#define DRIVE_8V           5   //������ѹΪ����Ϊ8V
#define DRIVE_12V          6   //������ѹ����Ϊ12V
#define DRIVE_5V           7   //������ѹ����Ϊ5V
#define NO_DRIVE           8   //������ѹ����
#define CLK_OUT_ENABLE     9   //����ʱ���������
#define CLK_OUT_DISENABLE  10  //��ֹCLK���
#define CAN_TWO_BUS_CHANNEL 11 //�ڶ�·can��ͨ
#define CAN_TWO_BUS_CHANNEL_DIS 12 //�ڶ�·can��ͨ

//rs232 485����
#define rs232 0
#define rs485 1
#define CLR_RS485_232_TO_OBD 2
//rs485 ���ͽ��տ���
#define RS485_TD 0
#define RS485_RX 1
//ͨѶ�ߺ�
#define LINE0              0
#define LINE1              1
#define PWM_LINE           2   //��ʱ��ͬʱ�õ�LINE0,LINE1
//ͨѶ������������
#define NO_LOAD            0   //����
#define UP_LOAD            1   //����
#define DOWN_LOAD          2   //����
#define PWM_LOAD           3   //������
//ͨ������
#define CHANNEL_OFF        0  //����ͨ���Ͽ�
//�ڶ�·can������
#define SECOND_CAN_SINGAL 0
#define SECOND_CAN_NORMAL 1
#define CLR_SECOND_CAN_TO_OBD 2
//���������Ϊǿ̬����̬ 1:��̬,���ϳ�״̬;0:ǿ̬,���������ݷ���
#define SET_OUT_STATUS_0 0
#define SET_OUT_STATUS_1 1
//�߼���ƽ����
#define LOGIC_1    1
#define LOGIC_0    0
//������������������
#define UP_PULL   1
#define DOWN_PULL 0
//����������ģ��
#define  SET_MCU_MASK              0x00
#define  SET_BUS_MASK              0x10
#define  INIT_ECU_MASK             0x20
#define  DATA_MASK                 0x30
#define  KEEP_COM_MASK             0x40
#define  DELAY_TX_MASK             0x50
#define  SPECIAL_MASK              0x60
#define  CAN_ID_FILT			   0x70
#define FLASH_MASK				   0x90
#define  KEEP_ALWAY			       0xC0 // �Ǳ���ʾ����
#define PROTOCOLTYPE			   0xD0

//#define  LKT4200_MASK							 0x90
#define  SYS_FUN_MASK              0xF0
//Э��������ģ��
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
//CAN TWO ID����
#define SET_CAN_TWO_ID             0X81 //�ڶ���id������
//ʵ�ʵĿ����ֱ������������ͺ�Э��������ģ����϶���,
//�������ģ����û�ж��壬�򲻴��ڸ���ֵ
//1��ͷ
#define  SET_CAN_BUS               SET_BUS_MASK|CAN_BUS_MASK    //CAN
#define  SET_PWM_BUS               SET_BUS_MASK|PWM_BUS_MASK    //PWM
#define  SET_VPW_BUS               SET_BUS_MASK|VPW_BUS_MASK    //VPWM
#define  SET_KWP2K_BUS             SET_BUS_MASK|KWP2K_BUS_MASK  //kwp2000
#define  SET_AUDI_BUS              SET_BUS_MASK|AUDI_BUS_MASK   //�µ�
#define  SET_BENZ_BUS              SET_BUS_MASK|BENZ_BUS_MASK   //�µ�
#define  SET_FLASH_BUS             SET_BUS_MASK|FLASH_BUS_MASK   //�µ�
#define  SET_CAN_TWO_BUS           SET_BUS_MASK|CAN_TWO_BUS_MASK    //CAN
#define  SET_WAVE_BUS              SET_BUS_MASK|WAVE_BUS_MASK    //����

//2��ͷ
#define  KWP2K_ECU_INIT            INIT_ECU_MASK|KWP2K_BUS_MASK  //��ʼ��
#define  AUDI_ECU_INIT             INIT_ECU_MASK|AUDI_BUS_MASK
#define  BENZ_ECU_INIT             INIT_ECU_MASK|BENZ_BUS_MASK
//3��ͷ
#define  CAN_DATA                  DATA_MASK|CAN_BUS_MASK    //CAN
#define  PWM_DATA                  DATA_MASK|PWM_BUS_MASK    //PWM
#define  VPW_DATA                  DATA_MASK|VPW_BUS_MASK    //VPWM
#define  KWP2K_DATA                DATA_MASK|KWP2K_BUS_MASK  //kwp2000
#define  AUDI_DATA                 DATA_MASK|AUDI_BUS_MASK   //�µ�
#define  BENZ_DATA                 DATA_MASK|BENZ_BUS_MASK   //����
#define  FLASH_DATA                DATA_MASK|FLASH_BUS_MASK   //������
#define  CAN_DATA_TWO              DATA_MASK|CAN_TWO_BUS_MASK    //CAN
#define  WAVE_DATA                 DATA_MASK|WAVE_BUS_MASK   //ʾ����
//4��ͷ
#define  CAN_KEEP_COM              KEEP_COM_MASK|CAN_BUS_MASK    //CAN
#define  PWM_KEEP_COM              KEEP_COM_MASK|PWM_BUS_MASK    //PWM
#define  VPW__KEEP_COM             KEEP_COM_MASK|VPW_BUS_MASK    //VPWM
#define  K2K_KEEP_COM              KEEP_COM_MASK|KWP2K_BUS_MASK  //kwp2000
#define  AUDI_KEEP_COM             KEEP_COM_MASK|AUDI_BUS_MASK   //�µ�

//F��ͷ
#define   CHECKSUM_ERR             0xFE
#define   UNKNOW_TYPE              0xFD
#define   INIT_ERR                 0xFC

//״̬��StatusByte
#define  NO_ANY_INIT               0x00
#define  PORT_INIT_OK              0x01  //bit0
#define  ECU_INIT_OK               0x02  //bit1
//
#define ECU_BUF_LEN  30

#define UART_WAIT_TIME   500   // ���ڵȴ�ʱ��50*10ms 

//ͨ��ģʽ
#define NO_RX_CHANNEL         0
#define ONLY_CH0RX            1      //ֻ��ͨ��0��
#define ONLY_CH1RX            2      //ֻ��ͨ��1����
#define CH0_CH1_RX            3      //ͨ��0��ͨ��1ͬʱ����

#define NO_TX_CHANNEL         0
#define ONLY_CH0TX            1      //ֻ��ͨ��0��
#define ONLY_CH1TX            2      //ֻ��ͨ��1����
#define CH0_CH1_TX            3      //ͨ��0��ͨ��1ͬʱ����

//����ģʽ
#define  STRONG_PULL_DOWN     0   //ǿ����
#define  STRONG_PULL_UP       1   //ǿ����
#define  PUSH_PULL_P          2   //����ʽ������1ǿ����������0ǿ��������̬ǿ����
#define  PUSH_PULL_N          3   //����ʽ������1ǿ����������0ǿ��������̬ǿ������NISSAN����ˣ�
 
//
#define NORMAL_RX     0     //������żУ�鹦��
#define DISEN_PARITY  1     //������żУ�鹦��
//J1860 ���
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

//ͨѶ����
#define AUTO_KEEP_COM_SWITCH_BIT        0x01  //BIT0  ��λ���Զ�ͨѶ����λ
#define AUTO_KEEP_COM_ANSWER_BIT        0x02  //BIT1  �Ƿ�ȴ���Ӧ
#define AUTO_KEEP_COM_RXTX_DELAY_BIT    0X04  //BIT2  ����ECU����/�������ݺ��Ƿ���ʱ�Զ�����
//bit0����
#define AUTO_KEEP_OFF                   0x00  //�ر��Զ�����
#define AUTO_KEEP_ON                    0x01  //�����Զ�����
//bit1����
#define KEEP_COM_ANS_OFF                0x00  //�������Զ�ͨѶʱECU�Ļ�Ӧ
#define KEEP_COM_ANS_ON                 0x02  //�����Զ�ͨѶ����ʱECU�Ļ�Ӧ
//bit2����
#define KEEP_COM_R_DELAY_ON            0x00  //������ʱ
#define KEEP_COM_R_DELAY_OFF           0x04  //������ʱ����ʱ���뷢��,�������һ����ֻ���Ͳ����գ���VPW,ĳЩCAN 


////PORT_NUM>=16
//#define INIT_PORT_FUN(PORT_NUM,PORT_FUN)   LPC_PINCON->PINSEL1=(LPC_PINCON->PINSEL1& ~(3 << ((PORT_NUM-16) * 2)))|(PORT_FUN << ((PORT_NUM-16) * 2))
////PORT_NUM<=15
//#define INIT_PORT_FUNC(PORT_NUM,PORT_FUN)  LPC_PINCON->PINSEL0=(LPC_PINCON->PINSEL0& ~(3 <<( PORT_NUM * 2)))|(PORT_FUN <<( PORT_NUM * 2)) 

//������У׼ϵ��
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
#define IAP_ENTER_ADR   0x1FFF1FF1  // IAP��ڵ�ַ����

/* ����IAP������ */
                                    //   ����           ����
#define     IAP_SELECTOR        50  // ѡ������     ����ʼ�����š����������š�
#define     IAP_RAMTOFLASH      51  // ��������     ��FLASHĿ���ַ��RAMԴ��ַ��д���ֽ�����ϵͳʱ��Ƶ�ʡ�
#define     IAP_ERASESECTOR     52  // ��������     ����ʼ�����š����������š�ϵͳʱ��Ƶ�ʡ�
#define     IAP_BLANKCHK        53  // �������     ����ʼ�����š����������š�
#define     IAP_READPARTID      54  // ������ID     ���ޡ�
#define     IAP_BOOTCODEID      55  // ��Boot�汾�� ���ޡ�
#define     IAP_COMPARE         56  // �Ƚ�����     ��Flash��ʼ��ַ��RAM��ʼ��ַ����Ҫ�Ƚϵ��ֽ�����
#define     IAP_ReadSN          58  // �Ƚ�����     ��Flash��ʼ��ַ��RAM��ʼ��ַ����Ҫ�Ƚϵ��ֽ�����

/* ����IAP����״̬�� */
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
#define SEND_BLOCK  1 //ֹͣ����λ����������
#define SEND_RUN    0 //��������λ����������
//ecuͨѶ����,����ecu��������ʱ���ж���ı�־
#define ecu_send 0
#define ecu_recv 1
#define ecu_stop 2

#define MAX_CANID_COUNT  256 				 //  ���á�can id����
#define CAN_EXTID_MASK   0x1CFFF800  //CAN��չID���� ����ʶ���׼ID����չID
#define CAN_StructSize   72 				 //Atmel CAN�ṹ���ֽڴ�С  �Լ��޸����ԭ���Ĵ���

#define CAN_TX_Request_MAXNUM	10			//CAN��������������Դ���
#define USB_TX_Request_MAXNUM	10000		//USB��������������Դ���
#define CAN0_CHANNL				0			//new ʹ��CAN0��CAN1�ı�־
#define CAN1_CHANNL				1			//
#define CHANNL0					0			//Э��ͨ��0 �� Э������0  CAN PWM VPW����ͨ��  ����ֻ��һ��ͨ��
#define CHANNL1					1			//Э��ͨ��1 �� Э������1  
//#define CAN_SET_BAUDRATE			0X80	//CAN���ò�����
//#define CAN_SET_BitSamplePoint		0X82    //CAN���ò�����
//#define CAN_SET_SyncJumpWidth		0X8F	//CAN����ͬ������
#define NON_SYSTEM_CONFIRM			0X00	//��ϵͳȷ�Ϸ�����
#define SYSTEM_CONFIRM				0X01	//ϵͳȷ�Ϸ�����
#define INDICATION_CONFIRM			0X02	//ָʾȷ�Ϸ�����
#define ECU_CONFIRM					0X03	//ECUȷ�Ϸ�����
#define START_MsgFilter				0X01	//����Filter
#define STOP_MsgFilter				0X02    //ֹͣFilter
#define CLEAR_MsgFilter				0X03	//���Filter
#define FLT_PASS 					1		//����������ʹ��ͨ��
#define FLT_BLOCK					2		//����������ʹ�ܲ�ͨ��
#define CAN_CHANNEL_START			0X01	//CANЭ��ͨ������
#define CAN_CHANNEL_CLOSE			0X02    //CANЭ��ͨ���ر�
#define CAN_PAUSE					0X03	//CAN��ͣ
#define CAN_CONTINUE				0X04	//CAN����
#define CAN_STATUS_CHECK			0X05	//�������״̬
#define TX_BUFFER_CLEAR				0X01	//���TX BUFFER
#define RX_BUFFER_CLEAR				0X02    //CAN���RX BUFFER
#define DEVICE_ACTIVE_STEP1			0X01	//�豸�����1
#define DEVICE_ACTIVE_STEP2			0X02    //�豸�����2
#define PROTOCOL_START				0X01	//Э������������
#define PROTOCOL_CLOSE				0X00	//Э��رղ�������
#define PROTOCOL_RX_START			0X01	//Э���������������
#define PROTOCOL_RX_CLOSE			0X00	//Э����չرղ�������
#define PRE_REPLY_START				0X01    //Ԥ�ظ����ܿ���
#define PRE_REPLY_CLOSE				0X02    //Ԥ�ظ����ܹر�
#define PRE_REPLY_PAUSE				0X03    //Ԥ�ظ�������ͣ
#define PRE_REPLY_CONTINUE			0X04    //Ԥ�ظ����ܼ���
#define PRE_REPLY_GROUPS_SET		0X05    //����Ԥ�ظ���
#define PRE_REPLY_GROUPS_DEL		0X06    //ɾ��Ԥ�ظ���
#define PRE_PROCESS_SET				0X07    //����Ԥ���� ����CAN��֡ʹ��
#define PRE_PROCESS_DEL				0X08    //ɾ��Ԥ����
#define PWM_PRE_REPLY_DATA_MAX		12		//PWMԤ�ظ�������󳤶�
#define CAN_STID_FILTER_OFFSET_NUM  0x00	//CAN ID��������� ��׼ID��0x00 - 0x7F  128��
#define CAN_EXTID_FILTER_OFFSET_NUM 0x80	//CAN ID��������� ��չID��0x80 - 0xBF  64��
#define CAN_MAX_FILTER_OFFSET_NUM   0xC0	//����0xC0��Ч
#define KWP_5_BAUD_INIT_SEQUENCE    1		//���ٳ�ʼ��ģʽ
#define KWP_FAST_INIT_SEQUENCE		2		//���ٳ�ʼ��ģʽ
#define KWP_NO_INIT_SEQUENCE		3		//�޳�ʼ��ģʽ
#define PDU_CONFIRM_SW_ON			0x00	//��λ����Ҫȷ�ϻظ�
#define PDU_CONFIRM_SW_OFF			0x01	//��λ������Ҫȷ�ϻظ�
#define PDU_CP_P2MAX_SW_ON			0x00	//CP_P2MAX��Ч
#define PDU_CP_P2MAX_SW_OFF			0x02	//CP_P2MAXʧЧ
#define PDU_CP_SWCAN_HV_TX			0x04	//����CAN�ߵ�ѹģʽ
#define PDU_INIT_ACTION				1       //��ʼ��������
#define PDU_INIT_IDLE				0       //��ʼ��δ����
#define PDU_GET_POWER_VOLT			0x02	//��PDU�豸�����ѹ
#define PDU_SET_PORG_VOLT			0x03	//���ñ�̵�ѹ
#define PDU_GET_PORG_VOLT			0x04	//��ȡ��̵�ѹ
#define PDU_GET_PIN_VOLT			0x05	//��ȡ�ܽŵ�ѹ
#define PDU_GET_CABLE_ID			0x06	//��ȡCableID
#define PDU_SET_PIN_FLOAT			0x07	//������������
#define PDU_SET_PIN_GROUND 			0x08	//�������Žӵ�
#define ISO_STD_INIT				0x000000000  //9141 14230 5 Baud init ���յ���ַȡ����ʼ���ɹ� ��׼��ʼ��
#define ISO_INV_KB2					0x000000001  //9141 5 Baud init ����KEY2ȡ�����ʼ���ɹ�
#define ISO_INV_ADD					0x000000002  //������KEY2ȡ����  ���յ���ַ��ȡ�� ��ʼ���ɹ�
#define ISO_9141_STD				0x000000003  //������KEY2 ��ʼ���ɹ�
#define PDU_CANID_EX_MASK			0x1FFFF800   //Can ��չ֡����
#define PDU_SET_CMP_MANUAL			1
#define PDU_SET_CMP_AUTO			0
#define PDU_DOIP_STATUS_INIT		1
#define PDU_DOIP_STATUS_CLOSE		2
#define PDU_DOIP_STATUS_SUSPEND		3
#define PDU_DOIP_STATUS_ERR			4
#define PDU_VCCIN_OFFSET_VAULE		900    //�ɼ�VCCIN��ѹ����900mV
#define PDU_SET_CPVAULE_MAX_LEN		64	   //��������CPֵ��������

/*-----------------------------------------------------------------------------
 *         Tab
 *----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 *         enum
 *----------------------------------------------------------------------------*/
enum UartFormat				//UARTЭ���ʽ
{
	 HEADER0          =0xA5,	//֡ͷֵ
	 HEADER1          =0xA5,	//֡ͷֵ
	 FrmHdr0Flg       =0x00,	//֡ͷ0��־
	 FrmHdr1Flg       =0x01,	//֡ͷ1��־
	 FrmLngthHFlg     =0x02,	//���ȸ�λ��־
	 FrmLngthLFlg     =0x03,	//���ȵ�λ��־
	 FrmLngthDat      =0x04,	//���ݱ�־
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
extern PDU_VERSION_STRUCT PDU_Version;			//PDU�汾��Ϣ


#endif // _CONFIG_PDU_H_
