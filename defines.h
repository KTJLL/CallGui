#include <windows.h>

#ifndef _DEFINES_H_
#define _DEFINES_H_

#define SSDT_NUMBERS	401
#define SSSDT_NUMBERS	827

extern ULONG64 OriginSSDTData[SSDT_NUMBERS];
extern const char* ssdtfunc[SSDT_NUMBERS];

extern ULONG64 OriginSSSDTData[SSSDT_NUMBERS];
extern const char* sssdtfunc[SSSDT_NUMBERS];

extern const char *procedureNames[][9];
extern ULONG procedureRVAs[46][8];

#endif
