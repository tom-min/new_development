#include "user_stm32f2xx.h"
#include "userflashconfig.h"
#include "user_config.h"
#include "App.h"
#include "ff.h"

#define INFO_BIN_SPI 	64	

extern const TCHAR gBinPath[]; 
extern void printfHexBug(char *buf,uint16_t len);

FRESULT UserBinFileSaveCheck(Vehicle_Model_Info *user_vehicle_model_info) //if return==0 start download
{
		FRESULT res;
		DIR dir;
		UpdateStatus status;
		TCHAR binfilename[64];

		res = f_opendir(&dir, gBinPath);      
		if(res != FR_OK)
		{
				res = f_mkdir((const TCHAR*)&gBinPath);       
				if(res != FR_OK)
				{
						printf("UserBinFileSaveCheck f_mkdir  fail...\r\n");
						return 1;
				}
		}
		
		res = f_closedir(&dir);
		if(res != FR_OK)  
		{
				printf("UserBinFileSaveCheck f_closedir res==%d fail...\r\n",res);
		}
		
		status = UserAppUpdateCheckVerion(user_vehicle_model_info);
		if(status == PASSED)
		{
				return 0;
		}
		else
		{
				return 1;
		}
}


void  UserBinFilemkdir(char *dir_name)
{
		FRESULT res;
		
		res = f_mkdir(dir_name);
		if(!res)
		{
				printf("f_mkdir successful,dir name: %s,rts = %d\r\n",dir_name,res);
		}
		else
		{
				printf("f_mkdir error,dir name: %s,rts = %d\r\n",dir_name,res);
		}
}


FRESULT UserBinFileSave(TCHAR *pbinfilename,const void* buff,UINT btw,UINT* bw)
{
		FRESULT res;
		TCHAR binfilename[64];
		FIL file;
		int file_size;
	
		memset(binfilename,0,sizeof(binfilename));
		sprintf(binfilename,"%s/%s",gBinPath,pbinfilename);
//		sprintf(binfilename,"0:/%s",pbinfilename);
		
//		printf("spi flash bin name: %s\r\n",binfilename);
		res = f_open(&file,binfilename, FA_OPEN_ALWAYS| FA_WRITE | FA_READ);
//		res = f_open(&file,"188.bin", FA_OPEN_ALWAYS| FA_WRITE | FA_READ);
//		res = f_open(&file,pbinfilename, FA_OPEN_ALWAYS| FA_WRITE | FA_READ);
		if(FR_OK != res)
		{
				printf("UserBinFileSave f_open fail, error number: %d\r\n",res);
				return 1 ;
		}
//		printf("11111111\r\n");
		
		file_size = f_size(&file);
		//printf("111 bin file size: %d\r\n",file_size);
//		printf("111 bin file size: %ld\r\n",f_size(&file));	
		
		res = f_lseek(&file,f_size(&file));
		if(FR_OK != res)
		{
				printf("UserBinFileSave f_lseek, error number: %d\r\n",res);
				res = f_unlink(binfilename);
				return 1 ;
		}
//		printf("12222222\r\n");
		
		res = f_write(&file,buff, btw,bw);
		if(FR_OK != res)
		{
				printf("UserBinFileSave f_write error \r\n");
				res = f_unlink(binfilename);
				return 1 ;
		}
//		printf("133333333\r\n");
		
		res = f_sync(&file);	
		if(FR_OK != res) 
		{
				printf("UserBinFileSave f_sync error \r\n");
				return 1 ;
		}
//		printf("144444444\r\n");
		
		file_size = f_size(&file);
		//printf("222 bin file size: %d\r\n",file_size);
//		printf("222 bin file size: %ld\r\n",f_size(&file));
		
		res = f_close(&file);
		if(FR_OK != res) 
		{
				printf("UserBinFileSave f_close error \r\n");
				res = f_unlink(binfilename);
				return 1 ;
		}
//		printf("15555555\r\n");

		return 0;
}


FRESULT UserBinFileDelete(TCHAR *pbinfilename)
{
		FRESULT res;
		TCHAR binfilename[64];
		FIL file;
		memset(binfilename,0,sizeof(binfilename));
		sprintf(binfilename,"%s/%s",gBinPath,pbinfilename);
		res = f_unlink(binfilename);
	
		return 1;
}


void read_binfile(char *filepath)
{
		FRESULT rts;
		FIL configfile;
		unsigned int SizeWritten = 0;
		uint8_t read_buf[INFO_BIN_SPI] = {0};
		uint8_t bin_path[64] = {0};
		uint32_t i,len,file_size; 
	
		sprintf((char *)bin_path,"%s/%s",gBinPath,filepath);

		printf("welcome to [%s], bin file name: %s\r\n",__func__,bin_path);
		
#if 1
		rts = f_open(&configfile,(char *)bin_path, FA_OPEN_ALWAYS| FA_WRITE | FA_READ);
		if(FR_OK == rts)
		{       
				file_size = f_size(&configfile);
				printf("bin file size: %d\r\n",file_size);
			
				return;
				if(file_size%INFO_BIN_SPI)
						len = (file_size/INFO_BIN_SPI)+1;
				else
						len = file_size/INFO_BIN_SPI;
				printf("len value: %d\r\n",len);
			
				for(i=0;i<len;i++)
				{
					rts = f_lseek(&configfile,i*INFO_BIN_SPI);
//					if(!rts)
//							printf("f_lseek rts=%d\r\n",rts);
					
					rts = f_read(&configfile,read_buf, sizeof(read_buf),&SizeWritten);
					if(!rts)
					{
//							printf("f_read read = %d,rts=%d,read_buf: %s\r\n",SizeWritten,rts,read_buf);
							printfHexBug((char *)read_buf,SizeWritten);
					}
				}
		}
		else
		{
				printf("f_open error, rts = %d\r\n",rts);
		}
		
		rts = f_close(&configfile); 
		if(!rts)
			printf("f_close rts = %d\r\n", rts);
#endif
		
}

