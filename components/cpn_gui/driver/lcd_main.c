/**
  ******************************************************************************
  * @file    lcd_main.c
  * @author  mousie-yu
  * @version V2.1.0
  * @date    2013.9.20
  * @brief   LCD ������������. ����LCD��Դ����, ���ȵ���, �������Ժ���.
  *          ���� emwin, ����������!
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "lcd_main.h"
#include "GUI.h"
#include "drv_systick.h"
#include "stm32f10x_rcc.h"

#ifdef LCD_POWER_NAME
    #include "drv_ctrlport.h"
#endif

#ifdef LCD_LIGHT_NAME
    #include "drv_ctrlport.h"
#endif



/** @addtogroup Components
  * @{
  */
/** @defgroup CPN_GUI
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
// ������ɫ����
#define LCD_TEST_BLACK                  ((uint16_t)0x0000)
#define LCD_TEST_RED                    ((uint16_t)0xF800)
#define LCD_TEST_GREEN                  ((uint16_t)0x07E0)
#define LCD_TEST_BLUE                   ((uint16_t)0x001F)
#define LCD_TEST_YELLOW                 (LCD_TEST_RED   | LCD_TEST_GREEN)
#define LCD_TEST_MAGENTA                (LCD_TEST_RED   | LCD_TEST_BLUE)
#define LCD_TEST_CYAN                   (LCD_TEST_GREEN | LCD_TEST_BLUE)
#define LCD_TEST_WHITE                  (LCD_TEST_RED   | LCD_TEST_GREEN | LCD_TEST_BLUE)
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
  * @brief  LCD IO�ڼ�����оƬ��ʼ������.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void lcd_init(void)
{
    // �õ���ST������emwin��, ��������ʹ��CRC�ж��Ƿ�ΪSTоƬ�Ĵ���, ��˱���ʹ��CRC!
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
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
    lcd_light_on();
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
  * @brief  ����һ�����ص���ɫ
  * @param  x,      ���ص��X������
  * @param  y,      ���ص��Y������
  * @param  rgb565, ���ص����ɫֵ
  * @retval none
  ******************************************************************************
  */
void lcd_set_pixel(uint16_t x, uint16_t y, uint16_t rgb565)
{
    lcd_chip_pixel_wr(x, y, rgb565);
}

/**
  ******************************************************************************
  * @brief  ��ȡһ�����ص���ɫ
  * @param  x,      ���ص��X������
  * @param  y,      ���ص��Y������
  * @retval none
  ******************************************************************************
  */
uint16_t lcd_get_pixel(uint16_t x, uint16_t y)
{
    return lcd_chip_pixel_rd(x, y);
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

    lcd_chip_canvas(x, y, w, l);
    lcd_chip_wr_start();
    for (i = w*l; i>0; i--)
    {
        lcd_chip_wring(rgb565);
    }
    lcd_chip_wr_stop();

#else
    uint16_t i, j;

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
  * @brief  LCD �������Ժ���.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void lcd_test_driver(void)
{
    uint16_t i, j;
    uint16_t rgb;

    // LCD��ʼ��
    lcd_init();

    // lcd_main ��������
    lcd_fill_rectangle(0, 0, 100, 100, LCD_TEST_MAGENTA);
    // ���㺯��
    for (i=0; i<10; i++)
    {
        for (j=10; j<20; j++)
        {
            lcd_set_pixel(i+10,j,LCD_TEST_WHITE);
            lcd_set_pixel(i+20,j,LCD_TEST_BLACK);
            lcd_set_pixel(i+30,j,LCD_TEST_RED);
            lcd_set_pixel(i+40,j,LCD_TEST_GREEN);
            lcd_set_pixel(i+50,j,LCD_TEST_BLUE);
            lcd_set_pixel(i+60,j,LCD_TEST_WHITE);
            lcd_set_pixel(i+70,j,LCD_TEST_BLACK);
        }
    }
    // ��ɫ��ȡ����ʾ
    for (i=1; i<8; i++)
    {
        rgb = lcd_get_pixel(5+10*i, 10);
        lcd_fill_rectangle(10*i, 30, 10, 10, rgb);
    }

    delay_ms(2000);

    // emWIN����
    GUI_Init();
    GUI_SetBkColor(GUI_BLUE);
    GUI_Clear();
    GUI_GotoXY(10, 20);
    GUI_DispString("emWIN BLUE");
    // ע��, ���ʴ�С����Ϊ1���ص�, XORģʽ����Ч!
    GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);
    GUI_FillCircle(120, 60, 40);
    GUI_SetDrawMode(GUI_DRAWMODE_XOR);
    GUI_FillCircle(140, 90, 40);
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
