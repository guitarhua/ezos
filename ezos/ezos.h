/**
  ******************************************************************************
  * @file    ezos.h
  * @author  mousie
  * @version V2.3.0
  * @date    2014.3.28
  * @brief   协作式RTOS
  *
  *   任务的状态:
  *       1. 任务删除状态, 即终止状态. 任务不在任务链表中, 不会运行.
  *       2. 任务阻塞状态, 任务已在任务链表中, 但被阻塞, 不会被运行. 任务延时也属于阻塞状态.
  *       3. 任务就绪状态, 任务已在任务队列中, 可以运行, 但未必正在运行.
  *       4. 任务运行状态, 任务已在任务列表中, 并正在运行.
  *
  *   ezos任务应用技巧:
  *       1. 首先调用ezos_init(); 初始化ezos参数
  *       2. 设置一个1ms定时中断作为ezos的时钟, 并调用ezos_scan_1ms();
  *       3. 在main函数的最后, 调用 ezos_schedule();
  *       4. 一般地, 在调度之前至少创建一个任务, 如:
  *          ezos_add(task_key_scan, task_key_scan, (void *)0, 10, 8);
  *          该函数表示添加一个按键扫描任务, 任务名称同函数名, 延时10ms后运行, 优先级为8.
  *       5. 如果任务运行中没有改变过状态(如调用 ezos_delay() 函数, 任务间的通讯函数), 则会在任务结束后自动删除该任务.
  *       6. 所有的任务操作依据就是任务的名称. 任务的名称是不可重复的.
  *          为了使用方便, 任务名称被设计成 void * 的格式, 可非常方便的使用函数名称作为任务名称.
  *          另特别宏定义了快速添加任务的函数: quick_add(pfun, prio);
  *          quick_add(func, prio);                                             // 添加一个任务, 无需函数参数, 任务名称同函数名
  *       7. 要对同一个函数添加多个任务, 必须保证该函数是可重入的或是线程安全的函数.
  *          调用时只要保证任务名称不一样即可. 任务名称可使用(函数名+0,1,2)的方式. 方便使用和记忆. 如:
  *          ezos_add(func, func, (void *)0, 0, 0);                             // 调用func函数的第一个任务, 任务名称 func
  *          ezos_add(EZOS_NAME(func, 1), func, (void *)0, 0, 0);               // 调用func函数的第二个任务, 任务名称 func+1
  *       8. 空闲任务钩子函数的使用建议.
  *          钩子函数只能是 void func(void) 形式的简单函数.
  *          一般建议用来喂狗, 或 翻转系统状态指示灯.
  *       9. 任务基础应用: 状态机方式.
  *          任务占用CPU时间过长或中间需要延时等待, 让该任务进入阻塞状态.
  *          使用状态机方式可让任务暂时放弃对CPU的占用.
  *          这是保证高优先级任务响应时间的一种妥协方式, 也可以提高CPU的利用率.
  *          以LED跑马灯为例, 形式如下:
  *          ezos_task_t task_led(ezos_state_t *state, ezos_para_t *para)
  *          {
  *             switch (*state)
  *             {
  *             case 0:                         // 任务添加后的初始状态, 可以做些任务的初始化工作
  *                 led_toggle(LED1);           // LED1翻转
  *                 ezos_delay(1000);           // 需要延时1S, 设置下一状态并放弃对CPU的占用
  *                 *state = 1;
  *                 break;
  *             default:
  *                 led_toggle(LED2);           // LED2翻转
  *                 ezos_delay(1000);           // 需要延时1S, 设置下一状态并放弃对CPU的占用
  *                 *state = 0;
  *                 break;
  *             }
  *          }
  *       10.任务高级应用: 宏定义方式.
  *          同样以LED跑马灯为例, 形式如下:
  *          ezos_task_t task_led(ezos_state_t *state, ezos_para_t *para)
  *          {
  *             EZOS_BEGIN(*state);
  *             while(1)
  *             {
  *                 led_toggle(LED1);           // LED1翻转
  *                 EZOS_DELAY(1000);           // 需要延时1S
  *                 led_toggle(LED2);           // LED2翻转
  *                 EZOS_DELAY(1000);           // 需要延时1S
  *             }
  *             EZOS_END();
  *          }
  *       11.任务通讯的使用. 每个任务建议只在任务最开始使用一个IPC. 若要用多个IPC需要使用状态机方式.
  *          以信号量为例, 调用信号量释放函数后, 和此信号量相关的任务都会被置为就绪状态.
  *          ezos_sem_release(lock);
  *          对于信号量的获取, 正常情况会返回 EZOS_OK, EZOS_TIMEOUT, EZOS_WAIT, 对于EZOS_WAIT, 使用return返回即可.
  *          示例如下:
  *          ezos_task_t task_sem(ezos_state_t *state, ezos_para_t *para)
  *          {
  *              ezos_t val;
  *              val = ezos_sem_take(lock, 10);
  *              if (val == EZOS_WAIT) return;                                  // 直接返回等待
  *              if      (val == EZOS_OK)      {do_something();}                // 成功获取信号量
  *              else if (val == EZOS_TIMEOUT) {do_something();}                // 信号量超时处理
  *              else if (val == EZOS_ERROR)   {exception();}                   // 一般不应返回EZOS_ERROR, 异常处理
  *          }
  *       12.由于是协作式的RTOS, 临界代码问题较少而且容易预测. 任务使用状态机方式时需特别注意此问题.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EZOS_H
#define __EZOS_H

/* Include -------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32f10x.h"                                                          // __INLINE



/** @addtogroup OS
  * @{
  */
