/**
  ******************************************************************************
  * @file    lcd_font.c
  * @author  mousie-yu
  * @version V2.0.0
  * @date    2012.2.22
  * @brief   LCD 字体显示驱动程序.
  *          需要 LCD基础函数的 支持.
  *          字模生成可使用工具 FontLibrary 或 PCtoLCD.
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
  * @brief    私有类型定义
  * @{
  */
/// 字模参数类型定义
typedef struct
{
    uint32_t back;                                                              ///< 字模背景色, >0xFFFF 为透明色
    uint16_t fore;                                                              ///< 字模前景色
    uint16_t curx;                                                              ///< 字模当前X坐标
    uint16_t cury;                                                              ///< 字模当前Y坐标
    uint16_t byte;                                                              ///< 单个字模占用的字节数
    uint8_t  mode;                                                              ///< 字模取模方式
    uint8_t  width;                                                             ///< 字模宽度
    uint8_t  heigth;                                                            ///< 字模高度
} typehead_para_t;
/**
  * @}
  */

/** @defgroup LCD_FONT_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup LCD_FONT_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */
static typehead_para_t typehead;
static const uint8_t * const hex2char_lowercase = "0123456789abcdef";           ///< 数字转字符索引表, 小写字母
static const uint8_t * const hex2char_capital   = "0123456789ABCDEF";           ///< 数字转字符索引表, 大写字母
/**
  * @}
  */

/** @defgroup LCD_FONT_Private_Function
  * @brief    定义私有函数
  * @{
  */
extern uint8_t lcd_load_typehead(uint16_t incode, uint8_t *pdata);
/**
  * @}
  */



/** @defgroup LCD_FONT_Function
  * @brief    函数原文件
  * @{
  */
/**
  ******************************************************************************
  * @brief  设置字体显示参数, 包括起始位置, 前景色, 背景色
  * @param  x, x轴显示位置
  * @param  y, y轴显示位置
  * @param  back_color, 字体背景色, rgb565格式, >0xFFFF 为透明色
  * @param  fore_color, 字体前景色, rgb565格式.
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
  * @brief  设置字模长宽和取模类型
  * @param  width,  字模点阵宽度
  * @param  heigth, 字模点阵长度
  * @param  type,   字模类型
  *   @arg  FONT_MOULD_TYPE_X_MSB,      逐行式, 高位在前取模
  *   @arg  FONT_MOULD_TYPE_X_LSB,      逐行式, 低位在前取模
  *   @arg  FONT_MOULD_TYPE_XY_MSB,     行列式, 高位在前取模
  *   @arg  FONT_MOULD_TYPE_XY_LSB,     行列式, 低位在前取模
  *   @arg  FONT_MOULD_TYPE_Y_MSB,      逐列式, 高位在前取模
  *   @arg  FONT_MOULD_TYPE_Y_LSB,      逐列式, 低位在前取模
  *   @arg  FONT_MOULD_TYPE_YX_MSB,     列行式, 高位在前取模
  *   @arg  FONT_MOULD_TYPE_YX_LSB,     列行式, 低位在前取模
  * @retval None
  ******************************************************************************
  */
void lcd_set_typehead(uint8_t width, uint8_t heigth, typehead_mode_t mode)
{
    typehead.width  = width;
    typehead.heigth = heigth;
    typehead.mode   = mode;
    if (mode & 0x02) typehead.byte = ((heigth-1)/8 + 1) * width;                // 逐列式和列行式
    else             typehead.byte = ((width-1)/8 + 1) * heigth;                // 逐行式和行列式
}

/**
  ******************************************************************************
  * @brief  获取一个字模占用的字节数
  * @param  None
  * @retval 字模字节数
  ******************************************************************************
  */
uint8_t lcd_get_typehead_byte(void)
{
    return typehead.byte;
}

