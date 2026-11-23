#include "timer.h"

volatile uint32_t g_SystemTick = 0U;
volatile WheelSpeed_t wheel_speed_l = {0};
volatile WheelSpeed_t wheel_speed_r = {0};

void Timer5_Init(void)
{
    TIM_TimeBaseInitTypeDef time_base;
    NVIC_InitTypeDef nvic;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

    TIM_TimeBaseStructInit(&time_base);
    time_base.TIM_Prescaler = SYSTEM_TIMER_PRESCALER - 1U;
    time_base.TIM_Period = SYSTEM_TIMER_PERIOD - 1U;
    time_base.TIM_ClockDivision = TIM_CKD_DIV1;
    time_base.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(SYSTEM_TIMER, &time_base);

    TIM_SetCounter(SYSTEM_TIMER, 0U);
    TIM_ClearITPendingBit(SYSTEM_TIMER, TIM_IT_Update);
    TIM_ITConfig(SYSTEM_TIMER, TIM_IT_Update, ENABLE);

    nvic.NVIC_IRQChannel = TIM5_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 3U;
    nvic.NVIC_IRQChannelSubPriority = 0U;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    TIM_Cmd(SYSTEM_TIMER, ENABLE);
}

void TIM5_IRQHandler(void)
{
    if (TIM_GetITStatus(SYSTEM_TIMER, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(SYSTEM_TIMER, TIM_IT_Update);
        g_SystemTick++;
    }
}

uint32_t Get_System_Tick(void)
{
    return g_SystemTick;
}

static void WheelSpeed_CaptureProcess(volatile WheelSpeed_t *wheel,
                                      uint16_t capture_value)
{
    wheel->pulse_count++;
    wheel->last_capture_tick = Get_System_Tick();

    if (wheel->capture_initialized == 0U)
    {
        wheel->capture_last = capture_value;
        wheel->capture_period = 0U;
        wheel->capture_initialized = 1U;
    }
    else
    {
        /* Unsigned subtraction handles one 16-bit counter wrap. */
        wheel->capture_period = (uint16_t)(capture_value - wheel->capture_last);
        wheel->capture_last = capture_value;
    }

    wheel->speed_timeout = 0U;
    wheel->update_flag = 1U;
}

void Timer3_Init(void)
{
    GPIO_InitTypeDef gpio;
    TIM_TimeBaseInitTypeDef time_base;
    TIM_ICInitTypeDef input_capture;
    NVIC_InitTypeDef nvic;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

    /* PB4=TIM3_CH1, PB5=TIM3_CH2; disable JTAG but retain SWD for J-Link. */
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);

    gpio.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &gpio);

    TIM_TimeBaseStructInit(&time_base);
    /* Current RCC plan: PCLK1=108 MHz, APB1 timer clock=216 MHz. */
    time_base.TIM_Prescaler =
        (uint16_t)(TIM3_APB1_TIMER_CLOCK_HZ / WHEEL_SPEED_TIMER_FREQ - 1U);
    time_base.TIM_Period = WHEEL_SPEED_TIMER_PERIOD;
    time_base.TIM_ClockDivision = TIM_CKD_DIV1;
    time_base.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(WHEEL_SPEED_TIMER, &time_base);

    TIM_ICStructInit(&input_capture);
    input_capture.TIM_ICPolarity = TIM_ICPolarity_Rising;
    input_capture.TIM_ICSelection = TIM_ICSelection_DirectTI;
    input_capture.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    input_capture.TIM_ICFilter = WHEEL_SPEED_IC_FILTER;

    input_capture.TIM_Channel = WHEEL_SPEED_R_CHANNEL;
    TIM_ICInit(WHEEL_SPEED_TIMER, &input_capture);
    input_capture.TIM_Channel = WHEEL_SPEED_L_CHANNEL;
    TIM_ICInit(WHEEL_SPEED_TIMER, &input_capture);

    TIM_SetCounter(WHEEL_SPEED_TIMER, 0U);
    TIM_ClearITPendingBit(WHEEL_SPEED_TIMER, TIM_IT_CC1 | TIM_IT_CC2);
    TIM_ITConfig(WHEEL_SPEED_TIMER, TIM_IT_CC1 | TIM_IT_CC2, ENABLE);

    nvic.NVIC_IRQChannel = TIM3_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 3U;
    nvic.NVIC_IRQChannelSubPriority = 1U;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    TIM_Cmd(WHEEL_SPEED_TIMER, ENABLE);
}

