/*
 * hal_spi.h
 *
 * Created: 2018/3/7 14:40:40
 *  Author: ppcc-02
 */ 


#ifndef HAL_SPI_H_
#define HAL_SPI_H_
#include "FPGA_Drives.h"

/* Chip select. */
#define SPI_CHIP_SEL_CS1		1		//PD25
#define SPI_CHIP_PCS_CS1 spi_get_pcs(SPI_CHIP_SEL_CS1)
/* Chip select. */
#define SPI_CHIP_SEL_CS3		3		//PD27
#define SPI_CHIP_PCS_CS3 spi_get_pcs(SPI_CHIP_SEL_CS3)

/* Clock polarity. */
#define SPI_CLK_POLARITY 1   //CPOL  CLK空闲状态为高电平

/* Clock phase. */
#define SPI_CLK_PHASE 1      //NCPHA  上升沿捕捉数据

/* Delay before SPCK. */
//#define SPI_DLYBS 0x40
#define SPI_DLYBS 0x80

/* Delay between consecutive transfers. */
//#define SPI_DLYBCT 0x10
#define SPI_DLYBCT 0x00

/* Number of SPI clock configurations. */
#define NUM_SPCK_CONFIGURATIONS 4

/** spi mode definition*/
#define	MASTER_MODE   0
#define SLAVE_MODE	  1

#define SPI_CLK			PIO_PD22_IDX
#define SPI_DAT			PIO_PD21_IDX
#define MODEL_SELECT	PIO_PD25_IDX
#define PIN_SELECT		PIO_PD27_IDX


#define DIS_PIN_SELECT 		arch_ioport_set_pin_level(PIN_SELECT,0)
#define EN_PIN_SELECT 		arch_ioport_set_pin_level(PIN_SELECT,1)
#define DIS_MODEL_SELECT	arch_ioport_set_pin_level(MODEL_SELECT,0)
#define EN_MODEL_SELECT	    arch_ioport_set_pin_level(MODEL_SELECT,1)
#define SPI_CLK_LOW			arch_ioport_set_pin_level(SPI_CLK,0)
#define SPI_CLK_HIGH		arch_ioport_set_pin_level(SPI_CLK,1)
#define SPI_DAT_LOW			arch_ioport_set_pin_level(SPI_DAT,0)
#define SPI_DAT_HIGH		arch_ioport_set_pin_level(SPI_DAT,1)

extern void hal_spi_master_init(void);
extern void hal_spi_master_init3(void);
extern void hal_soft_spi_master_init(void);
extern void tx_c_channel(FPGA_C_REG PDU_FPGA_C_REG);
extern void tx_PINSEL_channel(uint8_t JA_LINE0_Pin_Num,uint8_t JB_LINE1_Pin_Num);
extern void tx_spi_one_byte(uint8_t dat,uint8_t num);
extern void tx_PINSEL_channel1(void);

//extern void tx_c_channel1(void);

#endif /* HAL_SPI_H_ */