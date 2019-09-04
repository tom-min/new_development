#include "user_stm32f2xx.h"
#include "userflashconfig.h"
#include "user_config.h"
#include "App.h"
#include "ff.h"
#include "FreeRTOS.h"
#include "task.h"


//U_USER_DATA这个结构体的数据填充在FLASH_USER_DATA_START_ADDR这个扇区，并且初始化全为0x00,擦除后全为0xff
#ifdef BOOT_APPLACATION
static const  U_USER_DATA g_u_userdata __attribute__((at(FLASH_USER_DATA_START_ADDR)));
#endif

UpdateStatus UserAppUpdate( uint32_t  user_app_address_offset, uint8_t *p_appdata,uint32_t appdatalen);
const TCHAR gBinPath[] = "bin";
//const uint8_t gappprefixversion[] = "STM32F2xx_FCAR_OBDII_APP_V";
const uint8_t gappprefixversion[] = "STM32F2xx_FCAR_OBDII_4G_APP_V";
const uint8_t gappversion[] = APP_VERSION;
const uint8_t gbootversion[] = BOOT_VERSION;	//flash中

//USER_DATA tom_userdata;	//RAM中

extern Vehicle_Model_Info tom_vehicle_model_data;

extern TaskHandle_t  Task_ObdiiCmdProcess_Proc_handle;

extern int Vehicle_fire;


uint32_t GetSector(uint32_t Address)
{
    uint32_t sector = 0;

    if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
    {
        sector = FLASH_Sector_0;
    }
    else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
    {
        sector = FLASH_Sector_1;
    }
    else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
    {
        sector = FLASH_Sector_2;
    }
    else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
    {
        sector = FLASH_Sector_3;
    }
    else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
    {
        sector = FLASH_Sector_4;
    }
    else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
    {
        sector = FLASH_Sector_5;
    }
    else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
    {
        sector = FLASH_Sector_6;
    }
    else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
    {
        sector = FLASH_Sector_7;
    }
    else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
    {
        sector = FLASH_Sector_8;
    }
    else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
    {
        sector = FLASH_Sector_9;
    }
    else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
    {
        sector = FLASH_Sector_10;
    }
    else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11))*/
    {
        sector = FLASH_Sector_11;
    }

    return sector;
}


UpdateStatus UserDataWrite(USER_DATA *userdata)
{
    uint32_t  StartSector = 0;
    uint32_t  EndSector = 0;
    uint32_t  Address = 0;
    uint32_t  SectorCounter = 0;
    uint32_t  *pdata;
    uint32_t  i = 0;
    uint32_t userdatalen;
    userdatalen = sizeof(USER_DATA);
    /* Unlock the Flash to enable the flash control register access *************/
    FLASH_Unlock();

    /* Erase the user Flash area
      (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

    //0x0800C000,从0x0800C000开始将bin文件名字写到flash中，类似STM32F2xx_FCAR_OBDII_4G_APP_V1_00_20190726.bin
    Address =  FLASH_USER_DATA_START_ADDR;

    /* Clear pending flags (if any) */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                    FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);

    /* Get the number of the start and end sectors */
    StartSector = GetSector(FLASH_USER_DATA_START_ADDR);
    EndSector = GetSector(FLASH_USER_DATA_END_ADDR);

    for (SectorCounter = StartSector; SectorCounter < EndSector; SectorCounter += 8)
    {
        /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
        	 be done by word */
        if (FLASH_EraseSector(SectorCounter, VoltageRange_3) != FLASH_COMPLETE)
        {
            /* Error occurred while sector erase.
            	 User can add here some code to deal with this error  */
            return  FAILED;
        }
    }

    /* Program the user Flash area word by word
      (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

//	  pdata = (uint32_t  *)&userdata ;
    pdata = (uint32_t  *)userdata ;
    while (userdatalen)
    {

        if (FLASH_ProgramWord(Address, pdata[i]) == FLASH_COMPLETE)
        {
            i += 1;
            userdatalen -= 4;
            Address += 4;
        }
        else
        {
            /* Error occurred while writing data in Flash memory.
             User can add here some code to deal with this error */
            return  FAILED;
        }
    }

    /* Lock the Flash to disable the flash control register access (recommended
       to protect the FLASH memory against possible unwanted operation) *********/
    FLASH_Lock();
    return PASSED;
}


