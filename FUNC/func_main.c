#include "func_main.h"
#include "board.h"

#define SOFT_VER 100


void FUNC_runTask(void)
{
    VOFT_SendData();
    StateMachine_Update();
}



