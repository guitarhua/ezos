/**
  ******************************************************************************
  * @file    doxygen_demo.h
  * @author  mousie
  * @version V1.0.0
  * @date    2010.3.24
  * @brief   使用doxygen格式的头文件注释示例, 可作为软件注释的标准模板
  *          使用doxygen可以快速生成规范的软件说明文档.
  *          更多关于doxygen的信息请自行搜索查阅.
  *
  *          C语言编程规范使用linux风格.
  *
  *          版本记录使用如下格式:
  * -V1.0.1, 2010.3.25, mousie
  *          - 改动1
  *          - 改动2
  * -V1.0.0, 2010.3.24, mousie
  *          - 改动1
  *          - 改动2
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DOXYGEN_DEMO_H
#define __DOXYGEN_DEMO_H

/* Include -------------------------------------------------------------------*/
#include "somefile.h"



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup Doxygen_Demo
  * @{
  */



/**
  ******************************************************************************
  * @defgroup Doxygen_Demo_Configure
  * @brief    用户配置
  ******************************************************************************
  * @{
  */
#define __EZOS_TASK_NUM                 16                                      ///< 设置最大任务数
/**
  * @}
  */



/** @defgroup Doxygen_Demo_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */
/// 定义任务操作的状态
typedef enum
{
    EZTASK_OK                           = (0x00),                               ///< 任务相关操作成功
    EZTASK_ERROR                        = (0x01),                               ///< 任务相关操作失败
    EZTASK_EXIST                        = (0x02),                               ///< 任务已存在
} ezos_status_t;
/**
  * @}
  */

/** @defgroup Doxygen_Demo_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */
#define __TASK_PROC                     void                                    ///< 函数标示
/**
  * @}
  */

/** @defgroup Doxygen_Demo_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup Doxygen_Demo_Public_Function
  * @brief    定义公有函数
  * @{
  */
void demo_init(void);                                                           ///< 初始化函数
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
