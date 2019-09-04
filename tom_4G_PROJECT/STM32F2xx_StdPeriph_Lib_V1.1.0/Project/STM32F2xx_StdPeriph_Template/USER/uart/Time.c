
#include "Time.h"
#include "config.h"
#include "user_stm32f2xx.h"


uint32_t T_delay_ms_get_status(void);
uint32_t T_delay_us_get_status(void);

#define T_delay_Timer TIM2  //32BITS
#define T_delay_TimerAPB RCC_APB1Periph_TIM2
#define T_delay_Timer_irq TIM2_IRQn

#define T_DelayUs_Timer TIM4  //16BITS
#define T_DelayUs_TimerAPB RCC_APB1Periph_TIM4
#define T_DelayUs_Timer_IRQ TIM4_IRQn 

void TIM_Configuration(void)  //1ms
{
	 NVIC_InitTypeDef NVIC_InitStructure1;
	 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	 RCC_APB1PeriphClockCmd(T_delay_TimerAPB, ENABLE);

	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	 NVIC_InitStructure1.NVIC_IRQChannel = T_delay_Timer_irq;
	 NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority = 3;
	 NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 3;
	 NVIC_InitStructure1.NVIC_IRQChannelCmd = DISABLE;
	 NVIC_InitStructure1.NVIC_IRQChannelCmd = ENABLE;
	 NVIC_Init(&NVIC_InitStructure1);

	 TIM_DeInit(T_delay_Timer);
	 TIM_TimeBaseStructure.TIM_Period = (60000-1);
	 TIM_TimeBaseStructure.TIM_Prescaler = 0;
	 TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	 TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	 TIM_TimeBaseInit(T_delay_Timer, &TIM_TimeBaseStructure);

	 TIM_ClearFlag(T_delay_Timer, TIM_FLAG_Update);

	 TIM_ITConfig(T_delay_Timer, TIM_IT_Update, ENABLE);

	 TIM_Cmd(T_delay_Timer, ENABLE);
}


void T_delay_ms(uint16  n_ms)  //1ms
{
	 NVIC_InitTypeDef NVIC_InitStructure1;
	 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	 RCC_APB1PeriphClockCmd(T_delay_TimerAPB, ENABLE);

	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	 NVIC_InitStructure1.NVIC_IRQChannel = T_delay_Timer_irq;
	 NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority = 0;
	 NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 1;
	 NVIC_InitStructure1.NVIC_IRQChannelCmd = DISABLE;
	 NVIC_Init(&NVIC_InitStructure1);

	 TIM_DeInit(T_delay_Timer);

	 TIM_TimeBaseStructure.TIM_Period = (n_ms - 1);
	 TIM_TimeBaseStructure.TIM_Prescaler = (60000-1);//1m
	 TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	 TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	 TIM_TimeBaseInit(T_delay_Timer, &TIM_TimeBaseStructure);

	 TIM_ClearFlag(T_delay_Timer, TIM_FLAG_Update);

	 TIM_ITConfig(T_delay_Timer, TIM_IT_Update, ENABLE);

	 TIM_Cmd(T_delay_Timer, ENABLE);
	 
	 while(T_delay_ms_get_status()==RESET);
}
void T_delay_ms_set(uint16  n_ms)  //1ms
{
	 NVIC_InitTypeDef NVIC_InitStructure1;
	 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	 RCC_APB1PeriphClockCmd(T_delay_TimerAPB, ENABLE);

	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	 NVIC_InitStructure1.NVIC_IRQChannel = T_delay_Timer_irq;
	 NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority = 0;
	 NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 1;
	 NVIC_InitStructure1.NVIC_IRQChannelCmd = DISABLE;
	 NVIC_Init(&NVIC_InitStructure1);
	 
        RCC_APB1PeriphClockCmd(T_delay_TimerAPB, ENABLE);
		 
	 TIM_DeInit(T_delay_Timer);

	 TIM_TimeBaseStructure.TIM_Period = (n_ms - 1);
	 TIM_TimeBaseStructure.TIM_Prescaler = (60000-1);//1m
	 TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	 TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	 TIM_TimeBaseInit(T_delay_Timer, &TIM_TimeBaseStructure);

	 TIM_ClearFlag(T_delay_Timer, TIM_FLAG_Update);

	 TIM_ITConfig(T_delay_Timer, TIM_IT_Update, ENABLE);

	 TIM_Cmd(T_delay_Timer, ENABLE);
	 
}

