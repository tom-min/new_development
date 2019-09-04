#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "Fcar_Api.h"
#include "cJSON.h"
#include "4G_Common_Api.h"
#include "md5.h" 
#include "STD_OBD_CMD_PROCESS.h"
#include "my_queue.h"
#include "FreeRTOS.h"
#include "timers.h" 
#include "task.h"
#include "udp_queue.h"
#include "user_config.h"
#include "App.h"
#include "stm322xg_eval.h"


#define JSON_DEBUG
//#define URL_SW

#define EARTH_RADIUS  6378138.0
#define PI 			  		3.1415926	

#define ARR_SIZE(X)			(sizeof(X)/sizeof(X[0]))

int OBD_STATUS = 1;
int Vehicle_fire;	//0:默认状态，1:点火状态，2:熄火状态

extern intptr_t m_protocol_index;
extern Tom_QueueData  UDP_cmd_queue;
extern TimerHandle_t  test1Timer_Handle;
extern TimerHandle_t  test2Timer_Handle;
extern TimerHandle_t  test3Timer_Handle;
extern TimerHandle_t  test4Timer_Handle;

extern uint8_t  GPS_timer_flag;
extern uint16_t Vehicle_diagnosis_timer;
extern uint16_t Data_collection_timer;
extern uint8_t Fence_Warning_timer;

extern char Module_dcoding[20];

unsigned char OBD_usr_name[20];
unsigned char OBD_password[10];
unsigned char OBD_TOKEN_STR[512];
unsigned char OBD_PROPERTY[10];

double	FENCE_radius; 
double	FENCE_lng;
double	FENCE_lat;

extern uint8_t data_flag;
extern uint8_t diag_flag;
extern uint8_t ready_status_buf[20];
extern uint8_t iupr_buf[20];

extern TaskHandle_t  data_pack_task;
extern TaskHandle_t  data_transfer_task;
extern TaskHandle_t  Task_ObdiiCmdProcess_Proc_handle;

extern int server_cmd;
extern _SaveData Save_Data;//其他项目的经纬度
extern Vehicle_Model_Info tom_vehicle_model_data;


pudp_queue	UDP_data_queue;


//6个HTTP请求URL
#ifdef URL_SW
const char URL_inOut[] = "http://carnet-t.szfcar.com/data/cario/addcario";		//设备接入插口
const char OBD_Token[] = "http://carnet-t.szfcar.com/uaa/oauth/token"; 				//获取token
const char URL_login[] = "http://carnet-t.szfcar.com/data/carlogin/addlogin"; //车辆登录接口
const char URL_logout[] = "http://carnet-t.szfcar.com/data/carlogout/addCarlogoutRec"; 	//车辆登出接口
const char Get_Server_Time[] = "http://carnet-t.szfcar.com/data/devicetime/getServerTime"; 	//终端校时
const char Get_FENCE[] = "http://carnet-t.szfcar.com/data/carnetmap/getfence"; 	//获取圆形栅栏
const char Vehicle_Vibra[] = "http://carnet-t.szfcar.com/data/carshake/upshake"; 	//车身震动上报

#else

const char URL_inOut[] = "http://14.21.40.133:8008/data/cario/addcario";		//设备接入插口
const char OBD_Token[] = "http://14.21.40.133:8008/uaa/oauth/token"; 				//获取token
const char URL_login[] = "http://14.21.40.133:8008/data/carlogin/addlogin"; //车辆登录接口
const char URL_logout[] = "http://14.21.40.133:8008/data/carlogout/addCarlogoutRec"; 	//车辆登出接口
const char Get_Server_Time[] = "http://14.21.40.133:8008/data/devicetime/getServerTime"; 	//终端校时
const char Get_FENCE[] = "http://14.21.40.133:8008/data/carnetmap/getfence"; 	//获取圆形栅栏
const char Vehicle_Vibra[] = "http://14.21.40.133:8008/data/carshake/upshake"; 	//车身震动上报
#endif


extern void get_datacollection(Data_collection *tom_data_collection);
extern void get_Vehicle_diagnosis(Vehicle_diagnosis *tom_Vehicle_diagnosis);
extern int Download_File(void);

static cJSON* status_ready_pack_to_json(char *buf)
{
		int i,len;
		cJSON *data_json;
		char *pri_json;
		char tmp_buf[10] = {0};												
																
		char *status_ready_str[] = {"ready1","ready2","ready3","ready4","ready5",\
																 "ready6","ready7","ready8","ready9","ready10",\
																 "ready11","ready12","ready13","ready14","ready15"};
		
		len = ARR_SIZE(status_ready_str);											
		data_json = cJSON_CreateObject();
		if(data_json)
		{
				for(i=0;i<len;i++)
				{
						memset(tmp_buf,0,10);
						sprintf(tmp_buf,"%d",buf[i]);
						cJSON_AddStringToObject(data_json,	status_ready_str[i],tmp_buf);
				}		
				
				printf("status_ready_pack_to_json successful\r\n");
		}
		else
				printf("status_ready_pack_to_json failed\r\n");
	
#ifdef JSON_DEBUG
		pri_json = cJSON_Print(data_json);
//		pri_json = cJSON_PrintUnformatted(data_json);
//		
		printf("[%s]--->\r\n%s, %d\r\n",__func__,pri_json,strlen(pri_json));
		
		free(pri_json);
#endif		
		
		return (cJSON *)data_json;
}


static cJSON* iupr_pack_to_json(char *rate_value)
{
		int i,len;
		cJSON *data_json;
		char *pri_json;
		char tmp_buf[10] = {0};
																
		char *iupr_str[] = {"iupr1","iupr2","iupr3","iupr4","iupr5","iupr6",\
												 "iupr7","iupr8","iupr9","iupr10","iupr11","iupr12",\
												 "iupr13","iupr14","iupr15","iupr16"};									
		
		len = ARR_SIZE(iupr_str);											
		data_json = cJSON_CreateObject();
		if(data_json)
		{
				for(i=0;i<len;i++)
				{
						memset(tmp_buf,0,10);
						sprintf(tmp_buf,"%d",rate_value[i]);
						cJSON_AddStringToObject(data_json,	iupr_str[i],tmp_buf);
				}		
				printf("iupr_pack_to_json successful\r\n");
		}
		else
			printf("iupr_pack_to_json failed\r\n");
		
#ifdef JSON_DEBUG		
		pri_json = cJSON_Print(data_json);						
//		pri_json = cJSON_PrintUnformatted(data_json);
		
		printf("[%s]--->\r\n%s, %d\r\n",__func__,pri_json,strlen(pri_json));		
		free(pri_json);
#endif
		
		return (cJSON *)data_json;
}


