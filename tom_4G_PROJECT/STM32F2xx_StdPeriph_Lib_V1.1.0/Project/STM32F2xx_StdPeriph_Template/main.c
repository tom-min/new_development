/**
  ******************************************************************************
  * @file    Project/STM32F2xx_StdPeriph_Template/main.c
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
#include "user_stm32f2xx.h"
#include "user_queue.h"
#include "user_config.h"
#include "config.h"
#include "ff.h"
#include "stdio.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "string.h"
#include "stdarg.h"
#include "App.h"
#include "userflashconfig.h"
#include "Fcar_Api.h"
#include "stm322xg_eval.h"
#include "userfatfs.h"
#include "iwdg.h"


#ifdef BOOT_APPLACATION
//extern  const  U_USER_DATA g_u_userdata;// __attribute__((at(FLASH_USER_DATA_START_ADDR)))={0xaaaaaa,0xaaaaaa};
#endif
/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MESSAGE1   "     STM32F2xx      "
#define MESSAGE2   " Device running on  "
#define MESSAGE3   "   STM322xG-EVAL    "
extern void user_SysTick_Handler(void);
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;

TaskHandle_t  data_pack_task;
TaskHandle_t  data_transfer_task;
TaskHandle_t  Task_ObdiiCmdProcess_Proc_handle;

TaskHandle_t  u_box_task;

TimerHandle_t  test1Timer_Handle;
TimerHandle_t  test2Timer_Handle;
TimerHandle_t  test3Timer_Handle;
TimerHandle_t  test4Timer_Handle;

uint8_t  GPS_timer_flag = 0;
uint16_t Vehicle_diagnosis_timer = 0;
uint16_t Data_collection_timer = 0;
uint8_t  Fence_Warning_timer = 0;

typedef  void (*pfun)(void);

void _MainRoute(void);
extern void TaskCom_Proc(void *pvParameters);
extern void _MainRoute2(void *pvParameters);
void TaskCan_Proc(void *pvParameters);
void Task_ObdiiCmdProcess_Proc(void);
extern void u_blox_Test(void *pvParameters);

extern void mmain(void);
extern void Data_transfer(void *pvParameters);
extern void Data_pack(void *pvParameters);
extern void main_xml2(void);
extern int main_SPI(void);

extern USART_TypeDef* COM_USART[COMn];
extern Vehicle_Model_Info tom_vehicle_model_data;


void test1Timer_HandleCallback(TimerHandle_t xTimer)
{
    GPS_timer_flag = 1;
//		printf("t1\r\n");
//    vTaskDelay(2);
}

void test2Timer_HandleCallback(TimerHandle_t xTimer)
{
    Vehicle_diagnosis_timer++;
//		printf("t2\r\n");
//		vTaskDelay(2);
}

void test3Timer_HandleCallback(TimerHandle_t xTimer)
{
    Data_collection_timer++;
//		printf("t3\r\n");
//		vTaskDelay(2);
}

void test4Timer_HandleCallback(TimerHandle_t xTimer)
{
    Fence_Warning_timer++;
//		printf("t4\r\n");
//		vTaskDelay(2);
}

void appmain_jmp(void)
{
    void (*jumpappmain)();
    uint32_t *p;

    if( UserJumpToAppCheckVerion(&tom_vehicle_model_data) == PASSED)
    {
        FileuUnMount();
        USART_ITConfig(COM_USART[COM4], USART_IT_RXNE, DISABLE);
        USART_ITConfig(COM_USART[COM3], USART_IT_RXNE, DISABLE);
 	 ADC_DMACmd(ADC1, DISABLE);
        __set_FAULTMASK(1);//关闭总中断
        printf("\r\n[%s] UserJumpToAppCheckVerion success.\r\n",__func__);
        p = (uint32_t*)(NVIC_VectTab_FLASH + APP_OFFSET + 4);
        jumpappmain = (pfun) *p;
        __set_MSP(*(uint32_t*)(NVIC_VectTab_FLASH + APP_OFFSET));	//设置主堆栈指针
        jumpappmain(); //跳转失败???，如果失败会自动重启，跳回0x8000000
    }
    else
    {
        printf("\r\n[%s] app jump failed,User run in Boot\r\n",__func__);
        printf("\r\nBoot version = %s\r\n",BOOT_VERSION);
    }
}

void Cmdjmpappmain(void)
{
    void (*jumpappmain)();
    uint32_t *p;

    FileuUnMount();
    USART_ITConfig(COM_USART[COM4], USART_IT_RXNE, DISABLE);
    USART_ITConfig(COM_USART[COM3], USART_IT_RXNE, DISABLE);
    ADC_DMACmd(ADC1, DISABLE);
    __set_FAULTMASK(1);//关闭总中断
    printf("\r\n[%s] UserJumpToAppCheckVerion success.\r\n",__func__);
    p = (uint32_t*)(NVIC_VectTab_FLASH + APP_OFFSET + 4);
    jumpappmain = (pfun) *p;
    __set_MSP(*(uint32_t*)(NVIC_VectTab_FLASH + APP_OFFSET));	//设置主堆栈指针
    jumpappmain(); //跳转失败???，如果失败会自动重启，跳回0x8000000


}
int main_FreeRTos(void)
{
    mmain();
    xTaskCreate(_MainRoute2, "_MainRoute2", 400, NULL, 1, NULL);
    xTaskCreate(TaskCom_Proc, "TaskCom_Proc", 400, NULL,2, NULL);
    xTaskCreate(Task_ObdiiCmdProcess_Proc, "Task_ObdiiCmdPro", 400, NULL,2, &Task_ObdiiCmdProcess_Proc_handle);
    xTaskCreate(Data_pack, "Data_pack", 1500, NULL,3, &data_pack_task);
    xTaskCreate(Data_transfer, "Data_transfer", 500, NULL,3, &data_transfer_task);
    ///xTaskCreate(u_blox_Test, "u_blox", 400, NULL ,3, &u_box_task);
#define mainDONT_BLOCK	 ( 0U )

    test1Timer_Handle = xTimerCreate((const char*)"test1Timer_Handle",
                                     (TickType_t	)2000,
                                     (UBaseType_t)pdTRUE,
                                     (void *)1,
                                     (TimerCallbackFunction_t)test1Timer_HandleCallback);
    if (test1Timer_Handle == NULL)
    {
        printf("Failed to test1Timer_Handle\r\n");
    }
    else
    {
//				xTimerStart(test1Timer_Handle,mainDONT_BLOCK);
        printf("success to test1Timer_Handle\r\n");
    }

    test2Timer_Handle = xTimerCreate((const char*)"test2Timer_Handle",
                                     (TickType_t	)1000,
                                     (UBaseType_t)pdTRUE,	//周期模式
                                     (void *)1,
                                     (TimerCallbackFunction_t)test2Timer_HandleCallback);
    if (test2Timer_Handle == NULL)
    {
        printf("Failed to test2Timer_Handle\r\n");
    }
    else
    {
        printf("success to test2Timer_Handle\r\n");
    }

    test3Timer_Handle = xTimerCreate((const char*)"test3Timer_Handle",
                                     (TickType_t	)1000,
                                     (UBaseType_t)pdTRUE,
                                     (void *)1,
                                     (TimerCallbackFunction_t)test3Timer_HandleCallback);
    if (test3Timer_Handle == NULL)
    {
        printf("Failed to test3Timer_Handle\r\n");
    }
    else
    {
        printf("success to test3Timer_Handle\r\n");
    }

    test4Timer_Handle = xTimerCreate((const char*)"test4Timer_Handle",
                                     (TickType_t	)1000,
                                     (UBaseType_t)pdTRUE,
                                     (void *)1,
                                     (TimerCallbackFunction_t)test4Timer_HandleCallback);
    if (test4Timer_Handle == NULL)
    {
        printf("Failed to test4Timer_Handle\r\n");
    }
    else
    {
//				xTimerStart(test4Timer_Handle,mainDONT_BLOCK);
        printf("success to test4Timer_Handle\r\n");
    }

    vTaskStartScheduler();
    return 0;
}


void NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the USARTx Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = EVAL_COM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable the USART2 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = EVAL_COM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable the USART3 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = EVAL_COM1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


void NVIC_Config_OBD(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the USARTx Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = EVAL_COM5_IRQn ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable the USARTx Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = EVAL_COM6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */

