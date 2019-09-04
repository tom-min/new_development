//#include <asf.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "code.h"
#include "usart1.h"
#include "stdlib.h"
#include "conf_PDU.h"
#include "ProtocolDrives.h"
 
#include "common.h"
#define  HardVersion 3
extern uint8_t EcuComRxBuf[];
typedef enum
{
	FLASH_BUSY = 1,
	FLASH_ERROR_PG,
	FLASH_ERROR_WRP,
	FLASH_COMPLETE,
	FLASH_TIMEOUT
}FLASH_Status;
 
extern uint32 timeflag, timeidle;
uint8_t WriteIDflag=0;

const char SoftVer[10] = "V2.82rc0";//下位机软件版本
//分外部和内部两个版本，
//0x05:只发送外部版本
//0x0a:发送外部版本和内部版本，
extern PDU_VERSION_STRUCT PDU_Version;			//PDU版本信息
//extern void blueresetcmddatesend(Command_t cmd,uint8_t *data,uint16_t data_size);
void GetVer(uint8 len)
{
	uint16 loop;
	uint16 ByteNum = 0;
	uint8_t buffer[256];
	PDU_COMMON_DATA_STRUCT CommonData;
	uint8_t *buffer_P = buffer;
	
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = 0x00;
    EcuComRxBuf[ByteNum++] = 0xf0;
	loop = sizeof(PDU_VERSION_STRUCT);
	  EcuComRxBuf[ByteNum++]=1;
	CommonData.channl  = 0;   //new
	CommonData.confirm = 0;
	CommonData.reserve2 = 0;
  
	EcuComRxBuf[ByteNum++] = CommonData.channl;		//new
	EcuComRxBuf[ByteNum++] = CommonData.confirm;
	EcuComRxBuf[ByteNum++] = CommonData.reserve2;
	EcuComRxBuf[ByteNum++] = SYSTEM_CONFIRM;
	EcuComRxBuf[ByteNum++] = DEVICE_RETURN_CODE_OK;
	
	memcpy(buffer_P,&PDU_Version,sizeof(PDU_VERSION_STRUCT));
    for(loop=0;loop<sizeof(PDU_VERSION_STRUCT); loop++)
    {
        EcuComRxBuf[ByteNum++] = buffer_P[loop];
    }
	ByteNum-=2;
	EcuComRxBuf[0] = ByteNum >> 8;
	EcuComRxBuf[1] = ByteNum & 0xff;
    UartTxQueuePack(EcuComRxBuf);
}
extern void printf_usb2(const char *fmt,...);
uint8 gswitchprotocol =0;   
//0x0a:发送外部版本和内部版本，
 
extern void bluewifithorughputstatusget(void);
extern uint8 gprintfecutoolenable;
extern uint8_t READ_BLUEINFO(uint8_t *ReadFlashBuf);
extern uint8_t Write_BLUEINFO(uint8_t *WriteFlashBuf);
void GetProtocolVer(uint8* pswitchprotocol)//a5 a5 00 05 D0 XX 00 00 00 07
{
	uint16 loop;
	uint16 i;
	uint16 ByteNum = 0;
	EcuComRxBuf[ByteNum++] = 0x00;
	EcuComRxBuf[ByteNum++] = 0x00;
	EcuComRxBuf[ByteNum++] = 0xD0;
	 
	gswitchprotocol=*pswitchprotocol;
	EcuComRxBuf[ByteNum++] = gswitchprotocol;//QU
	 

	if(gswitchprotocol == 1)
	{
		 
	}
	else if(gswitchprotocol == 0)
	{
		 
	}

	EcuComRxBuf[ByteNum++] = 0x00;
	EcuComRxBuf[ByteNum++] = 0x00;
	EcuComRxBuf[ByteNum++] = 0x00; 

	EcuComRxBuf[ByteNum++] = 0x01;
	EcuComRxBuf[ByteNum++] = 0x01;
	

	ByteNum -= 2;
	EcuComRxBuf[0] = ByteNum >> 8;
	EcuComRxBuf[1] = ByteNum & 0xff;
	UartTxQueuePack(EcuComRxBuf);
}


