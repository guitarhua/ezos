/**
 ******************************************************************************
 * @file    assert.c
 * @author  mousie
 * @version V2.1.0
 * @date    2013.1.3
 * @brief   调试驱动程序. 改名为 assert.c 实现assert_printf
 ******************************************************************************
 */

/* Include -------------------------------------------------------------------*/
#include "assert.h"



/** @addtogroup Drivers
 * @{
 */
/** @defgroup Assert
 * @{
 */
#ifdef ASSERT_ENABLE



/**
 ******************************************************************************
 * @addtogroup Assert_Configure
 ******************************************************************************
 * @{
 */
#include "drv_uart.h"

///
void assert_init(void)
{
	uart_init(COM1, 115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No);
}

///
#ifdef __GNUC__
    #define put_char_callback()         int __io_putchar(int ch)
#else
    #define put_char_callback()         int fputc(int ch, FILE *f)
#endif

put_char_callback()
{
	uart_tx_byte(COM1, ch);
	return ch;
}

/**
 * @}
 */



/** @defgroup Assert_Private_TypeDefine
 * @brief    私有类型定义
 * @{
 */

/**
 * @}
 */

/** @defgroup Assert_Private_MacroDefine
 * @brief    私有宏定义
 * @{
 */

/**
 * @}
 */

/** @defgroup Assert_Variables
 * @brief    定义全局变量(私有/公有)
 * @{
 */

/**
 * @}
 */

/** @defgroup Assert_Private_Function
 * @brief    定义私有函数
 * @{
 */

/**
 * @}
 */



/** @defgroup Assert_Function
 * @brief    函数原文件
 * @{
 */
/**
 ******************************************************************************
 * @brief  参数报错, 断言函数
 * @param  file, 断言所在的文件
 * @param  line, 断言所在的行号
 * @retval none
 ******************************************************************************
 */
void assert_err(char* file, uint32_t line)
{
	assert_printf("\r\nwrong parameter value detected on\r\n");
	assert_printf("    file: %s\r\n", file);
	assert_printf("    line: %d\r\n", line);
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
