#ifndef _PA_Gif
#define _PA_Gif

#ifdef __cplusplus
extern "C" {
#endif

/*! \file PA_Gif.h
    \brief Gif, animations...

    Manages everything about gif files. CONVERTING GRAPHICS AT RUNTIME IS A BAD IDEA AND SHOULD BE AVOIDED AT ALL COSTS.
*/

#include <PA9.h>

#include "PA_Interrupt.h"
#include "PA_Palette.h"
#include "PA_Draw.h"
#include "gif/gif_lib.h"

extern GifFileType* gifinfo;
extern s16 gifwidth, gifheight; 
extern u8 PA_nBit[2]; // 8 or 16 bit Bg
extern s32 GifCurrentLine;

typedef struct{
	s32 StartFrame;
	s32 EndFrame;
	s32 CurrentFrame;
	float Speed;
	u8 Play; // 0 stop; 1 play; 2 pause;
} PA_GifInfos;
extern PA_GifInfos PA_GifInfo;

void PA_Load16bitGif(u8 screen, s16 x, s16 y, void *gif);

int DecodeGif(const u8 *userData, u8 *ScreenBuff, u16* Palette, u8 nBits, s16 SWidth);

/** @defgroup Gif Gif functions
 *  Manages everything about gif files.
 *  @{
 */

/*! \fn static inline u16 PA_GetGifWidth(void* gif)
    \brief
         \~english Get a Gif's width in pixels
         \~french Récupérer la largeur d'un Gif en pixels
    \~\param gif
         \~english Gif image...
         \~french image au format Gif...	 
*/
static inline u16 PA_GetGifWidth(void* gif){
	u16 *temp = (u16*)gif;
	temp += 3;
	return *temp;
}

/*! \fn static inline u16 PA_GetGifHeight(void* gif)
    \brief
         \~english Get a Gif's height in pixels
         \~french Récupérer la hauteur d'un Gif en pixels
    \~\param gif
         \~english Gif image...
         \~french image au format Gif...	 
*/
static inline u16 PA_GetGifHeight(void* gif){
	u16 *temp = (u16*)gif;
	temp += 4;
	return *temp;
}

/*! \fn static inline void PA_LoadGifXY(u8 screen, s16 x, s16 y, void *gif)
    \brief
         \~english Load a Gif on a 16 bit background... Don't forget to Init the background !
         \~french Charger un Gif sur un fond de 16 bits... Faut pas oublier de charger ce fond avant !
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)	
    \~\param x
         \~english X position on the screen
         \~french Position X à l'écran
    \~\param y
         \~english Y position on the screen
         \~french Position Y à l'écran	 
    \~\param gif
         \~english Gif image...
         \~french image au format Gif...	 
*/
static inline void PA_LoadGifXY(u8 screen, s16 x, s16 y, void *gif){
	if (PA_nBit[screen]) DecodeGif((const u8*)gif, (u8*)(PA_DrawBg[screen] + x + (y<<8)), (u16*)0x05000000, 1, 256);
	else DecodeGif((const u8*)gif, (u8*)(PA_DrawBg[screen] + ((x + (y<<8))>>1)), (u16*)(0x05000000+(0x400*screen)), 0, 256);
}

/*! \fn static inline void PA_LoadGif(u8 screen, void *gif)
    \brief
         \~english Load a Gif on a 16 bit background... Don't forget to Init the background !
         \~french Charger un Gif sur un fond de 16 bits... Faut pas oublier de charger ce fond avant !
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)		 
    \~\param gif
         \~english Gif image...
         \~french image au format Gif...	 
*/
static inline void PA_LoadGif(u8 screen, void *gif){
	PA_LoadGifXY(screen, 0, 0, gif);
}

/*! \fn static inline void PA_GifAnimSpeed(float speed)
    \brief
         \~english Set the gif's speed
         \~french Changer la vitesse d'un gif
    \~\param speed
         \~english 1 for normal, 2 for 2x, 0.5 for half speed...
         \~french 1 pour normal, 2 pour 2x, 0.5 pour la moitié... 
*/
static inline void PA_GifAnimSpeed(float speed){
	PA_GifInfo.Speed = speed;
}

/*! \fn static inline void PA_GifAnimStop(void)
    \brief
         \~english Stop a Gif animation
         \~french Arrêter l'animation d'un gif
*/
static inline void PA_GifAnimStop(void){
	PA_GifInfo.Play = 0;
}

/*! \fn static inline void PA_GifAnimPause(void)
    \brief
         \~english Pause a Gif animation
         \~french Mettre en pause l'animation d'un gif
*/
static inline void PA_GifAnimPause(void){
	PA_GifInfo.Play = 2;
}

/*! \fn static inline void PA_GifAnimStop(void)
    \brief
         \~english Unpause a Gif animation
         \~french Reprendre l'animation d'un gif
*/
static inline void PA_GifAnimPlay(void){
	PA_GifInfo.Play = 1;
}

/*! \fn static inline void PA_GifSetStartFrame(s32 StartFrame)
    \brief
         \~english Set the Gif's starting frame number 
         \~french Régler à partir de quelle image commencer le gif
    \~\param StartFrame
         \~english Starting frame... (0 to start from beginning)
         \~french Image où démarrer... (0 pour le début)
*/
static inline void PA_GifSetStartFrame(s32 StartFrame){
	PA_GifInfo.StartFrame = StartFrame;
}

/*! \fn static inline void PA_GifSetEndFrame(s32 EndFrame)
    \brief
         \~english Set the Gif's ending frame number 
         \~french Régler à partir de quelle image arrêter le gif
    \~\param EndFrame
         \~english Ending frame... (100000 if you want to be sure ^^)
         \~french Image où démarrer... (100000 si vous voulez être sure de finir ^^)
*/
static inline void PA_GifSetEndFrame(s32 EndFrame){
	PA_GifInfo.EndFrame = EndFrame;
}
/*! \fn static inline s16 PA_GifGetFrame(void)
    \brief
         \~english Return's the gif's current frame
         \~french Renvoie le numéro d'image du gif en cours
*/
static inline s32 PA_GifGetFrame(void){
	return PA_GifInfo.CurrentFrame;
}

/*
/N! \fn void PA_GifToTiles(void *gif, u16 *temppal)
    \brief
         \~english Export Gif to a friendly 8x8 tile format, allowing it to be used to create sprites and backgrounds ! Returns a pointer towards your sprite gfx
         \~french Exporter un Gif au format de tiles de 8x8 pixels, ce qui permet de l'utiliser pour créer des sprites ou des fonds ! Renvoie un pointeur vers le gfx du sprite
    \~\param gif
         \~english Your gif file...
         \~french Fichier gif...
    \~\param temppal
         \~english A 256 u16 array that will receive the palette info to load
         \~french Un tableau de 256 entrée en u16, qui recevra les infos de la palette à charger	 
		 
		 
N/
u8* PA_GifToTiles(void *gif, u16 *temppal);
*/

/** @} */ // end of Gif

#ifdef __cplusplus
}
#endif

#endif