//时间: 980us/250字节
uint16 GetCrc160(uint8 *pSource, uint16 len)
{
    uint8  i;
    uint16 crc=0;

    while(len--)
    {
        for(i=0x80; i!=0; i>>=1)
        {
            if((crc&0x8000)!=0)
            {
                crc=(crc<<1)^0x1021;
            }
            else
                crc<<=1;
            if((*pSource&i)!=0) crc^=0x1021;

        }
        pSource++;
    }
    return(crc);
}

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  将数据烧写到指定地址的Flash中 。
  * @param  Address Flash起始地址。
  * @param  Data 数据存储区起始地址。
  * @param  DataNum 数据字节数。
  * @retval 数据烧写状态。
  */
FLASH_Status ProgramDatatoFlash(unsigned int *Address,unsigned short *Data,unsigned int DataNum)
{
 

}

/**
  * @brief  擦出指定扇区区间的Flash数据 。
  * @param  StartPage 起始扇区
  * @param  EndPage 结束扇区
  * @retval 扇区擦出状态
  */
FLASH_Status ErasePage(unsigned int StartPage,unsigned int EndPage)
{

}

//用来判断是否是老的loader, 如果是老的loader,则直接升级
// void  Adjust_Loader(void)
// {
//     uint8 *pt;
// 	//	uint8 i,j;
//     uint32 FlashAddr=FLASH_ADDRESS_START;
//     //uint8   tempBuf[256];
// // while(1)
// // {pt =(unsigned char *)FlashAddr;
// // 		for(i=0;i<16;i++)
// // 	{
// // 		j=*pt++;
// // 		USART_SendByte(j);
// // 	}
// // }
// 		
// 		pt =(unsigned char *)FlashAddr;
// 		if((pt[8]==0x01)&&(pt[12]==0x03))
// 		{
// // 		if(*pt++==0x01)
// // 		{
// // 			pt+=3;
// // 			if(*pt==0x03)
// // 			{
// 				ErasePage(0,14);//前14k是loader,此处清掉;
// 				ProgramDatatoFlash((unsigned int *)(&FlashAddr),(unsigned short*)(Loader_p),sizeof(Loader_p));//直接烧写
// 		//	}
// 		}
// 		if((pt[8]==0xed)&&(pt[12]==0xef)&&(pt[0x188]==0x05))
// 		{
// 			ErasePage(0,14);//前14k是loader,此处清掉;
// 			ProgramDatatoFlash((unsigned int *)(&FlashAddr),(unsigned short*)(Loader_p),sizeof(Loader_p));//直接烧写
// 		}
// }

void addr08_return (void)
{ 
	uint8 *pt;
	uint8 i,j;
	uint32 FlashAddr=FLASH_ADDRESS_START;
	pt =(unsigned char *)FlashAddr+8;
	i=*pt;
	pt+=4;
	j=*pt;
	//quwenqi UARTAnswerErr(i,j);
	pt=(unsigned char *)FlashAddr+0x188;
	i=*pt++;
	j=*pt;
	//quwenqi UARTAnswerErr(i,j);
}
/**********************************************************

Input values:  k[4] 128位密钥

v[2]   加密时64位明文，解密时64位密文

Output values: v[2]    加密时64位密文，解密时64位明文

**********************************************************/
#define _ROUNDS 32  /*加密轮数*/
#define _DELTA 0x9e3779b9 /* （sqr(5)-1） * 2^31 */
static void _tea(uint32 *k, uint32 *v, int N) //如果N为负值就是解密过程，相应的v就为密文，密钥k一共就有k[0]、k[1]、k[2]、k[3]四个元素
{

    uint32 y=v[0], z=v[1];                 //y为明文或密文高32位，z为明文或密文低32位

    uint32 limit,sum=0;                    //sum为部分和

    if(N>0)
    {   /* 加密过程*/

        limit=_DELTA*N;

        while(sum!=limit) { //注意：高位和地位交叉运算，利用sum操作的低两位进行密钥的部分选择

            y+=((z<<4)^(z>>5)) + (z^sum) + k[sum&3];

            sum+=_DELTA;

            z+=((y<<4)^(y>>5)) + (y^sum) + k[(sum>>11)&3];

        }

    }
    else
    {   /* 解密过程 ，就是加密算法简单的反向运算*/

        sum=_DELTA*(-N);

        while(sum) {

            z-=((y<<4)^(y>>5)) + (y^sum) + k[(sum>>11)&3];

            sum-=_DELTA;

            y-=((z<<4)^(z>>5)) + (z^sum) + k[sum&3];

        }

    }

    v[0]=y;
    v[1]=z;
}

