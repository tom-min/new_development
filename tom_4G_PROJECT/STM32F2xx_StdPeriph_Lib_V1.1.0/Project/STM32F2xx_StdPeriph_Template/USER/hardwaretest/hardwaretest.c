//#include <asf.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "hardwaretest.h"
#include "ProtocolDrives.h"
#include "conf_PDU.h"
#include "FPGA_Drives.h"
 #include "interface.h"
#include "gpio.h"
#include "adc.h"

#include "PortAssign.h"
#include "conf_HW.h"

extern void printf_usb(const char *fmt,...);
extern void hal_init_pwm_CMP_Volt(void);
extern void hal_SET_CMP_Volt(float Volt);
extern void hal_spi_delay(void);

static FPGA_C_REG PDU_FPGA_C_REG2;
void hal_dac_init2(void);
uint8_t hal_set_ProgVolt2(float Output_Volt);
uint8_t hal_set_ProgVolt2_no_calib(float Output_Volt);
void J1850DrvAdcPc30Init(void);
uint32_t J1850DrvAdcPc30ValueGet(void);
void LinePinDirInit(void);
void ProVoltAdcInit(void);
uint32_t ProVoltAdcValueGet(void);//mv;
void VccInAdcInit(void);
uint32_t VccInAdcValueGet(void);

//void tx_CTRL_EN_channel2_AllOff(void);
void tx_CTRL_EN_channel22(uint8_t JC_CHANNEL,uint8_t VALUE);
void tx_CTRL_EN_channel2_write(void);
void tx_MODE_EN_channel2(uint8_t TXMODE,uint8_t RXMODE,uint8_t TX_REV,uint8_t RX_REV);

uint32_t HARDWARETEST_BUS1_CMP_V_SERVICE_FUNCTION(float Output_Volt);
	#if 0  //quwenqi2g
void hal_adc_init2( Afec *const afec,const enum afec_channel_num channel );
uint32_t hal_channel_get_adc_value2(Afec *const afec,uint16_t channel);
uint16_t hal_channel_get_adc_value_average2(Afec *const afec,uint16_t channel,uint8_t num);
#endif
//uint32_t HARDWARETEST_VCC_IN_SERVICE_FUNCTION(void);
//uint32_t HARDWARETEST_VCC_PROG_SERVICE_FUNCTION(float Output_Volt);
 
uint32_t HARDWARETEST_VCC_DRV_SERVICE_FUNCTION(uint32_t VCC_DRV);
//void HARDWARETEST_VCC_DRV_SERVICE_FUNCTION_NO_CLOSE(uint32_t VCC_DRV);
//uint32_t HARDWARETEST_BUS1_CMP_V_SERVICE_FUNCTION(float Output_Volt);
uint32_t HARDWARETEST_VCC_DRV2_SERVICE_FUNCTION(uint32_t VCC_DRV2);
uint32_t HARDWARETEST_J1850_P_M_SERVICE_FUNCTION(uint32_t PinType, uint32_t DirType, uint32_t value);
uint32_t HARDWARETEST_LINE_SERVICE_FUNCTION(uint32_t PinType, uint32_t DirType, uint32_t value);
uint32_t HARDWARETEST_LINE_PULLUP_DOWN_SERVICE_FUNCTION(uint32_t PinType, uint32_t value);
uint32_t HARDWARETEST_DB26_PIN1_SERVICE_FUNCTION( uint32_t value);
uint32_t HARDWARETEST_can1_SERVICE_FUNCTION( uint32_t value);
uint32_t HARDWARETEST_can2_SERVICE_FUNCTION( uint32_t value);
uint32_t HARDWARETEST_DB26_PIN_PWR_PG(uint32_t Pinnumber,uint32_t value);
uint32_t HARDWARETEST_JMUX1_SERVICE_FUNCTION(unsigned int JD_INDEX);
//uint32_t HARDWARETEST_JMUX0_SERVICE_FUNCTION(unsigned int JD_INDEX);
 
uint32_t HARDWARETEST_WIFI_SERVICE_FUNCTION(void);
uint32_t HARDWARETEST_BLUE_SERVICE_FUNCTION(void);	


unsigned int   hardwaretestcmdresponsevaluerange[][2]=
{
	{10000,25000},//HARDWARETEST_VCC_IN  /  mv
	
	{8000,1000},  //HARDWARETEST_VCC_PROG_9V  /*（0x01）VCC_PROG TEST*/
	{11000,13000},//HARDWARETEST_VCC_PROG_12V   /*（0x01）VCC_PROG TEST*/
	{19000,21000},  //HARDWARETEST_VCC_PROG_20V = 0x0102,   /*（0x01）VCC_PROG TEST*/
	
	{4000,6000},//HARDWARETEST_VCC_DRV_5V= 0x0200 ,  /*（0x02）VCC_DRV TEST*/
	{7000,9000},//HARDWARETEST_VCC_DRV_8V= 0x0201 ,  /*（0x02）VCC_DRV TEST*/
	{11000,13000},//HARDWARETEST_VCC_DRV_12V= 0x0202 ,  /*（0x02）VCC_DRV TEST*/
	
		
	{2000,22000},//HARDWARETEST_BUS1_CMP_V_2P1= 0x0300 , /*（0x0300）BUS1_CMP_V TEST*/
	{2000,2000},//HARDWARETEST_BUS1_CMP_V_2P2= 0x0301 , /*（0x0301）BUS1_CMP_V TEST*/

	{4000,6000},//HARDWARETEST_VCC_DRV2_5V= 0x0400 , /*（0x0400）VCC_DRV2 TEST*/
	{7000,9000},//HARDWARETEST_VCC_DRV2_8V=0x0401 , /*（0x0401）VCC_DRV2 TEST*/
	

	{7000,9000},//HARDWARETEST_J1850_P_OUT_HIGH = 0x0500, /*（0x0500）J1850_P TEST*/
	{0000,600},//HARDWARETEST_J1850_P_OUT_LOW = 0x0501, /*（0x0501）J1850_P TEST*/
	

	{7000,9000},//HARDWARETEST_J1850_M_OUT_HIGH = 0x0600, /*（0x0600）J1850_M TEST*/
	{0000,600},//HARDWARETEST_J1850_M_OUT_LOW  = 0x0601, /*（0x0601）J1850_M TEST*/

	{7000,9000},//HARDWARETEST_J1850_P_RX_HIGH = 0x0700, /*（0x0700）J1850_M TEST*/
	{0000,600},//HARDWARETEST_J1850_P_RX_LOW  = 0x0701, /*（0x0701）J1850_M TEST*/


	{7000,9000},//HARDWARETEST_DB26_PIN1_HIGH = 0x0800, /*（0x0800）DB26_PIN1 TEST*/
	{0000,600},//HARDWARETEST_DB26_PIN1_LOW = 0x0801, /*（0x0801）DB26_PIN1 TEST*/
	

	{7000,9000},//HARDWARETEST_LINE0_OUT_HIGH = 0x0900, /*（0x0900）LINE0 TEST*/
	{0000,600},//HARDWARETEST_LINE0_OUT_LOW = 0x0901, /*（0x0901）LINE0 TEST*/
	{7000,9000},//HARDWARETEST_LINE0_IN_HIGH = 0x0902, /*（0x0902）LINE0 TEST*/
	{0000,9000},//HARDWARETEST_LINE0_IN_LOW = 0x0903, /*（0x0903）LINE0 TEST*/
	
	{2300,2600},//HARDWARETEST_LINE0_PULLDOWN_PULLUP_OUT_HIGH = 0x0904, /*（0x0904）LINE0 TEST*/	
	{0000,600},//HARDWARETEST_LINE0_PULLDOWN_PULLUP_OUT_LOW = 0x0905, /*（0x0905）LINE0 TEST*/	
		
	{7000,9000},//HARDWARETEST_LINE1_OUT_HIGH = 0x0A00, /*（0x0A00）LINE1 TEST*/
	{0000,600},//HARDWARETEST_LINE1_OUT_LOW = 0x0A01, /*（0x0A01）LINE1 TEST*/
	{7000,9000},//HARDWARETEST_LINE1_IN_HIGH = 0x0A02, /*（0x0A02）LINE1 TEST*/
	{0000,600},//HARDWARETEST_LINE1_IN_LOW = 0x0A03, /*（0x0A03）LINE1 TEST*/
	
	{2300,2600},//HARDWARETEST_LINE1_PULLDOWN_PULLUP_OUT_HIGH = 0x0A04, /*（0x0A04）LINE0 TEST*/
    
	{0000,600},//HARDWARETEST_LINE1_PULLDOWN_PULLUP_OUT_LOW = 0x0A05, /*（0x0A05）LINE0 TEST*/
	

	{2300,2600},//HARDWARETEST_can1_H = 0x0E00, /*（0x0E00）can1 TEST*/
	{2300,2600},//HARDWARETEST_can1_L = 0x0E01, /*（0x0E01）can1 TEST*/

	{2300,2600},//HARDWARETEST_can2_H = 0x0F00, /*（0x0F00）can2 TEST*/
	{2300,2600},//HARDWARETEST_can2_L = 0x0F01, /*（0x0F01）can2 TEST*/

	{2300,2600},//HARDWARETEST_WCFTCAN_H = 0x1000, /*（0x1000）WCFTCANH TEST*/
	{2300,2600},///HARDWARETEST_WCFTCAN_L = 0x1001, /*（0x1001）WCFTCANH  TEST*/

	{10000,25000},//HARDWARETEST_DB26_PIN9_PWR = 0x1100, /*（0x1100）DB26_PIN9 TEST*/
	{0000,600},//HARDWARETEST_DB26_PIN9_PG  = 0x1101, /*（0x1101）DB26_PIN9 TEST*/
	
	{10000,25000},//HARDWARETEST_DB26_PIN12_PWR = 0x1200, /*（0x1200）DB26_PIN12 TEST*/
	{0000,600},//HARDWARETEST_DB26_PIN12_PG = 0x1201, /*（0x1201）DB26_PIN12 TEST*/	
		
	{10000,25000},//HARDWARETEST_DB26_PIN6_PWR_1 = 0x1300, /*（0x1300）DB26_PIN6 TEST*/
	{0000,600},//HARDWARETEST_DB26_PIN6_PWR_0 = 0x1301, /*（0x1301）DB26_PIN6 TEST*/
			
	{10000,25000},//HARDWARETEST_DB26_PIN11_PWR_1 = 0x1400, /*（0x1400）DB26_PIN11 TEST*/
	{0000,600},//HARDWARETEST_DB26_PIN11_PWR_0 = 0x1401, /*（0x1401）DB26_PIN11 TEST*/

	{10000,25000},//HARDWARETEST_DB26_PIN14_PWR_1 = 0x1500, /*（0x1500）DB26_PIN14 TEST*/
	{0000,600},//HARDWARETEST_DB26_PIN14_PWR_0 = 0x1501, /*（0x1501）DB26_PIN14 TEST*/
		
	{10000,25000},//HARDWARETEST_DB26_PIN13_PWR_1 = 0x1600, /*（0x1600）DB26_PIN13 TEST*/
	{0000,600},//HARDWARETEST_DB26_PIN13_PWR_0 = 0x1601, /*（0x1601）DB26_PIN13 TEST*/		
	
	{10000,25000},//HARDWARETEST_DB26_PIN15_PG_1 = 0x1700, /*（0x05）DB26_PIN15 TEST*/
	{0000,600},//HARDWARETEST_DB26_PIN15_PG_0 = 0x1701, /*（0x05）DB26_PIN15 TEST*/
	

	{0000,0000},//HARDWARETEST_JMUX0_41 =0x1801, /*（0x1801）J41 TEST*///MUX 0
	{0000,0000},//HARDWARETEST_JMUX0_42 =0x1802, /*（0x1802）J42 TEST*/
	{0000,0000},//HARDWARETEST_JMUX0_43 =0x1803, /*（0x1803）J43 TEST*/
	{0000,0000},//HARDWARETEST_JMUX0_44 =0x1806, /*（0x1806）J44 TEST*/
	{0000,0000},////HARDWARETEST_JMUX0_45 =0x1807, /*（0x1807）J45 TEST*/
	{0000,0000},//HARDWARETEST_JMUX0_46 =0x1808, /*（0x1808）J46 TEST*/
	{0000,0000},//HARDWARETEST_JMUX0_47 =0x1809, /*（0x1809）J47 TEST*/
	{0000,0000},//RDWARETEST_JMUX0_48 =0x180a, /*（0x180a）J48 TEST*/
	{0000,0000},//RDWARETEST_JMUX0_49 =0x180b, /*（0x180b）J49 TEST*/
	{0000,0000},//RDWARETEST_JMUX0_50 =0x180c, /*（0x180c）J50 TEST*/
	{0000,0000},//RDWARETEST_JMUX0_51 =0x180d, /*（0x180d）J51 TEST*/
	{0000,0000},//RDWARETEST_JMUX0_52 =0x180e, /*（0x180e）J52 TEST*/
	{0000,0000},//RDWARETEST_JMUX0_53 =0x180f, /*（0x180f）J53 TEST*/
		
	{0000,0000},//RDWARETEST_JMUX1_66 =0x1901, /*（0x1901）J66 TEST*/ //MUX 1
	{0000,0000},//RDWARETEST_JMUX1_65 =0x1902, /*（0x1902）J65 TEST*/
	{0000,0000},//HARDWARETEST_JMUX1_64 =0x1903, /*（0x1903）J64 TEST*/
	{0000,0000},//HARDWARETEST_JMUX1_63 =0x1906, /*（0x1906）J63 TEST*/
	{0000,0000},//HARDWARETEST_JMUX1_62 =0x1907, /*（0x1906）J62 TEST*/
	{0000,0000},//HARDWARETEST_JMUX1_61 =0x1908, /*（0x1908）J61 TEST*/
	{0000,0000},//HARDWARETEST_JMUX1_60 =0x1909, /*（0x1909）J60 TEST*/
	{0000,0000},//HARDWARETEST_JMUX1_59 =0x190a, /*（0x190a）J59 TEST*/
	{0000,0000},//HARDWARETEST_JMUX1_98 =0x190b, /*（0x190b）J58 TEST*/
	{0000,0000},//HARDWARETEST_JMUX1_57 =0x190c, /*（0x190c）J57 TEST*/
	{0000,0000},//HARDWARETEST_JMUX1_56 =0x190d, /*（0x190d）J56 TEST*/
	{0000,0000},//HARDWARETEST_JMUX1_55 =0x190e, /*（0x190e）J55 TEST*/
	{0000,0000},//HARDWARETEST_JMUX1_54 =0x190f, /*（0x190f）J54 TEST*/

	{0000,0000},//HARDWARETEST_WIFI =0x1A00, /*（0x1A00）WIFI TEST*/
	{0000,0000},//HARDWARETEST_BLUE= 0x1B00, /*（0x1B00）BLUE TEST*/
	{0000,0000},//HARDWARETEST_RJ45= 0x1C00, /*（0x1C00）RJ45 TEST*/
	{0000,0000},//HARDWARETEST_DDR= 0x1D00, /*（0x1D00）RJ45 TEST*/
	{0000,0000},//HARDWARETEST_FLASH= 0x1E00, /*（0x1E00）RJ45 TEST*/
	{0000,0000},//HARDWARETEST_ENCODE= 0x1F00, /*（0x1F00）RJ45 TEST*/
	{0000,0000}//HARDWARETEST_LED= 0x2000, /*（0x2000）RJ45 TEST*/
	
};
 


