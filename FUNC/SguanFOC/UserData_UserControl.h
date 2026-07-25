#ifndef __USERDATA_USERCONTROL_H
#define __USERDATA_USERCONTROL_H
#include "SguanFOC.h"
#include "myVF.h"

/* 电机控制User用户设置·实时参数控制页面 */

/* 用户自己的CODE BEGIN Includes */

/* 用户自己的CODE END Includes */

static inline void User_UserControl(void){
    /* 仅传入需要实时控制的数据，如Target_Speed */
    // like: Sguan.foc.Target_Speed = 0.0f;
}

static inline void User_AO_Adjust(float AO){
    /* Your code for Parameter set */
    /* like: 
    switch (Sguan.mode){
    case 0x00:
        Sguan.foc.Uq_in = AO;
        break;
    case 0x01:
        Sguan.foc.Target_Iq = AO;
        break;
    case 0x02:
        Sguan.foc.Target_Speed = AO;
        break;
    case 0x03:
        Sguan.foc.Target_Pos = AO;
        break;
    default:
        break;
    }
    */
}

static inline void User_BO_Adjust(float BO){
    /* Your code for Parameter set */
    /* like: 
    if ((0.0f < BO) && (BO < 1.0f)){
        Sguan.mode = 0x00;
    }
    else if ((1.0f <= BO) && (BO < 2.0f)){
        Sguan.mode = 0x01;
    }
    else if ((2.0f <= BO) && (BO < 3.0f)){
        Sguan.mode = 0x02;
    }
    else if ((3.0f <= BO) && (BO < 10.0f)){
        Sguan.mode = 0x03;
    }
    */
}

static inline void User_CO_Adjust(float CO){
    /* Your code for Parameter set */
    /* like: 
    if (CO < 0.5f){
        Sguan.status = 0x16;
    }
    else{
        Sguan.status = 0x01;
    }
    */
}

