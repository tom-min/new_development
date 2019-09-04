#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "stm32f2xx_usart.h"
#include "main.h"
#include "4G_Common_Api.h"
#include "cJSON.h"
#include "Fcar_Api.h"
#include "md5.h" 
#include "ff.h"
#include "my_queue.h"
#include "FreeRTOS.h"
#include "task.h"
#include "udp_queue.h"
#include "user_config.h"
#include "App.h"


#define AT_CMD_PRI

#define DOWN_FILE_SIZE  512

#define GPS_CONVERT(X) ((int)(X/100) + (X/100.0 - (int)(X/100)) *100.0 / 60.0)


Tom_QueueData  UDP_cmd_queue;
char Module_dcoding[20] = {0};

extern void LTE_Module_Init(void);
extern void POWER_EN_Config(void);
char  * GetGNRMCInfo(char *src,char *src_start,char *sep_start,char *sep_end,char *dst);
void str_parse_json(char *buf,char *path);
void parseGpsBuffer(Vehicle_GPS	*data);
void printGpsBuffer(void);
int parse_udp_server_data(char *buf);
int download_Vehicle_file(char *file_path);
void get_Vehicle_file_size(int *file_size);
void get_file_size(int *file_size);
extern const TCHAR gBinPath[]; 
extern void UserBinFilemkdir(char *dir_name);
extern FRESULT UserBinFileSaveCheck(Vehicle_Model_Info *user_vehicle_model_info);
extern char* gefiletstrinfo(char *src,char  *src_start,char  *sep_start,char  *sep_end,char  *dst);
extern void RTC_TimeGetStr(char *time_buf);
extern void RTC_TimeRegulateSetviaEc20(t_style t);


extern uint8_t RxBuffer [RXBUFFERSIZE];
extern __IO uint16_t RxCounter;

extern uint8_t RxBuffer2 [RXBUFFERSIZE_UBOX];
extern __IO uint16_t RxCounter2;


extern unsigned char OBD_usr_name[20];
extern unsigned char OBD_password[10];
extern unsigned char OBD_TOKEN_STR[512];
extern unsigned char OBD_PROPERTY[10];

extern double	FENCE_radius; 
extern double	FENCE_lng;
extern double	FENCE_lat;

extern pudp_queue	UDP_data_queue;
extern TaskHandle_t  data_pack_task;
extern TaskHandle_t  data_transfer_task;
extern TaskHandle_t  Task_ObdiiCmdProcess_Proc_handle;
extern int appupdate;

TaskHandle_t  current_task;

_SaveData Save_Data;//其他项目的经纬度
Vehicle_Model_Info tom_vehicle_model_data;

int server_cmd;

//bin文件里每个字符的大小是0~255，细节细节
//获取token的返回值大于1024 bytes，大约1275bytes
//考虑下在打包过程中这个buf会不会被udp服务器发送的数据影响
unsigned char downfile_buffer[1536];

int  u2_printf(char *Command,int len)
{
		unsigned int i,timeout=0;

		for(i=0;i<len;i++)
		{
				USART_SendData(USART2,(unsigned char)Command[i]);        
				while(USART_GetFlagStatus(USART2,USART_FLAG_TC) != SET)
				{
						if(timeout > 500000)	//次数越大，传输大量数据不容易丢失出错
								return 1;
						else
							timeout++;
				}
		}
		
//		printf("tom send data success\r\n");
		return 0;
}
int  u2_printfu_blox_M(char *Command,int len)
{
		unsigned int i,timeout=0;

		for(i=0;i<len;i++)
		{
				USART_SendData(USART3,(unsigned char)Command[i]);        
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC) != SET)
				{
						if(timeout > 500000)	//次数越大，传输大量数据不容易丢失出错
								return 1;
						else
							timeout++;
				}
		}
		
//		printf("tom send data success\r\n");
		return 0;
}

unsigned int sendCommand(char *Command,char *Response, unsigned long Timeout, unsigned char Retry)
{
		unsigned char n,ret;
		int Time_Cont;
		static unsigned char flag;
		eTaskState tmp_task_status;
		char *task_name;
	
		if(flag)
		{
				current_task = xTaskGetCurrentTaskHandle();
				task_name = pcTaskGetTaskName(current_task);
				tmp_task_status = eTaskGetState(current_task);
				printf("current suspend task name: %s\r\n",task_name);
				printf("current_task status: %d\r\n",tmp_task_status);
				vTaskSuspend(current_task);
		}
	
		flag = 1;
		
#ifdef AT_CMD_PRI	
		printf("\r\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~start send cmd~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n");
#endif	
	
		memset(RxBuffer,0,RXBUFFERSIZE);
		RxCounter = 0;
		for (n = 0; n < Retry; n++)
		{
				taskENTER_CRITICAL();
				ret = u2_printf(Command,strlen(Command));
				if(ret > 0)
				{
						printf("command send failed\r\n");
						return 0;
				}
//				printf("tom send data success\r\n");
				taskEXIT_CRITICAL();
				
				Time_Cont = 0;
				while (Time_Cont < Timeout)
				{
//						vTaskDelay(500);
						vTaskDelay(100);
						Time_Cont += 100;
						
						if(strstr((char *)RxBuffer, Response) != NULL )
						{		
#ifdef AT_CMD_PRI								
								printf("\r\n***************receive****************\r\n");
								printf("receice trust buf: \r\n%s\r\nreceice buf length: %d\r\n",RxBuffer,RxCounter);
//							
								printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^ happy endding send cmd^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\r\n\r\n\r\n");
								
#endif								
								if(!strncmp(Command,"AT+QGPSGNMEA",12))			//获取GPS数据
								{
										Save_Data.isGetData = true;
										memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      
										memcpy(Save_Data.GPS_Buffer, RxBuffer, RxCounter);
								}
								if(!strncmp(Command,"AT+QHTTPREAD",12))	//解析服务器返回的数据
								{
										memset(downfile_buffer, 0, 1536);      
										memcpy(downfile_buffer, RxBuffer, RxCounter);
								}
								if(!strncmp(Command,"AT+CCLK?",8))			//获取UTC时间
								{
										memset(downfile_buffer, 0, 1536);      
										memcpy(downfile_buffer, RxBuffer, RxCounter);
								}	
								if(!strncmp(Command,"AT+GSN",6))				//获取IMEI
								{
										memset(downfile_buffer, 0, 1536);      
										memcpy(downfile_buffer, RxBuffer, RxCounter);
								}	
								if(!strncmp(Command,"AT+QFDWL=",9))				//获取从RAM下载的数据
								{
										memset(downfile_buffer, 0, 1536);      
										memcpy(downfile_buffer, RxBuffer, RxCounter);
								}	
								
								if(!strncmp(Command,"AT+QFOPEN",9))				//获取打开RAM文件的文件句柄
								{
										memset(downfile_buffer, 0, 1536);      
										memcpy(downfile_buffer, RxBuffer, RxCounter);
								}	
								if(!strncmp(Command,"AT+QFREAD",9))				//分批获取从RAM下载的数据
								{
										memset(downfile_buffer, 0, 1536);      
										memcpy(downfile_buffer, RxBuffer, RxCounter);
								}	
								//有问题,bin文件中有很多'\0',这会影响此处的strstr，因为这个字符串库函数遇到'\0'会结束
								//还有bin文件中可能出现CONNECT,OK\r\n等字符，所以只能指定接收长度去读bin文件从RAM
								if(!strncmp(Command,"AT+QHTTPDL",10))				//从http server下载数据到RAM
								{
										memset(downfile_buffer, 0, 1536);      
										memcpy(downfile_buffer, RxBuffer, RxCounter);
								}	
								if(!strncmp(Command,"AT+QIRD=",8))				//解析UDP服务器返回的命令
								{
										memset(downfile_buffer, 0, 1536);      
										memcpy(downfile_buffer, RxBuffer, RxCounter);
								}	
								if(!strncmp(Command,"AT+QFLST",8))				//获取车型文件的大小
								{
										memset(downfile_buffer, 0, 1536);      
										memcpy(downfile_buffer, RxBuffer, RxCounter);
								}
								if(!strncmp(Command,"AT+QHTTPGET",11))
								{
										memset(downfile_buffer, 0, 1536);      
										memcpy(downfile_buffer, RxBuffer, RxCounter);
								}
								
								memset(RxBuffer,0,RXBUFFERSIZE);
								RxCounter = 0;
								tmp_task_status = eTaskGetState(current_task);
								if(tmp_task_status == eSuspended)
										vTaskResume(current_task);
								flag = 0;
								return 1;
						}
				}
				
				Time_Cont = 0;
#ifdef AT_CMD_PRI					
				printf("temp receice buf: \r\n%s\r\n",RxBuffer);
#endif					
				memset(RxBuffer,0,RXBUFFERSIZE);
				RxCounter = 0;
		}
#ifdef AT_CMD_PRI			
		printf("\r\n***************receive failed****************\r\n");
#endif		
//		printf("receice  failed buf: \r\n%s\r\n",RxBuffer);
		memset(RxBuffer,0,RXBUFFERSIZE);
		RxCounter = 0;
#ifdef AT_CMD_PRI			
		printf("\r\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~unnormal end send cmd~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n\r\n");
#endif		
		
		tmp_task_status = eTaskGetState(current_task);
		if(tmp_task_status == eSuspended)
				vTaskResume(current_task);
		flag = 0;
		return 0;
}


