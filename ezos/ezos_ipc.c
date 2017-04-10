/**
  ******************************************************************************
  * @file    ezos_ipc.c
  * @author  mousie
  * @version V2.3.0
  * @date    2014.3.28
  * @brief   ezos的任务通讯(IPC)相关代码
  * -V2.0.0, mousie, 2012.1.1
  *          - ezos任务通讯(IPC)首版代码, 独立成一个文件
  * -V2.0.2, mousie, 2012.8.6
  *          - 修正函数局部变量赋值未进行中断保护, 导致任务链表错误的BUG
  * -V2.1.0, mousie, 2013.2.25
  *          - 增加 EZOS_TASK 任务宏定义.
  *          - 任务参数state的类型改为 ezos_s_t
  *          - 任务参数para 的类型改为 ezos_p_t
  * -V2.2.0, mousie, 2013.4.10
  *          - 增加 ezos_suspend(), ezos_resume() 函数, 可用于简单的任务通讯
  *          - 增加 ezos_printf 调试信息打印函数
  *          - 增加 ipc清空函数, ezos_xxx_clear()
  *          - 去除 ipc的名称, ipc指针即名称, ipc必须是唯一的
  *          - 增加 ezos 系统时钟的设置, 将 ezos_scan_1ms 改名为 ezos_scan_tick
  *          - 任务参数 ezos_s_t 由 uint8_t 改为 uint16_t, 以便支持宏定义方式
  *          - 增加 任务状态机宏定义方式, 任务阅读更直观, 书写更灵活
  * -V2.3.0, mousie, 2014.3.28
  *          - 任务参数 ezos_s_t 的类型改为 ezos_state_t
  *          - 任务参数 ezos_p_t 的类型改为 ezos_para_t
  *          - 增加任务返回值类型 ezos_task_t 用于说明函数类型
  *          - 修改子任务宏定义 XXX_SUB_XXX 为 XXX_SUBFUN_XXX
  ******************************************************************************
  */

/* Include -------------------------------------------------------------------*/
#include "ezos.h"



/** @addtogroup OS
  * @{
  */
/** @defgroup ezOS_IPC
  * @{
  */
#ifdef __EZOS_IPC



/**
  ******************************************************************************
  * @addtogroup ezOS_ipc_Configure
  ******************************************************************************
  * @{
  */
#include "stm32f10x.h"
#define ezos_enable_int(mask)           __set_PRIMASK(mask)                     ///< ezos宏定义全局中断打开函数, 方便代码移植
#define ezos_disable_int()              __get_PRIMASK(); __disable_irq()        ///< ezos宏定义全局中断关闭函数, 方便代码移植
/**
  * @}
  */



/** @defgroup ezOS_ipc_Private_TypeDefine
  * @brief    私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup ezOS_ipc_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup ezOS_ipc_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */
extern ezos_struct_t idle;                                                      ///< 任务链表起始节点
extern ezos_ipc_t    ipc;                                                       ///< IPC链表起始节点
extern ezos_struct_t *run;                                                      ///< 指向正在运行的任务
/**
  * @}
  */

/** @defgroup ezOS_ipc_Private_Function
  * @brief    定义私有函数
  * @{
  */
static ezos_t ezos_ipc_add(void *name, ezos_t type);
static ezos_t ezos_ipc_delete(void *name);
/**
  * @}
  */



/** @defgroup ezOS_ipc_Function
  * @brief    函数原文件
  * @{
  */
/**
  ******************************************************************************
  * @brief  添加内容到IPC循环链表中
  * @param  name,       IPC名称, 同时也是IPC指针. 不能为 EZOS_NULL
  * @param  type,       IPC类型
  * @retval 返回值
  *   @arg  EZOS_OK,    添加IPC成功
  *   @arg  EZOS_EXSIT, 添加IPC失败, IPC已存在
  ******************************************************************************
  */
ezos_t ezos_ipc_add(void *name, ezos_t type)
{
    ezos_ipc_t *pre    = &ipc;
    ezos_ipc_t *search = ipc.next;
    ezos_ipc_t *node   = name;

    while(search->name != EZOS_NULL)
    {
        if (search->name == name) return EZOS_EXSIT;                            ///< IPC已存在
        pre    = search;
        search = search->next;
    }

    node->next = pre->next;                                                     ///< 加入到IPC链表
    pre->next  = node;
    node->name = name;
    node->type = type;

    return EZOS_OK;
}

/**
  ******************************************************************************
  * @brief  从IPC链表中删除内容
  * @param  name,       IPC名称, 不能为 EZOS_NULL
  * @retval 返回值
  *   @arg  EZOS_OK,    删除IPC成功
  *   @arg  EZOS_ERROR, 删除IPC失败
  ******************************************************************************
  */
ezos_t ezos_ipc_delete(void *name)
{
    ezos_ipc_t *pre    = &ipc;
    ezos_ipc_t *search = ipc.next;

    while(search->name != EZOS_NULL)
    {
        if ((search->name == name))                                             // 找到指定IPC
        {
            pre->next    = search->next;                                        // 从IPC链表中删除
            search->next = EZOS_NULL;
            search->name = EZOS_NULL;
            search->type = EZOS_DELETE;
            return EZOS_OK;
        }
        pre    = search;
        search = search->next;
    }

    return EZOS_ERROR;
}

