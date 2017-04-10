/**
  ******************************************************************************
  * @file    drv_tcsbus.c
  * @author  mousie
  * @version V2.0.0
  * @date    2012.10.26
  * @brief   TCS BUS �ײ�����. δ����
  ******************************************************************************
  */

/* Include -------------------------------------------------------------------*/
#include "drv_tcsbus.h"



/** @addtogroup Drivers
  * @{
  */
/** @defgroup DRV_TCSBUS
  * @{
  */
#if (TCSBUS_NUM > 0)



/**
  ******************************************************************************
  * @addtogroup DRV_TCSBUS_Configure
  ******************************************************************************
  * @{
  */
#include "stm32f10x.h"
#define tcsbus_enable_int(mask)         __set_PRIMASK(mask)                     ///< �궨��TCSBUS�жϴ򿪺���, ���������ֲ
#define tcsbus_disable_int()            __get_PRIMASK(); __disable_irq()        ///< �궨��TCSBUS�жϹرպ���, ���������ֲ
/**
  * @}
  */



/** @defgroup DRV_TCSBUS_Private_TypeDefine
  * @brief    ˽�����Ͷ���
  * @{
  */
/// TCSBUSӲ���������Ͷ���
typedef struct
{
    const uint32_t TX_CLK;                                                      ///< TCSBUS����IO��ʱ��ģ��
    const uint32_t RX_CLK;                                                      ///< TCSBUS����IO��ʱ��ģ��
    GPIO_TypeDef *const TX_PORT;                                                ///< TCSBUS����IO��PORT��
    GPIO_TypeDef *const RX_PORT;                                                ///< TCSBUS����IO��PORT��
    const uint16_t TX_PIN;                                                      ///< TCSBUS����IO��PIN��
    const uint16_t RX_PIN;                                                      ///< TCSBUS����IO��PIN��
    void (*rx_callback)(tcsbus_type_t type, uint32_t data);                     ///< TCSBUS�����жϻص�����
} tcsbus_hw_t;

/// TCSBUS�������Ͷ���
typedef struct
{
    uint32_t rx_buf;                                                            ///< TCSBUS �ж��ظ����յ�Э��
    volatile uint32_t tx_buf;                                                   ///< TCSBUS ���ͻ�����
    volatile uint32_t tx_rx_buf;                                                ///< TCSBUS �շ�������
    volatile uint32_t rx_level;                                                 ///< ��¼ RX ������ʷֵ
    volatile uint16_t tx_rx_flag;                                               ///< �շ���־
    volatile uint8_t  parity;                                                   ///< У��ֵ
    volatile uint8_t  start_timer;                                              ///< ��¼����λʱ�䳤��/2��ֵ(һ��Ϊ6ms/0.2ms/2=15����)
    volatile uint8_t  protocal_length;                                          ///< λ���ȼ�����
    volatile uint8_t  bit_timer;                                                ///< �շ���ʱ��ʱ�䳤�ȣ������ж�0,1
    volatile uint8_t  wait_timer;                                               ///< ׼������ʱ������
    volatile uint8_t  q_timer;                                                  ///< ����QЭ����ʱ
    volatile uint8_t  r_timer;                                                  ///< �����ظ�����Э�����ʱ
} tcsbus_para_t;
/**
  * @}
  */

/** @defgroup DRV_TCSBUS_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */
/// tx_rx_flag ���λ�궨��
#define TX_ACTIVE                       0x0001                                  ///< Sender aktiv             ��λ��ʾ�����Ѿ�����
#define TX_READY                        0x0002                                  ///< Senden starten           ��λ��ʾ��Ҫ������Ϣ
#define TX_REPEAT                       0x0004                                  ///< Sendewiederholung        ��λ��ʾ��Ҫ���·�����Ϣ
#define TX_Q_READY                      0x0008                                  ///< Quittung senden          ��λ��ʾ��Ҫ����QЭ��

