#ifndef _PA_3DSPRITES_
#define _PA_3DSPRITES_

static inline u16 PA_GetGifWidth(void* gif);
static inline u16 PA_GetGifHeight(void* gif);

#define PA_NMAXSPRITES 1024

#define TEX_A3I5   1
#define TEX_A5I3   6
#define TEX_4COL   2
#define TEX_16COL  3
#define TEX_256COL 4
#define TEX_16BITS 7

#define BLOCKSIZE_3D 16 // Minimal block size

#define N_TEXTURES 1024
extern s16 obj_per_gfx3D[N_TEXTURES];

static inline void PA_Set3D(bool texture_mapping, bool shader, bool alpha_test, bool alpha_blend,
             bool antialias, bool edgemark, bool fog_mode, bool fog, u8 fog_shift, bool rearplane){
	GFX_CONTROL = texture_mapping | (shader<<1) | (alpha_test<<2) | (alpha_blend<<3) | (antialias<<4)
	    | (edgemark<<5) | (fog_mode<<6) | (fog<<7) | (fog_shift<<8) | (rearplane<<14);
}

void PA_Load3DSpritePal(u16 palette_number, void* palette);

typedef struct{
	s16 x, y;
}pa3dcorners;

typedef struct{
	u8 Alive;
	s32 X, Y;
	s32 ImgWidth, ImgHeight;
	s32 Width, Height, Tex;
	s32 RotX, RotY, RotZ;
	u8 Hflip, Vflip;
	u16 textureID;
	u16 palette; // Palette...
	u16 Priority;
	pa3dcorners corner[4]; // Corners...
	u8 alpha, polyID;
}pa3dsprites;
extern pa3dsprites pa_3dsprites[PA_NMAXSPRITES];

extern uint32 textures[MAX_TEXTURES];
extern void *texturesptr[MAX_TEXTURES]; // for updates...
extern uint32 activeTexture;

typedef struct{
	s32 firstframe, lastframe, currentframe; // frames...
	s8 framechange; // 1 by default, -1 to go back...
	s32 time; // Time...
	u8 type;
	s16 speed;
	u8 play;
	s32 ncycles;
}type_3danims;
extern type_3danims sprite3danims[PA_NMAXSPRITES];
extern u16 n3Dspriteanims;

static inline void PA_glTexParameter(uint8 sizeX, uint8 sizeY, uint32* addr, uint8 mode, uint32 param){
	textures[activeTexture] = param | (sizeX << 20) | (sizeY << 23)
	    | (((uint32)addr>>3) & 0xFFFF) | (mode << 26) | (1<<29); // 1 << 29 to set background color to blank
}

/*! \file PA_3DSprites.h
    \brief Sprites on one screen using the DS's 3D GPU

	Allows to have up to 2048 sprites of pretty much any size!
*/

/** @defgroup f3DSprites 3D Sprite System
 *  Sprites on one screen using the DS's 3D GPU
 *  @{
 */

/// Initializes 3D.
void PA_Init3D();
/// Initializes 3D taking two banks of VRAM.
void PA_Init3D2Banks();
/// Renders the 3D sprites.
void PA_3DProcess();
/// Creates a 3D texture.
s16 PA_3DCreateTex(void* obj_data, u16 width, u16 height, u8 type);
/// Creates a 3D sprite from a texture.
void PA_3DCreateSpriteFromTex(u16 sprite, u16 texture, u16 width, u16 height, u8 palette, s16 x, s16 y);
/// Resets the 3D system.
void PA_Reset3DSprites();
/// Resets the dual bank 3D system.
void PA_Reset3DSprites2Banks();

/// Creates a 3D sprite
static inline u16 PA_3DCreateSprite(u16 sprite, void *image, u16 width, u16 height, u8 type, u8 palette, s16 x, s16 y){
	u16 tex_gfx = PA_3DCreateTex(image, width, height, type);
	PA_3DCreateSpriteFromTex(sprite, tex_gfx, width, height, palette, x, y);
	obj_per_gfx3D[tex_gfx] --;
	return tex_gfx;
}

/// Deletes a 3D texture.
void PA_3DDeleteTex(u32 tex_gfx); // Delete...

/// Deletes a 3D sprite.
static inline void PA_3DDeleteSprite(u16 sprite){
	pa_3dsprites[sprite].Alive = 0;	
	u16 tex_gfx = pa_3dsprites[sprite].textureID;
	obj_per_gfx3D[tex_gfx] --;
	if(obj_per_gfx3D[tex_gfx] == 0) PA_3DDeleteTex(tex_gfx); // Delete if only sprite to use the gfx...
}

