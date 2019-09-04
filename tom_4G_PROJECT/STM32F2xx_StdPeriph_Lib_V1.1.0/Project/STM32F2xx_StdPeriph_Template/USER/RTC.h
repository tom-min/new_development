/**
  ******************************************************************************
  * @file    RTC/TimeStamp/main.h 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    13-April-2012
  * @brief   Header for main.c module
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
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_H_
#define __RTC_H_


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void RTC_Config(void);

void RTC_TimeRegulateSet(RTC_TimeTypeDef RTC_TimeStructure,RTC_DateTypeDef RTC_DateStructure);
void RTC_TimeGet(RTC_TimeTypeDef *RTC_TimeStructure,RTC_DateTypeDef *RTC_DateStructure);

void RTC_TimeShow(void);
void RTC_DateShow(void);
void RTC_TimeStampShow(void);

#endif /* __RTC_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
