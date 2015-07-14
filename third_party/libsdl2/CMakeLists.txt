project(libsdl2)

include(build_functions)

if(WIN32 OR CYGWIN)
  add_cmake_third_party(
    NAME "SDL2-2.0.3_patch.tar.gz"
    VERSION "2.0.3"
    LIBS "SDL2.lib"
    PARAMETERS "-DSDL_SHARED:BOOL=OFF"
  )
else()
  add_cmake_third_party(
    NAME "SDL2-2.0.3.tar.gz"
    VERSION "2.0.3"
    LIBS "libSDL2.so"
  )
endif()