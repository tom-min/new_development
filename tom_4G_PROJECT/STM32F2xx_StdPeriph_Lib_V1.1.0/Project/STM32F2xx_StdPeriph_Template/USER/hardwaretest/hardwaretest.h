/**
 * \file
 *
 * \brief Common User Interface for HID Mouse application
 *
 * Copyright (c) 2009-2015 Atmel Corporation. All rights reserved.
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
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef _HARDWARE_TEST_H_
#define _HARDWARE_TEST_H_
#include <stdint.h>
#include "PortAssign.h"
#include "interface.h"
#include "gpio.h"
//VCC_PROG
#define PIO_PC16_IDX 1
#define PIO_PC17_IDX 1
#define PIO_PC18_IDX 1
#define PIO_PC19_IDX 1
#define PIO_PA21_IDX 1
 
#define PIO_PD18_IDX 1
#define PIO_PB4_IDX 1
#define PIO_PA22_IDX 1
 
#define PIO_PA9_IDX 1
#define PIO_PA10_IDX 1

#define  PIO_PA7_IDX 1
#define  PIO_PA8_IDX 3
#define  PIO_PA2_IDX 1
#define  PIO_PA1_IDX 1
#define  PIO_PA0_IDX 1
#define  PIO_PA3_IDX 1
#define  PIO_PA6_IDX 1
#define  PIO_PD10_IDX 1
#define   PIO_PD11_IDX 1 

#define  PIO_PC11_IDX 1
#define PIO_PD30_IDX 1


 
typedef enum
{
    HARDWARE_TEST_1 = 0, /*£¨0x00£©VCC_IN TEST*/
    HARDWARE_TEST_2 ,   /*£¨0x01£©VCC_PROG TEST*/
    HARDWARE_TEST_3 ,  /*£¨0x02£©VCC_DRV TEST*/
    HARDWARE_TEST_4 , /*£¨0x03£©BUS1_CMP_V TEST*/
    HARDWARE_TEST_5 , /*£¨0x04£©VCC_DRV2 TEST*/
    HARDWARE_TEST_6 , /*£¨0x05£©DB26_PIN1 TEST*/ 
	HARDWARE_TEST_7 , /*£¨0x06£©LINE0 TEST*/     
	HARDWARE_TEST_8 , /*£¨0x07£©can1 TEST*/ 
	HARDWARE_TEST_9 , /*£¨0x08£©can2 TEST*/ 
	HARDWARE_TEST_10 , /*£¨0x09£©WCFTCANH TEST*/ 
	
	HARDWARE_TEST_11 , /*£¨0x0A£©DB_26_PIN  TEST*/ 
	
	
	HARDWARE_TEST_12 , /*£¨0x0B£©LINE0 PULLUP PULLDOWN TEST*/  
	HARDWARE_TEST_13, /*£¨0x0C£©LINE1 PULLUP PULLDOWN TEST*/  
	
	
	HARDWARE_TEST_14 , /*£¨0x0D£©JMUX0   TEST*/
	HARDWARE_TEST_15, /*£¨0x0E£©JMUX1  TEST*/
	
	
	HARDWARE_TEST_16 , /*£¨0x0F£©J1850   TEST*/
	HARDWARE_TEST_17, /*£¨0x10£©J1850  TEST*/
	HARDWARE_TEST_18 , /*£¨0x11£©J1850   TEST*/
		
	
}hardwaretest;


