#include <stdio.h>


#include <ruby.h>

#include "fbgfx.h"
#include "arial_bold.h"

void Init_tgfx();
static session screen;
static font_info* font;

VALUE TGfx = Qnil;
VALUE FB = Qnil;
/*-------------------------------------*/

VALUE method_init(VALUE self, VALUE device){
	char* cdev = StringValueCStr(device);
	printf("dev: %s",cdev);
	init_fb(cdev,&screen);
    font = create_font_info(&arial_bold);
    screen.fg_color=0xff000000;
    screen.bg_color=0xffffffff;
	return Qnil;
}
VALUE method_sync(VALUE self){
	blit(&screen);
	return Qnil;
}

VALUE method_clear_screen(VALUE self){
	clear_screen(&screen);
	return Qnil;
}
VALUE method_draw_rect(VALUE self, VALUE x, VALUE y, VALUE w, VALUE h){
	draw_rect(&screen,NUM2UINT(x),NUM2UINT(y),NUM2UINT(w),NUM2UINT(h),screen.fg_color);
	return Qnil;
}

void Init_tgfx(){
	TGfx = rb_define_module("TGfx");
	FB = rb_define_module_under(TGfx,"FB");

	rb_define_singleton_method(FB,"init",method_init,1);
	rb_define_singleton_method(FB,"sync",method_sync,0);
	rb_define_singleton_method(FB,"clear_screen",method_clear_screen,0);
	rb_define_singleton_method(FB,"draw_rect",method_draw_rect,5);

    /*
    mrb_define_module_function(mrb,root_module,"draw_rect",draw_rect_method,MRB_ARGS_ARG(4,1));
    mrb_define_module_function(mrb,root_module,"draw_text",draw_text_method,MRB_ARGS_REQ(3));
    mrb_define_module_function(mrb,root_module,"foreground_color=",set_fgcolor_method,MRB_ARGS_REQ(1));
    mrb_define_module_function(mrb,root_module,"background_color=",set_bgcolor_method,MRB_ARGS_REQ(1));
    mrb_define_module_function(mrb,root_module,"background_color",get_bgcolor_method,MRB_ARGS_NONE());
    mrb_define_module_function(mrb,root_module,"foreground_color",get_fgcolor_method,MRB_ARGS_NONE());
    //mrb_define_module_function(mrb,root_module,"draw_image",flip_method,MRB_ARGS_ARG(1,1));*/
}
