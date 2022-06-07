#ifndef _PA_TEXT
#define _PA_TEXT


#ifdef __cplusplus
extern "C" {
#endif

#include "PA_BgTiles.h"

extern u8 *bit8_tiles[10];

// DEPRECATED
#define PA_InitCustomTextEx(screen, bg_select, tiles, map, pal) do{\
	PA_DEPRECATED_MACRO;\
	PAbgtext[screen] = bg_select;\
	PA_LoadSimpleBg(screen, bg_select, tiles, NULL, BG_256X256, 0, 1);\
	PA_LoadBgPal(screen, bg_select, (void*)pal);\
	PA_textmap[screen] = (u16*)map;PA_textpal[screen] = (u16*)pal;PAtext_pal[screen]=0;\
	u32 PAtexttempvar = 0; for (PAtexttempvar = 0; PAtexttempvar < 32*24; PAtexttempvar++) PA_SetTileLetter(screen, PAtexttempvar, 0, 0);\
}while(0)

void PA_CreateTextPal(u8 screen, u8 pal_number, u16 r, u16 g, u16 b);

extern u32 pa_printx[2];
extern u32 pa_printy[2];
extern s16 pa_printscroll[2];

void PA_PrintYPlus(u8 screen);

/*! \file PA_Text.h
    \brief Text system in tile mode

    Not much to say, just a good debug option...
*/

#define TEXT_WHITE 0
#define TEXT_RED 1
#define TEXT_GREEN 2
#define TEXT_BLUE 3
#define TEXT_MAGENTA 4
#define TEXT_CYAN 5
#define TEXT_YELLOW 6
#define TEXT_LGRAY 7
#define TEXT_DGRAY 8
#define TEXT_BLACK 9

//////////////////////////////////////////////////////////////////////
// Système pour le texte
//////////////////////////////////////////////////////////////////////

extern u16 textcol[2]; // Garde en mémoire la couleur du texte
extern u8 PAbgtext[2];

extern u16 *PA_textmap[2]; //Pointeur vers la map et les tiles...
extern u8 *PA_texttiles[2];
extern u16 *PA_textpal[2];

extern s8 PA_font[2];  // 0 pour normal, 1 pour dégradé, -1 pour custom

extern const u8 *textData[10];
extern u8 policeheight[10];
extern u16 policewidth[10];
extern u8 *policesize[10];	

extern u8 *text_tiles[10];
extern u16 *text_maps[10];	

extern u8 *pa8bitdefaultsize[10];
extern u8 police8bitheight[10];

typedef void(*letterfp)(u8 size, u8 screen, u16 x, u16 y, char lettertemp, u8 color);
typedef void(*letterfp16bit)(u8 size, u8 screen, u16 x, u16 y, char lettertemp, u16 color);

extern letterfp letters[5];

extern u16 PAtext_pal[2];

typedef struct{
	u8 x1, x2, y1, y2;
} textborders;
extern textborders PA_TextBox[2];

/** @defgroup Text Text output system
 *  Allows you to output text...
 *  @{
 */

/// Old name for PA_LoadDefaultText().
#define PA_InitText PA_LoadDefaultText

/*! \fn void PA_LoadDefaultText(u8 screen, u8 bg_select)
    \brief
         \~english Load and initialize the default text. Works only in modes 0-2
         \~french Charger et initialiser le texte. Ne marche qu'en modes 0-2
    \~\param screen
         \~english Choose the screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param bg_select
      \~english Background number (0-3)
      \~french Numéro du fond que l'on veut tourner (0-3)
*/

void PA_LoadDefaultText(u8 screen, u8 bg_select);


/*! \def PA_SetTileLetter(screen, x, y, letter)
    \brief
         \~english Output a letter on the DS screen.
         \~french Ecrire une lettre à l'écran.
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param x
         \~english X coordinate in TILES (0-31) where to write the letter
         \~french Coordonnée X en TILES (0-31) où afficher la lettre
    \~\param y
         \~english Y coordinate in TILES (0-19) where to write the letter
         \~french Coordonnée Y en TILES (0-19) où afficher la lettre
    \~\param letter
         \~english Letter... 'a', 'Z', etc...
         \~french Lettre... 'a', 'Z', etc...
*/
#define PA_SetTileLetter(screen, x, y, letter) PA_SetMapTileAll(screen, PAbgtext[screen], x, y, (PA_textmap[screen][(u16)letter]&((1<<12)-1)) + (PAtext_pal[screen] << 12))



