/**
  ******************************************************************************
  * @file    lcd_main.c
  * @author  mousie-yu
  * @version V2.0.0
  * @date    2012.2.22
  * @brief   LCD ������������. ����LCD��Դ����, ���ȵ���, ����, ��ͼ����.
  *          ��Ҫ LCDͨѶ���� �� LCDоƬ���� ֧��.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "lcd_main.h"

#ifdef LCD_POWER_NAME
    #include "drv_ctrlport.h"
#endif

#ifdef LCD_LIGHT_NAME
    #include "drv_ctrlport.h"
#endif



/** @addtogroup Components
  * @{
  */
/** @defgroup LCD_MAIN
  * @{
  */
#ifdef LCD_ENABLE



/**
  ******************************************************************************
  * @addtogroup LCD_MAIN_Configure
  ******************************************************************************
  * @{
  */
#include "drv_systick.h"
#define lcd_delay_ms(time)              delay_ms(time)                          ///< ����LCD������ʱ����
/**
  * @}
  */


/** @defgroup LCD_MAIN_Private_TypeDefine
  * @brief    ˽�����Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup LCD_MAIN_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup LCD_MAIN_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */

/**
  * @}
  */

/** @defgroup LCD_MAIN_Private_Function
  * @brief    ����˽�к���
  * @{
  */

/**
  * @}
  */



/** @defgroup LCD_MAIN_Function
  * @brief    ����ԭ�ļ�
  * @{
  */
/**
  ******************************************************************************
  * @brief  LCD �򿪵�Դ����ʼ������оƬ.
  *         ������Ļ�����ܶ�, ��һ����Ŀ����ʾ���̶��󲻱�.
  *         ��� ��Ļ��ʾ���� �Լ� ��ʾ���� �Ⱦ�����������ֲ�˴��޸�.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void lcd_init(void)
{
    lcd_bus_port_init();

#ifdef LCD_POWER_NAME                                                           // ��Դ��λ���ų�ʼ��
    ctrlport_init(LCD_POWER_NAME);
    ctrlport_off(LCD_POWER_NAME);
#endif

#ifdef LCD_LIGHT_NAME                                                           // �������ų�ʼ��
    ctrlport_init(LCD_LIGHT_NAME);
    ctrlport_off(LCD_LIGHT_NAME);
#endif

    lcd_light_off();
    lcd_power_off();
    delay_ms(10);
    lcd_power_on();
    delay_ms(10);
    lcd_chip_init();
    lcd_clear_all();
    lcd_light_on();

#ifdef LCD_FONT_ENABLE
    lcd_font_set_para(0, 0, LCD_BLACK, LCD_WHITE);
#endif
}

/**
  ******************************************************************************
  * @brief  LCD �򿪵�Դ.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void lcd_power_on(void)
{
#ifdef LCD_POWER_NAME
    ctrlport_on(LCD_POWER_NAME);
#endif
}

/**
  ******************************************************************************
  * @brief  LCD �رյ�Դ.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void lcd_power_off(void)
{
#ifdef LCD_POWER_NAME
    ctrlport_off(LCD_POWER_NAME);
#endif
}

/**
  ******************************************************************************
  * @brief  LCD �򿪱���.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void lcd_light_on(void)
{
#ifdef LCD_LIGHT_NAME
    ctrlport_on(LCD_LIGHT_NAME);
#endif
}

/**
  ******************************************************************************
  * @brief  LCD ���ڱ�������.
  * @param  level, ���ȵȼ�. 0-����, 0xFF-Ϩ��
  * @retval none
  ******************************************************************************
  */
void lcd_light_adjust(uint8_t level)
{
#ifdef LCD_LIGHT_NAME
    if (level < 0xFF) ctrlport_on(LCD_LIGHT_NAME);
    else              ctrlport_off(LCD_LIGHT_NAME);
#endif
}

/**
  ******************************************************************************
  * @brief  LCD �رձ���.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void lcd_light_off(void)
{
#ifdef LCD_LIGHT_NAME
    ctrlport_off(LCD_LIGHT_NAME);
#endif
}

/**
  ******************************************************************************
  * @brief  ת����ɫΪRGB565����
  * @param  red,   ��ɫɫֵ
  * @param  green, ��ɫɫֵ
  * @retval blue,  ��ɫɫֵ
  ******************************************************************************
  */
