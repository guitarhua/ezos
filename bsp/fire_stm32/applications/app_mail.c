// 按键按下后, LED灯翻转, 示例邮箱的使用

#include "ezos.h"
#include "drv_led.h"
#include "drv_key.h"

ezos_mb_t mb_key;
uint8_t   mb_buf[10];

static EZOS_TASK(task_key);
static EZOS_TASK(task_led);

void mail_start(void)
{
    led_init_all();
    key_init_all();
    ezos_mb_add(&mb_key, mb_buf, sizeof(mb_buf));
    ezos_add(task_key, task_key, 0, 0, 3);
    ezos_add(task_led, task_led, 0, 0, 8);
}

EZOS_TASK(task_key)
{
    key_scan_10ms();
    switch(key_status.click)
    {
    case KEY_MASK(KEY1):
        ezos_mb_send(&mb_key, LED1);
        break;
    case KEY_MASK(KEY2):
        ezos_mb_send(&mb_key, LED2);
        break;
    }

    ezos_delay(10);
}

EZOS_TASK(task_led)
{
    ezos_t val;
    uint8_t mail;

    val = ezos_mb_receive(&mb_key, &mail, EZOS_FOREVER);
    if (val != EZOS_WAIT)
    {
        if (val == EZOS_OK) led_toggle_gradual((led_name_t)mail);
        else assert_printf("mb_key: %d\r\n", val);          // 出错, 调试信息
        ezos_delay(0);                                      // 不延时, 进入就绪态
    }
}