//读版本号
//a5 a5 00 05 f2 52 64 69 73 76
void  Read_Ver(uint8 *InsBuf)
{
    uint8 *pt,i,j;
    uint8  length=MAX_SIZE_SNUM;
    //uint8   tempBuf[256];
    uint32 FlashAddr=FLASH_ADDRESS_SERIAL;
    pt =(unsigned char *)FlashAddr+9;

    if(InsBuf[1] == 0x05 && InsBuf[3] == 'R' && InsBuf[4] == 'd' && InsBuf[5] == 'i' && InsBuf[6] == 's')
    {
        i = 0;
        EcuComRxBuf[i++] = 0x00;
        EcuComRxBuf[i++] = 0x11;
        EcuComRxBuf[i++] = 0xfa;
        for(j = 0; j < length; j++)
        {
            EcuComRxBuf[i++] =  *pt++;
        }
        UartTxQueuePack(EcuComRxBuf);
    }
}
void  Read_bl_addr(void)
{
    uint8 *pt,i,j;
    pt =(unsigned char *)FLASH_ADDRESS_UPFLAG+0X2d;
    i = 0;
    EcuComRxBuf[i++] = 0x00;
    EcuComRxBuf[i++] = 0x8;
		EcuComRxBuf[i++] = 0xfC;
		EcuComRxBuf[i++] = 0x12;
		for(j = 0; j < 6; j++)
		{
				EcuComRxBuf[i++] =  *pt++;
		}
  //  EcuComRxBuf[1] = 8;
		UartTxQueuePack(EcuComRxBuf);
}

void Write_Ver(uint8 *InsBuf)
{
    uint8 StartPage;
    uint32 PageSize=PAGE_SIZE;
    uint32 StartAddr=FLASH_ADDRESS_START;
    uint32 FlashAddr=FLASH_ADDRESS_SERIAL;
    uint32  Param_In[8];                         // IAP入口参数缓冲区
    uint32  Param_Out[8];                        // IAP出口参数缓冲区
    uint8   tempBuf[256];
    int i;
    uint8 len;
    uint8 *pt;
    uint16 Crc;

    WriteIDflag++;
    if(WriteIDflag>5)
    {
        WriteIDflag=2;
        return;
    }

    //禁止串口中断，禁止定时器中断

    srand(timeidle);
    for(i=0; i<8; i++)
    {
        tempBuf[i]=rand();
        //quwenqi USART_SendByte(tempBuf[i]);
    }

    //key
    Param_In[0]=0x09875321;
    Param_In[1]=0x15698670;
    Param_In[2]=0x18324794;
    Param_In[3]=0x85296059;
    Param_Out[0]=(tempBuf[0]<<24)+(tempBuf[1]<<16)+(tempBuf[2]<<8)+tempBuf[3];
    Param_Out[1]=(tempBuf[4]<<24)+(tempBuf[5]<<16)+(tempBuf[6]<<8)+tempBuf[7];
    _tea(&Param_In[0], &Param_Out[0],_ROUNDS);
	
   //quwenqi len=USART1_ReceiveData(1000);
	
    tempBuf[0]=len;
    for(i=0; i<len; i++)
    {
        //quwenqi  tempBuf[1+i]=USART1_ReceiveData(100);
    }
    Crc=GetCrc160(&tempBuf[1],len-2);
    if(tempBuf[len-1]==(Crc>>8)&&tempBuf[len]==(Crc&0xff))
    {

    }
    else
    {
        //quwenqi USART_SendByte(0x56);
        return;
    }

    //判断数据是否合法
    pt=(uint8 *)Param_Out;
    for(i=0; i<8; i++)
    {
        if(tempBuf[i+1]!=pt[i])
        {
            //quwenqi  USART_SendByte(0x55);
            return;
        }
    }

    StartPage = (FlashAddr-StartAddr)/PageSize;
    if(ErasePage(StartPage,StartPage)!=0)
    {
        ProgramDatatoFlash((unsigned int *)(&FlashAddr),(unsigned short*)(tempBuf), 256);
        //quwenqi  USART_SendByte(0xaa);
				return;
    }
    //quwenqi  USART_SendByte(0x54);
}