#define RX_ACTIVE                       0x0010                                  ///< Empfang aktiv            ��λ��ʾ�����Ѿ�����
#define RX_START                        0x0020                                  ///< Startimp. empf.          ��λ��ʾ���յ�6ms�͵�ƽ���������
#define RX_LENGTH                       0x0040                                  ///< Laengenbit empf.         ��λ��ʾ���յ��ǳ��ȱ��
#define TX_RX_LONG                      0x0080                                  ///< Info lang                ��λ��ʾ���ͽ��յ��ǳ�Э��(32bit)
#define RX_FINISH                       0x0100                                  ///< Info empfangen           ��λ��ʾ������Ϣ����ɣ���ȷ����
#define RX_Q_NEED                       0x0200                                  ///< Quittungsempfang         ��λ��ʾҪ����QЭ��
#define RX_Q_FINISH                     0x0400                                  ///< Quittung wurde empfangen ��λ��ʾ����QЭ�����ɣ���ȷ����
/**
  * @}
  */

/** @defgroup DRV_TCSBUS_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */
/// TCSBUSӲ������ӳ���
static const tcsbus_hw_t tcsbus_hw[TCSBUS_NUM] = {
#if (TCSBUS_NUM > 0)
    {TCSBUS0_TX_CLK,  TCSBUS0_RX_CLK,
     TCSBUS0_TX_PORT, TCSBUS0_RX_PORT,
     TCSBUS0_TX_PIN,  TCSBUS0_RX_PIN,
     TCSBUS0_RX_CALLBACK,},
#endif

#if (TCSBUS_NUM > 1)
    {TCSBUS1_TX_CLK,  TCSBUS1_RX_CLK,
     TCSBUS1_TX_PORT, TCSBUS1_RX_PORT,
     TCSBUS1_TX_PIN,  TCSBUS1_RX_PIN,
     TCSBUS1_RX_CALLBACK,},
#endif

#if (TCSBUS_NUM > 2)
    {TCSBUS2_TX_CLK,  TCSBUS2_RX_CLK,
     TCSBUS2_TX_PORT, TCSBUS2_RX_PORT,
     TCSBUS2_TX_PIN,  TCSBUS2_RX_PIN,
     TCSBUS2_RX_CALLBACK,},
#endif
};

/// TCSBUS������
static tcsbus_para_t tcsbus_para[TCSBUS_NUM];
/**
  * @}
  */

/** @defgroup DRV_TCSBUS_Private_Function
  * @brief    ����˽�к���
  * @{
  */
static void tcsbus_tx_200us(tcsbus_name_t tcsbus_name);                         ///< TCSBUS ������ѯ����
static void tcsbus_rx_200us(tcsbus_name_t tcsbus_name);                         ///< TCSBUS ������ѯ����

/// TCSBUS PORT�ڳ�ʼ��
__INLINE static void tcsbus_port_init(tcsbus_name_t tcsbus_name)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(tcsbus_hw[tcsbus_name].TX_CLK | tcsbus_hw[tcsbus_name].RX_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin   = tcsbus_hw[tcsbus_name].TX_PIN;
    GPIO_Init(tcsbus_hw[tcsbus_name].TX_PORT, &GPIO_InitStructure);             // ����TCSBUS TX����, �������
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin   = tcsbus_hw[tcsbus_name].RX_PIN;
    GPIO_Init(tcsbus_hw[tcsbus_name].RX_PORT, &GPIO_InitStructure);             // ����TCSBUS RX����, ��������
}

/// TCSBUS TX ��ƽ�ø�
__INLINE static void TCSBUS_TX_HIGH(tcsbus_name_t tcsbus_name)
{
    tcsbus_hw[tcsbus_name].TX_PORT->BSRR  = tcsbus_hw[tcsbus_name].TX_PIN;
}

