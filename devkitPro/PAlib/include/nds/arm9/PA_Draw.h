#ifndef _PA_Buffer
#define _PA_Buffer

#ifdef __cplusplus
extern "C" {
#endif

/*! \file PA_Draw.h
    \brief Bitmap mode, for drawing, loading images in 8 or 16 bit mode...

    Draw on screen, either a pixel or a line, or anything ! Load a Bitmap, a Jpeg...
*/

#include "PA9.h"

#include "PA_Interrupt.h"
#include "PA_Palette.h"
#include "PA_Gif.h"

extern u8 PA_nBit[2]; // 8 or 16 bit Bg

extern u16 tempvar;

#define PA_RGB8(r,g,b)	((((b)>>3)<<10)|(((g)>>3)<<5)|((r)>>3)|(1 << 15))

#define __PACKED __attribute__ ((__packed__))

typedef struct{
	u16 Id; // ?
	u32 Length;
	u16 Nothing1, Nothing2; // ?
	u16 ImageStart1, ImageStart2; // Offset of start of image, start at position 0x0A, which can only be 2-byte aligined
} __PACKED BMPHeader0;

typedef struct{
	u32 SizeofHeader; // 40
	u32 Width, Height;
	u16 Colorplanes; // Usually 1
	u16 BitsperPixel; //1, 2, 4, 8, 16, 24, 32
	u32 Compression;  // 0 for none, 1...
	u32 SizeofData; // Not reliable
	u32 WidthperMeter, HeightperMeter; // Don't care
	u32 NColors, ImportantColors; // Number of colors used, important colors ?
} BMP_Headers;

extern u16 PA_oldx[2];
extern u16 PA_oldy[2];
extern s16 PA_olddowntime[2];
extern u8 PA_drawsize[2];
extern u16 *PA_DrawBg[2];
extern u32 *PA_DrawBg32[2];
extern u8 PA_nBit[2]; // 8 or 16 bit Bg

extern u16 PA_temppal[256];

/** @defgroup Bitmap Bitmap mode
 *  Draw on screen, either a pixel or a line, or anything ! Load a Bitmap, a Jpeg...
 *  @{
 */

/*! \fn void PA_Init8bitBg(u8 screen, u8 bg_priority)
    \brief
         \~english Initialise 8 bit draw mode (palette mode)... Chose the screen and the background priority (0-3). This drawable background will replace Background 3, and must be loaded before all other backgrounds. Takes about 3/8 of the VRAM.
         \~french Initialise le mode de dessin 8 bit (avec palette). Il suffit de choisir l'écran et la priorité de ce font (de 0 à 3). Ce fond sera placé sur le fond 3 (le remplacant), et doit etre chargé avant tout autre fond ! Prend environ 3/8 de la VRAM
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param bg_priority
         \~english Background priority (0-3)
         \~english Background priority (0-3) 
*/
void PA_Init8bitBg(u8 screen, u8 bg_priority);

/*! \fn void PA_InitBig8bitBg(u8 screen, u8 bg_priority)
    \brief
         \~english Same as PA_Init8bitBg, but with an available size of 256x256. Takes up a little more space but allows correct vertical scrolling...
         \~french Similaire à PA_Init8bitBg, mais avec une taille de 256x256. Ceci prend un peu plus de mémoire, mais autorise le scrolling vertical
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param bg_priority
         \~english Background priority (0-3)
         \~english Background priority (0-3) 
*/
void PA_InitBig8bitBg(u8 screen, u8 bg_priority);

void PA_8bitSwapBuffer(u8 screen);
void PA_Init8bitDblBuffer(u8 screen, u8 bg_priority);


/*! \fn void PA_Init16bitBg(u8 screen, u8 bg_priority)
    \brief
         \~english Initialise 16 bit draw mode (no palette mode, true colors)... Chose the screen and the background priority (0-3). This drawable background will replace Background 3, and must be loaded before all other backgrounds. Takes about 6/8 of the VRAM, so almost all the space !
         \~french Initialise le mode de dessin 16 bit (sans palettes, couleurs RGB). Il suffit de choisir l'écran et la priorité de ce font (de 0 à 3). Ce fond sera placé sur le fond 3 (le remplacant), et doit etre chargé avant tout autre fond ! Prend environ 3/8 de la VRAM
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param bg_priority
         \~english Background priority (0-3)
         \~english Background priority (0-3) 
*/
void PA_Init16bitBg(u8 screen, u8 bg_priority);

void PA_Init16bitDblBuffer(u8 screen, u8 bg_priority);
void PA_16bitSwapBuffer(u8 screen);

/*! \fn static inline void PA_Put8bitPixel(u8 screen, s16 x, s16 y, u8 color) 
    \brief
         \~english Draw a pixel on screen, on an 8 bit background
         \~french Dessine un pixel à l'écran, sur un fond de 8 bits
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param x
         \~english X position (0-255)
         \~french Position X (0-255)
    \~\param y
         \~english Y position (0-191)
         \~french Position Y (0-191)	
    \~\param color
         \~english Color in the background palette (0-255)
         \~french Couleur de la palette du fond (0-255) 
*/

static inline void PA_Put8bitPixel(u8 screen, s16 x, s16 y, u8 color) {
	u32 pos = (y<<7) + (x>>1);
	u32 decal = ((x&1)<<3);
	PA_DrawBg[screen][pos] &= 0xff00>>decal;
	PA_DrawBg[screen][pos] |= color<<decal;
}

/*! \fn static inline void PA_Put2_8bitPixels(u8 screen, s16 x, s16 y, u16 colors)
    \brief
         \~english Draw 2 pixels on screen, on an 8 bit background. These pixels are next to another, and the first pixel must be with a pair X. WAY faster than drawing both pixels separately
         \~french Dessine deux pixels à l'écran, sur un fond de 8 bits. Ces pixels sont contigus, et le premier doit avoir une position X pair. Beaucoup plus rapide que de dessiner les 2 pixels séparemment
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1) 
   \~\param x
         \~english X position (0-254), must be PAIR
         \~french Position X (0-254), doit etre PAIR
    \~\param y
         \~english Y position (0-191)
         \~french Position Y (0-191)	
    \~\param colors
         \~english Colors of the first and second pixels (*256 for the second)
         \~french Couleurs des premier et deuxième pixels (*256 pour le deuxième)
	 
*/
static inline void PA_Put2_8bitPixels(u8 screen, s16 x, s16 y, u16 colors) {		
	PA_DrawBg[screen][(x >> 1) + (y << 7)] = colors;
}

/*! \fn static inline void PA_PutDouble8bitPixels(u8 screen, s16 x, s16 y, u8 color1, u8 color2)
    \brief
         \~english Draw 2 pixels on screen, on an 8 bit background. These pixels are next to another, and the first pixel must be with a pair X. WAY faster than drawing both pixels separately
         \~french Dessine deux pixels à l'écran, sur un fond de 8 bits. Ces pixels sont contigus, et le premier doit avoir une position X pair. Beaucoup plus rapide que de dessiner les 2 pixels séparemment
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1) 
   \~\param x
         \~english X position (0-254), must be PAIR
         \~french Position X (0-254), doit etre PAIR
    \~\param y
         \~english Y position (0-191)
         \~french Position Y (0-191)	
    \~\param color1
         \~english Color of the first pixel, in the background palette (0-255)
         \~french Couleur de la palette du fond (0-255) pour le premier pixel
    \~\param color2
         \~english Color of the second pixel, in the background palette (0-255)
         \~french Couleur de la palette du fond (0-255) pour le deuxième pixel		 
*/
static inline void PA_PutDouble8bitPixels(u8 screen, s16 x, s16 y, u8 color1, u8 color2) {
	PA_Put2_8bitPixels(screen, x, y, color1 + (color2 << 8));		
}

/*! \fn static inline void PA_Put4_8bitPixels(u8 screen, s16 x, s16 y, u32 colors)
    \brief
         \~english Draw 4 pixels on screen, on an 8 bit background. These pixels are next to another, and the first pixel must be with a pair X. Fastest way to draw on the screen...
         \~french Dessine 4 pixels à l'écran, sur un fond de 8 bits. Ces pixels sont contigus, et le premier doit avoir une position X pair. Façon la plus rapide de dessiner à l'écran
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1) 
   \~\param x
         \~english X position (0-254), must be PAIR
         \~french Position X (0-254), doit etre PAIR
    \~\param y
         \~english Y position (0-191)
         \~french Position Y (0-191)	
    \~\param colors
         \~english Colors of the 4 pixels
         \~french Couleurs des 4 pixels
	 
*/
static inline void PA_Put4_8bitPixels(u8 screen, s16 x, s16 y, u32 colors) {
	PA_DrawBg32[screen][(x >> 2) + (y << 6)] = colors;
}

/*! \fn static inline u8 PA_Get8bitPixel(u8 screen, u8 x, u8 y)
    \brief
         \~english Get the pixel's color in 8 bit Draw mode... 
         \~french Récupérer la couleur d'un pixel, en mode dessin 8 bit
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1) 
    \~\param x
         \~english X position. Be carefull, if X is not between 0 and 255, it'll give unwanted results
         \~french Position X. Attention, si X n'est pas compris entre 0 et 255, le résultat ne sera pas celui escompté
    \~\param y
         \~english Y position. Be carefull, if Y is not between 0 and 191, it'll give unwanted results
         \~french Position Y. Attention, si Y n'est pas compris entre 0 et 191, le résultat ne sera pas celui escompté
 */
static inline u8 PA_Get8bitPixel(u8 screen, u8 x, u8 y) {
	u16 pos = (x >> 1) + (y << 7);
	if(x & 1)
		return (PA_DrawBg[screen][pos] >> 8);
	else
		return (PA_DrawBg[screen][pos]);
}

/*! \fn static inline void PA_Put16bitPixel(u8 screen, s16 x, s16 y, u16 color)
    \brief
         \~english Draw a pixel on screen, on an 16 bit background
         \~french Dessine un pixel à l'écran, sur un fond de 16 bits
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param x
         \~english X position (0-255)
         \~french Position X (0-255)
    \~\param y
         \~english Y position (0-191)
         \~french Position Y (0-191)	
    \~\param color
         \~english 16 bit color, obtained using PA_RGB(red, green, blue)
         \~french Couleur de 16 bits, obtenue avec PA_RGB(rouge, vert, bleu)
*/
static inline void PA_Put16bitPixel(u8 screen, s16 x, s16 y, u16 color) {
	PA_DrawBg[screen][x + (y << 8)] = color;
}

/*! \def PA_Get16bitPixel(x, y)
    \brief
         \~english Get the pixel's color in 16 bit Draw mode... 
         \~french Récupérer la couleur d'un pixel, en mode dessin 16 bit
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1) 
    \~\param x
         \~english X position. Be carefull, if X is not between 0 and 255, it'll give unwanted results
         \~french Position X. Attention, si X n'est pas compris entre 0 et 255, le résultat ne sera pas celui escompté
    \~\param y
         \~english Y position. Be carefull, if Y is not between 0 and 191, it'll give unwanted results
         \~french Position Y. Attention, si Y n'est pas compris entre 0 et 191, le résultat ne sera pas celui escompté
 */
#define PA_Get16bitPixel(screen, x, y) PA_DrawBg[screen][(x) + ((y) << 8)]

/*! \fn void PA_Draw8bitLine(u8 screen, u16 x1, u16 y1, u16 x2, u16 y2, u8 color)
    \brief
         \~english Draw a line in Draw mode... for 8 bit drawable background
         \~french Dessiner une ligne en mode dessin... pour le mode dessin 8 bit
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1) 
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
         \~english Color in the background palette (0-255)
         \~french Couleur de la palette du fond (0-255) 
 */
void PA_Draw8bitLine(u8 screen, u16 x1, u16 y1, u16 x2, u16 y2, u8 color);

/*! \fn void PA_Draw16bitLine(u8 screen, u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
    \brief
         \~english Draw a line in Draw mode... for 16 bit drawable background
         \~french Dessiner une ligne en mode dessin... pour le mode dessin 16 bit
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1) 
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
void PA_Draw16bitLine(u8 screen, u16 x1, u16 y1, u16 x2, u16 y2, u16 color);

/*! \fn void PA_Draw16bitLineEx(u8 screen, s16 basex, s16 basey, s16 endx, s16 endy, u16 color, s8 size)
    \brief
         \~english Draw a thick line in Draw mode... for 16 bit drawable background
         \~french Dessiner une ligne épaisse en mode dessin... pour le mode dessin 16 bit
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1) 
    \~\param basex
         \~english X position of the first point. Be carefull, if X is not between 0 and 255, it'll give unwanted results
         \~french Position X du premier point. Attention, si X n'est pas compris entre 0 et 255, le résultat ne sera pas celui escompté
    \~\param basey
         \~english Y position of the first point. Be carefull, if Y is not between 0 and 191, it'll give unwanted results
         \~french Position Y du premier point. Attention, si Y n'est pas compris entre 0 et 191, le résultat ne sera pas celui escompté
    \~\param endx
         \~english X position of the second point. Be carefull, if X is not between 0 and 255, it'll give unwanted results
         \~french Position X du deuxième point. Attention, si X n'est pas compris entre 0 et 255, le résultat ne sera pas celui escompté
    \~\param endy
         \~english Y position of the second point. Be carefull, if Y is not between 0 and 191, it'll give unwanted results
         \~french Position Y du deuxième point. Attention, si Y n'est pas compris entre 0 et 191, le résultat ne sera pas celui escompté
    \~\param color
         \~english 15 bits color. You can use the PA_RGB macro to set the RGB values...
         \~french Couleur de 15 bits.On peut utiliser la macro PA_RGB pour entrer les valeurs RGB...
    \~\param size
         \~english Width of the line, in pixels
         \~french Largeur du trait, en pixels		 
 */
void PA_Draw16bitLineEx(u8 screen, s16 basex, s16 basey, s16 endx, s16 endy, u16 color, s8 size);

/*! \fn void PA_Draw8bitLineEx(u8 screen, s16 basex, s16 basey, s16 endx, s16 endy, u8 color, s8 size)
    \brief
         \~english Draw a thick line in Draw mode... for 8 bit drawable background
         \~french Dessiner une ligne épaisse en mode dessin... pour le mode dessin 8 bit
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1) 
    \~\param basex
         \~english X position of the first point. Be carefull, if X is not between 0 and 255, it'll give unwanted results
         \~french Position X du premier point. Attention, si X n'est pas compris entre 0 et 255, le résultat ne sera pas celui escompté
    \~\param basey
         \~english Y position of the first point. Be carefull, if Y is not between 0 and 191, it'll give unwanted results
         \~french Position Y du premier point. Attention, si Y n'est pas compris entre 0 et 191, le résultat ne sera pas celui escompté
    \~\param endx
         \~english X position of the second point. Be carefull, if X is not between 0 and 255, it'll give unwanted results
         \~french Position X du deuxième point. Attention, si X n'est pas compris entre 0 et 255, le résultat ne sera pas celui escompté
    \~\param endy
         \~english Y position of the second point. Be carefull, if Y is not between 0 and 191, it'll give unwanted results
         \~french Position Y du deuxième point. Attention, si Y n'est pas compris entre 0 et 191, le résultat ne sera pas celui escompté
    \~\param color
         \~english 15 bits color. You can use the PA_RGB macro to set the RGB values...
         \~french Couleur de 15 bits.On peut utiliser la macro PA_RGB pour entrer les valeurs RGB...
    \~\param size
         \~english Width of the line, in pixels
         \~french Largeur du trait, en pixels		 
 */
void PA_Draw8bitLineEx(u8 screen, s16 basex, s16 basey, s16 endx, s16 endy, u8 color, s8 size);

/*! \fn void PA_Draw16bitRect(u8 screen, s16 basex, s16 basey, s16 endx, s16 endy, u16 color)
    \brief
         \~english Draw a rectangle in Draw mode... for 16 bit drawable background
         \~french Dessiner rectangle en mode dessin... pour le mode dessin 16 bit
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1) 
    \~\param basex
         \~english X position of the first point. Be carefull, if X is not between 0 and 255, it'll give unwanted results
         \~french Position X du premier point. Attention, si X n'est pas compris entre 0 et 255, le résultat ne sera pas celui escompté
    \~\param basey
         \~english Y position of the first point. Be carefull, if Y is not between 0 and 191, it'll give unwanted results
         \~french Position Y du premier point. Attention, si Y n'est pas compris entre 0 et 191, le résultat ne sera pas celui escompté
    \~\param endx
         \~english X position of the second point. Be carefull, if X is not between 0 and 255, it'll give unwanted results
         \~french Position X du deuxième point. Attention, si X n'est pas compris entre 0 et 255, le résultat ne sera pas celui escompté
    \~\param endy
         \~english Y position of the second point. Be carefull, if Y is not between 0 and 191, it'll give unwanted results
         \~french Position Y du deuxième point. Attention, si Y n'est pas compris entre 0 et 191, le résultat ne sera pas celui escompté
    \~\param color
         \~english 15 bits color. You can use the PA_RGB macro to set the RGB values...
         \~french Couleur de 15 bits.On peut utiliser la macro PA_RGB pour entrer les valeurs RGB...		 
 */
void PA_Draw16bitRect(u8 screen, s16 basex, s16 basey, s16 endx, s16 endy, u16 color);

/*! \fn PA_8bitDraw(u8 screen, u8 color)
    \brief
         \~english For 8 bit background : Nice little function that draws on screen ! All you need to do is chose the color, it'll do the rest. If the PA VBL isn't initialised, don't forget to update the stylus position every frame... Juste execute PA_Draw every cycle...
         \~french Pour 8 bit : Jolie petite fonction qui dessine à l'écran ! Tout ce qu'il reste à faire, c'est de choisir la couleur. Si le VBL PA n'est pas initialiser, ne pas oublier de rafraichir le Stylet à chaque cycle (et non, pas avec des glacons !). Il suffit d'executer PA_Draw à chaque cycle pour dessiner...
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1) 
    \~\param color
         \~english Color number in the palette (0-255)
         \~french Numéro de la couleur dans la palette (0-255)
 */
void PA_8bitDraw(u8 screen, u8 color);

/*! \fn PA_16bitDraw(u8 screen, u16 color)
    \brief
         \~english For 16 bit : Nice little function that draws on screen ! All you need to do is chose the color, it'll do the rest. If the PA VBL isn't initialised, don't forget to update the stylus position every frame... Juste execute PA_Draw every cycle...
         \~french Pour 16 bit : Jolie petite fonction qui dessine à l'écran ! Tout ce qu'il reste à faire, c'est de choisir la couleur. Si le VBL PA n'est pas initialiser, ne pas oublier de rafraichir le Stylet à chaque cycle (et non, pas avec des glacons !). Il suffit d'executer PA_Draw à chaque cycle pour dessiner...
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1) 
    \~\param color
         \~english 15 bits color. You can use the PA_RGB macro to set the RGB values...
         \~french Couleur de 15 bits.On peut utiliser la macro PA_RGB pour entrer les valeurs RGB...
 */
void PA_16bitDraw(u8 screen, u16 color);

/*! \def PA_SetDrawSize(screen, draw_size)
    \brief
         \~english Set the size of the pen when drawing.
         \~french Regler la taille du stylo quand on dessine.
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1) 		 
    \~\param draw_size
         \~english Size...
         \~french Taille...
*/
#define PA_SetDrawSize(screen, draw_size) PA_drawsize[screen] = draw_size;

/*! \def PA_Load8bitBitmap(screen, bitmap)
    \brief
         \~english Load a bitmap on the screen for an 8 bit drawable background
         \~french Charger une image à l'écran... pour une fond dessinable de 8 bits
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)	
    \~\param bitmap
         \~english Bitmap name
         \~french Nom du bitmap
*/
#define PA_Load8bitBitmap(screen, bitmap) DMA_Copy(bitmap, (void*)PA_DrawBg[screen], 256*96, DMA_16NOW)

/*! \def PA_Load16bitBitmap(screen, bitmap)
    \brief
         \~english Load a bitmap on the screen for an 16 bit drawable background
         \~french Charger une image à l'écran... pour une fond dessinable de 16 bits
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)	
    \~\param bitmap
         \~english Bitmap name
         \~french Nom du bitmap
*/
#define PA_Load16bitBitmap(screen, bitmap) do{u32 PA_temp; \
	for (PA_temp = 0; PA_temp < 256*192; PA_temp++)\
	PA_DrawBg[screen][PA_temp] = bitmap[PA_temp] | (1 << 15);}while(0)


