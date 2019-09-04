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


extern uint8_t RxBuffer [RXBUFFERSIZE];
extern uint8_t TxBuffer [RXBUFFERSIZE];
extern __IO uint16_t RxCounter;

extern  unsigned int sendCommand(char *Command,char *Response, unsigned long Timeout, unsigned char Retry);
extern unsigned char downfile_buffer[];
extern int u2_printf(char *Command,int len);


void printfHexBug(char *buf,uint16_t len)
{
		uint16_t i;
		return;
//		printf("\r\n"); 
		for(i=0;i<len;i++)
		{
				if(((i%16) == 0) && (i != 0))
				{
					printf("\r\n");
				}
				printf("%02x ",buf[i]);
		}
		
		printf("\r\n");
}


const char* strstr2(const char* src, const char* sub)
{
    const char *bp;
    const char *sp;
	
    if(!src || !sub)
    {
        return src;
    }
    /* 遍历src字符串  */
    while(*src)
    {
        /* 用来遍历子串 */
        bp = src;
        sp = sub;
        do
        {
            if(!*sp)  /*到了sub的结束位置，返回src位置   */
                return src;
        }
		
				while(*bp++ == *sp ++);
		
        src ++;
    }
    	
    return NULL;
}


unsigned int sendFileReadCommand(char *Command,char *Response, unsigned long Timeout, unsigned char Retry,int len)
{
		unsigned char n,ret;
		int Time_Cont;
		const char* p;
		int len_check;
		len_check = len + strlen(Command) + strlen(Response) + strlen("\r\nok\r\n") + 1;
		memset(RxBuffer,0,RXBUFFERSIZE);
		RxCounter = 0;
		for (n = 0; n < Retry; n++)
		{
				ret = u2_printf(Command,strlen(Command));
				if(ret > 0)
				{
						printf("command send failed\r\n");
						return 0;
				}
				
				Time_Cont = 0;
				while (Time_Cont < Timeout) //start 
				{
						vTaskDelay(100);
						Time_Cont += 100;
						if(len_check == RxCounter)	//分批获取从RAM下载的数据
						{
						       break ;  
						}
				}
				
				if(len_check == RxCounter)	//分批获取从RAM下载的数据
				{
						break ; 
				}
				
				break;
		}
			
		if(len_check != RxCounter)
		{
				printf("all == %d\r\n",len_check); 

				printf("%s\r\n",RxBuffer);
				printf("RxCounter == %d\r\n",RxCounter);
				printf("strlen(Command) == %d\r\n",strlen(Command));
				printf("strlen(Response) == %d\r\n",strlen(Response));
				printf("strlen(ok) == %d\r\n",strlen("\r\nok\r\n")); 
				
				return 0;
		}

		p = strstr2((const char*)RxBuffer,(const char*)Response);
		if( p )	//分批获取从RAM下载的数据
		{
		    memset(downfile_buffer, 0, RXBUFFERSIZE);   
				memcpy(downfile_buffer,p+strlen(Response),len);   
				memset(RxBuffer,0,RXBUFFERSIZE);
				RxCounter = 0;
				printfHexBug(downfile_buffer,len);
			
				return 1;
		}
		else
		{
				return 0;
		}
		memset(RxBuffer,0,RXBUFFERSIZE);
		RxCounter = 0;
		
		return 1;
}


char* gefiletstrinfo(char *src,char  *src_start,char  *sep_start,char  *sep_end,char  *dst)
{
		char *p1,*p2,*p3;
		char tmp_buf[20] = {0};
		p1 = NULL;
		p2 = NULL;
		p3 = NULL;
	
		memset(tmp_buf,0,sizeof(tmp_buf));
		
		p1 = strstr(src,src_start);
		
		if( p1 == NULL )	
		{
				//printf("src_start fail \r\n");
				return NULL;
		}
	 
		p2 = strstr(p1,sep_start);
		if(p2  == NULL )
		{
				//printf("sep_start fail \r\n");
				return NULL;
		}
	
		if(sep_end == NULL)
		{
				p3 = p2 + strlen(p2);
		}
		else if (strstr(sep_start,sep_end) == sep_start)
		{
				p3 = strstr(p2+1,sep_end);
				if( p3 == NULL )
				{
					//printf("sep_end fail \r\n");
						return NULL;
				}
		}
		else
		{
				p3 = strstr(p2,sep_end);
				if( p3 == NULL )
				{
					//printf("sep_end fail \r\n");
						return NULL;
				}
		}
	
		if(p2 != p3)
		{
				memcpy(dst,p2+1,(p3-p2-1));
				//printf("dst dif==%d\r\n",p3-p2-1);
		}
	
		//printf("dst==%s\r\n",dst);
		//printf("start==%s\r\n",p3);
		
		return p3;
}


void gefiletstrinfotest()
{
		char ginffostr[]="+QHTTPDL: 2048,105332,4017";
		char dst[12];
		memset(dst,0,sizeof(dst));
		gefiletstrinfo(ginffostr,"QHTTPDL",":",",",dst);
		gefiletstrinfo(ginffostr,"QHTTPDL",",",",",dst);
}


