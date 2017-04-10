/**
  ******************************************************************************
  * @file    cpn_ring.c
  * @author  mousie
  * @version V1.0.0
  * @date    2014.5.31
  * @brief   �����ض�Ƶ�ʵ�����.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "cpn_ring.h"



/** @addtogroup Components
  * @{
  */
/** @defgroup CPN_Ring
  * @{
  */
#ifdef RING_ENABLE



/**
  ******************************************************************************
  * @addtogroup CPN_Ring_Configure
  ******************************************************************************
  * @{
  */
#include "stm32f10x.h"
#define ring_enable_int(mask)           __set_PRIMASK(mask)                     ///< ezos�궨��ȫ���жϴ򿪺���, ���������ֲ
#define ring_disable_int()              __get_PRIMASK(); __disable_irq()        ///< ezos�궨��ȫ���жϹرպ���, ���������ֲ

#define TIMER_CLK_FREQ                  72                                      // APB1��ʱ��*2, �Ҳ�����72MHz
/**
  * @}
  */



/** @defgroup CPN_Ring_Private_TypeDefine
  * @brief    ˽�����Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup CPN_Ring_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */
#define F                               500000ul
#define RING_START                      0x01
/**
  * @}
  */

/** @defgroup CPN_Ring_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */
/* -----------------------------------------------------------------------------
Comment   : �����������˵��

--------|  f1  |  f2  |  f3  |  f1  |  f2  |  f3  |  f1  |  f2  |  f3  |--------
        |<-t1->|<-t2->|<-t3->|....................|....................|
������  |<-  �������Tone  ->|<- �ڶ��β���Tone ->|<- �����β���Tone ->|  ������
        |<-         �ظ�N��Ļ�������(Tone)����һ����������          ->|

��ͼ�ɼ���������ͬ��Ƶ�����һ����������(Tone), ��������������������������һ��������.
������һ����ڳ���������, ����������ĺ�������:
BYTE1 -- ��ɻ���������Ƶ������.һ��Ҫ�ͺ�����д��Ƶ����һ��.
BYTE2 -- �����������ظ�����.
BYTE3 -- Ƶ��1. ��ʽΪ F/f, fΪƵ��, ��λHz.
BYTE4 -- Ƶ��1����ʱ��, ������λ 1ms.
BYTE5 -- Ƶ��2. ��������

uint16_t di_du_[] = {2,1,   F/4000, 103, F/420 ,108}; // ����˵��
���������� 2 ��Ƶ�� 4000 �� 420, Ƶ�ʵĳ���ʱ��Ϊ�ֱ�Ϊ1.03S �� 1.08S, �ɴ���ɻ�������
������������ 1 �κ�, ������˸�������.

Ƶ�����˵��
����Ƶ�ʻ����õ�������, ������ F��Ƶ�� �����һ��������ʱ, ���кܴ�����.
ͬһƵ����, ��� F ��ֵ���Ի�ý�С�����, ���߾������ļ���Ҳ�ɵõ��ӽ���Ŀ��Ƶ��.

���� ringFreqNum   ��¼����һ�����������ɼ���Ƶ�����.ͼ����3��Ƶ��.
���� ringFreqTimer ��¼�����ض�Ƶ�ʵĳ���ʱ��.������λ��10ms.
���� ringToneTimer ��¼���ǻ���������Ҫ�ظ�������������յ�������.
----------------------------------------------------------------------------- */
const uint16_t alpha1[]        = {2,15, F/1976, 80 ,  F/1480, 80               };
const uint16_t alpha2[]        = {2,15, F/1047, 80 ,  F/988 , 80               };
const uint16_t alpha3[]        = {2,15, F/699 , 80 ,  F/587 , 80               };
const uint16_t alpha4[]        = {2,8,  F/740 , 160,  F/587 , 160              };
const uint16_t alpha5[]        = {3,7,  F/1245, 120,  F/1047, 120, F/988, 120  };
const uint16_t alpha6[]        = {3,7,  F/831 , 120,  F/740 , 120, F/699, 120  };
const uint16_t alpha7[]        = {2,20, F/420 , 40 ,  F/480 , 70               };
const uint16_t alpha8[]        = {2,6,  F/1245, 200,  F/988 , 280              };

const uint16_t du[]            = {1,1,  F/782 , 200                            };
const uint16_t du_[]           = {1,1,  F/782 , 800                            };
const uint16_t di[]            = {1,1,  F/1480, 200                            };
const uint16_t didu[]          = {2,1,  F/1047, 120,  F/831 , 300              };
const uint16_t di_du_[]        = {2,1,  F/1397, 520,  F/1175, 520              };
const uint16_t di_di_di_[]     = {3,1,  F/1480, 600,  F/1175, 600, F/831, 600  };



static TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
volatile uint8_t ringFlag;                                                      // ʹ����Ƶ�豸�ı��
static uint16_t *ringType;                                                      // ������ָ�����
static uint16_t *ringPtr;                                                       // ������ָ��
volatile static uint16_t ringToneTimer;                                         // �����������ظ�����
volatile static uint16_t ringFreqNum;                                           // ����������Ƶ������
volatile static uint16_t ringFreqTimer;                                         // �ض�Ƶ�ʳ���ʱ�䶨ʱ��
/**
  * @}
  */

/** @defgroup CPN_Ring_Private_Function
  * @brief    ����˽�к���
  * @{
  */

/**
  * @}
  */



/** @defgroup CPN_Ring_Function
  * @brief    ����ԭ�ļ�
  * @{
  */
/**
  ******************************************************************************
  * @brief  �����ʼ��
  * @param  none
  * @retval none
  ******************************************************************************
  */