void get_task_info(TaskHandle_t task)
{
		eTaskState task_status;
		char *task_name;
		UBaseType_t task_remain_stack_size;
		
		task_name = pcTaskGetTaskName(task);
		task_status = eTaskGetState(task);
		printf("current_task name: %s\r\n",task_name);
		if(task_status == 0)
		{
				printf("current_task status : %d, eRunning\r\n",task_status);
		}	
		else if(task_status == 1)
		{
				printf("current_task status : %d, eReady\r\n",task_status);
		}
		else if(task_status == 2)
		{
				printf("current_task status : %d, eBlocked\r\n",task_status);
		}
		else if(task_status == 3)
		{
				printf("current_task status : %d, eSuspended\r\n",task_status);
		}
		else if(task_status == 4)
		{	
				printf("current_task status : %d, eDeleted\r\n",task_status);
		}
		else if(task_status == 5)
		{
				printf("current_task status : %d, eInvalid\r\n",task_status);
		}
		
		task_remain_stack_size = uxTaskGetStackHighWaterMark(task);
		printf("current_task Remaining stack size: %d bytes\r\n",task_remain_stack_size*4);
}


//UDP client 初始化，申请socket
void UDP_Client_init(char *UDPServer,char *Port)
{
		char send_buf[128] = {0};
	
//		memset(send_buf, 0, 128);    //清空
		strcpy(send_buf, "AT+QIOPEN=1,0,\"UDP\",\"");//转义字符,\"是"
		strcat(send_buf, UDPServer);
		strcat(send_buf, "\",");
		strcat(send_buf, Port);
		strcat(send_buf, ",0,0\r\n");
		if (sendCommand(send_buf,"QIOPEN:", 2000, 10) == 1);
		else 
				printf("AT+QIOPEN= send failed\r\n");
		vTaskDelay(10);
		
		if (sendCommand("AT+QISTATE=1,0\r\n", "OK\r\n", 2000, 5) == 1);
		else 
				printf("AT+QISTATE send failed\r\n");
		vTaskDelay(10);
}


//UDP client通信完成
void tom_UDP_Client(cJSON *json_str)
{
		char send_buf[2] = {0};
		char *print_json;
		unsigned char ret;
		
//		print_json = cJSON_Print(json_str);
		print_json = cJSON_PrintUnformatted(json_str); //减小内存开销
		printf("[%s] send json string length: %d\r\n",__func__,strlen(print_json));
		//发送数据
		if (sendCommand("AT+QISEND=0\r\n", ">", 5000, 5) == 1);
		else 
				printf("AT+QISEND send failed\r\n");
		vTaskDelay(10);

		//受网络影响，延时可能要加长点
		if (sendCommand(print_json, print_json, 10000, 5) == 1);
		else 
		{
				printf("UDP client DATA send failed\r\n");
				return;
		}
		vTaskDelay(10);				
		
		send_buf[0] = 0x1a;	//ctrl+z 的ascii码值
		send_buf[1] = '\0';
		if (sendCommand(send_buf, "OK\r\n", 5000, 5) == 1);
		else 
		{
				printf("UDP client DATA end flag send failed\r\n");
				return;
		}
		
		if (sendCommand("AT+QIRD=0\r\n", "OK\r\n", 5000, 5) == 1);
		else 
		{
				printf("AT+QIRD send failed\r\n");
				return;
		}
		vTaskDelay(10);
		
		//这个命令解析在UDP数据发送线程调用，细节细节，难怪调度不到呢
		ret = parse_udp_server_data((char *)downfile_buffer);		
		if(ret == 1)
		{
//				printf("parse UDP server command successful\r\n");
		}
		else
		{
				printf("The server does not return a command.\r\n");
		}
		
		free(print_json);
		cJSON_Delete(json_str);		
}


//函数可以分成两部分，一部分做GPS初始化，另一部分专门做获取GPS数据的接口
void Get_GPS_Data(void)
{
		printf("---------^--start--^---------\r\n");
	
		if(sendCommand("AT\r\n","OK\r\n", 2000, 3) == 1);
		else
		{
				printf("MCU communicates with 2G_Moudle failed\r\n");
				return;
		}
		vTaskDelay(10);
		
		if (sendCommand("AT+CPIN?\r\n", "READY\r\n", 2000, 10) == 1);
		else
				printf("AT+CPIN? send failed,???SIM????GPS??\r\n");
		vTaskDelay(10);
		
		if (sendCommand("AT+QCFG=\"nwscanmode\",0\r\n", "OK\r\n", 2000, 5) == 1);
		else
				printf("AT+QCFG send failed\r\n");
		vTaskDelay(10);
	
		if (sendCommand("AT+CSQ\r\n", "OK\r\n", 2000, 5) == 1);
		else
				printf("AT+CSQ send failed\r\n");
		vTaskDelay(10);
		
		
		if (sendCommand("AT+CREG?\r\n", ",1\r\n", 2000, 15) == 1)				//??SIM?,??????s
		{
				printf("Register network successed\r\n");
		}
		else 	if (sendCommand("AT+CREG?\r\n", ",5\r\n", 2000, 10) == 1); //??SIM?
		else
		{		
				printf("Register network fail,customer needs do check\r\n");	
//				return;
		}
		vTaskDelay(10);
		
		if (sendCommand("AT+CGREG?\r\n", ",1\r\n", 2000, 15) == 1)//
		{
				printf("Register GPRS network successed\r\n");
		}
		else 
		{
				printf("Register GPRS network fail,customer needs do check\r\n");	
//				return;
		}
		vTaskDelay(10);						
	
		if (sendCommand("AT+QGPSCFG=\"nmeasrc\",1\r\n", "OK\r\n", 2000, 10) == 1);
		else
				printf("AT+QGPSCFG send failed\r\n");
		vTaskDelay(10);
		
		if (sendCommand("AT+QGPS=1\r\n", "OK\r\n", 2000, 10) == 1);
		else
				printf("AT+QGPS send failed\r\n");
		vTaskDelay(10);	
		

		if (sendCommand("AT+QNTP=1,\"ntp1.aliyun.com\",123\r\n", "OK\r\n", 2000, 10) == 1);
		else 
		{
				printf("setting time synchronization failed\r\n");
				return;
		}
		vTaskDelay(10);	
		
		if (sendCommand("AT+QNTP?\r\n", "OK\r\n", 2000, 5) == 1);
		else 
		{
				printf("tom_min time synchronization failed\r\n");
		}
		vTaskDelay(10);
		
		while(1)
		{
				Get_current_time(NULL);
			
				//GGA
//				if (sendCommand("AT+QGPSGNMEA=\"GGA\"\r\n", "QGPSGNMEA:", 2000, 5) == 1);
//				else
//						printf("AT+QGPSGNMEA send failed\r\n");
//				vTaskDelay(10);
				
				//RMC
				if (sendCommand("AT+QGPSGNMEA=\"RMC\"\r\n", "QGPSGNMEA:", 2000, 5) == 1);
				else
						printf("AT+QGPSGNMEA send failed\r\n");
				vTaskDelay(10);
				
				//??GPS??,GSV
//				if (sendCommand("AT+QGPSGNMEA=\"GSV\"\r\n", "QGPSGNMEA:", 2000, 5) == 1);
//				else
//						printf("AT+QGPSGNMEA send failed\r\n");
//				vTaskDelay(10);
				
				//GSA
//				if (sendCommand("AT+QGPSGNMEA=\"GSA\"\r\n", "QGPSGNMEA:", 2000, 5) == 1);
//				else
//						printf("AT+QGPSGNMEA send failed\r\n");
//				vTaskDelay(10);
				
				//VTG
//				if (sendCommand("AT+QGPSGNMEA=\"VTG\"\r\n", "QGPSGNMEA:", 2000, 5) == 1);
//				else
//						printf("AT+QGPSGNMEA send failed\r\n");
//				vTaskDelay(10);
				
				//??GPS??,GNS
//				if (sendCommand("AT+QGPSGNMEA=\"GNS\"\r\n", "QGPSGNMEA:", 2000, 5) == 1);
//				else
//						printf("AT+QGPSGNMEA send failed\r\n");
				
//				parseGpsBuffer();
//				printGpsBuffer();
				
				vTaskDelay(15000);
		}
		
		//这里编译器警告表示程序执行不到这里，因为上面是死循环
		if (sendCommand("AT+QGPSEND\r\n", "OK\r\n", 2000, 10) == 1);
		else
				printf("AT+QGPSEND send failed\r\n");
		
		printf("GPS Working over,hahahaha\r\n");
}


void get_final_gps_data(char *buf,double *gps_data)
{
		double gps_tmp;

		gps_tmp = atof(buf);
		*gps_data = GPS_CONVERT(gps_tmp);
}


void parse_GPGGA(Vehicle_GPS *data_gps)
{
		char *subString;
		char *subStringNext;
		char i = 0;
		char tmp_buf[20] = {0}; 

		//GGA
		if (sendCommand("AT+QGPSGNMEA=\"GGA\"\r\n", "QGPSGNMEA:", 2000, 2) == 1);
		else
				printf("AT+QGPSGNMEA send failed\r\n");
		vTaskDelay(10);

		if (Save_Data.isGetData)
		{
				Save_Data.isGetData = false;

				for (i = 0 ; i <= 8 ; i++)
				{
						if (i == 0)
						{
								if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
										printf("parseGpsBuffer failed1\r\n");	//解析错误
						}
						else
						{
								subString++;
								if ((subStringNext = strstr(subString, ",")) != NULL)
								{
										switch(i)
										{
												case 6:
													
															memset(tmp_buf,0,20);
															memcpy(tmp_buf, subString, subStringNext - subString);
															data_gps->pos_type = atoi(tmp_buf);
															printf("[%s] pos_type : %s\r\n",__func__,tmp_buf);
															break;	//获取定位质量指标
												case 8:
															memset(tmp_buf,0,20);
															memset(data_gps->GPS_Precision,0,10);
															memcpy(tmp_buf, subString, subStringNext - subString);
															memcpy(data_gps->GPS_Precision, subString, subStringNext - subString);
															printf("[%s] GPS_Precision : %s\r\n",__func__,tmp_buf);
															break;	//获取水平精度

												default:
															break;
										}

										subString = subStringNext;
										Save_Data.isParseData = true;
								}
								else
								{
										printf("parseGpsBuffer failed2\r\n");	//解析错误
								}
								
						}

				}
		}
}


