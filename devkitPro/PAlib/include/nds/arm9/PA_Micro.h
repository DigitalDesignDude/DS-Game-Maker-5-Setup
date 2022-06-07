#ifndef _PA_MICRO
#define _PA_MICRO

#include "PA_General.h"

/** @defgroup Micro Microphone
 * Record a sound and replay it...
 *  @{
 */


/*! \def PA_MicGetVol()
    \brief
         \~english Returns the Microphone volume
         \~french Renvoie le volume du micro
*/
#define PA_MicGetVol() PA_Transfer->micvol

/*! \fn static inline void PA_MicStartRecording(u8 *Buffer, u32 Length)
    \brief
         \~english Start recording from the microphone.
         \~french Commencer à enregistrer avec le microphone.
    \~\param Buffer
         \~english 8bit buffer in which to record the sound
         \~french Buffer dans lequel enregistrer le son
    \~\param Length
         \~english Buffer length. To convert seconds to 8bit length you have to multiply the seconds by 16384.
         \~french Longueur du buffer.
*/
static inline void PA_MicStartRecording(u8* buffer, u32 length){
	PA_FifoMsg msg;
	msg.type = PA_MSG_MIC;
	msg.MicMsg.buffer = buffer;
	msg.MicMsg.length = length;
	PA_SendFifoMsg(msg);
}

/*! \def PA_MicStopRecording()
    \brief
         \~english Stop recording from the microphone.
         \~french Arrête l'enregistrement.
*/

#define PA_MicStopRecording() PA_SendFifoCmd(PA_MSG_MICSTOP)


/*! \fn static inline void PA_MicReplay(u8 *Buffer, s32 Length)
    \brief
         \~english Play a recorded sound using ASlib
         \~french Rejouer un son enregistrer
    \~\param Buffer
         \~english 8bit buffer in which the sound was recorded
         \~french Buffer dans lequel on a enregistré le son
    \~\param Length
         \~english Buffer length
         \~french Longueur du buffer
*/
static inline void PA_MicReplay(u8* buffer, s32 length){
	SoundInfo snd = { 
        buffer, 
        length, 
        AS_PCM_8BIT,
        16384, 
        127, 
        64, 
        false, 
        0, 
        AS_NO_DELAY};
    AS_SoundPlay(snd);
}

/** @} */ // end of Micro

#endif
