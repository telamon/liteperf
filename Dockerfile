FROM hypriot/rpi-alpine-scratch

RUN apk add  --update alpine-sdk git bison libpng-dev

RUN git clone https://github.com/telamon/liteperf.git \
	&& cd liteperf \
	&& ./build.sh

CMD sh

# CMAKE line for libdirectfb
# cmake  -DENABLE_SYSTEM_X11=OFF -DENABLE_FONTPROVIDER_FREETYPE=OFF -DENABLE_IMAGEPROVIDER_DFIFF=OFF -DENABLE_IMAGEPROVIDER_JPEG=OFF -DENABLE_FUSIONSOUND=OFF ../directfb/
# wget https://github.com/Distrotech/DirectFB/archive/distrotech-DirectFB-1.7.0.tar.gz
# 