#if 0
//建议将经纬度改为double类型，便于计算围栏报警
void parseGpsBuffer(Vehicle_GPS	*data)
{
		char *subString;
		char *subStringNext;
		char i = 0;
		char usefullBuffer[2] = {0}; 
		char tmp_buf[20] = {0}; 

		//RMC
		if (sendCommand("AT+QGPSGNMEA=\"RMC\"\r\n", "QGPSGNMEA:", 3000, 5) == 1);
		else
				printf("AT+QGPSGNMEA send failed\r\n");
		vTaskDelay(10);

		if (Save_Data.isGetData)
		{
				Save_Data.isGetData = false;

				for (i = 0 ; i <= 8 ; i++)
				{
						if (i == 0)
						{
								if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
										printf("parseGpsBuffer failed1\r\n");	//解析错误
						}
						else
						{
								subString++;
								if ((subStringNext = strstr(subString, ",")) != NULL)
								{
										switch(i)
										{
												case 1:
															memcpy(Save_Data.UTCTime, subString, subStringNext - subString);
//															printf("UTCTime : %s\r\n",Save_Data.UTCTime);
															break;	//获取UTC时间
												case 2:
															memcpy(usefullBuffer, subString, subStringNext - subString);
//															printf("usefullBuffer : %s\r\n",usefullBuffer);
															break;	//获取GPS是否有效的状态
												case 3:
//															memcpy(Save_Data.latitude, subString, subStringNext - subString-1);
															memcpy(Save_Data.latitude, subString, subStringNext - subString);
//															printf("latitude : %s\r\n",Save_Data.latitude);
															break;	//获取纬度信息
												case 4:
															memcpy(Save_Data.N_S, subString, subStringNext - subString);
//															printf("N_S : %s\r\n",Save_Data.N_S);
															break;	//获取N/S
												case 5:
//															memcpy(Save_Data.longitude, subString, subStringNext - subString-1);
															memcpy(Save_Data.longitude, subString, subStringNext - subString);
//															printf("longitude : %s\r\n",Save_Data.longitude);
															break;	//获取经度信息
												case 6:
															memcpy(Save_Data.E_W, subString, subStringNext - subString);
//															printf("E_W : %s\r\n",Save_Data.E_W);
															break;	//获取E/W
												case 7:
															memset(tmp_buf,0,20);
															memcpy(tmp_buf, subString, subStringNext - subString);
															Save_Data.speed = atof(tmp_buf);
//															printf("E_W : %s\r\n",Save_Data.E_W);
															break;	//获取速度
												case 8:
															memset(tmp_buf,0,20);
															memcpy(tmp_buf, subString, subStringNext - subString);
															Save_Data.angle = atof(tmp_buf);
//															printf("E_W : %s\r\n",Save_Data.E_W);
															break;	//获取方位角
												default:
															break;
										}

										subString = subStringNext;
										Save_Data.isParseData = true;
								}
								else
								{
										printf("parseGpsBuffer failed2\r\n");	//解析错误
								}
						}
				}
				
				//用全局给其他项目引用
				if(usefullBuffer[0] == 'A')
				{
						Save_Data.isUsefull = true;		
						data->pos_status = 0;
						memset(tmp_buf,0,20);
						memcpy(tmp_buf, Save_Data.latitude, strlen(Save_Data.latitude)-1);
						get_final_gps_data(tmp_buf,&(data->lat));
					
						memset(tmp_buf,0,20);
						memcpy(tmp_buf, Save_Data.longitude, strlen(Save_Data.longitude)-1);
						get_final_gps_data(tmp_buf,&(data->lng));
						data->speed = Save_Data.speed;
						data->angle = Save_Data.angle;
					
						parse_GPGGA(data);
						printf("VEHICLE_GPS position info：latitude: %f\t, longitude : %f\r\n",data->lat,data->lng);
				}
				else if(usefullBuffer[0] == 'V')	//数据无效不上报,待会做
				{
						Save_Data.isUsefull = false;
						data->pos_status = 1;
						printf("VEHICLE_GPS data invalid\r\n");											
				}
		}
}

#else

//建议将经纬度改为double类型，便于计算围栏报警
void parseGpsBuffer(Vehicle_GPS	*data)
{
		char *subString;
		char *subStringNext;
		char i = 0;
		char usefullBuffer[2] = {0}; 
		char tmp_buf[20] = {0}; 

		//RMC
		if (sendCommand("AT+QGPSGNMEA=\"RMC\"\r\n", "QGPSGNMEA:", 2000, 2) == 1);
		else
				printf("AT+QGPSGNMEA send failed\r\n");
		vTaskDelay(50);

		if (Save_Data.isGetData)
		{
				Save_Data.isGetData = false;

				for (i = 0 ; i <= 8 ; i++)
				{
						if (i == 0)
						{
								if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
										printf("parseGpsBuffer failed1\r\n");	//解析错误
						}
						else
						{
								subString++;
								if ((subStringNext = strstr(subString, ",")) != NULL)
								{
										switch(i)
										{
												case 2:
															memcpy(usefullBuffer, subString, subStringNext - subString);
//															printf("usefullBuffer : %s\r\n",usefullBuffer);
															break;	//获取GPS是否有效的状态
												case 3:
															memset(tmp_buf,0,20);
															memcpy(tmp_buf, subString, subStringNext - subString);
															get_final_gps_data(tmp_buf,&(Save_Data.latitude));
//															printf("latitude : %f\r\n",Save_Data.latitude);
															break;	//获取纬度信息
												case 4:
															memcpy(Save_Data.N_S, subString, subStringNext - subString);
//															printf("N_S : %s\r\n",Save_Data.N_S);
															break;	//获取N/S
												case 5:
															memset(tmp_buf,0,20);
															memcpy(tmp_buf, subString, subStringNext - subString);
															get_final_gps_data(tmp_buf,&(Save_Data.longitude));
//															printf("longitude : %f\r\n",Save_Data.longitude);
															break;	//获取经度信息
												case 6:
															memcpy(Save_Data.E_W, subString, subStringNext - subString);
//															printf("E_W : %s\r\n",Save_Data.E_W);
															break;	//获取E/W
												case 7:
															memset(tmp_buf,0,20);
															memcpy(tmp_buf, subString, subStringNext - subString);
															Save_Data.speed = atof(tmp_buf);
//															printf("E_W : %s\r\n",Save_Data.E_W);
															break;	//获取速度
												case 8:
															memset(tmp_buf,0,20);
															memcpy(tmp_buf, subString, subStringNext - subString);
															Save_Data.angle = atof(tmp_buf);
//															printf("E_W : %s\r\n",Save_Data.E_W);
															break;	//获取方位角
												default:
															break;
										}
										
										subString = subStringNext;
										Save_Data.isParseData = true;
								}
								else
								{
										printf("parseGpsBuffer failed2\r\n");	//解析错误
								}
						}
				}
				
				if(data)
				{
						//用全局给其他项目引用
						if(usefullBuffer[0] == 'A')
						{
								Save_Data.isUsefull = true;		
								data->pos_status = 0;
								data->lng = Save_Data.longitude;
								data->lat = Save_Data.latitude;
								data->speed = Save_Data.speed;
								data->angle = Save_Data.angle;
								
								parse_GPGGA(data);
								printf("VEHICLE_GPS position info：latitude: %f\t, longitude : %f\r\n",data->lat,data->lng);
						}
						else if(usefullBuffer[0] == 'V')	//数据无效不上报,待会做
						{
								Save_Data.isUsefull = false;
								data->pos_status = 1;
								printf("VEHICLE_GPS data invalid\r\n");											
						}
				}
		}
}

#endif


void printGpsBuffer(void)
{
		if (Save_Data.isParseData)
		{
				Save_Data.isParseData = false;

				if(Save_Data.isUsefull)
				{
						Save_Data.isUsefull = false;
						printf("Save_Data.latitude = ");
						printf("%s",Save_Data.latitude);
						printf("\r\n");

						printf("Save_Data.N_S = ");
						printf("%s",Save_Data.N_S);
						printf("\r\n");

						printf("Save_Data.longitude = ");
						printf("%s",Save_Data.longitude);
						printf("\r\n");

						printf("Save_Data.E_W = ");
						printf("%s",Save_Data.E_W);
						printf("\r\n");
				}
				else
				{
						printf("GPS DATA is not useful!\r\n");
				}
		}
}


int GetJsonValueStr(char *buf,const char *key,const char *childkey, char *valuestring)
{
		char *p1,*p2;
		//这个json字符串长度有点大，buf长度一定要谨慎，不然会卡死
		char tmp_buf[512] = {0};
		cJSON *tom_json,*json_val,*json_path;
		int ret = 0x01;

		if( (p1 = strstr(buf,"{")) != NULL )
		{
				if( (p2 = strstr(p1,"\r\n")) != NULL)
				{
						memset(tmp_buf,0,512);
						memcpy(tmp_buf,p1,p2-p1);
						printf("GetJsonValueStr json: %s, %d\r\n",tmp_buf,strlen(tmp_buf));  
						tom_json = cJSON_Parse(tmp_buf);
						if(tom_json)
						{							
								json_val = cJSON_GetObjectItem(tom_json,key);
								if(json_val)
								{
										json_path = cJSON_GetObjectItem(json_val,childkey);
										if(json_path)
										{
												memcpy(valuestring,json_path->valuestring,strlen(json_path->valuestring));
												ret = 0x00;
										}
								}
								
								cJSON_Delete(tom_json);
						}
				}
		}
		else
		{
				ret = 0x01;
		}
		
		return ret ;
}