UpdateStatus UserDataRead(USER_DATA *userdata)
{
    uint8_t  *pdata;
    uint32_t  i = 0;
    uint32_t	userdatalen;
    uint32_t 	addr;

    userdatalen = sizeof(USER_DATA);
    pdata = (uint8_t *)userdata;
    addr = FLASH_USER_DATA_START_ADDR;

     

//		userdatalen = 128;
    //如果在前面阻塞OBD任务，这里会使OBD任务被删除，估计还是内存的问题,内存溢出导致任务被删除了，明天移植查看任务堆栈的函数看看
    while(userdatalen--)
    {
        *(pdata++) = *((uint8_t*)addr++);
    }

//		printf("kkkkkkkkkkkkkkkkk3333\r\n");
//		get_task_info(Task_ObdiiCmdProcess_Proc_handle);

    return PASSED;
}


UpdateStatus UserAppErase(void)
{
    uint32_t  StartSector = 0;
    uint32_t  EndSector = 0;

    uint32_t  SectorCounter = 0;
    uint32_t  *pdata;
    uint32_t  i = 0;
    /* Unlock the Flash to enable the flash control register access *************/
    FLASH_Unlock();

    /* Erase the user Flash area
      (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

    /* Clear pending flags (if any) */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                    FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);

    /* Get the number of the start and end sectors */
    StartSector = GetSector(FLASH_USER_APP_START_ADDR);
    EndSector = GetSector(FLASH_USER_APP_END_ADDR);

    for (SectorCounter = StartSector; SectorCounter < EndSector; SectorCounter += 8)
    {
        /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
        be done by word */

        if (FLASH_EraseSector(SectorCounter, VoltageRange_3) != FLASH_COMPLETE)
        {
            /* Error occurred while sector erase.
             User can add here some code to deal with this error  */
            return  FAILED;
        }
    }

    FLASH_Lock();
    /* Lock the Flash to disable the flash control register access (recommended
    to protect the FLASH memory against possible unwanted operation) *********/

    return PASSED;
}


UpdateStatus UserAppUpdate( uint32_t  user_app_address_offset, uint8_t *p_appdata,uint32_t appdatalen)
{
    uint32_t  *pdata;
    uint32_t  i = 0;
    uint32_t  Address = 0;

    /* Unlock the Flash to enable the flash control register access *************/
    FLASH_Unlock();

    /* Program the user Flash area word by word
      (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

    //0x08040000,写bin文件操作从0x08040000开始，每次写512bytes
    Address = (FLASH_USER_APP_START_ADDR + user_app_address_offset);

    if((Address + appdatalen) >= (FLASH_USER_APP_END_ADDR))
    {

	//printf("UserAppUpdate erro appdatalen=%d Address=%08x\r\n",appdatalen,Address);	
        return  FAILED;
    }

    while (appdatalen)
    {
        //每次写4个字节
        pdata = (uint32_t *)(&p_appdata[i]);
        if(FLASH_ProgramWord(Address, pdata[0]) == FLASH_COMPLETE)
        {
            i += 4;
            if(appdatalen >= 4)
            {
                appdatalen -= 4;
            }
            else
            {
                appdatalen -= appdatalen ;//处理写的字节数不是4个整数倍的最后次
            }

            Address += 4;
        }
        else
        {
            /* Error occurred while writing data in Flash memory.
             User can add here some code to deal with this error */
            return  FAILED;
        }
    }

    /* Lock the Flash to disable the flash control register access (recommended
    to protect the FLASH memory against possible unwanted operation) *********/
    FLASH_Lock();

    return PASSED;
}


