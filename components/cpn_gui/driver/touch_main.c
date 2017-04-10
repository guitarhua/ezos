/**
  ******************************************************************************
  * @file    touch_main.c
  * @author  mousie-yu
  * @version V1.0.0
  * @date    2013.10.3
  * @brief   ��������������. ������ emwin
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
  * @brief    ˽�����Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup TOUCH_MAIN_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup TOUCH_MAIN_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */

/**
  * @}
  */

/** @defgroup TOUCH_MAIN_Private_Function
  * @brief    ����˽�к���
  * @{
  */
static EZOS_TASK(task_touch_scan);
/**
  * @}
  */



/** @defgroup TOUCH_MAIN_Function
  * @brief    ����ԭ�ļ�
  * @{
  */
/**
  ******************************************************************************
  * @brief  ������ IO�ڼ�����оƬ��ʼ������.
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
  * @brief  ���������ܵĿ�����ر�
  * @param  none
  * @retval none
  ******************************************************************************
  */
void touch_start(void)
{
    // 1S ������������ɨ������, ���ȼ�Ϊ10
    ezos_add(task_touch_scan, task_touch_scan, 0, 1000, 10);
}

void touch_stop(void)
{
    ezos_delete(task_touch_scan);
}

/**
  ******************************************************************************
  * @brief  ������ɨ������, ����10ms����ɨ��һ��
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
  * @brief  emWIN ��Ҫ�õ���4������
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
  * @brief  �������������Ժ���. ��ʱ��Ч!
  * @param  none
  * @retval none
  ******************************************************************************
  */
void touch_test_driver(void)
{
    // ��ʼ��
    lcd_init();
    touch_init();
    touch_stop();

    // emWIN����
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