//获取下载文件信息
int GetDownloadFileInfo(char *download_path,char *downloadfilename)
{
//		char URL_download[] = "http://down.szcitycar.com/file/downfile/get?fileapp=2";
		char URL_download[] = "http://fcar-oss-public.oss-cn-hangzhou.aliyuncs.com/public/4g-carnet/carkind/STM32F2xx_FCAR_OBDII_4G_APP_V1_00_20190729.bin";
 		char send_buf[128] = {0};
		  
#if 1		
		memset(send_buf,0,128);
		//sprintf最大的问题就是不安全，怕数组越界
		sprintf(send_buf,"AT+QHTTPURL=%d,%d\r\n",strlen(URL_download),30);//URL
			
		if (sendCommand(send_buf, "CONNECT\r\n", 2000, 5) == 1);//
		else
		{
				printf("AT+QHTTPURL= failed\r\n");
				return 1;
		}
		vTaskDelay(10);
		
		if (sendCommand(URL_download, "OK\r\n", 3000, 10) == 1);
		else 
		{
				printf("SET URL failed\r\n");
				return 1;
		}
		
		if (sendCommand("AT+QHTTPGET=60\r\n", "OK\r\n", 2000, 20) == 1);//
		else
		{ 
				printf("AT+QHTTPGET= failed\r\n");	
				return 1;
		}
		vTaskDelay(10);
		
		if (sendCommand("AT+QHTTPREAD=30\r\n", "OK\r\n", 5000, 5) == 1);
		else 
		{
				printf("AT+QHTTPREAD= failed\r\n");	
				return 1;
		}
		vTaskDelay(10);

#endif		
		
		//有BUG，待修改，整体的下载已经完成
		memset(download_path,0,sizeof(download_path));
		//这个函数为什么会卡，明天着重研究下
		//str_parse_json((char *)downfile_buffer,download_path); //get download url
        		
#if 1
		//
		memset(downloadfilename,0,sizeof(downloadfilename));
		if(GetJsonValueStr((char *)downfile_buffer,"data","ossPath",download_path) != 0) //get download url
		{
		    printf("GetJsonValueStr ossPath error  \r\n" );
		    return 1;
		}
		if(GetJsonValueStr((char *)downfile_buffer,"data","fileName",downloadfilename) != 0)// //get downloadfilename
		{
				printf("GetJsonValueStr fileName error  \r\n" );
				return 1;
		}
          
		printf("ossPath: %s\r\n",download_path);
		printf("fileName: %s\r\n",downloadfilename);
#endif
		
		if (sendCommand("AT+QIDEACT\r\n", "OK\r\n", 5000, 5) == 1);
		else
		{
				printf("AT+QIDEACT send failed\r\n");
				return 1;
		} 
    
		return 0;
}

int tom_downfile(char * binname,int remotefilelen)
{
		char data_buf[DOWN_FILE_SIZE+1] = {0};
		uint32_t filehandle,file_size;
		uint32_t read_file_size;
		uint32_t readed_file_size;	
		FILINFO *filinfo;
		FRESULT rts_1;
		uint32_t SizeWritten = 0;
		int i,len;


		printf("welcome to [%s]\r\n",__func__);


//		if (sendCommand("AT+QFLST=\"UFS:*\"\r\n", "OK\r\n", 2000, 5) == 1);
//		else 
//		{
//				printf("AT+QFLST failed\r\n");	
//				return 1;
//		}
//		vTaskDelay(10);
	
		
#ifdef M_NEWHARDWARE
	if (sendCommand("AT+QHTTPREADFILE=\"UFS:1.bin\",60\r\n", "QHTTPREADFILE:", 5000, 20) == 1);
#else
	if (sendCommand("AT+QHTTPREADFILE=\"RAM:1.bin\",60\r\n", "QHTTPREADFILE:", 5000, 20) == 1);
#endif	   
		else 
		{
				printf("AT+QHTTPREADFILE= failed\r\n");	
				return 1;
		}
		vTaskDelay(10);
         

	
//		if (sendCommand("AT+QFLST=\"UFS:*\"\r\n", "OK\r\n", 2000, 5) == 1);
//		if (sendCommand("AT+QFLST=\"UFS:1.bin\"\r\n", "OK\r\n", 2000, 5) == 1);
//		else 
//		{
//				printf("AT+QFLST failed\r\n");	
//				return 1;
//		}
//		vTaskDelay(10);
		
		get_Vehicle_file_size(&file_size);

		printf("[%s] get Vehicle file size : %d\r\n",__func__,file_size);
		if(remotefilelen != file_size)
		{ 
			printf(" remotefilelen=%d   file_size=%d \r\n",remotefilelen,file_size);
			printf(" bin file download error \r\n" );
			return   1;
		}
		
#ifdef M_NEWHARDWARE		
		if (sendCommand("AT+QFLDS=\"UFS\"\r\n", "OK\r\n", 2000, 5) == 1);
#else
		if (sendCommand("AT+QFLDS=\"RAM\"\r\n", "OK\r\n", 2000, 5) == 1);

#endif
		else 
		{
				printf("AT+QFLDS failed\r\n");	
				return 1;
		}
		
	  
		vTaskDelay(10);	
#ifdef M_NEWHARDWARE	
		//文件打开成功才能继续读写
		if(EcFileOpen("UFS:1.bin",2,&filehandle) != 0)
#else
		if(EcFileOpen("RAM:1.bin",2,&filehandle) != 0)
#endif
		{
				printf("EcFileOpen fail \r\n" );
				return 1;
		}
		else
		{
				printf("filehandle :%d\r\n",filehandle);
		}
		 printf("3333333\r\n"); 
		//读写数据的次数
		if(file_size%DOWN_FILE_SIZE)
				len = (file_size/DOWN_FILE_SIZE)+1;
		else
				len = file_size/DOWN_FILE_SIZE;

		printf("len value: %d\r\n",len);
		

		
		for(i=0;i<len;i++)
		{
				printf("\r\n--------------------------this is  %d times write to SPI_FLASH------------------------------------\r\n\r\n",i+1);
                           STM_EVAL_LED_FIFO_2();  
				if(EcFilePostion(filehandle) != 0)
				{
						printf("AT+QFPOSITION failed\r\n");	
						UserBinFileDelete(binname);
						return 1;
				}
				vTaskDelay(1);
				
				//从RAM读数据之前先设置偏移地址
				if(EcFileSeek(filehandle,DOWN_FILE_SIZE*i,0)!=0)
				{
						printf("AT+EcFileSeek failed\r\n");	
						UserBinFileDelete(binname);
						return 1;
				}

				vTaskDelay(1);
								 	  
				if( (i == len-1) && (file_size % DOWN_FILE_SIZE))
				{
						read_file_size = file_size%DOWN_FILE_SIZE;
				}
				else
				{
						read_file_size = DOWN_FILE_SIZE;
				}
                             
				if(EcFileRead2(filehandle,read_file_size,&readed_file_size) == 0)
				{
						printf("EcFileRead2 success  \r\n");
						memset(data_buf,0,DOWN_FILE_SIZE);
						memcpy(data_buf,downfile_buffer,read_file_size);
								
						rts_1 = UserBinFileSave(binname,data_buf, read_file_size,&SizeWritten);
						if(rts_1 != 0)
						{
								printf("UserBinFileSave fail \r\n");
								UserBinFileDelete(binname);
								break;
						}
						vTaskDelay(1);					
				}
				else
				{
						printf("EcFileRead2 fail  \r\n");
						UserBinFileDelete(binname);
						break;
				}
				
		}

		
		if(EcFileClose(filehandle) != 0)
		{
				printf("EcFileClose failed\r\n");	
		}
#ifdef M_NEWHARDWARE		
		if(EcFileDelete("UFS:1.bin") != 0)
#else
		if(EcFileDelete("RAM:1.bin") != 0)

#endif
		{
				printf("EcFileDelete failed\r\n");	
			 
		}
		vTaskDelay(10);
		
    return 0;	
}
//下载文件demo
int Download_File(void)
{
		int filesize;
		printf("welcome to [%s]\r\n",__func__);
 
		if(UserBinFileSaveCheck(&tom_vehicle_model_data) != 0)
		{
				printf("UserBinFileSaveCheck fail \r\n");
				return 1;
		}	
		if(Get_Request(tom_vehicle_model_data.vehicle_file_url,0)!=0)
		{
			printf("Download_File Get_Request fail \r\n");
			return 1;
		}
	        get_file_size(&filesize);
		 
		if(tom_downfile(tom_vehicle_model_data.vehicle_file_name,filesize) != 0)
		{
				return 1;
		}
		
		return 0;
}


//返回的json字符串内容不同，需要匹配相应的解析函数
//解析post之后的返回值，解析json字符串
void str_parse_json(char *buf,char *path)
{
		char *p1,*p2;
		//这个json字符串长度有点大，buf长度一定要谨慎，不然会卡死
		char tmp_buf[512] = {0};
		cJSON *tom_json,*json_val,*json_path;
		
		if( (p1 = strstr(buf,"{")) != NULL )
		{
				if( (p2 = strstr(p1,"\r\n")) != NULL)
				{
						memset(tmp_buf,0,350);
						memcpy(tmp_buf,p1,p2-p1);
						printf("tom first json string,hahaha: %s, %d\r\n",tmp_buf,strlen(tmp_buf));
						tom_json = cJSON_Parse(tmp_buf);
						if(tom_json)
						{							
								json_val = cJSON_GetObjectItem(tom_json,"data");
								if(json_val)
								{
										json_path = cJSON_GetObjectItem(json_val,"ossPath");
										if(json_path)
										{
												memcpy(path,json_path->valuestring,strlen(json_path->valuestring));
												//不占内存
												printf("download file path: %s\r\n",json_path->valuestring);
										}
								}
								else
										printf("data is empty, hahaha!\r\n");
								
								cJSON_Delete(tom_json);
						}
				}
		}
}


