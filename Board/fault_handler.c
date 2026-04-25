#include "board.h"
#include "fault_handler.h"
#include "device.h"



/* 外部调用接口 */
void Fault_Hardware_Init(void);      // 初始化 DAC/COMP/Break
void Fault_Set_OCP_Threshold(float Amps); // 设置过流保护阈值 (电流值转 DAC)
Motor_Fault_t Fault_Get_Status(void); // 获取当前故障状态

/**
  * @brief  电流传感器零点校准初始化
  * @note   在电机静止时采集三相电流的零点偏移值，用于后续电流测量补偿
  *         该函数会对ADC进行自校准，然后循环采样500次计算平均值
  * @param  无
  * @retval 无
  */
void FH_InitOverCurr(void)
{
    HAL_DAC_Start(&hdac3,DAC_CHANNEL_1);
    HAL_DAC_SetValue(&hdac3,DAC_CHANNEL_1,DAC_ALIGN_12B_R,3000);
    HAL_COMP_Start(&hcomp1);


}

