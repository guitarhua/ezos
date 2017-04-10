// �������º�, LED�Ʒ�ת, ʾ���ź������������Ƶ�ʹ��

#include "ezos.h"
#include "drv_led.h"
#include "drv_key.h"
#include "drv_extint.h"

ezos_sem_t key2;
ezos_sem_t key3;
ezos_sem_t key4;

EZOS_TASK(task_key);
EZOS_TASK(task_led);

void key_led_start(void)
{
    led_init_all();
    key_init_all();
    extint_init(S3_WAKEUP, EXTI_Trigger_Falling);

    ezos_sem_add(&key2, 0);
    ezos_sem_add(&key3, 0);
    ezos_sem_add(&key4, 0);
    ezos_add(task_key, task_key, 0, 0, 3);
    ezos_add(EZOS_NAME(task_led, 0), task_led, LED2, 0, 8);
    ezos_add(EZOS_NAME(task_led, 1), task_led, LED3, 0, 8);
    ezos_add(EZOS_NAME(task_led, 2), task_led, LED4, 0, 8);
}

void int_key(void)
{
    ezos_sem_release(&key3);                                // �ж����ͷ��ź���
}

EZOS_TASK(task_key)
{
    key_scan_10ms();
    switch(key_status.click)
    {
    case KEY_MASK(S2_TAMPER):
        ezos_sem_release(&key2);                            // �ͷ��ź���
        break;
    case KEY_MASK(S4_USER):
        ezos_sem_release(&key4);
        break;
    }

    ezos_delay(10);
}

EZOS_TASK(task_led)
{
    ezos_t val;

    if ((led_name_t)(uint32_t)(*para) == LED2)
    {
        val = ezos_sem_take(&key2, EZOS_FOREVER);           // ��ȡkey1�ź���
        if (val != EZOS_WAIT)
        {
            if (val == EZOS_OK) led_toggle_gradual(LED2);   // �ɹ���ȡ�ź���
            else assert_printf("sem key1: %d\r\n", val);    // ����, ������Ϣ
            ezos_delay(0);                                  // ����ʱ, �������̬
        }
    }
    else if ((led_name_t)(uint32_t)(*para) == LED3)
    {
        val = ezos_sem_take(&key3, EZOS_FOREVER);           // ��ȡkey2�ź���
        if (val != EZOS_WAIT)
        {
            if (val == EZOS_OK) led_toggle(LED3);           // �ɹ���ȡ�ź���
            else assert_printf("sem key2: %d\r\n", val);    // ����, ������Ϣ
            ezos_delay(0);                                  // ����ʱ, �������̬
        }
    }
    else if ((led_name_t)(uint32_t)(*para) == LED4)
    {
        val = ezos_sem_take(&key4, EZOS_FOREVER);           // ��ȡkey3�ź���
        if (val != EZOS_WAIT)
        {
            if (val == EZOS_OK) led_toggle_gradual(LED4);   // �ɹ���ȡ�ź���
            else assert_printf("sem key3: %d\r\n", val);    // ����, ������Ϣ
            ezos_delay(0);                                  // ����ʱ, �������̬
        }
    }
}
