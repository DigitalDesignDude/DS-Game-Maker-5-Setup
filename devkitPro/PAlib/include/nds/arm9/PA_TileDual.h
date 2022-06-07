#ifndef _PA_TileDual
#define _PA_TileDual



/*! \file PA_TileDual.h
    \brief Everything concerning the Bg Tile modes on 2 screens

    This file contains all the macros and variables regarding Tile modes (0-2), loading tiles and Bg, etc... on 2 screens !
*/

#include "PA_BgTiles.h"

/** @defgroup TileDual Bg Modes on 2 Screens
 *  Load tiles, a map, scroll it... and 2 screens automatically
 *  @{
 */

/*!
    \fn static inline void PA_DualHideBg(u8 bg_select)
    \brief
      \~english Hide a background on both screens
      \~french Cacher un fond sur les 2 écrans
    \~\param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
*/
static inline void PA_DualHideBg(u8 bg_select){
	PA_HideBg(0, bg_select);
	PA_HideBg(1, bg_select);	
}

/*!
    \fn static inline void PA_DualShowBg(u8 bg_select)
    \brief
      \~english Show a hidden background, on both screens
      \~french Afficher un fond auparavant caché sur les 2 écrans
    \~\param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
*/
static inline void PA_DualShowBg(u8 bg_select){
	PA_ShowBg(0, bg_select);
	PA_ShowBg(1, bg_select);
}

/*!
    \fn static inline void PA_DualResetBg(void)
    \brief
      \~english Reinitialize de Bg system
      \~french Reinitialiser les fonds d'un écran. En fait ca ne fait que cacher tous les fonds
*/
static inline void PA_DualResetBg(void){
	PA_ResetBg(0);
	PA_ResetBg(1);
}

/*!
    \fn static inline void PA_DualDeleteBg(u8 bg_select)
    \brief
      \~english Delete a complete background (tiles + map + hide it...)
      \~french Effacer un fond complètement (tiles + map + cacher)
    \~\param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
*/
static inline void PA_DualDeleteBg(u8 bg_select){
	PA_DeleteBg(0, bg_select);
	PA_DeleteBg(1, bg_select);
}

/*!
    \def PA_DualLoadTiledBg(bg_number, bg_name)
	\deprecated
    \brief
      \~english [DEPRECATED] This will never get easier... Loads a background TiledBg converted with PAGfx, with it's tiles, map, and palette. Only 256 color mode available. On 2 screens as 1...
      \~french [DEPRECATED] On ne pourra jamais rendre ca plus simple... Charge un fond de type TiledBg converti avec PAGfx, en mettant les tiles, la map, et meme la palette ! Seulement en mode 256 couleurs. Sur 2 écrans, comme un seul grand
    \~\param bg_number
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
    \~\param bg_name
      \~english Background name, like bg0
      \~french Nom du fond, comme bg0
*/
#define PA_DualLoadTiledBg(bg_number, bg_name)do{\
	PA_DEPRECATED_MACRO;\
	PA_LoadTiledBg(0, bg_number, bg_name);\
	PA_LoadTiledBg(1, bg_number, bg_name);\
	PA_DualBGScrollY(bg_number, 0);}while(0)

/*!
    \def PA_DualLoadSimpleBg(bg_select, bg_tiles, bg_map, bg_size, wraparound, color_mode)
	\deprecated
    \brief
      \~english [DEPRECATED] Simplest way to load a Background on both screens
      \~french [DEPRECATED] Facon la plus simple de cahrger un fond sur les 2 écrans 
    \~\param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
    \~\param bg_tiles
      \~english Name of the tiles' info (example: ship_Tiles)
      \~french Nom du tableau contenant les tiles (exemple: ship_Tiles)
    \~\param bg_map
      \~english Name of the map's info (example : ship_Map)
      \~french Nom du tableau contenant les infos sur la map (exemple : ship_Map)
    \~\param bg_size
      \~english Background size. To use a normal background, use the macros BG_256X256, BG_256X512, etc...
      \~french Taille du fond. Pour un fond normal, on utilise les macros BG_256X256, BG_256X512, etc... 
    \~\param wraparound
      \~english If the background wraps around or not. More important for rotating backgrounds. 
      \~french Si le fond boucle ou non. C'est plus important pour les fonds rotatifs... 
    \~\param color_mode
      \~english Color mode : 0 for 16 color mode, 1 for 256...
      \~french Nombre de couleurs : 0 pour 16 couleurs, 1 pour 256
*/

