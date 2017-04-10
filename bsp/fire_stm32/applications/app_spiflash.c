// spiflash∂¡–¥≤‚ ‘

#include <stdlib.h>
#include "user_types.h"
#include "ezos.h"
#include "drv_spiflash.h"
#include "drv_systick.h"
#include "drv_uart.h"

static EZOS_TASK(task_spiflash_test);

void spiflash_start(void)
{
    spiflash_init();
    quick_add(task_spiflash_test, 4);
}

// spiflash∂¡–¥≤‚ ‘
EZOS_TASK(task_spiflash_test)
{
    #define SFLOOPS     100
    #define BUF_SIZE    1024
    uint32_t i, time;
    uint8_t rtn = 0;
    uint8_t *buf;

    buf = (uint8_t *)malloc(BUF_SIZE);
    if (buf != NULL)
    {
        uart_printf(COM1, "\r\n\r\n===== spiflash test =====\r\n");

        // …Ë÷√ª∫¥Ê ˝æ›
        for (i=0; i<BUF_SIZE; i++) buf[i] = i;

        // –¥≤‚ ‘
        time = delay_tick_get();
        for (i=0; i<SFLOOPS; i++)
            spiflash_write_buf(buf, BUF_SIZE*i, BUF_SIZE);
        spiflash_write_buf2flash();
        time = delay_tick_get() - time;
        uart_printf(COM1, "spiflash write 100K bytes used %u ms\r\n", time);

        // ∂¡≤‚ ‘
        time = delay_tick_get();
        spiflash_read_start(0);
        for (i=0; i<SFLOOPS; i++)
            spiflash_read_bytes(buf, BUF_SIZE);
        spiflash_read_end();
        time = delay_tick_get() - time;
        uart_printf(COM1, "spiflash read 100K bytes used %u ms\r\n", time);

        // ≈–∂œ ˝æ› «∑Ò’˝»∑
        for (i=0; i<SFLOOPS; i++)
        {
            for (time=0; time<BUF_SIZE; time++) buf[time] = 0;
            spiflash_read(buf, BUF_SIZE*i, BUF_SIZE);
            for (time=0; time<BUF_SIZE; time++)
            {
                if (buf[time] != (time & 0xFF))
                {
                    rtn = 1;
                    break;
                }
            }
            if (rtn) break;
        }

        if (rtn) uart_printf(COM1, "spiflash cmp error\r\n\r\n");
        else     uart_printf(COM1, "spiflash cmp ok\r\n\r\n");
    }
    free(buf);
}