void T_delay_us(uint16  n_us)  //1us
{
	 NVIC_InitTypeDef NVIC_InitStructure1;
	 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	 RCC_APB1PeriphClockCmd(T_DelayUs_TimerAPB, ENABLE);

	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	 NVIC_InitStructure1.NVIC_IRQChannel = T_DelayUs_Timer_IRQ;
	 NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority = 0;
	 NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 1;
	 NVIC_InitStructure1.NVIC_IRQChannelCmd = DISABLE;
	 NVIC_Init(&NVIC_InitStructure1);

	 TIM_DeInit(T_DelayUs_Timer);

	 TIM_TimeBaseStructure.TIM_Period = n_us - 1;
	 TIM_TimeBaseStructure.TIM_Prescaler = (Fpclk/1000000-1);//1us;
	 TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	 TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	 TIM_TimeBaseInit(T_DelayUs_Timer, &TIM_TimeBaseStructure);

	 TIM_ClearFlag(T_DelayUs_Timer, TIM_FLAG_Update);

	 TIM_ITConfig(T_DelayUs_Timer, TIM_IT_Update, ENABLE);

	 TIM_Cmd(T_DelayUs_Timer, ENABLE);
	 while(T_delay_us_get_status()==RESET);
}


void T_delay_us_Set(uint16  n_us)  //1us
{
	 NVIC_InitTypeDef NVIC_InitStructure1;
	 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	 RCC_APB1PeriphClockCmd(T_DelayUs_TimerAPB, ENABLE);

	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	 NVIC_InitStructure1.NVIC_IRQChannel = T_DelayUs_Timer_IRQ;
	 NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority = 0;
	 NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 1;
	 NVIC_InitStructure1.NVIC_IRQChannelCmd = DISABLE;
	 NVIC_Init(&NVIC_InitStructure1);

	 TIM_DeInit(T_DelayUs_Timer);

	 TIM_TimeBaseStructure.TIM_Period = n_us - 1;
	 TIM_TimeBaseStructure.TIM_Prescaler = (Fpclk/1000000-1);//1us;
	 TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	 TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	 TIM_TimeBaseInit(T_DelayUs_Timer, &TIM_TimeBaseStructure);

	 TIM_ClearFlag(T_DelayUs_Timer, TIM_FLAG_Update);

	 TIM_ITConfig(T_DelayUs_Timer, TIM_IT_Update, ENABLE);

	 TIM_Cmd(T_DelayUs_Timer, ENABLE);
	 
}



void T_delay_us_irq(void)
{
	if( TIM_GetITStatus(T_DelayUs_Timer,TIM_IT_Update) != RESET )
	{
		TIM_ClearITPendingBit(T_DelayUs_Timer,TIM_IT_Update);
	}
}
void T_delay_ms_irq(void)
{
	if( TIM_GetITStatus(T_delay_Timer,TIM_IT_Update) != RESET )
	{
		TIM_ClearITPendingBit(T_delay_Timer,TIM_IT_Update);
	}
}

void T_delay_us_init(void)  //in us
{
	 NVIC_InitTypeDef NVIC_InitStructure1;
	 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	 RCC_APB1PeriphClockCmd(T_DelayUs_TimerAPB, ENABLE);

	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	 NVIC_InitStructure1.NVIC_IRQChannel = T_DelayUs_Timer_IRQ;
	 NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority = 0;
	 NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 1;
	 NVIC_InitStructure1.NVIC_IRQChannelCmd = DISABLE;
	 NVIC_Init(&NVIC_InitStructure1);

	 TIM_DeInit(T_DelayUs_Timer);

	 TIM_TimeBaseStructure.TIM_Period = 0xffff;
	 TIM_TimeBaseStructure.TIM_Prescaler = (Fpclk/1000000-1);//1us;
	 TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	 TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	 TIM_TimeBaseInit(T_DelayUs_Timer, &TIM_TimeBaseStructure);

	 TIM_ClearFlag(T_DelayUs_Timer, TIM_FLAG_Update);

	 TIM_ITConfig(T_DelayUs_Timer, TIM_IT_Update, ENABLE);

	 TIM_Cmd(T_DelayUs_Timer, ENABLE);
}