#define PA_DualLoadSimpleBg(bg_select, bg_tiles, bg_map, bg_size, wraparound, color_mode) do{\
	PA_DEPRECATED_MACRO;\
	PA_LoadSimpleBg(0, bg_select, bg_tiles, bg_map, bg_size, wraparound, color_mode);\
	PA_LoadSimpleBg(1, bg_select, bg_tiles, bg_map, bg_size, wraparound, color_mode);\
	PA_DualBGScrollY(bg_select, 0);}while(0)

/*!
    \def PA_DualLoadRotBg(bg_select, bg_tiles, bg_map, bg_size, wraparound)
	\deprecated
    \brief
      \~english [DEPRECATED] Load a background fit for rotating/scaling ! Warning, you must use PA_SetVideoMode to 1 if you want 1 rotating background (Bg3 only !), or 2 for 2 rotating backgrounds (Bg2 and 3). The background MUST be in 256 colors
      \~french [DEPRECATED] Charger un fond pour les rotations/zoom ! Attention, il faut avant utiliser PA_SetVideoMode avec 1 pour utiliser un fond rotatif (le fond 3 uniquement !), ou 2 pour 2 fonds (2 et 3). Le fond DOIT etre de 256 couleurs
    \~\param bg_select
      \~english Background number to load
      \~french Numéro du fond que l'on veut charger
    \~\param bg_tiles
      \~english Name of the tiles' info (example: ship_Tiles)
      \~french Nom du tableau contenant les tiles (exemple: ship_Tiles)
    \~\param bg_map
      \~english Name of the map's info (example : ship_Map)
      \~french Nom du tableau contenant les infos sur la map (exemple : ship_Map)
    \~\param bg_size
      \~english Background size. Use the following macros : BG_ROT_128X128, or 256X256, 512X512, or 1024X1024
      \~french Taille du fond. Utiliser les macros suivantes : BG_ROT_128X128, ou 256X256, 512X512, ou enfin 1024X1024
    \~\param wraparound
      \~english If the background wraps around or not. 
      \~french Si le fond boucle ou non. 
*/
#define PA_DualLoadRotBg(bg_select, bg_tiles, bg_map, bg_size, wraparound) do{\
	PA_DEPRECATED_MACRO;\
	PA_LoadRotBg(0, bg_select, bg_tiles, bg_map, bg_size, wraparound);\
	PA_LoadRotBg(1, bg_select, bg_tiles, bg_map, bg_size, wraparound);\
	PA_DualBGScrollY(bg_select, 0);}while(0)

static inline void PA_DualSetBgRot(u8 bg_select, s32 x_scroll, s32 y_scroll, s32 x_rotcentre, s32 y_rotcentre, s16 bg_angle, s32 bg_zoom){
	PA_SetBgRot(0, bg_select, x_scroll, y_scroll, x_rotcentre, y_rotcentre, bg_angle, bg_zoom);
	PA_SetBgRot(1, bg_select, x_scroll, y_scroll, x_rotcentre, y_rotcentre, bg_angle, bg_zoom);
}

