#ifndef __INTERFACE_H
#define __INTERFACE_H

#include "hardwaretest.h"
#include "hardware.h"
#include "gpio.h"
#include "user_config.h"

#define GET_TIMER2_CH0_CV	 T_delay_us_get_count()
#define RESET_TIMER2_CH0_CV	 T_delay_us_clear_count()
#define GET_TIMER2_CH0_COVFS T_delay_us_get_status()


//****************VCC¿ØÖÆ**************//
#define BUS_5V_CTL_PIN    GPIO_Pin_7
#define BUS_5V_CTL_PORT   GPIOD
#define BUS_5V_CTL_CLK    RCC_AHB1Periph_GPIOD  

#define BUS_8V_CTL_PIN  GPIO_Pin_10
#define BUS_8V_CTL_PORT  GPIOD
#define BUS_8V_CTL_CLK   RCC_AHB1Periph_GPIOD 


#define BUS_VCCIN_CTL_PIN  GPIO_Pin_11
#define BUS_VCCIN_CTL_PORT  GPIOD
#define BUS_VCCIN_CTL_CLK   RCC_AHB1Periph_GPIOD 

//****************PIN SELECT¿ØÖÆ**************//
#define PB14_MUX_CTL_PIN    GPIO_Pin_14
#define PB14_MUX_CTL_PORT   GPIOB
#define PB14_MUX_CTL_CLK    RCC_AHB1Periph_GPIOB  

#define PC4_LINE_CTL_PIN    GPIO_Pin_4
#define PC4_LINE_CTL_PORT   GPIOC
#define PC4_LINE_CTLCLK    RCC_AHB1Periph_GPIOC  

//////////////////////////////////////////////////////////////
#define PC5_W_CAN_CTL_PIN    GPIO_Pin_5
#define PC5_W_CAN_CTL_PORT   GPIOC
#define PC5_W_CAN_CTL_CLK    RCC_AHB1Periph_GPIOC  


#define PA9_CAN_MODE_PIN    GPIO_Pin_9
#define PA9_CAN_MODE_PORT   GPIOA
#define PA9_CAN_MODE_CLK    RCC_AHB1Periph_GPIOA  


#define PA10_CAN_MODE_PIN    GPIO_Pin_10
#define PA10_CAN_MODE_PORT   GPIOA
#define PA10_CAN_MODE_CLK    RCC_AHB1Periph_GPIOA  

#ifndef  M_NEWHARDWARE
#define PC1_CAN0_CTL1_PIN    GPIO_Pin_1
#define PC1_CAN0_CTL1_PORT   GPIOC
#define PC1_CAN0_CTL1_CTLCLK    RCC_AHB1Periph_GPIOC  

#define PC2_CAN0_CTL2_PIN    GPIO_Pin_2
#define PC2_CAN0_CTL2_PORT    GPIOC
#define PC2_CAN0_CTL2_CTLCLK    RCC_AHB1Periph_GPIOC
#else
#define PC1_CAN0_CTL1_PIN    GPIO_Pin_14
#define PC1_CAN0_CTL1_PORT   GPIOB
#define PC1_CAN0_CTL1_CTLCLK    RCC_AHB1Periph_GPIOB  

#define PC2_CAN0_CTL2_PIN    GPIO_Pin_15
#define PC2_CAN0_CTL2_PORT    GPIOB
#define PC2_CAN0_CTL2_CTLCLK    RCC_AHB1Periph_GPIOB
#endif

///////////////////////////////////////////////////////////

#define  PD13_LINE0_PU_PORT   GPIOD
#define  PD13_LINE0_PU_PIN    GPIO_Pin_13
#define  PD13_LINE0_PU_CLK   RCC_AHB1Periph_GPIOD

#define  PD12_LINE0_PD_PORT  GPIOD 
#define  PD12_LINE0_PD_PIN   GPIO_Pin_12
#define  PD12_LINE0_PD_CLK   RCC_AHB1Periph_GPIOD


#define  PD15_LINE1_PU_PORT  GPIOD 
#define  PD15_LINE1_PU_PIN   GPIO_Pin_15
#define  PD15_LINE1_PU_CLK   RCC_AHB1Periph_GPIOD