#ifdef __EZOS_SEM
/**
  ******************************************************************************
  * @brief  添加信号量到IPC循环链表中, 可以在中断中使用
  * @param  sem,        信号量指针. 不能为 EZOS_NULL
  * @param  value,      信号量初始值
  * @retval 返回值
  *   @arg  EZOS_OK,    添加信号量成功
  *   @arg  EZOS_ERROR, 添加信号量失败
  *   @arg  EZOS_EXSIT, 添加信号量失败, 名称已存在
  ******************************************************************************
  */
ezos_t ezos_sem_add(ezos_sem_t *sem, uint8_t value)
{
    ezos_t rtn = EZOS_ERROR;
    uint32_t mask;

    if (sem == EZOS_NULL) return rtn;
    mask = ezos_disable_int();

    rtn = ezos_ipc_add(sem, EZOS_SEM);
    if (rtn == EZOS_OK) sem->value = value;

    ezos_enable_int(mask);
    return rtn;
}

/**
  ******************************************************************************
  * @brief  从IPC链表中删除信号量, 可以在中断中使用
  * @param  sem,        信号量指针. 不能为 EZOS_NULL
  * @retval 返回值
  *   @arg  EZOS_OK,    删除信号量成功
  *   @arg  EZOS_ERROR, 删除信号量失败
  ******************************************************************************
  */
ezos_t ezos_sem_delete(ezos_sem_t *sem)
{
    ezos_t rtn = EZOS_ERROR;
    uint32_t mask;

    if (sem == EZOS_NULL) return rtn;
    mask = ezos_disable_int();

    rtn = ezos_ipc_delete(sem);

    ezos_enable_int(mask);
    return rtn;
}

/**
  ******************************************************************************
  * @brief  获取信号量. 只可以在任务中使用
  * @param  sem,        信号量指针. 不能为 EZOS_NULL
  * @param  time_ms,    信号量等待时间, 单位 ms
  *   @arg  <0,         永久等待
  *   @arg  0,          不等待, 直接返回信号量状态
  *   @arg  >0,         等待信号量一段时间
  * @retval 返回值
  *   @arg  EZOS_OK,      获取信号量成功
  *   @arg  EZOS_WAIT,    需要等待信号量, 返回此值后, 原函数需要return
  *   @arg  EZOS_TIMEOUT, 信号量等待超时
  *   @arg  EZOS_ERROR,   指定信号量有错误, 说明代码有误
  ******************************************************************************
  */
ezos_t ezos_sem_take(ezos_sem_t *sem, int32_t time_ms)
{
    uint32_t mask;
    ezos_t rtn = EZOS_ERROR;
    ezos_sem_t *search;

    if ((sem == EZOS_NULL) || (run == EZOS_NULL)) return rtn;
    mask = ezos_disable_int();

    search = ipc.next;
    while(search->ipc.name != EZOS_NULL)
    {
        if (search->ipc.name == sem)                                            // 找到信号量
        {
            if (search->ipc.type != EZOS_SEM)                                   // 信号量类型错误
            {
                break;
            }
            if (search->value > 0)                                              // 成功获取信号量
            {
                --search->value;
                run->ipc  = EZOS_NULL;
                run->time = 0;
                rtn = EZOS_OK;
                break;
            }
            else if (run->ipc != search)                                        // 信号量等待处理
            {
                if (time_ms == 0)
                {
                    run->ipc  = EZOS_NULL;
                    run->time = 0;
                    rtn = EZOS_TIMEOUT;                                         // 不等待, 直接返回信号量超时
                }
                else
                {
                    run->ipc = search;
                    ezos_delay(time_ms);                                        // 任务挂起, 等待信号量释放或超时
                    rtn = EZOS_WAIT;
                }
                break;
            }
            else                                                                // 信号量超时处理
            {
                if (run->time == 0)
                {
                    run->ipc = EZOS_NULL;                                       // 信号量超时
                    rtn = EZOS_TIMEOUT;
                }
                else
                {
                    run->status = EZOS_SUSPEND;                                 // 未获得信号量, 继续挂起等待
                    rtn = EZOS_WAIT;
                }
                break;
            }
        }
        search = search->ipc.next;
    }

    ezos_enable_int(mask);
    return rtn;
}

/**
  ******************************************************************************
  * @brief  释放信号量. 可以在中断中使用
  * @param  sem,        信号量指针, 不能为 EZOS_NULL
  * @retval 返回值
  *   @arg  EZOS_OK,    释放信号量成功
  *   @arg  EZOS_ERROR, 释放信号量失败
  ******************************************************************************
  */
ezos_t ezos_sem_release(ezos_sem_t *sem)
{
    uint32_t mask;
    ezos_t rtn = EZOS_ERROR;
    ezos_struct_t *task;
    ezos_sem_t    *search;

    if (sem == EZOS_NULL) return rtn;
    mask = ezos_disable_int();

    task   = idle.next;
    search = ipc.next;
    while(search->ipc.name != EZOS_NULL)
    {
        if (search->ipc.name == sem)                                            // 找到指定信号量
        {
            if (search->ipc.type != EZOS_SEM) break;                            // 信号量类型错误
            while (task->name != EZOS_NULL)
            {
                if (task->ipc == search) task->status = EZOS_READY;             // 信号量相关任务进入就绪状态
                task = task->next;
            }
            if (search->value < 0xFF) ++search->value;
            rtn = EZOS_OK;
            break;
        }
        search = search->ipc.next;
    }

    ezos_enable_int(mask);
    return rtn;
}

