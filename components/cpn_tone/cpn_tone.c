/**
  ******************************************************************************
  * @file    cpn_tone.c
  * @author  mousie
  * @version V2.0.0
  * @date    2014.3.6
  * @brief   �ɲ�������, ʹ�÷������򾭹��˲��Ĺ��Ž��в���.
  *          ��Ҫ ezos����ϵͳ, ctrlport����, timer 10us�жϺ�����֧��.
  *
  * ����֪ʶ���:
  * ���򵥽������׵Ĺؼ����, ��������, ����, ����. ������������, ����, ����ĸ���
  *
  * ����, Ҳ������. ʵ��Ϊ��ͬ��Ƶ�ʲ���������.
  * ���ʱ�׼������Ƶ�ʶ�������:
  * ����        c4      #c4     d4      #d4     e4      f4      #f4     g4      #g4     a4      #a4     b4  ��
  * Ƶ��(Hz)    261.6   277.2   293.7   311.1   329.6   349.2   370.0   392.0   415.3   440.0   466.2   493.9
  * ������7��ȫ��(do re mi fa suo la xi)�������������(�Ա���#��).
  * �໥���˽�������Ƶ�ʳɱ�����ϵ, �� c5Ƶ�� = c4Ƶ��*2, a3Ƶ�� = c3Ƶ��/2.
  *
  * ����, ��ָ������ÿһС�ڵ������ܳ���. һ������ʱ��ԼΪ800-1200ms��Ϊ����.
  * ���׵Ľ���������һ��ʼ��ע����, ���õ��� 2/4, 3/4, 4/4, 6/8.
  * 2/4��, ָ4������Ϊһ��, ÿС��2��, ����2��4������. ǿ, ��.
  * 3/4��, ָ4������Ϊһ��, ÿС��3��, ����3��4������. ǿ, ��, ��.
  * 4/4��, ָ4������Ϊһ��, ÿС��4��, ����4��4������. ǿ, ��, ��ǿ, ��.
  * 6/8��, ָ8������Ϊһ��, ÿС��6��, ����6��8������. ǿ, ��, ��, ��ǿ, ��, ��.
  * ��Ƭ������Ƶ��ʱ, ���޷��������ȵ�, ������׵�ǿ����ϵ�޷����ֳ���.
  *
  * ����
  * ����, ����������������������, ��ʾ�����ڲ�ͬ������Ҫ����, ��Ҫͣ��.
  * ����, �����������Ƿ���ǵ���, ��ʾ�����Ҫ����Ķ̴ٶ������е���.
  * ����, �������������ұߵ�Բ��, ��ʾҪ������һ����������ĳ���.
  *
  *
  * ���״洢��ʽ˵��:
  * const unsigned char tone_name[] = {���׻�������, ����ʱ��, ����, ����, ����, ����, ..., 0, 0};
  * ���׵��������ɿ�ʹ�ù��� "MusicEncode.exe", �������������ͷ�����ֽ�(�����׻�������, ����ʱ��).
  *
  *     byte0, ���׻�������.
  *         - ��4bit, ������������, 1, �����(c1). 2, ������(c2). 3,����(c3). 4,��׼��(c4). 5,����(c5). 6,������(c6). �����(c7)
  *         - ��4bit, ָ���������ٸ�����. ��Чֵ 0-11. ����������Ϊ����, ��0=c4, 1=#c4, 2=d4, ...
  *         - �Ƽ�ֵ, 0x50.
  *     byte1, ����ʱ��. ��λΪ10ms. ��Чֵ 10-250.
  *         - һ��һ������Ϊ800-1200ms��Ϊ����. ����Խ��, �����ٶ�Խ��.
  *         - �Ƽ�ֵ, 128
  *     byte2/4/6/8/..., ����
  *         - ��λ��, ����. ��Чֵ1-7, ����������Ϊc4, ��ֱ�c4, d4, e4, f4, g4, a4, b4.
  *         - ʮλ��, ����. 1,���˶�����. 2,Ĭ������. 3, ���˶�����.
  *         - ��λ��, ����. 0,����, Ϊȫ��. 1,���������
  *         - byte=0x00, ��ʾ���׽���, ֹͣ����.
  *     byte3/5/7/9/..., ����
  *         - ��λ��, ��������, �����ķ�Ƶ. ��Чֵ0-6, �ֱ��Ӧ 1��, 2��, 4��, 8��, ..., 64������
  *         - ʮλ��, ����Ч��. 0,��ͨͣ��. 1,����. 2,����.
  *         - ��λ��, ��������. 0,�޸���. 1,�и���.
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
  * @brief    ˽�����Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup CPN_Tone_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */
