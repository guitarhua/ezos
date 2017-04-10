/**
  ******************************************************************************
  * @file    lcd_font.c
  * @author  mousie-yu
  * @version V2.0.0
  * @date    2012.2.22
  * @brief   LCD ������ʾ��������.
  *          ��Ҫ LCD���������� ֧��.
  *          ��ģ���ɿ�ʹ�ù��� FontLibrary �� PCtoLCD.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "lcd_font.h"
#include <stdarg.h>



/** @addtogroup Components
  * @{
  */
/** @defgroup LCD_FONT
  * @{
  */
#ifdef LCD_FONT_ENABLE



/**
  ******************************************************************************
  * @addtogroup LCD_FONT_Configure
  ******************************************************************************
  * @{
  */

/**
  * @}
  */


/** @defgroup LCD_FONT_Private_TypeDefine
  * @brief    ˽�����Ͷ���
  * @{
  */
/// ��ģ�������Ͷ���
typedef struct
{
    uint32_t back;                                                              ///< ��ģ����ɫ, >0xFFFF Ϊ͸��ɫ
    uint16_t fore;                                                              ///< ��ģǰ��ɫ
    uint16_t curx;                                                              ///< ��ģ��ǰX����
    uint16_t cury;                                                              ///< ��ģ��ǰY����
    uint16_t byte;                                                              ///< ������ģռ�õ��ֽ���
    uint8_t  mode;                                                              ///< ��ģȡģ��ʽ
    uint8_t  width;                                                             ///< ��ģ���
    uint8_t  heigth;                                                            ///< ��ģ�߶�
} typehead_para_t;
/**
  * @}
  */

/** @defgroup LCD_FONT_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup LCD_FONT_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */
static typehead_para_t typehead;
static const uint8_t * const hex2char_lowercase = "0123456789abcdef";           ///< ����ת�ַ�������, Сд��ĸ
static const uint8_t * const hex2char_capital   = "0123456789ABCDEF";           ///< ����ת�ַ�������, ��д��ĸ
/**
  * @}
  */

/** @defgroup LCD_FONT_Private_Function
  * @brief    ����˽�к���
  * @{
  */
extern uint8_t lcd_load_typehead(uint16_t incode, uint8_t *pdata);
/**
  * @}
  */



/** @defgroup LCD_FONT_Function
  * @brief    ����ԭ�ļ�
  * @{
  */
/**
  ******************************************************************************
  * @brief  ����������ʾ����, ������ʼλ��, ǰ��ɫ, ����ɫ
  * @param  x, x����ʾλ��
  * @param  y, y����ʾλ��
  * @param  back_color, ���屳��ɫ, rgb565��ʽ, >0xFFFF Ϊ͸��ɫ
  * @param  fore_color, ����ǰ��ɫ, rgb565��ʽ.
  * @retval None
  ******************************************************************************
  */
void lcd_font_set_para(uint16_t x, uint16_t y, uint32_t back_color, uint16_t fore_color)
{
    typehead.curx = x;
    typehead.cury = y;
    typehead.back = back_color;
    typehead.fore = fore_color;
}

/**
  ******************************************************************************
  * @brief  ������ģ�����ȡģ����
  * @param  width,  ��ģ������
  * @param  heigth, ��ģ���󳤶�
  * @param  type,   ��ģ����
  *   @arg  FONT_MOULD_TYPE_X_MSB,      ����ʽ, ��λ��ǰȡģ
  *   @arg  FONT_MOULD_TYPE_X_LSB,      ����ʽ, ��λ��ǰȡģ
  *   @arg  FONT_MOULD_TYPE_XY_MSB,     ����ʽ, ��λ��ǰȡģ
  *   @arg  FONT_MOULD_TYPE_XY_LSB,     ����ʽ, ��λ��ǰȡģ
  *   @arg  FONT_MOULD_TYPE_Y_MSB,      ����ʽ, ��λ��ǰȡģ
  *   @arg  FONT_MOULD_TYPE_Y_LSB,      ����ʽ, ��λ��ǰȡģ
  *   @arg  FONT_MOULD_TYPE_YX_MSB,     ����ʽ, ��λ��ǰȡģ
  *   @arg  FONT_MOULD_TYPE_YX_LSB,     ����ʽ, ��λ��ǰȡģ
  * @retval None
  ******************************************************************************
  */
