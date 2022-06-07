#pragma once

#include <stddef.h>

/*! \addtogroup BgTiles
	@{
*/

/// Graphics align define for PAGfx.
#define _GFX_ALIGN __attribute__((aligned (4)))

/// Types of background
enum{
	/// Invalid background
	PA_BgInvalid,
	/// Normal tiled background AKA TiledBg
	PA_BgNormal,
	/// Large background AKA LargeMap
	PA_BgLarge,
	/// Unlimited background AKA InfiniteMap
	PA_BgUnlimited,
	/// Rotational background
	PA_BgRot,
	/// 1-bit bitmap font
	PA_Font1bit,
	/// 4-bit bitmap font
	PA_Font4bit,
	/// 8-bit bitmap font
	PA_Font8bit
};

/// Background structure.
typedef struct{
	/// Type of background
	int BgType;
	/// Width of background in pixels
	int width;
	/// Height of background in pixels
	int height;

	/// Pointer to background tiles
	const void* BgTiles;
	/// Pointer to background map
	const void* BgMap;

	union{
		/// Pointer to palette
		const void* BgPalette;
		/// Pointer to font sizes
		const void* FontSizes;
	};

	/// Size of tiles in bytes
	size_t BgTiles_size;

	union{
		/// Size of the map in bytes
		size_t BgMap_size;
		/// Height of the font in pixels
		int FontHeight;
	};

	// No need for BgPalette_size since palettes for
	// backgrounds are always 512 bytes long.
}PA_BgStruct;

/*! @} */
