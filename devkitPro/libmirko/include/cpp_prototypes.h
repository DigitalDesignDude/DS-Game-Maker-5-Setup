#ifndef GP32_CPLUSPLUS_H
#define GP32_CPLUSPLUS_H

#include <fileio.h>
#include <sbox.h>
#include <sprite.h>

#ifdef __cplusplus
extern "C" {
#endif
        #ifndef size_t
        #define size_t int
        #endif

        // gp_buttons.c
        void gp_initButton();
        u16  gp_getButton();

        // gp_cpuspeed.c
        int  gp_getPCLK();
        int  gp_getHCLK();
        void gp_setCpuspeed(int freq);
        void gp_Reset();

        // gp_grafik.c
        void  gp_drawPixel8  (int x, int y, u8 c, u8 *framebuffer);
        void  gp_drawPixel16 (int x, int y, u16 c, u16 *framebuffer);
        short gp_initFramebuffer(void *addr,u16 bitmode,u16 refreshrate);
        void gp_setFramebuffer (void *addr,int vsync);
        void  gp_setPalette (unsigned char pos,u16 color);

        // gp_arm.S
        void gp_clearFramebuffer16 (u16 *framebuffer, u16 color);
        void gp_setMMU(u32 mempos_start,u32 mempos_end,int mode);

        // gp_irq.S
        void gp_enableIRQ();
        void gp_disableIRQ();
        void gp_installSWIIRQ(int irqnumber,void *addr);
        void gp_removeSWIIRQ (int irqnumber,void *addr);

        // gp_memory.c
        int  gp_get32MBCHECK();
        void gp_waitDMA (u8 channel);
        void gp_setDMA  (u32 s, u32 d, u32 size,u8 channel );

        // gp_timer.c
        u32  gp_getRTC();
        void gp_initRTC();
        void gp_clearRTC();

        // gp_chatboard.c
        int gp_initChatboard();
        u8  gp_getChatboard();

        // gp_setfont.c
        void gp_drawString (int x,int y,int len,char *buffer,u16 color,void *framebuffer);

        // gp_sprite.c
        void gp_drawSpriteHTB( u16 *sprite, short put_x, short put_y, u16 *framebuffer, u16 trans,u8 alpha);
        void gp_drawSpriteHT ( u16 *sprite, short put_x, short put_y, u16 *framebuffer, u16 trans);
        void gp_drawSpriteH  ( u16 *sprite, short put_x, short put_y, u16 *framebuffer);
        void gp_drawSpriteT  ( u16 *sprite, short put_x, short put_y, u16 *framebuffer, u16 trans, u16 xsize, u16 ysize);
        void gp_drawSprite   ( u16 *sprite, short put_x, short put_y, u16 *framebuffer, u16 xsize, u16 ysize);

	// gp_clipped.c
	void gp_FastTransBlit(void *framebuffer, int dx, const int dy, const int width, const int height, const unsigned char *src, const int trans);
	void gp_FastSolidBlit(void *framebuffer, int dx, const int dy, const int width, const int height, const unsigned char *src, const int trans, const int coul);
	void gp_ZoomBlit(void *framebuffer, const int dx, const int dy, const int width, const int height, const unsigned char *src);
	void gp_ZoomTransBlit(void *framebuffer, const int dx, const int dy, const int width, const int height, const unsigned char *src, const int trans);
	void gp_ZoomSolidBlit(void *framebuffer, const int dx, const int dy, const int width, const int height, const unsigned char *src, const int trans, const int coul);
	void gp_ZoomTransBlitFlip(void *framebuffer, const int dx, const int dy, const int width, const int height, const unsigned char *src, const int trans, const int cote);
	void gp_ZoomSolidBlitFlip(void *framebuffer, const int dx, const int dy, const int width, const int height, const unsigned char *src, const int trans, const int coul, const int cote);
	void gp_SaveBitmap(void *framebuffer, int dx, const int dy, const int width, const int height, const unsigned char *dest);
	void gp_FastClear(void *framebuffer, int dx, int dy, int width, int height);

        // gp_line.c
        void gp_drawLine16(int x0, int y0, int x1, int y1, u16 color, u16 *framebuffer);

        // gp_tiled.c
        void gp_drawTiled16(u16 *sprite_data, u16 trans, u16 x_len, u16 plot_number, s16 plot_x, s16 plot_y, u16 *framebuffer);

        // gp_sound.c
        void gp_clearRingbuffer();
        void gp_initSound(int freq, int bit, int ringsize);
        void gp_addRingsegment(u16 *add_buffer);
	void gp_addSample(u16 *data, u16 freq, u16 stereo, u8 volume, u32 size);
	void gp_pauseModfile();
	void gp_resumeModfile();
	void gp_startModfile(unsigned char *mod);
	void gp_stopSoundmixer();
	void gp_startSoundmixer(int finetuning);

        // gp_filesys.c
        int smc_dir(char *dirname, DIR *dir_list );
        int smc_read(char *filename,void *dest,int offset,int size);
        int smc_write(char *filename,void *dest,int size);
        int smc_createdir(char *dirname);

        GPFILE *smc_fopen(const char *path, const char *mode);
        size_t  smc_fread(void *ptr, size_t size, size_t nmemb, GPFILE *stream);
        size_t  smc_fwrite(void *ptr, size_t size, size_t nmemb, GPFILE *stream);
        int     smc_fclose(GPFILE *stream);
        int     smc_fseek(GPFILE *stream, long offset, int whence);
        long    smc_ftell(GPFILE *stream);
        void    smc_rewind(GPFILE *stream);
        int     smc_filesize(GPFILE *stream);

        // gp_sbox.c
        int gp_drawSelectbox( SELECTBOX *sbox,u16 *framebuffer );

#ifdef __cplusplus
}
#endif

#endif

