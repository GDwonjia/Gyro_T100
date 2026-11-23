#ifndef __gpio_H
#define __gpio_H

#ifdef __cplusplus
extern "C" {
#endif

#include "board_support.h"

/* External 3.3V / Slave MCU power */
#define ON_SWMCU3V3     GPIO_SetBits(MCU_SWMCU3V3_PORT, MCU_SWMCU3V3_PIN)
#define OFF_SWMCU3V3    GPIO_ResetBits(MCU_SWMCU3V3_PORT, MCU_SWMCU3V3_PIN)

/* Collision */
#define LEFTBUMP        GPIO_ReadInputDataBit(MCU_COLLISION_L_PORT, MCU_COLLISION_L_PIN)
#define RIGHTBUMP       GPIO_ReadInputDataBit(MCU_COLLISION_R_PORT, MCU_COLLISION_R_PIN)

/* Power key */
#define MAINKEY         (!GPIO_ReadInputDataBit(MCU_KEY_POWER_PORT, MCU_KEY_POWER_PIN))

/* Hall sensors */
#define DET_DUSTBIN     GPIO_ReadInputDataBit(MCU_HALL_DUST_PORT, MCU_HALL_DUST_PIN)
#define DET_WATERTANK   GPIO_ReadInputDataBit(MCU_HALL_WATER_PORT, MCU_HALL_WATER_PIN)


/* Rear IR */
#define IR_REAR_L_GPIO_PORT       GPIOA
#define IR_REAR_L_GPIO_PIN        GPIO_Pin_2
#define IR_REAR_L_EXTI_LINE       EXTI_Line2

#define IR_REAR_R_GPIO_PORT       GPIOA
#define IR_REAR_R_GPIO_PIN        GPIO_Pin_3
#define IR_REAR_R_EXTI_LINE       EXTI_Line3

/* Front IR */
#define IR_FRONT_R_GPIO_PORT      GPIOA
#define IR_FRONT_R_GPIO_PIN       GPIO_Pin_8
#define IR_FRONT_R_EXTI_LINE      EXTI_Line8

#define IR_FRONT_L_GPIO_PORT      GPIOA
#define IR_FRONT_L_GPIO_PIN       GPIO_Pin_11
#define IR_FRONT_L_EXTI_LINE      EXTI_Line11

/* IMU: INT1 */
#define ICM_INT1_GPIO_PORT        GPIOA
#define ICM_INT1_GPIO_PIN         GPIO_Pin_12
#define ICM_INT1_EXTI_LINE        EXTI_Line12

/* Fan FG */
#define MCU_FAN_FG_GPIO_PORT      GPIOB
#define MCU_FAN_FG_GPIO_PIN       GPIO_Pin_13
#define MCU_FAN_FG_EXTI_LINE      EXTI_Line13

/* Border IR */
#define BORDER_R_GPIO_PORT        GPIOB
#define BORDER_R_GPIO_PIN         GPIO_Pin_14
#define BORDER_R_EXTI_LINE        EXTI_Line14

#define BORDER_L_GPIO_PORT        GPIOB
#define BORDER_L_GPIO_PIN         GPIO_Pin_15
#define BORDER_L_EXTI_LINE        EXTI_Line15

extern __IO uint16_t g_LSpeedCnt;
extern __IO uint16_t g_RSpeedCnt;
extern __IO uint16_t g_FanSpeedCnt;

extern volatile uint32_t WakeupFlag;

void MH22xx_PortInit(void);

void GPIO_EXTI_Callback(uint16_t GPIO_Pin);

void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void EXTI15_10_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __gpio_H */
