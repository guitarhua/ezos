// ʵʱʱ��

#include "ezos.h"
#include "drv_rtc_time.h"
#include "drv_uart.h"

EZOS_TASK(task_rtc_1s);

void rtc_start(void)
{
    // ���RTC�����, �س�ʼ��������Ҫ��������ʱ��
    if (rtc_init())
    {
        struct tm  cal_time;
        cal_time.tm_year = 2012;
        cal_time.tm_mon  = 1;
        cal_time.tm_mday = 4;
        cal_time.tm_hour = 10;
        cal_time.tm_min  = 0;
        cal_time.tm_sec  = 0;
        time_set_calendar(cal_time);
    }
    quick_add(task_rtc_1s, 10);
}

EZOS_TASK(task_rtc_1s)
{
    time_t     unix_time;
    struct tm  cal_time;

    // RTC time ����
    unix_time = time_get_unix();
    cal_time  = time_get_calendar();
    uart_printf(COM1, "unix_time: %02x%02x%02x%02x.",                           // ��ʾUnixʱ���
                     (uint8_t)(unix_time>>24),
                     (uint8_t)(unix_time>>16),
                     (uint8_t)(unix_time>>8) ,
                     (uint8_t)(unix_time)   );
    uart_printf(COM1, "cal_time: %02u%02u-%02u-%02u, %02u:%02u:%02u \r\n",      // ��ʾ����ʱ��
                     (uint8_t)(cal_time.tm_year / 100),
                     (uint8_t)(cal_time.tm_year % 100),
                     (uint8_t)(cal_time.tm_mon) ,
                     (uint8_t)(cal_time.tm_mday),
                     (uint8_t)(cal_time.tm_hour),
                     (uint8_t)(cal_time.tm_min) ,
                     (uint8_t)(cal_time.tm_sec) );
    ezos_delay(1000);                                                           // ��ʱ1s
}
