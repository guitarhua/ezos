// 液晶屏测试

#include "ezos.h"
#include "lcd_main.h"
#include "touch_main.h"
#include "GUI.h"

// 注意: 此版本触摸屏功能确保可用, 可根据此架构进行移植. 但功能有限, 仅可做演示用.
// 但 MainTask 为死循环函数, 导致触摸屏10ms定时任务无法运行, 故无法使用触摸屏功能!!!
// 因此触摸屏10ms定时扫描任务只能在中断中实现, 即 GUI_TOUCH_Exec() 函数. 可自行修改测试.

extern void lcd_driver_test(void);
void gui_start(void)
{
    lcd_init();
    touch_init();

    // LCD驱动测试
    // lcd_test_driver();

    // emWIN演示, 为死循环任务! 增减演示范例见 GUIDemo.h
    MainTask();
}
