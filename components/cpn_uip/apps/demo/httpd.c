/**
  ******************************************************************************
  * @file    httpd.c
  * @author  mousie
  * @version V1.0.0
  * @date    2013.5.7
  * @brief   提供 webserver 示例.
  ******************************************************************************
  */

/* Include -------------------------------------------------------------------*/
#include "httpd.h"
#include "memb.h"
#include <string.h>



/** @addtogroup Components
  * @{
  */
/** @defgroup HTTPD
  * @{
  */



/**
  ******************************************************************************
  * @addtogroup HTTPD_Configure
  ******************************************************************************
  * @{
  */
#define HTTPD_PRINTF(...)               assert_printf(__VA_ARGS__)
/**
  * @}
  */



/** @defgroup HTTPD_Private_TypeDefine
  * @brief    私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup HTTPD_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */
#define HTTPD_SERVER_PORT               80
/**
  * @}
  */

/** @defgroup HTTPD_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */
// 设置memb的数据结构, 名称, 和可用数量.
// 可理解为在内存开辟了如下一块区域供分配使用,
// httpd_app_t httpd_app[UIP_CONF_MAX_CONNECTIONS];
MEMB(httpd_app_t, httpd_app, UIP_CONF_MAX_CONNECTIONS);

static const char http_index_html[]            = "/index.html";
static const char http_404_html[]              = "/404.html";
//static const char http_referer[]               = "Referer:";
static const char http_header_200[]            = "HTTP/1.0 200 OK\r\nServer: uIP/1.0 http://www.sics.se/~adam/uip/\r\nConnection: close\r\n";
static const char http_header_404[]            = "HTTP/1.0 404 Not found\r\nServer: uIP/1.0 http://www.sics.se/~adam/uip/\r\nConnection: close\r\n";
static const char http_content_type_plain[]    ="Content-type: text/plain\r\n\r\n";
static const char http_content_type_html[]     = "Content-type: text/html\r\n\r\n";
static const char http_content_type_css []     = "Content-type: text/css\r\n\r\n";
//static const char http_content_type_text[]   = "Content-type: text/text\r\n\r\n";
static const char http_content_type_png []     = "Content-type: image/png\r\n\r\n";
static const char http_content_type_gif []     = "Content-type: image/gif\r\n\r\n";
static const char http_content_type_jpg []     = "Content-type: image/jpeg\r\n\r\n";
static const char http_content_type_binary[]   = "Content-type: application/octet-stream\r\n\r\n";
static const char http_html[]                  = ".html";
static const char http_shtml[]                 = ".shtml";
//static const char http_htm[]                   = ".htm";
static const char http_css[]                   = ".css";
static const char http_png[]                   = ".png";
static const char http_gif[]                   = ".gif";
static const char http_jpg[]                   = ".jpg";
//static const char http_txt[]                   = ".txt";
/**
  * @}
  */

/** @defgroup HTTPD_Private_Function
  * @brief    定义私有函数
  * @{
  */
static SOCKET(socket_web_server);
static socket_t send_headers(httpd_app_t *s);
static socket_t send_script(httpd_app_t *s);
static void next_scriptstate(httpd_app_t *s);
/**
  * @}
  */



/** @defgroup HTTPD_Function
  * @brief    函数原文件
  * @{
  */
/**
  ******************************************************************************
  * @brief  web服务器
  * @param  none
  * @retval none
  ******************************************************************************
  */
void example_web_server(void)
{
    memb_init(&httpd_app);
    httpd_fs_init();

    tcp_server_delete(socket_web_server);
    tcp_server_add(socket_web_server, UIP_NULL, HTTPD_SERVER_PORT);
}

/**
  ******************************************************************************
  * @brief  webserver的socket回调处理函数
  * @param  socket回调函数内置参数
  * @retval none
  ******************************************************************************
  */
