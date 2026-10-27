#ifndef _DOLPHIN_CARDREAD_H_
#define _DOLPHIN_CARDREAD_H_

s32 CARDReadAsync(CARDFileInfo *fileInfo, void *buf, s32 length, s32 offset, CARDCallback callback);
#if !defined(TARGET_PC) || !defined(__LP64__) /* long is 64-bit: card.h declares it with s32 */
long CARDRead(struct CARDFileInfo * fileInfo, void * buf, long length, long offset);
#endif
s32 CARDCancel(CARDFileInfo *fileInfo);

#endif // _DOLPHIN_CARDREAD_H_
