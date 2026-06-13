/**
 * @file    VF.h
 * @brief   V/F 开环控制 + SVPWM 实现 (适用于 STM32G4)
 * @note    电压/频率比控制, 10kHz PWM 中断调用 VF_step()
 */

#ifndef __MYVF_H
#define __MYVF_H

#include <stdint.h>
#include <math.h>
#include "arm_math.h"   /* CMSIS-DSP, 提供 arm_sin_f32 / arm_cos_f32 */

/* ========================== 配置参数 ========================== */
#define VF_PWM_FREQ_HZ       10000.0f     /* PWM 频率 */
#define VF_TIM_ARR           8000.0f      /* TIM 自动重装载值 */
#define VF_DT                (1.0f / VF_PWM_FREQ_HZ)  /* 中断周期: 0.0001s */

/* ========================== 接口结构体 ========================== */

/** V/F 控制器状态 */
typedef struct {
    float angle;      /* 当前电角度 (rad), [0, 2pi) */
} VF_Handle_t;

/** V/F 输入参数 (由外部每步更新) */
typedef struct {
    float ud;         /* d 轴电压 (V), 开环通常设为 0 */
    float uq;         /* q 轴电压 (V), 从 V/F 曲线获取 */
    float freq;       /* 目标电频率 (Hz) */
    float vbus;       /* 母线电压 (V) */
} VF_Input_t;

/* ========================== 函数声明 ========================== */

/**
 * @brief   V/F 控制器初始化
 * @param   h  控制句柄
 */
void VF_Init(VF_Handle_t *h);

/**
 * @brief   V/F 控制器步进函数 (每 100us 调用一次)
 * @param   h    控制句柄
 * @param   in   输入 (ud, uq, freq, vbus)
 * @param   out  输出三相占空比数组 [0..2], 范围 [0, TIM_ARR]
 */
void myVF_Step(VF_Handle_t *h, VF_Input_t *in, float out[3]);


extern VF_Input_t  vfin;
extern VF_Handle_t vf;
extern float VF_PWMLoad[3];

extern void VF_configPara(VF_Input_t *vfc);

#endif /* __VF_H */