/*!
    \def PA_DualLoadBg(bg_select, bg_tiles, tile_size, bg_map, bg_size, wraparound, color_mode)
	\deprecated
    \brief
      \~english [DEPRECATED] Simplest way to load a Background. Combines PA_InitBg, PA_LoadBgTiles, and PA_LoadBgMap
      \~french [DEPRECATED] Facon la plus simple de cahrger un fond. Combine PA_InitBg, PA_LoadBgTiles, et PA_LoadBgMap
    \~\param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
    \~\param bg_tiles
      \~english Name of the tiles' info (example: ship_Tiles)
      \~french Nom du tableau contenant les tiles (exemple: ship_Tiles)
    \~\param tile_size
      \~english Size of your tileset
      \~french Taille du tilset
    \~\param bg_map
      \~english Name of the map's info (example : ship_Map)
      \~french Nom du tableau contenant les infos sur la map (exemple : ship_Map)
    \~\param bg_size
      \~english Background size. This is important, because it also determines whether the Bg is rotatable or not. To use a normal background, use the macros BG_256X256, BG_256X512, etc... For a rotatable Bg, use the macros BG_ROT_128X128...
      \~french Taille du fond. Ceci est très important, car ça détermine aussi si le Bg est rotatif ou non. Pour un fond normal, on utilise les macros BG_256X256, BG_256X512, etc... Por un fond rotatif, il suffit d'utiliser BG_ROT_128X128...
    \~\param wraparound
      \~english If the background wraps around or not. More important for rotating backgrounds. 
      \~french Si le fond boucle ou non. C'est plus important pour les fonds rotatifs... 
    \~\param color_mode
      \~english Color mode : 0 for 16 color mode, 1 for 256...
      \~french Nombre de couleurs : 0 pour 16 couleurs, 1 pour 256
*/

#define PA_DualLoadBg(bg_select, bg_tiles, tile_size, bg_map, bg_size, wraparound, color_mode) do{\
	PA_DEPRECATED_MACRO;\
	PA_LoadBg(0, bg_select, bg_tiles, tile_size, bg_map, bg_size, wraparound, color_mode);\
	PA_LoadBg(1, bg_select, bg_tiles, tile_size, bg_map, bg_size, wraparound, color_mode);\
	PA_DualBGScrollY(bg_select, 0);}while(0)

/*!
    \fn static inline void PA_DualBGScrollX(u8 bg_number, s16 x)
    \brief
      \~english Scroll horizontaly any background, on both screens
      \~french Scroll horizontal de n'importe quel fond, sur les 2 écrans
    \~\param bg_number
      \~english Background number (0-3)
      \~french Numéro du fond que l'on veut tourner (0-3)
    \~\param x
      \~english X value to scroll
      \~french Valeur X à déplacer, horizontalement...
*/
static inline void PA_DualBGScrollX(u8 bg_number, s16 x){
	PA_BGScrollX(0, bg_number, x);
	PA_BGScrollX(1, bg_number, x);
}

/*!
    \fn static inline void PA_DualBGScrollY(u8 bg_number, s16 y)
    \brief
      \~english Scroll vertically any background
      \~french Scroll vertical de n'importe quel fond
    \~\param bg_number
      \~english Background number (0-3)
      \~french Numéro du fond que l'on veut tourner (0-3)
    \~\param y
      \~english Y value to scroll
      \~french Valeur Y à déplacer, verticalement...
*/
static inline void PA_DualBGScrollY(u8 bg_number, s16 y){
	PA_BGScrollY(0, bg_number, y+PA_ScreenSpace);
	PA_BGScrollY(1, bg_number, y);
}

/*!
    \fn static inline void PA_DualBGScrollXY(u8 bg_number, s16 x, s16 y)
    \brief
      \~english Scroll horizontaly and vertically any background
      \~french Scroll horizontal et vertical de n'importe quel fond
    \~\param bg_number
      \~english Background number (0-3)
      \~french Numéro du fond que l'on veut tourner (0-3)
    \~\param x
      \~english X value to scroll
      \~french Valeur X à déplacer, horizontalement...
    \~\param y
      \~english Y value to scroll
      \~french Valeur Y à déplacer, verticalement...
*/
static inline void PA_DualBGScrollXY(u8 bg_number, s16 x, s16 y) {
	PA_DualBGScrollX(bg_number, x); 
	PA_DualBGScrollY(bg_number, y);
}

