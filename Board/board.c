#include "main.h"
#include "board.h"
#include "func_main.h"

#define ADC_UDC_REG   hadc2.Instance->DR
#define ADC_UPOT_REG  hadc1.Instance->DR
// #define ADC_CURRU_REG hadc1.Instance->JDR1
// #define ADC_CURRV_REG hadc2.Instance->JDR1
// #define ADC_CURRW_REG hadc1.Instance->JDR2

//#define LOAD_U_COMP(X)  TIM1->CCR1 = X;
//#define LOAD_V_COMP(X)  TIM1->CCR2 = X;
//#define LOAD_W_COMP(X)  TIM1->CCR3 = X;

extern void Drive_calcUdc(MT_Sam_t *pMT);

static void Drive_calcCurr(MT_Sam_t *pMT);

extern uint8_t DataB1[32];
extern uint8_t DataB2[32];
extern uint8_t DataB3[32];

extern DMA_HandleTypeDef hdma_usart3_tx;
extern UART_HandleTypeDef huart3;




void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(GPIO_Pin);
  if(Button1_Pin == GPIO_Pin)
  {
	 HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
	 // 方案2：按键与串口 MOTOR=1 等价，统一走 Func_Start
	 // (内部置状态机START并开启硬件PWM发波)
	 Sguan.Func_Start();
  }
  if(Button2_Pin == GPIO_Pin)
  {
	 HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
	 // 方案2：按键与串口 MOTOR=0 等价，统一走 Func_Stop
	 // (内部置状态机STANDBY并关闭硬件PWM关波)
	 Sguan.Func_Stop();
  }
  if(Button3_Pin == GPIO_Pin)
  {
	HAL_GPIO_TogglePin(LED3_GPIO_Port,LED3_Pin);
//	while(HAL_DMA_GetState(&hdma_usart3_tx)!= HAL_DMA_STATE_READY);
//    HAL_UART_Transmit_DMA(&huart3,DataB3,sizeof(DataB3));	  
  
  }

  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_GPIO_EXTI_Callback could be implemented in the user file
   */
}









uint16_t test;
//void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
//{
//	uint16_t Xbufsize;
//	//float temp;
//	if(hadc == &hadc1)
//	{
//		Xbufsize = 2;
//		//FrameV.fdata[Xbufsize++] = hadc1.Instance->JDR1;
//		//temp =  hadc1.Instance->JDR1;
//		
////		FrameV.fdata[Xbufsize++] = ((float)hadc1.Instance->JDR1 - 0x7ef)*0.02197f;
////		FrameV.fdata[Xbufsize] = ((float)hadc1.Instance->JDR2 - 0x7ef)*0.02197f;
//		
//		FrameV.fdata[Xbufsize++] = ((float)hadc1.Instance->JDR1 - gPhaseCurrZero.IZero[0])*0.02197f;
//		FrameV.fdata[Xbufsize] = ((float)hadc1.Instance->JDR2 - gPhaseCurrZero.IZero[1])*0.02197f;
//		test++;
//		
//	}
//	if(hadc == &hadc2)
//	{
//		Xbufsize = 4;
//		//FrameV.fdata[Xbufsize++] = hadc1.Instance->JDR1;
//		//temp =  hadc1.Instance->JDR1;
//		
//		FrameV.fdata[Xbufsize] = ((float)hadc2.Instance->JDR1 - gPhaseCurrZero.IZero[2])*0.02197f;
//		test++;

////		temp[0] = hadc2.Instance->JDR1;
////		FrameV.fdata[Xbufsize] = (temp[0] - 0x7ef)*0.02197f;
//		
//	}

//}





void ENPWMABC(void)
{
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
    //HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);

    HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_3);
}


//启动TIM4以便于ADC转换
void ENPWM_ADC(void)
{
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
}

void DISPWMABC(void)
{
    HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_2);
    HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_3);

    HAL_TIMEx_PWMN_Stop(&htim1,TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Stop(&htim1,TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Stop(&htim1,TIM_CHANNEL_3);
}



void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	uint16_t Xbufsize;
	//float temp;
	if(hadc == &hadc1)
	{
        test++;
        
		
        #if TEST_CURR_UVW	
            Xbufsize = 2;
            FrameV.fdata[Xbufsize++] = ((float)hadc1.Instance->JDR1 - gPhaseCurrZero.UCurZero)*0.02197f;
            FrameV.fdata[Xbufsize] = ((float)hadc1.Instance->JDR2 - gPhaseCurrZero.VCurZero)*0.02197f;      
        #endif
        
        #if TEST_CLARK
            Clark_test_step();   
        #endif

        Drive_calcCurr(&MT_Sam);
    #if SMGUAN_FOC
        SguanFOC_High_Loop();

        myVF_Step(&vf,&vfin,VF_PWMLoad);

    #elif MY_FOC_TEST
        myVF_Step(&vf,&vfin,VF_PWMLoad);
        LOAD_U_COMP(VF_PWMLoad[0]);
        LOAD_V_COMP(VF_PWMLoad[1]);
        LOAD_W_COMP(VF_PWMLoad[2]);

    #endif

	}

	if(hadc == &hadc2)
	{
        test++;
       #if TEST_CURR_UVW
		Xbufsize = 4;
		
		FrameV.fdata[Xbufsize] = ((float)hadc2.Instance->JDR1 - gPhaseCurrZero.WCurZero)*0.02197f;
		#endif
		
	}

}

void Drive_calcUdc(MT_Sam_t *pMT)
{
	HAL_ADC_Start(&hadc1);
	pMT->Upot = ADC_UPOT_REG; //电位器          //阻塞式读取，后续优化
	
	HAL_ADC_Start(&hadc2);
	pMT->Udc = ADC_UDC_REG*0.02094726f; //母线电压 


}

static void Drive_calcCurr(MT_Sam_t *pMT)
{
    pMT->CurrU = ((float)ADC_CURRU_REG - gPhaseCurrZero.UCurZero)*0.02197f;
    pMT->CurrV = ((float)ADC_CURRV_REG - gPhaseCurrZero.VCurZero)*0.02197f;
    pMT->CurrW = ((float)ADC_CURRW_REG - gPhaseCurrZero.WCurZero)*0.02197f;
}