uint16_t lcd_rgb565(uint8_t red, uint8_t green, uint8_t blue)
{
    return (((uint16_t)red << 11) | (((uint16_t)green << 5) & 0x07E0) | (blue & 0x1F));
}

/**
  ******************************************************************************
  * @brief  ���LCDȫ��, ���
  * @param  None
  * @retval None
  ******************************************************************************
  */
void lcd_clear_all(void)
{
    lcd_fill_rectangle(0, 0, LCD_X_MAX, LCD_Y_MAX, LCD_BLACK);
}

/**
  ******************************************************************************
  * @brief  ��һ�����ص�
  * @param  x,      ���ص��X������
  * @param  y,      ���ص��Y������
  * @param  rgb565, ���ص����ɫֵ
  * @retval none
  ******************************************************************************
  */
void lcd_draw_pixel(uint16_t x, uint16_t y, uint16_t rgb565)
{
    if ((x >= LCD_X_MAX) || (y >= LCD_Y_MAX)) return;                           // Խ��ֱ�ӷ���
    lcd_chip_pixel_wr(x, y, rgb565);
}

/**
  ******************************************************************************
  * @brief  ���ߺ���
  * @param  x0, y0, ��������1
  * @param  x1, y1, ��������2
  * @param  rgb565, Ҫ��ʾ����ɫ
  * @retval none
  ******************************************************************************
  */
void lcd_draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t rgb565)
{
    uint16_t x,y;
    uint16_t dx;                                                                // = abs(x1 - x0);
    uint16_t dy;                                                                // = abs(y1 - y0);
    int16_t  p, _2dx, _2dy, _2dy_dx, _2dx_dy;

    // ���(x,y) �յ�(x1,y1), ����(dx, dy)
    if (x0 > x1)
    {
        dx = x0-x1;
        x  = x1;
        y  = y1;
        x1 = x0;
        y1 = y0;
    }
    else
    {
        dx = x1-x0;
        x  = x0;
        y  = y0;
    }

    if (y > y1) { dy = y-y1; }
    else        { dy = y1-y; }

    if (y == y1)                                                                // ˮƽ��
    {
        lcd_fill_rectangle(x, y, dx+1, 1, rgb565);
    }
    else if (x == x1)                                                           // ��ֱ��
    {
        lcd_fill_rectangle(x, y, 1, dy+1, rgb565);
    }
    else if (dx == dy)                                                          // 45��б��
    {
        for (++dx; dx>0; dx--)
        {
            lcd_draw_pixel(x++, y, rgb565);
            if (y>y1) y--;
            else      y++;
        }
    }
    else                                                                        // ����б��
    {
        lcd_draw_pixel(x, y, rgb565);
        if(y < y1)
        {
            if (dx > dy)
            {
                p       = 2 * dy - dx;
                _2dy    = 2 * dy;
                _2dy_dx = 2 * (dy - dx);
                while(x < x1)
                {
                    x++;
                    if (p < 0) { p+=_2dy; }
                    else       { y++; p+=_2dy_dx; }
                    lcd_draw_pixel(x, y, rgb565);
                }
            }
            else
            {
                p       = 2 * dx - dy;
                _2dx    = 2 * dx;
                _2dx_dy = 2 * (dx - dy);
                while(y < y1)
                {
                    y++;
                    if (p < 0) { p+=_2dx; }
                    else       { x++; p+=_2dx_dy; }
                    lcd_draw_pixel(x, y, rgb565);
                }
            }
        }
        else
        {
            if (dx > dy)
            {
                p       = 2 * dy - dx;
                _2dy    = 2 * dy;
                _2dy_dx = 2 * (dy - dx);
                while(x < x1)
                {
                    x++;
                    if (p < 0) { p+=_2dy;   }
                    else       { y--; p+=_2dy_dx; }
                    lcd_draw_pixel(x, y, rgb565);
                }
            }
            else
            {
                p       = 2 * dx - dy;
                _2dx    = 2 * dx;
                _2dx_dy = 2 * (dx - dy);
                while(y1 < y)
                {
                    y--;
                    if (p < 0) { p+=_2dx; }
                    else       { x++; p+=_2dx_dy; }
                    lcd_draw_pixel(x, y, rgb565);
                }
            }
        }
    }
}

