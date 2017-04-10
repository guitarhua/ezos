/**
  ******************************************************************************
  * @file    lcdgui_conf.h
  * @author  mousie
  * @version V1.0.0
  * @date    2013.9.21
  * @brief   LCD�ײ����� �� emWIN GUI ����.
  ******************************************************************************
  */
#ifndef __LCDGUI_CONF_H__
#define __LCDGUI_CONF_H__



/*-----------------------------------------------------------------------------/
/ LCD GUI ��������
/-----------------------------------------------------------------------------*/
#define LCD_ENABLE                                                              ///< ʹ��LCD

#define GUI_SUPPORT_TOUCH               0                                       ///< ֧�ִ�����
#define GUI_SUPPORT_MOUSE               0                                       ///< ֧�����
#define GUI_WINSUPPORT                  1                                       ///< ֧�ִ��ڹ�������widgets
#define GUI_SUPPORT_AA                  1                                       ///< ֧�ֿ���ݹ���

#define GUI_SUPPORT_MEMDEV              1                                       ///< ֧�ִ洢�豸
#define GUI_NUMBYTES                    (1024) * 40                             ///< emWIN�ڴ������, ����4K����
#define GUI_BLOCKSIZE                   0x80                                    ///< �ڴ���С, ����ֵ32-1024. �����д�����ͼ, ��ѡ��С��ֵ. ������ͼ���ѹ, ��ѡ��ֵ.
#define GUI_DEFAULT_FONT                &GUI_Font6x8                            ///< �趨Ĭ������

/*-----------------------------------------------------------------------------/
/ LCD �� ��������
/-----------------------------------------------------------------------------*/
#define XSIZE_PHYS                      240                                     ///< ����X������, ����
#define YSIZE_PHYS                      320                                     ///< ����Y������, ����
#define VXSIZE_PHYS                     XSIZE_PHYS                              ///< ������ĻX��, һ���������Ļ����һ��
#define VYSIZE_PHYS                     YSIZE_PHYS                              ///< ������ĻY��, һ���������Ļ����һ��

#define COLOR_CONVERSION                GUICC_565                               ///< ɫ��ת��ģʽ, ��Ļ16λɫRGBΪ GUICC_M565, BGRΪ GUICC_565

// �������¹��ܻ�Ӱ��ˢ���ٶ�, ����ֱ���޸� LCD �ײ�����, ʹ����XY���ʵ��XY��һ��!!!
#define LCD_MIRROR_X                    0                                       ///< LCD ʵ��X�᾵��
#define LCD_MIRROR_Y                    0                                       ///< LCD ʵ��Y�᾵��
#define LCD_SWAP_XY                     0                                       ///< LCD ʵ��XY�ụ��

#define TOUCH_MIRROR_X                  0                                       ///< ������ʵ��X�᾵��
#define TOUCH_MIRROR_Y                  0                                       ///< ������ʵ��Y�᾵��
#define TOUCH_SWAP_XY                   0                                       ///< ������ʵ��XY�ụ��



