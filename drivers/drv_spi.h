/**
  ******************************************************************************
  * @file    drv_spi.h
  * @author  mousie
  * @version V2.0.0
  * @date    2011.12.15
  * @brief   �ṩ SPI ��������. ����ģʽ. ͬʱ֧��IO�����ģ���Ӳ��SPI.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_SPI_H
#define __DRV_SPI_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup DRV_SPI
  * @{
  */



/**
  ******************************************************************************
  * @defgroup DRV_SPI_Configure
  * @brief    �û�����
  * @note     ������������SPI��SS����, SS�����ɵ���SPI�ĺ�������
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // Ӳ�����������ļ�

//#define SPI_NUM                         5                                       ///< ����ʹ�õ�SPI����.�������������5��SPI��
//
//#if (SPI_NUM > 0)
//    /**
//      * @brief ���ø�SPI����������
//      *   @arg 0,                       ʹ��IO��ģ��SPI
//      *   @arg SPI1,                    ʹ��SPI1����, SPI1��CLKΪ RCC_APB2Periph_SPI1
//      *   @arg SPI2,                    ʹ��SPI2����, �����CLKΪ RCC_APB1Periph_XXX
//      *   @arg SPI3,                    ʹ��SPI3����
//      */
//    #define SPI0_PERIPHERAL             SPI1                                    ///< ���ø�SPI��оƬ������, ��������ģ��SPI, ��Ϊ0
//    #define SPI0_CLK                    RCC_APB2Periph_SPI1                     ///< ���ø�SPI��ʱ��ģ��,   ��������ģ��SPI, ��Ϊ0
//
//    #define SPI0_SCK_PORT               GPIOA                                   ///< ���ø�SPI��SCK����PORT��
//    #define SPI0_SCK_CLK                RCC_APB2Periph_GPIOA                    ///< ���ø�SPI��SCK����ʱ��ģ��
//    #define SPI0_SCK_PIN                GPIO_Pin_5                              ///< ���ø�SPI��SCK����PIN��
//
//    #define SPI0_MISO_PORT              GPIOA                                   ///< ���ø�SPI��MISO����PORT��
//    #define SPI0_MISO_CLK               RCC_APB2Periph_GPIOA                    ///< ���ø�SPI��MISO����ʱ��ģ��
//    #define SPI0_MISO_PIN               GPIO_Pin_6                              ///< ���ø�SPI��MISO����PIN��
//
//    #define SPI0_MOSI_PORT              GPIOA                                   ///< ���ø�SPI��MOSI����PORT��
//    #define SPI0_MOSI_CLK               RCC_APB2Periph_GPIOA                    ///< ���ø�SPI��MOSI����ʱ��ģ��
//    #define SPI0_MOSI_PIN               GPIO_Pin_7                              ///< ���ø�SPI��MOSI����PIN��
//
//    /**
//      * @brief ���ø�SPI��������ӳ�书��. ��ʹ�õ�IO���й�.
//      *   @arg 0,                       ��ʹ��������ӳ�书��
//      *   @arg GPIO_Remap_SPI1,         SPI1 ������ӳ��
//      *   @arg GPIO_Remap_SPI3,         SPI3 ������ӳ��
//      */
//    #define SPI0_GPIO_REMAP             0
//#endif
//
//#if (SPI_NUM > 1)
//    #define SPI1_PERIPHERAL             SPI2
//    #define SPI1_CLK                    RCC_APB1Periph_SPI2
//
//    #define SPI1_SCK_PORT               GPIOB
//    #define SPI1_SCK_CLK                RCC_APB2Periph_GPIOB
//    #define SPI1_SCK_PIN                GPIO_Pin_13
//
//    #define SPI1_MISO_PORT              GPIOB
//    #define SPI1_MISO_CLK               RCC_APB2Periph_GPIOB
//    #define SPI1_MISO_PIN               GPIO_Pin_14
//
//    #define SPI1_MOSI_PORT              GPIOB
//    #define SPI1_MOSI_CLK               RCC_APB2Periph_GPIOB
//    #define SPI1_MOSI_PIN               GPIO_Pin_15
//
//    #define SPI1_GPIO_REMAP             0
//#endif
//
//#if (SPI_NUM > 2)
//    #define SPI2_PERIPHERAL             SPI2
//    #define SPI2_CLK                    RCC_APB1Periph_SPI2
//
//    #define SPI2_SCK_PORT               GPIOB
//    #define SPI2_SCK_CLK                RCC_APB2Periph_GPIOB
//    #define SPI2_SCK_PIN                GPIO_Pin_13
//
//    #define SPI2_MISO_PORT              GPIOB
//    #define SPI2_MISO_CLK               RCC_APB2Periph_GPIOB
//    #define SPI2_MISO_PIN               GPIO_Pin_14
//
//    #define SPI2_MOSI_PORT              GPIOB
//    #define SPI2_MOSI_CLK               RCC_APB2Periph_GPIOB
//    #define SPI2_MOSI_PIN               GPIO_Pin_15
//
//    #define SPI2_GPIO_REMAP             0
//#endif
//
//#if (SPI_NUM > 3)
//    #define SPI3_PERIPHERAL             SPI2
//    #define SPI3_CLK                    RCC_APB1Periph_SPI2
//
//    #define SPI3_SCK_PORT               GPIOB
//    #define SPI3_SCK_CLK                RCC_APB2Periph_GPIOB
//    #define SPI3_SCK_PIN                GPIO_Pin_13
//
//    #define SPI3_MISO_PORT              GPIOB
//    #define SPI3_MISO_CLK               RCC_APB2Periph_GPIOB
//    #define SPI3_MISO_PIN               GPIO_Pin_14
//
//    #define SPI3_MOSI_PORT              GPIOB
//    #define SPI3_MOSI_CLK               RCC_APB2Periph_GPIOB
//    #define SPI3_MOSI_PIN               GPIO_Pin_15
//
//    #define SPI3_GPIO_REMAP             0
//#endif
//
//#if (SPI_NUM > 4)
//    #define SPI4_PERIPHERAL             0
//    #define SPI4_CLK                    0
//
//    #define SPI4_SCK_PORT               GPIOB
//    #define SPI4_SCK_CLK                RCC_APB2Periph_GPIOB
//    #define SPI4_SCK_PIN                GPIO_Pin_13
//
//    #define SPI4_MISO_PORT              GPIOB
//    #define SPI4_MISO_CLK               RCC_APB2Periph_GPIOB
//    #define SPI4_MISO_PIN               GPIO_Pin_14
//
//    #define SPI4_MOSI_PORT              GPIOB
//    #define SPI4_MOSI_CLK               RCC_APB2Periph_GPIOB
//    #define SPI4_MOSI_PIN               GPIO_Pin_15
//
//    #define SPI4_GPIO_REMAP             0
//#endif
//
///// SPI�������Ͷ���
//typedef enum
//{
//    SD_SPI     = 0,                                                             ///< ����Ϊ��������SPI����, ֵ0��ӦSPI0_xxx
//    SFLASH_SPI,                                                                 ///< ֵ1��ӦSPI1_xxx, ��������
//    USER0_SPI,
//    USER1_SPI,
//    USER2_SPI,
//} spi_name_t;



