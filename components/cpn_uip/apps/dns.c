/**
  ******************************************************************************
  * @file    dns.c
  * @author  mousie
  * @version V1.0.0
  * @date    2013.4.26
  * @brief   提供 DNS CLIENT 功能. 域名解析
  ******************************************************************************
  */

/* Include -------------------------------------------------------------------*/
#include "dns.h"
#include <string.h>
#include <stdlib.h>



/** @addtogroup Components
  * @{
  */
/** @defgroup DNS
  * @{
  */
#if UIP_DNS



/**
  ******************************************************************************
  * @addtogroup DNS_Configure
  ******************************************************************************
  * @{
  */
#define DNS_LOG(...)                    //assert_printf(__VA_ARGS__)
/**
  * @}
  */



/** @defgroup DNS_Private_TypeDefine
  * @brief    私有类型定义
  * @{
  */
// DNS协议头格式
typedef struct {
    uint16_t id;
    uint8_t  flags1, flags2;
    uint16_t numquestions;
    uint16_t numanswers;
    uint16_t numauthrr;
    uint16_t numextrarr;
} dns_hdr_t;

// DNS应答协议格式
typedef struct {
    uint16_t type;
    uint16_t class;
    uint16_t ttl[2];
    uint16_t len;
    uint8_t  ipaddr[4];
} dns_answer_t;

// DNS回调函数用数据结构
typedef struct {
    uint8_t  state;
    uint16_t timer;
    char name[32];
    uip_ipaddr_t ipaddr;
} namemap_t;

/**
  * @}
  */

/** @defgroup DNS_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */
#define DNS_FLAG1_RESPONSE              0x80
#define DNS_FLAG1_OPCODE_STATUS         0x10
#define DNS_FLAG1_OPCODE_INVERSE        0x08
#define DNS_FLAG1_OPCODE_STANDARD       0x00
#define DNS_FLAG1_AUTHORATIVE           0x04
#define DNS_FLAG1_TRUNC                 0x02
#define DNS_FLAG1_RD                    0x01
#define DNS_FLAG2_RA                    0x80
#define DNS_FLAG2_ERR_MASK              0x0f
#define DNS_FLAG2_ERR_NONE              0x00
#define DNS_FLAG2_ERR_NAME              0x03

#define DNS_STATE_UNUSED                0
#define DNS_STATE_NEW                   1
#define DNS_STATE_ASKING                2
#define DNS_STATE_WAITACK               3
#define DNS_STATE_OK                    4
#define DNS_STATE_ERROR                 5
#define DNS_STATE_EXCEPTION             0xFF

#define DNS_SERVER_PORT                 53
/**
  * @}
  */

/** @defgroup DNS_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */
static namemap_t names[UIP_DNS];
/**
  * @}
  */

/** @defgroup DNS_Private_Function
  * @brief    定义私有函数
  * @{
  */
static SOCKET(socket_dns_client);
/**
  * @}
  */



/** @defgroup DNS_Function
  * @brief    函数原文件
  * @{
  */
/**
  ******************************************************************************
  * @brief  dns协议内部处理函数
  * @param  自定义
  * @retval 自定义
  ******************************************************************************
  */
static char *parse_name(char *query)
{
    unsigned char n;

    do {
        n = *query++;
        while(n > 0)
        {
            ++query;
            --n;
        };
    } while(*query != 0);

    return query + 1;
}

static uint16_t create_dns_msg(uint8_t idx)
{
    dns_hdr_t *hdr;
    char *query, *nptr, *nameptr;

    hdr = (dns_hdr_t *)uip_appdata;
    memset(hdr, 0, sizeof(dns_hdr_t));
    hdr->id           = uip_htons(idx);
    hdr->flags1       = DNS_FLAG1_RD;
    hdr->numquestions = UIP_HTONS(1);

    query = (char *)uip_appdata + 12;
    nameptr = names[idx].name;
    --nameptr;

    // Convert hostname into suitable query format
    do
    {
        uint8_t n;

        ++nameptr;
        nptr = query;
        ++query;
        for (n = 0; *nameptr != '.' && *nameptr != 0; ++nameptr)
        {
            *query = *nameptr;
            ++query;
            ++n;
        }
        *nptr = n;
    } while(*nameptr != 0);

    *query++ = 0;
    *query++ = 0;
    *query++ = 1;
    *query++ = 0;
    *query++ = 1;

    return (uint16_t)(query - (char *)uip_appdata);
}

