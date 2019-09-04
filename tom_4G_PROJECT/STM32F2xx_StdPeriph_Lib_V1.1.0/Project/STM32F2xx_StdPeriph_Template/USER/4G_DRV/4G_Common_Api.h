#ifndef _2G_COMMON_API_
#define _2G_COMMON_API_

#include "Fcar_Api.h"
#include "cJSON.h"
#include "FreeRTOS.h"
#include "task.h"


#define false 0
#define true 1

//#define UDP_SERVER				"47.98.39.139"			//IP
//#define UDP_SERVER				"carnet-t.szfcar.com"	//����
#define UDP_SERVER				"14.21.40.133"	//
#define UDP_SERVER_PORT		"10022"

//�������鳤��
#define GPS_Buffer_Length 	256
#define UTCTime_Length 			11
#define latitude_Length 		11
#define N_S_Length 					2
#define longitude_Length 		12
#define E_W_Length 					2 

typedef struct SaveData 
{
	char GPS_Buffer[GPS_Buffer_Length];
	char isGetData;										//�Ƿ��ȡ��GPS����
	char isParseData;									//�Ƿ�������
	char UTCTime[UTCTime_Length];			//UTCʱ��
//	char latitude[latitude_Length];		//γ��
	double latitude;
	char N_S[N_S_Length];							//N/S
//	char longitude[longitude_Length];	//����
	double longitude;
	char E_W[E_W_Length];							//E/W
	char isUsefull;										//��λ��Ϣ�Ƿ���Ч
	double speed;											//�ٶ�
	double angle;											//��λ��
} _SaveData;												


typedef struct time_style{
 	unsigned short year;	
	unsigned char  month;	
	unsigned char date;
	unsigned char hour;
	unsigned char min; 	
	unsigned char sec;
}t_style;


extern void get_task_info(TaskHandle_t task);

extern void tom_GPRS(char *TCPServer,char *Port,char *gps_data);


extern void Get_current_time(char *time_buf);

extern int Post_Request_pack(char *url,char *context,unsigned char Retry,JSON_PARSE_TYPE parse_type);
extern void Post_Common_init(void);

extern void close_gprs(void);
extern void get_IMEI(char *device_coding);

extern void Get_GPS_Data_Report(Vehicle_GPS	*data);
extern void parse_GPGGA(Vehicle_GPS *data_gps);

extern void UDP_Client_init(char *UDPServer,char *Port);
extern void tom_UDP_Client(cJSON *json_str);

extern void post_head_switch(char mode);

extern int download_Vehicle_file(char *file_path);


#endif