#define  PD14_LINE1_PD_PORT  GPIOD 
#define  PD14_LINE1_PD_PIN   GPIO_Pin_14
#define  PD14_LINE1_PD_CLK   RCC_AHB1Periph_GPIOD



#define SetEclk 
#define HardVersion   0x03//minivci ad
 
#define delay_ms vTaskDelay 
#define DelayNus T_delay_us 
#define DelayNms vTaskDelay 
#define hal_HW_Delay_us T_delay_us
 
#define  E70B_vTaskDelay_Modulus(x) (x)
////////////////////////////////////////////////////////////////////////////////////////////////////
#define J1850_RX0_PORT  GPIOD
#define J1850_RX0_PIN  GPIO_Pin_2

#define J1850_RX1_PORT  GPIOC
#define J1850_RX1_PIN  GPIO_Pin_7

#define GET_J1850_RX0_STATUS	   GPIO_ReadInputDataBit(J1850_RX0_PORT,J1850_RX0_PIN)		
#define GET_J1850_VPW_RX0_STATUS   GPIO_ReadInputDataBit(J1850_RX0_PORT,J1850_RX0_PIN)		
#define GET_J1850_VPW_RX1_STATUS   GPIO_ReadInputDataBit(J1850_RX0_PORT,J1850_RX0_PIN)	

#define hal_tc1_ch0_Init T_delay_ms_init
#define GET_KWP_RX_STATUS  		GET_J1850_RX0_STATUS
//#define GET_J1850_TIMER_COVFS T_delay_ms_get_status()
#define RESET_J1850_TIMER_CV  T_delay_us_clear_count()
#define J1850_TIMER_CV        T_delay_us_get_count()
#define BAUD_COUNTER_OFFSET			49

#define  TX_CTL_PORT GPIOC

#define TX00_CTL_PORT GPIOC
#define TX01_CTL_PORT GPIOC

#define TX10_CTL_PORT GPIOC
#define TX11_CTL_PORT GPIOC

#define TX00_CTL GPIO_Pin_12
#define TX01_CTL GPIO_Pin_9

#define TX10_CTL GPIO_Pin_6
#define TX11_CTL GPIO_Pin_8


#define RESET_J1850_TX0_PORT	{GPIO_SetBits  (TX00_CTL_PORT,TX00_CTL); \
	                             			 GPIO_ResetBits(TX01_CTL_PORT,TX01_CTL);\
								 GPIO_ResetBits(TX10_CTL_PORT,TX10_CTL); \
								 GPIO_SetBits   (TX11_CTL_PORT,TX11_CTL);}

#define SET_J1850_TX0_PORT		{ GPIO_ResetBits(TX00_CTL_PORT,TX00_CTL); \
								  GPIO_SetBits   (TX01_CTL_PORT,TX01_CTL);\
								  GPIO_SetBits   (TX10_CTL_PORT,TX10_CTL); \
								  GPIO_ResetBits(TX11_CTL_PORT,TX11_CTL);}

  
#define SETNULL_J1850_TX0_PORT {  GPIO_ResetBits(TX00_CTL_PORT,TX00_CTL); \
								  GPIO_ResetBits   (TX01_CTL_PORT,TX01_CTL);\
								  GPIO_ResetBits   (TX10_CTL_PORT,TX10_CTL); \
								  GPIO_ResetBits(TX11_CTL_PORT,TX11_CTL);}



//vpw
#define  RESET_J1850_VPW_TX0_PORT {  GPIO_SetBits(TX00_CTL_PORT,TX00_CTL); \
									 GPIO_ResetBits   (TX01_CTL_PORT,TX01_CTL);\
									 GPIO_ResetBits   (TX10_CTL_PORT,TX10_CTL); \
									 GPIO_ResetBits(TX11_CTL_PORT,TX11_CTL);}

#define   SET_J1850_VPW_TX0_PORT {	GPIO_ResetBits(TX00_CTL_PORT,TX00_CTL); \
								    GPIO_SetBits   (TX01_CTL_PORT,TX01_CTL);\
								    GPIO_ResetBits   (TX10_CTL_PORT,TX10_CTL); \
								    GPIO_ResetBits(TX11_CTL_PORT,TX11_CTL);}