#define SIGNATURE_DEFAULT               0                                       // ����Ĭ��ֵ, 0, c����ʼ
#define OCTACHORD_DEFAULT               5                                       // ����Ĭ��ֵ, 5, ��׼����
#define METER_TIME_DEFAULT              128                                     // ����Ĭ��ֵ, 128, һ��1.28s
/**
  * @}
  */

/** @defgroup CPN_Tone_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */
volatile static uint16_t note_time = 0;                                         // ���ڱ���������Ҫ��Ƶ��ʱ��
/**
  * @}
  */

/** @defgroup CPN_Tone_Private_Function
  * @brief    ����˽�к���
  * @{
  */
static ezos_task_t task_tone_play(ezos_state_t *state, ezos_para_t *para);
/**
  * @}
  */



/** @defgroup CPN_Tone_Function
  * @brief    ����ԭ�ļ�
  * @{
  */
/**
  ******************************************************************************
  * @brief  �������ź���
  * @param  ptone, ����ָ��, �������ݽṹ���ļ�˵��.
  * @retval none
  ******************************************************************************
  */
void tone_play(uint8_t *ptone)
{
    force_add((void *)task_tone_play, task_tone_play, (uint32_t)ptone, 0, 0);
}

/**
  ******************************************************************************
  * @brief  ������������
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
  * @brief  �ж������Ƿ񲥷���
  * @param  none
  * @retval 0, δ����. !0, ������
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
  * @brief  ��������������
  * @param  ptone, ����ָ��, �������ݽṹ���ļ�˵��.
  * @retval none
  ******************************************************************************
  */
