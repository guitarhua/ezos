/**
  ******************************************************************************
  * @file    ezos_kernel.c
  * @author  mousie
  * @version V2.3.0
  * @date    2014.3.28
  * @brief   Э��ʽRTOS
  *
  * -����˼��, mousie, 2011.12.31
  *      ��Э��ʽRTOS�ĺ���˼�������鵥������: �������� �� ��������.
  *  ��������Ϊ����ѭ������, ������ʼ�ڵ�����ϵͳ��������.
  *  ����Ľڵ㰴���������ȼ����Ⱥ�˳���������. ����ͬ���ȼ�������,
  *  ������ӻ�����ı�����״̬ʱ, �Ὣ������ڵ�ŵ�ͬ���ȼ�����ڵ��ĩβ.
  *  ��������Ϊ�����ѭ������, ��ɾ��������ڵ�ŵ�������������. ������ӵ������
  *  ��ӻ�������ȡ�������䵽����������.
  *      ������ȷǳ���, ÿ�ζ��Ǵ�ͷ��ʼѰ�Ҵ��ھ���̬������,
  *  ������ĳ�������ʹ�ͷ���¿�ʼ��Ѱ. �������������ǰ������ȼ�˳�������,
  *  ������ʵ���˸����ȼ�����Ŀ�����Ӧ. ��ͬ���ȼ�����������ѯ�����.
  *      ���ڼ����������������ϵ, ���ڿ����صĺ����Ϳ�����Ӷ��������.
  *  �����е�����������ݾ������������. ����������ǲ����ظ���.
  *  ���ھ�̬������, һ�㽫�亯��������Ϊ�������Ƽ���, �����ֺü�.
  *  ���ڿ����غ����Ķ������, ����ʹ�� ������+0, ������+1 �ķ�ʽ��������������.
  *
  * -��ʷ����, mousie, 2011.12.28
  *      ezos�����˼����Դ��TCS�¹��Ĵ����������ʵĳ�ѭ����ʽ(ǰ��̨��ʽ),
  *  Ϊʹ�÷���, �ҽ���������ķ�ʽֱ�Ӻͺ��������, ������ʱ����.
  *  ��д�˺��Ĵ��� ezos_schedule(); ezos_add(); ezos_delete(); ezos_delay();
  *  ����һ��������RTOS, �ں�Ϊ��ѭ�������OS�͵�����, �汾V1.0.0.
  *  ����ʹ�������ǳ�����, ����Ϊ��C����, ���׶�, �������Ϊ ezos, ��easy OS.
  *  ������ TCS �����������������, ��̬����, ��ʵ��������ĸ���.
  *  ������Щ�����, ��д�˲��ִ���, ��ezos�İ汾��������Ϊʵ�õ�V1.3.1.
  *  ������׶�, ʹ��ezos���˹��ܽ�Ϊ���ӵ���Ŀ, ������һЩ����ͨѶ��ͬ��������,
  *  �ڽ������ռʽRTOS��һЩ���Ժ�, ������дezos, �����Ϊ������Э��ʽRTOS.
  *  ͬʱҪ����������������, ����������Դ���ٵ�8λ��Ƭ��Ӧ��.
  *
  * -V2.0.0, mousie, 2011.12.28
  *          - �����Ϊlinux��д�淶
  *          - ����ΪЭ��ʽOS, ֧��254�����ȼ�
  *          - ʹ������, ��д����ϵͳ����
  *          - �����ź���, ������, �¼�
  * -V2.0.1, mousie, 2012.5.30
  *          - ������������жϱ�������, ���������б���ҵĴ���
  *          - ���� ezos_delete_cur(), �ɱ����ж�ʹ�ܺͽ�ֹ��Ƕ������
  * -V2.0.2, mousie, 2012.8.6
  *          - ���������ֲ�������ֵδ�����жϱ���, ����������������BUG
  * -V2.1.0, mousie, 2013.2.25
  *          - ���� EZOS_TASK ����궨��.
  *          - �������state�����͸�Ϊ ezos_s_t
  *          - �������para �����͸�Ϊ ezos_p_t
  * -V2.2.0, mousie, 2013.4.10
  *          - ���� ezos_suspend(), ezos_resume() ����, �����ڼ򵥵�����ͨѶ
  *          - ���� ezos_printf ������Ϣ��ӡ����
  *          - ���� ipc��պ���, ezos_xxx_clear()
  *          - ȥ�� ipc������, ipcָ�뼴����, ipc������Ψһ��
  *          - ���� ezos ϵͳʱ�ӵ�����, �� ezos_scan_1ms ����Ϊ ezos_scan_tick
  *          - ������� ezos_s_t �� uint8_t ��Ϊ uint16_t, �Ա�֧�ֺ궨�巽ʽ
  *          - ���� ����״̬���궨�巽ʽ, �����Ķ���ֱ��, ��д�����
  * -V2.3.0, mousie, 2014.3.28
  *          - ������� ezos_s_t �����͸�Ϊ ezos_state_t
  *          - ������� ezos_p_t �����͸�Ϊ ezos_para_t
  *          - �������񷵻�ֵ���� ezos_task_t ����˵����������
  *          - �޸�������궨�� XXX_SUB_XXX Ϊ XXX_SUBFUN_XXX
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
#define ezos_enable_int(mask)           __set_PRIMASK(mask)                     ///< ezos�궨��ȫ���жϴ򿪺���, ���������ֲ
#define ezos_disable_int()              __get_PRIMASK(); __disable_irq()        ///< ezos�궨��ȫ���жϹرպ���, ���������ֲ
/**
  * @}
  */



