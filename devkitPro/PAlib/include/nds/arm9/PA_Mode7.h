#ifndef PA_MODE7
#define PA_MODE7


#include "PA_BgRot.h"
//#include "PA_BgTiles.h"


// Mode 7 functions !!

void hbl_mode7(void);

extern const s32 DIV[160];


#define M7_D_SH        7 // focal shift
#define M7O_NORM       1 // object renormalization shift (by /2)

// view frustrum limits
#define M7_NEAR     16
#define M7_FAR     384
#define M7_LEFT   -120
#define M7_RIGHT   120
#define M7_TOP      80
#define M7_BOTTOM  -80

// the background uses a different far-plane than objects
#define M7_FAR_BG  512


typedef struct{
	s32 x, y, z;
}VECTOR;

typedef struct tagM7CAM{
	VECTOR pos;     
	int phi;        // yaw
	int theta;      // pitch
	VECTOR u;       // local x-axis (right)
	VECTOR v;       // local y-axis (up)
	VECTOR w;       // local z-axis (back)
} M7CAM;


typedef struct { 
	s16 pa, pb, pc, pd; s32 dx, dy; 
} BGAFF_EX;

// in mode7.c
extern M7CAM _m7_cam;
extern int _m7_horizon;
extern BGAFF_EX _m7_bgaff_ex[192];  // affine parameters for each line

#define X0    256<<8
#define Y0     32<<8  // Default camera height -> 8192
#define Z0    256<<8

// DRV
extern u8 PA_currentmode;

// === GLOBALS ========================================================
// DRV
extern s32 mode7cam_x, mode7cam_y, mode7cam_z, mode7alpha;
// DRV
extern s32 mode7cos, mode7sin;	// temporaries for cos and sin alpha
// DRV
extern s16 mode7_distance;

/** @defgroup Mode7 Mode 7 commands
 *  Different commands for Mode 7 :p A big thanks to TONC for these...
 *  @{
 */

/*! \fn void PA_InitMode7(u8 bg_select)
    \brief
         \~english Initialize Mode 7 for a given background. You MUST be in video mode 1 or 2
         \~french Initialise le Mode 7 pour un fond donné. Vous devez etre en mode 1 ou 2 impérativement !
    \~\param bg_select
         \~english Bg number, 2 in mode 1, 2 or 3 in mode 2
         \~french Numéro du fond. 2 en mode 1, 2 ou 3 en mode 2
*/
void PA_InitMode7(u8 bg_select);

/*! \fn static inline void PA_DeInitMode7(void)
    \brief
         \~english DeInitialize Mode 7
         \~french DesInitialise le Mode 7
*/
static inline void PA_DeInitMode7(){
	irqDisable(IRQ_HBLANK);
}


/*! \fn static inline void PA_Mode7Angle(s16 angle)
    \brief
         \~english Define the current angle.
         \~french Définir l'angle
    \~\param angle
         \~english The angle ranges from 0 to 511...
         \~french L'angle, qui va de 0 à 511...

*/
static inline void PA_Mode7Angle(s16 angle){
	mode7alpha = angle&511;
	mode7cos = PA_Cos(mode7alpha);
	mode7sin = PA_Sin(mode7alpha);
}



/*! \fn static inline void PA_Mode7MoveLeftRight(s16 x_deplac)
    \brief
         \~english Move lateraly, so left or right...
         \~french Se déplacer latéralement
    \~\param x_deplac
         \~english Number of pixels to move left or right
         \~french De combien de pixels se déplacer à gauche ou à droite

*/
static inline void PA_Mode7MoveLeftRight(s16 x_deplac){
	mode7cam_x += x_deplac*mode7cos;
	mode7cam_z += x_deplac*mode7sin;
}


/*! \fn static inline void PA_Mode7MoveForwardBack(s16 z_deplac)
    \brief
         \~english Move forward or backwards
         \~french Se déplacer latéralement
    \~\param z_deplac
         \~english Number of pixels to move forward or backwards
         \~french De combien se déplacer en avant ou en arrière

*/
static inline void PA_Mode7MoveForwardBack(s16 z_deplac){
	mode7cam_x += z_deplac*mode7sin;
	mode7cam_z -= z_deplac*mode7cos;
}



/*! \fn static inline void PA_Mode7X(s16 mode7x)
    \brief
         \~english Move to a given point on the map
         \~french Se déplacer en un point donné de la carte
    \~\param mode7x
         \~english X position on the map
         \~french Position X sur la carte

*/
static inline void PA_Mode7X(s16 mode7x){
	mode7cam_x = mode7x;
}


/*! \fn static inline void PA_Mode7Z(s16 mode7z)
    \brief
         \~english Move to a given point on the map
         \~french Se déplacer en un point donné de la carte
    \~\param mode7z
         \~english Z position on the map
         \~french Position Z sur la carte

*/
static inline void PA_Mode7Z(s16 mode7z){
	mode7cam_z = mode7z;
}

/*! \fn static inline void PA_Mode7SetPointXZ(s16 mode7x, s16 mode7z)
    \brief
         \~english Move to a given point on the map (of coordinates x, z)
         \~french Se déplacer en un point donné de la carte (de coordonnées x, z)
    \~\param mode7x
         \~english X position on the map
         \~french Position X sur la carte
    \~\param mode7z
         \~english Z position on the map
         \~french Position Z sur la carte
*/
static inline void PA_Mode7SetPointXZ(s16 mode7x, s16 mode7z){
	mode7cam_x = mode7x;
	mode7cam_z = mode7z;
}


/*! \fn static inline void PA_Mode7Height(s16 mode7y)
    \brief
         \~english Set the camera height.
         \~french Régler la hauteur de la caméra
    \~\param mode7y
         \~english Camera Height. By default, 8192. You can set this from 0 to 40 000 (or even more, but then it gets a little small...
         \~french Hauteur... Par défaut, elle est de 8192. On peut la mettre de 0 à 40 000 (ou beaucoup plus, mais après ca commence à faire petit...

*/
static inline void PA_Mode7Height(s16 mode7y){
	mode7cam_y = mode7y;
}

/** @} */ // end of Mode7

void m7_aff_calc();
void m7_hbl_flr();
void m7_horz_calc();

#endif
