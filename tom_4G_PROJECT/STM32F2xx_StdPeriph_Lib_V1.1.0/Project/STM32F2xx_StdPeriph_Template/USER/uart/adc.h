#ifndef __ADC_H
#define	__ADC_H
#include "config.h"
#include "user_config.h"

#define Channel_Num  7 //���ģõ�ͨ����������ʹ��11��ͨ��
#define Sample_Num  10 //��������������ʹ��ƽ���˲�������10��ȡ��ֵ

extern void ADC1_Init(void);
extern uint32 Read_Dac(uint8 Vcc_Pass_Select);

#endif /* __ADC_H */

