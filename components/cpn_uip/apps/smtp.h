/**
  ******************************************************************************
  * @file    smtp.h
  * @author  mousie
  * @version V1.0.0
  * @date    2013.4.26
  * @brief   提供 SMTP CLIENT 功能. 发送邮件
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SMTP_H
#define __SMTP_H

/* Include -------------------------------------------------------------------*/
#include <stdint.h>
#include "tcpip.h"



/** @addtogroup Components
  * @{
  */
/** @addtogroup SMTP
  * @{
  */



/**
  ******************************************************************************
  * @defgroup SMTP_Configure
  * @brief    用户配置
  ******************************************************************************
  * @{
  */
///// 配置SMTP客户端功能(发送邮件). 0-不使能, 1-使能
//#define UIP_SMTP                        0
/**
  * @}
  */



/** @defgroup SMTP_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */
typedef enum
{
    EMAIL_OK = 0,                       // EMAIL 发送成功
    EMAIL_SENDING,                      // EMAIL 发送中
    EMAIL_ERROR,                        // EMAIL 发送失败, 通讯错误, 可重发
    EMAIL_PSWERR,                       // EMAIL 发送失败, 用户名密码错误
} email_type_t;

typedef struct
{
    uip_ipaddr_t server;                // 服务器IP地址
    char *hostname;                     // 主机名称
    char *from;                         // 发送端 mail
    char *to;                           // 接收端 mail
    char *username;                     // 登录用户名
    char *password;                     // 登录密码
    char *subject;                      // 邮件标题
    char *msg;                          // 邮件正文
} email_t;
/**
  * @}
  */

/** @defgroup SMTP_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup SMTP_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup SMTP_Public_Function
  * @brief    定义公有函数
  * @{
  */
#if UIP_SMTP

uint8_t      smtp_send(email_t *email);                                         // 发送邮件
email_type_t smtp_check(void);                                                  // 检查邮件

#endif
/**
  * @}
  */



/**
  * @}
  */
/**
  * @}
  */

#endif
/* END OF FILE ---------------------------------------------------------------*/
