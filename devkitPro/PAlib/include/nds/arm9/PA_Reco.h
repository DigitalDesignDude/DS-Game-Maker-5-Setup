#ifndef _PA_Reco
#define _PA_Reco

#ifdef __cplusplus
extern "C" {
#endif

#include "PA9.h"

/*! \file PA_Reco.h
    \brief Touchscreen Shape recognotion system

    This will allow to recognize basic shapes drawn by the user !
*/

typedef struct{
	u8 x, y;
} PA_StylusPosition;
extern PA_StylusPosition PA_StylusPos[20000];

#define PA_RECOTESTS 30

typedef struct{
	char letter;
	char code[16];
} PA_FormType;

extern PA_FormType PA_Graffiti[PA_RECOTESTS];		

typedef struct{
	u32 nvalues, oldn, veryold;
} PA_RecoValues;
extern PA_RecoValues PA_Reco;

typedef struct{
	u32 Length;
	u8 minX, minY, maxX, maxY; // Shape limits
	u8 startX, startY, endX, endY; // Start and end position
	s16 Angle;
	s32 Difference;
	u8 Shape;
} PA_RecoInfos;
extern PA_RecoInfos PA_RecoInfo;

extern char PA_RecoShape[16]; // Stores the recognized pattern

extern u8 PA_UseGraffiti;

extern u8 PA_CustomReco; // number of custom shapes
extern PA_FormType PA_CustomShape[200];

/** @defgroup Reco Shape Recognition
 *  Draw a shape and have it recognized !
 *  @{
 */

/*!
    \fn char PA_CheckLetter()
    \brief
		\~english Analyzes the drawn shape and returns a letter according to it. 0 if nothing. The drawn shape's string is copied into PA_RecoShape on Stylus Release. You can find a copy of the current letters used here : http://www.palib.info/Reco/PAGraffiti.gif
		\~french Analyse la forme et renvoie une lettre correspondante. 0 si rien. La chaine représentative de la forme est copiée dans PA_RecoShape au Stylus Release. Copie des images dispos : http://www.palib.info/Reco/PAGraffiti.gif
*/
char PA_CheckLetter();

/*!
    \fn static inline void PA_RecoAddShape(char letter, char *shape)
    \brief
		\~english Adds a new shape to the recognition system
		\~french Ajouter une nouvelle forme au système de reconnaissance
    \~\param letter
      \~english Letter it will return for that shape (you can use any thing, even a number from 1 to 255)
      \~french Lettre renvoyée par le système de reconnaissance pour cette forme (peut être n'imorte quelle lettre, ou un nombre de 1 à 255)
    \~\param shape
      \~english 15 characters string given by the recognition system in PA_RecoShape
      \~french Chaine de 15 caractères fournie par le systeme de reconnaissance in PA_RecoShape
*/
static inline void PA_RecoAddShape(char letter, char *shape){
	PA_CustomShape[PA_CustomReco].letter = letter;
	int i;
	for (i = 0; i < 16; i++)
		PA_CustomShape[PA_CustomReco].code[i] = shape[i];
	PA_CustomReco++;
}

/*!
    \fn static inline void PA_ResetRecoSys(void)
    \brief
		\~english Resets the Recognition system
		\~french Réinitialise le systeme de reconnaissance
*/
static inline void PA_ResetRecoSys(){
	PA_CustomReco = 0;
	PA_UseGraffiti = 1;
}

/*!
    \fn static inline void PA_UsePAGraffiti(u8 use)
    \brief
		\~english Set on or off the PA Graffiti letters. You'll want to turn them off if you plan on using your own shapes....
		\~french Activer ou désactiver les lettres PA Graffiti. On voudra le désactiver quand on veut utiliser uniquement ses propres formes
    \~\param use
      \~english 1/0, on/off...
      \~french 1/0, on/off...
*/
static inline void PA_UsePAGraffiti(u8 use){
	PA_UseGraffiti = use;
}

/** @} */ // end of Reco

#ifdef __cplusplus
}
#endif

#endif