/*! \fn static inline void PA_SetTextTileCol(u8 screen, u8 color)
    \brief
         \~english Change the text writing color (does not change the current text's color)
         \~french Change la couleur du texte à écrire (ne change pas la couleur du texte déjà écrit)
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param color
         \~english Color, from 0 to 6, just test to see the result...
         \~french Couleur de 0 à 6, suffit de tester pour voir le résultat :)
*/
static inline void PA_SetTextTileCol(u8 screen, u8 color){
	PAtext_pal[screen] = color;
}

/*! \fn void PA_OutputText(u8 screen, u16 x, u16 y, const char* text, ...)
    \brief
         \~english Output text on the DS screen. Works only in modes 0-2
         \~french Ecrire du texte à l'écran. Ne marche qu'en modes 0-2
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param x
         \~english X coordinate in TILES (0-31) where to begin writing the text
         \~french Coordonnée X en TILES (0-31) où commencer à afficher le text
    \~\param y
         \~english Y coordinate in TILES (0-19) where to begin writing the text
         \~french Coordonnée Y en TILES (0-19) où commencer à afficher le text
    \~\param text
         \~english String to output. The following commands are avaiblable : \%s to output another string, \%d to output a value, \%fX to output a float with X digits, \\n to go to the line. Here's an example : PA_OutputText(0, 0, 1, "My name is %s and I have only %d teeth", "Mollusk", 20);
         \~french Chaine de caractère à écrire. On dispose des commandes suivantes : \%s pour une autre chaine de caractères, \%d pour écrire la valeur d'une variables, \%fX pour afficher un nombre avec X chiffres après la virgule, \\n pour aller à la ligne. Voici un exemple : PA_OutputText(0, 0, 1, "Mon nom est %s et je n'ai que %d dents...", "Mollusk", 20);
*/
void PA_OutputText(u8 screen, u16 x, u16 y, const char* text, ...);

/*! \fn u16 PA_OutputSimpleText(u8 screen, u16 x, u16 y, const char *text)
    \brief
         \~english Output simple text on the DS screen. Works only in modes 0-2. Much faster than PA_OutputText, but much more limited... Returns the number of letters
         \~french Ecrire du texte tout simple à l'écran. Ne marche qu'en modes 0-2. Beaucoup plus rapide que PA_OutputText, masi aussi beaucoup plus limité... Renvoie le nombre de lettres
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param x
         \~english X coordinate in TILES (0-31) where to begin writing the text
         \~french Coordonnée X en TILES (0-31) où commencer à afficher le text
    \~\param y
         \~english Y coordinate in TILES (0-19) where to begin writing the text
         \~french Coordonnée Y en TILES (0-19) où commencer à afficher le text
    \~\param text
         \~english String to output. 
         \~french Chaine de caractère à écrire.
*/
u16 PA_OutputSimpleText(u8 screen, u16 x, u16 y, const char *text);

