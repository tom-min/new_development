#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cmdline.h"
#include "main.h"
#include "user_stm32f2xx.h"
#include "user_queue.h"
#include "user_config.h"
#include "config.h"
#include "ff.h" 
#include "stdio.h" 
#include "FreeRTOS.h"
#include "timers.h" 
#include "string.h"
#include "stdarg.h"
#include "App.h"
#include "userfatfs.h"
#include "userflashconfig.h"
#include "stm322xg_eval.h"
#include "iwdg.h"

#define BOOT_LOG_NOTES   ("\r\n[NOTES]: ")
#define BOOT_LOG_ERROR   ("\r\n[ERROR]: ")

void CmdSendData( uint8_t *pBuf, uint32_t Length)
{
    uint32_t i=0;
    for(i=0; i<Length; i++)
    {
        USART_SendData(PRINTF_DEBUG1_UART,(unsigned char)pBuf[i]);
        while(USART_GetFlagStatus(PRINTF_DEBUG1_UART,USART_FLAG_TC) != SET);
    }
}
uint32_t CmdGetData(uint8_t *pBuf, uint32_t Length)
{
    /* 等待串口输入数据 */
    uint32_t Num = 0;
    for (Num = 0; Num < Length; Num++)
    {
        if (USART_GetFlagStatus(PRINTF_DEBUG1_UART, USART_FLAG_RXNE) == SET)
        {
            *pBuf = (uint8_t)USART_ReceiveData(PRINTF_DEBUG1_UART);
            pBuf++;
        }
        else
            break;
    }
    return Num;
}

#define xprintf printf


#define ESC (0x1B)  
#define DEL (0x08)
#define CR  (0x0D)
#define LF  (0x0A)
#define CRLF_COLON  ("\r\n:")
#define CONSOLE_BUF_MAX (32)
#define CMD_BUF_MAX (64) 

char CmdBuf[CMD_BUF_MAX + 1];

unsigned int  CmdBufCnt = 0;
////////////////////////////////////////////
int TestMain(int argc, char *argv[]);
int CanMain(int argc, char *argv[]);
int KwpMain(int argc, char *argv[]);
int VpwMain(int argc, char *argv[]);
int PwmMain(int argc, char *argv[]);
int ImeiMain(int argc, char *argv[]);
int NumberMain(int argc, char *argv[]);
int NumberWriteMain(int argc, char *argv[]);
//int SignalMain(int argc, char *argv[]);
int NetMain(int argc, char *argv[]);
int GpsMain(int argc, char *argv[]);
int GpsTdMain(int argc, char *argv[]);
int GpsBoxMain(int argc, char *argv[]);
int SensorMain(int argc, char *argv[]);
int EcuLogMain(int argc, char *argv[]);
int AdcMain(int argc, char *argv[]);
int LedMain(int argc, char *argv[]);
int  BackUpMain(int argc, char *argv[]);
int  FormatMain(int argc, char *argv[]);
int AppVesionMain(int argc, char *argv[]);
int BootVesionMain(int argc, char *argv[]);
int HardMain(int argc, char *argv[]);
int ResetMain(int argc, char *argv[]);
int JmpAppMain(int argc, char *argv[]);
int DeviceMain(int argc, char *argv[]);
int EscMain(int argc, char *argv[]);
int Help(int argc, char *argv[]);

