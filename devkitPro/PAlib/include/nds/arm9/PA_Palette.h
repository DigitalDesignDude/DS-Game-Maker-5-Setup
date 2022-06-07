#ifndef _PA_Palette
#define _PA_Palette

void PA_LoadSpriteExtPal(u8 screen, u16 palette_number, void* palette);
#define PA_LoadBgExtPal(screen, bg_number, palette) PA_LoadBgPal(screen, bg_number, palette)

/*! \file PA_Palette.h
    \brief Everything concerning the palette system

    Not much for now, only a function to load a complete palette to a given palette
*/

//////////////////////////////////////////////////////////////////////
// Système pour les palettes
//////////////////////////////////////////////////////////////////////
#define PAL_BG0 0x05000000 
#define PAL_SPRITE0 0x05000200 
#define PAL_BG1 0x05000400 
#define PAL_SPRITE1 0x05000600 

#define REG_BRIGHT 0x0400006C  // Brightness

/** @defgroup Palette Palette system
 *  Load palettes, change palette colors, set the gamma, etc...
 *  @{
 */

/*! \def PA_LoadPal(palette, source)
    \brief
         \~english Load a 256 color palette in the Bg or Sprite palette of screen 0 or 1. Ex : PA_LoadPal(PALETTE_BG1, bg_pal);
         \~french Charger une palette de 256 couleurs pour les fonds ou les sprites pour l'écran 0 ou 1. Ex : PA_LoadPal(PALETTE_BG1, bg_pal);
    \~\param palette
         \~english Set the Bg palette or Obj palette, screen 0 or 1 : PAL_BG0, PAL_SPRITE0, PAL_BG1, or PAL_SPRITE1
         \~french Charger pour les Bg ou les Sprites, sur l'écran 0 ou 1 : PAL_BG0, PAL_SPRITE0, PAL_BG1, ou PAL_SPRITE1
    \~\param source
         \~english Palette name (ex : master_Palette)
         \~french Nom de la palette (ex : master_Palette)
*/
#define PA_LoadPal(palette, source) do{\
	DMA_Copy((void*)source, (void*)palette, 256, DMA_16NOW);\
	if (palette == PAL_SPRITE0) PA_LoadSpritePal(0, 0, (void*)source);\
	if (palette == PAL_SPRITE1) PA_LoadSpritePal(1, 0, (void*)source);\
	if (palette == PAL_BG0) {u8 itemp; for (itemp = 0; itemp < 4; itemp++) PA_LoadBgPal(0, itemp, (void*)(source));}\
	if (palette == PAL_BG1) {u8 itemp; for (itemp = 0; itemp < 4; itemp++) PA_LoadBgPal(1, itemp, (void*)(source));}}while(0)


/*! \fn static inline void PA_Load8bitBgPal(u8 screen, void *Pal)
    \brief
         \~english Load a palette to be used by the 8bit background
         \~french Charger une palette pour le fond 8bit
    \~\param screen
         \~english Screen...
         \~french Ecran...
    \~\param Pal
         \~english Palette name (ex : master_Palette)
         \~french Nom de la palette (ex : master_Palette)
*/
static inline void PA_Load8bitBgPal(u8 screen, void *Pal){
	u32 dest = PAL_BG0 + (screen<<10);
	DMA_Copy((void*)Pal, (void*)dest, 256, DMA_16NOW);
} 

/*! \def PA_LoadPal16(palette, n_palette, source)
    \brief
         \~english Load a 16 color palette in the Bg or Sprite palette of screen 0 or 1. Ex : PA_LoadPal16(PALETTE_BG1, 4, bg_pal);
         \~french Charger une palette de 16 couleurs pour les fonds ou les sprites pour l'écran 0 ou 1. Ex : PA_LoadPal16(PALETTE_BG1, 4, bg_pal);
    \~\param palette
         \~english Set the Bg palette or Obj palette, screen 0 or 1 : PAL_BG0, PAL_SPRITE0, PAL_BG1, or PAL_SPRITE1
         \~french Charger pour les Bg ou les Sprites, sur l'écran 0 ou 1 : PAL_BG0, PAL_SPRITE0, PAL_BG1, ou PAL_SPRITE1
    \~\param n_palette
         \~english Number of the 16 color palette to load (0-15)
         \~french Numéro de la palette de 16 couleurs que l'on veut charger (0-15)

    \~\param source
         \~english Palette name (ex : master_Palette)
         \~french Nom de la palette (ex : master_Palette)
*/
#define PA_LoadPal16(palette, n_palette, source) DMA_Copy((void*)source, (void*)(palette + (n_palette << 5)), 16, DMA_16NOW)

