#pragma once

#ifdef __cplusplus
extern "C"{
#endif

/*! \file PA_Debug.h
    \brief Debugging utilities

    Some debugging utilities like emulator detecting and iDeaS debug console printing
*/

/** @defgroup Debug Debugging utilities
 *  Some debugging utilities like emulator detecting and iDeaS debug console printing
 *  @{
 */

/*! \fn bool PA_IsEmulator()
    \brief
         \~english Detects if the program is running on an emulator
         \~french Détecte si le programme est exécuté sur un émulateur
*/

bool PA_IsEmulator();

/*! \fn void PA_iDeaS_DebugOutput(const char* str)
    \brief
         \~english Outputs text to the iDeaS debugging console
         \~french Sorties de texte à la iDeaS debugging console
    \~\param str
         \~english The text to output
         \~french Le texte
*/

#ifndef PA_DISABLE_DEBUG
void PA_iDeaS_DebugOutput(const char* str);
#else
#define PA_iDeaS_DebugOutput(str) ((void)0)
#endif

/*! \fn void PA_iDeaS_DebugPrintf(const char* str, ...)
    \brief
         \~english Outputs formatted text to the iDeaS debugging console
         \~french Sorties de texte formaté à la iDeaS debugging console
    \~\param str
         \~english The text to output
         \~french Le texte
*/

#ifndef PA_DISABLE_DEBUG
void PA_iDeaS_DebugPrintf(const char* str, ...);
#else
static inline void PA_iDeaS_DebugPrintf(const char* str, ...){}
#endif

/*! \fn void PA_iDeaS_Breakpoint()
    \brief
         \~english Triggers a breakpoint on iDeaS
         \~french Mets un breakpoint sur iDeaS
*/

#ifndef PA_DISABLE_DEBUG
void PA_iDeaS_Breakpoint();
#else
#define PA_iDeaS_Breakpoint() ((void)0)
#endif

/** @} */ // end of debugging utilities

#ifdef __cplusplus
}
#endif
