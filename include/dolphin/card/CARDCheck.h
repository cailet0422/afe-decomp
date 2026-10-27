#ifndef _DOLPHIN_CARDCHECK_H_
#define _DOLPHIN_CARDCHECK_H_

s32 CARDCheckAsync(s32 chan, CARDCallback callback);
#if !defined(TARGET_PC) || !defined(__LP64__) /* long is 64-bit: card.h declares it with s32 */
long CARDCheck(long chan);
#endif

#endif // _DOLPHIN_CARDCHECK_H_
