#include <stdlib.h>

#include "gp32.h"
#define W            240
#define H            320

unsigned short *framebuffer;

static unsigned int *color;
static unsigned char *scr1;
static unsigned char *scr2;

static void init_colormap();
static void refresh ();
static void conv ();

int main() {

  int x;
  //u16 *framebuffer;
  framebuffer = (u16*) FRAMEBUFFER;
  gp_setCpuspeed(133); // 33 66 100 133 156
  gp_initFramebuffer(framebuffer,16,85);
  for (x=0;x<320*240;x++) framebuffer[x]=0x0000; // Clear Screen
 

                     
  /* Init Colormap */

  init_colormap();

  /* Init Scr */
  
  scr1 = (unsigned char *) calloc ( W*(H+4) , 1 );
   
  scr2 = (unsigned char *) calloc ( W*(H+4) , 1 );

  
  while (1) {
    
    refresh ();
    conv ();
  }

  /* Free */

  free (color);
  free (scr1);
  free (scr2);

}


static void conv () {

  int x;
  unsigned short *row;
  row = framebuffer+(320*240/2);
  unsigned char *scr_tmp = scr1;
  
  for (x=320*240/2;x<320*240;x++) {
     *row++ = ((color[scr_tmp[x]])>>3) ;
  }


}

static void refresh () {

  int bcl,tmp;
  unsigned char *src;
  unsigned char *dst;
  unsigned char *swp;

  for ( bcl=0 ; bcl<2*W ; bcl++ )
    *(scr2+W*H+bcl) = 56 + rand()%40;
  
  tmp = 60 + rand()%20;

  for ( bcl=0 ; bcl<tmp ; bcl++ )
    {
      dst = scr2 + W*(H+1) + rand()%(W-3);

      *dst =
        *(dst+1) =
        *(dst+2) =
        *(dst+W) =
        *(dst+W+1) =
        *(dst+W+2) =
        *(dst+2*W+1) =
        *(dst+2*W+2) =
        *(dst+2*W+3) = 149;
      }

  src = scr2 + 2*W;
  dst = scr1 + W;

  for ( bcl=0 ; bcl<W*(H+2)-2 ; bcl++ )
    {

      tmp =   *(src+W)
            + *(src+2*W-1)
            + *(src+2*W)
            + *(src+2*W+1);
     
      tmp >>= 2;
      
      if (tmp != 0)
        *dst++ = tmp-1;
      else
        *dst++ = 0;

      src++;
        
    }

  swp = scr1;
  scr1 = scr2;
  scr2 = swp;

}

static void init_colormap() {

  int i;
  int r,g,b;

  color = (unsigned int *) calloc ( 256 , 4 );

  for ( i=0 ; i<256 ; i++ )    
    {  
      r = g = b = 0 ;
      
      if ( (i > 31) && (i < 32) )
        r = 10 * ( i - 7 );
      if ( i > 31 )
        r = 255;
      
      if ( (i > 32 ) && (i < 57 ) )
        g = 10 * ( i - 32 );
      if ( i > 56 )
        g = 255;
      
      if ( i < 8 )
        b = 8 * i;
      if ( (i > 7) && (i < 17) )
        b = 8 * ( 16 - i );
      if ( (i > 57) && (i < 82) )
        b = 10 * ( i - 57 );
      if ( i > 81 )
        b = 255;
      
      color [i] = (r<<16) + (g<<8) + (b);
      
      //color [i] = (r%16<<11) + (g%32<<6) + (b%32<y1);
    }

}
