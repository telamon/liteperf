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
static session screen;

/******************************************************
 *** Mruby integration ********************************
 ******************************************************/

mrb_value flip_method(mrb_state* mrb, mrb_value self){
    blit(&screen);
    return mrb_nil_value();
}

mrb_value draw_image_method(mrb_state* mrb,mrb_value self){
    char *filename;    
    mrb_value opts;
    uint sx,sy,sw,sh,dx,dy;
    mrb_bool opts_set;

    mrb_get_args(mrb,"s?H!",filename, &opts_set, &opts);    
    if(opts_set){
        
    }
    
}

void register_namespace(mrb_state *mrb){
    struct RClass *root_module;    
    root_module = mrb_define_module(mrb,"Liteperf");
    mrb_define_module_function(mrb,root_module,"flip",flip_method,MRB_ARGS_NONE());
    mrb_define_module_function(mrb,root_module,"draw_image",flip_method,MRB_ARGS_ARG(1,1));

}

/******************************************************
 *** Entrypoint ***************************************
 ******************************************************/

int main(int argc,const char *argv[]){

    if(argc<3){
        printf("Usage: liteperf /dev/fb0 examples/cpu.rb\n");
        return 1;
    }
    //printf("Argc: %d \tA1: %s\tA2: %s\tA3: %s\n",argc,argv[0],argv[1],argv[2]);

    // Allocate the framebuffer.
    init_fb(argv[1],&screen);


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


    // Paint pretty colors.
    int k=0;
    while(run){
        fill_screen(&screen,rand() & 0xffffffff);

        draw_square(&screen,100+k,30,50,60+k,0x541212);
        k=(k+10)%50;
        blit(&screen);
        sleep(1);
    }
    // Destroy the mruby runtime.
    mrb_close(mrb);
    // Release the framebuffer.
    destroy_fb(&screen);
    return 0;

}