#if (SPI_NUM > 5)
    #error "SPI_NUM define error!"
#endif
/**
  * @}
  */



/** @defgroup DRV_SPI_Public_TypeDefine
  * @brief    �������Ͷ���
  * @{
  */

/**
  * @brief  SPI�Ĵ������Ͷ���
  *
  *   @arg  SPI_TYPE_HIGH_EDGE1_MSB, SCKĬ�ϸߵ�ƽ, �½��ؼ��, MSBģʽ
  *         SCK��       ----��__��--��__��--��__��--��__��--��__��--��__��--��__��--��__��----
  *         MOSI / MISO --BIT7---XXX---XXX---XXX---XXX---XXX---XXX---BIT0-----
  *
  *   @arg  SPI_TYPE_HIGH_EDGE1_LSB, SCKĬ�ϸߵ�ƽ, �½��ؼ��, LSBģʽ
  *         SCK��       ----��__��--��__��--��__��--��__��--��__��--��__��--��__��--��__��----
  *         MOSI / MISO --BIT0---XXX---XXX---XXX---XXX---XXX---XXX---BIT7-----
  *
  *   @arg  SPI_TYPE_HIGH_EDGE2_MSB, SCKĬ�ϸߵ�ƽ, �����ؼ��, MSBģʽ
  *         SCK��       ----��__��--��__��--��__��--��__��--��__��--��__��--��__��--��__��----
  *         MOSI / MISO -----BIT7---XXX---XXX---XXX---XXX---XXX---XXX---BIT0--
  *
  *   @arg  SPI_TYPE_HIGH_EDGE2_LSB, SCKĬ�ϸߵ�ƽ, �����ؼ��, LSBģʽ
  *         SCK��       ----��__��--��__��--��__��--��__��--��__��--��__��--��__��--��__��----
  *         MOSI / MISO -----BIT0---XXX---XXX---XXX---XXX---XXX---XXX---BIT7--
  *
  *   @arg  SPI_TYPE_LOW_EDGE1_MSB,  SCKĬ�ϵ͵�ƽ, �����ؼ��, MSBģʽ
  *         SCK��       ____��--��__��--��__��--��__��--��__��--��__��--��__��--��__��--��____
  *         MOSI / MISO --BIT7---XXX---XXX---XXX---XXX---XXX---XXX---BIT0-----
  *
  *   @arg  SPI_TYPE_LOW_EDGE1_LSB,  SCKĬ�ϵ͵�ƽ, �����ؼ��, LSBģʽ
  *         SCK��       ____��--��__��--��__��--��__��--��__��--��__��--��__��--��__��--��____
  *         MOSI / MISO --BIT0---XXX---XXX---XXX---XXX---XXX---XXX---BIT7-----
  *
  *   @arg  SPI_TYPE_LOW_EDGE2_MSB,  SCKĬ�ϵ͵�ƽ, �½��ؼ��, MSBģʽ
  *         SCK��       ____��--��__��--��__��--��__��--��__��--��__��--��__��--��__��--��____
  *         MOSI / MISO -----BIT7---XXX---XXX---XXX---XXX---XXX---XXX---BIT0--
  *
  *   @arg  SPI_TYPE_LOW_EDGE2_LSB,  SCKĬ�ϵ͵�ƽ, �½��ؼ��, LSBģʽ
  *         SCK��       ____��--��__��--��__��--��__��--��__��--��__��--��__��--��__��--��____
  *         MOSI / MISO -----BIT0---XXX---XXX---XXX---XXX---XXX---XXX---BIT7--
  */
