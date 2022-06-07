#ifndef _PA_Keyboard
#define _PA_Keyboard

#ifdef __cplusplus
extern "C" {
#endif

#include "PA9.h"

extern u8 keyb_screen;

/*! \file PA_Keyboard.h
    \brief Keyboard functions

    This will allow you to show a keyboard, use it, etc...
*/

void PA_SetLetterPal(s16 x, s16 y, u8 Pal);

extern const unsigned char keyboard_Tiles[10400];
extern const unsigned short keyboard_Map[4096];

typedef struct {
	u8 Bg; // Numéro du fond
	u8 Type; // 0 pour normal, 1 pour majuscules..
	char Letter;  // Lettre
	s16 ScrollX, ScrollY;  // Scroll x et y...
	s16 Repeat;  // Temps avant de répéter...
	s16 oldX, oldY; // Ancienne zone touchée
	u8 Color1, Color2; // Main color, pressed color...
	u8 Custom; // using custom graphics
} Keyboards;
extern Keyboards PA_Keyboard_Struct;

extern const u8 PA_Keyboard[2][5][24];

/** @defgroup Keyboard Keyboard
 *  Load a keyboard and have fun
 *  @{
 */

/// Old name for PA_LoadDefaultKeyboard().
#define PA_InitKeyboard PA_LoadDefaultKeyboard

/*!
    \fn void PA_LoadDefaultKeyboard(u8 bg_number)
    \brief
		\~english Initialise the default Keyboard on a given background. Uses 16 color palettes 14 and 15 (doesn't mix with text though, don't worry)
		\~french Initialiser le calvier sur un fond donné. Utilise les palettes de 16 couleurs 14 et 15 (n'interfère pas avec le texte)
    \~\param bg_number
      \~english Background number (0-3)
      \~french Numéro du fond que l'on veut tourner (0-3)
*/
void PA_LoadDefaultKeyboard(u8 bg_number);

/*!
    \def PA_InitCustomKeyboard(bg_number, keyb_custom)
	\deprecated
    \brief
		\~english [DEPRECATED] Initialise a custom Keyboard on a given background. 
		\~french [DEPRECATED] Initialiser un clavier perso sur un fond donné   
	\~\param bg_number
		\~english Background number (0-3)
		\~french Numéro du fond que l'on veut tourner (0-3)
	\~\param keyb_custom
		\~english Custom Keyboard name, converted as EasyBg
		\~french Claviet perso, converti comme EasyBg			
*/
#define PA_InitCustomKeyboard(bg_number, keyb_custom) do{\
	PA_DEPRECATED_MACRO;\
	PA_LoadBgPal(keyb_screen, bg_number, (void*)keyb_custom##_Pal);\
	PA_LoadSimpleBg(keyb_screen, bg_number, keyb_custom##_Tiles, keyb_custom##_Map, BG_256X512, 1, 1);\
	PA_Keyboard_Struct.Bg = bg_number;	PA_Keyboard_Struct.Type = 0;	PA_Keyboard_Struct.Repeat = 0;\
	PA_Keyboard_Struct.Custom = 1;\
	PA_BgInfo[keyb_screen][PA_Keyboard_Struct.Bg].Map = (u32)keyb_custom##_Map;\
}while(0)

/*!
    \fn void PA_LoadKeyboard(u8 bg_number, const PA_BgStruct* keyboard)
    \brief
		\~english Load a custom Keyboard on a given background. 
		\~french Initialiser un clavier perso sur un fond donné   
	\~\param bg_number
		\~english Background number (0-3)
		\~french Numéro du fond que l'on veut tourner (0-3)
	\~\param keyboard
		\~english Pointer to the keyboard background, converted as EasyBg
		\~french Pointeur vers le fond du claviet perso, converti comme EasyBg			
*/
void PA_LoadKeyboard(u8 bg_number, const PA_BgStruct* keyboard);

/*!
    \fn char PA_CheckKeyboard(void)
    \brief
		\~english Checks if the keyboard is used, and return the letter :)  Use this every turn (even if the stylus isn't pressed)
		\~french Vérifie le clavier, s'il est utilisé, et renvoie la lettre appuyée (0 si pas de nouvel appuye). A utiliser tout le temps, même si le stylet ne touche pas l'écran
*/
char PA_CheckKeyboard(void);

/*!
    \fn static inline void PA_ScrollKeyboardX(s16 x)
    \brief
		\~english Set the Keyboard's X position		
		\~french Placer le Clavier à la position X
    \~\param x
      \~english X position...
      \~french Position X
*/
static inline void PA_ScrollKeyboardX(s16 x) {
	PA_BGScrollX(keyb_screen, PA_Keyboard_Struct.Bg, -x);
	PA_Keyboard_Struct.ScrollX = x;
}

/*!
    \fn static inline void PA_ScrollKeyboardY(s16 y)
    \brief
		\~english Set the Keyboard's Y position		
		\~french Placer le Clavier à la position Y
    \~\param y
      \~english Y position...
      \~french Position Y
*/
static inline void PA_ScrollKeyboardY(s16 y) {
	PA_BGScrollY(keyb_screen, PA_Keyboard_Struct.Bg, -y);
	PA_Keyboard_Struct.ScrollY = y;
}

/*!
    \fn static inline void PA_ScrollKeyboardXY(s16 x, s16 y)
    \brief
		\~english Set the Keyboard's position		
		\~french Placer le Clavier à une position donnée
    \~\param x
      \~english X position...
      \~french Position X
    \~\param y
      \~english Y position...
      \~french Position Y
*/
static inline void PA_ScrollKeyboardXY(s16 x, s16 y) {
	PA_BGScrollXY(keyb_screen, PA_Keyboard_Struct.Bg, -x, -y);
	PA_Keyboard_Struct.ScrollX = x;
	PA_Keyboard_Struct.ScrollY = y;
}

/*!
    \def PA_EraseLastKey()
    \brief
		\~english Erase the last key lit up (if it didn't on it's own)
		\~french Effacer la dernière touche pressée, si ca ne le fait pas tout seul
*/
#define PA_EraseLastKey() PA_SetLetterPal(PA_Keyboard_Struct.oldX, PA_Keyboard_Struct.oldY, 15)

/*!
    \fn static inline void PA_KeyboardIn(s16 x, s16 y)
    \brief
		\~english Make the keyboard enter to position (x, y), scrolling from the bottom of the screen	
		\~french Faire entrer le clavier à la position (x, y) en glissant depuis le bas de l'écran
    \~\param x
      \~english X position...
      \~french Position X
    \~\param y
      \~english Y position...
      \~french Position Y
*/
static inline void PA_KeyboardIn(s16 x, s16 y){  // Faire rentrer le clavier... 
	int i;
	PA_ScrollKeyboardXY(x, 192);  // On le sort et met en X. Puis on fait glisser...
	for (i = 192; i > y; i-=8){
		PA_ScrollKeyboardY(i);
		PA_WaitForVBL();
	}
	PA_ScrollKeyboardY(y);  // On amène pile au bon y...		
}

/*!
    \fn static inline void PA_KeyboardOut(void)
    \brief
		\~english Make the keyboard scroll out
		\~french Faire sortir le clavier
*/
static inline void PA_KeyboardOut(void){  // Faire sortir le clavier... 
	int i;
	PA_EraseLastKey(); // Efface s'il reste une touche pressée...
	for (i = PA_Keyboard_Struct.ScrollY; i < 200; i+=8){
		PA_ScrollKeyboardY(i);
		PA_WaitForVBL();
	}
}

/*!
    \fn void PA_ReloadKeyboardCol(void)
    \brief
		\~english Reloads the keyboard's palette, usefull if you changed the background palette
		\~french Recharge la palette du clavier, utile si on a changé de palette pour les fonds
*/
void PA_ReloadKeyboardCol(void);


/*!
    \fn static inline void PA_SetKeyboardColor(u8 color1, u8 color2)
    \brief
		\~english You can change the color used by the keyboard... 
		\~french On peut changer la couleur du clavier !
    \~\param color1
      \~english Normal color, 0 for blue, 1 for red, 2 for green
      \~french Couleur normale, 0 pour bleu, 1 pour rouge, 2 pour vert
    \~\param color2
      \~english Pressed key color, 0 for blue, 1 for red, 2 for green
      \~french Couleur de la touche appuyée, 0 pour bleu, 1 pour rouge, 2 pour vert
*/
static inline void PA_SetKeyboardColor(u8 color1, u8 color2){
	PA_Keyboard_Struct.Color1 = color1;
	PA_Keyboard_Struct.Color2 = color2;
	PA_ReloadKeyboardCol();
}

/*!
    \fn static inline void PA_SetKeyboardScreen(u8 screen)
    \brief
		\~english Set Keyboard screen. Must be used BEFORE the keyboard init..
		\~french Régler l'écran du clavier. Doit être utilisé AVANT l'init du clavier
    \~\param screen
      \~english 0 (bottom) or 1 (top)
      \~french 0 (bas) or 1 (haut)
*/
static inline void PA_SetKeyboardScreen(u8 screen){
	keyb_screen = screen; // Top screen
} 

/** @} */ // end of Keyboard

#ifdef __cplusplus
}
#endif

#endif
