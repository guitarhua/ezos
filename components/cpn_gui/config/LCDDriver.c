// �û��Զ�LCD�ӿ�����, ��ʹ��emWIN����LCD����, ��������ļ�

#include "LCDConf.h"
#include "lcdgui_conf.h"

#if (defined LCD_ENABLE) && (EMWIN_LCD_DRIVER_API_ENABLE == 0)

#include "LCD_Private.h"
#include "GUI_Private.h"
#include "LCD_ConfDefaults.h"



// GUIDriver �û��Ż�, ���ˢ���ٶ�. ���������������ʵ��������һ��ʱʹ��!
#define LCD_DRIVER_USER_OPTIMIZE        1

#if (LCD_MIRROR_X || LCD_MIRROR_Y || LCD_SWAP_XY)
    #undef LCD_DRIVER_USER_OPTIMIZE
    #define LCD_DRIVER_USER_OPTIMIZE    0
#endif

// Macros for MIRROR_, SWAP_ and LUT_
#if (!defined (LCD_LUT_COM) && !defined(LCD_LUT_SEG))
  #if   (!LCD_MIRROR_X && !LCD_MIRROR_Y && !LCD_SWAP_XY)
    #define LOG2PHYS_X(x, y) (x)
    #define LOG2PHYS_Y(x, y) (y)
  #elif (!LCD_MIRROR_X && !LCD_MIRROR_Y &&  LCD_SWAP_XY)
    #define LOG2PHYS_X(x, y) (y)
    #define LOG2PHYS_Y(x, y) (x)
  #elif (!LCD_MIRROR_X &&  LCD_MIRROR_Y && !LCD_SWAP_XY)
    #define LOG2PHYS_X(x, y) (x)
    #define LOG2PHYS_Y(x, y) (YSIZE_PHYS - 1 - (y))
  #elif (!LCD_MIRROR_X &&  LCD_MIRROR_Y &&  LCD_SWAP_XY)
    #define LOG2PHYS_X(x, y) (y)
    #define LOG2PHYS_Y(x, y) (XSIZE_PHYS - 1 - (x))
  #elif ( LCD_MIRROR_X && !LCD_MIRROR_Y && !LCD_SWAP_XY)
    #define LOG2PHYS_X(x, y) (XSIZE_PHYS - 1 - (x))
    #define LOG2PHYS_Y(x, y) (y)
  #elif ( LCD_MIRROR_X && !LCD_MIRROR_Y &&  LCD_SWAP_XY)
    #define LOG2PHYS_X(x, y) (YSIZE_PHYS - 1 - (y))
    #define LOG2PHYS_Y(x, y) (x)
  #elif ( LCD_MIRROR_X &&  LCD_MIRROR_Y && !LCD_SWAP_XY)
    #define LOG2PHYS_X(x, y) (XSIZE_PHYS - 1 - (x))
    #define LOG2PHYS_Y(x, y) (YSIZE_PHYS - 1 - (y))
  #elif ( LCD_MIRROR_X &&  LCD_MIRROR_Y &&  LCD_SWAP_XY)
    #define LOG2PHYS_X(x, y) (YSIZE_PHYS - 1 - (y))
    #define LOG2PHYS_Y(x, y) (XSIZE_PHYS - 1 - (x))
  #endif
#else
  #if   ( defined (LCD_LUT_COM) && !defined(LCD_LUT_SEG))
    #define LOG2PHYS_X(x, y) (x)
    #define LOG2PHYS_Y(x, y) (LCD__aLine2Com0[y])
  #elif (!defined (LCD_LUT_COM) &&  defined(LCD_LUT_SEG))
    #define LOG2PHYS_X(x, y) (LCD__aCol2Seg0[x])
    #define LOG2PHYS_Y(x, y) (y)
  #elif ( defined (LCD_LUT_COM) &&  defined(LCD_LUT_SEG))
    #define LOG2PHYS_X(x, y) (LCD__aCol2Seg0[x])
    #define LOG2PHYS_Y(x, y) (LCD__aLine2Com0[y])
  #endif
#endif

