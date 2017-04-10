// �������º�, LED�Ʒ�ת, ʾ���ź������������Ƶ�ʹ��

#include "ezos.h"
#include "drv_led.h"
#include "drv_key.h"

ezos_sem_t key1;
ezos_sem_t key2;

static EZOS_TASK(task_key);
static EZOS_TASK(task_led);

void sem_start(void)
{
    led_init_all();
    key_init_all();
    ezos_sem_add(&key1, 0);
    ezos_sem_add(&key2, 0);
    ezos_add(task_key, task_key, 0, 0, 3);
    ezos_add(EZOS_NAME(task_led, 0), task_led, LED1, 0, 8);
    ezos_add(EZOS_NAME(task_led, 1), task_led, LED2, 0, 8);
}

EZOS_TASK(task_key)
{
    key_scan_10ms();
    switch(key_status.click)
    {
    case KEY_MASK(KEY1):
        ezos_sem_release(&key1);                            // �ͷ��ź���
        break;
    case KEY_MASK(KEY2):
        ezos_sem_release(&key2);
        break;
    }

    ezos_delay(10);
}

EZOS_TASK(task_led)
{
    ezos_t val;

    if ((led_name_t)(*para) == LED1)
    {
        val = ezos_sem_take(&key1, EZOS_FOREVER);           // ��ȡkey1�ź���
        if (val != EZOS_WAIT)
        {
            if (val == EZOS_OK) led_toggle_gradual(LED1);   // �ɹ���ȡ�ź���
            else assert_printf("sem key1: %d\r\n", val);    // ����, ������Ϣ
            ezos_delay(0);                                  // ����ʱ, �������̬
        }
    }
    else if ((led_name_t)(*para) == LED2)
    {
        val = ezos_sem_take(&key2, EZOS_FOREVER);           // ��ȡkey2�ź���
        if (val != EZOS_WAIT)
        {
            if (val == EZOS_OK) led_toggle_gradual(LED2);   // �ɹ���ȡ�ź���
            else assert_printf("sem key2: %d\r\n", val);    // ����, ������Ϣ
            ezos_delay(0);                                  // ����ʱ, �������̬
        }
    }
}
