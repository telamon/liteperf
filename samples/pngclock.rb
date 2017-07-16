#!/usr/bin/env ruby

require 'pry'
require 'tgfx'
require 'rmagick'
require 'nokogiri'
require 'active_support/all'
require 'open-uri'
Kalvin = 273.15
IconCache={}
def forecast
  key = ENV["OWM_KEY"]
  if key.nil? && File.exists?(File.expand_path("../../omw_api.key",__FILE__))
    key = File.read(File.expand_path("../../omw_api.key",__FILE__))
  end
  city= ENV["OWM_CITY"] || "Göteborg,se"
  

  forecast_file = File.expand_path("../forecast.json",__FILE__)
  data=nil
  if File.exists?(forecast_file) && Time.now - File.ctime(forecast_file) < 60
    data = JSON.parse File.read(forecast_file), symbolize_names: true
  else
    res = open("http://api.openweathermap.org/data/2.5/weather?q=#{CGI.escape(city)}&APPID=#{key}").read
    File.write forecast_file, res
    data = JSON.parse res, symbolize_names: true
  end
  data[:main][:tempc] = data[:main][:temp] - Kalvin
  data[:main][:temp_minc] = data[:main][:temp_min] - Kalvin
  data[:main][:temp_maxc] = data[:main][:temp_max] - Kalvin
  icon_id= data[:weather].first.try(:[],:icon)
  data[:icon_url] =  "http://openweathermap.org/img/w/#{icon_id}.png"
  unless IconCache[icon_id]
    IconCache[icon_id] = Magick::Image.from_blob(open(data[:icon_url]).read).first
  end
  data[:icon]=IconCache[icon_id]
  data[:wdesc] = data[:weather].first.try(:[],:description).try(:camelcase)
  data
end

def humanize secs
  [[60, :s], [60, :m], [24, :h], [1000, :d]].map{ |count, name|
    if secs > 0
      secs, n = secs.divmod(count)
      "#{n.to_i}#{name}" if n > 0
    end
  }.compact.reverse.join('').gsub(/^(\d+d\d+h)\d+m\d+s/,'\1')
end
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
    #text.font_family = 'Helvetica'
    text.gravity = Magick::CenterGravity

    # Time
    text.pointsize = 48
    text.fill = LightPink
    img.annotate text, 165, 48, 280, 95, Time.now.strftime("%H:%M")

    # Day of week
    text.pointsize = 24
    img.annotate text, 165, 24, 280, 95+48, Time.now.strftime("%A")

    # Day of month
    text.pointsize = 42
    text.fill = "#030755" # cold-black
    text.font_weight= Magick::BoldWeight
    img.annotate text, 65,42, 336, 200, Time.now.day.to_s

    # Month/Year
    text.font_weight= Magick::NormalWeight
    text.pointsize = 17
    img.annotate text, 65,18, 336, 246, Time.now.strftime("%b %Y")

    # Temperature
    text.gravity = Magick::WestGravity
    text.pointsize = 42
    text.fill = "#030755" # cold-black
    text.font_weight= Magick::BoldWeight
    img.annotate text, 65,42, 45, 20, "#{forecast[:main][:tempc].round}C" #\u2103"
    # weather icon & description
    img.composite! forecast[:icon], 2,12, Magick::OverCompositeOp
    text.pointsize = 20
    img.annotate text, 65,100, 45, 18, "#{forecast[:wdesc]}"

    # misc lines
    lines=[]

    # Rain object from forecast.
    if forecast[:rain]
      forecast[:rain].map{|h,c| "#{(c*100).round}% ~#{h}"}.join(", ")
    end

    # Sun-rise/set
    if forecast[:sys][:sunrise] > Time.now.to_i
      lines <<  "Sunrise in " + humanize( forecast[:sys][:sunrise] - Time.now.to_i)
    elsif forecast[:sys][:sunset] > Time.now.to_i
      lines << "Sunset in " + humanize( forecast[:sys][:sunset] - Time.now.to_i)
    end
    lines << "Humidity: #{forecast[:main][:humidity]}%"
    lines << "Pressure: #{forecast[:main][:pressure]}"
    lines << "Tony's birthday: #{humanize(Time.parse("09/08/2017 00:00") - Time.now)}"
    y = 80
    text.pointsize = 20
    lines.each do |line|
      y+= 20 + 4
      img.annotate text, 65,100, 30, y, line
    end
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
    break if !use_hw # Render once and exit if no tft is attached.
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
