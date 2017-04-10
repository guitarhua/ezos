/**
  ******************************************************************************
  * @file    drv_key.h
  * @author  mousie
  * @version V2.1.1
  * @date    2015.4.21
  * @brief   ֱ��ʽ������������.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_KEY_H
#define __DRV_KEY_H

/* Include -------------------------------------------------------------------*/
#include <stdint.h>

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup DRV_Key
  * @{
  */



/**
  ******************************************************************************
  * @defgroup DRV_Key_Configure
  * @brief    �û�����
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // Ӳ�����������ļ�

//#define KEY_NUM                         8                                       ///< ���ð�������, �������������16��ֱ��ʽ����.
//#define KEY_EXTPULL                                                             ///< �������ⲿ��������
//
//#define KEY0_PORT                       GPIOC                                   ///< ���øð�����PORT��
//#define KEY0_CLK                        RCC_APB2Periph_GPIOC                    ///< ���øð�����ʱ��ģ��
//#define KEY0_PIN                        GPIO_Pin_13                             ///< ���øð�����PIN��
//
//#define KEY1_PORT                       GPIOA
//#define KEY1_CLK                        RCC_APB2Periph_GPIOA
//#define KEY1_PIN                        GPIO_Pin_8
//
//#define KEY2_PORT                       GPIOD
//#define KEY2_CLK                        RCC_APB2Periph_GPIOD
//#define KEY2_PIN                        GPIO_Pin_0
//
//#define KEY3_PORT                       GPIOD
//#define KEY3_CLK                        RCC_APB2Periph_GPIOD
//#define KEY3_PIN                        GPIO_Pin_1
//
//#define KEY4_PORT                       GPIOD
//#define KEY4_CLK                        RCC_APB2Periph_GPIOD
//#define KEY4_PIN                        GPIO_Pin_2
//
//#define KEY5_PORT                       GPIOD
//#define KEY5_CLK                        RCC_APB2Periph_GPIOD
//#define KEY5_PIN                        GPIO_Pin_3
//
//#define KEY6_PORT                       GPIOD
//#define KEY6_CLK                        RCC_APB2Periph_GPIOD
//#define KEY6_PIN                        GPIO_Pin_4
//
//#define KEY7_PORT                       GPIOA
//#define KEY7_CLK                        RCC_APB2Periph_GPIOA
//#define KEY7_PIN                        GPIO_Pin_0
//
///// KEY�������Ͷ���
//typedef enum
//{
//    S2_TAMPER = 0,                                                              ///< ����Ϊ��������KEY����, ֵ0��ӦKEY0_xxx
//    S4_USER,                                                                    ///< ֵ1��ӦKEY1_xxx, ��������
//    JOY_SEL,
//    JOY_DOWN,
//    JOY_LEFT,
//    JOY_RIGHT,
//    JOY_UP,
//    KEY7,
//} key_name_t;



#if (KEY_NUM > 16)
    #error "KEY_NUM define error!"
#endif
/**
  * @}
  */



/** @defgroup DRV_Key_Public_TypeDefine
  * @brief    �������Ͷ���
  * @{
  */
/// ����״̬���Ͷ���
typedef struct
{
    uint16_t on_off;                                                            ///< ��ǰ��������ֵ
    uint16_t touch;                                                             ///< �̰�������ʾ
    uint16_t click;                                                             ///< ���������ͷű�ʾ
    uint16_t lclick;                                                            ///< ����������ʾ
    uint16_t lpress;                                                            ///< ����������ʾ
} key_status_t;
/**
  * @}
  */

/** @defgroup DRV_Key_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */
#define KEY_MASK(key_name)              (1 << key_name)                         ///< ����ȡģ, ��Ӧ��bit
/**
  * @}
  */

/** @defgroup DRV_Key_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */
/**
  * @brief ʹ��ȫ�ֱ����ķ�ʽ�ṩֱ��ʽ��������ֵ
  *
  *   @arg KeyStatus.on_off,  ��������״̬�Ĵ���.      ��Ӧbit.            0,�ް���. 1,�а���
  *   @arg KeyStatus.touch,   ��������״̬�Ĵ���.      �������,           ��Ӧbit��λ
  *   @arg KeyStatus.click,   ���������ͷ�״̬�Ĵ���.  ���������ͷ�,       ��Ӧbit��λ
  *   @arg KeyStatus.lclick,  ���������仯״̬�Ĵ���.  ��������,           ��Ӧbit��λ
  *   @arg KeyStatus.lpress,  �����ڽ��볤��״̬��.    ����Ч��,           ��Ӧbit��λ
  *
  * @note ���������������Ķ�Ӧ��ϵ�� keyName_t ��ö��ֵ��������bitλ, ���16������
  *       ����0ʾ����
  * ______________--------------------------------------------_________________________
  *     |         |<- 0.2S ->|<-    1.1S   ->|<- 0.2S ->|<- 0.2 S->|     |           |
  *  �ް���     ����                        ����      ����      ����     �ͷ�       �ް���
  * .on_off=0 .on_off=1  .on_off=1          .on_off=1 .on_off=1 .on_off=1 .on_off=0  .on_off=0
  * .touch=0  .touch=1                                          .touch=0   .touch=0
  * .click=0             .click=1(��1.3S���ͷ�)                 .click=0  .click=0
  * .lclick=0                               .lclick=1 .lclick=0 .lclick=0 .lclick=0  .lclick=0
  * .lpress=0                               .lpress=1 .lpress=1 .lpress=1 .lpress=0  .lpress=0
  */
volatile extern key_status_t key_status;
/**
  * @}
  */

/** @defgroup DRV_Key_Public_Function
  * @brief    ���幫�к���
  * @{
  */
#if (KEY_NUM > 0)

void key_init_all(void);                                                        // ��ʼ�����а���
void key_init(key_name_t key_name);                                             // ��ʼ��ָ������
void key_scan_10ms(void);                                                       // ����ɨ�躯��
///void key_task_10ms(void);                                                    // ��������������, �ɸ��Ƶ�Ӧ�ò�ʹ��

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