/*! \fn u32 PA_BoxText(u8 screen, u16 basex, u16 basey, u16 maxx, u16 maxy, const char *text, u32 limit)
    \brief
         \~english Output text on the DS screen. This text is limited to a chosen box, and you can chose the number of letters to output (can be used to show 'typed' text, just put 10000 if you want to show all the text...). Returns the number of letters outputed
         \~french Permet d'écrire du texte à l'écran, dans une boite délimitée au choix, et en choisissant le nombre de lettres à afficher (peut être utile pour afficher du texte en train de se taper, sinon suffit de mettre 10000 pour afficher tout d'un coup) Renvoie le nmobre de lettre écrites
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param basex
         \~english X coordinate in TILES (0-31) where to begin writing the text
         \~french Coordonnée X en TILES (0-31) où commencer à afficher le text
    \~\param basey
         \~english Y coordinate in TILES (0-19) where to begin writing the text
         \~french Coordonnée Y en TILES (0-19) où commencer à afficher le text
    \~\param maxx
         \~english X coordinate in TILES (0-31) where to stop writing the text
         \~french Coordonnée X en TILES (0-31) où finir d'afficher le text
    \~\param maxy
         \~english Y coordinate in TILES (0-19) where to stop writing the text
         \~french Coordonnée Y en TILES (0-19) où finir d'afficher le text		 
    \~\param text
         \~english String to output. 
         \~french Chaine de caractère à écrire.
    \~\param limit
         \~english Maximum number of letters to show this time
         \~french Nombre maximum de lettres à afficher pour ce coup-ci 
*/
u32 PA_BoxText(u8 screen, u16 basex, u16 basey, u16 maxx, u16 maxy, const char *text, u32 limit);

/*! \fn u32 PA_BoxTextNoWrap(u8 screen, u16 basex, u16 basey, u16 maxx, u16 maxy, const char *text, u32 limit)
    \brief
         \~english Output text on the DS screen. This text is limited to a chosen box, and you can chose the number of letters to output (can be used to show 'typed' text, just put 10000 if you want to show all the text...). Returns the number of letters outputed. This function does not support word wrapping
         \~french Permet d'écrire du texte à l'écran, dans une boite délimitée au choix, et en choisissant le nombre de lettres à afficher (peut être utile pour afficher du texte en train de se taper, sinon suffit de mettre 10000 pour afficher tout d'un coup) Renvoie le nombre de lettre écrites. Cette fonction coupe les mots...
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param basex
         \~english X coordinate in TILES (0-31) where to begin writing the text
         \~french Coordonnée X en TILES (0-31) où commencer à afficher le text
    \~\param basey
         \~english Y coordinate in TILES (0-19) where to begin writing the text
         \~french Coordonnée Y en TILES (0-19) où commencer à afficher le text
    \~\param maxx
         \~english X coordinate in TILES (0-31) where to stop writing the text
         \~french Coordonnée X en TILES (0-31) où finir d'afficher le text
    \~\param maxy
         \~english Y coordinate in TILES (0-19) where to stop writing the text
         \~french Coordonnée Y en TILES (0-19) où finir d'afficher le text		 
    \~\param text
         \~english String to output. 
         \~french Chaine de caractère à écrire.
    \~\param limit
         \~english Maximum number of letters to show this time
         \~french Nombre maximum de lettres à afficher pour ce coup-ci 
*/
u32 PA_BoxTextNoWrap(u8 screen, u16 basex, u16 basey, u16 maxx, u16 maxy, const char *text, u32 limit);

/*!
    \fn static inline void PA_SetTextCol(u8 screen, u16 r, u16 g, u16 b)
    \brief
      \~english Change the screen text's default color
      \~french Changer la couleur de base du texte à l'écran
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param r
      \~english Red amount (0-31)
      \~french Quantité de rouge (0-31)
    \~\param g
      \~english Green amount (0-31)
      \~french Quantité de vert (0-31)
    \~\param b
      \~english Blue amount (0-31)
      \~french Quantité de bleu (0-31)
*/
static inline void PA_SetTextCol(u8 screen, u16 r, u16 g, u16 b){
	PA_CreateTextPal(screen, 0, r, g, b);
}

/*!
    \def PA_InitCustomText(screen, bg_select, text)
	\deprecated
    \brief
      \~english [DEPRECATED] Init the text using one of your own fonts ! 
      \~french [DEPRECATED] Initialiser le texte en utilisant une police perso
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param bg_select
         \~english Background number...
         \~french Numéro du fond...	 
    \~\param text
      \~english Font image file name converted with PAGfx
      \~french Image de la police, converti avec PAGfx

*/
#define PA_InitCustomText(screen, bg_select, text) PA_InitCustomTextEx(screen, bg_select, text##_Tiles, text##_Map, text##_Pal)

