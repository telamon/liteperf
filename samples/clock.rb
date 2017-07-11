require 'pry'
require 'tgfx'
require 'chunky_png'
begin
  TGfx.init("/dev/fb1")
  while(true) do
    TGfx.clear_screen
    TGfx.fg= 0x0
    TGfx.draw_text Time.now.strftime("%H:%M:%S  %d/%m/%y"),10,10
    TGfx.fg= (rand * 0xffffff).to_i
    TGfx.draw_rect 10,60,100,50
    TGfx.sync 
  end
rescue Exception => e
  puts e
  TGfx.destroy
end
  
