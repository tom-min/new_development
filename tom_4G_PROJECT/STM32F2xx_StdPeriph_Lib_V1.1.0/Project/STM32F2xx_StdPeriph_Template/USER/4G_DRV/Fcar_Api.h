#ifndef __FCAR_API_H
#define __FCAR_API_H



//��������γ���¼
typedef struct{
		char create_time[30];		//������ʱ����
		char device_coding[20];
		char type;							//1:���룬2���γ�
		char vin_sn[20];
}Vehicle_status;


//���������¼
typedef struct{
		char create_time[30];		//������ʱ����
		char device_coding[20];
		char mobile[20];				//SIM������
		int  serial_number;			//��¼��ˮ��
		char vin_sn[20];
		double lng;
		double lat;
}Vehicle_login;


//������ϼ�¼
typedef struct{
		char create_time[30];		//������ʱ����
		char endTime[30];				//����ʱ��
		char device_coding[20];
		char agreement;					//��Ч��Χ0~2,"0"����IOS15765,"1"����IOS27145,"2"����SAEJ1939,"-1"��ʾ��Ч
		char mil_status;				//��Ч��Χ0~1,"0"����Ϊδ����,"1"�������,"2"����SAEJ1939,"-1"��ʾ��Ч
		char status_ready[20];	//json�ַ���
		char vin_sn[20];				//����ʱ������Ψһʶ����
		int  fault_total;				//��Ч��Χ:0~253��"-1"��ʾ��Ч��
		char fault_list[256];		//ÿ��������Ϊ4���ֽ�,�ɰ�����ʵ��˳���������
		char sys_list[128];			//ÿ��������Ϊ4���ֽ�,�ɰ�����ʵ��˳���������
}Vehicle_diagnosis;


//���ݲɼ�
typedef struct{
		char create_time[30];		//������ʱ����,�ɼ�ʱ��
		char device_coding[20];	//�豸����
		double kilometres;			//������
		int  speed;							//���٣�KM/H
		double max_torque;			//����������׼Ť�أ�N*m
		double out_torque;			//�����������Ť��,%
		int friction_torque;		//Ħ��Ť��,%
		int engine_speed;				//������ת��
		double engine_flow;			//������ȼ������
		double put_temperature;	//SCR����¶�
		double out_temperature;	//SCR�����¶�
		double pressure_diff;		//DPFѹ��
		double intake_volume;		//������
		double reactant_surplus;//��Ӧ������
		double tank_level;			//����Һλ,mm
		double coolant_temperature;	//��������ȴҺ�¶�
		double fuel_injection_quantity;	//������,mm
		double  voltage;						//��ƿ��ѹ,0~1000
		double engine_start;		//����������ʱ��
		char fault_code[40];		//������,0:�������������������Ϊ������
		char vin_sn[20];
		char frontOCurrent[6*16];		
		char frontOVoltage[6*16];		
		char backOCurrent[6*16];		
		char backOVoltage[6*16];			
		double engineLoad;			//����������
		double airInPressure;		//�������ѹ��
		double airInTemperature;//�����¶�
		double injectionPwm;		//��������
		double pressure;				//����ѹ��
		double airGasScale;			//��ȼ��
		double fireVol;					//����ѹ
		char software_identification_code[30];	//����궨ʶ�����������ҵ�Զ��壬��ĸ��������ϣ�������油�ַ�"0"
		char cvn[20];						//�궨��֤����������ҵ�Զ��壬��ĸ��������ϣ�������油�ַ�"0"
		char iupr[20];
}Data_collection;


//�ⲿ�������ӿڱ���ʱ����
typedef struct{
		int gsensor_x;
		int gsensor_y;
		int gsensor_z;
}Sensor_interface;


//�����ǳ�
typedef struct{
		char create_time[30];		//������ʱ����
		char device_coding[20];
		char vin_sn[20];
		double lng;
		double lat;
}Vehicle_logout;


//�ն�Уʱ����ʱ����
typedef struct{
		char create_time[30];		//������ʱ����
		char device_coding[20];
}Terminal_timing;


//����GPS
typedef struct{
		char create_time[30];		//
		char device_coding[20];
		int  pos_status;				
		double lng;
		double lat;
		double speed;
		double angle;
		int  pos_type;					//��λ����
		char GPS_Precision[10];	//����
}Vehicle_GPS;


//����������
typedef struct{
		char create_time[30];		//
		char device_coding[20];
		int  dataType;
		char deviceData[200];
		double lng;
		double lat;
}Vehicle_Vibration;


//��ʻ�г�����
typedef struct{
		char create_time[30];		
		char device_coding[20];
		double driveTimeLen;
		char driveTime[20];
		double driveMil;
		double driveOil;
		char urgenSpeedUp;
		char urgenSpeedDown;
		char urgenTurn;
		char isIdlingDriving;
		char isIdlingStop;
		double lng;
		double lat;
}Driving_experience;


//դ��Խλ�澯
typedef struct{
		char alarnTime[30];		//
		char device_coding[20];
		double lng;
		double lat;
		int alarnEvent;		//�澯�¼���0-Խ����1-����
}Fence_Warning;


//������Ϣ
typedef struct{
		int vehicle_model;					//����
		int child_vehicle_model;		//�ӳ���
		int vehicle_version;				//�汾��
		char vehicle_file_name[64];	//�����ļ���
		
		char vehicle_file_url[200];	//�����ļ���������
}Vehicle_Model_Info;


typedef enum{
		JSON_COMMON,
		JSON_ADDCARIO,
		JSON_TOKEN,
		JSON_SERVER_TIME,
		JSON_FENCE
}JSON_PARSE_TYPE;


extern void Fence_Warning_project(void);
extern void Vehicle_diagnosis_project(void *para);
extern void Data_collection_project(void *para);
extern void Vehicle_GPS_project(void *para);
extern void Vehicle_Vibration_project(void *para);
extern void udp_server_cmd_action(int action);

extern void get_OBD_Prop(char *src_buf,char *prop_buf);

extern void fcar_data_pack(void);
extern void udp_send_data(void);
#endif