/*!
    \fn void PA_LoadText(u8 screen, u8 bg_select, const PA_BgStruct* font)
    \brief
      \~english Load and initialize a custom font. 
      \~french Initialiser le texte en utilisant une police perso
    \~\param screen
         \~english Chose the screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param bg_select
         \~english Background number...
         \~french Numéro du fond...	 
    \~\param font
      \~english Pointer to the font
      \~french Pointeur vers la police
*/

void PA_LoadText(u8 screen, u8 bg_number, const PA_BgStruct* font);

/*!
    \def PA_ShowFont(screen)
    \brief
      \~english Show the current font used. This is just for debug, no real use ingame
      \~french Affiche la police utilisée. C'est juste utile pour du débuggage, aucun intéret autrement
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
*/
#define PA_ShowFont(screen) PA_LoadBgMap(screen, PAbgtext[screen], (void*)PA_textmap[screen], BG_256X256)

/*! \fn s16 PA_8bitText(u8 screen, s16 basex, s16 basey, s16 maxx, s16 maxy, const char* text, u8 color, u8 size, u8 transp, s32 limit)
    \brief
         \~english This is a variable width and variable size function to draw text on the screen. It draws on an 8 bit background (see PA_Init8bitBg for more info), and has options such as size, transaprency, and box limits, as well as the color. Only problem : it does not take commands such as %d, etc... The function returns the number of characters it outputed
         \~french Cette fonction permet d'écrire du texte à chasse variable à l'écran. Elle nécessite d'avoir un fond dessinable de 8 bits (cf PA_Init8bitBg). Les options sont la taille, la transparence, et les limites, ainsi que la couleur. Seul inconvénient : il n'accepte pas les commande comme %d, etc... La fonction renvoie le nombre de charactères écrits
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param basex
         \~english X coordinate of the top left corner
         \~french Coordonnée X du coin supérieur gauche
    \~\param basey
         \~english Y coordinate of the top left corner
         \~french Coordonnée Y du coin supérieur gauche
    \~\param maxx
         \~english X coordinate of the down right corner
         \~french Coordonnée X du coin inférieur droit
    \~\param maxy
         \~english Y coordinate of the down right corner
         \~french Coordonnée Y du coin inférieur droit
    \~\param text
         \~english Text, such as "Hello World"
         \~french Texte, tel que "Hello World"
    \~\param color
         \~english Palette color to use (0-255)
         \~french Couleur de la palette à utiliser (0-255)
    \~\param size
         \~english Size of the text, from 0 (really small) to 4 (pretty big)
         \~french Taille du texte, de 0 (vraiment petit) à 4 (assez grand) 
    \~\param transp
         \~english Transparency. Setting this to 0 will overwrite all drawing in the text zone. 1 will write the text without erasing the drawing. 2 won't output anything (just to count the letters), 3 is rotated one way, 4 rotated the other way
         \~french Transparence. Mettre à 0 effecera tout dessin de la zone de texte. 1 écrira le texte par-dessus le dessin sans l'effacer. 2 n'écrira rien (juste pour compter les lettres). 3 fera un texte tourné à 90°. 4 est un texte tourné dans l'autre sens.	 
    \~\param limit
         \~english You can give a maximum number of characters to output. This can be usefull to have a slowing drawing text (allow to draw 1 more character each frame...)
         \~french On peut fixer une limite au nombre de caractères. Ceci peut etre utile pour dessiner un texte progressivement, en augmentant de 1 le nombre de caractères à chaque boucle....
*/
s16 PA_8bitText(u8 screen, s16 basex, s16 basey, s16 maxx, s16 maxy, const char* text, u8 color, u8 size, u8 transp, s32 limit);
s16 PA_16bitText(u8 screen, s16 basex, s16 basey, s16 maxx, s16 maxy, const char* text, u16 color, u8 size, u8 transp, s32 limit);

