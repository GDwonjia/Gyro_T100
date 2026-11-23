#include "gpio.h"
#include "misc.h"


__IO uint16_t g_LSpeedCnt = 0;
__IO uint16_t g_RSpeedCnt = 0;
__IO uint16_t g_FanSpeedCnt = 0;

volatile uint32_t WakeupFlag = 0;

static void GPIO_Input_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = MCU_COLLISION_R_PIN;
    GPIO_Init(MCU_COLLISION_R_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = MCU_COLLISION_L_PIN;
    GPIO_Init(MCU_COLLISION_L_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = MCU_KEY_POWER_PIN;
    GPIO_Init(MCU_KEY_POWER_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = MCU_HALL_DUST_PIN;
    GPIO_Init(MCU_HALL_DUST_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = MCU_HALL_WATER_PIN;
    GPIO_Init(MCU_HALL_WATER_PORT, &GPIO_InitStructure);
}


static void GPIO_Output_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = MCU_SWMCU3V3_PIN;
    GPIO_Init(MCU_SWMCU3V3_PORT, &GPIO_InitStructure);
}

static void GPIO_EXTI_PinInit(GPIO_TypeDef *GPIOx,
                              uint16_t GPIO_Pin,
                              uint8_t GPIO_PortSource,
                              uint8_t GPIO_PinSource,
                              uint32_t EXTI_Line,
                              EXTITrigger_TypeDef trigger)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOx, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSource, GPIO_PinSource);

    EXTI_InitStructure.EXTI_Line = EXTI_Line;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = trigger;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    EXTI_ClearITPendingBit(EXTI_Line);
}


static void GPIO_EXTI_NVIC_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_Init(&NVIC_InitStructure);
}


static void GPIO_EXTI_Init(void)
{
   
    GPIO_EXTI_PinInit(GPIOA, GPIO_Pin_2,
                      GPIO_PortSourceGPIOA, GPIO_PinSource2,
                      EXTI_Line2, EXTI_Trigger_Rising_Falling);

    GPIO_EXTI_PinInit(GPIOA, GPIO_Pin_3,
                      GPIO_PortSourceGPIOA, GPIO_PinSource3,
                      EXTI_Line3, EXTI_Trigger_Rising_Falling);

    GPIO_EXTI_PinInit(GPIOA, GPIO_Pin_8,
                      GPIO_PortSourceGPIOA, GPIO_PinSource8,
                      EXTI_Line8, EXTI_Trigger_Rising_Falling);

    GPIO_EXTI_PinInit(GPIOA, GPIO_Pin_11,
                      GPIO_PortSourceGPIOA, GPIO_PinSource11,
                      EXTI_Line11, EXTI_Trigger_Rising_Falling);

    GPIO_EXTI_PinInit(GPIOA, GPIO_Pin_12,
                      GPIO_PortSourceGPIOA, GPIO_PinSource12,
                      EXTI_Line12, EXTI_Trigger_Rising);
	
    GPIO_EXTI_PinInit(GPIOB, GPIO_Pin_13,
                      GPIO_PortSourceGPIOB, GPIO_PinSource13,
                      EXTI_Line13, EXTI_Trigger_Rising);
											
    GPIO_EXTI_PinInit(GPIOB, GPIO_Pin_14,
                      GPIO_PortSourceGPIOB, GPIO_PinSource14,
                      EXTI_Line14, EXTI_Trigger_Rising_Falling);

    GPIO_EXTI_PinInit(GPIOB, GPIO_Pin_15,
                      GPIO_PortSourceGPIOB, GPIO_PinSource15,
                      EXTI_Line15, EXTI_Trigger_Rising_Falling);

    GPIO_EXTI_NVIC_Init();
}


void MH22xx_PortInit(void)
{
    RCC_APB2PeriphClockCmd(
        RCC_APB2Periph_GPIOA |
        RCC_APB2Periph_GPIOB |
        RCC_APB2Periph_GPIOC |
        RCC_APB2Periph_GPIOD |
        RCC_APB2Periph_GPIOF |
        RCC_APB2Periph_AFIO,
        ENABLE
    );

    GPIO_Input_Init();
    GPIO_Output_Init();
    GPIO_EXTI_Init();
}


/* Weak default callback. Application may provide a strong one. */
__attribute__((weak)) void GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    (void)GPIO_Pin;
}


void EXTI2_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line2) != RESET)
    {
        GPIO_EXTI_Callback(GPIO_Pin_2);
        EXTI_ClearITPendingBit(EXTI_Line2);
    }
}


void EXTI3_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line3) != RESET)
    {
        GPIO_EXTI_Callback(GPIO_Pin_3);
        EXTI_ClearITPendingBit(EXTI_Line3);
    }
}


void EXTI9_5_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line8) != RESET)
    {
        GPIO_EXTI_Callback(GPIO_Pin_8);
        EXTI_ClearITPendingBit(EXTI_Line8);
    }
}


void EXTI15_10_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line11) != RESET)
    {
        GPIO_EXTI_Callback(GPIO_Pin_11);
        EXTI_ClearITPendingBit(EXTI_Line11);
    }

    if (EXTI_GetITStatus(EXTI_Line12) != RESET)
    {
        GPIO_EXTI_Callback(GPIO_Pin_12);
        EXTI_ClearITPendingBit(EXTI_Line12);
    }

    if (EXTI_GetITStatus(EXTI_Line13) != RESET)
    {
        g_FanSpeedCnt++;
        GPIO_EXTI_Callback(GPIO_Pin_13);
        EXTI_ClearITPendingBit(EXTI_Line13);
    }

    if (EXTI_GetITStatus(EXTI_Line14) != RESET)
    {
        GPIO_EXTI_Callback(GPIO_Pin_14);
        EXTI_ClearITPendingBit(EXTI_Line14);
    }

    if (EXTI_GetITStatus(EXTI_Line15) != RESET)
    {
        GPIO_EXTI_Callback(GPIO_Pin_15);
        EXTI_ClearITPendingBit(EXTI_Line15);
    }
}