/** @defgroup ezOS_Kernel_Private_TypeDefine
  * @brief    ˽�����Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup ezOS_Kernel_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup ezOS_Kernel_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */
ezos_struct_t task_link[__EZOS_TASK_NUM];                                       ///< ezos��������ռ�
ezos_struct_t idle;                                                             ///< ϵͳ��������, ����������ʼ�ڵ�
ezos_struct_t *trash;                                                           ///< ����������ʼ�ڵ�
ezos_struct_t *run;                                                             ///< ָ���������е�����
void (*idle_hook)(void);                                                        ///< ���������Ӻ���

#ifdef __EZOS_IPC
    ezos_ipc_t ipc;                                                           ///< IPC������ʼ�ڵ�
#endif

#ifdef __EZOS_DEBUG
    static uint32_t idle_tick;                                                  ///< ϵͳ�����������, �̶�Ϊ1ms
    static uint32_t idle_tick_max;                                              ///< ϵͳ�������������ʷ���ֵ
    static uint8_t  trash_num;                                                  ///< ��������ڵ���
    static uint8_t  trash_num_min;                                              ///< ��������ڵ�����ʷ��Сֵ
#endif
/**
  * @}
  */

/** @defgroup ezOS_Kernel_Private_Function
  * @brief    ����˽�к���
  * @{
  */
static void ezos_delete_cur(void);                                              ///< ɾ����ǰ����
static void ezos_task_idle(ezos_state_t *state, ezos_para_t *para);             ///< ϵͳ��������
/**
  * @}
  */



/** @defgroup ezOS_Kernel_Function
  * @brief    ����ԭ�ļ�
  * @{
  */

/**
  ******************************************************************************
  * @brief  ezos��ʼ������
  * @param  none
  * @retval none
  ******************************************************************************
  */
