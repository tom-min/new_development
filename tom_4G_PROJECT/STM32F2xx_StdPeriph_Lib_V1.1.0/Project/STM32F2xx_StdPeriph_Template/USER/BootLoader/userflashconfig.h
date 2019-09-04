/*
 * hal_spi.h
 *
 * Created: 2018/3/7 14:40:40
 *  Author: ppcc-02
 */ 


#ifndef USER_FLASH_CONFIG_H_
#define USER_FLASH_CONFIG_H_


#include "stdint.h"

#define FLASH_USER_BOOT_START_ADDR   ADDR_FLASH_SECTOR_0   /* Start @ of user Flash area */
#define FLASH_USER_BOOT_END_ADDR     ADDR_FLASH_SECTOR_6   /* End @ of user Flash area */

#define FLASH_USER_DATA_START_ADDR   ADDR_FLASH_SECTOR_3   /* Start @ of user Flash area *///0x0800C000
#define FLASH_USER_DATA_END_ADDR      ADDR_FLASH_SECTOR_4  /* End @ of user Flash area */

#define FLASH_USER_APP_START_ADDR   ADDR_FLASH_SECTOR_6   /* Start @ of user Flash area *///0x08040000
#define FLASH_USER_APP_END_ADDR     ADDR_FLASH_SECTOR_8   /* End @ of user Flash area */

/* Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */

#define M_user_app_verson_prefix_len 64 //STM32F2xx_FCAR_OBDII_APP_V
#define M_user_app_verson_len 64


#define M_user_boot_verson_prefix_len M_user_app_verson_prefix_len //STM32F2xx_FCAR_OBDII_APP_V
#define M_user_boot_verson_len M_user_app_verson_len


#define M_user_data_len 256

typedef struct
{  
	int vehicle_model;			//车型
	int child_vehicle_model;		//子车型
	int vehicle_version;			//版本号

	uint8_t user_app_verson[M_user_app_verson_len];
	

	uint8_t data[M_user_data_len-M_user_app_verson_len-sizeof(uint32_t)-sizeof(int)*3];	
} USER_APP_DATA;

typedef struct
{  
	uint8_t user_boot_verson[M_user_app_verson_len];
	uint8_t data[M_user_data_len-M_user_boot_verson_len-sizeof(uint32_t)];	
} USER_BOOT_DATA;

 
typedef struct
{
	uint32_t head;
	USER_BOOT_DATA bootdata;	
	USER_APP_DATA appdata;	
	uint32_t end;
} USER_DATA;

//联合体
typedef union
{
	USER_DATA user_data;
	uint8_t data[16*1024];	
}U_USER_DATA;


#endif /* USER_FLASH_CONFIG_H_ */