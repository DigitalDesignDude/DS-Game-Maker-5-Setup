#include <PA_BgStruct.h>

extern const char Default_Tiles[];
extern const char Default_Map[];
extern const char Default_Pal[];

const PA_BgStruct Default = {
	PA_BgNormal,
	256, 192,

	Default_Tiles,
	Default_Map,
	{Default_Pal},

	12160,
	{1536}
};