UpdateStatus UserJumpToAppCheckVerion(Vehicle_Model_Info *jump_check_info)
{
	USER_DATA userdata;

    if(UserDataRead(&userdata) == PASSED)
    {
	if(userdata.appdata.vehicle_model ==0)
	{
		return FAILED;
	}
        if(memcmp((uint8_t*) (&userdata.appdata.vehicle_model),(uint8_t *)jump_check_info,sizeof(Vehicle_Model_Info)-200) == 0)
        {
            return PASSED;
        }
        else
        {

            if(userdata.appdata.vehicle_model ==jump_check_info->vehicle_model)
            {
                printf("UserJumpToAppCheckVerion success but car file  is not newest \r\n");
                return PASSED;
            }
            else
            {
                printf("UserJumpToAppCheckVerion fail car file type error \r\n");
                return FAILED;
            }
        }
    }
    else	//读车型信息和版本号失败
    {
        printf("UserJumpToAppCheckVerion  FAILED\r\n");
        return FAILED;
    }
}


UpdateStatus UserAppUpdateCheckVerion(Vehicle_Model_Info *vehicle_model_info)
{
    USER_DATA userdata;
    memset(&userdata,0,sizeof(userdata));
    if(UserDataRead(&userdata) == PASSED)
    {
        if(memcmp((uint8_t *)(&userdata.appdata.vehicle_model),(uint8_t *)vehicle_model_info,sizeof(Vehicle_Model_Info)-200) != 0)
        {
            return PASSED;
        }
        else
        {
            return NEWEST;
        }
    }
    else	//读车型信息和版本号失败
    {
        return FAILED;
    }
}


UpdateStatus UserAppUpdateVerionSave(Vehicle_Model_Info *version_info)
{
    USER_DATA userdata;
    memset(&userdata,0,sizeof(userdata));
    if(UserDataRead(&userdata) == PASSED)
    {
        userdata.head = 0xAAAAAAAA;
        userdata.end = 0xAAAAAAAA;
        memcpy((uint8_t *)(&userdata.appdata.vehicle_model),(uint8_t *)version_info,sizeof(Vehicle_Model_Info)-200);

    }
    else
    {
        return FAILED;
    }

    if(UserDataWrite(&userdata) == PASSED)
    {
        return PASSED;
    }
    else
    {
        return FAILED;
    }
}


UpdateStatus UserAppLock(void)
{
    FLASH_Lock();
    /* Lock the Flash to disable the flash control register access (recommended
    to protect the FLASH memory against possible unwanted operation) *********/

    return PASSED;
}


UpdateStatus UserBootUnlockErase(uint8_t *p_bootdata,uint32_t bootdatalen)
{
    uint32_t  StartSector = 0;
    uint32_t  EndSector = 0;
    uint32_t  Address = 0;
    uint32_t  SectorCounter = 0;

    /* Unlock the Flash to enable the flash control register access *************/
    FLASH_Unlock();

    /* Erase the user Flash area
      (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

    /* Clear pending flags (if any) */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                    FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);

    /* Get the number of the start and end sectors */
    StartSector = GetSector(FLASH_USER_BOOT_START_ADDR);
    EndSector = GetSector(FLASH_USER_BOOT_END_ADDR);

    for (SectorCounter = StartSector; SectorCounter < EndSector; SectorCounter += 8)
    {
        /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
        	 be done by word */
        if (FLASH_EraseSector(SectorCounter, VoltageRange_3) != FLASH_COMPLETE)
        {
            /* Error occurred while sector erase.
             User can add here some code to deal with this error  */
            return  FAILED;
        }
    }
    /* Lock the Flash to disable the flash control register access (recommended
    to protect the FLASH memory against possible unwanted operation) *********/
    FLASH_Lock();
    return PASSED;
}