//初始化DAC 用于编程电压  SCI协议等
//输出IO：PB13
void hal_dac_init2(void)
{
	#if 0  //quwenqi2g
	/*must code*/
	sysclk_enable_peripheral_clock(DACC_ID);
	/* Reset DACC registers */
	dacc_reset(DACC_BASE);
	/* Half word transfer mode */
	dacc_set_transfer_mode(DACC_BASE, 0);
	/* Enable output channel DACC_CHANNEL */
	dacc_enable_channel(DACC_BASE, DACC_CHANNEL);
	/* Set up analog current */
	dacc_set_analog_control(DACC_BASE, DACC_ANALOG_CONTROL);
	dacc_write_conversion_data(DACC_BASE,0,0);                //output 0V
	#endif
}

//Vout = 38.9 - 11.606*Vin
//Vin = (38.9-Vout)/11.606

//步进9.35mV * 11 反比关系
uint8_t hal_set_ProgVolt2(float Output_Volt)
{
	#if 0  //quwenqi2g
	float calc1,Input_Volt,cali_Volt,offset_Volt;
	uint16_t DAC_Vaule,DAC_Vaule_offset;
	uint16_t ADC_Vaule;
	
	Input_Volt = (38.9-Output_Volt)/11.606;
	calc1 = (Input_Volt/3.3)*4096;
	DAC_Vaule = (uint16_t)calc1;
	
	if (DAC_Vaule < 4096)
	{
		printf_usb("DAC_Vaule:%d\r\n",DAC_Vaule);  //test code
		dacc_write_conversion_data(DACC_BASE,DAC_Vaule,0);
		delay_ms(10);
		//校准
		ADC_Vaule = hal_channel_get_adc_value_average_Bubble(AFEC0,AFEC_CHANNEL_PROG_VOLT);
		printf_usb("ADC_Vaule:%d\r\n",ADC_Vaule);  //test code
		cali_Volt = (float)ADC_Vaule;
		cali_Volt = cali_Volt*3.3*11/4096;
		offset_Volt = cali_Volt - Output_Volt;
		if (offset_Volt > 0)
		{
			
			DAC_Vaule_offset = (uint16_t)(offset_Volt/0.00935);
			printf_usb("DAC_Vaule_offset1:%d\r\n",DAC_Vaule_offset); //test code
			DAC_Vaule = DAC_Vaule + DAC_Vaule_offset;
			printf_usb("DAC_Vaule:%d\r\n",DAC_Vaule);  //test code
			dacc_write_conversion_data(DACC_BASE,DAC_Vaule,0);
		}
		else
		{
			DAC_Vaule_offset = (uint16_t)(-offset_Volt/0.00935);
			printf_usb("DAC_Vaule_offset0:%d\r\n",DAC_Vaule_offset); //test code
			DAC_Vaule = DAC_Vaule - DAC_Vaule_offset;
			dacc_write_conversion_data(DACC_BASE,DAC_Vaule,0);
		}
		
		return true;
	}
	
	return false;
	#endif
}




void tx_PINSEL_DG408(uint8_t pin1,uint8_t pin2)
{
	uint8_t  pin;
	
	if(pin1)pin=pin1;
	
	else if (pin2)pin=pin2;
	

	if((pin >= 7) && (pin <= 13))
	{
		pin -= 7 ;
	}
	else if(pin == 15 )
	{
          	pin -= 8 ;
	}
	else  if(pin == 2)
	{
		GPIO_ResetBits(PC4_MUX1_CTL_PORT,PC4_MUX1_CTL_PIN);
		return;
	}

       GPIO_SetBits(DG1_PORT,DG1_EN_PIN);
	if((pin&0x01)==0)
	{
            GPIO_ResetBits(DG1_PORT,DG1_A0_PIN);
	}
	else
	{
             GPIO_SetBits(DG1_PORT,DG1_A0_PIN);
	}
	if((pin&0x02)==0)
	{
             GPIO_ResetBits(DG1_PORT,DG1_A1_PIN);
	}
	else
	{
             GPIO_SetBits(DG1_PORT,DG1_A1_PIN);
	}

	if((pin&0x04)==0)
	{
            GPIO_ResetBits(DG1_PORT,DG1_A2_PIN);
	}
	else
	{
            GPIO_SetBits(DG1_PORT,DG1_A2_PIN);
	}

}

void tx_PINSEL_channel2(uint8_t JA_LINE0_Pin_Num,uint8_t JB_LINE1_Pin_Num)  //LINE0-->CAN_H  LINE1-->CAN_L
{
	/////////////////////////////////////////////////////////////////////////////
	//printf("JA_LINE0_Pin_Num==%d\r\n",JA_LINE0_Pin_Num);
	//printf("JB_LINE1_Pin_Num==%d\r\n",JB_LINE1_Pin_Num);


     if((JA_LINE0_Pin_Num==3)&&(JB_LINE1_Pin_Num==11))
   {
	GPIO_SetBits(PC1_CAN0_CTL1_PORT,PC1_CAN0_CTL1_PIN);
	GPIO_SetBits(PC2_CAN0_CTL2_PORT,PC2_CAN0_CTL2_PIN);
	GPIO_SetBits( PC5_W_CAN_CTL_PORT, PC5_W_CAN_CTL_PIN);
	GPIO_ResetBits(DG1_PORT,DG1_EN_PIN);
	return;
   }
   else  if((JA_LINE0_Pin_Num==11)&&(JB_LINE1_Pin_Num==3))
   {
	GPIO_SetBits(PC1_CAN0_CTL1_PORT,PC1_CAN0_CTL1_PIN);
	GPIO_SetBits(PC2_CAN0_CTL2_PORT,PC2_CAN0_CTL2_PIN);
	GPIO_SetBits( PC5_W_CAN_CTL_PORT, PC5_W_CAN_CTL_PIN);
	GPIO_ResetBits(DG1_PORT,DG1_EN_PIN);
	return;
   }



	   ///////////////////////////////vpw pwm //////////////////////////////////////	
   if((JA_LINE0_Pin_Num==2)&&(JB_LINE1_Pin_Num==10))
   {
	 tx_PINSEL_DG408(JA_LINE0_Pin_Num,JA_LINE0_Pin_Num);
	 tx_PINSEL_DG408(JB_LINE1_Pin_Num,JB_LINE1_Pin_Num);
	 return;  
   }
	 
   ///////////////////////////////mux 0 mux 1//////////////////////////////////////	
   if((JA_LINE0_Pin_Num==7)||(JB_LINE1_Pin_Num==7))
   {
	 tx_PINSEL_DG408(JA_LINE0_Pin_Num,JB_LINE1_Pin_Num);
	 
	 return;  
   }
   else if((JA_LINE0_Pin_Num==8)||(JB_LINE1_Pin_Num==8))
   {
	 tx_PINSEL_DG408(JA_LINE0_Pin_Num,JB_LINE1_Pin_Num);
	 return; 
   }
   else  if((JA_LINE0_Pin_Num==9)||(JB_LINE1_Pin_Num==9))
   {
	tx_PINSEL_DG408(JA_LINE0_Pin_Num,JB_LINE1_Pin_Num);
	    return; 
   }
   else  if((JA_LINE0_Pin_Num==10)||(JB_LINE1_Pin_Num==10))
   {
	  tx_PINSEL_DG408(JA_LINE0_Pin_Num,JB_LINE1_Pin_Num);
	    return; 
   }
   else  if((JA_LINE0_Pin_Num==11)||(JB_LINE1_Pin_Num==11))
   {
	 tx_PINSEL_DG408(JA_LINE0_Pin_Num,JB_LINE1_Pin_Num);
	    return; 
   }
      else  if((JA_LINE0_Pin_Num==12)||(JB_LINE1_Pin_Num==12))
   {
	 tx_PINSEL_DG408(JA_LINE0_Pin_Num,JB_LINE1_Pin_Num);
	    return; 
   }
    else  if((JA_LINE0_Pin_Num==13)||(JB_LINE1_Pin_Num==13))
   {
	 tx_PINSEL_DG408(JA_LINE0_Pin_Num ,JB_LINE1_Pin_Num);
	    return; 
   }
   else  if((JA_LINE0_Pin_Num==15)||(JB_LINE1_Pin_Num==15))
   {
	tx_PINSEL_DG408(JA_LINE0_Pin_Num,JB_LINE1_Pin_Num);
	    return; 
   } 
   
   ///////////////////////////////can 1//////////////////////////////////////
   if((JA_LINE0_Pin_Num==1)||(JB_LINE1_Pin_Num==1))
   {
	   
	GPIO_ResetBits(PC1_CAN0_CTL1_PORT,PC1_CAN0_CTL1_PIN);
	GPIO_ResetBits(PC2_CAN0_CTL2_PORT,PC2_CAN0_CTL2_PIN);

	GPIO_ResetBits( PC5_W_CAN_CTL_PORT, PC5_W_CAN_CTL_PIN);

	GPIO_SetBits(PA9_CAN_MODE_PORT, PA9_CAN_MODE_PIN);
	GPIO_SetBits(PA10_CAN_MODE_PORT,PA10_CAN_MODE_PIN);
	return ;
   }

   //////////////////////////////////can2 //////////////////////////////////////
   if((JA_LINE0_Pin_Num==6)&&(JB_LINE1_Pin_Num==14))
   {
		GPIO_SetBits(PC1_CAN0_CTL1_PORT,PC1_CAN0_CTL1_PIN);
		GPIO_ResetBits(PC2_CAN0_CTL2_PORT,PC2_CAN0_CTL2_PIN);

		GPIO_SetBits( PC5_W_CAN_CTL_PORT, PC5_W_CAN_CTL_PIN);
   }
   else if((JA_LINE0_Pin_Num==14)&&(JB_LINE1_Pin_Num==6))
   {
	    GPIO_SetBits(PC1_CAN0_CTL1_PORT,PC1_CAN0_CTL1_PIN);
	    GPIO_ResetBits(PC2_CAN0_CTL2_PORT,PC2_CAN0_CTL2_PIN);
		
	   GPIO_SetBits(PC5_W_CAN_CTL_PORT, PC5_W_CAN_CTL_PIN);
   }
   else  if((JA_LINE0_Pin_Num==3)&&(JB_LINE1_Pin_Num==11))
   {
  	  GPIO_SetBits(PC1_CAN0_CTL1_PORT,PC1_CAN0_CTL1_PIN);
	  GPIO_SetBits(PC2_CAN0_CTL2_PORT,PC2_CAN0_CTL2_PIN);
		
	   GPIO_SetBits( PC5_W_CAN_CTL_PORT, PC5_W_CAN_CTL_PIN);
   }
   else  if((JA_LINE0_Pin_Num==11)&&(JB_LINE1_Pin_Num==3))
   {
	   GPIO_SetBits(PC1_CAN0_CTL1_PORT,PC1_CAN0_CTL1_PIN);
	   GPIO_SetBits(PC2_CAN0_CTL2_PORT,PC2_CAN0_CTL2_PIN);
		
	   GPIO_SetBits( PC5_W_CAN_CTL_PORT, PC5_W_CAN_CTL_PIN);
   }
   /////////////////////////////////////////////////////////////////////////////
    if((JA_LINE0_Pin_Num==DB26_PIN_NC)&&(JB_LINE1_Pin_Num==DB26_PIN_NC))
   {
	   
	   GPIO_ResetBits(PC4_LINE_CTL_PORT,PC4_LINE_CTL_PIN);
	   
	   GPIO_SetBits(PC5_W_CAN_CTL_PORT, PC5_W_CAN_CTL_PIN);
	   GPIO_ResetBits(PC1_CAN0_CTL1_PORT,PC1_CAN0_CTL1_PIN);
	   GPIO_ResetBits(DG1_PORT,DG1_EN_PIN);
   }
 
}


void tx_CTRL_EN_channel2(uint8_t JC_CHANNEL,uint8_t VALUE)
{
      return;
	if(JC_CHANNEL== JC0_LINE0_PU)//LINE 0  PULL UP
	{

		 GPIO_WriteBit(PD13_LINE0_PU_PORT,PD13_LINE0_PU_PIN,(BitAction)VALUE);
	}
	
	if(JC_CHANNEL== JC2_LINE0_PD)//LINE 0  PULL DOWN
	{
		 
		 GPIO_WriteBit(PD12_LINE0_PD_PORT,PD12_LINE0_PD_PIN,(BitAction)VALUE);
	}



	if(JC_CHANNEL==JC8_LINE1_PU )//LINE 1  PULL UP
	{
		  
		 GPIO_WriteBit(PD15_LINE1_PU_PORT,PD15_LINE1_PU_PIN,(BitAction)VALUE);
	}

	
	if(JC_CHANNEL==JC3_LINE1_PD)//LINE 1  PULL DOWN
	{
		 
		 GPIO_WriteBit(PD14_LINE1_PD_PORT,PD14_LINE1_PD_PIN,(BitAction)VALUE);
	}


}

void tx_CTRL_EN_channel22(uint8_t JC_CHANNEL,uint8_t VALUE)
{
}
void tx_CTRL_EN_channel2_write(void)
{
}


void tx_CTRL_EN_channel2_AllOff(void)
{ 
    //tx_CTRL_EN_channel2_write();
}

void tx_MODE_EN_channel2(uint8_t TXMODE,uint8_t RXMODE,uint8_t TX_REV,uint8_t RX_REV)
{
	uint8_t Control_Register[5];
}