SOCKET(socket_web_server)
{
    socket_t val;
    httpd_app_t *app = *appdata;                                                // 记录应用层数据结构指针

    if ((*state) && (*appdata == UIP_NULL))                                     // 避免异常情况导致内存泄露, 正常情况不会发生
    {
        socket_close();
        return;
    }

    // 连接被关闭的情况随时可能发生, 因此要在 SOCKET 函数头部处理
    if (socket_closed() == SOCKET_CLOSED)                                       // 客户端主动关闭了链接
    {
        HTTPD_PRINTF("   web was closed by client: 0x%x.\r\n\r\n", app);
        memb_free(&httpd_app, *appdata);                                        // 需释放应用层数据结构空间
        return;
    }


    SOCKET_BEGIN(*state);

    // 初始化入口, 分配此连接的用户数据空间
    *appdata = memb_alloc(&httpd_app);                                          // 获取应用层数据结构空间
    if (*appdata == UIP_NULL)                                                   // 获取失败
    {
        HTTPD_PRINTF("\r\n-- web memb error!\r\n\r\n");
        socket_close();
        return;
    }
    app = *appdata;
    HTTPD_PRINTF("\r\n-- web linked: 0x%x\r\n", app);

    // 准备接收数据"GET /..."
    SOCKET_WAIT_SUBFUN(val = socket_rx((uint8_t *)app->buf, sizeof(app->buf), HTTPD_WAIT_TIME));
    app->buf[socket_len()-1] = 0;
    HTTPD_PRINTF("   web rx:     0x%x\r\n%s\r\n\r\n", app, app->buf);
    if (val == SOCKET_TIMEOUT)         goto __EXCEPTION;                    // 非预期状态的处理
    if (val == SOCKET_ERROR)           goto __EXCEPTION;
    if (strncmp(app->buf, "GET /", 5)) goto __EXCEPTION;

    // 请求路径为 "GET / ", 更新请求页面filename为/index.html
    if(app->buf[5] == ' ')
    {
        strcpy(app->filename, http_index_html);
    }
    else
    {
        char *ptr = strchr(&app->buf[5], ' ');
        if (ptr) *ptr = 0;
        strncpy(app->filename, &app->buf[4], sizeof(app->filename));
    }

    if (!httpd_fs_open(app->filename, &app->file))
    {
        // 没有此页面,打开404页面
        httpd_fs_open(http_404_html, &app->file);
        strcpy(app->filename, http_404_html);

        app->subst = 0;                                                         // 子函数初始化状态寄存器
        app->ptr = (char *)http_header_404;
        SOCKET_WAIT_SUBFUN(val = send_headers(app));
        if (val != SOCKET_OK) goto __EXCEPTION;

        SOCKET_WAIT_SUBFUN(val = socket_tx((uint8_t *)app->file.data, app->file.len, HTTPD_WAIT_TIME));
        HTTPD_PRINTF("   web tx:     0x%x\r\n%s\r\n", app, app->filename);
        if (val != SOCKET_OK) goto __EXCEPTION;
    }
    else
    {
        char *ptr;

        // 正常打印相应页面
        app->subst = 0;
        app->ptr = (char *)http_header_200;
        SOCKET_WAIT_SUBFUN(val = send_headers(app));
        if (val != SOCKET_OK) goto __EXCEPTION;

        // 查找字符串s->filename中首次出现字符'.'的位置,返回首次出现c的位置的指针
        ptr = strchr(app->filename, '.');
        // 判断是否为 .shtml网页文件
        if ((ptr != NULL) && strncmp(ptr, http_shtml, 6) == 0)
        {
            // .shtml页面, 调用handle_script()生成动态网页
            app->subst = 0;
            SOCKET_WAIT_SUBFUN(val = send_script(app));
            if (val != SOCKET_OK) goto __EXCEPTION;
        }
        else
        {
            // .html, 直接输出该页面数据
            SOCKET_WAIT_SUBFUN(val = socket_tx((uint8_t *)app->file.data, app->file.len, HTTPD_WAIT_TIME));
            HTTPD_PRINTF("   web tx:     0x%x\r\n%s\r\n", app, app->filename);
            if (val != SOCKET_OK) goto __EXCEPTION;
        }
    }

    HTTPD_PRINTF("   web close:  0x%x\r\n\r\n", app);
    socket_close();                                                         // 主动关闭连接
    memb_free(&httpd_app, *appdata);                                        // 释放应用层数据结构空间

    SOCKET_END();


__EXCEPTION:
    HTTPD_PRINTF("   web exception: 0x%x\r\n\r\n", app);                    // 异常处理
    socket_close();                                                         // 主动关闭连接
    memb_free(&httpd_app, *appdata);                                        // 释放应用层数据结构空间
}

/**
  ******************************************************************************
  * @brief  webserver支持函数
  * @param  自定义
  * @retval none
  ******************************************************************************
  */
