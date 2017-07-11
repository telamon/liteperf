require 'pry'
require 'tgfx'
require 'chunky_png'

img = ChunkyPNG::Image.from_file File.expand_path('../test.png',__FILE__)

# Strip the alpha-component as the TGfx handle ARGB at best.
raster= img.pixels.map do |rgba|
  rgba >> 8
end

# Initialize the buffer
TGfx.init("/dev/fb1")

# Clear the buffer
TGfx.clear_screen
# Draw image
TGfx.draw_image raster, 0,0,img.width

# Draw a Green Square in top left
TGfx.bg=0x0
TGfx.fg=0x00ff00
TGfx.draw_rect 0,0,32,32

# Draw some red Text
TGfx.fg=0xff0000
TGfx.draw_text("Hello tgfx/fbgfx",20,10)

# Draw a random colored square in lower-left
TGfx.fg= (rand * 0xffffff).to_i
TGfx.draw_rect 0,TGfx.fb_height - 42,32,32

# Important `sync` method draws the actual frame to the screen
# and returns when finished, nothing will get displayed
# if you forget to call the sync method.
TGfx.sync
TGfx.destroy
