/**
  ******************************************************************************
  * @file    drv_rtc_time.h
  * @author  mousie
  * @version V2.0.1
  * @date    2015.04.13
  * @brief   RTC初始化, TIME相关函数
  *
  * RTC中保存的时间格式，是UNIX时间戳格式的。即一个32bit的time_t变量（实为u32）
  *
  * ANSI-C的标准库中，提供了两种表示时间的数据  型：
  * time_t:    UNIX时间戳（从1970-1-1起到某时间经过的秒数）
  *   typedef unsigned int time_t;
  *
  * struct tm: Calendar格式（年月日形式）
  *   tm结构如下：
  *   struct tm {
  *       int tm_sec;   // 秒 seconds after the minute, 0 to 59
  *       int tm_min;   // 分 minutes after the hour, 0 to 59
  *       int tm_hour;  // 时 hours since midnight, 0 to 23
  *       int tm_mday;  // 日 day of the month, 1 to 31
  *       int tm_mon;   // 月 months since January, 0 to 11
  *       int tm_year;  // 年 years since 1900
  *       int tm_wday;  // 星期 days since Sunday, 0 to 6
  *       int tm_yday;  // 从元旦起的天数 days since January 1, 0 to 365
  *       int tm_isdst; // 夏令时. Daylight Savings Time flag
  *       ...
  *  }
  *  其中wday，yday可以自动产生，软件直接读取
  *  mon的取值为0-11
  *
  *  代码改进, 注意!!!
  *  1. tm_year: 在 time.h 库中定义为1900年起的年份, 即2010年应表示为2010-1900=110
  *              这种表示方法不直观, 在 drv_rtc_time.h 中, tm_year 即为 2010
  *              即外部调用本文件的函数时，tm结构体类型的日期，
  *              若要调用系统库time.c中的函数，需要自行将 tm_year-=1900
  *  2. tm_mon:  在 time.h 库中定义为0-11, 对应1-12月份.
  *              这同样不够直观, 在drv_rtc_time.h 中, tm_mon 值为1-12对应1-12月
  *              若要调用系统库time.c中的函数，需要自行将 tm_mon-=1
  *
  *  外部调用实例：
  *  定义一个Calendar格式的日期变量：
  *  struct tm tm_now;
  *  tm_now.tm_year = 2010;
  *  tm_now.tm_mon  = 5;
  *  tm_now.tm_mday = 14;
  *  tm_now.tm_hour = 20;
  *  tm_now.tm_min  = 12;
  *  tm_now.tm_sec  = 30;
  *
  *  获取当前日期时间：
  *  tm_now = time_get_calendar();
  *  然后可以直接读tm_now.tm_wday获取星期数
  *
  *  设置时间：
  *  Step1. tm_now.xxx = xxxxxxxxx;
  *  Step2. time_set_calendar(tm_now);
  *
  *  计算两个时间的差:
  *  struct tm t1,t2;
  *  t1_t = time_calendar2unix(t1);
  *  t2_t = time_calendar2unix(t2);
  *  dt = t1_t - t2_t;
  *  dt就是两个时间差的秒数
  *  dt_tm = mktime(dt);                // 注意dt的年份匹配, ansi库中函数为相对年份, 注意超限
  *  另可以参考相关资料, 调用ansi-c库的格式化输出等功能ctime, strftime等
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_RTC_TIME_H
#define __DRV_RTC_TIME_H

/* Include -------------------------------------------------------------------*/
#include <stdint.h>
#include <time.h>



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup DRV_RTC_TIME
  * @{
  */



/**
  ******************************************************************************
  * @defgroup DRV_RTC_TIME_Configure
  * @brief    用户配置
  ******************************************************************************
  * @{
  */

/**
  * @}
  */



/** @defgroup DRV_RTC_TIME_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_RTC_TIME_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_RTC_TIME_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_RTC_TIME_Public_Function
  * @brief    定义公有函数
  * @{
  */
uint8_t rtc_init(void);                                                         // RTC初始化函数

struct tm time_get_calendar(void);                                              // 获得当前时间日期
time_t    time_get_unix(void);                                                  // 获得UNIX时间戳

void time_set_calendar(struct tm cal_time);                                     // 设置时间日期
void time_set_unix(time_t unix_time);                                           // 设置UNIX时间戳

struct tm time_unix2calendar(time_t unix_time);                                 // UNIX时间戳和日期时间的互换
time_t    time_calendar2unix(struct tm cal_time);
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