// Types
typedef struct {
    U32 VRAMAddr;
    int xSize, ySize;
    int vxSize, vySize;
    int vxSizePhys;
    int BitsPerPixel;
} DRIVER_CONTEXT;



/*
**********************************************************************
*   д�㺯��, ����Ҫ�ĺ���. ʵ����Ŀ�������Ż������ʾ�ٶ�!
**********************************************************************
*/
static void _SetPixelIndex(GUI_DEVICE * pDevice, int x, int y, int PixelIndex) {
    // ����ĳЩ���������� pDevice δʹ�õľ���
    // GUI_USE_PARA(pDevice);

    // ʵ�ʵ��õ�д�㺯��
    lcd_set_pixel(LOG2PHYS_X(x, y), LOG2PHYS_Y(x, y), PixelIndex);
    // lcd_chip_pixel_wr(LOG2PHYS_X(x, y), LOG2PHYS_Y(x, y), PixelIndex);
}

/*
**********************************************************************
*   ���㺯��, ��Ҫ����֮һ.
*   ��ʵ����Ŀ����Ҫ�õ� ��ɫ/͸��/��͸�� Ч��ʱ, �ɲ��ô˺���
**********************************************************************
*/
static unsigned int _GetPixelIndex(GUI_DEVICE * pDevice, int x, int y) {
    unsigned int PixelIndex;

    // ����ĳЩ���������� pDevice δʹ�õľ���
    // GUI_USE_PARA(pDevice);

    // ʵ�ʵ��õĶ��㺯��
    PixelIndex = lcd_get_pixel(LOG2PHYS_X(x, y), LOG2PHYS_Y(x, y));
    // PixelIndex = lcd_chip_pixel_rd(LOG2PHYS_X(x, y), LOG2PHYS_Y(x, y));

    return PixelIndex;
}

// _XorPixel
static void _XorPixel(GUI_DEVICE * pDevice, int x, int y) {
    LCD_PIXELINDEX PixelIndex;
    LCD_PIXELINDEX IndexMask;

    PixelIndex = _GetPixelIndex(pDevice, x, y);
    IndexMask  = pDevice->pColorConvAPI->pfGetIndexMask();
    _SetPixelIndex(pDevice, x, y, PixelIndex ^ IndexMask);
}

/*
**********************************************************************
*   ��ˮƽ��
**********************************************************************
*/
static void _DrawHLine  (GUI_DEVICE * pDevice, int x0, int y,  int x1) {
    if (GUI_pContext->DrawMode & LCD_DRAWMODE_XOR) {
        for (; x0 <= x1; x0++) {
            _XorPixel(pDevice, x0, y);
        }
    } else {
        #if LCD_DRIVER_USER_OPTIMIZE

            lcd_fill_rectangle(x0, y, x1-x0+1, 1, LCD_COLORINDEX);

        #else
            LCD_PIXELINDEX ColorIndex = LCD__GetColorIndex();
            for (; x0 <= x1; x0++) {
                _SetPixelIndex(pDevice, x0, y, ColorIndex);
            }
        #endif
    }
}

/*
**********************************************************************
*   ����ֱ��
**********************************************************************
*/
static void _DrawVLine (GUI_DEVICE * pDevice, int x, int y0,  int y1) {
    if (GUI_pContext->DrawMode & LCD_DRAWMODE_XOR) {
        for (; y0 <= y1; y0++) {
            _XorPixel(pDevice, x, y0);
        }
    } else {
        #if LCD_DRIVER_USER_OPTIMIZE

            lcd_fill_rectangle(
                x,
                y0,
                1, y1-y0+1, LCD_COLORINDEX);

        #else
            LCD_PIXELINDEX ColorIndex = LCD__GetColorIndex();
            for (; y0 <= y1; y0++)  {
                _SetPixelIndex(pDevice, x, y0, ColorIndex);
            }
        #endif
    }
}