/*! \def PA_LoadSprite16cPal(screen, n_palette, palette)
    \brief
         \~english Load a 16 color palette for sprites
         \~french Charger une palette de 16 couleurs pour les sprites
    \~\param screen
         \~english Screen (0-1)
         \~french Ecran (0-1)
    \~\param n_palette
         \~english Number of the 16 color palette to load (0-15)
         \~french Numéro de la palette de 16 couleurs que l'on veut charger (0-15)

    \~\param palette
         \~english Palette name (ex : Sprite_Pal)
         \~french Nom de la palette (ex : Sprite_Pal)
*/
#define PA_LoadSprite16cPal(screen, n_palette, palette) PA_LoadPal16((PAL_SPRITE0+(0x400*screen)), (n_palette), palette)

/*! \def PA_RGB(r,g,b)
    \brief
         \~english Convert Red, Green, and Blue color indexes into a number used in the palette system. Careful : the R, G, B values range from 0 to 31 on gba !
         \~french Convertir une couleurs au format Rouge, Vert, Bleu en un nombre utilisable par le système de palette. Attention : sur Gba, les valeurs vont de 0 à 31...
    \~\param r
         \~english Red (0-31)
         \~french Rouge (0-31)
    \~\param g
         \~english Green (0-31)
         \~french Vert (0-31)
    \~\param b
         \~english Blue (0-31)
         \~french Bleu (0-31)

*/
#define PA_RGB(r,g,b) ((1<<15) | (r) | ((g)<<5) | ((b)<<10))

/*! \fn void PA_SetBrightness(u8 screen, s8 bright)
    \brief
         \~english Set the screen's brightness
         \~french Régler la luminosité de l'écran
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param bright
         \~english Brightness level, from -32 to 32, 0 being neutral
         \~french Luminosité, de -32 à 32, 0 étant neutre
*/
void PA_SetBrightness(u8 screen, s8 bright);

/*! \fn static inline void PA_SetPalNeg(u32 palette)
    \brief
         \~english Set all the palette's color to negative. To undo this, simply negative again...
         \~french Négativer une palette donnée. Pour annuler, il suffit de négativer à nouveau. 
    \~\param palette
         \~english Set the Bg palette or Obj palette, screen 0 or 1 : PAL_BG0, PAL_SPRITE0, PAL_BG1, or PAL_SPRITE1
         \~french Charger pour les Bg ou les Sprites, sur l'écran 0 ou 1 : PAL_BG0, PAL_SPRITE0, PAL_BG1, ou PAL_SPRITE1
*/
static inline void PA_SetPalNeg(u32 palette) {
	u16* pal = (u16*)palette;
	int i;

	for (i = 0; i < 256; i++) pal[i] = ~pal[i]; // On fout le négatif...
}

/*! \fn static inline void PA_SetPal16Neg(u32 palette, u8 n_palette)
    \brief
         \~english Set 16 color palette to negative. To undo this, simply negative again...
         \~french Négativer une palette de 16 couleurs donnée. Pour annuler, il suffit de négativer à nouveau. 
    \~\param palette
         \~english Set the Bg palette or Obj palette, screen 0 or 1 : PAL_BG0, PAL_SPRITE0, PAL_BG1, or PAL_SPRITE1
         \~french Charger pour les Bg ou les Sprites, sur l'écran 0 ou 1 : PAL_BG0, PAL_SPRITE0, PAL_BG1, ou PAL_SPRITE1
    \~\param n_palette
         \~english Number of the 16 color palette (0-15)
         \~french Numéro de la palette de 16 couleurs (0-15)
*/
static inline void PA_SetPal16Neg(u32 palette, u8 n_palette) {
	u16* pal = (u16*)palette;
	int i;

	for (i = (n_palette << 4); i < ((n_palette + 1) << 4); i++) pal[i] = ~pal[i]; // On fout le négatif...
}


