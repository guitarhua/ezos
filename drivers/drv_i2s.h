/**
  ******************************************************************************
  * @file    drv_i2s.h
  * @author  mousie
  * @version V1.0.0
  * @date    2012.3.1
  * @brief   ʹ����ģʽ, �жϷ�ʽ�� I2S ��������, ��������������ģʽ.
  *          ���� STM32F10x ϵ�е� I2S2 ʹ��. ������I2S�ӿڵ���Ƶ���.
  *          Ŀǰ�� I2S_DataFormat_8b ��ʽ����������, ԭ����.
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
  * @brief    �û�����
  ******************************************************************************
  * @{
  */
#define I2S_ENABLE
/**
  * @}
  */



/** @defgroup DRV_I2S_Public_TypeDefine
  * @brief    �������Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_I2S_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */
#define I2S_DataFormat_8b               ((uint16_t)0xFFFF)                      // �Զ�������ݸ�ʽ
/**
  * @}
  */

/** @defgroup DRV_I2S_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_I2S_Public_Function
  * @brief    ���幫�к���
  * @{
  */
#ifdef I2S_ENABLE

/**
  ******************************************************************************
  * @brief  I2S��ʼ��, ��ʼ��������, ��ʹ�ܸ�I2S
  * @param  data_format, ���� I2S ���ݸ�ʽ
  *   @arg  I2S_DataFormat_8b,          8λ���ݿ��,  ����16λ���ݿ�ȷ���
  *   @arg  I2S_DataFormat_16b,         16λ���ݿ��, ����16λ���ݿ�ȷ���
  *   @arg  I2S_DataFormat_16bextended, 16λ���ݿ����չΪ32λ���ݿ�ȷ���
  *   @arg  I2S_DataFormat_24b,         24λ���ݿ����չΪ32λ���ݿ�ȷ���
  *   @arg  I2S_DataFormat_32b,         32λ���ݿ��
  * @param  audio_freq, ������Ƶ�ļ�������
  *   @arg  I2S_AudioFreq_8k,   8K������
  *   @arg  I2S_AudioFreq_11k,  11K������
  *   @arg  I2S_AudioFreq_16k,  16K������
  *   @arg  I2S_AudioFreq_22k,  22K������
  *   @arg  I2S_AudioFreq_32k,  32K������
  *   @arg  I2S_AudioFreq_44k,  44K������
  *   @arg  I2S_AudioFreq_48k,  48K������
  *   @arg  I2S_AudioFreq_96k,  96K������
  *   @arg  I2S_AudioFreq_192k, 192K������
  * @param  audio_track, ������Ƶ�ļ�����
  *   @arg  0,  ������
  *   @arg  !0, ˫����
  * @retval none
  ******************************************************************************
  */
void i2s_init(uint16_t data_format, uint32_t audio_freq, uint8_t audio_track);
void i2s_deinit(void);                                                              // ��ֹI2S����, �ɽڵ�
void i2s_buf_clear(void);                                                           // ���I2S���ͻ�����

void i2s_tx(uint8_t *pdata, uint16_t length);                                       // ͨ��I2S����һ��С�˴洢������

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
