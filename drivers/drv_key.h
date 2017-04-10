/**
  ******************************************************************************
  * @file    drv_key.h
  * @author  mousie
  * @version V2.1.1
  * @date    2015.4.21
  * @brief   直连式按键驱动程序.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_KEY_H
#define __DRV_KEY_H

/* Include -------------------------------------------------------------------*/
#include <stdint.h>

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup DRV_Key
  * @{
  */



/**
  ******************************************************************************
  * @defgroup DRV_Key_Configure
  * @brief    用户配置
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // 硬件集中配置文件

//#define KEY_NUM                         8                                       ///< 设置按键数量, 该驱动最多允许16个直按式按键.
//#define KEY_EXTPULL                                                             ///< 按键有外部上拉电阻
//
//#define KEY0_PORT                       GPIOC                                   ///< 设置该按键的PORT口
//#define KEY0_CLK                        RCC_APB2Periph_GPIOC                    ///< 设置该按键的时钟模块
//#define KEY0_PIN                        GPIO_Pin_13                             ///< 设置该按键的PIN口
//
//#define KEY1_PORT                       GPIOA
//#define KEY1_CLK                        RCC_APB2Periph_GPIOA
//#define KEY1_PIN                        GPIO_Pin_8
//
//#define KEY2_PORT                       GPIOD
//#define KEY2_CLK                        RCC_APB2Periph_GPIOD
//#define KEY2_PIN                        GPIO_Pin_0
//
//#define KEY3_PORT                       GPIOD
//#define KEY3_CLK                        RCC_APB2Periph_GPIOD
//#define KEY3_PIN                        GPIO_Pin_1
//
//#define KEY4_PORT                       GPIOD
//#define KEY4_CLK                        RCC_APB2Periph_GPIOD
//#define KEY4_PIN                        GPIO_Pin_2
//
//#define KEY5_PORT                       GPIOD
//#define KEY5_CLK                        RCC_APB2Periph_GPIOD
//#define KEY5_PIN                        GPIO_Pin_3
//
//#define KEY6_PORT                       GPIOD
//#define KEY6_CLK                        RCC_APB2Periph_GPIOD
//#define KEY6_PIN                        GPIO_Pin_4
//
//#define KEY7_PORT                       GPIOA
//#define KEY7_CLK                        RCC_APB2Periph_GPIOA
//#define KEY7_PIN                        GPIO_Pin_0
//
///// KEY名称类型定义
//typedef enum
//{
//    S2_TAMPER = 0,                                                              ///< 命名为易于理解的KEY名称, 值0对应KEY0_xxx
//    S4_USER,                                                                    ///< 值1对应KEY1_xxx, 以下类推
//    JOY_SEL,
//    JOY_DOWN,
//    JOY_LEFT,
//    JOY_RIGHT,
//    JOY_UP,
//    KEY7,
//} key_name_t;



#if (KEY_NUM > 16)
    #error "KEY_NUM define error!"
#endif
/**
  * @}
  */



/** @defgroup DRV_Key_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */
/// 按键状态类型定义
typedef struct
{
    uint16_t on_off;                                                            ///< 当前按键开关值
    uint16_t touch;                                                             ///< 短按触发标示
    uint16_t click;                                                             ///< 按键快速释放标示
    uint16_t lclick;                                                            ///< 长按触发标示
    uint16_t lpress;                                                            ///< 连发触发标示
} key_status_t;
/**
  * @}
  */

/** @defgroup DRV_Key_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */
#define KEY_MASK(key_name)              (1 << key_name)                         ///< 按键取模, 对应到bit
/**
  * @}
  */

/** @defgroup DRV_Key_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */
/**
  * @brief 使用全局变量的方式提供直按式按键返回值
  *
  *   @arg KeyStatus.on_off,  按键开关状态寄存器.      对应bit.            0,无按键. 1,有按键
  *   @arg KeyStatus.touch,   按键触发状态寄存器.      按键点击,           对应bit置位
  *   @arg KeyStatus.click,   按键快速释放状态寄存器.  按键快速释放,       对应bit置位
  *   @arg KeyStatus.lclick,  按键长按变化状态寄存器.  按键长按,           对应bit置位
  *   @arg KeyStatus.lpress,  按键在进入长按状态后.    连发效果,           对应bit置位
  *
  * @note 按键与上述参数的对应关系由 keyName_t 的枚举值决定按键bit位, 最多16个按键
  *       按键0示例：
  * ______________--------------------------------------------_________________________
  *     |         |<- 0.2S ->|<-    1.1S   ->|<- 0.2S ->|<- 0.2 S->|     |           |
  *  无按键     按下                        按着      按着      按着     释放       无按键
  * .on_off=0 .on_off=1  .on_off=1          .on_off=1 .on_off=1 .on_off=1 .on_off=0  .on_off=0
  * .touch=0  .touch=1                                          .touch=0   .touch=0
  * .click=0             .click=1(若1.3S内释放)                 .click=0  .click=0
  * .lclick=0                               .lclick=1 .lclick=0 .lclick=0 .lclick=0  .lclick=0
  * .lpress=0                               .lpress=1 .lpress=1 .lpress=1 .lpress=0  .lpress=0
  */
volatile extern key_status_t key_status;
/**
  * @}
  */

/** @defgroup DRV_Key_Public_Function
  * @brief    定义公有函数
  * @{
  */
#if (KEY_NUM > 0)

void key_init_all(void);                                                        // 初始化所有按键
void key_init(key_name_t key_name);                                             // 初始化指定按键
void key_scan_10ms(void);                                                       // 按键扫描函数
///void key_task_10ms(void);                                                    // 按键任务函数范例, 可复制到应用层使用

#endif
/**
  * @}
  */



/**
  * @}
  */
/**
  * @}
  */

#endif
/* END OF FILE ---------------------------------------------------------------*/