void Db26_PgAndPwrPinDirInit(void)
{
	 #if 0
	arch_ioport_set_pin_dir(DB26_PIN9_PG_FPGA,IOPORT_DIR_OUTPUT);
	
	arch_ioport_set_pin_dir(DB26_PIN9_PWR_FPGA,IOPORT_DIR_OUTPUT);
		
	arch_ioport_set_pin_dir(DB26_PIN12_PG_FPGA,IOPORT_DIR_OUTPUT);
	arch_ioport_set_pin_dir(DB26_PIN12_PWR_FPGA,IOPORT_DIR_OUTPUT);
		
	arch_ioport_set_pin_dir(DB26_PIN6_PWR,IOPORT_DIR_OUTPUT);
	arch_ioport_set_pin_dir(DB26_PIN11_PWR,IOPORT_DIR_OUTPUT);
		
	arch_ioport_set_pin_dir(DB26_PIN14_PWR,IOPORT_DIR_OUTPUT);
	arch_ioport_set_pin_dir(DB26_PIN13_PWR,IOPORT_DIR_OUTPUT);
	#endif
	 
}
void Db26_PgAndPwrPinLevelInit(void)
{
	#if 0  //quwenqi2g
	//////////////////////////////////
	arch_ioport_set_pin_level(DB26_PIN9_PG_FPGA,0);
	arch_ioport_set_pin_level(DB26_PIN9_PWR_FPGA,0);
	arch_ioport_set_pin_level(DB26_PIN12_PG_FPGA,0);
	arch_ioport_set_pin_level(DB26_PIN12_PWR_FPGA,0);
					
	arch_ioport_set_pin_level(DB26_PIN6_PWR,0);
	arch_ioport_set_pin_level(DB26_PIN11_PWR,0);
	arch_ioport_set_pin_level(DB26_PIN14_PWR,1);
	arch_ioport_set_pin_level(DB26_PIN13_PWR,1);
	#endif
}
 
void LinePinDirInit(void)
{
	#if 0  //quwenqi2g
    arch_ioport_set_pin_dir(MCU_TXD_FPGA_PD19,IOPORT_DIR_OUTPUT);
    arch_ioport_set_pin_dir(GPIO_TXD0,IOPORT_DIR_OUTPUT);
    arch_ioport_set_pin_dir(GPIO_TXD1,IOPORT_DIR_OUTPUT);
    ioport_set_pin_mode(MCU_TXD_FPGA_PD19,IOPORT_MODE_PULLUP);
    ioport_set_pin_mode(GPIO_TXD0,IOPORT_MODE_PULLUP);
    ioport_set_pin_mode(GPIO_TXD1,IOPORT_MODE_PULLUP);

    /****/
    arch_ioport_set_pin_dir(LINE0_TEST,IOPORT_DIR_OUTPUT);
    arch_ioport_set_pin_dir(LINE1_TEST,IOPORT_DIR_OUTPUT);
    				
    ioport_set_pin_mode(LINE0_TEST,IOPORT_MODE_PULLUP);
    ioport_set_pin_mode(LINE1_TEST,IOPORT_MODE_PULLUP);
    				
    				
    arch_ioport_set_pin_dir(MCU_RXD_FPGA_PD18,IOPORT_DIR_INPUT);
    arch_ioport_set_pin_dir(GPIO_RXD0,IOPORT_DIR_INPUT);
    arch_ioport_set_pin_dir(GPIO_RXD1,IOPORT_DIR_INPUT);
	#endif
}
 typedef enum
{ 
  GPIO_Mode_IN1   = 0x00, /*!< GPIO Input Mode */
  GPIO_Mode_OUT1  = 0x01, /*!< GPIO Output Mode */
  GPIO_Mode_AF1   = 0x02, /*!< GPIO Alternate function Mode */
  GPIO_Mode_AN1  = 0x03  /*!< GPIO Analog Mode */
}GPIOMode_TypeDef1;

void AdcPinInit(ioport_pin_t pin, ioport_direction dir, ioport_mode_t mode)
{
	#if 0  //quwenqi2g
	arch_ioport_set_pin_dir(pin,dir);
	
	ioport_set_pin_mode(pin,mode);  
	#endif 
}
   
void VccInAdcInit(void)
{
	#if 0  //quwenqi2g
	hal_adc_init2(AFEC1,AFEC_CHANNEL_POTENTIOMETER);
	#endif
}
uint32_t VccInAdcValueGet(void)
{
	#if 0  //quwenqi2g
	uint32_t value=0;
	uint32_t i;
	for(i=0;i<2;i++)
	{
		value+=  hal_channel_get_adc_value2(AFEC1,AFEC_CHANNEL_POTENTIOMETER);	
	}

	value/=2;
	
	value= 3300*11*value/4096;
	return value;
	#endif
}


void ProVoltAdcInit(void)
{
	#if 0  //quwenqi2g
	hal_adc_init2(AFEC0,AFEC_CHANNEL_PROG_VOLT);	
	#endif         
}
uint32_t ProVoltAdcValueGet(void)//mv
{
	#if 0  //quwenqi2g
	uint32_t value=0;
	value=	hal_channel_get_adc_value2(AFEC0,AFEC_CHANNEL_PROG_VOLT);
	value= 3300*11*value/4096;

	return value;
		#endif
}


void Bus1CmpAndDrvAdcInit(void)
{
	#if 0  //quwenqi2g
	uint32_t value=0;
	#ifdef MINI_HW_001
		AdcPinInit(PIO_PA21_IDX,IOPORT_DIR_INPUT,IOPORT_MODE_PULLDOWN);
		hal_adc_init2(AFEC0,BUS1_ADC_PA21);
	#endif
	#ifdef MINI_HW_002
		AdcPinInit(PIO_PA19_IDX,IOPORT_DIR_INPUT,IOPORT_MODE_PULLDOWN);
		hal_adc_init2(AFEC0,BUS1_ADC_PA19);
	#endif
	#endif
}
uint32_t Bus1CmpAndDrvAdcValueGet(void)
{
	#if 0  //quwenqi2g
	uint32_t value=0;
	uint32_t i;
	for(i=0;i<32;i++)
	{
		#ifdef MINI_HW_001
		value+=	hal_channel_get_adc_value2(AFEC0,BUS1_ADC_PA21);
		#endif
		#ifdef MINI_HW_002
		value+=	hal_channel_get_adc_value2(AFEC0,BUS1_ADC_PA19);
		#endif
	}

	value/=32;
	value= 3300*11*value/4096;
	return value;	 
	#endif 
}

 

void Bus1AdcPc31Init(void)
{
	#if 0  //quwenqi2g
	hal_adc_init2(AFEC1,BUS1_ADC_PC31);	
	#endif
}
uint32_t Bus1AdcPc31ValueGet(void)
{
	#if 0  //quwenqi2g
	uint32_t value=0;
	uint32_t i;
	for(i=0;i<32;i++)
	{
		value+=hal_channel_get_adc_value2(AFEC1,BUS1_ADC_PC31);
	}
	value/=32;

	value= 3300*11*value/4096;
	return value;
	#endif
}
void J1850DrvAdcPc30Init(void)
{
	#if 0  //quwenqi2g
	hal_adc_init2(AFEC1,J1850_DRV_ADC_PC30);
	#endif	
}
uint32_t J1850DrvAdcPc30ValueGet(void)
{
	#if 0  //quwenqi2g
    uint32_t value=0;
	uint32_t i;
	for(i=0;i<32;i++)
	{
    value+=	hal_channel_get_adc_value2(AFEC1,J1850_DRV_ADC_PC30);
	}
	value/=32;
	
    value= 3300*11*value/4096;
		
    return value; 
	#endif		
		
}  
 
uint32_t HARDWARETEST_VCC_IN_SERVICE_FUNCTION(void)
{
	unsigned int data_voltage;
	VccInAdcInit();
	data_voltage = VccInAdcValueGet( );
	printf_usb("VCC_IN==%d mv",data_voltage);
	return  data_voltage;
}
uint32_t HARDWARETEST_VCC_PROG_SERVICE_FUNCTION(float Output_Volt)
{
	unsigned int data_voltage;
	Db26_PgAndPwrPinDirInit();
	Db26_PgAndPwrPinLevelInit();
#if 0  //quwenqi2g
	AdcPinInit(PIO_PB0_IDX,IOPORT_DIR_INPUT,IOPORT_MODE_PULLDOWN);
	ProVoltAdcInit(); 
	hal_dac_init();
	hal_set_ProgVolt2(Output_Volt);//20V
    if(Output_Volt == 9.0)
	{
		//hal_set_ProgVolt2(Output_Volt);//20V
		vTaskDelay(E70B_vTaskDelay_Modulus(20));
	}
	
	//hal_set_ProgVolt2_no_calib(Output_Volt); //test code

	
	data_voltage = ProVoltAdcValueGet();
					
	printf_usb("VCC_PROG 9v==%d mv",data_voltage);
	 #endif
	return  data_voltage;
}
 
uint32_t HARDWARETEST_VCC_DRV_SERVICE_FUNCTION(uint32_t VCC_DRV)
{
	unsigned int data_voltage;
	Bus1CmpAndDrvAdcInit();
	 #if 0  //quwenqi2g
	memset((uint8_t *)&PDU_FPGA_C_REG2.L32.reg,0,sizeof(PDU_FPGA_C_REG));   //clears sizeof(PDU_FPGA_C_REG) = 8
	tx_CTRL_EN_channel2_write();			
	tx_CTRL_EN_channel2(JC10_BUS1_DRV_CMPV_ADC_CTRL,JC_LOW);    //ADC CHANNEL CHOSSE
				
	if(VCC_DRV==5)
	{
		vTaskDelay(100);
		tx_CTRL_EN_channel2(BUS1_PWR_8V_CTL,JC_LOW);				//VCC CHANNEL CHOSSE
		tx_CTRL_EN_channel2(BUS1_PWR_VCCIN_CTL,JC_LOW);				//VCC CHANNEL CHOSSE
		tx_CTRL_EN_channel2_write();
	}
	else if(VCC_DRV==8)
	{
		tx_CTRL_EN_channel2(BUS1_PWR_8V_CTL,JC_HIGH);
		tx_CTRL_EN_channel2(BUS1_PWR_VCCIN_CTL,JC_LOW);				//VCC CHANNEL CHOSSE
	}
	else if(VCC_DRV==12)
	{
		tx_CTRL_EN_channel2(BUS1_PWR_8V_CTL,JC_LOW);
		tx_CTRL_EN_channel2(BUS1_PWR_VCCIN_CTL,JC_HIGH);				//VCC CHANNEL CHOSSE
	}
	tx_CTRL_EN_channel2_write();			

	vTaskDelay(E70B_vTaskDelay_Modulus(5));

	data_voltage = Bus1CmpAndDrvAdcValueGet();

	tx_CTRL_EN_channel2(BUS1_PWR_8V_CTL,JC_LOW);				//VCC CHANNEL CHOSSE
	tx_CTRL_EN_channel2(BUS1_PWR_VCCIN_CTL,JC_LOW);				//VCC CHANNEL CHOSSE
	tx_CTRL_EN_channel2_write();
	printf_usb("VCC_DRV %dV==%d mv",VCC_DRV,data_voltage);
			#endif	
	return  data_voltage;
}



void HARDWARETEST_VCC_DRV_SERVICE_FUNCTION_NO_CLOSE(uint32_t VCC_DRV)
{
	if(VCC_DRV==5) 
	{
		GPIO_ResetBits(BUS_5V_CTL_PORT,BUS_5V_CTL_PIN);
		GPIO_ResetBits(BUS_8V_CTL_PORT,BUS_8V_CTL_PIN);
		GPIO_ResetBits(BUS_VCCIN_CTL_PORT,BUS_VCCIN_CTL_PIN);
		
		GPIO_SetBits(BUS_5V_CTL_PORT,BUS_5V_CTL_PIN);
	}
	else if(VCC_DRV==8)  
	{
		GPIO_ResetBits(BUS_5V_CTL_PORT,BUS_5V_CTL_PIN);
		GPIO_ResetBits(BUS_8V_CTL_PORT,BUS_8V_CTL_PIN);
		GPIO_ResetBits(BUS_VCCIN_CTL_PORT,BUS_VCCIN_CTL_PIN);

		GPIO_SetBits(BUS_8V_CTL_PORT,BUS_8V_CTL_PIN);
	}
	else if(VCC_DRV==12) 
	{
		GPIO_ResetBits(BUS_5V_CTL_PORT,BUS_5V_CTL_PIN);
		GPIO_ResetBits(BUS_8V_CTL_PORT,BUS_8V_CTL_PIN);
		GPIO_ResetBits(BUS_VCCIN_CTL_PORT,BUS_VCCIN_CTL_PIN);
		
		GPIO_SetBits(BUS_VCCIN_CTL_PORT,BUS_VCCIN_CTL_PIN);
		
	}

 
}

uint32_t HARDWARETEST_BUS1_CMP_V_SERVICE_FUNCTION(float Output_Volt)
{	
	#if 0  //quwenqi2g
	unsigned int data_voltage;
	memset((uint8_t *)&PDU_FPGA_C_REG2.L32.reg,0,sizeof(PDU_FPGA_C_REG));   //clears sizeof(PDU_FPGA_C_REG) = 8
	tx_CTRL_EN_channel2(JC10_BUS1_DRV_CMPV_ADC_CTRL,JC_HIGH);   //ADC CHANNEL CHOSSE
	tx_CTRL_EN_channel2_write();
	
	Bus1CmpAndDrvAdcInit();
	
	hal_init_pwm_CMP_Volt();
	
	vTaskDelay(E70B_vTaskDelay_Modulus(20));
	
	//hal_SET_CMP_Volt(Output_Volt);//2//2v---9V
	data_voltage = Bus1CmpAndDrvAdcValueGet();

	printf_usb("BUS1_CMP_V %0.2f ==%d mv",Output_Volt,data_voltage);
	
	return  data_voltage;
#endif
}

uint32_t HARDWARETEST_BUS1_CMP_V_SERVICE_GET_FUNCTION(void)
{
	#if 0  //quwenqi2g
	unsigned int data_voltage;
	memset((uint8_t *)&PDU_FPGA_C_REG2.L32.reg,0,sizeof(PDU_FPGA_C_REG));   //clears sizeof(PDU_FPGA_C_REG) = 8
	tx_CTRL_EN_channel2(JC10_BUS1_DRV_CMPV_ADC_CTRL,JC_HIGH);   //ADC CHANNEL CHOSSE
	tx_CTRL_EN_channel2_write();
	
	data_voltage = Bus1CmpAndDrvAdcValueGet();

	printf_usb("BUS1_CMP_V ==%d mv",data_voltage);
	return  data_voltage;
#endif
}

