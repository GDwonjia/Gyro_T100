#include "adc.h"

volatile uint16_t adc_dma_buffer[ADC_CHANNEL_NUM] = {0U};
volatile uint16_t adc_value[ADC_CHANNEL_NUM] = {0U};
volatile uint8_t adc_data_ready = 0U;

/* Scan order follows the current MH22G3 board pin allocation. */
static const uint8_t adc_channel_list[ADC_CHANNEL_NUM] =
{
    ADC_Channel_0,   /* PA0: left wheel current */
    ADC_Channel_1,   /* PA1: right wheel current */
    ADC_Channel_4,   /* PA4: border */
    ADC_Channel_5,   /* PA5: touch left */
    ADC_Channel_6,   /* PA6: touch front */
    ADC_Channel_7,   /* PA7: touch right */
    ADC_Channel_8,   /* PB0: left cliff */
    ADC_Channel_9,   /* PB1: left side-brush current */
    ADC_Channel_10,  /* PC0: right side-brush current */
    ADC_Channel_11,  /* PC1: battery voltage */
    ADC_Channel_12,  /* PC2: charge current */
    ADC_Channel_13,  /* PC3: main-brush current */
    ADC_Channel_14,  /* PC4: right cliff */
    ADC_Channel_15   /* PC5: front cliff */
};

static void ADC_GPIO_Init(void)
{
    GPIO_InitTypeDef gpio;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                           RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_GPIOC, ENABLE);

    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Mode = GPIO_Mode_AIN;
    gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |
                    GPIO_Pin_4 | GPIO_Pin_5 |
                    GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOA, &gpio);
    gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init(GPIOB, &gpio);
    gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |
                    GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_Init(GPIOC, &gpio);
}

static void ADC_DMA_Init(void)
{
    DMA_InitTypeDef dma;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    DMA_DeInit(ADC_DMA_CHANNEL);

    dma.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
    dma.DMA_MemoryBaseAddr = (uint32_t)adc_dma_buffer;
    dma.DMA_DIR = DMA_DIR_PeripheralSRC;
    dma.DMA_BufferSize = ADC_CHANNEL_NUM;
    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    dma.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    dma.DMA_Mode = DMA_Mode_Circular;
    dma.DMA_Priority = DMA_Priority_High;
    dma.DMA_M2M = DMA_M2M_Disable;

    DMA_Init(ADC_DMA_CHANNEL, &dma);
    DMA_Cmd(ADC_DMA_CHANNEL, ENABLE);
}

void ADCInit(void)
{
    ADC_InitTypeDef adc;
    uint8_t i;
    volatile uint32_t timeout;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    /* PCLK2=216 MHz; ADC clock = 216/16 = 13.5 MHz. */
    RCC_ADCCLKConfig(RCC_PCLK2_Div16);

    ADC_GPIO_Init();
    ADC_DeInit(ADC1);

    adc.ADC_Mode = ADC_Mode_Independent;
    adc.ADC_ScanConvMode = ENABLE;
    adc.ADC_ContinuousConvMode = ENABLE;
    adc.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    adc.ADC_DataAlign = ADC_DataAlign_Right;
    adc.ADC_NbrOfChannel = ADC_CHANNEL_NUM;
    ADC_Init(ADC1, &adc);

    for (i = 0U; i < ADC_CHANNEL_NUM; i++)
    {
        ADC_RegularChannelConfig(ADC1,
                                 adc_channel_list[i],
                                 (uint8_t)(i + 1U),
                                 ADC_SampleTime_239Cycles5);
    }

    ADC_DMA_Init();
    ADC_Cmd(ADC1, ENABLE);

    /* Calibration waits are bounded to avoid a permanent startup hang. */
    ADC_ResetCalibration(ADC1);
    timeout = 0x10000UL;
    while (ADC_GetResetCalibrationStatus(ADC1) != RESET)
    {
        if (timeout-- == 0U)
        {
            ADC_Stop();
            return;
        }
    }

    ADC_StartCalibration(ADC1);
    timeout = 0x10000UL;
    while (ADC_GetCalibrationStatus(ADC1) != RESET)
    {
        if (timeout-- == 0U)
        {
            ADC_Stop();
            return;
        }
    }

    ADC_DMACmd(ADC1, ENABLE);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void ADC_Task(void)
{
    uint8_t i;

    /* Copy the latest DMA scan into the application-facing array. */
    for (i = 0U; i < ADC_CHANNEL_NUM; i++)
    {
        adc_value[i] = adc_dma_buffer[i];
    }
    adc_data_ready = 1U;
}

uint16_t ADC_GetValue(ADC_Index_t index)
{
    if ((uint8_t)index >= ADC_CHANNEL_NUM)
    {
        return 0U;
    }
    return adc_value[(uint8_t)index];
}

void ADC_Start(void)
{
    DMA_Cmd(ADC_DMA_CHANNEL, ENABLE);
    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void ADC_Stop(void)
{
    ADC_SoftwareStartConvCmd(ADC1, DISABLE);
    ADC_DMACmd(ADC1, DISABLE);
    DMA_Cmd(ADC_DMA_CHANNEL, DISABLE);
    ADC_Cmd(ADC1, DISABLE);
}
