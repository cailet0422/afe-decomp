#ifndef LOS_THREAD_H
#define LOS_THREAD_H

#include "types.h"
#include "dolphin/os/OSThread.h"

typedef s32 OSPri;
typedef s32 OSId;


#if defined(__cplusplus) && defined(TARGET_PC)
extern "C" {
#endif

OSId osGetThreadId(OSThread*);
void osYieldThread(void);
BOOL osIsTerminatedThread(OSThread*);

#if defined(__cplusplus) && defined(TARGET_PC)
}
#endif

#endif