/*
**********************************************************************
*   �����ο�
**********************************************************************
*/
static void _FillRect(GUI_DEVICE * pDevice, int x0, int y0, int x1, int y1)
{
    if (GUI_pContext->DrawMode & LCD_DRAWMODE_XOR) {
        for (; x0 <= x1; x0++) {
            for (; y0 <= y1; y0++) {
                _XorPixel(pDevice, x0, y0);
            }
        }
    } else {
        #if LCD_DRIVER_USER_OPTIMIZE

            lcd_fill_rectangle(x0, y0, x1-x0+1, y1-y0+1, LCD_COLORINDEX);

        #else
            int i;
            LCD_PIXELINDEX ColorIndex = LCD__GetColorIndex();
            for (; x0 <= x1; x0++) {
                for (i=y0; i <= y1; i++) {
                    _SetPixelIndex(pDevice, x0, i, ColorIndex);
                }
            }
        #endif
    }
}

// Draw Bitmap 1 BPP
static void _DrawBitLine1BPP(GUI_DEVICE * pDevice, unsigned x, unsigned y, U8 const GUI_UNI_PTR * p, int Diff, int xsize, const LCD_PIXELINDEX * pTrans) {
  LCD_PIXELINDEX IndexMask, Index0, Index1, Pixel;

  Index0 = *(pTrans + 0);
  Index1 = *(pTrans + 1);
  x += Diff;
  switch (GUI_pContext->DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR)) {
  case 0:
    do {
      _SetPixelIndex(pDevice, x++, y, (*p & (0x80 >> Diff)) ? Index1 : Index0);
      if (++Diff == 8) {
        Diff = 0;
        p++;
      }
    } while (--xsize);
    break;
  case LCD_DRAWMODE_TRANS:
    do {
      if (*p & (0x80 >> Diff))
        _SetPixelIndex(pDevice, x, y, Index1);
      x++;
      if (++Diff == 8) {
        Diff = 0;
        p++;
      }
    } while (--xsize);
    break;
  case LCD_DRAWMODE_XOR | LCD_DRAWMODE_TRANS:
  case LCD_DRAWMODE_XOR:
    IndexMask = pDevice->pColorConvAPI->pfGetIndexMask();
    do {
      if (*p & (0x80 >> Diff)) {
        Pixel = _GetPixelIndex(pDevice, x, y);
        _SetPixelIndex(pDevice, x, y, Pixel ^ IndexMask);
      }
      x++;
      if (++Diff == 8) {
        Diff = 0;
        p++;
      }
    } while (--xsize);
    break;
  }
}

// Draw Bitmap 2 BPP
static void  _DrawBitLine2BPP(GUI_DEVICE * pDevice, int x, int y, U8 const GUI_UNI_PTR * p, int Diff, int xsize, const LCD_PIXELINDEX * pTrans) {
  LCD_PIXELINDEX Pixels, PixelIndex;
  int CurrentPixel, Shift, Index;

  Pixels = *p;
  CurrentPixel = Diff;
  x += Diff;
  switch (GUI_pContext->DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR)) {
  case 0:
    if (pTrans) {
      do {
        Shift = (3 - CurrentPixel) << 1;
        Index = (Pixels & (0xC0 >> (6 - Shift))) >> Shift;
        PixelIndex = *(pTrans + Index);
        _SetPixelIndex(pDevice, x++, y, PixelIndex);
        if (++CurrentPixel == 4) {
          CurrentPixel = 0;
          Pixels = *(++p);
        }
      } while (--xsize);
    } else {
      do {
        Shift = (3 - CurrentPixel) << 1;
        Index = (Pixels & (0xC0 >> (6 - Shift))) >> Shift;
        _SetPixelIndex(pDevice, x++, y, Index);
        if (++CurrentPixel == 4) {
          CurrentPixel = 0;
          Pixels = *(++p);
        }
      } while (--xsize);
    }
    break;
  case LCD_DRAWMODE_TRANS:
    if (pTrans) {
      do {
        Shift = (3 - CurrentPixel) << 1;
        Index = (Pixels & (0xC0 >> (6 - Shift))) >> Shift;
        if (Index) {
          PixelIndex = *(pTrans + Index);
          _SetPixelIndex(pDevice, x, y, PixelIndex);
        }
        x++;
        if (++CurrentPixel == 4) {
          CurrentPixel = 0;
          Pixels = *(++p);
        }
      } while (--xsize);
    } else {
      do {
        Shift = (3 - CurrentPixel) << 1;
        Index = (Pixels & (0xC0 >> (6 - Shift))) >> Shift;
        if (Index) {
          _SetPixelIndex(pDevice, x, y, Index);
        }
        x++;
        if (++CurrentPixel == 4) {
          CurrentPixel = 0;
          Pixels = *(++p);
        }
      } while (--xsize);
    }
    break;
  }
}

