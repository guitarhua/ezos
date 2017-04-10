/**
  ******************************************************************************
  * @file    drv_uart.c
  * @author  mousie
  * @version V2.1.1
  * @date    2013.3.26
  * @brief   ʹ���жϷ�ʽ�� Uart ������������. ������Ҫ����ص�����
  *          V2.0.1, ������δ������ջص�����ʱ, ϵͳ����������.
  *          V2.1.0, UART �շ�ģʽ��ѡ. Ĭ�ϲ��÷�����ѯ, ���ջص�����ģʽ.
  *                  ֧�ַ�����ѯ, �����жϻ���, ���ջص�����, �����жϻ���ģʽ.
  *          V2.1.1, �޸� uart_tx_byte_static() ����. ��������.
  ******************************************************************************
  */

/* Include -------------------------------------------------------------------*/
#include "drv_uart.h"
#include <stdarg.h>



/** @addtogroup Drivers
  * @{
  */
/** @defgroup DRV_Uart
  * @{
  */
#if (UART_NUM > 0)



/**
  ******************************************************************************
  * @addtogroup DRV_Uart_Configure
  ******************************************************************************
  * @{
  */
#include "stm32f10x.h"
#define uart_enable_int(mask)           __set_PRIMASK(mask)                     ///< �궨��UART�жϴ򿪺���, ���������ֲ
#define uart_disable_int()              __get_PRIMASK(); __disable_irq()        ///< �궨��UART�жϹرպ���, ���������ֲ

#define TX_BUF_SIZE                     32                                      ///< ���巢��������С, ����Χ 2-65535
#define RX_BUF_SIZE                     64                                      ///< ���巢��������С, ����Χ 2-65535

#if ((TX_BUF_SIZE < 2) || (TX_BUF_SIZE > 65535))
    #error "TX_BUF_SIZE define error!"
#endif

#if ((RX_BUF_SIZE < 2) || (RX_BUF_SIZE > 65535))
    #error "RX_BUF_SIZE define error!"
#endif

#if (defined(UART_TX_POLLING_MODE) && defined(UART_TX_INT_BUF_MODE))
    #error "UART_TX_MODE define error!"
#endif

#if (defined(UART_RX_POLLING_MODE) && (defined(UART_RX_INT_BUF_MODE) || defined(UART_RX_INT_FUN_MODE)))
    #error "UART_RX_MODE define error!"
#endif
/**
  * @}
  */



/** @defgroup DRV_Uart_Private_TypeDefine
  * @brief    ˽�����Ͷ���
  * @{
  */
/// UARTӲ���������Ͷ���
typedef struct
{
    const uint32_t CLK;                                                         ///< UARTʱ��ģ��
    const uint32_t TX_CLK;                                                      ///< UART����IO��ʱ��ģ��
    const uint32_t RX_CLK;                                                      ///< UART����IO��ʱ��ģ��
    USART_TypeDef *const PERIPHERAL;                                            ///< UARTģ����
    GPIO_TypeDef  *const TX_PORT;                                               ///< UART����IO��PORT��
    GPIO_TypeDef  *const RX_PORT;                                               ///< UART����IO��PORT��
    const uint32_t REMAP;                                                       ///< UART������ӳ��
    const uint16_t TX_PIN;                                                      ///< UART����IO��PIN��
    const uint16_t RX_PIN;                                                      ///< UART����IO��PIN��
#ifdef UART_RX_INT_FUN_MODE
    void (*rx_callback)(uint8_t data);                                          ///< UART�����жϻص�����
#endif
    const uint8_t  IRQ;                                                         ///< UART�жϺ�
    const uint8_t  PRIORITY;                                                    ///< UART�ж����ȼ�
} uart_hw_t;
/**
  * @}
  */

