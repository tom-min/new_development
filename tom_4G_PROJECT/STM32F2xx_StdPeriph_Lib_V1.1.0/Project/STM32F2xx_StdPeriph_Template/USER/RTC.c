/**
  ******************************************************************************
  * @file    RTC/TimeStamp/main.c 
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
#include "user_stm32f2xx.h"
#include "4G_Common_Api.h"
#include "RTC.h"
/** @addtogroup STM32F2xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup RTC_TimeStamp
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Uncomment the corresponding line to select the RTC Clock source */
#define RTC_CLOCK_SOURCE_LSE   /* LSE used as RTC source clock */
/* #define RTC_CLOCK_SOURCE_LSI */ /* LSI used as RTC source clock. The RTC Clock
                                      may varies due to LSI frequency dispersion. */ 

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
 

__IO uint32_t AsynchPrediv = 0, SynchPrediv = 0;


RTC_TimeTypeDef RTC_TimeStructure={13,30,30,RTC_H12_PM};//13:30:30
RTC_DateTypeDef RTC_DateStructure={3,06,05,19};//2019,06,05,3




/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int RTC_init(void)
{
  
	RTC_InitTypeDef RTC_InitStructure;
 
  /* Output a message on Hyperterminal using printf function */
    if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F2)
  {
    /* RTC configuration  */
    RTC_Config();

    /* Configure the RTC data register and RTC prescaler */
    RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
    RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
   
    /* Check on RTC init */
    if (RTC_Init(&RTC_InitStructure) == ERROR)
    {
      printf("\n\r        /!\\***** RTC Prescaler Config failed ********/!\\ \n\r");
    }

    /* Configure the time register */
    RTC_TimeRegulateSet(RTC_TimeStructure,RTC_DateStructure); 
  }
  else
  {
    /* Check if the Power On Reset flag is set */
    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    {
      printf("\r\nPower On Reset occurred....\n\r");
    }
    /* Check if the Pin Reset flag is set */
    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
    {
      printf("\r\nExternal Reset occurred....\n\r");
    }

    printf("\r\nNo need to configure RTC....\n\r");
    
    /* Enable the PWR clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Allow access to RTC */
    PWR_BackupAccessCmd(ENABLE);

    /* Wait for RTC APB registers synchronisation */
    RTC_WaitForSynchro();

    /* Clear the RTC Alarm Flag */
    RTC_ClearFlag(RTC_FLAG_ALRAF);

    /* Clear the EXTI Line 17 Pending bit (Connected internally to RTC Alarm) */
    EXTI_ClearITPendingBit(EXTI_Line17);

    /* Display the RTC Time/Date and TimeStamp Time/Date */
    //RTC_TimeShow();
    //RTC_DateShow();
    //RTC_TimeStampShow();
  }
   

}


int RTC_main(void)
{
	RTC_TimeTypeDef RTC_TimeStructure={13,30,30,RTC_H12_PM};//13:30:30
	RTC_DateTypeDef RTC_DateStructure={3,06,05,19};//2019,06,05,3

	RTC_TimeRegulateSet(RTC_TimeStructure,RTC_DateStructure);
	RTC_TimeGet(&RTC_TimeStructure,&RTC_DateStructure);
}



/**
  * @brief  Configure the RTC peripheral by selecting the clock source.
  * @param  None
  * @retval None
  */
void RTC_Config(void)
{

  /* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /* Allow access to RTC */
  PWR_BackupAccessCmd(ENABLE);

#if defined (RTC_CLOCK_SOURCE_LSI)  /* LSI used as RTC source clock*/
/* The RTC Clock may varies due to LSI frequency dispersion. */
  /* Enable the LSI OSC */ 
  RCC_LSICmd(ENABLE);

  /* Wait till LSI is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {
  }

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

  SynchPrediv = 0xFF;
  AsynchPrediv = 0x7F;

#elif defined (RTC_CLOCK_SOURCE_LSE) /* LSE used as RTC source clock */
  /* Enable the LSE OSC */
  RCC_LSEConfig(RCC_LSE_ON);

  /* Wait till LSE is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {
  }

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

  SynchPrediv = 0xFF;
  AsynchPrediv = 0x7F;
    
#else
  #error Please select the RTC Clock source inside the main.c file
#endif /* RTC_CLOCK_SOURCE_LSI */

  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();

  /* Enable The TimeStamp */
  RTC_TimeStampCmd(RTC_TimeStampEdge_Falling, ENABLE);    
}