const tCmdLineEntry gCmdTbl[] =
{
    ////////////////////////////////////////////

    //{"AT+TEST", TestMain, "Test One Key"},

    {"AT+CAN", CanMain, "Test OBD Can[car_Can 29.xml]"},
    {"AT+KWP", KwpMain, "Test OBD Kwp[car_Kwp135BPS.xml]"},
    {"AT+VPW", VpwMain, "Test OBD Vpw[car_Vpw.xml]"},
    {"AT+PWM", PwmMain, "Test OBD Pwm[car_Pwm.xml]"},

    { "AT+IMEI",ImeiMain, "Read EC20 IMEI"},
    { "AT+NUM",NumberMain, "Read SIM Number"},
    { "AT+WNUM",NumberWriteMain, "Write SIM Number AT+WNUM 134xxxxxxxx"},
   // { "AT+SIGNAL",SignalMain, "Read Net Signal Rssi"},
    { "AT+NET",NetMain, "Read Net Status"},
   // { "AT+GPS",GpsMain, "Read Gps Status"},
    //{ "AT+GPSTD",GpsTdMain, "Read Gps Status Via TDMonitor2.1.9.4.exe"},
    { "AT+GPSBOX",GpsBoxMain, "Read Gps Status Via ubox device"},
    { "AT+SENSOR",SensorMain, "Read G sensor data"},
    { "AT+ECULOG",EcuLogMain, "Printf ECU TOOLS INFO [ AT+ECULOG 1/0]"},
    { "AT+ADC",AdcMain, "Test AdC Value"},
     { "AT+LED",LedMain,"Test Led Value AT+LED 0"},
    { "AT+BACKUP",BackUpMain, "BackUp Net Data Test"},
    { "AT+FORMAT",FormatMain, "Forma Sd Card"},
    { "AT+APPVER",AppVesionMain, "Read App Version"},
    { "AT+BOOTVER",BootVesionMain, "Read Boot Version"},
     { "AT+HARDVER",HardMain, "Read Hardware Version"},
    { "AT+DEVICE",DeviceMain, "Read Infomations"},
    { "AT+JMPAPP",JmpAppMain, "Jump To App"},
    { "AT+RESET",ResetMain, "Reset Deivice "},
    { "AT+ESC",EscMain, "Esc Cmd Mode "},
    { "?",   Help,"For Help"},

};
extern uint8 gEcuSelFTest;

int TestMain(int argc, char *argv[])
{
    int ret1[6];
    int index = 0;

    memset(ret1, 0xff, sizeof(ret1));

    ret1[index++] = CanMain(argc, argv);
    ret1[index++] = KwpMain(argc, argv);
    ret1[index++] = VpwMain(argc, argv);
    ret1[index++] = PwmMain(argc, argv);
    //get_IMEI_init();
    //ret1[index++] = FcarNetTest();
    //ret1[index++] = Get_GPS_All_Data_Report();

    if (ret1[0] == 0)
        xprintf("Can  test  success\r\n");
    else
        xprintf("Can  test  fail\r\n");
    if (ret1[1] == 0)
        xprintf("Kwp test  success\r\n");
    else
        xprintf("Kwp  test  fail\r\n");
    if (ret1[2] == 0)
        xprintf("Pwm test  success\r\n");
    else
        xprintf("Pwm  test  fail\r\n");
    if (ret1[3] == 0)
        xprintf("Vpw  test  success\r\n");
    else
        xprintf("Vpw  test  fail\r\n");
    if (ret1[4] == 0)
        xprintf("NET  test  success\r\n");
    else
        xprintf("NET  test  fail\r\n");
    if (ret1[5] == 0)
        xprintf("GPS  test  success\r\n");
    else
        xprintf("GPS  test  fail\r\n");

    return 0;
}

uint8_t OBDIITest_ReadBuf(uint16_t obdii_outtime)
{

    uint8_t recvbuf[64] = {0};
    uint16_t len;
    while (obdii_outtime--)
    {
        if (USB_BULK_Queue_Read(&OBD_RX_QueueData, recvbuf, &len) == true)
        {
            //xprintf("OBDIITest_ReadBuf success len=%d\r\n", len);
            return 0;
        }
        else
        {
            vTaskDelay(2);
        }
    }

    return 1;
}

