#ifndef _PA_Interrupt
#define _PA_Interrupt

#ifdef __cplusplus
extern "C" {
#endif

/*! \file PA_Interrupt.h
    \brief Interrupt system

    Mainly used for the VBlank system and the counters...
*/

typedef void (*funcpointer)();

extern funcpointer MotionVBL;
extern funcpointer GHPadVBL;
extern funcpointer PaddleVBL;

extern s32 PA_VBLCounter[16]; // VBL counters
extern bool PA_VBLCounterOn[16]; // VBL counters enable/disable flag

extern funcpointer CustomVBL;

/*! \fn void PA_vblFunc()
    \brief
         \~english The standard PAlib VBL function. This will update the pad, the stylus, the RTC, etc... You could/should use this function if you do your own custom VBL.
         \~french Fonction standard du VBL PAlib. Ceci met à jour le pad, le stylet, le RTC... Utilisez cette fonction si vous faites votre propre VBL.
*/
void PA_vblFunc();

/*! \fn static inline void PA_InitVBL()
	\deprecated
    \brief
         \~english [DEPRECATED] In previous PAlib versions this initialized the VBlank system, but it now does nothing since it's automatically done at PA_Init().
         \~french [DEPRECATED] Initialise une fonction VBL basique. Ca met à jour l'OAM (les sprites), les touches, et le stylet chaque frame.

*/
static inline void PA_InitVBL(){
	/*
	irqSet(IRQ_VBLANK, PA_vblFunc);
	irqEnable(IRQ_VBLANK);
	*/
}

/*! \def PA_GetVcount()
    \brief
         \~english Get the vertical line count...
         \~french Récupérer le numéro de ligne tracée
*/
#define PA_GetVcount() (REG_VCOUNT&511)

/*! \fn void PA_VBLCountersReset()
    \brief
         \~english Resets the VBL counters
         \~french Remet à 0 les compteurs VBL
*/
void PA_VBLCountersReset();

/*! \fn static inline void PA_VBLCounterStart(u8 nCounter)
    \brief
         \~english Resets a given counter and starts running
         \~french Remet à 0 un compteur donné et le démarre
    \~\param nCounter
         \~english Counter number (0-15)
         \~french Numéro du compteur (0-15)
*/
static inline void PA_VBLCounterStart(u8 nCounter){
	PA_VBLCounter[nCounter]   = 0;
	PA_VBLCounterOn[nCounter] = 1;
}

/*! \fn static inline void PA_VBLCounterPause(u8 nCounter)
    \brief
         \~english Pauses a given VBL counter
         \~french Met en pause un compteur VBL donné
    \~\param nCounter
         \~english Counter number (0-15)
         \~french Numéro du compteur (0-15)
*/
static inline void PA_VBLCounterPause(u8 nCounter){
	PA_VBLCounterOn[nCounter] = 0;
}

/*! \fn static inline void PA_VBLCounterUnpause(u8 nCounter)
    \brief
         \~english Unpauses a given VBL counter
         \~french Refait tourner un compteur VBL donné
    \~\param nCounter
         \~english Counter number (0-15)
         \~french Numéro du compteur (0-15)
*/
static inline void PA_VBLCounterUnpause(u8 nCounter){
	PA_VBLCounterOn[nCounter] = 1;
}

static inline void PA_VBLFunctionInit(funcpointer VBLFunc){
	CustomVBL = VBLFunc;
}

void PA_Nothing();

static inline void PA_VBLFunctionReset(void){
	PA_VBLFunctionInit(PA_Nothing);
}

/** @} */ // end of Interrupts

#ifdef __cplusplus
}
#endif
#endif
