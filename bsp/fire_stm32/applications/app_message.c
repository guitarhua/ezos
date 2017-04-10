// 按键按下后, LED灯翻转, 示例邮箱的使用

#include "ezos.h"
#include "drv_led.h"
#include "drv_key.h"

ezos_mq_t mq_key;
uint8_t   mq_buf[10];

static EZOS_TASK(task_key);
static EZOS_TASK(task_led);

void message_start(void)
{
    led_init_all();
    key_init_all();
    ezos_mq_add(&mq_key, sizeof(led_name_t), mq_buf, sizeof(mq_buf));
    ezos_add(task_key, task_key, 0, 0, 3);
    ezos_add(task_led, task_led, 0, 0, 8);
}

EZOS_TASK(task_key)
{
    led_name_t led[2] = {LED1, LED2};

    key_scan_10ms();
    switch(key_status.click)
    {
    case KEY_MASK(KEY1):
        ezos_mq_send(&mq_key, &led[0]);
        break;
    case KEY_MASK(KEY2):
        ezos_mq_send(&mq_key, &led[1]);
        break;
    }

    ezos_delay(10);
}

EZOS_TASK(task_led)
{
    ezos_t val;
    uint8_t msg;

    val = ezos_mq_receive(&mq_key, &msg, EZOS_FOREVER);
    if (val != EZOS_WAIT)
    {
        if (val == EZOS_OK) led_toggle_gradual((led_name_t)msg);
        else assert_printf("mq_key: %d\r\n", val);          // 出错, 调试信息
        ezos_delay(0);                                      // 不延时, 进入就绪态
    }
}
