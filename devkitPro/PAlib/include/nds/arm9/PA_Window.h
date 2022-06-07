#ifndef _PA_Window
#define _PA_Window


/*! \file PA_Window.h
    \brief Window options...

   Window options, nothing much to say.
*/

// utilisé pour activer ou non les fenetres...
#define WIN_BG0 1
#define WIN_BG1 2
#define WIN_BG2 4
#define WIN_BG3 8
#define WIN_OBJ 16
#define WIN_SFX 32

#define DISPCNTL(screen) _REG16(0x04000000 + (screen * 0x1000))
#define DISPCNTH(screen) _REG16(0x04000002 + (screen * 0x1000))

#define WINDOW0 (1 << 13)
#define WINDOW1 (1 << 14)
#define WINDOWOBJ (1 << 15)

#define WIN0X(screen) _REG16(0x04000040 + (screen * 0x1000))
#define WIN0Y(screen) _REG16(0x04000044 + (screen * 0x1000))
#define WIN1X(screen) _REG16(0x04000042 + (screen * 0x1000))
#define WIN1Y(screen) _REG16(0x04000046 + (screen * 0x1000))

#define WININ(screen) _REG16(0x04000048 + (screen * 0x1000))
#define WINOUT(screen) _REG16(0x0400004A + (screen * 0x1000))

extern const s16 winfades[][4];

/** @defgroup Window Window system
 *  Set up 2 windows and a possible object window...
 *  @{
 */

/*! \def PA_SetWin0XY(screen, x1, y1, x2, y2)
    \brief
         \~english Set the X et Y coordinates of the rectangular first window. You'll also have to use PA_SetWin0 to chose which Backgrounds are visible and if sprites are too...
         \~french Régler les coordonnées X et Y du rectangle de la première fenetre. Il faudra aussi utiliser PA_SetWin0 pour choisir quels fonds sont visibles et si les sprites le sont ou non...
    \~\param screen
         \~english Screen...
         \~french Ecran...
    \~\param x1
         \~english X coordinate of the top left point
         \~french Coordonnée X du coin supérieur gauche
    \~\param y1
         \~english Y coordinate of the top left point
         \~french Coordonnée Y du coin supérieur gauche
    \~\param x2
         \~english X coordinate of the bottom right point
         \~french Coordonnée X du coin inférieur droit
    \~\param y2
         \~english Y coordinate of the bottom right point
         \~french Coordonnée Y du coin inférieur droit
*/
static inline void PA_SetWin0XY(u8 screen, u8 x1, u8 y1, u8 x2, u8 y2) {
	WIN0X(screen) = x2 + ((x1) << 8); 
	WIN0Y(screen) = y2 + ((y1) << 8);
}

/*! \def PA_SetWin1XY(screen, x1, y1, x2, y2)
    \brief
         \~english Set the X et Y coordinates of the rectangular second window. You'll also have to use PA_SetWin1 to chose which Backgrounds are visible and if sprites are too...
         \~french Régler les coordonnées X et Y du rectangle de la deuxième fenetre. Il faudra aussi utiliser PA_SetWin1 pour choisir quels fonds sont visibles et si les sprites le sont ou non...
    \~\param screen
         \~english Screen...
         \~french Ecran...
    \~\param x1
         \~english X coordinate of the top left point
         \~french Coordonnée X du coin supérieur gauche
    \~\param y1
         \~english Y coordinate of the top left point
         \~french Coordonnée Y du coin supérieur gauche
    \~\param x2
         \~english X coordinate of the bottom right point
         \~french Coordonnée X du coin inférieur droit
    \~\param y2
         \~english Y coordinate of the bottom right point
         \~french Coordonnée Y du coin inférieur droit
*/
#define PA_SetWin1XY(screen, x1, y1, x2, y2) do{WIN1X(screen) = x2 + ((x1) << 8); WIN1Y(screen) = y2 + ((y1) << 8);}while(0)

/*! \def PA_EnableWin0(screen, bg_sprites)
    \brief
         \~english Enable and set which backgrounds will be visible and whether sprites will too or not, for Window 0. You'll then have to configure it with PA_SetWin0XY
         \~french Activer et choisir quels fonds seront visibles dans la fenetre, et si les sprites le seront ou pas, pour la fenetre 0. Il faudra ensuite régler la taille avec PA_SetWin0XY
    \~\param screen
         \~english Screen...
         \~french Ecran...
    \~\param bg_sprites
         \~english Backgrounds and sprites, use the following macro : WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ | WIN_SFX (for special effects)
         \~french Fond et sprites à afficher, que l'on choisi de la facon suivante : WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ | WIN_SFX (pour les effets spéciaux)
*/
#define PA_EnableWin0(screen, bg_sprites) do{DISPCNTL(screen) |= WINDOW0; WININ(screen) &= 255; WININ(screen) |= (bg_sprites);}while(0)

