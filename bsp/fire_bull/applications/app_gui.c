// Һ��������

#include "ezos.h"
#include "lcd_main.h"
#include "touch_main.h"
#include "GUI.h"

// ע��: �˰汾����������ȷ������, �ɸ��ݴ˼ܹ�������ֲ. ����������, ��������ʾ��.
// �� MainTask Ϊ��ѭ������, ���´�����10ms��ʱ�����޷�����, ���޷�ʹ�ô���������!!!
// ��˴�����10ms��ʱɨ������ֻ�����ж���ʵ��, �� GUI_TOUCH_Exec() ����. �������޸Ĳ���.

extern void lcd_driver_test(void);
void gui_start(void)
{
    lcd_init();
    touch_init();

    // LCD��������
    // lcd_test_driver();

    // emWIN��ʾ, Ϊ��ѭ������! ������ʾ������ GUIDemo.h
    MainTask();
}