int CanMain(int argc, char *argv[])
{
    const char cmd[]= {0xA5,0xA5,0x00,0x42,0x10,0x01,0x01,0xe8,0x48,0x02,0x81,0x03,0x10,0x04,0x98,0xda,0x11,0xf1,0x05,0x98,0xda,0xf1,0x11,0x06,0x1f,0xff,0xff,0xff,0x07,0x00,0x00,0x01,0x01,0x08,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x0a,0x82,0x00,0x0b,0x00,0x00,0x0c,0xE6,0x0d,0x01,0x0e,0x01,0x00,0x04,0x30,0x30,0x00,0x0a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x0a,0x8b };
    const char test_data[]= { 0xa5,0xa5,0x00,0x0d,0x30,0x98,0xda,0x11,0xf1,0x01,0x3e,0x00,0x00,0x00,0x00,0x00,0x00,0x0f};
    xprintf("\r\n  can test   \r\n");
    gEcuSelFTest=1;
    USB_BULK_Queue_Write(&USB_RX_QueueData,(uint8_t *)&cmd,sizeof(cmd));
    vTaskDelay(100) ;

    USB_BULK_Queue_Write(&USB_RX_QueueData,(uint8_t *)&test_data,sizeof(test_data));
    vTaskDelay(100) ;

    return 0;
}
int KwpMain(int argc, char *argv[])
{
	
	const char cmd[]={0xa5 ,0xa5 ,0x00 ,0x28 ,0x13 ,0x01 ,0x00 ,0x00 ,0x00 ,0x02 ,0x00 ,0x03 ,0x80 ,0x04 ,0x00 ,0x77 ,0xc1 ,0xc1 ,0x05 ,0x08 ,0x0a ,0x0a ,0x03 ,0x03 ,0xe8 ,0x19 ,0x06 ,0x01 ,0x07 ,0x00 ,0x08 ,0x25 ,0x09 ,0x03 ,0x0a ,0x00 ,0x00 ,0x00 ,0x0b ,0x00 ,0x0c ,0x00 ,0x00 ,0x00 ,0xb1 };
	const char test_data[]={ 0xa5,0xa5,0x00,0x02,0x23,0x01,0xd9};
	xprintf("\r\n  kwp test   \r\n"); 
	gEcuSelFTest=1;
	USB_BULK_Queue_Write(&USB_RX_QueueData,(uint8_t *)&cmd,sizeof(cmd));
	vTaskDelay(3000) ; 
	USB_BULK_Queue_Write(&USB_RX_QueueData,(uint8_t *)&test_data,sizeof(test_data));
	vTaskDelay(2000) ;
	 
	return 0;
}
int VpwMain(int argc, char *argv[])
{

    const char cmd[]= { 0xa5,0xa5,0x00,0x17,0x11,0x02,0x00,0x03,0x01,0x04,0x01,0x01,0x05,0x00,0x00,0x06,0x6c,0x07,0xff,0x08,0xf1,0x09,0xff,0x0a,0x10,0x0b,0xff,0x29 };
    const char test_data[]= { 0xa5,0xa5,0x00,0x31,0x31,0x83,0xd6,0xeb,0x2c,0xa9,0x03,0x21,0xbb,0xef,0x5f,0x5f,0x4c,0xfc,0x10,0xec,0xbe,0xd4,0xed,0x51,0x06,0x45,0x4d,0x99,0x25,0x8e,0x51,0x65,0x53,0x05,0x5c,0x33,0xec,0x3f,0x54,0x16,0xa7,0x22,0xcd,0xcc,0x8f,0x60,0xd4,0xf3,0x4e,0x4a,0x60,0x3d,0xd8,0xa9};
    xprintf("\r\n Vpw test  \r\n");
    gEcuSelFTest=1;
    USB_BULK_Queue_Write(&USB_RX_QueueData,(uint8_t *)&cmd,sizeof(cmd));
    vTaskDelay(100) ;
    USB_BULK_Queue_Write(&USB_RX_QueueData,(uint8_t *)&test_data,sizeof(test_data));
    vTaskDelay(100) ;
    return 0;
}
int PwmMain(int argc, char *argv[])
{
    const char cmd[]= {0xa5,0xa5,0x00,0x1a,0x12,0x01,0x00,0x00,0x02,0x00,0x03,0x01,0x04,0x01,0x10,0x05,0x00,0x00,0x06,0xc4,0x07,0xff,0x08,0x10,0x09,0xff,0x0a,0xf5,0x0b,0xff,0xb9};
    const char test_data[]= { 0xa5,0xa5,0x00,0x08,0x32,0xc4,0x10,0xf5,0x22,0x02,0x00,0x5b,0x7d};
    xprintf("\r\n Pwm test \r\n");
    gEcuSelFTest=1;
    USB_BULK_Queue_Write(&USB_RX_QueueData,(uint8_t *)&cmd,sizeof(cmd));
    vTaskDelay(100) ;
    USB_BULK_Queue_Write(&USB_RX_QueueData,(uint8_t *)&test_data,sizeof(test_data));
    vTaskDelay(100) ;
    return 0;
}



