/**
  ******************************************************************************
  * @file    lcd_main.h
  * @author  mousie-yu
  * @version V2.0.0
  * @date    2012.2.22
  * @brief   LCD ��������.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_MAIN_H
#define __LCD_MAIN_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup LCD_MAIN
  * @{
  */
/**
  ******************************************************************************
  * @defgroup LCD_MAIN_Configure
  * @brief    �û�����
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // Ӳ�����������ļ�

//// LCD����ʹ��
//#define LCD_ENABLE                                                              ///< ʹ��LCD
//#define LCD_FONT_ENABLE                                                         ///< ʹ��LCD������ʾ����
//
//// LCD ����оƬѡ��, ע��ֻ��ѡһ��
////#define LCD_CHIP_ILI9320
////#define LCD_CHIP_ILI9325
////#define LCD_CHIP_HX8347
//
//// LCD �ֱ�������
//#define LCD_X_MAX                       240                                     ///< X��ֱ���
//#define LCD_Y_MAX                       320                                     ///< Y��ֱ���
//
//// LCD ��Դ�������������
////#define LCD_POWER_NAME                  LCD_POWER                               ///< ��Դ���ƻ�Ӹ�λ����. �����򲻶���, ʹ����drv_ctrlport����
////#define LCD_LIGHT_NAME                  LCD_LIGHT                               ///< LCD�������. �����򲻶���, ʹ����drv_ctrlport����
//
///**
//  * LCDͨѶ����, ע��ֻ��ѡһ��! ���Ŷ�Ӧ��ϵ����ͼ:
//  * +------------------------------------------------------------------------+
//  * |                            Pin assignment                              |
//  * +---------------------+----------------+----------+----------------------|
//  * |  STM32 Pins         | STM32 FSMC     | LCD Pins |  describe            |
//  * +---------------------+----------------+----------+----------------------+
//  * |  LCD_DATA_PIN[15:0] | FSMC_D[15:0]   | DB[15:0] |  16bit Data          |
//  * |  LCD_CS_PIN         | FSMC_NE1       | CS       |  low, chip selection |
//  * |  LCD_RS_PIN         | FSMC_A16       | RS       |  low/high, reg/data  |
//  * |  LCD_WR_PIN         | FSMC_NWE       | WR       |  low, write enable   |
//  * |  LCD_RD_PIN         | FSMC_NOE       | RD       |  low, read enable    |
//  * +---------------------+----------------+----------+----------------------+
//  */
////#define LCD_BUS_FSMC                                                            ///< ʹ��FSMC�ӿ�, ��LCD��ΪRAM����
////#define LCD_BUS_8080                                                            ///< ʹ��IO��ģ��INTE 80800��������
//
//#ifdef LCD_BUS_8080
//    #define LCD_DATA_PORT               GPIOE                                   ///< LCD 16bit�����ߵ�PORT��
//    #define LCD_DATA_CLK                RCC_APB2Periph_GPIOE                    ///< LCD 16bit�����ߵ�ʱ��ģ��
//
//    #define LCD_CS_PORT                 GPIOD                                   ///< LCDƬѡ�źŵ�PORT��
//    #define LCD_CS_CLK                  RCC_APB2Periph_GPIOD                    ///< LCDƬѡ�źŵ�ʱ��ģ��
//    #define LCD_CS_PIN                  GPIO_Pin_12                             ///< LCDƬѡ�źŵ�PIN��
//
//    #define LCD_RS_PORT                 GPIOD                                   ///< LCD�Ĵ���/�����źŵ�PORT��
//    #define LCD_RS_CLK                  RCC_APB2Periph_GPIOD                    ///< LCD�Ĵ���/�����źŵ�ʱ��ģ��
//    #define LCD_RS_PIN                  GPIO_Pin_13                             ///< LCD�Ĵ���/�����źŵ�PIN��
//
//    #define LCD_WR_PORT                 GPIOD                                   ///< LCDд�źŵ�PORT��
//    #define LCD_WR_CLK                  RCC_APB2Periph_GPIOD                    ///< LCDд�źŵ�ʱ��ģ��
//    #define LCD_WR_PIN                  GPIO_Pin_14                             ///< LCDд�źŵ�PIN��
//
//    #define LCD_RD_PORT                 GPIOD                                   ///< LCD���źŵ�PORT��
//    #define LCD_RD_CLK                  RCC_APB2Periph_GPIOD                    ///< LCD���źŵ�ʱ��ģ��
//    #define LCD_RD_PIN                  GPIO_Pin_15                             ///< LCD���źŵ�PIN��
//#endif



