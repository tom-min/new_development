#ifndef COMMON_H_
#define COMMON_H_
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
typedef struct
{
	uint8_t	 Name[16];		//设备名称
	uint8_t  HW_SN[4];		//硬件序列�?
	uint8_t  HW_Ver[4];		//硬件版本
	uint8_t  HW_Data[4];    //硬件日期
	uint8_t  FW_Ver[4];		//软件版本
	uint8_t  FW_Data[4];	//软件日期
	uint8_t  HW_Interface[4]; //上下位机协议版本
} PDU_VERSION_STRUCT;
typedef struct
{
	uint8_t	 channl;
	uint8_t  confirm;
	uint8_t  reserve2;
} PDU_COMMON_DATA_STRUCT;


typedef struct
{
	uint32_t identifier;
	uint8_t  format;
	uint8_t  dlc;
	uint8_t  data[64];
} PDU_CAN_RAW_DATA;



typedef struct
{
	uint8_t	 command;
	uint8_t	 type;
	uint8_t	 number;
	uint32_t MaskMessage;
	uint32_t PatternMessage;
} PDU_CAN_FILTER_STRUCT;

//hpl_can.h
#endif /* COMMON_H_ */


