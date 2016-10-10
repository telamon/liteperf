#ifndef __FBGFX_H_INCLUDED__
#define __FBGFX_H_INCLUDED__

#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <string.h>

typedef unsigned char uint8_t;

typedef struct
{
    uint8_t* font;
    uint8_t x_size;
    uint8_t y_size;
    uint8_t offset;
    uint8_t numchars;
} font_info;



// 565 format.
#define RGBA32_TO_BGR16(c) ( ((((c)>>3)&31) << 11) | ((((c)>>10)&63) << 5) | (((c)>>19)&31) )
#define RGBA32_TO_RGB16(c) ( ((((c)>>19)&31) << 11) | ((((c)>>10)&63) << 5) | (((c)>>3)&31) )


class Fbgfx {
public:
	struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
	int fbfd;
	void *front;
	long int screensize;
    void *back;
    unsigned int fg_color;
    unsigned int bg_color;

	Fbgfx(const char *device);	
	~Fbgfx();
	void blit();
	void put_pixel32(unsigned int x, unsigned int y, unsigned int rgba);
	void fill_screen(unsigned int rgba);
	void clear_screen();
	void load_font(unsigned char* font);
	void draw_char(unsigned char c,int x, int y);
	void draw_text(const unsigned char* string,int x, int y);
	void draw_rect(unsigned int ox,unsigned int oy,unsigned int w,unsigned int h,unsigned int rgba);


private:
	void put_pixel32(unsigned int x,unsigned int y,unsigned int rgba);
	void put_pixel16(unsigned int x,unsigned int y,unsigned int rgba);	
};


#endif

