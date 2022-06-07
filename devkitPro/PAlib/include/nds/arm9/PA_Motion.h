#ifndef PA_MOTIOn
#define PA_MOTIOn

#include <nds/arm9/ndsmotion.h>

/** @defgroup Motion DS Motion functions
 *  Easy enable and play around with your DS Motion !
 *  @{
 */

typedef struct {
	u8 Up, Down, Right, Left;
} pamotionpad;

typedef struct{
	s32 X, Y, Z, AccelX, AccelY, AccelZ, Zrot, Angle, Force;
	s32 Vx, Vy, Vz, Vzrot;
	pamotionpad Newpress, Held, Released;
} motion_struct;
/// Motion struct.
extern motion_struct Motion;

void PA_MotionVBL(void);

/// Turn on the accelerometer
static inline void PA_MotionInit(void) {
	motion_init();
	MotionVBL = PA_MotionVBL;
}

/// Checks whether a DS Motion Card is plugged in
static inline u8 PA_CheckDSMotion(){
    return motion_init();
}

extern u8 pamotiontopad;

/// Maps the DS Motion Card to the Pad structure (!!)
static inline void PA_MotionToPad(u8 enable){
	pamotiontopad = enable;
}

/** @} */ // end of Motion

#endif
