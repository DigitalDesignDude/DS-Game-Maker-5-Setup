#include <PA_BgStruct.h>

extern const char ComicSans_Tiles[];
extern const char ComicSans_Map[];
extern const char ComicSans_Pal[];

const PA_BgStruct ComicSans = {
	PA_BgNormal,
	256, 192,

	ComicSans_Tiles,
	ComicSans_Map,
	{ComicSans_Pal},

	5760,
	{1536}
};
