#include <PA_BgStruct.h>

extern const char Italics_Tiles[];
extern const char Italics_Map[];
extern const char Italics_Pal[];

const PA_BgStruct Italics = {
	PA_BgNormal,
	256, 192,

	Italics_Tiles,
	Italics_Map,
	{Italics_Pal},

	6080,
	{1536}
};
