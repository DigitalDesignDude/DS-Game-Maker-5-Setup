/* SDK V0.90 Mirko Roller */

#ifndef GP32_H
#define GP32_H

#ifndef NULL
#define NULL 0
#endif

typedef unsigned char   u8;
typedef unsigned short  u16;
typedef unsigned long   u32;

typedef volatile unsigned char  vu8;
typedef volatile unsigned short vu16;
typedef volatile unsigned long  vu32;

typedef signed char     s8;
typedef signed short    s16;
typedef signed long     s32;

typedef unsigned char   byte;
typedef unsigned short  hword;
typedef unsigned long   word;

#define PI 3.14159
#define RADIAN(n)       (((float)n)/(float)180*PI)

#define BIT00 1
#define BIT01 2
#define BIT02 4
#define BIT03 8
#define BIT04 16
#define BIT05 32
#define BIT06 64
#define BIT07 128
#define BIT08 256
#define BIT09 512
#define BIT10 1024
#define BIT11 2048
#define BIT12 4096
#define BIT13 8192
#define BIT14 16384
#define BIT15 32768



#define BUTTON_LEFT     1
#define BUTTON_DOWN     2
#define BUTTON_RIGHT    4
#define BUTTON_UP       8
#define BUTTON_L       16
#define BUTTON_B       32
#define BUTTON_A       64
#define BUTTON_R      128
#define BUTTON_START  256
#define BUTTON_SELECT 512


/* PENDING BIT */
#define BIT_EINT0       (0x1)
#define BIT_EINT1       (0x1<<1)
#define BIT_EINT2       (0x1<<2)
#define BIT_EINT3       (0x1<<3)
#define BIT_EINT4       (0x1<<4)
#define BIT_EINT5       (0x1<<5)
#define BIT_EINT6       (0x1<<6)
#define BIT_EINT7       (0x1<<7)
#define BIT_TICK        (0x1<<8)
#define BIT_WDT         (0x1<<9)
#define BIT_TIMER0      (0x1<<10)
#define BIT_TIMER1      (0x1<<11)
#define BIT_TIMER2      (0x1<<12)
#define BIT_TIMER3      (0x1<<13)
#define BIT_TIMER4      (0x1<<14)
#define BIT_UERR01      (0x1<<15)
#define BIT_NOTUSED     (0x1<<16)
#define BIT_DMA0        (0x1<<17)
#define BIT_DMA1        (0x1<<18)
#define BIT_DMA2        (0x1<<19)
#define BIT_DMA3        (0x1<<20)
#define BIT_MMC         (0x1<<21)
#define BIT_SPI         (0x1<<22)
#define BIT_URXD0       (0x1<<23)
#define BIT_URXD1       (0x1<<24)
#define BIT_USBD        (0x1<<25)
#define BIT_USBH        (0x1<<26)
#define BIT_IIC         (0x1<<27)
#define BIT_UTXD0       (0x1<<28)
#define BIT_UTXD1       (0x1<<29)
#define BIT_RTC         (0x1<<30)
#define BIT_ADC         (0x1<<31)
#define BIT_ALLMSK      (0xffffffff)

#define GP32RGB(x,y,z)     ( (x) << 11 | (y) << 6 | (z) << 1 )
#define GP32RGBA(x,y,z,a)  ( (x) << 11 | (y) << 6 | (z) << 1 | (a) )
#define BITSET(a,b)   (a|= (1<<b))
#define BITCLEAR(a,b) (a&=~(1<<b))

#define LCD_WIDTH   320
#define LCD_HEIGHT  240

#include "gp32_registers.h"