void TIM3_IRQHandler(void)
{
    uint16_t capture_value;

    if (TIM_GetITStatus(WHEEL_SPEED_TIMER, TIM_IT_CC1) != RESET)
    {
        capture_value = TIM_GetCapture1(WHEEL_SPEED_TIMER);
        if (TIM_GetFlagStatus(WHEEL_SPEED_TIMER, TIM_FLAG_CC1OF) != RESET)
        {
            TIM_ClearFlag(WHEEL_SPEED_TIMER, TIM_FLAG_CC1OF);
            wheel_speed_r.overcapture_count++;
            wheel_speed_r.capture_last = capture_value;
            wheel_speed_r.capture_period = 0U;
            wheel_speed_r.capture_initialized = 1U;
            wheel_speed_r.speed_timeout = 0U;
            wheel_speed_r.last_capture_tick = Get_System_Tick();
            wheel_speed_r.update_flag = 1U;
        }
        else
        {
            WheelSpeed_CaptureProcess(&wheel_speed_r, capture_value);
        }
        TIM_ClearITPendingBit(WHEEL_SPEED_TIMER, TIM_IT_CC1);
    }

    if (TIM_GetITStatus(WHEEL_SPEED_TIMER, TIM_IT_CC2) != RESET)
    {
        capture_value = TIM_GetCapture2(WHEEL_SPEED_TIMER);
        if (TIM_GetFlagStatus(WHEEL_SPEED_TIMER, TIM_FLAG_CC2OF) != RESET)
        {
            TIM_ClearFlag(WHEEL_SPEED_TIMER, TIM_FLAG_CC2OF);
            wheel_speed_l.overcapture_count++;
            wheel_speed_l.capture_last = capture_value;
            wheel_speed_l.capture_period = 0U;
            wheel_speed_l.capture_initialized = 1U;
            wheel_speed_l.speed_timeout = 0U;
            wheel_speed_l.last_capture_tick = Get_System_Tick();
            wheel_speed_l.update_flag = 1U;
        }
        else
        {
            WheelSpeed_CaptureProcess(&wheel_speed_l, capture_value);
        }
        TIM_ClearITPendingBit(WHEEL_SPEED_TIMER, TIM_IT_CC2);
    }
}

void WheelSpeed_TimeoutCheck(void)
{
    uint32_t current_tick = Get_System_Tick();

    if ((wheel_speed_l.capture_initialized != 0U) &&
        ((current_tick - wheel_speed_l.last_capture_tick) >= WHEEL_SPEED_TIMEOUT_MS))
    {
        wheel_speed_l.capture_period = 0U;
        wheel_speed_l.capture_initialized = 0U;
        wheel_speed_l.speed_timeout = 1U;
        wheel_speed_l.update_flag = 1U;
    }

    if ((wheel_speed_r.capture_initialized != 0U) &&
        ((current_tick - wheel_speed_r.last_capture_tick) >= WHEEL_SPEED_TIMEOUT_MS))
    {
        wheel_speed_r.capture_period = 0U;
        wheel_speed_r.capture_initialized = 0U;
        wheel_speed_r.speed_timeout = 1U;
        wheel_speed_r.update_flag = 1U;
    }
}

void WheelSpeed_GetData(WheelSpeedData_t *left, WheelSpeedData_t *right)
{
    uint32_t primask;

    if ((left == 0) || (right == 0))
    {
        return;
    }

    primask = __get_PRIMASK();
    __disable_irq();

    left->capture_period = wheel_speed_l.capture_period;
    left->pulse_count = wheel_speed_l.pulse_count;
    left->overcapture_count = wheel_speed_l.overcapture_count;
    left->update_flag = wheel_speed_l.update_flag;
    left->speed_timeout = wheel_speed_l.speed_timeout;

    right->capture_period = wheel_speed_r.capture_period;
    right->pulse_count = wheel_speed_r.pulse_count;
    right->overcapture_count = wheel_speed_r.overcapture_count;
    right->update_flag = wheel_speed_r.update_flag;
    right->speed_timeout = wheel_speed_r.speed_timeout;

    wheel_speed_l.update_flag = 0U;
    wheel_speed_r.update_flag = 0U;
    __set_PRIMASK(primask);
}

void Timer_Init(void)
{
    Timer5_Init();
    Timer3_Init();
}