/**
  ******************************************************************************
  * @brief  根据字模地址, LCD显示一个字体
  * @param  pdata, 字模地址
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

    if (mode & 0x04)                                                            // 行列式或列行式
    {
        if (mode & 0x02)                                                        // 列行式
        {
            I = ((typehead.heigth-1) >> 3) + 1;                                 // 统计字符高度使用的字节数
            J = typehead.width;
            X = typehead.cury;
            Y = typehead.curx;
        }
        else                                                                    // 行列式
        {
            I = ((typehead.width-1) >> 3) + 1;                                  // 统计字符宽度使用的字节数
            J = typehead.heigth;
            X = typehead.curx;
            Y = typehead.cury;
        }

        for (i = I; i>0; i--, X += 8)
        {
            y = Y;
            if (i==1)                                                           // 计算要显示的剩余宽度
            {
                if (mode & 0x02) K = typehead.heigth % 8;
                else             K = typehead.width % 8;
                if (K==0) K=8;
            }
            else K = 8;

            for (j = J; j>0; j--, y++)
            {
                x = X;
                temp = *pdata++;                                                // 取字模
                if (mode & 0x01) l = 0x01;                                      // LSB字模
                else             l = 0x80;                                      // MSB字模

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
                    if (mode & 0x01) l <<= 1;                                   // 取下一个bit
                    else             l >>= 1;
                }
            }
        }
    }
    else                                                                        // 逐列式或逐行式
    {
        if (mode & 0x02)                                                        // 逐列式
        {
            I = typehead.width;
            J = ((typehead.heigth-1) >> 3) + 1;                                 // 统计字符高度度使用的字节数
            K = typehead.heigth % 8;                                            // 计算字符最后一段要显示的高度
            X = typehead.cury;
            Y = typehead.curx;
        }
        else                                                                    // 逐行式
        {
            I = typehead.heigth;
            J = ((typehead.width-1) >> 3) + 1;                                  // 统计字符宽度使用的字节数
            K = typehead.width % 8;                                             // 计算字符最后一段要显示的宽度
            X = typehead.curx;
            Y = typehead.cury;
        }
        if (K==0) K=8;

        for (i = I, y = Y; i>0; i--, y++)
        {
            for (x = X, j = J; j>0; j--)
            {
                temp = *pdata++;                                                // 取字模

                if (mode & 0x01) l = 0x01;                                      // LSB字模
                else             l = 0x80;                                      // MSB字模
                if (j==1) k = K;                                                // 最后一个字节, 显示剩余宽度
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
                    if (mode & 0x01) l <<= 1;                                   // 取下一个bit
                    else             l >>= 1;
                }
            }
        }
    }

    typehead.curx += typehead.width;                                            // X坐标自动右移
    if (typehead.curx >= LCD_X_MAX)                                             // 行结束, 自动换行
    {
        typehead.curx  = 0;
        typehead.cury += typehead.heigth;
        if (typehead.cury >= LCD_Y_MAX) typehead.cury = 0;
    }
}

/**
  ******************************************************************************
  * @brief  根据编码, LCD显示一个字体
  * @param  incode, 字符编码
  * @retval None
  ******************************************************************************
  */
void lcd_put_font(uint16_t incode)
{
    uint8_t buf[TYPEHEAD_BYTE_MAX];

    if (incode < 0x20)                                                          // ASCII码表, 不可显示字符
    {
        if (incode == 8)                                                        // 退格键
        {
            typehead.curx -= typehead.width;
            if (typehead.back >= 0x10000) typehead.back = LCD_BLACK;
            lcd_fill_rectangle(typehead.curx, typehead.cury, typehead.width, typehead.heigth, typehead.back);
        }
        else if (incode == 10)                                                  // LF, 换行键
        {
            typehead.cury += typehead.heigth;
            if (typehead.cury >= LCD_Y_MAX) typehead.cury = 0;
        }
        else if (incode == 13)                                                  // CR, 回车键
        {
            typehead.curx = 0;
        }
    }
    else
    {
        if (lcd_load_typehead(incode, buf) == 0)                                // 调用字模成功
            lcd_put_typehead(buf);                                              // 打印显示
    }
}

