
#include "pwm_output.h"
#include "stdint.h"
extern void Timer_PWM_Config(uint16_t CCR3_Val);  //in (/75us) //16bite
extern uint8 set_compv;

/*
 * 函数名：PWM_GPIO_Config
 * 描述  ：配置TIM3复用输出PWM时用到的I/O
          PB13_TIM1_PWM :用来控制比较电压的输出
					PB6_TIM4_PWM  :用来给4200输出时钟
					PA6_TIM3_PWM  :日产输出时钟
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
static void PWM_GPIO_Config(void) 
{
	#if 0
	GPIO_InitTypeDef GPIO_InitStructure;

	//引脚通用配置
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE); 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 

	//PA6_TIM3_PWM 日产输出时钟复用推挽输出
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//PB13_TIM1_PWMN1 :用来控制比较电压的输出
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//PB6_TIM4_PWM1  :用来给4200输出时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	#endif
}


/*
 * 函数名：TIM3_Mode_Config
 * 描述  ：配置TIM3输出的PWM信号的模式，如周期、极性、占空比
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
void SetEclk(uint32 Feclk )
{
	#if 0
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	if(Feclk==0)
	{
		clk_dis;
		TIM_Cmd(TIM3, DISABLE);
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);
		return;
	}
	clk_en;
	/* PWM信号电平跳变值 */
  /* Time base configuration */		 
  TIM_TimeBaseStructure.TIM_Period = Fpclk/Feclk; //当定时器从0计数到999，即为1000次，为一个定时周期
  TIM_TimeBaseStructure.TIM_Prescaler = 0;	    //设置预分频：不预分频，即为36MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//设置时钟分频系数：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = TIM_TimeBaseStructure.TIM_Period/2;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变,日产协议设置50%
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平
	//使用输出
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);	 //使能通道1
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
  TIM_ARRPreloadConfig(TIM3, ENABLE);			 // 使能TIM3重载寄存器ARR
  TIM_Cmd(TIM3, ENABLE);                   //使能定时器3	
	#endif
}
void TIM3_Mode_DIS(void)
{
	#if 0
	TIM_Cmd(TIM3, DISABLE); 
	#endif
}
/*
 * 函数名：TIM3_Mode_Config
 * 描述  ：TIM3 输出PWM信号初始化，只要调用这个函数
 *         TIM3的四个通道就会有PWM信号输出
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void TIM_PWM_Init(void)
{
		PWM_GPIO_Config();	
}

/*
 * 函数名：TIM3_Mode_Config
 * 描述  ：配置TIM3输出的PWM信号的模式，如周期、极性、占空比
		PB13_TIM1_PWMN1
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
void TIM1_Mode_Config(uint16 CCR2_Val)
{
	#if 0
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM信号电平跳变值 */
	//u16 CCR2_Val = 375;
	//4200
  // Time base configuration 
  TIM_TimeBaseStructure.TIM_Period = 999;       //当定时器从0计数到999，即为1000次，为一个定时周期
  TIM_TimeBaseStructure.TIM_Prescaler = 0;	    //设置预分频：不预分频，即为36MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//设置时钟分频系数：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
  // PWM1 Mode configuration: Channel1 
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;	    //配置为PWM模式1
	/*																				TIM_OCMode_Timing       输出比较时间模式    (输出引脚冻结无效)
                                           TIM_OCMode_Active        输出比较主动模式    (匹配时设置输出引脚为有效电平，当计数值为比较/捕获寄存器值相同时，强制输出为高电平)                                                
                                           TIM_OCMode_Inactive;     输出比较非主动模式 (匹配时设置输出引脚为无效电平，当计数值为比较/捕获寄存器值相同时，强制输出为低电平)      
                                           TIM_OCMode_Toggle        输出比较触发模式    (翻转。当计数值与比较/捕获寄存器值相同时，翻转输出引脚的电平)
                                           TIM_OCMode_PWM1          向上计数时，当TIMx_CNT < TIMx_CCR*时，输出电平有效，否则为无效
                                                                    向下计数时，当TIMx_CNT > TIMx_CCR*时，输出电平无效，否则为有效
                                           TIM_OCMode_PWM2     			与PWM1模式相反
	*/
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
																				/*
																				TIM_OutputState_Disable      禁止OC*输出
                                            TIM_OutputState_Enable       开启OC*输出到对应引脚
																				*/
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;//
																						/*
																						TIM_OutputNState_Disable     互补输出使能。关闭OC*N输出
                                            TIM_OutputNState_Enable      互补输出使能。开启OC*N输出到对应的引脚    
																						*/
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//如果设置为高电平有效，那么当定时器比较匹配之后，输出口输出高电平，否则就反一下。
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//TIM_OCNPolarity
																						/*
																						IM_OCNPolarity_High;        极性为正
                                            TIM_OCNPolarity_Low          极必为负
																						*/
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
																						/*
																						IM_OCIdleState_Set          当MOE=0时，如果实现了OC*N，则死区后OC*=1
                                            TIM_OCIdleState_Reset        当MOE=0时，如果实现了OC*N，则死区后OC*=0
																						*/
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
																					/*IM_OCIdleNState_Set         当MOE=0时，死区后OC*N=1
                                            TIM_OCIdleNState_Reset       当MOE=0时，死区后OC*N=0
																					*/

  TIM_OC1Init(TIM1, &TIM_OCInitStructure);	 //使能通道1
  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);//禁止OC1重装载,即TIM*_CCR*的数一经写入立即生效，否则要在下一个更新事件到来后才被装入寄存器
	TIM_ARRPreloadConfig(TIM1, ENABLE);			 // 使能TIM4重载寄存器AR
  TIM_Cmd(TIM1, ENABLE);   	
	TIM_CtrlPWMOutputs(TIM1, ENABLE);				// 如果使用PWM模式，则此句一定不能省
	#endif
}

