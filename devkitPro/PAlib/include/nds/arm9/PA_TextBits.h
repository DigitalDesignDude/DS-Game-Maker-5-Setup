#ifndef _PA_TEXTBITS
#define _PA_TEXTBITS

#ifdef __cplusplus
extern "C" {
#endif

#include <PA9.h>

typedef struct{
	u8 linespacing;
	u8 letterspacing;
	u8 align;
	u8 rot;
} textinfo_type;
extern textinfo_type textinfo;

//extern textinfo_type textinfo;

extern u16 *bittext_maps[10];		
extern u8 *bittext_tiles_blank[10];		
extern u32 *c16_tiles[10];	
extern u8 *pa_bittextdefaultsize[10];
extern u16 pa_bittextsizes[5];

extern u8 pa_bittextpoliceheight[10];

typedef struct{
	s32 NLetters;
	struct{
		char Letter;
		u8 X, Y;
		u8 Size;
		u16 Color;
	}Letter[2048];
} LetterPos;
extern LetterPos PA_LetterPos;

void PA_DoAlign(u16 start, s16 x, s16 maxx, u8 justify);
void PA_DoLetterPos(s16 basex, s16 basey, s16 maxx, s16 maxy, const char* text, u16 color, u8 size, s32 limit);

static inline void PA_TextAlign(u8 align){
	textinfo.align = align;
}  

static inline void PA_TextLineSpacing(u8 spacing){
	textinfo.linespacing = spacing;
}  
static inline void PA_TextLetterSpacing(u8 spacing){
	textinfo.letterspacing = spacing;
}  

#ifdef __cplusplus
}
#endif

#endif
