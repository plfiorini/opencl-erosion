project(libsdl2_ttf)

include(build_functions)

if(WIN32 OR CYGWIN)
  #add_traditional_third_party(
  #  NAME "SDL2_ttf-2.0.12.tar.gz"
  #  VERSION "2.0.12"
  #  LIBS "SDL2.lib"
  #)
  include_directories("win\\include")
  
else()
  add_traditional_third_party(
    NAME "SDL2_ttf-2.0.12.tar.gz"
    VERSION "2.0.12"
    LIBS "libSDL2_ttf.so"
  )
endif()
