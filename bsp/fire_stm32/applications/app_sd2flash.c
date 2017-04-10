// 从SD卡读取文件烧录到FLASH中

#include <stdlib.h>
#include "user_types.h"
#include "ezos.h"
#include "drv_spiflash.h"
#include "drv_sdcard.h"
#include "ff.h"
#include "drv_uart.h"

#define SD_FILE                         "0:/test.bin"                           // SD卡烧录文件名
#define FLASH_ADDRESS                   ((uint32_t)0x00000000)                  // FLASH目标地址

static EZOS_TASK(task_sd2flash);

void sd2flash_start(void)
{
    spiflash_init();
    sdcard_port_init();
    quick_add(task_sd2flash, 0);
}

// fatfs读写测试
EZOS_TASK(task_sd2flash)
{
    FATFS    fs;
    FIL      fsrc;
    FRESULT  ff_flag;
    uint32_t num, addr;
    uint8_t  buf[512];

    uart_printf(COM1, "\r\n\r\n===== sd2flash start =====\r\n");

    // 初始化SD卡
    sdcard_init();
    ff_flag = f_mount(0,&fs);

    // 读原文件
    ff_flag = f_open(&fsrc, SD_FILE, FA_OPEN_EXISTING | FA_READ);
    uart_printf(COM1, "open %s, %u\r\n", SD_FILE, ff_flag);

    if (!ff_flag)
    {
        addr = FLASH_ADDRESS;
        while (num)
        {
            if (f_read(&fsrc, buf, sizeof(buf), &num)) break;
            spiflash_write_buf(buf, addr, num);
            addr += num;
        }
        spiflash_write_buf2flash();
    }

    f_close(&fsrc);
    uart_printf(COM1, "sd2flash end!!! \r\n");
}
