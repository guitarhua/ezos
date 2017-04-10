/**
  ******************************************************************************
  * @file    smtp.c
  * @author  mousie
  * @version V1.0.0
  * @date    2013.4.26
  * @brief   提供 SMTP CLIENT 功能. 发送邮件
  ******************************************************************************
  */

/* Include -------------------------------------------------------------------*/
#include "smtp.h"
#include <string.h>



/** @addtogroup Components
  * @{
  */
/** @defgroup SMTP
  * @{
  */
#if UIP_SMTP



/**
  ******************************************************************************
  * @addtogroup SMTP_Configure
  ******************************************************************************
  * @{
  */
#define SMTP_PRINTF(...)                //assert_printf(__VA_ARGS__)
/**
  * @}
  */



/** @defgroup SMTP_Private_TypeDefine
  * @brief    私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup SMTP_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */
#define SMTP_SERVER_ACK                 0
#define SMTP_HELO_SEND                  1
#define SMTP_HELO_ACK                   2
#define SMTP_AUTH_LOGIN_SEND            3
#define SMTP_AUTH_LOGIN_ACK             4
#define SMTP_USERNAME_SEND              5
#define SMTP_USERNAME_ACK               6
#define SMTP_PASSWORD_SEND              7
#define SMTP_PASSWORD_ACK               8
#define SMTP_MAIL_FROM_SEND             9
#define SMTP_MAIL_FROM_ACK              10
#define SMTP_RCPT_TO_SEND               11
#define SMTP_RCPT_TO_ACK                12
#define SMTP_DATA_SEND                  13
#define SMTP_DATA_ACK                   14
#define SMTP_FROM_SEND                  15
#define SMTP_TO_SEND                    16
#define SMTP_SUBJECT_SEND               17
#define SMTP_MESSAGE_SEND               18
#define SMTP_MESSAGE_ACK                19
#define SMTP_QUIT_SEND                  20
#define SMTP_ERROR_PASSWORD             0xF0
#define SMTP_ERROR_OTHER                0xF1
#define SMTP_CLOSE                      0xFF

#define SMTP_SERVER_PORT                25
#define SMTP_WAIT_TIME                  10000
/**
  * @}
  */

/** @defgroup SMTP_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */
static email_type_t e_state = EMAIL_OK;
static email_t e;
/**
  * @}
  */

/** @defgroup SMTP_Private_Function
  * @brief    定义私有函数
  * @{
  */
static void base64_encode(char *out_str, char *in_str, uint8_t in_len);
static SOCKET(socket_smtp_client);
/**
  * @}
  */



/** @defgroup SMTP_Function
  * @brief    函数原文件
  * @{
  */
/**
  ******************************************************************************
  * @brief  base64 编码, 用于加密用户名和密码
  * @param  out_str, 加密后的字符串
  * @param  in_str,  加密前的字符串
  * @param  in_len,  加密前的字符串长度(需去掉结束符)
  * @retval none
  ******************************************************************************
  */
