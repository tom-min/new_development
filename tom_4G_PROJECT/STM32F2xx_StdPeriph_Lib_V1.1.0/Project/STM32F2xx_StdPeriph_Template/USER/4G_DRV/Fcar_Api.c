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
int Vehicle_fire;	//0:Ĭ��״̬��1:���״̬��2:Ϩ��״̬

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
extern _SaveData Save_Data;//������Ŀ�ľ�γ��
extern Vehicle_Model_Info tom_vehicle_model_data;


pudp_queue	UDP_data_queue;


//6��HTTP����URL
#ifdef URL_SW
const char URL_inOut[] = "http://carnet-t.szfcar.com/data/cario/addcario";		//�豸������
const char OBD_Token[] = "http://carnet-t.szfcar.com/uaa/oauth/token"; 				//��ȡtoken
const char URL_login[] = "http://carnet-t.szfcar.com/data/carlogin/addlogin"; //������¼�ӿ�
const char URL_logout[] = "http://carnet-t.szfcar.com/data/carlogout/addCarlogoutRec"; 	//�����ǳ��ӿ�
const char Get_Server_Time[] = "http://carnet-t.szfcar.com/data/devicetime/getServerTime"; 	//�ն�Уʱ
const char Get_FENCE[] = "http://carnet-t.szfcar.com/data/carnetmap/getfence"; 	//��ȡԲ��դ��
const char Vehicle_Vibra[] = "http://carnet-t.szfcar.com/data/carshake/upshake"; 	//�������ϱ�

#else

const char URL_inOut[] = "http://14.21.40.133:8008/data/cario/addcario";		//�豸������
const char OBD_Token[] = "http://14.21.40.133:8008/uaa/oauth/token"; 				//��ȡtoken
const char URL_login[] = "http://14.21.40.133:8008/data/carlogin/addlogin"; //������¼�ӿ�
const char URL_logout[] = "http://14.21.40.133:8008/data/carlogout/addCarlogoutRec"; 	//�����ǳ��ӿ�
const char Get_Server_Time[] = "http://14.21.40.133:8008/data/devicetime/getServerTime"; 	//�ն�Уʱ
const char Get_FENCE[] = "http://14.21.40.133:8008/data/carnetmap/getfence"; 	//��ȡԲ��դ��
const char Vehicle_Vibra[] = "http://14.21.40.133:8008/data/carshake/upshake"; 	//�������ϱ�
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


//�ն�Уʱ
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


//����OBD����γ���¼
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
		
		//��ȡ����ʧ�����Ի���
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


//����������¼
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
		//��¼ʧ�����Ի���
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


//����Ϩ��ǳ�
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

		printf("[%s] context string: %s, len: %d\r\n",__func__,pack_buf,strlen(pack_buf));//���֮����ַ�������512bytes��ϸ��
		
		post_head_switch(1);
		ret = Post_Request_pack((char *)URL_logout,pack_buf,3,JSON_COMMON);
		if(ret == 0)
		{
				printf("%s failed\r\n",__func__);
		}
		post_head_switch(0);
		
		return ret;
}


//��ȡԲ��դ��
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


//��ȡToken
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
		//��ȡtokenʧ�����Ի���
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


