#ifndef _PA_16C
#define _PA_16C

#ifdef __cplusplus
extern "C" {
#endif

/*! \file PA_16c.h
    \brief 16color pseudo-bitmap mode. 

    Special 16color background on which you can paste images. Usefull to show shots in SHMUP !
*/

#include "PA9.h"

#include "PA_Palette.h"
#include "PA_TextBits.h"

// Pointers...
extern u16 *PA_Draw16[2];
extern u32 *PA_Draw1632[2];

extern u16 *bittext_maps[10];		
extern u8 *bittext_tiles_blank[10];		
extern u32 *c16_tiles[10];	
extern u8 *pa_bittextdefaultsize[10];
extern u16 pa_bittextsizes[5];
extern 	u8 pa_bittextpoliceheight[10];

#define ALIGN_LEFT 0
#define ALIGN_RIGHT 1
#define ALIGN_CENTER 2
#define ALIGN_JUSTIFY 3

#define PA_16cLetterPos PA_LetterPos // For backwards compatibility

#define PA_LoadPal16c(palette, source)   DMA_Copy(source, (void*)palette, 16, DMA_16NOW);
#define PA_16cPos(x, y) ((((x)>>3)*26*8) + (y) + 8)
ALWAYSINLINE void PA_16c8X8(u8 screen, s16 x, s16 y, u32 *image);

extern u32 *c16_tiles[10];
extern u16 *bittext_maps[10];
extern u8 *pa16cdefaultsize[10];
extern u8 c16policeheight[10];
	
void PA_16c8X8Color(u8 screen, s16 x, s16 y, u32 *image, u8 color);

#define PA_Plot8Pixels(a,b,c,d) do{ PA_Draw1632[a][b] |= (d<<c); PA_Draw1632[a][b+(26*8)] |= (d>>(32-c)); }while(0)

static inline void PA_16c8pixels(u8 screen, s16 x, s16 y, u32 colors){
	PA_Plot8Pixels(screen, PA_16cPos(x+8, y+8), ((x&7)<<2), colors);
}

static inline void PA_16c16X16Letter(u8 screen, s16 x, s16 y, u8 letter, u8 size, u8 color){  
	u16 firstpos = ((letter&31)<<1)+((letter >> 5)<<7);
	PA_16c8X8Color(screen, x, y, (c16_tiles[size]+(bittext_maps[size][firstpos]<<3)), color);
	PA_16c8X8Color(screen, x, y+8, (c16_tiles[size]+(bittext_maps[size][firstpos+64]<<3)), color);
	firstpos ++;
	PA_16c8X8Color(screen, x+8, y, (c16_tiles[size]+(bittext_maps[size][firstpos]<<3)), color);
	PA_16c8X8Color(screen, x+8, y+8, (c16_tiles[size]+(bittext_maps[size][firstpos+64]<<3)), color);	
}  

static inline void PA_16c8X8Letter(u8 screen, s16 x, s16 y, u8 letter, u8 size, u8 color){  
	PA_16c8X8Color(screen, x, y, (c16_tiles[size]+(bittext_maps[size][letter]<<3)), color);
}  

/** @defgroup c16 16color pseudo-bitmap mode
 *  Special 16color background on which you can paste images. Usefull to show shots in SHMUP !
 *  @{
 */

/*! \fn void PA_Init16cBg(u8 screen, u8 bg)
    \brief
         \~english Initialise 16color background on which you can paste images...
         \~french Initialise le mode de dessin 16 couleurs, sur lequel on peut coller de petites images...
    \~\param screen
         \~english Choose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param bg
         \~english Background number (0-3)
         \~english Background number (0-3) 
*/
void PA_Init16cBgEx(u8 screen, u8 bg, u8 npalette);

/*! \fn static inline void PA_Init16cBg(u8 screen, u8 bg)
    \brief
         \~english Initialise 16color background on which you can paste images... Using palette 0
         \~french Initialise le mode de dessin 16 couleurs, sur lequel on peut coller de petites images... Utilisant la palette 0
    \~\param screen
         \~english Choose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param bg
         \~english Background number (0-3)
         \~english Background number (0-3) 
*/
static inline void PA_Init16cBg(u8 screen, u8 bg){
	PA_Init16cBgEx(screen, bg, 0);
}

/*! \fn static inline void PA_16cErase(u8 screen)
    \brief
         \~english Erase the 16color background. Must be used right after PA_WaitForVBL to avoid glitches
         \~french Effacer un écran de 16 couleurs. Doit être utilisé juste après PA_WaitForVBL pour éviter des erreurs d'affichage
    \~\param screen
         \~english Choose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
*/
void PA_16cErase(u8 screen);



/*! \fn static inline void PA_Dual16cErase(void)
    \brief
         \~english Erase the 16color background on both screens. Must be used right after PA_WaitForVBL to avoid glitches
         \~french Effacer un écran de 16 couleurs sur les 2 écrans. Doit être utilisé juste après PA_WaitForVBL pour éviter des erreurs d'affichage
*/
static inline void PA_Dual16cErase(void){
	PA_16cErase(0); PA_16cErase(1);
}


/*! \fn static inline void PA_InitComplete16c(u8 bg, void *Palette)
    \brief
         \~english Initialise a 16color background on each screen and give them a given palette
         \~french Initialise le mode de dessin 16 couleurs sur les 2 écrans avec une palette donnée
    \~\param bg
         \~english Background number
         \~french Numéro du fond
    \~\param Palette
         \~english 16 color palette...
         \~french Palette de 16 couleurs 
*/
static inline void PA_InitComplete16c(u8 bg, void *Palette){
	PA_LoadPal16c(PAL_BG0, Palette);
	PA_LoadPal16c(PAL_BG1, Palette);	
	PA_Init16cBg(0, bg); PA_Init16cBg(1, bg);
}


/*! \fn s16 PA_16cText(u8 screen, s16 basex, s16 basey, s16 maxx, s16 maxy, const char* text, u8 color, u8 size, s32 limit)
    \brief
         \~english This is a variable width and variable size function to draw text on the screen. 
         \~french Cette fonction permet d'écrire du texte à chasse variable à l'écran. 
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param basex
         \~english X coordinate of the top left corner
         \~french Coordonnée X du coin supérieur gauche
    \~\param basey
         \~english Y coordinate of the top left corner
         \~french Coordonnée Y du coin supérieur gauche
    \~\param maxx
         \~english X coordinate of the down right corner
         \~french Coordonnée X du coin inférieur droit
    \~\param maxy
         \~english Y coordinate of the down right corner
         \~french Coordonnée Y du coin inférieur droit
    \~\param text
         \~english Text, such as "Hello World"
         \~french Texte, tel que "Hello World"
    \~\param color
         \~english Palette color to use (0-255)
         \~french Couleur de la palette à utiliser (0-255)
    \~\param size
         \~english Size of the text, from 0 (really small) to 4 (pretty big)
         \~french Taille du texte, de 0 (vraiment petit) à 4 (assez grand)  
    \~\param limit
         \~english You can give a maximum number of characters to output. This can be usefull to have a slowing drawing text (allow to draw 1 more character each frame...)
         \~french On peut fixer une limite au nombre de caractères. Ceci peut etre utile pour dessiner un texte progressivement, en augmentant de 1 le nombre de caractères à chaque boucle....
*/
s16 PA_16cText(u8 screen, s16 basex, s16 basey, s16 maxx, s16 maxy, const char* text, u8 color, u8 size, s32 limit);

/*! \def PA_16cCustomFont(c16_slot, c16_font)
	\deprecated
    \brief
         \~english [DEPRECATED] Add custom fonts to the 16cText system !! Font must be converted with PAGfx
         \~french [DEPRECATED] Ajouter une police perso dans le systeme de texte 16c !! Doit être convertie avec PAGfx
    \~\param c16_slot
         \~english Font slot... 0-4 are used by the defaut PAlib fonts, 5-9 are free to use. You can freely overwrite the PAlib fonts if you want
         \~french Slot pour ajouter la police. Les slots 0-4 sont utilisés pour les polices par défaut de PAlib, et 5-9 sont libres. On peut néanmoins charger par-dessus les polices PAlib si on veut.
    \~\param c16_font
         \~english Font name...
         \~french Nom de la police... 
*/
#define PA_16cCustomFont(c16_slot, c16_font) do{\
	PA_DEPRECATED_MACRO;\
	bittext_maps[c16_slot] = (u16*)(void*)c16_font##_Map;\
	c16_tiles[c16_slot] = (u32*)(void*)c16_font##_Tiles;\
	pa_bittextdefaultsize[c16_slot] = (u8*)c16_font##_Sizes;\
	pa_bittextpoliceheight[c16_slot] = c16_font##_Height;\
}while(0)