/** @defgroup DRV_Uart_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_Uart_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */
/// UARTӲ������ӳ���
static const uart_hw_t uart_hw[UART_NUM] = {
#if (UART_NUM > 0)
    {UART0_CLK,         UART0_TX_CLK,  UART0_RX_CLK,
     UART0_PERIPHERAL,  UART0_TX_PORT, UART0_RX_PORT,
     UART0_GPIO_REMAP,  UART0_TX_PIN,  UART0_RX_PIN,
#ifdef UART_RX_INT_FUN_MODE
     UART0_RX_CALLBACK,
#endif
     UART0_IRQ,     UART0_IRQ_PRIORITY},
#endif

#if (UART_NUM > 1)
    {UART1_CLK,         UART1_TX_CLK,  UART1_RX_CLK,
     UART1_PERIPHERAL,  UART1_TX_PORT, UART1_RX_PORT,
     UART1_GPIO_REMAP,  UART1_TX_PIN,  UART1_RX_PIN,
#ifdef UART_RX_INT_FUN_MODE
     UART1_RX_CALLBACK,
#endif
     UART1_IRQ,     UART1_IRQ_PRIORITY},
#endif

#if (UART_NUM > 2)
    {UART2_CLK,         UART2_TX_CLK,  UART2_RX_CLK,
     UART2_PERIPHERAL,  UART2_TX_PORT, UART2_RX_PORT,
     UART2_GPIO_REMAP,  UART2_TX_PIN,  UART2_RX_PIN,
#ifdef UART_RX_INT_FUN_MODE
     UART2_RX_CALLBACK,
#endif
     UART2_IRQ,     UART2_IRQ_PRIORITY},
#endif

#if (UART_NUM > 3)
    {UART3_CLK,         UART3_TX_CLK,  UART3_RX_CLK,
     UART3_PERIPHERAL,  UART3_TX_PORT, UART3_RX_PORT,
     UART3_GPIO_REMAP,  UART3_TX_PIN,  UART3_RX_PIN,
#ifdef UART_RX_INT_FUN_MODE
     UART3_RX_CALLBACK,
#endif
     UART3_IRQ,     UART3_IRQ_PRIORITY},
#endif

#if (UART_NUM > 4)
    {UART4_CLK,         UART4_TX_CLK,  UART4_RX_CLK,
     UART4_PERIPHERAL,  UART4_TX_PORT, UART4_RX_PORT,
     UART4_GPIO_REMAP,  UART4_TX_PIN,  UART4_RX_PIN,
#ifdef UART_RX_INT_FUN_MODE
     UART4_RX_CALLBACK,
#endif
     UART4_IRQ,     UART4_IRQ_PRIORITY},
#endif
};

#ifdef UART_TX_INT_BUF_MODE
    static uint16_t tx_wr_index[UART_NUM];                                      ///< ���ͻ�����д����
    static uint16_t tx_rd_index[UART_NUM];                                      ///< ���ͻ�����������
    static volatile uint16_t tx_counter[UART_NUM];                              ///< ���ͻ���������
    static volatile uint8_t  tx_buffer[UART_NUM][TX_BUF_SIZE];                  ///< ���ͻ�����
#endif

#ifdef UART_RX_INT_BUF_MODE
    static uint16_t rx_wr_index[UART_NUM];                                      ///< ���ջ�����д����
    static uint16_t rx_rd_index[UART_NUM];                                      ///< ���ջ�����������
    static volatile uint8_t rx_buffer[UART_NUM][RX_BUF_SIZE];                   ///< ���ջ�����
#endif

static const uint8_t *const hex2char_lowercase = "0123456789abcdef";            ///< ����ת�ַ�������, Сд��ĸ
static const uint8_t *const hex2char_capital   = "0123456789ABCDEF";            ///< ����ת�ַ�������, ��д��ĸ
/**
  * @}
  */

/** @defgroup DRV_Uart_Private_Function
  * @brief    ����˽�к���
  * @{
  */
__INLINE static uint8_t uart_rx_byte_static(uart_name_t uart_name, uint8_t *pdata);
__INLINE static void uart_tx_byte_static(uart_name_t uart_name, uint8_t data);
__INLINE static void uart_irq_handler(uint8_t idx);
/**
  * @}
  */



