/**
  ******************************************************************************
  * @file    user_types.h
  * @author  mousie
  * @version V2.0.0
  * @date    2011.12.16
  * @brief   用户数据类型定义
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USER_TYPES_H
#define __USER_TYPES_H

/* Include -------------------------------------------------------------------*/
#include <stdint.h>



/** @addtogroup Applications
  * @{
  */
/** @addtogroup Types
  * @{
  */



/** @defgroup Types_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */
// BOOL值, 标准用FALSE, TRUE. 如果这两个值已经被宏定义, 则使用uint8_t代替.
//typedef enum {FALSE = 0, TRUE = !FALSE} bool_t;
typedef uint8_t                         bool_t;

// 定义MCU位宽, 局部变量使用与CPU位宽相同的类型可提高优化度
typedef int32_t                         base_t;
typedef uint32_t                        ubase_t;

// 自定义
typedef signed char                     CHAR;
typedef unsigned char                   UCHAR;
typedef unsigned char                   BYTE;

typedef int                             INT;
typedef unsigned int                    UINT;

typedef short                           SHORT;
typedef unsigned short                  USHORT;
typedef unsigned short                  WORD;
typedef unsigned short                  WCHAR;

typedef long                            LONG;
typedef unsigned long                   ULONG;
typedef unsigned long                   DWORD;

typedef int8_t                          INT8S;
typedef int16_t                         INT16S;
typedef int32_t                         INT32S;
typedef int64_t                         INT64S;
typedef uint8_t                         INT8U;
typedef uint16_t                        INT16U;
typedef uint32_t                        INT32U;
typedef uint64_t                        INT64U;

typedef int8_t                          s8;
typedef int8_t                          S8;
typedef int8_t                          S08;
typedef uint8_t                         u8;
typedef uint8_t                         U8;
typedef uint8_t                         U08;

typedef int16_t                         s16;
typedef int16_t                         S16;
typedef uint16_t                        u16;
typedef uint16_t                        U16;

typedef int32_t                         s32;
typedef int32_t                         S32;
typedef uint32_t                        u32;
typedef uint32_t                        U32;

typedef int64_t                         s64;
typedef int64_t                         S64;
typedef uint64_t                        u64;
typedef uint64_t                        U64;

#ifndef NULL
#define NULL    0
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
