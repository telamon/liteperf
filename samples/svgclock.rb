#!/usr/bin/env ruby

require 'pry'
require 'tgfx'
require 'rmagick'
require 'nokogiri'
begin
  width,height=[420,320]
  use_hw = File.exists? '/dev/fb1'
  if use_hw
    TGfx.init '/dev/fb1'
    width= TGfx.fb_width
    height= TGfx.fb_height
  end

  svg = Nokogiri::XML.parse File.read(File.expand_path('../clock.svg',__FILE__))
  svg.css("#dow flowPara").first.content= Time.now.wday
  svg.css("#monthyear flowPara").first.content = Time.now.strftime("%b %Y")
  svg.css("#day flowPara").first.content= Time.now.day
  svg.css("#time flowPara").first.content = Time.now.strftime("%H:%M")
  img, data = Magick::Image.from_blob(svg.to_xml) {
    self.format = 'SVG'
    self.background_color = 'black'
  }
  img.resize_to_fill! width, height

  if use_hw 
    raster = img.to_blob{ self.format='rgba'}
    TGfx.draw_image raster.unpack("L*"), 0,0,width
    TGfx.sync
  else
    raster = img.to_blob{ self.format='png'}
    File.write(File.expand_path("../out.png",__FILE__),raster)
  end
rescue Exception => e
  TGfx.destroy
end
