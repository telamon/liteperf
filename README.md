# TGfx

An attempt to put some of the dust-collecting `fbtft` compatible screens back into work.
(Sadly most of those screens only render at 4-10fps due to slow but functional gpio interfaces,
so don't get your hopes up.)

The idea is to provide a way to draw graphics directly to the `/dev/fb0` on small headless
devices that you do not want to install/run a full Xorg on. (Raspberry pi, odroid and so on)

Another usecase is to package the precompiled binary into a minimalistic docker-image and making the
framebuffer device accessible as a volume. 


During development this code is tested on an OdroidXU4 in a Cloudshell.

Manual building:

	bundle
	bundle exec rake compile

	# Tweak the device-string-identifier in this file if necessary
	# defaults to: /dev/fb1
	bundle exec ruby samples/basic.rb


Usage example:

	require 'tgfx'

	# Initialize the buffer
	TGfx.init("/dev/fb1")

	# Clear the buffer
	TGfx.clear_screen

	# Draw image
	require 'chunky_png'
	img = ChunkyPNG::Image.from_file File.expand_path('../test.png',__FILE__)
	# Strip the alpha-component as the TGfx handle ARGB at best.
	raster= img.pixels.map do |rgba|
	  rgba >> 8
	end
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
	TGfx.draw_rect 0,TGfx.fb_height - 10,32,32

	# Important `sync` method draws the actual frame to the screen
	# and returns when finished, nothing will get displayed
	# if you forget to call the sync method.
	TGfx.sync

	# Release the framebuffer and destroy
	# allocated resources.
	TGfx.destroy


### TODO:

* Replace current memcpy with buffer-blitting as described here: http://www.ummon.eu/Linux/API/Devices/framebuffer.html
* Write some useful gfx routines
* See if it's not more useful to port UTFT through wiring and expose it's calls as nice mruby methods. That would give us another great option to use our TFT-modules on raspberries and what not without having to install/setup the FBTFT driver.
