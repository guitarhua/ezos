// 蜂鸣器

#include "ezos.h"
#include "drv_ctrlport.h"

static EZOS_TASK(task_bell_1s);

void bell_start(void)
{
    quick_add(task_bell_1s, 10);
}

EZOS_TASK(task_bell_1s)
{
    switch (*state)
    {
    case 0:                                                 // 任务添加后的初始状态
        ctrlport_init(BELL);
        ezos_delay(50);
        *state = 1;
        break;
    case 200:
        break;
    default:
        ctrlport_toggle(BELL);
        ezos_delay(5);
        ++*state;
        break;
    }
}
