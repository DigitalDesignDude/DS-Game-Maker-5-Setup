#ifndef _PA_SpriteDual
#define _PA_SpriteDual

extern s16 DualSpriteX[128];

/*! \file PA_SpriteDual.h
    \brief Everything concerning the sprite system, but for 2 screens !

    This file contains all macros, variables, and prototypes regarding the sprite system (OAM, Sprite mouvement, Gfx loading...) on 2 screens, easily, like if it was just one screen
*/

#include "PA_Sprite.h"

/** @defgroup SpriteDual Sprite system for Dual Screen
 *  Load Sprite, move them around, rotate them...
 *  @{
 */

/*! \fn static inline void PA_SetScreenSpace(s16 ScreenSpace)
    \brief
         \~english Set the space between the 2 screens for the Dual Fonctions. 48 pixels by default
         \~french Désigner l'espace entre les 2 écrans, 48 pixels par défaut
    \~\param ScreenSpace
         \~english Space in pixels
         \~french Espace en pixels
*/
static inline void PA_SetScreenSpace(s16 ScreenSpace){
	PA_ScreenSpace = 192 + ScreenSpace;
}

/*! \fn static inline void PA_DualSetSpriteX(u8 obj, s16 x)
    \brief
         \~english Set the X position of a sprite on screen
         \~french Position X du sprite à l'écran
    \~\param obj
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
    \~\param x
         \~english X position
         \~french Position X
*/
static inline void PA_DualSetSpriteX(u8 obj, s16 x){
	PA_obj[0][obj].atr1 = PA_obj[1][obj].atr1 = (PA_obj[0][obj].atr1 & ALL_BUT(PA_OBJ_X)) + ((x) & PA_OBJ_X);
	DualSpriteX[obj] = x;  // Memorize X, as moving out will be used to hide sprites
}

/*! \fn static inline void PA_DualSetSpriteY(u8 obj, s16 y)
    \brief
         \~english Set the Y position of a sprite on screen
         \~french Position Y du sprite à l'écran
    \~\param obj
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
    \~\param y
         \~english Y position
         \~french Position Y
*/
static inline void PA_DualSetSpriteY(u8 obj, s16 y){ 
	if (y < 192) {
		PA_SetSpriteY(1, obj, y);
		PA_SetSpriteX(1, obj, DualSpriteX[obj]);
	}
	else PA_SetSpriteX(1, obj, 256); // hide sprite if not on screen...
	if (y > PA_ScreenSpace-64) {
		PA_SetSpriteY(0, obj, y - PA_ScreenSpace);
		PA_SetSpriteX(0, obj, DualSpriteX[obj]);
	}
	else PA_SetSpriteX(0, obj, 256);
}

/*! \fn static inline void PA_DualSetSpriteXY(u8 sprite, s16 x, s16 y)
    \brief
         \~english Set the X and Y position of a sprite on screen
         \~french Position X et Y du sprite à l'écran
    \~\param sprite
         \~english sprite number in the sprite system
         \~french Numéro du sprite dans le systeme de sprite
    \~\param x
         \~english X position
         \~french Position Y
    \~\param y
         \~english X position
         \~french Position Y
*/
static inline void PA_DualSetSpriteXY(u8 sprite, s16 x, s16 y) {
	PA_DualSetSpriteX(sprite, x); 
	PA_DualSetSpriteY(sprite, y);
}

/*! \fn static inline void PA_DualCreateSprite(u8 obj_number, void* obj_data, u8 obj_shape, u8 obj_size, u8 color_mode, u8 palette, s16 x, s16 y)
    \brief
         \~english Create a sprite with it's gfx, on 2 screens
         \~french Creer un sprite avec ses gfxsur les 2 écrans
    \~\param obj_number
         \~english Object number you want to use (0-127 for each screen seperately). 
         \~french Numéro du sprite que vous voulez utiliser (de 0 à 127 pour chaque écran séparemment).
    \~\param obj_data
         \~english Gfx to load
         \~french Gfx à charger
    \~\param obj_shape
         \~english Object shape, from 0 to 2. Use the OBJ_SIZE_32X32 (...) macros for object shape and obj_size...
         \~french Forme du sprite à charger, de 0 à 2. Utiliser la macro OBJ_SIZE_32X32 (...) pour charger la forme et la taille...
    \~\param obj_size
         \~english Object size. Use the OBJ_SIZE_32X32 (...) macros for object shape and obj_size...
         \~french Taille du sprite. Utiliser la macro OBJ_SIZE_32X32 (...) pour charger la forme et la taille...
    \~\param color_mode
         \~english 256 or 16 color mode (1 or 0).
         \~french Mode 256 ou 16 couleurs (1 ou 0).
     \~\param palette
         \~english Palette to use (0-15).
         \~french Palette à utiliser (0-15). 
    \~\param x
         \~english X position of the sprite
         \~french Position X du sprite
    \~\param y
         \~english Y position of the sprite
         \~french Position Y du sprite
*/
static inline void PA_DualCreateSprite(u8 obj_number, void* obj_data, u8 obj_shape, u8 obj_size, u8 color_mode, u8 palette, s16 x, s16 y) {
	PA_CreateSprite(0, obj_number, obj_data, obj_shape, obj_size, color_mode, palette, x, 192);
	PA_CreateSprite(1, obj_number, obj_data, obj_shape, obj_size, color_mode, palette, x, 192);	
	PA_DualSetSpriteXY(obj_number, x, y);
};

