/*
   收发函数
*/
#include "user_queue.h"
#include "STD_OBD_SendRecv.h"
#include "user_config.h"


DEFAULT_MSGATTR										g_defaultMsgAttr= {0}; 		//MSG项的缺省参数

uint8_t m_modulenum=0;//ECU单元个数
uint8_t m_ecumodule;//当前的ECU单元
uint8_t m_setmodule;//选择的ECU单元
static uint8_t checkData0;
static uint8_t checkData1;
static uint8_t checkData2;
uint8_t m_refuseflag;
uint8_t m_refuseretry=10;
uint32_t m_canbussendid=0x000007df;/*系统默认发送ID*/
uint32_t m_canbusflowid=0x000007e0;/*当前系统发送ID*/
uint32_t m_canbusreadid=0x000007e8;/*当前系统接收ID*/
uint32_t m_canbusflowidchang=0;/*需要改变的发送ID*/
uint32_t m_canbusreadidchang=0;/*需要改变的接收ID*/
uint8_t m_KwpWord1;
uint8_t m_two_id_toyota = 0;//丰田CAN第二id
extern uint32_t m_Dtcnum;


uint16_t pro_ReadBuf_Kwp2003(uint8_t *recvbuf, uint32_t delay);

void setcanbusflowid(uint32_t canbusflowid)
{
    m_canbusflowid = canbusflowid;
}
void setcanbussendid(uint32_t canbussendid)
{
    m_canbussendid = canbussendid;
}
void setcanbusreadid(uint32_t canbusreadid)
{
    m_canbusreadid = canbusreadid;
}
void setcanbusflowidchang(uint32_t canbusflowid)
{
    m_canbusflowidchang = canbusflowid;
}
void setcanbusreadidchang(uint32_t canbusreadid)
{
    m_canbusreadidchang = canbusreadid;
}

uint32_t getcanbusflowid()
{
    return m_canbusflowid;
}
uint32_t getcanbusreadid()
{
    return m_canbusflowid;
}
uint32_t getcanbusflowidchang()
{
    return m_canbusflowidchang;
}
uint32_t getcanbusreadidchang()
{
    return m_canbusreadidchang;
}

/*
打包需要发送的数据
a5 a5 00 len msgord sendbuf ??    ??是一字节和取反
*/
uint16_t pro_packsenddata(uint8_t *sendbuf, uint16_t number, uint8_t msgord)
{
    int i;
    uint8_t u8buf[128];
    if(msgord == 0)
    {
        msgord = g_defaultMsgAttr.m_ORD;
    }
    memcpy(u8buf+5, sendbuf, number);
    number++;
    u8buf[0] = 0xa5;
    u8buf[1] = 0xa5;
    u8buf[2] = 0x00;
    u8buf[3] = number;
    u8buf[4] = msgord;
    number += 5;
    u8buf[number-1] = 0;
    for(i=2; i<number-1; i++)
    {
        u8buf[number-1] += u8buf[i];
    }
    u8buf[number-1] = ~u8buf[number-1];
    memcpy(sendbuf, u8buf, number);
    return number;
}

/*
解包接收的数据
a5 a5 00 len msgord data ??    ??是一字节和取反
??,??
data
*/
uint16_t pro_unpackrecvdata(uint8_t *recvbuf, uint16_t number)
{
    if(number <6)
    {
        return 0;
    }
    if((recvbuf[4]&0xf0) == 0xf0)
    {
        return 0;
    }
    number -= 6;
    memcpy(recvbuf, recvbuf+5, number);
    return number;
}

/*
解包kwp2000数据
*/
int unpack_kwp2000(uint8_t *recvbuf, int number)
{
    number = pro_unpackrecvdata(recvbuf, number);
    if(number == 0)
    {
        return 0;
    }
    if(number < 3)
    {
        return 0;
    }
    number = 0;
    if(recvbuf[0] == 0x80)
    {
        number = recvbuf[3];
        memcpy(recvbuf, recvbuf+4, number);
    }
    else if(recvbuf[0] > 0x80)
    {
        number = recvbuf[0]&0x3f;
        memcpy(recvbuf, recvbuf+3, number);
    }
    else if (recvbuf[0] == 0x00)
    {
        number = recvbuf[1];
        memcpy(recvbuf, recvbuf+2, number);
    }
    else if ((recvbuf[0]&0xc0) == 0x00)
    {
        number = recvbuf[0]&0x3f;
        memcpy(recvbuf, recvbuf+1, number);
    }
    return number;
}

uint8_t OBDII_ReadBuf(uint8_t *p_obdii_buff, uint16_t * obdii_buff_len, uint32_t obdii_outtime)
{
    * obdii_buff_len=0;
    while(obdii_outtime--)
    {
        if (USB_BULK_Queue_Read(&OBD_RX_QueueData, p_obdii_buff, obdii_buff_len) == true)
        {
            return 0;
        }
        else
        {
            vTaskDelay(2);
        }
    }
    return 1;

}
uint32_t CommPackBufferDefault(uint8_t *buffer, uint32_t count)
{
    uint32_t  i;
    uint32_t  cmdReturn=count;
    while(cmdReturn-- > 0)
    {
        buffer[cmdReturn+4] = buffer[cmdReturn];
    }
    cmdReturn = count;
    buffer[0x00] = 0xa5;
    buffer[0x01] = 0xa5;
    buffer[0x02] = cmdReturn/0x100;
    buffer[0x03] = cmdReturn%0x100;
    cmdReturn = count+5;
    buffer[cmdReturn-1] = 0x00;
    for(i=2; i<cmdReturn-1; i++)
    {
        buffer[cmdReturn-1] += buffer[i];
    }
    buffer[cmdReturn-1] = ~buffer[cmdReturn-1];
    return cmdReturn;
}

uint32_t CommPackBufferCanbus(uint8_t *buffer, uint32_t canid, uint32_t count)
{
    uint32_t  cmdReturn=count;
    while(cmdReturn-- > 0)
    {
        buffer[cmdReturn+5] = buffer[cmdReturn];
    }
    buffer[0x00] = 0x30;
    buffer[0x01] = (canid>>24)&0xff;
    buffer[0x02] = (canid>>16)&0xff;
    buffer[0x03] = (canid>>8)&0xff;
    buffer[0x04] = canid&0xff;
    cmdReturn = CommPackBufferDefault(buffer, count+5);
    return cmdReturn;
}


uint32_t CommUnpackBufferDeafult(uint8_t *buffer, uint32_t count)
{
    uint32_t  cmdReturn=0;
    if(count > 6)
    {
        cmdReturn=count-6;
        memcpy(buffer, buffer+5, cmdReturn);
    }
    return cmdReturn;
}
/*
Normal 发送初始化IDO
*/
uint16_t pro_SendRecv_InitIo(uint8_t * sendbuf, uint8_t *recvbuf)
{
    uint16_t number;
    uint8_t msgord;
    //if(!SetFrameTimeOut(msgxml))
    //{
    //	set_init_inthread(FALSE);
    //	return FALSE;
    //}
    msgord = g_defaultMsgAttr.m_ORD;
    number = sendbuf[0];

    number = pro_packsenddata(sendbuf+1, number, msgord);
    //Uart_ClearInput(g_carportHandle);
    //Uart_ClearOutput(g_carportHandle);
    USB_BULK_Queue_Write(&USB_RX_QueueData, sendbuf+1 ,number);
    OBDII_ReadBuf(recvbuf, &number,1000);
    vTaskDelay(1000);
    return number;
}
/*
Normal
*/
uint16_t pro_SendRecv_Normal(uint8_t * sendbuf, uint8_t *recvbuf)
{

    uint16_t number;
//	uint8_t msgord = sendbuf.Msg_Ord;
    uint8_t msgord = g_defaultMsgAttr.m_ORD;


    if((msgord&0xf0) == 0x10)
    {
        return pro_SendRecv_InitIo(sendbuf, recvbuf);
    }
    number = pro_packsenddata(sendbuf+1, sendbuf[0], msgord);
    USB_BULK_Queue_Write(&USB_RX_QueueData,sendbuf+1,number);

    printfuartdebug("ord== %d\r\n",msgord);
    printfuartdebug("number == %d\r\n",number);

    if((msgord&0xf0) == 0x20 && number == 7)
    {
        OBDII_ReadBuf(recvbuf, &number,5000);
    }
    else
    {
        OBDII_ReadBuf(recvbuf, &number,g_defaultMsgAttr.m_AnswerFrameTimeOut*20);
    }
    number = pro_unpackrecvdata(recvbuf, number);
    return number;
}


