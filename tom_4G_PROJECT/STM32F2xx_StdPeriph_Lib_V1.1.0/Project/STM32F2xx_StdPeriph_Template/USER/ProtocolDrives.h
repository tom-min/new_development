#ifndef __PROTOCOL_DRIVES_H
#define __PROTOCOL_DRIVES_H


#include <string.h>
#include <stdint.h>
//#include "HardwareDrives.h"
//#include "TransportProtocol.h"
//#include "conf_PDU.h"
//#include "hal_can.h"

// Basic data types                                                                              
                                                                                                 
typedef unsigned char      UNUM8;      // Unsigned numeric 8 bits                                
typedef signed char        SNUM8;      // Signed numeric 8 bits                                  
typedef unsigned short     UNUM16;     // Unsigned numeric 16 bits                               
typedef signed short       SNUM16;     // Signed numeric 16 bits                                 
typedef unsigned long      UNUM32;     // Unsigned numeric 32 bits                               
typedef signed long        SNUM32;     // Signed numeric 32 bits                                 
typedef char               CHAR8;      // ASCII-coded 8-bit character value (ISO8859-1 (Latin 1))

#define DEVICE_RETURN_CODE_UNKNOWN    0
#define DEVICE_RETURN_CODE_OK         1
#define DEVICE_RETURN_CODE_BUS_BUSY   2
#define DEVICE_RETURN_CODE_BUS_ERROR  3
#define DEVICE_RETURN_CODE_DATA_ERROR 4
#define DEVICE_RETURN_CODE_NO_DATA    5
#define DEVICE_RETURN_CODE_DATA       6
#define DEVICE_SPECIAL_STATUS   	  7

#define PDU_PROTOCOL_INIT            0x01
#define PDU_PROTOCOL_CLOSE           0x02
#define PDU_PROTOCOL_PAUSE           0x03
#define PDU_PROTOCOL_CONTINUE        0x04

////通道定义 Physical Layer Standard protocol
//#define CHANNEL_DEVICE_INFORMATION     0x00
//#define CHANNEL_ISO_11898_2_DWCAN      0x01
//#define CHANNEL_ISO_11898_3_DWFTCAN    0x02
//#define CHANNEL_ISO_9141_2_UART        0x03
//#define CHANNEL_ISO_14230_1_UART       0x04
//#define CHANNEL_SAE_J1708_UART         0x05
//#define CHANNEL_SAE_J1939_11_DWCAN     0x06
//#define CHANNEL_SAE_J1850_PWM          0x07
//#define CHANNEL_SAE_J1850_VPW          0x08
//#define CHANNEL_SAE_J2411_SWCAN        0x09
//#define CHANNEL_SAE_J2610_UART         0x0A

//通道定义 Physical Layer Standard protocol
#define CHANNEL_DEVICE_INFORMATION      0x00
#define CHANNEL_ISO_9141				0x03
#define CHANNEL_ISO_14230				0x04
#define CHANNEL_SAE_J1708				0x05
#define CHANNEL_SAE_J1939				0x06
#define CHANNEL_SAE_J1850_PWM			0x07
#define CHANNEL_SAE_J1850_VPW			0x08
#define CHANNEL_SAE_J2411				0x09
#define CHANNEL_SAE_J2610				0x0A
#define CHANNEL_ISO_15765_2				0x0B
#define CHANNEL_ISO_13400				0x0C
#define CHANNEL_ISO_11898_2_DWCAN       0x11
#define CHANNEL_ISO_11898_3_DWFTCAN     0x12
#define CHANNEL_ISO_11992_1_DWCAN	    0x13
#define CHANNEL_ISO_9141_2_UART         0x14
#define CHANNEL_ISO_14230_1_UART        0x15
#define CHANNEL_SAE_J2610_UART          0x16
#define CHANNEL_SAE_J1708_UART          0x17
#define CHANNEL_SAE_J1939_11_DWCAN      0x18
//#define CHANNEL_SAE_J1850_PWM           0x19
//#define CHANNEL_SAE_J1850_VPW           0x1A
#define CHANNEL_SAE_J2411_SWCAN        0x1B


//诊断函数功能定义
#define FUNCTION_FILTER				 0x01
#define FUNCTION_CLEAR_BUFFER		 0x02
#define FUNCTION_PROTOCOL            0x03
#define FUNCTION_SETPINTYPE          0x04
#define FUNCTION_SETCOMPARAM         0x05
#define FUNCTION_GETCOMPARAM         0x06
#define FUNCTION_REQUEST             0x07
#define FUNCTION_INDICATION          0x08
#define FUNCTION_ABORTREQUEST        0x09
#define FUNCTION_PRE_REPLY			 0x0A
#define FUNCTION_SOFT_PULSE			 0x0B
#define FUNCTION_SET_PIN8			 0x0C
#define FUNCTION_SWITCHPROTOCOL      0xD0

#define FUNCTION_READ_VERSION        0xF0
#define	FUNCTION_MVCI_VOLT			 0xF1
#define	FUNCTION_HARDWARETEST        0xF2
#define	FUNCTION_ERASE_FW            0xF3    //擦除PDU固件
#define	FUNCTION_SET_CMP_VOLT        0xF6    //设置比较电压
#define	FUNCTION_GET_TIMESTAMP		 0xF8			
#define FUNCTION_REBOOT				 0XF9
#define FUNCTION_RESET				 0XFA
#define	FUNCTION_GET_UID			 0xFB
#define	FUNCTION_GET_SN				 0xFC
#define FUNCTION_ACTIVE              0xFF




#define DATA_TYPE_MASK               0xC000
#define DATA_TYPE_UNUM32             0x0000
#define DATA_TYPE_BYTEFIELD          0x4000
#define DATA_TYPE_LONGFIELD          0x8000
#define DATA_TYPE_STRUCTFIELD        0xC000

#define PINTYPE_HI                   4097 //ID10
#define PINTYPE_LOW                  4098 //ID11
#define PINTYPE_K                    4099 //ID12
#define PINTYPE_L                    5000 //ID13
#define PINTYPE_TX                   5001 //ID14
#define PINTYPE_TXMINUS              5002 //ID15
#define PINTYPE_RX                   5003 //ID16
#define PINTYPE_RXMINUS              5004 //ID17

#define CAN_FW_TYPE					 0x10  //CAN流控制帧类型
#define CAN_MF_TYPE					 0x20  //CAN多帧类型		
#define CAN_FW_ReplyValue			 0x30  //CAN流控制帧固定回复值
#define CAN_FW_Weights				 0xFF  //CAN流控制权重位固定值

#define CAN_FW_PROCESS_TYPE			 0x30
#define CAN_FW_TYPE_OVFLW            0x32//CAN流控制帧溢出
#define CAN_FW_TYPE_WAIT             0x31//等流控制帧

typedef struct
{
	UNUM32 CAN_HI;//Differential Line - High (e.g. DW_CAN High)
	UNUM32 CAN_LOW;//Differential Line - Low (e.g. DW_CAN Low)
	UNUM32 K_Line;//UART K-Line (e.g. KWP2000)
	UNUM32 L_Line;//UART L-Line (e.g. ISO 9141-2)
	UNUM32 UART_TX;//UART uni-directional transmit (e.g. SAE J2190)
	UNUM32 UART_RX;//UART uni-directional receive (e.g. SAE J2190)
	UNUM32 J1850_PLUS;//SAE J1850 Plus (e.g. SAE J1850 VPW and SAE J1850 PWM)
	UNUM32 J1850_MINUS;//SAE J1850 Minus (e.g. SAE J1850 PWM)
	UNUM32 TX_SINGLE;//Single wire [e.g. SW_CAN, and UART bi-directional transmit/receive (e.g.SAE J2740)]
	UNUM32 DLC_PROGV;//Pin to set the programmable voltage on DLC
	UNUM32 IGNITION_CLAMP;//Pin to read the ignition sense state from DLC
}PINTYPE_ITEM;


/*
ID: 0
EID: ID60
SHORT_NAME: CP_CanTransmissionTime
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: APPLICATION
DESCRIPTION:
If the timeout values are used which have been received by the ECU via session control response (0x50), the CAN transmission time has to be added to the timeout values.
P2 = received P2 + CanTransmissionTime (contains delay for both transmission directions).
*/
#define COMPARAM_CP_CanTransmissionTime                   0

/*
ID: 1
EID: ID61
SHORT_NAME: CP_CyclicRespTimeout
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: APPLICATION
DESCRIPTION:
This ComParam is used for ComPrimitives that have a NumRecvCycles set to IS-CYCLIC (-1, infinite). The timer is enabled after the first positive response is received from an ECU. If CP_CyclicRespTimeout = 0, there is no receive timing enabled for the infinite receive ComPrimitive.
*/
#define COMPARAM_CP_CyclicRespTimeout                     1

/*
ID: 2
EID: ID62
SHORT_NAME: CP_EnablePerformanceTest
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: APPLICATION
DESCRIPTION:
This ComParam (when enabled) will place the tester into a performance measurement mode. Measurements will be collected during a normal ComPrimitive communications session. ComParams such as P1Min, P2Min, Br, Cs will be tested in this mode. Once the testing is disabled, results of the testing will be returned to the client application.
*/
#define COMPARAM_CP_EnablePerformanceTest                 2

/*
ID: 3
EID: ID63
SHORT_NAME: CP_J1939Name
DATA_TYPE: PDU_PT_BYTEFIELD
DEFAULT_VALUE: 8 0
CLASS: UNIQUE_ID
LAYER: APPLICATION
DESCRIPTION:
Name field from J1939 document. This ComParam will contain the NAME of the Tester. The tester will require this to make an address claim.
*/
#define COMPARAM_CP_J1939Name                             3

/*
ID: 4
EID: ID64
SHORT_NAME: CP_J1939Name_Ecu
DATA_TYPE: PDU_PT_BYTEFIELD
DEFAULT_VALUE: 8 0
CLASS: UNIQUE_ID
LAYER: APPLICATION
DESCRIPTION:
Name field from J1939 document. This ComParam will contain an ECU NAME. By keeping a list of Names and Addresses of all ECUs on the bus (Network Management), the tester will find the Name corresponding to the source address of each incoming vehicle bus messages, and attempt to find a match in the URID table. (This ComParam is used in the URID table).
*/
#define COMPARAM_CP_J1939Name_Ecu                         4

/*
ID: 5
EID: ID65
SHORT_NAME: CP_J1939PreferredAddress
DATA_TYPE: PDU_PT_BYTEFIELD
DEFAULT_VALUE: 8 0
CLASS: UNIQUE_ID
LAYER: APPLICATION
DESCRIPTION:
List of preferred addresses for the Tester. This ComParam is a list of source addresses for the MVCI Protocol Module. The first source address claimed by the MVCI Prtocol Module remains claimed  unless a higher-priority node on the bus requests the same adddress, at which time the tester will have to try to claim the next address in the list. An address remains claimed until the end of the ComLogicalLink communication. A PDU_COPT_STARTCOMM ComPrimitive will try to claim one of the source addresses in this byte field. Since the MVCI Protocol module might not be able to claim the first address requested, a list of tester addresses are supplied.
*/
#define COMPARAM_CP_J1939PreferredAddress                 5

/*
ID: 6
EID: ID66
SHORT_NAME: CP_J1939PreferredAddress_Ecu
DATA_TYPE: PDU_PT_BYTEFIELD
DEFAULT_VALUE: 2 0
CLASS: UNIQUE_ID
LAYER: APPLICATION
DESCRIPTION:
List of preferred addresses for the ECU. This ComParam is a list of source addresses that the ECU would like to acquire on the J1939 bus. The PDU API will use one of the addresses from this list as the destination address for requests to this ECU. The exact source address of the ECU may have to be resolved by forcing a NAME claim and determining the exact source address via CP_J1939Name_Ecu. It will be required for ECU specification and will also be used by the tester if the tester fails to force an address claim sequence on the bus.  If forcing the ECU to claim their addresses is successful, then the D-PDU API will have to use CP_J1939Name_Ecu to resolve the source address which should be one of the addresses in this list. If for some reason the ECU does not respond to the forced address claim, then the first ECU address in this list should be used. (This ComParam is used in the URID table).
*/
#define COMPARAM_CP_J1939PreferredAddress_Ecu             6

/*
ID: 7
EID: ID67
SHORT_NAME: CP_ModifyTiming
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: APPLICATION
DESCRIPTION:
This parameter signals the D-PDU API to automatically modify timing parameters based on a response from the ECU. For ISO 14230-2 this would apply to service 0x83/0xC3 with TPI of 1, 2 or 3. For ISO 15765-3 this would apply to service 0x10/0x50.  For functional addressing mode, the worst case timing parameter returned by the responding ECUs shall be used.
Based on the protocol, the following parameters are modified when a positive ECU response is received:
 - CP_P2Max
 - CP_P2Min
 - CP_P2Star
 - CP_P3Min
 - CP_P4Min
NOTE: The values returned by an ECU are in a different time resolution than the ComParams to be automatically modified. The values will be reinterpreted from the protocol specified time resolution to the 1us resolution specified in the D-PDU API.
*/
#define COMPARAM_CP_ModifyTiming                          7

/*
ID: 8
EID: ID68
SHORT_NAME: CP_P2Max
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: APPLICATION
DESCRIPTION:
Timeout in receiving an expected frame after a successful transmit is complete. Also used for multiple ECU responses.
*/
#define COMPARAM_CP_P2Max                                 8

/*
ID: 9
EID: ID69
SHORT_NAME: CP_P2Max_Ecu
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: APPLICATION
DESCRIPTION:
Performance requirement for the server to start with the response message after the reception of a request message (indicated via N_PDU_ISO15765_2.ind). This is a performance requirement ComParam.
(CP_P2Max_ECU lower than CP_P2Max - CP_CanTransmissionTime)
NOTE: For ISO_14229_5 this is the complete time, including the network transmission time
*/
#define COMPARAM_CP_P2Max_Ecu                             9

/*
ID: 10
EID: ID610
SHORT_NAME: CP_P2Min
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: APPLICATION
DESCRIPTION:
This sets the minimum time between tester request and ECU responses, or two ECU responses. After the request, the interface shall be capable of handling an immediate response (P2_min=0). For subsequent responses, a byte received after P1_MAX shall be considered as the start of the subsequent response. This is a performance requirement ComParam.
*/
#define COMPARAM_CP_P2Min                                 10

