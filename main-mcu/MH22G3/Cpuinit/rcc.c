#include "rcc.h"

#define RCC_HSE_STARTUP_TIMEOUT       0x5000U
#define RCC_PLL_STARTUP_TIMEOUT       0x5000U
#define RCC_SYSCLK_STARTUP_TIMEOUT    0x5000U
#define RCC_LSI_STARTUP_TIMEOUT       0x5000U
#define RCC_HSI_STARTUP_TIMEOUT       0x5000U

/*
 * Clock plan:
 * HSE   = 8 MHz
 * PLL   = HSE * 27 = 216 MHz
 * SYSCLK/HCLK = 216 MHz
 * PCLK1 = 108 MHz (APB1 prescaler = 2)
 * PCLK2 = 216 MHz (APB2 prescaler = 1)
 * APB1 timer clock = 216 MHz
 */
RCC_InitStatus_t RCC_ClkConfiguration(void)
{
    uint32_t timeout;

    RCC_DeInit();

    /* Start external high-speed clock. */
    RCC_HSEConfig(RCC_HSE_ON);
    timeout = RCC_HSE_STARTUP_TIMEOUT;
    while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
    {
        if (timeout-- == 0U)
        {
            return RCC_INIT_HSE_TIMEOUT;
        }
    }

    /* Configure PLL and bus clocks. */
    RCC_PLLCmd(DISABLE);
    FLASH_SetLatency(FLASH_Latency_2);
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_27);
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    RCC_PCLK1Config(RCC_HCLK_Div2);
    RCC_PCLK2Config(RCC_HCLK_Div1);

    RCC_PLLCmd(ENABLE);
    timeout = RCC_PLL_STARTUP_TIMEOUT;
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
        if (timeout-- == 0U)
        {
            return RCC_INIT_PLL_TIMEOUT;
        }
    }

    /* Switch SYSCLK to PLL and verify the switch status. */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    timeout = RCC_SYSCLK_STARTUP_TIMEOUT;
    while (RCC_GetSYSCLKSource() != 0x08U)
    {
        if (timeout-- == 0U)
        {
            return RCC_INIT_SYSCLK_TIMEOUT;
        }
    }

    /* Keep internal clocks available for peripherals that require them. */
    RCC_LSICmd(ENABLE);
    timeout = RCC_LSI_STARTUP_TIMEOUT;
    while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
    {
        if (timeout-- == 0U)
        {
            break;
        }
    }

    RCC_HSICmd(ENABLE);
    timeout = RCC_HSI_STARTUP_TIMEOUT;
    while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)
    {
        if (timeout-- == 0U)
        {
            break;
        }
    }

    return RCC_INIT_OK;
}

uint32_t SystemClock_GetSYSCLK(void)
{
    RCC_ClocksTypeDef clocks;
    RCC_GetClocksFreq(&clocks);
    return clocks.SYSCLK_Frequency;
}