// Draw Bitmap 4 BPP
static void  _DrawBitLine4BPP(GUI_DEVICE * pDevice, int x, int y, U8 const GUI_UNI_PTR * p, int Diff, int xsize, const LCD_PIXELINDEX * pTrans) {
  LCD_PIXELINDEX Pixels, PixelIndex;
  int CurrentPixel, Shift, Index;

  Pixels = *p;
  CurrentPixel = Diff;
  x += Diff;
  switch (GUI_pContext->DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR)) {
  case 0:
    if (pTrans) {
      do {
        Shift = (1 - CurrentPixel) << 2;
        Index = (Pixels & (0xF0 >> (4 - Shift))) >> Shift;
        PixelIndex = *(pTrans + Index);
        _SetPixelIndex(pDevice, x++, y, PixelIndex);
        if (++CurrentPixel == 2) {
          CurrentPixel = 0;
          Pixels = *(++p);
        }
      } while (--xsize);
    } else {
      do {
        Shift = (1 - CurrentPixel) << 2;
        Index = (Pixels & (0xF0 >> (4 - Shift))) >> Shift;
        _SetPixelIndex(pDevice, x++, y, Index);
        if (++CurrentPixel == 2) {
          CurrentPixel = 0;
          Pixels = *(++p);
        }
      } while (--xsize);
    }
    break;
  case LCD_DRAWMODE_TRANS:
    if (pTrans) {
      do {
        Shift = (1 - CurrentPixel) << 2;
        Index = (Pixels & (0xF0 >> (4 - Shift))) >> Shift;
        if (Index) {
          PixelIndex = *(pTrans + Index);
          _SetPixelIndex(pDevice, x, y, PixelIndex);
        }
        x++;
        if (++CurrentPixel == 2) {
          CurrentPixel = 0;
          Pixels = *(++p);
        }
      } while (--xsize);
    } else {
      do {
        Shift = (1 - CurrentPixel) << 2;
        Index = (Pixels & (0xF0 >> (4 - Shift))) >> Shift;
        if (Index) {
          _SetPixelIndex(pDevice, x, y, Index);
        }
        x++;
        if (++CurrentPixel == 2) {
          CurrentPixel = 0;
          Pixels = *(++p);
        }
      } while (--xsize);
    }
    break;
  }
}

/*
**********************************************************************
*  8λɫ�ͼ����, ������Խ����Ż�
**********************************************************************
*/
static void  _DrawBitLine8BPP(GUI_DEVICE * pDevice, int x, int y, U8 const GUI_UNI_PTR * p, int xsize, const LCD_PIXELINDEX * pTrans) {
  LCD_PIXELINDEX Pixel;

  switch (GUI_pContext->DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR)) {
  case 0:
    // �˴����Ż�, ʹ�� lcd_chip_canvas, �� lcd_chip_wring...
    if (pTrans) {
        for (; xsize > 0; xsize--, x++, p++) {
          Pixel = *p;
          _SetPixelIndex(pDevice, x, y, *(pTrans + Pixel));
        }
    } else {
      for (; xsize > 0; xsize--, x++, p++) {
        _SetPixelIndex(pDevice, x, y, *p);
      }
    }
    break;
  case LCD_DRAWMODE_TRANS:
    if (pTrans) {
      for (; xsize > 0; xsize--, x++, p++) {
        Pixel = *p;
        if (Pixel) {
          _SetPixelIndex(pDevice, x, y, *(pTrans + Pixel));
        }
      }
    } else {
      for (; xsize > 0; xsize--, x++, p++) {
        Pixel = *p;
        if (Pixel) {
          _SetPixelIndex(pDevice, x, y, Pixel);
        }
      }
    }
    break;
  }
}

