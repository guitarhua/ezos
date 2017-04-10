/**
  ******************************************************************************
  * @file    dhcp.c
  * @author  mousie
  * @version V1.0.0
  * @date    2013.4.26
  * @brief   提供 DHCP CLIENT 功能. 自动分配IP地址
  ******************************************************************************
  */

/* Include -------------------------------------------------------------------*/
#include "dhcp.h"
#include "uip_arp.h"
#include <string.h>



/** @addtogroup Components
  * @{
  */
/** @defgroup DHCP
  * @{
  */
#if UIP_DHCP



/**
  ******************************************************************************
  * @addtogroup DHCP_Configure
  ******************************************************************************
  * @{
  */
#define DHCP_PRINTF(...)                //assert_printf(__VA_ARGS__)
/**
  * @}
  */



/** @defgroup DHCP_Private_TypeDefine
  * @brief    私有类型定义
  * @{
  */
typedef struct {
    uint8_t op, htype, hlen, hops;
    uint8_t xid[4];
    uint16_t secs, flags;
    uint8_t ciaddr[4];
    uint8_t yiaddr[4];
    uint8_t siaddr[4];
    uint8_t giaddr[4];
    uint8_t chaddr[16];
    uint8_t sname[64];
    uint8_t file[128];
    uint8_t options[312];
} dhcp_msg_t;
/**
  * @}
  */

/** @defgroup DHCP_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */
#define STATE_INIT                      0
#define STATE_SEND_DISCOVER             1
#define STATE_SELECTING                 2
#define STATE_SEND_REQUEST              3
#define STATE_REQUESTING                4
#define STATE_BOUND                     5
#define STATE_RENEWING                  6
#define STATE_EXCEPTION                 0xFF

#define DHCP_REQUEST                    1
#define DHCP_REPLY                      2
#define DHCP_HTYPE_ETHERNET             1
#define DHCP_HLEN_ETHERNET              6
#define DHCP_MSG_LEN                    236

#define DHCP_SERVER_PORT                67
#define DHCP_CLIENT_PORT                68

#define DHCPDISCOVER                    1
#define DHCPOFFER                       2
#define DHCPREQUEST                     3
#define DHCPDECLINE                     4
#define DHCPACK                         5
#define DHCPNAK                         6
#define DHCPRELEASE                     7

#define DHCP_OPTION_SUBNET_MASK         1
#define DHCP_OPTION_ROUTER              3
#define DHCP_OPTION_DNS_SERVER          6
#define DHCP_OPTION_REQ_IPADDR          50
#define DHCP_OPTION_LEASE_TIME          51
#define DHCP_OPTION_MSG_TYPE            53
#define DHCP_OPTION_SERVER_ID           54
#define DHCP_OPTION_REQ_LIST            55
#define DHCP_OPTION_END                 255

#define BOOTP_BROADCAST                 0x8000
#define MAX_TICKS32                     (~((uint32_t)0))
/**
  * @}
  */

/** @defgroup DHCP_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */
static uint32_t xid;
static dhcp_app_t dhcp_app;
/**
  * @}
  */

/** @defgroup DHCP_Private_Function
  * @brief    定义私有函数
  * @{
  */
static uint8_t *add_msg_type(uint8_t *optptr, uint8_t type);
static uint8_t *add_server_id(uint8_t *optptr);
static uint8_t *add_req_ipaddr(uint8_t *optptr);
static uint8_t *add_req_options(uint8_t *optptr);
static uint8_t *add_end(uint8_t *optptr);
static void create_msg(dhcp_msg_t *m);
static uint16_t creat_discover_msg(void);
static uint16_t creat_request_msg(void);
static uint8_t parse_options(uint8_t *optptr, int len);
static uint8_t parse_msg(void);
static int msg_for_me(void);
static dhcp_app_config(void);
static dhcp_app_unconfig(void);

static SOCKET(socket_dhcp_client);
/**
  * @}
  */



/** @defgroup DHCP_Function
  * @brief    函数原文件
  * @{
  */
/**
  ******************************************************************************
  * @brief  dhcp协议内部处理函数
  * @param  自定义
  * @retval 自定义
  ******************************************************************************
  */
static uint8_t *add_msg_type(uint8_t *optptr, uint8_t type)
{
    *optptr++ = DHCP_OPTION_MSG_TYPE;
    *optptr++ = 1;
    *optptr++ = type;
    return optptr;
}

static uint8_t *add_server_id(uint8_t *optptr)
{
    *optptr++ = DHCP_OPTION_SERVER_ID;
    *optptr++ = 4;
    memcpy(optptr, dhcp_app.serverid, 4);
    return optptr + 4;
}

static uint8_t *add_req_ipaddr(uint8_t *optptr)
{
    *optptr++ = DHCP_OPTION_REQ_IPADDR;
    *optptr++ = 4;
    memcpy(optptr, dhcp_app.ipaddr.u16, 4);
    return optptr + 4;
}