/*  !!!!!!! MEMORY MAP OF GP32 !!!!!!!!

 // 0x00000000 - 0x0003ffff  512kb   system rom (BOOT_ROM)
 // 0x0c000000 - 0x0c7fffff  8mb     workram (WORK_RAM)
 // 0x0c780000 - 0x0c7fffff  512kb   used by system

    0x0C79C000             - MMU first-level translation table descriptors total 16KB
    0x0C7A0000             - MMU second-level translation table descriptors
    0x0C7AC000             - Internal IRQ table - pointers to IRQ handlers - total 32 entries
    0x0C7AC100             -
    0x0C7AC800             - Top of Undefined mode stack
    0x0C7ACC00             - Top of Abort mode stack
    0x0C7AD400             - Top of FIQ mode stack
    0x0C7AE800             - Top of IRQ mode stack
    0x0C7AFF00             - Top of Supervisor mode stack
    0x????????             - User mode stack is not defined??
    0x00000020             - Top of Reset mode stack

    0x0C7B0000             - FCLK set after GpClockSpeedChange
    0x0C7B0004             - HCLK   -""-
    0x0C7B0008             - PCLK   -""-
    0x0C7B004C             - Screen mode and state bits (see GPLCDINFO.U32_lcd)
                                 bits 0-7       : clk_value
                                 bits 8-15      : lcd_buf_count
                                 bits 16-23     : bpp - bit per pixel
                                 bits 24-32     : b_lcd_on - copy of ENVID (bit 0) bit from LCDCON1
    0x0c7B0050             - a ptr to GPDRAWSURFACE (set by GpSurfaceSet & GpSurfaceFlip)
    0x0C7B0054             - buffer_size (see GPLCDINFO.buffer_size)
    0x0C7B0058             - a ptr to screen bitmap 0 (see GPLCDINFO.buf_addr[0])
    0x0C7B005C             - a ptr to screen bitmap 1 (see GPLCDINFO.buf_addr[1])
    0x0C7B0060             - a ptr to screen bitmap 2 (see GPLCDINFO.buf_addr[2])
    0x0C7B0064             - a ptr to screen bitmap 3 (see GPLCDINFO.buf_addr[3])
    0x0C7B0398             - _timepassed

    0x0C7B0600             - USB buffer size (n*256 bytes, max 2KB)
    0x0C7B0700             - USB created - 0 = no, 0 != yes (USB packet size)

    0x0C7B0800             - m_palette (see GPLCDINFO.m_palette) (default palette)

    0x0C7B0C00->0x0C7B0CFF - Application path name
    0x0C7B0D00             - Application path name length

    0x0C7B0E00             -
    0x0C7B0E04             - Application code start - updated on each GpAppExecute() call
    0x0C7B0E08             - Application code end
    0x0C7B0E0C             - Application data start
    0x0C7B0E10             - Application bss end
    0x0C7B0E14             - Application bss start
    0x0C7B0E18             - Application bss end
    0x0C7B1400-0x0C7B1508  - GpSurfaceSet() or GpSurfaceFlip() code
    0x0C7B2800             - USB buffer area - 4Kb
    0x0C7B4000             - Bitmap memory area starts here
    0x0C7FF800             - Bitmap memory area ends here
*/


// #define SAMPLEBUFFER0  0x0C780000  // 114688 = 7*16384 Bytes till MMU page  0x0C79C000
// This is free memory if we are not using the official SDK  :)

// 4 kb for global Varibles
#define SDKmemory      0x0C797000    // 4kb  Global SDK Memory for Variables

// 16 kb free memory 0x0C798000 - 0x0C79C000
#define UNCACHED4KB0   0x0C798000    // 4kb, 16384 Bytes till MMU page
#define UNCACHED4KB1   0x0C799000    // 4kb
#define UNCACHED4KB2   0x0C79A000    // 4kb
#define UNCACHED4KB3   0x0C79B000    // 4kb

// All MMU stuff

// 16kb free memory for soundmixer 0x0C7B0000 - 0x0C7B4000
#define SAMPLEBUFFER0  0x0C7B0000    // 8kb  RINGBUFFER, 2 segments of 4096Bytes, 1920*2 Bytes used...
#define SAMPLEBUFFER1  0x0C7B2000    // 4kb  MODPLAYER_RENDERBUFFER
#define SAMPLEBUFFER2  0x0C7B3000    // 2kb  MODPLAYER_LEFT_CHANNEL
#define SAMPLEBUFFER3  0x0C7B3800    // 2kb  MODPLAYER_RIGHT_CHANNEL

#define FRAMEBUFFER    0x0C7B4000    // 153600
#define FRAMEBUFFER1   0x0C7B4000
#define FRAMEBUFFER2   0x0C7DA000    // 153600

#define GP24MBEXTENDEDRAM 0x0C800000  // 0x0C800000 to 0x0DFFFFFF if you have a 32MB ram update

#include <cpp_prototypes.h>
#endif