/*! \fn void PA_InitSpriteExtPal()
    \brief
         \~english Initialise 16 palette mode for 256 color sprites. Done by default
         \~french Initialise le mode 16 palettes pour sprites de 256 couleurs... Effectué par défaut
*/
void PA_InitSpriteExtPal();


/*! \fn void PA_InitBgExtPal()
    \brief
         \~english Initialise 16 palette mode for 256 color backgrounds
         \~french Initialise le mode 16 palettes pour fonds de 256 couleurs...
*/
void PA_InitBgExtPal();

/*! \fn void PA_LoadSpritePal(u8 screen, u8 palette_number, void* palette)
    \brief
         \~english Load a 256 color palette for Sprites
         \~french Charger une palette de 256 couleurs pour les sprites
    \~\param screen
         \~english Screen...
         \~french Ecran...
    \~\param palette_number
         \~english Palette number (0-15)
         \~french Numéro de la palette (0-15)
    \~\param palette
         \~english Palette to load ((void*)palette_name)
         \~french Nom de la palette à charger ((void*)nom_palette)
*/
static inline void PA_LoadSpritePal(u8 screen, u8 palette_number, void* palette){
	PA_LoadSpriteExtPal(screen, palette_number, palette);
}

/*! \fn PA_LoadBgPalN(u8 screen, u8 bg_number, u8 pal_number, void* palette)
    \brief
         \~english Load a 256 color palette in the Background palettes, to a given slot
         \~french Charger une palette de 256 couleurs dans les palettes des fonds, à un slot donné
    \~\param screen
         \~english Screen...
         \~french Ecran...
    \~\param bg_number
         \~english Background number (0-3)
         \~french Numéro du fond (0-3)
    \~\param pal_number
         \~english Palette number
         \~french Numéro de palette	 
    \~\param palette
         \~english Palette to load ((void*)palette_name)
         \~french Nom de la palette à charger ((void*)nom_palette)
*/
void PA_LoadBgPalN(u8 screen, u8 bg_number, u8 pal_number, void* palette);

/*! \fn void PA_LoadBgPal(u8 screen, u16 bg_number, void* palette)
    \brief
         \~english Load a 256 color palette in the Background palettes
         \~french Charger une palette de 256 couleurs dans les palettes des fonds
    \~\param screen
         \~english Screen...
         \~french Ecran...
    \~\param bg_number
         \~english Background number (0-3)
         \~french Numéro du fond (0-3)
    \~\param palette
         \~english Palette to load ((void*)palette_name)
         \~french Nom de la palette à charger ((void*)nom_palette)
*/
static inline void PA_LoadBgPal(u8 screen, u16 bg_number, void* palette){
	PA_LoadBgPalN(screen, bg_number, 0, palette);
}

/*! \fn void PA_LoadBgPalN(u8 screen, u8 bg_number, u8 pal_number, void* palette)
    \brief
         \~english Load a 256 color palette in a given Background's palette
         \~french Charger une palette de 256 couleurs dans les palettes des fonds
    \~\param screen
         \~english Screen...
         \~french Ecran...
    \~\param bg_number
         \~english Background number (0-3)
         \~french Numéro du fond (0-3)
    \~\param pal_number
         \~english Palette number (0-15)
         \~french Numéro de la palette (0-15)		 
    \~\param palette
         \~english Palette to load ((void*)palette_name)
         \~french Nom de la palette à charger ((void*)nom_palette)
*/
void PA_LoadBgPalN(u8 screen, u8 bg_number, u8 pal_number, void* palette);


