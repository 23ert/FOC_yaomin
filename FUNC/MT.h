#ifndef _MT_H
#define _MT_H


#include "board.h"

/** 电机控制采样数据*/
typedef struct {
    float Udc;         /* 母线电压*/
    float Upot;        /* 电位器电压 */
    float CurrU;       /* U相电流*/
    float CurrV;       /* V相电流*/
    float CurrW;       /* W相电流*/
} MT_Sam_t;


extern MT_Sam_t MT_Sam;




#endif