/*
ID: 11
EID: ID611
SHORT_NAME: CP_P2Star
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: APPLICATION
DESCRIPTION:
Timeout for the client to expect the start of the response message after the reception of a negative response message (indicated via N_PDU_ISO15765_2.ind) with response code 0x78 (enhanced response timing). This parameter is used for all protocols that support the negative response code 0x78. For some protocols it is used instead of the recommended P3Max parameter.
*/
#define COMPARAM_CP_P2Star                                11

/*
ID: 12
EID: ID612
SHORT_NAME: CP_P2Star_Ecu
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: APPLICATION
DESCRIPTION:
Performance requirement for the server to start with the response message after the transmission of a negative response message (indicated via N_PDU_ISO15765_2.con) with response code 0x78 (enhanced response timing). This is a performance requirement ComParam.
*/
#define COMPARAM_CP_P2Star_Ecu                            12

/*
ID: 13
EID: ID613
SHORT_NAME: CP_P3Func
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: APPLICATION
DESCRIPTION:
Minimum time for the client to wait, after the successful transmission of a functionally addressed request message (indicated via N_PDU_ISO15765_2.con), before it can transmit the next functionally addressed request message, in case no response is required, or the requested data is only supported by a subset of the functionally addressed servers.
*/
#define COMPARAM_CP_P3Func                                13

/*
ID: 14
EID: ID614
SHORT_NAME: CP_P3Max_Ecu
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: APPLICATION
DESCRIPTION:
Time between end of ECU responses and start of new tester request.
*/
#define COMPARAM_CP_P3Max_Ecu                             14

/*
ID: 15
EID: ID615
SHORT_NAME: CP_P3Min
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: APPLICATION
DESCRIPTION:
Minimum time between end of non-negative ECU responses and start of new request. The interface will accept all responses up to CP_P3Min time. The interface will allow transmission of a request any time after CP_P3Min.
*/
#define COMPARAM_CP_P3Min                                 15

/*
ID: 16
EID: ID616
SHORT_NAME: CP_P3Phys
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: APPLICATION
DESCRIPTION:
Minimum time for the client to wait after the successful transmission of a physically addressed request message (indicated via N_PDU_ISO15765_2.con) with no response required before it can transmit the next physically-addressed request message.
*/
#define COMPARAM_CP_P3Phys                                16

/*
ID: 17
EID: ID617
SHORT_NAME: CP_RC21CompletionTimeout
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: ERRHDL
LAYER: APPLICATION
DESCRIPTION:
Time period the tester accepts repeated negative responses with response code 0x21 and repeats the same request. Timer is started after reception of first negative response.
*/
#define COMPARAM_CP_RC21CompletionTimeout                 17

/*
ID: 18
EID: ID618
SHORT_NAME: CP_RC21Handling
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: ERRHDL
LAYER: APPLICATION
DESCRIPTION:
Repetition mode in case of response code 0x7F XX 0x21.
*/
#define COMPARAM_CP_RC21Handling                          18

/*
ID: 19
EID: ID619
SHORT_NAME: CP_RC21RequestTime
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: ERRHDL
LAYER: APPLICATION
DESCRIPTION:
Time between negative response with response code 0x21 and the retransmission of the same request. If CP_P3Min is greater than CP_RC21RequestTime, the time delay prior to the retransmission of the same request will be CP_P3Min.
*/
#define COMPARAM_CP_RC21RequestTime                       19

/*
ID: 20
EID: ID620
SHORT_NAME: CP_RC23CompletionTimeout
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: ERRHDL
LAYER: APPLICATION
DESCRIPTION:
Time period the tester accepts repeated negative responses with response code 0x23 and repeats the same request.
*/
#define COMPARAM_CP_RC23CompletionTimeout                 20

/*
ID: 21
EID: ID621
SHORT_NAME: CP_RC23Handling
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: ERRHDL
LAYER: APPLICATION
DESCRIPTION:
Repetition mode in case of response code 0x7F XX 0x23.
*/
#define COMPARAM_CP_RC23Handling                          21

/*
ID: 22
EID: ID622
SHORT_NAME: CP_RC23RequestTime
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: ERRHDL
LAYER: APPLICATION
DESCRIPTION:
The time the D-PDU API waits to re-request the message when receiving a negative response code 0x23. For some protocols (SAE_J1850_VPW) it is possible to get a positive response after receiving a negative response code 0x23, so the D-PDU API uses this ComParam as the time to receive a possible positive response before making the re-request. On a positive response within this time, the re-request is cancelled. The D-PDU API postpones the re-request until the timeout of CP_RC23RequestTime (or a CP_P3Min timeout, in case CP_P3Min is greater than CP_RC23RequestTime).  For ISO14230-3, there will be no positive response following a RC23 therefore the D-PDU API is expected to always make a re-request if enabled (CP_RC23Handling != 0).
The cycle of receiving negative response code 0x23 and retransmitting the request continues until CP_RC23CompletionTimeout expires (applicable only if CP_RC23Handling is set to 1).
*/
#define COMPARAM_CP_RC23RequestTime                       22

/*
ID: 23
EID: ID623
SHORT_NAME: CP_RC78CompletionTimeout
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: ERRHDL
LAYER: APPLICATION
DESCRIPTION:
Time period the tester accepts repeated negative responses with response code 0x78 and waits for a positive response further on.
*/
#define COMPARAM_CP_RC78CompletionTimeout                 23

/*
ID: 24
EID: ID624
SHORT_NAME: CP_RC78Handling
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: ERRHDL
LAYER: APPLICATION
DESCRIPTION:
Handling of 0x7F XX 0x78 ResponseTimeout and 0x78 Repetitions. The receive timeout value will be CP_P2Star. This timer will be reset on each consecutive reception of the 0x7F 0x78 response.
*/
#define COMPARAM_CP_RC78Handling                          24

/*
ID: 25
EID: ID625
SHORT_NAME: CP_RCByteOffset
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: ERRHDL
LAYER: APPLICATION
DESCRIPTION:
This parameter is used by the MVCI Protocol Handlers to offset into the received negative response message (0x7F) to retrieve the response code byte. Most protocols as a default place the response code as the last byte of the message. There are some protocols which place the response code after the Service Id (offset = 1). A range is provided to allow for different negative response configurations.
*/
#define COMPARAM_CP_RCByteOffset                          25

/*
ID: 26
EID: ID626
SHORT_NAME: CP_RepeatReqCountApp
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: ERRHDL
LAYER: APPLICATION
DESCRIPTION:
This ComParam contains a counter to enable a re-transmission of the last request when either a transmit, receive error, or timeout with no response is detected. This only applies to the application layer.
*/
#define COMPARAM_CP_RepeatReqCountApp                     26

/*
ID: 27
EID: ID627
SHORT_NAME: CP_SessionTiming_ECU
DATA_TYPE: PDU_PT_STRUCTFIELD
DEFAULT_VALUE: 1 255 0
CLASS: TIMING
LAYER: APPLICATION
DESCRIPTION:
Timing parameters to be used by different sessions for the ECU protocol application layer ISO15765_3 in response to a service 0x10 (Set Diagnostic Session). The ECU returns the session timing information on a positive response to the service. If CP_ModifyTiming is enabled, the MVCI protocol module will interpret the positive response from the ECU and set the appropriate timing ComParams.
*/
#define COMPARAM_CP_SessionTiming_ECU                     27

/*
ID: 28
EID: ID628
SHORT_NAME: CP_SessionTimingOverride
DATA_TYPE: PDU_PT_STRUCTFIELD
DEFAULT_VALUE: 1 255 0
CLASS: TIMING
LAYER: APPLICATION
DESCRIPTION:
This parameter signals the D-PDU API to override the response from any ECUs to a Session Timing request. The timing parameters are to be used for the ECU protocol application layer ISO15765_3 in response to a service 0x10 (Set Diagnostic Session). The ECU returns the session timing information on a positive response to the service. If CP_ModifyTiming is enabled and CP_SessionTimingOverride is not empty (ParamActEntries != 0), then the MVCI protocol module will use data in this ComParam instead of the data returned in a positive reponse fom the ECUs.
*/
#define COMPARAM_CP_SessionTimingOverride                 28

/*
ID: 29
EID: ID629
SHORT_NAME: CP_StartMsgIndEnable
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: APPLICATION
DESCRIPTION:
Start Message Indication Enable. Upon receiving a first frame of a multi-frame message (ISO15765), or upon receiving a first byte of a UART message, an indication will be set in the RX result item.  No data bytes will accompany the result item.
*/
#define COMPARAM_CP_StartMsgIndEnable                     29

/*
ID: 30
EID: ID630
SHORT_NAME: CP_SuspendQueueOnError
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: ERRHDL
LAYER: APPLICATION
DESCRIPTION:
This ComParam is used as a temporary ComParam, for services that require a positive response before any further Com Primitives can be executed.
*/
#define COMPARAM_CP_SuspendQueueOnError                   30

/*
ID: 31
EID: ID631
SHORT_NAME: CP_SwCan_HighVoltage
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: APPLICATION
DESCRIPTION:
Indicates that the Single Wire CAN message should be transmitted as a High-Voltage Message. Simultaneously transmitting in high voltage and high speed mode will result in undefined behaviour. This ComParam is only applicable when the Bus Type selected is SAE_J2411_SWCAN.
Note: This ComParam has no effect with the Softing D-PDU API!
*/
#define COMPARAM_CP_SwCan_HighVoltage                     31

/*
ID: 32
EID: ID632
SHORT_NAME: CP_TesterPresentHandling
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TESTER_PRESENT
LAYER: APPLICATION
DESCRIPTION:
Define Tester Present message generation settings. The ComLogicaLink must be in the state PDU_CLLST_COMM_STARTED to enable tester present message handling.
NOTE 1: If the CLL is in the PDU_CLLST_COMM_STARTED state and tester present handling is enabled any changes to one of the tester present ComParms will cause the tester present message to be sent immediately, prior to the initial tester present cyclic time.
NOTE 2:   Protocol handler must always wait the proper P3Min time before allowing any transmit.
*/
#define COMPARAM_CP_TesterPresentHandling                 32

/*
ID: 33
EID: ID633
SHORT_NAME: CP_TesterPresentReqRsp
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TESTER_PRESENT
LAYER: APPLICATION
DESCRIPTION:
Define settings for handling Tester Present ECU responses.
NOTE 1: If the CLL is in the PDU_CLLST_COMM_STARTED state and tester present handling is enabled any changes to one of the tester present ComParms will cause the tester present message to be sent immediately, prior to the initial tester present cyclic time.
NOTE 2:   Protocol handler must always wait the proper P3Min time before allowing any transmit.
*/
#define COMPARAM_CP_TesterPresentReqRsp                   33

/*
ID: 34
EID: ID634
SHORT_NAME: CP_TesterPresentSendType
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TESTER_PRESENT
LAYER: APPLICATION
DESCRIPTION:
Define settings for the type of tester present transmits.
NOTE 1: If the CLL is in the PDU_CLLST_COMM_STARTED state and tester present handling is enabled any changes to one of the tester present ComParms will cause the tester present message to be sent immediately, prior to the initial tester present cyclic time.
NOTE 2: Protocol handler must always wait the proper P3Min time before allowing any transmit.
*/
#define COMPARAM_CP_TesterPresentSendType                 34

/*
ID: 35
EID: ID635
SHORT_NAME: CP_TesterPresentAddrMode
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TESTER_PRESENT
LAYER: APPLICATION
DESCRIPTION:
Addressing Mode to be used for periodic Tester Present messages.
Uses the PhysReqxxx or FuncReqxxx ComParams.
NOTE 1: If the CLL is in the PDU_CLLST_COMM_STARTED state and tester present handling is enabled any changes to one of the tester present ComParms will cause the tester present message to be sent immediately, prior to the initial tester present cyclic time.
NOTE 2: Protocol handler must always wait the proper P3Min time before allowing any transmit.
*/
#define COMPARAM_CP_TesterPresentAddrMode                 35

/*
ID: 36
EID: ID636
SHORT_NAME: CP_TesterPresentTime
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TESTER_PRESENT
LAYER: APPLICATION
DESCRIPTION:
Time between Tester Present messages, or Time bus must be idle before transmitting a Tester Present Message.
NOTE 1:   If the CLL is in the PDU_CLLST_COMM_STARTED state and tester present handling is enabled any changes to one of the tester present ComParms will cause the tester present message to be sent immediately, prior to the initial tester present cyclic time.
NOTE 2: Protocol handler must always wait the proper P3Min time before allowing any transmit.
*/
#define COMPARAM_CP_TesterPresentTime                     36

/*
ID: 37
EID: ID637
SHORT_NAME: CP_TesterPresentMessage
DATA_TYPE: PDU_PT_BYTEFIELD
DEFAULT_VALUE: 12 0
CLASS: TESTER_PRESENT
LAYER: APPLICATION
DESCRIPTION:
Define the Tester Present Message. This message data does not include any header bytes or checksum information.
NOTE 1:   If the CLL is in the PDU_CLLST_COMM_STARTED state and tester present handling is enabled any changes to one of the tester present ComParms will cause the tester present message to be sent immediately, prior to the initial tester present cyclic time.
NOTE 2: Protocol handler must always wait the proper P3Min time before allowing any transmit.
*/
#define COMPARAM_CP_TesterPresentMessage                  37

/*
ID: 38
EID: ID638
SHORT_NAME: CP_TesterPresentExpPosResp
DATA_TYPE: PDU_PT_BYTEFIELD
DEFAULT_VALUE: 12 0
CLASS: TESTER_PRESENT
LAYER: APPLICATION
DESCRIPTION:
Define the expected ECU positive response to a Tester Present Message. This is only applicable if CP_TesterPresentReqRsp is set to 1 (ECU responses are expected on a Tester Present Message). No header bytes or checksum bytes are included. Only the ParamActLen bytes in the array will be compared to the received ECU data.
NOTE 1: If the CLL is in the PDU_CLLST_COMM_STARTED state and tester present handling is enabled any changes to one of the tester present ComParms will cause the tester present message to be sent immediately, prior to the initial tester present cyclic time.
NOTE 2: Protocol handler must always wait the proper P3Min time before allowing any transmit.
*/
#define COMPARAM_CP_TesterPresentExpPosResp               38