/// Moves a 3D sprite in the X axis.
static inline void PA_3DSetSpriteX(u16 sprite, s16 x){
	pa_3dsprites[sprite].X = x;
}

/// Moves a 3D sprite in the Y axis.
static inline void PA_3DSetSpriteY(u16 sprite, s16 y){
	pa_3dsprites[sprite].Y = y;
}

/// Moves a 3D sprite.
static inline void PA_3DSetSpriteXY(u16 sprite, s16 x, s16 y){
	pa_3dsprites[sprite].X = x;
	pa_3dsprites[sprite].Y = y;
}

/// Rotates a 3D sprite in the X axis.
static inline void PA_3DSetSpriteRotateX(u16 sprite, s16 rotateX){
	pa_3dsprites[sprite].RotX = rotateX;
}

/// Rotates a 3D sprite in the Y axis.
static inline void PA_3DSetSpriteRotateY(u16 sprite, s16 rotateY){
	pa_3dsprites[sprite].RotY = rotateY;
}

/// Rotates a 3D sprite in the Z axis.
static inline void PA_3DSetSpriteRotateZ(u16 sprite, s16 rotate){
	pa_3dsprites[sprite].RotZ = rotate;
}

/// Rotates a 3D sprite.
static inline void PA_3DSetSpriteRotateXYZ(u16 sprite, s16 rotateX, s16 rotateY, s16 rotateZ){
	pa_3dsprites[sprite].RotX = rotateX;
	pa_3dsprites[sprite].RotY = rotateY;
	pa_3dsprites[sprite].RotZ = rotateZ;
}

/// Zooms a 3D sprite horizontally.
static inline void PA_3DSetSpriteZoomX(u16 sprite, float zoomx){
	pa_3dsprites[sprite].Width = (s32)(pa_3dsprites[sprite].ImgWidth * zoomx);
}

/// Zooms a 3D sprite vertically.
static inline void PA_3DSetSpriteZoomY(u16 sprite, float zoomy){
	pa_3dsprites[sprite].Height = (s32)(pa_3dsprites[sprite].ImgHeight * zoomy);
}

/// Zooms a 3D sprite.
static inline void PA_3DSetSpriteZoomXY(u16 sprite, float zoomx, float zoomy){
	pa_3dsprites[sprite].Width = (s32)(pa_3dsprites[sprite].ImgWidth * zoomx);
	pa_3dsprites[sprite].Height = (s32)(pa_3dsprites[sprite].ImgHeight * zoomy);
}

/// Changes the width of a 3D sprite.
static inline void PA_3DSetSpriteWidth(u16 sprite, u16 width){
	pa_3dsprites[sprite].Width = width;
}

/// Changes the height of a 3D sprite.
static inline void PA_3DSetSpriteHeight(u16 sprite, u16 height){
	pa_3dsprites[sprite].Height = height;
}

/// Changes the size of a 3D sprite.
static inline void PA_3DSetSpriteWidthHeight(u16 sprite, u16 width, u16 height){
	PA_3DSetSpriteWidth(sprite, width);
	PA_3DSetSpriteHeight(sprite, height);		   
}

/// Sets the HFlip of a 3D sprite.
static inline void PA_3DSetSpriteHflip(u16 sprite, u8 hflip){
	pa_3dsprites[sprite].Hflip = hflip;
}

/// Sets the VFlip of a 3D sprite.
static inline void PA_3DSetSpriteVflip(u16 sprite, u8 vflip){
	pa_3dsprites[sprite].Vflip = vflip;
}

/// Retrives if a 3D sprite is being touched by the stylus.
static inline u8 PA_3DSpriteTouched(u16 sprite){
	// No comment.
	return PA_StylusInZone((pa_3dsprites[sprite].X-(pa_3dsprites[sprite].Width>>1)), (pa_3dsprites[sprite].Y-(pa_3dsprites[sprite].Height>>1)),
	                      (pa_3dsprites[sprite].X+(pa_3dsprites[sprite].Width>>1)), (pa_3dsprites[sprite].Y+(pa_3dsprites[sprite].Height>>1)));
}   

/// Sets the texture of a 3D sprite.
static inline void PA_3DSetSpriteTex(u16 sprite, u16 texture){
	pa_3dsprites[sprite].textureID = texture;
}

