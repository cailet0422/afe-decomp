#ifndef ASTEST_H
#define ASTEST_H

#include "types.h"
#include "jaudio_NES/audiostruct.h"

#if defined(__cplusplus) && defined(TARGET_PC)
extern "C" {
#endif

extern void CopyStc(commonch* dstStc, commonch* srcStc);

#if defined(__cplusplus) && defined(TARGET_PC)
}
#endif

#endif
