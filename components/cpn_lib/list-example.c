// ����������

#include "list.h"

typedef struct example_list_struct{
    struct example_list_struct *next;   // ���ݽṹ��, ������ *next ָ������������������
    int number;
} example_list_t;



LIST(example_list);                     // ����������������

static example_list_t element1;         // ��������Ԫ��1
static example_list_t element2;         // ��������Ԫ��2



void example_list_fun(void)
{
    example_list_t *l;

    list_init(example_list);            // ��ʼ���������� example_list

    element1.number = 1;
    list_add(example_list, &element1);  // element1 ���� example_list

    element2.number = 2;
    list_add(example_list, &element2);  // element2 ���� example_list

    for (l = list_head(example_list); l != 0; l = list_item_next(l))
    {
        printf("List element number %d\n", l->number);
    }
}
