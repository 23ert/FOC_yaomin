#ifndef _SAMPLE_INIT_H
#define _SAMPLE_INIT_H

#include "device.h"

typedef struct CurrZeroDef
{
    /* data */
    uint32_t IZeroSum[3];
    uint16_t UCurZero;
    uint16_t VCurZero;
    uint16_t WCurZero;
}CurrZero;


extern CurrZero gPhaseCurrZero;

extern void Sample_InitCurr(void);
extern void Sample_getZeroCurr(void);

#endif





