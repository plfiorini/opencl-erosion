project(libsdl2)

include(build_functions)

add_cmake_third_party(
  NAME "SDL-2.0.1-8054.tar.gz"
  VERSION "2.0.1"
  LIBS "libSDL2-2.0.so"
  PARAMETERS "-DSDL_SHARED:BOOL=OFF"
)