// 网络
#include "user_types.h"
#include "dhcp.h"
#include "dns.h"
#include "smtp.h"
#include "demo/httpd.h"
#include "demo/tcps.h"
#include "demo/tcpc.h"
#include "demo/udps.h"
#include "demo/udpc.h"
#include "tcpip.h"
#include "ezos.h"
#include "drv_uart.h"
#include <string.h>

void net_static_ip_set(void);
void net_example_add(void);
EZOS_TASK(task_net_check);

#if UIP_DNS
    EZOS_TASK(task_dns);
#endif

#if UIP_SMTP
    EZOS_TASK(task_email);
#endif



void net_start(void)
{
    uip_macaddr_t mac;

    // 设置 mac 地址
    // OUI 00-04-A3 Microchip Technology, Inc.
    mac.addr[0] = 0x00;
    mac.addr[1] = 0x04;
    mac.addr[2] = 0xA3;
    // generate MAC addr (only for test)
    mac.addr[3] = 0x11;
    mac.addr[4] = 0x22;
    mac.addr[5] = 0x33;

    // uIP系统及底层芯片初始化, IP属性值会采用默认值
    uipsys_init(mac);

    // 定时检查网络状态的任务
    quick_add(task_net_check, 10);
}

void net_static_ip_set(void)
{
    uip_ipaddr_t ip;

    // 重设IP属性值, 包括本机IP地址, 子网掩码, 默认网关
    ip_set(&ip, 192, 168, 1, 109);
    ip_set_host(&ip);
    ip_set(&ip, 255, 255, 255, 0);
    ip_set_mask(&ip);
    ip_set(&ip, 192, 168, 1, 1);
    ip_set_gateway(&ip);
    ip_set(&ip, 192, 168, 1, 10);
    ip_set_dns(&ip);
}

void net_example_add(void)
{
    uip_ipaddr_t ip;

    example_tcp_server(1000);                                                   // TCP服务器范例

    ip_set(&ip, 192, 168, 1, 117);                                              // TCP客户端范例, 会自动尝试连接
    example_tcp_client(&ip, 1000);

    example_udp_server(1000);                                                   // UDP服务器范例

    ip_set(&ip, 192, 168, 1, 117);                                              // UDP客户端范例, 会自动尝试连接
    example_udp_client(&ip, 1000);

    example_web_server();                                                       // WEB服务器范例

#if UIP_DNS
    quick_add(task_dns, 10);                                                    // DNS使用范例
#endif

#if UIP_SMTP
    quick_add(task_email, 10);                                                  // email使用范例
#endif
}

EZOS_TASK(task_net_check)
{
    static uipsys_t old = UIPSYS_NULL;
    uipsys_t val;

    val = uipsys_check();
    if (val != old)
    {
        if (val == UIPSYS_UNLINK)                                               // 变为未连接状态
        {
            uart_printf(COM1, "net_unlink!!!\r\n");
        }
        else if (old == UIPSYS_UNLINK)                                          // 变为连接状态
        {
            uart_printf(COM1, "net_linked!!!\r\n");
            // 如有必要, 此处可重新进行网络初始化工作
        }

        if (val == UIPSYS_STATIC_OK)                                            // 使用的默认静态IP
        {
            net_static_ip_set();                                                // 重设静态IP
        }

        if ((val & UIPSYS_MASK_OK) == UIPSYS_MASK_OK)                           // val == UIPSYS_XXX_OK, IP地址设置完成
        {
            uart_printf(COM1, "\r\n-- uIP ip infomation\r\n");                  // 显示配置好的IP信息
            uart_printf(COM1, "   IP addr: %d.%d.%d.%d\r\n", uip_ipaddr_to_quad(&uip_hostaddr));
            uart_printf(COM1, "   netmask: %d.%d.%d.%d\r\n", uip_ipaddr_to_quad(&uip_netmask));
            uart_printf(COM1, "   router:  %d.%d.%d.%d\r\n", uip_ipaddr_to_quad(&uip_draddr));
            uart_printf(COM1, "   DNS:     %d.%d.%d.%d\r\n\r\n", uip_ipaddr_to_quad(&uip_dnsaddr));
            net_example_add();                                                  // 添加范例
        }
#if UIP_DHCP
        else if (val == UIPSYS_DHCP_ERROR)                                      // DHCP设置失败
        {
            // 此处做相关处理
            net_static_ip_set();                                                // 重设静态IP
            net_example_add();                                                  // 添加范例
        }
        else if (val == UIPSYS_DHCP_BUSY)                                       // DHCP配置中
        {
            // 一般不做处理
        }
#endif

        old = val;
    }

    ezos_delay(1000);
}

