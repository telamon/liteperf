FROM hypriot/rpi-alpine-scratch

RUN apk update && apk upgrade && apk add git gcc bison musl-dev gdb

RUN git clone https://github.com/telamon/liteperf.git \
	&& cd liteperf \
	&& ./build.sh

CMD sh
