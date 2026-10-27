#ifndef _DOLPHIN_CARDWRITE_H_
#define _DOLPHIN_CARDWRITE_H_

#if !defined(TARGET_PC) || !defined(__LP64__) /* long is 64-bit: card.h declares it with s32 */
long CARDWriteAsync(struct CARDFileInfo * fileInfo, const void * buf, long length, long offset, void (* callback)(long, long));
#endif
#if !defined(TARGET_PC) || !defined(__LP64__) /* long is 64-bit: card.h declares it with s32 */
long CARDWrite(struct CARDFileInfo * fileInfo, const void * buf, long length, long offset);
#endif

#endif // _DOLPHIN_CARDWRITE_H_
