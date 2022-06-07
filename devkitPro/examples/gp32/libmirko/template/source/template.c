#include <stdio.h>
#include "gp32.h"

unsigned short *framebuffer;

int main() {

  int refreshrate;
  framebuffer = (u16*) FRAMEBUFFER;
  gp_setCpuspeed(33);
  refreshrate = gp_initFramebuffer(framebuffer,16,80);
    
  gp_clearFramebuffer16(framebuffer,0xFFFF);
  
    gp_drawString(10,100,12,"Hello World!",0xf800,framebuffer);

  while (1) { if ( gp_getButton()&BUTTON_A) gp_Reset();  }
  
}


