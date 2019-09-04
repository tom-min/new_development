/*
 * FPGA_Drives.h
 *
 * Created: 2018/3/15 10:06:03
 *  Author: ppcc-02
 */ 


#ifndef FPGA_DRIVES_H_
#define FPGA_DRIVES_H_
#include "common.h"

//����Э�������ƶ��� ����λ��ͨ��Э��2018.3.22
#define PROTOCOL_LINE_NC			0x00	//û���κ�Э�飬����
#define PROTOCOL_LINE_HI			0x01	//Differential Line - High (e.g. DW_CAN High)
#define PROTOCOL_LINE_LOW			0x02	//Differential Line - Low (e.g. DW_CAN Low)
#define PROTOCOL_LINE_K				0x03	//UART K-Line (e.g. KWP2000)
#define PROTOCOL_LINE_L				0x04	//UART L-Line (e.g. ISO 9141-2)
#define PROTOCOL_LINE_TX			0x05	//TX LINE
#define PROTOCOL_LINE_RX			0x06	//RX LINE
#define PROTOCOL_LINE_PLUS			0x07	//Plus line J1850
#define PROTOCOL_LINE_MINUS			0x08	//Minus line J1850
#define PROTOCOL_LINE_SINGLE		0x09	//Single Line
#define PROTOCOL_LINE_IGN			0x0a	//Pin for detection of Ignition Sense State
#define PROTOCOL_LINE_PROGV			0x0b	//Pin for Reading and Setting the Programable Voltage
#define PROTOCOL_LINE_TX_PLUS		0x0c	//IEEE 802.3 TX+ line
#define PROTOCOL_LINE_TX_MINUS		0x0d	//IEEE 802.3 TX- line
#define PROTOCOL_LINE_RX_PLUS		0x0e	//IEEE 802.3 RX+ line
#define PROTOCOL_LINE_RX_MINUS		0x0f	//IEEE 802.3 RX- line

//ODB26��λ����
#define DB26_PIN_NC		0
#define DB26_PIN_1		1
#define DB26_PIN_2		2
#define DB26_PIN_3		3
#define DB26_PIN_6		6
#define DB26_PIN_7		7
#define DB26_PIN_8		8
#define DB26_PIN_9		9
#define DB26_PIN_10		10
#define DB26_PIN_11		11
#define DB26_PIN_12		12
#define DB26_PIN_13		13
#define DB26_PIN_14		14
#define DB26_PIN_15		15


typedef union {
	struct {
		/* bit:  0..23  FPGA JC���ƽ� */
		uint32_t JC0:1;
		uint32_t JC1:1;
		uint32_t JC2:1;
		uint32_t JC3:1;
		uint32_t JC4:1;
		uint32_t JC5:1;
		uint32_t JC6:1;
		uint32_t JC7:1;
		uint32_t JC8:1;
		uint32_t JC9:1;
		uint32_t JC10:1;
		uint32_t JC11:1;
		uint32_t JC12:1;
		uint32_t JC13:1;
		uint32_t JC14:1;
		uint32_t JC15:1;
		uint32_t JC16:1;
		uint32_t JC17:1;
		uint32_t JC18:1;
		uint32_t JC19:1;
		uint32_t JC20:1;
		uint32_t JC21:1;
		uint32_t JC22:1;
		uint32_t JC23:1;
		/* bit:  24..29  NC    */
		uint32_t NC:6;
		/* bit:  30     TXD������� */
		uint32_t NON_TXD:1;
		/* bit:  31     RXD�������*/
		uint32_t NON_RXD:1;
	} bit;
	/* Type used for register access */
	uint32_t reg;
}FPGA_C_REG_L32_Type;

typedef union {
	struct {
		/* bit:  0..3     ����ģʽ���� */
		uint8_t TXDMODE:4;
		/* bit:  4..7     ����ģʽ����*/
		uint8_t RXDMODE:4;
	} bit;
	/* Type used for register access */
	uint8_t reg;
}FPGA_C_REG_H8_Type;

typedef struct {
	volatile FPGA_C_REG_L32_Type L32;
	volatile FPGA_C_REG_H8_Type  H8;
}FPGA_C_REG;

extern FPGA_C_REG	PDU_FPGA_C_REG;
 