/*! \fn static inline void PA_DualCreateSpriteEx(u8 obj_number, void* obj_data, u8 obj_shape, u8 obj_size, u8 color_mode, u8 palette, u8 obj_mode, u8 mosaic, u8 hflip, u8 vflip, u8 prio, u8 dblsize, s16 x, s16 y)
    \brief
         \~english Create a sprite with it's gfx. This is the complex version of the function
         \~french Creer un sprite avec ses gfx... Ceci est la version complexe de la fonction
    \~\param obj_number
         \~english Object number you want to use (0-127 for each screen seperately). 
         \~french Numéro du sprite que vous voulez utiliser (de 0 à 127 pour chaque écran séparemment).
    \~\param obj_data
         \~english Gfx to load
         \~french Gfx à charger
    \~\param obj_shape
         \~english Object shape, from 0 to 2. Use the OBJ_SIZE_32X32 (...) macros for object shape and obj_size...
         \~french Forme du sprite à charger, de 0 à 2. Utiliser la macro OBJ_SIZE_32X32 (...) pour charger la forme et la taille...
    \~\param obj_size
         \~english Object size. Use the OBJ_SIZE_32X32 (...) macros for object shape and obj_size...
         \~french Taille du sprite. Utiliser la macro OBJ_SIZE_32X32 (...) pour charger la forme et la taille...
    \~\param color_mode
         \~english 256 or 16 color mode (1 or 0).
         \~french Mode 256 ou 16 couleurs (1 ou 0).
     \~\param palette
         \~english Palette to use (0-15). 
         \~french Palette à utiliser (0-15). 
    \~\param obj_mode
         \~english Object mode (normal, transparent, window). Not functionnal yet, please leave to 0 for now
         \~french Mode du sprite (normal, transparent, fenetre). Pas encore opérationnel, laisser à 0...
     \~\param mosaic
         \~english Activate Mosaic for the sprite or not. Not yet functionnal either :p
         \~french Activer le mode mosaique pour ce sprite. Pas encore au point...
    \~\param hflip
         \~english Horizontal flip on or off...
         \~french Flip horizontal activé ou non.
     \~\param vflip
         \~english Vertical flip...
         \~french Flip vertical...
    \~\param prio
         \~english Sprite priority regarding backgrounds : in front of which background to show it (0-3)
         \~french Priorité du sprite vis-à-vis des fonds : devant quel fond l'afficher... (0-3)
     \~\param dblsize
         \~english Double the possible sprite size. Activate only if you are going to rotate and zoom in the sprite
         \~french Doubler la taille possible du sprite. A activer uniquement si on compte grossir et faire tourner le sprite
    \~\param x
         \~english X position of the sprite
         \~french Position X du sprite
    \~\param y
         \~english Y position of the sprite
         \~french Position Y du sprite
*/
static inline void PA_DualCreateSpriteEx(u8 obj_number, void* obj_data, u8 obj_shape, u8 obj_size, u8 color_mode, u8 palette, u8 obj_mode, u8 mosaic, u8 hflip, u8 vflip, u8 prio, u8 dblsize, s16 x, s16 y){
	PA_CreateSpriteEx(0, obj_number, obj_data, obj_shape, obj_size, color_mode, palette, obj_mode, mosaic, hflip, vflip, prio, dblsize, x, 192);
	PA_CreateSpriteEx(1, obj_number, obj_data, obj_shape, obj_size, color_mode, palette, obj_mode, mosaic, hflip, vflip, prio, dblsize, x, 192);
	PA_DualSetSpriteXY(obj_number, x, y);
}

/*! \fn static inline void PA_DualCreate16bitSpriteEx(u8 obj_number, void* obj_data, u8 obj_shape, u8 obj_size, u8 mosaic, u8 hflip, u8 vflip, u8 prio, u8 dblsize, s16 x, s16 y)
    \brief
         \~english Create a 16 bit sprite with it's gfx. This is the complex version of the function. Warning : a 16bit sprite MUST be 128 pixels large, even if you sprite only takes up a small part on the left...
         \~french Creer un sprite de 16 bits avec ses gfx... Ceci est la version complexe de la fonction. Attention : un sprite de 16 bits DOIT etre large de 128 pixels, meme si ce sprite ne prend qu'une petite partie sur la gauche
    \~\param obj_number
         \~english Object number you want to use (0-127 for each screen seperately). 
         \~french Numéro du sprite que vous voulez utiliser (de 0 à 127 pour chaque écran séparemment).
    \~\param obj_data
         \~english Gfx to load
         \~french Gfx à charger
    \~\param obj_shape
         \~english Object shape, from 0 to 2. Use the OBJ_SIZE_32X32 (...) macros for object shape and obj_size...
         \~french Forme du sprite à charger, de 0 à 2. Utiliser la macro OBJ_SIZE_32X32 (...) pour charger la forme et la taille...
    \~\param obj_size
         \~english Object size. Use the OBJ_SIZE_32X32 (...) macros for object shape and obj_size...
         \~french Taille du sprite. Utiliser la macro OBJ_SIZE_32X32 (...) pour charger la forme et la taille...
     \~\param mosaic
         \~english Activate Mosaic for the sprite or not. Not yet functionnal either :p
         \~french Activer le mode mosaique pour ce sprite. Pas encore au point...
    \~\param hflip
         \~english Horizontal flip on or off...
         \~french Flip horizontal activé ou non.
     \~\param vflip
         \~english Vertical flip...
         \~french Flip vertical...
    \~\param prio
         \~english Sprite priority regarding backgrounds : in front of which background to show it (0-3)
         \~french Priorité du sprite vis-à-vis des fonds : devant quel fond l'afficher... (0-3)
     \~\param dblsize
         \~english Double the possible sprite size. Activate only if you are going to rotate and zoom in the sprite
         \~french Doubler la taille possible du sprite. A activer uniquement si on compte grossir et faire tourner le sprite
    \~\param x
         \~english X position of the sprite
         \~french Position X du sprite
    \~\param y
         \~english Y position of the sprite
         \~french Position Y du sprite
*/

