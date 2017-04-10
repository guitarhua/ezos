/**
  ******************************************************************************
  * @file    main.c
  * @author  mousie
  * @version V1.0.0
  * @date    2010.5.23
  * @brief   包含main函数, 最顶层的工程文件
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
  * @brief    私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup Main_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup Main_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */

/**
  * @}
  */

/** @defgroup Main_Private_Function
  * @brief    定义私有函数
  * @{
  */

/**
  * @}
  */



/** @defgroup Main_Function
  * @brief    函数原文件
  * @{
  */

/**
  ******************************************************************************
  * @brief  系统初始化函数
  * @param  none
  * @retval none
  ******************************************************************************
  */
void system_init(void)
{
    __disable_irq();
    assert_init();                                                              // 调试使用COM1口, 速率115200

    systick_init_1ms();
    ezos_init();
    ezos_idle_hook_set(iwatchdog_clear);

    watchdog_init();
    __enable_irq();

    assert_printf("Assert Mode\r\n");
}

/**
  ******************************************************************************
  * @brief  main函数
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
    // gui_start 和 其它任务不能同时使用, 软件冲突!
    // define_start 和 race_led_start, key_led_start 不可同时调用, 软件冲突!

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