#define SETNULL_J1850_VPW_TX0_PORT	 {GPIO_ResetBits(TX00_CTL_PORT,TX00_CTL); \
								      GPIO_ResetBits   (TX01_CTL_PORT,TX01_CTL);\
								      GPIO_ResetBits   (TX10_CTL_PORT,TX10_CTL); \
								      GPIO_ResetBits(TX11_CTL_PORT,TX11_CTL);}



///////////////////////////////////////////////////////////////////////////////////////////////////////////

#define KWP_LINE0_SET_0			{GPIO_SetBits   (TX01_CTL_PORT,TX01_CTL);GPIO_ResetBits(TX00_CTL_PORT,TX00_CTL);}
#define KWP_LINE0_SET_1			{GPIO_ResetBits(TX01_CTL_PORT,TX01_CTL);GPIO_SetBits    (TX00_CTL_PORT,TX00_CTL);}

#define KWP_LINE0_SET_NULL		{GPIO_ResetBits(TX01_CTL_PORT,TX01_CTL);GPIO_ResetBits(TX00_CTL_PORT,TX00_CTL);}
#define GET_KWP_LINE0_STATUS	GPIO_ReadInputDataBit(J1850_RX0_PORT,J1850_RX0_PIN)


#define KWP_LINE1_SET_0			{GPIO_SetBits   (TX11_CTL_PORT,TX11_CTL);GPIO_ResetBits(TX10_CTL_PORT,TX10_CTL);}
#define KWP_LINE1_SET_1			{GPIO_SetBits   (TX11_CTL_PORT,TX11_CTL);GPIO_ResetBits(TX10_CTL_PORT,TX10_CTL);}
#define KWP_LINE1_SET_NULL		      {GPIO_ResetBits(TX11_CTL_PORT,TX11_CTL);GPIO_ResetBits(TX10_CTL_PORT,TX10_CTL);}

#define GET_KWP_LINE1_STATUS	GPIO_ReadInputDataBit(J1850_RX1_PORT,J1850_RX1_PIN)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

								  
#define RESET_J1850_VPW_TX1_PORT{ GPIO_ResetBits (TX00_CTL_PORT,TX00_CTL); \
								  GPIO_SetBits   (TX01_CTL_PORT,TX01_CTL);\
								  GPIO_SetBits   (TX10_CTL_PORT,TX10_CTL); \
								  GPIO_ResetBits (TX11_CTL_PORT,TX11_CTL);}

#define SET_J1850_VPW_TX1_PORT  { GPIO_ResetBits(TX00_CTL_PORT,TX00_CTL); \
								  GPIO_SetBits   (TX01_CTL_PORT,TX01_CTL);\
								  GPIO_SetBits   (TX10_CTL_PORT,TX10_CTL); \
								  GPIO_ResetBits(TX11_CTL_PORT,TX11_CTL);}




//****************LINE0¿ØÖÆ***************//


#define SET_TX0_PRE_IO_HIGH KWP_LINE0_SET_1
#define SET_TX0_PRE_IO_LOW  KWP_LINE0_SET_0
 
#define LINE0_OR_LINE1_LOW   KWP_LINE0_SET_0
#define LINE0_OR_LINE1_HIGH  KWP_LINE0_SET_1
 
#define GET_RX0_STATUS    	 arch_ioport_get_pin_level(GPIOD,GPIO_Pin_2)
#define GET_LINE0_OR_LINE1   arch_ioport_get_pin_level(GPIOC,GPIO_Pin_7)


#define   MUX_CTL_PORT  GPIOB
#define   MUX_CTL_PIN  GPIO_Pin_14


#define   LIN_CTL_PORT  GPIOC
#define   LIN_CTL_PIN  GPIO_Pin_4

//////////////////////////////////////


#define  DG1_PORT   GPIOE
#define  DG1_CLK    RCC_AHB1Periph_GPIOE

#define  DG1_A0_PIN  GPIO_Pin_8
#define  DG1_A1_PIN  GPIO_Pin_9
#define  DG1_A2_PIN  GPIO_Pin_10
#define  DG1_EN_PIN  GPIO_Pin_11

