#ifndef __SGUAN_PRINTF_H
#define __SGUAN_PRINTF_H

#include "Sguan_Config.h"
#include <stdint.h>

#define SGUAN_CH_COUNT      16    // JustFloat 通道数
#define CMD_MAX_LEN   20    // 最大支持的指令长度
#define HASH_SIZE     32    // 哈希表大小
typedef void (*CmdCallback)(float value);

typedef struct {
    const char* name;       // 指令名称
    uint32_t hash_val;      // 哈希值 (运行时计算并缓存)
    CmdCallback callback;   // 对应的处理函数
} CMD_MAP_STRUCT;

typedef struct {
    float fdata[SGUAN_CH_COUNT];
    uint8_t tail[4];
} PRINTF_STRUCT;

extern uint8_t Sguan_PrintfBuff[200];
extern uint8_t Sguan_RxBuff[200];   // DMA 接收专用缓冲，与解析缓冲 Sguan_PrintfBuff 分离，避免 DMA 与解析共用同一数组导致跨片指令被覆盖

/* 接收解析上下文：将“写指针/累计状态”收进结构体，替代原函数内的 static 变量，
   使其可被外部重置、可支持多实例，消除隐式全局状态耦合 */
typedef struct {
    uint16_t buff_ptr;               // 当前写入 Sguan_PrintfBuff 的位置
    uint8_t  *buff;                  // 指向解析缓冲（通常即 Sguan_PrintfBuff）
    uint16_t  buff_size;             // 缓冲容量，用于边界保护
} RX_PARSER_CTX;

void Printf_TX_Init(PRINTF_STRUCT *str);
void Printf_TX_Loop(PRINTF_STRUCT *str);
void Printf_RX_Init(void);
void Printf_RX_Reset(RX_PARSER_CTX *ctx);                       // 外部复位解析状态（DMA 错乱/协议异常时调用）
void Printf_RX_Loop(uint8_t *data, uint16_t length);


#endif // SGUAN_PRINTF_H
