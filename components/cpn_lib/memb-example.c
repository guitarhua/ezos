// 内存块管理范例

#include "memb.h"

typedef struct example_memb_struct{
    int num1;
    int num2;
} example_memb_t;


// 设置memb的数据结构, 名称, 和可用数量.
// 可理解为在内存开辟了如下一块区域供分配使用,
// example_memb_t example_memb[10];
MEMB(example_memb_t, example_memb, 10);



void example_memb_fun(void)
{
    example_memb_t *m;

    memb_init(&example_memb);           // 初始化内存块

    m = memb_alloc(&example_mem);       // 申请内存块
    if (m == NULL)                      // 申请失败
    {
        printf("example_mem alloc failed\n");
    }
    else                                // 申请成功
    {
        printf("example_mem alloc ok\n");
        memb_free(&example_mem, m);     // 释放内存块
    }
}
