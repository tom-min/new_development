/**
  ******************************************************************************
  * @file    I2C/I2C_TwoBoards/MasterReceiverInterrupt/main.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    13-April-2012
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"
 
/* Private variables ---------------------------------------------------------*/
I2C_InitTypeDef  I2C_InitStructure;
 
/* Private function prototypes -----------------------------------------------*/
static void I2C_Config(void);

/* I2Cx Communication boards Interface */
#define I2Cx                          I2C1
#define I2Cx_CLK                      RCC_APB1Periph_I2C1
#define I2Cx_EV_IRQn                  I2C1_EV_IRQn
#define I2Cx_ER_IRQn                  I2C1_ER_IRQn
#define I2Cx_EV_IRQHandler            I2C1_EV_IRQHandler
#define I2Cx_ER_IRQHandler            I2C1_ER_IRQHandler
  
#define I2Cx_SDA_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define I2Cx_SDA_PIN                  GPIO_Pin_7                
#define I2Cx_SDA_GPIO_PORT            GPIOB                       
#define I2Cx_SDA_SOURCE               GPIO_PinSource7
#define I2Cx_SDA_AF                   GPIO_AF_I2C1
  
#define I2Cx_SCL_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define I2Cx_SCL_PIN                  GPIO_Pin_6                
#define I2Cx_SCL_GPIO_PORT            GPIOB                    
#define I2Cx_SCL_SOURCE               GPIO_PinSource6
#define I2Cx_SCL_AF                   GPIO_AF_I2C1



#define STK8BA53_CHIP_ID 0x00
#define STK8BA53_XYZ 0x02
#define STK8BA53_RANGESEL 0x0F

#define I2C_MASTER


/* Define I2C Speed --------------------------------------------------------*/
#ifdef FAST_I2C_MODE
 #define I2C_SPEED 340000
 #define I2C_DUTYCYCLE I2C_DutyCycle_16_9  
#else /* STANDARD_I2C_MODE*/
 #define I2C_SPEED 100000
 #define I2C_DUTYCYCLE  I2C_DutyCycle_2
#endif /* FAST_I2C_MODE*/
  
/* Define Slave Address  ---------------------------------------------------*/
#ifdef I2C_10BITS_ADDRESS
 #define SLAVE_ADDRESS (uint16_t)0x0330
#else /* I2C_7BITS_ADDRESS */
 #define STK8BA53_ADDRESS (0x18<<1)
#endif /* I2C_10BITS_ADDRESS */
 

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
void USR_I2C_Config(void)
{
	/*At this stage the microcontroller clock setting is already configured, 
	this is done through SystemInit() function which is called from startup
	file (startup_stm32f2xx.s) before to branch to application main.
	To reconfigure the default setting of SystemInit() function, refer to
	system_stm32f2xx.c file
	*/     

  /* I2C configuration ---------------------------------------------------------*/
  I2C_Config();
  

  
/*************************************Master Code******************************/
#if defined (I2C_MASTER)
  /* I2C De-initialize */
  I2C_DeInit(I2Cx);
  
  /*I2C Struct Initialize */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DUTYCYCLE;
  I2C_InitStructure.I2C_OwnAddress1 = 0xa0;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
  
#ifndef I2C_10BITS_ADDRESS
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
#else
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_10bit;
#endif /* I2C_10BITS_ADDRESS */
  
  /*I2C Initialize */
  I2C_Init(I2Cx, &I2C_InitStructure);
  
  /* Enable Error Interrupt */
  I2C_ITConfig(I2Cx, I2C_IT_ERR , DISABLE);
  
  /* I2C ENABLE */
  I2C_Cmd(I2Cx, ENABLE);
 
  #endif /* I2C_MASTER */
  
  }

 
/**
  * @brief  Enables the I2C Clock and configures the different GPIO ports.
  * @param  None
  * @retval None
  */
