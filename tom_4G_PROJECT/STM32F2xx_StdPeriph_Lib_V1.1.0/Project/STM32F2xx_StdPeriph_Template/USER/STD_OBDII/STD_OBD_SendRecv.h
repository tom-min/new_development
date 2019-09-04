#ifndef _OBD_SENDRECV_H_
#define _OBD_SENDRECV_H_


#include "user_queue.h"
#include "STD_OBD_CMD_PROCESS.h"

void setcanbusflowid(uint32_t canbusflowid);
void setcanbussendid(uint32_t canbussendid);
void setcanbusreadid(uint32_t canbusreadid);
void setcanbusflowidchang(uint32_t canbusflowid);
void setcanbusreadidchang(uint32_t canbusreadid);
uint32_t getcanbusflowid();
uint32_t getcanbusreadid();
uint32_t getcanbusflowidchang();
uint32_t getcanbusreadidchang();

uint16_t pro_packsenddata(uint8_t *sendbuf, uint16_t number, uint8_t msgord);
uint16_t pro_unpackrecvdata(uint8_t *recvbuf, uint16_t number);
uint32_t CommPackBufferDefault(uint8_t *buffer, uint32_t count);
uint32_t CommPackBufferCanbus(uint8_t *buffer, uint32_t canid, uint32_t count);
uint32_t CommUnpackBufferDeafult(uint8_t *buffer, uint32_t count);
uint8_t OBDII_ReadBuf(uint8_t *p_obdii_buff, uint16_t * obdii_buff_len, uint32_t obdii_outtime);
uint32_t CommReadSingleCanbus(uint8_t* readbuf, uint16_t maxlen, uint32_t delay);
uint32_t CommSendBufferCanbus(uint8_t* sendbuf, uint16_t sendnum);
uint16_t pro_SendRecv_CanDefault_Recvmore(uint8_t *recvbuf, uint32_t delay);
uint8_t GetCrc(uint8_t  val,uint8_t  crc) ;
uint16_t pro_SendRecv(uint8_t * cmdbuf, uint8_t *recvbuf);
uint16_t pro_SendRecv_Normal(uint8_t * cmdbuf, uint8_t *recvbuf);
uint16_t pro_SendRecv_CanDefault(uint8_t * sendbuf, uint8_t *recvbuf);
uint16_t pro_SendRecv_CanNormal(uint8_t * sendbuf, uint8_t *readbuf);
uint16_t pro_SendRecv_CanTwoID(uint8_t *sendbuf, uint8_t *recvbuf);//toyota
uint16_t pro_SendRecv_CanTwoID_Send(uint8_t *sendbuf, int number);//toyota
uint16_t pro_SendRecv_CanTwoID_Recvmore(uint8_t *recvbuf, int delay);//toyota
uint16_t pro_SendRecv_CanDefault_Send(uint8_t * sendbuf, uint16_t number);
uint16_t pro_SendRecv_InitIo(uint8_t * sendbuf, uint8_t *recvbuf);
uint16_t pro_SendRecv_KwpA(uint8_t * cmddbuf, uint8_t *recvbuf);//toyota
uint16_t pro_SendRecv_KwpD(uint8_t * cmddbuf, uint8_t *recvbuf);
//add by znf 
uint16_t pro_SendRecv_KwpE(uint8_t * cmddbuf, uint8_t *recvbuf); //�����ʽE �ղ�ʹ��
uint16_t pro_SendRecv_KwpF(uint8_t * cmddbuf, uint8_t *recvbuf); //�����ʽF �ղ�ʹ��
uint16_t pro_SendRecv_KwpB(uint8_t * cmddbuf, uint8_t *recvbuf);
uint16_t pro_SendRecv_NISSAN(uint8_t * cmddbuf, uint8_t *recvbuf);
uint16_t pro_ReadBuf_Kwp2000(uint8_t *recvbuf, uint32_t delay);
uint16_t pro_ReadBuf_Kwp2003(uint8_t *recvbuf, uint32_t delay);  //�ղ�ר�ý������
uint16_t pro_SendRecv_ISO(uint8_t * cmdbuf, uint8_t *recvbuf);
uint16_t pro_OBDII_ReadBuffer(uint8_t *recvbuf);
uint16_t pro_SendRecv_PWM(uint8_t * cmdbuf, uint8_t *recvbuf);
uint16_t pro_SendRecv_VPW(uint8_t * cmdbuf, uint8_t *recvbuf);
uint16_t pro_SendRecv_HondaPro(uint8_t *cmdbuf, uint8_t *recvbuf);//����Э���շ�
uint16_t pro_SendRecv_HondaKwpA(uint8_t *cmdbuf, uint8_t *recvbuf);//�����õ���KWP2000Э���շ�
uint8_t cmd_set_ecumodule(uint8_t module);
uint8_t cmd_get_ecumodule();
uint8_t cmd_set_modulenum(uint8_t number);
uint8_t cmd_get_modulenum();
uint8_t cmd_set_two_id_toyota(uint8_t id);//toyota
uint8_t cmd_get_two_id_toyota();//toyota
uint16_t pro_SendRecv_OBDII_MORE(uint8_t* cmdbuf, uint8_t *recvbuf, uint8_t msgnum);
int pack_typef(uint8_t *u8bufin, uint8_t *u8buf, intptr_t bufsize);//����Э�� 0x0f �����ʽ
int unpack_buff(uint8_t *buf, int bufsize);//����Э�� 0x0f �����ʽ
int pack_type4(uint8_t *u8bufin, uint8_t *u8buf, intptr_t bufsize);//����Э�� 0x04 �����ʽ
int unpack_buf4(uint8_t *buf, int bufsize);//����Э�� 0x04 �����ʽ

//�����շ�����
int pro_unpackmzd(uint8_t *buf, int bufsize);
int pro_SendRecv_FORD_MZD_Recvmore(uint8_t *recvbuf);
int pro_SendRecv_FORD_MZD_Recv(uint8_t *recvbuf);
uint16_t pro_SendRecv_FORD_MZD(uint8_t * cmddbuf, uint8_t *recvbuf);
uint16_t pro_SendRecv_PWM_Recvmore(uint8_t *recvbuf);
uint16_t pro_SendRecv_PWM_FORD(uint8_t * cmdbuf, uint8_t *recvbuf);
uint16_t pro_SendRecv_PWM_Recv(uint8_t *recvbuf);
uint16_t pro_ReadBuf_Kwp2000_X(uint8_t *recvbuf, uint32_t delay);
uint16_t pro_SendRecv_KwpA_X(uint8_t * cmddbuf, uint8_t *recvbuf);
//�������Դ��շ�����


void Debug_buf(const char *tip, const uint8_t* buf,uint16_t number );//��ӡdebug��Ϣ

#endif