void lcd_set_typehead(uint8_t width, uint8_t heigth, typehead_mode_t mode)
{
    typehead.width  = width;
    typehead.heigth = heigth;
    typehead.mode   = mode;
    if (mode & 0x02) typehead.byte = ((heigth-1)/8 + 1) * width;                // ����ʽ������ʽ
    else             typehead.byte = ((width-1)/8 + 1) * heigth;                // ����ʽ������ʽ
}

/**
  ******************************************************************************
  * @brief  ��ȡһ����ģռ�õ��ֽ���
  * @param  None
  * @retval ��ģ�ֽ���
  ******************************************************************************
  */
uint8_t lcd_get_typehead_byte(void)
{
    return typehead.byte;
}

/**
  ******************************************************************************
  * @brief  ������ģ��ַ, LCD��ʾһ������
  * @param  pdata, ��ģ��ַ
  * @retval None
  ******************************************************************************
  */
void lcd_put_typehead(const uint8_t *pdata)
{
    uint16_t i, I, j, J, k, K, l, temp;
    uint16_t x, X, y, Y;
    uint32_t bc  = typehead.back;
    uint16_t fc  = typehead.fore;
    uint8_t mode = typehead.mode;

    if (mode & 0x04)                                                            // ����ʽ������ʽ
    {
        if (mode & 0x02)                                                        // ����ʽ
        {
            I = ((typehead.heigth-1) >> 3) + 1;                                 // ͳ���ַ��߶�ʹ�õ��ֽ���
            J = typehead.width;
            X = typehead.cury;
            Y = typehead.curx;
        }
        else                                                                    // ����ʽ
        {
            I = ((typehead.width-1) >> 3) + 1;                                  // ͳ���ַ����ʹ�õ��ֽ���
            J = typehead.heigth;
            X = typehead.curx;
            Y = typehead.cury;
        }

        for (i = I; i>0; i--, X += 8)
        {
            y = Y;
            if (i==1)                                                           // ����Ҫ��ʾ��ʣ����
            {
                if (mode & 0x02) K = typehead.heigth % 8;
                else             K = typehead.width % 8;
                if (K==0) K=8;
            }
            else K = 8;

            for (j = J; j>0; j--, y++)
            {
                x = X;
                temp = *pdata++;                                                // ȡ��ģ
                if (mode & 0x01) l = 0x01;                                      // LSB��ģ
                else             l = 0x80;                                      // MSB��ģ

                for (k = K; k>0; k--, x++)
                {
                    if (mode & 0x02)
                    {
                        if (temp & l)          lcd_draw_pixel(y, x, fc);
                        else if (bc < 0x10000) lcd_draw_pixel(y, x, bc);
                    }
                    else
                    {
                        if (temp & l)          lcd_draw_pixel(x, y, fc);
                        else if (bc < 0x10000) lcd_draw_pixel(x, y, bc);
                    }
                    if (mode & 0x01) l <<= 1;                                   // ȡ��һ��bit
                    else             l >>= 1;
                }
            }
        }
    }
    else                                                                        // ����ʽ������ʽ
    {
        if (mode & 0x02)                                                        // ����ʽ
        {
            I = typehead.width;
            J = ((typehead.heigth-1) >> 3) + 1;                                 // ͳ���ַ��߶ȶ�ʹ�õ��ֽ���
            K = typehead.heigth % 8;                                            // �����ַ����һ��Ҫ��ʾ�ĸ߶�
            X = typehead.cury;
            Y = typehead.curx;
        }
        else                                                                    // ����ʽ
        {
            I = typehead.heigth;
            J = ((typehead.width-1) >> 3) + 1;                                  // ͳ���ַ����ʹ�õ��ֽ���
            K = typehead.width % 8;                                             // �����ַ����һ��Ҫ��ʾ�Ŀ��
            X = typehead.curx;
            Y = typehead.cury;
        }
        if (K==0) K=8;

        for (i = I, y = Y; i>0; i--, y++)
        {
            for (x = X, j = J; j>0; j--)
            {
                temp = *pdata++;                                                // ȡ��ģ

                if (mode & 0x01) l = 0x01;                                      // LSB��ģ
                else             l = 0x80;                                      // MSB��ģ
                if (j==1) k = K;                                                // ���һ���ֽ�, ��ʾʣ����
                else      k = 8;

                for (; k>0; k--, x++)
                {
                    if (mode & 0x02)
                    {
                        if (temp & l)          lcd_draw_pixel(y, x, fc);
                        else if (bc < 0x10000) lcd_draw_pixel(y, x, bc);
                    }
                    else
                    {
                        if (temp & l)          lcd_draw_pixel(x, y, fc);
                        else if (bc < 0x10000) lcd_draw_pixel(x, y, bc);
                    }
                    if (mode & 0x01) l <<= 1;                                   // ȡ��һ��bit
                    else             l >>= 1;
                }
            }
        }
    }

    typehead.curx += typehead.width;                                            // X�����Զ�����
    if (typehead.curx >= LCD_X_MAX)                                             // �н���, �Զ�����
    {
        typehead.curx  = 0;
        typehead.cury += typehead.heigth;
        if (typehead.cury >= LCD_Y_MAX) typehead.cury = 0;
    }
}

