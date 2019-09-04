
/*
 * USB_Queue.h
 *
 * Created: 2018/1/15 9:29:08
 *  Author: ppcc-02
 */ 
#ifndef __USER_FATFS_H__
#define __USER_FATFS_H__

FRESULT myf_getfree(char *drv, uint32_t *total, uint32_t *free);
FRESULT FileFormat(void);

void write_test();
FRESULT FileuUnMount(void);

FRESULT FileuMount(void);

void fatfstest(void);


#endif 
 