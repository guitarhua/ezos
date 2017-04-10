/**
  ******************************************************************************
  * @file    drv_uart.c
  * @author  mousie
  * @version V2.1.1
  * @date    2013.3.26
  * @brief   使用中断方式的 Uart 串口驱动程序. 接收需要定义回调函数
  *          V2.0.1, 修正了未定义接收回调函数时, 系统假死的问题.
  *          V2.1.0, UART 收发模式可选. 默认采用发送轮询, 接收回调函数模式.
  *                  支持发送轮询, 发送中断缓冲, 接收回调函数, 接收中断缓冲模式.
  *          V2.1.1, 修改 uart_tx_byte_static() 函数. 避免死机.
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
#define uart_enable_int(mask)           __set_PRIMASK(mask)                     ///< 宏定义UART中断打开函数, 方便代码移植
#define uart_disable_int()              __get_PRIMASK(); __disable_irq()        ///< 宏定义UART中断关闭函数, 方便代码移植

#define TX_BUF_SIZE                     32                                      ///< 定义发缓冲区大小, 允许范围 2-65535
#define RX_BUF_SIZE                     64                                      ///< 定义发缓冲区大小, 允许范围 2-65535

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
  * @brief    私有类型定义
  * @{
  */
/// UART硬件参数类型定义
typedef struct
{
    const uint32_t CLK;                                                         ///< UART时钟模块
    const uint32_t TX_CLK;                                                      ///< UART发送IO的时钟模块
    const uint32_t RX_CLK;                                                      ///< UART接收IO的时钟模块
    USART_TypeDef *const PERIPHERAL;                                            ///< UART模块名
    GPIO_TypeDef  *const TX_PORT;                                               ///< UART发送IO的PORT口
    GPIO_TypeDef  *const RX_PORT;                                               ///< UART接收IO的PORT口
    const uint32_t REMAP;                                                       ///< UART引脚重映射
    const uint16_t TX_PIN;                                                      ///< UART发送IO的PIN口
    const uint16_t RX_PIN;                                                      ///< UART接收IO的PIN口
#ifdef UART_RX_INT_FUN_MODE
    void (*rx_callback)(uint8_t data);                                          ///< UART接收中断回调函数
#endif
    const uint8_t  IRQ;                                                         ///< UART中断号
    const uint8_t  PRIORITY;                                                    ///< UART中断优先级
} uart_hw_t;
/**
  * @}
  */

/** @defgroup DRV_Uart_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_Uart_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */
/// UART硬件参数映射表
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
    static uint16_t tx_wr_index[UART_NUM];                                      ///< 发送缓冲区写索引
    static uint16_t tx_rd_index[UART_NUM];                                      ///< 发送缓冲区读索引
    static volatile uint16_t tx_counter[UART_NUM];                              ///< 发送缓冲区计数
    static volatile uint8_t  tx_buffer[UART_NUM][TX_BUF_SIZE];                  ///< 发送缓存区
#endif

#ifdef UART_RX_INT_BUF_MODE
    static uint16_t rx_wr_index[UART_NUM];                                      ///< 接收缓冲区写索引
    static uint16_t rx_rd_index[UART_NUM];                                      ///< 接收缓冲区读索引
    static volatile uint8_t rx_buffer[UART_NUM][RX_BUF_SIZE];                   ///< 接收缓存区
#endif

static const uint8_t *const hex2char_lowercase = "0123456789abcdef";            ///< 数字转字符索引表, 小写字母
static const uint8_t *const hex2char_capital   = "0123456789ABCDEF";            ///< 数字转字符索引表, 大写字母
/**
  * @}
  */

/** @defgroup DRV_Uart_Private_Function
  * @brief    定义私有函数
  * @{
  */
__INLINE static uint8_t uart_rx_byte_static(uart_name_t uart_name, uint8_t *pdata);
__INLINE static void uart_tx_byte_static(uart_name_t uart_name, uint8_t data);
__INLINE static void uart_irq_handler(uint8_t idx);
/**
  * @}
  */



/** @defgroup DRV_Uart_Function
  * @brief    函数原文件
  * @{
  */

