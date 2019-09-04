/**
 * \file
 *
 * \brief FreeRTOS Real Time Kernel example.
 *
 * Copyright (c) 2012-2016 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

/**
 * \mainpage FreeRTOS Real Time Kernel example
 *
 * \section Purpose
 *
 * The FreeRTOS example will help users how to use FreeRTOS in SAM boards.
 * This basic application shows how to create task and get information of
 * created task.
 *
 * \section Requirements
 *
 * This package can be used with SAM boards.
 *
 * \section Description
 *
 * The demonstration program create two task, one is make LED on the board
 * blink at a fixed rate, and another is monitor status of task.
 *
 * \section Usage
 *
 * -# Build the program and download it inside the evaluation board. Please
 *    refer to the
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6224.pdf">
 *    SAM-BA User Guide</a>, the
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">
 *    GNU-Based Software Development</a>
 *    application note or to the
 *    <a href="ftp://ftp.iar.se/WWWfiles/arm/Guides/EWARM_UserGuide.ENU.pdf">
 *    IAR EWARM User Guide</a>,
 *    depending on your chosen solution.
 * -# On the computer, open and configure a terminal application
 *    (e.g. HyperTerminal on Microsoft Windows) with these settings:
 *   - 115200 bauds
 *   - 8 bits of data
 *   - No parity
 *   - 1 stop bit
 *   - No flow control
 * -# Start the application.
 * -# LED should start blinking on the board. In the terminal window, the
 *    following text should appear (values depend on the board and chip used):
 *    \code
	-- Freertos Example xxx --
	-- xxxxxx-xx
	-- Compiled: xxx xx xxxx xx:xx:xx --
\endcode
 *
 */

#include "config.h"
#include "PassThru.h"
 
 typedef unsigned char  uint8;                   /* defined for unsigned 8-bits integer variable     ???8?????  */
 typedef signed   char  int8;                    /* defined for signed 8-bits integer variable       ???8?????  */
 typedef unsigned short uint16;                  /* defined for unsigned 16-bits integer variable    ???16????? */
 typedef signed   short int16;                   /* defined for signed 16-bits integer variable      ???16????? */
 typedef unsigned int   uint32;                  /* defined for unsigned 32-bits integer variable    ???32????? */
 typedef signed   int   int32;                   /* defined for signed 32-bits integer variable      ???32????? */
 typedef float          fp32;                    /* single precision floating point variable (32bits) ??????(32???) */
 typedef double         fp64;                    /* double precision floating point variable (64bits) ??????(64???) */


 uint16 usart_len;																	//2
 uint32 ECU_Byte;			//Ҫ��������									//4
 uint8  ECU_Byte_num;  //Ҫ�������ݵ�bitλ					//1
 uint8  ecu_staus;			//�������ݳɹ�							//1 
	
 uint8  RxFrameFlag;

 uint16 USART1_ReceiveNum;													//2
 uint8 USART1_TxFlag;																//1
 uint8 USART1_RxFlag;																//1
// uint8 USART1_TxBuff[TXQUEUEBUFLEN];//[DMATXBUFLEN];									//250
// uint8 USART1_RxBuff[TXQUEUEBUFLEN];//[DMARXBUFLEN];									//250
//extern uint8 USART1_RxBuff_temp[TXQUEUEBUFLEN];//USART1_RxBuff_temp[DMARXBUFLEN]; 4110  4622
 uint32 timeflag, timeidle;																//8

 uint16 ErrCode;//������																	//2
 PASSTHRU_CONFIG PassThruConfig;													//200
 PASSTHRU_DRIVER PassThruDriver;												//

 volatile uint16 KeepComTimeCounter[2];      //ECUͨѶ��������
//extern volatile uint16 UartTimeLimited;     //����ͨѶʱ�䣬Ĭ��Ϊ500ms


 volatile uint32 counter_1ms;													//4
 volatile uint32 counter_1ms_;												//4

 ComCtlType ComCtl;  //ͨѶ����
 UARTMODE UartCtlReg;
 uint8 USART1_RxFlag;																	//1
 uint16 KeepComTime[2];																//2
 uint8 KeepComSwitch;																	//1
 uint8 KeepComSwitch1;																//1
 uint8 KeepComSwitch2;//���յ�7F78�񶨻ش�ر���λ���Զ�ͨѶ����//1

 uint8  CanbusInsert[KEEP_COM_BUF_LEN];								//100
 uint8 EcuComRxBuf_temp[ECURXBUFLEN];									//250
 //uint8 *EcuComRxBuf;
 uint8 EcuComTxBuf[ECUTXBUFLEN];											//250
 uint8 TempProcBuf[TXQUEUEBUFLEN];												//4110
//;TEMPBUFLEN
 uint8 KeepComBuf[KEEP_COM_BUF_LEN];									//100
 uint8 KeepComBuf1[KEEP_COM_BUF_LEN];									//100
 uint8 ProtocolType;   //Э������
 uint8 StatusByte;     //״̬�����ֽ�
 uint8 ValidFlag;
 uint8 AnswerFlag;
 uint8 LineInitFlag;
 uint8 LineLevelFlag;
 uint32 _1msCounter;
 uint32 _10msCounter;
 uint32 endmsCounter;
 uint32 ecumsCounter;
 uint8 K2K_Simulate_firstRX_flag;    //ģ��ģʽ��һ�ν��ձ�־
 uint8 Stop_bit_5bps_flag;
 uint8 DelayTimeForHoldCom; //ͨѶ���ֺ����ʱ
 uint8 UartTxQueueBuf_ext[TXQUEUEBUFLEN];													//4k
 uint8 *UartTxQueueBuf;
 uint8 RxChecksum;
 uint8 TxFrameFlag;
 uint16 ByteCounter;
 uint16 revPt;
 uint8 RxFrameFlag;
 uint8 UartTxSwitch;
 uint8 CAN_ID_FILT_FLAG;        //CAN ID ������˱�־λ
 uint8 ChannelBuf[8];//���ĸ��ֽڿ��������������ͽ���ģʽ�����ĸ��ֽڿ��ƽ�ͨobd�����ź�
//23 5baudǰ�ڿ���ʱ�䷢����
 uint8 kwp_5bpsdata_delay_sent_flag ;
 uint8 kl_set[4];
 uint32 kl_set_time;
 uint8 adjust_width[4];//0:line0����ʱ��line0 line1ͬʱ����ʱ�ĵ�������;1:line1����;2��line0������line0 line1ͬʱ����;3:line1����
 uint8 adjust_width_select;
 uint8 ExitWaveFlag;
 uint8 WaveNum;
 uint8 FrameFormat[2];   //֡��ʽSFF �� EFF
 uint8 Can_Send_Mode[2];
 uint8 UartBit9;
 uint8 can_test_status;
 uint16 vcc_drv;
