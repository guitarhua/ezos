// eeprom∂¡–¥≤‚ ‘

#include "ezos.h"
#include "drv_eeprom.h"
#include "drv_systick.h"
#include "drv_uart.h"

EZOS_TASK(task_eeprom_test);

void eeprom_start(void)
{
    eeprom_init(0);
    quick_add(task_eeprom_test, 0);
}

EZOS_TASK(task_eeprom_test)
{
    uint8_t buf[200];
    uint32_t i, time;
    uint8_t rtn;

    uart_printf(COM1, "\r\n\r\n===== eeprom test =====\r\n");

    // …Ë÷√ª∫¥Ê ˝æ›
    for (i=0; i<sizeof(buf); i++) buf[i] = i+0x30;

    // –¥≤‚ ‘
    time = delay_tick_get();
    rtn  = eeprom_write(buf, 0, sizeof(buf));
    time = delay_tick_get() - time;
    uart_printf(COM1, "Eeprom write: %u\r\n", rtn);
    uart_printf(COM1, "Eeprom write %u bytes used %u ms\r\n", sizeof(buf), time);

    // «Âø’ª∫¥Ê ˝æ›
    for (i=0; i<sizeof(buf); i++) buf[i] = 0;

    // ∂¡≤‚ ‘
    time = delay_tick_get();
    rtn  = eeprom_read(buf, 0, sizeof(buf));
    time = delay_tick_get() - time;
    uart_printf(COM1, "Eeprom read: %u\r\n", rtn);
    uart_printf(COM1, "Eeprom read %u bytes used %u ms\r\n", sizeof(buf), time);

    // ≈–∂œ ˝æ› «∑Ò’˝»∑
    rtn = 0;
    for (i=0; i<sizeof(buf); i++)
    {
        if (buf[i] != i+0x30)
        {
            rtn = 1;
            break;
        }
    }
    if (rtn) uart_printf(COM1, "Eeprom cmp error\r\n\r\n");
    else     uart_printf(COM1, "Eeprom cmp ok\r\n\r\n");
}