static inline void PA_DualCreate16bitSpriteEx(u8 obj_number, void* obj_data, u8 obj_shape, u8 obj_size, u8 mosaic, u8 hflip, u8 vflip, u8 prio, u8 dblsize, s16 x, s16 y){
	u16 mem_size = PA_obj_sizes[obj_size][obj_shape] << 1;
	u16 *gfx = (u16*)malloc(mem_size);
	mem_size = mem_size >> 1;
	s32 i;
	u16 *data = (u16*)obj_data;
	for (i = 0; i < mem_size; i++) gfx[i] = data[i] + (1 << 15);
	PA_obj[0][obj_number].atr2 = PA_CreateGfx(0, gfx, obj_shape, obj_size, 2) + (prio << 10) + (15 << 12);
	PA_obj[1][obj_number].atr2 = PA_CreateGfx(1, gfx, obj_shape, obj_size, 2) + (prio << 10) + (15 << 12);   
	PA_obj[1][obj_number].atr0 = PA_obj[0][obj_number].atr0 = (192 & PA_OBJ_Y) + (dblsize << 9) + (3 << 10) + (mosaic << 12) + (0 << 13) + (obj_shape << 14);
	PA_obj[1][obj_number].atr1 = PA_obj[0][obj_number].atr1 = (x & PA_OBJ_X) + (hflip << 12) + (vflip << 13) + (obj_size << 14);
	PA_DualSetSpriteXY(obj_number, x, y);

	free(gfx);
}

/*! \fn static inline void PA_DualCreate16bitSprite(u8 obj_number, void* obj_data, u8 obj_shape, u8 obj_size, s16 x, s16 y)
    \brief
         \~english Create a 16 bit sprite with it's gfx. This is the simple version of the function. Warning : a 16bit sprite MUST be 128 pixels large, even if you sprite only takes up a small part on the left...
         \~french Creer un sprite de 16 bits avec ses gfx... Ceci est la version simple de la fonction. Attention : un sprite de 16 bits DOIT etre large de 128 pixels, meme si ce sprite ne prend qu'une petite partie sur la gauche
    \~\param obj_number
         \~english Object number you want to use (0-127 for each screen seperately). 
         \~french Numéro du sprite que vous voulez utiliser (de 0 à 127 pour chaque écran séparemment).
    \~\param obj_data
         \~english Gfx to load
         \~french Gfx à charger
    \~\param obj_shape
         \~english Object shape, from 0 to 2. Use the OBJ_SIZE_32X32 (...) macros for object shape and obj_size...
         \~french Forme du sprite à charger, de 0 à 2. Utiliser la macro OBJ_SIZE_32X32 (...) pour charger la forme et la taille...
    \~\param obj_size
         \~english Object size. Use the OBJ_SIZE_32X32 (...) macros for object shape and obj_size...
         \~french Taille du sprite. Utiliser la macro OBJ_SIZE_32X32 (...) pour charger la forme et la taille...
    \~\param x
         \~english X position of the sprite
         \~french Position X du sprite
    \~\param y
         \~english Y position of the sprite
         \~french Position Y du sprite
*/
static inline void PA_DualCreate16bitSprite(u8 obj_number, void* obj_data, u8 obj_shape, u8 obj_size, s16 x, s16 y){
	PA_DualCreate16bitSpriteEx(obj_number, obj_data, obj_shape, obj_size, 0, 0, 0, 0, 0, x, y);
}

/*! \fn static inline void PA_DualCreateSpriteFromGfx(u8 obj_number, u16 *obj_gfx, u8 obj_shape, u8 obj_size, u8 color_mode, u8 palette, s16 x, s16 y)
    \brief
         \~english Create a sprite with it's gfx. This is the simple version of the function
         \~french Creer un sprite avec ses gfx... Ceci est la version simple de la fonction
    \~\param obj_number
         \~english Object number you want to use (0-127 for each screen seperately). 
         \~french Numéro du sprite que vous voulez utiliser (de 0 à 127 pour chaque écran séparemment).
    \~\param obj_gfx
         \~english Memory gfx to use. Get it by using PA_GetSpriteGfx or PA_CreateGfx
         \~french Gfx en mémoire à utiliser. On peut en avoir avec PA_GetSpriteGfx ou PA_CreateGfx
    \~\param obj_shape
         \~english Object shape, from 0 to 2. Use the OBJ_SIZE_32X32 (...) macros for object shape and obj_size...
         \~french Forme du sprite à charger, de 0 à 2. Utiliser la macro OBJ_SIZE_32X32 (...) pour charger la forme et la taille...
    \~\param obj_size
         \~english Object size. Use the OBJ_SIZE_32X32 (...) macros for object shape and obj_size...
         \~french Taille du sprite. Utiliser la macro OBJ_SIZE_32X32 (...) pour charger la forme et la taille...
    \~\param color_mode
         \~english 256 or 16 color mode (1 or 0).
         \~french Mode 256 ou 16 couleurs (1 ou 0).
     \~\param palette
         \~english Palette to use (0-15). 
         \~french Palette à utiliser (0-15). 
    \~\param x
         \~english X position of the sprite
         \~french Position X du sprite
    \~\param y
         \~english Y position of the sprite
         \~french Position Y du sprite
*/
static inline void PA_DualCreateSpriteFromGfx(u8 obj_number, u16 *obj_gfx, u8 obj_shape, u8 obj_size, u8 color_mode, u8 palette, s16 x, s16 y) {
	PA_CreateSpriteFromGfx(0, obj_number, obj_gfx[0], obj_shape, obj_size, color_mode, palette, x, 192);
	PA_CreateSpriteFromGfx(1, obj_number, obj_gfx[1], obj_shape, obj_size, color_mode, palette, x, 192);  
	PA_DualSetSpriteXY(obj_number, x, y);
}