void Init_WatchDog(void)
{
  

}


void IWDG_Feed(void)
{
	
}

// uint8_t Encrypt_Key1[]={0x3E,0x85,0xB3,0x21,0x99,0xC8,0x92,0xE7,0xA5,0xF4,0x95,0x33,0x7E,0x8A,0x07,0x13,0xFB,0x9C,0x7A,0xA7,0x03,0x2A,0xDC,0x75};
// ElemType plainBlock[8],cipherBlock[8],keyBlock[8];
// ElemType bKey[64];
// ElemType subKeys[16][48];

// int XOR_DecryptBlock(ElemType cipherBlock[8], ElemType Encrypt_Key[256], ElemType Encrypt_Pos)
// {
//     int i;
//     for(i=0; i<8; i++)
//     {
//         cipherBlock[i] ^= Encrypt_Key[i+Encrypt_Pos];
//     }
//     return TRUE;
// }

// int DES_Encrypt_UpdateKey(ElemType *rxbuf, uint16 rlen)
// {
//     int i;
//     //uint32 SN=ReadSn();
//     uint32 SN=0x446C5496;
//     ElemType *Encrypt_Key = Encrypt_Key1;
//     ElemType key;
//     if((rlen%8) > 0)
//     {
//         return 0;
//     }
//     memcpy(keyBlock,Encrypt_Key,8);
//     //加入序列号计算种子
//     for(i=0; i<8; i++)
//     {
//         key = (SN>>(i*4))*7;
//         keyBlock[i] ^= (Encrypt_Key[15+i]^key);
//     }
//     Char8ToBit64(keyBlock,bKey);
//     DES_MakeSubKeys(bKey,subKeys);

//     for(i=0; i<rlen; )
//     {
//         memcpy(plainBlock,rxbuf+i,8);
//         XOR_DecryptBlock(plainBlock, Encrypt_Key, i%17);
//         DES_EncryptBlock(plainBlock,subKeys,cipherBlock);
//         memcpy(rxbuf+i,cipherBlock,8);
//         i += 8;
//     }
//     return rlen;
// }

// int DES_Decrypt_UpdateKey(ElemType *rxbuf, uint16 rlen)
// {
//     int i;
//     //uint32 SN=ReadSn();
//     uint32 SN=0x446C5496;
//     ElemType *Encrypt_Key = Encrypt_Key1;
//     ElemType key;
//     if((rlen%8) > 0)
//     {
//         return 0;
//     }
//     memcpy(keyBlock,Encrypt_Key,8);
//     //加入序列号计算种子
//     for(i=0; i<8; i++)
//     {
//         key = (SN>>(i*4))*7;
//         keyBlock[i] ^= (Encrypt_Key[15+i]^key);
//     }
//     Char8ToBit64(keyBlock,bKey);
//     DES_MakeSubKeys(bKey,subKeys);

//     for(i=0; i<rlen; )
//     {
//         memcpy(cipherBlock,rxbuf+i,8);
//         DES_DecryptBlock(cipherBlock,subKeys,plainBlock);
//         XOR_DecryptBlock(plainBlock, Encrypt_Key, i%17);
//         memcpy(rxbuf+i,plainBlock,8);
//         i += 8;
//     }
//     return rlen;
// }
