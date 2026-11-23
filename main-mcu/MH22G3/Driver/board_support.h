
#ifndef __BOARD_SUPPORT_H
#define __BOARD_SUPPORT_H

#include "mh22xx.h"

/*==========================================================
 * 1. PWM OUTPUT
 *==========================================================*/

// Charging PWM PB9 - TIM4_CH4
#define MCU_PWM_CHARGE_PORT          GPIOB
#define MCU_PWM_CHARGE_PIN           GPIO_Pin_9

// Main Brush PWM PB8 - TIM4_CH3
#define MCU_MAIN_PWM_PORT            GPIOB
#define MCU_MAIN_PWM_PIN             GPIO_Pin_8

// Fan PWM PB7 - TIM4_CH2
#define MCU_FAN_PWM_PORT             GPIOB
#define MCU_FAN_PWM_PIN              GPIO_Pin_7

// Side Brush PWM PB6 - TIM4_CH1
#define MCU_PWM_BRUSH_D_PORT         GPIOB
#define MCU_PWM_BRUSH_D_PIN          GPIO_Pin_6

// Touch Cliff PWM PB3 - TIM2_CH2
#define MCU_TOUCH_CLIEF_PORT         GPIOB
#define MCU_TOUCH_CLIEF_PIN          GPIO_Pin_3

// Cliff PWM PA15 - TIM2_CH1
#define MCU_PWM_CLIEF_PORT           GPIOA
#define MCU_PWM_CLIEF_PIN            GPIO_Pin_15

// Wheel PWM Left Reverse PC6 - TIM8_CH1
#define MCU_WHEEL_PWM_LR_PORT        GPIOC
#define MCU_WHEEL_PWM_LR_PIN         GPIO_Pin_6

// Wheel PWM Left Forward PC7 - TIM8_CH2
#define MCU_WHEEL_PWM_LD_PORT        GPIOC
#define MCU_WHEEL_PWM_LD_PIN         GPIO_Pin_7

// Wheel PWM Right Forward PC8 - TIM8_CH3
#define MCU_WHEEL_PWM_RR_PORT        GPIOC
#define MCU_WHEEL_PWM_RR_PIN         GPIO_Pin_8

// Wheel PWM Right Reverse PC9 - TIM8_CH4
#define MCU_WHEEL_PWM_RD_PORT        GPIOC
#define MCU_WHEEL_PWM_RD_PIN         GPIO_Pin_9


/*==========================================================
 * 2. SPEED FEEDBACK
 *==========================================================*/

// Left Wheel Speed PB5 - TIM3_CH2
#define MCU_SPEED_L_PORT             GPIOB
#define MCU_SPEED_L_PIN              GPIO_Pin_5

// Right Wheel Speed PB4 - TIM3_CH1
#define MCU_SPEED_R_PORT             GPIOB
#define MCU_SPEED_R_PIN              GPIO_Pin_4

// Fan Speed Feedback PB13 - EXTI13
#define MCU_FAN_FG_PORT              GPIOB
#define MCU_FAN_FG_PIN               GPIO_Pin_13


/*==========================================================
 * 3. IMU
 *==========================================================*/

// IMU INT1 PA12 - EXTI12
#define MCU_IMU_INT1_PORT            GPIOA
#define MCU_IMU_INT1_PIN             GPIO_Pin_12

// IMU INT2 PC12 - EXTI12 (暂不启用)
#define MCU_IMU_INT2_PORT            GPIOC
#define MCU_IMU_INT2_PIN             GPIO_Pin_12

// IMU SDA PF7
#define MCU_IMU_SDA_PORT             GPIOF
#define MCU_IMU_SDA_PIN              GPIO_Pin_7

// IMU SCL PF6
#define MCU_IMU_SCL_PORT             GPIOF
#define MCU_IMU_SCL_PIN              GPIO_Pin_6


/*==========================================================
 * 4. IR RECEIVER
 *==========================================================*/

// Left Rear IR PA2 - EXTI2
#define MCU_HL_BUMP_PORT             GPIOA
#define MCU_HL_BUMP_PIN              GPIO_Pin_2

// Right Rear IR PA3 - EXTI3
#define MCU_HR_BUMP_PORT             GPIOA
#define MCU_HR_BUMP_PIN              GPIO_Pin_3

// Left Front IR PA11 - EXTI11
#define MCU_FL_BUMP_PORT             GPIOA
#define MCU_FL_BUMP_PIN              GPIO_Pin_11

// Right Front IR PA8 - EXTI8
#define MCU_FR_BUMP_PORT             GPIOA
#define MCU_FR_BUMP_PIN              GPIO_Pin_8

// Left Border IR PB15 - EXTI15
#define MCU_BORDER_BUMP_L_PORT       GPIOB
#define MCU_BORDER_BUMP_L_PIN        GPIO_Pin_15

// Right Border IR PB14 - EXTI14
#define MCU_BORDER_BUMP_R_PORT       GPIOB
#define MCU_BORDER_BUMP_R_PIN        GPIO_Pin_14


/*==========================================================
 * 5. COLLISION INPUT
 *==========================================================*/

// Right Collision PD2
#define MCU_COLLISION_R_PORT         GPIOD
#define MCU_COLLISION_R_PIN          GPIO_Pin_2

// Left Collision PC13
#define MCU_COLLISION_L_PORT         GPIOC
#define MCU_COLLISION_L_PIN          GPIO_Pin_13


