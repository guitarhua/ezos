/**
  ******************************************************************************
  * @file    drv_rtc_time.h
  * @author  mousie
  * @version V2.0.1
  * @date    2015.04.13
  * @brief   RTC��ʼ��, TIME��غ���
  *
  * RTC�б����ʱ���ʽ����UNIXʱ�����ʽ�ġ���һ��32bit��time_t������ʵΪu32��
  *
  * ANSI-C�ı�׼���У��ṩ�����ֱ�ʾʱ�������  �ͣ�
  * time_t:    UNIXʱ�������1970-1-1��ĳʱ�侭����������
  *   typedef unsigned int time_t;
  *
  * struct tm: Calendar��ʽ����������ʽ��
  *   tm�ṹ���£�
  *   struct tm {
  *       int tm_sec;   // �� seconds after the minute, 0 to 59
  *       int tm_min;   // �� minutes after the hour, 0 to 59
  *       int tm_hour;  // ʱ hours since midnight, 0 to 23
  *       int tm_mday;  // �� day of the month, 1 to 31
  *       int tm_mon;   // �� months since January, 0 to 11
  *       int tm_year;  // �� years since 1900
  *       int tm_wday;  // ���� days since Sunday, 0 to 6
  *       int tm_yday;  // ��Ԫ��������� days since January 1, 0 to 365
  *       int tm_isdst; // ����ʱ. Daylight Savings Time flag
  *       ...
  *  }
  *  ����wday��yday�����Զ����������ֱ�Ӷ�ȡ
  *  mon��ȡֵΪ0-11
  *
  *  ����Ľ�, ע��!!!
  *  1. tm_year: �� time.h ���ж���Ϊ1900��������, ��2010��Ӧ��ʾΪ2010-1900=110
  *              ���ֱ�ʾ������ֱ��, �� drv_rtc_time.h ��, tm_year ��Ϊ 2010
  *              ���ⲿ���ñ��ļ��ĺ���ʱ��tm�ṹ�����͵����ڣ�
  *              ��Ҫ����ϵͳ��time.c�еĺ�������Ҫ���н� tm_year-=1900
  *  2. tm_mon:  �� time.h ���ж���Ϊ0-11, ��Ӧ1-12�·�.
  *              ��ͬ������ֱ��, ��drv_rtc_time.h ��, tm_mon ֵΪ1-12��Ӧ1-12��
  *              ��Ҫ����ϵͳ��time.c�еĺ�������Ҫ���н� tm_mon-=1
  *
  *  �ⲿ����ʵ����
  *  ����һ��Calendar��ʽ�����ڱ�����
  *  struct tm tm_now;
  *  tm_now.tm_year = 2010;
  *  tm_now.tm_mon  = 5;
  *  tm_now.tm_mday = 14;
  *  tm_now.tm_hour = 20;
  *  tm_now.tm_min  = 12;
  *  tm_now.tm_sec  = 30;
  *
  *  ��ȡ��ǰ����ʱ�䣺
  *  tm_now = time_get_calendar();
  *  Ȼ�����ֱ�Ӷ�tm_now.tm_wday��ȡ������
  *
  *  ����ʱ�䣺
  *  Step1. tm_now.xxx = xxxxxxxxx;
  *  Step2. time_set_calendar(tm_now);
  *
  *  ��������ʱ��Ĳ�:
  *  struct tm t1,t2;
  *  t1_t = time_calendar2unix(t1);
  *  t2_t = time_calendar2unix(t2);
  *  dt = t1_t - t2_t;
  *  dt��������ʱ��������
  *  dt_tm = mktime(dt);                // ע��dt�����ƥ��, ansi���к���Ϊ������, ע�ⳬ��
  *  ����Բο��������, ����ansi-c��ĸ�ʽ������ȹ���ctime, strftime��
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
  * @brief    �û�����
  ******************************************************************************
  * @{
  */

/**
  * @}
  */



/** @defgroup DRV_RTC_TIME_Public_TypeDefine
  * @brief    �������Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_RTC_TIME_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_RTC_TIME_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_RTC_TIME_Public_Function
  * @brief    ���幫�к���
  * @{
  */
uint8_t rtc_init(void);                                                         // RTC��ʼ������

struct tm time_get_calendar(void);                                              // ��õ�ǰʱ������
time_t    time_get_unix(void);                                                  // ���UNIXʱ���

void time_set_calendar(struct tm cal_time);                                     // ����ʱ������
void time_set_unix(time_t unix_time);                                           // ����UNIXʱ���

struct tm time_unix2calendar(time_t unix_time);                                 // UNIXʱ���������ʱ��Ļ���
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
