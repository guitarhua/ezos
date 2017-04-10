// sdflash��д����

#include <stdlib.h>
#include "user_types.h"
#include "ezos.h"
#include "drv_sdcard.h"
#include "drv_systick.h"
#include "drv_uart.h"

static EZOS_TASK(task_sd_block_test);
static EZOS_TASK(task_sd_blocks_test);

void sd_start(void)
{
    sdcard_port_init();
    quick_add(task_sd_block_test, 0);
    quick_add(task_sd_blocks_test, 1);
}

// SD����������д����
EZOS_TASK(task_sd_block_test)
{
    #define SLOOP   200
    sdcard_info_t info;
    uint32_t i, time;
    uint8_t rtn;
    uint8_t *buf;

    buf = (uint8_t *)malloc(512);
    if (buf != NULL)
    {
        uart_printf(COM1, "\r\n\r\n===== sd single block test =====\r\n");
        // ��ʼ��SD��
        rtn = sdcard_init();
        uart_printf(COM1, "sd init: %u\r\n", rtn);

        // ��ȡSD����Ϣ
        rtn = sdcard_get_info(&info);
        uart_printf(COM1, "sd_get_info: %u\r\n", rtn);
        uart_printf(COM1, "type: %u\r\n", info.type);
        uart_printf(COM1, "capacityH: %u\r\n", (uint32_t)(info.capacity>>32));
        uart_printf(COM1, "capacityL: %u\r\n\r\n", (uint32_t)info.capacity);

        // ���û���������
        for (i=0; i<512; i++) buf[i] = i;

        // ������д����
        time = delay_tick_get();
        rtn  = sdcard_write_blocks(buf, 0, 1);
        time = delay_tick_get() - time;
        uart_printf(COM1, "sd block write: %u\r\n", rtn);
        uart_printf(COM1, "sd write 1 block used %u ms\r\n", time);

        // ��ջ���������
        for (i=0; i<512; i++) buf[i] = 0;

        // ������������
        time = delay_tick_get();
        rtn  = sdcard_read_blocks(buf, 0, 1);
        time = delay_tick_get() - time;
        uart_printf(COM1, "sd block read: %u\r\n", rtn);
        uart_printf(COM1, "sd read 1 block used %u ms\r\n", time);

         // �ж������Ƿ���ȷ
        rtn = 0;
        for (i=0; i<512; i++)
        {
            if (buf[i] != (i&0xFF))
            {
                rtn = 1;
                break;
            }
        }
        if (rtn) uart_printf(COM1, "sd cmp 1 block error\r\n\r\n");
        else     uart_printf(COM1, "sd cmp 1 block ok\r\n\r\n");

        // ���û���������
        for (i=0; i<512; i++) buf[i] = i;

        // 100K����д�ٶȲ���
        time = delay_tick_get();
        for (i=0; i<SLOOP; i++)
        {
            rtn = sdcard_write_blocks(buf, i, 1);
            if (rtn) break;
        }
        time = delay_tick_get() - time;
        uart_printf(COM1, "sd 100K bytes write: %u\r\n", rtn);
        uart_printf(COM1, "sd write 100K bytes used %u ms\r\n", time);

        // 100K���ݶ��ٶȲ���
        time = delay_tick_get();
        for (i=0; i<SLOOP; i++)
        {
            sdcard_read_blocks(buf, i, 1);
        }
        time = delay_tick_get() - time;
        uart_printf(COM1, "sd read 100K bytes used %u ms\r\n", time);

        // ��ջ���������
        for (i=0; i<512; i++) buf[i] = 0;

        // �ж������Ƿ���ȷ
        for (i=0; i<SLOOP; i++)
        {
            rtn = sdcard_read_blocks(buf, i, 1);
            for (time=0; time<512; time++)
            {
                if (buf[time] != (time&0xFF))
                {
                    rtn = 1;
                    break;
                }
            }
            if (rtn) break;
        }
        if (rtn) uart_printf(COM1, "sd single cmp 100K bytes error\r\n\r\n");
        else     uart_printf(COM1, "sd single cmp 100K bytes ok\r\n\r\n");
    }
    free(buf);
}