/*
ID: 39
EID: ID639
SHORT_NAME: CP_TesterPresentExpNegResp
DATA_TYPE: PDU_PT_BYTEFIELD
DEFAULT_VALUE: 12 0
CLASS: TESTER_PRESENT
LAYER: APPLICATION
DESCRIPTION:
Define the expected ECU negative response to a Tester Present Message. This is only applicable if CP_TesterPresentReqRsp is set to 1 (ECU responses are expected on a Tester Present Message). No header bytes or checksum bytes are included.
When a negative response is received to a tester present message, which cannot be handled by the MVCI Protocol module, the MVCI Protocol Module should report a Tester Present Error, but continue sending Tester Present Messages.
NOTE 1: If the CLL is in the PDU_CLLST_COMM_STARTED state and tester present handling is enabled any changes to one of the tester present ComParms will cause the tester present message to be sent immediately, prior to the initial tester present cyclic time.
NOTE 2: Protocol handler must always wait the proper P3Min time before allowing any transmit.
*/
#define COMPARAM_CP_TesterPresentExpNegResp               39

/*
ID: 40
EID: ID640
SHORT_NAME: CP_TesterPresentTime_Ecu
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TESTER_PRESENT
LAYER: APPLICATION
DESCRIPTION:
Time for the server to keep a diagnostic session (other than the default session) active while not receiving any diagnostic request message.
*/
#define COMPARAM_CP_TesterPresentTime_Ecu                 40

/*
ID: 41
EID: ID641
SHORT_NAME: CP_TransmitIndEnable
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: APPLICATION
DESCRIPTION:
Transmit Indication Enable. On completion of a transmit message by the protocol, an indication will be set in the VPW_RX_FLAG result item. No data bytes will accompany the result item.
NOTE: For ISO_14229_5, Transmit Indication is triggered by DoIP Acknowledge
*/
#define COMPARAM_CP_TransmitIndEnable                     41

/*
ID: 42
EID: ID642
SHORT_NAME: CP_5BaudAddressFunc
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
Value of 5Baud Address in case of functional-addressed communication.
The correct baud rate address type (functional/physical) is selected during execution of a Start Communication Com Primitive based on the setting of the CP_RequestAddrMode ComParam.
*/
#define COMPARAM_CP_5BaudAddressFunc                      42

/*
ID: 43
EID: ID643
SHORT_NAME: CP_5BaudAddressPhys
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
Value of 5Baud Address in case of physical-addressed communication.
The correct baud rate address type (functional/physical) is selected during execution of a Start Communication Com Primitive based on the setting of the CP_RequestAddrMode ComParam.
*/
#define COMPARAM_CP_5BaudAddressPhys                      43

/*
ID: 44
EID: ID644
SHORT_NAME: CP_5BaudMode
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: INIT
LAYER: TRANSPORT
DESCRIPTION:
Type of 5 Baud initialization. This ComParam allows either ISO9141 initialization sequence, ISO9141-2/ISO14230 initialization sequence, or hybrid versions, which include only one of the extra bytes defined for ISO9141-2 and ISO14230.
(Initialization for ISO9141-2 and ISO14230 include the init sequence as defined in ISO9141 plus inverted key byte 2 sent from the interface to the ECU and the inverted address sent from the ECU to the interface).
*/
#define COMPARAM_CP_5BaudMode                             44

/*
ID: 45
EID: ID645
SHORT_NAME: CP_AccessTiming_Ecu
DATA_TYPE: PDU_PT_STRUCTFIELD
DEFAULT_VALUE: 2 8 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Timing parameters to be sent/used in response to a Service Id 0x83 (Access Timing Service) with TPI 1, 2 or 3. For a TPI of 1 (set default values), the ECU will set the timing parameters to the default values specified by ISO 14230-2. For a TPI of 2 (read active values), the ECU will return the active timing parameters in the response message. For a TPI of 3 (set parameters) the MVCI Protocol Module will set the timing parameters to the values to be used by the ECU. This ComParam allows the ECU to define sets of timing parameters to be used for normal and extended timing, as well as override timing values defined by a Tester.
*/
#define COMPARAM_CP_AccessTiming_Ecu                      45

/*
ID: 46
EID: ID646
SHORT_NAME: CP_AccessTimingOverride
DATA_TYPE: PDU_PT_STRUCTFIELD
DEFAULT_VALUE: 2 8 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
This ComParam along with CP_ModifyTiming ComParam signals the D-PDU API to override the response from any ECUs to an Access Timing request. The timing parameters are to be used in response to a Service Id 0x83 (Access Timing Service) with TPI 1, 2 or 3. For a TPI of 1 (set default values), the ECU will set the timing parameters to the default values specified by ISO 14230-2. For a TPI of 2 (read active values), the ECU will return the timing parameters in the response message. For a TPI of 3 (set parameters) the MVCI Protocol Module will set the timing parameters to the values to be used by the ECU. If CP_ModifyTiming is enabled and CP_AccessTimingOverride is not empty (ParamActEntries != 0), then the MVCI protocol module will use data in this ComParam instead of the data returned in a positive reponse from the ECU for TPI of 2.
*/
#define COMPARAM_CP_AccessTimingOverride                  46

/*
ID: 47
EID: ID647
SHORT_NAME: CP_ExtendedTiming
DATA_TYPE: PDU_PT_STRUCTFIELD
DEFAULT_VALUE: 2 1 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
This ComParam is used to define extended timing values for K-Line protocols. The values are used after the keybytes are received from the ECU during the initialialization sequence. If CP_ExtendedTiming is not empty (ParamActEntries != 0), then the MVCI protocol module will use data in this ComParam otherwise the MVCI protocol module will use the default extended values defined in ISO14230-2. For normal timing the MVCI protocol module uses timing defined in the ComParams: CP_P2Max, CP_P3Min, etc.
*/
#define COMPARAM_CP_ExtendedTiming                        47

/*
ID: 48
EID: ID648
SHORT_NAME: CP_Ar
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Time for transmission of the CAN frame (any N_PDU) on the receiver side.
*/
#define COMPARAM_CP_Ar                                    48

/*
ID: 49
EID: ID649
SHORT_NAME: CP_Ar_Ecu
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Time for transmission of the CAN frame (any N_PDU) on the receiver side.
*/
#define COMPARAM_CP_Ar_Ecu                                49

/*
ID: 50
EID: ID650
SHORT_NAME: CP_As
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Time for transmission of the CAN frame (any N_PDU) on the sender side.
*/
#define COMPARAM_CP_As                                    50

/*
ID: 51
EID: ID651
SHORT_NAME: CP_As_Ecu
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Time for transmission of the CAN frame (any N_PDU) on the sender side.
*/
#define COMPARAM_CP_As_Ecu                                51

/*
ID: 52
EID: ID652
SHORT_NAME: CP_BlockSize
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
This sets the block size that the interface should report to the vehicle for receiving segmented transfers in a Transmit Flow Control Message.
*/
#define COMPARAM_CP_BlockSize                             52

/*
ID: 53
EID: ID653
SHORT_NAME: CP_BlockSize_Ecu
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
This sets the block size that the ECU should report to the tester for receiving segmented transfers in a Transmit Flow Control Message.
*/
#define COMPARAM_CP_BlockSize_Ecu                         53

/*
ID: 54
EID: ID654
SHORT_NAME: CP_BlockSizeOverride
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
This sets the block size that the interface should use to send segmented messages to the vehicle. The flow control value reported by the vehicle should be ignored.
*/
#define COMPARAM_CP_BlockSizeOverride                     54

/*
ID: 55
EID: ID655
SHORT_NAME: CP_Br
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Time until transmission of the next FlowControl. This is equivalent to Th in J1939-21.
For ISO 15765-2 and 15765-4, this value is a performance requirement ComParam and should not be used as a timeout value by the tester.
*/
#define COMPARAM_CP_Br                                    55

/*
ID: 56
EID: ID656
SHORT_NAME: CP_Br_Ecu
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Time until transmission of the next FlowControl. This is a performance requirement ComParam.
*/
#define COMPARAM_CP_Br_Ecu                                56

/*
ID: 57
EID: ID657
SHORT_NAME: CP_Bs
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Timeout until reception of the next FlowControl. This is equivalent to T4 in J1939-21.
*/
#define COMPARAM_CP_Bs                                    57

/*
ID: 58
EID: ID658
SHORT_NAME: CP_Bs_Ecu
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Timeout until reception of the next FlowControl.
*/
#define COMPARAM_CP_Bs_Ecu                                58

/*
ID: 59
EID: ID659
SHORT_NAME: CP_CanDataSizeOffset
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
Offset subtracted from the total number of expected bytes received/transmitted in a first frame message.
*/
#define COMPARAM_CP_CanDataSizeOffset                     59

/*
ID: 60
EID: ID660
SHORT_NAME: CP_FillerByte
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
Padding data byte to be used to pad all USDT type transmits frames (SF, FC, and last CF).
*/
#define COMPARAM_CP_FillerByte                            60

/*
ID: 61
EID: ID661
SHORT_NAME: CP_FillerByteHandling
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
Enable Padding, forcing the DLC of a CAN frame to always be 8.
*/
#define COMPARAM_CP_FillerByteHandling                    61

/*
ID: 62
EID: ID662
SHORT_NAME: CP_FillerByteLength
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
Length to pad the data portion of the message for ISO 14230.
*/
#define COMPARAM_CP_FillerByteLength                      62

/*
ID: 63
EID: ID663
SHORT_NAME: CP_CanFillerByte
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
Padding data byte to be used to pad all USDT type transmits frames (SF, FC, and last CF).
*/
#define COMPARAM_CP_CanFillerByte                         63

/*
ID: 64
EID: ID664
SHORT_NAME: CP_CanFillerByteHandling
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
Enable Padding, forcing the DLC of a CAN frame to always be 8.
*/
#define COMPARAM_CP_CanFillerByteHandling                 64

/*
ID: 65
EID: ID665
SHORT_NAME: CP_CanFirstConsecutiveFrameValue
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
First consecutive frame number to be transmitted/received on a multi-segment transfer. Used to override the normal first consecutive frame value of 1.
*/
#define COMPARAM_CP_CanFirstConsecutiveFrameValue         65

/*
ID: 66
EID: ID666
SHORT_NAME: CP_CanFuncReqExtAddr
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
Address extension for enhanced diagnostics. The first byte of the requested CAN frame data contains the N_AE/N_TA byte followed by the correct number of PCI bytes. This ComParam is used for all transmitted CAN Frames that have the CAN Address Extension bit set in the CanIdFormat.
*/
#define COMPARAM_CP_CanFuncReqExtAddr                     66

/*
ID: 67
EID: ID667
SHORT_NAME: CP_CanFuncReqFormat
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
CAN Format used for a functional address transmit.
*/
#define COMPARAM_CP_CanFuncReqFormat                      67

/*
ID: 68
EID: ID668
SHORT_NAME: CP_CanFuncReqId
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
CAN ID used for a functional address transmit.
*/
#define COMPARAM_CP_CanFuncReqId                          68

/*
ID: 69
EID: ID669
SHORT_NAME: CP_CanMaxNumWaitFrames
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
The maximum number of WAIT flow control frames allowed during a multi-segment transfer. For J1939, this is the maximum number of allowed CTS frames.
*/
#define COMPARAM_CP_CanMaxNumWaitFrames                   69

/*
ID: 70
EID: ID670
SHORT_NAME: CP_CanPhysReqExtAddr
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: UNIQUE_ID
LAYER: TRANSPORT
DESCRIPTION:
Address extension for enhanced diagnostics. The first byte of the requested CAN frame data contains the N_AE/N_TA byte followed by the correct number of PCI bytes. This ComParam is used for all transmitted CAN Frames that have the CAN Address Extension bit set in the CanIdFormat.
*/
#define COMPARAM_CP_CanPhysReqExtAddr                     70

/*
ID: 71
EID: ID671
SHORT_NAME: CP_CanPhysReqFormat
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: UNIQUE_ID
LAYER: TRANSPORT
DESCRIPTION:
CAN Format used for a physical address transmit.
*/
#define COMPARAM_CP_CanPhysReqFormat                      71

/*
ID: 72
EID: ID672
SHORT_NAME: CP_CanPhysReqId
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: UNIQUE_ID
LAYER: TRANSPORT
DESCRIPTION:
CAN ID used for a physical address transmit.
*/
#define COMPARAM_CP_CanPhysReqId                          72

/*
ID: 73
EID: ID673
SHORT_NAME: CP_CanRespUSDTExtAddr
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: UNIQUE_ID
LAYER: TRANSPORT
DESCRIPTION:
Extended Address used for a USDT response from an ECU if the CAN Format indicates address extension.
*/
#define COMPARAM_CP_CanRespUSDTExtAddr                    73

/*
ID: 74
EID: ID674
SHORT_NAME: CP_CanRespUSDTFormat
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: UNIQUE_ID
LAYER: TRANSPORT
DESCRIPTION:
: CAN Format for the USDT CAN ID received from an ECU (Segment type Bit must = 1).
*/
#define COMPARAM_CP_CanRespUSDTFormat                     74

/*
ID: 75
EID: ID675
SHORT_NAME: CP_CanRespUSDTId
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: UNIQUE_ID
LAYER: TRANSPORT
DESCRIPTION:
Received USDT CAN ID from an ECU.
*/
#define COMPARAM_CP_CanRespUSDTId                         75

/*
ID: 76
EID: ID676
SHORT_NAME: CP_CanRespUUDTExtAddr
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: UNIQUE_ID
LAYER: TRANSPORT
DESCRIPTION:
Extended Address used for UUDT response if the CAN Format indicates address extension.
*/
#define COMPARAM_CP_CanRespUUDTExtAddr                    76

/*
ID: 77
EID: ID677
SHORT_NAME: CP_CanRespUUDTFormat
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: UNIQUE_ID
LAYER: TRANSPORT
DESCRIPTION:
Received CAN Format for CAN ID without segmentation (Segment Type Bit must = 0).
*/
#define COMPARAM_CP_CanRespUUDTFormat                     77

/*
ID: 78
EID: ID678
SHORT_NAME: CP_CanRespUUDTId
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: UNIQUE_ID
LAYER: TRANSPORT
DESCRIPTION:
Received UUDT CAN ID from an ECU.
*/
#define COMPARAM_CP_CanRespUUDTId                         78

/*
ID: 79
EID: ID679
SHORT_NAME: CP_Cr
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Timeout for reception of the next ConsecutiveFrame. For J1939-21, this is equivalent to T1.
*/
#define COMPARAM_CP_Cr                                    79

/*
ID: 80
EID: ID680
SHORT_NAME: CP_Cr_Ecu
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Timeout for reception of the next ConsecutiveFrame.
*/
#define COMPARAM_CP_Cr_Ecu                                80

