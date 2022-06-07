#ifndef _PA_GENERAL
#define _PA_GENERAL

#ifdef __cplusplus
extern "C" {
#endif

#include <PA_IPC.h>
#include <PA_FifoMessages.h>

#include <arm9/as_lib9.h>
#include <arm9/PA_Sound.h>
#include <arm9/PA_Interrupt.h>

#include <nds.h>
#include <nds/memory.h>
#include <nds/bios.h>
#include <malloc.h>

#include "../PA_Transfer.h"

#include <stdarg.h> // Pour le système de text
#include <string.h>

#ifndef PA_NO_DEPRECATION
#define DEPRECATED __attribute__ ((deprecated))
#define PA_DEPRECATED_MACRO _PA_DeprecationStub()
#else
#define DEPRECATED
#define PA_DEPRECATED_MACRO ((void)0)
#endif

static inline void _PA_DeprecationStub() __attribute__((deprecated));
static inline void _PA_DeprecationStub(){}

extern u8 PA_Screen;

typedef struct {
	u16 Year;
	u8 Month;
	u8 Day;
	u8 Hour;
	u8 Minutes;
	u8 Seconds;
//	u8 FPS, Frames, OldSeconds; // To test the FPS...
} RTC;
extern RTC PA_RTC;

static inline void PA_WaitForVBL();

extern s16 PA_ScreenSpace; // Espace entre les 2 écrans...+192

#define PAWaitForVBL() swiWaitForVBlank()
/*
static inline void PAWaitForVBL(){
	//PA_RTC.Frames ++; // For the FPS counter
	swiWaitForVBlank();
}
*/
static inline void PA_PowerOff(){
	powerOn(PM_SYSTEM_PWR);
}

extern u8 pa_checklid;

#define BG_GFX1			0x6000000
#define BG_GFX2			0x6200000
#define SPRITE_GFX1		0x6400000
#define SPRITE_GFX2		0x6600000

//Pour le DMA Copy...
#define REG_DMA3SRC *(volatile u32*)0x040000D4
#define REG_DMA3DST *(volatile u32*)0x040000D8
#define REG_DMA3CNT *(volatile u32*)0x040000DC
#define DMA_ON 0x80000000
#define DMA_NOW 0x00000000
#define DMA_16 0x00000000
#define DMA_32 0x04000000
#define DMA_16NOW (DMA_ON | DMA_NOW | DMA_16)
#define DMA_32NOW (DMA_ON | DMA_NOW | DMA_32)

#define DMA_Copy(source, dest, count, mode) do{if(((u32)(source)) == 0) DMA_Clear(dest, count, mode); else{REG_DMA3SRC = (u32)source; REG_DMA3DST = (u32)dest; REG_DMA3CNT = (count) | (mode);}}while(0)

#define DMA_Clear(dest, count, mode) do{ if((mode) & DMA_32) dmaFillWords(0, (void*) (dest), (count) << 2); else dmaFillHalfWords(0, (void*) (dest), (count) << 1); }while(0)

#define DMA_Force(ulVal, dest, count, mode) do{REG_DMA3SRC=(u32)&ulVal; REG_DMA3DST = (u32)dest; REG_DMA3CNT = (count) |(mode) | DMA_SRC_FIX;}while(0)

#define DMA_CopyEx(type, source, dest, count, mode) do{DMA_SRC(type) = (u32)source; DMA_DEST(type) = (u32)dest; DMA_CR(type) = (count) | (mode);}while(0)


// Commandes pour la lumière des écrans
#define BACKLIGHT(screen)	BIT(2+screen)

#define ALWAYSINLINE __attribute__((always_inline)) static inline

#define _REG8 *(vu16*)
#define _REG16 *(vu16*)
#define _REG32 *(vu32*)

#define SWITCH_SCREENS  (1<<15)

#define N_HBLS 263

/*! \file PA_General.h
    \brief Contains prototypes and macros... for the arm9

    Contains prototypes and macros... for the arm9
*/

// The PAlib IPC is deprecated...
extern volatile DEPRECATED PA_IPCType PA_IPC;

//////////////////////////////////////////////////////////////////////
// Général
//////////////////////////////////////////////////////////////////////

#define SIZEOF_8BIT(x)  (sizeof(x))
#define SIZEOF_16BIT(x) (sizeof(x)>>1)
#define SIZEOF_32BIT(x) (sizeof(x)>>2)

#define PACKED __attribute__ ((packed))

// Jpeg functions:
void PA_jpeg_GetImageSize(char* name, int* width, int* height);
void PA_jpeg_BltImage(char* name, u16* vram, int output_width, int output_height);

extern const s16 PA_SIN[512];

typedef struct {
   u8 Color;  // Favorite Color
   u8 BdayMonth; // Mois d'anniversaire
   u8 BdayDay;  // Jour de naissance
   u8 AlarmHour;
   u8 AlarmMinute;
   u8 Name[21];
   u8 NameLength;
   u8 Message[53];
   u8 MessageLength;
   u8 Language; // Langue... 0 pour Jap, 1 pour anglais, 2 pour français...
} infos;
extern infos PA_UserInfo;

/** @defgroup General General Functions
 *  Initialise the lib, and other general functions...
 *  @{
 */

/*! \fn void PA_Init(void)
    \brief
         \~english Initialise the library. Should be used at the beginning of main()
         \~french Initialise la lib... Doit etre placé au début de main()
*/
void PA_Init();

/*! \fn void PA_InitFifo()
    \brief
         \~english Initialize the Fifo system. It is automatically done in PA_Init().
         \~french Initialise le système Fifo. C'est fait automatiquement dans PA_Init().
*/
void PA_InitFifo();

/*! \def PA_LegacyIPCInit()
	\deprecated
    \brief
         \~english [DEPRECATED] Initialize the legacy IPC system.
         \~french TODO
*/

#define PA_LegacyIPCInit() do{ \
	memset((void*) &PA_IPC, 0, sizeof(PA_IPCType)); \
	PA_Transfer->mailData = (u32)(&PA_IPC); \
}while(0)

