/**
  ******************************************************************************
  * @file    httpd.h
  * @author  mousie
  * @version V1.0.0
  * @date    2013.5.7
  * @brief   提供 webserver 示例.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HTTPD_H__
#define __HTTPD_H__

/* Include -------------------------------------------------------------------*/
#include "tcpip.h"



/** @addtogroup Components
  * @{
  */
/** @addtogroup HTTPD
  * @{
  */



/**
  ******************************************************************************
  * @defgroup HTTPD_Configure
  * @brief    用户配置
  ******************************************************************************
  * @{
  */
/// WEB服务器 文件统计功能
#define HTTPD_FS_STATISTICS             1

/// WEB服务器 收发超时定义
#define HTTPD_WAIT_TIME                 10000
/**
  * @}
  */



/** @defgroup HTTPD_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */
struct httpd_fs_file
{
    char *data;
    int16_t len;
};

struct httpd_fsdata_file
{
    const struct httpd_fsdata_file *next;
    const char *name;
    const char *data;
    const int16_t len;
#if HTTPD_FS_STATISTICS
    uint16_t count;
#endif
};

struct httpd_fsdata_file_noconst
{
    struct httpd_fsdata_file *next;
    char *name;
    char *data;
    int16_t len;
#if HTTPD_FS_STATISTICS
    uint16_t count;
#endif
};

typedef struct
{
    char buf[50];
    char filename[20];
    struct httpd_fs_file file;
    char *ptr;
    int16_t len;
    uint16_t subst;
    int16_t cgilen;
    uint16_t cgist;
    uint8_t count;
} httpd_app_t;

typedef socket_t (*httpd_cgifun_t)(httpd_app_t *);
/**
  * @}
  */

/** @defgroup HTTPD_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup HTTPD_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup HTTPD_Public_Function
  * @brief    定义公有函数
  * @{
  */
void example_web_server(void);

httpd_cgifun_t httpd_cgi(char *name);
void httpd_fs_init(void);
uint8_t httpd_fs_open(const char *name, struct httpd_fs_file *file);
#if HTTPD_FS_STATISTICS
    uint16_t httpd_fs_count(char *name);
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
