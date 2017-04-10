// web 服务器 script生成数据

#include "httpd.h"
#include <stdio.h>
#include <string.h>



typedef struct
{
    const char *name;
    const httpd_cgifun_t function;
} httpd_cgicall_t;

#define HTTPD_CGI_CALL(name, str, fun)  \
    static socket_t fun(httpd_app_t *); \
    static const httpd_cgicall_t name = {str, fun}

HTTPD_CGI_CALL(file, "file-stats", file_stats);
HTTPD_CGI_CALL(tcp, "tcp-connections", tcp_stats);
HTTPD_CGI_CALL(net, "net-stats", net_stats);

static const httpd_cgicall_t *calls[] = { &file, &tcp, &net, NULL };



/*---------------------------------------------------------------------------*/
static socket_t nullfunction(httpd_app_t *s)
{
    SOCKET_SUBFUN_BEGIN(s->cgist);
    SOCKET_SUBFUN_END();
}

httpd_cgifun_t httpd_cgi(char *name)
{
    const httpd_cgicall_t **f;

    /* Find the matching name in the table, return the function. */
    for(f = calls; *f != NULL; ++f)
    {
        if(strncmp((*f)->name, name, strlen((*f)->name)) == 0)
            return (*f)->function;
    }

    return nullfunction;
}



/*---------------------------------------------------------------------------*/
static socket_t file_stats(httpd_app_t *s)
{
    char *f;

    SOCKET_SUBFUN_BEGIN(s->cgist);

    f = strchr(s->ptr, ' ') + 1;
    s->cgilen = snprintf((char *)uip_appdata, UIP_APPDATA_SIZE, "%5u", httpd_fs_count(f));
    SOCKET_SUBFUN_WAIT_SUBFUN(socket_tx((uint8_t *)uip_appdata, s->cgilen, HTTPD_WAIT_TIME));

    SOCKET_SUBFUN_END();
}



/*---------------------------------------------------------------------------*/
static const char closed[] =   "CLOSED";
static const char syn_rcvd[] = "SYN-RCVD";
static const char syn_sent[] = "SYN-SENT";
static const char established[] = "ESTABLISHED";
static const char fin_wait_1[] = "FIN-WAIT-1";
static const char fin_wait_2[] = "FIN-WAIT-2";
static const char closing[] = "CLOSING";
static const char time_wait[] = "TIME-WAIT,";
static const char last_ack[] = "LAST-ACK";

static const char *states[] =
{
    closed,
    syn_rcvd,
    syn_sent,
    established,
    fin_wait_1,
    fin_wait_2,
    closing,
    time_wait,
    last_ack
};

static socket_t tcp_stats(httpd_app_t *s)
{

    SOCKET_SUBFUN_BEGIN(s->cgist);

    for(s->count = 0; s->count < UIP_CONNS; ++s->count)
    {
        uip_conn_t *conn;
        conn = &uip_conns[s->count];
        if((conn->tcpstateflags & UIP_TS_MASK) != UIP_CLOSED)
        {
            s->cgilen = snprintf((char *)uip_appdata, UIP_APPDATA_SIZE,
                "<tr><td>%d</td><td>%u.%u.%u.%u:%u</td><td>%s</td><td>%u</td><td>%u</td><td>%c %c</td></tr>\r\n",
                uip_htons(conn->lport),
                conn->ripaddr.u8[0],
                conn->ripaddr.u8[1],
                conn->ripaddr.u8[2],
                conn->ripaddr.u8[3],
                uip_htons(conn->rport),
                states[conn->tcpstateflags & UIP_TS_MASK],
                conn->nrtx,
                conn->timer,
                (uip_outstanding(conn))? '*':' ',
                (uip_stopped(conn))? '!':' ');

            SOCKET_SUBFUN_WAIT_SUBFUN(socket_tx((uint8_t *)uip_appdata, s->cgilen, HTTPD_WAIT_TIME));
        }
    }

    SOCKET_SUBFUN_END();
}



/*---------------------------------------------------------------------------*/
static socket_t net_stats(httpd_app_t *s)
{
    SOCKET_SUBFUN_BEGIN(s->cgist);

#if UIP_STATISTICS
    for(s->count = 0; s->count < sizeof(uip_stat) / sizeof(uip_stats_t); ++s->count)
    {
        s->cgilen = snprintf((char *)uip_appdata, UIP_APPDATA_SIZE, "%5u\n", ((uip_stats_t *)&uip_stat)[s->count]);
        SOCKET_SUBFUN_WAIT_SUBFUN(socket_tx((uint8_t *)uip_appdata, s->cgilen, HTTPD_WAIT_TIME));
    }
#endif

    SOCKET_SUBFUN_END();
}
