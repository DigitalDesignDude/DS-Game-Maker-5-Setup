#include <PA_BgStruct.h>

extern const char DSGMPro_Tiles[];
extern const char DSGMPro_Map[];
extern const char DSGMPro_Pal[];

const PA_BgStruct DSGMPro = {
	PA_BgNormal,
	512, 256,

	DSGMPro_Tiles,
	DSGMPro_Map,
	{DSGMPro_Pal},

	41728,
	{4096}
};
