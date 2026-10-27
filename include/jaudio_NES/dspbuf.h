#ifndef DSPBUF_H
#define DSPBUF_H

#include "types.h"
#include "jaudio_NES/audiocommon.h"
#include "jaudio_NES/rate.h"

#if defined(__cplusplus) && defined(TARGET_PC)
extern "C" {
#endif

extern s16* DspbufProcess(DSPBUF_EVENTS event);
extern s16* MixDsp(s32 nSamples);
extern void UpdateDSP(void);
extern void DspFrameEnd(void);

#if defined(__cplusplus) && defined(TARGET_PC)
}
#endif

#endif
