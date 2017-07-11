require 'pry'
require 'tgfx'
require 'chunky_png'

img = ChunkyPNG::Image.from_file File.expand_path('../test.png',__FILE__)
raster= img.pixels.map do |rgba|
  rgba >> 8
end

TGfx::FB.init("/dev/fb1") 
TGfx::FB.clear_screen
TGfx::FB.draw_image raster, 0,0,img.width
TGfx::FB.background_color=0x0
TGfx::FB.foreground_color=0x00ff00
TGfx::FB.draw_rect 0,0,32,32
TGfx::FB.foreground_color=0xff0000
TGfx::FB.draw_text("Hello tgfx/fbgfx",20,10)
TGfx::FB.sync
