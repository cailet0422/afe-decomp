#ifndef WAVEREAD_H
#define WAVEREAD_H
#include "types.h"

#if defined(__cplusplus) && defined(TARGET_PC)
extern "C" {
#endif

int WaveScene_Set(u32, u32);
void Wavegroup_Init();
int Wavegroup_Regist(void*, u32);
#if defined(__cplusplus) && defined(TARGET_PC)
}
#endif

#endif
