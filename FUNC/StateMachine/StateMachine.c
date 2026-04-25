#include "StateMachine.h"
#include "func_main.h"

SystemState g_SystemState;

void StateMachine_Init(void)
{
    g_SystemState = STATE_STOP;
}

void StateMachine_Update(void)
{
    switch(g_SystemState)
    {
        case STATE_STOP:
            DISPWMABC();
            ENPWM_ADC();
            Sample_getZeroCurr();
            break;
            
        case STATE_RUN:
            ENPWMABC();
            ENPWM_ADC();
            break;
            
        default:
            g_SystemState = STATE_STOP;
            break;
    }
}

SystemState StateMachine_GetState(void)
{
    return g_SystemState;
}

void StateMachine_SetState(SystemState state)
{
    g_SystemState = state;
}