/**
  ******************************************************************************
  * @brief  串口初始化, 初始化缓存区, 并使能该uart
  * @param  uart_name
  *   @arg  可用值请参考 uart_name_t 类型定义中的枚举值
  * @param  baud_rate, 设置uart波特率
  *   @arg  9600,                       常用波特率9600hz
  *   @arg  19200,                      常用波特率19200hz
  *   @arg  115200,                     常用波特率115200hz
  * @param  word_len   设置uart字宽
  *   @arg  UART_WordLength_8b,         8bit传输
  *   @arg  UART_WordLength_9b,         9bit传输
  * @param  stop_bits, 设置uart停止位
  *   @arg  UART_StopBits_0_5,          0.5bit停止位
  *   @arg  UART_StopBits_1,            1bit停止位
  *   @arg  UART_StopBits_1_5,          1.5bit停止位
  *   @arg  UART_StopBits_2,            2bit停止位
  * @param  parity, 设置uart校验位
  *   @arg  UART_Parity_No,             无校验
  *   @arg  UART_Parity_Odd,            奇校验
  *   @arg  UART_Parity_Even,           偶校验
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
        GPIO_PinRemapConfig(uart_hw[uart_name].REMAP, ENABLE);                  // 使能引脚重映射
        RCC_APB2PeriphClockCmd(uart_hw[uart_name].TX_CLK |                      // 使能 UART 的IO口时钟
                               uart_hw[uart_name].RX_CLK |
                               RCC_APB2Periph_AFIO, ENABLE);                    // 使能交替功能时钟
    }
    else
    {
        RCC_APB2PeriphClockCmd(uart_hw[uart_name].TX_CLK |                      // 使能 UART 的IO口时钟
                               uart_hw[uart_name].RX_CLK, ENABLE);
    }

    if (uart_hw[uart_name].CLK == RCC_APB2Periph_USART1)                        // 使能 UART 模块
        RCC_APB2PeriphClockCmd(uart_hw[uart_name].CLK, ENABLE);
    else
        RCC_APB1PeriphClockCmd(uart_hw[uart_name].CLK, ENABLE);

    USART_Cmd(uart, DISABLE);                                                   // 禁用Uart模块
    USART_DeInit(uart);                                                         // 重置Uart模块参数

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;                            // Tx设置为推拉输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = uart_hw[uart_name].TX_PIN;
    GPIO_Init(uart_hw[uart_name].TX_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;                      // Rx设置为高阻输入
    GPIO_InitStructure.GPIO_Pin   = uart_hw[uart_name].RX_PIN;
    GPIO_Init(uart_hw[uart_name].RX_PORT, &GPIO_InitStructure);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = uart_hw[uart_name].IRQ;                // 配置UART中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = (uart_hw[uart_name].PRIORITY & 0x0C) >> 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = (uart_hw[uart_name].PRIORITY & 0x03);
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    UART_InitStructure.USART_BaudRate            = baud_rate;                   // 设置UART参数.
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

    uart_buf_clear(uart_name);                                                  // 初始化Uart缓存区

#if (defined(UART_RX_INT_BUF_MODE) || defined(UART_RX_INT_FUN_MODE))
    USART_ITConfig(uart, USART_IT_RXNE, ENABLE);
#endif
    USART_Cmd(uart, ENABLE);
}

/**
  ******************************************************************************
  * @brief  禁止某个Uart. 并将Uart寄存器恢复为默认值. 引脚设为悬空输入.
  * @param  uart_name
  *   @arg  可用值请参考 uart_name_t 类型定义中的枚举值
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
  * @brief  清空Uart缓存区
  * @param  uart_name
  *   @arg  可用值请参考 uart_name_t 类型定义中的枚举值
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
  * @brief  通过中断方式, Uart接收一个字节
  * @param  uart_name
  *    arg  可用值请参考 uart_name_t 类型定义中的枚举值
  * @param  *pdata, 存储接收到数据
  * @retval 返回值, 0, 接收正确, 1, 接收错误
  ******************************************************************************
  */
