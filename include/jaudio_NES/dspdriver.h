#ifndef _JAUDIO_DSPDRIVER_H
#define _JAUDIO_DSPDRIVER_H

#include "types.h"
#include "jaudio_NES/audiostruct.h"

/////////// JAUDIO DSP DRIVER DEFINITIONS ///////////
// Global functions (all C++, so no extern C wrap).

#if defined(__cplusplus) && defined(TARGET_PC)
extern "C" {
#endif

dspch_* GetDSPchannelHandle(u32 idx);
void InitDSPchannel();
#ifdef TARGET_PC
/* the owner is the logical channel (a jc_*), kept whole for its callbacks */
dspch_* AllocDSPchannel(u32, uintptr_t);
int DeAllocDSPchannel(dspch_*, uintptr_t);
#else
dspch_* AllocDSPchannel(u32, u32);
int DeAllocDSPchannel(dspch_*, u32);
#endif
dspch_* GetLowerDSPchannel();
dspch_* GetLowerActiveDSPchannel();
BOOL ForceStopDSPchannel(dspch_*);
BOOL BreakLowerDSPchannel(u8);
BOOL BreakLowerActiveDSPchannel(u8);
void UpdateDSPchannelAll();

/////////////////////////////////////////////////////

#if defined(__cplusplus) && defined(TARGET_PC)
}
#endif

#endif