#define DISABLE_CAN0_BUS				PDU_FPGA_C_REG.L32.bit.JC1=0
#define ENABLE_CAN0_BUS					PDU_FPGA_C_REG.L32.bit.JC1=1
#define ENABLE_J1850_BUS0_P_PD			PDU_FPGA_C_REG.L32.bit.JC12=1
#define DISABLE_J1850_BUS0_P_PD			PDU_FPGA_C_REG.L32.bit.JC12=0
#define ENABLE_J1850_BUS0_M_PU			PDU_FPGA_C_REG.L32.bit.JC14=1
#define DISABLE_J1850_BUS0_M_PU			PDU_FPGA_C_REG.L32.bit.JC14=0
#define SET_J1850_VPW_BUS_8V			PDU_FPGA_C_REG.L32.bit.JC19=1
#define SET_J1850_PWM_BUS_5V			PDU_FPGA_C_REG.L32.bit.JC19=0

#define SET_DWFTCAN_TO_MUX_OUTPUT		PDU_FPGA_C_REG.L32.bit.JC5=1			//���ݴ�CAN�л�������������
#define SET_MUX_INPUT_TO_MUX_OUTPUT		PDU_FPGA_C_REG.L32.bit.JC5=0			//�ѻ�����������л�������������
#define SET_CAN1_TO_MUX_INPUT			PDU_FPGA_C_REG.L32.bit.JC4=1			//CAN1�л��������������
#define SET_LINE01_TO_MUX_INPUT			PDU_FPGA_C_REG.L32.bit.JC4=0			//LINE0,LINE1�л����������	

#define SET_DOIP_TO_DB26				PDU_FPGA_C_REG.L32.bit.JC6=1			//��RJ45 pin�л���DB26
#define RESET_DOIP_TO_DB26				PDU_FPGA_C_REG.L32.bit.JC6=0			//��RJ45 pin���л���DB26

#define SET_LINE0_PULL_UP				PDU_FPGA_C_REG.L32.bit.JC0=1			//��LINE0��������1K����    ��������PDU_FPGA_C_REG.L32.bit.NON_TXD = 1;  //TXD����
#define RESET_LINE0_PULL_UP				PDU_FPGA_C_REG.L32.bit.JC0=0			//��LINE0���ò�����
#define SET_LINE0_PULL_DOWN				PDU_FPGA_C_REG.L32.bit.JC2=1			//��LINE0��������510ŷ����  ��������PDU_FPGA_C_REG.L32.bit.NON_TXD = 0;  //TXDͬ��
#define RESET_LINE0_PULL_DOWN			PDU_FPGA_C_REG.L32.bit.JC2=0			//��LINE0���ò�����
#define SET_LINE1_PULL_UP				PDU_FPGA_C_REG.L32.bit.JC8=1			//��LINE1��������4.7K����
#define RESET_LINE1_PULL_UP				PDU_FPGA_C_REG.L32.bit.JC8=0			//��LINE1���ò�����
#define SET_LINE1_PULL_DOWN				PDU_FPGA_C_REG.L32.bit.JC3=1			//��LINE1��������1Kŷ����
#define RESET_LINE1_PULL_DOWN			PDU_FPGA_C_REG.L32.bit.JC3=0			//��LINE1���ò�����

#define SET_CAN1_RES					PDU_FPGA_C_REG.L32.bit.JC18=1			//����CAN1ƥ�����60ŷ
#define RESET_CAN1_RES					PDU_FPGA_C_REG.L32.bit.JC18=0			//������CAN1ƥ�����60ŷ

#define SET_LINE01_8V					PDU_FPGA_C_REG.L32.bit.JC20=1			//
#define RESET_LINE01_8V					PDU_FPGA_C_REG.L32.bit.JC20=0
#define SET_LINE01_12_24V				PDU_FPGA_C_REG.L32.bit.JC16=1			//
#define RESET_LINE01_12_24V				PDU_FPGA_C_REG.L32.bit.JC16=0


#define SET_LINE0_ADC_CHANNEL			PDU_FPGA_C_REG.L32.bit.JC11=1			//��BUS1_ADC_PC31�ܽ��е�Line0  ���ڶ�Line0���ϵ�ѹ
#define SET_LINE1_ADC_CHANNEL			PDU_FPGA_C_REG.L32.bit.JC11=0			//��BUS1_ADC_PC31�ܽ��е�Line1  ���ڶ�Line1���ϵ�ѹ

#define SET_CMP_ADC_CHANNEL			    PDU_FPGA_C_REG.L32.bit.JC10=1			//��BUS1_ADC_PA19�ܽŶ��Ƚϵ�ѹ��
#define SET_DRV_ADC_CHANNEL				PDU_FPGA_C_REG.L32.bit.JC10=0			    //��BUS1_ADC_PA19�ܽ�����������ѹ 5V��8V��12V��24V


extern const uint8_t DB26_TO_Decoder[16];
#if 0
 
#endif
#endif /* FPGA_DRIVES_H_ */