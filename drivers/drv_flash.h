/**
  ******************************************************************************
  * @file    drv_flash.h
  * @author  mousie-yu
  * @version V2.0.0
  * @date    2015.1.9
  * @brief   STM32 内部 Flash 读写驱动程序.
  *          需要设定 Heap 至少为 FLASH_PAGE_SIZE+0x100.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_FLASH_H
#define __DRV_FLASH_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>





/** @addtogroup Drivers
  * @{
  */
/** @addtogroup DRV_Flash
  * @{
  */



/**
  ******************************************************************************
  * @defgroup DRV_Flash_Configure
  * @brief    用户配置
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // 硬件集中配置文件

/**
  ******************************************************************************
  * STM32 内部Flash可读写区域配置.
  *     FLASH_SIZE, 设置可读写Flash区域大小.
  *         建议使用 0x800, 即 2048 bytes.
  *         此值必须为 STM32 页大小的整数倍.
  *         STM32 LD MD    系列页大小为 0x400
  *         STM32 HD VL XL 系列页大小为 0x800
  *     FLASH_END_ADDRESS, 设置可读写Flash的结束地址.
  *         建议将此值设置为 FLASH 的最大值.
  *         这样可读写区就处于 FLASH 的最末端.
  *     FLASH_START_ADDRESS, 设置可读写Flash的起始地址.
  *         STM32 Flash 的起始地址为 0x8000000
  *         需注意, 代码跳转默认到 0x8000000, 因此修改首页会造成系统错误!
  *         因此, 条件判断 FLASH_START_ADDRESS 的起始值至少为 0x8000800
  *         如果您清楚的了解整个系统的工作原理, 则可以修改为 0x8000000
  ******************************************************************************
  */
//#define FLASH_SIZE                      (0x800 * 1)                             ///< 配置可读写FLASH大小
//#define FLASH_END_ADDRESS               (0x8000000 + 0x40000)                   ///< 配置可读写FLASH结束地址
//#define FLASH_START_ADDRESS             (FLASH_END_ADDRESS - FLASH_SIZE)        ///< 配置可读写FLASH起始地址



#if (FLASH_SIZE > 0)
/// 设置 STM32 FLASH 页大小
#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
    #define FLASH_PAGE_SIZE             0x800
#elif defined (STM32F10X_LD) || (STM32F10X_MD)
    #define FLASH_PAGE_SIZE             0x400
#else
    #error "FLASH_PAGE_SIZE define error!"
#endif

/// STM32 FLASH默认起始地址
#define FLASH_ADDRESS                   0x8000000

#if (FLASH_START_ADDRESS < (FLASH_ADDRESS + FLASH_PAGE_SIZE))
    #error "FLASH_ADDRESS define error!"
#endif

#if ((FLASH_SIZE % FLASH_PAGE_SIZE) > 0)
    #error "FLASH_SIZE define error!"
#endif
#endif
/**
  * @}
  */

/**
  * @}
  */



/** @defgroup DRV_Flash_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_Flash_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_Flash_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_Flash_Public_Function
  * @brief    定义公有函数
  * @{
  */
#if (FLASH_SIZE > 0)

void flash_lock(void);                                                          // 锁定FLASH写入功能
void flash_unlock(void);                                                        // 解锁FLASH写入功能
uint8_t flash_write(uint8_t *pdata, uint32_t addr, uint32_t num);               // 写一串数据, 完成后自动锁定写入功能
uint8_t flash_read(uint8_t *pdata, uint32_t addr, uint32_t num);                // 读一串数据

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
