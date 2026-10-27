#ifndef _JAUDIO_JA_CALC_H
#define _JAUDIO_JA_CALC_H

#include "types.h"

#if defined(__cplusplus) && defined(TARGET_PC)
extern "C" {
#endif

/////////// JAUDIO MATH DEFINITIONS ///////////
f32 sqrtf2(f32);
f32 atanf2(f32, f32);
void Jac_InitSinTable();
f32 sinf3(f32);

///////////////////////////////////////////////

#if defined(__cplusplus) && defined(TARGET_PC)
}
#endif

#endif
