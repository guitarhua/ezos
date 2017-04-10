/**
  ******************************************************************************
  * @file    cpn_tone.c
  * @author  mousie
  * @version V2.0.0
  * @date    2014.3.6
  * @brief   可播放曲调, 使用蜂鸣器或经过滤波的功放进行播放.
  *          需要 ezos操作系统, ctrlport驱动, timer 10us中断函数的支持.
  *
  * 曲谱知识简介:
  * 将简单介绍曲谱的关键组成, 包括音阶, 节拍, 其它. 其它包括连音, 顿音, 附点的概念
  *
  * 音阶, 也称音高. 实质为不同的频率产生的声音.
  * 国际标准的音阶频率定义如下:
  * 音阶        c4      #c4     d4      #d4     e4      f4      #f4     g4      #g4     a4      #a4     b4  　
  * 频率(Hz)    261.6   277.2   293.7   311.1   329.6   349.2   370.0   392.0   415.3   440.0   466.2   493.9
  * 音符由7个全音(do re mi fa suo la xi)和五个半音构成(旁边有#的).
  * 相互相差八阶音符的频率成倍数关系, 如 c5频率 = c4频率*2, a3频率 = c3频率/2.
  *
  * 节拍, 是指曲谱中每一小节的音符总长度. 一个节拍时长约为800-1200ms较为合适.
  * 曲谱的节拍在曲谱一开始就注明了, 常用的有 2/4, 3/4, 4/4, 6/8.
  * 2/4拍, 指4分音符为一拍, 每小节2拍, 可有2个4分音符. 强, 弱.
  * 3/4拍, 指4分音符为一拍, 每小节3拍, 可有3个4分音符. 强, 弱, 弱.
  * 4/4拍, 指4分音符为一拍, 每小节4拍, 可有4个4分音符. 强, 弱, 次强, 弱.
  * 6/8拍, 指8分音符为一拍, 每小节6拍, 可有6个8分音符. 强, 弱, 弱, 次强, 弱, 弱.
  * 单片机产生频率时, 是无法控制力度的, 因此音阶的强弱关系无法表现出来.
  *
  * 其它
  * 连音, 曲谱上用连线连起来的音, 表示连线内不同的音阶要连贯, 不要停顿.
  * 顿音, 曲谱上用三角符标记的音, 表示这个音要演奏的短促而轻巧有弹性.
  * 附点, 曲谱上在音符右边的圆点, 表示要再增加一半的音符节拍长度.
  *
  *
  * 曲谱存储格式说明:
  * const unsigned char tone_name[] = {曲谱基础音阶, 节拍时间, 音阶, 音长, 音阶, 音长, ..., 0, 0};
  * 曲谱的数据生成可使用工具 "MusicEncode.exe", 但必须自行添加头二个字节(即曲谱基础音阶, 节拍时间).
  *
  *     byte0, 曲谱基础音阶.
  *         - 高4bit, 曲调基础音区, 1, 最低音(c1). 2, 超低音(c2). 3,低音(c3). 4,标准音(c4). 5,高音(c5). 6,超高音(c6). 最高音(c7)
  *         - 低4bit, 指曲调升多少个半音. 有效值 0-11. 若基础音区为中音, 则0=c4, 1=#c4, 2=d4, ...
  *         - 推荐值, 0x50.
  *     byte1, 节拍时间. 单位为10ms. 有效值 10-250.
  *         - 一般一个节拍为800-1200ms较为合适. 节拍越短, 播放速度越快.
  *         - 推荐值, 128
  *     byte2/4/6/8/..., 音阶
  *         - 个位数, 音符. 有效值1-7, 若基础音阶为c4, 则分别c4, d4, e4, f4, g4, a4, b4.
  *         - 十位数, 音区. 1,降八度音区. 2,默认音区. 3, 升八度音区.
  *         - 百位数, 半音. 0,不升, 为全音. 1,升半个音阶
  *         - byte=0x00, 表示曲谱结束, 停止演奏.
  *     byte3/5/7/9/..., 音长
  *         - 个位数, 几分音符, 即节拍分频. 有效值0-6, 分别对应 1分, 2分, 4分, 8分, ..., 64分音符
  *         - 十位数, 演奏效果. 0,普通停顿. 1,连音. 2,顿音.
  *         - 百位数, 附点音符. 0,无附点. 1,有附点.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "cpn_tone.h"
#include "ezos.h"



/** @addtogroup Components
  * @{
  */
