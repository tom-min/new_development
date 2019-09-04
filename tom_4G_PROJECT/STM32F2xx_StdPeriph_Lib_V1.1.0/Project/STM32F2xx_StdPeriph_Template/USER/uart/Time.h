#ifndef TIME_TEST_H
#define TIME_TEST_H
#include "config.h"
#include "stdint.h"
extern void Timer_PWM_Config(uint16_t CCR3_Val);  //in (/75us) //16bite
extern void Timer_PWM_VPM_init(void);//in (/75us)
extern void T_delay_ms(uint16  n_ms);//1ms;
//extern void T_delay_us(uint16  n_ms);  //1us;

extern uint32_t T_delay_ms_get_status(void);
extern uint32_t T_delay_us_get_status(void);


extern uint32_t T_delay_ms_clear_count(void);
extern uint32_t T_delay_us_clear_count(void);

extern uint32_t T_delay_us_get_status(void);
extern uint32_t ms_clear_count(void);

#define hal_HW_Delay_ns_Init T_delay_us_init
#define hal_HW_Delay_ns T_delay_us

#endif	/* TIME_TEST_H */
