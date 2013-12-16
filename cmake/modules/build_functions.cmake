include(CMakeParseArguments)

include(versioning)
include(installation)

#
# This function generates a makefile code for a standard static & shared library
#
# The ${PROJECT_NAME} is used to derive several variables.
# Additional parameters ${ARGN} specify libraries, which get linked.
#
function(add_standard_lib)
  private_add_standard_lib("STATIC" ${ARGN})
endfunction(add_standard_lib)

#
# This function generates a makefile code for a standard IDS library
#
# The ${PROJECT_NAME} is used to derive several variables.
# library_type specifies if a shared or a static library shall be built.
#
# CMake Parameters:
#  PKG - When the PKG parameter is given the tool will be added to the specified package
#
# Additional parameters ${ARGN} specify libraries, which get linked.
#
function(private_add_standard_lib library_type)
  cmake_parse_arguments(INSTALL "" "PKG" "" ${ARGN})
  # if specified INSTALL_PKG holds the tools installation package name
  # unrecognized parameters can be found in INSTALL_UNPARSED_ARGUMENTS

  # The source files
  file(GLOB ${PROJECT_NAME}_srcs "source/*.cpp" "source/*.C" "source/*.c")

  # Path for internal header files
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/source")

  # generate version info and create dependency of lib
  add_version_info()

  add_library(${PROJECT_NAME} ${library_type}
    ${${PROJECT_NAME}_srcs}
  )

  # Link other libraries
  target_link_libraries(${PROJECT_NAME} 
    ${INSTALL_UNPARSED_ARGUMENTS}
  )

  # Handle the test cases
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test")
    add_subdirectory(test)
  endif()

  # Handle the example code
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples")
    add_subdirectory(examples)
  endif()

  # Installation instructions for the header files
  install_headers()
  install_library(${library_type})

endfunction(private_add_standard_lib)

#
# This function generates a makefile code for a standard IDS tool
#
# The ${PROJECT_NAME} is used to derive several variables
#
# CMake Parameters:
#  PKG - When the PKG parameter is given the tool will be added to the specified package
#  TYPE - Specifies if the tool gets installed to bin or sbin. 
#         Accepted values: "sbin" for sbin, everything else results in bin
# Additional parameters ${ARGN} specify libraries, which get linked.
#
# e.g.: add_standard_tool(PKG core common ldb)
#       will put the tool ${PROJECT_NAME} into the core package
#       common & ldb will be used as link libraries
#
function(add_standard_executable)
  cmake_parse_arguments(INSTALL "" "PKG;TYPE" "" ${ARGN})
  # if specified INSTALL_PKG holds the tools installation package name
  # unrecognized parameters can be found in INSTALL_UNPARSED_ARGUMENTS

  # The source files
  file(GLOB ${PROJECT_NAME}_srcs "source/*.cpp" "source/*.C" "source/*.c")

  # Path for internal header files
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/source")

  # generate version info and create dependency of lib
  add_version_info()

  # The binary of the tool
  add_executable(${PROJECT_NAME} 
    ${${PROJECT_NAME}_srcs}
  )

  # Link libraries
  target_link_libraries(${PROJECT_NAME} 
    ${INSTALL_UNPARSED_ARGUMENTS}
  )

  # Installation instructions for the library
  if( "${INSTALL_TYPE}" STREQUAL "sbin")
    install_special_binary()
  else()
    install_binary()
  endif()

endfunction(add_standard_executable)

#
# This function just adds the given string to the CXX flag
#
# Remarks: don't use this function if you don't know what you are doing
#
function(add_c_flags flags)
  log_info("Adding '${flags}' to existing CMAKE_C_FLAGS: '${CMAKE_C_FLAGS}'")

  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${flags}" PARENT_SCOPE)
endfunction(add_c_flags)

#
# This function just adds the given string to the CXX flag
#
# Remarks: don't use this function if you don't know what you are doing
#
function(add_cxx_flags flags)
  log_info("Adding '${flags}' to existing CMAKE_CXX_FLAGS: '${CMAKE_CXX_FLAGS}'")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flags}" PARENT_SCOPE)
endfunction(add_cxx_flags)

#
# This function modifies the optimization level in the CXX and CC flags
# Supported levels: 0, 1, 2, 3, s, fast
#
# Remarks: Expects PROJECT_NAME to be set for logging
#
function(force_optimization level)
  if(${level} STREQUAL "s")
    set(checked_level ${level})
  elseif(${level} STREQUAL "fast")
    set(checked_level ${level})
  elseif(${level} GREATER -1 AND ${level} LESS 4)
    set(checked_level ${level})
  else()
    log_fatal_error("Used wrong optimization level: ${level} on ${PROJECT_NAME}")
  endif()

  log_info("Forcing optimization level -O${checked_level}")

  string(REGEX REPLACE "\\-O|\\-O.|\\-Ofast" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  string(REGEX REPLACE "\\-O|\\-O.|\\-Ofast" "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O${checked_level}" PARENT_SCOPE)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O${checked_level}" PARENT_SCOPE)
endfunction(force_optimization)

#
# This function forces a specific standard in the compilers C flags
# given by the argument "standard_name"
#
# Remarks: Expects PROJECT_NAME to be set for logging
#
function(force_c_std standard_name)
  log_info("Forcing C standard -std=${standard_name}")

  string(REGEX REPLACE "\\-std=[a-zA-Z0-9+:]+" "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=${standard_name}" PARENT_SCOPE)
endfunction(force_c_std)

#
# This function forces a specific standard in the compilers CXX flags
# given by the argument "standard_name"
#
# Remarks: Expects PROJECT_NAME to be set for logging
#
function(force_cxx_std standard_name)
  log_info("Forcing CXX standard -std=${standard_name}")

  string(REGEX REPLACE "\\-std=[a-zA-Z0-9+:]+" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=${standard_name}" PARENT_SCOPE)
endfunction(force_cxx_std)
