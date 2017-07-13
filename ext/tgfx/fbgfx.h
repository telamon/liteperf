#ifndef __FBGFX_H_INCLUDED__
#define __FBGFX_H_INCLUDED__

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <string.h>




typedef struct {
	struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
	int fbfd;
	char *front;
	long int screensize;
    char *back;
    unsigned int fg_color;
    unsigned int bg_color;
} session;

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

// Gfx
void init_fb(const char *device,session *scr);
void destroy_fb(session *scr);
void blit(session *scr);
font_info* create_font_info(unsigned char* font);
void draw_char(session *scr,font_info* cfont,unsigned char c,int x, int y);
void draw_text(session* scr,font_info* cfont,const unsigned char* string,int x, int y);
void draw_rect(session *scr,unsigned int ox,unsigned int oy,unsigned int w,unsigned int h,unsigned int rgba);
void fill_screen(session* scr,unsigned int rgba);
void clear_screen(session* scr);

void put_pixel32(session *scr,unsigned int x,unsigned int y,unsigned int rgba);

#define put_pixel16(scr,x,y,color) \
( \
    *((unsigned short int*)((scr)->back + ( (x) * ( (scr)->vinfo.bits_per_pixel/8) + (y) * (scr)->finfo.line_length) )) \
    = RGBA32_TO_RGB16(color) \
)

#define put_pixel(scr,x,y,color) \
( \
   (x)+(y)*(scr)->vinfo.xres >=0 && (x)+(y)*(scr)->vinfo.xres < (scr)->vinfo.xres * (scr)->vinfo.yres ? ( \
    (scr)->vinfo.bits_per_pixel==32 ? put_pixel32((scr),(x),(y),(color)) : put_pixel16((scr),(x),(y),(color)) \
    ) : 0 \
)

#endif