#include "func_main.h"
#include "board.h"



extern void Drive_calcUdc(MT_Sam_t *pMT);

void FUNC_runTask(void)
{
    
    Drive_calcUdc(&MT_Sam);

    #if SMGUAN_FOC
    SguanFOC_main_Loop();  //only for init step;
    SguanFOC_Low_Loop();  //commen state deal  //后面这个可能会移到定时器所以它对初始化单独处理了
    #endif

// #if MY_FOC_TEST
//     VOFT_SendData();
// #endif

}