void ezos_init(void)
{
    uint8_t i;
    uint32_t mask;

    mask = ezos_disable_int();

    idle.next = &idle;                                                          // ��������Ϊѭ������
    idle.pfun = ezos_task_idle;                                                 // ����������
    idle.name = EZOS_NULL;                                                      // ������������Ϊ�ض�ֵ EZOS_NULL
#ifdef __EZOS_IPC
    idle.ipc  = EZOS_NULL;
#endif
    idle.time = 0;
    idle.status = EZOS_READY;                                                   // ��������ʼ��Ϊ����״̬
    idle.prio = 0xFF;                                                           // ������������ȼ����, Ϊ�ض�ֵ0xFF
    idle_hook = EZOS_NULL;                                                      // ���������Ӻ���

#ifdef __EZOS_IPC
    ipc.next  = (void *)&ipc;                                                   // IPCѭ������
    ipc.name  = EZOS_NULL;                                                      // IPC��ʼ�ڵ�����Ϊ�ض�ֵ0
    ipc.type  = EZOS_DELETE;                                                    // IPC��ʼ�ڵ�Ϊɾ��״̬
#endif

    for (i=0; i < (__EZOS_TASK_NUM-1); i++)                                     // ��������Ϊ��������
    {
        task_link[i].next   = &task_link[i+1];
        task_link[i].name   = EZOS_NULL;
        task_link[i].status = EZOS_DELETE;
    }
    task_link[__EZOS_TASK_NUM-1].next = EZOS_NULL;                              // ������������ڵ�
    trash = &task_link[0];                                                      // ����������ʼ�ڵ�
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
  * @brief  ezOS ϵͳʱ�Ӷ�ʱɨ��. �ڶ�ʱ�ж��е���.
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
        if (task->time > 0)                                                     // ������ʱ״̬
        {
            if (--task->time == 0) task->status = EZOS_READY;                   // �������̬
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
  * @brief  ������Ⱥ���, ����main���������.
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
        while (task->status != EZOS_READY)                                      // Ѱ�Ҿ���������
        {
            task = task->next;
        }
        run = task;
        run->status = EZOS_RUNNING;
        ezos_enable_int(mask);
        task->pfun(&task->state, &task->para);                                  // ����������
        mask = ezos_disable_int();
        if (run->status == EZOS_RUNNING) ezos_delete_cur();                     // �Զ�ɾ������
        run = &idle;
    }
}

/**
  ******************************************************************************
  * @brief  �������, ����״̬��ʼ��Ϊ0ֵ. �����ж���ʹ��.
  * @param  name,       ��������, ����Ϊ EZOS_NULL
  * @param  pfun,       ��������
  * @param  para,       ��������
  * @param  time_ms,    ��ʱʱ��, ��λ ms
  *   @arg  <0,         ��ʼ��Ϊ���ù���״̬
  *   @arg  0,          ��ʼ��Ϊ����״̬
  *   @arg  >0,         ��ʼ��Ϊ����״̬, ��ʱһ��ʱ��
  * @param  priority,   �������ȼ�. ��Чֵ0-254, ֵԽС���ȼ�Խ��
  * @retval ����ֵ
  *   @arg  EZOS_OK,    �������ɹ�
  *   @arg  EZOS_ERROR, �������ʧ��
  *   @arg  EZOS_EXSIT, �������ʧ��, �����Ѵ���
  *   @arg  EZOS_FULL,  �������ʧ��, ��������
  ******************************************************************************
  */
ezos_t ezos_add(void *name, void (*pfun)(ezos_state_t *, ezos_para_t *), ezos_para_t para, int32_t time_ms, uint8_t priority)
{
    uint32_t mask;
    ezos_struct_t *task, *pre, *search;

    if (name  == EZOS_NULL) return EZOS_ERROR;                                  // ����������Ч, ������
    if (trash == EZOS_NULL) return EZOS_FULL;                                   // ������������, ��������Ϊ��

    mask = ezos_disable_int();

    if (priority == 0xFF) priority = 0xFE;                                      // 0xFF���ȼ����޿�������ʹ��
    if (time_ms < 0)                                                            // ���ù���ֵ
    {
        time_ms = EZOS_FOREVER;
    }
#if (__EZOS_TICK_MS > 1)
    else                                                                        // ��ms����ʱֵת��Ϊ����ʱ�ӵ���ʱֵ
    {
        if (time_ms % __EZOS_TICK_MS) ++time_ms;
        time_ms /= __EZOS_TICK_MS;
    }
#endif

    // ��������Ƿ����
    task = idle.next;
    while (task->name != EZOS_NULL)
    {
        if (task->name == name)                                                 // �����Ѵ���
        {
            ezos_enable_int(mask);
            return EZOS_EXSIT;
        }
        task = task->next;
    }

    // �ӻ���������ȡ��һ���ڵ�
    if (trash != run)                                                           // ��������ڵ�����״̬
    {
        task  = trash;                                                          // ȡ���û�������ڵ�
        trash = task->next;
    }
    else                                                                        // ����ж���ɾ�����������е�����, ����Ҫ����������ռ�
    {
        task = trash->next;                                                     // ȡ��һ����������ڵ�
        if (task == EZOS_NULL)                                                  // ����ڵ���Ч, ������
        {
            ezos_enable_int(mask);
            return EZOS_FULL;                                                   // ������������, ��������ռ䲻��
        }
        trash->next = task->next;
    }

    // Ѱ��Ҫ�����ǰ��ڵ�
    pre    = &idle;
    search = idle.next;
    while (search->prio <= priority)                                            // �ӵ�ͬ���ȼ����������ĩβ
    {
        pre    = search;
        search = search->next;
    }
    task->next = search;                                                        // ��������ڵ�
    pre->next  = task;

    // ������ڵ㸳ֵ
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
    return EZOS_OK;                                                             // �������ɹ�
}