/*
ID: 81
EID: ID681
SHORT_NAME: CP_Cs
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Time until transmission of the next Consecutive Frame. (This is used if FC is not enabled or if the STmin value in the FC=0 and STminOverride=0xFFFF). For ISO 15765-2 and 15765-4, this is a performance requirement ComParam and should not be used as a timeout value by the tester.
For J1939, this is equivalent to the maximum time between sending packets in a multi-packet broadcast and multi-packet destination-specific message.
*/
#define COMPARAM_CP_Cs                                    81

/*
ID: 82
EID: ID682
SHORT_NAME: CP_Cs_Ecu
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Time until transmission of the next Consecutive Frame. (This is used if FC is not enabled or if the STmin value in the FC=0 and STminOverride=0xFFFF). This is a performance requirement ComParam.
*/
#define COMPARAM_CP_Cs_Ecu                                82

/*
ID: 83
EID: ID683
SHORT_NAME: CP_EcuRespSourceAddress
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: UNIQUE_ID
LAYER: TRANSPORT
DESCRIPTION:
ECU Source Address response of a non-CAN message. This ComParam is used for response handling only. It is a URID ComParam and is used whether addressing is functional or physical. The protocol handler extracts the ECU address from the response message and uses this information along with other URID ComParams to find a match in the URID table to retrieve the Unique Response Id for the ECU. For physical addressing it is possible that CP_EcuRespSourceAddress equals CP_PhysReqTargetAddr.
*/
#define COMPARAM_CP_EcuRespSourceAddress                  83

/*
ID: 84
EID: ID684
SHORT_NAME: CP_EnableConcatenation
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
This ComParam instructs the application layer to automatically detect multiple responses from a single ECU and construct a single ECU response to the client application. For details please read the corresponding protocol documentation.
*/
#define COMPARAM_CP_EnableConcatenation                   84

/*
ID: 85
EID: ID685
SHORT_NAME: CP_FuncReqFormatPriorityType
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
First Header Byte of a non-CAN message for a functional address transmit. This ComParam is used for proper request message header construction in non-Raw mode.
*/
#define COMPARAM_CP_FuncReqFormatPriorityType             85

/*
ID: 86
EID: ID686
SHORT_NAME: CP_FuncReqTargetAddr
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
Second Header Byte of a non-CAN message for a functional address transmit. This ComParam is used for proper request message header construction in non-Raw mode.
*/
#define COMPARAM_CP_FuncReqTargetAddr                     86

/*
ID: 87
EID: ID687
SHORT_NAME: CP_FuncRespFormatPriorityType
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: UNIQUE_ID
LAYER: TRANSPORT
DESCRIPTION:
First Header Byte of a non-CAN message received from the ECU for functional addressing. This ComParam is used for response handling only. It is a URID ComParam and is used for functional addressing only. The protocol handler extracts the format/priority byte from the response message and uses this information along with other URID ComParams to find a match in the URID table to retrieve the Unique Response Id for the ECU.
*/
#define COMPARAM_CP_FuncRespFormatPriorityType            87

/*
ID: 88
EID: ID688
SHORT_NAME: CP_FuncRespTargetAddr
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: UNIQUE_ID
LAYER: TRANSPORT
DESCRIPTION:
Second Header Byte of a non-CAN message received from the ECU for functional addressing. This ComParam is used for response handling only. It is a URID ComParam and is used for functional addressing only. The protocol handler extracts the Target address from the response message and uses this information along with other URID ComParams to find a match in the URID table to retrieve the Unique Response Id for the ECU.
This information is also used to fill out the functional lookup table for J1850_PWM.
*/
#define COMPARAM_CP_FuncRespTargetAddr                    88

/*
ID: 89
EID: ID689
SHORT_NAME: CP_HeaderFormatJ1850
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
Header Byte configuration to be used for J1850 communication.
This setting is used to properly construct the message header bytes to complete the PDU.
This ComParam is not used if the ComLogicalLink is in RawMode.
Header bytes are constructed following the rules of the protocol specification.
*/
#define COMPARAM_CP_HeaderFormatJ1850                     89

/*
ID: 90
EID: ID690
SHORT_NAME: CP_HeaderFormatKW
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
Header Byte configuration for K-Line protocol (Keyword).
This setting is used to properly construct the message header bytes to complete the PDU.
This ComParam is not used if the ComLogicalLink is in RawMode.
Header bytes are constructed following the rules of the protocol specification. This ComParam can be used to override any keybyte values received from the ECU during initialization.
If the protocol cannot handle the length of a ComPrimitive based on the settings of this ComParam, then an error event, PDU_ERR_EVT_PROT_ERR, is generated and the ComPrimitive is FINISHED.
*/
#define COMPARAM_CP_HeaderFormatKW                        90

/*
ID: 91
EID: ID691
SHORT_NAME: CP_InitializationSettings
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: INIT
LAYER: TRANSPORT
DESCRIPTION:
Set Initialization method.
*/
#define COMPARAM_CP_InitializationSettings                91

/*
ID: 92
EID: ID692
SHORT_NAME: CP_J1939TransportType
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: INIT
LAYER: TRANSPORT
DESCRIPTION:
Transport type for J1939.
*/
#define COMPARAM_CP_J1939TransportType                    92

/*
ID: 93
EID: ID693
SHORT_NAME: CP_MessagePriority
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
Message Priority.
J1939 protocol uses the 3 least significant bits that become part of the CAN ID. This is used only for request messages sent by the tester to the ECU. This parameter is used in bits 26 thru 28 of the CAN ID for the J1939 message.
J1708: The message priority goes into calculating the required idle bus time before transmitting the message.
*/
#define COMPARAM_CP_MessagePriority                       93

/*
ID: 94
EID: ID694
SHORT_NAME: CP_MidReqId
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
Request Message Identifier used in building a transmit message to an ECU for a J1708 protocol only.
*/
#define COMPARAM_CP_MidReqId                              94

/*
ID: 95
EID: ID695
SHORT_NAME: CP_MidRespId
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: UNIQUE_ID
LAYER: TRANSPORT
DESCRIPTION:
Response Message Identifier received from an ECU for a J1708 protocol only.
*/
#define COMPARAM_CP_MidRespId                             95

/*
ID: 96
EID: ID696
SHORT_NAME: CP_P1Max
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Maximum inter-byte time for ECU Responses. Interface must be capable of handling a P1_MIN time of 0 ms.
After the request, the interface shall be capable of handling an immediate response (P2_MIN=0). For subsequent responses, a byte received after P1_MAX shall be considered as the start of the subsequent response.
*/
#define COMPARAM_CP_P1Max                                 96

/*
ID: 97
EID: ID697
SHORT_NAME: CP_P1Min
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
This sets the minimum inter-byte time for the ECU responses. Application shall not get or set this value. Interface must be capable of handling P1_MIN=0. This is a performance requirement ComParam.
*/
#define COMPARAM_CP_P1Min                                 97

/*
ID: 98
EID: ID698
SHORT_NAME: CP_P4Max
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Maximum inter-byte time for a tester request.
*/
#define COMPARAM_CP_P4Max                                 98

/*
ID: 99
EID: ID699
SHORT_NAME: CP_P4Min
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Minimum inter-byte time for tester transmits.
*/
#define COMPARAM_CP_P4Min                                 99

/*
ID: 100
EID: ID6100
SHORT_NAME: CP_PhysReqFormatPriorityType
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
First Header Byte of a non-CAN message for physical address transmit. This ComParam is used for proper request message header construction in non-Raw mode.
*/
#define COMPARAM_CP_PhysReqFormatPriorityType             100

/*
ID: 101
EID: ID6101
SHORT_NAME: CP_PhysReqTargetAddr
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
Physical Target Addressing Information used for correct Message Header Construction. This ComParam is used for proper request message header construction in non-Raw mode. It is possible that CP_PhysReqTargetAddr matches CP_EcuRespSourceAddress in one of the URID table entries.
*/
#define COMPARAM_CP_PhysReqTargetAddr                     101

/*
ID: 102
EID: ID6102
SHORT_NAME: CP_PhysRespFormatPriorityType
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: UNIQUE_ID
LAYER: TRANSPORT
DESCRIPTION:
First Header Byte of a non-CAN message received from the ECU for physical addressing. This ComParam is used for response handling only. It is a URID ComParam and is used for physical addressing only. The protocol handler extracts the format/priority byte from the response message and uses this information along with other URID ComParams to find a match in the URID table to retrieve the Unique Response Id for the ECU.
*/
#define COMPARAM_CP_PhysRespFormatPriorityType            102

/*
ID: 103
EID: ID6103
SHORT_NAME: CP_RepeatReqCountTrans
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: ERRHDL
LAYER: TRANSPORT
DESCRIPTION:
This ComParam contains a counter to enable a re-transmission of the last request when either a transmit, a receive error, or transport layer timeout is detected. This applies to the transport layer only.
NOTE: For ISO_13400_2 this ComParam is used in case of ACK timeout (see CP_DoIPDiagnosticAckTimeout) and communication errors signalled by IP-Stack / operating system
*/
#define COMPARAM_CP_RepeatReqCountTrans                   103

///* 暂时无此参数
//ID: 104
//EID: ID6104
//SHORT_NAME: CP_DoIPTcpErrHandling
//DATA_TYPE: PDU_PT_UNUM32
//DEFAULT_VALUE: 0
//CLASS: ERRHDL
//LAYER: APPLICATION
//DESCRIPTION:
//This ComParam is used to enable/disable the TcpError handling on the DoIP protocols.
//*/
//#define COMPARAM_CP_DoIPTcpErrHandling                    104

/*
ID: 104
EID: ID6104
SHORT_NAME: CP_RequestAddrMode
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
Addressing Mode, physical or functional, to be used for sending a ComPrimitive.
*/
#define COMPARAM_CP_RequestAddrMode                       104

/*
ID: 105
EID: ID6105
SHORT_NAME: CP_SCITransmitMode
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: INIT
LAYER: TRANSPORT
DESCRIPTION:
SCI transmit mode.
*/
#define COMPARAM_CP_SCITransmitMode                       105

/*
ID: 106
EID: ID6106
SHORT_NAME: CP_SendRemoteFrame
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
This ComParam is used for CAN remote frame handling. (No data bytes are transmitted. Just the CAN ID. The first byte of the PDU Data must contain the Data Length Code).
Note: This ComParam has no effect with the Softing D-PDU API!
*/
#define COMPARAM_CP_SendRemoteFrame                       106

/*
ID: 107
EID: ID6107
SHORT_NAME: CP_StMin
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
This sets the separation time the interface should report to the vehicle for receiving segmented transfers in a Transmit Flow Control Message.
*/
#define COMPARAM_CP_StMin                                 107

/*
ID: 108
EID: ID6108
SHORT_NAME: CP_StMin_Ecu
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
The minimum time the sender shall wait between the transmissions of two ConsecutiveFrame N_PDUs.
*/
#define COMPARAM_CP_StMin_Ecu                             108

/*
ID: 109
EID: ID6109
SHORT_NAME: CP_StMinOverride
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
This sets the separation time the interface should use to transmit segmented messages to the vehicle. The flow control value reported by the vehicle will be ignored. Parameter settings between 1 to 450 result in a value of 450 microseconds on the vehicle bus.
*/
#define COMPARAM_CP_StMinOverride                         109

/*
ID: 110
EID: ID6110
SHORT_NAME: CP_T1Max
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
This sets the maximum inter-frame response delay.
*/
#define COMPARAM_CP_T1Max                                 110

/*
ID: 111
EID: ID6111
SHORT_NAME: CP_T2Max
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
This sets the maximum inter-frame request delay.
*/
#define COMPARAM_CP_T2Max                                 111

/*
ID: 112
EID: ID6112
SHORT_NAME: CP_T3Max
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
This sets the maximum response delay from the ECU after processing a valid request message from the interface. For J1939-21, this is equivalent to Tr.
*/
#define COMPARAM_CP_T3Max                                 112

/*
ID: 113
EID: ID6113
SHORT_NAME: CP_T4Max
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
This sets the maximum inter-message response delay.
For J1939, this is equivalent to T3, the maximum time allowed for the Originator to receive a CTS or an ACK after sending a packet.
*/
#define COMPARAM_CP_T4Max                                 113

/*
ID: 114
EID: ID6114
SHORT_NAME: CP_T5Max
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
This sets the maximum inter-message request delay.
For J1939, this is equivalent to T2, the maximum time allowed for the Originator to send a packet after receiving a CTS from the Responder.
*/
#define COMPARAM_CP_T5Max                                 114

/*
ID: 115
EID: ID6115
SHORT_NAME: CP_TesterSourceAddress
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
Source address of transmitted message for non-CAN messages. This ComParam is used for proper request message header construction in non-Raw mode.
This ComParam will also contain the claimed Tester Address for J1939, which can be read by the client application after a successful address claim determined during a PDU_COPT_STARTCOMM ComPrimitive.
*/
#define COMPARAM_CP_TesterSourceAddress                   115

/*
ID: 116
EID: ID6116
SHORT_NAME: CP_TIdle
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: INIT
LAYER: TRANSPORT
DESCRIPTION:
Minimum bus idle time before tester starts the address byte sequence or the fast init sequence. (TIdle replaces W0 and W5.)
*/
#define COMPARAM_CP_TIdle                                 116

/*
ID: 117
EID: ID6117
SHORT_NAME: CP_TInil
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: INIT
LAYER: TRANSPORT
DESCRIPTION:
Sets the duration for the low pulse in a fast initialization sequence.
*/
#define COMPARAM_CP_TInil                                 117

/*
ID: 118
EID: ID6118
SHORT_NAME: CP_TWup
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: INIT
LAYER: TRANSPORT
DESCRIPTION:
Sets total duration of the wakeup pulse (TWUP-TINIL)=high pulse before start communication message.
*/
#define COMPARAM_CP_TWup                                  118

/*
ID: 119
EID: ID6119
SHORT_NAME: CP_W1Max
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: INIT
LAYER: TRANSPORT
DESCRIPTION:
Maximum time from the end of address byte to start of the synchronization pattern from the ECU.
*/
#define COMPARAM_CP_W1Max                                 119

/*
ID: 120
EID: ID6120
SHORT_NAME: CP_W1Min
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: INIT
LAYER: TRANSPORT
DESCRIPTION:
Minimum time from the end of address byte to start of the synchronization pattern from the ECU.
*/
#define COMPARAM_CP_W1Min                                 120

/*
ID: 121
EID: ID6121
SHORT_NAME: CP_W2Max
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: INIT
LAYER: TRANSPORT
DESCRIPTION:
Maximum time from the end of the synchronization pattern to the start of key byte 1.
*/
#define COMPARAM_CP_W2Max                                 121

