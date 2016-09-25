#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <string.h>
typedef struct {
	struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
	int fbfd;
	char *front;
	long int screensize;
    char *back;
} session;



//#define COORDS_TO_LOC(s,x,y) (((x)+scr->vinfo.xoffset) * (scr->vinfo.bits_per_pixel/8) + ((y)+scr->vinfo.yoffset) * scr->finfo.line_length)

#define RGBA32_TO_BGR16(c) ( ((((c)>>3)&31) << 11) | ((((c)>>10)&63) << 5) | (((c)>>19)&31) )

/******************************************************
 *** This belongs in framebuffer.c ********************
 ******************************************************/
void destroy_fb(session *scr){
    munmap(scr->front, scr->screensize);
    close(scr->fbfd);
    free(scr->back);
    printf("\nUnmapped successfully\n");
}
void put_pixel32(session *scr,unsigned int x,unsigned int y,unsigned int rgba){
	long int location = (x+scr->vinfo.xoffset) * (scr->vinfo.bits_per_pixel/8) + (y+scr->vinfo.yoffset) * scr->finfo.line_length;
	*(scr->back + location) = (rgba>>16) & 0xff;		// Blue
	*(scr->back + location + 1) = (rgba>>8) & 0xff;	// Green
	*(scr->back + location + 2) =  rgba & 0xff; 		// Red
	*(scr->back + location + 3) = (rgba>>24) & 0xff;	// Alpha
}
void blit(session *scr){
    memcpy(scr->front,scr->back,scr->screensize);
}
void fill_screen(session *scr,unsigned int rgba){
    int x = 0, y = 0;
    long int location;
    // Figure out where in memory to put the pixel
    for (y = 0; y < scr->vinfo.yres; y++)
    for (x = 0; x < scr->vinfo.xres; x++) {

        if (scr->vinfo.bits_per_pixel == 32) {
            put_pixel32(scr,x,y,rgba);
        } else  { //assume 16bpp bgr565 // this is the cloudshell monitor
            location = (x+scr->vinfo.xoffset) * (scr->vinfo.bits_per_pixel/8) + (y+scr->vinfo.yoffset) * scr->finfo.line_length;
            *((unsigned short int*)(scr->back + location)) = RGBA32_TO_BGR16(rgba);
        }
    }

}

void init_fb(const char *device,session *scr){

    // Open the file for reading and writing
    scr->fbfd = open(device, O_RDWR);
    if (scr->fbfd == -1) {
        perror("Error: cannot open framebuffer device");
        exit(1);
    }
    printf("The framebuffer device was opened successfully.\n");

    // Get fixed screen information
    if (ioctl(scr->fbfd, FBIOGET_FSCREENINFO, &(scr->finfo)) == -1) {
        perror("Error reading fixed information");
        exit(2);
    }

    // Get variable screen information
    if (ioctl(scr->fbfd, FBIOGET_VSCREENINFO, &(scr->vinfo)) == -1) {
        perror("Error reading variable information");
        exit(3);
    }

    printf("%dx%d, %dbpp\n", scr->vinfo.xres, scr->vinfo.yres, scr->vinfo.bits_per_pixel);

    // Figure out the size of the screen in bytes
    scr->screensize = scr->vinfo.xres * scr->vinfo.yres * scr->vinfo.bits_per_pixel / 8;
    scr->back = malloc(scr->screensize);

    // Map the device to memory
    scr->front = (char *)mmap(0, scr->screensize, PROT_READ | PROT_WRITE, MAP_SHARED, scr->fbfd, 0);
    if ((long)scr->front == -1) {
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");
}
