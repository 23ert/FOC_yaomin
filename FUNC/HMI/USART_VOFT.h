#ifndef _USART_VOFT_H
#define _USART_VOFT_H

#include "board.h"

#define TEST_CLARK 1
#define TEST_PWM_GNE 0
#define TEST_CURR_UVW 0

#if TEST_CURR_UVW
    #define CH_COUNT 3
#elif TEST_CLARK
    #define CH_COUNT 5
#endif

struct Frame 
{
    float fdata[CH_COUNT];
	uint8_t data[CH_COUNT*4];
    uint8_t tail[4];
};



extern struct Frame FrameV;
extern void VOFT_sendUdc(void);
extern void VOFT_sendPWM(void);
extern void clc_sendUdc(void);
extern void VOFT_comClark(void);
extern void VOFT_SendData(void);

#endif
