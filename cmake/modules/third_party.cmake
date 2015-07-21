#
# This function extends the include path for the use of the sqlite3 library
# for the project defined by ${PROJECT_NAMESPACE}${PROJECT_NAME}
# Also the library name is appended to the variable ${}_dependant_libs
# for the use in dependent applications
#
function(use_sqlite3)
  find_path(SQLITE3_INCLUDE_PATH sqlite3.h
    PATHS /usr/include/
    DOC "Include path for header files of sqlite3"
  )

  find_library(SQLITE3_LIB libsqlite3.so
    PATHS /usr/lib
    DOC "Fully qualified library name of the sqlite3 library"
  )

  log_debug("SQLITE3_INCLUDE_PATH=${SQLITE3_INCLUDE_PATH}")
  log_debug("SQLITE3_LIB=${SQLITE3_LIB}")

  if(NOT EXISTS ${SQLITE3_INCLUDE_PATH})
    log_fatal_error("sqlite3 header files not found")
  endif()

  if(NOT EXISTS ${SQLITE3_LIB})
    log_fatal_error("libsqlite3.so library not found")
  endif()

  include_directories(
    SYSTEM ${SQLITE3_INCLUDE_PATH}
  )

  target_link_libraries(${PROJECT_NAME}
    ${SQLITE3_LIB}
  )

  add_include_dependency(${SQLITE3_INCLUDE_PATH})
endfunction(use_sqlite3)

function(use_opencl)
  find_package(OpenCL REQUIRED)

  include_directories(${OPENCL_INCLUDE_DIRS})
  if( OPENCL_HAS_CPP_BINDINGS )
    log_info( "OpenCL has CPP bindings. Full include is: ${OPENCL_INCLUDE_DIRS}")
  else( OPENCL_HAS_CPP_BINDINGS )
    log_info( "No OpenCL CPP bindings found" )
  endif( OPENCL_HAS_CPP_BINDINGS )

  target_link_libraries(${PROJECT_NAME}
    ${OPENCL_LIBRARIES}
  )
  log_info("use_opencl: Using ${OPENCL_LIBRARIES}") 

  add_include_dependency(${OPENCL_INCLUDE_DIRS})
endfunction(use_opencl)

function(use_boost)
  if( "${CMAKE_SYSTEM_NAME}" STREQUAL "Linux" ) 
    foreach(lib ${ARGN})
      log_debug("use_boost: adding boost library ${lib}")
      set(libname "boost_${lib}")
      use_system_library(${libname})
    endforeach()
  elseif( "${CMAKE_SYSTEM_NAME}" STREQUAL "Windows" )
    set(BOOST_INCLUDE_PATH "${WINDOWS_LIBRARY_BASE_PATH}\\boost_1_58_0" CACHE PATH  "Set the include path to the boost libraries")
    include_directories(${BOOST_INCLUDE_PATH})
    add_include_dependency(${BOOST_INCLUDE_PATH})
    set(BOOST_LIB_PATH "${WINDOWS_LIBRARY_BASE_PATH}\\boost_1_58_vc120\\lib"  CACHE PATH  "Set the library path to the boost libraries")
	set(BOOST_PREFIX "libboost_")
	use_user_library( 
      SEARCH_PATH "${BOOST_LIB_PATH}"
      PREFIX "${BOOST_PREFIX}"
      POSTFIX "-vc120-mt-gd-1_58"
	  BUILD_TYPE "debug"
      ${ARGN}
    )
	use_user_library( 
      SEARCH_PATH "${BOOST_LIB_PATH}"
      PREFIX "${BOOST_PREFIX}"
      POSTFIX "-vc120-mt-1_58"
	  BUILD_TYPE "general"
      ${ARGN}
    )
  endif()
endfunction(use_boost)