/*! \fn static inline void PA_DualCreateSpriteExFromGfx(u8 obj_number, u16 *obj_gfx, u8 obj_shape, u8 obj_size, u8 color_mode, u8 palette, u8 obj_mode, u8 mosaic, u8 hflip, u8 vflip, u8 prio, u8 dblsize, s16 x, s16 y)
    \brief
         \~english Create a sprite with it's gfx. This is the complex version of the function
         \~french Creer un sprite avec ses gfx... Ceci est la version complexe de la fonction
    \~\param obj_number
         \~english Object number you want to use (0-127 for each screen seperately). 
         \~french Numéro du sprite que vous voulez utiliser (de 0 à 127 pour chaque écran séparemment).
    \~\param obj_gfx
         \~english Memory gfx to use. Get it by using PA_GetSpriteGfx or PA_CreateGfx
         \~french Gfx en mémoire à utiliser. On peut en avoir avec PA_GetSpriteGfx ou PA_CreateGfx
    \~\param obj_shape
         \~english Object shape, from 0 to 2. Use the OBJ_SIZE_32X32 (...) macros for object shape and obj_size...
         \~french Forme du sprite à charger, de 0 à 2. Utiliser la macro OBJ_SIZE_32X32 (...) pour charger la forme et la taille...
    \~\param obj_size
         \~english Object size. Use the OBJ_SIZE_32X32 (...) macros for object shape and obj_size...
         \~french Taille du sprite. Utiliser la macro OBJ_SIZE_32X32 (...) pour charger la forme et la taille...
    \~\param color_mode
         \~english 256 or 16 color mode (1 or 0).
         \~french Mode 256 ou 16 couleurs (1 ou 0).
     \~\param palette
         \~english Palette to use (0-15). 
         \~french Palette à utiliser (0-15). 
    \~\param obj_mode
         \~english Object mode (normal, transparent, window). Not functionnal yet, please leave to 0 for now
         \~french Mode du sprite (normal, transparent, fenetre). Pas encore opérationnel, laisser à 0...
     \~\param mosaic
         \~english Activate Mosaic for the sprite or not. Not yet functionnal either :p
         \~french Activer le mode mosaique pour ce sprite. Pas encore au point...
    \~\param hflip
         \~english Horizontal flip on or off...
         \~french Flip horizontal activé ou non.
     \~\param vflip
         \~english Vertical flip...
         \~french Flip vertical...
    \~\param prio
         \~english Sprite priority regarding backgrounds : in front of which background to show it (0-3)
         \~french Priorité du sprite vis-à-vis des fonds : devant quel fond l'afficher... (0-3)
     \~\param dblsize
         \~english Double the possible sprite size. Activate only if you are going to rotate and zoom in the sprite
         \~french Doubler la taille possible du sprite. A activer uniquement si on compte grossir et faire tourner le sprite
    \~\param x
         \~english X position of the sprite
         \~french Position X du sprite
    \~\param y
         \~english Y position of the sprite
         \~french Position Y du sprite
*/
static inline void PA_DualCreateSpriteExFromGfx(u8 obj_number, u16 *obj_gfx, u8 obj_shape, u8 obj_size, u8 color_mode, u8 palette, u8 obj_mode, u8 mosaic, u8 hflip, u8 vflip, u8 prio, u8 dblsize, s16 x, s16 y) {
	PA_CreateSpriteExFromGfx(0, obj_number, obj_gfx[0], obj_shape, obj_size, color_mode, palette, obj_mode, mosaic, hflip, vflip, prio, dblsize, x, 192);
	PA_CreateSpriteExFromGfx(1, obj_number, obj_gfx[1], obj_shape, obj_size, color_mode, palette, obj_mode, mosaic, hflip, vflip, prio, dblsize, x, 192);
	PA_DualSetSpriteXY(obj_number, x, y);
}


/*! \fn static inline void PA_DualUpdateSpriteGfx(u8 obj_number, void *obj_data)
    \brief
         \~english Update the Gfx of a given sprite
         \~french Mettre à jour les Gfx d'un sprite donné
    \~\param obj_number
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
    \~\param obj_data
         \~english Gfx to load
         \~french Graphisme à charger
*/
static inline void PA_DualUpdateSpriteGfx(u8 obj_number, void *obj_data){
	PA_UpdateSpriteGfx(0, obj_number, obj_data);
	PA_UpdateSpriteGfx(1, obj_number, obj_data);
}


/*! \fn static inline void PA_DualUpdateGfx(u16 gfx_number, void *obj_data)
    \brief
         \~english Update the Gfx of a given sprite
         \~french Mettre à jour les Gfx d'un sprite donné
    \~\param gfx_number
         \~english Gfx number in memory
         \~french Numéro du Gfx en mémoire
    \~\param obj_data
         \~english Gfx to load
         \~french Graphisme à charger
*/
static inline void PA_DualUpdateGfx(u16 gfx_number, void *obj_data){
	PA_UpdateGfx(0, gfx_number, obj_data);
	PA_UpdateGfx(1, gfx_number, obj_data);	
}

/*! \fn static inline void PA_DualDeleteSprite(u8 obj_number)
    \brief
         \~english Delete a given sprite. If it is the only one to use it's gfx, it'll be deleted too.
         \~french Effacer un sprite. S'il était le seul à utiliser un gfx, il sera effacé lui aussi
    \~\param obj_number
         \~english Sprite number
         \~french Numéro du sprite
*/
static inline void PA_DualDeleteSprite(u8 obj_number){
	PA_DeleteSprite(0, obj_number);
	PA_DeleteSprite(1, obj_number);	
}