#ifdef LCD_ENABLE
/*-----------------------------------------------------------------------------/
/ LCD�ײ���������
/-----------------------------------------------------------------------------*/
    #include "stm32f10x_conf.h"                                                 // Ӳ�����������ļ�

    // LCD ����оƬѡ��, ע��ֻ��ѡһ��
    #define LCD_CHIP_ILI9320
    //#define LCD_CHIP_ILI9325
    //#define LCD_CHIP_HX8347

    // LCD ��Դ�������������
    //#define LCD_POWER_NAME                  LCD_POWER                         ///< ��Դ���ƻ�Ӹ�λ����. �����򲻶���, ʹ����drv_ctrlport����
    //#define LCD_LIGHT_NAME                  LCD_LIGHT                         ///< LCD�������. �����򲻶���, ʹ����drv_ctrlport����

    /**
      * LCDͨѶ����, ע��ֻ��ѡһ��! ���Ŷ�Ӧ��ϵ����ͼ:
      * +------------------------------------------------------------------------+
      * |                            Pin assignment                              |
      * +---------------------+----------------+----------+----------------------|
      * |  STM32 Pins         | STM32 FSMC     | LCD Pins |  describe            |
      * +---------------------+----------------+----------+----------------------+
      * |  LCD_DATA_PIN[15:0] | FSMC_D[15:0]   | DB[15:0] |  16bit Data          |
      * |  LCD_CS_PIN         | FSMC_NE1       | CS       |  low, chip selection |
      * |  LCD_RS_PIN         | FSMC_A16       | RS       |  low/high, reg/data  |
      * |  LCD_WR_PIN         | FSMC_NWE       | WR       |  low, write enable   |
      * |  LCD_RD_PIN         | FSMC_NOE       | RD       |  low, read enable    |
      * +---------------------+----------------+----------+----------------------+
      */
    #define LCD_BUS_8080                                                        ///< ʹ��IO��ģ��INTE 80800��������
    //#define LCD_BUS_FSMC                                                        ///< ʹ��FSMC�ӿ�, ��LCD��ΪRAM����

    #ifdef LCD_BUS_8080
        #define LCD_DATA_PORT           GPIOE                                   ///< LCD 16bit�����ߵ�PORT��
        #define LCD_DATA_CLK            RCC_APB2Periph_GPIOE                    ///< LCD 16bit�����ߵ�ʱ��ģ��

        #define LCD_CS_PORT             GPIOC                                   ///< LCDƬѡ�źŵ�PORT��
        #define LCD_CS_CLK              RCC_APB2Periph_GPIOC                    ///< LCDƬѡ�źŵ�ʱ��ģ��
        #define LCD_CS_PIN              GPIO_Pin_6                              ///< LCDƬѡ�źŵ�PIN��

        #define LCD_RS_PORT             GPIOD                                   ///< LCD�Ĵ���/�����źŵ�PORT��
        #define LCD_RS_CLK              RCC_APB2Periph_GPIOD                    ///< LCD�Ĵ���/�����źŵ�ʱ��ģ��
        #define LCD_RS_PIN              GPIO_Pin_13                             ///< LCD�Ĵ���/�����źŵ�PIN��

        #define LCD_WR_PORT             GPIOD                                   ///< LCDд�źŵ�PORT��
        #define LCD_WR_CLK              RCC_APB2Periph_GPIOD                    ///< LCDд�źŵ�ʱ��ģ��
        #define LCD_WR_PIN              GPIO_Pin_14                             ///< LCDд�źŵ�PIN��

        #define LCD_RD_PORT             GPIOD                                   ///< LCD���źŵ�PORT��
        #define LCD_RD_CLK              RCC_APB2Periph_GPIOD                    ///< LCD���źŵ�ʱ��ģ��
        #define LCD_RD_PIN              GPIO_Pin_15                             ///< LCD���źŵ�PIN��
    #endif

    // LCD����оƬͨѶ��ʽͷ�ļ�
    #ifdef LCD_BUS_8080
        #include "lcd_bus_8080.h"
    #endif
    #ifdef LCD_BUS_FSMC
        #include "lcd_bus_fsmc.h"
    #endif

    // LCD����оƬԤ����
    #ifdef LCD_CHIP_ILI9320
        #define LCD_QUICK                                                       ///< ʹ��LCD���ٻ�ͼ
        #include "lcd_chip_ili9320.h"
    #endif
    #ifdef LCD_CHIP_ILI9325
        #define LCD_QUICK                                                       ///< ʹ��LCD���ٻ�ͼ
        #include "lcd_chip_ili9325.h"
    #endif
    #ifdef LCD_CHIP_HX8347
        #define LCD_QUICK                                                       ///< ʹ��LCD���ٻ�ͼ
        #include "lcd_chip_hx8347.h"
    #endif
#endif // LCD_ENABLE



#if GUI_SUPPORT_TOUCH && (defined LCD_ENABLE)
/*-----------------------------------------------------------------------------/
/ �����ײ���������
/-----------------------------------------------------------------------------*/
    // ����оƬͨѶ����, ��Ϊ SPI / I2C
    #define TOUCH_SPI_NAME                  TOUCH_SPI                           ///< ��������ʹ��һ��SPI, SPI��SCK, MISO, MOSI����"drv_spi.h"������
    #define TOUCH_CS_PORT                   GPIOC                               ///< ��������оƬƬѡ�źŵ�PORT��
    #define TOUCH_CS_CLK                    RCC_APB2Periph_GPIOC                ///< ��������оƬƬѡ�źŵ�ʱ��ģ��
    #define TOUCH_CS_PIN                    GPIO_Pin_8                          ///< ��������оƬƬѡ�źŵ�PIN��

    // ��������
    #define TOUCH_AD_LEFT                   400                                 ///< ��������߲���ֵ
    #define TOUCH_AD_RIGHT                  4000                                ///< �������ұ߲���ֵ
    #define TOUCH_AD_TOP                    400                                 ///< ���������߲���ֵ
    #define TOUCH_AD_BOTTOM                 4000                                ///< �������ױ߲���ֵ

    // ѡ����������
    #include "touch_chip_tsc2046.h"
#endif // LCD_ENABLE && GUI_SUPPORT_TOUCH



/*-----------------------------------------------------------------------------/
/ GUI�ܳ�ʼ������
/-----------------------------------------------------------------------------*/
extern void gui_main_init(void);                                                ///< gui�ܳ�ʼ������, ��׼����gui���������.



#endif // __LCDGUI_CONF_H__

/** @} */
/** @} */