/*! \fn void PA_Add16cFont(int slot, const PA_BgStruct* font)
    \brief
         \~english Add a custom font to the 16c font system.
         \~french Ajouter une police perso dans le système de texte 16c.
    \~\param slot
         \~english Font slot. 0-4 are used by the default PAlib fonts, 5-9 are free to use. You can freely overwrite the PAlib fonts if you want.
         \~french Slot pour ajouter la police. Les slots 0-4 sont utilisés pour les polices par défaut de PAlib, et 5-9 sont libres. On peut néanmoins charger par-dessus les polices PAlib si on veut.
    \~\param font
         \~english Pointer to the font.
         \~french Pointeur vers le police perso.
*/
void PA_Add16cFont(int slot, const PA_BgStruct* font);

/*! \fn ALWAYSINLINE PA_16cPutPixel(u8 screen, s16 x, s16 y, u32 color)
    \brief
         \~english Plot a pixel on a 16c background
         \~french Afficher un pixel sur un fond 16c
    \~\param screen
         \~english Screen...
         \~french Ecran...
    \~\param x
         \~english X position in pixels of the top left corner. Note that it ranges from -8 to 263, in order to allow half-way offscreen images. NEVER DRAW BEYOND THESE LIMITS, or else you'll get major background glitches
         \~french Position X en pixels du coin supérieur gauche. A noter que celle-ci va de -8 à 263, afin de permettre des images à moitié sorties... NE JAMAIS DEPASSER DU CADRE, sous peine de gros bugs graphiques...
    \~\param y
         \~english y position in pixels of the top left corner. Note that it ranges from -8 to 199, in order to allow half-way offscreen images. NEVER DRAW BEYOND THESE LIMITS, or else you'll get major background glitches
         \~french Position y en pixels du coin supérieur gauche. A noter que celle-ci va de -8 à 199, afin de permettre des images à moitié sorties... NE JAMAIS DEPASSER DU CADRE, sous peine de gros bugs graphiques...
    \~\param color
         \~english Pixel value (0-15, uses the loaded palette)
         \~french Valeur du pixel (0-15, prend la couleur dans la palette chargée)
*/
ALWAYSINLINE void PA_16cPutPixel(u8 screen, s16 x, s16 y, u32 color){
   x += 8; y += 8;
   u16 temp = (x&7)<<2;
   u16 pos = PA_16cPos(x, y);
   PA_Draw1632[screen][pos] = (color<<temp) | ( (~(15<<temp) ) & PA_Draw1632[screen][pos]) ;
}

