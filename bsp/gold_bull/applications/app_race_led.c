// ����Ʒ���, ʾ������״̬����ʹ��

#include "ezos.h"
#include "drv_led.h"

EZOS_TASK(task_race_led);

void race_led_start(void)
{
    led_init_all();
    quick_add(task_race_led, 10);
}

EZOS_TASK(task_race_led)
{
    switch (*state)
    {
    case 0:                                                 // ������Ӻ�ĳ�ʼ״̬
        led_off(LED1);                                      // LED��ʼ״̬ȫ��
        led_off(LED2);
        led_off(LED3);
        led_off(LED4);
        *state = 1;
        ezos_delay(1000);
        break;
    case 1:
        led_toggle_gradual(LED1);
        *state = 2;
        ezos_delay(1000);
        break;
    case 2:
        led_toggle_gradual(LED2);
        *state = 3;
        ezos_delay(1000);
        break;
    case 3:
        led_toggle_gradual(LED3);
        *state = 4;
        ezos_delay(1000);
        break;
    default:
        led_toggle_gradual(LED4);
        *state = 1;
        ezos_delay(1000);
        break;
    }
}
