
//#include <asf.h>
#include "config.h"
//extern void ALL_GPIO_Config(void);
extern void SET_Another_STATUS(uint8 OUT_STATUS);
extern void SET_LINE_STATUS(uint8 OUT_STATUS);
extern uint8 GET_LINE_STATUS(void);
extern void  SetCChannel(uint8 ChannelNum);
extern void Reset_Line(void);
extern void Reset_All_IO(void);
extern uint8 PassThru_Default_SetConfig(void);
extern uint8 Set_DrvPort(uint8 PIN1, uint8 PIN2, uint8 LOGIC_V1, uint8 LOGIC_V2,uint8 GND_COM);
extern void  SetLine1Channel(uint8 ChannelNum);
extern void  SetLine0Channel(uint8 ChannelNum);
extern uint32 DelayN_Count_Get_Logic_fpclk_1000(uint32 n_Count,uint8 line_status);
extern uint32 DelayN_Count_Get_Logic_count(uint32 n_Count,uint8 line_status);
extern void SetLine_ext(uint8 Line0_Channel,uint8 Line1_channel);
extern uint8 Get_Bus_status(void);
extern void SetCompV(uint8 CompV);
extern uint32 Read_Dac(uint8 Vcc_Pass_Select);
//tx_mode
#define  tx_mode_LINE0 0x00//1	0	0	0	0	"LINE0由MCU_TXD_FPGA_PD19，GPIO_TXD0 发送数据；line1由GPIO_TXD1控制发送数据
//Control Register 中的“TXD反相控制”使能，可以配置发送数据是否反相"
#define  tx_mode_LINE1 0x01   //2	0	0	0	1	//"LINE1由MCU_TXD_FPGA_PD19，GPIO_TXD0 发送数据；line0由GPIO_TXD1控制发送数据
//Control Register 中的“TXD反相控制”使能，可以配置发送数据是否反相"
#define  tx_mode_LINE0ANDLINE1 0x02  //3	0	0	1	0	LINE0，LINE1同相发送（由MCU_TXD_FPGA_PD19，GPIO_TXD0发送数据）                                          Control Register 中的“TXD反相控制”使能，可以配置发送数据是否反相
#define  tx_mode_DIFFERENTIAL 0x03  //4	0	0	1	1	LINE0，LINE1差分发送（由GPIO_TXD0 发送数据）；                                      Control Register 中的“TXD反相控制”使能，可以配置发送数据是否反相
#define  tx_mode_TW 0x04  //5	0	1	0	0	"LINE0可推挽发送模式：
//TXD00_CT：由GPIO_TXD0控制；
//TXD01_CT：由GPIO_TXD1控制；"

#define  tx_mode_SINGLE_CAN 0x05 //6	0	1	0	1	单线CAN
#define  tx_mode_HIGH_CAN 0x06 //7	0	1	1	0	高速CAN
#define  tx_mode_DWFT_CAN 0x07 //8	0	1	1	1	容错CAN
#define  tx_mode_0reserve8 0x08 //9	1	0	0	0	预留，默认和第16中方式一样
#define  tx_mode_reserve9 0x09 //10	1	0	0	1	预留，默认和第16中方式一样
#define  tx_mode_reserve10 0x0a //11	1	0	1	0	预留，默认和第16中方式一样
#define  tx_mode_reserve11 0x0b //12	1	0	1	1	预留，默认和第16中方式一样
#define  tx_mode_reserve12 0x0c //13	1	1	0	0	预留，默认和第16中方式一样
#define  tx_mode_reserve13 0x0d //14	1	1	0	1	预留，默认和第16中方式一样
#define  tx_mode_reserve14 0x0e //15	1	1	1	0	预留，默认和第16中方式一样
#define  tx_mode_high_res15 0x0f //16	1	1	1	1	禁止发送数据，配合上下拉电阻可以实现高阻态（悬空）

#define  REVERSE  0
#define  NORMAL  1
#define  MODEMASK  0XF0