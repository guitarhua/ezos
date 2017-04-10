// sd卡读写测试

#include <stdlib.h>
#include "user_types.h"
#include "ezos.h"
#include "drv_sdcard.h"
#include "ff.h"
#include "drv_systick.h"
#include "drv_uart.h"

EZOS_TASK(task_fatfs_test);

void fatfs_start(void)
{
    sdcard_port_init();
    quick_add(task_fatfs_test, 2);
}

// fatfs读写测试
EZOS_TASK(task_fatfs_test)
{
    #define FLOOPS 100
    #define BUF_SIZE 1024
    FATFS    fs;
    FIL      fsrc, fdst;
    FRESULT  ff_flag;
    uint32_t i, num, time;
    uint8_t* buf;

    buf = (uint8_t *)malloc(BUF_SIZE);
    if (buf != NULL)
    {
        uart_printf(COM1, "\r\n\r\n===== fatfs test =====\r\n");

        // 初始化SD卡
        sdcard_init();
        ff_flag = f_mount(0,&fs);
        uart_printf(COM1, "f_mount: %u\r\n", ff_flag);

        // 格式化测试
        uart_printf(COM1, "start format sd, please wait...\r\n");
        ff_flag = f_mkfs( 0, 1, 4096 );                                         // 格式化SD卡, 1表示不需要引导扇区。4096是8个扇区
        uart_printf(COM1, "f_mkfs: %u\r\n", ff_flag);

        // 创建文件夹
        ff_flag = f_mkdir("0:/hello");                                          // 创建一个文件夹hello
        uart_printf(COM1, "f_mkdir: %u\r\n\r\n", ff_flag);

        // 创建文件, 写测试
        ff_flag = f_open(&fdst, "0:/hello/test.bin", FA_CREATE_ALWAYS | FA_WRITE);
        uart_printf(COM1, "f_open: %u\r\n", ff_flag);

        for (i=0; i<BUF_SIZE; i++) buf[i] = i;
        time = delay_tick_get();
        for (i=0; i<FLOOPS; i++) f_write(&fdst, buf ,BUF_SIZE, &num);           // 写test.bin文件
        time = delay_tick_get() - time;
        uart_printf(COM1, "fatfs write 100K bytes used %u ms\r\n", time);

        ff_flag = f_close(&fdst);
        uart_printf(COM1, "f_close: %u\r\n\r\n", ff_flag);

        // 读测试
        ff_flag = f_open(&fsrc, "0:/hello/test.bin", FA_OPEN_EXISTING | FA_READ);
        uart_printf(COM1, "f_open: %u\r\n", ff_flag);

        time = delay_tick_get();
        for (i=0; i<FLOOPS; i++) f_read(&fsrc, buf ,BUF_SIZE, &num);            // 读test.bin文件
        time = delay_tick_get() - time;
        uart_printf(COM1, "fatfs read 100K bytes used %u ms\r\n", time);

        ff_flag = f_close(&fsrc);
        uart_printf(COM1, "f_close: %u\r\n\r\n", ff_flag);
    }
    free(buf);
}
