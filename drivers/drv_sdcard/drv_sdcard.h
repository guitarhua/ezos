/**
  ******************************************************************************
  * @file    drv_sdcard.h
  * @author  mousie-yu
  * @version V2.1.0
  * @date    2015.2.13
  * @brief   SD����������, ֧�� SD_V1.x, SD_V2, SDHC��.
  *          ֧��SPIģʽ �� SDIOģʽ, ��Ҫ drv_sdspi �� drv_sdio ����.
  *          ���ڵ�ַʹ�õ��� uint32_t ��ʽ, �������֧�ֵ�4G�Ŀ�.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_SDCARD_H
#define __DRV_SDCARD_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup DRV_SDCARD
  * @{
  */



/**
  ******************************************************************************
  * @defgroup DRV_SDCARD_Configure
  * @brief    �û�����
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // Ӳ�����������ļ�

//#define SDCARD_ENABLE                                                           ///< �Ƿ�ʹ��SD������

//#define SDCARD_POWER_NAME               SDCARD_POWER                            ///< SD����Դ������������, û�е�Դ���Ʋ�Ҫ�궨�����

/** @defgroup SDCARD_Pin_Assignment
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
  *           | SDCARD_DETECT_PIN       |   CD          |    10       |
  *           |                         |   GND         |    11 (0 V) |
  *           |                         |   WP          |    12       |
  *           +-------------------------+---------------+-------------+
  * @{
  */
//#define SDCARD_DETECT_ENABLE                                                    ///< һ�㿨�����ɼ��SD���Ƿ����
//#define SDCARD_DETECT_PORT              GPIOC                                   ///< SD����CD����, SD�����ڼ��. PORT��
//#define SDCARD_DETECT_CLK               RCC_APB2Periph_GPIOC                    ///< SD����CD����, SD�����ڼ��. ʱ��ģ��
//#define SDCARD_DETECT_PIN               GPIO_Pin_11                             ///< SD����CD����, SD�����ڼ��. PIN��
/**
  * @}
  */

#if   (defined SDCARD_MODE_SDIO)
    #include "sd_sdio.h"
#elif (defined SDCARD_MODE_SPIDMA)
    #include "sd_sdspidma.h"
#elif (defined SDCARD_MODE_SPI)
    #include "sd_sdspi.h"
#endif

#if  ((defined SDCARD_MODE_SDIO)   && (defined SDCARD_MODE_SPIDMA))
  || ((defined SDCARD_MODE_SDIO)   && (defined SDCARD_MODE_SPI))
  || ((defined SDCARD_MODE_SPIDMA) && (defined SDCARD_MODE_SPI))

    #error "SDCARD_MODE define error!"
#endif
/**
  * @}
  */



/** @defgroup DRV_SDCARD_Public_TypeDefine
  * @brief    �������Ͷ���
  * @{
  */
/// SD�����Ͷ���
typedef enum
{
    SDCARD_V1     = 0x00,                                                       ///< V1.x�汾��SD��
    SDCARD_V2     = 0x01,                                                       ///< V2.0�汾��SD��
    SDCARD_SDHC   = 0x02,                                                       ///< SDHC��
    SDCARD_MMC    = 0x03,                                                       ///< MMC��
    SDCARD_UNKNOW = 0xFF,                                                       ///< ��������
} sdcard_type_t;

/// SD����Ϣ���Ͷ���
typedef struct
{
    sdcard_type_t type;                                                         ///< SD������
    uint8_t  revision;                                                          ///< SD���汾, ʹ��BCD��. ��0x32��ʾ3.2
    uint32_t serial;                                                            ///< ���к�
    uint8_t  year;                                                              ///< �������, 0��ʾ2000��, ������
    uint8_t  month;                                                             ///< �����·�
    uint64_t capacity;                                                          ///< SD������, ��λΪ�ֽ�
} sdcard_info_t;
/**
  * @}
  */

/** @defgroup DRV_SDCARD_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_SDCARD_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_SDCARD_Public_Function
  * @brief    ���幫�к���
  * @{
  */
#ifdef SDCARD_ENABLE

void sdcard_port_init(void);                                                    // SD���ĵ�Դ��, SPI�ڳ�ʼ������
void sdcard_power_on(void);                                                     // ��SD����Դ
void sdcard_power_off(void);                                                    // �ر�SD����Դ

uint8_t sdcard_init(void);                                                      // ��SD����Դ����ʼ��. 0, �ɹ�. 1, ʧ��
uint8_t sdcard_present(void);                                                   // ���SD���Ƿ����. 0, ������. 1, ����
uint8_t sdcard_get_info(sdcard_info_t *info);                                   // ��ȡSD����Ϣ. 0, �ɹ�. 1, ʧ��

uint8_t sdcard_read_blocks(uint8_t* pbuf, uint32_t block, uint32_t block_num);          // SD������
uint8_t sdcard_write_blocks(const uint8_t* pbuf, uint32_t block, uint32_t block_num);   // SD��д��

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