UpdateStatus UserBootUpdate(uint8_t *p_bootdata,uint32_t bootdatalen)
{
    uint32_t  Address = 0;
    uint32_t  *pdata;
    uint32_t  i=0;
    /* Unlock the Flash to enable the flash control register access *************/
    FLASH_Unlock();

    /* Program the user Flash area word by word
      (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/
    Address = FLASH_USER_BOOT_START_ADDR;
    while (bootdatalen)
    {
        pdata = (uint32_t  *)(&p_bootdata[i]);
        if (FLASH_ProgramWord(Address, pdata[0]) == FLASH_COMPLETE)
        {
            i+=4;
            bootdatalen-=4;
            Address+=4;
        }
        else
        {
            /* Error occurred while writing data in Flash memory.
            User can add here some code to deal with this error */
            return  FAILED;
        }
    }

    /* Lock the Flash to disable the flash control register access (recommended
       to protect the FLASH memory against possible unwanted operation) *********/
    FLASH_Lock();
    return PASSED;
}


UpdateStatus UserBootVerionSave(uint8_t *p_bootversionback,uint8_t check)
{
    USER_DATA userdata;
    uint8_t *p_bootversion;

    memset(&userdata,0,sizeof(userdata));
    if(p_bootversionback == 0)
    {
        p_bootversion = (uint8_t *)gbootversion;
    }
    else
    {
        p_bootversion = p_bootversionback;
    }

    //boot版本名字长度大于64，则失败
    if(strlen((char *)p_bootversion) > 64)
    {
        return  FAILED;
    }
    //boot版本名字为空，则失败
    if(strlen((char *)p_bootversion) == 0)
    {
        return  FAILED;
    }

    if(UserDataRead(&userdata) == PASSED)
    {
        if(check == 1)
        {
            if((userdata.head == 0xAAAAAAAA)&&(userdata.end == 0xAAAAAAAA))
            {
                return NEWEST;
            }
        }
        {
            userdata.head = 0xAAAAAAAA;
            memset(userdata.bootdata.user_boot_verson,0,M_user_boot_verson_len);
            memcpy(userdata.bootdata.user_boot_verson,p_bootversion,strlen((char *)p_bootversion));
            userdata.end = 0xAAAAAAAA;
        }
    }
    else
    {
        return FAILED;
    }

    if(UserDataWrite(&userdata) == PASSED)
    {
        return PASSED;
    }
    else
    {
        return FAILED;
    }
}


FRESULT UserBinFilescan(FILINFO *filinfo)
{
    FRESULT res;
    FIL configfile;
    DIR dir;
    uint8_t i;

    res = f_opendir(&dir, gBinPath);    /*打开图片所在的目录*/
    if(res != FR_OK)
    {
        printf("UserBinFilescan f_opendir  fail \r\n");
        return 1;
    }
    while(1)
    {
        res = f_readdir(&dir,filinfo);
        if(res != FR_OK )
        {
            return 1;
        }
        if(filinfo->fname[0] == 0)
        {
            printf("232222222221213nakda\r\n");
            return 0;
        }
        if(filinfo->fname[0] == '.')
            continue;

        if(filinfo->fattrib != AM_DIR)
        {
            for(i = 0; i < 64; i ++)
            {
                /*寻找后缀为.bin的文件*/
                if((filinfo->fname[i+0] == '.') && (filinfo->fname[i+1] == 'b') && (filinfo->fname[i+2] == 'i') && (filinfo->fname[i+3] == 'n'))
                {
                    res = f_closedir(&dir );
                    printf("filinfo->fname %s \r\n",filinfo->fname);
                    return FR_OK ;
                }
            }
        }
    }

    {
        res = f_closedir(&dir);
        return 1;
    }
}