void ring_init(void)
{
    NVIC_InitTypeDef  NVIC_InitStructure;

    ctrlport_init(RING_OUT);
    RCC_APB1PeriphClockCmd(RING_TIMER_CLK, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = RING_TIMER_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /**
      * ��ʱʱ��T���㹫ʽ:
      * T = (TIM_Period+1) * (TIM_Prescaler+1) / TIMxCLK
      * ����ϵͳĬ������ʱ TIMxCLK ��ֵΪ72Mhz.
      */
    TIM_TimeBaseStructure.TIM_Prescaler = (TIMER_CLK_FREQ / 4);
    TIM_TimeBaseStructure.TIM_Period = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(RING_TIMER_PERIPHERAL, &TIM_TimeBaseStructure);
    TIM_ARRPreloadConfig(RING_TIMER_PERIPHERAL, DISABLE);                                       // �������͵�Ӱ�ӼĴ���
    TIM_ClearFlag(RING_TIMER_PERIPHERAL, TIM_FLAG_Update);                                      // ������±�־λ
    TIM_ITConfig(RING_TIMER_PERIPHERAL, TIM_IT_Update, ENABLE);                                 // ʹ������ж�

    ringFlag = 0x00;
    ringFreqTimer = 0x00;
}

/**
  ******************************************************************************
  * @brief  ��������
  * @param  none
  * @retval none
  ******************************************************************************
  */
void ring_deinit(void)
{
    NVIC_InitTypeDef  NVIC_InitStructure;

    ctrlport_off(RING_OUT);
    TIM_Cmd(RING_TIMER_PERIPHERAL, DISABLE);
    TIM_DeInit(RING_TIMER_PERIPHERAL);

    RCC_APB1PeriphClockCmd(RING_TIMER_CLK, DISABLE);

    NVIC_InitStructure.NVIC_IRQChannel = RING_TIMER_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
  ******************************************************************************
  * @brief  ǿ�ƿ�ʼ����
  * @param  pRing, ������ָ��
  * @retval none
  ******************************************************************************
  */
void ring_start(const uint16_t *pRing)
{
    RCC_APB1PeriphClockCmd(RING_TIMER_CLK, ENABLE);
    TIM_Cmd(RING_TIMER_PERIPHERAL, DISABLE);
    ringFlag     |= RING_START;                                                 // �����������ʼ
    ringFreqTimer = 10;                                                         // 10ms��ʼ����
    ringPtr       = (uint16_t *)pRing;
    ringType      = (uint16_t *)pRing;
    ringFreqNum   = *ringType++;                                                // ����������Ƶ����
    ringToneTimer = *ringType++;                                                // �����������ż�����
}

/**
  ******************************************************************************
  * @brief  ���Կ�ʼ����, ��Ӱ��ԭ������
  * @param  pRing, ������ָ��
  * @retval none
  ******************************************************************************
  */
void ring_try_start(const uint16_t *pRing)
{
    if (ringFlag & RING_START) return;
    ring_start(pRing);
}

/**
  ******************************************************************************
  * @brief  ��������
  * @param  none
  * @retval none
  ******************************************************************************
  */
void ring_stop(void)
{
    uint32_t mask;

    mask = ring_disable_int();
    TIM_Cmd(RING_TIMER_PERIPHERAL, DISABLE);
    RCC_APB1PeriphClockCmd(RING_TIMER_CLK, DISABLE);
    ringFreqTimer = 0;
    ringFlag = 0;
    ring_enable_int(mask);
    ctrlport_off(RING_OUT);
}

/**
  ******************************************************************************
  * @brief  ��ȡ����״̬
  * @param  none
  * @retval 0, ֹͣ״̬. 1, ����״̬
  ******************************************************************************
  */
uint8_t ring_get_status(void)
{
    return ringFlag;
}

/**
  ******************************************************************************
  * @brief  ����ɨ�躯��, ��������жϺ�����
  * @param  none
  * @retval none
  ******************************************************************************
  */
void ring_scan_1ms(void)
{
    if (ringFreqTimer)
    {
        if (--ringFreqTimer == 0)
        {
            if ((ringFlag & RING_START) && ringToneTimer)                       // ʹ�����岢������δ����
            {
                TIM_TimeBaseStructure.TIM_Period = *ringType++;                 // ȡ��ǰ����Ƶ��
                if (TIM_TimeBaseStructure.TIM_Period)
                {
                    RING_TIMER_PERIPHERAL->ARR = TIM_TimeBaseStructure.TIM_Period ;
                    TIM_Cmd(RING_TIMER_PERIPHERAL, ENABLE);
                }

                ringFreqTimer = *ringType++;                                    // ��¼����Ƶ��ά��ʱ��
                if (--ringFreqNum == 0)                                         // �����һ��Ƶ��
                {
                    ringFreqNum = *ringPtr;
                    ringType    = ringPtr + 2;                                  // ���¿�ʼһ����������
                    ringToneTimer--;                                            // ���������ظ������ݼ�
                }
            }
            else                                                                // ���屻��ֹ���������
            {
                ring_stop();                                                    // ֹͣ����
            }
        }
    }
}

/**
  ******************************************************************************
  * @brief  ��ʱ���жϺ���
  * @param  none
  * @retval none
  ******************************************************************************
  */
void RING_TIMER_IRQ_HANDLER(void)
{
    if(TIM_GetITStatus(RING_TIMER_PERIPHERAL, TIM_IT_Update) != RESET )
    {
        TIM_ClearITPendingBit(RING_TIMER_PERIPHERAL, TIM_IT_Update);
        ctrlport_toggle(RING_OUT);
    }
}
/**
  * @}
  */



#endif
/**
  * @}
  */
/**
  * @}
  */

/* END OF FILE ---------------------------------------------------------------*/