/**
  ******************************************************************************
  * @brief  LCD字体格式化打印函数
  * @param  pstring. 字符串指针. 格式判断如下:
  *   @arg  %c,     打印一个字符
  *   @arg  %d,     打印有符号10进制数, uint32_t 格式
  *   @arg  %s,     打印字符串
  *   @arg  %u,     打印无符号10进制数, uint32_t 格式
  *   @arg  %x,     打印16进制数字, uint32_t 格式, 字母小写
  *   @arg  %X,     打印16进制数字, uint32_t 格式, 字母大写
  *   @arg  %p,     打印指针, 16进制显示, uint32_t 格式, 字母小写
  *   @arg  %%,     打印%号
  *   @arg  %8c,    按8位宽度打印一个字符, 右对齐
  *   @arg  %08d,   按8位宽度打印一个整数, 填充0
  * @retval none
  ******************************************************************************
  */
void lcd_printf(char *pstring, ...)
{
    uint32_t value, i;
    uint16_t width, data_type, negative, incode;
    uint8_t *pstr, fill;
    const uint8_t * phex2char;
    va_list pva_arg;                                                            // 定义一个可变参数

    va_start(pva_arg, pstring);
    phex2char = hex2char_lowercase;                                             // 默认16进制字母小写

    while (*pstring)
    {
        while ((*pstring != '%') && (*pstring != '\0'))
        {
            incode = *pstring++;
            if (incode >= 0x80)                                                 // 非ASCII码字符, 需要16bit的编码
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
            case '0':                                                           // 设置打印宽度
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                if ((*pstring == '0') && (width == 0)) fill = '0';              // 是%0, 则填充0
                width *= 10;                                                    // 统计格式宽度
                width += *pstring - '0';
                goto again;

            case 'c':                                                           // %c, 打印一个字符
                while (width-- > 1) lcd_put_font(fill);
                value = va_arg(pva_arg, uint32_t);                              // 从可变参数中取值
                lcd_put_font((uint8_t)value);
                break;

            case 's': {                                                         // %s, 字符串
                pstr = va_arg(pva_arg, uint8_t *);                              // 从可变参数中取值
                for (i=0; pstr[i] != '\0'; i++)
                {
                    if (width) width--;                                         // 统计填充字符宽度
                    else break;
                }
                while (width--) lcd_put_font(' ');
                for (i=0; pstr[i] != '\0';)
                {
                    incode = pstr[i++];
                    if (incode >= 0x80)                                         // 非ASCII码字符, 需要16bit的编码
                    {
                        incode <<= 8;
                        incode |= pstr[i++];
                    }
                    lcd_put_font(incode);
                }
                break; }

            case 'd': {                                                         // %d, 有符号十进制数
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

            case 'u':                                                           // %u, 无符号十进制数
                value = va_arg(pva_arg, uint32_t);                              // 从可变参数中取值
                data_type = 10;
                negative = 0;
                goto convert;

            case 'X':                                                           // %X, 无符号十六进制数
                phex2char = hex2char_capital;                                   // 改为16进制字母大写
            case 'x':                                                           // %x, 无符号十六进制数
            case 'p': {                                                         // %p, 显示一个指针
                value = va_arg(pva_arg, uint32_t);
                data_type = 16;
                negative = 0;
convert:                                                                        // 数字到字符串显示的转换
                if (negative)                                                   // 负数并且要填充0,
                {
                    if (fill == '0')
                    {
                        negative = 0;
                        lcd_put_font('-');                                      // 首字符打印负号
                    }
                    if (width) width--;
                }
                for ( i=1; (((i * data_type) <= value) && (((i * data_type) / data_type) == i)); i *= data_type )
                    if (width) width--;                                         // 统计填充宽度
                while (width-- > 1) lcd_put_font(fill);                         // 打印填充字符
                if (negative) lcd_put_font('-');                                // 是负数则打印负号
                for(; i; i /= data_type)                                        // 打印数字字符
                    lcd_put_font(phex2char[(value / i) % data_type]);
                break; }

            case '%':                                                           //  %%, 输出一个百分号
                lcd_put_font(*pstring);
                break;

            default:                                                            // 其它非法参数
                break;
            }
            pstring++;                                                          // 跳过格式符
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