/*
ID: 122
EID: ID6122
SHORT_NAME: CP_W2Min
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: INIT
LAYER: TRANSPORT
DESCRIPTION:
Minimum time from the end of the synchronization pattern to the start of key byte 1.
*/
#define COMPARAM_CP_W2Min                                 122

/*
ID: 123
EID: ID6123
SHORT_NAME: CP_W3Max
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: INIT
LAYER: TRANSPORT
DESCRIPTION:
Maximum time between key byte 1 and key byte 2.
*/
#define COMPARAM_CP_W3Max                                 123

/*
ID: 124
EID: ID6124
SHORT_NAME: CP_W3Min
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: INIT
LAYER: TRANSPORT
DESCRIPTION:
Minimum time between key byte 1 and key byte 2.
*/
#define COMPARAM_CP_W3Min                                 124

/*
ID: 125
EID: ID6125
SHORT_NAME: CP_W4Max
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: INIT
LAYER: TRANSPORT
DESCRIPTION:
Maximum time between receiving key byte 2 from the vehicle and the inversion being returned by the interface.
*/
#define COMPARAM_CP_W4Max                                 125

/*
ID: 126
EID: ID6126
SHORT_NAME: CP_W4Min
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: INIT
LAYER: TRANSPORT
DESCRIPTION:
Minimum time between receiving key byte 2 from the vehicle and the inversion being returned by the interface.
*/
#define COMPARAM_CP_W4Min                                 126

/*
ID: 127
EID: ID6127
SHORT_NAME: CP_Baudrate
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: BUSTYPE
LAYER: PHYSICAL
DESCRIPTION:
Represents the desired baud rate. If the desired baud rate cannot be achieved within the tolerance of the protocol, the interface will remain at the previous baud rate.
*/
#define COMPARAM_CP_Baudrate                              127

/*
ID: 128
EID: ID6128
SHORT_NAME: CP_BitSamplePoint
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: BUSTYPE
LAYER: PHYSICAL
DESCRIPTION:
This sets the desired bit sample point as a percentage of the bit time.
*/
#define COMPARAM_CP_BitSamplePoint                        128

/*
ID: 129
EID: ID6129
SHORT_NAME: CP_BitSamplePoint_Ecu
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: BUSTYPE
LAYER: PHYSICAL
DESCRIPTION:
This sets the desired bit sample point as a percentage of the bit time.
*/
#define COMPARAM_CP_BitSamplePoint_Ecu                    129

/*
ID: 130
EID: ID6130
SHORT_NAME: CP_CanBaudrateRecord
DATA_TYPE: PDU_PT_LONGFIELD
DEFAULT_VALUE: 12 0
CLASS: BUSTYPE
LAYER: PHYSICAL
DESCRIPTION:
List of baud rates to use during an OBD CAN initialization sequence. For a detailed description of the parameter usage, see chapters - [CAN baudrate detection] and [OBD on CAN Application], in the Documentation_PDU-API_ISO_OBD_on_ISO_15765_4 document.
*/
#define COMPARAM_CP_CanBaudrateRecord                     130

/*
ID: 131
EID: ID6131
SHORT_NAME: CP_ChangeSpeedCtrl
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: APPLICATION
DESCRIPTION:
Control the behavior of the MVCI Protocol Module in processing speed change messages. When this ComParam is enabled, the speed rate change will be activated on a successful Send or SendRecv ComPrimitive when the transmitted or received message matches the CP_ChangeSpeedMessage (baudrate as specified in CP_ChangeSpeedRate and termination resistor as specified in CP_ChangeSpeedResCtrl). In the case of monitoring mode, when a receive PDU is bound to a Receive Only ComPrimitive, and this ComParam is enabled, the speed rate change will also be activated and the corresponding ComParams will be interpreted accordingly.
*/
#define COMPARAM_CP_ChangeSpeedCtrl                       131

/*
ID: 132
EID: ID6132
SHORT_NAME: CP_ChangeSpeedMessage
DATA_TYPE: PDU_PT_BYTEFIELD
DEFAULT_VALUE: 12 0
CLASS: COM
LAYER: APPLICATION
DESCRIPTION:
Switch Speed Message. The message is monitored for transmit and receive. When this message is detected on the vehicle bus, the CP_ChangeSpeedRate and CP_ChangeSpeedResCtrl ComParams are processed.
NOTE: CP_ChangeSpeedCtrl must be enabled for this ComParam to be active.
*/
#define COMPARAM_CP_ChangeSpeedMessage                    132

/*
ID: 133
EID: ID6133
SHORT_NAME: CP_ChangeSpeedRate
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: APPLICATION
DESCRIPTION:
The data rate to be used when switching speed rates. When changed, this value is copied to CP_Baudrate ComParam.
*/
#define COMPARAM_CP_ChangeSpeedRate                       133

/*
ID: 134
EID: ID6134
SHORT_NAME: CP_ChangeSpeedResCtrl
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: APPLICATION
DESCRIPTION:
This ComParam is used in conjunction with CP_ChangeSpeedCtrl. This ComParam is used to control automatic loading or unloading of the physical resource resistor when a change speed message has been transmitted or received.
*/
#define COMPARAM_CP_ChangeSpeedResCtrl                    134

/*
ID: 135
EID: ID6135
SHORT_NAME: CP_ChangeSpeedTxDelay
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: APPLICATION
DESCRIPTION:
Minimum amount of time to wait before allowing the next transmit message on the Vehicle Bus after the successful transmission of a baud rate change message.
*/
#define COMPARAM_CP_ChangeSpeedTxDelay                    135

/*
ID: 136
EID: ID6136
SHORT_NAME: CP_K_L_LineInit
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: BUSTYPE
LAYER: PHYSICAL
DESCRIPTION:
: K and L line usage for ISO9141 and ISO14230 initialization address.
*/
#define COMPARAM_CP_K_L_LineInit                          136

/*
ID: 137
EID: ID6137
SHORT_NAME: CP_K_LinePullup
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: BUSTYPE
LAYER: PHYSICAL
DESCRIPTION:
Control the K-Line voltage to either 12V or 24V.
*/
#define COMPARAM_CP_K_LinePullup                          137

/*
ID: 138
EID: ID6138
SHORT_NAME: CP_ListenOnly
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: BUSTYPE
LAYER: PHYSICAL
DESCRIPTION:
Enable a Listen Only mode on the Com Logical Link. This will cause the link to no longer acknowledge received frames on the CAN Network.
*/
#define COMPARAM_CP_ListenOnly                            138

/*
ID: 139
EID: ID6139
SHORT_NAME: CP_NetworkLine
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: BUSTYPE
LAYER: PHYSICAL
DESCRIPTION:
This sets the network line(s) that are active during communication (for cases where the physical layer allows this).
*/
#define COMPARAM_CP_NetworkLine                           139

/*
ID: 140
EID: ID6140
SHORT_NAME: CP_SamplesPerBit
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: BUSTYPE
LAYER: PHYSICAL
DESCRIPTION:
Number of samples per bit.
*/
#define COMPARAM_CP_SamplesPerBit                         140

/*
ID: 141
EID: ID6141
SHORT_NAME: CP_SamplesPerBit_Ecu
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: BUSTYPE
LAYER: PHYSICAL
DESCRIPTION:
Number of samples per bit for the ECU.
*/
#define COMPARAM_CP_SamplesPerBit_Ecu                     141

/*
ID: 142
EID: ID6142
SHORT_NAME: CP_SyncJumpWidth
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: BUSTYPE
LAYER: PHYSICAL
DESCRIPTION:
This sets the desired synchronization jump width as a percentage of the bit time.
*/
#define COMPARAM_CP_SyncJumpWidth                         142

/*
ID: 143
EID: ID6143
SHORT_NAME: CP_SyncJumpWidth_Ecu
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: BUSTYPE
LAYER: PHYSICAL
DESCRIPTION:
This sets the desired synchronization jump width as a percentage of the bit time.
*/
#define COMPARAM_CP_SyncJumpWidth_Ecu                     143

/*
ID: 144
EID: ID6144
SHORT_NAME: CP_TerminationType
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: BUSTYPE
LAYER: PHYSICAL
DESCRIPTION:
CAN termination settings. This ComParam can be used to manually change the termination being used on the vehicle bus line.
*/
#define COMPARAM_CP_TerminationType                       144

/*
ID: 145
EID: ID6145
SHORT_NAME: CP_TerminationType_Ecu
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: BUSTYPE
LAYER: PHYSICAL
DESCRIPTION:
CAN termination settings for SWCAN ECU emulation.
*/
#define COMPARAM_CP_TerminationType_Ecu                   145

/*
ID: 146
EID: ID6146
SHORT_NAME: CP_UartConfig
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: BUSTYPE
LAYER: PHYSICAL
DESCRIPTION:
Configure the parity, data bit size and stop bits of a Uart protocol. This is restricted by the supported formats of a VCI.
*/
#define COMPARAM_CP_UartConfig                            146

/*
ID: 147
EID: ID6147
SHORT_NAME: CP_Loopback
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: APPLICATION
DESCRIPTION:
Echo Transmitted messages in the receive queue, including periodic messages. Loopback messages shall only be sent after successful transmission of a message. Loopback frames are not subject to message filtering.
*/
#define COMPARAM_CP_Loopback                              147

///*暂无参数
//ID: 148
//EID: ID6148
//SHORT_NAME: CP_NetworkTransmissionTime
//DATA_TYPE: PDU_PT_UNUM32
//DEFAULT_VALUE: 0
//CLASS: TIMING
//LAYER: APPLICATION
//DESCRIPTION:
//If the timeout values are used which have been received by the ECU via session control response (0x50), the Network transmission time has to be added to the timeout values.
//P2 = received P2 + NetworkTransmissionTime (contains delay for both transmission directions).
//NOTE: This ComParam is relevant for all protocols except those on CAN (especially Ethernet). For CAN protocols, the ComParam CP_CanTransmissionTime is relevant instead.
//*/
//#define COMPARAM_CP_NetworkTransmissionTime               148
//
///*
//ID: 150
//EID: ID6150
//SHORT_NAME: CP_P6Max
//DATA_TYPE: PDU_PT_UNUM32
//DEFAULT_VALUE: 0
//CLASS: TIMING
//LAYER: APPLICATION
//DESCRIPTION:
//Timeout for the client to wait after the successful transmission of a request message (indicated via T_Data.con) for the complete reception of the corresponding response message (indicated via T_Data.ind). If the transport/network layer doesn't support the T_DataSOM.ind interface (e.g. ISO 13400 DoIP), this ComParam shall be used instead of CP_P2Max.
//*/
//#define COMPARAM_CP_P6Max                                 150
//
///*
//ID: 151
//EID: ID6151
//SHORT_NAME: CP_P6Star
//DATA_TYPE: PDU_PT_UNUM32
//DEFAULT_VALUE: 0
//CLASS: TIMING
//LAYER: APPLICATION
//DESCRIPTION:
//Enhanced timeout for the client to wait after the reception of a negative response message with negative response code 0x78 (indicated via T_Data.ind) for the complete reception of the corresponding response messages (indicated via T_Data.ind). If the transport/network layer doesn't support the T_DataSOM.ind interface (e.g. ISO 13400 DoIP), this ComParam shall be used instead of CP_P2Star.
//*/
//#define COMPARAM_CP_P6Star                                151
//
///*
//ID: 152
//EID: ID6152
//SHORT_NAME: CP_DoIPLogicalGatewayAddress
//DATA_TYPE: PDU_PT_UNUM32
//DEFAULT_VALUE: 0
//CLASS: COM
//LAYER: TRANSPORT
//DESCRIPTION:
//The logical address of the DoIP gateway of the DoIP node. For a DoIP gateway, this address has to be used to reach the target addressed in CP_DoIPLogicalTargetAddress or CP_DoIPLogicalFunctionalAddress.
//For a DoIP node, the values for CP_DoIPLogicalGatewayAddress and CP_DoIPLogicalTargetAddress are identical
//*/
//#define COMPARAM_CP_DoIPLogicalGatewayAddress             152
//
///*
//ID: 153
//EID: ID6153
//SHORT_NAME: CP_DoIPLogicalSourceAddress
//DATA_TYPE: PDU_PT_UNUM32
//DEFAULT_VALUE: 0
//CLASS: COM
//LAYER: TRANSPORT
//DESCRIPTION:
//The logical source address of the Tester
//*/
//#define COMPARAM_CP_DoIPLogicalSourceAddress              153
//
///*
//ID: 154
//EID: ID6154
//SHORT_NAME: CP_DoIPLogicalTargetAddress
//DATA_TYPE: PDU_PT_UNUM32
//DEFAULT_VALUE: 0
//CLASS: UNIQUE_ID
//LAYER: TRANSPORT
//DESCRIPTION:
//The logical target address of the ECU to communicate with.
//*/
//#define COMPARAM_CP_DoIPLogicalTargetAddress              154
//
///*
//ID: 155
//EID: ID6155
//SHORT_NAME: CP_DoIPSecondaryLogicalECUResponseAddress
//DATA_TYPE: PDU_PT_UNUM32
//DEFAULT_VALUE: 0
//CLASS: UNIQUE_ID
//LAYER: TRANSPORT
//DESCRIPTION:
//Secondary logical ECU address delivered with ECU responses corresponding to CAN UUDT addressed responses. The special value of 0 is used to indicate that UUDT responses are not used.
//*/
//#define COMPARAM_CP_DoIPSecondaryLogicalECUResponseAddress 155
//
///*
//ID: 156
//EID: ID6156
//SHORT_NAME: CP_DoIPLogicalFunctionalAddress
//DATA_TYPE: PDU_PT_UNUM32
//DEFAULT_VALUE: 0
//CLASS: COM
//LAYER: TRANSPORT
//DESCRIPTION:
//The logical functional target address to address multiple ECUs behind a DoIP gateway.
//NOTE: Functional addressing is only possible on a single LogicalLink, which leads to the restriction, that functional addressing is not possible over multiple DoIP gateways or DoIP nodes
//*/
//#define COMPARAM_CP_DoIPLogicalFunctionalAddress          156
//
///*
//ID: 157
//EID: ID6157
//SHORT_NAME: CP_DoIPNumberOfNAckRetries
//DATA_TYPE: PDU_PT_UNUM32
//DEFAULT_VALUE: 0
//CLASS: COM
//LAYER: TRANSPORT
//DESCRIPTION:
//The number of retries to be performed when a certain NACK condition is encountered; see also CP_DoIPNAckRetryPeriod.
//*/
//#define COMPARAM_CP_DoIPNumberOfNAckRetries               157
//
///*
//ID: 158
//EID: ID6158
//SHORT_NAME: CP_DoIPCtrlTimeout
//DATA_TYPE: PDU_PT_UNUM32
//DEFAULT_VALUE: 0
//CLASS: TIMING
//LAYER: TRANSPORT
//DESCRIPTION:
//This timeout specifies the maximum time that the test equipment waits for a response to a previously sent UDP message. This includes the maximum time to wait and collect multiple responses to a previous broadcast (UDP only). (Corresponds to A_DoIP_Ctrl)
//*/
//#define COMPARAM_CP_DoIPCtrlTimeout                       158
//
///*
//ID: 159
//EID: ID6159
//SHORT_NAME: CP_DoIPTCPAliveCheckTime
//DATA_TYPE: PDU_PT_UNUM32
//DEFAULT_VALUE: 0
//CLASS: TIMING
//LAYER: TRANSPORT
//DESCRIPTION:
//This ComParam specifies the minimum amount of time the test equipment will wait after reception of an Alive Check Request message before transmitting the Alive Check Response message. (Corresponds to T_TCP_Alive_Check)
//*/
//#define COMPARAM_CP_DoIPTCPAliveCheckTime                 159
//
///*
//ID: 160
//EID: ID6160
//SHORT_NAME: CP_DoIPDiagnosticAckTimeout
//DATA_TYPE: PDU_PT_UNUM32
//DEFAULT_VALUE: 0
//CLASS: TIMING
//LAYER: TRANSPORT
//DESCRIPTION:
//This timeout specifies the maximum time that the test equipment waits for a confirmation ACK or NACK from the DoIP entity after the last byte of a DoIP Diagnostic request message has been sent.
//After the timeout has elapsed the request must be considered lost and the request may be repeated (see CP_RepeatReqCountTrans). (Corresponds to A_DoIP_Diagnostic_Message)
//*/
//#define COMPARAM_CP_DoIPDiagnosticAckTimeout              160
//
///*
//ID: 161
//EID: ID6161
//SHORT_NAME: CP_DoIPNAckRetryPeriod
//DATA_TYPE: PDU_PT_UNUM32
//DEFAULT_VALUE: 0
//CLASS: TIMING
//LAYER: TRANSPORT
//DESCRIPTION:
//The period between retries, performed when a certain NACK condition is encountered; see also CP_DoIPNumberOfNAckRetries.
//*/
//#define COMPARAM_CP_DoIPNAckRetryPeriod                   161
//
///*
//ID: 162
//EID: ID6162
//SHORT_NAME: CP_DoIPProcessingTime
//DATA_TYPE: PDU_PT_UNUM32
//DEFAULT_VALUE: 0
//CLASS: TIMING
//LAYER: TRANSPORT
//DESCRIPTION:
//This ComParam specifies the minimum amount of time that the test equipment must wait after successful transmission of a DoIP message that requires no response before sending another request to the DoIP entity. (Corresponds to A_Processing_Time)
//*/
//#define COMPARAM_CP_DoIPProcessingTime                    162
//
///*
//ID: 163
//EID: ID6163
//SHORT_NAME: CP_DoIPRoutingActivationType
//DATA_TYPE: PDU_PT_UNUM32
//DEFAULT_VALUE: 0
//CLASS: COM
//LAYER: TRANSPORT
//DESCRIPTION:
//Specifies the Activation Type sent in the DoIP Routing Activation request.
//Note: TCP connections shall be shared by multiple ComLogicalLinks that share the same value of CP_DoIPRoutingActivationType. For all ComLogicalLinks with differing values of CP_DoIPRoutingActivationType separate TCP connections (including separate Routing Activations) shall be used.
//*/
//#define COMPARAM_CP_DoIPRoutingActivationType             163