//����������ݷ��͵�������
static cJSON* Vehicle_diagnosis_pack_to_server(Vehicle_diagnosis *buf)
{
		cJSON *data_json,*pack_json,*status_ready_json;
		char *print_json;
		char tmp_buf[64] = {0};

		pack_json = cJSON_CreateObject();
		if(pack_json)
		{
				//msgType�����ϴ�����
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
//						cJSON_AddItemToObject(data_json,"status_ready",status_ready_json);//����ת��json�ַ�������
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


//���ݲɼ����͵�������
static cJSON* Data_collection_pack_to_server(Data_collection *buf)
{
		cJSON *data_json,*pack_json,*iupr_json;
		char *print_json;
		char tmp_buf[64] = {0};

		pack_json = cJSON_CreateObject();
		if(pack_json)
		{
				//msgType�����ϴ�����
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


//����GPS���ݷ��͵�������
static cJSON* Vehicle_GPS_pack_to_server(Vehicle_GPS *buf)
{
		cJSON *data_json,*pack_json;
		char *print_json;
		char tmp_buf[64] = {0};
		
		printf("welcome to [%s], hahaha\r\n",__func__);

		pack_json = cJSON_CreateObject();
		if(pack_json)
		{
				//msgType�����ϴ�����
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


//��ʻ�г����ݷ��͵�������
static cJSON* Driving_experience_pack_to_server(Driving_experience *buf)
{
		cJSON *data_json,*pack_json;
		char *print_json;
		char tmp_buf[32] = {0};
		
		printf("welcome to [%s], hahaha\r\n",__func__);

		pack_json = cJSON_CreateObject();
		if(pack_json)
		{
				//msgType�����ϴ�����
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


//դ��Խλ�澯���͵�������
static cJSON* Fence_Warning_pack_to_server(Fence_Warning *buf)
{
		cJSON *data_json,*pack_json;
		char *print_json;
		char tmp_buf[32] = {0};
		
		printf("welcome to [%s], hahaha\r\n",__func__);

		pack_json = cJSON_CreateObject();
		if(pack_json)
		{
				//msgType�����ϴ�����
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


//�������ϱ�,����UDP�ϴ�
static cJSON* Vehicle_Vibration_pack_to_server(Vehicle_Vibration *buf)
{
		cJSON *data_json,*pack_json;
		char *print_json;
		char tmp_buf[32] = {0};
		
		printf("welcome to [%s], hahaha\r\n",__func__);

		pack_json = cJSON_CreateObject();
		if(pack_json)
		{
				//msgType�����ϴ�����
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


//Χ������
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


//�豸����γ���¼��Ŀ
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


//����������¼��Ŀ
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
	
		//���ʱ����
		ret = Vehicle_login_pack_to_server(&data);
		
		printf("[%s] report over,hahaha\r\n",__func__);
	
		return ret;
}


//����Ϩ��ǳ���Ŀ
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


//դ��Խλ�澯��Ŀ
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
		if(ret) //δԽ��
		{
				data.alarnEvent = 1;//����
		}
		else
		{
				data.alarnEvent = 0;//Խ��
		}

		data_json = Fence_Warning_pack_to_server(&data);
		UDP_Queue_In(UDP_data_queue,data_json);
		
		printf("[%s] report over,hahaha\r\n",__func__);
}



//////////////////////////////////////////////////////////////////////////////////
int get_vehiclediagnosies_status(int timeout)//   �ϲ��ջ����� //return ==1 ��ʾ�ײ�vehiclediagnosies���� ok
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
		diag_flag = 0;//�޸���Ϩ����ٴε����������ݵ�BUG
		return 1;
	}
}


int get_datacollection_status(int timeout)//�ϲ��ջ����� //return ==1 ��ʾ�ײ�datacollection���� ok
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
		data_flag = 0;//�޸���Ϩ����ٴε����������ݵ�BUG
		return 1;
	}

}



//���������Ŀ
void Vehicle_diagnosis_project(void *para)
{
	Vehicle_diagnosis	data;
	cJSON *data_json;
	int Time_Cont = 0;	//���ֵû��ʼ����ϸ��ϸ��

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

	//if(data.fault_total > 0)		//�й�������ϱ�
	{
	data_json = Vehicle_diagnosis_pack_to_server(&data);
	UDP_Queue_In(UDP_data_queue,data_json);
	}
	printf("[%s] report over,hahaha\r\n",__func__);
}


//���ݲɼ���Ŀ
void Data_collection_project(void *para)
{
	Data_collection	data;
	cJSON *data_json;
	int Time_Cont = 0;	//���ֵû��ʼ����ϸ��ϸ��

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
	//�й�������ϴ����ݲɼ�
	data_json = Data_collection_pack_to_server(&data);
	UDP_Queue_In(UDP_data_queue,data_json);

	}

	vTaskDelay(100);
	printf("[%s] report over,hahaha\r\n",__func__);
}


//����GPS������Ŀ
//�����Ŀ�����⣬���Ż�
void Vehicle_GPS_project(void *para)
{
		Vehicle_GPS data;
		cJSON *data_json;
	
		printf("welcome to [%s],hahaha\r\n",__func__);

		memset(&data,0,sizeof(data));
		//�Ż���������п�����Ҫ�ĳ�RTC
		Get_current_time(data.create_time);
		memcpy(data.device_coding,Module_dcoding,strlen(Module_dcoding));
		data.pos_status = 1;
		Get_GPS_Data_Report(&data);
//		parse_GPGGA(&data);					

		if(data.pos_status == 0)		//GPS������Ч
		{
//				parse_GPGGA(&data);	
				data_json = Vehicle_GPS_pack_to_server(&data);
				UDP_Queue_In(UDP_data_queue,data_json);
		}
		
//		vTaskDelay(100);
		printf("[%s] report over,hahaha\r\n",__func__);
}


//���г�ҳ����ˢ��
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
	

//�������ϱ���Ŀ
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
	
		if(action == 1)					//�������
		{
				printf("Vehicle medical examination is starting, hahaha\r\n");
				Data_collection_project(NULL);
				printf("Vehicle medical examination is over, hahaha\r\n");
		}
		else if(action == 2)		//�ͷ�����ʵʱ״̬
		{			
				printf("Customer service chat real-time status is starting, hahaha\r\n");
				Data_collection_project(NULL);
				printf("Customer service chat real-time status is over, hahaha\r\n");
				vTaskDelay(5000);//���ȵ��ȣ���Ȼ�Ῠ			
		}
		else if(action == 3)		//ʵʱ���
		{
				printf("real time monitoring is starting, hahaha\r\n");
				Vehicle_GPS_project(NULL);
				printf("real time monitoring is over, hahaha\r\n");
				vTaskDelay(2000);//���ȵ��ȣ���Ȼ�Ῠ
		}
		else if(action == 4)		//�������
		{
				printf("Vehicle diagnosis is starting, hahaha\r\n");
				Vehicle_diagnosis_project(NULL);
				printf("Vehicle diagnosis is over, hahaha\r\n");
		}
		else if(action == 5)		//���г�ҳ����ˢ��
		{
				printf("Driving experience is starting, hahaha\r\n");
				Driving_experience_project(NULL);
				printf("Driving experience is over, hahaha\r\n");
				vTaskDelay(10000);//���ȵ��ȣ���Ȼ�Ῠ
		}
		else if(action == 6)		//���������
		{
				printf("Fault code clear is starting, hahaha\r\n");
				
				printf("Fault code clear is over, hahaha\r\n");
				vTaskDelay(10);//���ȵ��ȣ���Ȼ�Ῠ
		}
		//������������������Ҫ��������
		else if(action == 7)		//��ֹ����ִ�е�����
		{
				printf("Terminate the task being executed is starting, hahaha\r\n");
				printf("Terminate the task being executed is over, hahaha\r\n");
				vTaskDelay(10);//���ȵ��ȣ���Ȼ�Ῠ
		}
		
		if(action == 1 || action == 4 || action == 6 || action == 7)
		{
				Timer_project_switch(1);
				server_cmd = 0;
		}
}


//�õ���������
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



//6��Сʱ���»�ȡһ��,���Ҳ��Ҫ��ȡһ��,��Ϊ����ʱ��ȡ��һ�Σ�����ʱ��̫�ã����Ե��Ҳ��Ҫ��ȡһ��


//�ڴ�������д��udp����
void udp_data_pack(void)
{
		//ɨ����������͵���������
		if(server_cmd != 0)
		{
				udp_server_cmd_action(server_cmd);
		}	
		//��ʱGPS����
		if(GPS_timer_flag == 1)
		{
				get_task_info(data_pack_task);
				get_task_info(data_transfer_task);
				xTimerStop(test1Timer_Handle,0);
				GPS_timer_flag = 0;
				Vehicle_GPS_project(NULL);
				xTimerStart(test1Timer_Handle,0);
		}
//		//��ʱդ���澯
//		if(Fence_Warning_timer >= 10)
//		{
//				xTimerStop(test4Timer_Handle,0);
//				printf("Fence_Warning_timer: %d\r\n",Fence_Warning_timer);
//				Fence_Warning_timer = 0;
////				Fence_Warning_project();
//				Data_collection_project(NULL);
//				xTimerStart(test4Timer_Handle,0);
//		}
//		//��ʱ�������
		if(Vehicle_diagnosis_timer >= 10)
		{
				xTimerStop(test2Timer_Handle,0);
				printf("Vehicle_diagnosis_timer: %d\r\n",Vehicle_diagnosis_timer);
				Vehicle_diagnosis_timer = 0;
				Vehicle_diagnosis_project(NULL);
				xTimerStart(test2Timer_Handle,0);
		}
		//��ʱ���ݲɼ�
		//����������BUG,��ʱ�ı���δ��ʼ��
		if(Data_collection_timer >= 5)
		{
				xTimerStop(test3Timer_Handle,0);
				printf("Data_collection_timer: %d\r\n",Data_collection_timer);
				Data_collection_timer = 0;
				Data_collection_project(NULL);
				xTimerStart(test3Timer_Handle,0);
		}
}


//���ݴ������
void fcar_data_pack(void)
{
	int ret ;
	static int login_status=0;
	static int token_status=0;

	if(OBD_STATUS == 1)
	{
		//OBD���Ӳ���
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
		//��ȡ����Token
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
			__set_FAULTMASK(1);//�ر����ж�
			NVIC_SystemReset();//����Ƭ������
		}
	}
#ifdef BOOT_APPLACATION
	Vehicle_fire=1;	
#endif
	//���������źţ������־λ���ȴ�OBD�Ǳ���
	if(login_status == 1 && Vehicle_fire == 1)
	{
		Vehicle_fire = 0;
		login_status=0;
		//����������¼����Ҫ����ź�
		ret = Vehicle_login_project();
		if(ret == 0)
		{
			printf("Vehicle failed to login \r\n");
			//��½ʧ�ܾ�����
			while(1)
			{
				printf("Device ReSet \r\n");
				__set_FAULTMASK(1);//�ر����ж�
				NVIC_SystemReset();//����Ƭ������
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
		if(Download_File() == 0)	//GET����Ĳ���������ͷ�й�
		{
			printf("\r\n[%s] Download_File success...\r\n",__func__);
			ret = UserAppUpdateViaBinFileAndCheck();
			if((ret == PASSED) || (ret == NEWEST))
			{
					printf("[%s] The current bin file upgrade is successful and jump to app.\r\n",__func__);
					appmain_jmp();
					//��תʧ�ܿ�������
					__set_FAULTMASK(1);//�ر����ж�
					NVIC_SystemReset();//����Ƭ������
			}
			else
			{
					printf("[%s] Download bin file is not completed or the file name is wrong\r\n",__func__);
					printf("\r\n[%s] tom_min User run in Boot...\r\n",__func__);
					printf("\r\nBoot version = %s\r\n",BOOT_VERSION);	
			}
		}
		else	//��ǰ�����ļ�Ϊ���£�������������ʧ�ܣ������ļ�ʧ�ܵȶ�����ת��app����ת�����ͻ��Զ�����
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


//���ݴ��亯��
void udp_send_data(void)
{							
		pudp_node tom_node;
		char *pri_json;

		if(!EmptyQueue(UDP_data_queue)) //������в�Ϊ��
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

