#ifndef _PA_BGROT
#define _PA_BGROT

#ifdef __cplusplus
extern "C"{
#endif

#include "PA_BgTiles.h"

/*! \file PA_BgRot.h
    \brief Everything concerning rotscale backgrounds

    This file contains all the macros and variables regarding Rotation backgrounds
*/

#define BG_ROT_128X128 0
#define BG_ROT_256X256 1
#define BG_ROT_512X512 2
#define BG_ROT_1024X1024 3

void PA_LoadRotBgMap(u8 screen, u8 bg_select, void* bg_map, u8 bg_size);

#define PA_BGXX(screen, bg) _REG32(0x4000008 + (0x1000 * screen) + (bg << 4))
#define PA_BGXY(screen, bg) _REG32(0x400000C + (0x1000 * screen) + (bg << 4))
#define PA_BGXPA(screen, bg) _REG16(0x4000000 + (0x1000 * screen) + (bg << 4))
#define PA_BGXPB(screen, bg) _REG16(0x4000002 + (0x1000 * screen) + (bg << 4))
#define PA_BGXPC(screen, bg) _REG16(0x4000004 + (0x1000 * screen) + (bg << 4))
#define PA_BGXPD(screen, bg) _REG16(0x4000006 + (0x1000 * screen) + (bg << 4))

extern u8 rotbg_size[2][4]; // Background size of each possible rotbg

/** @defgroup BgRot Rotating Backgrounds
 *  Load rotating backgrounds, move, rotate, scale them
 *  @{
 */

/*!
    \def PA_LoadRotBg(screen, bg_select, bg_tiles, bg_map, bg_size, wraparound)
	\deprecated
    \brief
      \~english [DEPRECATED] Load a background fit for rotating/scaling! Warning, you must use PA_SetVideoMode to 1 if you want 1 rotating background (Bg3 only !), or 2 for 2 rotating backgrounds (Bg2 and 3). The background MUST be in 256 colors
      \~french [DEPRECATED] Charger un fond pour les rotations/zoom! Attention, il faut avant utiliser PA_SetVideoMode avec 1 pour utiliser un fond rotatif (le fond 3 uniquement !), ou 2 pour 2 fonds (2 et 3). Le fond DOIT etre de 256 couleurs
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
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
#define PA_LoadRotBg(screen, bg_select, bg_tiles, bg_map, bg_size, wraparound) do{\
	PA_DEPRECATED_MACRO;\
	PA_DeleteBg(screen, bg_select);\
	PA_LoadBgTiles(screen, bg_select, bg_tiles); \
	PA_LoadRotBgMap(screen, bg_select, (void*)bg_map, bg_size); \
	PA_InitBg(screen, bg_select, bg_size, wraparound, 1);\
	PA_SetBgRot(screen, bg_select, 0, 0, 0, 0, 0, 256);\
	}while(0)

/*!
    \def PA_LoadPAGfxRotBg(screen, bg_select, bg_name, wraparound)
	\deprecated
    \brief
      \~english [DEPRECATED] Load a background fit for rotating/scaling ! Warning, you must use PA_SetVideoMode to 1 if you want 1 rotating background (Bg3 only !), or 2 for 2 rotating backgrounds (Bg2 and 3). The background MUST be in 256 colors
      \~french [DEPRECATED] Charger un fond pour les rotations/zoom ! Attention, il faut avant utiliser PA_SetVideoMode avec 1 pour utiliser un fond rotatif (le fond 3 uniquement !), ou 2 pour 2 fonds (2 et 3). Le fond DOIT etre de 256 couleurs
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param bg_select
      \~english Background number to load
      \~french Numéro du fond que l'on veut charger
    \~\param bg_name
      \~english Background name, like bg0
      \~french Nom du fond, comme bg0
    \~\param wraparound
      \~english If the background wraps around or not. 
      \~french Si le fond boucle ou non. 
*/
#define PA_LoadPAGfxRotBg(screen, bg_select, bg_name, wraparound) do{\
	PA_DEPRECATED_MACRO;\
	PA_Load8bitBgPal(screen, (void*)bg_name##_Pal);\
	PA_LoadRotBg(screen, bg_select, bg_name##_Tiles, bg_name##_Map, PA_GetPAGfxRotBgSize(bg_name##_Info[1]), wraparound);\
	}while(0)

/*!
    \fn static inline void PA_SetBgRot(u8 screen, u8 bg_select, s32 x_scroll, s32 y_scroll, s32 x_rotcentre, s32 y_rotcentre, s16 bg_angle, s32 bg_zoom)
    \brief
      \~english Rotate/Scale a RotBg
      \~french Faire tourner/zoomer un fond rotatif
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param bg_select
      \~english Background number to load
      \~french Numéro du fond que l'on veut charger
    \~\param x_scroll
      \~english X Scroll...
      \~french Scroll x...
    \~\param y_scroll
      \~english Y Scroll...
      \~french Scroll y...
    \~\param x_rotcentre
      \~english X position of the rotation center
      \~french Position X du centre de rotation
	\~\param y_rotcentre
      \~english Y position of the rotation center
      \~french Position Y du centre de rotation
    \~\param bg_angle
      \~english Rotation Angle (0-511)
      \~french Angle de rotation (0-511)
    \~\param bg_zoom
      \~english Zoom (256 for no zoom)
      \~french Zoom (256 pour pas de zoom...)	    
*/
static inline void PA_SetBgRot(u8 screen, u8 bg_select, s32 x_scroll, s32 y_scroll, s32 x_rotcentre, s32 y_rotcentre, s16 bg_angle, s32 bg_zoom){
	s32 pa, pb, pc, pd;
	
	pa = (PA_Cos(bg_angle) * (bg_zoom)) >> 8;
	pb = (-PA_Sin(bg_angle) * (bg_zoom)) >> 8;
	pc = (PA_Sin(bg_angle) * (bg_zoom)) >> 8;
	pd = pa;
	
	PA_BGXX(screen, bg_select) = (x_scroll << 8) - (x_rotcentre * pa + y_rotcentre * pb);
	PA_BGXY(screen, bg_select) = (y_scroll << 8) - (x_rotcentre * pc + y_rotcentre * pd);
	
	PA_BGXPA(screen, bg_select) = pa;
	PA_BGXPB(screen, bg_select) = pb;
	PA_BGXPC(screen, bg_select) = pc;
	PA_BGXPD(screen, bg_select) = pd;
}


// SetRotMapTile by gmiller
static inline void PA_SetRotMapTile(u8 screen, u8 bg_select, s16 x, s16 y, u8 tile_number)
{
   u16 hold, *where;

   // Calculate offset into rotational background map x + (y*32) ... tile is 8x8 and each pixel is 8 bits to 32  bytes wide
   where = (u16*)(PA_BgInfo[screen][bg_select].Map + ((x&1022) + (y << (4+rotbg_size[screen][bg_select]))));

   // Get current value as 16 bit but we only have 8 bits per pixes (must be written as 16 bit)
   hold = *where;

   // Odd or even (high or low byte
   if ((x & 1) == 0)
     hold = (hold & 0xFF00) | tile_number; // change low order byte
   else
     hold = (hold & 0x00FF) | (tile_number << 8); // change high order byte
   *where = hold; // save as 16 bit, as required by hardware
} 

// SetRotMapTile by gmiller
static inline u8 PA_GetRotMapTile(u8 screen, u8 bg_select, s16 x, s16 y)
{
   u16 hold, *where;

   // Calculate offset into rotational background map x + (y*32) ... tile is 8x8 and each pixel is 8 bits to 32  bytes wide
   where = (u16*)(PA_BgInfo[screen][bg_select].Map + ((x&1022) + (y << (4+rotbg_size[screen][bg_select]))));

   // Get current value as 16 bit but we only have 8 bits per pixes (must be written as 16 bit)
   hold = *where;

   // Odd or even (high or low byte
   if ((x & 1) == 0) return (u8)(hold&255);
   else return (u8)(hold>>8);

} 

/** @} */ // end of BgRot

#ifdef __cplusplus
}
#endif

#endif