void TIM1_Mode_DIS(void)
{
	//TIM_Cmd(TIM1, DISABLE); 
}
/*
 * 函数名：TIM4_Mode_Config
 * 描述  ：配置TIM3输出的PWM信号的模式，如周期、极性、占空比
				PB6_TIM4_PWM  :用来给4200输出时钟
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
void TIM4_Mode_Config(void)
{
	
		#if 0
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM信号电平跳变值 */
	u16 CCR3_Val = 10;
	//4200
  // Time base configuration 
  TIM_TimeBaseStructure.TIM_Period = 19;       //当定时器从0计数到999，即为1000次，为一个定时周期
  TIM_TimeBaseStructure.TIM_Prescaler = 0;	    //设置预分频：不预分频，即为36MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//设置时钟分频系数：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式

  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  // PWM1 Mode configuration: Channel1 
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;	    //配置为PWM模式1
	/*																				TIM_OCMode_Timing       输出比较时间模式    (输出引脚冻结无效)
                                           TIM_OCMode_Active        输出比较主动模式    (匹配时设置输出引脚为有效电平，当计数值为比较/捕获寄存器值相同时，强制输出为高电平)                                                
                                           TIM_OCMode_Inactive;     输出比较非主动模式 (匹配时设置输出引脚为无效电平，当计数值为比较/捕获寄存器值相同时，强制输出为低电平)      
                                           TIM_OCMode_Toggle        输出比较触发模式    (翻转。当计数值与比较/捕获寄存器值相同时，翻转输出引脚的电平)
                                           TIM_OCMode_PWM1          向上计数时，当TIMx_CNT < TIMx_CCR*时，输出电平有效，否则为无效
                                                                    向下计数时，当TIMx_CNT > TIMx_CCR*时，输出电平无效，否则为有效
                                           TIM_OCMode_PWM2     			与PWM1模式相反
	*/
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
																				/*
																				TIM_OutputState_Disable      禁止OC*输出
                                            TIM_OutputState_Enable       开启OC*输出到对应引脚
																				*/
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平
																						/*TIM_OCPolarity_High;         极性为正
                                            TIM_OCPolarity_Low           极必为负*/
  TIM_OC1Init(TIM4, &TIM_OCInitStructure);	 //使能通道1
  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM4, ENABLE);			 // 使能TIM4重载寄存器AR
  TIM_Cmd(TIM4, ENABLE);  
  #endif
}

void TIM4_Mode_DIS(void)
{
	#if 0
	TIM_Cmd(TIM4, DISABLE); 
	#endif
}
//设置比较电压
void SetCompV2(uint8 CompV)
{
	uint16 temp;
	uint16 DAV;
	
	if(set_compv!=0)
	{
		CompV=set_compv;
		//UARTAnswerErr(0xfc,CompV);
	}
	if(CompV>0xa0) CompV=0xA0;

	temp=((CompV&0xf0)>>4)*10+(CompV&0x0f);
	DAV =temp*100*4.7/14.7/3.3;
	Timer_PWM_Config(DAV);
}
/******************* (C) COPYRIGHT 2011 野火嵌入式开发工作室 *****END OF FILE*****/
