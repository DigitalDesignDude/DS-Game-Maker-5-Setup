#ifndef _PA_SpecialFx
#define _PA_SpecialFx

#define PA_REG_MOSAIC(screen) _REG16(0x0400004C + (screen * 0x1000))
#define PA_REG_BLDCNT(screen) _REG16(0x04000050 + (screen * 0x1000))
#define PA_REG_BLDALPHA(screen) _REG16(0x04000052 + (screen * 0x1000))

#define SFX_BG0 1
#define SFX_BG1 2
#define SFX_BG2 4
#define SFX_BG3 8
#define SFX_OBJ 16
#define SFX_BD  32

#define SFX_NONE 0
#define SFX_ALPHA 1
#define SFX_BRIGHTINC 2
#define SFX_BRIGHTDEC 3

/*! \file PA_SpecialFx.h
    \brief Special effects options

   Special effects (alpha-blending, luminosity, mosaic effect...)
*/

/** @defgroup SpecialFx Special Effects
 *  Set the sprite special effects (alpha-blending, luminosity, mosaic effects...)
 *  @{
 */

/*! \def PA_EnableBgMosaic(screen, bg)
    \brief
         \~english Enable the mosaic effect for a given background
         \~french Activer l'effet de mosaic pour un fond donné
    \~\param screen
         \~english Background screen (0 or 1)
         \~french Ecran du bg (0 ou 1)
    \~\param bg
         \~english Background number
         \~french Numéro du fond
*/
#define PA_EnableBgMosaic(screen, bg) _REG16(REG_BGCNT(screen, bg)) |= (1 << 6)

/*! \def PA_DisableBgMosaic(screen, bg)
    \brief
         \~english Disable the mosaic effect for a given background
         \~french Désactiver l'effet de mosaic pour un fond donné
    \~\param screen
         \~english Background screen (0 or 1)
         \~french Ecran du bg (0 ou 1)
    \~\param bg
         \~english Background number
         \~french Numéro du fond
*/
#define PA_DisableBgMosaic(screen, bg) _REG16(REG_BGCNT(screen, bg)) &= ~(1 << 6)

/*! \def PA_SetBgMosaicXY(screen, h_size, v_size)
    \brief
         \~english Set the Mosaic parameters for the backgrounds
         \~french Régler les paramètres de la mosaic pour les fonds
    \~\param screen
         \~english Screen...
         \~french Ecran...
    \~\param h_size
         \~english Horizontal size of the mosaic (1 for 1 pixel, 2 for 2 pixels, etc...)
         \~french Taille horizontale de la mosaic (1 pour 1 pixel, 2 pour 2 pixels, etc...)
    \~\param v_size
         \~english Vertical size of the mosaic (1 for 1 pixel, 2 for 2 pixels, etc...)
         \~french Taille verticale de la mosaic (1 pour 1 pixel, 2 pour 2 pixels, etc...)
*/
#define PA_SetBgMosaicXY(screen, h_size, v_size) do{PA_REG_MOSAIC(screen) &= 255; PA_REG_MOSAIC(screen) |= ((h_size) + ((v_size) << 4));}while(0)


/*! \def PA_SetSpriteMosaicXY(screen, h_size, v_size)
    \brief
         \~english Set the Mosaic parameters for the sprites
         \~french Régler les paramètres de la mosaic pour les sprites
    \~\param screen
         \~english Screen...
         \~french Ecran...
    \~\param h_size
         \~english Horizontal size of the mosaic (1 for 1 pixel, 2 for 2 pixels, etc...)
         \~french Taille horizontale de la mosaic (1 pour 1 pixel, 2 pour 2 pixels, etc...)
    \~\param v_size
         \~english Vertical size of the mosaic (1 for 1 pixel, 2 for 2 pixels, etc...)
         \~french Taille verticale de la mosaic (1 pour 1 pixel, 2 pour 2 pixels, etc...)
*/
#define PA_SetSpriteMosaicXY(screen, h_size, v_size) do{PA_REG_MOSAIC(screen) &= (255 << 8); PA_REG_MOSAIC(screen) |= (((h_size) << 8) + ((v_size) << 12));}while(0)

/*! \def PA_EnableSpecialFx(screen, EffectType, FirstTarget, SecondTarget)
    \brief
         \~english Enable Special Effects and set whether backgrounds and sprites will use them or not. This also sets the type of Effect
         \~french Activer les Effets Speciaux et choisir si les fonds et sprites l'utiliseront ou pas. On choisit aussi au passage quel Effet utiliser  
	\~\param screen
         \~english Screen...
         \~french Ecran...
	\~\param EffectType
         \~english Effect Type. 0 for non, 1 for alpha-blending, 2 for brightness increase, and 3 for brightness decrease. You can use the macros SFX_NONE, SFX_ALPHA, SFX_BRIGHTINC, SFX_BRIGHTDEC
         \~french Type d'effet. 0 pour aucun, 1 pour transparence, 2 pour augmentation de la luminosité, et 3 pour diminution de celle-ci... On peut utiliser les macors SFX_NONE, SFX_ALPHA, SFX_BRIGHTINC, SFX_BRIGHTDEC
    \~\param FirstTarget
         \~english Backgrounds and sprites for which to activate the effect. Use the following macro : SFX_BG0 | SFX_BG1 | SFX_BG2 | SFX_BG3 | SFX_OBJ | SFX_BD (back drop)
         \~french Fond et sprites à afficher aevc l'effet spécial, que l'on choisi de la facon suivante : SFX_BG0 | SFX_BG1 | SFX_BG2 | SFX_BG3 | SFX_OBJ | SFX_BD (back drop)
    \~\param SecondTarget
         \~english Backgrounds and sprites to be seen behind the alpha-blending. Use the following macro : SFX_BG0 | SFX_BG1 | SFX_BG2 | SFX_BG3 | SFX_OBJ | SFX_BD (back drop)
         \~french Fond et sprites à afficher derrière la transparence, que l'on choisi de la facon suivante : SFX_BG0 | SFX_BG1 | SFX_BG2 | SFX_BG3 | SFX_OBJ | SFX_BD (back drop)
*/
#define PA_EnableSpecialFx(screen, EffectType, FirstTarget, SecondTarget) PA_REG_BLDCNT(screen) = ((FirstTarget) + ((SecondTarget) << 8) + ((EffectType) << 6))

/*! \def PA_DisableSpecialFx(screen)
    \brief
         \~english Disable Special Effects
         \~french Désactiver les Effets Speciaux 
	\~\param screen
         \~english Screen...
         \~french Ecran...
*/
#define PA_DisableSpecialFx(screen) PA_REG_BLDCNT(screen) = 0

/*! \def PA_SetSFXAlpha(screen, Coeff1, Coeff2)
    \brief
         \~english Set the special effect parameters for Alpha-Blending
         \~french Régler les paramètres pour l'Alpha-Blending
	\~\param screen
         \~english Screen...
         \~french Ecran...
	\~\param Coeff1
         \~english Coefficient for the first layer, from 0 to 31. Apparently, it's better to set between 0 and 16
         \~french Coefficient pour la première couche, de 0 à 31. A priori vaut mieux le mettre entre 0 et 16
	\~\param Coeff2
         \~english Coefficient for the second layer, from 0 to 31. Apparently, it's better to set between 0 and 16
         \~french Coefficient pour la deuxième couche, de 0 à 31. A priori vaut mieux le mettre entre 0 et 16
*/
#define PA_SetSFXAlpha(screen, Coeff1, Coeff2) PA_REG_BLDALPHA(screen) = (Coeff1) + ((Coeff2) << 8)

/** @} */ // end of SpecialFx

#endif