/// TCSBUS TX ��ƽ�õ�
__INLINE static void TCSBUS_TX_LOW(tcsbus_name_t tcsbus_name)
{
    tcsbus_hw[tcsbus_name].TX_PORT->BRR = tcsbus_hw[tcsbus_name].TX_PIN;
}

/// ��ȡ TCSBUS TX ��ƽֵ
__INLINE static int32_t TCSBUS_TX_LEVEL(tcsbus_name_t tcsbus_name)
{
    return (tcsbus_hw[tcsbus_name].TX_PORT->IDR & tcsbus_hw[tcsbus_name].TX_PIN);
}

/// ��ȡ TCSBUS RX ��ƽֵ
__INLINE static int32_t TCSBUS_RX_LEVEL(tcsbus_name_t tcsbus_name)
{
    return (tcsbus_hw[tcsbus_name].RX_PORT->IDR & tcsbus_hw[tcsbus_name].RX_PIN);
}
/**
  * @}
  */



/** @defgroup DRV_TCSBUS_Function
  * @brief    ����ԭ�ļ�
  * @{
  */

/**
  ******************************************************************************
  * @brief  ��ʼ������ TCSBUS ����
  * @param  none
  * @retval none
  ******************************************************************************
  */
void tcsbus_init_all(void)
{
    uint8_t i;

    for (i=0; i<TCSBUS_NUM; i++)
        tcsbus_init((tcsbus_name_t)i);
}

/**
  ******************************************************************************
  * @brief  ��ʼ��ָ�� TCSBUS ����
  * @param  tcsbus_name
  *   @arg  ����ֵ��ο� tcsbus_name_t ���Ͷ����е�ö��ֵ
  * @retval none
  ******************************************************************************
  */
void tcsbus_init(tcsbus_name_t tcsbus_name)
{
    tcsbus_port_init(tcsbus_name);
    TCSBUS_TX_LOW(tcsbus_name);

    tcsbus_para[tcsbus_name].tx_rx_flag = 0;
    tcsbus_para[tcsbus_name].tx_buf     = 0;
    tcsbus_para[tcsbus_name].rx_buf     = 0;
    tcsbus_para[tcsbus_name].tx_rx_buf  = 0;
    tcsbus_para[tcsbus_name].wait_timer = 0;
    tcsbus_para[tcsbus_name].q_timer    = 0;
    tcsbus_para[tcsbus_name].r_timer    = 0;
}

/**
  ******************************************************************************
  * @brief  TCSBUS 200us��ʱɨ�躯��
  * @param  tcsbus_name
  *   @arg  ����ֵ��ο� tcsbus_name_t ���Ͷ����е�ö��ֵ
  * @retval none
  ******************************************************************************
  */
void tcsbus_scan_200us(tcsbus_name_t tcsbus_name)
{
    if (tcsbus_para[tcsbus_name].wait_timer) tcsbus_para[tcsbus_name].wait_timer--;
    if (tcsbus_para[tcsbus_name].r_timer)    tcsbus_para[tcsbus_name].r_timer--;

    // ���ǽ���QЭ�飬δ������գ���Ҫ������Ϣ�����ͼ��ʱ�䵽 ���ͣ�������ա�
    if (   !(tcsbus_para[tcsbus_name].tx_rx_flag & (RX_Q_NEED + RX_ACTIVE))
         && (tcsbus_para[tcsbus_name].tx_rx_flag & TX_READY)
         && (tcsbus_para[tcsbus_name].wait_timer == 0) )
    {
        tcsbus_tx_200us(tcsbus_name);
    }
    else
    {
        tcsbus_rx_200us(tcsbus_name);
    }
}

/**
  ******************************************************************************
  * @brief  TCS BUS Э�鷢�ͺ���
  * @param  tcsbus_name
  *   @arg  ����ֵ��ο� tcsbus_name_t ���Ͷ����е�ö��ֵ
  * @retval none
  ******************************************************************************
  */
