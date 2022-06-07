#ifndef _PA_Fake16bit
#define _PA_Fake16bit

#ifdef __cplusplus
extern "C" {
#endif

#include "PA9.h"

/*! \file PA_Fake16bit.h
    \brief Fake 16 bit background functions

    This will allow you to create 16 bit backgrounds that use less memory.
*/



extern u16* PA_DrawFake16[2];
extern bool PA_HasFake16bitBg[2];

/** @defgroup Fake16bit Fake 16bit bitmap mode
 *  Functions to handle fake 16 bit backgrounds that take up less
 *  memory than real ones!
 *  @{
 */

/*! \fn void PA_InitFake16bitBg(u8 screen, u8 prio)
    \brief
         \~english Initialize a fake 16 bit background
         \~french [nothing]
    \~\param screen
         \~english Choose the screen (0 or 1)
         \~french [nothing]
    \~\param prio
         \~english Background priority (from 0 to 3, being 0 the highest)
         \~french [nothing]
*/
extern void PA_InitFake16bitBg(u8 screen, u8 prio);

/*! \fn void PA_DrawFake16bitLine(u8 screen, u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
    \brief
         \~english Draws a line on a fake 16 bit background
         \~french [nothing]
    \~\param screen
         \~english Choose the screen (0 or 1)
         \~french [nothing]
    \~\param x1
         \~english X position of the first point. Be carefull, if X is not between 0 and 255, it'll give unwanted results
         \~french Position X du premier point. Attention, si X n'est pas compris entre 0 et 255, le résultat ne sera pas celui escompté
    \~\param y1
         \~english Y position of the first point. Be carefull, if Y is not between 0 and 191, it'll give unwanted results
         \~french Position Y du premier point. Attention, si Y n'est pas compris entre 0 et 191, le résultat ne sera pas celui escompté
    \~\param x2
         \~english X position of the second point. Be carefull, if X is not between 0 and 255, it'll give unwanted results
         \~french Position X du deuxième point. Attention, si X n'est pas compris entre 0 et 255, le résultat ne sera pas celui escompté
    \~\param y2
         \~english Y position of the second point. Be carefull, if Y is not between 0 and 191, it'll give unwanted results
         \~french Position Y du deuxième point. Attention, si Y n'est pas compris entre 0 et 191, le résultat ne sera pas celui escompté
    \~\param color
         \~english 15 bits color. You can use the PA_RGB macro to set the RGB values...
         \~french Couleur de 15 bits.On peut utiliser la macro PA_RGB pour entrer les valeurs RGB...
*/
extern void PA_DrawFake16bitLine(u8 screen, u16 x1, u16 y1, u16 x2, u16 y2, u16 color);

/*! \def PA_LoadFake16bitBitmap(screen, bitmap)
    \brief
         \~english Load a 16 bit bitmap into a fake 16 bit background
         \~french [nothing]
    \~\param screen
         \~english Choose the screen (0 or 1)
         \~french [nothing]
    \~\param bitmap
         \~english Bitmap name
         \~french [nothing]
*/
#define PA_LoadFake16bitBitmap(screen, bitmap) DMA_Copy(bitmap, (void*)PA_DrawFake16[screen], 256*192, DMA_16NOW)

/*! \def PA_ClearFake16bitBg(screen)
    \brief
         \~english Clear a fake 16 bit background
         \~french [nothing]
    \~\param screen
         \~english Choose the screen (0 or 1)
         \~french [nothing]
*/
#define PA_ClearFake16bitBg(screen) dmaFillWords(0, (void*)PA_DrawFake16[screen], 256*192*2)

/*! \def PA_PutFake16bitPixel(screen, x, y, color)
    \brief
         \~english Plots a pixel into a fake 16 bit background
         \~french [nothing]
    \~\param screen
         \~english Choose the screen (0 or 1)
         \~french [nothing]
    \~\param x
         \~english X position of the point. Be carefull, if X is not between 0 and 255, it'll give unwanted results
         \~french Position X du point. Attention, si X n'est pas compris entre 0 et 255, le résultat ne sera pas celui escompté
    \~\param y
         \~english Y position of the point. Be carefull, if Y is not between 0 and 191, it'll give unwanted results
         \~french Position Y du point. Attention, si Y n'est pas compris entre 0 et 191, le résultat ne sera pas celui escompté
    \~\param color
         \~english 15 bits color. You can use the PA_RGB macro to set the RGB values...
         \~french Couleur de 15 bits.On peut utiliser la macro PA_RGB pour entrer les valeurs RGB...
*/
#define PA_PutFake16bitPixel(screen, x, y, color) PA_DrawFake16[screen][(x) + 256 * (y)] = color

/*! \def PA_GetFake16bitPixel(screen, x, y)
    \brief
         \~english Gets the color of a specified pixel of a fake 16 bit background
         \~french [nothing]
    \~\param screen
         \~english Choose the screen (0 or 1)
         \~french [nothing]
    \~\param x
         \~english X position of the point. Be carefull, if X is not between 0 and 255, it'll give unwanted results
         \~french Position X du point. Attention, si X n'est pas compris entre 0 et 255, le résultat ne sera pas celui escompté
    \~\param y
         \~english Y position of the point. Be carefull, if Y is not between 0 and 191, it'll give unwanted results
         \~french Position Y du point. Attention, si Y n'est pas compris entre 0 et 191, le résultat ne sera pas celui escompté
*/
#define PA_GetFake16bitPixel(screen, x, y) PA_DrawFake16[screen][(x) + 256 * (y)]

/*! \def PA_DrawFake16bitRect(u8 screen, u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
    \brief
         \~english Draws a rectangle on a fake 16 bit background
         \~french [nothing]
    \~\param screen
         \~english Choose the screen (0 or 1)
         \~french [nothing]
    \~\param x1
         \~english X position of the first point. Be carefull, if X is not between 0 and 255, it'll give unwanted results
         \~french Position X du premier point. Attention, si X n'est pas compris entre 0 et 255, le résultat ne sera pas celui escompté
    \~\param y1
         \~english Y position of the first point. Be carefull, if Y is not between 0 and 191, it'll give unwanted results
         \~french Position Y du premier point. Attention, si Y n'est pas compris entre 0 et 191, le résultat ne sera pas celui escompté
    \~\param x2
         \~english X position of the second point. Be carefull, if X is not between 0 and 255, it'll give unwanted results
         \~french Position X du deuxième point. Attention, si X n'est pas compris entre 0 et 255, le résultat ne sera pas celui escompté
    \~\param y2
         \~english Y position of the second point. Be carefull, if Y is not between 0 and 191, it'll give unwanted results
         \~french Position Y du deuxième point. Attention, si Y n'est pas compris entre 0 et 191, le résultat ne sera pas celui escompté
    \~\param color
         \~english 15 bits color. You can use the PA_RGB macro to set the RGB values...
         \~french Couleur de 15 bits.On peut utiliser la macro PA_RGB pour entrer les valeurs RGB...
*/
#define PA_DrawFake16bitRect(screen, x1, y1, x2, y2, color) do{\
	PA_DrawFake16bitLine(screen, x1, y1, x2, y1, color);\
	PA_DrawFake16bitLine(screen, x1, y1, x1, y2, color);\
	PA_DrawFake16bitLine(screen, x2, y1, x2, y2, color);\
	PA_DrawFake16bitLine(screen, x1, y2, x2, y2, color);}while(0)