ezos_task_t task_tone_play(ezos_state_t *state, ezos_para_t *para)
{
    // ��������Ƶ��.                         c4,  #c4, d4,  #d4, e4,  f4,  #f4, g4,  #g4, a4,  #a4, b4  ��
    static const uint16_t base_freq[12]   = {262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494};
    static const uint8_t  note_idx[7]     = {0, 2, 4, 5, 7,  9,  11};           // 1,2,3...7,��Ƶ�ʱ��е�λ��
    static const uint8_t  meter_demul[10] = {1, 2, 4, 8, 16, 32, 64, 4, 4, 4 }; // ���ķ�Ƶ, �ɹ���1/2, 1/4�Ƚ���

    static uint8_t  *ptone;
    static uint8_t  tone, time;
    static uint16_t tone_freq[12];                                              // ��������Ҫ��, ת���������Ƶ�ʱ�
    static uint8_t  meter_time;                                                 // ����ʱ��, ��λΪ10ms. ��Чֵ 10-200
    static uint8_t  counter_note;                                               // �������೤��(ͳ��10ms�ĸ���)
    static uint8_t  counter_space;                                              // ����ͣ�ٳ���(ͳ��10ms�ĸ���)

    switch(*state)
    {
    case 0: {
        uint8_t  octachord;                                                     // ����: 1,����. 2,����. 3,����.
        uint8_t  signature;                                                     // ����, ָ���������ٸ�����. ��Чֵ 0-11
        uint8_t  i,j;

        // ������ʼ��
        ctrlport_init(TONE_OUT);
        note_time = 0;
        ptone = (uint8_t *)(*para);

        // ȡ�����׵Ļ������׼�����ʱ��
        tone = *ptone++;
        time = *ptone++;
        octachord = tone >> 4;
        signature = tone & 0x0F;
        if (signature > 11) signature = SIGNATURE_DEFAULT;
        meter_time = time;
        if ((meter_time < 10) || (meter_time > 250)) meter_time = METER_TIME_DEFAULT;

        // ���ݻ���ת�ɵ�Ƶ�ʱ�
        for (i=0; i<12; i++)
        {
            j = i+signature;
            if (j>11) tone_freq[i] = base_freq[j%12]<<1;                        // �߰˶ȵ�����, ����Ƶ�ʷ�������
            else      tone_freq[i] = base_freq[j];

            if      (octachord == 1) tone_freq[i] >>= 3;                        // �������
            else if (octachord == 2) tone_freq[i] >>= 2;                        // ��������
            else if (octachord == 3) tone_freq[i] >>= 1;                        // ������
            else if (octachord == 5) tone_freq[i] <<= 1;                        // ������
            else if (octachord == 6) tone_freq[i] <<= 2;                        // ��������
            else if (octachord == 7) tone_freq[i] <<= 3;                        // �������
        }

        *state = 1;
        ezos_delay(10);
        break; }
    case 1: {
        uint8_t  note;                                                          // do...�߸�����, ��Чֵ1-7
        uint8_t  octachord;                                                     // ��������. 1,���˶�. 2,ԭ����. 3,���˶�.
        uint8_t  half;                                                          // ���������: 0, ����. 1, �������.
        uint16_t note_freq;                                                     // ��ǰ����������
        uint8_t  demultip;                                                      // ���ķ�Ƶ, demultiplication
        uint8_t  type;                                                          // ��������: 0, ��ͨ. 1, ����. 2, ����.
        uint8_t  dotted;                                                        // ��������: 0, �Ǹ���. 1, ��������

        tone = *ptone++;                                                        // ȡһ������
        time = *ptone++;
        if (tone)
        {
            // ��ȡ�����ͽ�����
            note      = tone % 10 - 1;                                          // ��λ��Ϊ����
            octachord = tone / 10 % 10;                                         // ʮλ��Ϊ�����Ƿ������˶�
            half      = tone / 100;                                             // ��λ��Ϊ�Ƿ����������

            if (note < 7)                                                       // ������Ч
            {
                // �����������Ƶ��
                note_freq = tone_freq[note_idx[note]+half];
                if      (octachord == 1) note_freq >>= 1;
                else if (octachord == 3) note_freq <<= 1;
            }
            else
            {
                note_freq = 0xFFFF;                                             // �Ƿ�����, ������.
            }

            // ����������Ĳ��ź�ͣ��ʱ��
            demultip = meter_demul[time%10];                                    // ���ķ�Ƶ
            type     = time/10%10;                                              // ��������
            dotted   = time/100;                                                // ��������
            counter_note = meter_time/demultip;                                 // �������೤��(ͳ��10ms�ĸ���)
            counter_note <<= 1;
            if (dotted == 1) counter_note += (counter_note >> 1);               // ���������ӳ�1.5��

            if (demultip < 4)   counter_space = (meter_time >> 2);              // ��ͨ���, 1/4�����ϵĽ���, ͣ��1/4��
            else                counter_space = (counter_note / 3);             // ��ͨ���, 1/4�����µĽ���, ͣ�������1/3.
            if      (type == 1) counter_space = 0;                              // ����
            else if (type == 2) counter_space = (counter_note >> 1);            // ����
            counter_note -= counter_space;                                      // Ҫ������ʱ��

            // ��������ʱ��
            note_time = 50000 / note_freq;                                      // ��������PWM����, ��ʼ����
            *state = 2;
            ezos_delay(10 * counter_note);
        }
        else                                                                    // tone == 0, ��������
        {
            note_time = 0;
            *state = 0xFF;                                                      // ���Ž���, ��β����
            ezos_delay(10);
        }
        break; }
    case 2: {
        // ����ͣ��ʱ��
        note_time = 0;
        ctrlport_off(TONE_OUT);
        *state = 1;                                                             // ȡ��һ������
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
  * @brief  ���� 10us ��ʱɨ��, �������ж���ʵ��
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
