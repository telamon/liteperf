FROM hypriot/rpi-alpine-scratch

RUN apk add  --update alpine-sdk git bison libpng-dev

RUN git clone https://github.com/telamon/liteperf.git \
	&& cd liteperf \
	&& ./build.sh

CMD sh



# if trying to build DirectFB from source which we're not gonna do. 
# wget https://github.com/Distrotech/DirectFB/archive/distrotech-DirectFB-1.7.0.tar.gz
# ./configure --disable-jpeg --disable-png --disable-gif --enable-fbdev=yes --disable-freetype
 --prefix=/usr --disable-devmem --with-gfxdrivers='none' --with-inputdrivers='none' --enable-static --disable-shared
#wget https://github.com/Distrotech/flux/archive/distrotech-flux-1.4.4.tar.gz