/*! \fn static inline void PA_DualSetSpriteRotEnable(u8 sprite, u8 rotset)
    \brief
         \~english Rotate and zoom a sprite
         \~french Faire tourner et zoomer un sprite
    \~\param sprite
         \~english Sprite you want to rotate
         \~french Sprite que l'on veut faire tourner
    \~\param rotset
         \~english Rotset you want to give to that sprite (0-31). You can apparently use a rotset for multiple sprites if zoomed/rotated identically...
         \~french Rotset que l'on veut pour un sprite donné (0-31). On peut a priori utiliser un rotset pour plusieurs sprites, s'ils sont zoomés/tournés pareil...
*/
static inline void PA_DualSetSpriteRotEnable(u8 sprite, u8 rotset){
	PA_SetSpriteRotEnable(0, sprite, rotset);
	PA_SetSpriteRotEnable(1, sprite, rotset);	
}

/*! \fn static inline void PA_DualSetSpriteRotDisable(u8 sprite)
    \brief
         \~english Stop rotating and zooming a sprite
         \~french Arreter de faire tourner et zoomer un sprite
    \~\param sprite
         \~english Sprite you want to rotate
         \~french Sprite que l'on veut faire tourner
*/
static inline void PA_DualSetSpriteRotDisable(u8 sprite){
	PA_SetSpriteRotDisable(0, sprite);
	PA_SetSpriteRotDisable(1, sprite);	
}

/*! \fn static inline void PA_DualSetRotset(u8 rotset, s16 angle, u16 zoomx, u16 zoomy)
    \brief
         \~english Rotate and zoom a sprite
         \~french Faire tourner et zoomer un sprite
    \~\param rotset
         \~english Rotset you want to change. To give a sprite a rotset, use PA_SetSpriteRotEnable...
         \~french Rotset que l'on veut changer. Pour attribuer un rotset à un sprite, utiliser PA_SetSpriteRotEnable
    \~\param angle
         \~english Angle, between 0 and 512 (not 360, be carefull)
         \~french Angle, entre 0 et 512 (et non 360, attention !)
    \~\param zoomx
         \~english Horizontal zoom. 256 is unzoomed, 512 is 2 times smaller, and 128 twice as big... So adjust at will ! :p
         \~french Zoom horizontal. 256 est pas de zoom, 512 2 fois plus petit, et 128 2 fois plus grand... Ajuster au mieux ! :p
    \~\param zoomy
         \~english Vertical zoom. 256 is unzoomed, 512 is 2 times smaller, and 128 twice as big... So adjust at will ! :p
         \~french Zoom vertical. 256 est pas de zoom, 512 2 fois plus petit, et 128 2 fois plus grand... Ajuster au mieux ! :p
*/

static inline void PA_DualSetRotset(u8 rotset, s16 angle, u16 zoomx, u16 zoomy) {
	u8 obj_num = (rotset << 2);
	PA_obj[0][obj_num].atr3 = PA_obj[1][obj_num].atr3 = (PA_Cos(angle) * zoomx) >> 8;
	PA_obj[0][obj_num + 1].atr3 = PA_obj[1][obj_num + 1].atr3 = (-PA_Sin(angle) * zoomy) >> 8;
	PA_obj[0][obj_num + 2].atr3 = PA_obj[1][obj_num + 2].atr3 = (PA_Sin(angle) * zoomx) >> 8;
	PA_obj[0][obj_num + 2].atr3 = PA_obj[1][obj_num + 2].atr3 = (PA_Sin(angle) * zoomx) >> 8;
	PA_obj[0][obj_num + 3].atr3 = PA_obj[1][obj_num + 3].atr3 = (PA_Cos(angle) * zoomy) >> 8;
}

/*! \fn static inline void PA_DualSetRotsetNoZoom(u8 rotset, s16 angle)
    \brief
         \~english Rotate a sprite without zooming. It's a bit faster than the normal PA_SetRotset function
         \~french Faire tourner un sprite sans zoomer. C'est un peu plus rapide que la fonction PA_SetRotset
    \~\param rotset
         \~english Rotset you want to change. To give a sprite a rotset, use PA_SetSpriteRotEnable...
         \~french Rotset que l'on veut changer. Pour attribuer un rotset à un sprite, utiliser PA_SetSpriteRotEnable
    \~\param angle
         \~english Angle, between 0 and 512 (not 360, be carefull)
         \~french Angle, entre 0 et 512 (et non 360, attention !)
*/
static inline void PA_DualSetRotsetNoZoom(u8 rotset, s16 angle)   {
	u8 obj_num = (rotset << 2);
	PA_obj[0][obj_num].atr3 = PA_obj[1][obj_num].atr3 = PA_Cos(angle);
	PA_obj[0][obj_num + 1].atr3 = PA_obj[1][obj_num + 1].atr3 = -PA_Sin(angle);
	PA_obj[0][obj_num + 2].atr3 = PA_obj[1][obj_num + 2].atr3 = PA_Sin(angle);
	PA_obj[0][obj_num + 3].atr3 = PA_obj[1][obj_num + 3].atr3 = PA_Cos(angle);
}