/*
**********************************************************************
*  16λɫ�ͼ����, ������Խ����Ż�
**********************************************************************
*/
static void _DrawBitLine16BPP(GUI_DEVICE * pDevice, int x, int y, U16 const GUI_UNI_PTR * p, int xsize, const LCD_PIXELINDEX * pTrans) {
  LCD_PIXELINDEX Pixel;

  switch (GUI_pContext->DrawMode & LCD_DRAWMODE_TRANS) {
  case 0:
    // �˴����Ż�, ʹ�� lcd_chip_canvas, �� lcd_chip_wring...
    if (pTrans) {
        for (; xsize > 0; xsize--, x++, p++) {
          Pixel = *p;
          _SetPixelIndex(pDevice, x, y, *(pTrans + Pixel));
        }
    } else {
      for (; xsize > 0; xsize--, x++, p++) {
        _SetPixelIndex(pDevice, x, y, *p);
      }
    }
    break;
  case LCD_DRAWMODE_TRANS:
    if (pTrans) {
      for (; xsize > 0; xsize--, x++, p++) {
        Pixel = *p;
        if (Pixel) {
          _SetPixelIndex(pDevice, x, y, *(pTrans + Pixel));
        }
      }
    } else {
      for (; xsize > 0; xsize--, x++, p++) {
        Pixel = *p;
        if (Pixel) {
          _SetPixelIndex(pDevice, x, y, Pixel);
        }
      }
    }
    break;
  }
}

/*
**********************************************************************
*  32λɫ�ͼ����, ������Խ����Ż�
**********************************************************************
*/
static void _DrawBitLine32BPP(GUI_DEVICE * pDevice, int x, int y, U32 const GUI_UNI_PTR * p, int xsize, const LCD_PIXELINDEX * pTrans) {
  LCD_PIXELINDEX Pixel;

  switch (GUI_pContext->DrawMode & LCD_DRAWMODE_TRANS) {
  case 0:
    // �˴����Ż�, ʹ�� lcd_chip_canvas, �� lcd_chip_wring...
    if (pTrans) {
        for (; xsize > 0; xsize--, x++, p++) {
          Pixel = *p;
          _SetPixelIndex(pDevice, x, y, *(pTrans + Pixel));
        }
    } else {
      for (; xsize > 0; xsize--, x++, p++) {
        _SetPixelIndex(pDevice, x, y, *p);
      }
    }
    break;
  case LCD_DRAWMODE_TRANS:
    if (pTrans) {
      for (; xsize > 0; xsize--, x++, p++) {
        Pixel = *p;
        if (Pixel) {
          _SetPixelIndex(pDevice, x, y, *(pTrans + Pixel));
        }
      }
    } else {
      for (; xsize > 0; xsize--, x++, p++) {
        Pixel = *p;
        if (Pixel) {
          _SetPixelIndex(pDevice, x, y, Pixel);
        }
      }
    }
    break;
  }
}

/*
**********************************************************************
*  ��ͼ�����, ������Եؽ��и߶��Ż�
**********************************************************************
*/
static void _DrawBitmap(GUI_DEVICE * pDevice, int x0, int y0,
                       int xSize, int ySize,
                       int BitsPerPixel,
                       int BytesPerLine,
                       const U8 GUI_UNI_PTR * pData, int Diff,
                       const LCD_PIXELINDEX* pTrans) {
  int i;

  switch (BitsPerPixel) {
  case 1:
    for (i = 0; i < ySize; i++) {
      _DrawBitLine1BPP(pDevice, x0, i + y0, pData, Diff, xSize, pTrans);
      pData += BytesPerLine;
    }
    break;
  case 2:
    for (i = 0; i < ySize; i++) {
      _DrawBitLine2BPP(pDevice, x0, i + y0, pData, Diff, xSize, pTrans);
      pData += BytesPerLine;
    }
    break;
  case 4:
    for (i = 0; i < ySize; i++) {
      _DrawBitLine4BPP(pDevice, x0, i + y0, pData, Diff, xSize, pTrans);
      pData += BytesPerLine;
    }
    break;
  case 8:
    for (i = 0; i < ySize; i++) {
      _DrawBitLine8BPP(pDevice, x0, i + y0, pData, xSize, pTrans);
      pData += BytesPerLine;
    }
    break;
  case 16:
    for (i = 0; i < ySize; i++) {
      _DrawBitLine16BPP(pDevice, x0, i + y0, (const U16 *)pData, xSize, pTrans);
      pData += BytesPerLine;
    }
    break;
  case 32:
    for (i = 0; i < ySize; i++) {
      _DrawBitLine32BPP(pDevice, x0, i + y0, (const U32 *)pData, xSize, pTrans);
      pData += BytesPerLine;
    }
    break;
  }
}

