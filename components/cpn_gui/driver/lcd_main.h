/**
  ******************************************************************************
  * @file    lcd_main.h
  * @author  mousie-yu
  * @version V2.1.0
  * @date    2013.9.20
  * @brief   LCD ������������. ����LCD��Դ����, ���ȵ���, �������Ժ���.
  *          ���� emwin, ����������!
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_MAIN_H
#define __LCD_MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "lcdgui_conf.h"



/** @addtogroup Components
  * @{
  */
/** @addtogroup LCD_MAIN
  * @{
  */

/** @defgroup LCD_MAIN_Public_TypeDefine
  * @brief    �������Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup LCD_MAIN_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup LCD_MAIN_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */

/**
  * @}
  */

/** @defgroup LCD_MAIN_Public_Function
  * @brief    ���幫�к���
  * @{
  */
#ifdef LCD_ENABLE

// LCD���ƺ���
void lcd_init(void);                                                            // LCD IO�ڳ�ʼ��
void lcd_power_on(void);                                                        // �򿪵�Դ
void lcd_power_off(void);                                                       // �رյ�Դ
void lcd_light_on(void);                                                        // �򿪱���
void lcd_light_adjust(uint8_t level);                                           // �������ȵ���
void lcd_light_off(void);                                                       // �رձ���

// LCD��������
uint16_t lcd_rgb565(uint8_t red, uint8_t green, uint8_t blue);                  // ɫ��ת��
void     lcd_set_pixel(uint16_t x, uint16_t y, uint16_t rgb565);                // ����һ������ɫ
uint16_t lcd_get_pixel(uint16_t x, uint16_t y);                                 // ��ȡһ������ɫ
void     lcd_fill_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t l, uint16_t rgb565);

// LCD���Ժ���
void lcd_test_driver(void);                                                     // LCD�ײ��������Ժ���

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
