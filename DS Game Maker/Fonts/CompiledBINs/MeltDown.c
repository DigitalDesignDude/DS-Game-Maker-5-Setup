#include <PA_BgStruct.h>

extern const char MeltDown_Tiles[];
extern const char MeltDown_Map[];
extern const char MeltDown_Pal[];

const PA_BgStruct MeltDown = {
	PA_BgNormal,
	256, 192,

	MeltDown_Tiles,
	MeltDown_Map,
	{MeltDown_Pal},

	4416,
	{1536}
};