ALWAYSINLINE void PA_16cDeletePixel(u8 screen, s16 x, s16 y){
   x += 8; y += 8;
   u16 temp = (x&7)<<2;
   u16 pos = PA_16cPos(x, y);
   PA_Draw1632[screen][pos] &= ~(15<<temp);
}

/*! \fn ALWAYSINLINE void PA_16c8X4(u8 screen, s16 x, s16 y, u32 *image)
    \brief
         \~english Load an 8x4 pixels image at a given position. Fastest of all pasting functions
         \~french Afficher une image de 8x4 pixels à un endroit donné, fonction la plus rapide de copie...
    \~\param screen
         \~english Screen...
         \~french Ecran...
    \~\param x
         \~english X position in pixels of the top left corner. Note that it ranges from -8 to 255, in order to allow half-way offscreen images. NEVER DRAW BEYOND THESE LIMITS, or else you'll get major background glitches
         \~french Position X en pixels du coin supérieur gauche. A noter que celle-ci va de -8 à 255, afin de permettre des images à moitié sorties... NE JAMAIS DEPASSER DU CADRE, sous peine de gros bugs graphiques...
    \~\param y
         \~english y position in pixels of the top left corner. Note that it ranges from -8 to 191, in order to allow half-way offscreen images. NEVER DRAW BEYOND THESE LIMITS, or else you'll get major background glitches
         \~french Position y en pixels du coin supérieur gauche. A noter que celle-ci va de -8 à 191, afin de permettre des images à moitié sorties... NE JAMAIS DEPASSER DU CADRE, sous peine de gros bugs graphiques...
    \~\param image
         \~english 16 color image to load. Use (u32*)ImageName if you get an error...
         \~french Image en 16 couleurs à charger. Utiliser (u32*)NomImage en cas d'erreur de compilation
*/
ALWAYSINLINE void PA_16c8X4(u8 screen, s16 x, s16 y, u32 *image){  
   x += 8; y += 8;
   u16 temp = (x&7)<<2;
   u16 pos = PA_16cPos(x, y);
   PA_Draw1632[screen][pos] |= (image[0]<<temp);
   PA_Draw1632[screen][pos+1] |= (image[1]<<temp);
   PA_Draw1632[screen][pos+2] |= (image[2]<<temp);
   PA_Draw1632[screen][pos+3] |= (image[3]<<temp);
   pos += (26*8); // Next columns
   temp = 32-temp;
   PA_Draw1632[screen][pos] |= (image[0]>>temp);
   PA_Draw1632[screen][pos+1] |= (image[1]>>temp);
   PA_Draw1632[screen][pos+2] |= (image[2]>>temp);
   PA_Draw1632[screen][pos+3] |= (image[3]>>temp);
}  