static uint8_t *add_req_options(uint8_t *optptr)
{
    *optptr++ = DHCP_OPTION_REQ_LIST;
    *optptr++ = 3;
    *optptr++ = DHCP_OPTION_SUBNET_MASK;
    *optptr++ = DHCP_OPTION_ROUTER;
    *optptr++ = DHCP_OPTION_DNS_SERVER;
    return optptr;
}

static uint8_t *add_end(uint8_t *optptr)
{
    *optptr++ = DHCP_OPTION_END;
    return optptr;
}

static void create_msg(dhcp_msg_t *m)
{
    static const uint8_t magic_cookie[4] = {99, 130, 83, 99};

    m->op = DHCP_REQUEST;
    m->htype = DHCP_HTYPE_ETHERNET;
    m->hlen = sizeof(uip_macaddr_t);
    m->hops = 0;
    memcpy(m->xid, &xid, sizeof(m->xid));
    m->secs = 0;
    m->flags = UIP_HTONS(BOOTP_BROADCAST);
    memcpy(m->ciaddr, uip_hostaddr.u16, sizeof(m->ciaddr));
    memset(m->yiaddr, 0, sizeof(m->yiaddr));
    memset(m->siaddr, 0, sizeof(m->siaddr));
    memset(m->giaddr, 0, sizeof(m->giaddr));
    memcpy(m->chaddr, &uip_ethaddr, sizeof(uip_macaddr_t));
    memset(&m->chaddr[sizeof(uip_macaddr_t)], 0, sizeof(m->chaddr) - sizeof(uip_macaddr_t));
    memset(m->sname, 0, sizeof(m->sname));
    memset(m->file, 0, sizeof(m->file));
    memcpy(m->options, magic_cookie, sizeof(magic_cookie));
}

static uint16_t creat_discover_msg(void)
{
    uint8_t *end;
    dhcp_msg_t *m = (dhcp_msg_t *)uip_appdata;

    create_msg(m);
    end = add_msg_type(&m->options[4], DHCPDISCOVER);
    end = add_req_options(end);
    end = add_end(end);

    return (uint16_t)(end - (uint8_t *)uip_appdata);
}

static uint16_t creat_request_msg(void)
{
    uint8_t *end;
    dhcp_msg_t *m = (dhcp_msg_t *)uip_appdata;

    create_msg(m);
    end = add_msg_type(&m->options[4], DHCPREQUEST);
    end = add_server_id(end);
    end = add_req_ipaddr(end);
    end = add_end(end);

    return (uint16_t)(end - (uint8_t *)uip_appdata);
}

static uint8_t parse_options(uint8_t *optptr, int len)
{
    uint8_t *end = optptr + len;
    uint8_t type = 0;

    while(optptr < end)
    {
        switch(*optptr)
        {
        case DHCP_OPTION_SUBNET_MASK:
            memcpy(dhcp_app.netmask.u16, optptr + 2, 4);
            break;
        case DHCP_OPTION_ROUTER:
            memcpy(dhcp_app.router.u16, optptr + 2, 4);
            break;
        case DHCP_OPTION_DNS_SERVER:
            memcpy(dhcp_app.dnsaddr.u16, optptr + 2, 4);
            break;
        case DHCP_OPTION_MSG_TYPE:
            type = *(optptr + 2);
            break;
        case DHCP_OPTION_SERVER_ID:
            memcpy(dhcp_app.serverid, optptr + 2, 4);
            break;
        case DHCP_OPTION_LEASE_TIME:
            memcpy(dhcp_app.lease_time, optptr + 2, 4);
            dhcp_app.lease_time[0] = uip_ntohs(dhcp_app.lease_time[0]);
            dhcp_app.lease_time[1] = uip_ntohs(dhcp_app.lease_time[1]);
            break;
        case DHCP_OPTION_END:
            return type;
        }
        optptr += optptr[1] + 2;
    }
    return type;
}

static uint8_t parse_msg(void)
{
    dhcp_msg_t *m = (dhcp_msg_t *)uip_appdata;

    if (m->op == DHCP_REPLY &&
        memcmp(m->xid, &xid, sizeof(xid)) == 0 &&
        memcmp(m->chaddr, &uip_ethaddr, sizeof(uip_macaddr_t)) == 0)
    {
        memcpy(dhcp_app.ipaddr.u16, m->yiaddr, 4);
        return parse_options(&m->options[4], uip_datalen());
    }

    return 0;
}

