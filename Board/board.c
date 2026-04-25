#include "main.h"
#include "board.h"
#include "func_main.h"



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
	 StateMachine_SetState(STATE_RUN);
  }
  if(Button2_Pin == GPIO_Pin)
  {
	 StateMachine_SetState(STATE_STOP);
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


void testTIM(void)
{
	//TIM1->PSC = 30000;
    
    TIM1->PSC = 1;
//	TIM1->ARR = 10000;
    
    TIM1->ARR = 8000;
    TIM1->CCR1 = 2000;
    TIM1->CCR2 = 5000;
    TIM1->CCR3 = 8000;
    TIM1->CCR4 = 8000-2;
    
    HAL_TIM_Base_Start(&htim1);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
    //
    HAL_ADCEx_InjectedStart_IT(&hadc1);
    HAL_ADCEx_InjectedStart_IT(&hadc2);
    
    HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_3);
	
}


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
//	uint16_t Xbufsize;
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




