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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_UART_H
#define __DRV_UART_H

/* Include -------------------------------------------------------------------*/
#include <stdint.h>

#include "misc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup DRV_Uart
  * @{
  */
/**
  ******************************************************************************
  * @defgroup DRV_Uart_Configure
  * @brief    用户配置
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // 硬件集中配置文件

//#define UART_NUM                        3                                       ///< 设置使用的Uart数量.该驱动最多允许5个Uart数
//
//#define UART_TX_POLLING_MODE                                                    ///< UART 发送轮询模式, 此为默认值.
//#define UART_TX_INT_BUF_MODE                                                    ///< UART 发送中断缓冲区模式
//
//#define UART_RX_POLLING_MODE                                                    ///< UART 接收轮询模式, 此为默认值.
//#define UART_RX_INT_BUF_MODE                                                    ///< UART 接收中断缓冲区模式,  可与 UART_RX_INT_FUN_MODE 共用
//#define UART_RX_INT_FUN_MODE                                                    ///< UART 接收中断回调函数模式,可与 UART_RX_INT_BUF_MODE 共用
//
//#if (UART_NUM > 0)
//    /**
//      * @brief 设置该串口的外设名称
//      *   @arg USART1,                  使用USART1外设, USART1的CLK为 RCC_APB2Periph_USART1
//      *   @arg USART2,                  使用USART2外设,  其余 的CLK为 RCC_APB1Periph_XXX
//      *   @arg USART3,                  使用USART3外设
//      *   @arg UART4,                   使用UART4 外设
//      *   @arg UART5,                   使用UART5 外设
//      */
//    #define UART0_PERIPHERAL            USART1
//    #define UART0_IRQ                   USART1_IRQn                             ///< 设置该串口的中断
//    #define UART0_IRQ_HANDLER           USART1_IRQHandler                       ///< 设置该串口的中断函数
//    #define UART0_CLK                   RCC_APB2Periph_USART1                   ///< 设置该串口的时钟模块
//
//    #define UART0_TX_PORT               GPIOA                                   ///< 设置该串口的输出引脚PORT口
//    #define UART0_TX_CLK                RCC_APB2Periph_GPIOA                    ///< 设置该串口的输出引脚时钟模块
//    #define UART0_TX_PIN                GPIO_Pin_9                              ///< 设置该串口的输出引脚PIN口
//
//    #define UART0_RX_PORT               GPIOA                                   ///< 设置该串口的输入引脚PORT口
//    #define UART0_RX_CLK                RCC_APB2Periph_GPIOA                    ///< 设置该串口的输入引脚时钟模块
//    #define UART0_RX_PIN                GPIO_Pin_10                             ///< 设置该串口的输入引脚PIN口
//
//    /**
//      * @brief 设置该串口的引脚重映射功能. 与使用的IO口有关.
//      *   @arg 0,                       不使用引脚重映射功能
//      *   @arg GPIO_Remap_USART1,       USART1 引脚重映射
//      *   @arg GPIO_Remap_USART2,       USART2 引脚重映射
//      *   @arg GPIO_PartialRemap_USART3,USART3 部分引脚重映射
//      *   @arg GPIO_FullRemap_USART3,   USART3 全部引脚重映射
//      */
//    #define UART0_GPIO_REMAP            0
//
//    /**
//      * @brief 设置该串口的接收中断回调函数, 仅 UART_RX_INT_FUN_MODE 下需要配置
//      *   @arg 0,                       不使用回调函数, 不对接收数据进行处理
//      *   @arg void fun(uint8_t data),  回调函数格式, 宏定义fun函数名称即可
//      */
//    #define UART0_RX_CALLBACK           0
//    #define UART0_IRQ_PRIORITY          0xF                                     ///< 设置中断优先级. 高2位为可抢占主优先级, 低2位为非抢占的子优先级
//#endif
//
//#if (UART_NUM > 1)
//    #define UART1_PERIPHERAL            USART2
//    #define UART1_IRQ                   USART2_IRQn
//    #define UART1_IRQ_HANDLER           USART2_IRQHandler
//    #define UART1_CLK                   RCC_APB1Periph_USART2
//
//    #define UART1_TX_PORT               GPIOA
//    #define UART1_TX_CLK                RCC_APB2Periph_GPIOA
//    #define UART1_TX_PIN                GPIO_Pin_2
//
//    #define UART1_RX_PORT               GPIOA
//    #define UART1_RX_CLK                RCC_APB2Periph_GPIOA
//    #define UART1_RX_PIN                GPIO_Pin_3
//
//    #define UART1_GPIO_REMAP            0
//
//    #define UART1_RX_CALLBACK           0
//    #define UART1_IRQ_PRIORITY          0xF
//#endif
//
//#if (UART_NUM > 2)
//    #define UART2_PERIPHERAL            USART3
//    #define UART2_IRQ                   USART3_IRQn
//    #define UART2_IRQ_HANDLER           USART3_IRQHandler
//    #define UART2_CLK                   RCC_APB1Periph_USART3
//
//    #define UART2_TX_PORT               GPIOB
//    #define UART2_TX_CLK                RCC_APB2Periph_GPIOB
//    #define UART2_TX_PIN                GPIO_Pin_10
//
//    #define UART2_RX_PORT               GPIOB
//    #define UART2_RX_CLK                RCC_APB2Periph_GPIOB
//    #define UART2_RX_PIN                GPIO_Pin_11
//
//    #define UART2_GPIO_REMAP            0
//
//    #define UART2_RX_CALLBACK           0
//    #define UART2_IRQ_PRIORITY          0xF
//#endif
//
//#if (UART_NUM > 3)
//    #define UART3_PERIPHERAL            UART4
//    #define UART3_IRQ                   UART4_IRQn
//    #define UART3_IRQ_HANDLER           UART4_IRQHandler
//    #define UART3_CLK                   RCC_APB1Periph_UART4
//
//    #define UART3_TX_PORT               GPIOC
//    #define UART3_TX_CLK                RCC_APB2Periph_GPIOC
//    #define UART3_TX_PIN                GPIO_Pin_2
//
//    #define UART3_RX_PORT               GPIOC
//    #define UART3_RX_CLK                RCC_APB2Periph_GPIOC
//    #define UART3_RX_PIN                GPIO_Pin_3
//
//    #define UART3_GPIO_REMAP            0
//
//    #define UART3_RX_CALLBACK           0
//    #define UART3_IRQ_PRIORITY          0xF
//#endif
//
//#if (UART_NUM > 4)
//    #define UART4_PERIPHERAL            UART5
//    #define UART4_IRQ                   UART5_IRQn
//    #define UART4_IRQ_HANDLER           UART5_IRQHandler
//    #define UART4_CLK                   RCC_APB1Periph_UART5
//
//    #define UART4_TX_PORT               GPIOD
//    #define UART4_TX_CLK                RCC_APB2Periph_GPIOC
//    #define UART4_TX_PIN                GPIO_Pin_8
//
//    #define UART4_RX_PORT               GPIOD
//    #define UART4_RX_CLK                RCC_APB2Periph_GPIOC
//    #define UART4_RX_PIN                GPIO_Pin_9
//
//    #define UART4_GPIO_REMAP            0
//
//    #define UART4_RX_CALLBACK           0
//    #define UART4_IRQ_PRIORITY          0xF
//#endif
//
///// 串口名称类型定义
//typedef enum
//{
//    COM1 = 0,                                                                   ///< 命名为易于理解的串口名称, 值0对应UART0_xxx
//    COM2,                                                                       ///< 值1对应UART1_xxx, 以下类推
//    COM3,
//    COM4,
//    COM5,
//} uart_name_t;



#if (UART_NUM > 5)
    #error "UART_NUM define error!"
#endif
/**
  * @}
  */



/** @defgroup DRV_Uart_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_Uart_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_Uart_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_Uart_Public_Function
  * @brief    定义公有函数
  * @{
  */
#if (UART_NUM > 0)

/**
  ******************************************************************************
  * @brief  串口初始化, 初始化缓存区, 并使能该Uart
  * @param  uart_name
  *   @arg  可用值请参考 uart_name_t 类型定义中的枚举值
  * @param  baud_rate, 设置Uart波特率
  *   @arg  9600,                       常用波特率9600hz
  *   @arg  19200,                      常用波特率19200hz
  *   @arg  115200,                     常用波特率115200hz
  * @param  word_len   设置Uart字宽
  *   @arg  UART_WordLength_8b,         8bit传输
  *   @arg  UART_WordLength_9b,         9bit传输
  * @param  stop_bits, 设置Uart停止位
  *   @arg  UART_StopBits_0_5,          0.5bit停止位
  *   @arg  UART_StopBits_1,            1bit停止位
  *   @arg  UART_StopBits_1_5,          1.5bit停止位
  *   @arg  UART_StopBits_2,            2bit停止位
  * @param  parity, 设置Uart校验位
  *   @arg  UART_Parity_No,             无校验
  *   @arg  UART_Parity_Odd,            奇校验
  *   @arg  UART_Parity_Even,           偶校验
  * @retval none
  ******************************************************************************
  */
void uart_init(uart_name_t uart_name, uint32_t baud_rate, uint16_t word_len, uint16_t stop_bits, uint16_t parity);
void uart_deinit(uart_name_t uart_name);                                        // 禁止UART功能, 可节电
void uart_buf_clear(uart_name_t uart_name);                                     // 清空UART发送缓存区

uint8_t  uart_rx_byte(uart_name_t uart_name, uint8_t *pdata);                   // 接收一个字节数据. 0, 正确. 1, 错误. UART_RX_INT_FUN_MODE 下无意义.
uint16_t uart_rx_data(uart_name_t uart_name, uint8_t *pdata, uint16_t max_len); // 接收一串数据. 返回正确接收的字节数. UART_RX_INT_FUN_MODE 下无意义.

void uart_tx_byte(uart_name_t uart_name, uint8_t data);                         // 通过UART发送一个字节
void uart_tx_data(uart_name_t uart_name, uint8_t *pdata, uint16_t length);      // 通过UART发送一串数据
void uart_tx_data2ascii(uart_name_t uart_name, uint8_t *pdata, uint16_t length);// 将数据转换为ASCII码后通过UART发送
void uart_printf(uart_name_t uart_name, char *pstring, ...);                    // UART打印函数

#endif
/**
  * @}
  */



/**
  * @}
  */
/**
  * @}
  */

#endif
/* END OF FILE ---------------------------------------------------------------*/