uint32_t HARDWARETEST_VCC_DRV2_SERVICE_FUNCTION(uint32_t VCC_DRV2)
{
	#if 0  //quwenqi2g
	unsigned int data_voltage;
	AdcPinInit(PIO_PC30_IDX,IOPORT_DIR_INPUT,IOPORT_MODE_PULLDOWN);
	J1850DrvAdcPc30Init( );
	//delay_ms(10);
	//while(1)
	{
		//delay_ms(100);
		//printf_usb("PIO_PC30_IDX FLAG==%d  ",arch_ioport_get_pin_level(PIO_PC30_IDX));
	}

	memset((uint8_t *)&PDU_FPGA_C_REG2.L32.reg,0,sizeof(PDU_FPGA_C_REG));   //clears sizeof(PDU_FPGA_C_REG) = 8
				
	//tx_CTRL_EN_channel2(JC15_J1850_DRV_ADC_DB26PIN1_ADC_CTRL,JC_LOW);//J1850_DRV_ADC[ ADC CHANNEL]
	arch_ioport_set_pin_dir(DAT_LIGHT0_JC15,IOPORT_DIR_OUTPUT);
	arch_ioport_set_pin_level(DAT_LIGHT0_JC15,0);
	if(VCC_DRV2==5)
	{
		tx_CTRL_EN_channel2(J1850_8V_CTRL,JC_LOW);				//VCC CHANNEL CHOSSE 5v
	}
	else 
	{
		tx_CTRL_EN_channel2(J1850_8V_CTRL,JC_HIGH);				//VCC CHANNEL CHOSSE 8v
	}
	tx_CTRL_EN_channel2_write();				
	vTaskDelay(E70B_vTaskDelay_Modulus(5));
	data_voltage = J1850DrvAdcPc30ValueGet( );
	
	printf_usb("VCC_DRV2 %d V==%d mv",VCC_DRV2,data_voltage);
	
	return  data_voltage;
	#endif
}

uint32_t HARDWARETEST_J1850_P_M_SERVICE_FUNCTION(uint32_t PinType, uint32_t DirType, uint32_t value)
{
	unsigned int data_voltage;
	#if 0  //quwenqi2g
	arch_ioport_set_pin_dir(J1850_M_TXD,IOPORT_DIR_OUTPUT);
	arch_ioport_set_pin_dir(J1850_P_TXD,IOPORT_DIR_OUTPUT);
					
	arch_ioport_set_pin_dir(J1850_P_RXD,IOPORT_DIR_INPUT);
					
	AdcPinInit(PIO_PC31_IDX,IOPORT_DIR_INPUT,IOPORT_MODE_PULLDOWN);
	Bus1AdcPc31Init( );
					
					
	memset((uint8_t *)&PDU_FPGA_C_REG2.L32.reg,0,sizeof(PDU_FPGA_C_REG));   //clears sizeof(PDU_FPGA_C_REG) == 8
	tx_CTRL_EN_channel2(J1850_8V_CTRL,JC_HIGH);							//VCC CHANNEL CHOSSE //VCC_DRV == 8V
					
	tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_LOW);//LINE 0  PULL UP
	tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_LOW);//LINE 1  PULL UP
					
					
	tx_CTRL_EN_channel2(JC2_LINE0_PD,JC_LOW);//LINE 1  PULL DOWN
	tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_LOW);//LINE 0  PULL DOWN
					
	tx_CTRL_EN_channel2(JC4_CAN_SW0,JC_LOW);//LINE0 AND LINE1
	tx_CTRL_EN_channel2(JC5_CAN_SW1,JC_LOW);//MUX0 AND MUX1
					
					
	tx_CTRL_EN_channel2(J1850_M_PU,JC_HIGH);				//P+ PULL UP
	tx_CTRL_EN_channel2(J1850_P_PD,JC_HIGH);				//M- PULL DOWN
					
					
	tx_CTRL_EN_channel2(JC11_BUS1_LINE0_LINE1_ADC_CTRL,JC_HIGH);//MUX0[ ADC CHANNEL]
	tx_CTRL_EN_channel2_write();
	tx_PINSEL_channel2(DB26_PIN_2,DB26_PIN_10);

	if((PinType==1 )&&(DirType==1 ))// P --- out  
	{
		if(value==1) // out 1
		{
				arch_ioport_set_pin_level(J1850_M_TXD,0);
				arch_ioport_set_pin_level(J1850_P_TXD,1);
		}
		else   // out 0
		{
				arch_ioport_set_pin_level(J1850_M_TXD,0);
				arch_ioport_set_pin_level(J1850_P_TXD,0);
		}
		
		vTaskDelay(E70B_vTaskDelay_Modulus(5));
		data_voltage = Bus1AdcPc31ValueGet( );
		if(value==1)
		{
			printf_usb("J1850_P out high 8V==%d mv value=%d",data_voltage,value);
		}
			
			
		else if(value==0)
		{
			printf_usb("J1850_P out low 0V  ==%d mv value=%d",data_voltage,value);
		}
			
				
		arch_ioport_set_pin_level(J1850_M_TXD,0);
		arch_ioport_set_pin_level(J1850_P_TXD,0);
		return  data_voltage;
    }
	  

	else if((PinType==0 )&&(DirType==1 ))// M --- out
	{
		tx_CTRL_EN_channel2(JC11_BUS1_LINE0_LINE1_ADC_CTRL,JC_LOW);//MUX1[ ADC CHANNEL]
		tx_CTRL_EN_channel2_write();
		if(value==1) // out 1
		{
			arch_ioport_set_pin_level(J1850_M_TXD,0);
			arch_ioport_set_pin_level(J1850_P_TXD,0);
		}
		else   // out 0
		{
			arch_ioport_set_pin_level(J1850_M_TXD,1);
			arch_ioport_set_pin_level(J1850_P_TXD,0);
		}
			
		vTaskDelay(E70B_vTaskDelay_Modulus(5));
		data_voltage = Bus1AdcPc31ValueGet( );
			
		if(value==1)
		printf_usb("J1850_M out high 8V==%d mv",data_voltage);
		else if(value==0)
		printf_usb("J1850_M out low 0V==%d mv",data_voltage);
			
		arch_ioport_set_pin_level(J1850_M_TXD,0);
		arch_ioport_set_pin_level(J1850_P_TXD,0);
		return  data_voltage;
	}
		
	else if((PinType==1 )&&(DirType==0 ))// P --- INPUT
	{
		
			if(value==1) // out 1
			{
				arch_ioport_set_pin_level(J1850_M_TXD,0);
				arch_ioport_set_pin_level(J1850_P_TXD,1);
			}
			else   // out 0
			{
				arch_ioport_set_pin_level(J1850_M_TXD,0);
				arch_ioport_set_pin_level(J1850_P_TXD,0);
			}
			vTaskDelay(E70B_vTaskDelay_Modulus(5));
			data_voltage=arch_ioport_get_pin_level(J1850_P_RXD);
			if(value==1)
			printf_usb("J1850_P_RXD high RX==%d",data_voltage);
	        else if(value==0)
			printf_usb("J1850_P_RXD low RX==%d",data_voltage);
				
		return  data_voltage;
	}
	else return  data_voltage;
	#endif
}

uint32_t HARDWARETEST_LINE_SERVICE_FUNCTION(uint32_t PinType, uint32_t DirType, uint32_t value)
{
	
	unsigned int data_voltage;
	#if 0  //quwenqi2g
	AdcPinInit(PIO_PC31_IDX,IOPORT_DIR_INPUT,IOPORT_MODE_PULLDOWN);
	Bus1AdcPc31Init();

	memset((uint8_t *)&PDU_FPGA_C_REG2.L32.reg,0,sizeof(PDU_FPGA_C_REG));   //clears sizeof(PDU_FPGA_C_REG) == 8
	tx_CTRL_EN_channel2(BUS1_PWR_8V_CTL,JC_HIGH);							//VCC CHANNEL CHOSSE //VCC_DRV == 8V
				
	tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_HIGH);//LINE 0  PULL UP
	tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_HIGH);//LINE 1  PULL UP
				
	//tx_CTRL_EN_channel2(JC0_LINE0_PD,JC_HIGH);//LINE 0  PULL DOWN
	//tx_CTRL_EN_channel2(JC8_LINE1_PD,JC_HIGH);//LINE 1  PULL DOWN
				
	tx_CTRL_EN_channel2(JC4_CAN_SW0,JC_LOW);//LINE0 AND LINE1
	tx_CTRL_EN_channel2(JC5_CAN_SW1,JC_LOW);//MUX0 AND MUX1
	tx_CTRL_EN_channel2_write();	
			 
				
	tx_MODE_EN_channel2(0,0,1,1);
				
	/****/
	LinePinDirInit();
				
				
	arch_ioport_set_pin_level(LINE0_TEST,0);
	arch_ioport_set_pin_level(LINE1_TEST,0);
			
	hal_init_pwm_CMP_Volt();

	//hal_SET_CMP_Volt(2.2);//2//2v---9V
			

	//tx_PINSEL_channel2(DB26_PIN_7,DB26_PIN_13);//LINE0 JA LINE1 JB
	if((PinType==0)&&(DirType==0))	 //0--line0 0--output
	{
		tx_CTRL_EN_channel2(JC11_BUS1_LINE0_LINE1_ADC_CTRL,JC_HIGH);//MUX0[ ADC CHANNEL]
		tx_CTRL_EN_channel2_write();
			if(value==1)
			{
			arch_ioport_set_pin_level(MCU_TXD_FPGA_PD19,1);
			arch_ioport_set_pin_level(GPIO_TXD1,1);
			arch_ioport_set_pin_level(GPIO_TXD0,1);
			}
			else
			{
				arch_ioport_set_pin_level(MCU_TXD_FPGA_PD19,0);
				arch_ioport_set_pin_level(GPIO_TXD1,0);
				arch_ioport_set_pin_level(GPIO_TXD0,0);	
			}
			vTaskDelay(E70B_vTaskDelay_Modulus(5));
			data_voltage = Bus1AdcPc31ValueGet( );
			if(value==1)
			printf_usb("LINE0   output 8v ==%d",data_voltage);
			else if(value==0)
			printf_usb("LINE0   output 0v ==%d",data_voltage);
	}
			
	else if((PinType==1)&&(DirType==0))	 //1--line1 0--output
	{
		tx_CTRL_EN_channel2(JC11_BUS1_LINE0_LINE1_ADC_CTRL,JC_LOW);//MUX1[ ADC CHANNEL]
		tx_CTRL_EN_channel2_write();
		if(value==1)
		{
			arch_ioport_set_pin_level(MCU_TXD_FPGA_PD19,1);
			arch_ioport_set_pin_level(GPIO_TXD1,1);
			arch_ioport_set_pin_level(GPIO_TXD0,1);
		}
		else
		{
			arch_ioport_set_pin_level(MCU_TXD_FPGA_PD19,0);
			arch_ioport_set_pin_level(GPIO_TXD1,0);
			arch_ioport_set_pin_level(GPIO_TXD0,0);
		}
		vTaskDelay(E70B_vTaskDelay_Modulus(5));
		data_voltage = Bus1AdcPc31ValueGet( );
		if(value==1)
		printf_usb("LINE1   output 8v ==%d",data_voltage);
		else if(value==0)
		printf_usb("LINE1   output 0v ==%d",data_voltage);
	}
		
		
	//tx_PINSEL_channel2(DB26_PIN_7,DB26_PIN_13);//LINE0 JA LINE1 JB
	if((PinType==0)&&(DirType==1))	 //0--line0 1--input
	{
		tx_CTRL_EN_channel2(JC11_BUS1_LINE0_LINE1_ADC_CTRL,JC_HIGH);//MUX0[ ADC CHANNEL]
		tx_CTRL_EN_channel2_write();
		if(value==1)
		{
			arch_ioport_set_pin_level(MCU_TXD_FPGA_PD19,1);
			arch_ioport_set_pin_level(GPIO_TXD1,1);
			arch_ioport_set_pin_level(GPIO_TXD0,1);
		}
		else
		{
			arch_ioport_set_pin_level(MCU_TXD_FPGA_PD19,0);
			arch_ioport_set_pin_level(GPIO_TXD1,0);
			arch_ioport_set_pin_level(GPIO_TXD0,0);
		}
		vTaskDelay(E70B_vTaskDelay_Modulus(5));
		data_voltage = arch_ioport_get_pin_level(GPIO_RXD0);
		if(value==1)
		printf_usb("LINE0  GPIO_RXD1 high RX==%d",data_voltage);		
		else if(value==0)
		printf_usb("LINE0  GPIO_RXD1 low RX==%d",data_voltage);
	}
	else if((PinType==1)&&(DirType==1))	 //1--line1 1--input
	{
		tx_CTRL_EN_channel2(JC11_BUS1_LINE0_LINE1_ADC_CTRL,JC_LOW);//MUX1[ ADC CHANNEL]
		tx_CTRL_EN_channel2_write();
		if(value==1)
		{
			arch_ioport_set_pin_level(MCU_TXD_FPGA_PD19,1);
			arch_ioport_set_pin_level(GPIO_TXD1,1);
			arch_ioport_set_pin_level(GPIO_TXD0,1);
		}
		else
		{
			arch_ioport_set_pin_level(MCU_TXD_FPGA_PD19,0);
			arch_ioport_set_pin_level(GPIO_TXD1,0);
			arch_ioport_set_pin_level(GPIO_TXD0,0);
		}
		vTaskDelay(E70B_vTaskDelay_Modulus(5)); 
		data_voltage = arch_ioport_get_pin_level(GPIO_RXD1);
		if(value==1)
		printf_usb("LINE1  GPIO_RXD1 high RX==%d",data_voltage);
		else if(value==0)
		printf_usb("LINE1  GPIO_RXD1 low RX==%d",data_voltage);
	}
		#endif
		
    return  data_voltage;
}



