
#include "pwm_output.h"
#include "stdint.h"
extern void Timer_PWM_Config(uint16_t CCR3_Val);  //in (/75us) //16bite
extern uint8 set_compv;

/*
 * ��������PWM_GPIO_Config
 * ����  ������TIM3�������PWMʱ�õ���I/O
          PB13_TIM1_PWM :�������ƱȽϵ�ѹ�����
					PB6_TIM4_PWM  :������4200���ʱ��
					PA6_TIM3_PWM  :�ղ����ʱ��
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
static void PWM_GPIO_Config(void) 
{
	#if 0
	GPIO_InitTypeDef GPIO_InitStructure;

	//����ͨ������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE); 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 

	//PA6_TIM3_PWM �ղ����ʱ�Ӹ����������
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//PB13_TIM1_PWMN1 :�������ƱȽϵ�ѹ�����
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//PB6_TIM4_PWM1  :������4200���ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	#endif
}


/*
 * ��������TIM3_Mode_Config
 * ����  ������TIM3�����PWM�źŵ�ģʽ�������ڡ����ԡ�ռ�ձ�
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
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
	/* PWM�źŵ�ƽ����ֵ */
  /* Time base configuration */		 
  TIM_TimeBaseStructure.TIM_Period = Fpclk/Feclk; //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Prescaler = 0;	    //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ36MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//����ʱ�ӷ�Ƶϵ��������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = TIM_TimeBaseStructure.TIM_Period/2;	   //��������ֵ�������������������ֵʱ����ƽ��������,�ղ�Э������50%
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ
	//ʹ�����
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);	 //ʹ��ͨ��1
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
  TIM_ARRPreloadConfig(TIM3, ENABLE);			 // ʹ��TIM3���ؼĴ���ARR
  TIM_Cmd(TIM3, ENABLE);                   //ʹ�ܶ�ʱ��3	
	#endif
}
void TIM3_Mode_DIS(void)
{
	#if 0
	TIM_Cmd(TIM3, DISABLE); 
	#endif
}
/*
 * ��������TIM3_Mode_Config
 * ����  ��TIM3 ���PWM�źų�ʼ����ֻҪ�����������
 *         TIM3���ĸ�ͨ���ͻ���PWM�ź����
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void TIM_PWM_Init(void)
{
		PWM_GPIO_Config();	
}

/*
 * ��������TIM3_Mode_Config
 * ����  ������TIM3�����PWM�źŵ�ģʽ�������ڡ����ԡ�ռ�ձ�
		PB13_TIM1_PWMN1
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
void TIM1_Mode_Config(uint16 CCR2_Val)
{
	#if 0
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM�źŵ�ƽ����ֵ */
	//u16 CCR2_Val = 375;
	//4200
  // Time base configuration 
  TIM_TimeBaseStructure.TIM_Period = 999;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Prescaler = 0;	    //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ36MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//����ʱ�ӷ�Ƶϵ��������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
  // PWM1 Mode configuration: Channel1 
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;	    //����ΪPWMģʽ1
	/*																				TIM_OCMode_Timing       ����Ƚ�ʱ��ģʽ    (������Ŷ�����Ч)
                                           TIM_OCMode_Active        ����Ƚ�����ģʽ    (ƥ��ʱ�����������Ϊ��Ч��ƽ��������ֵΪ�Ƚ�/����Ĵ���ֵ��ͬʱ��ǿ�����Ϊ�ߵ�ƽ)                                                
                                           TIM_OCMode_Inactive;     ����ȽϷ�����ģʽ (ƥ��ʱ�����������Ϊ��Ч��ƽ��������ֵΪ�Ƚ�/����Ĵ���ֵ��ͬʱ��ǿ�����Ϊ�͵�ƽ)      
                                           TIM_OCMode_Toggle        ����Ƚϴ���ģʽ    (��ת��������ֵ��Ƚ�/����Ĵ���ֵ��ͬʱ����ת������ŵĵ�ƽ)
                                           TIM_OCMode_PWM1          ���ϼ���ʱ����TIMx_CNT < TIMx_CCR*ʱ�������ƽ��Ч������Ϊ��Ч
                                                                    ���¼���ʱ����TIMx_CNT > TIMx_CCR*ʱ�������ƽ��Ч������Ϊ��Ч
                                           TIM_OCMode_PWM2     			��PWM1ģʽ�෴
	*/
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
																				/*
																				TIM_OutputState_Disable      ��ֹOC*���
                                            TIM_OutputState_Enable       ����OC*�������Ӧ����
																				*/
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	   //��������ֵ�������������������ֵʱ����ƽ��������
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;//
																						/*
																						TIM_OutputNState_Disable     �������ʹ�ܡ��ر�OC*N���
                                            TIM_OutputNState_Enable      �������ʹ�ܡ�����OC*N�������Ӧ������    
																						*/
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//�������Ϊ�ߵ�ƽ��Ч����ô����ʱ���Ƚ�ƥ��֮�����������ߵ�ƽ������ͷ�һ�¡�
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//TIM_OCNPolarity
																						/*
																						IM_OCNPolarity_High;        ����Ϊ��
                                            TIM_OCNPolarity_Low          ����Ϊ��
																						*/
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
																						/*
																						IM_OCIdleState_Set          ��MOE=0ʱ�����ʵ����OC*N����������OC*=1
                                            TIM_OCIdleState_Reset        ��MOE=0ʱ�����ʵ����OC*N����������OC*=0
																						*/
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
																					/*IM_OCIdleNState_Set         ��MOE=0ʱ��������OC*N=1
                                            TIM_OCIdleNState_Reset       ��MOE=0ʱ��������OC*N=0
																					*/

  TIM_OC1Init(TIM1, &TIM_OCInitStructure);	 //ʹ��ͨ��1
  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);//��ֹOC1��װ��,��TIM*_CCR*����һ��д��������Ч������Ҫ����һ�������¼�������ű�װ��Ĵ���
	TIM_ARRPreloadConfig(TIM1, ENABLE);			 // ʹ��TIM4���ؼĴ���AR
  TIM_Cmd(TIM1, ENABLE);   	
	TIM_CtrlPWMOutputs(TIM1, ENABLE);				// ���ʹ��PWMģʽ����˾�һ������ʡ
	#endif
}