/*! \fn ALWAYSINLINE void PA_16c8X6(u8 screen, s16 x, s16 y, u32 *image)
    \brief
         \~english Load an 8x6 pixels image at a given position. Second fastest of all pasting functions
         \~french Afficher une image de 8x6 pixels à un endroit donné, deuxième fonction la plus rapide de copie...
    \~\param screen
         \~english Screen...
         \~french Ecran...
    \~\param x
         \~english X position in pixels of the top left corner. Note that it ranges from -8 to 255, in order to allow half-way offscreen images. NEVER DRAW BEYOND THESE LIMITS, or else you'll get major background glitches
         \~french Position X en pixels du coin supérieur gauche. A noter que celle-ci va de -8 à 255, afin de permettre des images à moitié sorties... NE JAMAIS DEPASSER DU CADRE, sous peine de gros bugs graphiques...
    \~\param y
         \~english y position in pixels of the top left corner. Note that it ranges from -8 to 191, in order to allow half-way offscreen images. NEVER DRAW BEYOND THESE LIMITS, or else you'll get major background glitches
         \~french Position y en pixels du coin supérieur gauche. A noter que celle-ci va de -8 à 191, afin de permettre des images à moitié sorties... NE JAMAIS DEPASSER DU CADRE, sous peine de gros bugs graphiques...
    \~\param image
         \~english 16 color image to load. Use (u32*)ImageName if you get an error...
         \~french Image en 16 couleurs à charger. Utiliser (u32*)NomImage en cas d'erreur de compilation
*/
ALWAYSINLINE void PA_16c8X6(u8 screen, s16 x, s16 y, u32 *image){  
   
   x += 8; y += 8;
   u16 temp = (x&7)<<2;
   u16 pos = PA_16cPos(x, y);
   PA_Plot8Pixels(screen, pos, temp, image[0]);
   PA_Plot8Pixels(screen, pos+1, temp, image[1]);  
   PA_Plot8Pixels(screen, pos+2, temp, image[2]);   
   PA_Plot8Pixels(screen, pos+3, temp, image[3]); 
   PA_Plot8Pixels(screen, pos+4, temp, image[4]);
   PA_Plot8Pixels(screen, pos+5, temp, image[5]);  
}  