//终端校时
static int get_Server_Time(Vehicle_status *buf)
{
		char pack_buf[512] = {0};
		char context_buf[128] = {0};
		char d_code[20] = {0};
		char *p_buf = context_buf;
		int ret;
	
		memset(pack_buf,0,512);
		memset(context_buf,0,128);
		
		memcpy(d_code,Module_dcoding,strlen(Module_dcoding));
		sprintf(p_buf,"%s=%s","devicecoding",d_code); 
		p_buf += strlen(p_buf);
		get_vehicle_vin(d_code);
		sprintf(p_buf,"%s=%s","vin_sn",d_code); 

		p_buf = pack_buf;
		sprintf(p_buf,"%s\r\n","POST /data/carnetmap/getfence HTTP/1.1");
		p_buf += strlen(p_buf);
#ifdef URL_SW
		sprintf(p_buf,"%s:%s\r\n","Host","carnet-t.szfcar.com");
#else
		sprintf(p_buf,"%s:%s\r\n","Host","14.21.40.133:8008");
#endif
		p_buf += strlen(p_buf);

		sprintf(p_buf,"%s:%s\r\n","Content-Type","application/x-www-form-urlencoded");
		p_buf += strlen(p_buf);
		sprintf(p_buf,"%s:%d\r\n","Content-Length",strlen(context_buf));
		p_buf += strlen(p_buf);
		sprintf(p_buf,"%s:%s %s\r\n\r\n","Authorization","bearer",OBD_TOKEN_STR); 
		p_buf += strlen(p_buf);
		sprintf(p_buf,"%s",context_buf);  

		printf("[%s] context string: %s, len: %d\r\n",__func__,pack_buf,strlen(pack_buf));
		
		post_head_switch(1);
		ret = Post_Request_pack((char *)Get_Server_Time,pack_buf,3,JSON_COMMON);
		if(ret == 0)
		{
				printf("POST encryption URL failed!\r\n");
		}
		post_head_switch(0);
		
		return ret;
}


//车辆OBD插入拔出记录
static int Vehicle_status_pack_to_server(Vehicle_status *buf)
{
		char pack_buf[128] = {0};
		char *p_buf = pack_buf;
		int retry,ret=0;

		memset(pack_buf,0,128);
		sprintf(p_buf,"%s=%s","create_time",buf->create_time);
		p_buf += strlen(p_buf);
		sprintf(p_buf,"&%s=%s","device_coding",buf->device_coding);
		p_buf += strlen(p_buf);
		sprintf(p_buf,"&%s=%d","ioType",buf->type);
		p_buf += strlen(p_buf);
		sprintf(p_buf,"&%s=%s","vin_sn",buf->vin_sn);   

		printf("[%s] string: %s, len: %d\r\n",__func__,pack_buf,strlen(pack_buf));

		post_head_switch(0);
		
		//获取密码失败重试机制
		for(retry = 0;retry < 5;retry++)
		{
				ret = Post_Request_pack((char *)URL_inOut,pack_buf,3,JSON_ADDCARIO);
				if(ret == 0)
				{
						printf("%s failed, retry count: %d\r\n",__func__,retry+1);
				}
				else if(ret == 1)
				{
						break;
				}
				vTaskDelay(2000);
		}
		
		return ret;
}


//车辆启动登录
static int Vehicle_login_pack_to_server(Vehicle_login *buf)
{
		char pack_buf[700] = {0};
		char context_buf[128] = {0};
		char *p_buf = context_buf;
		int retry,ret=0;
		
		memset(context_buf,0,128);
		memset(pack_buf,0,700);

		sprintf(p_buf,"%s=%s","create_time",buf->create_time);
		p_buf += strlen(p_buf);
		sprintf(p_buf,"&%s=%s","device_coding",buf->device_coding);
		p_buf += strlen(p_buf);
		sprintf(p_buf,"&%s=%s","mobile",buf->mobile);
		p_buf += strlen(p_buf);
		sprintf(p_buf,"&%s=%s","vin_sn",buf->vin_sn); 
		p_buf += strlen(p_buf);
		sprintf(p_buf,"&%s=%f","lng",buf->lng); 
		p_buf += strlen(p_buf);
		sprintf(p_buf,"&%s=%f","lat",buf->lat); 

#ifdef BOOT_APPLACATION
		p_buf += strlen(p_buf);
		sprintf(p_buf,"&%s=%d","isUse",0); 
#else 
		p_buf += strlen(p_buf);
		sprintf(p_buf,"&%s=%d","isUse",1); 
#endif	
	
		p_buf = pack_buf;
		sprintf(p_buf,"%s\r\n","POST /data/carlogin/addlogin HTTP/1.1");
		p_buf += strlen(p_buf);
#ifdef URL_SW
		sprintf(p_buf,"%s:%s\r\n","Host","carnet-t.szfcar.com");
#else
		sprintf(p_buf,"%s:%s\r\n","Host","14.21.40.133:8008");
#endif
		p_buf += strlen(p_buf);
		
		sprintf(p_buf,"%s:%s\r\n","Content-Type","application/x-www-form-urlencoded");
		p_buf += strlen(p_buf);
		sprintf(p_buf,"%s:%d\r\n","Content-Length",strlen(context_buf));
		p_buf += strlen(p_buf);
		sprintf(p_buf,"%s:%s %s\r\n\r\n","Authorization","bearer",OBD_TOKEN_STR); 
		p_buf += strlen(p_buf);
		sprintf(p_buf,"%s",context_buf);   

		printf("[%s] context string: %s, len: %d\r\n",__func__,pack_buf,strlen(pack_buf));
		
		post_head_switch(1);
		//登录失败重试机制
		for(retry = 0;retry < 3;retry++)
		{
				ret = Post_Request_pack((char *)URL_login,pack_buf,3,JSON_FENCE);
				if(ret == 0)
				{
						printf("%s failed, retry count: %d\r\n",__func__,retry+1);
				}
				else if(ret == 1)
				{
						break;
				}
				vTaskDelay(1000);
		}
		
		post_head_switch(0);	
		
		return ret;
}


//车辆熄火登出
static int Vehicle_logout_pack_to_server(Vehicle_logout *buf)
{
		char pack_buf[700] = {0};
		char context_buf[128] = {0};
		char *p_buf = context_buf;
		int ret;

		memset(context_buf,0,128);
		memset(pack_buf,0,700);

		sprintf(p_buf,"%s=%s","create_time",buf->create_time);
		p_buf += strlen(p_buf);
		sprintf(p_buf,"&%s=%s","device_coding",buf->device_coding);
		p_buf += strlen(p_buf);
		sprintf(p_buf,"&%s=%s","vin_sn",buf->vin_sn);
		p_buf += strlen(p_buf);
		sprintf(p_buf,"&%s=%f","lng",buf->lng); 
		p_buf += strlen(p_buf);
		sprintf(p_buf,"&%s=%f","lat",buf->lat);
		
		p_buf = pack_buf;
		sprintf(p_buf,"%s\r\n","POST /data/carlogout/addCarlogoutRec HTTP/1.1");
		p_buf += strlen(p_buf);
#ifdef URL_SW
		sprintf(p_buf,"%s:%s\r\n","Host","carnet-t.szfcar.com");
#else
		sprintf(p_buf,"%s:%s\r\n","Host","14.21.40.133:8008");
#endif
		p_buf += strlen(p_buf);	

		sprintf(p_buf,"%s:%s\r\n","Content-Type","application/x-www-form-urlencoded");
		p_buf += strlen(p_buf);
		sprintf(p_buf,"%s:%d\r\n","Content-Length",strlen(context_buf));
		p_buf += strlen(p_buf);
		sprintf(p_buf,"%s:%s %s\r\n\r\n","Authorization","bearer",OBD_TOKEN_STR); 
		p_buf += strlen(p_buf);
		sprintf(p_buf,"%s",context_buf);   

		printf("[%s] context string: %s, len: %d\r\n",__func__,pack_buf,strlen(pack_buf));//打包之后的字符串大于512bytes，细节
		
		post_head_switch(1);
		ret = Post_Request_pack((char *)URL_logout,pack_buf,3,JSON_COMMON);
		if(ret == 0)
		{
				printf("%s failed\r\n",__func__);
		}
		post_head_switch(0);
		
		return ret;
}


