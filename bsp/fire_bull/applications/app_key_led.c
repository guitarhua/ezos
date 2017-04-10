// 按键按下后, LED灯翻转, 示例信号量和任务名称的使用

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
    ezos_sem_release(&key3);                                // 中断中释放信号量
}

EZOS_TASK(task_key)
{
    key_scan_10ms();
    switch(key_status.click)
    {
    case KEY_MASK(S2_TAMPER):
        ezos_sem_release(&key2);                            // 释放信号量
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
        val = ezos_sem_take(&key2, EZOS_FOREVER);           // 获取key1信号量
        if (val != EZOS_WAIT)
        {
            if (val == EZOS_OK) led_toggle_gradual(LED2);   // 成功获取信号量
            else assert_printf("sem key1: %d\r\n", val);    // 出错, 调试信息
            ezos_delay(0);                                  // 不延时, 进入就绪态
        }
    }
    else if ((led_name_t)(uint32_t)(*para) == LED3)
    {
        val = ezos_sem_take(&key3, EZOS_FOREVER);           // 获取key2信号量
        if (val != EZOS_WAIT)
        {
            if (val == EZOS_OK) led_toggle(LED3);           // 成功获取信号量
            else assert_printf("sem key2: %d\r\n", val);    // 出错, 调试信息
            ezos_delay(0);                                  // 不延时, 进入就绪态
        }
    }
    else if ((led_name_t)(uint32_t)(*para) == LED4)
    {
        val = ezos_sem_take(&key4, EZOS_FOREVER);           // 获取key3信号量
        if (val != EZOS_WAIT)
        {
            if (val == EZOS_OK) led_toggle_gradual(LED4);   // 成功获取信号量
            else assert_printf("sem key3: %d\r\n", val);    // 出错, 调试信息
            ezos_delay(0);                                  // 不延时, 进入就绪态
        }
    }
}
