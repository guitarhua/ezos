// ����
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

    // ���� mac ��ַ
    // OUI 00-04-A3 Microchip Technology, Inc.
    mac.addr[0] = 0x00;
    mac.addr[1] = 0x04;
    mac.addr[2] = 0xA3;
    // generate MAC addr (only for test)
    mac.addr[3] = 0x11;
    mac.addr[4] = 0x22;
    mac.addr[5] = 0x33;

    // uIPϵͳ���ײ�оƬ��ʼ��, IP����ֵ�����Ĭ��ֵ
    uipsys_init(mac);

    // ��ʱ�������״̬������
    quick_add(task_net_check, 10);
}

void net_static_ip_set(void)
{
    uip_ipaddr_t ip;

    // ����IP����ֵ, ��������IP��ַ, ��������, Ĭ������
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

    example_tcp_server(1000);                                                   // TCP����������

    ip_set(&ip, 192, 168, 1, 117);                                              // TCP�ͻ��˷���, ���Զ���������
    example_tcp_client(&ip, 1000);

    example_udp_server(1000);                                                   // UDP����������

    ip_set(&ip, 192, 168, 1, 117);                                              // UDP�ͻ��˷���, ���Զ���������
    example_udp_client(&ip, 1000);

    example_web_server();                                                       // WEB����������

#if UIP_DNS
    quick_add(task_dns, 10);                                                    // DNSʹ�÷���
#endif

#if UIP_SMTP
    quick_add(task_email, 10);                                                  // emailʹ�÷���
#endif
}

EZOS_TASK(task_net_check)
{
    static uipsys_t old = UIPSYS_NULL;
    uipsys_t val;

    val = uipsys_check();
    if (val != old)
    {
        if (val == UIPSYS_UNLINK)                                               // ��Ϊδ����״̬
        {
            uart_printf(COM1, "net_unlink!!!\r\n");
        }
        else if (old == UIPSYS_UNLINK)                                          // ��Ϊ����״̬
        {
            uart_printf(COM1, "net_linked!!!\r\n");
            // ���б�Ҫ, �˴������½��������ʼ������
        }

        if (val == UIPSYS_STATIC_OK)                                            // ʹ�õ�Ĭ�Ͼ�̬IP
        {
            net_static_ip_set();                                                // ���農̬IP
        }

        if ((val & UIPSYS_MASK_OK) == UIPSYS_MASK_OK)                           // val == UIPSYS_XXX_OK, IP��ַ�������
        {
            uart_printf(COM1, "\r\n-- uIP ip infomation\r\n");                  // ��ʾ���úõ�IP��Ϣ
            uart_printf(COM1, "   IP addr: %d.%d.%d.%d\r\n", uip_ipaddr_to_quad(&uip_hostaddr));
            uart_printf(COM1, "   netmask: %d.%d.%d.%d\r\n", uip_ipaddr_to_quad(&uip_netmask));
            uart_printf(COM1, "   router:  %d.%d.%d.%d\r\n", uip_ipaddr_to_quad(&uip_draddr));
            uart_printf(COM1, "   DNS:     %d.%d.%d.%d\r\n\r\n", uip_ipaddr_to_quad(&uip_dnsaddr));
            net_example_add();                                                  // ��ӷ���
        }
#if UIP_DHCP
        else if (val == UIPSYS_DHCP_ERROR)                                      // DHCP����ʧ��
        {
            // �˴�����ش���
            net_static_ip_set();                                                // ���農̬IP
            net_example_add();                                                  // ��ӷ���
        }
        else if (val == UIPSYS_DHCP_BUSY)                                       // DHCP������
        {
            // һ�㲻������
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
#if UIP_DNS                                                                     // ʹ��DNS����SMTP����
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

#else                                                                           // δ��DNS
        email_t email;

        // ���ÿ����ն˴�"nslookup", ��������� smtp.163.com ��IP��ַ
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
