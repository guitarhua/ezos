/**
  ******************************************************************************
  * @file    drv_led.h
  * @author  mousie
  * @version V2.1.0
  * @date    2012.2.25
  * @brief   LED����������. ֧�ֿɵ���/������/��˸��Ч��.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_LED_H
#define __DRV_LED_H

/* Include -------------------------------------------------------------------*/
#include <stdint.h>

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup DRV_Led
  * @{
  */



/**
  ******************************************************************************
  * @defgroup DRV_Led_Configure
  * @brief    �û�����
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // Ӳ�����������ļ�

//#define LED_NUM                         4                                       ///< ����LED������, �������������16��LED��.
//#define LED_LEVEL                       0                                       ///< 0, �͵�ƽ����LED. 1, �ߵ�ƽ����LED
//#define LED_ADVANCED_ENABLE                                                     ///< ʹ��LED�ĸ߼�����, ������˸�ͽ���Ч��
//
//#define LED0_PORT                       GPIOD                                   ///< ���ø�LED��PORT��
//#define LED0_CLK                        RCC_APB2Periph_GPIOD                    ///< ���ø�LED��ʱ��ģ��
//#define LED0_PIN                        GPIO_Pin_8                              ///< ���ø�LED��PIN��
//
//#define LED1_PORT                       GPIOD
//#define LED1_CLK                        RCC_APB2Periph_GPIOD
//#define LED1_PIN                        GPIO_Pin_9
//
//#define LED2_PORT                       GPIOD
//#define LED2_CLK                        RCC_APB2Periph_GPIOD
//#define LED2_PIN                        GPIO_Pin_10
//
//#define LED3_PORT                       GPIOD
//#define LED3_CLK                        RCC_APB2Periph_GPIOD
//#define LED3_PIN                        GPIO_Pin_11
//
///// LED�������Ͷ���
//typedef enum
//{
//    LED1 = 0,                                                                   ///< ����Ϊ��������LED����, ֵ0��ӦLED0_xxx
//    LED2,                                                                       ///< ֵ1��ӦLED1_xxx, ��������
//    LED3,
//    LED4,
//} led_name_t;



#if (LED_NUM > 16)
    #error "LED_NUM define error!"
#endif
/**
  * @}
  */



/** @defgroup DRV_Led_Public_TypeDefine
  * @brief    �������Ͷ���
  * @{
  */
/// LED��״̬���Ͷ���
typedef enum
{
    LED_ON   = 0x01,                                                            ///< LED�ƴ�״̬
    LED_OFF  = 0x00,                                                            ///< LED�ƹر�״̬
    BLED_ON  = 0x11,                                                            ///< �����ƴ�״̬
    BLED_OFF = 0x10,                                                            ///< �����ƹر�״̬
    FLED_ON  = 0x21,                                                            ///< ��˸�Ƹߵ�ƽ״̬
    FLED_OFF = 0x20,                                                            ///< ��˸�Ƶ͵�ƽ״̬
} led_status_t;
/**
  * @}
  */

/** @defgroup DRV_Led_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_Led_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_Led_Public_Function
  * @brief    ���幫�к���
  * @{
  */
#if (LED_NUM > 0)

// �������ܺ���
void led_init_all(void);                                                        // ��ʼ�����е�LED
void led_init(led_name_t led_name);                                             // ��ʼ��ָ����LED
void led_on(led_name_t led_name);                                               // LED��
void led_off(led_name_t led_name);                                              // LED�ر�
void led_toggle(led_name_t led_name);                                           // LED��ת
led_status_t led_get_status(led_name_t led_name);                               // ��ȡ��ǰLED״̬

#ifdef LED_ADVANCED_ENABLE

// �߼����ܺ���
void led_scan_1ms(void);                                                        // LED 1ms ��ʱɨ��, ��1ms��ʱ�жϺ����е���
void led_adjust(led_name_t led_name, uint8_t level);                            // LED��������ֵ. 0����, 0xFFϨ��.
void led_on_gradual(led_name_t led_name);                                       // LED�����
void led_off_gradual(led_name_t led_name);                                      // LED����ر�
void led_toggle_gradual(led_name_t led_name);                                   // LED���䷭ת
void led_flash(led_name_t led_name, uint16_t time_ms, uint8_t counter);         // LED��˸ָ������, counter=0 ��ʾһֱ��˸

#endif
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