/** @defgroup CPN_Tone
  * @{
  */
#ifdef TONE_ENABLE



/**
  ******************************************************************************
  * @addtogroup CPN_Tone_Configure
  ******************************************************************************
  * @{
  */

/**
  * @}
  */



/** @defgroup CPN_Tone_Private_TypeDefine
  * @brief    私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup CPN_Tone_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */
#define SIGNATURE_DEFAULT               0                                       // 音阶默认值, 0, c调开始
#define OCTACHORD_DEFAULT               5                                       // 音区默认值, 5, 标准音区
#define METER_TIME_DEFAULT              128                                     // 节拍默认值, 128, 一拍1.28s
/**
  * @}
  */

/** @defgroup CPN_Tone_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */
volatile static uint16_t note_time = 0;                                         // 用于保存音符需要的频率时间
/**
  * @}
  */

/** @defgroup CPN_Tone_Private_Function
  * @brief    定义私有函数
  * @{
  */
static ezos_task_t task_tone_play(ezos_state_t *state, ezos_para_t *para);
/**
  * @}
  */



/** @defgroup CPN_Tone_Function
  * @brief    函数原文件
  * @{
  */
/**
  ******************************************************************************
  * @brief  曲调播放函数
  * @param  ptone, 曲调指针, 具体数据结构见文件说明.
  * @retval none
  ******************************************************************************
  */
void tone_play(uint8_t *ptone)
{
    force_add((void *)task_tone_play, task_tone_play, (uint32_t)ptone, 0, 0);
}

/**
  ******************************************************************************
  * @brief  曲调结束函数
  * @param  none
  * @retval none
  ******************************************************************************
  */
void tone_stop(void)
{
    note_time = 0;
    ctrlport_off(TONE_OUT);
    ezos_delete((void *)task_tone_play);
}

/**
  ******************************************************************************
  * @brief  判断铃声是否播放中
  * @param  none
  * @retval 0, 未播放. !0, 播放中
  ******************************************************************************
  */
uint8_t tone_isPlaying(void) {
    ezos_t status;
    status = ezos_status_get(task_tone_play);

    if (status == EZOS_DELETE) {
        return 0;
    } else {
        return 1;
    }
}

/**
  ******************************************************************************
  * @brief  曲调播放任务函数
  * @param  ptone, 曲调指针, 具体数据结构见文件说明.
  * @retval none
  ******************************************************************************
  */
