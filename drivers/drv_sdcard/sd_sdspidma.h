/**
  ******************************************************************************
  * @file    sd_sdspidma.h
  * @author  mousie-yu
  * @version V1.0.0
  * @date    2014.4.28
  * @brief   SD����������, ʹ��SPI DMAģʽ.
  *          ֧��MMC��(δ��), SD��, SDHC��. ���֧��4G�Ŀ�.
  *          ����������֧�� drv_sdcard �� drv_sdflash. ����ֱ�ӵ���.
  *
  *          RVMDK���Ż�ֻ���� level-0, ����дû�гɹ�. ԭ����.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SD_SDSPIDMA_H
#define __SD_SDSPIDMA_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup SD_SDSPIDMA
  * @{
  */



/**
  ******************************************************************************
  * @defgroup SD_SDSPIDMA_Configure
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
//#define SDCARD_MODE_SPIDMA                                                      ///< drv_sdcard ʹ��SPI DMAģʽͨѶ
//#define SDFLASH_MODE_SPIDMA                                                     ///< drv_sdflashʹ��SPI DMAģʽͨѶ
//
//#if (defined SDCARD_MODE_SPIDMA) || (defined SDFLASH_MODE_SPIDMA)
//    #define SD_SPI_X                    2                                       ///< ѡ��Ӳ��SPI��, 1��ʾSPI1, 2��ʾSPI2
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



/** @defgroup SD_SDSPIDMA_Public_TypeDefine
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

/** @defgroup SD_SDSPIDMA_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup SD_SDSPIDMA_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */

/**
  * @}
  */

/** @defgroup SD_SDSPIDMA_Public_Function
  * @brief    ���幫�к���
  * @{
  */
#if (defined SDCARD_MODE_SPIDMA) || (defined SDFLASH_MODE_SPIDMA)

// ��Ҫֱ�ӵ���������Щ����, SD����غ����� drv_sdcard ��
void sdspidma_port_init(void);                                                  // SD��SPI�ڳ�ʼ������

uint8_t sdspidma_init(uint16_t block_size);                                     // ��ʼ��SD������SPIģʽ. 0, �ɹ�. 1, ʧ��
uint8_t sdspidma_get_info(sd_info_t *info);                                     // SPIģʽ��ȡSD����Ϣ. 0, �ɹ�. 1, ʧ��

uint8_t sdspidma_read_block(uint8_t* pbuf, uint32_t sector);                    // SD��SPIģʽ��������
uint8_t sdspidma_write_block(const uint8_t* pbuf, uint32_t sector);             // SD��SPIģʽд������

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