static void I2C_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
    
  /* RCC Configuration */
  /*I2C Peripheral clock enable */
  RCC_APB1PeriphClockCmd(I2Cx_CLK, ENABLE);
  
  /*SDA GPIO clock enable */
  RCC_AHB1PeriphClockCmd(I2Cx_SDA_GPIO_CLK, ENABLE);
  
  /*SCL GPIO clock enable */
  RCC_AHB1PeriphClockCmd(I2Cx_SCL_GPIO_CLK, ENABLE);
  
  /* Reset I2Cx IP */
  RCC_APB1PeriphResetCmd(I2Cx_CLK, ENABLE);
  
  /* Release reset signal of I2Cx IP */
  RCC_APB1PeriphResetCmd(I2Cx_CLK, DISABLE);
  
  /* GPIO Configuration */
  /*Configure I2C SCL pin */
  GPIO_InitStructure.GPIO_Pin = I2Cx_SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(I2Cx_SCL_GPIO_PORT, &GPIO_InitStructure);
  
  /*Configure I2C SDA pin */
  GPIO_InitStructure.GPIO_Pin = I2Cx_SDA_PIN;
  GPIO_Init(I2Cx_SDA_GPIO_PORT, &GPIO_InitStructure);
    
  /* Connect PXx to I2C_SCL */
  GPIO_PinAFConfig(I2Cx_SCL_GPIO_PORT, I2Cx_SCL_SOURCE, I2Cx_SCL_AF);
  
  /* Connect PXx to I2C_SDA */
  GPIO_PinAFConfig(I2Cx_SDA_GPIO_PORT, I2Cx_SDA_SOURCE, I2Cx_SDA_AF);
  
 
}


void STK8BA53ByteWrite(u8 addr,u8 * data,u8 data_len)  
{  
	u8 i=0;
	
	I2C_GenerateSTART(I2C1,ENABLE);  

	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));  

	I2C_Send7bitAddress(I2C1,STK8BA53_ADDRESS,I2C_Direction_Transmitter);  

	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));  

	I2C_SendData(I2C1,addr);  

	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));  

	for(i=0;i<data_len;i++)
	{

		I2C_SendData(I2C1,data[i]);  
		while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));  
	}

	I2C_GenerateSTOP(I2C1,ENABLE);  
  
}

u8 STK8BA53ByteRead(u8 addr,u8 * data,u8 data_len)  
{  
    
    u8 i=0;
	
    I2C_AcknowledgeConfig(I2C1,ENABLE); //使能应答
    while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));  
    
    I2C_GenerateSTART(I2C1,ENABLE);//起始信号  
  
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));  
	 
    I2C_Send7bitAddress(I2C1,STK8BA53_ADDRESS,I2C_Direction_Transmitter);//发送设备地址+写信号  
  
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//  
     
    I2C_Cmd(I2C1,ENABLE);  
     
    I2C_SendData(I2C1,addr);//发送存储单元地址，从0开始  
  
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));  
  
    I2C_GenerateSTART(I2C1,ENABLE);//起始信号  
  
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));  
  
    I2C_Send7bitAddress(I2C1,STK8BA53_ADDRESS,I2C_Direction_Receiver);//发送设备地址+读信号  
  
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));  
  
 
	for(i=0;i<data_len;i++)
	{
		if(i == data_len-1)  
		{
			I2C_AcknowledgeConfig(I2C1,DISABLE);  

			I2C_GenerateSTOP(I2C1,ENABLE);  
		}
		while(!(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)));  

		data[i]=I2C_ReceiveData(I2C1);//读出寄存器数据  
	}

	I2C_AcknowledgeConfig(I2C1,ENABLE); //使能应答

	return 0;  
}  

static short sx,sy,sz;
u8 CollectSTK8BA53Acceleration(void)  
{
    char data[6];
    u16 x,y,z;
    
    STK8BA53ByteRead(STK8BA53_CHIP_ID,data,1);
   // printfuartdebug("CHIP_ID==%02x  \r\n",data[0]);
    STK8BA53ByteRead(STK8BA53_RANGESEL,data,1);
   // printfuartdebug("range==%02x  \r\n",data[0]);//+-2g
   
    STK8BA53ByteRead(STK8BA53_XYZ,data,6);
    
    x= (u16) (((data[1]<<8)) |data[0])&0xFFFF;
    y= (u16) (((data[3]<<8))|data[2])&0xFFFF;
    z= (u16) (((data[5]<<8))|data[4])&0xFFFF;
    
    x =x/16;
    y =y/16;
    z =z/16;
#if 1
    if(x<2048)
    {
        sx=(x*4000/4096);   
    }
    else
    {
        x=4096-x;
        sx=(x*4000/4096);   
        sx=x*(-1);
    }

    if(y<2048)
    {
        sy=(y*4000/4096);   
    }
    else
    {
        y=4096-y;
        sy=(y*4000/4096);   
        sy=y*(-1);
    }
    if(z<2048)
    {
        sz=(z*4000/4096);   
    }
    else
    {
        z=4096-z;
        sz=(z*4000/4096);   
        sz=z*(-1);
    }
#endif

    //  printfuartdebug("xyz3==%d mg %d mg %d mg \r\n", (sx),(sy),(sz)); //obd

}

u8 GetSTK8BA53Acceleration(short * x_data,short * y_data,short * z_data ) 
{
     *x_data=sx;
     *y_data=sy;
     *z_data=sz;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
