#ifndef _DOLPHIN_CARDMOUNT_H_
#define _DOLPHIN_CARDMOUNT_H_

#if !defined(TARGET_PC) || !defined(__LP64__) /* long is 64-bit: card.h declares it with s32 */
int CARDProbe(long chan);
#endif
s32 CARDProbeEx(s32 chan, s32 *memSize, s32 *sectorSize);
s32 CARDMountAsync(s32 chan, void *workArea, CARDCallback detachCallback, CARDCallback attachCallback);
s32 CARDMount(s32 chan, void *workArea, CARDCallback detachCallback);
s32 CARDUnmount(s32 chan);

#endif // _DOLPHIN_CARDMOUNT_H_