static int msg_for_me(void)
{
    dhcp_msg_t *m = (dhcp_msg_t *)uip_appdata;
    uint8_t *optptr = &m->options[4];
    uint8_t *end = (uint8_t*)uip_appdata + uip_datalen();

    if (m->op == DHCP_REPLY &&
        memcmp(m->xid, &xid, sizeof(xid)) == 0 &&
        memcmp(m->chaddr, &uip_ethaddr, sizeof(uip_macaddr_t)) == 0)
    {
        while(optptr < end)
        {
            if (*optptr == DHCP_OPTION_MSG_TYPE) return *(optptr + 2);
            else if (*optptr == DHCP_OPTION_END) return -1;
            optptr += optptr[1] + 2;
        }
    }

    return -1;
}

static dhcp_app_config(void)
{
    ip_set_host(&dhcp_app.ipaddr);
    ip_set_mask(&dhcp_app.netmask);
    ip_set_gateway(&dhcp_app.router);
    ip_set_dns(&dhcp_app.dnsaddr);

    DHCP_PRINTF("\r\n-- dhcp config OK!\r\n");
    DHCP_PRINTF("   IP addr: %d.%d.%d.%d\r\n", uip_ipaddr_to_quad(&dhcp_app.ipaddr));
    DHCP_PRINTF("   netmask: %d.%d.%d.%d\r\n", uip_ipaddr_to_quad(&dhcp_app.netmask));
    DHCP_PRINTF("   router:  %d.%d.%d.%d\r\n", uip_ipaddr_to_quad(&dhcp_app.router));
    DHCP_PRINTF("   DNS:     %d.%d.%d.%d\r\n", uip_ipaddr_to_quad(&dhcp_app.dnsaddr));
    DHCP_PRINTF("   Lease expires in %ld seconds\r\n\r\n", dhcp_app.lease_time[0]*65536ul + dhcp_app.lease_time[1]);
}

static dhcp_app_unconfig(void)
{
    uip_ipaddr_t  ip;

    dhcp_app.lease_time[0] = 0;
    dhcp_app.lease_time[1] = 0;

    // 设置本机IP地址
    ip_set(&ip, UIP_IPADDR0,  UIP_IPADDR1,  UIP_IPADDR2,  UIP_IPADDR3);
    ip_copy(&dhcp_app.ipaddr, &ip);
    ip_set_host(&ip);
    // 设置子网掩码
    ip_set(&ip, UIP_MSKADDR0, UIP_MSKADDR1, UIP_MSKADDR2, UIP_MSKADDR3);
    ip_copy(&dhcp_app.netmask, &ip);
    ip_set_mask(&ip);
    // 设置默认网关
    ip_set(&ip, UIP_GWADDR0,  UIP_GWADDR1,  UIP_GWADDR2,  UIP_GWADDR3);
    ip_copy(&dhcp_app.router, &ip);
    ip_set_gateway(&ip);
    // 设置DNS服务器
    ip_set(&ip, UIP_DNSADDR0, UIP_DNSADDR1, UIP_DNSADDR2, UIP_DNSADDR3);
    ip_copy(&dhcp_app.dnsaddr, &ip);
    ip_set_dns(&ip);

    DHCP_PRINTF("uip> DHCP unconfig. Load default ip!\r\n");
    DHCP_PRINTF("uip> IP addr: %d.%d.%d.%d\r\n", uip_ipaddr_to_quad(&dhcp_app.ipaddr));
    DHCP_PRINTF("uip> netmask: %d.%d.%d.%d\r\n", uip_ipaddr_to_quad(&dhcp_app.netmask));
    DHCP_PRINTF("uip> router:  %d.%d.%d.%d\r\n", uip_ipaddr_to_quad(&dhcp_app.router));
    DHCP_PRINTF("uip> DNS:     %d.%d.%d.%d\r\n", uip_ipaddr_to_quad(&dhcp_app.dnsaddr));
}



/**
  ******************************************************************************
  * @brief  dhcp客户端socket回调处理函数
  * @param  socket回调函数内置参数
  * @retval none
  ******************************************************************************
  */
