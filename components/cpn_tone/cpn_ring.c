/**
  ******************************************************************************
  * @file    cpn_ring.c
  * @author  mousie
  * @version V1.0.0
  * @date    2014.5.31
  * @brief   播放特定频率的铃声.
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
#define ring_enable_int(mask)           __set_PRIMASK(mask)                     ///< ezos宏定义全局中断打开函数, 方便代码移植
#define ring_disable_int()              __get_PRIMASK(); __disable_irq()        ///< ezos宏定义全局中断关闭函数, 方便代码移植

#define TIMER_CLK_FREQ                  72                                      // APB1的时钟*2, 且不超过72MHz
/**
  * @}
  */



/** @defgroup CPN_Ring_Private_TypeDefine
  * @brief    私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup CPN_Ring_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */
#define F                               500000ul
#define RING_START                      0x01
/**
  * @}
  */

/** @defgroup CPN_Ring_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */
/* -----------------------------------------------------------------------------
Comment   : 振铃音的组成说明

--------|  f1  |  f2  |  f3  |  f1  |  f2  |  f3  |  f1  |  f2  |  f3  |--------
        |<-t1->|<-t2->|<-t3->|....................|....................|
无声音  |<-  三者组成Tone  ->|<- 第二次播放Tone ->|<- 第三次播放Tone ->|  无声音
        |<-         重复N遍的基本音调(Tone)构成一组振铃音乐          ->|

由图可见，几个不同的频率组成一个基本音调(Tone), 反复播放这个基本音调即能组成一个振铃音.
振铃音一般存在程序区即可, 振铃音数组的含义如下:
BYTE1 -- 组成基本音调的频率数量.一定要和后面填写的频率数一致.
BYTE2 -- 基本音调的重复次数.
BYTE3 -- 频率1. 格式为 F/f, f为频率, 单位Hz.
BYTE4 -- 频率1持续时间, 基本单位 1ms.
BYTE5 -- 频率2. 以下类推

uint16_t di_du_[] = {2,1,   F/4000, 103, F/420 ,108}; // 范例说明
该振铃音有 2 个频率 4000 和 420, 频率的持续时间为分别为1.03S 和 1.08S, 由此组成基本音调
基本音调播放 1 次后, 就组成了该振铃音.

频率误差说明
由于频率换算用的是整除, 所以在 F与频率 仅相差一个数量级时, 会有很大的误差.
同一频率下, 提高 F 的值可以获得较小的误差, 或者经过精心计算也可得到接近的目标频率.

参数 ringFreqNum   记录的是一个基本音调由几个频率组成.图中是3个频率.
参数 ringFreqTimer 记录的是特定频率的持续时间.基本单位是10ms.
参数 ringToneTimer 记录的是基本音调需要重复几次来组成最终的振铃音.
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
volatile uint8_t ringFlag;                                                      // 使用音频设备的标记
static uint16_t *ringType;                                                      // 振铃音指针变量
static uint16_t *ringPtr;                                                       // 振铃音指针
volatile static uint16_t ringToneTimer;                                         // 基本音调的重复次数
volatile static uint16_t ringFreqNum;                                           // 基本音调的频率数量
volatile static uint16_t ringFreqTimer;                                         // 特定频率持续时间定时器
/**
  * @}
  */

/** @defgroup CPN_Ring_Private_Function
  * @brief    定义私有函数
  * @{
  */

/**
  * @}
  */



/** @defgroup CPN_Ring_Function
  * @brief    函数原文件
  * @{
  */
/**
  ******************************************************************************
  * @brief  振铃初始化
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
      * 定时时间T计算公式:
      * T = (TIM_Period+1) * (TIM_Prescaler+1) / TIMxCLK
      * 采用系统默认配置时 TIMxCLK 的值为72Mhz.
      */
    TIM_TimeBaseStructure.TIM_Prescaler = (TIMER_CLK_FREQ / 4);
    TIM_TimeBaseStructure.TIM_Period = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(RING_TIMER_PERIPHERAL, &TIM_TimeBaseStructure);
    TIM_ARRPreloadConfig(RING_TIMER_PERIPHERAL, DISABLE);                                       // 立即传送到影子寄存器
    TIM_ClearFlag(RING_TIMER_PERIPHERAL, TIM_FLAG_Update);                                      // 清除更新标志位
    TIM_ITConfig(RING_TIMER_PERIPHERAL, TIM_IT_Update, ENABLE);                                 // 使能溢出中断

    ringFlag = 0x00;
    ringFreqTimer = 0x00;
}

/**
  ******************************************************************************
  * @brief  禁用振铃
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
  * @brief  强制开始振铃
  * @param  pRing, 振铃音指针
  * @retval none
  ******************************************************************************
  */
void ring_start(const uint16_t *pRing)
{
    RCC_APB1PeriphClockCmd(RING_TIMER_CLK, ENABLE);
    TIM_Cmd(RING_TIMER_PERIPHERAL, DISABLE);
    ringFlag     |= RING_START;                                                 // 标记振铃音开始
    ringFreqTimer = 10;                                                         // 10ms后开始振铃
    ringPtr       = (uint16_t *)pRing;
    ringType      = (uint16_t *)pRing;
    ringFreqNum   = *ringType++;                                                // 基本音调的频率数
    ringToneTimer = *ringType++;                                                // 基本音调播放计数器
}

/**
  ******************************************************************************
  * @brief  尝试开始振铃, 不影响原有铃声
  * @param  pRing, 振铃音指针
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
  * @brief  结束振铃
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
  * @brief  获取振铃状态
  * @param  none
  * @retval 0, 停止状态. 1, 播放状态
  ******************************************************************************
  */
uint8_t ring_get_status(void)
{
    return ringFlag;
}

/**
  ******************************************************************************
  * @brief  振铃扫描函数, 必须放在中断函数中
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
            if ((ringFlag & RING_START) && ringToneTimer)                       // 使能振铃并且振铃未结束
            {
                TIM_TimeBaseStructure.TIM_Period = *ringType++;                 // 取当前振铃频率
                if (TIM_TimeBaseStructure.TIM_Period)
                {
                    RING_TIMER_PERIPHERAL->ARR = TIM_TimeBaseStructure.TIM_Period ;
                    TIM_Cmd(RING_TIMER_PERIPHERAL, ENABLE);
                }

                ringFreqTimer = *ringType++;                                    // 记录振铃频率维持时间
                if (--ringFreqNum == 0)                                         // 是最后一个频率
                {
                    ringFreqNum = *ringPtr;
                    ringType    = ringPtr + 2;                                  // 重新开始一个基本音调
                    ringToneTimer--;                                            // 基本音调重复次数递减
                }
            }
            else                                                                // 振铃被禁止或振铃结束
            {
                ring_stop();                                                    // 停止振铃
            }
        }
    }
}

/**
  ******************************************************************************
  * @brief  定时器中断函数
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