/*!
    \def PA_DualLoadPAGfxLargeBg(bg_number, bg_name)
	\deprecated
    \brief
      \~english [DEPRECATED] Completely load and initialise a background with infinite scrolling (usefull if larger or wider than 512 pixels), converted with PAGfx. Background on both screens, as one
      \~french [DEPRECATED] Charger et initialiser un fond pour le scrolling infini (pour les fonds de plus de 512 pixels de haut ou de large), converti avec PAGfx. Fond sur les 2 écrans comme un seul
    \~\param bg_number
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (0-3)
   \~\param  bg_name
       \~english Background name, in PAGfx
      \~french Nom du fond dans PAGfx 
*/
#define PA_DualLoadPAGfxLargeBg(bg_number, bg_name)do{\
	PA_DEPRECATED_MACRO;\
	PA_LoadPAGfxLargeBg(0, bg_number, bg_name);\
	PA_LoadPAGfxLargeBg(1, bg_number, bg_name);\
	PA_DualInfLargeScrollY(bg_number, 0);}while(0)

/*!
    \def PA_DualLoadLargeBg(bg_select, bg_tiles, bg_map, color_mode, lx, ly)
	\deprecated
    \brief
      \~english [DEPRECATED] Completely load and initialise a background with infinite scrolling (usefull if larger or wider than 512 pixels), on both screens
      \~french [DEPRECATED] Charger et initialiser un fond pour le scrolling infini (pour les fonds de plus de 512 pixels de haut ou de large), sur les 2 écrans
    \~\param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
    \~\param bg_tiles
      \~english Name of the tiles' info (example: ship_Tiles)
      \~french Nom du tableau contenant les tiles (exemple: ship_Tiles)
    \~\param bg_map
      \~english Name of the map's info (example : ship_Map)
      \~french Nom du tableau contenant les infos sur la map (exemple : ship_Map)
   \~\param color_mode
      \~english Color mode : 0 for 16 color mode, 1 for 256...
      \~french Nombre de couleurs : 0 pour 16 couleurs, 1 pour 256
   \~\param lx
      \~english Width, in tiles. So a 512 pixel wide map is 64 tiles wide...
      \~french Largeur, en tiles. Un fond de 512 pixels de large fera 64 tiles de large.
   \~\param ly
      \~english Height, in tiles. So a 512 pixel high map is 64 tiles high...
      \~french Hauteur, en tiles. Un fond de 512 pixels de hauy fera 64 tiles de haut.	  
*/
#define PA_DualLoadLargeBg(bg_select, bg_tiles, bg_map, color_mode, lx, ly)do{\
	PA_DEPRECATED_MACRO;\
	PA_LoadLargeBg(0, bg_select, bg_tiles, bg_map, color_mode, lx, ly);\
	PA_LoadLargeBg(1, bg_select, bg_tiles, bg_map, color_mode, lx, ly);\
	PA_DualInfLargeScrollY(bg_select, 0);}while(0)

