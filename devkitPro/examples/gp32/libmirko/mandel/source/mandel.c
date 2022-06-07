#include "gp32.h"

unsigned short *framebuffer;
unsigned short *screen;

void mandelbrot(double xmin,double xmax,double ymin,double ymax,int ill);

int main() {
  double z;
  int x,y;
  double xmin=-2.5,xmax=2.0,ymin=-2.2,ymax=2.2;
    
  framebuffer = (u16*) FRAMEBUFFER;
  gp_setCpuspeed(133);
  gp_initFramebuffer(framebuffer,16,85);

  for (x=0;x<320*240;x++) framebuffer[x]=0xFFFF; // Clear Screen
  screen=framebuffer;
  
  //gp_SetCpuSpeed(156);
  mandelbrot(xmin,xmax,ymin,ymax,64);
  //gp_SetCpuSpeed(33);
  
  x=10;y=10;
  while (1) {
     short backup;
     backup = screen[x+240*y];
     screen[x+240*y]=0xFFFF;

     for (z=0;z<5000;z++) z=z;
     screen[x+240*y]=backup;
     
     if ( gp_getButton()&BUTTON_RIGHT) y+=1;
     if ( gp_getButton()&BUTTON_LEFT ) y-=1;
     if ( gp_getButton()&BUTTON_UP   ) x+=1;
     if ( gp_getButton()&BUTTON_DOWN ) x-=1;
     if (x>239) x=239;
     if (x<1)   x=1;
     if (y>319) y=319;
     if (y<1)   y=1;

  }


while (1) { }
}




void mandelbrot(double xmin,double xmax,double ymin,double ymax,int ill) {
      int k,pos;
      double divx=(xmax- xmin)/240;
      double divy=(ymax- ymin)/320;
      double x,y,temp,i,j;

      for (i=xmin;i<=xmax;i=i+divx)
         for (j=ymin;j<=ymax;j=j+divy) {
             x=0;
             y=0;
             k=0;
             while ((x*x+ y*y)< 4&&k<=ill) {
                 temp=x;
                 x=x*x- y*y+ i;
                 y=2*temp*y+ j;
                 k++ ;
             }
             pos = ((i- xmin)/divx) + (((j- ymin)/divy)*240);
             if ( k < ill )  {
                 screen[pos] = (2+k*128);
             }
             else    screen[pos] =0;

         }
}
 

