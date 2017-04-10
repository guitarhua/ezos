/**
  ******************************************************************************
  * @file    drv_sdflash.h
  * @author  mousie-yu
  * @version V2.1.0
  * @date    2015.2.13
  * @brief   ��SD����ΪFLASHʹ�õ���������.
  *          ֧�� SD_V1.x, SD_V2, SDHC��.
  *          ֧��SPIģʽ �� SDIOģʽ, ����ᵼ�¶�д����. ԭ����.
  *          ���ڵ�ַʹ�õ��� uint32_t ��ʽ, �������֧�ֵ�4G�Ŀ�.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_SDFLASH_H
#define __DRV_SDFLASH_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup DRV_SDFLASH
  * @{
  */



/**
  ******************************************************************************
  * @defgroup DRV_SDFLASH_Configure
  * @brief    �û�����
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // Ӳ�����������ļ�

//#define SDFLASH_ENABLE                                                          ///< �Ƿ�ʹ��SD FLASH����

//#define SDFLASH_POWER_NAME            SDFLASH_POWER                             ///< SD FLASH��Դ������������, û�е�Դ���Ʋ�Ҫ�궨�����

/** @defgroup SDFLASH_Pin_Assignment
  * @brief    SD���Ŷ�Ӧ��ϵ, ����ͼ:
  *           +-------------------------------------------------------+
  *           |                     Pin assignment                    |
  *           +-------------------------+---------------+-------------+
  *           | STM32        Pins       |     SD        |    Pin      |
  *           +-------------------------+---------------+-------------+
  *           |                         |   CD DAT3     |    1        |
  *           |                         |   CMD         |    2        |
  *           |                         |   GND         |    3 (0 V)  |
  *           |                         |   VDD         |    4 (3.3 V)|
  *           |                         |   CLK         |    5        |
  *           |                         |   GND         |    6 (0 V)  |
  *           |                         |   DAT0        |    7        |
  *           |                         |   DAT1        |    8        |
  *           |                         |   DAT2        |    9        |
  *           +-------------------------+---------------+-------------+
  *           |                         |    ����       |             |
  *           +-------------------------+---------------+-------------+
  *           | SDFLASH_DETECT_PIN      |   CD          |    10       |
  *           |                         |   GND         |    11 (0 V) |
  *           |                         |   WP          |    12       |
  *           +-------------------------+---------------+-------------+
  * @{
  */
//#define SDFLASH_DETECT_ENABLE                                                   ///< һ�㿨�����ɼ��SD���Ƿ����
//#define SDFLASH_DETECT_PORT             GPIOC                                   ///< SD FLASH��CD����, SD�����ڼ��. PORT��
//#define SDFLASH_DETECT_CLK              RCC_APB2Periph_GPIOC                    ///< SD FLASH��CD����, SD�����ڼ��. ʱ��ģ��
//#define SDFLASH_DETECT_PIN              GPIO_Pin_11                             ///< SD FLASH��CD����, SD�����ڼ��. PIN��
/**
  * @}
  */

#if   (defined SDFLASH_MODE_SDIO)
    #include "sd_sdio.h"
#elif (defined SDFLASH_MODE_SPIDMA)
    #include "sd_sdspidma.h"
#elif (defined SDFLASH_MODE_SPI)
    #include "sd_sdspi.h"
#endif

#if  ((defined SDFLASH_MODE_SDIO)   && (defined SDFLASH_MODE_SPIDMA))
  || ((defined SDFLASH_MODE_SDIO)   && (defined SDFLASH_MODE_SPI))
  || ((defined SDFLASH_MODE_SPIDMA) && (defined SDFLASH_MODE_SPI))

    #error "SDFLASH_MODE define error!"
#endif
/**
  * @}
  */



/** @defgroup DRV_SDFLASH_Public_TypeDefine
  * @brief    �������Ͷ���
  * @{
  */
/// SD�����Ͷ���
typedef enum
{
    SDFLASH_V1     = 0x00,                                                      ///< V1.x�汾��SD��
    SDFLASH_V2     = 0x01,                                                      ///< V2.0�汾��SD��
    SDFLASH_SDHC   = 0x02,                                                      ///< SDHC��
    SDFLASH_MMC    = 0x03,                                                      ///< MMC��
    SDFLASH_UNKNOW = 0xFF,                                                      ///< ��������
} sdflash_type_t;

/// SD����Ϣ���Ͷ���
typedef struct
{
    sdflash_type_t type;                                                        ///< SD������
    uint8_t  revision;                                                          ///< SD���汾, ʹ��BCD��. ��0x32��ʾ3.2
    uint32_t serial;                                                            ///< ���к�
    uint8_t  year;                                                              ///< �������, 0��ʾ2000��, ������
    uint8_t  month;                                                             ///< �����·�
    uint64_t capacity;                                                          ///< SD������, ��λΪ�ֽ�
} sdflash_info_t;
/**
  * @}
  */

/** @defgroup DRV_SDFLASH_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_SDFLASH_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_SDFLASH_Public_Function
  * @brief    ���幫�к���
  * @{
  */
#ifdef SDFLASH_ENABLE

uint8_t sdflash_init(void);                                                     // ��SD FLASH��Դ����ʼ��. 0, �ɹ�. 1, ʧ��
void    sdflash_power_off(void);                                                // �ر�SD FLASH��Դ
uint8_t sdflash_get_info(sdflash_info_t *info);                                 // ��ȡSD FLASH��Ϣ. 0, �ɹ�. 1, ʧ��

// ���º���ֱ�ӵ��ü���. ���迼��SD�����밴���д������
uint8_t sdflash_read(uint8_t *pbuf, uint32_t read_addr, uint32_t num);          // ��sd flash�ж�ȡ����
uint8_t sdflash_write_buf(uint8_t *pbuf, uint32_t write_addr, uint32_t num);    // д������, δ��һ��Sector�������ݴ���buf��
void    sdflash_write_buf2flash(void);                                          // ��buf�е�����д�뵽flash��

// ���º���ΪSD���Ŀ��д����, �ɹ��������ʹ��
uint8_t sdflash_read_blocks(uint8_t* pbuf, uint32_t block, uint32_t block_num);         // SD�������ȡ, blockΪ�����ʼ��
uint8_t sdflash_write_blocks(const uint8_t* pbuf, uint32_t block, uint32_t block_num);  // SD������д��, blockΪ�����ʼ��

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