/*! \fn static inline void PA_DualSetRotsetNoAngle(u8 rotset, u16 zoomx, u16 zoomy)
    \brief
         \~english Zoom a sprite without rotating. It's a bit faster than the normal PA_SetRotset function
         \~french Zoomer un sprite sans le faire tourner. C'est un peu plus rapide que la fonction PA_SetRotset
    \~\param rotset
         \~english Rotset you want to change. To give a sprite a rotset, use PA_SetSpriteRotEnable...
         \~french Rotset que l'on veut changer. Pour attribuer un rotset à un sprite, utiliser PA_SetSpriteRotEnable
    \~\param zoomx
         \~english Horizontal zoom. 256 is unzoomed, 512 is 2 times smaller, and 128 twice as big... So adjust at will ! :p
         \~french Zoom horizontal. 256 est pas de zoom, 512 2 fois plus petit, et 128 2 fois plus grand... Ajuster au mieux ! :p
    \~\param zoomy
         \~english Vertical zoom. 256 is unzoomed, 512 is 2 times smaller, and 128 twice as big... So adjust at will ! :p
         \~french Zoom vertical. 256 est pas de zoom, 512 2 fois plus petit, et 128 2 fois plus grand... Ajuster au mieux ! :p
*/
static inline void PA_DualSetRotsetNoAngle(u8 rotset, u16 zoomx, u16 zoomy) {
	u8 obj_num = (rotset << 2);
	PA_obj[0][obj_num].atr3 = PA_obj[1][obj_num].atr3 = zoomx;
	PA_obj[0][obj_num + 1].atr3 = PA_obj[1][obj_num + 1].atr3 = 0;
	PA_obj[0][obj_num + 2].atr3 = PA_obj[1][obj_num + 2].atr3 = 0;
	PA_obj[0][obj_num + 3].atr3 = PA_obj[1][obj_num + 3].atr3 = zoomy;
}

/*! \fn static inline void PA_DualSetSpritePal(u8 obj, u8 pal)
    \brief
         \~english Set the color palette used by a sprite
         \~french Changer la palette d'un sprite
    \~\param obj
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
    \~\param pal
         \~english Palette number (0 - 15)
         \~french Numéro de la palette (de 0 à 15)
*/
static inline void PA_DualSetSpritePal(u8 obj, u8 pal){
	PA_SetSpritePal(0, obj, pal);
	PA_SetSpritePal(1, obj, pal);	
}

/*! \fn static inline void PA_DualSetSpriteDblsize(u8 obj, u8 dblsize)
    \brief
         \~english Enable or disable double size for a given sprite
         \~french Activer ou désactiver le mode Doublesize pour un sprite
    \~\param obj
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
    \~\param dblsize
         \~english 1 to enable doublesize, 0 to disable it...
         \~french 1 pour l'activer, 0 pour l'inactiver
*/
static inline void PA_DualSetSpriteDblsize(u8 obj, u8 dblsize){
	PA_SetSpriteDblsize(0, obj, dblsize);
	PA_SetSpriteDblsize(1, obj, dblsize);
}

/*! \fn static inline void PA_DualSetSpriteColors(u8 sprite, u8 n_colors)
    \brief
         \~english Change the sprite's color mode
         \~french Changer le mode de couleur du sprite
    \~\param sprite
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
    \~\param n_colors
         \~english 0 for 16 colors, 1 for 256
         \~french 0 pour 16 couleurs, 1 pour 256
*/
static inline void PA_DualSetSpriteColors(u8 sprite, u8 n_colors){
	PA_SetSpriteColors(0, sprite, n_colors);
	PA_SetSpriteColors(1, sprite, n_colors);

}

/*! \fn static inline void PA_DualSetSpriteMode(u8 sprite, u8 obj_mode)
    \brief
         \~english Set the sprite's mode : 0 for normal, 1 for alpha blending, 2 for window
         \~french Régler le mode d'un sprite : 0 pour normal, 1 pour transparent, 2 pour fenetre
    \~\param sprite
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
    \~\param obj_mode
         \~english Object mode : 0 for normal, 1 for alpha blending, 2 for window ; not working yet
         \~french Mode : 0 pour normal, 1 pour transparent, 2 pour fenetre ; ne marche pas encore
*/
static inline void PA_DualSetSpriteMode(u8 sprite, u8 obj_mode){
	PA_SetSpriteMode(0, sprite, obj_mode);
	PA_SetSpriteMode(1, sprite, obj_mode);
}

/*! \fn static inline void PA_DualSetSpriteMosaic(u8 obj, u8 mosaic)
    \brief
         \~english Enable or disable mosaic mode for a given sprite
         \~french Mettre ou non un sprite en mode mosaic
    \~\param obj
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
    \~\param mosaic
         \~english Set mosaic on (1) or off (0)
         \~french Mode mosaic activé (1) ou désactivé (0)
*/
static inline void PA_DualSetSpriteMosaic(u8 obj, u8 mosaic){
	PA_SetSpriteMosaic(0, obj, mosaic);
	PA_SetSpriteMosaic(1, obj, mosaic);
}

/*! \fn static inline void PA_DualSetSpriteHflip(u8 obj, u8 hflip)
    \brief
         \~english Enable or disable horizontal flip for a given sprite
         \~french Utiliser ou non le flip horizontal pour un sprite
    \~\param obj
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
    \~\param hflip
         \~english Horizontal flip, 1 to enable, 0 to disable...
         \~french Flip horizontal, 1 pour oui, 0 pour non...
*/
static inline void PA_DualSetSpriteHflip(u8 obj, u8 hflip){
	PA_SetSpriteHflip(0, obj, hflip);
	PA_SetSpriteHflip(1, obj, hflip);
}	

/*! \fn static inline void PA_DualSetSpriteVflip(u8 obj, u8 vflip)
    \brief
         \~english Enable or disable vertical flip for a given sprite
         \~french Utiliser ou non le flip vertical pour un sprite
    \~\param obj
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
    \~\param vflip
         \~english Vertical flip, 1 to enable, 0 to disable...
         \~french Flip vertical, 1 pour oui, 0 pour non...
*/
static inline void PA_DualSetSpriteVflip(u8 obj, u8 vflip){
	PA_SetSpriteVflip(0, obj, vflip);
	PA_SetSpriteVflip(1, obj, vflip);
}	

