// Small GP32 demo by Torlus (http://torlus.com)
// using WinterMute's DevkitARM and Mirko's SDK (http://www.mirkoroller.de/)

#include <stdlib.h>
#include <gp32.h>

#include "spriteBall_bin.h"

#define BALLS 5
typedef struct {
	int x,y,dx,dy;
} Ball;
Ball balls[BALLS];

void swapBuffers();

u16 *frameBuffer = (u16 *)FRAMEBUFFER1;

int main() {
	int i;
	Ball *ball;
	
	gp_setCpuspeed(133);
	gp_initFramebuffer(frameBuffer,16,80);

	for(i=0; i<BALLS; i++) {
		ball = &balls[i];
		ball->x = (rand() % (320-64));
		ball->y = (rand() % (240-64));
		ball->dx = (rand() & 1)*2-1; /* -1 or 1 */
		ball->dy = (rand() & 1)*2-1;  
	}

	while(1) {	
		gp_clearFramebuffer16(frameBuffer,0xFFFE);

		for(i=0; i<BALLS; i++) {
			ball = &balls[i];
			ball->x += ball->dx;
			ball->y += ball->dy;
			if ( (ball->x < 0) || (ball->x > (320-64)) ) ball->dx = -ball->dx;
			if ( (ball->y < 0) || (ball->y > (240-64)) ) ball->dy = -ball->dy;
			gp_drawSpriteHTB((u16 *)spriteBall_bin,ball->x,ball->y,frameBuffer,0xFFFE,24);
		}
		
		gp_drawString(90,220,16,"Press A to reset",0x0000,frameBuffer);
		
		swapBuffers();
		if ( gp_getButton()&BUTTON_A) gp_Reset();		
	}
	return 0;
}

void swapBuffers() {
	if (frameBuffer == (u16*)FRAMEBUFFER1) {
		gp_setFramebuffer((u16 *)FRAMEBUFFER1,1);
		frameBuffer = (u16 *)FRAMEBUFFER2;
	} else {
		gp_setFramebuffer((u16 *)FRAMEBUFFER2,1);
		frameBuffer = (u16 *)FRAMEBUFFER1;
	}
}