/*
ID: 148
EID: ID6148
SHORT_NAME: CPM_VWTP_5BaudAddressParity
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: INIT
LAYER: TRANSPORT
DESCRIPTION:
Controls the calculation and building of the parity bit of the 5BaudAddress byte.
*/
#define COMPARAM_CPM_VWTP_5BaudAddressParity              148

/*
ID: 149
EID: ID6149
SHORT_NAME: CPM_VWTP_ApplicationType
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: INIT
LAYER: TRANSPORT
DESCRIPTION:
Sets the Application Type field in the channel setup message that is sent from the tester to the ECU.
*/
#define COMPARAM_CPM_VWTP_ApplicationType                 149

/*
ID: 150
EID: ID6150
SHORT_NAME: CPM_VWTP_BlockSize
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
Sets the block size until ACK which is passed to the ECU on connection setup message.
*/
#define COMPARAM_CPM_VWTP_BlockSize                       150

/*
ID: 151
EID: ID6151
SHORT_NAME: CPM_VWTP_BlockSizeOverrideHandling
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
Signals to override the ECU block size.
*/
#define COMPARAM_CPM_VWTP_BlockSizeOverrideHandling       151

/*
ID: 152
EID: ID6152
SHORT_NAME: CPM_VWTP_BlockSizeOverrideValue
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
Sets the ECU block size until ACK. This overwrites the received value from the ECU when CPM_VWTP_BlockSizeOverrideHandling is enabled.
*/
#define COMPARAM_CPM_VWTP_BlockSizeOverrideValue          152

/*
ID: 153
EID: ID6153
SHORT_NAME: CPM_VWTP_BusTypeClass
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: INIT
LAYER: TRANSPORT
DESCRIPTION:
Sets bus type class information. Used to calculate the CAN identifier when CPM_VWTP_ComfortDiag is set to 1. Used to determine the setup and communication address of the target ECU.
*/
#define COMPARAM_CPM_VWTP_BusTypeClass                    153

/*
ID: 154
EID: ID6154
SHORT_NAME: CPM_VWTP_ChecksumCheck
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
Defines whether the checksum byte in the ECU response is verified.
*/
#define COMPARAM_CPM_VWTP_ChecksumCheck                   154

/*
ID: 155
EID: ID6155
SHORT_NAME: CPM_VWTP_ConnectionTestMode
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
Defines the operation mode for Connection Test.
*/
#define COMPARAM_CPM_VWTP_ConnectionTestMode              155

/*
ID: 156
EID: ID6156
SHORT_NAME: CPM_VWTP_ComfortDiag
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: UNIQUE_ID
LAYER: TRANSPORT
DESCRIPTION:
Used for automatic CAN-ID calculation. When set, the target address set in CPM_VWTP_DestAddr will be calculated according to VW Destination Address scheme.
VW CAN buses are divided into: Antrieb(Powertrain)-CAN, Komfort-CAN and Infotainment-CAN. For each of these, there is a defined way to determine the Channel Setup Address for the ECU and the tester.
Tester: 0x200 + CPM_VWTP_TesterAddr
ECU: 0x200 + CPM_VWTP_DestAddr.
*/
#define COMPARAM_CPM_VWTP_ComfortDiag                     156

/*
ID: 157
EID: ID6157
SHORT_NAME: CPM_VWTP_DC_RequestTime
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Sets the time until a DC message is sent by the tester after the last response. Required to cover problems with some ECUs.
*/
#define COMPARAM_CPM_VWTP_DC_RequestTime                  157

/*
ID: 158
EID: ID6158
SHORT_NAME: CPM_VWTP_DC_ResponseTimeout
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Sets the timeout for DC message from ECU. After this time the channel will be closed if no DC message was received. Can also be used to keep the CAN-Bus active when the logical link is the last one open.
*/
#define COMPARAM_CPM_VWTP_DC_ResponseTimeout              158

/*
ID: 159
EID: ID6159
SHORT_NAME: CPM_VWTP_DestAddr
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: INIT
LAYER: TRANSPORT
DESCRIPTION:
Sets the logical address of the target ECU used on Channel Setup.
*/
#define COMPARAM_CPM_VWTP_DestAddr                        159

/*
ID: 160
EID: ID6160
SHORT_NAME: CPM_VWTP_EcuAddressQfMode
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
Address of the ECU to use for the TesterPresent request if CPM_EnableQfMode is enabled.
*/
#define COMPARAM_CPM_VWTP_EcuAddressQfMode                160

/*
ID: 161
EID: ID6161
SHORT_NAME: CPM_VWTP_EnableQfMode
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
Enables use of the quasi functional mode.
If enabled the TesterPresent Message will be sent with the CPM_VWTP_EcuAddressQfMode.
*/
#define COMPARAM_CPM_VWTP_EnableQfMode                    161

/*
ID: 162
EID: ID6162
SHORT_NAME: CPM_VWTP_EOMAckHandling
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
Defines how the direction change is done: slow or fast.
*/
#define COMPARAM_CPM_VWTP_EOMAckHandling                  162

/*
ID: 163
EID: ID6163
SHORT_NAME: CPM_VWTP_InfoReceive
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: INIT
LAYER: TRANSPORT
DESCRIPTION:
This parameter controls the Info RX Flag Field (Bit 4) in Channel Setup.
Defines whether a valid RX-ID is contained in the RX-ID of Channel Setup.
*/
#define COMPARAM_CPM_VWTP_InfoReceive                     163

/*
ID: 164
EID: ID6164
SHORT_NAME: CPM_VWTP_InfoTransmit
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: INIT
LAYER: TRANSPORT
DESCRIPTION:
This parameter controls the Info TX Flag Field (Bit 4) in Channel Setup.
Defines whether a valid TX-ID is contained in the TX-ID of Channel Setup.
*/
#define COMPARAM_CPM_VWTP_InfoTransmit                    164

/*
ID: 165
EID: ID6165
SHORT_NAME: CPM_VWTP_MNCT
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
Defines the maximum number of retransmission for connection test telegrams.
A retransmission count of 'n' means that the applicable telegram will be sent up to n+1 times in total.
*/
#define COMPARAM_CPM_VWTP_MNCT                            165

/*
ID: 166
EID: ID6166
SHORT_NAME: CPM_VWTP_MNT
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
Defines the maximum number of acknowlege request repetitions.
A repeat count of 'n' means that the applicable telegram will be sent up to n+1 times in total.
*/
#define COMPARAM_CPM_VWTP_MNT                             166

/*
ID: 167
EID: ID6167
SHORT_NAME: CPM_VWTP_MNTB
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
Defines the maximum number of acceptances of telegram not ready requests within the block size.
An acceptance count of 'n' means that the corresponding request will be sent up to n+1 times in total.
*/
#define COMPARAM_CPM_VWTP_MNTB                            167

/*
ID: 168
EID: ID6168
SHORT_NAME: CPM_VWTP_MNTC
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
Defines the maximum number of retransmission for connecting structure telegrams (ChS/CS).
A retransmission count of 'n' means that the applicable telegram will be sent up to n+1 times in total.
*/
#define COMPARAM_CPM_VWTP_MNTC                            168

/*
ID: 169
EID: ID6169
SHORT_NAME: CPM_VWTP_PhysIdFormat
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: UNIQUE_ID
LAYER: TRANSPORT
DESCRIPTION:
Defines the CAN ID format used for all CAN identifiers, 0 and 1 = 11 bit, 2 = 29 bit.
*/
#define COMPARAM_CPM_VWTP_PhysIdFormat                    169

/*
ID: 170
EID: ID6170
SHORT_NAME: CPM_VWTP_PhysReqIdCon
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: UNIQUE_ID
LAYER: TRANSPORT
DESCRIPTION:
Defines the CAN ID used by tester for sending messages within an established connection. This parameter can be overwritten by the ECU with the Info TX.
Note: By default the RX-CAN ID is retrieved from ECU response during channel setup.
*/
#define COMPARAM_CPM_VWTP_PhysReqIdCon                    170

/*
ID: 171
EID: ID6171
SHORT_NAME: CPM_VWTP_PhysReqIdSetup
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: UNIQUE_ID
LAYER: TRANSPORT
DESCRIPTION:
Sets the CAN ID to be used by the ECU within established connection, part of tester Channel Setup request.
*/
#define COMPARAM_CPM_VWTP_PhysReqIdSetup                  171

/*
ID: 172
EID: ID6172
SHORT_NAME: CPM_VWTP_PhysRespIdCon
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: UNIQUE_ID
LAYER: TRANSPORT
DESCRIPTION:
Defines the CAN ID to be used by the ECU within established connection, part of tester Channel Setup request.
*/
#define COMPARAM_CPM_VWTP_PhysRespIdCon                   172

/*
ID: 173
EID: ID6173
SHORT_NAME: CPM_VWTP_PhysRespIdSetup
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: UNIQUE_ID
LAYER: TRANSPORT
DESCRIPTION:
Sets the CAN ID used for receiving the Channel Setup message by the tester.
*/
#define COMPARAM_CPM_VWTP_PhysRespIdSetup                 173

/*
ID: 174
EID: ID6174
SHORT_NAME: CPM_VWTP_SndTimeout
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Defines the timeout value for transmission of a CAN frame.
Note: The time value of the parameter has a granularity of about 40 ms.
*/
#define COMPARAM_CPM_VWTP_SndTimeout                      174

/*
ID: 175
EID: ID6175
SHORT_NAME: CPM_VWTP_TE
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Sets the T_E time, used with channel setup. It defines the timeout for ChA and CA messages from the ECU.
TP2.0-name: T_E
*/
#define COMPARAM_CPM_VWTP_TE                              175

/*
ID: 176
EID: ID6176
SHORT_NAME: CPM_VWTP_TesterTEDelay
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Defines the time between channel acknowledge and connection setup message sent to the ECU.
*/
#define COMPARAM_CPM_VWTP_TesterTEDelay                   176

/*
ID: 177
EID: ID6177
SHORT_NAME: CPM_VWTP_TN
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Defines the network latency time TN. This value is added to all receive timeout values. Within this time the ECU must send a response.
TP2.0-name: TN
*/
#define COMPARAM_CPM_VWTP_TN                              177

/*
ID: 178
EID: ID6178
SHORT_NAME: CPM_VWTP_T1
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Defines the tester T1 time, which is the timeout for acknowledge messages from the ECU and the value passed by the Connection Setup to the ECU.
TP2.0-name: T1
*/
#define COMPARAM_CPM_VWTP_T1                              178

/*
ID: 179
EID: ID6179
SHORT_NAME: CPM_VWTP_TesterAckDelay
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Defines the reaction time of the tester in case of sending an ACK message to the ECU.
Note: This is not T3. T3 is handled independently from this timing.
*/
#define COMPARAM_CPM_VWTP_TesterAckDelay                  179

