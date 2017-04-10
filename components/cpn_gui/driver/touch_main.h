/**
  ******************************************************************************
  * @file    touch_main.h
  * @author  mousie-yu
  * @version V2.1.0
  * @date    2013.9.20
  * @brief   LCD ������������. ����LCD��Դ����, ���ȵ���, �������Ժ���.
  *          ���� emwin, ����������!
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TOUCH_MAIN_H
#define __TOUCH_MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "lcdgui_conf.h"



/** @addtogroup Components
  * @{
  */
/** @addtogroup CPN_GUI
  * @{
  */

/** @defgroup TOUCH_MAIN_Public_TypeDefine
  * @brief    �������Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup TOUCH_MAIN_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup TOUCH_MAIN_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */

/**
  * @}
  */

/** @defgroup TOUCH_MAIN_Public_Function
  * @brief    ���幫�к���
  * @{
  */
#if GUI_SUPPORT_TOUCH && (defined LCD_ENABLE)

// ���������ƺ���
void touch_init(void);                                                          // ��������ʼ��, �Ѱ��� touch_start
void touch_stop(void);                                                          // ������ֹͣ����.
void touch_start(void);                                                         // ������ֹͣ������

// ���������Ժ���
void touch_test_driver(void);

#else

#define touch_init()

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
