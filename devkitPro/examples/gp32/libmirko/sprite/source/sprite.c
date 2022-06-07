// written 2004 Mirko Roller   mirko@mirkoroller.de
// Example shows you how to use sprites, and clear the screen...
// The sprites are generated with: bmp2raw -x name.bmp name.raw

#include <stdio.h>
#include "gp32.h"

#include "garfield_bin.h"
#include "odie_bin.h"
#include "test_bin.h"

u16 *framebuffer[2];

int swapper=0;
void swap_screen () {
   gp_setFramebuffer(framebuffer[swapper],1);
   swapper++; if (swapper == 2) swapper=0;
}

int main() {

  int spritex=-150;
  int x;
  int framerate=0;
  int refreshrate=0;
  char buffer[64]="Framerate:         ";
  framebuffer[0] = (u16*)  FRAMEBUFFER1;
  framebuffer[1] = (u16*)  FRAMEBUFFER2;

  gp_setCpuspeed(133);
  
  gp_initRTC();
  refreshrate=gp_initFramebuffer(framebuffer[0],16,85);
  gp_clearRTC();
  
 while (1) {

  // Drawing in Sprite header mode, only usefull if the sprite got a header.
  // Use it only, if your Sprite is converted with ./bmp2raw -x
  // You can use:  gp_drawSpriteT,gp_drawSprite  if your sprite data, are in raw data format.

  swap_screen();
    gp_clearFramebuffer16(framebuffer[swapper],0xffff); // very very fast asm, faster than memset
    for (x=200;x<320;x+=4) gp_drawLine16 (319,239,x,  0,0xf800,framebuffer[swapper]);
    gp_drawSpriteH  ( (u16*)test_bin,100, 10, framebuffer[swapper]);
    gp_drawSpriteHT ( (u16*)garfield_bin, 90, 50, framebuffer[swapper], 0xFFFE );
    gp_drawSpriteHTB( (u16*)odie_bin,   spritex, spritex , framebuffer[swapper], 0xFFFE,15);

	spritex++;


  framerate++;
  if (gp_getRTC() > 63) {sprintf(buffer,"Framerate: %d,%d    ",framerate,refreshrate);framerate=0;gp_clearRTC(); }
  gp_drawString(20,220,20,buffer,0x0000,framebuffer[swapper]);

  if (spritex>300) spritex=-150;
  if (gp_getButton()&BUTTON_A) gp_Reset();
 }
  
}

