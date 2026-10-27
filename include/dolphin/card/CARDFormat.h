#ifndef _DOLPHIN_CARDFORMAT_H_
#define _DOLPHIN_CARDFORMAT_H_

#if !defined(TARGET_PC) || !defined(__LP64__) /* long is 64-bit: card.h declares it with s32 */
long CARDFormat(long chan);
#endif

#endif // _DOLPHIN_CARDFORMAT_H_
