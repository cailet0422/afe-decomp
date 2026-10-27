#ifndef NEOSTHREAD_H
#define NEOSTHREAD_H

#include "types.h"
#include "jaudio_NES/audiostruct.h"

#if defined(__cplusplus) && defined(TARGET_PC)
extern "C" {
#endif

extern u32 Neos_Update(s16* dst);
extern void ImageLoaded(u32 param);
extern BOOL Neos_CheckBoot(void);
extern void* neosproc(void* param);

#ifdef TARGET_PC
void pc_neos_init_sync(void);
#endif

#if defined(__cplusplus) && defined(TARGET_PC)
}
#endif

#endif
