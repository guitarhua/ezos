/**
  ******************************************************************************
  * @file    ezos_kernel.c
  * @author  mousie
  * @version V2.3.0
  * @date    2014.3.28
  * @brief   协作式RTOS
  *
  * -核心思想, mousie, 2011.12.31
  *      该协作式RTOS的核心思想是两组单向链表: 任务链表 和 回收链表.
  *  任务链表为单向循环链表, 链表起始节点存放着系统空闲任务.
  *  后面的节点按照任务优先级的先后顺序进行排序. 对于同优先级的任务,
  *  任务添加或任务改变自身状态时, 会将该任务节点放到同优先级链表节点的末尾.
  *  回收链表为单向非循环链表, 被删除的任务节点放到到回收链表中. 而被添加的任务节
  *  点从回收链表取出并分配到任务链表中.
  *      任务调度非常简单, 每次都是从头开始寻找处于就绪态的任务,
  *  运行完某个任务后就从头重新开始搜寻. 由于任务链表是按照优先级顺序排序的,
  *  这样就实现了高优先级任务的快速响应. 而同优先级任务则是轮询处理的.
  *      由于加入了任务的命名体系, 对于可重载的函数就可以添加多个任务了.
  *  而所有的任务操作依据就是任务的名称. 任务的名称是不可重复的.
  *  对于静态任务函数, 一般将其函数名命名为任务名称即可, 方便又好记.
  *  对于可重载函数的多个任务, 建议使用 函数名+0, 函数名+1 的方式来命名任务名称.
  *
  * -历史介绍, mousie, 2011.12.28
  *      ezos最早的思想起源于TCS德国的带有任务性质的超循环方式(前后台方式),
  *  为使用方便, 我将增减任务的方式直接和函数名相关, 加入延时功能.
  *  并写了核心代码 ezos_schedule(); ezos_add(); ezos_delete(); ezos_delay();
  *  这样一个外表酷似RTOS, 内核为超循环任务的OS就诞生了, 版本V1.0.0.
  *  由于使用起来非常方便, 代码为纯C语言, 简单易懂, 因此命名为 ezos, 即easy OS.
  *  后续由 TCS 胡工加入了任务参数, 静态任务, 可实例化任务的概念.
  *  整合这些概念后, 重写了部分代码, 将ezos的版本升级至较为实用的V1.3.1.
  *  在这个阶段, 使用ezos做了功能较为复杂的项目, 并发现一些任务通讯和同步的问题,
  *  在借鉴了抢占式RTOS的一些特性后, 决心重写ezos, 将其变为真正的协作式RTOS.
  *  同时要求代码依旧清晰简洁, 可适用于资源较少的8位单片机应用.
  *
  * -V2.0.0, mousie, 2011.12.28
  *          - 代码改为linux书写规范
  *          - 进化为协作式OS, 支持254级优先级
  *          - 使用链表, 重写所有系统函数
  *          - 增加信号量, 互斥量, 事件
  * -V2.0.1, mousie, 2012.5.30
  *          - 修正任务调度中断保护不足, 导致任务列表混乱的错误
  *          - 新增 ezos_delete_cur(), 可避免中断使能和禁止的嵌套问题
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
/** @defgroup ezOS_Kernel
  * @{
  */



/**
  ******************************************************************************
  * @addtogroup ezOS_Kernel_Configure
  ******************************************************************************
  * @{
  */
#include "stm32f10x.h"
#define ezos_enable_int(mask)           __set_PRIMASK(mask)                     ///< ezos宏定义全局中断打开函数, 方便代码移植
#define ezos_disable_int()              __get_PRIMASK(); __disable_irq()        ///< ezos宏定义全局中断关闭函数, 方便代码移植
/**
  * @}
  */



/** @defgroup ezOS_Kernel_Private_TypeDefine
  * @brief    私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup ezOS_Kernel_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup ezOS_Kernel_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */
ezos_struct_t task_link[__EZOS_TASK_NUM];                                       ///< ezos任务链表空间
ezos_struct_t idle;                                                             ///< 系统空闲任务, 任务链表起始节点
ezos_struct_t *trash;                                                           ///< 回收链表起始节点
ezos_struct_t *run;                                                             ///< 指向正在运行的任务
void (*idle_hook)(void);                                                        ///< 空闲任务钩子函数