/**
  ******************************************************************************
  * @brief  清空信号量. 可以在中断中使用
  * @param  sem,        信号量指针, 不能为 EZOS_NULL
  * @retval 返回值
  *   @arg  EZOS_OK,    清空信号量成功
  *   @arg  EZOS_ERROR, 清空信号量失败
  ******************************************************************************
  */
ezos_t ezos_sem_clear(ezos_sem_t *sem)
{
    uint32_t mask;
    ezos_t rtn = EZOS_ERROR;
    ezos_sem_t  *search;

    if (sem == EZOS_NULL) return rtn;
    mask = ezos_disable_int();

    search = ipc.next;
    while(search->ipc.name != EZOS_NULL)
    {
        if (search->ipc.name == sem)                                            // 找到指定信号量
        {
            if (search->ipc.type != EZOS_SEM) break;                            // 信号量类型错误
            search->value = 0;
            rtn = EZOS_OK;
            break;
        }
        search = search->ipc.next;
    }

    ezos_enable_int(mask);
    return rtn;
}
#endif

#ifdef __EZOS_MUTEX
/**
  ******************************************************************************
  * @brief  添加互斥量到IPC循环链表中, 可以在中断中使用
  * @param  mutex,      互斥量指针, 不能为 EZOS_NULL
  * @param  value,      互斥量初始值
  * @retval 返回值
  *   @arg  EZOS_OK,    添加互斥量成功
  *   @arg  EZOS_ERROR, 添加互斥量失败
  *   @arg  EZOS_EXSIT, 添加互斥量失败, 名称已存在
  ******************************************************************************
  */
ezos_t ezos_mutex_add(ezos_mutex_t *mutex, uint8_t value)
{
    ezos_t rtn = EZOS_ERROR;
    uint32_t mask;

    if (mutex == EZOS_NULL) return rtn;
    mask = ezos_disable_int();

    rtn = ezos_ipc_add(mutex, EZOS_MUTEX);
    if (rtn == EZOS_OK) mutex->value = value;

    ezos_enable_int(mask);
    return rtn;
}

/**
  ******************************************************************************
  * @brief  从IPC链表中删除互斥量, 可以在中断中使用
  * @param  mutex,      互斥量指针, 不能为 EZOS_NULL
  * @retval 返回值
  *   @arg  EZOS_OK,    删除互斥量成功
  *   @arg  EZOS_ERROR, 删除互斥量失败
  ******************************************************************************
  */
ezos_t ezos_mutex_delete(ezos_mutex_t *mutex)
{
    ezos_t rtn = EZOS_ERROR;
    uint32_t mask;

    if (mutex == EZOS_NULL) return rtn;
    mask = ezos_disable_int();

    rtn = ezos_ipc_delete(mutex);

    ezos_enable_int(mask);
    return rtn;
}

/**
  ******************************************************************************
  * @brief  获取互斥量. 只可以在任务中使用
  * @param  mutex,      互斥量指针, 不能为 EZOS_NULL
  * @param  time_ms,    互斥量等待时间, 单位 ms
  *   @arg  <0,         永久等待
  *   @arg  0,          不等待, 直接返回互斥量状态
  *   @arg  >0,         等待互斥量一段时间
  * @retval 返回值
  *   @arg  EZOS_OK,      获取互斥量成功
  *   @arg  EZOS_WAIT,    需要等待互斥量, 返回此值后, 原函数需要return
  *   @arg  EZOS_TIMEOUT, 互斥量等待超时
  *   @arg  EZOS_ERROR,   指定互斥量有错误, 说明代码有误
  ******************************************************************************
  */
ezos_t ezos_mutex_take(ezos_mutex_t *mutex, int32_t time_ms)
{
    uint32_t mask;
    ezos_t rtn = EZOS_ERROR;
    ezos_mutex_t *search;

    if ((mutex == EZOS_NULL) || (run == EZOS_NULL)) return rtn;
    mask = ezos_disable_int();

    search = ipc.next;
    while(search->ipc.name != EZOS_NULL)
    {
        if (search->ipc.name == mutex)                                          // 找到互斥量
        {
            if (search->ipc.type != EZOS_MUTEX)                                 // 互斥量类型错误
            {
                break;
            }
            if (search->value > 0)                                              // 成功获取互斥量
            {
                search->value = 0;
                run->ipc  = EZOS_NULL;
                run->time = 0;
                rtn = EZOS_OK;
                break;
            }
            else if (run->ipc != search)                                        // 互斥量等待处理
            {
                if (time_ms == 0)
                {
                    run->ipc  = EZOS_NULL;
                    run->time = 0;
                    rtn = EZOS_TIMEOUT;                                         // 不等待, 直接返回互斥量超时
                }
                else
                {
                    run->ipc = search;
                    ezos_delay(time_ms);                                        // 任务挂起, 等待互斥量释放或超时
                    rtn = EZOS_WAIT;
                }
                break;
            }
            else                                                                // 互斥量超时处理
            {
                if (run->time == 0)
                {
                    run->ipc = EZOS_NULL;                                       // 互斥量超时
                    rtn = EZOS_TIMEOUT;
                }
                else
                {
                    run->status = EZOS_SUSPEND;                                 // 未获得互斥量, 继续挂起等待
                    rtn = EZOS_WAIT;
                }
                break;
            }
        }
        search = search->ipc.next;
    }

    ezos_enable_int(mask);
    return rtn;
}