int ImeiMain(int argc, char *argv[])
{
	unsigned int value;
	char code[20] = {0};
	get_IMEI(code);
	return 0;
	 
}
int NumberMain(int argc, char *argv[])
{
	/*
	get_iphone_number_init();
	return 0;
	*/
}
int NumberWriteMain(int argc, char *argv[])
{
/*
	if(argc==2)
	{
		Set_iphone_number(argv[1]);
	}
	else
	{
	       xprintf("\r\n cmd error \r\n"); 
	}
*/
	return 0;
}
int NetMain(int argc, char *argv[])
{
	int ret;	
	ret=Vehicle_status_project();
	if(ret==1) xprintf("\r\n Net Test Success \r\n"); 
	else   xprintf("\r\n Net Test Fail \r\n"); 

	return 0;
	 
}

int GpsMain(int argc, char *argv[])
{
    unsigned int value;
    //Post_Common_init();
    //Get_GPS_All_Data_Report();
    return 0;
}
int GpsTdMain(int argc, char *argv[])
{
    static unsigned int init=0;
    unsigned char len = 0;
    unsigned char Byte;

    if(init==0)
    {
        //Post_Common_init();
        init=1;
    }
    printf("Please Connect TDMonitor2.1.9.4.exe \r\n");
    while(1)
    {
        //Get_GPS_Td_Data_Report();
        vTaskDelay(100);
        len = CmdGetData(&Byte, 1);
        if((0 != len)&&(Byte==' '))
        {
            printf("GpsTdMain exit\r\n");
            break;
        }
    }
    return 0;
}

int GpsBoxMain(int argc, char *argv[])
{
	static unsigned int init=0;
	unsigned char len = 0;
	unsigned char Byte;
	 
	while(1)
	{
		u_blox_Test();
		vTaskDelay(100);
		len = CmdGetData(&Byte, 1);
		if((0 != len)&&(Byte==' '))
		{
			printf("GpsBoxMain exit\r\n");
			break;
		}
	}
	return 0;
}


int SensorMain(int argc, char *argv[])
{
    short sx,sy,sz;
    unsigned char Byte;
    unsigned char len = 0;
    while(1)
    {
        CollectSTK8BA53Acceleration();
        GetSTK8BA53Acceleration(&sx,&sy,&sz);
        printf("xyz==%04d mg %04d mg %04d mg \r\n", (sx),(sy),(sz)); //obd
        vTaskDelay(10) ;
        len = CmdGetData(&Byte, 1);
        if(0 != len) break;
    }
    return 0;
}
int EcuLogMain(int argc, char *argv[])
{
    if(argc==2)
    {

        gEcuSelFTest = atoi(argv[1]);
    }
    else
    {
        xprintf("\r\n cmd error \r\n");
    }
}

