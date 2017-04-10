/**
  ******************************************************************************
  * @file    sd_sdspi.h
  * @author  mousie-yu
  * @version V2.1.0
  * @date    2015.2.13
  * @brief   SD����������, ʹ��SPIģʽ.
  *          ֧��MMC��(δ��), SD��, SDHC��. ���֧��4G�Ŀ�.
  *          ����������֧�� drv_sdcard �� drv_sdflash. ����ֱ�ӵ���.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SD_SDSPI_H
#define __SD_SDSPI_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "drv_spi.h"



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup SD_SDSPI
  * @{
  */



/**
  ******************************************************************************
  * @defgroup SD_SDSPI_Configure
  * @brief    �û�����
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // Ӳ�����������ļ�

/** @defgroup SDSPI_Pin_Assignment
  * @brief    SD SPI���Ŷ�Ӧ��ϵ, ����ͼ:
  *           +-------------------------------------------------------+
  *           |                     Pin assignment                    |
  *           +-------------------------+---------------+-------------+
  *           | STM32 SD SPI Pins       |     SD        |    Pin      |
  *           +-------------------------+---------------+-------------+
  *           | SD_CS_PIN               |   CD DAT3     |    1        |
  *           | SD_SPI_MOSI_PIN         |   CMD         |    2        |
  *           |                         |   GND         |    3 (GND)  |
  *           |                         |   VDD         |    4 (3.3 V)|
  *           | SD_SPI_SCK_PIN          |   CLK         |    5        |
  *           |                         |   GND         |    6 (GND)  |
  *           | SD_SPI_MISO_PIN         |   DAT0        |    7        |
  *           |                         |   DAT1        |    8        |
  *           |                         |   DAT2        |    9        |
  *           +-------------------------+---------------+-------------+
  * @{
  */
//#define SDCARD_MODE_SPI                                                         ///< drv_sdcard ʹ��SPIģʽͨѶ
//#define SDFLASH_MODE_SPI                                                        ///< drv_sdflashʹ��SPIģʽͨѶ
//
//#if (defined SDCARD_MODE_SPI) || (defined SDFLASH_MODE_SPI)
//    #define SD_SPI_NAME                 SD_SPI                                  ///< ��������ʹ��һ��SPI, SPI��SCK, MISO, MOSI����"drv_spi.h"������
//    #define SD_CS_PORT                  GPIOB                                   ///< SD��Ƭѡ�źŵ�PORT��
//    #define SD_CS_CLK                   RCC_APB2Periph_GPIOB                    ///< SD��Ƭѡ�źŵ�ʱ��ģ��
//    #define SD_CS_PIN                   GPIO_Pin_12                             ///< SD��Ƭѡ�źŵ�PIN��
//#endif

#if ((defined SDCARD_MODE_SPI) && (defined SDFLASH_MODE_SPI))
    #error "SDxxx_MODE_SPI define error!"
#endif
/**
  * @}
  */

/**
  * @}
  */



/** @defgroup SD_SDSPI_Public_TypeDefine
  * @brief    �������Ͷ���
  * @{
  */
/// SD�����Ͷ���
typedef enum
{
    SD_V1 = 0x00,                                                               ///< V1.x�汾��SD��
    SD_V2,                                                                      ///< V2.0�汾��SD��
    SDHC,                                                                       ///< SDHC��
    MMC,                                                                        ///< MMC��
    UNKNOW,                                                                     ///< ��������
} sd_type_t;

/// SD����Ϣ���Ͷ���
typedef struct
{
    sd_type_t sd_type;                                                          ///< SD������
    uint8_t  sd_manu;                                                           ///< �������̱��, ��SD����֯����
    uint8_t  sd_oem[3];                                                         ///< OEM��ʾ, ��SD����֯����
    uint8_t  sd_product[6];                                                     ///< ������������
    uint8_t  sd_rev;                                                            ///< SD���汾, ʹ��BCD��. ��0x32��ʾ3.2
    uint32_t sd_sn;                                                             ///< ���к�
    uint8_t  sd_year;                                                           ///< �������, 0��ʾ2000��, ������
    uint8_t  sd_mon;                                                            ///< �����·�
    uint64_t sd_cap;                                                            ///< SD������, ��λΪ�ֽ�
} sd_info_t;
/**
  * @}
  */

/** @defgroup SD_SDSPI_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup SD_SDSPI_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */

/**
  * @}
  */

/** @defgroup SD_SDSPI_Public_Function
  * @brief    ���幫�к���
  * @{
  */
#if (defined SDCARD_MODE_SPI) || (defined SDFLASH_MODE_SPI)

// ��Ҫֱ�ӵ���������Щ����, SD����غ����� drv_sdcard ��
void sdspi_port_init(void);                                                     // SD��SPI�ڳ�ʼ������

uint8_t sdspi_init(uint16_t block_size);                                        // ��ʼ��SD������SPIģʽ. 0, �ɹ�. 1, ʧ��
uint8_t sdspi_get_info(sd_info_t *info);                                        // SPIģʽ��ȡSD����Ϣ. 0, �ɹ�. 1, ʧ��

uint8_t sdspi_read_block(uint8_t* pbuf, uint32_t sector);                                           // SD��SPIģʽ��������
uint8_t sdspi_read_multi_blocks(uint8_t* pbuf, uint32_t sector, uint32_t block_num);                // SD��SPIģʽ�������
uint8_t sdspi_write_block(const uint8_t* pbuf, uint32_t sector);                                    // SD��SPIģʽд������
uint8_t sdspi_write_multi_blocks(const uint8_t* pbuf, uint32_t sector, uint32_t block_num);         // SD��SPIģʽд�����

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
