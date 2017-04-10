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
File        : MENU_Private.h
Purpose     : Internal header file
---------------------------END-OF-HEADER------------------------------
*/

/**
  ******************************************************************************
  * @attention
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

#ifndef MENU_PRIVATE_H
#define MENU_PRIVATE_H

#if GUI_WINSUPPORT

#include "WIDGET.h"
#include "GUI_ARRAY.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
/*********************************************************************
*
*       Status flags
*/
#define MENU_SF_HORIZONTAL              MENU_CF_HORIZONTAL
#define MENU_SF_VERTICAL                MENU_CF_VERTICAL
#define MENU_SF_OPEN_ON_POINTEROVER     MENU_CF_OPEN_ON_POINTEROVER
#define MENU_SF_CLOSE_ON_SECOND_CLICK   MENU_CF_CLOSE_ON_SECOND_CLICK
#define MENU_SF_HIDE_DISABLED_SEL       MENU_CF_HIDE_DISABLED_SEL

#define MENU_SF_ACTIVE                  (1<<6)  /* Internal flag only */
#define MENU_SF_POPUP                   (1<<7)  /* Internal flag only */

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
/*********************************************************************
*
*       menu item
*/
typedef struct {
  MENU_Handle hSubmenu;
  U16         Id;
  U16         Flags;
  int         TextWidth;
  char        acText[1];
} MENU_ITEM;

/*********************************************************************
*
*       menu properties
*/
typedef struct {
  GUI_COLOR                   aTextColor[5];
  GUI_COLOR                   aBkColor[5];
  U8                          aBorder[4];
  const GUI_FONT GUI_UNI_PTR* pFont;
} MENU_PROPS;

/*********************************************************************
*
*       menu object
*/
typedef struct {
  WIDGET      Widget;
  MENU_PROPS  Props;
  GUI_ARRAY   ItemArray;
  WM_HWIN     hOwner;
  U16         Flags;
  char        IsSubmenuActive;
  int         Width;
  int         Height;
  int         Sel;
  #if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
    U32 DebugId;
  #endif  
} MENU_Obj;

/*********************************************************************
*
*       Macros for internal use
*
**********************************************************************
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  #define MENU_INIT_ID(p) p->DebugId = MENU_ID
#else
  #define MENU_INIT_ID(p)
#endif

#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  MENU_Obj * MENU_LockH(MENU_Handle h);
  #define MENU_LOCK_H(h)   MENU_LockH(h)
#else
  #define MENU_LOCK_H(h)   (MENU_Obj *)GUI_LOCK_H(h)
#endif

/*********************************************************************
*
*       Public data (internal defaults)
*
**********************************************************************
*/

extern MENU_PROPS           MENU__DefaultProps;
extern const WIDGET_EFFECT* MENU__pDefaultEffect;

/*********************************************************************
*
*       Public functions (internal)
*
**********************************************************************
*/

void      MENU__RecalcTextWidthOfItems(MENU_Obj* pObj);
void      MENU__ResizeMenu            (MENU_Handle hObj);
unsigned  MENU__GetNumItems           (MENU_Obj* pObj);
char      MENU__SetItem               (MENU_Handle hObj, unsigned Index, const MENU_ITEM_DATA* pItemData);
void      MENU__SetItemFlags          (MENU_Obj* pObj, unsigned Index, U16 Mask, U16 Flags);
void      MENU__InvalidateItem        (MENU_Handle hObj, unsigned Index);
int       MENU__FindItem              (MENU_Handle hObj, U16 ItemId, MENU_Handle* phMenu);
int       MENU__SendMenuMessage       (MENU_Handle hObj, WM_HWIN hDestWin, U16 MsgType, U16 ItemId);

#endif /* GUI_WINSUPPORT */
#endif /* MENU_PRIVATE_H */

/*************************** End of file ****************************/