/** @addtogroup ezOS
  * @{
  */



/**
  ******************************************************************************
  * @defgroup ezOS_Configure
  * @brief    用户配置
  ******************************************************************************
  * @{
  */
#include "ezos_conf.h"                                                          // ezos 配置文件

#if ((__EZOS_TICK_MS == 0) || (__EZOS_TICK_MS > 1000))
    #error "__EZOS_TICK_MS error!"
#endif

#if ((__EZOS_TASK_NUM == 0) || (__EZOS_TASK_NUM > 250))
    #error "__EZOS_TASK_NUM error!"
#endif

#if ((defined __EZOS_SEM) || (defined __EZOS_MUTEX) || (defined __EZOS_EVENT) || (defined __EZOS_MAILBOX) || (defined __EZOS_MESSAGE))
    #define __EZOS_IPC
#endif
/**
  * @}
  */



/** @defgroup ezOS_Public_TypeDefine
* @brief    公有类型定义
* @{
*/
/// ezos枚举类型定义
typedef enum
{
    EZOS_OK      = 0,                                                           ///< 任务或信号量操作成功
    EZOS_ERROR   = 1,                                                           ///< 任务或信号量操作失败
    EZOS_FULL    = 2,                                                           ///< 任务或信号量溢出错误
    EZOS_EXSIT   = 3,                                                           ///< 任务或信号量名称已存在错误
    // 任务状态
    EZOS_RUNNING = 4,                                                           ///< 任务运行状态
    EZOS_READY   = 5,                                                           ///< 任务就绪状态
    EZOS_SUSPEND = 6,                                                           ///< 任务阻塞状态
    EZOS_DELETE  = 7,                                                           ///< 任务删除状态, 信号量删除状态
    // 信号量状态
    EZOS_WAIT    = 8,                                                           ///< 需要等待信号量
    EZOS_TIMEOUT = 9,                                                           ///< 信号量超时
    // 信号量类型
    EZOS_SEM     = 10,                                                          ///< 信号量类型
    EZOS_MUTEX   = 11,                                                          ///< 互斥量类型
    EZOS_EVENT   = 12,                                                          ///< 事件类型
    EZOS_MAILBOX = 13,                                                          ///< 邮箱类型
    EZOS_MESSAGE = 14,                                                          ///< 消息队列类型
} ezos_t;

/// 任务类型定义. 用于标记任务函数
typedef void                ezos_task_t;

/// 状态类型定义, 指定参数state的类型.
typedef uint16_t            ezos_state_t;

/// 参数类型定义, 指定参数para的类型. 平台移植或为节省RAM, 可谨慎修改
typedef uint32_t            ezos_para_t;

/// 任务参数类型定义
struct __ezos_struct
{
    struct __ezos_struct *next;                                                 ///< 任务链表指针
    void *name;                                                                 ///< 任务名称
    ezos_task_t (*pfun)(ezos_state_t *state, ezos_para_t *para);                ///< 任务函数
#ifdef __EZOS_IPC
    void *ipc;                                                                  ///< 任务相关IPC
#endif
    int32_t      time;                                                          ///< 任务阻塞延时时间
    ezos_para_t  para;                                                          ///< 函数参数
    ezos_state_t state;                                                         ///< 函数状态
    ezos_t       status;                                                        ///< 任务状态
    uint8_t      prio;                                                          ///< 任务优先级
};
typedef struct __ezos_struct  ezos_struct_t;