/*! \fn ALWAYSINLINE void PA_16c8X8(u8 screen, s16 x, s16 y, u32 *image)
    \brief
         \~english Load an 8x8 pixels image at a given position. 
         \~french Afficher une image de 8x8 pixels à un endroit donné.
    \~\param screen
         \~english Screen...
         \~french Ecran...
    \~\param x
         \~english X position in pixels of the top left corner. Note that it ranges from -8 to 255, in order to allow half-way offscreen images. NEVER DRAW BEYOND THESE LIMITS, or else you'll get major background glitches
         \~french Position X en pixels du coin supérieur gauche. A noter que celle-ci va de -8 à 255, afin de permettre des images à moitié sorties... NE JAMAIS DEPASSER DU CADRE, sous peine de gros bugs graphiques...
    \~\param y
         \~english y position in pixels of the top left corner. Note that it ranges from -8 to 191, in order to allow half-way offscreen images. NEVER DRAW BEYOND THESE LIMITS, or else you'll get major background glitches
         \~french Position y en pixels du coin supérieur gauche. A noter que celle-ci va de -8 à 191, afin de permettre des images à moitié sorties... NE JAMAIS DEPASSER DU CADRE, sous peine de gros bugs graphiques...
    \~\param image
         \~english 16 color image to load. Use (u32*)ImageName if you get an error...
         \~french Image en 16 couleurs à charger. Utiliser (u32*)NomImage en cas d'erreur de compilation
*/
ALWAYSINLINE void PA_16c8X8(u8 screen, s16 x, s16 y, u32 *image){  
   x += 8; y += 8;
   u16 temp = (x&7)<<2;
   u16 pos = PA_16cPos(x, y);
   PA_Draw1632[screen][pos] |= (image[0]<<temp);
   PA_Draw1632[screen][pos+1] |= (image[1]<<temp);
   PA_Draw1632[screen][pos+2] |= (image[2]<<temp);
   PA_Draw1632[screen][pos+3] |= (image[3]<<temp);
   PA_Draw1632[screen][pos+4] |= (image[4]<<temp);   
   PA_Draw1632[screen][pos+5] |= (image[5]<<temp);
   PA_Draw1632[screen][pos+6] |= (image[6]<<temp);
   PA_Draw1632[screen][pos+7] |= (image[7]<<temp);  
   pos += (26*8); // Next columns
   temp = 32-temp;
   PA_Draw1632[screen][pos] |= (image[0]>>temp);
   PA_Draw1632[screen][pos+1] |= (image[1]>>temp);
   PA_Draw1632[screen][pos+2] |= (image[2]>>temp);
   PA_Draw1632[screen][pos+3] |= (image[3]>>temp);
   PA_Draw1632[screen][pos+4] |= (image[4]>>temp);   
   PA_Draw1632[screen][pos+5] |= (image[5]>>temp);
   PA_Draw1632[screen][pos+6] |= (image[6]>>temp);
   PA_Draw1632[screen][pos+7] |= (image[7]>>temp);      
}  

ALWAYSINLINE void PA_16c16X8(u8 screen, s16 x, s16 y, u32 *image){  
   x += 8; y += 8;
   u16 temp = (x&7)<<2;
   u16 pos = PA_16cPos(x, y);
   u8 i;
   for (i = 0; i < 2; i++){
	   PA_Plot8Pixels(screen, pos, temp, image[0]);
	   PA_Plot8Pixels(screen, pos+1, temp, image[1]);  
	   PA_Plot8Pixels(screen, pos+2, temp, image[2]);   
	   PA_Plot8Pixels(screen, pos+3, temp, image[3]); 
	   PA_Plot8Pixels(screen, pos+4, temp, image[4]);
	   PA_Plot8Pixels(screen, pos+5, temp, image[5]);  
	   PA_Plot8Pixels(screen, pos+6, temp, image[6]);   
	   PA_Plot8Pixels(screen, pos+7, temp, image[7]); 	
	   pos+=26*8;
	   image+=8;	   
	}
} 

