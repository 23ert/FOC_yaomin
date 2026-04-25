#ifndef _STATEMACHINE_H
#define _STATEMACHINE_H

typedef enum {
    STATE_STOP = 0,
    STATE_RUN = 1
} SystemState;

extern void StateMachine_Init(void);
extern void StateMachine_Update(void);
extern SystemState StateMachine_GetState(void);
extern void StateMachine_SetState(SystemState state);

#endif
