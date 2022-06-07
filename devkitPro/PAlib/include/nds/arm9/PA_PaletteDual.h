#ifndef _PA_PaletteDual
#define _PA_PaletteDual


// Left for backwards compatibility
#define PA_DualLoadSpriteExtPal PA_DualLoadSpritePal


/*! \file PA_PaletteDual.h
    \brief Everything concerning the palette system, for both screens at once

    Not much for now, only a function to load a complete palette to a given palette
*/

#include "PA_Palette.h"

#define PAL_BG PAL_BG0
#define PAL_SPRITE PAL_SPRITE0


/** @defgroup PaletteDual Palette system for Dual Screen
 *  Load palettes, change palette colors, set the gamma, etc... on both screens !
 *  @{
 */


/*! \def PA_DualLoadPal(palette, source)
    \brief
         \~english Load a 256 color palette in the Bg or Sprite palette of both screens
         \~french Charger une palette de 256 couleurs pour les fonds ou les sprites pour les 2 écrans
    \~\param palette
         \~english Set the Bg palette or Sprite palette : PAL_BG or PAL_SPRITE
         \~french Charger pour les Bg ou les Sprites : PAL_BG ou PAL_SPRITE
    \~\param source
         \~english Palette name (ex : master_Palette)
         \~french Nom de la palette (ex : master_Palette)
*/
#define PA_DualLoadPal(palette, source) do{\
	DMA_Copy((void*)source, (void*)palette, 256, DMA_16NOW);\
	DMA_Copy((void*)(source+1024), (void*)palette, 256, DMA_16NOW);\
	if(palette == PAL_SPRITE)\
		PA_DualLoadSpriteExtPal(0, (void*)palette);\
}while(0)




/*! \def PA_DualLoadPal16(palette, n_palette, source)
    \brief
         \~english Load a 16 color palette in the Bg or Sprite palette of both screens
         \~french Charger une palette de 16 couleurs pour les fonds ou les sprites pour les deux écrans
    \~\param palette
         \~english Set the Bg palette or Obj palette : PAL_BG or PAL_SPRITE
         \~french Charger pour les Bg ou les Sprites : PAL_BG ou PAL_SPRITE
    \~\param n_palette
         \~english Number of the 16 color palette to load (0-15)
         \~french Numéro de la palette de 16 couleurs que l'on veut charger (0-15)

    \~\param source
         \~english Palette name (ex : master_Palette)
         \~french Nom de la palette (ex : master_Palette)
*/
#define PA_DualLoadPal16(palette, n_palette, source) do{\
	DMA_Copy((void*)source, (void*)(palette + (n_palette << 5)), 16, DMA_16NOW);\
	DMA_Copy((void*)source, (void*)(palette + 1024 + (n_palette << 5)), 16, DMA_16NOW);}while(0)




/*! \fn static inline void PA_DualSetPalNeg(u32 palette)
    \brief
         \~english Set all the palette's color to negative. To undo this, simply negative again...
         \~french Négativer une palette donnée. Pour annuler, il suffit de négativer à nouveau. 
    \~\param palette
         \~english Set the Bg palette or Obj palette : PAL_BG, PAL_SPRITE
         \~french Charger pour les Bg ou les Sprites : PAL_BG, PAL_SPRITE
*/
static inline void PA_DualSetPalNeg(u32 palette) {
	u16* pal = (u16*)palette;
	int i;
	for (i = 0; i < 256; i++) {
		pal[i+512] = pal[i] = ~pal[i]; // On fout le négatif...
	}
}




/*! \fn static inline void PA_DualSetPal16Neg(u32 palette, u8 n_palette)
    \brief
         \~english Set 16 color palette to negative. To undo this, simply negative again...
         \~french Négativer une palette de 16 couleurs donnée. Pour annuler, il suffit de négativer à nouveau. 
    \~\param palette
         \~english Set the Bg palette or Obj palette : PAL_BG, PAL_SPRITE
         \~french Charger pour les Bg ou les Sprites : PAL_BG, PAL_SPRITE
    \~\param n_palette
         \~english Number of the 16 color palette (0-15)
         \~french Numéro de la palette de 16 couleurs (0-15)
*/
static inline void PA_DualSetPal16Neg(u32 palette, u8 n_palette) {
	u16* pal = (u16*)palette;
	int i;
	for (i = (n_palette << 4); i < ((n_palette + 1) << 4); i++) {
		pal[i+512] = pal[i] = ~pal[i]; // On fout le négatif...
	}
}

/*! \fn static inline void PA_DualLoadSpritePal(u8 palette_number, void* palette)
    \brief
         \~english Load a 256 color palette in the Sprite palettes
         \~french Charger une palette de 256 couleurs dans les palettes des sprites
    \~\param palette_number
         \~english Palette number (0-15)
         \~french Numéro de la palette (0-15)
    \~\param palette
         \~english Palette to load ((void*)palette_name)
         \~french Nom de la palette à charger ((void*)nom_palette)
*/
static inline void PA_DualLoadSpritePal(u8 palette_number, void* palette){
	PA_LoadSpriteExtPal(0, palette_number, palette);
	PA_LoadSpriteExtPal(1, palette_number, palette);
}

/*! \fn static inline void PA_DualLoadBgPal(u8 bg_number, void* palette)
    \brief
         \~english Load a 256 color palette for a given background
         \~french Charger une palette de 256 couleurs pour un fond
    \~\param bg_number
         \~english Background number (0-3)
         \~french Numéro du fond (0-3)
    \~\param palette
         \~english Palette to load ((void*)palette_name)
         \~french Nom de la palette à charger ((void*)nom_palette)
*/
static inline void PA_DualLoadBgPal(u8 bg_number, void* palette){
	PA_LoadBgPal(0, bg_number, palette);
	PA_LoadBgPal(1, bg_number, palette);
}

/*! \fn static inline void PA_DualSetBgColor(u16 color)
    \brief
         \~english Change the background color of both screens
         \~french Changer la couleur de fond des 2 écrans
    \~\param color
         \~english RGB value, like PA_RGB(31, 31, 31) for white
         \~french Valeur RGB, comme PA_RGB(31, 31, 31) pour blanc
*/
static inline void PA_DualSetBgColor(u16 color){
	BG_PALETTE[512] = BG_PALETTE[0] = color;
} 

/** @} */ // end of Palette
//////////////////////////////////////////////////////////////////////

#endif
