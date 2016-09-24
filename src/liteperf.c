#include "liteperf.h"
#include <mruby.h>
#include <mruby/compile.h>

static int run=1;
static void exitHandler(int s){
	run = 0;
}
static void catchSignals (void){
    struct sigaction action;
    action.sa_handler = exitHandler;
    action.sa_flags = 0;
    sigemptyset (&action.sa_mask);
    sigaction (SIGINT, &action, NULL);
    sigaction (SIGTERM, &action, NULL);
}
mrb_value flip_method(mrb_state* mrb, mrb_value self){
    puts("Flip called");
    return mrb_nil_value();
}
void register_namespace(mrb_state *mrb){
    struct RClass *root_module;    
    root_module = mrb_define_module(mrb,"Liteperf");
    mrb_define_module_function(mrb,root_module,"flip",flip_method,MRB_ARGS_NONE());
    
}
static session screen;
int main(int argc,const char *argv[]){

    if(argc<3){
        printf("Usage: liteperf /dev/fb0 examples/cpu.rb\n");
        return 1;
    }
    //printf("Argc: %d \tA1: %s\tA2: %s\tA3: %s\n",argc,argv[0],argv[1],argv[2]);


    //init_fb(argv[1],&screen);


    catchSignals(); 
    // Initialize mruby
    mrb_state *mrb = mrb_open();
    if (!mrb) { }
    // Initialize the Liteperf class in mruby
    register_namespace(mrb);

    // Open and load the script provided as second argument.
    FILE *fp = fopen(argv[2],"r");
    mrb_value obj = mrb_load_file(mrb,fp);
    fclose(fp);

    // call some initialize method?
    mrb_int i = 99;
    mrb_funcall(mrb, obj, "draw", 1, mrb_fixnum_value(i));
    
    // Destroy the mruby runtime.
    mrb_close(mrb);

    //destroy_fb(&screen);
    return 0;

    /*while(run){

        int x = 0, y = 0;
	    x = 0; y = 0;       // Where we are going to put the pixel

	    // Figure out where in memory to put the pixel
	    for (y = 0; y < vinfo.yres; y++)
		for (x = 0; x < vinfo.xres; x++) {

		    location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
			       (y+vinfo.yoffset) * finfo.line_length;

		    if (vinfo.bits_per_pixel == 32) {
			*(fbp + location) = 100;        // Some blue
			*(fbp + location + 1) = 15+(x-100)/2;     // A little green
			*(fbp + location + 2) = 200-(y-100)/5;    // A lot of red
			*(fbp + location + 3) = 0;      // No transparency
		//location += 4;
		    } else  { //assume 16bpp // this is the cloudshell monitor
			int b = rand()%255;
			int g = (rand()%255)/6;     // A little green
			int r = 31-(rand()%200)/16;    // A lot of red
			unsigned short int t = r<<11 | g << 5 | b;
			//t = (0xff)<<11 | (0xff) << 5 | (0xff);
			*((unsigned short int*)(fbp + location)) = t;
		    }

	    }
    }*/
}