/**
  ******************************************************************************
  * @brief  释放互斥量. 可以在中断中使用
  * @param  mutex,      互斥量指针, 不能为 EZOS_NULL
  * @retval 返回值
  *   @arg  EZOS_OK,    释放互斥量成功
  *   @arg  EZOS_ERROR, 释放互斥量失败
  ******************************************************************************
  */
ezos_t ezos_mutex_release(ezos_mutex_t *mutex)
{
    uint32_t mask;
    ezos_t rtn = EZOS_ERROR;
    ezos_struct_t *task;
    ezos_mutex_t  *search;

    if (mutex == EZOS_NULL) return rtn;
    mask = ezos_disable_int();

    task   = idle.next;
    search = ipc.next;
    while(search->ipc.name != EZOS_NULL)
    {
        if (search->ipc.name == mutex)                                          // 找到指定互斥量
        {
            if (search->ipc.type != EZOS_MUTEX) break;                          // 互斥量类型错误
            while (task->name != EZOS_NULL)
            {
                if (task->ipc == search) task->status = EZOS_READY;             // 互斥量相关任务进入就绪状态
                task = task->next;
            }
            search->value = 1;
            rtn = EZOS_OK;
            break;
        }
        search = search->ipc.next;
    }

    ezos_enable_int(mask);
    return rtn;
}

/**
  ******************************************************************************
  * @brief  清空互斥量. 可以在中断中使用
  * @param  mutex,      互斥量指针, 不能为 EZOS_NULL
  * @retval 返回值
  *   @arg  EZOS_OK,    清空互斥量成功
  *   @arg  EZOS_ERROR, 清空互斥量失败
  ******************************************************************************
  */
ezos_t ezos_mutex_clear(ezos_mutex_t *mutex)
{
    uint32_t mask;
    ezos_t rtn = EZOS_ERROR;
    ezos_mutex_t *search;

    if (mutex == EZOS_NULL) return rtn;
    mask = ezos_disable_int();

    search = ipc.next;
    while(search->ipc.name != EZOS_NULL)
    {
        if (search->ipc.name == mutex)                                          // 找到指定互斥量
        {
            if (search->ipc.type != EZOS_MUTEX) break;                          // 互斥量类型错误
            search->value = 0;
            rtn = EZOS_OK;
            break;
        }
        search = search->ipc.next;
    }

    ezos_enable_int(mask);
    return rtn;
}
#endif

#ifdef __EZOS_EVENT
/**
  ******************************************************************************
  * @brief  添加事件到IPC循环链表中, 可以在中断中使用
  * @param  event,      事件指针, 不能为 EZOS_NULL
  * @param  value,      事件初始值
  * @retval 返回值
  *   @arg  EZOS_OK,    添加事件成功
  *   @arg  EZOS_ERROR, 添加事件失败
  *   @arg  EZOS_EXSIT, 添加事件失败, 名称已存在
  ******************************************************************************
  */
ezos_t ezos_event_add(ezos_event_t *event, uint8_t value)
{
    ezos_t rtn = EZOS_ERROR;
    uint32_t mask;

    if (event == EZOS_NULL) return rtn;
    mask = ezos_disable_int();

    rtn = ezos_ipc_add(event, EZOS_EVENT);
    if (rtn == EZOS_OK) event->value = value;

    ezos_enable_int(mask);
    return rtn;
}

/**
  ******************************************************************************
  * @brief  从IPC链表中删除事件, 可以在中断中使用
  * @param  event,      事件指针, 不能为 EZOS_NULL
  * @retval 返回值
  *   @arg  EZOS_OK,    删除事件成功
  *   @arg  EZOS_ERROR, 删除事件失败
  ******************************************************************************
  */
ezos_t ezos_event_delete(ezos_event_t *event)
{
    ezos_t rtn = EZOS_ERROR;
    uint32_t mask;

    if (event == EZOS_NULL) return rtn;
    mask = ezos_disable_int();

    rtn = ezos_ipc_delete(event);

    ezos_enable_int(mask);
    return rtn;
}