/// 进程通信类型定义
struct __ezos_ipc
{
    void *next;                                                                 ///< 进程通信链表指针
    void *name;                                                                 ///< 进程通信名称
    ezos_t type;                                                                ///< 进程通信类型
};
typedef struct __ezos_ipc   ezos_ipc_t;

/// 信号量类型定义
typedef struct
{
    ezos_ipc_t ipc;                                                             ///< ipc共有属性
    uint8_t value;                                                              ///< 信号量值
} ezos_sem_t;

/// 互斥量类型定义
typedef struct
{
    ezos_ipc_t ipc;                                                             ///< ipc共有属性
    uint8_t value;                                                              ///< 互斥量值
} ezos_mutex_t;

/// 事件类型定义
typedef struct
{
    ezos_ipc_t ipc;                                                             ///< ipc共有属性
    uint8_t value;                                                              ///< 事件值
} ezos_event_t;

/// 邮箱类型定义
typedef struct
{
    ezos_ipc_t ipc;                                                             ///< ipc共有属性
    uint8_t *pool;                                                              ///< 邮箱内存池指针
    uint16_t mails_max;                                                         ///< 最大邮件数量
    uint16_t mails_num;                                                         ///< 当前邮件数量
    uint16_t in;                                                                ///< 邮件发送偏移量
    uint16_t out;                                                               ///< 邮件接收偏移量
} ezos_mb_t;

/// 消息队列类型定义
typedef struct
{
    ezos_ipc_t ipc;                                                             ///< ipc共有属性
    void *pool;                                                                 ///< 消息队列内存池指针
    uint16_t msg_size;                                                          ///< 单个消息的长度
    uint16_t msgs_max;                                                          ///< 最大消息数量
    uint16_t msgs_num;                                                          ///< 当前消息数量
    uint16_t in;                                                                ///< 消息发送偏移量
    uint16_t out;                                                               ///< 消息接收偏移量
} ezos_mq_t;
/**
  * @}
  */

/** @defgroup ezOS_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */
#define EZOS_NULL                       0

/// ezos 时间值, 表示永久等待
#define EZOS_FOREVER                    (int32_t)-1

/// ezos event 事件属性定义
#define EZOS_EVENT_OR                   0x01
#define EZOS_EVENT_AND                  0x02
#define EZOS_EVENT_KEEP                 0x04

/// ezos 快捷宏定义
#define EZOS_TASK(func)                 ezos_task_t func(ezos_state_t *state, ezos_para_t *para)// 任务函数宏定义
#define EZOS_NAME(name, id)             ((void *)((uint32_t)(name) + (uint32_t)(id)))           // 任务名称宏定义. 多任务使用同一函数时使用

/**
  * ezos 任务高级应用, 操作宏定义
  * 使用ezos宏定义操作, 不建议在该任务中使用switch语句!!! 若要使用必须注意:
  * switch 内不可使用EZOS宏定义操作, 如 EZOS_DELAY, EZOS_WAIT_XXX, EZOS_SUBFUN_XXX
  * 优点是阅读顺序清晰, 易于理解软件流程, 易于扩展子任务
  * 缺点是语法理解困难, 调试困难, 当有较复杂的分支情况时, 难以使用!
  * 综合而言, 不建议使用ezos宏定义操作, 宏定义过多后语意会变得晦涩难懂
  * 但以下两种情况可使用:
  * 1. 软件流程简单明了
  * 2. 单个任务代码量过大, 需拆分为子任务的形式
  * 子任务返回值类型必须为 ezos_t, 参数则无限制
  * 子任务定义形式为 ezos_t ezos_sub_function(...)
  * 子任务可嵌套使用, 需使用宏定义EZOS_SUBFUN_XXX
  */
#define LC_EZOS_RESUME(s)               switch(s) { case 0:
#define LC_EZOS_SET(s)                  s = __LINE__; case __LINE__:
#define LC_EZOS_SET_EXECUTE(s, e)       s = __LINE__; e; case __LINE__:
#define LC_EZOS_END(s)                  s = 0; }

