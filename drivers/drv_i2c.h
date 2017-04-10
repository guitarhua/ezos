/**
  ******************************************************************************
  * @file    drv_i2c.c
  * @author  mousie
  * @version V2.0.0
  * @date    2011.12.16
  * @brief   �ṩ I2C ��������. ����ģʽ, 7λ��ַ. ͬʱ֧��IO�����ģ���Ӳ��I2C.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_I2C_H
#define __DRV_I2C_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_i2c.h"



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup DRV_I2C
  * @{
  */



/**
  ******************************************************************************
  * @defgroup DRV_I2C_Configure
  * @brief    �û�����
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // Ӳ�����������ļ�

//#define I2C_NUM                         5                                       ///< ����ʹ�õ�I2C����.�������������5��I2C��
//
//#if (I2C_NUM > 0)
//    /**
//      * @brief ���ø�I2C����������
//      *   @arg 0,                       ʹ��IO��ģ��SPI
//      *   @arg I2C1,                    ʹ��I2C1����
//      *   @arg I2C2,                    ʹ��I2C2����
//      */
//    #define I2C0_PERIPHERAL             I2C1                                    ///< ���ø�I2C��ģ����,   ��������ģ��I2C, ��Ϊ0
//    #define I2C0_CLK                    RCC_APB1Periph_I2C1                     ///< ���ø�I2C��ʱ��ģ��, ��������ģ��I2C, ��Ϊ0
//
//    #define I2C0_SCL_PORT               GPIOB                                   ///< ���ø�I2C��SCL���ŵ�PORT��
//    #define I2C0_SCL_CLK                RCC_APB2Periph_GPIOB                    ///< ���ø�I2C��SCL���ŵ�ʱ��ģ��
//    #define I2C0_SCL_PIN                GPIO_Pin_6                              ///< ���ø�I2C��SCL���ŵ�PIN��
//
//    #define I2C0_SDA_PORT               GPIOB                                   ///< ���ø�I2C��SDA���ŵ�PORT��
//    #define I2C0_SDA_CLK                RCC_APB2Periph_GPIOB                    ///< ���ø�I2C��SDA���ŵ�ʱ��ģ��
//    #define I2C0_SDA_PIN                GPIO_Pin_7                              ///< ���ø�I2C��SDA���ŵ�PIN��
//
//    /**
//      * @brief ����I2C������ӳ�书��. ������Ϣ��ο��ٷ������ֲ�.
//      *        ��I2C��ص���������:
//      *   @arg 0,                       ��ʹ��������ӳ�书��
//      *   @arg GPIO_Remap_I2C1,         I2C1 ������ӳ��
//      */
//    #define I2C0_GPIO_REMAP             0
//#endif
//
//#if (I2C_NUM > 1)
//    #define I2C1_PERIPHERAL             I2C2
//    #define I2C1_CLK                    RCC_APB1Periph_I2C2
//
//    #define I2C1_SCL_PORT               GPIOB
//    #define I2C1_SCL_CLK                RCC_APB2Periph_GPIOB
//    #define I2C1_SCL_PIN                GPIO_Pin_10
//
//    #define I2C1_SDA_PORT               GPIOB
//    #define I2C1_SDA_CLK                RCC_APB2Periph_GPIOB
//    #define I2C1_SDA_PIN                GPIO_Pin_11
//
//    #define I2C1_GPIO_REMAP             0
//#endif
//
//#if (I2C_NUM > 2)
//    #define I2C2_PERIPHERAL             0
//    #define I2C2_CLK                    0
//
//    #define I2C2_SCL_PORT               GPIOB
//    #define I2C2_SCL_CLK                RCC_APB2Periph_GPIOB
//    #define I2C2_SCL_PIN                GPIO_Pin_6
//
//    #define I2C2_SDA_PORT               GPIOB
//    #define I2C2_SDA_CLK                RCC_APB2Periph_GPIOB
//    #define I2C2_SDA_PIN                GPIO_Pin_7
//
//    #define I2C2_GPIO_REMAP             0
//#endif
//
//#if (I2C_NUM > 3)
//    #define I2C3_PERIPHERAL             0
//    #define I2C3_CLK                    0
//
//    #define I2C3_SCL_PORT               GPIOB
//    #define I2C3_SCL_CLK                RCC_APB2Periph_GPIOB
//    #define I2C3_SCL_PIN                GPIO_Pin_6
//
//    #define I2C3_SDA_PORT               GPIOB
//    #define I2C3_SDA_CLK                RCC_APB2Periph_GPIOB
//    #define I2C3_SDA_PIN                GPIO_Pin_7
//
//    #define I2C3_GPIO_REMAP             0
//#endif
//
//#if (I2C_NUM > 4)
//    #define I2C4_PERIPHERAL             0
//    #define I2C4_CLK                    0
//
//    #define I2C4_SCL_PORT               GPIOB
//    #define I2C4_SCL_CLK                RCC_APB2Periph_GPIOB
//    #define I2C4_SCL_PIN                GPIO_Pin_6
//
//    #define I2C4_SDA_PORT               GPIOB
//    #define I2C4_SDA_CLK                RCC_APB2Periph_GPIOB
//    #define I2C4_SDA_PIN                GPIO_Pin_7
//
//    #define I2C4_GPIO_REMAP             0
//#endif
//
///// I2C�������Ͷ���
//typedef enum
//{
//    EEPROM_I2C = 0,                                                             ///< ����Ϊ��������I2C����, ֵ0��ӦI2C0_xxx
//    USER1_I2C,                                                                  ///< ֵ1��ӦI2C1_xxx, ��������
//    USER2_I2C,
//    USER3_I2C,
//    USER4_I2C,
//} i2c_name_t;



#if (I2C_NUM > 5)
    #error "I2C_NUM define error!"
#endif
/**
  * @}
  */



/** @defgroup DRV_I2C_Public_TypeDefine
  * @brief    �������Ͷ���
  * @{
  */
/// I2CӦ�����Ͷ���
typedef enum
{
    I2C_ACK    = 0,                                                             ///< I2C ��Ӧ���ź�
    I2C_NO_ACK = 1,                                                             ///< I2C ��Ӧ���ź�
} i2c_ack_t;
/**
  * @}
  */

/** @defgroup DRV_I2C_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_I2C_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_I2C_Public_Function
  * @brief    ���幫�к���
  * @{
  */
#if (I2C_NUM > 0)

void i2c_init(i2c_name_t i2c_name, uint16_t baud_rate_khz);                     // I2C��ʼ��
void i2c_deinit(i2c_name_t i2c_name);                                           // ����I2C
void i2c_start(i2c_name_t i2c_name);                                            // ����I2C��ʼ�ź�
void i2c_stop(i2c_name_t i2c_name);                                             // ����I2C�����ź�
i2c_ack_t i2c_tx_addr_wr_slaver(i2c_name_t i2c_name, uint8_t addr);             // I2C׼��д���豸
i2c_ack_t i2c_tx_addr_rd_slaver(i2c_name_t i2c_name, uint8_t addr);             // I2C׼�������豸
i2c_ack_t i2c_tx_byte(i2c_name_t i2c_name, uint8_t data);                       // ͨ��I2C����һ���ֽ�
uint8_t i2c_rx_byte(i2c_name_t i2c_name, i2c_ack_t ack);                        // ͨ��I2C����һ���ֽ�

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