SOCKET(socket_dhcp_client)
{
    static uint32_t tick_ms;

    switch (*state)
    {
    case STATE_INIT: {
        dhcp_app.state = DHCP_BUSY;
        ip_set_host(&uip_all_zeroes_addr);                                      // DHCP 主机地址必须为0
        xid++;
        tick_ms = 1000;
        *state = STATE_SEND_DISCOVER;
        break; }

    case STATE_SEND_DISCOVER: {
        socket_t val;
        uint16_t datalen;

        datalen = creat_discover_msg();                                         // 准备 DHCPDISCOVER 报文
        val = socket_tx(uip_appdata, datalen, 1000 * 60);                       // 发送 DHCPDISCOVER 报文
        if (val == SOCKET_WAIT) return;
        if (val == SOCKET_OK) *state = STATE_SELECTING;
        else                  *state = STATE_EXCEPTION;
        break; }

    case STATE_SELECTING: {
        socket_t val;

        val = socket_rx(uip_appdata, sizeof(dhcp_msg_t), tick_ms);              // 等待 DHCPOFFER 响应
        if (val == SOCKET_WAIT) return;
        if (val == SOCKET_TIMEOUT)
        {
            if (tick_ms < 1000 * 60)                                            // 超时, 无响应
            {
                tick_ms *= 2;
                *state = STATE_SEND_DISCOVER;
            }
            else *state = STATE_EXCEPTION;
        }
        else if ((val == SOCKET_OK) || (val == SOCKET_FULL))
        {
            if (msg_for_me() == DHCPOFFER)                                      // 正确应答
            {
                parse_msg();
                xid++;
                tick_ms = 1000;
                *state = STATE_SEND_REQUEST;
            }
        }
        else *state = STATE_EXCEPTION;
        break; }

    case STATE_SEND_REQUEST: {
        socket_t val;
        uint16_t datalen;

        datalen = creat_request_msg();                                          // 准备 DHCPREQUEST 报文
        val = socket_tx(uip_appdata, datalen, 1000 * 60);                       // 发送 DHCPREQUEST 报文
        if (val == SOCKET_WAIT) return;
        if (val == SOCKET_OK)  *state = STATE_REQUESTING;
        else                   *state = STATE_EXCEPTION;
        break; }

    case STATE_REQUESTING: {
        socket_t val;

        val = socket_rx(uip_appdata, sizeof(dhcp_msg_t), tick_ms);              // 等待 DHCPACK 响应
        if (val == SOCKET_WAIT) return;
        if (val == SOCKET_TIMEOUT)
        {
            *state = STATE_INIT;
            if (dhcp_app.state == DHCP_OK)                                      // 续约的延时
            {
                if (tick_ms >= 3000)
                {
                    tick_ms /= 2;
                    *state = STATE_SEND_REQUEST;
                }
            }
            else if (tick_ms <= 1000 * 10)                                      // 全新租约的延时
            {
                tick_ms += 1000;
                *state = STATE_SEND_REQUEST;
            }
        }
        else if ((val == SOCKET_OK) || (val == SOCKET_FULL))
        {
            if (msg_for_me() == DHCPACK)
            {
                parse_msg();
                *state = STATE_BOUND;
            }
            else socket_break();                                                // 必须加此句, 等待下一条输入
        }
        else *state = STATE_EXCEPTION;
        break; }

    case STATE_BOUND: {
        dhcp_app.state = DHCP_OK;
        dhcp_app_config();

        tick_ms   = dhcp_app.lease_time[0];                                     // 计算租约期的一半时间
        tick_ms <<= 16;
        tick_ms  |= dhcp_app.lease_time[1];
        if (tick_ms < (MAX_TICKS32 / 500)) tick_ms *= 500;
        else                               tick_ms  = MAX_TICKS32;
        socket_delay(tick_ms);                                                  // 租约期倒计时
        *state = STATE_RENEWING;
        break; }

    case STATE_RENEWING: {
        xid++;
        tick_ms /= 2;
        *state = STATE_SEND_REQUEST;
        break; }

    case STATE_EXCEPTION:
    default: {
        socket_close();
        DHCP_PRINTF("uip> DHCP exception!!!\r\n");
        dhcp_app.state = DHCP_ERROR;
        dhcp_app_unconfig();
        break; }
    }
}

/**
  ******************************************************************************
  * @brief  DHCP启动
  * @param  none
  * @retval none
  ******************************************************************************
  */
void dhcp_start(void)
{
    uip_udp_conn_t *conn;

    udp_delete(socket_dhcp_client);                                             // 确保删除已有的DHCP SOCKET

    conn = udp_add(socket_dhcp_client, UIP_NULL, (uip_ipaddr_t *)&uip_broadcast_addr, DHCP_SERVER_PORT, DHCP_CLIENT_PORT);
    if (conn == UIP_NULL)                                                       // 添加失败
    {
        dhcp_app.state = DHCP_ERROR;
        DHCP_PRINTF("uip> DHCP start error!!!\r\n");
        dhcp_app.state = DHCP_ERROR;
        dhcp_app_unconfig();
    }
    else
    {
        dhcp_app.state = DHCP_BUSY;
    }
}

/**
  ******************************************************************************
  * @brief  DHCP停止, 并载入默认静态IP地址
  * @param  none
  * @retval none
  ******************************************************************************
  */
void dhcp_stop(void)
{
    udp_delete(socket_dhcp_client);
    dhcp_app.state = DHCP_NONE;
    dhcp_app_unconfig();
}

/**
  ******************************************************************************
  * @brief  获取DHCP状态
  * @param  none
  * @retval 返回值, 存储DHCP当前状态的指针
  ******************************************************************************
  */
dhcp_app_t *dhcp_get(void)
{
    return &dhcp_app;
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