/*! \fn void PA_Init2D(void)
    \brief
         \~english Resets to 2D state after using 3D functions
         \~french Remet en mode 2D après avoir utilisé la 3D
*/
void PA_Init2D();

// for the video modes...
extern u8 PA_ExtPal[2][2];

/*! \fn void PA_SetVideoMode(u8 screen, u8 mode)
    \brief
         \~english Change the video mode... Use this with caution
         \~french Changer de mode video... A utiliser avec précaution
    \~\param screen
         \~english Screen...
         \~french Ecran...
    \~\param mode
         \~english Mode 0 for normal, 1 for 1 rotating backgrounds, 2 for 2
         \~french Mode 0 pour normal, 1 pour 1 fond rotatif, 2 pour 2
*/
void PA_SetVideoMode(u8 screen, u8 mode);

#define PA_GetVideoMode(screen) (((screen) ? REG_DISPCNT_SUB : REG_DISPCNT) & 7)


/*! \fn void PA_UpdateUserInfo(void)
    \brief
         \~english Updates the user info. This is automatically done in PA_Init. You can then get any info with the following variables : PA_UserInfo.Color (favorite color), .BdayDay, .BdayMonth, .AlarmHour, .AlarmMinute, .Name, .NameLength, .Message, .MessageLength, .Language
         \~french Met à jour les infos sur l'utilisateur... C'est fait automatiquement dans PA_Init. On peut ensuite récupérer toutes les infos avec PA_UserInfo.Color (couleur favorite), .BdayDay, .BdayMonth, .AlarmHour, .AlarmMinute, .Name, .NameLength, .Message, .MessageLength, .Language
*/
void PA_UpdateUserInfo(void);


/*! \fn void PA_UpdateRTC(void)
    \brief
         \~english Updates the Real Time Clock, with info on the current date and hour. Automatically updated in the PA VBL... Get the info with PA_RTC.Minutes, .Hour, .Seconds, .Day, .Month, and .Year
         \~french Met à jour les infos sur l'heure et la date. C'est automatiquement mis à jour dans le VBL PA... On récupère les infos avec PA_RTC.Minutes, .Hour, .Seconds, .Day, .Month, et .Year
*/
void PA_UpdateRTC(void);

/*! \fn static inline void PA_SwitchScreens()
    \brief
         \~english Switch the bottom and top screens...
         \~french Echange les écrans du haut et du bas
*/
static inline void PA_SwitchScreens() {
	REG_POWERCNT ^= SWITCH_SCREENS; 
	PA_Screen = !PA_Screen;
}

extern vu8 _PA_LidDown;

/*! \def PA_LidClosed()
    \brief
         \~english Check if the DS is closed. Returns 0 if open, 1 if closed
         \~french Vérifie si la DS est fermée. Renvoie 0 si ouverte, 1 si fermée
*/
#define PA_LidClosed() _PA_LidDown

/*! \fn static inline void PA_SetAutoCheckLid(u8 on)
    \brief
         \~english Automatically check if the DS is closed in PA_WaitForVBL
         \~french Vérifie automatiquement si la DS est fermée dans PA_WaitForVBL
    \~\param on
         \~english 1 for on, 0 for off
         \~french 1 pour activer, 0 pour désactiver
*/
static inline void PA_SetAutoCheckLid(u8 on){
	pa_checklid = on;
}

static inline void PA_SetLedBlink(u8 blink, u8 speed);

/*! \fn u8 PA_CheckLid()
    \brief
         \~english Check if the DS is closed. If closed, it pauses the DS, and returns 1.
         \~french Vérifie si la DS est fermée. Si fermée, ca met en pause la DS et renvoie 1.
*/
u8 PA_CheckLid();

/*! \fn static inline void PA_WaitForVBL()
    \brief
         \~english Wait for the VBlank to occur
         \~french Attendre le vbl...
*/
static inline void PA_WaitForVBL(){
	if(pa_checklid) PA_CheckLid();
	swiWaitForVBlank();
}