/** @defgroup DRV_Uart_Function
  * @brief    ����ԭ�ļ�
  * @{
  */

/**
  ******************************************************************************
  * @brief  ���ڳ�ʼ��, ��ʼ��������, ��ʹ�ܸ�uart
  * @param  uart_name
  *   @arg  ����ֵ��ο� uart_name_t ���Ͷ����е�ö��ֵ
  * @param  baud_rate, ����uart������
  *   @arg  9600,                       ���ò�����9600hz
  *   @arg  19200,                      ���ò�����19200hz
  *   @arg  115200,                     ���ò�����115200hz
  * @param  word_len   ����uart�ֿ�
  *   @arg  UART_WordLength_8b,         8bit����
  *   @arg  UART_WordLength_9b,         9bit����
  * @param  stop_bits, ����uartֹͣλ
  *   @arg  UART_StopBits_0_5,          0.5bitֹͣλ
  *   @arg  UART_StopBits_1,            1bitֹͣλ
  *   @arg  UART_StopBits_1_5,          1.5bitֹͣλ
  *   @arg  UART_StopBits_2,            2bitֹͣλ
  * @param  parity, ����uartУ��λ
  *   @arg  UART_Parity_No,             ��У��
  *   @arg  UART_Parity_Odd,            ��У��
  *   @arg  UART_Parity_Even,           żУ��
  * @retval none
  ******************************************************************************
  */
void uart_init(uart_name_t uart_name, uint32_t baud_rate, uint16_t word_len, uint16_t stop_bits, uint16_t parity)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef UART_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
    USART_ClockInitTypeDef UART_ClockInitStructure;
    USART_TypeDef *uart = uart_hw[uart_name].PERIPHERAL;

    assert_param(uart_name < UART_NUM);
    assert_param(IS_USART_BAUDRATE(baud_rate));
    assert_param(IS_USART_WORD_LENGTH(word_len));
    assert_param(IS_USART_STOPBITS(stop_bits));
    assert_param(IS_USART_PARITY(parity));
    if (uart_name >= UART_NUM) return;

    if (uart_hw[uart_name].REMAP != 0)
    {
        GPIO_PinRemapConfig(uart_hw[uart_name].REMAP, ENABLE);                  // ʹ��������ӳ��
        RCC_APB2PeriphClockCmd(uart_hw[uart_name].TX_CLK |                      // ʹ�� UART ��IO��ʱ��
                               uart_hw[uart_name].RX_CLK |
                               RCC_APB2Periph_AFIO, ENABLE);                    // ʹ�ܽ��湦��ʱ��
    }
    else
    {
        RCC_APB2PeriphClockCmd(uart_hw[uart_name].TX_CLK |                      // ʹ�� UART ��IO��ʱ��
                               uart_hw[uart_name].RX_CLK, ENABLE);
    }

    if (uart_hw[uart_name].CLK == RCC_APB2Periph_USART1)                        // ʹ�� UART ģ��
        RCC_APB2PeriphClockCmd(uart_hw[uart_name].CLK, ENABLE);
    else
        RCC_APB1PeriphClockCmd(uart_hw[uart_name].CLK, ENABLE);

    USART_Cmd(uart, DISABLE);                                                   // ����Uartģ��
    USART_DeInit(uart);                                                         // ����Uartģ�����

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;                            // Tx����Ϊ�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = uart_hw[uart_name].TX_PIN;
    GPIO_Init(uart_hw[uart_name].TX_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;                      // Rx����Ϊ��������
    GPIO_InitStructure.GPIO_Pin   = uart_hw[uart_name].RX_PIN;
    GPIO_Init(uart_hw[uart_name].RX_PORT, &GPIO_InitStructure);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = uart_hw[uart_name].IRQ;                // ����UART�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = (uart_hw[uart_name].PRIORITY & 0x0C) >> 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = (uart_hw[uart_name].PRIORITY & 0x03);
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    UART_InitStructure.USART_BaudRate            = baud_rate;                   // ����UART����.
    UART_InitStructure.USART_WordLength          = word_len;
    UART_InitStructure.USART_StopBits            = stop_bits;
    UART_InitStructure.USART_Parity              = parity;
    UART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    UART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(uart, &UART_InitStructure);

    UART_ClockInitStructure.USART_Clock   = USART_Clock_Disable;
    UART_ClockInitStructure.USART_CPOL    = USART_CPOL_Low;
    UART_ClockInitStructure.USART_CPHA    = USART_CPHA_2Edge;
    UART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
    USART_ClockInit(USART1, &UART_ClockInitStructure);

    uart_buf_clear(uart_name);                                                  // ��ʼ��Uart������

#if (defined(UART_RX_INT_BUF_MODE) || defined(UART_RX_INT_FUN_MODE))
    USART_ITConfig(uart, USART_IT_RXNE, ENABLE);
#endif
    USART_Cmd(uart, ENABLE);
}