/// EZOS 主任务高级应用, 宏定义操作
#define EZOS_BEGIN(s)                   {ezos_state_t *_s = &s; LC_EZOS_RESUME(*_s)
#define EZOS_END()                      LC_EZOS_END(*_s); return; }             // EZOS_BEGIN() 与 EZOS_END() 配对使用

#define EZOS_DELAY(time)                \
    do {                                \
        ezos_delay(time);               \
        LC_EZOS_SET_EXECUTE(*_s, return); \
    } while(0)                                                                  // EZOS 延时宏定义

#define EZOS_WAIT_SUBFUN(subfun)        \
    do {                                \
        LC_EZOS_SET(*_s);               \
        if ((subfun) == EZOS_WAIT) return; \
    } while(0)                                                                  // EZOS 子任务调用宏定义

#define EZOS_WAIT_UNTIL(cond)           \
    do {                                \
        LC_EZOS_SET(*_s);               \
        if (!(cond)) {                  \
            if (ezos_status_get_cur() == EZOS_RUNNING) ezos_delay(__EZOS_TICK_MS); \
            return; }                   \
    } while(0)                                                                  // EZOS 条件判断宏定义

#define EZOS_WAIT_WHILE(cond)           EZOS_WAIT_UNTIL(!(cond))                // EZOS 条件判断宏定义

/// EZOS 子任务高级应用, 宏定义操作
#define EZOS_SUBFUN_BEGIN(s)            {ezos_state_t *_s = &s; LC_EZOS_RESUME(*_s)
#define EZOS_SUBFUN_END()               LC_EZOS_END(*_s); return EZOS_OK; }

#define EZOS_SUBFUN_DELAY(time)         \
    do {                                \
        ezos_delay(time);               \
        LC_EZOS_SET_EXECUTE(*_s, return EZOS_WAIT); \
    } while(0)                                                                  // EZOS 延时宏定义

#define EZOS_SUBFUN_WAIT_SUBFUN(subfun) \
    do {                                \
        LC_EZOS_SET(*_s);               \
        if ((subfun) == EZOS_WAIT) return EZOS_WAIT; \
    } while(0)                                                                  // EZOS 子任务调用宏定义

#define EZOS_SUBFUN_WAIT_UNTIL(cond)    \
    do {                                \
        LC_EZOS_SET(*_s);               \
        if (!(cond)) {                  \
            if (ezos_status_get_cur() == EZOS_RUNNING) ezos_delay(__EZOS_TICK_MS); \
            return EZOS_WAIT; }         \
    } while(0)                                                                  // EZOS 条件判断宏定义

#define EZOS_SUBFUN_WAIT_WHILE(cond)    EZOS_SUBFUN_WAIT_UNTIL(!(cond))         // EZOS 条件判断宏定义
/**
  * @}
  */

/** @defgroup ezOS_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup ezOS_Public_Function
  * @brief    定义公有函数
  * @{
  */
void ezos_init(void);                                                           // ezOS初始化函数
void ezos_scan_tick(void);                                                      // ezOS 系统时钟定时扫描. 在定时中断中调用
void ezos_schedule(void);                                                       // 任务调度, main函数的最后调用
void ezos_idle_hook_set(void (*hook)(void));                                    // 设置空闲任务钩子函数, 只能是void fun(void)的简单函数
uint8_t  ezos_trash_num_min_get(void);                                          // 获取回收链表节点历史最小值. 需使能__EZOS_DEBUG
uint32_t ezos_idle_tick_max_get(void);                                          // 获取空闲任务运行间隔最大值. 需使能__EZOS_DEBUG

ezos_t ezos_add(void *name,                                                     // 添加任务, 任务名称不能为 EZOS_NULL
                ezos_task_t (*pfun)(ezos_state_t*, ezos_para_t*),               // 函数名称和参数指针
                ezos_para_t para,                                               // 函数内部参数
                int32_t time_ms,                                                // 任务初始状态. EZOS_FOREVER 永久阻塞, =0 就绪状态, >0 任务延时
                uint8_t priority);                                              // 优先级. 有效值0-254. 值越小优先级越高.
ezos_t ezos_delete(void *name);                                                 // 删除指定任务. 一般不使用, 而是让任务自删除
ezos_t ezos_delay(int32_t time_ms);                                             // 当前任务延时. EZOS_FOREVER 永久阻塞, =0 就绪状态, >0 任务延时
ezos_t ezos_status_get(void *name);                                             // 获取指定任务状态
ezos_t ezos_status_get_cur(void);                                               // 获取当前任务状态