void tcsbus_tx_200us(tcsbus_name_t tcsbus_name)
{
    uint8_t bit_len;
    uint32_t temp;

    if (tcsbus_para[tcsbus_name].tx_rx_flag & TX_ACTIVE)                        // �����Ѽ���
    {
        tcsbus_para[tcsbus_name].bit_timer--;

        // ���͵�ƽʧ��(A����P�߶�·)
        if ((!TCSBUS_RX_LEVEL(tcsbus_name)) && (tcsbus_para[tcsbus_name].bit_timer == 29))
        {
            TCSBUS_TX_LOW(tcsbus_name);                                         // ���ߵ�ƽ�ָ�Ϊ25V��������ѹ�����ܡ�
            tcsbus_para[tcsbus_name].tx_rx_flag &= ~(TX_ACTIVE + TX_READY);
        }
        // ���25V������Ϊ8V��0V(�б�������豸�ڷ�����Ϣ)
        else if ((!TCSBUS_TX_LEVEL(tcsbus_name)) && TCSBUS_RX_LEVEL(tcsbus_name))
        {
            if(tcsbus_para[tcsbus_name].tx_rx_flag & TX_Q_READY)                // ��QЭ��Ҫ����
            {
                tcsbus_para[tcsbus_name].tx_rx_flag = RX_Q_NEED;                // ����յ���QЭ��
                tcsbus_para[tcsbus_name].q_timer = 0;
            }
            else
            {
                tcsbus_para[tcsbus_name].tx_rx_flag = 0;
                tcsbus_para[tcsbus_name].wait_timer = 100;                      // 20ms����ܽ��뷢��״̬
            }
        }
        else if (tcsbus_para[tcsbus_name].bit_timer == 0)                       // ������һλ
        {
            if (TCSBUS_TX_LEVEL(tcsbus_name))
            {
                TCSBUS_TX_LOW(tcsbus_name);                                     // �������
            }
            else
            {
                TCSBUS_TX_HIGH(tcsbus_name);
            }
            if (tcsbus_para[tcsbus_name].tx_rx_flag & TX_RX_LONG)               // ���͵��ǳ�Э��
            {
                temp = 1;
                bit_len = 32;
            }
            else                                                                // ���͵��Ƕ�Э��
            {
                temp = 0;
                if (tcsbus_para[tcsbus_name].tx_rx_flag & TX_Q_READY)
                    bit_len = 4;                                                // ���͵���Q�źţ���Э��
                else
                    bit_len = 16;
            }
            if (    (tcsbus_para[tcsbus_name].protocal_length <= bit_len)
                 && (tcsbus_para[tcsbus_name].protocal_length > 0) )            // �������ݴ����У��λ����
            {
                temp = tcsbus_para[tcsbus_name].tx_rx_buf & 0x80000000;
                tcsbus_para[tcsbus_name].tx_rx_buf <<= 1;
                if (temp != 0) tcsbus_para[tcsbus_name].parity ^= 1;
            }
            else if (tcsbus_para[tcsbus_name].protocal_length == (bit_len+1))   // ������ĩλ��У��λ
            {
                temp = tcsbus_para[tcsbus_name].parity;
            }
            else if (tcsbus_para[tcsbus_name].protocal_length > (bit_len+1))    // ���ͽ���
            {
                tcsbus_para[tcsbus_name].tx_rx_flag &= ~TX_ACTIVE;              // �رշ��ͼ�����
                if (tcsbus_para[tcsbus_name].tx_rx_flag & TX_Q_READY)           // ���������QЭ��
                {
                    tcsbus_para[tcsbus_name].tx_rx_flag &= ~(TX_READY | TX_Q_READY);
                    tcsbus_para[tcsbus_name].wait_timer = 100;
                }
                else                                                            // ������AЭ��
                {
                    tcsbus_para[tcsbus_name].tx_rx_flag |= RX_Q_NEED;           // ׼������QЭ��
                    tcsbus_para[tcsbus_name].q_timer = 0;
                }
            }
            if (temp) tcsbus_para[tcsbus_name].bit_timer = 20;                  // ����Ϊ1����ƽ����4ms
            else      tcsbus_para[tcsbus_name].bit_timer = 10;                  // ����Ϊ0����ƽ����2ms
            tcsbus_para[tcsbus_name].protocal_length++;                         // ���ݳ���+1
        }
    }
    else                                                                        // �ս��뷢��״̬
    {
        if (TCSBUS_RX_LEVEL(tcsbus_name))                                       // ���ߵ�ƽΪ 8V������æ������豸�ڷ�����Ϣ
        {
            if (tcsbus_para[tcsbus_name].tx_rx_flag & TX_Q_READY)               // ��Ҫ����QЭ�飬������豸�Ѿ�������QЭ��
            {
                tcsbus_para[tcsbus_name].tx_rx_flag = RX_Q_NEED;                // ���ձ���豸��QЭ��
                tcsbus_para[tcsbus_name].q_timer = 0;
            }
            else
            {
                tcsbus_para[tcsbus_name].tx_rx_flag = 0;
                tcsbus_para[tcsbus_name].wait_timer = 120;                      // 24ms���������������
            }
            return;                                                             // ����æ����ʱ����������Ϣ����
        }
        else                                                                    // ���߿��У������״̬
        {
            //// �˴������Ӻ���, ����Э��ʱ��ʱ�ر������� 200ms
            TCSBUS_TX_HIGH(tcsbus_name);                                        // ��������λ,�����ߵ�ƽ����Ϊ8V
            tcsbus_para[tcsbus_name].bit_timer = 30;                            // ����������ƽʱ�� 30*0.2ms=6ms
            tcsbus_para[tcsbus_name].protocal_length = 0;                       // ��Ϣλ��������
            tcsbus_para[tcsbus_name].parity = 1;
            tcsbus_para[tcsbus_name].tx_rx_buf = tcsbus_para[tcsbus_name].tx_buf;
            tcsbus_para[tcsbus_name].tx_rx_flag |= TX_ACTIVE;                   // �����״̬
        }
    }
}

