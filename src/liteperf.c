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

    // Destroy the mruby runtime.
    mrb_close(mrb);
    while(run){
      fill_screen(&screen,rand() & 0xffffffff);
      sleep(1);
    }
    destroy_fb(&screen);
    return 0;

}