function(use_opengl)
  if("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    use_system_library(GL GLEW glut)
  elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
	log_info("Using Windows OpenGL Extension Headers: ${WINDOWS_OPENGL_EXTENSIONS}")
	include_directories(${WINDOWS_OPENGL_EXTENSIONS})
    use_system_library(OpenGL32)
	
	set(GLEW_BASE_PATH "${WINDOWS_LIBRARY_BASE_PATH}/glew-1.12.0")
	set(GLEW_INCLUDE_PATH "${GLEW_BASE_PATH}/include" CACHE PATH "Set the include path to the GLEW libraries")
    include_directories(${GLEW_INCLUDE_PATH})
    add_include_dependency(${GLEW_INCLUDE_PATH})
    set(GLEW_LIB_PATH "${GLEW_BASE_PATH}/lib/Release/Win32" CACHE PATH  "Set the library path to the GLEW libraries")
    use_user_library(
      SEARCH_PATH ${GLEW_LIB_PATH}
      glew32
    )
	set(GLEW_BIN_PATH "${GLEW_BASE_PATH}/bin/Release/Win32" CACHE PATH  "Set the bin path to the GLEW dlls")
	add_dll_dependency("${GLEW_BIN_PATH}/glew32.dll")
	
	set(GLUT_BASE_PATH "${WINDOWS_LIBRARY_BASE_PATH}/freeglut-3.0.0-1")
	set(GLUT_INCLUDE_PATH "${GLUT_BASE_PATH}/include" CACHE PATH "Set the include path to the GLUT libraries")
    include_directories(${GLUT_INCLUDE_PATH})
    add_include_dependency(${GLUT_INCLUDE_PATH})
    set(GLUT_LIB_PATH "${GLUT_BASE_PATH}/lib" CACHE PATH  "Set the library path to the GLUT libraries")
	use_user_library(
      SEARCH_PATH ${GLUT_LIB_PATH}
      freeglut
    )
	set(GLUT_BIN_PATH "${GLUT_BASE_PATH}/bin" CACHE PATH  "Set the bin path to the GLUT dlls")
	add_dll_dependency("${GLUT_BIN_PATH}/freeglut.dll")
  endif()
endfunction(use_opengl)

function(use_sdl)
  if("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
	use_third_party(libsdl2)
  elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
	use_third_party(libsdl2)
	use_system_library(
	  winmm.lib
      version.lib
      Imm32.lib
	)
  endif()
endfunction(use_sdl)

function(use_sdlttf)
  if("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
	use_third_party(libsdl2_ttf)
  elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
	include_directories(${libsdl2_HEADERS}/SDL2)
    set(SDLTTF_INCLUDE_PATH "${CMAKE_SOURCE_DIR}\\third_party\\libsdl2_ttf\\win\\include" CACHE PATH "Set the include path to the SDL TTF libraries")
    include_directories(${SDLTTF_INCLUDE_PATH})
    add_include_dependency(${SDLTTF_INCLUDE_PATH})
    set(SDLTTF_LIB_PATH "${CMAKE_SOURCE_DIR}\\third_party\\libsdl2_ttf\\win\\lib" CACHE PATH  "Set the library path to the SDL TTF libraries")
    use_user_library(
      SEARCH_PATH ${SDLTTF_LIB_PATH}
      SDL_ttf_VS2013
    )
	set(FREETYPE_LIB_PATH "${WINDOWS_LIBRARY_BASE_PATH}/freetype-2.6_vc120/lib" CACHE PATH  "Set the library path to the freetype libraries")
	use_user_library(
	  SEARCH_PATH ${FREETYPE_LIB_PATH}
	  freetype26MT
	)	
  endif()
endfunction(use_sdlttf)

function(use_devil)
  if("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    use_system_library(IL ILU ILUT)
  elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    set(DEVIL_INCLUDE_PATH "${WINDOWS_LIBRARY_BASE_PATH}\\DevIL\\include" CACHE PATH "Set the include path to the DevIL libraries")
    include_directories(${DEVIL_INCLUDE_PATH})
    add_include_dependency(${DEVIL_INCLUDE_PATH})
    set(DEVIL_LIB_PATH "${WINDOWS_LIBRARY_BASE_PATH}\\DevIL\\lib" CACHE PATH  "Set the library path to the DevIL libraries")
    use_user_library(
      SEARCH_PATH ${DEVIL_LIB_PATH}
      DevIL ILU ILUT
    )
	add_dll_dependency("${DEVIL_LIB_PATH}/DevIL.dll")
	add_dll_dependency("${DEVIL_LIB_PATH}/ILU.dll")
	add_dll_dependency("${DEVIL_LIB_PATH}/ILUT.dll")
  endif()
endfunction(use_devil)

function(use_system_library)
  foreach(name ${ARGN})
    log_debug("use_system_library: Searching for ${name}")

    if("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
      find_library(${name}_LIB ${name}
        PATHS /usr/lib /usr/lib64 /usr/lib/x86_64-linux-gnu
        DOC "Fully qualified library path of ${name}"
      )
    elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
      find_library(${name}_LIB ${name}
        PATHS "C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Lib"
        DOC "Fully qualified library path of ${name}"
      )  
    endif()

    if(NOT EXISTS ${${name}_LIB})
      log_fatal_error("use_system_library: ${name} library not found")
    else()
      log_info("use_system_library: Using library ${${name}_LIB}")
    endif()

    target_link_libraries(${PROJECT_NAME}
      ${${name}_LIB}
    )
  endforeach()
endfunction(use_system_library)

function(use_third_party name)
  log_debug("use_third_party: Searching libraries of ${name}")
  foreach(lib ${${name}_LIBS})
    if(NOT EXISTS ${lib})
      log_warning("${lib} library not found (yet)")
    else()
      log_info("use_third_party: Using library ${lib}")
    endif()

    target_link_libraries(${PROJECT_NAME}
      ${lib}
    )
  endforeach()

  add_dependencies(${PROJECT_NAME} ${name})
endfunction(use_third_party)

function(use_user_library)
  cmake_parse_arguments( USER_LIB "" "SEARCH_PATH;PREFIX;POSTFIX;BUILD_TYPE" "" ${ARGN} )
  
  foreach(name ${USER_LIB_UNPARSED_ARGUMENTS})
    if( USER_LIB_PREFIX )
      set(name "${USER_LIB_PREFIX}${name}")
    endif()
    if( USER_LIB_POSTFIX )
      set(name "${name}${USER_LIB_POSTFIX}")
    endif()
    log_debug("use_user_library: Searching for ${name}")

    find_library(${name}_USER_LIB ${name}
      PATHS "${USER_LIB_SEARCH_PATH}"
      DOC "Fully qualified library path of ${name}"
    )  

    if(NOT EXISTS ${${name}_USER_LIB})
      log_fatal_error("use_user_library: ${name} library not found")
    else()
      log_info("use_user_library: Using library ${${name}_USER_LIB}")
    endif()

	if( USER_LIB_BUILD_TYPE )
      target_link_libraries(
	    ${PROJECT_NAME}
		${USER_LIB_BUILD_TYPE}
        ${${name}_USER_LIB}
      )
	else()
      target_link_libraries(
	    ${PROJECT_NAME}
        ${${name}_USER_LIB}
      )
	endif()
  endforeach()
endfunction(use_user_library)