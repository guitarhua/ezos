/**
  ******************************************************************************
  * @file    cpn_volume.h
  * @author  mousie
  * @version V1.0.0
  * @date    2014.8.6
  * @brief   ���׵�������������. ������.
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
  * @brief    �û�����
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // Ӳ�����������ļ�

//#define VOLUME_ENABLE                   1                                     // ������������. 1-2������, 2-4������, 3-8������, 4-16������
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
  * @brief    �������Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup CPN_Volume_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup CPN_Volume_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */

/**
  * @}
  */

/** @defgroup CPN_Volume_Public_Function
  * @brief    ���幫�к���
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