void base64_encode(char *out_str, char *in_str, uint8_t in_len)
{
    const char base64_table[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    uint16_t i = 0;
    uint16_t curr_out = 0;

    while (i<in_len)
    {
        char a = in_str[i];
        char b = i+1>=in_len?0:in_str[i+1];
        char c = i+2>=in_len?0:in_str[i+2];

        if (i+2<in_len)                                                         // 余0
        {
            out_str[curr_out++]=base64_table[(a>>2)&(0x3f)];                    // the first
            out_str[curr_out++]=base64_table[((a&0x03)<<4)+(b>>4)];             // the first and the second
            out_str[curr_out++]=base64_table[((b&0x0f)<<2)+(c>>6)];             // the second and the third
            out_str[curr_out++]=base64_table[c&0x3f];                           // the third
        }
        else if (i+1<in_len)                                                    // 余2
        {
            out_str[curr_out++]=base64_table[(a>>2)&(0x3f)];                    // the first
            out_str[curr_out++]=base64_table[((a&0x03)<<4)+(b>>4)];             // the first and the second
            out_str[curr_out++]=base64_table[(b&0x0f)<<2];                      // the last low 4 bits of the second
            out_str[curr_out++]='=';                                            // fill with the '='
        }
        else                                                                    // 余1
        {
            out_str[curr_out++]=base64_table[(a>>2)&(0x3f)];                    // the first
            out_str[curr_out++]=base64_table[(a&(0x03))<<4];                    // the rest of of the first
            out_str[curr_out++]='=';                                            // fill with  '=' in the last two bits
            out_str[curr_out++]='=';
        }
        i+=3;
    }
    out_str[curr_out]=0;
};

/**
  ******************************************************************************
  * @brief  smtp客户端socket回调处理函数
  * @param  socket回调函数内置参数
  * @retval none
  ******************************************************************************
  */
SOCKET(socket_smtp_client)
{
    static char buf[100];
    static uint16_t len;

    if ((socket_closed() == SOCKET_CLOSED) && (e_state == EMAIL_SENDING))       // SMTP服务器主动关闭
    {
        if (*state == SMTP_ERROR_PASSWORD)
        {
            SMTP_PRINTF("   SMTP password error!\r\n\r\n");
            e_state = EMAIL_PSWERR;
        }
        else
        {
            SMTP_PRINTF("   SMTP send error!\r\n\r\n");
            e_state = EMAIL_ERROR;
        }
        return;
    }

    switch (*state)
    {
    case SMTP_SERVER_ACK: {                                                     // 等待应答220
        socket_t val;

        val = socket_rx_chr((uint8_t *)buf, 20, '\n', SMTP_WAIT_TIME);
        if (val == SOCKET_WAIT) return;
        buf[socket_len()-1] = 0;
        SMTP_PRINTF("   SMTP_SERVER_ACK:     %s\r\n", buf);
        if (((val == SOCKET_OK) || (val == SOCKET_FULL)) && (memcmp(buf, "220", 3) == 0))
        {
            strcpy(buf, "HELO ");
            strncat(buf, e.hostname, 20);
            strcat(buf, "\r\n");
            *state = SMTP_HELO_SEND;
        }
        else *state = SMTP_ERROR_OTHER;
        break; }

    case SMTP_HELO_SEND: {                                                      // 发送HELO
        socket_t val;

        val = socket_tx((uint8_t *)buf, strlen(buf), SMTP_WAIT_TIME);
        if (val == SOCKET_WAIT) return;
        SMTP_PRINTF("   SMTP_HELO_SEND:      %s", buf);
        if (val == SOCKET_OK) *state = SMTP_HELO_ACK;
        else                  *state = SMTP_ERROR_OTHER;
        break; }

    case SMTP_HELO_ACK: {                                                       // 等待应答250
        socket_t val;

        val = socket_rx_chr((uint8_t *)buf, 20, '\n', SMTP_WAIT_TIME);
        if (val == SOCKET_WAIT) return;
        buf[socket_len()-1] = 0;
        SMTP_PRINTF("   SMTP_HELO_ACK:       %s\r\n", buf);
        if (((val == SOCKET_OK) || (val == SOCKET_FULL)) && (buf[0] == '2'))
        {
            if (strlen(e.username))
            {
                strcpy(buf, "AUTH LOGIN\r\n");
                *state = SMTP_AUTH_LOGIN_SEND;
            }
            else
            {
                strcpy(buf, "MAIL FROM: <");
                strncat(buf, e.from, 40);
                strcat(buf, ">\r\n");
                *state = SMTP_MAIL_FROM_SEND;
            }
        }
        else *state = SMTP_ERROR_OTHER;
        break; }

    case SMTP_AUTH_LOGIN_SEND: {                                                // 发送AUTH LOGIN
        socket_t val;

        val = socket_tx((uint8_t *)buf, strlen(buf), SMTP_WAIT_TIME);
        if (val == SOCKET_WAIT) return;
        SMTP_PRINTF("   SMTP_AUTH_LOGIN_SEND:%s", buf);
        if (val == SOCKET_OK) *state = SMTP_AUTH_LOGIN_ACK;
        else                  *state = SMTP_ERROR_OTHER;
        break; }

    case SMTP_AUTH_LOGIN_ACK: {                                                 // 等待应答334
        socket_t val;

        val = socket_rx_chr((uint8_t *)buf, 20, '\n', SMTP_WAIT_TIME);
        if (val == SOCKET_WAIT) return;
        buf[socket_len()-1] = 0;
        SMTP_PRINTF("   SMTP_AUTH_LOGIN_ACK: %s\r\n", buf);
        if (((val == SOCKET_OK) || (val == SOCKET_FULL)) && (buf[0] == '3'))
        {
            len = strlen(e.username);

            if (len < 40)
            {
                base64_encode(buf, e.username, len);
                strcat(buf, "\r\n");
                *state = SMTP_USERNAME_SEND;
                break;
            }
            *state = SMTP_ERROR_PASSWORD;                                       // 用户名密码错误
        }
        else *state = SMTP_ERROR_OTHER;
        break; }

    case SMTP_USERNAME_SEND: {                                                  // 发送SMTP用户名
        socket_t val;

        val = socket_tx((uint8_t *)buf, strlen(buf), SMTP_WAIT_TIME);
        if (val == SOCKET_WAIT) return;
        SMTP_PRINTF("   SMTP_USERNAME_SEND:  %s", buf);
        if (val == SOCKET_OK) *state = SMTP_USERNAME_ACK;
        else                  *state = SMTP_ERROR_OTHER;
        break; }

    case SMTP_USERNAME_ACK: {                                                   // 等待应答334
        socket_t val;

        val = socket_rx_chr((uint8_t *)buf, 20, '\n', SMTP_WAIT_TIME);
        if (val == SOCKET_WAIT) return;
        buf[socket_len()-1] = 0;
        SMTP_PRINTF("   SMTP_USERNAME_ACK:   %s\r\n", buf);
        if ((val == SOCKET_OK) || (val == SOCKET_FULL))
        {
            if (buf[0] == '3')
            {
                uint8_t len = strlen(e.password);

                if (len < 20)
                {
                    base64_encode(buf, e.password, len);
                    strcat(buf, "\r\n");
                    *state = SMTP_PASSWORD_SEND;
                    break;
                }
            }
            *state = SMTP_ERROR_PASSWORD;                                       // 用户名密码错误
        }
        else *state = SMTP_ERROR_OTHER;
        break; }

    case SMTP_PASSWORD_SEND: {                                                  // 发送SMTP密码
        socket_t val;

        val = socket_tx((uint8_t *)buf, strlen(buf), SMTP_WAIT_TIME);
        if (val == SOCKET_WAIT) return;
        SMTP_PRINTF("   SMTP_PASSWORD_SEND:  %s", buf);
        if (val == SOCKET_OK) *state = SMTP_PASSWORD_ACK;
        else                  *state = SMTP_ERROR_OTHER;
        break; }

    case SMTP_PASSWORD_ACK: {                                                   // 等待应答235
        socket_t val;

        val = socket_rx_chr((uint8_t *)buf, 20, '\n', SMTP_WAIT_TIME);
        if (val == SOCKET_WAIT) return;
        buf[socket_len()-1] = 0;
        SMTP_PRINTF("   SMTP_PASSWORD_ACK:   %s\r\n", buf);
        if ((val == SOCKET_OK) || (val == SOCKET_FULL))
        {
            if (buf[0] == '2')
            {
                strcpy(buf, "MAIL FROM: <");
                strncat(buf, e.from, 40);
                strcat(buf, ">\r\n");
                *state = SMTP_MAIL_FROM_SEND;
                break;
            }
            *state = SMTP_ERROR_PASSWORD;                                       // 用户名密码错误
        }
        else *state = SMTP_ERROR_OTHER;
        break; }

    case SMTP_MAIL_FROM_SEND: {                                                 // 发送MAIL FROM
        socket_t val;

        val = socket_tx((uint8_t *)buf, strlen(buf), SMTP_WAIT_TIME);
        if (val == SOCKET_WAIT) return;
        SMTP_PRINTF("   SMTP_MAIL_FROM_SEND: %s", buf);
        if (val == SOCKET_OK) *state = SMTP_MAIL_FROM_ACK;
        else                  *state = SMTP_ERROR_OTHER;
        break; }

    case SMTP_MAIL_FROM_ACK: {                                                  // 等待应答250
        socket_t val;

        val = socket_rx_chr((uint8_t *)buf, 20, '\n', SMTP_WAIT_TIME);
        if (val == SOCKET_WAIT) return;
        buf[socket_len()-1] = 0;
        SMTP_PRINTF("   SMTP_MAIL_FROM_ACK:  %s\r\n", buf);
        if (((val == SOCKET_OK) || (val == SOCKET_FULL)) && (buf[0] == '2'))
        {
            strcpy(buf, "RCPT TO: <");
            strncat(buf, e.to, 40);
            strcat(buf, ">\r\n");
            *state = SMTP_RCPT_TO_SEND;
        }
        else *state = SMTP_ERROR_OTHER;
        break; }

    case SMTP_RCPT_TO_SEND: {                                                   // 发送RCPT TO
        socket_t val;

        // 如果有多个邮件接收人, 可重复此步骤. 此代码未实现邮件多接收人
        val = socket_tx((uint8_t *)buf, strlen(buf), SMTP_WAIT_TIME);
        if (val == SOCKET_WAIT) return;
        SMTP_PRINTF("   SMTP_RCPT_TO_SEND:   %s", buf);
        if (val == SOCKET_OK) *state = SMTP_RCPT_TO_ACK;
        else                  *state = SMTP_ERROR_OTHER;
        break; }

    case SMTP_RCPT_TO_ACK: {                                                    // 等待应答250
        socket_t val;

        val = socket_rx_chr((uint8_t *)buf, 20, '\n', SMTP_WAIT_TIME);
        if (val == SOCKET_WAIT) return;
        buf[socket_len()-1] = 0;
        SMTP_PRINTF("   SMTP_RCPT_TO_ACK:    %s\r\n", buf);
        if (((val == SOCKET_OK) || (val == SOCKET_FULL)) && (buf[0] == '2'))
        {
            strcpy(buf, "DATA\r\n");
            *state = SMTP_DATA_SEND;
        }
        else *state = SMTP_ERROR_OTHER;
        break; }

    case SMTP_DATA_SEND: {                                                      // 发送DATA, 请求发送数据
        socket_t val;

        val = socket_tx((uint8_t *)buf, strlen(buf), SMTP_WAIT_TIME);
        if (val == SOCKET_WAIT) return;
        SMTP_PRINTF("   SMTP_DATA_SEND:      %s", buf);
        if (val == SOCKET_OK) *state = SMTP_DATA_ACK;
        else                  *state = SMTP_ERROR_OTHER;
        break; }

    case SMTP_DATA_ACK: {                                                       // 等待应答354
        socket_t val;

        val = socket_rx_chr((uint8_t *)buf, 20, '\n', SMTP_WAIT_TIME);
        if (val == SOCKET_WAIT) return;
        buf[socket_len()-1] = 0;
        SMTP_PRINTF("   SMTP_DATA_ACK:       %s\r\n", buf);
        if (((val == SOCKET_OK) || (val == SOCKET_FULL)) && (buf[0] == '3'))
        {
            strcpy(buf, "From: ");
            strncat(buf, e.from, 40);
            strcat(buf, "\r\n");
            *state = SMTP_FROM_SEND;
        }
        else *state = SMTP_ERROR_OTHER;
        break; }

    case SMTP_FROM_SEND: {                                                      // 发送From
        socket_t val;

        val = socket_tx((uint8_t *)buf, strlen(buf), SMTP_WAIT_TIME);
        if (val == SOCKET_WAIT) return;
        SMTP_PRINTF("   SMTP_FROM_SEND:      %s", buf);
        if (val == SOCKET_OK)
        {
            strcpy(buf, "To: ");
            strncat(buf, e.to, 40);
            strcat(buf, "\r\n");
            *state = SMTP_TO_SEND;
        }
        else *state = SMTP_ERROR_OTHER;
        break; }

    case SMTP_TO_SEND: {                                                        // 发送To
        socket_t val;

        val = socket_tx((uint8_t *)buf, strlen(buf), SMTP_WAIT_TIME);
        if (val == SOCKET_WAIT) return;
        SMTP_PRINTF("   SMTP_TO_SEND:        %s", buf);
        if (val == SOCKET_OK)
        {
            strcpy(buf, "Subject: ");
            strncat(buf, e.subject, 60);
            strcat(buf, "\r\n\r\n");
            *state = SMTP_SUBJECT_SEND;
        }
        else *state = SMTP_ERROR_OTHER;
        break; }

    case SMTP_SUBJECT_SEND: {                                                   // 发送Subject
        socket_t val;

        val = socket_tx((uint8_t *)buf, strlen(buf), SMTP_WAIT_TIME);
        if (val == SOCKET_WAIT) return;
        SMTP_PRINTF("   SMTP_SUBJECT_SEND:   %s", buf);
        if (val == SOCKET_OK)
        {
            len = strlen(e.msg);
            strncpy(buf, e.msg, 90);
            if (len <= 90) strcat(buf, "\r\n.\r\n");
            *state = SMTP_MESSAGE_SEND;
        }
        else *state = SMTP_ERROR_OTHER;
        break; }

    case SMTP_MESSAGE_SEND: {                                                   // 发送邮件正文
        socket_t val;

        val = socket_tx((uint8_t *)buf, strlen(buf), SMTP_WAIT_TIME);
        if (val == SOCKET_WAIT) return;
        SMTP_PRINTF("   SMTP_MESSAGE_SEND:   %s", buf);
        if (val == SOCKET_OK)
        {
            if (len <= 90)                                                      // 已发送完成
            {
                *state = SMTP_MESSAGE_ACK;
            }
            else                                                                // 未发送完成, 继续发送
            {
                len   -= 90;
                e.msg += 90;
                strncpy(buf, e.msg, 90);
                if (len <= 90) strcat(buf, "\r\n.\r\n");
                // *state = SMTP_MESSAGE_SEND;
            }
        }
        else *state = SMTP_ERROR_OTHER;
        break; }

    case SMTP_MESSAGE_ACK: {                                                    // 等待应答250
        socket_t val;

        val = socket_rx_chr((uint8_t *)buf, 20, '\n', SMTP_WAIT_TIME);
        if (val == SOCKET_WAIT) return;
        buf[socket_len()-1] = 0;
        SMTP_PRINTF("   SMTP_MESSAGE_ACK:    %s\r\n", buf);
        if (((val == SOCKET_OK) || (val == SOCKET_FULL)) && (buf[0] == '2'))
        {
            strcpy(buf, "QUIT\r\n");
            *state = SMTP_QUIT_SEND;
        }
        else *state = SMTP_ERROR_OTHER;
        break; }

    case SMTP_QUIT_SEND: {                                                      // 发送QUIT, 返回码应为221
        socket_t val;

        val = socket_tx((uint8_t *)buf, strlen(buf), SMTP_WAIT_TIME);
        if (val == SOCKET_WAIT) return;
        SMTP_PRINTF("   SMTP_QUIT_SEND:      %s", buf);
        if (val == SOCKET_OK)
        {
            e_state = EMAIL_OK;
            SMTP_PRINTF("   SMTP send ok!\r\n\r\n");
            *state = SMTP_CLOSE;
        }
        else *state = SMTP_ERROR_OTHER;
        break; }

    case SMTP_ERROR_PASSWORD:
    case SMTP_ERROR_OTHER: {
        socket_t val;

        val = socket_tx((uint8_t *)"QUIT\r\n", 6, SMTP_WAIT_TIME);
        if (val == SOCKET_WAIT) return;
        if (*state == SMTP_ERROR_PASSWORD)
        {
            SMTP_PRINTF("   SMTP password error!\r\n\r\n");
            e_state = EMAIL_PSWERR;
        }
        else
        {
            SMTP_PRINTF("   SMTP send error!\r\n\r\n");
            e_state = EMAIL_ERROR;
        }

        *state = SMTP_CLOSE;
        break; }

    case SMTP_CLOSE:
    default: {
        socket_close();
        break; }
    }
}

/**
  ******************************************************************************
  * @brief  SMTP发送邮件.
  * @param  email, 要发送邮件的信息指针. 指向内容必须为全局变量或静态变量
  * @retval 返回值, 0, 发送添加成功, 1, 发送添加失败.
  ******************************************************************************
  */
uint8_t smtp_send(email_t *email)
{
    uip_conn_t *conn;

    if (tcp_client_get(socket_smtp_client)) return 1;                           // 有邮件未处理完

    conn = tcp_client_add(socket_smtp_client, UIP_NULL, &email->server, SMTP_SERVER_PORT);
    if (conn == UIP_NULL) return 1;

    e_state    = EMAIL_SENDING;
    e.hostname = email->hostname;
    e.from     = email->from;
    e.to       = email->to;
    e.username = email->username;
    e.password = email->password;
    e.subject  = email->subject;
    e.msg      = email->msg;

    SMTP_PRINTF("\r\n-- SMTP send start!\r\n");

    return 0;
}

/**
  ******************************************************************************
  * @brief  SMTP检查最后一次发送的邮件状态.
  * @param  none
  * @retval 返回值, email_type_t 类型
  ******************************************************************************
  */
email_type_t smtp_check(void)
{
    return e_state;
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
