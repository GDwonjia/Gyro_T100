
#include "board_support.h"

/**
 * @brief 
 */
static void GPIO_Input_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

    // Right Collision PD2
    GPIO_InitStructure.GPIO_Pin = MCU_COLLISION_R_PIN;
    GPIO_Init(MCU_COLLISION_R_PORT, &GPIO_InitStructure);

    // Left Collision PC13
    GPIO_InitStructure.GPIO_Pin = MCU_COLLISION_L_PIN;
    GPIO_Init(MCU_COLLISION_L_PORT, &GPIO_InitStructure);

    // Power Key PB12
    GPIO_InitStructure.GPIO_Pin = MCU_KEY_POWER_PIN;
    GPIO_Init(MCU_KEY_POWER_PORT, &GPIO_InitStructure);

    // Dust Box Hall PC14
    GPIO_InitStructure.GPIO_Pin = MCU_HALL_DUST_PIN;
    GPIO_Init(MCU_HALL_DUST_PORT, &GPIO_InitStructure);

    // Water Tank Hall PC15
    GPIO_InitStructure.GPIO_Pin = MCU_HALL_WATER_PIN;
    GPIO_Init(MCU_HALL_WATER_PORT, &GPIO_InitStructure);
}


/**
 * @brief 
 */
static void GPIO_Output_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

    // External 3.3V Enable PB2
    GPIO_InitStructure.GPIO_Pin = MCU_SWMCU3V3_PIN;
    GPIO_Init(MCU_SWMCU3V3_PORT, &GPIO_InitStructure);
}


/**
 * @brief 
 */
void RCC_GPIO_Init(void)
{

    RCC_APB2PeriphClockCmd(
				RCC_APB2Periph_AFIO  |
        RCC_APB2Periph_GPIOA |
        RCC_APB2Periph_GPIOB |
        RCC_APB2Periph_GPIOC |
        RCC_APB2Periph_GPIOD |
        RCC_APB2Periph_GPIOF,
        ENABLE
    );
    GPIO_Input_Init();
    GPIO_Output_Init();
}
