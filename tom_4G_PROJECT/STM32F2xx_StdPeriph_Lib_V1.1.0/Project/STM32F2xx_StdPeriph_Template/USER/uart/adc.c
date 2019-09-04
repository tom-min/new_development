
#include "adc.h"
#include "user_stm32f2xx.h"
#include "interface.h"
#include "user_config.h"
void ADC1_CH7_DMA_Config(void);
void GPIO_Config(void);
#define ADC_CCR_ADDRESS    ((uint32_t)0x40012308)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


__IO uint16_t ADC1ConvertedValue[Sample_Num][Channel_Num]={ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}};

//>3.2v no battery

int ADC_INIT(void)
{
	ADC1_CH7_DMA_Config();
	ADC_SoftwareStartConv(ADC1);
}

void GPIO_Config(void)
	{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1|GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


/**
* @brief  ADC1 channel07 with DMA configuration
* @param  None
* @retval None
*/
void ADC1_CH7_DMA_Config(void)
{
ADC_InitTypeDef       ADC_InitStructure;
ADC_CommonInitTypeDef ADC_CommonInitStructure;
DMA_InitTypeDef       DMA_InitStructure;
GPIO_InitTypeDef      GPIO_InitStructure;

/* Enable ADC1, DMA2 and GPIO clocks ****************************************/

RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

/* Configure ADC1 Channel7 pin as analog input ******************************/
GPIO_Config();

/* DMA2 Stream0 channel2 configuration **************************************/
DMA_InitStructure.DMA_Channel = DMA_Channel_0;  
DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ADC1ConvertedValue;  
DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
DMA_InitStructure.DMA_BufferSize = (Channel_Num*Sample_Num);         
DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
DMA_InitStructure.DMA_Priority = DMA_Priority_High;
DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
#ifdef APP_APPLACATION
DMA_DeInit(DMA2_Stream0);
#endif	
DMA_Init(DMA2_Stream0, &DMA_InitStructure);
DMA_Cmd(DMA2_Stream0, ENABLE);

/* ADC Common Init **********************************************************/
ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8;
ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
ADC_CommonInit(&ADC_CommonInitStructure);

/* ADC1 Init ****************************************************************/
ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
ADC_InitStructure.ADC_ScanConvMode = ENABLE;
ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
ADC_InitStructure.ADC_NbrOfConversion = Channel_Num;
#ifdef APP_APPLACATION
ADC_DeInit();
#endif

ADC_Init(ADC1, &ADC_InitStructure);

/* ADC1 regular channel7 configuration *************************************/
ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_56Cycles);//
ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_56Cycles);
ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_56Cycles);

ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 4, ADC_SampleTime_56Cycles);
ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 5, ADC_SampleTime_56Cycles);
ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 6, ADC_SampleTime_56Cycles);
ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 7, ADC_SampleTime_56Cycles);


ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

/* Enable ADC1 DMA */
ADC_DMACmd(ADC1, ENABLE);

/* Enable ADC1 */
ADC_Cmd(ADC1, ENABLE);
}


uint32_t ADC_GET(ADC_CHANNEL chnneal)
{
uint32_t value=0;
uint32_t i;
uint32_t temp;
for(i=0;i<Sample_Num;i++)
{
temp= ADC1ConvertedValue[i][chnneal];
//printf("v=%04d %04d \r\n",temp,chnneal);
value += (temp&0xFFF);
}
//printf(" \r\n" );
value /= Sample_Num;
///printf("value==%d chnneal=%d \r\n",value,chnneal);

value= (3300*11*value)/4096;	

//printf("value==%d chnneal=%d \r\n",value,chnneal);
return  value;
}

uint16_t GetAdc(void)
{
uint32_t vol[Channel_Num]= {0,0,0,0,0,0,0};
uint16_t i;
uint16_t times=Sample_Num;

for(i=0; i<times; i++)
{

vol[MUX0_ADC_ADC_CHANNEL] += ADC_GET(MUX0_ADC_ADC_CHANNEL);
vol[MUX1_ADC_ADC_CHANNEL] += ADC_GET(MUX1_ADC_ADC_CHANNEL);
vol[BUS_CMP_ADC_CHANNEL] += ADC_GET(BUS_CMP_ADC_CHANNEL);
vol[BUS_DRV_ADC_CHANNEL] += ADC_GET(BUS_DRV_ADC_CHANNEL);
vol[PC1_CAN_H_ADC] += ADC_GET(PC1_CAN_H_ADC);
vol[PC2_CAN_L_ADC] += ADC_GET(PC2_CAN_L_ADC);
vol[PC3_VCCIN_ADC] += ADC_GET(PC3_VCCIN_ADC);
}

for(i=0; i<sizeof(vol)/sizeof(uint32_t); i++)
{
vol[i]/=times;
}

printf("mux0 vol==%d mv\r\n",vol[MUX0_ADC_ADC_CHANNEL]);
printf("mux1 vol==%d mv\r\n",vol[MUX1_ADC_ADC_CHANNEL]);
printf("bus cmp vol==%d mv\r\n",vol[BUS_CMP_ADC_CHANNEL]);
printf("BUS_DRV vol==%d mv\r\n",vol[BUS_DRV_ADC_CHANNEL]);
printf("CAN_H vol==%d mv\r\n",vol[PC1_CAN_H_ADC]);
printf("CAN_L vol==%d mv\r\n",vol[PC2_CAN_L_ADC]);
printf("VCCIN vol==%d mv\r\n",vol[PC3_VCCIN_ADC]);
}


