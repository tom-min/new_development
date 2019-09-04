/*-----------------------------------------------------------------------*/
/* Glue functions for FatFs - MCI/NAND drivers            (C)ChaN, 2010  */
/*-----------------------------------------------------------------------*/
#include "integer.h"
#include "diskio.h"
#include "time.h"

#define u8 unsigned char

#define FLASH_SECTOR_SIZE (4*1024)
#define FLASH_SECTOR_COUNT (16*1024*1024)/FLASH_SECTOR_SIZE
#define FLASH_BLOCK_SIZE 1
DSTATUS disk_initialize (unsigned char drv)//输入的是哪个卡，0为第一个卡，返回的是卡的状态 初始化成功，无卡，初始化失败
{
    u8 state;

    if(drv)
    {
        return STA_NOINIT;  //仅支持磁盘0的操作  0x01
    }

    state = 0;//DRV_SdInit();//20160107

    return (0 == state)? 0 : STA_NODISK;
}

DSTATUS disk_status (BYTE drv)//输入的是哪个卡，0为第一个卡，其他暂不支持；返回的是卡的状态 初始化成功，无卡，初始化失败
{
    if(drv)
    {
        return STA_NOINIT;  //仅支持磁盘0操作
    }

    //检查SD卡是否插入
    //if(0 == DRV_SdInsertGet())//20160107
    //{
        //return STA_NODISK;
    //}

    return 0;
}

DRESULT disk_read (BYTE pdrv, BYTE* buff, DWORD sector, UINT count)
{
    
#if 1    
    u8 res=0;
    u8  i;
    if ((pdrv != 0)||(0 == count))
    {
        return RES_PARERR;  //仅支持单磁盘操作，count不能等于0，否则返回参数错误
    }

   // if(0 == DRV_SdInsertGet())
    {
        //return RES_NOTRDY;  //没有检测到SD卡，报NOT READY错误
    }
    for(i=0;i<count;i++)
	{
		res = sFLASH_ReadBuffer( buff,sector*FLASH_SECTOR_SIZE, FLASH_SECTOR_SIZE);
		buff += FLASH_SECTOR_SIZE;
		sector++;
	}
   buff -= FLASH_SECTOR_SIZE;  
    //处理返回值，将SPI_SD_driver.c的返回值转成ff.c的返回值
    return (res == 0x00)?RES_OK:RES_ERROR;

#endif

}

#if _READONLY == 0 //是否为只读模式

DRESULT disk_write (BYTE pdrv, const BYTE* buff, DWORD sector, UINT count)
{
	u8 res;
	u8  i;
	if (pdrv || !count)
	{
		return RES_PARERR;  //仅支持单磁盘操作，count不能等于0，否则返回参数错误
	}

	// if(0 == DRV_SdInsertGet())
	{
	//  return RES_NOTRDY;  //没有检测到SD卡，报NOT READY错误
	}

    // 读写操作
   //res = DRV_SdWriteMultiBlock(sector, (unsigned char *)buff, count);
	for(i=0;i<count;i++)
	{
		sFLASH_EraseSector(sector*FLASH_SECTOR_SIZE);
		res = sFLASH_WriteBuffer(buff,sector*FLASH_SECTOR_SIZE,  FLASH_SECTOR_SIZE);
		buff += FLASH_SECTOR_SIZE;
		sector++;
	}
     
    // 返回值转换
    return (res == 0)?RES_OK:RES_ERROR;
}

#endif /* _READONLY */

DRESULT disk_ioctl (BYTE drv, BYTE ctrl, void* buff)//哪个盘，控制命令（0刷新缓存，3获取块大小，1获取磁盘容量），块大小和磁盘容量存放的指针
{
    DRESULT res;

    if (drv)
    {
        return RES_PARERR;  //仅支持单磁盘操作，否则返回参数错误
    }

    //FATFS目前版本仅需处理CTRL_SYNC，GET_SECTOR_COUNT，GET_BLOCK_SIZ三个命令
    switch(ctrl)
    {
        case CTRL_SYNC: //刷新磁盘缓存
            //DRV_SdEnable();

           // res = (0 == DRV_SdWaitReady())?RES_OK:RES_ERROR;

             res = RES_OK;;
            break;

        case GET_SECTOR_SIZE:
            *(WORD*)buff = FLASH_SECTOR_SIZE;
            break;

        case GET_BLOCK_SIZE://获取块的大小
            *(WORD*)buff = FLASH_BLOCK_SIZE;
            res = RES_OK;
            break;

        case GET_SECTOR_COUNT: //获取磁盘的容量
            *(DWORD*)buff = FLASH_SECTOR_COUNT;
            res = RES_OK;
            break;
        default:
            res = RES_PARERR;
            break;
    }

    return res;
}

DWORD get_fattime (void)
{ 
}