#if UIP_DNS
EZOS_TASK(task_dns)
{
    dns_t val;
    uip_ipaddr_t ip;
    char dns_name[] = "www.baidu.com";

    val = dns_lookup(dns_name, &ip);
    if (val == DNS_OK)
    {
        uart_printf(COM1, "\r\n-- DNS OK!\r\n");
        uart_printf(COM1, "   Name: \"%s\"\r\n", dns_name);
        uart_printf(COM1, "   IP  : %d.%d.%d.%d\r\n\r\n", uip_ipaddr_to_quad(&ip));
    }
    else if (val == DNS_ERROR)
    {
        uart_printf(COM1, "\r\n-- dns error!\r\n");
        uart_printf(COM1, "   Name: \"%s\"\r\n", dns_name);
        uart_printf(COM1, "   IP  : %d.%d.%d.%d\r\n\r\n", uip_ipaddr_to_quad(&ip));
    }
    else ezos_delay(500);
}
#endif

#if UIP_SMTP
EZOS_TASK(task_email)
{
    switch (*state)
    {
    case 0: {
#if UIP_DNS                                                                     // 使用DNS解析SMTP域名
        dns_t val;
        uip_ipaddr_t ip;
        char dns_name[] = "smtp.163.com";

        val = dns_lookup(dns_name, &ip);
        if (val == DNS_OK)
        {
            email_t email;

            uart_printf(COM1, "email server IP: %d.%d.%d.%d\r\n", uip_ipaddr_to_quad(&ip));

            ip_copy(&email.server, &ip);
            email.hostname = "FIRE_STM32_SMTP";
            email.from     = "uip_test@163.com";
            email.to       = "uip_test@163.com";
            email.username = "uip_test@163.com";
            email.password = "abcdef";
            email.subject  = "Testing SMTP from uIP";
            email.msg      = "Testing SMTP from uIP, \r\nby GOLD_BELL!";

            if (smtp_send(&email) == 0)
            {
                *state = 1;
                ezos_delay(200);
            }
            else
            {
                uart_printf(COM1, "email error, busy!\r\n");
            }
        }
        else if (val == DNS_ERROR)
        {
            uart_printf(COM1, "email error, dns failed!\r\n");
        }
        else ezos_delay(500);

#else                                                                           // 未用DNS
        email_t email;

        // 可用控制终端打"nslookup", 查出并填上 smtp.163.com 的IP地址
        ip_set(&email.server, 123,58,178,204);
        email.hostname = "FIRE_STM32_SMTP";
        email.from     = "uip_test@163.com";
        email.to       = "uip_test@163.com";
        email.username = "uip_test@163.com";
        email.password = "abcdef";
        email.subject  = "Testing SMTP from uIP";
        email.msg      = "Testing SMTP from uIP, \r\nby GOLD_BELL!";

        if (smtp_send(&email) == 0)
        {
            *state = 1;
            ezos_delay(200);
        }
        else
        {
            uart_printf(COM1, "email error, busy!\r\n");
        }
#endif
        break; }

    case 1: {
        email_type_t type = smtp_check();

        if (type == EMAIL_OK)
        {
            uart_printf(COM1, "email send ok!\r\n");
        }
        else if (type == EMAIL_PSWERR)
        {
            uart_printf(COM1, "email password error!\r\n");
        }
        else if (type == EMAIL_ERROR)
        {
            *state = 0;
            uart_printf(COM1, "email send error!\r\n");
        }
        else
        {
            ezos_delay(100);
        }
        break; }
    }
}
#endif