__INLINE static uint8_t uart_rx_byte_static(uart_name_t uart_name, uint8_t *pdata)
{
    uint32_t mask;
    uint8_t  rtn;

    mask = uart_disable_int();

    if (rx_rd_index[uart_name] == rx_wr_index[uart_name])                       // 接收缓存为空
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
  * @brief  通过中断回调函数, Uart接收一个字节
  * @param  none
  * @retval 返回值, 1, 接收错误
  ******************************************************************************
  */
__INLINE static uint8_t uart_rx_byte_static(uart_name_t uart_name, uint8_t *pdata)
{
    return 1;
}

#else

/**
  ******************************************************************************
  * @brief  通过轮询方式, Uart接收一个字节
  * @param  uart_name
  *    arg  可用值请参考 uart_name_t 类型定义中的枚举值
  * @param  *pdata, 存储接收到数据
  * @retval 返回值, 0, 接收正确, 1, 接收错误
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
  * @brief  通过Uart接收一个字节. 通过中断回调函数方式无效.
  * @param  uart_name
  *    arg  可用值请参考 uart_name_t 类型定义中的枚举值
  * @param  data, 要发送的数据
  * @retval 返回值, 0, 接收正确, 1, 接收错误
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
  * @brief  通过指定的Uart接收一串数据
  * @param  uart_name
  *   @arg  可用值请参考 uart_name_t 类型定义中的枚举值
  * @param  *pdata, 要接收的数据串指针
  * @param  length, 要接收的数据串最大长度
  * @retval 返回值, 接收到的数据串实际长度
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
  * @brief  通过中断方式, Uart发送一个字节
  * @param  uart_name
  *    arg  可用值请参考 uart_name_t 类型定义中的枚举值
  * @param  data, 要发送的数据
  * @retval none
  ******************************************************************************
  */
__INLINE static void uart_tx_byte_static(uart_name_t uart_name, uint8_t data)
{
    uint32_t mask;

    mask = uart_disable_int();
    while (tx_counter[uart_name] >= TX_BUF_SIZE)                                // 发送缓存溢出时, 不再使用中断, 采用uart轮询直接发送一个数据.
    {
        USART_TypeDef *uart = uart_hw[uart_name].PERIPHERAL;

        while (!(uart->SR & USART_FLAG_TXE));                                   // 等待上一个数据发送完成
        uart->DR = tx_buffer[uart_name][tx_rd_index[uart_name]];                // 加载要发送的数据
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
  * @brief  通过轮询方式, Uart发送一个字节
  * @param  uart_name
  *    arg  可用值请参考 uart_name_t 类型定义中的枚举值
  * @param  data, 要发送的数据
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
  * @brief  通过Uart发送一个字节
  * @param  uart_name
  *    arg  可用值请参考 uart_name_t 类型定义中的枚举值
  * @param  data, 要发送的数据
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
  * @brief  通过指定的Uart发送一串数据
  * @param  uart_name
  *   @arg  可用值请参考 uart_name_t 类型定义中的枚举值
  * @param  *pdata, 要发送的数据串指针
  * @param  length, 要发送的数据串长度
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
  * @brief  通过指定的Uart, 将数据转换为ASCII码, 以十六进制显示, 字节间用空格分隔.
  * @param  uart_name
  *   @arg  可用值请参考 uart_name_t 类型定义中的枚举值
  * @param  *pdata, 要发送的数据串指针.
  * @param  length, 要发送的数据串长度
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
  * @brief  串口格式化打印函数
  * @param  uart_name
  *   @arg  可用值请参考 uart_name_t 类型定义中的枚举值
  * @param  pstring. 字符串指针. 格式判断如下:
  *   @arg  %c,     打印一个字符
  *   @arg  %d,     打印有符号10进制数, uint32_t 格式
  *   @arg  %s,     打印字符串
  *   @arg  %u,     打印无符号10进制数, uint32_t 格式
  *   @arg  %x,     打印16进制数字, uint32_t 格式, 字母小写
  *   @arg  %X,     打印16进制数字, uint32_t 格式, 字母大写
  *   @arg  %p,     打印指针, 16进制显示, uint32_t 格式, 字母小写
  *   @arg  %%,     打印%号
  *   @arg  %8c,    按8位宽度打印一个字符, 右对齐
  *   @arg  %08d,   按8位宽度打印一个整数, 填充0
  * @retval none
  ******************************************************************************
  */
void uart_printf(uart_name_t uart_name, char *pstring, ...)
{
    uint32_t value, i;
    uint16_t width, data_type, negative;
    uint8_t *pstr, fill;
    const uint8_t * phex2char;
    va_list pva_arg;                                                            // 定义一个可变参数

    assert_param(uart_name < UART_NUM);
    if (uart_name >= UART_NUM) return;

    va_start(pva_arg, pstring);
    phex2char = hex2char_lowercase;                                             // 默认16进制字母小写

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
            case '0':                                                           // 设置打印宽度
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                if ((*pstring == '0') && (width == 0)) fill = '0';              // 是%0, 则填充0
                width *= 10;                                                    // 统计格式宽度
                width += *pstring - '0';
                goto again;

            case 'c':                                                           // %c, 打印一个字符
                while (width-- > 1) uart_tx_byte_static(uart_name, fill);
                value = va_arg(pva_arg, uint32_t);                              // 从可变参数中取值
                uart_tx_byte_static(uart_name, (uint8_t)value);
                break;

            case 's': {                                                         // %s, 字符串
                pstr = va_arg(pva_arg, uint8_t *);                              // 从可变参数中取值
                for (i=0; pstr[i] != '\0'; i++)
                {
                    if (width) width--;                                         // 统计填充字符宽度
                    else break;
                }
                while (width--) uart_tx_byte_static(uart_name, ' ');
                for (i=0; pstr[i] != '\0'; i++)
                    uart_tx_byte_static(uart_name, pstr[i]);
                break; }

            case 'd': {                                                         // %d, 有符号十进制数
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

            case 'u':                                                           // %u, 无符号十进制数
                value = va_arg(pva_arg, uint32_t);                              // 从可变参数中取值
                data_type = 10;
                negative = 0;
                goto convert;

            case 'X':                                                           // %X, 无符号十六进制数
                phex2char = hex2char_capital;                                   // 改为16进制字母大写
            case 'x':                                                           // %x, 无符号十六进制数
            case 'p': {                                                         // %p, 显示一个指针
                value = va_arg(pva_arg, uint32_t);
                data_type = 16;
                negative = 0;
convert:                                                                        // 数字到字符串显示的转换
                if (negative)                                                   // 负数并且要填充0,
                {
                    if (fill == '0')
                    {
                        negative = 0;
                        uart_tx_byte_static(uart_name, '-');                    // 首字符打印负号
                    }
                    if (width) width--;
                }
                for ( i=1; (((i * data_type) <= value) && (((i * data_type) / data_type) == i)); i *= data_type )
                    if (width) width--;                                         // 统计填充宽度
                while (width-- > 1) uart_tx_byte_static(uart_name, fill);       // 打印填充字符
                if (negative) uart_tx_byte_static(uart_name, '-');              // 是负数则打印负号
                for(; i; i /= data_type)                                        // 打印数字字符
                    uart_tx_byte_static(uart_name, phex2char[(value / i) % data_type]);
                break; }

            case '%':                                                           //  %%, 输出一个百分号
                uart_tx_byte_static(uart_name, *pstring);
                break;

            default:
                uart_tx_byte_static(uart_name, '?');                            // 其它非法参数
                break;
            }
            pstring++;                                                          // 跳过格式符
        }
    }

    va_end(pva_arg);
}

/**
  ******************************************************************************
  * @brief  Uart的中断处理函数
  * @param  none
  * @retval none
  ******************************************************************************
*/
__INLINE static void uart_irq_handler(uint8_t idx)
{
#if (defined(UART_RX_INT_BUF_MODE) || defined(UART_RX_INT_FUN_MODE))
    if(USART_GetITStatus(uart_hw[idx].PERIPHERAL, USART_IT_RXNE))               // 接收寄存器非空中断
    {
        uint8_t data = uart_hw[idx].PERIPHERAL->DR;                             // 读取以清空接收中断

#ifdef UART_RX_INT_BUF_MODE
        rx_buffer[idx][rx_wr_index[idx]] = data;
        rx_wr_index[idx]++;
        if (rx_wr_index[idx] >= RX_BUF_SIZE) rx_wr_index[idx] = 0;

        if (rx_wr_index[idx] == rx_rd_index[idx])                               // 接收溢出, 采用FIFO方式复写掉最早的数据
        {
            rx_rd_index[idx]++;
            if (rx_rd_index[idx] >= RX_BUF_SIZE) rx_rd_index[idx] = 0;
        }
#endif

#ifdef UART_RX_INT_FUN_MODE
        if (uart_hw[idx].rx_callback != 0) uart_hw[idx].rx_callback(data);      // 接收数据后的回调函数
#endif
    }
#endif

#ifdef UART_TX_INT_BUF_MODE
    if(USART_GetITStatus(uart_hw[idx].PERIPHERAL, USART_IT_TXE))                // 发送寄存器空中断
    {
        if (tx_counter[idx])                                                    // 发送缓存区有数据
        {
            uart_hw[idx].PERIPHERAL->DR = tx_buffer[idx][tx_rd_index[idx]];     // 发送数据
            if (++tx_rd_index[idx] >= TX_BUF_SIZE) tx_rd_index[idx] = 0;
            tx_counter[idx]--;
        }
        else                                                                    // 没有数据了, 关闭发送中断
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
