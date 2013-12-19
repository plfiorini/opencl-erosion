project(libsdl2)

include(build_functions)

if(WIN32)
  add_cmake_third_party(
    NAME "SDL-2.0.1-8054.tar.gz"
    VERSION "2.0.1"
    LIBS "SDL2.lib"
    PARAMETERS "-DSDL_SHARED:BOOL=OFF"
  )
else()
  add_cmake_third_party(
    NAME "SDL-2.0.1-8054.tar.gz"
    VERSION "2.0.1"
    LIBS "libSDL2.so"
  )
endif()