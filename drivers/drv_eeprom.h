/**
  ******************************************************************************
  * @file    drv_eeprom.h
  * @author  mousie-yu
  * @version V2.0.0
  * @date    2011.12.16
  * @brief   24Cxxϵ�� EEPROM ��������.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_EEPROM_H
#define __DRV_EEPROM_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#include "drv_i2c.h"



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup DRV_EEPROM
  * @{
  */



/**
  ******************************************************************************
  * @defgroup DRV_EEPROM_Configure
  * @brief    �û�����
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // Ӳ�����������ļ�

/** @defgroup EEPROM_Pin_Assignment
  * @brief    EEPROM���Ŷ�Ӧ��ϵ, ����ͼ:
  *           +---------------------------------------------+
  *           |                Pin assignment               |
  *           +-------------------+-----------+-------------+
  *           | STM32 I2C Pins    |   EEPROM  |   Pin       |
  *           +-------------------+-----------+-------------+
  *           | .                 |   E0(GND) |    1  (0V)  |
  *           | .                 |   E1(GND) |    2  (0V)  |
  *           | .                 |   E2(GND) |    3  (0V)  |
  *           | .                 |   E0(VSS) |    4  (0V)  |
  *           | EEPROM_I2C_SDA    |   SDA     |    5        |
  *           | EEPROM_I2C_SCL    |   SCL     |    6        |
  *           | .                 |   /WC(VDD)|    7 (3.3V) |
  *           | .                 |   VDD     |    8 (3.3V) |
  *           +-------------------+-----------+-------------+
  * @{
  */
//#define EEPROM_I2C_NAME                 EEPROM_I2C_HW                           ///< ��������ʹ��һ��I2C, I2C��SCL, SDA. ��"drv_i2c.h"������
//#define EEPROM_TYPE_24C                 2                                       ///< �趨EEPROMоƬ�ͺ�. 1��ʾ24C01, 64��ʾ24C64/24C65, 1024��ʾ24C1024, ����
/**
  * @}
  */

/**
  * @}
  */



/** @defgroup DRV_EEPROM_Public_TypeDefine
  * @brief    �������Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_EEPROM_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_EEPROM_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_EEPROM_Public_Function
  * @brief    ���幫�к���
  * @{
  */
#if (EEPROM_TYPE_24C > 0)

void eeprom_init(uint8_t a2a1a0);                                               // EEPROM��ʼ������. ������Ĭ�ϵ�ַ, ������A2A1A0��ֵ.
void eeprom_set_chip_addr(uint8_t a2a1a0);                                      // ����EEPROM��ַ, ��ʵ��ͬһ�����϶�д���EEPROM

uint8_t eeprom_read(uint8_t *pdata, uint32_t addr, uint32_t num);               // ��һ������
uint8_t eeprom_write(uint8_t *pdata, uint32_t addr, uint32_t num);              // дһ������

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
