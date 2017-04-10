// 宏定义的任务使用, 跑马灯, 开关

#include "ezos.h"
#include "drv_led.h"
#include "drv_key.h"

ezos_sem_t sem;

EZOS_TASK(task_define_led);
EZOS_TASK(task_define_key);
EZOS_TASK(task_define_sem);

void define_start(void)
{
    led_init_all();
    key_init_all();

    ezos_sem_add(&sem, 0);
    quick_add(task_define_led, 10);
    quick_add(task_define_key, 3);
    quick_add(task_define_sem, 10);
}

EZOS_TASK(task_define_led)
{
    EZOS_BEGIN(*state);

    led_off(LED1);                                          // LED初始状态全关
    led_off(LED2);
    led_off(LED3);
    led_off(LED4);
    EZOS_DELAY(1000);

    while(1)
    {
        led_toggle_gradual(LED1);
        EZOS_DELAY(1000);
        led_toggle_gradual(LED2);
        EZOS_DELAY(1000);
        led_toggle_gradual(LED3);
        EZOS_DELAY(1000);
        led_toggle_gradual(LED4);
        EZOS_DELAY(1000);
    }

    EZOS_END();
}

EZOS_TASK(task_define_key)
{
    EZOS_BEGIN(*state);

    while(1)
    {
        key_scan_10ms();
        switch(key_status.click)                            // 注意! switch 内不可使用EZOS宏定义操作
        {
        case KEY_MASK(TAMPER):
            ezos_sem_release(&sem);                         // 释放信号量
            assert_printf("sem released: %d\r\n", sem.value);
            break;
        }

        EZOS_DELAY(10);
    }

    EZOS_END();
}

EZOS_TASK(task_define_sem)
{
    EZOS_BEGIN(*state);

    while(1)
    {
        ezos_t val;
        EZOS_WAIT_SUBFUN(val = ezos_sem_take(&sem, EZOS_FOREVER));

        if (val == EZOS_OK)
        {
            led_toggle(LED1);
            led_toggle(LED2);
            led_toggle(LED3);
            led_toggle(LED4);
            assert_printf("sem taked:    %d\r\n", sem.value);
        }
        else
        {
            assert_printf("sem exception:%d\r\n", sem.value);
        }
    }

    EZOS_END();
}
