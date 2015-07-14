project(wingl)

if(WIN32 OR CYGWIN)
  set(WINDOWS_OPENGL_EXTENSIONS "${CMAKE_CURRENT_SOURCE_DIR}/win/include" CACHE PATCH "Window OpenGL Extension Header Files" )
  log_info("Setup Windows OpenGL Extensions Cache Variable: ${WINDOWS_OPENGL_EXTENSIONS}")
endif()
