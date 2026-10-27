#ifndef _JAUDIO_WAVEREAD_H
#define _JAUDIO_WAVEREAD_H

#include "types.h"

typedef struct CtrlGroup_ CtrlGroup_;

#if defined(__cplusplus) && defined(TARGET_PC)
extern "C" {
#endif

CtrlGroup_* Wave_Test(u8*);
void GetSound_Test(u32);
BOOL Wavegroup_Regist(void*, u32);
void Wavegroup_Init();
CtrlGroup_* WaveidToWavegroup(u32, u32);
BOOL WaveScene_Set(u32, u32);
BOOL WaveScene_Load(u32, u32);
void WaveScene_Close(u32, u32);
void WaveScene_Erase(u32, u32);

#if defined(__cplusplus) && defined(TARGET_PC)
}
#endif

#endif
