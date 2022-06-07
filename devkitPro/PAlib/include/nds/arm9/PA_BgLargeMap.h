#ifndef _PA_BGLARGEMAP
#define _PA_BGLARGEMAP

#include "PA_BgTiles.h"

// Tile engine functions, do not use !
#define PA_TileNumber(x, y)	(PA_Modulo((x)+1, 36)+(PA_Modulo((y)+1, 28)*36)) // Used by the tile engine

#define PA_LoadTileEngine(screen, bg_select, bg_tiles) do{\
	PA_DeleteBg(screen, bg_select);\
	PA_LoadBgTilesEx(screen, bg_select, (void*)NULL, (1008<<5));\
	PA_BgInfo[screen][bg_select].Tiles = (void*)bg_tiles;\
	PA_LoadBgMap(screen, bg_select, NULL, BG_512X256); \
	PA_InitBg(screen, bg_select, BG_512X256, 0, 1);\
	PA_BGScrollXY(screen, bg_select, 0, 0);}while(0)

#define INF_JUSTTILE 268435455
#define MAX_TILES 1024

void PA_InfTilesScrollX(u8 screen, u8 bg_select, s32 x);
void PA_InfTilesScrollY(u8 screen, u8 bg_select, s32 y);
void PA_InfLargeScrollXN(u8 screen, u8 bg_select, s32 x);
void PA_InfLargeScrollYN(u8 screen, u8 bg_select, s32 y);
void PA_LargeScrollXN(u8 screen, u8 bg_select, s32 x);
void PA_LargeScrollYN(u8 screen, u8 bg_select, s32 y);

/*! \file PA_BgLargeMap.h
    \brief Everything concerning the LargeBg System

    This file contains all the old macros and variables regarding LargeMaps (> 512x512),
	which have been obsoleted by PA_LoadBackground().
*/

/** @defgroup BgLargeMap Old large background system
 *  Old LargeMap functions, obsoleted by PA_LoadBackground()
 *  @{
 */

