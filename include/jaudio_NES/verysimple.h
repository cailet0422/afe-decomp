#ifndef VERYSIMPLE_H
#define VERYSIMPLE_H
#include "types.h"

#if defined(__cplusplus) && defined(TARGET_PC)
extern "C" {
#endif

void Jac_Start(void*, u32, u32);
void Jac_PlayInit(void);

#if defined(__cplusplus) && defined(TARGET_PC)
}
#endif

#endif
