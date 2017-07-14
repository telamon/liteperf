#!/usr/bin/env ruby

require 'pry'
require 'tgfx'
require 'rmagick'
require 'nokogiri'
require 'active_support/all'
begin
  LightPink = "#ffd0e2"
  width,height=[420,320]
  use_hw = File.exists? '/dev/fb1'
  if use_hw
    TGfx.init '/dev/fb1'
    width= TGfx.fb_width
    height= TGfx.fb_height
  end

  bg = Magick::Image.read(File.expand_path("../clock-bg.png",__FILE__)).first
  while true
	  img = bg.clone  
	  text = Magick::Draw.new
	  text.font_family = 'helvetica'
	  text.gravity = Magick::CenterGravity
	  text.pointsize = 48
	  text.fill = LightPink
	  img.annotate text, 165, 48, 280, 95, Time.now.strftime("%H:%M")
	  text.pointsize = 24
	  img.annotate text, 165, 24, 280, 95+48, Time.now.strftime("%A")

	  #svg.css("#dow flowPara").first.content= Time.now.wday
	  #svg.css("#monthyear flowPara").first.content = Time.now.strftime("%b %Y")
	  #svg.css("#day flowPara").first.content= Time.now.day

	  #img.resize_to_fill! width, height
	  # Extract the pixel data into RGBA-format compatible with TGfx
	  raster = img.to_blob{ self.format='rgba'}.unpack("L*")
	  if use_hw 
	    TGfx.draw_image raster, 0,0,width
	    TGfx.sync
	  else
	    raster = img.to_blob{ self.format='png'}
	    File.write(File.expand_path("../out.png",__FILE__),raster)
	  end
          sleep(5)
  end

rescue Exception => e
  if use_hw
    puts e
    TGfx.destroy
  else
    raise e
  end
end