// SD����������д����
EZOS_TASK(task_sd_blocks_test)
{
    #define BLOCK_NUM   2
    #define BUF_SIZE    512 * BLOCK_NUM
    #define MLOOP       100

    sdcard_info_t info;
    uint32_t i, time;
    uint8_t rtn;
    uint8_t *buf;

    buf = (uint8_t *)malloc(BUF_SIZE);
    if (buf != NULL)
    {
        uart_printf(COM1, "\r\n\r\n===== sd multi blocks test =====\r\n");
        // ��ʼ��SD��
        rtn = sdcard_init();
        uart_printf(COM1, "sd init: %u\r\n", rtn);

        // ��ȡSD����Ϣ
        rtn = sdcard_get_info(&info);
        uart_printf(COM1, "sd_get_info: %u\r\n", rtn);
        uart_printf(COM1, "type: %u\r\n", info.type);
        uart_printf(COM1, "capacityH: %u\r\n", (uint32_t)(info.capacity>>32));
        uart_printf(COM1, "capacityL: %u\r\n\r\n", (uint32_t)info.capacity);

        // ���û���������
        for (i=0; i<BUF_SIZE; i++) buf[i] = i;

        // ������д����
        time = delay_tick_get();
        rtn  = sdcard_write_blocks(buf, 0, BLOCK_NUM);
        time = delay_tick_get() - time;
        uart_printf(COM1, "sd multi blocks write: %u\r\n", rtn);
        uart_printf(COM1, "sd write %u blocks used %u ms\r\n", BLOCK_NUM, time);

        // ��ջ���������
        for (i=0; i<BUF_SIZE; i++) buf[i] = 0;

        // ������������
        time = delay_tick_get();
        rtn = sdcard_read_blocks(buf, 0, BLOCK_NUM);
        time = delay_tick_get() - time;
        uart_printf(COM1, "sd multi blocks read: %u\r\n", rtn);
        uart_printf(COM1, "sd read %u blocks used %u ms\r\n", BLOCK_NUM, time);

         // �ж������Ƿ���ȷ
        rtn = 0;
        for (i=0; i<BUF_SIZE; i++)
        {
            if (buf[i] != (i & 0xFF))
            {
                rtn = 1;
                break;
            }
        }
        if (rtn) uart_printf(COM1, "sd cmp multi blocks error\r\n\r\n");
        else     uart_printf(COM1, "sd cmp multi blocks ok\r\n\r\n");

        // ���û���������
        for (i=0; i<BUF_SIZE; i++) buf[i] = i;

        // 100K����д�ٶȲ���
        time = delay_tick_get();
        for (i=0; i<MLOOP; i++)
        {
            rtn = sdcard_write_blocks(buf, i, BLOCK_NUM);
            if (rtn) break;
        }
        time = delay_tick_get() - time;
        uart_printf(COM1, "sd 100K bytes write: %u\r\n", rtn);
        uart_printf(COM1, "sd write 100K bytes used %u ms\r\n", time);

        // 100K���ݶ��ٶȲ���
        time = delay_tick_get();
        for (i=0; i<MLOOP; i++) sdcard_read_blocks(buf, i, BLOCK_NUM);
        time = delay_tick_get() - time;
        uart_printf(COM1, "sd read 100K bytes used %u ms\r\n", time);

        // ��ջ���������
        for (i=0; i<BUF_SIZE; i++) buf[i] = 0;

        // �ж������Ƿ���ȷ
        for (i=0; i<MLOOP; i++)
        {
            rtn = sdcard_read_blocks(buf, i, BLOCK_NUM);
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
        if (rtn) uart_printf(COM1, "sd multi cmp 100K bytes error\r\n\r\n");
        else     uart_printf(COM1, "sd multi cmp 100K bytes ok\r\n\r\n");
    }
    free(buf);
}
