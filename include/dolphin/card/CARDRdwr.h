#ifndef _DOLPHIN_CARDRDWR_H_
#define _DOLPHIN_CARDRDWR_H_

#if !defined(TARGET_PC) || !defined(__LP64__) /* long is 64-bit: card.h declares it with s32 */
long CARDGetXferredBytes(long chan);
#endif

#endif // _DOLPHIN_CARDRDWR_H_