void TIM1_Mode_DIS(void)
{
	//TIM_Cmd(TIM1, DISABLE); 
}
/*
 * ��������TIM4_Mode_Config
 * ����  ������TIM3�����PWM�źŵ�ģʽ�������ڡ����ԡ�ռ�ձ�
				PB6_TIM4_PWM  :������4200���ʱ��
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
void TIM4_Mode_Config(void)
{
	
		#if 0
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM�źŵ�ƽ����ֵ */
	u16 CCR3_Val = 10;
	//4200
  // Time base configuration 
  TIM_TimeBaseStructure.TIM_Period = 19;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Prescaler = 0;	    //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ36MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//����ʱ�ӷ�Ƶϵ��������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ

  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  // PWM1 Mode configuration: Channel1 
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;	    //����ΪPWMģʽ1
	/*																				TIM_OCMode_Timing       ����Ƚ�ʱ��ģʽ    (������Ŷ�����Ч)
                                           TIM_OCMode_Active        ����Ƚ�����ģʽ    (ƥ��ʱ�����������Ϊ��Ч��ƽ��������ֵΪ�Ƚ�/����Ĵ���ֵ��ͬʱ��ǿ�����Ϊ�ߵ�ƽ)                                                
                                           TIM_OCMode_Inactive;     ����ȽϷ�����ģʽ (ƥ��ʱ�����������Ϊ��Ч��ƽ��������ֵΪ�Ƚ�/����Ĵ���ֵ��ͬʱ��ǿ�����Ϊ�͵�ƽ)      
                                           TIM_OCMode_Toggle        ����Ƚϴ���ģʽ    (��ת��������ֵ��Ƚ�/����Ĵ���ֵ��ͬʱ����ת������ŵĵ�ƽ)
                                           TIM_OCMode_PWM1          ���ϼ���ʱ����TIMx_CNT < TIMx_CCR*ʱ�������ƽ��Ч������Ϊ��Ч
                                                                    ���¼���ʱ����TIMx_CNT > TIMx_CCR*ʱ�������ƽ��Ч������Ϊ��Ч
                                           TIM_OCMode_PWM2     			��PWM1ģʽ�෴
	*/
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
																				/*
																				TIM_OutputState_Disable      ��ֹOC*���
                                            TIM_OutputState_Enable       ����OC*�������Ӧ����
																				*/
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	   //��������ֵ�������������������ֵʱ����ƽ��������
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ
																						/*TIM_OCPolarity_High;         ����Ϊ��
                                            TIM_OCPolarity_Low           ����Ϊ��*/
  TIM_OC1Init(TIM4, &TIM_OCInitStructure);	 //ʹ��ͨ��1
  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM4, ENABLE);			 // ʹ��TIM4���ؼĴ���AR
  TIM_Cmd(TIM4, ENABLE);  
  #endif
}

void TIM4_Mode_DIS(void)
{
	#if 0
	TIM_Cmd(TIM4, DISABLE); 
	#endif
}
//���ñȽϵ�ѹ
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
/******************* (C) COPYRIGHT 2011 Ұ��Ƕ��ʽ���������� *****END OF FILE*****/