/**
  * @brief  Returns the time entered by user, using Hyperterminal.
  * @param  None
  * @retval None
  */
void RTC_TimeRegulateSet(RTC_TimeTypeDef RTC_TimeStructure,RTC_DateTypeDef RTC_DateStructure)
{

	// 2019_04_11 13:00:00 4
	printf("  Please Set WeekDay (01-07)\n\r");
	printf("  Please Set Date (01-31)\n\r");
	printf("  Please Set Month (01-12)\n\r");
	printf("  Please Set Year (00-99)\n\r");
   

	RTC_TimeStructure.RTC_H12     = RTC_H12_PM;

	/* Configure the RTC time register */
	if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR)
	{
		printf("\n\r>> !! RTC Set Time failed. !! <<\n\r");
	} 
	else
	{
		printf("\n\r>> !! RTC Set Time success. !! <<\n\r");
		 RTC_TimeShow();
		/* Indicator for the RTC configuration */
		RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
	}
  

  /* Configure the RTC date register */
  if(RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure) == ERROR)
  {
    	printf("\n\r>> !! RTC Set Date failed. !! <<\n\r");
  } 
  else
  {
	    printf("\n\r>> !! RTC Set Date success. !! <<\n\r");
	    RTC_DateShow();
	    /* Indicator for the RTC configuration */
	    RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
  }

}



int getWeekdayByYearday(int iY, int iM, int iD) 
{
    int iWeekDay = -1; 
    if (1 == iM || 2 == iM) 
    {   
        iM += 12; 
        iY--;
    }   
	iWeekDay = (iD + 1 + 2 * iM + 3 * (iM + 1) / 5 + iY + iY / 4 - iY / 100 + iY / 400) % 7;
#if 0
	switch(iWeekDay)
	{   
		case 0 : printf( "Sunday"); break;
		case 1 : printf( "Monday"); break;
		case 2 : printf( "Tuesday"); break;
		case 3 : printf( "Wednesday"); break;
		case 4 : printf( "Thursday"); break;
		case 5 : printf( "Friday"); break;                                                             
		case 6 : printf( "Saturday"); break;
		default :break;
	} 
#endif
	if(iWeekDay==0)
	{
		iWeekDay=7;
	}
	return iWeekDay;
}

void RTC_TimeRegulateSetviaEc20(t_style t)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
	

	RTC_DateStructure.RTC_Year=(uint8_t)(t.year-2000);
	RTC_DateStructure.RTC_Month=(uint8_t)(t.month);
	RTC_DateStructure.RTC_Date=(uint8_t)(t.date);
	  
	RTC_TimeStructure.RTC_Hours=(uint8_t)(t.hour);
	RTC_TimeStructure.RTC_Minutes=(uint8_t)(t.min);
	RTC_TimeStructure.RTC_Seconds=(uint8_t)(t.sec);


	RTC_DateStructure.RTC_WeekDay=getWeekdayByYearday(t.year,t.month,t.date);

#if 0
	// 2019_04_11 13:00:00 4
	printf("  Please Set WeekDay (01-07)\n\r");
	printf("  Please Set Date (01-31)\n\r");
	printf("  Please Set Month (01-12)\n\r");
	printf("  Please Set Year (00-99)\n\r");
   #endif

      if(RTC_TimeStructure.RTC_Hours<12)
      	{
		RTC_TimeStructure.RTC_H12   = RTC_H12_PM;
      	}

      if(RTC_TimeStructure.RTC_Hours>=12)
	{
		RTC_TimeStructure.RTC_H12  = RTC_H12_AM;
      	}


	/* Configure the RTC time register */
	if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR)
	{
		//printf("\n\r>> !! RTC Set Time failed. !! <<\n\r");
	} 
	else
	{
		//printf("\n\r>> !! RTC Set Time success. !! <<\n\r");
		// RTC_TimeShow();
		/* Indicator for the RTC configuration */
		RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
	}
  

  /* Configure the RTC date register */
  if(RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure) == ERROR)
  {
    	//printf("\n\r>> !! RTC Set Date failed. !! <<\n\r");
  } 
  else
  {
	  //  printf("\n\r>> !! RTC Set Date success. !! <<\n\r");
	  //  RTC_DateShow();
	    /* Indicator for the RTC configuration */
	    RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
  }

}