//获取圆形栅栏
static int Get_Fence_from_server(void)
{
		char pack_buf[700] = {0};
		char context_buf[64] = {0};
		char d_code[20] = {0};
		char *p_buf = context_buf;
		int ret;
	
		memset(pack_buf,0,700);
		memset(context_buf,0,64);
		
		memcpy(d_code,Module_dcoding,strlen(Module_dcoding));
		sprintf(p_buf,"%s=%s","devicecoding",d_code); 

		p_buf = pack_buf;
		sprintf(p_buf,"%s\r\n","POST /data/carnetmap/getfence HTTP/1.1");
		p_buf += strlen(p_buf);
#ifdef URL_SW
		sprintf(p_buf,"%s:%s\r\n","Host","carnet-t.szfcar.com");
#else
		sprintf(p_buf,"%s:%s\r\n","Host","14.21.40.133:8008");
#endif
		p_buf += strlen(p_buf);

		sprintf(p_buf,"%s:%s\r\n","Content-Type","application/x-www-form-urlencoded");
		p_buf += strlen(p_buf);
		sprintf(p_buf,"%s:%d\r\n","Content-Length",strlen(context_buf));
		p_buf += strlen(p_buf);
		sprintf(p_buf,"%s:%s %s\r\n\r\n","Authorization","bearer",OBD_TOKEN_STR); 
		p_buf += strlen(p_buf);
		sprintf(p_buf,"%s",context_buf);  
		
		printf("[%s] context string: %s, len: %d\r\n",__func__,pack_buf,strlen(pack_buf));
		
		post_head_switch(1);
		ret = Post_Request_pack((char *)Get_FENCE,pack_buf,3,JSON_FENCE);
		if(ret == 0)
		{
				printf("%s failed\r\n",__func__);
		}
		post_head_switch(0);
		
		return ret;
}


//获取Token
int get_obd_token(void)
{
		char context_buf[128] = {0};
		char pack_buf[512] = {0};
		char *p_buf = context_buf;
		int retry,ret=0;

		memset(context_buf,0,sizeof(context_buf));
		memset(pack_buf,0,sizeof(pack_buf));
		
		sprintf(p_buf,"%s=%s","username",OBD_usr_name);
		p_buf += strlen(p_buf);
		sprintf(p_buf,"&%s=%s","password",OBD_password);
		p_buf += strlen(p_buf);
		sprintf(p_buf,"&%s=%s","grant_type","password");
		p_buf += strlen(p_buf);
		sprintf(p_buf,"&%s=%s","scope","app"); 
		
		p_buf = pack_buf;
		sprintf(p_buf,"%s\r\n","POST /uaa/oauth/token HTTP/1.1");
		p_buf += strlen(p_buf);
#ifdef URL_SW
		sprintf(p_buf,"%s:%s\r\n","Host","carnet-t.szfcar.com");
#else
		sprintf(p_buf,"%s:%s\r\n","Host","14.21.40.133:8008");
#endif
		p_buf += strlen(p_buf);
//		sprintf(p_buf,"%s:%s\r\n","Accept","*/*");
//		p_buf += strlen(p_buf);
//		sprintf(p_buf,"%s:%s\r\n","User-Agent","QUECTEL_MODULE");
//		p_buf += strlen(p_buf);
//		sprintf(p_buf,"%s:%s\r\n","Connection","Keep-Alive");
//		p_buf += strlen(p_buf);		
		sprintf(p_buf,"%s:%s\r\n","Content-Type","application/x-www-form-urlencoded");
		p_buf += strlen(p_buf);
		sprintf(p_buf,"%s:%d\r\n","Content-Length",strlen(context_buf));
		p_buf += strlen(p_buf);
		sprintf(p_buf,"%s:%s\r\n\r\n","Authorization","Basic YXBwOjEyMzQ1Ng=="); 
		p_buf += strlen(p_buf);
		sprintf(p_buf,"%s",context_buf);
			
		printf("[%s] context string length: %d\r\n",__func__,strlen(context_buf));
		printf("[%s] context string: %s, len: %d\r\n",__func__,pack_buf,strlen(pack_buf));

		post_head_switch(1);
		//获取token失败重试机制
		for(retry = 0;retry < 3;retry++)
		{
				ret = Post_Request_pack((char *)OBD_Token,pack_buf,3,JSON_TOKEN);
				if(ret == 0)
				{
						printf("%s failed, retry count: %d\r\n",__func__,retry+1);
				}
				else if(ret == 1)
				{
					break;
				}
				else if(ret ==2)
				{
						break;
				}
				vTaskDelay(1000);
		}
				
		post_head_switch(0);
		
		return ret;
}


//车辆诊断数据发送到服务器
static cJSON* Vehicle_diagnosis_pack_to_server(Vehicle_diagnosis *buf)
{
		cJSON *data_json,*pack_json,*status_ready_json;
		char *print_json;
		char tmp_buf[64] = {0};

		pack_json = cJSON_CreateObject();
		if(pack_json)
		{
				//msgType决定上传类型
				cJSON_AddStringToObject(pack_json, "msgType", "4");
				cJSON_AddStringToObject(pack_json, "prop", OBD_PROPERTY);
				data_json = cJSON_CreateObject();
				if(data_json)
				{
						cJSON_AddStringToObject(data_json, "time", buf->create_time);
						cJSON_AddStringToObject(data_json, "etime", buf->endTime);
						cJSON_AddStringToObject(data_json, "code", buf->device_coding);
						memset(tmp_buf,0,64);
						sprintf(tmp_buf,"%d",buf->agreement);
						cJSON_AddStringToObject(data_json, "ag",tmp_buf);
						memset(tmp_buf,0,64);
						sprintf(tmp_buf,"%d",buf->mil_status);
						cJSON_AddStringToObject(data_json, "mil",tmp_buf);
					
						//status_ready
						status_ready_json = status_ready_pack_to_json(ready_status_buf);	
//						cJSON_AddItemToObject(data_json,"status_ready",status_ready_json);//可以转成json字符串发送
						print_json = cJSON_PrintUnformatted(status_ready_json);
						cJSON_AddStringToObject(data_json,"status_ready",print_json);
						cJSON_Delete(status_ready_json);
						free(print_json);	
					
						cJSON_AddStringToObject(data_json, "vin", buf->vin_sn);
					
						memset(tmp_buf,0,64);
						sprintf(tmp_buf,"%d",buf->fault_total);
						cJSON_AddStringToObject(data_json, "fa",tmp_buf);	
						cJSON_AddStringToObject(data_json, "flst", buf->fault_list);				
						cJSON_AddStringToObject(data_json, "slst", buf->sys_list);	
										
				}
				cJSON_AddItemToObject(pack_json, "data", data_json);
				
#ifdef JSON_DEBUG					
				print_json = cJSON_Print(pack_json);
				printf("%s\r\n",print_json);
				free(print_json);
#endif				
		}
		
#if 0		
		tom_UDP_Client(pack_json);

#else		
		
		return (cJSON *)pack_json;
#endif
}


