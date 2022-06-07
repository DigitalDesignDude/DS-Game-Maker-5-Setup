#ifndef _PA_BGTRANS
#define _PA_BGTRANS


/*! \file PA_BgTrans.h
    \brief Background Transition Effects

   All the different transition effects...
*/

enum{
	TRANS_ROUND, TRANS_DIAMOND, TRANS_CROSS, TRANS_LINES, TRANS_STAR,
	TRANS_LENGTH = 70
};
/*
#define TRANS_LENGTH 70

#define TRANS_ROUND 0
#define TRANS_DIAMOND 1
#define TRANS_CROSS 2
#define TRANS_LINES 3
#define TRANS_STAR 4
*/
/** @defgroup bgtrans Background Transition Effects
 *  All the different transition effects...
 *  @{
 */

/*! \fn void PA_InitBgTransEx(u8 screen, u8 bg)
    \brief
         \~english Init the BgTransition System.
         \~french Initialise le systeme BgTrans sur le fond 0
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param bg
         \~english Background (0-3)
         \~french Fond (0-3)
*/
void PA_InitBgTransEx(u8 screen, u8 bg);

/*! \fn static inline void PA_InitBgTrans(u8 screen)
    \brief
         \~english Init the BgTransition System. USES BG0 !! Place your sprite at a priority of 1 or more if you want them to disappear...
         \~french Initialise le systeme BgTrans sur le fond 0
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
*/
static inline void PA_InitBgTrans(u8 screen){
	PA_InitBgTransEx(screen, 0);
}   

/*! \fn void PA_BgTransUpDown(u8 screen, u16 type, u8 vflip, s16 state)
    \brief
         \~english Up/Down swipping transition effect
         \~french Effet de transition en Haut/Bas
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param type
         \~english BgTrans type... (0-4). Use macros TRANS_ROUND, TRANS_DIAMOND , TRANS_CROSS, TRANS_LINES, or TRANS_STAR
         \~french Type de transition... (0-4). Utiliser TRANS_ROUND, TRANS_DIAMOND , TRANS_CROSS, TRANS_LINES, ou TRANS_STAR 
    \~\param vflip
         \~english Vertical flip...
         \~french Flip vertical...
    \~\param state
         \~english State, from 0 to TRANS_LENGTH. 0 being visible, TRANS_LENGTH invisible
         \~french Etat, de 0 à TRANS_LENGTH. 0 pour visible, TRANS_LENGTH pour invisible... 
*/
void PA_BgTransUpDown(u8 screen, u16 type, u8 vflip, s16 state);

/*! \fn void PA_BgTransLeftRight(u8 screen, u16 type, u8 hflip, s16 state)
    \brief
         \~english Left/Right swipping transition effect
         \~french Effet de transition en Gauche/Droite
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param type
         \~english BgTrans type... (0-4). Use macros TRANS_ROUND, TRANS_DIAMOND , TRANS_CROSS, TRANS_LINES, or TRANS_STAR
         \~french Type de transition... (0-4). Utiliser TRANS_ROUND, TRANS_DIAMOND , TRANS_CROSS, TRANS_LINES, ou TRANS_STAR 
    \~\param hflip
         \~english Horizontal flip...
         \~french Flip horizontal...
    \~\param state
         \~english State, from 0 to TRANS_LENGTH. 0 being visible, TRANS_LENGTH invisible
         \~french Etat, de 0 à TRANS_LENGTH. 0 pour visible, TRANS_LENGTH pour invisible... 
*/
void PA_BgTransLeftRight(u8 screen, u16 type, u8 hflip, s16 state);

/*! \fn void PA_BgTransDiag(u8 screen, u16 type, u8 hflip, u8 vflip, s16 state)
    \brief
         \~english Diagonal swipping transition effect
         \~french Effet de transition en diagonale
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param type
         \~english BgTrans type... (0-4). Use macros TRANS_ROUND, TRANS_DIAMOND , TRANS_CROSS, TRANS_LINES, or TRANS_STAR
         \~french Type de transition... (0-4). Utiliser TRANS_ROUND, TRANS_DIAMOND , TRANS_CROSS, TRANS_LINES, ou TRANS_STAR 
    \~\param hflip
         \~english Horizontal flip...
         \~french Flip horizontal...
    \~\param vflip
         \~english Vertical flip...
         \~french Flip vertical...
    \~\param state
         \~english State, from 0 to TRANS_LENGTH. 0 being visible, TRANS_LENGTH invisible
         \~french Etat, de 0 à TRANS_LENGTH. 0 pour visible, TRANS_LENGTH pour invisible... 
*/
void PA_BgTransDiag(u8 screen, u16 type, u8 hflip, u8 vflip, s16 state);


/*! \fn void PA_BgTransCenter(u8 screen, u16 type, u8 invert, s16 state)
    \brief
         \~english Center transition effect
         \~french Effet de transition depuis le centre
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param type
         \~english BgTrans type... (0-4). Use macros TRANS_ROUND, TRANS_DIAMOND , TRANS_CROSS, TRANS_LINES, or TRANS_STAR
         \~french Type de transition... (0-4). Utiliser TRANS_ROUND, TRANS_DIAMOND , TRANS_CROSS, TRANS_LINES, ou TRANS_STAR 
    \~\param invert
         \~english Invert in/out
         \~french Inverser dedans/dehors
    \~\param state
         \~english State, from 0 to TRANS_LENGTH. 0 being visible, TRANS_LENGTH invisible
         \~french Etat, de 0 à TRANS_LENGTH. 0 pour visible, TRANS_LENGTH pour invisible... 
*/
void PA_BgTransCenter(u8 screen, u16 type, u8 invert, s16 state);

/** @} */ // end of bgtrans
//////////////////////////////////////////////////////////////////////

#endif