/*!
    \def PA_DualLoadLargeBgEx(bg_select, bg_tiles, tile_size, bg_map, color_mode, lx, ly)
	\deprecated
    \brief
      \~english [DEPRECATED] Completely load and initialise a background with infinite scrolling (usefull if larger or wider than 512 pixels), but here you can put yourself the tile size...
      \~french [DEPRECATED] Charger et initialiser un fond pour le scrolling infini (pour les fonds de plus de 512 pixels de haut ou de large), mais ici on met soi-meme la taille des tiles
    \~\param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
    \~\param bg_tiles
      \~english Name of the tiles' info (example: ship_Tiles)
      \~french Nom du tableau contenant les tiles (exemple: ship_Tiles)
    \~\param tile_size
      \~english Size of your tileset
      \~french Taille du tilset
    \~\param bg_map
      \~english Name of the map's info (example : ship_Map)
      \~french Nom du tableau contenant les infos sur la map (exemple : ship_Map)
   \~\param color_mode
      \~english Color mode : 0 for 16 color mode, 1 for 256...
      \~french Nombre de couleurs : 0 pour 16 couleurs, 1 pour 256
   \~\param lx
      \~english Width, in tiles. So a 512 pixel wide map is 64 tiles wide...
      \~french Largeur, en tiles. Un fond de 512 pixels de large fera 64 tiles de large.
   \~\param ly
      \~english Height, in tiles. So a 512 pixel high map is 64 tiles high...
      \~french Hauteur, en tiles. Un fond de 512 pixels de hauy fera 64 tiles de haut.	  
*/
#define PA_DualLoadLargeBgEx(bg_select, bg_tiles, tile_size, bg_map, color_mode, lx, ly) do{\
	PA_DEPRECATED_MACRO;\
	PA_LoadLargeBgEx(0, bg_select, bg_tiles, tile_size, bg_map, color_mode, lx, ly);\
	PA_LoadLargeBgEx(1, bg_select, bg_tiles, tile_size, bg_map, color_mode, lx, ly);\
	PA_DualInfLargeScrollY(bg_select, 0);}while(0)

/*!
    \def PA_DualEasyBgLoad(bg_number, bg_name)
	\deprecated
    \brief
      \~english [DEPRECATED] EasyBg load, but for Dual screen...
      \~french [DEPRECATED] Chargement de fond EasyBg, mais pour le Dual Screen...
    \~\param bg_number
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (0-3)
   \~\param  bg_name
       \~english Background name, in PAGfx
      \~french Nom du fond dans PAGfx 
*/
#define PA_DualEasyBgLoad(bg_number, bg_name)do{\
	PA_DEPRECATED_MACRO;\
	PA_EasyBgLoad(0, bg_number, bg_name);\
	PA_EasyBgLoad(1, bg_number, bg_name);\
	PA_DualEasyBgScrollY(bg_number, 0);}while(0)

/*!
    \fn static inline void PA_DualEasyBgScrollX(u8 bg_select, s32 x)
    \brief
      \~english Scroll an EasyBg horizontaly. It must have been initialised with PA_LoadLargeBg.
      \~french Déplacer un fond EasyBg horizontalement. Doit etre initialisé avec PA_LoadLargeBg.
    \~\param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
    \~\param x
      \~english X value to scroll
      \~french Valeur X à déplacer
*/
static inline void PA_DualEasyBgScrollX(u8 bg_select, s32 x){
	PA_EasyBgScrollX(0, bg_select, x);
	PA_EasyBgScrollX(1, bg_select, x);
}

/*!
    \fn static inline void PA_DualEasyBgScrollY(u8 bg_select, s32 y)
    \brief
      \~english Scroll an EasyBg vertically.
      \~french Déplacer un fond EasyBg verticalement.
    \~\param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
    \~\param y
      \~english Y value to scroll
      \~french Valeur Y à déplacer
*/
static inline void PA_DualEasyBgScrollY(u8 bg_select, s32 y){
	PA_EasyBgScrollY(0, bg_select, y+PA_ScreenSpace);
	PA_EasyBgScrollY(1, bg_select, y);
}

/*!
    \fn static inline void PA_DualLoadBackground(u8 bg_number, const PA_BgStruct* bg)
    \brief
      \~english Load a background (EasyBg, RotBg or UnlimitedBg), but for Dual screen...
      \~french Charger un fond (EasyBg, RotBg or UnlimitedBg), mais pour le Dual Screen...
    \~\param bg_number
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (0-3)
    \~\param bg
         \~english Pointer to the background (struct)
         \~french Pointeur vers le fond
*/