//数据采集发送到服务器
static cJSON* Data_collection_pack_to_server(Data_collection *buf)
{
		cJSON *data_json,*pack_json,*iupr_json;
		char *print_json;
		char tmp_buf[64] = {0};

		pack_json = cJSON_CreateObject();
		if(pack_json)
		{
				//msgType决定上传类型
				cJSON_AddStringToObject(pack_json, "msgType", "1");
				cJSON_AddStringToObject(pack_json, "prop", OBD_PROPERTY);
				data_json = cJSON_CreateObject();
				if(data_json)
				{
						cJSON_AddStringToObject(data_json, "time", buf->create_time);
						cJSON_AddStringToObject(data_json, "code", buf->device_coding);
						memset(tmp_buf,0,64);
						sprintf(tmp_buf,"%.2f",buf->kilometres);
						cJSON_AddStringToObject(data_json, "kil",tmp_buf);
						memset(tmp_buf,0,64);
						sprintf(tmp_buf,"%d",buf->speed);
						cJSON_AddStringToObject(data_json, "sp",tmp_buf);
						memset(tmp_buf,0,64);
						sprintf(tmp_buf,"%.2f",buf->max_torque);
						cJSON_AddStringToObject(data_json, "maxSt",tmp_buf);
						memset(tmp_buf,0,64);
						sprintf(tmp_buf,"%.2f",buf->out_torque);
						cJSON_AddStringToObject(data_json, "outSt",tmp_buf);
						memset(tmp_buf,0,64);
						sprintf(tmp_buf,"%d",buf->friction_torque);
						cJSON_AddStringToObject(data_json, "frict",tmp_buf);
						memset(tmp_buf,0,64);
						sprintf(tmp_buf,"%d",buf->engine_speed);
						cJSON_AddStringToObject(data_json, "engSp",tmp_buf);
						memset(tmp_buf,0,64);
						sprintf(tmp_buf,"%.2f",buf->engine_flow);
						cJSON_AddStringToObject(data_json, "engFl",tmp_buf);
						memset(tmp_buf,0,64);
						sprintf(tmp_buf,"%.2f",buf->put_temperature);
						cJSON_AddStringToObject(data_json, "putT",tmp_buf);
						memset(tmp_buf,0,64);
						sprintf(tmp_buf,"%.2f",buf->out_temperature);
						cJSON_AddStringToObject(data_json, "outT",tmp_buf);
						memset(tmp_buf,0,64);
						sprintf(tmp_buf,"%.2f",buf->pressure_diff);
						cJSON_AddStringToObject(data_json, "press",tmp_buf);
						memset(tmp_buf,0,64);
						sprintf(tmp_buf,"%.2f",buf->intake_volume);
						cJSON_AddStringToObject(data_json, "intake",tmp_buf);
						memset(tmp_buf,0,64);
						sprintf(tmp_buf,"%.2f",buf->reactant_surplus);
						cJSON_AddStringToObject(data_json, "react",tmp_buf);
						memset(tmp_buf,0,64);
						sprintf(tmp_buf,"%.2f",buf->tank_level);
						cJSON_AddStringToObject(data_json, "tank",tmp_buf);
						memset(tmp_buf,0,64);
						sprintf(tmp_buf,"%.2f",buf->coolant_temperature);
						cJSON_AddStringToObject(data_json, "cool",tmp_buf);
						memset(tmp_buf,0,64);
						sprintf(tmp_buf,"%.2f",buf->fuel_injection_quantity);
						cJSON_AddStringToObject(data_json, "fuelJ",tmp_buf);
						memset(tmp_buf,0,64);
						sprintf(tmp_buf,"%.2f",buf->voltage);
						cJSON_AddStringToObject(data_json, "vol",tmp_buf);
						memset(tmp_buf,0,64);
						sprintf(tmp_buf,"%.2f",buf->engine_start);
						cJSON_AddStringToObject(data_json, "engS", tmp_buf);

						cJSON_AddStringToObject(data_json, "fault", buf->fault_code);
						cJSON_AddStringToObject(data_json, "vin", buf->vin_sn);
						
						cJSON_AddStringToObject(data_json, "foCur",buf->frontOCurrent);
						cJSON_AddStringToObject(data_json, "foVol",buf->frontOVoltage);
						cJSON_AddStringToObject(data_json, "boCur",buf->backOCurrent);
						cJSON_AddStringToObject(data_json, "boVol",buf->backOVoltage);
											
						memset(tmp_buf,0,64);
						sprintf(tmp_buf,"%.2f",buf->engineLoad);
						cJSON_AddStringToObject(data_json, "load",tmp_buf);					
						memset(tmp_buf,0,64);
						sprintf(tmp_buf,"%.2f",buf->airInPressure);
						cJSON_AddStringToObject(data_json, "ain",tmp_buf);						
						memset(tmp_buf,0,64);
						sprintf(tmp_buf,"%.2f",buf->airInTemperature);
						cJSON_AddStringToObject(data_json, "aint",tmp_buf);						
						memset(tmp_buf,0,64);
						sprintf(tmp_buf,"%.2f",buf->injectionPwm);
						cJSON_AddStringToObject(data_json, "pwm",tmp_buf);						
						memset(tmp_buf,0,64);
						sprintf(tmp_buf,"%.2f",buf->pressure);
						cJSON_AddStringToObject(data_json, "pres",tmp_buf);						
						memset(tmp_buf,0,64);
						sprintf(tmp_buf,"%.2f",buf->airGasScale);
						cJSON_AddStringToObject(data_json, "gas",tmp_buf);						
						memset(tmp_buf,0,64);
						sprintf(tmp_buf,"%.2f",buf->fireVol);
						cJSON_AddStringToObject(data_json, "fv",tmp_buf);
						cJSON_AddStringToObject(data_json, "soft", buf->software_identification_code);
						cJSON_AddStringToObject(data_json, "cvn", buf->cvn);
						
						//iupr
						iupr_json = iupr_pack_to_json(iupr_buf);	
//						cJSON_AddItemToObject(data_json,"iupr",iupr_json);
						print_json = cJSON_PrintUnformatted(iupr_json);
						cJSON_AddStringToObject(data_json,"iupr",print_json);
						cJSON_Delete(iupr_json);
						free(print_json);
				}
				cJSON_AddItemToObject(pack_json, "data", data_json);
				
#ifdef JSON_DEBUG						
				print_json = cJSON_Print(pack_json);
				printf("%s\r\n",print_json);
				free(print_json);
#endif	
		}
		
#if 0		
		tom_UDP_Client(pack_json);
		
#else		
		
		return (cJSON *)pack_json;
#endif
}


