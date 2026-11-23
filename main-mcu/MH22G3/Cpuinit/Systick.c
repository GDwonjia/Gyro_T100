#include "Systick.h"


volatile uint8_t FLAG1MS = 0;
volatile uint8_t FLAG5MS = 0;
volatile uint8_t FLAG10MS = 0;
volatile uint8_t FLAG20MS = 0;
volatile uint8_t FLAG50MS = 0;
volatile uint8_t FLAG100MS = 0;
volatile uint8_t FLAG500MS = 0;
volatile uint8_t FLAG1000MS = 0;

volatile uint32_t g_SysTickMs = 0;

void SysTick_Init(void)
{
    RCC_ClocksTypeDef clock;

    RCC_GetClocksFreq(&clock);

    /*
     * Use HCLK as SysTick clock source.
     * 1ms period: LOAD = HCLK / 1000 - 1.
     */
    SysTick->CTRL = 0;
    SysTick->LOAD = (clock.HCLK_Frequency / 1000U) - 1U;
    SysTick->VAL = 0;

    NVIC_SetPriority(SysTick_IRQn, 0x0FU);

    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk   |
                    SysTick_CTRL_ENABLE_Msk;
}


void SysTick_Handler(void)
{
    static uint16_t cnt_5ms = 0;
    static uint16_t cnt_10ms = 0;
    static uint16_t cnt_20ms = 0;
    static uint16_t cnt_50ms = 0;
    static uint16_t cnt_100ms = 0;
    static uint16_t cnt_500ms = 0;
    static uint16_t cnt_1000ms = 0;

    g_SysTickMs++;

    FLAG1MS = 1;

    if (++cnt_5ms >= 5U)
    {
        cnt_5ms = 0;
        FLAG5MS = 1;
    }

    if (++cnt_10ms >= 10U)
    {
        cnt_10ms = 0;
        FLAG10MS = 1;
    }

    if (++cnt_20ms >= 20U)
    {
        cnt_20ms = 0;
        FLAG20MS = 1;
    }

    if (++cnt_50ms >= 50U)
    {
        cnt_50ms = 0;
        FLAG50MS = 1;
    }

    if (++cnt_100ms >= 100U)
    {
        cnt_100ms = 0;
        FLAG100MS = 1;
    }

    if (++cnt_500ms >= 500U)
    {
        cnt_500ms = 0;
        FLAG500MS = 1;
    }

    if (++cnt_1000ms >= 1000U)
    {
        cnt_1000ms = 0;
        FLAG1000MS = 1;
    }
}