// 快速添加一个任务, pfun-任务函数名, priority-任务优先级
__INLINE static ezos_t quick_add(ezos_task_t (*pfun)(ezos_state_t*, ezos_para_t*), uint8_t priority)
{
    return ezos_add((void *)pfun, pfun, 0, 0, priority);
}

// 强制添加一个任务, 针对已存在任务, 会删除后重新添加
__INLINE static ezos_t force_add(
                void *name,                                                     // 添加任务, 任务名称不能为 EZOS_NULL
                ezos_task_t (*pfun)(ezos_state_t*, ezos_para_t*),               // 函数名称和参数
                ezos_para_t para,                                               // 函数内部参数
                int32_t time_ms,                                                // 任务初始状态. EZOS_FOREVER 永久阻塞, =0 就绪状态, >0 任务延时.
                uint8_t priority)                                               // 优先级. 有效值0-254. 值越小优先级越高.
{
    ezos_delete(name);
    return ezos_add(name, pfun, para, time_ms, priority);
}

__INLINE static ezos_t ezos_suspend(void)                                       // 永久挂起当前任务
{
    return ezos_delay(EZOS_FOREVER);
}
ezos_t ezos_resume(void *name, ezos_para_t para);                               // 指定任务进入就绪态, 有信号量功能时, 不建议使用此函数



#ifdef __EZOS_SEM
ezos_t ezos_sem_add(ezos_sem_t *sem, uint8_t value);                            // 添加信号量
ezos_t ezos_sem_delete(ezos_sem_t *sem);                                        // 删除信号量
ezos_t ezos_sem_take(ezos_sem_t *sem, int32_t time_ms);                         // 获取信号量
ezos_t ezos_sem_release(ezos_sem_t *sem);                                       // 释放信号量
ezos_t ezos_sem_clear(ezos_sem_t *sem);                                         // 清空信号量
#endif

#ifdef __EZOS_MUTEX
ezos_t ezos_mutex_add(ezos_mutex_t *mutex, uint8_t value);                      // 添加互斥量
ezos_t ezos_mutex_delete(ezos_mutex_t *mutex);                                  // 删除互斥量
ezos_t ezos_mutex_take(ezos_mutex_t *mutex, int32_t time_ms);                   // 获取互斥量
ezos_t ezos_mutex_release(ezos_mutex_t *mutex);                                 // 释放互斥量
ezos_t ezos_mutex_clear(ezos_mutex_t *mutex);                                   // 清空互斥量
#endif

#ifdef __EZOS_EVENT
ezos_t ezos_event_add(ezos_event_t *event, uint8_t value);                      // 添加事件
ezos_t ezos_event_delete(ezos_event_t *event);                                  // 删除事件
ezos_t ezos_event_take(ezos_event_t *event, uint8_t value, uint8_t option, int32_t time_ms);// 获取一组事件
ezos_t ezos_event_release(ezos_event_t *event, uint8_t value);                  // 释放一个事件
ezos_t ezos_event_clear(ezos_event_t *event);                                   // 清空事件
#endif

#ifdef __EZOS_MAILBOX
ezos_t ezos_mb_add(ezos_mb_t *mailbox, uint8_t *pool, uint16_t pool_size);      // 添加邮箱
ezos_t ezos_mb_delete(ezos_mb_t *mailbox);                                      // 删除邮箱
ezos_t ezos_mb_send(ezos_mb_t *mailbox, uint8_t mail);                          // 发送邮件
ezos_t ezos_mb_receive(ezos_mb_t *mailbox, uint8_t *mail, int32_t time_ms);     // 接收邮件
ezos_t ezos_mb_clear(ezos_mb_t *mailbox);                                       // 清空邮箱
#endif

#ifdef __EZOS_MESSAGE
ezos_t ezos_mq_add(ezos_mq_t *message, uint16_t msg_size,                       // 添加消息队列
                   void *pool, uint16_t pool_size);
ezos_t ezos_mq_delete(ezos_mq_t *message);                                      // 删除消息队列
ezos_t ezos_mq_send(ezos_mq_t *message, void *msg);                             // 发送消息
ezos_t ezos_mq_receive(ezos_mq_t *message, void *msg, int32_t time_ms);         // 接收消息
ezos_t ezos_mq_clear(ezos_mq_t *message);                                       // 清空消息
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
