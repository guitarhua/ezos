// 按键按下后, LED灯翻转, 示例信号量和任务名称的使用

#include "ezos.h"
#include "drv_led.h"
#include "drv_key.h"

ezos_sem_t key;

EZOS_TASK(task_key);
EZOS_TASK(task_led);

void key_led_start(void)
{
    led_init_all();
    key_init_all();

    ezos_sem_add(&key, 0);
    ezos_add(task_key, task_key, 0, 0, 3);
    ezos_add(task_led, task_led, 0, 0, 8);
}

EZOS_TASK(task_key)
{
    key_scan_10ms();
    switch(key_status.click)
    {
    case KEY_MASK(TAMPER):
        ezos_sem_release(&key);                             // 释放信号量
        break;
    }

    ezos_delay(10);
}

EZOS_TASK(task_led)
{
    ezos_t val;

    val = ezos_sem_take(&key, EZOS_FOREVER) ;               // 获取key信号量
    if (val != EZOS_WAIT)
    {
        if (val == EZOS_OK) led_toggle_gradual(LED1);       // 成功获取信号量
        else assert_printf("sem key: %d\r\n", val);         // 出错, 调试信息
        ezos_delay(0);                                      // 不延时, 进入就绪态
    }
}
