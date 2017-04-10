/**
  ******************************************************************************
  * @file    drv_i2s.h
  * @author  mousie
  * @version V1.0.0
  * @date    2012.3.1
  * @brief   使用主模式, 中断方式的 I2S 驱动程序, 工作于主机发送模式.
  *          仅限 STM32F10x 系列的 I2S2 使用. 可用于I2S接口的音频输出.
  *          目前仅 I2S_DataFormat_8b 格式可正常播放, 原因不明.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_I2S_H
#define __DRV_I2S_H

/* Include -------------------------------------------------------------------*/
#include <stdint.h>

#include "misc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_spi.h"



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup DRV_I2S
  * @{
  */
/**
  ******************************************************************************
  * @defgroup DRV_I2S_Configure
  * @brief    用户配置
  ******************************************************************************
  * @{
  */
#define I2S_ENABLE
/**
  * @}
  */



/** @defgroup DRV_I2S_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_I2S_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */
#define I2S_DataFormat_8b               ((uint16_t)0xFFFF)                      // 自定义的数据格式
/**
  * @}
  */

/** @defgroup DRV_I2S_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_I2S_Public_Function
  * @brief    定义公有函数
  * @{
  */
#ifdef I2S_ENABLE

/**
  ******************************************************************************
  * @brief  I2S初始化, 初始化缓存区, 并使能该I2S
  * @param  data_format, 设置 I2S 数据格式
  *   @arg  I2S_DataFormat_8b,          8位数据宽度,  采用16位数据宽度发送
  *   @arg  I2S_DataFormat_16b,         16位数据宽度, 采用16位数据宽度发送
  *   @arg  I2S_DataFormat_16bextended, 16位数据宽度扩展为32位数据宽度发送
  *   @arg  I2S_DataFormat_24b,         24位数据宽度扩展为32位数据宽度发送
  *   @arg  I2S_DataFormat_32b,         32位数据宽度
  * @param  audio_freq, 设置音频文件采样率
  *   @arg  I2S_AudioFreq_8k,   8K采样率
  *   @arg  I2S_AudioFreq_11k,  11K采样率
  *   @arg  I2S_AudioFreq_16k,  16K采样率
  *   @arg  I2S_AudioFreq_22k,  22K采样率
  *   @arg  I2S_AudioFreq_32k,  32K采样率
  *   @arg  I2S_AudioFreq_44k,  44K采样率
  *   @arg  I2S_AudioFreq_48k,  48K采样率
  *   @arg  I2S_AudioFreq_96k,  96K采样率
  *   @arg  I2S_AudioFreq_192k, 192K采样率
  * @param  audio_track, 设置音频文件声道
  *   @arg  0,  单声道
  *   @arg  !0, 双声道
  * @retval none
  ******************************************************************************
  */
void i2s_init(uint16_t data_format, uint32_t audio_freq, uint8_t audio_track);
void i2s_deinit(void);                                                              // 禁止I2S功能, 可节电
void i2s_buf_clear(void);                                                           // 清空I2S发送缓存区

void i2s_tx(uint8_t *pdata, uint16_t length);                                       // 通过I2S发送一串小端存储的数据

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