/*
ID: 180
EID: ID6180
SHORT_NAME: CPM_VWTP_T1EcuOverrideHandling
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Defines the override handling of tester time T1.
*/
#define COMPARAM_CPM_VWTP_T1EcuOverrideHandling           180

/*
ID: 181
EID: ID6181
SHORT_NAME: CPM_VWTP_T1EcuOverrideValue
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Defines the override value for T1 time used by the tester for expected ACK messages. Used when CPM_VWTP_T1EcuOverrideHandling = 1.
TP2.0-name: T1
*/
#define COMPARAM_CPM_VWTP_T1EcuOverrideValue              181

/*
ID: 182
EID: ID6182
SHORT_NAME: CPM_VWTP_T2
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Defines the tester T2 time, which is the maximal time between two CAN messages from the ECU in a response.
TP1.6-name: T2
*/
#define COMPARAM_CPM_VWTP_T2                              182

/*
ID: 183
EID: ID6183
SHORT_NAME: CPM_VWTP_T2EcuOverrideHandling
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Defines the override handling of ECU T2 time.
*/
#define COMPARAM_CPM_VWTP_T2EcuOverrideHandling           183

/*
ID: 184
EID: ID6184
SHORT_NAME: CPM_VWTP_T2EcuOverrideValue
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Defines the ECU T2 time, which overrides the received value from the ECU if CPM_VWTP_T2EcuOverrideHandling is set to 1.
*/
#define COMPARAM_CPM_VWTP_T2EcuOverrideValue              184

/*
ID: 185
EID: ID6185
SHORT_NAME: CPM_VWTP_T3
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Defines the T3 time, which is the minimum time between two received messages from the ECU. The ECU has to stick to this time. The value is passed in the Connection Setup to the ECU.
TP1.6-name: T_T3
*/
#define COMPARAM_CPM_VWTP_T3                              185

/*
ID: 186
EID: ID6186
SHORT_NAME: CPM_VWTP_T3EcuOverrideHandling
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Defines the override handling of the ECU T3 time.
*/
#define COMPARAM_CPM_VWTP_T3EcuOverrideHandling           186

/*
ID: 187
EID: ID6187
SHORT_NAME: CPM_VWTP_T3EcuOverrideValue
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Defines the T3 value for the tester to send CAN messages, in case of CPM_VWTP_T3EcuOverrideHandling = 1.
*/
#define COMPARAM_CPM_VWTP_T3EcuOverrideValue              187

/*
ID: 188
EID: ID6188
SHORT_NAME: CPM_VWTP_T4
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Defines the tester T4 time, which is the maximum time between the last ACK and the next request from the tester. The value is passed to the ECU in the Connection Setup.
*/
#define COMPARAM_CPM_VWTP_T4                              188

/*
ID: 189
EID: ID6189
SHORT_NAME: CPM_VWTP_T4EcuOverrideHandling
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Defines the override handling of tester time T4.
*/
#define COMPARAM_CPM_VWTP_T4EcuOverrideHandling           189

/*
ID: 190
EID: ID6190
SHORT_NAME: CPM_VWTP_T4EcuOverrideValue
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Defines the tester T4 time that overrides the value T4 of the Connection Setup sent to the ECU if CPM_VWTP_T4EcuOverrideHandling = 1.
*/
#define COMPARAM_CPM_VWTP_T4EcuOverrideValue              190

/*
ID: 191
EID: ID6191
SHORT_NAME: CPM_VWTP_T_CT
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Defines the connection test time interval.
*/
#define COMPARAM_CPM_VWTP_T_CT                            191

/*
ID: 192
EID: ID6192
SHORT_NAME: CPM_VWTP_T_Wait
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Defines the delay timer used with RNR (ReceiverNotReady).
TP2.0-name: T_Wait
*/
#define COMPARAM_CPM_VWTP_T_Wait                          192

/*
ID: 193
EID: ID6193
SHORT_NAME: CPM_VWTP_TesterAddr
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: INIT
LAYER: TRANSPORT
DESCRIPTION:
Tester address, compared to the destination of the Channel Acknowledge message from the ECU in channel setup. Also used in ComfortDiag mode.
*/
#define COMPARAM_CPM_VWTP_TesterAddr                      193

/*
ID: 194
EID: ID6194
SHORT_NAME: CPM_VW2000_IgnoreEcuRespSourceAddress
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
Controls checking of the ECU response source address (see CP_EcuRespSourceAddress). If CPM_VW2000_IgnoreEcuRespSourceAddress is set to 1, the D-PDU API will not check the value of the ECU response source address for all ECU responses, which means that the D-PDU API will accept diagnostic messages from any ECU source address.
This ComParam is required to successfully communicate with certain ECUs.
*/
#define COMPARAM_CPM_VW2000_IgnoreEcuRespSourceAddress    194

/*
ID: 195
EID: ID6195
SHORT_NAME: CPM_VW1281_SkipAckCompletionTimeout
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: APPLICATION
DESCRIPTION:
Sets the maximum time the tester can wait until the beginning of the first ECU response block. The ACK blocks received before the first ECU response block are skipped.
*/
#define COMPARAM_CPM_VW1281_SkipAckCompletionTimeout      195

/*
ID: 196
EID: ID6196
SHORT_NAME: CPM_VW1281_DisableBlockTitleCheck
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: APPLICATION
DESCRIPTION:
Controls BlockTitle checking for received blocks.
*/
#define COMPARAM_CPM_VW1281_DisableBlockTitleCheck        196

/*
ID: 197
EID: ID6197
SHORT_NAME: CPM_VW1281_P2Max_SGID
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: APPLICATION
DESCRIPTION:
Sets the timeout for receiving the first ECU identification response block after the 5Baud-initialisation.
*/
#define COMPARAM_CPM_VW1281_P2Max_SGID                    197

/*
ID: 198
EID: ID6198
SHORT_NAME: CPM_VW1281_MaxNoAck1Ecu
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: ERRHDL
LAYER: APPLICATION
DESCRIPTION:
Counter to enable a re-transmission of the last request when either a transmit, receive error, or timeout with no response is detected.
*/
#define COMPARAM_CPM_VW1281_MaxNoAck1Ecu                  198

/*
ID: 199
EID: ID6199
SHORT_NAME: CPM_VW1281_MaxNoAck1TG
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: ERRHDL
LAYER: APPLICATION
DESCRIPTION:
Counter to control the re-transmission of NoAck-1 or NoAck-1 UB messages sent by the tester when an error occurred.
*/
#define COMPARAM_CPM_VW1281_MaxNoAck1TG                   199

/*
ID: 200
EID: ID6200
SHORT_NAME: CPM_VW1281_P1Slow
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: TIMING
LAYER: TRANSPORT
DESCRIPTION:
Sets the maximum time the tester has to wait for the next byte from the ECU, when communication speed is 1000 Baud. If the communication speed is greater than 1000 Baud, the CP_P1Max parameter value is used as inter-byte time.
*/
#define COMPARAM_CPM_VW1281_P1Slow                        200

/*
ID: 201
EID: ID6201
SHORT_NAME: CPM_VW1281_CollectBlocksOnIdle
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: APPLICATION
DESCRIPTION:
This parameters is used to collect responses from the ECU in idle state after a ComPrimitive is finished.
Note: There is a 10KB data buffer defined for collected responses. When this limit is reached, no more responses can be collected until the collected responses are retrieved by repeating the last request.
*/
#define COMPARAM_CPM_VW1281_CollectBlocksOnIdle           201

/*
ID: 202
EID: ID6202
SHORT_NAME: CPM_VW1281_RepeatLastBlock
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: APPLICATION
DESCRIPTION:
This parameter is used to enable or disable periodic repetition of the last request message.
*/
#define COMPARAM_CPM_VW1281_RepeatLastBlock               202

/*
ID: 203
EID: ID6203
SHORT_NAME: CPM_VW1281_RespToStopComm
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: APPLICATION
DESCRIPTION:
Determines whether a response is expected after the StopCom service.
*/
#define COMPARAM_CPM_VW1281_RespToStopComm                203

/*
ID: 204
EID: ID6204
SHORT_NAME: CPM_VW1281_SkipLeadingAckBlocks
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: APPLICATION
DESCRIPTION:
Determines whether the tester will wait for an ECU response and skip ACK blocks that are sent before the response begins.
*/
#define COMPARAM_CPM_VW1281_SkipLeadingAckBlocks          204

/*
ID: 205
EID: ID6205
SHORT_NAME: CPM_VW1281_GenerateHeader
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: APPLICATION
DESCRIPTION:
Enables the automatic header generation for KWP1281 messages and also adds the ETX byte at the end of the message.
*/
#define COMPARAM_CPM_VW1281_GenerateHeader                205

/*
ID: 206
EID: ID6206
SHORT_NAME: CPM_VW1281_RemoveHeader
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: APPLICATION
DESCRIPTION:
Optional protocol parameter that enables removal of header for KWP1281 messages and also the ETX byte at the end of the message.
*/
#define COMPARAM_CPM_VW1281_RemoveHeader                  206

/*
ID: 207
EID: ID6207
SHORT_NAME: CPM_VW2000_5BaudFlashProgInit
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: TRANSPORT
DESCRIPTION:
Enables the VWFlashProg1 and VWFlashProg2 programming sequences for KWP1281 ECUs during protocol initialisation.
*/
#define COMPARAM_CPM_VW2000_5BaudFlashProgInit            207

/*
ID: 208
EID: ID6208
SHORT_NAME: CPM_VW1281_5BaudCombiInitialization
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: INIT
LAYER: TRANSPORT
DESCRIPTION:
Enables or disables the KWP1281 / KWP2000 combination initialization mode.
*/
#define COMPARAM_CPM_VW1281_5BaudCombiInitialization      208

///* 暂时无此参数
//ID: 225
//EID: ID6225
//SHORT_NAME: CPM_VW1281_DisableBlockCounterCheck
//DATA_TYPE: PDU_PT_UNUM32
//DEFAULT_VALUE: 0
//CLASS: COM
//LAYER: APPLICATION
//DESCRIPTION:
//Controls the BlockCounter checking for received KWP1281 protocol blocks.
//NOTE: Setting the value to (1) shall lead to the usage of the value n+1 (with n being the (wrong) BlockCounter received from the ECU) for the next KWP1281 protocol block sent by the D-PDU-API regardless of the BlockCounter that has been calculated internally, e.g. a wrong BlockCounter of 0x02 received from the ECU shall be followed by a BlockCounter of 0x03 from the D PDU API.
//*/
//#define COMPARAM_CPM_VW1281_DisableBlockCounterCheck      225
//
///*
//ID: 226
//EID: ID6226
//SHORT_NAME: CPM_SFTG_KeyByteCheck
//DATA_TYPE: PDU_PT_UNUM32
//DEFAULT_VALUE: 0
//CLASS: INIT
//LAYER: TRANSPORT
//DESCRIPTION:
//Enables or disables the key byte check after protocol initialization via PDU_COPT_STARTCOMM.
//*/
//#define COMPARAM_CPM_SFTG_KeyByteCheck                    226

/*
ID: 209
EID: ID6209
SHORT_NAME: CPM_SFTG_ExtendedMonitorSettings
DATA_TYPE: PDU_PT_UNUM32
DEFAULT_VALUE: 0
CLASS: COM
LAYER: APPLICATION
DESCRIPTION:
Extended monitor functionality available
*/
#define COMPARAM_CPM_SFTG_ExtendedMonitorSettings         209


//爱夫卡自定义参数 以下参数按协议号区别  
//9141或14230
#define COMPARAM_CPM_KWPKeyWord1                 0xF1//9141或14230 5bps init Key1 Key2
#define COMPARAM_CPM_KWPKeyWord2                 0xF2

//PROTOCOL 
#define PROTOCOL_ISO_15765_3_on_ISO_15765_2               260 //ID4260 UDSCAN
#define PROTOCOL_ISO_14230_3_on_ISO_15765_2               258 //ID4258 DiagCAN
#define PROTOCOL_ISO_14230_3_on_ISO_14230_2               266 //ID4266 KWP2000GV
#define PROTOCOL_MSP_VW2000LP_on_ISO_14230_2              284 //ID4284 VW2000LP
#define PROTOCOL_ISO_11898_RAW                            283 //ID4283 CAN RAW protocol
#define PROTOCOL_MSP_SFTNG_ISO_11898_onboard              282 //ID4282 Softing CAN onboard protocol
#define PROTOCOL_MSP_VW2000LP_on_TP20                     264 //ID4264 KWP2000 on VW TP2.0
#define PROTOCOL_MSP_VW2000LP_on_TP16                     279 //ID4279 KWP2000 on VW TP1.6
#define PROTOCOL_MSP_KW1281_on_ISO_9141_2                 277 //ID4277 KWP1281 on K-Line
#define PROTOCOL_MSP_KW1281_on_TP16                       278 //ID4278 KWP1281 on TP1.6
#define PROTOCOL_ISO_OBD_on_K_Line                        280 //ID4280 ISO OBD on 9141-2 K-Line and KWP2000 K-Line
#define PROTOCOL_ISO_OBD_on_ISO_15765_4                   281 //ID4281 ISO OBD on CAN
#define PROTOCOL_ISO_14229_5_on_ISO_13400_2               285 //ID4285 UDSonDoIP

