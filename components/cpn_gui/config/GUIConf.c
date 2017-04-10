/*********************************************************************
*          Portions COPYRIGHT 2013 STMicroelectronics                *
*          Portions SEGGER Microcontroller GmbH & Co. KG             *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2013  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.20 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agree_
ment supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUIConf.c
Purpose     : Display controller initialization
---------------------------END-OF-HEADER------------------------------
*/

#include "GUIConf.h"
#include "GUI.h"
#include "ezos.h"

#ifdef LCD_ENABLE


// 32 bit aligned memory area
static U32 extMem[GUI_NUMBYTES / 4];

#define GUI_TASK_PERIOD                 50                                      // GUI任务扫描时间, 单位ms. 建议 10-100ms
#define GUI_TASK_PRIORITY               20                                      // GUI任务优先级, 0-254. 优先级无需过高
ezos_task_t task_gui_scan(ezos_state_t *state, ezos_para_t *para);


void GUI_X_Config(void)
{
    GUI_ALLOC_AssignMemory(extMem, GUI_NUMBYTES);                               // 分配内存区域
    GUI_ALLOC_SetAvBlockSize(GUI_BLOCKSIZE);                                    // 设置内存块平均大小
}

// GUI 总初始化函数, 并调用需要的ezos任务
void gui_main_init(void)
{
	lcd_init();
	GUI_Init();
	quick_add(task_gui_scan, GUI_TASK_PRIORITY);
}

// gui定时扫描
ezos_task_t task_gui_scan(ezos_state_t *state, ezos_para_t *para)
{
	GUI_Exec();
	ezos_delay(GUI_TASK_PERIOD);
}


#endif

/*************************** End of file ****************************/
