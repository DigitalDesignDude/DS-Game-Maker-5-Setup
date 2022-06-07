#ifndef _PA_SOUND9_
#define _PA_SOUND9_

#ifdef __cplusplus
extern "C" {
#endif

#include <nds.h>

// Some macros to improve compatibility to old code
#define PA_PlaySimpleSound(name)         AS_SoundDefaultPlay((u8*)name, (u32)name##_size, 127, 64, false, 0)
#define PA_PlaySoundRepeat(name)         AS_SoundDefaultPlay((u8*)name, (u32)name##_size, 127, 64, true, 0)
#define PA_SoundChannelIsBusy(channel)   IPC_Sound->chan[channel].busy
#define PA_SetSoundChannelPan(chan, pan) AS_SetSoundPan(chan, pan)
#define PA_SetSoundChannelVol(chan, vol) AS_SetSoundVolume(chan, vol)
#define PA_StopSound(chan)               AS_SoundStop(chan)
#define PA_SetSoundVol(vol)              AS_SetMasterVolume(vol)

/*! \fn void PA_PlayPSG(u8 chan, u8 vol, u8 pan, u32 freq, u8 duty)
    \brief
         \~english Play a tone (or noise) using the Programmable Sound Generator
         \~french Utiliser le Programmable Sound Generator
    \param PA_Channel
         \~english Audio channel, from 8 to 15
         \~french Canal audio, de 8 à 15
    \param vol
         \~english Volume (0-127)
         \~french Volume (0-127)	 		 
    \param freq
         \~english Frenquency
         \~french Fréquence
    \param duty
         \~english Duty (0-7)
         \~french Duty (0-7)		 
*/
void PA_PlayPSG(u8 chan, u8 vol, u8 pan, u32 freq, u8 duty);

// Legacy functions...
void PA_PlaySoundEx2(u8 chan, const void* data, s32 length, u8 volume, int freq, s16 format, bool repeat, int repeatPoint) __attribute__((deprecated));
void PA_SoundStop(u8 chan) __attribute__((deprecated));

#ifdef __cplusplus
}
#endif

#endif
