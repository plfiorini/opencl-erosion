project(libsdl2_ttf)

include(build_functions)

if(WIN32)
  add_traditional_third_party(
    NAME "SDL2_ttf-2.0.12.tar.gz"
    VERSION "2.0.12"
    LIBS "SDL2.lib"
  )
else()
  add_traditional_third_party(
    NAME "SDL2_ttf-2.0.12.tar.gz"
    VERSION "2.0.12"
    LIBS "libSDL2.so"
  )
endif()