uint32_t HARDWARETEST_LINE_PULLUP_DOWN_SERVICE_FUNCTION(uint32_t PinType, uint32_t value)
{
	
	unsigned int data_voltage;
	#if 0  //quwenqi2g
	AdcPinInit(PIO_PC31_IDX,IOPORT_DIR_INPUT,IOPORT_MODE_PULLDOWN);
	Bus1AdcPc31Init();

	memset((uint8_t *)&PDU_FPGA_C_REG2.L32.reg,0,sizeof(PDU_FPGA_C_REG));   //clears sizeof(PDU_FPGA_C_REG) == 8
	tx_CTRL_EN_channel2(BUS1_PWR_8V_CTL,JC_HIGH);							//VCC CHANNEL CHOSSE //VCC_DRV == 8V
	
	
	tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_HIGH);//LINE 0  PULL UP
	tx_CTRL_EN_channel2(JC2_LINE0_PD,JC_HIGH);//LINE 0  PULL DOWN
							
	tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_HIGH);//LINE 0  PULL UP
	tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_HIGH);//LINE 0  PULL DOWN
							
	
	tx_CTRL_EN_channel2(JC4_CAN_SW0,JC_LOW);//LINE0 AND LINE1
	tx_CTRL_EN_channel2(JC5_CAN_SW1,JC_LOW);//MUX0 AND MUX1
	tx_CTRL_EN_channel2_write();

	tx_MODE_EN_channel2(0,0,1,1);
	
	/****/
	LinePinDirInit();
	
	arch_ioport_set_pin_level(LINE0_TEST,0);
	arch_ioport_set_pin_level(LINE1_TEST,0);
	
	hal_init_pwm_CMP_Volt(); 
	//hal_SET_CMP_Volt(2.2);//2//2v---9V
	

	//tx_PINSEL_channel2(DB26_PIN_7,DB26_PIN_13);//LINE0 JA LINE1 JB
	if((PinType==0))	 //0--line0 0--output
	{
		tx_CTRL_EN_channel2(JC11_BUS1_LINE0_LINE1_ADC_CTRL,JC_HIGH);//MUX0[ ADC CHANNEL]
		tx_CTRL_EN_channel2_write();
		if(value==1)
		{
			arch_ioport_set_pin_level(MCU_TXD_FPGA_PD19,1);
			arch_ioport_set_pin_level(GPIO_TXD1,1);
			arch_ioport_set_pin_level(GPIO_TXD0,1);
		}
		else
		{
			arch_ioport_set_pin_level(MCU_TXD_FPGA_PD19,0);
			arch_ioport_set_pin_level(GPIO_TXD1,0);
			arch_ioport_set_pin_level(GPIO_TXD0,0);
		}
		vTaskDelay(E70B_vTaskDelay_Modulus(5)); 
		data_voltage = Bus1AdcPc31ValueGet( );
		
		if(value==1)
		printf_usb("LINE0  out high  ==%d",data_voltage);
		else if(value==0)
		printf_usb("LINE0  out low  ==%d",data_voltage);
		
	}
	
	else if((PinType==1))	 //1--line1 0--output
	{
		tx_CTRL_EN_channel2(JC11_BUS1_LINE0_LINE1_ADC_CTRL,JC_LOW);//MUX1[ ADC CHANNEL]
		tx_CTRL_EN_channel2_write();
		if(value==1)
		{
			arch_ioport_set_pin_level(MCU_TXD_FPGA_PD19,1);
			arch_ioport_set_pin_level(GPIO_TXD1,1);
			arch_ioport_set_pin_level(GPIO_TXD0,1);
		}
		else
		{
			arch_ioport_set_pin_level(MCU_TXD_FPGA_PD19,0);
			arch_ioport_set_pin_level(GPIO_TXD1,0);
			arch_ioport_set_pin_level(GPIO_TXD0,0);
		}
		vTaskDelay(E70B_vTaskDelay_Modulus(5)); 
		data_voltage = Bus1AdcPc31ValueGet( );
		if(value==1)
		printf_usb("LINE1  out high  ==%d",data_voltage);
		else if(value==0)
		printf_usb("LINE1  out low  ==%d",data_voltage);
		
	}
	#endif

	return  data_voltage;
}

uint32_t HARDWARETEST_DB26_PIN1_SERVICE_FUNCTION( uint32_t value)
{
	#if 0  //quwenqi2g
	unsigned int data_voltage;
	AdcPinInit(PIO_PC30_IDX,IOPORT_DIR_INPUT,IOPORT_MODE_PULLDOWN);
	J1850DrvAdcPc30Init( );
		
	memset((uint8_t *)&PDU_FPGA_C_REG2.L32.reg,0,sizeof(PDU_FPGA_C_REG));   //clears sizeof(PDU_FPGA_C_REG) = 8
				
	//tx_CTRL_EN_channel2(JC15_J1850_DRV_ADC_DB26PIN1_ADC_CTRL,JC_HIGH);//DB26_PIN1_ADC[ ADC CHANNEL]
	arch_ioport_set_pin_dir(DAT_LIGHT0_JC15,IOPORT_DIR_OUTPUT);
	arch_ioport_set_pin_level(DAT_LIGHT0_JC15,1);
				
	tx_PINSEL_channel2(DB26_PIN_1,DB26_PIN_1);//PIN1 TO MUX0 AND MUX1
				
	if(value == 0) 
	{
		tx_CTRL_EN_channel2(JC9_DB26_PIN1_PD,JC_HIGH); // OUT 0V
	}
								
	if(value == 8)
	{
		tx_CTRL_EN_channel2(JC9_DB26_PIN1_PD,JC_LOW);// OUT 8V
	}
				
	//***LINE0 OUT 1***/
	tx_CTRL_EN_channel2(BUS1_PWR_8V_CTL,JC_HIGH);		//VCC CHANNEL CHOSSE //VCC_DRV == 8V
				
	tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_HIGH);//LINE 0  PULL UP
	tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_HIGH);//LINE 1  PULL UP
				
	//tx_CTRL_EN_channel2(JC0_LINE0_PD,JC_HIGH);//LINE 0  PULL DOWN
	//tx_CTRL_EN_channel2(JC8_LINE1_PD,JC_HIGH);//LINE 1  PULL DOWN
				
	//tx_CTRL_EN_channel2(JC2_LINE0_PD,JC_LOW);//LINE 0  PULL DOWN
	//tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_LOW);//LINE 0  PULL DOWN

	tx_CTRL_EN_channel2(JC4_CAN_SW0,JC_LOW);//LINE0 AND LINE1
	tx_CTRL_EN_channel2(JC5_CAN_SW1,JC_LOW);//MUX0 AND MUX1
				
	tx_CTRL_EN_channel2(JC11_BUS1_LINE0_LINE1_ADC_CTRL,JC_HIGH);//MUX0[ ADC CHANNEL]
	tx_CTRL_EN_channel2_write();
	tx_MODE_EN_channel2(0,0,1,1);
				
	LinePinDirInit();

	arch_ioport_set_pin_level(LINE0_TEST,0);
	arch_ioport_set_pin_level(LINE1_TEST,0);

	arch_ioport_set_pin_level(MCU_TXD_FPGA_PD19,1);
	arch_ioport_set_pin_level(GPIO_TXD1,1);
	arch_ioport_set_pin_level(GPIO_TXD0,1);
	vTaskDelay(100);
	data_voltage = J1850DrvAdcPc30ValueGet( );
	printf_usb("DB26_PIN1 %d V==%d mv",value,data_voltage);
	return  data_voltage;
	#endif	
}


uint32_t HARDWARETEST_can1_SERVICE_FUNCTION( uint32_t value)
{
	#if 0  //quwenqi2g
	unsigned int data_voltage;
	AdcPinInit(PIO_PC31_IDX,IOPORT_DIR_INPUT,IOPORT_MODE_PULLDOWN);
	Bus1AdcPc31Init();
				
	memset((uint8_t *)&PDU_FPGA_C_REG2.L32.reg,0,sizeof(PDU_FPGA_C_REG));   //clears sizeof(PDU_FPGA_C_REG) = 8
				
	//tx_CTRL_EN_channel2(BUS1_PWR_8V_CTL,JC_HIGH);				//VCC CHANNEL CHOSSE //VCC_DRV= 8V
				
	tx_CTRL_EN_channel2(BUS1_CAN_60R_CTRL,JC_HIGH);//CAN_L AND CAN_H 66 LOAD
				
				
	tx_CTRL_EN_channel2(JC4_CAN_SW0,JC_HIGH);//CAN_L AND CAN_H
	tx_CTRL_EN_channel2(JC5_CAN_SW1,JC_LOW);//MUX0 AND MUX1
				
	if( value==0 ) 	
	{
		tx_CTRL_EN_channel2(JC11_BUS1_LINE0_LINE1_ADC_CTRL,JC_HIGH);//MUX0[ ADC CHANNEL]	
	}	
	
	else if ( value==1 ) 
	{
		tx_CTRL_EN_channel2(JC11_BUS1_LINE0_LINE1_ADC_CTRL,JC_LOW);//MUX1 [ ADC CHANNEL]
	}
	tx_CTRL_EN_channel2_write();
	vTaskDelay(5);
	data_voltage = Bus1AdcPc31ValueGet( );
	
			
	if(value==0)
	printf_usb("CAN H  2.5V==%d mv",data_voltage);
	else if(value==1)
	printf_usb("CAN L  2.5V==%d mv",data_voltage);
	return  data_voltage;	
	#endif
		
}
uint32_t HARDWARETEST_can2_SERVICE_FUNCTION( uint32_t value)
{
	#if 0  //quwenqi2g
	unsigned int data_voltage;
	AdcPinInit(PIO_PC31_IDX,IOPORT_DIR_INPUT,IOPORT_MODE_PULLDOWN);
	Bus1AdcPc31Init();
				
	memset((uint8_t *)&PDU_FPGA_C_REG2.L32.reg,0,sizeof(PDU_FPGA_C_REG));   //clears sizeof(PDU_FPGA_C_REG) = 8
				
	tx_CTRL_EN_channel2(JC4_CAN_SW0,JC_LOW);//LINE0 AND LINE1
	tx_CTRL_EN_channel2(JC5_CAN_SW1,JC_LOW);//MUX0 AND MUX1
				
				
	tx_CTRL_EN_channel2(JC1_BUS3_CAN_SW,JC_HIGH);//CAN2 CAN_L AND CAN_H ON
				
	if( value==0 ) 
	{
		tx_CTRL_EN_channel2(JC11_BUS1_LINE0_LINE1_ADC_CTRL,JC_HIGH);//MUX0[ ADC CHANNEL]//h
	}
				
	else if( value==1) 
	{
		tx_CTRL_EN_channel2(JC11_BUS1_LINE0_LINE1_ADC_CTRL,JC_LOW);//MUX1 [ ADC CHANNEL]//L
	}
		tx_CTRL_EN_channel2_write();
				
	tx_MODE_EN_channel2(0,0,1,0);

	LinePinDirInit();

	/****/
	arch_ioport_set_pin_level(LINE0_TEST,0);
	arch_ioport_set_pin_level(LINE1_TEST,0);

    tx_PINSEL_channel2(DB26_PIN_6,DB26_PIN_14);//LINE0 JA LINE1 JB
    tx_CTRL_EN_channel2_write();
    arch_ioport_set_pin_level(MCU_TXD_FPGA_PD19,1);
    arch_ioport_set_pin_level(GPIO_TXD1,1);
	arch_ioport_set_pin_level(GPIO_TXD0,1);

    vTaskDelay(E70B_vTaskDelay_Modulus(5));
    data_voltage = Bus1AdcPc31ValueGet( );

    if(value==0)
    printf_usb("CAN2 H  2.5V==%d mv",data_voltage);
    else if(value==1)
    printf_usb("CAN2 L  2.5V==%d mv",data_voltage);

    return  data_voltage;
	#endif	
}

uint32_t HARDWARETEST_DWFCAN_SERVICE_FUNCTION( uint32_t value)
{
	#if 0  //quwenqi2g
	unsigned int data_voltage;
	AdcPinInit(PIO_PC31_IDX,IOPORT_DIR_INPUT,IOPORT_MODE_PULLDOWN);
	Bus1AdcPc31Init();
				
	memset((uint8_t *)&PDU_FPGA_C_REG2.L32.reg,0,sizeof(PDU_FPGA_C_REG));   //clears sizeof(PDU_FPGA_C_REG) = 8
				
	tx_CTRL_EN_channel2(JC4_CAN_SW0,JC_LOW);//LINE0 AND LINE1

	tx_CTRL_EN_channel2(JC1_BUS3_CAN_SW,JC_LOW);//CAN0 CAN_L AND CAN_H OFF
	tx_CTRL_EN_channel2(JC4_CAN_SW0,JC_LOW );//CAN  CAN_L AND CAN_H OFF
	tx_CTRL_EN_channel2(JC5_CAN_SW1,JC_HIGH);//WCFT CAN_L AND CAN_H ON
	
	if( value==0) 
	{
		tx_CTRL_EN_channel2(JC11_BUS1_LINE0_LINE1_ADC_CTRL,JC_HIGH);//MUX0[ ADC CHANNEL]//h
	}
				
	else if( value==1) 
	{
		tx_CTRL_EN_channel2(JC11_BUS1_LINE0_LINE1_ADC_CTRL,JC_LOW);//MUX1 [ ADC CHANNEL]//L
	}
	
	tx_CTRL_EN_channel2_write();
				
	tx_MODE_EN_channel2(0,0,1,0);

	LinePinDirInit();

	/****/
	arch_ioport_set_pin_level(LINE0_TEST,0);
	arch_ioport_set_pin_level(LINE1_TEST,0);

	tx_PINSEL_channel2(DB26_PIN_6,DB26_PIN_14);//LINE0 JA LINE1 JB
	tx_CTRL_EN_channel2_write();
	arch_ioport_set_pin_level(MCU_TXD_FPGA_PD19,1);
	arch_ioport_set_pin_level(GPIO_TXD1,1);
	arch_ioport_set_pin_level(GPIO_TXD0,1);

    vTaskDelay(E70B_vTaskDelay_Modulus(100));
    data_voltage = Bus1AdcPc31ValueGet( );

    if(value==0)
    printf_usb("DWFCAN H  0V==%d mv",data_voltage);
    else if(value==1)
    printf_usb("DWFCAN L  5V==%d mv",data_voltage);
	tx_CTRL_EN_channel2(JC5_CAN_SW1,JC_LOW);//WCFT CAN_L AND CAN_H OFF
	tx_CTRL_EN_channel2_write();
    return  data_voltage;
	#endif	
}