int fputc(int ch,FILE *f)
{
#ifndef PRINTF_DEBUG
    return;
#endif
    //while(USART_GetFlagStatus(UART_DBUG,USART_FLAG_TC) != SET);
    USART_SendData(PRINTF_DEBUG1_UART,(unsigned char)ch);
    while(USART_GetFlagStatus(PRINTF_DEBUG1_UART,USART_FLAG_TC) != SET);

    return (ch);
}


void Config_blox_usart(uint32_t baudrate )
{
    USART_InitTypeDef USART_InitStructure;
    USART_ITConfig(COM_USART[COM1], USART_IT_RXNE, DISABLE);//u-blox M
    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    STM_EVAL_COMInit(COM1, &USART_InitStructure);//u-blox M
    USART_ITConfig(COM_USART[COM1], USART_IT_RXNE, ENABLE);//u-blox M
}
int main(void)
{
    void (*jumpappmain)();
    uint32_t *p;
    RCC_ClocksTypeDef RCC_Clocks;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NvicInitdef;

#ifdef APP_APPLACATION
    {
        printf("[%s] tom_min enter [%s] VEHICLE MODEL successfully,hahaha\r\n",__func__,VEHICLE_MODEL);
        NVIC_SetVectorTable(NVIC_VectTab_FLASH, APP_OFFSET);//使能偏移位
    }
#endif

#ifdef APP_APPLACATION //解决APP 无法进入中断问题。
    __enable_irq();
    //RCC_DeInit();
    GPIO_DeInit(GPIOA);
    GPIO_DeInit(GPIOB);
    GPIO_DeInit(GPIOC);
    GPIO_DeInit(GPIOD);
    GPIO_DeInit(GPIOE);
#endif

    STM_EVAL_LEDInit(LED1);
    STM_EVAL_LEDInit(LED2);
    STM_EVAL_LEDInit(LED3);
    NVIC_Config();
    NVIC_Config_OBD();

    /*!< At this stage the microcontroller clock setting is already configured,
    	this is done through SystemInit() function which is called from startup
    	file (startup_stm32f2xx.s) before to branch to application main.
    	To reconfigure the default setting of SystemInit() function, refer to
    	system_stm32f2xx.c file
    */

    /* SysTick end of count event each 10ms */

    //NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00000);//使能偏移位

    RCC_GetClocksFreq(&RCC_Clocks);
    //SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);//1ms

#if 1
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    STM_EVAL_COMInit(COM4, &USART_InitStructure);
    STM_EVAL_COMInit(COM3, &USART_InitStructure);
    STM_EVAL_COMInit(COM1, &USART_InitStructure);

    /* Enable the EVAL_COM1 Transmit interrupt: this interrupt is generated when the
    EVAL_COM1 transmit data register is empty */
    //USART_ITConfig(COM_USART[COM4], USART_IT_TXE, ENABLE);
    USART_ITConfig(COM_USART[COM4], USART_IT_RXNE, ENABLE);
    USART_ITConfig(COM_USART[COM3], USART_IT_RXNE, ENABLE);
    USART_ITConfig(COM_USART[COM1], USART_IT_RXNE, ENABLE);
#endif

    RTC_init();

#ifdef APP_APPLACATION
    {
        printf("\r\n[%s] tom_min User run in App...\r\n",__func__);
        printf("\r\nApp version = %s\r\n",APP_VERSION);
    }
#endif

#ifdef BOOT_APPLACATION
    {
        printf("\r\n[%s] tom_min User run in Boot...\r\n",__func__);
        printf("\r\nBoot version = %s\r\n",BOOT_VERSION);
    }
#endif

    printf("\r\nSYSCLK_Frequency = %d MHz\n",RCC_Clocks.SYSCLK_Frequency/1000000);
    printf("\r\nHCLK_Frequency = %d MHz\n",RCC_Clocks.HCLK_Frequency/1000000);
    printf("\r\nPCLK1_Frequency = %d MHz\n",RCC_Clocks.PCLK1_Frequency/1000000);
    printf("\r\nPCLK2_Frequency = %d MHz\n",RCC_Clocks.PCLK2_Frequency/1000000);

    main_SPI();
    fatfstest();

#ifdef BOOT_APPLACATION
    //写boot版本到flash中，地址在起始地址为0x0800C000所在的扇区
    if(UserBootVerionSave(0,1) == PASSED)
    {
        printf("\r\nUserBootVerionSave() success.\r\n");
    }
    else
    {
        printf("\r\nUserBootVerionSave() error.\r\n");
    }
#endif

    main_xml2();
//		iwdg_init();
//		iwdg_eable();
    main_FreeRTos();

#if 1
    __set_FAULTMASK(1);//关闭总中断
    NVIC_SystemReset();//请求单片机重启
#endif
}