int AdcMain(int argc, char *argv[])
{
	GetAdc();
}
int LedMain(int argc, char *argv[])
{

	if(argc==2)
	{
		{STM_EVAL_LEDOff(0);STM_EVAL_LEDOff(1);STM_EVAL_LEDOff(2);}

		if(atoi(argv[1])==0)	  {STM_EVAL_LEDOff(0);STM_EVAL_LEDOff(1);STM_EVAL_LEDOff(2);}
		
		else if(atoi(argv[1])==1){STM_EVAL_LEDOff(0);STM_EVAL_LEDOff(1);STM_EVAL_LEDOn(2);}//blue
		else if(atoi(argv[1])==2){STM_EVAL_LEDOff(0);STM_EVAL_LEDOn(1);STM_EVAL_LEDOff(2);}//green
		else if(atoi(argv[1])==4){STM_EVAL_LEDOn(0);STM_EVAL_LEDOff(1);STM_EVAL_LEDOff(2);}//red
		
	}
	else
	{
	       xprintf("\r\n LedMain cmd error \r\n"); 
	}

}
int  BackUpMain(int argc, char *argv[])
{
    //Post_Common_init();
   // FcarInterfaceSendAgainMainTest();
}
int  FormatMain(int argc, char *argv[])
{
    FileFormat();
}
int AppVesionMain(int argc, char *argv[])
{
	
	USER_DATA info;
	printf("\r\n App version =%s\r\n",APP_VERSION);
	get_device_info(&info);
	printf("vehicle_model =%d\r\n",info.appdata.vehicle_model);
	printf("child_vehicle_model=%d\r\n",info.appdata.child_vehicle_model);
	printf("vehicle_version =%d\r\n",info.appdata.vehicle_version);
	printf("filename =%s\r\n",info.appdata.user_app_verson);
    return 0;
}
int BootVesionMain(int argc, char *argv[])
{
    printf("\r\n Boot version =%s\r\n",BOOT_VERSION);
    return 0;
}
int HardMain(int argc, char *argv[])
{
#ifdef  M_NEWHARDWARE
    xprintf("\r\n HardWare Version Need >= V1.2 \r\n");
#else
   xprintf("\r\n HardWare Version Need < V1.2\r\n");
#endif
}
int DeviceMain(int argc, char *argv[])
{
    xprintf("\r\n Device Infomation \r\n");
#ifdef  M_BIN_DE
    xprintf("\r\n Device App Bin File Need Encryption  \r\n");
#else
    xprintf("\r\n Device App Bin File Not Need Encryption  \r\n");
#endif

#ifdef  M_2_DATA_BACK
    xprintf("\r\n Device Support 2G Data BackUp  \r\n");
#else
    xprintf("\r\n Device Not Support 2G Data BackUp  \r\n");
#endif
}

int ResetMain(int argc, char *argv[])
{
    xprintf("\r\n Device Reset start \r\n");
    //DeviceReset();
}
int JmpAppMain(int argc, char *argv[])
{
	xprintf("\r\n Jump to App  \r\n");
	Cmdjmpappmain();
}
int EscMain(int argc, char *argv[])
{
    xprintf("\r\n Esc Cmd Mode \r\n");

}
/////////////////////////////////////////////////////////////////

void strtoupper(char *lpSRC)
{
    char Byte;

    while(*lpSRC)
    {
        Byte = *lpSRC;
		if(islower(Byte))
		{
			*lpSRC = toupper(*lpSRC);
		}
        lpSRC++;
    }
}

int Help(int argc, char *argv[])
{
    int i;
    xprintf("\r\n");
    xprintf("---------------------------------------------------------------------------\r\n");
    for(i = 0; i < sizeof(gCmdTbl)/sizeof(tCmdLineEntry); i++)
    {
        xprintf(" %s\ : %s\r\n", gCmdTbl[i].pcCmd, gCmdTbl[i].pcHelp);
    }
    xprintf("---------------------------------------------------------------------------\r\n");
    xprintf("\r\n");

    return 0;
}
void LOG_Info(const char *Info, const char *_format, ...)
{
    unsigned char xBuf[128];
    va_list argptr;
    memset(xBuf,0,sizeof(xBuf));
    va_start(argptr, _format);
    vsprintf((char*)xBuf, _format, argptr);
    va_end(argptr);
    CmdSendData( (uint8_t *)Info, strlen(Info));
    CmdSendData(xBuf, strlen((const char *)xBuf));
}



