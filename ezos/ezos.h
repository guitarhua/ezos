/**
  ******************************************************************************
  * @file    ezos.h
  * @author  mousie
  * @version V2.3.0
  * @date    2014.3.28
  * @brief   Э��ʽRTOS
  *
  *   �����״̬:
  *       1. ����ɾ��״̬, ����ֹ״̬. ����������������, ��������.
  *       2. ��������״̬, ������������������, ��������, ���ᱻ����. ������ʱҲ��������״̬.
  *       3. �������״̬, �����������������, ��������, ��δ����������.
  *       4. ��������״̬, �������������б���, ����������.
  *
  *   ezos����Ӧ�ü���:
  *       1. ���ȵ���ezos_init(); ��ʼ��ezos����
  *       2. ����һ��1ms��ʱ�ж���Ϊezos��ʱ��, ������ezos_scan_1ms();
  *       3. ��main���������, ���� ezos_schedule();
  *       4. һ���, �ڵ���֮ǰ���ٴ���һ������, ��:
  *          ezos_add(task_key_scan, task_key_scan, (void *)0, 10, 8);
  *          �ú�����ʾ���һ������ɨ������, ��������ͬ������, ��ʱ10ms������, ���ȼ�Ϊ8.
  *       5. �������������û�иı��״̬(����� ezos_delay() ����, ������ͨѶ����), ���������������Զ�ɾ��������.
  *       6. ���е�����������ݾ������������. ����������ǲ����ظ���.
  *          Ϊ��ʹ�÷���, �������Ʊ���Ƴ� void * �ĸ�ʽ, �ɷǳ������ʹ�ú���������Ϊ��������.
  *          ���ر�궨���˿����������ĺ���: quick_add(pfun, prio);
  *          quick_add(func, prio);                                             // ���һ������, ���躯������, ��������ͬ������
  *       7. Ҫ��ͬһ��������Ӷ������, ���뱣֤�ú����ǿ�����Ļ����̰߳�ȫ�ĺ���.
  *          ����ʱֻҪ��֤�������Ʋ�һ������. �������ƿ�ʹ��(������+0,1,2)�ķ�ʽ. ����ʹ�úͼ���. ��:
  *          ezos_add(func, func, (void *)0, 0, 0);                             // ����func�����ĵ�һ������, �������� func
  *          ezos_add(EZOS_NAME(func, 1), func, (void *)0, 0, 0);               // ����func�����ĵڶ�������, �������� func+1
  *       8. ���������Ӻ�����ʹ�ý���.
  *          ���Ӻ���ֻ���� void func(void) ��ʽ�ļ򵥺���.
  *          һ�㽨������ι��, �� ��תϵͳ״ָ̬ʾ��.
  *       9. �������Ӧ��: ״̬����ʽ.
  *          ����ռ��CPUʱ��������м���Ҫ��ʱ�ȴ�, �ø������������״̬.
  *          ʹ��״̬����ʽ����������ʱ������CPU��ռ��.
  *          ���Ǳ�֤�����ȼ�������Ӧʱ���һ����Э��ʽ, Ҳ�������CPU��������.
  *          ��LED�����Ϊ��, ��ʽ����:
  *          ezos_task_t task_led(ezos_state_t *state, ezos_para_t *para)
  *          {
  *             switch (*state)
  *             {
  *             case 0:                         // ������Ӻ�ĳ�ʼ״̬, ������Щ����ĳ�ʼ������
  *                 led_toggle(LED1);           // LED1��ת
  *                 ezos_delay(1000);           // ��Ҫ��ʱ1S, ������һ״̬��������CPU��ռ��
  *                 *state = 1;
  *                 break;
  *             default:
  *                 led_toggle(LED2);           // LED2��ת
  *                 ezos_delay(1000);           // ��Ҫ��ʱ1S, ������һ״̬��������CPU��ռ��
  *                 *state = 0;
  *                 break;
  *             }
  *          }
  *       10.����߼�Ӧ��: �궨�巽ʽ.
  *          ͬ����LED�����Ϊ��, ��ʽ����:
  *          ezos_task_t task_led(ezos_state_t *state, ezos_para_t *para)
  *          {
  *             EZOS_BEGIN(*state);
  *             while(1)
  *             {
  *                 led_toggle(LED1);           // LED1��ת
  *                 EZOS_DELAY(1000);           // ��Ҫ��ʱ1S
  *                 led_toggle(LED2);           // LED2��ת
  *                 EZOS_DELAY(1000);           // ��Ҫ��ʱ1S
  *             }
  *             EZOS_END();
  *          }
  *       11.����ͨѶ��ʹ��. ÿ��������ֻ�������ʼʹ��һ��IPC. ��Ҫ�ö��IPC��Ҫʹ��״̬����ʽ.
  *          ���ź���Ϊ��, �����ź����ͷź�����, �ʹ��ź�����ص����񶼻ᱻ��Ϊ����״̬.
  *          ezos_sem_release(lock);
  *          �����ź����Ļ�ȡ, ��������᷵�� EZOS_OK, EZOS_TIMEOUT, EZOS_WAIT, ����EZOS_WAIT, ʹ��return���ؼ���.
  *          ʾ������:
  *          ezos_task_t task_sem(ezos_state_t *state, ezos_para_t *para)
  *          {
  *              ezos_t val;
  *              val = ezos_sem_take(lock, 10);
  *              if (val == EZOS_WAIT) return;                                  // ֱ�ӷ��صȴ�
  *              if      (val == EZOS_OK)      {do_something();}                // �ɹ���ȡ�ź���
  *              else if (val == EZOS_TIMEOUT) {do_something();}                // �ź�����ʱ����
  *              else if (val == EZOS_ERROR)   {exception();}                   // һ�㲻Ӧ����EZOS_ERROR, �쳣����
  *          }
  *       12.������Э��ʽ��RTOS, �ٽ����������ٶ�������Ԥ��. ����ʹ��״̬����ʽʱ���ر�ע�������.
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
  * @brief    �û�����
  ******************************************************************************
  * @{
  */
