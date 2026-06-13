/**
 * @file    VF.c
 * @brief   V/F 开环控制 + SVPWM 实现
 *
 * ============================ 原理说明 ============================
 * V/F (Volts per Hertz) 开环控制:
 *   - 对目标频率积分得到电角度: theta += 2*pi*freq*dt
 *   - 反 Park 变换: (Ud, Uq, theta) -> (Valpha, Vbeta)
 *   - SVPWM: (Valpha, Vbeta, Vbus) -> 三相占空比
 *
 * Ud/Uq 的取值:
 *   - Ud = 0 : 将电压矢量完全对齐到 q 轴 (假定转子与定子磁势同步)
 *   - Uq = VF_Curve(freq) : 电压幅值与频率成正比, 保持磁通恒定
 *     在低频段通常需要额外提升 Uq 以克服定子电阻压降
 *
 * 信号流:
 *   Freq -> [积分] -> theta -> [sin/cos] -> [反Park] -> Valpha/Vbeta
 *   -> [反Clark] -> Ta/Tb/Tc -> [零序注入 + 归一化] -> 三相占空比
 * =================================================================
 */

#include "myVF.h"

VF_Handle_t vf = {0};
VF_Input_t  vfin = {0};
float VF_PWMLoad[3];

#define M_PI 3.14159f

/* ======================== 静态函数声明 ========================= */

/**
 * @brief  SVPWM: 将 Valpha/Vbeta 调制为三相 PWM 占空比
 * @param  valpha  alpha 轴电压 (V)
 * @param  vbeta   beta 轴电压  (V)
 * @param  vbus    母线电压 (V)
 * @param  tABC    输出三相占空比 [0..2]
 */
static void SVPWM_Calc(float valpha, float vbeta, float vbus, float tABC[3]);

/**
 * @brief  将角度限制在 [0, 2*pi) 范围内
 */
static float Angle_Wrap(float theta);

/* ======================== API 实现 ============================= */

void VF_Init(VF_Handle_t *h)
{
    h->angle = 0.0f;
}

void VF_configPara(VF_Input_t *vfc)
{
    vfc->ud = 0.0f;
    vfc->uq = 12.0f;
    vfc->freq = 3;
    vfc->vbus = 24;
    
}

void myVF_Step(VF_Handle_t *h,  VF_Input_t *in, float out[3])
{
    /* ---- 1. 角度积分: theta += 2*pi * freq * dt ---- */
    static const float K_ANGLE = 2.0f * (float)M_PI * VF_DT;  /* 2*pi*0.0001 */
    h->angle += K_ANGLE * in->freq;
    h->angle = Angle_Wrap(h->angle);

    /* ---- 2. 计算 sin / cos ---- */
    float sin_theta = arm_sin_f32(h->angle);
    float cos_theta = arm_cos_f32(h->angle);

    /* ---- 3. 反 Park 变换 (dq -> alpha-beta) ----
     *   Valpha = Ud * cos(theta) - Uq * sin(theta)
     *   Vbeta  = Ud * sin(theta) + Uq * cos(theta)
     */
    float valpha = in->ud * cos_theta - in->uq * sin_theta;
    float vbeta  = in->ud * sin_theta + in->uq * cos_theta;

    /* ---- 4. SVPWM: (Valpha, Vbeta) -> 三相占空比 ---- */
    SVPWM_Calc(valpha, vbeta, in->vbus, out);
}

/* ======================== 静态函数实现 ========================= */

/**
 * SVPWM 实现步骤:
 *   (1) 反 Clark 变换: (alpha, beta) -> (Ta, Tb, Tc)
 *       Ta = Valpha
 *       Tb = -0.5*Valpha + sqrt(3)/2 * Vbeta
 *       Tc = -0.5*Valpha - sqrt(3)/2 * Vbeta
 *
 *   (2) 零序注入 (中心对齐 SVPWM):
 *       Voffset = -0.5 * (min(Ta,Tb,Tc) + max(Ta,Tb,Tc))
 *       T'a = Ta + Voffset, etc.
 *
 *   (3) 归一化到 TIM 计数值:
 *       Duty = (-T'a / Vbus + 0.5) * TIM_ARR
 *       负号是因为 SVPWM 有反相器效应
 */
static void SVPWM_Calc(float valpha, float vbeta, float vbus, float tABC[3])
{
    /* 反 Clark 变换常量 */
    static const float INV_SQRT3_2 = 0.86602540378f;  /* sqrt(3)/2 */
    static const float HALF        = 0.5f;

    /* ---- (1) 反 Clark: alpha-beta -> 三相电压 ---- */
    float ta = valpha;
    float tb = -HALF * valpha + INV_SQRT3_2 * vbeta;
    float tc = -HALF * valpha - INV_SQRT3_2 * vbeta;

    /* ---- (2) 零序注入 ----
     *  标准 SVPWM: offset = -0.5*(max + min)
     *  使三相电压在 [-Vbus/2, Vbus/2] 内中心对称
     */
    float tmax = fmaxf(fmaxf(ta, tb), tc);
    float tmin = fminf(fminf(ta, tb), tc);
    float offset = -HALF * (tmax + tmin);

    ta += offset;
    tb += offset;
    tc += offset;

    /* ---- (3) 归一化到 PWM 占空比计数值 ----
     *  公式: duty = (-phase_voltage / vbus + 0.5) * TIM_ARR
     *  注意负号: 逆变器三相桥臂, 当 phase_voltage 为正时,
     *  上管导通时间减少 (低边调制), 这是硬件拓扑决定的
     */
    float inv_vbus = 1.0f / vbus;
    tABC[0] = (-ta * inv_vbus + HALF) * VF_TIM_ARR;
    tABC[1] = (-tb * inv_vbus + HALF) * VF_TIM_ARR;
    tABC[2] = (-tc * inv_vbus + HALF) * VF_TIM_ARR;
}

static float Angle_Wrap(float theta)
{
    static const float TWO_PI = 2.0f * (float)M_PI;

    if (theta > TWO_PI) {
        theta -= TWO_PI;
    } else if (theta < 0.0f) {
        theta += TWO_PI;
    }
    return theta;
}