// _InitOnce
static int _InitOnce(GUI_DEVICE * pDevice) {
  DRIVER_CONTEXT * pContext;

  if (pDevice->u.pContext == NULL) {
    pDevice->u.pContext = GUI_ALLOC_GetFixedBlock(sizeof(DRIVER_CONTEXT));
    pContext = (DRIVER_CONTEXT *)pDevice->u.pContext;
    pContext->BitsPerPixel = LCD__GetBPP(pDevice->pColorConvAPI->pfGetIndexMask());
  }
  return pDevice->u.pContext ? 0 : 1;
}

// _GetDevProp
static I32 _GetDevProp(GUI_DEVICE * pDevice, int Index) {
  DRIVER_CONTEXT * pContext;

  pContext = (DRIVER_CONTEXT *)pDevice->u.pContext;
  switch (Index) {
  case LCD_DEVCAP_XSIZE:
    return pContext->xSize;
  case LCD_DEVCAP_YSIZE:
    return pContext->ySize;
  case LCD_DEVCAP_VXSIZE:
    return pContext->vxSize;
  case LCD_DEVCAP_VYSIZE:
    return pContext->vySize;
  case LCD_DEVCAP_BITSPERPIXEL:
    return pContext->BitsPerPixel;
  case LCD_DEVCAP_NUMCOLORS:
    return 0;
  case LCD_DEVCAP_XMAG:
    return 1;
  case LCD_DEVCAP_YMAG:
    return 1;
  case LCD_DEVCAP_MIRROR_X:
    return 0;
  case LCD_DEVCAP_MIRROR_Y:
    return 0;
  case LCD_DEVCAP_SWAP_XY:
    return 0;
  }
  return -1;
}

// _GetDevData
static void * _GetDevData(GUI_DEVICE * pDevice, int Index) {
  GUI_USE_PARA(pDevice);
  #if GUI_SUPPORT_MEMDEV
    switch (Index) {
    case LCD_DEVDATA_MEMDEV:
    // TBD: Has to be adapted to the right memory device depending on the used color depth!
      return (void *)&GUI_MEMDEV_DEVICE_16;
    }
  #else
    GUI_USE_PARA(Index);
  #endif
  return NULL;
}

// _GetRect
static void _GetRect(GUI_DEVICE * pDevice, LCD_RECT * pRect) {
  DRIVER_CONTEXT * pContext;

  pContext = (DRIVER_CONTEXT *)pDevice->u.pContext;
  pRect->x0 = 0;
  pRect->y0 = 0;
  pRect->x1 = pContext->vxSize - 1;
  pRect->y1 = pContext->vySize - 1;
}

// _SetOrg
static void _SetOrg(GUI_DEVICE * pDevice, int x, int y) {
  LCD_X_SETORG_INFO Data = {0};

  Data.xPos = x;
  Data.yPos = y;
  LCD_X_DisplayDriver(pDevice->LayerIndex, LCD_X_SETORG, (void *)&Data);
}