typedef struct
{
	UNUM32 CP_CanIDPreReply;		//my code
	UNUM32 CP_CanTransmissionTime;
	UNUM32 CP_CyclicRespTimeout;
	UNUM32 CP_EnablePerformanceTest;
	UNUM32 CP_ModifyTiming;
	UNUM32 CP_P2Max;
	UNUM32 CP_P2Max_Ecu;
	UNUM32 CP_P2Min;
	UNUM32 CP_P2Star;
	UNUM32 CP_P2Star_Ecu;
	UNUM32 CP_P3Func;
	UNUM32 CP_P3Max_Ecu;
	UNUM32 CP_P3Min;
	UNUM32 CP_P3Phys;
	UNUM32 CP_RC21CompletionTimeout;
	UNUM32 CP_RC21Handling;
	UNUM32 CP_RC21RequestTime;
	UNUM32 CP_RC23CompletionTimeout;
	UNUM32 CP_RC23Handling;
	UNUM32 CP_RC23RequestTime;
	UNUM32 CP_RC78CompletionTimeout;
	UNUM32 CP_RC78Handling;
	UNUM32 CP_RCByteOffset;
	UNUM32 CP_RepeatReqCountApp;
	UNUM32 CP_StartMsgIndEnable;
	UNUM32 CP_SuspendQueueOnError;
	UNUM32 CP_SwCan_HighVoltage;
	UNUM32 CP_TesterPresentHandling;
	UNUM32 CP_TesterPresentReqRsp;
	UNUM32 CP_TesterPresentSendType;
	UNUM32 CP_TesterPresentAddrMode;
	UNUM32 CP_TesterPresentTime;
	UNUM32 CP_TesterPresentTime_Ecu;
	UNUM32 CP_TesterPresentExpPosResp;
	UNUM32 CP_TesterPresentExpNegResp;
	UNUM32 CP_TesterPresentMessage;
	UNUM32 CP_TransmitIndEnable;
	UNUM32 CP_5BaudAddressFunc;
	UNUM32 CP_5BaudAddressPhys;
	UNUM32 CP_5BaudAddress;			//PDU内只有这一个参数，用于发送给ECU
	UNUM32 CP_5BaudMode;
	UNUM32 CP_Ar;
	UNUM32 CP_Ar_Ecu;
	UNUM32 CP_As;
	UNUM32 CP_As_Ecu;
	UNUM32 CP_BlockSize;
	UNUM32 CP_BlockSize_Ecu;
	UNUM32 CP_BlockSizeOverride;
	UNUM32 CP_Br;
	UNUM32 CP_Br_Ecu;
	UNUM32 CP_Bs;
	UNUM32 CP_Bs_Ecu;
	UNUM32 CP_CanDataSizeOffset;
	UNUM32 CP_FillerByte;
	UNUM32 CP_FillerByteHandling;
	UNUM32 CP_FillerByteLength;
	UNUM32 CP_CanFillerByte;
	UNUM32 CP_CanFillerByteHandling;
	UNUM32 CP_CanFirstConsecutiveFrameValue;
	UNUM32 CP_CanFuncReqExtAddr;
	UNUM32 CP_CanFuncReqFormat;
	UNUM32 CP_CanFuncReqId;
	UNUM32 CP_CanMaxNumWaitFrames;
	UNUM32 CP_CanPhysReqExtAddr;
	UNUM32 CP_CanPhysReqFormat;
	UNUM32 CP_CanPhysReqId;
	UNUM32 CP_CanRespUSDTExtAddr;
	UNUM32 CP_CanRespUSDTFormat;
	UNUM32 CP_CanRespUSDTId;
	UNUM32 CP_CanRespUUDTExtAddr;
	UNUM32 CP_CanRespUUDTFormat;
	UNUM32 CP_CanRespUUDTId;
	UNUM32 CP_Cr;
	UNUM32 CP_Cr_Ecu;
	UNUM32 CP_Cs;
	UNUM32 CP_Cs_Ecu;
	UNUM32 CP_EcuRespSourceAddress;
	UNUM32 CP_EnableConcatenation;
	UNUM32 CP_FuncReqFormatPriorityType;
	UNUM32 CP_FuncReqTargetAddr;
	UNUM32 CP_FuncRespFormatPriorityType;
	UNUM32 CP_FuncRespTargetAddr;
	UNUM32 CP_HeaderFormatJ1850;
	UNUM32 CP_HeaderFormatKW;
	UNUM32 CP_InitializationSettings;
	UNUM32 CP_J1939TransportType;
	UNUM32 CP_MessagePriority;
	UNUM32 CP_MidReqId;
	UNUM32 CP_MidRespId;
	UNUM32 CP_P1Max;
	UNUM32 CP_P1Min;
	UNUM32 CP_P4Max;
	UNUM32 CP_P4Min;
	UNUM32 CP_PhysReqFormatPriorityType;
	UNUM32 CP_PhysReqTargetAddr;
	UNUM32 CP_PhysRespFormatPriorityType;
	UNUM32 CP_RepeatReqCountTrans;
	UNUM32 CP_DoIPTcpErrHandling;
	UNUM32 CP_RequestAddrMode;
	UNUM32 CP_SCITransmitMode;
	UNUM32 CP_SendRemoteFrame;
	UNUM32 CP_StMin;
	UNUM32 CP_StMin_Ecu;
	UNUM32 CP_StMinOverride;
	UNUM32 CP_T1Max;
	UNUM32 CP_T2Max;
	UNUM32 CP_T3Max;
	UNUM32 CP_T4Max;
	UNUM32 CP_T5Max;
	UNUM32 CP_TesterSourceAddress;
	UNUM32 CP_TIdle;
	UNUM32 CP_TInil;
	UNUM32 CP_TWup;
	UNUM32 CP_W1Max;
	UNUM32 CP_W1Min;
	UNUM32 CP_W2Max;
	UNUM32 CP_W2Min;
	UNUM32 CP_W3Max;
	UNUM32 CP_W3Min;
	UNUM32 CP_W4Max;
	UNUM32 CP_W4Min;
	UNUM32 CP_Baudrate;
	UNUM32 CP_BitSamplePoint;
	UNUM32 CP_BitSamplePoint_Ecu;
	UNUM32 CP_ChangeSpeedCtrl;
	UNUM32 CP_ChangeSpeedRate;
	UNUM32 CP_ChangeSpeedResCtrl;
	UNUM32 CP_ChangeSpeedTxDelay;
	UNUM32 CP_K_L_LineInit;
	UNUM32 CP_K_LinePullup;
	UNUM32 CP_ListenOnly;
	UNUM32 CP_NetworkLine;
	UNUM32 CP_SamplesPerBit;
	UNUM32 CP_SamplesPerBit_Ecu;
	UNUM32 CP_SyncJumpWidth;
	UNUM32 CP_SyncJumpWidth_Ecu;
	UNUM32 CP_TerminationType;
	UNUM32 CP_TerminationType_Ecu;
	UNUM32 CP_UartConfig;
	UNUM32 CP_Loopback;
	UNUM32 CP_NetworkTransmissionTime;
	UNUM32 CP_P6Max;
	UNUM32 CP_P6Star;
	UNUM32 CP_DoIPLogicalGatewayAddress;
	UNUM32 CP_DoIPLogicalSourceAddress;
	UNUM32 CP_DoIPLogicalTargetAddress;
	UNUM32 CP_DoIPSecondaryLogicalECUResponseAddress;
	UNUM32 CP_DoIPLogicalFunctionalAddress;
	UNUM32 CP_DoIPNumberOfNAckRetries;
	UNUM32 CP_DoIPCtrlTimeout;
	UNUM32 CP_DoIPTCPAliveCheckTime;
	UNUM32 CP_DoIPDiagnosticAckTimeout;
	UNUM32 CP_DoIPNAckRetryPeriod;
	UNUM32 CP_DoIPProcessingTime;
	UNUM32 CP_DoIPRoutingActivationType;
	UNUM32 CPM_VWTP_5BaudAddressParity;
	UNUM32 CPM_VWTP_ApplicationType;
	UNUM32 CPM_VWTP_BlockSize;
	UNUM32 CPM_VWTP_BlockSizeOverrideHandling;
	UNUM32 CPM_VWTP_BlockSizeOverrideValue;
	UNUM32 CPM_VWTP_BusTypeClass;
	UNUM32 CPM_VWTP_ChecksumCheck;
	UNUM32 CPM_VWTP_ConnectionTestMode;
	UNUM32 CPM_VWTP_ComfortDiag;
	UNUM32 CPM_VWTP_DC_RequestTime;
	UNUM32 CPM_VWTP_DC_ResponseTimeout;
	UNUM32 CPM_VWTP_DestAddr;
	UNUM32 CPM_VWTP_EcuAddressQfMode;
	UNUM32 CPM_VWTP_EnableQfMode;
	UNUM32 CPM_VWTP_EOMAckHandling;
	UNUM32 CPM_VWTP_InfoReceive;
	UNUM32 CPM_VWTP_InfoTransmit;
	UNUM32 CPM_VWTP_MNCT;
	UNUM32 CPM_VWTP_MNT;
	UNUM32 CPM_VWTP_MNTB;
	UNUM32 CPM_VWTP_MNTC;
	UNUM32 CPM_VWTP_PhysIdFormat;
	UNUM32 CPM_VWTP_PhysReqIdCon;
	UNUM32 CPM_VWTP_PhysReqIdSetup;
	UNUM32 CPM_VWTP_PhysRespIdCon;
	UNUM32 CPM_VWTP_PhysRespIdSetup;
	UNUM32 CPM_VWTP_SndTimeout;
	UNUM32 CPM_VWTP_TE;
	UNUM32 CPM_VWTP_TesterTEDelay;
	UNUM32 CPM_VWTP_TN;
	UNUM32 CPM_VWTP_T1;
	UNUM32 CPM_VWTP_TesterAckDelay;
	UNUM32 CPM_VWTP_T1EcuOverrideHandling;
	UNUM32 CPM_VWTP_T1EcuOverrideValue;
	UNUM32 CPM_VWTP_T2;
	UNUM32 CPM_VWTP_T2EcuOverrideHandling;
	UNUM32 CPM_VWTP_T2EcuOverrideValue;
	UNUM32 CPM_VWTP_T3;
	UNUM32 CPM_VWTP_T3EcuOverrideHandling;
	UNUM32 CPM_VWTP_T3EcuOverrideValue;
	UNUM32 CPM_VWTP_T4;
	UNUM32 CPM_VWTP_T4EcuOverrideHandling;
	UNUM32 CPM_VWTP_T4EcuOverrideValue;
	UNUM32 CPM_VWTP_T_CT;
	UNUM32 CPM_VWTP_T_Wait;
	UNUM32 CPM_VWTP_TesterAddr;
	UNUM32 CPM_VW2000_IgnoreEcuRespSourceAddress;
	UNUM32 CPM_VW1281_SkipAckCompletionTimeout;
	UNUM32 CPM_VW1281_DisableBlockTitleCheck;
	UNUM32 CPM_VW1281_P2Max_SGID;
	UNUM32 CPM_VW1281_MaxNoAck1Ecu;
	UNUM32 CPM_VW1281_MaxNoAck1TG;
	UNUM32 CPM_VW1281_P1Slow;
	UNUM32 CPM_VW1281_CollectBlocksOnIdle;
	UNUM32 CPM_VW1281_RepeatLastBlock;
	UNUM32 CPM_VW1281_RespToStopComm;
	UNUM32 CPM_VW1281_SkipLeadingAckBlocks;
	UNUM32 CPM_VW1281_GenerateHeader;
	UNUM32 CPM_VW1281_RemoveHeader;
	UNUM32 CPM_VW2000_5BaudFlashProgInit;
	UNUM32 CPM_VW1281_5BaudCombiInitialization;
	UNUM32 CPM_VW1281_DisableBlockCounterCheck;
	UNUM32 CPM_SFTG_KeyByteCheck;
	UNUM32 CPM_SFTG_ExtendedMonitorSettings;
} CHANNEL_PARAM_ITEM;


//诊断入口
extern void Work_Thread(void);
extern uint8_t PDU_Transport_Protocol_ReadData_DoNull(uint8_t *Data, uint16_t maxlen);
extern uint8_t (*PDU_Transport_Protocol_Indication_CH0)(uint8_t *Data, uint16_t maxlen); //函数指针
extern uint8_t (*PDU_Transport_Protocol_Indication_CH1)(uint8_t *Data, uint16_t maxlen); //函数指针
extern uint8_t (*PDU_Transport_Protocol_Indication_CH1_Bak)(uint8_t *Data, uint16_t maxlen);

//通道使用
extern void Channel_Protocol(uint8_t Channel, uint16_t wParam, uint8_t *lParam);
extern void Channel_SetPinType(uint8_t Channel, uint16_t wParam, uint8_t *lParam);//设置PINTYPE
extern void Channel_SetComParam(uint8_t Channel, uint16_t wParam, uint8_t *lParam);//设置参数
extern void Channel_SetFilter(uint8_t Channel, uint16_t wParam, uint8_t *lParam);
extern void Channel_ClearBuffer(uint8_t Channel, uint16_t wParam, uint8_t *lParam);
extern void Channel_GetComParam(uint8_t Channel, uint16_t wParam, uint8_t *lParam);//获取参数
extern void Channel_Request(uint8_t Channel, uint16_t wParam, uint8_t *lParam);
extern void Channel_AbortRequest(uint8_t Channel, uint16_t wParam, uint8_t *lParam);
void Channel_SwitchProtocol(uint8_t Protocol, uint16_t wParam, uint8_t *lParam);
extern void Channel_ReadVersion_confirm(uint8_t Protocol, uint16_t wParam, uint8_t *lParam);
extern void Channel_Device_Active(uint8_t Protocol, uint16_t wParam, uint8_t *lParam);
extern uint8_t PDU_Null_Protocol_Indication(uint8_t *RXdata, uint16_t maxlen);
extern void Channel_Device_Reset(uint8_t Protocol, uint16_t wParam, uint8_t *lParam);
extern void Channel_Pre_Reply(uint8_t Protocol, uint16_t wParam, uint8_t *lParam);
extern void Channel_Get_Timestamp(uint8_t Protocol, uint16_t wParam, uint8_t *lParam);
extern void Channel_SoftPulse(uint8_t Protocol, uint16_t wParam, uint8_t *lParam);
extern void Channel_Erase_Firmware(uint8_t Protocol, uint16_t wParam, uint8_t *lParam);
extern void Channel_Device_Reboot(uint8_t Protocol, uint16_t wParam, uint8_t *lParam);
extern void Channel_MvciVolt(uint8_t Protocol, uint16_t wParam, uint8_t *lParam);
extern void Channel_GetUID(uint8_t Protocol, uint16_t wParam, uint8_t *lParam);
extern void Channel_GetSN(uint8_t Protocol, uint16_t wParam, uint8_t *lParam);
extern void Channel_Set_CmpVolt(uint8_t Protocol, uint16_t wParam, uint8_t *lParam);
extern void Channel_SetPin8_Status(uint8_t Protocol, uint16_t wParam, uint8_t *lParam);

extern CHANNEL_PARAM_ITEM Channel_0_Param;
extern CHANNEL_PARAM_ITEM Channel_1_Param;
extern PINTYPE_ITEM       PinTypeParam;
extern uint8_t PDU_CanBus_PreReply_Start_Flag;
extern uint8_t PDU_CanBus_PreReplyGroup_Start_Flag;
extern uint16_t PDU_DeviceActiveVaule;    //设备激活标志



#endif /* __PROTOCOL_Device_H */