/*!
    \def PA_LoadLargeBg(screen, bg_select, bg_tiles, bg_map, color_mode, lx, ly)
	\deprecated
    \brief
      \~english [DEPRECATED] Completely load and initialise a background with infinite scrolling (usefull if larger or wider than 512 pixels)
      \~french [DEPRECATED] Charger et initialiser un fond pour le scrolling infini (pour les fonds de plus de 512 pixels de haut ou de large)
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
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

#define PA_LoadLargeBg(screen, bg_select, bg_tiles, bg_map, color_mode, lx, ly) do{\
	PA_DEPRECATED_MACRO;\
	PA_BgInfo[screen][bg_select].NTiles = SIZEOF_16BIT(bg_tiles)>>5;\
	if (PA_BgInfo[screen][bg_select].NTiles < MAX_TILES) {PA_LoadSimpleBg(screen, bg_select, bg_tiles, NULL, BG_512X256, 0, color_mode);}\
	else{PA_LoadTileEngine(screen, bg_select, (void*)bg_tiles);}\
	PA_InitLargeBg(screen, bg_select, lx, ly, (void*)bg_map);}while(0)

/*!
    \def PA_LoadPAGfxLargeBg(screen, bg_number, bg_name)
	\deprecated
    \brief
      \~english [DEPRECATED] Completely load and initialise a background with infinite scrolling (usefull if larger or wider than 512 pixels), converted with PAGfx
      \~french [DEPRECATED] Charger et initialiser un fond pour le scrolling infini (pour les fonds de plus de 512 pixels de haut ou de large), converti avec PAGfx
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param bg_number
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (0-3)
   \~\param  bg_name
       \~english Background name, in PAGfx
      \~french Nom du fond dans PAGfx 
*/
#define PA_LoadPAGfxLargeBg(screen, bg_number, bg_name) do{\
	PA_DEPRECATED_MACRO;\
	PA_LoadBgPal(screen, bg_number, (void*)bg_name##_Pal); \
	PA_LoadLargeBg(screen, bg_number, bg_name##_Tiles, bg_name##_Map, 1, (bg_name##_Info[1]) >> 3, (bg_name##_Info[2]) >> 3);}while(0)

/*!
    \def PA_LoadLargeBgEx(screen, bg_select, bg_tiles, tile_size, bg_map, color_mode, lx, ly)
	\deprecated
    \brief
      \~english [DEPRECATED] Completely load and initialise a background with infinite scrolling (usefull if larger or wider than 512 pixels), but here you can put yourself the tile size...
      \~french [DEPRECATED] Charger et initialiser un fond pour le scrolling infini (pour les fonds de plus de 512 pixels de haut ou de large), mais ici on met soi-meme la taille des tiles
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
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
#define PA_LoadLargeBgEx(screen, bg_select, bg_tiles, tile_size, bg_map, color_mode, lx, ly) do{\
	PA_DEPRECATED_MACRO;\
	PA_BgInfo[screen][bg_select].NTiles = SIZEOF_16BIT(bg_tiles)>>5;\
	if (PA_BgInfo[screen][bg_select].NTiles < MAX_TILES) {PA_LoadBg(screen, bg_select, bg_tiles, tile_size, NULL, BG_512X256, 0, color_mode);}\
	else{PA_LoadTileEngine(screen, bg_select, bg_tiles);}\
	PA_InitLargeBg(screen, bg_select, lx, ly, (void*)bg_map);}while(0)

/*!
    \fn void PA_InfLargeScrollX(u8 screen, u8 bg_select, s32 x)
    \brief
      \~english Scroll a large infinite scrolling background horizontaly. It must have been initialised with PA_LoadLargeBg.
      \~french Déplacer un fond à scrolling 'infini' horizontalement. Doit etre initialisé avec PA_LoadLargeBg.
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
    \~\param x
      \~english X value to scroll
      \~french Valeur X à déplacer
*/

static inline void PA_InfLargeScrollX(u8 screen, u8 bg_select, s32 x){ // Autoselect
	if (PA_BgInfo[screen][bg_select].NTiles < MAX_TILES) PA_InfLargeScrollXN(screen, bg_select, x);
	else PA_InfTilesScrollX(screen, bg_select, x);
	PA_BgInfo[screen][bg_select].ScrollX = x;
}

/*!
    \fn void PA_InfLargeScrollY(u8 screen, u8 bg_select, s32 y)
    \brief
      \~english Scroll a large infinite scrolling background vertically. It must have been initialised with PA_LoadLargeBg.
      \~french Déplacer un fond à scrolling 'infini' verticalement. Doit etre initialisé avec PA_LoadLargeBg.
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
    \~\param y
      \~english Y value to scroll
      \~french Valeur Y à déplacer
*/
static inline void PA_InfLargeScrollY(u8 screen, u8 bg_select, s32 y){ // Autoselect
	if (PA_BgInfo[screen][bg_select].NTiles < MAX_TILES) PA_InfLargeScrollYN(screen, bg_select, y);
	else PA_InfTilesScrollY(screen, bg_select, y);
	PA_BgInfo[screen][bg_select].ScrollY = y;
}


/*!
    \fn static inline void PA_InfLargeScrollXY(u8 screen, u8 bg_select, s32 x, s32 y)
    \brief
      \~english Scroll a large infinite scrolling background horizontaly and vertically. It must have been initialised with PA_LoadLargeBg.
      \~french Déplacer un fond à scrolling 'infini' horizontalement et verticalement. Doit etre initialisé avec PA_LoadLargeBg.
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
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
static inline void PA_InfLargeScrollXY(u8 screen, u8 bg_select, s32 x, s32 y){
	PA_InfLargeScrollX(screen, bg_select, x);
	PA_InfLargeScrollY(screen, bg_select, y);
}

/*!
    \fn void PA_LargeScrollX(u8 screen, u8 bg_select, s32 x)
    \brief
      \~english Scroll a large background horizontaly. It must have been initialised with PA_LoadLargeBg.  This function does not wrap around, but is faster than the InfLargeScroll...
      \~french Déplacer un grand fond à scrolling horizontalement.Doit etre initialisé avec PA_LoadLargeBg. Cette fonction ne permet pas au fond de 'boucler' sur lui-meme, mais est bien plus rapide que InfLargeScroll...
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
    \~\param x
      \~english X value to scroll
      \~french Valeur X à déplacer
*/
static inline void PA_LargeScrollX(u8 screen, u8 bg_select, s32 x){ // Autoselect
	if (PA_BgInfo[screen][bg_select].NTiles < MAX_TILES) {
		PA_LargeScrollXN(screen, bg_select, x);
		if (x < 0)x = 0;
		else if (x >= (s32)(PA_BgInfo[screen][bg_select].Infos.Width-256))
			x = PA_BgInfo[screen][bg_select].Infos.Width - 257;
	}
	else {
		PA_InfTilesScrollX(screen, bg_select, x);
	}	
	PA_BgInfo[screen][bg_select].ScrollX = x;	
}

/*!
    \fn void PA_LargeScrollY(u8 screen, u8 bg_select, s32 y)
    \brief
      \~english Scroll a large background vertically. It must have been initialised with PA_LoadLargeBg. This function does not wrap around, but is faster than the InfLargeScroll...
      \~french Déplacer un grand fond à scrolling verticalement. Doit etre initialisé avec PA_LoadLargeBg.  Cette fonction ne permet pas au fond de 'boucler' sur lui-meme, mais est bien plus rapide que InfLargeScroll...
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
    \~\param y
      \~english Y value to scroll
      \~french Valeur Y à déplacer
*/
static inline void PA_LargeScrollY(u8 screen, u8 bg_select, s32 y){ // Autoselect
	if (PA_BgInfo[screen][bg_select].NTiles < MAX_TILES) {
		PA_LargeScrollYN(screen, bg_select, y);
		if (y < 0) y = 0;
		else if (y >= (s32)(PA_BgInfo[screen][bg_select].Infos.Height-192)) y = PA_BgInfo[screen][bg_select].Infos.Height - 193;
	}
	else PA_InfTilesScrollY(screen, bg_select, y);
	PA_BgInfo[screen][bg_select].ScrollY = y;
}

/*!
    \fn static inline void PA_LargeScrollXY(u8 screen, u8 bg_select, s32 x, s32 y)
    \brief
      \~english Scroll a large background horizontaly and vertically. It must have been initialised with PA_LoadLargeBg. This function does not wrap around, but is faster than the InfLargeScroll...
      \~french Déplacer un grand fond à scrolling horizontalement et verticalement. Doit etre initialisé avec PA_LoadLargeBg. Cette fonction ne permet pas au fond de 'boucler' sur lui-meme, mais est bien plus rapide que InfLargeScroll...
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
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
static inline void PA_LargeScrollXY(u8 screen, u8 bg_select, s32 x, s32 y){
	PA_LargeScrollX(screen, bg_select, x);
	PA_LargeScrollY(screen, bg_select, y);
}

/** @} */ // end of BgLargeMap

// Pas utilisé en direct par le programmaeur
void PA_InitLargeBg(u8 screen, u8 bg_select, s32 lx, s32 ly, void* bg_map);

#endif
