#ifndef _FAULT_HANDLER_H
#define _FAULT_HANDLER_H


#include "device.h"

/* 定义故障类型枚举，方便以后扩展 */
typedef enum {
    FAULT_NONE = 0,
    FAULT_OVER_CURRENT,   // 过流 (COMP 触发)
    FAULT_OVER_VOLTAGE,   // 过压
    FAULT_UNDER_VOLTAGE,  // 欠压
    FAULT_OVER_HEAT       // 过温
} Motor_Fault_t;

extern void FH_InitOverCurr(void);



#endif
