#ifndef __USER_CONFIG_H_
#define __USER_CONFIG_H_
#define APP_OFFSET (256*1024) //40000 0X8040000

#define VERSION_HEAD "STM32F2xx_FCAR_OBDII_APP_V"
#define BOOT_VERSION "STM32F2xx_FCAR_OBDII_4G_BOOT_V1_04_20190827.bin"

#define PRINTF_DEBUG1_UART UART4
//#define PRINTF_DEBUG2_UART USART2 //GSM

#define PRINTF_DEBUG2_UART UART4  //OBD


// ֻ��ͬʱ����һ��
//#define BOOT_APPLACATION  //BOOT  ����
#define APP_APPLACATION //APP ����


#if defined(BOOT_APPLACATION)&&defined(APP_APPLACATION)
	#error BOOT_APPLACATION APP_APPLACATION ֻ��ͬʱ����һ��
#endif 

#if defined(BOOT_APPLACATION)||defined(APP_APPLACATION)  
       
#else
	#error BOOT_APPLACATION APP_APPLACATION ��û�ж���һ��
#endif

#define M_NEWHARDWARE

//#define M_ECU_DEBUG 			// ��ECU ������Ϣ
//#define M_ODBII_DEBUG 	// ��ECU ������Ϣ
#define PRINTF_DEBUG 



/////////////////qu wenqi ����ͬʱֻ�ܶ���һ��/////////////////////////
//#define M_MODE_CAR_TOYOTA 
//#define M_MODE_CAR_NISSAN 
//#define M_MODE_CAR_HONDA
//#define M_MODE_CAR_FORD
//#define M_MODE_CAR_VW
#define M_MODE_CAR_HYUNDAI
//#define M_MODE_CAR_GMCN
#define M_MODE_CAR_GREATWALL

//#define		TOM_RDWR_DEBUG


#if defined(M_MODE_CAR_TOYOTA)
#define		VEHICLE_MODEL  "TOYOTA"
#define 	APP_VERSION   "STM32F2xx_FCAR_OBDII_4G_APP_V1_03_TOYOTA.bin"

#elif defined(M_MODE_CAR_NISSAN)
#define		VEHICLE_MODEL  "NISSAN"
#define 	APP_VERSION   "STM32F2xx_FCAR_OBDII_4G_APP_V1_04_NISSAN.bin"

#elif defined(M_MODE_CAR_HONDA)
#define		VEHICLE_MODEL  "HONDA"
#define 	APP_VERSION   "STM32F2xx_FCAR_OBDII_4G_APP_V1_04_HONDA.bin"

#elif defined(M_MODE_CAR_FORD)
#define		VEHICLE_MODEL  "FORD"
#define 	APP_VERSION   "STM32F2xx_FCAR_OBDII_4G_APP_V1_03_FORD.bin"

#elif defined(M_MODE_CAR_VW)
#define		VEHICLE_MODEL  "VW"
#define 	APP_VERSION   "STM32F2xx_FCAR_OBDII_4G_APP_V1_01_VW.bin"

#elif defined(M_MODE_CAR_HYUNDAI)
#define		VEHICLE_MODEL  "HYUNDAI"
#define 	APP_VERSION   "STM32F2xx_FCAR_OBDII_4G_APP_V1_01_HYUNDAI.bin"

#elif defined(M_MODE_CAR_GMCN)
#define		VEHICLE_MODEL  "GMCN"
#define 	APP_VERSION   "STM32F2xx_FCAR_OBDII_4G_APP_V1_01_GMCN.bin"

#elif defined(M_MODE_CAR_GREATWALL)
#define		VEHICLE_MODEL  "GREATWALL"
#define 	APP_VERSION   "STM32F2xx_FCAR_OBDII_4G_APP_V1_02_GREATWALL.bin"

#endif

//#define M_FIX_IMEI 
#define FIX_IMEI_CONST "865501045609956"


#endif

