#include "usart.h"
#include "misc.h"

/*==========================================================
 * RX BUFFERS
 *==========================================================*/

USART_RxBuffer_t g_wifi_rx_buffer;
USART_RxBuffer_t g_debug_rx_buffer;
USART_RxBuffer_t g_slave_rx_buffer;


/*==========================================================
 * INTERNAL FUNCTIONS
 *==========================================================*/

static void USART_GPIO_Init(GPIO_TypeDef *GPIOx,
                            uint16_t pin,
                            GPIOSpeed_TypeDef speed,
                            GPIOMode_TypeDef mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = pin;
    GPIO_InitStructure.GPIO_Speed = speed;
    GPIO_InitStructure.GPIO_Mode = mode;
    GPIO_Init(GPIOx, &GPIO_InitStructure);
}


static void USART_NVIC_Init(uint8_t channel,
                            uint8_t preemption_priority,
                            uint8_t sub_priority)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = channel;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemption_priority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub_priority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


static void USART_Config(USART_TypeDef *USARTx, uint32_t baudrate)
{
    USART_InitTypeDef USART_InitStructure;

    USART_StructInit(&USART_InitStructure);
    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USARTx, &USART_InitStructure);
    USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
    USART_Cmd(USARTx, ENABLE);
}


static void USART_RxBufferPush(USART_RxBuffer_t *buffer, uint8_t data)
{
    uint16_t next;

    next = (uint16_t)((buffer->head + 1U) % USART_RX_BUFFER_SIZE);

    if (next == buffer->tail)
    {
        buffer->overflow++;
        return;
    }

    buffer->data[buffer->head] = data;
    buffer->head = next;
}


static void USART_RxIRQHandler(USART_TypeDef *USARTx,
                               USART_RxBuffer_t *buffer)
{
    if (USART_GetITStatus(USARTx, USART_IT_RXNE) != RESET)
    {
        uint8_t data = (uint8_t)USART_ReceiveData(USARTx);
        USART_RxBufferPush(buffer, data);
    }

    /* Read SR/DR to clear receive error conditions on STM32F1-like USARTs. */
    if (USART_GetFlagStatus(USARTx, USART_FLAG_ORE) != RESET ||
        USART_GetFlagStatus(USARTx, USART_FLAG_NE) != RESET ||
        USART_GetFlagStatus(USARTx, USART_FLAG_FE) != RESET ||
        USART_GetFlagStatus(USARTx, USART_FLAG_PE) != RESET)
    {
        volatile uint32_t temp;
        temp = USARTx->SR;
        temp = USARTx->DR;
        (void)temp;
    }
}


void USART1_Init(void)
{
    /* Wi-Fi: PA9 TX, PA10 RX */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                           RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    USART_GPIO_Init(GPIOA, GPIO_Pin_9, GPIO_Speed_50MHz, GPIO_Mode_AF_PP);
    USART_GPIO_Init(GPIOA, GPIO_Pin_10, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING);

    USART_Config(USART1, USART_BAUDRATE);
    USART_NVIC_Init(USART1_IRQn, 2, 0);
}


void USART3_Init(void)
{
    /* Debug: PB10 TX, PB11 RX */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    USART_GPIO_Init(GPIOB, GPIO_Pin_10, GPIO_Speed_50MHz, GPIO_Mode_AF_PP);
    USART_GPIO_Init(GPIOB, GPIO_Pin_11, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING);

    USART_Config(USART3, USART_BAUDRATE);
    USART_NVIC_Init(USART3_IRQn, 3, 0);
}


void UART4_Init(void)
{
    /* Slave MCU: PC10 TX, PC11 RX */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |
                           RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

    USART_GPIO_Init(GPIOC, GPIO_Pin_10, GPIO_Speed_50MHz, GPIO_Mode_AF_PP);
    USART_GPIO_Init(GPIOC, GPIO_Pin_11, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING);

    USART_Config(UART4, USART_BAUDRATE);
    USART_NVIC_Init(UART4_IRQn, 2, 1);
}


void USART_Init_All(void)
{
    USART1_Init();
    USART3_Init();
    UART4_Init();
}

void USART_SendByte(USART_TypeDef *USARTx, uint8_t data)
{
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
    USART_SendData(USARTx, data);
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
}


void USART_SendBuffer(USART_TypeDef *USARTx, const uint8_t *data, uint16_t length)
{
    uint16_t i;

    if (data == 0)
    {
        return;
    }

    for (i = 0; i < length; i++)
    {
        USART_SendByte(USARTx, data[i]);
    }
}


void USART1_SendByte(uint8_t data)
{
    USART_SendByte(USART1, data);
}


void USART3_SendByte(uint8_t data)
{
    USART_SendByte(USART3, data);
}


void UART4_SendByte(uint8_t data)
{
    USART_SendByte(UART4, data);
}


void USART1_SendData(const uint8_t *data, uint16_t length)
{
    USART_SendBuffer(USART1, data, length);
}


void USART3_SendData(const uint8_t *data, uint16_t length)
{
    USART_SendBuffer(USART3, data, length);
}


void UART4_SendData(const uint8_t *data, uint16_t length)
{
    USART_SendBuffer(UART4, data, length);
}


uint16_t USART_RxAvailable(USART_RxBuffer_t *buffer)
{
    if (buffer == 0)
    {
        return 0;
    }

    if (buffer->head >= buffer->tail)
    {
        return (uint16_t)(buffer->head - buffer->tail);
    }

    return (uint16_t)(USART_RX_BUFFER_SIZE - buffer->tail + buffer->head);
}


bool USART_ReadByte(USART_RxBuffer_t *buffer, uint8_t *data)
{
    if ((buffer == 0) || (data == 0) || (buffer->head == buffer->tail))
    {
        return false;
    }

    *data = buffer->data[buffer->tail];
    buffer->tail = (uint16_t)((buffer->tail + 1U) % USART_RX_BUFFER_SIZE);

    return true;
}


uint16_t USART_ReadData(USART_RxBuffer_t *buffer, uint8_t *data, uint16_t max_length)
{
    uint16_t count = 0;

    if ((buffer == 0) || (data == 0))
    {
        return 0;
    }

    while ((count < max_length) && USART_ReadByte(buffer, &data[count]))
    {
        count++;
    }

    return count;
}

int __io_putchar(int ch)
{
    USART_SendByte(USART3, (uint8_t)ch);
    return ch;
}


int fputc(int ch, FILE *f)
{
    (void)f;
    USART_SendByte(USART3, (uint8_t)ch);
    return ch;
}


void USART1_IRQHandler(void)
{
    USART_RxIRQHandler(USART1, &g_wifi_rx_buffer);
}


void USART3_IRQHandler(void)
{
    USART_RxIRQHandler(USART3, &g_debug_rx_buffer);
}


void UART4_IRQHandler(void)
{
    USART_RxIRQHandler(UART4, &g_slave_rx_buffer);
}