#ifdef __EZOS_IPC
    ezos_ipc_t ipc;                                                           ///< IPC链表起始节点
#endif

#ifdef __EZOS_DEBUG
    static uint32_t idle_tick;                                                  ///< 系统空闲任务节拍, 固定为1ms
    static uint32_t idle_tick_max;                                              ///< 系统空闲任务节拍历史最大值
    static uint8_t  trash_num;                                                  ///< 回收链表节点数
    static uint8_t  trash_num_min;                                              ///< 回收链表节点数历史最小值
#endif
/**
  * @}
  */

/** @defgroup ezOS_Kernel_Private_Function
  * @brief    定义私有函数
  * @{
  */
static void ezos_delete_cur(void);                                              ///< 删除当前任务
static void ezos_task_idle(ezos_state_t *state, ezos_para_t *para);             ///< 系统空闲任务
/**
  * @}
  */



/** @defgroup ezOS_Kernel_Function
  * @brief    函数原文件
  * @{
  */

/**
  ******************************************************************************
  * @brief  ezos初始化函数
  * @param  none
  * @retval none
  ******************************************************************************
  */
void ezos_init(void)
{
    uint8_t i;
    uint32_t mask;

    mask = ezos_disable_int();

    idle.next = &idle;                                                          // 任务链表为循环链表
    idle.pfun = ezos_task_idle;                                                 // 空闲任务函数
    idle.name = EZOS_NULL;                                                      // 空闲任务名称为特定值 EZOS_NULL
#ifdef __EZOS_IPC
    idle.ipc  = EZOS_NULL;
#endif
    idle.time = 0;
    idle.status = EZOS_READY;                                                   // 空闲任务始终为就绪状态
    idle.prio = 0xFF;                                                           // 空闲任务的优先级最低, 为特定值0xFF
    idle_hook = EZOS_NULL;                                                      // 空闲任务钩子函数

#ifdef __EZOS_IPC
    ipc.next  = (void *)&ipc;                                                   // IPC循环链表
    ipc.name  = EZOS_NULL;                                                      // IPC起始节点名称为特定值0
    ipc.type  = EZOS_DELETE;                                                    // IPC起始节点为删除状态
#endif

    for (i=0; i < (__EZOS_TASK_NUM-1); i++)                                     // 回收链表为单向链表
    {
        task_link[i].next   = &task_link[i+1];
        task_link[i].name   = EZOS_NULL;
        task_link[i].status = EZOS_DELETE;
    }
    task_link[__EZOS_TASK_NUM-1].next = EZOS_NULL;                              // 回收链表结束节点
    trash = &task_link[0];                                                      // 回收链表起始节点
    run   = &idle;

#ifdef __EZOS_DEBUG
    idle_tick     = 0;
    idle_tick_max = 0;
    trash_num     = __EZOS_TASK_NUM;
    trash_num_min = __EZOS_TASK_NUM;
#endif

    ezos_enable_int(mask);
}

/**
  ******************************************************************************
  * @brief  ezOS 系统时钟定时扫描. 在定时中断中调用.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void ezos_scan_tick(void)
{
    uint32_t mask;
    ezos_struct_t *task;
    mask = ezos_disable_int();

    task = idle.next;
    while (task->name != EZOS_NULL)
    {
        if (task->time > 0)                                                     // 挂起延时状态
        {
            if (--task->time == 0) task->status = EZOS_READY;                   // 进入就绪态
        }
        task = task->next;
    }

#ifdef __EZOS_DEBUG
    ++idle_tick;
#endif

    ezos_enable_int(mask);
}

/**
  ******************************************************************************
  * @brief  任务调度函数, 放在main函数的最后.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void ezos_schedule(void)
{
    uint32_t mask;
    ezos_struct_t *task;

    mask = ezos_disable_int();
    while(1)
    {
        task = idle.next;
        while (task->status != EZOS_READY)                                      // 寻找就绪的任务
        {
            task = task->next;
        }
        run = task;
        run->status = EZOS_RUNNING;
        ezos_enable_int(mask);
        task->pfun(&task->state, &task->para);                                  // 运行任务函数
        mask = ezos_disable_int();
        if (run->status == EZOS_RUNNING) ezos_delete_cur();                     // 自动删除任务
        run = &idle;
    }
}

/**
  ******************************************************************************
  * @brief  添加任务, 任务状态初始化为0值. 可在中断中使用.
  * @param  name,       任务名称, 不能为 EZOS_NULL
  * @param  pfun,       任务函数名
  * @param  para,       函数参数
  * @param  time_ms,    延时时间, 单位 ms
  *   @arg  <0,         初始化为永久挂起状态
  *   @arg  0,          初始化为就绪状态
  *   @arg  >0,         初始化为挂起状态, 延时一段时间
  * @param  priority,   任务优先级. 有效值0-254, 值越小优先级越高
  * @retval 返回值
  *   @arg  EZOS_OK,    添加任务成功
  *   @arg  EZOS_ERROR, 添加任务失败
  *   @arg  EZOS_EXSIT, 添加任务失败, 任务已存在
  *   @arg  EZOS_FULL,  添加任务失败, 任务已满
  ******************************************************************************
  */
