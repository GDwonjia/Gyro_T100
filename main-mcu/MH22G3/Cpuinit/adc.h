#ifndef __ADC_H
#define __ADC_H

#include "mh22xx.h"
#include <stdint.h>

#define ADC_CHANNEL_NUM             14U
#define ADC_DMA_CHANNEL             DMA1_Channel1

typedef enum
{
    ADC_IDX_WHEEL_L_CUR = 0,
    ADC_IDX_WHEEL_R_CUR,
    ADC_IDX_BORDER,
    ADC_IDX_TOUCH_L,
    ADC_IDX_TOUCH_F,
    ADC_IDX_TOUCH_R,
    ADC_IDX_CLIFF_L,
    ADC_IDX_BRUSH_L_CUR,
    ADC_IDX_BRUSH_R_CUR,
    ADC_IDX_BAT_VOL,
    ADC_IDX_CHARGE_CUR,
    ADC_IDX_MAIN_CUR,
    ADC_IDX_CLIFF_R,
    ADC_IDX_CLIFF_F
} ADC_Index_t;

extern volatile uint16_t adc_dma_buffer[ADC_CHANNEL_NUM];
extern volatile uint16_t adc_value[ADC_CHANNEL_NUM];
extern volatile uint8_t adc_data_ready;
void ADCInit(void);
void ADC_Task(void);
uint16_t ADC_GetValue(ADC_Index_t index);
void ADC_Start(void);
void ADC_Stop(void);

#endif