/**
  ******************************************************************************
  * @brief  TCS BUS Э����պ���
  * @param  tcsbus_name
  *   @arg  ����ֵ��ο� tcsbus_name_t ���Ͷ����е�ö��ֵ
  * @retval none
  ******************************************************************************
  */
void tcsbus_rx_200us(tcsbus_name_t tcsbus_name)
{
    uint8_t temp, bit_len;

    if (tcsbus_para[tcsbus_name].tx_rx_flag & RX_ACTIVE)                        // �����Ѽ���
    {
        tcsbus_para[tcsbus_name].bit_timer++;                                   // ��¼ʱ�䳤��
        if (TCSBUS_RX_LEVEL(tcsbus_name) == tcsbus_para[tcsbus_name].rx_level)  // TCS BUS ����û�б仯
        {
            // ��������λ����, �����쳣���
            temp = (tcsbus_para[tcsbus_name].start_timer << 1);
            if (tcsbus_para[tcsbus_name].bit_timer > temp)
            {
                tcsbus_para[tcsbus_name].tx_rx_flag = 0;
                tcsbus_para[tcsbus_name].wait_timer = 50;                       // 10ms��������
            }
        }
        else                                                                    // ���ط����仯
        {
            tcsbus_para[tcsbus_name].rx_level = TCSBUS_RX_LEVEL(tcsbus_name);
            temp = tcsbus_para[tcsbus_name].start_timer;
            if (tcsbus_para[tcsbus_name].bit_timer > temp)
                temp = 1;                                                       // ʱ��=������ƽʱ���2/3�����յ�������Ϊ1
            else
                temp = 0;                                                                   // ʱ��=������ƽʱ���1/3�����յ�������Ϊ0
            tcsbus_para[tcsbus_name].bit_timer=0;                               // ʱ�䳤�����¼���
            if (tcsbus_para[tcsbus_name].tx_rx_flag & RX_LENGTH)                // ��Э�鳤��λ
            {
                tcsbus_para[tcsbus_name].tx_rx_flag &= ~RX_LENGTH;
                if (temp == 1) tcsbus_para[tcsbus_name].tx_rx_flag |= TX_RX_LONG;
                else           tcsbus_para[tcsbus_name].tx_rx_flag &= ~TX_RX_LONG;
            }
            else                                                                // ����λ
            {
                if (tcsbus_para[tcsbus_name].tx_rx_flag & TX_RX_LONG)
                {
                    bit_len = 32;                                               // ��Э��32bit
                }
                else
                {
                    bit_len = 16;                                               // ��Э��16bit
                    if (tcsbus_para[tcsbus_name].tx_rx_flag & RX_Q_NEED)
                        bit_len = 4;                                            // ��QЭ�飬4bit
                }
                if (tcsbus_para[tcsbus_name].protocal_length < bit_len)         // δ������������Ϣ��
                {
                    tcsbus_para[tcsbus_name].tx_rx_buf <<= 1;                   // ������Ϣ�����У��
                    if (temp == 1)
                    {
                        tcsbus_para[tcsbus_name].tx_rx_buf |= 1;
                        tcsbus_para[tcsbus_name].parity ^= 1;
                    }
                }
                else                                                            // ��������Ϣ��
                {
                    if (tcsbus_para[tcsbus_name].parity == temp)                // У����ȷ
                    {
                        tcsbus_para[tcsbus_name].tx_rx_flag |= RX_FINISH;       // ��ʾ�Ѿ��ɹ����ܵ���Ϣ
                        if (tcsbus_para[tcsbus_name].tx_rx_flag & RX_Q_NEED)    // ����ǽ��յ���QЭ��
                        {
                            tcsbus_para[tcsbus_name].tx_rx_flag |= RX_Q_FINISH; // ��ʾ�Ѿ��յ�QЭ��
                            tcsbus_para[tcsbus_name].tx_rx_flag &= ~RX_Q_NEED;  // ȡ��QЭ����(���յ���һ��Э�鲻����QЭ��)
                            if (tcsbus_hw[tcsbus_name].rx_callback != 0)
                            {
                                if (tcsbus_para[tcsbus_name].tx_rx_flag & TX_RX_LONG)
                                    tcsbus_hw[tcsbus_name].rx_callback(Q_LONG, tcsbus_para[tcsbus_name].tx_rx_buf);
                                else                                            // Q��Э��
                                    tcsbus_hw[tcsbus_name].rx_callback(Q_SHORT, tcsbus_para[tcsbus_name].tx_rx_buf);
                            }
                            tcsbus_para[tcsbus_name].wait_timer = 50;           // 10ms��������ͳ���
                            tcsbus_para[tcsbus_name].r_timer    = 0;            // ��ȷ����Ӧ��Э��, ��һ�����յ�Э����봦��
                            if(tcsbus_para[tcsbus_name].tx_rx_flag & TX_READY)  // ��Ϣ���ͱ�� (�˴����������������Ϣ���ͱ�ǵ�)
                            {
                                tcsbus_para[tcsbus_name].tx_rx_flag  &= ~(TX_READY | TX_REPEAT);                                               // ������QЭ���ȡ���������ط����
                                tcsbus_para[tcsbus_name].wait_timer = 30;       // 6ms��������ͳ���
                            }
                        }
                        else                                                    // ���յ�AЭ��
                        {
                            tcsbus_para[tcsbus_name].tx_rx_flag |= RX_Q_NEED;   // ��λҪ����QЭ����(��һ��������QЭ�飬q_timer��ʱ����Ϊ��ͨЭ��)
                            tcsbus_para[tcsbus_name].q_timer = 0;               // ����QЭ����ʱ������
                            if (tcsbus_hw[tcsbus_name].rx_callback != 0)
                            {
                                if ((tcsbus_para[tcsbus_name].r_timer == 0) || (tcsbus_para[tcsbus_name].rx_buf != tcsbus_para[tcsbus_name].tx_rx_buf))
                                {
                                    if (tcsbus_para[tcsbus_name].tx_rx_flag & TX_RX_LONG)
                                        tcsbus_hw[tcsbus_name].rx_callback(A_LONG, tcsbus_para[tcsbus_name].tx_rx_buf);
                                    else                                        // A��Э��
                                        tcsbus_hw[tcsbus_name].rx_callback(A_SHORT, tcsbus_para[tcsbus_name].tx_rx_buf);
                                }
                            }
                            tcsbus_para[tcsbus_name].r_timer = 100;
                            tcsbus_para[tcsbus_name].rx_buf  = tcsbus_para[tcsbus_name].tx_rx_buf;
                        }
                    }
                    tcsbus_para[tcsbus_name].tx_rx_flag &= ~RX_ACTIVE;          // ���ս�������������״̬
                }
                tcsbus_para[tcsbus_name].protocal_length++;                     // ����λ���ȼ�һ��׼��������һλ��Ϣ
            }
        }
    }
    else                                                                        // ����δ�������״̬
    {
        if (TCSBUS_RX_LEVEL(tcsbus_name))                                       // ����״̬ʱ��TCS BUS ��ƽ��תΪ 8V, ��������Э��
        {
            if (tcsbus_para[tcsbus_name].tx_rx_flag & RX_START)                 // ������λ
            {
                tcsbus_para[tcsbus_name].bit_timer++;                           // ����ʱ�䳤��
            }
            else                                                                // ��ƽ���ͣ������źſ�ʼ
            {
                //// �˴������Ӻ���, ����Э��ʱ��ֹ������ 200ms
                tcsbus_para[tcsbus_name].tx_rx_flag |= RX_START;                // ���Ϊ����λ
                tcsbus_para[tcsbus_name].bit_timer = 0;
            }
        }
        else                                                                    // TCS BUS �� 25V
        {
            if (tcsbus_para[tcsbus_name].tx_rx_flag & RX_START)                 // ����λ�жϽ���
            {
                // ׼������Э�鳤��λ�ж�,�����Ѿ�����
                tcsbus_para[tcsbus_name].tx_rx_flag |=  (RX_LENGTH | RX_ACTIVE);
                tcsbus_para[tcsbus_name].tx_rx_flag &= ~(RX_START | RX_FINISH | RX_Q_FINISH | TX_Q_READY);                                              // һϵ�г�ʼ��
                tcsbus_para[tcsbus_name].start_timer  = tcsbus_para[tcsbus_name].bit_timer >> 1;                                                // ��¼����λʱ�䳤��/2��ֵ�������ж�0,1
                tcsbus_para[tcsbus_name].bit_timer  = 0;
                tcsbus_para[tcsbus_name].rx_level    = TCSBUS_RX_LEVEL(tcsbus_name);
                tcsbus_para[tcsbus_name].tx_rx_buf = 0;                         // ������Ϣ�Ĵ���
                tcsbus_para[tcsbus_name].parity   = 1;
                tcsbus_para[tcsbus_name].protocal_length  = 0;                  // ����λ��������
            }
            if (tcsbus_para[tcsbus_name].tx_rx_flag & RX_Q_NEED)                // Ҫ����QЭ��
            {
                tcsbus_para[tcsbus_name].q_timer++;                             // QЭ����ʱ��¼
                if (tcsbus_para[tcsbus_name].q_timer >= 50)                     // QЭ�鳬ʱ��50*0.2ms=10ms
                {
                    tcsbus_para[tcsbus_name].tx_rx_flag  &= ~(RX_Q_NEED | RX_Q_FINISH);                                                     // �ȴ�����QЭ�鳬ʱ������QЭ��������
                    tcsbus_para[tcsbus_name].wait_timer = 50;                   // ���÷��ͼ��ʱ�䣬10ms��������
                    if (tcsbus_para[tcsbus_name].tx_rx_flag & TX_READY)         // ����Ϣ������
                    {
                        if (tcsbus_para[tcsbus_name].tx_rx_flag & TX_REPEAT)    // ���ط���Ϣ״̬(��������QЭ��Ӧ���״̬)
                        {
                            tcsbus_para[tcsbus_name].tx_rx_flag &= ~(TX_READY | TX_REPEAT);                                                  // ���ͱ�����㣬��������
                            tcsbus_para[tcsbus_name].wait_timer = 100;          // 20ms��ſ�������������
                            if (tcsbus_hw[tcsbus_name].rx_callback != 0)        // �ȴ�QЭ�鳬ʱ
                            {
                                tcsbus_hw[tcsbus_name].rx_callback(Q_NONE, 0);
                            }
                        }
                        else                                                    // ���͵���Ϣ��QЭ��Ӧ��׼���ط�
                        {
                            tcsbus_para[tcsbus_name].tx_rx_flag |= TX_REPEAT;   // ��λ�ط����
                        }
                    }
                }
            }
        }
    }
}

