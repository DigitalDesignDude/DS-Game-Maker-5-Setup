#ifndef _PA_Sprite
#define _PA_Sprite

#ifdef __cplusplus
extern "C" {
#endif

/*! \file PA_Sprite.h
    \brief Everything concerning the sprite system

    This file contains all macros, variables, and prototypes regarding the sprite system (OAM, Sprite mouvement, Gfx loading...)
*/

#include "PA_Math.h"

#define PA_Cos(angle) PA_SIN[((angle) + 128)&511]
#define PA_Sin(angle) PA_SIN[((angle))&511]

extern const s16 PA_SIN[512];

#define MAX_DRAW 16  // Maximum 16 sprites dessinables...

//////////////////////////////////////////////////////////////////////
// Système de sprites
//////////////////////////////////////////////////////////////////////

#define OBJ_SIZE_8X8   0,0
#define OBJ_SIZE_16X16 0,1
#define OBJ_SIZE_32X32 0,2
#define OBJ_SIZE_64X64 0,3
#define OBJ_SIZE_16X8  1,0
#define OBJ_SIZE_32X8  1,1
#define OBJ_SIZE_32X16 1,2
#define OBJ_SIZE_64X32 1,3
#define OBJ_SIZE_8X16  2,0
#define OBJ_SIZE_8X32  2,1
#define OBJ_SIZE_16X32 2,2
#define OBJ_SIZE_32X64 2,3

typedef struct {
	u8 lx, ly;
} PA_sizes;

extern const PA_sizes PA_size[3][4];

#define BITS_16 65535
#define ALL_BUT(ATR) (BITS_16 - ATR)

#define PA_OBJ_X 511 //511, debugging...
#define PA_OBJ_Y 255
#define OBJ_ROT 256
#define ALL_BUT_ROTSET 49663  // Numéro du rotset
#define ALL_BUT_PAL 4095

#define DBLSIZE 512
#define N_COLORS (1 << 13)
#define OBJ_MODE 3072
#define OBJ_MOSAIC 4096
#define OBJ_HFLIP 4096
#define OBJ_VFLIP 8192

#define OBJ_GFX 1023
#define OBJ_PRIO 3072

#define COLORS_256 1
#define COLORS_16 0

#define MEM_DECAL 5
#define NUMBER_DECAL 7

// Mémoire gfx
typedef struct {
   u16 mem_block;  // Emplacement en mémoire, de 0 à 1023
   u16 free; // Doonées libres en ce point
} mem_usage;

extern u16 n_free_mem[2]; // nombre d'emplacements libres
extern u8 used_mem[2][1024]; // Note la quantité de mémoire utilisée en chaque point de la mémoire pour pouvoir effacer les gfx...
extern u8 obj_per_gfx[2][1024]; // Nombre de sprites utilisant un gfx donné...
extern u8 pa_obj_created[2][128];
extern mem_usage free_mem[2][1024];

extern u16 FirstGfx[2];

extern const u16 PA_obj_sizes[4][3];

typedef struct {
	u16 atr0, atr1, atr2, atr3;
}obj_inf;
extern obj_inf PA_obj[2][128] __attribute__ ((aligned (4)));  // Les 128 premiers pour l'écran du haut, et encore 128 pour l'écran du bas...

extern u8 PA_SpritePrio[2][128]; // Set the sprite priorities...

// Pour le dessin sur sprite
extern unsigned char *PA_SpriteBuffer[MAX_DRAW]; // Pointeurs vers les buffers  que l'on créera à la volée

extern u16 *PA_SpriteAnimP[2][1024];

#define ANIM_LOOP 0 // loop around
#define ANIM_UPDOWN 1 // back and forth animation

#define ANIM_ONESHOT ANIM_LOOP, 1
#define ANIM_INFINITE ANIM_LOOP, -1

extern s16 nspriteanims; // Number of animated sprites currently...
typedef struct{
	s32 firstframe, lastframe, currentframe; // frames...
	s32 time; // Time...
	s8 framechange; // 1 by default, -1 to go back...
	u8 lx, ly; // Sprite sizes
	u8 colors; 
	s16 speed;
	u8 play;
	u8 type;
	u16 *gfx; // gfx pointer
	s32 ncycles;
	u32 HideX;
} spriteanim;
extern spriteanim spriteanims[2][128]; // Init the array on PAlib init...

extern u8 PA_SpriteExtPrio;

#define OAM0 0x07000000
#define OAM1 0x07000400

/** @defgroup Sprite Sprite system
 *  Load Sprite, move them around, rotate them...
 *  @{
 */

/*! \def PA_UpdateOAM0()
    \brief
         \~english Update the sprite infos for screen 0 only. Do this in the VBL
         \~french Mettre à jour les infos des sprites pour l'écran 0 uniquement. A faire dans le VBL
*/
#define PA_UpdateOAM0() DMA_Copy((void*)PA_obj, (void*)OAM0, 256, DMA_32NOW)

/*! \def PA_UpdateOAM1()
    \brief
         \~english Update the sprite infos for screen 1 only. Do this in the VBL
         \~french Mettre à jour les infos des sprites pour l'écran 1 uniquement. A faire dans le VBL
*/
#define PA_UpdateOAM1() DMA_Copy((void*)PA_obj + 256, (void*)OAM1, 256, DMA_32NOW)

/*! \fn void PA_UpdateOAM(void)
    \brief
         \~english Update the sprite infos for both screens. Do this in the VBL
         \~french Mettre à jour les infos des sprites pour les 2 écrans. A faire dans le VBL
*/

void PA_UpdateOAM(void);

/*! \fn u16 PA_CreateGfx(u8 screen, void* obj_data, u8 obj_shape, u8 obj_size, u8 color_mode)
    \brief
         \~english Load in mémory a gfx to use later on for a sprite. Returns the gfx's number in memory
         \~french Charger en mémoire un gfx à utiliser plus tard pour un sprite. Renvoie le numéro en mémoire.
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
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
         \~english 256 or 16 color mode (1 or 0), or 2 for 16bit
         \~french Mode 256 ou 16 couleurs (1 ou 0), ou 2 pour 16 bits
*/

u16 PA_CreateGfx(u8 screen, void* obj_data, u8 obj_shape, u8 obj_size, u8 color_mode);

/*! \fn void PA_ResetSpriteSys(void)
    \brief
         \~english Reset the sprite system, memory, etc...
         \~french Remise à 0 du système de sprite, de la mémoire...
*/
void PA_ResetSpriteSysScreen(u8 screen);


/*! \fn void PA_ResetSpriteSys(void)
    \brief
         \~english Reset the sprite system, memory, etc...
         \~french Remise à 0 du système de sprite, de la mémoire...
*/
void PA_ResetSpriteSys(void);

/*! \fn static inline void PA_CreateSprite(u8 screen, u8 obj_number, void* obj_data, u8 obj_shape, u8 obj_size, u8 color_mode, u8 palette, s16 x, s16 y)
    \brief
         \~english Create a sprite with it's gfx. This is the simple version of the function
         \~french Creer un sprite avec ses gfx... Ceci est la version simple de la fonction
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
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

static inline void PA_CreateSprite(u8 screen, u8 obj_number, void* obj_data, u8 obj_shape, u8 obj_size, u8 color_mode, u8 palette, s16 x, s16 y) {
	PA_obj[screen][obj_number].atr2 = PA_CreateGfx(screen, obj_data, obj_shape, obj_size, color_mode) + (palette << 12);
	PA_obj[screen][obj_number].atr0 = (y&PA_OBJ_Y) + (color_mode << 13) + (obj_shape << 14);
	PA_obj[screen][obj_number].atr1 = (x & PA_OBJ_X) + (obj_size << 14);
	obj_per_gfx[screen][PA_obj[screen][obj_number].atr2]++;
	pa_obj_created[screen][obj_number]=1;
};

/*! \fn static inline void PA_CreateSpriteEx(u8 screen, u8 obj_number, void* obj_data, u8 obj_shape, u8 obj_size, u8 color_mode, u8 palette, u8 obj_mode, u8 mosaic, u8 hflip, u8 vflip, u8 prio, u8 dblsize, s16 x, s16 y)
    \brief
         \~english Create a sprite with it's gfx. This is the complex version of the function
         \~french Creer un sprite avec ses gfx... Ceci est la version complexe de la fonction
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
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
static inline void PA_CreateSpriteEx(u8 screen, u8 obj_number, void* obj_data, u8 obj_shape, u8 obj_size, u8 color_mode, u8 palette, u8 obj_mode, u8 mosaic, u8 hflip, u8 vflip, u8 prio, u8 dblsize, s16 x, s16 y) {
	PA_obj[screen][obj_number].atr2 = PA_CreateGfx(screen, obj_data, obj_shape, obj_size, color_mode) + (prio << 10) + (palette << 12);
	PA_obj[screen][obj_number].atr0 = (y&PA_OBJ_Y) + (dblsize << 9) + (obj_mode << 10) + (mosaic << 12) + ((color_mode) << 13) + (obj_shape << 14);
	PA_obj[screen][obj_number].atr1 = (x & PA_OBJ_X) + (hflip << 12) + (vflip << 13) + (obj_size << 14);
	obj_per_gfx[screen][PA_obj[screen][obj_number].atr2]++;
	pa_obj_created[screen][obj_number]=1;
};

/*! \fn static inline void PA_Create16bitSpriteEx(u8 screen, u8 obj_number, void* obj_data, u8 obj_shape, u8 obj_size, u8 mosaic, u8 hflip, u8 vflip, u8 prio, u8 dblsize, s16 x, s16 y)
    \brief
         \~english Create a 16 bit sprite with it's gfx. This is the complex version of the function. Warning : a 16bit sprite MUST be 128 pixels large, even if you sprite only takes up a small part on the left...
         \~french Creer un sprite de 16 bits avec ses gfx... Ceci est la version complexe de la fonction. Attention : un sprite de 16 bits DOIT etre large de 128 pixels, meme si ce sprite ne prend qu'une petite partie sur la gauche
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
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
static inline void PA_Create16bitSpriteEx(u8 screen, u8 obj_number, void* obj_data, u8 obj_shape, u8 obj_size, u8 mosaic, u8 hflip, u8 vflip, u8 prio, u8 dblsize, s16 x, s16 y){
	PA_obj[screen][obj_number].atr2 = PA_CreateGfx(screen, obj_data, obj_shape, obj_size, 2) + (prio << 10) + (15 << 12);
	PA_obj[screen][obj_number].atr0 = (y&PA_OBJ_Y) + (dblsize << 9) + (3 << 10) + (mosaic << 12) + (0 << 13) + (obj_shape << 14);
	PA_obj[screen][obj_number].atr1 = (x & PA_OBJ_X) + (hflip << 12) + (vflip << 13) + (obj_size << 14);
	obj_per_gfx[screen][PA_obj[screen][obj_number].atr2]++;
	pa_obj_created[screen][obj_number]=1;
}

/*! \fn static inline void PA_Create16bitSpriteFromGfx(u8 screen, u8 obj_number, u16 gfx, u8 obj_shape, u8 obj_size, s16 x, s16 y)
    \brief
         \~english Create a 16 bit sprite using a given gfx.
         \~french Creer un sprite de 16 bits à partir de gfx... 
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param obj_number
         \~english Object number you want to use (0-127 for each screen seperately). 
         \~french Numéro du sprite que vous voulez utiliser (de 0 à 127 pour chaque écran séparemment).
    \~\param gfx
         \~english Gfx to use
         \~french Gfx à utiliser
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
static inline void PA_Create16bitSpriteFromGfx(u8 screen, u8 obj_number, u16 gfx, u8 obj_shape, u8 obj_size, s16 x, s16 y){
	PA_obj[screen][obj_number].atr2 = gfx + (15 << 12);
	PA_obj[screen][obj_number].atr0 = (y&PA_OBJ_Y) + (3 << 10) + (obj_shape << 14);
	PA_obj[screen][obj_number].atr1 = (x & PA_OBJ_X) + (obj_size << 14);
	obj_per_gfx[screen][PA_obj[screen][obj_number].atr2]++;
	pa_obj_created[screen][obj_number]=1;
}

/*! \fn static inline void PA_Create16bitSprite(u8 screen, u8 obj_number, void* obj_data, u8 obj_shape, u8 obj_size, s16 x, s16 y)
    \brief
         \~english Create a 16 bit sprite with it's gfx. This is the simple version of the function. Warning : a 16bit sprite MUST be 128 pixels large, even if you sprite only takes up a small part on the left...
         \~french Creer un sprite de 16 bits avec ses gfx... Ceci est la version simple de la fonction. Attention : un sprite de 16 bits DOIT etre large de 128 pixels, meme si ce sprite ne prend qu'une petite partie sur la gauche
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
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
static inline void PA_Create16bitSprite(u8 screen, u8 obj_number, void* obj_data, u8 obj_shape, u8 obj_size, s16 x, s16 y){
	PA_Create16bitSpriteEx(screen, obj_number, obj_data, obj_shape, obj_size, 0, 0, 0, 0, 0, x, y);
}

/*! \fn static inline void PA_CreateSpriteFromGfx(u8 screen, u8 obj_number, u16 obj_gfx, u8 obj_shape, u8 obj_size, u8 color_mode, u8 palette, s16 x, s16 y)
    \brief
         \~english Create a sprite with it's gfx. This is the simple version of the function
         \~french Creer un sprite avec ses gfx... Ceci est la version simple de la fonction
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
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
static inline void PA_CreateSpriteFromGfx(u8 screen, u8 obj_number, u16 obj_gfx, u8 obj_shape, u8 obj_size, u8 color_mode, u8 palette, s16 x, s16 y) {
	PA_obj[screen][obj_number].atr2 = obj_gfx + (palette << 12);
	PA_obj[screen][obj_number].atr0 = (y&PA_OBJ_Y) + (color_mode << 13) + (obj_shape << 14);
	PA_obj[screen][obj_number].atr1 = (x & PA_OBJ_X) + (obj_size << 14);
	obj_per_gfx[screen][obj_gfx]++;
	pa_obj_created[screen][obj_number]=1;
};

/*! \fn static inline void PA_CreateSpriteExFromGfx(u8 screen, u8 obj_number, u16 obj_gfx, u8 obj_shape, u8 obj_size, u8 color_mode, u8 palette, u8 obj_mode, u8 mosaic, u8 hflip, u8 vflip, u8 prio, u8 dblsize, s16 x, s16 y)
    \brief
         \~english Create a sprite with it's gfx. This is the complex version of the function
         \~french Creer un sprite avec ses gfx... Ceci est la version complexe de la fonction
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
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
static inline void PA_CreateSpriteExFromGfx(u8 screen, u8 obj_number, u16 obj_gfx, u8 obj_shape, u8 obj_size, u8 color_mode, u8 palette, u8 obj_mode, u8 mosaic, u8 hflip, u8 vflip, u8 prio, u8 dblsize, s16 x, s16 y) {
	PA_obj[screen][obj_number].atr2 = obj_gfx + (prio << 10) + (palette << 12);
	PA_obj[screen][obj_number].atr0 = (y&PA_OBJ_Y) + (dblsize << 9) + (obj_mode << 10) + (mosaic << 12) + (color_mode << 13) + (obj_shape << 14);
	PA_obj[screen][obj_number].atr1 = (x & PA_OBJ_X) + (hflip << 12) + (vflip << 13) + (obj_size << 14);
	obj_per_gfx[screen][obj_gfx]++;
	pa_obj_created[screen][obj_number]=1;
};

/*! \def PA_UpdateSpriteGfx(screen, obj_number, obj_data)
    \brief
         \~english Update the Gfx of a given sprite
         \~french Mettre à jour les Gfx d'un sprite donné
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param obj_number
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
    \~\param obj_data
         \~english Gfx to load
         \~french Graphisme à charger
*/
#define PA_UpdateSpriteGfx(screen, obj_number, obj_data) PA_UpdateGfx(screen, PA_GetSpriteGfx(screen, obj_number), obj_data)

/*! \fn static inline void PA_UpdateGfx(u8 screen, u16 gfx_number, void *obj_data) 
    \brief
         \~english Update a given Gfx
         \~french Mettre à jour les Gfx donnés
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param gfx_number
         \~english Gfx number in memory
         \~french Numéro du Gfx en mémoire
    \~\param obj_data
         \~english Gfx to load
         \~french Graphisme à charger
*/

static inline void PA_UpdateGfx(u8 screen, u16 gfx_number, void *obj_data) {
	DMA_Copy((obj_data), (void*)(SPRITE_GFX1 + (0x200000 *  (screen)) + ((gfx_number) << NUMBER_DECAL)), (used_mem[screen][gfx_number] << (MEM_DECAL+1)), DMA_16NOW);
}

/*! \fn static inline void PA_UpdateGfxAndMem(u8 screen, u8 gfx_number, void *obj_data)
    \brief
         \~english Update the Gfx of a given sprite and updates the PAlib animation pointer... Only for advanced users
         \~french Mettre à jour les Gfx donnés et le pointer d'animation dans PAlib... Uniquement pour utilisateurs avertis
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param gfx_number
         \~english Gfx number in memory
         \~french Numéro du Gfx en mémoire
    \~\param obj_data
         \~english Gfx to load
         \~french Graphisme à charger
*/

static inline void PA_UpdateGfxAndMem(u8 screen, u8 gfx_number, void *obj_data){
	DMA_Copy((obj_data), (void*)(SPRITE_GFX1 + (0x200000 *  (screen)) + ((gfx_number) << NUMBER_DECAL)), (used_mem[screen][gfx_number] << MEM_DECAL), DMA_32NOW);
	PA_SpriteAnimP[screen][gfx_number] = (u16*)obj_data; // mémorise la source de l'image...
}

/*! \fn void PA_DeleteGfx(u8 screen, u16 obj_gfx)
    \brief
         \~english Delete a given Gfx. If a sprite uses this gfx, it'll become invisible
         \~french Effacer un Gfx. Si un sprite l'utilisait, il deviendra invisible...
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param obj_gfx
         \~english Gfx number in memory
         \~french Numéro du Gfx en mémoire
*/
void PA_DeleteGfx(u8 screen, u16 obj_gfx);

/*! \fn void PA_DeleteSprite(u8 screen, u8 obj_number)
    \brief
         \~english Delete a given sprite. If it is the only one to use it's gfx, it'll be deleted too.
         \~french Effacer un sprite. S'il était le seul à utiliser un gfx, il sera effacé lui aussi
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param obj_number
         \~english Sprite number
         \~french Numéro du sprite
*/
void PA_DeleteSprite(u8 screen, u8 obj_number);

/*! \def PA_SetSpriteRotEnable(screen, sprite, rotset)
    \brief
         \~english Rotate and zoom a sprite
         \~french Faire tourner et zoomer un sprite
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param sprite
         \~english Sprite you want to rotate
         \~french Sprite que l'on veut faire tourner
    \~\param rotset
         \~english Rotset you want to give to that sprite (0-31). You can apparently use a rotset for multiple sprites if zoomed/rotated identically...
         \~french Rotset que l'on veut pour un sprite donné (0-31). On peut a priori utiliser un rotset pour plusieurs sprites, s'ils sont zoomés/tournés pareil...
*/
#define PA_SetSpriteRotEnable(screen, sprite, rotset) do{PA_obj[screen][sprite].atr0 |= OBJ_ROT;  PA_obj[screen][sprite].atr1 = (PA_obj[screen][sprite].atr1 & ALL_BUT_ROTSET) + ((rotset) << 9);}while(0)

/*! \def PA_SetSpriteRotDisable(screen, sprite)
    \brief
         \~english Stop rotating and zooming a sprite
         \~french Arreter de faire tourner et zoomer un sprite
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param sprite
         \~english Sprite you want to rotate
         \~french Sprite que l'on veut faire tourner
*/
#define PA_SetSpriteRotDisable(screen, sprite) do{PA_obj[screen][sprite].atr0 &= ALL_BUT(OBJ_ROT);  PA_obj[screen][sprite].atr1 &= ALL_BUT_ROTSET;}while(0)

/*! \fn static inline void PA_SetRotset(u8 screen, u8 rotset, s16 angle, u16 zoomx, u16 zoomy)
    \brief
         \~english Rotate and zoom a sprite
         \~french Faire tourner et zoomer un sprite
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
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

static inline void PA_SetRotset(u8 screen, u8 rotset, s16 angle, u16 zoomx, u16 zoomy) {
	u8 obj_num = (rotset << 2);
	PA_obj[screen][obj_num].atr3 = (PA_Cos(angle) * zoomx) >> 8;
	PA_obj[screen][obj_num + 1].atr3 = (-PA_Sin(angle) * zoomy) >> 8;
	PA_obj[screen][obj_num + 2].atr3 = (PA_Sin(angle) * zoomx) >> 8;
	PA_obj[screen][obj_num + 3].atr3 = (PA_Cos(angle) * zoomy) >> 8;
}

/*! \fn static inline void PA_SetRotsetNoZoom(u8 screen, u8 rotset, s16 angle)
    \brief
         \~english Rotate a sprite without zooming. It's a bit faster than the normal PA_SetRotset function
         \~french Faire tourner un sprite sans zoomer. C'est un peu plus rapide que la fonction PA_SetRotset
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param rotset
         \~english Rotset you want to change. To give a sprite a rotset, use PA_SetSpriteRotEnable...
         \~french Rotset que l'on veut changer. Pour attribuer un rotset à un sprite, utiliser PA_SetSpriteRotEnable
    \~\param angle
         \~english Angle, between 0 and 512 (not 360, be carefull)
         \~french Angle, entre 0 et 512 (et non 360, attention !)
*/
static inline void PA_SetRotsetNoZoom(u8 screen, u8 rotset, s16 angle)   {
	u8 obj_num = (rotset << 2);
	PA_obj[screen][obj_num].atr3 = PA_Cos(angle);
	PA_obj[screen][obj_num + 1].atr3 = -PA_Sin(angle);
	PA_obj[screen][obj_num + 2].atr3 = PA_Sin(angle);
	PA_obj[screen][obj_num + 3].atr3 = PA_Cos(angle);
}

/*! \fn static inline void PA_SetRotsetNoAngle(u8 screen, u8 rotset, u16 zoomx, u16 zoomy)
    \brief
         \~english Zoom a sprite without rotating. It's a bit faster than the normal PA_SetRotset function
         \~french Zoomer un sprite sans le faire tourner. C'est un peu plus rapide que la fonction PA_SetRotset
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
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
static inline void PA_SetRotsetNoAngle(u8 screen, u8 rotset, u16 zoomx, u16 zoomy) {
	u8 obj_num = (rotset << 2);
	PA_obj[screen][obj_num].atr3 = zoomx;
	PA_obj[screen][obj_num + 1].atr3 = 0;
	PA_obj[screen][obj_num + 2].atr3 = 0;
	PA_obj[screen][obj_num + 3].atr3 = zoomy;
}

/*! \def PA_SetSpriteX(screen, obj, x)
    \brief
         \~english Set the X position of a sprite on screen
         \~french Position X du sprite à l'écran
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param obj
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
    \~\param x
         \~english X position
         \~french Position X
*/
#define PA_SetSpriteX(screen, obj, x) PA_obj[screen][obj].atr1 = (PA_obj[screen][obj].atr1 & ALL_BUT(PA_OBJ_X)) + ((x) & PA_OBJ_X)

/*! \def PA_GetSpriteX(screen, obj)
    \brief
         \~english Get the X position of a sprite on screen
         \~french Position X du sprite à l'écran
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param obj
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
*/

#define PA_GetSpriteX(screen, obj) (PA_obj[screen][obj].atr1 & (PA_OBJ_X))

/*! \def PA_SetSpriteY(screen, obj, y)
    \brief
         \~english Set the Y position of a sprite on screen
         \~french Position Y du sprite à l'écran
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param obj
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
    \~\param y
         \~english Y position
         \~french Position Y
*/
#define PA_SetSpriteY(screen, obj, y) PA_obj[screen][obj].atr0 = (PA_obj[screen][obj].atr0 & ALL_BUT(PA_OBJ_Y)) + ((y) & PA_OBJ_Y)

/*! \def PA_GetSpriteY(screen, obj)
    \brief
         \~english Get the Y position of a sprite on screen
         \~french Position Y du sprite à l'écran
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param obj
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite

*/
#define PA_GetSpriteY(screen, obj) (PA_obj[screen][obj].atr0 & PA_OBJ_Y)

/*! \fn static inline void PA_SetSpriteXY(u8 screen, u8 sprite, s16 x, s16 y)
    \brief
         \~english Set the X and Y position of a sprite on screen
         \~french Position X et Y du sprite à l'écran
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
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
static inline void PA_SetSpriteXY(u8 screen, u8 sprite, s16 x, s16 y) {
	PA_SetSpriteX(screen, sprite, x); 
	PA_SetSpriteY(screen, sprite, y);
}

/*! \def PA_SetSpritePal(screen, obj, pal)
    \brief
         \~english Set the sprite's palette number
         \~french Changer la palette d'un sprite
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param obj
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
    \~\param pal
         \~english Palette number (0 - 15)
         \~french Numéro de la palette (de 0 à 15)
*/
#define PA_SetSpritePal(screen, obj, pal) PA_obj[screen][obj].atr2 = (PA_obj[screen][obj].atr2 & ALL_BUT_PAL) + ((pal) << 12)

/*! \fn static inline void PA_Set16bitSpriteAlpha(u8 screen, u8 sprite, u8 alpha)
    \brief
         \~english Set the X position of a sprite on screen
         \~french Position X du sprite à l'écran
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param sprite
         \~english Object number in the sprite system, only for 16bit sprites
         \~french Numéro de l'objet dans le systeme de sprite, uniquement pour les sprites 16bit
    \~\param alpha
         \~english Alpha parameter, 0-15
         \~french Paramèter alpha, 0-15
*/

static inline void PA_Set16bitSpriteAlpha(u8 screen, u8 sprite, u8 alpha){
	PA_SetSpritePal(screen, sprite, alpha&15);
}

/*! \def PA_GetSpritePal(screen, obj)
    \brief
         \~english Get thepalette used by a sprite
         \~french Palette d'un sprite
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param obj
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
*/
#define PA_GetSpritePal(screen, obj) (PA_obj[screen][obj].atr2 >> 12)

/*! \def PA_SetSpriteDblsize(screen, obj, dblsize)
    \brief
         \~english Enable or disable double size for a given sprite
         \~french Activer ou désactiver le mode Doublesize pour un sprite
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param obj
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
    \~\param dblsize
         \~english 1 to enable doublesize, 0 to disable it...
         \~french 1 pour l'activer, 0 pour l'inactiver
*/
#define PA_SetSpriteDblsize(screen, obj, dblsize) PA_obj[screen][obj].atr0 = (PA_obj[screen][obj].atr0 & ALL_BUT(DBLSIZE)) + ((dblsize) << 9)

/*! \def PA_GetSpriteDblsize(screen, obj)
    \brief
         \~english Get the double size state for a given sprite
         \~french Etat du mode Doublesize pour un sprite
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param obj
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
*/
#define PA_GetSpriteDblsize(screen, obj) ((PA_obj[screen][obj].atr0 & DBLSIZE) >> 9)

/*! \def PA_SetSpriteColors(screen, sprite, n_colors)
    \brief
         \~english Change the sprite's color mode
         \~french Changer le mode de couleur du sprite
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param sprite
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
    \~\param n_colors
         \~english 0 for 16 colors, 1 for 256
         \~french 0 pour 16 couleurs, 1 pour 256
*/
#define PA_SetSpriteColors(screen, sprite, n_colors) PA_obj[screen][sprite].atr0 = (PA_obj[screen][sprite].atr0 & ALL_BUT(N_COLORS)) + ((n_colors) << 13)

/*! \def PA_GetSpriteColors(screen, sprite)
    \brief
         \~english Get a sprite's color mode
         \~french Mode de couleur d'un sprite
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param sprite
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
*/
#define PA_GetSpriteColors(screen, sprite) ((PA_obj[screen][sprite].atr0 & N_COLORS) >> 13)

/*! \def PA_SetSpriteMode(screen, sprite, obj_mode)
    \brief
         \~english Set the sprite's mode : 0 for normal, 1 for alpha blending, 2 for window
         \~french Régler le mode d'un sprite : 0 pour normal, 1 pour transparent, 2 pour fenetre
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param sprite
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
    \~\param obj_mode
         \~english Object mode : 0 for normal, 1 for alpha blending, 2 for window ; not working yet
         \~french Mode : 0 pour normal, 1 pour transparent, 2 pour fenetre ; ne marche pas encore
*/
#define PA_SetSpriteMode(screen, sprite, obj_mode) PA_obj[screen][sprite].atr0 = (PA_obj[screen][sprite].atr0 & ALL_BUT(OBJ_MODE)) + ((obj_mode) << 10)

/*! \def PA_GetSpriteMode(screen, obj)
    \brief
         \~english Get the sprite's mode : 0 for normal, 1 for alpha blending, 2 for window
         \~french Mode d'un sprite : 0 pour normal, 1 pour transparent, 2 pour fenetre
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param obj
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
*/
#define PA_GetSpriteMode(screen, obj) ((PA_obj[screen][obj].atr0 & OBJ_MODE) >> 10)

/*! \def PA_SetSpriteMosaic(screen, obj, mosaic)
    \brief
         \~english Enable or disable mosaic mode for a given sprite
         \~french Mettre ou non un sprite en mode mosaic
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param obj
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
    \~\param mosaic
         \~english Set mosaic on (1) or off (0)
         \~french Mode mosaic activé (1) ou désactivé (0)
*/
#define PA_SetSpriteMosaic(screen, obj, mosaic) PA_obj[screen][obj].atr0 = (PA_obj[screen][obj].atr0 & ALL_BUT(OBJ_MOSAIC)) + ((mosaic) << 12)

/*! \def PA_GetSpriteMosaic(screen, obj)
    \brief
         \~english Get the mosaic mode for a given sprite
         \~french Si un sprite est en mode mosaic
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param obj
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
*/
#define PA_GetSpriteMosaic(screen, obj) ((PA_obj[screen][obj].atr0 & OBJ_MOSAIC) >> 12)

/*! \def PA_SetSpriteHflip(screen, obj, hflip)
    \brief
         \~english Enable or disable horizontal flip for a given sprite
         \~french Utiliser ou non le flip horizontal pour un sprite
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param obj
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
    \~\param hflip
         \~english Horizontal flip, 1 to enable, 0 to disable...
         \~french Flip horizontal, 1 pour oui, 0 pour non...
*/
#define PA_SetSpriteHflip(screen, obj, hflip) PA_obj[screen][obj].atr1 = (PA_obj[screen][obj].atr1 & ALL_BUT(OBJ_HFLIP)) + ((hflip) << 12)

/*! \def PA_GetSpriteHflip(screen, obj)
    \brief
         \~english Get the horizontal flip state for a given sprite
         \~french S'il y a un flip horizontal pour un sprite
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param obj
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
*/
#define PA_GetSpriteHflip(screen, obj) ((PA_obj[screen][obj].atr1 & OBJ_HFLIP) >> 12)

/*! \def PA_SetSpriteVflip(screen, obj, vflip)
    \brief
         \~english Enable or disable vertical flip for a given sprite
         \~french Utiliser ou non le flip vertical pour un sprite
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param obj
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
    \~\param vflip
         \~english Vertical flip, 1 to enable, 0 to disable...
         \~french Flip vertical, 1 pour oui, 0 pour non...
*/
#define PA_SetSpriteVflip(screen, obj, vflip) PA_obj[screen][obj].atr1 = (PA_obj[screen][obj].atr1 & ALL_BUT(OBJ_VFLIP)) + ((vflip) << 13)

/*! \def PA_GetSpriteVflip(screen, obj)
    \brief
         \~english Get the  vertical flip state for a given sprite
         \~french Si le flip vertical est utilisé ou non pour un sprite
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param obj
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
*/
#define PA_GetSpriteVflip(screen, obj) ((PA_obj[screen][obj].atr1 & OBJ_VFLIP) >> 13)

/*! \def PA_SetSpriteGfx(screen, obj, gfx)
    \brief
         \~english Change the gfx used by a sprite
         \~french Modifier les graphismes utilisés par un sprite
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param obj
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
    \~\param gfx
         \~english Gfx number ; you can get one by using PA_CreateGfx or PA_GetSpriteGfx(obj_number);
         \~french Numéro du gfx en mémoire ; on peut obtenir un numéro avec PA_CreateGfx ou PA_GetSpriteGfx(obj_number);
*/
#define PA_SetSpriteGfx(screen, obj, gfx) PA_obj[screen][obj].atr2 = (PA_obj[screen][obj].atr2 & ALL_BUT(OBJ_GFX)) + ((gfx) & OBJ_GFX)

/*! \def PA_GetSpriteGfx(screen, obj)
    \brief
         \~english Get the gfx used by a sprite
         \~french Récupérer le gfx utilisés par un sprite
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param obj
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
*/
#define PA_GetSpriteGfx(screen, obj) (PA_obj[screen][obj].atr2 & OBJ_GFX)

/*! \def PA_SetSpritePrio(screen, obj, prio)
    \brief
         \~english Set a sprite's Background priority
         \~french Régler la priorité d'un sprite par rapport au Bg.
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param obj
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
    \~\param prio
         \~english Sprite priority : 0 is over background 0, 1 over Bg 1, etc... (0-3)
         \~french Priorité du sprite : 0 est au-dessus du fond 0, 1 au-dessus du 1, etc... (0-3)
*/
#define PA_SetSpritePrio(screen, obj, prio) PA_obj[screen][obj].atr2 = (PA_obj[screen][obj].atr2 & ALL_BUT(OBJ_PRIO)) + ((prio) << 10)

/*! \def PA_GetSpritePrio(screen, obj)
    \brief
         \~english Get a sprite's Background priority
         \~french Récupérer la priorité d'un sprite par rapport au Bg.
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param obj
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
*/
#define PA_GetSpritePrio(screen, obj) ((PA_obj[screen][obj].atr2 & OBJ_PRIO) >> 10)

/*! \def PA_GetSpriteLx(screen, sprite)
    \brief
         \~english Get a sprite's length
         \~french Récupérer la largeur d'un sprite
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param sprite
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
*/
#define PA_GetSpriteLx(screen, sprite) PA_size[PA_obj[screen][sprite].atr0 >> 14][PA_obj[screen][sprite].atr1 >> 14].lx

/*! \def PA_GetSpriteLy(screen, sprite)
    \brief
         \~english Get a sprite's height
         \~french Récupérer la hauteur d'un sprite
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param sprite
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
*/
#define PA_GetSpriteLy(screen, sprite)PA_size[PA_obj[screen][sprite].atr0 >> 14][PA_obj[screen][sprite].atr1 >> 14].ly

/*! \def PA_CloneSprite(screen, obj, target)
    \brief
         \~english Clone a sprite. Works only for sprites on the same screen
         \~french Cloner un sprite. Marche uniquement pour les sprites sur un meme écran
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param obj
         \~english Object number in the sprite system
         \~french Numéro de l'objet dans le systeme de sprite
    \~\param target
         \~english Target sprite to clone
         \~french Numéro de la cible à cloner
*/
#define PA_CloneSprite(screen, obj, target) do{PA_obj[screen][obj].atr0 = PA_obj[screen][target].atr0; PA_obj[screen][obj].atr1 = PA_obj[screen][target].atr1; PA_obj[screen][obj].atr2 = PA_obj[screen][target].atr2; ++obj_per_gfx[screen][PA_GetSpriteGfx(screen, target)];}while(0)

/*! \fn static inline void PA_SetSpriteAnimEx(u8 screen, u8 sprite, u8 lx, u8 ly, u8 ncolors, s16 animframe)
    \brief
         \~english Set the animation frame for a given sprite. This function is faster than the normal PA_SetSpriteAnim because it doesn't have to lookup the sprite dimensions...
         \~french Régler l'image du sprite dans l'animation. Cette fonction est plus rapide que PA_SetSpriteAnim parce qu'elle n'a pas à rechercher les dimensions du sprite
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
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
static inline void PA_SetSpriteAnimEx(u8 screen, u8 sprite, u8 lx, u8 ly, u8 ncolors, s16 animframe){
	u16 gfx = PA_GetSpriteGfx(screen, sprite);
	spriteanims[screen][sprite].currentframe = animframe;
	PA_UpdateGfx(screen, gfx, (void*)(PA_SpriteAnimP[screen][gfx] + (animframe * (lx * ly) >> (2 - ncolors))));
}

/*! \fn static inline void PA_SetSpriteAnim(u8 screen, u8 sprite, s16 animframe)
    \brief
         \~english Set the animation frame for a given sprite. Same as PA_SetSpriteAnimEx, but a bit slower and easier to use...
         \~french Régler l'image du sprite dans l'animation. Identique à PA_SetSpriteAnimEx, mais plus simple à utiliser, par contre plus lent
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param sprite
         \~english sprite number in the sprite system
         \~french Numéro du sprite dans le systeme de sprite	 
    \~\param animframe
         \~english Sprite animation frame (0, 1, 2, etc...)
         \~french Frame de l'animation du sprite (0, 1, 2, etc...)
*/
static inline void PA_SetSpriteAnim(u8 screen, u8 sprite, s16 animframe){
	PA_SetSpriteAnimEx(screen, sprite, PA_GetSpriteLx(screen, sprite), PA_GetSpriteLy(screen, sprite), PA_GetSpriteColors(screen, sprite), animframe);
}

/*! \fn void PA_StartSpriteAnimEx(u8 screen, u8 sprite, s16 firstframe, s16 lastframe, s16 speed, u8 type, s16 ncycles)
    \brief
         \~english Start a sprite animation. Once started, it continues on and on by itself until you stop it !
         \~french Démarre une animation de sprite. Une fois démarrée, elle continue tant qu'on ne l'arrête pas !
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
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
void PA_StartSpriteAnimEx(u8 screen, u8 sprite, s16 firstframe, s16 lastframe, s16 speed, u8 type, s16 ncycles);

/*! \fn static inline void PA_StartSpriteAnim(u8 screen, u8 sprite, s16 firstframe, s16 lastframe, s16 speed)
    \brief
         \~english Start a sprite animation. Once started, it continues on and on by itself until you stop it !
         \~french Démarre une animation de sprite. Une fois démarrée, elle continue tant qu'on ne l'arrête pas !
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
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
static inline void PA_StartSpriteAnim(u8 screen, u8 sprite, s16 firstframe, s16 lastframe, s16 speed){
	PA_StartSpriteAnimEx(screen, sprite, firstframe, lastframe, speed, ANIM_INFINITE);
}

/*! \fn static inline void PA_StopSpriteAnim(u8 screen, u8 sprite)
    \brief
         \~english Stop a sprite animation
         \~french Arrêter une animation de sprite
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param sprite
         \~english sprite number in the sprite system
         \~french Numéro du sprite dans le systeme de sprite	
*/
static inline void PA_StopSpriteAnim(u8 screen, u8 sprite){
	if (spriteanims[screen][sprite].play) nspriteanims--;
	spriteanims[screen][sprite].play = 0;
}

/*! \fn static inline void PA_SetSpriteAnimFrame(u8 screen, u8 sprite, u16 frame)
    \brief
         \~english Set the current animation frame number
         \~french Changer le numéro actuel de la frame d'animation
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param sprite
         \~english sprite number in the sprite system
         \~french Numéro du sprite dans le systeme de sprite	
    \~\param frame
         \~english Frame number to use...
         \~french Numéro de frame...	 
*/
static inline void PA_SetSpriteAnimFrame(u8 screen, u8 sprite, u16 frame){
	if(spriteanims[screen][sprite].currentframe != frame){
		spriteanims[screen][sprite].currentframe = frame;
		PA_SetSpriteAnimEx(screen, sprite, spriteanims[screen][sprite].lx, spriteanims[screen][sprite].ly, spriteanims[screen][sprite].colors, spriteanims[screen][sprite].currentframe);
	}
}

/*! \fn static inline u16 PA_GetSpriteAnimFrame(u8 screen, u8 sprite)
    \brief
         \~english Returns the current animation frame number
         \~french Renvoie le numéro actuel de la frame d'animation
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param sprite
         \~english sprite number in the sprite system
         \~french Numéro du sprite dans le systeme de sprite	 
*/
static inline u16 PA_GetSpriteAnimFrame(u8 screen, u8 sprite){
	return spriteanims[screen][sprite].currentframe;
}

/*! \fn static inline void PA_SetSpriteAnimSpeed(u8 screen, u8 sprite, s16 speed)
    \brief
         \~english Set the current animation speed
         \~french Changer la vitesse de l'animation
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param sprite
         \~english sprite number in the sprite system
         \~french Numéro du sprite dans le systeme de sprite	
    \~\param speed
         \~english Speed, in fps...
         \~french Vitesse, en fps...		 
*/
static inline void PA_SetSpriteAnimSpeed(u8 screen, u8 sprite, s16 speed){
	spriteanims[screen][sprite].speed = speed;
}

/*! \fn static inline u16 PA_GetSpriteAnimSpeed(u8 screen, u8 sprite)
    \brief
         \~english Returns the current animation speed
         \~french Renvoie la vitesse de l'animation
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param sprite
         \~english sprite number in the sprite system
         \~french Numéro du sprite dans le systeme de sprite	
*/
static inline u16 PA_GetSpriteAnimSpeed(u8 screen, u8 sprite){
	return spriteanims[screen][sprite].speed;
}

/*! \fn static inline void PA_SetSpriteNCycles(u8 screen, u8 sprite, s32 NCycles)
    \brief
         \~english Set the current animation cycles left (-1 for inifinite loop)
         \~french Changer le nombre de cycles d'animation restant (-1 pour inifini)
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param sprite
         \~english sprite number in the sprite system
         \~french Numéro du sprite dans le systeme de sprite	
    \~\param NCycles
         \~english Number of cycles
         \~french Nombre de cycles		 
*/
static inline void PA_SetSpriteNCycles(u8 screen, u8 sprite, s32 NCycles){
	spriteanims[screen][sprite].ncycles = NCycles;
}

/*! \fn static inline s32 PA_GetSpriteNCycles(u8 screen, u8 sprite)
    \brief
         \~english Returns the current number of animation cycles left
         \~french Renvoie le nombre de cycles d'animation restants
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param sprite
         \~english sprite number in the sprite system
         \~french Numéro du sprite dans le systeme de sprite	
*/
static inline s32 PA_GetSpriteNCycles(u8 screen, u8 sprite){
	return spriteanims[screen][sprite].ncycles;
}

/*! \fn static inline u16 PA_SpriteAnimPause(u8 screen, u8 sprite, u8 pause)
    \brief
         \~english Pause or UnPause a sprite animation
         \~french Mettre en Pause en remettre en lecture une animation de sprite
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param sprite
         \~english sprite number in the sprite system
         \~french Numéro du sprite dans le systeme de sprite	
     \~\param pause
         \~english 1 for pause, 0 for unpause
         \~french 1 pour pause, 0 pour reprendre la lecture...
*/
static inline void PA_SpriteAnimPause(u8 screen, u8 sprite, u8 pause){
	if (pause&&spriteanims[screen][sprite].play) nspriteanims--;
	else if ((!pause)&&(!spriteanims[screen][sprite].play)) nspriteanims++;
	spriteanims[screen][sprite].play = !pause;
}

/*! \fn static inline void PA_SetSpritePixel(u8 screen, u8 sprite, u8 x, u8 y, u8 color)
    \brief
         \~english Set a sprite's pixel to a given palette color. Like PA_SetSpritePixelEx, with less options, but a little slower
         \~french Mettre un pixel d'un sprite à une couleur donnée. Comme PA_SetSpritePixelEx, avec moins d'options, mais un peu plus lent
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param sprite
         \~english Sprite number in the sprite system
         \~french Numéro du sprite dans le systeme de sprite
    \~\param x
         \~english X coordinate of the pixel to change
         \~french Coordonnée X du pixel à changer
    \~\param y
         \~english Y coordinate of the pixel to change
         \~french Coordonnée Y du pixel à changer
    \~\param color
         \~english New palette color to put
         \~french Nouvelle couleur de la palette à metrre
*/
static inline void PA_SetSpritePixel(u8 screen, u8 sprite, u8 x, u8 y, u8 color){
	u8 hsize = spriteanims[screen][sprite].lx>>3;

	s32 pos = (x >> 3) + ((y >> 3) * hsize);
	x&=7; y&=7;
	
	pos = (pos << 5) + (x >> 1) + (y << 2);
	
	u16 pixel = spriteanims[screen][sprite].gfx[pos];
	
	if (x&1)
		spriteanims[screen][sprite].gfx[pos] = (color << 8) | (pixel&0x00FF);			
	else
		spriteanims[screen][sprite].gfx[pos] = color | (pixel&0xFF00);	
}

/*! \fn static inline u8 PA_GetSpritePixel(u8 screen, u8 sprite, u8 x, u8 y)
    \brief
         \~english Get a sprite's pixel color. Like PA_GetSpritePixelEx, with less options, but a little slower
         \~french Récupérer la couleur d'un pixel d'un sprite. Comme PA_GetSpritePixelEx, avec moins d'options, mais un peu plus lent
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param sprite
         \~english Sprite number in the sprite system
         \~french Numéro du sprite dans le systeme de sprite
    \~\param x
         \~english X coordinate of the pixel
         \~french Coordonnée X du pixel
    \~\param y
         \~english Y coordinate of the pixel
         \~french Coordonnée Y du pixel
*/
static inline u8 PA_GetSpritePixel(u8 screen, u8 sprite, u8 x, u8 y) {
	u8 hsize = spriteanims[screen][sprite].lx>>3;

	s32 pos = (x >> 3) + ((y >> 3) * hsize);
	x&=7; y&=7;
	
	pos = (pos << 5) + (x >> 1) + (y << 2);
	
	u16 pixel = spriteanims[screen][sprite].gfx[pos];
	
	if (x&1)
		return ((pixel>>8)&255);
	else
		return (pixel&255);
}

/*! \fn static inline u8 PA_GetSprite16cPixel(u8 screen, u8 sprite, u8 x, u8 y)
    \brief
         \~english Get a 16 color sprite's pixel color. 
         \~french Récupérer la couleur d'un pixel d'un sprite de 16 couleurs.
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param sprite
         \~english Sprite number in the sprite system
         \~french Numéro du sprite dans le systeme de sprite
    \~\param x
         \~english X coordinate of the pixel
         \~french Coordonnée X du pixel
    \~\param y
         \~english Y coordinate of the pixel
         \~french Coordonnée Y du pixel
*/
static inline u8 PA_GetSprite16cPixel(u8 screen, u8 sprite, u8 x, u8 y) {
	u8 hsize = spriteanims[screen][sprite].lx>>3;

	s32 pos = (x >> 3) + ((y >> 3) * hsize);
	x&=7; y&=7;
	
	pos = (pos << 4) + (x >> 2) + (y << 1);
	
	u16 pixel = spriteanims[screen][sprite].gfx[pos];
	return ((pixel>>(4*(x&3)))&15);	
}

/*! \fn void PA_InitSpriteDraw(u8 screen, u8 sprite)
    \brief
         \~english Initialise a sprite to be able to draw on it !
         \~french Initialise un sprite pour pouvoir dessiner dessus ! 
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param sprite
         \~english Sprite number in the sprite system
         \~french Numéro du sprite dans le systeme de sprite
*/
void PA_InitSpriteDraw(u8 screen, u8 sprite);

/*! \fn static inline void PA_InitAllSpriteDraw(void)
    \brief
         \~english Initialise all the onscreen sprites to draw on them
         \~french Initialise tous les sprites à l'écran pour dessiner dessus
*/
static inline void PA_InitAllSpriteDraw(void){
	int i, j;
	for (j = 0; j < 2; j++)
		for (i = 0; i < 128; i++)
			PA_InitSpriteDraw(j, i);
}

/*! \fn void PA_InitSpriteExtPrio(u8 SpritePrio)
    \brief
         \~english Enable the PAlib sprite priority system. Slower than the normal priority system, but offering 256 levels of priority for the sprites (overrides the sprite number's priority)
         \~french Activer le systeme de priorité de sprites PAlib. Plus lent que le systeme normal, il permet d'avoir 256 niveaux de priorité (supplante la priorité par numéro de sprites)
    \~\param SpritePrio
         \~english 1 for on, 0 for off...
         \~french 1 pour on, 0 pour off...
*/
void PA_InitSpriteExtPrio(u8 SpritePrio);

static inline void PA_SetSpriteExtPrio(u8 screen, u8 sprite, u8 prio){
	PA_SpritePrio[screen][sprite] = prio;
}

/** @} */ // end of Sprite

void PA_UpdateSpriteAnims();

#ifdef __cplusplus
}
#endif

#endif
