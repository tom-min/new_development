/*
 * hal_spi.c
 *
 * Created: 2018/3/7 14:40:08
 *  Author: ppcc-02
 */ 
 
#include <string.h>
#include "hal_spi.h"
//#include "conf_board.h"
//#include "FPGA_Drives.h"

/**
 * \brief Initialize SPI as master.
 */
void hal_spi_master_init(void)
{
	 
}
void hal_spi_master_init3(void)
{
 
}

void hal_soft_spi_master_init(void)
{
 
}

void hal_spi_delay(void)  //1.6us
{
	volatile uint32_t i,j;
	for (i=0;i<5;i++)
	{
		j++;
	}
}

void tx_spi_one_byte(uint8_t dat,uint8_t num)
{
 
}

////static
//void tx_c_channel(void )
//{
	//uint8_t Control_Register[5];
	//memset(Control_Register,0,5);
	//Control_Register[0] = 0x02;  //JC[1]=1  CAN0 6 14 断开
	//
	//DIS_MODEL_SELECT;//LPC_GPIO2->FIOSET=(1<<CTRL_EN);	//先禁止有效
	//hal_spi_delay();
	//tx_spi_one_byte(Control_Register[4],8);
	//hal_spi_delay();
	//hal_spi_delay();
	//tx_spi_one_byte(Control_Register[3],8);
	//hal_spi_delay();
	//hal_spi_delay();
	//tx_spi_one_byte(Control_Register[2],8);
	//hal_spi_delay();
	//hal_spi_delay();
	//tx_spi_one_byte(Control_Register[1],8);
	//hal_spi_delay();
	//hal_spi_delay();
	//tx_spi_one_byte(Control_Register[0],8);
	//hal_spi_delay();
	//EN_MODEL_SELECT;//LPC_GPIO2->FIOCLR=(1<<CTRL_EN);	//设置完了再输出有效信号
//
//}

void tx_c_channel1(void )
{



}

//static
void tx_c_channel(FPGA_C_REG PDU_FPGA_C_REG)
{


}
//FPGA_JA CAN_H			//FPGA_JB CAN_L    //FPGA JA JB CAN_H CAN_L LINE0 LINE1 对应关系
void tx_PINSEL_channel(uint8_t JA_LINE0_Pin_Num,uint8_t JB_LINE1_Pin_Num)  //LINE0-->CAN_H  LINE1-->CAN_L 
{

}

void tx_PINSEL_channel1(void)
{


}