/**
  ******************************************************************************
  * @brief  ���ݱ���, LCD��ʾһ������
  * @param  incode, �ַ�����
  * @retval None
  ******************************************************************************
  */
void lcd_put_font(uint16_t incode)
{
    uint8_t buf[TYPEHEAD_BYTE_MAX];

    if (incode < 0x20)                                                          // ASCII���, ������ʾ�ַ�
    {
        if (incode == 8)                                                        // �˸��
        {
            typehead.curx -= typehead.width;
            if (typehead.back >= 0x10000) typehead.back = LCD_BLACK;
            lcd_fill_rectangle(typehead.curx, typehead.cury, typehead.width, typehead.heigth, typehead.back);
        }
        else if (incode == 10)                                                  // LF, ���м�
        {
            typehead.cury += typehead.heigth;
            if (typehead.cury >= LCD_Y_MAX) typehead.cury = 0;
        }
        else if (incode == 13)                                                  // CR, �س���
        {
            typehead.curx = 0;
        }
    }
    else
    {
        if (lcd_load_typehead(incode, buf) == 0)                                // ������ģ�ɹ�
            lcd_put_typehead(buf);                                              // ��ӡ��ʾ
    }
}

/**
  ******************************************************************************
  * @brief  LCD�����ʽ����ӡ����
  * @param  pstring. �ַ���ָ��. ��ʽ�ж�����:
  *   @arg  %c,     ��ӡһ���ַ�
  *   @arg  %d,     ��ӡ�з���10������, uint32_t ��ʽ
  *   @arg  %s,     ��ӡ�ַ���
  *   @arg  %u,     ��ӡ�޷���10������, uint32_t ��ʽ
  *   @arg  %x,     ��ӡ16��������, uint32_t ��ʽ, ��ĸСд
  *   @arg  %X,     ��ӡ16��������, uint32_t ��ʽ, ��ĸ��д
  *   @arg  %p,     ��ӡָ��, 16������ʾ, uint32_t ��ʽ, ��ĸСд
  *   @arg  %%,     ��ӡ%��
  *   @arg  %8c,    ��8λ��ȴ�ӡһ���ַ�, �Ҷ���
  *   @arg  %08d,   ��8λ��ȴ�ӡһ������, ���0
  * @retval none
  ******************************************************************************
  */
