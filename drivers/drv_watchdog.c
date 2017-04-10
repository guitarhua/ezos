/**
  ******************************************************************************
  * @file    drv_watchdog.c
  * @author  mousie
  * @version V2.0.0
  * @date    2011.12.6
  * @brief   ���Ź���������, �ṩ�������Ź��Լ����ڿ��Ź�����
  ******************************************************************************
  */

/* Include -------------------------------------------------------------------*/
#include "drv_watchdog.h"

#include "stm32f10x_rcc.h"
#include "stm32f10x_iwdg.h"
#include "stm32f10x_wwdg.h"



/** @addtogroup Drivers
  * @{
  */
/** @defgroup DRV_WatchDog
  * @{
  */



/**
  ******************************************************************************
  * @addtogroup DRV_WatchDog_Configure
  ******************************************************************************
  * @{
  */

/**
  * @}
  */



/** @defgroup DRV_WatchDog_Private_TypeDefine
  * @brief    ˽�����Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_WatchDog_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_WatchDog_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_WatchDog_Private_Function
  * @brief    ����˽�к���
  * @{
  */

/**
  * @}
  */



/** @defgroup DRV_WatchDog_Function
  * @brief    ����ԭ�ļ�
  * @{
  */

/**
  ******************************************************************************
  * @brief  ���Ź���ʼ��
  * @param  none
  * @retval none
  ******************************************************************************
  */
void watchdog_init(void)
{
#ifdef IWATCHDOG_ENABLE
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);                               // ʹ�ܶ������ſڹ��Ĵ���д�빦��
    IWDG_SetPrescaler(IWDG_Prescaler_256);                                      // �������Ź�ʱ�ӷ�Ƶ, 40K/256=156HZ(6.4ms)
    IWDG_SetReload(IWATCHDOG_VALUE);                                            // ���ö������Ź���λʱ��. ����ֵ���ܴ���0xfff
    IWDG_ReloadCounter();                                                       // ι��
    IWDG_Enable();                                                              // ʹ�ܶ������Ź�
#endif

#ifdef WWATCHDOG_ENABLE
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);                        // ���ڿ��Ź�ʱ������
    WWDG_SetPrescaler(WWDG_Prescaler_8);                                        // ���ڿ��Ź����� (36M/4096)/8 = 1098 Hz
    WWDG_SetWindowValue(WWATCHDOG_VALUE);                                       // ���ô��ڿ��Ź�����ֵ
    WWDG_Enable(WWATCHDOG_COUNT);                                               // ʹ�ܴ��ڿ��Ź�����ʼ����ʱ��ֵ
    WWDG_ClearFlag();                                                           // ������ڿ��Ź��жϱ��
//  WWDG_EnableIT();                                                            // ʹ�ܴ��ڿ��Ź���λǰ�ж�(�����ڸ�λǰ�����ݱ���, �������ô��ж�ι��)
#endif
}

/**
  ******************************************************************************
  * @brief  �������Ź�ι��, ���������ȼ���͵�һ��������ִ��.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void iwatchdog_clear(void)
{
#ifdef IWATCHDOG_ENABLE
    IWDG_ReloadCounter();                                                       // ι�������Ź�
#endif
}

/**
  ******************************************************************************
  * @brief  ���ڿ��Ź�ι��, ������ڶ�ʱ���ж���ִ��.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void wwatchdog_clear(void)
{
#ifdef WWATCHDOG_ENABLE
    WWDG_SetCounter(WWATCHDOG_COUNT);                                           // ι���ڿ��Ź�
#endif
}

/**
  ******************************************************************************
  * @brief  ͣ�ô��ڿ��Ź�. ԭ�����ж���Clock, ʹ�䲻�ټ���. �������ʹ��
  * @param  none
  * @retval none
  ******************************************************************************
  */
void wwatchdog_disable(void)
{
#ifdef WWATCHDOG_ENABLE
    WWDG_DeInit();
#endif
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