ezos_task_t task_tone_play(ezos_state_t *state, ezos_para_t *para)
{
    // 音符基础频率.                         c4,  #c4, d4,  #d4, e4,  f4,  #f4, g4,  #g4, a4,  #a4, b4  　
    static const uint16_t base_freq[12]   = {262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494};
    static const uint8_t  note_idx[7]     = {0, 2, 4, 5, 7,  9,  11};           // 1,2,3...7,在频率表中的位置
    static const uint8_t  meter_demul[10] = {1, 2, 4, 8, 16, 32, 64, 4, 4, 4 }; // 节拍分频, 可构成1/2, 1/4等节拍

    static uint8_t  *ptone;
    static uint8_t  tone, time;
    static uint16_t tone_freq[12];                                              // 根据曲谱要求, 转换后的音阶频率表
    static uint8_t  meter_time;                                                 // 节拍时间, 单位为10ms. 有效值 10-200
    static uint8_t  counter_note;                                               // 音符演奏长度(统计10ms的个数)
    static uint8_t  counter_space;                                              // 音符停顿长度(统计10ms的个数)

    switch(*state)
    {
    case 0: {
        uint8_t  octachord;                                                     // 音区: 1,低音. 2,中音. 3,高音.
        uint8_t  signature;                                                     // 音阶, 指曲谱升多少个半音. 有效值 0-11
        uint8_t  i,j;

        // 参数初始化
        ctrlport_init(TONE_OUT);
        note_time = 0;
        ptone = (uint8_t *)(*para);

        // 取出曲谱的基础音阶及节拍时间
        tone = *ptone++;
        time = *ptone++;
        octachord = tone >> 4;
        signature = tone & 0x0F;
        if (signature > 11) signature = SIGNATURE_DEFAULT;
        meter_time = time;
        if ((meter_time < 10) || (meter_time > 250)) meter_time = METER_TIME_DEFAULT;

        // 根据基调转成的频率表
        for (i=0; i<12; i++)
        {
            j = i+signature;
            if (j>11) tone_freq[i] = base_freq[j%12]<<1;                        // 高八度的音区, 基础频率翻倍即可
            else      tone_freq[i] = base_freq[j];

            if      (octachord == 1) tone_freq[i] >>= 3;                        // 最低音区
            else if (octachord == 2) tone_freq[i] >>= 2;                        // 超低音区
            else if (octachord == 3) tone_freq[i] >>= 1;                        // 低音区
            else if (octachord == 5) tone_freq[i] <<= 1;                        // 高音区
            else if (octachord == 6) tone_freq[i] <<= 2;                        // 超高音区
            else if (octachord == 7) tone_freq[i] <<= 3;                        // 最高音区
        }

        *state = 1;
        ezos_delay(10);
        break; }
    case 1: {
        uint8_t  note;                                                          // do...七个音阶, 有效值1-7
        uint8_t  octachord;                                                     // 升降音区. 1,降八度. 2,原音区. 3,升八度.
        uint8_t  half;                                                          // 升半个音阶: 0, 不升. 1, 升半个音.
        uint16_t note_freq;                                                     // 当前音符的音阶
        uint8_t  demultip;                                                      // 节拍分频, demultiplication
        uint8_t  type;                                                          // 音符类型: 0, 普通. 1, 连音. 2, 顿音.
        uint8_t  dotted;                                                        // 附点音符: 0, 非附点. 1, 附点音符

        tone = *ptone++;                                                        // 取一个音符
        time = *ptone++;
        if (tone)
        {
            // 读取音符和节拍数
            note      = tone % 10 - 1;                                          // 个位数为音符
            octachord = tone / 10 % 10;                                         // 十位数为音符是否升降八度
            half      = tone / 100;                                             // 百位数为是否升半个音阶

            if (note < 7)                                                       // 音符有效
            {
                // 计算该音符的频率
                note_freq = tone_freq[note_idx[note]+half];
                if      (octachord == 1) note_freq >>= 1;
                else if (octachord == 3) note_freq <<= 1;
            }
            else
            {
                note_freq = 0xFFFF;                                             // 非法音符, 不播放.
            }

            // 计算该音符的播放和停顿时间
            demultip = meter_demul[time%10];                                    // 节拍分频
            type     = time/10%10;                                              // 音符类型
            dotted   = time/100;                                                // 附点音符
            counter_note = meter_time/demultip;                                 // 音符演奏长度(统计10ms的个数)
            counter_note <<= 1;
            if (dotted == 1) counter_note += (counter_note >> 1);               // 附点音符延长1.5倍

            if (demultip < 4)   counter_space = (meter_time >> 2);              // 普通情况, 1/4拍以上的节拍, 停顿1/4拍
            else                counter_space = (counter_note / 3);             // 普通情况, 1/4拍以下的节拍, 停顿自身的1/3.
            if      (type == 1) counter_space = 0;                              // 连音
            else if (type == 2) counter_space = (counter_note >> 1);            // 顿音
            counter_note -= counter_space;                                      // 要发音的时间

            // 音符播放时间
            note_time = 50000 / note_freq;                                      // 计算音符PWM次数, 开始发音
            *state = 2;
            ezos_delay(10 * counter_note);
        }
        else                                                                    // tone == 0, 曲调结束
        {
            note_time = 0;
            *state = 0xFF;                                                      // 播放结束, 收尾工作
            ezos_delay(10);
        }
        break; }
    case 2: {
        // 音符停顿时间
        note_time = 0;
        ctrlport_off(TONE_OUT);
        *state = 1;                                                             // 取下一个音符
        ezos_delay(10 * counter_space);
        break; }
    default: {
        note_time = 0;
        ctrlport_off(TONE_OUT);
        break; }
    }
}

/**
  ******************************************************************************
  * @brief  曲调 10us 定时扫描, 必须在中断中实现
  * @param  none
  * @retval none
  ******************************************************************************
  */
void tone_scan_10us(void)
{
    static uint16_t note_timer;

    if (note_time)
    {
        if (++note_timer >= note_time)
        {
            ctrlport_toggle(TONE_OUT);
            note_timer = 0;
        }
    }
    else
    {
        note_timer = 0;
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