static socket_t send_headers(httpd_app_t *s)
{
    socket_t val;
    char *ptr;

    SOCKET_SUBFUN_BEGIN(s->subst);

    SOCKET_SUBFUN_WAIT_SUBFUN(val = socket_tx((uint8_t *)s->ptr, strlen(s->ptr), HTTPD_WAIT_TIME));
    HTTPD_PRINTF("   web tx:     0x%x\r\n%s\r\n", s, s->ptr);
    if (val != SOCKET_OK) return val;

    ptr = strrchr(s->filename, '.');
    if(ptr == NULL)
        s->ptr = (char *)http_content_type_binary;
    else if ((strncmp(http_html, ptr, 5)) == 0 || (strncmp(http_shtml, ptr, 6) == 0))
        s->ptr = (char *)http_content_type_html;
    else if(strncmp(http_css, ptr, 4) == 0)
        s->ptr = (char *)http_content_type_css;
    else if(strncmp(http_png, ptr, 4) == 0)
        s->ptr = (char *)http_content_type_png;
    else if(strncmp(http_gif, ptr, 4) == 0)
        s->ptr = (char *)http_content_type_gif;
    else if(strncmp(http_jpg, ptr, 4) == 0)
        s->ptr = (char *)http_content_type_jpg;
    else
        s->ptr = (char *)http_content_type_plain;

    SOCKET_SUBFUN_WAIT_SUBFUN(val = socket_tx((uint8_t *)s->ptr, strlen(s->ptr), HTTPD_WAIT_TIME));
    HTTPD_PRINTF("   web tx:     0x%x\r\n%s\r\n", s, s->ptr);
    if (val != SOCKET_OK) return val;

    SOCKET_SUBFUN_END();
}

static socket_t send_script(httpd_app_t *s)
{
    SOCKET_SUBFUN_BEGIN(s->subst);

    while (s->file.len > 0)
    {
        // 检测当前html数据(定义于httpd-fsdata.c)中是否存在字符 %! 和 %!:
        if ((*s->file.data == '%') && (*(s->file.data + 1) == '!'))
        {
            s->ptr = s->file.data + 3;
            s->len = s->file.len - 3;

            if(*(s->ptr - 1) == ':')
            {
                // 若为 %!: 根据其后变量名, 打开并输出相应文件
                // eg.  %!: /header.html 打印 /header.html
                httpd_fs_open(s->ptr + 1, &s->file);
                SOCKET_SUBFUN_WAIT_SUBFUN(socket_tx((uint8_t *)s->file.data, s->file.len, HTTPD_WAIT_TIME));
                HTTPD_PRINTF("   web tx:     0x%x\r\n%s\r\n", s, s->filename);
            }
            else
            {
                // 若为 %! 根据其后变量名,调用相应处理程序(定义于httpd-cgi.c)
                // eg. %! file-stats 调用file-stats 绑定的file_stats()函数,打印出相关数据,实现动态网页
                s->cgist = 0;
                SOCKET_SUBFUN_WAIT_SUBFUN(httpd_cgi(s->ptr)(s));
                HTTPD_PRINTF("   web tx scr: 0x%x\r\n%s\r\n", s, (char *)uip_appdata);
            }
            next_scriptstate(s);

            /* The script is over, so we reset the pointers and continue sending the rest of the file. */
            s->file.data = s->ptr;
            s->file.len  = s->len;
        }
        else
        {
            char *ptr;

            /* See if we find the start of script marker in the block of HTML to be sent. */
            if(*s->file.data == '%') ptr = strchr(s->file.data + 1, '%');
            else                     ptr = strchr(s->file.data, '%');

            if ((ptr != NULL) && (ptr != s->file.data))
                s->len = (uint16_t)(ptr - s->file.data);
            else
                s->len = s->file.len;

            SOCKET_SUBFUN_WAIT_SUBFUN(socket_tx((uint8_t *)s->file.data, s->len, HTTPD_WAIT_TIME));
            HTTPD_PRINTF("   web tx     0x%x:\r\n%s\r\n", s, s->filename);

            s->file.data += s->len;
            s->file.len  -= s->len;
        }
    }

    SOCKET_SUBFUN_END();
}

static void next_scriptstate(httpd_app_t *s)
{
    char *p;

    p = strchr(s->ptr, '\n') + 1;
    s->len -= (uint16_t)(p - s->ptr);
    s->ptr = p;
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