//UpdateStatus UserAppUpdateViaBinFile(TCHAR *pbinfilename)
UpdateStatus UserAppUpdateViaBinFile(Vehicle_Model_Info *tom_info)
{
    FRESULT rts;
    FIL file;
    FILINFO filinfo;
    unsigned int SizeWritten = 0;
    uint8_t buf[512];

    TCHAR binfilename[64];

    uint32_t len,i;
    uint32_t address_offset = 0;
    UpdateStatus updatastatus;

    //查询文件系统里是否有这个bin文件
    if(UserBinFilescan(&filinfo) != FR_OK)
    {
        printf("UserBinFilescan  fail \r\n");
        return 1;
    }

//		printf("tom update bin_file name : %s\r\n",filinfo.fname);
    //这个函数很重要，升级的关键
    updatastatus = UserAppUpdateCheckVerion(tom_info);
    if( updatastatus == FAILED)
    {
        printf("UserAppUpdateCheckVerion failed...\r\n");
        return FAILED;
    }
    if( updatastatus == NEWEST)					//app中的bin已经是最新的
    {
        printf("UserApp is the newest...\r\n");
        memset(binfilename,0,sizeof(binfilename));
        sprintf(binfilename,"%s/%s",gBinPath,filinfo.fname);

        rts = f_unlink(binfilename);
        if(rts  != FR_OK)
            printf("f_unlink delete fail %s \r\n",binfilename);

        return NEWEST;
    }
    if( updatastatus == BINFILEPREFIXERROR)	//bin文件头错误
    {
        printf("UserApp bin file prefix is not [%s].\r\n",gappprefixversion);

        memset(binfilename,0,sizeof(binfilename));
        sprintf(binfilename,"%s/%s",gBinPath,filinfo.fname);

        rts = f_unlink(binfilename);
        if(rts  != FR_OK)
            printf("f_unlink delete incorrect_prefix name  file  fail  %s \r\n",binfilename);

        return FAILED;
    }
    if(UserAppErase() == PASSED)
    {
        printf("UserAppErase completed\r\n");
    }
    else
    {
        printf("UserAppErase Error\r\n");
        return FAILED;
    }

    memset(binfilename,0,sizeof(binfilename));
    sprintf(binfilename,"%s/%s",gBinPath,filinfo.fname);
//		memcpy(pbinfilename,filinfo.fname,strlen(filinfo.fname));

    rts = f_open(&file,binfilename, FA_OPEN_EXISTING| FA_WRITE | FA_READ);
    if(FR_OK != rts)
    {
        printf("UserAppUpdateViaBinFile f_open fail, %s \r\n",binfilename);
        return 1 ;
    }
    else
    {
        printf("UserAppUpdateViaBinFile f_open success \r\n");
    }

    //这里加个逻辑来判断写到flash的数据是不是等于文件的总大小，不然就失败，防止下载过程中断网
    len = ( f_size(&file) / sizeof(buf) ) * (sizeof(buf) ) ;

    for(i=0; i<len; )
    {
        //每次读512bytes，然后写进flash
        rts = f_read(&file,buf, sizeof(buf),&SizeWritten);
        if((FR_OK != rts)|| (SizeWritten == 0) )
        {
            rts = f_close(&file);
            rts = f_unlink(binfilename);
            if(rts  != FR_OK) printf("f_unlink1 delete f_read  file  fail  %s \r\n",binfilename);
            return  1;
        }


        //写bin文件到flash，每次写512bytes
        if(UserAppUpdate(address_offset,buf,SizeWritten) != PASSED)
        {
            rts = f_close(&file);
            rts = f_unlink(binfilename);
            if(rts  != FR_OK) printf("f_unlink2  UserAppUpdate fail  \r\n");
            return  1;
        }
        else
        {
            // printf("UserAppUpdateViaBinFile UserAppUpdate  address_offset==%d SizeWritten %dfsuccess \r\n",address_offset,SizeWritten);
        }

        address_offset += SizeWritten;

        i += SizeWritten;

        //printf("bin file to flash successful, %d\r\n",i);
    }

    // printf("UserAppUpdateViaBinFile UserAppUpdate tail  f_size(&file)==%d \r\n",f_size(&file));
    SizeWritten = 0;
    len = (f_size(&file)) % (sizeof(buf));
    // printf("UserAppUpdateViaBinFile UserAppUpdate tail  len==%d \r\n",len);
    if(len != 0)
    {
        rts = f_read(&file,buf,len,&SizeWritten);
        if((FR_OK != rts)|| (SizeWritten == 0) )
        {
            rts = f_close(&file);
            rts = f_unlink(binfilename);
            if(rts  != FR_OK) printf("f_unlink3 delete f_read  file  fail  %s \r\n",binfilename);
            return  1;
        }

        // printf("UserAppUpdateViaBinFile f_read success  SizeWritten==%d \r\n",SizeWritten);

        if(UserAppUpdate(address_offset,buf,SizeWritten) != PASSED)
        {
            rts = f_close(&file);
            rts = f_unlink(binfilename);
            if(rts  != FR_OK) printf("f_unlink4  UserAppUpdate fail  \r\n");
            return  1;
        }
        else
        {
            //printf("UserAppUpdateViaBinFile UserAppUpdate tail  address_offset==%d SizeWritten %dfsuccess \r\n",address_offset,SizeWritten);
        }

        printf("bin file Write the last time to flash, %d\r\n",len);
    }

    rts = f_close(&file);

    memset(binfilename,0,sizeof(binfilename));
    sprintf(binfilename,"%s/%s",gBinPath,filinfo.fname);

    //写完删除bin文件
    rts = f_unlink(binfilename);
    if(rts != FR_OK)
    {
        printf("f_unlink delete bin  file  fail  %s \r\n",binfilename);
    }
    else
    {
        printf("f_unlink delete bin  file  success  %s \r\n",binfilename);
    }

    return  (UpdateStatus)FR_OK;
}


