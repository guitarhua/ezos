/**
  ******************************************************************************
  * @file    drv_rtc_time.c
  * @author  mousie
  * @version V2.0.1
  * @date    2015.04.13
  * @brief   RTC初始化, TIME相关函数
  ******************************************************************************
  */

/* Include -------------------------------------------------------------------*/
#include "drv_rtc_time.h"

#include "stm32f10x_bkp.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_rtc.h"



/** @addtogroup Drivers
  * @{
  */
/** @defgroup DRV_RTC_TIME
  * @{
  */



/**
  ******************************************************************************
  * @addtogroup DRV_RTC_TIME_Configure
  ******************************************************************************
  * @{
  */
/// 将RTC的时钟通过64分频后在芯片的侵入检测引脚上输出. 可供检测时间精度使用
//#define RTC_CLOCK_OUTPUT_ENABLE
/**
  * @}
  */



/** @defgroup DRV_RTC_TIME_Private_TypeDefine
  * @brief    私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_RTC_TIME_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_RTC_TIME_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_RTC_TIME_Private_Function
  * @brief    定义私有函数
  * @{
  */

/**
  * @}
  */



/** @defgroup DRV_RTC_TIME_Function
  * @brief    函数原文件
  * @{
  */

/**
  ******************************************************************************
  * @brief  RTC 实时时钟初始化
  * @param  none
  * @retval rvalue
  *   @arg  0, RTC正常运行, 表明已经初始化过, 没有掉电
  *   @arg  1, RTC重初始化, 表明RTC之前掉电了
  *   @arg  0xFF, RTC初始化失败, RTC未能启动
  ******************************************************************************
  */
uint8_t rtc_init(void)
{
    uint8_t rvalue = 0xFF;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);                                                // RTC和后备域解锁

    // 检测特定的备寄存器RTC配置标记(该寄存器也由RTC电池供电), RTC掉电后则需要重新初始化, 否则RTC已经在工作中
    if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
    {
        uint32_t i;
        BKP_DeInit();                                                           // 备份寄存器模块复位
        RCC_LSEConfig(RCC_LSE_ON);                                              // 使能LSE(低速外部晶振), 32.768KHz
        for (i=0; i<5000000; i++) {
            if (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == SET)                      // 等待起振
            {
                RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);                         // RTC时钟源配置成LSE
                RCC_RTCCLKCmd(ENABLE);
                RTC_WaitForSynchro();                                           // 等待APB1时钟和RTC时钟同步

                RTC_WaitForLastTask();                                          // 读写寄存器前, 确定上一个操作已经结束
                RTC_SetPrescaler(32767);                                        // 设置RTC分频器, 使RTC时钟为1Hz
                RTC_WaitForLastTask();                                          // 等待寄存器写入完成

                BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);                       // 向后备寄存器中写入RTC配置成功标记
                rvalue = 1;
                break;
            }
        }
    }
    else
    {
        RTC_WaitForSynchro();                                                   // 等待时钟同步
        rvalue = 0;
    }

#ifdef RTC_CLOCK_OUTPUT_ENABLE
    BKP_TamperPinCmd(DISABLE);                                                  // 禁用侵入检测功能
    BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);                        // 将RTC时钟64分频后输出到侵入检测引脚上
#endif

    return rvalue;
}

/**
  ******************************************************************************
  * @brief  转换UNIX时间戳为日历时间
  * @param  unix_time, UNIX时间戳
  * @retval 返回值,   日历时间结构体指针
  ******************************************************************************
  */
struct tm time_unix2calendar(time_t unix_time)
{
    struct tm *cal_time;
    cal_time = localtime(&unix_time);
    // time.h 转换结果的tm_year是相对值(基于1900年), 需要转成日常值
    cal_time->tm_year += 1940;
    // time.h 转换结果的tm_mon 0表示1月份, 故要加一符合日常值
    cal_time->tm_mon++;

    return *cal_time;
}

/**
  ******************************************************************************
  * @brief  转换日历时间为UNIX时间戳
  * @param  cal_time, 日历时间
  * @retval 返回值,  UNIX时间戳
  ******************************************************************************
  */
time_t time_calendar2unix(struct tm cal_time)
{
    // time.h 中定义的年份格式为1900年开始的年份, 使用time.h中的函数要将年份转为基于1900年开始.
    cal_time.tm_year -= 1940;
    // time.h 中定义的 tm_mon 0 对应 1月份, 故要对实际的月份减一后处理.
    cal_time.tm_mon--;
    return mktime(&cal_time);
}

/**
  ******************************************************************************
  * @brief  从RTC取当前时间的日历时间(struct tm)
  * @param  none
  * @retval 返回值,   日历时间结构体指针
  ******************************************************************************
  */
struct tm time_get_calendar(void)
{
    time_t unix_time;
    struct tm cal_time;

    unix_time = (time_t)RTC_GetCounter();
    cal_time  = time_unix2calendar(unix_time);
    return cal_time;
}

/**
  ******************************************************************************
  * @brief  从RTC取当前时间的Unix时间戳值
  * @param  none
  * @retval 返回值,  UNIX时间戳
  ******************************************************************************
  */
time_t time_get_unix(void)
{
    return (time_t)RTC_GetCounter();
}

/**
  ******************************************************************************
  * @brief  将给定的Calendar格式时间转换成UNIX时间戳写入RTC
  * @param  cal_time, 日历时间
  * @retval none
  ******************************************************************************
  */
void time_set_calendar(struct tm cal_time)
{
    time_set_unix(time_calendar2unix(cal_time));
    return;
}

/**
  ******************************************************************************
  * @brief  将给定的Unix时间戳写入RTC
  * @param  unix_time, UNIX时间戳
  * @retval none
  ******************************************************************************
  */
void time_set_unix(time_t unix_time)
{
    RTC_WaitForLastTask();
    RTC_SetCounter((u32)unix_time);
    RTC_WaitForLastTask();
    return;
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
