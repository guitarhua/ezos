/**
  ******************************************************************************
  * @file    drv_tcsbus.c
  * @author  mousie
  * @version V2.0.0
  * @date    2012.10.26
  * @brief   TCS BUS 底层驱动. 未测试
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
#define tcsbus_enable_int(mask)         __set_PRIMASK(mask)                     ///< 宏定义TCSBUS中断打开函数, 方便代码移植
#define tcsbus_disable_int()            __get_PRIMASK(); __disable_irq()        ///< 宏定义TCSBUS中断关闭函数, 方便代码移植
/**
  * @}
  */



/** @defgroup DRV_TCSBUS_Private_TypeDefine
  * @brief    私有类型定义
  * @{
  */
/// TCSBUS硬件参数类型定义
typedef struct
{
    const uint32_t TX_CLK;                                                      ///< TCSBUS发送IO的时钟模块
    const uint32_t RX_CLK;                                                      ///< TCSBUS接收IO的时钟模块
    GPIO_TypeDef *const TX_PORT;                                                ///< TCSBUS发送IO的PORT口
    GPIO_TypeDef *const RX_PORT;                                                ///< TCSBUS接收IO的PORT口
    const uint16_t TX_PIN;                                                      ///< TCSBUS发送IO的PIN口
    const uint16_t RX_PIN;                                                      ///< TCSBUS接收IO的PIN口
    void (*rx_callback)(tcsbus_type_t type, uint32_t data);                     ///< TCSBUS接收中断回调函数
} tcsbus_hw_t;

/// TCSBUS参数类型定义
typedef struct
{
    uint32_t rx_buf;                                                            ///< TCSBUS 判断重复接收的协议
    volatile uint32_t tx_buf;                                                   ///< TCSBUS 发送缓存器
    volatile uint32_t tx_rx_buf;                                                ///< TCSBUS 收发缓存器
    volatile uint32_t rx_level;                                                 ///< 记录 RX 引脚历史值
    volatile uint16_t tx_rx_flag;                                               ///< 收发标志
    volatile uint8_t  parity;                                                   ///< 校验值
    volatile uint8_t  start_timer;                                              ///< 记录启动位时间长度/2的值(一般为6ms/0.2ms/2=15左右)
    volatile uint8_t  protocal_length;                                          ///< 位长度计数器
    volatile uint8_t  bit_timer;                                                ///< 收发定时器时间长度，用于判断0,1
    volatile uint8_t  wait_timer;                                               ///< 准备发送时间设置
    volatile uint8_t  q_timer;                                                  ///< 接收Q协议限时
    volatile uint8_t  r_timer;                                                  ///< 接收重复发送协议的限时
} tcsbus_para_t;
/**
  * @}
  */

/** @defgroup DRV_TCSBUS_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */
/// tx_rx_flag 标记位宏定义
#define TX_ACTIVE                       0x0001                                  ///< Sender aktiv             置位表示发送已经激活
#define TX_READY                        0x0002                                  ///< Senden starten           置位表示需要发送信息
#define TX_REPEAT                       0x0004                                  ///< Sendewiederholung        置位表示需要重新发送信息
#define TX_Q_READY                      0x0008                                  ///< Quittung senden          置位表示需要发送Q协议

#define RX_ACTIVE                       0x0010                                  ///< Empfang aktiv            置位表示接收已经激活
#define RX_START                        0x0020                                  ///< Startimp. empf.          置位表示接收到6ms低电平的启动标记
#define RX_LENGTH                       0x0040                                  ///< Laengenbit empf.         置位表示接收的是长度标记
#define TX_RX_LONG                      0x0080                                  ///< Info lang                置位表示发送接收的是长协议(32bit)
#define RX_FINISH                       0x0100                                  ///< Info empfangen           置位表示接收信息包完成，正确无误
#define RX_Q_NEED                       0x0200                                  ///< Quittungsempfang         置位表示要接收Q协议
#define RX_Q_FINISH                     0x0400                                  ///< Quittung wurde empfangen 置位表示接收Q协议包完成，正确无误
/**
  * @}
  */

/** @defgroup DRV_TCSBUS_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */
/// TCSBUS硬件参数映射表
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

/// TCSBUS参数表
static tcsbus_para_t tcsbus_para[TCSBUS_NUM];
/**
  * @}
  */

/** @defgroup DRV_TCSBUS_Private_Function
  * @brief    定义私有函数
  * @{
  */