/**
  ******************************************************************************
  * @brief  ɾ����ǰ����. �������Ⱥ���ʹ��.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void ezos_delete_cur(void)
{
    ezos_struct_t *pre, *search;

    pre    = &idle;
    search = idle.next;
    while (search->name != EZOS_NULL)                                           // Ѱ������
    {
        if (search->name == run->name)                                          // �������ҵ�
        {
            search->name   = EZOS_NULL;                                         // ɾ������
            search->status = EZOS_DELETE;                                       // ��Ϊɾ��״̬
            pre->next      = search->next;                                      // ������������ɾ������ڵ�
            search->next   = trash->next;                                       // ��ӵ�ɾ������
            trash->next    = search;
        #ifdef __EZOS_DEBUG
            ++trash_num;
        #endif
            break;                                                              // ɾ����ǰ����ɹ�
        }
        pre    = search;
        search = search->next;
    }
}

/**
  ******************************************************************************
  * @brief  ɾ������. �����ж���ʹ��.
  *         һ�㲻ʹ��, ������������ɾ��
  * @param  name,       ��������, ����Ϊ EZOS_NULL
  * @retval ����ֵ
  *   @arg  EZOS_OK,    ɾ������ɹ�
  *   @arg  EZOS_ERROR, ɾ������ʧ��
  ******************************************************************************
  */