/*! \def PA_SetBgPalCol(screen, color_number, colorRGB)
    \brief
         \~english Change the color of one of the main background palette colors. Not used anymore
         \~french Changer la couleur d'une des couleurs de la palette des fonds. Ne plus utiliser
    \~\param screen
         \~english Screen...
         \~french Ecran...
    \~\param color_number
         \~english Color number in palette (0-255)
         \~french Numéro de la couleur dans la palette (0-255)
    \~\param colorRGB
         \~english RGB value, like PA_RGB(31, 31, 31) for white
         \~french Valeur RGB, comme PA_RGB(31, 31, 31) pour blanc
*/
#define PA_SetBgPalCol(screen, color_number, colorRGB) BG_PALETTE[color_number + ((screen) << 9)] = colorRGB

/*! \fn void PA_SetBgPalNCol(u8 screen, u8 bg_number, u8 pal_number, u8 color_number, u16 color)
    \brief
         \~english Change the color of one of the backgrounds' palettes' colors
         \~french Changer la couleur d'une des couleurs d'une palette d'un fonds
    \~\param screen
         \~english Screen...
         \~french Ecran...
    \~\param bg_number
         \~english Background number (0-3)
         \~french Numéro du fond (0-3)		 
    \~\param pal_number
         \~english Palette number (0-15). Leave to 0 if unsure
         \~french Numéro de palette (0-15), laisser à 0 si pas sur...		 
    \~\param color_number
         \~english Color number in palette (0-255)
         \~french Numéro de la couleur dans la palette (0-255)
    \~\param color
         \~english RGB value, like PA_RGB(31, 31, 31) for white
         \~french Valeur RGB, comme PA_RGB(31, 31, 31) pour blanc
*/
void PA_SetBgPalNCol(u8 screen, u8 bg_number, u8 pal_number, u8 color_number, u16 color);

/*! \fn static inline void PA_SetBgColor(u8 screen, u16 color)
    \brief
         \~english Change the background color of a given screen
         \~french Changer la couleur de fond d'un écran
    \~\param screen
         \~english Screen...
         \~french Ecran...
    \~\param color
         \~english RGB value, like PA_RGB(31, 31, 31) for white
         \~french Valeur RGB, comme PA_RGB(31, 31, 31) pour blanc
*/
static inline void PA_SetBgColor(u8 screen, u16 color){
	BG_PALETTE[screen<<9] = color;
} 

/*! \fn void PA_SetSpritePalCol(u8 screen, u8 pal_number, u8 color_number, u16 color)
    \brief
         \~english Changes a color in a sprite palette
         \~french Changer la couleur de fond d'un écran
    \~\param screen
         \~english Screen...
         \~french Ecran...
    \~\param pal_number
         \~english Palette number
         \~french Numéro de la palette
    \~\param color_number
         \~english Color in the palette
         \~french Numéro de la couleur
    \~\param color
         \~english Color (given by PA_RGB...)
         \~french Couleur (venant de PA_RGB...)	 
*/
void PA_SetSpritePalCol(u8 screen, u8 pal_number, u8 color_number, u16 color);

/*! \fn void PA_3DSetSpritePalCol(u8 pal_number, u8 color_number, u16 color)
    \brief
         Changes a color in a 3d sprite palette
    \param pal_number
         Palette number
    \param color_number
         Color number in the palette
    \param color
         Color (given by PA_RGB...)
*/
void PA_3DSetSpritePalCol(u8 pal_number, u8 color_number, u16 color);

#define PA_AdjustCol(color, bright) do{color+= bright; if (color < 0) color = 0; if (color > 31) color = 31;}while(0)

void PA_CreatePalBright(u16 *pal, u16 *newpal, s8 bright);
void PA_CreatePalTransition(u16 *pal, u16 *newpal, s8 level, u8 destr, u8 destg, u8 destb);

/** @} */ // end of Palette
//////////////////////////////////////////////////////////////////////

static inline void* PA_GetBgPal(u8 screen, u8 bg_number){
	if (screen == 0) return (void*)(VRAM_E + (bg_number << 12));
	else return (void*)(VRAM_H + (bg_number << 12));
}

#endif
