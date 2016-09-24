#!/bin/bash

git submodule init \
	&& git submodule update \
	&& cd mruby \
	&& ./minirake \
	&& cd .. \
	&& mkdir -p build/ \
	&& cd build \
	&& cmake ../ \
	&& make