static void tcsbus_tx_200us(tcsbus_name_t tcsbus_name);                         ///< TCSBUS 发送轮询函数
static void tcsbus_rx_200us(tcsbus_name_t tcsbus_name);                         ///< TCSBUS 接收轮询函数

/// TCSBUS PORT口初始化
__INLINE static void tcsbus_port_init(tcsbus_name_t tcsbus_name)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(tcsbus_hw[tcsbus_name].TX_CLK | tcsbus_hw[tcsbus_name].RX_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin   = tcsbus_hw[tcsbus_name].TX_PIN;
    GPIO_Init(tcsbus_hw[tcsbus_name].TX_PORT, &GPIO_InitStructure);             // 设置TCSBUS TX引脚, 推挽输出
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin   = tcsbus_hw[tcsbus_name].RX_PIN;
    GPIO_Init(tcsbus_hw[tcsbus_name].RX_PORT, &GPIO_InitStructure);             // 设置TCSBUS RX引脚, 高阻输入
}

/// TCSBUS TX 电平置高
__INLINE static void TCSBUS_TX_HIGH(tcsbus_name_t tcsbus_name)
{
    tcsbus_hw[tcsbus_name].TX_PORT->BSRR  = tcsbus_hw[tcsbus_name].TX_PIN;
}

/// TCSBUS TX 电平置低
__INLINE static void TCSBUS_TX_LOW(tcsbus_name_t tcsbus_name)
{
    tcsbus_hw[tcsbus_name].TX_PORT->BRR = tcsbus_hw[tcsbus_name].TX_PIN;
}

/// 读取 TCSBUS TX 电平值
__INLINE static int32_t TCSBUS_TX_LEVEL(tcsbus_name_t tcsbus_name)
{
    return (tcsbus_hw[tcsbus_name].TX_PORT->IDR & tcsbus_hw[tcsbus_name].TX_PIN);
}

/// 读取 TCSBUS RX 电平值
__INLINE static int32_t TCSBUS_RX_LEVEL(tcsbus_name_t tcsbus_name)
{
    return (tcsbus_hw[tcsbus_name].RX_PORT->IDR & tcsbus_hw[tcsbus_name].RX_PIN);
}
/**
  * @}
  */



/** @defgroup DRV_TCSBUS_Function
  * @brief    函数原文件
  * @{
  */

/**
  ******************************************************************************
  * @brief  初始化所有 TCSBUS 总线
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
  * @brief  初始化指定 TCSBUS 总线
  * @param  tcsbus_name
  *   @arg  可用值请参考 tcsbus_name_t 类型定义中的枚举值
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
  * @brief  TCSBUS 200us定时扫描函数
  * @param  tcsbus_name
  *   @arg  可用值请参考 tcsbus_name_t 类型定义中的枚举值
  * @retval none
  ******************************************************************************
  */
