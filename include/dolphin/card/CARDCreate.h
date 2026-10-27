#ifndef _DOLPHIN_CARDCREATE_H_
#define _DOLPHIN_CARDCREATE_H_

s32 CARDCreateAsync(s32 chan, const char* fileName, u32 size, CARDFileInfo* fileInfo, CARDCallback callback);
#if !defined(TARGET_PC) || !defined(__LP64__) /* long is 64-bit: card.h declares it with s32 */
long CARDCreate(long chan, const char * fileName, unsigned long size, struct CARDFileInfo * fileInfo);
#endif

#endif // _DOLPHIN_CARDCREATE_H_
