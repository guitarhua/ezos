/**
  ******************************************************************************
  * @file    ezos_ipc.c
  * @author  mousie
  * @version V2.3.0
  * @date    2014.3.28
  * @brief   ezos������ͨѶ(IPC)��ش���
  * -V2.0.0, mousie, 2012.1.1
  *          - ezos����ͨѶ(IPC)�װ����, ������һ���ļ�
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
#define ezos_enable_int(mask)           __set_PRIMASK(mask)                     ///< ezos�궨��ȫ���жϴ򿪺���, ���������ֲ
#define ezos_disable_int()              __get_PRIMASK(); __disable_irq()        ///< ezos�궨��ȫ���жϹرպ���, ���������ֲ
/**
  * @}
  */



/** @defgroup ezOS_ipc_Private_TypeDefine
  * @brief    ˽�����Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup ezOS_ipc_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup ezOS_ipc_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */
extern ezos_struct_t idle;                                                      ///< ����������ʼ�ڵ�
extern ezos_ipc_t    ipc;                                                       ///< IPC������ʼ�ڵ�
extern ezos_struct_t *run;                                                      ///< ָ���������е�����
/**
  * @}
  */

/** @defgroup ezOS_ipc_Private_Function
  * @brief    ����˽�к���
  * @{
  */
static ezos_t ezos_ipc_add(void *name, ezos_t type);
static ezos_t ezos_ipc_delete(void *name);
/**
  * @}
  */



/** @defgroup ezOS_ipc_Function
  * @brief    ����ԭ�ļ�
  * @{
  */
/**
  ******************************************************************************
  * @brief  ������ݵ�IPCѭ��������
  * @param  name,       IPC����, ͬʱҲ��IPCָ��. ����Ϊ EZOS_NULL
  * @param  type,       IPC����
  * @retval ����ֵ
  *   @arg  EZOS_OK,    ���IPC�ɹ�
  *   @arg  EZOS_EXSIT, ���IPCʧ��, IPC�Ѵ���
  ******************************************************************************
  */
ezos_t ezos_ipc_add(void *name, ezos_t type)
{
    ezos_ipc_t *pre    = &ipc;
    ezos_ipc_t *search = ipc.next;
    ezos_ipc_t *node   = name;

    while(search->name != EZOS_NULL)
    {
        if (search->name == name) return EZOS_EXSIT;                            ///< IPC�Ѵ���
        pre    = search;
        search = search->next;
    }

    node->next = pre->next;                                                     ///< ���뵽IPC����
    pre->next  = node;
    node->name = name;
    node->type = type;

    return EZOS_OK;
}

/**
  ******************************************************************************
  * @brief  ��IPC������ɾ������
  * @param  name,       IPC����, ����Ϊ EZOS_NULL
  * @retval ����ֵ
  *   @arg  EZOS_OK,    ɾ��IPC�ɹ�
  *   @arg  EZOS_ERROR, ɾ��IPCʧ��
  ******************************************************************************
  */
