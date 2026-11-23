#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "delay.h"
#include "mh22xx.h"
#include "rcc.h"
#include "board_support.h"
#include "timer.h"
#include "pwm.h"
#include "adc.h"
#include "usart.h"
#include "Systick.h"

#define PRINTF_LOG 	printf

#define GPIO_GROUP_TEST		GPIOB
#define GPIO_MODE_TEST		GPIO_Mode_IPU
#define GPIO_SPEED_TEST		GPIO_Speed_50MHz
#define GPIO_PIN1_TEST		GPIO_Pin_1

USART_TypeDef* USART_TEST = USART1;

void UART_Configuration(uint32_t bound);


int main(void)
{
		RCC_ClkConfiguration();

		RCC_GPIO_Init();

		SysTick_Init();

		Timer_Init();
		PWM_Init();
		ADCInit();
		USART_Init_All();

    while (1)
		{
				if (FLAG1MS)
				{
						FLAG1MS = 0;
				}

				if (FLAG5MS)
				{
						FLAG5MS = 0;
				}

				if (FLAG10MS)
				{
						FLAG10MS = 0;
				}

				if (FLAG20MS)
				{
						FLAG20MS = 0;
				}

				if (FLAG50MS)
				{
						FLAG50MS = 0;
				}

				if (FLAG100MS)
				{
						FLAG100MS = 0;
				}

				if (FLAG500MS)
				{
						FLAG500MS = 0;
				}

				if (FLAG1000MS)
				{
						FLAG1000MS = 0;
				}
		}
}


