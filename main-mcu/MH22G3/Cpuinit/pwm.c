#include "pwm.h"

static void PWM_GPIO_Init(void)
{
    GPIO_InitTypeDef gpio;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                           RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_GPIOC |
                           RCC_APB2Periph_AFIO, ENABLE);

    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);

    /* TIM2 partial remap 1: PA15=CH1, PB3=CH2. */
    gpio.GPIO_Pin = GPIO_Pin_15;
    GPIO_Init(GPIOA, &gpio);
    gpio.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOB, &gpio);

    /* TIM4: PB6=CH1, PB7=CH2, PB8=CH3, PB9=CH4. */
    gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_Init(GPIOB, &gpio);

    /* TIM8: PC6=CH1, PC7=CH2, PC8=CH3, PC9=CH4. */
    gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_Init(GPIOC, &gpio);
}

static void PWM_TimerBase_Init(TIM_TypeDef *timer,
                               uint16_t prescaler_div,
                               uint16_t reload)
{
    TIM_TimeBaseInitTypeDef time_base;

    TIM_TimeBaseStructInit(&time_base);
    time_base.TIM_Prescaler = prescaler_div - 1U;
    time_base.TIM_Period = reload - 1U;
    time_base.TIM_ClockDivision = TIM_CKD_DIV1;
    time_base.TIM_CounterMode = TIM_CounterMode_Up;
    time_base.TIM_RepetitionCounter = 0U;

    TIM_TimeBaseInit(timer, &time_base);
    TIM_ARRPreloadConfig(timer, ENABLE);
}

static void PWM_Channel_Init(TIM_TypeDef *timer, uint16_t channel)
{
    TIM_OCInitTypeDef oc;

    TIM_OCStructInit(&oc);
    oc.TIM_OCMode = TIM_OCMode_PWM1;
    oc.TIM_OutputState = TIM_OutputState_Enable;
    oc.TIM_Pulse = 0U;
    oc.TIM_OCPolarity = TIM_OCPolarity_High;

    switch (channel)
    {
        case TIM_Channel_1:
            TIM_OC1Init(timer, &oc);
            TIM_OC1PreloadConfig(timer, TIM_OCPreload_Enable);
            break;
        case TIM_Channel_2:
            TIM_OC2Init(timer, &oc);
            TIM_OC2PreloadConfig(timer, TIM_OCPreload_Enable);
            break;
        case TIM_Channel_3:
            TIM_OC3Init(timer, &oc);
            TIM_OC3PreloadConfig(timer, TIM_OCPreload_Enable);
            break;
        case TIM_Channel_4:
            TIM_OC4Init(timer, &oc);
            TIM_OC4PreloadConfig(timer, TIM_OCPreload_Enable);
            break;
        default:
            break;
    }
}

void PWM_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

    PWM_GPIO_Init();

    /* TIM2: cliff / touch, 216 MHz / 18 / 900 = 13.333 kHz. */
    PWM_TimerBase_Init(TIM2, CLIFF_PRESCALER, CLIFF_RELOAD);
    PWM_Channel_Init(TIM2, TIM_Channel_1);
    PWM_Channel_Init(TIM2, TIM_Channel_2);

    /* TIM4: fan / main brush / side brush / charge, 13.333 kHz. */
    PWM_TimerBase_Init(TIM4, MOTOR_PRESCALER, MOTOR_RELOAD);
    PWM_Channel_Init(TIM4, TIM_Channel_1);
    PWM_Channel_Init(TIM4, TIM_Channel_2);
    PWM_Channel_Init(TIM4, TIM_Channel_3);
    PWM_Channel_Init(TIM4, TIM_Channel_4);

    /* TIM8: four wheel outputs, 216 MHz / 12 / 1440 = 12.5 kHz. */
    PWM_TimerBase_Init(TIM8, WHEEL_PRESCALER, WHEEL_RELOAD);
    PWM_Channel_Init(TIM8, TIM_Channel_1);
    PWM_Channel_Init(TIM8, TIM_Channel_2);
    PWM_Channel_Init(TIM8, TIM_Channel_3);
    PWM_Channel_Init(TIM8, TIM_Channel_4);

    PWM_SetAllZero();

    /* TIM8 is an advanced-control timer; enable its main output gate. */
    TIM_CtrlPWMOutputs(TIM8, ENABLE);

    TIM_Cmd(TIM2, ENABLE);
    TIM_Cmd(TIM4, ENABLE);
    TIM_Cmd(TIM8, ENABLE);
}

void PWM_SetAllZero(void)
{
    LWPWMP = 0;
    LWPWMN = 0;
    RWPWMP = 0;
    RWPWMN = 0;

    FANPWM(0);
    MBPWM(0);
    SBPWM(0);
    CHARGEPWM(0);
    CLIFFPWM(0);
    TOUCHPWM(0);
}
