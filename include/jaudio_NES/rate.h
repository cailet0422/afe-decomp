#ifndef _JAUDIO_RATE_H
#define _JAUDIO_RATE_H

#include "types.h"

#if defined(__cplusplus) && defined(TARGET_PC)
extern "C" {
#endif

/////////// JAUDIO GLOBAL RATE SETTINGS ///////////
extern u32 JAC_AI_SETTING;
extern f32 JAC_DAC_RATE;
extern u32 JAC_SUBFRAMES;
extern u32 JAC_FRAMESAMPLES;
extern u32 DAC_SIZE;

///////////////////////////////////////////////////

#if defined(__cplusplus) && defined(TARGET_PC)
}
#endif

#endif