static uint8_t msg_for_me(uint8_t idx)
{
    dns_hdr_t *hdr = (dns_hdr_t *)uip_appdata;

    if ((uint8_t)uip_htons(hdr->id) == idx)
    {
        DNS_LOG("   DNS_STATE_WAITACK received:\r\n");
        DNS_LOG("      ID=%d, Query=%d, Error=%d\r\n",
            uip_htons(hdr->id),
            (hdr->flags1 & DNS_FLAG1_RESPONSE),
            (hdr->flags2 & DNS_FLAG2_ERR_MASK));
        DNS_LOG("      questions=%d, answers=%d, authrr=%d, extrarr=%d,\r\n",
            uip_htons(hdr->numquestions),
            uip_htons(hdr->numanswers),
            uip_htons(hdr->numauthrr),
            uip_htons(hdr->numextrarr));

        if ((hdr->flags2 & DNS_FLAG2_ERR_MASK) == 0)
        {
            uint8_t nanswers;
            char *nameptr;

            nanswers   = (uint8_t)uip_htons(hdr->numanswers);
            nameptr    = parse_name((char *)uip_appdata + 12) + 4;

            while (nanswers > 0)
            {
                dns_answer_t *ans;

                if(*nameptr & 0xc0) nameptr += 2;
                else                nameptr  = parse_name(nameptr);
                ans = (dns_answer_t *)nameptr;

                if (ans->type  == UIP_HTONS(1) && ans->class == UIP_HTONS(1) && ans->len   == UIP_HTONS(4))
                {
                    uint8_t i;
                    for(i = 0; i < 4; i++) names[idx].ipaddr.u8[i] = ans->ipaddr[i];
                    return DNS_STATE_OK;
                }
                else
                {
                    nameptr = nameptr + 10 + uip_htons(ans->len);
                }
                --nanswers;
            }
        }
        return DNS_STATE_ERROR;
    }
    return DNS_STATE_WAITACK;
}

/**
  ******************************************************************************
  * @brief  dns客户端socket回调处理函数
  * @param  socket回调函数内置参数
  * @retval none
  ******************************************************************************
  */
SOCKET(socket_dns_client)
{
    uint8_t idx = (uint32_t)(*appdata);

    if (idx >= UIP_DNS)
    {
        DNS_LOG("   DNS idx exception!\r\n\r\n");
        socket_close();
        return;
    }

    switch (names[idx].state)
    {
    case DNS_STATE_NEW: {
        DNS_LOG("   DNS_STATE_NEW.\r\n");
        names[idx].timer = 500;
        names[idx].state = DNS_STATE_ASKING;
        break; }

    case DNS_STATE_ASKING: {
        socket_t val;
        uint16_t datalen;

        datalen = create_dns_msg(idx);
        val = socket_tx(uip_appdata, datalen, 5000);
        if (val == SOCKET_WAIT) return;
        if (val == SOCKET_OK)
        {
            DNS_LOG("   DNS_STATE_ASKING send ok.\r\n");
            names[idx].state = DNS_STATE_WAITACK;
        }
        else names[idx].state = DNS_STATE_EXCEPTION;
        break; }

    case DNS_STATE_WAITACK: {
        socket_t val;

        val = socket_rx(uip_appdata, sizeof(dns_hdr_t) + sizeof(dns_answer_t), names[idx].timer);
        if (val == SOCKET_WAIT) return;
        if (val == SOCKET_TIMEOUT)
        {
            DNS_LOG("   DNS_STATE_WAITACK timeout.\r\n");
            if (names[idx].timer < 4000)                                    // 超时, 无响应
            {
                names[idx].timer *= 2;
                names[idx].state = DNS_STATE_ASKING;
            }
            else names[idx].state = DNS_STATE_EXCEPTION;
        }
        else if ((val == SOCKET_OK) || (val == SOCKET_FULL))
        {
            uint8_t st;

            st = msg_for_me(idx);
            if (st == DNS_STATE_OK)                                         // 正确应答
            {
                DNS_LOG("   Name:\"%s\"\r\n", names[idx].name);
                DNS_LOG("   IP  : %d.%d.%d.%d\r\n", uip_ipaddr_to_quad(&names[idx].ipaddr));
                DNS_LOG("   DNS OK!\r\n\r\n");
                names[idx].state = DNS_STATE_OK;
            }
            else if (st == DNS_STATE_ERROR)                                 // DNS有错误
            {
                DNS_LOG("   DNS error!\r\n\r\n");
                names[idx].state = DNS_STATE_ERROR;
            }
            else                                                            // 继续等待
            {
                socket_break();
            }
        }
        else names[idx].state = DNS_STATE_EXCEPTION;
        break; }

    case DNS_STATE_EXCEPTION:
    default:
        DNS_LOG("   DNS exception!\r\n\r\n");
        names[idx].state = DNS_STATE_EXCEPTION;
    case DNS_STATE_OK:
    case DNS_STATE_ERROR:
        socket_close();
        break;
    }
}

