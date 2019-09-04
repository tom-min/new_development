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
DSTATUS disk_initialize (unsigned char drv)//��������ĸ�����0Ϊ��һ���������ص��ǿ���״̬ ��ʼ���ɹ����޿�����ʼ��ʧ��
{
    u8 state;

    if(drv)
    {
        return STA_NOINIT;  //��֧�ִ���0�Ĳ���  0x01
    }

    state = 0;//DRV_SdInit();//20160107

    return (0 == state)? 0 : STA_NODISK;
}

DSTATUS disk_status (BYTE drv)//��������ĸ�����0Ϊ��һ�����������ݲ�֧�֣����ص��ǿ���״̬ ��ʼ���ɹ����޿�����ʼ��ʧ��
{
    if(drv)
    {
        return STA_NOINIT;  //��֧�ִ���0����
    }

    //���SD���Ƿ����
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
        return RES_PARERR;  //��֧�ֵ����̲�����count���ܵ���0�����򷵻ز�������
    }

   // if(0 == DRV_SdInsertGet())
    {
        //return RES_NOTRDY;  //û�м�⵽SD������NOT READY����
    }
    for(i=0;i<count;i++)
	{
		res = sFLASH_ReadBuffer( buff,sector*FLASH_SECTOR_SIZE, FLASH_SECTOR_SIZE);
		buff += FLASH_SECTOR_SIZE;
		sector++;
	}
   buff -= FLASH_SECTOR_SIZE;  
    //������ֵ����SPI_SD_driver.c�ķ���ֵת��ff.c�ķ���ֵ
    return (res == 0x00)?RES_OK:RES_ERROR;

#endif

}

#if _READONLY == 0 //�Ƿ�Ϊֻ��ģʽ

DRESULT disk_write (BYTE pdrv, const BYTE* buff, DWORD sector, UINT count)
{
	u8 res;
	u8  i;
	if (pdrv || !count)
	{
		return RES_PARERR;  //��֧�ֵ����̲�����count���ܵ���0�����򷵻ز�������
	}

	// if(0 == DRV_SdInsertGet())
	{
	//  return RES_NOTRDY;  //û�м�⵽SD������NOT READY����
	}

    // ��д����
   //res = DRV_SdWriteMultiBlock(sector, (unsigned char *)buff, count);
	for(i=0;i<count;i++)
	{
		sFLASH_EraseSector(sector*FLASH_SECTOR_SIZE);
		res = sFLASH_WriteBuffer(buff,sector*FLASH_SECTOR_SIZE,  FLASH_SECTOR_SIZE);
		buff += FLASH_SECTOR_SIZE;
		sector++;
	}
     
    // ����ֵת��
    return (res == 0)?RES_OK:RES_ERROR;
}

#endif /* _READONLY */

DRESULT disk_ioctl (BYTE drv, BYTE ctrl, void* buff)//�ĸ��̣��������0ˢ�»��棬3��ȡ���С��1��ȡ���������������С�ʹ���������ŵ�ָ��
{
    DRESULT res;

    if (drv)
    {
        return RES_PARERR;  //��֧�ֵ����̲��������򷵻ز�������
    }

    //FATFSĿǰ�汾���账��CTRL_SYNC��GET_SECTOR_COUNT��GET_BLOCK_SIZ��������
    switch(ctrl)
    {
        case CTRL_SYNC: //ˢ�´��̻���
            //DRV_SdEnable();

           // res = (0 == DRV_SdWaitReady())?RES_OK:RES_ERROR;

             res = RES_OK;;
            break;

        case GET_SECTOR_SIZE:
            *(WORD*)buff = FLASH_SECTOR_SIZE;
            break;

        case GET_BLOCK_SIZE://��ȡ��Ĵ�С
            *(WORD*)buff = FLASH_BLOCK_SIZE;
            res = RES_OK;
            break;

        case GET_SECTOR_COUNT: //��ȡ���̵�����
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