static inline void PA_DualLoadBackground(u8 bg_number, const PA_BgStruct* bg){
	PA_LoadBackground(0, bg_number, bg);
	PA_LoadBackground(1, bg_number, bg);
	PA_DualEasyBgScrollY(bg_number, 0);
}

/*!
    \fn static inline void PA_DualEasyBgScrollXY(u8 bg_select, s32 x, s32 y)
    \brief
      \~english Scroll a Dual EasyBg.
      \~french Déplacer un fond EasyBg en Dual Screen.
    \~\param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
    \~\param x
      \~english X value to scroll
      \~french Valeur X à déplacer
    \~\param y
      \~english Y value to scroll
      \~french Valeur Y à déplacer
*/
static inline void PA_DualEasyBgScrollXY(u8 bg_select, s32 x, s32 y){
	PA_DualEasyBgScrollX(bg_select, x);
	PA_DualEasyBgScrollY(bg_select, y);
}

/*!
    \fn static inline void PA_DualInfLargeScrollX(u8 bg_select, s32 x)
    \brief
      \~english Scroll a large infinite scrolling background horizontaly. It must have been initialised with PA_LoadLargeBg.
      \~french Déplacer un fond à scrolling 'infini' horizontalement. Doit etre initialisé avec PA_LoadLargeBg.
    \~\param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
    \~\param x
      \~english X value to scroll
      \~french Valeur X à déplacer
*/
static inline void PA_DualInfLargeScrollX(u8 bg_select, s32 x){
	PA_InfLargeScrollX(0, bg_select, x);
	PA_InfLargeScrollX(1, bg_select, x);
}

/*!
    \fn static inline void PA_DualInfLargeScrollY(u8 bg_select, s32 y)
    \brief
      \~english Scroll a large infinite scrolling background vertically. It must have been initialised with PA_LoadLargeBg.
      \~french Déplacer un fond à scrolling 'infini' verticalement. Doit etre initialisé avec PA_LoadLargeBg.
    \~\param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
    \~\param y
      \~english Y value to scroll
      \~french Valeur Y à déplacer
*/
static inline void PA_DualInfLargeScrollY(u8 bg_select, s32 y){
	PA_InfLargeScrollY(0, bg_select, y+PA_ScreenSpace);
	PA_InfLargeScrollY(1, bg_select, y);
}

/*!
    \fn static inline void PA_DualInfLargeScrollXY(u8 bg_select, s32 x, s32 y)
    \brief
      \~english Scroll a large infinite scrolling background horizontaly and vertically. It must have been initialised with PA_LoadLargeBg.
      \~french Déplacer un fond à scrolling 'infini' horizontalement et verticalement. Doit etre initialisé avec PA_LoadLargeBg.
    \~\param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
    \~\param x
      \~english X value to scroll
      \~french Valeur X à déplacer
    \~\param y
      \~english Y value to scroll
      \~french Valeur Y à déplacer	  
*/
static inline void PA_DualInfLargeScrollXY(u8 bg_select, s32 x, s32 y){
	PA_DualInfLargeScrollX(bg_select, x);
	PA_DualInfLargeScrollY(bg_select, y);
}

/*!
    \fn static inline void PA_DualLargeScrollX(u8 bg_select, s32 x)
    \brief
      \~english Scroll a large background horizontaly. It must have been initialised with PA_LoadLargeBg.  This function does not wrap around, but is faster than the InfLargeScroll...
      \~french Déplacer un grand fond à scrolling horizontalement.Doit etre initialisé avec PA_LoadLargeBg. Cette fonction ne permet pas au fond de 'boucler' sur lui-meme, mais est bien plus rapide que InfLargeScroll...
    \~\param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
    \~\param x
      \~english X value to scroll
      \~french Valeur X à déplacer
*/
static inline void PA_DualLargeScrollX(u8 bg_select, s32 x){
	PA_LargeScrollX(0, bg_select, x);
	PA_LargeScrollX(1, bg_select, x);
}