/// Sets the palette of a 3D sprite.
static inline void PA_3DSetSpritePal(u16 sprite, u16 palette){
	pa_3dsprites[sprite].palette = palette;
}

void PA_3DUpdateGfx(u16 texture, void *image);

/// Sets the animation frame of a 3D sprite.
void PA_3DSetSpriteFrame(u16 sprite, u16 frame);

/// Sets the top left corner of a 3D sprite.
static inline void PA_3DSetSpriteTopLeft(u16 sprite, s16 x, s16 y){
	pa_3dsprites[sprite].corner[0].x = x;
	pa_3dsprites[sprite].corner[0].y = y;
}

/// Sets the top right corner of a 3D sprite.
static inline void PA_3DSetSpriteTopRight(u16 sprite, s16 x, s16 y){
	pa_3dsprites[sprite].corner[1].x = x;
	pa_3dsprites[sprite].corner[1].y = y;
}

/// Sets the bottom left corner of a 3D sprite.
static inline void PA_3DSetSpriteBottomLeft(u16 sprite, s16 x, s16 y){
	pa_3dsprites[sprite].corner[2].x = x;
	pa_3dsprites[sprite].corner[2].y = y;
}

/// Sets the bottom right corner of a 3D sprite.
static inline void PA_3DSetSpriteBottomRight(u16 sprite, s16 x, s16 y){
	pa_3dsprites[sprite].corner[3].x = x;
	pa_3dsprites[sprite].corner[3].y = y;
}

/// Sets the priority of a 3D sprite.
static inline void PA_3DSetSpritePrio(u16 sprite, u16 priority){
	pa_3dsprites[sprite].Priority = priority;
}   

/// Sets the PolyID of a 3D sprite.
static inline void PA_3DSetSpritePolyID(u16 sprite, u8 polyID){
   pa_3dsprites[sprite].polyID = polyID;
}   

/// Sets the alpha value of a 3D sprite.
static inline void PA_3DSetSpriteAlpha(u16 sprite, u8 alpha){
	pa_3dsprites[sprite].alpha = alpha;
}

/*! \fn void PA_3DStartSpriteAnimEx(u16 sprite, s16 firstframe, s16 lastframe, s16 speed, u8 type, s16 ncycles)
    \brief
         \~english Start a 3D sprite animation. Once started, it continues on and on by itself until you stop it !
         \~french Démarre une animation de sprite. Une fois démarrée, elle continue tant qu'on ne l'arrête pas !
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

void PA_3DStartSpriteAnimEx(u16 sprite, s16 firstframe, s16 lastframe, s16 speed, u8 type, s16 ncycles);

/*! \fn static inline void PA_3DStartSpriteAnim(u16 sprite, s16 firstframe, s16 lastframe, s16 speed)
    \brief
         \~english Start a sprite animation. Once started, it continues on and on by itself until you stop it !
         \~french Démarre une animation de sprite. Une fois démarrée, elle continue tant qu'on ne l'arrête pas !
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
static inline void PA_3DStartSpriteAnim(u16 sprite, s16 firstframe, s16 lastframe, s16 speed){
	PA_3DStartSpriteAnimEx(sprite, firstframe, lastframe, speed, ANIM_INFINITE);
}

/*! \fn static inline void PA_3DStopSpriteAnim(u16 sprite)
    \brief
         \~english Stop a sprite animation
         \~french Arrêter une animation de sprite
    \~\param sprite
         \~english sprite number in the sprite system
         \~french Numéro du sprite dans le systeme de sprite	
*/

static inline void PA_3DStopSpriteAnim(u16 sprite){
	if (sprite3danims[sprite].play) n3Dspriteanims --;
	sprite3danims[sprite].play = 0;
}

/*! \fn static inline void PA_3DSetSpriteAnimFrame(u16 sprite, u16 frame)
    \brief
         \~english Set the current animation frame number
         \~french Changer le numéro actuel de la frame d'animation
    \~\param sprite
         \~english sprite number in the sprite system
         \~french Numéro du sprite dans le systeme de sprite	
    \~\param frame
         \~english Frame number to use...
         \~french Numéro de frame...	 
*/

static inline void PA_3DSetSpriteAnimFrame(u16 sprite, u16 frame){
	if(sprite3danims[sprite].currentframe != frame){
		PA_3DSetSpriteFrame(sprite, frame);
		sprite3danims[sprite].currentframe = frame;
	}
}

