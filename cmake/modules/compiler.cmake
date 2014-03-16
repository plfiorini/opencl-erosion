include(logging)
include(build_functions)

#
# figure out gcc version
#
if(CMAKE_CXX_COMPILER)
  execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion 
    OUTPUT_VARIABLE gcc_version_raw 
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  # extract first two numbers, separated by a dot (e.g. 4.3)
  string(REGEX REPLACE "([0-9+])\\.([0-9]+).*" "\\1" GCC_MAJOR "${gcc_version_raw}")
  string(REGEX REPLACE "([0-9+])\\.([0-9]+).*" "\\2" GCC_MINOR "${gcc_version_raw}")

  # set legacy variable for backward compatibility
  set(GCC_VERSION "${GCC_MAJOR}.${GCC_MINOR}" CACHE STRING "Used major and minor version of gcc")
  
  add_cxx_flags("-Wall")
  if ( "${GCC_VERSION}" STREQUAL "4.6" )
    force_cxx_std("c++0x")
  elseif ( ${GCC_MAJOR} EQUAL 4 AND NOT ( ${GCC_MINOR} LESS 7 ) )
    force_cxx_std("c++11")
  endif()

  execute_process(COMMAND ${CMAKE_CXX_COMPILER} "--version"
    OUTPUT_VARIABLE cxx_compiler_version
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  string(REGEX REPLACE "\\\n.*" "" cxx_compiler_version "${cxx_compiler_version}")
  set(CXX_COMPILER_VERSION "${cxx_compiler_version}" CACHE STRING "Full version string of the used c++ compiler" FORCE)

  log_info("compiler.cmake: Using cxx-compiler version ${CXX_COMPILER_VERSION}")

else()
  log_warning("compiler.cmake: CMAKE_CXX_COMPILER not defined!")
endif()

if(CMAKE_C_COMPILER)
  execute_process(COMMAND ${CMAKE_C_COMPILER} "--version"
    OUTPUT_VARIABLE c_compiler_version
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  string(REGEX REPLACE "\\\n.*" "" c_compiler_version "${c_compiler_version}")
  set(C_COMPILER_VERSION "${c_compiler_version}" CACHE STRING "Full version string of the used c compiler" FORCE)
  log_info("compiler.cmake: Using c-compiler version ${C_COMPILER_VERSION}")

else()
  log_warning("compiler.cmake: CMAKE_C_COMPILER not defined!")
endif()