uint32_t  EcFileOpen(char  *filehname,int mode,uint32_t *handle) //AT+QFOPEN="RAM:1.bin",0
{
		char cmd_buf[128] = {0};
		char dst[12];

		memset(dst,0,sizeof(dst));
		
		memset(cmd_buf,0,sizeof(cmd_buf));
		sprintf(cmd_buf,"AT+QFOPEN=\"%s\",%d\r\n",filehname,mode);
		
		printf("EcFileOpen=%s",cmd_buf);
		
		if (sendCommand(cmd_buf, "OK\r\n", 5000, 20) == 1)
		{
				 gefiletstrinfo(downfile_buffer,"QFOPEN:"," ",0,dst);//get file handle
				*handle = atoi(dst);
			
				return 0;
		}
		else 
		{	
				return 1;
		}
}


uint32_t  EcFileQFLDS(char  *filehname,int mode) 
{
#if 0
		char cmd_buf[128] = {0};
		memset(cmd_buf,0,sizeof(cmd_buf));
		sprintf(cmd_buf,"AT+QFPOSITION=%d\r\n",filehandle);
		if (sendCommand(cmd_buf, "OK\r\n", 5000, 10) == 1)
		{
				return 0;
		}
		else
		{
				return 1;
		}
#endif
}


unsigned int  EcFileSeek(uint32_t filehandle,int offset,int position) 
{
		char cmd_buf[128] = {0};
		char dst[12];
		memset(dst,0,sizeof(dst));
		memset(cmd_buf,0,sizeof(cmd_buf));
		//从RAM读数据之前先设置偏移地址
		sprintf(cmd_buf,"AT+QFSEEK=%d,%d,%d\r\n",filehandle,offset,position);
		if (sendCommand(cmd_buf, "OK\r\n", 5000, 10) == 1)
		{
				return 0;
		}
		else
		{ 
				return 1;
		}
}
					

unsigned int EcFileRead(uint32_t filehandle,int length,int *readlength) 
{
		char cmd_buf[128] = {0};
		char dst[12];
		memset(dst,0,sizeof(dst));
		memset(cmd_buf,0,sizeof(cmd_buf));
		sprintf(cmd_buf,"AT+QFREAD=%d,%d\r\n",filehandle,length);
		if (sendCommand(cmd_buf, "OK\r\n", 5000, 10) == 1)
		{
				gefiletstrinfo(downfile_buffer,"CONNECT"," ",0,dst);//get file handle
				*readlength = atoi(dst);
			
				return 0;
		}
		else 
		{	
				return 1;
		}
}


unsigned int  EcFileRead2(uint32_t filehandle,int length,int *readlength) 
{
		char cmd_buf[128] = {0};
		char dst[12];
		char respond[128] = {0};
		memset(dst,0,sizeof(dst));
		memset(cmd_buf,0,sizeof(cmd_buf));
		memset(respond,0,sizeof(respond));
			
		sprintf(cmd_buf,"AT+QFREAD=%d,%d\r\n",filehandle,length);	
		
//		sprintf(respond,"CONNECT %d\r\n",length,length);
		sprintf(respond,"CONNECT %d\r\n",length);
		if (sendFileReadCommand(cmd_buf, respond, 5000, 10,length) == 1)
		{
				// gefiletstrinfo(downfile_buffer,"CONNECT"," ","\r\n",dst);//get file handle
				// *readlength = atoi(dst);
				return 0;
		}
		else 
		{	
				return 1;
		}
}


unsigned int  EcFileDelete(char  *filehname) 
{
		char cmd_buf[128] = {0};

		memset(cmd_buf,0,sizeof(cmd_buf));
		sprintf(cmd_buf,"AT+QFDEL=\"%s\"\r\n",filehname);
		
		if (sendCommand(cmd_buf, "OK\r\n", 5000, 20) == 1)
		{
				return 0;
		}
		else 
		{	
				return 1;
		}
}


unsigned int  EcFilePostion(uint32_t filehandle) 
{
	char cmd_buf[128] = {0};
	char dst[12];

	memset(dst,0,sizeof(dst));
	memset(cmd_buf,0,sizeof(cmd_buf));
	sprintf(cmd_buf,"AT+QFPOSITION=%d\r\n",filehandle);
	
	printf("EcFilePostion=%s",cmd_buf);
	
	if (sendCommand(cmd_buf, "OK\r\n", 5000, 20) == 1)
	{
			return 0;
	}
	else 
	{	
			return 1;
	}
}


unsigned int  EcFileClose(uint32_t filehandle) 
{
		char cmd_buf[128] = {0};
		char dst[12];

		memset(dst,0,sizeof(dst));
		memset(cmd_buf,0,sizeof(cmd_buf));
		sprintf(cmd_buf,"AT+QFCLOSE=%d\r\n",filehandle);
		
		//printf("EcFilePostion=%s",cmd_buf);
		
		if (sendCommand(cmd_buf, "OK\r\n", 5000, 20) == 1)
		{
				return 0;
		}
		else 
		{	
				return 1;
		}
}

