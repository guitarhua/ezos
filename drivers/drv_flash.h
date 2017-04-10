/**
  ******************************************************************************
  * @file    drv_flash.h
  * @author  mousie-yu
  * @version V2.0.0
  * @date    2015.1.9
  * @brief   STM32 �ڲ� Flash ��д��������.
  *          ��Ҫ�趨 Heap ����Ϊ FLASH_PAGE_SIZE+0x100.
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
  * @brief    �û�����
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // Ӳ�����������ļ�

/**
  ******************************************************************************
  * STM32 �ڲ�Flash�ɶ�д��������.
  *     FLASH_SIZE, ���ÿɶ�дFlash�����С.
  *         ����ʹ�� 0x800, �� 2048 bytes.
  *         ��ֵ����Ϊ STM32 ҳ��С��������.
  *         STM32 LD MD    ϵ��ҳ��СΪ 0x400
  *         STM32 HD VL XL ϵ��ҳ��СΪ 0x800
  *     FLASH_END_ADDRESS, ���ÿɶ�дFlash�Ľ�����ַ.
  *         ���齫��ֵ����Ϊ FLASH �����ֵ.
  *         �����ɶ�д���ʹ��� FLASH ����ĩ��.
  *     FLASH_START_ADDRESS, ���ÿɶ�дFlash����ʼ��ַ.
  *         STM32 Flash ����ʼ��ַΪ 0x8000000
  *         ��ע��, ������תĬ�ϵ� 0x8000000, ����޸���ҳ�����ϵͳ����!
  *         ���, �����ж� FLASH_START_ADDRESS ����ʼֵ����Ϊ 0x8000800
  *         �����������˽�����ϵͳ�Ĺ���ԭ��, ������޸�Ϊ 0x8000000
  ******************************************************************************
  */
//#define FLASH_SIZE                      (0x800 * 1)                             ///< ���ÿɶ�дFLASH��С
//#define FLASH_END_ADDRESS               (0x8000000 + 0x40000)                   ///< ���ÿɶ�дFLASH������ַ
//#define FLASH_START_ADDRESS             (FLASH_END_ADDRESS - FLASH_SIZE)        ///< ���ÿɶ�дFLASH��ʼ��ַ



#if (FLASH_SIZE > 0)
/// ���� STM32 FLASH ҳ��С
#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
    #define FLASH_PAGE_SIZE             0x800
#elif defined (STM32F10X_LD) || (STM32F10X_MD)
    #define FLASH_PAGE_SIZE             0x400
#else
    #error "FLASH_PAGE_SIZE define error!"
#endif

/// STM32 FLASHĬ����ʼ��ַ
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
  * @brief    �������Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_Flash_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_Flash_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_Flash_Public_Function
  * @brief    ���幫�к���
  * @{
  */
#if (FLASH_SIZE > 0)

void flash_lock(void);                                                          // ����FLASHд�빦��
void flash_unlock(void);                                                        // ����FLASHд�빦��
uint8_t flash_write(uint8_t *pdata, uint32_t addr, uint32_t num);               // дһ������, ��ɺ��Զ�����д�빦��
uint8_t flash_read(uint8_t *pdata, uint32_t addr, uint32_t num);                // ��һ������

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
