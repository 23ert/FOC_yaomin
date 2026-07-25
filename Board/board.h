#ifndef _BOARD_H
#define _BOARD_H

#include "stdio.h"
#include "string.h"
#include <stdint.h>

#include "gpio.h"
#include "opamp.h"
#include "adc.h"
#include "tim.h"
#include "comp.h"
#include "dac.h"
#include "usart.h"
#include "dma.h"

#define ADC_CURRU_REG hadc1.Instance->JDR1
#define ADC_CURRV_REG hadc2.Instance->JDR1
#define ADC_CURRW_REG hadc1.Instance->JDR2

#define LOAD_U_COMP(X)  TIM1->CCR1 = X;
#define LOAD_V_COMP(X)  TIM1->CCR2 = X;
#define LOAD_W_COMP(X)  TIM1->CCR3 = X;


extern void testTIM(void);


extern void ENPWMABC(void);
extern void ENPWM_ADC(void);
extern void DISPWMABC(void);



#endif