#define PA_WaitForVBlank PA_WaitForVBL

/*! \def PA_CloseLidSound(close_sound)
    \brief
         \~english Check if the DS is closed. If closed, it pauses the DS, and plays a sound
         \~french Vérifie si la DS est fermée. Si fermée, ca met en pause la DS et joue un son
    \~\param close_sound
         \~english Sound to play, check the sounds doc if you're not sure what to do here
         \~french Son à jouer, regarder la doc son si pas certain de quoi mettre... 
*/
#define PA_CloseLidSound(close_sound)do{\
	if(PA_LidClosed()){\
		PA_PlaySimpleSound(close_sound);\
		PA_CheckLid(); \
	}}while(0)


/*! \def PA_CloseLidSound2(close_sound, open_sound)
    \brief
         \~english Check if the DS is closed. If closed, it pauses the DS, and plays a sound. The sound system must be initialized before.
         \~french Vérifie si la DS est fermée. Si fermée, ca met en pause la DS et joue un son
    \~\param close_sound
         \~english Sound to play when closes, check the sounds doc if you're not sure what to do here
         \~french Son à jouer quand se ferme, regarder la doc son si pas certain de quoi mettre...
    \~\param open_sound
         \~english Sound to play when opens, check the sounds doc if you're not sure what to do here
         \~french Son à jouer quand s'ouvre, regarder la doc son si pas certain de quoi mettre... 
*/
#define PA_CloseLidSound2(close_sound, open_sound)do{\
	if(PA_LidClosed()){\
		PA_PlaySimpleSound(close_sound);\
		PA_CheckLid(); \
		PA_PlaySimpleSound(open_sound); \
	}}while(0)

/*! \fn void PA_SetScreenLight(u8 screen, u8 light)
    \brief
         \~english Set on or off the screen's light
         \~french Allumer ou eteindre la lumière d'un écran
    \~\param screen
         \~english Screen...
         \~french Ecran...
    \~\param light
         \~english Light, 1 for on, 0 for off
         \~french Lumière, 1 pour allumé, 0 pour éteint
*/
static inline void PA_SetScreenLight(u8 screen, u8 light){
	// Beware of the nested one-line IF blocks!
	if(screen)
		if(light)
			powerOn(PM_BACKLIGHT_TOP);
		else
			powerOff(PM_BACKLIGHT_TOP);
	else
		if(light)
			powerOn(PM_BACKLIGHT_BOTTOM);
		else
			powerOff(PM_BACKLIGHT_BOTTOM);
}


/*! \fn static inline void PA_SetLedBlink(u8 blink, u8 speed)
    \brief
         \~english Set teh DS Led blinking
         \~french Régler le clignotement de la led
    \~\param blink
         \~english 1 for blinking, 0 for always on
         \~french 1 pour clignoter, 0 pour toujours allumé
    \~\param speed
         \~english Speed : 0 for slow, 1 for fast
         \~french Vitesse : 0 pour lent, 1 pour rapide
*/
static inline void PA_SetLedBlink(u8 blink, u8 speed){
	ledBlink(((blink&1) | ((speed&1) << 1)) << 4);
}

/*! \def PA_WaitFor(something)
    \brief
         \~english Wait for a specific thing to happen...
         \~french Attendre un événement précis...
    \~\param something
         \~english Thing to wait for, like Pad.Newpress.A, or Stylus.Newpress, etc...
         \~french Evénement à attendre, comme Pad.Newpress.A, ou Stylus.Newpress, etc...
*/

#define PA_WaitFor(something) do{while(!(something)) PA_WaitForVBL();}while(0)

/*! \fn static inline void PA_SetDSLBrightness(u8 level)
    \brief
         \~english Set the DS Lite Light level...
         \~french Régler le niveau de lumière de la DS Lite !
    \~\param level
         \~english Light level (0-3)
         \~french Niveau de la lumière (0-3)
*/
static inline void PA_SetDSLBrightness(u8 level){
	PA_FifoMsg msg;
	msg.type = PA_MSG_DSLBRIGHT;
	msg.DSLBrightMsg.brightness = level;
	PA_SendFifoMsg(msg);
}

/*! \fn bool PA_Locate(char *start, char *target, bool isDir, int depth, char *result)
    \brief
        Find a directory in the file system within a given depth
    \param start
        from which directory to start, use "/" to search from the root
	\param target
		what to look for: the name of a file or directory
	\param isDir
		look for a directory or a file?
	\param depth
		how much depth level (in number of directories) to traverse; limiting this speeds up the search on crowded cards. A reasonable value is, for example, 3.
	\param result
		pointer to a buffer where the result will be stored
	\return
		true if the target was found
*/
bool PA_Locate(char* start, char* target, bool isDir, int depth, char* result);

/// Displays an error message.
void PA_Error(const char* text);

/** @} */ // end of General

void PA_Nothing(void);

#ifdef __cplusplus
}
#endif
#endif