void lcd_printf(char *pstring, ...)
{
    uint32_t value, i;
    uint16_t width, data_type, negative, incode;
    uint8_t *pstr, fill;
    const uint8_t * phex2char;
    va_list pva_arg;                                                            // ����һ���ɱ����

    va_start(pva_arg, pstring);
    phex2char = hex2char_lowercase;                                             // Ĭ��16������ĸСд

    while (*pstring)
    {
        while ((*pstring != '%') && (*pstring != '\0'))
        {
            incode = *pstring++;
            if (incode >= 0x80)                                                 // ��ASCII���ַ�, ��Ҫ16bit�ı���
            {
                incode <<= 8;
                incode |= *pstring++;
            }
            lcd_put_font(incode);
        }

        if (*pstring == '%')
        {
            width = 0;
            fill = ' ';
again:
            pstring++;
            switch(*pstring)
            {
            case '0':                                                           // ���ô�ӡ���
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                if ((*pstring == '0') && (width == 0)) fill = '0';              // ��%0, �����0
                width *= 10;                                                    // ͳ�Ƹ�ʽ���
                width += *pstring - '0';
                goto again;

            case 'c':                                                           // %c, ��ӡһ���ַ�
                while (width-- > 1) lcd_put_font(fill);
                value = va_arg(pva_arg, uint32_t);                              // �ӿɱ������ȡֵ
                lcd_put_font((uint8_t)value);
                break;

            case 's': {                                                         // %s, �ַ���
                pstr = va_arg(pva_arg, uint8_t *);                              // �ӿɱ������ȡֵ
                for (i=0; pstr[i] != '\0'; i++)
                {
                    if (width) width--;                                         // ͳ������ַ����
                    else break;
                }
                while (width--) lcd_put_font(' ');
                for (i=0; pstr[i] != '\0';)
                {
                    incode = pstr[i++];
                    if (incode >= 0x80)                                         // ��ASCII���ַ�, ��Ҫ16bit�ı���
                    {
                        incode <<= 8;
                        incode |= pstr[i++];
                    }
                    lcd_put_font(incode);
                }
                break; }

            case 'd': {                                                         // %d, �з���ʮ������
                value = va_arg(pva_arg, uint32_t);
                if ((int32_t)value < 0)
                {
                    value = -value;
                    negative = 1;
                }
                else
                {
                    negative = 0;
                }
                data_type = 10;
                goto convert; }

            case 'u':                                                           // %u, �޷���ʮ������
                value = va_arg(pva_arg, uint32_t);                              // �ӿɱ������ȡֵ
                data_type = 10;
                negative = 0;
                goto convert;

            case 'X':                                                           // %X, �޷���ʮ��������
                phex2char = hex2char_capital;                                   // ��Ϊ16������ĸ��д
            case 'x':                                                           // %x, �޷���ʮ��������
            case 'p': {                                                         // %p, ��ʾһ��ָ��
                value = va_arg(pva_arg, uint32_t);
                data_type = 16;
                negative = 0;
convert:                                                                        // ���ֵ��ַ�����ʾ��ת��
                if (negative)                                                   // ��������Ҫ���0,
                {
                    if (fill == '0')
                    {
                        negative = 0;
                        lcd_put_font('-');                                      // ���ַ���ӡ����
                    }
                    if (width) width--;
                }
                for ( i=1; (((i * data_type) <= value) && (((i * data_type) / data_type) == i)); i *= data_type )
                    if (width) width--;                                         // ͳ�������
                while (width-- > 1) lcd_put_font(fill);                         // ��ӡ����ַ�
                if (negative) lcd_put_font('-');                                // �Ǹ������ӡ����
                for(; i; i /= data_type)                                        // ��ӡ�����ַ�
                    lcd_put_font(phex2char[(value / i) % data_type]);
                break; }

            case '%':                                                           //  %%, ���һ���ٷֺ�
                lcd_put_font(*pstring);
                break;

            default:                                                            // �����Ƿ�����
                break;
            }
            pstring++;                                                          // ������ʽ��
        }
    }

    va_end(pva_arg);
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