void POWER_EN_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	//PB15
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_SetBits(GPIOB,GPIO_Pin_15);
    vTaskDelay(200);
}

void POWER_UBOX_EN_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	//PB11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_SetBits(GPIOB,GPIO_Pin_11);
    vTaskDelay(200);
}

void LTE_Module_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    /* Enable the GSM_POWER_PIN Clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    /* Configure the GSM_POWER_PIN pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;		//PB1,POWERKEY
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Enable the GSM_PWR_PIN Clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    /* Configure the GSM_PWR_PIN pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15|GPIO_Pin_5;	//PA15,RESET
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Enable the GSM_PWR_PIN Clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    /* Configure the GSM_PWR_PIN pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;	//PB3,RESET
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOB,GPIO_Pin_3);
    GPIO_ResetBits(GPIOB,GPIO_Pin_4);
    GPIO_SetBits(GPIOA,GPIO_Pin_5);
    vTaskDelay(500);

    //  GPIO_SetBits(GPIOB,GPIO_Pin_1);
    GPIO_ResetBits(GPIOB,GPIO_Pin_1);
    //  GPIO_SetBits(GPIOA,GPIO_Pin_15);
    GPIO_ResetBits(GPIOA,GPIO_Pin_15);
    vTaskDelay(200);
    //  GPIO_ResetBits(GPIOB,GPIO_Pin_1);
    GPIO_SetBits(GPIOB,GPIO_Pin_1);
    vTaskDelay(3000);
    //	GPIO_SetBits(GPIOB,GPIO_Pin_1);
    GPIO_ResetBits(GPIOB,GPIO_Pin_1);

    printf("%s ok\r\n",__func__);
    vTaskDelay(5000);
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