//////////////////////////////////////

#define   PC4_MUX1_CTL_PORT  GPIOC
#define   PC4_MUX1_CTL_PIN  GPIO_Pin_4
//////////////////////////////////////

///////////////VCC ///////////////////
#define   BUS_CTL_PORT  GPIOD

#define   BUS_5V_CTL_PIN  GPIO_Pin_7
#define   BUS_8V_CTL_PIN  GPIO_Pin_10
#define   BUS_VCCIN_CTL_PIN  GPIO_Pin_11


///////////////ADC/////////////////////
#define   BUS_DRV_ADC_PORT  GPIOC
#define   BUS_DRV_ADC_PIN  GPIO_Pin_0

#define   BUS_CMP_ADC_PORT  GPIOA
#define   BUS_CMP_ADC_PIN  GPIO_Pin_2

#define   MUX0_ADC_PORT  GPIOA
#define   MUX0_ADC_PIN  GPIO_Pin_0

#define  MUX1_ADC_PORT  GPIOA
#define   MUX1_ADC_PIN  GPIO_Pin_1


#define  BATTERY_ADC_PORT  GPIOC
#define   BATTERY_ADC_PIN  GPIO_Pin_1

typedef enum
{ 

	MUX0_ADC_ADC_CHANNEL= 0x00,//IN0
	MUX1_ADC_ADC_CHANNEL,//IN1
	BUS_CMP_ADC_CHANNEL,//IN2
	BUS_DRV_ADC_CHANNEL,//IN10
	PC1_CAN_H_ADC,//IN11
	PC2_CAN_L_ADC,//IN12
	PC3_VCCIN_ADC,//IN13

}ADC_CHANNEL;

///////////////LED/////////////////////
#define   LED_PORT  GPIOB

#define  RED_LED_PIN  GPIO_Pin_8
#define  GREEN_LED_PIN  GPIO_Pin_9
#define  BLUE_LED_PIN  GPIO_Pin_10

///////////////POWER EN  /////////////////////
#define GSM_PWR_PIN  GPIO_Pin_12
#define GSM_PWR_PORT  GPIOE
#define GSM_PWR_CLK   RCC_AHB1Periph_GPIOE  

///////////////GNSS POWER   /////////////////////

#define  GNSS_POWER_PORT  GPIOE
#define  GNSS_POWER_PIN  GPIO_Pin_11


///////////////2G POWER   /////////////////////
#define GSM_POWER_PIN    GPIO_Pin_1
#define GSM_POWER_PORT   GPIOB
#define GSM_POWER_CLK    RCC_AHB1Periph_GPIOB  


///////////////PWM   /////////////////////
#define  PWM_PORT  GPIOB
#define  PWM_PIN  GPIO_Pin_0


///////////////EC20  /////////////////////
#define PA15_RESET_EC20_PIN    GPIO_Pin_15
#define PA15_RESET_EC20_PORT   GPIOA
#define PA15_RESET_EC20_CLK    RCC_AHB1Periph_GPIOA 

#define PB1_ON_OFF_EC20_PIN    GPIO_Pin_1
#define PB1_ON_OFF_EC20_PORT   GPIOB
#define PB1_ON_OFF_EC20_CLK    RCC_AHB1Periph_GPIOB  

#define PB3_WAKEUP_IN_EC20_PIN    GPIO_Pin_3
#define PB3_WAKEUP_IN_EC20_PORT   GPIOB
#define PB3_WAKEUP_IN_EC20_CLK    RCC_AHB1Periph_GPIOB  

#define PA05_SLEEP_STATUS_EC20_PIN    GPIO_Pin_5
#define PA05_SLEEP_STATUS_EC20_PORT   GPIOA
#define PA05_SLEEP_STATUS_EC20_CLK    RCC_AHB1Periph_GPIOA  


#define PB15_VBAT_OPEN_PIN    GPIO_Pin_5
#define PB15_VBAT_OPEN_PORT   GPIOA
#define PB15_VBAT_OPEN_CLK    RCC_AHB1Periph_GPIOA  

#endif