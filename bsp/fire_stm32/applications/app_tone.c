// 使用蜂鸣器播放曲调

#include "drv_timer.h"
#include "cpn_tone.h"

extern const unsigned char tone_test_girl[];
extern const unsigned char tone_test_same[];
extern const unsigned char tone_test_butterfly[];

void tone_start(void)
{
    timer_init(TIMER_10US, 36, 20);
    tone_play((uint8_t *)tone_test_same);
}