/**
  ******************************************************************************
  * @brief  获取一组事件. 只可以在任务中使用
  * @param  event,      事件指针, 不能为 EZOS_NULL
  * @param  value,      事件值
  * @param  option,     事件属性
  *   @arg  EVENT_OR,   事件或
  *   @arg  EVENT_AND,  事件和
  *   @arg  EVENT_KEEP, 获取事件后, 保留事件值
  * @param  time_ms,    事件等待时间, 单位 ms
  *   @arg  <0,         永久等待
  *   @arg  0,          不等待, 直接返回事件状态
  *   @arg  >0,         等待事件一段时间
  * @retval 返回值
  *   @arg  EZOS_OK,      获取事件成功
  *   @arg  EZOS_WAIT,    需要等待事件, 返回此值后, 原函数需要return
  *   @arg  EZOS_TIMEOUT, 事件等待超时
  *   @arg  EZOS_ERROR,   指定事件有错误, 说明代码有误
  ******************************************************************************
  */
ezos_t ezos_event_take(ezos_event_t *event, uint8_t value, uint8_t option, int32_t time_ms)
{
    uint32_t mask;
    ezos_t rtn = EZOS_ERROR;
    ezos_event_t *search;

    if ((event == EZOS_NULL) || (run == EZOS_NULL)) return rtn;
    mask = ezos_disable_int();

    search = ipc.next;
    while(search->ipc.name != EZOS_NULL)
    {
        if (search->ipc.name == event)                                          // 找到事件
        {
            if (search->ipc.type != EZOS_EVENT) break;                          // 事件类型错误

            if (option & EZOS_EVENT_OR)                                         // 事件或
            {
                if (search->value | value) rtn = EZOS_OK;
            }
            else if (option & EZOS_EVENT_AND)                                   // 事件和
            {
                if ((search->value & value) == value) rtn = EZOS_OK;
            }

            if (rtn == EZOS_OK)                                                 // 成功获取事件
            {
                if ((option & EZOS_EVENT_KEEP) == 0) search->value &= ~value;
                run->ipc  = EZOS_NULL;
                run->time = 0;
                break;
            }
            else if (run->ipc != search)                                        // 事件等待处理
            {
                if (time_ms == 0)
                {
                    run->ipc  = EZOS_NULL;
                    run->time = 0;
                    rtn = EZOS_TIMEOUT;                                         // 不等待, 直接返回事件超时
                }
                else
                {
                    run->ipc = search;
                    ezos_delay(time_ms);                                        // 任务挂起, 等待事件释放或超时
                    rtn = EZOS_WAIT;
                }
                break;
            }
            else                                                                // 事件超时处理
            {
                if (run->time == 0)
                {
                    run->ipc = EZOS_NULL;                                       // 事件超时
                    rtn = EZOS_TIMEOUT;
                }
                else
                {
                    run->status = EZOS_SUSPEND;                                 // 未获得事件, 继续挂起等待
                    rtn = EZOS_WAIT;
                }
                break;
            }
        }
        search = search->ipc.next;
    }

    ezos_enable_int(mask);
    return rtn;
}

/**
  ******************************************************************************
  * @brief  释放一个事件. 可以在中断中使用
  * @param  event,      事件指针, 不能为 EZOS_NULL
  * @param  value,      事件值
  * @retval 返回值
  *   @arg  EZOS_OK,    释放事件成功
  *   @arg  EZOS_ERROR, 释放事件失败
  ******************************************************************************
  */
ezos_t ezos_event_release(ezos_event_t *event, uint8_t value)
{
    uint32_t mask;
    ezos_t rtn = EZOS_ERROR;
    ezos_struct_t *task;
    ezos_event_t  *search;

    if (event == EZOS_NULL) return rtn;
    mask = ezos_disable_int();

    task   = idle.next;
    search = ipc.next;
    while(search->ipc.name != EZOS_NULL)
    {
        if (search->ipc.name == event)                                          // 找到指定事件
        {
            if (search->ipc.type != EZOS_EVENT) break;                          // 事件类型错误
            while (task->name != EZOS_NULL)
            {
                if (task->ipc == search) task->status = EZOS_READY;             // 事件相关任务进入就绪状态
                task = task->next;
            }
            search->value |= value;
            rtn = EZOS_OK;
            break;
        }
        search = search->ipc.next;
    }

    ezos_enable_int(mask);
    return rtn;
}

/**
  ******************************************************************************
  * @brief  清空事件. 可以在中断中使用
  * @param  event,      事件指针, 不能为 EZOS_NULL
  * @retval 返回值
  *   @arg  EZOS_OK,    清空事件成功
  *   @arg  EZOS_ERROR, 清空事件失败
  ******************************************************************************
  */
ezos_t ezos_event_reset(ezos_event_t *event, uint8_t value)
{
    uint32_t mask;
    ezos_t rtn = EZOS_ERROR;
    ezos_event_t *search;

    if (event == EZOS_NULL) return rtn;
    mask = ezos_disable_int();

    search = ipc.next;
    while(search->ipc.name != EZOS_NULL)
    {
        if (search->ipc.name == event)                                          // 找到指定事件
        {
            if (search->ipc.type != EZOS_EVENT) break;                          // 事件类型错误
            search->value = 0;
            rtn = EZOS_OK;
            break;
        }
        search = search->ipc.next;
    }

    ezos_enable_int(mask);
    return rtn;
}
#endif