/*! \fn static inline void PA_DualSetSpriteGfx(u8 obj, u16 *gfx)
    \brief
         \~english Change the gfx used by a sprite
         \~french Modifier les graphismes utilisés par un sprite
    \~\param obj
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
    \~\param gfx
         \~english Gfx number ; you can get one by using PA_CreateGfx or PA_GetSpriteGfx(obj_number);
         \~french Numéro du gfx en mémoire ; on peut obtenir un numéro avec PA_CreateGfx ou PA_GetSpriteGfx(obj_number);
*/
static inline void PA_DualSetSpriteGfx(u8 obj, u16 *gfx){
	PA_SetSpriteGfx(0, obj, gfx[0]);
	PA_SetSpriteGfx(1, obj, gfx[1]);
}

/*! \fn static inline void PA_DualSetSpritePrio(u8 obj, u8 prio)
    \brief
         \~english Set a sprite's Background priority
         \~french Régler la priorité d'un sprite par rapport au Bg.
    \~\param obj
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
    \~\param prio
         \~english Sprite priority : 0 is over background 0, 1 over Bg 1, etc... (0-3)
         \~french Priorité du sprite : 0 est au-dessus du fond 0, 1 au-dessus du 1, etc... (0-3)
*/
static inline void PA_DualSetSpritePrio(u8 obj, u8 prio){
	PA_SetSpritePrio(0, obj, prio);
	PA_SetSpritePrio(1, obj, prio);
}	

/*! \fn static inline void PA_DualCloneSprite(u8 obj, u8 target)
    \brief
         \~english Clone a sprite. Works only for sprites on the same screen
         \~french Cloner un sprite. Marche uniquement pour les sprites sur un meme écran
    \~\param obj
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
    \~\param target
         \~english Target sprite to clone
         \~french Numéro de la cible à cloner
*/
static inline void PA_DualCloneSprite(u8 obj, u8 target){
	PA_CloneSprite(0, obj, target);
	PA_CloneSprite(1, obj, target);
}

/*! \fn static inline void PA_DualSetSpriteAnimEx(u8 sprite, u8 lx, u8 ly, u8 ncolors, s16 animframe)
    \brief
         \~english Set the animation frame for a given sprite. This function is faster than the normal PA_SetSpriteAnim because it doesn't have to lookup the sprite dimensions...
         \~french Régler l'image du sprite dans l'animation. Cette fonction est plus rapide que PA_SetSpriteAnim parce qu'elle n'a pas à rechercher les dimensions du sprite
    \~\param sprite
         \~english sprite number in the sprite system
         \~french Numéro du sprite dans le systeme de sprite
    \~\param lx
         \~english Sprite width (8, 16, 32, 64)
         \~french Largeur du sprite (8, 16, 32, 64)
    \~\param ly
         \~english Sprite height (8, 16, 32, 64)
         \~french Hauteur du sprite (8, 16, 32, 64)		 
    \~\param ncolors
         \~english Sprite color mode (0 for 16 colors, 1 for 256)
         \~french Mode couleur du sprite (0 pour 16 couleurs, 1 pour 256)
    \~\param animframe
         \~english Sprite animation frame (0, 1, 2, etc...)
         \~french Frame de l'animation du sprite (0, 1, 2, etc...)
*/
static inline void PA_DualSetSpriteAnimEx(u8 sprite, u8 lx, u8 ly, u8 ncolors, s16 animframe){
	PA_SetSpriteAnimEx(0, sprite, lx, ly, ncolors, animframe);
	PA_SetSpriteAnimEx(1, sprite, lx, ly, ncolors, animframe);	
}

/*! \fn static inline void PA_DualSetSpriteAnim(u8 sprite, s16 animframe)
    \brief
         \~english Set the animation frame for a given sprite. Same as PA_SetSpriteAnimEx, but a bit slower and easier to use...
         \~french Régler l'image du sprite dans l'animation. Identique à PA_SetSpriteAnimEx, mais plus simple à utiliser, par contre plus lent
    \~\param sprite
         \~english sprite number in the sprite system
         \~french Numéro du sprite dans le systeme de sprite	 
    \~\param animframe
         \~english Sprite animation frame (0, 1, 2, etc...)
         \~french Frame de l'animation du sprite (0, 1, 2, etc...)
*/
static inline void PA_DualSetSpriteAnim(u8 sprite, s16 animframe){
	u8 lx = PA_GetSpriteLx(0, sprite); u8 ly = PA_GetSpriteLy(0, sprite);
	u8 ncolors = PA_GetSpriteColors(0, sprite);
	PA_SetSpriteAnimEx(0, sprite, lx, ly, ncolors, animframe);
	PA_SetSpriteAnimEx(1, sprite, lx, ly, ncolors, animframe);	
}

/*! \fn static inline void PA_DualStartSpriteAnimEx(u8 sprite, s16 firstframe, s16 lastframe, s16 speed, u8 type, s16 ncycles)
    \brief
         \~english Start a sprite animation for DualSprites. Once started, it continues on and on by itself until you stop it !
         \~french Démarre une animation de sprite pour DualSprites. Une fois démarrée, elle continue tant qu'on ne l'arrête pas !
    \~\param sprite
         \~english sprite number in the sprite system
         \~french Numéro du sprite dans le systeme de sprite	 
    \~\param firstframe
         \~english First frame of the animation sequence, most of the time 0...
         \~french Premières image de l'animation, généralement 0....
    \~\param lastframe
         \~english Last frame to be displayed. When it gets there, it loops back to the first frame
         \~french Dernière image à afficher. Une fois atteinte, ca retourne à la première
    \~\param speed
         \~english Speed, in frames per second. So speed 1 would mean 1 image per second, so 1 image every game frame
         \~french Vitesse, en frames par seconde (fps). 1 signifie donc 1 image par seconde... 
    \~\param type
         \~english Defines how you want it to loop. ANIM_LOOP (0) for a normal loop, ANIM_UPDOWN (1) for back and forth animation.
         \~french Défini de quelle manière on veut boucler. ANIM_LOOP (0) pour normal, et ANIM_UPDOWN (1) pour d'avant en arrière
    \~\param ncycles
         \~english Number of animation cycles before stopping. If using ANIM_UPDOWN, it takes 2 cycles to come back to the original image
         \~french Nombres de cycles d'animations avant l'arrêt. Si on utilise ANIM_UPDOWN, il faut 2 cycles pour que l'animation revienne à l'image de base
*/
static inline void PA_DualStartSpriteAnimEx(u8 sprite, s16 firstframe, s16 lastframe, s16 speed, u8 type, s16 ncycles){
	PA_StartSpriteAnimEx(0, sprite, firstframe, lastframe, speed, type, ncycles);
	PA_StartSpriteAnimEx(1, sprite, firstframe, lastframe, speed, type, ncycles);
}

