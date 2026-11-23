#ifndef __TIMER_H
#define __TIMER_H

#include <stdint.h>
#include "mh22xx.h"

/* TIM5: 1 ms system time base, assuming TIM5 clock = 216 MHz. */
#define SYSTEM_TIMER                  TIM5
#define SYSTEM_TIMER_PRESCALER        216U
#define SYSTEM_TIMER_PERIOD           1000U

/* TIM3 input capture: PB4/TIM3_CH1 right wheel; PB5/TIM3_CH2 left wheel. */
#define WHEEL_SPEED_TIMER             TIM3
#define WHEEL_SPEED_R_CHANNEL         TIM_Channel_1
#define WHEEL_SPEED_L_CHANNEL         TIM_Channel_2
#define WHEEL_SPEED_TIMER_FREQ        10000U
#define WHEEL_SPEED_TIMER_PERIOD      65535U
#define WHEEL_SPEED_TIMEOUT_MS        100U
#define WHEEL_SPEED_IC_FILTER         4U

#define TIM3_APB1_TIMER_CLOCK_HZ      216000000UL

typedef struct
{
    uint16_t capture_last;
    uint16_t capture_period;       /* 100 us per count at 10 kHz. */
    uint32_t pulse_count;
    uint32_t last_capture_tick;
    uint32_t overcapture_count;
    uint8_t capture_initialized;
    uint8_t update_flag;
    uint8_t speed_timeout;
} WheelSpeed_t;

typedef struct
{
    uint16_t capture_period;
    uint32_t pulse_count;
    uint32_t overcapture_count;
    uint8_t update_flag;
    uint8_t speed_timeout;
} WheelSpeedData_t;

extern volatile WheelSpeed_t wheel_speed_l;
extern volatile WheelSpeed_t wheel_speed_r;
extern volatile uint32_t g_SystemTick;

void Timer_Init(void);
void Timer5_Init(void);
void Timer3_Init(void);
uint32_t Get_System_Tick(void);
void WheelSpeed_TimeoutCheck(void);
void WheelSpeed_GetData(WheelSpeedData_t *left, WheelSpeedData_t *right);

#endif