typedef enum
{
    SPI_TYPE_HIGH_EDGE1_MSB = (0x80 | 0x04 | 0x00 | 0x00),
    SPI_TYPE_HIGH_EDGE1_LSB = (0x80 | 0x04 | 0x00 | 0x01),
    SPI_TYPE_HIGH_EDGE2_MSB = (0x80 | 0x04 | 0x02 | 0x00),
    SPI_TYPE_HIGH_EDGE2_LSB = (0x80 | 0x04 | 0x02 | 0x01),
    SPI_TYPE_LOW_EDGE1_MSB  = (0x80 | 0x00 | 0x00 | 0x00),
    SPI_TYPE_LOW_EDGE1_LSB  = (0x80 | 0x00 | 0x00 | 0x01),
    SPI_TYPE_LOW_EDGE2_MSB  = (0x80 | 0x00 | 0x02 | 0x00),
    SPI_TYPE_LOW_EDGE2_LSB  = (0x80 | 0x00 | 0x02 | 0x01),
} spi_type_t;

#define IS_SPI_TYPE(spi_type)         ( ((spi_type) == SPI_TYPE_HIGH_EDGE1_MSB) || \
                                        ((spi_type) == SPI_TYPE_HIGH_EDGE1_LSB) || \
                                        ((spi_type) == SPI_TYPE_HIGH_EDGE2_MSB) || \
                                        ((spi_type) == SPI_TYPE_HIGH_EDGE2_LSB) || \
                                        ((spi_type) == SPI_TYPE_LOW_EDGE1_MSB ) || \
                                        ((spi_type) == SPI_TYPE_LOW_EDGE1_LSB ) || \
                                        ((spi_type) == SPI_TYPE_LOW_EDGE2_MSB ) || \
                                        ((spi_type) == SPI_TYPE_LOW_EDGE2_LSB ) )