/*! \fn s16 PA_CenterSmartText(u8 screen, s16 basex, s16 basey, s16 maxx, s16 maxy, const char* text, u8 color, u8 size, u8 transp)
    \brief
         \~english Basicaly the same as the SmartText function, but this time centered...
         \~french En gros la meme chose que SmartText, mais en centré...
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param basex
         \~english X coordinate of the top left corner
         \~french Coordonnée X du coin supérieur gauche
    \~\param basey
         \~english Y coordinate of the top left corner
         \~french Coordonnée Y du coin supérieur gauche
    \~\param maxx
         \~english X coordinate of the down right corner
         \~french Coordonnée X du coin inférieur droit
    \~\param maxy
         \~english Y coordinate of the down right corner
         \~french Coordonnée Y du coin inférieur droit
    \~\param text
         \~english Text, such as "Hello World"
         \~french Texte, tel que "Hello World"
    \~\param color
         \~english Palette color to use (0-255)
         \~french Couleur de la palette à utiliser (0-255)
    \~\param size
         \~english Size of the text, from 0 (really small) to 4 (pretty big)
         \~french Taille du texte, de 0 (vraiment petit) à 4 (assez grand) 
    \~\param transp
         \~english Transparency. Setting this to 0 will overwrite all drawing in the text zone. 1 will write the text without erasing the drawing. 2 won't output anything (just to count the letters), 3 is rotated one way, 4 rotated the other way
         \~french Transparence. Mettre à 0 effecera tout dessin de la zone de texte. 1 écrira le texte par-dessus le dessin sans l'effacer. 2 n'écrira rien (juste pour compter les lettres). 3 fera un texte tourné à 90°. 4 est un texte tourné dans l'autre sens.	 
*/
s16 PA_CenterSmartText(u8 screen, s16 basex, s16 basey, s16 maxx, s16 maxy, const char* text, u8 color, u8 size, u8 transp);

/*! \def PA_8bitCustomFont(bit8_slot, bit8_font)
	\deprecated
    \brief
         \~english [DEPRECATED] Add custom fonts to the 8bit Font system !! Font must be converted with PAGfx
         \~french [DEPRECATED] Ajouter une police perso dans le systeme de texte 8bit !! Doit être convertie avec PAGfx
    \~\param bit8_slot
         \~english Font slot... 0-4 are used by the defaut PAlib fonts, 5-9 are free to use. You can freely overwrite the PAlib fonts if you want
         \~french Slot pour ajouter la police. Les slots 0-4 sont utilisés pour les polices par défaut de PAlib, et 5-9 sont libres. On peut néanmoins charger par-dessus les polices PAlib si on veut
    \~\param bit8_font
         \~english Font name;..
         \~french Nom de la police... 
*/
#define PA_8bitCustomFont(bit8_slot, bit8_font)do{\
	PA_DEPRECATED_MACRO;\
	bittext_maps[bit8_slot] = (u16*)(void*)bit8_font##_Map; \
	bit8_tiles[bit8_slot] = (u8*)bit8_font##_Tiles;	\
	pa_bittextdefaultsize[bit8_slot] = (u8*)bit8_font##_Sizes;	\
	pa_bittextpoliceheight[bit8_slot] = bit8_font##_Height;\
}while(0)

/*! \fn void PA_AddBitmapFont(int slot, const PA_BgStruct* font)
    \brief
         \~english Add a custom font to the 8bit/16bit font system.
         \~french Ajouter une police perso dans le système de texte 8bit/16bit.
    \~\param slot
         \~english Font slot. 0-4 are used by the default PAlib fonts, 5-9 are free to use. You can freely overwrite the PAlib fonts if you want.
         \~french Slot pour ajouter la police. Les slots 0-4 sont utilisés pour les polices par défaut de PAlib, et 5-9 sont libres. On peut néanmoins charger par-dessus les polices PAlib si on veut.
    \~\param font
         \~english Pointer to the font.
         \~french Pointeur vers le police perso.
*/
void PA_AddBitmapFont(int slot, const PA_BgStruct* font);

