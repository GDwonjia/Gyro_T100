#ifndef __RCC_H
#define __RCC_H

#include "mh22xx.h"
#include <stdint.h>

typedef enum
{
    RCC_INIT_OK = 0U,
    RCC_INIT_HSE_TIMEOUT,
    RCC_INIT_PLL_TIMEOUT,
    RCC_INIT_SYSCLK_TIMEOUT
} RCC_InitStatus_t;

RCC_InitStatus_t RCC_ClkConfiguration(void);
uint32_t SystemClock_GetSYSCLK(void);

#endif
