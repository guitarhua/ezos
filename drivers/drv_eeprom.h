/**
  ******************************************************************************
  * @file    drv_eeprom.h
  * @author  mousie-yu
  * @version V2.0.0
  * @date    2011.12.16
  * @brief   24Cxx系列 EEPROM 驱动程序.
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
  * @brief    用户配置
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // 硬件集中配置文件

/** @defgroup EEPROM_Pin_Assignment
  * @brief    EEPROM引脚对应关系, 如下图:
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
//#define EEPROM_I2C_NAME                 EEPROM_I2C_HW                           ///< 该驱动需使用一个I2C, I2C的SCL, SDA. 在"drv_i2c.h"中配置
//#define EEPROM_TYPE_24C                 2                                       ///< 设定EEPROM芯片型号. 1表示24C01, 64表示24C64/24C65, 1024表示24C1024, 类推
/**
  * @}
  */

/**
  * @}
  */



/** @defgroup DRV_EEPROM_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_EEPROM_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_EEPROM_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_EEPROM_Public_Function
  * @brief    定义公有函数
  * @{
  */
#if (EEPROM_TYPE_24C > 0)

void eeprom_init(uint8_t a2a1a0);                                               // EEPROM初始化函数. 并设置默认地址, 即引脚A2A1A0的值.
void eeprom_set_chip_addr(uint8_t a2a1a0);                                      // 重设EEPROM地址, 可实现同一总线上读写多个EEPROM

uint8_t eeprom_read(uint8_t *pdata, uint32_t addr, uint32_t num);               // 读一串数据
uint8_t eeprom_write(uint8_t *pdata, uint32_t addr, uint32_t num);              // 写一串数据

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