void BootCmdGet(void)
{
    unsigned int i;
    unsigned char Byte;
    unsigned char len = 0;
    const tCmdLineEntry *pCmdEntry=&gCmdTbl[sizeof(gCmdTbl)/sizeof(tCmdLineEntry)-2];
    xprintf(CRLF_COLON);

    while(1)
    {
        len = CmdGetData(&Byte, 1);

        if(0 != len)
        {
            switch(Byte)
            {
            case '\r':
                xprintf("\r\n");
                strtoupper(CmdBuf);
                if(CMDLINE_BAD_CMD == CmdLineProcess(CmdBuf,sizeof(gCmdTbl)/sizeof(tCmdLineEntry)))
                {
                    LOG_Info(BOOT_LOG_ERROR, "Invalid Command.\r\n");
                    LOG_Info(BOOT_LOG_NOTES, "Please Enter '?' for Help.\r\n");
                }
                else
                {
                    if(strstr((const char *)CmdBuf, (const char *)pCmdEntry->pcCmd) !=0)
                    {
                        return;
                    }
                }
                memset(CmdBuf, 0, CMD_BUF_MAX);
                CmdBufCnt = 0;
                xprintf(CRLF_COLON);
                break;
            case DEL:
                if(CmdBufCnt > 0)
                {
                    CmdBufCnt--;
                    CmdBuf[CmdBufCnt] = 0;
                    xprintf(CRLF_COLON);
                    xprintf("%s", CmdBuf);
                }
                break;
            default:
                CmdSendData(&Byte,1);
                if(CmdBufCnt >= CMD_BUF_MAX)
                {
                    for(i = 0; i < CMD_BUF_MAX; i++)
                    {
                        CmdBuf[i] = CmdBuf[i+1];
                    }
                    CmdBuf[i] = Byte;
                    CmdBuf[CMD_BUF_MAX] = 0;
                }
                else
                {
                    CmdBuf[CmdBufCnt++] = Byte;
                    CmdBuf[CmdBufCnt]   = 0;
                }
                break;
            }
        }
    }
}

extern TaskHandle_t	data_transfer_task;
extern TaskHandle_t	Task_ObdiiCmdProcess_Proc_handle;
void cmd_main(void)
{
	unsigned char Byte;
	unsigned int timeCnt = 0;
	unsigned char len = 0;
	unsigned int Obdiiflag;
#ifndef APP_APPLACATION
    USART_ITConfig(PRINTF_DEBUG1_UART, USART_IT_RXNE, DISABLE);
    xprintf("Enter Factory Mode\r\n");
   
    CmdBufCnt = 0;
    while(timeCnt++ < 15)
    {
    	 STM_EVAL_LED_FIFO(100);
        vTaskDelay(1) ;
        len = CmdGetData(&Byte, 1);
        if(0 != len)
        {
            if(' ' == Byte)
            {
		  vTaskSuspend(data_transfer_task); 
                vTaskSuspend (Task_ObdiiCmdProcess_Proc_handle);
                Obdiiflag=1;
                BootCmdGet();
            }
        }
    }
    if(Obdiiflag==1)
    {
	 vTaskResume (data_transfer_task);
        vTaskResume (Task_ObdiiCmdProcess_Proc_handle);
        Obdiiflag=0;

    }
    USART_ITConfig(PRINTF_DEBUG1_UART, USART_IT_RXNE, ENABLE);
    xprintf("Exit Factory Mode \r\n");
#endif
}