void T_delay_ms_init(void)  //in ms
{
	 NVIC_InitTypeDef NVIC_InitStructure1;
	 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	 RCC_APB1PeriphClockCmd(T_delay_TimerAPB, ENABLE);

	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	 NVIC_InitStructure1.NVIC_IRQChannel = T_delay_Timer_irq;
	 NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority = 0;
	 NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 1;
	 NVIC_InitStructure1.NVIC_IRQChannelCmd = DISABLE;
	 NVIC_Init(&NVIC_InitStructure1);

	 TIM_DeInit(T_delay_Timer);

	 TIM_TimeBaseStructure.TIM_Period = 0xffff;
	 TIM_TimeBaseStructure.TIM_Prescaler = (60000-1);//1ms;
	 TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	 TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	 TIM_TimeBaseInit(T_delay_Timer, &TIM_TimeBaseStructure);

	 TIM_ClearFlag(T_delay_Timer, TIM_FLAG_Update);

	 TIM_ITConfig(T_delay_Timer, TIM_IT_Update, ENABLE);

	 TIM_Cmd(T_delay_Timer, ENABLE);
}


uint32_t T_delay_ms_get_count(void)
{
	return  TIM_GetCounter(T_delay_Timer);
}

uint32_t T_delay_us_get_count(void)
{
	return TIM_GetCounter(T_DelayUs_Timer);
}

uint32_t T_delay_us_clear_count(void)
{
	TIM_SetCounter(T_DelayUs_Timer,0);
}
uint32_t T_delay_ms_clear_count(void)
{
	TIM_SetCounter(T_delay_Timer,0);
}

uint32_t T_delay_ms_get_status(void)
{
	if( TIM_GetITStatus(T_delay_Timer,TIM_IT_Update) != RESET )
	{
		TIM_ClearITPendingBit(T_delay_Timer,TIM_IT_Update);
		return  SET;
	}
	else return RESET;
}
uint32_t T_delay_us_get_status(void)
{
	if( TIM_GetITStatus(T_DelayUs_Timer,TIM_IT_Update) != RESET )
	{
		TIM_ClearITPendingBit(T_DelayUs_Timer,TIM_IT_Update);
		return  SET;
	}
	else return RESET;
}

void Timer_PWM_VPM_init(void)  //in (/75us)
{
	 NVIC_InitTypeDef NVIC_InitStructure1;
	 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	 RCC_APB1PeriphClockCmd(T_DelayUs_Timer_IRQ, ENABLE);

	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	 NVIC_InitStructure1.NVIC_IRQChannel = T_DelayUs_Timer_IRQ;
	 NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority = 0;
	 NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 1;
	 NVIC_InitStructure1.NVIC_IRQChannelCmd = DISABLE;
	 NVIC_Init(&NVIC_InitStructure1);

	 TIM_DeInit(T_DelayUs_Timer);

	 TIM_TimeBaseStructure.TIM_Period = 0xffff;
	 TIM_TimeBaseStructure.TIM_Prescaler = (1-1);//1us;
	 TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	 TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	 TIM_TimeBaseInit(T_DelayUs_Timer, &TIM_TimeBaseStructure);

	 TIM_ClearFlag(T_DelayUs_Timer, TIM_FLAG_Update);

	 TIM_ITConfig(T_DelayUs_Timer, TIM_IT_Update, ENABLE);

	 TIM_Cmd(T_DelayUs_Timer, ENABLE);
}

void Timer_PWM_Config(uint16_t CCR3_Val)  //in (/75us) //16bite
{

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
    
	uint16_t PrescalerValue = 0;
    
	TIM_DeInit(TIM3);
	 
	/* Compute the prescaler value */
	PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 60000000) - 1;
  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = (1000-1);
	TIM_TimeBaseStructure.TIM_Prescaler =PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel3 */

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    

	TIM_OC3Init(TIM3, &TIM_OCInitStructure);

	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);

	TIM_CtrlPWMOutputs(TIM3, ENABLE);	
   // printf("Timer_PWM_Config TIM_OCInitStructure.TIM_Pulse==%d\r\n",TIM_OCInitStructure.TIM_Pulse);
}