#ifdef __EZOS_MAILBOX
/**
  ******************************************************************************
  * @brief  添加邮箱到IPC循环链表中, 可以在中断中使用
  * @param  mailbox,    邮箱指针, 不能为 EZOS_NULL
  * @param  pool,       邮箱缓存池指针
  * @param  pool_size,  邮箱缓存池大小
  * @retval 返回值
  *   @arg  EZOS_OK,    添加邮箱成功
  *   @arg  EZOS_ERROR, 添加邮箱失败
  *   @arg  EZOS_EXSIT, 添加邮箱失败, 名称已存在
  ******************************************************************************
  */
ezos_t ezos_mb_add(ezos_mb_t *mailbox, uint8_t *pool, uint16_t pool_size)
{
    ezos_t rtn = EZOS_ERROR;
    uint32_t mask;

    if (mailbox == EZOS_NULL) return rtn;
    mask = ezos_disable_int();

    rtn = ezos_ipc_add(mailbox, EZOS_MAILBOX);
    if (rtn == EZOS_OK)
    {
        mailbox->pool      = pool;
        mailbox->mails_max = pool_size;
        mailbox->mails_num = 0;
        mailbox->in        = 0;
        mailbox->out       = 0;
    }

    ezos_enable_int(mask);
    return rtn;
}

/**
  ******************************************************************************
  * @brief  从IPC链表中删除邮箱, 可以在中断中使用
  * @param  mailbox,    邮箱指针, 不能为 EZOS_NULL
  * @retval 返回值
  *   @arg  EZOS_OK,    删除邮箱成功
  *   @arg  EZOS_ERROR, 删除邮箱失败
  ******************************************************************************
  */
ezos_t ezos_mb_delete(ezos_mb_t *mailbox)
{
    ezos_t rtn = EZOS_ERROR;
    uint32_t mask;

    if (mailbox == EZOS_NULL) return rtn;
    mask = ezos_disable_int();

    rtn = ezos_ipc_delete(mailbox);

    ezos_enable_int(mask);
    return rtn;
}

/**
  ******************************************************************************
  * @brief  发出一封邮件. 可以在中断中使用
  * @param  mailbox,    邮箱指针, 不能为 EZOS_NULL
  * @param  mail,       邮件值
  * @retval 返回值
  *   @arg  EZOS_OK,    发送邮件成功
  *   @arg  EZOS_ERROR, 发送邮件失败
  *   @arg  EZOS_FULL,  发送邮件失败, 邮箱已满
  ******************************************************************************
  */
ezos_t ezos_mb_send(ezos_mb_t *mailbox, uint8_t mail)
{
    uint32_t mask;
    ezos_t rtn = EZOS_ERROR;
    ezos_struct_t *task;
    ezos_mb_t     *search;

    if (mailbox == EZOS_NULL) return rtn;
    mask = ezos_disable_int();

    task   = idle.next;
    search = ipc.next;
    while(search->ipc.name != EZOS_NULL)
    {
        if (search->ipc.name == mailbox)                                        // 找到指定邮箱
        {
            if (search->ipc.type != EZOS_MAILBOX) break;                        // 邮箱类型错误
            while (task->name != EZOS_NULL)
            {
                if (task->ipc == search) task->status = EZOS_READY;             // 邮箱相关任务进入就绪状态
                task = task->next;
            }
            if (search->mails_num < search->mails_max)
            {
                search->pool[search->in] = mail;                                // 将邮件加入邮箱中
                ++search->in;
                if (search->in >= search->mails_max) search->in = 0;
                ++search->mails_num;
                rtn = EZOS_OK;
            }
            else rtn = EZOS_FULL;                                               // 邮箱已满, 发送失败
            break;
        }
        search = search->ipc.next;
    }

    ezos_enable_int(mask);
    return rtn;
}

/**
  ******************************************************************************
  * @brief  接收一封邮件. 只可以在任务中使用
  * @param  mailbox,    邮箱指针, 不能为 EZOS_NULL
  * @param  mail,       存放邮件值的指针
  * @param  time_ms,    邮件等待时间, 单位 ms
  *   @arg  <0,         永久等待
  *   @arg  0,          不等待, 直接返回邮箱状态
  *   @arg  >0,         等待邮件一段时间
  * @retval 返回值
  *   @arg  EZOS_OK,      获取邮件成功
  *   @arg  EZOS_WAIT,    需要等待邮件, 返回此值后, 原函数需要return
  *   @arg  EZOS_TIMEOUT, 邮件等待超时
  *   @arg  EZOS_ERROR,   指定邮箱有错误, 说明代码有误
  ******************************************************************************
  */