#ifdef LCD_ENABLE
            // LCD����оƬͨѶ��ʽͷ�ļ�
    #ifdef LCD_BUS_8080
        #include "lcd_bus_8080.h"
    #endif
    #ifdef LCD_BUS_FSMC
        #include "lcd_bus_fsmc.h"
    #endif

    // LCD����оƬԤ����
    #ifdef LCD_CHIP_ILI9320
        #define LCD_QUICK                                                           ///< ʹ��LCD���ٻ�ͼ
        #include "lcd_chip_ili9320.h"
    #endif
    #ifdef LCD_CHIP_ILI9325
        #define LCD_QUICK                                                           ///< ʹ��LCD���ٻ�ͼ
        #include "lcd_chip_ili9325.h"
    #endif
    #ifdef LCD_CHIP_HX8347
        #define LCD_QUICK                                                           ///< ʹ��LCD���ٻ�ͼ
        #include "lcd_chip_hx8347.h"
    #endif

#else
    #undef LCD_FONT_ENABLE

#endif
/**
  * @}
  */



/** @defgroup LCD_MAIN_Public_TypeDefine
  * @brief    �������Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup LCD_MAIN_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */
// ������ɫ����
#define LCD_BLACK                       ((uint16_t)0x0000)
#define LCD_RED                         ((uint16_t)0xF800)
#define LCD_GREEN                       ((uint16_t)0x07E0)
#define LCD_BLUE                        ((uint16_t)0x001F)
#define LCD_YELLOW                      (LCD_RED   | LCD_GREEN)
#define LCD_MAGENTA                     (LCD_RED   | LCD_BLUE)
#define LCD_CYAN                        (LCD_GREEN | LCD_BLUE)
#define LCD_WHITE                       (LCD_RED   | LCD_GREEN | LCD_BLUE)

#define LCD_TRANSPARENT                 ((uint32_t)0xFFFFFFFF)                  // ���屳��Ϊ͸��ɫ
/**
  * @}
  */

/** @defgroup LCD_MAIN_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */

/**
  * @}
  */

/** @defgroup LCD_MAIN_Public_Function
  * @brief    ���幫�к���
  * @{
  */
#ifdef LCD_ENABLE

// LCD���ƺ���
void lcd_init(void);                                                            // LCD��ʼ��, �򿪵�Դ�ͱ���

void lcd_power_on(void);                                                        // �򿪵�Դ
void lcd_power_off(void);                                                       // �رյ�Դ
void lcd_light_on(void);                                                        // �򿪱���
void lcd_light_adjust(uint8_t level);                                           // �������ȵ���
void lcd_light_off(void);                                                       // �رձ���

// LCD��������
uint16_t lcd_rgb565(uint8_t red, uint8_t green, uint8_t blue);                                      // ����ɫתΪRGB565��ʽ

// LCD��ͼ����
void lcd_clear_all(void);                                                                           // ���LCDȫ��, ���

void lcd_draw_pixel(uint16_t x, uint16_t y, uint16_t rgb565);                                       // ��һ�����ص�
void lcd_draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t rgb565);            // ��һ����
void lcd_draw_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t l, uint16_t rgb565);           // �����ľ���
void lcd_fill_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t l, uint16_t rgb565);           // ��ʵ�ľ���
void lcd_draw_circle(uint16_t cx, uint16_t cy, uint16_t r, uint16_t rgb565);                        // ������Բ
void lcd_fill_circle(uint16_t cx, uint16_t cy, uint16_t r, uint16_t rgb565);                        // ��ʵ��Բ

void lcd_paint_image(uint16_t x, uint16_t y, uint16_t w, uint16_t l, uint16_t *rgb565);             // ��һ��ͼ, rgb565
#ifdef LCD_QUICK
	void lcd_paint_canvas(uint16_t x, uint16_t y, uint16_t w, uint16_t l);                          // ָ����������
	void lcd_paint_rgb565(uint16_t *rgb565, uint32_t num);                                          // ��ʾ��ɫ, rgb565
#endif

// LCD������ʾ����
#ifdef LCD_FONT_ENABLE
	void lcd_font_set_para(uint16_t x, uint16_t y, uint32_t back_color, uint16_t fore_color);       // ����������ʾ����
    void lcd_put_font(uint16_t incode);                                                             // ��ʾһ���ַ�
    void lcd_printf(char *pstring, ...);                                                            // LCD��ӡ����
#endif

// LCD��ɫ��ȡ����, ��δȫ����ͨ, ����Ժ�ʹ��
uint16_t lcd_absorb_pixel(uint16_t x, uint16_t y);                                                  // ��ȡһ�����ص����ɫ

#endif
/**
  * @}
  */



/**
  * @}
  */
/**
  * @}
  */

#endif
/* END OF FILE ---------------------------------------------------------------*/