//车辆GPS数据发送到服务器
static cJSON* Vehicle_GPS_pack_to_server(Vehicle_GPS *buf)
{
		cJSON *data_json,*pack_json;
		char *print_json;
		char tmp_buf[64] = {0};
		
		printf("welcome to [%s], hahaha\r\n",__func__);

		pack_json = cJSON_CreateObject();
		if(pack_json)
		{
				//msgType决定上传类型
				cJSON_AddStringToObject(pack_json, "msgType", "2");
				cJSON_AddStringToObject(pack_json, "prop", OBD_PROPERTY);
				data_json = cJSON_CreateObject();
				if(data_json)
				{
						cJSON_AddStringToObject(data_json, "time", buf->create_time);
						cJSON_AddStringToObject(data_json, "code", buf->device_coding);
						cJSON_AddNumberToObject(data_json, "pos",buf->pos_status);
						memset(tmp_buf,0,64);
						sprintf(tmp_buf,"%.6f",buf->lng);
						cJSON_AddStringToObject(data_json, "lng",tmp_buf);
						memset(tmp_buf,0,64);
						sprintf(tmp_buf,"%.6f",buf->lat);
						cJSON_AddStringToObject(data_json, "lat",tmp_buf);
						memset(tmp_buf,0,64);
						sprintf(tmp_buf,"%.2f",buf->speed);
						cJSON_AddStringToObject(data_json, "sp",tmp_buf);
						memset(tmp_buf,0,64);
						sprintf(tmp_buf,"%.2f",buf->angle);
						cJSON_AddStringToObject(data_json, "ag",tmp_buf);
						cJSON_AddNumberToObject(data_json, "pos_type",buf->pos_type);
						cJSON_AddStringToObject(data_json, "GPS_Precision", buf->GPS_Precision);
				}
				cJSON_AddItemToObject(pack_json, "data", data_json);
				
#ifdef JSON_DEBUG					
				print_json = cJSON_Print(pack_json);
				printf("%s\r\n",print_json);
				free(print_json);
#endif	
		}
		
#if 0		
		tom_UDP_Client(pack_json);
		
#else		
		
		return (cJSON *)pack_json;
#endif
}


//驾驶行程数据发送到服务器
static cJSON* Driving_experience_pack_to_server(Driving_experience *buf)
{
		cJSON *data_json,*pack_json;
		char *print_json;
		char tmp_buf[32] = {0};
		
		printf("welcome to [%s], hahaha\r\n",__func__);

		pack_json = cJSON_CreateObject();
		if(pack_json)
		{
				//msgType决定上传类型
				cJSON_AddStringToObject(pack_json, "msgType", "3");
				cJSON_AddStringToObject(pack_json, "prop", OBD_PROPERTY);
				data_json = cJSON_CreateObject();
				if(data_json)
				{
						cJSON_AddStringToObject(data_json, "time", buf->create_time);
						cJSON_AddStringToObject(data_json, "code", buf->device_coding);
						memset(tmp_buf,0,32);
						sprintf(tmp_buf,"%.2f",buf->driveTimeLen);
						cJSON_AddStringToObject(data_json, "timeLen",tmp_buf);					
						cJSON_AddStringToObject(data_json, "dTime",buf->driveTime);					
						memset(tmp_buf,0,32);
						sprintf(tmp_buf,"%.2f",buf->driveMil);
						cJSON_AddStringToObject(data_json, "mil",tmp_buf);					
						memset(tmp_buf,0,32);
						sprintf(tmp_buf,"%.2f",buf->driveOil);
						cJSON_AddStringToObject(data_json, "oil",tmp_buf);					
						memset(tmp_buf,0,32);
						sprintf(tmp_buf,"%d",buf->urgenSpeedUp);
						cJSON_AddStringToObject(data_json, "spUp",tmp_buf);
						memset(tmp_buf,0,32);
						sprintf(tmp_buf,"%d",buf->urgenSpeedDown);
						cJSON_AddStringToObject(data_json, "spDown",tmp_buf);
						memset(tmp_buf,0,32);
						sprintf(tmp_buf,"%d",buf->urgenTurn);
						cJSON_AddStringToObject(data_json, "turn",tmp_buf);	
						memset(tmp_buf,0,32);
						sprintf(tmp_buf,"%d",buf->isIdlingDriving);
						cJSON_AddStringToObject(data_json, "isIdl",tmp_buf);	
						memset(tmp_buf,0,32);
						sprintf(tmp_buf,"%d",buf->isIdlingStop);
						cJSON_AddStringToObject(data_json, "isIdlSt",tmp_buf);	

				}
				cJSON_AddItemToObject(pack_json, "data", data_json);
				
#ifdef JSON_DEBUG					
				print_json = cJSON_Print(pack_json);
				printf("%s\r\n",print_json);
				free(print_json);
#endif
		}
		
#if 0		
		tom_UDP_Client(pack_json);
			
#else		
		
		return (cJSON *)pack_json;
#endif
}


//栅栏越位告警发送到服务器
static cJSON* Fence_Warning_pack_to_server(Fence_Warning *buf)
{
		cJSON *data_json,*pack_json;
		char *print_json;
		char tmp_buf[32] = {0};
		
		printf("welcome to [%s], hahaha\r\n",__func__);

		pack_json = cJSON_CreateObject();
		if(pack_json)
		{
				//msgType决定上传类型
				cJSON_AddStringToObject(pack_json, "msgType", "5");
				cJSON_AddStringToObject(pack_json, "prop", OBD_PROPERTY);
				data_json = cJSON_CreateObject();
				if(data_json)
				{
						cJSON_AddStringToObject(data_json, "time", buf->alarnTime);
						cJSON_AddStringToObject(data_json, "code", buf->device_coding);
						memset(tmp_buf,0,32);
						sprintf(tmp_buf,"%.6f",buf->lng);
						cJSON_AddStringToObject(data_json, "lng",tmp_buf);
						memset(tmp_buf,0,32);
						sprintf(tmp_buf,"%.6f",buf->lat);
						cJSON_AddStringToObject(data_json, "lat",tmp_buf);
						memset(tmp_buf,0,32);
						sprintf(tmp_buf,"%d",buf->alarnEvent);
						cJSON_AddStringToObject(data_json, "event",tmp_buf);

				}
				cJSON_AddItemToObject(pack_json, "data", data_json);
				
#ifdef JSON_DEBUG					
				print_json = cJSON_Print(pack_json);
				printf("%s\r\n",print_json);
				free(print_json);
#endif	
		}
		
#if 0		
		tom_UDP_Client(pack_json);
		
#else		
		
		return (cJSON *)pack_json;
#endif
}