/**
  ******************************************************************************
  * @brief  ��ʵ�ľ��κ���
  * @param  x, �������Ͻ�X������
  * @param  y, �������Ͻ�Y������
  * @param  w, ����X����
  * @param  l, ����Y����
  * @param  rgb565, Ҫ��ʾ����ɫ
  * @retval none
  ******************************************************************************
  */
void lcd_fill_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t l, uint16_t rgb565)
{
#ifdef LCD_QUICK
    uint32_t i;

    if ((x >= LCD_X_MAX) || (y >= LCD_Y_MAX) || (w == 0) || (l == 0)) return;
    if (x+w >= LCD_X_MAX) w = LCD_X_MAX-x;
    if (y+l >= LCD_Y_MAX) l = LCD_Y_MAX-y;

    lcd_chip_canvas(x, y, w, l);
    lcd_chip_wr_start();
    for (i = w*l; i>0; i--)
    {
        lcd_chip_wring(rgb565);
    }
    lcd_chip_wr_stop();

#else
    uint16_t i, j;

    if ((x >= LCD_X_MAX) || (y >= LCD_Y_MAX) || (w == 0) || (l == 0)) return;
    if (x+w >= LCD_X_MAX) w = LCD_X_MAX-x;
    if (y+l >= LCD_Y_MAX) l = LCD_Y_MAX-y;

    for (i=y; i<=y+l; i++)
    {
        for (j=x; j<=x+w; j++)
        {
            lcd_chip_pixel_wr(j, i, rgb565);
        }
    }

#endif
}

/**
  ******************************************************************************
  * @brief  �����ľ��κ���
  * @param  x, �������Ͻ�X������
  * @param  y, �������Ͻ�Y������
  * @param  w, ����X����
  * @param  l, ����Y����
  * @param  rgb565, Ҫ��ʾ����ɫ
  * @retval none
  ******************************************************************************
  */
void lcd_draw_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t l, uint16_t rgb565)
{
    lcd_fill_rectangle(x, y,     1, l, rgb565);
    lcd_fill_rectangle(x, y,     w, 1, rgb565);
    lcd_fill_rectangle(x, y+l-1, w, 1, rgb565);
    lcd_fill_rectangle(x+w-1, y, 1, l, rgb565);
}

/**
  ******************************************************************************
  * @brief  ��ʵ��Բ����
  * @param  cx, cy, Բ������
  * @param  r,      Բ�뾶, ��λΪ���ص�
  * @param  rgb565, Ҫ��ʾ����ɫ
  * @retval none
  ******************************************************************************
  */
void lcd_fill_circle(uint16_t cx, uint16_t cy, uint16_t r, uint16_t rgb565)
{
    uint16_t x,y,dx,dy;
    int16_t delta,tmp;

    x = 0;
    y = r;
    delta = 3-(r<<1);

    while (y >= x)
    {
        if (cx > x) dx = cx - x;
        else        dx = 0;
        if (cy > y) dy = cy - y;
        else        dy = 0;
        lcd_fill_rectangle(dx, cy+y, (x<<1)+1, 1, rgb565);
        lcd_fill_rectangle(dx, dy,   (x<<1)+1, 1,   rgb565);

        if (cx > y) dx = cx - y;
        else        dx = 0;
        if (cy > x) dy = cy - x;
        else        dy = 0;
        lcd_fill_rectangle(dx, cy+x, (y<<1)+1, 1, rgb565);
        lcd_fill_rectangle(dx, dy,   (y<<1)+1, 1,   rgb565);

        x++;
        if (delta >= 0)
        {
            y--;
            tmp    = (x<<2);
            tmp   -= (y<<2);
            delta += (tmp+10);
        }
        else
        {
            delta += ((x<<2)+6);
        }
    }
}

