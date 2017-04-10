/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"
#include "drv_rtc_time.h"
#include "drv_sdcard.h"

/* Definitions of physical drive number for each media */
#define SD      0
#define USB     1


/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
    BYTE pdrv               /* Physical drive nmuber (0..) */
)
{
    DSTATUS stat;

    switch (pdrv)
    {
    case SD:
        if (sdcard_init() == 0)
            stat = 0;
        else
            stat = STA_NOINIT;
        break;

    case USB:
    default:
        stat = STA_NOINIT;
        break;
    }

    return stat;
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
    BYTE pdrv       /* Physical drive nmuber (0..) */
)
{
    DSTATUS stat;

    switch (pdrv)
    {
    case SD:
        if (sdcard_present())
            stat = 0;
        else
            stat = STA_NOINIT;
        break;

    case USB:
    default:
        stat = STA_NOINIT;
        break;
    }

    return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
    BYTE pdrv,      /* Physical drive nmuber (0..) */
    BYTE *buff,     /* Data buffer to store read data */
    DWORD sector,   /* Sector address (LBA) */
    BYTE count      /* Number of sectors to read (1..128) */
)
{
    DRESULT res;

    switch (pdrv) {
    case SD:
        if (sdcard_read_blocks(buff, sector, count) == 0)
            res = RES_OK;
        else
            res = RES_ERROR;
        break;

    case USB:
    default:
        res = RES_PARERR;
        break;
    }

    return res;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
    BYTE pdrv,          /* Physical drive nmuber (0..) */
    const BYTE *buff,   /* Data to be written */
    DWORD sector,       /* Sector address (LBA) */
    BYTE count          /* Number of sectors to write (1..128) */
)
{
    DRESULT res;

    switch (pdrv) {
    case SD:
        if (sdcard_write_blocks(buff, sector, count) == 0)
            res = RES_OK;
        else
            res = RES_ERROR;
        break;

    case USB:
    default:
        res = RES_PARERR;
        break;
    }

    return res;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
    BYTE pdrv,      /* Physical drive nmuber (0..) */
    BYTE cmd,       /* Control code */
    void *buff      /* Buffer to send/receive control data */
)
{
    DRESULT res;
    sdcard_info_t sd;

    switch (pdrv) {
    case SD:
        switch(cmd)
        {
        case CTRL_SYNC:
            res = RES_OK;
            break;
        case GET_BLOCK_SIZE:
            *(WORD*)buff = 512;
            res = RES_OK;
          break;
        case GET_SECTOR_COUNT:
            if (sdcard_get_info(&sd) == 0)
            {
                *(DWORD*)buff = sd.capacity >> 9;                               // sd.capacity / 512, 容量除以块大小就是块的数量
                res = RES_OK;
            }
            else
            {
                res = RES_ERROR;
            }
            break;
        default:
            res = RES_PARERR;
            break;
        }
        break;

    case USB:
    default:
        res = RES_PARERR;
        break;
    }

    return res;
}
#endif

/*-----------------------------------------------------------------------*/
/* User defined function to give a current time to fatfs module          */
/* 31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) */
/* 15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */
DWORD get_fattime (void)
{
    struct tm time;

    time = time_get_calendar();
    time.tm_year -= 1980;                                                       // ff文件系统年份起始值1980年
    time.tm_sec >>= 1;                                                          // 秒除以2, 已符合ff文件系统时间格式要求

    return(((DWORD)time.tm_year << 25) |                                        // 时间格式转换
           ((DWORD)time.tm_mon  << 21) |
           ((DWORD)time.tm_mday << 16) |
           ((DWORD)time.tm_hour << 11) |
           ((DWORD)time.tm_min  << 5 ) |
           ((DWORD)time.tm_sec)        );
}