typedef enum
{
	HARDWARETEST_VCC_IN = 0x0000, /*£¨0x00£©VCC_IN TEST*/
	
	HARDWARETEST_VCC_PROG_9V  = 0x0100 ,   /*£¨0x01£©VCC_PROG TEST*/
	HARDWARETEST_VCC_PROG_12V = 0x0101,   /*£¨0x01£©VCC_PROG TEST*/
	HARDWARETEST_VCC_PROG_20V = 0x0102,   /*£¨0x01£©VCC_PROG TEST*/
	
	HARDWARETEST_VCC_DRV_5V= 0x0200 ,  /*£¨0x02£©VCC_DRV TEST*/
	HARDWARETEST_VCC_DRV_8V= 0x0201 ,  /*£¨0x02£©VCC_DRV TEST*/
	HARDWARETEST_VCC_DRV_12V= 0x0202 ,  /*£¨0x02£©VCC_DRV TEST*/
	
		
	HARDWARETEST_BUS1_CMP_V_2P1= 0x0300 , /*£¨0x03£©BUS1_CMP_V TEST*/
	HARDWARETEST_BUS1_CMP_V_2P2= 0x0301 , /*£¨0x04£©BUS1_CMP_V TEST*/

	HARDWARETEST_VCC_DRV2_5V= 0x0400 , /*£¨0x03£©VCC_DRV2 TEST*/
	HARDWARETEST_VCC_DRV2_8V=0x0401 , /*£¨0x04£©VCC_DRV2 TEST*/
	

	HARDWARETEST_J1850_P_OUT_HIGH = 0x0500, /*£¨0x06£©J1850_P TEST*/
	HARDWARETEST_J1850_P_OUT_LOW = 0x0501, /*£¨0x06£©J1850_P TEST*/
	

	HARDWARETEST_J1850_M_OUT_HIGH = 0x0600, /*£¨0x06£©J1850_M TEST*/
	HARDWARETEST_J1850_M_OUT_LOW  = 0x0601, /*£¨0x06£©J1850_M TEST*/

	HARDWARETEST_J1850_P_RX_HIGH = 0x0700, /*£¨0x06£©J1850_M TEST*/
	HARDWARETEST_J1850_P_RX_LOW  = 0x0701, /*£¨0x06£©J1850_M TEST*/


	HARDWARETEST_DB26_PIN1_HIGH = 0x0800, /*£¨0x05£©DB26_PIN1 TEST*/
	HARDWARETEST_DB26_PIN1_LOW = 0x0801, /*£¨0x05£©DB26_PIN1 TEST*/
	

	HARDWARETEST_LINE0_OUT_HIGH = 0x0900, /*£¨0x06£©LINE0 TEST*/
	HARDWARETEST_LINE0_OUT_LOW = 0x0901, /*£¨0x06£©LINE0 TEST*/
	HARDWARETEST_LINE0_IN_HIGH = 0x0902, /*£¨0x06£©LINE0 TEST*/
	HARDWARETEST_LINE0_IN_LOW = 0x0903, /*£¨0x06£©LINE0 TEST*/
	
	HARDWARETEST_LINE0_PULLDOWN_PULLUP_OUT_HIGH = 0x0904, /*£¨0x06£©LINE0 TEST*/	
	HARDWARETEST_LINE0_PULLDOWN_PULLUP_OUT_LOW = 0x0905, /*£¨0x06£©LINE0 TEST*/	
		
	HARDWARETEST_LINE1_OUT_HIGH = 0x0A00, /*£¨0x06£©LINE1 TEST*/
	HARDWARETEST_LINE1_OUT_LOW = 0x0A01, /*£¨0x06£©LINE1 TEST*/
	HARDWARETEST_LINE1_IN_HIGH = 0x0A02, /*£¨0x06£©LINE1 TEST*/
	HARDWARETEST_LINE1_IN_LOW = 0x0A03, /*£¨0x06£©LINE1 TEST*/
	
	HARDWARETEST_LINE1_PULLDOWN_PULLUP_OUT_HIGH = 0x0A04, /*£¨0x06£©LINE0 TEST*/
	HARDWARETEST_LINE1_PULLDOWN_PULLUP_OUT_LOW = 0x0A05, /*£¨0x06£©LINE0 TEST*/
	

	HARDWARETEST_can1_H = 0x0E00, /*£¨0x07£©can1 TEST*/
	HARDWARETEST_can1_L = 0x0E01, /*£¨0x08£©can1 TEST*/

	HARDWARETEST_can2_H = 0x0F00, /*£¨0x07£©can2 TEST*/
	HARDWARETEST_can2_L = 0x0F01, /*£¨0x08£©can2 TEST*/

	HARDWARETEST_WCFTCAN_H = 0x1000, /*£¨0x09£©WCFTCANH TEST*/
	HARDWARETEST_WCFTCAN_L = 0x1001, /*£¨0x0A£©WCFTCANH  TEST*/

	HARDWARETEST_DB26_PIN9_PWR = 0x1100, /*£¨0x05£©DB26_PIN9 TEST*/
	HARDWARETEST_DB26_PIN9_PG  = 0x1101, /*£¨0x05£©DB26_PIN9 TEST*/
	
	HARDWARETEST_DB26_PIN12_PWR = 0x1200, /*£¨0x05£©DB26_PIN12 TEST*/
	HARDWARETEST_DB26_PIN12_PG = 0x1201, /*£¨0x05£©DB26_PIN12 TEST*/	
		
	HARDWARETEST_DB26_PIN6_PWR_1 = 0x1300, /*£¨0x05£©DB26_PIN6 TEST*/
	HARDWARETEST_DB26_PIN6_PWR_0 = 0x1301, /*£¨0x05£©DB26_PIN6 TEST*/
			
	HARDWARETEST_DB26_PIN11_PWR_1 = 0x1400, /*£¨0x05£©DB26_PIN11 TEST*/
	HARDWARETEST_DB26_PIN11_PWR_0 = 0x1401, /*£¨0x05£©DB26_PIN11 TEST*/

	HARDWARETEST_DB26_PIN14_PWR_1 = 0x1500, /*£¨0x05£©DB26_PIN14 TEST*/
	HARDWARETEST_DB26_PIN14_PWR_0 = 0x1501, /*£¨0x05£©DB26_PIN14 TEST*/
		
	HARDWARETEST_DB26_PIN13_PWR_1 = 0x1600, /*£¨0x05£©DB26_PIN13 TEST*/
	HARDWARETEST_DB26_PIN13_PWR_0 = 0x1601, /*£¨0x05£©DB26_PIN13 TEST*/		
	
	HARDWARETEST_DB26_PIN15_PG_1 = 0x1700, /*£¨0x05£©DB26_PIN15 TEST*/
	HARDWARETEST_DB26_PIN15_PG_0 = 0x1701, /*£¨0x05£©DB26_PIN15 TEST*/
	

	HARDWARETEST_JMUX0_41 =0x1801, /*£¨0x05£©J41 TEST*///MUX 0
	HARDWARETEST_JMUX0_42 =0x1802, /*£¨0x05£©J42 TEST*/
	HARDWARETEST_JMUX0_43 =0x1803, /*£¨0x05£©J43 TEST*/
	HARDWARETEST_JMUX0_44 =0x1806, /*£¨0x05£©J44 TEST*/
	HARDWARETEST_JMUX0_45 =0x1807, /*£¨0x05£©J45 TEST*/
	HARDWARETEST_JMUX0_46 =0x1808, /*£¨0x05£©J46 TEST*/
	HARDWARETEST_JMUX0_47 =0x1809, /*£¨0x05£©J47 TEST*/
	HARDWARETEST_JMUX0_48 =0x180a, /*£¨0x05£©J48 TEST*/
	HARDWARETEST_JMUX0_49 =0x180b, /*£¨0x05£©J49 TEST*/
	HARDWARETEST_JMUX0_50 =0x180c, /*£¨0x05£©J50 TEST*/
	HARDWARETEST_JMUX0_51 =0x180d, /*£¨0x05£©J51 TEST*/
	HARDWARETEST_JMUX0_52 =0x180e, /*£¨0x05£©J52 TEST*/
	HARDWARETEST_JMUX0_53 =0x180f, /*£¨0x05£©J53 TEST*/
		
	HARDWARETEST_JMUX1_66 =0x1901, /*£¨0x05£©J66 TEST*/ //MUX 1
	HARDWARETEST_JMUX1_65 =0x1902, /*£¨0x05£©J65 TEST*/
	HARDWARETEST_JMUX1_64 =0x1903, /*£¨0x05£©J64 TEST*/
	HARDWARETEST_JMUX1_63 =0x1906, /*£¨0x05£©J63 TEST*/
	HARDWARETEST_JMUX1_62 =0x1907, /*£¨0x05£©J62 TEST*/
	HARDWARETEST_JMUX1_61 =0x1908, /*£¨0x05£©J61 TEST*/
	HARDWARETEST_JMUX1_60 =0x1909, /*£¨0x05£©J60 TEST*/
	HARDWARETEST_JMUX1_59 =0x190a, /*£¨0x05£©J59 TEST*/
	HARDWARETEST_JMUX1_98 =0x190b, /*£¨0x05£©J58 TEST*/
	HARDWARETEST_JMUX1_57 =0x190c, /*£¨0x05£©J57 TEST*/
	HARDWARETEST_JMUX1_56 =0x190d, /*£¨0x05£©J56 TEST*/
	HARDWARETEST_JMUX1_55 =0x190e, /*£¨0x05£©J55 TEST*/
	HARDWARETEST_JMUX1_54 =0x190f, /*£¨0x05£©J54 TEST*/

	HARDWARETEST_WIFI =0x1A00, /*£¨0x05£©WIFI TEST*/
	HARDWARETEST_BLUE= 0x1B00, /*£¨0x05£©BLUE TEST*/
	HARDWARETEST_RJ45= 0x1C00, /*£¨0x05£©RJ45 TEST*/
	HARDWARETEST_DDR= 0x1D00, /*£¨0x05£©RJ45 TEST*/
	HARDWARETEST_FLASH= 0x1E00, /*£¨0x05£©RJ45 TEST*/
	HARDWARETEST_ENCODE= 0x1F00, /*£¨0x05£©RJ45 TEST*/
	HARDWARETEST_LED= 0x2000, /*£¨0x05£©RJ45 TEST*/
	
}hardwaretestcmd;