ALWAYSINLINE void PA_16c16X12(u8 screen, s16 x, s16 y, u32 *image){  
   
   x += 8; y += 8;
   u16 temp = (x&7)<<2;
   u16 pos = PA_16cPos(x, y);
   u8 i;
   for (i = 0; i < 2; i++){
	   PA_Plot8Pixels(screen, pos, temp, image[0]);
	   PA_Plot8Pixels(screen, pos+1, temp, image[1]);  
	   PA_Plot8Pixels(screen, pos+2, temp, image[2]);   
	   PA_Plot8Pixels(screen, pos+3, temp, image[3]); 
	   PA_Plot8Pixels(screen, pos+4, temp, image[4]);
	   PA_Plot8Pixels(screen, pos+5, temp, image[5]);  
	   PA_Plot8Pixels(screen, pos+6, temp, image[6]);   
	   PA_Plot8Pixels(screen, pos+7, temp, image[7]); 	
	   PA_Plot8Pixels(screen, pos+8, temp, image[16]);
	   PA_Plot8Pixels(screen, pos+9, temp, image[17]);  
	   PA_Plot8Pixels(screen, pos+10, temp, image[18]);   
	   PA_Plot8Pixels(screen, pos+11, temp, image[19]); 	   
	   pos+=26*8;
	   image+=8;	   
	}
} 

ALWAYSINLINE void PA_16c16X16(u8 screen, s16 x, s16 y, u32 *image){  
   x += 8; y += 8;

   u16 temp = (x&7)<<2;
   u16 pos = PA_16cPos(x, y);
   u8 i;
   for (i = 0; i < 2; i++){
	   PA_Plot8Pixels(screen, pos, temp, image[0]);
	   PA_Plot8Pixels(screen, pos+1, temp, image[1]);  
	   PA_Plot8Pixels(screen, pos+2, temp, image[2]);   
	   PA_Plot8Pixels(screen, pos+3, temp, image[3]); 
	   PA_Plot8Pixels(screen, pos+4, temp, image[4]);
	   PA_Plot8Pixels(screen, pos+5, temp, image[5]);  
	   PA_Plot8Pixels(screen, pos+6, temp, image[6]);   
	   PA_Plot8Pixels(screen, pos+7, temp, image[7]); 	
	   PA_Plot8Pixels(screen, pos+8, temp, image[16]);
	   PA_Plot8Pixels(screen, pos+9, temp, image[17]);  
	   PA_Plot8Pixels(screen, pos+10, temp, image[18]);   
	   PA_Plot8Pixels(screen, pos+11, temp, image[19]); 
	   PA_Plot8Pixels(screen, pos+12, temp, image[20]);
	   PA_Plot8Pixels(screen, pos+13, temp, image[21]);  
	   PA_Plot8Pixels(screen, pos+14, temp, image[22]);   
	   PA_Plot8Pixels(screen, pos+15, temp, image[23]); 		   
	   pos+=26*8;
	   image+=8;	   
	}
}  