ezos_t ezos_add(void *name, void (*pfun)(ezos_state_t *, ezos_para_t *), ezos_para_t para, int32_t time_ms, uint8_t priority)
{
    uint32_t mask;
    ezos_struct_t *task, *pre, *search;

    if (name  == EZOS_NULL) return EZOS_ERROR;                                  // 任务名称无效, 报错返回
    if (trash == EZOS_NULL) return EZOS_FULL;                                   // 任务链表已满, 回收链表为空

    mask = ezos_disable_int();

    if (priority == 0xFF) priority = 0xFE;                                      // 0xFF优先级仅限空闲任务使用
    if (time_ms < 0)                                                            // 永久挂起赋值
    {
        time_ms = EZOS_FOREVER;
    }
#if (__EZOS_TICK_MS > 1)
    else                                                                        // 将ms的延时值转换为基础时钟的延时值
    {
        if (time_ms % __EZOS_TICK_MS) ++time_ms;
        time_ms /= __EZOS_TICK_MS;
    }
#endif

    // 检查任务是否存在
    task = idle.next;
    while (task->name != EZOS_NULL)
    {
        if (task->name == name)                                                 // 任务已存在
        {
            ezos_enable_int(mask);
            return EZOS_EXSIT;
        }
        task = task->next;
    }

    // 从回收链表中取出一个节点
    if (trash != run)                                                           // 回收链表节点正常状态
    {
        task  = trash;                                                          // 取出该回收链表节点
        trash = task->next;
    }
    else                                                                        // 如果中断中删除了正在运行的任务, 则需要保护该任务空间
    {
        task = trash->next;                                                     // 取下一个回收链表节点
        if (task == EZOS_NULL)                                                  // 如果节点无效, 报错返回
        {
            ezos_enable_int(mask);
            return EZOS_FULL;                                                   // 任务链表已满, 回收链表空间不足
        }
        trash->next = task->next;
    }

    // 寻找要加入的前后节点
    pre    = &idle;
    search = idle.next;
    while (search->prio <= priority)                                            // 加到同优先级任务链表的末尾
    {
        pre    = search;
        search = search->next;
    }
    task->next = search;                                                        // 加入任务节点
    pre->next  = task;

    // 给任务节点赋值
    task->name  = name;
    task->pfun  = pfun;
    task->state = 0;
    task->para  = para;
#ifdef __EZOS_IPC
    task->ipc   = EZOS_NULL;
#endif
    task->time  = time_ms;
    task->prio  = priority;
    if (time_ms == 0) task->status = EZOS_READY;
    else              task->status = EZOS_SUSPEND;

#ifdef __EZOS_DEBUG
    if (--trash_num < trash_num_min)
    {
        --trash_num_min;
        ezos_printf("ezos> trash_num_min %d\r\n", trash_num_min);
    }
#endif
    ezos_enable_int(mask);
    return EZOS_OK;                                                             // 添加任务成功
}

/**
  ******************************************************************************
  * @brief  删除当前任务. 仅供调度函数使用.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void ezos_delete_cur(void)
{
    ezos_struct_t *pre, *search;

    pre    = &idle;
    search = idle.next;
    while (search->name != EZOS_NULL)                                           // 寻找任务
    {
        if (search->name == run->name)                                          // 任务已找到
        {
            search->name   = EZOS_NULL;                                         // 删除任务
            search->status = EZOS_DELETE;                                       // 设为删除状态
            pre->next      = search->next;                                      // 从任务链表中删除任务节点
            search->next   = trash->next;                                       // 添加到删除链表
            trash->next    = search;
        #ifdef __EZOS_DEBUG
            ++trash_num;
        #endif
            break;                                                              // 删除当前任务成功
        }
        pre    = search;
        search = search->next;
    }
}

/**
  ******************************************************************************
  * @brief  删除任务. 可在中断中使用.
  *         一般不使用, 而是让任务自删除
  * @param  name,       任务名称, 不能为 EZOS_NULL
  * @retval 返回值
  *   @arg  EZOS_OK,    删除任务成功
  *   @arg  EZOS_ERROR, 删除任务失败
  ******************************************************************************
  */
