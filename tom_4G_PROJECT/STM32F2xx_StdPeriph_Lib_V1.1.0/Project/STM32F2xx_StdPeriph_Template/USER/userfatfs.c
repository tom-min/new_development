#include "ff.h" 
#include "stdint.h" 
#include "stdio.h"
FATFS gDosFS;
#define NULL 0
FRESULT myf_getfree(char *drv, uint32_t *total, uint32_t *free)
{
	FATFS *fs1;
	FRESULT res;
	uint32_t fre_clust = 0, fre_sect = 0, tot_sect = 0;


	res = (uint32_t)f_getfree((const TCHAR*)&drv, (DWORD*)&fre_clust, &fs1);
	if(res == 0)
	{
		tot_sect = (fs1->n_fatent - 2) * fs1->csize;                //
		fre_sect = fre_clust * fs1->csize;                          //     

#if _MAX_SS != 512                                                  //
		tot_sect *= fs1->ssize / 512;
		fre_sect *= fs1->ssize / 512;
#endif          

		*total = (tot_sect>>1);        //
		*free  = (fre_sect>>1);        //
	}

	printf("total:%10lu KB\r\nfree:  %10lu KB\r\n", *total , *free);

	return res;
}


FRESULT FileFormat(void)
{
		FRESULT result=FR_OK;
		char work[_MAX_SS];
		result = f_mkfs("",FM_ANY,0,work,sizeof(work));
		if(FR_OK != result)
		{
				printf("FileFormat error result=%d\r\n",result);
		}
		else
		{
				printf("FileFormat success result=%d\r\n",result);
		}
		
		return result;
}


void write_test()
{
	FRESULT rts;
	FIL configfile;
	unsigned int SizeWritten = 0;
	uint8_t buf[1024];
	uint32_t total; uint32_t free;
	rts = f_open(&configfile,"123456789.txt", FA_OPEN_ALWAYS| FA_WRITE | FA_READ);
	if(FR_OK == rts)
	{       
		rts = f_lseek(&configfile,0);
		if(rts)
		printf("f_lseek rts=%d\r\n",rts);
		rts = f_write(&configfile,buf, sizeof(buf),&SizeWritten);
		if(rts)
		printf("f_write w = %d,rts=%d\r\n",SizeWritten,rts);
		
		rts = f_sync(&configfile);
		if(rts)
		printf("f_sync rts=%d\r\n", rts);
		rts = f_close(&configfile); 
		if(rts)
		printf("f_close  rts=%d\r\n", rts);
		
		//myf_getfree("0:",&total,&free);
	}
	else
	{
			printf("f_open err rts = %d \r\n",rts);
			FileFormat();
#ifdef 1
			__set_FAULTMASK(1);//关闭总中断
			NVIC_SystemReset();//请求单片机重启
#endif
	}

}


FRESULT FileuUnMount(void)
{
    FRESULT result = FR_OK;
	
    result = f_mount(NULL, "", 1);
    if(FR_OK != result)
    {
        printf("unmount error result=%d\r\n",result);
		
    }
		else
		{
				printf("unmount success result=%d\r\n",result);
		}
		
    return result;
}


FRESULT FileuMount(void)
{
    FRESULT result = FR_OK;
	
    result =  f_mount(&gDosFS, "", 1);
    if(FR_OK != result)
    {
        printf("mount error result=%d\r\n",result);
    }
		else
		{
				printf("mount success result=%d\r\n",result);
		}
    return result;
}


void fatfstest(void)
{
	uint32_t total; 
	uint32_t free;
	FRESULT ret;
	
	ret = FileuMount();
	 if(ret==FR_OK)
	{
		myf_getfree("0:",&total,&free); 
	}
	else
	{
		FileuUnMount();
		sFLASH_EraseBulk();
		ret = FileFormat(); 
		if(FR_OK == ret)
		{
			FileuUnMount();
			FileuMount();
		}

	}
 
}

