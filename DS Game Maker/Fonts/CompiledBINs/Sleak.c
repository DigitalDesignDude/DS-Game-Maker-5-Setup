#include <PA_BgStruct.h>

extern const char Sleak_Tiles[];
extern const char Sleak_Map[];
extern const char Sleak_Pal[];

const PA_BgStruct Sleak = {
	PA_BgNormal,
	256, 192,

	Sleak_Tiles,
	Sleak_Map,
	{Sleak_Pal},

	4032,
	{1536}
};