/*! \def PA_Clear8bitBg(screen)
    \brief
         \~english Clears the screen... for an 8 bit drawable background
         \~french Efface l'écran... pour une fond dessinable de 8 bits
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)		 
*/
#define PA_Clear8bitBg(screen) dmaFillWords(0, (void*)PA_DrawBg[screen], 256*96*2);

/*! \def PA_Clear16bitBg(screen)
    \brief
         \~english Clears the screen... for an 16 bit drawable background
         \~french Efface l'écran... pour une fond dessinable de 16 bits
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)		 
*/
#define PA_Clear16bitBg(screen) dmaFillWords(0, (void*)PA_DrawBg[screen], 256*192*2)


/*! \fn static inline void PA_LoadJpeg(u8 screen, void *jpeg)
    \brief
         \~english Load a jpeg on a 16 bit background... Don't forget to Init the background !
         \~french Charger un jpeg sur un fond de 16 bits... Faut pas oublier de charger ce fond avant !
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)	
    \~\param jpeg
         \~english jpeg image...
         \~french image au format jpeg...	 
*/
static inline void PA_LoadJpeg(u8 screen, void *jpeg) {
	REG_IME = 0;
	JPEG_DecompressImage((u8*)jpeg, PA_DrawBg[screen], 256, 192);
	REG_IME = 1;	
}