void get_server_fence_para(char *buf)
{
		double lat,lng;
		
		sscanf(buf,"%lf,%lf",&lat,&lng);
		FENCE_lat = lat;
		FENCE_lng = lng;
		printf("[%s]: lat: %lf, lng: %lf\r\n",__func__,lat,lng);
}




int parse_COMMON_data(cJSON *object)
{
	int ret=0 ;
	cJSON *json_val;
	json_val = cJSON_GetObjectItem(object,"code");
	if(json_val)
	{
		ret = json_val->valueint;
		printf("JSON_COMMON code val: %d\r\n",json_val->valueint);
	}
	return ret;
}
int parse_ADDCARIO_data(cJSON *object)
{
	int ret=0 ;
	cJSON *json_val;
	json_val = cJSON_GetObjectItem(object,"code");
	if(json_val)
	{
			ret = json_val->valueint;
			printf("JSON_ADDCARIO code val: %d\r\n",json_val->valueint);
	}
	
	json_val = cJSON_GetObjectItem(object,"data");
	if(json_val)
	{
			printf("server password: %s\r\n",json_val->valuestring);
			memset(OBD_password,0,10);
			memset(OBD_PROPERTY,0,10);
			memcpy(OBD_password,json_val->valuestring,strlen(json_val->valuestring));
			// get_OBD_Prop("123456",(char *)OBD_PROPERTY);
			get_OBD_Prop((char *)OBD_password,(char *)OBD_PROPERTY);
	}
	return ret;
}


int parse_TOKEN_data(cJSON *object)
{
	int ret=0 ;
	cJSON *json_val;
	unsigned char error[64];
	json_val = cJSON_GetObjectItem(object,"access_token");
	if(json_val)
	{
		memset(OBD_TOKEN_STR,0,512);
		memcpy(OBD_TOKEN_STR,json_val->valuestring,strlen(json_val->valuestring));
		printf("get server Token: %s\r\n",json_val->valuestring);
		printf("Token string : %s, len: %d\r\n",OBD_TOKEN_STR,strlen((char *)OBD_TOKEN_STR));
		ret = 1;		
	}
	else
	{
		json_val = cJSON_GetObjectItem(object,"error");
		if(json_val)
		{
			memset(error,0,sizeof(error));
			memcpy(error,json_val->valuestring,strlen(json_val->valuestring));
			printf("get server Token error : %s\r\n",json_val->valuestring);
			printf("Token string : %s, len: %d\r\n",error,strlen(error));
			 
			ret = 2;
		}
	}
	return ret;
}
int parse_TIME_data(cJSON *object)
{

}

int parse_FENCE_data(cJSON *object)
{
	int ret=0 ;
	cJSON *tom_json;
	cJSON *json_val;
	cJSON *json_t1;
	cJSON *json_t2;
	cJSON *json_t3;
	cJSON *json_t4;
	cJSON *json_t5;

	json_val = cJSON_GetObjectItem(object,"code");
	if(json_val)
	{
		ret = json_val->valueint;
		printf("JSON_FENCE code val: %d\r\n",json_val->valueint);
	}

	json_val = cJSON_GetObjectItem(object,"data");
	
	if(json_val)
	{
			json_t1 = cJSON_GetObjectItem(json_val,"carkind");
			if(json_t1)
			{
					json_t2 = cJSON_GetObjectItem(json_t1,"ver");
					if(json_t2)
					{
							tom_vehicle_model_data.vehicle_version = json_t2->valueint;
							printf("JSON_FENCE version : %d\r\n",json_t2->valueint);
					}
					json_t2 = cJSON_GetObjectItem(json_t1,"filename");
					if(json_t2)
					{
							memset(tom_vehicle_model_data.vehicle_file_name,0,sizeof(tom_vehicle_model_data.vehicle_file_name));
							memcpy(tom_vehicle_model_data.vehicle_file_name,json_t2->valuestring,strlen(json_t2->valuestring));
							printf("JSON_FENCE Vehicle bin file name : %s\r\n",json_t2->valuestring);
					}
					json_t2 = cJSON_GetObjectItem(json_t1,"carid");
					if(json_t2)
					{
							tom_vehicle_model_data.vehicle_model = json_t2->valueint;
							printf("JSON_FENCE Vehicle model : %d\r\n",json_t2->valueint);
					}
					json_t2 = cJSON_GetObjectItem(json_t1,"ossPath");
					if(json_t2)
					{
							memset(tom_vehicle_model_data.vehicle_file_url,0,sizeof(tom_vehicle_model_data.vehicle_file_url));
							memcpy(tom_vehicle_model_data.vehicle_file_url,json_t2->valuestring,strlen(json_t2->valuestring));
							printf("JSON_FENCE Vehicle ossPath : %s,length: %d\r\n",json_t2->valuestring,strlen(json_t2->valuestring));
					}
					json_t2 = cJSON_GetObjectItem(json_t1,"carkindid");
					if(json_t2)
					{
							tom_vehicle_model_data.child_vehicle_model = json_t2->valueint;
							printf("JSON_FENCE child_vehicle_model : %d\r\n",json_t2->valueint);
					}
			}
		
			json_t1 = cJSON_GetObjectItem(json_val,"fence");
			if(json_t1)
			{
					//	json_t2 = cJSON_GetObjectItem(json_t1,"devicecoding");
					//	if(json_t2)
					//	{
					//		printf("JSON_FENCE devicecoding : %s\r\n",json_t2->valuestring);
					//	}
					json_t2 = cJSON_GetObjectItem(json_t1,"coords");
					if(json_t2)
					{
							//这个对象是json字符串，所以先要转化成json对象
							json_t3 = cJSON_Parse(json_t2->valuestring);
							//获取栅栏半径
							json_t4 = cJSON_GetObjectItem(json_t3,"radius");
							if(json_t4)
							{
									FENCE_radius = json_t4->valueint;
									printf("JSON_FENCE radius : %d\r\n",json_t4->valueint);
							}
							//如果这个json数组有多个元素，这里得加循环去获取
							json_t4 = cJSON_GetObjectItem(json_t3,"coords");
							if(json_t4)
							{
									json_t5 = cJSON_GetArrayItem(json_t4,0);
									if(json_t5)
											printf("JSON_FENCE lat_lng : %s\r\n",json_t5->valuestring);
									get_server_fence_para(json_t5->valuestring);
							}
					}
			}
	}
	return ret;
}


//返回的json字符串内容不同，需要匹配相应的解析函数
//解析post请求之后的返回值，解析json字符串
int parse_http_server_data(char *buf,JSON_PARSE_TYPE parse_type)
{
		char *p1,*p2;
		char tmp_buf[1024] = {0};
		cJSON *tom_json;
		int ret = 0;

		if(( (p1 = strstr(buf,"{")) == NULL )||( (p2 = strstr(p1,"\r\n")) == NULL) )
		{
			return ret;
		}
		memset(tmp_buf,0,sizeof(tmp_buf));
		memcpy(tmp_buf,p1,p2-p1);
		printf("get http server json string,hahaha: %s, %d\r\n",tmp_buf,strlen(tmp_buf));
		tom_json = cJSON_Parse(tmp_buf);
		if(tom_json)
		{				
				if(parse_type == JSON_COMMON)
				{
					ret =parse_COMMON_data(tom_json);
				}
				else if(parse_type == JSON_ADDCARIO)	//车辆OBD插入拔出记录
				{
					ret =parse_ADDCARIO_data(tom_json);
				}						
				else if(parse_type == JSON_TOKEN)			//OBD登录获取Token
				{	
					ret =parse_TOKEN_data(tom_json);					
				}
				else if(parse_type == JSON_SERVER_TIME)	//终端校时
				{	
				}
				else if(parse_type == JSON_FENCE)			//获取登录成功后的一系列数据
				{
				       ret =  parse_FENCE_data(tom_json);		
				}
				else
						printf("JSON_PARSE_TYPE error\r\n");
					
				cJSON_Delete(tom_json);
		}
	
		return ret;
}


/*
receice trust buf: 
AT+QIRD=0
+QIRD: 171
["{\"cmd\":1,\"param1\":\"tom1\"}","{\"cmd\":2,\"param1\":\"tom2\"}","{\"cmd\":3,\"param1\":\"tom3\"}","{\"cmd\":4,\"param1\":\"tom4\"}","{\"cmd\":5,\"param1\":\"tom5\"}"]

OK
*/
int parse_udp_server_data(char *buf)
{
		char *p1,*p2,*p3;
		//如果udp server 一次性发送的命令不超过4个，这个buf可以改为256
		char tmp_buf[512] = {0};
		cJSON *tom_json,*json_val,*json_t1,*json_t2;
		int i,index,ret = 0;
		
		if( (p1 = strstr(buf,"+QIRD:")) != NULL )
		{
				if( (p2 = strstr(p1,"\r\n")) != NULL)
				{
						memset(tmp_buf,0,sizeof(tmp_buf));
						memcpy(tmp_buf,p1+7,p2-p1-7);
//						printf("%s,%d\r\n",tmp_buf,atoi(tmp_buf));
						if(atoi(tmp_buf) > 2)
						{
								if( (p3 = strstr(p2+2,"\r\n")) != NULL)
								{
										memset(tmp_buf,0,sizeof(tmp_buf));
										memcpy(tmp_buf,p2+2,p3-p2-2);
										printf("[%s]---> %s\r\n",__func__,tmp_buf);
									
										tom_json = cJSON_Parse(tmp_buf);
										if(tom_json)
										{
												index = cJSON_GetArraySize(tom_json);
												for(i=0;i<index;i++)
												{
														json_val = cJSON_GetArrayItem(tom_json,i);
														if(json_val)
														{
																json_t1 = cJSON_Parse(json_val->valuestring);
																if(json_t1)
																{
																		json_t2 = cJSON_GetObjectItem(json_t1,"cmd");
																		printf("receive udp server cmd: %d\r\n",json_t2->valueint);
#if 1																	
																		//这个会在UDP发送任务中调用，导致切换不到打包任务
//																		udp_server_cmd_action(json_t2->valueint);
																		//在数据打包任务中解析服务器发送的命令
//																		server_cmd = json_t2->valueint;
#else																	
																		ret = Tom_Queue_In(&UDP_cmd_queue,json_t2->valueint);
																		if(ret == 1)
																				printf("Tom_Queue_In successed,hahaha\r\n");
																		else
																				printf("Tom_Queue_In failed,wwwwww\r\n");
#endif																		
																}															
														}
												}
												ret = 1;
										}
								}
						}
						else if(atoi(tmp_buf) == 2)
						{
								ret = 1;
//								printf("The server has no commands to process,hahaha\r\n");
						}
				}
		}

		return ret;
}