//车身震动上报,换成UDP上传
static cJSON* Vehicle_Vibration_pack_to_server(Vehicle_Vibration *buf)
{
		cJSON *data_json,*pack_json;
		char *print_json;
		char tmp_buf[32] = {0};
		
		printf("welcome to [%s], hahaha\r\n",__func__);

		pack_json = cJSON_CreateObject();
		if(pack_json)
		{
				//msgType决定上传类型
				cJSON_AddStringToObject(pack_json, "msgType", "6");
				cJSON_AddStringToObject(pack_json, "prop", OBD_PROPERTY);
				data_json = cJSON_CreateObject();
				if(data_json)
				{
						cJSON_AddStringToObject(data_json, "time", buf->create_time);
						cJSON_AddStringToObject(data_json, "code", buf->device_coding);
						memset(tmp_buf,0,32);
						sprintf(tmp_buf,"%d",buf->dataType);
						cJSON_AddStringToObject(data_json, "type",tmp_buf);
						cJSON_AddStringToObject(data_json, "data",buf->deviceData);
					
						memset(tmp_buf,0,32);
						sprintf(tmp_buf,"%.6f",buf->lng);
						cJSON_AddStringToObject(data_json, "lng",tmp_buf);
						memset(tmp_buf,0,32);
						sprintf(tmp_buf,"%.6f",buf->lat);
						cJSON_AddStringToObject(data_json, "lat",tmp_buf);

				}
				cJSON_AddItemToObject(pack_json, "data", data_json);
				
#ifdef JSON_DEBUG					
				print_json = cJSON_Print(pack_json);
				printf("%s\r\n",print_json);
				free(print_json);
#endif	
		}
		
		return (cJSON *)pack_json;
}


static double rad(double d) 
{
		return (d * PI / 180.0);
}


//围栏报警
static bool isInCircle(double radius, double lat1, double lng1, double lat2, double lng2) 
{
		double radLat1 = rad(lat1);
		double radLat2 = rad(lat2);
		double a = radLat1 - radLat2;
		double b = rad(lng1) - rad(lng2);
		double s = 2 * asin(sqrt(pow(sin(a / 2), 2) + cos(radLat1) * cos(radLat2) * pow(sin(b / 2), 2)));
		s = s * EARTH_RADIUS;
		s = round(s * 10000) / 10000;
		if (s > radius) 
		{
				return false;
		} 
		else 
		{
				return true;
		}
}


//设备插入拔出记录项目
int Vehicle_status_project(void)
{
		Vehicle_status	data;
		int ret;
		
		printf("welcome to [%s],hahaha\r\n",__func__);
		memset(&data,0,sizeof(data));
		memcpy(data.device_coding,Module_dcoding,strlen(Module_dcoding));	
		Get_current_time(data.create_time);
		data.type = 1;
//		memcpy(data.vin_sn,"tom_min",7);
		get_vehicle_vin(data.vin_sn);
	
		ret = Vehicle_status_pack_to_server(&data);
		
		printf("[%s] report over,hahaha\r\n",__func__);
		
		return ret;
}


//车辆启动登录项目
int Vehicle_login_project(void)
{
		Vehicle_login	data;
		int ret=0;
		
		printf("welcome to [%s],hahaha\r\n",__func__);
		memset(&data,0,sizeof(data));
		Get_current_time(data.create_time);
		memcpy(data.device_coding,Module_dcoding,strlen(Module_dcoding));	
		memcpy(data.mobile,"13651438144",11);
		get_vehicle_vin(data.vin_sn);
		Get_GPS_Data_Report(NULL);
		data.lng = Save_Data.longitude;
		data.lat = Save_Data.latitude;
	
		//点火时触发
		ret = Vehicle_login_pack_to_server(&data);
		
		printf("[%s] report over,hahaha\r\n",__func__);
	
		return ret;
}


//车辆熄火登出项目
void Vehicle_logout_project(void)
{
		Vehicle_logout data;
	
		printf("welcome to [%s],hahaha\r\n",__func__);
		memset(&data,0,sizeof(data));
		Get_current_time(data.create_time);
		memcpy(data.device_coding,Module_dcoding,strlen(Module_dcoding));
		get_vehicle_vin(data.vin_sn);	
		Get_GPS_Data_Report(NULL);
		data.lng = Save_Data.longitude;
		data.lat = Save_Data.latitude;
	
		Vehicle_logout_pack_to_server(&data);
		
		printf("[%s] report over,hahaha\r\n",__func__);		
}


//栅栏越位告警项目
void Fence_Warning_project(void)
{
		Fence_Warning data;
		cJSON *data_json;
		bool ret;
		
		printf("welcome to [%s],hahaha\r\n",__func__);
		memset(&data,0,sizeof(data));
		Get_current_time(data.alarnTime);
		memcpy(data.device_coding,Module_dcoding,strlen(Module_dcoding));
		Get_GPS_Data_Report(NULL);
		data.lng = Save_Data.longitude;
		data.lat = Save_Data.latitude;
	
		ret = isInCircle(FENCE_radius,FENCE_lat,FENCE_lng,data.lat,data.lng);
		if(ret) //未越界
		{
				data.alarnEvent = 1;//进入
		}
		else
		{
				data.alarnEvent = 0;//越出
		}

		data_json = Fence_Warning_pack_to_server(&data);
		UDP_Queue_In(UDP_data_queue,data_json);
		
		printf("[%s] report over,hahaha\r\n",__func__);
}



//////////////////////////////////////////////////////////////////////////////////
int get_vehiclediagnosies_status(int timeout)//   上层收户掉用 //return ==1 表示底层vehiclediagnosies数据 ok
{
	diag_flag = 1;
	while(timeout--)
	{
		vTaskDelay(1);
		if(diag_flag==0)
		{
			return 0;
		}
	}
	
	{
		diag_flag = 0;//修复车熄火后再次点火读不到数据的BUG
		return 1;
	}
}


int get_datacollection_status(int timeout)//上层收户掉用 //return ==1 表示底层datacollection数据 ok
{
	data_flag = 1;
	while(timeout--)
	{
		vTaskDelay(1);
		if(data_flag==0)
		{
			return 0;
		}
	}
	
	{
		data_flag = 0;//修复车熄火后再次点火读不到数据的BUG
		return 1;
	}

}



