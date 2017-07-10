#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>



#include "fbgfx.h"
#include "arial_bold.c"

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
static font_info* font;

/******************************************************
 *** Mruby integration ********************************
 ******************************************************/
/*
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
mrb_value clear_screen_method(mrb_state* mrb,mrb_value self){
    clear_screen(&screen);
    return mrb_nil_value();
}

mrb_value draw_rect_method(mrb_state* mrb,mrb_value self){
    mrb_int x,y,w,h,color;
    mrb_bool color_set=FALSE;
    mrb_get_args(mrb,"iiiii?",&x,&y,&w,&h,&color,&color_set);
    if(!color_set)
        color= screen.fg_color;

    draw_rect(&screen,x,y,w,h,color);
    return mrb_nil_value();
}
mrb_value draw_text_method(mrb_state* mrb,mrb_value self){
    char* text;
    mrb_int x,y,color;    
    mrb_bool color_set=FALSE;

    mrb_get_args(mrb,"zii",&text,&x,&y);
    draw_text(&screen,font,text,x,y);
    return mrb_nil_value();
}
mrb_value get_fgcolor_method(mrb_state* mrb,mrb_value self){
    return mrb_fixnum_value(screen.fg_color);
}
mrb_value get_bgcolor_method(mrb_state* mrb,mrb_value self){
    printf("bg_getter called\n");
    return mrb_fixnum_value(screen.bg_color);
}
mrb_value set_fgcolor_method(mrb_state* mrb,mrb_value self){
    mrb_int color;
    mrb_get_args(mrb,"i",color);

    screen.fg_color = color;
    return mrb_fixnum_value(color);
}
mrb_value set_bgcolor_method(mrb_state* mrb,mrb_value self){
    mrb_int color;
    mrb_get_args(mrb,"i",&color);
    printf("set_bgcolor_method  %08x\n",color);
    screen.bg_color=color;
    return mrb_fixnum_value(color);
}
void initialize_mrb_namespace(mrb_state *mrb){
    struct RClass *root_module;    
    root_module = mrb_define_module(mrb,"Liteperf");
    mrb_define_module_function(mrb,root_module,"wait_for_vsync",flip_method,MRB_ARGS_NONE());
    mrb_define_module_function(mrb,root_module,"clear_screen",clear_screen_method,MRB_ARGS_NONE());
    mrb_define_module_function(mrb,root_module,"draw_rect",draw_rect_method,MRB_ARGS_ARG(4,1));
    mrb_define_module_function(mrb,root_module,"draw_text",draw_text_method,MRB_ARGS_REQ(3));
    mrb_define_module_function(mrb,root_module,"foreground_color=",set_fgcolor_method,MRB_ARGS_REQ(1));
    mrb_define_module_function(mrb,root_module,"background_color=",set_bgcolor_method,MRB_ARGS_REQ(1));
    mrb_define_module_function(mrb,root_module,"background_color",get_bgcolor_method,MRB_ARGS_NONE());
    mrb_define_module_function(mrb,root_module,"foreground_color",get_fgcolor_method,MRB_ARGS_NONE());
    //mrb_define_module_function(mrb,root_module,"draw_image",flip_method,MRB_ARGS_ARG(1,1));

    // TODO: export current session info (vsize/hsize) as constants.

}
*/
/******************************************************
 *** Entrypoint ***************************************
 ******************************************************/

int main(int argc,const char *argv[]){

    if(argc<2){
        printf("Usage: liteperf /dev/fb0\n");
        return 1;
    }
    //printf("Argc: %d \tA1: %s\tA2: %s\tA3: %s\n",argc,argv[0],argv[1],argv[2]);
    font = create_font_info(&arial_bold);
    // Allocate the framebuffer.
    init_fb(argv[1],&screen);


    catchSignals(); 

    screen.fg_color=0xff000000;
    screen.bg_color=0xffffffff;

    while(run){
        clear_screen(&screen);
        // DO things!
        fill_screen(&screen,0xffAACCDDEE); 
        blit(&screen);
    }
    // Release the framebuffer.
    free(font);
    destroy_fb(&screen);

    return 0;

}
