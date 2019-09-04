#ifndef __PWM_OUTPUT_H
#define	__PWM_OUTPUT_H
#include "config.h"

extern void TIM_PWM_Init(void);
extern void TIM1_Mode_Config(uint16 CCR2_Val);
extern void SetEclk(uint32 Feclk );
extern void TIM4_Mode_Config(void);
extern void TIM1_Mode_DIS(void);
extern void TIM3_Mode_DIS(void);
extern void TIM4_Mode_DIS(void);
extern void SetCompV(uint8 CompV);
#endif /* __PWM_OUTPUT_H */