void tcsbus_scan_200us(tcsbus_name_t tcsbus_name)
{
    if (tcsbus_para[tcsbus_name].wait_timer) tcsbus_para[tcsbus_name].wait_timer--;
    if (tcsbus_para[tcsbus_name].r_timer)    tcsbus_para[tcsbus_name].r_timer--;

    // 不是接收Q协议，未激活接收，需要发送信息，发送间隔时间到 则发送，否则接收。
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
  * @brief  TCS BUS 协议发送函数
  * @param  tcsbus_name
  *   @arg  可用值请参考 tcsbus_name_t 类型定义中的枚举值
  * @retval none
  ******************************************************************************
  */
void tcsbus_tx_200us(tcsbus_name_t tcsbus_name)
{
    uint8_t bit_len;
    uint32_t temp;

    if (tcsbus_para[tcsbus_name].tx_rx_flag & TX_ACTIVE)                        // 发送已激活
    {
        tcsbus_para[tcsbus_name].bit_timer--;

        // 拉低电平失败(A线与P线短路)
        if ((!TCSBUS_RX_LEVEL(tcsbus_name)) && (tcsbus_para[tcsbus_name].bit_timer == 29))
        {
            TCSBUS_TX_LOW(tcsbus_name);                                         // 总线电平恢复为25V，保护稳压二极管。
            tcsbus_para[tcsbus_name].tx_rx_flag &= ~(TX_ACTIVE + TX_READY);
        }
        // 输出25V但输入为8V或0V(有别的总线设备在发送信息)
        else if ((!TCSBUS_TX_LEVEL(tcsbus_name)) && TCSBUS_RX_LEVEL(tcsbus_name))
        {
            if(tcsbus_para[tcsbus_name].tx_rx_flag & TX_Q_READY)                // 有Q协议要发送
            {
                tcsbus_para[tcsbus_name].tx_rx_flag = RX_Q_NEED;                // 设接收的是Q协议
                tcsbus_para[tcsbus_name].q_timer = 0;
            }
            else
            {
                tcsbus_para[tcsbus_name].tx_rx_flag = 0;
                tcsbus_para[tcsbus_name].wait_timer = 100;                      // 20ms后才能进入发送状态
            }
        }
        else if (tcsbus_para[tcsbus_name].bit_timer == 0)                       // 发送完一位
        {
            if (TCSBUS_TX_LEVEL(tcsbus_name))
            {
                TCSBUS_TX_LOW(tcsbus_name);                                     // 输出反向
            }
            else
            {
                TCSBUS_TX_HIGH(tcsbus_name);
            }
            if (tcsbus_para[tcsbus_name].tx_rx_flag & TX_RX_LONG)               // 发送的是长协议
            {
                temp = 1;
                bit_len = 32;
            }
            else                                                                // 发送的是短协议
            {
                temp = 0;
                if (tcsbus_para[tcsbus_name].tx_rx_flag & TX_Q_READY)
                    bit_len = 4;                                                // 发送的是Q信号，短协议
                else
                    bit_len = 16;
            }
            if (    (tcsbus_para[tcsbus_name].protocal_length <= bit_len)
                 && (tcsbus_para[tcsbus_name].protocal_length > 0) )            // 发送数据处理和校验位设置
            {
                temp = tcsbus_para[tcsbus_name].tx_rx_buf & 0x80000000;
                tcsbus_para[tcsbus_name].tx_rx_buf <<= 1;
                if (temp != 0) tcsbus_para[tcsbus_name].parity ^= 1;
            }
            else if (tcsbus_para[tcsbus_name].protocal_length == (bit_len+1))   // 发送最末位的校验位
            {
                temp = tcsbus_para[tcsbus_name].parity;
            }
            else if (tcsbus_para[tcsbus_name].protocal_length > (bit_len+1))    // 发送结束
            {
                tcsbus_para[tcsbus_name].tx_rx_flag &= ~TX_ACTIVE;              // 关闭发送激活标记
                if (tcsbus_para[tcsbus_name].tx_rx_flag & TX_Q_READY)           // 如果发的是Q协议
                {
                    tcsbus_para[tcsbus_name].tx_rx_flag &= ~(TX_READY | TX_Q_READY);
                    tcsbus_para[tcsbus_name].wait_timer = 100;
                }
                else                                                            // 发的是A协议
                {
                    tcsbus_para[tcsbus_name].tx_rx_flag |= RX_Q_NEED;           // 准备接收Q协议
                    tcsbus_para[tcsbus_name].q_timer = 0;
                }
            }
            if (temp) tcsbus_para[tcsbus_name].bit_timer = 20;                  // 数据为1，电平保持4ms
            else      tcsbus_para[tcsbus_name].bit_timer = 10;                  // 数据为0，电平保持2ms
            tcsbus_para[tcsbus_name].protocal_length++;                         // 数据长度+1
        }
    }
    else                                                                        // 刚进入发送状态
    {
        if (TCSBUS_RX_LEVEL(tcsbus_name))                                       // 总线电平为 8V，总线忙，别的设备在发送信息
        {
            if (tcsbus_para[tcsbus_name].tx_rx_flag & TX_Q_READY)               // 需要发送Q协议，但别的设备已经发送了Q协议
            {
                tcsbus_para[tcsbus_name].tx_rx_flag = RX_Q_NEED;                // 接收别的设备的Q协议
                tcsbus_para[tcsbus_name].q_timer = 0;
            }
            else
            {
                tcsbus_para[tcsbus_name].tx_rx_flag = 0;
                tcsbus_para[tcsbus_name].wait_timer = 120;                      // 24ms后才允许启动发送
            }
            return;                                                             // 总线忙，暂时不做发射信息处理
        }
        else                                                                    // 总线空闲，激活发送状态
        {
            //// 此处可增加函数, 发送协议时暂时关闭扬声器 200ms
            TCSBUS_TX_HIGH(tcsbus_name);                                        // 发送启动位,将总线电平拉低为8V
            tcsbus_para[tcsbus_name].bit_timer = 30;                            // 发送启动电平时间 30*0.2ms=6ms
            tcsbus_para[tcsbus_name].protocal_length = 0;                       // 信息位长度清零
            tcsbus_para[tcsbus_name].parity = 1;
            tcsbus_para[tcsbus_name].tx_rx_buf = tcsbus_para[tcsbus_name].tx_buf;
            tcsbus_para[tcsbus_name].tx_rx_flag |= TX_ACTIVE;                   // 激活发送状态
        }
    }
}

/**
  ******************************************************************************
  * @brief  TCS BUS 协议接收函数
  * @param  tcsbus_name
  *   @arg  可用值请参考 tcsbus_name_t 类型定义中的枚举值
  * @retval none
  ******************************************************************************
  */
void tcsbus_rx_200us(tcsbus_name_t tcsbus_name)
{
    uint8_t temp, bit_len;

    if (tcsbus_para[tcsbus_name].tx_rx_flag & RX_ACTIVE)                        // 接收已激活
    {
        tcsbus_para[tcsbus_name].bit_timer++;                                   // 记录时间长度
        if (TCSBUS_RX_LEVEL(tcsbus_name) == tcsbus_para[tcsbus_name].rx_level)  // TCS BUS 边沿没有变化
        {
            // 大于启动位长度, 属于异常情况
            temp = (tcsbus_para[tcsbus_name].start_timer << 1);
            if (tcsbus_para[tcsbus_name].bit_timer > temp)
            {
                tcsbus_para[tcsbus_name].tx_rx_flag = 0;
                tcsbus_para[tcsbus_name].wait_timer = 50;                       // 10ms后允许发送
            }
        }
        else                                                                    // 边沿发生变化
        {
            tcsbus_para[tcsbus_name].rx_level = TCSBUS_RX_LEVEL(tcsbus_name);
            temp = tcsbus_para[tcsbus_name].start_timer;
            if (tcsbus_para[tcsbus_name].bit_timer > temp)
                temp = 1;                                                       // 时间=启动电平时间的2/3，接收到的数据为1
            else
                temp = 0;                                                                   // 时间=启动电平时间的1/3，接收到的数据为0
            tcsbus_para[tcsbus_name].bit_timer=0;                               // 时间长度重新计算
            if (tcsbus_para[tcsbus_name].tx_rx_flag & RX_LENGTH)                // 是协议长度位
            {
                tcsbus_para[tcsbus_name].tx_rx_flag &= ~RX_LENGTH;
                if (temp == 1) tcsbus_para[tcsbus_name].tx_rx_flag |= TX_RX_LONG;
                else           tcsbus_para[tcsbus_name].tx_rx_flag &= ~TX_RX_LONG;
            }
            else                                                                // 数据位
            {
                if (tcsbus_para[tcsbus_name].tx_rx_flag & TX_RX_LONG)
                {
                    bit_len = 32;                                               // 长协议32bit
                }
                else
                {
                    bit_len = 16;                                               // 短协议16bit
                    if (tcsbus_para[tcsbus_name].tx_rx_flag & RX_Q_NEED)
                        bit_len = 4;                                            // 是Q协议，4bit
                }
                if (tcsbus_para[tcsbus_name].protocal_length < bit_len)         // 未接收完整个信息包
                {
                    tcsbus_para[tcsbus_name].tx_rx_buf <<= 1;                   // 数据信息整理和校验
                    if (temp == 1)
                    {
                        tcsbus_para[tcsbus_name].tx_rx_buf |= 1;
                        tcsbus_para[tcsbus_name].parity ^= 1;
                    }
                }
                else                                                            // 接收完信息包
                {
                    if (tcsbus_para[tcsbus_name].parity == temp)                // 校验正确
                    {
                        tcsbus_para[tcsbus_name].tx_rx_flag |= RX_FINISH;       // 表示已经成功接受到信息
                        if (tcsbus_para[tcsbus_name].tx_rx_flag & RX_Q_NEED)    // 如果是接收的是Q协议
                        {
                            tcsbus_para[tcsbus_name].tx_rx_flag |= RX_Q_FINISH; // 表示已经收到Q协议
                            tcsbus_para[tcsbus_name].tx_rx_flag &= ~RX_Q_NEED;  // 取消Q协议标记(接收的下一个协议不会是Q协议)
                            if (tcsbus_hw[tcsbus_name].rx_callback != 0)
                            {
                                if (tcsbus_para[tcsbus_name].tx_rx_flag & TX_RX_LONG)
                                    tcsbus_hw[tcsbus_name].rx_callback(Q_LONG, tcsbus_para[tcsbus_name].tx_rx_buf);
                                else                                            // Q短协议
                                    tcsbus_hw[tcsbus_name].rx_callback(Q_SHORT, tcsbus_para[tcsbus_name].tx_rx_buf);
                            }
                            tcsbus_para[tcsbus_name].wait_timer = 50;           // 10ms后才允许发送程序
                            tcsbus_para[tcsbus_name].r_timer    = 0;            // 正确接收应答协议, 下一条接收的协议必须处理
                            if(tcsbus_para[tcsbus_name].tx_rx_flag & TX_READY)  // 信息发送标记 (此处正常情况都是有信息发送标记的)
                            {
                                tcsbus_para[tcsbus_name].tx_rx_flag  &= ~(TX_READY | TX_REPEAT);                                               // 接收完Q协议后取消发送与重发标记
                                tcsbus_para[tcsbus_name].wait_timer = 30;       // 6ms后才允许发送程序
                            }
                        }
                        else                                                    // 接收到A协议
                        {
                            tcsbus_para[tcsbus_name].tx_rx_flag |= RX_Q_NEED;   // 置位要接收Q协议标记(下一个可能是Q协议，q_timer超时后会变为普通协议)
                            tcsbus_para[tcsbus_name].q_timer = 0;               // 接收Q协议限时器清零
                            if (tcsbus_hw[tcsbus_name].rx_callback != 0)
                            {
                                if ((tcsbus_para[tcsbus_name].r_timer == 0) || (tcsbus_para[tcsbus_name].rx_buf != tcsbus_para[tcsbus_name].tx_rx_buf))
                                {
                                    if (tcsbus_para[tcsbus_name].tx_rx_flag & TX_RX_LONG)
                                        tcsbus_hw[tcsbus_name].rx_callback(A_LONG, tcsbus_para[tcsbus_name].tx_rx_buf);
                                    else                                        // A短协议
                                        tcsbus_hw[tcsbus_name].rx_callback(A_SHORT, tcsbus_para[tcsbus_name].tx_rx_buf);
                                }
                            }
                            tcsbus_para[tcsbus_name].r_timer = 100;
                            tcsbus_para[tcsbus_name].rx_buf  = tcsbus_para[tcsbus_name].tx_rx_buf;
                        }
                    }
                    tcsbus_para[tcsbus_name].tx_rx_flag &= ~RX_ACTIVE;          // 接收结束，进入休眠状态
                }
                tcsbus_para[tcsbus_name].protocal_length++;                     // 接收位长度加一，准备接收下一位信息
            }
        }
    }
    else                                                                        // 接收未激活，休眠状态
    {
        if (TCSBUS_RX_LEVEL(tcsbus_name))                                       // 空闲状态时，TCS BUS 电平翻转为 8V, 总线上有协议
        {
            if (tcsbus_para[tcsbus_name].tx_rx_flag & RX_START)                 // 是启动位
            {
                tcsbus_para[tcsbus_name].bit_timer++;                           // 计算时间长度
            }
            else                                                                // 电平拉低，接收信号开始
            {
                //// 此处可增加函数, 接收协议时禁止扬声器 200ms
                tcsbus_para[tcsbus_name].tx_rx_flag |= RX_START;                // 标记为启动位
                tcsbus_para[tcsbus_name].bit_timer = 0;
            }
        }
        else                                                                    // TCS BUS 是 25V
        {
            if (tcsbus_para[tcsbus_name].tx_rx_flag & RX_START)                 // 启动位判断结束
            {
                // 准备进入协议长度位判断,接收已经激活
                tcsbus_para[tcsbus_name].tx_rx_flag |=  (RX_LENGTH | RX_ACTIVE);
                tcsbus_para[tcsbus_name].tx_rx_flag &= ~(RX_START | RX_FINISH | RX_Q_FINISH | TX_Q_READY);                                              // 一系列初始化
                tcsbus_para[tcsbus_name].start_timer  = tcsbus_para[tcsbus_name].bit_timer >> 1;                                                // 记录启动位时间长度/2的值，用于判断0,1
                tcsbus_para[tcsbus_name].bit_timer  = 0;
                tcsbus_para[tcsbus_name].rx_level    = TCSBUS_RX_LEVEL(tcsbus_name);
                tcsbus_para[tcsbus_name].tx_rx_buf = 0;                         // 清收信息寄存器
                tcsbus_para[tcsbus_name].parity   = 1;
                tcsbus_para[tcsbus_name].protocal_length  = 0;                  // 接收位长度清零
            }
            if (tcsbus_para[tcsbus_name].tx_rx_flag & RX_Q_NEED)                // 要接收Q协议
            {
                tcsbus_para[tcsbus_name].q_timer++;                             // Q协议限时记录
                if (tcsbus_para[tcsbus_name].q_timer >= 50)                     // Q协议超时，50*0.2ms=10ms
                {
                    tcsbus_para[tcsbus_name].tx_rx_flag  &= ~(RX_Q_NEED | RX_Q_FINISH);                                                     // 等待接收Q协议超时，接收Q协议标记清零
                    tcsbus_para[tcsbus_name].wait_timer = 50;                   // 设置发送间隔时间，10ms后允许发送
                    if (tcsbus_para[tcsbus_name].tx_rx_flag & TX_READY)         // 有信息待发送
                    {
                        if (tcsbus_para[tcsbus_name].tx_rx_flag & TX_REPEAT)    // 是重发信息状态(发两遍无Q协议应答的状态)
                        {
                            tcsbus_para[tcsbus_name].tx_rx_flag &= ~(TX_READY | TX_REPEAT);                                                  // 发送标记清零，放弃发送
                            tcsbus_para[tcsbus_name].wait_timer = 100;          // 20ms后才可以再启动发送
                            if (tcsbus_hw[tcsbus_name].rx_callback != 0)        // 等待Q协议超时
                            {
                                tcsbus_hw[tcsbus_name].rx_callback(Q_NONE, 0);
                            }
                        }
                        else                                                    // 发送的信息无Q协议应答，准备重发
                        {
                            tcsbus_para[tcsbus_name].tx_rx_flag |= TX_REPEAT;   // 置位重发标记
                        }
                    }
                }
            }
        }
    }
}

/**
  ******************************************************************************
  * @brief  TCS BUS 协议发送准备函数
  * @param  tcsbus_name
  *   @arg  可用值请参考 tcsbus_name_t 类型定义中的枚举值
  * @param  type, 要发送的协议类型
  *   @arg  A_LONG,  准备发送A长协议, 32bit
  *   @arg  A_SHORT, 准备发送A短协议, 16bit
  *   @arg  Q_LONG,  准备发送Q长协议, 32bit
  *   @arg  Q_SHORT, 准备发送Q短协议, 4bit
  * @param  data, 要发送的数据
  * @retval 返回值, 0-准备发送成功, 1-准备发送失败
  ******************************************************************************
  */
uint8_t tcsbus_tx(tcsbus_name_t tcsbus_name, tcsbus_type_t type, uint32_t data)
{
    uint32_t mask;

    if ((type & 0x10) == 0x00)                                                  // 准备发送A协议
    {
        // 正在处理收发过程
        if ((tcsbus_para[tcsbus_name].wait_timer) || (tcsbus_para[tcsbus_name].tx_rx_flag & (TX_ACTIVE | TX_READY | RX_ACTIVE | RX_START | RX_Q_NEED)))
        {
            return 1;                                                           // 发送失败
        }

        mask = tcsbus_disable_int();
        tcsbus_para[tcsbus_name].tx_buf = data;
        tcsbus_para[tcsbus_name].tx_rx_flag = TX_READY;
        if (type & 0x01) tcsbus_para[tcsbus_name].tx_rx_flag |= TX_RX_LONG;     // 长A协议
        tcsbus_enable_int(mask);
        return 0;
    }
    else                                                                        // 准备发送Q协议
    {
        type &= 0x0F;
        if (type == 0x0F) return 1;                                             // 不支持的发送类型

        mask = tcsbus_disable_int();
        tcsbus_para[tcsbus_name].tx_buf = data;
        tcsbus_para[tcsbus_name].tx_rx_flag = TX_READY | TX_Q_READY;
        if (type & 0x01) tcsbus_para[tcsbus_name].tx_rx_flag |= TX_RX_LONG;     // 长Q协议
        else             tcsbus_para[tcsbus_name].tx_buf <<= 4;                 // 短Q协议, 调整数据
        tcsbus_para[tcsbus_name].wait_timer = 30;                               // 6ms后发送Q协议，
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
