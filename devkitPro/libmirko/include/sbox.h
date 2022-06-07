#ifndef SBOX_H
#define SBOX_H

typedef struct {
  char banner[80];     // Banner text of the SelectBox
  char text[128][32];  // Space for 128 Entrys of 32 Bytes
  int  entrys;         // entrys in text field
  u16  bannercolor;    // 16Bit Truecolor color
  u16  textcolor;      // 16Bit Truecolor color
  u16  boxcolor;       // 16Bit Truecolor color
  int  x;              // x pos on screen
  int  y;              // y pos on screen
} SELECTBOX;

#endif

