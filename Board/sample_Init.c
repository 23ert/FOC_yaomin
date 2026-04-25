#include "board.h"
#include "sample_Init.h"
#include "device.h"

CurrZero gPhaseCurrZero;

/**
  * @brief  电流传感器零点校准初始化
  * @note   在电机静止时采集三相电流的零点偏移值，用于后续电流测量补偿
  *         该函数会对ADC进行自校准，然后循环采样500次计算平均值
  * @param  无
  * @retval 无
  */
void Sample_InitCurr(void)
{
 //   Uint16 uiCurrCnt = 0;
    
    
    HAL_ADCEx_Calibration_Start(&hadc1,ADC_SINGLE_ENDED); // ADC1自校准：单端模式
    HAL_ADCEx_Calibration_Start(&hadc2,ADC_SINGLE_ENDED); // ADC2自校准：单端模式

//	HAL_ADCEx_InjectedStart(&hadc1); // 启动ADC1注入通道
//    HAL_ADCEx_InjectedStart(&hadc2); // 启动ADC2注入通道

    
//    for(uiCurrCnt=0;uiCurrCnt<500;uiCurrCnt++)
//    {
////        HAL_ADCEx_InjectedStart(&hadc1); // 触发ADC1注入通道采样
////        HAL_ADCEx_InjectedStart(&hadc2); // 触发ADC2注入通道采样

//        HAL_Delay(10); // 延时100ms等待ADC转换完成

//        
//        gPhaseCurrZero.IZero[0] += hadc1.Instance->JDR1; // 累加ADC1注入通道1数据（相位A电流零点）
//        gPhaseCurrZero.IZero[1] += hadc1.Instance->JDR2; // 累加ADC1注入通道2数据（相位B电流零点）
//        gPhaseCurrZero.IZero[2] += hadc2.Instance->JDR1; // 累加ADC2注入通道1数据（相位C电流零点）
//    }
//  
//    gPhaseCurrZero.IZero[0] /= 500; // 计算相位A电流零点平均值
//    gPhaseCurrZero.IZero[1] /= 500; // 计算相位B电流零点平均值
//    gPhaseCurrZero.IZero[2] /= 500; // 计算相位C电流零点平均值


}

void Sample_getZeroCurr(void)
{
    static Uint16 uiSampleCnt = 0;
    
    if(uiSampleCnt < 500)
    {
        uiSampleCnt++;
        gPhaseCurrZero.IZeroSum[0] += hadc1.Instance->JDR1; // 累加ADC1注入通道1数据（相位A电流零点）
        gPhaseCurrZero.IZeroSum[1] += hadc1.Instance->JDR2; // 累加ADC1注入通道2数据（相位B电流零点）
        gPhaseCurrZero.IZeroSum[2] += hadc2.Instance->JDR1; // 累加ADC2注入通道1数据（相位C电流零点）
        return;
    }
    else
    {
        uiSampleCnt = 0;
        
        gPhaseCurrZero.UCurZero = gPhaseCurrZero.IZeroSum[0]/500; // 计算相位A电流零点平均值
        gPhaseCurrZero.VCurZero = gPhaseCurrZero.IZeroSum[1]/500; // 计算相位B电流零点平均值
        gPhaseCurrZero.WCurZero = gPhaseCurrZero.IZeroSum[2]/500; // 计算相位C电流零点平均值
        
        gPhaseCurrZero.IZeroSum[0] = 0;
        gPhaseCurrZero.IZeroSum[1] = 0;
        gPhaseCurrZero.IZeroSum[2] = 0;
    
    }


}
