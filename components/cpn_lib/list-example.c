// 线性链表范例

#include "list.h"

typedef struct example_list_struct{
    struct example_list_struct *next;   // 数据结构中, 必须有 *next 指针用于生成线性链表
    int number;
} example_list_t;



LIST(example_list);                     // 设置线性链表名称

static example_list_t element1;         // 线性链表元素1
static example_list_t element2;         // 线性链表元素2



void example_list_fun(void)
{
    example_list_t *l;

    list_init(example_list);            // 初始化线性链表 example_list

    element1.number = 1;
    list_add(example_list, &element1);  // element1 加入 example_list

    element2.number = 2;
    list_add(example_list, &element2);  // element2 加入 example_list

    for (l = list_head(example_list); l != 0; l = list_item_next(l))
    {
        printf("List element number %d\n", l->number);
    }
}
