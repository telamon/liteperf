#!/usr/bin/env ruby

require 'pry'
require 'tgfx'
require 'rmagick'
require 'nokogiri'
require 'active_support/all'
begin
  width,height=[420,320]
  use_hw = File.exists? '/dev/fb1'
  if use_hw
    TGfx.init '/dev/fb1'
    width= TGfx.fb_width
    height= TGfx.fb_height
  end

  img = Magick::Image.read(File.expand_path("../clock-bg.png",__FILE__)).first
  text = Magick::Draw.new
  text.font_family = 'helvetica'
  text.gravity = Magick::CenterGravity
  LightPink = "#ffd0e2"
  text.pointsize = 48
  text.fill = LightPink
  img.annotate text, 165, 48, 280, 95, Time.now.strftime("%H:%M")
  text.pointsize = 24
  img.annotate text, 165, 24, 280, 95+48, Time.now.strftime("%A")

  #svg.css("#dow flowPara").first.content= Time.now.wday
  #svg.css("#monthyear flowPara").first.content = Time.now.strftime("%b %Y")
  #svg.css("#day flowPara").first.content= Time.now.day

  img.resize_to_fill! width, height
  # Extract the pixel data into RGBA-format compatible with TGfx
  raster = img.to_blob{ self.format='rgb'}.unpack("C*")
  # For some reason RMagick outputs duplicate sets of components.
  # We have to strip them away, seems this is only raspberry/arm related...
  raster = raster.in_groups_of(6).map do |group|
     (group[4] | group[2] << 8 | group[0] << 16)
  end

  if use_hw 
    TGfx.draw_image raster, 0,0,width
    TGfx.sync
  else
    raster = img.to_blob{ self.format='png'}
    File.write(File.expand_path("../out.png",__FILE__),raster)
  end

rescue Exception => e
  if use_hw
    puts e
    TGfx.destroy
  else
    raise e
  end
end
