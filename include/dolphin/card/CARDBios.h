#ifndef _DOLPHIN_CARDBIOS_H_
#define _DOLPHIN_CARDBIOS_H_

void CARDInit(void);
s32 CARDGetResultCode(s32 chan);
s32 CARDFreeBlocks(s32 chan, s32 *byteNotUsed, s32 *filesNotUsed);
#if !defined(TARGET_PC) || !defined(__LP64__) /* long is 64-bit: card.h declares it with s32 */
long CARDGetEncoding(long chan, unsigned short * encode);
#endif
#if !defined(TARGET_PC) || !defined(__LP64__) /* long is 64-bit: card.h declares it with s32 */
long CARDGetMemSize(long chan, unsigned short * size);
#endif
s32 CARDGetSectorSize(s32 chan, u32 *size);

#endif // _DOLPHIN_CARDBIOS_H_