/*! \def PA_DisableWin0(screen)
    \brief
         \~english Disable the first window...
         \~french Désactiver la première fenetre...
    \~\param screen
         \~english Screen...
         \~french Ecran...
*/
#define PA_DisableWin0(screen) DISPCNTL(screen) &= ~WINDOW0

/*! \def PA_EnableWin1(screen, bg_sprites)
    \brief
         \~english Enable and set which backgrounds will be visible and whether sprites will too or not, for Window 1. You'll then have to configure it with PA_SetWin1XY
         \~french Activer et choisir quels fonds seront visibles dans la fenetre, et si les sprites le seront ou pas, pour la fenetre 1. Il faudra ensuite régler la taille avec PA_SetWin1X    
	\~\param screen
         \~english Screen...
         \~french Ecran...
    \~\param bg_sprites
         \~english Backgrounds and sprites, use the following macro : WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ | WIN_SFX (for special effects)
         \~french Fond et sprites à afficher, que l'on choisi de la facon suivante : WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ | WIN_SFX (pour les effets spéciaux)
*/
#define PA_EnableWin1(screen, bg_sprites) do{DISPCNTL(screen) |= WINDOW1; WININ(screen) &= 255; WININ(screen) |= ((bg_sprites) << 8);}while(0)

/*! \def PA_DisableWin1(screen)
    \brief
         \~english Disable the second window...
         \~french Désactiver la deuxième fenetre...
    \~\param screen
         \~english Screen...
         \~french Ecran...
*/
#define PA_DisableWin1(screen) DISPCNTL(screen) &= ~WINDOW1

/*! \fn static inline void PA_EnableWinObj(u8 screen, u16 bg_sprites)
    \brief
         \~english Enable and set which backgrounds will be visible and whether sprites will too or not, for Object Winodw (created from sprites in Window mode).
         \~french Activer et choisir quels fonds seront visibles dans la fenetre, et si les sprites le seront ou pas, pour la fenetre objets (créée à partir des sprites en mode Window).
    \~\param screen
         \~english Screen...
         \~french Ecran...
    \~\param bg_sprites
         \~english Backgrounds and sprites, use the following macro : WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ | WIN_SFX (for special effects)
         \~french Fond et sprites à afficher, que l'on choisi de la facon suivante : WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ | WIN_SFX (pour les effets spéciaux)
*/
static inline void PA_EnableWinObj(u8 screen, u16 bg_sprites) {
	DISPCNTL(screen) |= WINDOWOBJ; 
	WINOUT(screen) &= 255; 
	WINOUT(screen) |= (bg_sprites << 8);
}

/*! \def PA_DisableWinObj(screen)
    \brief
         \~english Disable the object window...
         \~french Désactiver la fenetre objet...
    \~\param screen
         \~english Screen...
         \~french Ecran...
*/
#define PA_DisableWinObj(screen) DISPCNTL(screen) &= ~WINDOWOBJ

/*! \def PA_SetOutWin(screen, bg_sprites)
    \brief
         \~english Set which backgrounds will be visible and whether sprites will too or not, outside of the windows
         \~french Choisir quels fonds seront visibles dans la fenetre, et si les sprites le seront ou pas, en dehors des fenetres
    \~\param screen
         \~english Screen...
         \~french Ecran...
    \~\param bg_sprites
         \~english Backgrounds and sprites, use the following macro : WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ
         \~french Fond et sprites à afficher, que l'on choisi de la facon suivante : WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ
*/
#define PA_SetOutWin(screen, bg_sprites) do{WINOUT(screen) &= ~255; WINOUT(screen) |= bg_sprites;}while(0)

/*! \fn static inline void PA_WindowFade(u8 screen, u8 type, u8 time)
    \brief
         \~english This allows you to do fade in and out, using the window system
         \~french Permet de faire des entrées/sorties en utilisant le systeme de fenetre
    \~\param screen
         \~english Screen...
         \~french Ecran...
    \~\param type
         \~english Type... 8 different types are available (0-7)
         \~french Type... 8 différents types sont disponibles (0-7)
    \~\param time
         \~english Time, from 0 to 32 (included). 0 is a completely viewable screen, 32 is completely out
         \~french Temps, de 0 à 32 (inclus). 0 est écran visisble, 32 invisible	 
*/
static inline void PA_WindowFade(u8 screen, u8 type, u8 time){
	PA_EnableWin0(screen, WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ);
	PA_SetWin0XY(screen, ((time*winfades[type][0])*255)>>8, 
						(time*winfades[type][1]), 
						(((time*winfades[type][2]) + 256)*255)>>8, 
						(time*winfades[type][3]) + 192);
	if (time == 0) PA_DisableWin0(screen);
}

/** @} */ // end of Window
//////////////////////////////////////////////////////////////////////

#endif
