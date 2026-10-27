#ifndef JSUIOSBASE_H
#define JSUIOSBASE_H

#include "types.h"
#include "JSystem/JSupport/JSUStreamEnum.h"

#ifdef __cplusplus
class JSUIosBase {
  public:
    inline JSUIosBase() : mState(GOOD) {
    }

    virtual ~JSUIosBase() {
    }

    bool isGood() {
        return !this->mState;
    }
    void clrState(EIoState ioState) {
        this->mState &= ~ioState;
    }
    void setState(EIoState ioState) {
        this->mState |= ioState;
    }
#ifdef TARGET_PC
    /* Sources say EOF, which stdio makes the int -1 (JSUStreamEnum.h) */
    void clrState(int ioState) {
        this->clrState((EIoState)(ioState == -1 ? 1 : ioState));
    }
    void setState(int ioState) {
        this->setState((EIoState)(ioState == -1 ? 1 : ioState));
    }
#endif

    u8 mState;
};
#endif

#endif