/**
  ******************************************************************************
  * @brief  ������Բ����
  * @param  cx, cy, Բ������
  * @param  r,      Բ�뾶, ��λΪ���ص�
  * @param  rgb565, Ҫ��ʾ����ɫ
  * @retval none
  ******************************************************************************
  */
void lcd_draw_circle(uint16_t cx, uint16_t cy, uint16_t r, uint16_t rgb565)
{
    uint16_t x,y;
    int16_t delta,tmp;

    x = 0;
    y = r;
    delta = 3-(r<<1);

    while (y >= x)
    {
        lcd_draw_pixel(cx+x, cy+y, rgb565);
        lcd_draw_pixel(cx+y, cy+x, rgb565);
        lcd_draw_pixel(cx-x, cy+y, rgb565);
        lcd_draw_pixel(cx+x, cy-y, rgb565);
        lcd_draw_pixel(cx-y, cy+x, rgb565);
        lcd_draw_pixel(cx+y, cy-x, rgb565);
        lcd_draw_pixel(cx-x, cy-y, rgb565);
        lcd_draw_pixel(cx-y, cy-x, rgb565);

        x++;
        if (delta >= 0)
        {
            y--;
            tmp    = (x<<2);
            tmp   -= (y<<2);
            delta += (tmp+10);
        }
        else
        {
            delta += ((x<<2)+6);
        }
    }
}

#ifdef LCD_QUICK
/**
  ******************************************************************************
  * @brief  LCDָ����������
  * @param  x, �������Ͻ�X������
  * @param  y, �������Ͻ�Y������
  * @param  w, ����X����
  * @param  l, ����Y����
  * @retval none
  ******************************************************************************
  */
void lcd_paint_canvas(uint16_t x, uint16_t y, uint16_t w, uint16_t l)
{
    if ((x >= LCD_X_MAX) || (y >= LCD_Y_MAX) || (w == 0) || (l == 0))           // ��������, �޶���һ��������
    {
        x = LCD_X_MAX;
        y = LCD_Y_MAX;
        w = 1;
        l = 1;
    }
    lcd_chip_canvas(x, y, w, l);

}
/**
  ******************************************************************************
  * @brief  LCD����������ʾ��ɫ, rgb565
  * @param  *rgb565, ��ɫָ��, 16bit��ʾһ�����ص�, �Զ�����
  * @param  num,     16bit ��ɫ��ɫ����
  * @retval none
  ******************************************************************************
  */
void lcd_paint_rgb565(uint16_t *rgb565, uint32_t num)
{
    lcd_chip_wr_start();
    while (num--)
    {
        lcd_chip_wring(*rgb565++);
    }
    lcd_chip_wr_stop();
}
#endif

/**
  ******************************************************************************
  * @brief  LCD��ʾһ��ͼ��, rgb565
  * @param  x, ͼ�����Ͻ�X������
  * @param  y, ͼ�����Ͻ�Y������
  * @param  w, ͼ��X����
  * @param  l, ͼ��Y����
  * @param  *rgb565, ��ɫָ��, 16bit��ʾһ�����ص�, �Զ�����
  * @retval none
  ******************************************************************************
  */
void lcd_paint_image(uint16_t x, uint16_t y, uint16_t w, uint16_t l, uint16_t *rgb565)
{
#ifdef LCD_QUICK
    uint16_t num = w*l;
    lcd_paint_canvas(x, y, w, l);
    lcd_paint_rgb565(rgb565, num);

#else
    uint16_t x1, w1;
    for (; l>0; l--)
    {
        for (x1=x, w1=w; w1>0; w1--)
        {
            lcd_draw_pixel(x1++, y, *rgb565++);
        }
        ++y;
    }

#endif
}

/**
  ******************************************************************************
  * @brief  ��ȡһ�����ص���ɫ
  * @param  x,      ���ص��X������
  * @param  y,      ���ص��Y������
  * @retval none
  ******************************************************************************
  */
uint16_t lcd_absorb_pixel(uint16_t x, uint16_t y)
{
    if ((x >= LCD_X_MAX) || (y >= LCD_Y_MAX)) return 0;
    return lcd_chip_pixel_rd(x, y);
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
