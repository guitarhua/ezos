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
File        : LCDConf.c
Purpose     : Display controller configuration (single layer)
---------------------------END-OF-HEADER------------------------------
*/

#include "lcdgui_conf.h"

#ifdef LCD_ENABLE

#include "LCDConf.h"
#include "GUI.h"
#include "GUIDRV_FlexColor.h"



#if EMWIN_LCD_DRIVER_API_ENABLE

    // 使用emWIN自带的LCD驱动, 但屏幕初始化依旧需要额外调用!
    #define DISPLAY_DRIVER                  &GUIDRV_FlexColor_API

    // emWin内置驱动需要的底层函数
    static void LcdWriteReg(U16 Data) {
        lcd_bus_reg_wr(Data);
    }

    static void LcdWriteData(U16 Data) {
        lcd_bus_data_wr(Data);
    }

    static void LcdWriteDataMultiple(U16 * pData, int NumItems) {
        lcd_bus_datas_wr_start();
        while (NumItems--) {
            lcd_bus_datas_wring(*pData++);
        }
        lcd_bus_datas_wr_stop();
    }

    static void LcdReadDataMultiple(U16 * pData, int NumItems) {
        lcd_bus_datas_rd_start();
        while (NumItems--) {
            *pData++ = lcd_bus_datas_rding();
        }
        lcd_bus_datas_rd_stop();
    }

#else

    // GUIDriver_API 为模板提供的用户LCD底层驱动, 建议使用此方式
    extern const GUI_DEVICE_API LCDDriver_API;
    #define DISPLAY_DRIVER                  &LCDDriver_API

#endif



void LCD_X_Config(void) {
#if EMWIN_LCD_DRIVER_API_ENABLE

    GUI_DEVICE * pDevice;
    CONFIG_FLEXCOLOR Config = {0};
    GUI_PORT_API PortAPI = {0};

    // Set display driver and color conversion
    pDevice = GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER, COLOR_CONVERSION, 0, 0);

    // Display driver configuration, required for Lin-driver
    LCD_SetSizeEx (0, XSIZE_PHYS , YSIZE_PHYS);
    LCD_SetVSizeEx(0, VXSIZE_PHYS, VYSIZE_PHYS);

    // Orientation
    Config.Orientation = 0;
    #if LCD_MIRROR_X
        Config.Orientation |= GUI_MIRROR_X;
    #endif
    #if LCD_MIRROR_Y
        Config.Orientation |= GUI_MIRROR_Y;
    #endif
    #if LCD_SWAP_XY
        Config.Orientation |= GUI_SWAP_XY;
    #endif
    GUIDRV_FlexColor_Config(pDevice, &Config);

    // Set controller and operation mode
    PortAPI.pfWrite16_A0  = LcdWriteReg;
    PortAPI.pfWrite16_A1  = LcdWriteData;
    PortAPI.pfWriteM16_A1 = LcdWriteDataMultiple;
    PortAPI.pfReadM16_A1  = LcdReadDataMultiple;

    // 使用emWIN内置驱动时, 需要注意修改此函数的后面两个参数.
    #if (defined LCD_CHIP_ILI9320) || (defined LCD_CHIP_ILI9325)
        #define GUIDRV_FLEXCOLOR_CHIP   GUIDRV_FLEXCOLOR_F66708
        #define GUIDRV_FLEXCOLOR_MODE   GUIDRV_FLEXCOLOR_M16C0B16
    #elif (defined LCD_CHIP_HX8347)
        #define GUIDRV_FLEXCOLOR_CHIP   GUIDRV_FLEXCOLOR_F66712
        #define GUIDRV_FLEXCOLOR_MODE   GUIDRV_FLEXCOLOR_M16C0B16
    #endif
    GUIDRV_FlexColor_SetFunc(pDevice, &PortAPI, GUIDRV_FLEXCOLOR_CHIP, GUIDRV_FLEXCOLOR_MODE);

#else

    GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER, COLOR_CONVERSION, 0, 0);

    // Display driver configuration, required for Lin-driver
    LCD_SetSizeEx (0, XSIZE_PHYS , YSIZE_PHYS);
    LCD_SetVSizeEx(0, VXSIZE_PHYS, VYSIZE_PHYS);

#endif

#if GUI_SUPPORT_TOUCH
  {
    // 使能了触摸屏功能
    unsigned Orientation = 0;
    #if TOUCH_MIRROR_X
        Orientation |= GUI_MIRROR_X;
    #endif
    #if TOUCH_MIRROR_Y
        Orientation |= GUI_MIRROR_Y;
    #endif
    #if TOUCH_SWAP_XY
        Orientation |= GUI_SWAP_XY;
    #endif
    GUI_TOUCH_SetOrientation(Orientation);

    GUI_TOUCH_Calibrate(GUI_COORD_X, 0, XSIZE_PHYS-1, TOUCH_AD_LEFT, TOUCH_AD_RIGHT);
    GUI_TOUCH_Calibrate(GUI_COORD_Y, 0, YSIZE_PHYS-1, TOUCH_AD_TOP,  TOUCH_AD_BOTTOM);
  }
#endif
}

// LCD_X_DisplayDriver. return: <-1,ERR. =-1, No CMD. =0, OK
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
    switch (Cmd) {
    case LCD_X_INITCONTROLLER:
        return -1;
    case LCD_X_SETVRAMADDR:
        return -1;
    case LCD_X_ON:
        return -1;
    case LCD_X_OFF:
        return -1;
    }
    return -1;
}

#endif
/*************************** End of file ****************************/