/*! \fn void PA_LoadBmpToBuffer(u16 *Buffer, s16 x, s16 y, void *bmp, s16 SWidth)
    \brief
         \~english Load a BMP in a 16 bit Buffer
         \~french Charger un BMP dans un buffer de 16 bit
    \~\param Buffer
         \~english Buffer...
         \~french Buffer...
    \~\param x
         \~english X position of the top left corner
         \~french Position X du coin supérieur gauche	
    \~\param y
         \~english Y position of the top left corner
         \~french Position Y du coin supérieur gauche		 
    \~\param bmp
         \~english BMP image...
         \~french image au format BMP...
    \~\param SWidth
         \~english Buffer width to use (256 for screen width...)
         \~french Largeur du buffer, en pixels (256 pour la taille de l'écran...) 
*/
void PA_LoadBmpToBuffer(u16 *Buffer, s16 x, s16 y, void *bmp, s16 SWidth);


/*! \fn static inline void PA_LoadBmpEx(u8 screen, s16 x, s16 y, void *bmp)
    \brief
         \~english Load a BMP on a 16 bit background... Don't forget to Init the background !
         \~french Charger un BMP sur un fond de 16 bits... Faut pas oublier de charger ce fond avant !
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
static inline void PA_LoadBmpEx(u8 screen, s16 x, s16 y, void *bmp){
	PA_LoadBmpToBuffer(PA_DrawBg[screen], x, y, bmp, 256);
}



/*! \fn static inline void PA_LoadBmp(u8 screen, void *bmp)
    \brief
         \~english Load a BMP on a 16 bit background... Don't forget to Init the background !
         \~french Charger un BMP sur un fond de 16 bits... Faut pas oublier de charger ce fond avant !
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)		 
    \~\param bmp
         \~english BMP image...
         \~french image au format BMP...	 
*/
static inline void PA_LoadBmp(u8 screen, void *bmp){
	PA_LoadBmpEx(screen, 0, 0, bmp);
}