//timezone可正可负
void UTC_to_BJtime(t_style*	utc_time, int timezone)
{
    int year,month,day,hour;
    int lastday 	= 0;
    int lastlastday = 0;

    year   = utc_time->year;
    month  = utc_time->month;
    day    = utc_time->date;
    hour   = utc_time->hour + timezone; 
	
    if(month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
    {
        lastday = 31;
        if(month == 3)
				{
            if((year%400 == 0)||(year%4 == 0 && year%100 != 0))
                lastlastday = 29;
            else
                lastlastday = 28;
        }
        if(month == 8)
            lastlastday = 31;
    }
    else if(month == 4 || month == 6 || month == 9 || month == 11)
    {
        lastday = 30;
        lastlastday = 31;
    }
    else
    {
        lastlastday = 31;
        if((year%400 == 0)||(year%4 == 0 && year%100 != 0))
            lastday = 29;
        else
            lastday = 28;
    }

    if(hour >= 24)
    {					
				hour -= 24;
				day += 1; 

				if(day > lastday)
				{ 		
						day -= lastday;
						month += 1;

						if(month > 12)
						{		
								month -= 12;
								year += 1;
						}
				}
    }
    if(hour < 0)
    {
				hour += 24;
				day -= 1; 
				if(day < 1)
				{					  
						day = lastlastday;
						month -= 1;
						if(month < 1)
						{ 			
								month = 12;
								year -= 1;
						}
				}
    }
	
//	t->year  = year;
		utc_time->year  = year+2000;
		utc_time->month = month;
		utc_time->date  = day;
		utc_time->hour  = hour;
		utc_time->min   = utc_time->min;
		utc_time->sec   = utc_time->sec;
}


//获取当前时间和时间戳
void Get_current_time(char *time_buf)
{
		char tmp_buf[128] = {0};
		char *p1,*p2;
		int time_zone,tmp_zone;
		t_style tm_188;
		static int rtcflag = 0;

		if(rtcflag == 1)	
		{
			  RTC_TimeGetStr(tmp_buf);
				if(time_buf)
				{
						memcpy(time_buf,tmp_buf,strlen(tmp_buf));
				}
				//printf("bj time %s\r\n", tmp_buf);	
				return;
		}

		//获取当前时间，并封装在结构体中，供调用
		if (sendCommand("AT+CCLK?\r\n", "OK\r\n", 3000, 5) == 1);
		else 
			printf("AT+CCLK? send failed\r\n");	
		
//		printf("[%s]--> %s\r\n",__func__,downfile_buffer);
		if( (p1 = strstr((char *)downfile_buffer,"\"")) != NULL )
		{
				if((p2 = strstr(p1+1,"\"")) != NULL )
				{
						memset(tmp_buf,0,128);
						memset(&tm_188,0,sizeof(tm_188));
						memcpy(tmp_buf,p1+1,p2-p1);							

						sscanf(tmp_buf,"%d/%d/%d,%d:%d:%d+%d",(uint16_t *)&(tm_188.year),(uint8_t *)&(tm_188.month),(uint8_t *)&(tm_188.date),(uint8_t *)&(tm_188.hour),(uint8_t *)&(tm_188.min),(uint8_t *)&(tm_188.sec),&time_zone);
	
						if(time_zone >= 0)
						{
								tmp_zone = (32-time_zone)/4;
						}
						else if(time_zone < 0)
						{
								tmp_zone = (32-time_zone+1)/4;
						}
						
						UTC_to_BJtime(&tm_188,tmp_zone);
						printf("现在是北京时间: %04d年%02d月%02d日,%02d:%02d:%02d\n",(tm_188.year),(tm_188.month),(tm_188.date),(tm_188.hour),(tm_188.min),(tm_188.sec));
						memset(tmp_buf,0,128);
						sprintf(tmp_buf,"%04d-%02d-%02d %02d:%02d:%02d",(tm_188.year),(tm_188.month),(tm_188.date),(tm_188.hour),(tm_188.min),(tm_188.sec));
						if(time_buf)
								memcpy(time_buf,tmp_buf,strlen(tmp_buf));
						
						rtcflag = 1;
						
						RTC_TimeRegulateSetviaEc20(tm_188);
						RTC_TimeGetStr(tmp_buf);
						printf("bj time %s\r\n", tmp_buf);																		
				}	
		}
		else
				printf("[%s] parse failed\r\n",__func__);
}


//4G模块网络配置，http协议配置，udp协议配置
void Module_protocol_init(void)
{
		int net_retry;
	
		//测试MCU与4G模块是否通信正常
		if(sendCommand("AT\r\n","OK\r\n", 2000, 2) == 1);
		else
		{
				printf("MCU communicates with 4G_Moudle failed\r\n");
				return;
		}
		
		vTaskDelay(10);	
		memset(Module_dcoding,0,20);
		memset(OBD_usr_name,0,20);
		get_IMEI((char *)Module_dcoding);
		get_IMEI((char *)OBD_usr_name);
		
		//显示错误详细信息
		if (sendCommand("AT+CMEE=2\r\n", "OK\r\n", 2000, 5) == 1);
		else
				printf("AT+CMEE? send failed\r\n");
		vTaskDelay(10);
		
		if (sendCommand("AT+CPIN?\r\n", "READY\r\n", 2000, 5) == 1);
		else
				printf("AT+CPIN? send failed\r\n");
		vTaskDelay(10);
		
		//设置为自动找网模式
		if (sendCommand("AT+QCFG=\"nwscanmode\",0\r\n", "OK\r\n", 2000, 5) == 1);
		else
				printf("AT+QCFG send failed\r\n");
		vTaskDelay(10);
		
		//网络重试机制
		for(net_retry = 0;net_retry < 5;net_retry++)
		{
				//查看网络注册状态，1：local SIM card ,0: remote SIM card
				if (sendCommand("AT+CREG?\r\n", ",1\r\n", 2000, 15) == 1)				//本地SIM卡,一般要等十几s
				{
						printf("Register local network successfully.\r\n");
						break;
				}
				else 	if (sendCommand("AT+CREG?\r\n", ",5\r\n", 2000, 5) == 1) //漫游SIM卡
				{
						printf("Register roaming network successfully.\r\n");
						break;
				}
				else
				{		
						printf("Register network fail,please retry\r\n");
				}
				vTaskDelay(10);
				if(net_retry == 4)
				{
						printf("Re-register the network has arrived, please exit and restart\r\n");
						__set_FAULTMASK(1);//关闭总中断
						NVIC_SystemReset();//请求单片机重启
				}
		}
		
		if (sendCommand("AT+QSPN\r\n", "OK\r\n", 2000, 5) == 1);//
		else 
				printf("AT+QSPN send failed\r\n");	
		vTaskDelay(10);		
		
		//读取自动同步网络时间的模式和状态
		if (sendCommand("AT+QNTP=1,\"ntp1.aliyun.com\",123\r\n", "QNTP:", 2000, 10) == 1);//设置手动从NTP服务器同步网络时间，端口一般都是123，我使用阿里云的NTP服务器,1~7都可以
		else 
		{
				printf("setting time synchronization failed\r\n");
				return;
		}
		vTaskDelay(10);	
		
		if (sendCommand("AT+QGPSCFG=\"nmeasrc\",1\r\n", "OK\r\n", 2000, 10) == 1);
		else
				printf("AT+QGPSCFG send failed\r\n");
		vTaskDelay(10);
		
		if (sendCommand("AT+QGPS=1\r\n", "OK\r\n", 2000, 10) == 1);
		else
				printf("AT+QGPS send failed\r\n");
		vTaskDelay(10);	
		
		//post初始化设置
		if (sendCommand("AT+QHTTPCFG=\"contextid\",1\r\n", "OK\r\n", 2000, 5) == 1);//
		else 
				printf("setting PDP context failed\r\n");	
		vTaskDelay(10);
		
		if (sendCommand("AT+QICSGP=1\r\n", "OK\r\n", 2000, 5) == 1);//
		else 
				printf("setting GPRS mode failed\r\n");	
		vTaskDelay(10);
		
		//这句是设置联通网络，有待确定，当使用移动sim卡时，是否还能配置成功
//		if (sendCommand("AT+QICSGP=1,1,\"UNINET\",\"\",\"\",1\r\n", "OK\r\n", 2000, 5) == 1);//
//		else 
//				printf("setting GPRS mode failed\r\n");	
//		vTaskDelay(10);
		
//		if (sendCommand("AT+QIACT=1\r\n", "OK\r\n", 2000, 10) == 1);//
//		else 
//				printf("AT+QIACT failed\r\n");	
//		vTaskDelay(10);
		
		if (sendCommand("AT+QIACT?\r\n", "OK\r\n", 2000, 10) == 1);//
		else 
				printf("AT+QIACT? send failed\r\n");	
		vTaskDelay(10);		
		
		if (sendCommand("AT+QHTTPCFG?\r\n", "OK\r\n", 2000, 5) == 1);//
		else 
				printf("AT+QHTTPCFG? send failed\r\n");	
		vTaskDelay(10);
		

		UDP_Client_init(UDP_SERVER,UDP_SERVER_PORT);
}


void post_head_switch(char mode)
{
		if(mode == 0)
		{
					if (sendCommand("AT+QHTTPCFG=\"requestheader\",0\r\n", "OK\r\n", 2000, 5) == 1);//
					else 
							printf("requestheader context failed\r\n");	
					vTaskDelay(10);
					if (sendCommand("AT+QHTTPCFG=\"responseheader\",0\r\n", "OK\r\n", 2000, 5) == 1);//
					else 
							printf("responseheader context failed\r\n");	
					vTaskDelay(10);
					if (sendCommand("AT+QHTTPCFG?\r\n", "OK\r\n", 2000, 5) == 1);//
					else 
							printf("AT+QHTTPCFG? send failed\r\n");	
					vTaskDelay(10);
		}
		else if(mode == 1)
		{
					if (sendCommand("AT+QHTTPCFG=\"requestheader\",1\r\n", "OK\r\n", 2000, 5) == 1);//
					else 
							printf("requestheader context failed\r\n");	
					vTaskDelay(10);
					if (sendCommand("AT+QHTTPCFG=\"responseheader\",1\r\n", "OK\r\n", 2000, 5) == 1);//
					else 
							printf("responseheader context failed\r\n");	
					vTaskDelay(10);
					if (sendCommand("AT+QHTTPCFG?\r\n", "OK\r\n", 2000, 5) == 1);//
					else 
							printf("AT+QHTTPCFG? send failed\r\n");	
					vTaskDelay(10);
		}
}


//get请求公共接口，只需传入URL,加入retry机制
int Get_Request(char *url,unsigned char Retry)
{
		char send_buf[128] = {0};
		printf("welcome to [%s]\r\n",__func__);
		memset(send_buf,0,sizeof(send_buf));
		sprintf(send_buf,"AT+QHTTPURL=%d,%d\r\n",strlen(url),30);//URL	
		if (sendCommand(send_buf, "CONNECT\r\n", 2000, 20) == 1);//
		else 
		{
				printf("AT+QHTTPURL= failed\r\n");	
				return 1;
		}
		vTaskDelay(10);
		
		if (sendCommand(url, "OK\r\n", 3000, 10) == 1);
		else 
		{
				printf("SET URL failed\r\n");
				return 1;
		}
		
		if (sendCommand("AT+QHTTPGET=60\r\n", "+QHTTPGET: ", 2000, 20) == 1);//
		else
		{
				printf("AT+QHTTPGET= failed\r\n");
				return 1;
		}
		vTaskDelay(10);
		return 0;
}

//post请求公共接口，只需传入URL,加入retry机制
int Post_Request(char *url,char *context,unsigned char Retry,JSON_PARSE_TYPE parse_type)
{
		char send_buf[64] = {0};
		unsigned char i,ret = 1;
		for (i = 0; i < Retry; i++)
		{
				memset(send_buf,0,64);
				sprintf(send_buf,"AT+QHTTPURL=%d,%d\r\n",strlen(url),30);//请求
				
				//这个超时必须改长点，不然很容易失败
				if (sendCommand(send_buf, "CONNECT\r\n", 3000, 10) == 1);//URL
				else 
				{
						printf("AT+QHTTPURL= failed\r\n");
						continue;
				}
				vTaskDelay(10);
				
				if (sendCommand(url, "OK\r\n", 3000, 5) == 1);
				else 
				{
						printf("SET URL failed\r\n");
						continue;
				}
				vTaskDelay(10);
				
				memset(send_buf,0,64);
				sprintf(send_buf,"AT+QHTTPPOST=%d,%d,%d\r\n",strlen(context),50,20);//URL
				if (sendCommand(send_buf, "CONNECT\r\n", 3000, 5) == 1);//内容
				else 
				{
						printf("AT+QHTTPPOST= failed\r\n");	
						continue;
				}
				vTaskDelay(10);

				if (sendCommand(context, "OK\r\n", 3000, 5) == 1);
				else 
				{
						printf("SET URL message failed\r\n");
						continue;
				}
				vTaskDelay(10);
				
				//每次的超时尽量长点，不然有可能收不到
				if (sendCommand("AT+QHTTPREAD=30\r\n", "OK\r\n", 5000, 5) == 1);
				else 
				{
						printf("AT+QHTTPREAD= failed\r\n");	
						continue;
				}
				ret= 0;
				break;
		}
		return ret;
}


//post请求公共接口，只需传入URL,加入retry机制
int Post_Request_pack(char *url,char *context,unsigned char Retry,JSON_PARSE_TYPE parse_type)
{
	 
		unsigned char  ret = 0;
		printf("[%s]--->\r\npost url: %s\r\npost context: %s\r\n",__func__,url,context);
		STM_EVAL_LEDOn(BLUE); 
		if(Post_Request(url,context,Retry,parse_type)==1) 
		{
			printf("Post_Request_pack Post_Request error\r\n");
			return 0;
		}
		else
		{
			ret = parse_http_server_data((char *)downfile_buffer,parse_type);
			STM_EVAL_LEDOff(BLUE); 
		}
		return ret;
}


//这函数暂时关闭不了http请求，有可能需要关闭tcp/ip连接
void close_gprs(void)
{
		if (sendCommand("AT+QIDEACT\r\n", "OK\r\n", 5000, 8) == 1);
		else 
				printf("AT+QIDEACT send failed\r\n");
	
		vTaskDelay(100);	
}


void get_IMEI(char *device_coding)
{
		char *p1,*p2;
		char tmp_buf[20] = {0};
		
#ifdef M_FIX_IMEI
	memcpy(device_coding, FIX_IMEI_CONST,strlen(FIX_IMEI_CONST));
	return;
#endif  

		if (sendCommand("AT+GSN\r\n", "OK\r\n", 2000, 5) == 1);
		else 
				printf("AT+GSN send failed\r\n");
		vTaskDelay(10);
		
		if( (p1 = strstr((char *)downfile_buffer,"AT+GSN")) != NULL )
		{
				if( (p2 = strstr(p1+9,"\r\n")) != NULL )
				{
						memset(tmp_buf,0,20);
						memcpy(tmp_buf,p1+9,p2-p1-9);
						printf("IMEI :%s, %d\r\n",tmp_buf,strlen(tmp_buf));
						memcpy(device_coding, tmp_buf,strlen(tmp_buf));
				}
		}
}


void Get_GPS_Data_Report(Vehicle_GPS	*data)
{	
#ifdef M_NEWHARDWARE
		Get_Ubox_GPS_Data(data);
#else
		//解析数据，填充到数据采集中
		parseGpsBuffer(data);
		// printGpsBuffer();			
#endif
}

void get_file_size(int *file_size)
{
	char  dst[32];
	char *p1;
	
	memset(dst,0,sizeof(dst));
	p1=GetGNRMCInfo(downfile_buffer,"+QHTTPGET:"," ",",",dst); 
	memset(dst,0,sizeof(dst));
	p1=GetGNRMCInfo(p1,",",",",",",dst); 
	memset(dst,0,sizeof(dst));
	p1=GetGNRMCInfo(p1,",",",","\r\n",dst);  
	*file_size = atoi(dst);
}
void get_Vehicle_file_size(int *file_size)
{
		char *p1,*p2;
		char tmp_buf[20] = {0};
#ifdef M_NEWHARDWARE
		if (sendCommand("AT+QFLST=\"UFS:1.bin\"\r\n", "OK\r\n", 2000, 5) == 1);
#else
	    if (sendCommand("AT+QFLST=\"RAM:1.bin\"\r\n", "OK\r\n", 2000, 5) == 1);
#endif
		else 
		{
				printf("AT+QFLST failed\r\n");	
		}
		vTaskDelay(10);
		
		if( (p1 = strstr((char *)downfile_buffer,",")) != NULL )
		{
				if( (p2 = strstr(p1,"\r\n")) != NULL )
				{
						memset(tmp_buf,0,20);
						memcpy(tmp_buf,p1+1,p2-p1-1);
						printf("Vehicle_file_size :%s, %d\r\n",tmp_buf,strlen(tmp_buf));
						*file_size = atoi(tmp_buf);
				}
		}
}

void u_blox_init(void);
void u_blox(void  );

void Data_pack(void *pvParameters)
{
		//4G模块上电
		#ifndef M_NEWHARDWARE
		POWER_EN_Config();
		#endif
		LTE_Module_Init();
		Module_protocol_init();
		main_SPI();
#if 0
		if (sendCommand("AT+QFLDS=\"RAM\"\r\n", "OK\r\n", 2000, 5) == 1);
		else 
		{
				printf("AT+QFLDS failed\r\n");	
				 
		}
		if (sendCommand("AT+QFLDS=\"UFS\"\r\n", "OK\r\n", 2000, 5) == 1);
		else 
		{
				printf("AT+QFLDS failed\r\n");	
				 
		}
#endif
		 cmd_main();
		while(1)
		{				
			 	fcar_data_pack();
				vTaskDelay(5000); 
		}
}


void Data_transfer(void *pvParameters)
{
	InitQueue(&UDP_data_queue);
	u_blox_init();
	while(1)
	{				
		udp_send_data();
		vTaskDelay(1000); 
		//u_blox( );
	}
}

char  * GetGNRMCInfo(char *src,char  *src_start,char  *sep_start,char  *sep_end,char  *dst)
{
    char *p1,*p2,*p3;
    char  buf[128] = {0};
    p1=NULL;
    p2=NULL;
    p3=NULL;

    memset(buf,0,sizeof(buf));

    if(src==NULL)          return NULL;
    if(src_start==NULL) return NULL;
    if(sep_start==NULL) return NULL;
    if(sep_end==NULL)  return NULL;


    p1 = strstr(src,src_start);

    if( p1 == NULL )	 return NULL;

    p2 = strstr(p1,sep_start);

    if(p2 == NULL ) return NULL;
    if(p2+1  == NULL ) return NULL;


    p3 = strstr((p2+1),sep_end);

    if( p3 == NULL )return NULL;

    if((p2+1)  !=  p3)
    {
        // printf("p2+1=%s\r\n",p2+1);
        memcpy(dst,(p2+1),(p3-p2-1));
    }
    else
    {
        *dst=NULL;
    }
    return p3;

}

char  * GetGNRMCInfoItem(char *src,char  *src_start,char  *src_end,char  *dst)
{
	char *p1,*p2,*p3;
	char  buf[128] = {0};
	p1=NULL;
	p2=NULL;
	memset(buf,0,sizeof(buf));

	if(src==NULL)          return NULL;
	if(src_start==NULL) return NULL;
	if(src_end==NULL) return NULL;

	p1 = strstr(src,src_start);

	if(p1 == NULL ) return NULL;

	p2 = strstr(p1,src_end);
	if( p2 == NULL ) return NULL;

	memcpy(dst,p1-3,(p2-p1+strlen(src_end)+3));

	return p1;

}

//WGS84
//+QGNSSRD: $GNRMC,035132.000,A,2228.2743,N,11353.3708,E,0.31,176.37,250719,,,A*79


//****//
/*
$GNRMC,,V,,,,,,,,,,N*4D
$GNVTG,,,,,,,,,N*2E
$GNGGA,,,,,,0,00,99.99,,,,,,*56
$GNGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99*2E
$GNGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99*2E
$GPGSV,1,1,00*79
$GLGSV,1,1,00*65
$GNGLL,,,,,,V,N*7A
*/
int  Get_Ubox_GPS_Data(Vehicle_GPS *data)
{
	char  dst[128];
	char *p1;
	uint8_t *buf;
	char *p2;
	
	memset(data,0,sizeof(Vehicle_GPS));	
	memset(dst,0,sizeof(dst));
	if(Save_Data.isGetData == true) 
	{
		buf=Save_Data.GPS_Buffer;
		Save_Data.isGetData=false;

		 if(strstr((char *)RxBuffer2, "RMC") != NULL ) 
		{ 
			p2	=GetGNRMCInfoItem((char *)RxBuffer2, "RMC","\r\n",  dst);
			if(p2)printf("%s\r\n",dst);	
		}
		//printf("GPS_Buffer=\r\n%s\r\n",buf);	
	}
	else
	{
		return 1;
	}
	 
       memset(dst,0,sizeof(dst));
	p1=GetGNRMCInfo(buf,"RMC",",",",",dst);//UTC时间 hhmmss.sss
	if(p1)  printf("UTC hhmmss.sss=%s\r\n",dst);
	else  return 1;

	memset(dst,0,sizeof(dst));
	p1=GetGNRMCInfo(p1,",",",",",",dst); //状态 A/V
	{
		if(dst[0] == 'A')
		{
			data->pos_status = 0;
		}
		else if(dst[0] == 'V')
		{
			data->pos_status = 1;
		}
	}
	
	memset(dst,0,sizeof(dst));
	p1=GetGNRMCInfo(p1,",",",",",",dst); //纬度 ddmm.mmmm
	{
		if(p1)
		get_final_gps_data(dst,&(data->lng));
	}

	memset(dst,0,sizeof(dst));
	p1=GetGNRMCInfo(p1,",",",",",",dst); //北纬/南纬 N/S


	memset(dst,0,sizeof(dst));
	p1=GetGNRMCInfo(p1,",",",",",",dst); //经度 ddmm.mmmm
	{
		if(p1)
		get_final_gps_data(dst,&(data->lat));
	}

	memset(dst,0,sizeof(dst));
	p1=GetGNRMCInfo(p1,",",",",",",dst); //E/W

	
	memset(dst,0,sizeof(dst));
	p1=GetGNRMCInfo(p1,",",",",",",dst); //速度节
	data->speed=atof(dst) ;


	memset(dst,0,sizeof(dst));
	p1=GetGNRMCInfo(p1,",",",",",",dst); //方位角度
	data->angle= atof(dst);

	memset(dst,0,sizeof(dst));
	p1=GetGNRMCInfo(p1,",",",",",",dst); //UTC日期 ddmmyy
	printf("UTC ddmmyy=%s\r\n",dst);

	memset(dst,0,sizeof(dst));
	p1=GetGNRMCInfo(p1,",",",",",",dst); //磁偏角 000 - 180

	memset(dst,0,sizeof(dst));
	p1=GetGNRMCInfo(p1,",",",",",",dst); //磁偏角方向 E/W

	data->pos_type ;

	memset(dst,0,sizeof(dst));
	p1=GetGNRMCInfo(p1,",",",","\r",dst); //模式A/D/E/N
	printf("A/D/E/N=%s\r\n",dst);
 	 
	return 0;
}


int  Get_Ubox_GPS_Data_CN(void)
{
	char  dst[128];
	char *p1;
	uint8_t *buf;
	char *p2;
	char  dst2[32];
	char  dst_t1[32];
	char  dst_t2[32];
	char  dst_t3[32];

	uint8_t i,num;
	 
	memset(dst,0,sizeof(dst));
	memset(dst2,0,sizeof(dst2));
	if(Save_Data.isGetData == true) 
	{
		buf=Save_Data.GPS_Buffer;
		Save_Data.isGetData=false;
		if(strstr((char *)RxBuffer2, "GSV") != NULL ) 
		{ 
			p2	=GetGNRMCInfoItem((char *)RxBuffer2, "GSV","\r\n",  dst);
			if(p2)printf("%s\r\n",dst);	

			
			memset(dst_t1,0,sizeof(dst_t1));
			p1=GetGNRMCInfo(dst,"GSV",",",",",dst_t1);
			
			memset(dst_t2,0,sizeof(dst_t2));
			p1=GetGNRMCInfo(p1,",",",",",",dst_t2);


			memset(dst_t3,0,sizeof(dst_t3));
			p1=GetGNRMCInfo(p1,",",",",",",dst_t3);

			if(strstr((char *)dst_t3, "00*79") != NULL )  
			{
				printf(" 卫星总数 :%d\r\n",0);
				return 0;
			}
				  
			num=(atoi)(dst_t3);
			printf(" 卫星总数 :%d\r\n",num);
			
			for(i=0;i<num;i++)
			{
			
				printf(" 卫星编号 :%d ", i);
				memset(dst2,0,sizeof(dst2));
				p1=GetGNRMCInfo(p1,",",",",",",dst2);
				
				memset(dst2,0,sizeof(dst2));
				p1=GetGNRMCInfo(p1,",",",",",",dst2);
				
				memset(dst2,0,sizeof(dst2));
				p1=GetGNRMCInfo(p1,",",",",",",dst2);
				
				memset(dst2,0,sizeof(dst2));
				p1=GetGNRMCInfo(p1,",",",",",",dst2);
				if(p1==NULL)
				{
					printf(" 信噪比C\No : \r\n" );
					break;
				}
				else
				{
				       printf(" 信噪比C\No : %s\r\n", dst2 );
				}
			}
			
		}
              
		 if(strstr((char *)RxBuffer2, "RMC") != NULL ) 
		{ 
			p2	=GetGNRMCInfoItem((char *)RxBuffer2, "RMC","\r\n",  dst);
			if(p2)printf("%s\r\n",dst);	
		}
	}

}

void GPS_BUF_SAVE(uint8_t *buf,uint16_t len)
{
	memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);    
	if(len>GPS_Buffer_Length)
	memcpy(Save_Data.GPS_Buffer, buf, GPS_Buffer_Length-2);
	else
	memcpy(Save_Data.GPS_Buffer, buf, len);	
	Save_Data.isGetData = true;
}

