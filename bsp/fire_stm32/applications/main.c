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
    ezos_idle_hook_set(iwatchdog_clear);                                        // ϵͳ��શ�ʱ����ʼ��

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
    extern void sem_start(void);
    extern void mail_start(void);
    extern void message_start(void);
    extern void bell_start(void);
    extern void rtc_start(void);
    extern void eeprom_start(void);
    extern void spiflash_start(void);
    extern void sd_start(void);
    extern void fatfs_start(void);
    extern void sd2flash_start(void);
    extern void tone_start(void);
    extern void gui_start(void);
    extern void net_start(void);

    system_init();
    // gui_start �� ����������ͬʱʹ��, �����ͻ! �Һ� net_start, eeprom_start Ӳ����ͻ!
    // define_start �� race_led_start, key_led_start ����ͬʱ����, �����ͻ!

    //define_start();
    //race_led_start();
    //sem_start();
    //mail_start();
    //message_start();
    //bell_start();
    //rtc_start();
    //eeprom_start();
    //spiflash_start();
    //sd_start();
    //fatfs_start();
    //sd2flash_start();
    //tone_start();
    gui_start();
    //net_start();

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