/**
  ******************************************************************************
  * @brief  ��ֹĳ��Uart. ����Uart�Ĵ����ָ�ΪĬ��ֵ. ������Ϊ��������.
  * @param  uart_name
  *   @arg  ����ֵ��ο� uart_name_t ���Ͷ����е�ö��ֵ
  * @retval none
  ******************************************************************************
  */
void uart_deinit(uart_name_t uart_name)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
    USART_TypeDef *uart = uart_hw[uart_name].PERIPHERAL;

    assert_param(uart_name < UART_NUM);
    if (uart_name >= UART_NUM) return;

    USART_Cmd(uart, DISABLE);
    USART_DeInit(uart);

    if (uart_hw[uart_name].CLK == RCC_APB2Periph_USART1)
        RCC_APB2PeriphClockCmd(uart_hw[uart_name].CLK, DISABLE);
    else
        RCC_APB1PeriphClockCmd(uart_hw[uart_name].CLK, DISABLE);

    uart_buf_clear(uart_name);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Pin = uart_hw[uart_name].TX_PIN;
    GPIO_Init(uart_hw[uart_name].TX_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = uart_hw[uart_name].RX_PIN;
    GPIO_Init(uart_hw[uart_name].RX_PORT, &GPIO_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = uart_hw[uart_name].IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
  ******************************************************************************
  * @brief  ���Uart������
  * @param  uart_name
  *   @arg  ����ֵ��ο� uart_name_t ���Ͷ����е�ö��ֵ
  * @retval none
  ******************************************************************************
  */
void uart_buf_clear(uart_name_t uart_name)
{
    uint32_t mask;

    assert_param(uart_name < UART_NUM);
    if (uart_name >= UART_NUM) return;

    mask = uart_disable_int();

#ifdef UART_TX_INT_BUF_MODE
    tx_wr_index[uart_name] = 0;
    tx_rd_index[uart_name] = 0;
    tx_counter[uart_name]  = 0;
#endif

#ifdef UART_RX_INT_BUF_MODE
    rx_wr_index[uart_name] = 0;
    rx_rd_index[uart_name] = 0;
#endif

    uart_enable_int(mask);
}

#if defined(UART_RX_INT_BUF_MODE)
/**
  ******************************************************************************
  * @brief  ͨ���жϷ�ʽ, Uart����һ���ֽ�
  * @param  uart_name
  *    arg  ����ֵ��ο� uart_name_t ���Ͷ����е�ö��ֵ
  * @param  *pdata, �洢���յ�����
  * @retval ����ֵ, 0, ������ȷ, 1, ���մ���
  ******************************************************************************
  */
__INLINE static uint8_t uart_rx_byte_static(uart_name_t uart_name, uint8_t *pdata)
{
    uint32_t mask;
    uint8_t  rtn;

    mask = uart_disable_int();

    if (rx_rd_index[uart_name] == rx_wr_index[uart_name])                       // ���ջ���Ϊ��
    {
        rtn = 1;
    }
    else
    {
        *pdata = rx_buffer[uart_name][rx_rd_index[uart_name]];
        if (++rx_rd_index[uart_name] >= RX_BUF_SIZE) rx_rd_index[uart_name] = 0;
        rtn = 0;
    }

    uart_enable_int(mask);
    return rtn;
}

#elif defined(UART_RX_INT_FUN_MODE)

/**
  ******************************************************************************
  * @brief  ͨ���жϻص�����, Uart����һ���ֽ�
  * @param  none
  * @retval ����ֵ, 1, ���մ���
  ******************************************************************************
  */
__INLINE static uint8_t uart_rx_byte_static(uart_name_t uart_name, uint8_t *pdata)
{
    return 1;
}

#else

/**
  ******************************************************************************
  * @brief  ͨ����ѯ��ʽ, Uart����һ���ֽ�
  * @param  uart_name
  *    arg  ����ֵ��ο� uart_name_t ���Ͷ����е�ö��ֵ
  * @param  *pdata, �洢���յ�����
  * @retval ����ֵ, 0, ������ȷ, 1, ���մ���
  ******************************************************************************
  */
__INLINE static uint8_t uart_rx_byte_static(uart_name_t uart_name, uint8_t *pdata)
{
    USART_TypeDef *uart = uart_hw[uart_name].PERIPHERAL;

    if (uart->SR & USART_FLAG_RXNE)
    {
        *pdata = uart->DR;
        return 0;
    }
    else
    {
        return 1;
    }
}

#endif

/**
  ******************************************************************************
  * @brief  ͨ��Uart����һ���ֽ�. ͨ���жϻص�������ʽ��Ч.
  * @param  uart_name
  *    arg  ����ֵ��ο� uart_name_t ���Ͷ����е�ö��ֵ
  * @param  data, Ҫ���͵�����
  * @retval ����ֵ, 0, ������ȷ, 1, ���մ���
  ******************************************************************************
  */
uint8_t uart_rx_byte(uart_name_t uart_name, uint8_t *pdata)
{
    assert_param(uart_name < UART_NUM);
    if (uart_name >= UART_NUM) return 1;

    return uart_rx_byte_static(uart_name, pdata);
}

/**
  ******************************************************************************
  * @brief  ͨ��ָ����Uart����һ������
  * @param  uart_name
  *   @arg  ����ֵ��ο� uart_name_t ���Ͷ����е�ö��ֵ
  * @param  *pdata, Ҫ���յ����ݴ�ָ��
  * @param  length, Ҫ���յ����ݴ���󳤶�
  * @retval ����ֵ, ���յ������ݴ�ʵ�ʳ���
  ******************************************************************************
*/
uint16_t uart_rx_data(uart_name_t uart_name, uint8_t *pdata, uint16_t max_len)
{
    uint16_t i;
    uint8_t  val;

    assert_param(uart_name < UART_NUM);
    if (uart_name >= UART_NUM) return 0;

    for (i=0; i<max_len; i++)
    {
        val = uart_rx_byte_static(uart_name, pdata++);
        if (val != 0) break;
    }

    return i;
}

#ifdef UART_TX_INT_BUF_MODE
/**
  ******************************************************************************
  * @brief  ͨ���жϷ�ʽ, Uart����һ���ֽ�
  * @param  uart_name
  *    arg  ����ֵ��ο� uart_name_t ���Ͷ����е�ö��ֵ
  * @param  data, Ҫ���͵�����
  * @retval none
  ******************************************************************************
  */
__INLINE static void uart_tx_byte_static(uart_name_t uart_name, uint8_t data)
{
    uint32_t mask;

    mask = uart_disable_int();
    while (tx_counter[uart_name] >= TX_BUF_SIZE)                                // ���ͻ������ʱ, ����ʹ���ж�, ����uart��ѯֱ�ӷ���һ������.
    {
        USART_TypeDef *uart = uart_hw[uart_name].PERIPHERAL;

        while (!(uart->SR & USART_FLAG_TXE));                                   // �ȴ���һ�����ݷ������
        uart->DR = tx_buffer[uart_name][tx_rd_index[uart_name]];                // ����Ҫ���͵�����
        if (++tx_rd_index[uart_name] >= TX_BUF_SIZE) tx_rd_index[uart_name] = 0;
        tx_counter[uart_name]--;
    }

    tx_buffer[uart_name][tx_wr_index[uart_name]] = data;
    tx_counter[uart_name]++;
    if (++tx_wr_index[uart_name] >= TX_BUF_SIZE) tx_wr_index[uart_name] = 0;
    USART_ITConfig(uart_hw[uart_name].PERIPHERAL, USART_IT_TXE, ENABLE);
    uart_enable_int(mask);
}

#else

/**
  ******************************************************************************
  * @brief  ͨ����ѯ��ʽ, Uart����һ���ֽ�
  * @param  uart_name
  *    arg  ����ֵ��ο� uart_name_t ���Ͷ����е�ö��ֵ
  * @param  data, Ҫ���͵�����
  * @retval none
  ******************************************************************************
  */
__INLINE static void uart_tx_byte_static(uart_name_t uart_name, uint8_t data)
{
    USART_TypeDef *uart = uart_hw[uart_name].PERIPHERAL;

    while (!(uart->SR & USART_FLAG_TXE));
    uart->DR = data;
}
#endif

/**
  ******************************************************************************
  * @brief  ͨ��Uart����һ���ֽ�
  * @param  uart_name
  *    arg  ����ֵ��ο� uart_name_t ���Ͷ����е�ö��ֵ
  * @param  data, Ҫ���͵�����
  * @retval none
  ******************************************************************************
  */
void uart_tx_byte(uart_name_t uart_name, uint8_t data)
{
    assert_param(uart_name < UART_NUM);
    if (uart_name >= UART_NUM) return;

    uart_tx_byte_static(uart_name, data);
}

/**
  ******************************************************************************
  * @brief  ͨ��ָ����Uart����һ������
  * @param  uart_name
  *   @arg  ����ֵ��ο� uart_name_t ���Ͷ����е�ö��ֵ
  * @param  *pdata, Ҫ���͵����ݴ�ָ��
  * @param  length, Ҫ���͵����ݴ�����
  * @retval none
  ******************************************************************************
*/
void uart_tx_data(uart_name_t uart_name, uint8_t *pdata, uint16_t length)
{
    assert_param(uart_name < UART_NUM);
    if (uart_name >= UART_NUM) return;

    while (length--)
    {
        uart_tx_byte_static(uart_name, *pdata++);
    }
}

/**
  ******************************************************************************
  * @brief  ͨ��ָ����Uart, ������ת��ΪASCII��, ��ʮ��������ʾ, �ֽڼ��ÿո�ָ�.
  * @param  uart_name
  *   @arg  ����ֵ��ο� uart_name_t ���Ͷ����е�ö��ֵ
  * @param  *pdata, Ҫ���͵����ݴ�ָ��.
  * @param  length, Ҫ���͵����ݴ�����
  * @retval none
  ******************************************************************************
*/
void uart_tx_data2ascii(uart_name_t uart_name, uint8_t *pdata, uint16_t length)
{
    uint16_t i;
    uint8_t  temp;

    assert_param(uart_name < UART_NUM);
    if (uart_name >= UART_NUM) return;

    for (i=0; i < length; i++)
    {
        temp = (*pdata) >> 4;
        if (temp < 10) temp += '0';
        else           temp += ('A' - 10);
        uart_tx_byte(uart_name, temp);

        temp = (*pdata++) & 0x0F;
        if (temp < 10) temp += '0';
        else           temp += ('A' - 10);
        uart_tx_byte_static(uart_name, temp);
        uart_tx_byte_static(uart_name, ' ');
    }
}

/**
  ******************************************************************************
  * @brief  ���ڸ�ʽ����ӡ����
  * @param  uart_name
  *   @arg  ����ֵ��ο� uart_name_t ���Ͷ����е�ö��ֵ
  * @param  pstring. �ַ���ָ��. ��ʽ�ж�����:
  *   @arg  %c,     ��ӡһ���ַ�
  *   @arg  %d,     ��ӡ�з���10������, uint32_t ��ʽ
  *   @arg  %s,     ��ӡ�ַ���
  *   @arg  %u,     ��ӡ�޷���10������, uint32_t ��ʽ
  *   @arg  %x,     ��ӡ16��������, uint32_t ��ʽ, ��ĸСд
  *   @arg  %X,     ��ӡ16��������, uint32_t ��ʽ, ��ĸ��д
  *   @arg  %p,     ��ӡָ��, 16������ʾ, uint32_t ��ʽ, ��ĸСд
  *   @arg  %%,     ��ӡ%��
  *   @arg  %8c,    ��8λ��ȴ�ӡһ���ַ�, �Ҷ���
  *   @arg  %08d,   ��8λ��ȴ�ӡһ������, ���0
  * @retval none
  ******************************************************************************
  */
void uart_printf(uart_name_t uart_name, char *pstring, ...)
{
    uint32_t value, i;
    uint16_t width, data_type, negative;
    uint8_t *pstr, fill;
    const uint8_t * phex2char;
    va_list pva_arg;                                                            // ����һ���ɱ����

    assert_param(uart_name < UART_NUM);
    if (uart_name >= UART_NUM) return;

    va_start(pva_arg, pstring);
    phex2char = hex2char_lowercase;                                             // Ĭ��16������ĸСд

    while (*pstring)
    {
        while ((*pstring != '%') && (*pstring != '\0'))
            uart_tx_byte_static(uart_name, *pstring++);

        if (*pstring == '%')
        {
            width = 0;
            fill = ' ';
again:
            pstring++;
            switch(*pstring)
            {
            case '0':                                                           // ���ô�ӡ���
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                if ((*pstring == '0') && (width == 0)) fill = '0';              // ��%0, �����0
                width *= 10;                                                    // ͳ�Ƹ�ʽ���
                width += *pstring - '0';
                goto again;

            case 'c':                                                           // %c, ��ӡһ���ַ�
                while (width-- > 1) uart_tx_byte_static(uart_name, fill);
                value = va_arg(pva_arg, uint32_t);                              // �ӿɱ������ȡֵ
                uart_tx_byte_static(uart_name, (uint8_t)value);
                break;

            case 's': {                                                         // %s, �ַ���
                pstr = va_arg(pva_arg, uint8_t *);                              // �ӿɱ������ȡֵ
                for (i=0; pstr[i] != '\0'; i++)
                {
                    if (width) width--;                                         // ͳ������ַ����
                    else break;
                }
                while (width--) uart_tx_byte_static(uart_name, ' ');
                for (i=0; pstr[i] != '\0'; i++)
                    uart_tx_byte_static(uart_name, pstr[i]);
                break; }

            case 'd': {                                                         // %d, �з���ʮ������
                value = va_arg(pva_arg, uint32_t);
                if ((int32_t)value < 0)
                {
                    value = -value;
                    negative = 1;
                }
                else
                {
                    negative = 0;
                }
                data_type = 10;
                goto convert; }

            case 'u':                                                           // %u, �޷���ʮ������
                value = va_arg(pva_arg, uint32_t);                              // �ӿɱ������ȡֵ
                data_type = 10;
                negative = 0;
                goto convert;

            case 'X':                                                           // %X, �޷���ʮ��������
                phex2char = hex2char_capital;                                   // ��Ϊ16������ĸ��д
            case 'x':                                                           // %x, �޷���ʮ��������
            case 'p': {                                                         // %p, ��ʾһ��ָ��
                value = va_arg(pva_arg, uint32_t);
                data_type = 16;
                negative = 0;
convert:                                                                        // ���ֵ��ַ�����ʾ��ת��
                if (negative)                                                   // ��������Ҫ���0,
                {
                    if (fill == '0')
                    {
                        negative = 0;
                        uart_tx_byte_static(uart_name, '-');                    // ���ַ���ӡ����
                    }
                    if (width) width--;
                }
                for ( i=1; (((i * data_type) <= value) && (((i * data_type) / data_type) == i)); i *= data_type )
                    if (width) width--;                                         // ͳ�������
                while (width-- > 1) uart_tx_byte_static(uart_name, fill);       // ��ӡ����ַ�
                if (negative) uart_tx_byte_static(uart_name, '-');              // �Ǹ������ӡ����
                for(; i; i /= data_type)                                        // ��ӡ�����ַ�
                    uart_tx_byte_static(uart_name, phex2char[(value / i) % data_type]);
                break; }

            case '%':                                                           //  %%, ���һ���ٷֺ�
                uart_tx_byte_static(uart_name, *pstring);
                break;

            default:
                uart_tx_byte_static(uart_name, '?');                            // �����Ƿ�����
                break;
            }
            pstring++;                                                          // ������ʽ��
        }
    }

    va_end(pva_arg);
}

/**
  ******************************************************************************
  * @brief  Uart���жϴ�����
  * @param  none
  * @retval none
  ******************************************************************************
*/
__INLINE static void uart_irq_handler(uint8_t idx)
{
#if (defined(UART_RX_INT_BUF_MODE) || defined(UART_RX_INT_FUN_MODE))
    if(USART_GetITStatus(uart_hw[idx].PERIPHERAL, USART_IT_RXNE))               // ���ռĴ����ǿ��ж�
    {
        uint8_t data = uart_hw[idx].PERIPHERAL->DR;                             // ��ȡ����ս����ж�

#ifdef UART_RX_INT_BUF_MODE
        rx_buffer[idx][rx_wr_index[idx]] = data;
        rx_wr_index[idx]++;
        if (rx_wr_index[idx] >= RX_BUF_SIZE) rx_wr_index[idx] = 0;

        if (rx_wr_index[idx] == rx_rd_index[idx])                               // �������, ����FIFO��ʽ��д�����������
        {
            rx_rd_index[idx]++;
            if (rx_rd_index[idx] >= RX_BUF_SIZE) rx_rd_index[idx] = 0;
        }
#endif

#ifdef UART_RX_INT_FUN_MODE
        if (uart_hw[idx].rx_callback != 0) uart_hw[idx].rx_callback(data);      // �������ݺ�Ļص�����
#endif
    }
#endif

#ifdef UART_TX_INT_BUF_MODE
    if(USART_GetITStatus(uart_hw[idx].PERIPHERAL, USART_IT_TXE))                // ���ͼĴ������ж�
    {
        if (tx_counter[idx])                                                    // ���ͻ�����������
        {
            uart_hw[idx].PERIPHERAL->DR = tx_buffer[idx][tx_rd_index[idx]];     // ��������
            if (++tx_rd_index[idx] >= TX_BUF_SIZE) tx_rd_index[idx] = 0;
            tx_counter[idx]--;
        }
        else                                                                    // û��������, �رշ����ж�
        {
            USART_ITConfig(uart_hw[idx].PERIPHERAL, USART_IT_TXE, DISABLE);
        }
    }
#endif
}

#if (UART_NUM > 0)
void UART0_IRQ_HANDLER(void)
{
    uart_irq_handler(0);
}
#endif

#if (UART_NUM > 1)
void UART1_IRQ_HANDLER(void)
{
    uart_irq_handler(1);
}
#endif

#if (UART_NUM > 2)
void UART2_IRQ_HANDLER(void)
{
    uart_irq_handler(2);
}
#endif

#if (UART_NUM > 3)
void UART3_IRQ_HANDLER(void)
{
    uart_irq_handler(3);
}
#endif

#if (UART_NUM > 4)
void UART4_IRQ_HANDLER(void)
{
    uart_irq_handler(4);
}
#endif

/**
  * @}
  */



#endif
/**
  * @}
  */
/**
  * @}
  */

/* END OF FILE ---------------------------------------------------------------*/
