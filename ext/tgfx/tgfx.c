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
	Check_Type(device, T_STRING);
	char* cdev = StringValueCStr(device);
	printf("Opening: %s\n",cdev);
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
	Check_Type(x, T_FIXNUM);
	Check_Type(y, T_FIXNUM);
	Check_Type(w, T_FIXNUM);
	Check_Type(h, T_FIXNUM);
	draw_rect(&screen,NUM2UINT(x),NUM2UINT(y),NUM2UINT(w),NUM2UINT(h),screen.fg_color);
	return Qnil;
}

VALUE method_set_fg(VALUE self,VALUE rgba){
	Check_Type(rgba, T_FIXNUM);
    screen.fg_color=NUM2UINT(rgba);
	return UINT2NUM(screen.fg_color);
}
VALUE method_get_fg(VALUE self){
	return UINT2NUM(screen.fg_color);
}
VALUE method_set_bg(VALUE self,VALUE rgba){
	Check_Type(rgba, T_FIXNUM);
    screen.bg_color=NUM2UINT(rgba);
	return UINT2NUM(screen.bg_color);
}
VALUE method_get_bg(VALUE self){
	return UINT2NUM(screen.bg_color);
}
VALUE method_draw_text(VALUE self,VALUE text,VALUE x,VALUE y){
	Check_Type(text, T_STRING);
	Check_Type(x, T_FIXNUM);
	Check_Type(y, T_FIXNUM);
	draw_text(&screen,font,StringValueCStr(text),UINT2NUM(x),UINT2NUM(y));
	return Qnil;
}

VALUE method_draw_image(VALUE self,VALUE data,VALUE ox,VALUE oy,VALUE scansize){
	Check_Type(data, T_ARRAY);
	Check_Type(ox, T_FIXNUM);
	Check_Type(oy, T_FIXNUM);
	Check_Type(scansize, T_FIXNUM);
	int x,y,w = NUM2UINT(scansize),h = RARRAY_LEN(data) / w;
	for(y=0;y<h;y++)
		for(x=0;x<w;x++){
			put_pixel(&screen,ox+x,oy+y, NUM2UINT(rb_ary_entry(data, y*w+x)));
		}
	return Qnil;
}
void Init_tgfx(){
	TGfx = rb_define_module("TGfx");
	FB = rb_define_module_under(TGfx,"FB");

	rb_define_singleton_method(FB,"init",method_init,1);
	rb_define_singleton_method(FB,"sync",method_sync,0);
	rb_define_singleton_method(FB,"clear_screen",method_clear_screen,0);
	rb_define_singleton_method(FB,"draw_rect",method_draw_rect,4);
	rb_define_singleton_method(FB,"foreground_color=",method_set_fg,1);
	rb_define_singleton_method(FB,"foreground_color",method_get_fg,0);
	rb_define_singleton_method(FB,"background_color=",method_set_bg,1);
	rb_define_singleton_method(FB,"background_color",method_get_bg,0);
	rb_define_singleton_method(FB,"draw_text",method_draw_text,3);
	rb_define_singleton_method(FB,"draw_image",method_draw_image,4);
    /*
    mrb_define_module_function(mrb,root_module,"draw_rect",draw_rect_method,MRB_ARGS_ARG(4,1));
    mrb_define_module_function(mrb,root_module,"draw_text",draw_text_method,MRB_ARGS_REQ(3));
    mrb_define_module_function(mrb,root_module,"foreground_color=",set_fgcolor_method,MRB_ARGS_REQ(1));
    mrb_define_module_function(mrb,root_module,"background_color=",set_bgcolor_method,MRB_ARGS_REQ(1));
    mrb_define_module_function(mrb,root_module,"background_color",get_bgcolor_method,MRB_ARGS_NONE());
    mrb_define_module_function(mrb,root_module,"foreground_color",get_fgcolor_method,MRB_ARGS_NONE());
    //mrb_define_module_function(mrb,root_module,"draw_image",flip_method,MRB_ARGS_ARG(1,1));*/
}