/*! \def PA_Fake16bitLoadBmpEx(screen, x, y, bmp)
    \brief
         \~english Load a BMP on a fake 16 bit background... Don't forget to Init the background !
         \~french [nothing]
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)	
    \~\param x
         \~english X position of the top left corner
         \~french Position X du coin supérieur gauche	
    \~\param y
         \~english Y position of the top left corner
         \~french Position Y du coin supérieur gauche		 
    \~\param bmp
         \~english BMP image...
         \~french image au format BMP...	 
*/
#define PA_Fake16bitLoadBmpEx(screen, bmp, x, y) PA_LoadBmpToBuffer(PA_DrawFake16[screen], x, y, bmp, 256)

/*! \def PA_Fake16bitLoadBmp(screen, bmp)
    \brief
         \~english Load a BMP on a fake 16 bit background... Don't forget to Init the background !
         \~french [nothing]
    \~\param screen
         \~english Choose the screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)		 
    \~\param bmp
         \~english BMP image...
         \~french image au format BMP...	 
*/
#define PA_Fake16bitLoadBmp(screen, bmp) PA_Fake16bitLoadBmpEx(screen, bmp, 0, 0)

/*! \def PA_LoadGifXY(screen, x, y, gif)
    \brief
         \~english Load a Gif on a fake 16 bit background... Don't forget to Init the background !
         \~french [nothing]
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
#define PA_Fake16bitLoadGifXY(screen, gif, x, y) DecodeGif((const u8*)gif, (u8*)(PA_DrawFake16[screen] + x + (y<<8)), NULL, 1, 256);

/*! \def PA_Fake16bitLoadGif(screen, gif)
    \brief
         \~english Load a Gif on a fake 16 bit background... Don't forget to Init the background !
         \~french [nothing]
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)		 
    \~\param gif
         \~english Gif image...
         \~french image au format Gif...	 
*/
#define PA_Fake16bitLoadGif(screen, gif) PA_Fake16bitLoadGifXY(screen, gif, 0, 0)

/*! \def PA_Fake16bitLoadJpeg(screen, jpeg)
    \brief
         \~english Load a jpeg on a fake 16 bit background... Don't forget to Init the background !
         \~french [nothing]
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)	
    \~\param jpeg
         \~english jpeg image...
         \~french image au format jpeg...	 
*/
#define PA_Fake16bitLoadJpeg(screen, jpeg) JPEG_DecompressImage((u8*)jpeg, PA_DrawFake16[screen], 256, 192)

#ifdef __cplusplus
}
#endif

/** @} */ // end of fake 16 bit backgrounds

#endif
