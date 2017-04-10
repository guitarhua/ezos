/**
  ******************************************************************************
  * @file    sf_mx25l.h
  * @author  mousie-yu
  * @version V2.0.0
  * @date    2011.12.23
  * @brief   ���� MX25Lϵ�� SPI Flash ��������. ʹ�� 4DTRD ģʽ��, 4PPģʽд.
  *          ����������֧�� drv_spiflash. ����ֱ�ӵ���.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SF_MX25L_H
#define __SF_MX25L_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup SF_MX25L
  * @{
  */



/**
  ******************************************************************************
  * @defgroup SF_MX25L_Configure
  * @brief    �û�����
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // Ӳ�����������ļ�

/** @defgroup SF_MX25L_Pin_Assignment
  * @brief    Ϊ׷�����Ķ�ȡ�ٶ�, Ӳ�����ϸ�Ҫ��. ��������������.
  *           SIO0-SIO3 �������η�����ͬһPORT�ڵ�PIN0-PIN3������.
  *           ���Ŷ�Ӧ��ϵ, ����ͼ:
  *           +-----------------------------------------------------+
  *           |                   Pin assignment                    |
  *           +------------------------------+----------------------+
  *           | STM32 SPI Pins               |      MX25LоƬ       |
  *           +------------------------------+----------------------+
  *           | MX25L_CS_PIN                 |      /CS             |
  *           | MX25L_SCLK_PIN               |      SCLK            |
  *           | MX25L_SIO0_PIN(������PIN0��) |      SI/SIO0         |
  *           | MX25L_SIO1_PIN(������PIN1��) |      SO/SIO1         |
  *           | MX25L_SIO2_PIN(������PIN2��) |      /WP/SIO2        |
  *           | MX25L_SIO3_PIN(������PIN3��) |      SIO3            |
  *           |                              |      GND             |
  *           |                              |      VCC(3.3V)       |
  *           +------------------------------+----------------------+
  * @{
  */
//#define MX25L_4DTRD_4PP                                                         ///< ѡ�� MX25L ˫����4IO�� �� 4IO д������

#ifdef MX25L_4DTRD_4PP
//    #define MX25L_CS_PORT               GPIOD                                   ///< MX25LƬѡ�źŵ�PORT��
//    #define MX25L_CS_CLK                RCC_APB2Periph_GPIOD                    ///< MX25LƬѡ�źŵ�ʱ��ģ��
//    #define MX25L_CS_PIN                GPIO_Pin_5                              ///< MX25LƬѡ�źŵ�PIN��
//
//    #define MX25L_SCLK_PORT             GPIOD                                   ///< MX25Lʱ���źŵ�PORT��
//    #define MX25L_SCLK_CLK              RCC_APB2Periph_GPIOD                    ///< MX25Lʱ���źŵ�ʱ��ģ��
//    #define MX25L_SCLK_PIN              GPIO_Pin_4                              ///< MX25Lʱ���źŵ�PIN��
//
//    #define MX25L_SIO_PORT              GPIOD                                   ///< MX25L������0��PORT��
//    #define MX25L_SIO_CLK               RCC_APB2Periph_GPIOD                    ///< MX25L������0��ʱ��ģ��

    #define MX25L_SIO0_PIN              GPIO_Pin_0                              ///< MX25L������0��PIN��, ���������GPIO_Pin_0��
    #define MX25L_SIO1_PIN              GPIO_Pin_1                              ///< MX25L������1��PIN��, ���������GPIO_Pin_1��
    #define MX25L_SIO2_PIN              GPIO_Pin_2                              ///< MX25L������2��PIN��, ���������GPIO_Pin_2��
    #define MX25L_SIO3_PIN              GPIO_Pin_3                              ///< MX25L������3��PIN��, ���������GPIO_Pin_3��
#endif
/**
  * @}
  */

/**
  * @}
  */



/** @defgroup SF_MX25L_Public_TypeDefine
  * @brief    �������Ͷ���
  * @{
  */
/**
  * @}
  */

/** @defgroup SF_MX25L_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */
#ifdef MX25L_4DTRD_4PP

// д�������λ�궨��(Status Register)
#define BP3                             0x20
#define BP2                             0x10
#define BP1                             0x08
#define BP0                             0x04

#endif
/**
  * @}
  */

/** @defgroup SF_MX25L_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */

/**
  * @}
  */

/** @defgroup SF_MX25L_Public_Function
  * @brief    ���幫�к���
  * @{
  */
#ifdef MX25L_4DTRD_4PP

// ���º����� drv_spiflash ʹ��. ����ֱ�ӵ���.
void mx25l_init(void);                                                           // ͨѶ�ڳ�ʼ��
/**
  ******************************************************************************
  * ����д��������
  * ÿ��оƬ�ɱ������򶼲�һ��, ������Ҫ�ɲο������ֲ����ж���
  * ������г�2����õı�����ʽ: ȫ��д����, ȫ����д
  ******************************************************************************
  */
#define MX25L_PROTECT_ALL                (BP3 | BP2 | BP1 | BP0)
#define MX25L_PROTECT_NONE               0
void mx25l_set_protect(uint8_t status);
void mx25l_power_down(void);                                                     // оƬ��������ģʽ
void mx25l_power_on(void);                                                       // оƬ�����˳�����ģʽ
uint32_t mx25l_read_id(void);                                                    // ��ȡID��Ϣ

void mx25l_sector_erase(uint32_t addr);                                          // ����һ����, ��СΪ4K byte
void mx25l_block_erase(uint32_t addr);                                           // ����һ����, ��СΪ64K byte
void mx25l_chip_erase(void);                                                     // ��������оƬ����, ��������ʱ5-100s�ȴ�����ɲ���.

void mx25l_write_page(uint8_t *pbuf, uint32_t addr, uint32_t num);               // д��һ��ҳ

void mx25l_read_start(uint32_t addr);                                            // ׼����ȡ
void mx25l_read_bytes(uint8_t *pbuf, uint32_t num);                              // ��ȡ����
void mx25l_read_end(void);                                                       // ������ȡ

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
