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




extern void testTIM(void);


extern void ENPWMABC(void);
extern void ENPWM_ADC(void);
extern void DISPWMABC(void);

#endif

