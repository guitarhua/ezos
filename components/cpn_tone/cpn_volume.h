/**
  ******************************************************************************
  * @file    cpn_volume.h
  * @author  mousie
  * @version V1.0.0
  * @date    2014.8.6
  * @brief   简易的铃声音量控制. 待测试.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CPN_VOLUME_H
#define __CPN_VOLUME_H

/* Includes ------------------------------------------------------------------*/
#include "drv_ctrlport.h"



/** @addtogroup Components
  * @{
  */
/** @addtogroup CPN_Volume
  * @{
  */



/**
  ******************************************************************************
  * @defgroup CPN_Volume_Configure
  * @brief    用户配置
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // 硬件集中配置文件

//#define VOLUME_ENABLE                   1                                     // 设置振铃音量. 1-2级音量, 2-4级音量, 3-8级音量, 4-16级音量
//#define VOLUME_IO0
//#define VOLUME_IO1
//#define VOLUME_IO2
//#define VOLUME_IO3



#if ((VOLUME_ENABLE==0) || (VOLUME_ENABLE > 4))
    #error "VOLUME_ENABLE define error!"
#endif
/**
  * @}
  */



/** @defgroup CPN_Volume_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup CPN_Volume_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup CPN_Volume_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup CPN_Volume_Public_Function
  * @brief    定义公有函数
  * @{
  */
#ifdef VOLUME_ENABLE

void volume_init(void);
void volume_set(uint8_t volume);
uint8_t volume_get(void);

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
