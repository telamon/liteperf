require 'pry'
require 'tgfx'

TGfx::FB.init("/dev/fb1")
TGfx::FB.clear_screen
TGfx::FB.draw_text("Hello tgfx/fbgfx",20,10)
TGfx::FB.sync
