// written 2004 Mirko Roller   mirko@mirkoroller.de
// Example shows you how to print a string to the screen
#include <stdio.h>
#include "gp32.h"

unsigned short *framebuffer;

int main() {

  int refreshrate;
  framebuffer = (u16*) FRAMEBUFFER;
  gp_setCpuspeed(33);
  refreshrate = gp_initFramebuffer(framebuffer,16,80);
    
  gp_clearFramebuffer16(framebuffer,0xFFFF);
  
  { char text[]="Press A to reset your GP32. ";
    int xx=10;
    int yy=100;
    int len=26;
    int color=0xF800; // RGB ( RRRRRGGGGGBBBBB0)
    gp_drawString(xx,yy,len,text,color,framebuffer);
  }

  { char text[80];
    int xx=10;
    int yy=130;
    int len=23;
    int color=0xF800>>5; // RGB ( RRRRRGGGGGBBBBB0)
    sprintf (text,"LCD refreshrate =%dHz   ",refreshrate);
    gp_drawString(xx,yy,len,text,color,framebuffer);
  }


  while (1) { if ( gp_getButton()&BUTTON_A) gp_Reset();  }
  
}