#define JC_HIGH 0X01
#define JC_LOW  0X00

#define M_JC00 0
#define M_JC01 1
#define M_JC02 2
#define M_JC03 3
#define M_JC04 4
#define M_JC05 5
#define M_JC06 6
#define M_JC07 7
#define M_JC08 8
#define M_JC09 9
#define M_JC10 10
#define M_JC11 11
#define M_JC12 12
#define M_JC13 13
#define M_JC14 14
#define M_JC15 15
#define M_JC16 16
#define M_JC17 17
#define M_JC18 18
#define M_JC19 19
#define M_JC20 20
#define M_JC21 21
#define M_JC22 22
#define M_JC23 23


#define BUS1_ADC_PA21               AFEC_CHANNEL_1 //afec 0
#define BUS1_ADC_PA19               AFEC_CHANNEL_8 //afec 0

#define J1850_DRV_ADC_PC30          AFEC_CHANNEL_5 //afec 1

#define BUS1_ADC_PC31               AFEC_CHANNEL_6 //afec 1



#define BUS1_PWR_8V_CTL    M_JC20
#define BUS1_PWR_VCCIN_CTL M_JC16

#define DWFTCAN_WAKE M_JC17


#define J1850_8V_CTRL	M_JC19

#define JC10_BUS1_DRV_CMPV_ADC_CTRL  M_JC10
#define JC15_J1850_DRV_ADC_DB26PIN1_ADC_CTRL M_JC15


