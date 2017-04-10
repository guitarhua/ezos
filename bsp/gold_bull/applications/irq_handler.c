/**
  ******************************************************************************
  * @file    irq_handler.c
  * @author  mousie
  * @version V1.0.0
  * @date    2010.5.5
  * @brief   ϵͳ�жϷ�����
  ******************************************************************************
  */

/* Include -------------------------------------------------------------------*/
#include <stdint.h>



/** @addtogroup Applications
  * @{
  */
/** @defgroup IRQ_Handler
  * @{
  */



/** @defgroup IRQ_Handler_Private_TypeDefine
  * @brief    ˽�����Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup IRQ_Handler_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup IRQ_Handler_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */

/**
  * @}
  */

/** @defgroup IRQ_Handler_Private_Function
  * @brief    ����˽�к���
  * @{
  */

/**
  * @}
  */



/** @defgroup IRQ_Handler_Function
  * @brief    ����ԭ�ļ�
  * @{
  */

/**
  ******************************************************************************
  * @brief  ϵͳ����ж�, 1ms����.
  * @param  none
  * @retval none
  ******************************************************************************
  */
#include "drv_watchdog.h"
#include "ezos.h"
#include "drv_systick.h"
#include "drv_led.h"
#include "tcpip.h"
void SysTick_Handler(void)
{
    wwatchdog_clear();
    ezos_scan_tick();
    delay_scan_1ms();
    led_scan_1ms();
    uipsys_scan_1ms();
}

/**
  ******************************************************************************
  * @brief  This function handles NMI exception.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void NMI_Handler(void)
{
}

/**
  ******************************************************************************
  * @brief  This function handles SVCall exception.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void SVC_Handler(void)
{
}

/**
  ******************************************************************************
  * @brief  This function handles PendSV_Handler exception.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void PendSV_Handler(void)
{
}

/**
  ******************************************************************************
  * @brief  This function handles Debug Monitor exception.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void DebugMon_Handler(void)
{
}

/**
  ******************************************************************************
  * @brief  This function handles Hard Fault exception.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void HardFault_Handler(void)
{
    while (1)
    {
    }
}

/**
  ******************************************************************************
  * @brief  This function handles Memory Manage exception.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void MemManage_Handler(void)
{
    while (1)
    {
    }
}

/**
  ******************************************************************************
  * @brief  This function handles Bus Fault exception.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void BusFault_Handler(void)
{
    while (1)
    {
    }
}

/**
  ******************************************************************************
  * @brief  This function handles Usage Fault exception.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void UsageFault_Handler(void)
{
    while (1)
    {
    }
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
