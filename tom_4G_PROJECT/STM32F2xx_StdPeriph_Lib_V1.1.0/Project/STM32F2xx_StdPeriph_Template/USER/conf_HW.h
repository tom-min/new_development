/*
 * conf_HW.h
 *
 * Created: 2018/5/18 15:21:45
 *  Author: ppcc-02
 */ 


#ifndef CONF_HW_H_
#define CONF_HW_H_

//PDUӲ���汾�� ��������ֻ�ܶ���һ��
//#define  PDU_HW_001		1    //��һ��PDUӲ��

#define  PDU_HW_002		1    //�ڶ���PDUӲ��


//E70 A��IC
//vTaskDelay(100); = 200ms
//vTaskDelay(1);   = 2ms
//E70 B��IC 
//vTaskDelay(100); = 80ms
//vTaskDelay(1);   = 0.8ms
#define		E70B_vTaskDelay_Modulus(x)		(uint32_t)(x*1.25)			



#endif /* CONF_HW_H_ */