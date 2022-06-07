// written 2004 Mirko Roller   mirko@mirkoroller.de
// Example shows you how to setup 256 color mode

#include <gp32.h>

u8  *framebuffer;
u16 palette[256];

int main() {

	int x,y;
	unsigned char color;
  
	framebuffer = (u8*) FRAMEBUFFER;   // 0x0C7B4000

	gp_setCpuspeed(40);
	gp_initFramebuffer(framebuffer,8,85); 
 
	// Palette format: %RRRRRGGGGGBBBBBI     (5551) 
	// I : Intensity Bit 1=on 0=off

	for (x=0;x<32;x++)    palette[x] =(x<<1);
	for (x=32;x<64;x++)   palette[x] =((63-x)<<1);
	for (x=64;x<256;x++)  palette[x] =0x0;

	// Register (0-255) 255 looks like some kind of contrast register

	color = 0;

	for (y=0;y<240;y++)
		for (x=0;x<320;x++) gp_drawPixel8 (x, y, (color++)%255, framebuffer);

	gp_drawString(50,110,23,"Text in 8Bit mode....  ",255,framebuffer);

	while (1) { // Rotate palette
		u16 save;    
		for (x=0;x<255;x++) gp_setPalette(x,palette[x]);
		save=palette[0];
		for (x=0;x<255;x++) palette[x]=palette[x+1]; palette[255]=save;

		for (x=0;x<200000;x++) x=x;
	}

	while (1);
}