#define JC9_DB26_PIN1_PD  M_JC09

#define  JC0_LINE0_PU  M_JC00
#define  JC2_LINE0_PD  M_JC02

#define  JC8_LINE1_PU  M_JC08
#define  JC3_LINE1_PD  M_JC03

#define  JC4_CAN_SW0 M_JC04
#define  JC5_CAN_SW1 M_JC05

#define JC11_BUS1_LINE0_LINE1_ADC_CTRL M_JC11

#ifdef MINI_HW_001
#define  MCU_TXD_FPGA_PD19  PIO_PD19_IDX
#endif

#ifdef MINI_HW_002
#define  MCU_TXD_FPGA_PD19  PIO_PB4_IDX
#endif

#define  MCU_RXD_FPGA_PD18  PIO_PD18_IDX

#define GPIO_TXD0   PIO_PC16_IDX
#ifdef MINI_HW_001
#define GPIO_RXD0   PIO_PD28_IDX
#endif
#ifdef MINI_HW_002
#define GPIO_RXD0   PIO_PA21_IDX
#define DAT_LIGHT0_JC15 PIO_PD19_IDX
#endif
#define GPIO_TXD1   PIO_PA22_IDX
#define GPIO_RXD1   PIO_PD30_IDX


#define LINE0_TEST   PIO_PC17_IDX
#define LINE1_TEST   PIO_PC19_IDX