uint32_t HARDWARETEST_DB26_PIN_PWR_PG(uint32_t Pinnumber,uint32_t value)		
{
			#if 0  //quwenqi2g	
	unsigned int data_voltage;		
			 
	Db26_PgAndPwrPinDirInit();
	Db26_PgAndPwrPinLevelInit();
			
	hal_dac_init();
	hal_adc_init2(AFEC0,AFEC_CHANNEL_PROG_VOLT);//don't delete
	//return data_voltage;
	hal_set_ProgVolt2_no_calib(9.0);//9V
	vTaskDelay(E70B_vTaskDelay_Modulus(3));
			
	AdcPinInit(PIO_PC30_IDX,IOPORT_DIR_INPUT,IOPORT_MODE_OPEN_DRAIN);
	J1850DrvAdcPc30Init();
			
	memset((uint8_t *)&PDU_FPGA_C_REG2.L32.reg,0,sizeof(PDU_FPGA_C_REG));   //clears sizeof(PDU_FPGA_C_REG) == 8
	//tx_CTRL_EN_channel2(JC15_J1850_DRV_ADC_DB26PIN1_ADC_CTRL,JC_HIGH);//DB26_PIN1_ADC[ ADC CHANNEL]
	arch_ioport_set_pin_dir(DAT_LIGHT0_JC15,IOPORT_DIR_OUTPUT);
	arch_ioport_set_pin_level(DAT_LIGHT0_JC15,1);
			
	tx_CTRL_EN_channel2_write();

	Db26_PgAndPwrPinDirInit();
	//////////////////////////////////
	Db26_PgAndPwrPinLevelInit();
			
	arch_ioport_set_pin_level(DB26_PIN14_PWR,0);
	arch_ioport_set_pin_level(DB26_PIN13_PWR,0);
			
	if((Pinnumber==DB26_PIN9_PG_FPGA)&&(value==1))
	{
		////////////////////////////////////////////////////////
		arch_ioport_set_pin_level(DB26_PIN9_PG_FPGA,0);
		arch_ioport_set_pin_level(DB26_PIN9_PWR_FPGA,1);	
		vTaskDelay(E70B_vTaskDelay_Modulus(60));
		data_voltage = J1850DrvAdcPc30ValueGet( );
		printf_usb("DB26_PIN_9  9V PWR==%d mv",data_voltage);
		arch_ioport_set_pin_level(DB26_PIN9_PG_FPGA,0);
	}
	else if((Pinnumber==DB26_PIN9_PG_FPGA)&&(value==0))
	{
		////////////////////////////////////////////////////////
		arch_ioport_set_pin_level(DB26_PIN9_PG_FPGA,1);
		arch_ioport_set_pin_level(DB26_PIN9_PWR_FPGA,0);
		vTaskDelay(E70B_vTaskDelay_Modulus(30));
		data_voltage = J1850DrvAdcPc30ValueGet( );
		printf_usb("DB26_PIN_9  9V PG==%d mv",data_voltage);
		arch_ioport_set_pin_level(DB26_PIN9_PG_FPGA,0);
	}
	else if((Pinnumber==DB26_PIN12_PG_FPGA)&&(value==1))
	{
		////////////////////////////////////////////////////////
		arch_ioport_set_pin_level(DB26_PIN12_PG_FPGA,0);
		arch_ioport_set_pin_level(DB26_PIN12_PWR_FPGA,1);
		vTaskDelay(60);
		data_voltage = J1850DrvAdcPc30ValueGet( );
		printf_usb("DB26_PIN_12  9V PWR==%d mv",data_voltage);
	}
	else if((Pinnumber==DB26_PIN12_PG_FPGA)&&(value==0))
	{
		////////////////////////////////////////////////////////
		arch_ioport_set_pin_level(DB26_PIN12_PG_FPGA,1);
		arch_ioport_set_pin_level(DB26_PIN12_PWR_FPGA,0);
		vTaskDelay(E70B_vTaskDelay_Modulus(30));
		data_voltage = J1850DrvAdcPc30ValueGet( );
		printf_usb("DB26_PIN_12  9V PG==%d mv",data_voltage);
	}
///////////////////////////////////////
				
	else if((Pinnumber==DB26_PIN6_PWR)&&(value==1))
	{
		arch_ioport_set_pin_level(DB26_PIN12_PG_FPGA,0);
		arch_ioport_set_pin_level(DB26_PIN12_PWR_FPGA,0);
		arch_ioport_set_pin_level(DB26_PIN9_PG_FPGA,0);
		arch_ioport_set_pin_level(DB26_PIN9_PWR_FPGA,0);
						
		////////////////////////////////////////////////////////
		arch_ioport_set_pin_level(DB26_PIN6_PWR,1);
					 
		vTaskDelay(E70B_vTaskDelay_Modulus(60));
		data_voltage = J1850DrvAdcPc30ValueGet( );
		printf_usb("DB26_PIN_6  9V high PWR==%d mv",data_voltage);
		arch_ioport_set_pin_level(DB26_PIN6_PWR,0);
	}
	else if((Pinnumber==DB26_PIN6_PWR)&&(value==0))
	{
		arch_ioport_set_pin_level(DB26_PIN12_PG_FPGA,0);
		arch_ioport_set_pin_level(DB26_PIN12_PWR_FPGA,0);
		arch_ioport_set_pin_level(DB26_PIN9_PG_FPGA,0);
		arch_ioport_set_pin_level(DB26_PIN9_PWR_FPGA,0);
		////////////////////////////////////////////////////////
		
		arch_ioport_set_pin_level(DB26_PIN6_PWR,0);
		vTaskDelay(E70B_vTaskDelay_Modulus(60));
		data_voltage = J1850DrvAdcPc30ValueGet( );
		printf_usb("DB26_PIN_6  9V low PWR==%d mv",data_voltage);
	}	
	//////////////////////////////////////////////////////

	else if((Pinnumber==DB26_PIN11_PWR)&&(value==1))
	{
		arch_ioport_set_pin_level(DB26_PIN12_PG_FPGA,0);
		arch_ioport_set_pin_level(DB26_PIN12_PWR_FPGA,0);
		arch_ioport_set_pin_level(DB26_PIN9_PG_FPGA,0);
		arch_ioport_set_pin_level(DB26_PIN9_PWR_FPGA,0);
					
		////////////////////////////////////////////////////////
		arch_ioport_set_pin_level(DB26_PIN11_PWR,1);
					
		vTaskDelay(E70B_vTaskDelay_Modulus(60));
		data_voltage = J1850DrvAdcPc30ValueGet( );
		printf_usb("DB26_PIN_11  9V high PWR==%d mv",data_voltage);
		arch_ioport_set_pin_level(DB26_PIN11_PWR,0);
	}
	else if((Pinnumber==DB26_PIN11_PWR)&&(value==0))
	{
		arch_ioport_set_pin_level(DB26_PIN12_PG_FPGA,0);
		arch_ioport_set_pin_level(DB26_PIN12_PWR_FPGA,0);
		arch_ioport_set_pin_level(DB26_PIN9_PG_FPGA,0);
		arch_ioport_set_pin_level(DB26_PIN9_PWR_FPGA,0);
		////////////////////////////////////////////////////////
	
		arch_ioport_set_pin_level(DB26_PIN11_PWR,0);
		vTaskDelay(E70B_vTaskDelay_Modulus(60));
		data_voltage = J1850DrvAdcPc30ValueGet( );
		printf_usb("DB26_PIN_11  9V low PWR==%d mv",data_voltage);
	}
	else if((Pinnumber==DB26_PIN14_PWR)&&(value==1))
	{
		arch_ioport_set_pin_level(DB26_PIN12_PG_FPGA,0);
		arch_ioport_set_pin_level(DB26_PIN12_PWR_FPGA,0);
		arch_ioport_set_pin_level(DB26_PIN9_PG_FPGA,0);
		arch_ioport_set_pin_level(DB26_PIN9_PWR_FPGA,0);
							
		////////////////////////////////////////////////////////
		arch_ioport_set_pin_level(DB26_PIN14_PWR,1);
							
		vTaskDelay(60);
		data_voltage = J1850DrvAdcPc30ValueGet( );
		printf_usb("DB26_PIN_14  9V high PWR==%d mv",data_voltage);
		arch_ioport_set_pin_level(DB26_PIN14_PWR,0);
	}
	else if((Pinnumber==DB26_PIN14_PWR)&&(value==0))
	{
		arch_ioport_set_pin_level(DB26_PIN12_PG_FPGA,0);
		arch_ioport_set_pin_level(DB26_PIN12_PWR_FPGA,0);
		arch_ioport_set_pin_level(DB26_PIN9_PG_FPGA,0);
		arch_ioport_set_pin_level(DB26_PIN9_PWR_FPGA,0);
		////////////////////////////////////////////////////////
							
							
		arch_ioport_set_pin_level(DB26_PIN14_PWR,0);
		vTaskDelay(E70B_vTaskDelay_Modulus(60));
		data_voltage = J1850DrvAdcPc30ValueGet( );
		printf_usb("DB26_PIN_14  9V low PWR==%d mv",data_voltage);
	}	
	else if((Pinnumber==DB26_PIN13_PWR)&&(value==1))
	{
		arch_ioport_set_pin_level(DB26_PIN12_PG_FPGA,0);
		arch_ioport_set_pin_level(DB26_PIN12_PWR_FPGA,0);
		arch_ioport_set_pin_level(DB26_PIN9_PG_FPGA,0);
		arch_ioport_set_pin_level(DB26_PIN9_PWR_FPGA,0);
											
		////////////////////////////////////////////////////////
		arch_ioport_set_pin_level(DB26_PIN13_PWR,1);
											
		vTaskDelay(E70B_vTaskDelay_Modulus(60));
		data_voltage = J1850DrvAdcPc30ValueGet( );
		printf_usb("DB26_PIN_13  9V high PWR==%d mv",data_voltage);
		arch_ioport_set_pin_level(DB26_PIN13_PWR,0);
	}
	else if((Pinnumber==DB26_PIN13_PWR)&&(value==0))
	{
		arch_ioport_set_pin_level(DB26_PIN12_PG_FPGA,0);
		arch_ioport_set_pin_level(DB26_PIN12_PWR_FPGA,0);
		arch_ioport_set_pin_level(DB26_PIN9_PG_FPGA,0);
		arch_ioport_set_pin_level(DB26_PIN9_PWR_FPGA,0);
		////////////////////////////////////////////////////////
											
											
		arch_ioport_set_pin_level(DB26_PIN13_PWR,0);
		vTaskDelay(60);
		data_voltage = J1850DrvAdcPc30ValueGet( );
		printf_usb("DB26_PIN_13  9V low PWR==%d mv",data_voltage);
	}
	else if((Pinnumber==DB26_PIN15_PG)&&(value==1))
	{
		//return data_voltage;
		arch_ioport_set_pin_level(DB26_PIN12_PG_FPGA,0);
		arch_ioport_set_pin_level(DB26_PIN12_PWR_FPGA,0);
		arch_ioport_set_pin_level(DB26_PIN9_PG_FPGA,0);
		arch_ioport_set_pin_level(DB26_PIN9_PWR_FPGA,0);
						
		arch_ioport_set_pin_dir(DB26_PIN15_PG,IOPORT_DIR_OUTPUT);							
		////////////////////////////////////////////////////////
		arch_ioport_set_pin_level(DB26_PIN15_PG,0);
													
        arch_ioport_set_pin_level(DB26_PIN9_PWR_FPGA,1);													
													
		vTaskDelay(E70B_vTaskDelay_Modulus(60));
		data_voltage = J1850DrvAdcPc30ValueGet( );
		printf_usb("DB26_PIN15_PG  2.5V high PWR==%d mv",data_voltage);
		arch_ioport_set_pin_level(DB26_PIN9_PWR_FPGA,0);
		arch_ioport_set_pin_level(DB26_PIN15_PG,1);
		arch_ioport_set_pin_dir(DB26_PIN15_PG,IOPORT_DIR_INPUT);	
	}
	else if((Pinnumber==DB26_PIN15_PG)&&(value==0))
	{
		//return data_voltage;
		arch_ioport_set_pin_level(DB26_PIN12_PG_FPGA,0);
		arch_ioport_set_pin_level(DB26_PIN12_PWR_FPGA,0);
		arch_ioport_set_pin_level(DB26_PIN9_PG_FPGA,0);
		arch_ioport_set_pin_level(DB26_PIN9_PWR_FPGA,0);
		////////////////////////////////////////////////////////
													
		arch_ioport_set_pin_dir(DB26_PIN15_PG,IOPORT_DIR_OUTPUT);							
		arch_ioport_set_pin_level(DB26_PIN15_PG,1);
		vTaskDelay(E70B_vTaskDelay_Modulus(60));
		data_voltage = J1850DrvAdcPc30ValueGet( );
		printf_usb("DB26_PIN15_PG  9V low PWR==%d mv",data_voltage);
		arch_ioport_set_pin_level(DB26_PIN15_PG,0);
		arch_ioport_set_pin_dir(DB26_PIN15_PG,IOPORT_DIR_INPUT);
	}
			
	return  data_voltage;
	#endif	
}



uint32_t HARDWARETEST_JMUX1_SERVICE_FUNCTION(unsigned int JD_INDEX)
{
	unsigned int data_voltage;
	unsigned int data_voltage1;
	unsigned int data_voltage2;
		unsigned int i;
				#if 0  //quwenqi2g	
	AdcPinInit(PIO_PC31_IDX,IOPORT_DIR_INPUT,IOPORT_MODE_PULLDOWN);
						
	Bus1AdcPc31Init();
						
	memset((uint8_t *)&PDU_FPGA_C_REG2.L32.reg,0,sizeof(PDU_FPGA_C_REG));   //clears sizeof(PDU_FPGA_C_REG) == 8
	tx_CTRL_EN_channel2(BUS1_PWR_8V_CTL,JC_HIGH);							//VCC CHANNEL CHOSSE //VCC_DRV == 8V
						
	tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_HIGH);//LINE 0  PULL UP
	//tx_CTRL_EN_channel2(JC2_LINE0_PD,JC_HIGH);//LINE 0  PULL DOWN
						
	//tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_HIGH);//LINE 1  PULL UP
	//tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_HIGH);//LINE 1  PULL DOWN


    tx_CTRL_EN_channel2(JC1_BUS3_CAN_SW,JC_LOW);//LINE 1  PULL UP //DB26_PIN6  DB26_PIN14 DONT TO CAN2
						
	tx_CTRL_EN_channel2(JC4_CAN_SW0,JC_LOW);//LINE0 AND LINE1
	tx_CTRL_EN_channel2(JC5_CAN_SW1,JC_LOW);//MUX0 AND MUX1
						
	tx_CTRL_EN_channel2(JC11_BUS1_LINE0_LINE1_ADC_CTRL,JC_HIGH);//MUX0[ ADC CHANNEL]
	tx_CTRL_EN_channel2_write();					
	tx_MODE_EN_channel2(0,0,1,1);
						
	LinePinDirInit();
						
	/****/

	arch_ioport_set_pin_level(LINE0_TEST,0);
	arch_ioport_set_pin_level(LINE1_TEST,0);

						
						
	arch_ioport_set_pin_level(MCU_TXD_FPGA_PD19,1);
	arch_ioport_set_pin_level(GPIO_TXD1,1);
	arch_ioport_set_pin_level(GPIO_TXD0,1);
						
	hal_init_pwm_CMP_Volt();

	//hal_SET_CMP_Volt(2.2);//2//2v---9V
										
	vTaskDelay (E70B_vTaskDelay_Modulus(1));
	data_voltage = Bus1AdcPc31ValueGet( );
	printf_usb("LINE0 out high 8V==%d mv",data_voltage);
						
						
	tx_PINSEL_channel2(JD_INDEX,JD_INDEX);// JDA0 -->MUX0--- LINE0--1
	//  JDB0-->MUX1--- JA LINE1 JB
						
	tx_CTRL_EN_channel2(JC11_BUS1_LINE0_LINE1_ADC_CTRL,JC_LOW);//MUX1[ ADC CHANNEL]
	tx_CTRL_EN_channel2_write();
	/////////////J66//////////////
	vTaskDelay(E70B_vTaskDelay_Modulus(1));
	data_voltage1 = Bus1AdcPc31ValueGet( );
						
	printf_usb("%d out high 8V==%d mv",JD_INDEX,data_voltage1);
						
	tx_PINSEL_channel2(DB26_PIN_NC,JD_INDEX);// JDA0 -->MUX0--- LINE0--1
	tx_CTRL_EN_channel2_write();	
	vTaskDelay(E70B_vTaskDelay_Modulus(10));				
	for(i=0;i<5;i++)
	{
		vTaskDelay(E70B_vTaskDelay_Modulus(1));
		data_voltage2 = Bus1AdcPc31ValueGet( );
		if(data_voltage2<450)	break;
	}
						
	printf_usb("%d out low 8V==%d mv",JD_INDEX,data_voltage2);

	if((data_voltage1>7000)&&(data_voltage1<9000)&&(data_voltage2>=0)&&(data_voltage2<450))
	{
		data_voltage=0;
	}
	else
	{
		data_voltage=1;
	}
	#endif	
	return data_voltage;		
}