ezos_t ezos_delete(void *name)
{
    ezos_t val;
    uint32_t mask;
    ezos_struct_t *pre, *search;

    mask = ezos_disable_int();

    val    = EZOS_ERROR;                                                        // 默认任务不存在, 可认为已删除
    pre    = &idle;
    search = idle.next;
    while (search->name != EZOS_NULL)                                           // 寻找任务
    {
        if (search->name == name)                                               // 任务已找到
        {
            search->name   = EZOS_NULL;                                         // 删除任务
            search->status = EZOS_DELETE;                                       // 设为删除状态
            pre->next      = search->next;                                      // 从任务链表中删除任务节点
            search->next   = trash->next;                                       // 添加到删除链表
            trash->next    = search;
        #ifdef __EZOS_DEBUG
            ++trash_num;
        #endif
            val = EZOS_OK;                                                      // 删除任务成功
            break;
        }
        pre    = search;
        search = search->next;
    }

    ezos_enable_int(mask);
    return val;
}

/**
  ******************************************************************************
  * @brief  当前任务延时. 只可以在任务中使用
  * @param  time_ms,    延时时间, 单位 ms
  *   @arg  <0,         永久挂起状态
  *   @arg  0,          就绪状态
  *   @arg  >0,         挂起状态, 延时一段时间
  * @retval 返回值
  *   @arg  EZOS_OK,    当前任务延时成功
  *   @arg  EZOS_ERROR, 当前任务延时失败
  ******************************************************************************
  */
ezos_t ezos_delay(int32_t time_ms)
{
    ezos_t val;
    uint32_t mask;
    ezos_struct_t *task, *pre, *search;

    mask = ezos_disable_int();

    if (time_ms < 0)
    {
        time_ms = EZOS_FOREVER;
    }
#if (__EZOS_TICK_MS > 1)
    else                                                                        // 将ms的延时值转换为基础时钟的延时值
    {
        if (time_ms % __EZOS_TICK_MS) ++time_ms;
        time_ms /= __EZOS_TICK_MS;
    }
#endif

    val  = EZOS_ERROR;
    task = idle.next;
    pre  = &idle;
    while (task->name != EZOS_NULL)                                             // 检查任务是否存在
    {
        if (task->name == run->name)                                            // 任务存在
        {
            task->time = time_ms;
            if (time_ms == 0) task->status = EZOS_READY;
            else              task->status = EZOS_SUSPEND;
            pre->next = task->next;                                             // 暂时脱离任务链表
            search    = task->next;
            while (search->prio <= task->prio)                                  // 转到同优先级任务链表的末尾
            {
                pre    = search;
                search = search->next;
            }
            pre->next  = task;                                                  // 重新加入任务链表
            task->next = search;
            val = EZOS_OK;
            break;
        }
        pre  = task;
        task = task->next;
    }

    ezos_enable_int(mask);
    return val;
}

/**
  ******************************************************************************
  * @brief  唤醒任务. 不建议使用此函数.
  *         一般建议使用信号量来进行任务间的通讯.
  *         当未提供信号量功能且通讯需求很简单时, 方可用此函数.
  * @param  name,       任务名称, 不能为 EZOS_NULL
  * @param  para,       函数参数
  * @retval 返回值
  *   @arg  EZOS_OK,    唤醒任务成功
  *   @arg  EZOS_ERROR, 唤醒任务失败
  *   @arg  EZOS_EXSIT, 唤醒任务失败, 任务正在运行或已处于就绪状态
  ******************************************************************************
  */
