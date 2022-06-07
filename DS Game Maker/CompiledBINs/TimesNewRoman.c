#include <PA_BgStruct.h>

extern const char TimesNewRoman_Tiles[];
extern const char TimesNewRoman_Map[];
extern const char TimesNewRoman_Pal[];

const PA_BgStruct TimesNewRoman = {
	PA_BgNormal,
	256, 192,

	TimesNewRoman_Tiles,
	TimesNewRoman_Map,
	{TimesNewRoman_Pal},

	5888,
	{1536}
};
