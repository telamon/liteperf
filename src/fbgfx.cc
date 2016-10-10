#include <stdlib.h>  
#include <stdio.h>  

#include "fbgfx.h"


Fbgfx::Fbgfx(const char *device){
   // Open the file for reading and writing
    this->fbfd = open(device, O_RDWR);
    if (this->fbfd == -1) {
        perror("Error: cannot open framebuffer device");
        exit(1);
    }
    printf("The framebuffer device was opened successfully.\n");

    // Get fixed screen information
    if (ioctl(this->fbfd, FBIOGET_FSCREENINFO, &(this->finfo)) == -1) {
        perror("Error reading fixed information");
        exit(2);
    }

    // Get variable screen information
    if (ioctl(this->fbfd, FBIOGET_VSCREENINFO, &(this->vinfo)) == -1) {
        perror("Error reading variable information");
        exit(3);
    }

    printf("%dx%d, %dbpp\n", this->vinfo.xres, this->vinfo.yres, this->vinfo.bits_per_pixel);

    // Figure out the size of the screen in bytes
    this->screensize = this->vinfo.xres * this->vinfo.yres * this->vinfo.bits_per_pixel / 8;
    this->back = malloc(this->screensize);

    // Map the device to memory
    this->front = (char *)mmap(0, this->screensize, PROT_READ | PROT_WRITE, MAP_SHARED, this->fbfd, 0);
    if ((long)this->front == -1) {
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");
}

Fbgfx::~Fbgfx(){
	munmap(this->front, this->screensize);
    close(this->fbfd);
    free(this->back);
    printf("\nUnmapped successfully\n");
}
    