/*! \fn static inline void PA_DualStartSpriteAnim(u8 sprite, s16 firstframe, s16 lastframe, s16 speed)
    \brief
         \~english Start a sprite animation for DualSprite. Once started, it continues on and on by itself until you stop it !
         \~french Démarre une animation de sprite pour DualSprite. Une fois démarrée, elle continue tant qu'on ne l'arrête pas !
    \~\param sprite
         \~english sprite number in the sprite system
         \~french Numéro du sprite dans le systeme de sprite	 
    \~\param firstframe
         \~english First frame of the animation sequence, most of the time 0...
         \~french Premières image de l'animation, généralement 0....
    \~\param lastframe
         \~english Last frame to be displayed. When it gets there, it loops back to the first frame
         \~french Dernière image à afficher. Une fois atteinte, ca retourne à la première
    \~\param speed
         \~english Speed, in frames per second. So speed 1 would mean 1 image per second, so 1 image every game frame
         \~french Vitesse, en frames par seconde (fps). 1 signifie donc 1 image par seconde... 
*/
static inline void PA_DualStartSpriteAnim(u8 sprite, s16 firstframe, s16 lastframe, s16 speed){
	PA_StartSpriteAnim(0, sprite, firstframe, lastframe, speed);
	PA_StartSpriteAnim(1, sprite, firstframe, lastframe, speed);	
}

/*! \fn static inline void PA_DualStopSpriteAnim(u8 sprite)
    \brief
         \~english Stop a sprite animation for DualSprites
         \~french Arrêter une animation de sprite pour les DualSprites
    \~\param sprite
         \~english sprite number in the sprite system
         \~french Numéro du sprite dans le systeme de sprite	
*/
static inline void PA_DualStopSpriteAnim(u8 sprite){
	PA_StopSpriteAnim(0, sprite);
	PA_StopSpriteAnim(1, sprite);
}

/*! \fn static inline void PA_DualSetSpriteAnimFrame(u8 sprite, u16 frame)
    \brief
         \~english Set the current animation frame number for DualSprites
         \~french Changer le numéro actuel de la frame d'animation pour les DualSprites
    \~\param sprite
         \~english sprite number in the sprite system
         \~french Numéro du sprite dans le systeme de sprite	
    \~\param frame
         \~english Frame number to use...
         \~french Numéro de frame...	 
*/
static inline void PA_DualSetSpriteAnimFrame(u8 sprite, u16 frame){
	PA_SetSpriteAnimFrame(0, sprite, frame);
	PA_SetSpriteAnimFrame(1, sprite, frame);
}

/*! \fn static inline u16 PA_DualGetSpriteAnimFrame(u8 sprite)
    \brief
         \~english Returns the current animation frame number for DualSprites
         \~french Renvoie le numéro actuel de la frame d'animation pour les DualSprites
    \~\param sprite
         \~english sprite number in the sprite system
         \~french Numéro du sprite dans le systeme de sprite	 
*/
static inline u16 PA_DualGetSpriteAnimFrame(u8 sprite){
	return spriteanims[0][sprite].currentframe;
}

/*! \fn static inline void PA_DualSetSpriteAnimSpeed(u8 sprite, s16 speed)
    \brief
         \~english Set the current animation speed for DualSprites
         \~french Changer la vitesse de l'animation pour les DualSprites
    \~\param sprite
         \~english sprite number in the sprite system
         \~french Numéro du sprite dans le systeme de sprite	
    \~\param speed
         \~english Speed, in fps...
         \~french Vitesse, en fps...		 
*/
static inline void PA_DualSetSpriteAnimSpeed(u8 sprite, s16 speed){
	PA_SetSpriteAnimSpeed(0, sprite, speed);
	PA_SetSpriteAnimSpeed(1, sprite, speed);	
}

/*! \fn static inline u16 PA_DualGetSpriteAnimSpeed(u8 sprite)
    \brief
         \~english Returns the current animation speed for DualSprites
         \~french Renvoie la vitesse de l'animation pour les DualSprites
    \~\param sprite
         \~english sprite number in the sprite system
         \~french Numéro du sprite dans le systeme de sprite	
*/
static inline u16 PA_DualGetSpriteAnimSpeed(u8 sprite){
	return spriteanims[0][sprite].speed;
}

/*! \fn static inline void PA_DualSpriteAnimPause(u8 sprite, u8 pause)
    \brief
         \~english Pause or UnPause a sprite animation for DualSprites
         \~french Mettre en Pause en remettre en lecture une animation de sprite pour les DualSprites
    \~\param sprite
         \~english sprite number in the sprite system
         \~french Numéro du sprite dans le systeme de sprite	
     \~\param pause
         \~english 1 for pause, 0 for unpause
         \~french 1 pour pause, 0 pour reprendre la lecture...
*/
static inline void PA_DualSpriteAnimPause(u8 sprite, u8 pause){
	PA_SpriteAnimPause(0, sprite, pause);
	PA_SpriteAnimPause(1, sprite, pause);	
}

/** @} */ // end of Sprite

#endif