ezos_t ezos_resume(void *name, ezos_para_t para)
{
    ezos_t val;
    uint32_t mask;
    ezos_struct_t *search;

    mask = ezos_disable_int();

    val    = EZOS_ERROR;                                                        // 默认任务不存在, 可认为已删除
    search = idle.next;
    while (search->name != EZOS_NULL)                                           // 寻找任务
    {
        if (search->name == name)                                               // 任务已找到
        {
            if (search->status == EZOS_SUSPEND)                                 // 仅在阻塞状态时
            {
                search->status = EZOS_READY;                                    // 设为就绪状态
                search->para   = para;                                          // 重设任务参数
                val = EZOS_OK;                                                  // 删除任务成功
            }
            else
            {
                val = EZOS_EXSIT;
            }
            break;
        }
        search = search->next;
    }

    ezos_enable_int(mask);
    return val;
}

/**
  ******************************************************************************
  * @brief  获取任务状态. 可在中断中使用.
  * @param  name,        任务名称
  * @retval 返回值
  *   @arg  EZOS_ERROR,  获取任务状态失败
  *   @arg  EZOS_RUNNING,任务运行状态
  *   @arg  EZOS_READY,  任务就绪状态
  *   @arg  EZOS_SUSPEND,任务挂起状态
  *   @arg  EZOS_DELETE, 任务删除状态, 不存在该任务
  ******************************************************************************
  */
ezos_t ezos_status_get(void *name)
{
    ezos_t   status;
    uint32_t mask;
    ezos_struct_t *search;

    if (name == EZOS_NULL) return EZOS_ERROR;                                   // 不允许查询系统空闲任务
    mask = ezos_disable_int();

    search = idle.next;
    while (search->name != EZOS_NULL)                                           // 检查任务是否存在
    {
        if (search->name == name)                                               // 任务存在
        {
            status = search->status;
            ezos_enable_int(mask);
            return status;
        }
        search = search->next;
    }

    ezos_enable_int(mask);
    return EZOS_DELETE;                                                         // 不在任务链表中, 认为是删除的任务
}

/**
  ******************************************************************************
  * @brief  获取当前任务状态.
  * @param  none
  * @retval 返回值
  *   @arg  EZOS_ERROR,  获取任务状态失败
  *   @arg  EZOS_RUNNING,任务运行状态
  *   @arg  EZOS_READY,  任务就绪状态
  *   @arg  EZOS_SUSPEND,任务挂起状态
  *   @arg  EZOS_DELETE, 任务删除状态, 不存在该任务
  ******************************************************************************
  */
ezos_t ezos_status_get_cur(void)
{
    if (run && run->name) return run->status;
    else                  return EZOS_ERROR;
}

/**
  ******************************************************************************
  * @brief  获取回收链表节点的历史最小值
  * @param  none
  * @retval 返回值, 回收链表节点历史最小值
  ******************************************************************************
  */
uint8_t ezos_trash_num_min_get()
{
#ifdef __EZOS_DEBUG
    return trash_num_min;
#else
    return __EZOS_TASK_NUM;
#endif
}

/**
  ******************************************************************************
  * @brief  获取空闲任务运行间隔最大值
  * @param  none
  * @retval 返回值, 空闲任务运行间隔最大值
  ******************************************************************************
  */
uint32_t ezos_idle_tick_max_get()
{
#ifdef __EZOS_DEBUG
    return idle_tick_max;
#else
    return 0;
#endif
}

/**
  ******************************************************************************
  * @brief  设置空闲任务钩子函数
  * @param  none
  * @retval none
  ******************************************************************************
  */
void ezos_idle_hook_set(void (*hook)(void))
{
    idle_hook = hook;
}

/**
  ******************************************************************************
  * @brief  ezos空闲任务函数
  * @param  none
  * @retval none
  ******************************************************************************
  */
void ezos_task_idle(ezos_state_t *state, ezos_para_t *para)
{
#ifdef __EZOS_DEBUG
    static uint8_t first_run = 1;

    if (first_run)                                                              // 不计算空闲任务首次运行前的时间
    {
        first_run = 0;
        ezos_printf("ezos> idle task start %d\r\n", idle_tick);
        ezos_printf("ezos> idle_tick_max %d\r\n", idle_tick_max);
    }
    else if (idle_tick_max < idle_tick)
    {
        idle_tick_max = idle_tick;
        ezos_printf("ezos> idle_tick_max %d\r\n", idle_tick_max);
    }
    idle_tick = 0;
#endif

    if (idle_hook != EZOS_NULL) idle_hook();                                    // 调用空闲任务钩子函数
    idle.status = EZOS_READY;                                                   // 防止任务自动删除
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