/*!
    \fn static inline void PA_DualLargeScrollY(u8 bg_select, s32 y)
    \brief
      \~english Scroll a large background vertically. It must have been initialised with PA_LoadLargeBg. This function does not wrap around, but is faster than the InfLargeScroll...
      \~french Déplacer un grand fond à scrolling verticalement. Doit etre initialisé avec PA_LoadLargeBg.  Cette fonction ne permet pas au fond de 'boucler' sur lui-meme, mais est bien plus rapide que InfLargeScroll...
    \~\param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
    \~\param y
      \~english Y value to scroll
      \~french Valeur Y à déplacer
*/
static inline void PA_DualLargeScrollY(u8 bg_select, s32 y){
	PA_LargeScrollY(0, bg_select, y+PA_ScreenSpace);
	PA_LargeScrollY(1, bg_select, y);
}

/*!
    \fn static inline void PA_DualLargeScrollXY(u8 bg_select, s32 x, s32 y)
    \brief
      \~english Scroll a large background horizontaly and vertically. It must have been initialised with PA_LoadLargeBg. This function does not wrap around, but is faster than the InfLargeScroll...
      \~french Déplacer un grand fond à scrolling horizontalement et verticalement. Doit etre initialisé avec PA_LoadLargeBg. Cette fonction ne permet pas au fond de 'boucler' sur lui-meme, mais est bien plus rapide que InfLargeScroll...
    \~\param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
    \~\param x
      \~english X value to scroll
      \~french Valeur X à déplacer
    \~\param y
      \~english Y value to scroll
      \~french Valeur Y à déplacer	  
*/
static inline void PA_DualLargeScrollXY(u8 bg_select, s32 x, s32 y){
	PA_DualLargeScrollX(bg_select, x);
	PA_DualLargeScrollY(bg_select, y);
}

/*!
    \fn static inline void PA_DualInitParallaxX(s32 bg0, s32 bg1, s32 bg2, s32 bg3)
    \brief
      \~english Initialise Parallax Scrolling for multiple backgrounds, horizontaly. Chose the speed at which each background will scroll compared to the others. Then use PA_ParallaxScrollX to scroll...
      \~french Initialiser le Parallax Scrolling pour plusieurs fonds, horizontalement. Choix de la vitesse à laquelle les fonds vont défiler par rapport aux autres... Utiliser PA_ParallaxScrollX par la suite pour scroller
    \~\param bg0
      \~english Value for the first background (0). Set to 256 for normal scroll speed, lower for lower speed (128 is half speed...), higher for faster (512 is twice as fast...). You can set negative values. 0 inactivates parallax scrolling for this background
      \~french Valeur pour le premier fond (0). 256 met en vitesse normal, moins pour moins lent (128 pour moitié de vitesse), plus pour plus rapide (512 équivaut à 2 fois plus vite). On peut utiliser des valeurs négatives. 0 désactive le scrolling parallax pour ce fond
    \~\param bg1
      \~english Same thing for Background 1
      \~french Idem, pour le Fond 1
    \~\param bg2
      \~english Same thing for Background 2
      \~french Idem, pour le Fond 2
    \~\param bg3
      \~english Same thing for Background 3
      \~french Idem, pour le Fond 3
*/
static inline void PA_DualInitParallaxX(s32 bg0, s32 bg1, s32 bg2, s32 bg3) {
	 PA_InitParallaxX(0, bg0, bg1, bg2, bg3);
	 PA_InitParallaxX(1, bg0, bg1, bg2, bg3);
}