/*! \fn static inline u16 PA_3DGetSpriteAnimFrame( u16 sprite)
    \brief
         \~english Returns the current animation frame number
         \~french Renvoie le numéro actuel de la frame d'animation
    \~\param sprite
         \~english sprite number in the sprite system
         \~french Numéro du sprite dans le systeme de sprite	 
*/

static inline u16 PA_3DGetSpriteAnimFrame(u16 sprite){
	return sprite3danims[sprite].currentframe;
}

/*! \fn static inline void PA_3DSetSpriteAnimSpeed(u16 sprite, s16 speed)
    \brief
         \~english Set the current animation speed
         \~french Changer la vitesse de l'animation
    \~\param sprite
         \~english sprite number in the sprite system
         \~french Numéro du sprite dans le systeme de sprite	
    \~\param speed
         \~english Speed, in fps...
         \~french Vitesse, en fps...		 
*/
static inline void PA_3DSetSpriteAnimSpeed(u16 sprite, s16 speed){
	sprite3danims[sprite].speed = speed;
}

/*! \fn static inline u16 PA_3DGetSpriteAnimSpeed(u16 sprite)
    \brief
         \~english Returns the current animation speed
         \~french Renvoie la vitesse de l'animation
    \~\param sprite
         \~english sprite number in the sprite system
         \~french Numéro du sprite dans le systeme de sprite	
*/
static inline u16 PA_3DGetSpriteAnimSpeed(u16 sprite){
	return sprite3danims[sprite].speed;
}


/*! \fn static inline void PA_3DSetSpriteNCycles(u16 sprite, s16 NCycles)
    \brief
         \~english Set the current animation cycles left (-1 for inifinite loop)
         \~french Changer le nombre de cycles d'animation restant (-1 pour inifini)
    \~\param sprite
         \~english sprite number in the sprite system
         \~french Numéro du sprite dans le systeme de sprite	
    \~\param NCycles
         \~english Number of cycles
         \~french Nombre de cycles		 
*/
static inline void PA_3DSetSpriteNCycles(u16 sprite, s16 NCycles){
	sprite3danims[sprite].ncycles = NCycles;
}

/*! \fn static inline u16 PA_3DGetSpriteNCycles(u16 sprite)
    \brief
         \~english Returns the current number of animation cycles left
         \~french Renvoie le nombre de cycles d'animation restants
    \~\param sprite
         \~english sprite number in the sprite system
         \~french Numéro du sprite dans le systeme de sprite	
*/
static inline u16 PA_3DGetSpriteNCycles(u16 sprite){
	return sprite3danims[sprite].speed;
}

/*! \fn static inline u16 PA_3DSpriteAnimPause(u16 sprite, u8 pause)
    \brief
         \~english Pause or UnPause a sprite animation
         \~french Mettre en Pause en remettre en lecture une animation de sprite
    \~\param sprite
         \~english sprite number in the sprite system
         \~french Numéro du sprite dans le systeme de sprite	
     \~\param pause
         \~english 1 for pause, 0 for unpause
         \~french 1 pour pause, 0 pour reprendre la lecture...
*/
static inline void PA_3DSpriteAnimPause(u16 sprite, u8 pause){
	if(pause && sprite3danims[sprite].play) n3Dspriteanims --;
	else if(!(pause) && !(sprite3danims[sprite].play)) n3Dspriteanims ++;
	sprite3danims[sprite].play = !pause;
}

/*
void PA_GifToTexTransp(u16 color);

u16 PA_3DCreateTexFromGif(void *gif, u8 palette);

static inline void PA_3DCreateSpriteFromGif(u16 sprite, void *gif, u8 palette, s16 x, s16 y){
	PA_3DCreateSpriteFromTex(sprite, PA_3DCreateTexFromGif(gif, palette), PA_GetGifWidth(gif), PA_GetGifHeight(gif), palette, x, y);
}
*/

/// Gets the X value of a 3D sprite.
static inline s32 PA_3DGetSpriteX(u16 sprite){ return pa_3dsprites[sprite].X; }
/// Gets the Y value of a 3D sprite.
static inline s32 PA_3DGetSpriteY(u16 sprite){ return pa_3dsprites[sprite].Y; }
/// Retrieves if a 3D sprite is visible.
static inline void PA_3DSetSpriteVisible(u16 sprite, u8 visible){ pa_3dsprites[sprite].Alive = visible; }

void PA_Init3DDual(void);

/** @} */ // end of 3DSprites

#endif