/*! \fn ALWAYSINLINE void PA_16c8Xi(u8 screen, s16 x, s16 y, u32 *image,u8 i)
    \brief
         \~english Load an 8xi row from a 8x16 pixels image at a given position. If i>16 the image is repeated.
         \~french Afficher une image de 8x8 pixels à un endroit donné.
    \~\param screen
         \~english Screen...
         \~french Ecran...
    \~\param x
         \~english X position in pixels of the top left corner. Note that it ranges from -8 to 255, in order to allow half-way offscreen images. NEVER DRAW BEYOND THESE LIMITS, or else you'll get major background glitches
         \~french Position X en pixels du coin supérieur gauche. A noter que celle-ci va de -8 à 255, afin de permettre des images à moitié sorties... NE JAMAIS DEPASSER DU CADRE, sous peine de gros bugs graphiques...
    \~\param y
         \~english y position in pixels of the top left corner. Note that it ranges from -8 to 191, in order to allow half-way offscreen images. NEVER DRAW BEYOND THESE LIMITS, or else you'll get major background glitches
         \~french Position y en pixels du coin supérieur gauche. A noter que celle-ci va de -8 à 191, afin de permettre des images à moitié sorties... NE JAMAIS DEPASSER DU CADRE, sous peine de gros bugs graphiques...
    \~\param image
         \~english 16 color image to load. Use (u32*)ImageName if you get an error...
         \~french Image en 16 couleurs à charger. Utiliser (u32*)NomImage en cas d'erreur de compilation
    \~\param i
    		\~english Number of lines of the image drawn (if greater than 16 the image will be repeated).
    		\~french Nombre de lignes à dessiner
*/
ALWAYSINLINE void PA_16c8Xi(u8 screen, s16 x, s16 y, u32 *image, u8 i){
	x += 8; y += 8;
	u16 temp = ((x)&7)<<2;
	u16 temp2 = 32-temp;
	u16 pos = PA_16cPos(x, y);
	u16 pos2 = pos + 208; // Next columns
	u8 j=0;
	while (j<i)
	{
		PA_Draw1632[screen][pos++] |= (image[j & 0x000F]<<temp);
		PA_Draw1632[screen][pos2++] |= (image[j & 0x000F]>>temp2);
		++j;
	}
}  

static inline void PA_16cLetter(u8 screen, s16 x, s16 y, char letter, u8 size, u8 color){
	if (pa_bittextpoliceheight[size]<=8) PA_16c8X8Letter(screen, x, y, letter, size, color);
	else PA_16c16X16Letter(screen, x, y, letter, size, color);
}

/*! \fn void PA_16cClearZone(u8 screen, s16 x1, s16 y1, s16 x2, s16 y2)
    \brief
         \~english Erase a 16c background zone
         \~french Effacer une partie d'un fond 16c
    \~\param screen
         \~english Screen...
         \~french Ecran...
    \~\param x1
         \~english Upper left corner...
         \~french Coin supérieur gauche...
    \~\param y1
         \~english Upper left corner...
         \~french Coin supérieur gauche...
    \~\param x2
         \~english Lower right corner...
         \~french Coin inférieur droit...
    \~\param y2
         \~english Lower right corner...
         \~french Coin inférieur droit...
*/
void PA_16cClearZone(u8 screen, s16 x1, s16 y1, s16 x2, s16 y2);

/*! \fn static inline u8 PA_16cGetPixel(u8 screen, s16 x, s16 y)
    \brief
         \~english Returns the pixel value of a given point on a 16c background
         \~french Renvoie la valeur d'un pixel donné sur un fond 16c
    \~\param screen
         \~english Screen...
         \~french Ecran...
    \~\param x
         \~english X value...
         \~french Valeur X...
    \~\param y
         \~english Y value...
         \~french Valeur Y...		 		 
*/
static inline u8 PA_16cGetPixel(u8 screen, s16 x, s16 y){
	x += 8; y += 8;
   u16 temp = (x&7)<<2;
   u16 pos = PA_16cPos(x, y);
   return (PA_Draw1632[screen][pos]&(15<<temp))>>temp;
}

// For backwards compatibility....
#define PA_16cTextAlign(align) PA_TextAlign(align)
#define PA_16cTextLineSpacing(spacing) PA_TextLineSpacing(spacing)
#define PA_16cTextLetterSpacing(spacing) PA_TextLetterSpacing(spacing)
#define PA_16cSetTextRot(rotate) textinfo.rot = rotate
s16 PA_16cTextRot(u8 screen, s16 basex, s16 basey, s16 maxx, s16 maxy, char* text, u8 color, u8 size, s32 limit);

extern u32 buffer16c[8];

/** @} */ // end of 16c

#ifdef __cplusplus
}
#endif

#endif