void RTC_TimeGet(RTC_TimeTypeDef *RTC_TimeStructure,RTC_DateTypeDef *RTC_DateStructure)
{
	RTC_GetTime(RTC_Format_BIN, RTC_TimeStructure);
	(void)RTC->DR;

	RTC_GetDate(RTC_Format_BIN, RTC_DateStructure);
	//printf("\n\r  The current time (Hour-Minute-Second) is :  %0.2d:%0.2d:%0.2d \n\r", RTC_TimeStructure->RTC_Hours, RTC_TimeStructure->RTC_Minutes, RTC_TimeStructure->RTC_Seconds);
	//printf("\n\r  The current date (WeekDay-Date-Month-Year) is :  %0.2d-%0.2d-%0.2d-%0.2d \n\r", RTC_DateStructure->RTC_WeekDay, RTC_DateStructure->RTC_Date, RTC_DateStructure->RTC_Month, RTC_DateStructure->RTC_Year); 
}

void RTC_TimeGetStr(char *time_buf)
{

	RTC_TimeTypeDef  Time={13,30,30,RTC_H12_PM};//13:30:30
	RTC_DateTypeDef Date={3,06,05,19};//2019,06,05,3
	
	RTC_GetTime(RTC_Format_BIN, &Time);
	(void)RTC->DR;
	RTC_GetDate(RTC_Format_BIN, &Date);

	sprintf(time_buf,"%04d-%02d-%02d %02d:%02d:%02d",(Date.RTC_Year)+2000,(Date.RTC_Month),(Date.RTC_Date),(Time.RTC_Hours),(Time.RTC_Minutes),(Time.RTC_Seconds));
	

}
void mktimeset(t_style *p_t)
{

	RTC_TimeTypeDef  Time;//13:30:30
	RTC_DateTypeDef Date;//2019,06,05,3
	
	RTC_GetTime(RTC_Format_BIN, &Time);
	(void)RTC->DR;
	RTC_GetDate(RTC_Format_BIN, &Date);
	
 	p_t->year=(Date.RTC_Year)+2000;	
	p_t->month=(Date.RTC_Month);
	p_t->date=(Date.RTC_Date);
	p_t->hour=(Time.RTC_Hours);
	p_t->min=(Time.RTC_Minutes);
	p_t->sec=(Time.RTC_Seconds);

}

/**
  * @brief  Display the current time on the Hyperterminal.
  * @param  None
  * @retval None
  */
void RTC_TimeShow(void)
{
  RTC_TimeTypeDef RTC_TimeStructure;
	/* Get the current Time and Date */
  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
  //printf("\n\r============== Current Time Display ============================\n\r");
 // printf("\n\r  The current time (Hour-Minute-Second) is :  %0.2d:%0.2d:%0.2d \n\r", RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
  /* Unfreeze the RTC DR Register */
  (void)RTC->DR;
}

/**
  * @brief  Display the current date on the Hyperterminal.
  * @param  None
  * @retval None
  */
void RTC_DateShow(void)
{
	RTC_DateTypeDef RTC_DateStructure;
  /* Get the current Date */
  RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
  printf("\n\r============== Current Date Display ============================\n\r");
  printf("\n\r  The current date (WeekDay-Date-Month-Year) is :  %0.2d-%0.2d-%0.2d-%0.2d \n\r", RTC_DateStructure.RTC_WeekDay, RTC_DateStructure.RTC_Date, RTC_DateStructure.RTC_Month, RTC_DateStructure.RTC_Year);
}


/**
  * @brief  Display the current TimeStamp (time and date) on the Hyperterminal.
  * @param  None
  * @retval None
  */
  
  RTC_TimeTypeDef  RTC_TimeStampStructure;
RTC_DateTypeDef  RTC_TimeStampDateStructure;
void RTC_TimeStampShow(void)
{
 
  
  /* Get the current TimeStamp */
  RTC_GetTimeStamp(RTC_Format_BIN, &RTC_TimeStampStructure, &RTC_TimeStampDateStructure);
  printf("\n\r==============TimeStamp Display (Time and Date)=================\n\r");
  printf("\n\r  The current time stamp time (Hour-Minute-Second) is :  %0.2d:%0.2d:%0.2d \n\r", RTC_TimeStampStructure.RTC_Hours, RTC_TimeStampStructure.RTC_Minutes, RTC_TimeStampStructure.RTC_Seconds);
  printf("\n\r  The current timestamp date (WeekDay-Date-Month) is :  %0.2d-%0.2d-%0.2d \n\r", RTC_TimeStampDateStructure.RTC_WeekDay, RTC_TimeStampDateStructure.RTC_Date, RTC_TimeStampDateStructure.RTC_Month);
}


 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