static inline void User_UserTX(void){
    /* 仅传入主循环printf发送的数据，如TXdata.fdata[0],默认最多12个 */
    /* like:
    Sguan.TXdata.fdata[0] = Sguan.status;
    Sguan.TXdata.fdata[1] = Sguan.encoder.Real_Speed;
    Sguan.TXdata.fdata[2] = Sguan.foc.Target_Speed;
    Sguan.TXdata.fdata[3] = Sguan.current.Real_Id;
    Sguan.TXdata.fdata[4] = Sguan.current.Real_Iq;
    Sguan.TXdata.fdata[5] = Sguan.foc.Target_Id;
    Sguan.TXdata.fdata[6] = Sguan.foc.Target_Iq;
    Sguan.TXdata.fdata[7] = Sguan.foc.Uq_in;
    Sguan.TXdata.fdata[8] = Sguan.current.Real_Ia;
    Sguan.TXdata.fdata[9] = Sguan.encoder.Real_Pos;
    Sguan.TXdata.fdata[10] = Sguan.encoder.Pos_offset;
    Sguan.TXdata.fdata[11] = Sguan.mode;
    */
    // 传入串口printf要发送的数据，如txdata.fdata[0]，最多默认16个数值
    // 如需传入更多数值，请修改Sguan_printf中的参数
    // ========== 组1: 状态与核心指令 (0-3) ==========
    // Sguan.txdata.fdata[0] = (float)Sguan.status;        // 状态机当前状态码
    // Sguan.txdata.fdata[1] = Sguan.foc.Target_Speed;     // 目标机械转速 (rad/s)
    // Sguan.txdata.fdata[2] = Sguan.foc.Target_Iq;        // 目标Q轴电流 (A)
    // Sguan.txdata.fdata[3] = Sguan.foc.Target_Id;        // 目标D轴电流 (A)

    // // ========== 组2: 实际反馈 (4-7) ==========
    // Sguan.txdata.fdata[4] = Sguan.encoder.Real_Speed;   // 实际机械转速 (rad/s)
    // Sguan.txdata.fdata[5] = Sguan.current.Real_Iq;      // 实际Q轴电流 (A)
    // Sguan.txdata.fdata[6] = Sguan.current.Real_Id;      // 实际D轴电流 (A)
    // Sguan.txdata.fdata[7] = Sguan.encoder.Real_Pos;     // 实际机械角度 (rad)

    // // ========== 组3: 控制器输出与中间变量 (8-14) ==========
    // Sguan.txdata.fdata[8]  = Sguan.foc.Uq_in;           // Q轴电压指令 (V)
    // // Sguan.txdata.fdata[9]  = Sguan.foc.Ud_in;           // D轴电压指令 (V)
    // Sguan.txdata.fdata[9]  = Sguan.encoder.Real_Re;           // D轴电压指令 (V)
    // Sguan.txdata.fdata[10] = Sguan.foc.Real_VBUS;       // 母线电压实际值 (V)

    // Sguan.txdata.fdata[11] = Sguan.current.Real_Ia;  
    // Sguan.txdata.fdata[12] = Sguan.current.Real_Ib;  
    // Sguan.txdata.fdata[13] = Sguan.current.Real_Ic;           

    // // Sguan.txdata.fdata[11] = Sguan.foc.Du;              // U相占空比 (0~1)
    // // Sguan.txdata.fdata[12] = Sguan.foc.Dv;              // V相占空比 (0~1)
    // // Sguan.txdata.fdata[13] = Sguan.foc.Dw;              // W相占空比 (0~1)
    
    // // Sguan.txdata.fdata[11] = Sguan.encoder.Sensorless_offset;       // 母线电压实际值 (V)

    // // Sguan.txdata.fdata[11] = Sguan.motor.identify.Rs;              // W相占空比 (0~1)
    // // Sguan.txdata.fdata[12] = Sguan.motor.identify.Lq;              // W相占空比 (0~1)
    // // Sguan.txdata.fdata[13] = Sguan.motor.identify.Ld;              // W相占空比 (0~1)

    // // Sguan.txdata.fdata[12] = Sguan.encoder.Sensorless_Speed;
    // // Sguan.txdata.fdata[13] = Sguan.encoder.Sensorless_Pos;
    // // Sguan.txdata.fdata[14] = Sguan.encoder.Sensorless_We;
    // // Sguan.txdata.fdata[15] = Sguan.encoder.Sensorless_Re;

    // Sguan.txdata.fdata[14] = Sguan.current.Real_Ialpha;     // A相瞬时电流值 (A)
    // Sguan.txdata.fdata[15] = Sguan.current.Real_Ibeta;     // A相瞬时电流值 (A)

    Sguan.txdata.fdata[0] = Sguan.encoder.Real_Speed;



    // Sguan.txdata.fdata[1] = Sguan.encoder.Real_Re;
    // Sguan.txdata.fdata[2] = Sguan.foc.sine;
    // Sguan.txdata.fdata[3] = Sguan.foc.cosine;

    // Sguan.txdata.fdata[4] = vfin.freq;
    // Sguan.txdata.fdata[5] = vf.angle;
    // Sguan.txdata.fdata[6] = sin_theta;
    // Sguan.txdata.fdata[7] = cos_theta;

    Sguan.txdata.fdata[1] = MT_Sam.CurrU;
    Sguan.txdata.fdata[2] = MT_Sam.CurrV;
    Sguan.txdata.fdata[3] = MT_Sam.CurrW;

    Sguan.txdata.fdata[4] = Sguan.current.Real_Ia;  
    Sguan.txdata.fdata[5] = Sguan.current.Real_Ib;  
    Sguan.txdata.fdata[6] = Sguan.current.Real_Ic;      



    // ========== 反Park输出对比 (8-11) ==========
    // Sguan.txdata.fdata[8]  = Sguan.foc.Ualpha;   // Sguan反Park输出α
    // Sguan.txdata.fdata[9]  = vf_valpha;          // myVF反Park输出α
    // Sguan.txdata.fdata[10] = Sguan.foc.Ubeta;    // Sguan反Park输出β
    // Sguan.txdata.fdata[11] = vf_vbeta;           // myVF反Park输出β

    Sguan.txdata.fdata[8] = VF_PWMLoad[0];
    Sguan.txdata.fdata[9] = VF_PWMLoad[1];
    Sguan.txdata.fdata[10] =VF_PWMLoad[2];

    Sguan.txdata.fdata[11] = pwmu;
    Sguan.txdata.fdata[12] = pwmv;
    Sguan.txdata.fdata[13] = pwmw;

    

    // ========== 组4: 系统配置与辅助信息 (15) ==========
   // Sguan.txdata.fdata[15] = (float)CONFIG_MODE;        // 当前控制模式 (0-10)
}

/**
 * @description: 2.用户的通信接收接口
 * @reminder: (此方函数->填写浮点数赋值对象)
 * @reminder: (如果需要其他指令，可在Sguan_printf.c中添加)
 * @param {float} data
 * @return {*}
 */
static inline void Handle_User0_Adjust(float data){
    /* Your code for Parameter set */
    // 接收到串口或者CAN的数据是User0=xx?
    // 收到指令后，会把数据赋值到data
}

/**
 * @description: 3.用户的通信接收接口
 * @reminder: (此方函数->填写浮点数赋值对象)
 * @param {float} data
 * @return {*}
 */
static inline void Handle_User1_Adjust(float data){
    /* Your code for Parameter set */
    // 接收到串口或者CAN的数据是User1=xx?
    // 收到指令后，会把数据赋值到data
}

/**
 * @description: 4.用户的通信接收接口
 * @reminder: (此方函数->填写浮点数赋值对象)
 * @param {float} data
 * @return {*}
 */
static inline void Handle_User2_Adjust(float data){
    /* Your code for Parameter set */
    // 接收到串口或者CAN的数据是User2=xx?
    // 收到指令后，会把数据赋值到data
}

#endif // USERDATA_USERCONTROL_H
