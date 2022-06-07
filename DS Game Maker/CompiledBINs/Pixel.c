#include <PA_BgStruct.h>

extern const char Pixel_Tiles[];
extern const char Pixel_Map[];
extern const char Pixel_Pal[];

const PA_BgStruct Pixel = {
	PA_BgNormal,
	256, 192,

	Pixel_Tiles,
	Pixel_Map,
	{Pixel_Pal},

	4096,
	{1536}
};