/*! \fn static inline u16 PA_GetBmpWidth(void *bmp)
    \brief
         \~english Get a BMP's width in pixels
         \~french Récupérer la largeur d'un BMP en pixels
    \~\param bmp
         \~english BMP image...
         \~french image au format BMP...	 
*/
static inline u16 PA_GetBmpWidth(void *bmpdata){
	u8 *temp = (u8*)bmpdata;
	BMP_Headers *Bmpinfo = (BMP_Headers*)(temp+14);
	return Bmpinfo->Width;
}


/*! \fn static inline u16 PA_GetBmpHeight(void *bmp)
    \brief
         \~english Get a BMP's height in pixels
         \~french Récupérer la hauteur d'un BMP en pixels
    \~\param bmp
         \~english BMP image...
         \~french image au format BMP...	 
*/
static inline u16 PA_GetBmpHeight(void *bmpdata){
	u8 *temp = (u8*)bmpdata;
	BMP_Headers *Bmpinfo = (BMP_Headers*)(temp+14);
	return Bmpinfo->Height;
}
/** @} */ // end of Draw

static inline void PA_8bit8x8Image(u8 screen, u16 basex, u16 basey, u8* data){
	int x, y;

	for (x = 0; x < 8; x++)
		for (y = 0; y < 8; y++)
			PA_Put8bitPixel(screen, x + basex, y + basey, data[x + (y << 3)]);
}

#ifdef __cplusplus
}
#endif

#endif
