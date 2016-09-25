# Liteperf

An attempt to put some of the dust-collecting `fbtft` compatible screens back into work.
(Sadly most of those screens only render at 4-10fps due to slow but functional gpio interfaces,
so don't get your hopes up.)

The idea is to provide a way to draw graphics directly to the `/dev/fb0` on small headless
devices that you do not want to install/run a full Xorg on. (Raspberry pi, odroid and so on)

Another usecase is to package the precompiled binary into a minimalistic docker-image and making the
framebuffer device accessible as a volume. 


During development this code is tested on an OdroidXU4 in a Cloudshell.

Manual building:

	./build.sh


### TODO:

* Replace current memcpy with buffer-blitting as described here: http://www.ummon.eu/Linux/API/Devices/framebuffer.html
* Write some useful gfx routines
* Expose some useful gfx routines through mrb interface
* See if it's not more useful to port UTFT through wiring and expose it's calls as nice mruby methods. That would give us another great option to use our TFT-modules on raspberries and what not without having to install/setup the FBTFT driver.
* Rename project to something more appropriate.