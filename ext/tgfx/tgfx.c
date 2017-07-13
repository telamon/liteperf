#include <stdio.h>


#include <ruby.h>

#include "fbgfx.h"
#include "arial_bold.h"

void Init_tgfx();
static session screen;
static font_info* font;

VALUE TGfx = Qnil;
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
			put_pixel(&screen,NUM2UINT(ox)+x,NUM2UINT(oy)+y, NUM2UINT(rb_ary_entry(data, y*w+x)));
		}
	return Qnil;
}
VALUE method_destroy(VALUE self){
	destroy_fb(&screen);
	return Qnil;
}
VALUE method_get_height(VALUE self){
	return UINT2NUM(screen.vinfo.yres);
}
VALUE method_get_width(VALUE self){
	return UINT2NUM(screen.vinfo.xres);
}
VALUE method_get_depth(VALUE self){
	return UINT2NUM(screen.vinfo.bits_per_pixel);
}
VALUE method_put_pixel(VALUE self, VALUE x, VALUE y, VALUE color){
	put_pixel(&screen,NUM2UINT(x),NUM2UINT(y),NUM2UINT(color));
	return Qnil;
}
void Init_tgfx(){
	TGfx = rb_define_module("TGfx");
	rb_define_singleton_method(TGfx,"init",method_init,1);
	rb_define_singleton_method(TGfx,"sync",method_sync,0);
	rb_define_singleton_method(TGfx,"clear_screen",method_clear_screen,0);
	rb_define_singleton_method(TGfx,"draw_rect",method_draw_rect,4);
	rb_define_singleton_method(TGfx,"fg=",method_set_fg,1);
	rb_define_singleton_method(TGfx,"fg",method_get_fg,0);
	rb_define_singleton_method(TGfx,"bg=",method_set_bg,1);
	rb_define_singleton_method(TGfx,"bg",method_get_bg,0);
	rb_define_singleton_method(TGfx,"draw_text",method_draw_text,3);
	rb_define_singleton_method(TGfx,"draw_image",method_draw_image,4);
	rb_define_singleton_method(TGfx,"destroy",method_destroy,0);
	rb_define_singleton_method(TGfx,"fb_height",method_get_height,0);
	rb_define_singleton_method(TGfx,"fb_width",method_get_width,0);
	rb_define_singleton_method(TGfx,"fb_depth",method_get_depth,0);
}
