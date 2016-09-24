# Liteperf

An attempt to put some of the dust-collecting `fbtft` compatible screens back into work.

The idea is to provide a way to draw graphics directly to the `/dev/fb0` on small headless
devices that you do not want to run a full Xorg on. (Raspberry pi, odroid and so on)

During development this code is tested on an OdroidXU4 in a Cloudshell.

Manual building:

	./build.sh