UpdateStatus UserAppUpdateViaBinFileAndCheck(void)
{
    UpdateStatus ret;

#ifdef APP_APPLACATION
    printf("in app ,no update app\r\n");
    return;//in app ,no update app;
#endif

    taskENTER_CRITICAL();
    //检查bin文件
    ret = UserAppUpdateViaBinFile(&tom_vehicle_model_data);
    taskEXIT_CRITICAL();
    if(ret == 0)
    {
        printf("UserAppUpdate has completed \r\n");
    }
    else if(ret == NEWEST)
    {
        printf("UserAppUpdate has NEWEST \r\n");
        return NEWEST;
    }
    else
    {
        printf("UserAppUpdate  Error\r\n");
        return FAILED;
    }

    if( UserAppUpdateVerionSave(&tom_vehicle_model_data) == PASSED)
    {
        printf("UserAppUpdateVerionSave completed \r\n");
        return PASSED;
    }
    else
    {
        printf("UserAppUpdateVerionSave  error \r\n");
        return  FAILED;
    }
}


UpdateStatus get_vehicle_info(Vehicle_Model_Info *version_info)
{
    USER_DATA userdata;
    memset(&userdata,0,sizeof(userdata));

    //printf("[%s] tom app_vehicle_model: %d,appversion: %d\r\n",__func__,version_info->vehicle_model,version_info->vehicle_version);

    if(UserDataRead(&userdata) == PASSED)
    {
        version_info->child_vehicle_model = userdata.appdata.child_vehicle_model;
        return PASSED;
    }
    else
    {
        return FAILED;
    }
}
UpdateStatus get_device_info(USER_DATA *info)
{
    USER_DATA userdata;
    memset(&userdata,0,sizeof(userdata));
    if(UserDataRead(&userdata) == PASSED)
    {
        memcpy((uint8_t *)(info),(uint8_t *)(&userdata),sizeof(USER_DATA));
        return PASSED;
    }
    else
    {
        return FAILED;
    }
}