#ifndef __SYSTICK_H
#define __SYSTICK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "board_support.h"
#include <stdint.h>

extern volatile uint8_t FLAG1MS;
extern volatile uint8_t FLAG5MS;
extern volatile uint8_t FLAG10MS;
extern volatile uint8_t FLAG20MS;
extern volatile uint8_t FLAG50MS;
extern volatile uint8_t FLAG100MS;
extern volatile uint8_t FLAG500MS;
extern volatile uint8_t FLAG1000MS;
extern volatile uint32_t g_SysTickMs;

void SysTick_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __SYSTICK_H */
