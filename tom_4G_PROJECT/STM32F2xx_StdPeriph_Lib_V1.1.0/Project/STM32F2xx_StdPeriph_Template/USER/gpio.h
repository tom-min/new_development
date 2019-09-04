#ifndef _GPIO_H_
#define _GPIO_H_
 
#include "user_stm32f2xx.h"

#define ioport_pin_t uint16_t

#define ioport_direction GPIOMode_TypeDef
#define ioport_mode_t GPIOPuPd_TypeDef

#define arch_ioport_set_pin_level  GPIO_WriteBit
#define arch_ioport_get_pin_level  GPIO_ReadInputDataBit

#define IOPORT_DIR_INPUT GPIO_Mode_IN
#define IOPORT_DIR_OUTPUT GPIO_Mode_OUT

#define IOPORT_MODE_PULLDOWN GPIO_PuPd_UP
#define IOPORT_MODE_PULLUP GPIO_PuPd_DOWN


#endif