/**
  ******************************************************************************
  * @brief  DNS启动域名查询
  * @param  none
  * @retval none
  ******************************************************************************
  */
dns_t dns_lookup(const char *name, uip_ipaddr_t *ip)
{
    uint8_t i;
    uip_udp_conn_t *dns_conn;

    // 寻找已有的DNS结果
    for (i=0; i < UIP_DNS; ++i)
    {
        if (strcmp(name, names[i].name) == 0)
        {
            if (names[i].state == DNS_STATE_OK)
            {
                ip_copy(ip, &names[i].ipaddr);
                return DNS_OK;
            }

            ip_copy(ip, &uip_all_zeroes_addr);
            if (names[i].state == DNS_STATE_EXCEPTION)
            {
                memset(&names[i], 0, sizeof(namemap_t));
                return DNS_ERROR;
            }
            if (names[i].state == DNS_STATE_ERROR)  return DNS_ERROR;
            if (names[i].state != DNS_STATE_UNUSED) return DNS_BUSY;
        }
    }

    // 其它DNS运行中
    if (udp_client_get(socket_dns_client)) return DNS_BUSY;

    // 缓冲区无结果, 寻找空缓冲区
    for (i=0; i < UIP_DNS; ++i)
    {
        if ((names[i].state == DNS_STATE_UNUSED) || (names[i].state == DNS_STATE_EXCEPTION))
            goto __DNS_START;
    }

    // 缓冲区满, 寻找错误结果的缓冲区,
    for (i=0; i < UIP_DNS; ++i)
    {
        if (names[i].state == DNS_STATE_ERROR)
            goto __DNS_START;
    }

    // 未找到合适的缓冲区, 则随机分配一个
    i = rand() % UIP_DNS;

    // 准备开始DNS
__DNS_START:

    dns_conn = udp_client_add(socket_dns_client, (void *)i, &uip_dnsaddr, DNS_SERVER_PORT);
    if (dns_conn)
    {
        names[i].state = DNS_STATE_NEW;
        names[i].timer = 0;
        strncpy(names[i].name, name, sizeof(names[i].name)-1);
        ip_copy(&names[i].ipaddr, &uip_all_zeroes_addr);
        DNS_LOG("\r\n-- DNS start! search \"%s\".\r\n", names[i].name);
        return DNS_BUSY;
    }
    return DNS_ERROR;
}

/**
  ******************************************************************************
  * @brief  清楚已有的DNS数据缓存
  * @param  none
  * @retval none
  ******************************************************************************
  */
void dns_clear(void)
{
    memset(names, 0, UIP_DNS * sizeof(namemap_t));
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
