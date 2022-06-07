// This file is written/copyright  Mirko Roller
#include <math.h>
#include <stdlib.h>

#include "gp32.h"

#define STARS 300  //How many stars to render ?

int i;
u16 *framebuffer = (u16*)FRAMEBUFFER;
static u16 colors[256];
static int xoff=320/2, yoff=240/2;
static double angle = 0.0;
static int  stars[STARS][3];
static int starsr[STARS][3];
static int    old[STARS][3];

void transform(int *ta, int *tb) {
        double y = 0.0, z = 0.0;
        y = (cos((angle / 20)) * *ta) - (sin((angle / 20)) * *tb);
        z = (sin((angle / 20)) * *ta) + (cos((angle / 20)) * *tb);
        *ta = (int) (y);
        *tb = (int) (z);
}

void initstars(void) {
        for (i = 0; i < STARS; i++) {
            stars[i][0] = ((rand() % 320)+1 - 160) *2;
            stars[i][1] = ((rand() % 320)+1 - 160) *2;
            stars[i][2] = ((rand() % 320)+1 - 160) *2;
        }
}

void initcolor() {
        for (i = 0; i < 255; i++) colors[i] = GP32RGB(i/8,i/8,i/8);
}

void rotate() {
        int x, y, oldx, oldy;
        unsigned char color = 0;

        while(1) {
             for (i = 0; i < STARS; i++) {
                        old[i][0] = starsr[i][0];
                        old[i][1] = starsr[i][1];
                        old[i][2] = starsr[i][2];
        
                        starsr[i][0] = stars[i][0];
                        starsr[i][1] = stars[i][1];
                        starsr[i][2] = stars[i][2];
        
                        transform(&starsr[i][1], &starsr[i][2]);
                        transform(&starsr[i][0], &starsr[i][2]);
                        transform(&starsr[i][0], &starsr[i][1]);

                        oldx = (int) (((256*old[i][0])/(old[i][2]-1024))+xoff);
                        oldy = (int) (((256*old[i][1])/(old[i][2]-1024))+yoff);

                        x = (int) (((256*starsr[i][0])/(starsr[i][2]-1024))+xoff);
                        y = (int) (((256*starsr[i][1])/(starsr[i][2]-1024))+yoff);
                
                        color = ((starsr[i][2] + 721) / 5);

                        gp_drawPixel16 ( (oldx+0)%320, (oldy+0)%240, 0x0000 ,framebuffer );
                        gp_drawPixel16 ( (oldx+1)%320, (oldy+0)%240, 0x0000 ,framebuffer );
                        gp_drawPixel16 ( (oldx+0)%320, (oldy+1)%240, 0x0000 ,framebuffer );
                        gp_drawPixel16 ( (oldx+1)%320, (oldy+1)%240, 0x0000 ,framebuffer );
                        
                        gp_drawPixel16 ( (x+0)%320, (y+0)%240, colors[color] ,framebuffer );
                        gp_drawPixel16 ( (x+1)%320, (y+0)%240, colors[color] ,framebuffer );
                        gp_drawPixel16 ( (x+0)%320, (y+1)%240, colors[color] ,framebuffer );
                        gp_drawPixel16 ( (x+1)%320, (y+1)%240, colors[color] ,framebuffer );
             }
             angle += 0.4;
        }

}

int main() {
   gp_setCpuspeed(156);
   gp_initFramebuffer(framebuffer,16,85);
   gp_clearFramebuffer16 (framebuffer ,0x0000);
   initcolor();
   initstars();
   rotate();
	return 0;
}