/*! \fn void PA_InitTextBorders(u8 screen, u8 x1, u8 y1, u8 x2, u8 y2)
    \brief
         \~english Initialise a text box with it's borders. This makes writing in a delimited area much easier...
         \~french Initialise une boite à texte, avec la bordure. Ceci rend l'utilisation des textes délimités bien plus simple
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param x1
         \~english Left limit in tiles
         \~french Limite gauche en tiles
    \~\param y1
         \~english Top
         \~french Haut
    \~\param x2
         \~english Right
         \~french Droite
    \~\param y2
         \~english Bottom
         \~french Bas 
*/
void PA_InitTextBorders(u8 screen, u8 x1, u8 y1, u8 x2, u8 y2);

/*! \fn void PA_EraseTextBox(u8 screen)
    \brief
         \~english Erases the text in a textbox. Requires that that box be initialized with PA_InitTextBorders
         \~french Efface le text d'un boite à texte... Nécessite qu'il ait été initialisé avec PA_InitTextBorders
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
*/
void PA_EraseTextBox(u8 screen);

/*! \fn static inline u32 PA_SimpleBoxText(u8 screen, const char *text, u32 limit)
    \brief
         \~english Write text in an initiliazed textbox. Similar to PA_BoxText, but without needing the text limits
         \~french Ecrit du texte dans une zone délimitée. Similaire à PA_BoxText, mais sans avoir besoin de délimiter
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param text
         \~english String to output. 
         \~french Chaine de caractère à écrire.
    \~\param limit
         \~english Maximum number of letters to show this time
         \~french Nombre maximum de lettres à afficher pour ce coup-ci 		 
*/
static inline u32 PA_SimpleBoxText(u8 screen, const char *text, u32 limit){
	return PA_BoxText(screen, PA_TextBox[screen].x1+1, PA_TextBox[screen].y1+1, PA_TextBox[screen].x2-1, PA_TextBox[screen].y2-1, text, limit);
}

/*! \fn void PA_ClearTextBg(u8 screen)
    \brief
         \~english Erase all the text on a given screen
         \~french Effacer tout le texte sur un écran donné
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
*/
void PA_ClearTextBg(u8 screen);

/*! \fn void PA_Print(u8 screen, const char* text, ...)
    \brief
         \~english Output text on the DS screen. Works like a printf function
         \~french Ecrire du texte à l'écran. Marche comme la fonction printf
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param text
         \~english String to output. The following commands are avaiblable : \%s to output another string, \%d to output a value, \%fX to output a float with X digits, \\n to go to the line. Here's an example : PA_OutputText(0, 0, 1, "My name is %s and I have only %d teeth", "Mollusk", 20);
         \~french Chaine de caractère à écrire. On dispose des commandes suivantes : \%s pour une autre chaine de caractères, \%d pour écrire la valeur d'une variables, \%fX pour afficher un nombre avec X chiffres après la virgule, \\n pour aller à la ligne. Voici un exemple : PA_OutputText(0, 0, 1, "Mon nom est %s et je n'ai que %d dents...", "Mollusk", 20);
*/
void PA_Print(u8 screen, const char* text, ...);

/*! \fn static inline void PA_PrintLetter(u8 screen, char letter)
    \brief
         \~english Like PA_Print, but for a letter
         \~french Comme PA_Print, mais juste pour une lettre
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param letter
         \~english Any letter...
         \~french Une lettre...
*/
static inline void PA_PrintLetter(u8 screen, char letter){
	PA_SetTileLetter(screen, pa_printx[screen], pa_printy[screen]&31, letter);
	pa_printx[screen]++;
	if(pa_printx[screen] >= 32){
 		PA_PrintYPlus(screen);
	}   
}

void PA_OutputTextSpecial0(u8 screen, int x1, int y,const char *text);
void PA_OutputTextSpecial1(u8 screen, int x1, int y,const char *text);
void PA_OutputTextSpecial2(u8 screen, int x1, int y,const char *text);
void PA_OutputTextSpecial3(u8 screen, int x1, int y,const char *text);
void PA_OutputTextSpecial4(u8 screen, int x1, int y,const char *text);
void PA_OutputTextSpecial5(u8 screen, int x1, int y,const char *text);

/** @} */ // end of Text

// Backwards compatibility
#define PA_SmartText PA_8bitText

#ifdef __cplusplus
}
#endif

#endif