uint16_t pro_SendRecv_CanDefault_Recvmore(uint8_t *recvbuf, uint32_t delay)
{
    uint8_t u8buf[64];
    int dataNum;
    int count;
    uint16_t number;
    short frameflag;


    OBDII_ReadBuf(u8buf, &number,delay);
    number = pro_unpackrecvdata(u8buf, number);

    if(number == 0)
    {
        return 0;
    }

    //如果接收数据超过64个,需要修改recvbuf数组大小,U8 recvbuf[128]
    if(u8buf[0] < 8)
    {
        dataNum = u8buf[0];
        memcpy(recvbuf, u8buf+1, dataNum);
        return dataNum;
    }
    else if((u8buf[0]&0xf0) == 0x10)
    {
        frameflag = 0x21;
        count = 6;
        dataNum = (u8buf[0]&0x0f)*0x100 + u8buf[1];
        memcpy(recvbuf, u8buf+2, count);
        while(count<dataNum)
        {
            OBDII_ReadBuf(u8buf, &number,200);
            number = pro_unpackrecvdata(u8buf, number);
            if(number == 0)
            {
                return 0;
            }
            memcpy(recvbuf+count, u8buf+1, 7);
            count += 7;
            frameflag++;
            if (frameflag == 0x30)
            {
                frameflag = 0x20;
            }
        }
        return dataNum;
    }
    return 0;
}
uint16_t pro_SendRecv_CanDefault_Send(uint8_t *sendbuf, uint16_t number)
{
    uint8_t u8buf[64];
    int i, count;
    uint8_t PsNum;
    uint8_t *sendpt;
    //单帧发送
    if(number <= 7)
    {
        u8buf[0] = number;
        memcpy(u8buf+1, sendbuf, number);
        number++;
        for(i=number; i<8; i++)
        {
            u8buf[i] = 0;
        }
        number = pro_packsenddata(u8buf, 8, 0x30);
        //Uart_ClearInput(g_carportHandle);
        //Uart_ClearOutput(g_carportHandle);
        USB_BULK_Queue_Write(&USB_RX_QueueData,u8buf,number);
        return number;
    }
    //多针发送
    u8buf[0] = 0x10;
    u8buf[1] = number;
    sendpt = sendbuf;
    memcpy(u8buf+2, sendpt, 6);
    number -= 6;
    sendpt += 6;
    count = 8;
    PsNum = 0x20;
    while(number > 0)
    {
        //添加计数器
        PsNum++;
        if(PsNum == 0x30)
        {
            PsNum = 0x21;
        }
        u8buf[count] = PsNum;
        count++;
        memcpy(u8buf+count, sendpt, 7);
        if(number>7)
        {
            number -= 7;
            sendpt += 7;
        }
        else
        {
            for(i=number; i<8; i++)
            {
                u8buf[count+i] = 0;
            }
            number = 0;
        }
        count += 7;
    }
    number = pro_packsenddata(u8buf, count, 0x30);
    //Uart_ClearInput(g_carportHandle);
    //Uart_ClearOutput(g_carportHandle);
    USB_BULK_Queue_Write(&USB_RX_QueueData,u8buf,number);
    return number;
}
uint16_t pro_SendRecv_CanDefault(uint8_t *sendbuf, uint8_t *recvbuf)
{
    int i, number, sendnum;
    int count=0;
    sendnum = sendbuf[0];
    if(sendnum > 0 )
    {
        checkData0 = sendbuf[1];
        checkData0 += 0x40;
        number = pro_SendRecv_CanDefault_Send(sendbuf+1, sendnum);
    }
    number = pro_SendRecv_CanDefault_Recvmore(recvbuf, 200);
    if(number == 0)
    {
        return 0;
    }
    i = 0;
    while(number > 2 && recvbuf[0] == 0x7f)
    {
        if((recvbuf[2] == 0x21) && (sendnum > 0))
        {
            number = pro_SendRecv_CanDefault_Send(sendbuf+1, sendnum);
            number = pro_SendRecv_CanDefault_Recvmore(recvbuf, 200);
        }
        else if(recvbuf[2] == 0x78 )
        {
            number = pro_SendRecv_CanDefault_Recvmore(recvbuf, 5000);
            count ++;
            if(count > 20)
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    if(number == 0)
    {
        return 0;
    }
    if(recvbuf[0] == 0x7f)
    {
        return number;
    }
    if(recvbuf[0] == checkData0)
    {
        return number;
    }
    return 0;
}


uint16_t pro_SendRecv_CanTwoID(uint8_t *sendbuf, uint8_t *recvbuf)//toyota
{
    uint32_t i, number, sendnum;
//	uint8_t sendbuf[256];
    uint32_t count = 0;

    sendnum = sendbuf[0];
    if(sendnum > 0 )
    {
        checkData0 = sendbuf[1];
        checkData0 += 0x40;
        number = pro_SendRecv_CanTwoID_Send(sendbuf+1, sendnum);
    }
    number = pro_SendRecv_CanTwoID_Recvmore(recvbuf, 500);
    if(number == 0)
    {
        return 0;
    }
    i = 0;
    while((number > 2) && (recvbuf[0] == 0x7f)) //chengbb 20130423 ?????????? ????
    {
        //count--;
        if((recvbuf[2] == 0x21) && (sendnum > 0))
        {
            number = pro_SendRecv_CanTwoID_Send(sendbuf+1, sendnum);
            number = pro_SendRecv_CanTwoID_Recvmore(recvbuf, 500);
        }
        else if(recvbuf[2] == 0x78 )
        {
            number = pro_SendRecv_CanTwoID_Recvmore(recvbuf, 5000);
            if(count++ > 20)
            {
                return 0;
            }

        }
        //chengbb 20130523 ????bug ??????????
        else
        {
            break;
        }
        //if(count == 0)//??100?????????,????ECU??
        //{
        //	if((recvbuf[0] == 0x7f) && (m_initxml != NULL))
        //	{
        //		pro_SendRecv(m_initxml, recvinitbuf);
        //	}
        //	break;
        //}
    }
    if(number == 0)
    {
        return 0;
    }
    if(recvbuf[0] == 0x7f)
    {
        return number;
    }
    if(recvbuf[0] == checkData1)
    {
        return number;
    }
    if(number > 0)
    {
        return number;
    }
    return 0;
}

uint16_t pro_SendRecv_CanTwoID_Send(uint8_t *sendbuf, int number)
{
    uint8_t u8buf[256];
    int i, count;
    uint8_t PsNum;
    uint8_t *sendpt;
    //单帧发送
    if(number <= 6)
    {
        u8buf[0] = m_two_id_toyota;
        u8buf[1] = number;
        memcpy(u8buf+2, sendbuf, number);
        number += 2;
        for(i=number; i<8; i++)
        {
            u8buf[i] = 0;
        }
        number = pro_packsenddata(u8buf, 8, 0x30);
        //Uart_ClearInput(g_carportHandle);
        //Uart_ClearOutput(g_carportHandle);
        //Uart_SendBuf(g_carportHandle, u8buf , number);
        USB_BULK_Queue_Write(&USB_RX_QueueData,u8buf,number);
        return number;
    }
    //多帧发送
    u8buf[0] = m_two_id_toyota;
    u8buf[1] = 0x10;
    u8buf[2] = number;
    sendpt = sendbuf;
    memcpy(u8buf+3, sendpt, 5);
    number -= 5;
    sendpt += 5;
    count = 8;
    PsNum = 0x20;
    while(number > 0)
    {
        //添加计数器
        PsNum++;
        if(PsNum == 0x30)
        {
            PsNum = 0x21;
        }
        u8buf[count] = m_two_id_toyota;
        count++;
        u8buf[count] = PsNum;
        count++;
        memcpy(u8buf+count, sendpt, 6);
        if(number>6)
        {
            number -= 6;
            sendpt += 6;
        }
        else
        {
            for(i=number; i<8; i++)
            {
                u8buf[count+i] = 0;
            }
            number = 0;
        }
        count += 6;
    }
    number = pro_packsenddata(u8buf, count, 0x30);
    //Uart_ClearInput(g_carportHandle);
    //Uart_ClearOutput(g_carportHandle);
    //Uart_SendBuf(g_carportHandle, u8buf , number);
    USB_BULK_Queue_Write(&USB_RX_QueueData,u8buf,number);
    return number;
}

uint16_t pro_SendRecv_CanTwoID_Recvmore(uint8_t *recvbuf, int delay)
{
    uint8_t u8buf[256];
    uint8_t dataNum;
    uint16_t count;
    uint16_t number;
    //number = Uart_ReadBuf(g_carportHandle, u8buf, 256, delay);
    OBDII_ReadBuf(u8buf, &number,delay);
    number = pro_unpackrecvdata(u8buf, number);
    if(number == 0)
    {
        return 0;
    }
    /*
    dataNum = u8buf[1];
    if(dataNum < 7)
    {
    memcpy(recvbuf, u8buf+2, dataNum);
    return dataNum;
    }
    else if(dataNum == 0x10)
    {
    count = 5;
    dataNum = u8buf[2];*/
    //如果接收数据超过256个，需要修改recvbuf数组大小，uint8_t recvbuf[4096]
    if(u8buf[1] < 7)
    {
        dataNum = u8buf[1];
        memcpy(recvbuf, u8buf+2, dataNum);
        return dataNum;
    }
    else if((u8buf[1]&0xf0) == 0x10)
    {
        count = 5;
        dataNum = (u8buf[1]&0x0f)*0x100 + u8buf[2];
        memcpy(recvbuf, u8buf+3, count);
        while(count<dataNum)
        {
            //number = Uart_ReadBuf(g_carportHandle, u8buf, 256, 1000);
            OBDII_ReadBuf(u8buf, &number,delay);
            number = pro_unpackrecvdata(u8buf, number);
            if(number == 0)
            {
                return 0;
            }
            memcpy(recvbuf+count, u8buf+2, 6);
            count += 6;
        }
        return dataNum;
    }
    //收到的数据不是第一帧数据
    else
    {
        return -1;
    }
    return 0;
}


uint32_t CommReadSingleCanbus(uint8_t* readbuf, uint16_t maxlen, uint32_t delay)
{
    uint32_t canid=0;
    uint16_t count=0;
    uint32_t errnum=0;
    while(m_canbusreadid != canid)
    {
        OBDII_ReadBuf(readbuf, &count,delay);
        count = CommUnpackBufferDeafult(readbuf, count);
        if(count < 4 || errnum++ > 20)
        {
            m_refuseflag = 0x03;/*03: 与汽车通讯失败*/
            return 0;
        }
        canid = (readbuf[0]<<24) + (readbuf[1]<<16) + (readbuf[2]<<8) + readbuf[3];
    }
    return count;
}

uint32_t CommSendBufferCanbus(uint8_t* sendbuf, uint16_t sendnum)
{
    uint32_t count=0;
    uint32_t cmdReturn=0;
    uint32_t framecount;
    uint32_t frameflag;
    uint32_t flowBS=0;
    uint32_t flowST=0;
    uint8_t buffer[64];
    memset(buffer, 0x00, sizeof(buffer));
    if(sendnum == 0)
    {
        m_refuseflag = 0x03;
        return 0;
    }
    /*发送单帧 标准OBDII只支持发送单帧*/
    if(sendnum < 8)
    {
        buffer[0] = sendnum;
        memcpy(buffer + 1, sendbuf, sendnum);

        count = CommPackBufferCanbus(buffer, m_canbussendid, 8);
        USB_BULK_Queue_Write(&USB_RX_QueueData,buffer,count);
    }
    /*发送多针数据 标准OBDII不支持多针  这里写上备用*/
    else
    {
        framecount = 6;
        frameflag = 1;
        buffer[0] = (int)sendnum/0x100 + 0x10;
        buffer[1] = (int)sendnum%0x100;
        memcpy(buffer+2,sendbuf, sendnum);
        count = CommPackBufferCanbus(buffer, m_canbusflowid, 8);
        USB_BULK_Queue_Write(&USB_RX_QueueData,buffer,count);
    }
    cmdReturn = 1;
    return cmdReturn;
}

uint32_t CommReadBufferCanbus(uint8_t* readbuf, uint16_t maxlen, uint32_t delay)
{
    uint32_t cmdReturn=0;
    uint32_t count=0;
    uint32_t canid=0;
    uint32_t framecount;
    uint8_t frameflag;
    uint8_t flowBS=0;/*接收最大帧数 默认全部接受*/
    uint8_t flowST=0x0a;/*接收最小时间间隔 默认10ms*/
    uint8_t buffer[64];
    count = CommReadSingleCanbus(buffer, maxlen, delay);
    if(count == 0)
    {
        return 0;
    }
    if(buffer[4] < 0x08)
    {
        cmdReturn = buffer[4];
        memcpy(readbuf, buffer+5, cmdReturn);
    }
    else if((buffer[4]&0xf0) == 0x10)
    {
        cmdReturn = (buffer[4]&0x0f)*0x100 + buffer[5];
        memcpy(readbuf, buffer+6, 6);
        memset(buffer, 0x00, sizeof(buffer));

        framecount = 6;
        frameflag  = 1;
        while(framecount < cmdReturn)
        {
            vTaskDelay(flowST);
            count = CommReadSingleCanbus(buffer, maxlen, 200);
            if(count < 4)
            {
                m_refuseflag = 0x03;/*03:CANBUS控制字节错误*/
                return 0;
            }
            if(buffer[4] != 0x20+frameflag)
            {
                m_refuseflag = 0x01;/*01:CANBUS控制字节错误*/
                return 0;
            }
            memcpy(readbuf+framecount, buffer+5, 7);
            frameflag++;
            if(frameflag > 0x0f)
            {
                frameflag = 0;
            }
            framecount += 7;
        }
    }
    return cmdReturn;
}

uint16_t pro_SendRecv_CanNormal(uint8_t * sendbuf, uint8_t *readbuf)
{
    uint32_t cmdReturn=0;
    uint8_t maxlen=0x64;
    uint8_t count=0;
    uint16_t sendnum=0;
    uint8_t RetryCnt = 0;
    int count1=0;
    sendnum = sendbuf[0];
    checkData2 = sendbuf[1]+0x40;
    cmdReturn = CommSendBufferCanbus(sendbuf+1, sendnum);
    if(cmdReturn == 0x00)
    {
        return 0;
    }
    cmdReturn = CommReadBufferCanbus(readbuf, maxlen, 1000);
    count = m_refuseretry;
    while(cmdReturn > 0x02 && readbuf[0x00] == 0x7f)
    {
        if(readbuf[0x02] == 0x78) //调整否定回答重试次数，一直重试
        {
            vTaskDelay(20);
            cmdReturn = CommReadBufferCanbus(readbuf, maxlen, 200);

            if((++RetryCnt) > 5)
            {
                return 0;
            }
            printfUartObdiideBug("\r\nRetry time is %d \r\n" , RetryCnt);
        }
        else if(readbuf[0x02] == 0x21||readbuf[0x02] == 0x23||readbuf[0x02] == 0xFE) //调整否定回答重试次数，一直重试
        {
            vTaskDelay(20);
            cmdReturn = CommSendBufferCanbus(sendbuf+1, sendnum);
            cmdReturn = CommReadBufferCanbus(readbuf, maxlen, 200);
        }
        else
        {
            break;
        }
    }
    if((cmdReturn == 0x00) && (m_refuseflag != 0xff))
    {
        m_refuseflag = 0x03;/*03:无响应*/
        return 0;
    }
    if(readbuf[0] != checkData2)
    {
        m_refuseflag = 0x02;
        return 0;
    }
    if(sendbuf[1] < 0x0a && sendnum > 1 && sendbuf[2] != readbuf[1])
    {
        m_refuseflag = 0x03;
        return 0;
    }

    memset(readbuf+cmdReturn, 0x00, maxlen-cmdReturn);
    return cmdReturn;
}


uint16_t pro_SendRecv_KwpA(uint8_t * cmddbuf, uint8_t *recvbuf)//toyota
{
    int number = 0;
    uint8_t sendbuf[128] = {0};
    uint8_t u8buf[128] = {0};
    int sendnum = 0;
    int i = 0;
    int pack_t = 2;
    int count = 10;
    int retry = 1;
    int delay = g_defaultMsgAttr.m_AnswerFrameTimeOut;//ecu??????


    number = cmddbuf[0];
    memcpy(u8buf+3, cmddbuf+1, number);

    u8buf[0] = 0x80+number;
    u8buf[1] = g_defaultMsgAttr.m_TGT;
    u8buf[2] = g_defaultMsgAttr.m_SRC;
    number += 4;
    u8buf[number-1] = 0;
    for(i=0; i<number-1; i++)
    {
        u8buf[number-1] += u8buf[i];
    }
//	sendnum = pro_packsenddata(u8buf, number, cmddbuf.Msg_Ord);

    sendnum = pro_packsenddata(u8buf, number,g_defaultMsgAttr.m_ORD);

    for(i=0; i<retry; i++)
    {
        USB_BULK_Queue_Write(&USB_RX_QueueData,u8buf,sendnum);
        number = pro_ReadBuf_Kwp2000(recvbuf, delay*30);
        printfUartObdiideBug("number ==================== %d\r\n" , number);
        if(number > 0)
        {
            break;
        }
    }
    while(number > 0 && recvbuf[0] == 0x7f)//chengbb 20130423 ?????????? ????
    {
        if(recvbuf[2] == 0x21)
        {
            USB_BULK_Queue_Write(&USB_RX_QueueData,u8buf,sendnum);
            number = pro_ReadBuf_Kwp2000(recvbuf, delay*20);
        }
        else if(recvbuf[2] == 0x78)
        {
            number = pro_ReadBuf_Kwp2000(recvbuf, delay*20);
        }
        else
        {
            break;
        }
        vTaskDelay(delay);
    }
    return number;
}
uint16_t pro_SendRecv_KwpA_X(uint8_t * cmddbuf, uint8_t *recvbuf)// GREATWALL
{
    int number = 0;
    uint8_t sendbuf[128] = {0};
    uint8_t u8buf[128] = {0};
    int sendnum = 0;
    int i = 0;
    int pack_t = 2;
    int count = 10;
    int retry = 1;
    int delay = g_defaultMsgAttr.m_AnswerFrameTimeOut;//ecu??????


    number = cmddbuf[0];
    memcpy(u8buf+3, cmddbuf+1, number);

    u8buf[0] = 0x80+number;
    u8buf[1] = g_defaultMsgAttr.m_TGT;
    u8buf[2] = g_defaultMsgAttr.m_SRC;
    number += 4;
    u8buf[number-1] = 0;
    for(i=0; i<number-1; i++)
    {
        u8buf[number-1] += u8buf[i];
    }
//	sendnum = pro_packsenddata(u8buf, number, cmddbuf.Msg_Ord);

    sendnum = pro_packsenddata(u8buf, number,g_defaultMsgAttr.m_ORD);

    for(i=0; i<retry; i++)
    {
        USB_BULK_Queue_Write(&USB_RX_QueueData,u8buf,sendnum);
        number = pro_ReadBuf_Kwp2000_X(recvbuf, delay*30);
        printfUartObdiideBug("number ==================== %d\r\n" , number);
        if(number > 0)
        {
            break;
        }
    }
    while(number > 0 && recvbuf[0] == 0x7f)//chengbb 20130423 ?????????? ????
    {
        if(recvbuf[2] == 0x21)
        {
            USB_BULK_Queue_Write(&USB_RX_QueueData,u8buf,sendnum);
            number = pro_ReadBuf_Kwp2000_X(recvbuf, delay*20);
        }
        else if(recvbuf[2] == 0x78)
        {
            number = pro_ReadBuf_Kwp2000_X(recvbuf, delay*20);
        }
        else
        {
            break;
        }
        vTaskDelay(delay);
    }
    return number;
}

uint16_t pro_SendRecv_KwpD(uint8_t * cmddbuf, uint8_t *recvbuf)
{
    int number = 0;
    uint8_t sendbuf[64] = {0};
    uint8_t u8buf[64] = {0};
    int sendnum = 0;
    int i = 0;
    int pack_t = 2;
    int count = 10;
    int retry = 1;
    int delay = g_defaultMsgAttr.m_AnswerFrameTimeOut;//ecu??????


    number = cmddbuf[0];
    memcpy(u8buf+3, cmddbuf+1, number);

    u8buf[0] = 0xC0+number;
    u8buf[1] = g_defaultMsgAttr.m_TGT;
    u8buf[2] = g_defaultMsgAttr.m_SRC;
    number += 4;
    u8buf[number-1] = 0;
    for(i=0; i<number-1; i++)
    {
        u8buf[number-1] += u8buf[i];
    }
//	sendnum = pro_packsenddata(u8buf, number, cmddbuf.Msg_Ord);

    sendnum = pro_packsenddata(u8buf, number,g_defaultMsgAttr.m_ORD);

    for(i=0; i<retry; i++)
    {
        USB_BULK_Queue_Write(&USB_RX_QueueData,u8buf,sendnum);
        number = pro_ReadBuf_Kwp2000(recvbuf, delay*30);
        if(number > 0)
        {
            break;
        }
    }
    while(number > 0 && recvbuf[0] == 0x7f)//chengbb 20130423 ?????????? ????
    {
        if(recvbuf[2] == 0x21)
        {
            USB_BULK_Queue_Write(&USB_RX_QueueData,u8buf,sendnum);
            number = pro_ReadBuf_Kwp2000(recvbuf, delay*20);
        }
        else if(recvbuf[2] == 0x78)
        {
            number = pro_ReadBuf_Kwp2000(recvbuf, delay*20);
        }
        else
        {
            break;
        }
        vTaskDelay(delay);
    }
    return number;
}

//ADD BY ZNF NISSAN 添加打包方式E
uint16_t pro_SendRecv_KwpE(uint8_t * cmddbuf, uint8_t *recvbuf)
{
    int number = 0;
    uint8_t sendbuf[64] = {0};
    uint8_t u8buf[64] = {0};
    int sendnum = 0;
    int i = 0;
    int pack_t = 2;
    int count = 10;
    int retry = 1;
    int delay = g_defaultMsgAttr.m_AnswerFrameTimeOut;//ecu??????


    number = cmddbuf[0];
    memcpy(u8buf+1, cmddbuf+1, number);

    u8buf[0] = 0x00+number;
    number += 2;
    u8buf[number-1] = 0;
    for(i=0; i<number-1; i++)
    {
        u8buf[number-1] += u8buf[i];
    }
//	sendnum = pro_packsenddata(u8buf, number, cmddbuf.Msg_Ord);

    sendnum = pro_packsenddata(u8buf, number,g_defaultMsgAttr.m_ORD);

    for(i=0; i<retry; i++)
    {
        USB_BULK_Queue_Write(&USB_RX_QueueData,u8buf,sendnum);
        number = pro_ReadBuf_Kwp2003(recvbuf, delay*30);
        if(number > 0)
        {
            break;
        }
    }
    while(number > 0 && recvbuf[0] == 0x7f)//chengbb 20130423 ?????????? ????
    {
        if(recvbuf[2] == 0x21)
        {
            USB_BULK_Queue_Write(&USB_RX_QueueData,u8buf,sendnum);
            number = pro_ReadBuf_Kwp2000(recvbuf, delay*20);
        }
        else if(recvbuf[2] == 0x78)
        {
            number = pro_ReadBuf_Kwp2000(recvbuf, delay*20);
        }
        else
        {
            break;
        }
        vTaskDelay(delay);
    }
    return number;
}

uint16_t pro_SendRecv_KwpF(uint8_t * cmddbuf, uint8_t *recvbuf)
{
    int number = 0;
    uint8_t sendbuf[64] = {0};
    uint8_t u8buf[64] = {0};
    int sendnum = {0};
    int i = {0};
    int count = 10;
    int retry = 1;
    int delay = g_defaultMsgAttr.m_AnswerFrameTimeOut;//ecu??????

    memset(sendbuf , 0 ,sizeof(sendbuf));
    memset(u8buf , 0 , sizeof(u8buf));

    number = cmddbuf[0];
    memcpy(u8buf+2, cmddbuf+1, number);

    u8buf[0] = 0x00;
    u8buf[1] = number;
    number += 3;
    u8buf[number-1] = 0;
    for(i=0; i<number-1; i++)
    {
        u8buf[number-1] += u8buf[i];
    }
//	sendnum = pro_packsenddata(u8buf, number, cmddbuf.Msg_Ord);

    sendnum = pro_packsenddata(u8buf, number,g_defaultMsgAttr.m_ORD);

    for(i=0; i<retry; i++)
    {
        USB_BULK_Queue_Write(&USB_RX_QueueData,u8buf,sendnum);
        number = pro_ReadBuf_Kwp2003(recvbuf, delay*30);
        if(number > 0)
        {
            break;
        }
    }
    while(number > 0 && recvbuf[0] == 0x7f)//chengbb 20130423 ?????????? ????
    {
        if(recvbuf[2] == 0x21)
        {
            USB_BULK_Queue_Write(&USB_RX_QueueData,u8buf,sendnum);
            number = pro_ReadBuf_Kwp2000(recvbuf, delay*20);
        }
        else if(recvbuf[2] == 0x78)
        {
            number = pro_ReadBuf_Kwp2000(recvbuf, delay*20);
        }
        else
        {
            break;
        }
        vTaskDelay(delay);
    }
    return number;
}

uint16_t pro_SendRecv_KwpB(uint8_t * cmddbuf, uint8_t *recvbuf)
{
    int number;
    uint8_t sendbuf[64];
    uint8_t u8buf[64];
    int sendnum;
    int i;
    int count = 10;
    int retry = 1;
    int delay = g_defaultMsgAttr.m_AnswerFrameTimeOut;//ecu??????


    number = cmddbuf[0];
    memcpy(u8buf+4, cmddbuf+1, number);

    u8buf[0] = 0x80;
    u8buf[1] = g_defaultMsgAttr.m_TGT;
    u8buf[2] = g_defaultMsgAttr.m_SRC;
    u8buf[3] = number;
    number += 5;
    u8buf[number-1] = 0;
    for(i=0; i<number-1; i++)
    {
        u8buf[number-1] += u8buf[i];
    }
//	sendnum = pro_packsenddata(u8buf, number, cmddbuf.Msg_Ord);

    sendnum = pro_packsenddata(u8buf, number,g_defaultMsgAttr.m_ORD);

    for(i=0; i<retry; i++)
    {
        USB_BULK_Queue_Write(&USB_RX_QueueData,u8buf,sendnum);
        number = pro_ReadBuf_Kwp2000(recvbuf, delay*30);
        if(number > 0)
        {
            break;
        }
    }
    while(number > 0 && recvbuf[0] == 0x7f)//chengbb 20130423 ?????????? ????
    {
        if(recvbuf[2] == 0x21)
        {
            USB_BULK_Queue_Write(&USB_RX_QueueData,u8buf,sendnum);
            number = pro_ReadBuf_Kwp2000(recvbuf, delay*20);
        }
        else if(recvbuf[2] == 0x78)
        {
            number = pro_ReadBuf_Kwp2000(recvbuf, delay*20);
        }
        else
        {
            break;
        }
        vTaskDelay(delay);
    }
    return number;
}

uint16_t pro_SendRecv_NISSAN(uint8_t * cmddbuf, uint8_t *recvbuf)
{
    int number, i;


    if((m_KwpWord1&0xf0) == 0xd0)
    {
        number = pro_SendRecv_KwpA(cmddbuf,recvbuf);//Header[(fmt + len) + tgt + src] + Data + CS
    }
    else if ((m_KwpWord1&0x01) == 0x01)
    {
        if (((m_KwpWord1&0x0f) == 0x09) || ((m_KwpWord1&0x0f) == 0x0b))
        {
            number = pro_SendRecv_KwpA(cmddbuf,recvbuf);//Header[(fmt + len) + tgt + src] + Data + CS , Header 3Bytes
        }
        else
        {
            number = pro_SendRecv_KwpE(cmddbuf, recvbuf);//Header[(fmt + len)] + Data + CS , Header 1Byte
        }
    }
    else if (((m_KwpWord1&0x0f) == 0x02) || ((m_KwpWord1&0x0f) == 0x06) || ((m_KwpWord1&0x0f) == 0x0e))
    {
        number = pro_SendRecv_KwpF(cmddbuf, recvbuf);//Header[fmt + len] + Data + CS , Header 2Bytes
    }
    else if ((m_KwpWord1&0x0f) == 0x0a)
    {
        number = pro_SendRecv_KwpB(cmddbuf, recvbuf);//Header[fmt + len + tgt + src] + Data + CS , Header 4Bytes
    }
    else
    {
        number = 0;
    }
    return number;
}

//日产专用解包函数

uint16_t pro_ReadBuf_Kwp2003(uint8_t *recvbuf, uint32_t delay)
{
    uint16_t number;
    uint8_t i=0;
    int x;
    OBDII_ReadBuf(recvbuf, &number,delay);
    number = pro_unpackrecvdata(recvbuf, number);
//	printfuartdebug("RECV number == %d\r\n",number);
//	printfuartdebug("RECV DATA ==\r\n");
//	for(;i<number;i++)
//	{
//		printfuartdebug("%02x",recvbuf[i]);
//	}


    if(number < 2)
    {

        return 0;
    }

    m_ecumodule = recvbuf[2];
    //chengbb 20130604 ???????? ????
    if(m_modulenum == 1 && m_setmodule != m_ecumodule)
    {

        return 0;
    }
    while(number > 0 && m_modulenum > 1 && m_setmodule != m_ecumodule)
    {
        OBDII_ReadBuf(recvbuf, &number,delay);
        number = pro_unpackrecvdata(recvbuf, number);

        if(number < 2)
        {
            return 0;
        }
        m_ecumodule = recvbuf[2];
    }

    for(x = 0 ;	x < number ; x++)
    {
        printfUartObdiideBug("%02x ", recvbuf[x]);
    }
    printfUartObdiideBug("\r\n");
    number = 0;
    if(recvbuf[0] == 0x80)
    {

        number = recvbuf[3];
        memcpy(recvbuf, recvbuf+4, number);
    }
    else if(recvbuf[0] > 0x80)
    {

        number = recvbuf[0]&0x3f;
        memcpy(recvbuf, recvbuf+3, number);
    }
    else if(recvbuf[0] == 0x00)
    {

        number = recvbuf[1];
        memcpy(recvbuf, recvbuf+2, number);
    }
    else
    {

        number = recvbuf[0];
        memcpy(recvbuf, recvbuf+1, number);
    }
    return number;
}

uint16_t pro_ReadBuf_Kwp2000(uint8_t *recvbuf, uint32_t delay)
{
    uint16_t number;
    uint8_t i=0;
    int x;
    OBDII_ReadBuf(recvbuf, &number,delay);
    printfUartObdiideBug("OBDII_ReadBuf number === %d \r\n",number);
    number = pro_unpackrecvdata(recvbuf, number);
    printfUartObdiideBug("pro_ReadBuf_Kwp2000 number === %d \r\n",number);
//	printfuartdebug("RECV number == %d\r\n",number);
//	printfuartdebug("RECV DATA ==\r\n");
//	for(;i<number;i++)
//	{
//		printfuartdebug("%02x",recvbuf[i]);
//	}
    if(number < 5)
    {
        return 0;
    }

    m_ecumodule = recvbuf[2];
    //chengbb 20130604 ???????? ????
    if(m_modulenum == 1 && m_setmodule != m_ecumodule)
    {
        printfUartObdiideBug("m_modulenum ==  %d,m_setmodule== %d m_ecumodule==%d\r\n",m_modulenum , m_setmodule , m_ecumodule);
        return 0;
    }
    while(number > 0 && m_modulenum > 1 && m_setmodule != m_ecumodule)
    {
        OBDII_ReadBuf(recvbuf, &number,delay);
        number = pro_unpackrecvdata(recvbuf, number);
        if(number < 5)
        {
            return 0;
        }
        m_ecumodule = recvbuf[2];
    }

    for(x = 0 ;	x < number ; x++)
    {
        printfUartObdiideBug("%02x ", recvbuf[x]);
    }
    printfUartObdiideBug("\r\n");
    number = 0;
    if(recvbuf[0] == 0x80)
    {
        number = recvbuf[3];
        memcpy(recvbuf, recvbuf+4, number);
    }
    else if(recvbuf[0] > 0x80)
    {
        number = recvbuf[0]&0x3f;
        memcpy(recvbuf, recvbuf+3, number);
    }
    else if(recvbuf[0] == 0x00)
    {
        number = recvbuf[1];
        memcpy(recvbuf, recvbuf+2, number);
    }
    else
    {
        number = recvbuf[0];
        memcpy(recvbuf, recvbuf+1, number);
    }
    return number;
}

uint16_t pro_ReadBuf_Kwp2000_X(uint8_t *recvbuf, uint32_t delay)
{
    uint16_t number;
    uint8_t i=0;
    int x;
    OBDII_ReadBuf(recvbuf, &number,delay);
    printfUartObdiideBug("OBDII_ReadBuf number === %d \r\n",number);
    number = pro_unpackrecvdata(recvbuf, number);
    printfUartObdiideBug("pro_ReadBuf_Kwp2000 number === %d \r\n",number);
//	printfuartdebug("RECV number == %d\r\n",number);
//	printfuartdebug("RECV DATA ==\r\n");
//	for(;i<number;i++)
//	{
//		printfuartdebug("%02x",recvbuf[i]);
//	}
    if(number < 5)
    {
        return 0;
    }

    for(x = 0 ;	x < number ; x++)
    {
        printfUartObdiideBug("%02x ", recvbuf[x]);
    }
    printfUartObdiideBug("\r\n");
    number = 0;
    if(recvbuf[0] == 0x80)
    {
        number = recvbuf[3];
        memcpy(recvbuf, recvbuf+4, number);
    }
    else if(recvbuf[0] > 0x80)
    {
        number = recvbuf[0]&0x3f;
        memcpy(recvbuf, recvbuf+3, number);
    }
    else if(recvbuf[0] == 0x00)
    {
        number = recvbuf[1];
        memcpy(recvbuf, recvbuf+2, number);
    }
    else
    {
        number = recvbuf[0];
        memcpy(recvbuf, recvbuf+1, number);
    }
    return number;
}


uint16_t pro_SendRecv_ISO(uint8_t * cmdbuf, uint8_t *recvbuf)
{
    int number;
    int i;
    uint8_t sendbuf[64];

//	if(m_modulenum > 1)
//	{
//		pro_ReadBuf_Clear();
//	}

    number = cmdbuf[0];
    memcpy(sendbuf+3, cmdbuf+1, number);

    sendbuf[0] = 0x68;
    sendbuf[1] = g_defaultMsgAttr.m_TGT;
    sendbuf[2] = g_defaultMsgAttr.m_SRC;
    number += 4;
    sendbuf[number-1] = 0;

    for(i=0; i<number-1; i++)
    {
        sendbuf[number-1] += sendbuf[i];
    }
    number = pro_packsenddata(sendbuf, number, 0x33);

    USB_BULK_Queue_Write(&USB_RX_QueueData,sendbuf,number);

    number = pro_OBDII_ReadBuffer(recvbuf);
    return number;
}


uint16_t pro_SendRecv_PWM(uint8_t * cmdbuf, uint8_t *recvbuf)
{
    int number;
    int i;
    uint8_t crc = 0xff;
    uint8_t sendbuf[256] = {0};
    //OBDII?? ???? ???????
//	if(m_modulenum > 1)
//	{
//		pro_ReadBuf_Clear();
//	}
    number = cmdbuf[0];
    memcpy(sendbuf+3, cmdbuf+1, number);
    printfUartObdiideBug("sendbuf is %02x %02x %02x",sendbuf[3],sendbuf[4],sendbuf[5]);
    sendbuf[1] = g_defaultMsgAttr.m_TGT;
    sendbuf[2] = g_defaultMsgAttr.m_SRC;
    if(sendbuf[1] == 0x6a)
    {
        sendbuf[0] = 0x61;
    }
    else
    {
        sendbuf[0] = 0xC4;
    }
    number += 4;
    sendbuf[number-1] = 0;
    for(i=0; i<number-1; i++)
    {
        crc= GetCrc(sendbuf[i],crc);
    }
    crc = ~crc;
    sendbuf[number-1] = crc;
    number = pro_packsenddata(sendbuf, number, 0x32);

    USB_BULK_Queue_Write(&USB_RX_QueueData,sendbuf,number);

    number = pro_OBDII_ReadBuffer(recvbuf);

    vTaskDelay(30);
    return number;
}


uint8_t GetCrc(uint8_t  val,uint8_t  crc)
{
    uint8_t  ii= 8;
    while(ii--)
    {
        if(((val ^ crc) & 0x80) != 0)
        {
            crc^= 0x0e;
            crc = (uint8_t )((crc << 1) | 1);
        }
        else
        {
            crc= (uint8_t )(crc << 1);
        }
        val = (uint8_t )(val << 1);
    }
    return crc;
}


uint16_t pro_OBDII_ReadBuffer(uint8_t *recvbuf)
{
    uint16_t number;

    OBDII_ReadBuf(recvbuf, &number,500);
    printfUartObdiideBug("OBDII 000 number=%d\r\n",number);
    number = pro_unpackrecvdata(recvbuf, number);
    if(number < 4)
    {
        printfUartObdiideBug("OBDII 001 number=%d\r\n",number);
        return 0;
    }
    m_ecumodule = recvbuf[2];
    if(m_modulenum == 1 && m_setmodule != m_ecumodule)
    {
        printfUartObdiideBug("OBDII 002 number=%d\r\n",number);
        return 0;
    }
    while(number > 0 && m_modulenum > 1 && m_setmodule != m_ecumodule)
    {
        number = OBDII_ReadBuf(recvbuf, &number,500);
        number = pro_unpackrecvdata(recvbuf, number);
        if(number < 5)
        {
            printfUartObdiideBug("OBDII 003 number=%d\r\n",number);
            return 0;
        }
        m_ecumodule = recvbuf[2];
    }
    if(recvbuf[3] == 0x7f)
    {
        printfUartObdiideBug("OBDII 004 number=%d\r\n",number);
        return 0;
    }
    number -= 4;
    memcpy(recvbuf, recvbuf+3, number);
    printfUartObdiideBug("OBDII 005 number=%d\r\n",number);
    return number;

}

uint16_t pro_SendRecv_VPW(uint8_t *cmdbuf, uint8_t *recvbuf)
{
    uint16_t number;
    int i;
    uint8_t crc = 0xff;
    uint8_t sendbuf[256];
    //OBDII?? ???? ???????
//	if(m_modulenum > 1)
//	{
//		pro_ReadBuf_Clear();
//	}

    number = cmdbuf[0];
    memcpy(sendbuf+3, cmdbuf+1, number);

    sendbuf[1] = g_defaultMsgAttr.m_TGT;
    sendbuf[2] = g_defaultMsgAttr.m_SRC;
    if(sendbuf[1] == 0x6a)
    {
        sendbuf[0] = 0x68;
    }
    else
    {
        sendbuf[0] = 0x6C;
    }
    number += 4;
    sendbuf[number-1] = 0;
    //???CRC??
    for(i=0; i<number-1; i++)
    {
        crc= GetCrc(sendbuf[i],crc);
    }
    crc = ~crc;
    sendbuf[number-1] = crc;
    number = pro_packsenddata(sendbuf, number, 0x31);

    USB_BULK_Queue_Write(&USB_RX_QueueData,sendbuf,number);

    number = pro_OBDII_ReadBuffer(recvbuf);
    return number;
}

uint16_t CommReadBufferDefault(uint8_t *buffer, uint16_t maxlen, uint16_t delay)
{
    uint16_t cmdReturn=0;
//	cmdReturn = Device_ReadBuf(g_carportHandle, buffer, maxlen, delay);

    OBDII_ReadBuf(buffer, &cmdReturn,delay);
    return cmdReturn;
}

uint16_t pro_SendRecv_HondaPro(uint8_t *cmdbuf, uint8_t *recvbuf)//本田协议收发
{
    uint16_t number;
    uint8_t sendbuf[128];
    uint8_t u8buf[128];
    int sendnum;
    int i;
    //int pack_t = 2;
    int count = 10;
    int retry = 1;
    int delay = g_defaultMsgAttr.m_AnswerFrameTimeOut;//ecu??????

    //打包
    sendnum = pack_typef(cmdbuf+1,u8buf,cmdbuf[0]);

    for(i=0; i<retry; i++)
    {
        USB_BULK_Queue_Write(&USB_RX_QueueData,u8buf,sendnum);
        OBDII_ReadBuf(recvbuf, &number,delay*30);
        number = unpack_buff(recvbuf,number);
        if(number > 0)
        {
            break;
        }
    }
    while(number > 0 && recvbuf[0] == 0x7f)//chengbb 20130423 ?????????? ????
    {
        if(recvbuf[2] == 0x21)
        {
            USB_BULK_Queue_Write(&USB_RX_QueueData,u8buf,sendnum);
            OBDII_ReadBuf(recvbuf, &number,delay*30);
            number = unpack_buff(recvbuf,number);
        }
        else if(recvbuf[2] == 0x78)
        {
            OBDII_ReadBuf(recvbuf, &number,delay*30);
            number = unpack_buff(recvbuf,number);
        }
        else
        {
            break;
        }
        vTaskDelay(delay);
    }
    return number;
}

uint16_t pro_SendRecv_HondaKwpA(uint8_t *cmdbuf, uint8_t *recvbuf)//本田用到的KWP2000协议收发
{
    uint16_t number;
    uint8_t sendbuf[128];
    uint8_t u8buf[128];
    int sendnum;
    int i;
    //int pack_t = 2;
    int count = 10;
    int retry = 1;
    int delay = g_defaultMsgAttr.m_AnswerFrameTimeOut;//ecu??????

    //打包
    sendnum = pack_type4(cmdbuf+1,u8buf,cmdbuf[0]);

    for(i=0; i<retry; i++)
    {
        USB_BULK_Queue_Write(&USB_RX_QueueData,u8buf,sendnum);
        OBDII_ReadBuf(recvbuf, &number,delay*30);
        number = unpack_buf4(recvbuf,number);
        if(number > 0)
        {
            break;
        }
    }
    while(number > 0 && recvbuf[0] == 0x7f)//chengbb 20130423 ?????????? ????
    {
        if(recvbuf[2] == 0x21)
        {
            USB_BULK_Queue_Write(&USB_RX_QueueData,u8buf,sendnum);
            OBDII_ReadBuf(recvbuf, &number,delay*30);
            number = unpack_buf4(recvbuf,number);
        }
        else if(recvbuf[2] == 0x78)
        {
            OBDII_ReadBuf(recvbuf, &number,delay*30);
            number = unpack_buf4(recvbuf,number);
        }
        else
        {
            break;
        }
        vTaskDelay(delay);
    }
    return number;
}


uint16_t pro_SendRecv_OBDII_MORE(uint8_t* cmdbuf, uint8_t *recvbuf, uint8_t msgnum)
{
    uint8_t i=0;
    uint8_t pos =0;
    uint16_t number=0,count=0;
    uint8_t buffer[32];

//pos = cmdbuf[0];
    number = pro_SendRecv(cmdbuf, buffer);
    if(number < pos)
    {
        return 0;
    }
    number -= pos;
    memcpy(recvbuf, buffer+pos, number);
    count += number;
    for(i=1; i<msgnum; i++)
    {
        memset(buffer,0,sizeof(buffer));
        printfUartObdiideBug("index == %d",i);
        number = pro_OBDII_ReadBuffer(buffer);
        printfUartObdiideBug("number=%d\r\n",number);
        if(number == 0)
        {
            break;
        }
        number -= pos;
        memcpy(recvbuf+count, buffer+pos, number);
        count += number;
    }
    return count;
}


uint8_t cmd_get_ecumodule()
{
    return m_ecumodule;
}

uint8_t cmd_set_ecumodule(uint8_t module)
{
    m_setmodule = module;
    return m_setmodule;
}

uint8_t cmd_get_modulenum()
{
    return m_modulenum;
}

uint8_t cmd_set_modulenum(uint8_t number)
{
    m_modulenum = number;
    return m_modulenum;
}

uint8_t cmd_set_two_id_toyota(uint8_t id)//toyota
{
    m_two_id_toyota = id;
    return m_two_id_toyota;
}

uint8_t cmd_get_two_id_toyota()//toyota
{
    return m_two_id_toyota;
}

uint16_t pro_SendRecv(uint8_t * sendbuf, uint8_t *recvbuf)
{
    int number,i;
    uint8_t pckType;
    uint8_t delay = 20;

    pckType = g_defaultMsgAttr.m_PCK_TYPE;

#if defined(TOM_RDWR_DEBUG)
    printfUartObdiideBug("[%s] sendbuf: \r\n",__func__);
    for(i=0; i<=sendbuf[0]; i++)
        printfUartObdiideBug("%02x ",sendbuf[i]);
    printfUartObdiideBug("\r\n");
#endif

    switch(pckType)
    {
        //Normal chengbb 20110519
    case 0x00:
        number = pro_SendRecv_Normal(sendbuf, recvbuf);
        break;
        //CANBUS
    case 0x20:
        number = pro_SendRecv_CanDefault(sendbuf, recvbuf);
        break;
    case 0x21:
        number = pro_SendRecv_CanTwoID(sendbuf, recvbuf);//toyota
        break;
        //CANBUS
    case 0x22:
        number = pro_SendRecv_CanNormal(sendbuf, recvbuf);
        break;
    case 0x02:
        number = pro_SendRecv_KwpA(sendbuf, recvbuf);//toyota
        break;
    case 0x0d:
        number = pro_SendRecv_KwpD(sendbuf, recvbuf);
        break;
    case 0x05:
        number = pro_SendRecv_ISO(sendbuf, recvbuf);
        break;
    case 0x09:
        number = pro_SendRecv_PWM(sendbuf, recvbuf);
        break;
    case 0x0b:
        number = pro_SendRecv_VPW(sendbuf, recvbuf);
        break;
    case 0x0f://
        number = pro_SendRecv_HondaPro(sendbuf, recvbuf);
        break;
    case 0x04://
        number = pro_SendRecv_HondaKwpA(sendbuf, recvbuf);
        break;
    case 0x30:
        number = pro_SendRecv_NISSAN(sendbuf,recvbuf);
        break;
    case 0x53:
        number = pro_SendRecv_FORD_MZD(sendbuf,recvbuf);
        break;
		case 0x12:
        number = pro_SendRecv_KwpA_X(sendbuf,recvbuf);
        break;
    default:
        number = 0;
        break;
    }

#if defined(TOM_RDWR_DEBUG)
    printfUartObdiideBug("[%s] recvbuf: \r\n",__func__);
    for(i=0; i<number; i++)
        printfUartObdiideBug("%02X ",recvbuf[i]);
    printfUartObdiideBug("\r\n");
#endif

    vTaskDelay(20);
    return number;
}

//本田协议 0x0f 打包方式
int pack_typef(uint8_t *u8bufin, uint8_t *u8buf, intptr_t bufsize)
{
    int number = 5;
    int i;
    char *p;
    char *p0 ;

    memcpy(u8buf+number,u8bufin,bufsize);

    //处理
    number += bufsize;
    if(u8buf[6]+5==number)
        number=number-1;

    //校验
    u8buf[number] = 0;
    for(i = 5; i<number; i++)
    {
        u8buf[number] += u8buf[i];
    }
    u8buf[number]=~u8buf[number]+1;
    number++;

    //头
    u8buf[0] = 0xa5;
    u8buf[1] = 0xa5;
    u8buf[2] = (number-4) / 0x100;
    u8buf[3] = (number-4) % 0x100;
//	p = (char*)ezxml_attr(xml,"ORD");
//	if(p)
//		u8buf[4] = getHex(p);
//	else
    u8buf[4] = g_defaultMsgAttr.m_ORD;


    //校验和
    u8buf[number] = 0;
    for(i = 2; i<number; i++)
    {
        u8buf[number] += u8buf[i];
    }
    u8buf[number] = ~u8buf[number];

    return number+1;
}

//本田协议 0x0f 解包
int unpack_buff(uint8_t *buf, int bufsize)
{
    int number=0;
    int i;
    uint8_t len;

    if(bufsize < 8)
        return 0;
    number=5;

    //长度校验
    len = buf[6];

    //if(bufsize < len+6)
    //	return 0;

    //校验和校验
    //for(i = 0; i< len+3;i++)
    //{
    //	hex += buf[5+i];
    //}
    //if(hex != buf[5+i])
    //	return 0;

    for(i=0; i<len; i++)
    {
        buf[i]= buf[number+i];
    }
    buf[i] = '\0';

    return len;

}

//本田KWP2000 0x04打包格式
int pack_type4(uint8_t *u8bufin, uint8_t *u8buf, intptr_t bufsize)
{
    int number = 5;
    int i;
    char *p;
    char *p0 ;
    uint8_t buf[256];

    //??
    u8buf[number++] = 0x80;

//	p=(char*)ezxml_attr(xml,"TGT");
//	if(p)
//		u8buf[number++] = getHex(p);
//	else
    u8buf[number++] = g_defaultMsgAttr.m_TGT;

//	p =(char*)ezxml_attr(xml,"SRC");

//	if(p)
//		u8buf[number++] = getHex(p);
//	else
    u8buf[number++] = g_defaultMsgAttr.m_SRC;

    //u8buf[number++] =getHexEx(ezxml_txt(xml),buf);
    u8buf[number++] = bufsize;

    //number+=getHexEx(p0,u8buf+number);
    memcpy(u8buf+number,u8bufin,bufsize);
    number+=bufsize;

    u8buf[number] = 0;
    for(i = 5; i<number; i++)
    {
        u8buf[number] += u8buf[i];
    }
    number++;

    //?
    u8buf[0] = 0xa5;
    u8buf[1] = 0xa5;
    u8buf[2] = (number-4) / 0x100;
    u8buf[3] = (number-4) % 0x100;
//	p = (char*)ezxml_attr(xml,"ORD");
//	if(p)
//		u8buf[4] = getHex(p);
//	else
    u8buf[4] = g_defaultMsgAttr.m_ORD;


    //???
    u8buf[number] = 0;
    for(i = 2; i<number; i++)
    {
        u8buf[number] += u8buf[i];
    }
    u8buf[number] = ~u8buf[number];

    return number+1;
}

//本田KWP2000 0x04解包
int unpack_buf4(uint8_t *buf, int bufsize)
{
    int number=0;
    int i;
    uint8_t len;
    //U8 hex = 0;

    if(bufsize < 8)
        return 0;

    //????
    len = buf[5]&0x3f;
    if(len==0)
    {
        len=buf[8];
        number=9;
    }
    else
        number=8;

    if(bufsize < number+ len+1)
        return 0;

    //?????
    //for(i = 0; i< len+3;i++)
    //{
    //	hex += buf[5+i];
    //}
    //if(hex != buf[5+i])
    //	return 0;

    for(i=0; i<len; i++)
    {
        buf[i]= buf[number+i];
    }
    buf[i] = '\0';
    return len;
}





void Debug_buf(const char *tip, const uint8_t* buf,uint16_t number )
{
    int i = 0;
    char ch[4] = {0};
    char str[128] = {0};

    if(number>0)
    {
        for(i = 0; (i<number)&&(i<41); i++)
        {
            sprintf(ch,"%02X ",buf[i]);
            strcat(str,ch);
        }
        if(i>=41)
        {
            strcat(str,"超长");
        }
        printf(" %s%s\r\n",tip,str);
    }
    else
    {
        printfUartObdiideBug(" %s\r\n",tip);
    }
}




int pro_unpackmzd(uint8_t *buf, int bufsize)
{
    int m=0;
    int i,j;
    int n = 0;
    uint8_t u8buf[128];
    int number = 0 ;
    if(bufsize < 9)
    {
        return 0;
    }
    for(i=5; i<bufsize; i++)
    {
        buf[m++] = buf[i];
    }
    m=bufsize-5;
    if(m > (buf[0]/16+2))
    {
        number = m;
        for(j = 0; number >1; )
        {
            memcpy(u8buf, buf+j, number);
            n = u8buf[0]/16+1;
            u8buf[n] = 0;
            for(i = 0; i<n-1; i++)
            {
                u8buf[n] += u8buf[i];
            }
            if(u8buf[n]!=u8buf[n-1])
                return 0;
            number = number-n;
            j = j+n;
        }
        return m-1;
    }
    else
    {
        buf[m] = 0;
        for(i = 0; i<m-2; i++)
        {
            buf[m] += buf[i];
        }
        if(buf[m]!=buf[m-2])
            return 0;
    }
    return m-2;
}

int pro_SendRecv_FORD_MZD_Recvmore(uint8_t *recvbuf)
{
    uint8_t u8buf[50];
    uint8_t databuf[256];
    int count=0;
    uint16_t number;
    int i=0;
    uint8_t dataNum;
    int j;

    OBDII_ReadBuf(u8buf, &number, g_defaultMsgAttr.m_AnswerFrameTimeOut*20);
    printfUartObdiideBug("first recv:\r\n");
    for(i = 0 ; i < number ; i++)
        printfUartObdiideBug("%02x ",u8buf[i]);
    printfUartObdiideBug("\r\n");
    number = pro_unpackrecvdata(u8buf, number);
    printfUartObdiideBug("first recv unpack:\r\n");
    for(i = 0 ; i < number ; i++)
        printfUartObdiideBug("%02x ",u8buf[i]);
    printfUartObdiideBug("\r\n");
    memcpy(databuf, u8buf, number);
    while(number > 0)//?????????,??????
    {
        count+=number;
        OBDII_ReadBuf(u8buf, &number, g_defaultMsgAttr.m_AnswerFrameTimeOut*20);
        printfUartObdiideBug("more  recv :\r\n");
        for(i = 0 ; i < number ; i++)
            printfUartObdiideBug("%02x ",u8buf[i]);
        printfUartObdiideBug("%d\r\n" , number);
        number = pro_unpackrecvdata(u8buf, number);
        printfUartObdiideBug("more  recv unpack:\r\n");
        for(i = 0 ; i < number ; i++)
            printfUartObdiideBug("%02x ",u8buf[i]);
        printfUartObdiideBug("%d\r\n" , number);
        memcpy(databuf+count, u8buf, number);
    }
    if (count < 4)
    {
        return 0;
    }
    else if(count<12)
    {
        memcpy(recvbuf, databuf+3, count-4);
        dataNum = count-4;
    }
    else
    {
        j = 11;
        number = count/j;
        for (i=0; i<number; i++)
        {
            dataNum = 7;
            if (i==0)
            {
                memcpy(recvbuf,databuf+3,dataNum);
                continue;
            }
            memcpy(recvbuf+dataNum, databuf+(i*j+4), 6);
            dataNum+=6;
        }
        if (count-(number*j)>0)
        {
            memcpy(recvbuf+dataNum, databuf+(i*j+4), count-(number*j)-5);
            dataNum+=count-(number*j)-5;
        }
    }
    m_Dtcnum = 0;
    return dataNum;
}

int pro_SendRecv_FORD_MZD_Recv(uint8_t *recvbuf)
{
    uint16_t number;
    OBDII_ReadBuf(recvbuf, &number, g_defaultMsgAttr.m_AnswerFrameTimeOut*20);
    number = pro_unpackmzd(recvbuf, number);
    if(number < 3)
    {
        return 0;
    }
    return number;
}

uint16_t pro_SendRecv_FORD_MZD(uint8_t * cmddbuf, uint8_t *recvbuf)
{
    int number = 0;
    uint8_t sendbuf[128] = {0};
    uint8_t u8buf[128] = {0};
    int sendnum = 0;
    int i = 0;
    int pack_t = 2;
    int count = 10;
    int retry = 1;
    int delay = g_defaultMsgAttr.m_AnswerFrameTimeOut;//ecu??????
    number = cmddbuf[0];
    memcpy(u8buf+3, cmddbuf+1, number);
    u8buf[0] = 0x34 + number*0x10;
    u8buf[1] = g_defaultMsgAttr.m_TGT;
    u8buf[2] = g_defaultMsgAttr.m_SRC;
    number += 4;
    u8buf[number-1] = 0;
    for(i=0; i<number-1; i++)
    {
        u8buf[number-1] += u8buf[i];
    }
    sendnum = pro_packsenddata(u8buf, number,g_defaultMsgAttr.m_ORD);
    for(i=0; i<retry; i++)
    {
        USB_BULK_Queue_Write(&USB_RX_QueueData,u8buf,sendnum);
        if(m_Dtcnum > 1)
        {
            number = pro_SendRecv_FORD_MZD_Recvmore(recvbuf);
        }
        else
        {
            number = pro_SendRecv_FORD_MZD_Recv(recvbuf);
            if (number > 0 && recvbuf[2] == u8buf[6] && recvbuf[1] == u8buf[7])//20161128 ????? 1998 E-Series,??id????
            {
                number-=3;
                memcpy(recvbuf,recvbuf+3,number);
            }
            else
            {
                number = 0;
            }
        }
        if(number > 0)
        {
            break;
        }
    }
    while(number > 2 && recvbuf[0] == 0x7f)//chengbb 20130423 ?????????? ????
    {
        if(recvbuf[2] == 0x21)
        {
            USB_BULK_Queue_Write(&USB_RX_QueueData,u8buf,sendnum);
            if(m_Dtcnum > 1)
            {
                number = pro_SendRecv_FORD_MZD_Recvmore(recvbuf);
            }
            else
            {
                number = pro_SendRecv_FORD_MZD_Recv(recvbuf);
                if (number > 0 && recvbuf[2] == u8buf[6] && recvbuf[1] == u8buf[7])//20161128 ????? 1998 E-Series,??id????
                {
                    number-=3;
                    memcpy(recvbuf,recvbuf+3,number);
                }
                else
                {
                    number = 0;
                }
            }
        }
        else if(recvbuf[2] == 0x78)
        {
            if(m_Dtcnum > 1)
            {
                number = pro_SendRecv_FORD_MZD_Recvmore(recvbuf);
            }
            else
            {
                number = pro_SendRecv_FORD_MZD_Recv(recvbuf);
                if (number > 0 && recvbuf[2] == u8buf[6] && recvbuf[1] == u8buf[7])//20161128 ????? 1998 E-Series,??id????
                {
                    number-=3;
                    memcpy(recvbuf,recvbuf+3,number);
                }
                else
                {
                    number = 0;
                }
            }
        }
        else
        {
            break;
        }
        vTaskDelay(delay);
    }
    return number;
}
