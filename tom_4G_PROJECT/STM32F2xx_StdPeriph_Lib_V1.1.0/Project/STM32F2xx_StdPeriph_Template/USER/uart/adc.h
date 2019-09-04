#ifndef __ADC_H
#define	__ADC_H
#include "config.h"
#include "user_config.h"

#define Channel_Num  7 //ＡＤＣ的通道数，本例使用11个通道
#define Sample_Num  10 //采样次数，本例使用平均滤波，采样10次取均值

extern void ADC1_Init(void);
extern uint32 Read_Dac(uint8 Vcc_Pass_Select);

#endif /* __ADC_H */

