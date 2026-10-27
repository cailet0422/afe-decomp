#ifndef RAND_H
#define RAND_H

#ifdef TARGET_PC
#include <stdlib.h>
#else

#ifdef __cplusplus
extern "C" {
#endif

void srand(unsigned long seed);
int rand(void);

#ifdef __cplusplus
}
#endif

#endif /* TARGET_PC */
#endif
