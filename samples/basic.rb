require 'pry'
require 'tgfx'
require 'chunky_png'

img = ChunkyPNG::Image.from_file File.expand_path('../test.png',__FILE__)

TGfx::FB.init("/dev/fb1") 
TGfx::FB.clear_screen
TGfx::FB.draw_image img.pixels, 0,0,img.width

TGfx::FB.draw_text("Hello tgfx/fbgfx",20,10)
TGfx::FB.sync
