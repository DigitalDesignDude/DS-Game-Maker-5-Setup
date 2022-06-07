#include <PA_BgStruct.h>

extern const char TrebuchetMS_Tiles[];
extern const char TrebuchetMS_Map[];
extern const char TrebuchetMS_Pal[];

const PA_BgStruct TrebuchetMS = {
	PA_BgNormal,
	256, 192,

	TrebuchetMS_Tiles,
	TrebuchetMS_Map,
	{TrebuchetMS_Pal},

	5824,
	{1536}
};
