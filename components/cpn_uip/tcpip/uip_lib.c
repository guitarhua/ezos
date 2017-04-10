/**
  ******************************************************************************
  * @file    uip_lib.c
  * @author  mousie
  * @version V1.0.0
  * @date    2013.1.3
  * @brief   uIPϵͳ�⺯��. Ӧ�ò��ص�������.
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
  * @brief    ˽�����Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup UIP_LIB_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup UIP_LIB_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */

/**
  * @}
  */

/** @defgroup UIP_LIB_Private_Function
  * @brief    ����˽�к���
  * @{
  */

/**
  * @}
  */



/** @defgroup UIP_LIB_Function
  * @brief    ����ԭ�ļ�
  * @{
  */
/**
  ******************************************************************************
  * @brief  uIP �ַ���ת��ΪIP��ַ
  * @param  ipstr, ָ����ʽ���ַ���. ֧��IPv4��ʽ a.b.c.d
  * @param  ip,    ת���ɹ���, ��ipֵ�����ڴ˴�
  * @����ֵ 0, ת���ɹ�. 1, ת��ʧ��.
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
  * @brief  uIP IP��ַת��Ϊ�ַ���
  * @param  ip,    ip��ֵַ
  * @param  ipstr, ת��Ϊָ����ʽ���ַ���. ֧��IPv4��ʽ a.b.c.d, ��������Ϊ16.
  * @����ֵ 0, ת���ɹ�. 1, ת��ʧ��.
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
