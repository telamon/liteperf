#include "fbgfx.h"

//#define COORDS_TO_LOC(s,x,y) (((x)+scr->vinfo.xoffset) * (scr->vinfo.bits_per_pixel/8) + ((y)+scr->vinfo.yoffset) * scr->finfo.line_length)

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


void destroy_fb(session *scr){
    munmap(scr->front, scr->screensize);
    close(scr->fbfd);
    free(scr->back);
    printf("\nUnmapped successfully\n");
}
void blit(session *scr){
    memcpy(scr->front,scr->back,scr->screensize);
}
void put_pixel32(session *scr,unsigned int x,unsigned int y,unsigned int rgba){
	long int location = (x+scr->vinfo.xoffset) * (scr->vinfo.bits_per_pixel/8) + (y+scr->vinfo.yoffset) * scr->finfo.line_length;
	*(scr->back + location) = (rgba>>16) & 0xff;		// Blue
	*(scr->back + location + 1) = (rgba>>8) & 0xff;	// Green
	*(scr->back + location + 2) =  rgba & 0xff; 		// Red
	*(scr->back + location + 3) = (rgba>>24) & 0xff;	// Alpha
}

#define put_pixel16(scr,x,y,color) \
( \
    *((unsigned short int*)((scr)->back + ( (x) * ( (scr)->vinfo.bits_per_pixel/8) + (y) * (scr)->finfo.line_length) )) \
    = RGBA32_TO_RGB16(color) \
)

#define put_pixel(scr,x,y,color) \
( \
    (scr)->vinfo.bits_per_pixel==32 ? put_pixel32((scr),(x),(y),(color)) : put_pixel16((scr),(x),(y),(color)) \
)

font_info* create_font_info(unsigned char* font){
    font_info* cfont = (font_info*) malloc(sizeof(font_info));
    cfont->font=font;
    cfont->x_size = font[0];
    cfont->y_size = font[1];
    cfont->offset = font[2];
    cfont->numchars = font[3];
    return cfont;
}

void draw_char(session *scr,font_info* cfont,unsigned char c,int x, int y){
    /* I have completley no clue how this method works; borrowed it from UTFT but was too lazy
    to try and figure it out, got it working with some minor adjustments and redundant -8's
    */
    unsigned char i,ch;
    int j;
    int temp; 
    int bwidth=cfont->x_size/8;

    temp= ((c-cfont->offset) * (bwidth*cfont->y_size)) + 4;

    for(j=0 ; j < (bwidth*cfont->y_size) ; j += bwidth){
        int x1= x;
        int y1= y + ( j / bwidth );
        int x2= x + cfont->x_size-1;
        int y2= y + ( j / bwidth );
        
        for( int zz = bwidth - 1 ; zz >= 0 ; zz-- ){
            ch = *( &cfont->font[temp+zz] );

            for( i = 0 ; i < 8 ; i++ ){   
                if( (ch & (1<<i) ) !=0 ){
                    put_pixel(scr,x1+x2-i+zz*8-8,y1,scr->fg_color); // draw frontcolor
                }else{
                    put_pixel(scr,x1+x2-i+zz*8-8,y1,scr->bg_color); // draw back-color
                } 
            }
        
        }
        temp+= bwidth;
        
    }
    //clrXY();

}
void draw_text(session* scr,font_info* cfont,const unsigned char* string,int x, int y){
    int oy=0;
    int ox=0;
    for(int i=0; i< strlen(string); i++){
        ox+=cfont->x_size>>1;
        if(string[i]=='\n'){
            ox=0;
            oy+=cfont->y_size;
        }else{
            draw_char(scr,cfont,string[i],x+ox,y+oy);
        }
    }
}

void draw_rect(session *scr,unsigned int ox,unsigned int oy,unsigned int w,unsigned int h,unsigned int rgba){
    for(int y = oy;y<h+oy;y++)
    for(int x = ox;x<w+ox;x++){
        put_pixel(scr,x,y,rgba);
    };
}

void fill_screen(session* scr,unsigned int rgba){
    int x = 0, y = 0;
    
    // Figure out where in memory to put the pixel
    for (y = 0; y < scr->vinfo.yres; y++)
    for (x = 0; x < scr->vinfo.xres; x++) {
        put_pixel(scr,x,y,rgba);
    }

}
void clear_screen(session* scr){
    fill_screen(scr,scr->bg_color);
}

