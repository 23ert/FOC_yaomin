#include "func_main.h"
#include "board.h"



void FUNC_runTask(void)
{
    VOFT_SendData();
    StateMachine_Update();
}



