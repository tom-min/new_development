#ifndef __FCAR_API_H
#define __FCAR_API_H



//车辆插入拔出记录
typedef struct{
		char create_time[30];		//年月日时分秒
		char device_coding[20];
		char type;							//1:插入，2：拔出
		char vin_sn[20];
}Vehicle_status;


//车辆登入记录
typedef struct{
		char create_time[30];		//年月日时分秒
		char device_coding[20];
		char mobile[20];				//SIM卡号码
		int  serial_number;			//登录流水号
		char vin_sn[20];
		double lng;
		double lat;
}Vehicle_login;


//车辆诊断记录
typedef struct{
		char create_time[30];		//年月日时分秒
		char endTime[30];				//结束时间
		char device_coding[20];
		char agreement;					//有效范围0~2,"0"代表IOS15765,"1"代表IOS27145,"2"代表SAEJ1939,"-1"表示无效
		char mil_status;				//有效范围0~1,"0"代表为未点亮,"1"代表点亮,"2"代表SAEJ1939,"-1"表示无效
		char status_ready[20];	//json字符串
		char vin_sn[20];				//登入时车辆的唯一识别码
		int  fault_total;				//有效范围:0~253，"-1"表示无效。
		char fault_list[256];		//每个故障码为4个字节,可按故障实际顺序进行排序。
		char sys_list[128];			//每个故障码为4个字节,可按故障实际顺序进行排序。
}Vehicle_diagnosis;


//数据采集
typedef struct{
		char create_time[30];		//年月日时分秒,采集时间
		char device_coding[20];	//设备编码
		double kilometres;			//公里数
		int  speed;							//车速，KM/H
		double max_torque;			//发动机最大基准扭矩，N*m
		double out_torque;			//发动机净输出扭矩,%
		int friction_torque;		//摩擦扭矩,%
		int engine_speed;				//发动机转速
		double engine_flow;			//发动机燃料流量
		double put_temperature;	//SCR入口温度
		double out_temperature;	//SCR出口温度
		double pressure_diff;		//DPF压差
		double intake_volume;		//进气量
		double reactant_surplus;//反应剂余量
		double tank_level;			//油箱液位,mm
		double coolant_temperature;	//发动机冷却液温度
		double fuel_injection_quantity;	//喷油量,mm
		double  voltage;						//电瓶电压,0~1000
		double engine_start;		//发动机启动时间
		char fault_code[40];		//故障码,0:代表正常，其他情况视为故障码
		char vin_sn[20];
		char frontOCurrent[6*16];		
		char frontOVoltage[6*16];		
		char backOCurrent[6*16];		
		char backOVoltage[6*16];			
		double engineLoad;			//发动机负载
		double airInPressure;		//进气歧管压力
		double airInTemperature;//进气温度
		double injectionPwm;		//喷油脉宽
		double pressure;				//大气压力
		double airGasScale;			//空燃比
		double fireVol;					//点火电压
		char software_identification_code[30];	//软件标定识别号由生产企业自定义，字母或数字组合，不足后面补字符"0"
		char cvn[20];						//标定验证码由生产企业自定义，字母或数字组合，不足后面补字符"0"
		char iupr[20];
}Data_collection;


//外部传感器接口表，暂时保留
typedef struct{
		int gsensor_x;
		int gsensor_y;
		int gsensor_z;
}Sensor_interface;


//车辆登出
typedef struct{
		char create_time[30];		//年月日时分秒
		char device_coding[20];
		char vin_sn[20];
		double lng;
		double lat;
}Vehicle_logout;


//终端校时，暂时保留
typedef struct{
		char create_time[30];		//年月日时分秒
		char device_coding[20];
}Terminal_timing;


//车辆GPS
typedef struct{
		char create_time[30];		//
		char device_coding[20];
		int  pos_status;				
		double lng;
		double lat;
		double speed;
		double angle;
		int  pos_type;					//定位类型
		char GPS_Precision[10];	//精度
}Vehicle_GPS;


//车身震动数据
typedef struct{
		char create_time[30];		//
		char device_coding[20];
		int  dataType;
		char deviceData[200];
		double lng;
		double lat;
}Vehicle_Vibration;


//驾驶行程数据
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


//栅栏越位告警
typedef struct{
		char alarnTime[30];		//
		char device_coding[20];
		double lng;
		double lat;
		int alarnEvent;		//告警事件，0-越出，1-进入
}Fence_Warning;


//车型信息
typedef struct{
		int vehicle_model;					//车型
		int child_vehicle_model;		//子车型
		int vehicle_version;				//版本号
		char vehicle_file_name[64];	//车型文件名
		
		char vehicle_file_url[200];	//车型文件下载链接
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