//车辆诊断项目
void Vehicle_diagnosis_project(void *para)
{
	Vehicle_diagnosis	data;
	cJSON *data_json;
	int Time_Cont = 0;	//这个值没初始化，细节细节

	printf("welcome to [%s] ,hahaha\r\n",__func__);
	memset(&data,0,sizeof(data));

	STM_EVAL_LEDOn(GREEN);
	if( get_vehiclediagnosies_status(20000))return ;
	STM_EVAL_LEDOff(GREEN);


	//printf("333diag_flag val: %d, data_flag val: %d\r\n",diag_flag,data_flag);

	printf("m_protocol_index : %d\r\n",m_protocol_index);

	get_Vehicle_diagnosis(&data);
	Get_current_time(data.create_time);
	//		data.agreement = 2;
	get_agreement(&data.agreement);
	memcpy(data.device_coding,Module_dcoding,strlen(Module_dcoding));
	//		get_vehicle_vin(data.vin_sn);
	//		get_mil_status(data.mil_status);
	//		get_vehicle_calid(data.software_identification_code);
	//		get_vehicle_cvn(data.cvn);
	//		printf("m_protocol_index : %d\r\n",m_protocol_index);
	//		pro_obdii_protocol_init(m_protocol_index);
	//		pro_read_dtc(m_protocol_index,&data.fault_total,data.fault_list);
	//		get_Vehicle_diagnosis(&data);
	Get_current_time(data.endTime);

	//printf("444diag_flag val: %d, data_flag val: %d\r\n",diag_flag,data_flag);

	//if(data.fault_total > 0)		//有故障码才上报
	{
	data_json = Vehicle_diagnosis_pack_to_server(&data);
	UDP_Queue_In(UDP_data_queue,data_json);
	}
	printf("[%s] report over,hahaha\r\n",__func__);
}


//数据采集项目
void Data_collection_project(void *para)
{
	Data_collection	data;
	cJSON *data_json;
	int Time_Cont = 0;	//这个值没初始化，细节细节

	printf("welcome to [%s],hahaha\r\n",__func__);
	memset(&data,0,sizeof(data));

	STM_EVAL_LEDOn(GREEN);
	if( get_vehiclediagnosies_status(20000))return ;
	STM_EVAL_LEDOff(GREEN);

	//printf("111diag_flag val: %d, data_flag val: %d\r\n",diag_flag,data_flag);

	{
	STM_EVAL_LEDOff(GREEN);
	get_datacollection(&data);
	Get_current_time(data.create_time);
	memcpy(data.device_coding,Module_dcoding,strlen(Module_dcoding));
	get_vehicle_vin(data.vin_sn);

	//printf("222diag_flag val: %d, data_flag val: %d\r\n",diag_flag,data_flag);
	//有故障码才上传数据采集
	data_json = Data_collection_pack_to_server(&data);
	UDP_Queue_In(UDP_data_queue,data_json);

	}

	vTaskDelay(100);
	printf("[%s] report over,hahaha\r\n",__func__);
}


//车辆GPS数据项目
//这个项目有问题，需优化
void Vehicle_GPS_project(void *para)
{
		Vehicle_GPS data;
		cJSON *data_json;
	
		printf("welcome to [%s],hahaha\r\n",__func__);

		memset(&data,0,sizeof(data));
		//优化下这个，有可能需要改成RTC
		Get_current_time(data.create_time);
		memcpy(data.device_coding,Module_dcoding,strlen(Module_dcoding));
		data.pos_status = 1;
		Get_GPS_Data_Report(&data);
//		parse_GPGGA(&data);					

		if(data.pos_status == 0)		//GPS数据有效
		{
//				parse_GPGGA(&data);	
				data_json = Vehicle_GPS_pack_to_server(&data);
				UDP_Queue_In(UDP_data_queue,data_json);
		}
		
//		vTaskDelay(100);
		printf("[%s] report over,hahaha\r\n",__func__);
}


//打开行程页数据刷新
void Driving_experience_project(void *para)
{
		Driving_experience data;
		cJSON *data_json;
	
		memset(&data,0,sizeof(data));
		Get_current_time(data.create_time);
		memcpy(data.device_coding,Module_dcoding,strlen(Module_dcoding));
		data.driveTimeLen = 5.55;
		Get_current_time(data.driveTime);
		data.driveMil = 7.77;
		data.driveOil = 8.88;
		
		data.urgenSpeedUp = 111;
		data.urgenSpeedDown = 112;
		data.urgenTurn = 121;
		data.isIdlingDriving = 122;
		data.isIdlingStop = 100;		
		
		Get_GPS_Data_Report(NULL);
		data.lng = Save_Data.longitude;
		data.lat = Save_Data.latitude;
	
		data_json = Driving_experience_pack_to_server(&data);
		UDP_Queue_In(UDP_data_queue,data_json);
}
	

//车身震动上报项目
void Vehicle_Vibration_project(void *para)
{
		Vehicle_Vibration data;
		cJSON *data_json;
	
		memset(&data,0,sizeof(data));
		Get_current_time(data.create_time);
		memcpy(data.device_coding,Module_dcoding,strlen(Module_dcoding));
		data.dataType = 1;
		memcpy(data.deviceData,"tom_min188",10);
	
	
		data_json = Vehicle_Vibration_pack_to_server(&data);
		UDP_Queue_In(UDP_data_queue,data_json);
}


static void Timer_project_switch(char mode)
{
		if(mode == 1)
		{
				xTimerStart(test1Timer_Handle,0);
				xTimerStart(test2Timer_Handle,0);
				xTimerStart(test3Timer_Handle,0);
				xTimerStart(test4Timer_Handle,0);
		}
		else if(mode == 0)
		{
				xTimerStop(test1Timer_Handle,0);
				xTimerStop(test2Timer_Handle,0);
				xTimerStop(test3Timer_Handle,0);
				xTimerStop(test4Timer_Handle,0);
		}
}


void udp_server_cmd_action(int action)
{						
		printf("welcome to [%s],hahahha\r\n",__func__);
	
		Timer_project_switch(0);
	
		if(action == 1)					//车辆体检
		{
				printf("Vehicle medical examination is starting, hahaha\r\n");
				Data_collection_project(NULL);
				printf("Vehicle medical examination is over, hahaha\r\n");
		}
		else if(action == 2)		//客服聊天实时状态
		{			
				printf("Customer service chat real-time status is starting, hahaha\r\n");
				Data_collection_project(NULL);
				printf("Customer service chat real-time status is over, hahaha\r\n");
				vTaskDelay(5000);//调度调度，不然会卡			
		}
		else if(action == 3)		//实时监控
		{
				printf("real time monitoring is starting, hahaha\r\n");
				Vehicle_GPS_project(NULL);
				printf("real time monitoring is over, hahaha\r\n");
				vTaskDelay(2000);//调度调度，不然会卡
		}
		else if(action == 4)		//车辆诊断
		{
				printf("Vehicle diagnosis is starting, hahaha\r\n");
				Vehicle_diagnosis_project(NULL);
				printf("Vehicle diagnosis is over, hahaha\r\n");
		}
		else if(action == 5)		//打开行程页数据刷新
		{
				printf("Driving experience is starting, hahaha\r\n");
				Driving_experience_project(NULL);
				printf("Driving experience is over, hahaha\r\n");
				vTaskDelay(10000);//调度调度，不然会卡
		}
		else if(action == 6)		//故障码清除
		{
				printf("Fault code clear is starting, hahaha\r\n");
				
				printf("Fault code clear is over, hahaha\r\n");
				vTaskDelay(10);//调度调度，不然会卡
		}
		//这个命令带个参数，需要单独解析
		else if(action == 7)		//终止正在执行的命令
		{
				printf("Terminate the task being executed is starting, hahaha\r\n");
				printf("Terminate the task being executed is over, hahaha\r\n");
				vTaskDelay(10);//调度调度，不然会卡
		}
		
		if(action == 1 || action == 4 || action == 6 || action == 7)
		{
				Timer_project_switch(1);
				server_cmd = 0;
		}
}


