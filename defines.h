#include <windows.h>

#ifndef _DEFINES_H_
#define _DEFINES_H_

#define SSDT_NUMBERS	401
#define SSSDT_NUMBERS	827

extern ULONG64 OriginSSDTData[SSDT_NUMBERS];
extern const char* ssdtfunc[SSDT_NUMBERS];

extern ULONG64 OriginSSSDTData[SSSDT_NUMBERS];
extern const char* sssdtfunc[SSSDT_NUMBERS];

typedef struct _PROCEDURE_NAMES {
    const char *ObjectName;
    const char *ProcedureName[8];
    UINT8 pattern[32];
    UINT Length;
    //char *DumpProcedure, *OpenProcedure, *CloseProcedure, *DeleteProcedure, *ParseProcedure, *SecurityProcedure, *QueryNameProcedure, *OkayToCloseProcedure;
} PROCEDURE_NAMES, *PPROCEDURE_NAMES;

extern PROCEDURE_NAMES procedureNames[42];
extern ULONG procedureRVAs[42][8];

/* 存放 n 个位需要多少字节 */
#define BM_BYTES(n)         (((size_t)(n) + 7u) / 8u)

/* 位索引 k 对应的字节下标 */
#define BM_BYTE_IDX(k)      ((size_t)(k) >> 3)

/* 位索引 k 在字节内的掩码（LSB-first） */
#define BM_BIT(k)           (1u << ((unsigned)(k) & 7u))

/* ---- 读取 ---- */

/* 测试：非零表示置 1，适合 if/while 直接判断 */
#define BM_TEST(bm, k)      ((bm)[BM_BYTE_IDX(k)] & BM_BIT(k))

/* 取值：严格返回 0 或 1，适合赋给 int */
#define BM_GET(bm, k)       (((bm)[BM_BYTE_IDX(k)] >> ((unsigned)(k) & 7u)) & 1u)

/* ---- 写入（读-改-写） ---- */

#define BM_SET(bm, k)       ((bm)[BM_BYTE_IDX(k)] |=  (UINT8)BM_BIT(k))
#define BM_CLR(bm, k)       ((bm)[BM_BYTE_IDX(k)] &= (UINT8)~BM_BIT(k))
#define BM_FLIP(bm, k)      ((bm)[BM_BYTE_IDX(k)] ^=  (UINT8)BM_BIT(k))

#endif