/**
  ******************************************************************************
  * @brief  TCS BUS Э�鷢��׼������
  * @param  tcsbus_name
  *   @arg  ����ֵ��ο� tcsbus_name_t ���Ͷ����е�ö��ֵ
  * @param  type, Ҫ���͵�Э������
  *   @arg  A_LONG,  ׼������A��Э��, 32bit
  *   @arg  A_SHORT, ׼������A��Э��, 16bit
  *   @arg  Q_LONG,  ׼������Q��Э��, 32bit
  *   @arg  Q_SHORT, ׼������Q��Э��, 4bit
  * @param  data, Ҫ���͵�����
  * @retval ����ֵ, 0-׼�����ͳɹ�, 1-׼������ʧ��
  ******************************************************************************
  */
uint8_t tcsbus_tx(tcsbus_name_t tcsbus_name, tcsbus_type_t type, uint32_t data)
{
    uint32_t mask;

    if ((type & 0x10) == 0x00)                                                  // ׼������AЭ��
    {
        // ���ڴ����շ�����
        if ((tcsbus_para[tcsbus_name].wait_timer) || (tcsbus_para[tcsbus_name].tx_rx_flag & (TX_ACTIVE | TX_READY | RX_ACTIVE | RX_START | RX_Q_NEED)))
        {
            return 1;                                                           // ����ʧ��
        }

        mask = tcsbus_disable_int();
        tcsbus_para[tcsbus_name].tx_buf = data;
        tcsbus_para[tcsbus_name].tx_rx_flag = TX_READY;
        if (type & 0x01) tcsbus_para[tcsbus_name].tx_rx_flag |= TX_RX_LONG;     // ��AЭ��
        tcsbus_enable_int(mask);
        return 0;
    }
    else                                                                        // ׼������QЭ��
    {
        type &= 0x0F;
        if (type == 0x0F) return 1;                                             // ��֧�ֵķ�������

        mask = tcsbus_disable_int();
        tcsbus_para[tcsbus_name].tx_buf = data;
        tcsbus_para[tcsbus_name].tx_rx_flag = TX_READY | TX_Q_READY;
        if (type & 0x01) tcsbus_para[tcsbus_name].tx_rx_flag |= TX_RX_LONG;     // ��QЭ��
        else             tcsbus_para[tcsbus_name].tx_buf <<= 4;                 // ��QЭ��, ��������
        tcsbus_para[tcsbus_name].wait_timer = 30;                               // 6ms����QЭ�飬
        tcsbus_enable_int(mask);
        return 0;
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
