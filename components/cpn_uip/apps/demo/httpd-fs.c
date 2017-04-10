// web 服务器 页面文件数据处理

#include "httpd.h"
#include "httpd-fsdata.c"



#if HTTPD_FS_STATISTICS
    static uint16_t count[HTTPD_FS_NUMFILES];
#endif



void httpd_fs_init(void)
{
#if HTTPD_FS_STATISTICS
    uint16_t i;
    for(i = 0; i < HTTPD_FS_NUMFILES; i++)
    {
        count[i] = 0;
    }
#endif /* HTTPD_FS_STATISTICS */
}

static uint8_t httpd_fs_strcmp(const char *str1, const char *str2)
{
    uint8_t i;

    for (i=0; ; i++)
    {
        if ((str2[i] == 0) || (str1[i] == '\r') || (str1[i] == '\n'))
            return 0;
        if (str1[i] != str2[i])
            return 1;
    }
}

uint8_t httpd_fs_open(const char *name, struct httpd_fs_file *file)
{
#if HTTPD_FS_STATISTICS
    uint16_t i = 0;
#endif
    struct httpd_fsdata_file_noconst *f;

    // 遍历所有的页面数据, 方便校验是否存在该页面
    for(f = (struct httpd_fsdata_file_noconst *)HTTPD_FS_ROOT;
        f != UIP_NULL;
        f = (struct httpd_fsdata_file_noconst *)f->next)
    {
        if(httpd_fs_strcmp(name, f->name) == 0)
        {
            file->data = f->data;
            file->len = f->len;
#if HTTPD_FS_STATISTICS
            ++count[i];
#endif
            return 1;
        }
#if HTTPD_FS_STATISTICS
        ++i;
#endif
    }
    return 0;
}

#if HTTPD_FS_STATISTICS
uint16_t httpd_fs_count(char *name)
{
    struct httpd_fsdata_file_noconst *f;
    uint16_t i;

    i = 0;
    for(f = (struct httpd_fsdata_file_noconst *)HTTPD_FS_ROOT;
        f != UIP_NULL;
        f = (struct httpd_fsdata_file_noconst *)f->next)
    {

        if(httpd_fs_strcmp(name, f->name) == 0)
        {
            return count[i];
        }
        ++i;
    }
    return 0;
}
#endif
