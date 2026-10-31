#include "jaudio_NES/dspproc.h"

#ifdef TARGET_PC
/*==========================================================================
 * PC: No hardware DSP — pc_dsp.c mixes JAudio's voices in software, a
 * subframe at a time from the sound thread (pc_audio_process_frame).
 *==========================================================================*/
static u16 DSP_MIXERLEVEL = 0x4000;

extern void pc_dsp_setup_table(uintptr_t channels, uintptr_t res_filter, uintptr_t afc_filter);
extern void pc_dsp_sync_frame(u32 subframes, uintptr_t out, u16 mixer_level);

extern s32 DSPSendCommands(u32* commands, u32 count) {
    /* No hardware DSP on PC */
    (void)commands; (void)count;
    return 0;
}

extern u32 DSPReleaseHalt(void) {
    /* No hardware DSP */
    return 0x88881357;
}

extern void DSPWaitFinish(void) {
    /* No hardware DSP */
}

extern void DsetupTable(u32 arg0, uintptr_t arg1, uintptr_t arg2, uintptr_t arg3, uintptr_t arg4) {
    /* the channel table and the resampling and AFC coefficients */
    (void)arg0; (void)arg4;
    pc_dsp_setup_table(arg1, arg2, arg3);
}

extern void DsyncFrame(u32 subframes, uintptr_t dspbuf_start, uintptr_t dspbuf_end) {
    /* the frame is made interleaved at dspbuf_start (Jac_VframeWork) */
    (void)dspbuf_end;
    pc_dsp_sync_frame(subframes, dspbuf_start, DSP_MIXERLEVEL);
}

extern void DwaitFrame(void) {
    /* No-op on PC */
}

extern void DiplSec(u32 arg0) {
    /* IPL security — not needed on PC */
    (void)arg0;
}

extern void DagbSec(u32 arg0) {
    /* AGB security — not needed on PC */
    (void)arg0;
}

#else /* !TARGET_PC — original GC code */

#include "dolphin/dsp.h"

static u16 DSP_MIXERLEVEL = 0x4000;

extern s32 DSPSendCommands(u32* commands, u32 count) {
    if (DSPCheckMailToDSP() != 0) {
        return -1;
    }
    if (DSPCheckMailFromDSP() != 0) {
        return -1;
    }

    int i;

    for (i = 0; i < count; i++) {
        DSPSendMailToDSP(commands[i]);

        while (DSPCheckMailToDSP() != 0);
    }

    return 0;
}

extern u32 DSPReleaseHalt() {
    while (DSPCheckMailToDSP() != 0);
    DSPSendMailToDSP(0);
    if (DSPCheckMailFromDSP() != 0) {
        DSPReadMailFromDSP();
    }

    return 0x88881357;
}

extern void DSPWaitFinish() {
    do {
        while (DSPCheckMailFromDSP() == 0);
    } while ((DSPReadMailFromDSP() + 0x77780000) == 0x1357);
}

extern void DsetupTable(u32 arg0, u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    u32 commands[5];

    commands[0] = (arg0 & 0xFFFF) | 0x81000000;
    commands[1] = arg1;
    commands[2] = arg2;
    commands[3] = arg3;
    commands[4] = arg4;

    DSPSendCommands(commands, ARRAY_COUNT(commands));
    DSPWaitFinish();
}

extern void DsyncFrame(u32 subframes, u32 dspbuf_start, u32 dspbuf_end){
    u32 commands[3];

    commands[0] = (subframes << 16 & 0xFF0000) | 0x82000000 | DSP_MIXERLEVEL;
    commands[1] = dspbuf_start;
    commands[2] = dspbuf_end;

    DSPSendCommands(commands, ARRAY_COUNT(commands));
    DSPWaitFinish();
}

extern void DwaitFrame(){
    u32 commands[1];

    commands[0] = 0x80000000;
    DSPSendCommands(commands, ARRAY_COUNT(commands));
    DSPWaitFinish();
}

extern void DiplSec(u32 arg0){
    u32 commands[2];

    commands[0] = 0x8B000008;
    commands[1] = arg0;
    DSPSendCommands(commands, ARRAY_COUNT(commands));
    DSPWaitFinish();
}

extern void DagbSec(u32 arg0){
    u32 commands[2];

    commands[0] = 0x8C000008;
    commands[1] = arg0;
    DSPSendCommands(commands, ARRAY_COUNT(commands));
    DSPWaitFinish();
}

#endif /* TARGET_PC */