ezos_t ezos_delete(void *name)
{
    ezos_t val;
    uint32_t mask;
    ezos_struct_t *pre, *search;

    mask = ezos_disable_int();

    val    = EZOS_ERROR;                                                        // Ĭ�����񲻴���, ����Ϊ��ɾ��
    pre    = &idle;
    search = idle.next;
    while (search->name != EZOS_NULL)                                           // Ѱ������
    {
        if (search->name == name)                                               // �������ҵ�
        {
            search->name   = EZOS_NULL;                                         // ɾ������
            search->status = EZOS_DELETE;                                       // ��Ϊɾ��״̬
            pre->next      = search->next;                                      // ������������ɾ������ڵ�
            search->next   = trash->next;                                       // ��ӵ�ɾ������
            trash->next    = search;
        #ifdef __EZOS_DEBUG
            ++trash_num;
        #endif
            val = EZOS_OK;                                                      // ɾ������ɹ�
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
  * @brief  ��ǰ������ʱ. ֻ������������ʹ��
  * @param  time_ms,    ��ʱʱ��, ��λ ms
  *   @arg  <0,         ���ù���״̬
  *   @arg  0,          ����״̬
  *   @arg  >0,         ����״̬, ��ʱһ��ʱ��
  * @retval ����ֵ
  *   @arg  EZOS_OK,    ��ǰ������ʱ�ɹ�
  *   @arg  EZOS_ERROR, ��ǰ������ʱʧ��
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
    else                                                                        // ��ms����ʱֵת��Ϊ����ʱ�ӵ���ʱֵ
    {
        if (time_ms % __EZOS_TICK_MS) ++time_ms;
        time_ms /= __EZOS_TICK_MS;
    }
#endif

    val  = EZOS_ERROR;
    task = idle.next;
    pre  = &idle;
    while (task->name != EZOS_NULL)                                             // ��������Ƿ����
    {
        if (task->name == run->name)                                            // �������
        {
            task->time = time_ms;
            if (time_ms == 0) task->status = EZOS_READY;
            else              task->status = EZOS_SUSPEND;
            pre->next = task->next;                                             // ��ʱ������������
            search    = task->next;
            while (search->prio <= task->prio)                                  // ת��ͬ���ȼ����������ĩβ
            {
                pre    = search;
                search = search->next;
            }
            pre->next  = task;                                                  // ���¼�����������
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
  * @brief  ��������. ������ʹ�ô˺���.
  *         һ�㽨��ʹ���ź���������������ͨѶ.
  *         ��δ�ṩ�ź���������ͨѶ����ܼ�ʱ, �����ô˺���.
  * @param  name,       ��������, ����Ϊ EZOS_NULL
  * @param  para,       ��������
  * @retval ����ֵ
  *   @arg  EZOS_OK,    ��������ɹ�
  *   @arg  EZOS_ERROR, ��������ʧ��
  *   @arg  EZOS_EXSIT, ��������ʧ��, �����������л��Ѵ��ھ���״̬
  ******************************************************************************
  */
ezos_t ezos_resume(void *name, ezos_para_t para)
{
    ezos_t val;
    uint32_t mask;
    ezos_struct_t *search;

    mask = ezos_disable_int();

    val    = EZOS_ERROR;                                                        // Ĭ�����񲻴���, ����Ϊ��ɾ��
    search = idle.next;
    while (search->name != EZOS_NULL)                                           // Ѱ������
    {
        if (search->name == name)                                               // �������ҵ�
        {
            if (search->status == EZOS_SUSPEND)                                 // ��������״̬ʱ
            {
                search->status = EZOS_READY;                                    // ��Ϊ����״̬
                search->para   = para;                                          // �����������
                val = EZOS_OK;                                                  // ɾ������ɹ�
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
  * @brief  ��ȡ����״̬. �����ж���ʹ��.
  * @param  name,        ��������
  * @retval ����ֵ
  *   @arg  EZOS_ERROR,  ��ȡ����״̬ʧ��
  *   @arg  EZOS_RUNNING,��������״̬
  *   @arg  EZOS_READY,  �������״̬
  *   @arg  EZOS_SUSPEND,�������״̬
  *   @arg  EZOS_DELETE, ����ɾ��״̬, �����ڸ�����
  ******************************************************************************
  */
ezos_t ezos_status_get(void *name)
{
    ezos_t   status;
    uint32_t mask;
    ezos_struct_t *search;

    if (name == EZOS_NULL) return EZOS_ERROR;                                   // �������ѯϵͳ��������
    mask = ezos_disable_int();

    search = idle.next;
    while (search->name != EZOS_NULL)                                           // ��������Ƿ����
    {
        if (search->name == name)                                               // �������
        {
            status = search->status;
            ezos_enable_int(mask);
            return status;
        }
        search = search->next;
    }

    ezos_enable_int(mask);
    return EZOS_DELETE;                                                         // ��������������, ��Ϊ��ɾ��������
}

/**
  ******************************************************************************
  * @brief  ��ȡ��ǰ����״̬.
  * @param  none
  * @retval ����ֵ
  *   @arg  EZOS_ERROR,  ��ȡ����״̬ʧ��
  *   @arg  EZOS_RUNNING,��������״̬
  *   @arg  EZOS_READY,  �������״̬
  *   @arg  EZOS_SUSPEND,�������״̬
  *   @arg  EZOS_DELETE, ����ɾ��״̬, �����ڸ�����
  ******************************************************************************
  */
ezos_t ezos_status_get_cur(void)
{
    if (run && run->name) return run->status;
    else                  return EZOS_ERROR;
}

/**
  ******************************************************************************
  * @brief  ��ȡ��������ڵ����ʷ��Сֵ
  * @param  none
  * @retval ����ֵ, ��������ڵ���ʷ��Сֵ
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
  * @brief  ��ȡ�����������м�����ֵ
  * @param  none
  * @retval ����ֵ, �����������м�����ֵ
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
  * @brief  ���ÿ��������Ӻ���
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
  * @brief  ezos����������
  * @param  none
  * @retval none
  ******************************************************************************
  */
void ezos_task_idle(ezos_state_t *state, ezos_para_t *para)
{
#ifdef __EZOS_DEBUG
    static uint8_t first_run = 1;

    if (first_run)                                                              // ��������������״�����ǰ��ʱ��
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

    if (idle_hook != EZOS_NULL) idle_hook();                                    // ���ÿ��������Ӻ���
    idle.status = EZOS_READY;                                                   // ��ֹ�����Զ�ɾ��
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