/*!
    \fn static inline void PA_DualInitParallaxY(s32 bg0, s32 bg1, s32 bg2, s32 bg3)
    \brief
      \~english Initialise Parallax Scrolling for multiple backgrounds, horizontaly. Chose the speed at which each background will scroll compared to the others. Then use PA_ParallaxScrollX to scroll...
      \~french Initialiser le Parallax Scrolling pour plusieurs fonds, horizontalement. Choix de la vitesse à laquelle les fonds vont défiler par rapport aux autres... Utiliser PA_ParallaxScrollX par la suite pour scroller
    \~\param bg0
      \~english Value for the first background (0). Set to 256 for normal scroll speed, lower for lower speed (128 is half speed...), higher for faster (512 is twice as fast...). You can set negative values. 0 inactivates parallax scrolling for this background
      \~french Valeur pour le premier fond (0). 256 met en vitesse normal, moins pour moins lent (128 pour moitié de vitesse), plus pour plus rapide (512 équivaut à 2 fois plus vite). On peut utiliser des valeurs négatives. 0 désactive le scrolling parallax pour ce fond
    \~\param bg1
      \~english Same thing for Background 1
      \~french Idem, pour le Fond 1
    \~\param bg2
      \~english Same thing for Background 2
      \~french Idem, pour le Fond 2
    \~\param bg3
      \~english Same thing for Background 3
      \~french Idem, pour le Fond 3
*/
static inline void PA_DualInitParallaxY(s32 bg0, s32 bg1, s32 bg2, s32 bg3) {
	PA_InitParallaxY(0, bg0, bg1, bg2, bg3);
	PA_InitParallaxY(1, bg0, bg1, bg2, bg3);	
}

/*!
    \fn static inline void PA_DualParallaxScrollX(s32 x)
    \brief
      \~english Scroll the backgrounds
      \~french Déplacer les fonds activés pour le parallax...
    \~\param x
      \~english X value to scroll
      \~french Valeur X à déplacer
*/
static inline void PA_DualParallaxScrollX(s32 x){
	int i;
	for(i = 0; i < 4; i++)
		if (PA_parallaxX[0][i])
			PA_DualEasyBgScrollX(i, (PA_parallaxX[0][i] * x) >> 8);
}

/*!
    \fn static inline void PA_DualParallaxScrollY(s32 y)
    \brief
      \~english Scroll the backgrounds
      \~french Déplacer les fonds activés pour le parallax...
    \~\param y
      \~english Y value to scroll
      \~french Valeur Y à déplacer
*/
static inline void PA_DualParallaxScrollY(s32 y){
	int i;
	for(i = 0; i < 4; i++)
		if (PA_parallaxY[0][i])
			PA_DualEasyBgScrollY(i, (PA_parallaxY[0][i] * y) >> 8);
}

/*!
    \fn static inline void PA_DualParallaxScrollXY(s32 x, s32 y)
    \brief
      \~english Scroll the backgrounds
      \~french Déplacer les fonds activés pour le parallax...
    \~\param x
      \~english X value to scroll
      \~french Valeur X à déplacer
    \~\param y
      \~english Y value to scroll
      \~french Valeur Y à déplacer
*/
static inline void PA_DualParallaxScrollXY(s32 x, s32 y){
	PA_DualParallaxScrollX(x);
	PA_DualParallaxScrollY(y);
}

/*!
    \fn static inline void PA_DualSetBgPrio(u8 bg, u8 prio)
    \brief
      \~english Change a backgrounds priority
      \~french Changer la priorité d'un fond
    \~\param bg
      \~english Background...
      \~french Numéro du fond...
    \~\param prio
      \~english Priority level (0-3, 0 being the highest)
      \~french Niveau de priorité, de 0 à 3, 0 étant priorité la plus élevée
*/
static inline void PA_DualSetBgPrio(u8 bg, u8 prio) {
	PA_SetBgPrio(0, bg, prio);
	PA_SetBgPrio(1, bg, prio);
}

/** @} */ // end of Backgrounds

// Pas utilisé en direct par le programmeur
void PA_InitLargeBg(u8 screen, u8 bg_select, s32 lx, s32 ly, void* bg_map);

#endif
