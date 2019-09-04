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

const char SoftVer[10] = "V2.82rc0";//��λ������汾
//���ⲿ���ڲ������汾��
//0x05:ֻ�����ⲿ�汾
//0x0a:�����ⲿ�汾���ڲ��汾��
extern PDU_VERSION_STRUCT PDU_Version;			//PDU�汾��Ϣ
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
//0x0a:�����ⲿ�汾���ڲ��汾��
 
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


//ʱ��: 980us/250�ֽ�
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
  * @brief  ��������д��ָ����ַ��Flash�� ��
  * @param  Address Flash��ʼ��ַ��
  * @param  Data ���ݴ洢����ʼ��ַ��
  * @param  DataNum �����ֽ�����
  * @retval ������д״̬��
  */
FLASH_Status ProgramDatatoFlash(unsigned int *Address,unsigned short *Data,unsigned int DataNum)
{
 

}

/**
  * @brief  ����ָ�����������Flash���� ��
  * @param  StartPage ��ʼ����
  * @param  EndPage ��������
  * @retval ��������״̬
  */
FLASH_Status ErasePage(unsigned int StartPage,unsigned int EndPage)
{

}

//�����ж��Ƿ����ϵ�loader, ������ϵ�loader,��ֱ������
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
// 				ErasePage(0,14);//ǰ14k��loader,�˴����;
// 				ProgramDatatoFlash((unsigned int *)(&FlashAddr),(unsigned short*)(Loader_p),sizeof(Loader_p));//ֱ����д
// 		//	}
// 		}
// 		if((pt[8]==0xed)&&(pt[12]==0xef)&&(pt[0x188]==0x05))
// 		{
// 			ErasePage(0,14);//ǰ14k��loader,�˴����;
// 			ProgramDatatoFlash((unsigned int *)(&FlashAddr),(unsigned short*)(Loader_p),sizeof(Loader_p));//ֱ����д
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

Input values:  k[4] 128λ��Կ

v[2]   ����ʱ64λ���ģ�����ʱ64λ����

Output values: v[2]    ����ʱ64λ���ģ�����ʱ64λ����

**********************************************************/
#define _ROUNDS 32  /*��������*/
#define _DELTA 0x9e3779b9 /* ��sqr(5)-1�� * 2^31 */
static void _tea(uint32 *k, uint32 *v, int N) //���NΪ��ֵ���ǽ��ܹ��̣���Ӧ��v��Ϊ���ģ���Կkһ������k[0]��k[1]��k[2]��k[3]�ĸ�Ԫ��
{

    uint32 y=v[0], z=v[1];                 //yΪ���Ļ����ĸ�32λ��zΪ���Ļ����ĵ�32λ

    uint32 limit,sum=0;                    //sumΪ���ֺ�

    if(N>0)
    {   /* ���ܹ���*/

        limit=_DELTA*N;

        while(sum!=limit) { //ע�⣺��λ�͵�λ�������㣬����sum�����ĵ���λ������Կ�Ĳ���ѡ��

            y+=((z<<4)^(z>>5)) + (z^sum) + k[sum&3];

            sum+=_DELTA;

            z+=((y<<4)^(y>>5)) + (y^sum) + k[(sum>>11)&3];

        }

    }
    else
    {   /* ���ܹ��� �����Ǽ����㷨�򵥵ķ�������*/

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

//���汾��
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
    uint32  Param_In[8];                         // IAP��ڲ���������
    uint32  Param_Out[8];                        // IAP���ڲ���������
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

    //��ֹ�����жϣ���ֹ��ʱ���ж�

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

    //�ж������Ƿ�Ϸ�
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
//     //�������кż�������
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
//     //�������кż�������
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
