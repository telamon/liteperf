#!/usr/bin/env ruby

require 'pry'
require 'tgfx'
require 'rmagick'
require 'nokogiri'
width,height=[420,340]
if File.exists? '/dev/fb1'
  Tgfx.init '/dev/fb1'
  width= Tgfx.fb_width
  height= Tgfx.fb_height
end

svg = Nokogiri::XML.parse File.read(File.expand_path('../clock.svg',__FILE__))
svg.css("#dow flowPara").first.content= Time.now.wday
svg.css("#monthyear flowPara").first.content = Time.now.strftime("%b %Y")
svg.css("#day flowPara").first.content= Time.now.day
svg.css("#time flowPara").first.content = Time.now.strftime("%H:%M")
img, data = Magick::Image.from_blob(svg.to_xml) {
  self.format = 'SVG'
  self.background_color = 'transparent'
}
img.resize_to_fill! width, height
raster = img.to_blob{ self.format='rgba'}
Tgfx.draw_image raster, 0,0,width