ezos_t ezos_mb_receive(ezos_mb_t *mailbox, uint8_t *mail, int32_t time_ms)
{
    uint32_t mask;
    ezos_t rtn = EZOS_ERROR;
    ezos_mb_t *search;

    if ((mailbox == EZOS_NULL) || (run == EZOS_NULL)) return rtn;
    mask = ezos_disable_int();

    search = ipc.next;
    while(search->ipc.name != EZOS_NULL)
    {
        if (search->ipc.name == mailbox)                                        // 找到邮箱
        {
            if (search->ipc.type != EZOS_MAILBOX) break;                        // 邮箱类型错误

            if (search->mails_num > 0)
            {
                *mail = search->pool[search->out];                              // 取出邮件
                ++search->out;
                if (search->out >= search->mails_max) search->out = 0;
                --search->mails_num;
                run->ipc  = EZOS_NULL;
                run->time = 0;
                rtn = EZOS_OK;
            }
            else if (run->ipc != search)                                        // 邮箱等待处理
            {
                if (time_ms == 0)
                {
                    run->ipc  = EZOS_NULL;
                    run->time = 0;
                    rtn = EZOS_TIMEOUT;                                         // 不等待, 直接返回邮箱超时
                }
                else
                {
                    run->ipc = search;
                    ezos_delay(time_ms);                                        // 任务挂起
                    rtn = EZOS_WAIT;
                }
                break;
            }
            else                                                                // 邮箱超时处理
            {
                if (run->time == 0)
                {
                    run->ipc = EZOS_NULL;                                       // 邮箱超时
                    rtn = EZOS_TIMEOUT;
                }
                else
                {
                    run->status = EZOS_SUSPEND;                                 // 未获得邮件, 继续挂起等待
                    rtn = EZOS_WAIT;
                }
                break;
            }
        }
        search = search->ipc.next;
    }

    ezos_enable_int(mask);
    return rtn;
}

/**
  ******************************************************************************
  * @brief  清空邮件. 可以在中断中使用
  * @param  mailbox,    邮箱名称, 不能为 EZOS_NULL
  * @retval 返回值
  *   @arg  EZOS_OK,    清空邮件成功
  *   @arg  EZOS_ERROR, 清空邮件失败
  ******************************************************************************
  */
ezos_t ezos_mb_clear(ezos_mb_t *mailbox)
{
    uint32_t mask;
    ezos_t rtn = EZOS_ERROR;
    ezos_mb_t   *search;

    if (mailbox == EZOS_NULL) return rtn;
    mask = ezos_disable_int();

    search = ipc.next;
    while(search->ipc.name != EZOS_NULL)
    {
        if (search->ipc.name == mailbox)                                        // 找到指定邮箱
        {
            if (search->ipc.type != EZOS_MAILBOX) break;                        // 邮箱类型错误
            search->mails_num = 0;
            search->in        = 0;
            search->out       = 0;
            rtn = EZOS_OK;
            break;
        }
        search = search->ipc.next;
    }

    ezos_enable_int(mask);
    return rtn;
}
#endif

#ifdef __EZOS_MESSAGE
/**
  ******************************************************************************
  * @brief  添加消息队列到IPC循环链表中, 可以在中断中使用
  * @param  message,    消息队列指针, 不能为 EZOS_NULL
  * @param  msg_size,   消息长度
  * @param  pool,       消息队列缓存池指针
  * @param  pool_size,  消息队列缓存池大小
  * @retval 返回值
  *   @arg  EZOS_OK,    添加消息队列成功
  *   @arg  EZOS_ERROR, 添加消息队列失败
  *   @arg  EZOS_EXSIT, 添加消息队列失败, 名称已存在
  ******************************************************************************
  */
ezos_t ezos_mq_add(ezos_mq_t *message, uint16_t msg_size, void *pool, uint16_t pool_size)
{
    ezos_t rtn = EZOS_ERROR;
    uint32_t mask;

    if ((message == EZOS_NULL) || (msg_size == 0)) return rtn;
    mask = ezos_disable_int();

    rtn = ezos_ipc_add(message, EZOS_MESSAGE);
    if (rtn == EZOS_OK)
    {
        message->pool     = pool;
        message->msg_size = msg_size;
        message->msgs_max = pool_size;
        message->msgs_num = 0;
        message->in       = 0;
        message->out      = 0;
    }

    ezos_enable_int(mask);
    return rtn;
}

/**
  ******************************************************************************
  * @brief  从IPC链表中删除消息队列, 可以在中断中使用
  * @param  message,    消息队列指针, 不能为 EZOS_NULL
  * @retval 返回值
  *   @arg  EZOS_OK,    删除消息队列成功
  *   @arg  EZOS_ERROR, 删除消息队列失败
  ******************************************************************************
  */
ezos_t ezos_mq_delete(ezos_mq_t *message)
{
    ezos_t rtn = EZOS_ERROR;
    uint32_t mask;

    if (message == EZOS_NULL) return rtn;
    mask = ezos_disable_int();

    rtn = ezos_ipc_delete(message);

    ezos_enable_int(mask);
    return rtn;
}

/**
  ******************************************************************************
  * @brief  拷贝消息
  * @param  src,    源地址指针
  * @param  dst,    目标地址指针
  * @param  size,   字节数
  * @retval none
  ******************************************************************************
  */
static void message_copy(uint8_t *src, uint8_t *dst, uint16_t size)
{
    while (size--)
    {
        *dst++ = *src++;
    }
}

/**
  ******************************************************************************
  * @brief  发出一份消息, 大小为 msg_size. 可以在中断中使用
  * @param  message,    消息队列指针, 不能为 EZOS_NULL
  * @param  msg,        消息指针
  * @retval 返回值
  *   @arg  EZOS_OK,    发送消息成功
  *   @arg  EZOS_ERROR, 发送消息失败
  *   @arg  EZOS_FULL,  发送消息失败, 消息队列已满
  ******************************************************************************
  */