//得到加密属性
void get_OBD_Prop(char *src_buf,char *prop_buf)
{
		char pack_buf[33] = {0};
		char tmp_buf[64] = {0};

		sprintf(pack_buf,"%s%s","Y6sL2EoLVWTo",src_buf);
		watch_startMD5(pack_buf, tmp_buf, strlen(pack_buf));
		printf("the first MD5 string : %s\r\n",tmp_buf);
		memset(pack_buf,0,33);
		watch_startMD5(tmp_buf, pack_buf,strlen(tmp_buf));
		printf("the second MD5 string : %s\r\n",pack_buf);
		memset(tmp_buf,0,64);
		sprintf(tmp_buf,"%s%s","##",pack_buf);
		memcpy(prop_buf,tmp_buf+9,8);
		
		printf("OBD property string: %s\r\n",prop_buf);
}



//6个小时重新获取一次,点火也需要获取一次,因为插入时获取了一次，可能时间太久，所以点火也需要获取一次


//在打包任务中打包udp数据
void udp_data_pack(void)
{
		//扫描服务器发送的特殊命令
		if(server_cmd != 0)
		{
				udp_server_cmd_action(server_cmd);
		}	
		//定时GPS数据
		if(GPS_timer_flag == 1)
		{
				get_task_info(data_pack_task);
				get_task_info(data_transfer_task);
				xTimerStop(test1Timer_Handle,0);
				GPS_timer_flag = 0;
				Vehicle_GPS_project(NULL);
				xTimerStart(test1Timer_Handle,0);
		}
//		//定时栅栏告警
//		if(Fence_Warning_timer >= 10)
//		{
//				xTimerStop(test4Timer_Handle,0);
//				printf("Fence_Warning_timer: %d\r\n",Fence_Warning_timer);
//				Fence_Warning_timer = 0;
////				Fence_Warning_project();
//				Data_collection_project(NULL);
//				xTimerStart(test4Timer_Handle,0);
//		}
//		//定时车辆诊断
		if(Vehicle_diagnosis_timer >= 10)
		{
				xTimerStop(test2Timer_Handle,0);
				printf("Vehicle_diagnosis_timer: %d\r\n",Vehicle_diagnosis_timer);
				Vehicle_diagnosis_timer = 0;
				Vehicle_diagnosis_project(NULL);
				xTimerStart(test2Timer_Handle,0);
		}
		//定时数据采集
		//单独产生的BUG,计时的变量未初始化
		if(Data_collection_timer >= 5)
		{
				xTimerStop(test3Timer_Handle,0);
				printf("Data_collection_timer: %d\r\n",Data_collection_timer);
				Data_collection_timer = 0;
				Data_collection_project(NULL);
				xTimerStart(test3Timer_Handle,0);
		}
}


//数据打包函数
void fcar_data_pack(void)
{
	int ret ;
	static int login_status=0;
	static int token_status=0;

	if(OBD_STATUS == 1)
	{
		//OBD盒子插入
		ret = Vehicle_status_project();//http
		if(ret == 0)
		{
			printf("OBD insert,acquire user passwprd failed\r\n");
			return;
		}
		else
		{
			OBD_STATUS  = 0;
			printf("OBD insert success \r\n ");
		}
	}
	if((OBD_STATUS == 00)&&(token_status==0))
	{
		//获取访问Token
		ret = get_obd_token();
		if(ret == 0)
		{
			printf("acquire Token failed\r\n ");
			return;
		}
		else if(ret==1)
		{
			token_status=1;
			login_status = 1;
			printf("acquire Token successful\r\n ");	
		}
		else if(ret==2)
		{
			STM_EVAL_LED_ALL_FIFO(500,10);  
			printf("Device No unauthorized \r\n");
			printf("Device ReSet \r\n");
			__set_FAULTMASK(1);//关闭总中断
			NVIC_SystemReset();//请求单片机重启
		}
	}
#ifdef BOOT_APPLACATION
	Vehicle_fire=1;	
#endif
	//还差个点火信号，差个标志位，等待OBD那边置
	if(login_status == 1 && Vehicle_fire == 1)
	{
		Vehicle_fire = 0;
		login_status=0;
		//车辆启动登录，需要点火信号
		ret = Vehicle_login_project();
		if(ret == 0)
		{
			printf("Vehicle failed to login \r\n");
			//登陆失败就重启
			while(1)
			{
				printf("Device ReSet \r\n");
				__set_FAULTMASK(1);//关闭总中断
				NVIC_SystemReset();//请求单片机重启
			}
			return;
		}
		else
		{
			printf("Vehicle success to login \r\n");
		}
	}
	else
	{
	     return;
	}
	#if 1
 			
#ifdef BOOT_APPLACATION
		vTaskSuspend(data_transfer_task); 
		vTaskSuspend(Task_ObdiiCmdProcess_Proc_handle);
		STM_EVAL_LEDOff(RED); 
		STM_EVAL_LEDOff(GREEN); 
		STM_EVAL_LEDOn(BLUE); 
		if(Download_File() == 0)	//GET请求的参数与请求头有关
		{
			printf("\r\n[%s] Download_File success...\r\n",__func__);
			ret = UserAppUpdateViaBinFileAndCheck();
			if((ret == PASSED) || (ret == NEWEST))
			{
					printf("[%s] The current bin file upgrade is successful and jump to app.\r\n",__func__);
					appmain_jmp();
					//跳转失败可以重启
					__set_FAULTMASK(1);//关闭总中断
					NVIC_SystemReset();//请求单片机重启
			}
			else
			{
					printf("[%s] Download bin file is not completed or the file name is wrong\r\n",__func__);
					printf("\r\n[%s] tom_min User run in Boot...\r\n",__func__);
					printf("\r\nBoot version = %s\r\n",BOOT_VERSION);	
			}
		}
		else	//当前车型文件为最新，或者网络请求失败，下载文件失败等都会跳转到app，跳转不到就会自动重启
		{
			printf("[%s] Current bin file does not require an upgrade, or Failed during network request\r\n",__func__);
			appmain_jmp();
		}						

		vTaskResume(Task_ObdiiCmdProcess_Proc_handle); 
		vTaskResume(data_transfer_task); 				
#endif				
#endif
	Timer_project_switch(1);
	while(1)
	{														
		udp_data_pack();
		vTaskDelay(10);		
	}
			
}


//数据传输函数
void udp_send_data(void)
{							
		pudp_node tom_node;
		char *pri_json;

		if(!EmptyQueue(UDP_data_queue)) //如果队列不为空
		{						
				printf("UDP_Queue out start...\r\n");
				UDP_Queue_Out(UDP_data_queue,&tom_node);

				tom_UDP_Client(tom_node->data_json);
				free(tom_node);
				printf("UDP_Queue out over...\r\n");
		}
		else
		{
				//printf("UDP_Queue is empty...\r\n");		
		}
}

