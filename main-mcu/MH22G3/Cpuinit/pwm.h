#ifndef __PWM_H
#define __PWM_H

#include "mh22xx.h"
#include <stdint.h>

/* Timer input clocks are 216 MHz under the current RCC configuration. */
#define CLIFF_RELOAD                900U
#define CLIFF_PRESCALER             18U
#define MOTOR_RELOAD                900U
#define MOTOR_PRESCALER             18U
#define WHEEL_RELOAD                1440U
#define WHEEL_PRESCALER             12U

#define CLIFF_PWM_MAX               (CLIFF_RELOAD - 1U)
#define MOTOR_PWM_MAX               (MOTOR_RELOAD - 1U)
#define WHEEL_PWM_MAX               (WHEEL_RELOAD - 1U)

/* TIM8: wheel H-bridge PWM pairs. */
#define LWPWMP                      (TIM8->CCR1)  /* PC6, left forward */
#define LWPWMN                      (TIM8->CCR2)  /* PC7, left reverse */
#define RWPWMP                      (TIM8->CCR3)  /* PC8, right forward */
#define RWPWMN                      (TIM8->CCR4)  /* PC9, right reverse */

/* TIM4: actuator PWM. */
#define SBPWM(x)                    (TIM4->CCR1 = (uint16_t)(x))
#define FANPWM(x)                   (TIM4->CCR2 = (uint16_t)(x))
#define MBPWM(x)                    (TIM4->CCR3 = (uint16_t)(x))
#define CHARGEPWM(x)                (TIM4->CCR4 = (uint16_t)(x))

/* TIM2: cliff and omnidirectional sensor modulation. */
#define CLIFFPWM(x)                 (TIM2->CCR1 = (uint16_t)(x))
#define TOUCHPWM(x)                 (TIM2->CCR2 = (uint16_t)(x))

void PWM_Init(void);
void PWM_SetAllZero(void);

#endif