uint32_t HARDWARETEST_JMUX0_SERVICE_FUNCTION(unsigned int JD_INDEX)
{
	unsigned int data_voltage;
	unsigned int data_voltage1;
	unsigned int data_voltage2; 
	unsigned int i;
			#if 0  //quwenqi2g	
	AdcPinInit(PIO_PC31_IDX,IOPORT_DIR_INPUT,IOPORT_MODE_PULLDOWN);
	
	Bus1AdcPc31Init();
	
	memset((uint8_t *)&PDU_FPGA_C_REG2.L32.reg,0,sizeof(PDU_FPGA_C_REG));   //clears sizeof(PDU_FPGA_C_REG) == 8
	tx_CTRL_EN_channel2(BUS1_PWR_8V_CTL,JC_HIGH);							//VCC CHANNEL CHOSSE //VCC_DRV == 8V
	

	tx_CTRL_EN_channel2(JC0_LINE0_PU,JC_LOW);//LINE 0  PULL UP
	tx_CTRL_EN_channel2(JC2_LINE0_PD,JC_LOW);//LINE 0  PULL DOWN
				
	tx_CTRL_EN_channel2(JC8_LINE1_PU,JC_HIGH);//LINE 1  PULL UP
	//tx_CTRL_EN_channel2(JC3_LINE1_PD,JC_HIGH);//LINE 1  PULL DOWN
	
	
	tx_CTRL_EN_channel2(JC1_BUS3_CAN_SW,JC_LOW);//LINE 1  PULL UP //DB26_PIN6  DB26_PIN14 DONT TO CAN2
	

	tx_CTRL_EN_channel2(JC4_CAN_SW0,JC_LOW);//LINE0 AND LINE1
	tx_CTRL_EN_channel2(JC5_CAN_SW1,JC_LOW);//MUX0 AND MUX1
	
	tx_CTRL_EN_channel2(JC11_BUS1_LINE0_LINE1_ADC_CTRL,JC_LOW);//MUX1[ ADC CHANNEL]
	
	tx_MODE_EN_channel2(0,0,1,1);
	tx_CTRL_EN_channel2_write();
	LinePinDirInit();
	
	/****/

	arch_ioport_set_pin_level(LINE0_TEST,0);
	arch_ioport_set_pin_level(LINE1_TEST,0);


	arch_ioport_set_pin_level(MCU_TXD_FPGA_PD19,1);
	arch_ioport_set_pin_level(GPIO_TXD1,1);
	arch_ioport_set_pin_level(GPIO_TXD0,1);
	
	hal_init_pwm_CMP_Volt();

	//hal_SET_CMP_Volt(2.2);//2//2v---9V
	
	vTaskDelay(E70B_vTaskDelay_Modulus(2));
	data_voltage1 = Bus1AdcPc31ValueGet( );
	printf_usb("LINE1 out high 8V==%d mv",data_voltage1);
	
	
	tx_PINSEL_channel2(JD_INDEX,JD_INDEX);// JDA0 -->MUX0--- LINE0--1
	//  JDB0-->MUX1--- JA LINE1 JB
	
	tx_CTRL_EN_channel2(JC11_BUS1_LINE0_LINE1_ADC_CTRL,JC_HIGH);//MUX0[ ADC CHANNEL]
	tx_CTRL_EN_channel2_write();
	/////////////J66//////////////
	vTaskDelay(E70B_vTaskDelay_Modulus(2));
	data_voltage1 = Bus1AdcPc31ValueGet( );
	
	printf_usb("%d out high 8V==%d mv",JD_INDEX,data_voltage1);
	
	tx_PINSEL_channel2(JD_INDEX,DB26_PIN_NC);// JDA0 -->MUX0--- LINE0--1
	tx_CTRL_EN_channel2_write();
	vTaskDelay(E70B_vTaskDelay_Modulus(10));
	for(i=0;i<5;i++)
	{
		vTaskDelay(E70B_vTaskDelay_Modulus(1));
		data_voltage2 = Bus1AdcPc31ValueGet( );
		if(data_voltage2<450)	break;
	}

	printf_usb("%d out low 8V==%d mv",JD_INDEX,data_voltage2);
	if((data_voltage1>7000)&&(data_voltage1<9000)&&(data_voltage2>=0)&&(data_voltage2<450))
	{
		data_voltage=0;
	}
	else
	{
		data_voltage=1;
	}
	#endif	
   return data_voltage;
}




uint32_t HARDWARETEST_WIFI_SERVICE_FUNCTION(void)
{
	return 0;
}

uint32_t HARDWARETEST_BLUE_SERVICE_FUNCTION(void)
{
	return 0;
}


