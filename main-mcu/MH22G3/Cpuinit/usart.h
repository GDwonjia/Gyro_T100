#ifndef __usart_H
#define __usart_H

#ifdef __cplusplus
extern "C" {
#endif

#include "board_support.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define USART_BAUDRATE          115200UL

/* USART1: Wi-Fi, PA9-TX / PA10-RX */
#define HUART_WIFI              USART1

/* USART3: Debug, PB10-TX / PB11-RX */
#define HUART_DEBUG             USART3

/* UART4: Slave MCU, PC10-TX / PC11-RX */
#define HUART_SLAVE             UART4

#define USART_RX_BUFFER_SIZE   256U

typedef struct
{
    volatile uint16_t head;
    volatile uint16_t tail;
    volatile uint32_t overflow;
    uint8_t data[USART_RX_BUFFER_SIZE];
} USART_RxBuffer_t;

extern USART_RxBuffer_t g_wifi_rx_buffer;
extern USART_RxBuffer_t g_debug_rx_buffer;
extern USART_RxBuffer_t g_slave_rx_buffer;

void USART_Init_All(void);
void USART1_Init(void);
void USART3_Init(void);
void UART4_Init(void);

void USART_SendByte(USART_TypeDef *USARTx, uint8_t data);
void USART_SendBuffer(USART_TypeDef *USARTx, const uint8_t *data, uint16_t length);

void USART1_SendByte(uint8_t data);
void USART3_SendByte(uint8_t data);
void UART4_SendByte(uint8_t data);

void USART1_SendData(const uint8_t *data, uint16_t length);
void USART3_SendData(const uint8_t *data, uint16_t length);
void UART4_SendData(const uint8_t *data, uint16_t length);

uint16_t USART_RxAvailable(USART_RxBuffer_t *buffer);
bool USART_ReadByte(USART_RxBuffer_t *buffer, uint8_t *data);
uint16_t USART_ReadData(USART_RxBuffer_t *buffer, uint8_t *data, uint16_t max_length);
int __io_putchar(int ch);
int fputc(int ch, FILE *f);

#ifdef __cplusplus
}
#endif

#endif /* __usart_H */
