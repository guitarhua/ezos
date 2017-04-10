/**
  ******************************************************************************
  * @file    sf_universal.h
  * @author  mousie-yu
  * @version V1.0.0
  * @date    2011.12.23
  * @brief   SPI Flash ͨ������. ʹ�ñ�׼��SPIͨѶЭ��. ���ݴ�������̵�SPI FLASH.
  *          ����������֧�� drv_spiflash. ����ֱ�ӵ���.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SF_UNIVERSAL_H
#define __SF_UNIVERSAL_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "drv_spi.h"



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup SF_Universal
  * @{
  */



/**
  ******************************************************************************
  * @defgroup SF_Universal_Configure
  * @brief    �û�����
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // Ӳ�����������ļ�

/** @defgroup SF_Universal_Pin_Assignment
  * @brief    SPI Flash���Ŷ�Ӧ��ϵ, ����ͼ:
  *           +-------------------------------------------------------+
  *           |                     Pin assignment                    |
  *           +-------------------------+---------------+-------------+
  *           | STM32 SPI Pins          |   SPI Flash   |    Pin      |
  *           +-------------------------+---------------+-------------+
  *           | SPIFLASH_CS_PIN         |   /CS         |    1        |
  *           | SPIFLASH_MISO_PIN       |   DO          |    2        |
  *           |                         |   /WP         |    3 (GND)  |
  *           |                         |   GND         |    4 (GND)  |
  *           |                         |   VCC         |    5 (3.3V) |
  *           |                         |   /HOLD       |    6 (VCC)  |
  *           | SPIFLASH_CLK_PIN        |   CLK         |    7        |
  *           | SPIFLASH_MOSI_PIN       |   DIO         |    8        |
  *           +-------------------------+---------------+-------------+
  * @{
  */
//#define SPIFLASH_UNIVERSAL
//
//#ifdef SPIFLASH_UNIVERSAL                                                       ///< ѡ��SPI FLASHͨ������
//    #define SPIFLASH_SPI_NAME           FLASH_SPI                               ///< ��������ʹ��һ��SPI, SPI��SCK, MISO, MOSI����"drv_spi.h"������
//
//    #define SPIFLASH_CS_PORT            GPIOA                                   ///< SpiFlash Ƭѡ�źŵ�PORT��
//    #define SPIFLASH_CS_CLK             RCC_APB2Periph_GPIOA                    ///< SpiFlash Ƭѡ�źŵ�ʱ��ģ��
//    #define SPIFLASH_CS_PIN             GPIO_Pin_4                              ///< SpiFlash Ƭѡ�źŵ�PIN��
//#endif
/**
  * @}
  */

/**
  * @}
  */



/** @defgroup SF_Universal_Public_TypeDefine
  * @brief    �������Ͷ���
  * @{
  */
/**
  * @}
  */

/** @defgroup SF_Universal_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */
#ifdef SPIFLASH_UNIVERSAL

// д�������λ�궨��(Status Register)
#define BP3                             0x20
#define BP2                             0x10
#define BP1                             0x08
#define BP0                             0x04

#endif
/**
  * @}
  */

/** @defgroup SF_Universal_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */

/**
  * @}
  */

/** @defgroup SF_Universal_Public_Function
  * @brief    ���幫�к���
  * @{
  */
#ifdef SPIFLASH_UNIVERSAL

// ���º����� drv_spiflash ʹ��. ����ֱ�ӵ���.
void sf_unvsl_init(void);                                                       // SpiFlash Universal ͨѶ�ڳ�ʼ��
/**
  ******************************************************************************
  * ����д��������
  * ÿ��оƬ�ɱ������򶼲�һ��, ������Ҫ�ɲο������ֲ����ж���
  * ������г�2����õı�����ʽ: ȫ��д����, ȫ����д
  ******************************************************************************
  */
#define SPIFLASH_PROTECT_ALL            (BP3 | BP2 | BP1 | BP0)
#define SPIFLASH_PROTECT_NONE           0
void sf_unvsl_set_protect(uint8_t status);
void sf_unvsl_power_down(void);                                                 // оƬ��������ģʽ
void sf_unvsl_power_on(void);                                                   // оƬ�����˳�����ģʽ
uint32_t sf_unvsl_read_id(void);                                                // ��ȡID��Ϣ

void sf_unvsl_sector_erase(uint32_t addr);                                      // ����һ����, ��СΪ4K byte
void sf_unvsl_block_erase(uint32_t addr);                                       // ����һ����, ��СΪ64K byte
void sf_unvsl_chip_erase(void);                                                 // ��������оƬ����, ��������ʱ5-100s�ȴ�����ɲ���.

void sf_unvsl_write_page(uint8_t *pbuf, uint32_t addr, uint32_t num);           // д��һ��ҳ

void sf_unvsl_read_start(uint32_t addr);                                        // ׼����ȡ
void sf_unvsl_read_bytes(uint8_t *pbuf, uint32_t num);                          // ��ȡ����
void sf_unvsl_read_end(void);                                                   // ������ȡ

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
