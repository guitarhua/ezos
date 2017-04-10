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
File        : MULTIPAGE.h
Purpose     : MULTIPAGE include
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

#ifndef MULTIPAGE_H
#define MULTIPAGE_H

#include "WM.h"
#include "DIALOG.h"      /* Req. for Create indirect data structure */

#if GUI_WINSUPPORT

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       defines
*
**********************************************************************
*/
/*********************************************************************
*
*       Create / Status flags
*/

#define MULTIPAGE_ALIGN_LEFT    (0<<0)
#define MULTIPAGE_ALIGN_RIGHT   (1<<0)
#define MULTIPAGE_ALIGN_TOP     (0<<2)
#define MULTIPAGE_ALIGN_BOTTOM  (1<<2)

#define MULTIPAGE_CF_ROTATE_CW WIDGET_CF_VERTICAL

#define MULTIPAGE_CI_DISABLED 0
#define MULTIPAGE_CI_ENABLED  1

/*********************************************************************
*
*       Public Types
*
**********************************************************************
*/

typedef WM_HMEM MULTIPAGE_Handle;

/*********************************************************************
*
*       Create functions
*
**********************************************************************
*/

MULTIPAGE_Handle MULTIPAGE_Create        (int x0, int y0, int xsize, int ysize, WM_HWIN hParent, int Id, int Flags, int SpecialFlags);
MULTIPAGE_Handle MULTIPAGE_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb);
MULTIPAGE_Handle MULTIPAGE_CreateEx      (int x0, int y0, int xsize, int ysize, WM_HWIN hParent,
                                          int WinFlags, int ExFlags, int Id);
MULTIPAGE_Handle MULTIPAGE_CreateUser    (int x0, int y0, int xsize, int ysize, WM_HWIN hParent,
                                          int WinFlags, int ExFlags, int Id, int NumExtraBytes);

/*********************************************************************
*
*       The callback ...
*
* Do not call it directly ! It is only to be used from within an
* overwritten callback.
*/
void MULTIPAGE_Callback(WM_MESSAGE *pMsg);

/*********************************************************************
*
*       Member functions
*
**********************************************************************
*/

/* Methods changing properties */
void        MULTIPAGE_AddPage       (MULTIPAGE_Handle hObj, WM_HWIN hWin ,const char * pText);
void        MULTIPAGE_AddEmptyPage  (MULTIPAGE_Handle hObj, WM_HWIN hWin ,const char * pText);
WM_HWIN     MULTIPAGE_AttachWindow  (MULTIPAGE_Handle hObj, unsigned Index, WM_HWIN hWin);
void        MULTIPAGE_DeletePage    (MULTIPAGE_Handle hObj, unsigned Index, int Delete);
void        MULTIPAGE_DisablePage   (MULTIPAGE_Handle hObj, unsigned Index);
void        MULTIPAGE_EnablePage    (MULTIPAGE_Handle hObj, unsigned Index);
int         MULTIPAGE_GetSelection  (MULTIPAGE_Handle hObj);
WM_HWIN     MULTIPAGE_GetWindow     (MULTIPAGE_Handle hObj, unsigned Index);
int         MULTIPAGE_GetUserData   (MULTIPAGE_Handle hObj, void * pDest, int NumBytes);
int         MULTIPAGE_IsPageEnabled (MULTIPAGE_Handle hObj, unsigned Index);
void        MULTIPAGE_SelectPage    (MULTIPAGE_Handle hObj, unsigned Index);
void        MULTIPAGE_SetAlign      (MULTIPAGE_Handle hObj, unsigned Align);
void        MULTIPAGE_SetBkColor    (MULTIPAGE_Handle hObj, GUI_COLOR Color, unsigned Index);
void        MULTIPAGE_SetFont       (MULTIPAGE_Handle hObj, const GUI_FONT GUI_UNI_PTR * pFont);
void        MULTIPAGE_SetRotation   (MULTIPAGE_Handle hObj, unsigned Rotation);
void        MULTIPAGE_SetText       (MULTIPAGE_Handle hObj, const char * pText, unsigned Index);
void        MULTIPAGE_SetTextColor  (MULTIPAGE_Handle hObj, GUI_COLOR Color, unsigned Index);
int         MULTIPAGE_SetUserData   (MULTIPAGE_Handle hObj, const void * pSrc, int NumBytes);

/*********************************************************************
*
*       Get/Set defaults
*
**********************************************************************
*/

unsigned                     MULTIPAGE_GetDefaultAlign(void);
GUI_COLOR                    MULTIPAGE_GetDefaultBkColor(unsigned Index);
const GUI_FONT GUI_UNI_PTR * MULTIPAGE_GetDefaultFont(void);
GUI_COLOR                    MULTIPAGE_GetDefaultTextColor(unsigned Index);

void                         MULTIPAGE_SetDefaultAlign(unsigned Align);
void                         MULTIPAGE_SetDefaultBkColor(GUI_COLOR Color, unsigned Index);
void                         MULTIPAGE_SetDefaultFont(const GUI_FONT GUI_UNI_PTR * pFont);
void                         MULTIPAGE_SetDefaultTextColor(GUI_COLOR Color, unsigned Index);

void      MULTIPAGE_SetEffectColor    (unsigned Index, GUI_COLOR Color);
GUI_COLOR MULTIPAGE_GetEffectColor    (unsigned Index);
int       MULTIPAGE_GetNumEffectColors(void);

#if defined(__cplusplus)
  }
#endif

#endif   /* if GUI_WINSUPPORT */
#endif   /* MULTIPAGE_H */