ezos_t ezos_ipc_delete(void *name)
{
    ezos_ipc_t *pre    = &ipc;
    ezos_ipc_t *search = ipc.next;

    while(search->name != EZOS_NULL)
    {
        if ((search->name == name))                                             // �ҵ�ָ��IPC
        {
            pre->next    = search->next;                                        // ��IPC������ɾ��
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
  * @brief  ����ź�����IPCѭ��������, �������ж���ʹ��
  * @param  sem,        �ź���ָ��. ����Ϊ EZOS_NULL
  * @param  value,      �ź�����ʼֵ
  * @retval ����ֵ
  *   @arg  EZOS_OK,    ����ź����ɹ�
  *   @arg  EZOS_ERROR, ����ź���ʧ��
  *   @arg  EZOS_EXSIT, ����ź���ʧ��, �����Ѵ���
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
  * @brief  ��IPC������ɾ���ź���, �������ж���ʹ��
  * @param  sem,        �ź���ָ��. ����Ϊ EZOS_NULL
  * @retval ����ֵ
  *   @arg  EZOS_OK,    ɾ���ź����ɹ�
  *   @arg  EZOS_ERROR, ɾ���ź���ʧ��
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
  * @brief  ��ȡ�ź���. ֻ������������ʹ��
  * @param  sem,        �ź���ָ��. ����Ϊ EZOS_NULL
  * @param  time_ms,    �ź����ȴ�ʱ��, ��λ ms
  *   @arg  <0,         ���õȴ�
  *   @arg  0,          ���ȴ�, ֱ�ӷ����ź���״̬
  *   @arg  >0,         �ȴ��ź���һ��ʱ��
  * @retval ����ֵ
  *   @arg  EZOS_OK,      ��ȡ�ź����ɹ�
  *   @arg  EZOS_WAIT,    ��Ҫ�ȴ��ź���, ���ش�ֵ��, ԭ������Ҫreturn
  *   @arg  EZOS_TIMEOUT, �ź����ȴ���ʱ
  *   @arg  EZOS_ERROR,   ָ���ź����д���, ˵����������
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
        if (search->ipc.name == sem)                                            // �ҵ��ź���
        {
            if (search->ipc.type != EZOS_SEM)                                   // �ź������ʹ���
            {
                break;
            }
            if (search->value > 0)                                              // �ɹ���ȡ�ź���
            {
                --search->value;
                run->ipc  = EZOS_NULL;
                run->time = 0;
                rtn = EZOS_OK;
                break;
            }
            else if (run->ipc != search)                                        // �ź����ȴ�����
            {
                if (time_ms == 0)
                {
                    run->ipc  = EZOS_NULL;
                    run->time = 0;
                    rtn = EZOS_TIMEOUT;                                         // ���ȴ�, ֱ�ӷ����ź�����ʱ
                }
                else
                {
                    run->ipc = search;
                    ezos_delay(time_ms);                                        // �������, �ȴ��ź����ͷŻ�ʱ
                    rtn = EZOS_WAIT;
                }
                break;
            }
            else                                                                // �ź�����ʱ����
            {
                if (run->time == 0)
                {
                    run->ipc = EZOS_NULL;                                       // �ź�����ʱ
                    rtn = EZOS_TIMEOUT;
                }
                else
                {
                    run->status = EZOS_SUSPEND;                                 // δ����ź���, ��������ȴ�
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
  * @brief  �ͷ��ź���. �������ж���ʹ��
  * @param  sem,        �ź���ָ��, ����Ϊ EZOS_NULL
  * @retval ����ֵ
  *   @arg  EZOS_OK,    �ͷ��ź����ɹ�
  *   @arg  EZOS_ERROR, �ͷ��ź���ʧ��
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
        if (search->ipc.name == sem)                                            // �ҵ�ָ���ź���
        {
            if (search->ipc.type != EZOS_SEM) break;                            // �ź������ʹ���
            while (task->name != EZOS_NULL)
            {
                if (task->ipc == search) task->status = EZOS_READY;             // �ź����������������״̬
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
  * @brief  ����ź���. �������ж���ʹ��
  * @param  sem,        �ź���ָ��, ����Ϊ EZOS_NULL
  * @retval ����ֵ
  *   @arg  EZOS_OK,    ����ź����ɹ�
  *   @arg  EZOS_ERROR, ����ź���ʧ��
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
        if (search->ipc.name == sem)                                            // �ҵ�ָ���ź���
        {
            if (search->ipc.type != EZOS_SEM) break;                            // �ź������ʹ���
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
  * @brief  ��ӻ�������IPCѭ��������, �������ж���ʹ��
  * @param  mutex,      ������ָ��, ����Ϊ EZOS_NULL
  * @param  value,      ��������ʼֵ
  * @retval ����ֵ
  *   @arg  EZOS_OK,    ��ӻ������ɹ�
  *   @arg  EZOS_ERROR, ��ӻ�����ʧ��
  *   @arg  EZOS_EXSIT, ��ӻ�����ʧ��, �����Ѵ���
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
  * @brief  ��IPC������ɾ��������, �������ж���ʹ��
  * @param  mutex,      ������ָ��, ����Ϊ EZOS_NULL
  * @retval ����ֵ
  *   @arg  EZOS_OK,    ɾ���������ɹ�
  *   @arg  EZOS_ERROR, ɾ��������ʧ��
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
  * @brief  ��ȡ������. ֻ������������ʹ��
  * @param  mutex,      ������ָ��, ����Ϊ EZOS_NULL
  * @param  time_ms,    �������ȴ�ʱ��, ��λ ms
  *   @arg  <0,         ���õȴ�
  *   @arg  0,          ���ȴ�, ֱ�ӷ��ػ�����״̬
  *   @arg  >0,         �ȴ�������һ��ʱ��
  * @retval ����ֵ
  *   @arg  EZOS_OK,      ��ȡ�������ɹ�
  *   @arg  EZOS_WAIT,    ��Ҫ�ȴ�������, ���ش�ֵ��, ԭ������Ҫreturn
  *   @arg  EZOS_TIMEOUT, �������ȴ���ʱ
  *   @arg  EZOS_ERROR,   ָ���������д���, ˵����������
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
        if (search->ipc.name == mutex)                                          // �ҵ�������
        {
            if (search->ipc.type != EZOS_MUTEX)                                 // ���������ʹ���
            {
                break;
            }
            if (search->value > 0)                                              // �ɹ���ȡ������
            {
                search->value = 0;
                run->ipc  = EZOS_NULL;
                run->time = 0;
                rtn = EZOS_OK;
                break;
            }
            else if (run->ipc != search)                                        // �������ȴ�����
            {
                if (time_ms == 0)
                {
                    run->ipc  = EZOS_NULL;
                    run->time = 0;
                    rtn = EZOS_TIMEOUT;                                         // ���ȴ�, ֱ�ӷ��ػ�������ʱ
                }
                else
                {
                    run->ipc = search;
                    ezos_delay(time_ms);                                        // �������, �ȴ��������ͷŻ�ʱ
                    rtn = EZOS_WAIT;
                }
                break;
            }
            else                                                                // ��������ʱ����
            {
                if (run->time == 0)
                {
                    run->ipc = EZOS_NULL;                                       // ��������ʱ
                    rtn = EZOS_TIMEOUT;
                }
                else
                {
                    run->status = EZOS_SUSPEND;                                 // δ��û�����, ��������ȴ�
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
  * @brief  �ͷŻ�����. �������ж���ʹ��
  * @param  mutex,      ������ָ��, ����Ϊ EZOS_NULL
  * @retval ����ֵ
  *   @arg  EZOS_OK,    �ͷŻ������ɹ�
  *   @arg  EZOS_ERROR, �ͷŻ�����ʧ��
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
        if (search->ipc.name == mutex)                                          // �ҵ�ָ��������
        {
            if (search->ipc.type != EZOS_MUTEX) break;                          // ���������ʹ���
            while (task->name != EZOS_NULL)
            {
                if (task->ipc == search) task->status = EZOS_READY;             // �������������������״̬
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
  * @brief  ��ջ�����. �������ж���ʹ��
  * @param  mutex,      ������ָ��, ����Ϊ EZOS_NULL
  * @retval ����ֵ
  *   @arg  EZOS_OK,    ��ջ������ɹ�
  *   @arg  EZOS_ERROR, ��ջ�����ʧ��
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
        if (search->ipc.name == mutex)                                          // �ҵ�ָ��������
        {
            if (search->ipc.type != EZOS_MUTEX) break;                          // ���������ʹ���
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
  * @brief  ����¼���IPCѭ��������, �������ж���ʹ��
  * @param  event,      �¼�ָ��, ����Ϊ EZOS_NULL
  * @param  value,      �¼���ʼֵ
  * @retval ����ֵ
  *   @arg  EZOS_OK,    ����¼��ɹ�
  *   @arg  EZOS_ERROR, ����¼�ʧ��
  *   @arg  EZOS_EXSIT, ����¼�ʧ��, �����Ѵ���
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
  * @brief  ��IPC������ɾ���¼�, �������ж���ʹ��
  * @param  event,      �¼�ָ��, ����Ϊ EZOS_NULL
  * @retval ����ֵ
  *   @arg  EZOS_OK,    ɾ���¼��ɹ�
  *   @arg  EZOS_ERROR, ɾ���¼�ʧ��
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
  * @brief  ��ȡһ���¼�. ֻ������������ʹ��
  * @param  event,      �¼�ָ��, ����Ϊ EZOS_NULL
  * @param  value,      �¼�ֵ
  * @param  option,     �¼�����
  *   @arg  EVENT_OR,   �¼���
  *   @arg  EVENT_AND,  �¼���
  *   @arg  EVENT_KEEP, ��ȡ�¼���, �����¼�ֵ
  * @param  time_ms,    �¼��ȴ�ʱ��, ��λ ms
  *   @arg  <0,         ���õȴ�
  *   @arg  0,          ���ȴ�, ֱ�ӷ����¼�״̬
  *   @arg  >0,         �ȴ��¼�һ��ʱ��
  * @retval ����ֵ
  *   @arg  EZOS_OK,      ��ȡ�¼��ɹ�
  *   @arg  EZOS_WAIT,    ��Ҫ�ȴ��¼�, ���ش�ֵ��, ԭ������Ҫreturn
  *   @arg  EZOS_TIMEOUT, �¼��ȴ���ʱ
  *   @arg  EZOS_ERROR,   ָ���¼��д���, ˵����������
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
        if (search->ipc.name == event)                                          // �ҵ��¼�
        {
            if (search->ipc.type != EZOS_EVENT) break;                          // �¼����ʹ���

            if (option & EZOS_EVENT_OR)                                         // �¼���
            {
                if (search->value | value) rtn = EZOS_OK;
            }
            else if (option & EZOS_EVENT_AND)                                   // �¼���
            {
                if ((search->value & value) == value) rtn = EZOS_OK;
            }

            if (rtn == EZOS_OK)                                                 // �ɹ���ȡ�¼�
            {
                if ((option & EZOS_EVENT_KEEP) == 0) search->value &= ~value;
                run->ipc  = EZOS_NULL;
                run->time = 0;
                break;
            }
            else if (run->ipc != search)                                        // �¼��ȴ�����
            {
                if (time_ms == 0)
                {
                    run->ipc  = EZOS_NULL;
                    run->time = 0;
                    rtn = EZOS_TIMEOUT;                                         // ���ȴ�, ֱ�ӷ����¼���ʱ
                }
                else
                {
                    run->ipc = search;
                    ezos_delay(time_ms);                                        // �������, �ȴ��¼��ͷŻ�ʱ
                    rtn = EZOS_WAIT;
                }
                break;
            }
            else                                                                // �¼���ʱ����
            {
                if (run->time == 0)
                {
                    run->ipc = EZOS_NULL;                                       // �¼���ʱ
                    rtn = EZOS_TIMEOUT;
                }
                else
                {
                    run->status = EZOS_SUSPEND;                                 // δ����¼�, ��������ȴ�
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
  * @brief  �ͷ�һ���¼�. �������ж���ʹ��
  * @param  event,      �¼�ָ��, ����Ϊ EZOS_NULL
  * @param  value,      �¼�ֵ
  * @retval ����ֵ
  *   @arg  EZOS_OK,    �ͷ��¼��ɹ�
  *   @arg  EZOS_ERROR, �ͷ��¼�ʧ��
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
        if (search->ipc.name == event)                                          // �ҵ�ָ���¼�
        {
            if (search->ipc.type != EZOS_EVENT) break;                          // �¼����ʹ���
            while (task->name != EZOS_NULL)
            {
                if (task->ipc == search) task->status = EZOS_READY;             // �¼��������������״̬
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
  * @brief  ����¼�. �������ж���ʹ��
  * @param  event,      �¼�ָ��, ����Ϊ EZOS_NULL
  * @retval ����ֵ
  *   @arg  EZOS_OK,    ����¼��ɹ�
  *   @arg  EZOS_ERROR, ����¼�ʧ��
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
        if (search->ipc.name == event)                                          // �ҵ�ָ���¼�
        {
            if (search->ipc.type != EZOS_EVENT) break;                          // �¼����ʹ���
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
  * @brief  ������䵽IPCѭ��������, �������ж���ʹ��
  * @param  mailbox,    ����ָ��, ����Ϊ EZOS_NULL
  * @param  pool,       ���仺���ָ��
  * @param  pool_size,  ���仺��ش�С
  * @retval ����ֵ
  *   @arg  EZOS_OK,    �������ɹ�
  *   @arg  EZOS_ERROR, �������ʧ��
  *   @arg  EZOS_EXSIT, �������ʧ��, �����Ѵ���
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
  * @brief  ��IPC������ɾ������, �������ж���ʹ��
  * @param  mailbox,    ����ָ��, ����Ϊ EZOS_NULL
  * @retval ����ֵ
  *   @arg  EZOS_OK,    ɾ������ɹ�
  *   @arg  EZOS_ERROR, ɾ������ʧ��
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
  * @brief  ����һ���ʼ�. �������ж���ʹ��
  * @param  mailbox,    ����ָ��, ����Ϊ EZOS_NULL
  * @param  mail,       �ʼ�ֵ
  * @retval ����ֵ
  *   @arg  EZOS_OK,    �����ʼ��ɹ�
  *   @arg  EZOS_ERROR, �����ʼ�ʧ��
  *   @arg  EZOS_FULL,  �����ʼ�ʧ��, ��������
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
        if (search->ipc.name == mailbox)                                        // �ҵ�ָ������
        {
            if (search->ipc.type != EZOS_MAILBOX) break;                        // �������ʹ���
            while (task->name != EZOS_NULL)
            {
                if (task->ipc == search) task->status = EZOS_READY;             // �����������������״̬
                task = task->next;
            }
            if (search->mails_num < search->mails_max)
            {
                search->pool[search->in] = mail;                                // ���ʼ�����������
                ++search->in;
                if (search->in >= search->mails_max) search->in = 0;
                ++search->mails_num;
                rtn = EZOS_OK;
            }
            else rtn = EZOS_FULL;                                               // ��������, ����ʧ��
            break;
        }
        search = search->ipc.next;
    }

    ezos_enable_int(mask);
    return rtn;
}

/**
  ******************************************************************************
  * @brief  ����һ���ʼ�. ֻ������������ʹ��
  * @param  mailbox,    ����ָ��, ����Ϊ EZOS_NULL
  * @param  mail,       ����ʼ�ֵ��ָ��
  * @param  time_ms,    �ʼ��ȴ�ʱ��, ��λ ms
  *   @arg  <0,         ���õȴ�
  *   @arg  0,          ���ȴ�, ֱ�ӷ�������״̬
  *   @arg  >0,         �ȴ��ʼ�һ��ʱ��
  * @retval ����ֵ
  *   @arg  EZOS_OK,      ��ȡ�ʼ��ɹ�
  *   @arg  EZOS_WAIT,    ��Ҫ�ȴ��ʼ�, ���ش�ֵ��, ԭ������Ҫreturn
  *   @arg  EZOS_TIMEOUT, �ʼ��ȴ���ʱ
  *   @arg  EZOS_ERROR,   ָ�������д���, ˵����������
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
        if (search->ipc.name == mailbox)                                        // �ҵ�����
        {
            if (search->ipc.type != EZOS_MAILBOX) break;                        // �������ʹ���

            if (search->mails_num > 0)
            {
                *mail = search->pool[search->out];                              // ȡ���ʼ�
                ++search->out;
                if (search->out >= search->mails_max) search->out = 0;
                --search->mails_num;
                run->ipc  = EZOS_NULL;
                run->time = 0;
                rtn = EZOS_OK;
            }
            else if (run->ipc != search)                                        // ����ȴ�����
            {
                if (time_ms == 0)
                {
                    run->ipc  = EZOS_NULL;
                    run->time = 0;
                    rtn = EZOS_TIMEOUT;                                         // ���ȴ�, ֱ�ӷ������䳬ʱ
                }
                else
                {
                    run->ipc = search;
                    ezos_delay(time_ms);                                        // �������
                    rtn = EZOS_WAIT;
                }
                break;
            }
            else                                                                // ���䳬ʱ����
            {
                if (run->time == 0)
                {
                    run->ipc = EZOS_NULL;                                       // ���䳬ʱ
                    rtn = EZOS_TIMEOUT;
                }
                else
                {
                    run->status = EZOS_SUSPEND;                                 // δ����ʼ�, ��������ȴ�
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
  * @brief  ����ʼ�. �������ж���ʹ��
  * @param  mailbox,    ��������, ����Ϊ EZOS_NULL
  * @retval ����ֵ
  *   @arg  EZOS_OK,    ����ʼ��ɹ�
  *   @arg  EZOS_ERROR, ����ʼ�ʧ��
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
        if (search->ipc.name == mailbox)                                        // �ҵ�ָ������
        {
            if (search->ipc.type != EZOS_MAILBOX) break;                        // �������ʹ���
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
  * @brief  �����Ϣ���е�IPCѭ��������, �������ж���ʹ��
  * @param  message,    ��Ϣ����ָ��, ����Ϊ EZOS_NULL
  * @param  msg_size,   ��Ϣ����
  * @param  pool,       ��Ϣ���л����ָ��
  * @param  pool_size,  ��Ϣ���л���ش�С
  * @retval ����ֵ
  *   @arg  EZOS_OK,    �����Ϣ���гɹ�
  *   @arg  EZOS_ERROR, �����Ϣ����ʧ��
  *   @arg  EZOS_EXSIT, �����Ϣ����ʧ��, �����Ѵ���
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
  * @brief  ��IPC������ɾ����Ϣ����, �������ж���ʹ��
  * @param  message,    ��Ϣ����ָ��, ����Ϊ EZOS_NULL
  * @retval ����ֵ
  *   @arg  EZOS_OK,    ɾ����Ϣ���гɹ�
  *   @arg  EZOS_ERROR, ɾ����Ϣ����ʧ��
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
  * @brief  ������Ϣ
  * @param  src,    Դ��ַָ��
  * @param  dst,    Ŀ���ַָ��
  * @param  size,   �ֽ���
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
  * @brief  ����һ����Ϣ, ��СΪ msg_size. �������ж���ʹ��
  * @param  message,    ��Ϣ����ָ��, ����Ϊ EZOS_NULL
  * @param  msg,        ��Ϣָ��
  * @retval ����ֵ
  *   @arg  EZOS_OK,    ������Ϣ�ɹ�
  *   @arg  EZOS_ERROR, ������Ϣʧ��
  *   @arg  EZOS_FULL,  ������Ϣʧ��, ��Ϣ��������
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
        if (search->ipc.name == message)                                        // �ҵ�ָ����Ϣ����
        {
            if (search->ipc.type != EZOS_MESSAGE) break;                        // ��Ϣ�������ʹ���
            while (task->name != EZOS_NULL)
            {
                if (task->ipc == search) task->status = EZOS_READY;             // ��Ϣ�����������������״̬
                task = task->next;
            }
            if ((search->msgs_num + search->msg_size - 1) < search->msgs_max)
            {
                // ����Ϣ���뵽��Ϣ������
                message_copy(msg, (uint8_t *)search->pool + search->in, search->msg_size);
                search->in += search->msg_size;
                if (search->in >= search->msgs_max) search->in = 0;
                search->msgs_num += search->msg_size;
                rtn = EZOS_OK;
            }
            else rtn = EZOS_FULL;                                               // ��Ϣ��������, ����ʧ��
            break;
        }
        search = search->ipc.next;
    }

    ezos_enable_int(mask);
    return rtn;
}

/**
  ******************************************************************************
  * @brief  ����һ����Ϣ, ��СΪ msg_size. ֻ������������ʹ��
  * @param  message,    ��Ϣ����ָ��, ����Ϊ EZOS_NULL
  * @param  msg,        �����Ϣ��ָ��
  * @param  time_ms,    ��Ϣ�ȴ�ʱ��, ��λ ms
  *   @arg  <0,         ���õȴ�
  *   @arg  0,          ���ȴ�, ֱ�ӷ�����Ϣ����״̬
  *   @arg  >0,         �ȴ���Ϣһ��ʱ��
  * @retval ����ֵ
  *   @arg  EZOS_OK,      ��ȡ��Ϣ�ɹ�
  *   @arg  EZOS_WAIT,    ��Ҫ�ȴ���Ϣ, ���ش�ֵ��, ԭ������Ҫreturn
  *   @arg  EZOS_TIMEOUT, ��Ϣ�ȴ���ʱ
  *   @arg  EZOS_ERROR,   ָ����Ϣ�����д���, ˵����������
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
        if (search->ipc.name == message)                                        // �ҵ���Ϣ����
        {
            if (search->ipc.type != EZOS_MESSAGE) break;                        // ��Ϣ�������ʹ���

            if (search->msgs_num > 0)
            {
                // ����Ϣ������ȡ����Ϣ
                message_copy((uint8_t *)search->pool + search->out, msg, search->msg_size);
                search->out += search->msg_size;
                if (search->out >= search->msgs_max) search->out = 0;
                search->msgs_num -= search->msg_size;
                run->ipc  = EZOS_NULL;
                run->time = 0;
                rtn = EZOS_OK;
            }
            else if (run->ipc != search)                                        // ��Ϣ���еȴ�����
            {
                if (time_ms == 0)
                {
                    run->ipc  = EZOS_NULL;
                    run->time = 0;
                    rtn = EZOS_TIMEOUT;                                         // ���ȴ�, ֱ�ӷ�����Ϣ���г�ʱ
                }
                else
                {
                    run->ipc = search;
                    ezos_delay(time_ms);                                        // �������
                    rtn = EZOS_WAIT;
                }
                break;
            }
            else                                                                // ��Ϣ���г�ʱ����
            {
                if (run->time == 0)
                {
                    run->ipc = EZOS_NULL;                                       // ��Ϣ���г�ʱ
                    rtn = EZOS_TIMEOUT;
                }
                else
                {
                    run->status = EZOS_SUSPEND;                                 // δ�����Ϣ, ��������ȴ�
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
  * @brief  �����Ϣ. �������ж���ʹ��
  * @param  message,    ��Ϣ����ָ��, ����Ϊ EZOS_NULL
  * @retval ����ֵ
  *   @arg  EZOS_OK,    �����Ϣ�ɹ�
  *   @arg  EZOS_ERROR, �����Ϣʧ��
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
        if (search->ipc.name == message)                                        // �ҵ�ָ����Ϣ����
        {
            if (search->ipc.type != EZOS_MESSAGE) break;                        // ��Ϣ�������ʹ���
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
