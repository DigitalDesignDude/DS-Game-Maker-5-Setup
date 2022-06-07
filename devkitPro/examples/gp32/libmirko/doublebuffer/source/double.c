// written 2004 Mirko Roller   mirko@mirkoroller.de
// Example shows you how to set up a double Buffer

#include "gp32.h"

u16 *framebuffer1;
u16 *framebuffer2;

int main() {

  int i;
  framebuffer1 = (u16*) FRAMEBUFFER1;  // 0x0C7B4000
  framebuffer2 = (u16*) FRAMEBUFFER2;  // 0x0C7DA000

  gp_setCpuspeed(33);
  gp_initFramebuffer(framebuffer1,16,85);
  
  gp_clearFramebuffer16(framebuffer1,0xFFFF);  // Clear Framebuffer1
  gp_clearFramebuffer16(framebuffer2,0xFFFF);  // Clear Framebuffer2
 

 while (1) {

  gp_setFramebuffer(framebuffer1,1);                            // Display of Screen 1
  gp_clearFramebuffer16(framebuffer2,0xFFFF);                   // Clear Framebuffer2
  gp_drawString(30,110,13,"Iam Screen 2 ",0xF800,framebuffer2);   // Painting on Screen 2

  for (i=0;i<1600000;i++);
  gp_setFramebuffer(framebuffer2,1);                            // Display of Screen 2
  gp_clearFramebuffer16(framebuffer1,0xFFFF);                   // Clear Framebuffer1
  gp_drawString(30,100,13,"Iam Screen 1 ",0xF800,framebuffer1);   // Painting on Screen 1
  for (i=0;i<1600000;i++);
  
  if ( gp_getButton()&BUTTON_A) gp_Reset();
 }
  
}