/*==========================================================
 * 6. KEY / HALL INPUT
 *==========================================================*/

// Power Key PB12
#define MCU_KEY_POWER_PORT           GPIOB
#define MCU_KEY_POWER_PIN            GPIO_Pin_12

// Dust Box Hall PC14
#define MCU_HALL_DUST_PORT           GPIOC
#define MCU_HALL_DUST_PIN            GPIO_Pin_14

// Water Tank Hall PC15
#define MCU_HALL_WATER_PORT          GPIOC
#define MCU_HALL_WATER_PIN           GPIO_Pin_15


/*==========================================================
 * 7. GPIO OUTPUT
 *==========================================================*/

// External 3.3V Enable PB2
#define MCU_SWMCU3V3_PORT            GPIOB
#define MCU_SWMCU3V3_PIN             GPIO_Pin_2


/*==========================================================
 * 8. ADC INPUT
 *==========================================================*/

// Right Side Brush Current PC0 - ADC1_CH10
#define MCU_BRUSH_ADC_R_PORT         GPIOC
#define MCU_BRUSH_ADC_R_PIN          GPIO_Pin_0

// Battery Voltage PC1 - ADC1_CH11
#define MCU_BAT_VOL_PORT             GPIOC
#define MCU_BAT_VOL_PIN              GPIO_Pin_1

// Charging Current PC2 - ADC1_CH12
#define MCU_CHARGE_CUR_PORT          GPIOC
#define MCU_CHARGE_CUR_PIN           GPIO_Pin_2

// Main Brush Current PC3 - ADC1_CH13
#define MCU_MAIN_ADC_PORT            GPIOC
#define MCU_MAIN_ADC_PIN             GPIO_Pin_3

// Left Wheel Current PA0 - ADC1_CH0
#define MCU_WHEEL_L_CUR_PORT         GPIOA
#define MCU_WHEEL_L_CUR_PIN          GPIO_Pin_0

// Right Wheel Current PA1 - ADC1_CH1
#define MCU_WHEEL_R_CUR_PORT         GPIOA
#define MCU_WHEEL_R_CUR_PIN          GPIO_Pin_1

// Wall Sensor PA4 - ADC1_CH4
#define MCU_ADC_BORDER_PORT          GPIOA
#define MCU_ADC_BORDER_PIN           GPIO_Pin_4

// Left Touch Sensor PA5 - ADC1_CH5
#define MCU_ADC_TOUCH_L_PORT         GPIOA
#define MCU_ADC_TOUCH_L_PIN          GPIO_Pin_5

// Front Touch Sensor PA6 - ADC1_CH6
#define MCU_ADC_TOUCH_F_PORT         GPIOA
#define MCU_ADC_TOUCH_F_PIN          GPIO_Pin_6

// Right Touch Sensor PA7 - ADC1_CH7
#define MCU_ADC_TOUCH_R_PORT         GPIOA
#define MCU_ADC_TOUCH_R_PIN          GPIO_Pin_7

// Right Cliff Sensor PC4 - ADC1_CH14
#define MCU_ADC_CLIEF_R_PORT         GPIOC
#define MCU_ADC_CLIEF_R_PIN          GPIO_Pin_4

// Front Cliff Sensor PC5 - ADC1_CH15
#define MCU_ADC_CLIEF_F_PORT         GPIOC
#define MCU_ADC_CLIEF_F_PIN          GPIO_Pin_5

// Left Cliff Sensor PB0 - ADC1_CH8
#define MCU_ADC_CLIEF_L_PORT         GPIOB
#define MCU_ADC_CLIEF_L_PIN          GPIO_Pin_0

// Left Side Brush Current PB1 - ADC1_CH9
#define MCU_BRUSH_ADC_L_PORT         GPIOB
#define MCU_BRUSH_ADC_L_PIN          GPIO_Pin_1


/*==========================================================
 * 9. USART / UART
 *==========================================================*/

// WiFi TX PA9 - USART1_TX
#define MCU_WIFI_TX_PORT             GPIOA
#define MCU_WIFI_TX_PIN              GPIO_Pin_9

// WiFi RX PA10 - USART1_RX
#define MCU_WIFI_RX_PORT             GPIOA
#define MCU_WIFI_RX_PIN              GPIO_Pin_10

// Debug UART3 TX PB10 - USART3_TX
#define MCU_UART3_TX_PORT            GPIOB
#define MCU_UART3_TX_PIN             GPIO_Pin_10

// Debug UART3 RX PB11 - USART3_RX
#define MCU_UART3_RX_PORT            GPIOB
#define MCU_UART3_RX_PIN             GPIO_Pin_11

// Slave MCU UART4 TX PC10 - UART4_TX
#define MCU_UART4_TX_PORT            GPIOC
#define MCU_UART4_TX_PIN             GPIO_Pin_10

// Slave MCU UART4 RX PC11 - UART4_RX
#define MCU_UART4_RX_PORT            GPIOC
#define MCU_UART4_RX_PIN             GPIO_Pin_11


/*==========================================================
 * 10. SWD DEBUG
 *==========================================================*/

// SWDIO PA13
#define MCU_SWDIO_PORT               GPIOA
#define MCU_SWDIO_PIN                GPIO_Pin_13

// SWCLK PA14
#define MCU_SWCLK_PORT               GPIOA
#define MCU_SWCLK_PIN                GPIO_Pin_14

void RCC_GPIO_Init(void);
#endif
