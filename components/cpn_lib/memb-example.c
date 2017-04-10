// �ڴ�������

#include "memb.h"

typedef struct example_memb_struct{
    int num1;
    int num2;
} example_memb_t;


// ����memb�����ݽṹ, ����, �Ϳ�������.
// �����Ϊ���ڴ濪��������һ�����򹩷���ʹ��,
// example_memb_t example_memb[10];
MEMB(example_memb_t, example_memb, 10);



void example_memb_fun(void)
{
    example_memb_t *m;

    memb_init(&example_memb);           // ��ʼ���ڴ��

    m = memb_alloc(&example_mem);       // �����ڴ��
    if (m == NULL)                      // ����ʧ��
    {
        printf("example_mem alloc failed\n");
    }
    else                                // ����ɹ�
    {
        printf("example_mem alloc ok\n");
        memb_free(&example_mem, m);     // �ͷ��ڴ��
    }
}
