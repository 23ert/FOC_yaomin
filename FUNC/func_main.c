#include "func_main.h"
#include "board.h"

#define SOFT_VER 100

extern void Drive_calcUdc(MT_Sam_t *pMT);

void FUNC_runTask(void)
{
    Drive_calcUdc(&MT_Sam);
    VOFT_SendData();
    StateMachine_Update();
}