ezos_t ezos_mq_send(ezos_mq_t *message, void *msg)
{
    uint32_t mask;
    ezos_t rtn = EZOS_ERROR;
    ezos_struct_t *task;
    ezos_mq_t     *search;

    if (message == EZOS_NULL) return rtn;
    mask = ezos_disable_int();

    task   = idle.next;
    search = ipc.next;
    while(search->ipc.name != EZOS_NULL)
    {
        if (search->ipc.name == message)                                        // 找到指定消息队列
        {
            if (search->ipc.type != EZOS_MESSAGE) break;                        // 消息队列类型错误
            while (task->name != EZOS_NULL)
            {
                if (task->ipc == search) task->status = EZOS_READY;             // 消息队列相关任务进入就绪状态
                task = task->next;
            }
            if ((search->msgs_num + search->msg_size - 1) < search->msgs_max)
            {
                // 将消息加入到消息队列中
                message_copy(msg, (uint8_t *)search->pool + search->in, search->msg_size);
                search->in += search->msg_size;
                if (search->in >= search->msgs_max) search->in = 0;
                search->msgs_num += search->msg_size;
                rtn = EZOS_OK;
            }
            else rtn = EZOS_FULL;                                               // 消息队列已满, 发送失败
            break;
        }
        search = search->ipc.next;
    }

    ezos_enable_int(mask);
    return rtn;
}

/**
  ******************************************************************************
  * @brief  接收一份消息, 大小为 msg_size. 只可以在任务中使用
  * @param  message,    消息队列指针, 不能为 EZOS_NULL
  * @param  msg,        存放消息的指针
  * @param  time_ms,    消息等待时间, 单位 ms
  *   @arg  <0,         永久等待
  *   @arg  0,          不等待, 直接返回消息队列状态
  *   @arg  >0,         等待消息一段时间
  * @retval 返回值
  *   @arg  EZOS_OK,      获取消息成功
  *   @arg  EZOS_WAIT,    需要等待消息, 返回此值后, 原函数需要return
  *   @arg  EZOS_TIMEOUT, 消息等待超时
  *   @arg  EZOS_ERROR,   指定消息队列有错误, 说明代码有误
  ******************************************************************************
  */
ezos_t ezos_mq_receive(ezos_mq_t *message, void *msg, int32_t time_ms)
{
    uint32_t mask;
    ezos_t rtn = EZOS_ERROR;
    ezos_mq_t *search;

    if ((message == EZOS_NULL) || (run == EZOS_NULL)) return rtn;
    mask = ezos_disable_int();

    search = ipc.next;
    while(search->ipc.name != EZOS_NULL)
    {
        if (search->ipc.name == message)                                        // 找到消息队列
        {
            if (search->ipc.type != EZOS_MESSAGE) break;                        // 消息队列类型错误

            if (search->msgs_num > 0)
            {
                // 从消息队列中取出消息
                message_copy((uint8_t *)search->pool + search->out, msg, search->msg_size);
                search->out += search->msg_size;
                if (search->out >= search->msgs_max) search->out = 0;
                search->msgs_num -= search->msg_size;
                run->ipc  = EZOS_NULL;
                run->time = 0;
                rtn = EZOS_OK;
            }
            else if (run->ipc != search)                                        // 消息队列等待处理
            {
                if (time_ms == 0)
                {
                    run->ipc  = EZOS_NULL;
                    run->time = 0;
                    rtn = EZOS_TIMEOUT;                                         // 不等待, 直接返回消息队列超时
                }
                else
                {
                    run->ipc = search;
                    ezos_delay(time_ms);                                        // 任务挂起
                    rtn = EZOS_WAIT;
                }
                break;
            }
            else                                                                // 消息队列超时处理
            {
                if (run->time == 0)
                {
                    run->ipc = EZOS_NULL;                                       // 消息队列超时
                    rtn = EZOS_TIMEOUT;
                }
                else
                {
                    run->status = EZOS_SUSPEND;                                 // 未获得消息, 继续挂起等待
                    rtn = EZOS_WAIT;
                }
                break;
            }
        }
        search = search->ipc.next;
    }

    ezos_enable_int(mask);
    return rtn;
}

/**
  ******************************************************************************
  * @brief  清空消息. 可以在中断中使用
  * @param  message,    消息队列指针, 不能为 EZOS_NULL
  * @retval 返回值
  *   @arg  EZOS_OK,    清空消息成功
  *   @arg  EZOS_ERROR, 清空消息失败
  ******************************************************************************
  */
ezos_t ezos_mq_clear(ezos_mq_t *message)
{
    uint32_t mask;
    ezos_t rtn = EZOS_ERROR;
    ezos_mq_t   *search;

    if (message == EZOS_NULL) return rtn;
    mask = ezos_disable_int();

    search = ipc.next;
    while(search->ipc.name != EZOS_NULL)
    {
        if (search->ipc.name == message)                                        // 找到指定消息队列
        {
            if (search->ipc.type != EZOS_MESSAGE) break;                        // 消息队列类型错误
            search->msgs_num = 0;
            search->in       = 0;
            search->out      = 0;
            rtn = EZOS_OK;
            break;
        }
        search = search->ipc.next;
    }

    ezos_enable_int(mask);
    return rtn;
}
#endif
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
