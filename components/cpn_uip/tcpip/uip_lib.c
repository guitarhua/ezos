/**
  ******************************************************************************
  * @file    uip_lib.c
  * @author  mousie
  * @version V1.0.0
  * @date    2013.1.3
  * @brief   uIP系统库函数. 应用层或回调函数用.
  ******************************************************************************
  */

/* Include -------------------------------------------------------------------*/
#include "tcpip.h"
#include <string.h>



/** @addtogroup Components
  * @{
  */
/** @defgroup UIP_LIB
  * @{
  */



/**
  ******************************************************************************
  * @addtogroup UIP_LIB_Configure
  ******************************************************************************
  * @{
  */

/**
  * @}
  */



/** @defgroup UIP_LIB_Private_TypeDefine
  * @brief    私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup UIP_LIB_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup UIP_LIB_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */

/**
  * @}
  */

/** @defgroup UIP_LIB_Private_Function
  * @brief    定义私有函数
  * @{
  */

/**
  * @}
  */



/** @defgroup UIP_LIB_Function
  * @brief    函数原文件
  * @{
  */
/**
  ******************************************************************************
  * @brief  uIP 字符串转换为IP地址
  * @param  ipstr, 指定格式的字符串. 支持IPv4格式 a.b.c.d
  * @param  ip,    转换成功后, 将ip值保存在此处
  * @返回值 0, 转换成功. 1, 转换失败.
  ******************************************************************************
  */
uint8_t ip_str2ip(const char *ipstr, uip_ipaddr_t *ip)
{
    uint8_t tmp = 0;
    uint8_t i, j;
    char c;

    if (ip == UIP_NULL) return 1;

    for(i=0; i<4; ++i)
    {
        j = 0;
        do
        {
            c = *ipstr;
            if(++j > 4) return 1;
            if(c == '.' || c == 0)
            {
                ip->u8[i] = tmp;
                tmp = 0;
            }
            else if (c >= '0' && c <= '9')
            {
                tmp = (tmp * 10) + (c - '0');
            }
            else
            {
                return 1;
            }
            ++ipstr;
        } while(c != '.' && c != 0);
    }

    return 0;
}

/**
  ******************************************************************************
  * @brief  uIP IP地址转换为字符串
  * @param  ip,    ip地址值
  * @param  ipstr, 转换为指定格式的字符串. 支持IPv4格式 a.b.c.d, 长度至少为16.
  * @返回值 0, 转换成功. 1, 转换失败.
  ******************************************************************************
  */
uint8_t ip_ip2str(uip_ipaddr_t *ip, char *ipstr)
{
    uint8_t i, j, val;

    if (ipstr == UIP_NULL) return 1;

    for(i=0; i<4; ++i)
    {
        val = ip->u8[i];
        j = val / 100;
        if (j) *ipstr++ = j + '0';
        j = (val / 10) % 10;
        if (j || (val / 100)) *ipstr++ = j + '0';
        j = val % 100;
        *ipstr++ = j + '0';
        *ipstr++ = '.';
    }
    --ipstr;
    *ipstr = 0;

    return 0;
}
/**
  * @}
  */



/**
  * @}
  */
/**
  * @}
  */

/* END OF FILE ---------------------------------------------------------------*/
