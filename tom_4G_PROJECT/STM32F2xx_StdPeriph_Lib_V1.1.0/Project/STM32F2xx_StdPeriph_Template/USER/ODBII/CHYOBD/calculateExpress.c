/*******************************************************************************
* FILENAME  :   calculateExpress.c
* AUTHOR    :   fengjl026@126.com
* DATA      :   2007-1-3
* MODIFY    :   fengjl
* HISTORY   :   2007-1-3 by fengjl 创建
* DESC      :   该模块主要是用于计算数据流文件公式，还有一些用于数值计算的基本的函数
* COPYRIGHT :   深圳永辉昌 2007
*******************************************************************************/
#include "config.h"
#include "calculateExpress.h"
#include "ezxml.h"
#include "command.h"

#ifdef __cplusplus
extern "C" {
#endif 


static char result_mem[20];		//有的数据流结果要记录下来,其它数据流计算时要用到它的结果
extern struct SERIAL_COMMON_DATAEX g_commondata;
/*----------------------------------------------------------------------------------------
| 作用：比较16进制串是否相等
| 返回：不相等返回1，相等返回0
| 参数：hex1 第一个比较的16进制数组，hex2第二个数组，size比较的长度
|-----------------------------------------------------------------------------------------*/
int cmpnHex(const char *hex1, const U8 *hex2, int size)
{
	int i;

	if(hex2 == NULL)
		return 1;
	
	if((int)strlen(hex1) < (size *3 - 1))
		return 1;

	for(i=0;i<size;i++)
	{
		if(hex1[i*3] == 'X' && hex1[i*3+1] == 'X')
		{
			continue;
		}
		if(getHex(hex1+i*3) != hex2[i])
		{
			return 1;
		}
	}

	return 0;
}

int getCharEx(const char buf[256], char u8buf[256][16])
{
	int len = 1;
	int i=0;
	int j=0;
	char strbuf[256][16] = {0};
	if(!strlen(buf))
	{
		return 0;
	}
	for(j=0;j<strlen(buf);j++)
	{
		if(buf[j] == 0x2c)
		{
			j++;
			len++;
			i = 0;
		}
		strbuf[len-1][i++] = buf[j];
	}
	for(i = 0; i< 256; i++)
	{
		memcpy(u8buf[i],strbuf[i],sizeof(u8buf[i]));
	}
	return len;
}

U8 getHexEx(const char* buf, U8 *u8buf)
{
	char *p;
	U8 len = 0;

	p = (char*)buf;

	while(p)
	{
		if(strlen(p)>0)
			u8buf[len++] = getHex(p);
		p = strstr(p, ",");
		if(p)
		{
			p++;
			if(strcmp(p,"")==0)
			 break;
		}
	}

return len;
}
/*----------------------------------------------------------------------------------------
| 作用：转换字符串到16进制数据，使用注意，只做了空判断，长度判断,不能自动识别"0x" ","等符号
|       只做简单的判断,仅识别buf的前两位字符
| 返回：返回一个字节的16进制数,失败的情况也返回 0
| 参数：buf 16进制数据的字符串
|-----------------------------------------------------------------------------------------*/
U8 getHex(const char *buf)
{
	U8 hex,hex2;
	if(buf == NULL)
		return 0;

	if((strlen(buf) < 2) || ((strstr(buf, ",")) && (strstr(buf, ",") -buf < 2)))// || (strstr(buf, ",") -buf) < 2)
	{
		switch(buf[0])
		{
		case '1': hex= 0x01; break;
		case '2': hex= 0x02; break;
		case '3': hex= 0x03; break;
		case '4': hex= 0x04; break;
		case '5': hex= 0x05; break;
		case '6': hex= 0x06; break;
		case '7': hex= 0x07; break;
		case '8': hex= 0x08; break;
		case '9': hex= 0x09; break;
		case '0': hex= 0x00; break;
		case 'a': 
		case 'A': hex= 0x0a; break;
		case 'b': 
		case 'B': hex= 0x0b; break;
		case 'c': 
		case 'C': hex= 0x0c; break;
		case 'd': 
		case 'D': hex= 0x0d; break;
		case 'e': 
		case 'E': hex= 0x0e; break;
		case 'f': 
		case 'F': hex= 0x0f; break;
		default: return 0;
		}
		return hex;
	}

	switch(buf[0])
	{
	case '1': hex= 0x10; break;
	case '2': hex= 0x20; break;
	case '3': hex= 0x30; break;
	case '4': hex= 0x40; break;
	case '5': hex= 0x50; break;
	case '6': hex= 0x60; break;
	case '7': hex= 0x70; break;
	case '8': hex= 0x80; break;
	case '9': hex= 0x90; break;
	case '0': hex= 0x00; break;
	case 'a': 
	case 'A': hex= 0xa0; break;
	case 'b': 
	case 'B': hex= 0xb0; break;
	case 'c': 
	case 'C': hex= 0xc0; break;
	case 'd': 
	case 'D': hex= 0xd0; break;
	case 'e': 
	case 'E': hex= 0xe0; break;
	case 'f': 
	case 'F': hex= 0xf0; break;
	default: return 0;
	}
	
	switch(buf[1])
	{
	case '1': hex2= 0x01; break;
	case '2': hex2= 0x02; break;
	case '3': hex2= 0x03; break;
	case '4': hex2= 0x04; break;
	case '5': hex2= 0x05; break;
	case '6': hex2= 0x06; break;
	case '7': hex2= 0x07; break;
	case '8': hex2= 0x08; break;
	case '9': hex2= 0x09; break;
	case '0': hex2= 0x00; break;
	case 'a': 
	case 'A': hex2= 0x0a; break;
	case 'b': 
	case 'B': hex2= 0x0b; break;
	case 'c': 
	case 'C': hex2= 0x0c; break;
	case 'd': 
	case 'D': hex2= 0x0d; break;
	case 'e': 
	case 'E': hex2= 0x0e; break;
	case 'f': 
	case 'F': hex2= 0x0f; break;
	default: return 0;
	}

	return hex+hex2;
}

/*------------------------------------------------------------------------------------------------------
| 作用：计算字符串公式，公式只能有加减乘除运算，可以用括号括数，但是不支持括号中的公式
| 返回：返回公式计算结果
| 参数：strbuf,公式表达式，只支持10进制数运算
--------------------------------------------------------------------------------------------------------*/
double calculateExpress(char *strbuf)
{
	double oldnum=0;
	double newnum;
	char buf[256];
	char buf1[256];
	//char strnum[256];
	int i =0;
	//int nexttype = 0;
	U32 itemp;
	double jtemp;
	memset(buf,0,256);
	memset(buf1,0,256);
	if(getnexttype(strbuf) == 0)
		return atof(strbuf);

	while(strbuf[i])
	{
		switch(strbuf[i])
		{
		case '+':
			if(getnexttype(strbuf+i+1) < 3)
			{
				//可以计算并替换
				strcpy(buf,strbuf+i+strnumlen(strbuf+i+1)+1);
				newnum = atof(strbuf + i +1)+oldnum;
				sprintf(strbuf,"%f",newnum);
				strcat(strbuf,buf);
				return calculateExpress(strbuf);
			}
			i++;
			break;
		case '-':
			if(i==0)
			{
				oldnum = atof(strbuf+i);
			i += strnumlen(strbuf+i);
			}
			else
			{
				if(getnexttype(strbuf+i+1) < 3)
				{
					//可以计算替换
					if(i==0)
					{
					//sscanf(strbuf,"%f",&oldnum);
						oldnum=atof(strbuf);
						i=strnumlen(strbuf);
						break;
					}
					strcpy(buf,strbuf+i+strnumlen(strbuf+i+1)+1);
					newnum = oldnum - atof(strbuf + i +1);
					sprintf(strbuf,"%f",newnum);			//加和减的优先级比较低，算到此处说明前面的算法已全算完，可以继续往下算
					//sprintf(strbuf+i-strrnumlen(strbuf+i),"%f",newnum);
					strcat(strbuf,buf);				
					return calculateExpress(strbuf);
				}
				i++;
			}
			
			break;
		case '%':
			//计算
			if(getnexttype(strbuf+i+1) < 5)
			{
			
			strcpy(buf,strbuf+i+strnumlen(strbuf+i+1)+1);
			newnum = (int)oldnum%atoi(strbuf + i +1);
			sprintf(strbuf+i-strrnumlen(strbuf+i,i),"%f",newnum);
			strcat(strbuf,buf);
			
			return calculateExpress(strbuf);
			}
			i++;
			break;
		case '*':
			//计算
			if(getnexttype(strbuf+i+1) < 5)
			{
			
			strcpy(buf,strbuf+i+strnumlen(strbuf+i+1)+1);
			newnum = atof(strbuf + i +1)*oldnum;
			sprintf(strbuf+i-strrnumlen(strbuf+i,i),"%f",newnum);
			strcat(strbuf,buf);
			return calculateExpress(strbuf);
			}
			i++;
			break;
		case '/':
			//计算
			if(getnexttype(strbuf+i+1) < 5)
			{strcpy(buf,strbuf+i+strnumlen(strbuf+i+1)+1);
				sscanf(strbuf+i+1,"%f",&jtemp);
			if(atof(strbuf+i+1)>0)
				newnum = oldnum /atof(strbuf+i+1);///getDec(strbuf + i +1);
			else
				newnum = 0;
			sprintf(strbuf+i-strrnumlen(strbuf+i,i),"%f",newnum);
			strcat(strbuf,buf);
			return calculateExpress(strbuf);
			}
			i++;
			break;
		case '(':
			if(!strstr(strbuf+i+1,"("))
			{
				memcpy(buf,strbuf+i+1,strstr(strbuf+i+1,")")-strbuf-i-1);
				calculateExpress(buf);
				strcpy(buf1,strstr(strbuf+i+1,")")+1);
				strcpy(strbuf+i,buf);
				//strbuf+i=buf;
				strcat(strbuf,buf1);
				return calculateExpress(strbuf);
			}
			i++;
			break;
		case ')':
			i++;
			break;
		case '^':
			if(getnexttype(strbuf+i+1)<2)
			{
				//可以计算并替换
				strcpy(buf,strbuf+i+strnumlen(strbuf+i+1)+1);
				sscanf(strbuf+i+1,"%X",&itemp);
				newnum =(int)itemp^(int)oldnum;//getHex(strbuf + i +1)^(int)oldnum;
				sprintf(strbuf,"%f",newnum);
				strcat(strbuf,buf);
				
				return calculateExpress(strbuf);
			}
			i++;
			break;
		case'&':

			if(getnexttype(strbuf+i+1)<2)
			{
				//可以计算并替换
				strcpy(buf,strbuf+i+strnumlen(strbuf+i+1)+1);
				sscanf(strbuf+i+1,"%X",&itemp);
				newnum =(int)itemp&(int)oldnum;//getHex(strbuf + i +1)&(int)oldnum;
				sprintf(strbuf,"%f",newnum);
				strcat(strbuf,buf);
				
				return calculateExpress(strbuf);
			}
			i++;
			break;
		case '|':
			if(getnexttype(strbuf+i+1)<2)
			{
				//可以计算并替换
				strcpy(buf,strbuf+i+strnumlen(strbuf+i+1)+1);
				sscanf(strbuf+i+1,"%X",&itemp);
				newnum = (int)itemp|(int)oldnum;//getHex(strbuf + i +1)|(int)oldnum;
				sprintf(strbuf,"%f",newnum);
				strcat(strbuf,buf);
				return calculateExpress(strbuf);
			}
			i++;
			break;
		case '>':
            if(strbuf[i+1] == '=')
            {
                sscanf(strbuf+i+2,"%X",&itemp);
			    if(oldnum>=itemp)//getHex(strbuf + i +1))
				    return 1;
			    else
				    return 0;
            }
            else
            {
			    sscanf(strbuf+i+1,"%X",&itemp);
			    if(oldnum>itemp)//getHex(strbuf + i +1))
				    return 1;
			    else
				    return 0;
            }
			break;
		case '<':
            if(strbuf[i+1] == '=')
            {
                sscanf(strbuf+i+2,"%X",&itemp);
			    if(oldnum<=itemp)//<getHex(strbuf + i +1))
				    return 1;
			    else
				    return 0;
            }
            else
            {
			sscanf(strbuf+i+1,"%X",&itemp);
			if(oldnum<itemp)//<getHex(strbuf + i +1))
			{
				
				return 1;
			}
			else
				return 0;
            }
			break;
		case '=':
			
			sscanf(strbuf+i+1,"%X",&itemp);
			if(oldnum==itemp)//==getHex(strbuf + i +1))
				return 1;
			else
				return 0;
			break;
		case '!':
			sscanf(strbuf+i+2,"%X",&itemp);
			if(oldnum==itemp)//getHex(strbuf + i +1))
				return 0;
			else
				return 1;
			break;

		default:
			if(strnumlen(strbuf) == strlen(strbuf))
				return atof(strbuf);
			
				//sscanf(strbuf+i,"%X",&oldnum);				 
			oldnum = atof(strbuf+i);
			i += strnumlen(strbuf+i);
		}
	}
	return 0;
}

double calculateExpress1(char *strbuf)
{
	double oldnum=0;
	char buf[256];
	char buf1[256];
	//char strnum[256];
	int i =0;
	//int nexttype = 0;
	double itemp;
	memset(buf,0,256);
	memset(buf1,0,256);
	if(getnexttype(strbuf) == 0)
		return atof(strbuf);

	while(strbuf[i])
	{
		switch(strbuf[i])
		{
		case '>':
            if(strbuf[i+1] == '=')
            {
             //   sscanf(strbuf+i+2,"%X",&itemp);                 //xxg20080604  将itemp与oldnum变为同一类型比较. 如X00Y01为80,80,以前的比较为128与0X128比较.
				itemp = atof(strbuf+i+2);
			    if(oldnum>=itemp)//getHex(strbuf + i +1))
				    return 1;
			    else
				    return 0;
            }
            else
            {
			   // sscanf(strbuf+i+1,"%X",&itemp);
				itemp = atof(strbuf+i+1);
			    if(oldnum>itemp)//getHex(strbuf + i +1))
				    return 1;
			    else
				    return 0;
            }
			break;
		case '<':
            if(strbuf[i+1] == '=')
            {
              //  sscanf(strbuf+i+2,"%X",&itemp);
				itemp = atof(strbuf+i+2);
			    if(oldnum<=itemp)//<getHex(strbuf + i +1))
				    return 1;
			    else
				    return 0;
            }
            else
            {
		//	sscanf(strbuf+i+1,"%X",&itemp);
			itemp = atof(strbuf+i+1);                  
			if(oldnum<itemp)//<getHex(strbuf + i +1))
			{
				
				return 1;
			}
			else
				return 0;
            }
			break;
		case '=':
			
			//sscanf(strbuf+i+1,"%X",&itemp);
			itemp = atof(strbuf+i+1);
			if(oldnum==itemp)//==getHex(strbuf + i +1))
				return 1;
			else
				return 0;
			break;

		default:
			if(strnumlen(strbuf) == strlen(strbuf))
				return atof(strbuf);
			
				//sscanf(strbuf+i,"%X",&oldnum);				 
			oldnum = atof(strbuf+i);
			i += strnumlen(strbuf+i);
		}
	}
	return 0;
}
/*------------------------------------------------------------------------------------------------------
| 作用：返回当前字符串的第一个操作方式
| 返回：如果是"+"返回1 "-"返回2，"*"返回3" "/"返回4 如果没有操作方式，返回0；
| 参数：strbuf,要判断的字符串公式
--------------------------------------------------------------------------------------------------------*/
int getnexttype(const char *strbuf)
{
	int i =0;
	while(strbuf[i])
	{
		switch(strbuf[i])
		{
		case '+':
			if(i ==0)
			{
				i++;
				break;
			}

			return 2;
		case '-':
			if(i ==0)
			{
				i++;
				break;
			}

			return 2;
		case '*':
			return 3;
		case '/':
			return 4;
		case '%':
			return 4;
		case '(':
			return 5;

		case '&':
			return 1;
		case '|':
			return 1;
		case '^':
			return 1;
		case '>':
		case '<':
		case '=':
		case '!':
			return 1;
		//cheng 20090618 优先级的问题
		case ')':
			return 1;
		default:
			i++;
		}
	}
	return 0;
}

//将X00Y01的ASCII码转为字符显示，31 30显示10
char *formatExpress04(char *strbuf, U8 *buf)
{
	char buff[256];
	int len = 0;
	char *p = strbuf;
	memset(buff, 0, 256);

	while(1)
	{
		p = strstr(p, "[");
		if(p == NULL)
			break;
		buff[len] = buf[(U8)atoi(p+2)];
		if(buff[len] == 0x00)
			buff[len] = 0x20;
		len++;
	}

	memcpy(strbuf, buff, len);
	return strbuf;	
}

//根据X00Y01的数据取表中的字符串
char *formatExpress05(char *strbuf, U8 *buf, ezxml_t xml)
{
	U16 i;
	char *p;
	char *p1;
	ezxml_t node;
	U16 word = buf[0] *0x100 + buf[1];

	p = strstr(strbuf, "]");
	if(p == NULL)
		goto result;
	p = strstr(p , "]");
	if(p == NULL)
		goto result;
	node = ezxml_child(xml, p);
	if(node == NULL)
		goto result;
	p = (char*)ezxml_txt(node);
	for(i =0; i< word; i++)
	{
		p = strstr(p,",");
		if(p == NULL)
			goto result;
	}

	p1 = strstr(p, ",");
	if(p1 == NULL)
	{
		strcpy(strbuf, p);
		return strbuf;
	}

	strncpy(strbuf, p, p1-p);
	strbuf[p1-p] = '\0';
	return strbuf;

result:
	strbuf[0] = '\0';
	return strbuf;

}

//取数据Y01的二进制位，受X00控制，在X00为0时空格，在1时有效
char *formatExpress06(char *strbuf, U8 *buf)
{
	U8 x;
	U8 y;
	int i;
	U8 temp;
	//char *p = strstr(strbuf, "[X");
	//if(p)
	//	x = getHex(p+2);
	//p = strstr(strbuf, "[Y");
	//memset(buf, 0, 80);

	//if(p)
	//	y = getHex(p+2);
	x = buf[0];
	y = buf[1];

	for(i=0; i<8; i++)
	{
		temp = 1;
		temp <<= (7-i);

		if(x & temp)
		{
			if( y & temp)
			{
				strbuf[i] = '1';
			}
			else
			{
				strbuf[i] = '0';
			}
		}
		else
		{
			strbuf[i] = '_';
		}
	}

	strbuf[i] = '\0';
	return strbuf;
}
//将X00Y01的ASCII码转为字符显示，但能与04相比有所不同,04中间不带字符，本字节转换时可带其它字符31 30显示10
char *formatExpress11(char *strbuf, U8 *buf,int number)
{
	char buff[256];
	char *p = strbuf;
	int i;
	//long tempdata=0;
	char str[10];
	memset(buff, 0, 256);

	for(i=0;i<number;i++)
	{
		p = strstr(p+1, "X");
		if(p == NULL)
			break;
		buff[i]=buf[getHex(p+1)];
		if(buff[i]==0)
			buff[i]=0x20;
		if(strstr(p+1, "X")!= NULL &&(strstr(p+1,"X")-p)>3)
		{
			memset(str,0,sizeof(str));
			memcpy(str,p+3,strstr(p+1,"X")-p-3);
			strcat(buff,str);
			number += 1;
			i += 1;
		}
	}
	strcpy(strbuf,buff);
	return strbuf;	
}

//将X00Y01的ASCII码转为字符显示，但能与04相比有所不同,04中间不带字符，本字节转换时可带其它字符31 30显示10
//2A0C[X07X08X09X0AX0BX0CX08X09X0AX0BX0CX0D] 遇到2A结束
char *formatExpress2A(char *strbuf, U8 *buf,int number)
{
	char buff[256];
	char *p = strbuf;
	int i;
	//long tempdata=0;
	char str[10];
	memset(buff, 0, 256);

	for(i=0;i<number;i++)
	{
		p = strstr(p+1, "X");
		if(p == NULL)
			break;
		buff[i]=buf[getHex(p+1)];
		if(buff[i]<0x1f || buff[i]>0x7f)
		{
			buff[i] = 0x00;
			break;
		}
		if(buff[i] == 0x2a)
		{
			buff[i] = 0x00;
			break;
		}
		if(strstr(p+1, "X")!= NULL &&(strstr(p+1,"X")-p)>3)
		{
			memset(str,0,sizeof(str));
			memcpy(str,p+3,strstr(p+1,"X")-p-3);
			strcat(buff,str);
			number += 1;
			i += 1;
		}
	}
	strcpy(strbuf,buff);
	return strbuf;	
}

//将X00Y01十进制显示
char *formatExpress13(char *strbuf, U8 *buf,int number)
{
	char buff[256];
	char *p = strbuf;
	int i;
	long tempdata=0;
	char str[256];
	memset(buff, 0, 256);

	for(i=0;i<number;i++)
	{
		p = strstr(p+1, "X");
		if(p == NULL)
			break;
		tempdata=tempdata*256+buf[getHex(p+1)];

		if((strstr(p+1,"X")-p)!=3)
		{
			sprintf(str,"%d",tempdata);
			strcat(buff,str);
			tempdata=0;
			if(strstr(p+1, "X")!= NULL &&(strstr(p+1,"X")-p)>3)
			{
				memset(str,0,sizeof(str));
				memcpy(str,p+3,strstr(p+1,"X")-p-3);
				strcat(buff,str);
			}
		}
	}
	strcpy(strbuf,buff);
	return strbuf;	
}
//将X00Y01的16进制
char *formatExpress12(char *strbuf, U8 *buf,int number)
{
	char buff[256];
	char *p = strbuf;
	int i;
	//long tempdata=0;
	char str[10];
	memset(buff, 0, 256);

	for(i=0;i<number;i++)
	{
		p = strstr(p+1, "X");
		if(p == NULL)
			break;
		sprintf(str,"%02X",buf[getHex(p+1)]);
		strcat(buff,str);
		if(strstr(p+1, "X")!= NULL &&(strstr(p+1,"X")-p)>3)
		{
			memset(str,0,sizeof(str));
			memcpy(str,p+3,strstr(p+1,"X")-p-3);
			strcat(buff,str);
		}
	}
	strcpy(strbuf,buff);
	return strbuf;	
}


char *formatExpress30(char *strbuf, U8 *buf,int number)
{
	U8 buff[256];
	char *p = strbuf;
	int data0;
	int data1;
	//long tempdata=0;
	memset(buff, 0, 256);
	data0=buf[getHex(p+2)];
	data1=getHex(p+6);
	strbuf[6]='\0';
	sprintf((char *)buff,"%d%s%d",data0/data1,p+5,data0%data1);
	strcpy(strbuf,(char *)buff);
	return strbuf;	
}

/*
3101[X00X01X02]
年份: 1985+X02
月份: X01 %d显示
日  : X00/4 %d显示     
所有数据不能超出年月日的有效范围.
*/
char *formatExpress31(char *strbuf, U8 *buf,int number)
{
	char *p = strbuf;
	int day;
	int mon;
	int year;
	day=buf[getHex(p+2)]/4;
	mon=buf[getHex(p+5)];
	year=buf[getHex(p+8)] + 1985;
	sprintf(strbuf,"%d-%d-%d",year, mon, day);
	return strbuf;	
}

long formatExpressBits(char *strbuf, U8 *buf,int start)
{
	long loop, pos, end;
	long data = 0;
	if(strstr(strbuf, "X") && strstr(strbuf, "Y"))
	{
		pos = atoi(strstr(strbuf, "X")+1);
		end = atoi(strstr(strbuf, "Y")+1)+1;
	}
	for(loop=0;loop<end;loop++)
	{
		int bit, byte;
		if(loop<pos)
			continue;
		bit = loop%8;
		byte = loop/8;
		data = data<<1;
		if(((buf[byte]<<bit)&0x80) == 0x80)
		{
			data += 1;
		}
	}
	return data;
}

/*
知豆协议专用公式
按照bit位计算公式
CMP="4000[X24Y31]"
X:起始位 Y:结束位 
*/
char *formatExpress40(char *strbuf, U8 *buf,int start)
{
	char *pt1;
	char *pt2;
	char sbuf[512];
	char rbuf[512];
	long data;

	memset(sbuf,0x00,sizeof(sbuf));
	memset(rbuf,0x00,sizeof(rbuf));
	if(strstr(strbuf,"[") && strstr(strbuf,"]"))
	{
		pt1 = strstr(strbuf,"[");
		pt2 = strstr(strbuf,"]");
		strcpy(sbuf, strstr(strbuf,"]")+1);
		memcpy(rbuf, pt1, pt2-pt1+1);
		data = formatExpressBits(rbuf,buf,start);
		pt1 = strstr(strbuf,"[");
		//sprintf(p,"%d",data);
		//chengbb 20110222 大于0x80000000会溢出
		sprintf(pt1,"%u",data);
	}
	else
	{
		return strbuf;
	}
	strcat(strbuf,sbuf);
	if(strnumlen(strbuf) == strlen(strbuf))
		return strbuf;

	return formatExpress40(strbuf,buf,start);
}

/*------------------------------------------------------------------------------------------------------
| 作用：替换公式中的[]符号或者{}符号
| 返回：返回替换正确数据后的公式
| 参数：strbuf 包含{} [] 符号的公式
|		buf    查表的数据，注意没有边界检查
|		start  查表开始的数据起始地址
|       end	   buf 中的结束地址（绝对查表地址）
--------------------------------------------------------------------------------------------------------*/
char *formatExpress41(char *strbuf, U8 *buf, U8 start)
{
	//int len = (int)strlen(strbuf);
	char sbuf[1024];
	char sbuf1[128];
	//int i=0;
	char *p=strchr(strbuf,':');
	char *p0;
	if(!p)
		return 0;
	p0=strchr(p,';');
	memset(sbuf,0,sizeof(sbuf));
	while(p)
	{
		memset(sbuf1,0,sizeof(sbuf1));
		strncpy(sbuf1,strbuf,p-strbuf);
		formatExpress40(sbuf1,buf,start);
		strcat(sbuf,sbuf1);
		if(p0)
		{
			strncpy(sbuf+strlen(sbuf),p,p0-p+1);
			strcpy(strbuf,p0+1);
			p=strchr(strbuf,':');
			if(p)
				p0=strchr(p,';');
			else
				strcat(sbuf,strbuf);
		}
		else
		{
			strcat(sbuf,p);
			break;
		}


	}
	strcpy(strbuf,sbuf);
	return strbuf;
}

char * strserch(char *strbuf,char *type)
{
	char *p=strstr(strbuf,"X");
	char *p0=strstr(strbuf,"Y");
	char *p1=strstr(strbuf,"Z");
	int i,j,k,l;
	if(p)
		i=p-strbuf;
	else
		i=0xff;
	if(p0)
		j=p0-strbuf;
	else
		j=0xff;
	if(p1)
		k=p1-strbuf;
	else
		k=0xff;
	l=i<j ? i:j;
	l=l<k ? l:k;
	if(l==i)
	{	type[0]=0;
		return p;
	}
	else if(l==j)
	{	type[0]=1;
		return p0;
	}
	else
	{	type[0]=2;
		return p1;
	}

}
/*X03表示将第三个字节直接十六制显示
			   Y03表示以十六进制显示第三个字节的高半字节
			   Z03表示以十六进制显示第三个字节的低半字节*/
char *formatExpress14(char *strbuf, U8 *buf,int number)
{
	char buff[256];
	char *p = strbuf;
	char *p1;
	int i;
	U8 type;
	//long tempdata=0;
	char str[10];
	memset(buff, 0, 256);

	for(i=0;i<number;i++)
	{
		p = strserch(p+1,(char*)(&type));
		if(p == NULL)
			break;
		switch (type)
		{
			case 0:
				sprintf(str,"%02X",buf[getHex(p+1)]);
			break;
			case 2:
				sprintf(str,"%01X",buf[getHex(p+1)]/16);
			break;
			case 1:
				sprintf(str,"%01X",buf[getHex(p+1)]&0X0f);
			break;
		}
		strcat(buff,str);
			p1=strserch(p+1,(char*)(&type));
		if((p1!=NULL)&&((p1-p)>3))
		{
			memset(str,0,sizeof(str));
			memcpy(str,p+3,p1-p-3);
		    strcat(buff,str);
		}
	}
	strcpy(strbuf,buff);
	return strbuf;	
}

/*------------------------------------------------------------------------------------------------------
| 作用：替换公式中的[]符号或者{}符号
| 返回：返回替换正确数据后的公式
| 参数：strbuf 包含{} [] 符号的公式
|		buf    查表的数据，注意没有边界检查
|		start  查表开始的数据起始地址
|       end	   buf 中的结束地址（绝对查表地址）
--------------------------------------------------------------------------------------------------------*/
char *formatExpress(char *strbuf, U8 *buf, U8 start)
{
	//int len = (int)strlen(strbuf);
	char *p = strstr(strbuf,"[");
	char sbuf[512];
	long data;
	
	memset(sbuf,00,sizeof(sbuf));
	while(strstr(strbuf,"**RP**"))
	{
		memset(sbuf,00,sizeof(sbuf));
		memcpy(sbuf,strbuf,(strstr(strbuf,"**RP**")-strbuf));
		strcat(sbuf,result_mem);
		strcat(sbuf,strstr(strbuf,"**RP**")+6);
		strcpy(strbuf,sbuf);
	}
	p = strstr(strbuf,"[");
	if((p == NULL) || (strlen(p) < 2))
	{
		p = strstr(strbuf,"{");
		
		if ((p == NULL)||( strlen(p) < 2))
		{
			return strbuf;
		}
		
		if(getHex(p+2) < start)
			return NULL;

		//有符号数
		sprintf(sbuf,strstr(strbuf,"}")+1);
		p+=2;
		
		if(buf[getHex(p)] & 0x80)
		{
			data=-1;
			while(strstr(strbuf,"}")!=NULL && (strstr(strbuf,"}")-p)>0)
			{
				data=data<<8;
				data+=buf[getHex(p)];
				p+=3;
			}
			
		}
		else
		{
			data=0;
			while(strstr(strbuf,"}")!=NULL && (strstr(strbuf,"}")-p)>0)
			{
				data=data<<8;
				data+=buf[getHex(p)];
				p+=3;
			}
		}
		p = strstr(strbuf,"{");
		sprintf(p,"%d", data);
	}
	else
	{
		if(getHex(p+2) < start)
			return NULL;
		//无符号数
		strcpy(sbuf,strstr(strbuf,"]")+1);
		p+=2;
		data=0;
		while(strstr(strbuf,"]")!=NULL && (strstr(strbuf,"]")-p)>0)
		{
			data=data<<8;
			data+=buf[getHex(p)];
			p+=3;
		}
		p = strstr(strbuf,"[");
		//sprintf(p,"%d",data);
		//chengbb 20110222 大于0x80000000会溢出
		sprintf(p,"%u",data);
	}
	
	
	strcat(strbuf,sbuf);
	if(strnumlen(strbuf) == strlen(strbuf))
		return strbuf;
	
	return formatExpress(strbuf,buf,start);
}

//将X00Y01的ASCII码转为字符显示，但能与04相比有所不同,04中间不带字符，本字节转换时可带其它字符31 30显示10
char *formatExpressF1(char *strbuf, U8 *buf,int number)
{
	char buff[256];
	char *p = strbuf;
	int i;
	//long tempdata=0;
	char str[10];
	memset(buff, 0, 256);

	for(i=0;i<number;i++)
	{
		p = strstr(p+1, "X");
		if(p == NULL)
			break;
		if(buf[getHex(p+1)]==0x7e)
		{
			--i;
			--number;
			continue;
		}
		buff[i]=buf[getHex(p+1)];
		if(buff[i]==0)
			buff[i]=0x20;
		if(strstr(p+1, "X")!= NULL &&(strstr(p+1,"X")-p)>3)
		{
			memset(str,0,sizeof(str));
			memcpy(str,p+3,strstr(p+1,"X")-p-3);
			strcat(buff,str);
			number += 1;
			i += 1;
		}
	}
	strcpy(strbuf,buff);
	return strbuf;	
}
//将X00Y01的ASCII码转为字符显示，但能与04相比有所不同,04中间不带字符，本字节转换时可带其它字符31 30显示10
char *formatExpressE1(char *strbuf, U8 *buf,int number)
{
	char buff[256];
	char *p = strbuf;
	int i;
	//long tempdata=0;
	char str[10];
	memset(buff, 0, 256);

	for(i=0;i<number;i++)
	{
		p = strstr(p+1, "X");
		if(p == NULL)
			break;
		if(buf[getHex(p+1)]==0x00)
		{
			--i;
			--number;
			continue;
		}
		buff[i]=buf[getHex(p+1)];
		if(buff[i]==0)
			buff[i]=0x20;
		if(strstr(p+1, "X")!= NULL &&(strstr(p+1,"X")-p)>3)
		{
			memset(str,0,sizeof(str));
			memcpy(str,p+3,strstr(p+1,"X")-p-3);
			strcat(buff,str);
			number += 1;
			i += 1;
		}
	}
	strcpy(strbuf,buff);
	return strbuf;	
}
/*------------------------------------------------------------------------------------------------------
| 作用：替换公式中的[]符号或者{}符号
| 返回：返回替换正确数据后的公式
| 参数：strbuf 包含{} [] 符号的公式
|		buf    查表的数据，注意没有边界检查
|		start  查表开始的数据起始地址
|       end	   buf 中的结束地址（绝对查表地址）
--------------------------------------------------------------------------------------------------------*/
char *formatExpress01(char *strbuf, U8 *buf, U8 start)
{
	//int len = (int)strlen(strbuf);
	char sbuf[1024];
	char sbuf1[128];
	//int i=0;
	char *p=strchr(strbuf,':');
	char *p0;
	if(!p)
		return 0;
	p0=strchr(p,';');
	memset(sbuf,0,sizeof(sbuf));
	while(p)
	{
		memset(sbuf1,0,sizeof(sbuf1));
		strncpy(sbuf1,strbuf,p-strbuf);
		formatExpress(sbuf1,buf,start);
		strcat(sbuf,sbuf1);
		if(p0)
		{
			strncpy(sbuf+strlen(sbuf),p,p0-p+1);
			strcpy(strbuf,p0+1);
			p=strchr(strbuf,':');
			if(p)
				p0=strchr(p,';');
			else
				strcat(sbuf,strbuf);
		}
		else
		{
			strcat(sbuf,p);
			break;
		}
		
		
	}
	strcpy(strbuf,sbuf);
	return strbuf;
}
/*
先读表得出值，然后再参与计算。AUDI
*/
char *formatExpress20(char *strExpress, U8 *buf, const char* table,const char *unit,ezxml_t xmlroot)
{
	double result;
	char *p;
	char tabbuf[64];
	char tempbuf[256];
	char *p0;
	int i;
	int len;
	ezxml_t tabezxml;
	tabezxml=xmlroot;
	p0= strstr(strExpress,"TAB");	
	while(p = strstr(p0,"/"))
	{
		strncpy(tabbuf,p0,p-p0);
		tabbuf[p-p0] = '\0';
		tabezxml = ezxml_child(tabezxml,tabbuf);
		p0 = p+1;
	}
	tabezxml = ezxml_child(tabezxml,p0);
	table=ezxml_txt(tabezxml);
	memset(tempbuf,0,256);
	if(buf[1] < 0xa7)
	{
		tempbuf[0]=0;
	}
	else 
	{
		tempbuf[0]=buf[1]-0xa6;
		sprintf(tempbuf,"%d",tempbuf[0]);
	}

	result =table_express(tempbuf,(char*)table,0);
	//  p0=strstr(strExpress,"[X",6);
	p0=strstr(strExpress,"[Y");

	sprintf(tabbuf,"%f",result);
	len=(int)strlen(strExpress)-(int)strlen(p0);
	strcpy(tempbuf,strExpress);
	for(i=0;i<len+0;i++)
		tempbuf[i]=tempbuf[i];
	tempbuf[i]= '\0';
	strcat(tempbuf,tabbuf);
	memset(strExpress,0,512);
	strExpress[0]=0x30;
	strExpress[1]=0x30;
	strExpress[2]=0x30;
	strExpress[3]=0x30;
	strcat(strExpress,tempbuf);
	getCMPstrResult(strExpress,buf,NULL,NULL,xmlroot);
	if(unit)
		strcat(strExpress,unit);
	return strExpress;
}
/*------------------------------------------------------------------------------------------------------
| 作用：返回公式某位置上的离下一个计算符号的距离
| 返回：返回距离大小
| 参数：strbuf,公式中的位置
--------------------------------------------------------------------------------------------------------*/
int strnumlen(const char*strbuf)
{
	int i;
	
	for(i = 1; i<(int)strlen(strbuf);i++)
	{
		if(strbuf[i] == '.')
			continue;
		if(strbuf[i] > 0x2f && strbuf[i] < 0x3a)
			continue;
		if(strbuf[i] > 0x40 && strbuf[i] < 0x47)
			continue;
		if(strbuf[i] > 0x60 && strbuf[i] < 0x67)
			continue;
		if(strbuf[i] == '(')
			continue;
		if(strbuf[i] == ')')
			continue;
		if ((i >0) && (strbuf[i-1] == '('))
			continue;

		if(strbuf[i] < 0x30 || strbuf[i] > 0x39)
		{
			return i;
		}
	}
	return i;
}

/*------------------------------------------------------------------------------------------------------
| 作用：返回公式中某位置的上一个计算符号的距离
| 返回：返回距离大小
| 参数：buf 公式中的位置
--------------------------------------------------------------------------------------------------------*/
int strrnumlen(const char *buf,int start)
{
	int i=1;
	
	while(1)
	{
		if(i==start)
			return i;
		if(buf[-i] == '.')
		{
			i++;
		}
		else if(buf[-i] > 0x2f && buf[-i] < 0x3a)
		{
			i++;
		}
		else if(buf[-i-1] == '(')
		{
			i++;
		}
		else if(buf[-i] == '(')
		{
			i++;
		}
		else if(buf[-i] == ')')
		{
			i++;
		}
		else
			break;
		
	}
	return i-1;
}

/*------------------------------------------------------------------------------------------------------
| 作用：转换字符串到小数,字符串为公式中的
| 返回：返回一个浮点数
| 参数：buf，字符串，函数不做检查，返回第一个不是数字之前的数据
--------------------------------------------------------------------------------------------------------*/
double getDec(const char *buf)
{ 
	U8 size;
	int i;
	int base = 1;
	int pos = (int)strlen(buf)-1;
	double number=0.000000;
	for(i=0; i<(int)strlen(buf);i++)
	{
		if((buf[i] > 0x2F) && (buf[i] < 0x3A))
		{
			number = (buf[i] - 0x30) + number * 10;
		}
		else if(buf[i] == '.')
		{
			pos = i;
		}
		else if(buf[i] == '(' && i ==0 )
		{
			continue;
		}
		else if( i > 0 && (buf[i-1] == '('))
		{
			if(buf[i] == '-')
			{
				//负数标志
				base = -1;
			}
		}
		else
		{
			if(i-1 >= pos)
				size = i-pos -1;
			else
				size = 0;
			pos =1;
			for(i=0; i< size; i++)
			{
				pos *= 10;
			}
			return number / pos * base;
		}
	}

	size = i - pos -1;
	
	pos = 1;
	for(i=0;i< size;i++)
	{
		pos *= 10;

	}
	return number/pos;
}

/*------------------------------------------------------------------------------------------------
| 作用：计算查表数据公式
| 返回：数值
| 参数：
--------------------------------------------------------------------------------------------------*/
double table_express(char *strbuf, char *tablebuf, U8 start)
{
	char *p1;
	char *p2;
	int i = 0;
	char *p;

	p1 = strbuf;

	if(p1 ==0)
		return 0;
	
	if(tablebuf == 0)
		return 0;

	if(strtol(p1,&p2,10) < start)
		return 0;

	p2 = tablebuf;
	for(i=0;i<strtol(p1,&p,10)-start;i++)
	{
		if(strstr(p2,",") == 0)
		{
			strcpy(strbuf, "---");
			return 0;
		}
		p2 = strstr(p2,",") +1;
	}
	p1=strstr(p2,",");
	if(p1)
	{
		strncpy(strbuf,p2,p1-p2);
		strbuf[p1-p2]=00;

	}
	else
		strcpy(strbuf,p2);
	return getDec(p2);
}

/*-------------------------------------------------------------------------------
| 作用：计算完整的判断式，返回字符串
|
|
---------------------------------------------------------------------------------*/
char *bool_express(char *strbuf, U8 *buf, U8 start)
{
	char *p1;
	char *p2;
	
	p1 = strstr(strbuf,":");
	if(p1 == NULL)
	{
		if(strbuf[0] == '\'')
		{
			if(strstr(strbuf+1,"\'"))
			{
				strstr(strbuf+1,"\'")[0] ='\0';
				return strbuf+1;
			}
		}
		if(strcmp(strbuf,"")==0)
			return NULL;
		return strbuf;
	}

	p2 = strstr(strbuf,";");
	if(p2)
	{
		p2[0] = '\0';
	}
	//else
	//{
	//	return NULL;
	//}

	if(str_to_bool(strbuf,buf,start))
	{
		return strbuf;
	}
	else
	{
		if(p2)
		{
			strcpy(strbuf,p2+1);
			return bool_express(strbuf,buf,start);
		}
		else
			return NULL;
	}
}
char *bool_express1(char *strbuf, U8 *buf, U8 start)
{
	char *p1;
	char *p2;
	
	p1 = strstr(strbuf,":");
	if(p1 == NULL)
	{
		if(strbuf[0] == '\'')
		{
			if(strstr(strbuf+1,"\'"))
			{
				strstr(strbuf+1,"\'")[0] ='\0';
				return strbuf+1;
			}
		}
		if(strcmp(strbuf,"")==0)
			return NULL;
		return strbuf;
	}

	p2 = strstr(strbuf,";");
	if(p2)
	{
		p2[0] = '\0';
	}
	//else
	//{
	//	return NULL;
	//}

	if(str_to_bool1(strbuf,buf,start))
	{
		return strbuf;
	}
	else
	{
		if(p2)
		{
			strcpy(strbuf,p2+1);
			return bool_express1(strbuf,buf,start);
		}
		else
			return NULL;
	}
}
/*--------------------------------------------------------------------------------------------------------
| 作用：计算判断式，返回0或者1 为真返回1，为假返回0,只支持一个与计算
|
|
----------------------------------------------------------------------------------------------------------*/
int str_to_bool(char *strbuf, U8 *buf ,U8 start)
{
	char *p2;
	//formatExpress(strbuf,buf);
	char u8buf[256];
	strcpy(u8buf,strbuf);
	p2= strstr(u8buf,":");
	
	if(p2)
	{
		p2[0] = '\0';
		if(calculateExpress(u8buf)>0)
		{
			p2= strstr(strbuf,":");
			strcpy(strbuf,p2+1);
			return 1;
		}
		else
			return 0;
	}
	else
	{
		return 0;
	}
	

}
int str_to_bool1(char *strbuf, U8 *buf ,U8 start)
{
	char *p2;
	//formatExpress(strbuf,buf);
	char u8buf[256];
	strcpy(u8buf,strbuf);
	p2= strstr(u8buf,":");
	
	if(p2)
	{
		p2[0] = '\0';
		if(calculateExpress1(u8buf)>0)
		{
			p2= strstr(strbuf,":");
			strcpy(strbuf,p2+1);
			return 1;
		}
		else
			return 0;
	}
	else
	{
		return 0;
	}
	

}
/*
		CMP :前两个字符
			00:公式计算
			01:开关量,有带公式计算
			02:公式计算，如果为负，则加16
			03:查表
			04:ascii码换
			05:查表得字符串
			06:大众专用的
			07:将两个字符转成标准OBD故障码
			08;用00来计算，但将计算结果记录下来
			09:带公式查表,先将前面的数据计算出来后,再去查表,查表以tab间隔开例:CMP="0900([X03Y04]&0X3FFF)/3TAB:TAB1"
			0A:以00类型算出数据后以时间显示 秒
			0B:以00类型算出数据后以时间显示 分
			注，如公式中包含"**RP**"这个字符，则要将RP这个字符替换成result_mem包含的字符
			11:ASC转换
			12:十六进制显示 (0x20 显示出来是 20)
			13:十进制显示   (0x20 显示出来是 32)
			14:X03表示将第三个字节直接十六制显示
			   Y03表示以十六进制显示第三个字节的高半字节
			   Z03表示以十六进制显示第三个字节的低半字节
		   15:直接将该字节以二进制显示
           16:byfengjl 根据x00转换x00位置上的字符串，这些字符串跟在x00后面( 红旗车测试后的变化 )
		   17:byxiongxg 根据接收到数据的第一个字节表示需要显示的数据长度，如：接收到数据为 01 02 31 32，其中01表示组号，02表示长度，31，32显示为12。 AUDI
		   18:byxiongxg时间显示如X00X01为FFFF，则显示255：255 
		   19:byxiongxg日期显示,从X01表示年,X00的高5位表示月,低3位表示日期，
		   20:byxiongxg 先读出Y01对应表中的数据，再与X00进行计算。  AUDI 8B，8C，93用
		   30:bycheng特殊显示,3000[X0F]/20，最后结果是 X0F/0x20 / X0F%0x20
		   31:日期显示 特殊计算方式
*/
char *getCMPstrResult(char *strcmp, U8* buf, const char* table,const char* unit,ezxml_t xmlroot)
{
	double result;
	char *p;
	char tabbuf[64];
	char tempbuf[256];
	char *p0;
	I32 i,j;
	int len;
	char strExpress[1024];
	ezxml_t tabezxml;
	strcpy(strExpress, strcmp+4);
	
	switch(getHex(strcmp))
	{
	case 0x0a:
		strcmp[0]=0x30;
		strcmp[1]=0x30;
		getCMPstrResult(strcmp,buf,NULL,NULL,xmlroot);
		i=atoi(strcmp);
		//cheng 20090402 以前的就是错的,无法正确显示时间
		//sprintf(strExpress,"%02d:%02d:%02d",(U8)(i/3600),(U8)(i%3600)/60,(U8)(i%60)%60);
		sprintf(strExpress,"%d:%02d:%02d",i/3600,(i%3600)/60,i%60);
		break;
	case 0x0B:
		strcmp[0]=0x30;
		strcmp[1]=0x30;
		getCMPstrResult(strcmp,buf,NULL,NULL,xmlroot);
		i=atoi(strcmp);
		//cheng 20091124 计算结果是分钟，按照 小时:分 的格式显示
		sprintf(strExpress,"%d:%02d",i/60,i%60);
		break;
	case 0x19:	
		strcpy(tempbuf,(char*)buf);
		if(buf[1]&0x80)
		{
			buf[1]=buf[1]-0x80;			
			sprintf(strExpress,"%04d:%02d:%02d",buf[1]+2000,(tempbuf[0]&0x07)*2+1,(buf[0]&0xf8)>>3);
		}
		else
			sprintf(strExpress,"%04d:%02d:%02d",buf[1]+2000,(tempbuf[0]&0x07)*2,(buf[0]&0xf8)>>3);
		break;
	case 0x18:
		strExpress[0] = '\0';
		sprintf(strExpress,"%02d:%02d %s",buf[0],buf[1],unit);
		break;
	case 0x06://将Y01以二进制显示，但受X00控制，如X00的二进制对应为0,则显示空格，否则显示y01对应位(x=F0,y=55,显示 0101____)
		formatExpress06(strExpress, buf);
		break;
	case 0x04://将X00Y01的ASCII码转为字符显示，31 30显示10
		formatExpress11(strExpress, buf,getHex(strcmp + 2));
		break;
	case 0x01://判断式
		if(formatExpress01(strExpress, buf,0) == NULL)
		{
			strExpress[0] = '\0';
			break;
		}
		if(bool_express(strExpress,buf,0) == NULL)
			strcpy(strExpress, "***");
		if((strstr(strExpress,"CMP")-strExpress)==0)
		{
			strcpy(strcmp,strExpress+3);
			return getCMPstrResult(strcmp,buf,NULL,unit,xmlroot);
			//strcpy(strcmp,strExpress+7);
			//result=calculateExpress(strcmp);
			if(!unit)
			unit="";
			switch(getHex(strExpress + 5))
			{
			case 00:
				sprintf(strExpress,"%d %s",(int)result,unit);
				break;
			case 01:
				sprintf(strExpress,"%5.1f %s",result,unit);
				break;
			default:
				sprintf(strExpress,"%5.2f %s",result,unit);
				break;
			}
		}
	break;
	case 0x21:
		if(formatExpress01(strExpress, buf,0) == NULL)
		{
			strExpress[0] = '\0';
			break;
		}
		if(bool_express1(strExpress,buf,0) == NULL)
			strcpy(strExpress, "***");
		break;
    case 0x02:
	case 0x08:
	case 0x00://公式
		if(formatExpress(strExpress, buf,0) == NULL)
		{
			strExpress[0] = '\0';
			break;
		}
		result = calculateExpress(strExpress);
        if(getHex(strcmp) == 0x02 && result < 0)
            result += 16;
		if(!unit)
			unit="";
		switch(getHex(strcmp + 2))
		{
		case 00:
			sprintf(strExpress,"%d %s",(int)(result),unit);
			if(getHex(strcmp)==8)
				sprintf(result_mem,"%5.0f",result);
			break;
		case 01:
			sprintf(strExpress,"%5.1f %s",result,unit);
			if(getHex(strcmp)==8)
				sprintf(result_mem,"%5.1f",result);
			break;
		case 02:
			sprintf(strExpress,"%5.2f %s",result,unit);
			if(getHex(strcmp)==8)
				sprintf(result_mem,"%5.2f",result);
			break;
		default:
			sprintf(strExpress,"%5.3f %s",result,unit);
			if(getHex(strcmp)==8)
				sprintf(result_mem,"%5.3f",result);
			break;
		}
		break;
	case 0x09://查表
		tabezxml=xmlroot;
		p0= strstr(strExpress,"TAB");
		p0=p0+4;
		while(p = strstr(p0,"/"))
		{
			strncpy(tabbuf,p0,p-p0);
			tabbuf[p-p0] = '\0';
			tabezxml = ezxml_child(tabezxml,tabbuf);
			p0 = p+1;
		}
		tabezxml = ezxml_child(tabezxml,p0);
		strcmp[strstr(strcmp,"TAB")-strcmp+1]='\0';
		//if(formatExpress(strExpress, buf,0) == NULL)
		//{
			//strExpress[0] = '\0';
			//break;
		//}
		strcmp[0]=0x30;
		strcmp[1]=0x30;
		getCMPstrResult(strcmp,buf,NULL,NULL,xmlroot);
		table=ezxml_txt(tabezxml);
		result =table_express(strExpress,(char*)table,0);
		strcat(strExpress,unit);
		break;
	case 0x20:
		formatExpress20(strExpress, buf,table,unit,xmlroot);
		break;
	case 0x05://查表得字符串
		//p= strrchr(strExpress,']');
		//formatExpress05(strExpress, buf, ezxml_child(g_commondata.streamxml->parent->parent,p));
		//break;
	case 0x03://查表
		
		tabezxml=xmlroot;
		p0= strrchr(strExpress,']');
		p0=p0+1;
		while(p = strstr(p0,"/"))
		{
			strncpy(tabbuf,p0,p-p0);
			tabbuf[p-p0] = '\0';
			tabezxml = ezxml_child(tabezxml,tabbuf);
			p0 = p+1;
		}
		tabezxml = ezxml_child(tabezxml,p0);
		strExpress[strrchr(strExpress,']')-strExpress+1]='\0';
		if(formatExpress(strExpress, buf,0) == NULL)
		{
			strExpress[0] = '\0';
			break;
		}
		table=ezxml_txt(tabezxml);
		result =table_express(strExpress,(char*)table,0);
        if(unit)
		strcat(strExpress,unit);
		
		break;
	case 0x07://故障码显示
		if(formatExpress(strExpress, buf,0) == NULL)
		{
			strExpress[0] = '\0';
			break;
		}
		i=atoi(strExpress);
		switch(i&0xc000)
		{
		case 00:
			sprintf(strExpress,"P%04X",i&0x3fff);
			break;
		
		case 0X4000:
			sprintf(strExpress,"C%04X",i&0x3fff);
			break;
		
		case 0X8000:
			sprintf(strExpress,"B%04X",i&0x3fff);
			break;
			
		case 0XC000:
			sprintf(strExpress,"U%04X",i&0x3fff);
			break;
		}
		break;  
	case 0x11://ASCII码显示
		formatExpress11(strExpress, buf,getHex(strcmp + 2));
		break;
	case 0xF1://ASCII码显示
		formatExpressF1(strExpress, buf,getHex(strcmp + 2));
		break;
	case 0xE1://ASCII码显示
		formatExpressE1(strExpress, buf,getHex(strcmp + 2));
		break; 
	case 0x2a://ASCII码显示
		formatExpress2A(strExpress, buf,getHex(strcmp + 2));
		break;
    case 0x16:
        j = 2;
        for( i=0; i< buf[0]; )
        {
           if(buf[j++] == 0x03)
		   {
			   i++;
		   }
		   if(j>buf[1])
		   {
				strcpy(strcmp,trr(String_overflow));
				return strcmp;
		   }

        }
        i = 0;
        while(buf[j] != 0x03)
        {
            strExpress[i++] = buf[j++];
			
		   if(j>buf[1]+2)
		   {
				strcpy(strcmp,trr(String_overflow));
				return strcmp;
		   }
        }
        strExpress[i] = 0;

        break;
	case 0x12://十六进制数显示
		formatExpress12(strExpress, buf,getHex(strcmp + 2));
		break;
	case 0x13://十进制数显示
		formatExpress13(strExpress, buf,getHex(strcmp + 2));
		break;
	case 0x14://十进制数显示
		formatExpress14(strExpress, buf,getHex(strcmp + 2));
		break;
	case 0x15://二进制数显示
		//formatExpress15(strExpress, buf,getHex(strcmp + 2));
		j=getHex(strcmp+2);
		strcmp[0]=0x30;
		strcmp[1]=0x30;
		strcmp[2]=0x30;
		strcmp[3]=0x30;
		getCMPstrResult(strcmp,buf,NULL,unit,xmlroot);
		result=(int)atoi(strcmp);
		j=j*8;
		memset(strExpress,0,sizeof(strExpress));
		for(i=0;i<j;i++)
			sprintf(strExpress,"%s%x",strExpress,((int)result>>(j-i-1))&01);
		
		break;
	case 0x17://X00表示需要显示的数据长度，。
		memset(strExpress,0,512);
		len=0;
		for(;len<buf[0];len++)
			strExpress[len]=buf[len+1];
		strExpress[len] = '\0';
		break;
	case 0x30://数据计算，特殊显示
		formatExpress30(strExpress, buf,getHex(strcmp + 2));
		break;
	case 0x31:
		formatExpress31(strExpress, buf,getHex(strcmp + 2));
		break;
	case 0x40:
		formatExpress40(strExpress, buf, 0);
		result = calculateExpress(strExpress);
		if(!unit)
			unit="";
		switch(getHex(strcmp + 2))
		{
		case 00:
			sprintf(strExpress,"%d %s",(int)(result),unit);
			if(getHex(strcmp)==8)
				sprintf(result_mem,"%5.0f",result);
			break;
		case 01:
			sprintf(strExpress,"%5.1f %s",result,unit);
			if(getHex(strcmp)==8)
				sprintf(result_mem,"%5.1f",result);
			break;
		case 02:
			sprintf(strExpress,"%5.2f %s",result,unit);
			if(getHex(strcmp)==8)
				sprintf(result_mem,"%5.2f",result);
			break;
		default:
			sprintf(strExpress,"%5.3f %s",result,unit);
			if(getHex(strcmp)==8)
				sprintf(result_mem,"%5.3f",result);
			break;
		}
		break;
	case 0x41:
		if(formatExpress41(strExpress, buf,0) == NULL)
		{
			strExpress[0] = '\0';
			break;
		}
		if(bool_express(strExpress,buf,0) == NULL)
			strcpy(strExpress, "***");
		break;
	}
	strcpy(strcmp, strExpress);
	return strcmp;

}
BOOL setHex(char *p , int data)
{
	if( p == NULL)
		return FALSE;
	if(data> 255)
		return FALSE;
	switch(data & 0xf0)
	{
	case 0x00:
		p[0] = '0';
		break;
	case 0x10:
		p[0] = '1';
		break;
	case 0x20:
		p[0] = '2';
		break;
	case 0x30:
		p[0] = '3';
		break;
	case 0x40:
		p[0] = '4';
		break;
	case 0x50:
		p[0] = '5';
		break;
	case 0x60:
		p[0] = '6';
		break;
	case 0x70:
		p[0] = '7';
		break;
	case 0x80:
		p[0] = '8';
		break;
	case 0x90:
		p[0] = '9';
		break;
	case 0xa0:
		p[0] = 'A';
		break;
	case 0xb0:
		p[0] = 'B';
		break;
	case 0xc0:
		p[0] = 'C';
		break;
	case 0xd0:
		p[0] = 'D';
		break;
	case 0xe0:
		p[0] = 'E';
		break;
	case 0xf0:
		p[0] = 'F';
		break;
	}
	switch(data & 0x0f)
	{
	case 0x00:
		p[1] = '0';
		break;
	case 0x01:
		p[1] = '1';
		break;
	case 0x02:
		p[1] = '2';
		break;
	case 0x03:
		p[1] = '3';
		break;
	case 0x04:
		p[1] = '4';
		break;
	case 0x05:
		p[1] = '5';
		break;
	case 0x06:
		p[1] = '6';
		break;
	case 0x07:
		p[1] = '7';
		break;
	case 0x08:
		p[1] = '8';
		break;
	case 0x09:
		p[1] = '9';
		break;
	case 0x0a:
		p[1] = 'A';
		break;
	case 0x0b:
		p[1] = 'B';
		break;
	case 0x0c:
		p[1] = 'C';
		break;
	case 0x0d:
		p[1] = 'D';
		break;
	case 0x0e:
		p[1] = 'E';
		break;
	case 0x0f:
		p[1] = 'F';
		break;
	}
	return TRUE;
}

long GetMsgResult_Hex(int len)
{
	char str[64];
	long groupid=0;
	int i=0;
	int datalen = (int)strlen(GetMsgResult());
	if(datalen>len)
		datalen = len;
	strncpy(str,GetMsgResult(),datalen);
	str[datalen]=0x30;
	while(i<datalen)
	{
		groupid *= 0x100;
		groupid += getHex(str+i);
		i += 2;
	}
	if((datalen%2) && groupid)
		groupid /= 0x10;
	return groupid;

}

long getHexMore(const char *buf)
{
	int len;
	int data=0;
	int i=0;
	len = (int)strlen(buf);
	for(i=0;i<len-1;)
	{
		data <<= 8;
		data += getHex(buf+i);
		i += 2;
	}
	//数据个数是奇数，处理最后一个
	if(i == len-1)
	{
		data <<= 4;
		data += getHex(buf+i);
	}
	return data;
}
#ifdef __cplusplus
}
#endif 
