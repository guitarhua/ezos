/**
  ******************************************************************************
  * @file    touch_main.c
  * @author  mousie-yu
  * @version V1.0.0
  * @date    2013.10.3
  * @brief   触摸屏驱动程序. 适用于 emwin
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "touch_main.h"
#include "GUI.h"
#include "ezos.h"



/** @addtogroup Components
  * @{
  */
/** @defgroup CPN_GUI
  * @{
  */
#if GUI_SUPPORT_TOUCH && (defined LCD_ENABLE)



/** @defgroup TOUCH_MAIN_Private_TypeDefine
  * @brief    私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup TOUCH_MAIN_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup TOUCH_MAIN_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */

/**
  * @}
  */

/** @defgroup TOUCH_MAIN_Private_Function
  * @brief    定义私有函数
  * @{
  */
static EZOS_TASK(task_touch_scan);
/**
  * @}
  */



/** @defgroup TOUCH_MAIN_Function
  * @brief    函数原文件
  * @{
  */
/**
  ******************************************************************************
  * @brief  触摸屏 IO口及驱动芯片初始化函数.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void touch_init(void)
{
    touch_port_init();
    touch_chip_init();
    touch_start();
}

/**
  ******************************************************************************
  * @brief  触摸屏功能的开启与关闭
  * @param  none
  * @retval none
  ******************************************************************************
  */
void touch_start(void)
{
    // 1S 后启动触摸屏扫描任务, 优先级为10
    ezos_add(task_touch_scan, task_touch_scan, 0, 1000, 10);
}

void touch_stop(void)
{
    ezos_delete(task_touch_scan);
}

/**
  ******************************************************************************
  * @brief  触摸屏扫描任务, 建议10ms左右扫描一次
  * @param  none
  * @retval none / int
  ******************************************************************************
  */
EZOS_TASK(task_touch_scan)
{
    GUI_TOUCH_Exec();
    ezos_delay(10);
}

/**
  ******************************************************************************
  * @brief  emWIN 需要用到的4个函数
  * @param  none
  * @retval none / int
  ******************************************************************************
  */
void GUI_TOUCH_X_ActivateX(void)
{
}

void GUI_TOUCH_X_ActivateY(void)
{
}

int  GUI_TOUCH_X_MeasureX(void)
{
    return touch_read_x();
}

int  GUI_TOUCH_X_MeasureY(void)
{
    return touch_read_y();
}

/**
  ******************************************************************************
  * @brief  触摸屏驱动测试函数. 暂时无效!
  * @param  none
  * @retval none
  ******************************************************************************
  */
void touch_test_driver(void)
{
    // 初始化
    lcd_init();
    touch_init();
    touch_stop();

    // emWIN测试
    GUI_Init();
    GUI_SetBkColor(GUI_BLUE);
    GUI_Clear();
    GUI_GotoXY(10, 20);
    GUI_DispString("TOUCH TEST");
}



/**
  * @}
  */

#endif
/**
  * @}
  */
/**
  * @}
  */

/* END OF FILE ---------------------------------------------------------------*/