// _SetVRAMAddr
static void _SetVRAMAddr(GUI_DEVICE * pDevice, void * pVRAM) {
  DRIVER_CONTEXT * pContext;
  LCD_X_SETVRAMADDR_INFO Data = {0};

  _InitOnce(pDevice);
  if (pDevice->u.pContext) {
    pContext = (DRIVER_CONTEXT *)pDevice->u.pContext;
    pContext->VRAMAddr = (U32)pVRAM;
    Data.pVRAM = pVRAM;
    LCD_X_DisplayDriver(pDevice->LayerIndex, LCD_X_SETVRAMADDR, (void *)&Data);
  }
}

// _SetVSize
static void _SetVSize(GUI_DEVICE * pDevice, int xSize, int ySize) {
  DRIVER_CONTEXT * pContext;

  _InitOnce(pDevice);
  if (pDevice->u.pContext) {
    pContext = (DRIVER_CONTEXT *)pDevice->u.pContext;
    pContext->vxSize = xSize;
    pContext->vySize = ySize;
    pContext->vxSizePhys = xSize;
  }
}

// _SetSize
static void _SetSize(GUI_DEVICE * pDevice, int xSize, int ySize) {
  DRIVER_CONTEXT * pContext;
  LCD_X_SETSIZE_INFO Data = {0};

  _InitOnce(pDevice);
  if (pDevice->u.pContext) {
    pContext = (DRIVER_CONTEXT *)pDevice->u.pContext;
    pContext->vxSizePhys = (pContext->vxSizePhys == 0) ? xSize : pContext->vxSizePhys;
    pContext->xSize = xSize;
    pContext->ySize = ySize;
    Data.xSize = xSize;
    Data.ySize = ySize;
    LCD_X_DisplayDriver(pDevice->LayerIndex, LCD_X_SETSIZE, (void *)&Data);
  }
}

// _Init
static int  _Init(GUI_DEVICE * pDevice) {
  int r;

  r = _InitOnce(pDevice);
  r |= LCD_X_DisplayDriver(pDevice->LayerIndex, LCD_X_INITCONTROLLER, NULL);
  return r;
}

// _On
static void _On (GUI_DEVICE * pDevice) {
  LCD_X_DisplayDriver(pDevice->LayerIndex, LCD_X_ON, NULL);
}

// _Off
static void _Off (GUI_DEVICE * pDevice) {
  LCD_X_DisplayDriver(pDevice->LayerIndex, LCD_X_OFF, NULL);
}

// _SetLUTEntry
static void _SetLUTEntry(GUI_DEVICE * pDevice, U8 Pos, LCD_COLOR Color) {
  LCD_X_SETLUTENTRY_INFO Data = {0};

  Data.Pos   = Pos;
  Data.Color = Color;
  LCD_X_DisplayDriver(pDevice->LayerIndex, LCD_X_SETLUTENTRY, (void *)&Data);
}

// _GetDevFunc
static void (* _GetDevFunc(GUI_DEVICE ** ppDevice, int Index))(void) {
  GUI_USE_PARA(ppDevice);
  switch (Index) {
  case LCD_DEVFUNC_SET_VRAM_ADDR:
    return (void (*)(void))_SetVRAMAddr;
  case LCD_DEVFUNC_SET_VSIZE:
    return (void (*)(void))_SetVSize;
  case LCD_DEVFUNC_SET_SIZE:
    return (void (*)(void))_SetSize;
  case LCD_DEVFUNC_INIT:
    return (void (*)(void))_Init;
  case LCD_DEVFUNC_ON:
    return (void (*)(void))_On;
  case LCD_DEVFUNC_OFF:
    return (void (*)(void))_Off;
  case LCD_DEVFUNC_SETLUTENTRY:
    return (void (*)(void))_SetLUTEntry;
  }
  return NULL;
}

// GUI_DEVICE_API structure
const GUI_DEVICE_API LCDDriver_API = {
  // Data
  DEVICE_CLASS_DRIVER,

  // Drawing functions
  _DrawBitmap,
  _DrawHLine,
  _DrawVLine,
  _FillRect,
  _GetPixelIndex,
  _SetPixelIndex,
  _XorPixel,

  // Set origin
  _SetOrg,

  // Request information
  _GetDevFunc,
  _GetDevProp,
  _GetDevData,
  _GetRect,
};

#endif
/*************************** End of file ****************************/
