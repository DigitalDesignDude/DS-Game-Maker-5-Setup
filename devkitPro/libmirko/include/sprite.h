#ifndef GP32SPRITE_H
#define GP32SPRITE_H


typedef struct {
   char magic[4];  // "MR.M"
   u16 size_x;     // x size of sprite
   u16 size_y;     // y size of sprite
   u16 reserved1;  // User usable
   u16 reserved2;  // User usable
} SHEADER;


#endif
