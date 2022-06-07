#ifndef _PA_MAIN
#define _PA_MAIN

/*!
	\mainpage PAlib 100707 Documentation

	\section intro Introduction
	Welcome to the PAlib documentation. Here you'll find
	information on how to use PAlib.

	\section core_lib Core library
	- \ref General "General functions"
	- \ref Debug "Debugging utilities"
	- \ref Math "Math functions"
	- \ref cxx "C++ wrappers"

	\section bg_section Tiled backgrounds
	- \ref BgTiles "Normal background functions"
	- \ref BgRot "Rotating background functions"
	- \ref TileDual "Dual background functions"
	- \ref Text "Text system"
	- \ref Mode7 "Mode 7 functions"

	\section bmp_section Bitmapped backgrounds
	- \ref Bitmap "Bitmapped background functions"
	- \ref c16 "16-color bitmapped background functions"
	- \ref Fake16bit "Fake 16-bit background functions"
	- \ref Gif "GIF functions"

	\section spr_section Sprites
	- \ref Sprite "Sprite functions"
	- \ref SpriteDual "Dual sprite functions"
	- \ref f3DSprites "3D Sprite functions"

	\section pal_section Palettes
	- \ref Palette "Palette functions"
	- \ref PaletteDual "Dual palette functions"

	\section input_section Input
	- \ref Keys "Pad and stylus functions"
	- \ref Keyboard "Keyboard functions"
	- \ref Reco "Handwriting recognition functions"
	- \ref Micro "Microphone functions"
	- \ref KeysSpecial "Special controller functions"

	\section snd_section Sound
	- \ref ASlib "ASlib library"

	\section misc_section Misc.
	- \ref SpecialFx "Special effects"

	\example Text/Normal/HelloWorld/source/main.c
	\example Backgrounds/Effects/Mode7/source/main.c
*/

#ifdef __cplusplus
extern "C"{
#endif

#include <nds.h>
#include <fat.h>
#include <filesystem.h>

#include <malloc.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include "PA_IPC.h"
#include "PA_FifoMessages.h"
#include "PA_BgStruct.h"

#include "arm9/jpeg.h"

#include "arm9/PA_Assert.h"
#include "arm9/PA_General.h"
#include "arm9/PA_Text.h"
#include "arm9/PA_TextBits.h"
#include "arm9/PA_BgTiles.h"
#include "arm9/PA_BgLargeMap.h"
#include "arm9/PA_Sprite.h"
#include "arm9/PA_SpriteDual.h"
#include "arm9/PA_Sound.h"
#include "arm9/PA_Micro.h"
#include "arm9/PA_Draw.h"
#include "arm9/PA_Interrupt.h"
#include "arm9/PA_Keys.h"
#include "arm9/PA_Motion.h"
#include "arm9/PA_BgRot.h"
#include "arm9/PA_Timer.h"
#include "arm9/PA_Math.h"
#include "arm9/PA_Palette.h"
#include "arm9/PA_PaletteDual.h"
#include "arm9/PA_SpecialFx.h"
#include "arm9/PA_TileDual.h"
#include "arm9/PA_Keyboard.h"
#include "arm9/PA_Window.h"
#include "arm9/PA_Mode7.h"
#include "arm9/PA_3DSprites.h"
#include "arm9/PA_IA.h"
#include "arm9/PA_Reco.h"
#include "arm9/PA_16c.h"
#include "arm9/PA_Wifi.h"
#include "arm9/PA_Gif.h"
#include "arm9/PA_BgTrans.h"
#include "arm9/PA_KeysSpecial.h"
#include "arm9/PA_Fake16bit.h"
#include "arm9/PA_Debug.h"

#include "arm9/as_lib9.h"

#ifdef __cplusplus
}

// C++ specific header
#include "arm9/cpp/PA_CppHeader.h"

#endif

#endif
