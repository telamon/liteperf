cmake_minimum_required (VERSION 2.6)
project (liteperf)

add_executable(liteperf src/fbgfx.c src/liteperf.c)

include_directories("${PROJECT_SOURCE_DIR}/mruby/include/")

add_library( mruby STATIC IMPORTED )
set_target_properties( mruby PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/mruby/build/host-debug/lib/libmruby.a )

target_link_libraries (liteperf mruby m)