void hardware_test(uint8_t Protocol, uint16_t wParam, uint8_t *lParam)
{
	uint16_t hardwarefunctionnum;
	uint8_t buffer[256];
	uint8_t *buffer_P ;
	//uint16_t number = sizeof(data_voltage) + 5;
	uint16_t number ;
	PDU_COMMON_DATA_STRUCT CommonData;
	unsigned int data_voltage;
	CommonData.channl  = *lParam++;   //new
	CommonData.confirm = *lParam++;
	CommonData.reserve2 = *lParam++;
	
	hardwarefunctionnum=(*lParam++)*0x100+(*lParam++);
	 
	
	
	*buffer_P++ = CommonData.channl>>8;        //new
	*buffer_P++ = hardwarefunctionnum>>8;
	*buffer_P++ = hardwarefunctionnum;
	*buffer_P++ = SYSTEM_CONFIRM;
	*buffer_P++ = DEVICE_RETURN_CODE_OK;
	switch(hardwarefunctionnum)
	{
		case 	HARDWARETEST_VCC_IN :/*（0x00）VCC_IN TEST*/ //// a5 a5 00 07 f2 00 00 00 00 00 00 07 //VCC_IN Voltage mv
			data_voltage= HARDWARETEST_VCC_IN_SERVICE_FUNCTION();
			break;	
			
		case 	HARDWARETEST_VCC_PROG_9V  :   /*（0x01）VCC_PROG TEST*/  // a5 a5 00 07 f2 00 00 00 00  01 00 07 //VCC_PROG 9V Voltage mv
			data_voltage=HARDWARETEST_VCC_PROG_SERVICE_FUNCTION(9.0);
			
			break;
		case 	HARDWARETEST_VCC_PROG_12V :   /*（0x01）VCC_PROG TEST*/ // a5 a5 00 07 f2 00 00 00 00  01  01 07 //VCC_PROG 12 V Voltage mv
			data_voltage=HARDWARETEST_VCC_PROG_SERVICE_FUNCTION(12.0);
			break;
		case 	HARDWARETEST_VCC_PROG_20V :   /*（0x01）VCC_PROG TEST*/ // a5 a5 00 07 f2 00 00 00 00  01  02 07 //VCC_PROG 20 V Voltage mv
			data_voltage=HARDWARETEST_VCC_PROG_SERVICE_FUNCTION(20.0);
			break;	
		
		case 	HARDWARETEST_VCC_DRV_5V:  /*（0x02）VCC_DRV TEST*/  // a5 a5 00 07 f2 00 00 00 00  02 00 07 //VCC_DRV 5 V Voltage mv
			data_voltage=HARDWARETEST_VCC_DRV_SERVICE_FUNCTION(5);
			break;
		case 	HARDWARETEST_VCC_DRV_8V:   /*（0x02）VCC_DRV TEST*/ // a5 a5 00 07 f2 00 00 00 00  02  01 07 //VCC_DRV 8 V Voltage mv
			data_voltage=HARDWARETEST_VCC_DRV_SERVICE_FUNCTION(8);
			break;
		case 	HARDWARETEST_VCC_DRV_12V:   /*（0x02）VCC_DRV TEST*/// a5 a5 00 07 f2 00 00 00 00  02  02 07 //VCC_DRV 8 V Voltage mv
			data_voltage=HARDWARETEST_VCC_DRV_SERVICE_FUNCTION(12);
			break;
		
		case 	HARDWARETEST_BUS1_CMP_V_2P1:  /*（0x0300）BUS1_CMP_V TEST*/ // a5 a5 00 07 f2 00 00 00 00 00 03 07 //VCC_DRV 8 V Voltage mv
			data_voltage=HARDWARETEST_BUS1_CMP_V_SERVICE_FUNCTION(2.1);
			break;
		
		case 	HARDWARETEST_BUS1_CMP_V_2P2:  /*（0x0301）BUS1_CMP_V TEST*/ // a5 a5 00 07 f2 00 00 00 00 01 03 07 //VCC_DRV 8 V Voltage mv
			data_voltage=HARDWARETEST_BUS1_CMP_V_SERVICE_FUNCTION(2.2);
			break;
		
		
		case 	HARDWARETEST_VCC_DRV2_5V: /*（0x0400）VCC_DRV2 TEST*/
			data_voltage=HARDWARETEST_VCC_DRV2_SERVICE_FUNCTION(5); // a5 a5 00 07 f2 00 00 00 00 00 04 07 //VCC_DRV 8 V Voltage mv
			break;
		
		case 	HARDWARETEST_VCC_DRV2_8V: /*（0x0401）VCC_DRV2 TEST*/
			data_voltage=HARDWARETEST_VCC_DRV2_SERVICE_FUNCTION(8);// a5 a5 00 07 f2 00 00 00 00 01 04 07 //VCC_DRV 8 V Voltage mv
			break;
		
		case 	HARDWARETEST_J1850_P_OUT_HIGH : /*（0x0500）J1850_P TEST*/
			data_voltage=HARDWARETEST_J1850_P_M_SERVICE_FUNCTION(1,1,1);// a5 a5 00 07 f2 00 00 00 00 05 00 07 //VCC_DRV 8 V Voltage mv 
			break;
		case 	HARDWARETEST_J1850_P_OUT_LOW : /*（0x0501）J1850_P TEST*/
			data_voltage=HARDWARETEST_J1850_P_M_SERVICE_FUNCTION(1,1,0);// a5 a5 00 07 f2 00 00 00 00 05 01 07 //VCC_DRV 8 V Voltage mv
			break;
		case HARDWARETEST_J1850_M_OUT_HIGH  : /*（0x0600）J1850_M TEST*/
			data_voltage=HARDWARETEST_J1850_P_M_SERVICE_FUNCTION(0,1,1);// a5 a5 00 07 f2 00 00 00 00 06 00 07 //VCC_DRV 8 V Voltage mv
			break;
		case HARDWARETEST_J1850_M_OUT_LOW   : /*（0x0601）J1850_M TEST*/
			data_voltage=HARDWARETEST_J1850_P_M_SERVICE_FUNCTION(0,1,0);// a5 a5 00 07 f2 00 00 00 00 06 01 07 //VCC_DRV 8 V Voltage mv
			break;
		case HARDWARETEST_J1850_P_RX_HIGH  : /*（0x0700）J1850_M TEST*/
			data_voltage=HARDWARETEST_J1850_P_M_SERVICE_FUNCTION(1,0,1);// a5 a5 00 07 f2 00 00 00 00 07 00 07 //VCC_DRV 8 V Voltage mv
			break;
		case HARDWARETEST_J1850_P_RX_LOW   : /*（0x0701）J1850_M TEST*/
			data_voltage=HARDWARETEST_J1850_P_M_SERVICE_FUNCTION(1,0,0);// a5 a5 00 07 f2 00 00 00 00 07 01 07 //VCC_DRV 8 V Voltage mv
			break;

		case HARDWARETEST_DB26_PIN1_HIGH  : /*（0x0800）DB26_PIN1 TEST*/ 
    		data_voltage = HARDWARETEST_DB26_PIN1_SERVICE_FUNCTION(8);  //a5 a5 00 07 f2 00 00 00 00 08 00 07 //DB26_PIN1 8 V Voltage mv
    		break;
		
		case HARDWARETEST_DB26_PIN1_LOW  : /*（0x0801）DB26_PIN1 TEST*/
    		data_voltage = HARDWARETEST_DB26_PIN1_SERVICE_FUNCTION(0);//a5 a5 00 07 f2 00 00 00 00 08 01 07 //DB26_PIN1 0 V Voltage mv
    		break;
		
		case HARDWARETEST_LINE0_OUT_HIGH : /*（0x0900）LINE0 TEST*/
			data_voltage=HARDWARETEST_LINE_SERVICE_FUNCTION(0,0,1); //a5 a5 00 07 f2 00 00 00 00 00 09 07 //LINE0 0 V Voltage mv
			break;
		case HARDWARETEST_LINE0_OUT_LOW  : /*（0x0901）LINE0 TEST*/
			data_voltage=HARDWARETEST_LINE_SERVICE_FUNCTION(0,0,0);//a5 a5 00 07 f2 00 00 00 00 01 09 07 //LINE0 0 V Voltage mv
			break;
		case HARDWARETEST_LINE0_IN_HIGH  : /*（0x0902）LINE0 TEST*/
			data_voltage=HARDWARETEST_LINE_SERVICE_FUNCTION(0,1,1);//a5 a5 00 07 f2 00 00 00 00 02 09 07 //LINE0 0 V Voltage mv
			break;
		case HARDWARETEST_LINE0_IN_LOW  : /*（0x0903）LINE0 TEST*/
			data_voltage=HARDWARETEST_LINE_SERVICE_FUNCTION(0,1,0);//a5 a5 00 07 f2 00 00 00 00 03 09 07 //LINE0 0 V Voltage mv
			break;
			
		case HARDWARETEST_LINE0_PULLDOWN_PULLUP_OUT_HIGH  :/*（0x0904）LINE0 TEST*/
			data_voltage=HARDWARETEST_LINE_PULLUP_DOWN_SERVICE_FUNCTION(0,1);//a5 a5 00 07 f2 00 00 00 00 04 09 07 //LINE0 0 V Voltage mv
			break;
			
		case HARDWARETEST_LINE0_PULLDOWN_PULLUP_OUT_LOW  :/*（0x0905）LINE0 TEST*/
			data_voltage=HARDWARETEST_LINE_PULLUP_DOWN_SERVICE_FUNCTION(0,0);//a5 a5 00 07 f2 00 00 00 00 09 05 07 //LINE0 0 V Voltage mv
			break;

		case HARDWARETEST_LINE1_OUT_HIGH  : /*（0x0A00）LINE1 TEST*/
			data_voltage = HARDWARETEST_LINE_SERVICE_FUNCTION(1,0,1);//a5 a5 00 07 f2 00 00 00 00 00 0a 07 //LINE0 0 V Voltage mv
			break;
		case HARDWARETEST_LINE1_OUT_LOW  : /*（0x0A01）LINE1 TEST*/
			data_voltage = HARDWARETEST_LINE_SERVICE_FUNCTION(1,0,0);//a5 a5 00 07 f2 00 00 00 00 01 0a 07 //LINE1 0 V Voltage mv
			break;
		case HARDWARETEST_LINE1_IN_HIGH  : /*（0x0A02）LINE1 TEST*/
			data_voltage = HARDWARETEST_LINE_SERVICE_FUNCTION(1,1,1);//a5 a5 00 07 f2 00 00 00 00 02 0a 07 //LINE1 0 V Voltage mv
			break;
		case HARDWARETEST_LINE1_IN_LOW  : /*（0x0A03）LINE1 TEST*/
			data_voltage = HARDWARETEST_LINE_SERVICE_FUNCTION(1,1,0);//a5 a5 00 07 f2 00 00 00 00 03 0a 07 //LINE1 0 V Voltage mv
			break;
			
		case HARDWARETEST_LINE1_PULLDOWN_PULLUP_OUT_HIGH  :/*（0x0A04）LINE1 TEST*/
			data_voltage=HARDWARETEST_LINE_PULLUP_DOWN_SERVICE_FUNCTION(1,1);//a5 a5 00 07 f2 00 00 00 00 04 0a 07 //LINE1 0 V Voltage mv
		break;
			
		case HARDWARETEST_LINE1_PULLDOWN_PULLUP_OUT_LOW  :/*（0x0A05）LINE1 TEST*/
			data_voltage=HARDWARETEST_LINE_PULLUP_DOWN_SERVICE_FUNCTION(1,0);//a5 a5 00 07 f2 00 00 00 00 05 0a 07 //LINE1 0 V Voltage mv
		break;

		case HARDWARETEST_can1_H  : /*（0x0e00）can1 TEST*/
    		data_voltage=HARDWARETEST_can1_SERVICE_FUNCTION(0); //a5 a5 00 07 f2 00 00 00 00 0e 00 07 //can1 0 V Voltage mv  //2.5--2.7
    		break;
		case HARDWARETEST_can1_L  :/*（0x0e01）can1 TEST*/
    		data_voltage=HARDWARETEST_can1_SERVICE_FUNCTION(1);//a5 a5 00 07 f2 00 00 00 00 0e 01 07 //can1  0 V Voltage mv   //2.5--2.7
    		break;
		case HARDWARETEST_can2_H  :/*（0x0F00）can2 TEST*/
			data_voltage=HARDWARETEST_can2_SERVICE_FUNCTION(0);//a5 a5 00 07 f2 00 00 00 00 00 0f 07 //LINE1 0 V Voltage mv
		break;
		case HARDWARETEST_can2_L  : /*（0x0F01）can2 TEST*/
			data_voltage=HARDWARETEST_can2_SERVICE_FUNCTION(1);//a5 a5 00 07 f2 00 00 00 00 01 0f 07 //LINE1 0 V Voltage mv
		break;
		
		
		case HARDWARETEST_WCFTCAN_H  : /*（0x09）WCFTCANH TEST*/
			data_voltage= HARDWARETEST_DWFCAN_SERVICE_FUNCTION( 0 );
		break;
		
		case HARDWARETEST_WCFTCAN_L  : /*（0x0A）WCFTCANH  TEST*/
			data_voltage= HARDWARETEST_DWFCAN_SERVICE_FUNCTION( 1 );
		break;
		
		case HARDWARETEST_DB26_PIN9_PWR  : /*（0x05）DB26_PIN9 TEST*/ //a5 a5 00 07 f2 00 00 00 00 00 11 07 //LINE1 0 V Voltage mv
			data_voltage=HARDWARETEST_DB26_PIN_PWR_PG(DB26_PIN9_PG_FPGA,1);
			break;
		case HARDWARETEST_DB26_PIN9_PG   : /*（0x05）DB26_PIN9 TEST*/
			data_voltage=HARDWARETEST_DB26_PIN_PWR_PG(DB26_PIN9_PG_FPGA,0);//a5 a5 00 07 f2 00 00 00 00 01 11 07 //LINE1 0 V Voltage mv
			break;
		case HARDWARETEST_DB26_PIN12_PWR  : /*（0x05）DB26_PIN12 TEST*/
			data_voltage=HARDWARETEST_DB26_PIN_PWR_PG(DB26_PIN12_PG_FPGA,1);
			break;
		case HARDWARETEST_DB26_PIN12_PG  :/*（0x05）DB26_PIN12 TEST*/
			data_voltage=HARDWARETEST_DB26_PIN_PWR_PG(DB26_PIN12_PG_FPGA,0);//a5 a5 00 07 f2 00 00 00 00 01 12 07 //LINE1 0 V Voltage mv
			break;
		
		case HARDWARETEST_DB26_PIN6_PWR_1 : /*（0x05）DB26_PIN6 TEST*/    //a5 a5 00 07 f2 00 00 00 00 00 13 07 //LINE1 0 V Voltage mv
			data_voltage=HARDWARETEST_DB26_PIN_PWR_PG(DB26_PIN6_PWR,1);
			break;
		case HARDWARETEST_DB26_PIN6_PWR_0  : /*（0x05）DB26_PIN6 TEST*///a5 a5 00 07 f2 00 00 00 00 01 13 07 //LINE1 0 V Voltage mv
			data_voltage=HARDWARETEST_DB26_PIN_PWR_PG(DB26_PIN6_PWR,0);
			break;
		
		case HARDWARETEST_DB26_PIN11_PWR_1  :/*（0x05）DB26_PIN11 TEST*///a5 a5 00 07 f2 00 00 00 00 00 14 07 //LINE1 0 V Voltage mv
			data_voltage=HARDWARETEST_DB26_PIN_PWR_PG(DB26_PIN11_PWR,1);
			break;
		case HARDWARETEST_DB26_PIN11_PWR_0  : /*（0x05）DB26_PIN11 TEST*///a5 a5 00 07 f2 00 00 00 00 01 14 07 //LINE1 0 V Voltage mv
			data_voltage=HARDWARETEST_DB26_PIN_PWR_PG(DB26_PIN11_PWR,0);
			break;
		
		case HARDWARETEST_DB26_PIN14_PWR_1  : /*（0x05）DB26_PIN14 TEST*///a5 a5 00 07 f2 00 00 00 00 00 15 07 //LINE1 0 V Voltage mv
			data_voltage=HARDWARETEST_DB26_PIN_PWR_PG(DB26_PIN14_PWR,1);
			break;
		case HARDWARETEST_DB26_PIN14_PWR_0  : /*（0x05）DB26_PIN14 TEST*///a5 a5 00 07 f2 00 00 00 00 01 15 07 //LINE1 0 V Voltage mv
			data_voltage=HARDWARETEST_DB26_PIN_PWR_PG(DB26_PIN14_PWR,0);
			break;
		
		case HARDWARETEST_DB26_PIN13_PWR_1  : /*（0x05）DB26_PIN13 TEST*///a5 a5 00 07 f2 00 00 00 00 00 16 07 //LINE1 0 V Voltage mv
			data_voltage=HARDWARETEST_DB26_PIN_PWR_PG(DB26_PIN13_PWR,1);
			break;
		case HARDWARETEST_DB26_PIN13_PWR_0 : /*（0x05）DB26_PIN13 TEST*///a5 a5 00 07 f2 00 00 00 00 01 16 07 //LINE1 0 V Voltage mv
			data_voltage=HARDWARETEST_DB26_PIN_PWR_PG(DB26_PIN13_PWR ,0);
			break;
		
		case HARDWARETEST_DB26_PIN15_PG_1  :/*（0x05）DB26_PIN15 TEST*///a5 a5 00 07 f2 00 00 00 00 00 17 07 //LINE1 0 V Voltage mv
			data_voltage=HARDWARETEST_DB26_PIN_PWR_PG(DB26_PIN15_PG,1);
			break;
		case HARDWARETEST_DB26_PIN15_PG_0  : /*（0x05）DB26_PIN15 TEST*///a5 a5 00 07 f2 00 00 00 00 01 17 07 //LINE1 0 V Voltage mv
			data_voltage=HARDWARETEST_DB26_PIN_PWR_PG(DB26_PIN15_PG,0);
			break;

		case HARDWARETEST_JMUX0_41: /*（0x05）J41 TEST*///MUX 0  //a5 a5 00 07 f2 00 00 00 00 01 18 07 //LINE1 0 V Voltage mv
		case HARDWARETEST_JMUX0_42:/*（0x05）J42 TEST*/         //a5 a5 00 07 f2 00 00 00 00 02 18 07 //LINE1 0 V Voltage mv
		case HARDWARETEST_JMUX0_43: /*（0x05）J43 TEST*/        //a5 a5 00 07 f2 00 00 00 00 03 18 07 //LINE1 0 V Voltage mv
		case HARDWARETEST_JMUX0_44: /*（0x05）J44 TEST*/        //a5 a5 00 07 f2 00 00 00 00 06 18 07 //LINE1 0 V Voltage mv
		case HARDWARETEST_JMUX0_45: /*（0x05）J45 TEST*/         //a5 a5 00 07 f2 00 00 00 00 07 18 07 //LINE1 0 V Voltage mv
		case HARDWARETEST_JMUX0_46 : /*（0x05）J46 TEST*/       //a5 a5 00 07 f2 00 00 00 00 08 18 07 //LINE1 0 V Voltage mv
		case HARDWARETEST_JMUX0_47 : /*（0x05）J47 TEST*/    //a5 a5 00 07 f2 00 00 00 00 09 18 07 //LINE1 0 V Voltage mv
		case HARDWARETEST_JMUX0_48 : /*（0x05）J45 TEST*/  //a5 a5 00 07 f2 00 00 00 00 0a 18 07 //LINE1 0 V Voltage mv
		case HARDWARETEST_JMUX0_49 : /*（0x05）J46 TEST*/  //a5 a5 00 07 f2 00 00 00 00 0b 18 07 //LINE1 0 V Voltage mv
		case HARDWARETEST_JMUX0_50 : /*（0x05）J47 TEST*/ //a5 a5 00 07 f2 00 00 00 00 0c 18 07 //LINE1 0 V Voltage mv
		case HARDWARETEST_JMUX0_51 : /*（0x05）J47 TEST*/  //a5 a5 00 07 f2 00 00 00 00 0d 18 07 //LINE1 0 V Voltage mv
		case HARDWARETEST_JMUX0_52 : /*（0x05）WIFI TEST*/ //a5 a5 00 07 f2 00 00 00 00 0e 18 07 //LINE1 0 V Voltage mv
		case HARDWARETEST_JMUX0_53 :/*（0x05）WIFI TEST*/   //a5 a5 00 07 f2 00 00 00 00 0f 18 07 //LINE1 0 V Voltage mv
			data_voltage=HARDWARETEST_JMUX0_SERVICE_FUNCTION(hardwarefunctionnum&0xff);
		break;
		
		case HARDWARETEST_JMUX1_66 : /*（0x05）J41 TEST*/ //MUX 1
		case HARDWARETEST_JMUX1_65 : /*（0x05）J42 TEST*/
		case HARDWARETEST_JMUX1_64 :/*（0x05）J43 TEST*/
		case HARDWARETEST_JMUX1_63 : /*（0x05）J44 TEST*/
		case HARDWARETEST_JMUX1_62 : /*（0x05）J45 TEST*/
		case HARDWARETEST_JMUX1_61 : /*（0x05）J46 TEST*/
		case HARDWARETEST_JMUX1_60 : /*（0x05）J47 TEST*/
		case HARDWARETEST_JMUX1_59 : /*（0x05）J45 TEST*/
		case HARDWARETEST_JMUX1_98 : /*（0x05）J46 TEST*/
		case HARDWARETEST_JMUX1_57 : /*（0x05）J47 TEST*/
		case HARDWARETEST_JMUX1_56 : /*（0x05）J47 TEST*/
		case HARDWARETEST_JMUX1_55 : /*（0x05）J47 TEST*/
		case HARDWARETEST_JMUX1_54 : /*（0x05）J47 TEST*/
			data_voltage  = HARDWARETEST_JMUX1_SERVICE_FUNCTION(hardwarefunctionnum&0xff);
		break;
 
		case HARDWARETEST_WIFI:/*（0x05）WIFI TEST*/
		data_voltage  = HARDWARETEST_WIFI_SERVICE_FUNCTION();
		 break;
		case HARDWARETEST_BLUE: /*（0x05）BLUE TEST*/
		data_voltage  = HARDWARETEST_BLUE_SERVICE_FUNCTION();
		 break;
		case HARDWARETEST_RJ45: /*（0x05）RJ45 TEST*/
		 break;
		case HARDWARETEST_DDR: /*（0x05）ddr TEST*/
		 break;
		case HARDWARETEST_FLASH: /*（0x05）flash TEST*/
		 break;
		case HARDWARETEST_ENCODE:/*（0x05）ENCODE TEST*/
		 break;
		case HARDWARETEST_LED:/*（0x05）led TEST*/	
		 break;
		
		default:
		break;
	}
	memcpy(buffer_P,&data_voltage,sizeof(data_voltage));
	
	//Device_SendMessage(Protocol,FUNCTION_HARDWARETEST, number, buffer);
}
