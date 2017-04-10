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
  * @brief    �û�����
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // Ӳ�����������ļ�

//#define UART_NUM                        3                                       ///< ����ʹ�õ�Uart����.�������������5��Uart��
//
//#define UART_TX_POLLING_MODE                                                    ///< UART ������ѯģʽ, ��ΪĬ��ֵ.
//#define UART_TX_INT_BUF_MODE                                                    ///< UART �����жϻ�����ģʽ
//
//#define UART_RX_POLLING_MODE                                                    ///< UART ������ѯģʽ, ��ΪĬ��ֵ.
//#define UART_RX_INT_BUF_MODE                                                    ///< UART �����жϻ�����ģʽ,  ���� UART_RX_INT_FUN_MODE ����
//#define UART_RX_INT_FUN_MODE                                                    ///< UART �����жϻص�����ģʽ,���� UART_RX_INT_BUF_MODE ����
//
//#if (UART_NUM > 0)
//    /**
//      * @brief ���øô��ڵ���������
//      *   @arg USART1,                  ʹ��USART1����, USART1��CLKΪ RCC_APB2Periph_USART1
//      *   @arg USART2,                  ʹ��USART2����,  ���� ��CLKΪ RCC_APB1Periph_XXX
//      *   @arg USART3,                  ʹ��USART3����
//      *   @arg UART4,                   ʹ��UART4 ����
//      *   @arg UART5,                   ʹ��UART5 ����
//      */
//    #define UART0_PERIPHERAL            USART1
//    #define UART0_IRQ                   USART1_IRQn                             ///< ���øô��ڵ��ж�
//    #define UART0_IRQ_HANDLER           USART1_IRQHandler                       ///< ���øô��ڵ��жϺ���
//    #define UART0_CLK                   RCC_APB2Periph_USART1                   ///< ���øô��ڵ�ʱ��ģ��
//
//    #define UART0_TX_PORT               GPIOA                                   ///< ���øô��ڵ��������PORT��
//    #define UART0_TX_CLK                RCC_APB2Periph_GPIOA                    ///< ���øô��ڵ��������ʱ��ģ��
//    #define UART0_TX_PIN                GPIO_Pin_9                              ///< ���øô��ڵ��������PIN��
//
//    #define UART0_RX_PORT               GPIOA                                   ///< ���øô��ڵ���������PORT��
//    #define UART0_RX_CLK                RCC_APB2Periph_GPIOA                    ///< ���øô��ڵ���������ʱ��ģ��
//    #define UART0_RX_PIN                GPIO_Pin_10                             ///< ���øô��ڵ���������PIN��
//
//    /**
//      * @brief ���øô��ڵ�������ӳ�书��. ��ʹ�õ�IO���й�.
//      *   @arg 0,                       ��ʹ��������ӳ�书��
//      *   @arg GPIO_Remap_USART1,       USART1 ������ӳ��
//      *   @arg GPIO_Remap_USART2,       USART2 ������ӳ��
//      *   @arg GPIO_PartialRemap_USART3,USART3 ����������ӳ��
//      *   @arg GPIO_FullRemap_USART3,   USART3 ȫ��������ӳ��
//      */
//    #define UART0_GPIO_REMAP            0
//
//    /**
//      * @brief ���øô��ڵĽ����жϻص�����, �� UART_RX_INT_FUN_MODE ����Ҫ����
//      *   @arg 0,                       ��ʹ�ûص�����, ���Խ������ݽ��д���
//      *   @arg void fun(uint8_t data),  �ص�������ʽ, �궨��fun�������Ƽ���
//      */
//    #define UART0_RX_CALLBACK           0
//    #define UART0_IRQ_PRIORITY          0xF                                     ///< �����ж����ȼ�. ��2λΪ����ռ�����ȼ�, ��2λΪ����ռ�������ȼ�
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
///// �����������Ͷ���
//typedef enum
//{
//    COM1 = 0,                                                                   ///< ����Ϊ�������Ĵ�������, ֵ0��ӦUART0_xxx
//    COM2,                                                                       ///< ֵ1��ӦUART1_xxx, ��������
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
  * @brief    �������Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_Uart_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_Uart_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_Uart_Public_Function
  * @brief    ���幫�к���
  * @{
  */
#if (UART_NUM > 0)

/**
  ******************************************************************************
  * @brief  ���ڳ�ʼ��, ��ʼ��������, ��ʹ�ܸ�Uart
  * @param  uart_name
  *   @arg  ����ֵ��ο� uart_name_t ���Ͷ����е�ö��ֵ
  * @param  baud_rate, ����Uart������
  *   @arg  9600,                       ���ò�����9600hz
  *   @arg  19200,                      ���ò�����19200hz
  *   @arg  115200,                     ���ò�����115200hz
  * @param  word_len   ����Uart�ֿ�
  *   @arg  UART_WordLength_8b,         8bit����
  *   @arg  UART_WordLength_9b,         9bit����
  * @param  stop_bits, ����Uartֹͣλ
  *   @arg  UART_StopBits_0_5,          0.5bitֹͣλ
  *   @arg  UART_StopBits_1,            1bitֹͣλ
  *   @arg  UART_StopBits_1_5,          1.5bitֹͣλ
  *   @arg  UART_StopBits_2,            2bitֹͣλ
  * @param  parity, ����UartУ��λ
  *   @arg  UART_Parity_No,             ��У��
  *   @arg  UART_Parity_Odd,            ��У��
  *   @arg  UART_Parity_Even,           żУ��
  * @retval none
  ******************************************************************************
  */
void uart_init(uart_name_t uart_name, uint32_t baud_rate, uint16_t word_len, uint16_t stop_bits, uint16_t parity);
void uart_deinit(uart_name_t uart_name);                                        // ��ֹUART����, �ɽڵ�
void uart_buf_clear(uart_name_t uart_name);                                     // ���UART���ͻ�����

uint8_t  uart_rx_byte(uart_name_t uart_name, uint8_t *pdata);                   // ����һ���ֽ�����. 0, ��ȷ. 1, ����. UART_RX_INT_FUN_MODE ��������.
uint16_t uart_rx_data(uart_name_t uart_name, uint8_t *pdata, uint16_t max_len); // ����һ������. ������ȷ���յ��ֽ���. UART_RX_INT_FUN_MODE ��������.

void uart_tx_byte(uart_name_t uart_name, uint8_t data);                         // ͨ��UART����һ���ֽ�
void uart_tx_data(uart_name_t uart_name, uint8_t *pdata, uint16_t length);      // ͨ��UART����һ������
void uart_tx_data2ascii(uart_name_t uart_name, uint8_t *pdata, uint16_t length);// ������ת��ΪASCII���ͨ��UART����
void uart_printf(uart_name_t uart_name, char *pstring, ...);                    // UART��ӡ����

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