extern void Config_blox_usart(uint32_t baudrate );

void u_blox_init(void)
{
#ifdef M_NEWHARDWARE
	const uint8_t baudrate[]={0xb5,0x62 ,0x06 ,0x00 ,0x14 ,0x00 ,0x01 ,0x00 ,0x00 ,0x00 ,0xd0 ,0x08 ,0x00 ,0x00 ,0x00 ,0xc2 ,0x01 ,0x00 ,0x07 ,0x00 ,0x07 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0xc4 ,0x96 ,0xb5 ,0x62 ,0x06 ,0x00 ,0x01 ,0x00 ,0x01 ,0x08 ,0x22 };
	 
	POWER_UBOX_EN_Config();
	Config_blox_usart(9600);
	vTaskDelay(400); 
	u2_printfu_blox_M((char *)baudrate,sizeof(baudrate));
	Config_blox_usart(115200);
	vTaskDelay(50);
	memset(RxBuffer2,0,RXBUFFERSIZE_UBOX);
	RxCounter2 = 0;
#endif
}
void u_blox_Test(void *pvParameters)
{
#ifdef M_NEWHARDWARE
	if(strstr((char *)RxBuffer2, "\r\n") != NULL )
	{
		GPS_BUF_SAVE(RxBuffer2, strlen(RxBuffer2));
		RxCounter2=0;
		Get_Ubox_GPS_Data_CN();
	}	
	vTaskDelay(2000);
#endif
}

void u_blox(void  )
{
#ifdef M_NEWHARDWARE
	Vehicle_GPS gpsdata;
	if(strstr((char *)RxBuffer2, "\r\n") != NULL )
	{
		GPS_BUF_SAVE(RxBuffer2, strlen(RxBuffer2));
		RxCounter2=0;
		Get_Ubox_GPS_Data(&gpsdata);
	}	 	 
#endif
}

