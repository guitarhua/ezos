/**
  ******************************************************************************
  * @file    main.c
  * @author  mousie
  * @version V1.0.0
  * @date    2010.5.23
  * @brief   ����main����, ���Ĺ����ļ�
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"                                                          // __enable_irq, __disable_irq
#include "user_types.h"

#include "ezos.h"
#include "assert.h"
#include "drv_systick.h"
#include "drv_watchdog.h"



/** @addtogroup Applications
  * @{
  */
/** @defgroup Main
  * @{
  */

/** @defgroup Main_Private_TypeDefine
  * @brief    ˽�����Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup Main_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup Main_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */

/**
  * @}
  */

/** @defgroup Main_Private_Function
  * @brief    ����˽�к���
  * @{
  */

/**
  * @}
  */



/** @defgroup Main_Function
  * @brief    ����ԭ�ļ�
  * @{
  */

/**
  ******************************************************************************
  * @brief  ϵͳ��ʼ������
  * @param  none
  * @retval none
  ******************************************************************************
  */
void system_init(void)
{
    __disable_irq();
    assert_init();                                                              // ����ʹ��COM1��, ����115200

    systick_init_1ms();
    ezos_init();
    ezos_idle_hook_set(iwatchdog_clear);

    watchdog_init();
    __enable_irq();

    assert_printf("Assert Mode\r\n");
}

/**
  ******************************************************************************
  * @brief  main����
  * @param  none
  * @retval none
  ******************************************************************************
  */
int main(void)
{
    extern void define_start(void);
    extern void race_led_start(void);
    extern void key_led_start(void);
    extern void rtc_start(void);
    extern void eeprom_start(void);
    extern void sd_start(void);
    extern void fatfs_start(void);
    extern void tone_start(void);
    extern void gui_start(void);

    system_init();
    // gui_start �� ����������ͬʱʹ��, �����ͻ!
    // define_start �� race_led_start, key_led_start ����ͬʱ����, �����ͻ!

    //define_start();
    race_led_start();
    key_led_start();
    rtc_start();
    eeprom_start();
    sd_start();
    fatfs_start();
    tone_start();
    //gui_start();

    ezos_schedule();
}

/**
  * @}
  */



/**
  * @}
  */
/**
  * @}
  */

/* END OF FILE ---------------------------------------------------------------*/