/**
  * @}
  */

/** @defgroup DRV_SPI_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_SPI_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_SPI_Public_Function
  * @brief    ���幫�к���
  * @{
  */
#if (SPI_NUM > 0)

/**
  ******************************************************************************
  * @brief  SPI��ʼ������, �����ڳ�ʼ����ɺ�������SPI��SS����
  * @param  spi_name
  *   @arg  ����ֵ��ο� spi_name_t ���Ͷ����е�ö��ֵ
  * @param  spi_type, ����SPI�Ĵ�������
  *   @arg  SPI_TYPE_HIGH_EDGE1_MSB,    SCKĬ�ϸߵ�ƽ, �½��ؼ��, MSBģʽ
  *   @arg  SPI_TYPE_HIGH_EDGE1_LSB,    SCKĬ�ϸߵ�ƽ, �½��ؼ��, LSBģʽ
  *   @arg  SPI_TYPE_HIGH_EDGE2_MSB,    SCKĬ�ϸߵ�ƽ, �����ؼ��, MSBģʽ
  *   @arg  SPI_TYPE_HIGH_EDGE2_LSB,    SCKĬ�ϸߵ�ƽ, �����ؼ��, LSBģʽ
  *   @arg  SPI_TYPE_LOW_EDGE1_MSB,     SCKĬ�ϵ͵�ƽ, �����ؼ��, MSBģʽ
  *   @arg  SPI_TYPE_LOW_EDGE1_LSB,     SCKĬ�ϵ͵�ƽ, �����ؼ��, LSBģʽ
  *   @arg  SPI_TYPE_LOW_EDGE2_MSB,     SCKĬ�ϵ͵�ƽ, �½��ؼ��, MSBģʽ
  *   @arg  SPI_TYPE_LOW_EDGE2_LSB,     SCKĬ�ϵ͵�ƽ, �½��ؼ��, LSBģʽ
  * @param  baud_rate, ����SPI������.
  *   ����Ӳ��SPI, SPI������ͨ����APB����Ԥ��Ƶ����, ��Чֵ����:
  *   @arg   SPI_BaudRatePrescaler_2,   ��APB����2��Ƶ����ΪSPIʱ���ź�
  *   @arg   SPI_BaudRatePrescaler_4,   ��APB����4��Ƶ����ΪSPIʱ���ź�
  *   @arg   SPI_BaudRatePrescaler_8,   ��������
  *   @arg   SPI_BaudRatePrescaler_16
  *   @arg   SPI_BaudRatePrescaler_32
  *   @arg   SPI_BaudRatePrescaler_64
  *   @arg   SPI_BaudRatePrescaler_128
  *   @arg   SPI_BaudRatePrescaler_256
  *   �������ģ��SPI, ֱ�������䲨����ֵ, ��λΪKhz.
  *   @arg   1-500,                      ���ģ��SPI��������Чֵ 1-500 Khz.
  *   @arg   0,                          ��ʾ����ʱ. �ɴ�Լ2MHz
  * @retval none
  ******************************************************************************
  */
void spi_init(spi_name_t spi_name, spi_type_t spi_type, uint16_t baud_rate);
void spi_deinit(spi_name_t spi_name);                                           // SPI���ú���
uint8_t spi_tx_rx_byte(spi_name_t spi_name, uint8_t data);                      // SPI�շ�һ���ֽ�

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
