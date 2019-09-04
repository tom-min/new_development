#ifndef __CODE_H
#define	__CODE_H

#include "usart1.h"

// #if defined (STM32F10X_MD) || defined (STM32F10X_MD_VL)
// #define PAGE_SIZE                         (0x400)    /* 1 Kbyte */
// #define FLASH_SIZE                        (0x20000)  /* 128 KBytes */
// #elif defined STM32F10X_CL
// #define PAGE_SIZE                         (0x800)    /* 2 Kbytes */
// #define FLASH_SIZE                        (0x40000)  /* 256 KBytes */
// #elif defined STM32F10X_HD
// #define PAGE_SIZE                         (0x800)    /* 2 Kbytes */
// #define FLASH_SIZE                        (0x80000)  /* 512 KBytes */
// #elif defined STM32F10X_XL
// #define PAGE_SIZE                         (0x800)    /* 2 Kbytes */
// #define FLASH_SIZE                        (0x100000) /* 1 MByte */
// #else
// #error "Please select first the STM32 device to be used (in stm32f10x.h)"
// #endif

#define PAGE_SIZE                         (0x400)    /* 1 Kbyte */

void GetVer(uint8 len);
void GetProtocolVer(uint8* pswitchprotocol);
void  Read_Ver(uint8 *InsBuf);
void  Write_Ver(uint8 *InsBuf);
void  Read_bl_addr(void);
void  Adjust_Loader(void);
void addr08_return (void);
#endif /* __CODE_H */