#include "ezos_conf.h"                                                          // ezos �����ļ�

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
* @brief    �������Ͷ���
* @{
*/
/// ezosö�����Ͷ���
typedef enum
{
    EZOS_OK      = 0,                                                           ///< ������ź��������ɹ�
    EZOS_ERROR   = 1,                                                           ///< ������ź�������ʧ��
    EZOS_FULL    = 2,                                                           ///< ������ź����������
    EZOS_EXSIT   = 3,                                                           ///< ������ź��������Ѵ��ڴ���
    // ����״̬
    EZOS_RUNNING = 4,                                                           ///< ��������״̬
    EZOS_READY   = 5,                                                           ///< �������״̬
    EZOS_SUSPEND = 6,                                                           ///< ��������״̬
    EZOS_DELETE  = 7,                                                           ///< ����ɾ��״̬, �ź���ɾ��״̬
    // �ź���״̬
    EZOS_WAIT    = 8,                                                           ///< ��Ҫ�ȴ��ź���
    EZOS_TIMEOUT = 9,                                                           ///< �ź�����ʱ
    // �ź�������
    EZOS_SEM     = 10,                                                          ///< �ź�������
    EZOS_MUTEX   = 11,                                                          ///< ����������
    EZOS_EVENT   = 12,                                                          ///< �¼�����
    EZOS_MAILBOX = 13,                                                          ///< ��������
    EZOS_MESSAGE = 14,                                                          ///< ��Ϣ��������
} ezos_t;

/// �������Ͷ���. ���ڱ��������
typedef void                ezos_task_t;

/// ״̬���Ͷ���, ָ������state������.
typedef uint16_t            ezos_state_t;

/// �������Ͷ���, ָ������para������. ƽ̨��ֲ��Ϊ��ʡRAM, �ɽ����޸�
typedef uint32_t            ezos_para_t;

/// ����������Ͷ���
struct __ezos_struct
{
    struct __ezos_struct *next;                                                 ///< ��������ָ��
    void *name;                                                                 ///< ��������
    ezos_task_t (*pfun)(ezos_state_t *state, ezos_para_t *para);                ///< ������
#ifdef __EZOS_IPC
    void *ipc;                                                                  ///< �������IPC
#endif
    int32_t      time;                                                          ///< ����������ʱʱ��
    ezos_para_t  para;                                                          ///< ��������
    ezos_state_t state;                                                         ///< ����״̬
    ezos_t       status;                                                        ///< ����״̬
    uint8_t      prio;                                                          ///< �������ȼ�
};
typedef struct __ezos_struct  ezos_struct_t;

/// ����ͨ�����Ͷ���
struct __ezos_ipc
{
    void *next;                                                                 ///< ����ͨ������ָ��
    void *name;                                                                 ///< ����ͨ������
    ezos_t type;                                                                ///< ����ͨ������
};
typedef struct __ezos_ipc   ezos_ipc_t;

/// �ź������Ͷ���
typedef struct
{
    ezos_ipc_t ipc;                                                             ///< ipc��������
    uint8_t value;                                                              ///< �ź���ֵ
} ezos_sem_t;

/// ���������Ͷ���
typedef struct
{
    ezos_ipc_t ipc;                                                             ///< ipc��������
    uint8_t value;                                                              ///< ������ֵ
} ezos_mutex_t;

/// �¼����Ͷ���
typedef struct
{
    ezos_ipc_t ipc;                                                             ///< ipc��������
    uint8_t value;                                                              ///< �¼�ֵ
} ezos_event_t;

/// �������Ͷ���
typedef struct
{
    ezos_ipc_t ipc;                                                             ///< ipc��������
    uint8_t *pool;                                                              ///< �����ڴ��ָ��
    uint16_t mails_max;                                                         ///< ����ʼ�����
    uint16_t mails_num;                                                         ///< ��ǰ�ʼ�����
    uint16_t in;                                                                ///< �ʼ�����ƫ����
    uint16_t out;                                                               ///< �ʼ�����ƫ����
} ezos_mb_t;

/// ��Ϣ�������Ͷ���
typedef struct
{
    ezos_ipc_t ipc;                                                             ///< ipc��������
    void *pool;                                                                 ///< ��Ϣ�����ڴ��ָ��
    uint16_t msg_size;                                                          ///< ������Ϣ�ĳ���
    uint16_t msgs_max;                                                          ///< �����Ϣ����
    uint16_t msgs_num;                                                          ///< ��ǰ��Ϣ����
    uint16_t in;                                                                ///< ��Ϣ����ƫ����
    uint16_t out;                                                               ///< ��Ϣ����ƫ����
} ezos_mq_t;
/**
  * @}
  */

/** @defgroup ezOS_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */
#define EZOS_NULL                       0

/// ezos ʱ��ֵ, ��ʾ���õȴ�
#define EZOS_FOREVER                    (int32_t)-1

/// ezos event �¼����Զ���
#define EZOS_EVENT_OR                   0x01
#define EZOS_EVENT_AND                  0x02
#define EZOS_EVENT_KEEP                 0x04

/// ezos ��ݺ궨��
#define EZOS_TASK(func)                 ezos_task_t func(ezos_state_t *state, ezos_para_t *para)// �������궨��
#define EZOS_NAME(name, id)             ((void *)((uint32_t)(name) + (uint32_t)(id)))           // �������ƺ궨��. ������ʹ��ͬһ����ʱʹ��

/**
  * ezos ����߼�Ӧ��, �����궨��
  * ʹ��ezos�궨�����, �������ڸ�������ʹ��switch���!!! ��Ҫʹ�ñ���ע��:
  * switch �ڲ���ʹ��EZOS�궨�����, �� EZOS_DELAY, EZOS_WAIT_XXX, EZOS_SUBFUN_XXX
  * �ŵ����Ķ�˳������, ��������������, ������չ������
  * ȱ�����﷨�������, ��������, ���нϸ��ӵķ�֧���ʱ, ����ʹ��!
  * �ۺ϶���, ������ʹ��ezos�궨�����, �궨������������û�ɬ�Ѷ�
  * ���������������ʹ��:
  * 1. ������̼�����
  * 2. �����������������, ����Ϊ���������ʽ
  * �����񷵻�ֵ���ͱ���Ϊ ezos_t, ������������
  * ����������ʽΪ ezos_t ezos_sub_function(...)
  * �������Ƕ��ʹ��, ��ʹ�ú궨��EZOS_SUBFUN_XXX
  */
#define LC_EZOS_RESUME(s)               switch(s) { case 0:
#define LC_EZOS_SET(s)                  s = __LINE__; case __LINE__:
#define LC_EZOS_SET_EXECUTE(s, e)       s = __LINE__; e; case __LINE__:
#define LC_EZOS_END(s)                  s = 0; }

/// EZOS ������߼�Ӧ��, �궨�����
#define EZOS_BEGIN(s)                   {ezos_state_t *_s = &s; LC_EZOS_RESUME(*_s)
#define EZOS_END()                      LC_EZOS_END(*_s); return; }             // EZOS_BEGIN() �� EZOS_END() ���ʹ��

#define EZOS_DELAY(time)                \
    do {                                \
        ezos_delay(time);               \
        LC_EZOS_SET_EXECUTE(*_s, return); \
    } while(0)                                                                  // EZOS ��ʱ�궨��

#define EZOS_WAIT_SUBFUN(subfun)        \
    do {                                \
        LC_EZOS_SET(*_s);               \
        if ((subfun) == EZOS_WAIT) return; \
    } while(0)                                                                  // EZOS ��������ú궨��

#define EZOS_WAIT_UNTIL(cond)           \
    do {                                \
        LC_EZOS_SET(*_s);               \
        if (!(cond)) {                  \
            if (ezos_status_get_cur() == EZOS_RUNNING) ezos_delay(__EZOS_TICK_MS); \
            return; }                   \
    } while(0)                                                                  // EZOS �����жϺ궨��

#define EZOS_WAIT_WHILE(cond)           EZOS_WAIT_UNTIL(!(cond))                // EZOS �����жϺ궨��

/// EZOS ������߼�Ӧ��, �궨�����
#define EZOS_SUBFUN_BEGIN(s)            {ezos_state_t *_s = &s; LC_EZOS_RESUME(*_s)
#define EZOS_SUBFUN_END()               LC_EZOS_END(*_s); return EZOS_OK; }

#define EZOS_SUBFUN_DELAY(time)         \
    do {                                \
        ezos_delay(time);               \
        LC_EZOS_SET_EXECUTE(*_s, return EZOS_WAIT); \
    } while(0)                                                                  // EZOS ��ʱ�궨��

#define EZOS_SUBFUN_WAIT_SUBFUN(subfun) \
    do {                                \
        LC_EZOS_SET(*_s);               \
        if ((subfun) == EZOS_WAIT) return EZOS_WAIT; \
    } while(0)                                                                  // EZOS ��������ú궨��

#define EZOS_SUBFUN_WAIT_UNTIL(cond)    \
    do {                                \
        LC_EZOS_SET(*_s);               \
        if (!(cond)) {                  \
            if (ezos_status_get_cur() == EZOS_RUNNING) ezos_delay(__EZOS_TICK_MS); \
            return EZOS_WAIT; }         \
    } while(0)                                                                  // EZOS �����жϺ궨��

#define EZOS_SUBFUN_WAIT_WHILE(cond)    EZOS_SUBFUN_WAIT_UNTIL(!(cond))         // EZOS �����жϺ궨��
/**
  * @}
  */

/** @defgroup ezOS_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */

/**
  * @}
  */

/** @defgroup ezOS_Public_Function
  * @brief    ���幫�к���
  * @{
  */
void ezos_init(void);                                                           // ezOS��ʼ������
void ezos_scan_tick(void);                                                      // ezOS ϵͳʱ�Ӷ�ʱɨ��. �ڶ�ʱ�ж��е���
void ezos_schedule(void);                                                       // �������, main������������
void ezos_idle_hook_set(void (*hook)(void));                                    // ���ÿ��������Ӻ���, ֻ����void fun(void)�ļ򵥺���
uint8_t  ezos_trash_num_min_get(void);                                          // ��ȡ��������ڵ���ʷ��Сֵ. ��ʹ��__EZOS_DEBUG
uint32_t ezos_idle_tick_max_get(void);                                          // ��ȡ�����������м�����ֵ. ��ʹ��__EZOS_DEBUG

ezos_t ezos_add(void *name,                                                     // �������, �������Ʋ���Ϊ EZOS_NULL
                ezos_task_t (*pfun)(ezos_state_t*, ezos_para_t*),               // �������ƺͲ���ָ��
                ezos_para_t para,                                               // �����ڲ�����
                int32_t time_ms,                                                // �����ʼ״̬. EZOS_FOREVER ��������, =0 ����״̬, >0 ������ʱ
                uint8_t priority);                                              // ���ȼ�. ��Чֵ0-254. ֵԽС���ȼ�Խ��.
ezos_t ezos_delete(void *name);                                                 // ɾ��ָ������. һ�㲻ʹ��, ������������ɾ��
ezos_t ezos_delay(int32_t time_ms);                                             // ��ǰ������ʱ. EZOS_FOREVER ��������, =0 ����״̬, >0 ������ʱ
ezos_t ezos_status_get(void *name);                                             // ��ȡָ������״̬
ezos_t ezos_status_get_cur(void);                                               // ��ȡ��ǰ����״̬

// �������һ������, pfun-��������, priority-�������ȼ�
__INLINE static ezos_t quick_add(ezos_task_t (*pfun)(ezos_state_t*, ezos_para_t*), uint8_t priority)
{
    return ezos_add((void *)pfun, pfun, 0, 0, priority);
}

// ǿ�����һ������, ����Ѵ�������, ��ɾ�����������
__INLINE static ezos_t force_add(
                void *name,                                                     // �������, �������Ʋ���Ϊ EZOS_NULL
                ezos_task_t (*pfun)(ezos_state_t*, ezos_para_t*),               // �������ƺͲ���
                ezos_para_t para,                                               // �����ڲ�����
                int32_t time_ms,                                                // �����ʼ״̬. EZOS_FOREVER ��������, =0 ����״̬, >0 ������ʱ.
                uint8_t priority)                                               // ���ȼ�. ��Чֵ0-254. ֵԽС���ȼ�Խ��.
{
    ezos_delete(name);
    return ezos_add(name, pfun, para, time_ms, priority);
}

__INLINE static ezos_t ezos_suspend(void)                                       // ���ù���ǰ����
{
    return ezos_delay(EZOS_FOREVER);
}
ezos_t ezos_resume(void *name, ezos_para_t para);                               // ָ������������̬, ���ź�������ʱ, ������ʹ�ô˺���



#ifdef __EZOS_SEM
ezos_t ezos_sem_add(ezos_sem_t *sem, uint8_t value);                            // ����ź���
ezos_t ezos_sem_delete(ezos_sem_t *sem);                                        // ɾ���ź���
ezos_t ezos_sem_take(ezos_sem_t *sem, int32_t time_ms);                         // ��ȡ�ź���
ezos_t ezos_sem_release(ezos_sem_t *sem);                                       // �ͷ��ź���
ezos_t ezos_sem_clear(ezos_sem_t *sem);                                         // ����ź���
#endif

#ifdef __EZOS_MUTEX
ezos_t ezos_mutex_add(ezos_mutex_t *mutex, uint8_t value);                      // ��ӻ�����
ezos_t ezos_mutex_delete(ezos_mutex_t *mutex);                                  // ɾ��������
ezos_t ezos_mutex_take(ezos_mutex_t *mutex, int32_t time_ms);                   // ��ȡ������
ezos_t ezos_mutex_release(ezos_mutex_t *mutex);                                 // �ͷŻ�����
ezos_t ezos_mutex_clear(ezos_mutex_t *mutex);                                   // ��ջ�����
#endif

#ifdef __EZOS_EVENT
ezos_t ezos_event_add(ezos_event_t *event, uint8_t value);                      // ����¼�
ezos_t ezos_event_delete(ezos_event_t *event);                                  // ɾ���¼�
ezos_t ezos_event_take(ezos_event_t *event, uint8_t value, uint8_t option, int32_t time_ms);// ��ȡһ���¼�
ezos_t ezos_event_release(ezos_event_t *event, uint8_t value);                  // �ͷ�һ���¼�
ezos_t ezos_event_clear(ezos_event_t *event);                                   // ����¼�
#endif

#ifdef __EZOS_MAILBOX
ezos_t ezos_mb_add(ezos_mb_t *mailbox, uint8_t *pool, uint16_t pool_size);      // �������
ezos_t ezos_mb_delete(ezos_mb_t *mailbox);                                      // ɾ������
ezos_t ezos_mb_send(ezos_mb_t *mailbox, uint8_t mail);                          // �����ʼ�
ezos_t ezos_mb_receive(ezos_mb_t *mailbox, uint8_t *mail, int32_t time_ms);     // �����ʼ�
ezos_t ezos_mb_clear(ezos_mb_t *mailbox);                                       // �������
#endif

#ifdef __EZOS_MESSAGE
ezos_t ezos_mq_add(ezos_mq_t *message, uint16_t msg_size,                       // �����Ϣ����
                   void *pool, uint16_t pool_size);
ezos_t ezos_mq_delete(ezos_mq_t *message);                                      // ɾ����Ϣ����
ezos_t ezos_mq_send(ezos_mq_t *message, void *msg);                             // ������Ϣ
ezos_t ezos_mq_receive(ezos_mq_t *message, void *msg, int32_t time_ms);         // ������Ϣ
ezos_t ezos_mq_clear(ezos_mq_t *message);                                       // �����Ϣ
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