#define BUS1_CAN_60R_CTRL M_JC18
#define JC1_BUS3_CAN_SW M_JC01
///////////////////////////////////////

//VCC_PROG
#define  DB26_PIN9_PG_FPGA   PIO_PA9_IDX
#define  DB26_PIN9_PWR_FPGA  PIO_PA10_IDX

#define  DB26_PIN12_PG_FPGA   PIO_PA7_IDX
#define  DB26_PIN12_PWR_FPGA  PIO_PA8_IDX


#define  DB26_PIN6_PWR	 PIO_PA2_IDX
#define  DB26_PIN11_PWR  PIO_PA1_IDX
#define  DB26_PIN14_PWR  PIO_PA0_IDX
#define  DB26_PIN13_PWR  PIO_PA3_IDX

#define  DB26_PIN15_PG  PIO_PA6_IDX

#define  J1850_M_TXD PIO_PD10_IDX
#define  J1850_P_TXD PIO_PD11_IDX

#define  J1850_M_PU M_JC14
#define  J1850_P_PD M_JC12
#define  J1850_P_RXD PIO_PC11_IDX

void hardware_test(uint8_t Protocol, uint16_t wParam, uint8_t *lParam);
void tx_CTRL_EN_channel2_AllOff(void);
void tx_CTRL_EN_channel2(uint8_t JC_CHANNEL,uint8_t VALUE);
void tx_PINSEL_channel2(uint8_t JA_LINE0_Pin_Num,uint8_t JB_LINE1_Pin_Num);

extern uint32_t HARDWARETEST_VCC_PROG_SERVICE_FUNCTION(float Output_Volt);
extern void HARDWARETEST_VCC_DRV_SERVICE_FUNCTION_NO_CLOSE(uint32_t VCC_DRV);
extern uint32_t HARDWARETEST_BUS1_CMP_V_SERVICE_GET_FUNCTION(void);
extern  void Db26_PgAndPwrPinDirInit(void);
extern  void Db26_PgAndPwrPinLevelInit(void);
extern uint32_t HARDWARETEST_VCC_IN_SERVICE_FUNCTION(void);
extern uint32_t HARDWARETEST_JMUX0_SERVICE_FUNCTION(unsigned int JD_INDEX);
extern uint32_t Bus1AdcPc31ValueGet(void);

 extern void AdcPinInit(ioport_pin_t pin,  ioport_direction dir,  ioport_mode_t mode);

extern void Bus1AdcPc31Init(void);

extern void Bus1CmpAndDrvAdcInit(void);

extern uint32_t Bus1CmpAndDrvAdcValueGet(void);



#endif // _HARDWARE_TEST_H_