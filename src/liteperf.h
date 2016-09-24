#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <signal.h>

typedef struct {
	struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long int location;
	int fbfd;
	char *fbp;
	long int screensize;
} session;


void destroy_fb(session *scr){
	munmap(scr->fbp, scr->screensize);
    close(scr->fbfd);
    printf("\nUnmapped successfully\n");
}

void init_fb(char *device,session *scr){

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

    // Map the device to memory
    scr->fbp = (char *)mmap(0, scr->screensize, PROT_READ | PROT_WRITE, MAP_SHARED, scr->fbfd, 0);
    if ((long)scr->fbp == -1) {
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");
}