function(add_include_dependency include_path)
  log_debug("Adding header file dependency ${include_path} to ${PROJECT_NAME}${PROJECT_DEPENDENCY_POSTFIX}") 
  if(${PROJECT_NAME}${PROJECT_DEPENDENCY_POSTFIX})
    string(REPLACE "${include_path}" "" deps ${${PROJECT_NAME}${PROJECT_DEPENDENCY_POSTFIX}})
  endif()
  set(${PROJECT_NAME}${PROJECT_DEPENDENCY_POSTFIX} "${deps}" "${include_path}" CACHE INTERNAL "${PROJECT_NAME} include dependencies" FORCE)
endfunction(add_include_dependency)

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

  include_directories( ${OPENCL_INCLUDE_DIRS} )
  if( OPENCL_HAS_CPP_BINDINGS )
    log_info( "OpenCL has CPP bindings. Full include is: " ${OPENCL_INCLUDE_DIRS} )
  else( OPENCL_HAS_CPP_BINDINGS )
    log_info( "No OpenCL CPP bindings found" )
  endif( OPENCL_HAS_CPP_BINDINGS )

  log_info("ld path: $ENV{LD_LIBRARY_PATH}")

  target_link_libraries(${PROJECT_NAME}
    ${OPENCL_LIBRARIES}
  )
  log_info("use_opencl: Using ${OPENCL_LIBRARIES}") 

  add_include_dependency(${OPENCL_INCLUDE_DIRS})
endfunction(use_opencl)

#function(use_libsdl2)
#
#endfunction(use_libsdl2)

function(use_opengl)
  if("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    use_system_library(GL)
    use_system_library(GLEW)
  elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    use_system_library(OpenGL32)  
  endif()

endfunction(use_opengl)

function(use_system_library name)
  log_info("use_system_library: Searching for ${name}")

  if("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    find_library(${name}_LIB ${name}
      PATHS /usr/lib /usr/lib64
      DOC "Fully qualified library path of ${name}"
    )
  elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    find_library(${name}_LIB ${name}
      PATHS "C:/Program Files/Microsoft SDKs/Windows/v7.0/Lib"
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
endfunction(use_system_library)

function(use_third_party_cmake name)
  log_info("use_third_party_cmake: Searching libraries of ${name}")
  foreach(lib ${${name}_LIBS})
    if(NOT EXISTS ${lib})
      log_warning("${lib} library not found (yet)")
    else()
      log_info("use_third_party_cmake: Using library ${lib}")
    endif()

    target_link_libraries(${PROJECT_NAME}
      ${lib}
    )
  endforeach()

  add_dependencies(${PROJECT_NAME} ${name})
endfunction(use_third_party_cmake)